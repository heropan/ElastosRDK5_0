
#ifndef __ELASTOS_DROID_VIEW_ANIMATION_CALPHAANIMATION_H__
#define __ELASTOS_DROID_VIEW_ANIMATION_CALPHAANIMATION_H__

#include "_CAlphaAnimation.h"
#include "view/animation/AlphaAnimation.h"


namespace Elastos {
namespace Droid {
namespace View {
namespace Animation {

CarClass(CAlphaAnimation), public AlphaAnimation
{
public:
    IANIMATION_METHODS_DECL()

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ Float fromAlpha,
        /* [in] */ Float toAlpha);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

private:
    // TODO: Add your private member variables here.
};

}// namespace Animation
}// namespace View
}// namespace Droid
}// namespace Elastos

#endif // __ELASTOS_DROID_VIEW_ANIMATION_CALPHAANIMATION_H__
