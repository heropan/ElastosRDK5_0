#ifndef __ELASTOS_DROID_INTERNAL_POLICY_IMPL_PHONEWINDOWMANAGER_H__
#define __ELASTOS_DROID_INTERNAL_POLICY_IMPL_PHONEWINDOWMANAGER_H__

#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/database/ContentObserver.h"

using Elastos::Droid::Os::IHandler;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::View::IInputChannel;
using Elastos::Droid::Os::ILooper;
using Elastos::Droid::View::IInputEvent;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Os::IIBinder;
using Elastos::Droid::Os::IMessage;
using Elastos::Droid::View::IInputEventReceiver;
using Elastos::Droid::Content::IComponentName;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::View::IIWindowManager;
using Elastos::Droid::View::IDisplay;
using Elastos::Droid::Content::Res::IConfiguration;
using Elastos::Droid::View::IView;
using Elastos::Droid::Content::Res::ICompatibilityInfo;
using Elastos::Droid::View::Animation::IAnimation;
using Elastos::Droid::Service::Dreams::IIDreamManager;
using Elastos::Droid::Media::IIAudioService;
using Elastos::Droid::View::IKeyEvent;
using Elastos::Droid::Graphics::IRect;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Io::IPrintWriter;
using Elastos::Droid::App::ISearchManager;

using Elastos::Droid::Database::ContentObserver;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Policy {
namespace Impl {

/**
  * WindowManagerPolicy implementation for the Android phone UI.  This
  * introduces a new method suffix, Lp, for an internal lock of the
  * PhoneWindowManager.  This is used to protect some internal state, and
  * can be acquired with either the Lw and Li lock held, so has the restrictions
  * of both of those when held.
  */
class PhoneWindowManager
    : public Object
    , public IWindowManagerPolicy
    , public IPhoneWindowManager
{
public:
    class SettingsObserver
        : public ContentObserver
    {
    public:
        SettingsObserver(
            /* [in] */ IHandler* handler);

        // @Override public void onChange(boolean selfChange) {
        UpdateSettings();

        UpdateRotation(
            /* [in] */  false);

        virtual CARAPI Observe();
    };

    class MyWakeGestureListener
        : public Object
        , public WakeGestureListener
    {
    public:
        MyWakeGestureListener(
            /* [in] */ IContext* context,
            /* [in] */ IHandler* handler);

        // @Override
        CARAPI OnWakeUp();
    };

    class MyOrientationListener
        : public Object
        , public WindowOrientationListener
    {
    public:
        MyOrientationListener(
            /* [in] */ IContext* context,
            /* [in] */ IHandler* handler);

        // @Override
        CARAPI OnProposedRotationChanged(
            /* [in] */ Int32 rotation);
    };

    /**
      * Input handler used while nav bar is hidden.  Captures any touch on the screen,
      * to determine when the nav bar should be shown and prevent applications from
      * receiving those touches.
      */
    class HideNavInputEventReceiver
        : public Object
        , public IInputEventReceiver
    {
    public:
        HideNavInputEventReceiver(
            /* [in] */ IInputChannel* inputChannel,
            /* [in] */ ILooper* looper);

        // @Override
        CARAPI OnInputEvent(
            /* [in] */ IInputEvent* event);
    };

    class ScreenLockTimeout
        : public Object
        , public Runnable
    {
    public:
        // @Override
        CARAPI Run();

        virtual CARAPI SetLockOptions(
            /* [in] */ IBundle* options);

    public:
        AutoPtr<IBundle> options;
    };

private:
    class InnerRunnable
        : public Object
        , public Runnable
    {
    public:
        InnerRunnable(
            /* [in] */ PhoneWindowManager* owner);

        // @Override
        CARAPI Run();

    private:
        PhoneWindowManager* mOwner;
    };

    class InnerShowListener
        : public Object
        , public IShowListener
    {
    public:
        InnerShowListener(
            /* [in] */ PhoneWindowManager* owner);

        // @Override
        CARAPI OnShown(
            /* [in] */ IIBinder* windowToken);

    private:
        PhoneWindowManager* mOwner;
    };

    class PolicyHandler
        : public Object
        , public IHandler
    {
    public:
        // @Override
        CARAPI HandleMessage(
            /* [in] */ IMessage* msg);
    };

    class InnerUEventObserver
        : public Object
        , public IUEventObserver
    {
    public:
        InnerUEventObserver(
            /* [in] */ PhoneWindowManager* owner);

        // @Override
        CARAPI OnUEvent(
            /* [in] */  UEventObserver);

    private:
        PhoneWindowManager* mOwner;
    };

    class InnerRunnable1
        : public Object
        , public Runnable
    {
    public:
        InnerRunnable1(
            /* [in] */ PhoneWindowManager* owner);

        // @Override
        CARAPI Run();

    private:
        PhoneWindowManager* mOwner;
    };

    class InnerRunnable2
        : public Object
        , public Runnable
    {
    public:
        InnerRunnable2(
            /* [in] */ PhoneWindowManager* owner);

        // @Override
        CARAPI Run();

    private:
        PhoneWindowManager* mOwner;
    };

    class InnerRunnable3
        : public Object
        , public Runnable
    {
    public:
        InnerRunnable3(
            /* [in] */ PhoneWindowManager* owner);

        // @Override
        CARAPI Run();

    private:
        PhoneWindowManager* mOwner;
    };

    class InnerSystemGesturesPointerEventListenerCallbacks1
        : public Object
        , public SystemGesturesPointerEventListener::Callbacks
    {
    public:
        InnerSystemGesturesPointerEventListenerCallbacks1(
            /* [in] */ PhoneWindowManager* owner);

        // @Override
        CARAPI OnSwipeFromTop();

        // @Override
        CARAPI OnSwipeFromBottom();

        // @Override
        CARAPI OnSwipeFromRight();

        // @Override
        CARAPI OnDebug();

    private:
        PhoneWindowManager* mOwner;
    };

    class InnerOnKeyguardExitResult1
        : public Object
        , public OnKeyguardExitResult
    {
    public:
        InnerOnKeyguardExitResult1(
            /* [in] */ PhoneWindowManager* owner);

        // @Override
        CARAPI OnKeyguardExitResult(
            /* [in] */ Boolean success);

    private:
        PhoneWindowManager* mOwner;
    };

    class InnerRunnable4
        : public Object
        , public Runnable
    {
    public:
        InnerRunnable4(
            /* [in] */ PhoneWindowManager* owner);

        // @Override
        CARAPI Run();

    private:
        PhoneWindowManager* mOwner;
    };

    class InnerInputEventReceiverFactory
        : public Object
        , public InputEventReceiver::Factory
    {
    public:
        InnerInputEventReceiverFactory(
            /* [in] */ PhoneWindowManager* owner);

        // @Override
        CARAPI_(AutoPtr<IInputEventReceiver>) CreateInputEventReceiver(
            /* [in] */ IInputChannel* inputChannel,
            /* [in] */ ILooper* looper);

    private:
        PhoneWindowManager* mOwner;
    };

    class InnerRunnable6
        : public Object
        , public Runnable
    {
    public:
        InnerRunnable6(
            /* [in] */ PhoneWindowManager* owner);

        // @Override
        CARAPI Run();

    private:
        PhoneWindowManager* mOwner;
    };

    class InnerRunnable8
        : public Object
        , public Runnable
    {
    public:
        InnerRunnable8(
            /* [in] */ PhoneWindowManager* owner);

        // @Override
        CARAPI Run();

    private:
        PhoneWindowManager* mOwner;
    };

    class InnerRunnable9
        : public Object
        , public Runnable
    {
    public:
        InnerRunnable9(
            /* [in] */ PhoneWindowManager* owner);

        // @Override void run() {
        // synchronized

            /* [in] */  mScreenshotLock);

    private:
        AutoPtr<}

 PhoneWindowManager*> mOwner;
    };

    class InnerServiceConnection1
        : public Object
        , public IServiceConnection
    {
    private:
        class InnerHandler
            : public Object
            , public IHandler
        {
        public:
            InnerHandler(
                /* [in] */ InnerServiceConnection1* owner);

            // @Override
            CARAPI HandleMessage(
                /* [in] */ IMessage* msg);

        private:
            InnerServiceConnection1* mOwner;
        };

    public:
        InnerServiceConnection1(
            /* [in] */ PhoneWindowManager* owner);

        // @Override
        CARAPI OnServiceConnected(
            /* [in] */ IComponentName* name,
            /* [in] */ IIBinder* service);

        // @Override
        CARAPI OnServiceDisconnected(
            /* [in] */ IComponentName* name);

    private:
        PhoneWindowManager* mOwner;
    };

    class InnerBroadcastReceiver
        : public Object
        , public IBroadcastReceiver
    {
    public:
        InnerBroadcastReceiver(
            /* [in] */ PhoneWindowManager* owner);

        // @Override
        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

    private:
        PhoneWindowManager* mOwner;
    };

    class InnerBroadcastReceiver1
        : public Object
        , public IBroadcastReceiver
    {
    public:
        InnerBroadcastReceiver1(
            /* [in] */ PhoneWindowManager* owner);

        // @Override
        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

    private:
        PhoneWindowManager* mOwner;
    };

    class InnerBroadcastReceiver2
        : public Object
        , public IBroadcastReceiver
    {
    public:
        InnerBroadcastReceiver2(
            /* [in] */ PhoneWindowManager* owner);

        // @Override
        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

    private:
        PhoneWindowManager* mOwner;
    };

    class InnerRunnable10
        : public Object
        , public Runnable
    {
    public:
        InnerRunnable10(
            /* [in] */ PhoneWindowManager* owner);

        // @Override
        CARAPI Run();

    private:
        PhoneWindowManager* mOwner;
    };

    class InnerRunnable12
        : public Object
        , public Runnable
    {
    public:
        InnerRunnable12(
            /* [in] */ PhoneWindowManager* owner);

        // @Override
        CARAPI Run();

    private:
        PhoneWindowManager* mOwner;
    };

    class InnerRunnable14
        : public Object
        , public Runnable
    {
    public:
        InnerRunnable14(
            /* [in] */ PhoneWindowManager* owner);

        // @Override
        CARAPI Run();

    private:
        PhoneWindowManager* mOwner;
    };

    class InnerRunnable16
        : public Object
        , public Runnable
    {
    public:
        InnerRunnable16(
            /* [in] */ PhoneWindowManager* owner);

        // @Override
        CARAPI Run();

    private:
        PhoneWindowManager* mOwner;
    };

    class InnerRunnable18
        : public Object
        , public Runnable
    {
    public:
        InnerRunnable18(
            /* [in] */ PhoneWindowManager* owner);

        // @Override public void run() {
        MBootMsgDialog.setMessage(
            /* [in] */  msg);

    private:
        AutoPtr<}

 PhoneWindowManager*> mOwner;
    };

    class InnerRunnable20
        : public Object
        , public Runnable
    {
    public:
        InnerRunnable20(
            /* [in] */ PhoneWindowManager* owner);

        // @Override
        CARAPI Run();

    private:
        PhoneWindowManager* mOwner;
    };

