
#include "view/animation/CScaleAnimation.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Animation {

CAR_OBJECT_IMPL(CScaleAnimation);

ECode CScaleAnimation::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return ScaleAnimation::constructor(context, attrs);
}

ECode CScaleAnimation::constructor(
    /* [in] */ Float fromX,
    /* [in] */ Float toX,
    /* [in] */ Float fromY,
    /* [in] */ Float toY)
{
    return ScaleAnimation::constructor(fromX, toX, fromY, toY);
}

ECode CScaleAnimation::constructor(
    /* [in] */ Float fromX,
    /* [in] */ Float toX,
    /* [in] */ Float fromY,
    /* [in] */ Float toY,
    /* [in] */ Float pivotX,
    /* [in] */ Float pivotY)
{
    return ScaleAnimation::constructor(fromX, toX, fromY, toY, pivotX, pivotY);
}

ECode CScaleAnimation::constructor(
    /* [in] */ Float fromX,
    /* [in] */ Float toX,
    /* [in] */ Float fromY,
    /* [in] */ Float toY,
    /* [in] */ Float pivotXType,
    /* [in] */ Float pivotXValue,
    /* [in] */ Float pivotYType,
    /* [in] */ Float pivotYValue)
{
    return ScaleAnimation::constructor(fromX, toX, fromY, toY, pivotXType,
        pivotXValue, pivotYType, pivotYValue);
}

}   //namespace Animation
}   //namespace View
}   //namespace Droid
}   //namespace Elastos
