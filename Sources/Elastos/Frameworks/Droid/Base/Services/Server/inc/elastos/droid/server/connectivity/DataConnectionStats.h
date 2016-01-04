
#ifndef __ELASTOS_DROID_SERVER_CONNECTIVITY_DATA_CONNECTION_STATS_H__
#define __ELASTOS_DROID_SERVER_CONNECTIVITY_DATA_CONNECTION_STATS_H__

#include <elastos/droid/content/BroadcastReceiver.h>
// #include "elastos/droid/server/am/CBatteryStatsService.h"
// #include <Elastos.Droid.Telephony.h>
#include <Elastos.Droid.Internal.h>

using Elastos::Droid::Content::BroadcastReceiver;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::IIntentFilter;
using Elastos::Droid::Net::IConnectivityManager;
using Elastos::Droid::Telephony::IPhoneStateListener;
using Elastos::Droid::Telephony::IServiceState;
using Elastos::Droid::Telephony::ISignalStrength;
using Elastos::Droid::Telephony::ITelephonyManager;
using Elastos::Droid::Internal::App::IIBatteryStats;
using Elastos::Droid::Internal::Telephony::IccCardConstantsState;
using Elastos::Droid::Internal::Telephony::IIccCardConstants;
using Elastos::Droid::Internal::Telephony::ITelephonyIntents;
// using Elastos::Droid::Server::Am::CBatteryStatsService;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Connectivity {

class DataConnectionStats
    : public BroadcastReceiver
{
private:
    class MyPhoneStateListener
        : public Object
        , public IPhoneStateListener
    {
    public:
        CAR_INTERFACE_DECL()

        MyPhoneStateListener(
            /* [in] */ DataConnectionStats* host);

        //@Override
        CARAPI OnSignalStrengthsChanged(
            /* [in] */ ISignalStrength* signalStrength);

        //@Override
        CARAPI OnServiceStateChanged(
            /* [in] */ IServiceState state);

        //@Override
        CARAPI OnDataConnectionStateChanged(
            /* [in] */ Int32 state,
            /* [in] */ Int32 networkType);

        //@Override
        CARAPI OnDataActivity(
            /* [in] */ Int32 direction);
    private:
        DataConnectionStats* mHost;
    };

public:

    DataConnectionStats(
        /* [in] */ IContext* context);

    CARAPI StartMonitoring();

    //@Override
    CARAPI OnReceive(
        /* [in] */ IContext* context,
        /* [in] */ IIntent* intent);

private:
     void NotePhoneDataConnectionState();

    void UpdateSimState(
        /* [in] */ IIntent* intent);

    Boolean IsCdma();

    Boolean HasService();

private:
    static const String TAG;
    static const Boolean DEBUG;

    AutoPtr<IContext> mContext;
    AutoPtr<IIBatteryStats> mBatteryStats;

    IccCardConstantsState mSimState;
    AutoPtr<ISignalStrength> mSignalStrength;
    AutoPtr<IServiceState> mServiceState;
    Int32 mDataState;

    AutoPtr<IPhoneStateListener> mPhoneStateListener;
};

} // Connectivity
} // Server
} // Droid
} // Elastos

#endif //__ELASTOS_DROID_SERVER_CONNECTIVITY_DATA_CONNECTION_STATS_H__
