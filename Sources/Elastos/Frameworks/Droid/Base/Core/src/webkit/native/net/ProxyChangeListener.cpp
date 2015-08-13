
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Net {

//===============================================================
// 				ProxyChangeListener::ProxyConfig
//===============================================================
ProxyChangeListener::ProxyConfig::ProxyConfig(
    /* [in] */ String host,
    /* [in] */ Int32 port)
    : mHost(host)
    , mPort(port)
{
}

//===============================================================
// 				ProxyChangeListener::ProxyReceiver
//===============================================================
//@Override
ECode ProxyChangeListener::ProxyReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
	VALIDATE_NOT_NULL(context);
	VALIDATE_NOT_NULL(intent);

	String action;
	intent->GetAction(&action);
    if (action.Equals(IProxy::PROXY_CHANGE_ACTION)) {
        ProxySettingsChanged(ExtractNewProxy(intent));
    }
    return NOERROR;
}

// Extract a ProxyConfig object from the supplied Intent's extra data
// bundle. The android.net.ProxyProperties class is not exported from
// tne Android SDK, so we have to use reflection to get at it and invoke
// methods on it. If we fail, return an empty proxy config (meaning
// 'direct').
// TODO(sgurun): once android.net.ProxyInfo is public, rewrite this.
AutoPtr<ProxyConfig> ProxyChangeListener::ProxyReceiver::ExtractNewProxy(
    /* [in] */ IIntent* intent)
{
    //try {
        const String GET_HOST_NAME("getHost");
        const String GET_PORT_NAME("getPort");
        String className;
        String proxyInfo;
        if (IBuild::VERSION::SDK_INT <= IBuild::VERSION_CODES::KITKAT) {
            className = "android.net.ProxyProperties";
            proxyInfo = "proxy";
        }
        else {
            className = "android.net.ProxyInfo";
            proxyInfo = "android.intent.extra.PROXY_INFO";
        }

		// question: reflection
		AutoPtr<Object> props;
		AutoPtr<IExtras> extras;
		intent->GetExtras((IExtras**)&extras);
		extras->Get(proxyInfo, (Object**)&props);
        if (NULL == props) {
        	AutoPtr<ProxyConfig> ret = NULL;
            return ret;
        }

		// question: reflection
        AutoPtr<IProxyProperties> proxyProper;
        AutoPtr<IProxyInfo> proxyInfo;
        AutoPtr<IMethod> getHostMethod;
        AutoPtr<IMethod> getPortMethod;

        if (IBuild::VERSION::SDK_INT <= IBuild::VERSION_CODES::KITKAT) {
			CProxyProper::New((IProxyProperties**)&proxyProper);
            proxyProper->GetDeclaredMethod(GET_HOST_NAME, (IMethod**)&getHostMethod);
            proxyProper->GetDeclaredMethod(GET_PORT_NAME, (IMethod**)&getPortMethod);
        }
        else {
        	CProxyInfo::New((IProxyInfo**)&proxyInfo);
            proxyInfo->GetDeclaredMethod(GET_HOST_NAME, (IMethod**)&getHostMethod);
            proxyInfo->GetDeclaredMethod(GET_PORT_NAME, (IMethod**)&getPortMethod);
        }

        String host;
        Int32 port;
        getHostMethod->Invoke(props, &host);
        getPortMethod->Invoke(props, &port);

		AutoPtr<ProxyConfig> result = new ProxyConfig(host, port);
        return result;
    //} catch (ClassNotFoundException ex) {
    //    Log.e(TAG, "Using no proxy configuration due to exception:" + ex);
    //    return null;
    //} catch (NoSuchMethodException ex) {
    //    Log.e(TAG, "Using no proxy configuration due to exception:" + ex);
    //    return null;
    //} catch (IllegalAccessException ex) {
    //    Log.e(TAG, "Using no proxy configuration due to exception:" + ex);
    //    return null;
    //} catch (InvocationTargetException ex) {
    //    Log.e(TAG, "Using no proxy configuration due to exception:" + ex);
    //    return null;
    //} catch (NullPointerException ex) {
    //    Log.e(TAG, "Using no proxy configuration due to exception:" + ex);
    //    return null;
    //}
}

