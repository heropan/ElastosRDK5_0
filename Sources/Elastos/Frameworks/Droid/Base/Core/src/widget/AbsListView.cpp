
#include "widget/AbsListView.h"
#include <elastos/core/Math.h>
#include <elastos/utility/logging/Logger.h>
#include <elastos/core/StringBuffer.h>
#include <elastos/core/StringUtils.h>
#include "R.h"
#include "util/StateSet.h"
#include "os/CStrictMode.h"
#include "view/CKeyEvent.h"
#include "view/accessibility/CAccessibilityManager.h"
#include "widget/CAbsListViewLayoutParams.h"
#include "widget/CAbsListViewSavedState.h"
#include "widget/OverScroller.h"
#include "widget/CEdgeEffect.h"
#include "widget/FastScroller.h"
#include "widget/CPopupWindow.h"

namespace Elastos{
namespace Droid{
namespace Widget{

using Elastos::Utility::Logging::Logger;
using Elastos::Core::IBoolean;
using Elastos::Core::CBoolean;
using Elastos::Core::EIID_IRunnable;
using Elastos::Core::CStringWrapper;
using Elastos::Core::StringUtils;
using Elastos::Utility::CObjectInt32Map;
using Elastos::Droid::R;
using Elastos::Droid::Os::IStrictMode;
using Elastos::Droid::Os::CStrictMode;
using Elastos::Droid::Utility::IDisplayMetrics;
using Elastos::Droid::Utility::StateSet;
using Elastos::Droid::Text::IInputType;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::Pm::IApplicationInfo;
using Elastos::Droid::Content::IIntentFilterComparison;
using Elastos::Droid::Content::CIntentFilterComparison;
using Elastos::Droid::Content::Res::ITypedArray;
using Elastos::Droid::Graphics::Drawable::ITransitionDrawable;
using Elastos::Droid::Graphics::Drawable::EIID_ITransitionDrawable;
using Elastos::Droid::Graphics::Drawable::IDrawableCallback;
using Elastos::Droid::Graphics::Drawable::EIID_IDrawableCallback;
using Elastos::Droid::View::IViewConfigurationHelper;
using Elastos::Droid::View::IViewConfiguration;
using Elastos::Droid::View::CViewConfigurationHelper;
using Elastos::Droid::View::IHapticFeedbackConstants;
using Elastos::Droid::View::IInputDevice;
using Elastos::Droid::View::IGravity;
using Elastos::Droid::View::IViewGroup;
using Elastos::Droid::View::IDispatcherState;
using Elastos::Droid::View::IKeyEventHelper;
using Elastos::Droid::View::CKeyEventHelper;
using Elastos::Droid::View::IKeyEvent;
using Elastos::Droid::View::InputMethod::IEditorInfo;
using Elastos::Droid::View::ILayoutInflater;
using Elastos::Droid::View::EIID_IView;
using Elastos::Droid::View::EIID_IViewGroup;
using Elastos::Droid::View::IAccessibilityDelegate;
using Elastos::Droid::View::IViewTreeObserver;
using Elastos::Droid::View::EIID_IOnTouchModeChangeListener;
using Elastos::Droid::View::EIID_IOnGlobalLayoutListener;
using Elastos::Droid::View::IKeyEventCallback;
using Elastos::Droid::View::EIID_IKeyEventCallback;
using Elastos::Droid::View::Accessibility::IAccessibilityManager;
using Elastos::Droid::View::Accessibility::CAccessibilityManager;
using Elastos::Droid::Widget::IAbsListView;
using Elastos::Droid::Widget::EIID_IAbsListView;
using Elastos::Droid::Widget::IFilterListener;
using Elastos::Droid::Widget::EIID_IFilterListener;
using Elastos::Droid::Widget::IRecyclerListener;
using Elastos::Droid::View::EIID_IViewGroupLayoutParams;

CAR_INTERFACE_IMPL(AbsListView::MultiChoiceModeWrapper, IMultiChoiceModeListener)


//==============================================================================
//          AbsListView::ListItemAccessibilityDelegate
//==============================================================================

AbsListView::ListItemAccessibilityDelegate::ListItemAccessibilityDelegate(
    /* [in] */ AbsListView* host)
{
    mHost = host;
}

//@Override
ECode AbsListView::ListItemAccessibilityDelegate::OnInitializeAccessibilityNodeInfo(
     /* [in] */ IView* host,
     /* [in] */ IAccessibilityNodeInfo* info)
{
    AccessibilityDelegate::OnInitializeAccessibilityNodeInfo(host, info);

    Int32 position = mHost->GetPositionForView(host);
    AutoPtr<IAdapter> adapterTemp;
    ((IAdapterView*)(mHost->Probe(EIID_IAdapterView)))->GetAdapter((IAdapter**)&adapterTemp);
    AutoPtr<IListAdapter> adapter = (IListAdapter*)adapterTemp->Probe(EIID_IListAdapter);

    if ((position == IAdapterView::INVALID_POSITION) || (adapter == NULL)) {
        return NOERROR;
    }

    Boolean adapterEnabled;
    adapter->IsEnabled(position, &adapterEnabled);
    if (!mHost->IsEnabled() || !adapterEnabled) {
        return NOERROR;
    }

    if (position == mHost->GetSelectedItemPosition()) {
        info->SetSelected(TRUE);
        info->AddAction(IAccessibilityNodeInfo::ACTION_CLEAR_SELECTION);
    } else {
        info->AddAction(IAccessibilityNodeInfo::ACTION_SELECT);
    }

    if (mHost->IsClickable()) {
        info->AddAction(IAccessibilityNodeInfo::ACTION_CLICK);
        info->SetClickable(TRUE);
    }

    if (mHost->IsLongClickable()) {
        info->AddAction(IAccessibilityNodeInfo::ACTION_LONG_CLICK);
        info->SetLongClickable(TRUE);
    }
    return NOERROR;
}

//@Override
ECode AbsListView::ListItemAccessibilityDelegate::PerformAccessibilityAction(
    /* [in] */ IView* host,
    /* [in] */ Int32 action,
    /* [in] */ IBundle* arguments,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    Boolean bval;
    View::AccessibilityDelegate::PerformAccessibilityAction(host, action, arguments, &bval);
    if (bval) {
        *result = TRUE;
        return NOERROR;
    }

    *result = FALSE;

    Int32 position = mHost->GetPositionForView(host);
    AutoPtr<IAdapter> adapterTemp;
    ((IAdapterView*)(mHost->Probe(EIID_IAdapterView)))->GetAdapter((IAdapter**)&adapterTemp);
    AutoPtr<IListAdapter> adapter = (IListAdapter*)adapterTemp->Probe(EIID_IListAdapter);

    if ((position == IAdapterView::INVALID_POSITION) || (adapter == NULL)) {
        // Cannot perform actions on invalid items.
        return NOERROR;
    }

    Boolean adapterEnabled;
    adapter->IsEnabled(position, &adapterEnabled);
    if (!mHost->IsEnabled() || !adapterEnabled) {
        // Cannot perform actions on disabled items.
        return NOERROR;
    }

    Int64 id = mHost->GetItemIdAtPosition(position);

    switch (action) {
        case IAccessibilityNodeInfo::ACTION_CLEAR_SELECTION:
        {
            if (mHost->GetSelectedItemPosition() == position) {
                mHost->SetSelection(IAdapterView::INVALID_POSITION);
                *result = TRUE;
            }
            return NOERROR;
        }
        case IAccessibilityNodeInfo::ACTION_SELECT:
        {
            if (mHost->GetSelectedItemPosition() != position) {
                mHost->SetSelection(position);
                *result = TRUE;
            }
            return NOERROR;
        }
        case IAccessibilityNodeInfo::ACTION_CLICK:
        {
            if (mHost->IsClickable()) {
                *result = mHost->PerformItemClick(host, position, id);
            }
            return NOERROR;
        }
        case IAccessibilityNodeInfo::ACTION_LONG_CLICK:
        {
            if (mHost->IsLongClickable()) {
                *result = mHost->PerformLongPress(host, position, id);
            }
            return NOERROR;
        }
    }

    return NOERROR;
}

//==============================================================================
//          AbsListView::WindowRunnnable
//==============================================================================

AbsListView::WindowRunnnable::WindowRunnnable(
    /* [in] */ AbsListView* host)
    : mHost(host)
    , mOriginalAttachCount(0)
{}

void AbsListView::WindowRunnnable::RememberWindowAttachCount()
{
    mOriginalAttachCount = mHost->GetWindowAttachCount();
}

Boolean AbsListView::WindowRunnnable::SameWindow()
{
    return mHost->HasWindowFocus() &&
        mHost->GetWindowAttachCount() == mOriginalAttachCount;
}

//==============================================================================
//          AbsListView::AbsListViewPerformClick
//==============================================================================

AbsListView::AbsListViewPerformClick::AbsListViewPerformClick (
    /* [in] */ AbsListView* host)
    : WindowRunnnable(host)
{}

ECode AbsListView::AbsListViewPerformClick::Run()
{
    // The data has changed since we posted this action in the event queue,
    // bail out before bad things happen
    if (mHost->mDataChanged) {
        return NOERROR;
    }

    AutoPtr<IListAdapter> adapter = mHost->mAdapter;
    Int32 motionPosition = mClickMotionPosition;
    if (adapter != NULL && mHost->mItemCount > 0
        && motionPosition != IAdapterView::INVALID_POSITION) {
        Int32 count;
        adapter->GetCount(&count);
        if (motionPosition < count && SameWindow()) {
            AutoPtr<IView> view = mHost->GetChildAt(motionPosition - mHost->mFirstPosition);
            // If there is no view, something bad happened (the view scrolled off the
            // screen, etc.) and we should cancel the click
            if (view != NULL) {
                Int64 id;
                adapter->GetItemId(motionPosition,&id);
                mHost->PerformItemClick(view, motionPosition, id);
            }
        }
    }
    return NOERROR;
}

//==============================================================================
//          AbsListView::CheckForLongPress
//==============================================================================

AbsListView::CheckForLongPress::CheckForLongPress (
    /* [in] */ AbsListView* host)
    : WindowRunnnable(host)
{}

ECode AbsListView::CheckForLongPress::Run()
{
    Int32 motionPosition = mHost->mMotionPosition;
    AutoPtr<IView> child = mHost->GetChildAt(motionPosition - mHost->mFirstPosition);
    if (child != NULL) {
        Int32 longPressPosition = mHost->mMotionPosition;
        Int64 longPressId;
        mHost->mAdapter->GetItemId(mHost->mMotionPosition, &longPressId);

        Boolean handled = FALSE;
        if (SameWindow() && !mHost->mDataChanged) {
            handled = mHost->PerformLongPress(child, longPressPosition, longPressId);
        }
        if (handled) {
            mHost->mTouchMode = AbsListView::TOUCH_MODE_REST;
            mHost->SetPressed(FALSE);
            child->SetPressed(FALSE);
        }
        else {
            mHost->mTouchMode = AbsListView::TOUCH_MODE_DONE_WAITING;
        }
    }
    return NOERROR;
}

//==============================================================================
//          AbsListView::CheckForKeyLongPress
//==============================================================================

AbsListView::CheckForKeyLongPress::CheckForKeyLongPress (
    /* [in] */ AbsListView* host)
    : WindowRunnnable(host)
{}

ECode AbsListView::CheckForKeyLongPress::Run()
{
    if(mHost->IsPressed() && mHost->mSelectedPosition >= 0){
        AutoPtr<IView> child = mHost->GetChildAt(
                mHost->mSelectedPosition - mHost->mFirstPosition);
        if(!mHost->mDataChanged)
        {
            Boolean handled = FALSE;
            if (SameWindow()) {
                 handled = mHost->PerformLongPress(
                     child, mHost->mSelectedPosition, mHost->mSelectedRowId);
            }
            if (handled) {
                mHost->SetPressed(FALSE);
                child->SetPressed(FALSE);
            }
        }else{
            mHost->SetPressed(FALSE);
            if(child != NULL)
                child->SetPressed(FALSE);
        }
    }

    return NOERROR;
}

//==============================================================================
//          AbsListView::CheckForTap
//==============================================================================

AbsListView::CheckForTap::CheckForTap(
    /* [in] */ AbsListView* host)
    : mHost(host)
{}

ECode AbsListView::CheckForTap::Run()
{
    if (mHost->mTouchMode == mHost->TOUCH_MODE_DOWN) {
        mHost->mTouchMode = mHost->TOUCH_MODE_TAP;
        AutoPtr<IView> child = mHost->GetChildAt(
            mHost->mMotionPosition - mHost->mFirstPosition);

        if (child != NULL) {
            Boolean hasFocusable;
            child->HasFocusable(&hasFocusable);
            if (!hasFocusable) {
                mHost->mLayoutMode = mHost->LAYOUT_NORMAL;
                if (!mHost->mDataChanged) {
                    child->SetPressed(TRUE);
                    mHost->SetPressed(TRUE);
                    mHost->LayoutChildren();
                    mHost->PositionSelector(mHost->mMotionPosition, child);
                    mHost->RefreshDrawableState();

                    AutoPtr<IViewConfigurationHelper> configHelper;
                    CViewConfigurationHelper::AcquireSingleton((IViewConfigurationHelper**)&configHelper);
                    Int32 longPressTimeout;
                    configHelper->GetLongPressTimeout(&longPressTimeout);
                    Boolean longClickable = mHost->IsLongClickable();
                    if (mHost->mSelector != NULL) {
                        AutoPtr<IDrawable> d;
                        mHost->mSelector->GetCurrent((IDrawable**)&d);
                        AutoPtr<ITransitionDrawable> td ;
                        if(d != NULL)
                            td = ITransitionDrawable::Probe(d);

                        if (td != NULL) {
                            if (longClickable) {
                                td->StartTransition(longPressTimeout);
                            } else {
                                td->ResetTransition();
                            }
                        }
                    }

                    if (longClickable) {
                        if (mHost->mPendingCheckForLongPress == NULL) {
                            mHost->mPendingCheckForLongPress =
                                new AbsListView::CheckForLongPress(mHost);
                        }

                        mHost->mPendingCheckForLongPress->RememberWindowAttachCount();
                        mHost->PostDelayed(mHost->mPendingCheckForLongPress, longPressTimeout);
                    }
                    else {
                        mHost->mTouchMode = mHost->TOUCH_MODE_DONE_WAITING;
                    }
                }
                else {
                    mHost->mTouchMode = mHost->TOUCH_MODE_DONE_WAITING;
                }
            }
        }
    }

    return NOERROR;
}

//==============================================================================
//          AbsListView::FlingRunnableInner
//==============================================================================

AbsListView::FlingRunnableInner::FlingRunnableInner(
    /* [in] */ FlingRunnable* host,
    /* [in] */ AbsListView* mainHost)
    : mHost(host)
    , mMainHost(mainHost)
{
}

ECode AbsListView::FlingRunnableInner::Run()
{
    Int32 activeId = mMainHost->mActivePointerId;

    if (mMainHost->mVelocityTracker == NULL || activeId == AbsListView::INVALID_POINTER) {
        return NOERROR;
    }

    mMainHost->mVelocityTracker->ComputeCurrentVelocity(1000, mMainHost->mMaximumVelocity);
    Float y;
    mMainHost->mVelocityTracker->GetYVelocity(activeId, &y);
    Float yvel = -y;

    if (Elastos::Core::Math::Abs(yvel) >= mMainHost->mMinimumVelocity
            && mHost->mScroller->IsScrollingInDirection(0, yvel)) {
        // Keep the fling alive a little longer
        mMainHost->PostDelayed(this, FlingRunnable::FLYWHEEL_TIMEOUT);
    } else {
        mHost->EndFling();
        mMainHost->mTouchMode = AbsListView::TOUCH_MODE_SCROLL;
        mMainHost->ReportScrollStateChange(IAbsListViewOnScrollListener::SCROLL_STATE_TOUCH_SCROLL);
    }
    return NOERROR;
}


//==============================================================================
//          AbsListView::FlingRunnableInner
//==============================================================================

AbsListView::FlingRunnable::FlingRunnable(
    /* [in] */ AbsListView* host)
    : mHost(host)
{
    mScroller = new OverScroller(mHost->GetContext());
    mCheckFlywheel = new FlingRunnableInner(this, mHost);
}

void AbsListView::FlingRunnable::Start(
    /* [in] */ Int32 initialVelocity)
{
    Int32 initialY = initialVelocity < 0 ? Elastos::Core::Math::INT32_MAX_VALUE : 0;
    mLastFlingY = initialY;
    mScroller->SetInterpolator(NULL);
    mScroller->Fling(0, initialY, 0, initialVelocity,
        0, Elastos::Core::Math::INT32_MAX_VALUE, 0, Elastos::Core::Math::INT32_MAX_VALUE);
    mHost->mTouchMode = AbsListView::TOUCH_MODE_FLING;

    mHost->PostOnAnimation(this);

    if (AbsListView::PROFILE_FLINGING) {
        if (!mHost->mFlingProfilingStarted) {
            //Debug.startMethodTracing("AbsListViewFling");
            mHost->mFlingProfilingStarted = TRUE;
        }
    }

    if (mHost->mFlingStrictSpan == NULL) {
//        assert(0 && "TODO"); //StrictMode is not exsit
//        mFlingStrictSpan = StrictMode.enterCriticalSpan("AbsListView-fling");
    }
}

void AbsListView::FlingRunnable::StartSpringback()
{
    if (mScroller->SpringBack(0, mHost->mScrollY, 0, 0, 0, 0)) {
        mHost->mTouchMode = AbsListView::TOUCH_MODE_OVERFLING;
        mHost->Invalidate();
        mHost->PostOnAnimation(this);
    }
    else {
        mHost->mTouchMode = AbsListView::TOUCH_MODE_REST;
        mHost->ReportScrollStateChange(IAbsListViewOnScrollListener::SCROLL_STATE_IDLE);
    }
}

void AbsListView::FlingRunnable::StartOverfling(
    /* [in] */ Int32 initialVelocity)
{
    mScroller->SetInterpolator(NULL);
    mScroller->Fling(0, mHost->mScrollY, 0, initialVelocity, 0, 0,
            Elastos::Core::Math::INT32_MIN_VALUE, Elastos::Core::Math::INT32_MAX_VALUE, 0, mHost->GetHeight());
    mHost->mTouchMode = AbsListView::TOUCH_MODE_OVERFLING;
    mHost->Invalidate();
    mHost->PostOnAnimation(this);
}

void AbsListView::FlingRunnable::EdgeReached(
    /* [in] */ Int32 delta)
{
    mScroller->NotifyVerticalEdgeReached(
        mHost->mScrollY, 0, mHost->mOverflingDistance);
    Int32 overscrollMode = mHost->GetOverScrollMode();
    if (overscrollMode == IView::OVER_SCROLL_ALWAYS ||
        (overscrollMode == IView::OVER_SCROLL_IF_CONTENT_SCROLLS
        && !mHost->ContentFits())) {
        mHost->mTouchMode = AbsListView::TOUCH_MODE_OVERFLING;
        Int32 vel = (Int32)mScroller->GetCurrVelocity();
        if (delta > 0) {
            mHost->mEdgeGlowTop->OnAbsorb(vel);
        }
        else {
            mHost->mEdgeGlowBottom->OnAbsorb(vel);
        }
    }
    else {
        mHost->mTouchMode = AbsListView::TOUCH_MODE_REST;
        if (mHost->mPositionScroller != NULL) {
            mHost->mPositionScroller->Stop();
        }
    }

    mHost->Invalidate();
    mHost->PostOnAnimation(this);
}

void AbsListView::FlingRunnable::StartScroll(
    /* [in] */ Int32 distance,
    /* [in] */ Int32 duration,
    /* [in] */ Boolean linear)
{
    Int32 initialY = distance < 0 ? Elastos::Core::Math::INT32_MAX_VALUE : 0;
    mLastFlingY = initialY;
    mScroller->SetInterpolator(linear ? AbsListView::sLinearInterpolator : NULL);
    mScroller->StartScroll(0, initialY, 0, distance, duration);
    mHost->mTouchMode = AbsListView::TOUCH_MODE_FLING;
    mHost->PostOnAnimation(this);
}

void AbsListView::FlingRunnable::EndFling()
{
    mHost->mTouchMode = AbsListView::TOUCH_MODE_REST;

    mHost->RemoveCallbacks(this);
    mHost->RemoveCallbacks(mCheckFlywheel);

    mHost->ReportScrollStateChange(IAbsListViewOnScrollListener::SCROLL_STATE_IDLE);
    mHost->ClearScrollingCache();

    mScroller->AbortAnimation();

    if (mHost->mFlingStrictSpan != NULL) {
        assert(0 && "TODO");
//        mHost->mFlingStrictSpan->Finish();
        mHost->mFlingStrictSpan = NULL;
    }
}


void AbsListView::FlingRunnable::FlywheelTouch()
{
    mHost->PostDelayed(mCheckFlywheel, FLYWHEEL_TIMEOUT);
}

ECode AbsListView::FlingRunnable::Run()
{
    switch(mHost->mTouchMode)
    {
        default:
            EndFling();
            return NOERROR;
        case AbsListView::TOUCH_MODE_SCROLL:
            if(mScroller->IsFinished())
            {
                return NOERROR;
            }
        case AbsListView::TOUCH_MODE_FLING:
        {
            if(mHost->mDataChanged)
            {
                mHost->LayoutChildren();
            }

            if(mHost->mItemCount == 0 || mHost->GetChildCount() == 0)
            {
                EndFling();
                return NOERROR;
            }

            Boolean more = mScroller->ComputeScrollOffset();
            Int32 y = mScroller->GetCurrY();
            // Flip sign to convert finger direction to list items direction
            // (e.g. finger moving down means list is moving towards the top)
            Int32 delta = mLastFlingY - y;

            // Pretend that each frame of a fling scroll is a touch scroll
            if (delta > 0) {
                // List is moving towards the top. Use first view as mMotionPosition
                mHost->mMotionPosition = mHost->mFirstPosition;
                AutoPtr<IView> firstView = mHost->GetChildAt(0);
                firstView->GetTop(&(mHost->mMotionViewOriginalTop));

                // Don't fling more than 1 screen
                delta = Elastos::Core::Math::Min(mHost->GetHeight() - mHost->mPaddingBottom - mHost->mPaddingTop - 1, delta);
            } else {
                // List is moving towards the bottom. Use last view as mMotionPosition
                Int32 offsetToLast = mHost->GetChildCount() - 1;
                mHost->mMotionPosition = mHost->mFirstPosition + offsetToLast;

                AutoPtr<IView> lastView = mHost->GetChildAt(offsetToLast);
                lastView->GetTop(&(mHost->mMotionViewOriginalTop));

                // Don't fling more than 1 screen
                delta = Elastos::Core::Math::Max(-(mHost->GetHeight() - mHost->mPaddingBottom - mHost->mPaddingTop - 1), delta);
            }

            // Check to see if we have bumped into the scroll limit
            AutoPtr<IView> motionView = mHost->GetChildAt(mHost->mMotionPosition - mHost->mFirstPosition);
            Int32 oldTop = 0;
            if (motionView != NULL) {
                motionView->GetTop(&oldTop);
            }

            // Don't stop just because delta is zero (it could have been rounded)
            Boolean atEdge = mHost->TrackMotionScroll(delta, delta);
            Boolean atEnd = atEdge && (delta != 0);
            if (atEnd) {
                if (motionView != NULL) {
                    // Tweak the scroll for how far we overshot
                    Int32 top;
                    motionView->GetTop(&top);
                    Int32 overshoot = -(delta - (top - oldTop));
                    mHost->OverScrollBy(0, overshoot, 0, mHost->mScrollY, 0, 0,
                            0, mHost->mOverflingDistance, FALSE);
                }
                if (more) {
                    EdgeReached(delta);
                }
                break;
            }

            if (more && !atEnd) {
                if (atEdge) mHost->Invalidate();
                mLastFlingY = y;
                mHost->PostOnAnimation(this);
            } else {
                EndFling();

                if (AbsListView::PROFILE_FLINGING) {
                    if (mHost->mFlingProfilingStarted) {
    //                    Debug.stopMethodTracing();
                        mHost->mFlingProfilingStarted = FALSE;
                    }

                    if (mHost->mFlingStrictSpan != NULL) {
                        assert(0 && "TODO");
//                        mHost->mFlingStrictSpan->Finish();
                        mHost->mFlingStrictSpan = NULL;
                    }
                }
            }
            break;
        }

        case TOUCH_MODE_OVERFLING:
        {
            if (mScroller->ComputeScrollOffset()) {
                Int32 scrollY = mHost->mScrollY;
                Int32 currY = mScroller->GetCurrY();
                Int32 deltaY = currY - scrollY;
                if (mHost->OverScrollBy(0, deltaY, 0, scrollY, 0, 0,
                        0, mHost->mOverflingDistance, FALSE)) {
                    Boolean crossDown = scrollY <= 0 && currY > 0;
                    Boolean crossUp = scrollY >= 0 && currY < 0;
                    if (crossDown || crossUp) {
                        Int32 velocity = (Int32) mScroller->GetCurrVelocity();
                        if (crossUp)
                            velocity = -velocity;

                        // Don't flywheel from this; we're just continuing things.
                        mScroller->AbortAnimation();
                        Start(velocity);
                    } else {
                        StartSpringback();
                    }
                } else {
                    mHost->Invalidate();
                    mHost->PostOnAnimation(this);
                }
            } else {
                EndFling();
            }
            break;
        }
    }

    return NOERROR;
}

//==============================================================================
//          AbsListView::PositionScrollerStartRunnable
//==============================================================================
AbsListView::PositionScrollerStartRunnable::PositionScrollerStartRunnable(
    /* [in] */ Int32 paramsCount,
    /* [in] */ Int32 param1,
    /* [in] */ Int32 param2,
    /* [in] */ Int32 param3,
    /* [in] */ PositionScroller* host)
    : mParamsCount(paramsCount)
    , mParam1(param1), mParam2(param1)
    , mParam3(param1), mHost(host)
{
}

ECode AbsListView::PositionScrollerStartRunnable::Run()
{
    switch(mParamsCount)
    {
    case 1:
        mHost->Start(mParam1);
        break;
    case 2:
        mHost->Start(mParam1, mParam2);
        break;
    case 3:
        mHost->StartWithOffset(mParam1, mParam2, mParam3);
        break;
    default:
        break;
    }
    return NOERROR;
}

//==============================================================================
//          AbsListView::PositionScroller
//==============================================================================

AbsListView::PositionScroller::PositionScroller(
    /* [in] */ AbsListView* host)
    : mHost(host)
    , mMode(0)
    , mTargetPos(0)
    , mBoundPos(0)
    , mLastSeenPos(0)
    , mScrollDuration(0)
    , mOffsetFromTop(0)
{
    AutoPtr<IViewConfigurationHelper> configHelper;
    CViewConfigurationHelper::AcquireSingleton((IViewConfigurationHelper**)&configHelper);

    AutoPtr<IViewConfiguration> configuration;
    configHelper->Get(mHost->mContext, (IViewConfiguration**)&configuration);

    configuration->GetScaledFadingEdgeLength(&mExtraScroll);
}

void AbsListView::PositionScroller::Start(
    /* [in] */ const Int32 position)
{
    Stop();

    if (mHost->mDataChanged) {
        // Wait until we're back in a stable state to try this.
        mHost->mPositionScrollAfterLayout =
                new PositionScrollerStartRunnable(1, position, 0, 0, this);
        return;
    }

    Int32 childCount = mHost->GetChildCount();
    if (childCount == 0) {
        // Can't scroll without children.
        return;
    }

    Int32 firstPos = mHost->mFirstPosition;
    Int32 lastPos = firstPos + childCount - 1;

    Int32 viewTravelCount;

    int clampedPosition = Elastos::Core::Math::Max(0, Elastos::Core::Math::Min(mHost->GetCount() - 1, position));

    if (clampedPosition < firstPos) {
        viewTravelCount = firstPos - clampedPosition + 1;
        mMode = MOVE_UP_POS;
    }
    else if (clampedPosition >= lastPos) {
        viewTravelCount = clampedPosition - lastPos + 1;
        mMode = MOVE_DOWN_POS;
    }
    else {
        ScrollToVisible(clampedPosition, IAbsListView::INVALID_POSITION, SCROLL_DURATION);
        return;
    }

    if (viewTravelCount > 0) {
        mScrollDuration = SCROLL_DURATION / viewTravelCount;
    }
    else {
        mScrollDuration = SCROLL_DURATION;
    }
    mTargetPos = clampedPosition;
    mBoundPos = IAdapterView::INVALID_POSITION;
    mLastSeenPos = IAdapterView::INVALID_POSITION;

    mHost->PostOnAnimation(this);
}

void AbsListView::PositionScroller::Start(
    /* [in] */ const Int32 position,
    /* [in] */ const Int32 boundPosition)
{
    Stop();

    if (boundPosition == IAdapterView::INVALID_POSITION) {
        Start(position);
        return;
    }

    if (mHost->mDataChanged) {
        // Wait until we're back in a stable state to try this.
        mHost->mPositionScrollAfterLayout =
                new PositionScrollerStartRunnable(2, position, boundPosition, 0, this);
        return;
    }

    Int32 childCount = mHost->GetChildCount();
    if (childCount == 0) {
        // Can't scroll without children.
        return;
    }

    Int32 firstPos = mHost->mFirstPosition;
    Int32 lastPos = firstPos + childCount - 1;

    Int32 viewTravelCount = 0;
    Int32 clampedPosition = Elastos::Core::Math::Max(0, Elastos::Core::Math::Min(mHost->GetCount() - 1, position));

    if (clampedPosition < firstPos) {
        Int32 boundPosFromLast = lastPos - boundPosition;
        if (boundPosFromLast < 1) {
            // Moving would shift our bound position off the screen. Abort.
            return;
        }

        Int32 posTravel = firstPos - clampedPosition + 1;
        Int32 boundTravel = boundPosFromLast - 1;
        if (boundTravel < posTravel) {
            viewTravelCount = boundTravel;
            mMode = MOVE_UP_BOUND;
        }
        else {
            viewTravelCount = posTravel;
            mMode = MOVE_UP_POS;
        }
    }
    else if (clampedPosition > lastPos) {
        Int32 boundPosFromFirst = boundPosition - firstPos;
        if (boundPosFromFirst < 1) {
            // Moving would shift our bound position off the screen. Abort.
            return;
        }

        Int32 posTravel = clampedPosition - lastPos + 1;
        Int32 boundTravel = boundPosFromFirst - 1;
        if (boundTravel < posTravel) {
            viewTravelCount = boundTravel;
            mMode = MOVE_DOWN_BOUND;
        }
        else {
            viewTravelCount = posTravel;
            mMode = MOVE_DOWN_POS;
        }
    }
    else {
        ScrollToVisible(clampedPosition, boundPosition, SCROLL_DURATION);
        return;
    }

    if (viewTravelCount > 0) {
        mScrollDuration = SCROLL_DURATION / viewTravelCount;
    }
    else {
        mScrollDuration = SCROLL_DURATION;
    }
    mTargetPos = clampedPosition;
    mBoundPos = boundPosition;
    mLastSeenPos = IAdapterView::INVALID_POSITION;

    mHost->PostOnAnimation(this);
}

void AbsListView::PositionScroller::StartWithOffset(
    /* [in] */ Int32 position,
    /* [in] */ Int32 offset)
{
    StartWithOffset(position, offset, SCROLL_DURATION);
}

void AbsListView::PositionScroller::StartWithOffset(
    /* [in] */ Int32 position,
    /* [in] */ Int32 offset,
    /* [in] */ const Int32 duration)
{
    Stop();

    if (mHost->mDataChanged) {
        // Wait until we're back in a stable state to try this.
        mHost->mPositionScrollAfterLayout =
                new PositionScrollerStartRunnable(3, position, offset, duration, this);
        return;
    }

    Int32 childCount = mHost->GetChildCount();
    if (childCount == 0) {
        // Can't scroll without children.
        return;
    }

    offset += mHost->GetPaddingTop();

    mTargetPos = Elastos::Core::Math::Max(0, Elastos::Core::Math::Min(mHost->GetCount() - 1, position));
    mOffsetFromTop = offset;
    mBoundPos = IAbsListView::INVALID_POSITION;
    mLastSeenPos = IAbsListView::INVALID_POSITION;
    mMode = MOVE_OFFSET;

    Int32 firstPos = mHost->mFirstPosition;
    Int32 lastPos = firstPos + childCount - 1;

    Int32 viewTravelCount;
    if (mTargetPos < firstPos) {
        viewTravelCount = firstPos - mTargetPos;
    } else if (mTargetPos > lastPos) {
        viewTravelCount = mTargetPos - lastPos;
    } else {
        // On-screen, just scroll.
        Int32 targetTop ;
        mHost->GetChildAt(mTargetPos - firstPos)->GetTop(&targetTop);
        mHost->SmoothScrollBy(targetTop - offset, duration, true);
        return;
    }

    // Estimate how many screens we should travel
    Float screenTravelCount = (Float) viewTravelCount / childCount;
    mScrollDuration = screenTravelCount < 1 ?
            duration : (Int32) (duration / screenTravelCount);
    mLastSeenPos = IAbsListView::INVALID_POSITION;

    mHost->PostOnAnimation(this);
}

/**
 * Scroll such that targetPos is in the visible padded region without scrolling
 * boundPos out of view. Assumes targetPos is onscreen.
 */
void AbsListView::PositionScroller::ScrollToVisible(
    /* [in] */ Int32 targetPos,
    /* [in] */ Int32 boundPos,
    /* [in] */ Int32 duration)
{
    Int32 firstPos = mHost->mFirstPosition;
    Int32 childCount = mHost->GetChildCount();
    Int32 lastPos = firstPos + childCount - 1;
    Int32 paddedTop = mHost->mListPadding->mTop;
    Int32 paddedBottom = mHost->GetHeight() - mHost->mListPadding->mBottom;

    if (targetPos < firstPos || targetPos > lastPos) {
        Logger::W("AbsListView", "scrollToVisible called with targetPos %d  not visible [%d, %d].",
            targetPos, firstPos, lastPos);
    }
    if (boundPos < firstPos || boundPos > lastPos) {
        // boundPos doesn't matter, it's already offscreen.
        boundPos = IAbsListView::INVALID_POSITION;
    }

    AutoPtr<IView> targetChild = mHost->GetChildAt(targetPos - firstPos);
    Int32 targetTop;
    targetChild->GetTop(&targetTop);
    Int32 targetBottom;
    targetChild->GetBottom(&targetBottom);
    Int32 scrollBy = 0;

    if (targetBottom > paddedBottom) {
        scrollBy = targetBottom - paddedBottom;
    }
    if (targetTop < paddedTop) {
        scrollBy = targetTop - paddedTop;
    }

    if (scrollBy == 0) {
        return;
    }

    if (boundPos >= 0) {
        AutoPtr<IView> boundChild = mHost->GetChildAt(boundPos - firstPos);
        Int32 boundTop;
        boundChild->GetTop(&boundTop);
        Int32 boundBottom ;
        boundChild->GetBottom(&boundBottom);
        Int32 absScroll = Elastos::Core::Math::Abs(scrollBy);

        if (scrollBy < 0 && boundBottom + absScroll > paddedBottom) {
            // Don't scroll the bound view off the bottom of the screen.
            scrollBy = Elastos::Core::Math::Max(0, boundBottom - paddedBottom);
        } else if (scrollBy > 0 && boundTop - absScroll < paddedTop) {
            // Don't scroll the bound view off the top of the screen.
            scrollBy = Elastos::Core::Math::Min(0, boundTop - paddedTop);
        }
    }

    mHost->SmoothScrollBy(scrollBy, duration);
}

void AbsListView::PositionScroller::Stop()
{
    mHost->RemoveCallbacks(this);
}

ECode AbsListView::PositionScroller::Run()
{
    Int32 listHeight = mHost->GetHeight();
    Int32 firstPos = mHost->mFirstPosition;

    switch (mMode) {
    case MOVE_DOWN_POS:
        {
            Int32 lastViewIndex = mHost->GetChildCount() - 1;
            Int32 lastPos = firstPos + lastViewIndex;

            if (lastViewIndex < 0) {
                return NOERROR;
            }

            if (lastPos == mLastSeenPos) {
                // No new views, let things keep going.
                mHost->PostOnAnimation(this);
                return NOERROR;
            }

            AutoPtr<IView> lastView = mHost->GetChildAt(lastViewIndex);
            Int32 lastViewHeight;
            lastView->GetHeight(&lastViewHeight);
            Int32 lastViewTop;
            lastView->GetTop(&lastViewTop);
            Int32 lastViewPixelsShowing = listHeight - lastViewTop;
            Int32 extraScroll = lastPos < mHost->mItemCount - 1
                ? Elastos::Core::Math::Max(mHost->mListPadding->mBottom, mExtraScroll) : mHost->mListPadding->mBottom;

            mHost->SmoothScrollBy(
                lastViewHeight - lastViewPixelsShowing + extraScroll,
                mScrollDuration);
            Int32 scrollBy = lastViewHeight - lastViewPixelsShowing + extraScroll;
            mHost->SmoothScrollBy(scrollBy, mScrollDuration, true);

            mLastSeenPos = lastPos;
            if (lastPos < mTargetPos) {
                mHost->PostOnAnimation(this);
            }
        }
        break;
    case MOVE_DOWN_BOUND:
        {
            Int32 nextViewIndex = 1;
            Int32 childCount = mHost->GetChildCount();

            if (firstPos == mBoundPos || childCount <= nextViewIndex
                || firstPos + childCount >= mHost->mItemCount) {
                return NOERROR;
            }

            Int32 nextPos = firstPos + nextViewIndex;
            if (nextPos == mLastSeenPos) {
                // No new views, let things keep going.
                mHost->PostOnAnimation(this);
                return NOERROR;
            }

            AutoPtr<IView> nextView = mHost->GetChildAt(nextViewIndex);
            Int32 nextViewHeight;
            nextView->GetHeight(&nextViewHeight);
            Int32 nextViewTop;
            nextView->GetTop(&nextViewTop);
            Int32 extraScroll = Elastos::Core::Math::Max(mHost->mListPadding->mBottom, mExtraScroll);
            if (nextPos < mBoundPos) {
                mHost->SmoothScrollBy(
                    Elastos::Core::Math::Max(0, nextViewHeight + nextViewTop - extraScroll),
                    mScrollDuration, TRUE);
                mLastSeenPos = nextPos;
                mHost->PostOnAnimation(this);
            }
            else  {
                if (nextViewTop > extraScroll) {
                    mHost->SmoothScrollBy(nextViewTop - extraScroll, mScrollDuration, TRUE);
                }
            }
        }
        break;
    case MOVE_UP_POS:
        {
            if (firstPos == mLastSeenPos) {
                // No new views, let things keep going.
                mHost->PostOnAnimation(this);
                return NOERROR;
            }

            AutoPtr<IView> firstView = mHost->GetChildAt(0);
            if (firstView == NULL) {
                return NOERROR;
            }
            Int32 firstViewTop;
            firstView->GetTop(&firstViewTop);
            Int32 extraScroll = firstPos > 0
                ? Elastos::Core::Math::Max(mExtraScroll, mHost->mListPadding->mTop) : mHost->mListPadding->mTop;

            mHost->SmoothScrollBy(
                firstViewTop - extraScroll, mScrollDuration, TRUE);

            mLastSeenPos = firstPos;

            if (firstPos > mTargetPos) {
                mHost->PostOnAnimation(this);
            }
        }
        break;
    case MOVE_UP_BOUND:
        {
            Int32 lastViewIndex = mHost->GetChildCount() - 2;
            if (lastViewIndex < 0) {
                return NOERROR;
            }

            Int32 lastPos = firstPos + lastViewIndex;
            if (lastPos == mLastSeenPos) {
                // No new views, let things keep going.
                mHost->PostOnAnimation(this);
                return NOERROR;
            }

            AutoPtr<IView> lastView = mHost->GetChildAt(lastViewIndex);
            Int32 lastViewHeight;
            lastView->GetHeight(&lastViewHeight);
            Int32 lastViewTop;
            lastView->GetTop(&lastViewTop);
            Int32 lastViewPixelsShowing = listHeight - lastViewTop;
            Int32 extraScroll = Elastos::Core::Math::Max(mHost->mListPadding->mTop, mExtraScroll);
            mLastSeenPos = lastPos;
            if (lastPos > mBoundPos) {
                mHost->SmoothScrollBy(
                    -(lastViewPixelsShowing - extraScroll),
                    mScrollDuration, TRUE);
                mHost->PostOnAnimation(this);
            }
            else {
                Int32 bottom = listHeight - mExtraScroll;
                Int32 lastViewBottom = lastViewTop + lastViewHeight;
                if (bottom > lastViewBottom) {
                    mHost->SmoothScrollBy(
                        -(bottom - lastViewBottom), mScrollDuration, TRUE);
                }
            }
        }
        break;

    case MOVE_OFFSET:
    {
        if (mLastSeenPos == firstPos) {
            // No new views, let things keep going.
            mHost->PostOnAnimation(this);
            return NOERROR;
        }

        mLastSeenPos = firstPos;

        Int32 childCount = mHost->GetChildCount();
        Int32 position = mTargetPos;
        Int32 lastPos = firstPos + childCount - 1;

        Int32 viewTravelCount = 0;
        if (position < firstPos) {
            viewTravelCount = firstPos - position + 1;
        } else if (position > lastPos) {
            viewTravelCount = position - lastPos;
        }

        // Estimate how many screens we should travel
        Float screenTravelCount = (Float) viewTravelCount / childCount;

        Float modifier = Elastos::Core::Math::Min(Elastos::Core::Math::Abs(screenTravelCount), 1.0f);
        if (position < firstPos) {
            Int32 distance = (Int32) (-mHost->GetHeight() * modifier);
            Int32 duration = (Int32) (mScrollDuration * modifier);
            mHost->SmoothScrollBy(distance, duration, TRUE);
            mHost->PostOnAnimation(this);
        } else if (position > lastPos) {
            Int32 distance = (Int32) (mHost->GetHeight() * modifier);
            Int32 duration = (Int32) (mScrollDuration * modifier);
            mHost->SmoothScrollBy(distance, duration, TRUE);
            mHost->PostOnAnimation(this);
        } else {
            // On-screen, just scroll.
            Int32 targetTop;
            mHost->GetChildAt(position - firstPos)->GetTop(&targetTop);
            Int32 distance = targetTop - mOffsetFromTop;
            Int32 duration = (Int32) (mScrollDuration *
                ((Float) Elastos::Core::Math::Abs(distance) / mHost->GetHeight()));
            mHost->SmoothScrollBy(distance, duration, TRUE);
        }
        break;
    }

    default:
        break;
    }

    return NOERROR;
}

//==============================================================================
//          AbsListView::AdapterDataSetObserver
//==============================================================================

AbsListView::AdapterDataSetObserver::AdapterDataSetObserver(
    /* [in] */ AbsListView* host)
    : AdapterView::AdapterDataSetObserver(host)
    , mOwner(host)
{
}

ECode AbsListView::AdapterDataSetObserver::OnChanged()
{
    FAIL_RETURN(AdapterView::AdapterDataSetObserver::OnChanged());
    if (mOwner->mFastScroller != NULL) {
        mOwner->mFastScroller->OnSectionsChanged();
    }
    return NOERROR;
}

ECode AbsListView::AdapterDataSetObserver::OnInvalidated()
{
    FAIL_RETURN(AdapterView::AdapterDataSetObserver::OnInvalidated());
    if (mOwner->mFastScroller != NULL) {
        mOwner->mFastScroller->OnSectionsChanged();
    }
    return NOERROR;
}

//==============================================================================
//          AbsListView::MultiChoiceModeWrapper
//==============================================================================
AbsListView::MultiChoiceModeWrapper::MultiChoiceModeWrapper(
    /* [in] */ AbsListView* host) : mHost(host)
{}


ECode AbsListView::MultiChoiceModeWrapper::SetWrapped(
    /* [in] */ IMultiChoiceModeListener* wrapped)
{
    mWrapped = wrapped;
    return NOERROR;
}

ECode AbsListView::MultiChoiceModeWrapper::HasWrappedCallback(
    /* [out] */ Boolean* result)
{
    *result = mWrapped != NULL;
    return NOERROR;
}

ECode AbsListView::MultiChoiceModeWrapper::OnCreateActionMode(
    /* [in] */ IActionMode* mode,
    /* [in] */ IMenu* menu,
    /* [out] */ Boolean* result)
{
    Boolean superResult;
    mWrapped->OnCreateActionMode(mode, menu, &superResult);
    if (superResult)
    {
        // Initialize checked graphic state?
        mHost->SetLongClickable(FALSE);
        *result = TRUE;
        return NOERROR;
    }
    *result = FALSE;
    return NOERROR;
}

ECode AbsListView::MultiChoiceModeWrapper::OnPrepareActionMode(
    /* [in] */ IActionMode* mode,
    /* [in] */ IMenu* menu,
    /* [out] */ Boolean* result)
{
    return mWrapped->OnPrepareActionMode(mode, menu, result);
}

ECode AbsListView::MultiChoiceModeWrapper::OnActionItemClicked(
    /* [in] */ IActionMode* mode,
    /* [in] */ IMenuItem* item,
    /* [out] */ Boolean* result)
{
    return mWrapped->OnActionItemClicked(mode, item, result);
}

ECode AbsListView::MultiChoiceModeWrapper::OnDestroyActionMode(
    /* [in] */ IActionMode* mode)
{
    mWrapped->OnDestroyActionMode(mode);
    mHost->mChoiceActionMode = NULL;

    // Ending selection mode means deselecting everything.
    mHost->ClearChoices();

    mHost->mDataChanged = TRUE;
    mHost->RememberSyncState();
    mHost->RequestLayout();

    mHost->SetLongClickable(TRUE);
    return NOERROR;
}

ECode AbsListView::MultiChoiceModeWrapper::OnItemCheckedStateChanged(
    /* [in] */ IActionMode* mode,
    /* [in] */ Int32 position,
    /* [in] */ Int64 id,
    /* [in] */ Boolean checked)
{
    mWrapped->OnItemCheckedStateChanged(mode, position, id, checked);

    // If there are no items selected we no longer need the selection mode.
    if (mHost->GetCheckedItemCount() == 0) {
        mode->Finish();
    }
    return NOERROR;
}

//==============================================================================
//          AbsListView::RecycleBin
//==============================================================================

AbsListView::RecycleBin::RecycleBin(
    /* [in] */ AbsListView* host)
    : mFirstActivePosition(0)
    , mViewTypeCount(0)
    , mHost(host)
{
    mActiveViews = ArrayOf<IView*>::Alloc(0);
}

AbsListView::RecycleBin::~RecycleBin()
{
    mScrapViews = NULL;
    mCurrentScrap = NULL;
    mSkippedScrap = NULL;

    mTransientStateViews = NULL;
}

ECode AbsListView::RecycleBin::SetViewTypeCount(
    /* [in] */ Int32 viewTypeCount)
{
    if (viewTypeCount < 1) {
        Logger::E("AbsListView::RecycleBin", "Can't have a viewTypeCount < 1");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    //noinspection unchecked
    AutoPtr<ArrayOf<ViewList* > > scrapViews = ArrayOf<ViewList* >::Alloc(viewTypeCount);
    for (Int32 i = 0; i < viewTypeCount; i++) {
        AutoPtr<ViewList> list = new ViewList();
        scrapViews->Set(i, list);
    }

    mScrapViews = scrapViews;
    mViewTypeCount = viewTypeCount;
    mCurrentScrap = (*mScrapViews)[0];

    return NOERROR;
}

void AbsListView::RecycleBin::MarkChildrenDirty()
{
    if (mViewTypeCount == 1) {
        ViewListIterator iter = mCurrentScrap->Begin();
        for (; iter != mCurrentScrap->End(); ++iter) {
            (*iter)->ForceLayout();
        }
    }
    else {
        for (Int32 i = 0; i < mViewTypeCount; i++) {
            AutoPtr<ViewList> scrap = (*mScrapViews)[i];
            ViewListIterator iter = scrap->Begin();
            for (; iter != scrap->End(); ++iter) {
                (*iter)->ForceLayout();
            }
        }
    }
    if (mTransientStateViews != NULL) {
        ViewMapIterator it = mTransientStateViews->Begin();
        for (; it != mTransientStateViews->End(); it++) {
            it->mSecond->ForceLayout();
        }
    }
}

Boolean AbsListView::RecycleBin::ShouldRecycleViewType(
    /* [in] */ Int32 viewType)
{
    return viewType >= 0;
}

/**
 * Clears the scrap heap.
 */
void AbsListView::RecycleBin::Clear()
{
    if (mViewTypeCount == 1) {
        ViewListReverseIterator rit = mCurrentScrap->RBegin();
        for (; rit != mCurrentScrap->REnd(); ++rit) {
            mHost->RemoveDetachedView((*rit), FALSE);
        }
        mCurrentScrap->Clear();
    }
    else {
        for (Int32 i = 0; i < mViewTypeCount; i++) {
            AutoPtr<ViewList> scrap = (*mScrapViews)[i];
            ViewListReverseIterator rit = scrap->RBegin();
            for (; rit != scrap->REnd(); ++rit) {
                mHost->RemoveDetachedView((*rit), FALSE);
            }
            scrap->Clear();
        }
    }
    if (mTransientStateViews != NULL) {
        mTransientStateViews->Clear();
    }
}

/**
 * Fill ActiveViews with all of the children of the AbsListView.
 *
 * @param childCount The minimum number of views mActiveViews should hold
 * @param firstActivePosition The position of the first view that will be stored in
 *        mActiveViews
 */
void AbsListView::RecycleBin::FillActiveViews(
    /* [in] */ Int32 childCount,
    /* [in] */ Int32 firstActivePosition)
{
    if (mActiveViews->GetLength() < childCount) {
        mActiveViews = ArrayOf<IView*>::Alloc(childCount);
    }

    mFirstActivePosition = firstActivePosition;

    for (Int32 i = 0; i < childCount; i++) {
        AutoPtr<IView> child = mHost->GetChildAt(i);
        AutoPtr<IAbsListViewLayoutParams> lp;
        child->GetLayoutParams((IViewGroupLayoutParams**)&lp);
        // Don't put header or footer views into the scrap heap

        if(lp != NULL)
        {
            Int32 viewType;
            lp->GetViewType(&viewType);
            if(viewType != IAdapterView::ITEM_VIEW_TYPE_HEADER_OR_FOOTER) {
                // Note:  We do place AdapterView.ITEM_VIEW_TYPE_IGNORE in active views.
                //        However, we will NOT place them into scrap views.
                mActiveViews->Set(i, child);
            }
        }
    }
}

/**
 * Get the view corresponding to the specified position. The view will be removed from
 * mActiveViews if it is found.
 *
 * @param position The position to look up in mActiveViews
 * @return The view if it is found, NULL otherwise
 */
AutoPtr<IView> AbsListView::RecycleBin::GetActiveView(
    /* [in] */ Int32 position)
{
    Int32 index = position - mFirstActivePosition;
    if (index >=0 && index < mActiveViews->GetLength()) {
        AutoPtr<IView> match = (*mActiveViews)[index];
        mActiveViews->Set(index, NULL);
        return match;
    }

    return NULL;
}

AutoPtr<IView> AbsListView::RecycleBin::GetTransientStateView(
    /* [in] */ Int32 position)
{
    if (mTransientStateViews == NULL) {
        return NULL;
    }
    ViewMapIterator it = mTransientStateViews->Find(position);
    AutoPtr<IView> result;
    if (it != mTransientStateViews->End()) {
        result = it->mSecond;
        mTransientStateViews->Erase(it);
    }

    return result;
}

/**
 * Dump any currently saved views with transient state.
 */
void AbsListView::RecycleBin::ClearTransientStateViews()
{
    if (mTransientStateViews != NULL) {
        mTransientStateViews->Clear();
    }
}

/**
 * @return A view from the ScrapViews collection. These are unordered.
 */
AutoPtr<IView> AbsListView::RecycleBin::GetScrapView(
    /* [in] */ Int32 position)
{
    if (mViewTypeCount == 1) {
        return AbsListView::RetrieveFromScrap(mCurrentScrap, position);
    }
    else {
        Int32 whichScrap;
        mHost->mAdapter->GetItemViewType(position, &whichScrap);
        if (whichScrap >= 0 && whichScrap < mScrapViews->GetLength()) {
            return AbsListView::RetrieveFromScrap((*mScrapViews)[whichScrap], position);
        }
    }
    return NULL;
}

/**
 * Put a view into the ScapViews list. These views are unordered.
 *
 * @param scrap The view to add
 */
void AbsListView::RecycleBin::AddScrapView(
    /* [in ]*/ IView* scrap,
    /* [in] */ Int32 position)
{
    AutoPtr<IAbsListViewLayoutParams> lp;
    scrap->GetLayoutParams((IViewGroupLayoutParams**)&lp);
    if (lp == NULL) {
        return;
    }
    lp->SetScrappedFromPosition(position);
    // Don't put header or footer views or views that should be ignored
    // into the scrap heap

    Int32 viewType = 0;
    lp->GetViewType(&viewType);
    Boolean scrapHasTransientState;
    scrap->HasTransientState(&scrapHasTransientState);

    if (!ShouldRecycleViewType(viewType) || scrapHasTransientState) {
        if (viewType != IAbsListView::ITEM_VIEW_TYPE_HEADER_OR_FOOTER || scrapHasTransientState) {
            if (mSkippedScrap == NULL) {
                mSkippedScrap = new ViewList();
            }
            mSkippedScrap->PushBack(scrap);
        }
        if (scrapHasTransientState) {
            if (mTransientStateViews == NULL) {
                mTransientStateViews = new ViewMap();
            }
            scrap->DispatchStartTemporaryDetach();
            (*mTransientStateViews)[position] = scrap;
        }
        return;
    }

    scrap->DispatchStartTemporaryDetach();
    if (mViewTypeCount == 1) {
        mCurrentScrap->PushBack(scrap);
    }
    else {
        (*mScrapViews)[viewType]->PushBack(scrap);
    }

    scrap->SetAccessibilityDelegate(NULL);

    if (mRecyclerListener != NULL) {
        mRecyclerListener->OnMovedToScrapHeap(scrap);
    }
}

/**
 * Finish the removal of any views that skipped the scrap heap.
 */
void AbsListView::RecycleBin::RemoveSkippedScrap()
{
    if (mSkippedScrap == NULL) {
        return;
    }

    ViewListIterator it = mSkippedScrap->Begin();
    for (; it != mSkippedScrap->End(); ++it) {
        mHost->RemoveDetachedView(*it, FALSE);
    }
    mSkippedScrap->Clear();
}

/**
 * Move all views remaining in mActiveViews to mScrapViews.
 */
void AbsListView::RecycleBin::ScrapActiveViews()
{
    Boolean hasListener = mRecyclerListener != NULL;
    Boolean multipleScraps = mViewTypeCount > 1;

    Int32 count = mActiveViews->GetLength();
    for (Int32 i = count - 1; i >= 0; i--) {
        AutoPtr<IView> victim = (*mActiveViews)[i];
        if (victim != NULL) {
            AutoPtr<IAbsListViewLayoutParams> lp;
            victim->GetLayoutParams((IViewGroupLayoutParams**)&lp);
            Int32 whichScrap = 0;
            lp->GetViewType(&whichScrap);

            mActiveViews->Set(i, NULL);

            Boolean scrapHasTransientState;
            victim->HasTransientState(&scrapHasTransientState);
            if (!ShouldRecycleViewType(whichScrap) || scrapHasTransientState) {
                // Do not move views that should be ignored
                if (whichScrap != IAbsListView::ITEM_VIEW_TYPE_HEADER_OR_FOOTER
                    || scrapHasTransientState)
                {
                    mHost->RemoveDetachedView(victim, FALSE);
                }

                if (scrapHasTransientState) {
                    if (mTransientStateViews == NULL) {
                        mTransientStateViews = new ViewMap();
                    }
                    (*mTransientStateViews)[mFirstActivePosition + i] = victim;
                }
                continue;
            }

            if (multipleScraps) {
                mCurrentScrap = (*mScrapViews)[whichScrap];
            }
            victim->DispatchStartTemporaryDetach();
            lp->SetScrappedFromPosition(mFirstActivePosition + i);
            mCurrentScrap->PushBack(victim);

            victim->SetAccessibilityDelegate(NULL);

            if (hasListener) {
                mRecyclerListener->OnMovedToScrapHeap(victim);
            }
        }
    }
    PruneScrapViews();
}

/**
 * Makes sure that the size of mScrapViews does not exceed the size of mActiveViews.
 * (This can happen if an adapter does not recycle its views).
 */
void AbsListView::RecycleBin::PruneScrapViews()
{
    Int32 maxViews = mActiveViews->GetLength();
    Int32 viewTypeCount = mViewTypeCount;
    for (Int32 i = 0; i < viewTypeCount; ++i) {
        AutoPtr<ViewList> scrapPile = (*mScrapViews)[i];
        Int32 extras = scrapPile->GetSize() - maxViews;
        ViewListReverseIterator rit = scrapPile->RBegin();
        AutoPtr<IView> view;
        for (Int32 j = 0; j < extras && rit != scrapPile->REnd(); ++j) {
            view = *rit;
            mHost->RemoveDetachedView(view, FALSE);
            rit = ViewListReverseIterator(scrapPile->Erase(--(rit.GetBase())));
        }
    }

    if (mTransientStateViews != NULL) {
        AutoPtr<IView> view;
        Boolean hasTransientState;
        ViewMapIterator it = mTransientStateViews->Begin();
        for (; it != mTransientStateViews->End();) {
            view = it->mSecond;
            view->HasTransientState(&hasTransientState);
            if (!hasTransientState) {
                mTransientStateViews->Erase(it++);
            }
            else {
                ++it;
            }
        }
    }
}

/**
 * Puts all views in the scrap heap into the supplied list.
 */
void AbsListView::RecycleBin::ReclaimScrapViews(
    /* [in] */ IObjectContainer* views)
{
    if (mViewTypeCount == 1) {
        ViewListIterator iter = mCurrentScrap->Begin();
        for (; iter != mCurrentScrap->End(); ++iter) {
            views->Add((*iter));
        }
    }
    else {
        for (Int32 i = 0; i < mViewTypeCount; i++) {
            ViewListIterator iter = (*mScrapViews)[i]->Begin();
            for (; iter != (*mScrapViews)[i]->End(); ++iter) {
                views->Add(*iter);
            }
        }
    }
}

/**
 * Updates the cache color hint of all known views.
 *
 * @param color The new cache color hint.
 */
void AbsListView::RecycleBin::SetCacheColorHint(
    /* [in] */ Int32 color)
{
    if (mViewTypeCount == 1) {
        assert(mCurrentScrap != NULL);
        ViewListIterator iter = mCurrentScrap->Begin();
        for (; iter != mCurrentScrap->End(); ++iter) {
            (*iter)->SetDrawingCacheBackgroundColor(color);
        }
    }
    else {
        for (Int32 i = 0; i < mViewTypeCount; i++) {
            ViewListIterator iter = (*mScrapViews)[i]->Begin();
            for (; iter != (*mScrapViews)[i]->End(); ++iter) {
                (*iter)->SetDrawingCacheBackgroundColor(color);
            }
        }
    }

    assert(mActiveViews != NULL);
    // Just in case this is called during a layout pass
    for (Int32 i = 0; i < mActiveViews->GetLength(); ++i) {
        AutoPtr<IView> victim = (*mActiveViews)[i];
        if (victim != NULL) {
            victim->SetDrawingCacheBackgroundColor(color);
        }
    }
}


//==============================================================================
//          AbsListView::ClearScrollingCacheRunnable
//==============================================================================

AbsListView::ClearScrollingCacheRunnable::ClearScrollingCacheRunnable(
    /* [in] */ AbsListView* host)
    : mHost(host)
{}

ECode AbsListView::ClearScrollingCacheRunnable::Run()
{
    if (mHost->mCachingStarted) {
        mHost->mCachingStarted = mHost->mCachingActive = FALSE;
        mHost->SetChildrenDrawnWithCacheEnabled(FALSE);
        if ((mHost->mPersistentDrawingCache
            & IViewGroup::PERSISTENT_SCROLLING_CACHE) == 0) {
                mHost->SetChildrenDrawingCacheEnabled(FALSE);
        }

        if (!mHost->IsAlwaysDrawnWithCacheEnabled()) {
            mHost->Invalidate();
        }
    }

    return NOERROR;
}

//==============================================================================
//          AbsListView::TouchModeResetRunnable
//==============================================================================

AbsListView::TouchModeResetRunnable::TouchModeResetRunnable(
    /* [in] */ IRunnable* r,
    /* [in] */ AbsListView* host,
    /* [in] */ IView* v)
    : mRunnable(r)
    , mHost(host)
    , mView(v)
{
}

ECode AbsListView::TouchModeResetRunnable::Run()
{
    mHost->mTouchMode = AbsListView::TOUCH_MODE_REST;
    mView->SetPressed(FALSE);
    mHost->SetPressed(FALSE);
    if(!mHost->mDataChanged) {
        mRunnable->Run();
    }
    return NOERROR;
}

//==============================================================================
//                  AbsListView
//==============================================================================

static AutoPtr<IInterpolator> InitInterpolator()
{
    //assert(0 && "TODO"); //can't complement without CLinearInterpolator;
    return NULL;
}

const Int32 AbsListView::TOUCH_MODE_REST;
const Int32 AbsListView::TOUCH_MODE_DOWN;
const Int32 AbsListView::TOUCH_MODE_TAP;
const Int32 AbsListView::TOUCH_MODE_DONE_WAITING;
const Int32 AbsListView::TOUCH_MODE_SCROLL;
const Int32 AbsListView::TOUCH_MODE_FLING;
const Int32 AbsListView::TOUCH_MODE_OVERSCROLL;
const Int32 AbsListView::TOUCH_MODE_OVERFLING;
const Int32 AbsListView::LAYOUT_NORMAL;
const Int32 AbsListView::LAYOUT_FORCE_TOP;
const Int32 AbsListView::LAYOUT_SET_SELECTION;
const Int32 AbsListView::LAYOUT_FORCE_BOTTOM;
const Int32 AbsListView::LAYOUT_SPECIFIC;
const Int32 AbsListView::LAYOUT_SYNC;
const Int32 AbsListView::LAYOUT_MOVE_SELECTION;
const Int32 AbsListView::OVERSCROLL_LIMIT_DIVISOR;

const Int32 AbsListView::TOUCH_MODE_UNKNOWN;
const Int32 AbsListView::TOUCH_MODE_ON;
const Int32 AbsListView::TOUCH_MODE_OFF;
const Boolean AbsListView::PROFILE_SCROLLING;
const Boolean AbsListView::PROFILE_FLINGING;
const Int32 AbsListView::INVALID_POINTER;

const Int32 AbsListView::PositionScroller::SCROLL_DURATION;
const Int32 AbsListView::PositionScroller::MOVE_DOWN_POS;
const Int32 AbsListView::PositionScroller::MOVE_UP_POS;
const Int32 AbsListView::PositionScroller::MOVE_DOWN_BOUND;
const Int32 AbsListView::PositionScroller::MOVE_UP_BOUND;
AutoPtr<IInterpolator> AbsListView::sLinearInterpolator = InitInterpolator();

AbsListView::AbsListView()
    : mChoiceMode(IAbsListView::CHOICE_MODE_NONE)
    , mCheckedItemCount(0)
    , mLayoutMode(LAYOUT_NORMAL)
    , mAdapterHasStableIds(FALSE)
    , mDrawSelectorOnTop(FALSE)
    , mSelectorPosition(IAdapterView::INVALID_POSITION)
    , mSelectionLeftPadding(0)
    , mSelectionTopPadding(0)
    , mSelectionRightPadding(0)
    , mSelectionBottomPadding(0)
    , mWidthMeasureSpec(0)
    , mCachingStarted(FALSE)
    , mCachingActive(FALSE)
    , mMotionPosition(0)
    , mMotionViewOriginalTop(0)
    , mMotionViewNewTop(0)
    , mMotionX(0)
    , mMotionY(0)
    , mTouchMode(TOUCH_MODE_REST)
    , mLastY(0)
    , mMotionCorrection(0)
    , mSelectedTop(0)
    , mStackFromBottom(FALSE)
    , mScrollingCacheEnabled(FALSE)
    , mFastScrollEnabled(FALSE)
    , mResurrectToPosition(IAdapterView::INVALID_POSITION)
    , mOverscrollMax(FALSE)
    , mOverscrollDistance(0)
    , mOverflingDistance(0)
    , mIsAttached(FALSE)
    , mDeferNotifyDataSetChanged(FALSE)
    , mSmoothScrollbarEnabled(TRUE)
    , mTextFilterEnabled(FALSE)
    , mFiltered(FALSE)
    , mLastTouchMode(TOUCH_MODE_UNKNOWN)
    , mScrollProfilingStarted(FALSE)
    , mFlingProfilingStarted(FALSE)
    , mTranscriptMode(0)
    , mCacheColorHint(0)
    , mIsChildViewEnabled(FALSE)
    , mLastScrollState(IAbsListViewOnScrollListener::SCROLL_STATE_IDLE)
    , mGlobalLayoutListenerAddedFilter(FALSE)
    , mTouchSlop(0)
    , mDensityScale(0)
    , mMinimumVelocity(0)
    , mMaximumVelocity(0)
    , mVelocityScale(1.0f)
    , mPopupHidden(FALSE)
    , mActivePointerId(INVALID_POINTER)
    , mFirstPositionDistanceGuess(0)
    , mLastPositionDistanceGuess(0)
    , mDirection(0)
    , mForceTranscriptScroll(0)
    , mGlowPaddingLeft(0)
    , mGlowPaddingRight(0)
    , mLastAccessibilityScrollEventFromIndex(0)
    , mLastAccessibilityScrollEventToIndex(0)
    , mLastHandledItemCount(0)
{
    CRect::NewByFriend((CRect**)&mSelectorRect);
    CRect::NewByFriend((CRect**)&mListPadding);
    mRecycler = new RecycleBin(this);
    mIsScrap = ArrayOf<Boolean>::Alloc(1);
}

AbsListView::AbsListView(
    /* [in] */ IContext* context)
    : mChoiceMode(IAbsListView::CHOICE_MODE_NONE)
    , mCheckedItemCount(0)
    , mLayoutMode(LAYOUT_NORMAL)
    , mAdapterHasStableIds(FALSE)
    , mDrawSelectorOnTop(FALSE)
    , mSelectorPosition(IAdapterView::INVALID_POSITION)
    , mSelectionLeftPadding(0)
    , mSelectionTopPadding(0)
    , mSelectionRightPadding(0)
    , mSelectionBottomPadding(0)
    , mWidthMeasureSpec(0)
    , mCachingStarted(FALSE)
    , mCachingActive(FALSE)
    , mMotionPosition(0)
    , mMotionViewOriginalTop(0)
    , mMotionViewNewTop(0)
    , mMotionX(0)
    , mMotionY(0)
    , mTouchMode(TOUCH_MODE_REST)
    , mLastY(0)
    , mMotionCorrection(0)
    , mSelectedTop(0)
    , mStackFromBottom(FALSE)
    , mScrollingCacheEnabled(FALSE)
    , mFastScrollEnabled(FALSE)
    , mResurrectToPosition(IAdapterView::INVALID_POSITION)
    , mOverscrollMax(FALSE)
    , mOverscrollDistance(0)
    , mOverflingDistance(0)
    , mIsAttached(FALSE)
    , mDeferNotifyDataSetChanged(FALSE)
    , mSmoothScrollbarEnabled(TRUE)
    , mTextFilterEnabled(FALSE)
    , mFiltered(FALSE)
    , mLastTouchMode(TOUCH_MODE_UNKNOWN)
    , mScrollProfilingStarted(FALSE)
    , mFlingProfilingStarted(FALSE)
    , mTranscriptMode(0)
    , mCacheColorHint(0)
    , mIsChildViewEnabled(FALSE)
    , mLastScrollState(IAbsListViewOnScrollListener::SCROLL_STATE_IDLE)
    , mGlobalLayoutListenerAddedFilter(FALSE)
    , mTouchSlop(0)
    , mDensityScale(0)
    , mMinimumVelocity(0)
    , mMaximumVelocity(0)
    , mVelocityScale(1.0f)
    , mPopupHidden(FALSE)
    , mActivePointerId(INVALID_POINTER)
    , mFirstPositionDistanceGuess(0)
    , mLastPositionDistanceGuess(0)
    , mDirection(0)
    , mForceTranscriptScroll(0)
    , mGlowPaddingLeft(0)
    , mGlowPaddingRight(0)
    , mLastAccessibilityScrollEventFromIndex(0)
    , mLastAccessibilityScrollEventToIndex(0)
    , mLastHandledItemCount(0)
{
    CRect::NewByFriend((CRect**)&mSelectorRect);
    CRect::NewByFriend((CRect**)&mListPadding);
    mRecycler = new RecycleBin(this);
    mIsScrap = ArrayOf<Boolean>::Alloc(1);
    Init(context);
}

AbsListView::AbsListView(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
    : mChoiceMode(IAbsListView::CHOICE_MODE_NONE)
    , mCheckedItemCount(0)
    , mLayoutMode(LAYOUT_NORMAL)
    , mAdapterHasStableIds(FALSE)
    , mDrawSelectorOnTop(FALSE)
    , mSelectorPosition(IAdapterView::INVALID_POSITION)
    , mSelectionLeftPadding(0)
    , mSelectionTopPadding(0)
    , mSelectionRightPadding(0)
    , mSelectionBottomPadding(0)
    , mWidthMeasureSpec(0)
    , mCachingStarted(FALSE)
    , mCachingActive(FALSE)
    , mMotionPosition(0)
    , mMotionViewOriginalTop(0)
    , mMotionViewNewTop(0)
    , mMotionX(0)
    , mMotionY(0)
    , mTouchMode(TOUCH_MODE_REST)
    , mLastY(0)
    , mMotionCorrection(0)
    , mSelectedTop(0)
    , mStackFromBottom(FALSE)
    , mScrollingCacheEnabled(FALSE)
    , mFastScrollEnabled(FALSE)
    , mResurrectToPosition(IAdapterView::INVALID_POSITION)
    , mOverscrollMax(FALSE)
    , mOverscrollDistance(0)
    , mOverflingDistance(0)
    , mIsAttached(FALSE)
    , mDeferNotifyDataSetChanged(FALSE)
    , mSmoothScrollbarEnabled(TRUE)
    , mTextFilterEnabled(FALSE)
    , mFiltered(FALSE)
    , mLastTouchMode(TOUCH_MODE_UNKNOWN)
    , mScrollProfilingStarted(FALSE)
    , mFlingProfilingStarted(FALSE)
    , mTranscriptMode(0)
    , mCacheColorHint(0)
    , mIsChildViewEnabled(FALSE)
    , mLastScrollState(IAbsListViewOnScrollListener::SCROLL_STATE_IDLE)
    , mGlobalLayoutListenerAddedFilter(FALSE)
    , mTouchSlop(0)
    , mDensityScale(0)
    , mMinimumVelocity(0)
    , mMaximumVelocity(0)
    , mVelocityScale(1.0f)
    , mPopupHidden(FALSE)
    , mActivePointerId(INVALID_POINTER)
    , mFirstPositionDistanceGuess(0)
    , mLastPositionDistanceGuess(0)
    , mDirection(0)
    , mForceTranscriptScroll(0)
    , mGlowPaddingLeft(0)
    , mGlowPaddingRight(0)
    , mLastAccessibilityScrollEventFromIndex(0)
    , mLastAccessibilityScrollEventToIndex(0)
    , mLastHandledItemCount(0)
{
    CRect::NewByFriend((CRect**)&mSelectorRect);
    CRect::NewByFriend((CRect**)&mListPadding);
    mRecycler = new RecycleBin(this);
    mIsScrap = ArrayOf<Boolean>::Alloc(1);
    Init(context, attrs, defStyle);
}

AbsListView::~AbsListView()
{
    mCheckStates = NULL;
    mCheckedIdStates = NULL;
}

ECode AbsListView::InitAbsListView()
{
    // Setting focusable in touch mode will set the focusable property to TRUE
    SetClickable(TRUE);
    SetFocusableInTouchMode(TRUE);
    SetWillNotDraw(FALSE);
    SetAlwaysDrawnWithCacheEnabled(FALSE);
    SetScrollingCacheEnabled(TRUE);
    AutoPtr<IViewConfigurationHelper> configHelper;
    CViewConfigurationHelper::AcquireSingleton((IViewConfigurationHelper**)&configHelper);

    AutoPtr<IViewConfiguration> configuration;
    configHelper->Get(mContext, (IViewConfiguration**)&configuration);

    configuration->GetScaledTouchSlop(&mTouchSlop);
    configuration->GetScaledMinimumFlingVelocity(&mMinimumVelocity);
    configuration->GetScaledMaximumFlingVelocity(&mMaximumVelocity);
    configuration->GetScaledOverscrollDistance(&mOverscrollDistance);
    configuration->GetScaledOverflingDistance(&mOverflingDistance);

    AutoPtr<IResources> resources;
    FAIL_RETURN(mContext->GetResources((IResources**)&resources));

    AutoPtr<IDisplayMetrics> dm;
    FAIL_RETURN(resources->GetDisplayMetrics((IDisplayMetrics**)&dm));
    dm->GetDensity(&mDensityScale);

    return NOERROR;
}

//@Override
ECode AbsListView::SetOverScrollMode(
    /* [in] */ Int32 mode)
{
    if (mode != IView::OVER_SCROLL_NEVER) {
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

    return AdapterView::SetOverScrollMode(mode);
}

/**
 * {@inheritDoc}
 */
//@Override
ECode AbsListView::SetAdapter(
    /* [in] */ IAdapter* adapter)
{
    if (adapter != NULL) {
        mAdapter->HasStableIds(&mAdapterHasStableIds);
        if ((mChoiceMode != IAbsListView::CHOICE_MODE_NONE) && mAdapterHasStableIds &&
                mCheckedIdStates == NULL) {
            mCheckedIdStates = new HashMap<Int64, Int32>();
        }
    }

    if (mCheckStates != NULL) {
        mCheckStates->Clear();
    }

    if (mCheckedIdStates != NULL) {
        mCheckedIdStates->Clear();
    }
    return NOERROR;
}

/**
 * Returns the number of items currently selected. This will only be valid
 * if the choice mode is not {@link #CHOICE_MODE_NONE} (default).
 *
 * <p>To determine the specific items that are currently selected, use one of
 * the <code>getChecked*</code> methods.
 *
 * @return The number of items currently selected
 *
 * @see #getCheckedItemPosition()
 * @see #getCheckedItemPositions()
 * @see #getCheckedItemIds()
 */
Int32 AbsListView::GetCheckedItemCount()
{
    return mCheckedItemCount;
}

/**
 * Returns the checked state of the specified position. The result is only
 * valid if the choice mode has been set to {@link #CHOICE_MODE_SINGLE}
 * or {@link #CHOICE_MODE_MULTIPLE}.
 *
 * @param position The item whose checked state to return
 * @return The item's checked state or <code>FALSE</code> if choice mode
 *         is invalid
 *
 * @see #setChoiceMode(Int32)
 */
ECode AbsListView::IsItemChecked(
    /* [in] */ Int32 position,
    /* [out] */ Boolean* result)
{
    if (mChoiceMode != IAbsListView::CHOICE_MODE_NONE && mCheckStates != NULL) {
        *result = (*mCheckStates)[position];
        return NOERROR;
    }
    *result = FALSE;
    return NOERROR;
}

/**
 * Returns the currently checked item. The result is only valid if the choice
 * mode has been set to {@link #CHOICE_MODE_SINGLE}.
 *
 * @return The position of the currently checked item or
 *         {@link #INVALID_POSITION} if nothing is selected
 *
 * @see #setChoiceMode(Int32)
 */
ECode AbsListView::GetCheckedItemPosition(
    /* [out] */ Int32* result)
{
    if (mChoiceMode == IAbsListView::CHOICE_MODE_SINGLE
        && mCheckStates != NULL && mCheckStates->GetSize() == 1)
    {
        HashMap<Int32, Boolean>::Iterator it = mCheckStates->Begin();
        assert(it != mCheckStates->End());
        *result = it->mFirst;
        return NOERROR;
    }

    *result = IAdapterView::INVALID_POSITION;
    return NOERROR;
}

/**
 * Returns the set of checked items in the list. The result is only valid if
 * the choice mode has not been set to {@link #CHOICE_MODE_NONE}.
 *
 * @return  A SparseBooleanArray which will return TRUE for each call to
 *          get(Int32 position) where position is a position in the list,
 *          or <code>NULL</code> if the choice mode is set to
 *          {@link #CHOICE_MODE_NONE}.
 */
//AutoPtr<HashMap<Int32, Boolean> > AbsListView::GetCheckedItemPositions()
//{
//    if (mChoiceMode != IAbsListView::CHOICE_MODE_NONE) {
//        return mCheckStates;
//    }
//    return NULL;
//}

AutoPtr<IObjectInt32Map> AbsListView::GetCheckedItemPositions()
{
    AutoPtr<IObjectInt32Map> map;
    if (mChoiceMode != IAbsListView::CHOICE_MODE_NONE) {
        CObjectInt32Map::New((IObjectInt32Map**)&map);

        HashMap<Int32, Boolean>::Iterator it = mCheckStates->Begin();
        for(; it != mCheckStates->End(); ++it) {
            AutoPtr<IBoolean> ib;
            CBoolean::New(it->mSecond, (IBoolean**)&ib);
            map->Put(it->mFirst, (IInterface*)ib);
        }
    }

    return map;
}

/**
 * Returns the set of checked items ids. The result is only valid if the
 * choice mode has not been set to {@link #CHOICE_MODE_NONE} and the adapter
 * has stable IDs. ({@link ListAdapter#hasStableIds()} == {@code TRUE})
 *
 * @return A new array which contains the id of each checked item in the
 *         list.
 */
ECode AbsListView::GetCheckedItemIds(
    /* [out] */ ArrayOf<Int64>** result)
{
    VALIDATE_NOT_NULL(result);
    if (mChoiceMode == IAbsListView::CHOICE_MODE_NONE || mCheckedIdStates == NULL || mAdapter == NULL) {
        *result = ArrayOf<Int64>::Alloc(0);
        REFCOUNT_ADD(*result);
        return NOERROR;
    }

    Int32 count = mCheckedIdStates->GetSize();
    AutoPtr<ArrayOf<Int64> > tmp = ArrayOf<Int64>::Alloc(count);

    HashMap<Int64, Int32>::Iterator it = mCheckedIdStates->Begin();
    for (Int32 i = 0; it != mCheckedIdStates->End(); it++, i++) {
        (*tmp)[i] = it->mFirst;
    }

    *result = tmp;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

/**
 * Clear any choices previously set
 */
ECode AbsListView::ClearChoices()
{
    if (mCheckStates != NULL) {
        mCheckStates->Clear();
    }
    if (mCheckedIdStates != NULL) {
        mCheckedIdStates->Clear();
    }
    mCheckedItemCount = 0;

    return NOERROR;
}

/**
 * Sets the checked state of the specified position. The is only valid if
 * the choice mode has been set to {@link #CHOICE_MODE_SINGLE} or
 * {@link #CHOICE_MODE_MULTIPLE}.
 *
 * @param position The item whose checked state is to be checked
 * @param value The new checked state for the item
 */
ECode AbsListView::SetItemChecked(
    /* [in] */ Int32 position,
    /* [in] */Boolean value)
{
    if (mChoiceMode == IAbsListView::CHOICE_MODE_NONE) {
        return NOERROR;
    }

    // Start selection mode if needed. We don't need to if we're unchecking something.
    if (value && mChoiceMode == IAbsListView::CHOICE_MODE_MULTIPLE_MODAL && mChoiceActionMode == NULL) {
        Boolean hasWrappedCb;
        mMultiChoiceModeCallback->HasWrappedCallback(&hasWrappedCb);
        if (mMultiChoiceModeCallback == NULL || !hasWrappedCb)
        {
            return  E_ILLEGAL_STATE_EXCEPTION;
        }
        mChoiceActionMode = StartActionMode(mMultiChoiceModeCallback);
    }

    if (mChoiceMode == IAbsListView::CHOICE_MODE_MULTIPLE || mChoiceMode == IAbsListView::CHOICE_MODE_MULTIPLE_MODAL) {
        Boolean oldValue;
        oldValue = (*mCheckStates)[position];
        (*mCheckStates)[position] = value;
        Boolean hasId;
        mAdapter->HasStableIds(&hasId);
        if (mCheckedIdStates != NULL && hasId) {

            Int64 key;
            mAdapter->GetItemId(position, &key);

            if (value) {
                (*mCheckedIdStates)[key] = position;
            } else {
                mCheckedIdStates->Erase(key);
            }

        }
        if (oldValue != value) {
            if (value) {
                mCheckedItemCount++;
            } else {
                mCheckedItemCount--;
            }
        }
        if (mChoiceActionMode != NULL) {

            Int64 id;
            mAdapter->GetItemId(position, &id);
            mMultiChoiceModeCallback->OnItemCheckedStateChanged(mChoiceActionMode,
                    position, id, value);
        }
    } else {
        Boolean hasId;
        mAdapter->HasStableIds(&hasId);
        Boolean updateIds = mCheckedIdStates != NULL && hasId;
        // Clear all values if we're checking something, or unchecking the currently
        // selected item
        Boolean checked;
        IsItemChecked(position, &checked);
        if (value || checked) {
            mCheckStates->Clear();
            if (updateIds) {
                mCheckedIdStates->Clear();
            }
        }
        // this may end up selecting the value we just cleared but this way
        // we ensure length of mCheckStates is 1, a fact getCheckedItemPosition relies on
        if (value) {
            (*mCheckStates)[position] = TRUE;
            if (updateIds) {
                Int64 id;
                mAdapter->GetItemId(position, &id);
                (*mCheckedIdStates)[id] = position;
            }
            mCheckedItemCount = 1;
        } else if (mCheckStates->IsEmpty() || !(*mCheckStates)[0]) {
            mCheckedItemCount = 0;
        }
    }

    // Do not generate a data change while we are in the layout phase
    if (!mInLayout && !mBlockLayoutRequests) {
        mDataChanged = TRUE;
        RememberSyncState();
        RequestLayout();
    }

    return NOERROR;
}

Boolean AbsListView::PerformItemClick(
    /* [in] */ IView* view,
    /* [in] */ Int32 position,
    /* [in] */ Int64 id)
{
    Boolean handled = FALSE;
    Boolean dispatchItemClick = TRUE;

    if (mChoiceMode != IAbsListView::CHOICE_MODE_NONE) {
        handled = TRUE;
        Boolean checkedStateChanged = FALSE;

        if (mChoiceMode == IAbsListView::CHOICE_MODE_MULTIPLE ||
                (mChoiceMode == IAbsListView::CHOICE_MODE_MULTIPLE_MODAL && mChoiceActionMode != NULL)) {
            Boolean checked;
            HashMap<Int32, Boolean>::Iterator it = mCheckStates->Find(position);
            if(it != mCheckStates->End())
            {
                checked = it->mSecond;
            }else{
                checked = FALSE;
            }
            (*mCheckStates)[position] = checked;
            Boolean hasId;
            mAdapter->HasStableIds(&hasId);
            if (mCheckedIdStates != NULL && hasId) {
                if (checked) {
                    Int64 id;
                    mAdapter->GetItemId(position, &id);
                    (*mCheckedIdStates)[id] = position;
                } else {
                    Int64 id;
                    mAdapter->GetItemId(position, &id);
                    mCheckedIdStates->Erase(id);
                }
            }
            if (checked) {
                mCheckedItemCount++;
            } else {
                mCheckedItemCount--;
            }
            if (mChoiceActionMode != NULL) {
                mMultiChoiceModeCallback->OnItemCheckedStateChanged(mChoiceActionMode,
                        position, id, checked);
                dispatchItemClick = FALSE;
            }
            checkedStateChanged = TRUE;
        }
        else if (mChoiceMode == IAbsListView::CHOICE_MODE_SINGLE) {
            HashMap<Int32, Boolean>::Iterator it = mCheckStates->Find(position);
            Boolean checked;
            if (it != mCheckStates->End()) {
                checked = it->mSecond;
            }
            else{
                checked = FALSE;
            }

            if (checked) {
                mCheckStates->Clear();
                (*mCheckStates)[position] = TRUE;
                Boolean hasId;
                mAdapter->HasStableIds(&hasId);
                if (mCheckedIdStates != NULL && hasId) {
                    mCheckedIdStates->Clear();
                    Int64 itemId;
                    mAdapter->GetItemId(position, &itemId);
                    (*mCheckedIdStates)[itemId] = position;
                }
                mCheckedItemCount = 1;
            }
            else if (mCheckStates->IsEmpty() || !(*mCheckStates)[0]) {
                mCheckedItemCount = 0;
            }
            checkedStateChanged = TRUE;
        }

        if (checkedStateChanged) {
            UpdateOnScreenCheckedViews();
        }
    }

    if (dispatchItemClick) {
        handled |= AdapterView::PerformItemClick(view, position, id);
    }

    return handled;
}

/**
 * Perform a quick, in-place update of the checked or activated state
 * on all visible item views. This should only be called when a valid
 * choice mode is active.
 */

void AbsListView::UpdateOnScreenCheckedViews() {
    Int32 firstPos = mFirstPosition;
    Int32 count = GetChildCount();
    AutoPtr<IContext> context = GetContext();
    AutoPtr<IApplicationInfo> info;

    context->GetApplicationInfo((IApplicationInfo**)&info);

    Int32 sdkVersion;
    info->GetTargetSdkVersion(&sdkVersion);
    Boolean useActivated = sdkVersion >= 11;
    for (Int32 i = 0; i < count; i++) {
        AutoPtr<IView> child = GetChildAt(i);
        Int32 position = firstPos + i;
        AutoPtr<ICheckable> checkable = (ICheckable*)child->Probe(EIID_ICheckable);

        if (checkable != NULL) {
            checkable ->SetChecked((*mCheckStates)[position]);
        } else if (useActivated) {
            child->SetActivated((*mCheckStates)[position]);
        }
    }

}

/**
 * @see #setChoiceMode(Int32)
 *
 * @return The current choice mode
 */
ECode AbsListView::GetChoiceMode(
        /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mChoiceMode;
    return NOERROR;
}

/**
 * Defines the choice behavior for the List. By default, Lists do not have any choice behavior
 * ({@link #CHOICE_MODE_NONE}). By setting the choiceMode to {@link #CHOICE_MODE_SINGLE}, the
 * List allows up to one item to  be in a chosen state. By setting the choiceMode to
 * {@link #CHOICE_MODE_MULTIPLE}, the list allows any number of items to be chosen.
 *
 * @param choiceMode One of {@link #CHOICE_MODE_NONE}, {@link #CHOICE_MODE_SINGLE}, or
 * {@link #CHOICE_MODE_MULTIPLE}
 */
ECode AbsListView::SetChoiceMode(
        /* [in] */ Int32 choiceMode)
{
    mChoiceMode = choiceMode;
    if (mChoiceActionMode != NULL) {
        mChoiceActionMode->Finish();
        mChoiceActionMode = NULL;
    }
    if (mChoiceMode != IAbsListView::CHOICE_MODE_NONE) {
        if (mCheckStates == NULL) {
            mCheckStates = new HashMap<Int32, Boolean>();
        }
        if (mCheckedIdStates == NULL && mAdapter != NULL) {
            Boolean stableIds;
            mAdapter->HasStableIds(&stableIds);
            if(stableIds) mCheckedIdStates = new HashMap<Int64, Int32>();
        }
        // Modal multi-choice mode only has choices when the mode is active. Clear them.
        if (mChoiceMode == IAbsListView::CHOICE_MODE_MULTIPLE_MODAL) {
            ClearChoices();
            SetLongClickable(TRUE);
        }
    }
    return NOERROR;
}

/**
 * Set a {@link MultiChoiceModeListener} that will manage the lifecycle of the
 * selection {@link ActionMode}. Only used when the choice mode is set to
 * {@link #CHOICE_MODE_MULTIPLE_MODAL}.
 *
 * @param listener Listener that will manage the selection mode
 *
 * @see #setChoiceMode(Int32)
 */
ECode AbsListView::SetMultiChoiceModeListener(
        /* [in] */ IMultiChoiceModeListener* listener)
{
    if (mMultiChoiceModeCallback == NULL) {
        mMultiChoiceModeCallback = new MultiChoiceModeWrapper(this);
    }
    mMultiChoiceModeCallback->SetWrapped(listener);
    return NOERROR;
}

/**
 * @return TRUE if all list content currently fits within the view boundaries
 */
Boolean AbsListView::ContentFits()
{
    Int32 childCount = GetChildCount();
    if(childCount == 0)
    {
        return TRUE;
    }
    if(childCount != mItemCount)
    {
        return FALSE;
    }

    Int32 childTop;
    Int32 childBottom;
    GetChildAt(0)->GetTop(&childTop);
    GetChildAt(childCount - 1)->GetBottom(&childBottom);

    return (childTop >= mListPadding->mTop && childBottom <= GetHeight() - mListPadding->mBottom);
}

/**
 * Enables fast scrolling by letting the user quickly scroll through lists by
 * dragging the fast scroll thumb. The adapter attached to the list may want
 * to implement {@link SectionIndexer} if it wishes to display alphabet preview and
 * jump between sections of the list.
 * @see SectionIndexer
 * @see #isFastScrollEnabled()
 * @param enabled whether or not to enable fast scrolling
 */
ECode AbsListView::SetFastScrollEnabled(
    /* [in] */ Boolean enabled)
{
    mFastScrollEnabled = enabled;
    if (enabled) {
        if (mFastScroller == NULL) {
            AutoPtr<IContext> context = GetContext();
            mFastScroller = new FastScroller(context, this);
        }
    }
    else {
        if (mFastScroller != NULL) {
            mFastScroller->Stop();
            mFastScroller = NULL;
        }
    }
    return NOERROR;
}

/**
 * Set whether or not the fast scroller should always be shown in place of the
 * standard scrollbars. Fast scrollers shown in this way will not fade out and will
 * be a permanent fixture within the list. Best combined with an inset scroll bar style
 * that will ensure enough padding. This will enable fast scrolling if it is not
 * already enabled.
 *
 * @param alwaysShow TRUE if the fast scroller should always be displayed.
 * @see #setScrollBarStyle(Int32)
 * @see #setFastScrollEnabled(Boolean)
 */
ECode AbsListView::SetFastScrollAlwaysVisible(
        /* [in] */ Boolean alwaysShow)
{
    if (alwaysShow && !mFastScrollEnabled) {
        SetFastScrollEnabled(TRUE);
    }

    if (mFastScroller != NULL) {
        mFastScroller->SetAlwaysShow(alwaysShow);
    }

    ComputeOpaqueFlags();
    RecomputePadding();
    return NOERROR;
}

/**
 * Returns TRUE if the fast scroller is set to always show on this view rather than
 * fade out when not in use.
 *
 * @return TRUE if the fast scroller will always show.
 * @see #setFastScrollAlwaysVisible(Boolean)
 */
Boolean AbsListView::IsFastScrollAlwaysVisible()
{
    return mFastScrollEnabled
        && mFastScroller != NULL
        && mFastScroller->IsAlwaysShowEnabled();
}

//@Override
Int32 AbsListView::GetVerticalScrollbarWidth()
{
    using Elastos::Core::Math;
    if (IsFastScrollAlwaysVisible()) {
        return Math::Max(View::GetVerticalScrollbarWidth(), mFastScroller->GetWidth());
    }
    return View::GetVerticalScrollbarWidth();
}

/**
 * Returns the current state of the fast scroll feature.
 * @see #setFastScrollEnabled(Boolean)
 * @return TRUE if fast scroll is enabled, FALSE otherwise
 */
Boolean AbsListView::IsFastScrollEnabled()
{
    return mFastScrollEnabled;
}

//@Override
ECode AbsListView::SetVerticalScrollbarPosition(
    /* [in] */ Int32 position)
{
    View::SetVerticalScrollbarPosition(position);
    if (mFastScroller != NULL) {
        mFastScroller->SetScrollbarPosition(position);
    }
    return NOERROR;
}

/**
 * If fast scroll is visible, then don't draw the vertical scrollbar.
 * @hide
 */
//@Override
Boolean AbsListView::IsVerticalScrollBarHidden()
{
    return mFastScroller != NULL && mFastScroller->IsVisible();
}

/**
 * When smooth scrollbar is enabled, the position and size of the scrollbar thumb
 * is computed based on the number of visible pixels in the visible items. This
 * however assumes that all list items have the same height. If you use a list in
 * which items have different heights, the scrollbar will change appearance as the
 * user scrolls through the list. To avoid this issue, you need to disable this
 * property.
 *
 * When smooth scrollbar is disabled, the position and size of the scrollbar thumb
 * is based solely on the number of items in the adapter and the position of the
 * visible items inside the adapter. This provides a stable scrollbar as the user
 * navigates through a list of items with varying heights.
 *
 * @param enabled Whether or not to enable smooth scrollbar.
 *
 * @see #setSmoothScrollbarEnabled(Boolean)
 * @attr ref android.R.styleable#AbsListView_smoothScrollbar
 */
ECode AbsListView::SetSmoothScrollbarEnabled(
    /* [in] */ Boolean enabled)
{
    mSmoothScrollbarEnabled = enabled;

    return NOERROR;
}

/**
 * Returns the current state of the fast scroll feature.
 *
 * @return True if smooth scrollbar is enabled is enabled, FALSE otherwise.
 *
 * @see #setSmoothScrollbarEnabled(Boolean)
 */
//@ViewDebug.ExportedProperty
Boolean AbsListView::IsSmoothScrollbarEnabled()
{
    return mSmoothScrollbarEnabled;
}

/**
 * Set the listener that will receive notifications every time the list scrolls.
 *
 * @param l the scroll listener
 */
ECode AbsListView::SetOnScrollListener(
    /* [in] */ IAbsListViewOnScrollListener* l)
{
    mOnScrollListener = l;
    InvokeOnItemScrollListener();

    return NOERROR;
}

/**
 * Notify our scroll listener (if there is one) of a change in scroll state
 */
ECode AbsListView::InvokeOnItemScrollListener()
{
    if (mFastScroller != NULL) {
        mFastScroller->OnScroll(THIS_PROBE(IAbsListView),
                mFirstPosition, GetChildCount(), mItemCount);
    }

    if (mOnScrollListener != NULL) {
        mOnScrollListener->OnScroll(THIS_PROBE(IAbsListView),
            mFirstPosition, GetChildCount(), mItemCount);
    }

    OnScrollChanged(0, 0, 0, 0); // dummy values, View's implementation does not use these.

    return NOERROR;
}

//@Override
ECode AbsListView::SendAccessibilityEvent(
        /* [in] */ Int32 eventType)
{
    // Since this class calls onScrollChanged even if the mFirstPosition and the
    // child count have not changed we will avoid sending duplicate accessibility
    // events.
    if (eventType == IAccessibilityEvent::TYPE_VIEW_SCROLLED) {
        Int32 firstVisiblePosition = GetFirstVisiblePosition();
        Int32 lastVisiblePosition = GetLastVisiblePosition();
        if (mLastAccessibilityScrollEventFromIndex == firstVisiblePosition
            && mLastAccessibilityScrollEventToIndex == lastVisiblePosition) {
                return NOERROR;
        } else {
            mLastAccessibilityScrollEventFromIndex = firstVisiblePosition;
            mLastAccessibilityScrollEventToIndex = lastVisiblePosition;
        }
    }
    return View::SendAccessibilityEvent(eventType);
}

//@Override
ECode AbsListView::OnInitializeAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event)
{
    FAIL_RETURN(View::OnInitializeAccessibilityEvent(event));
    String classNameStr("AbsListView");
    AutoPtr<ICharSequence> className;
    FAIL_RETURN(CStringWrapper::New(classNameStr, (ICharSequence**)&className));
    event->SetClassName(className);
    return NOERROR;
}

//@Override
ECode AbsListView::OnInitializeAccessibilityNodeInfo(
        /* [in] */ IAccessibilityNodeInfo* info)
{
    View::OnInitializeAccessibilityNodeInfo(info);
    String classNameStr("AbsListView");
    AutoPtr<ICharSequence> className;
    FAIL_RETURN(CStringWrapper::New(classNameStr, (ICharSequence**)&className));
    info->SetClassName(className);
    if (IsEnabled()) {
        if (GetFirstVisiblePosition() > 0) {
            info->AddAction(IAccessibilityNodeInfo::ACTION_SCROLL_BACKWARD);
        }
        if (GetLastVisiblePosition() < GetCount() - 1) {
            info->AddAction(IAccessibilityNodeInfo::ACTION_SCROLL_FORWARD);
        }
    }
    return NOERROR;
}

//@Override
Boolean AbsListView::PerformAccessibilityAction(
    /* [in] */ Int32 action,
    /* [in] */ IBundle* arguments)
{
    if (View::PerformAccessibilityAction(action, arguments)) {
        return TRUE;
    }
    switch(action)
    {
        case IAccessibilityNodeInfo::ACTION_SCROLL_FORWARD:
        {
            if (IsEnabled() && (GetLastVisiblePosition() < GetCount() - 1))
            {
                Int32 viewportHeight = GetHeight() - mListPadding->mTop - mListPadding->mBottom;
                SmoothScrollBy(viewportHeight, PositionScroller::SCROLL_DURATION);
                return TRUE;
            }
        }
        return FALSE;

        case IAccessibilityNodeInfo::ACTION_SCROLL_BACKWARD:
        {
            if (IsEnabled() && mFirstPosition > 0) {
                Int32 viewportHeight = GetHeight() - mListPadding->mTop - mListPadding->mBottom;
                SmoothScrollBy(-viewportHeight, PositionScroller::SCROLL_DURATION);
                return TRUE;
            }
        }
        return FALSE;
    }
    return FALSE;//
}

/**
 * Indicates whether the children's drawing cache is used during a scroll.
 * By default, the drawing cache is enabled but this will consume more memory.
 *
 * @return TRUE if the scrolling cache is enabled, FALSE otherwise
 *
 * @see #setScrollingCacheEnabled(Boolean)
 * @see View#setDrawingCacheEnabled(Boolean)
 */
//@ViewDebug.ExportedProperty
Boolean AbsListView::IsScrollingCacheEnabled()
{
    return mScrollingCacheEnabled;
}

/**
 * Enables or disables the children's drawing cache during a scroll.
 * By default, the drawing cache is enabled but this will use more memory.
 *
 * When the scrolling cache is enabled, the caches are kept after the
 * first scrolling. You can manually clear the cache by calling
 * {@link android.view.ViewGroup#setChildrenDrawingCacheEnabled(Boolean)}.
 *
 * @param enabled TRUE to enable the scroll cache, FALSE otherwise
 *
 * @see #isScrollingCacheEnabled()
 * @see View#setDrawingCacheEnabled(Boolean)
 */
ECode AbsListView::SetScrollingCacheEnabled(
    /* [in] */ Boolean enabled)
{
    if (mScrollingCacheEnabled && !enabled) {
        ClearScrollingCache();
    }
    mScrollingCacheEnabled = enabled;

    return NOERROR;
}

/**
 * Enables or disables the type filter window. If enabled, typing when
 * this view has focus will filter the children to match the users input.
 * Note that the {@link Adapter} used by this view must implement the
 * {@link Filterable} interface.
 *
 * @param textFilterEnabled TRUE to enable type filtering, FALSE otherwise
 *
 * @see Filterable
 */
ECode AbsListView::SetTextFilterEnabled(
    /* [in] */ Boolean textFilterEnabled)
{
    mTextFilterEnabled = textFilterEnabled;

    return NOERROR;
}

/**
 * Indicates whether type filtering is enabled for this view
 *
 * @return TRUE if type filtering is enabled, FALSE otherwise
 *
 * @see #setTextFilterEnabled(Boolean)
 * @see Filterable
 */
//@ViewDebug.ExportedProperty
Boolean AbsListView::IsTextFilterEnabled()
{
    return mTextFilterEnabled;
}

//@Override
ECode AbsListView::GetFocusedRect(
    /* [in] */ IRect* r)
{

    AutoPtr<IView> view = GetSelectedView();
    if (view != NULL) {
        AutoPtr<IViewParent> vp;
        view->GetParent((IViewParent**)&vp);
        if (IView::Probe(vp) == THIS_PROBE(IView)) {
            // the focused rectangle of the selected view offset into the
            // coordinate space of this view.
            view->GetFocusedRect(r);
            OffsetDescendantRectToMyCoords(view, r);
        }
    }
    else {
        // otherwise, just the norm
        return AdapterView::GetFocusedRect(r);
    }
    return NOERROR;

}

ECode AbsListView::UseDefaultSelector()
{
    AutoPtr<IDrawable> drawable;
    GetResources()->GetDrawable(
        R::drawable::list_selector_background,
        (IDrawable**)&drawable);
    SetSelector(drawable);
    return NOERROR;
}

/**
 * Indicates whether the content of this view is pinned to, or stacked from,
 * the bottom edge.
 *
 * @return TRUE if the content is stacked from the bottom edge, FALSE otherwise
 */
//@ViewDebug.ExportedProperty
Boolean AbsListView::IsStackFromBottom()
{
    return mStackFromBottom;
}

/**
 * When stack from bottom is set to TRUE, the list fills its content starting from
 * the bottom of the view.
 *
 * @param stackFromBottom TRUE to pin the view's content to the bottom edge,
 *        FALSE to pin the view's content to the top edge
 */
ECode AbsListView::SetStackFromBottom(
    /* [in] */ Boolean stackFromBottom)
{
    if (mStackFromBottom != stackFromBottom) {
        mStackFromBottom = stackFromBottom;
        RequestLayoutIfNecessary();
    }

    return NOERROR;
}

ECode AbsListView::RequestLayoutIfNecessary()
{
    if (GetChildCount() > 0) {
        ResetList();
        RequestLayout();
        Invalidate();
    }

    return NOERROR;
}

//@Override
AutoPtr<IParcelable> AbsListView::OnSaveInstanceState()
{
    /*
     * This doesn't really make sense as the place to dismiss the
     * popups, but there don't seem to be any other useful hooks
     * that happen early enough to keep from getting complaints
     * about having leaked the window.
     */
    DismissPopup();

    AutoPtr<IParcelable> superState = AdapterView::OnSaveInstanceState();

    AutoPtr<CAbsListViewSavedState> ss;
    ASSERT_SUCCEEDED(CAbsListViewSavedState::NewByFriend(
        superState, (CAbsListViewSavedState**)&ss));

    if (mPendingSync != NULL) {
        // Just keep what we last restored.
        ss->mSelectedId = mPendingSync->mSelectedId;
        ss->mFirstId = mPendingSync->mFirstId;
        ss->mViewTop = mPendingSync->mViewTop;
        ss->mPosition = mPendingSync->mPosition;
        ss->mHeight = mPendingSync->mHeight;
        ss->mFilter = mPendingSync->mFilter;
        ss->mInActionMode = mPendingSync->mInActionMode;
        ss->mCheckedItemCount = mPendingSync->mCheckedItemCount;
        ss->mCheckState = mPendingSync->mCheckState;
        ss->mCheckIdState = mPendingSync->mCheckIdState;
        return (IParcelable*)ss->Probe(EIID_IParcelable);
    }

    Boolean haveChildren = GetChildCount() > 0 && mItemCount > 0;
    Int64 selectedId = GetSelectedItemId();
    ss->mSelectedId = selectedId;
    ss->mHeight = GetHeight();

    if (selectedId >= 0) {
        // Remember the selection
        ss->mViewTop = this->mSelectedTop;
        ss->mPosition = GetSelectedItemPosition();
        ss->mFirstId = IAdapterView::INVALID_POSITION;
    }
    else {
        if (haveChildren && mFirstPosition > 0) {
            // Remember the position of the first child.
            // We only do this if we are not currently at the top of
            // the list, for two reasons:
            // (1) The list may be in the process of becoming empty, in
            // which case mItemCount may not be 0, but if we try to
            // ask for any information about position 0 we will crash.
            // (2) Being "at the top" seems like a special case, anyway,
            // and the user wouldn't expect to end up somewhere else when
            // they revisit the list even if its content has changed.
            AutoPtr<IView> v = GetChildAt(0);
            v->GetTop(&ss->mViewTop);
            Int32 firstPos = mFirstPosition;
            if (firstPos >= mItemCount) {
                firstPos = mItemCount - 1;
            }
            ss->mPosition = firstPos;
            mAdapter->GetItemId(firstPos, &ss->mFirstId);
        }
        else {
            ss->mViewTop = 0;
            ss->mFirstId = IAdapterView::INVALID_POSITION;
            ss->mPosition = 0;
        }
    }
    ss->mFilter = NULL;
    if (mFiltered) {
        AutoPtr<IEditText> textFilter = mTextFilter;
        if (textFilter != NULL) {
            AutoPtr<ICharSequence> filterText;
            textFilter->GetText((ICharSequence**)&filterText);
            if (filterText->Probe(EIID_IEditable) != NULL) {
                filterText->ToString(&(ss->mFilter));
            }
        }
    }

    ss->mInActionMode = (mChoiceMode == IAbsListView::CHOICE_MODE_MULTIPLE_MODAL) && (mChoiceActionMode != NULL);

    if (mCheckStates != NULL) {
        ss->mCheckState = new HashMap<Int32, Boolean>(*(mCheckStates.Get()));
    }

    if (mCheckedIdStates != NULL) {
        ss->mCheckIdState = new HashMap<Int64, Int32>(*(mCheckedIdStates.Get()));
    }
    ss->mCheckedItemCount = mCheckedItemCount;

    if (mRemoteAdapter != NULL) {
        mRemoteAdapter->SaveRemoteViewsCache();
    }

    return (IParcelable*)ss->Probe(EIID_IParcelable);
}

//@Override
void AbsListView::OnRestoreInstanceState(
    /* [in] */ IParcelable* state)
{
    AutoPtr<CAbsListViewSavedState> ss =
        (CAbsListViewSavedState*)IAbsListViewSavedState::Probe(state);
    if(!ss)
        return;

    AutoPtr<IParcelable> superState;
    ss->GetSuperState((IParcelable**)&superState);

    AdapterView::OnRestoreInstanceState(superState);

    mDataChanged = TRUE;

    mSyncHeight = ss->mHeight;

    if (ss->mSelectedId >= 0) {
        mNeedSync = TRUE;
        mPendingSync = ss;
        mSyncRowId = ss->mSelectedId;
        mSyncPosition = ss->mPosition;
        mSpecificTop = ss->mViewTop;
        mSyncMode = AdapterView::SYNC_SELECTED_POSITION;
    }
    else if (ss->mFirstId >= 0) {
        SetSelectedPositionInt(IAdapterView::INVALID_POSITION);
        // Do this before setting mNeedSync since setNextSelectedPosition looks at mNeedSync
        SetNextSelectedPositionInt(IAdapterView::INVALID_POSITION);
        mSelectorPosition = IAdapterView::INVALID_POSITION;
        mNeedSync = TRUE;
        mPendingSync = ss;
        mSyncRowId = ss->mFirstId;
        mSyncPosition = ss->mPosition;
        mSpecificTop = ss->mViewTop;
        mSyncMode = AdapterView::SYNC_FIRST_POSITION;
    }

    SetFilterText(ss->mFilter);

    if (ss->mCheckState != NULL) {
        mCheckStates = ss->mCheckState;
    }

    if (ss->mCheckIdState != NULL) {
        mCheckedIdStates = ss->mCheckIdState;
    }

    mCheckedItemCount = ss->mCheckedItemCount;

    if (ss->mInActionMode && mChoiceMode == IAbsListView::CHOICE_MODE_MULTIPLE_MODAL &&
        mMultiChoiceModeCallback != NULL) {
            mChoiceActionMode = StartActionMode(mMultiChoiceModeCallback);
    }

    RequestLayout();

}

Boolean AbsListView::AcceptFilter()
{
    AutoPtr<IAdapter> adapter;
    ((IAdapterView*)(this->Probe(EIID_IAdapterView)))->GetAdapter((IAdapter**)&adapter);
    AutoPtr<IFilterable> temp = (IFilterable*)adapter->Probe(EIID_IFilterable);
    if(temp == NULL || !mTextFilterEnabled)
    {
        return FALSE;
    }else{
        AutoPtr<IFilter> filter;
        temp->GetFilter((IFilter**)&filter);
        if(filter != NULL)
            return TRUE;
    }
    return FALSE;//
}

/**
 * Sets the initial value for the text filter.
 * @param filterText The text to use for the filter.
 *
 * @see #setTextFilterEnabled
 */
ECode AbsListView::SetFilterText(
    /* [in] */ const String& filterText)
{
    // TODO: Should we check for acceptFilter()?
    if (mTextFilterEnabled && !filterText.IsNullOrEmpty()) {
        CreateTextFilter(FALSE);
        // This is going to call our listener onTextChanged, but we might not
        // be ready to bring up a window yet
        AutoPtr<ICharSequence> cFilterText;
        FAIL_RETURN(CStringWrapper::New(filterText, (ICharSequence**)&cFilterText));
        mTextFilter->SetText(cFilterText);
        mTextFilter->SetSelection(filterText.GetLength());

        AutoPtr<IFilterable> filterable = (IFilterable*)mAdapter->Probe(EIID_IFilterable);
        if (filterable) {
            // if mPopup is non-NULL, then onTextChanged will do the filtering
            if (mPopup == NULL) {
                AutoPtr<IFilter> f;
                filterable->GetFilter((IFilter**)&f);
                f->DoFilter(cFilterText);
            }
            // Set filtered to TRUE so we will display the filter window when our main
            // window is ready
            mFiltered = TRUE;
            mDataSetObserver->ClearSavedState();
        }
    }
    return NOERROR;
}

/**
 * Returns the list's text filter, if available.
 * @return the list's text filter or NULL if filtering isn't enabled
 */
AutoPtr<ICharSequence> AbsListView::GetTextFilter()
{
    if (mTextFilterEnabled && mTextFilter != NULL) {
        AutoPtr<ICharSequence> result;
        mTextFilter->GetText((ICharSequence**)&result);
        return result;
    }
    return NULL;
}

//@Override
void AbsListView::OnFocusChanged(
    /* [in] */ Boolean gainFocus,
    /* [in] */ Int32 direction,
    /* [in] */ IRect* previouslyFocusedRect)
{
    AdapterView::OnFocusChanged(gainFocus, direction, previouslyFocusedRect);
    if (gainFocus && mSelectedPosition < 0 && !IsInTouchMode()) {
        if (!mIsAttached && mAdapter != NULL) {
            // Data may have changed while we were detached and it's valid
            // to change focus while detached. Refresh so we don't die.
            mDataChanged = TRUE;
            mOldItemCount = mItemCount;
            mAdapter->GetCount(&mItemCount);
        }
        ResurrectSelection();
    }
}

//@Override
ECode AbsListView::RequestLayout()
{
    if (!mBlockLayoutRequests && !mInLayout) {
        AdapterView::RequestLayout();
    }

    return NOERROR;
}

/**
 * The list is empty. Clear everything out.
 */
void AbsListView::ResetList()
{
    RemoveAllViewsInLayout();
    mFirstPosition = 0;
    mDataChanged = FALSE;
    mPositionScrollAfterLayout = NULL;
    mNeedSync = FALSE;
    mPendingSync = NULL;
    mOldSelectedPosition = IAdapterView::INVALID_POSITION;
    mOldSelectedRowId = IAdapterView::INVALID_ROW_ID;
    SetSelectedPositionInt(IAdapterView::INVALID_POSITION);
    SetNextSelectedPositionInt(IAdapterView::INVALID_POSITION);
    mSelectedTop = 0;
    mSelectorPosition = IAdapterView::INVALID_POSITION;
    mSelectorRect->SetEmpty();
    Invalidate();
}

//@Override
Int32 AbsListView::ComputeVerticalScrollExtent()
{
    Int32 count = GetChildCount();
    if (count > 0) {
        if (mSmoothScrollbarEnabled) {
            Int32 extent = count * 100;

            AutoPtr<IView> view = GetChildAt(0);
            Int32 top, height;
            view->GetTop(&top);
            view->GetHeight(&height);

            if (height > 0) {
                extent += (top * 100) / height;
            }

            view = GetChildAt(count - 1);

            Int32 bottom;
            view->GetBottom(&bottom);
            view->GetHeight(&height);

            if (height > 0) {
                extent -= ((bottom - GetHeight()) * 100) / height;
            }

            return extent;
        }
        else {
            return 1;
        }
    }
    return 0;
}

//@Override
Int32 AbsListView::ComputeVerticalScrollOffset()
{
    Int32 firstPosition = mFirstPosition;
    Int32 childCount = GetChildCount();
    if (firstPosition >= 0 && childCount > 0) {
        if (mSmoothScrollbarEnabled) {
            AutoPtr<IView> view = GetChildAt(0);
            Int32 top, height;
            view->GetTop(&top);
            view->GetHeight(&height);
            if (height > 0) {
                return Elastos::Core::Math::Max(firstPosition * 100 - (top * 100) / height +
                    (Int32)((Float)mScrollY / GetHeight() * mItemCount * 100), 0);
            }
        }
        else {
            Int32 index;
            Int32 count = mItemCount;
            if (firstPosition == 0) {
                index = 0;
            }
            else if (firstPosition + childCount == count) {
                index = count;
            }
            else {
                index = firstPosition + childCount / 2;
            }
            return (Int32)(firstPosition + childCount * (index / (Float)count));
        }
    }
    return 0;
}

//@Override
Int32 AbsListView::ComputeVerticalScrollRange()
{
    Int32 result;
    if (mSmoothScrollbarEnabled) {
        result = Elastos::Core::Math::Max(mItemCount * 100, 0);
        if (mScrollY != 0) {
            // Compensate for overscroll
            result += Elastos::Core::Math::Abs((Int32)((Float) mScrollY / GetHeight() * mItemCount * 100));
        }
    }
    else {
        result = mItemCount;
    }

    return result;
}

//@Override
Float AbsListView::GetTopFadingEdgeStrength()
{
    Int32 count = GetChildCount();
    Float fadeEdge = AdapterView::GetTopFadingEdgeStrength();
    if (count == 0) {
        return fadeEdge;
    }
    else {
        if (mFirstPosition > 0) {
            return 1.0f;
        }

        Int32 top;
        GetChildAt(0)->GetTop(&top);
        Float fadeLength = (Float)GetVerticalFadingEdgeLength();
        return top < mPaddingTop ? (Float)-(top - mPaddingTop) / fadeLength : fadeEdge;
    }
}

//@Override
Float AbsListView::GetBottomFadingEdgeStrength()
{
    Int32 count = GetChildCount();
    Float fadeEdge = AdapterView::GetBottomFadingEdgeStrength();
    if (count == 0) {
        return fadeEdge;
    }
    else {
        if (mFirstPosition + count - 1 < mItemCount - 1) {
            return 1.0f;
        }

        Int32 bottom;
        GetChildAt(count - 1)->GetBottom(&bottom);

        Int32 height = GetHeight();
        Float fadeLength = (Float)GetVerticalFadingEdgeLength();
        return bottom > height - mPaddingBottom ?
            (Float)(bottom - height + mPaddingBottom) / fadeLength : fadeEdge;
    }
}

//@Override
void AbsListView::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    if (mSelector == NULL) {
        UseDefaultSelector();
    }

    mListPadding->Set(mSelectionLeftPadding + mPaddingLeft
                    , mSelectionTopPadding + mPaddingTop
                    , mSelectionRightPadding + mPaddingRight
                    , mSelectionBottomPadding + mPaddingBottom);

    // Check if our previous measured size was at a point where we should scroll later.
    if (mTranscriptMode == IAbsListView::AbsListView_TRANSCRIPT_MODE_NORMAL) {
        Int32 childCount = GetChildCount();
        Int32 listBottom = GetHeight() - GetPaddingBottom();
        AutoPtr<IView> lastChild = GetChildAt(childCount - 1);
        Int32 lastChildBottom;
        if(lastChild != NULL)
        {
            lastChild->GetBottom(&lastChildBottom);
        }else{
            lastChildBottom = listBottom;
        }

        mForceTranscriptScroll = mFirstPosition + childCount >= mLastHandledItemCount &&
                lastChildBottom <= listBottom;
    }
}

/**
 * Subclasses should NOT override this method but
 *  {@link #layoutChildren()} instead.
 */
//@Override
void AbsListView::OnLayout(
    /* [in] */ Boolean changed,
    /* [in] */ Int32 l,
    /* [in] */ Int32 t,
    /* [in] */ Int32 r,
    /* [in] */ Int32 b)
{
    AdapterView::OnLayout(changed, l, t, r, b);
    mInLayout = TRUE;
    if (changed) {
        Int32 childCount = GetChildCount();
        for (Int32 i = 0; i < childCount; i++) {
            GetChildAt(i)->ForceLayout();
        }
        mRecycler->MarkChildrenDirty();
    }
    if (mFastScroller != NULL && mItemCount != mOldItemCount) {
        mFastScroller->OnItemCountChanged(mOldItemCount, mItemCount);
    }
    LayoutChildren();
    mInLayout = FALSE;

    mOverscrollMax = (b - t) / AbsListView::OVERSCROLL_LIMIT_DIVISOR;
}

/**
 * @hide
 */
//@Override
Boolean AbsListView::SetFrame(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    Boolean changed = AdapterView::SetFrame(left, top, right, bottom);

    if (changed) {
        // Reposition the popup when the frame has changed. This includes
        // translating the widget, not just changing its dimension. The
        // filter popup needs to follow the widget.
        Boolean visible = GetWindowVisibility() == IView::VISIBLE;
        Boolean showing = FALSE;
        if(mPopup)
            mPopup->IsShowing(&showing);
        if (mFiltered && visible && showing) {
            PositionPopup();
        }
    }

    return changed;
}

void AbsListView::LayoutChildren()
{
}

void AbsListView::UpdateScrollIndicators()
{
    if (mScrollUp != NULL) {
        Boolean canScrollUp;
        // 0th element is not visible
        canScrollUp = mFirstPosition > 0;

        // ... Or top of 0th element is not visible
        if (!canScrollUp) {
            if (GetChildCount() > 0) {
                Int32 top;
                GetChildAt(0)->GetTop(&top);
                canScrollUp = top < mListPadding->mTop;
            }
        }

        mScrollUp->SetVisibility(canScrollUp ? IView::VISIBLE : IView::INVISIBLE);
    }

    if (mScrollDown != NULL) {
        Boolean canScrollDown;
        Int32 count = GetChildCount();

        // Last item is not visible
        canScrollDown = (mFirstPosition + count) < mItemCount;

        // ... Or bottom of the last element is not visible
        if (!canScrollDown && count > 0) {
            Int32 bottom;
            GetChildAt(count - 1)->GetBottom(&bottom);
            canScrollDown = bottom > mBottom - mListPadding->mBottom;
        }

        mScrollDown->SetVisibility(canScrollDown ? IView::VISIBLE : IView::INVISIBLE);
    }
}

//@Override
//@ViewDebug.ExportedProperty
AutoPtr<IView> AbsListView::GetSelectedView()
{
    if (mItemCount > 0 && mSelectedPosition >= 0) {
        return GetChildAt(mSelectedPosition - mFirstPosition);
    }
    else {
        return NULL;
    }
    return NULL;////
}

/**
 * List padding is the maximum of the normal view's padding and the padding of the selector.
 *
 * @see android.view.View#getPaddingTop()
 * @see #getSelector()
 *
 * @return The top list padding.
 */
Int32 AbsListView::GetListPaddingTop()
{
    return mListPadding->mTop;
}

/**
 * List padding is the maximum of the normal view's padding and the padding of the selector.
 *
 * @see android.view.View#getPaddingBottom()
 * @see #getSelector()
 *
 * @return The bottom list padding.
 */
Int32 AbsListView::GetListPaddingBottom()
{
    return mListPadding->mBottom;
}

/**
 * List padding is the maximum of the normal view's padding and the padding of the selector.
 *
 * @see android.view.View#getPaddingLeft()
 * @see #getSelector()
 *
 * @return The left list padding.
 */
Int32 AbsListView::GetListPaddingLeft()
{
    return mListPadding->mLeft;
}

/**
 * List padding is the maximum of the normal view's padding and the padding of the selector.
 *
 * @see android.view.View#getPaddingRight()
 * @see #getSelector()
 *
 * @return The right list padding.
 */
Int32 AbsListView::GetListPaddingRight()
{
    return mListPadding->mRight;
}

/**
 * Get a view and have it show the data associated with the specified
 * position. This is called when we have already discovered that the view is
 * not available for reuse in the recycle bin. The only choices left are
 * converting an old view or making a new one.
 *
 * @param position The position to display
 * @param isScrap Array of at least 1 Boolean, the first entry will become TRUE if
 *                the returned view was taken from the scrap heap, FALSE if otherwise.
 *
 * @return A view displaying the data associated with the specified position
 */
AutoPtr<IView> AbsListView::ObtainView(
    /* [in] */ Int32 position,
    /* [in] */ ArrayOf<Boolean>* isScrap)
{
    (*isScrap)[0] = FALSE;
    AutoPtr<IView> scrapView = mRecycler->GetTransientStateView(position);
    if (scrapView != NULL) {
        return scrapView;
    }
    scrapView = mRecycler->GetScrapView(position);
    AutoPtr<IView> child;
    if (scrapView != NULL) {
        mAdapter->GetView(position, scrapView, THIS_PROBE(IViewGroup), (IView**)&child);
        if (child != NULL) {
            Int32 importantForAccessibility;
            child->GetImportantForAccessibility(&importantForAccessibility);
            if (importantForAccessibility == IView::IMPORTANT_FOR_ACCESSIBILITY_AUTO) {
                child->SetImportantForAccessibility(IView::IMPORTANT_FOR_ACCESSIBILITY_YES);
            }

            if (child != scrapView) {
                mRecycler->AddScrapView(scrapView, position);
                if (mCacheColorHint != 0) {
                    child->SetDrawingCacheBackgroundColor(mCacheColorHint);
                }
            }
            else {
                (*isScrap)[0] = TRUE;
                child->DispatchFinishTemporaryDetach();
            }
        }
        else {
            Logger::E("AbsListView", "Failed to get child view at position %d", position);
            return NULL;
        }
    }
    else {
        mAdapter->GetView(position, NULL, THIS_PROBE(IViewGroup), (IView**)&child);
        if (child != NULL) {
            Int32 importantForAccessibility;
            child->GetImportantForAccessibility(&importantForAccessibility);
            if (importantForAccessibility == IView::IMPORTANT_FOR_ACCESSIBILITY_AUTO) {
                child->SetImportantForAccessibility(IView::IMPORTANT_FOR_ACCESSIBILITY_YES);
            }

            if (mCacheColorHint != 0) {
                child->SetDrawingCacheBackgroundColor(mCacheColorHint);
            }
        }
        else {
            Logger::E("AbsListView", "Failed to get child view at position %d", position);
            return NULL;
        }
    }

    if (mAdapterHasStableIds) {
        AutoPtr<IViewGroupLayoutParams> vlp;
        child->GetLayoutParams((IViewGroupLayoutParams**)&vlp);
        AutoPtr<IAbsListViewLayoutParams> lp;
        AutoPtr<IViewGroupLayoutParams> lpTemp;
        if (vlp == NULL) {
            GenerateDefaultLayoutParams((IViewGroupLayoutParams**)&lpTemp);
            lp = IAbsListViewLayoutParams::Probe(lpTemp);
        }
        else if (!CheckLayoutParams(vlp)) {
            AutoPtr<IViewGroupLayoutParams> vglp = GenerateLayoutParams(vlp);
            lp = IAbsListViewLayoutParams::Probe(vglp);
        }
        else {
            lp = IAbsListViewLayoutParams::Probe(vlp);
        }
        Int64 itemId;
        mAdapter->GetItemId(position, &itemId);
        lp->SetItemId(itemId);
        child->SetLayoutParams(lp.Get());
    }

    AutoPtr<IAccessibilityManager> manager;
    CAccessibilityManager::GetInstance(mContext, (IAccessibilityManager**)&manager);
    Boolean enable = TRUE;
    manager->IsEnabled(&enable);
    if (enable) {
        if (mAccessibilityDelegate == NULL) {
            mAccessibilityDelegate = new ListItemAccessibilityDelegate(this);
        }
        AutoPtr<IAccessibilityDelegate> delegate;
        child->GetAccessibilityDelegate((IAccessibilityDelegate**)&delegate);
        if (delegate == NULL) {
            child->SetAccessibilityDelegate(mAccessibilityDelegate);
        }
    }
    return child;
}

void AbsListView::PositionSelector(
    /* [in] */ Int32 position,
    /* [in] */ IView* sel)
{
    if (position != IAdapterView::INVALID_POSITION) {
        mSelectorPosition = position;
    }

    assert(sel);
    Int32 left, top, right, bottom;
    sel->GetLeft(&left);
    sel->GetTop(&top);
    sel->GetRight(&right);
    sel->GetBottom(&bottom);
    mSelectorRect->Set(left, top, right, bottom);

    AutoPtr<ISelectionBoundsAdjuster> adjuster = ISelectionBoundsAdjuster::Probe(sel);
    if (adjuster != NULL) {
        adjuster->AdjustListItemSelectionBounds(mSelectorRect);
    }

    PositionSelector(left, top, right, bottom);

    Boolean viewEnabled;
    sel->IsEnabled(&viewEnabled);
    if (viewEnabled != mIsChildViewEnabled) {
        mIsChildViewEnabled = !mIsChildViewEnabled;
        if (GetSelectedItemPosition() != IAdapterView::INVALID_POSITION) {
            RefreshDrawableState();
        }
    }
}

void AbsListView::PositionSelector(
    /* [in] */ Int32 l,
    /* [in] */ Int32 t,
    /* [in] */ Int32 r,
    /* [in] */ Int32 b)
{
    mSelectorRect->Set(
        l - mSelectionLeftPadding, t - mSelectionTopPadding,
        r + mSelectionRightPadding, b + mSelectionBottomPadding);
}

//@Override
void AbsListView::DispatchDraw(
    /* [in] */ ICanvas* canvas)
{
    Int32 saveCount = 0;
    Boolean clipToPadding = (mGroupFlags & ViewGroup::CLIP_TO_PADDING_MASK) == ViewGroup::CLIP_TO_PADDING_MASK;
    if (clipToPadding) {
        canvas->Save(&saveCount);

        Boolean result;
        canvas->ClipRect(
            mScrollX + mPaddingLeft, mScrollY + mPaddingTop,
            mScrollX + mRight - mLeft - mPaddingRight,
            mScrollY + mBottom - mTop - mPaddingBottom,
            &result);
        mGroupFlags &= ~ViewGroup::CLIP_TO_PADDING_MASK;
    }

    if (!mDrawSelectorOnTop) {
        DrawSelector(canvas);
    }

    AdapterView::DispatchDraw(canvas);

    if (mDrawSelectorOnTop) {
        DrawSelector(canvas);
    }

    if (clipToPadding) {
        canvas->RestoreToCount(saveCount);
        mGroupFlags |= ViewGroup::CLIP_TO_PADDING_MASK;
    }
}

//@Override
Boolean AbsListView::IsPaddingOffsetRequired()
{
    return (mGroupFlags & ViewGroup::CLIP_TO_PADDING_MASK) != ViewGroup::CLIP_TO_PADDING_MASK;
}

Int32 AbsListView::GetLeftPaddingOffset()
{
    return (mGroupFlags & ViewGroup::CLIP_TO_PADDING_MASK) == ViewGroup::CLIP_TO_PADDING_MASK ? 0 : -mPaddingLeft;
}

//@Override
Int32 AbsListView::GetTopPaddingOffset()
{
    return (mGroupFlags & ViewGroup::CLIP_TO_PADDING_MASK) == ViewGroup::CLIP_TO_PADDING_MASK ? 0 : -mPaddingTop;
}

Int32 AbsListView::GetRightPaddingOffset()
{
    return (mGroupFlags & ViewGroup::CLIP_TO_PADDING_MASK) == ViewGroup::CLIP_TO_PADDING_MASK ? 0 : mPaddingRight;
}

//@Override
Int32 AbsListView::GetBottomPaddingOffset()
{
    return (mGroupFlags & ViewGroup::CLIP_TO_PADDING_MASK) == ViewGroup::CLIP_TO_PADDING_MASK ? 0 : mPaddingBottom;
}

//@Override
void AbsListView::OnSizeChanged(
    /* [in] */ Int32 w,
    /* [in] */ Int32 h,
    /* [in] */ Int32 oldw,
    /* [in] */ Int32 oldh)
{
    if (GetChildCount() > 0) {
        mDataChanged = TRUE;
        RememberSyncState();
    }

    if (mFastScroller != NULL) {
        mFastScroller->OnSizeChanged(w, h, oldw, oldh);
    }
}

/**
 * @return True if the current touch mode requires that we draw the selector in the pressed
 *         state.
 */
Boolean AbsListView::TouchModeDrawsInPressedState()
{
    // FIXME use isPressed for this
    switch (mTouchMode) {
    case TOUCH_MODE_TAP:
    case TOUCH_MODE_DONE_WAITING:
        return TRUE;
    default:
        return FALSE;
    }
}

/**
 * Indicates whether this view is in a state where the selector should be drawn. This will
 * happen if we have focus but are not in touch mode, or we are in the middle of displaying
 * the pressed state for an item.
 *
 * @return True if the selector should be shown
 */
Boolean AbsListView::ShouldShowSelector()
{
    return (HasFocus() && !IsInTouchMode()) || TouchModeDrawsInPressedState();
}

void AbsListView::DrawSelector(
    /* [in] */ ICanvas* canvas)
{
    Boolean empty;
    mSelectorRect->IsEmpty(&empty);
    if (!empty)
    {
         mSelector->SetBounds(mSelectorRect);
         mSelector->Draw(canvas);
    }
}

/**
 * Controls whether the selection highlight drawable should be drawn on top of the item or
 * behind it.
 *
 * @param onTop If TRUE, the selector will be drawn on the item it is highlighting. The default
 *        is FALSE.
 *
 * @attr ref android.R.styleable#AbsListView_drawSelectorOnTop
 */
ECode AbsListView::SetDrawSelectorOnTop(
    /* [in] */ Boolean onTop)
{
    mDrawSelectorOnTop = onTop;

    return NOERROR;
}

/**
 * Set a Drawable that should be used to highlight the currently selected item.
 *
 * @param resID A Drawable resource to use as the selection highlight.
 *
 * @attr ref android.R.styleable#AbsListView_listSelector
 */
ECode AbsListView::SetSelector(
    /* [in] */ Int32 resID)
{
    AutoPtr<IDrawable> drawable;
    GetResources()->GetDrawable(resID, (IDrawable**)&drawable);

    return SetSelector(drawable);
}

ECode AbsListView::SetSelector(
    /* [in] */ IDrawable* sel)
{
    if (mSelector != NULL) {
        mSelector->SetCallback(NULL);
        UnscheduleDrawable(mSelector);
    }

    mSelector = sel;
    AutoPtr<IRect> padding;
    FAIL_RETURN(CRect::New((IRect**)&padding));
    if (sel == NULL) {
        return NOERROR;
    }

    Boolean result;
    sel->GetPadding(padding, &result);

    padding->GetLeft(&mSelectionLeftPadding);
    padding->GetTop(&mSelectionTopPadding);
    padding->GetRight(&mSelectionRightPadding);
    padding->GetBottom(&mSelectionBottomPadding);
    sel->SetCallback(THIS_PROBE(IDrawableCallback));
    UpdateSelectorState();

    return NOERROR;
}

/**
 * Returns the selector {@link android.graphics.drawable.Drawable} that is used to draw the
 * selection in the list.
 *
 * @return the drawable used to display the selector
 */
AutoPtr<IDrawable> AbsListView::GetSelector()
{
    return mSelector;
}

/**
 * Sets the selector state to "pressed" and posts a CheckForKeyLongPress to see if
 * this is a Int64 press.
 */
void AbsListView::KeyPressed()
{
    if (!IsEnabled() || !IsClickable()) {
        return;
    }

    AutoPtr<IDrawable> selector = mSelector;
    AutoPtr<IRect> selectorRect = mSelectorRect;
    Boolean rectEmpty;
    selectorRect->IsEmpty(&rectEmpty);
    if (selector != NULL && (IsFocused() || TouchModeDrawsInPressedState())
        && !rectEmpty) {
        AutoPtr<IView> v = GetChildAt(mSelectedPosition - mFirstPosition);
        if (v != NULL) {
            Boolean hasfoucsable;
            v->HasFocusable(&hasfoucsable);
            if (hasfoucsable) {
                return;
            }

            v->SetPressed(TRUE);
        }

        SetPressed(TRUE);

        Boolean longClickable = IsLongClickable();
        AutoPtr<IDrawable> d;
        selector->GetCurrent((IDrawable**)&d);
        AutoPtr<ITransitionDrawable> td = ITransitionDrawable::Probe(d);
        AutoPtr<IViewConfigurationHelper> viewConfiguration;
        CViewConfigurationHelper::AcquireSingleton((IViewConfigurationHelper**)&viewConfiguration);
        Int32 timeout;
        viewConfiguration->GetLongPressTimeout(&timeout);

        if (td != NULL) {
            if (longClickable) {
                td->StartTransition(timeout);
            } else {
                td->ResetTransition();
            }
        }

        if (longClickable && !mDataChanged) {
            if (mPendingCheckForKeyLongPress == NULL) {
                mPendingCheckForKeyLongPress = new CheckForKeyLongPress(this);
            }

            mPendingCheckForKeyLongPress->RememberWindowAttachCount();
            PostDelayed(mPendingCheckForKeyLongPress,timeout);
        }
    }
}

ECode AbsListView::SetScrollIndicators(
    /* [in] */ IView* up,
    /* [in] */ IView* down)
{
    mScrollUp = up;
    mScrollDown = down;

    return NOERROR;
}

void AbsListView::UpdateSelectorState()
{
    if (mSelector != NULL) {
        Boolean result;
        if (ShouldShowSelector()) {
            AutoPtr<ArrayOf<Int32> > drawableState = GetDrawableState();
            mSelector->SetState(drawableState, &result);
        } else {
            mSelector->SetState(StateSet::NOTHING, &result);
        }
    }
}

//@Override
ECode AbsListView::DrawableStateChanged()
{
    FAIL_RETURN(AdapterView::DrawableStateChanged());
    UpdateSelectorState();

    return NOERROR;
}

//@Override
ECode AbsListView::OnCreateDrawableState(
    /* [in] */ Int32 extraSpace,
    /* [out] */ ArrayOf<Int32>** drawableState)
{
    VALIDATE_NOT_NULL(drawableState);
    *drawableState = NULL;

    AutoPtr<ArrayOf<Int32> > state;

    // If the child view is enabled then do the default behavior.
    if (mIsChildViewEnabled) {
        // Common case
        FAIL_RETURN(AdapterView::OnCreateDrawableState(extraSpace, (ArrayOf<Int32>**)&state));
        *drawableState = state;
        REFCOUNT_ADD(*drawableState);
        return NOERROR;
    }

    // The selector uses this View's drawable state. The selected child view
    // is disabled, so we need to remove the enabled state from the drawable
    // states.
    Int32 enabledState = (*ENABLED_STATE_SET)[0];

    // If we don't have any extra space, it will return one of the static state arrays,
    // and clearing the enabled state on those arrays is a bad thing!  If we specify
    // we need extra space, it will create+copy into a new array that safely mutable.
    //
    state = NULL;
    AdapterView::OnCreateDrawableState(extraSpace + 1, (ArrayOf<Int32>**)&state);
    Int32 enabledPos = -1;
    if (state != NULL) {
        for (Int32 i = state->GetLength() - 1; i >= 0; i--) {
            if ((*state)[i] == enabledState) {
                enabledPos = i;
                break;
            }
        }
    }

    // Remove the enabled state
    if (enabledPos >= 0) {
        //System.arraycopy(state, enabledPos + 1, state, enabledPos,
        //        state.length - enabledPos - 1);
        state->Copy(enabledPos, state, enabledPos + 1, state->GetLength() - enabledPos - 1);
    }

    *drawableState = state;
    REFCOUNT_ADD(*drawableState);
    return NOERROR;
}

//@Override
Boolean AbsListView::VerifyDrawable(
    /* [in] */ IDrawable* dr)
{
    return mSelector.Get() == dr || AdapterView::VerifyDrawable(dr);
}

ECode AbsListView::JumpDrawablesToCurrentState()
{
    FAIL_RETURN(AdapterView::JumpDrawablesToCurrentState());
    if (mSelector != NULL)
        mSelector->JumpToCurrentState();
    return NOERROR;
}

//@Override
ECode AbsListView::OnAttachedToWindow()
{
    AdapterView::OnAttachedToWindow();

    AutoPtr<IViewTreeObserver> treeObserver = GetViewTreeObserver();
    treeObserver->AddOnTouchModeChangeListener(THIS_PROBE(IOnTouchModeChangeListener));
    if (mTextFilterEnabled && mPopup != NULL && !mGlobalLayoutListenerAddedFilter) {
        treeObserver->AddOnGlobalLayoutListener(THIS_PROBE(IOnGlobalLayoutListener));
    }

    if (mAdapter != NULL && mDataSetObserver == NULL) {
        mDataSetObserver = new AdapterDataSetObserver(this);
        mAdapter->RegisterDataSetObserver(mDataSetObserver);

        // Data may have changed while we were detached. Refresh.
        mDataChanged = TRUE;
        mOldItemCount = mItemCount;
        mAdapter->GetCount(&mItemCount);
    }
    mIsAttached = TRUE;
    return NOERROR;
}

//@Override
ECode AbsListView::OnDetachedFromWindow()
{
    AdapterView::OnDetachedFromWindow();

    // Dismiss the popup in case onSaveInstanceState() was not invoked
    DismissPopup();

    // Detach any view left in the scrap heap
    mRecycler->Clear();

    AutoPtr<IViewTreeObserver> treeObserver = GetViewTreeObserver();
    treeObserver->RemoveOnTouchModeChangeListener(THIS_PROBE(IOnTouchModeChangeListener));
    if (mTextFilterEnabled && mPopup != NULL) {
        treeObserver->RemoveGlobalOnLayoutListener(THIS_PROBE(IOnGlobalLayoutListener));
        mGlobalLayoutListenerAddedFilter = FALSE;
    }

    if (mAdapter != NULL) {
        mAdapter->UnregisterDataSetObserver(mDataSetObserver);
        mDataSetObserver = NULL;
    }

    if (mScrollStrictSpan != NULL) {
        mScrollStrictSpan->Finish();
        mScrollStrictSpan = NULL;
    }

    if (mFlingStrictSpan != NULL) {
        mFlingStrictSpan->Finish();
        mFlingStrictSpan = NULL;
    }

    if (mFlingRunnable != NULL) {
        RemoveCallbacks(mFlingRunnable);
    }

    if (mPositionScroller != NULL) {
        mPositionScroller->Stop();
    }

    if (mClearScrollingCache != NULL) {
        RemoveCallbacks(mClearScrollingCache);
    }

    if (mPerformClick != NULL) {
        RemoveCallbacks(mPerformClick);
    }

    if (mTouchModeReset != NULL) {
        RemoveCallbacks(mTouchModeReset);
        mTouchModeReset = NULL;
    }
    mIsAttached = FALSE;
    return NOERROR;
}

//@Override
ECode AbsListView::OnWindowFocusChanged(
    /* [in] */ Boolean hasWindowFocus)
{
    FAIL_RETURN(AdapterView::OnWindowFocusChanged(hasWindowFocus));

    Int32 touchMode = IsInTouchMode() ? TOUCH_MODE_ON : TOUCH_MODE_OFF;

    if (!hasWindowFocus) {
        SetChildrenDrawingCacheEnabled(FALSE);
        if (mFlingRunnable != NULL) {
            RemoveCallbacks(mFlingRunnable);
            // let the fling runnable report it's new state which
            // should be idle
            mFlingRunnable->EndFling();

            if (mPositionScroller != NULL) {
                mPositionScroller->Stop();
            }

            if (mScrollY != 0) {
                mScrollY = 0;

                InvalidateParentCaches();

                FinishGlows();
                Invalidate();
            }
        }
        // Always hide the type filter
        DismissPopup();

        if (touchMode == TOUCH_MODE_OFF) {
            // Remember the last selected element
            mResurrectToPosition = mSelectedPosition;
        }
    }
    else {
        if (mFiltered && !mPopupHidden) {
            // Show the type filter only if a filter is in effect
            ShowPopup();
        }

        // If we changed touch mode since the last time we had focus
        if (touchMode != mLastTouchMode && mLastTouchMode != TOUCH_MODE_UNKNOWN) {
            // If we come back in trackball mode, we bring the selection back
            if (touchMode == TOUCH_MODE_OFF) {
                // This will trigger a layout
                ResurrectSelection();

            // If we come back in touch mode, then we want to hide the selector
            }
            else {
                HideSelector();
                mLayoutMode = LAYOUT_NORMAL;
                LayoutChildren();
            }
        }
    }

    mLastTouchMode = touchMode;

    return NOERROR;
}

/**
 * Creates the ContextMenuInfo returned from {@link #getContextMenuInfo()}. This
 * methods knows the view, position and ID of the item that received the
 * Int64 press.
 *
 * @param view The view that received the Int64 press.
 * @param position The position of the item that received the Int64 press.
 * @param id The ID of the item that received the Int64 press.
 * @return The extra information that should be returned by
 *         {@link #getContextMenuInfo()}.
 */
AutoPtr<IContextMenuInfo> AbsListView::CreateContextMenuInfo(
    /* [in] */ IView* view,
    /* [in] */ Int32 position,
    /* [in] */ Int64 id)
{
    return new AdapterView::AdapterContextMenuInfo(view, position, id);
}

Boolean AbsListView::PerformLongPress(
    /* [in] */ IView* child,
    /* [in] */ Int32 longPressPosition,
    /* [in] */ Int64 longPressId)
{
    // CHOICE_MODE_MULTIPLE_MODAL takes over long press.
    if (mChoiceMode == IAbsListView::CHOICE_MODE_MULTIPLE_MODAL) {
        if (mChoiceActionMode == NULL &&
            (mChoiceActionMode = StartActionMode(mMultiChoiceModeCallback)) != NULL) {
                SetItemChecked(longPressPosition, TRUE);
                PerformHapticFeedback(IHapticFeedbackConstants::LONG_PRESS);
        }
        return TRUE;
    }

    Boolean handled = FALSE;
    if (mOnItemLongClickListener != NULL) {
        mOnItemLongClickListener->OnItemLongClick(
            (IAdapterView*)this->Probe(EIID_IAdapterView),
            child, longPressPosition, longPressId, &handled);
    }
    if (!handled) {
        mContextMenuInfo = CreateContextMenuInfo(child, longPressPosition, longPressId);
        handled = AdapterView::ShowContextMenuForChild(THIS_PROBE(IView));
    }

    if (handled) {
        PerformHapticFeedback(IHapticFeedbackConstants::LONG_PRESS);
    }

    return handled;
}

//@Override
AutoPtr<IContextMenuInfo> AbsListView::GetContextMenuInfo()
{
    return mContextMenuInfo;
}

Boolean AbsListView::ShowContextMenu()
{
    return View::ShowContextMenu();
}
/** @hide */
//@Override
Boolean AbsListView::ShowContextMenu(
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Int32 metaState)
{
    Int32 position = PointToPosition((Int32)x, (Int32)y);
    if (position != IAdapterView::INVALID_POSITION) {
        Int64 id;
        mAdapter->GetItemId(position, &id);
        AutoPtr<IView> child = GetChildAt(position - mFirstPosition);
        if (child != NULL) {
            mContextMenuInfo = CreateContextMenuInfo(child, position, id);
            return AdapterView::ShowContextMenuForChild(THIS_PROBE(IView));
        }
    }
    return AdapterView::ShowContextMenu(x, y, metaState);

}

//@Override
Boolean AbsListView::ShowContextMenuForChild(
    /* [in] */ IView* originalView)
{
    Int32 longPressPosition = GetPositionForView(originalView);
    if (longPressPosition >= 0) {
        Int64 longPressId;
        mAdapter->GetItemId(longPressPosition, &longPressId);

        Boolean handled = FALSE;
        if (mOnItemLongClickListener != NULL) {
            mOnItemLongClickListener->OnItemLongClick(
                (IAdapterView*)this->Probe(EIID_IAdapterView),
                originalView, longPressPosition, longPressId, &handled);
        }

        if (!handled) {
            AutoPtr<IView> child = GetChildAt(
                longPressPosition - mFirstPosition);

            mContextMenuInfo = CreateContextMenuInfo(
                child, longPressPosition, longPressId);

            handled = AdapterView::ShowContextMenuForChild(originalView);
        }

        return handled;
    }
    return FALSE;
}

//@Override
Boolean AbsListView::OnKeyDown(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    return FALSE;
}

//@Override
Boolean AbsListView::OnKeyUp(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    switch (keyCode) {
        case IKeyEvent::KEYCODE_DPAD_CENTER:
        case IKeyEvent::KEYCODE_ENTER: {
            if (!IsEnabled()) {
                return TRUE;
            }

            Int32 count = -1;
            if (mAdapter != NULL) {
                mAdapter->GetCount(&count);
            }

            if (IsClickable() && IsPressed() &&
                mSelectedPosition >= 0 && mSelectedPosition < count) {
                    AutoPtr<IView> view =
                        GetChildAt(mSelectedPosition - mFirstPosition);
                    if (view != NULL) {
                        PerformItemClick(
                            view, mSelectedPosition, mSelectedRowId);
                        view->SetPressed(FALSE);
                    }
                    SetPressed(FALSE);
                    return TRUE;
            }
        }
        break;
    }

    return AdapterView::OnKeyUp(keyCode, event);
}

//@Override
void AbsListView::DispatchSetPressed(
    /* [in] */ Boolean pressed)
{
    // Don't dispatch setPressed to our children. We call setPressed on ourselves to
    // get the selector in the right state, but we don't want to press each child.
}

/**
 * Maps a point to a position in the list.
 *
 * @param x X in local coordinate
 * @param y Y in local coordinate
 * @return The position of the item which contains the specified point, or
 *         {@link #IAdapterView::INVALID_POSITION} if the point does not intersect an item.
 */
Int32 AbsListView::PointToPosition(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    AutoPtr<IRect> frame = mTouchFrame;
    if (frame == NULL) {
        mTouchFrame = NULL;
        FAIL_RETURN(CRect::New((IRect**)&mTouchFrame));
        frame = mTouchFrame;
    }

    Int32 count = GetChildCount();
    for (Int32 i = count - 1; i >= 0; i--) {
        AutoPtr<IView> child = GetChildAt(i);
        Int32 visibility;
        child->GetVisibility(&visibility);
        if (visibility == IView::VISIBLE) {
            child->GetHitRect(frame);
            Boolean isContains;
            frame->Contains(x, y, &isContains);
            if (isContains) {
                return mFirstPosition + i;
            }
        }
    }

    return IAdapterView::INVALID_POSITION;
}

/**
 * Maps a point to a the rowId of the item which intersects that point.
 *
 * @param x X in local coordinate
 * @param y Y in local coordinate
 * @return The rowId of the item which contains the specified point, or {@link #IAdapterView::INVALID_ROW_ID}
 *         if the point does not intersect an item.
 */
Int64 AbsListView::PointToRowId(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    Int32 position = PointToPosition(x, y);
    Int64 itemId = IAdapterView::INVALID_ROW_ID;

    if (position >= 0) {
        mAdapter->GetItemId(position, &itemId);
    }

    return itemId;
}

Boolean AbsListView::StartScrollIfNeeded(
    /* [in] */ Int32 y)
{
    // Check if we have moved far enough that it looks more like a
    // scroll than a tap
    Int32 deltaY = y - mMotionY;
    Int32 distance = Elastos::Core::Math::Abs(deltaY);
    Boolean overscroll = mScrollY != 0;

    if (overscroll || distance > mTouchSlop) {
        CreateScrollingCache();
        if (overscroll) {
            mTouchMode = TOUCH_MODE_OVERSCROLL;
            mMotionCorrection = 0;
        }
        else {
            mTouchMode = TOUCH_MODE_SCROLL;
            mMotionCorrection = deltaY > 0 ? mTouchSlop : -mTouchSlop;
        }

        AutoPtr<IHandler> handler = GetHandler();
        // Handler should not be NULL unless the AbsListView is not attached to a
        // window, which would make it very hard to scroll it... but the monkeys
        // say it's possible.
        if (handler != NULL && mPendingCheckForLongPress!= NULL) {
            handler->RemoveCallbacks(mPendingCheckForLongPress);
        }

        SetPressed(FALSE);
        AutoPtr<IView> motionView = GetChildAt(mMotionPosition - mFirstPosition);
        if (motionView != NULL) {
            motionView->SetPressed(FALSE);
        }

        ReportScrollStateChange(IAbsListViewOnScrollListener::SCROLL_STATE_TOUCH_SCROLL);
        // Time to start stealing events! Once we've stolen them, don't let anyone
        // steal from us

        AutoPtr<IViewParent> parent = GetParent();
        if (parent != NULL) {
            parent->RequestDisallowInterceptTouchEvent(TRUE);
        }
        ScrollIfNeeded(y);

        return TRUE;
    }

    return FALSE;
}

void AbsListView::ScrollIfNeeded(
    /* [in] */ Int32 y)
{
    using Elastos::Core::Math;
    Int32 rawDeltaY = y - mMotionY;
    Int32 deltaY = rawDeltaY - mMotionCorrection;
    Int32 incrementalDeltaY = mLastY != Math::INT32_MIN_VALUE ? y - mLastY : deltaY;
    Boolean temp;
    if (mTouchMode == TOUCH_MODE_SCROLL) {
        if (PROFILE_SCROLLING) {
            if (!mScrollProfilingStarted) {
//                Debug.startMethodTracing("AbsListViewScroll");
                mScrollProfilingStarted = TRUE;
            }
        }

        if (mScrollStrictSpan == NULL) {
            // If it's non-null, we're already in a scroll.
            AutoPtr<IStrictMode> helper;
            CStrictMode::AcquireSingleton((IStrictMode**)&helper);
            helper->EnterCriticalSpan(String("AbsListView-scroll"),
                (IStrictModeSpan**)&mScrollStrictSpan);
        }

        if (y != mLastY) {
            // We may be here after stopping a fling and continuing to scroll.
            // If so, we haven't disallowed intercepting touch events yet.
            // Make sure that we do so in case we're in a parent that can intercept.
            if ((mGroupFlags & FLAG_DISALLOW_INTERCEPT) == 0
                && Math::Abs(rawDeltaY) > mTouchSlop) {
                AutoPtr<IViewParent> parent = GetParent();
                if (parent != NULL) {
                    parent->RequestDisallowInterceptTouchEvent(TRUE);
                }
            }

            Int32 motionIndex;
            if (mMotionPosition >= 0) {
                motionIndex = mMotionPosition - mFirstPosition;
            } else {
                // If we don't have a motion position that we can reliably track,
                // pick something in the middle to make a best guess at things below.
                motionIndex = GetChildCount() / 2;
            }

            Int32 motionViewPrevTop = 0;
            AutoPtr<IView> motionView = this->GetChildAt(motionIndex);
            if (motionView != NULL) {
                 motionView->GetTop(&  motionViewPrevTop);
            }

            // No need to do all this work if we're not going to move anyway
            Boolean atEdge = FALSE;
            if (incrementalDeltaY != 0) {
                atEdge = TrackMotionScroll(deltaY, incrementalDeltaY);
            }

            // Check to see if we have bumped into the scroll limit
            motionView = this->GetChildAt(motionIndex);
            if (motionView != NULL) {
                // Check if the top of the motion view is where it is
                // supposed to be
                Int32 motionViewRealTop;
                motionView->GetTop(&motionViewRealTop);
                if (atEdge) {
                    // Apply overscroll

                    Int32 overscroll = -incrementalDeltaY -
                        (motionViewRealTop - motionViewPrevTop);
                    OverScrollBy(0, overscroll, 0, mScrollY, 0, 0,
                        0, mOverscrollDistance, TRUE);
                    if (Math::Abs(mOverscrollDistance) == Math::Abs(mScrollY)) {
                        // Don't allow overfling if we're at the edge.
                        if (mVelocityTracker != NULL) {
                            mVelocityTracker->Clear();
                        }
                    }

                    Int32 overscrollMode = GetOverScrollMode();
                    if (overscrollMode == IView::OVER_SCROLL_ALWAYS ||
                        (overscrollMode == IView::OVER_SCROLL_IF_CONTENT_SCROLLS
                            && !ContentFits())) {
                        mDirection = 0; // Reset when entering overscroll.
                        mTouchMode = TOUCH_MODE_OVERSCROLL;
                        if (rawDeltaY > 0) {
                            mEdgeGlowTop->OnPull((Float) overscroll / GetHeight());
                            mEdgeGlowBottom->IsFinished(&temp);
                            if (!temp) {
                                mEdgeGlowBottom->OnRelease();
                            }
                            AutoPtr<IRect> rect;
                            mEdgeGlowTop->GetBounds(FALSE, (IRect**)&rect);
                            Invalidate(rect);
                        }
                        else if (rawDeltaY < 0) {
                            mEdgeGlowBottom->OnPull((Float) overscroll / GetHeight());
                            mEdgeGlowTop->IsFinished(&temp);
                            if (!temp) {
                                mEdgeGlowTop->OnRelease();
                            }
                            AutoPtr<IRect> rect;
                            mEdgeGlowBottom->GetBounds(TRUE, (IRect**)&rect);
                            Invalidate(rect);
                        }
                    }
                }
                mMotionY = y;
            }
            mLastY = y;
        }
    }
    else if (mTouchMode == TOUCH_MODE_OVERSCROLL) {
        if (y != mLastY) {
            Int32 oldScroll = mScrollY;
            Int32 newScroll = oldScroll - incrementalDeltaY;
            Int32 newDirection = y > mLastY ? 1 : -1;

            if (mDirection == 0) {
                mDirection = newDirection;
            }

            Int32 overScrollDistance = -incrementalDeltaY;
            if ((newScroll < 0 && oldScroll >= 0) || (newScroll > 0 && oldScroll <= 0)) {
                overScrollDistance = -oldScroll;
                incrementalDeltaY += overScrollDistance;
            } else {
                incrementalDeltaY = 0;
            }

            if (overScrollDistance != 0) {
                OverScrollBy(0, overScrollDistance, 0, mScrollY, 0, 0,
                    0, mOverscrollDistance, TRUE);
                Int32 overscrollMode = GetOverScrollMode();
                if (overscrollMode == IView::OVER_SCROLL_ALWAYS ||
                    (overscrollMode == IView::OVER_SCROLL_IF_CONTENT_SCROLLS &&
                    !ContentFits())) {
                        if (rawDeltaY > 0) {
                            mEdgeGlowTop->OnPull((Float) overScrollDistance / GetHeight());
                            mEdgeGlowBottom->IsFinished(&temp);
                            if (!temp) {
                                mEdgeGlowBottom->OnRelease();
                            }
                            AutoPtr<IRect> rect;
                            mEdgeGlowTop->GetBounds(TRUE, (IRect**)&rect);
                            Invalidate(rect);
                        } else if (rawDeltaY < 0) {
                            mEdgeGlowBottom->OnPull((Float) overScrollDistance / GetHeight());
                            mEdgeGlowTop->IsFinished(&temp);
                            if (!temp) {
                                mEdgeGlowTop->OnRelease();
                            }
                            AutoPtr<IRect> rect;
                            mEdgeGlowBottom->GetBounds(TRUE, (IRect**)&rect);
                            Invalidate(rect);
                        }
                }
            }

            if (incrementalDeltaY != 0) {
                // Coming back to 'real' list scrolling
                if (mScrollY != 0) {
                    mScrollY = 0;
                    InvalidateParentIfNeeded();
                }

                TrackMotionScroll(incrementalDeltaY, incrementalDeltaY);

                mTouchMode = TOUCH_MODE_SCROLL;

                // We did not scroll the full amount. Treat this essentially like the
                // start of a new touch scroll
                Int32 motionPosition = FindClosestMotionRow(y);

                mMotionCorrection = 0;
                AutoPtr<IView> motionView = GetChildAt(motionPosition - mFirstPosition);
                Int32 viewTop;
                motionView->GetTop(&viewTop);
                mMotionViewOriginalTop = motionView != NULL ? viewTop : 0;
                mMotionY = y;
                mMotionPosition = motionPosition;
            }
            mLastY = y;
            mDirection = newDirection;
        }
    }
}

ECode AbsListView::OnTouchModeChanged(
    /* [in] */ Boolean isInTouchMode)
{
    if (isInTouchMode) {
        // Get rid of the selection when we enter touch mode
        HideSelector();
        // Layout, but only if we already have done so previously.
        // (Otherwise may clobber a LAYOUT_SYNC layout that was requested to restore
        // state.)
        if (GetHeight() > 0 && GetChildCount() > 0) {
            // We do not lose focus initiating a touch (since AbsListView is focusable in
            // touch mode). Force an initial layout to get rid of the selection.
            LayoutChildren();
        }
        UpdateSelectorState();
    }
    else {
        Int32 touchMode = mTouchMode;
        if (touchMode == TOUCH_MODE_OVERSCROLL || touchMode == TOUCH_MODE_OVERFLING) {
            if (mFlingRunnable != NULL) {
                mFlingRunnable->EndFling();
            }
            if (mPositionScroller != NULL) {
                mPositionScroller->Stop();
            }

            if (mScrollY != 0) {
                mScrollY = 0;
                InvalidateParentCaches();
                FinishGlows();
                Invalidate();
            }
        }
    }

    return NOERROR;
}

//@Override
Boolean AbsListView::OnTouchEvent(
    /* [in] */ IMotionEvent* ev)
{
    if (!IsEnabled()) {
        // A disabled view that is clickable still consumes the touch
        // events, it just doesn't respond to them.
        return IsClickable() || IsLongClickable();
    }

    if (mPositionScroller != NULL) {
        mPositionScroller->Stop();
    }

    if (!mIsAttached) {
        // Something isn't right.
        // Since we rely on being attached to get data set change notifications,
        // don't risk doing anything where we might try to resync and find things
        // in a bogus state.
        return FALSE;
    }

    if (mFastScroller != NULL) {
        Boolean intercepted = mFastScroller->OnTouchEvent(ev);
        if (intercepted) {
            return TRUE;
        }
    }

    Int32 action;
    ev->GetAction(&action);

    Float fx, fy;
    ev->GetX(&fx);
    ev->GetY(&fy);

    InitVelocityTrackerIfNotExists();
    mVelocityTracker->AddMovement(ev);

    switch (action & IMotionEvent::ACTION_MASK) {
        case IMotionEvent::ACTION_DOWN: {
            switch (mTouchMode) {
                case TOUCH_MODE_OVERFLING: {
                    mFlingRunnable->EndFling();
                    if (mPositionScroller != NULL) {
                        mPositionScroller->Stop();
                    }
                    mTouchMode = TOUCH_MODE_OVERSCROLL;
                    mMotionX = (Int32) fx;
                    mMotionY = mLastY = (Int32)fy;
                    mMotionCorrection = 0;
                    ev->GetPointerId(0, &mActivePointerId);
                    mDirection = 0;
                    break;
                }

                default: {
                    ev->GetPointerId(0, &mActivePointerId);
                    Int32 x = (Int32)fx;
                    Int32 y = (Int32)fy;
                    Int32 motionPosition = PointToPosition(x, y);

                    if (!mDataChanged) {
                        Boolean isEnabled = (mTouchMode != TOUCH_MODE_FLING) && (motionPosition >= 0);
                        if (isEnabled) {
                            IAdapterView* av = THIS_PROBE(IAdapterView);
                            AutoPtr<IAdapter> adapter;
                            av->GetAdapter((IAdapter**)&adapter);
                            IListAdapter::Probe(adapter)->IsEnabled(motionPosition, &isEnabled);
                        }

                        if (isEnabled) {
                            // User clicked on an actual view (and was not
                            // stopping a fling). It might be a click or a scroll.
                            // Assume it is a click until proven otherwise
                            //
                            mTouchMode = TOUCH_MODE_DOWN;
                            // FIXME Debounce
                            if (mPendingCheckForTap == NULL) {
                                mPendingCheckForTap = new CheckForTap(this);
                            }
                            AutoPtr<IViewConfigurationHelper> viewConfiguration;
                            CViewConfigurationHelper::AcquireSingleton((IViewConfigurationHelper**)&viewConfiguration);
                            Int32 timeout;
                            viewConfiguration->GetTapTimeout(&timeout);
                            PostDelayed(mPendingCheckForTap, timeout);
                        }
                        else {
                            if (mTouchMode == TOUCH_MODE_FLING) {
                                // Stopped a fling. It is a scroll.
                                CreateScrollingCache();
                                mTouchMode = TOUCH_MODE_SCROLL;
                                mMotionCorrection = 0;
                                motionPosition = FindMotionRow(y);
                                mFlingRunnable->FlywheelTouch();
                            }
                        }
                    }

                    if (motionPosition >= 0) {
                        // Remember where the motion event started
                        AutoPtr<IView> v = GetChildAt(motionPosition - mFirstPosition);
                        v->GetTop(&mMotionViewOriginalTop);
                    }
                    mMotionX = x;
                    mMotionY = y;
                    mMotionPosition = motionPosition;
                    mLastY = Elastos::Core::Math::INT32_MIN_VALUE;
                    break;
                }
            } // End Switch (mTouchMode)

            if (PerformButtonActionOnTouchDown(ev)) {
                if (mTouchMode == TOUCH_MODE_DOWN) {
                    RemoveCallbacks(mPendingCheckForTap);
                }
            }
            break;
        }

        case IMotionEvent::ACTION_MOVE: {
            Int32 pointerIndex;
            ev->FindPointerIndex(mActivePointerId, &pointerIndex);
            if (pointerIndex == -1) {
                pointerIndex = 0;
                ev->GetPointerId(pointerIndex,&mActivePointerId);
            }
            Float pify;
            ev->GetY(pointerIndex, &pify);
            Int32 y = (Int32)pify;

            if (mDataChanged) {
                // Re-sync everything if data has been changed
                // since the scroll operation can query the adapter.
                LayoutChildren();
            }

            switch (mTouchMode) {
                case TOUCH_MODE_DOWN:
                case TOUCH_MODE_TAP:
                case TOUCH_MODE_DONE_WAITING:
                    // Check if we have moved far enough that it looks more like a
                    // scroll than a tap
                    StartScrollIfNeeded(y);
                    break;

                case TOUCH_MODE_SCROLL:
                case TOUCH_MODE_OVERSCROLL:
                    ScrollIfNeeded(y);
                    break;
            }
            break;
        }

        case IMotionEvent::ACTION_UP: {
            switch (mTouchMode) {
                case TOUCH_MODE_DOWN:
                case TOUCH_MODE_TAP:
                case TOUCH_MODE_DONE_WAITING: {
                    Int32 motionPosition = mMotionPosition;
                    AutoPtr<IView> child = GetChildAt(motionPosition - mFirstPosition);
                    Boolean inList = fx > mListPadding->mLeft && fx < GetWidth() - mListPadding->mRight;
                    Boolean hasFocusable;
                    if (child != NULL && (child->HasFocusable(&hasFocusable), !hasFocusable) && inList) {
                        if (mTouchMode != TOUCH_MODE_DOWN) {
                            child->SetPressed(FALSE);
                        }

                        if (mPerformClick == NULL) {
                            mPerformClick = new AbsListViewPerformClick(this);
                        }

                        AutoPtr<AbsListViewPerformClick> performClick = mPerformClick;

                        performClick->mClickMotionPosition = motionPosition;
                        performClick->RememberWindowAttachCount();

                        mResurrectToPosition = motionPosition;

                        Boolean isEnabled;
                        mAdapter->IsEnabled(motionPosition, &isEnabled);
                        if (mTouchMode == TOUCH_MODE_DOWN || mTouchMode == TOUCH_MODE_TAP) {
                            AutoPtr<IHandler> handler = GetHandler();
                            if (handler != NULL && mPendingCheckForLongPress != NULL) {
                                AutoPtr<IRunnable> target = (mTouchMode == TOUCH_MODE_DOWN ?
                                        mPendingCheckForTap.Get() : mPendingCheckForLongPress.Get());
                                handler->RemoveCallbacks(target);
                            }

                            mLayoutMode = LAYOUT_NORMAL;
                            if (!mDataChanged && isEnabled) {
                                mTouchMode = TOUCH_MODE_TAP;
                                SetSelectedPositionInt(mMotionPosition);
                                LayoutChildren();
                                child->SetPressed(TRUE);
                                PositionSelector(mMotionPosition, child);
                                SetPressed(TRUE);
                                if (mSelector != NULL) {
                                    AutoPtr<IDrawable> d;
                                    mSelector->GetCurrent((IDrawable**)&d);
                                    AutoPtr<ITransitionDrawable> td = ITransitionDrawable::Probe(d);
                                    if (td != NULL) {
                                        td->ResetTransition();
                                    }
                                }
                                if (mTouchModeReset != NULL) {
                                    RemoveCallbacks(mTouchModeReset);
                                }
                                mTouchModeReset = new TouchModeResetRunnable(performClick, this, child);
                                AutoPtr<IViewConfigurationHelper> viewConfiguration;
                                CViewConfigurationHelper::AcquireSingleton((IViewConfigurationHelper**)&viewConfiguration);
                                Int32 timeout;
                                viewConfiguration->GetPressedStateDuration(&timeout);
                                PostDelayed(mTouchModeReset, timeout);
                            }
                            else {
                                mTouchMode = TOUCH_MODE_REST;
                                UpdateSelectorState();
                            }
                            return TRUE;
                        }
                        else if (!mDataChanged && isEnabled) {
                            performClick->Run();
                        }
                    }
                    mTouchMode = TOUCH_MODE_REST;
                    UpdateSelectorState();
                    break;
                }

                case TOUCH_MODE_SCROLL: {
                    Int32 childCount = GetChildCount();
                    if (childCount > 0) {
                        Int32 firstChildTop;
                        GetChildAt(0)->GetTop(&firstChildTop);
                        Int32 lastChildBottom;
                        GetChildAt(childCount - 1)->GetBottom(&lastChildBottom);

                        Int32 contentTop = mListPadding->mTop;
                        Int32 contentBottom = GetHeight() - mListPadding->mBottom;
                        if (mFirstPosition == 0 && firstChildTop >= contentTop &&
                            mFirstPosition + childCount < mItemCount &&
                            lastChildBottom <= GetHeight() - contentBottom) {
                                mTouchMode = TOUCH_MODE_REST;
                                ReportScrollStateChange(IAbsListViewOnScrollListener::SCROLL_STATE_IDLE);
                        }
                        else {
                            mVelocityTracker->ComputeCurrentVelocity(1000, mMaximumVelocity);
                            Float y;
                            mVelocityTracker->GetYVelocity(mActivePointerId, &y);
                            Int32 initialVelocity = (Int32)(y * mVelocityScale);

                            // Fling if we have enough velocity and we aren't at a boundary.
                            // Since we can potentially overfling more than we can overscroll, don't
                            // allow the weird behavior where you can scroll to a boundary then
                            // fling further.
                            if (Elastos::Core::Math::Abs(initialVelocity) > mMinimumVelocity &&
                                !((mFirstPosition == 0 &&
                                firstChildTop == contentTop - mOverscrollDistance) ||
                                (mFirstPosition + childCount == mItemCount &&
                                lastChildBottom == contentBottom + mOverscrollDistance))) {
                                    if (mFlingRunnable == NULL) {
                                        mFlingRunnable = new FlingRunnable(this);
                                    }
                                    ReportScrollStateChange(IAbsListViewOnScrollListener::SCROLL_STATE_FLING);

                                    mFlingRunnable->Start(-initialVelocity);
                            }
                            else {
                                mTouchMode = TOUCH_MODE_REST;
                                ReportScrollStateChange(IAbsListViewOnScrollListener::SCROLL_STATE_IDLE);

                                if (mFlingRunnable != NULL) {
                                    mFlingRunnable->EndFling();
                                }
                                if (mPositionScroller != NULL) {
                                    mPositionScroller->Stop();
                                }
                            }
                        }
                    }
                    else {
                        mTouchMode = TOUCH_MODE_REST;
                        ReportScrollStateChange(IAbsListViewOnScrollListener::SCROLL_STATE_IDLE);
                    }
                    break;
                }

                case TOUCH_MODE_OVERSCROLL: {
                    if (mFlingRunnable == NULL) {
                        mFlingRunnable = new FlingRunnable(this);
                    }

                    mVelocityTracker->ComputeCurrentVelocity(1000, mMaximumVelocity);
                    Float y;
                    mVelocityTracker->GetYVelocity(mActivePointerId, &y);
                    Int32 initialVelocity = (Int32)y;

                    ReportScrollStateChange(IAbsListViewOnScrollListener::SCROLL_STATE_FLING);
                    if (Elastos::Core::Math::Abs(initialVelocity) > mMinimumVelocity) {
                        mFlingRunnable->StartOverfling(-initialVelocity);
                    }
                    else {
                        mFlingRunnable->StartSpringback();
                    }

                    break;
                }
                default:
                    break;
            }

            SetPressed(FALSE);

            if (mEdgeGlowTop != NULL) {
                mEdgeGlowTop->OnRelease();
                mEdgeGlowBottom->OnRelease();
            }

            // Need to redraw since we probably aren't drawing the selector anymore
            Invalidate();

            AutoPtr<IHandler> handler = GetHandler();
            if (handler != NULL && mPendingCheckForLongPress != NULL) {
                handler->RemoveCallbacks(mPendingCheckForLongPress);
            }

            RecycleVelocityTracker();

            mActivePointerId = INVALID_POINTER;

            if (PROFILE_SCROLLING) {
                if (mScrollProfilingStarted) {
                    //Debug.stopMethodTracing();
                    mScrollProfilingStarted = FALSE;
                }
            }

            if (mScrollStrictSpan != NULL) {
                mScrollStrictSpan->Finish();
                mScrollStrictSpan = NULL;
            }
            break;
        }

        case IMotionEvent::ACTION_CANCEL: {
            switch (mTouchMode) {
                case TOUCH_MODE_OVERSCROLL:
                    if (mFlingRunnable == NULL) {
                        mFlingRunnable = new FlingRunnable(this);
                    }
                    mFlingRunnable->StartSpringback();
                    break;

                case TOUCH_MODE_OVERFLING:
                    // Do nothing - let it play out.
                    break;

                default:
                    mTouchMode = TOUCH_MODE_REST;
                    SetPressed(FALSE);
                    AutoPtr<IView> motionView = GetChildAt(
                        mMotionPosition - mFirstPosition);
                    if (motionView != NULL) {
                        motionView->SetPressed(FALSE);
                    }
                    ClearScrollingCache();

                    AutoPtr<IHandler> handler = GetHandler();
                    if (handler != NULL && mPendingCheckForLongPress != NULL) {
                        AutoPtr<IRunnable> runTemp = mPendingCheckForLongPress.Get();
                        handler->RemoveCallbacks(runTemp);
                    }

                    RecycleVelocityTracker();
                    break;
                }

                if (mEdgeGlowTop != NULL) {
                    mEdgeGlowTop->OnRelease();
                    mEdgeGlowBottom->OnRelease();
                }
                mActivePointerId = INVALID_POINTER;
                break;
            }

        case IMotionEvent::ACTION_POINTER_UP: {
            OnSecondaryPointerUp(ev);
            Int32 x = mMotionX;
            Int32 y = mMotionY;
            Int32 motionPosition = PointToPosition(x, y);
            if (motionPosition >= 0) {
                // Remember where the motion event started
                AutoPtr<IView> v = GetChildAt(motionPosition - mFirstPosition);
                v->GetTop(&mMotionViewOriginalTop);
                mMotionPosition = motionPosition;
            }
            mLastY = y;
            break;
        }

        case IMotionEvent::ACTION_POINTER_DOWN: {
            // New pointers take over dragging duties
            Int32 index;
            ev->GetActionIndex(&index);
            Int32 id;
            ev->GetPointerId(index, &id);
            Int32 x = (Int32)fx;
            Int32 y = (Int32)fy;
            mMotionCorrection = 0;
            mActivePointerId = id;
            mMotionX = x;
            mMotionY = y;
            Int32 motionPosition = PointToPosition(x, y);
            if (motionPosition >= 0) {
                // Remember where the motion event started
                AutoPtr<IView> v = GetChildAt(motionPosition - mFirstPosition);
                v->GetTop(&mMotionViewOriginalTop);
                mMotionPosition = motionPosition;
            }
            mLastY = y;
            break;
        }

        default:
            break;
    }

    return TRUE;
}

//@Override
void AbsListView::OnOverScrolled(
    /* [in] */ Int32 scrollX,
    /* [in] */ Int32 scrollY,
    /* [in] */ Boolean clampedX,
    /* [in] */ Boolean clampedY)
{
    if (mScrollY != scrollY) {
        OnScrollChanged(mScrollX, scrollY, mScrollX, mScrollY);
        mScrollY = scrollY;
        InvalidateParentIfNeeded();

        AwakenScrollBars();
    }
}

Boolean AbsListView::OnGenericMotionEvent(
    /* [in] */ IMotionEvent* event)
{
    Int32 source;
    event->GetSource(&source);
    if ((source & IInputDevice::SOURCE_CLASS_POINTER) != 0) {
        Int32 action;
        event->GetAction(&action);
        switch (action) {
            case IMotionEvent::ACTION_SCROLL: {
                if (mTouchMode == TOUCH_MODE_REST) {
                    Float vscroll;
                    event->GetAxisValue(IMotionEvent::AXIS_VSCROLL, &vscroll);
                    if (vscroll != 0) {
                        Int32 delta = (Int32) (vscroll * GetVerticalScrollFactor());
                        if (!TrackMotionScroll(delta, delta)) {
                            return TRUE;
                        }
                    }
                }
            }
        }
    }
    return AdapterView::OnGenericMotionEvent(event);
}

//@Override
ECode AbsListView::Draw(
    /* [in] */ ICanvas* canvas)
{
    AdapterView::Draw(canvas);
    if (mEdgeGlowTop != NULL) {
        AutoPtr<IRect> rect;
        Boolean temp;
        Int32 scrollY = mScrollY;
        mEdgeGlowTop->IsFinished(&temp);
        if (!temp) {
            Int32 restoreCount;
            canvas->Save(&restoreCount);

            Int32 leftPadding = mListPadding->mLeft + mGlowPaddingLeft;
            Int32 rightPadding = mListPadding->mRight + mGlowPaddingRight;
            Int32 width = GetWidth() - leftPadding - rightPadding;

            Int32 edgeY = Elastos::Core::Math::Min(0, scrollY + mFirstPositionDistanceGuess);
            canvas->Translate(leftPadding, edgeY);
            mEdgeGlowTop->SetSize(width, GetHeight());
            mEdgeGlowTop->Draw(canvas, &temp);
            if (temp) {
                mEdgeGlowTop->SetPosition(leftPadding, edgeY);
                mEdgeGlowTop->GetBounds(FALSE, (IRect**)&rect);
                Invalidate(rect);
            }
            canvas->RestoreToCount(restoreCount);
        }

        mEdgeGlowBottom->IsFinished(&temp);
        if (!temp) {
            Int32 restoreCount;
            canvas->Save(&restoreCount);

            Int32 leftPadding = mListPadding->mLeft + mGlowPaddingLeft;
            Int32 rightPadding = mListPadding->mRight + mGlowPaddingRight;
            Int32 width = GetWidth() - leftPadding - rightPadding;
            Int32 height = GetHeight();

            Int32 edgeX = -width + leftPadding;
            Int32 edgeY = Elastos::Core::Math::Max(height, scrollY + mLastPositionDistanceGuess);
            canvas->Translate(edgeX, edgeY);
            canvas->Rotate(180, width, 0);
            mEdgeGlowBottom->SetSize(width, height);
            mEdgeGlowBottom->Draw(canvas, &temp);
            if (temp) {
                // Account for the rotation
                mEdgeGlowBottom->SetPosition(edgeX + width, edgeY);
                rect = NULL;
                mEdgeGlowBottom->GetBounds(TRUE, (IRect**)&rect);
                Invalidate(rect);
            }
            canvas->RestoreToCount(restoreCount);
        }
    }

    if (mFastScroller != NULL) {
        Int32 scrollY = mScrollY;
        if (scrollY != 0) {
            // Pin to the top/bottom during overscroll
            Int32 restoreCount;
            canvas->Save(&restoreCount);
            canvas->Translate(0, (Float)scrollY);

            mFastScroller->Draw(canvas);
            canvas->RestoreToCount(restoreCount);
        }
        else {
            mFastScroller->Draw(canvas);
        }
    }

    return NOERROR;
}

/**
 * @hide
 */
ECode AbsListView::SetOverScrollEffectPadding(
    /* [in] */ Int32 leftPadding,
    /* [in] */ Int32 rightPadding)
{
    mGlowPaddingLeft = leftPadding;
    mGlowPaddingRight = rightPadding;
    return NOERROR;
}

void AbsListView::InitOrResetVelocityTracker()
{
    if (mVelocityTracker == NULL) {
        mVelocityTracker = VelocityTracker::Obtain();
    } else {
        mVelocityTracker->Clear();
    }
}

void AbsListView::InitVelocityTrackerIfNotExists()
{
    if (mVelocityTracker == NULL) {
        mVelocityTracker = VelocityTracker::Obtain();
    }
}

void AbsListView::RecycleVelocityTracker()
{
    if (mVelocityTracker != NULL) {
        mVelocityTracker->Recycle();
        mVelocityTracker = NULL;
    }
}

ECode AbsListView::RequestDisallowInterceptTouchEvent(
    /* [in] */ Boolean disallowIntercept)
{
    if (disallowIntercept) {
        RecycleVelocityTracker();
    }
    return AdapterView::RequestDisallowInterceptTouchEvent(disallowIntercept);
}

//@Override
Boolean AbsListView::OnInterceptTouchEvent(
    /* [in] */ IMotionEvent* ev)
{
    Int32 action;
    ev->GetAction(&action);

    if (mPositionScroller != NULL) {
        mPositionScroller->Stop();
    }

    if (!mIsAttached) {
        // Something isn't right.
        // Since we rely on being attached to get data set change notifications,
        // don't risk doing anything where we might try to resync and find things
        // in a bogus state.
        return FALSE;
    }

    if (mFastScroller != NULL) {
        Boolean intercepted = mFastScroller->OnInterceptTouchEvent(ev);
        if (intercepted) {
            return TRUE;
        }
    }

    switch (action & IMotionEvent::ACTION_MASK) {
        case IMotionEvent::ACTION_DOWN: {
            Int32 touchMode = mTouchMode;
            if (touchMode == TOUCH_MODE_OVERFLING || touchMode == TOUCH_MODE_OVERSCROLL) {
                mMotionCorrection = 0;
                return TRUE;
            }

            Float fx, fy;
            ev->GetX(&fx);
            ev->GetY(&fy);

            Int32 x = (Int32)fx;
            Int32 y = (Int32)fy;
            ev->GetPointerId(0, &mActivePointerId);

            Int32 motionPosition = FindMotionRow(y);
            if (touchMode != TOUCH_MODE_FLING && motionPosition >= 0) {
                // User clicked on an actual view (and was not stopping a fling).
                // Remember where the motion event started
                //
                AutoPtr<IView>  v = GetChildAt(motionPosition - mFirstPosition);
                v->GetTop(&mMotionViewOriginalTop);
                mMotionX = x;
                mMotionY = y;
                mMotionPosition = motionPosition;
                mTouchMode = TOUCH_MODE_DOWN;
                ClearScrollingCache();
            }

            mLastY = Elastos::Core::Math::INT32_MIN_VALUE;
            InitOrResetVelocityTracker();
            mVelocityTracker->AddMovement(ev);
            if (touchMode == TOUCH_MODE_FLING) {
                return TRUE;
            }
            break;
        }

        case IMotionEvent::ACTION_MOVE: {
            switch (mTouchMode) {
                case TOUCH_MODE_DOWN: {
                    Int32 pointerIndex;
                    ev->FindPointerIndex(mActivePointerId, &pointerIndex);
                    if (pointerIndex == -1) {
                        pointerIndex = 0;
                        ev->GetPointerId(pointerIndex, &mActivePointerId);
                    }
                    Float fy;
                    ev->GetY(pointerIndex, &fy);
                    Int32 y = (Int32)fy;
                    InitVelocityTrackerIfNotExists();
                    mVelocityTracker->AddMovement(ev);
                    if (StartScrollIfNeeded(y)) {
                        return TRUE;
                    }
                    break;
                }
            }

            break;
        }

        case IMotionEvent::ACTION_CANCEL:
        case IMotionEvent::ACTION_UP: {
            mTouchMode = TOUCH_MODE_REST;
            mActivePointerId = INVALID_POINTER;
            RecycleVelocityTracker();
            ReportScrollStateChange(IAbsListViewOnScrollListener::SCROLL_STATE_IDLE);
            break;
        }

        case IMotionEvent::ACTION_POINTER_UP: {
            OnSecondaryPointerUp(ev);
            break;
        }

        default:
            break;
    }

    return FALSE;
}

void AbsListView::OnSecondaryPointerUp(
    /* [in] */ IMotionEvent* ev)
{
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
        Float fx, fy;
        ev->GetX(newPointerIndex, &fx);
        ev->GetY(newPointerIndex, &fy);
        mMotionX = (Int32)fx;
        mMotionY = (Int32)fy;
        mMotionCorrection = 0;
        ev->GetPointerId(newPointerIndex, &mActivePointerId);

    }
}

/**
 * {@inheritDoc}
 */
//@Override
ECode AbsListView::AddTouchables(
    /* [in] */ IObjectContainer* views)
{
    Int32 count = GetChildCount();
    Int32 firstPosition = mFirstPosition;
    AutoPtr<IListAdapter> adapter = mAdapter;

    if (adapter == NULL) {
        return NOERROR;
    }

    for (Int32 i = 0; i < count; i++) {
        AutoPtr<IView> child = GetChildAt(i);
        Boolean isEnabled;
        adapter->IsEnabled(firstPosition + i, &isEnabled);
        if (isEnabled) {
            views->Add(child);
        }
        child->AddTouchables(views);
    }

    return NOERROR;
}

/**
 * Fires an "on scroll state changed" event to the registered
 * {@link android.widget.AbsListView.OnScrollListener}, if any. The state change
 * is fired only if the specified state is different from the previously known state.
 *
 * @param newState The new scroll state.
 */
void AbsListView::ReportScrollStateChange(
    /* [in] */ Int32 newState)
{
    if (newState != mLastScrollState) {
        if (mOnScrollListener != NULL) {

            mLastScrollState = newState;

            mOnScrollListener->OnScrollStateChanged(THIS_PROBE(IAbsListView), newState);
        }
    }
}

/**
 * The amount of friction applied to flings. The default value
 * is {@link ViewConfiguration#getScrollFriction}.
 *
 * @return A scalar dimensionless value representing the coefficient of
 *         friction.
 */
ECode AbsListView::SetFriction(
    /* [in] */ Float friction)
{
    if (mFlingRunnable == NULL) {
        mFlingRunnable = new FlingRunnable(this);
    }
    mFlingRunnable->mScroller->SetFriction(friction);
    return NOERROR;

}

/**
 * Sets a scale factor for the fling velocity. The initial scale
 * factor is 1.0.
 *
 * @param scale The scale factor to multiply the velocity by.
 */
ECode AbsListView::SetVelocityScale(
    /* [in] */ Float scale)
{
    mVelocityScale = scale;
    return NOERROR;
}

/**
 * Smoothly scroll to the specified adapter position. The view will
 * scroll such that the indicated position is displayed.
 * @param position Scroll to this adapter position.
 */
ECode AbsListView::SmoothScrollToPosition(
    /* [in] */ Int32 position)
{
    if (mPositionScroller == NULL) {
        mPositionScroller = new PositionScroller(this);
    }
    mPositionScroller->Start(position);

    return NOERROR;
}

/**
 * Smoothly scroll to the specified adapter position. The view will scroll
 * such that the indicated position is displayed <code>offset</code> pixels from
 * the top edge of the view. If this is impossible, (e.g. the offset would scroll
 * the first or last item beyond the boundaries of the list) it will get as close
 * as possible. The scroll will take <code>duration</code> milliseconds to complete.
 *
 * @param position Position to scroll to
 * @param offset Desired distance in pixels of <code>position</code> from the top
 *               of the view when scrolling is finished
 * @param duration Number of milliseconds to use for the scroll
 */
ECode AbsListView::SmoothScrollToPositionFromTop(
    /* [in] */ Int32 position,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 duration)
{
    if (mPositionScroller == NULL) {
        mPositionScroller = new PositionScroller(this);
    }
    mPositionScroller->StartWithOffset(position, offset, duration);
    return NOERROR;
}

/**
 * Smoothly scroll to the specified adapter position. The view will scroll
 * such that the indicated position is displayed <code>offset</code> pixels from
 * the top edge of the view. If this is impossible, (e.g. the offset would scroll
 * the first or last item beyond the boundaries of the list) it will get as close
 * as possible.
 *
 * @param position Position to scroll to
 * @param offset Desired distance in pixels of <code>position</code> from the top
 *               of the view when scrolling is finished
 */
ECode AbsListView::SmoothScrollToPositionFromTop(
    /* [in] */ Int32 position,
    /* [in] */ Int32 offset)
{
    if (mPositionScroller == NULL) {
        mPositionScroller = new PositionScroller(this);
    }
    mPositionScroller->StartWithOffset(position, offset);
    return NOERROR;
}

/**
 * Smoothly scroll to the specified adapter position. The view will
 * scroll such that the indicated position is displayed, but it will
 * stop early if scrolling further would scroll boundPosition out of
 * view.
 * @param position Scroll to this adapter position.
 * @param boundPosition Do not scroll if it would move this adapter
 *          position out of view.
 */
ECode AbsListView::SmoothScrollToPosition(
    /* [in] */ Int32 position,
    /* [in] */ Int32 boundPosition)
{
    if (mPositionScroller == NULL) {
        mPositionScroller = new PositionScroller(this);
    }
    mPositionScroller->Start(position, boundPosition);

    return NOERROR;
}

/**
 * Smoothly scroll by distance pixels over duration milliseconds.
 * @param distance Distance to scroll in pixels.
 * @param duration Duration of the scroll animation in milliseconds.
 */
ECode AbsListView::SmoothScrollBy(
    /* [in] */ Int32 distance,
    /* [in] */ Int32 duration)
{
    SmoothScrollBy(distance, duration, FALSE);

    return NOERROR;
}

void AbsListView::SmoothScrollBy(
    /* [in] */ Int32 distance,
    /* [in] */ Int32 duration,
    /* [in] */ Boolean linear)
{
    if (mFlingRunnable == NULL) {
        mFlingRunnable = new FlingRunnable(this);
    }

    // No sense starting to scroll if we're not going anywhere
    Int32 firstPos = mFirstPosition;
    Int32 childCount = GetChildCount();
    Int32 lastPos = firstPos + childCount;
    Int32 topLimit = GetPaddingTop();
    Int32 bottomLimit = GetHeight() - GetPaddingBottom();

    Int32 topTemp;
    GetChildAt(0)->GetTop(&topTemp);
    Int32 bottomTemp;
    GetChildAt(childCount - 1)->GetBottom(&bottomTemp);
    if (distance == 0 || mItemCount == 0 || childCount == 0 ||
            (firstPos == 0 && topTemp == topLimit && distance < 0) ||
            (lastPos == mItemCount && bottomTemp== bottomLimit && distance > 0)) {
        mFlingRunnable->EndFling();
        if (mPositionScroller != NULL) {
            mPositionScroller->Stop();
        }
    } else {
        ReportScrollStateChange(IAbsListViewOnScrollListener::SCROLL_STATE_FLING);
        mFlingRunnable->StartScroll(distance, duration, linear);
    }
}

/**
 * Allows RemoteViews to scroll relatively to a position.
 */
ECode AbsListView::SmoothScrollByOffset(
    /* [in] */ Int32 position)
{
    Int32 index = -1;
    if (position < 0) {
        index = GetFirstVisiblePosition();
    } else if (position > 0) {
        index = GetLastVisiblePosition();
    }

    if (index > -1) {
        AutoPtr<IView> child = GetChildAt(index - GetFirstVisiblePosition());
        if (child != NULL) {
            AutoPtr<IRect> visibleRect;
            CRect::New((IRect**)&visibleRect);
            Boolean childVisible;
            if (child->GetGlobalVisibleRect(visibleRect, &childVisible), childVisible) {
                // the child is partially visible
                Int32 childRectArea ;
                child->GetWidth(&childRectArea);
                Int32 childHeight;
                child->GetHeight(&childHeight);
                childRectArea *= childHeight;
                Int32 visibleRectArea;
                visibleRect->GetWidth(&visibleRectArea);
                Int32 visibleHeight;
                visibleRect->GetHeight(&visibleHeight);
                visibleRectArea *= visibleHeight;
                Float visibleArea = (visibleRectArea / (Float) childRectArea);
                Float visibleThreshold = 0.75f;
                if ((position < 0) && (visibleArea < visibleThreshold)) {
                    // the top index is not perceivably visible so offset
                    // to account for showing that top index as well
                    ++index;
                } else if ((position > 0) && (visibleArea < visibleThreshold)) {
                    // the bottom index is not perceivably visible so offset
                    // to account for showing that bottom index as well
                    --index;
                }
            }
            SmoothScrollToPosition(Elastos::Core::Math::Max(0, Elastos::Core::Math::Min(GetCount(), index + position)));
        }
    }
    return NOERROR;
}

void AbsListView::CreateScrollingCache()
{
    if (mScrollingCacheEnabled && !mCachingStarted && !IsHardwareAccelerated()) {
        SetChildrenDrawnWithCacheEnabled(TRUE);
        SetChildrenDrawingCacheEnabled(TRUE);
        mCachingStarted = mCachingActive = TRUE;
    }
}

void AbsListView::ClearScrollingCache()
{
    if (!IsHardwareAccelerated()) {
        if (mClearScrollingCache == NULL) {
            AutoPtr<ClearScrollingCacheRunnable> temp
                = new ClearScrollingCacheRunnable(this);
            mClearScrollingCache = temp;
        }
    }

    Post(mClearScrollingCache);
}

/**
 * Track a motion scroll
 *
 * @param deltaY Amount to offset mMotionView. This is the accumulated delta since the motion
 *        began. Positive numbers mean the user's finger is moving down the screen.
 * @param incrementalDeltaY Change in deltaY from the previous event.
 * @return TRUE if we're already at the beginning/end of the list and have nothing to do.
 */
Boolean AbsListView::TrackMotionScroll(
    /* [in] */ Int32 deltaY,
    /* [in] */ Int32 incrementalDeltaY)
{
    Int32 childCount = GetChildCount();
    if (childCount == 0) {
        return TRUE;
    }

    Int32 firstTop;
    GetChildAt(0)->GetTop(&firstTop);
    Int32 lastBottom;
    GetChildAt(childCount - 1)->GetBottom(&lastBottom);

    // "effective padding" In this case is the amount of padding that affects
    // how much space should not be filled by items. If we don't clip to padding
    // there is no effective padding.
    Int32 effectivePaddingTop = 0;
    Int32 effectivePaddingBottom = 0;
    if ((mGroupFlags & ViewGroup::CLIP_TO_PADDING_MASK) == ViewGroup::CLIP_TO_PADDING_MASK) {
        effectivePaddingTop = mListPadding->mTop;
        effectivePaddingBottom = mListPadding->mBottom;
    }

     // FIXME account for grid vertical spacing too?
    Int32 spaceAbove = effectivePaddingTop - firstTop;
    Int32 end = GetHeight() - effectivePaddingBottom;
    Int32 spaceBelow = lastBottom - end;

    Int32 height = GetHeight() - mPaddingBottom - mPaddingTop;
    if (deltaY < 0) {
        deltaY = Elastos::Core::Math::Max(-(height - 1), deltaY);
    }
    else {
        deltaY = Elastos::Core::Math::Min(height - 1, deltaY);
    }

    if (incrementalDeltaY < 0) {
        incrementalDeltaY = Elastos::Core::Math::Max(-(height - 1), incrementalDeltaY);
    }
    else {
        incrementalDeltaY = Elastos::Core::Math::Min(height - 1, incrementalDeltaY);
    }

    Int32 firstPosition = mFirstPosition;

    // Update our guesses for where the first and last views are
    if (firstPosition == 0) {
        mFirstPositionDistanceGuess = firstTop - mListPadding->mTop;
    }
    else {
        mFirstPositionDistanceGuess += incrementalDeltaY;
    }

    if (firstPosition + childCount == mItemCount) {
        mLastPositionDistanceGuess = lastBottom + mListPadding->mBottom;
    }
    else {
        mLastPositionDistanceGuess += incrementalDeltaY;
    }

    Boolean cannotScrollDown = (firstPosition == 0 &&
        firstTop >= mListPadding->mTop && incrementalDeltaY >= 0);
    Boolean cannotScrollUp = (firstPosition + childCount == mItemCount &&
        lastBottom <= GetHeight() - mListPadding->mBottom && incrementalDeltaY <= 0);

    if (cannotScrollDown || cannotScrollUp) {
        return incrementalDeltaY != 0;
    }

    Boolean down = incrementalDeltaY < 0;
    Boolean inTouchMode = IsInTouchMode();
    if (inTouchMode) {
        HideSelector();
    }

    Int32 headerViewsCount = GetHeaderViewsCount();
    Int32 footerViewsStart = mItemCount - GetFooterViewsCount();

    Int32 start = 0;
    Int32 count = 0;

    if (down) {
        Int32 top = -incrementalDeltaY;
        if ((mGroupFlags & ViewGroup::CLIP_TO_PADDING_MASK) == ViewGroup::CLIP_TO_PADDING_MASK) {
            top += mListPadding->mTop;
        }
        for (Int32 i = 0; i < childCount; i++) {
            AutoPtr<IView> child = GetChildAt(i);
            Int32 childBottom;
            child->GetBottom(&childBottom);
            if (childBottom >= top) {
                break;
            }
            else {
                count++;
                Int32 position = firstPosition + i;
                if (position >= headerViewsCount && position < footerViewsStart) {
                    mRecycler->AddScrapView(child, position);
                }
            }
        }
    }
    else {
        Int32 bottom = GetHeight() - incrementalDeltaY;
        if ((mGroupFlags & ViewGroup::CLIP_TO_PADDING_MASK) == ViewGroup::CLIP_TO_PADDING_MASK) {
            bottom -= mListPadding->mBottom;
        }
        for (Int32 i = childCount - 1; i >= 0; i--) {
            AutoPtr<IView> child = GetChildAt(i);
            Int32 childTop;
            child->GetTop(&childTop);
            if (childTop <= bottom) {
                break;
            }
            else {
                start = i;
                count++;
                Int32 position = firstPosition + i;
                if (position >= headerViewsCount && position < footerViewsStart) {
                    mRecycler->AddScrapView(child, position);
                }
            }
        }
    }

    mMotionViewNewTop = mMotionViewOriginalTop + deltaY;

    mBlockLayoutRequests = TRUE;

    if (count > 0) {
        DetachViewsFromParent(start, count);

        mRecycler->RemoveSkippedScrap();
    }

    // invalidate before moving the children to avoid unnecessary invalidate
    // calls to bubble up from the children all the way to the top
    if (!AwakenScrollBars()) {
        Invalidate();
    }

    OffsetChildrenTopAndBottom(incrementalDeltaY);

    if (down) {
        mFirstPosition += count;
    }


    Int32 absIncrementalDeltaY = Elastos::Core::Math::Abs(incrementalDeltaY);
    if (spaceAbove < absIncrementalDeltaY
        || spaceBelow < absIncrementalDeltaY) {
        FillGap(down);
    }

    if (!inTouchMode && mSelectedPosition != IAdapterView::INVALID_POSITION) {
        Int32 childIndex = mSelectedPosition - mFirstPosition;
        if (childIndex >= 0 && childIndex < GetChildCount()) {
            PositionSelector(mSelectedPosition, GetChildAt(childIndex));
        }
    }
    else if (mSelectorPosition != IAdapterView::INVALID_POSITION) {
        Int32 childIndex = mSelectorPosition - mFirstPosition;
        if (childIndex >= 0 && childIndex < GetChildCount()) {
            PositionSelector(IAdapterView::INVALID_POSITION, GetChildAt(childIndex));
        }
    } else {
        mSelectorRect->SetEmpty();
    }

    mBlockLayoutRequests = FALSE;

    InvokeOnItemScrollListener();
    return FALSE;
}

/**
 * Returns the number of header views in the list. Header views are special views
 * at the top of the list that should not be recycled during a layout.
 *
 * @return The number of header views, 0 in the default implementation.
 */
Int32 AbsListView::GetHeaderViewsCount()
{
    return 0;
}

/**
 * Returns the number of footer views in the list. Footer views are special views
 * at the bottom of the list that should not be recycled during a layout.
 *
 * @return The number of footer views, 0 in the default implementation.
 */
Int32 AbsListView::GetFooterViewsCount()
{
    return 0;
}

void AbsListView::HideSelector()
{
    if (mSelectedPosition != IAdapterView::INVALID_POSITION) {
        if (mLayoutMode != LAYOUT_SPECIFIC) {
            mResurrectToPosition = mSelectedPosition;
        }

        if (mNextSelectedPosition >= 0 && mNextSelectedPosition != mSelectedPosition) {
            mResurrectToPosition = mNextSelectedPosition;
        }

        SetSelectedPositionInt(IAdapterView::INVALID_POSITION);
        SetNextSelectedPositionInt(IAdapterView::INVALID_POSITION);
        mSelectedTop = 0;
    }
}

/**
 * @return A position to select. First we try mSelectedPosition. If that has been clobbered by
 * entering touch mode, we then try mResurrectToPosition. Values are pinned to the range
 * of items available in the adapter
 */
Int32 AbsListView::ReconcileSelectedPosition()
{
    Int32 position = mSelectedPosition;
    if (position < 0) {
        position = mResurrectToPosition;
    }
    position = Elastos::Core::Math::Max(0, position);
    position = Elastos::Core::Math::Min(position, mItemCount - 1);
    return position;
}

/**
 * Find the row closest to y. This row will be used as the motion row when scrolling.
 *
 * @param y Where the user touched
 * @return The position of the first (or only) item in the row closest to y
 */
Int32 AbsListView::FindClosestMotionRow(
    /* [in] */ Int32 y)
{
    Int32 childCount = GetChildCount();
    if (childCount == 0) {
        return IAdapterView::INVALID_POSITION;
    }

    Int32 motionRow = FindMotionRow(y);
    return motionRow != IAdapterView::INVALID_POSITION
        ? motionRow : mFirstPosition + childCount - 1;
}

/**
 * Causes all the views to be rebuilt and redrawn.
 */
ECode AbsListView::InvalidateViews()
{
    mDataChanged = TRUE;
    RememberSyncState();
    RequestLayout();
    Invalidate();

    return NOERROR;
}

/**
 * If there is a selection returns FALSE.
 * Otherwise resurrects the selection and returns TRUE if resurrected.
 */
Boolean AbsListView::ResurrectSelectionIfNeeded()
{
    if (mSelectedPosition < 0 && ResurrectSelection()) {
        UpdateSelectorState();
        return TRUE;
    }

    return FALSE;
}


/**
 * Attempt to bring the selection back if the user is switching from touch
 * to trackball mode
 * @return Whether selection was set to something.
 */
Boolean AbsListView::ResurrectSelection()
{
    Int32 childCount = GetChildCount();

    if (childCount <= 0) {
        return FALSE;
    }

    Int32 selectedTop = 0;
    Int32 selectedPos;
    Int32 childrenTop = mListPadding->mTop;
    Int32 childrenBottom = mBottom - mTop - mListPadding->mBottom;
    Int32 firstPosition = mFirstPosition;
    Int32 toPosition = mResurrectToPosition;
    Boolean down = TRUE;

    if (toPosition >= firstPosition
        && toPosition < firstPosition + childCount) {

        selectedPos = toPosition;
        AutoPtr<IView> selected = GetChildAt(selectedPos - mFirstPosition);
        selected->GetTop(&selectedTop);
        Int32 selectedBottom;
        selected->GetBottom(&selectedBottom);

        // We are scrolled, don't get in the fade
        if (selectedTop < childrenTop) {
            selectedTop = childrenTop + GetVerticalFadingEdgeLength();
        }
        else if (selectedBottom > childrenBottom) {
            Int32 measureHeight;
            selected->GetMeasuredHeight(&measureHeight);
            selectedTop =
                childrenBottom - measureHeight - GetVerticalFadingEdgeLength();
        }
    }
    else {
        if (toPosition < firstPosition) {
            // Default to selecting whatever is first
            selectedPos = firstPosition;
            for (Int32 i = 0; i < childCount; i++) {
                Int32 top;
                GetChildAt(i)->GetTop(&top);

                if (i == 0) {
                    // Remember the position of the first item
                    selectedTop = top;
                    // See if we are scrolled at all
                    if (firstPosition > 0 || top < childrenTop) {
                        // If we are scrolled, don't select anything that is
                        // in the fade region
                        childrenTop += GetVerticalFadingEdgeLength();
                    }
                }

                if (top >= childrenTop) {
                    // Found a view whose top is fully visisble
                    selectedPos = firstPosition + i;
                    selectedTop = top;
                    break;
                }
            }
        }
        else {
            Int32 itemCount = mItemCount;
            down = FALSE;
            selectedPos = firstPosition + childCount - 1;

            for (Int32 i = childCount - 1; i >= 0; i--) {
                AutoPtr<IView> v = GetChildAt(i);
                Int32 top, bottom;
                v->GetTop(&top);
                v->GetBottom(&bottom);

                if (i == childCount - 1) {
                    selectedTop = top;
                    if (firstPosition + childCount < itemCount
                        || bottom > childrenBottom) {
                        childrenBottom -= GetVerticalFadingEdgeLength();
                    }
                }

                if (bottom <= childrenBottom) {
                    selectedPos = firstPosition + i;
                    selectedTop = top;
                    break;
                }
            }
        }
    }

    mResurrectToPosition = IAdapterView::INVALID_POSITION;
    RemoveCallbacks(mFlingRunnable);
    if (mPositionScroller != NULL) {
        mPositionScroller->Stop();
    }
    mTouchMode = TOUCH_MODE_REST;
    ClearScrollingCache();
    mSpecificTop = selectedTop;
    selectedPos = LookForSelectablePosition(selectedPos, down);
    if (selectedPos >= firstPosition
        && selectedPos <= GetLastVisiblePosition()) {
        mLayoutMode = LAYOUT_SPECIFIC;
        UpdateSelectorState();
        SetSelectionInt(selectedPos);
        InvokeOnItemScrollListener();
    }
    else {
        selectedPos = IAdapterView::INVALID_POSITION;
    }
    ReportScrollStateChange(IAbsListViewOnScrollListener::SCROLL_STATE_IDLE);

    return selectedPos >= 0;

}

void AbsListView::ConfirmCheckedPositionsById()
{
    using Elastos::Core::Math;

    // Clear out the positional check states, we'll rebuild it below from IDs.
    mCheckStates->Clear();

    Int64 id, lastPosId, searchId;
    Int32 lastPos, start, end;
    Boolean found = FALSE;
    Boolean checkedCountChanged = FALSE;
    HashMap<Int64, Int32>::Iterator it = mCheckedIdStates->Begin();
    for (; it != mCheckedIdStates->End();) {
        id = it->mFirst;
        lastPos = it->mSecond;
        mAdapter->GetItemId(lastPos, &lastPosId);
        if (id != lastPosId) {
            // Look around to see if the ID is nearby. If not, uncheck it.
            start = Math::Max(0, lastPos - CHECK_POSITION_SEARCH_DISTANCE);
            end = Math::Min(lastPos + CHECK_POSITION_SEARCH_DISTANCE, mItemCount);

            for (Int32 searchPos = start; searchPos < end; searchPos++) {
                mAdapter->GetItemId(searchPos, &searchId);
                if (id == searchId) {
                    found = TRUE;
                    (*mCheckStates)[searchPos] = TRUE;
                    (*mCheckedIdStates)[id] = searchPos;
                    break;
                }
            }

            if (!found) {
                mCheckedIdStates->Erase(it++);
                mCheckedItemCount--;
                checkedCountChanged = TRUE;
                if (mChoiceActionMode != NULL && mMultiChoiceModeCallback != NULL) {
                    mMultiChoiceModeCallback->OnItemCheckedStateChanged(mChoiceActionMode,
                        lastPos, id, FALSE);
                }
            }
            else {
                ++it;
            }
        }
        else {
            (*mCheckStates)[lastPos] = TRUE;
            ++it;
        }
    }

    if (checkedCountChanged && mChoiceActionMode != NULL) {
        mChoiceActionMode->Invalidate();
    }
}

//@Override
void AbsListView::HandleDataChanged()
{
    Int32 count = mItemCount;

    Int32 lastHandledItemCount = mLastHandledItemCount;
    mLastHandledItemCount = mItemCount;

    Boolean hasStableIds;
    mAdapter->HasStableIds(&hasStableIds);
    if (mChoiceMode != IAbsListView::CHOICE_MODE_NONE && mAdapter != NULL && hasStableIds) {
        ConfirmCheckedPositionsById();
    }

    // TODO: In the future we can recycle these views based on stable ID instead.
    mRecycler->ClearTransientStateViews();

    if (count > 0) {
        Int32 newPos;
        Int32 selectablePos;

        // Find the row we are supposed to sync to
        if (mNeedSync) {
            // Update this first, since setNextSelectedPositionInt inspects it
            mNeedSync = FALSE;
            mPendingSync = NULL;

            if (mTranscriptMode == IAbsListView::AbsListView_TRANSCRIPT_MODE_ALWAYS_SCROLL) {
                mLayoutMode = LAYOUT_FORCE_BOTTOM;
                return;
            } else if (mTranscriptMode == IAbsListView::AbsListView_TRANSCRIPT_MODE_NORMAL) {
                if (mForceTranscriptScroll) {
                    mForceTranscriptScroll = FALSE;
                    mLayoutMode = LAYOUT_FORCE_BOTTOM;
                    return;
                }
                Int32 childCount = GetChildCount();
                Int32 listBottom = GetHeight() - GetPaddingBottom();
                AutoPtr<IView> lastChild = GetChildAt(childCount - 1);
                Int32 lastChildBottom;
                lastChild->GetBottom(&lastChildBottom);
                Int32 lastBottom = lastChild != NULL ? lastChildBottom : listBottom;
                if (mFirstPosition + childCount >= lastHandledItemCount &&
                    lastBottom <= listBottom) {
                        mLayoutMode = LAYOUT_FORCE_BOTTOM;
                        return;
                }
                // Something new came in and we didn't scroll; give the user a clue that
                // there's something new.
                AwakenScrollBars();
            }

            switch (mSyncMode) {
            case SYNC_SELECTED_POSITION:
                if (IsInTouchMode())
                {
                    // We saved our state when not in touch mode. (We know this because
                    // mSyncMode is SYNC_SELECTED_POSITION.) Now we are trying to
                    // restore in touch mode. Just leave mSyncPosition as it is (possibly
                    // adjusting if the available range changed) and return.
                    mLayoutMode = LAYOUT_SYNC;
                    mSyncPosition = Elastos::Core::Math::Min(Elastos::Core::Math::Max(0, mSyncPosition), count - 1);

                    return;
                }
                else {
                    // See if we can find a position in the new data with the same
                    // id as the old selection. This will change mSyncPosition.
                    newPos = FindSyncPosition();
                    if (newPos >= 0) {
                        // Found it. Now verify that new selection is still selectable
                        selectablePos = LookForSelectablePosition(newPos, TRUE);
                        if (selectablePos == newPos) {
                            // Same row id is selected
                            mSyncPosition = newPos;

                            if (mSyncHeight == GetHeight()) {
                                // If we are at the same height as when we saved state, try
                                // to restore the scroll position too.
                                mLayoutMode = LAYOUT_SYNC;
                            }
                            else {
                                // We are not the same height as when the selection was saved, so
                                // don't try to restore the exact position
                                mLayoutMode = LAYOUT_SET_SELECTION;
                            }

                            // Restore selection
                            SetNextSelectedPositionInt(newPos);
                            return;
                        }
                    }
                }
                break;
            case SYNC_FIRST_POSITION:
                // Leave mSyncPosition as it is -- just pin to available range
                mLayoutMode = LAYOUT_SYNC;
                mSyncPosition = Elastos::Core::Math::Min(Elastos::Core::Math::Max(0, mSyncPosition), count - 1);

                return;
            default:
                break;
            }
        }

        if (!IsInTouchMode()) {
            // We couldn't find matching data -- try to use the same position
            newPos = GetSelectedItemPosition();

            // Pin position to the available range
            if (newPos >= count) {
                newPos = count - 1;
            }

            if (newPos < 0) {
                newPos = 0;
            }

            // Make sure we select something selectable -- first look down
            selectablePos = LookForSelectablePosition(newPos, TRUE);

            if (selectablePos >= 0) {
                SetNextSelectedPositionInt(selectablePos);
                return;
            }
            else {
                // Looking down didn't work -- try looking up
                selectablePos = LookForSelectablePosition(newPos, FALSE);
                if (selectablePos >= 0) {
                    SetNextSelectedPositionInt(selectablePos);
                    return;
                }
            }
        }
        else {
            // We already know where we want to resurrect the selection
            if (mResurrectToPosition >= 0) {
                return;
            }
        }
    }

    // Nothing is selected. Give up and reset everything.
    mLayoutMode = mStackFromBottom ? LAYOUT_FORCE_BOTTOM : LAYOUT_FORCE_TOP;
    mSelectedPosition = IAdapterView::INVALID_POSITION;
    mSelectedRowId = IAdapterView::INVALID_ROW_ID;
    mNextSelectedPosition = IAdapterView::INVALID_POSITION;
    mNextSelectedRowId = IAdapterView::INVALID_ROW_ID;
    mNeedSync = FALSE;
    mPendingSync = NULL;
    mSelectorPosition = IAdapterView::INVALID_POSITION;
    CheckSelectionChanged();

}

//@Override
void AbsListView::OnDisplayHint(
    /* [in] */ Int32 hint)
{
    AdapterView::OnDisplayHint(hint);
    switch (hint) {
    case IView::INVISIBLE:
        if (mPopup != NULL) {
            Boolean showing;
            mPopup->IsShowing(&showing);
            if(showing)
            DismissPopup();
        }
        break;
    case IView::VISIBLE:
        if (mFiltered && mPopup != NULL)
        {
            Boolean showing;
            mPopup->IsShowing(&showing);
            if(showing)
            ShowPopup();
        }
        break;
    }
    mPopupHidden = hint == IView::INVISIBLE;
}

/**
 * Removes the filter window
 */
void AbsListView::DismissPopup()
{
    if (mPopup != NULL) {
        mPopup->Dismiss();
    }
}

/**
 * Shows the filter window
 */
void AbsListView::ShowPopup()
{
    // Make sure we have a window before showing the popup
    if (GetWindowVisibility() == IView::VISIBLE) {
        CreateTextFilter(TRUE);
        PositionPopup();
        // Make sure we get focus if we are showing the popup
        CheckFocus();
    }
}

void AbsListView::PositionPopup()
{
    AutoPtr<IDisplayMetrics> dm;
    GetResources()->GetDisplayMetrics((IDisplayMetrics**)&dm);
    Int32 screenHeight;
    dm->GetHeightPixels(&screenHeight);
    Int32 x, y;
    GetLocationOnScreen(&x, &y);
    // TODO: The 20 below should come from the theme
    // TODO: And the gravity should be defined in the theme as well
    //
    Int32 bottomGap = screenHeight - y - GetHeight() + (Int32)(mDensityScale * 20);

    Boolean showing;
    if (mPopup->IsShowing(&showing), !showing) {
        mPopup->ShowAtLocation(THIS_PROBE(IAbsListView),
                IGravity::BOTTOM | IGravity::CENTER_HORIZONTAL, x, bottomGap);
    }
    else {
        mPopup->Update(x, bottomGap, -1, -1);
    }
}

/**
 * What is the distance between the source and destination rectangles given the direction of
 * focus navigation between them? The direction basically helps figure out more quickly what is
 * self evident by the relationship between the rects...
 *
 * @param source the source rectangle
 * @param dest the destination rectangle
 * @param direction the direction
 * @return the distance between the rectangles
 */
Int32 AbsListView::GetDistance(
    /* [in] */ IRect* sourceRect,
    /* [in] */ IRect* destRect,
    /* [in] */ Int32 direction)
{
    assert(sourceRect && destRect);
    CRect* source = (CRect*)sourceRect;
    CRect* dest = (CRect*)destRect;

    Int32 sX, sY; // source x, y
    Int32 dX, dY; // dest x, y

    Int32 sHeight, sWidth, dHeight, dWidth;
    source->GetHeight(&sHeight);
    source->GetWidth(&sWidth);
    dest->GetHeight(&dHeight);
    dest->GetWidth(&dWidth);

    switch (direction) {
    case IView::FOCUS_RIGHT:
        {
            sX = source->mRight;
            sY = source->mTop + sHeight / 2;
            dX = dest->mLeft;
            dY = dest->mTop + dHeight / 2;
        }
        break;
    case IView::FOCUS_DOWN:
        {
            sX = source->mLeft + sWidth / 2;
            sY = source->mBottom;
            dX = dest->mLeft + dWidth / 2;
            dY = dest->mTop;
        }
        break;
    case IView::FOCUS_LEFT:
        {
            sX = source->mLeft;
            sY = source->mTop + sHeight / 2;
            dX = dest->mRight;
            dY = dest->mTop + dHeight / 2;
        }
        break;
    case IView::FOCUS_UP:
        {
            sX = source->mLeft + sWidth / 2;
            sY = source->mTop;
            dX = dest->mLeft + dWidth / 2;
            dY = dest->mBottom;
        }
        break;
    case IView::FOCUS_FORWARD:
    case IView::FOCUS_BACKWARD:
        sX = source->mRight + sWidth / 2;
        sY = source->mTop + sHeight / 2;
        dX = dest->mLeft + dWidth / 2;
        dY = dest->mTop + dHeight / 2;
        break;
    default:
        Logger::E("AbsListView",
                "direction must be one of {FOCUS_UP, FOCUS_DOWN, FOCUS_LEFT, FOCUS_RIGHT}.");
        assert(0);
    }

    Int32 deltaX = dX - sX;
    Int32 deltaY = dY - sY;

    return deltaY * deltaY + deltaX * deltaX;
}

//@Override
Boolean AbsListView::IsInFilterMode()
{
    return mFiltered;
}

/**
 * Sends a key to the text filter window
 *
 * @param keyCode The keycode for the event
 * @param event The actual key event
 *
 * @return True if the text filter handled the event, FALSE otherwise.
 */
Boolean AbsListView::SendToTextFilter(
    /* [in] */ Int32 keyCode,
    /* [in] */ Int32 count,
    /* [in] */ IKeyEvent* event)
{
    if (!AcceptFilter()) {
        return FALSE;
    }

    Boolean handled = FALSE;
    Boolean okToSend = TRUE;
    switch (keyCode) {
    case IKeyEvent::KEYCODE_DPAD_UP:
    case IKeyEvent::KEYCODE_DPAD_DOWN:
    case IKeyEvent::KEYCODE_DPAD_LEFT:
    case IKeyEvent::KEYCODE_DPAD_RIGHT:
    case IKeyEvent::KEYCODE_DPAD_CENTER:
    case IKeyEvent::KEYCODE_ENTER:
        okToSend = FALSE;
        break;
    case IKeyEvent::KEYCODE_BACK:
        if (mFiltered && mPopup != NULL)
        {
            Boolean showing;
            mPopup->IsShowing(&showing);
            if(showing)
            {
                Int32 action, repeatCount;
                Boolean tracking, canceled;
                event->GetAction(&action);
                event->GetRepeatCount(&repeatCount);
                event->IsTracking(&tracking);
                event->IsCanceled(&canceled);
                if (action == IKeyEvent::ACTION_DOWN && repeatCount == 0) {
                    AutoPtr<IDispatcherState> state;
                    GetKeyDispatcherState((IDispatcherState**)&state);
                    state->StartTracking(event, this->Probe(EIID_IAbsListView));
                    handled = TRUE;
                } else if (action == IKeyEvent::ACTION_UP && tracking && !canceled) {
                    handled = TRUE;
                    String t("");
                    AutoPtr<ICharSequence> text;
                    FAIL_RETURN(CStringWrapper::New(t, (ICharSequence**)&text));
                    mTextFilter->SetText(text);
                }
            }
        }
        okToSend = FALSE;
        break;

    case IKeyEvent::KEYCODE_SPACE:
        // Only send spaces once we are filtered
        okToSend = mFiltered;
        break;
    }

    if (okToSend) {
        CreateTextFilter(TRUE);

        AutoPtr<IKeyEvent> forwardEvent = event;
        Int32 repeatCount;
        forwardEvent->GetRepeatCount(&repeatCount);
        if (repeatCount > 0) {
            Int64 eventTime;
            event->GetEventTime(&eventTime);
            AutoPtr<IKeyEventHelper> helper;
            CKeyEventHelper::AcquireSingleton((IKeyEventHelper**)&helper);
            forwardEvent = NULL;
            helper->ChangeTimeRepeat(
                event, eventTime, 0, (IKeyEvent**)&forwardEvent);
        }

        Int32 action;
        event->GetAction(&action);
        AutoPtr<IKeyEventCallback> keyCallback =
                (IKeyEventCallback*)mTextFilter->Probe(EIID_IKeyEventCallback);
        switch (action)
        {
            case IKeyEvent::ACTION_DOWN:
                keyCallback->OnKeyDown(keyCode, forwardEvent, &handled);
                break;
            case IKeyEvent::ACTION_UP:
                keyCallback->OnKeyUp(keyCode, forwardEvent, &handled);
                break;
            case IKeyEvent::ACTION_MULTIPLE:
                keyCallback->OnKeyMultiple(keyCode, count, event, &handled);
                break;
        }
    }
    return handled;
}

/**
 * Return an InputConnection for editing of the filter text.
 */
AutoPtr<IInputConnection> AbsListView::OnCreateInputConnection(
    /* [in] */ IEditorInfo* outAttrs)
{
    Logger::W("AbsListView", " >> TODO: AbsListView::OnCreateInputConnection.");
//    if (IsTextFilterEnabled()) {
//        // XXX we need to have the text filter created, so we can get an
//        // InputConnection to proxy to.  Unfortunately this means we pretty
//        // much need to make it as soon as a list view gets focus.
//        CreateTextFilter(FALSE);
//        if (mPublicInputConnection == NULL) {
//            mDefInputConnection = new BaseInputConnection(this, FALSE);
//            mPublicInputConnection = new InputConnectionWrapper(
//                    mTextFilter.onCreateInputConnection(outAttrs), TRUE) {
//                @Override
//                public Boolean reportFullscreenMode(Boolean enabled) {
//                    // Use our own input connection, since it is
//                    // the "real" one the IME is talking with.
//                    return mDefInputConnection.reportFullscreenMode(enabled);
//                }
//
//                @Override
//                public Boolean performEditorAction(Int32 editorAction) {
//                    // The editor is off in its own window; we need to be
//                    // the one that does this.
//                    if (editorAction == EditorInfo.IME_ACTION_DONE) {
//                        InputMethodManager imm = (InputMethodManager)
//                                getContext().getSystemService(
//                                        Context.INPUT_METHOD_SERVICE);
//                        if (imm != NULL) {
//                            imm.hideSoftInputFromWindow(getWindowToken(), 0);
//                        }
//                        return TRUE;
//                    }
//                    return FALSE;
//                }
//
//                @Override
//                public Boolean sendKeyEvent(KeyEvent event) {
//                    // Use our own input connection, since the filter
//                    // text view may not be shown in a window so has
//                    // no ViewRootImpl to dispatch events with.
//                    return mDefInputConnection.sendKeyEvent(event);
//                }
//            };
//        }
//        outAttrs.inputType = EditorInfo.TYPE_CLASS_TEXT
//                | EditorInfo.TYPE_TEXT_VARIATION_FILTER;
//        outAttrs.imeOptions = EditorInfo.IME_ACTION_DONE;
//        return mPublicInputConnection;
//    }
    return NULL;
}

/**
 * For filtering we proxy an input connection to an internal text editor,
 * and this allows the proxying to happen.
 */
//@Override
Boolean AbsListView::CheckInputConnectionProxy(
    /* [in] */ IView* view)
{
    return view == IView::Probe(mTextFilter);
}

/**
 * Creates the window for the text filter and populates it with an EditText field;
 *
 * @param animateEntrance TRUE if the window should appear with an animation
 */
void AbsListView::CreateTextFilter(
    /* [in] */ Boolean animateEntrance)
{
    if (mPopup == NULL) {
        AutoPtr<IContext> c = GetContext();;
        AutoPtr<IPopupWindow> p;
        CPopupWindow::New(c, (IPopupWindow**)&p);
        AutoPtr<ILayoutInflater> layoutInflater;
        c->GetSystemService(IContext::LAYOUT_INFLATER_SERVICE, (IInterface**)&layoutInflater);
        mTextFilter = NULL;
        layoutInflater->Inflate(R::layout::typing_filter, NULL, (IView**)&mTextFilter);
        // For some reason setting this as the "real" input type changes
        // the text view in some way that it doesn't work, and I don't
        // want to figure out why this is.
        mTextFilter->SetRawInputType(IInputType::TYPE_CLASS_TEXT
                | IInputType::TYPE_TEXT_VARIATION_FILTER);
        mTextFilter->SetImeOptions(IEditorInfo::IME_FLAG_NO_EXTRACT_UI);
        mTextFilter->AddTextChangedListener((ITextWatcher*)this->Probe(EIID_ITextWatcher));
        p->SetFocusable(FALSE);
        p->SetTouchable(FALSE);

        p->SetInputMethodMode(IPopupWindow::INPUT_METHOD_NOT_NEEDED);
        p->SetContentView(mTextFilter);
        p->SetWidth(IAbsListViewLayoutParams::WRAP_CONTENT);
        p->SetHeight(IAbsListViewLayoutParams::WRAP_CONTENT);
        p->SetBackgroundDrawable(NULL);
        mPopup = p;
        GetViewTreeObserver()->AddOnGlobalLayoutListener((IOnGlobalLayoutListener*)this->Probe(EIID_IOnGlobalLayoutListener));
        mGlobalLayoutListenerAddedFilter = TRUE;
    }
    if (animateEntrance) {
        mPopup->SetAnimationStyle(R::style::Animation_TypingFilter);
    } else {
        mPopup->SetAnimationStyle(R::style::Animation_TypingFilterRestore);
    }
}

/**
 * Clear the text filter.
 */
ECode AbsListView::ClearTextFilter()
{
    if (mFiltered) {
        String t("");
        AutoPtr<ICharSequence> text;
        FAIL_RETURN(CStringWrapper::New(t, (ICharSequence**)&text));
        mTextFilter->SetText(text);
        mFiltered = FALSE;
        if (mPopup != NULL)
        {
            Boolean showing;
            mPopup->IsShowing(&showing);
            if(showing)
                DismissPopup();
        }
    }
    return NOERROR;
}

/**
 * Returns if the ListView currently has a text filter.
 */
Boolean AbsListView::HasTextFilter()
{
    return mFiltered;
}

ECode AbsListView::OnGlobalLayout()
{
    if (IsShown()) {
        // Show the popup if we are filtered
        if (mFiltered && mPopup != NULL)
        {
            Boolean showing;
            mPopup->IsShowing(&showing);
            if(!showing && !mPopupHidden)
                ShowPopup();
        }
    }
    else {
        // Hide the popup when we are no longer visible
        if (mPopup != NULL)
        {
            Boolean showing;
            mPopup->IsShowing(&showing);
            if(showing)
                DismissPopup();
        }
    }

    return NOERROR;
}

/**
 * For our text watcher that is associated with the text filter.  Does
 * nothing.
 */
ECode AbsListView::BeforeTextChanged(
    /* [in] */ ICharSequence* s,
    /* [in] */ Int32 start,
    /* [in] */ Int32 count,
    /* [in] */ Int32 after)
{
    return NOERROR;
}

/**
 * For our text watcher that is associated with the text filter. Performs
 * the actual filtering as the text changes, and takes care of hiding and
 * showing the popup displaying the currently entered filter text.
 */
ECode AbsListView::OnTextChanged(
    /* [in] */ ICharSequence* s,
    /* [in] */ Int32 start,
    /* [in] */ Int32 before,
    /* [in] */ Int32 count)
{
    if (mPopup != NULL && IsTextFilterEnabled()) {
        Int32 length;
        s->GetLength(&length);
        Boolean showing;
        mPopup->IsShowing(&showing);
        if (!showing && length > 0) {
            // Show the filter popup if necessary
            ShowPopup();
            mFiltered = TRUE;
        } else if (showing && length == 0) {
            // Remove the filter popup if the user has cleared all text
            DismissPopup();
            mFiltered = FALSE;
        }
        AutoPtr<IFilterable> filterable = (IFilterable*)mAdapter->Probe(EIID_IFilterable);
        if (filterable != NULL) {
            AutoPtr<IFilter> f;
            filterable->GetFilter((IFilter**)&f);
            // Filter should not be NULL when we reach this part
            if (f != NULL) {
                f->DoFilter(s, (IFilterListener*)this->Probe(EIID_IFilterListener));
            } else {
                return E_ILLEGAL_STATE_EXCEPTION;
            }
        }
    }
    return NOERROR;
}

/**
 * For our text watcher that is associated with the text filter.  Does
 * nothing.
 */
ECode AbsListView::AfterTextChanged(
    /* [in] */ IEditable* s)
{
    return NOERROR;
}

ECode AbsListView::OnFilterComplete(
    /* [in] */ Int32 count)
{
    if (mSelectedPosition < 0 && count > 0) {
        mResurrectToPosition = IAdapterView::INVALID_POSITION;
        ResurrectSelection();
    }
    return NOERROR;
}

ECode AbsListView::GenerateDefaultLayoutParams(
    /* [out] */ IViewGroupLayoutParams** result)
{
    AutoPtr<IAbsListViewLayoutParams> temp;
    CAbsListViewLayoutParams::New(IViewGroupLayoutParams::MATCH_PARENT, IViewGroupLayoutParams::WRAP_CONTENT, (IAbsListViewLayoutParams**)&temp);
    *result = temp.Get();
    REFCOUNT_ADD(*result);
    return NOERROR;
}

//@Override

AutoPtr<IViewGroupLayoutParams> AbsListView::GenerateLayoutParams(
    /* [in] */ IViewGroupLayoutParams* p)
{
    AutoPtr<IAbsListViewLayoutParams> temp;
    CAbsListViewLayoutParams::New(p, (IAbsListViewLayoutParams**)&temp);
    return temp.Get();
}

//@Override
ECode AbsListView::GenerateLayoutParams(
    /* [in] */ IAttributeSet* attrs,
    /* [out] */ IViewGroupLayoutParams** params)
{
    VALIDATE_NOT_NULL(params);
    AutoPtr<IAbsListViewLayoutParams> lp;
    FAIL_RETURN(CAbsListViewLayoutParams::New(GetContext(), attrs, (IAbsListViewLayoutParams**)&lp));
    *params = IViewGroupLayoutParams::Probe(lp);
    REFCOUNT_ADD(*params);
    return NOERROR;
}

//@Override
Boolean AbsListView::CheckLayoutParams(
    /* [in] */ IViewGroupLayoutParams* p)
{
    return (p != NULL) && (p->Probe(EIID_IAbsListViewLayoutParams) != NULL);
}

/**
 * Puts the list or grid into transcript mode. In this mode the list or grid will always scroll
 * to the bottom to show new items.
 *
 * @param mode the transcript mode to set
 *
 * @see #AbsListView_TRANSCRIPT_MODE_DISABLED
 * @see #AbsListView_TRANSCRIPT_MODE_NORMAL
 * @see #AbsListView_TRANSCRIPT_MODE_ALWAYS_SCROLL
 */
ECode AbsListView::SetTranscriptMode(
    /* [in] */ Int32 mode)
{
    mTranscriptMode = mode;

    return NOERROR;
}

/**
 * Returns the current transcript mode.
 *
 * @return {@link #AbsListView_TRANSCRIPT_MODE_DISABLED}, {@link #AbsListView_TRANSCRIPT_MODE_NORMAL} or
 *         {@link #AbsListView_TRANSCRIPT_MODE_ALWAYS_SCROLL}
 */
Int32 AbsListView::GetTranscriptMode()
{
    return mTranscriptMode;
}

//@Override
Int32 AbsListView::GetSolidColor()
{
    return mCacheColorHint;
}

/**
 * When set to a non-zero value, the cache color hint indicates that this list is always drawn
 * on top of a solid, single-color, opaque background.
 *
 * Zero means that what's behind this object is translucent (non solid) or is not made of a
 * single color. This hint will not affect any existing background drawable set on this view (
 * typically set via {@link #setBackgroundDrawable(Drawable)}).
 *
 * @param color The background color
 */
ECode AbsListView::SetCacheColorHint(
    /* [in] */ Int32 color)
{
    if (color != mCacheColorHint) {
        mCacheColorHint = color;
        Int32 count = GetChildCount();
        for (Int32 i = 0; i < count; i++) {
            AutoPtr<IView> child = GetChildAt(i);
            child->SetDrawingCacheBackgroundColor(color);
        }
        mRecycler->SetCacheColorHint(color);
    }

    return NOERROR;
}

/**
 * When set to a non-zero value, the cache color hint indicates that this list is always drawn
 * on top of a solid, single-color, opaque background
 *
 * @return The cache color hint
 */
Int32 AbsListView::GetCacheColorHint()
{
    return mCacheColorHint;
}

/**
 * Move all views (excluding headers and footers) held by this AbsListView into the supplied
 * List. This includes views displayed on the screen as well as views stored in AbsListView's
 * internal view recycler.
 *
 * @param views A list into which to put the reclaimed views
 */
ECode AbsListView::ReclaimViews(
    /* [in] */ IObjectContainer* views)
{
    Int32 childCount = GetChildCount();
    AutoPtr<IRecyclerListener> listener = mRecycler->mRecyclerListener;

    // Reclaim views on screen
    for (Int32 i = 0; i < childCount; i++) {
        AutoPtr<IView> child = GetChildAt(i);
        AutoPtr<IAbsListViewLayoutParams> lp;
        child->GetLayoutParams((IViewGroupLayoutParams**)&lp);
        // Don't reclaim header or footer views, or views that should be ignored

        if (lp != NULL && mRecycler->ShouldRecycleViewType(
                ((CAbsListViewLayoutParams*)lp.Get())->mViewType))
        {
            views->Add(child);
            child->SetAccessibilityDelegate(NULL);
            if (listener != NULL) {
                // Pretend they went through the scrap heap
                listener->OnMovedToScrapHeap(child);
            }
        }
    }
    mRecycler->ReclaimScrapViews(views);
    RemoveAllViewsInLayout();

    return NOERROR;
}

void AbsListView::FinishGlows()
{
    if (mEdgeGlowTop != NULL) {
        mEdgeGlowTop->Finish();
        mEdgeGlowBottom->Finish();
    }
}

/**
 * Sets up this AbsListView to use a remote views adapter which connects to a RemoteViewsService
 * through the specified intent.
 * @param intent the intent used to identify the RemoteViewsService for the adapter to connect to.
 */
ECode AbsListView::SetRemoteViewsAdapter(
    /* [in] */ IIntent* intent)
{
    // Ensure that we don't already have a RemoteViewsAdapter that is bound to an existing
    // service handling the specified intent.
    if (mRemoteAdapter != NULL) {
        AutoPtr<IIntentFilterComparison> fcNew;
        CIntentFilterComparison::New(intent, (IIntentFilterComparison**)&fcNew);
        AutoPtr<IIntentFilterComparison> fcOld;
        AutoPtr<IIntent> adapterIntent;
        mRemoteAdapter->GetRemoteViewsServiceIntent((IIntent**)&adapterIntent);
        CIntentFilterComparison::New(adapterIntent, (IIntentFilterComparison**)&fcOld);
        Boolean equals;
        if (fcNew->Equals(fcOld, &equals), equals) {
            return NOERROR;
        }
    }
    mDeferNotifyDataSetChanged = FALSE;
    // Otherwise, create a new RemoteViewsAdapter for binding
    assert(0 && "TODO"); //RemoteViewsAdapter is not exsit
//    mRemoteAdapter = new RemoteViewsAdapter(getContext(), intent, this);
    Boolean ready;
    if (mRemoteAdapter->IsDataReady(&ready), ready) {
        SetAdapter((IListAdapter*)mRemoteAdapter->Probe(EIID_IListAdapter));
    }
    return NOERROR;
}

/**
 * Sets up the onClickHandler to be used by the RemoteViewsAdapter when inflating RemoteViews
 *
 * @param handler The OnClickHandler to use when inflating RemoteViews.
 *
 * @hide
 */
ECode AbsListView::SetRemoteViewsOnClickHandler(
    /* [in] */ IRemoteViewsOnClickHandler* handler)
{
    // Ensure that we don't already have a RemoteViewsAdapter that is bound to an existing
    // service handling the specified intent.
    if (mRemoteAdapter != NULL) {
        mRemoteAdapter->SetRemoteViewsOnClickHandler(handler);
    }
    return NOERROR;
}

/**
 * This defers a notifyDataSetChanged on the pending RemoteViewsAdapter if it has not
 * connected yet.
 */
ECode AbsListView::DeferNotifyDataSetChanged()
{
    mDeferNotifyDataSetChanged = TRUE;
    return NOERROR;

}

/**
 * Called back when the adapter connects to the RemoteViewsService.
 */
Boolean AbsListView::OnRemoteAdapterConnected()
{
    AutoPtr<IRemoteViewsAdapter> temp = (IRemoteViewsAdapter*)mAdapter->Probe(EIID_IRemoteViewsAdapter);
    if (temp != NULL && mRemoteAdapter != temp) {
        SetAdapter((IListAdapter*)mRemoteAdapter->Probe(EIID_IListAdapter));
        if (mDeferNotifyDataSetChanged) {
            mRemoteAdapter->NotifyDataSetChanged();
            mDeferNotifyDataSetChanged = FALSE;
        }
        return FALSE;
    }
    else if (mRemoteAdapter != NULL) {
        mRemoteAdapter->SuperNotifyDataSetChanged();
        return TRUE;
    }
    return FALSE;
}

/**
 * Called back when the adapter disconnects from the RemoteViewsService.
 */
ECode AbsListView::OnRemoteAdapterDisconnected()
{
    // If the remote adapter disconnects, we keep it around
    // since the currently displayed items are still cached.
    // Further, we want the service to eventually reconnect
    // when necessary, as triggered by this view requesting
    // items from the Adapter.
    return NOERROR;
}

/**
 * Hints the RemoteViewsAdapter, if it exists, about which views are currently
 * being displayed by the AbsListView.
 */
void AbsListView::SetVisibleRangeHint(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    if (mRemoteAdapter != NULL) {
        mRemoteAdapter->SetVisibleRangeHint(start, end);
    }
}

/**
 * Sets the recycler listener to be notified whenever a View is set aside in
 * the recycler for later reuse. This listener can be used to free resources
 * associated to the View::
 *
 * @param listener The recycler listener to be notified of views set aside
 *        in the recycler.
 *
 * @see android.widget.AbsListView.RecycleBin
 * @see android.widget.AbsListView.RecyclerListener
 */
ECode AbsListView::SetRecyclerListener(
    /* [in] */ IRecyclerListener* listener)
{
    mRecycler->mRecyclerListener = listener;

    return NOERROR;
}

ECode AbsListView::Init(
    /* [in] */ IContext* context)
{
    FAIL_RETURN(AdapterView::Init(context));

    InitAbsListView();

    Int32 size = ARRAY_SIZE(R::styleable::View);
    AutoPtr<ArrayOf<Int32> > layout = ArrayOf<Int32>::Alloc(size);
    layout->Copy(R::styleable::View, size);

    SetVerticalScrollBarEnabled(TRUE);
    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributes(layout, (ITypedArray**)&a);
    InitializeScrollbars(a);
    a->Recycle();

    return NOERROR;
}

ECode AbsListView:: Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return Init(context, attrs, R::attr::absListViewStyle);
}

ECode AbsListView::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    FAIL_RETURN(AdapterView::Init(context, attrs, defStyle));

