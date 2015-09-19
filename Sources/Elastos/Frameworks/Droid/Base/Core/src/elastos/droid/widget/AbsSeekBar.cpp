
#include "widget/AbsSeekBar.h"
#include "view/CViewConfiguration.h"
#include <elastos/core/Math.h>

using Elastos::Core::CStringWrapper;
using Elastos::Droid::Graphics::Drawable::IDrawableCallback;
using Elastos::Droid::Graphics::Drawable::EIID_IDrawableCallback;
using Elastos::Droid::View::CViewConfiguration;

namespace Elastos {
namespace Droid {
namespace Widget {

const Int32 AbsSeekBar::NO_ALPHA;


AbsSeekBar::AbsSeekBar()
    : mThumbOffset(0)
    , mKeyProgressIncrement(1)
    , mDisabledAlpha(0.0f)
    , mScaledTouchSlop(0)
    , mTouchDownX(0.0f)
    , mIsDragging(FALSE)
    , mTouchProgressOffset(0.f)
    , mIsUserSeekable(TRUE)
{}

AbsSeekBar::AbsSeekBar(
    /* [in] */ IContext* context)
    : ProgressBar(context)
    , mThumbOffset(0)
    , mKeyProgressIncrement(1)
    , mDisabledAlpha(0.0f)
    , mScaledTouchSlop(0)
    , mTouchDownX(0.0f)
    , mIsDragging(FALSE)
    , mTouchProgressOffset(0.f)
    , mIsUserSeekable(TRUE)
{}

AbsSeekBar::AbsSeekBar(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
    : ProgressBar(context, attrs)
    , mThumbOffset(0)
    , mKeyProgressIncrement(1)
    , mDisabledAlpha(0.0f)
    , mScaledTouchSlop(0)
    , mTouchDownX(0.0f)
    , mIsDragging(FALSE)
    , mTouchProgressOffset(0.f)
    , mIsUserSeekable(TRUE)
{}

AbsSeekBar::AbsSeekBar(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
    : ProgressBar(context, attrs, defStyle)
    , mThumbOffset(0)
    , mKeyProgressIncrement(1)
    , mDisabledAlpha(0.0f)
    , mScaledTouchSlop(0)
    , mTouchDownX(0.0f)
    , mIsDragging(FALSE)
    , mTouchProgressOffset(0.f)
    , mIsUserSeekable(TRUE)
{
    ASSERT_SUCCEEDED(InitFromAttributes(context, attrs, defStyle));
}

ECode AbsSeekBar::Init(
    /* [in] */ IContext* context)
{
    return ProgressBar::Init(context);
}

ECode AbsSeekBar::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return ProgressBar::Init(context, attrs);
}

ECode AbsSeekBar::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    FAIL_RETURN(ProgressBar::Init(context, attrs, defStyle));
    return InitFromAttributes(context, attrs, defStyle);
}

ECode AbsSeekBar::InitFromAttributes(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::SeekBar),
            ARRAY_SIZE(R::styleable::SeekBar));
    AutoPtr<ITypedArray> a;

    FAIL_RETURN(context->ObtainStyledAttributes(
            attrs, attrIds, defStyle, 0, (ITypedArray**)&a));
    AutoPtr<IDrawable> thumb;
    FAIL_RETURN(a->GetDrawable(R::styleable::SeekBar_thumb,
            (IDrawable**)&thumb));
    // ...but allow layout to override this
    Int32 thumbOffset = 0;
    FAIL_RETURN(a->GetDimensionPixelOffset(
        R::styleable::SeekBar_thumbOffset,
            GetThumbOffset(), &thumbOffset));
    SetThumb(thumb);
    SetThumbOffset(thumbOffset);

    FAIL_RETURN(a->Recycle());

