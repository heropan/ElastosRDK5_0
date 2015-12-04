#ifndef __ELASTOS_DROID_INTERNAL_POLICY_IMPL_LOGDECELERATEINTERPOLATOR_H__
#define __ELASTOS_DROID_INTERNAL_POLICY_IMPL_LOGDECELERATEINTERPOLATOR_H__

#include "elastos/droid/ext/frameworkext.h"

//TODO using Elastos::Droid::View::Animation::ITimeInterpolator;
//TODO using Elastos::Droid::View::Animation::Interpolator;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Policy {
namespace Impl {

class LogDecelerateInterpolator
    : public Object
    , public ILogDecelerateInterpolator
    //TODO , public IInterpolator
    //TODO , public ITimeInterpolator
{
public:
    CAR_INTERFACE_DECL()

    LogDecelerateInterpolator();

    CARAPI constructor(
        /* [in] */ Int32 base,
        /* [in] */ Int32 drift);

    // @Override
    CARAPI GetInterpolation(
        /* [in] */ Float t,
        /* [out] */ Float* output);

private:
    CARAPI HasNativeInterpolator(
        /* [out] */ Boolean* res);

    static CARAPI_(Float) ComputeLog(
        /* [in] */ Float t,
        /* [in] */ Int32 base,
        /* [in] */ Int32 drift);

private:
    Int32 mBase;
    Int32 mDrift;
    /*const*/ Float mLogScale;
};

} // namespace Impl
} // namespace Policy
} // namespace Internal
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_INTERNAL_POLICY_IMPL_LOGDECELERATEINTERPOLATOR_H__

