
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Net {

//===============================================================
// NetworkChangeNotifierAutoDetect::ConnectivityManagerDelegate
//===============================================================
NetworkChangeNotifierAutoDetect::ConnectivityManagerDelegate::ConnectivityManagerDelegate(
    /* [in] */ IContext* context)
{
   	context->GetSystemService(IContext::CONNECTIVITY_SERVICE, (IInterface**)&mConnectivityManager);
}

// For testing.
NetworkChangeNotifierAutoDetect::ConnectivityManagerDelegate::ConnectivityManagerDelegate()
{
    // All the methods below should be overridden.
    mConnectivityManager = NULL;
}

Boolean NetworkChangeNotifierAutoDetect::ConnectivityManagerDelegate::ActiveNetworkExists()
{
	AutoPtr<INetworkInfo> info;
    CNetworkInfo::New((INetworkInfo**)&info);
	mConnectivityManager->GetActiveNetworkInfo((INetworkInfo**)&info);
    return Boolean(info != NULL);
}

Boolean NetworkChangeNotifierAutoDetect::ConnectivityManagerDelegate::IsConnected()
{
	AutoPtr<INetworkInfo> info;
	CNetworkInfo::New((INetworkInfo**)&info);
	mConnectivityManager->GetActiveNetworkInfo(&info);
	Boolean isconnect;
	info->IsConnected(&isconnect);
    return isconnect;
}

Int32 NetworkChangeNotifierAutoDetect::ConnectivityManagerDelegate::GetNetworkType()
{
	AutoPtr<INetworkInfo> info;
	CNetworkInfo::New((INetworkInfo**)&info);
	mConnectivityManager->GetActiveNetworkInfo(&info);
	Int32 type;
	info->GetType(&type);
    return type;
}

Int32 NetworkChangeNotifierAutoDetect::ConnectivityManagerDelegate::GetNetworkSubtype()
{
	AutoPtr<INetworkInfo> info;
	CNetworkInfo::New((INetworkInfo**)&info);
	mConnectivityManager->GetActiveNetworkInfo(&info);
	Int32 type;
	info->GetSubtype(&type);
    return type;
}

//===============================================================
// 		NetworkChangeNotifierAutoDetect::WifiManagerDelegate
//===============================================================
NetworkChangeNotifierAutoDetect::WifiManagerDelegate::WifiManagerDelegate(
    /* [in] */ IContext* context)
{
	context->GetSystemService(IContext::WIFI_SERVICE, (IWifiManager**)&mWifiManager);
}

// For testing.
NetworkChangeNotifierAutoDetect::WifiManagerDelegate::WifiManagerDelegate()
{
    // All the methods below should be overridden.
    mWifiManager = NULL;
}

String NetworkChangeNotifierAutoDetect::WifiManagerDelegate::GetWifiSSID()
{
    AutoPtr<IWifiInfo> wifiInfo;
    CWifiInfo::New((IWifiInfo**)&wifiInfo);
    mWifiManager->GetConnectionInfo(&wifiInfo);
    if (wifiInfo == NULL)
        return String("");

    String ssid;
    wifiInfo->GetSSID(&ssid);
    return ssid == String("") ? String("") : ssid;
}

//===============================================================
//NetworkChangeNotifierAutoDetect::NetworkConnectivityIntentFilter
//===============================================================
NetworkChangeNotifierAutoDetect::NetworkConnectivityIntentFilter::NetworkConnectivityIntentFilter()
{
    AddAction(IConnectivityManager::CONNECTIVITY_ACTION);
}

//===============================================================
// 				NetworkChangeNotifierAutoDetect
//===============================================================
const String NetworkChangeNotifierAutoDetect::TAG("NetworkChangeNotifierAutoDetect");

NetworkChangeNotifierAutoDetect::NetworkChangeNotifierAutoDetect(
    /* [in] */ Observer* observer,
    /* [in] */ IContext* context)
    : mIntentFilter(new NetworkConnectivityIntentFilter())
    , mObserver(observer)
    , mRegistered(FALSE)
    , mConnectionType(0)
{
	// question: ApplicationStatus and other.
    context->GetApplicationContext(&mContext);
    mConnectivityManagerDelegate = new ConnectivityManagerDelegate(context);
    mWifiManagerDelegate = new WifiManagerDelegate(context);
    mConnectionType = GetCurrentConnectionType();
    mWifiSSID = GetCurrentWifiSSID();
    ApplicationStatus::RegisterApplicationStateListener(this);
}

ECode NetworkChangeNotifierAutoDetect::Destroy()
{
    UnregisterReceiver();
    return NOERROR;
}

Int32 NetworkChangeNotifierAutoDetect::GetCurrentConnectionType()
{
    // Track exactly what type of connection we have.
    if (!mConnectivityManagerDelegate->ActiveNetworkExists() ||
            !mConnectivityManagerDelegate->IsConnected()) {
        return NetworkChangeNotifier::CONNECTION_NONE;
    }

    switch (mConnectivityManagerDelegate->GetNetworkType()) {
        case IConnectivityManager::TYPE_ETHERNET:
            return NetworkChangeNotifier::CONNECTION_ETHERNET;
        case IConnectivityManager::TYPE_WIFI:
            return NetworkChangeNotifier::CONNECTION_WIFI;
        case IConnectivityManager::TYPE_WIMAX:
            return NetworkChangeNotifier::CONNECTION_4G;
        case IConnectivityManager::TYPE_BLUETOOTH:
            return NetworkChangeNotifier::CONNECTION_BLUETOOTH;
        case IConnectivityManager::TYPE_MOBILE:
        	{
	            // Use information from TelephonyManager to classify the connection.
	            switch (mConnectivityManagerDelegate->GetNetworkSubtype()) {
	                case ITelephonyManager::NETWORK_TYPE_GPRS:
	                case ITelephonyManager::NETWORK_TYPE_EDGE:
	                case ITelephonyManager::NETWORK_TYPE_CDMA:
	                case ITelephonyManager::NETWORK_TYPE_1xRTT:
	                case ITelephonyManager::NETWORK_TYPE_IDEN:
	                    return NetworkChangeNotifier::CONNECTION_2G;
	                case ITelephonyManager::NETWORK_TYPE_UMTS:
	                case ITelephonyManager::NETWORK_TYPE_EVDO_0:
	                case ITelephonyManager::NETWORK_TYPE_EVDO_A:
	                case ITelephonyManager::NETWORK_TYPE_HSDPA:
	                case ITelephonyManager::NETWORK_TYPE_HSUPA:
	                case ITelephonyManager::NETWORK_TYPE_HSPA:
	                case ITelephonyManager::NETWORK_TYPE_EVDO_B:
	                case ITelephonyManager::NETWORK_TYPE_EHRPD:
	                case ITelephonyManager::NETWORK_TYPE_HSPAP:
	                    return NetworkChangeNotifier::CONNECTION_3G;
	                case ITelephonyManager::NETWORK_TYPE_LTE:
	                    return NetworkChangeNotifier::CONNECTION_4G;
	                default:
	                    return NetworkChangeNotifier::CONNECTION_UNKNOWN;
	            }
	        }
        default:
            return NetworkChangeNotifier::CONNECTION_UNKNOWN;
    }
}

// BroadcastReceiver
//@Override
ECode NetworkChangeNotifierAutoDetect::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
	VALIDATE_NOT_NULL(context);
	VALIDATE_NOT_NULL(intent);
    ConnectionTypeChanged();
    return NOERROR;
}