    a = NULL;
    attrIds = ArrayOf<Int32>::Alloc(
                const_cast<Int32 *>(R::styleable::Theme),
                ARRAY_SIZE(R::styleable::Theme));
    FAIL_RETURN(context->ObtainStyledAttributes(
            attrs, attrIds, 0, 0, (ITypedArray**)&a));
    FAIL_RETURN(a->GetFloat(
        R::styleable::Theme_disabledAlpha,
            0.5f, &mDisabledAlpha));
    ECode ec = a->Recycle();

    CViewConfiguration::Get(context)->GetScaledTouchSlop(&mScaledTouchSlop);

    return ec;

}

/**
 * Sets the thumb that will be drawn at the end of the progress meter within the SeekBar.
 * <p>
 * If the thumb is a valid drawable (i.e. not NULL), half its width will be
 * used as the new thumb offset (@see #setThumbOffset(Int32)).
 *
 * @param thumb Drawable representing the thumb
 */
ECode AbsSeekBar::SetThumb(
    /* [in] */ IDrawable* thumb)
{
    Boolean needUpdate = FALSE;
    // This way, calling setThumb again with the same bitmap will result in
    // it recalcuating mThumbOffset (if for example it the bounds of the
    // drawable changed)
    if (mThumb != NULL && thumb != mThumb) {
        mThumb->SetCallback(NULL);
        needUpdate = TRUE;
    } else {
        needUpdate = FALSE;
    }

    if (thumb != NULL) {
        thumb->SetCallback((IDrawableCallback*)this->Probe(EIID_IDrawableCallback));
        if (CanResolveLayoutDirection()) {
            thumb->SetLayoutDirection(GetLayoutDirection());
        }
        // Assuming the thumb drawable is symmetric, set the thumb offset
        // such that the thumb will hang halfway off either edge of the
        // progress bar.
        thumb->GetIntrinsicWidth(&mThumbOffset);
        mThumbOffset /= 2;

        if (needUpdate) {
            Int32 w1, w2, h1, h2;
            thumb->GetIntrinsicWidth(&w1);
            mThumb->GetIntrinsicWidth(&w2);
            thumb->GetIntrinsicHeight(&h1);
            mThumb->GetIntrinsicHeight(&h2);
            // If we're updating get the new states
            if (w1 != w2 || h1 != h2) {
                    RequestLayout();
            }
        }

    }
    mThumb = thumb;
    Invalidate();
    if (needUpdate) {
        UpdateThumbPos(GetWidth(), GetHeight());
        Boolean res;
        if (thumb != NULL && (thumb->IsStateful(&res), res)) {
            // Note that if the states are different this won't work.
            // For now, let's consider that an app bug.
            AutoPtr<ArrayOf<Int32> > state = GetDrawableState();
            Boolean res;
            thumb->SetState(state ,&res);
        }
    }
    return NOERROR;
}

/**
 * Return the drawable used to represent the scroll thumb - the component that
 * the user can drag back and forth indicating the current value by its position.
 *
 * @return The current thumb drawable
 */
AutoPtr<IDrawable> AbsSeekBar::GetThumb()
{
    return mThumb;
}

/**
 * @see #setThumbOffset(Int32)
 */
Int32 AbsSeekBar::GetThumbOffset()
{
    return mThumbOffset;
}

/**
 * Sets the thumb offset that allows the thumb to extend out of the range of
 * the track.
 *
 * @param thumbOffset The offset amount in pixels.
 */
ECode AbsSeekBar::SetThumbOffset(
    /* [in] */ Int32 thumbOffset)
{
    mThumbOffset = thumbOffset;
    Invalidate();

    return NOERROR;
}

/**
 * Sets the amount of progress changed via the arrow keys.
 *
 * @param increment The amount to increment or decrement when the user
 *            presses the arrow keys.
 */
ECode AbsSeekBar::SetKeyProgressIncrement(
    /* [in] */ Int32 increment)
{
    mKeyProgressIncrement = increment < 0 ? -increment : increment;

    return NOERROR;
}

/**
 * Returns the amount of progress changed via the arrow keys.
 * <p>
 * By default, this will be a value that is derived from the max progress.
 *
 * @return The amount to increment or decrement when the user presses the
 *         arrow keys. This will be positive.
 */
Int32 AbsSeekBar::GetKeyProgressIncrement()
{
    return mKeyProgressIncrement;
}

ECode AbsSeekBar::SetMax(
    /* [in] */ Int32 max)
{
    AutoLock lock(GetSelfLock());

    ProgressBar::SetMax(max);

    if ((mKeyProgressIncrement == 0) || (GetMax() / mKeyProgressIncrement > 20)) {
        // It will take the user too long to change this via keys, change it
        // to something more reasonable
        SetKeyProgressIncrement(Elastos::Core::Math::Max(1, Elastos::Core::Math::Round((Float) GetMax() / 20)));
    }

    return NOERROR;
}

Boolean AbsSeekBar::VerifyDrawable(
    /* [in] */ IDrawable* who)
{
    return who == mThumb || ProgressBar::VerifyDrawable(who);
}

ECode AbsSeekBar::JumpDrawablesToCurrentState()
{
    ProgressBar::JumpDrawablesToCurrentState();
    if (mThumb != NULL) mThumb->JumpToCurrentState();
    return NOERROR;
}


ECode AbsSeekBar::DrawableStateChanged()
{
    FAIL_RETURN(ProgressBar::DrawableStateChanged());

    AutoPtr<IDrawable> progressDrawable = GetProgressDrawable();
    if (progressDrawable != NULL) {
        progressDrawable->SetAlpha(IsEnabled() ? NO_ALPHA : (Int32) (NO_ALPHA * mDisabledAlpha));
    }

    Boolean stateful;
    if (mThumb != NULL && (mThumb->IsStateful(&stateful), stateful)) {
        AutoPtr<ArrayOf<Int32> > state = GetDrawableState();
        Boolean res;
        mThumb->SetState(state, &res);
    }

    return NOERROR;
}

void AbsSeekBar::OnProgressRefresh(
    /* [in] */ Float scale,
    /* [in] */ Boolean fromUser)
{
    ProgressBar::OnProgressRefresh(scale, fromUser);

    AutoPtr<IDrawable> thumb = mThumb;
    if (thumb != NULL) {
        SetThumbPos(GetWidth(), thumb, scale, Elastos::Core::Math::INT32_MIN_VALUE);
        /*
         * Since we draw translated, the drawable's bounds that it signals
         * for invalidation won't be the actual bounds we want invalidated,
         * so just invalidate this whole view.
         */
        Invalidate();
    }
}

void AbsSeekBar::OnSizeChanged(
    /* [in] */ Int32 w,
    /* [in] */ Int32 h,
    /* [in] */ Int32 oldw,
    /* [in] */ Int32 oldh)
{
    ProgressBar::OnSizeChanged(w, h, oldw, oldh);
    UpdateThumbPos(w, h);
}

void AbsSeekBar::UpdateThumbPos(
    /* [in] */ Int32 w,
    /* [in] */ Int32 h)
{

    AutoPtr<IDrawable> d = GetCurrentDrawable();
    AutoPtr<IDrawable> thumb = mThumb;
    Int32 height;
    Int32 thumbHeight = thumb == NULL ? 0 : (thumb->GetIntrinsicHeight(&height), height);
    // The max height does not incorporate padding, whereas the height
    // parameter does
    Int32 trackHeight = Elastos::Core::Math::Min(mMaxHeight, h - mPaddingTop - mPaddingBottom);

    Int32 max = GetMax();
    Float scale = max > 0 ? (Float) GetProgress() / (Float) max : 0;

    if (thumbHeight > trackHeight) {
        if (thumb != NULL) {
            SetThumbPos(w, thumb, scale, 0);
        }
        Int32 gapForCenteringTrack = (thumbHeight - trackHeight) / 2;
        if (d != NULL) {
            // Canvas will be translated by the padding, so 0,0 is where we start drawing
            d->SetBounds(0, gapForCenteringTrack,
                    w - mPaddingRight - mPaddingLeft, h - mPaddingBottom - gapForCenteringTrack
                    - mPaddingTop);
        }
    }
    else {
        if (d != NULL) {
            // Canvas will be translated by the padding, so 0,0 is where we start drawing
            d->SetBounds(0, 0, w - mPaddingRight - mPaddingLeft, h - mPaddingBottom
                    - mPaddingTop);
        }
        Int32 gap = (trackHeight - thumbHeight) / 2;
        if (thumb != NULL) {
            SetThumbPos(w, thumb, scale, gap);
        }
    }
}

/**
 * @param gap If set to {@link Integer#MIN_VALUE}, this will be ignored and
 */
void AbsSeekBar::SetThumbPos(
    /* [in] */ Int32 w,
    /* [in] */ IDrawable* thumb,
    /* [in] */ Float scale,
    /* [in] */ Int32 gap)
{
    Int32 available = w - mPaddingLeft - mPaddingRight;
    Int32 thumbWidth;
    thumb->GetIntrinsicWidth(&thumbWidth);
    Int32 thumbHeight;
    thumb->GetIntrinsicHeight(&thumbHeight);
    available -= thumbWidth;

    // The extra space for the thumb to move on the track
    available += mThumbOffset * 2;

    Int32 thumbPos = (Int32) (scale * available);

    Int32 topBound, bottomBound;
    if (gap == Elastos::Core::Math::INT32_MIN_VALUE) {
        AutoPtr<IRect> oldBounds;
        thumb->GetBounds((IRect**)&oldBounds);
        oldBounds->GetTop(&topBound);
        oldBounds->GetBottom(&bottomBound);
    }
    else {
        topBound = gap;
        bottomBound = gap + thumbHeight;
    }

    // Canvas will be translated, so 0,0 is where we start drawing
    Int32 left = IsLayoutRtl() ? available - thumbPos : thumbPos;
    thumb->SetBounds(left, topBound, left + thumbWidth, bottomBound);
}

/**
 * @hide
 */

ECode AbsSeekBar::OnResolveDrawables(
    /* [in] */ Int32 layoutDirection)
{
    ProgressBar::OnResolveDrawables(layoutDirection);

    if (mThumb != NULL) {
        mThumb->SetLayoutDirection(layoutDirection);
    }
    return NOERROR;
}

void AbsSeekBar::OnDraw(
    /* [in] */ ICanvas* canvas)
{
    AutoLock lock(GetSelfLock());

    ProgressBar::OnDraw(canvas);
    if (mThumb != NULL) {
        Int32 count;
        canvas->Save(&count);
        // Translate the padding. For the x, we need to allow the thumb to
        // draw in its extra space
        canvas->Translate(mPaddingLeft - mThumbOffset, mPaddingTop);
        mThumb->Draw(canvas);
        canvas->Restore();
    }
}

void AbsSeekBar::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    AutoLock lock(GetSelfLock());

