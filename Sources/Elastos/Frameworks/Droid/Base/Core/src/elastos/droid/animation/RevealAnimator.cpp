
#include "animation/RevealAnimator.h"

namespace Elastos {
namespace Droid {
namespace Animation {

RevealAnimator::RevealAnimator(
    /* [in] */ IView* clipView,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Float startRadius,
    /* [in] */ Float endRadius)
    // : RenderNodeAnimator(x, y, startRadius, endRadius)
    // , mClipView(clipView)
{
    assert(0 && "TODO");
    // SetTarget(mClipView);
}

void RevealAnimator::OnFinished()
{
    mClipView->SetRevealClip(FALSE, 0, 0, 0);
    assert(0 && "TODO");
    // RenderNodeAnimator::OnFinished();
}

}   //namespace Animation
}   //namespace Droid
}   //namespace Elastos
