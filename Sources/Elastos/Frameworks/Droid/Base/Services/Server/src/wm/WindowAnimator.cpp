
#include "wm/WindowAnimator.h"
#include "os/SystemClock.h"
#include "os/Handler.h"
#include <elastos/core/StringBuilder.h>
#include <elastos/core/StringUtils.h>

using Elastos::Core::StringUtils;
using Elastos::Core::StringBuilder;
using Elastos::Droid::Os::SystemClock;
using Elastos::Droid::View::ISurfaceHelper;
using Elastos::Droid::View::CSurfaceHelper;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Wm {

const Int32 WindowAnimator::WALLPAPER_ACTION_PENDING = 1;
const String WindowAnimator::TAG("WindowAnimator");
const Int32 WindowAnimator::KEYGUARD_NOT_SHOWN     = 0;
const Int32 WindowAnimator::KEYGUARD_ANIMATING_IN  = 1;
const Int32 WindowAnimator::KEYGUARD_SHOWN         = 2;
const Int32 WindowAnimator::KEYGUARD_ANIMATING_OUT = 3;


WindowAnimator::AnimatorToLayoutParams::AnimatorToLayoutParams()
    : mUpdateQueued(FALSE)
    , mBulkUpdateParams(0)
{
}

WindowAnimator::AnimationRunnable::AnimationRunnable(
    /* [in] */ WindowAnimator* host)
    : mHost(host)
{
}

ECode WindowAnimator::AnimationRunnable::Run()
{
    // TODO(cmautner): When full isolation is achieved for animation, the first lock
    // goes away and only the WindowAnimator.this remains.
    Object::Autolock lock(mHost->mService->mWindowMapLock);
    {
        Mutex::Autolock lock(mHost->mSelfLock);
        mHost->CopyLayoutToAnimParamsLocked();
        mHost->AnimateLocked();
    }

    return NOERROR;
}

WindowAnimator::DisplayContentsAnimator::DisplayContentsAnimator(
    /* [in] */ WindowAnimator* host,
    /* [in] */ Int32 displayId)
    : mHost(host)
{
    mDimAnimator = new DimAnimator(mHost->mService->mFxSession, displayId);
    mWindowAnimationBackgroundSurface =
            new DimSurface(mHost->mService->mFxSession, displayId);
}

WindowAnimator::WindowAnimator(
    /* [in] */ CWindowManagerService* service)
    : mService(service)
    , mAnimating(FALSE)
    , mAdjResult(0)
    , mDw(0)
    , mDh(0)
    , mInnerDw(0)
    , mInnerDh(0)
    , mCurrentTime(0)
    , mAboveUniverseLayer(0)
    , mBulkUpdateParams(0)
    , mPendingActions(0)
    , mInitialized(FALSE)
    , mForceHiding(KEYGUARD_NOT_SHOWN)
{
    mContext = service->mContext;
    mPolicy = service->mPolicy;

    mAnimationRunnable = new AnimationRunnable(this);

    mAnimToLayout = new AnimatorToLayoutParams();
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
            if (displayAnimator->mWindowAnimationBackgroundSurface != NULL) {
                displayAnimator->mWindowAnimationBackgroundSurface->Kill();
                displayAnimator->mWindowAnimationBackgroundSurface = NULL;
            }
            if (displayAnimator->mScreenRotationAnimation != NULL) {
                displayAnimator->mScreenRotationAnimation->Kill();
                displayAnimator->mScreenRotationAnimation = NULL;
            }
            if (displayAnimator->mDimAnimator != NULL) {
                displayAnimator->mDimAnimator->Kill();
                displayAnimator->mDimAnimator = NULL;
            }
        }

        mDisplayContentsAnimators.Erase(it);
    }
}

void WindowAnimator::UpdateAnimToLayoutLocked()
{
    Mutex::Autolock lock(mAnimToLayoutLock);
    mAnimToLayout->mBulkUpdateParams = mBulkUpdateParams;
    mAnimToLayout->mPendingLayoutChanges
            = new HashMap<Int32, Int32>(
                    mPendingLayoutChanges.Begin(), mPendingLayoutChanges.End());
    mAnimToLayout->mWindowDetachedWallpaper = mWindowDetachedWallpaper;

    if (!mAnimToLayout->mUpdateQueued) {
        mAnimToLayout->mUpdateQueued = TRUE;
        Boolean result;
        mService->mH->SendEmptyMessage(CWindowManagerService::H::UPDATE_ANIM_PARAMETERS, &result);
    }
}

