
#ifndef __ELASTOS_DROID_VIEW_ANIMATION_CTRANSLATEANIMATION_H__
#define __ELASTOS_DROID_VIEW_ANIMATION_CTRANSLATEANIMATION_H__

#include "_Elastos_Droid_View_Animation_CTranslateAnimation.h"
#include "view/animation/TranslateAnimation.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Animation {

CarClass(CTranslateAnimation), public TranslateAnimation
{
public:
    CAR_INTERFACE_DECL();

    CAR_OBJECT_DECL();

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet * attrs);

    CARAPI constructor(
        /* [in] */ Float fromXDelta,
        /* [in] */ Float toXDelta,
        /* [in] */ Float fromYDelta,
        /* [in] */ Float toYDelta);

    CARAPI constructor(
        /* [in] */ Float fromXType,
        /* [in] */ Float fromXValue,
        /* [in] */ Float toXType,
        /* [in] */ Float toXValue,
        /* [in] */ Float fromYType,
        /* [in] */ Float fromYValue,
        /* [in] */ Float toYType,
        /* [in] */ Float toYValue);

private:
    // TODO: Add your private member variables here.
};

}   //namespace Animation
}   //namespace View
}   //namespace Droid
}   //namespace Elastos

#endif // __ELASTOS_DROID_VIEW_ANIMATION_CTRANSLATEANIMATION_H__
