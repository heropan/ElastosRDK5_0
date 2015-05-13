
#ifndef __CSCALEANIMATION_H__
#define __CSCALEANIMATION_H__

#include "_CScaleAnimation.h"
#include "view/animation/ScaleAnimation.h"
#include "view/animation/AnimationMacro.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Animation {

CarClass(CScaleAnimation), public ScaleAnimation
{
public:
    IANIMATION_METHODS_DECL()

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

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

private:
    // TODO: Add your private member variables here.
};

}   //namespace Animation
}   //namespace View
}   //namespace Droid
}   //namespace Elastos

#endif // __CSCALEANIMATION_H__
