#include "elastos/droid/server/PersistentDataBlockService.h"

#include <elastos/droid/R.h>
#include <elastos/droid/Manifest.h>

using Elastos::Droid::R;
using Elastos::Droid::Manifest;


namespace Elastos {
namespace Droid {
namespace Server {

//===================================================================================
// PersistentDataBlockService::BinderService
//===================================================================================
class PersistentDataBlockService
    : public SystemService
{
public:

    class BinderService
        : public Object
        , public IIPersistentDataBlockService
        , public IBinder
    {
    public:
        CAR_INTERFACE_DECL()

        //@Override
        CARAPI Write(
            /* [in] */ ArrayOf<Byte>* data,
            /* [out] */ Int32* result)
        {
            FAIL_RETURN(EnforceUid(Binder::GetCallingUid()))

            // Need to ensure we don't write over the last Byte
            Int64 maxBlockSize = GetBlockDeviceSize() - HEADER_SIZE - 1;
            if (data.length > maxBlockSize) {
                // partition is ~500k so shouldn't be a problem to downcast
                return (Int32) -maxBlockSize;
            }

            DataOutputStream outputStream;
            try {
                outputStream = new DataOutputStream(new FileOutputStream(new File(mDataBlockFile)));
            } catch (FileNotFoundException e) {
                Slogger::E(TAG, "partition not available?", e);
                return -1;
            }

            ByteBuffer headerAndData = ByteBuffer->Allocate(data.length + HEADER_SIZE);
            headerAndData->PutInt(PARTITION_TYPE_MARKER);
            headerAndData->PutInt(data.length);
            headerAndData->Put(data);

            try {
                Synchronized(mLock) {
                    outputStream->Write(headerAndData->Array());
                    return data.length;
                }
            } catch (IOException e) {
                Slogger::E(TAG, "failed writing to the persistent data block", e);
                return -1;
            } finally {
                try {
                    outputStream->Close();
                } catch (IOException e) {
                    Slogger::E(TAG, "failed closing output stream", e);
                }
            }
        }

        //@Override
        CARAPI Read(
            /* [out, callee] */ ArrayOf<Byte>** data)
        {
            VALIDATE_NOT_NULL(data)
            *data = NULL;

            FAIL_RETURN(EnforceUid(Binder::GetCallingUid()))

            DataInputStream inputStream;
            try {
                inputStream = new DataInputStream(new FileInputStream(new File(mDataBlockFile)));
            } catch (FileNotFoundException e) {
                Slogger::E(TAG, "partition not available?", e);
                return NULL;
            }

            try {
                Synchronized(mLock) {
                    Int32 totalDataSize = GetTotalDataSizeLocked(inputStream);

                    if (totalDataSize == 0) {
                        return new Byte[0];
                    }

                    Byte[] data = new Byte[totalDataSize];
                    Int32 read = inputStream->Read(data, 0, totalDataSize);
                    if (read < totalDataSize) {
                        // something went wrong, not returning potentially corrupt data
                        Slogger::E(TAG, "failed to read entire data block. bytes read: " +
                                read + "/" + totalDataSize);
                        return NULL;
                    }
                    return data;
                }
            } catch (IOException e) {
                Slogger::E(TAG, "failed to read data", e);
                return NULL;
            } finally {
                try {
                    inputStream->Close();
                } catch (IOException e) {
                    Slogger::E(TAG, "failed to close OutputStream");
                }
            }
        }

        //@Override
        CARAPI Wipe()
        {
            FAIL_RETURN(EnforceOemUnlockPermission())

            Synchronized(mLock) {
                Int32 ret = NativeWipe(mDataBlockFile);

                if (ret < 0) {
                    Slogger::E(TAG, "failed to wipe persistent partition");
                }
            }
        }

        //@Override
        CARAPI SetOemUnlockEnabled(
            /* [in] */ Boolean enabled)
        {
            // do not allow monkey to flip the flag
            if (ActivityManager->IsUserAMonkey()) {
                return;
            }
            FAIL_RETURN(EnforceOemUnlockPermission())
            FileOutputStream outputStream;
            try {
                outputStream = new FileOutputStream(new File(mDataBlockFile));
            } catch (FileNotFoundException e) {
                Slogger::E(TAG, "parition not available", e);
                return;
            }

            try {
                FileChannel channel = outputStream->GetChannel();

                channel->Position(GetBlockDeviceSize() - 1);

                ByteBuffer data = ByteBuffer->Allocate(1);
                data->Put(enabled ? (Byte) 1 : (Byte) 0);
                data->Flip();

                Synchronized(mOemLock) {
                    channel->Write(data);
                }
            } catch (IOException e) {
                Slogger::E(TAG, "unable to access persistent partition", e);
            } finally {
                IoUtils->CloseQuietly(outputStream);
            }
        }

        //@Override
        CARAPI GetOemUnlockEnabled(
            /* [out] */ Boolean* result)
        {
            VALIDATE_NOT_NULL(result)
            *result = FALSE;

            FAIL_RETURN(EnforceOemUnlockPermission())
            DataInputStream inputStream;
            try {
                inputStream = new DataInputStream(new FileInputStream(new File(mDataBlockFile)));
            } catch (FileNotFoundException e) {
                Slogger::E(TAG, "partition not available");
                return FALSE;
            }

            try {
                inputStream->Skip(GetBlockDeviceSize() - 1);
                Synchronized(mOemLock) {
                    return inputStream->ReadByte() != 0;
                }
            } catch (IOException e) {
                Slogger::E(TAG, "unable to access persistent partition", e);
                return FALSE;
            } finally {
                IoUtils->CloseQuietly(inputStream);
            }
        }

        //@Override
        CARAPI GetDataBlockSize(
            /* [out] */ Int32* result)
        {
            FAIL_RETURN(EnforceUid(Binder::GetCallingUid()))

            DataInputStream inputStream;
            try {
                inputStream = new DataInputStream(new FileInputStream(new File(mDataBlockFile)));
            } catch (FileNotFoundException e) {
                Slogger::E(TAG, "partition not available");
                return 0;
            }

            try {
                Synchronized(mLock) {
                    return GetTotalDataSizeLocked(inputStream);
                }
            } catch (IOException e) {
                Slogger::E(TAG, "error reading data block size");
                return 0;
            } finally {
                IoUtils->CloseQuietly(inputStream);
            }
        }

        //@Override
        CARAPI GetMaximumDataBlockSize(
            /* [out] */ Int64* result)
        {
            Int64 actualSize = GetBlockDeviceSize() - HEADER_SIZE - 1;
            return actualSize <= MAX_DATA_BLOCK_SIZE ? actualSize : MAX_DATA_BLOCK_SIZE;
        }


        CARAPI ToString(
            /* [out] */ String* str);

//===================================================================================
// PersistentDataBlockService
//===================================================================================
    PersistentDataBlockService();

    ~PersistentDataBlockService();

    CARAPI constructor(
        /* [in] */ IContext* context)
    {
        SystemService::constructor(context);

        mContext = context;
        mDataBlockFile = SystemProperties->Get(PERSISTENT_DATA_BLOCK_PROP);
        mBlockDeviceSize = -1; // Load lazily
        mAllowedAppId = GetAllowedAppId(UserHandle.USER_OWNER);
    }

    //@Override
    CARAPI OnStart()
    {
        PublishBinderService(Context.PERSISTENT_DATA_BLOCK_SERVICE, mService);
    }

private:
    Int32 GetAllowedAppId(
        /* [in] */ Int32 userHandle)
    {
        String allowedPackage = mContext->GetResources()
                .GetString(R.string.config_persistentDataPackageName);
        PackageManager pm = mContext->GetPackageManager();
        Int32 allowedUid = -1;
        try {
            allowedUid = pm->GetPackageUid(allowedPackage, userHandle);
        } catch (PackageManager.NameNotFoundException e) {
            // not expected
            Slogger::E(TAG, "not able to find package " + allowedPackage, e);
        }
        return UserHandle->GetAppId(allowedUid);
    }


    void EnforceOemUnlockPermission()
    {
        mContext->EnforceCallingOrSelfPermission(
                Manifest.permission.OEM_UNLOCK_STATE,
                "Can't access OEM unlock state");
    }

    void EnforceUid(
        /* [in] */ Int32 callingUid)
    {
        if (UserHandle->GetAppId(callingUid) != mAllowedAppId) {
            throw new SecurityException("uid " + callingUid + " not allowed to access PST");
        }
    }

    Int32 GetTotalDataSizeLocked(
        /* [in] */ IDataInputStream* inputStream)
    {
        Int32 totalDataSize;
        Int32 blockId = inputStream->ReadInt();
        if (blockId == PARTITION_TYPE_MARKER) {
            totalDataSize = inputStream->ReadInt();
        } else {
            totalDataSize = 0;
        }
        return totalDataSize;
    }

    Int64 GetBlockDeviceSize()
    {
        Synchronized(mLock) {
            if (mBlockDeviceSize == -1) {
                mBlockDeviceSize = NativeGetBlockDeviceSize(mDataBlockFile);
            }
        }

        return mBlockDeviceSize;
    }

    Int64 NativeGetBlockDeviceSize(
        /* [in] */ const String& path);

    Int32 NativeWipe(
        /* [in] */ const String& path);

private:
    static const String TAG = "PersistentDataBlockService";

    static const String PERSISTENT_DATA_BLOCK_PROP = "ro.frp.pst";
    static const Int32 HEADER_SIZE = 8;
    // Magic number to mark block device as adhering to the format consumed by this service
    static const Int32 PARTITION_TYPE_MARKER = 0x1990;
    // Limit to 100k as blocks larger than this might cause strain on Binder.
    // TODO(anmorales): Consider splitting up too-large blocks in PersistentDataBlockManager
    static const Int32 MAX_DATA_BLOCK_SIZE = 1024 * 100;

    AutoPtr<IContext> mContext;
    String mDataBlockFile;
    Object mLock;

    Int32 mAllowedAppId = -1;
    /*
     * Separate lock for OEM unlock related operations as they can happen in parallel with regular
     * block operations.
     */
    Object mOemLock;

    Int64 mBlockDeviceSize;

    AutoPtr<IBinder> mService;
};

}// Server
}// Droid
}// Elastos
