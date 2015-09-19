#ifndef __ELASTOS_DROID_NET_NSD_CNSDMANAGER_H__
#define __ELASTOS_DROID_NET_NSD_CNSDMANAGER_H__

#include "_Elastos_Droid_Net_Nsd_CNsdManager.h"
#include "ext/frameworkdef.h"
#include <elastos/utility/etl/HashMap.h>
#include "text/TextUtils.h"
#include "os/HandlerBase.h"
#include "utility/AsyncChannel.h"

using Elastos::Utility::Etl::HashMap;
using Elastos::Utility::Concurrent::ICountDownLatch;
using Elastos::Droid::Os::HandlerBase;
using Elastos::Droid::Os::IMessenger;
using Elastos::Droid::Text::TextUtils;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Internal::Utility::AsyncChannel;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Nsd {

/**
 * The Network Service Discovery Manager class provides the API to discover services
 * on a network. As an example, if device A and device B are connected over a Wi-Fi
 * network, a game registered on device A can be discovered by a game on device
 * B. Another example use case is an application discovering printers on the network.
 *
 * <p> The API currently supports DNS based service discovery and discovery is currently
 * limited to a local network over Multicast DNS. DNS service discovery is described at
 * http://files.dns-sd.org/draft-cheshire-dnsext-dns-sd.txt
 *
 * <p> The API is asynchronous and responses to requests from an application are on listener
 * callbacks on a seperate thread.
 *
 * <p> There are three main operations the API supports - registration, discovery and resolution.
 * <pre>
 *                          Application start
 *                                 |
 *                                 |
 *                                 |                  onServiceRegistered()
 *                     Register any local services  /
 *                      to be advertised with       \
 *                       registerService()            onRegistrationFailed()
 *                                 |
 *                                 |
 *                          discoverServices()
 *                                 |
 *                      Maintain a list to track
 *                        discovered services
 *                                 |
 *                                 |--------->
 *                                 |          |
 *                                 |      onServiceFound()
 *                                 |          |
 *                                 |     add service to list
 *                                 |          |
 *                                 |<----------
 *                                 |
 *                                 |--------->
 *                                 |          |
 *                                 |      onServiceLost()
 *                                 |          |
 *                                 |   remove service from list
 *                                 |          |
 *                                 |<----------
 *                                 |
 *                                 |
 *                                 | Connect to a service
 *                                 | from list ?
 *                                 |
 *                          resolveService()
 *                                 |
 *                         onServiceResolved()
 *                                 |
 *                     Establish connection to service
 *                     with the host and port information
 *
 * </pre>
 * An application that needs to advertise itself over a network for other applications to
 * discover it can do so with a call to {@link #registerService}. If Example is a http based
 * application that can provide HTML data to peer services, it can register a name "Example"
 * with service type "_http._tcp". A successful registration is notified with a callback to
 * {@link RegistrationListener#onServiceRegistered} and a failure to register is notified
 * over {@link RegistrationListener#onRegistrationFailed}
 *
 * <p> A peer application looking for http services can initiate a discovery for "_http._tcp"
 * with a call to {@link #discoverServices}. A service found is notified with a callback
 * to {@link DiscoveryListener#onServiceFound} and a service lost is notified on
 * {@link DiscoveryListener#onServiceLost}.
 *
 * <p> Once the peer application discovers the "Example" http srevice, and needs to receive data
 * from the "Example" application, it can initiate a resolve with {@link #resolveService} to
 * resolve the host and port details for the purpose of establishing a connection. A successful
 * resolve is notified on {@link ResolveListener#onServiceResolved} and a failure is notified
 * on {@link ResolveListener#onResolveFailed}.
 *
 * Applications can reserve for a service type at
 * http://www.iana.org/form/ports-service. Existing services can be found at
 * http://www.iana.org/assignments/service-names-port-numbers/service-names-port-numbers.xml
 *
 * Get an instance of this class by calling {@link android.content.Context#getSystemService(String)
 * Context.getSystemService(Context.NSD_SERVICE)}.
 *
 * {@see NsdServiceInfo}
 */
CarClass(CNsdManager)
{
public:
    CNsdManager();

    ~CNsdManager();

    CARAPI constructor(
        /* [in] */ IContext * context,
        /* [in] */ IINsdManager* service);

    /**
     * Register a service to be discovered by other services.
     *
     * <p> The function call immediately returns after sending a request to register service
     * to the framework. The application is notified of a success to initiate
     * discovery through the callback {@link RegistrationListener#onServiceRegistered} or a failure
     * through {@link RegistrationListener#onRegistrationFailed}.
     *
     * @param serviceInfo The service being registered
     * @param protocolType The service discovery protocol
     * @param listener The listener notifies of a successful registration and is used to
     * unregister this service through a call on {@link #unregisterService}. Cannot be null.
     */
    CARAPI RegisterService(
        /* [in] */  INsdServiceInfo* serviceInfo,
        /* [in] */  Int32 protocolType,
        /* [in] */  INsdManagerRegistrationListener* listener);

    /**
     * Unregister a service registered through {@link #registerService}. A successful
     * unregister is notified to the application with a call to
     * {@link RegistrationListener#onServiceUnregistered}.
     *
     * @param listener This should be the listener object that was passed to
     * {@link #registerService}. It identifies the service that should be unregistered
     * and notifies of a successful unregistration.
     */
    CARAPI UnregisterService(
        /* [in] */  INsdManagerRegistrationListener* listener);

    /**
     * Initiate service discovery to browse for instances of a service type. Service discovery
     * consumes network bandwidth and will continue until the application calls
     * {@link #stopServiceDiscovery}.
     *
     * <p> The function call immediately returns after sending a request to start service
     * discovery to the framework. The application is notified of a success to initiate
     * discovery through the callback {@link DiscoveryListener#onDiscoveryStarted} or a failure
     * through {@link DiscoveryListener#onStartDiscoveryFailed}.
     *
     * <p> Upon successful start, application is notified when a service is found with
     * {@link DiscoveryListener#onServiceFound} or when a service is lost with
     * {@link DiscoveryListener#onServiceLost}.
     *
     * <p> Upon failure to start, service discovery is not active and application does
     * not need to invoke {@link #stopServiceDiscovery}
     *
     * @param serviceType The service type being discovered. Examples include "_http._tcp" for
     * http services or "_ipp._tcp" for printers
     * @param protocolType The service discovery protocol
     * @param listener  The listener notifies of a successful discovery and is used
     * to stop discovery on this serviceType through a call on {@link #stopServiceDiscovery}.
     * Cannot be null.
     */
    CARAPI DiscoverServices(
        /* [in] */  const String& serviceType,
        /* [in] */  Int32 protocolType,
        /* [in] */  INsdManagerDiscoveryListener* listener);

    /**
     * Stop service discovery initiated with {@link #discoverServices}. An active service
     * discovery is notified to the application with {@link DiscoveryListener#onDiscoveryStarted}
     * and it stays active until the application invokes a stop service discovery. A successful
     * stop is notified to with a call to {@link DiscoveryListener#onDiscoveryStopped}.
     *
     * <p> Upon failure to stop service discovery, application is notified through
     * {@link DiscoveryListener#onStopDiscoveryFailed}.
     *
     * @param listener This should be the listener object that was passed to {@link #discoverServices}.
     * It identifies the discovery that should be stopped and notifies of a successful stop.
     */
    CARAPI StopServiceDiscovery(
        /* [in] */  INsdManagerDiscoveryListener* listener);

    /**
     * Resolve a discovered service. An application can resolve a service right before
     * establishing a connection to fetch the IP and port details on which to setup
     * the connection.
     *
     * @param serviceInfo service to be resolved
     * @param listener to receive callback upon success or failure. Cannot be null.
     */
    CARAPI ResolveService(
        /* [in] */  INsdServiceInfo* serviceInfo,
        /* [in] */  INsdManagerResolveListener* listener);

    /** Internal use only @hide */
    CARAPI SetEnabled(
        /* [in] */  Boolean enabled);

private:
    CARAPI_(Int32) PutListener(
        /* [in] */ IInterface* listener,
        /* [in] */ INsdServiceInfo* s);

    CARAPI GetListener(
        /* [in] */ Int32 key,
        /* [out] */ IInterface** listener);

    CARAPI_(AutoPtr<INsdServiceInfo>) GetNsdService(
        /* [in] */ Int32 key);

    CARAPI_(void) RemoveListener(
        /* [in] */ Int32 key);

    CARAPI_(Int32) GetListenerKey(
        /* [in] */ IInterface* listener);

    /**
     * Initialize AsyncChannel
     */
    CARAPI Init();

    CARAPI_(AutoPtr<IMessenger>) GetMessenger();

private:
    class ServiceHandler
        : public HandlerBase
    {
    public:
        ServiceHandler(
            /* [in] */ ILooper* looper,
            /* [in] */ CNsdManager* host);

        CARAPI HandleMessage(
            /* [in] */ IMessage* message);

        CARAPI_(String) CodeToString(
            /* [in] */ Int32 what);
    private:
        CNsdManager* mHost;
    };

public:
    static const String TAG;
    static const Int32 BASE;
    static const Int32 INVALID_LISTENER_KEY;

    AutoPtr<IINsdManager> mService;
    AutoPtr<IContext> mContext;
    Int32 mListenerKey;
    HashMap<Int32, AutoPtr<IInterface> > mListenerMap;
    HashMap<Int32, AutoPtr<INsdServiceInfo> > mServiceMap;
    Object mMapLock;

    AutoPtr<AsyncChannel> mAsyncChannel;
    AutoPtr<ServiceHandler> mHandler;
    AutoPtr<ICountDownLatch> mConnected;
};

}
}
}
}

#endif // __ELASTOS_DROID_NET_NSD_CNSDMANAGER_H__
