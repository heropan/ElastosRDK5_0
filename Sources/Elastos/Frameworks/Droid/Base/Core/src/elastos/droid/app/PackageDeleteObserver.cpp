
#include "elastos/droid/app/PackageDeleteObserver.h"

namespace Elastos {
namespace Droid {
namespace App {

CAR_INTERFACE_IMPL(PackageDeleteObserver, Object, IPackageDeleteObserver)

PackageDeleteObserver::~PackageDeleteObserver()
{
    assert(0 && "TODO");
    // mBinder = new PackageDeleteObserver2();
}

ECode PackageDeleteObserver::GetBinder(
    /* [out] */ IIPackageDeleteObserver2** pio)
{
    VALIDATE_NOT_NULL(pio)
    *pio = mBinder;
    REFCOUNT_ADD(*pio)
    return NOERROR;
}

ECode PackageDeleteObserver::OnUserActionRequired(
    /* [in] */ IIntent* intent)
{
    return NOERROR;
}

ECode PackageDeleteObserver::OnPackageDeleted(
    /* [in] */ const String& basePackageName,
    /* [in] */ Int32 returnCode,
    /* [in] */ const String& msg)
{
    return NOERROR;
}

} // namespace App
} // namespace Droid
} // namespace Elastos
