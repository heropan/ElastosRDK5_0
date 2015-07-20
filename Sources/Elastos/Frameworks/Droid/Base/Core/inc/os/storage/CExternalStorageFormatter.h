
#ifndef __CEXTERNALSTORAGEFORMATTER_H__
#define __CEXTERNALSTORAGEFORMATTER_H__

#include "app/Service.h"
#include "_CExternalStorageFormatter.h"

namespace Elastos {
namespace Droid {
namespace Os {
namespace Storage {

CarClass(CExternalStorageFormatter), public Elastos::Droid::App::Service
{
public:
    CARAPI OnCreate();

    CARAPI OnStartCommand(
        /* [in] */ Intent intent,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 startId
        /* [out] */ Int32* pCmd);

    CARAPI OnDestroy();

    CARAPI OnBind(
        /* [in] */ IIntent* intent
        /* [out] */ IBinder** pResult);

    CARAPI OnCancel(
        /* [in] */ DialogInterface dialog);

    CARAPI Fail(
        /* [in] */ Int32 msg);

    CARAPI TryUnmountExternalStorage2();

    CARAPI UpdateProgressState(
        /* [in] */ const String& path,
        /* [in] */ const String& newState);

    CARAPI UpdateProgressDialog(
        /* [in] */ Int32 msg);

    CARAPI GetMountService(
        /* [out] */ IMountService** pResult);

public:
    static const String TAG;//= "ExternalStorageFormatter";
    static const String FORMAT_ONLY;//= "com.android.internal.os.storage.FORMAT_ONLY";
    static const String FORMAT_AND_FACTORY_RESET;//= "com.android.internal.os.storage.FORMAT_AND_FACTORY_RESET";

    static const String EXTRA_ALWAYS_RESET;//= "always_reset";

    static const ComponentName COMPONENT_NAME;//= new ComponentName("android", ExternalStorageFormatter.class.getName());

    AutoPtr<IStorageEventListener> mStorageListenerl;

private:
    // If non-NULL, the volume to format. Otherwise, will use the default external storage directory
    AutoPtr<IStorageVolume> mStorageVolume;

    // Access using getMountService()
    AutoPtr<IMountService> mMountService;

    AutoPtr<IStorageManager> mStorageManager;

    Object mWakeLock;

    AutoPtr<ProgressDialog> mProgressDialog;//= NULL;

    Boolean mFactoryReset = FALSE;
    Boolean mAlwaysReset = FALSE;

    // If !true, v-sdcard in enabled
    Boolean mTryMountExtStorage2 = FALSE;
    static Boolean mExternalStoragebeSD = TRUE;
};

} // namespace Storage
} // namespace Os
} // namepsace Droid
} // namespace Elastos

#endif // __CEXTERNALSTORAGEFORMATTER_H__
