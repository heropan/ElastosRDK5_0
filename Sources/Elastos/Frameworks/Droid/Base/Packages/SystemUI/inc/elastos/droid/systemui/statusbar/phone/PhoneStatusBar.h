
#ifndef __ELASTOS_DROID_SYSTEMUI_STATUSBAR_PHONE_PHONESTATUSBAR_H__
#define __ELASTOS_DROID_SYSTEMUI_STATUSBAR_PHONE_PHONESTATUSBAR_H__

#include "_Elastos.Droid.Core.h"
#include "elastos/droid/systemui/statusbar/BaseStatusBar.h"
#include "elastos/droid/systemui/statusbar/GestureRecorder.h"
#include "elastos/droid/systemui/statusbar/KeyguardIndicationController.h"
#include "elastos/droid/systemui/statusbar/phone/DemoStatusIcons.h"
#include "elastos/droid/systemui/statusbar/phone/PhoneStatusBarPolicy.h"
#include "elastos/droid/systemui/statusbar/phone/ScrimController.h"
#include "elastos/droid/systemui/statusbar/phone/StatusBarWindowManager.h"
#include "elastos/droid/systemui/statusbar/phone/Ticker.h"
#include "elastos/droid/systemui/statusbar/phone/UnlockMethodCache.h"
#include "elastos/droid/systemui/statusbar/policy/BatteryController.h"
#include "elastos/droid/systemui/statusbar/policy/BluetoothControllerImpl.h"
#include "elastos/droid/systemui/statusbar/policy/BrightnessMirrorController.h"
#include "elastos/droid/systemui/statusbar/policy/CastControllerImpl.h"
#include "elastos/droid/systemui/statusbar/policy/FlashlightController.h"
#include "elastos/droid/systemui/statusbar/policy/HotspotControllerImpl.h"
#include "elastos/droid/systemui/statusbar/policy/KeyguardMonitor.h"
#include "elastos/droid/systemui/statusbar/policy/KeyguardUserSwitcher.h"
#include "elastos/droid/systemui/statusbar/policy/LocationControllerImpl.h"
#include "elastos/droid/systemui/statusbar/policy/NetworkControllerImpl.h"
#include "elastos/droid/systemui/statusbar/policy/NextAlarmController.h"
#include "elastos/droid/systemui/statusbar/policy/SecurityControllerImpl.h"
#include "elastos/droid/systemui/statusbar/policy/RotationLockControllerImpl.h"
#include "elastos/droid/systemui/statusbar/policy/UserInfoController.h"
#include "elastos/droid/systemui/statusbar/policy/UserSwitcherController.h"
#include "Elastos.Droid.Graphics.h"
#include "Elastos.Droid.Media.h"
#include <elastos/droid/animation/AnimatorListenerAdapter.h>
#include <elastos/droid/graphics/drawable/Drawable.h>
#include <elastos/droid/media/session/MediaControllerCallback.h>

using Elastos::Droid::Animation::AnimatorListenerAdapter;
using Elastos::Droid::Graphics::Drawable::Drawable;
using Elastos::Droid::Graphics::ICanvas;
using Elastos::Droid::Graphics::IColorFilter;
using Elastos::Droid::Graphics::IPoint;
using Elastos::Droid::Graphics::IPorterDuffXfermode;
using Elastos::Droid::Keyguard::IViewMediatorCallback;
using Elastos::Droid::Media::IAudioAttributes;
using Elastos::Droid::Media::IMediaMetadata;
using Elastos::Droid::Media::Session::IMediaController;
using Elastos::Droid::Media::Session::IMediaControllerCallback;
using Elastos::Droid::Media::Session::IMediaSessionManager;
using Elastos::Droid::Media::Session::IPlaybackState;
using Elastos::Droid::Media::Session::MediaControllerCallback;
using Elastos::Droid::SystemUI::Doze::IDozeHost;
using Elastos::Droid::SystemUI::Doze::IDozeHostCallback;
using Elastos::Droid::SystemUI::Doze::IDozeHostPulseCallback;
using Elastos::Droid::SystemUI::Qs::IQSPanel;
using Elastos::Droid::SystemUI::Qs::IQSTileHost;
using Elastos::Droid::SystemUI::Qs::IQSTileHostCallback;
using Elastos::Droid::SystemUI::StatusBar::Policy::BatteryController;
using Elastos::Droid::SystemUI::StatusBar::Policy::BluetoothControllerImpl;
using Elastos::Droid::SystemUI::StatusBar::Policy::BrightnessMirrorController;
using Elastos::Droid::SystemUI::StatusBar::Policy::CastControllerImpl;
using Elastos::Droid::SystemUI::StatusBar::Policy::FlashlightController;
using Elastos::Droid::SystemUI::StatusBar::Policy::HotspotControllerImpl;
using Elastos::Droid::SystemUI::StatusBar::Policy::IAccessibilityController;
using Elastos::Droid::SystemUI::StatusBar::Policy::IBatteryStateChangeCallback;
using Elastos::Droid::SystemUI::StatusBar::Policy::IZenModeController;
using Elastos::Droid::SystemUI::StatusBar::Policy::KeyguardMonitor;
using Elastos::Droid::SystemUI::StatusBar::Policy::KeyguardUserSwitcher;
using Elastos::Droid::SystemUI::StatusBar::Policy::LocationControllerImpl;
using Elastos::Droid::SystemUI::StatusBar::Policy::NetworkControllerImpl;
using Elastos::Droid::SystemUI::StatusBar::Policy::NextAlarmController;
using Elastos::Droid::SystemUI::StatusBar::Policy::SecurityControllerImpl;
using Elastos::Droid::SystemUI::StatusBar::Policy::RotationLockControllerImpl;
using Elastos::Droid::SystemUI::StatusBar::Policy::UserInfoController;
using Elastos::Droid::SystemUI::StatusBar::Policy::UserSwitcherController;
using Elastos::Droid::SystemUI::StatusBar::Stack::IOnChildLocationsChangedListener;
using Elastos::Droid::SystemUI::StatusBar::BaseStatusBar;
using Elastos::Droid::SystemUI::StatusBar::GestureRecorder;
using Elastos::Droid::SystemUI::StatusBar::KeyguardIndicationController;
using Elastos::Droid::SystemUI::Volume::IVolumeComponent;
using Elastos::Droid::Utility::IDisplayMetrics;
using Elastos::Droid::View::Animation::IAnimation;
using Elastos::Droid::View::Animation::IAnimationAnimationListener;
using Elastos::Droid::View::Animation::IInterpolator;
using Elastos::Droid::View::IKeyEvent;
using Elastos::Droid::View::IOnPreDrawListener;
using Elastos::Droid::View::IVelocityTracker;
using Elastos::Droid::View::IViewPropertyAnimator;
using Elastos::Droid::View::IViewOnFocusChangeListener;
using Elastos::Droid::Widget::ILinearLayout;
using Elastos::Utility::ICollection;

