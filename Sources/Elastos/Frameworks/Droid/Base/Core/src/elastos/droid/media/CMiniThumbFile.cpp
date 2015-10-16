
#include "elastos/droid/media/CMiniThumbFile.h"

#include <elastos/utility/logging/Logger.h>
#include "elastos/droid/net/CUriHelper.h"
#include "elastos/droid/os/Environment.h"
#include <elastos/core/StringUtils.h>

using Elastos::Core::StringUtils;
using Elastos::IO::IByteBufferHelper;
using Elastos::IO::CByteBufferHelper;
using Elastos::IO::Channels::IFileLock;
using Elastos::Utility::Logging::Logger;
using Elastos::Droid::Net::IUriHelper;
using Elastos::Droid::Net::CUriHelper;
using Elastos::Droid::Os::Environment;
using Elastos::IO::CFile;
using Elastos::IO::CRandomAccessFile;

namespace Elastos {
namespace Droid {
namespace Media {

/*static*/ const String CMiniThumbFile::TAG("MiniThumbFile");

/*static*/ HashMap< String, AutoPtr<IMiniThumbFile> > CMiniThumbFile::sThumbFiles;

/*static*/ Object CMiniThumbFile::mStaticThislock;

const Int32 CMiniThumbFile::MINI_THUMB_DATA_FILE_VERSION = 3;
const Int32 CMiniThumbFile::HEADER_SIZE = 1 + 8 + 4;

CMiniThumbFile::CMiniThumbFile()
{}

ECode CMiniThumbFile::constructor(
    /* [in] */ IUri* uri)
{
    mUri = uri;
    AutoPtr<IByteBufferHelper> byteBufferHelper;
    CByteBufferHelper::AcquireSingleton((IByteBufferHelper**)&byteBufferHelper);
    byteBufferHelper->AllocateDirect(BYTES_PER_MINTHUMB, (IByteBuffer**)&mBuffer);
    return NOERROR;
}

/*synchronized*/
ECode CMiniThumbFile::Deactivate()
{
    AutoLock lock(mThislock);

    if (mMiniThumbFile != NULL) {
//        try {
            mMiniThumbFile->Close();
            mMiniThumbFile = NULL;
//        } catch (IOException ex) {
//            // ignore exception
//        }
    }
    return NOERROR;
}

/*synchronized*/
ECode CMiniThumbFile::GetMagic(
    /* [in] */ Int64 id,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);

