
#include "view/animation/CScaleAnimation.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Animation {

IANIMATION_METHODS_IMPL(CScaleAnimation, ScaleAnimation)

ECode CScaleAnimation::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return ScaleAnimation::Init(context, attrs);
}

ECode CScaleAnimation::constructor(
    /* [in] */ Float fromX,
    /* [in] */ Float toX,
    /* [in] */ Float fromY,
    /* [in] */ Float toY)
{
    return ScaleAnimation::Init(fromX, toX, fromY, toY);
}

ECode CScaleAnimation::constructor(
    /* [in] */ Float fromX,
    /* [in] */ Float toX,
    /* [in] */ Float fromY,
    /* [in] */ Float toY,
    /* [in] */ Float pivotX,
    /* [in] */ Float pivotY)
{
    return ScaleAnimation::Init(fromX, toX, fromY, toY, pivotX, pivotY);
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
    return ScaleAnimation::Init(fromX, toX, fromY, toY, pivotXType,
        pivotXValue, pivotYType, pivotYValue);
}

PInterface CScaleAnimation::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_Animation) {
        return reinterpret_cast<PInterface>((Animation*)this);
    }

    return _CScaleAnimation::Probe(riid);
}

}   //namespace Animation
}   //namespace View
}   //namespace Droid
}   //namespace Elastos
