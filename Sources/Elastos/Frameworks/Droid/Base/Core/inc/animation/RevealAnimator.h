
#ifndef  __ELASTOS_DROID_ANIMATION_REVEALANIMATOR_H__
#define  __ELASTOS_DROID_ANIMATION_REVEALANIMATOR_H__

#include "view/RenderNodeAnimator.h"

namespace Elastos {
namespace Droid {
namespace Animation {

/**
 * Reveals a View with an animated clipping circle.
 * The clipping is implemented efficiently by talking to a private reveal API on View.
 * This hidden class currently only accessed by the {@link android.view.View}.
 *
 * @hide
 */
class RevealAnimator
    : public RenderNodeAnimator
{
public:
    RevealAnimator(
        /* [in] */ IView* clipView,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Float startRadius,
        /* [in] */ Float endRadius);

protected:
    // @Override
    virtual CARAPI_(void) OnFinished();

private:
    AutoPtr<IView> mClipView;
};

}   //namespace Animation
}   //namespace Droid
}   //namespace Elastos

#endif  // __ELASTOS_DROID_ANIMATION_REVEALANIMATOR_H__
