
#ifndef __ELASTOS_DROID_SERVER_WM_CWINDOWMANAGERSERVICE_H__
#define __ELASTOS_DROID_SERVER_WM_CWINDOWMANAGERSERVICE_H__

#include "ext/frameworkext.h"
#include "_Elastos_Droid_Server_Wm_CWindowManagerService.h"
#include "input/CInputManagerService.h"
#include "display/CDisplayManagerService.h"
#include "power/CPowerManagerService.h"
#include "view/InputEventReceiver.h"
#include "wm/WindowState.h"
#include "wm/AppWindowToken.h"
#include "wm/FakeWindowImpl.h"
#include "wm/WindowToken.h"
#include "wm/Watermark.h"
#include "wm/StrictModeFlash.h"
#include "wm/WindowAnimator.h"
#include "wm/CSession.h"
#include "wm/AppWindowAnimator.h"
#include "wm/WindowStateAnimator.h"
#include "wm/DimAnimator.h"
#include "wm/DisplayContent.h"
#include "AttributeCache.h"
#include "wm/MagnificationSpec.h"
#include "wm/WindowBinder.h"
#include <elastos/utility/etl/List.h>
#include <elastos/utility/etl/HashSet.h>
#include <elastos/utility/etl/HashMap.h>

using Elastos::Utility::Etl::HashSet;
using Elastos::Utility::Etl::HashMap;
using Elastos::Utility::Etl::List;
using Elastos::Utility::Etl::Pair;
using Elastos::Core::ICharSequence;
using Elastos::Core::IRunnable;
using Elastos::Net::ISocket;
using Elastos::Droid::View::IWindowSession;
using Elastos::Droid::View::IInputChannel;
using Elastos::Droid::View::IIWindow;
using Elastos::Droid::View::IInputEvent;
using Elastos::Droid::View::IIInputFilter;
using Elastos::Droid::View::InputEventReceiver;
using Elastos::Droid::View::IChoreographer;
using Elastos::Droid::View::IRotationWatcher;
using Elastos::Droid::View::IWindowManagerPolicy;
using Elastos::Droid::View::IWindowInfo;
using Elastos::Droid::View::IDisplayContentChangeListener;
using Elastos::Droid::View::IOnKeyguardExitResult;
using Elastos::Droid::View::IWindowManagerLayoutParams;
using Elastos::Droid::View::IFakeWindow;
using Elastos::Droid::View::IInputEventReceiverFactory;
using Elastos::Droid::View::Animation::IAnimation;
using Elastos::Droid::View::IDisplayInfo;
using Elastos::Droid::View::IApplicationToken;
using Elastos::Droid::View::Animation::IInterpolator;
using Elastos::Droid::App::IIActivityManager;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IBroadcastReceiver;
using Elastos::Droid::Content::Res::IConfiguration;
using Elastos::Droid::Content::Res::ICompatibilityInfo;
using Elastos::Droid::Os::IPowerManagerWakeLock;
using Elastos::Droid::Os::IHandler;
using Elastos::Droid::Os::IRemoteCallback;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Os::ILooper;
using Elastos::Droid::Hardware::Display::IDisplayManager;
using Elastos::Droid::Graphics::IBitmap;
using Elastos::Droid::Graphics::IRect;
using Elastos::Droid::Graphics::IPoint;
using Elastos::Droid::Internal::App::IIBatteryStats;
using Elastos::Droid::Internal::View::IInputMethodClient;
using Elastos::Droid::Internal::View::IIInputMethodManager;
using Elastos::Droid::Internal::View::IInputContext;
using Elastos::Droid::Server::Display::CDisplayManagerService;
using Elastos::Droid::Server::Input::CInputManagerService;
using Elastos::Droid::Server::Power::CPowerManagerService;


namespace Elastos {
namespace Droid {
namespace Server {
namespace Wm {

class KeyguardDisableHandler;
class ViewServer;
class FakeWindowImpl;
class DisplayContent;
class DragState;
class InputMonitor;

CarClass(CWindowManagerService)
{
public:
    /** Pulled out of performLayoutAndPlaceSurfacesLockedInner in order to refactor into multiple
     * methods. */
    class LayoutFields : public ElRefBase
    {
    public:
        LayoutFields();

    public:
        static const Int32 SET_UPDATE_ROTATION;
        static const Int32 SET_WALLPAPER_MAY_CHANGE;
        static const Int32 SET_FORCE_HIDING_CHANGED;
        static const Int32 SET_ORIENTATION_CHANGE_COMPLETE;
        static const Int32 SET_TURN_ON_SCREEN;

        Boolean mWallpaperForceHidingChanged;
        Boolean mWallpaperMayChange;
        Boolean mOrientationChangeComplete;
        Int32 mAdjResult;

    private:
        static const Int32 DISPLAY_CONTENT_UNKNOWN;
        static const Int32 DISPLAY_CONTENT_MIRROR;
        static const Int32 DISPLAY_CONTENT_UNIQUE;

        AutoPtr<IWindowSession> mHoldScreen;
        Boolean mObscured;
        Boolean mDimming;
        Boolean mSyswin;
        Float mScreenBrightness;
        Float mButtonBrightness;
        Int64 mUserActivityTimeout;
        Boolean mUpdateRotation;
        Int32 mDisplayHasContent;

        friend class CWindowManagerService;
    };

    class AppWindowAnimParams : public ElRefBase
    {
    public:
        AppWindowAnimParams(
            /* [in] */ AppWindowAnimator* appAnimator);

        ~AppWindowAnimParams();

    public:
        AutoPtr<AppWindowAnimator> mAppAnimator;
        List< AutoPtr<WindowStateAnimator> > mWinAnimators;
    };

    class LayoutToAnimatorParams : public ElRefBase
    {
    public:
        LayoutToAnimatorParams();

        ~LayoutToAnimatorParams();

    public:
        static const Int64 WALLPAPER_TOKENS_CHANGED;

    public:
        Boolean mParamsModified;
        Int64 mChanges;
        Boolean mAnimationScheduled;
        HashMap<Int32, AutoPtr<List< AutoPtr<WindowStateAnimator> > > > mWinAnimatorLists;
        AutoPtr<WindowState> mWallpaperTarget;
        AutoPtr<WindowState> mLowerWallpaperTarget;
        AutoPtr<WindowState> mUpperWallpaperTarget;
        HashMap<Int32, AutoPtr<DimAnimator::Parameters> > mDimParams;
        List< AutoPtr<WindowToken> > mWallpaperTokens;
        List< AutoPtr<AppWindowAnimParams> > mAppWindowAnimParams;
    };

    class DragInputEventReceiver
        : public InputEventReceiver
    {
    public:
        DragInputEventReceiver(
            /* [in] */ IInputChannel* inputChannel,
            /* [in] */ ILooper* looper,
            /* [in] */ CWindowManagerService* host);

        CARAPI OnInputEvent(
            /* [in] */ IInputEvent* event);

    private:
        CWindowManagerService* mHost;
    };

    interface IWindowChangeListener : public IInterface
    {
        virtual CARAPI_(void) WindowsChanged() = 0;

        virtual CARAPI_(void) FocusChanged() = 0;
    };

    interface IOnHardKeyboardStatusChangeListener : public IInterface
    {
        virtual CARAPI_(void) OnHardKeyboardStatusChange(
            /* [in] */ Boolean available,
            /* [in] */ Boolean enabled) = 0;
    };

    // class AllWindowsIterator : public ElRefBase
    // {
    // public:
    //     AllWindowsIterator();

    //     AllWindowsIterator(
    //         /* [in] */ Boolean reverse);

    // private:
    //     AutoPtr<DisplayContent> mDisplayContent;
    //     AutoPtr<DisplayContentsIterator> mDisplayContentsIterator;
    //     List< AutoPtr<WindowState> > mWindowList;
    //     List< AutoPtr<WindowState> >::Iterator mWindowListIt;
    //     List< AutoPtr<WindowState> >::ReverseIterator mWindowListRIt;
    //     Boolean mReverse;
    // };

    class LocalBroadcastReceiver
        : public BroadcastReceiver
    {
    public:
        LocalBroadcastReceiver(
            /* [in] */ CWindowManagerService* host)
            : mHost(host)
        {}

        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

        CARAPI ToString(
            /* [out] */ String* info)
        {
            VALIDATE_NOT_NULL(info);
            *info = String("CWindowManagerService::LocalBroadcastReceiver: ");
            (*info).AppendFormat("%p", this);
            return NOERROR;
        }
    private:
        CWindowManagerService* mHost;
    };

    class SecurelyOnKeyguardExitResult
        : public ElRefBase
        , public IOnKeyguardExitResult
    {
    public:
        SecurelyOnKeyguardExitResult(
            /* [in] */ IOnKeyguardExitResult* callback)
            : mCallback(callback)
        {}

        CAR_INTERFACE_DECL()

        CARAPI OnKeyguardExitResult(
            /* [in] */ Boolean success);

    private:
        AutoPtr<IOnKeyguardExitResult> mCallback;
    };

    // -------------------------------------------------------------
    // Async Handler
    // -------------------------------------------------------------
    class H : public HandlerBase
    {
    public:
        H(
            /* [in] */ CWindowManagerService * host) : mHost(host)
        {}

        ECode HandleMessage(
            /* [in] */ IMessage* msg);

    public:
        static const Int32 REPORT_FOCUS_CHANGE; // = 2;
        static const Int32 REPORT_LOSING_FOCUS; // = 3;
        static const Int32 DO_TRAVERSAL; // = 4;
        static const Int32 ADD_STARTING; // = 5;
        static const Int32 REMOVE_STARTING; // = 6;
        static const Int32 FINISHED_STARTING; // = 7;
        static const Int32 REPORT_APPLICATION_TOKEN_WINDOWS; // = 8;
        static const Int32 REPORT_APPLICATION_TOKEN_DRAWN; // = 9;
        static const Int32 WINDOW_FREEZE_TIMEOUT; // = 11;

        static const Int32 APP_TRANSITION_TIMEOUT; // = 13;
        static const Int32 PERSIST_ANIMATION_SCALE; // = 14;
        static const Int32 FORCE_GC; // = 15;
        static const Int32 ENABLE_SCREEN; // = 16;
        static const Int32 APP_FREEZE_TIMEOUT; // = 17;
        static const Int32 SEND_NEW_CONFIGURATION; // = 18;
        static const Int32 REPORT_WINDOWS_CHANGE; // = 19;
        static const Int32 DRAG_START_TIMEOUT; // = 20;
        static const Int32 DRAG_END_TIMEOUT; // = 21;
        static const Int32 REPORT_HARD_KEYBOARD_STATUS_CHANGE; // = 22;
        static const Int32 BOOT_TIMEOUT; // = 23;
        static const Int32 WAITING_FOR_DRAWN_TIMEOUT; // = 24;
        static const Int32 UPDATE_ANIM_PARAMETERS; // = 25;
        static const Int32 SHOW_STRICT_MODE_VIOLATION; // = 26;
        static const Int32 DO_ANIMATION_CALLBACK; // = 27;
        static const Int32 NOTIFY_ROTATION_CHANGED; // = 28;
        static const Int32 NOTIFY_WINDOW_TRANSITION; // = 29;
        static const Int32 NOTIFY_RECTANGLE_ON_SCREEN_REQUESTED; // = 30;
        static const Int32 NOTIFY_WINDOW_LAYERS_CHANGED; // = 31;

        static const Int32 DO_DISPLAY_ADDED; // = 32;
        static const Int32 DO_DISPLAY_REMOVED; // = 33;
        static const Int32 DO_DISPLAY_CHANGED; // = 34;

        static const Int32 CLIENT_FREEZE_TIMEOUT; // = 35;

        static const Int32 ANIMATOR_WHAT_OFFSET; // = 100000;
        static const Int32 SET_TRANSPARENT_REGION; // = ANIMATOR_WHAT_OFFSET + 1;
        static const Int32 CLEAR_PENDING_ACTIONS; // = ANIMATOR_WHAT_OFFSET + 2

    private:
        CWindowManagerService * mHost;
    };

private:
    class WindowManagerServiceCreator
        : public ElRefBase
        , public IRunnable
    {
    public:
        WindowManagerServiceCreator(
            /* [in] */ IContext* context,
            /* [in] */ CPowerManagerService* pm,
            /* [in] */ CDisplayManagerService* dm,
            /* [in] */ CInputManagerService* im,
            /* [in] */ IHandler* uiHandler,
            /* [in] */ Boolean haveInputMethods,
            /* [in] */ Boolean showBootMsgs,
            /* [in] */ Boolean onlyCore);

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface* pObject,
            /* [in] */ InterfaceID* pIID);

        CARAPI Run();

    public:
        AutoPtr<IContext> mContext;
        AutoPtr<CPowerManagerService> mPm;
        AutoPtr<CDisplayManagerService> mDm;
        AutoPtr<CInputManagerService> mIm;
        AutoPtr<IHandler> mUiHandler;
        Boolean mHaveInputMethods;
        Boolean mShowBootMsgs;
        Boolean mOnlyCore;
        CWindowManagerService* mInstance;
    };

