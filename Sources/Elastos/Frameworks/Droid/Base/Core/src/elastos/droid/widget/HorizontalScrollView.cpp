
#include "widget/HorizontalScrollView.h"
#include "widget/ScrollView.h"
#include "view/CViewConfigurationHelper.h"
#include "widget/CFrameLayoutLayoutParams.h"
#include "view/FocusFinder.h"
#include "view/CMotionEvent.h"
#include <elastos/core/Math.h>
#include "view/accessibility/CAccessibilityEvent.h"
#include "view/animation/AnimationUtils.h"
#include "view/CViewGroupMarginLayoutParams.h"
#include "widget/CEdgeEffect.h"

using Elastos::Droid::Widget::ScrollView;
using Elastos::Droid::View::IViewConfiguration;
using Elastos::Droid::View::IViewConfigurationHelper;
using Elastos::Droid::View::CViewConfigurationHelper;
using Elastos::Droid::Widget::CFrameLayoutLayoutParams;
using Elastos::Droid::View::EIID_IView;
using Elastos::Droid::View::IViewGroup;
using Elastos::Droid::View::FocusFinder;
using Elastos::Droid::View::EIID_IViewGroup;
using Elastos::Droid::View::IViewParent;
using Elastos::Droid::View::CMotionEvent;
using Elastos::Droid::View::IInputDevice;
using Elastos::Core::ICharSequence;
using Elastos::Core::CStringWrapper;
using Elastos::Droid::View::Accessibility::CAccessibilityEvent;
using Elastos::Droid::View::Animation::AnimationUtils;
using Elastos::Droid::View::CViewGroupMarginLayoutParams;
using Elastos::Droid::Widget::CEdgeEffect;

namespace Elastos {
namespace Droid {
namespace Widget {

const String HorizontalScrollView::HORIZONTALSCROLLVIEW_NAME = String("HorizontalScrollView");
const Int32 HorizontalScrollView::ANIMATED_SCROLL_GAP = ScrollView::ANIMATED_SCROLL_GAP;
const Float HorizontalScrollView::MAX_SCROLL_FACTOR = ScrollView::MAX_SCROLL_FACTOR;
const Int32 HorizontalScrollView::INVALID_POINTER;

HorizontalScrollView::HorizontalScrollView()
    : mLastScroll(0)
    , mScroller(NULL)
    , mEdgeGlowLeft(NULL)
    , mEdgeGlowRight(NULL)
    , mLastMotionX(0)
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

HorizontalScrollView::HorizontalScrollView(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
    : FrameLayout(context, attrs, defStyle)
    , mLastScroll(0)
    , mScroller(NULL)
    , mEdgeGlowLeft(NULL)
    , mEdgeGlowRight(NULL)
    , mLastMotionX(0)
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

HorizontalScrollView::~HorizontalScrollView()
{
}

ECode HorizontalScrollView::InitFromAttributes(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    AutoPtr<ITypedArray> a;

    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::HorizontalScrollView),
            ARRAY_SIZE(R::styleable::HorizontalScrollView));
    FAIL_RETURN(context->ObtainStyledAttributes(attrs,
            attrIds, defStyle, 0, (ITypedArray**)&a));

    Boolean value;
    a->GetBoolean(R::styleable::HorizontalScrollView_fillViewport, FALSE, &value);

    FAIL_RETURN(SetFillViewport(value));

    a->Recycle();

