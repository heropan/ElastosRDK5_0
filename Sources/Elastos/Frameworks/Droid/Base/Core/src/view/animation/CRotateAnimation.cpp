
#include "view/animation/CRotateAnimation.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Animation {

CAR_OBJECT_IMPL(CRotateAnimation);

ECode CRotateAnimation::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return RotateAnimation::constructor(context, attrs);
}

ECode CRotateAnimation::constructor(
    /* [in] */ Float fromDegrees,
    /* [in] */ Float toDegrees)
{
    return RotateAnimation::constructor(fromDegrees, toDegrees);
}

ECode CRotateAnimation::constructor(
    /* [in] */ Float fromDegrees,
    /* [in] */ Float toDegrees,
    /* [in] */ Float pivotX,
    /* [in] */ Float pivotY)
{
    return RotateAnimation::constructor(fromDegrees, toDegrees, pivotX, pivotY);
}

ECode CRotateAnimation::constructor(
    /* [in] */ Float fromDegrees,
    /* [in] */ Float toDegrees,
    /* [in] */ Int32 pivotXType,
    /* [in] */ Float pivotXValue,
    /* [in] */ Int32 pivotYType,
    /* [in] */ Float pivotYValue)
{
    return RotateAnimation::constructor(fromDegrees, toDegrees, pivotXType,
        pivotXValue, pivotYType, pivotYValue);
}

}   //namespace Animation
}   //namespace View
}   //namespace Droid
}   //namespace Elastos