    class PolicyInitializer
        : public ElRefBase
        , public IRunnable
    {
    public:
        PolicyInitializer(
            /* [in] */ CWindowManagerService* wmService);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI GetInterfaceID(
            /* [in] */ IInterface* pObject,
            /* [in] */ InterfaceID* pIID);

        CARAPI Run();

    private:
        CWindowManagerService* mHost;
    };

    class RotationWatcherDeathRecipint
        : public ElRefBase
        , public IProxyDeathRecipient
    {
    public:
        CAR_INTERFACE_DECL();

        RotationWatcherDeathRecipint(
            /* [in] */ CWindowManagerService* owner,
            /* [in] */ IBinder* watcherBinder);

        //@Override
        CARAPI ProxyDied();

    private:
        CWindowManagerService* mOwner;
        AutoPtr<IBinder> mWatcherBinder;
    };

    class LocalInterpolator
        : public ElRefBase
        , public IInterpolator
    {
    public:
        CAR_INTERFACE_DECL()

        CARAPI GetInterpolation(
            /* [in] */ Float input,
            /* [out] */ Float* output);
    };

public:
    CWindowManagerService();

    ~CWindowManagerService();

    static AutoPtr<CWindowManagerService> Main(
        /* [in] */ IContext* context,
        /* [in] */ CPowerManagerService* pm,
        /* [in] */ CDisplayManagerService* dm,
        /* [in] */ CInputManagerService* im,
        /* [in] */ IHandler* uiHandler,
        /* [in] */ IHandler* wmHandler,
        /* [in] */ Boolean haveInputMethods,
        /* [in] */ Boolean showBootMsgs,
        /* [in] */ Boolean onlyCore);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ Handle32 pm,
        /* [in] */ Handle32 displayManager,
        /* [in] */ Handle32 inputManager,
        /* [in] */ IHandler* uiHandler,
        /* [in] */ Boolean haveInputMethods,
        /* [in] */ Boolean showBootMsgs,
        /* [in] */ Boolean onlyCore);

    CARAPI_(AutoPtr<InputMonitor>) GetInputMonitor();

    /**
     * Return the list of Windows from the passed token on the given Display.
     * @param token The token with all the windows.
     * @param displayContent The display we are interested in.
     * @return List of windows from token that are on displayContent.
     */
    CARAPI_(AutoPtr< List<AutoPtr<WindowState> > >) GetTokenWindowsOnDisplay(
        /* [in] */ WindowToken* token,
        /* [in] */ DisplayContent* displayContent);

    /** TODO(cmautner): Is this the same as {@link WindowState#canReceiveKeys()} */
    static CARAPI_(Boolean) CanBeImeTarget(
        /* [in] */ WindowState* w);

    /**
     * Dig through the WindowStates and find the one that the Input Method will target.
     * @param willMove
     * @return The index+1 in mWindows of the discovered target.
     */
    CARAPI_(List< AutoPtr<WindowState> >::Iterator) FindDesiredInputMethodWindowItLocked(
        /* [in] */ Boolean willMove);

    CARAPI_(void) AddInputMethodWindowToListLocked(
        /* [in] */ WindowState* win);

    CARAPI_(void) SetInputMethodAnimLayerAdjustment(
        /* [in] */ Int32 adj);

    CARAPI_(void) LogWindowList(
        /* [in] */ List< AutoPtr<WindowState> >& windows,
        /* [in] */ const String& prefix);

    CARAPI_(void) MoveInputMethodDialogsLocked(
        /* [in] */ List< AutoPtr<WindowState> >::Iterator posIt);

    CARAPI_(Boolean) MoveInputMethodWindowsIfNeededLocked(
        /* [in] */ Boolean needAssignLayers);

    CARAPI_(void) AdjustInputMethodDialogsLocked();

    CARAPI_(Boolean) IsWallpaperVisible(
        /* [in] */ WindowState* wallpaperTarget);

    CARAPI_(Int32) AdjustWallpaperWindowsLocked();

    CARAPI_(void) SetWallpaperAnimLayerAdjustmentLocked(
        /* [in] */ Int32 adj);

    CARAPI_(Boolean) UpdateWallpaperOffsetLocked(
        /* [in] */ WindowState* wallpaperWin,
        /* [in] */ Int32 dw,
        /* [in] */ Int32 dh,
        /* [in] */ Boolean sync);

    CARAPI_(void) WallpaperOffsetsComplete(
        /* [in] */ IBinder* window);

    CARAPI_(void) UpdateWallpaperOffsetLocked(
        /* [in] */ WindowState* changingTarget,
        /* [in] */ Boolean sync);

    /**
     * Check wallpaper for visiblity change and notify window if so.
     * @param wallpaper The wallpaper to test and notify.
     * @param visible Current visibility.
     */
    CARAPI_(void) DispatchWallpaperVisibility(
        /* [in] */ WindowState* wallpaper,
        /* [in] */ Boolean visible);

    CARAPI_(void) UpdateWallpaperVisibilityLocked();

    CARAPI_(Int32) AddWindow(
        /* [in] */ CSession* session,
        /* [in] */ IIWindow* client,
        /* [in] */ Int32 seq,
        /* [in] */ IWindowManagerLayoutParams* attrs,
        /* [in] */ Int32 viewVisibility,
        /* [in] */ Int32 displayId,
        /* [in] */ IRect* inContentInsets,
        /* [in] */ IInputChannel* inInputChannel,
        /* [out] */ IRect** outContentInsets,
        /* [out] */ IInputChannel** outInputChannel);

    CARAPI_(void) RemoveWindow(
        /* [in] */ CSession* session,
        /* [in] */ IIWindow* client);

    CARAPI_(void) RemoveWindowLocked(
        /* [in] */ CSession* session,
        /* [in] */ WindowState* win);

    // static CARAPI_(void) LogSurface(
    //     /* [in] */ WindowState* w,
    //     /* [in] */ const String& msg,
    //     /* [in] */ RuntimeException where);

    // static CARAPI_(void) logSurface(WindowState w, String msg, RuntimeException where);

    // TODO(cmautner): Move to WindowStateAnimator.
    CARAPI_(void) SetTransparentRegionHint(
        /* [in] */ WindowStateAnimator* winAnimator,
        /* [in] */ IRegion* region);

    CARAPI_(void) SetTransparentRegionWindow(
        /* [in] */ CSession* session,
        /* [in] */ IIWindow* client,
        /* [in] */ IRegion* region);

    CARAPI_(void) SetInsetsWindow(
        /* [in] */ CSession* session,
        /* [in] */ IIWindow* client,
        /* [in] */ Int32 touchableInsets,
        /* [in] */ IRect* contentInsets,
        /* [in] */ IRect* visibleInsets,
        /* [in] */ IRegion* touchableRegion);

    CARAPI_(void) GetWindowDisplayFrame(
        /* [in] */ CSession* session,
        /* [in] */ IIWindow* client,
        /* [in] */ IRect* outDisplayFrame);

    CARAPI_(void) SetWindowWallpaperPositionLocked(
        /* [in] */ WindowState* window,
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ Float xStep,
        /* [in] */ Float yStep);

    CARAPI_(void) WallpaperCommandComplete(
        /* [in] */ IBinder* window,
        /* [in] */ IBundle* result);

    CARAPI_(AutoPtr<IBundle>) SendWindowWallpaperCommandLocked(
        /* [in] */ WindowState* window,
        /* [in] */ const String& action,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Int32 z,
        /* [in] */ IBundle* extras,
        /* [in] */ Boolean sync);

    CARAPI_(void) SetUniverseTransformLocked(
        /* [in] */ WindowState* window,
        /* [in] */ Float alpha,
        /* [in] */ Float offx,
        /* [in] */ Float offy,
        /* [in] */ Float dsdx,
        /* [in] */ Float dtdx,
        /* [in] */ Float dsdy,
        /* [in] */ Float dtdy);

    CARAPI_(void) OnRectangleOnScreenRequested(
        /* [in] */ IBinder* token,
        /* [in] */ IRect* rectangle,
        /* [in] */ Boolean immediate);