    AutoLock lock(mThislock);
    // check the mini thumb file for the right data.  Right is
    // defined as having the right magic number at the offset
    // reserved for this "id".
    AutoPtr<IRandomAccessFile> r = MiniThumbDataFile();
    if (r != NULL) {
        Int64 pos = id * BYTES_PER_MINTHUMB;
        AutoPtr<IFileLock> fileLock = NULL;
//        try {
            mBuffer->Clear();
            mBuffer->SetLimit(1 + 8);

            mChannel->Lock(pos, 1 + 8, TRUE, (IFileLock**)&fileLock);
            // check that we can read the following 9 bytes
            // (1 for the "status" and 8 for the long)
            Int32 tempValue1;
            mChannel->ReadByteBuffer(mBuffer, pos, &tempValue1);
            if (tempValue1 == 9) {
                mBuffer->SetPosition(0);
                Byte tempValue2;
                mBuffer->GetByte(&tempValue2);
                if (tempValue2 == 1) {
                    mBuffer->GetInt64(result);
                    return NOERROR;
                }
            }
//        } catch (IOException ex) {
//            Logger::V(TAG, "Got exception checking file magic: "/*, ex*/);
//        } catch (RuntimeException ex) {
//            // Other NIO related exception like disk full, read only channel..etc
//            Logger::E(TAG, String("Got exception when reading magic, id = ") + StringUtils::Int64ToString(id) +
//                    String(", disk full or mount read-only? ") /*+ ex.getClass()*/);
//        } finally {
//            try {
                if (fileLock != NULL) {
                    fileLock->ReleaseLock();
                }
//            }
//            catch (IOException ex) {
//                // ignore it.
//            }
//        }
    }
    *result = 0;
    return NOERROR;
}

/*synchronized*/
ECode CMiniThumbFile::SaveMiniThumbToFile( // throws IOException
    /* [in] */ ArrayOf<Byte>* data,
    /* [in] */ Int64 id,
    /* [in] */ Int64 magic)
{
    AutoLock lock(mThislock);

    AutoPtr<IRandomAccessFile> r = MiniThumbDataFile();
    if (r == NULL) {
        return NOERROR;
    }

    Int64 pos = id * BYTES_PER_MINTHUMB;
    AutoPtr<IFileLock> fileLock;
//    try {
        if (data != NULL) {
            if (data->GetLength() > BYTES_PER_MINTHUMB - HEADER_SIZE) {
                // not enough space to store it.
                return NOERROR;
            }
            mBuffer->Clear();
            mBuffer->PutByte((Byte) 1);
            mBuffer->PutInt64(magic);
            mBuffer->PutInt32(data->GetLength());
            mBuffer->PutBytes(*data);
            mBuffer->Flip();

            mChannel->Lock(pos, BYTES_PER_MINTHUMB, FALSE, (IFileLock**)&fileLock);
            Int32 tempValue;
            mChannel->WriteByteBuffer(mBuffer, pos, &tempValue);
        }
//    } catch (IOException ex) {
//        Logger::E(TAG, String("couldn't save mini thumbnail data for ")
//                + StringUtils::Int64ToString(id) + String("; ")/*, ex*/);
//        throw ex;
//    } catch (RuntimeException ex) {
//        // Other NIO related exception like disk full, read only channel..etc
//        Logger::E(TAG, String("couldn't save mini thumbnail data for ")
//                + StringUtils::Int64ToString(id) + String("; disk full or mount read-only? ") /*+ ex.getClass()*/);
//    } finally {
//        try {
            if (fileLock != NULL) {
                fileLock->ReleaseLock();
            }
//        }
//        catch (IOException ex) {
//            // ignore it.
//        }
//    }
    return NOERROR;
}

/*synchronized*/
ECode CMiniThumbFile::GetMiniThumbFromFile(
    /* [in] */ Int64 id,
    /* [in] */ ArrayOf<Byte>* data,
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    VALIDATE_NOT_NULL(result);
    *result = NULL;

    AutoLock lock(mThislock);
    AutoPtr<IRandomAccessFile> r = MiniThumbDataFile();
    if (r == NULL) {
        return NOERROR;
    }

    Int64 pos = id * BYTES_PER_MINTHUMB;
    AutoPtr<IFileLock> fileLock = NULL;
//    try {
        mBuffer->Clear();
        mChannel->Lock(pos, BYTES_PER_MINTHUMB, TRUE, (IFileLock**)&fileLock);
        Int32 size;
        mChannel->ReadByteBuffer(mBuffer, pos, &size);
        if (size > 1 + 8 + 4) { // flag, magic, length
            mBuffer->SetPosition(0);
            Byte flag;
            mBuffer->GetByte(&flag);
            Int64 magic;
            mBuffer->GetInt64(&magic);
            Int32 length;
            mBuffer->GetInt32(&length);

            if (size >= 1 + 8 + 4 + length && data->GetLength() >= length) {
                mBuffer->GetBytes(data, 0, length);
                *result = data;
            }
        }
//    } catch (IOException ex) {
//        Logger::W(TAG, String("got exception when reading thumbnail id=") + StringUtils::Int64ToString(id) + String(", exception: ") /*+ ex*/);
//    } catch (RuntimeException ex) {
//        // Other NIO related exception like disk full, read only channel..etc
//        Logger::W(TAG, String("Got exception when reading thumbnail, id = ") + StringUtils::Int64ToString(id) +
//                String(", disk full or mount read-only? ") /*+ ex.getClass()*/);
//    } finally {
//        try {
            if (fileLock != NULL) {
                fileLock->ReleaseLock();
            }
//        }
//        catch (IOException ex) {
//            // ignore it.
//        }
//    }
    REFCOUNT_ADD(*result);
    return NOERROR;
}

/*static synchronized*/
ECode CMiniThumbFile::Reset()
{
    AutoLock lock(mStaticThislock);

    AutoPtr<IMiniThumbFile> file;
    HashMap< String, AutoPtr<IMiniThumbFile> >::Iterator it = sThumbFiles.Begin();

    for (; it != sThumbFiles.End(); ++it) {
        file = it->mSecond;
        file->Deactivate();
    }
    sThumbFiles.Clear();
    return NOERROR;
}

/*static synchronized*/
ECode CMiniThumbFile::Instance(
    /* [in] */ IUri* uri,
    /* [out] */ IMiniThumbFile** result)
{
    VALIDATE_NOT_NULL(result);

    AutoLock lock(mStaticThislock);

    AutoPtr< ArrayOf<String> > tempMatrix;
    uri->GetPathSegments((ArrayOf<String>**)&tempMatrix);
    String type = (*tempMatrix)[1];

    HashMap< String, AutoPtr<IMiniThumbFile> >::Iterator it = sThumbFiles.Find(type);
    AutoPtr<IMiniThumbFile> file = it->mSecond;
    // Logger::V(TAG, String("get minithumbfile for type: ") + type);
    if (file == NULL) {
        AutoPtr<IUriHelper> uriHelper;
        CUriHelper::AcquireSingleton((IUriHelper**)&uriHelper);
        AutoPtr<IUri> tempUri;
        uriHelper->Parse(String("content://media/external/") + type + String("/media"), (IUri**)&tempUri);
        AutoPtr<CMiniThumbFile> mtf;
        CMiniThumbFile::NewByFriend(tempUri, (CMiniThumbFile**)&mtf);
        file = (CMiniThumbFile*)mtf.Get();
        sThumbFiles[type] = file;
    }

    *result = file;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

String CMiniThumbFile::RandomAccessFilePath(
    /* [in] */ Int32 version)
{
    String tempText;
    AutoPtr<IFile> file = Environment::GetExternalStorageDirectory();
    file->ToString(&tempText);
    String directoryName = tempText + "/DCIM/.thumbnails";

    Int32 tempValue;
    mUri->GetHashCode(&tempValue);
    String result = directoryName + "/thumbdata" + StringUtils::Int32ToString(version) + "-" + StringUtils::Int32ToString(tempValue);
    return result;
}

void CMiniThumbFile::RemoveOldFile()
{
    String oldPath = RandomAccessFilePath(MINI_THUMB_DATA_FILE_VERSION - 1);
    AutoPtr<IFile> oldFile ;
    CFile::New(oldPath,(IFile**)&oldFile);

    Boolean tempState;
    oldFile->Exists(&tempState);
    if (tempState) {
//        try {
            oldFile->Delete(&tempState);
//        } catch (SecurityException ex) {
//            // ignore
//        }
    }
}

AutoPtr<IRandomAccessFile> CMiniThumbFile::MiniThumbDataFile()
{
    if (mMiniThumbFile == NULL) {
        RemoveOldFile();
        String path = RandomAccessFilePath(MINI_THUMB_DATA_FILE_VERSION);

        AutoPtr<IFile> dir, directory;
        CFile::New(path, (IFile**)&dir);
        dir->GetParentFile((IFile**)&directory);

        Boolean tempState;
        directory->IsDirectory(&tempState);
        if (!tempState) {
            directory->Mkdirs(&tempState);
            if (!tempState) {
                String tempText;
                directory->ToString(&tempText);
                Logger::E(TAG, String("Unable to create .thumbnails directory ") + tempText);
            }
        }
        AutoPtr<IFile> f;
        CFile::New(path,(IFile**)&f);
        ECode ec;
//        try {
            ec = CRandomAccessFile::New(f, String("rw"), (IRandomAccessFile**)&mMiniThumbFile);
//        } catch (IOException ex) {
//            // Open as read-only so we can at least read the existing
//            // thumbnails.
//            try {
                if(FAILED(ec)) {
                    mMiniThumbFile = NULL;
                    CRandomAccessFile::New(f, String("r"), (IRandomAccessFile**)&mMiniThumbFile);
                }
//            } catch (IOException ex2) {
//                // ignore exception
//            }
//        }

        if (mMiniThumbFile != NULL) {
            mChannel = NULL;
            mMiniThumbFile->GetChannel((IFileChannel**)&mChannel);
        }
    }
    return mMiniThumbFile;
}

} // namespace Media
} // namepsace Droid
} // namespace Elastos
