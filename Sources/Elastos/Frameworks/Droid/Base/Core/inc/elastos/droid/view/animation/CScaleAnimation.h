
#ifndef __ELASTOS_DROID_VIEW_ANIMATION_CSCALEANIMATION_H__
#define __ELASTOS_DROID_VIEW_ANIMATION_CSCALEANIMATION_H__

#include "_Elastos_Droid_View_Animation_Elastos_Droid_View_CScaleAnimation.h"
#include "elastos/droid/view/animation/ScaleAnimation.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Animation {

CarClass(CScaleAnimation), public ScaleAnimation
{
public:
    CAR_INTERFACE_DECL();

    CAR_OBJECT_DECL();

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ Float fromX,
        /* [in] */ Float toX,
        /* [in] */ Float fromY,
        /* [in] */ Float toY);

    CARAPI constructor(
        /* [in] */ Float fromX,
        /* [in] */ Float toX,
        /* [in] */ Float fromY,
        /* [in] */ Float toY,
        /* [in] */ Float pivotX,
        /* [in] */ Float pivotY);

    CARAPI constructor(
        /* [in] */ Float fromX,
        /* [in] */ Float toX,
        /* [in] */ Float fromY,
        /* [in] */ Float toY,
        /* [in] */ Float pivotXType,
        /* [in] */ Float pivotXValue,
        /* [in] */ Float pivotYType,
        /* [in] */ Float pivotYValue);

private:
};

}   //namespace Animation
}   //namespace View
}   //namespace Droid
}   //namespace Elastos

#endif // __ELASTOS_DROID_VIEW_ANIMATION_CSCALEANIMATION_H__
