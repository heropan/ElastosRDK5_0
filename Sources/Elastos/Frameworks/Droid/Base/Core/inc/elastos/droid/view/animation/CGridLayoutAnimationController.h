
#ifndef __ELASTOS_DROID_VIEW_ANIMATION_CGRIDLAYOUTANIMATIONCONTROLLER_H__
#define __ELASTOS_DROID_VIEW_ANIMATION_CGRIDLAYOUTANIMATIONCONTROLLER_H__

#include "_Elastos_Droid_View_Animation_CGridLayoutAnimationController.h"
#include "elastos/droid/view/animation/GridLayoutAnimationController.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Animation {

CarClass(CGridLayoutAnimationController), public GridLayoutAnimationController
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
        /* [in] */ Float columnDelay,
        /* [in] */ Float rowDelay);

private:
};

}   //namespace Animation
}   //namespace View
}   //namespace Droid
}   //namespace Elastos

#endif // __ELASTOS_DROID_VIEW_ANIMATION_CGRIDLAYOUTANIMATIONCONTROLLER_H__
