
#ifndef __ELASTOS_DROID_VIEW_ANIMATION_CCLIPRECTANIMATION_H__
#define __ELASTOS_DROID_VIEW_ANIMATION_CCLIPRECTANIMATION_H__

#include "_Elastos_Droid_View_Animation_CClipRectAnimation.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Animation {

/**
 * An animation that controls the clip of an object. See the
 * {@link android.view.animation full package} description for details and
 * sample code.
 *
 * @hide
 */
CarClass(CClipRectAnimation)
    , public Animation
{
public:
    CAR_INTERFACE_DECL();

    CAR_OBJECT_DECL();

    CClipRectAnimation();

    /**
     * Constructor to use when building a ClipRectAnimation from code
     *
     * @param fromClip the clip rect to animate from
     * @param toClip the clip rect to animate to
     */
    CARAPI constructor(
        /* [in] */ IRect* fromClip,
        /* [in] */ IRect* toClip);

    // @Override
    CARAPI_(void) ApplyTransformation(
        /* [in] */ Float it,
        /* [in] */ ITransformation* tr);

    // @Override
    CARAPI WillChangeTransformationMatrix(
        /* [out] */ Boolean* result);

private:
    AutoPtr<IRect> mFromRect;
    AutoPtr<IRect> mToRect;
};

}   //namespace Animation
}   //namespace View
}   //namespace Droid
}   //namespace Elastos

#endif // __ELASTOS_DROID_VIEW_ANIMATION_CCLIPRECTANIMATION_H__
