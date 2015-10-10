
#include "elastos/droid/net/ProxyDataTracker.h"

namespace Elastos {
namespace Droid {
namespace Net {

CAR_INTERFACE_IMPL(ProxyDataTracker, BaseNetworkStateTracker, IProxyDataTracker)

const String ProxyDataTracker::TAG = String("ProxyDataTracker");
const String ProxyDataTracker::NETWORK_TYPE = String("PROXY");
const String ProxyDataTracker::DNS1 = String("8.8.8.8");
const String ProxyDataTracker::DNS2 = String("8.8.4.4");
const String ProxyDataTracker::INTERFACE_NAME = String("ifb0");
const String ProxyDataTracker::REASON_ENABLED = String("enabled");
const String ProxyDataTracker::REASON_DISABLED = String("disabled");
const String ProxyDataTracker::REASON_PROXY_DOWN = String("proxy_down");
const Int32 ProxyDataTracker::MSG_TEAR_DOWN_REQUEST = 1;
const Int32 ProxyDataTracker::MSG_SETUP_REQUEST = 2;
const String ProxyDataTracker::PERMISSION_PROXY_STATUS_SENDER = String("android.permission.ACCESS_NETWORK_CONDITIONS");
const String ProxyDataTracker::ACTION_PROXY_STATUS_CHANGE = String("com.android.net.PROXY_STATUS_CHANGE");
const String ProxyDataTracker::KEY_IS_PROXY_AVAILABLE = String("is_proxy_available");
const String ProxyDataTracker::KEY_REPLY_TO_MESSENGER_BINDER = String("reply_to_messenger_binder");
const String ProxyDataTracker::KEY_REPLY_TO_MESSENGER_BINDER_BUNDLE = String("reply_to_messenger_binder_bundle");

ProxyDataTracker::ProxyDataTracker()
{
#if 0 // TODO: Translate codes below
    private AtomicBoolean mReconnectRequested = new AtomicBoolean(false);
    private AtomicBoolean mIsProxyAvailable = new AtomicBoolean(false);
    private final AtomicInteger mDefaultGatewayAddr = new AtomicInteger(0);

    private final BroadcastReceiver mProxyStatusServiceListener = new BroadcastReceiver() {
        @Override
#endif
}

ECode ProxyDataTracker::constructor()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mNetworkInfo = new NetworkInfo(ConnectivityManager.TYPE_PROXY, 0, NETWORK_TYPE, "");
        mLinkProperties = new LinkProperties();
        mNetworkCapabilities = new NetworkCapabilities();
        mNetworkInfo.setIsAvailable(true);
        try {
            mLinkProperties.addDnsServer(InetAddress.getByName(DNS1));
            mLinkProperties.addDnsServer(InetAddress.getByName(DNS2));
            mLinkProperties.setInterfaceName(INTERFACE_NAME);
        } catch (UnknownHostException e) {
            Log.e(TAG, "Could not add DNS address", e);
        }

#endif
}

ECode ProxyDataTracker::Clone(
    /* [out] */ IInterface** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        throw new CloneNotSupportedException();

#endif
}

ECode ProxyDataTracker::StartMonitoring(
    /* [in] */ IContext* context,
    /* [in] */ IHandler* target)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mContext = context;
        mTarget = target;
        mContext.registerReceiver(mProxyStatusServiceListener,
                new IntentFilter(ACTION_PROXY_STATUS_CHANGE),
                PERMISSION_PROXY_STATUS_SENDER,
                null);

#endif
}

ECode ProxyDataTracker::Teardown(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        setTeardownRequested(true);
        mReconnectRequested.set(false);
        try {
            if (mIsProxyAvailable.get() && mProxyStatusService != null) {
                mProxyStatusService.send(Message.obtain(null, MSG_TEAR_DOWN_REQUEST));
            }
        } catch (RemoteException e) {
            Log.e(TAG, "Unable to connect to proxy status service", e);
            return false;
        }
        setDetailedState(NetworkInfo.DetailedState.DISCONNECTED, REASON_DISABLED, null);
        return true;

#endif
}

ECode ProxyDataTracker::Reconnect(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mReconnectRequested.set(true);
        setTeardownRequested(false);
        if (!mIsProxyAvailable.get()) {
            Log.w(TAG, "Reconnect requested even though proxy service is not up. Bailing.");
            return false;
        }
        setDetailedState(NetworkInfo.DetailedState.CONNECTING, REASON_ENABLED, null);
        try {
            mProxyStatusService.send(Message.obtain(null, MSG_SETUP_REQUEST));
        } catch (RemoteException e) {
            Log.e(TAG, "Unable to connect to proxy status service", e);
            setDetailedState(NetworkInfo.DetailedState.DISCONNECTED, REASON_PROXY_DOWN, null);
            return false;
        }
        // We'll assume proxy is set up successfully. If not, a status change broadcast will be
        // received afterwards to indicate any failure.
        setDetailedState(NetworkInfo.DetailedState.CONNECTED, REASON_ENABLED, null);
        return true;

#endif
}

ECode ProxyDataTracker::GetDefaultGatewayAddr(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return mDefaultGatewayAddr.get();

#endif
}

ECode ProxyDataTracker::GetTcpBufferSizesPropName(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return "net.tcp.buffersize.wifi";

#endif
}

ECode ProxyDataTracker::SetDetailedState(
    /* [in] */ NetworkInfoDetailedState state,
    /* [in] */ const String& reason,
    /* [in] */ const String& extraInfo)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mNetworkInfo.setDetailedState(state, reason, extraInfo);
        Message msg = mTarget.obtainMessage(EVENT_STATE_CHANGED, mNetworkInfo);
        msg.sendToTarget();

#endif
}


} // namespace Net
} // namespace Droid
} // namespace Elastos
