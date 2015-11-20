#ifndef __ELASTOS_DROID_SERVER_WM_WINDOWANIMATOR_H__
#define __ELASTOS_DROID_SERVER_WM_WINDOWANIMATOR_H__

#include "wm/CWindowManagerService.h"
#include "wm/WindowState.h"
#include "wm/WindowStateAnimator.h"
#include "wm/DimAnimator.h"
#include "wm/DimSurface.h"
#include <elastos/utility/etl/List.h>
#include <elastos/utility/etl/HashMap.h>
#include <os/Runnable.h>

using Elastos::Utility::Etl::List;
using Elastos::Utility::Etl::HashMap;
using Elastos::Droid::Os::Runnable;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::View::IWindowManagerPolicy;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Wm {

class AppWindowAnimator;
class WindowToken;
class ScreenRotationAnimation;

/**
 * Singleton class that carries out the animations and Surface operations in a separate task
 * on behalf of WindowManagerService.
 */
class WindowAnimator : public Object
{
private:
    class AnimationRunnable : public Runnable
    {
    public:
        AnimationRunnable(
            /* [in] */ WindowAnimator* host)
            : mHost(host)

        {}

        CARAPI Run();

    private:
        WindowAnimator* mHost;
    };

    class DisplayContentsAnimator : public Object
    {
    public:
        DisplayContentsAnimator(
            /* [in] */ WindowAnimator* host,
            /* [in] */ Int32 displayId);

    public:
        List< AutoPtr<WindowStateAnimator> > mWinAnimators;
        AutoPtr<DimAnimator> mDimAnimator;
        AutoPtr<DimAnimator::Parameters> mDimParams;
        AutoPtr<DimSurface> mWindowAnimationBackgroundSurface;
        AutoPtr<ScreenRotationAnimation> mScreenRotationAnimation;

    private:
        WindowAnimator* mHost;
    };

public:
    WindowAnimator(
        /* [in] */ CWindowManagerService* service);

    ~WindowAnimator();

    CARAPI_(void) AddDisplayLocked(
        /* [in] */ Int32 displayId);

    CARAPI_(void) RemoveDisplayLocked(
        /* [in] */ Int32 displayId);

    CARAPI_(void) HideWallpapersLocked(
        /* [in] */ WindowState* w,
        /* [in] */ WindowState* wallpaperTarget,
        /* [in] */ WindowState* lowerWallpaperTarget,
        /* [in] */ List< AutoPtr<WindowToken> >& wallpaperTokens);

    CARAPI_(void) SetCurrentFocus(
        /* [in] */ WindowState* currentFocus);

    CARAPI_(void) SetDisplayDimensions(
        /* [in] */ Int32 curWidth,
        /* [in] */ Int32 curHeight,
        /* [in] */ Int32 appWidth,
        /* [in] */ Int32 appHeight);

    CARAPI_(Boolean) IsDimmingLocked(
        /* [in] */ Int32 displayId);

    CARAPI_(Boolean) IsDimmingLocked(
        /* [in] */ WindowStateAnimator* winAnimator);

    static CARAPI_(String) BulkUpdateParamsToString(
        /* [in] */ Int32 bulkUpdateParams);

    // void dumpLocked(PrintWriter pw, String prefix, boolean dumpAll);

    CARAPI_(void) ClearPendingActions();

    CARAPI_(void) SetPendingLayoutChanges(
        /* [in] */ Int32 displayId,
        /* [in] */ Int32 changes);

    CARAPI_(void) SetAppLayoutChanges(
        /* [in] */ AppWindowAnimator* appAnimator,
        /* [in] */ Int32 changes,
        /* [in] */ const String& s);

    CARAPI_(void) SetScreenRotationAnimationLocked(
        /* [in] */ Int32 displayId,
        /* [in] */ ScreenRotationAnimation* animation);

    CARAPI_(AutoPtr<ScreenRotationAnimation>) GetScreenRotationAnimationLocked(
        /* [in] */ Int32 displayId);

private:
    CARAPI_(String) ForceHidingToString();

    CARAPI_(void) UpdateAppWindowsLocked(
        /* [in] */ Int32 displayId);

    CARAPI_(void) UpdateWindowsLocked(
        /* [in] */ Int32 displayId);

    CARAPI_(void) UpdateWallpaperLocked(
        /* [in] */ Int32 displayId);

    CARAPI_(void) TestTokenMayBeDrawnLocked();

    CARAPI_(void) PerformAnimationsLocked(
        /* [in] */ Int32 displayId);

    // TODO(cmautner): Change the following comment when no longer locked on mWindowMap */
    /** Locked on mService.mWindowMap and this. */
    CARAPI_(void) AnimateLocked();

    CARAPI_(AutoPtr<DisplayContentsAnimator>) GetDisplayContentsAnimatorLocked(
        /* [in] */ Int32 displayId);

public:
    // forceHiding states.
    static const Int32 KEYGUARD_NOT_SHOWN = 0;
    static const Int32 KEYGUARD_ANIMATING_IN = 1;
    static const Int32 KEYGUARD_SHOWN = 2;
    static const Int32 KEYGUARD_ANIMATING_OUT = 3;

    Object mSelfLock;

    AutoPtr<CWindowManagerService> mService;
    AutoPtr<IContext> mContext;
    AutoPtr<IWindowManagerPolicy> mPolicy;

    Boolean mAnimating;

    AutoPtr<AnimationRunnable> mAnimationRunnable;

    /** Time of current animation step. Reset on each iteration */
    Int64 mCurrentTime;

    /** Window currently running an animation that has requested it be detached
     * from the wallpaper.  This means we need to ensure the wallpaper is
     * visible behind it in case it animates in a way that would allow it to be
     * seen. If multiple windows satisfy this, use the lowest window. */
    AutoPtr<WindowState> mWindowDetachedWallpaper;

    AutoPtr<WindowStateAnimator> mUniverseBackground;
    Int32 mAboveUniverseLayer;

    Int32 mBulkUpdateParams;
    AutoPtr<IObject> mLastWindowFreezeSource;

    HashMap<Int32, AutoPtr<DisplayContentsAnimator> > mDisplayContentsAnimators;

    Boolean mInitialized;

    Boolean mKeyguardGoingAway;
    Boolean mKeyguardGoingAwayToNotificationShade;
    Boolean mKeyguardGoingAwayDisableWindowAnimations;

    Int32 mForceHiding;

    AutoPtr<WindowState> mCurrentFocus;

private:
    static const String TAG;

    /** How long to give statusbar to clear the private keyguard flag when animating out */
    static const Int64 KEYGUARD_ANIM_TIMEOUT_MS = 1000;

    /** Skip repeated AppWindowTokens initialization. Note that AppWindowsToken's version of this
     * is a long initialized to Long.MIN_VALUE so that it doesn't match this value on startup. */
    Int32 mAnimTransactionSequence;

    friend class AnimationRunnable;
};

} // Wm
} // Server
} // Droid
} // Elastos

#endif //__ELASTOS_DROID_SERVER_WM_WINDOWANIMATOR_H__
