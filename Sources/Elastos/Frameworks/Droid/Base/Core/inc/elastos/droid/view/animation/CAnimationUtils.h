
#ifndef __ELASTOS_DROID_VIEW_ANIMATION_CANIMATIONUTILS_H__
#define __ELASTOS_DROID_VIEW_ANIMATION_CANIMATIONUTILS_H__

#include "AnimationUtils.h"
#include "_Elastos_Droid_View_Animation_Elastos_Droid_View_CAnimationUtils.h"

/**
 * Defines common utilities for working with animations.
 *
 */
namespace Elastos {
namespace Droid {
namespace View {
namespace Animation {

CarClass(CAnimationUtils)
    , public AnimationUtils
{
public:
    CAR_INTERFACE_DECL();

    CAR_OBJECT_DECL();
};

}   //namespace Animation
}   //namespace View
}   //namespace Droid
}   //namespace Elastos

#endif //__ELASTOS_DROID_VIEW_ANIMATION_CANIMATIONUTILS_H__