public:
    CAR_INTERFACE_DECL();

    PhoneWindowManager();

    CARAPI constructor();

    virtual CARAPI GetStatusBarService(
        /* [out] */ IStatusBarService** result);

    /*
      * We always let the sensor be switched on by default except when
      * the user has explicitly disabled sensor based rotation or when the
      * screen is switched off.
      */
    virtual CARAPI NeedSensorRunningLp(
        /* [out] */ Boolean* result);

    /*
      * Various use cases for invoking this function
      * screen turning off, should always disable listeners if already enabled
      * screen turned on and current app has sensor based orientation, enable listeners
      * if not already enabled
      * screen turned on and current app does not have sensor orientation, disable listeners if
      * already enabled
      * screen turning on and current app has sensor based orientation, enable listeners if needed
      * screen turning on and current app has nosensor based orientation, do nothing
      */
    virtual CARAPI UpdateOrientationListenerLp();

    // @Override
    CARAPI ShowGlobalActions();

    virtual CARAPI ShowGlobalActionsInternal();

    virtual CARAPI IsDeviceProvisioned(
        /* [out] */ Boolean* result);

    virtual CARAPI IsUserSetupComplete(
        /* [out] */ Boolean* result);

    /** {@inheritDoc} */
    // @Override
    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IIWindowManager* windowManager,
        /* [in] */ WindowManagerFuncs* windowManagerFuncs);

    // @Override
    CARAPI SetInitialDisplaySize(
        /* [in] */ IDisplay* display,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int32 density);

    // @Override
    CARAPI_(Boolean) IsDefaultOrientationForced();

    // @Override
    CARAPI SetDisplayOverscan(
        /* [in] */ IDisplay* display,
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom);

    virtual CARAPI UpdateSettings();

    /** {@inheritDoc} */
    // @Override
    CARAPI_(Int32) CheckAddPermission(
        /* [in] */ IWindowManager* ::LayoutParams* attrs,
        /* [in] */ ArrayOf<Int32>* outAppOp);

    // @Override
    CARAPI_(Boolean) CheckShowToOwnerOnly(
        /* [in] */  WindowManager);

    // @Override
    CARAPI AdjustWindowParamsLw(
        /* [in] */  WindowManager);

    virtual CARAPI ReadLidState();

    /** {@inheritDoc} */
    // @Override
    CARAPI AdjustConfigurationLw(
        /* [in] */ IConfiguration* config,
        /* [in] */ Int32 keyboardPresence,
        /* [in] */ Int32 navigationPresence);

    /** {@inheritDoc} */
    // @Override
    CARAPI_(Int32) WindowTypeToLayerLw(
        /* [in] */ Int32 type);

    /** {@inheritDoc} */
    // @Override
    CARAPI_(Int32) SubWindowTypeToLayerLw(
        /* [in] */ Int32 type);

    // @Override
    CARAPI_(Int32) GetMaxWallpaperLayer();

    // @Override
    CARAPI_(Int32) GetAboveUniverseLayer();

    // @Override
    CARAPI_(Int32) GetNonDecorDisplayWidth(
        /* [in] */ Int32 fullWidth,
        /* [in] */ Int32 fullHeight,
        /* [in] */ Int32 rotation);

    // @Override
    CARAPI_(Int32) GetNonDecorDisplayHeight(
        /* [in] */ Int32 fullWidth,
        /* [in] */ Int32 fullHeight,
        /* [in] */ Int32 rotation);

    // @Override
    CARAPI_(Int32) GetConfigDisplayWidth(
        /* [in] */ Int32 fullWidth,
        /* [in] */ Int32 fullHeight,
        /* [in] */ Int32 rotation);

    // @Override
    CARAPI_(Int32) GetConfigDisplayHeight(
        /* [in] */ Int32 fullWidth,
        /* [in] */ Int32 fullHeight,
        /* [in] */ Int32 rotation);

    // @Override
    CARAPI_(Boolean) IsForceHiding(
        /* [in] */  WindowManager);

    // @Override
    CARAPI_(Boolean) IsKeyguardHostWindow(
        /* [in] */  WindowManager);

    // @Override
    CARAPI_(Boolean) CanBeForceHidden(
        /* [in] */ WindowState* win,
        /* [in] */  WindowManager);

    // @Override
    CARAPI_(AutoPtr<WindowState>) GetWinShowWhenLockedLw();

    /** {@inheritDoc} */
    // @Override
    CARAPI_(AutoPtr<IView>) AddStartingWindow(
        /* [in] */ IIBinder* appToken,
        /* [in] */ String packageName,
        /* [in] */ Int32 theme,
        /* [in] */ ICompatibilityInfo* compatInfo,
        /* [in] */ CharSequence* nonLocalizedLabel,
        /* [in] */ Int32 labelRes,
        /* [in] */ Int32 icon,
        /* [in] */ Int32 logo,
        /* [in] */ Int32 windowFlags);

    /** {@inheritDoc} */
    // @Override
    CARAPI RemoveStartingWindow(
        /* [in] */ IIBinder* appToken,
        /* [in] */ IView* window);

    /**
      * Preflight adding a window to the system.
      *
      * Currently enforces that three window types are singletons:
      * <ul>
      * <li>STATUS_BAR_TYPE</li>
      * <li>KEYGUARD_TYPE</li>
      * </ul>
      *
      * @param win The window to be added
      * @param attrs Information about the window to be added
      *
      * @return If ok, WindowManagerImpl.ADD_OKAY.  If too many singletons,
      * WindowManagerImpl.ADD_MULTIPLE_SINGLETON
      */
    // @Override
    CARAPI_(Int32) PrepareAddWindowLw(
        /* [in] */ WindowState* win,
        /* [in] */  WindowManager);

    /** {@inheritDoc} */
    // @Override
    CARAPI RemoveWindowLw(
        /* [in] */ WindowState* win);

    /** {@inheritDoc} */
    // @Override
    CARAPI_(Int32) SelectAnimationLw(
        /* [in] */ WindowState* win,
        /* [in] */ Int32 transit);

    // @Override
    CARAPI SelectRotationAnimationLw(
        /* [in] */ Int32 anim[]);

    // @Override
    CARAPI_(Boolean) ValidateRotationAnimationLw(
        /* [in] */ Int32 exitAnimId,
        /* [in] */ Int32 enterAnimId,
        /* [in] */ Boolean forceDefault);

    // @Override
    CARAPI_(AutoPtr<IAnimation>) CreateForceHideEnterAnimation(
        /* [in] */ Boolean onWallpaper,
        /* [in] */ Boolean goingToNotificationShade);

    // @Override
    CARAPI_(AutoPtr<IAnimation>) CreateForceHideWallpaperExitAnimation(
        /* [in] */ Boolean goingToNotificationShade);

    static CARAPI_(AutoPtr<IIDreamManager>) GetDreamManager();

    virtual CARAPI GetTelecommService(
        /* [out] */ TelecomManager** result);

    static CARAPI_(AutoPtr<IIAudioService>) GetAudioService();

    virtual CARAPI KeyguardOn(
        /* [out] */ Boolean* result);

    /** {@inheritDoc} */
    // @Override
    CARAPI_(Int64) InterceptKeyBeforeDispatching(
        /* [in] */ WindowState* win,
        /* [in] */ IKeyEvent* event,
        /* [in] */ Int32 policyFlags);

    /** {@inheritDoc} */
    // @Override
    CARAPI_(AutoPtr<IKeyEvent>) DispatchUnhandledKey(
        /* [in] */ WindowState* win,
        /* [in] */ IKeyEvent* event,
        /* [in] */ Int32 policyFlags);

    // @Override
    CARAPI ShowRecentApps();

    /**
      * A home key -> launch home action was detected.  Take the appropriate action
      * given the situation with the keyguard.
      */
    virtual CARAPI LaunchHomeFromHotKey();

    // @Override
    CARAPI_(Int32) AdjustSystemUiVisibilityLw(
        /* [in] */ Int32 visibility);

    // @Override
    CARAPI GetContentInsetHintLw(
        /* [in] */ IWindowManager* ::LayoutParams* attrs,
        /* [in] */ IRect* contentInset);

    /** {@inheritDoc} */
    // @Override
    CARAPI BeginLayoutLw(
        /* [in] */ Boolean isDefaultDisplay,
        /* [in] */ Int32 displayWidth,
        /* [in] */ Int32 displayHeight,
        /* [in] */ Int32 displayRotation);

    /** {@inheritDoc} */
    // @Override
    CARAPI_(Int32) GetSystemDecorLayerLw();

    // @Override
    CARAPI GetContentRectLw(
        /* [in] */ IRect* r);

    virtual CARAPI SetAttachedWindowFrames(
        /* [in] */ WindowState* win,
        /* [in] */ Int32 fl,
        /* [in] */ Int32 adjust,
        /* [in] */ WindowState* attached,
        /* [in] */ Boolean insetDecors,
        /* [in] */ IRect* pf,
        /* [in] */ IRect* df,
        /* [in] */ IRect* of,
        /* [in] */ IRect* cf,
        /* [in] */ IRect* vf);

    /** {@inheritDoc} */
    // @Override
    CARAPI LayoutWindowLw(
        /* [in] */ WindowState* win,
        /* [in] */ WindowState* attached);

    /** {@inheritDoc} */
    // @Override
    CARAPI FinishLayoutLw();

    /** {@inheritDoc} */
    // @Override
    CARAPI BeginPostLayoutPolicyLw(
        /* [in] */ Int32 displayWidth,
        /* [in] */ Int32 displayHeight);

    /** {@inheritDoc} */
    // @Override
    CARAPI ApplyPostLayoutPolicyLw(
        /* [in] */ WindowState* win,
        /* [in] */  WindowManager);

    /** {@inheritDoc} */
    // @Override
    CARAPI_(Int32) FinishPostLayoutPolicyLw();

    // @Override
    CARAPI_(Boolean) AllowAppAnimationsLw();

    // @Override
    CARAPI_(Int32) FocusChangedLw(
        /* [in] */ WindowState* lastFocus,
        /* [in] */ WindowState* newFocus);

    /** {@inheritDoc} */
    // @Override
    CARAPI NotifyLidSwitchChanged(
        /* [in] */ Int64 whenNanos,
        /* [in] */ Boolean lidOpen);

    // @Override
    CARAPI NotifyCameraLensCoverSwitchChanged(
        /* [in] */ Int64 whenNanos,
        /* [in] */ Boolean lensCovered);

    virtual CARAPI SetHdmiPlugged(
        /* [in] */ Boolean plugged);

    virtual CARAPI InitializeHdmiState();

    /** {@inheritDoc} */
    // @Override
    CARAPI_(Int32) InterceptKeyBeforeQueueing(
        /* [in] */ IKeyEvent* event,
        /* [in] */ Int32 policyFlags);

    /** {@inheritDoc} */
    // @Override
    CARAPI_(Int32) InterceptMotionBeforeQueueingNonInteractive(
        /* [in] */ Int64 whenNanos,
        /* [in] */ Int32 policyFlags);

    virtual CARAPI DispatchMediaKeyWithWakeLock(
        /* [in] */ IKeyEvent* event);

    virtual CARAPI DispatchMediaKeyRepeatWithWakeLock(
        /* [in] */ IKeyEvent* event);

    virtual CARAPI DispatchMediaKeyWithWakeLockToAudioService(
        /* [in] */ IKeyEvent* event);

    virtual CARAPI LaunchVoiceAssistWithWakeLock(
        /* [in] */ Boolean keyguardActive);

    // Called on the PowerManager's Notifier thread.
    // @Override
    CARAPI GoingToSleep(
        /* [in] */ Int32 why);

    // Called on the PowerManager's Notifier thread.
    // @Override
    CARAPI WakingUp();

    // Called on the DisplayManager's DisplayPowerController thread.
    // @Override
    CARAPI ScreenTurnedOff();

    // Called on the DisplayManager's DisplayPowerController thread.
    // @Override
    CARAPI ScreenTurningOn(
        /* [in] */ ScreenOnListener* screenOnListener);

    // @Override
    CARAPI_(Boolean) IsScreenOn();

    /** {@inheritDoc} */
    // @Override
    CARAPI EnableKeyguard(
        /* [in] */ Boolean enabled);

    /** {@inheritDoc} */
    // @Override
    CARAPI ExitKeyguardSecurely(
        /* [in] */ OnKeyguardExitResult* callback);

    /** {@inheritDoc} */
    // @Override
    CARAPI_(Boolean) IsKeyguardLocked();

    /** {@inheritDoc} */
    // @Override
    CARAPI_(Boolean) IsKeyguardSecure();

    /** {@inheritDoc} */
    // @Override
    CARAPI_(Boolean) InKeyguardRestrictedKeyInputMode();

    // @Override
    CARAPI DismissKeyguardLw();

    virtual CARAPI NotifyActivityDrawnForKeyguardLw();

    // @Override
    CARAPI_(Boolean) IsKeyguardDrawnLw();

    // @Override
    CARAPI StartKeyguardExitAnimation(
        /* [in] */ Int64 startTime,
        /* [in] */ Int64 fadeoutDuration);

    virtual CARAPI SendCloseSystemWindows();

    virtual CARAPI SendCloseSystemWindows(
        /* [in] */ String reason);

    static CARAPI SendCloseSystemWindows(
        /* [in] */ IContext* context,
        /* [in] */ String reason);

    // @Override
    CARAPI_(Int32) RotationForOrientationLw(
        /* [in] */ Int32 orientation,
        /* [in] */ Int32 lastRotation);

    // @Override
    CARAPI_(Boolean) RotationHasCompatibleMetricsLw(
        /* [in] */ Int32 orientation,
        /* [in] */ Int32 rotation);

    // @Override
    CARAPI SetRotationLw(
        /* [in] */ Int32 rotation);

    // @Override
    CARAPI_(Int32) GetUserRotationMode();

    // User rotation: to be used when all else fails in assigning an orientation to the device
    // @Override
    CARAPI SetUserRotationMode(
        /* [in] */ Int32 mode,
        /* [in] */ Int32 rot);

    // @Override
    CARAPI SetSafeMode(
        /* [in] */ Boolean safeMode);

    static CARAPI_(AutoPtr< ArrayOf<Int64> >) GetLongIntArray(
        /* [in] */ IResources* r,
        /* [in] */ Int32 resid);

    /** {@inheritDoc} */
    // @Override
    CARAPI SystemReady();

    /** {@inheritDoc} */
    // @Override
    CARAPI SystemBooted();

    /** {@inheritDoc} */
    // @Override
    CARAPI ShowBootMessage(
        /* [in] */ CharSequence* msg,
        /* [in] */ Boolean always);

    /** {@inheritDoc} */
    // @Override
    CARAPI HideBootMessages();

    /** {@inheritDoc} */
    // @Override
    CARAPI UserActivity();

    // @Override
    CARAPI LockNow(
        /* [in] */ IBundle* options);

    /** {@inheritDoc} */
    // @Override
    CARAPI EnableScreenAfterBoot();

    virtual CARAPI UpdateUiMode();

    virtual CARAPI UpdateRotation(
        /* [in] */ Boolean alwaysSendConfiguration);

    virtual CARAPI UpdateRotation(
        /* [in] */ Boolean alwaysSendConfiguration,
        /* [in] */ Boolean forceRelayout);

    /**
      * Return an Intent to launch the currently active dock app as home.  Returns
      * null if the standard home should be launched, which is the case if any of the following is
      * true:
      * <ul>
      *  <li>The device is not in either car mode or desk mode
      *  <li>The device is in car mode but ENABLE_CAR_DOCK_HOME_CAPTURE is false
      *  <li>The device is in desk mode but ENABLE_DESK_DOCK_HOME_CAPTURE is false
      *  <li>The device is in car mode but there's no CAR_DOCK app with METADATA_DOCK_HOME
      *  <li>The device is in desk mode but there's no DESK_DOCK app with METADATA_DOCK_HOME
      * </ul>
      * @return A dock intent.
      */
    virtual CARAPI CreateHomeDockIntent(
        /* [out] */ Intent** result);

    virtual CARAPI StartDockOrHome();

    /**
      * goes to the home screen
      * @return whether it did anything
      */
    virtual CARAPI GoHome(
        /* [out] */ Boolean* result);

    // @Override
    CARAPI SetCurrentOrientationLw(
        /* [in] */ Int32 newOrientation);

    // @Override
    CARAPI_(Boolean) PerformHapticFeedbackLw(
        /* [in] */ WindowState* win,
        /* [in] */ Int32 effectId,
        /* [in] */ Boolean always);

    // @Override
    CARAPI KeepScreenOnStartedLw();

    // @Override
    CARAPI KeepScreenOnStoppedLw();

    // Use this instead of checking config_showNavigationBar so that it can be consistently
    // overridden by qemu.hw.mainkeys in the emulator.
    // @Override
    CARAPI_(Boolean) HasNavigationBar();

    // @Override
    CARAPI SetLastInputMethodWindowLw(
        /* [in] */ WindowState* ime,
        /* [in] */ WindowState* target);

    // @Override
    CARAPI_(Int32) GetInputMethodWindowVisibleHeightLw();

    // @Override
    CARAPI SetCurrentUserLw(
        /* [in] */ Int32 newUserId);

    // @Override
    CARAPI_(Boolean) CanMagnifyWindow(
        /* [in] */ Int32 windowType);

    // @Override
    CARAPI_(Boolean) IsTopLevelWindow(
        /* [in] */ Int32 windowType);

    // @Override
    CARAPI Dump(
        /* [in] */ String prefix,
        /* [in] */ IPrintWriter* pw,
        /* [in] */ ArrayOf<String>* args);

