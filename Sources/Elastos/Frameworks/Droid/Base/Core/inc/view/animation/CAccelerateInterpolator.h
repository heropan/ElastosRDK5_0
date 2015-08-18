
#ifndef __ELASTOS_DROID_VIEW_ANIMATION_CACCELERATEINTERPOLATOR_H__
#define __ELASTOS_DROID_VIEW_ANIMATION_CACCELERATEINTERPOLATOR_H__

#include "_Elastos_Droid_View_Animation_CAccelerateInterpolator.h"
#include "R.h"

using Elastos::Droid::R;
using Elastos::Droid::Content::Res::ITypedArray;
using Elastos::Droid::Utility::IAttributeSet;
using Elastos::Droid::Content::IContext;

namespace Elastos {
namespace Droid {
namespace View {
namespace Animation {

CarClass(CAccelerateInterpolator)
    : public Object
    , public IAccelerateInterpolator
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
        /* [in] */ Float factor);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    /** @hide */
    CARAPI constructor(
        /* [in] */ IResources* res,
        /* [in] */ ITheme* theme,
        /* [in] */ IAttributeSet* attrs);

    CARAPI GetInterpolation(
        /* [in] */ Float input,
        /* [out] */ Float* output);

    /** @hide */
    // @Override
    CARAPI CreateNativeInterpolator(
        /* [out] */ Int64* interpolator);

private:
    Float mFactor;
    Double mDoubleFactor;
};

}// namespace Animation
}// namespace View
}// namespace Droid
}// namespace Elastos

#endif //__ELASTOS_DROID_VIEW_ANIMATION_CACCELERATEINTERPOLATOR_H__
