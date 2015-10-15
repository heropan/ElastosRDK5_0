
#ifndef __ELASTOS_DROID_INTERNAL_OS_STORAGE_CEXTERNALSTORAGEFORMATTER_H__
#define __ELASTOS_DROID_INTERNAL_OS_STORAGE_CEXTERNALSTORAGEFORMATTER_H__

#include "_Elastos_Droid_Internal_Os_Storage_CExternalStorageFormatter.h"
#include "app/Service.h"
#include "os/storage/StorageEventListener.h"
#include <elastos/core/Thread.h>

using Elastos::Droid::App::Service;
using Elastos::Droid::App::IProgressDialog;
using Elastos::Droid::Content::IComponentName;
using Elastos::Droid::Content::IDialogInterface;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Os::IBinder;
using Elastos::Droid::Os::IMountService;
using Elastos::Droid::Os::IPowerManagerWakeLock;
using Elastos::Droid::Os::IStorageVolume;
using Elastos::Droid::Os::IStorageManager;
using Elastos::Droid::Os::Storage::StorageEventListener;
using Elastos::Core::Thread;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Os {
namespace Storage {

CarClass(CExternalStorageFormatter)
    , public Service
    , public IExternalStorageFormatter
{
private:
    class MyStorageEventListener : public StorageEventListener
    {
    public:
        MyStorageEventListener(
            /* [in] */ CExternalStorageFormatter* owner);

        CARAPI OnStorageStateChanged(
            /* [in] */ const String& path,
            /* [in] */ const String& oldState,
            /* [in] */ const String& newState);

    private:
        CExternalStorageFormatter* mOwner;
    };

    class MyThread : public Thread
    {
    public:
        MyThread(
            /* [in] */ CExternalStorageFormatter* owner);

        CARAPI Run();

    private:
        CExternalStorageFormatter* mOwner;
    };

public:
    CExternalStorageFormatter();

    CAR_OBJECT_DECL()

    CAR_INTERFACE_DECL()

    CARAPI OnCreate();

    CARAPI OnStartCommand(
        /* [in] */ IIntent* intent,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 startId
        /* [out] */ Int32* cmd);

    CARAPI OnDestroy();

    CARAPI OnBind(
        /* [in] */ IIntent* intent
        /* [out] */ IBinder** result);

    CARAPI OnCancel(
        /* [in] */ IDialogInterface* dialog);

    CARAPI UpdateProgressDialog(
        /* [in] */ Int32 msg);

    CARAPI_(AutoPtr<IMountService>) GetMountService();

protected:
    CARAPI_(void) Fail(
        /* [in] */ Int32 msg);

    CARAPI_(void) UpdateProgressState(
        /* [in] */ const String& path,
        /* [in] */ const String& newState);

public:
    static const String TAG;
    static const AutoPtr<IComponentName> COMPONENT_NAME;

    AutoPtr<IStorageEventListener> mStorageListener;

private:
    // If non-NULL, the volume to format. Otherwise, will use the default external storage directory
    AutoPtr<IStorageVolume> mStorageVolume;

    // Access using getMountService()
    AutoPtr<IMountService> mMountService;

    AutoPtr<IStorageManager> mStorageManager;

    AutoPtr<IPowerManagerWakeLock> mWakeLock;

    AutoPtr<IProgressDialog> mProgressDialog;

    Boolean mFactoryReset;
    Boolean mAlwaysReset;
    String mReason;
};

} // namespace Storage
} // namespace Os
} // namespace Internal
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_INTERNAL_OS_STORAGE_CEXTERNALSTORAGEFORMATTER_H__
