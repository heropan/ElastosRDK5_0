// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _ELASTOS_DROID_WEBKIT_NET_NETWORKCHANGENOTIFIERAUTODETECT_H_
#define _ELASTOS_DROID_WEBKIT_NET_NETWORKCHANGENOTIFIERAUTODETECT_H_

//package org.chromium.net;

//import android.content.BroadcastReceiver;
//import android.content.Context;
//import android.content.Intent;
//import android.content.IntentFilter;
//import android.net.ConnectivityManager;
//import android.net.wifi.WifiInfo;
//import android.net.wifi.WifiManager;
//import android.telephony.TelephonyManager;
//import android.util.Log;

//import org.chromium.base.ApplicationState;
//import org.chromium.base.ApplicationStatus;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Net {

/**
 * Used by the NetworkChangeNotifier to listens to platform changes in connectivity.
 * Note that use of this class requires that the app have the platform
 * ACCESS_NETWORK_STATE permission.
 */
class NetworkChangeNotifierAutoDetect
    : public BroadcastReceiver
    , public ApplicationStatus::ApplicationStateListener
{
private:
    /** Queries the ConnectivityManager for information about the current connection. */
    class ConnectivityManagerDelegate
    {
    private:
        ConnectivityManagerDelegate(
            /* [in] */ IContext* context);

        // For testing.
        ConnectivityManagerDelegate();

        CARAPI_(Boolean) ActiveNetworkExists();

        CARAPI_(Boolean) IsConnected();

        CARAPI_(Int32) GetNetworkType();

        CARAPI_(Int32) GetNetworkSubtype();

    private:
        const AutoPtr<IConnectivityManager> mConnectivityManager;
    };

    /** Queries the WifiManager for SSID of the current Wifi connection. */
    class WifiManagerDelegate
    {
    private:
        WifiManagerDelegate(
            /* [in] */ IContext* context);

        // For testing.
        WifiManagerDelegate();

        CARAPI_(String) GetWifiSSID();

    private:
        const AutoPtr<IWifiManager> mWifiManager;
    };

    class NetworkConnectivityIntentFilter : public IntentFilter
    {
    private:
        NetworkConnectivityIntentFilter();
    };

public:
    class Observer
    {
    public:
        virtual CARAPI OnConnectionTypeChanged(
            /* [in] */ Int32 newConnectionType) = 0;
    };

public:
    NetworkChangeNotifierAutoDetect(
        /* [in] */ Observer* observer,
        /* [in] */ IContext* context);

    virtual CARAPI Destroy();

    virtual CARAPI_(Int32) GetCurrentConnectionType();

    // BroadcastReceiver
    //@Override
    CARAPI OnReceive(
        /* [in] */ IContext* context,
        /* [in] */ IIntent* intent);

    // ApplicationStatus.ApplicationStateListener
    //@Override
    CARAPI OnApplicationStateChange(
        /* [in] */ Int32 newState);

private:
    /**
     * Allows overriding the ConnectivityManagerDelegate for tests.
     */
    CARAPI SetConnectivityManagerDelegateForTests(
        /* [in] */ ConnectivityManagerDelegate* delegate);

    /**
     * Allows overriding the WifiManagerDelegate for tests.
     */
    CARAPI SetWifiManagerDelegateForTests(
        /* [in] */ WifiManagerDelegate* delegate);

    /**
     * Register a BroadcastReceiver in the given context.
     */
    CARAPI RegisterReceiver();

    /**
     * Unregister the BroadcastReceiver in the given context.
     */
    CARAPI UnregisterReceiver();

    CARAPI_(String) GetCurrentWifiSSID();

    CARAPI ConnectionTypeChanged();

private:
    static const String TAG;
    const AutoPtr<NetworkConnectivityIntentFilter> mIntentFilter;
    const AutoPtr<Observer> mObserver;
    AutoPtr<IContext> mContext;
    AutoPtr<ConnectivityManagerDelegate> mConnectivityManagerDelegate;
    AutoPtr<WifiManagerDelegate> mWifiManagerDelegate;
    Boolean mRegistered;
    Int32 mConnectionType;
    String mWifiSSID;
};

} // namespace Net
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // _ELASTOS_DROID_WEBKIT_NET_NETWORKCHANGENOTIFIERAUTODETECT_H_

