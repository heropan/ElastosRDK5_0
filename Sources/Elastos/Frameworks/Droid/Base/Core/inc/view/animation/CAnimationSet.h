
#ifndef __CANIMATIONSET_H__
#define __CANIMATIONSET_H__

#include "_CAnimationSet.h"
#include "view/animation/AnimationSet.h"


namespace Elastos {
namespace Droid {
namespace View {
namespace Animation {

CarClass(CAnimationSet), public AnimationSet
{
public:
    IANIMATION_METHODS_DECL();

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ Boolean shareInterpolator);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI AddAnimation(
        /* [in] */ IAnimation* a);

    CARAPI GetAnimations(
        /* [out] */ IObjectContainer** animations);

private:
    // TODO: Add your private member variables here.
};

}// namespace Animation
}// namespace View
}// namespace Droid
}// namespace Elastos

#endif // __CANIMATIONSET_H__