//===============================================================
// 					ProxyChangeListener
//===============================================================
const String ProxyChangeListener::TAG("ProxyChangeListener");
Boolean ProxyChangeListener::sEnabled = TRUE;

ProxyChangeListener::ProxyChangeListener(
    /* [in] */ IContext* context)
    : mContext(context)
    , mNativePtr(0)
{
}

ECode ProxyChangeListener::SetEnabled(
    /* [in] */ Boolean enabled)
{
    sEnabled = enabled;
    return NOERROR;
}

ECode ProxyChangeListener::SetDelegateForTesting(
    /* [in] */ IDelegate* delegate)
{
	VALIDATE_NOT_NULL(delegate);
    mDelegate = delegate;
    return NOERROR;
}

//@CalledByNative
AutoPtr<ProxyChangeListener> ProxyChangeListener::Create(
    /* [in] */ IContext* context)
{
	AutoPtr<ProxyChangeListener> ret = new ProxyChangeListener(context);
    return ret;
}

//@CalledByNative
String ProxyChangeListener::GetProperty(
    /* [in] */ String property)
{
	String result;
	ISystem::GetProperty(property, &result);
    return result;
}

//@CalledByNative
ECode ProxyChangeListener::Start(
    /* [in] */ Int64 nativePtr)
{
    assert(0 == mNativePtr);
    mNativePtr = nativePtr;
    RegisterReceiver();
    return NOERROR;
}

//@CalledByNative
ECode ProxyChangeListener::Stop()
{
    mNativePtr = 0;
    UnregisterReceiver();
    return NOERROR;
}

ECode ProxyChangeListener::ProxySettingsChanged(
    /* [in] */ ProxyConfig* cfg)
{
	VALIDATE_NOT_NULL(cfg);

	// question: return ECode what value is, return NOERROR temporarily
    if (!sEnabled) {
        return NOERROR;
    }
    if (mDelegate != NULL) {
        mDelegate->ProxySettingsChanged();
    }
    if (0 == mNativePtr) {
        return NOERROR;
    }
    // Note that this code currently runs on a MESSAGE_LOOP_UI thread, but
    // the C++ code must run the callbacks on the network thread.
    if (cfg != NULL) {
        NativeProxySettingsChangedTo(mNativePtr, cfg.mHost, cfg.mPort);
    }
    else {
        NativeProxySettingsChanged(mNativePtr);
    }
    return NOERROR;
}

ECode ProxyChangeListener::RegisterReceiver()
{
	// question: return ECode what value is, return NOERROR temporarily
    if (mProxyReceiver != NULL) {
        return NOERROR;
    }
    AutoPtr<IntentFilter> filter = new IntentFilter();
    filter->AddAction(IProxy::PROXY_CHANGE_ACTION);
    mProxyReceiver = new ProxyReceiver();
    AutoPtr<IContext> context;
    mContext->GetApplicationContext(&context);
    context->RegisterReceiver(mProxyReceiver, filter);
    return NOERROR;
}

ECode ProxyChangeListener::UnregisterReceiver()
{
	// question: return ECode what value is, return NOERROR temporarily
    if (mProxyReceiver == NULL) {
        return NOERROR;
    }
    mContext->UnregisterReceiver(mProxyReceiver);
    mProxyReceiver = NULL;
    return NOERROR;
}

/**
 * See net/proxy/proxy_config_service_android.cc
 */
//@NativeClassQualifiedName("ProxyConfigServiceAndroid::JNIDelegate")
//private native void nativeProxySettingsChangedTo(long nativePtr,
//                                                 String host,
//                                                 int port);
ECode ProxyChangeListener::NativeProxySettingsChangedTo(
	/* [in] */ Int64 nativePtr,
	/* [in] */ String host,
	/* [in] */ Int32 port)
{
	return NOERROR;
}

//@NativeClassQualifiedName("ProxyConfigServiceAndroid::JNIDelegate")
//private native void nativeProxySettingsChanged(long nativePtr);
ECode ProxyChangeListener::NativeProxySettingsChanged(
	/* [in] */ Int64 nativePtr)
{
	return NOERROR;
}

} // namespace Net
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
