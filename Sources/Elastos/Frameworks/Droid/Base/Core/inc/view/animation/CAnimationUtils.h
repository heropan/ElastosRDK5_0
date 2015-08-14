
#ifndef __ELASTOS_DROID_VIEW_ANIMATION_CANIMATIONUTILS_H__
#define __ELASTOS_DROID_VIEW_ANIMATION_CANIMATIONUTILS_H__
#include "AnimationUtils.h"
#include "_CAnimationUtils.h"

/**
 * Defines common utilities for working with animations.
 *
 */

namespace Elastos {
namespace Droid {
namespace View {
namespace Animation {

CarClass(CAnimationUtils) , public AnimationUtils
{
public:
    CARAPI CurrentAnimationTimeMillis(
        /* [out] */ Int64* time);

    CARAPI LoadAnimation(
        /* [in] */ IContext* context,
        /* [in] */ Int32 id,
        /* [out] */ IAnimation** animation);

    CARAPI LoadLayoutAnimation(
        /* [in] */ IContext* context,
        /* [in] */ Int32 id,
        /* [out] */ ILayoutAnimationController** controller);

    CARAPI MakeInAnimation(
        /* [in] */ IContext* context,
        /* [in] */ Boolean fromLeft,
        /* [out] */ IAnimation** animation);

    CARAPI MakeOutAnimation(
        /* [in] */ IContext* context,
        /* [in] */ Boolean toRight,
        /* [out] */ IAnimation** animation);

    CARAPI MakeInChildBottomAnimation(
        /* [in] */ IContext* context,
        /* [out] */ IAnimation** animation);

    CARAPI LoadInterpolator(
        /* [in] */ IContext* context,
        /* [in] */ Int32 id,
        /* [out] */ IInterpolator** interpolator);
};

}   //namespace Animation
}   //namespace View
}   //namespace Droid
}   //namespace Elastos
#endif //__ELASTOS_DROID_VIEW_ANIMATION_CANIMATIONUTILS_H__