    return NOERROR;
}

ECode HorizontalScrollView::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    ASSERT_SUCCEEDED(FrameLayout::Init(context, attrs, defStyle));
    InitScrollView();
    return InitFromAttributes(context, attrs, defStyle);
}

Float HorizontalScrollView::GetLeftFadingEdgeStrength()
{
    if (GetChildCount() == 0) {
        return 0.0;
    }

    Int32 length = GetHorizontalFadingEdgeLength();

    if (mScrollX < length) {
        return mScrollX / (Float)length;
    }

    return 1.0;
}

Float HorizontalScrollView::GetRightFadingEdgeStrength()
{
    if (GetChildCount() == 0) {
        return 0.0;
    }

    Int32 length = GetHorizontalFadingEdgeLength();
    Int32 rightEdge = GetWidth() - mPaddingRight;
    Int32 right;
    GetChildAt(0)->GetRight(&right);
    Int32 span = right - mScrollX - rightEdge;
    if (span < length) {
        return span / (Float)length;
    }

    return 1.0;
}

Int32 HorizontalScrollView::GetMaxScrollAmount()
{
    return (Int32)(MAX_SCROLL_FACTOR * (mRight - mLeft));
}

void HorizontalScrollView::InitScrollView()
{
    mScroller = new OverScroller(GetContext());
    SetFocusable(TRUE);
    SetDescendantFocusability(ViewGroup::FOCUS_AFTER_DESCENDANTS);
    SetWillNotDraw(FALSE);

    AutoPtr<IViewConfigurationHelper> helper;
    CViewConfigurationHelper::AcquireSingleton((IViewConfigurationHelper**)&helper);
    AutoPtr<IViewConfiguration> configuration ;
    helper->Get(mContext, (IViewConfiguration**)&configuration);
    configuration->GetScaledTouchSlop(&mTouchSlop);
    configuration->GetScaledMinimumFlingVelocity(&mMinimumVelocity);
    configuration->GetScaledMaximumFlingVelocity(&mMaximumVelocity);
    configuration->GetScaledOverscrollDistance(&mOverscrollDistance);
    configuration->GetScaledOverflingDistance(&mOverflingDistance);
}

ECode HorizontalScrollView::AddView(
    /* [in] */ IView* child)
{
    if (GetChildCount() > 0) {
        //throw new IllegalStateException("HorizontalScrollView can host only one direct child");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    return FrameLayout::AddView(child);
}

ECode HorizontalScrollView::AddView(
    /* [in] */ IView* child,
    /* [in] */ Int32 index)
{
    if (GetChildCount() > 0) {
        //throw new IllegalStateException("HorizontalScrollView can host only one direct child");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    return FrameLayout::AddView(child, index);
}

ECode HorizontalScrollView::AddView(
    /* [in] */ IView* child,
    /* [in] */ IViewGroupLayoutParams* params)
{
    if (GetChildCount() > 0) {
        //throw new IllegalStateException("HorizontalScrollView can host only one direct child");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    return FrameLayout::AddView(child, params);
}

ECode HorizontalScrollView::AddView(
    /* [in] */ IView* child,
    /* [in] */ Int32 index,
    /* [in] */ IViewGroupLayoutParams* params)
{
    if (GetChildCount() > 0) {
        //throw new IllegalStateException("HorizontalScrollView can host only one direct child");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    return FrameLayout::AddView(child, index, params);
}

Boolean HorizontalScrollView::CanScroll()
{
    AutoPtr<IView> child = GetChildAt(0);
    if (child != NULL) {
        Int32 childWidth = 0;
        child->GetWidth(&childWidth);
        return GetWidth() < childWidth + mPaddingLeft + mPaddingRight;
    }
    return FALSE;
}

Boolean HorizontalScrollView::IsFillViewport()
{
    return mFillViewport;
}

ECode HorizontalScrollView::SetFillViewport(
    /* [in] */ Boolean fillViewport)
{
    if (fillViewport != mFillViewport) {
        mFillViewport = fillViewport;
        RequestLayout();
    }

    return NOERROR;
}

Boolean HorizontalScrollView::IsSmoothScrollingEnabled()
{
    return mSmoothScrollingEnabled;
}

ECode HorizontalScrollView::SetSmoothScrollingEnabled(
    /* [in] */ Boolean smoothScrollingEnabled)
{
    mSmoothScrollingEnabled = smoothScrollingEnabled;
    return NOERROR;
}

void HorizontalScrollView::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    FrameLayout::OnMeasure(widthMeasureSpec, heightMeasureSpec);

    if (!mFillViewport) {
        return;
    }

    Int32 widthMode = MeasureSpec::GetMode(widthMeasureSpec);
    if (widthMode == MeasureSpec::UNSPECIFIED) {
        return;
    }

    if (GetChildCount() > 0) {
        AutoPtr<IView> child = GetChildAt(0);
        Int32 width = GetMeasuredWidth();
        Int32 childWidth = 0;
        child->GetMeasuredHeight(&childWidth);
        if (childWidth < width) {

            ///////////////////////////////////////////////////////////
            AutoPtr<IFrameLayoutLayoutParams> params;
            child->GetLayoutParams((IViewGroupLayoutParams**)&params);

            Int32 childHeightMeasureSpec = GetChildMeasureSpec(heightMeasureSpec,
                    mPaddingTop + mPaddingBottom,
                    ((CFrameLayoutLayoutParams*)params.Get())->mHeight);
            width -= mPaddingLeft;
            width -= mPaddingRight;
            Int32 childWidthMeasureSpec =
                    MeasureSpec::MakeMeasureSpec(width, MeasureSpec::EXACTLY);

            child->Measure(childWidthMeasureSpec, childHeightMeasureSpec);
        }
    }
}

Boolean HorizontalScrollView::DispatchKeyEvent(
    /* [in] */ IKeyEvent* event)
{
    return FrameLayout::DispatchKeyEvent(event) || ExecuteKeyEvent(event);
}

Boolean HorizontalScrollView::ExecuteKeyEvent(
    /* [in] */ IKeyEvent* event)
{
    mTempRect->SetEmpty();

    if (!CanScroll()) {
        if (IsFocused()) {
            AutoPtr<IView> currentFocused = FindFocus();
            if (currentFocused.Get() == (IView*)this->Probe(EIID_IView)) {
                currentFocused = NULL;
            }
            AutoPtr<IView> nextFocused;
            FocusFinder::GetInstance()->FindNextFocus((IViewGroup*)this->Probe(EIID_IViewGroup),
                currentFocused, IView::FOCUS_RIGHT, (IView**)&nextFocused);

            if (nextFocused != NULL && nextFocused.Get() != (IView*)this->Probe(EIID_IView)) {
                Boolean isFocus = FALSE;
                nextFocused->RequestFocus(IView::FOCUS_RIGHT, &isFocus);
                return isFocus;
            } else {
                return FALSE;
            }
        }
        return FALSE;
    }
    Boolean handled = FALSE;
    Int32 action = 0;
    event->GetAction(&action);
    if (action == IKeyEvent::ACTION_DOWN) {
        Int32 keyCode = 0;
        event->GetKeyCode(&keyCode);
        Boolean isPressed = FALSE;
        switch (keyCode) {
            case IKeyEvent::KEYCODE_DPAD_LEFT:
                event->IsAltPressed(&isPressed);
                if (!isPressed) {
                    handled = ArrowScroll(IView::FOCUS_LEFT);
                }
                else {
                    handled = FullScroll(IView::FOCUS_LEFT);
                }
                break;
            case IKeyEvent::KEYCODE_DPAD_RIGHT:
                event->IsAltPressed(&isPressed);
                if (!isPressed) {
                    handled = ArrowScroll(IView::FOCUS_RIGHT);
                }
                else {
                    handled = FullScroll(IView::FOCUS_RIGHT);
                }
                break;
            case IKeyEvent::KEYCODE_SPACE:
                event->IsShiftPressed(&isPressed);
                PageScroll(isPressed ? IView::FOCUS_LEFT : IView::FOCUS_RIGHT);
                break;
        }
    }

    return handled;
}

Boolean HorizontalScrollView::InChild(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    if (GetChildCount() > 0) {
        Int32 scrollX = mScrollX;
        AutoPtr<IView> child = GetChildAt(0);
        Int32 top, bottom, left, right;
        child->GetTop(&top);
        child->GetBottom(&bottom);
        child->GetLeft(&left);
        child->GetRight(&right);
        return !(y < top
                || y >= bottom
                || x < left - scrollX
                || x >= right - scrollX);
    }

    return FALSE;
}

void HorizontalScrollView::InitOrResetVelocityTracker()
{
    if (mVelocityTracker == NULL) {
        mVelocityTracker = VelocityTracker::Obtain();
    } else {
        mVelocityTracker->Clear();
    }
}

void HorizontalScrollView::InitVelocityTrackerIfNotExists()
{
    if (mVelocityTracker == NULL) {
        mVelocityTracker = VelocityTracker::Obtain();
    }
}

void HorizontalScrollView::RecycleVelocityTracker()
{
    if (mVelocityTracker != NULL) {
        mVelocityTracker->Recycle();
        mVelocityTracker = NULL;
    }
}

ECode HorizontalScrollView::RequestDisallowInterceptTouchEvent(
    /* [in] */ Boolean disallowIntercept)
{
    if (disallowIntercept) {
        RecycleVelocityTracker();
    }
    FrameLayout::RequestDisallowInterceptTouchEvent(disallowIntercept);
    return NOERROR;
}

Boolean HorizontalScrollView::OnInterceptTouchEvent(
    /* [in] */ IMotionEvent* ev)
{
    Int32 action = 0;
    ev->GetAction(&action);
    if ((action == IMotionEvent::ACTION_MOVE) && (mIsBeingDragged)) {
        return TRUE;
    }

    switch (action & IMotionEvent::ACTION_MASK) {
        case IMotionEvent::ACTION_MOVE:
         {
            Int32 activePointerId = mActivePointerId;
            if (activePointerId == INVALID_POINTER) {
                // If we don't have a valid id, the touch down wasn't on content.
                break;
            }

            Int32 pointerIndex = 0;
            ev->FindPointerIndex(activePointerId, &pointerIndex);
            if (pointerIndex == -1) {
                /*Log.e(TAG, "Invalid pointerId=" + activePointerId
                    + " in onInterceptTouchEvent");*/
                break;
            }

            Float x = 0;
            ev->GetX(pointerIndex, &x);
            Int32 xDiff = (Int32)Elastos::Core::Math::Abs((Int32)x - mLastMotionX);
            if (xDiff > mTouchSlop) {
                mIsBeingDragged = TRUE;
                mLastMotionX = x;

                InitVelocityTrackerIfNotExists();
                mVelocityTracker->AddMovement(ev);

                if (mParent != NULL) {
                    mParent->RequestDisallowInterceptTouchEvent(TRUE);
                }
            }
            break;
        }

        case IMotionEvent::ACTION_DOWN:
        {
            Float y = 0;
            Float x = 0;
            ev->GetY(&y);
            ev->GetX(&x);
            if (!InChild((Int32)x, (Int32)y)) {
                mIsBeingDragged = FALSE;
                RecycleVelocityTracker();
                break;
            }

            mLastMotionX = x;
            ev->GetPointerId(0, &mActivePointerId);

            InitOrResetVelocityTracker();
            mVelocityTracker->AddMovement(ev);

            mIsBeingDragged = !mScroller->IsFinished();
            break;
        }

        case IMotionEvent::ACTION_CANCEL:
        case IMotionEvent::ACTION_UP:
            mIsBeingDragged = FALSE;
            mActivePointerId = INVALID_POINTER;
            if (mScroller->SpringBack(mScrollX, mScrollY, 0, GetScrollRange(), 0, 0)) {
                PostInvalidateOnAnimation();
            }
            break;

        case IMotionEvent::ACTION_POINTER_DOWN:
        {
            Int32 index = 0;
            ev->GetActionIndex(&index);
            Float f = 0;
            ev->GetX(index, &f);
            mLastMotionX = (Int32)f;
            ev->GetPointerId(index, &mActivePointerId);
            break;
        }

        case IMotionEvent::ACTION_POINTER_UP:
            OnSecondaryPointerUp(ev);
            Int32 find = 0;
            ev->FindPointerIndex(mActivePointerId, &find);
            Float fx = 0;
            ev->GetX(find, &fx);
            mLastMotionX = (Int32)fx;
            break;
    }

    return mIsBeingDragged;
}

Boolean HorizontalScrollView::OnTouchEvent(
    /* [in] */ IMotionEvent* ev)
{
    InitVelocityTrackerIfNotExists();
    mVelocityTracker->AddMovement(ev);

    Int32 action = 0;
    ev->GetAction(&action);

    switch(action & IMotionEvent::ACTION_MASK) {
        case IMotionEvent::ACTION_DOWN:
        {
            if(GetChildCount() == 0) {
                return FALSE;
            }
            if(mIsBeingDragged = !mScroller->IsFinished()) {
                AutoPtr<IViewParent> parent = GetParent();
                if(parent != NULL) {
                    parent->RequestDisallowInterceptTouchEvent(TRUE);
                }
            }

            if(!mScroller->IsFinished()) {
                mScroller->AbortAnimation();
            }
            Float motionX = 0;
            ev->GetX(&motionX);
            mLastMotionX = (Int32)motionX;
            ev->GetPointerId(0, &mActivePointerId);
            break;
        }

        case IMotionEvent::ACTION_MOVE:
        {
            Int32 activePointerIndex = 0;
            ev->FindPointerIndex(mActivePointerId, &activePointerIndex);
            if(activePointerIndex == -1) {
                //log.e(TAG, "invalid pointerID = ")
                break;
            }
            Int32 x = 0;
            Float fx = 0;
            ev->GetX(activePointerIndex, &fx);
            x = (Int32)fx;
            Int32 deltaX = mLastMotionX - x;
            if(!mIsBeingDragged && Elastos::Core::Math::Abs(deltaX) > mTouchSlop) {
                AutoPtr<IViewParent> parent = GetParent();
                if(parent != NULL) {
                    parent->RequestDisallowInterceptTouchEvent(TRUE);
                }
                mIsBeingDragged = TRUE;
                if(deltaX > 0) {
                    deltaX -= mTouchSlop;
                } else {
                    deltaX += mTouchSlop;
                }

            }
            if(mIsBeingDragged) {
                mLastMotionX = x;

                Int32 oldX = mScrollX;
                Int32 oldY = mScrollY;
                Int32 range = GetScrollRange();
                Int32 overscrollMode = GetOverScrollMode();
                Boolean canOverscroll = overscrollMode == IView::OVER_SCROLL_ALWAYS
                    || (overscrollMode == IView::OVER_SCROLL_IF_CONTENT_SCROLLS && range > 0) ;

                if(OverScrollBy(deltaX, 0, mScrollX, 0, range, 0,
                    mOverscrollDistance, 0, TRUE)) {
                    mVelocityTracker->Clear();
                }
                OnScrollChanged(mScrollX, mScrollY, oldX, oldY);

                if(canOverscroll) {
                    Int32 pulledToX = oldX + deltaX;
                    if(pulledToX < 0) {
                        mEdgeGlowLeft->OnPull((Float)deltaX/GetWidth());
                        Boolean isFinished = FALSE;
                        mEdgeGlowRight->IsFinished(&isFinished);
                        if(isFinished) {
                            mEdgeGlowRight->OnRelease();
                        }
                    } else if(pulledToX > range) {
                        mEdgeGlowRight->OnPull((Float)deltaX/GetWidth());
                        Boolean isFinished = FALSE;
                        mEdgeGlowLeft->IsFinished(&isFinished);
                        if(isFinished) {
                            mEdgeGlowLeft->OnRelease();
                        }
                    }
                    Boolean leftFin = FALSE;
                    Boolean rightFin = FALSE;
                    mEdgeGlowLeft->IsFinished(&leftFin);
                    mEdgeGlowRight->IsFinished(&rightFin);
                    if(mEdgeGlowLeft != NULL && (!leftFin || !rightFin)) {
                        PostInvalidateOnAnimation();
                    }
                }
            }
        }
            break;

        case IMotionEvent::ACTION_UP:
            if(mIsBeingDragged) {
                AutoPtr<VelocityTracker> velocityTracker = mVelocityTracker;
                velocityTracker->ComputeCurrentVelocity(1000, mMaximumVelocity);
                Float x;
                velocityTracker->GetXVelocity(mActivePointerId, &x);
                Int32 initialVelocity = (Int32)x;

                if(GetChildCount() > 0) {
                    if((Elastos::Core::Math::Abs(initialVelocity) > mMinimumVelocity)) {
                        Fling(-initialVelocity);
                    } else {
                        if(mScroller->SpringBack(mScrollX, mScrollY, 0,
                            GetScrollRange(), 0, 0)) {
                            PostInvalidateOnAnimation();
                        }
                    }
                }

                mActivePointerId = INVALID_POINTER;
                mIsBeingDragged = FALSE;
                RecycleVelocityTracker();

                if(mEdgeGlowLeft != NULL) {
                    mEdgeGlowLeft->OnRelease();
                    mEdgeGlowRight->OnRelease();
                }
            }
            break;

        case IMotionEvent::ACTION_CANCEL:
            if(mIsBeingDragged && GetChildCount() > 0) {
                if(mScroller->SpringBack(mScrollX, mScrollY, 0,
                    GetScrollRange(), 0, 0)) {
                    PostInvalidateOnAnimation();
                }
                mActivePointerId = INVALID_POINTER;
                mIsBeingDragged = FALSE;
                RecycleVelocityTracker();

                if(mEdgeGlowLeft != NULL) {
                    mEdgeGlowLeft->OnRelease();
                    mEdgeGlowRight->OnRelease();
                }
            }
            break;

        case IMotionEvent::ACTION_POINTER_UP:
            OnSecondaryPointerUp(ev);
            break;
    }
    return TRUE;
}

void HorizontalScrollView::OnSecondaryPointerUp(
    /* [in] */ IMotionEvent* ev)
{
    Int32 action = 0;
    ev->GetAction(&action);
    Int32 pointerIndex = (action & IMotionEvent::ACTION_POINTER_INDEX_MASK) >>
            IMotionEvent::ACTION_POINTER_INDEX_SHIFT;
    Int32 pointerId = 0;
    ev->GetPointerId(pointerIndex, &pointerId);
    if (pointerId == mActivePointerId) {
        Int32 newPointerIndex = pointerIndex == 0 ? 1 : 0;
        Float lastMotionX = 0;
        ev->GetX(newPointerIndex, &lastMotionX);
        mLastMotionX = (Int32)lastMotionX;
        ev->GetPointerId(newPointerIndex, &mActivePointerId);
        if (mVelocityTracker != NULL) {
            mVelocityTracker->Clear();
        }
    }
}

Boolean HorizontalScrollView::OnGenericMotionEvent(
    /* [in] */ IMotionEvent* event)
{
    AutoPtr<CMotionEvent> cevent = (CMotionEvent*)event;
    Int32 source = 0;
    cevent->GetSource(&source);
    if((source & IInputDevice::SOURCE_CLASS_POINTER) != 0) {
        Int32 action = 0;
        event->GetAction(&action);
        switch(action) {
            case IMotionEvent::ACTION_SCROLL:
                if(!mIsBeingDragged) {
                    Float hscroll = 0;
                    Int32 metaState = 0;
                    event->GetMetaState(&metaState);
                    if((metaState & IKeyEvent::META_SHIFT_ON) != 0) {
                        Float fhscroll = 0;
                        event->GetAxisValue(IMotionEvent::AXIS_VSCROLL, &fhscroll);
                        hscroll = -fhscroll;
                    } else {
                        event->GetAxisValue(IMotionEvent::AXIS_HSCROLL, &hscroll);
                    }

                    if(hscroll != 0) {
                        Int32 delta = (Int32)(hscroll * GetHorizontalScrollFactor());
                        Int32 range = GetScrollRange();
                        Int32 oldScrollX = mScrollX;
                        Int32 newScrollX = oldScrollX + delta;
                        if(newScrollX < 0) {
                            newScrollX = 0;
                        } else if(newScrollX > range) {
                            newScrollX = range;
                        }
                        if(newScrollX != oldScrollX) {
                            FrameLayout::ScrollTo(newScrollX, mScrollY);
                            return TRUE;
                        }
                    }
                }
        }
    }
    return FrameLayout::OnGenericMotionEvent(event);
}

Boolean HorizontalScrollView::ShouldDelayChildPressedState()
{
    return TRUE;
}

void HorizontalScrollView::OnOverScrolled(
    /* [in] */ Int32 scrollX,
    /* [in] */ Int32 scrollY,
    /* [in] */ Boolean clampedX,
    /* [in] */ Boolean clampedY)
{
    if (!mScroller->IsFinished()) {
        mScrollX = scrollX;
        mScrollY = scrollY;
        InvalidateParentIfNeeded();
        if (clampedY) {
            mScroller->SpringBack(mScrollX, mScrollY, 0, GetScrollRange(), 0, 0);
        }
    } else {
        FrameLayout::ScrollTo(scrollX, scrollY);
    }
    AwakenScrollBars();
}

Boolean HorizontalScrollView::PerformAccessibilityAction(
    /* [in] */ Int32 action,
    /* [in] */ IBundle* arguments)
{
    if(FrameLayout::PerformAccessibilityAction(action, arguments)) {
        return TRUE;
    }
    switch(action) {
        case IAccessibilityNodeInfo::ACTION_SCROLL_FORWARD:
        {
            if(!IsEnabled()) {
                return FALSE;
            }
            Int32 viewportWidth = GetWidth() - mPaddingLeft - mPaddingRight;
            Int32 targetScrollX = Elastos::Core::Math::Min(mScrollX + viewportWidth, GetScrollRange());
            if(targetScrollX != mScrollX) {
                SmoothScrollTo(targetScrollX, 0);
                return TRUE;
            }
        }
        return FALSE;

        case IAccessibilityNodeInfo::ACTION_SCROLL_BACKWARD:
        {
            if(!IsEnabled()) {
                return FALSE;
            }
            Int32 viewportWidth = GetWidth() - mPaddingLeft - mPaddingRight;
            Int32 targetScrollX = Elastos::Core::Math::Max(0, mScrollX - viewportWidth);
            if(targetScrollX != mScrollX) {
                SmoothScrollTo(targetScrollX, 0);
                return TRUE;
            }
        }
        return FALSE;
    }
    return FALSE;
}

ECode HorizontalScrollView::OnInitializeAccessibilityNodeInfo(
    /* [in] */ IAccessibilityNodeInfo* info)
{
    FrameLayout::OnInitializeAccessibilityNodeInfo(info);
    AutoPtr<ICharSequence> seq;
    FAIL_RETURN(CStringWrapper::New(HORIZONTALSCROLLVIEW_NAME, (ICharSequence**)&seq));
    info->SetClassName(seq);
    Int32 scrollRange = GetScrollRange();
    if(scrollRange > 0) {
        info->SetScrollable(TRUE);
        if(IsEnabled() && mScrollX > 0) {
            info->AddAction(IAccessibilityNodeInfo::ACTION_SCROLL_BACKWARD);
        }
        if(IsEnabled() && mScrollX < scrollRange) {
            info->AddAction(IAccessibilityNodeInfo::ACTION_SCROLL_FORWARD);
        }
    }
    return NOERROR;
}

ECode HorizontalScrollView::OnInitializeAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    FrameLayout::OnInitializeAccessibilityEvent(event);
    AutoPtr<ICharSequence> seq;
    FAIL_RETURN(CStringWrapper::New(HORIZONTALSCROLLVIEW_NAME, (ICharSequence**)&seq));
    event->SetClassName(seq);
    AutoPtr<CAccessibilityEvent> cevent = (CAccessibilityEvent*)event;
    cevent->SetScrollable(GetScrollRange() > 0);
    cevent->SetScrollX(mScrollX);
    cevent->SetScrollY(mScrollY);
    cevent->SetMaxScrollX(GetScrollRange());
    cevent->SetMaxScrollY(mScrollY);
    return NOERROR;
}

Int32 HorizontalScrollView::GetScrollRange()
{
    Int32 scrollRange = 0;
    if (GetChildCount() > 0) {
        AutoPtr<IView> child = GetChildAt(0);
        Int32 childWidth;
        child->GetWidth(&childWidth);
        scrollRange = Elastos::Core::Math::Max(0,
                 childWidth - (GetWidth() - mPaddingLeft - mPaddingRight));
    }
    return scrollRange;
}

AutoPtr<IView> HorizontalScrollView::FindFocusableViewInMyBounds(
    /* [in] */ Boolean leftFocus,
    /* [in] */ Int32 left,
    /* [in] */ IView* preferredFocusable)
{
    Int32 fadingEdgeLength = GetHorizontalFadingEdgeLength() / 2;
    Int32 leftWithoutFadingEdge = left + fadingEdgeLength;
    Int32 rightWithoutFadingEdge = left + GetWidth() - fadingEdgeLength;

    if (preferredFocusable != NULL) {
        Int32 preferredLeft = 0;
        Int32 preferredRight = 0;
        preferredFocusable->GetLeft(&preferredLeft);
        preferredFocusable->GetRight(&preferredRight);
        if ((preferredLeft < rightWithoutFadingEdge)
                && (preferredRight > leftWithoutFadingEdge)) {
            return preferredFocusable;
        }
    }

    return FindFocusableViewInBounds(leftFocus, leftWithoutFadingEdge,
            rightWithoutFadingEdge);
}

AutoPtr<IView> HorizontalScrollView::FindFocusableViewInBounds(
    /* [in] */ Boolean leftFocus,
    /* [in] */ Int32 left,
    /* [in] */ Int32 right)
{
    AutoPtr<IObjectContainer> focusables;
    ASSERT_SUCCEEDED(GetFocusables(IView::FOCUS_FORWARD, (IObjectContainer**)&focusables));

    AutoPtr<IView> focusCandidate;

    Boolean foundFullyContainedFocusable = FALSE;

    AutoPtr<IObjectEnumerator> objEmu;
    ASSERT_SUCCEEDED(focusables->GetObjectEnumerator((IObjectEnumerator**)&objEmu));

    Boolean isSucceeded = FALSE;
    objEmu->MoveNext(&isSucceeded);
    while (isSucceeded) {
        AutoPtr<IView> view;
        objEmu->Current((IInterface**)&view);
        Int32 viewLeft = 0;
        Int32 viewRight = 0;
        view->GetLeft(&viewLeft);
        view->GetRight(&viewRight);

        if (left < viewRight && viewLeft < right) {

            Boolean viewIsFullyContained = (left < viewLeft) &&
                    (viewRight < right);

            if (focusCandidate == NULL) {
                focusCandidate = view;
                foundFullyContainedFocusable = viewIsFullyContained;
            }
            else {
                Int32 l = 0;
                Int32 r = 0;
                focusCandidate->GetLeft(&l);
                focusCandidate->GetRight(&r);
                Boolean viewIsCloserToBoundary =
                        (leftFocus && viewLeft < l) || (!leftFocus && viewRight > r);

                if (foundFullyContainedFocusable) {
                    if (viewIsFullyContained && viewIsCloserToBoundary) {
                        focusCandidate = view;
                    }
                } else {
                    if (viewIsFullyContained) {
                        focusCandidate = view;
                        foundFullyContainedFocusable = TRUE;
                    } else if (viewIsCloserToBoundary) {
                        focusCandidate = view;
                    }
                }
            }
        }
        objEmu->MoveNext(&isSucceeded);
    }

    return focusCandidate;
}

Boolean HorizontalScrollView::PageScroll(
    /* [in] */ Int32 direction)
{
    Boolean right = direction == IView::FOCUS_RIGHT;
    Int32 width = GetWidth();

    if (right) {
        mTempRect->mLeft = GetScrollX() + width;
        Int32 count = GetChildCount();
        if (count > 0) {
            AutoPtr<IView> view = GetChildAt(0);
            Int32 right;
            view->GetRight(&right);
            if (mTempRect->mLeft + width > right) {
                mTempRect->mLeft = right - width;
            }
        }
    } else {
        mTempRect->mLeft = GetScrollX() - width;
        if (mTempRect->mLeft < 0) {
            mTempRect->mLeft = 0;
        }
    }
    mTempRect->mRight = mTempRect->mLeft + width;

    return ScrollAndFocus(direction,
            mTempRect->mLeft, mTempRect->mRight);
}

Boolean HorizontalScrollView::FullScroll(
    /* [in] */ Int32 direction)
{
    Boolean right = direction == IView::FOCUS_RIGHT;
    Int32 width = GetWidth();

    mTempRect->mLeft = 0;
    mTempRect->mRight = width;

    if (right) {
        Int32 count = GetChildCount();
        if (count > 0) {
            AutoPtr<IView> view = GetChildAt(0);
            view->GetRight(&mTempRect->mRight);
            mTempRect->mLeft = mTempRect->mRight - width;
        }
    }

    return ScrollAndFocus(direction,
            mTempRect->mLeft, mTempRect->mRight);
}

Boolean HorizontalScrollView::ScrollAndFocus(
    /* [in] */ Int32 direction,
    /* [in] */ Int32 left,
    /* [in] */ Int32 right)
{
    Boolean handled = TRUE;
    Int32 width = GetWidth();
    Int32 containerLeft = GetScrollX();
    Int32 containerRight = containerLeft + width;
    Boolean goLeft = direction == IView::FOCUS_LEFT;

    AutoPtr<IView> newFocused = FindFocusableViewInBounds(goLeft, left, right);
    if(newFocused == NULL) {
        newFocused = (IView*)this->Probe(EIID_IView);
    }

    if(left >= containerLeft && right <= containerRight) {
        handled = FALSE;
    } else {
        Int32 delta = goLeft ? (left - containerLeft) : (right - containerRight);
        DoScrollX(delta);
    }

    if(newFocused != FindFocus()) {
        Boolean res = FALSE;
        newFocused->RequestFocus(direction, &res);
    }
    return handled;
}

Boolean HorizontalScrollView::ArrowScroll(
    /* [in] */ Int32 direction)
{
    AutoPtr<IView> currentFocused = FindFocus();
    if(currentFocused.Get() == (IView*)this->Probe(EIID_IView)) currentFocused = NULL;
    AutoPtr<IView> nextFocused;
    FocusFinder::GetInstance()->FindNextFocus((IViewGroup*)this->Probe(EIID_IViewGroup),
        currentFocused, direction, (IView**)&nextFocused);
    Int32 maxJump = GetMaxScrollAmount();

    if(nextFocused.Get() && IsWithinDeltaOfScreen(nextFocused, maxJump)) {
        nextFocused->GetDrawingRect(mTempRect);
        OffsetDescendantRectToMyCoords(nextFocused, mTempRect);
        Int32 scrollDelta = ComputeScrollDeltaToGetChildRectOnScreen(mTempRect);
        DoScrollX(scrollDelta);
        Boolean res = FALSE;
        nextFocused->RequestFocus(direction, &res);
    } else {
        Int32 scrollDelta = maxJump;
        if(direction == IView::FOCUS_LEFT && GetScrollX() < scrollDelta) {
            scrollDelta = GetScrollX();
        } else if(direction == IView::FOCUS_RIGHT && GetChildCount() > 0) {
            Int32 daRight = 0;
            GetChildAt(0)->GetRight(&daRight);
            Int32 screenRight = GetScrollX() + GetWidth();

            if(daRight - screenRight < maxJump) {
                scrollDelta = daRight - screenRight;
            }
        }
        if(scrollDelta == 0) {
            return FALSE;
        }
        DoScrollX(direction == IView::FOCUS_RIGHT ? scrollDelta : -scrollDelta);
    }

    if (currentFocused) {
        Boolean isFocused = FALSE;
        currentFocused->IsFocused(&isFocused);
        if (isFocused && IsOffScreen(currentFocused)) {
            Int32 descendantFocusability = GetDescendantFocusability();
            SetDescendantFocusability(IViewGroup::FOCUS_BEFORE_DESCENDANTS);
            RequestFocus();
            SetDescendantFocusability(descendantFocusability);
        }
    }

    return TRUE;
}

Boolean HorizontalScrollView::IsOffScreen(
    /* [in] */ IView* descendant)
{
    return !IsWithinDeltaOfScreen(descendant, 0);
}

Boolean HorizontalScrollView::IsWithinDeltaOfScreen(
    /* [in] */ IView* descendant,
    /* [in] */ Int32 delta)
{
    descendant->GetDrawingRect(mTempRect);
    OffsetDescendantRectToMyCoords(descendant, mTempRect);

    return (mTempRect->mRight + delta) >= GetScrollX()
            && (mTempRect->mLeft - delta) <= (GetScrollX() + GetWidth());
}

void HorizontalScrollView::DoScrollX(
    /* [in] */ Int32 delta)
{
    if (delta != 0) {
        if (mSmoothScrollingEnabled) {
            SmoothScrollBy(delta, 0);
        }
        else {
            ScrollBy(delta, 0);
        }
    }
}

ECode HorizontalScrollView::SmoothScrollBy(
    /* [in] */ Int32 dx,
    /* [in] */ Int32 dy)
{
    if(GetChildCount() == 0) {
        return NOERROR;
    }

    Int64 duration = AnimationUtils::CurrentAnimationTimeMillis() - mLastScroll;
    if(duration > ANIMATED_SCROLL_GAP) {
        Int32 width = GetWidth() - mPaddingRight - mPaddingLeft;
        Int32 right = 0;
        GetChildAt(0)->GetWidth(&right);
        Int32 maxX= Elastos::Core::Math::Max(0, right - width);
        Int32 scrollX = mScrollX;
        dx = Elastos::Core::Math::Max(0, Elastos::Core::Math::Min(scrollX + dx, maxX)) - scrollX;

        mScroller->StartScroll(scrollX, mScrollY, dx, 0);
        PostInvalidateOnAnimation();
    } else {
        if(!mScroller->IsFinished()) {
            mScroller->AbortAnimation();
        }
        ScrollBy(dx, dy);
    }
    mLastScroll = AnimationUtils::CurrentAnimationTimeMillis();
    return NOERROR;
}

ECode HorizontalScrollView::SmoothScrollTo(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    return SmoothScrollBy(x - mScrollX, y - mScrollY);
}

Int32 HorizontalScrollView::ComputeHorizontalScrollRange()
{
    Int32 count = GetChildCount();
    Int32 contentWidth = GetWidth() - mPaddingLeft - mPaddingRight;
    if (count == 0) {
        return contentWidth;
    }

    Int32 scrollRange = 0;
    GetChildAt(0)->GetRight(&scrollRange);
    Int32 scrollX = mScrollX;
    Int32 overscrollRight = Elastos::Core::Math::Max(0, scrollRange - contentWidth);
    if (scrollX < 0) {
        scrollRange -= scrollX;
    }
    else if (scrollX > overscrollRight) {
        scrollRange += scrollX - overscrollRight;
    }

    return scrollRange;
}

Int32 HorizontalScrollView::ComputeHorizontalScrollOffset()
{
    return Elastos::Core::Math::Max(0, FrameLayout::ComputeHorizontalScrollOffset());
}

void HorizontalScrollView::MeasureChild(
    /* [in] */ IView* child,
    /* [in] */ Int32 parentWidthMeasureSpec,
    /* [in] */ Int32 parentHeightMeasureSpec)
{
    AutoPtr<IViewGroupLayoutParams> lp;
    child->GetLayoutParams((IViewGroupLayoutParams**)&lp);

    Int32 childWidthMeasureSpec = 0;
    Int32 childHeightMeasureSpec = 0;

    Int32 height = 0;
    lp->GetHeight(&height);
    childHeightMeasureSpec = GetChildMeasureSpec(parentHeightMeasureSpec,
            mPaddingTop + mPaddingBottom, height);

    childWidthMeasureSpec = MeasureSpec::MakeMeasureSpec(0, MeasureSpec::UNSPECIFIED);

    child->Measure(childWidthMeasureSpec, childHeightMeasureSpec);
}

void HorizontalScrollView::MeasureChildWithMargins(
    /* [in] */ IView* child,
    /* [in] */ Int32 parentWidthMeasureSpec,
    /* [in] */ Int32 widthUsed,
    /* [in] */ Int32 parentHeightMeasureSpec,
    /* [in] */ Int32 heightUsed)
{
    AutoPtr<IViewGroupMarginLayoutParams> params;
    child->GetLayoutParams((IViewGroupLayoutParams**)&params);
    CViewGroupMarginLayoutParams* lp = (CViewGroupMarginLayoutParams*)params.Get();

    Int32 childHeightMeasureSpec = GetChildMeasureSpec(parentHeightMeasureSpec,
            mPaddingTop + mPaddingBottom + lp->mTopMargin + lp->mBottomMargin
            + heightUsed, lp->mHeight);
    Int32 childWidthMeasureSpec = MeasureSpec::MakeMeasureSpec(
            lp->mLeftMargin + lp->mRightMargin, MeasureSpec::UNSPECIFIED);

    child->Measure(childWidthMeasureSpec, childHeightMeasureSpec);
}

ECode HorizontalScrollView::ComputeScroll()
{
   if(mScroller->ComputeScrollOffset()) {
        Int32 oldX = mScrollX;
        Int32 oldY = mScrollY;
        Int32 x = mScroller->GetCurrX();
        Int32 y = mScroller->GetCurrY();

        if(oldX != x || oldY != y) {
            Int32 range = GetScrollRange();
            Int32 overscrollMode = GetOverScrollMode();
            Boolean canOverscroll = overscrollMode == IView::OVER_SCROLL_ALWAYS
                || (overscrollMode == IView::OVER_SCROLL_IF_CONTENT_SCROLLS && range >0);

            OverScrollBy(x - oldX, y - oldY, oldX, oldY, range, 0,
                mOverflingDistance, 0, FALSE);
            OnScrollChanged(mScrollX, mScrollY, oldX, oldY);

            if(canOverscroll) {
                if(x < 0 && oldX >= 0) {
                    mEdgeGlowLeft->OnAbsorb((Int32) mScroller->GetCurrVelocity());
                } else if(x > range && oldX <= range) {
                    mEdgeGlowRight->OnAbsorb((Int32) mScroller->GetCurrVelocity());
                }
            }
        }
        if(!AwakenScrollBars()) {
            PostInvalidateOnAnimation();
        }
   }
    return NOERROR;
}

void HorizontalScrollView::ScrollToChild(
    /* [in] */ IView* child)
{
    child->GetDrawingRect(mTempRect);

    OffsetDescendantRectToMyCoords(child, mTempRect);

    Int32 scrollDelta = ComputeScrollDeltaToGetChildRectOnScreen(mTempRect);

    if (scrollDelta != 0) {
        ScrollBy(scrollDelta, 0);
    }
}

Boolean HorizontalScrollView::ScrollToChildRect(
    /* [in] */ IRect* rect,
    /* [in] */ Boolean immediate)
{
    Int32 delta = ComputeScrollDeltaToGetChildRectOnScreen(rect);
    Boolean scroll = delta != 0;
    if (scroll) {
        if (immediate) {
            ScrollBy(delta, 0);
        }
        else {
            SmoothScrollBy(delta, 0);
        }
    }
    return scroll;
}

Int32 HorizontalScrollView::ComputeScrollDeltaToGetChildRectOnScreen(
    /* [in] */ IRect* rect)
{
    if(GetChildCount() == 0) return 0;

    Int32 width = GetWidth();
    Int32 screenLeft = GetScrollX();
    Int32 screenRight = screenLeft + width;

    Int32 fadingEdge = GetHorizontalFadingEdgeLength();

    CRect* cRect = (CRect*)rect;
    if(cRect->mLeft > 0) {
        screenLeft += fadingEdge;
    }
    Int32 childWidth= 0;
    GetChildAt(0)->GetWidth(&childWidth);
    if(cRect->mRight < childWidth) {
        screenRight -= fadingEdge;
    }
    Int32 scrollXDelta = 0;
    if(cRect->mRight > screenRight && cRect->mLeft > screenLeft) {
        Int32 cWidth = 0;
        cRect->GetWidth(&cWidth);
        if(cWidth > width) {
            scrollXDelta += (cRect->mLeft - screenLeft);
        } else {
            scrollXDelta += (cRect->mRight - screenRight);
        }

        Int32 right = 0;
        GetChildAt(0)->GetRight(&right);
        Int32 distanceToRight = right - screenRight;
        scrollXDelta = Elastos::Core::Math::Min(scrollXDelta, distanceToRight);
    } else if(cRect->mLeft < screenLeft && cRect->mRight < screenRight) {
        Int32 cWidth = 0;
        cRect->GetWidth(&cWidth);
        if(cWidth > width) {
            scrollXDelta -= (screenRight - cRect->mRight);
        } else {
            scrollXDelta -= (screenLeft - cRect->mLeft);
        }
        scrollXDelta = Elastos::Core::Math::Max(scrollXDelta, -GetScrollX());
    }
    return scrollXDelta;
}

ECode HorizontalScrollView::RequestChildFocus(
    /* [in] */ IView* child,
    /* [in] */ IView* focused)
{
    if (!mIsLayoutDirty) {
        ScrollToChild(focused);
    }
    else {
         mChildToScrollTo = focused;
    }

    return FrameLayout::RequestChildFocus(child, focused);
}

Boolean HorizontalScrollView::OnRequestFocusInDescendants(
    /* [in] */ Int32 direction,
    /* [in] */ IRect* previouslyFocusedRect)
{
    if (direction == IView::FOCUS_FORWARD) {
        direction = IView::FOCUS_RIGHT;
    } else if (direction == IView::FOCUS_BACKWARD) {
        direction = IView::FOCUS_LEFT;
    }

    AutoPtr<IView> nextFocus;
    if(previouslyFocusedRect) {
        FocusFinder::GetInstance()->FindNextFocusFromRect(
                    (IViewGroup*)this->Probe(EIID_IViewGroup), previouslyFocusedRect, direction, (IView**)&nextFocus);
    } else {
        FocusFinder::GetInstance()->FindNextFocus(
                    (IViewGroup*)this->Probe(EIID_IViewGroup), NULL, direction, (IView**)&nextFocus);
    }

    if (nextFocus == NULL) {
        return FALSE;
    }

    if (IsOffScreen(nextFocus)) {
        return FALSE;
    }

    Boolean result = FALSE;
    ASSERT_SUCCEEDED(
            nextFocus->RequestFocus(direction, previouslyFocusedRect, &result));

    return result;
}

Boolean HorizontalScrollView::RequestChildRectangleOnScreen(
    /* [in] */ IView* child,
    /* [in] */ IRect* rectangle,
    /* [in] */ Boolean immediate)
{
    Int32 left = 0;
    Int32 scrollX = 0;
    Int32 top = 0;
    Int32 scrollY = 0;
    child->GetLeft(&left);
    child->GetScrollX(&scrollX);
    child->GetTop(&top);
    child->GetScrollY(&scrollY);
    rectangle->Offset(left - scrollX, top - scrollY);

    return ScrollToChildRect(rectangle, immediate);
}

ECode HorizontalScrollView::RequestLayout()
{
    mIsLayoutDirty = TRUE;
    return FrameLayout::RequestLayout();
}

void HorizontalScrollView::OnLayout(
    /* [in] */ Boolean changed,
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    FrameLayout::OnLayout(changed, left, top, right, bottom);
    mIsLayoutDirty = FALSE;
    if (mChildToScrollTo != NULL &&
            IsViewDescendantOf(mChildToScrollTo.Get(), (IView*)this->Probe(EIID_IView))) {
        ScrollToChild(mChildToScrollTo);
    }
    mChildToScrollTo = NULL;

    ScrollTo(mScrollX, mScrollY);
}

void HorizontalScrollView::OnSizeChanged(
    /* [in] */ Int32 w,
    /* [in] */ Int32 h,
    /* [in] */ Int32 oldw,
    /* [in] */ Int32 oldh)
{
    FrameLayout::OnSizeChanged(w, h, oldw, oldh);

    AutoPtr<IView> currentFocused = FindFocus();
    if (NULL == currentFocused || (IView*)this->Probe(EIID_IView) == currentFocused.Get()) {
        return;
    }

    Int32 maxJump = mRight - mLeft;

    if (IsWithinDeltaOfScreen(currentFocused, maxJump)) {
        currentFocused->GetDrawingRect(mTempRect);
        OffsetDescendantRectToMyCoords(currentFocused, mTempRect);
        Int32 scrollDelta = ComputeScrollDeltaToGetChildRectOnScreen(mTempRect);
        DoScrollX(scrollDelta);
    }
}

Boolean HorizontalScrollView::IsViewDescendantOf(
    /* [in] */ IView* child,
    /* [in] */ IView* parent)
{
    if (child == parent) {
        return TRUE;
    }

    AutoPtr<IViewParent> theParent;
    child->GetParent((IViewParent**)&theParent);
    AutoPtr<IViewGroup> viewGroup = IViewGroup::Probe(theParent);
    AutoPtr<IView> viewParent = IView::Probe(theParent);
    return (viewGroup != NULL) && IsViewDescendantOf(viewParent, parent);
}

ECode HorizontalScrollView::Fling(
    /* [in] */ Int32 velocityX)
{
    if(GetChildCount() > 0) {
        Int32 width = GetWidth() - mPaddingRight - mPaddingLeft;
        Int32 right = 0;
        GetChildAt(0)->GetWidth(&right);

        mScroller->Fling(mScrollX, mScrollY, velocityX, 0, 0,
            Elastos::Core::Math::Max(0, right - width), 0, 0, width/2, 0);
        Boolean movingRight = velocityX > 0;
        AutoPtr<IView> currentFocused = FindFocus();
        AutoPtr<IView> newFocused = FindFocusableViewInMyBounds(movingRight,
            mScroller->GetFinalX(), currentFocused);

        if(!newFocused) {
            newFocused = (IView*)this->Probe(EIID_IView);
        }

        if(newFocused.Get() != currentFocused.Get()) {
            Boolean res = FALSE;
            newFocused->RequestFocus(movingRight ? IView::FOCUS_RIGHT
                : IView::FOCUS_LEFT, &res);
        }
        PostInvalidateOnAnimation();
    }
    return NOERROR;
}

ECode HorizontalScrollView::ScrollTo(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    if (GetChildCount() > 0) {
        AutoPtr<IView> child = GetChildAt(0);
        Int32 width = 0;
        Int32 height = 0;
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

ECode HorizontalScrollView::SetOverScrollMode(
    /* [in] */ Int32 overScrollMode)
{
    if (overScrollMode != IView::OVER_SCROLL_NEVER) {
        if (mEdgeGlowLeft == NULL) {
            CEdgeEffect::New(GetContext(), (IEdgeEffect**)&mEdgeGlowLeft);
            mEdgeGlowRight = NULL;
            CEdgeEffect::New(GetContext(), (IEdgeEffect**)&mEdgeGlowRight);
        }
    } else {
        mEdgeGlowLeft = NULL;
        mEdgeGlowRight = NULL;
    }
    return FrameLayout::SetOverScrollMode(overScrollMode);
}

ECode HorizontalScrollView::Draw(
    /* [in] */ ICanvas* canvas)
{
    FAIL_RETURN(FrameLayout::Draw(canvas));

    if (mEdgeGlowLeft != NULL) {
        Int32 scrollX = mScrollX;
        Boolean res = FALSE;
        mEdgeGlowLeft->IsFinished(&res);
        if (!res) {
            Int32 restoreCount = 0;
            canvas->Save(&restoreCount);
            Int32 height = GetHeight() - mPaddingTop - mPaddingBottom;;

            canvas->Rotate(270);
            canvas->Translate(-height + mPaddingTop, Elastos::Core::Math::Min(0, scrollX));
            mEdgeGlowLeft->SetSize(height, GetWidth());
            Boolean isDraw = FALSE;
            mEdgeGlowLeft->Draw(canvas, &isDraw);
            if (isDraw) {
                PostInvalidateOnAnimation();
            }
            canvas->RestoreToCount(restoreCount);
        }
        Boolean isFinished = FALSE;
        mEdgeGlowLeft->IsFinished(&isFinished);
        if (!isFinished) {
            Int32 restoreCount = 0;
            canvas->Save(&restoreCount);
            Int32 width = GetWidth();
            Int32 height = GetHeight() - mPaddingTop - mPaddingBottom;

            canvas->Rotate(90);
            canvas->Translate(-mPaddingTop, Elastos::Core::Math::Max(GetScrollRange(), scrollX) + width);
            mEdgeGlowRight->SetSize(height, width);
            Boolean isDraw = FALSE;
            mEdgeGlowLeft->Draw(canvas, &isDraw);
            if (isDraw) {
                PostInvalidateOnAnimation();
            }
            canvas->RestoreToCount(restoreCount);
        }
    }
    return NOERROR;
}

Int32 HorizontalScrollView::Clamp(
    /* [in] */ Int32 n,
    /* [in] */ Int32 my,
    /* [in] */ Int32 child)
{
    if (my >= child || n < 0) {
        return 0;
    }
    if ((my + n) > child) {
        return child - my;
    }
    return n;
}

} // namespace Widget
} // namespace Droid
} // namespace Elastos
