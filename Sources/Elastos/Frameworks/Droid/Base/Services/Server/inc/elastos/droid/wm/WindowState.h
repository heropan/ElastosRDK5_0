#ifndef __ELASTOS_DROID_SERVER_WM_WINDOWSTATE_H__
#define __ELASTOS_DROID_SERVER_WM_WINDOWSTATE_H__

#include "wm/CWindowManagerService.h"
#include "wm/CSession.h"
#include "wm/DisplayContent.h"
#include "wm/MagnificationSpec.h"
#include "wm/WindowStateAnimator.h"
#include <elastos/utility/etl/List.h>
#include "input/InputWindowHandle.h"

using Elastos::Utility::Etl::List;
using Elastos::Core::ICharSequence;
using Elastos::Droid::View::IWindowState;
using Elastos::Droid::View::IWindowManagerLayoutParams;
using Elastos::Droid::View::IApplicationToken;
using Elastos::Droid::View::IInputChannel;
using Elastos::Droid::View::IWindowManagerPolicy;
using Elastos::Droid::View::IIWindow;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::Res::IConfiguration;
using Elastos::Droid::Graphics::IRect;
using Elastos::Droid::Graphics::IRegion;
using Elastos::Droid::Graphics::IRectF;
using Elastos::Droid::Graphics::IMatrix;
using Elastos::Droid::Server::Input::InputWindowHandle;


