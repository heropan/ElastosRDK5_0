
#include "elastos/droid/systemui/recents/views/AnimateableViewBounds.h"
#include <elastos/core/Math.h>

using Elastos::Droid::Animation::IValueAnimator;
using Elastos::Droid::Animation::CObjectAnimatorHelper;
using Elastos::Droid::Animation::IObjectAnimatorHelper;
using Elastos::Droid::Graphics::CRect;

namespace Elastos {
namespace Droid {
namespace SystemUI {
namespace Recents {
namespace Views {

AnimateableViewBounds::AnimateableViewBounds(
    /* [in] */ ITaskView* source,
    /* [in] */ Int32 cornerRadius)
    : mCornerRadius(0)
    , mAlpha(1.0f)
    , mMinAlpha(0.25f)
{
    CRect::New((IRect**)&mTmpRect);
    CRect::New((IRect**)&mClipRect);
    CRect::New((IRect**)&mClipBounds);
    CRect::New((IRect**)&mOutlineClipRect);

    assert(0);
    // mConfig = RecentsConfiguration::GetInstance();
    mSourceView = IView::Probe(source);
    mCornerRadius = cornerRadius;
    SetClipTop(GetClipTop());
    SetClipRight(GetClipRight());
    SetClipBottom(GetClipBottom());
    SetOutlineClipBottom(GetOutlineClipBottom());
}

// @Override
ECode AnimateableViewBounds::GetOutline(
    /* [in] */ IView* view,
    /* [in] */ IOutline* outline)
{
    outline->SetAlpha(mMinAlpha + mAlpha / (1.0f - mMinAlpha));
    Int32 cl, cr, ct, cb;
    mClipRect->GetLeft(&cl);
    mClipRect->GetRight(&cr);
    mClipRect->GetTop(&ct);
    mClipRect->GetBottom(&cb);
    Int32 oll, olr, olt, olb;
    mOutlineClipRect->GetLeft(&oll);
    mOutlineClipRect->GetRight(&olr);
    mOutlineClipRect->GetTop(&olt);
    mOutlineClipRect->GetBottom(&olb);
    Int32 w, h;
    mSourceView->GetWidth(&w);
    mSourceView->GetHeight(&h);
    outline->SetRoundRect(Elastos::Core::Math::Max(cl, oll),
        Elastos::Core::Math::Max(ct, olt), w - Elastos::Core::Math::Max(cr, olr),
        h - Elastos::Core::Math::Max(cb, olb), mCornerRadius);
    return NOERROR;
}

/** Sets the view outline alpha. */
void AnimateableViewBounds::SetAlpha(
    /* [in] */ Float alpha)
{
    if (Elastos::Core::Math::Compare(alpha, mAlpha) != 0) {
        mAlpha = alpha;
        mSourceView->InvalidateOutline();
    }
}

/** Animates the top clip. */
void AnimateableViewBounds::AnimateClipTop(
    /* [in] */ Int32 top,
    /* [in] */ Int32 duration,
    /* [in] */ IAnimatorUpdateListener* updateListener)
{
    if (mClipTopAnimator != NULL) {
        mClipTopAnimator->RemoveAllListeners();
        mClipTopAnimator->Cancel();
    }
    AutoPtr<IObjectAnimatorHelper> oaHelper;
    CObjectAnimatorHelper::AcquireSingleton((IObjectAnimatorHelper**)&oaHelper);
    AutoPtr<ArrayOf<Int32> > params = ArrayOf<Int32>::Alloc(1);
    (*params)[0] = top;
    AutoPtr<IObjectAnimator> objectAnimator;
    oaHelper->OfInt32((IObject*)this, String("clipTop"), params, (IObjectAnimator**)&objectAnimator);
    mClipTopAnimator = IAnimator::Probe(mClipTopAnimator);
    mClipTopAnimator->SetDuration(duration);
    assert(0);
    // mClipTopAnimator->SetInterpolator(mConfig->mFastOutSlowInInterpolator);
    if (updateListener != NULL) {
        IValueAnimator::Probe(mClipTopAnimator)->AddUpdateListener(updateListener);
    }
    mClipTopAnimator->Start();
}

/** Sets the top clip. */
void AnimateableViewBounds::SetClipTop(
    /* [in] */ Int32 top)
{
    Int32 ct;
    mClipRect->GetTop(&ct);
    if (top != ct) {
        mClipRect->SetTop(top);
        mSourceView->InvalidateOutline();
        UpdateClipBounds();
    }
}

/** Returns the top clip. */
Int32 AnimateableViewBounds::GetClipTop()
{
    Int32 top;
    mClipRect->GetTop(&top);
    return top;
}

/** Animates the right clip. */
void AnimateableViewBounds::AnimateClipRight(
    /* [in] */ Int32 right,
    /* [in] */ Int32 duration)
{
    if (mClipRightAnimator != NULL) {
        mClipRightAnimator->RemoveAllListeners();
        mClipRightAnimator->Cancel();
    }
    AutoPtr<IObjectAnimatorHelper> oaHelper;
    CObjectAnimatorHelper::AcquireSingleton((IObjectAnimatorHelper**)&oaHelper);
    AutoPtr<ArrayOf<Int32> > params = ArrayOf<Int32>::Alloc(1);
    (*params)[0] = right;
    AutoPtr<IObjectAnimator> objectAnimator;
    oaHelper->OfInt32((IObject*)this, String("clipRight"), params, (IObjectAnimator**)&objectAnimator);
    mClipRightAnimator = IAnimator::Probe(mClipRightAnimator);
    mClipRightAnimator->SetDuration(duration);
    assert(0);
    // mClipRightAnimator->SetInterpolator(mConfig->mFastOutSlowInInterpolator);
    mClipRightAnimator->Start();
}

/** Sets the right clip. */
void AnimateableViewBounds::SetClipRight(
    /* [in] */ Int32 right)
{
    Int32 cr;
    mClipRect->GetRight(&cr);
    if (right != cr) {
        mClipRect->SetRight(right);
        mSourceView->InvalidateOutline();
        UpdateClipBounds();
    }
}

/** Returns the right clip. */
Int32 AnimateableViewBounds::GetClipRight()
{
    Int32 right;
    mClipRect->GetRight(&right);
    return right;
}

/** Animates the bottom clip. */
void AnimateableViewBounds::AnimateClipBottom(
    /* [in] */ Int32 bottom,
    /* [in] */ Int32 duration)
{
    if (mClipBottomAnimator != NULL) {
        mClipBottomAnimator->RemoveAllListeners();
        mClipBottomAnimator->Cancel();
    }
    AutoPtr<IObjectAnimatorHelper> oaHelper;
    CObjectAnimatorHelper::AcquireSingleton((IObjectAnimatorHelper**)&oaHelper);
    AutoPtr<ArrayOf<Int32> > params = ArrayOf<Int32>::Alloc(1);
    (*params)[0] = bottom;
    AutoPtr<IObjectAnimator> objectAnimator;
    oaHelper->OfInt32((IObject*)this, String("clipBottom"), params, (IObjectAnimator**)&objectAnimator);
    mClipBottomAnimator = IAnimator::Probe(mClipBottomAnimator);
    mClipBottomAnimator->SetDuration(duration);
    assert(0);
    // mClipBottomAnimator->SetInterpolator(mConfig->mFastOutSlowInInterpolator);
    mClipBottomAnimator->Start();
}

/** Sets the bottom clip. */
void AnimateableViewBounds::SetClipBottom(
    /* [in] */ Int32 bottom)
{
    Int32 cb;
    mClipRect->GetBottom(&cb);
    if (bottom != cb) {
        mClipRect->SetBottom(bottom);
        mSourceView->InvalidateOutline();
        UpdateClipBounds();
        assert(0);
        // if (!mConfig->mUseHardwareLayers) {
        //     Int32 pb;
        //     mSourceView->GetPaddingBottom(&pb);
        //     mSourceView->mThumbnailView->UpdateVisibility(bottom - pb);
        // }
    }
}

/** Returns the bottom clip. */
Int32 AnimateableViewBounds::GetClipBottom()
{
    Int32 bottom;
    mClipRect->GetBottom(&bottom);
    return bottom;
}

/** Sets the outline bottom clip. */
void AnimateableViewBounds::SetOutlineClipBottom(
    /* [in] */ Int32 bottom)
{
    Int32 olb;
    mOutlineClipRect->GetBottom(&olb);
    if (bottom != olb) {
        mOutlineClipRect->SetBottom(bottom);
        mSourceView->InvalidateOutline();
    }
}

/** Gets the outline bottom clip. */
Int32 AnimateableViewBounds::GetOutlineClipBottom()
{
    Int32 bottom;
    mOutlineClipRect->GetBottom(&bottom);
    return bottom;
}

void AnimateableViewBounds::UpdateClipBounds()
{
    Int32 cl, cr, ct, cb;
    mClipRect->GetLeft(&cl);
    mClipRect->GetRight(&cr);
    mClipRect->GetTop(&ct);
    mClipRect->GetBottom(&cb);
    Int32 w, h;
    mSourceView->GetWidth(&w);
    mSourceView->GetHeight(&h);

    mClipBounds->Set(cl, ct, w - cr, h - cb);
    mSourceView->SetClipBounds(mClipBounds);
}

} // namespace Views
} // namespace Recents
} // namespace SystemUI
} // namespace Droid
} // namespace Elastos
