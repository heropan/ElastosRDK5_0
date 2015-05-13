
#include "widget/ScrollView.h"
#include <elastos/Math.h>
#include <R.h>
#include "view/FocusFinder.h"
#include "view/CViewConfiguration.h"
#include "view/CViewGroupLayoutParams.h"
#include "view/CViewGroupMarginLayoutParams.h"
#include "view/animation/AnimationUtils.h"
#include "widget/CFrameLayoutLayoutParams.h"
#include "widget/OverScroller.h"
#include "widget/CEdgeEffect.h"
#include "os/CStrictMode.h";

using Elastos::Core::CStringWrapper;
using Elastos::Core::ICharSequence;
using Elastos::Droid::R;
using Elastos::Droid::View::EIID_IView;
using Elastos::Droid::View::EIID_IViewGroup;
using Elastos::Droid::View::EIID_IViewParent;
using Elastos::Droid::View::IInputDevice;
using Elastos::Droid::View::IViewGroupMarginLayoutParams;
using Elastos::Droid::View::CViewGroupMarginLayoutParams;
using Elastos::Droid::View::IViewGroupLayoutParams;
using Elastos::Droid::View::CViewGroupLayoutParams;
using Elastos::Droid::View::IViewGroup;
using Elastos::Droid::View::IViewParent;
using Elastos::Droid::View::FocusFinder;
using Elastos::Droid::View::CViewConfiguration;
using Elastos::Droid::View::Animation::AnimationUtils;
using Elastos::Droid::Os::IStrictMode;
using Elastos::Droid::Os::CStrictMode;

namespace Elastos {
namespace Droid {
namespace Widget {

const Int32 ScrollView::ANIMATED_SCROLL_GAP;
const Float ScrollView::MAX_SCROLL_FACTOR;
const Int32 ScrollView::INVALID_POINTER;

const String ScrollView::TAG = String("ScrollView");

ScrollView::ScrollView()
    : mLastScroll(0)
    , mLastMotionY(0)
    , mIsLayoutDirty(TRUE)
    , mIsBeingDragged(FALSE)
    , mFillViewport(FALSE)
    , mSmoothScrollingEnabled(TRUE)
    , mTouchSlop(0)
    , mMinimumVelocity(0)
    , mMaximumVelocity(0)
    , mOverscrollDistance(0)
    , mOverflingDistance(0)
    , mActivePointerId(INVALID_POINTER)
{
    ASSERT_SUCCEEDED(CRect::NewByFriend((CRect**)&mTempRect));
}

ScrollView::ScrollView(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
    : FrameLayout(context, attrs, R::attr::scrollViewStyle)
    , mLastScroll(0)
    , mLastMotionY(0)
    , mIsLayoutDirty(TRUE)
    , mIsBeingDragged(FALSE)
    , mFillViewport(FALSE)
    , mSmoothScrollingEnabled(TRUE)
    , mTouchSlop(0)
    , mMinimumVelocity(0)
    , mMaximumVelocity(0)
    , mOverscrollDistance(0)
    , mOverflingDistance(0)
    , mActivePointerId(INVALID_POINTER)
{
    ASSERT_SUCCEEDED(CRect::NewByFriend((CRect**)&mTempRect));
    InitScrollView();
    ASSERT_SUCCEEDED(InitFromAttributes(context, attrs, R::attr::scrollViewStyle));
}

ScrollView::ScrollView(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
    : FrameLayout(context, attrs, defStyle)
    , mLastScroll(0)
    , mLastMotionY(0)
    , mIsLayoutDirty(TRUE)
    , mIsBeingDragged(FALSE)
    , mFillViewport(FALSE)
    , mSmoothScrollingEnabled(TRUE)
    , mTouchSlop(0)
    , mMinimumVelocity(0)
    , mMaximumVelocity(0)
    , mOverscrollDistance(0)
    , mOverflingDistance(0)
    , mActivePointerId(INVALID_POINTER)
{
    ASSERT_SUCCEEDED(CRect::NewByFriend((CRect**)&mTempRect));
    InitScrollView();
    ASSERT_SUCCEEDED(InitFromAttributes(context, attrs, defStyle));
}

ScrollView::~ScrollView()
{
}

ECode ScrollView::Init(
    /* [in] */ IContext* context)
{
    return Init(context, NULL);
}

ECode ScrollView::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return Init(context, attrs, R::attr::scrollViewStyle);
}

ECode ScrollView::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    ASSERT_SUCCEEDED(FrameLayout::Init(context, attrs, defStyle));
    InitScrollView();
    return InitFromAttributes(context, attrs, defStyle);
}

ECode ScrollView::InitFromAttributes(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::ScrollView),
            ARRAY_SIZE(R::styleable::ScrollView));
    AutoPtr<ITypedArray> a;
    FAIL_RETURN(context->ObtainStyledAttributesEx3(
            attrs, attrIds, defStyle, 0, (ITypedArray**)&a));

    Boolean value;
    a->GetBoolean(R::styleable::ScrollView_fillViewport, FALSE, &value);

    FAIL_RETURN(SetFillViewport(value));

    a->Recycle();