namespace Elastos {
namespace Droid {
namespace Server {
namespace Wm {

class AppWindowToken;
class WindowToken;
class WindowStateAnimator;

class WindowState
    : public ElRefBase
    , public IWindowState
{
private:
    class DeathRecipient
        : public ElRefBase
        , public IProxyDeathRecipient
    {
    public:
        CAR_INTERFACE_DECL();

        DeathRecipient(
            /* [in] */ WindowState* owner);

        //@Override
        CARAPI ProxyDied();

    private:
        WindowState* mOwner;
    };

public:
    CAR_INTERFACE_DECL();

    WindowState(
        /* [in] */ CWindowManagerService* service,
        /* [in] */ CSession* s,
        /* [in] */ IIWindow* c,
        /* [in] */ WindowToken* token,
        /* [in] */ WindowState* attachedWindow,
        /* [in] */ Int32 seq,
        /* [in] */ IWindowManagerLayoutParams* attrs,
        /* [in] */ Int32 viewVisibility,
        /* [in] */ DisplayContent* displayContent);

    ~WindowState();

    CARAPI_(void) Attach();

    CARAPI ComputeFrameLw(
        /* [in] */ IRect* pf,
        /* [in] */ IRect* df,
        /* [in] */ IRect* cf,
        /* [in] */ IRect* vf);

    CARAPI_(AutoPtr<MagnificationSpec>) GetWindowMagnificationSpecLocked();

    /**
     * Retrieve the current frame of the window that has been assigned by
     * the window manager.  Must be called with the window manager lock held.
     *
     * @return Rect The rectangle holding the window frame.
     */
    CARAPI GetFrameLw(
        /* [out] */ IRect** frame);

    /**
     * Retrieve the current frame of the window that is actually shown.
     * Must be called with the window manager lock held.
     *
     * @return Rect The rectangle holding the shown window frame.
     */
    CARAPI GetShownFrameLw(
        /* [out] */ IRectF** shownFrame);

    /**
     * Retrieve the frame of the display that this window was last
     * laid out in.  Must be called with the
     * window manager lock held.
     *
     * @return Rect The rectangle holding the display frame.
     */
    CARAPI GetDisplayFrameLw(
        /* [out] */ IRect** displayFrame);

    /**
     * Retrieve the frame of the content area that this window was last
     * laid out in.  This is the area in which the content of the window
     * should be placed.  It will be smaller than the display frame to
     * account for screen decorations such as a status bar or soft
     * keyboard.  Must be called with the
     * window manager lock held.
     *
     * @return Rect The rectangle holding the content frame.
     */
    CARAPI GetContentFrameLw(
        /* [out] */ IRect** contentFrame);

    /**
     * Retrieve the frame of the visible area that this window was last
     * laid out in.  This is the area of the screen in which the window
     * will actually be fully visible.  It will be smaller than the
     * content frame to account for transient UI elements blocking it
     * such as an input method's candidates UI.  Must be called with the
     * window manager lock held.
     *
     * @return Rect The rectangle holding the visible frame.
     */
    CARAPI GetVisibleFrameLw(
        /* [out] */ IRect** visibleFrame);

    /**
     * Returns true if this window is waiting to receive its given
     * internal insets from the client app, and so should not impact the
     * layout of other windows.
     */
    CARAPI GetGivenInsetsPendingLw(
        /* [out] */ Boolean* result);

    /**
     * Retrieve the insets given by this window's client for the content
     * area of windows behind it.  Must be called with the
     * window manager lock held.
     *
     * @return Rect The left, top, right, and bottom insets, relative
     * to the window's frame, of the actual contents.
     */
    CARAPI GetGivenContentInsetsLw(
        /* [out] */ IRect** insetsRect);

    /**
     * Retrieve the insets given by this window's client for the visible
     * area of windows behind it.  Must be called with the
     * window manager lock held.
     *
     * @return Rect The left, top, right, and bottom insets, relative
     * to the window's frame, of the actual visible area.
     */
    CARAPI GetGivenVisibleInsetsLw(
        /* [out] */ IRect** visibleArea);

    /**
     * Retrieve the current LayoutParams of the window.
     *
     * @return WindowManager.LayoutParams The window's internal LayoutParams
     *         instance.
     */
    CARAPI GetAttrs(
        /* [out] */ IWindowManagerLayoutParams** attrs);

    /**
     * Return whether this window needs the menu key shown.  Must be called
     * with window lock held, because it may need to traverse down through
     * window list to determine the result.
     * @param bottom The bottom-most window to consider when determining this.
     */
    CARAPI GetNeedsMenuLw(
        /* [in] */ IWindowState* bottom,
        /* [out] */ Boolean* result);

    /**
     * Retrieve the current system UI visibility flags associated with
     * this window.
     */
    CARAPI GetSystemUiVisibility(
        /* [out] */ Int32* flag);

    /**
     * Get the layer at which this window's surface will be Z-ordered.
     */
    CARAPI GetSurfaceLayer(
        /* [out] */ Int32* surfaceLayer);

    /**
     * Return the token for the application (actually activity) that owns
     * this window.  May return null for system windows.
     *
     * @return An IApplicationToken identifying the owning activity.
     */
    CARAPI GetAppToken(
        /* [out] */ IApplicationToken** token);

    CARAPI_(Int32) GetDisplayId();

    CARAPI_(Int64) GetInputDispatchingTimeoutNanos();

    /**
     * Return true if, at any point, the application token associated with
     * this window has actually displayed any windows.  This is most useful
     * with the "starting up" window to determine if any windows were
     * displayed when it is closed.
     *
     * @return Returns true if one or more windows have been displayed,
     *         else false.
     */
    CARAPI HasAppShownWindows(
        /* [out] */ Boolean* result);

    CARAPI_(Boolean) IsIdentityMatrix(
        /* [in] */ Float dsdx,
        /* [in] */ Float dtdx,
        /* [in] */ Float dsdy,
        /* [in] */ Float dtdy);

    CARAPI_(void) Prelayout();

    /**
     * Is this window visible?  It is not visible if there is no
     * surface, or we are in the process of running an exit animation
     * that will remove the surface.
     */
    CARAPI IsVisibleLw(
        /* [out] */ Boolean* isVisible);

    /**
     * Like {@link #isVisibleLw}, but also counts a window that is currently
     * "hidden" behind the keyguard as visible.  This allows us to apply
     * things like window flags that impact the keyguard.
     */
    CARAPI IsVisibleOrBehindKeyguardLw(
        /* [out] */ Boolean* result);

    /**
     * Is this window visible, ignoring its app token?  It is not visible
     * if there is no surface, or we are in the process of running an exit animation
     * that will remove the surface.
     */
    CARAPI_(Boolean) IsWinVisibleLw();

    /**
     * The same as isVisible(), but follows the current hidden state of
     * the associated app token, not the pending requested hidden state.
     */
    CARAPI_(Boolean) IsVisibleNow();

    /**
     * Can this window possibly be a drag/drop target?  The test here is
     * a combination of the above "visible now" with the check that the
     * Input Manager uses when discarding windows from input consideration.
     */
    CARAPI_(Boolean) IsPotentialDragTarget();

    /**
     * Same as isVisible(), but we also count it as visible between the
     * call to IWindowSession.add() and the first relayout().
     */
    CARAPI_(Boolean) IsVisibleOrAdding();

    /**
     * Is this window currently on-screen?  It is on-screen either if it
     * is visible or it is currently running an animation before no longer
     * being visible.
     */
    CARAPI_(Boolean) IsOnScreen();

    /**
     * Like isOnScreen(), but we don't return true if the window is part
     * of a transition that has not yet been started.
     */
    CARAPI_(Boolean) IsReadyForDisplay();

    /**
     * Like isReadyForDisplay(), but ignores any force hiding of the window due
     * to the keyguard.
     */
    CARAPI_(Boolean) IsReadyForDisplayIgnoringKeyguard();

    /**
     * Is this window currently visible to the user on-screen?  It is
     * displayed either if it is visible or it is currently running an
     * animation before no longer being visible.  Must be called with the
     * window manager lock held.
     */
    CARAPI IsDisplayedLw(
        /* [out] */ Boolean* isDisplayed);

    /**
     * Return true if this window (or a window it is attached to, but not
     * considering its app token) is currently animating.
     */
    CARAPI IsAnimatingLw(
        /* [out] */ Boolean* isAnimating);

    /**
     * Is this window considered to be gone for purposes of layout?
     */
    CARAPI IsGoneForLayoutLw(
        /* [out] */ Boolean* isGone);

    /**
     * Returns true if the window has a surface that it has drawn a
     * complete UI in to.
     */
    CARAPI_(Boolean) IsDrawnLw();

    /**
     * Return true if the window is opaque and fully drawn.  This indicates
     * it may obscure windows behind it.
     */
    CARAPI_(Boolean) IsOpaqueDrawn();

    /**
     * Return whether this window is wanting to have a translation
     * animation applied to it for an in-progress move.  (Only makes
     * sense to call from performLayoutAndPlaceSurfacesLockedInner().)
     */
    CARAPI_(Boolean) ShouldAnimateMove();

    CARAPI_(Boolean) IsFullscreen(
        /* [in] */ Int32 screenWidth,
        /* [in] */ Int32 screenHeight);

    CARAPI_(Boolean) IsConfigChanged();

    CARAPI_(Boolean) IsConfigDiff(
        /* [in] */ Int32 mask);

    CARAPI_(void) RemoveLocked();

    CARAPI_(void) SetInputChannel(
        /* [in] */ IInputChannel* inputChannel);

    CARAPI_(void) DisposeInputChannel();

    /** Returns true if this window desires key events.
     * TODO(cmautner): Is this the same as {@link WindowManagerService#canBeImeTarget}
     */
    CARAPI_(Boolean) CanReceiveKeys();

    /**
     * Returns true if this window has been shown on screen at some time in
     * the past.  Must be called with the window manager lock held.
     */
    CARAPI HasDrawnLw(
        /* [out] */ Boolean* drawn);

    /**
     * Can be called to undo the effect of {@link #hideLw}, allowing a
     * window to be shown as long as the window manager and client would
     * also like it to be shown.  Must be called with the window manager
     * lock held.
     * Returns true if {@link #hideLw} was last called for the window.
     */
    CARAPI ShowLw(
        /* [in] */ Boolean doAnimation,
        /* [out] */ Boolean* canShown);

    CARAPI_(Boolean) ShowLw(
        /* [in] */ Boolean doAnimation,
        /* [in] */ Boolean requestAnim);

    /**
     * Can be called by the policy to force a window to be hidden,
     * regardless of whether the client or window manager would like
     * it shown.  Must be called with the window manager lock held.
     * Returns true if {@link #showLw} was last called for the window.
     */
    CARAPI HideLw(
        /* [in] */ Boolean doAnimation,
        /* [out] */ Boolean* canHidden);

    CARAPI_(Boolean) HideLw(
        /* [in] */ Boolean doAnimation,
        /* [in] */ Boolean requestAnim);

    /**
     * Check whether the process hosting this window is currently alive.
     */
    CARAPI IsAlive(
        /* [out] */ Boolean* isAlive);

    CARAPI_(Boolean) IsClosing();

    /**
     * Check if window is on {@link Display#DEFAULT_DISPLAY}.
     * @return true if window is on default display.
     */
    CARAPI IsDefaultDisplay(
        /* [out] */ Boolean* result);

    CARAPI_(void) SetShowToOwnerOnlyLocked(
        /* [in] */ Boolean showToOwnerOnly);

    CARAPI_(Boolean) IsHiddenFromUserLocked();

    CARAPI_(void) GetTouchableRegion(
        /* [in] */ IRegion* outRegion);

    CARAPI_(List<AutoPtr<WindowState> >&) GetWindowList();

    // void dump(PrintWriter pw, String prefix, boolean dumpAll);

    CARAPI_(String) MakeInputChannelName();

    CARAPI SetConfiguration(
        /* [in] */ IConfiguration* config);

    CARAPI_(String) ToString();

    CARAPI ToString(
        /* [out]*/ String* str);

private:
    static CARAPI_(void) ApplyInsets(
        /* [in] */ IRegion* outRegion,
        /* [in] */ IRect* frame,
        /* [in] */ IRect* inset);

public:
    static const String TAG;

    static const Boolean DEBUG_VISIBILITY;
    static const Boolean SHOW_TRANSACTIONS;
    static const Boolean SHOW_LIGHT_TRANSACTIONS;
    static const Boolean SHOW_SURFACE_ALLOC;

public:
    AutoPtr<CWindowManagerService> mService;
    AutoPtr<IWindowManagerPolicy> mPolicy;
    AutoPtr<IContext> mContext;
    AutoPtr<CSession> mSession;
    AutoPtr<IIWindow> mClient;
    AutoPtr<WindowToken> mToken;
    AutoPtr<WindowToken> mRootToken;
    AppWindowToken* mAppToken;
    AutoPtr<AppWindowToken> mTargetAppToken;

    // mAttrs.flags is tested in animation without being locked. If the bits tested are ever
    // modified they will need to be locked.
    AutoPtr<IWindowManagerLayoutParams> mAttrs;
    AutoPtr<DeathRecipient> mDeathRecipient;
    AutoPtr<WindowState> mAttachedWindow;
    List<AutoPtr<WindowState> > mChildWindows;
    Int32 mBaseLayer;
    Int32 mSubLayer;
    Boolean mLayoutAttached;
    Boolean mIsImWindow;
    Boolean mIsWallpaper;
    Boolean mIsFloatingLayer;
    Int32 mSeq;
    Boolean mEnforceSizeCompat;
    Int32 mViewVisibility;
    Int32 mSystemUiVisibility;
    Boolean mPolicyVisibility;
    Boolean mPolicyVisibilityAfterAnim;
    Boolean mAppFreezing;
    Boolean mAttachedHidden;    // is our parent window hidden?
    Boolean mWallpaperVisible;  // for wallpaper, what was last vis report?

    /**
     * The window size that was requested by the application.  These are in
     * the application's coordinate space (without compatibility scale applied).
     */
    Int32 mRequestedWidth;
    Int32 mRequestedHeight;
    Int32 mLastRequestedWidth;
    Int32 mLastRequestedHeight;

    Int32 mLayer;
    Boolean mHaveFrame;
    Boolean mObscured;
    Boolean mTurnOnScreen;

    Int32 mLayoutSeq;

    AutoPtr<IConfiguration> mConfiguration;
    // Sticky answer to isConfigChanged(), remains true until new Configuration is assigned.
    // Used only on {@link #TYPE_KEYGUARD}.
    Boolean mConfigHasChanged;

    /**
     * Actual frame shown on-screen (may be modified by animation).  These
     * are in the screen's coordinate space (WITH the compatibility scale
     * applied).
     */
    AutoPtr<IRectF> mShownFrame;

    /**
     * Insets that determine the actually visible area.  These are in the application's
     * coordinate space (without compatibility scale applied).
     */
    AutoPtr<IRect> mVisibleInsets;
    AutoPtr<IRect> mLastVisibleInsets;
    Boolean mVisibleInsetsChanged;

    /**
     * Insets that are covered by system windows (such as the status bar) and
     * transient docking windows (such as the IME).  These are in the application's
     * coordinate space (without compatibility scale applied).
     */
    AutoPtr<IRect> mContentInsets;
    AutoPtr<IRect> mLastContentInsets;
    Boolean mContentInsetsChanged;

    /**
     * Set to true if we are waiting for this window to receive its
     * given internal insets before laying out other windows based on it.
     */
    Boolean mGivenInsetsPending;

    /**
     * These are the content insets that were given during layout for
     * this window, to be applied to windows behind it.
     */
    AutoPtr<IRect> mGivenContentInsets;

    /**
     * These are the visible insets that were given during layout for
     * this window, to be applied to windows behind it.
     */
    AutoPtr<IRect> mGivenVisibleInsets;

    /**
     * This is the given touchable area relative to the window frame, or null if none.
     */
    AutoPtr<IRegion> mGivenTouchableRegion;

    /**
     * Flag indicating whether the touchable region should be adjusted by
     * the visible insets; if false the area outside the visible insets is
     * NOT touchable, so we must use those to adjust the frame during hit
     * tests.
     */
    Int32 mTouchableInsets;

    /**
     * This is rectangle of the window's surface that is not covered by
     * system decorations.
     */
    AutoPtr<IRect> mSystemDecorRect;
    AutoPtr<IRect> mLastSystemDecorRect;

    // Current transformation being applied.
    Float mGlobalScale;
    Float mInvGlobalScale;
    Float mHScale;
    Float mVScale;
    Float mLastHScale;
    Float mLastVScale;
    AutoPtr<IMatrix> mTmpMatrix;

    // "Real" frame that the application sees, in display coordinate space.
    AutoPtr<IRect> mFrame;
    AutoPtr<IRect> mLastFrame;
    // Frame that is scaled to the application's coordinate space when in
    // screen size compatibility mode.
    AutoPtr<IRect> mCompatFrame;

    AutoPtr<IRect> mContainingFrame;
    AutoPtr<IRect> mDisplayFrame;
    AutoPtr<IRect> mContentFrame;
    AutoPtr<IRect> mParentFrame;
    AutoPtr<IRect> mVisibleFrame;

    Boolean mContentChanged;

    // If a window showing a wallpaper: the requested offset for the
    // wallpaper; if a wallpaper window: the currently applied offset.
    Float mWallpaperX;
    Float mWallpaperY;

    // If a window showing a wallpaper: what fraction of the offset
    // range corresponds to a full virtual screen.
    Float mWallpaperXStep;
    Float mWallpaperYStep;

    // Wallpaper windows: pixels offset based on above variables.
    Int32 mXOffset;
    Int32 mYOffset;

    // This is set after IWindowSession.relayout() has been called at
    // least once for the window.  It allows us to detect the situation
    // where we don't yet have a surface, but should have one soon, so
    // we can give the window focus before waiting for the relayout.
    Boolean mRelayoutCalled;

    // If the application has called relayout() with changes that can
    // impact its window's size, we need to perform a layout pass on it
    // even if it is not currently visible for layout.  This is set
    // when in that case until the layout is done.
    Boolean mLayoutNeeded;

    // Currently running an exit animation?
    Boolean mExiting;

    // Currently on the mDestroySurface list?
    Boolean mDestroying;

    // Completely remove from window manager after exit animation?
    Boolean mRemoveOnExit;

    // Set when the orientation is changing and this window has not yet
    // been updated for the new orientation.
    Boolean mOrientationChanging;

    // Is this window now (or just being) removed?
    Boolean mRemoved;

    // Temp for keeping track of windows that have been removed when
    // rebuilding window list.
    Boolean mRebuilding;

    // InpAutoPtr<ut channel and input window handle used by the input dispatcher.
    AutoPtr<InputWindowHandle> mInputWindowHandle;
    AutoPtr<IInputChannel> mInputChannel;

    // Used to improve performance of toString()
    String mStringNameCache;
    AutoPtr<ICharSequence> mLastTitle;
    Boolean mWasExiting;

    AutoPtr<WindowStateAnimator> mWinAnimator;

    Boolean mHasSurface;

    AutoPtr<DisplayContent>  mDisplayContent;

    // UserId and appId of the owner. Don't display windows of non-current user.
    Int32 mOwnerUid;

private:
    /** When true this window can be displayed on screens owther than mOwnerUid's */
    Boolean mShowToOwnerOnly;

};

} // Wm
} // Server
} // Droid
} // Elastos

#endif //__ELASTOS_DROID_SERVER_WM_WINDOWSTATE_H__
