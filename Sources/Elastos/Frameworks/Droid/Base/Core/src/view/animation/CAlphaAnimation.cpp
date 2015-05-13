
#include "view/animation/CAlphaAnimation.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Animation {

IANIMATION_METHODS_IMPL(CAlphaAnimation, AlphaAnimation)

ECode CAlphaAnimation::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return AlphaAnimation::Init(context, attrs);
}

ECode CAlphaAnimation::constructor(
    /* [in] */ Float fromAlpha,
    /* [in] */ Float toAlpha)
{
    return AlphaAnimation::Init(fromAlpha, toAlpha);
}

PInterface CAlphaAnimation::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_Animation) {
        return reinterpret_cast<PInterface>((Animation*)this);
    }

    return _CAlphaAnimation::Probe(riid);
}

}// namespace Animation
}// namespace View
}// namespace Droid
}// namespace Elastos
