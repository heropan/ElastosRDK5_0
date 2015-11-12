
#ifndef __ELASTOS_DROID_APP_NOTIFICATIONSTYLE_H__
#define __ELASTOS_DROID_APP_NOTIFICATIONSTYLE_H__

#include "elastos/droid/ext/frameworkext.h"
#include <elastos/core/Object.h>

using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::Pm::IIPackageInstallObserver2;
using Elastos::Droid::Os::IBundle;

namespace Elastos {
namespace Droid {
namespace App {

/** {@hide} */
class PackageInstallObserver
    : public Object
    , public IPackageInstallObserver
{
    // private final IPackageInstallObserver2.Stub mBinder = new IPackageInstallObserver2.Stub() {
    //     @Override
    //     public void onUserActionRequired(Intent intent) {
    //         PackageInstallObserver.this.onUserActionRequired(intent);
    //     }

    //     @Override
    //     public void onPackageInstalled(String basePackageName, int returnCode,
    //             String msg, Bundle extras) {
    //         PackageInstallObserver.this.onPackageInstalled(basePackageName, returnCode, msg,
    //                 extras);
    //     }
    // };

public:
    CAR_INTERFACE_DECL()

    virtual ~PackageInstallObserver();

    /** {@hide} */
    CARAPI GetBinder(
        /* [out] */ IIPackageInstallObserver2** pio);

    CARAPI OnUserActionRequired(
        /* [in] */ IIntent* intent);

    /**
     * This method will be called to report the result of the package
     * installation attempt.
     *
     * @param basePackageName Name of the package whose installation was
     *            attempted
     * @param extras If non-null, this Bundle contains extras providing
     *            additional information about an install failure. See
     *            {@link android.content.pm.PackageManager} for documentation
     *            about which extras apply to various failures; in particular
     *            the strings named EXTRA_FAILURE_*.
     * @param returnCode The numeric success or failure code indicating the
     *            basic outcome
     * @hide
     */
    CARAPI OnPackageInstalled(
        /* [in] */ const String& basePackageName,
        /* [in] */ Int32 returnCode,
        /* [in] */ const String& msg,
        /* [in] */ IBundle* extras);

private:
    AutoPtr<IIPackageInstallObserver2> mBinder;
};

} // namespace App
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_CONTENT_PM_PACKAGEMANAGER_H__