    AutoPtr<IDrawable> d = GetCurrentDrawable();

    Int32 h;
    Int32 thumbHeight = mThumb == NULL ? 0 : (mThumb->GetIntrinsicHeight(&h), h);
    Int32 dw = 0;
    Int32 dh = 0;
    if (d != NULL) {
        dw = Elastos::Core::Math::Max(mMinWidth, Elastos::Core::Math::Min(mMaxWidth, (d->GetIntrinsicWidth(&dw), dw)));
        dh = Elastos::Core::Math::Max(mMinHeight, Elastos::Core::Math::Min(mMaxHeight, (d->GetIntrinsicHeight(&dh), dh)));
        dh = Elastos::Core::Math::Max(thumbHeight, dh);
    }
    dw += mPaddingLeft + mPaddingRight;
    dh += mPaddingTop + mPaddingBottom;

    SetMeasuredDimension(ResolveSizeAndState(dw, widthMeasureSpec, 0),
            ResolveSizeAndState(dh, heightMeasureSpec, 0));
}

Boolean AbsSeekBar::OnTouchEvent(
    /* [in] */ IMotionEvent* event)
{
    if (!mIsUserSeekable || !IsEnabled()) {
        return FALSE;
    }

    Int32 action;
    switch (event->GetAction(&action), action) {
        case IMotionEvent::ACTION_DOWN:
            if (IsInScrollingContainer()) {
                event->GetX(&mTouchDownX);
            } else {
                SetPressed(TRUE);
                if (mThumb != NULL) {
                    AutoPtr<IRect> bounds;
                    mThumb->GetBounds((IRect**)&bounds);
                    Invalidate(bounds); // This may be within the padding region
                }
                OnStartTrackingTouch();
                TrackTouchEvent(event);
                AttemptClaimDrag();
            }
            break;

        case IMotionEvent::ACTION_MOVE:
            if (mIsDragging) {
                TrackTouchEvent(event);
            } else {
                Float x;
                event->GetX(&x);
                if (Elastos::Core::Math::Abs(x - mTouchDownX) > mScaledTouchSlop) {
                    SetPressed(TRUE);
                    if (mThumb != NULL) {
                        AutoPtr<IRect> bounds;
                        mThumb->GetBounds((IRect**)&bounds);
                        Invalidate(bounds); // This may be within the padding region
                    }
                    OnStartTrackingTouch();
                    TrackTouchEvent(event);
                    AttemptClaimDrag();
                }
            }
            break;

        case IMotionEvent::ACTION_UP:
            if (mIsDragging) {
                TrackTouchEvent(event);
                OnStopTrackingTouch();
                SetPressed(FALSE);
            } else {
                // Touch up when we never crossed the touch slop threshold should
                // be interpreted as a tap-seek to that location.
                OnStartTrackingTouch();
                TrackTouchEvent(event);
                OnStopTrackingTouch();
            }
            // ProgressBar doesn't know to repaint the thumb drawable
            // in its inactive state when the touch stops (because the
            // value has not apparently changed)
            Invalidate();
            break;

        case IMotionEvent::ACTION_CANCEL:
            if (mIsDragging) {
                OnStopTrackingTouch();
                SetPressed(FALSE);
            }
            Invalidate(); // see above explanation
            break;
    }
    return TRUE;
}

