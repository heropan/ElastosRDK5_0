
#include "elastos/droid/app/CPackageInstallObserver2.h"

using Elastos::Droid::Content::Pm::EIID_IIPackageInstallObserver2;

namespace Elastos {
namespace Droid {
namespace App {

CAR_INTERFACE_IMPL(CPackageInstallObserver2, Object, IIPackageInstallObserver2)

CAR_OBJECT_IMPL(CPackageInstallObserver2)

ECode CPackageInstallObserver2::constructor(
    /* [in] */ IPackageInstallObserver* host)
{
    IWeakReferenceSource::Probe(host)->GetWeakReference((IWeakReference**)&mWeakHost);
    return NOERROR;
}

ECode CPackageInstallObserver2::OnUserActionRequired(
    /* [in] */ IIntent* intent)
{
    AutoPtr<IPackageInstallObserver> host;
    mWeakHost->Resolve(EIID_IPackageInstallObserver, (IInterface**)&host);
    return host->OnUserActionRequired(intent);
}

ECode CPackageInstallObserver2::OnPackageInstalled(
    /* [in] */ const String& basePackageName,
    /* [in] */ Int32 returnCode,
    /* [in] */ const String& msg,
    /* [in] */ IBundle* extras)
{
    AutoPtr<IPackageInstallObserver> host;
    mWeakHost->Resolve(EIID_IPackageInstallObserver, (IInterface**)&host);
    return host->OnPackageInstalled(basePackageName, returnCode, msg, extras);
}

} // namespace App
} // namespace Droid
} // namespace Elastos