// ApplicationStatus.ApplicationStateListener
//@Override
ECode NetworkChangeNotifierAutoDetect::OnApplicationStateChange(
    /* [in] */ Int32 newState)
{
    if (newState == ApplicationState::HAS_RUNNING_ACTIVITIES) {
        ConnectionTypeChanged();
        RegisterReceiver();
    }
    else if (newState == ApplicationState::HAS_PAUSED_ACTIVITIES) {
        UnregisterReceiver();
    }
    return NOERROR;
}

/**
 * Allows overriding the ConnectivityManagerDelegate for tests.
 */
ECode NetworkChangeNotifierAutoDetect::SetConnectivityManagerDelegateForTests(
    /* [in] */ ConnectivityManagerDelegate* delegate)
{
	VALIDATE_NOT_NULL(delegate);
    mConnectivityManagerDelegate = delegate;
    return NOERROR;
}

/**
 * Allows overriding the WifiManagerDelegate for tests.
 */
ECode NetworkChangeNotifierAutoDetect::SetWifiManagerDelegateForTests(
    /* [in] */ WifiManagerDelegate* delegate)
{
	VALIDATE_NOT_NULL(delegate);
    mWifiManagerDelegate = delegate;
    return NOERROR;
}

/**
 * Register a BroadcastReceiver in the given context.
 */
ECode NetworkChangeNotifierAutoDetect::RegisterReceiver()
{
    if (!mRegistered) {
        mRegistered = TRUE;
        mContext->RegisterReceiver(this, mIntentFilter);
    }
    return NOERROR;
}

/**
 * Unregister the BroadcastReceiver in the given context.
 */
ECode NetworkChangeNotifierAutoDetect::UnregisterReceiver()
{
    if (mRegistered) {
        mRegistered = FALSE;
        mContext->UnregisterReceiver(this);
    }
    return NOERROR;
}

String NetworkChangeNotifierAutoDetect::GetCurrentWifiSSID()
{
    if (GetCurrentConnectionType() != NetworkChangeNotifier::CONNECTION_WIFI)
        return String("");
    return mWifiManagerDelegate->GetWifiSSID();
}

ECode NetworkChangeNotifierAutoDetect::ConnectionTypeChanged()
{
    Int32 newConnectionType = GetCurrentConnectionType();
    String newWifiSSID = GetCurrentWifiSSID();
    if (newConnectionType == mConnectionType && newWifiSSID.Equals(mWifiSSID))
        return NOERROR;

    mConnectionType = newConnectionType;
    mWifiSSID = newWifiSSID;
    //Log.d(TAG, "Network connectivity changed, type is: " + mConnectionType);
    mObserver->OnConnectionTypeChanged(newConnectionType);
    return NOERROR;
}

} // namespace Net
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
