
#ifndef __ELASTOS_DROID_VIEW_ANIMATION_CCYCLEINTERPOLATOR_H__
#define __ELASTOS_DROID_VIEW_ANIMATION_CCYCLEINTERPOLATOR_H__

#include "_Elastos_Droid_View_Animation_Elastos_Droid_View_CCycleInterpolator.h"
#include "R.h"

using Elastos::Droid::R;
using Elastos::Droid::Content::Res::ITypedArray;
using Elastos::Droid::Utility::IAttributeSet;
using Elastos::Droid::Content::IContext;

namespace Elastos {
namespace Droid {
namespace View {
namespace Animation {

CarClass(CCycleInterpolator)
    : public Object
    , public ICycleInterpolator
    , public INativeInterpolatorFactory
    , public IHasNativeInterpolator
    , public IInterpolator
    , public ITimeInterpolator
{
public:
    CAR_INTERFACE_DECL();

    CAR_OBJECT_DECL();

    CARAPI constructor(
        /* [in] */ Float cycles);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI GetInterpolation(
        /* [in] */ Float input,
        /* [out] */ Float* output);

private:
    Float mCycles;
};

}// namespace Animation
}// namespace View
}// namespace Droid
}// namespace Elastos

#endif //__ELASTOS_DROID_VIEW_ANIMATION_CCYCLEINTERPOLATOR_H__
