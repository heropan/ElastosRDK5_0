
#include "elastos/droid/app/CPackageDeleteObserver2.h"

using Elastos::Droid::Content::Pm::EIID_IIPackageDeleteObserver2;

namespace Elastos {
namespace Droid {
namespace App {

CAR_INTERFACE_IMPL(CPackageDeleteObserver2, Object, IIPackageDeleteObserver2)

CAR_OBJECT_IMPL(CPackageDeleteObserver2)

ECode CPackageDeleteObserver2::constructor(
    /* [in] */ IPackageDeleteObserver* host)
{
    IWeakReferenceSource::Probe(host)->GetWeakReference((IWeakReference**)&mWeakHost);
    return NOERROR;
}

ECode CPackageDeleteObserver2::OnUserActionRequired(
    /* [in] */ IIntent* intent)
{
    AutoPtr<IPackageDeleteObserver> host;
    mWeakHost->Resolve(EIID_IPackageDeleteObserver, (IInterface**)&host);
    return host->OnUserActionRequired(intent);
}

ECode CPackageDeleteObserver2::OnPackageDeleted(
    /* [in] */ const String& basePackageName,
    /* [in] */ Int32 returnCode,
    /* [in] */ const String& msg)
{
    AutoPtr<IPackageDeleteObserver> host;
    mWeakHost->Resolve(EIID_IPackageDeleteObserver, (IInterface**)&host);
    return host->OnPackageDeleted(basePackageName, returnCode, msg);
}

} // namespace App
} // namespace Droid
} // namespace Elastos
