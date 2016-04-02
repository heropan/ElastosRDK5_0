#ifndef __ELASTOS_DROID_SYSTEMUI_STATUSBAR_TV_TVSTATUSBAR_H__
#define __ELASTOS_DROID_SYSTEMUI_STATUSBAR_TV_TVSTATUSBAR_H__

#include "_SystemUI.h"
#include "elastos/droid/systemui/statusbar/BaseStatusBar.h"
#include "Elastos.Droid.Internal.h"

namespace Elastos {
namespace Droid {
namespace SystemUI {
namespace StatusBar {
namespace TV {

/*
 * Status bar implementation for "large screen" products that mostly present no on-screen nav
 */
class TvStatusBar
    : public BaseStatusBar
    , public ITvStatusBar
{
public:
    CAR_INTERFACE_DECL();

    // @Override
    CARAPI AddIcon(
        /* [in] */ const String& slot,
        /* [in] */ Int32 index,
        /* [in] */ Int32 viewIndex,
        /* [in] */ IStatusBarIcon* icon);

    // @Override
    CARAPI UpdateIcon(
        /* [in] */ const String& slot,
        /* [in] */ Int32 index,
        /* [in] */ Int32 viewIndex,
        /* [in] */ IStatusBarIcon* old,
        /* [in] */ IStatusBarIcon* icon);

    // @Override
    CARAPI RemoveIcon(
        /* [in] */ const String& slot,
        /* [in] */ Int32 index,
        /* [in] */ Int32 viewIndex);

    // @Override
    CARAPI AddNotification(
        /* [in] */ IStatusBarNotification* notification,
        /* [in] */ INotificationListenerServiceRankingMap* ranking);

    // @Override
    CARAPI RemoveNotification(
        /* [in] */ const String& key,
        /* [in] */ INotificationListenerServiceRankingMap* ranking);

    // @Override
    CARAPI Disable(
        /* [in] */ Int32 state,
        /* [in] */ Boolean animate);

    // @Override
    CARAPI AnimateExpandNotificationsPanel();

    // @Override
    CARAPI AnimateCollapsePanels(
        /* [in] */ Int32 flags);

    // @Override
    CARAPI SetSystemUiVisibility(
        /* [in] */ Int32 vis,
        /* [in] */ Int32 mask);

    // @Override
    CARAPI TopAppWindowChanged(
        /* [in] */ Boolean visible);

    // @Override
    CARAPI SetImeWindowStatus(
        /* [in] */ IBinder* token,
        /* [in] */ Int32 vis,
        /* [in] */ Int32 backDisposition,
        /* [in] */ Boolean showImeSwitcher);

    // @Override
    CARAPI ToggleRecentApps();

    // @Override // CommandQueue
    CARAPI SetWindowState(
        /* [in] */ Int32 window,
        /* [in] */ Int32 state);

    // @Override // CommandQueue
    CARAPI BuzzBeepBlinked();

    // @Override // CommandQueue
    CARAPI NotificationLightOff();

    // @Override // CommandQueue
    CARAPI NotificationLightPulse(
        /* [in] */ Int32 argb,
        /* [in] */ Int32 onMillis,
        /* [in] */ Int32 offMillis);

    CARAPI GetStatusBarView(
        /* [out] */ IView** view);

    // @Override
    CARAPI ResetHeadsUpDecayTimer();

    // @Override
    CARAPI ScheduleHeadsUpOpen();

    // @Override
    CARAPI ScheduleHeadsUpEscalation();

    // @Override
    CARAPI ScheduleHeadsUpClose();

    // @Override
    CARAPI AnimateExpandSettingsPanel();

    // @Override
    CARAPI OnActivated(
        /* [in] */ IActivatableNotificationView* view);

    // @Override
    CARAPI OnActivationReset(
        /* [in] */ IActivatableNotificationView* view);

protected:
    // @Override
    CARAPI UpdateNotificationRanking(
        /* [in] */ INotificationListenerServiceRankingMap* ranking);

    // @Override
    CARAPI_(AutoPtr<IWindowManagerLayoutParams>) GetSearchLayoutParams(
        /* [in] */ IViewGroupLayoutParams* layoutParams);

    // @Override
    CARAPI_(void) HaltTicker();

    // @Override
    CARAPI_(void) SetAreThereNotifications();

    // @Override
    CARAPI_(void) UpdateNotifications();

    // @Override
    CARAPI_(void) Tick(
        /* [in] */ IStatusBarNotification* n,
        /* [in] */ Boolean firstTime);

    // @Override
    CARAPI_(void) UpdateExpandedViewPos(
        /* [in] */ Int32 expandedPosition);

    // @Override
    CARAPI_(Boolean) ShouldDisableNavbarGestures();

    // @Override
    CARAPI_(Int32) GetMaxKeyguardNotifications();

    // @Override
    CARAPI_(void) CreateAndAddWindows();

    // @Override
    CARAPI_(void) RefreshLayout(
        /* [in] */ Int32 layoutDirection);
};

} // namespace TV
} // namespace StatusBar
} // namespace SystemUI
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_SYSTEMUI_STATUSBAR_TV_TVSTATUSBAR_H__