void AbsSeekBar::TrackTouchEvent(
    /* [in] */ IMotionEvent* event)
{
    Int32 width = GetWidth();
    Int32 available = width - mPaddingLeft - mPaddingRight;
    Float x;
    event->GetX(&x);
    Float scale;
    Float progress = 0;

    if (IsLayoutRtl()) {
        if (x > width - mPaddingRight) {
            scale = 0.0f;
        } else if (x < mPaddingLeft) {
            scale = 1.0f;
        } else {
            scale = (Float)(available - x + mPaddingLeft) / (Float)available;
            progress = mTouchProgressOffset;
        }
    } else {
        if (x < mPaddingLeft) {
            scale = 0.0f;
        }
        else if (x > width - mPaddingRight) {
            scale = 1.0f;
        }
        else {
            scale = (Float)(x - mPaddingLeft) / (Float)available;
            progress = mTouchProgressOffset;
        }
    }

    Int32 max = GetMax();
    progress += scale * max;

    SetProgress((Int32)progress, TRUE);
}

/**
 * Tries to claim the user's drag motion, and requests disallowing any
 * ancestors from stealing events in the drag.
 */
void AbsSeekBar::AttemptClaimDrag()
{
    if (mParent != NULL) {
        mParent->RequestDisallowInterceptTouchEvent(TRUE);
    }
}