namespace Elastos {
namespace Droid {
namespace SystemUI {
namespace StatusBar {
namespace Phone {

class PhoneStatusBar
    : public BaseStatusBar
    , public IPhoneStatusBar
    , public IDemoMode
    , public IDragDownCallback
    , public IActivityStarter
{
private:
    class UserSetupObserver: public ContentObserver
    {
    public:
        UserSetupObserver(
            /* [in] */ PhoneStatusBar* host,
            /* [in] */ IHandler* handler);

        // @Override
        CARAPI OnChange(
            /* [in] */ Boolean selfChange);

    private:
        PhoneStatusBar* mHost;
    };

    class HeadsUpObserver: public ContentObserver
    {
    public:
        HeadsUpObserver(
            /* [in] */ PhoneStatusBar* host,
            /* [in] */ IHandler* handler);

        // @Override
        CARAPI OnChange(
            /* [in] */ Boolean selfChange);

    private:
        PhoneStatusBar* mHost;
    };

    class Autohide: public Runnable
    {
    public:
        Autohide(
            /* [in] */ PhoneStatusBar* host);

        // @Override
        CARAPI Run();

    private:
        PhoneStatusBar* mHost;
    };

    class MediaListener: public MediaControllerCallback
    {
    public:
        MediaListener(
            /* [in] */ PhoneStatusBar* host);

        // @Override
        CARAPI OnPlaybackStateChanged(
            /* [in] */ IPlaybackState* state);

        // @Override
        CARAPI OnMetadataChanged(
            /* [in] */ IMediaMetadata* metadata);

    private:
        PhoneStatusBar* mHost;
    };

    class OnChildLocationsChangedListener
        : public Object
        , public IOnChildLocationsChangedListener
    {
    public:
        CAR_INTERFACE_DECL();

        OnChildLocationsChangedListener(
            /* [in] */ PhoneStatusBar* host);

        // @Override
        CARAPI OnChildLocationsChanged(
            /* [in] */ INotificationStackScrollLayout* stackScrollLayout);

    private:
        PhoneStatusBar* mHost;
    };

    class NotificationLocationsChangedListener
        : public Object
        , public IOnChildLocationsChangedListener
    {
    public:
        CAR_INTERFACE_DECL();

        NotificationLocationsChangedListener(
            /* [in] */ PhoneStatusBar* host);

        // @Override
        CARAPI OnChildLocationsChanged(
            /* [in] */ INotificationStackScrollLayout* stackScrollLayout);

    private:
        PhoneStatusBar* mHost;
    };

    class VisibilityReporter: public Runnable
    {
    public:
        VisibilityReporter(
            /* [in] */ PhoneStatusBar* host);

        // @Override
        CARAPI Run();

    private:
        PhoneStatusBar* mHost;
        AutoPtr<IArrayList/*<String*/> mTmpNewlyVisibleNotifications;
        AutoPtr<IArrayList/*<String*/> mTmpCurrentlyVisibleNotifications;
    };

    class OverflowClickListener
        : public Object
        , public IViewOnClickListener
    {
    public:
        CAR_INTERFACE_DECL();

        OverflowClickListener(
            /* [in] */ PhoneStatusBar* host);

        // @Override
        CARAPI OnClick(
            /* [in] */ IView* v);

    private:
        PhoneStatusBar* mHost;
    };

    class RecentsClickListener
        : public Object
        , public IViewOnClickListener
    {
    public:
        CAR_INTERFACE_DECL();

        RecentsClickListener(
            /* [in] */ PhoneStatusBar* host);

        CARAPI OnClick(
            /* [in] */ IView* v);

    private:
        PhoneStatusBar* mHost;
    };

    class LongPressBackRecentsListener
        : public Object
        , public IViewOnLongClickListener
    {
    public:
        CAR_INTERFACE_DECL();

        LongPressBackRecentsListener(
            /* [in] */ PhoneStatusBar* host);

        // @Override
        CARAPI OnLongClick(
            /* [in] */ IView* v,
            /* [out] */ Boolean* result);

    private:
        PhoneStatusBar* mHost;
    };

    class ShowSearchPanelRunnable: public Runnable
    {
    public:
        ShowSearchPanelRunnable(
            /* [in] */ PhoneStatusBar* host);

        CARAPI Run();

    private:
        PhoneStatusBar* mHost;
    };

    class HomeActionListener
        : public Object
        , public IViewOnTouchListener
    {
    public:
        CAR_INTERFACE_DECL();

        HomeActionListener(
            /* [in] */ PhoneStatusBar* host);

        CARAPI OnTouch(
            /* [in] */ IView* v,
            /* [in] */ IMotionEvent* event,
            /* [out] */ Boolean* result);

    private:
        PhoneStatusBar* mHost;
    };

    class HideBackdropFront: public Runnable
    {
    public:
        HideBackdropFront(
            /* [in] */ PhoneStatusBar* host);

        // @Override
        CARAPI Run();

    private:
        PhoneStatusBar* mHost;
    };

    /**
     * All changes to the status bar and notifications funnel through here and are batched.
     */
    class H: public BaseStatusBar::H
    {
    public:
        H(
            /* [in] */ PhoneStatusBar* host);

        CARAPI HandleMessage(
            /* [in] */ IMessage* m);

    private:
        PhoneStatusBar* mHost;
    };

    class FocusChangeListener
        : public Object
        , public IViewOnFocusChangeListener
    {
    public:
        CAR_INTERFACE_DECL();

        CARAPI OnFocusChange(
            /* [in] */ IView* v,
            /* [in] */ Boolean hasFocus);
    };

    class AnimateCollapsePanelsRunnable: public Runnable
    {
    public:
        AnimateCollapsePanelsRunnable(
            /* [in] */ PhoneStatusBar* host);

        // @Override
        CARAPI Run();

    private:
        PhoneStatusBar* mHost;
    };

    class CheckBarModesRunnable: public Runnable
    {
    public:
        CheckBarModesRunnable(
            /* [in] */ PhoneStatusBar* host);

        // @Override
        CARAPI Run();

    private:
        PhoneStatusBar* mHost;
    };

    class MyTicker: public Ticker
    {
    public:
        MyTicker(
            /* [in] */ PhoneStatusBar* host,
            /* [in] */ IContext* context,
            /* [in] */ IView* sb);

        // @Override
        CARAPI TickerStarting();

        // @Override
        CARAPI TickerDone();

        CARAPI TickerHalting();

    private:
        PhoneStatusBar* mHost;
    };

    class TickingDoneListener
        : public Object
        , public IAnimationAnimationListener
    {
    public:
        CAR_INTERFACE_DECL();

        TickingDoneListener(
            /* [in] */ PhoneStatusBar* host);

        CARAPI OnAnimationEnd(
            /* [in] */ IAnimation* animation);

        CARAPI OnAnimationRepeat(
            /* [in] */ IAnimation* animation);

        CARAPI OnAnimationStart(
            /* [in] */ IAnimation* animation);

    private:
        PhoneStatusBar* mHost;
    };

    class InnerReceiver: public BroadcastReceiver
    {
    public:
        InnerReceiver(
            /* [in] */ PhoneStatusBar* host);

        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

    private:
        PhoneStatusBar* mHost;
    };

    class StartTracing: public Runnable
    {
    public:
        StartTracing(
            /* [in] */ PhoneStatusBar* host);

        CARAPI Run();

    private:
        PhoneStatusBar* mHost;
    };

    class StopTracing: public Runnable
    {
    public:
        StopTracing(
            /* [in] */ PhoneStatusBar* host);

        CARAPI Run();

    private:
        PhoneStatusBar* mHost;
    };

    class FastColorDrawable: public Drawable
    {
    public:
        FastColorDrawable(
            /* [in] */ Int32 color);

        // @Override
        CARAPI Draw(
            /* [in] */ ICanvas* canvas);

        // @Override
        CARAPI SetAlpha(
            /* [in] */ Int32 alpha);

        // @Override
        CARAPI SetColorFilter(
            /* [in] */ IColorFilter* cf);

        // @Override
        CARAPI GetOpacity(
            /* [out] */ Int32* result);

        // @Override
        CARAPI SetBounds(
            /* [in] */ Int32 left,
            /* [in] */ Int32 top,
            /* [in] */ Int32 right,
            /* [in] */ Int32 bottom);

        // @Override
        CARAPI SetBounds(
            /* [in] */ IRect* bounds);

    private:
        Int32 mColor;
    };

    class ShadeUpdates: public Object
    {
    public:
        ShadeUpdates(
            /* [in] */ PhoneStatusBar* host);

        CARAPI Check();

    private:
        PhoneStatusBar* mHost;

        AutoPtr<IArraySet/*<String*/> mVisibleNotifications;
        AutoPtr<IArraySet/*<String*/> mNewVisibleNotifications;
    };

    class DozeServiceHost
        : public Object
        , public IDozeHost
    {
    private:
        class DozeServiceHostHandler: public Handler
        {
        public:
            DozeServiceHostHandler(
                /* [in] */ DozeServiceHost* host);

            // @Override
            CARAPI HandleMessage(
                /* [in] */ IMessage* msg);

        public:
            DozeServiceHost* mHost;
            static const Int32 MSG_START_DOZING;
            static const Int32 MSG_PULSE_WHILE_DOZING;
            static const Int32 MSG_STOP_DOZING;
        };

    public:
        CAR_INTERFACE_DECL();

        DozeServiceHost(
            /* [in] */ PhoneStatusBar* host);

        // @Override
        CARAPI ToString(
            /* [out] */ String* str);

        CARAPI FirePowerSaveChanged(
            /* [in] */ Boolean active);

        CARAPI FireBuzzBeepBlinked();

        CARAPI FireNotificationLight(
            /* [in] */ Boolean on);

        CARAPI FireNewNotifications();

        // @Override
        CARAPI AddCallback(
            /* [in] */ /*@NonNull*/ IDozeHostCallback* callback);

        // @Override
        CARAPI RemoveCallback(
            /* [in] */ /*@NonNull*/ IDozeHostCallback* callback);

        // @Override
        CARAPI StartDozing(
            /* [in] */ /*@NonNull*/ IRunnable* ready);

        // @Override
        CARAPI PulseWhileDozing(
            /* [in] */ /*@NonNull*/ IDozeHostPulseCallback* callback);

        // @Override
        CARAPI StopDozing();

        // @Override
        CARAPI IsPowerSaveActive(
            /* [out] */ Boolean* result);

    private:
        CARAPI_(void) HandleStartDozing(
            /* [in] */ /*@NonNull*/ IRunnable* ready);

        CARAPI_(void) HandlePulseWhileDozing(
            /* [in] */ /*@NonNull*/ IDozeHostPulseCallback* callback);

        CARAPI_(void) HandleStopDozing();

    private:
        PhoneStatusBar* mHost;
        // Amount of time to allow to update the time shown on the screen before releasing
        // the wakelock.  This timeout is design to compensate for the fact that we don't
        // currently have a way to know when time display contents have actually been
        // refreshed once we've finished rendering a new frame.
        static const Int64 PROCESSING_TIME;

        AutoPtr<IArrayList/*<Callback*/> mCallbacks;
        AutoPtr<DozeServiceHostHandler> mHandler;
    };

    class OnTouchListener1
        : public Object
        , public IViewOnTouchListener
    {
    public:
        CAR_INTERFACE_DECL();

        OnTouchListener1(
            /* [in] */ PhoneStatusBar* host);

        CARAPI OnTouch(
            /* [in] */ IView* v,
            /* [in] */ IMotionEvent* event,
            /* [out] */ Boolean* result);

    private:
        PhoneStatusBar* mHost;
    };

    class VerticalChangedListener
        : public Object
        , public IOnVerticalChangedListener
    {
    public:
        CAR_INTERFACE_DECL();

        VerticalChangedListener(
            /* [in] */ PhoneStatusBar* host);

        // @Override
        CARAPI OnVerticalChanged(
            /* [in] */ Boolean isVertical);

    private:
        PhoneStatusBar* mHost;
    };

    class OnTouchListener2
        : public Object
        , public IViewOnTouchListener
    {
    public:
        CAR_INTERFACE_DECL();

        OnTouchListener2(
            /* [in] */ PhoneStatusBar* host);

        CARAPI OnTouch(
            /* [in] */ IView* v,
            /* [in] */ IMotionEvent* event,
            /* [out] */ Boolean* result);

    private:
        PhoneStatusBar* mHost;
    };

    class OnClickListener1
        : public Object
        , public IViewOnClickListener
    {
    public:
        CAR_INTERFACE_DECL();

        OnClickListener1(
            /* [in] */ PhoneStatusBar* host);

        // @Override
        CARAPI OnClick(
            /* [in] */ IView* v);

    private:
        PhoneStatusBar* mHost;
    };

    class BatteryStateChangeCallback
        : public Object
        , public IBatteryStateChangeCallback
    {
    public:
        CAR_INTERFACE_DECL();

        BatteryStateChangeCallback(
            /* [in] */ PhoneStatusBar* host);

        // @Override
        CARAPI OnPowerSaveChanged();

        // @Override
        CARAPI OnBatteryLevelChanged(
            /* [in] */ Int32 level,
            /* [in] */ Boolean pluggedIn,
            /* [in] */ Boolean charging);

    private:
        PhoneStatusBar* mHost;
    };

    class HostCallback
        : public Object
        , public IQSTileHostCallback
    {
    public:
        CAR_INTERFACE_DECL();

        HostCallback(
            /* [in] */ PhoneStatusBar* host,
            /* [in] */ IQSTileHost* qsh);

        // @Override
        CARAPI OnTilesChanged();

    private:
        PhoneStatusBar* mHost;
        IQSTileHost* mQsh;
    };

    class Runnable1: public Runnable
    {
    public:
        Runnable1(
            /* [in] */ PhoneStatusBar* host);

        // @Override
        CARAPI Run();

    private:
        PhoneStatusBar* mHost;
    };

    class Runnable2: public Runnable
    {
    public:
        Runnable2(
            /* [in] */ PhoneStatusBar* host);

        // @Override
        CARAPI Run();

    private:
        PhoneStatusBar* mHost;
    };

    class Runnable3: public Runnable
    {
    public:
        Runnable3(
            /* [in] */ PhoneStatusBar* host);

        // @Override
        CARAPI Run();

    private:
        PhoneStatusBar* mHost;
    };

    class OnPreDrawListener
        : public Object
        , public IOnPreDrawListener
    {
    public:
        CAR_INTERFACE_DECL();

        OnPreDrawListener(
            /* [in] */ PhoneStatusBar* host);

        // @Override
        CARAPI OnPreDraw(
            /* [out] */ Boolean* result);

    private:
        PhoneStatusBar* mHost;
    };

    class Runnable4: public Runnable
    {
    public:
        Runnable4(
            /* [in] */ PhoneStatusBar* host);

        // @Override
        CARAPI Run();

    private:
        PhoneStatusBar* mHost;
    };

    class AnimatorListenerAdapter1: public AnimatorListenerAdapter
    {
    public:
        AnimatorListenerAdapter1(
            /* [in] */ PhoneStatusBar* host);

        // @Override
        CARAPI OnAnimationEnd(
            /* [in] */ IAnimator* animation);

    private:
        PhoneStatusBar* mHost;
    };

    class AnimatorListenerAdapter2: public AnimatorListenerAdapter
    {
    public:
        AnimatorListenerAdapter2(
            /* [in] */ IView* view);

        // @Override
        CARAPI OnAnimationEnd(
            /* [in] */ IAnimator* animation);

    private:
        AutoPtr<IView> mView;
    };

    class Runnable5: public Runnable
    {
    public:
        Runnable5(
            /* [in] */ PhoneStatusBar* host);

        // @Override
        CARAPI Run();

    private:
        PhoneStatusBar* mHost;
    };

    class Runnable6: public Runnable
    {
    public:
        Runnable6(
            /* [in] */ IView* view);

        // @Override
        CARAPI Run();

    private:
        AutoPtr<IView> mView;
    };

    class AnimatorListenerAdapter3: public AnimatorListenerAdapter
    {
    public:
        AnimatorListenerAdapter3(
            /* [in] */ IView* view,
            /* [in] */ IViewPropertyAnimator* a,
            /* [in] */ Int32 vis);

        // @Override
        CARAPI OnAnimationEnd(
            /* [in] */ IAnimator* animation);

    private:
        AutoPtr<IView> mView;
        AutoPtr<IViewPropertyAnimator> mA;
        Int32 mVis;
    };

    class AnimatorListenerAdapter4: public AnimatorListenerAdapter
    {
    public:
        AnimatorListenerAdapter4(
            /* [in] */ IView* view,
            /* [in] */ Int32 vis);

        // @Override
        CARAPI OnAnimationEnd(
            /* [in] */ IAnimator* animation);

    private:
        AutoPtr<IView> mView;
        Int32 mVis;
    };

    class Runnable7: public Runnable
    {
    public:
        Runnable7(
            /* [in] */ PhoneStatusBar* host);

        // @Override
        CARAPI Run();

    private:
        PhoneStatusBar* mHost;
    };

    class Runnable8: public Runnable
    {
    public:
        Runnable8(
            /* [in] */ PhoneStatusBar* host);

        // @Override
        CARAPI Run();

    private:
        PhoneStatusBar* mHost;
    };

    class Runnable9: public Runnable
    {
    public:
        Runnable9(
            /* [in] */ PhoneStatusBar* host,
            /* [in] */ IIntent* intent);

        // @Override
        CARAPI Run();

    private:
        PhoneStatusBar* mHost;
        AutoPtr<IIntent> mIntent;
    };

    class Runnable10: public Runnable
    {
    public:
        Runnable10(
            /* [in] */ PhoneStatusBar* host);

        // @Override
        CARAPI Run();

    private:
        PhoneStatusBar* mHost;
    };

    class Runnable11: public Runnable
    {
    public:
        Runnable11(
            /* [in] */ PhoneStatusBar* host,
            /* [in] */ IRunnable* beforeFading);

        // @Override
        CARAPI Run();

    private:
        PhoneStatusBar* mHost;
        AutoPtr<IRunnable> mBeforeFading;
    };

public:
    CAR_INTERFACE_DECL();

    PhoneStatusBar();

    // @Override
    CARAPI Start();

    CARAPI GetStatusBarWindow(
        /* [out] */ IStatusBarWindowView** view);

    // @Override
    CARAPI ShowSearchPanel();

    // @Override
    CARAPI HideSearchPanel();

    CARAPI GetStatusBarHeight(
        /* [out] */ Int32* result);

    CARAPI RefreshAllStatusBarIcons();

    CARAPI AddIcon(
        /* [in] */ const String& slot,
        /* [in] */ Int32 index,
        /* [in] */ Int32 viewIndex,
        /* [in] */ IStatusBarIcon* icon);

    CARAPI UpdateIcon(
        /* [in] */ const String& slot,
        /* [in] */ Int32 index,
        /* [in] */ Int32 viewIndex,
        /* [in] */ IStatusBarIcon* old,
        /* [in] */ IStatusBarIcon* icon);

    CARAPI RemoveIcon(
        /* [in] */ const String& slot,
        /* [in] */ Int32 index,
        /* [in] */ Int32 viewIndex);

    CARAPI GetCurrentUserHandle(
        /* [out] */ IUserHandle** uh);

    // @Override
    CARAPI AddNotification(
        /* [in] */ IStatusBarNotification* notification,
        /* [in] */ INotificationListenerServiceRankingMap* ranking);

    CARAPI DisplayNotificationFromHeadsUp(
        /* [in] */ IStatusBarNotification* notification);

    // @Override
    CARAPI ResetHeadsUpDecayTimer();

    // @Override
    CARAPI ScheduleHeadsUpOpen();

    // @Override
    CARAPI ScheduleHeadsUpClose();

    // @Override
    CARAPI ScheduleHeadsUpEscalation();

    // @Override
    CARAPI RemoveNotification(
        /* [in] */ const String& key,
        /* [in] */ INotificationListenerServiceRankingMap* ranking);

    CARAPI FindAndUpdateMediaNotifications();

    /**
     * Refresh or remove lockscreen artwork from media metadata.
     */
    CARAPI UpdateMediaMetaData(
        /* [in] */ Boolean metaDataChanged);

    CARAPI ShowClock(
        /* [in] */ Boolean show);

    /**
     * State is one or more of the DISABLE constants from StatusBarManager.
     */
    CARAPI Disable(
        /* [in] */ Int32 state,
        /* [in] */ Boolean animate);

    // @Override
    CARAPI StartActivity(
        /* [in] */ IIntent* intent,
        /* [in] */ Boolean dismissShade);

    CARAPI GetScrimController(
        /* [out] */ IScrimController** result);

    CARAPI SetQsExpanded(
        /* [in] */ Boolean expanded);

    CARAPI IsGoingToNotificationShade(
        /* [out] */ Boolean* result);

    CARAPI IsQsExpanded(
        /* [out] */ Boolean* result);

    CARAPI IsScreenOnComingFromTouch(
        /* [out] */ Boolean* result);

    CARAPI IsFalsingThresholdNeeded(
        /* [out] */ Boolean* result);

    CARAPI IsDozing(
        /* [out] */ Boolean* result);

    // @Override  // NotificationData.Environment
    CARAPI GetCurrentMediaNotificationKey(
        /* [out] */ String* result);

    CARAPI IsScrimSrcModeEnabled(
        /* [out] */ Boolean* result);

    CARAPI_(Boolean) PanelsEnabled();

    CARAPI MakeExpandedVisible(
        /* [in] */ Boolean force);

    CARAPI AnimateCollapsePanels();

    CARAPI PostAnimateCollapsePanels();

    CARAPI AnimateCollapsePanels(
        /* [in] */ Int32 flags);

    CARAPI AnimateCollapsePanels(
        /* [in] */ Int32 flags,
        /* [in] */ Boolean force);

    CARAPI SetVisibilityWhenDone(
        /* [in] */ IViewPropertyAnimator* a,
        /* [in] */ IView* v,
        /* [in] */ Int32 vis,
        /* [out] */ IViewPropertyAnimator** result);

    CARAPI SetVisibilityWhenDone(
        /* [in] */ IAnimator* a,
        /* [in] */ IView* v,
        /* [in] */ Int32 vis,
        /* [out] */ IAnimator** result);

    CARAPI Interpolator(
        /* [in] */ ITimeInterpolator* ti,
        /* [in] */ IAnimator* a,
        /* [out] */ IAnimator** result);

    CARAPI StartDelay(
        /* [in] */ Int32 d,
        /* [in] */ IAnimator* a,
        /* [out] */ IAnimator** result);

    CARAPI Start(
        /* [in] */ IAnimator* a,
        /* [out] */ IAnimator** result);

    // @Override
    CARAPI AnimateExpandNotificationsPanel();

    // @Override
    CARAPI AnimateExpandSettingsPanel();

    CARAPI AnimateCollapseQuickSettings();

    CARAPI MakeExpandedInvisible();

    CARAPI InterceptTouchEvent(
        /* [in] */ IMotionEvent* event,
        /* [out] */ Boolean* result);

    CARAPI GetGestureRecorder(
        /* [out] */ IGestureRecorder** result);

    // @Override // CommandQueue
    CARAPI SetWindowState(
        /* [in] */ Int32 window,
        /* [in] */ Int32 state);

    // @Override // CommandQueue
    CARAPI BuzzBeepBlinked();

    // @Override
    CARAPI NotificationLightOff();

    // @Override
    CARAPI NotificationLightPulse(
        /* [in] */ Int32 argb,
        /* [in] */ Int32 onMillis,
        /* [in] */ Int32 offMillis);

    // @Override // CommandQueue
    CARAPI SetSystemUiVisibility(
        /* [in] */ Int32 vis,
        /* [in] */ Int32 mask);

    // @Override
    CARAPI SetInteracting(
        /* [in] */ Int32 barWindow,
        /* [in] */ Boolean interacting);

    CARAPI SetLightsOn(
        /* [in] */ Boolean on);

    CARAPI TopAppWindowChanged(
        /* [in] */ Boolean showMenu);

    // @Override
    CARAPI SetImeWindowStatus(
        /* [in] */ IBinder* token,
        /* [in] */ Int32 vis,
        /* [in] */ Int32 backDisposition,
        /* [in] */ Boolean showImeSwitcher);

    CARAPI Dump(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ IPrintWriter* pw,
        /* [in] */ ArrayOf<String>* args);

    // @Override
    CARAPI_(void) CreateAndAddWindows();

    static CARAPI_(Float) Saturate(
        /* [in] */ Float a);

    // @Override
    CARAPI_(void) UpdateExpandedViewPos(
        /* [in] */ Int32 thingy);

    // called by makeStatusbar and also by PhoneStatusBarView
    CARAPI UpdateDisplaySize();

    CARAPI StartActivityDismissingKeyguard(
        /* [in] */ IIntent* intent,
        /* [in] */ Boolean onlyProvisioned,
        /* [in] */ Boolean dismissShade);

    // @Override
    CARAPI UserSwitched(
        /* [in] */ Int32 newUserId);

    CARAPI OnHeadsUpDismissed();

    /**
     * Reload some of our resources when the configuration changes.
     *
     * We don't reload everything when the configuration changes -- we probably
     * should, but getting that smooth is tough.  Someday we'll fix that.  In the
     * meantime, just update the things that we know change.
     */
    CARAPI UpdateResources();

    //
    // tracing
    //
    CARAPI PostStartTracing();

    CARAPI Vibrate();

    CARAPI PostStartSettingsActivity(
        /* [in] */ IIntent* intent,
        /* [in] */ Int32 delay);

    // @Override
    CARAPI Destroy();

    // @Override
    CARAPI DispatchDemoCommand(
        /* [in] */ const String& command,
        /* [in] */ IBundle* args);

    /**
     * @return The {@link StatusBarState} the status bar is in.
     */
    CARAPI GetBarState(
        /* [out] */ Int32* state);

    CARAPI ShowKeyguard();

    CARAPI IsCollapsing(
        /* [out] */ Boolean* result);

    CARAPI AddPostCollapseAction(
        /* [in] */ IRunnable* r);

    CARAPI IsInLaunchTransition(
        /* [out] */ Boolean* result);

    /**
     * Fades the content of the keyguard away after the launch transition is done.
     *
     * @param beforeFading the runnable to be run when the circle is fully expanded and the fading
     *                     starts
     * @param endRunnable the runnable to be run when the transition is done
     */
    CARAPI FadeKeyguardAfterLaunchTransition(
        /* [in] */ IRunnable* beforeFading,
        /* [in] */ IRunnable* endRunnable);

    /**
     * @return TRUE if we would like to stay in the shade, FALSE if it should go away entirely
     */
    CARAPI HideKeyguard(
        /* [out] */ Boolean* result);

    CARAPI CalculateGoingToFullShadeDelay(
        /* [out] */ Int64* result);

    /**
     * Notifies the status bar the Keyguard is fading away with the specified timings.
     *
     * @param delay the animation delay in miliseconds
     * @param fadeoutDuration the duration of the exit animation, in milliseconds
     */
    CARAPI SetKeyguardFadingAway(
        /* [in] */ Int64 delay,
        /* [in] */ Int64 fadeoutDuration);

    CARAPI IsKeyguardFadingAway(
        /* [out] */ Boolean* result);

    /**
     * Notifies that the Keyguard fading away animation is done.
     */
    CARAPI FinishKeyguardFadingAway();

    CARAPI UpdateStackScrollerState(
        /* [in] */ Boolean goingToFullShade);

    CARAPI UserActivity();

    CARAPI InterceptMediaKey(
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* result);

    CARAPI OnMenuPressed(
        /* [out] */ Boolean* result);

    CARAPI OnBackPressed(
        /* [out] */ Boolean* result);

    CARAPI OnSpacePressed(
        /* [out] */ Boolean* result);

    // @Override
    CARAPI OnActivated(
        /* [in] */ IActivatableNotificationView* view);

    /**
     * @param state The {@link StatusBarState} to set.
     */
    CARAPI SetBarState(
        /* [in] */ Int32 state);

    // @Override
    CARAPI OnActivationReset(
        /* [in] */ IActivatableNotificationView* view);

    CARAPI OnTrackingStarted();

    CARAPI OnUnlockHintStarted();

    CARAPI OnHintFinished();

    CARAPI OnCameraHintStarted();

    CARAPI OnPhoneHintStarted();

    CARAPI OnTrackingStopped(
        /* [in] */ Boolean expand);

    CARAPI GetNavigationBarView(
        /* [out] */ INavigationBarView** view);

    // ---------------------- DragDownHelper.OnDragDownListener ------------------------------------
    // @Override
    CARAPI OnDraggedDown(
        /* [in] */ IView* startingChild,
        /* [out] */ Boolean* result);

    // @Override
    CARAPI OnDragDownReset();

    // @Override
    CARAPI OnThresholdReached();

    // @Override
    CARAPI OnTouchSlopExceeded();

    // @Override
    CARAPI SetEmptyDragAmount(
        /* [in] */ Float amount);

    /**
     * If secure with redaction: Show bouncer, go to unlocked shade.
     *
     * <p>If secure without redaction or no security: Go to {@link StatusBarState#SHADE_LOCKED}.</p>
     *
     * @param expandView The view to expand after going to the shade.
     */
    CARAPI GoToLockedShade(
        /* [in] */ IView* expandView);

    /**
     * Goes back to the keyguard after hanging around in {@link StatusBarState#SHADE_LOCKED}.
     */
    CARAPI GoToKeyguard();

    /**
     * @return a ViewGroup that spans the entire panel which contains the quick settings
     */
    CARAPI GetQuickSettingsOverlayParent(
        /* [out] */ IViewGroup** parent);

    CARAPI GetKeyguardFadingAwayDelay(
        /* [out] */ Int64* result);

    CARAPI GetKeyguardFadingAwayDuration(
        /* [out] */ Int64* result);

    CARAPI GetSystemIcons(
        /* [out] */ ILinearLayout** result);

    CARAPI GetSystemIconArea(
        /* [out] */ ILinearLayout** result);

    // @Override
    CARAPI SetBouncerShowing(
        /* [in] */ Boolean bouncerShowing);

    CARAPI OnScreenTurnedOff();

    CARAPI OnScreenTurnedOn();

    // @Override
    CARAPI OnVisibilityChanged(
        /* [in] */ Boolean visible);

    CARAPI HasActiveNotifications(
        /* [out] */ Boolean* result);

    CARAPI WakeUpIfDozing(
        /* [in] */ Int64 time,
        /* [in] */ Boolean fromTouch);

    static CARAPI_(String) ViewInfo(
        /* [in] */ IView* v);

protected:
    // ================================================================================
    // Constructing the view
    // ================================================================================
    CARAPI_(AutoPtr<IPhoneStatusBarView>) MakeStatusBarView();

    // @Override
    CARAPI_(void) SetZenMode(
        /* [in] */ Int32 mode);

    // @Override
    CARAPI_(AutoPtr<IView>) GetStatusBarView();

    // @Override
    CARAPI_(AutoPtr<IWindowManagerLayoutParams>) GetSearchLayoutParams(
        /* [in] */ IViewGroupLayoutParams* layoutParams);

    // @Override
    CARAPI_(void) UpdateSearchPanel();

    // @Override
    CARAPI_(void) UpdateNotificationRanking(
        /* [in] */ INotificationListenerServiceRankingMap* ranking);

    // @Override
    CARAPI_(void) RefreshLayout(
        /* [in] */ Int32 layoutDirection);

    // @Override
    CARAPI_(void) UpdateNotifications();

    // @Override
    CARAPI_(void) UpdateRowStates();

    CARAPI_(void) UpdateCarrierLabelVisibility(
        /* [in] */ Boolean force);

    // @Override
    CARAPI_(void) SetAreThereNotifications();

    // @Override
    CARAPI_(AutoPtr<BaseStatusBar::H>) CreateHandler();

    // @Override
    CARAPI_(void) Tick(
        /* [in] */ IStatusBarNotification* n,
        /* [in] */ Boolean firstTime);

    // @Override
    CARAPI_(void) DismissKeyguardThenExecute(
        /* [in] */ IOnDismissAction* action,
        /* [in] */ Boolean afterKeyguardGone);

    // SystemUIService notifies SystemBars of configuration changes, which then calls down here
    // @Override
    CARAPI OnConfigurationChanged(
        /* [in] */ IConfiguration* newConfig);

    CARAPI_(void) LoadDimens();

    // Visibility reporting
    // @Override
    CARAPI_(void) VisibilityChanged(
        /* [in] */ Boolean visible);

    // @Override
    CARAPI_(void) HaltTicker();

    // @Override
    CARAPI_(Boolean) ShouldDisableNavbarGestures();

    // @Override
    CARAPI_(Int32) GetMaxKeyguardNotifications();

    // Recents
    // @Override
    CARAPI_(void) ShowRecents(
        /* [in] */ Boolean triggeredFromAltTab);

    // @Override
    CARAPI_(void) HideRecents(
        /* [in] */ Boolean triggeredFromAltTab,
        /* [in] */ Boolean triggeredFromHomeKey);

    // @Override
    CARAPI_(void) ToggleRecents();


private:
    CARAPI_(void) ClearAllNotifications();

    CARAPI_(void) PerformDismissAllAnimations(
        /* [in] */ IArrayList/*<View>*/* hideAnimatedList);

    /**
     * Hack to improve glyph rasterization for scaled text views.
     */
    CARAPI_(void) StartGlyphRasterizeHack();

    CARAPI_(void) StartKeyguard();

    CARAPI_(void) AwakenDreams();

    CARAPI_(void) PrepareNavigationBarView();

    // For small-screen devices (read: phones) that lack hardware navigation buttons
    CARAPI_(void) AddNavigationBar();

    CARAPI_(void) RepositionNavigationBar();

    CARAPI_(void) NotifyNavigationBarScreenOn(
        /* [in] */ Boolean screenOn);

    CARAPI_(AutoPtr<IWindowManagerLayoutParams>) GetNavigationBarLayoutParams();

    CARAPI_(void) AddHeadsUpView();

    CARAPI_(void) RemoveHeadsUpView();

    CARAPI_(void) RefreshAllIconsForLayout(
        /* [in] */ ILinearLayout* ll);

    CARAPI_(void) UpdateShowSearchHoldoff();

    CARAPI_(void) UpdateNotificationShade();

    CARAPI_(Boolean) PackageHasVisibilityOverride(
        /* [in] */ const String& key);

    CARAPI_(void) UpdateClearAll();

    CARAPI_(void) UpdateEmptyShadeView();

    CARAPI_(void) UpdateSpeedbump();

    CARAPI_(void) UpdateNotificationIcons();

    CARAPI_(Boolean) SameSessions(
        /* [in] */ IMediaController* a,
        /* [in] */ IMediaController* b);

    CARAPI_(Int32) AdjustDisableFlags(
        /* [in] */ Int32 state);

    /**
     * Animates {@code v}, a view that is part of the status bar, out.
     */
    CARAPI_(void) AnimateStatusBarHide(
        /* [in] */ IView* v,
        /* [in] */ Boolean animate);

    /**
     * Animates {@code v}, a view that is part of the status bar, in.
     */
    CARAPI_(void) AnimateStatusBarShow(
        /* [in] */ IView* v,
        /* [in] */ Boolean animate);

    /**  if the interrupting notification had a fullscreen intent, fire it now.  */
    CARAPI_(void) EscalateHeadsUp();

    CARAPI_(void) RunPostCollapseRunnables();

    CARAPI_(void) SetNavigationIconHints(
        /* [in] */ Int32 hints);

    CARAPI_(Int32) ComputeBarMode(
        /* [in] */ Int32 oldVis,
        /* [in] */ Int32 newVis,
        /* [in] */ IBarTransitions* transitions,
        /* [in] */ Int32 transientFlag,
        /* [in] */ Int32 translucentFlag);

    CARAPI_(Int32) BarMode(
        /* [in] */ Int32 vis,
        /* [in] */ Int32 transientFlag,
        /* [in] */ Int32 translucentFlag);

    CARAPI_(void) CheckBarModes();

    CARAPI_(void) CheckBarMode(
        /* [in] */ Int32 mode,
        /* [in] */ Int32 windowState,
        /* [in] */ IBarTransitions* transitions);

    CARAPI_(void) FinishBarAnimations();

    CARAPI_(void) ResumeSuspendedAutohide();

    CARAPI_(void) SuspendAutohide();

    CARAPI_(void) CancelAutohide();

    CARAPI_(void) ScheduleAutohide();

    CARAPI_(void) CheckUserAutohide(
        /* [in] */ IView* v,
        /* [in] */ IMotionEvent* event);

    CARAPI_(void) UserAutohide();

    CARAPI_(Boolean) AreLightsOn();

    CARAPI_(void) NotifyUiVisibilityChanged(
        /* [in] */ Int32 vis);

    CARAPI_(AutoPtr<IAnimation>) LoadAnim(
        /* [in] */ Int32 id,
        /* [in] */ IAnimationAnimationListener* listener);

    CARAPI_(String) HunStateToString(
        /* [in] */ INotificationDataEntry* entry);

    static CARAPI_(void) DumpBarTransitions(
        /* [in] */ IPrintWriter* pw,
        /* [in] */ const String& var,
        /* [in] */ IBarTransitions* transitions);

    CARAPI_(void) AddStatusBarWindow();

    CARAPI_(void) ResetUserExpandedStates();

    CARAPI_(void) SetControllerUsers();

    CARAPI_(void) ResetUserSetupObserver();

    CARAPI_(void) SetHeadsUpVisibility(
        /* [in] */ Boolean vis);

    CARAPI_(void) UpdateClockSize();

    CARAPI_(void) StopNotificationLogging();

    CARAPI_(void) StartNotificationLoggingIfScreenOnAndVisible();

    CARAPI_(void) LogNotificationVisibilityChanges(
        /* [in] */ ICollection/*<String>*/* newlyVisible,
        /* [in] */ ICollection/*<String>*/* noLongerVisible);

    CARAPI_(void) HandleStartSettingsActivity(
        /* [in] */ IIntent* intent,
        /* [in] */ Boolean onlyProvisioned);

    CARAPI_(void) DispatchDemoCommandToView(
        /* [in] */ const String& command,
        /* [in] */ IBundle* args,
        /* [in] */ Int32 id);

    CARAPI_(void) UpdatePublicMode();

    CARAPI_(void) UpdateKeyguardState(
        /* [in] */ Boolean goingToFullShade,
        /* [in] */ Boolean fromShadeLocked);

    CARAPI_(void) UpdateDozingState();

    CARAPI_(void) ShowBouncer();

    CARAPI_(void) InstantExpandNotificationsPanel();

    CARAPI_(void) InstantCollapseNotificationPanel();

    /**
     * This handles Int64-press of both back and recents.  They are
     * handled together to capture them both being Int64-pressed
     * at the same time to exit screen pinning (lock task).
     *
     * When accessibility mode is on, only a Int64-press from recents
     * is required to exit.
     *
     * In all other circumstances we try to pass through Int64-press events
     * for Back, so that apps can still use it.  Which can be from two things.
     * 1) Not currently in screen pinning (lock task).
     * 2) Back is Int64-pressed without recents.
     */
    CARAPI_(void) HandleLongPressBackRecents(
        /* [in] */ IView* v);

    static CARAPI_(Boolean) InitStatic();

public:
    static const String TAG;
    static const Boolean DEBUG;
    static const Boolean SPEW;
    static const Boolean DUMPTRUCK;
    static const Boolean DEBUG_GESTURES;
    static const Boolean DEBUG_MEDIA;
    static const Boolean DEBUG_MEDIA_FAKE_ARTWORK;

    static const Boolean DEBUG_WINDOW_STATE;

    // additional instrumentation for testing purposes; intended to be left on during development
    static const Boolean CHATTY;

    static const String ACTION_STATUSBAR_START;

    static const Boolean SHOW_LOCKSCREEN_MEDIA_ARTWORK;

    static const Int32 FADE_KEYGUARD_START_DELAY;
    static const Int32 FADE_KEYGUARD_DURATION;
    static Boolean sInit;
    static AutoPtr<IInterpolator> ALPHA_IN;
    static AutoPtr<IInterpolator> ALPHA_OUT;

private:
    static const Int32 MSG_OPEN_NOTIFICATION_PANEL;
    static const Int32 MSG_CLOSE_PANELS;
    static const Int32 MSG_OPEN_SETTINGS_PANEL;
    // 1020-1040 reserved for BaseStatusBar

    static const Boolean CLOSE_PANEL_WHEN_EMPTIED;

    static const Int32 NOTIFICATION_PRIORITY_MULTIPLIER; // see NotificationManagerService
    static const Int32 HIDE_ICONS_BELOW_SCORE;

    static const Int32 STATUS_OR_NAV_TRANSIENT;
    static const Int64 AUTOHIDE_TIMEOUT_MS;

    /** The minimum delay in ms between reports of notification visibility. */
    static const Int32 VISIBILITY_REPORT_MIN_DELAY_MS;

    /**
     * The delay to reset the hint text when the hint animation is finished running.
     */
    static const Int32 HINT_RESET_DELAY_MS;

    static AutoPtr<IAudioAttributes> VIBRATION_ATTRIBUTES;

    /** Allow some time inbetween the Int64 press for back and recents. */
    static const Int32 LOCK_TO_APP_GESTURE_TOLERENCE;

    AutoPtr<PhoneStatusBarPolicy> mIconPolicy;

    // These are no longer handled by the policy, because we need custom strategies for them
    AutoPtr<BluetoothControllerImpl> mBluetoothController;
    AutoPtr<SecurityControllerImpl> mSecurityController;
    AutoPtr<BatteryController> mBatteryController;
    AutoPtr<LocationControllerImpl> mLocationController;
    AutoPtr<NetworkControllerImpl> mNetworkController;
    AutoPtr<HotspotControllerImpl> mHotspotController;
    AutoPtr<RotationLockControllerImpl> mRotationLockController;
    AutoPtr<UserInfoController> mUserInfoController;
    AutoPtr<IZenModeController> mZenModeController;
    AutoPtr<CastControllerImpl> mCastController;
    AutoPtr<IVolumeComponent> mVolumeComponent;
    AutoPtr<KeyguardUserSwitcher> mKeyguardUserSwitcher;
    AutoPtr<FlashlightController> mFlashlightController;
    AutoPtr<UserSwitcherController> mUserSwitcherController;
    AutoPtr<NextAlarmController> mNextAlarmController;
    AutoPtr<KeyguardMonitor> mKeyguardMonitor;
    AutoPtr<BrightnessMirrorController> mBrightnessMirrorController;
    AutoPtr<IAccessibilityController> mAccessibilityController;

    Int32 mNaturalBarHeight;
    Int32 mIconSize;
    Int32 mIconHPadding;
    AutoPtr<IDisplay> mDisplay;
    AutoPtr<IPoint> mCurrentDisplaySize;

    AutoPtr<IStatusBarWindowView> mStatusBarWindow;
    AutoPtr<IPhoneStatusBarView> mStatusBarView;
    Int32 mStatusBarWindowState;
    AutoPtr<StatusBarWindowManager> mStatusBarWindowManager;
    AutoPtr<UnlockMethodCache> mUnlockMethodCache;
    AutoPtr<DozeServiceHost> mDozeServiceHost;
    Boolean mScreenOnComingFromTouch;

    Int32 mPixelFormat;
    Object mQueueLock;

    // viewgroup containing the normal contents of the statusbar
    AutoPtr<ILinearLayout> mStatusBarContents;

    // right-hand icons
    AutoPtr<ILinearLayout> mSystemIconArea;
    AutoPtr<ILinearLayout> mSystemIcons;

    // left-hand icons
    AutoPtr<ILinearLayout> mStatusIcons;
    AutoPtr<ILinearLayout> mStatusIconsKeyguard;

    // the icons themselves
    AutoPtr<IIconMerger> mNotificationIcons;
    AutoPtr<IView> mNotificationIconArea;

    // [+>
    AutoPtr<IView> mMoreIcon;

    // expanded notifications
    AutoPtr<INotificationPanelView> mNotificationPanel; // the sliding/resizing panel within the notification window
    AutoPtr<IView> mExpandedContents;
    Int32 mNotificationPanelGravity;
    Int32 mNotificationPanelMarginBottomPx;
    Float mNotificationPanelMinHeightFrac;
    AutoPtr<ITextView> mNotificationPanelDebugText;

    // settings
    AutoPtr<IView> mFlipSettingsView;
    AutoPtr<IQSPanel> mQSPanel;

    // top bar
    AutoPtr<IStatusBarHeaderView> mHeader;
    AutoPtr<IKeyguardStatusBarView> mKeyguardStatusBar;
    AutoPtr<IView> mKeyguardStatusView;
    AutoPtr<IKeyguardBottomAreaView> mKeyguardBottomArea;
    Boolean mLeaveOpenOnKeyguardHide;
    AutoPtr<KeyguardIndicationController> mKeyguardIndicationController;

    Boolean mKeyguardFadingAway;
    Int64 mKeyguardFadingAwayDelay;
    Int64 mKeyguardFadingAwayDuration;

    Int32 mKeyguardMaxNotificationCount;

    // carrier/wifi label
    AutoPtr<ITextView> mCarrierLabel;
    Boolean mCarrierLabelVisible;
    Int32 mCarrierLabelHeight;
    Int32 mStatusBarHeaderHeight;

    Boolean mShowCarrierInPanel;

    // position
    AutoPtr<ArrayOf<Int32> > mPositionTmp;
    Boolean mExpandedVisible;

    Int32 mNavigationBarWindowState;

    // the tracker view
    Int32 mTrackingPosition; // the position of the top of the tracking view.

    // ticker
    Boolean mTickerEnabled;
    AutoPtr<Ticker> mTicker;
    AutoPtr<IView> mTickerView;
    Boolean mTicking;

    // Tracking finger for opening/closing.
    Int32 mEdgeBorder; // corresponds to R.dimen.status_bar_edge_ignore
    Boolean mTracking;
    AutoPtr<IVelocityTracker> mVelocityTracker;

    AutoPtr<ArrayOf<Int32> > mAbsPos;
    AutoPtr<IArrayList/*<Runnable*/> mPostCollapseRunnables;

    // for disabling the status bar
    Int32 mDisabled;

    // tracking calls to View.setSystemUiVisibility()
    Int32 mSystemUiVisibility;

    AutoPtr<IDisplayMetrics> mDisplayMetrics;

    // XXX: gesture research
    AutoPtr<GestureRecorder> mGestureRec;

    Int32 mNavigationIconHints;

    // ensure quick settings is disabled until the current user makes it through the setup wizard
    Boolean mUserSetup;
    AutoPtr<IContentObserver> mUserSetupObserver;

    AutoPtr<IContentObserver> mHeadsUpObserver;

    Int32 mInteractingWindows;
    Boolean mAutohideSuspended;
    Int32 mStatusBarMode;
    Int32 mNavigationBarMode;
    Boolean mScreenOn;

    // The second field is a bit different from the first one because it only listens to screen on/
    // screen of events from Keyguard. We need this so we don't have a race condition with the
    // broadcast. In the future, we should remove the first field altogether and rename the second
    // field.
    Boolean mScreenOnFromKeyguard;

    AutoPtr<IViewMediatorCallback> mKeyguardViewMediatorCallback;
    AutoPtr<ScrimController> mScrimController;

    AutoPtr<IRunnable> mAutohide;

    Boolean mVisible;
    Boolean mWaitingForKeyguardExit;
    Boolean mDozing;
    Boolean mScrimSrcModeEnabled;

    AutoPtr<IInterpolator> mLinearOutSlowIn;
    AutoPtr<IInterpolator> mLinearInterpolator;
    AutoPtr<IInterpolator> mBackdropInterpolator;

    AutoPtr<IBackDropView> mBackdrop;
    AutoPtr<IImageView> mBackdropFront;
    AutoPtr<IImageView> mBackdropBack;
    AutoPtr<IPorterDuffXfermode> mSrcXferMode;
    AutoPtr<IPorterDuffXfermode> mSrcOverXferMode;

    AutoPtr<IMediaSessionManager> mMediaSessionManager;
    AutoPtr<IMediaController> mMediaController;
    String mMediaNotificationKey;
    AutoPtr<IMediaMetadata> mMediaMetadata;
    AutoPtr<IMediaControllerCallback> mMediaListener;

    AutoPtr<IOnChildLocationsChangedListener> mOnChildLocationsChangedListener;

    Int32 mDisabledUnmodified;

    /** Keys of notifications currently visible to the user. */
    AutoPtr<IArraySet/*<String*/> mCurrentlyVisibleNotifications;
    Int64 mLastVisibilityReportUptimeMs;

    AutoPtr<ShadeUpdates> mShadeUpdates;

    Int32 mDrawCount;
    AutoPtr<IRunnable> mLaunchTransitionEndRunnable;
    Boolean mLaunchTransitionFadingAway;
    AutoPtr<IExpandableNotificationRow> mDraggedDownRow;

    static const Int32 VISIBLE_LOCATIONS;

    AutoPtr<IOnChildLocationsChangedListener> mNotificationLocationsChangedListener;

    // Tracks notifications currently visible in mNotificationStackScroller and
    // emits visibility events via NoMan on changes.
    AutoPtr<IRunnable> mVisibilityReporter;

    AutoPtr<IViewOnClickListener> mOverflowClickListener;

    AutoPtr<IViewOnClickListener> mRecentsClickListener;

    Int64 mLastLockToAppLongPress;
    AutoPtr<IViewOnLongClickListener> mLongPressBackRecentsListener;

    Int32 mShowSearchHoldoff;
    AutoPtr<IRunnable> mShowSearchPanel;

    AutoPtr<IViewOnTouchListener> mHomeActionListener;

    /**
     * Hide the album artwork that is fading out and release its bitmap.
     */
    AutoPtr<IRunnable> mHideBackdropFront;

    AutoPtr<IViewOnFocusChangeListener> mFocusChangeListener;

    AutoPtr<IRunnable> mAnimateCollapsePanels;

    AutoPtr<ITimeInterpolator> mAccelerateInterpolator;
    AutoPtr<ITimeInterpolator> mDecelerateInterpolator;
    const Int32 FLIP_DURATION_OUT;
    const Int32 FLIP_DURATION_IN;
    const Int32 FLIP_DURATION;

    AutoPtr<IAnimator> mScrollViewAnim;
    AutoPtr<IAnimator> mClearButtonAnim;

    AutoPtr<IRunnable> mCheckBarModes;

    AutoPtr<IAnimationAnimationListener> mTickingDoneListener;

    AutoPtr<IBroadcastReceiver> mBroadcastReceiver;

    AutoPtr<IRunnable> mStartTracing;

    AutoPtr<IRunnable> mStopTracing;
    Boolean mDemoModeAllowed;
    Boolean mDemoMode;
    AutoPtr<DemoStatusIcons> mDemoStatusIcons;
};

}// namespace Phone
}// namespace StatusBar
}// namespace SystemUI
}// namespace Droid
}// namespace Elastos

#endif //__ELASTOS_DROID_SYSTEMUI_STATUSBAR_PHONE_PHONESTATUSBAR_H__