    InitAbsListView();

    Int32 size = ARRAY_SIZE(R::styleable::AbsListView);
    AutoPtr<ArrayOf<Int32> > layout = ArrayOf<Int32>::Alloc(size);
    layout->Copy(R::styleable::AbsListView, size);

    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributes(
        attrs, layout, defStyle, 0, (ITypedArray**)&a);

    AutoPtr<IDrawable> d;
    a->GetDrawable(R::styleable::AbsListView_listSelector, (IDrawable**)&d);
    if (d != NULL) {
        SetSelector(d);
    }

    a->GetBoolean(R::styleable::AbsListView_drawSelectorOnTop, FALSE, &mDrawSelectorOnTop);

    Boolean stackFromBottom;
    a->GetBoolean(R::styleable::AbsListView_stackFromBottom, FALSE, &stackFromBottom);
    SetStackFromBottom(stackFromBottom);

    Boolean scrollingCacheEnabled;
    a->GetBoolean(R::styleable::AbsListView_scrollingCache, TRUE, &scrollingCacheEnabled);
    SetScrollingCacheEnabled(scrollingCacheEnabled);

    Boolean useTextFilter;
    a->GetBoolean(R::styleable::AbsListView_textFilterEnabled, FALSE, &useTextFilter);
    SetTextFilterEnabled(useTextFilter);