/**
 * This is called when the user has started touching this widget.
 */
void AbsSeekBar::OnStartTrackingTouch()
{
    mIsDragging = TRUE;
}

/**
 * This is called when the user either releases his touch or the touch is
 * canceled.
 */
void AbsSeekBar::OnStopTrackingTouch()
{
    mIsDragging = FALSE;
}

/**
 * Called when the user changes the seekbar's progress by using a key event.
 */
void AbsSeekBar::OnKeyChange()
{
}

Boolean AbsSeekBar::OnKeyDown(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    if (IsEnabled()) {
        Int32 progress = GetProgress();
        switch (keyCode) {
            case IKeyEvent::KEYCODE_DPAD_LEFT:
                if (progress <= 0) break;
                SetProgress(progress - mKeyProgressIncrement, TRUE);
                OnKeyChange();
                return TRUE;

            case IKeyEvent::KEYCODE_DPAD_RIGHT:
                if (progress >= GetMax()) break;
                SetProgress(progress + mKeyProgressIncrement, TRUE);
                OnKeyChange();
                return TRUE;
        }
    }

    return ProgressBar::OnKeyDown(keyCode, event);
}

//@Override
ECode AbsSeekBar::OnInitializeAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    ProgressBar::OnInitializeAccessibilityEvent(event);
    String sName("AbsSeekBar");
    AutoPtr<ICharSequence> className;
    FAIL_RETURN(CStringWrapper::New(sName, (ICharSequence**)&className));
    event->SetClassName(className);
    return NOERROR;
}

