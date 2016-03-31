#include "elastos/droid/systemui/recent/FirstFrameAnimatorHelper.h"
#include "Elastos.CoreLibrary.Core.h"
#include <elastos/utility/logging/Logger.h>

using Elastos::Droid::Animation::EIID_IAnimatorUpdateListener;
using Elastos::Droid::View::EIID_IOnDrawListener;
using Elastos::Droid::View::IViewTreeObserver;
using Elastos::Core::CSystem;
using Elastos::Core::ISystem;
using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace SystemUI {
namespace Recent {

//-------------------------------------------------------------------------------------------------
// FirstFrameAnimatorHelper::MyViewTreeObserverOnDrawListener
//-------------------------------------------------------------------------------------------------

CAR_INTERFACE_IMPL(FirstFrameAnimatorHelper::MyViewTreeObserverOnDrawListener, Object, IOnDrawListener)

FirstFrameAnimatorHelper::MyViewTreeObserverOnDrawListener::MyViewTreeObserverOnDrawListener()
{
    AutoPtr<ISystem> sys;
    CSystem::AcquireSingleton((ISystem**)&sys);
    sys->GetCurrentTimeMillis(&mTime);
}

ECode FirstFrameAnimatorHelper::MyViewTreeObserverOnDrawListener::OnDraw()
{
    FirstFrameAnimatorHelper::sGlobalFrameCounter++;
    if (FirstFrameAnimatorHelper::DEBUG) {
        AutoPtr<ISystem> sys;
        CSystem::AcquireSingleton((ISystem**)&sys);
        Int64 newTime;
        sys->GetCurrentTimeMillis(&newTime);
        Logger::D("FirstFrameAnimatorHelper", "TICK %ld", (newTime - mTime));
        mTime = newTime;
    }
    return NOERROR;
}

//-------------------------------------------------------------------------------------------------
// FirstFrameAnimatorHelper::MyRunnable
//-------------------------------------------------------------------------------------------------

FirstFrameAnimatorHelper::MyRunnable::MyRunnable(
    /* [in] */ IValueAnimator* animation,
    /* [in] */ FirstFrameAnimatorHelper* host)
    : mAnimation(animation)
    , mHost(host)
{}

ECode FirstFrameAnimatorHelper::MyRunnable::Run()
{
    return mAnimation->RemoveUpdateListener((IAnimatorUpdateListener*)this);
}

//-------------------------------------------------------------------------------------------------
// FirstFrameAnimatorHelper
//-------------------------------------------------------------------------------------------------

const Boolean FirstFrameAnimatorHelper::DEBUG = FALSE;
const Int32 FirstFrameAnimatorHelper::MAX_DELAY;
const Int32 FirstFrameAnimatorHelper::IDEAL_FRAME_DURATION;
AutoPtr<IOnDrawListener> FirstFrameAnimatorHelper::sGlobalDrawListener;/*ViewTreeObserver.OnDrawListener*/
Int64 FirstFrameAnimatorHelper::sGlobalFrameCounter;

CAR_INTERFACE_IMPL(FirstFrameAnimatorHelper, AnimatorListenerAdapter, IAnimatorUpdateListener)

FirstFrameAnimatorHelper::FirstFrameAnimatorHelper(
    /* [in] */ IValueAnimator* animator,
    /* [in] */ IView* target)
    : mTarget(target)
    , mStartFrame(0L)
    , mStartTime(-1)
    , mHandlingOnAnimationUpdate(FALSE)
    , mAdjustedSecondFrameTime(FALSE)
{
    animator->AddUpdateListener(this);
}

FirstFrameAnimatorHelper::FirstFrameAnimatorHelper(
    /* [in] */ IViewPropertyAnimator* vpa,
    /* [in] */ IView* target)
    : mTarget(target)
    , mStartFrame(0L)
    , mStartTime(-1)
    , mHandlingOnAnimationUpdate(FALSE)
    , mAdjustedSecondFrameTime(FALSE)
{
    vpa->SetListener(this);
}

ECode FirstFrameAnimatorHelper::OnAnimationStart(
    /* [in] */ IAnimator* animation)
{
    const AutoPtr<IValueAnimator> va = IValueAnimator::Probe(animation);
    va->AddUpdateListener(this);
    OnAnimationUpdate(va);
    return NOERROR;
}

void FirstFrameAnimatorHelper::InitializeDrawListener(
    /* [in] */ IView* view)
{
    AutoPtr<IViewTreeObserver> observer;
    view->GetViewTreeObserver((IViewTreeObserver**)&observer);
    if (sGlobalDrawListener != NULL) {
        observer->RemoveOnDrawListener(sGlobalDrawListener);
    }

    AutoPtr<MyViewTreeObserverOnDrawListener> vtodl = new MyViewTreeObserverOnDrawListener();
    sGlobalDrawListener = vtodl;
    observer->AddOnDrawListener(sGlobalDrawListener);
}

ECode FirstFrameAnimatorHelper::OnAnimationUpdate(
    /* [in] */ IValueAnimator* animation)
{
    AutoPtr<ISystem> sys;
    CSystem::AcquireSingleton((ISystem**)&sys);
    Int64 currentTime;
    sys->GetCurrentTimeMillis(&currentTime);
    if (mStartTime == -1) {
        mStartFrame = sGlobalFrameCounter;
        mStartTime = currentTime;
    }

    Int64 pt, du;
    animation->GetCurrentPlayTime(&pt);
    animation->GetDuration(&du);
    if (!mHandlingOnAnimationUpdate &&
        // If the current play time exceeds the duration, the animation
        // will get finished, even if we call setCurrentPlayTime -- therefore
        // don't adjust the animation in that case
        pt < du) {
        mHandlingOnAnimationUpdate = TRUE;
        Int64 frameNum = sGlobalFrameCounter - mStartFrame;
        // If we haven't drawn our first frame, reset the time to t = 0
        // (give up after MAX_DELAY ms of waiting though - might happen, for example, if we
        // are no longer in the foreground and no frames are being rendered ever)
        if (frameNum == 0 && currentTime < mStartTime + MAX_DELAY) {
            // The first frame on animations doesn't always trigger an invalidate...
            // force an invalidate here to make sure the animation continues to advance
            AutoPtr<IView> v;
            mTarget->GetRootView((IView**)&v);
            v->Invalidate();
            animation->SetCurrentPlayTime(0L);

        // For the second frame, if the first frame took more than 16ms,
        // adjust the start time and pretend it took only 16ms anyway. This
        // prevents a large jump in the animation due to an expensive first frame
        }
        else if (frameNum == 1 && currentTime < mStartTime + MAX_DELAY &&
                   !mAdjustedSecondFrameTime &&
                   currentTime > mStartTime + IDEAL_FRAME_DURATION) {
            animation->SetCurrentPlayTime(IDEAL_FRAME_DURATION);
            mAdjustedSecondFrameTime = TRUE;
        }
        else {
            if (frameNum > 1) {
                AutoPtr<MyRunnable> r = new MyRunnable(animation, this);
                Boolean b;
                mTarget->Post(r, &b);
            }
            if (DEBUG) Print(animation);
        }
        mHandlingOnAnimationUpdate = FALSE;
    }
    else {
        if (DEBUG) Print(animation);
    }
    return NOERROR;
}

ECode FirstFrameAnimatorHelper::Print(
    /* [in] */ IValueAnimator* animation)
{
    Int64 pt, du;
    animation->GetCurrentPlayTime(&pt);
    animation->GetDuration(&du);
    Float flatFraction = pt / (Float)du;
    Boolean isDirty;
    mTarget->IsDirty(&isDirty);

    // Logger::D(
    //     "FirstFrameAnimatorHelper%ld(%ld)%s dirty? %s %f %s %s",
    //     sGlobalFrameCounter, sGlobalFrameCounter - mStartFrame,
    //     TO_CSTR(mTarget), (isDirty ? "TRUE" : "FALSE"), flatFraction,
    //     TO_CSTR(this), TO_CSTR(animation));
    return NOERROR;
}

} // namespace Recent
} // namespace SystemUI
} // namespace Droid
} // namespace Elastos