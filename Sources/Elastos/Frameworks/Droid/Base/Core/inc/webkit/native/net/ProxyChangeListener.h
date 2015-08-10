// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _ELASTOS_DROID_WEBKIT_NET_PROXYCHANGELISTENER_H_
#define _ELASTOS_DROID_WEBKIT_NET_PROXYCHANGELISTENER_H_

//package org.chromium.net;

//import android.content.BroadcastReceiver;
//import android.content.Context;
//import android.content.Intent;
//import android.content.IntentFilter;
//import android.net.Proxy;
//import android.os.Build;
//import android.util.Log;

//import org.chromium.base.CalledByNative;
//import org.chromium.base.JNINamespace;
//import org.chromium.base.NativeClassQualifiedName;

//import java.lang.reflect.InvocationTargetException;
//import java.lang.reflect.Method;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Net {

/**
 * This class partners with native ProxyConfigServiceAndroid to listen for
 * proxy change notifications from Android.
 */
//@JNINamespace("net")
class ProxyChangeListener
{
public:
    class Delegate
    {
    public:
        virtual CARAPI ProxySettingsChanged() = 0;
    };

private:
    class ProxyConfig
    {
    public:
        ProxyConfig(
            /* [in] */ String host,
            /* [in] */ Int32 port);

    public:
        const String mHost;
        const Int32 mPort;
    };

    class ProxyReceiver : public BroadcastReceiver
    {
    public:
        //@Override
        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

        // Extract a ProxyConfig object from the supplied Intent's extra data
        // bundle. The android.net.ProxyProperties class is not exported from
        // tne Android SDK, so we have to use reflection to get at it and invoke
        // methods on it. If we fail, return an empty proxy config (meaning
        // 'direct').
        // TODO(sgurun): once android.net.ProxyInfo is public, rewrite this.
    private:
        CARAPI_(AutoPtr<ProxyConfig>) ExtractNewProxy(
            /* [in] */ IIntent* intent);
    };

public:
    static CARAPI SetEnabled(
        /* [in] */ Boolean enabled);

    virtual CARAPI SetDelegateForTesting(
        /* [in] */ Delegate* delegate);

    //@CalledByNative
    static CARAPI_(AutoPtr<ProxyChangeListener>) Create(
        /* [in] */ IContext* context);

    //@CalledByNative
    static CARAPI_(String) GetProperty(
        /* [in] */ String property);

    //@CalledByNative
    virtual CARAPI Start(
        /* [in] */ Int64 nativePtr);

    //@CalledByNative
    virtual CARAPI Stop();

private:
    ProxyChangeListener(
        /* [in] */ IContext* context);

    CARAPI ProxySettingsChanged(
        /* [in] */ ProxyConfig* cfg);

    CARAPI RegisterReceiver();

    CARAPI UnregisterReceiver();

    // question: native function
    /**
     * See net/proxy/proxy_config_service_android.cc
     */
    //@NativeClassQualifiedName("ProxyConfigServiceAndroid::JNIDelegate")
    //private native void nativeProxySettingsChangedTo(long nativePtr,
    //                                                 String host,
    //                                                 int port);
    CARAPI NativeProxySettingsChangedTo(
        /* [in] */ Int64 nativePtr,
        /* [in] */ String host,
        /* [in] */ Int32 port);

    //@NativeClassQualifiedName("ProxyConfigServiceAndroid::JNIDelegate")
    //private native void nativeProxySettingsChanged(long nativePtr);
    CARAPI NativeProxySettingsChanged(
        /* [in] */ Int64 nativePtr);

private:
    static const String TAG;
    static Boolean sEnabled;
    Int64 mNativePtr;
    AutoPtr<IContext> mContext;
    AutoPtr<ProxyReceiver> mProxyReceiver;
    AutoPtr<Delegate> mDelegate;
};

} // namespace Net
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // _ELASTOS_DROID_WEBKIT_NET_PROXYCHANGELISTENER_H_

