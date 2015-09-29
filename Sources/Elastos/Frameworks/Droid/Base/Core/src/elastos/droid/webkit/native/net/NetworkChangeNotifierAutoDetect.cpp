
#include "elastos/droid/webkit/native/net/NetworkChangeNotifierAutoDetect.h"

using Elastos::Droid::Content::EIID_IBroadcastReceiver;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Net {

CAR_INTERFACE_IMPL(NetworkChangeNotifierAutoDetect, BroadcastReceiver, IBroadcastReceiver);

//=====================================================================
//     NetworkChangeNotifierAutoDetect::ConnectivityManagerDelegate
//=====================================================================
NetworkChangeNotifierAutoDetect::ConnectivityManagerDelegate::ConnectivityManagerDelegate(
    /* [in] */ IContext* context)
{
    // ==================before translated======================
    // mConnectivityManager =
    //         (ConnectivityManager) context.getSystemService(Context.CONNECTIVITY_SERVICE);
}

NetworkChangeNotifierAutoDetect::ConnectivityManagerDelegate::ConnectivityManagerDelegate()
{
    // ==================before translated======================
    // // All the methods below should be overridden.
    // mConnectivityManager = null;
}

Boolean NetworkChangeNotifierAutoDetect::ConnectivityManagerDelegate::ActiveNetworkExists()
{
    // ==================before translated======================
    // return mConnectivityManager.getActiveNetworkInfo() != null;
    assert(0);
    return FALSE;
}

Boolean NetworkChangeNotifierAutoDetect::ConnectivityManagerDelegate::IsConnected()
{
    // ==================before translated======================
    // return mConnectivityManager.getActiveNetworkInfo().isConnected();
    assert(0);
    return FALSE;
}

Int32 NetworkChangeNotifierAutoDetect::ConnectivityManagerDelegate::GetNetworkType()
{
    // ==================before translated======================
    // return mConnectivityManager.getActiveNetworkInfo().getType();
    assert(0);
    return 0;
}

Int32 NetworkChangeNotifierAutoDetect::ConnectivityManagerDelegate::GetNetworkSubtype()
{
    // ==================before translated======================
    // return mConnectivityManager.getActiveNetworkInfo().getSubtype();
    assert(0);
    return 0;
}

//=====================================================================
//         NetworkChangeNotifierAutoDetect::WifiManagerDelegate
//=====================================================================
NetworkChangeNotifierAutoDetect::WifiManagerDelegate::WifiManagerDelegate(
    /* [in] */ IContext* context)
{
    // ==================before translated======================
    // mWifiManager = (WifiManager) context.getSystemService(Context.WIFI_SERVICE);
}

NetworkChangeNotifierAutoDetect::WifiManagerDelegate::WifiManagerDelegate()
{
    // ==================before translated======================
    // // All the methods below should be overridden.
    // mWifiManager = null;
}

String NetworkChangeNotifierAutoDetect::WifiManagerDelegate::GetWifiSSID()
{
    // ==================before translated======================
    // WifiInfo wifiInfo = mWifiManager.getConnectionInfo();
    // if (wifiInfo == null)
    //     return "";
    // String ssid = wifiInfo.getSSID();
    // return ssid == null ? "" : ssid;
    assert(0);
    return String("");
}

//=====================================================================
//   NetworkChangeNotifierAutoDetect::NetworkConnectivityIntentFilter
//=====================================================================
NetworkChangeNotifierAutoDetect::NetworkConnectivityIntentFilter::NetworkConnectivityIntentFilter()
{
    // ==================before translated======================
    // addAction(ConnectivityManager.CONNECTIVITY_ACTION);
}

//=====================================================================
//                   NetworkChangeNotifierAutoDetect
//=====================================================================
const String NetworkChangeNotifierAutoDetect::TAG("NetworkChangeNotifierAutoDetect");

NetworkChangeNotifierAutoDetect::NetworkChangeNotifierAutoDetect(
    /* [in] */ Observer* observer,
    /* [in] */ IContext* context)
{
    // ==================before translated======================
    // mObserver = observer;
    // mContext = context.getApplicationContext();
    // mConnectivityManagerDelegate = new ConnectivityManagerDelegate(context);
    // mWifiManagerDelegate = new WifiManagerDelegate(context);
    // mConnectionType = getCurrentConnectionType();
    // mWifiSSID = getCurrentWifiSSID();
    // ApplicationStatus.registerApplicationStateListener(this);
}

ECode NetworkChangeNotifierAutoDetect::SetConnectivityManagerDelegateForTests(
    /* [in] */ ConnectivityManagerDelegate* delegate)
{
    VALIDATE_NOT_NULL(delegate);
    // ==================before translated======================
    // mConnectivityManagerDelegate = delegate;
    assert(0);
    return NOERROR;
}

ECode NetworkChangeNotifierAutoDetect::SetWifiManagerDelegateForTests(
    /* [in] */ WifiManagerDelegate* delegate)
{
    VALIDATE_NOT_NULL(delegate);
    // ==================before translated======================
    // mWifiManagerDelegate = delegate;
    assert(0);
    return NOERROR;
}

ECode NetworkChangeNotifierAutoDetect::Destroy()
{
    // ==================before translated======================
    // unregisterReceiver();
    assert(0);
    return NOERROR;
}

Int32 NetworkChangeNotifierAutoDetect::GetCurrentConnectionType()
{
    // ==================before translated======================
    // // Track exactly what type of connection we have.
    // if (!mConnectivityManagerDelegate.activeNetworkExists() ||
    //         !mConnectivityManagerDelegate.isConnected()) {
    //     return NetworkChangeNotifier.CONNECTION_NONE;
    // }
    //
    // switch (mConnectivityManagerDelegate.getNetworkType()) {
    //     case ConnectivityManager.TYPE_ETHERNET:
    //         return NetworkChangeNotifier.CONNECTION_ETHERNET;
    //     case ConnectivityManager.TYPE_WIFI:
    //         return NetworkChangeNotifier.CONNECTION_WIFI;
    //     case ConnectivityManager.TYPE_WIMAX:
    //         return NetworkChangeNotifier.CONNECTION_4G;
    //     case ConnectivityManager.TYPE_BLUETOOTH:
    //         return NetworkChangeNotifier.CONNECTION_BLUETOOTH;
    //     case ConnectivityManager.TYPE_MOBILE:
    //         // Use information from TelephonyManager to classify the connection.
    //         switch (mConnectivityManagerDelegate.getNetworkSubtype()) {
    //             case TelephonyManager.NETWORK_TYPE_GPRS:
    //             case TelephonyManager.NETWORK_TYPE_EDGE:
    //             case TelephonyManager.NETWORK_TYPE_CDMA:
    //             case TelephonyManager.NETWORK_TYPE_1xRTT:
    //             case TelephonyManager.NETWORK_TYPE_IDEN:
    //                 return NetworkChangeNotifier.CONNECTION_2G;
    //             case TelephonyManager.NETWORK_TYPE_UMTS:
    //             case TelephonyManager.NETWORK_TYPE_EVDO_0:
    //             case TelephonyManager.NETWORK_TYPE_EVDO_A:
    //             case TelephonyManager.NETWORK_TYPE_HSDPA:
    //             case TelephonyManager.NETWORK_TYPE_HSUPA:
    //             case TelephonyManager.NETWORK_TYPE_HSPA:
    //             case TelephonyManager.NETWORK_TYPE_EVDO_B:
    //             case TelephonyManager.NETWORK_TYPE_EHRPD:
    //             case TelephonyManager.NETWORK_TYPE_HSPAP:
    //                 return NetworkChangeNotifier.CONNECTION_3G;
    //             case TelephonyManager.NETWORK_TYPE_LTE:
    //                 return NetworkChangeNotifier.CONNECTION_4G;
    //             default:
    //                 return NetworkChangeNotifier.CONNECTION_UNKNOWN;
    //         }
    //     default:
    //         return NetworkChangeNotifier.CONNECTION_UNKNOWN;
    // }
    assert(0);
    return 0;
}

ECode NetworkChangeNotifierAutoDetect::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    VALIDATE_NOT_NULL(context);
    VALIDATE_NOT_NULL(intent);
    // ==================before translated======================
    // connectionTypeChanged();
    assert(0);
    return NOERROR;
}

