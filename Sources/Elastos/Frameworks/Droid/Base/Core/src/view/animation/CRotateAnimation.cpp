
#include "view/animation/CRotateAnimation.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Animation {

IANIMATION_METHODS_IMPL(CRotateAnimation, RotateAnimation)

ECode CRotateAnimation::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return RotateAnimation::Init(context, attrs);
}

ECode CRotateAnimation::constructor(
    /* [in] */ Float fromDegrees,
    /* [in] */ Float toDegrees)
{
    return RotateAnimation::Init(fromDegrees, toDegrees);
}

ECode CRotateAnimation::constructor(
    /* [in] */ Float fromDegrees,
    /* [in] */ Float toDegrees,
    /* [in] */ Float pivotX,
    /* [in] */ Float pivotY)
{
    return RotateAnimation::Init(fromDegrees, toDegrees, pivotX, pivotY);
}

ECode CRotateAnimation::constructor(
    /* [in] */ Float fromDegrees,
    /* [in] */ Float toDegrees,
    /* [in] */ Int32 pivotXType,
    /* [in] */ Float pivotXValue,
    /* [in] */ Int32 pivotYType,
    /* [in] */ Float pivotYValue)
{
    return RotateAnimation::Init(fromDegrees, toDegrees, pivotXType,
        pivotXValue, pivotYType, pivotYValue);
}

PInterface CRotateAnimation::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_Animation) {
        return reinterpret_cast<PInterface>((Animation*)this);
    }

    return _CRotateAnimation::Probe(riid);
}

}   //namespace Animation
}   //namespace View
}   //namespace Droid
}   //namespace Elastos
