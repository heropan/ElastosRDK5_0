
#include "elastos/droid/internal/policy/impl/LogDecelerateInterpolator.h"

//TODO using Elastos::Droid::View::Animation::EIID_ITimeInterpolator;
//TODO using Elastos::Droid::View::Animation::EIID_Interpolator;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Policy {
namespace Impl {

//=====================================================================
//                      LogDecelerateInterpolator
//=====================================================================
//TODO CAR_INTERFACE_IMPL_3(LogDecelerateInterpolator, Object, ILogDecelerateInterpolator, IInterpolator, ITimeInterpolator)
CAR_INTERFACE_IMPL(LogDecelerateInterpolator, Object, ILogDecelerateInterpolator)//TODO should be replaced by the line above

LogDecelerateInterpolator::LogDecelerateInterpolator()
{
}

ECode LogDecelerateInterpolator::constructor(
    /* [in] */ Int32 base,
    /* [in] */ Int32 drift)
{
    // ==================before translated======================
    // mBase = base;
    // mDrift = drift;
    //
    // mLogScale = 1f / computeLog(1, mBase, mDrift);
    return NOERROR;
}

ECode LogDecelerateInterpolator::GetInterpolation(
    /* [in] */ Float t,
    /* [out] */ Float* output)
{
    VALIDATE_NOT_NULL(output)
    // ==================before translated======================
    // return computeLog(t, mBase, mDrift) * mLogScale;
    assert(0);
    return NOERROR;
}
ECode HasNativeInterpolator(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = FALSE;
    return E_NOT_IMPLEMENTED;
}


Float LogDecelerateInterpolator::ComputeLog(
    /* [in] */ Float t,
    /* [in] */ Int32 base,
    /* [in] */ Int32 drift)
{
    // ==================before translated======================
    // return (float) -Math.pow(base, -t) + 1 + (drift * t);
    assert(0);
    return 0.0f;
}

} // namespace Impl
} // namespace Policy
} // namespace Internal
} // namespace Droid
} // namespace Elastos