// Inconsistent with ApplicationStatus.ApplicationStateListener, note it temporarily
//ECode NetworkChangeNotifierAutoDetect::OnApplicationStateChange(
//    /* [in] */ Int32 newState)
//{
    // ==================before translated======================
    // if (newState == ApplicationState.HAS_RUNNING_ACTIVITIES) {
    //     connectionTypeChanged();
    //     registerReceiver();
    // } else if (newState == ApplicationState.HAS_PAUSED_ACTIVITIES) {
    //     unregisterReceiver();
    // }
//    assert(0);
//    return NOERROR;
//}

ECode NetworkChangeNotifierAutoDetect::RegisterReceiver()
{
    // ==================before translated======================
    // if (!mRegistered) {
    //     mRegistered = true;
    //     mContext.registerReceiver(this, mIntentFilter);
    // }
    assert(0);
    return NOERROR;
}

ECode NetworkChangeNotifierAutoDetect::UnregisterReceiver()
{
    // ==================before translated======================
    // if (mRegistered) {
    //     mRegistered = false;
    //     mContext.unregisterReceiver(this);
    // }
    assert(0);
    return NOERROR;
}

String NetworkChangeNotifierAutoDetect::GetCurrentWifiSSID()
{
    // ==================before translated======================
    // if (getCurrentConnectionType() != NetworkChangeNotifier.CONNECTION_WIFI)
    //     return "";
    // return mWifiManagerDelegate.getWifiSSID();
    assert(0);
    return String("");
}

ECode NetworkChangeNotifierAutoDetect::ConnectionTypeChanged()
{
    // ==================before translated======================
    // int newConnectionType = getCurrentConnectionType();
    // String newWifiSSID = getCurrentWifiSSID();
    // if (newConnectionType == mConnectionType && newWifiSSID.equals(mWifiSSID))
    //     return;
    //
    // mConnectionType = newConnectionType;
    // mWifiSSID = newWifiSSID;
    // Log.d(TAG, "Network connectivity changed, type is: " + mConnectionType);
    // mObserver.onConnectionTypeChanged(newConnectionType);
    assert(0);
    return NOERROR;
}

} // namespace Net
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


