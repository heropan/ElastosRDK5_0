
#include "ext/frameworkdef.h"
#include "app/CActivityThreadHelper.h"
#include "app/CActivityThread.h"

namespace Elastos {
namespace Droid {
namespace App {

ECode CActivityThreadHelper::GetCurrentActivityThread(
    /* [out] */ IActivityThread** thread)
{
    VALIDATE_NOT_NULL(thread);
    AutoPtr<IActivityThread> thr = CActivityThread::GetCurrentActivityThread();
    *thread = thr;
    INTERFACE_ADDREF(*thread);
    return NOERROR;
}

ECode CActivityThreadHelper::GetCurrentPackageName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = NULL;

    AutoPtr<IActivityThread> at = CActivityThread::GetCurrentActivityThread();
    if (at != NULL) {
        CActivityThread* am = (CActivityThread*)at.Get();
        if (am->mBoundApplication = NULL) {
            *name = am->mBoundApplication->mProcessName;
        }
    }

    return NOERROR;
}

ECode CActivityThreadHelper::GetCurrentApplication(
    /* [out] */ IApplication** app)
{
    VALIDATE_NOT_NULL(app);
    *app = NULL;

    AutoPtr<IActivityThread> at = CActivityThread::GetCurrentActivityThread();
    if (at != NULL) {
        CActivityThread* am = (CActivityThread*)at.Get();
        if (am->mInitialApplication = NULL) {
            *app = am->mInitialApplication;
            INTERFACE_ADDREF(*app);
        }
    }

    return NOERROR;
}

ECode CActivityThreadHelper::GetPackageManager(
    /* [out] */ IIPackageManager** pkgManager)
{
    VALIDATE_NOT_NULL(pkgManager);
    AutoPtr<IIPackageManager> pm = CActivityThread::GetPackageManager();
    *pkgManager = pm;
    INTERFACE_ADDREF(*pkgManager);
    return NOERROR;
}

/**
 * Return the Intent that's currently being handled by a
 * BroadcastReceiver on this thread, or null if none.
 * @hide
 */
ECode CActivityThreadHelper::GetIntentBeingBroadcast(
    /* [out] */ IIntent** intent)
{
    AutoPtr<IIntent> curIntent = (IIntent*)pthread_getspecific(CActivityThread::sCurrentBroadcastIntentKey);
    *intent = curIntent;
    INTERFACE_ADDREF(*intent);
    return NOERROR;
}

ECode CActivityThreadHelper::GetSystemMain(
    /* [out] */ IActivityThread** thread)
{
    VALIDATE_NOT_NULL(thread);
    AutoPtr<IActivityThread> thr = CActivityThread::GetSystemMain();
    *thread = thr;
    INTERFACE_ADDREF(*thread);
    return NOERROR;
}

ECode CActivityThreadHelper::Main(
    /* [in] */ const ArrayOf<String>& args)
{
    return CActivityThread::Main(args);
}

} // namespace App
} // namespace Droid
} // namespace Elastos
