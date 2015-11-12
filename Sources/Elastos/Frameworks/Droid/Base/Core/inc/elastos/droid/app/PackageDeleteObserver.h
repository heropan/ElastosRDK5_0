
#ifndef __ELASTOS_DROID_APP_PACKAGE_DELETE_OBSERVER_H__
#define __ELASTOS_DROID_APP_PACKAGE_DELETE_OBSERVER_H__

#include "elastos/droid/ext/frameworkext.h"

using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::Pm::IIPackageDeleteObserver2;

namespace Elastos {
namespace Droid {
namespace App {

/** {@hide} */
class PackageDeleteObserver
    : public Object
    , public IPackageDeleteObserver
{
    // private final IPackageDeleteObserver2.Stub mBinder = new IPackageDeleteObserver2.Stub() {
    //     @Override
    //     public void onUserActionRequired(Intent intent) {
    //         PackageDeleteObserver.this.onUserActionRequired(intent);
    //     }

    //     @Override
    //     public void onPackageDeleted(String basePackageName, int returnCode, String msg) {
    //         PackageDeleteObserver.this.onPackageDeleted(basePackageName, returnCode, msg);
    //     }
    // };

public:
    CAR_INTERFACE_DECL()

    virtual ~PackageDeleteObserver();

    /** {@hide} */
    CARAPI GetBinder(
        /* [out] */ IIPackageDeleteObserver2** pio);

    CARAPI OnUserActionRequired(
        /* [in] */ IIntent* intent);

    CARAPI OnPackageDeleted(
        /* [in] */ const String& basePackageName,
        /* [in] */ Int32 returnCode,
        /* [in] */ const String& msg);

private:
    AutoPtr<IIPackageDeleteObserver2> mBinder;
};

} // namespace App
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_APP_PACKAGE_DELETE_OBSERVER_H__

