
#ifndef __CROTATEANIMATION_H__
#define __CROTATEANIMATION_H__

#include "_CRotateAnimation.h"
#include "view/animation/RotateAnimation.h"
#include "view/animation/AnimationMacro.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Animation {

CarClass(CRotateAnimation), public RotateAnimation
{
public:
    IANIMATION_METHODS_DECL()

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ Float fromDegrees,
        /* [in] */ Float toDegrees);

    CARAPI constructor(
        /* [in] */ Float fromDegrees,
        /* [in] */ Float toDegrees,
        /* [in] */ Float pivotX,
        /* [in] */ Float pivotY);

    CARAPI constructor(
        /* [in] */ Float fromDegrees,
        /* [in] */ Float toDegrees,
        /* [in] */ Int32 pivotXType,
        /* [in] */ Float pivotXValue,
        /* [in] */ Int32 pivotYType,
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

#endif // __CROTATEANIMATION_H__
