
#ifndef __ELASTOS_DROID_VIEW_ANIMATION_CLAYOUTANIMATIONCONTROLLER_H__
#define __ELASTOS_DROID_VIEW_ANIMATION_CLAYOUTANIMATIONCONTROLLER_H__

#include "_Elastos_Droid_View_Animation_CLayoutAnimationController.h"
#include "view/animation/LayoutAnimationController.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Animation {

CarClass(CLayoutAnimationController), public LayoutAnimationController
{
public:
    CAR_INTERFACE_DECL();

    CAR_OBJECT_DECL();

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ IAnimation* animation);

    CARAPI constructor(
        /* [in] */ IAnimation* animation,
        /* [in] */ Float delay);

private:
};

}   //namespace Animation
}   //namespace View
}   //namespace Droid
}   //namespace Elastos

#endif // __ELASTOS_DROID_VIEW_ANIMATION_CLAYOUTANIMATIONCONTROLLER_H__