//@Override
ECode AbsSeekBar::OnInitializeAccessibilityNodeInfo(
    /* [in] */ IAccessibilityNodeInfo* info)
{
    ProgressBar::OnInitializeAccessibilityNodeInfo(info);
    String sName("AbsSeekBar");
    AutoPtr<ICharSequence> className;
    FAIL_RETURN(CStringWrapper::New(sName, (ICharSequence**)&className));
    info->SetClassName(className);

    if (IsEnabled()) {
        Int32 progress = GetProgress();
        if (progress > 0) {
            info->AddAction(IAccessibilityNodeInfo::ACTION_SCROLL_BACKWARD);
        }
        if (progress < GetMax()) {
            info->AddAction(IAccessibilityNodeInfo::ACTION_SCROLL_FORWARD);
        }
    }
    return NOERROR;
}

//@Override
Boolean AbsSeekBar::PerformAccessibilityAction(
    /* [in] */ Int32 action,
    /* [in] */ IBundle* arguments)
{
    if (ProgressBar::PerformAccessibilityAction(action, arguments)) {
        return TRUE;
    }
    if (!IsEnabled()) {
        return FALSE;
    }
    Int32 progress = GetProgress();
    Int32 increment = Elastos::Core::Math::Max(1, Elastos::Core::Math::Round((Float) GetMax() / 5));
    switch (action) {
        case IAccessibilityNodeInfo::ACTION_SCROLL_BACKWARD: {
            if (progress <= 0) {
                return FALSE;
            }
            SetProgress(progress - increment, TRUE);
            OnKeyChange();
            return TRUE;
        }
        case IAccessibilityNodeInfo::ACTION_SCROLL_FORWARD: {
            if (progress >= GetMax()) {
                return FALSE;
            }
            SetProgress(progress + increment, TRUE);
            OnKeyChange();
            return TRUE;
        }
    }
    return FALSE;
}

//@Override
ECode AbsSeekBar::OnRtlPropertiesChanged(
    /* [in] */ Int32 layoutDirection)
{
    ProgressBar::OnRtlPropertiesChanged(layoutDirection);

    Int32 max = GetMax();
    Float scale = max > 0 ? (Float) GetProgress() / (Float) max : 0;

    AutoPtr<IDrawable> thumb = mThumb;
    if (thumb != NULL) {
        SetThumbPos(GetWidth(), thumb, scale, Elastos::Core::Math::INT32_MIN_VALUE);
        /*
         * Since we draw translated, the drawable's bounds that it signals
         * for invalidation won't be the actual bounds we want invalidated,
         * so just invalidate this whole view.
         */
        Invalidate();
    }
    return NOERROR;
}

}// namespace Elastos
}// namespace Droid
}// namespace Widget