    Int32 transcriptMode;
    a->GetInt32(R::styleable::AbsListView_transcriptMode,
        IAbsListView::AbsListView_TRANSCRIPT_MODE_DISABLED, &transcriptMode);
    SetTranscriptMode(transcriptMode);

    Int32 color;
    a->GetColor(R::styleable::AbsListView_cacheColorHint, 0, &color);
    SetCacheColorHint(color);

    Boolean enableFastScroll;
    a->GetBoolean(R::styleable::AbsListView_fastScrollEnabled, FALSE, &enableFastScroll);
    SetFastScrollEnabled(enableFastScroll);

    Boolean smoothScrollbar;
    a->GetBoolean(R::styleable::AbsListView_smoothScrollbar, TRUE, &smoothScrollbar);
    SetSmoothScrollbarEnabled(smoothScrollbar);

    Int32 choiceMode;
    a->GetInt32(R::styleable::AbsListView_choiceMode, IAbsListView::CHOICE_MODE_NONE, &choiceMode);

    Boolean fastScrollAlwaysVisible;
    a->GetBoolean(R::styleable::AbsListView_fastScrollAlwaysVisible, FALSE, &fastScrollAlwaysVisible);

    SetChoiceMode(choiceMode);
    SetFastScrollAlwaysVisible(fastScrollAlwaysVisible);

    a->Recycle();

    return NOERROR;
}

AutoPtr<IView> AbsListView::RetrieveFromScrap(
    /* [in] */ List<AutoPtr<IView> >* scrapViews,
    /* [in] */ Int32 position)
{
    assert(scrapViews != NULL);

    if (scrapViews->IsEmpty()) {
        return NULL;
    }

    // See if we still have a view for this position.
    AutoPtr<IView> view;
    ViewListIterator it = scrapViews->Begin();
    for (; it != scrapViews->End(); ++it) {
        view = *it;
        AutoPtr<IAbsListViewLayoutParams> layoutParams;
        view->GetLayoutParams((IViewGroupLayoutParams**)&layoutParams);
        Int32 scrappedFromPosition = 0;
        layoutParams->GetScrappedFromPosition(&scrappedFromPosition);
        if (scrappedFromPosition == position) {
            scrapViews->Erase(it);
            return view;
        }
    }

    // return last item.
    it = scrapViews->End();
    view = *(--it);
    scrapViews->Erase(it);
    return view;
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