private:
    CARAPI_(AutoPtr< ArrayOf<Int32> >) MiddleInitMnavigationbarheightforrotation();

    CARAPI_(AutoPtr< ArrayOf<Int32> >) MiddleInitMnavigationbarwidthforrotation();

    CARAPI_(void) InterceptPowerKeyDown(
        /* [in] */ Boolean handled);

    CARAPI_(Boolean) InterceptPowerKeyUp(
        /* [in] */ Boolean canceled);

    CARAPI_(void) CancelPendingPowerKeyAction();

    CARAPI_(void) InterceptScreenshotChord();

    CARAPI_(Int64) GetScreenshotChordLongPressDelay();

    CARAPI_(void) CancelPendingScreenshotChordAction();

    CARAPI_(void) PowerShortPress(
        /* [in] */ Int64 eventTime);

    CARAPI_(void) HandleLongPressOnHome();

    CARAPI_(void) HandleDoubleTapOnHome();

    /**
      * Read values from config.xml that may be overridden depending on
      * the configuration of the device.
      * eg. Disable long press on home goes to recents on sw600dp.
      */
    CARAPI_(void) ReadConfigurationDependentBehaviors();

    /**
      * @return whether the navigation bar can be hidden, e.g. the device has a
      *         navigation bar and touch exploration is not enabled
      */
    CARAPI_(Boolean) CanHideNavigationBar();

    CARAPI_(void) UpdateWakeGestureListenerLp();

    CARAPI_(Boolean) ShouldEnableWakeGestureLp();

    CARAPI_(void) EnablePointerLocation();

    CARAPI_(void) DisablePointerLocation();

    CARAPI_(Int32) ReadRotation(
        /* [in] */ Int32 resID);

    CARAPI_(void) ReadCameraLensCoverState();

    CARAPI_(Boolean) IsHidden(
        /* [in] */ Int32 accessibilityMode);

    static CARAPI_(void) AwakenDreams();

    static CARAPI_(AutoPtr< ArrayOf<Int32> >) MiddleInitWindowTypesWhereHomeDoesntWork();

    CARAPI_(Boolean) InterceptFallback(
        /* [in] */ WindowState* win,
        /* [in] */ IKeyEvent* fallbackEvent,
        /* [in] */ Int32 policyFlags);

    CARAPI_(void) LaunchAssistLongPressAction();

    CARAPI_(void) LaunchAssistAction();

    CARAPI_(void) LaunchAssistAction(
        /* [in] */ String hint);

    CARAPI_(AutoPtr<ISearchManager>) GetSearchManager();

    CARAPI_(void) PreloadRecentApps();

    CARAPI_(void) CancelPreloadRecentApps();

    CARAPI_(void) ToggleRecentApps();

    CARAPI_(void) ShowRecentApps(
        /* [in] */ Boolean triggeredFromAltTab);

    CARAPI_(void) HideRecentApps(
        /* [in] */ Boolean triggeredFromAltTab,
        /* [in] */ Boolean triggeredFromHome);

    CARAPI_(void) ApplyStableConstraints(
        /* [in] */ Int32 sysui,
        /* [in] */ Int32 fl,
        /* [in] */ IRect* r);

    CARAPI_(void) OffsetInputMethodWindowLw(
        /* [in] */ WindowState* win);

    CARAPI_(void) OffsetVoiceInputWindowLw(
        /* [in] */ WindowState* win);

    /**
      * Processes the result code of {@link IKeyguardService#setOccluded}. This is needed because we
      * immediately need to put the wallpaper directly behind the Keyguard when a window with flag
      * {@link android.view.WindowManager.LayoutParams#FLAG_SHOW_WHEN_LOCKED} gets dismissed. If we
      * would wait for Keyguard to change the flags, that would be running asynchronously and thus be
      * too late so the user might see the window behind.
      *
      * @param setHiddenResult The result code from {@link IKeyguardService#setOccluded}.
      * @return Whether the flags have changed and we have to redo the layout.
      */
    CARAPI_(Boolean) ProcessKeyguardSetHiddenResultLw(
        /* [in] */ Int32 setHiddenResult);

    CARAPI_(Boolean) IsStatusBarKeyguard();

    // Assume this is called from the Handler thread.
    CARAPI_(void) TakeScreenshot();

    /**
      * When the screen is off we ignore some keys that might otherwise typically
      * be considered wake keys.  We filter them out here.
      *
      * {@link KeyEvent#KEYCODE_POWER} is notably absent from this list because it
      * is always considered a wake key.
      */
    CARAPI_(Boolean) IsWakeKeyWhenScreenOff(
        /* [in] */ Int32 keyCode);

    CARAPI_(Boolean) ShouldDispatchInputWhenNonInteractive();

    CARAPI_(void) RequestTransientBars(
        /* [in] */ WindowState* swipeTarget);

    CARAPI_(void) FinishKeyguardDrawn();

    CARAPI_(void) FinishWindowsDrawn();

    CARAPI_(void) FinishScreenTurningOn();

    CARAPI_(void) HandleHideBootMessage();

    CARAPI_(Boolean) KeyguardIsShowingTq();

    // Returns true if keyguard is currently locked whether or not it is currently hidden.
    CARAPI_(Boolean) IsKeyguardSecureIncludingHidden();

    CARAPI_(Boolean) IsLandscapeOrSeascape(
        /* [in] */ Int32 rotation);

    CARAPI_(Boolean) IsAnyPortrait(
        /* [in] */ Int32 rotation);

    CARAPI_(void) UpdateLockScreenTimeout();

    CARAPI_(void) ApplyLidSwitchState();

    CARAPI_(void) PerformAuditoryFeedbackForAccessibilityIfNeed();

    CARAPI_(Boolean) IsGlobalAccessibilityGestureEnabled();

    CARAPI_(Int32) UpdateSystemUiVisibilityLw();

    CARAPI_(Int32) UpdateSystemBarsLw(
        /* [in] */ WindowState* win,
        /* [in] */ Int32 oldVis,
        /* [in] */ Int32 vis);

    CARAPI_(void) ClearClearableFlagsLw();

    CARAPI_(Boolean) IsImmersiveMode(
        /* [in] */ Int32 vis);

    /**
      * @return whether the navigation or status bar can be made translucent
      *
      * This should return true unless touch exploration is not enabled or
      * R.boolean.config_enableTranslucentDecor is false.
      */
    CARAPI_(Boolean) AreTranslucentBarsAllowed();