void WindowAnimator::CopyLayoutToAnimParamsLocked()
{
    AutoPtr<CWindowManagerService::LayoutToAnimatorParams> layoutToAnim
            = mService->mLayoutToAnim;

    Mutex::Autolock lock(mService->mLayoutToAnimLock);

    layoutToAnim->mAnimationScheduled = FALSE;

    if (!layoutToAnim->mParamsModified) {
        return;
    }
    layoutToAnim->mParamsModified = FALSE;

    if ((layoutToAnim->mChanges & CWindowManagerService::LayoutToAnimatorParams::WALLPAPER_TOKENS_CHANGED) != 0) {
        layoutToAnim->mChanges &= ~CWindowManagerService::LayoutToAnimatorParams::WALLPAPER_TOKENS_CHANGED;
        mWallpaperTokens.Assign(layoutToAnim->mWallpaperTokens.Begin(),
                layoutToAnim->mWallpaperTokens.End());
    }

    // if (WindowManagerService.DEBUG_WALLPAPER_LIGHT) {
    //     if (mWallpaperTarget != layoutToAnim.mWallpaperTarget
    //             || mLowerWallpaperTarget != layoutToAnim.mLowerWallpaperTarget
    //             || mUpperWallpaperTarget != layoutToAnim.mUpperWallpaperTarget) {
    //         Slog.d(TAG, "Pulling anim wallpaper: target=" + layoutToAnim.mWallpaperTarget
    //                 + " lower=" + layoutToAnim.mLowerWallpaperTarget + " upper="
    //                 + layoutToAnim.mUpperWallpaperTarget);
    //     }
    // }
    mWallpaperTarget = layoutToAnim->mWallpaperTarget;
    mWpAppAnimator = mWallpaperTarget == NULL
            ? NULL : mWallpaperTarget->mAppToken == NULL
                    ? NULL : mWallpaperTarget->mAppToken->mAppAnimator;
    mLowerWallpaperTarget = layoutToAnim->mLowerWallpaperTarget;
    mUpperWallpaperTarget = layoutToAnim->mUpperWallpaperTarget;

    // Set the new DimAnimator params.
    HashMap<Int32, AutoPtr<DisplayContentsAnimator> >::Iterator it =
            mDisplayContentsAnimators.Begin();
    for (; it != mDisplayContentsAnimators.End(); ++it) {
        Int32 displayId = it->mFirst;
        AutoPtr<DisplayContentsAnimator> displayAnimator = it->mSecond;

        displayAnimator->mWinAnimators.Clear();
        AutoPtr<List< AutoPtr<WindowStateAnimator> > > winAnimators;
        HashMap<Int32, AutoPtr<List<AutoPtr<WindowStateAnimator> > > >::Iterator listsIt
                = layoutToAnim->mWinAnimatorLists.Find(displayId);
        if (listsIt != layoutToAnim->mWinAnimatorLists.End()) {
            winAnimators = listsIt->mSecond;
        }
        if (winAnimators != NULL) {
            displayAnimator->mWinAnimators.Assign(winAnimators->Begin(),
                    winAnimators->End());
        }

        AutoPtr<DimAnimator::Parameters> dimParams;
        HashMap<Int32, AutoPtr<DimAnimator::Parameters> >::Iterator paramsIt
                = layoutToAnim->mDimParams.Find(displayId);
        if (paramsIt != layoutToAnim->mDimParams.End()) {
            dimParams = paramsIt->mSecond;
        }
        if (dimParams == NULL) {
            displayAnimator->mDimParams = NULL;
        }
        else {
            AutoPtr<WindowStateAnimator> newWinAnimator = dimParams->mDimWinAnimator;

            // Only set dim params on the highest dimmed layer.
            AutoPtr<WindowStateAnimator> existingDimWinAnimator =
                    displayAnimator->mDimParams == NULL ?
                            NULL : displayAnimator->mDimParams->mDimWinAnimator;
            // Don't turn on for an unshown surface, or for any layer but the highest
            // dimmed layer.
            if (newWinAnimator->mSurfaceShown && (existingDimWinAnimator == NULL
                    || !existingDimWinAnimator->mSurfaceShown
                    || existingDimWinAnimator->mAnimLayer < newWinAnimator->mAnimLayer)) {
                displayAnimator->mDimParams = new DimAnimator::Parameters(dimParams);
            }
        }
    }

    mAppAnimators.Clear();
    List< AutoPtr<CWindowManagerService::AppWindowAnimParams> >::Iterator paramsIt
            = layoutToAnim->mAppWindowAnimParams.Begin();
    for (; paramsIt != layoutToAnim->mAppWindowAnimParams.End(); ++paramsIt) {
        AutoPtr<CWindowManagerService::AppWindowAnimParams> params = *paramsIt;
        AutoPtr<AppWindowAnimator> appAnimator = params->mAppAnimator;
        appAnimator->mAllAppWinAnimators.Clear();
        appAnimator->mAllAppWinAnimators.Assign(params->mWinAnimators.Begin(),
                params->mWinAnimators.End());
        mAppAnimators.PushBack(appAnimator);
    }
}

