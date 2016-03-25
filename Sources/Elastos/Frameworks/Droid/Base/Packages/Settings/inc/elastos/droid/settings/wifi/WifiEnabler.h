#ifndef __ELASTOS_DROID_SETTINGS_WIFI_WIFIENABLER_H__
#define __ELASTOS_DROID_SETTINGS_WIFI_WIFIENABLER_H__

#include <Elastos.Droid.Wifi.h>
#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/content/BroadcastReceiver.h"
#include "elastos/droid/settings/widget/SwitchBar.h"
#include "elastos/droid/os/Handler.h"
#include <elastos/core/Object.h>
#include "_Settings.h"

using Elastos::Droid::Content::BroadcastReceiver;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::IIntentFilter;
using Elastos::Droid::Net::NetworkInfoDetailedState;
using Elastos::Droid::Os::Handler;
using Elastos::Droid::Os::IMessage;
using Elastos::Droid::Settings::Widget::ISwitchBarOnSwitchChangeListener;
using Elastos::Droid::Settings::Widget::SwitchBar;
using Elastos::Droid::Widget::ISwitch;
using Elastos::Droid::Wifi::IWifiManager;
using Elastos::Utility::Concurrent::Atomic::IAtomicBoolean;

namespace Elastos {
namespace Droid {
namespace Settings {
namespace Wifi {

class WifiEnabler
    : public Object
    , public ISwitchBarOnSwitchChangeListener
{
private:
    class InitBroadcastReceiver
        : public BroadcastReceiver
    {
    public:
        InitBroadcastReceiver(
            /* [in] */ WifiEnabler* host);

        ~InitBroadcastReceiver();

        //@Override
        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

    private:
        WifiEnabler* mHost;
    };

    class InitHandler
        : public Handler
    {
    public:
        InitHandler(
            /* [in] */ WifiEnabler* host);

        ~InitHandler();

        //@Override
        CARAPI HandleMessage(
            /* [in] */ IMessage* msg);

    private:
        WifiEnabler* mHost;
    };

public:
    CAR_INTERFACE_DECL();

    WifiEnabler(
        /* [in] */ IContext* context,
        /* [in] */ SwitchBar* switchBar);

    ~WifiEnabler();

    CARAPI SetupSwitchBar();

    CARAPI TeardownSwitchBar();

    CARAPI Resume(
        /* [in] */ IContext* context);

    CARAPI Pause();

    //@Override
    CARAPI OnSwitchChanged(
        /* [in] */ ISwitch* switchView,
        /* [in] */ Boolean isChecked);

private:
    CARAPI_(void) HandleWifiStateChanged(
        /* [in] */ Int32 state);

    CARAPI_(void) UpdateSearchIndex(
        /* [in] */ Boolean isWiFiOn);

    CARAPI_(void) SetSwitchBarChecked(
        /* [in] */ Boolean checked);

    CARAPI_(void) HandleStateChanged(
        /* [in] */ NetworkInfoDetailedState state);

private:
    AutoPtr<IContext> mContext;
    AutoPtr<SwitchBar> mSwitchBar;
    Boolean mListeningToOnSwitchChange;
    AutoPtr<IAtomicBoolean> mConnected;

    AutoPtr<IWifiManager> mWifiManager;
    Boolean mStateMachineEvent;
    AutoPtr<IIntentFilter> mIntentFilter;
    AutoPtr<BroadcastReceiver> mReceiver;

    static const String EVENT_DATA_IS_WIFI_ON;
    static const Int32 EVENT_UPDATE_INDEX = 0;

    AutoPtr<Handler> mHandler;
};

} // namespace Wifi
} // namespace Settings
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_SETTINGS_WIFI_WIFIENABLER_H__
