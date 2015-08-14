
#include "view/RenderNodeAnimator.h"

namespace Elastos {
namespace Droid {
namespace Animation {

RevealAnimator::RevealAnimator(
    /* [in] */ IView* clipView,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Float startRadius,
    /* [in] */ Float endRadius)
    : RenderNodeAnimator(x, y, startRadius, endRadius)
    , mClipView(clipView)
{
    SetTarget(mClipView);
}

void RevealAnimator::OnFinished()
{
    mClipView.setRevealClip(false, 0, 0, 0);
    super.onFinished();
}

}   //namespace Animation
}   //namespace Droid
}   //namespace Elastos