void WindowAnimator::HideWallpapersLocked(
    /* [in] */ WindowState* w,
    /* [in] */ Boolean fromAnimator)
{
    // There is an issue where this function can be called either from
    // the animation or the layout side of the window manager.  The problem
    // is that if it is called from the layout side, we may not yet have
    // propagated the current layout wallpaper state over into the animation
    // state.  If that is the case, we can do bad things like hide the
    // wallpaper when we had just made it shown because the animation side
    // doesn't yet see that there is now a wallpaper target.  As a temporary
    // work-around, we tell the function here which side of the window manager
    // is calling so it can use the right state.
    if (fromAnimator) {
        HideWallpapersLocked(w, mWallpaperTarget, mLowerWallpaperTarget, mWallpaperTokens);
    }
    else {
        HideWallpapersLocked(w, mService->mWallpaperTarget,
                mService->mLowerWallpaperTarget, mService->mWallpaperTokens);
    }
}

void WindowAnimator::HideWallpapersLocked(
    /* [in] */ WindowState* w,
    /* [in] */ WindowState* wallpaperTarget,
    /* [in] */ WindowState* lowerWallpaperTarget,
    /* [in] */ List<AutoPtr<WindowToken> >& wallpaperTokens)
{
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

void WindowAnimator::UpdateAppWindowsLocked()
{
    List<AutoPtr<AppWindowAnimator> >::Iterator it = mAppAnimators.Begin();
    for (; it != mAppAnimators.End(); ++it) {
        AutoPtr<AppWindowAnimator> appAnimator = *it;
        Boolean wasAnimating = appAnimator->mAnimation != NULL
                && appAnimator->mAnimation != AppWindowAnimator::sDummyAnimation;
        if (appAnimator->StepAnimationLocked(mCurrentTime, mInnerDw, mInnerDh)) {
            mAnimating = TRUE;
        }
        else if (wasAnimating) {
            String info("appToken ");
            AutoPtr<AppWindowToken> token = appAnimator->GetAppToken();
            if (token) {
                info += token->ToString();
            }
            else {
                info += "dead AppWindowToken";
            }
            info += " done";

            // stopped animating, do one more pass through the layout
            SetAppLayoutChanges(appAnimator, IWindowManagerPolicy::FINISH_LAYOUT_REDO_WALLPAPER, info);
            // if (WindowManagerService.DEBUG_ANIM) Slog.v(TAG,
            //         "updateWindowsApps...: done animating " + appAnimator.mAppToken);
        }
    }

    List<AutoPtr<AppWindowToken> >::Iterator appWinIt = mService->mExitingAppTokens.Begin();
    for (; appWinIt != mService->mExitingAppTokens.End(); ++appWinIt) {
        AutoPtr<AppWindowAnimator> appAnimator = (*appWinIt)->mAppAnimator;
        Boolean wasAnimating = appAnimator->mAnimation != NULL
                && appAnimator->mAnimation != AppWindowAnimator::sDummyAnimation;
        if (appAnimator->StepAnimationLocked(mCurrentTime, mInnerDw, mInnerDh)) {
            mAnimating = TRUE;
        }
        else if (wasAnimating) {
            String info("exiting appToken ");
            AutoPtr<AppWindowToken> token = appAnimator->GetAppToken();
            if (token) {
                info += token->ToString();
            }
            else {
                info += "dead AppWindowToken";
            }
            info += " done";

            // stopped animating, do one more pass through the layout
            SetAppLayoutChanges(appAnimator, IWindowManagerPolicy::FINISH_LAYOUT_REDO_WALLPAPER, info);
            // if (WindowManagerService.DEBUG_ANIM) Slog.v(TAG,
            //         "updateWindowsApps...: done animating exiting " + appAnimator.mAppToken);
        }
    }
}

void WindowAnimator::UpdateWindowsLocked(
    /* [in] */ Int32 displayId)
{
    ++mAnimTransactionSequence;

    List<AutoPtr<WindowStateAnimator> >& winAnimatorList =
            GetDisplayContentsAnimatorLocked(displayId)->mWinAnimators;
    List<AutoPtr<WindowStateAnimator> > unForceHiding;
    Boolean wallpaperInUnForceHiding = FALSE;
    mForceHiding = KEYGUARD_NOT_SHOWN;

    List<AutoPtr<WindowStateAnimator> >::ReverseIterator rit = winAnimatorList.RBegin();
    for (; rit != winAnimatorList.REnd(); ++rit) {
        AutoPtr<WindowStateAnimator> winAnimator = *rit;
        AutoPtr<WindowState> win = winAnimator->mWin;
        Int32 flags = winAnimator->mAttrFlags;

        if (winAnimator->mSurface != NULL) {
            Boolean wasAnimating = winAnimator->mWasAnimating;
            Boolean nowAnimating = winAnimator->StepAnimationLocked(mCurrentTime);

            // if (WindowManagerService.DEBUG_WALLPAPER) {
            //     Slog.v(TAG, win + ": wasAnimating=" + wasAnimating +
            //             ", nowAnimating=" + nowAnimating);
            // }

            if (wasAnimating && !winAnimator->mAnimating && mWallpaperTarget == win) {
                mBulkUpdateParams |= CWindowManagerService::LayoutFields::SET_WALLPAPER_MAY_CHANGE;
                SetPendingLayoutChanges(IDisplay::DEFAULT_DISPLAY,
                        IWindowManagerPolicy::FINISH_LAYOUT_REDO_WALLPAPER);
                if (CWindowManagerService::DEBUG_LAYOUT_REPEATS) {
                    Int32 value = 0;
                    IntIterator it = mPendingLayoutChanges.Find(IDisplay::DEFAULT_DISPLAY);
                    if (it != mPendingLayoutChanges.End()) {
                        value = it->mSecond;
                    }
                    mService->DebugLayoutRepeats(String("updateWindowsAndWallpaperLocked 2"), value);
                }
            }

            Boolean result;
            mPolicy->DoesForceHide(win, win->mAttrs, &result);
            if (result) {
                if (!wasAnimating && nowAnimating) {
                    // if (WindowManagerService.DEBUG_ANIM ||
                    //         WindowManagerService.DEBUG_VISIBILITY) Slog.v(TAG,
                    //         "Animation started that could impact force hide: " + win);
                    mBulkUpdateParams |= CWindowManagerService::LayoutFields::SET_FORCE_HIDING_CHANGED;
                    SetPendingLayoutChanges(displayId,
                            IWindowManagerPolicy::FINISH_LAYOUT_REDO_WALLPAPER);
                    if (CWindowManagerService::DEBUG_LAYOUT_REPEATS) {
                        Int32 value = 0;
                        IntIterator it = mPendingLayoutChanges.Find(displayId);
                        if (it != mPendingLayoutChanges.End()) {
                            value = it->mSecond;
                        }
                        mService->DebugLayoutRepeats(String("updateWindowsAndWallpaperLocked 3"), value);
                    }
                    mService->mFocusMayChange = TRUE;
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
                        mForceHiding = KEYGUARD_SHOWN;
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
                    Boolean hideWhenLocked =
                            (winAnimator->mAttrFlags & IWindowManagerLayoutParams::FLAG_SHOW_WHEN_LOCKED) == 0;
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
                            if (mCurrentFocus == NULL || mCurrentFocus->mLayer < win->mLayer) {
                                // We are showing on to of the current
                                // focus, so re-evaluate focus to make
                                // sure it is correct.
                                mService->mFocusMayChange = TRUE;
                            }
                        }
                    }
                    if (changed && (flags & IWindowManagerLayoutParams::FLAG_SHOW_WALLPAPER) != 0) {
                        mBulkUpdateParams |= CWindowManagerService::LayoutFields::SET_WALLPAPER_MAY_CHANGE;
                        SetPendingLayoutChanges(IDisplay::DEFAULT_DISPLAY,
                                IWindowManagerPolicy::FINISH_LAYOUT_REDO_WALLPAPER);
                        if (CWindowManagerService::DEBUG_LAYOUT_REPEATS) {
                            Int32 value = 0;
                            IntIterator it = mPendingLayoutChanges.Find(IDisplay::DEFAULT_DISPLAY);
                            if (it != mPendingLayoutChanges.End()) {
                                value = it->mSecond;
                            }
                            mService->DebugLayoutRepeats(String("updateWindowsAndWallpaperLocked 4"), value);
                        }
                    }
                }
            }
        }

        AutoPtr<AppWindowToken> atoken = win->mAppToken;
        if (winAnimator->mDrawState == WindowStateAnimator::READY_TO_SHOW) {
            if (atoken == NULL || atoken->mAllDrawn) {
                if (winAnimator->PerformShowLocked()) {
                    mPendingLayoutChanges[displayId]= IWindowManagerPolicy::FINISH_LAYOUT_REDO_ANIM;
                    if (CWindowManagerService::DEBUG_LAYOUT_REPEATS) {
                        Int32 value = 0;
                        IntIterator it = mPendingLayoutChanges.Find(displayId);
                        if (it != mPendingLayoutChanges.End()) {
                            value = it->mSecond;
                        }
                        mService->DebugLayoutRepeats(String("updateWindowsAndWallpaperLocked 5"), value);
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
    } // end forall windows

    // If we have windows that are being show due to them no longer
    // being force-hidden, apply the appropriate animation to them.
    if (unForceHiding.IsEmpty() == FALSE) {
        List<AutoPtr<WindowStateAnimator> >::ReverseIterator animRIt = unForceHiding.RBegin();
        for (; animRIt != unForceHiding.REnd(); ++animRIt) {
            AutoPtr<IAnimation> a;
            mPolicy->CreateForceHideEnterAnimation(wallpaperInUnForceHiding, (IAnimation**)&a);
            if (a != NULL) {
                AutoPtr<WindowStateAnimator> winAnimator = *animRIt;
                winAnimator->SetAnimation(a);
                winAnimator->mAnimationIsEntrance = TRUE;
            }
        }
    }
}

void WindowAnimator::UpdateWallpaperLocked(
    /* [in] */ Int32 displayId)
{
    AutoPtr<DisplayContentsAnimator> displayAnimator =
            GetDisplayContentsAnimatorLocked(displayId);
    List<AutoPtr<WindowStateAnimator> >& winAnimatorList = displayAnimator->mWinAnimators;
    AutoPtr<WindowStateAnimator> windowAnimationBackground;
    Int32 windowAnimationBackgroundColor = 0;
    AutoPtr<WindowState> detachedWallpaper;
    AutoPtr<DimSurface> windowAnimationBackgroundSurface =
            displayAnimator->mWindowAnimationBackgroundSurface;

    List<AutoPtr<WindowStateAnimator> >::ReverseIterator rit = winAnimatorList.RBegin();
    for (; rit != winAnimatorList.REnd(); ++rit) {
        AutoPtr<WindowStateAnimator> winAnimator = *rit;
        if (winAnimator->mSurface == NULL) {
            continue;
        }

        Int32 flags = winAnimator->mAttrFlags;
        AutoPtr<WindowState> win = winAnimator->mWin;

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
                Int32 backgroundColor;
                winAnimator->mAnimation->GetBackgroundColor(&backgroundColor);
                if (backgroundColor != 0) {
                    if (windowAnimationBackground == NULL || (winAnimator->mAnimLayer <
                            windowAnimationBackground->mAnimLayer)) {
                        windowAnimationBackground = winAnimator;
                        windowAnimationBackgroundColor = backgroundColor;
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

            Int32 backgroundColor;
            appAnimator->mAnimation->GetBackgroundColor(&backgroundColor);
            if (backgroundColor != 0) {
                if (windowAnimationBackground == NULL || (winAnimator->mAnimLayer <
                        windowAnimationBackground->mAnimLayer)) {
                    windowAnimationBackground = winAnimator;
                    windowAnimationBackgroundColor = backgroundColor;
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

    if (windowAnimationBackgroundColor != 0) {
        // If the window that wants black is the current wallpaper
        // target, then the black goes *below* the wallpaper so we
        // don't cause the wallpaper to suddenly disappear.
        Int32 animLayer = windowAnimationBackground->mAnimLayer;
        AutoPtr<WindowState> win = windowAnimationBackground->mWin;
        if (mWallpaperTarget == win
                || mLowerWallpaperTarget == win || mUpperWallpaperTarget == win) {
            List<AutoPtr<WindowStateAnimator> >::Iterator it = winAnimatorList.Begin();
            for (; it != winAnimatorList.End(); ++it) {
                AutoPtr<WindowStateAnimator> winAnimator = *it;
                if (winAnimator->mIsWallpaper) {
                    animLayer = winAnimator->mAnimLayer;
                    break;
                }
            }
        }

        if (windowAnimationBackgroundSurface != NULL) {
            windowAnimationBackgroundSurface->Show(
                mDw, mDh, animLayer - CWindowManagerService::LAYER_OFFSET_DIM,
                windowAnimationBackgroundColor);
        }
    }
    else {
        if (windowAnimationBackgroundSurface != NULL) {
            windowAnimationBackgroundSurface->Hide();
        }
    }
}

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
        Mutex::Autolock lock(mService->mLayoutToAnimLock);
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
    Mutex::Autolock lock(mSelfLock);
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
