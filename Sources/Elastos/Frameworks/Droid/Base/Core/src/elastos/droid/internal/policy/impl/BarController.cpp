
#include "elastos/droid/internal/policy/impl/BarController.h"

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Policy {
namespace Impl {

//=====================================================================
//                    BarController::InnerRunnable1
//=====================================================================
BarController::InnerRunnable1::InnerRunnable1(
    /* [in] */ BarController* owner)
    : mOwner(owner)
{
}

ECode BarController::InnerRunnable1::Run()
{
    // ==================before translated======================
    // try {
    //     IStatusBarService statusbar = getStatusBarService();
    //     if (statusbar != null) {
    //         statusbar.setWindowState(mStatusBarManagerId, state);
    //     }
    // } catch (RemoteException e) {
    //     if (DEBUG) Slog.w(mTag, "Error posting window state", e);
    //     // re-acquire status bar service next time it is needed.
    //     mStatusBarService = null;
    // }
    assert(0);
    return NOERROR;
}

//=====================================================================
//                            BarController
//=====================================================================
CAR_INTERFACE_IMPL(BarController, Object, IBarController)

const Boolean BarController::DEBUG = false;
const Int32 BarController::TRANSIENT_BAR_NONE;
const Int32 BarController::TRANSIENT_BAR_SHOW_REQUESTED;
const Int32 BarController::TRANSIENT_BAR_SHOWING;
const Int32 BarController::TRANSIENT_BAR_HIDING;
const Int32 BarController::TRANSLUCENT_ANIMATION_DELAY_MS;

BarController::BarController()
{
}

ECode BarController::constructor(
    /* [in] */ const String& tag,
    /* [in] */ Int32 transientFlag,
    /* [in] */ Int32 unhideFlag,
    /* [in] */ Int32 translucentFlag,
    /* [in] */ Int32 statusBarManagerId,
    /* [in] */ Int32 translucentWmFlag)
{
    // ==================before translated======================
    // mTag = "BarController." + tag;
    // mTransientFlag = transientFlag;
    // mUnhideFlag = unhideFlag;
    // mTranslucentFlag = translucentFlag;
    // mStatusBarManagerId = statusBarManagerId;
    // mTranslucentWmFlag = translucentWmFlag;
    // mHandler = new Handler();
    return NOERROR;
}

ECode BarController::SetWindow(
    /* [in] */ IWindowState* win)
{
    VALIDATE_NOT_NULL(win);
    // ==================before translated======================
    // mWin = win;
    assert(0);
    return NOERROR;
}

ECode BarController::ShowTransient()
{
    // ==================before translated======================
    // if (mWin != null) {
    //     setTransientBarState(TRANSIENT_BAR_SHOW_REQUESTED);
    // }
    assert(0);
    return NOERROR;
}

ECode BarController::IsTransientShowing(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mTransientBarState == TRANSIENT_BAR_SHOWING;
    assert(0);
    return NOERROR;
}

ECode BarController::IsTransientShowRequested(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return mTransientBarState == TRANSIENT_BAR_SHOW_REQUESTED;
    assert(0);
    return NOERROR;
}

ECode BarController::WasRecentlyTranslucent(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // return (SystemClock.uptimeMillis() - mLastTranslucent) < TRANSLUCENT_ANIMATION_DELAY_MS;
    assert(0);
    return NOERROR;
}

ECode BarController::AdjustSystemUiVisibilityLw(
    /* [in] */ Int32 oldVis,
    /* [in] */ Int32 vis)
{
    // ==================before translated======================
    // if (mWin != null && mTransientBarState == TRANSIENT_BAR_SHOWING &&
    //         (vis & mTransientFlag) == 0) {
    //     // sysui requests hide
    //     setTransientBarState(TRANSIENT_BAR_HIDING);
    //     setBarShowingLw(false);
    // } else if (mWin != null && (oldVis & mUnhideFlag) != 0 && (vis & mUnhideFlag) == 0) {
    //     // sysui ready to unhide
    //     setBarShowingLw(true);
    // }
    assert(0);
    return NOERROR;
}

ECode BarController::ApplyTranslucentFlagLw(
    /* [in] */ IWindowState* win,
    /* [in] */ Int32 vis,
    /* [in] */ Int32 oldVis,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(win);
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // if (mWin != null) {
    //     if (win != null && (win.getAttrs().privateFlags
    //             & WindowManager.LayoutParams.PRIVATE_FLAG_INHERIT_TRANSLUCENT_DECOR) == 0) {
    //         int fl = PolicyControl.getWindowFlags(win, null);
    //         if ((fl & mTranslucentWmFlag) != 0) {
    //             vis |= mTranslucentFlag;
    //         } else {
    //             vis &= ~mTranslucentFlag;
    //         }
    //         if ((fl & WindowManager.LayoutParams.FLAG_DRAWS_SYSTEM_BAR_BACKGROUNDS) != 0) {
    //             vis |= View.SYSTEM_UI_TRANSPARENT;
    //         } else {
    //             vis &= ~View.SYSTEM_UI_TRANSPARENT;
    //         }
    //     } else {
    //         vis = (vis & ~mTranslucentFlag) | (oldVis & mTranslucentFlag);
    //         vis = (vis & ~View.SYSTEM_UI_TRANSPARENT) | (oldVis & View.SYSTEM_UI_TRANSPARENT);
    //     }
    // }
    // return vis;
    assert(0);
    return NOERROR;
}

ECode BarController::SetBarShowingLw(
    /* [in] */ Boolean show,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // if (mWin == null) return false;
    // if (show && mTransientBarState == TRANSIENT_BAR_HIDING) {
    //     mPendingShow = true;
    //     return false;
    // }
    // final boolean wasVis = mWin.isVisibleLw();
    // final boolean wasAnim = mWin.isAnimatingLw();
    // final boolean change = show ? mWin.showLw(true) : mWin.hideLw(true);
    // final int state = computeStateLw(wasVis, wasAnim, mWin, change);
    // final boolean stateChanged = updateStateLw(state);
    // return change || stateChanged;
    assert(0);
    return NOERROR;
}

ECode BarController::CheckHiddenLw(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // if (mWin != null && mWin.hasDrawnLw()) {
    //     if (!mWin.isVisibleLw() && !mWin.isAnimatingLw()) {
    //         updateStateLw(StatusBarManager.WINDOW_STATE_HIDDEN);
    //     }
    //     if (mTransientBarState == TRANSIENT_BAR_HIDING && !mWin.isVisibleLw()) {
    //         // Finished animating out, clean up and reset style
    //         setTransientBarState(TRANSIENT_BAR_NONE);
    //         if (mPendingShow) {
    //             setBarShowingLw(true);
    //             mPendingShow = false;
    //         }
    //         return true;
    //     }
    // }
    // return false;
    assert(0);
    return NOERROR;
}

ECode BarController::CheckShowTransientBarLw(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // if (mTransientBarState == TRANSIENT_BAR_SHOWING) {
    //     if (DEBUG) Slog.d(mTag, "Not showing transient bar, already shown");
    //     return false;
    // } else if (mTransientBarState == TRANSIENT_BAR_SHOW_REQUESTED) {
    //     if (DEBUG) Slog.d(mTag, "Not showing transient bar, already requested");
    //     return false;
    // } else if (mWin == null) {
    //     if (DEBUG) Slog.d(mTag, "Not showing transient bar, bar doesn't exist");
    //     return false;
    // } else if (mWin.isDisplayedLw()) {
    //     if (DEBUG) Slog.d(mTag, "Not showing transient bar, bar already visible");
    //     return false;
    // } else {
    //     return true;
    // }
    assert(0);
    return NOERROR;
}

ECode BarController::UpdateVisibilityLw(
    /* [in] */ Boolean transientAllowed,
    /* [in] */ Int32 oldVis,
    /* [in] */ Int32 vis,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    // ==================before translated======================
    // if (mWin == null) return vis;
    // if (isTransientShowing() || isTransientShowRequested()) { // transient bar requested
    //     if (transientAllowed) {
    //         vis |= mTransientFlag;
    //         if ((oldVis & mTransientFlag) == 0) {
    //             vis |= mUnhideFlag;  // tell sysui we're ready to unhide
    //         }
    //         setTransientBarState(TRANSIENT_BAR_SHOWING);  // request accepted
    //     } else {
    //         setTransientBarState(TRANSIENT_BAR_NONE);  // request denied
    //     }
    // }
    // if (mTransientBarState != TRANSIENT_BAR_NONE) {
    //     vis |= mTransientFlag;  // ignore clear requests until transition completes
    //     vis &= ~View.SYSTEM_UI_FLAG_LOW_PROFILE;  // never show transient bars in low profile
    // }
    // if ((vis & mTranslucentFlag) != 0 || (oldVis & mTranslucentFlag) != 0 ||
    //         ((vis | oldVis) & View.SYSTEM_UI_FLAG_FULLSCREEN) != 0) {
    //     mLastTranslucent = SystemClock.uptimeMillis();
    // }
    // return vis;
    assert(0);
    return NOERROR;
}

ECode BarController::Dump(
    /* [in] */ IPrintWriter* pw,
    /* [in] */ const String& prefix)
{
    VALIDATE_NOT_NULL(pw);
    // ==================before translated======================
    // if (mWin != null) {
    //     pw.print(prefix); pw.println(mTag);
    //     pw.print(prefix); pw.print("  "); pw.print("mState"); pw.print('=');
    //     pw.println(StatusBarManager.windowStateToString(mState));
    //     pw.print(prefix); pw.print("  "); pw.print("mTransientBar"); pw.print('=');
    //     pw.println(transientBarStateToString(mTransientBarState));
    // }
    assert(0);
    return NOERROR;
}

Int32 BarController::ComputeStateLw(
    /* [in] */ Boolean wasVis,
    /* [in] */ Boolean wasAnim,
    /* [in] */ IWindowState* win,
    /* [in] */ Boolean change)
{
    // ==================before translated======================
    // if (win.hasDrawnLw()) {
    //     final boolean vis = win.isVisibleLw();
    //     final boolean anim = win.isAnimatingLw();
    //     if (mState == StatusBarManager.WINDOW_STATE_HIDING && !change && !vis) {
    //         return StatusBarManager.WINDOW_STATE_HIDDEN;
    //     } else if (mState == StatusBarManager.WINDOW_STATE_HIDDEN && vis) {
    //         return StatusBarManager.WINDOW_STATE_SHOWING;
    //     } else if (change) {
    //         if (wasVis && vis && !wasAnim && anim) {
    //             return StatusBarManager.WINDOW_STATE_HIDING;
    //         } else {
    //             return StatusBarManager.WINDOW_STATE_SHOWING;
    //         }
    //     }
    // }
    // return mState;
    assert(0);
    return 0;
}

Boolean BarController::UpdateStateLw(
    /* [in] */ Int32 state)
{
    // ==================before translated======================
    // if (state != mState) {
    //     mState = state;
    //     if (DEBUG) Slog.d(mTag, "mState: " + StatusBarManager.windowStateToString(state));
    //     mHandler.post(new Runnable() {
    //         @Override
    //         public void run() {
    //             try {
    //                 IStatusBarService statusbar = getStatusBarService();
    //                 if (statusbar != null) {
    //                     statusbar.setWindowState(mStatusBarManagerId, state);
    //                 }
    //             } catch (RemoteException e) {
    //                 if (DEBUG) Slog.w(mTag, "Error posting window state", e);
    //                 // re-acquire status bar service next time it is needed.
    //                 mStatusBarService = null;
    //             }
    //         }
    //     });
    //     return true;
    // }
    // return false;
    assert(0);
    return FALSE;
}

void BarController::SetTransientBarState(
    /* [in] */ Int32 state)
{
    // ==================before translated======================
    // if (mWin != null && state != mTransientBarState) {
    //     if (mTransientBarState == TRANSIENT_BAR_SHOWING || state == TRANSIENT_BAR_SHOWING) {
    //         mLastTranslucent = SystemClock.uptimeMillis();
    //     }
    //     mTransientBarState = state;
    //     if (DEBUG) Slog.d(mTag, "mTransientBarState: " + transientBarStateToString(state));
    // }
    assert(0);
}

AutoPtr<IIStatusBarService> BarController::GetStatusBarService()
{
    // ==================before translated======================
    // synchronized (mServiceAquireLock) {
    //     if (mStatusBarService == null) {
    //         mStatusBarService = IStatusBarService.Stub.asInterface(
    //                 ServiceManager.getService("statusbar"));
    //     }
    //     return mStatusBarService;
    // }
    assert(0);
    AutoPtr<IIStatusBarService> empty;
    return empty;
}

String BarController::TransientBarStateToString(
    /* [in] */ Int32 state)
{
    // ==================before translated======================
    // if (state == TRANSIENT_BAR_HIDING) return "TRANSIENT_BAR_HIDING";
    // if (state == TRANSIENT_BAR_SHOWING) return "TRANSIENT_BAR_SHOWING";
    // if (state == TRANSIENT_BAR_SHOW_REQUESTED) return "TRANSIENT_BAR_SHOW_REQUESTED";
    // if (state == TRANSIENT_BAR_NONE) return "TRANSIENT_BAR_NONE";
    // throw new IllegalArgumentException("Unknown state " + state);
    assert(0);
    return String("");
}

} // namespace Impl
} // namespace Policy
} // namespace Internal
} // namespace Droid
} // namespace Elastos


