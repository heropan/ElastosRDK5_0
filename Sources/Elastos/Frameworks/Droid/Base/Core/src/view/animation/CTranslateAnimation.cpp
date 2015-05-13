
#include "view/animation/CTranslateAnimation.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Animation {

IANIMATION_METHODS_IMPL(CTranslateAnimation, TranslateAnimation);

ECode CTranslateAnimation::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return TranslateAnimation::Init(context, attrs);
}

ECode CTranslateAnimation::constructor(
    /* [in] */ Float fromXDelta,
    /* [in] */ Float toXDelta,
    /* [in] */ Float fromYDelta,
    /* [in] */ Float toYDelta)
{
    return TranslateAnimation::Init(fromXDelta, toXDelta, fromYDelta, toYDelta);
}

ECode CTranslateAnimation::constructor(
    /* [in] */ Float fromXType,
    /* [in] */ Float fromXValue,
    /* [in] */ Float toXType,
    /* [in] */ Float toXValue,
    /* [in] */ Float fromYType,
    /* [in] */ Float fromYValue,
    /* [in] */ Float toYType,
    /* [in] */ Float toYValue)
{
    return TranslateAnimation::Init(fromYType, fromXValue, toXType, toXValue,
        fromYType, fromYValue, toYType, toYValue);
}

PInterface CTranslateAnimation::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_Animation) {
        return reinterpret_cast<PInterface>((Animation*)this);
    }

    return _CTranslateAnimation::Probe(riid);
}

}   //namespace Animation
}   //namespace View
}   //namespace Droid
}   //namespace Elastos
