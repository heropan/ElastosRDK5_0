
#include "wm/WindowAnimator.h"
#include "wm/AppWindowAnimator.h"
#include "wm/ScreenRotationAnimation.h"
#include "elastos/droid/os/SystemClock.h"
#include "elastos/droid/os/Handler.h"
#include <elastos/core/StringBuilder.h>
#include <elastos/core/StringUtils.h>

using Elastos::Core::StringUtils;
using Elastos::Core::StringBuilder;
using Elastos::Droid::Os::SystemClock;
using Elastos::Droid::View::ISurfaceHelper;
using Elastos::Droid::View::CSurfaceHelper;
using Elastos::Droid::View::Animation::IAlphaAnimation;
using Elastos::Droid::View::Animation::CAlphaAnimation;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Wm {

//==============================================================================
//                  WindowAnimator::AnimationRunnable
//==============================================================================

ECode WindowAnimator::AnimationRunnable::Run()
{
    synchronized (mHost->mWindowMapLock) {
        mHost->mService->mAnimationScheduled = FALSE;
        mHost->AnimateLocked();
    }
    return NOERROR;
}


//==============================================================================
//                  WindowAnimator::DisplayContentsAnimator
//==============================================================================

WindowAnimator::DisplayContentsAnimator::DisplayContentsAnimator(
    /* [in] */ WindowAnimator* host,
    /* [in] */ Int32 displayId)
    : mHost(host)
{
    mDimAnimator = new DimAnimator(mHost->mService->mFxSession, displayId);
    mWindowAnimationBackgroundSurface =
            new DimSurface(mHost->mService->mFxSession, displayId);
}


//==============================================================================
//                  WindowAnimator
//==============================================================================

const String WindowAnimator::TAG("WindowAnimator");
const Int64 KEYGUARD_ANIM_TIMEOUT_MS;
const Int32 WindowAnimator::KEYGUARD_NOT_SHOWN;
const Int32 WindowAnimator::KEYGUARD_ANIMATING_IN;
const Int32 WindowAnimator::KEYGUARD_SHOWN;
const Int32 WindowAnimator::KEYGUARD_ANIMATING_OUT;

WindowAnimator::WindowAnimator(
    /* [in] */ CWindowManagerService* service)
    : mService(service)
    , mAnimating(FALSE)
    , mCurrentTime(0)
    , mAboveUniverseLayer(0)
    , mBulkUpdateParams(0)
    , mInitialized(FALSE)
    , mKeyguardGoingAway(FALSE)
    , mKeyguardGoingAwayToNotificationShade(FALSE)
    , mKeyguardGoingAwayDisableWindowAnimations(FALSE)
    , mForceHiding(KEYGUARD_NOT_SHOWN)
{
    mContext = service->mContext;
    mPolicy = service->mPolicy;
    mAnimationRunnable = new AnimationRunnable(this);
}

WindowAnimator::~WindowAnimator()
{
    mDisplayContentsAnimators.Clear();
}

String WindowAnimator::ForceHidingToString()
{
    switch (mForceHiding) {
        case KEYGUARD_NOT_SHOWN:    return String("KEYGUARD_NOT_SHOWN");
        case KEYGUARD_ANIMATING_IN: return String("KEYGUARD_ANIMATING_IN");
        case KEYGUARD_SHOWN:        return String("KEYGUARD_SHOWN");
        case KEYGUARD_ANIMATING_OUT:return String("KEYGUARD_ANIMATING_OUT");
        default: return String("KEYGUARD STATE UNKNOWN ") + StringUtils::Int32ToString(mForceHiding);
    }
}

void WindowAnimator::AddDisplayLocked(
    /* [in] */ Int32 displayId)
{
    // Create the DisplayContentsAnimator object by retrieving it.
    GetDisplayContentsAnimatorLocked(displayId);
    if (displayId == IDisplay::DEFAULT_DISPLAY) {
        mInitialized = TRUE;
    }
}

void WindowAnimator::RemoveDisplayLocked(
    /* [in] */ Int32 displayId)
{
    AutoPtr<DisplayContentsAnimator> displayAnimator;
    HashMap<Int32, AutoPtr<DisplayContentsAnimator> >::Iterator it
            =  mDisplayContentsAnimators.Find(displayId);
    if (it != mDisplayContentsAnimators.End()) {
        displayAnimator = it->mSecond;
        if (displayAnimator != NULL) {
            if (displayAnimator->mScreenRotationAnimation != NULL) {
                displayAnimator->mScreenRotationAnimation->Kill();
                displayAnimator->mScreenRotationAnimation = NULL;
            }
        }

        mDisplayContentsAnimators.Erase(it);
    }
}

void WindowAnimator::HideWallpapersLocked(
    /* [in] */ WindowState* w)
{
    AutoPtr<WindowState> wallpaperTarget = mService->mWallpaperTarget;
    AutoPtr<WindowState> lowerWallpaperTarget = mService->mLowerWallpaperTarget;
    List< AutoPtr<WindowToken> > wallpaperTokens = mService.mWallpaperTokens;
    if ((wallpaperTarget == w && lowerWallpaperTarget == NULL) || wallpaperTarget == NULL) {
        List<AutoPtr<WindowToken> >::ReverseIterator rit = wallpaperTokens.RBegin();
        for (; rit != wallpaperTokens.REnd(); ++rit) {
            AutoPtr<WindowToken> token = *rit;
            List<AutoPtr<WindowState> >::ReverseIterator winRIt = token->mWindows.RBegin();
            for (; winRIt != token->mWindows.REnd(); ++winRIt) {
                AutoPtr<WindowState> wallpaper = *winRIt;
                AutoPtr<WindowStateAnimator> winAnimator = wallpaper->mWinAnimator;
                if (!winAnimator->mLastHidden) {
                    winAnimator->Hide();
                    mService->DispatchWallpaperVisibility(wallpaper, FALSE);
                    SetPendingLayoutChanges(IDisplay::DEFAULT_DISPLAY,
                            IWindowManagerPolicy::FINISH_LAYOUT_REDO_WALLPAPER);
                }
            }
            // if (WindowManagerService.DEBUG_WALLPAPER_LIGHT && !token.hidden) Slog.d(TAG,
            //         "Hiding wallpaper " + token + " from " + w
            //         + " target=" + wallpaperTarget + " lower=" + lowerWallpaperTarget
            //         + "\n" + Debug.getCallers(5, "  "));
            token->mHidden = TRUE;
        }
    }
}

void WindowAnimator::UpdateAppWindowsLocked(
    /* [in] */ Int32 displayId)
{
    AutoPtr< List<AutoPtr<TaskStack> > > stacks = mService->GetDisplayContentLocked(displayId)->GetStacks();
    List<AutoPtr<TaskStack> >::ReverseIterator stackRit = stacks->RBegin();
    for (; stackRit != stacks->REnd(); ++stackRit) {
        AutoPtr<TaskStack> stack = *stackRit;
        AutoPtr< List<AutoPtr<Task> > > tasks = stack->GetTasks();
        List<AutoPtr<Task> >::ReverseIterator taskRit = tasks->RBegin();
        for (; taskRit != tasks->REnd(); ++taskRit) {
            AppTokenList tokens = (*taskRit)->mAppTokens;
            AppTokenList::ReverseIterator tokenRit = tokens->RBegin();
            for (; tokenRit != tokens->REnd(); ++tokenRit) {
                AutoPtr<AppWindowAnimator> appAnimator = (*tokenRit)->mAppAnimator;
                Boolean wasAnimating = appAnimator->mAnimation != NULL
                        && appAnimator->mAnimation != AppWindowAnimator::sDummyAnimation;
                if (appAnimator->StepAnimationLocked(mCurrentTime)) {
                    mAnimating = TRUE;
                }
                else if (wasAnimating) {
                    // stopped animating, do one more pass through the layout
                    SetAppLayoutChanges(appAnimator,
                            IWindowManagerPolicy::FINISH_LAYOUT_REDO_WALLPAPER,
                            String("appToken ") + appAnimator->mAppToken + String(" done"));
                    if (CWindowManagerService::DEBUG_ANIM) Slogger::V(TAG,
                            "updateWindowsApps...: done animating %p", appAnimator->mAppToken.Get());
                }
            }
        }

        AppTokenList exitingAppTokens = stack->mExitingAppTokens;
        AppTokenList::Iterator tokenIt = exitingAppTokens.Begin();
        for (; tokenIt != exitingAppTokens.End(); ++tokenIt) {
            AutoPtr<AppWindowAnimator> appAnimator = (*tokenIt)->mAppAnimator;
            Boolean wasAnimating = appAnimator->mAnimation != NULL
                    && appAnimator->mAnimation != AppWindowAnimator::sDummyAnimation;
            if (appAnimator->StepAnimationLocked(mCurrentTime)) {
                mAnimating = TRUE;
            }
            else if (wasAnimating) {
                // stopped animating, do one more pass through the layout
                SetAppLayoutChanges(appAnimator, IWindowManagerPolicy::FINISH_LAYOUT_REDO_WALLPAPER,
                        String("exiting appToken ") + appAnimator->mAppToken + String(" done"));
                if (CWindowManagerService::DEBUG_ANIM) Slogger::V(TAG,
                        "updateWindowsApps...: done animating exiting %p", appAnimator->mAppToken.Get());
            }
        }
    }
}

void WindowAnimator::UpdateWindowsLocked(
    /* [in] */ Int32 displayId)
{
    ++mAnimTransactionSequence;
    AutoPtr<WindowList> windows = mService->GetWindowListLocked(displayId);
    List<AutoPtr<WindowStateAnimator> > unForceHiding;
    Boolean wallpaperInUnForceHiding = FALSE;
    AutoPtr<WindowState> wallpaper;

    if (mKeyguardGoingAway) {
        WindowList::ReverseIterator winRit = windows->RBegin();
        for (; winRit != windows->REnd(); ++winRit) {
            AutoPtr<WindowState> win = *winRit;
            Boolean isKeyguardHostWindow;
            if (mPolicy->IsKeyguardHostWindow(win->mAttrs, &isKeyguardHostWindow), !isKeyguardHostWindow) {
                continue;
            }
            AutoPtr<WindowStateAnimator> winAnimator = win->mWinAnimator;
            Int32 privateFlags;
            if (win->mAttrs->GetPrivateFlags(&privateFlags), (privateFlags & PRIVATE_FLAG_KEYGUARD) != 0) {
                if (!winAnimator->mAnimating) {
                    // Create a new animation to delay until keyguard is gone on its own.
                    winAnimator->mAnimation = NULL;
                    CAlphaAnimation::New(1.0f, 1.0f, (IAlphaAnimation**)&(winAnimator->mAnimation));
                    winAnimator->mAnimation->SetDuration(KEYGUARD_ANIM_TIMEOUT_MS);
                    winAnimator->mAnimationIsEntrance = FALSE;
                }
            }
            else {
                mKeyguardGoingAway = FALSE;
                winAnimator->ClearAnimation();
            }
            break;
        }
    }

    mForceHiding = KEYGUARD_NOT_SHOWN;

    AutoPtr<WindowState> imeTarget = mService->mInputMethodTarget;
    AutoPtr<IWindowManagerLayoutParams> lp;
    imeTarget->GetAttrs((IWindowManagerLayoutParams**)&lp);
    Int32 flags;
    Boolean showImeOverKeyguard = imeTarget != NULL && imeTarget->IsVisibleNow() &&
            (lp->GetFlags(&flags), (flags & FLAG_SHOW_WHEN_LOCKED) != 0);
    AutoPtr<IWindowState> ws;
    mPolicy->GetWinShowWhenLockedLw((IWindowState**)&ws);
    AutoPtr<WindowState> winShowWhenLocked = (WindowState*)ws.Get();
    AutoPtr<AppWindowToken> appShowWhenLocked = winShowWhenLocked == NULL ? NULL : winShowWhenLocked.mAppToken;

    WindowList::ReverseIterator winRit = windows->RBegin();
    for (; winRit != windows->REnd(); ++winRit) {
        AutoPtr<WindowState> win = *winRit;
        AutoPtr<WindowStateAnimator> winAnimator = win->mWinAnimator;
        Int32 flags;
        win->mAttrs->GetFlags(&flags);

        if (winAnimator->mSurfaceControl != NULL) {
            Boolean wasAnimating = winAnimator->mWasAnimating;
            Boolean nowAnimating = winAnimator->StepAnimationLocked(mCurrentTime);

            if (CWindowManagerService::DEBUG_WALLPAPER) {
                Slogger::V(TAG, "%p: wasAnimating=%d, nowAnimating=%d", win.Get(), wasAnimating, nowAnimating);
            }

            if (wasAnimating && !winAnimator->mAnimating && mService->mWallpaperTarget == win) {
                mBulkUpdateParams |= CWindowManagerService::LayoutFields::SET_WALLPAPER_MAY_CHANGE;
                SetPendingLayoutChanges(IDisplay::DEFAULT_DISPLAY,
                        IWindowManagerPolicy::FINISH_LAYOUT_REDO_WALLPAPER);
                if (CWindowManagerService::DEBUG_LAYOUT_REPEATS) {
                    mService->DebugLayoutRepeats(String("updateWindowsAndWallpaperLocked 2"),
                            GetPendingLayoutChanges(IDisplay::DEFAULT_DISPLAY));
                }
            }

            if (mPolicy->IsForceHiding(win->mAttrs, &isForceHiding), isForceHiding) {
                if (!wasAnimating && nowAnimating) {
                    // if (WindowManagerService.DEBUG_ANIM ||
                    //         WindowManagerService.DEBUG_VISIBILITY) Slog.v(TAG,
                    //         "Animation started that could impact force hide: " + win);
                    mBulkUpdateParams |= CWindowManagerService::LayoutFields::SET_FORCE_HIDING_CHANGED;
                    SetPendingLayoutChanges(displayId, IWindowManagerPolicy::FINISH_LAYOUT_REDO_WALLPAPER);
                    if (CWindowManagerService::DEBUG_LAYOUT_REPEATS) {
                        mService->DebugLayoutRepeats(String("updateWindowsAndWallpaperLocked 3"),
                                GetPendingLayoutChanges(displayId));
                    }
                    mService->mFocusMayChange = TRUE;
                }
                else if (mKeyguardGoingAway && !nowAnimating) {
                    // Timeout!!
                    Slogger::E(TAG, "Timeout waiting for animation to startup");
                    mPolicy->StartKeyguardExitAnimation(0, 0);
                    mKeyguardGoingAway = FALSE;
                }
                if (win->IsReadyForDisplay()) {
                    if (nowAnimating) {
                        if (winAnimator->mAnimationIsEntrance) {
                            mForceHiding = KEYGUARD_ANIMATING_IN;
                        }
                        else {
                            mForceHiding = KEYGUARD_ANIMATING_OUT;
                        }
                    }
                    else {
                        mForceHiding = win->IsDrawnLw() ? KEYGUARD_SHOWN : KEYGUARD_NOT_SHOWN;
                    }
                }
                // if (WindowManagerService.DEBUG_VISIBILITY) Slog.v(TAG,
                //         "Force hide " + mForceHiding
                //         + " hasSurface=" + win.mHasSurface
                //         + " policyVis=" + win.mPolicyVisibility
                //         + " destroying=" + win.mDestroying
                //         + " attHidden=" + win.mAttachedHidden
                //         + " vis=" + win.mViewVisibility
                //         + " hidden=" + win.mRootToken.hidden
                //         + " anim=" + win.mWinAnimator.mAnimation);
            }
            else {
                mPolicy->CanBeForceHidden(win, win->mAttrs, &result);
                if (result) {
                    Boolean hideWhenLocked = !((win->mIsImWindow && showImeOverKeyguard) ||
                            (appShowWhenLocked != NULL && appShowWhenLocked == win->mAppToken.Get()));
                    Boolean changed;
                    if (((mForceHiding == KEYGUARD_ANIMATING_IN)
                                && (!winAnimator->IsAnimating() || hideWhenLocked))
                            || ((mForceHiding == KEYGUARD_SHOWN) && hideWhenLocked)) {
                        changed = win->HideLw(FALSE, FALSE);
                        // if (WindowManagerService.DEBUG_VISIBILITY && changed) Slog.v(TAG,
                        //         "Now policy hidden: " + win);
                    }
                    else {
                        changed = win->ShowLw(FALSE, FALSE);
                        // if (WindowManagerService.DEBUG_VISIBILITY && changed) Slog.v(TAG,
                        //         "Now policy shown: " + win);
                        if (changed) {
                            if ((mBulkUpdateParams & CWindowManagerService::LayoutFields::SET_FORCE_HIDING_CHANGED) != 0
                                    && win->IsVisibleNow() /*w.isReadyForDisplay()*/) {
                                unForceHiding.PushBack(winAnimator);
                                if ((flags & IWindowManagerLayoutParams::FLAG_SHOW_WALLPAPER) != 0) {
                                    wallpaperInUnForceHiding = TRUE;
                                }
                            }
                            AutoPtr<WindowState> currentFocus = mService->mCurrentFocus;
                            if (currentFocus == NULL || currentFocus->mLayer < win->mLayer) {
                                // We are showing on to of the current
                                // focus, so re-evaluate focus to make
                                // sure it is correct.
                                if (CWindowManagerService::DEBUG_FOCUS_LIGHT) Slogger::V(TAG,
                                        "updateWindowsLocked: setting mFocusMayChange true");
                                mService->mFocusMayChange = TRUE;
                            }
                        }
                    }
                    if (changed && (flags & IWindowManagerLayoutParams::FLAG_SHOW_WALLPAPER) != 0) {
                        mBulkUpdateParams |= CWindowManagerService::LayoutFields::SET_WALLPAPER_MAY_CHANGE;
                        SetPendingLayoutChanges(IDisplay::DEFAULT_DISPLAY,
                                IWindowManagerPolicy::FINISH_LAYOUT_REDO_WALLPAPER);
                        if (CWindowManagerService::DEBUG_LAYOUT_REPEATS) {
                            mService->DebugLayoutRepeats(String("updateWindowsAndWallpaperLocked 4"),
                                    GetPendingLayoutChanges(IDisplay::DEFAULT_DISPLAY));
                        }
                    }
                }
            }
        }

        AutoPtr<AppWindowToken> atoken = win->mAppToken;
        if (winAnimator->mDrawState == WindowStateAnimator::READY_TO_SHOW) {
            if (atoken == NULL || atoken->mAllDrawn) {
                if (winAnimator->PerformShowLocked()) {
                    SetPendingLayoutChanges(displayId, IWindowManagerPolicy::FINISH_LAYOUT_REDO_ANIM);
                    if (CWindowManagerService::DEBUG_LAYOUT_REPEATS) {
                        mService->DebugLayoutRepeats(String("updateWindowsAndWallpaperLocked 5"),
                                GetPendingLayoutChanges(displayId));
                    }
                }
            }
        }
        AutoPtr<AppWindowAnimator> appAnimator = winAnimator->mAppAnimator;
        if (appAnimator != NULL && appAnimator->mThumbnail != NULL) {
            if (appAnimator->mThumbnailTransactionSeq != mAnimTransactionSequence) {
                appAnimator->mThumbnailTransactionSeq = mAnimTransactionSequence;
                appAnimator->mThumbnailLayer = 0;
            }
            if (appAnimator->mThumbnailLayer < winAnimator->mAnimLayer) {
                appAnimator->mThumbnailLayer = winAnimator->mAnimLayer;
            }
        }
        if (win->mIsWallpaper) {
            wallpaper = win;
        }
    } // end forall windows

    // If we have windows that are being show due to them no longer
    // being force-hidden, apply the appropriate animation to them.
    if (unForceHiding.Begin() != unForceHiding.End()) {
        Boolean startKeyguardExit = TRUE;
        List<AutoPtr<WindowStateAnimator> >::ReverseIterator animRIt = unForceHiding.RBegin();
        for (; animRIt != unForceHiding.REnd(); ++animRIt) {
            AutoPtr<IAnimation> a;
            if (!mKeyguardGoingAwayDisableWindowAnimations) {
                mPolicy->CreateForceHideEnterAnimation(wallpaperInUnForceHiding,
                        mKeyguardGoingAwayToNotificationShade, (IAnimation**)&a);
            }
            if (a != NULL) {
                AutoPtr<WindowStateAnimator> winAnimator = *animRIt;
                winAnimator->SetAnimation(a);
                winAnimator->mKeyguardGoingAwayAnimation = TRUE;
                if (startKeyguardExit && mKeyguardGoingAway) {
                    // Do one time only.
                    Int64 offset, duration;
                    a->GetStartOffset(&offset);
                    a->GetDuration(&duration);
                    mPolicy->StartKeyguardExitAnimation(mCurrentTime + offset, duration);
                    mKeyguardGoingAway = FALSE;
                    startKeyguardExit = FALSE;
                }
            }
        }

        // Wallpaper is going away in un-force-hide motion, animate it as well.
        if (!wallpaperInUnForceHiding && wallpaper != NULL
                && !mKeyguardGoingAwayDisableWindowAnimations) {
            AutoPtr<IAnimation> a;
            mPolicy->CreateForceHideWallpaperExitAnimation(mKeyguardGoingAwayToNotificationShade, (IAnimation**)&a);
            if (a != NULL) {
                AutoPtr<WindowStateAnimator> animator = wallpaper->mWinAnimator;
                animator->SetAnimation(a);
            }
        }
    }
}

void WindowAnimator::UpdateWallpaperLocked(
    /* [in] */ Int32 displayId)
{
    mService->GetDisplayContentLocked(displayId)->ResetAnimationBackgroundAnimator();

    AutoPtr<WindowList> windows = mService->GetWindowListLocked(displayId);
    AutoPtr<WindowState> detachedWallpaper;

    WindowList::ReverseIterator rit = windows->RBegin();
    for (; rit != windows->REnd(); ++rit) {
        AutoPtr<WindowState> win = *rit;
        AutoPtr<WindowStateAnimator> winAnimator = win->mWinAnimator;
        if (winAnimator->mSurfaceControl == NULL) {
            continue;
        }

        Int32 flags;
        win->mAttrs->GetFlags(&flags);

        // If this window is animating, make a note that we have
        // an animating window and take care of a request to run
        // a detached wallpaper animation.
        if (winAnimator->mAnimating) {
            if (winAnimator->mAnimation != NULL) {
                Boolean value;
                if ((flags & IWindowManagerLayoutParams::FLAG_SHOW_WALLPAPER) != 0
                        && (winAnimator->mAnimation->GetDetachWallpaper(&value), value)) {
                    detachedWallpaper = win;
                }
                Int32 color;
                winAnimator->mAnimation->GetBackgroundColor(&color);
                if (color != 0) {
                    AutoPtr<TaskStack> stack = win->GetStack();
                    if (stack != NULL) {
                        stack->SetAnimationBackground(winAnimator, color);
                    }
                }
            }
            mAnimating = TRUE;
        }

        // If this window's app token is running a detached wallpaper
        // animation, make a note so we can ensure the wallpaper is
        // displayed behind it.
        AutoPtr<AppWindowAnimator> appAnimator = winAnimator->mAppAnimator;
        if (appAnimator != NULL && appAnimator->mAnimation != NULL
                && appAnimator->mAnimating) {
            Boolean value;
            if ((flags & IWindowManagerLayoutParams::FLAG_SHOW_WALLPAPER) != 0
                    && (appAnimator->mAnimation->GetDetachWallpaper(&value), value)) {
                detachedWallpaper = win;
            }

            Int32 color;
            appAnimator->mAnimation->GetBackgroundColor(&color);
            if (color != 0) {
                AutoPtr<TaskStack> stack = win->GetStack();
                if (stack != NULL) {
                    stack->SetAnimationBackground(winAnimator, color);
                }
            }
        }
    } // end forall windows

    if (mWindowDetachedWallpaper != detachedWallpaper) {
        // if (WindowManagerService.DEBUG_WALLPAPER) Slog.v(TAG,
        //         "Detached wallpaper changed from " + mWindowDetachedWallpaper
        //         + " to " + detachedWallpaper);
        mWindowDetachedWallpaper = detachedWallpaper;
        mBulkUpdateParams |= CWindowManagerService::LayoutFields::SET_WALLPAPER_MAY_CHANGE;
    }
}
// begin from this
void WindowAnimator::TestTokenMayBeDrawnLocked()
{
    // See if any windows have been drawn, so they (and others
    // associated with them) can now be shown.
    List<AutoPtr<AppWindowAnimator> >::Iterator it = mAppAnimators.Begin();
    for (; it != mAppAnimators.End();) {
        AutoPtr<AppWindowAnimator> appAnimator = *it;
        AutoPtr<AppWindowToken> wtoken = appAnimator->GetAppToken();
        if (wtoken == NULL) {
            it = mAppAnimators.Erase(it);
            continue;
        }

        Boolean allDrawn = wtoken->mAllDrawn;
        if (allDrawn != appAnimator->mAllDrawn) {
            appAnimator->mAllDrawn = allDrawn;
            if (allDrawn) {
                // The token has now changed state to having all
                // windows shown...  what to do, what to do?
                if (appAnimator->mFreezingScreen) {
                    appAnimator->ShowAllWindowsLocked();
                    mService->UnsetAppFreezingScreenLocked(wtoken, FALSE, TRUE);
                    // if (WindowManagerService.DEBUG_ORIENTATION) Slog.i(TAG,
                    //         "Setting mOrientationChangeComplete=true because wtoken "
                    //         + wtoken + " numInteresting=" + wtoken.numInterestingWindows
                    //         + " numDrawn=" + wtoken.numDrawnWindows);
                    // This will set mOrientationChangeComplete and cause a pass through layout.
                    SetAppLayoutChanges(appAnimator,
                        IWindowManagerPolicy::FINISH_LAYOUT_REDO_WALLPAPER,
                        String("testTokenMayBeDrawnLocked: freezingScreen"));
                }
                else {
                    SetAppLayoutChanges(appAnimator,
                        IWindowManagerPolicy::FINISH_LAYOUT_REDO_ANIM,
                        String("testTokenMayBeDrawnLocked"));

                    // We can now show all of the drawn windows!
                    List<AutoPtr<AppWindowToken> >::Iterator awtIt;
                    awtIt = Find(mService->mOpeningApps.Begin(), mService->mOpeningApps.End(), wtoken);
                    if (awtIt == mService->mOpeningApps.End()) {
                        mAnimating |= appAnimator->ShowAllWindowsLocked();
                    }
                }
            }
        }

        ++it;
    }
}

void WindowAnimator::PerformAnimationsLocked(
    /* [in] */ Int32 displayId)
{
    UpdateWindowsLocked(displayId);
    UpdateWallpaperLocked(displayId);
}

void WindowAnimator::AnimateLocked()
{
    if (!mInitialized) {
        return;
    }

    mPendingLayoutChanges.Clear();
    mCurrentTime = SystemClock::GetUptimeMillis();
    mBulkUpdateParams = CWindowManagerService::LayoutFields::SET_ORIENTATION_CHANGE_COMPLETE;
    Boolean wasAnimating = mAnimating;
    mAnimating = FALSE;
    // if (WindowManagerService.DEBUG_WINDOW_TRACE) {
    //     Slog.i(TAG, "!!! animate: entry time=" + mCurrentTime);
    // }

    // if (WindowManagerService.SHOW_TRANSACTIONS) Slog.i(
    //         TAG, ">>> OPEN TRANSACTION animateLocked");
    AutoPtr<ISurfaceHelper> helper;
    CSurfaceHelper::AcquireSingleton((ISurfaceHelper**)&helper);
    helper->OpenTransaction();
    helper->SetAnimationTransaction();
    // try {
    UpdateAppWindowsLocked();

    HashMap<Int32, AutoPtr<DisplayContentsAnimator> >::Iterator it;
    for (it = mDisplayContentsAnimators.Begin(); it != mDisplayContentsAnimators.End(); ++it) {
        Int32 displayId = it->mFirst;
        AutoPtr<DisplayContentsAnimator> displayAnimator = it->mSecond;

        AutoPtr<ScreenRotationAnimation> screenRotationAnimation =
                displayAnimator->mScreenRotationAnimation;
        if (screenRotationAnimation != NULL && screenRotationAnimation->IsAnimating()) {
            if (screenRotationAnimation->StepAnimationLocked(mCurrentTime)) {
                mAnimating = TRUE;
            }
            else {
                mBulkUpdateParams |= CWindowManagerService::LayoutFields::SET_UPDATE_ROTATION;
                screenRotationAnimation->Kill();
                displayAnimator->mScreenRotationAnimation = NULL;
            }
        }

        // Update animations of all applications, including those
        // associated with exiting/removed apps
        PerformAnimationsLocked(displayId);

        List<AutoPtr<WindowStateAnimator> >& winAnimatorList = displayAnimator->mWinAnimators;
        List<AutoPtr<WindowStateAnimator> >::Iterator animIt = winAnimatorList.Begin();
        for (; animIt != winAnimatorList.End(); ++animIt) {
            (*animIt)->PrepareSurfaceLocked(TRUE);
        }
    }

    TestTokenMayBeDrawnLocked();

    for (it = mDisplayContentsAnimators.Begin(); it != mDisplayContentsAnimators.End(); ++it) {
        Int32 displayId = it->mFirst;
        AutoPtr<DisplayContentsAnimator> displayAnimator = it->mSecond;

        AutoPtr<ScreenRotationAnimation> screenRotationAnimation =
                displayAnimator->mScreenRotationAnimation;
        if (screenRotationAnimation != NULL) {
            screenRotationAnimation->UpdateSurfacesInTransaction();
        }

        AutoPtr<DimAnimator::Parameters> dimParams = displayAnimator->mDimParams;
        AutoPtr<DimAnimator> dimAnimator = displayAnimator->mDimAnimator;
        if (dimAnimator != NULL && dimParams != NULL) {
            AutoPtr<IResources> resources;
            mContext->GetResources((IResources**)&resources);
            dimAnimator->UpdateParameters(resources, dimParams, mCurrentTime);
        }
        if (dimAnimator != NULL && dimAnimator->mDimShown) {
            mAnimating |= dimAnimator->UpdateSurface(IsDimmingLocked(displayId),
                    mCurrentTime, !mService->OkToDisplay());
        }
    }

    if (mService->mWatermark != NULL) {
        mService->mWatermark->DrawIfNeeded();
    }
    // } catch (RuntimeException e) {
    //     Log.wtf(TAG, "Unhandled exception in Window Manager", e);
    // } finally {
    //     Surface.closeTransaction();
    //     if (WindowManagerService.SHOW_TRANSACTIONS) Slog.i(
    //             TAG, "<<< CLOSE TRANSACTION animateLocked");
    // }
    helper->CloseTransaction();

    HashMap<Int32, Int32>::Iterator changesIt = mPendingLayoutChanges.Begin();
    for (; changesIt != mPendingLayoutChanges.End(); ++changesIt) {
        if ((changesIt->mSecond
                & IWindowManagerPolicy::FINISH_LAYOUT_REDO_WALLPAPER) != 0) {
            mPendingActions |= WALLPAPER_ACTION_PENDING;
        }
    }

    if (mBulkUpdateParams != 0
            || mPendingLayoutChanges.Begin() != mPendingLayoutChanges.End()) {
        UpdateAnimToLayoutLocked();
    }

    if (mAnimating) {
        AutoLock lock(mService->mLayoutToAnimLock);
        mService->ScheduleAnimationLocked();
    }
    else if (wasAnimating) {
        mService->RequestTraversalLocked();
    }
    // if (WindowManagerService.DEBUG_WINDOW_TRACE) {
    //     Slog.i(TAG, "!!! animate: exit mAnimating=" + mAnimating
    //         + " mBulkUpdateParams=" + Integer.toHexString(mBulkUpdateParams)
    //         + " mPendingLayoutChanges(DEFAULT_DISPLAY)="
    //         + Integer.toHexString(mPendingLayoutChanges.get(Display.DEFAULT_DISPLAY)));
    // }
}

void WindowAnimator::SetCurrentFocus(
    /* [in] */ WindowState* currentFocus)
{
    mCurrentFocus = currentFocus;
}

void WindowAnimator::SetDisplayDimensions(
    /* [in] */ Int32 curWidth,
    /* [in] */ Int32 curHeight,
    /* [in] */ Int32 appWidth,
    /* [in] */ Int32 appHeight)
{
    mDw = curWidth;
    mDh = curHeight;
    mInnerDw = appWidth;
    mInnerDh = appHeight;
}

Boolean WindowAnimator::IsDimmingLocked(
    /* [in] */ Int32 displayId)
{
    return GetDisplayContentsAnimatorLocked(displayId)->mDimParams != NULL;
}

Boolean WindowAnimator::IsDimmingLocked(
    /* [in] */ WindowStateAnimator* winAnimator)
{
    AutoPtr<DimAnimator::Parameters> dimParams =
            GetDisplayContentsAnimatorLocked(winAnimator->mWin->GetDisplayId())->mDimParams;
    return dimParams != NULL && (dimParams->mDimWinAnimator.Get() == winAnimator);
}

String WindowAnimator::BulkUpdateParamsToString(
    /* [in] */ Int32 bulkUpdateParams)
{
    StringBuilder builder(128);
    if ((bulkUpdateParams & CWindowManagerService::LayoutFields::SET_UPDATE_ROTATION) != 0) {
        builder.AppendCStr(" UPDATE_ROTATION");
    }
    if ((bulkUpdateParams & CWindowManagerService::LayoutFields::SET_WALLPAPER_MAY_CHANGE) != 0) {
        builder.AppendCStr(" WALLPAPER_MAY_CHANGE");
    }
    if ((bulkUpdateParams & CWindowManagerService::LayoutFields::SET_FORCE_HIDING_CHANGED) != 0) {
        builder.AppendCStr(" FORCE_HIDING_CHANGED");
    }
    if ((bulkUpdateParams & CWindowManagerService::LayoutFields::SET_ORIENTATION_CHANGE_COMPLETE) != 0) {
        builder.AppendCStr(" ORIENTATION_CHANGE_COMPLETE");
    }
    if ((bulkUpdateParams & CWindowManagerService::LayoutFields::SET_TURN_ON_SCREEN) != 0) {
        builder.AppendCStr(" TURN_ON_SCREEN");
    }
    String result;
    builder.ToString(&result);
    return result;
}

void WindowAnimator::ClearPendingActions()
{
    AutoLock lock(mSelfLock);
    mPendingActions = 0;
}

void WindowAnimator::SetPendingLayoutChanges(
    /* [in] */ Int32 displayId,
    /* [in] */ Int32 changes)
{
    Int32 value = 0;
    HashMap<Int32, Int32>::Iterator it = mPendingLayoutChanges.Find(displayId);
    if (it != mPendingLayoutChanges.End()) {
        value = it->mSecond;
    }

    mPendingLayoutChanges[displayId] = value | changes;
}

void WindowAnimator::SetAppLayoutChanges(
    /* [in] */ AppWindowAnimator* appAnimator,
    /* [in] */ Int32 changes,
    /* [in] */ const String& s)
{
    //begin from this
    // Used to track which displays layout changes have been done.
    HashMap<Int32, Int32> displays;
    List<AutoPtr<WindowStateAnimator> >::ReverseIterator rit
            = appAnimator->mAllAppWinAnimators.RBegin();
    for (; rit != appAnimator->mAllAppWinAnimators.REnd(); ++rit) {
        AutoPtr<WindowStateAnimator> winAnimator = *rit;
        Int32 displayId = winAnimator->mWin->mDisplayContent->GetDisplayId();
        if (displays.Find(displayId) == displays.End()) {
            SetPendingLayoutChanges(displayId, changes);
            if (CWindowManagerService::DEBUG_LAYOUT_REPEATS) {
                Int32 value = 0;
                HashMap<Int32, Int32>::Iterator it = mPendingLayoutChanges.Find(displayId);
                if (it != mPendingLayoutChanges.End()) {
                    value = it->mSecond;
                }
                mService->DebugLayoutRepeats(s, value);
            }
            // Keep from processing this display again.
            displays[displayId] = changes;
        }
    }
}

AutoPtr<WindowAnimator::DisplayContentsAnimator> WindowAnimator::GetDisplayContentsAnimatorLocked(
    /* [in] */ Int32 displayId)
{
    AutoPtr<DisplayContentsAnimator> displayAnimator;
    HashMap<Int32, AutoPtr<DisplayContentsAnimator> >::Iterator it
            = mDisplayContentsAnimators.Find(displayId);
    if (it != mDisplayContentsAnimators.End()) {
        displayAnimator = it->mSecond;
    }
    if (displayAnimator == NULL) {
        displayAnimator = new DisplayContentsAnimator(this, displayId);
        mDisplayContentsAnimators[displayId] = displayAnimator;
    }
    return displayAnimator;
}

void WindowAnimator::SetScreenRotationAnimationLocked(
    /* [in] */ Int32 displayId,
    /* [in] */ ScreenRotationAnimation* animation)
{
    GetDisplayContentsAnimatorLocked(displayId)->mScreenRotationAnimation = animation;
}

AutoPtr<ScreenRotationAnimation> WindowAnimator::GetScreenRotationAnimationLocked(
    /* [in] */ Int32 displayId)
{
    return GetDisplayContentsAnimatorLocked(displayId)->mScreenRotationAnimation;
}


} // Wm
} // Server
} // Droid
} // Elastos