    CARAPI_(Int32) RelayoutWindow(
        /* [in] */ CSession* session,
        /* [in] */ IIWindow* client,
        /* [in] */ Int32 seq,
        /* [in] */ IWindowManagerLayoutParams* attrs,
        /* [in] */ Int32 requestedWidth,
        /* [in] */ Int32 requestedHeight,
        /* [in] */ Int32 viewVisibility,
        /* [in] */ Int32 flags,
        /* [in] */ IRect* inFrame,
        /* [in] */ IRect* inContentInsets,
        /* [in] */ IRect* inVisibleInsets,
        /* [in] */ IConfiguration* inConfig,
        /* [in] */ ISurface* inSurface,
        /* [out] */ IRect** outFrame,
        /* [out] */ IRect** outContentInsets,
        /* [out] */ IRect** outVisibleInsets,
        /* [out] */ IConfiguration** outConfig,
        /* [out] */ ISurface** outSurface);

    CARAPI_(void) PerformDeferredDestroyWindow(
        /* [in] */ CSession* session,
        /* [in] */ IIWindow* client);

    CARAPI_(Boolean) OutOfMemoryWindow(
        /* [in] */ CSession* session,
        /* [in] */ IIWindow* client);

    CARAPI_(void) FinishDrawingWindow(
        /* [in] */ CSession* session,
        /* [in] */ IIWindow* client);

    /**
     * Gets the compatibility scale of e window given its token.
     */
    CARAPI GetWindowCompatibilityScale(
        /* [in] */ IBinder* windowToken,
        /* [out] */ Float* scale);

    CARAPI GetWindowInfo(
        /* [in] */ IBinder* token,
        /* [out] */ IWindowInfo** info);

    CARAPI GetVisibleWindowsForDisplay(
        /* [in] */ Int32 displayId,
        /* [out] */ IObjectContainer** outInfos);

    /**
     * Sets the scale and offset for implementing accessibility magnification.
     */
    CARAPI MagnifyDisplay(
        /* [in] */ Int32 dipslayId,
        /* [in] */ Float scale,
        /* [in] */ Float offsetX,
        /* [in] */ Float offsetY);

    CARAPI_(AutoPtr<MagnificationSpec>) GetDisplayMagnificationSpecLocked(
        /* [in] */ Int32 displayId);

    CARAPI_(void) ValidateAppTokens(
        /* [in] */ List< AutoPtr<IBinder> >& tokens);

    CARAPI_(Boolean) CheckCallingPermission(
        /* [in] */ const String& permission,
        /* [in] */ const String& func);

    CARAPI_(Boolean) OkToDisplay();

    CARAPI_(AutoPtr<AppWindowToken>) FindAppWindowToken(
        /* [in] */ IBinder* token);

    CARAPI AddWindowToken(
        /* [in] */ IBinder* token,
        /* [in] */ Int32 type);

    CARAPI RemoveWindowToken(
        /* [in] */ IBinder* token);

    CARAPI AddAppToken(
        /* [in] */ Int32 addPos,
        /* [in] */ Int32 userId,
        /* [in] */ IApplicationToken* token,
        /* [in] */ Int32 groupId,
        /* [in] */ Int32 requestedOrientation,
        /* [in] */ Boolean fullscreen,
        /* [in] */ Boolean showWhenLocked);

    CARAPI SetAppGroupId(
        /* [in] */ IBinder* token,
        /* [in] */ Int32 groupId);

    CARAPI_(Int32) GetOrientationFromWindowsLocked();

    CARAPI_(Int32) GetOrientationFromAppTokensLocked();

    // Re-evaluate the current orientation from the caller's state.
    // If there is a change, the new Configuration is returned and the
    // caller must call setNewConfiguration() sometime later.
    CARAPI UpdateOrientationFromAppTokens(
        /* [in] */ IConfiguration* currentConfig,
        /* [in] */ IBinder* freezeThisOneIfNeeded,
        /* [out] */ IConfiguration** config);

    /*
     * Determine the new desired orientation of the display, returning
     * a non-null new Configuration if it has changed from the current
     * orientation.  IF TRUE IS RETURNED SOMEONE MUST CALL
     * setNewConfiguration() TO TELL THE WINDOW MANAGER IT CAN UNFREEZE THE
     * SCREEN.  This will typically be done for you if you call
     * sendNewConfiguration().
     *
     * The orientation is computed from non-application windows first. If none of
     * the non-application windows specify orientation, the orientation is computed from
     * application tokens.
     * @see android.view.IWindowManager#updateOrientationFromAppTokens(
     * android.os.IBinder)
     */
    CARAPI_(Boolean) UpdateOrientationFromAppTokensLocked(
        /* [in] */ Boolean inTransaction);

    CARAPI_(Int32) ComputeForcedAppOrientationLocked();

    CARAPI SetNewConfiguration(
        /* [in] */ IConfiguration* config);

    CARAPI SetAppOrientation(
        /* [in] */ IApplicationToken* token,
        /* [in] */ Int32 requestedOrientation);

    CARAPI GetAppOrientation(
        /* [in] */ IApplicationToken* token,
        /* [out] */ Int32* orientation);

    CARAPI SetFocusedApp(
        /* [in] */ IBinder* token,
        /* [in] */ Boolean moveFocusNow);

    CARAPI PrepareAppTransition(
        /* [in] */ Int32 transit,
        /* [in] */ Boolean alwaysKeepCurrent);

    CARAPI GetPendingAppTransition(
        /* [out] */ Int32* transit);

    CARAPI OverridePendingAppTransition(
        /* [in] */ const String& capsuleName,
        /* [in] */ Int32 enterAnim,
        /* [in] */ Int32 exitAnim,
        /* [in] */ IRemoteCallback* startedCallback);

    CARAPI OverridePendingAppTransitionScaleUp(
        /* [in] */ Int32 startX,
        /* [in] */ Int32 startY,
        /* [in] */ Int32 startWidth,
        /* [in] */ Int32 startHeight);

    CARAPI OverridePendingAppTransitionThumb(
        /* [in] */ IBitmap* srcThumb,
        /* [in] */ Int32 startX,
        /* [in] */ Int32 startY,
        /* [in] */ IRemoteCallback* startedCallback,
        /* [in] */ Boolean scaleUp);

    CARAPI ExecuteAppTransition();

    CARAPI SetAppStartingWindow(
        /* [in] */ IBinder* token,
        /* [in] */ const String& cap,
        /* [in] */ Int32 theme,
        /* [in] */ ICompatibilityInfo* compatInfo,
        /* [in] */ ICharSequence* nonLocalizedLabel,
        /* [in] */ Int32 labelRes,
        /* [in] */ Int32 icon,
        /* [in] */ Int32 windowFlags,
        /* [in] */ IBinder* transferFrom,
        /* [in] */ Boolean createIfNeeded);

    CARAPI SetAppWillBeHidden(
        /* [in] */ IBinder* token);

    CARAPI_(Boolean) SetTokenVisibilityLocked(
        /* [in] */ AppWindowToken* wtoken,
        /* [in] */ IWindowManagerLayoutParams* lp,
        /* [in] */ Boolean visible,
        /* [in] */ Int32 transit,
        /* [in] */ Boolean performLayout);

    CARAPI SetAppVisibility(
        /* [in] */ IBinder* token,
        /* [in] */ Boolean visible);

    CARAPI_(void) UnsetAppFreezingScreenLocked(
    /* [in] */ AppWindowToken* wtoken,
    /* [in] */ Boolean unfreezeSurfaceNow,
    /* [in] */ Boolean force);

    CARAPI_(void) StartAppFreezingScreenLocked(
        /* [in] */ AppWindowToken* wtoken,
        /* [in] */ Int32 configChanges);

    CARAPI StartAppFreezingScreen(
        /* [in] */ IBinder* token,
        /* [in] */ Int32 configChanges);

    CARAPI StopAppFreezingScreen(
        /* [in] */ IBinder* token,
        /* [in] */ Boolean force);

    CARAPI RemoveAppToken(
        /* [in] */ IBinder* token);

    CARAPI_(void) DumpAppTokensLocked();

    CARAPI_(void) DumpAnimatingAppTokensLocked();

    CARAPI_(void) DumpWindowsLocked();

    CARAPI MoveAppToken(
        /* [in] */ Int32 index,
        /* [in] */ IBinder* token);

    CARAPI MoveAppTokensToTop(
        /* [in] */ IObjectContainer* tokens);

    CARAPI MoveAppTokensToBottom(
        /* [in] */ IObjectContainer* tokens);

    CARAPI StartFreezingScreen(
        /* [in] */ Int32 exitAnim,
        /* [in] */ Int32 enterAnim);

    CARAPI StopFreezingScreen();

    // these require DISABLE_KEYGUARD permission
    CARAPI DisableKeyguard(
        /* [in] */ IBinder* token,
        /* [in] */ const String& tag);

    CARAPI ReenableKeyguard(
        /* [in] */ IBinder* token);

    /**
     * @see android.app.KeyguardManager#exitKeyguardSecurely
     */
    CARAPI ExitKeyguardSecurely(
        /* [in] */ IOnKeyguardExitResult* keyguardCallback);

    CARAPI InKeyguardRestrictedInputMode(
        /* [out] */ Boolean* result);

    CARAPI IsKeyguardLocked(
        /* [out] */ Boolean* isSecure);

    CARAPI IsKeyguardSecure(
        /* [out] */ Boolean* isSecure);

    CARAPI DismissKeyguard();

    CARAPI CloseSystemDialogs(
        /* [in] */ const String& reason);

    static CARAPI_(Float) FixScale(
        /* [in] */ Float scale);

    CARAPI SetAnimationScale(
        /* [in] */ Int32 which,
        /* [in] */ Float scale);

    CARAPI SetAnimationScales(
        /* [in] */ ArrayOf<Float>* scales);

    // These can only be called with the SET_ANIMATON_SCALE permission.
    CARAPI GetAnimationScale(
        /* [in] */ Int32 which,
        /* [out] */ Float* scale);

    CARAPI GetAnimationScales(
        /* [out, callee] */ ArrayOf<Float>** scales);

    /**
     * Returns a code that describes the current state of the lid switch.
     */
    // Called by window manager policy.  Not exposed externally.
    CARAPI GetLidState(
        /* [out] */ Int32* lidState);

    /**
     * Creates an input channel that will receive all input from the input dispatcher.
     */
    // Called by window manager policy.  Not exposed externally.
    CARAPI MonitorInput(
        /* [in] */ const String& name,
        /* [out] */ IInputChannel** inputChannel);

