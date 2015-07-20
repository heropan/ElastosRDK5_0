
#ifndef __REQUESTQUEUE_H__
#define __REQUESTQUEUE_H__

#include <ext/frameworkext.h>
#include <elastos/utility/etl/HashMap.h>
#include <elastos/utility/etl/List.h>
#include "Request.h"
#include "Connection.h"
#include "IdleCache.h"
#include "RequestFeeder.h"
#include "../../content/BroadcastReceiver.h"

using namespace Elastos::Core;
using namespace Elastos::Droid::Content;

using Elastos::Droid::Net::IConnectivityManager;
using Org::Apache::Http::IHttpHost;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

typedef List<AutoPtr<Request> > RequestList;
typedef RequestList::Iterator RequestListIterator;

class ConnectionThread;

class RequestQueue : public RequestFeeder
{
public:
    /**
     * This interface is exposed to each connection
     */
    class ConnectionManager
    {
    public:
        virtual CARAPI GetProxyHost(
            /* [out] */ IHttpHost** host) = 0;

        virtual CARAPI GetConnection(
            /* [in] */ IContext* context,
            /* [in] */ IHttpHost* host,
            /* [out] */ Connection** conn) = 0;

        virtual CARAPI RecycleConnection(
            /* [in] */ Connection* connection,
            /* [in] */ Boolean* result) = 0;
    };

    class ActivePool
        : public ConnectionManager
    {
        friend class RequestQueue;

    public:
        ActivePool(
            /* [in] */ Int32 connectionCount,
            /* [in] */ RequestQueue* parent);

        ~ActivePool();

        CARAPI Startup();

        CARAPI Shutdown();

        CARAPI StartConnectionThread();

        CARAPI StartTiming();

        CARAPI StopTiming();

        CARAPI LogState();

        /**
         * Turns off persistence on all live connections
         */
        CARAPI DisablePersistence();

        /* Linear lookup -- okay for small thread counts.  Might use
           private HashMap<HttpHost, LinkedList<ConnectionThread>> mActiveMap;
           if this turns out to be a hotspot */
        CARAPI GetThread(
            /* [in] */ IHttpHost* host,
            /* [out] */ ConnectionThread** thread);

        CARAPI GetProxyHost(
            /* [out] */ IHttpHost** host);

        CARAPI GetConnection(
            /* [in] */ IContext* context,
            /* [in] */ IHttpHost* host,
            /* [out] */ Connection** conn);

        CARAPI RecycleConnection(
            /* [in] */ Connection* connection,
            /* [in] */ Boolean* result);

    public:
        /** Threads used to process requests */
        AutoPtr<ArrayOf<ConnectionThread*> > mThreads;

        IdleCache* mIdleCache;

    private:
        Int32 mTotalRequest;

        Int32 mTotalConnection;

        Int32 mConnectionCount;

        RequestQueue* mParent;

        Object mLock;

    };

private:
    class LocalBroadcastReceiver
        // : public BroadcastReceiver
    {
    public:
        LocalBroadcastReceiver(
            /* [in] */ RequestQueue* parent);

    protected:
        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

    private:
        RequestQueue* mParent;

    };

    class SyncFeeder : public RequestFeeder
    {
    public:
        SyncFeeder(
            /* [in] */ RequestQueue* parent);

        CARAPI GetRequest(
            /* [out] */ Request** req);

        CARAPI GetRequest(
            /* [in] */ IHttpHost* host,
            /* [out] */ Request** req);

        CARAPI HaveRequest(
            /* [in] */ IHttpHost* host,
            /* [out] */ Boolean* result);

        CARAPI RequeueRequest(
            /* [in] */ Request* request);

    private:
        // This is used in the case where the request fails and needs to be
        // requeued into the RequestFeeder.
        Request* mRequest;

        RequestQueue* mParent;
    };

public:
    RequestQueue();

