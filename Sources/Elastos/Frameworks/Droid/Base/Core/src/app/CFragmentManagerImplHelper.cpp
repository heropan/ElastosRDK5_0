
#include "ext/frameworkdef.h"
#include "app/CFragmentManagerImplHelper.h"
#include "app/CFragmentManagerImpl.h"

namespace Elastos {
namespace Droid {
namespace App {

ECode CFragmentManagerImplHelper::ReverseTransit(
    /* [in] */ Int32 transit,
    /* [out] */ Int32* reverse)
{
    return CFragmentManagerImpl::ReverseTransit(transit, reverse);
}

ECode CFragmentManagerImplHelper::TransitToStyleIndex(
    /* [in] */ Int32 transit,
    /* [in] */ Boolean enter,
    /* [out] */ Int32* index)
{
    return CFragmentManagerImpl::TransitToStyleIndex(transit, enter, index);
}

ECode CFragmentManagerImplHelper::SetDEBUG(
    /* [in] */ Boolean debug)
{
    CFragmentManagerImpl::DEBUG = debug;
    return NOERROR;
}

ECode CFragmentManagerImplHelper::GetDEBUG(
    /* [out] */ Boolean* debug)
{
    VALIDATE_NOT_NULL(debug);
    *debug = CFragmentManagerImpl::DEBUG;
    return NOERROR;
}

} // namespace App
} // namespace Droid
} // namespace Elastos
