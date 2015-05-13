#ifndef __WINDOWANIMATOR_H__
#define __WINDOWANIMATOR_H__

#include "wm/CWindowManagerService.h"
#include "wm/WindowState.h"
#include "wm/AppWindowAnimator.h"
#include "wm/WindowStateAnimator.h"
#include "wm/ScreenRotationAnimation.h"
#include "wm/DimAnimator.h"
#include "wm/DimSurface.h"
#include <elastos/List.h>
#include <elastos/HashMap.h>
#include <os/Runnable.h>

using Elastos::Utility::List;
using Elastos::Utility::HashMap;
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
class WindowAnimator : public ElRefBase
{
public:
    /** Parameters being passed from this into mService. */
    class AnimatorToLayoutParams : public ElRefBase
    {
    public:
        AnimatorToLayoutParams();

    public:
        Boolean mUpdateQueued;
        Int32 mBulkUpdateParams;
        AutoPtr<HashMap<Int32, Int32> > mPendingLayoutChanges;
        AutoPtr<WindowState> mWindowDetachedWallpaper;
    };

private:
    class AnimationRunnable
        : public Runnable
    {
    public:
        AnimationRunnable(
            /* [in] */ WindowAnimator* host);

        CARAPI Run();

    private:
        WindowAnimator* mHost;
    };

    class DisplayContentsAnimator : public ElRefBase
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

    /** Locked on mAnimToLayout */
    CARAPI_(void) UpdateAnimToLayoutLocked();

    CARAPI_(void) HideWallpapersLocked(
        /* [in] */ WindowState* w,
        /* [in] */ Boolean fromAnimator);

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

    /** Copy all WindowManagerService params into local params here. Locked on 'this'. */
    CARAPI_(void) CopyLayoutToAnimParamsLocked();

    CARAPI_(void) UpdateAppWindowsLocked();

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
    static const Int32 WALLPAPER_ACTION_PENDING;

    // forceHiding states.
    static const Int32 KEYGUARD_NOT_SHOWN;
    static const Int32 KEYGUARD_ANIMATING_IN;
    static const Int32 KEYGUARD_SHOWN;
    static const Int32 KEYGUARD_ANIMATING_OUT;

    Mutex mSelfLock;

    AutoPtr<CWindowManagerService> mService;
    AutoPtr<IContext> mContext;
    AutoPtr<IWindowManagerPolicy> mPolicy;

    Boolean mAnimating;

    AutoPtr<AnimationRunnable> mAnimationRunnable;

    Int32 mAdjResult;

    // Layout changes for individual Displays. Indexed by displayId.
    HashMap<Int32, Int32> mPendingLayoutChanges;

    // TODO: Assign these from each iteration through DisplayContent. Only valid between loops.
    /** Overall window dimensions */
    Int32 mDw;
    Int32 mDh;

    /** Interior window dimensions */
    Int32 mInnerDw;
    Int32 mInnerDh;

    /** Time of current animation step. Reset on each iteration */
    Int64 mCurrentTime;

    // Window currently running an animation that has requested it be detached
    // from the wallpaper.  This means we need to ensure the wallpaper is
    // visible behind it in case it animates in a way that would allow it to be
    // seen. If multiple windows satisfy this, use the lowest window.
    AutoPtr<WindowState> mWindowDetachedWallpaper;

    AutoPtr<WindowStateAnimator> mUniverseBackground;
    Int32 mAboveUniverseLayer;

    Int32 mBulkUpdateParams;

    HashMap<Int32, AutoPtr<DisplayContentsAnimator> > mDisplayContentsAnimators;

    Int32 mPendingActions;

    AutoPtr<WindowState> mWallpaperTarget;
    AutoPtr<AppWindowAnimator> mWpAppAnimator;
    AutoPtr<WindowState> mLowerWallpaperTarget;
    AutoPtr<WindowState> mUpperWallpaperTarget;

    List< AutoPtr<AppWindowAnimator> > mAppAnimators;

    List< AutoPtr<WindowToken> > mWallpaperTokens;

    /** Do not modify unless holding mService.mWindowMap or this and mAnimToLayout in that order */
    AutoPtr<AnimatorToLayoutParams> mAnimToLayout;
    Mutex mAnimToLayoutLock;

    Boolean mInitialized;

    Int32 mForceHiding;

    AutoPtr<WindowState> mCurrentFocus;

private:
    friend class AnimationRunnable;

    static const String TAG;

    /** Skip repeated AppWindowTokens initialization. Note that AppWindowsToken's version of this
     * is a long initialized to Long.MIN_VALUE so that it doesn't match this value on startup. */
    Int32 mAnimTransactionSequence;

    typedef HashMap<Int32, Int32>::Iterator IntIterator;
};

} // Wm
} // Server
} // Droid
} // Elastos

#endif //__WINDOWANIMATOR_H__