public:
    static const String TAG;
    static const Boolean DEBUG;
    static const Boolean localLOGV;
    static const Boolean DEBUG_LAYOUT;
    static const Boolean DEBUG_INPUT;
    static const Boolean DEBUG_STARTING_WINDOW;
    static const Boolean DEBUG_WAKEUP;
    static const Boolean SHOW_STARTING_ANIMATIONS;
    static const Boolean SHOW_PROCESSES_ON_ALT_MENU;
    // Whether to allow dock apps with METADATA_DOCK_HOME to temporarily take over the Home key.
    // No longer recommended for desk docks; still useful in car docks.
    static const Boolean ENABLE_CAR_DOCK_HOME_CAPTURE;
    static const Boolean ENABLE_DESK_DOCK_HOME_CAPTURE;
    static const Int32 SHORT_PRESS_POWER_NOTHING = 0;
    static const Int32 SHORT_PRESS_POWER_GO_TO_SLEEP = 1;
    static const Int32 SHORT_PRESS_POWER_REALLY_GO_TO_SLEEP = 2;
    static const Int32 SHORT_PRESS_POWER_REALLY_GO_TO_SLEEP_AND_GO_HOME = 3;
    static const Int32 LONG_PRESS_POWER_NOTHING = 0;
    static const Int32 LONG_PRESS_POWER_GLOBAL_ACTIONS = 1;
    static const Int32 LONG_PRESS_POWER_SHUT_OFF = 2;
    static const Int32 LONG_PRESS_POWER_SHUT_OFF_NO_CONFIRM = 3;
    // These need to match the documentation/constant in
    // core/res/res/values/config.xml
    static const Int32 LONG_PRESS_HOME_NOTHING = 0;
    static const Int32 LONG_PRESS_HOME_RECENT_SYSTEM_UI = 1;
    static const Int32 LONG_PRESS_HOME_ASSIST = 2;
    static const Int32 DOUBLE_TAP_HOME_NOTHING = 0;
    static const Int32 DOUBLE_TAP_HOME_RECENT_SYSTEM_UI = 1;
    static const Int32 APPLICATION_MEDIA_SUBLAYER = -2;
    static const Int32 APPLICATION_MEDIA_OVERLAY_SUBLAYER = -1;
    static const Int32 APPLICATION_PANEL_SUBLAYER = 1;
    static const Int32 APPLICATION_SUB_PANEL_SUBLAYER = 2;
    static const String SYSTEM_DIALOG_REASON_KEY;
    static const String SYSTEM_DIALOG_REASON_GLOBAL_ACTIONS;
    static const String SYSTEM_DIALOG_REASON_RECENT_APPS;
    static const String SYSTEM_DIALOG_REASON_HOME_KEY;
    static const String SYSTEM_DIALOG_REASON_ASSIST;
    /**
      * These are the system UI flags that, when changing, can cause the layout
      * of the screen to change.
      */
    static const Int32 SYSTEM_UI_CHANGING_LAYOUT = View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
             | View.SYSTEM_UI_FLAG_FULLSCREEN
             | View.STATUS_BAR_TRANSLUCENT
             | View.NAVIGATION_BAR_TRANSLUCENT
             | View.SYSTEM_UI_TRANSPARENT;
    /* Table of Application Launch keys.  Maps from key codes to intent categories.
      *
      * These are special keys that are used to launch particular kinds of applications,
      * such as a web browser.  HID defines nearly a hundred of them in the Consumer (0x0C)
      * usage page.  We don't support quite that many yet...
      */
    static AutoPtr< ISparseArray<String> > sApplicationLaunchKeyCategories;
    /** Amount of time (in milliseconds) to wait for windows drawn before powering on. */
    static const Int32 WAITING_FOR_DRAWN_TIMEOUT = 1000;
    AutoPtr<IContext> mContext;
    AutoPtr<IIWindowManager> mWindowManager;
    AutoPtr<WindowManagerFuncs> mWindowManagerFuncs;
    AutoPtr<IWindowManagerInternal> mWindowManagerInternal;
    AutoPtr<IPowerManager> mPowerManager;
    AutoPtr<IDreamManagerInternal> mDreamManagerInternal;
    AutoPtr<IIStatusBarService> mStatusBarService;
    Boolean mPreloadedRecentApps;
    /*const*/ AutoPtr<Object> mServiceAquireLock;
    AutoPtr<IVibrator> mVibrator;
    // Vibrator for giving feedback of orientation changes
    AutoPtr<ISearchManager> mSearchManager;
    AutoPtr<IAccessibilityManager> mAccessibilityManager;
    // Vibrator pattern for haptic feedback of a long press.
    AutoPtr< ArrayOf<Int64> > mLongPressVibePattern;
    // Vibrator pattern for haptic feedback of virtual key press.
    AutoPtr< ArrayOf<Int64> > mVirtualKeyVibePattern;
    // Vibrator pattern for a short vibration.
    AutoPtr< ArrayOf<Int64> > mKeyboardTapVibePattern;
    // Vibrator pattern for a short vibration when tapping on an hour/minute tick of a Clock.
    AutoPtr< ArrayOf<Int64> > mClockTickVibePattern;
    // Vibrator pattern for a short vibration when tapping on a day/month/year date of a Calendar.
    AutoPtr< ArrayOf<Int64> > mCalendarDateVibePattern;
    // Vibrator pattern for haptic feedback during boot when safe mode is disabled.
    AutoPtr< ArrayOf<Int64> > mSafeModeDisabledVibePattern;
    // Vibrator pattern for haptic feedback during boot when safe mode is enabled.
    AutoPtr< ArrayOf<Int64> > mSafeModeEnabledVibePattern;
    /** If true, hitting shift & menu will broadcast Intent.ACTION_BUG_REPORT */
    Boolean mEnableShiftMenuBugReports;
    Boolean mSafeMode;
    AutoPtr<WindowState> mStatusBar;
    Int32 mStatusBarHeight;
    AutoPtr<WindowState> mNavigationBar;
    Boolean mHasNavigationBar;
    Boolean mCanHideNavigationBar;
    Boolean mNavigationBarCanMove;
    // can the navigation bar ever move to the side?
    Boolean mNavigationBarOnBottom;
    // is the navigation bar on the bottom *right now*?
    AutoPtr< ArrayOf<Int32> > mNavigationBarHeightForRotation;
    AutoPtr< ArrayOf<Int32> > mNavigationBarWidthForRotation;
    Boolean mBootMessageNeedsHiding;
    AutoPtr<IKeyguardServiceDelegate> mKeyguardDelegate;
    /*const*/ AutoPtr<Runnable> mWindowManagerDrawCallback;
    /*const*/ AutoPtr<IShowListener> mKeyguardDelegateCallback;
    AutoPtr<GlobalActions> mGlobalActions;
    AutoPtr<volatile> Boolean mPowerKeyHandled;
    // accessed from input reader and handler thread
    Boolean mPendingPowerKeyUpCanceled;
    AutoPtr<IHandler> mHandler;
    AutoPtr<WindowState> mLastInputMethodWindow;
    AutoPtr<WindowState> mLastInputMethodTargetWindow;
    Boolean mRecentsVisible;
    Int32 mRecentAppsHeldModifiers;
    Boolean mLanguageSwitchKeyPressed;
    Int32 mLidState;
    Int32 mCameraLensCoverState;
    Boolean mHaveBuiltInKeyboard;
    Boolean mSystemReady;
    Boolean mSystemBooted;
    Boolean mHdmiPlugged;
    AutoPtr<IIUiModeManager> mUiModeManager;
    Int32 mUiMode;
    Int32 mDockMode;
    Int32 mLidOpenRotation;
    Int32 mCarDockRotation;
    Int32 mDeskDockRotation;
    Int32 mUndockedHdmiRotation;
    Int32 mDemoHdmiRotation;
    Boolean mDemoHdmiRotationLock;
    Boolean mWakeGestureEnabledSetting;
    AutoPtr<MyWakeGestureListener> mWakeGestureListener;
    Int32 mUserRotationMode;
    Int32 mUserRotation;
    Boolean mAccelerometerDefault;
    Boolean mSupportAutoRotation;
    Int32 mAllowAllRotations;
    Boolean mCarDockEnablesAccelerometer;
    Boolean mDeskDockEnablesAccelerometer;
    Int32 mLidKeyboardAccessibility;
    Int32 mLidNavigationAccessibility;
    Boolean mLidControlsSleep;
    Int32 mShortPressOnPowerBehavior;
    Int32 mLongPressOnPowerBehavior;
    Boolean mAwake;
    Boolean mScreenOnEarly;
    Boolean mScreenOnFully;
    AutoPtr<ScreenOnListener> mScreenOnListener;
    Boolean mKeyguardDrawComplete;
    Boolean mWindowManagerDrawComplete;
    Boolean mOrientationSensorEnabled;
    Int32 mCurrentAppOrientation;
    Boolean mHasSoftInput;
    Boolean mTranslucentDecorEnabled;
    Int32 mPointerLocationMode;
    // guarded by mLock
    // The last window we were told about in focusChanged.
    AutoPtr<WindowState> mFocusedWindow;
    AutoPtr<IIApplicationToken> mFocusedApp;
    AutoPtr<IPointerLocationView> mPointerLocationView;
    // The current size of the screen; really; extends into the overscan area of
    // the screen and doesn't account for any system elements like the status bar.
    Int32 mOverscanScreenLeft;
    Int32 mOverscanScreenTop;
    Int32 mOverscanScreenWidth;
    Int32 mOverscanScreenHeight;
    // The current visible size of the screen; really; (ir)regardless of whether the status
    // bar can be hidden but not extending into the overscan area.
    Int32 mUnrestrictedScreenLeft;
    Int32 mUnrestrictedScreenTop;
    Int32 mUnrestrictedScreenWidth;
    Int32 mUnrestrictedScreenHeight;
    // Like mOverscanScreen*, but allowed to move into the overscan region where appropriate.
    Int32 mRestrictedOverscanScreenLeft;
    Int32 mRestrictedOverscanScreenTop;
    Int32 mRestrictedOverscanScreenWidth;
    Int32 mRestrictedOverscanScreenHeight;
    // The current size of the screen; these may be different than (0,0)-(dw,dh)
    // if the status bar can't be hidden; in that case it effectively carves out
    // that area of the display from all other windows.
    Int32 mRestrictedScreenLeft;
    Int32 mRestrictedScreenTop;
    Int32 mRestrictedScreenWidth;
    Int32 mRestrictedScreenHeight;
    // During layout, the current screen borders accounting for any currently
    // visible system UI elements.
    Int32 mSystemLeft;
    Int32 mSystemTop;
    Int32 mSystemRight;
    Int32 mSystemBottom;
    // For applications requesting stable content insets, these are them.
    Int32 mStableLeft;
    Int32 mStableTop;
    Int32 mStableRight;
    Int32 mStableBottom;
    // For applications requesting stable content insets but have also set the
    // fullscreen window flag, these are the stable dimensions without the status bar.
    Int32 mStableFullscreenLeft;
    Int32 mStableFullscreenTop;
    Int32 mStableFullscreenRight;
    Int32 mStableFullscreenBottom;
    // During layout, the current screen borders with all outer decoration
    // (status bar, input method dock) accounted for.
    Int32 mCurLeft;
    Int32 mCurTop;
    Int32 mCurRight;
    Int32 mCurBottom;
    // During layout, the frame in which content should be displayed
    // to the user, accounting for all screen decoration except for any
    // space they deem as available for other content.  This is usually
    // the same as mCur*, but may be larger if the screen decor has supplied
    // content insets.
    Int32 mContentLeft;
    Int32 mContentTop;
    Int32 mContentRight;
    Int32 mContentBottom;
    // During layout, the frame in which voice content should be displayed
    // to the user, accounting for all screen decoration except for any
    // space they deem as available for other content.
    Int32 mVoiceContentLeft;
    Int32 mVoiceContentTop;
    Int32 mVoiceContentRight;
    Int32 mVoiceContentBottom;
    // During layout, the current screen borders along which input method
    // windows are placed.
    Int32 mDockLeft;
    Int32 mDockTop;
    Int32 mDockRight;
    Int32 mDockBottom;
    // During layout, the layer at which the doc window is placed.
    Int32 mDockLayer;
    // During layout, this is the layer of the status bar.
    Int32 mStatusBarLayer;
    Int32 mLastSystemUiFlags;
    // Bits that we are in the process of clearing, so we want to prevent
    // them from being set by applications until everything has been updated
    // to have them clear.
    Int32 mResettingSystemUiFlags;
    // Bits that we are currently always keeping cleared.
    Int32 mForceClearedSystemUiFlags;
    // What we last reported to system UI about whether the compatibility
    // menu needs to be displayed.
    Boolean mLastFocusNeedsMenu;
    AutoPtr<FakeWindow> mHideNavFakeWindow;
    static const AutoPtr<IRect> mTmpParentFrame;
    static const AutoPtr<IRect> mTmpDisplayFrame;
    static const AutoPtr<IRect> mTmpOverscanFrame;
    static const AutoPtr<IRect> mTmpContentFrame;
    static const AutoPtr<IRect> mTmpVisibleFrame;
    static const AutoPtr<IRect> mTmpDecorFrame;
    static const AutoPtr<IRect> mTmpStableFrame;
    static const AutoPtr<IRect> mTmpNavigationFrame;
    AutoPtr<WindowState> mTopFullscreenOpaqueWindowState;
    AutoPtr< IHashSet<IIApplicationToken> > mAppsToBeHidden;
    AutoPtr< IHashSet<IIApplicationToken> > mAppsThatDismissKeyguard;
    Boolean mTopIsFullscreen;
    Boolean mForceStatusBar;
    Boolean mForceStatusBarFromKeyguard;
    Boolean mHideLockScreen;
    Boolean mForcingShowNavBar;
    Int32 mForcingShowNavBarLayer;
    // Keyguard has been dismissed.
    Int32 mDismissKeyguard;
    Boolean mShowingLockscreen;
    Boolean mShowingDream;
    Boolean mDreamingLockscreen;
    Boolean mHomePressed;
    Boolean mHomeConsumed;
    Boolean mHomeDoubleTapPending;
    AutoPtr<IIntent> mHomeIntent;
    AutoPtr<IIntent> mCarDockIntent;
    AutoPtr<IIntent> mDeskDockIntent;
    Boolean mSearchKeyShortcutPending;
    Boolean mConsumeSearchKeyUp;
    Boolean mAssistKeyLongPressed;
    Boolean mPendingMetaAction;
    // support for activating the lock screen while the screen is on
    Boolean mAllowLockscreenWhenOn;
    Int32 mLockScreenTimeout;
    Boolean mLockScreenTimerActive;
    // Behavior of ENDCALL Button.  (See Settings.System.END_BUTTON_BEHAVIOR.)
    Int32 mEndcallBehavior;
    // Behavior of POWER button while in-call and screen on.
    // (See Settings.Secure.INCALL_POWER_BUTTON_BEHAVIOR.)
    Int32 mIncallPowerBehavior;
    AutoPtr<IDisplay> mDisplay;
    Int32 mLandscapeRotation;
    // default landscape rotation
    Int32 mSeascapeRotation;
    // "other" landscape rotation, 180 degrees from mLandscapeRotation
    Int32 mPortraitRotation;
    // default portrait rotation
    Int32 mUpsideDownRotation;
    // "other" portrait rotation
    Int32 mOverscanLeft;
    Int32 mOverscanTop;
    Int32 mOverscanRight;
    Int32 mOverscanBottom;
    AutoPtr<SettingsObserver> mSettingsObserver;
    AutoPtr<ShortcutManager> mShortcutManager;
    AutoPtr<PowerManager::WakeLock> mBroadcastWakeLock;
    Boolean mHavePendingMediaKeyRepeatWithWakeLock;
    AutoPtr<MyOrientationListener> mOrientationListener;
    static const Boolean PRINT_ANIM;
    /*const*/ AutoPtr<InputEventReceiver::Factory> mHideNavInputEventReceiverFactory;
    /*const*/ AutoPtr<Object> mScreenshotLock;
    AutoPtr<IServiceConnection> mScreenshotConnection;
    /*const*/ AutoPtr<Runnable> mScreenshotTimeout;
    AutoPtr<IBroadcastReceiver> mDockReceiver;
    AutoPtr<IBroadcastReceiver> mDreamReceiver;
    AutoPtr<IBroadcastReceiver> mMultiuserReceiver;
    AutoPtr<IProgressDialog> mBootMsgDialog;
    AutoPtr<ScreenLockTimeout> mScreenLockTimeout;

