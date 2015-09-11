
#ifndef __ELASTOS_DROID_VIEW_ANIMATION_CALPHAANIMATION_H__
#define __ELASTOS_DROID_VIEW_ANIMATION_CALPHAANIMATION_H__

#include "_Elastos_Droid_View_Animation_CAlphaAnimation.h"
#include "view/animation/AlphaAnimation.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Animation {

CarClass(CAlphaAnimation)
    , public AlphaAnimation
{
public:
    CAR_INTERFACE_DECL();

    CAR_OBJECT_DECL();

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ Float fromAlpha,
        /* [in] */ Float toAlpha);

private:
};

}// namespace Animation
}// namespace View
}// namespace Droid
}// namespace Elastos

#endif // __ELASTOS_DROID_VIEW_ANIMATION_CALPHAANIMATION_H__
