
#ifndef __ELASTOS_DROID_VIEW_ANIMATION_COVERSHOOTINTERPOLATOR_H__
#define __ELASTOS_DROID_VIEW_ANIMATION_COVERSHOOTINTERPOLATOR_H__

#include "_Elastos_Droid_View_Animation_COvershootInterpolator.h"
#include "R.h"

using Elastos::Droid::R;
using Elastos::Droid::Content::Res::ITypedArray;
using Elastos::Droid::Utility::IAttributeSet;
using Elastos::Droid::Content::IContext;

namespace Elastos {
namespace Droid {
namespace View {
namespace Animation {

CarClass(COvershootInterpolator)
    : public Object
    , public IOvershootInterpolator
    , public INativeInterpolatorFactory
    , public IHasNativeInterpolator
    , public IInterpolator
    , public ITimeInterpolator
{
public:
    CAR_INTERFACE_DECL();

    CAR_OBJECT_DECL();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Float tension);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI GetInterpolation(
        /* [in] */ Float input,
        /* [out] */ Float* output);

private:
    Float mTension;
};

}// namespace Animation
}// namespace View
}// namespace Droid
}// namespace Elastos

#endif //__ELASTOS_DROID_VIEW_ANIMATION_COVERSHOOTINTERPOLATOR_H__