    return NOERROR;
}

Boolean ScrollView::ShouldDelayChildPressedState()
{
    return TRUE;
}

Float ScrollView::GetTopFadingEdgeStrength()
{
    if (GetChildCount() == 0) {
        return 0.0;
    }

    Int32 length = GetVerticalFadingEdgeLength();
    if (mScrollY < length) {
        return mScrollY / (Float) length;
    }

    return 1.0;
}

Float ScrollView::GetBottomFadingEdgeStrength()
{
    if (GetChildCount() == 0) {
        return 0.0;
    }

    Int32 length = GetVerticalFadingEdgeLength();
    Int32 bottomEdge = GetHeight() - mPaddingBottom;
    Int32 bottom;
    GetChildAt(0)->GetBottom(&bottom);
    Int32 span = bottom - mScrollY - bottomEdge;
    if (span < length) {
        return span / (Float)length;
    }

    return 1.0;
}

Int32 ScrollView::GetMaxScrollAmount()
{
    return (Int32)(MAX_SCROLL_FACTOR * (mBottom - mTop));
}

void ScrollView::InitScrollView()
{
    mScroller = new OverScroller(GetContext());
    SetFocusable(TRUE);
    SetDescendantFocusability(ViewGroup::FOCUS_AFTER_DESCENDANTS);
    SetWillNotDraw(FALSE);
    AutoPtr<CViewConfiguration> configuration = CViewConfiguration::Get(mContext);
    configuration->GetScaledTouchSlop(&mTouchSlop);
    configuration->GetScaledMinimumFlingVelocity(&mMinimumVelocity);
    configuration->GetScaledMaximumFlingVelocity(&mMaximumVelocity);
    configuration->GetScaledOverscrollDistance(&mOverscrollDistance);
    configuration->GetScaledOverflingDistance(&mOverflingDistance);
}

ECode ScrollView::AddView(
    /* [in] */ IView* child)
{
    if (GetChildCount() > 0) {
//        throw new IllegalStateException("ScrollView can host only one direct child");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    return FrameLayout::AddView(child);
}

ECode ScrollView::AddView(
    /* [in] */ IView* child,
    /* [in] */ Int32 index)
{
    if (GetChildCount() > 0) {
//        throw new IllegalStateException("ScrollView can host only one direct child");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    return FrameLayout::AddView(child, index);
}

ECode ScrollView::AddView(
    /* [in] */ IView* child,
    /* [in] */ IViewGroupLayoutParams* params)
{
    if (GetChildCount() > 0) {
//        throw new IllegalStateException("ScrollView can host only one direct child");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    return FrameLayout::AddView(child, params);
}

ECode ScrollView::AddView(
    /* [in] */ IView* child,
    /* [in] */ Int32 index,
    /* [in] */ IViewGroupLayoutParams* params)
{
    if (GetChildCount() > 0) {
//        throw new IllegalStateException("ScrollView can host only one direct child");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    return FrameLayout::AddView(child, index, params);
}

Boolean ScrollView::CanScroll()
{
    AutoPtr<IView> child = GetChildAt(0);
    if (child != NULL) {
        Int32 childHeight;
        child->GetHeight(&childHeight);
        return GetHeight() < childHeight + mPaddingTop + mPaddingBottom;
    }
    return FALSE;
}

Boolean ScrollView::IsFillViewport()
{
    return mFillViewport;
}

ECode ScrollView::SetFillViewport(
    /* [in] */ Boolean fillViewport)
{
    if (fillViewport != mFillViewport) {
        mFillViewport = fillViewport;
        RequestLayout();
    }

    return NOERROR;
}

Boolean ScrollView::IsSmoothScrollingEnabled()
{
    return mSmoothScrollingEnabled;
}

ECode ScrollView::SetSmoothScrollingEnabled(
    /* [in] */ Boolean smoothScrollingEnabled)
{
    mSmoothScrollingEnabled = smoothScrollingEnabled;

    return NOERROR;
}

void ScrollView::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    FrameLayout::OnMeasure(widthMeasureSpec, heightMeasureSpec);

    if (!mFillViewport) {
        return;
    }

    Int32 heightMode = MeasureSpec::GetMode(heightMeasureSpec);
    if (heightMode == MeasureSpec::UNSPECIFIED) {
        return;
    }

    if (GetChildCount() > 0) {
        AutoPtr<IView> child = GetChildAt(0);
        Int32 height = FrameLayout::GetMeasuredHeight();
        Int32 childHeight;
        child->GetMeasuredHeight(&childHeight);
        if (childHeight < height) {
            AutoPtr<IFrameLayoutLayoutParams> params;
            child->GetLayoutParams((IViewGroupLayoutParams**)&params);

            Int32 childWidthMeasureSpec = GetChildMeasureSpec(widthMeasureSpec,
                    mPaddingLeft + mPaddingRight,
                    ((CFrameLayoutLayoutParams*)params.Get())->mWidth);
            height -= mPaddingTop;
            height -= mPaddingBottom;
            Int32 childHeightMeasureSpec =
                    MeasureSpec::MakeMeasureSpec(height, MeasureSpec::EXACTLY);

            child->Measure(childWidthMeasureSpec, childHeightMeasureSpec);
        }
    }
}

Boolean ScrollView::DispatchKeyEvent(
    /* [in] */ IKeyEvent* event)
{
    //PEL("ScrollView::DispatchKeyEvent")
    // Let the focused view and/or our descendants get the key first
    return FrameLayout::DispatchKeyEvent(event) || ExecuteKeyEvent(event);
}

Boolean ScrollView::ExecuteKeyEvent(
    /* [in] */ IKeyEvent* event)
{
    mTempRect->SetEmpty();

    if (!CanScroll()) {
        Int32 keyCode;
        event->GetKeyCode(&keyCode);
        if (IsFocused() && keyCode != IKeyEvent::KEYCODE_BACK) {
            AutoPtr<IView> currentFocused = FindFocus();
            if (currentFocused.Get() == THIS_PROBE(IView)) {
                currentFocused = NULL;
            }

            AutoPtr<FocusFinder> finder = FocusFinder::GetInstance();
            AutoPtr<IView> nextFocused;
            finder->FindNextFocus(THIS_PROBE(IViewGroup),
                    currentFocused, IView::FOCUS_DOWN, (IView**)&nextFocused);

            if (nextFocused != NULL && nextFocused.Get() != THIS_PROBE(IView)) {
                Boolean isFocus;
                nextFocused->RequestFocusEx(IView::FOCUS_DOWN, &isFocus);
                return isFocus;
            }
            else return FALSE;
        }
        return FALSE;
    }

    Boolean handled = FALSE;
    Int32 action;
    event->GetAction(&action);
    if (action == IKeyEvent::ACTION_DOWN) {
        Int32 keyCode;
        event->GetKeyCode(&keyCode);
        Boolean isPressed;
        switch (keyCode) {
            case IKeyEvent::KEYCODE_DPAD_UP:
                event->IsAltPressed(&isPressed);
                if (!isPressed) {
                    handled = ArrowScroll(IView::FOCUS_UP);
                }
                else {
                    handled = FullScroll(IView::FOCUS_UP);
                }
                break;
            case IKeyEvent::KEYCODE_DPAD_DOWN:
                event->IsAltPressed(&isPressed);
                if (!isPressed) {
                    handled = ArrowScroll(IView::FOCUS_DOWN);
                }
                else {
                    handled = FullScroll(IView::FOCUS_DOWN);
                }
                break;
            case IKeyEvent::KEYCODE_SPACE:
                event->IsShiftPressed(&isPressed);
                PageScroll(isPressed ? IView::FOCUS_UP : IView::FOCUS_DOWN);
                break;
        }
    }

    return handled;
}

Boolean ScrollView::InChild(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    if (GetChildCount() > 0) {
        Int32 scrollY = mScrollY;
        AutoPtr<IView> child = GetChildAt(0);
        Int32 top, bottom, left, right;
        child->GetTop(&top);
        child->GetBottom(&bottom);
        child->GetLeft(&left);
        child->GetRight(&right);
        return !(y < top - scrollY
                || y >= bottom - scrollY
                || x < left
                || x >= right);
    }

    return FALSE;
}

void ScrollView::InitOrResetVelocityTracker()
{
    if (mVelocityTracker == NULL) {
        mVelocityTracker = VelocityTracker::Obtain();
    } else {
        mVelocityTracker->Clear();
    }
}

void ScrollView::InitVelocityTrackerIfNotExists()
{
    if (mVelocityTracker == NULL) {
        mVelocityTracker = VelocityTracker::Obtain();
    }
}

void ScrollView::RecycleVelocityTracker()
{
    if (mVelocityTracker != NULL) {
        mVelocityTracker->Recycle();
        mVelocityTracker = NULL;
    }
}

ECode ScrollView::RequestDisallowInterceptTouchEvent(
    /* [in] */ Boolean disallowIntercept)
{
    if (disallowIntercept) {
        RecycleVelocityTracker();
    }
    FrameLayout::RequestDisallowInterceptTouchEvent(disallowIntercept);
    return NOERROR;
}

Boolean ScrollView::OnInterceptTouchEvent(
    /* [in] */ IMotionEvent* ev)
{
    /*
     * This method JUST determines whether we want to intercept the motion.
     * If we return TRUE, onMotionEvent will be called and we do the actual
     * scrolling there.
     */

    /*
    * Shortcut the most recurring case: the user is in the dragging
    * state and he is moving his finger.  We want to intercept this
    * motion.
    */
    Int32 action;
    ev->GetAction(&action);
    if ((action == IMotionEvent::ACTION_MOVE) && (mIsBeingDragged)) {
        return TRUE;
    }

    /*
     * Don't try to intercept touch if we can't scroll anyway.
     */
    if (GetScrollY() == 0 && !CanScrollVertically(1)) {
        return FALSE;
    }

    switch (action & IMotionEvent::ACTION_MASK) {
        case IMotionEvent::ACTION_MOVE: {
            /*
             * mIsBeingDragged == FALSE, otherwise the shortcut would have caught it. Check
             * whether the user has moved far enough from his original down touch.
             */

            /*
            * Locally do absolute value. mLastMotionY is set to the y value
            * of the down event.
            */
            Int32 activePointerId = mActivePointerId;
            if (activePointerId == INVALID_POINTER) {
                // If we don't have a valid id, the touch down wasn't on content.
                break;
            }

            Int32 pointerIndex;
            ev->FindPointerIndex(activePointerId, &pointerIndex);
            if (pointerIndex == -1) {
//                Log.e(TAG, "Invalid pointerId=" + activePointerId
//                    + " in onInterceptTouchEvent");
                break;
            }

            Float y;
            ev->GetYEx(pointerIndex, &y);
            Int32 yDiff = (Int32)Elastos::Core::Math::Abs(y - mLastMotionY);
            if (yDiff > mTouchSlop) {
                mIsBeingDragged = TRUE;
                mLastMotionY = y;

                InitVelocityTrackerIfNotExists();
                mVelocityTracker->AddMovement(ev);
                if (mScrollStrictSpan == NULL) {
                    AutoPtr<IStrictMode> mode;
                    CStrictMode::AcquireSingleton((IStrictMode**)&mode);
                    mode->EnterCriticalSpan(String("ScrollView-scroll"), (IStrictModeSpan**)&mScrollStrictSpan);
                }
                AutoPtr<IViewParent> parent = GetParent();
                if (parent != NULL) {
                    parent->RequestDisallowInterceptTouchEvent(TRUE);
                }
            }
            break;
        }

        case IMotionEvent::ACTION_DOWN: {
            Float y, x;
            ev->GetY(&y);
            ev->GetX(&x);
            if (!InChild((Int32)x, (Int32)y)) {
                mIsBeingDragged = FALSE;
                RecycleVelocityTracker();
                break;
            }

            /*
             * Remember location of down touch.
             * ACTION_DOWN always refers to pointer index 0.
             */
            mLastMotionY = y;
            ev->GetPointerId(0, &mActivePointerId);

            InitOrResetVelocityTracker();
            mVelocityTracker->AddMovement(ev);

            /*
            * If being flinged and user touches the screen, initiate drag;
            * otherwise don't.  mScroller.isFinished should be FALSE when
            * being flinged.
            */
            mIsBeingDragged = !mScroller->IsFinished();
            AutoPtr<IStrictMode> mode;
            CStrictMode::AcquireSingleton((IStrictMode**)&mode);
            mode->EnterCriticalSpan(String("ScrollView-scroll"), (IStrictModeSpan**)&mScrollStrictSpan);
            break;
        }

        case IMotionEvent::ACTION_CANCEL:
        case IMotionEvent::ACTION_UP:
            /* Release the drag */
            mIsBeingDragged = FALSE;
            mActivePointerId = INVALID_POINTER;
            RecycleVelocityTracker();
            if (mScroller->SpringBack(mScrollX, mScrollY, 0, 0, 0, GetScrollRange())) {
                PostInvalidateOnAnimation();
            }
            break;
        case IMotionEvent::ACTION_POINTER_UP:
            OnSecondaryPointerUp(ev);
            break;
    }

    /*
    * The only time we want to intercept motion events is if we are in the
    * drag mode.
    */
    return mIsBeingDragged;
}

Boolean ScrollView::OnTouchEvent(
    /* [in] */ IMotionEvent* ev)
{
    InitVelocityTrackerIfNotExists();
    mVelocityTracker->AddMovement(ev);

    Int32 action;
    ev->GetAction(&action);
    switch (action & IMotionEvent::ACTION_MASK) {
        case IMotionEvent::ACTION_DOWN: {
            if (GetChildCount() == 0) {
                return FALSE;
            }
            if ((mIsBeingDragged = !mScroller->IsFinished())) {
                AutoPtr<IViewParent> parent = GetParent();
                if (parent != NULL) {
                    parent->RequestDisallowInterceptTouchEvent(TRUE);
                }
            }

            /*
             * If being flinged and user touches, stop the fling. isFinished
             * will be FALSE if being flinged.
             */
            if (!mScroller->IsFinished()) {
                mScroller->AbortAnimation();

                if (mFlingStrictSpan != NULL) {
                    mFlingStrictSpan->Finish();
                    mFlingStrictSpan = NULL;
                }
            }

            // Remember where the motion event started
            Float lasty;
            ev->GetY(&lasty);
            mLastMotionY = lasty;
            ev->GetPointerId(0, &mActivePointerId);
            break;
        }
        case IMotionEvent::ACTION_MOVE: {
            Int32 activePointerIndex;
            ev->FindPointerIndex(mActivePointerId, &activePointerIndex);
            if (activePointerIndex == -1) {
//                Log.e(TAG, "Invalid pointerId=" + mActivePointerId + " in onTouchEvent");
                break;
            }

            Float lasty;
            ev->GetYEx(activePointerIndex, &lasty);
            Int32 y = lasty;
            Int32 deltaY = mLastMotionY - y;
            if (!mIsBeingDragged && Elastos::Core::Math::Abs(deltaY) > mTouchSlop) {
                AutoPtr<IViewParent> parent = GetParent();
                if (parent != NULL) {
                    parent->RequestDisallowInterceptTouchEvent(TRUE);
                }
                mIsBeingDragged = TRUE;
                if (deltaY > 0) {
                    deltaY -= mTouchSlop;
                } else {
                    deltaY += mTouchSlop;
                }
            }

            if (mIsBeingDragged) {
                // Scroll to follow the motion event

                mLastMotionY = y;

                Int32 oldX = mScrollX;
                Int32 oldY = mScrollY;
                Int32 range = GetScrollRange();

                Int32 overscrollMode = GetOverScrollMode();
                Boolean canOverscroll = overscrollMode == IView::OVER_SCROLL_ALWAYS ||
                    (overscrollMode == IView::OVER_SCROLL_IF_CONTENT_SCROLLS && range > 0);

                if (OverScrollBy(0, deltaY, 0, mScrollY, 0, range,
                        0, mOverscrollDistance, TRUE)) {
                    // Break our velocity if we hit a scroll barrier.
                    mVelocityTracker->Clear();
                }
                OnScrollChanged(mScrollX, mScrollY, oldX, oldY);

                if (canOverscroll) {
                    Int32 pulledToY = oldY + deltaY;
                    if (pulledToY < 0) {
                        mEdgeGlowTop->OnPull((Float)deltaY / GetHeight());
                        Boolean bf;
                        mEdgeGlowBottom->IsFinished(&bf);
                        if (!bf) {
                            mEdgeGlowBottom->OnRelease();
                        }
                    }
                    else if (pulledToY > range) {
                        mEdgeGlowBottom->OnPull((Float)deltaY / GetHeight());
                        Boolean tf;
                        mEdgeGlowTop->IsFinished(&tf);
                        if (!tf) {
                            mEdgeGlowTop->OnRelease();
                        }
                    }

                    if (mEdgeGlowTop != NULL) {
                        Boolean tf, bf;
                        mEdgeGlowTop->IsFinished(&tf);
                        mEdgeGlowTop->IsFinished(&bf);
                        if (!tf || !bf) {
                            PostInvalidateOnAnimation();
                        }
                    }
                }
            }
        }
        break;
        case IMotionEvent::ACTION_UP:
            if (mIsBeingDragged) {
                mVelocityTracker->ComputeCurrentVelocityEx(1000, mMaximumVelocity);
                Float y;
                mVelocityTracker->GetYVelocityEx(mActivePointerId, &y);
                Int32 initialVelocity = (Int32)y;

                if (GetChildCount() > 0) {
                    if ((Elastos::Core::Math::Abs(initialVelocity) > mMinimumVelocity)) {
                        Fling(-initialVelocity);
                    }
                    else {
                        if (mScroller->SpringBack(mScrollX, mScrollY, 0, 0, 0,
                            GetScrollRange())) {
                                PostInvalidateOnAnimation();
                        }
                    }
                }

                mActivePointerId = INVALID_POINTER;
                EndDrag();
            }
            break;
        case IMotionEvent::ACTION_CANCEL:
            if (mIsBeingDragged && GetChildCount() > 0) {
                if (mScroller->SpringBack(mScrollX, mScrollY, 0, 0, 0, GetScrollRange())) {
                    PostInvalidateOnAnimation();
                }
                mActivePointerId = INVALID_POINTER;
                EndDrag();
            }
            break;
        case IMotionEvent::ACTION_POINTER_DOWN: {
            Int32 index;
            ev->GetActionIndex(&index);
            Float lasty;
            ev->GetYEx(index, &lasty);
            mLastMotionY = lasty;
            ev->GetPointerId(index, &mActivePointerId);
            break;
        }
        case IMotionEvent::ACTION_POINTER_UP:
            OnSecondaryPointerUp(ev);
            Int32 index;
            ev->FindPointerIndex(mActivePointerId, &index);
            Float lasty;
            ev->GetYEx(index, &lasty);
            mLastMotionY = lasty;
            break;
    }
    return TRUE;
}

Boolean ScrollView::OnGenericMotionEvent(
    /* [in] */ IMotionEvent* event)
{
    //PEL("ScrollView::OnGenericMotionEvent")
    Int32 source;
    event->GetSource(&source);
    if ((source & IInputDevice::SOURCE_CLASS_POINTER) != 0) {
        Int32 action;
        event->GetAction(&action);

        switch (action) {
            case IMotionEvent::ACTION_SCROLL: {
                if (!mIsBeingDragged) {
                    Float vscroll;
                    event->GetAxisValue(IMotionEvent::AXIS_VSCROLL, &vscroll);
                    if (vscroll != 0) {
                        Int32 delta = (Int32) (vscroll * GetVerticalScrollFactor());
                        Int32 range = GetScrollRange();
                        Int32 oldScrollY = mScrollY;
                        Int32 newScrollY = oldScrollY - delta;
                        if (newScrollY < 0) {
                            newScrollY = 0;
                        } else if (newScrollY > range) {
                            newScrollY = range;
                        }
                        if (newScrollY != oldScrollY) {
                            FrameLayout::ScrollTo(mScrollX, newScrollY);
                            return TRUE;
                        }
                    }
                }
            }
        }
    }
    return FrameLayout::OnGenericMotionEvent(event);
}

void ScrollView::OnSecondaryPointerUp(
    /* [in] */ IMotionEvent* ev)
{
    //PEL("ScrollView::OnSecondaryPointerUp")
    Int32 action;
    ev->GetAction(&action);
    Int32 pointerIndex = (action & IMotionEvent::ACTION_POINTER_INDEX_MASK) >>
            IMotionEvent::ACTION_POINTER_INDEX_SHIFT;
    Int32 pointerId;
    ev->GetPointerId(pointerIndex, &pointerId);
    if (pointerId == mActivePointerId) {
        // This was our active pointer going up. Choose a new
        // active pointer and adjust accordingly.
        // TODO: Make this decision more intelligent.
        Int32 newPointerIndex = pointerIndex == 0 ? 1 : 0;
        Float lastY;
        ev->GetYEx(newPointerIndex, &lastY);
        mLastMotionY = lastY;
        ev->GetPointerId(newPointerIndex, &mActivePointerId);
        if (mVelocityTracker != NULL) {
            mVelocityTracker->Clear();
        }
    }
}

void ScrollView::OnOverScrolled(
    /* [in] */ Int32 scrollX,
    /* [in] */ Int32 scrollY,
    /* [in] */ Boolean clampedX,
    /* [in] */ Boolean clampedY)
{
    //PEL("ScrollView::OnOverScrolled")
    // Treat animating scrolls differently; see #computeScroll() for why.
    if (!mScroller->IsFinished()) {
        mScrollX = scrollX;
        mScrollY = scrollY;
        InvalidateParentIfNeeded();
        if (clampedY) {
            mScroller->SpringBack(mScrollX, mScrollY, 0, 0, 0, GetScrollRange());
        }
    }
    else {
        FrameLayout::ScrollTo(scrollX, scrollY);
    }

    AwakenScrollBars();
}

Boolean ScrollView::PerformAccessibilityAction(
    /* [in] */ Int32 action,
    /* [in] */ IBundle* arguments)
{
    if (FrameLayout::PerformAccessibilityAction(action, arguments)) {
        return TRUE;
    }
    if (!IsEnabled()) {
        return FALSE;
    }
    switch (action) {
        case IAccessibilityNodeInfo::ACTION_SCROLL_FORWARD: {
            Int32 viewportHeight = GetHeight() - mPaddingBottom - mPaddingTop;
            Int32 targetScrollY = Elastos::Core::Math::Min(mScrollY + viewportHeight, GetScrollRange());
            if (targetScrollY != mScrollY) {
                SmoothScrollTo(0, targetScrollY);
                return TRUE;
            }
        }
        return FALSE;
        case IAccessibilityNodeInfo::ACTION_SCROLL_BACKWARD: {
            Int32 viewportHeight = GetHeight() - mPaddingBottom - mPaddingTop;
            Int32 targetScrollY = Elastos::Core::Math::Max(mScrollY - viewportHeight, 0);
            if (targetScrollY != mScrollY) {
                SmoothScrollTo(0, targetScrollY);
                return TRUE;
            }
        }
        return FALSE;
    }
    return FALSE;
}

ECode ScrollView::OnInitializeAccessibilityNodeInfo(
    /* [in] */ IAccessibilityNodeInfo* info)
{
    FrameLayout::OnInitializeAccessibilityNodeInfo(info);
    AutoPtr<ICharSequence> csq;
    CStringWrapper::New(String("CScrollView"), (ICharSequence**)&csq);
    info->SetClassName(csq);
    if (IsEnabled()) {
        Int32 scrollRange = GetScrollRange();
        if (scrollRange > 0) {
            info->SetScrollable(TRUE);
            if (mScrollY > 0) {
                info->AddAction(IAccessibilityNodeInfo::ACTION_SCROLL_BACKWARD);
            }
            if (mScrollY < scrollRange) {
                info->AddAction(IAccessibilityNodeInfo::ACTION_SCROLL_FORWARD);
            }
        }
    }
    return NOERROR;
}

ECode ScrollView::OnInitializeAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    FrameLayout::OnInitializeAccessibilityEvent(event);
    AutoPtr<ICharSequence> csq;
    CStringWrapper::New(String("CScrollView"), (ICharSequence**)&csq);
    event->SetClassName(csq);

    Boolean scrollable = GetScrollRange() > 0;
    event->SetScrollable(scrollable);
    event->SetScrollX(mScrollX);
    event->SetScrollY(mScrollY);
    event->SetMaxScrollX(mScrollX);
    event->SetMaxScrollY(GetScrollRange());
    return NOERROR;
}

Int32 ScrollView::GetScrollRange()
{
    //PEL("ScrollView::GetScrollRange")
    Int32 scrollRange = 0;
    if (GetChildCount() > 0) {
        AutoPtr<IView> child = GetChildAt(0);
        Int32 childHeight;
        child->GetHeight(&childHeight);
        scrollRange = Elastos::Core::Math::Max(0,
                 childHeight- (GetHeight() - mPaddingBottom - mPaddingTop));
    }
    return scrollRange;
}

/**
 * <p>
 * Finds the next focusable component that fits in the specified bounds.
 * </p>
 *
 * @param topFocus look for a candidate is the one at the top of the bounds
 *                 if topFocus is TRUE, or at the bottom of the bounds if topFocus is
 *                 FALSE
 * @param top      the top offset of the bounds in which a focusable must be
 *                 found
 * @param bottom   the bottom offset of the bounds in which a focusable must
 *                 be found
 * @return the next focusable component in the bounds or NULL if none can
 *         be found
 */
AutoPtr<IView> ScrollView::FindFocusableViewInBounds(
    /* [in] */ Boolean topFocus,
    /* [in] */ Int32 top,
    /* [in] */ Int32 bottom)
{
    //PEL("ScrollView::FindFocusableViewInBounds")
    AutoPtr<IObjectContainer> focusables;
    ASSERT_SUCCEEDED(GetFocusables(IView::FOCUS_FORWARD, (IObjectContainer**)&focusables));

    AutoPtr<IView> focusCandidate;

    /*
     * A fully contained focusable is one where its top is below the bound's
     * top, and its bottom is above the bound's bottom. A partially
     * contained focusable is one where some part of it is within the
     * bounds, but it also has some part that is not within bounds.  A fully contained
     * focusable is preferred to a partially contained focusable.
     */
    Boolean foundFullyContainedFocusable = FALSE;

    AutoPtr<IObjectEnumerator> objEmu;
    ASSERT_SUCCEEDED(focusables->GetObjectEnumerator((IObjectEnumerator**)&objEmu));

    Boolean isSucceeded;
    objEmu->MoveNext(&isSucceeded);
    while (isSucceeded) {
        AutoPtr<IView> view;
        objEmu->Current((IInterface**)&view);

        Int32 viewTop, viewBottom;
        view->GetTop(&viewTop);
        view->GetBottom(&viewBottom);

        if (top < viewBottom && viewTop < bottom) {
            /*
             * the focusable is in the target area, it is a candidate for
             * focusing
             */

            Boolean viewIsFullyContained = (top < viewTop) &&
                    (viewBottom < bottom);

            if (focusCandidate == NULL) {
                /* No candidate, take this one */
                focusCandidate = view;
                foundFullyContainedFocusable = viewIsFullyContained;
            }
            else {
                Int32 t, b;
                focusCandidate->GetTop(&t);
                focusCandidate->GetBottom(&b);
                Boolean viewIsCloserToBoundary =
                        (topFocus && viewTop < t) || (!topFocus && viewBottom > b);

                if (foundFullyContainedFocusable) {
                    if (viewIsFullyContained && viewIsCloserToBoundary) {
                        /*
                         * We're dealing with only fully contained views, so
                         * it has to be closer to the boundary to beat our
                         * candidate
                         */
                        focusCandidate = view;
                    }
                }
                else {
                    if (viewIsFullyContained) {
                        /* Any fully contained view beats a partially contained view */
                        focusCandidate = view;
                        foundFullyContainedFocusable = TRUE;
                    }
                    else if (viewIsCloserToBoundary) {
                        /*
                         * Partially contained view beats another partially
                         * contained view if it's closer
                         */
                        focusCandidate = view;
                    }
                }
            }
        }
        objEmu->MoveNext(&isSucceeded);
    }

    return focusCandidate;
}

Boolean ScrollView::PageScroll(
    /* [in] */ Int32 direction)
{
    //PEL("ScrollView::PageScroll")
    Boolean down = direction == IView::FOCUS_DOWN;
    Int32 height = GetHeight();

    if (down) {
        mTempRect->mTop = GetScrollY() + height;
        Int32 count = GetChildCount();
        if (count > 0) {
            AutoPtr<IView> view = GetChildAt(count - 1);
            Int32 bottom;
            view->GetBottom(&bottom);
            if (mTempRect->mTop + height > bottom) {
                mTempRect->mTop = bottom - height;
            }
        }
    }
    else {
        mTempRect->mTop = GetScrollY() - height;
        if (mTempRect->mTop < 0) {
            mTempRect->mTop = 0;
        }
    }
    mTempRect->mBottom = mTempRect->mTop + height;

    return ScrollAndFocus(direction,
            mTempRect->mTop, mTempRect->mBottom);
}

Boolean ScrollView::FullScroll(
    /* [in] */ Int32 direction)
{
    //PEL("ScrollView::FullScroll")
    Boolean down = direction == IView::FOCUS_DOWN;
    Int32 height = GetHeight();

    mTempRect->mTop = 0;
    mTempRect->mBottom = height;

    if (down) {
        Int32 count = GetChildCount();
        if (count > 0) {
            AutoPtr<IView> view = GetChildAt(count - 1);
            view->GetBottom(&mTempRect->mBottom);

            mTempRect->mBottom += mPaddingBottom;
            mTempRect->mTop = mTempRect->mBottom - height;
        }
    }

    return ScrollAndFocus(direction,
            mTempRect->mTop, mTempRect->mBottom);
}

Boolean ScrollView::ScrollAndFocus(
    /* [in] */ Int32 direction,
    /* [in] */ Int32 top,
    /* [in] */ Int32 bottom)
{
    Boolean handled = TRUE;

    Int32 height = GetHeight();
    Int32 containerTop = GetScrollY();
    Int32 containerBottom = containerTop + height;
    Boolean up = direction == IView::FOCUS_UP;

    AutoPtr<IView> newFocused = FindFocusableViewInBounds(up, top, bottom);
    if (newFocused == NULL) {
        newFocused = THIS_PROBE(IView);
    }

    if (top >= containerTop && bottom <= containerBottom) {
        handled = FALSE;
    }
    else {
        Int32 delta = up ? (top - containerTop) : (bottom - containerBottom);
        DoScrollY(delta);
    }

    AutoPtr<IView> currentFocused = FindFocus();
    if (newFocused != currentFocused)
        newFocused->RequestFocusEx(direction, &up);

    return handled;
}

Boolean ScrollView::ArrowScroll(
    /* [in] */ Int32 direction)
{
    //PEL("ScrollView::ArrowScroll")
    AutoPtr<IView> currentFocused = FindFocus();
    if (currentFocused.Get() == THIS_PROBE(IView)) {
        currentFocused = NULL;
    }

    AutoPtr<FocusFinder> finder = FocusFinder::GetInstance();
    AutoPtr<IView> nextFocused;
    finder->FindNextFocus(THIS_PROBE(IViewGroup),
            currentFocused, direction, (IView**)&nextFocused);

    Int32 maxJump = GetMaxScrollAmount();

    if (nextFocused != NULL && IsWithinDeltaOfScreen(nextFocused, maxJump, GetHeight())) {
        nextFocused->GetDrawingRect(mTempRect);
        OffsetDescendantRectToMyCoords(nextFocused, mTempRect);
        Int32 scrollDelta = ComputeScrollDeltaToGetChildRectOnScreen(mTempRect);
        DoScrollY(scrollDelta);
        Boolean result;
        nextFocused->RequestFocusEx(direction, &result);
    }
    else {
        // no new focus
        Int32 scrollDelta = maxJump;

        if (direction == IView::FOCUS_UP && GetScrollY() < scrollDelta) {
            scrollDelta = GetScrollY();
        }
        else if (direction == IView::FOCUS_DOWN) {
            if (GetChildCount() > 0) {
                Int32 daBottom;
                GetChildAt(0)->GetBottom(&daBottom);

                Int32 screenBottom = GetScrollY() + GetHeight();

                if (daBottom - screenBottom < maxJump) {
                    scrollDelta = daBottom - screenBottom;
                }
            }
        }
        if (scrollDelta == 0) {
            return FALSE;
        }
        DoScrollY(direction == IView::FOCUS_DOWN ? scrollDelta : -scrollDelta);
    }

    Boolean isFocused;
    if (currentFocused != NULL && (currentFocused->IsFocused(&isFocused), isFocused)
            && IsOffScreen(currentFocused)) {
        // previously focused item still has focus and is off screen, give
        // it up (take it back to ourselves)
        // (also, need to temporarily force FOCUS_BEFORE_DESCENDANTS so we are
        // sure to
        // get it)
        Int32 descendantFocusability = GetDescendantFocusability();  // save
        SetDescendantFocusability(IViewGroup::FOCUS_BEFORE_DESCENDANTS);
        RequestFocus();
        SetDescendantFocusability(descendantFocusability);  // restore
    }
    return TRUE;
}

/**
 * @return whether the descendant of this scroll view is scrolled off
 *  screen.
 */
Boolean ScrollView::IsOffScreen(
    /* [in] */ IView* descendant)
{
    return !IsWithinDeltaOfScreen(descendant, 0, GetHeight());
}

/**
 * @return whether the descendant of this scroll view is within delta
 *  pixels of being on the screen.
 */
Boolean ScrollView::IsWithinDeltaOfScreen(
    /* [in] */ IView* descendant,
    /* [in] */ Int32 delta,
    /* [in] */ Int32 height)
{
    descendant->GetDrawingRect(mTempRect);
    OffsetDescendantRectToMyCoords(descendant, mTempRect);

    return (mTempRect->mBottom + delta) >= GetScrollY()
            && (mTempRect->mTop - delta) <= (GetScrollY() + height);
}

/**
 * Smooth scroll by a Y delta
 *
 * @param delta the number of pixels to scroll by on the Y axis
 */
void ScrollView::DoScrollY(
    /* [in] */ Int32 delta)
{
    if (delta != 0) {
        if (mSmoothScrollingEnabled) {
            SmoothScrollBy(0, delta);
        }
        else {
            ScrollBy(0, delta);
        }
    }
}

ECode ScrollView::SmoothScrollBy(
    /* [in] */ Int32 dx,
    /* [in] */ Int32 dy)
{
    if (GetChildCount() == 0) {
        // Nothing to do.
        return NOERROR;
    }

    Int64 duration = AnimationUtils::CurrentAnimationTimeMillis() - mLastScroll;
    if (duration > ANIMATED_SCROLL_GAP) {
       Int32 height = GetHeight() - mPaddingBottom - mPaddingTop;
       Int32 bottom;
       GetChildAt(0)->GetHeight(&bottom);
       Int32 maxY = Elastos::Core::Math::Max(0, bottom - height);
       Int32 scrollY = mScrollY;
       dy = Elastos::Core::Math::Max(0, Elastos::Core::Math::Min(scrollY + dy, maxY)) - scrollY;

       mScroller->StartScroll(mScrollX, scrollY, 0, dy);
       PostInvalidateOnAnimation();
    }
    else {
        if (!mScroller->IsFinished()) {
            mScroller->AbortAnimation();

            if (mFlingStrictSpan != NULL) {
                mFlingStrictSpan->Finish();
                mFlingStrictSpan = NULL;
            }
        }
        ScrollBy(dx, dy);
    }
    mLastScroll = AnimationUtils::CurrentAnimationTimeMillis();

    return NOERROR;
}

ECode ScrollView::SmoothScrollTo(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    return SmoothScrollBy(x - mScrollX, y - mScrollY);
}

Int32 ScrollView::ComputeVerticalScrollRange()
{
    Int32 count = GetChildCount();
    Int32 contentHeight = GetHeight() - mPaddingBottom - mPaddingTop;
    if (count == 0) {
        return contentHeight;
    }

    Int32 scrollRange;
    GetChildAt(0)->GetBottom(&scrollRange);
    Int32 scrollY = mScrollY;
    Int32 overscrollBottom = Elastos::Core::Math::Max(0, scrollRange - contentHeight);
    if (scrollY < 0) {
        scrollRange -= scrollY;
    }
    else if (scrollY > overscrollBottom) {
        scrollRange += scrollY - overscrollBottom;
    }

    return scrollRange;
}

Int32 ScrollView::ComputeVerticalScrollOffset()
{
    return Elastos::Core::Math::Max(0, FrameLayout::ComputeVerticalScrollOffset());
}

void ScrollView::MeasureChild(
    /* [in] */ IView* child,
    /* [in] */ Int32 parentWidthMeasureSpec,
    /* [in] */ Int32 parentHeightMeasureSpec)
{
    AutoPtr<IViewGroupLayoutParams> lp;
    child->GetLayoutParams((IViewGroupLayoutParams**)&lp);

    Int32 childWidthMeasureSpec;
    Int32 childHeightMeasureSpec;

    Int32 width;
    lp->GetWidth(&width);
    childWidthMeasureSpec = GetChildMeasureSpec(parentWidthMeasureSpec,
            mPaddingLeft + mPaddingRight, width);

    childHeightMeasureSpec = MeasureSpec::MakeMeasureSpec(0, MeasureSpec::UNSPECIFIED);

    child->Measure(childWidthMeasureSpec, childHeightMeasureSpec);
}

void ScrollView::MeasureChildWithMargins(
    /* [in] */ IView* child,
    /* [in] */ Int32 parentWidthMeasureSpec,
    /* [in] */ Int32 widthUsed,
    /* [in] */ Int32 parentHeightMeasureSpec,
    /* [in] */ Int32 heightUsed)
{
    AutoPtr<IViewGroupMarginLayoutParams> params;
    child->GetLayoutParams((IViewGroupLayoutParams**)&params);
    CViewGroupMarginLayoutParams* lp = (CViewGroupMarginLayoutParams*)params.Get();

    Int32 childWidthMeasureSpec = GetChildMeasureSpec(parentWidthMeasureSpec,
            mPaddingLeft + mPaddingRight + lp->mLeftMargin + lp->mRightMargin
            + widthUsed, lp->mWidth);
    Int32 childHeightMeasureSpec = MeasureSpec::MakeMeasureSpec(
            lp->mTopMargin + lp->mBottomMargin, MeasureSpec::UNSPECIFIED);

    child->Measure(childWidthMeasureSpec, childHeightMeasureSpec);
}

ECode ScrollView::ComputeScroll()
{
    //PEL("ScrollView::ComputeScroll")
    if (mScroller->ComputeScrollOffset()) {
        // This is called at drawing time by ViewGroup.  We don't want to
        // re-show the scrollbars at this point, which scrollTo will do,
        // so we replicate most of scrollTo here.
        //
        //         It's a little odd to call onScrollChanged from inside the drawing.
        //
        //         It is, except when you remember that computeScroll() is used to
        //         animate scrolling. So unless we want to defer the onScrollChanged()
        //         until the end of the animated scrolling, we don't really have a
        //         choice here.
        //
        //         I agree.  The alternative, which I think would be worse, is to post
        //         something and tell the subclasses later.  This is bad because there
        //         will be a window where mScrollX/Y is different from what the app
        //         thinks it is.
        //
        Int32 oldX = mScrollX;
        Int32 oldY = mScrollY;
        Int32 x = mScroller->GetCurrX();
        Int32 y = mScroller->GetCurrY();

        if (oldX != x || oldY != y) {

            Int32 range = GetScrollRange();
            Int32 overscrollMode = GetOverScrollMode();
            Boolean canOverscroll = overscrollMode == IView::OVER_SCROLL_ALWAYS ||
                (overscrollMode == IView::OVER_SCROLL_IF_CONTENT_SCROLLS && range > 0);

            OverScrollBy(x - oldX, y - oldY, oldX, oldY, 0, range,
                0, mOverflingDistance, FALSE);
            OnScrollChanged(mScrollX, mScrollY, oldX, oldY);

            if (canOverscroll) {
                if (y < 0 && oldY >= 0) {
                    mEdgeGlowTop->OnAbsorb((Int32) mScroller->GetCurrVelocity());
                } else if (y > range && oldY <= range) {
                    mEdgeGlowBottom->OnAbsorb((Int32) mScroller->GetCurrVelocity());
                }
            }
        }

        if (!AwakenScrollBars()) {
            // Keep on drawing until the animation has finished.
            PostInvalidateOnAnimation();
        }
    } else {
        if (mFlingStrictSpan != NULL) {
            mFlingStrictSpan->Finish();
            mFlingStrictSpan = NULL;
        }
    }


    return NOERROR;
}

void ScrollView::ScrollToChild(
    /* [in] */ IView* child)
{
    //PEL("ScrollView::ScrollToChild")
    child->GetDrawingRect(mTempRect);

    /* Offset from child's local coordinates to ScrollView coordinates */
    OffsetDescendantRectToMyCoords(child, mTempRect);

    Int32 scrollDelta = ComputeScrollDeltaToGetChildRectOnScreen(mTempRect);

    if (scrollDelta != 0) {
        ScrollBy(0, scrollDelta);
    }
}

Boolean ScrollView::ScrollToChildRect(
    /* [in] */ IRect* rect,
    /* [in] */ Boolean immediate)
{
    //PEL("ScrollView::ScrollToChildRect")
    Int32 delta = ComputeScrollDeltaToGetChildRectOnScreen(rect);
    Boolean scroll = delta != 0;
    if (scroll) {
        if (immediate) {
            ScrollBy(0, delta);
        }
        else {
            SmoothScrollBy(0, delta);
        }
    }

    return scroll;
}

/**
 * Compute the amount to scroll in the Y direction in order to get
 * a rectangle completely on the screen (or, if taller than the screen,
 * at least the first screen size chunk of it).
 *
 * @param rect The rect.
 * @return The scroll delta.
 */
Int32 ScrollView::ComputeScrollDeltaToGetChildRectOnScreen(
    /* [in] */ IRect* rect)
{
    //PEL("ScrollView::ComputeScrollDeltaToGetChildRectOnScreen")
    if (GetChildCount() == 0) return 0;

    Int32 height = GetHeight();
    Int32 screenTop = GetScrollY();
    Int32 screenBottom = screenTop + height;

    Int32 fadingEdge = GetVerticalFadingEdgeLength();

    Int32 l, t, r, b, h;
    rect->Get(&l, &t, &r, &b);
    rect->GetHeight(&h);

    // leave room for top fading edge as long as rect isn't at very top
    if (t > 0) {
        screenTop += fadingEdge;
    }

    // leave room for bottom fading edge as long as rect isn't at very bottom
    Int32 childHeight;
    GetChildAt(0)->GetHeight(&childHeight);
    if (b < childHeight) {
        screenBottom -= fadingEdge;
    }

    Int32 scrollYDelta = 0;

    if (b > screenBottom && t > screenTop) {
        // need to move down to get it in view: move down just enough so
        // that the entire rectangle is in view (or at least the first
        // screen size chunk).

        if (h > height) {
            // just enough to get screen size chunk on
            scrollYDelta += (t - screenTop);
        }
        else {
            // get entire rect at bottom of screen
            scrollYDelta += (b - screenBottom);
        }

        // make sure we aren't scrolling beyond the end of our content
        Int32 bottom;
        GetChildAt(0)->GetBottom(&bottom);
        Int32 distanceToBottom = bottom - screenBottom;
        scrollYDelta = Elastos::Core::Math::Min(scrollYDelta, distanceToBottom);

    }
    else if (t < screenTop && b < screenBottom) {
        // need to move up to get it in view: move up just enough so that
        // entire rectangle is in view (or at least the first screen
        // size chunk of it).

        if (h > height) {
            // screen size chunk
            scrollYDelta -= (screenBottom - b);
        }
        else {
            // entire rect at top
            scrollYDelta -= (screenTop - t);
        }

        // make sure we aren't scrolling any further than the top our content
        scrollYDelta = Elastos::Core::Math::Max(scrollYDelta, -GetScrollY());
    }
    return scrollYDelta;
}

ECode ScrollView::RequestChildFocus(
    /* [in] */ IView* child,
    /* [in] */ IView* focused)
{
    if (!mIsLayoutDirty) {
        ScrollToChild(focused);
    }
    else {
        // The child may not be laid out yet, we can't compute the scroll yet
        mChildToScrollTo = focused;
    }
    return FrameLayout::RequestChildFocus(child, focused);
}

/**
 * When looking for focus in children of a scroll view, need to be a little
 * more careful not to give focus to something that is scrolled off screen.
 *
 * This is more expensive than the default {@link android.view.ViewGroup}
 * implementation, otherwise this behavior might have been made the default.
 */
Boolean ScrollView::OnRequestFocusInDescendants(
    /* [in] */ Int32 direction,
    /* [in] */ IRect* previouslyFocusedRect)
{
    // convert from forward / backward notation to up / down / left / right
    // (ugh).
    if (direction == IView::FOCUS_FORWARD) {
        direction = IView::FOCUS_DOWN;
    }
    else if (direction == IView::FOCUS_BACKWARD) {
        direction = IView::FOCUS_UP;
    }

    AutoPtr<FocusFinder> finder = FocusFinder::GetInstance();
    AutoPtr<IView> nextFocus;
    finder->FindNextFocusFromRect(THIS_PROBE(IViewGroup),
            previouslyFocusedRect, direction, (IView**)&nextFocus);

    if (nextFocus == NULL) {
        return FALSE;
    }

    if (IsOffScreen(nextFocus)) {
        return FALSE;
    }

    Boolean result;
    ASSERT_SUCCEEDED(nextFocus->RequestFocusEx2(
            direction, previouslyFocusedRect, &result));

    return result;
}

Boolean ScrollView::RequestChildRectangleOnScreen(
    /* [in] */ IView* child,
    /* [in] */ IRect* rectangle,
    /* [in] */ Boolean immediate)
{
    // offset into coordinate space of this scroll view
    Int32 left, scrollX, top, scrollY;
    child->GetLeft(&left);
    child->GetScrollX(&scrollX);
    child->GetTop(&top);
    child->GetScrollY(&scrollY);
    rectangle->Offset(left - scrollX, top - scrollY);

    return ScrollToChildRect(rectangle, immediate);
}

ECode ScrollView::RequestLayout()
{
    mIsLayoutDirty = TRUE;
    return FrameLayout::RequestLayout();
}

ECode ScrollView::OnDetachedFromWindow()
{
    FrameLayout::OnDetachedFromWindow();

    if (mScrollStrictSpan != NULL) {
        mScrollStrictSpan->Finish();
        mScrollStrictSpan = NULL;
    }
    if (mFlingStrictSpan != NULL) {
        mFlingStrictSpan->Finish();
        mFlingStrictSpan = NULL;
    }
    return NOERROR;
}

void ScrollView::OnLayout(
    /* [in] */ Boolean changed,
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    FrameLayout::OnLayout(changed, left, top, right, bottom);
    mIsLayoutDirty = FALSE;
    // Give a child focus if it needs it
    if (mChildToScrollTo != NULL &&
            IsViewDescendantOf(mChildToScrollTo, THIS_PROBE(IView))) {
        ScrollToChild(mChildToScrollTo);
    }
    mChildToScrollTo = NULL;

    // Calling this with the present values causes it to re-clam them
    ScrollTo(mScrollX, mScrollY);
}

void ScrollView::OnSizeChanged(
    /* [in] */ Int32 w,
    /* [in] */ Int32 h,
    /* [in] */ Int32 oldw,
    /* [in] */ Int32 oldh)
{
    FrameLayout::OnSizeChanged(w, h, oldw, oldh);

    AutoPtr<IView> currentFocused = FindFocus();
    if (NULL == currentFocused || THIS_PROBE(IView) == currentFocused.Get()) {
        return;
    }

    // If the currently-focused view was visible on the screen when the
    // screen was at the old height, then scroll the screen to make that
    // view visible with the new screen height.
    if (IsWithinDeltaOfScreen(currentFocused, 0, oldh)) {
        currentFocused->GetDrawingRect(mTempRect);
        OffsetDescendantRectToMyCoords(currentFocused, mTempRect);
        Int32 scrollDelta = ComputeScrollDeltaToGetChildRectOnScreen(mTempRect);
        DoScrollY(scrollDelta);
    }
}

/**
 * Return TRUE if child is an descendant of parent, (or equal to the parent).
 */
Boolean ScrollView::IsViewDescendantOf(
    /* [in] */ IView* child,
    /* [in] */ IView* parent)
{
    if (child == parent) {
        return TRUE;
    }

    AutoPtr<IViewParent> theParent;
    child->GetParent((IViewParent**)&theParent);
    IViewGroup* vg = IViewGroup::Probe(theParent);
    if (vg != NULL) {
        return IsViewDescendantOf(vg, parent);
    }
    return FALSE;
}

ECode ScrollView::Fling(
    /* [in] */ Int32 velocityY)
{
    //PEL("ScrollView::Fling")
    if (GetChildCount() > 0) {
        Int32 height = GetHeight() - mPaddingBottom - mPaddingTop;
        Int32 bottom;
        GetChildAt(0)->GetHeight(&bottom);

        mScroller->Fling(mScrollX, mScrollY, 0, velocityY, 0, 0, 0,
                Elastos::Core::Math::Max(0, bottom - height), 0, height / 2);


        if (mFlingStrictSpan == NULL) {
            AutoPtr<IStrictMode> mode;
            CStrictMode::AcquireSingleton((IStrictMode**)&mode);
            mode->EnterCriticalSpan(String("ScrollView-fling"), (IStrictModeSpan**)&mFlingStrictSpan);
        }

        PostInvalidateOnAnimation();
    }
    return NOERROR;
}

void ScrollView::EndDrag()
{
    mIsBeingDragged = FALSE;

    RecycleVelocityTracker();

    if (mEdgeGlowTop != NULL) {
        mEdgeGlowTop->OnRelease();
        mEdgeGlowBottom->OnRelease();
    }

     if (mScrollStrictSpan != NULL) {
         mScrollStrictSpan->Finish();
         mScrollStrictSpan = NULL;
     }
}

ECode ScrollView::ScrollTo(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    //PEL("ScrollView::ScrollTo")
    // we rely on the fact the View.scrollBy calls scrollTo.
    if (GetChildCount() > 0) {
        AutoPtr<IView> child = GetChildAt(0);
        Int32 width, height;
        child->GetWidth(&width);
        child->GetHeight(&height);
        x = Clamp(x, GetWidth() - mPaddingRight - mPaddingLeft, width);
        y = Clamp(y, GetHeight() - mPaddingBottom - mPaddingTop, height);
        if (x != mScrollX || y != mScrollY) {
            FAIL_RETURN(FrameLayout::ScrollTo(x, y));
        }
    }
    return NOERROR;
}

ECode ScrollView::SetOverScrollMode(
    /* [in] */ Int32 overScrollMode)
{
    if (overScrollMode != IView::OVER_SCROLL_NEVER) {
        if (mEdgeGlowTop == NULL) {
            AutoPtr<IContext> context = GetContext();
            CEdgeEffect::New(context, (IEdgeEffect**)&mEdgeGlowTop);
            CEdgeEffect::New(context, (IEdgeEffect**)&mEdgeGlowBottom);
        }
    }
    else {
        mEdgeGlowTop = NULL;
        mEdgeGlowBottom = NULL;
    }
    return FrameLayout::SetOverScrollMode(overScrollMode);
}

ECode ScrollView::Draw(
    /* [in] */ ICanvas* canvas)
{
    Boolean result;
    FAIL_RETURN(FrameLayout::Draw(canvas));

    if (mEdgeGlowTop != NULL) {
        Int32 scrollY = mScrollY;
        mEdgeGlowTop->IsFinished(&result);
        if (!result) {
            Int32 restoreCount;
            canvas->Save(&restoreCount);
            Int32 width = GetWidth()- mPaddingLeft - mPaddingRight;

            canvas->Translate(mPaddingLeft, Elastos::Core::Math::Min(0, scrollY));
            mEdgeGlowTop->SetSize(width, GetHeight());
            if (mEdgeGlowTop->Draw(canvas, &result), result) {
                PostInvalidateOnAnimation();
            }
            canvas->RestoreToCount(restoreCount);
        }

        mEdgeGlowBottom->IsFinished(&result);
        if (!result) {
            Int32 restoreCount;
            canvas->Save(&restoreCount);
            Int32 width = GetWidth() - mPaddingLeft - mPaddingRight;
            Int32 height = GetHeight();

            canvas->Translate(-width + mPaddingLeft, Elastos::Core::Math::Max(GetScrollRange(), scrollY) + height);
            canvas->RotateEx(180, width, 0);
            mEdgeGlowBottom->SetSize(width, height);

            if (mEdgeGlowBottom->Draw(canvas, &result), result) {
                PostInvalidateOnAnimation();
            }
            canvas->RestoreToCount(restoreCount);
        }
    }

    return NOERROR;
}

Int32 ScrollView::Clamp(
    /* [in] */ Int32 n,
    /* [in] */ Int32 my,
    /* [in] */ Int32 child)
{
    if (my >= child || n < 0) {
        /* my >= child is this case:
         *                    |--------------- me ---------------|
         *     |------ child ------|
         * or
         *     |--------------- me ---------------|
         *            |------ child ------|
         * or
         *     |--------------- me ---------------|
         *                                  |------ child ------|
         *
         * n < 0 is this case:
         *     |------ me ------|
         *                    |-------- child --------|
         *     |-- mScrollX --|
         */
        return 0;
    }
    if ((my + n) > child) {
        /* this case:
         *                    |------ me ------|
         *     |------ child ------|
         *     |-- mScrollX --|
         */
        return child - my;
    }
    return n;
}

} // namespace Widget
} // namespace Droid
} // namespace Elastos