    /**
     * Switch the keyboard layout for the given device.
     * Direction should be +1 or -1 to go to the next or previous keyboard layout.
     */
    CARAPI SwitchKeyboardLayout(
        /* [in] */ Int32 deviceId,
        /* [in] */ Int32 direction);

    // Called by window manager policy.  Not exposed externally.
    CARAPI Shutdown(
        /* [in] */ Boolean confirm);

    // Called by window manager policy.  Not exposed externally.
    CARAPI RebootSafeMode(
        /* [in] */ Boolean confirm);

    CARAPI SetInputFilter(
        /* [in] */ IIInputFilter* filter);

    CARAPI_(void) SetCurrentUser(
        /* [in] */ Int32 newUserId);

    CARAPI_(void) EnableScreenAfterBoot();

    CARAPI_(void) EnableScreenIfNeededLocked();

    CARAPI_(void) PerformBootTimeout();

    CARAPI_(void) PerformEnableScreen();

    CARAPI_(void) ShowBootMessage(
        /* [in] */ ICharSequence* msg,
        /* [in] */ Boolean always);

    CARAPI_(void) HideBootMessagesLocked();

    CARAPI SetInTouchMode(
        /* [in] */ Boolean mode);

    // For StrictMode flashing a red border on violations from the UI
    // thread.  The uid/pid is implicit from the Binder call, and the Window
    // Manager uses that to determine whether or not the red border should
    // actually be shown.  (it will be ignored that pid doesn't have windows
    // on screen)
    // TODO: more accounting of which pid(s) turned it on, keep count,
    // only allow disables from pids which have count on, etc.
    CARAPI ShowStrictModeViolation(
        /* [in] */ Boolean on);

    // Proxy to set the system property for whether the flashing
    // should be enabled.  The 'enabled' value is null or blank for
    // the system default (differs per build variant) or any valid
    // boolean string as parsed by SystemProperties.getBoolean().
    CARAPI SetStrictModeVisualIndicatorPreference(
        /* [in] */ const String& value);

    /**
     * Takes a snapshot of the screen.  In landscape mode this grabs the whole screen.
     * In portrait mode, it grabs the upper region of the screen based on the vertical dimension
     * of the target image.
     *
     * @param displayId the Display to take a screenshot of.
     * @param width the width of the target bitmap
     * @param height the height of the target bitmap
     */
    // @Override
    CARAPI ScreenshotApplications(
        /* [in] */ IBinder* appToken,
        /* [in] */ Int32 displayId,
        /* [in] */ Int32 maxWidth,
        /* [in] */ Int32 maxHeight,
        /* [out] */ IBitmap** bitmap);

    /**
     * Freeze rotation changes.  (Enable "rotation lock".)
     * Persists across reboots.
     * @param rotation The desired rotation to freeze to, or -1 to use the
     * current rotation.
     */
    CARAPI FreezeRotation(
        /* [in] */ Int32 rotation);

    /**
     * Thaw rotation changes.  (Disable "rotation lock".)
     * Persists across reboots.
     */
    CARAPI ThawRotation();

    /**
     * Recalculate the current rotation.
     *
     * Called by the window manager policy whenever the state of the system changes
     * such that the current rotation might need to be updated, such as when the
     * device is docked or rotated into a new posture.
     */
    CARAPI UpdateRotation(
        /* [in] */ Boolean alwaysSendConfiguration,
        /* [in] */ Boolean forceRelayout);

    /**
     * Temporarily pauses rotation changes until resumed.
     *
     * This can be used to prevent rotation changes from occurring while the user is
     * performing certain operations, such as drag and drop.
     *
     * This call nests and must be matched by an equal number of calls to {@link #resumeRotation}.
     */
    CARAPI_(void) PauseRotationLocked();

    /**
     * Resumes normal rotation changes after being paused.
     */
    CARAPI_(void) ResumeRotationLocked();

    CARAPI_(void) UpdateRotationUnchecked(
        /* [in] */ Boolean alwaysSendConfiguration,
        /* [in] */ Boolean forceRelayout);

    // TODO(multidisplay): Rotate any display?
    /**
     * Updates the current rotation.
     *
     * Returns true if the rotation has been changed.  In this case YOU
     * MUST CALL sendNewConfiguration() TO UNFREEZE THE SCREEN.
     */
    CARAPI_(Boolean) UpdateRotationUncheckedLocked(
        /* [in] */ Boolean inTransaction);

    /**
     * Retrieve the current screen orientation, constants as per
     * {@link android.view.Surface}.
     */
    CARAPI GetRotation(
        /* [out] */ Int32* rotation);

    /**
     * Watch the rotation of the screen.  Returns the current rotation,
     * calls back when it changes.
     */
    CARAPI WatchRotation(
        /* [in] */ IRotationWatcher* watcher,
        /* [out] */ Int32* rotation);

    /**
     * Apps that use the compact menu panel (as controlled by the panelMenuIsCompact
     * theme attribute) on devices that feature a physical options menu key attempt to position
     * their menu panel window along the edge of the screen nearest the physical menu key.
     * This lowers the travel distance between invoking the menu panel and selecting
     * a menu option.
     *
     * This method helps control where that menu is placed. Its current implementation makes
     * assumptions about the menu key and its relationship to the screen based on whether
     * the device's natural orientation is portrait (width < height) or landscape.
     *
     * The menu key is assumed to be located along the bottom edge of natural-portrait
     * devices and along the right edge of natural-landscape devices. If these assumptions
     * do not hold for the target device, this method should be changed to reflect that.
     *
     * @return A {@link Gravity} value for placing the options menu window
     */
    CARAPI GetPreferredOptionsPanelGravity(
        /* [out] */ Int32* value);

    /**
     * Starts the view server on the specified port.
     *
     * @param port The port to listener to.
     *
     * @return True if the server was successfully started, false otherwise.
     *
     * @see com.android.server.wm.ViewServer
     * @see com.android.server.wm.ViewServer#VIEW_SERVER_DEFAULT_PORT
     */
    CARAPI StartViewServer(
        /* [in] */ Int32 port,
        /* [out] */ Boolean* result);

    CARAPI StopViewServer(
        /* [out] */ Boolean* result);

    /**
     * Indicates whether the view server is running.
     *
     * @return True if the server is running, false otherwise.
     *
     * @see com.android.server.wm.ViewServer
     */
    CARAPI IsViewServerRunning(
        /* [out] */ Boolean* result);

    /**
     * Lists all availble windows in the system. The listing is written in the
     * specified Socket's output stream with the following syntax:
     * windowHashCodeInHexadecimal windowName
     * Each line of the ouput represents a different window.
     *
     * @param client The remote client to send the listing to.
     * @return False if an error occured, true otherwise.
     */
    CARAPI_(Boolean) ViewServerListWindows(
        /* [in] */ ISocket* client);

    // TODO(multidisplay): Extend to multiple displays.
    /**
     * Returns the focused window in the following format:
     * windowHashCodeInHexadecimal windowName
     *
     * @param client The remote client to send the listing to.
     * @return False if an error occurred, true otherwise.
     */
    CARAPI_(Boolean) ViewServerGetFocusedWindow(
        /* [in] */ ISocket* client);

    /**
     * Sends a command to a target window. The result of the command, if any, will be
     * written in the output stream of the specified socket.
     *
     * The parameters must follow this syntax:
     * windowHashcode extra
     *
     * Where XX is the length in characeters of the windowTitle.
     *
     * The first parameter is the target window. The window with the specified hashcode
     * will be the target. If no target can be found, nothing happens. The extra parameters
     * will be delivered to the target window and as parameters to the command itself.
     *
     * @param client The remote client to sent the result, if any, to.
     * @param command The command to execute.
     * @param parameters The command parameters.
     *
     * @return True if the command was successfully delivered, false otherwise. This does
     *         not indicate whether the command itself was successful.
     */
    CARAPI_(Boolean) ViewServerWindowCommand(
        /* [in] */ ISocket* client,
        /* [in] */ const String& command,
        /* [in] */ const String& parameters);

    /**
     * Adds a listener for display content changes.
     */
    CARAPI AddDisplayContentChangeListener(
        /* [in] */ Int32 displayId,
        /* [in] */ IDisplayContentChangeListener* listener);

    /**
     * Removes a listener for display content changes.
     */
    CARAPI RemoveDisplayContentChangeListener(
        /* [in] */ Int32 displayId,
        /* [in] */ IDisplayContentChangeListener* listener);

    CARAPI_(void) ScheduleNotifyWindowTranstionIfNeededLocked(
        /* [in] */ WindowState* window,
        /* [in] */ Int32 transition);

    CARAPI_(void) AddWindowChangeListener(
        /* [in] */ IWindowChangeListener* listener);

    CARAPI_(void) RemoveWindowChangeListener(
        /* [in] */ IWindowChangeListener* listener);

    /*
     * Instruct the Activity Manager to fetch the current configuration and broadcast
     * that to config-changed listeners if appropriate.
     */
    CARAPI SendNewConfiguration();

    CARAPI_(AutoPtr<IConfiguration>) ComputeNewConfiguration();

    CARAPI_(AutoPtr<IConfiguration>) ComputeNewConfigurationLocked();

    CARAPI_(Boolean) ComputeScreenConfigurationLocked(
        /* [in] */ IConfiguration* config);

    CARAPI_(Boolean) IsHardKeyboardAvailable();

    CARAPI_(Boolean) IsHardKeyboardEnabled();

    CARAPI_(void) SetHardKeyboardEnabled(
        /* [in] */ Boolean enabled);

    CARAPI_(void) SetOnHardKeyboardStatusChangeListener(
        /* [in] */ IOnHardKeyboardStatusChangeListener* listener);

    CARAPI_(void) NotifyHardKeyboardStatusChange();

    CARAPI_(AutoPtr<IBinder>) PrepareDragSurface(
        /* [in] */ IIWindow* window,
        /* [in] */ ISurfaceSession* session,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ ISurface* outSurface);

    CARAPI PauseKeyDispatching(
        /* [in] */ IBinder* token);

    CARAPI ResumeKeyDispatching(
        /* [in] */ IBinder* token);

    CARAPI SetEventDispatching(
        /* [in] */ Boolean enabled);

    CARAPI GetFocusedWindowToken(
        /* [out] */ IBinder** token);

    CARAPI_(Boolean) DetectSafeMode();

    CARAPI_(void) DisplayReady();

    CARAPI SystemReady();

    CARAPI OpenSession(
        /* [in] */ IInputMethodClient* client,
        /* [in] */ IInputContext* inputContext,
        /* [out] */ IWindowSession** session);

