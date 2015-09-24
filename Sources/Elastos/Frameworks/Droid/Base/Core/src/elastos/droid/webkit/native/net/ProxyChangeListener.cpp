// wuweizuo automatic build .cpp file from .java file.

#include "ProxyChangeListener.h"

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Net {

//=====================================================================
//                   ProxyChangeListener::ProxyConfig
//=====================================================================
ProxyChangeListener::ProxyConfig::ProxyConfig(
    /* [in] */ const String& host,
    /* [in] */ Int32 port)
{
    // ==================before translated======================
    // mHost = host;
    // mPort = port;
}

//=====================================================================
//                  ProxyChangeListener::ProxyReceiver
//=====================================================================
ECode ProxyChangeListener::ProxyReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    VALIDATE_NOT_NULL(context);
    VALIDATE_NOT_NULL(intent);
    // ==================before translated======================
    // if (intent.getAction().equals(Proxy.PROXY_CHANGE_ACTION)) {
    //     proxySettingsChanged(extractNewProxy(intent));
    // }
    assert(0);
    return NOERROR;
}

AutoPtr<ProxyConfig> ProxyChangeListener::ProxyReceiver::ExtractNewProxy(
    /* [in] */ IIntent* intent)
{
    // ==================before translated======================
    // try {
    //     final String GET_HOST_NAME = "getHost";
    //     final String GET_PORT_NAME = "getPort";
    //     String className;
    //     String proxyInfo;
    //     if (Build.VERSION.SDK_INT <= Build.VERSION_CODES.KITKAT) {
    //         className = "android.net.ProxyProperties";
    //         proxyInfo = "proxy";
    //     } else {
    //         className = "android.net.ProxyInfo";
    //         proxyInfo = "android.intent.extra.PROXY_INFO";
    //     }
    //
    //     Object props = intent.getExtras().get(proxyInfo);
    //     if (props == null) {
    //         return null;
    //     }
    //
    //     Class<?> cls = Class.forName(className);
    //     Method getHostMethod = cls.getDeclaredMethod(GET_HOST_NAME);
    //     Method getPortMethod = cls.getDeclaredMethod(GET_PORT_NAME);
    //
    //     String host = (String) getHostMethod.invoke(props);
    //     int port = (Integer) getPortMethod.invoke(props);
    //
    //     return new ProxyConfig(host, port);
    // } catch (ClassNotFoundException ex) {
    //     Log.e(TAG, "Using no proxy configuration due to exception:" + ex);
    //     return null;
    // } catch (NoSuchMethodException ex) {
    //     Log.e(TAG, "Using no proxy configuration due to exception:" + ex);
    //     return null;
    // } catch (IllegalAccessException ex) {
    //     Log.e(TAG, "Using no proxy configuration due to exception:" + ex);
    //     return null;
    // } catch (InvocationTargetException ex) {
    //     Log.e(TAG, "Using no proxy configuration due to exception:" + ex);
    //     return null;
    // } catch (NullPointerException ex) {
    //     Log.e(TAG, "Using no proxy configuration due to exception:" + ex);
    //     return null;
    // }
    assert(0);
    AutoPtr<ProxyConfig> empty;
    return empty;
}

//=====================================================================
//                         ProxyChangeListener
//=====================================================================
const String ProxyChangeListener::TAG("ProxyChangeListener");
Boolean ProxyChangeListener::sEnabled = true;

ECode ProxyChangeListener::SetEnabled(
    /* [in] */ Boolean enabled)
{
    // ==================before translated======================
    // sEnabled = enabled;
    assert(0);
    return NOERROR;
}

ECode ProxyChangeListener::SetDelegateForTesting(
    /* [in] */ Delegate* delegate)
{
    VALIDATE_NOT_NULL(delegate);
    // ==================before translated======================
    // mDelegate = delegate;
    assert(0);
    return NOERROR;
}

AutoPtr<ProxyChangeListener> ProxyChangeListener::Create(
    /* [in] */ IContext* context)
{
    // ==================before translated======================
    // return new ProxyChangeListener(context);
    assert(0);
    AutoPtr<ProxyChangeListener> empty;
    return empty;
}

String ProxyChangeListener::GetProperty(
    /* [in] */ const String& property)
{
    // ==================before translated======================
    // return System.getProperty(property);
    assert(0);
    return String("");
}

ECode ProxyChangeListener::Start(
    /* [in] */ Int64 nativePtr)
{
    // ==================before translated======================
    // assert mNativePtr == 0;
    // mNativePtr = nativePtr;
    // registerReceiver();
    assert(0);
    return NOERROR;
}

ECode ProxyChangeListener::Stop()
{
    // ==================before translated======================
    // mNativePtr = 0;
    // unregisterReceiver();
    assert(0);
    return NOERROR;
}

ProxyChangeListener::ProxyChangeListener(
    /* [in] */ IContext* context)
{
    // ==================before translated======================
    // mContext = context;
}

ECode ProxyChangeListener::ProxySettingsChanged(
    /* [in] */ ProxyConfig* cfg)
{
    VALIDATE_NOT_NULL(cfg);
    // ==================before translated======================
    // if (!sEnabled) {
    //     return;
    // }
    // if (mDelegate != null) {
    //     mDelegate.proxySettingsChanged();
    // }
    // if (mNativePtr == 0) {
    //     return;
    // }
    // // Note that this code currently runs on a MESSAGE_LOOP_UI thread, but
    // // the C++ code must run the callbacks on the network thread.
    // if (cfg != null) {
    //     nativeProxySettingsChangedTo(mNativePtr, cfg.mHost, cfg.mPort);
    // } else {
    //     nativeProxySettingsChanged(mNativePtr);
    // }
    assert(0);
    return NOERROR;
}

ECode ProxyChangeListener::RegisterReceiver()
{
    // ==================before translated======================
    // if (mProxyReceiver != null) {
    //     return;
    // }
    // IntentFilter filter = new IntentFilter();
    // filter.addAction(Proxy.PROXY_CHANGE_ACTION);
    // mProxyReceiver = new ProxyReceiver();
    // mContext.getApplicationContext().registerReceiver(mProxyReceiver, filter);
    assert(0);
    return NOERROR;
}

ECode ProxyChangeListener::UnregisterReceiver()
{
    // ==================before translated======================
    // if (mProxyReceiver == null) {
    //     return;
    // }
    // mContext.unregisterReceiver(mProxyReceiver);
    // mProxyReceiver = null;
    assert(0);
    return NOERROR;
}

ECode ProxyChangeListener::NativeProxySettingsChangedTo(
    /* [in] */ Int64 nativePtr,
    /* [in] */ const String& host,
    /* [in] */ Int32 port)
{
    assert(0);
    return NOERROR;
}

ECode ProxyChangeListener::NativeProxySettingsChanged(
    /* [in] */ Int64 nativePtr)
{
    assert(0);
    return NOERROR;
}

} // namespace Net
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


