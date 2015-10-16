#include "elastos/droid/ext/frameworkdef.h"
#include "elastos/droid/app/AppGlobals.h"
#ifdef DROID_CORE
#include "elastos/droid/app/CActivityThreadHelper.h"
#endif

namespace Elastos {
namespace Droid {
namespace App {

AutoPtr<IApplication> AppGlobals::GetInitialApplication()
{
    AutoPtr<IActivityThreadHelper> helper;
    CActivityThreadHelper::AcquireSingleton((IActivityThreadHelper**)&helper);
    AutoPtr<IApplication> application;
    helper->GetCurrentApplication((IApplication**)&application);
    return application;
}

String AppGlobals::GetInitialPackage()
{
    AutoPtr<IActivityThreadHelper> helper;
    CActivityThreadHelper::AcquireSingleton((IActivityThreadHelper**)&helper);
    String pkgName;
    helper->GetCurrentPackageName(&pkgName);
    return pkgName;
}

AutoPtr<IIPackageManager> AppGlobals::GetPackageManager()
{
    AutoPtr<IActivityThreadHelper> helper;
    CActivityThreadHelper::AcquireSingleton((IActivityThreadHelper**)&helper);
    AutoPtr<IIPackageManager> pm;
    helper->GetPackageManager((IIPackageManager**)&pm);
    return pm;
}

Int32 AppGlobals::GetIntCoreSetting(
    /* [in] */ const String& key,
    /* [in] */ Int32 defaultValue)
{
    AutoPtr<IActivityThreadHelper> helper;
    CActivityThreadHelper::AcquireSingleton((IActivityThreadHelper**)&helper);
    AutoPtr<IActivityThread> currentActivityThread;
    helper->GetCurrentActivityThread((IActivityThread**)&currentActivityThread);
    if (currentActivityThread != NULL) {
        Int32 value;
        currentActivityThread->GetIntCoreSetting(key, defaultValue, &value);
        return value;
    }
    else {
        return defaultValue;
    }
}

} // namespace App
} // namespace Droid
} // namespace Elastos