    CARAPI InputMethodClientHasFocus(
        /* [in] */ IInputMethodClient* client,
        /* [out] */ Boolean* result);

    CARAPI_(void) GetInitialDisplaySize(
        /* [in] */ Int32 displayId,
        /* [in] */ IPoint** size);

    CARAPI SetForcedDisplaySize(
        /* [in] */ Int32 displayId,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI ClearForcedDisplaySize(
        /* [in] */ Int32 displayId);

    CARAPI SetForcedDisplayDensity(
        /* [in] */ Int32 displayId,
        /* [in] */ Int32 density);

    CARAPI ClearForcedDisplayDensity(
        /* [in] */ Int32 displayId);

    // Is the device configured to have a full system bar for larger screens?
    CARAPI HasSystemNavBar(
        /* [out] */ Boolean* result);

    CARAPI WindowForClientLocked(
        /* [in] */ CSession* session,
        /* [in] */ IIWindow* client,
        /* [in] */ Boolean throwOnError,
        /* [out] */ WindowState** win);

    CARAPI WindowForClientLocked(
        /* [in] */ CSession* session,
        /* [in] */ IBinder* client,
        /* [in] */ Boolean throwOnError,
        /* [out] */ WindowState** win);

    CARAPI_(void) RebuildAppWindowListLocked();

    CARAPI_(void) MakeWindowFreezingScreenIfNeededLocked(
        /* [in] */ WindowState* w);

    /**
     * Extracted from {@link #performLayoutAndPlaceSurfacesLockedInner} to reduce size of method.
     * @param windows List of windows on default display.
     * @return bitmap indicating if another pass through layout must be made.
     */
    CARAPI_(Int32) HandleAppTransitionReadyLocked(
        /* [in] */ List< AutoPtr<WindowState> >& windows);

    CARAPI_(void) CheckDrawnWindowsLocked();

    CARAPI WaitForWindowDrawn(
        /* [in] */ IBinder* token,
        /* [in] */ IRemoteCallback* callback,
        /* [out] */ Boolean* result);

    CARAPI_(void) SetHoldScreenLocked(
        /* [in] */ CSession* newHoldScreen);

    CARAPI RequestTraversal();

    CARAPI_(void) RequestTraversalLocked();

    /** Note that Locked in this case is on mLayoutToAnim */
    CARAPI_(void) ScheduleAnimationLocked();

    CARAPI_(void) UpdateLayoutToAnimationLocked();

    CARAPI_(void) UpdateLayoutToAnimWallpaperTokens();

    CARAPI_(void) SetAnimDimParams(
        /* [in] */ Int32 displayId,
        /* [in] */ DimAnimator::Parameters* params);

    CARAPI_(void) StartDimmingLocked(
        /* [in] */ WindowStateAnimator* winAnimator,
        /* [in] */ Float target,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI_(void) StopDimmingLocked(
        /* [in] */ Int32 displayId);

    CARAPI_(Boolean) ReclaimSomeSurfaceMemoryLocked(
        /* [in] */ WindowStateAnimator* winAnimator,
        /* [in] */ const String& operation,
        /* [in] */ Boolean secure);

    static CARAPI_(Int32) GetPropertyInt(
        /* [in] */ ArrayOf<String>* tokens,
        /* [in] */ Int32 index,
        /* [in] */ Int32 defUnits,
        /* [in] */ Int32 defDps,
        /* [in] */ IDisplayMetrics* dm);

    CARAPI_(void) CreateWatermarkInTransaction();

    /**
     * Called by the status bar to notify Views of changes to System UI visiblity.
     */
    CARAPI StatusBarVisibilityChanged(
        /* [in] */ Int32 visibility);

    // TOOD(multidisplay): StatusBar on multiple screens?
    CARAPI_(void) UpdateStatusBarVisibilityLocked(
        /* [in] */ Int32 visibility);

    /**
     * Ask the window manager to re-evaluate the system UI flags.
     */
    CARAPI ReevaluateStatusBarVisibility();

    /**
     * Add a fake window to the window manager.  This window sits
     * at the top of the other windows and consumes events.
     */
    CARAPI AddFakeWindow(
        /* [in] */ ILooper* looper,
        /* [in] */ IInputEventReceiverFactory* inputEventReceiverFactory,
        /* [in] */ const String& name,
        /* [in] */ Int32 windowType,
        /* [in] */ Int32 layoutParamsFlags,
        /* [in] */ Boolean canReceiveKeys,
        /* [in] */ Boolean hasFocus,
        /* [in] */ Boolean touchFullscreen,
        /* [out] */ IFakeWindow** fakeWindow);

    CARAPI_(Boolean) RemoveFakeWindowLocked(
        /* [in] */ IFakeWindow* window);

    // It is assumed that this method is called only by InputMethodManagerService.
    CARAPI_(void) SaveLastInputMethodWindowForTransition();

    /**
     * Device has a software navigation bar (separate from the status bar).
     */
    CARAPI HasNavigationBar(
        /* [out] */ Boolean* result);

    /**
     * Lock the device immediately with the specified options (can be null).
     */
    CARAPI LockNow(
        /* [in] */ IBundle* options);

    /**
     * Device is in safe mode.
     */
    CARAPI IsSafeModeEnabled(
        /* [out] */ Boolean* isSafe);

    /**
     * Tell keyguard to show the assistant (Intent.ACTION_ASSIST) after asking for the user's
     * credentials.
     */
    CARAPI ShowAssistant();

    CARAPI KeyEnterMouseMode();

    CARAPI KeyExitMouseMode();

    CARAPI KeySetMouseMoveCode(int left,int right,int top,int bottom);

    CARAPI KeySetMouseBtnCode(int leftbtn,int midbtn,int rightbtn);

    CARAPI KeySetMouseDistance(int distance);

    CARAPI ResetInputCalibration();

    // CARAPI_(void) DumpPolicyLocked(PrintWriter pw, String[] args, boolean dumpAll);

    // CARAPI_(void) DumpAnimatorLocked(PrintWriter pw, String[] args, boolean dumpAll);

    // CARAPI_(void) DumpTokensLocked(PrintWriter pw, boolean dumpAll);

    // CARAPI_(void) DumpSessionsLocked(PrintWriter pw, boolean dumpAll);

    // CARAPI_(void) DumpWindowsLocked(PrintWriter pw, boolean dumpAll,
    //     ArrayList<WindowState> windows);

    // CARAPI_(void) DumpWindowsNoHeaderLocked(PrintWriter pw, boolean dumpAll,
    //     ArrayList<WindowState> windows);

    // CARAPI_(Boolean) DumpWindows(PrintWriter pw, String name, String[] args,
    //         int opti, boolean dumpAll);

    // CARAPI_(void) DumpLastANRLocked(PrintWriter pw);

    /**
     * Saves information about the state of the window manager at
     * the time an ANR occurred before anything else in the system changes
     * in response.
     *
     * @param appWindowToken The application that ANR'd, may be null.
     * @param windowState The window that ANR'd, may be null.
     */
    CARAPI_(void) SaveANRStateLocked(
        /* [in] */ AppWindowToken* appWindowToken,
        /* [in] */ WindowState* windowState);

    // CARAPI_(void) Dump(FileDescriptor fd, PrintWriter pw, String[] args);

    // Called by the heartbeat to ensure locks are not held indefnitely (for deadlock detection).
    CARAPI_(void) Monitor();

    CARAPI_(void) DebugLayoutRepeats(
        /* [in] */ const String& msg,
        /* [in] */ Int32 pendingLayoutChanges);

    CARAPI_(void) CreateDisplayContentLocked(
        /* [in] */ IDisplay* display);

    /**
     * Retrieve the DisplayContent for the specified displayId. Will create a new DisplayContent if
     * there is a Display for the displayId.
     * @param displayId The display the caller is interested in.
     * @return The DisplayContent associated with displayId or null if there is no Display for it.
     */
    CARAPI_(AutoPtr<DisplayContent>) GetDisplayContentLocked(
        /* [in] */ Int32 displayId);

    // There is an inherent assumption that this will never return null.
    CARAPI_(AutoPtr<DisplayContent>) GetDefaultDisplayContentLocked();

    CARAPI_(List< AutoPtr<WindowState> >&) GetDefaultWindowListLocked();

    CARAPI_(AutoPtr<IDisplayInfo>) GetDefaultDisplayInfoLocked();

    CARAPI OnDisplayAdded(
        /* [in] */ Int32 displayId);

    CARAPI OnDisplayRemoved(
        /* [in] */ Int32 displayId);

    CARAPI OnDisplayChanged(
        /* [in] */ Int32 displayId);

private:
    CARAPI_(void) InitPolicy(
        /* [in] */ IHandler* uiHandler);

    CARAPI_(void) PlaceWindowAfter(
        /* [in] */ WindowState* pos,
        /* [in] */ WindowState* window);

    CARAPI_(void) PlaceWindowBefore(
        /* [in] */ WindowState* pos,
        /* [in] */ WindowState* window);

    //This method finds out the index of a window that has the same app token as
    //win. used for z ordering the windows in mWindows
    CARAPI_(List< AutoPtr<WindowState> >::Iterator) FindIteratorBasedOnAppTokens(
        /* [in] */ WindowState* win);

    /**
     * Recursive search through a WindowList and all of its windows' children.
     * @param targetWin The window to search for.
     * @param windows The list to search.
     * @return The index of win in windows or of the window that is an ancestor of win.
     */
    CARAPI_(List<AutoPtr<WindowState> >::Iterator) IndexOfWinInWindowList(
        /* [in] */ WindowState* win,
        /* [in] */ List<AutoPtr<WindowState> >& windows);

    CARAPI_(void) AddWindowToListInOrderLocked(
        /* [in] */ WindowState* targetWin,
        /* [in] */ Boolean addToToken);

    CARAPI_(List< AutoPtr<WindowState> >::Iterator) TmpRemoveWindowLocked(
        /* [in] */ List< AutoPtr<WindowState> >::Iterator interestingPosIt,
        /* [in] */ WindowState* win);

    CARAPI_(void) ReAddWindowToListInOrderLocked(
        /* [in] */ WindowState* win);

    CARAPI_(void) RemoveWindowInnerLocked(
        /* [in] */ CSession* session,
        /* [in] */ WindowState* win);

    CARAPI_(void) ScheduleNotifyRectangleOnScreenRequestedIfNeededLocked(
        /* [in] */ WindowState* window,
        /* [in] */ IRect* rectangle,
        /* [in] */ Boolean immediate);

    CARAPI_(void) HandleNotifyRectangleOnScreenRequested(
        /* [in] */ Int32 displayId,
        /* [in] */ IRect* rectangle,
        /* [in] */ Boolean immediate);

    CARAPI_(AutoPtr<IWindowInfo>) GetWindowInfoForWindowStateLocked(
        /* [in] */ WindowState* window);

    CARAPI_(AutoPtr<AttributeCache::Entry>) GetCachedAnimations(
        /* [in] */ Int32 userId,
        /* [in] */ IWindowManagerLayoutParams* lp);

    CARAPI_(AutoPtr<AttributeCache::Entry>) GetCachedAnimations(
        /* [in] */ Int32 userId,
        /* [in] */ const String& packageName,
        /* [in] */ Int32 resId);

    CARAPI_(AutoPtr<IAnimation>) LoadAnimation(
        /* [in] */ Int32 userId,
        /* [in] */ const String& packageName,
        /* [in] */ Int32 resId);

    CARAPI_(AutoPtr<IAnimation>) LoadAnimation(
        /* [in] */ Int32 userId,
        /* [in] */ IWindowManagerLayoutParams* lp,
        /* [in] */ Int32 animAttr);

    CARAPI_(AutoPtr<IAnimation>) CreateExitAnimationLocked(
        /* [in] */ Int32 transit,
        /* [in] */ Int32 duration);

    /**
     * Compute the pivot point for an animation that is scaling from a small
     * rect on screen to a larger rect.  The pivot point varies depending on
     * the distance between the inner and outer edges on both sides.  This
     * function computes the pivot point for one dimension.
     * @param startPos  Offset from left/top edge of outer rectangle to
     * left/top edge of inner rectangle.
     * @param finalScale The scaling factor between the size of the outer
     * and inner rectangles.
     */
    static CARAPI_(Float) ComputePivot(
        /* [in] */ Int32 startPos,
        /* [in] */ Float finalScale);

    CARAPI_(AutoPtr<IAnimation>) CreateScaleUpAnimationLocked(
        /* [in] */ Int32 transit,
        /* [in] */ Boolean enter);

    CARAPI_(AutoPtr<IAnimation>) CreateThumbnailAnimationLocked(
        /* [in] */ Int32 transit,
        /* [in] */ Boolean enter,
        /* [in] */ Boolean thumb,
        /* [in] */ Boolean scaleUp);

    CARAPI_(Boolean) ApplyAnimationLocked(
        /* [in] */ AppWindowToken* win,
        /* [in] */ IWindowManagerLayoutParams* lp,
        /* [in] */ Int32 transit,
        /* [in] */ Boolean isEntrance);

    /**
     *  Find the location to insert a new AppWindowToken into the window-ordered app token list.
     *  Note that mAppTokens.size() == mAnimatingAppTokens.size() + 1.
     * @param addPos The location the token was inserted into in mAppTokens.
     * @param atoken The token to insert.
     */
    CARAPI_(void) AddAppTokenToAnimating(
        /* [in] */ Int32 addPos,
        /* [in] */ Int32 userId,
        /* [in] */ AppWindowToken* atoken);

    CARAPI_(AutoPtr<IConfiguration>) UpdateOrientationFromAppTokensLocked(
        /* [in] */ IConfiguration* currentConfig,
        /* [in] */ IBinder* freezeThisOneIfNeeded);

    CARAPI_(void) ScheduleAnimationCallback(
        /* [in] */ IRemoteCallback* cb);

    CARAPI_(Boolean) TmpRemoveAppWindowsLocked(
        /* [in] */ WindowToken* token);

    CARAPI_(List< AutoPtr<WindowState> >::Iterator) FindWindowOffsetLocked(
        /* [in] */ List< AutoPtr<WindowState> >& windows,
        /* [in] */ List< AutoPtr<AppWindowToken> >::Iterator tokenPosIt);

    CARAPI_(List< AutoPtr<WindowState> >::Iterator) ReAddWindowLocked(
        /* [in] */ List< AutoPtr<WindowState> >::Iterator it,
        /* [in] */ WindowState* win);

    CARAPI_(List< AutoPtr<WindowState> >::Iterator) ReAddAppWindowsLocked(
        /* [in] */ DisplayContent* displayContent,
        /* [in] */ List< AutoPtr<WindowState> >::Iterator it,
        /* [in] */ WindowToken* token);

    CARAPI_(void) RemoveAppTokensLocked(
        /* [in] */ IObjectContainer* tokens);

    CARAPI_(void) MoveAppWindowsLocked(
        /* [in] */ AppWindowToken* wtoken,
        /* [in] */ List< AutoPtr<AppWindowToken> >::Iterator tokenPosIt,
        /* [in] */ Boolean updateFocusAndLayout);

    CARAPI_(void) MoveAppWindowsLocked(
        /* [in] */ IObjectContainer* tokens,
        /* [in] */ List< AutoPtr<AppWindowToken> >::Iterator tokenPosIt);

    CARAPI_(void) SetAnimatorDurationScale(
        /* [in] */ Float scale);

    CARAPI_(void) ShowStrictModeViolation(
        /* [in] */ Int32 arg,
        /* [in] */ Int32 pid);

    CARAPI_(Boolean) IsSystemSecure();

    CARAPI_(void) HandleNotifyWindowTransition(
        /* [in] */ Int32 transition,
        /* [in] */ IWindowInfo* info);

    CARAPI_(void) ScheduleNotifyRotationChangedIfNeededLocked(
        /* [in] */ DisplayContent* displayContent,
        /* [in] */ Int32 rotation);

    CARAPI_(void) HandleNotifyRotationChanged(
        /* [in] */ Int32 displayId,
        /* [in] */ Int32 rotation);

    CARAPI_(void) ScheduleNotifyWindowLayersChangedIfNeededLocked(
        /* [in] */ DisplayContent* displayContent);

    CARAPI_(void) HandleNotifyWindowLayersChanged(
        /* [in] */ DisplayContent* displayContent);

    CARAPI_(void) NotifyWindowsChanged();

    CARAPI_(void) NotifyFocusChanged();

    CARAPI_(AutoPtr<WindowState>) FindWindow(
        /* [in] */ Int32 hashCode);

    CARAPI_(void) AdjustDisplaySizeRanges(
        /* [in] */ IDisplayInfo* displayInfo,
        /* [in] */ Int32 rotation,
        /* [in] */ Int32 dw,
        /* [in] */ Int32 dh);

    CARAPI_(Int32) ReduceConfigLayout(
        /* [in] */ Int32 curLayout,
        /* [in] */ Int32 rotation,
        /* [in] */ Float density,
        /* [in] */ Int32 dw,
        /* [in] */ Int32 dh);

    CARAPI_(void) ComputeSizeRangesAndScreenLayout(
        /* [in] */ IDisplayInfo* displayInfo,
        /* [in] */ Boolean rotated,
        /* [in] */ Int32 dw,
        /* [in] */ Int32 dh,
        /* [in] */ Float density,
        /* [in] */ IConfiguration* outConfig);

    CARAPI_(Int32) ReduceCompatConfigWidthSize(
        /* [in] */ Int32 curSize,
        /* [in] */ Int32 rotation,
        /* [in] */ IDisplayMetrics* dm,
        /* [in] */ Int32 dw,
        /* [in] */ Int32 dh);

    CARAPI_(Int32) ComputeCompatSmallestWidth(
        /* [in] */ Boolean rotated,
        /* [in] */ IDisplayMetrics* dm,
        /* [in] */ Int32 dw,
        /* [in] */ Int32 dh);

    CARAPI_(AutoPtr<WindowState>) GetFocusedWindow();

    CARAPI_(AutoPtr<WindowState>) GetFocusedWindowLocked();

    CARAPI_(void) DisplayReady(
        /* [in] */ Int32 displayId);

    // TODO(multidisplay): Call isScreenOn for each display.
    CARAPI_(void) SendScreenStatusToClientsLocked();

    // -------------------------------------------------------------
    // Async Handler
    // -------------------------------------------------------------
    CARAPI HandleReportFocusChange();

    CARAPI HandleReportLosingFocus();

    CARAPI HandleDoTraversal();

    CARAPI HandleAddStarting(
        /* [in] */ AppWindowToken* wtoken);

    CARAPI HandleRemoveStarting(
        /* [in] */ AppWindowToken* wtoken);

    CARAPI HandleFinishedStarting();

    CARAPI HandleReportApplicationTokenDrawn(
        /* [in] */ AppWindowToken* wtoken);

    CARAPI HandleReportApplicationTokenWindows(
        /* [in] */ Boolean nowVisible,
        /* [in] */ Boolean nowGone,
        /* [in] */ AppWindowToken* wtoken);

    CARAPI HandleWindowFreezeTimeout();

    CARAPI HandleAppTransitionTimeout();

    CARAPI HandlePersistAnimationScale();

    CARAPI HandleForceGc();

    CARAPI HandleAppFreezeTimeout();

    CARAPI HandleClientFreezeTimeout();

    CARAPI HandleReportWindowsChange();

    CARAPI HandleDragStartTimeout(
        /* [in] */ IBinder* winBinder);

    CARAPI HandleDragEndTimeout(
        /* [in] */ IBinder* winBinder);

    CARAPI HandleWaitingForDrawnTimeout(
        /* [in] */ WindowState* windowState,
        /* [in] */ IRemoteCallback* remoteCallback);

    CARAPI HandleUpdateAnimParameters();

    CARAPI HandleDoDisplayAdded(
        /* [in] */ Int32 displayId);

    CARAPI HandleDoDisplayRemoved(
        /* [in] */ Int32 displayId);

    CARAPI HandleDoDisplayChanged(
        /* [in] */ Int32 displayId);

    CARAPI_(void) ReadForcedDisplaySizeAndDensityLocked(
        /* [in] */ DisplayContent* displayContent);

    // displayContent must not be null
    CARAPI_(void) SetForcedDisplaySizeLocked(
        /* [in] */ DisplayContent* displayContent,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    // displayContent must not be null
    CARAPI_(void) SetForcedDisplayDensityLocked(
        /* [in] */ DisplayContent* displayContent,
        /* [in] */ Int32 density);

    // displayContent must not be null
    CARAPI_(void) ReconfigureDisplayLocked(
        /* [in] */ DisplayContent* displayContent);

    CARAPI_(void) RebuildAppWindowListLocked(
        /* [in] */ DisplayContent* displayContent);

    CARAPI_(void) AssignLayersLocked(
        /* [in] */ List< AutoPtr<WindowState> >& windows);

    CARAPI_(void) PerformLayoutAndPlaceSurfacesLocked();

    CARAPI_(void) PerformLayoutAndPlaceSurfacesLockedLoop();

    CARAPI_(void) PerformLayoutLockedInner(
        /* [in] */ DisplayContent* displayContent,
        /* [in] */ Boolean initial,
        /* [in] */ Boolean updateInputWindows);

    /**
     * Extracted from {@link #performLayoutAndPlaceSurfacesLockedInner} to reduce size of method.
     * @return bitmap indicating if another pass through layout must be made.
     */
    CARAPI_(Int32) HandleAnimatingStoppedAndTransitionLocked();

    /**
     * Extracted from {@link #performLayoutAndPlaceSurfacesLockedInner} to reduce size of method.
     *
     * @return bitmap indicating if another pass through layout must be made.
     */
    CARAPI_(Int32) AnimateAwayWallpaperLocked();

    CARAPI_(void) UpdateResizingWindows(
        /* [in] */ WindowState* w);

    /**
     * Extracted from {@link #performLayoutAndPlaceSurfacesLockedInner} to reduce size of method.
     *
     * @param w WindowState this method is applied to.
     * @param currentTime The time which animations use for calculating transitions.
     * @param innerDw Width of app window.
     * @param innerDh Height of app window.
     */
    CARAPI_(void) HandleNotObscuredLocked(
        /* [in] */ WindowState* w,
        /* [in] */ Int64 currentTime,
        /* [in] */ Int32 innerDw,
        /* [in] */ Int32 innerDh);

    CARAPI_(void) UpdateAllDrawnLocked();

    // "Something has changed!  Let's make it correct now."
    CARAPI_(void) PerformLayoutAndPlaceSurfacesLockedInner(
        /* [in] */ Boolean recoveringMemory);

    CARAPI_(Int32) ToBrightnessOverride(
        /* [in] */ Float value);

    CARAPI_(Boolean) NeedsLayout();

    CARAPI_(Boolean) CopyAnimToLayoutParamsLocked();

    CARAPI_(Boolean) UpdateFocusedWindowLocked(
        /* [in] */ Int32 mode,
        /* [in] */ Boolean updateInputWindows);

    CARAPI_(void) FinishUpdateFocusedWindowAfterAssignLayersLocked(
        /* [in] */ Boolean updateInputWindows);

    CARAPI_(AutoPtr<WindowState>) ComputeFocusedWindowLocked();

    CARAPI_(AutoPtr<WindowState>) FindFocusedWindowLocked(
        /* [in] */ DisplayContent* displayContent);

    CARAPI_(void) StartFreezingDisplayLocked(
        /* [in] */ Boolean inTransaction,
        /* [in] */ Int32 exitAnim,
        /* [in] */ Int32 enterAnim);

    CARAPI_(void) StopFreezingDisplayLocked();

    CARAPI_(void) HandleDisplayAddedLocked(
        /* [in] */ Int32 displayId);

    CARAPI_(void) HandleDisplayRemovedLocked(
        /* [in] */ Int32 displayId);

    CARAPI_(void) HandleDisplayChangedLocked(
        /* [in] */ Int32 displayId);

public:
    static const String TAG;
    static const Boolean DEBUG;
    static const Boolean DEBUG_ADD_REMOVE;
    static const Boolean DEBUG_FOCUS;
    static const Boolean DEBUG_ANIM;
    static const Boolean DEBUG_LAYOUT;
    static const Boolean DEBUG_RESIZE;
    static const Boolean DEBUG_LAYERS;
    static const Boolean DEBUG_INPUT;
    static const Boolean DEBUG_INPUT_METHOD;
    static const Boolean DEBUG_VISIBILITY;
    static const Boolean DEBUG_WINDOW_MOVEMENT;
    static const Boolean DEBUG_TOKEN_MOVEMENT;
    static const Boolean DEBUG_ORIENTATION;
    static const Boolean DEBUG_APP_ORIENTATION;
    static const Boolean DEBUG_CONFIGURATION;
    static const Boolean DEBUG_APP_TRANSITIONS;
    static const Boolean DEBUG_STARTING_WINDOW;
    static const Boolean DEBUG_REORDER;
    static const Boolean DEBUG_WALLPAPER;
    static const Boolean DEBUG_WALLPAPER_LIGHT;
    static const Boolean DEBUG_DRAG;
    static const Boolean DEBUG_SCREEN_ON;
    static const Boolean DEBUG_SCREENSHOT;
    static const Boolean DEBUG_BOOT;
    static const Boolean DEBUG_LAYOUT_REPEATS;
    static const Boolean DEBUG_SURFACE_TRACE;
    static const Boolean DEBUG_WINDOW_TRACE;
    static const Boolean SHOW_SURFACE_ALLOC;
    static const Boolean SHOW_TRANSACTIONS;
    static const Boolean SHOW_LIGHT_TRANSACTIONS;
    static const Boolean HIDE_STACK_CRAWLS;
    static const Int32 LAYOUT_REPEAT_THRESHOLD;

    static const Boolean PROFILE_ORIENTATION;
    static const Boolean mLocalLOGV;

    /** How much to multiply the policy's type layer, to reserve room
     * for multiple windows of the same type and Z-ordering adjustment
     * with TYPE_LAYER_OFFSET. */
    static const Int32 TYPE_LAYER_MULTIPLIER;

    /** Offset from TYPE_LAYER_MULTIPLIER for moving a group of windows above
     * or below others in the same layer. */
    static const Int32 TYPE_LAYER_OFFSET;

    /** How much to increment the layer for each window, to reserve room
     * for effect surfaces between them.
     */
    static const Int32 WINDOW_LAYER_MULTIPLIER;

    /**
     * Dim surface layer is immediately below target window.
     */
    static const Int32 LAYER_OFFSET_DIM;

    /**
     * Blur surface layer is immediately below dim layer.
     */
    static const Int32 LAYER_OFFSET_BLUR;

    /**
     * Animation thumbnail is as far as possible below the window above
     * the thumbnail (or in other words as far as possible above the window
     * below it).
     */
    static const Int32 LAYER_OFFSET_THUMBNAIL;

    /**
     * Layer at which to put the rotation freeze snapshot.
     */
    static const Int32 FREEZE_LAYER;

    /**
     * Layer at which to put the mask for emulated screen sizes.
     */
    static const Int32 MASK_LAYER;

    /** The maximum length we will accept for a loaded animation duration:
     * this is 10 seconds.
     */
    static const Int32 MAX_ANIMATION_DURATION;

    /** Amount of time (in milliseconds) to animate the dim surface from one
     * value to another, when no window animation is driving it.
     */
    static const Int32 DEFAULT_DIM_DURATION;

    /** Amount of time (in milliseconds) to animate the fade-in-out transition for
     * compatible windows.
     */
    static const Int32 DEFAULT_FADE_IN_OUT_DURATION;

    /** Amount of time (in milliseconds) to delay before declaring a window freeze timeout. */
    static const Int32 WINDOW_FREEZE_TIMEOUT_DURATION;

    /** Fraction of animation at which the recents thumbnail becomes completely transparent */
    static const Float RECENTS_THUMBNAIL_FADEOUT_FRACTION;

    /**
     * If true, the window manager will do its own custom freezing and general
     * management of the screen during rotation.
     */
    static const Boolean CUSTOM_SCREEN_ROTATION;

    // Default input dispatching timeout in nanoseconds.
    static const Int64 DEFAULT_INPUT_DISPATCHING_TIMEOUT_NANOS;

    static const Int32 UPDATE_FOCUS_NORMAL;
    static const Int32 UPDATE_FOCUS_WILL_ASSIGN_LAYERS;
    static const Int32 UPDATE_FOCUS_PLACING_SURFACES;
    static const Int32 UPDATE_FOCUS_WILL_PLACE_SURFACES;

    // We give a wallpaper up to 150ms to finish scrolling.
    static const Int64 WALLPAPER_TIMEOUT;
    // Time we wait after a timeout before trying to wait again.
    static const Int64 WALLPAPER_TIMEOUT_RECOVERY;

    static const Int32 ADJUST_WALLPAPER_LAYERS_CHANGED;
    static const Int32 ADJUST_WALLPAPER_VISIBILITY_CHANGED;

    AutoPtr<IBroadcastReceiver> mBroadcastReceiver;

    // Current user when multi-user is enabled. Don't show windows of non-current user.
    Int32 mCurrentUserId;

    AutoPtr<IContext> mContext;

    Boolean mHaveInputMethods;

    Boolean mAllowBootMessages;

    Boolean mLimitedAlphaCompositing;

    AutoPtr<IWindowManagerPolicy> mPolicy;

    AutoPtr<IIActivityManager> mActivityManager;

    AutoPtr<IIBatteryStats> mBatteryStats;

    /**
     * All currently active sessions with clients.
     */
    HashSet<AutoPtr<IWindowSession> > mSessions;

    /**
     * Mapping from an IIWindow IBinder to the server's Window object.
     * This is also used as the lock for all of our state.
     */
    HashMap<AutoPtr<IBinder>, AutoPtr<WindowState> > mWindowMap;
    Object mWindowMapLock;

    /**
     * Mapping from a token IBinder to a WindowToken object.
     */
    HashMap<AutoPtr<IBinder>, AutoPtr<WindowToken> > mTokenMap;

    /**
     * Window tokens that are in the process of exiting, but still
     * on screen for animations.
     */
    List< AutoPtr<WindowToken> > mExitingTokens;

    /**
     * List controlling the ordering of windows in different applications which must
     * be kept in sync with ActivityManager.
     */
    List< AutoPtr<AppWindowToken> > mAppTokens;

    /**
     * AppWindowTokens in the Z order they were in at the start of an animation. Between
     * animations this list is maintained in the exact order of mAppTokens. If tokens
     * are added to mAppTokens during an animation an attempt is made to insert them at the same
     * logical location in this list. Note that this list is always in sync with mWindows.
     */
    List< AutoPtr<AppWindowToken> > mAnimatingAppTokens;

    /**
     * Application tokens that are in the process of exiting, but still
     * on screen for animations.
     */
    List< AutoPtr<AppWindowToken> > mExitingAppTokens;

    /**
     * List of window tokens that have finished starting their application,
     * and now need to have the policy remove their windows.
     */
    List< AutoPtr<AppWindowToken> > mFinishedStarting;

    /**
     * Fake windows added to the window manager.  Note: ordered from top to
     * bottom, opposite of mWindows.
     */
    List< AutoPtr<FakeWindowImpl> > mFakeWindows;

    /**
     * Windows that are being resized.  Used so we can tell the client about
     * the resize after closing the transaction in which we resized the
     * underlying surface.
     */
    List< AutoPtr<WindowState> > mResizingWindows;

    /**
     * Windows whose animations have ended and now must be removed.
     */
    List< AutoPtr<WindowState> > mPendingRemove;

    /**
     * Used when processing mPendingRemove to avoid working on the original array.
     */
    AutoPtr< ArrayOf<WindowState*> > mPendingRemoveTmp;

    /**
     * Windows whose surface should be destroyed.
     */
    List< AutoPtr<WindowState> > mDestroySurface;

    /**
     * Windows that have lost input focus and are waiting for the new
     * focus window to be displayed before they are told about this.
     */
    List< AutoPtr<WindowState> > mLosingFocus;

    /**
     * This is set when we have run out of memory, and will either be an empty
     * list or contain windows that need to be force removed.
     */
    AutoPtr< List< AutoPtr<WindowState> > > mForceRemoves;

    /**
     * Windows that clients are waiting to have drawn.
     */
    List< Pair<AutoPtr<WindowState>, AutoPtr<IRemoteCallback> > > mWaitingForDrawn;

    /**
     * Windows that have called relayout() while we were running animations,
     * so we need to tell when the animation is done.
     */
    List< AutoPtr<WindowState> > mRelayoutWhileAnimating;

    /**
     * Used when rebuilding window list to keep track of windows that have
     * been removed.
     */
    AutoPtr< ArrayOf<WindowState*> > mRebuildTmp;

    AutoPtr<IIInputMethodManager> mInputMethodManager;

    AutoPtr<ISurfaceSession> mFxSession;
    AutoPtr<Watermark> mWatermark;
    AutoPtr<StrictModeFlash> mStrictModeFlash;

    AutoPtr< ArrayOf<Float> > mTmpFloats;

    Boolean mDisplayReady;
    Boolean mSafeMode;
    Boolean mDisplayEnabled;
    Boolean mSystemBooted;
    Boolean mForceDisplayEnabled;
    Boolean mShowingBootMessages;

    String mLastANRState;

    Int32 mRotation;
    Int32 mForcedAppOrientation;
    Boolean mAltOrientation;
    List< AutoPtr<IRotationWatcher> > mRotationWatchers;
    Int32 mDeferredRotationPauseCount;

    AutoPtr<IRect> mSystemDecorRect;
    Int32 mSystemDecorLayer;
    AutoPtr<IRect> mScreenRect;

    Boolean mTraversalScheduled;
    Boolean mDisplayFrozen;
    Boolean mWaitingForConfig;
    Boolean mWindowsFreezingScreen;
    Boolean mClientFreezingScreen;
    Int32 mAppsFreezingScreen;
    Int32 mLastWindowForcedOrientation;

    Int32 mLayoutSeq;

    Int32 mLastStatusBarVisibility;

    // State while inside of layoutAndPlaceSurfacesLocked().
    Boolean mFocusMayChange;

    AutoPtr<IConfiguration> mCurConfiguration;

    // This is held as long as we have the screen frozen, to give us time to
    // perform a rotation animation when turning off shows the lock screen which
    // changes the orientation.
    AutoPtr<IPowerManagerWakeLock> mScreenFrozenLock;

    // State management of app transitions.  When we are preparing for a
    // transition, mNextAppTransition will be the kind of transition to
    // perform or TRANSIT_NONE if we are not waiting.  If we are waiting,
    // mOpeningApps and mClosingApps are the lists of tokens that will be
    // made visible or hidden at the next transition.
    Int32 mNextAppTransition;
    Int32 mNextAppTransitionType;
    String mNextAppTransitionPackage;
    AutoPtr<IBitmap> mNextAppTransitionThumbnail;
    // Used for thumbnail transitions. True if we're scaling up, false if scaling down
    Boolean mNextAppTransitionScaleUp;
    AutoPtr<IRemoteCallback> mNextAppTransitionCallback;
    Int32 mNextAppTransitionEnter;
    Int32 mNextAppTransitionExit;
    Int32 mNextAppTransitionStartX;
    Int32 mNextAppTransitionStartY;
    Int32 mNextAppTransitionStartWidth;
    Int32 mNextAppTransitionStartHeight;
    Boolean mAppTransitionReady;
    Boolean mAppTransitionRunning;
    Boolean mAppTransitionTimeout;
    Boolean mStartingIconInTransition;
    Boolean mSkipAppTransitionAnimation;
    List< AutoPtr<AppWindowToken> > mOpeningApps;
    List< AutoPtr<AppWindowToken> > mClosingApps;

    Boolean mIsTouchDevice;

    AutoPtr<IDisplayMetrics> mDisplayMetrics;
    AutoPtr<IDisplayMetrics> mRealDisplayMetrics;
    AutoPtr<IDisplayMetrics> mTmpDisplayMetrics;
    AutoPtr<IDisplayMetrics> mCompatDisplayMetrics;

    AutoPtr<H> mH;

    AutoPtr<IChoreographer> mChoreographer;

    AutoPtr<WindowState> mCurrentFocus;
    AutoPtr<WindowState> mLastFocus;

    /** This just indicates the window the input method is on top of, not
     * necessarily the window its input is going to. */
    AutoPtr<WindowState> mInputMethodTarget;

    /** If true hold off on modifying the animation layer of mInputMethodTarget */
    Boolean mInputMethodTargetWaitingAnim;
    Int32 mInputMethodAnimLayerAdjustment;

    AutoPtr<WindowState> mInputMethodWindow;
    List< AutoPtr<WindowState> > mInputMethodDialogs;

    Boolean mHardKeyboardAvailable;
    Boolean mHardKeyboardEnabled;
    AutoPtr<IOnHardKeyboardStatusChangeListener> mHardKeyboardStatusChangeListener;

    List< AutoPtr<WindowToken> > mWallpaperTokens;

    // If non-null, this is the currently visible window that is associated
    // with the wallpaper.
    AutoPtr<WindowState> mWallpaperTarget;
    // If non-null, we are in the middle of animating from one wallpaper target
    // to another, and this is the lower one in Z-order.
    AutoPtr<WindowState> mLowerWallpaperTarget;

    Int32 mWallpaperAnimLayerAdjustment;
    Float mLastWallpaperX;
    Float mLastWallpaperY;
    Float mLastWallpaperXStep;
    Float mLastWallpaperYStep;
    // This is set when we are waiting for a wallpaper to tell us it is done
    // changing its scroll position.
    AutoPtr<WindowState> mWaitingOnWallpaper;
    // The last time we had a timeout when waiting for a wallpaper.
    Int64 mLastWallpaperTimeoutTime;

    AutoPtr<AppWindowToken> mFocusedApp;

    AutoPtr<CPowerManagerService> mPowerManager;

    Float mWindowAnimationScale;
    Float mTransitionAnimationScale;
    Float mAnimatorDurationScale;

    AutoPtr<CInputManagerService> mInputManager;
    AutoPtr<CDisplayManagerService> mDisplayManagerService;
    AutoPtr<IDisplayManager> mDisplayManager;

    // Who is holding the screen on.
    AutoPtr<CSession> mHoldingScreenOn;
    AutoPtr<IPowerManagerWakeLock> mHoldingScreenWakeLock;

    Boolean mTurnOnScreen;

    AutoPtr<DragState> mDragState;

    AutoPtr<LayoutFields> mInnerFields;

    /** Params from WindowManagerService to WindowAnimator. Do not modify or read without first
     * locking on either mWindowMap or mAnimator and then on mLayoutToAnim */
    AutoPtr<LayoutToAnimatorParams> mLayoutToAnim;
    Object mLayoutToAnimLock;

    /** The lowest wallpaper target with a detached wallpaper animation on it. */
    AutoPtr<WindowState> mWindowDetachedWallpaper;

    AutoPtr<WindowAnimator> mAnimator;
    Object mAnimatorLock;

    /**
     * Whether the UI is currently running in touch mode (not showing
     * navigational focus because the user is directly pressing the screen).
     */
    Boolean mInTouchMode;

    AutoPtr<IConfiguration> mTempConfiguration;

    // The desired scaling factor for compatible apps.
    Float mCompatibleScreenScale;

    // If true, only the core apps and services are being launched because the device
    // is in a special boot mode, such as being encrypted or waiting for a decryption password.
    // For example, when this flag is true, there will be no wallpaper service.
    Boolean mOnlyCore;

    AutoPtr<InputMonitor> mInputMonitor;

private:
    // Maximum number of milliseconds to wait for input devices to be enumerated before
    // proceding with safe mode detection.
    static const Int32 INPUT_DEVICES_READY_FOR_SAFE_MODE_DETECTION_TIMEOUT_MILLIS;
    static const String SYSTEM_SECURE;
    static const String SYSTEM_DEBUGGABLE;
    static const Float THUMBNAIL_ANIMATION_DECELERATE_FACTOR;

    AutoPtr<KeyguardDisableHandler> mKeyguardDisableHandler;

    Boolean mHeadless;

    /** All DisplayContents in the world, kept here */
    HashMap<Int32, AutoPtr<DisplayContent> > mDisplayContents;

    // If non-null, we are in the middle of animating from one wallpaper target
    // to another, and this is the higher one in Z-order.
    AutoPtr<WindowState> mUpperWallpaperTarget;

    /** Skip repeated AppWindowTokens initialization. Note that AppWindowsToken's version of this
     * is a long initialized to Long.MIN_VALUE so that it doesn't match this value on startup. */
    Int32 mTransactionSequence;

    /** Only do a maximum of 6 repeated layouts. After that quit */
    Int32 mLayoutRepeatCount;

    // Temp regions for intermediary calculations.
    AutoPtr<IRegion> mTempRegion;

    AutoPtr<ViewServer> mViewServer;
    List< AutoPtr<IWindowChangeListener> > mWindowChangeListeners;
    Boolean mWindowsChanged;
    Boolean mEventDispatchingEnabled;

    Boolean mInLayout;

    AutoPtr<WindowBinder> mWindowBinder;

    friend class CSession;
    friend class WindowAnimator;
    friend class WindowStateAnimator;
    friend class AppWindowToken;
    friend class DragState;
    friend class InputMonitor;
    // friend class InputManagerCallbacks;
};

} // Wm
} // Server
} // Droid
} // Elastos

template <>
struct Conversion<Elastos::Droid::Server::Wm::CWindowManagerService*, IInterface*>
{
    enum { exists = TRUE, exists2Way = FALSE, sameType = FALSE };
};

#endif //__ELASTOS_DROID_SERVER_WM_CWINDOWMANAGERSERVICE_H__
