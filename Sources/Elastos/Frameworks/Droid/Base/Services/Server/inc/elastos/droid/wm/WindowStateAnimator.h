#ifndef __ELASTOS_DROID_SERVER_WM_WINDOWSTATEANIMATOR_H__
#define __ELASTOS_DROID_SERVER_WM_WINDOWSTATEANIMATOR_H__

#include "ext/frameworkext.h"
#include "wm/CWindowManagerService.h"
#include "wm/AppWindowAnimator.h"
#include "wm/CSession.h"
#include "wm/WindowState.h"
#include "wm/WindowAnimator.h"

using Elastos::Droid::View::Animation::ITransformation;
using Elastos::Droid::View::Animation::IAnimation;
using Elastos::Droid::View::IWindowManagerPolicy;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Graphics::IRect;
using Elastos::Droid::Graphics::IRegion;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Wm {

class CWindowManagerService;
class WindowAnimator;
class AppWindowAnimator;

/**
 * Singleton class that carries out the animations and AutoPtr<ISurface> operations in a separate task
 * on behalf of WindowManagerService.
 */
class WindowStateAnimator
    : public ElRefBase
    , public IInterface
{
// public:
    // class SurfaceTrace
    //     : public ElRefBase
    //     , public ISurface
    //     , public Surface
    // {
    // public:
    //     CARAPI_(PInterface) Probe(
    //         /* [in] */ REIID riid);

    //     CARAPI_(UInt32) AddRef();

    //     CARAPI_(UInt32) Release();

    //     CARAPI GetInterfaceID(
    //         /* [in] */ IInterface *pObject,
    //         /* [out] */ InterfaceID *pIID);

    //     //add functions of ISurface
    // };

public:
    CAR_INTERFACE_DECL()

    WindowStateAnimator(
        /* [in] */ WindowState* win);

    static CARAPI_(String) DrawStateToString(
        /* [in] */ Int32 state);

    CARAPI_(void) SetAnimation(
        /* [in] */ IAnimation* anim);

    CARAPI_(void) ClearAnimation();

    /** Is the window or its container currently animating? */
    CARAPI_(Boolean) IsAnimating();

    /** Is the window animating the DummyAnimation? */
    CARAPI_(Boolean) IsDummyAnimation();

    /** Is this window currently animating? */
    CARAPI_(Boolean) IsWindowAnimating();

    CARAPI_(void) CancelExitAnimationForNextAnimationLocked();

    // This must be called while inside a transaction.  Returns true if
    // there is more animation to run.
    CARAPI_(Boolean) StepAnimationLocked(
        /* [in] */ Int64 currentTime);

    CARAPI_(void) FinishExit();

    CARAPI_(void) Hide();

    CARAPI_(Boolean) FinishDrawingLocked();

    // This must be called while inside a transaction.
    CARAPI_(Boolean) CommitFinishDrawingLocked(
        /* [in] */ Int64 currentTime);

    CARAPI_(AutoPtr<ISurface>) CreateSurfaceLocked();

    CARAPI_(void) DestroySurfaceLocked(
        /* [in] */ Boolean fromAnimator);

    CARAPI_(void) DestroyDeferredSurfaceLocked(
        /* [in] */ Boolean fromAnimator);

    CARAPI_(void) ComputeShownFrameLocked();

    CARAPI_(void) ApplyDecorRect(
        /* [in] */ IRect* decorRect);

    CARAPI_(void) UpdateSurfaceWindowCrop(
        /* [in] */ Boolean recoveringMemory);

    CARAPI_(void) SetSurfaceBoundariesLocked(
        /* [in] */ Boolean recoveringMemory);

    CARAPI_(void) PrepareSurfaceLocked(
        /* [in] */ Boolean recoveringMemory);

    CARAPI_(void) SetTransparentRegionHint(
        /* [in] */ IRegion* region);

    CARAPI_(void) SetWallpaperOffset(
        /* [in] */ Int32 left,
        /* [in] */ Int32 top);

    // This must be called while inside a transaction.
    CARAPI_(Boolean) PerformShowLocked();

    /**
     * Have the surface flinger show a surface, robustly dealing with
     * error conditions.  In particular, if there is not enough memory
     * to show the surface, then we will try to get rid of other surfaces
     * in order to succeed.
     *
     * @return Returns true if the surface was successfully shown.
     */
    CARAPI_(Boolean) ShowSurfaceRobustlyLocked();

    CARAPI_(void) ApplyEnterAnimationLocked();

    // TODO(cmautner): Move back to WindowState?
    /**
     * Choose the correct animation and set it to the passed WindowState.
     * @param transit If WindowManagerPolicy.TRANSIT_PREVIEW_DONE and the app window has been drawn
     *      then the animation will be app_starting_exit. Any other value loads the animation from
     *      the switch statement below.
     * @param isEntrance The animation type the last time this was called. Used to keep from
     *      loading the same animation twice.
     * @return true if an animation has been loaded.
     */
    CARAPI_(Boolean) ApplyAnimationLocked(
        /* [in] */ Int32 transit,
        /* [in] */ Boolean isEntrance);

    // void dump(PrintWriter pw, String prefix, boolean dumpAll);

private:
    CARAPI_(Boolean) StepAnimation(
        /* [in] */ Int64 currentTime);

public:
    static const Boolean DEBUG_VISIBILITY;
    static const Boolean DEBUG_ANIM;
    static const Boolean DEBUG_LAYERS;
    static const Boolean DEBUG_STARTING_WINDOW;
    static const Boolean SHOW_TRANSACTIONS;
    static const Boolean SHOW_LIGHT_TRANSACTIONS;
    static const Boolean SHOW_SURFACE_ALLOC;
    static const Boolean mLocalLOGV;
    static const Boolean DEBUG_ORIENTATION;
    static const Boolean DEBUG_SURFACE_TRACE;

    static const String TAG;

    /** This is set when there is no AutoPtr<ISurface> */
    static const Int32 NO_SURFACE;
    /** This is set after the AutoPtr<ISurface> has been created but before the window has been drawn. During
     * this time the surface is hidden. */
    static const Int32 DRAW_PENDING;
    /** This is set after the window has finished drawing for the first time but before its surface
     * is shown.  The surface will be displayed when the next layout is run. */
    static const Int32 COMMIT_DRAW_PENDING;
    /** This is set during the time after the window's drawing has been committed, and before its
     * surface is actually shown.  It is used to delay showing the surface until all windows in a
     * token are ready to be shown. */
    static const Int32 READY_TO_SHOW;
    /** Set when the window has been shown in the screen the first time. */
    static const Int32 HAS_DRAWN;

public:
    // Unchanging local convenience fields.
    AutoPtr<CWindowManagerService> mService;
    WindowState* mWin;
    AutoPtr<WindowStateAnimator> mAttachedWinAnimator;
    AutoPtr<WindowAnimator> mAnimator;
    AutoPtr<AppWindowAnimator> mAppAnimator;
    AutoPtr<CSession> mSession;
    AutoPtr<IWindowManagerPolicy> mPolicy;
    AutoPtr<IContext> mContext;
    Boolean mIsWallpaper;

    // If this is a universe background window, this is the transformation
    // it is applying to the rest of the universe.
    AutoPtr<ITransformation> mUniverseTransform;

    // Currently running animation.
    Boolean mAnimating;
    Boolean mLocalAnimating;
    AutoPtr<IAnimation> mAnimation;
    Boolean mAnimationIsEntrance;
    Boolean mHasTransformation;
    Boolean mHasLocalTransformation;
    AutoPtr<ITransformation> mTransformation;
    Boolean mWasAnimating;      // Were we animating going into the most recent animation step?
    Int32 mAnimLayer;
    Int32 mLastLayer;

    AutoPtr<ISurface> mSurface;
    AutoPtr<ISurface> mPendingDestroySurface;

    /**
     * Set when we have changed the size of the surface, to know that
     * we must tell them application to resize (and thus redraw itself).
     */
    Boolean mSurfaceResized;

    /**
     * Set if the client has asked that the destroy of its surface be delayed
     * until it explicitly says it is okay.
     */
    Boolean mSurfaceDestroyDeferred;

    Float mShownAlpha;
    Float mAlpha;
    Float mLastAlpha;

    // Used to save animation distances between the time they are calculated and when they are
    // used.
    Int32 mAnimDw;
    Int32 mAnimDh;
    Float mDsDx;
    Float mDtDx;
    Float mDsDy;
    Float mDtDy;
    Float mLastDsDx;
    Float mLastDtDx;
    Float mLastDsDy;
    Float mLastDtDy;

    Boolean mHaveMatrix;

    // For debugging, this is the last information given to the surface flinger.
    Boolean mSurfaceShown;
    Float mSurfaceX;
    Float mSurfaceY;
    Float mSurfaceW;
    Float mSurfaceH;
    Int32 mSurfaceLayer;
    Float mSurfaceAlpha;

    // Set to true if, when the window gets displayed, it should perform
    // an enter animation.
    Boolean mEnterAnimationPending;

    Int32 mDrawState;

    /** Was this window last hidden? */
    Boolean mLastHidden;

    Int32 mAttrFlags;
    Int32 mAttrType;

    Int32 mLayerStack;
};

} // Wm
} // Server
} // Droid
} // Elastos

#endif //__ELASTOS_DROID_SERVER_WM_WINDOWSTATEANIMATOR_H__