private:
    static const AutoPtr<IAudioAttributes> VIBRATION_ATTRIBUTES;
    /**
      * Keyguard stuff
      */
    AutoPtr<WindowState> mKeyguardScrim;
    Boolean mKeyguardHidden;
    Boolean mKeyguardDrawnOnce;
    /**
      * Lock protecting internal state.  Must not call out into window
      * manager with lock held.  (This lock will be acquired in places
      * where the window manager is calling in with its own lock held.)
      */
    /*const*/ AutoPtr<Object> mLock;
    // Default display does not rotate, apps that require non-default orientation will have to
    // have the orientation emulated.
    Boolean mForceDefaultOrientation;
    // States of keyguard dismiss.
    static const Int32 DISMISS_KEYGUARD_NONE = 0;
    // Keyguard not being dismissed.
    static const Int32 DISMISS_KEYGUARD_START = 1;
    // Keyguard needs to be dismissed.
    static const Int32 DISMISS_KEYGUARD_CONTINUE = 2;
    /** The window that is currently dismissing the keyguard. Dismissing the keyguard must only
      * be done once per window. */
    AutoPtr<WindowState> mWinDismissingKeyguard;
    /** The window that is currently showing "over" the keyguard. If there is an app window
      * belonging to another app on top of this the keyguard shows. If there is a fullscreen
      * app window under this, still dismiss the keyguard but don't show the app underneath. Show
      * the wallpaper. */
    AutoPtr<WindowState> mWinShowWhenLocked;
    // What we do when the user long presses on home
    Int32 mLongPressOnHomeBehavior;
    // What we do when the user double-taps on home
    Int32 mDoubleTapOnHomeBehavior;
    // Screenshot trigger states
    // Time to volume and power must be pressed within this interval of each other.
    static const Int64 SCREENSHOT_CHORD_DEBOUNCE_DELAY_MILLIS = 150;
    // Increase the chord delay when taking a screenshot from the keyguard
    static const Float KEYGUARD_SCREENSHOT_CHORD_DELAY_MULTIPLIER;
    Boolean mScreenshotChordEnabled;
    Boolean mVolumeDownKeyTriggered;
    Int64 mVolumeDownKeyTime;
    Boolean mVolumeDownKeyConsumedByScreenshotChord;
    Boolean mVolumeUpKeyTriggered;
    Boolean mPowerKeyTriggered;
    Int64 mPowerKeyTime;
    /* The number of steps between min and max brightness */
    static const Int32 BRIGHTNESS_STEPS = 10;
    Int32 mCurrentUserId;
    // Maps global key codes to the components that will handle them.
    AutoPtr<GlobalKeyManager> mGlobalKeyManager;
    // Fallback actions by key code.
    /*const*/ AutoPtr< ISparseArray<KeyCharacterMap::FallbackAction> > mFallbackActions;
    /*const*/ AutoPtr<LogDecelerateInterpolator> mLogDecelerateInterpolator;
    static const Int32 MSG_ENABLE_POINTER_LOCATION = 1;
    static const Int32 MSG_DISABLE_POINTER_LOCATION = 2;
    static const Int32 MSG_DISPATCH_MEDIA_KEY_WITH_WAKE_LOCK = 3;
    static const Int32 MSG_DISPATCH_MEDIA_KEY_REPEAT_WITH_WAKE_LOCK = 4;
    static const Int32 MSG_KEYGUARD_DRAWN_COMPLETE = 5;
    static const Int32 MSG_KEYGUARD_DRAWN_TIMEOUT = 6;
    static const Int32 MSG_WINDOW_MANAGER_DRAWN_COMPLETE = 7;
    static const Int32 MSG_DISPATCH_SHOW_RECENTS = 9;
    static const Int32 MSG_DISPATCH_SHOW_GLOBAL_ACTIONS = 10;
    static const Int32 MSG_HIDE_BOOT_MESSAGE = 11;
    static const Int32 MSG_LAUNCH_VOICE_ASSIST_WITH_WAKE_LOCK = 12;
    AutoPtr<IUEventObserver> mHDMIObserver;
    /*const*/ AutoPtr<BarController> mStatusBarController;
    /*const*/ AutoPtr<BarController> mNavigationBarController;
    AutoPtr<ImmersiveModeConfirmation> mImmersiveModeConfirmation;
    AutoPtr<SystemGesturesPointerEventListener> mSystemGestures;
    /*const*/ AutoPtr<Runnable> mPowerLongPress;
    /*const*/ AutoPtr<Runnable> mScreenshotRunnable;
    /*const*/ AutoPtr<Runnable> mHomeDoubleTapTimeoutRunnable;
    static AutoPtr< ArrayOf<Int32> > WINDOW_TYPES_WHERE_HOME_DOESNT_WORK;
    /*const*/ AutoPtr<Runnable> mClearHideNavigationFlag;
    /*const*/ AutoPtr<Runnable> mRequestTransientNav;
};

} // namespace Impl
} // namespace Policy
} // namespace Internal
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_INTERNAL_POLICY_IMPL_PHONEWINDOWMANAGER_H__

