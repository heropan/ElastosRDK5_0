
#include "view/animation/CAnimationSet.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Animation {

IANIMATION_METHODS_IMPL(CAnimationSet, AnimationSet)

ECode CAnimationSet::AddAnimation(
    /* [in] */ IAnimation* a)
{
    return AnimationSet::AddAnimation(a);
}

ECode CAnimationSet::GetAnimations(
    /* [out] */ IObjectContainer** animations)
{
    VALIDATE_NOT_NULL(animations);

    return AnimationSet::GetAnimations(animations);
}

ECode CAnimationSet::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return AnimationSet::Init(context, attrs);
}

ECode CAnimationSet::constructor(
    /* [in] */ Boolean shareInterpolator)
{
    return AnimationSet::Init(shareInterpolator);
}

PInterface CAnimationSet::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_Animation) {
        return reinterpret_cast<PInterface>((Animation*)this);
    }

    return _CAnimationSet::Probe(riid);
}

}// namespace Animation
}// namespace View
}// namespace Droid
}// namespace Elastos
