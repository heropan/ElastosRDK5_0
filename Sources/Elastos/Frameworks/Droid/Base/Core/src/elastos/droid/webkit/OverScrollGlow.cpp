
#include "elastos/droid/webkit/OverScrollGlow.h"
#include "elastos/droid/webkit/CWebViewClassic.h"
#include "elastos/droid/widget/CEdgeEffect.h"

#include <elastos/core/Math.h>

using Elastos::Droid::Widget::CEdgeEffect;

namespace Elastos {
namespace Droid {
namespace Webkit {

OverScrollGlow::OverScrollGlow(
    /* [in] */ CWebViewClassic* host)
       : mHostView(host)
{
    AutoPtr<IContext> context = host->GetContext();
    CEdgeEffect::New(context, (IEdgeEffect**)&mEdgeGlowTop);
    CEdgeEffect::New(context, (IEdgeEffect**)&mEdgeGlowBottom);
    CEdgeEffect::New(context, (IEdgeEffect**)&mEdgeGlowLeft);
    CEdgeEffect::New(context, (IEdgeEffect**)&mEdgeGlowRight);
}

/**
 * Pull leftover touch scroll distance into one of the edge glows as appropriate.
 *
 * @param x Current X scroll offset
 * @param y Current Y scroll offset
 * @param oldX Old X scroll offset
 * @param oldY Old Y scroll offset
 * @param maxX Maximum range for horizontal scrolling
 * @param maxY Maximum range for vertical scrolling
 */
void OverScrollGlow::PullGlow(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 oldX,
    /* [in] */ Int32 oldY,
    /* [in] */ Int32 maxX,
    /* [in] */ Int32 maxY)
{
    // Only show overscroll bars if there was no movement in any direction
    // as a result of scrolling.
    if (oldX == mHostView->GetScrollX() && oldY == mHostView->GetScrollY()) {
        // Don't show left/right glows if we fit the whole content.
        // Also don't show if there was vertical movement.
        if (maxX > 0) {
            Int32 pulledToX = oldX + mOverScrollDeltaX;
            if (pulledToX < 0) {
                mEdgeGlowLeft->OnPull((Float) mOverScrollDeltaX / mHostView->GetWidth());
                Boolean bFlag = FALSE;
                mEdgeGlowRight->IsFinished(&bFlag);
                if (!bFlag) {
                    mEdgeGlowRight->OnRelease();
                }
            }
            else if (pulledToX > maxX) {
                mEdgeGlowRight->OnPull((Float) mOverScrollDeltaX / mHostView->GetWidth());
                Boolean bFlag = FALSE;
                mEdgeGlowLeft->IsFinished(&bFlag);
                if (!bFlag) {
                    mEdgeGlowLeft->OnRelease();
                }
            }
            mOverScrollDeltaX = 0;
        }

        Int32 scrollMode = 0;
        assert(0);
//        mHostView->GetWebView()->GetOverScrollMode(&scrollMode);
        if (maxY > 0 || scrollMode == IView::OVER_SCROLL_ALWAYS) {
            Int32 pulledToY = oldY + mOverScrollDeltaY;
            if (pulledToY < 0) {
                mEdgeGlowTop->OnPull((Float) mOverScrollDeltaY / mHostView->GetHeight());
                Boolean bFlag = FALSE;
                mEdgeGlowBottom->IsFinished(&bFlag);
                if (!bFlag) {
                    mEdgeGlowBottom->OnRelease();
                }
            }
            else if (pulledToY > maxY) {
                mEdgeGlowBottom->OnPull((Float) mOverScrollDeltaY / mHostView->GetHeight());
                Boolean bFlag = FALSE;
                mEdgeGlowTop->IsFinished(&bFlag);
                if (!bFlag) {
                    mEdgeGlowTop->OnRelease();
                }
            }
            mOverScrollDeltaY = 0;
        }
    }
}

/**
 * Set touch delta values indicating the current amount of overscroll.
 *
 * @param deltaX
 * @param deltaY
 */
void OverScrollGlow::SetOverScrollDeltas(
    /* [in] */ Int32 deltaX,
    /* [in] */ Int32 deltaY)
{
    mOverScrollDeltaX = deltaX;
    mOverScrollDeltaY = deltaY;
}

/**
 * Absorb leftover fling velocity into one of the edge glows as appropriate.
 *
 * @param x Current X scroll offset
 * @param y Current Y scroll offset
 * @param oldX Old X scroll offset
 * @param oldY Old Y scroll offset
 * @param rangeX Maximum range for horizontal scrolling
 * @param rangeY Maximum range for vertical scrolling
 */
void OverScrollGlow::AbsorbGlow(
    /* [in] */ Int32 x, Int32 y,
    /* [in] */ Int32 oldX,
    /* [in] */ Int32 oldY,
    /* [in] */ Int32 rangeX,
    /* [in] */ Int32 rangeY)
{
    Int32 scrollMode = 0;
//    mHostView->GetWebView()->GetOverScrollMode(&scrollMode);
    if (rangeY > 0 || scrollMode == IView::OVER_SCROLL_ALWAYS) {
        if (y < 0 && oldY >= 0) {
            mEdgeGlowTop->OnAbsorb((Int32) mHostView->mScroller->GetCurrVelocity());
            Boolean bFlag = FALSE;
            mEdgeGlowBottom->IsFinished(&bFlag);
            if (!bFlag) {
                mEdgeGlowBottom->OnRelease();
            }
        }
        else if (y > rangeY && oldY <= rangeY) {
            mEdgeGlowBottom->OnAbsorb((Int32) mHostView->mScroller->GetCurrVelocity());
            Boolean bFlag = FALSE;
            mEdgeGlowTop->IsFinished(&bFlag);
            if (!bFlag) {
                mEdgeGlowTop->OnRelease();
            }
        }
    }

    if (rangeX > 0) {
        if (x < 0 && oldX >= 0) {
            mEdgeGlowLeft->OnAbsorb((Int32) mHostView->mScroller->GetCurrVelocity());
            Boolean bFlag = FALSE;
            mEdgeGlowRight->IsFinished(&bFlag);
            if (!bFlag) {
                mEdgeGlowRight->OnRelease();
            }
        }
        else if (x > rangeX && oldX <= rangeX) {
            mEdgeGlowRight->OnAbsorb((Int32) mHostView->mScroller->GetCurrVelocity());
            Boolean bFlag = FALSE;
            mEdgeGlowLeft->IsFinished(&bFlag);
            if (!bFlag) {
                mEdgeGlowLeft->OnRelease();
            }
        }
    }
}

/**
 * Draw the glow effect along the sides of the widget. mEdgeGlow* must be non-null.
 *
 * @param canvas Canvas to draw into, transformed into view coordinates.
 * @return true if glow effects are still animating and the view should invalidate again.
 */
Boolean OverScrollGlow::DrawEdgeGlows(
    /* [in] */ ICanvas* canvas)
{
    Int32 scrollX = mHostView->GetScrollX();
    Int32 scrollY = mHostView->GetScrollY();
    Int32 width = mHostView->GetWidth();
    Int32 height = mHostView->GetHeight();

    Boolean invalidateForGlow = FALSE;
    Boolean _invalidateForGlow = FALSE;
    Boolean isFinished = FALSE;
    mEdgeGlowTop->IsFinished(&isFinished);
    if (!isFinished) {
        Int32 restoreCount;

        canvas->Save(&restoreCount);
        Int32 visibleTitleHeight;
        mHostView->GetVisibleTitleHeight(&visibleTitleHeight);
        canvas->Translate(scrollX, visibleTitleHeight + Elastos::Core::Math::Min(0, scrollY));
        mEdgeGlowTop->SetSize(width, height);
        mEdgeGlowTop->Draw(canvas, &_invalidateForGlow);
        invalidateForGlow |= _invalidateForGlow;
        canvas->RestoreToCount(restoreCount);
    }

    mEdgeGlowBottom->IsFinished(&isFinished);
    if (!isFinished) {
        Int32 restoreCount;

        canvas->Save(&restoreCount);
        canvas->Translate(-width + scrollX, Elastos::Core::Math::Max(mHostView->ComputeMaxScrollY(), scrollY)
                + height);
//        canvas->Rotate(180, width, 0);
        mEdgeGlowBottom->SetSize(width, height);
        mEdgeGlowBottom->Draw(canvas, &_invalidateForGlow);
        invalidateForGlow |= _invalidateForGlow;
        canvas->RestoreToCount(restoreCount);
    }

    mEdgeGlowLeft->IsFinished(&isFinished);
    if (!isFinished) {
        Int32 restoreCount;
        canvas->Save(&restoreCount);
        canvas->Rotate(270);
        canvas->Translate(-height - scrollY, Elastos::Core::Math::Min(0, scrollX));
        mEdgeGlowLeft->SetSize(height, width);
        mEdgeGlowLeft->Draw(canvas, &_invalidateForGlow);
        invalidateForGlow |= _invalidateForGlow;
        canvas->RestoreToCount(restoreCount);
    }

    mEdgeGlowRight->IsFinished(&isFinished);
    if (!isFinished) {
        Int32 restoreCount;
        canvas->Save(&restoreCount);
        canvas->Rotate(90);
        canvas->Translate(scrollY,
                -(Elastos::Core::Math::Max(mHostView->ComputeMaxScrollX(), scrollX) + width));
        mEdgeGlowRight->SetSize(height, width);
        mEdgeGlowRight->Draw(canvas, &_invalidateForGlow);
        invalidateForGlow |= _invalidateForGlow;
        canvas->RestoreToCount(restoreCount);
    }

    return invalidateForGlow;
}

/**
 * @return True if any glow is still animating
 */
Boolean OverScrollGlow::IsAnimating()
{
    Boolean bFlag1, bFlag2, bFlag3, bFlag4;

    mEdgeGlowTop->IsFinished(&bFlag1);
    mEdgeGlowBottom->IsFinished(&bFlag2);
    mEdgeGlowLeft->IsFinished(&bFlag3);
    mEdgeGlowRight->IsFinished(&bFlag4);

    return (!bFlag1 || !bFlag2 || !bFlag3 || !bFlag4);
}

/**
 * Release all glows from any touch pulls in progress.
 */
void OverScrollGlow::ReleaseAll()
{
    mEdgeGlowTop->OnRelease();
    mEdgeGlowBottom->OnRelease();
    mEdgeGlowLeft->OnRelease();
    mEdgeGlowRight->OnRelease();
}

} // namespace Webkit
} // namepsace Droid
} // namespace Elastos