    ~RequestQueue();

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ Int32 connectionCount);

    CARAPI EnablePlatformNotifications();

    CARAPI DisablePlatformNotifications();

    CARAPI GetProxyHost(
        /* [out] */ Org::Apache::Http::IHttpHost** host);

    CARAPI QueueRequest(
        /* [in] */ IRequestQueue* reqQueue,
        /* [in] */ const String& url,
        /* [in] */ const String& method,
        /* [in] */ HashMap<String, String>* headers,
        /* [in] */ Elastos::Droid::Net::Http::IEventHandler* eventHandler,
        /* [in] */ Elastos::IO::IInputStream* bodyProvider,
        /* [in] */ Int32 bodyLength,
        /* [out] */ Elastos::Droid::Net::Http::IRequestHandle** handle);

    CARAPI QueueRequest(
        /* [in] */ IRequestQueue* reqQueue,
        /* [in] */ const String& url,
        /* [in] */ Elastos::Droid::Net::IWebAddress* uri,
        /* [in] */ const String& method,
        /* [in] */ HashMap<String, String>* headers,
        /* [in] */ Elastos::Droid::Net::Http::IEventHandler* eventHandler,
        /* [in] */ Elastos::IO::IInputStream* bodyProvider,
        /* [in] */ Int32 bodyLength,
        /* [out] */ Elastos::Droid::Net::Http::IRequestHandle** handle);

    CARAPI QueueSynchronousRequest(
        /* [in] */ IRequestQueue* reqQueue,
        /* [in] */ const String& url,
        /* [in] */ Elastos::Droid::Net::IWebAddress* uri,
        /* [in] */ const String& method,
        /* [in] */ HashMap<String, String>* headers,
        /* [in] */ Elastos::Droid::Net::Http::IEventHandler* eventHandler,
        /* [in] */ Elastos::IO::IInputStream* bodyProvider,
        /* [in] */ Int32 bodyLength,
        /* [out] */ Elastos::Droid::Net::Http::IRequestHandle** handle);

    /**
     * @return true iff there are any non-active requests pending
     */
    CARAPI_(Boolean) RequestsPending();

    /**
     * debug tool: prints request queue to log
     */
    CARAPI Dump();

    CARAPI GetRequest(
         /* [out] */ Request** req);

    CARAPI GetRequest(
        /* [in] */ IHttpHost* host,
        /* [out] */ Request** req);

    CARAPI HaveRequest(
        /* [in] */ IHttpHost* host,
        /* [out] */ Boolean* result);

    CARAPI RequeueRequest(
        /* [in] */ Request* request);

    CARAPI Shutdown();

    CARAPI StartTiming();

    CARAPI StopTiming();

protected:
    CARAPI QueueRequest(
        /* [in] */ Request* request,
        /* [in] */ Boolean head);

private:
    /**
     * Because our IntentReceiver can run within a different thread,
     * synchronize setting the proxy
     */
    CARAPI SetProxyConfig();

    // Chooses between the proxy and the request's host.
    IHttpHost* DetermineHost(
        /* [in] */ IHttpHost* host);

    /* helper */
    CARAPI_(AutoPtr<Request>) RemoveFirst(
        /* [in] */ HashMap<AutoPtr<IHttpHost>, AutoPtr<RequestList> >* requestQueue);

private:
    /**
     * Requests, indexed by HttpHost (scheme, host, port)
     */
    HashMap<AutoPtr<IHttpHost>, AutoPtr<RequestList> > mPending;

    AutoPtr<IContext> mContext;

    ActivePool* mActivePool;

    AutoPtr<IConnectivityManager> mConnectivityManager;

    AutoPtr<IHttpHost> mProxyHost;

    // AutoPtr<IBroadcastReceiver> mProxyChangeReceiver;
    IBroadcastReceiver* mProxyChangeReceiver;

    Object mLock;

};

}
}
}
}

#endif // __REQUESTQUEUE_H__
