
#ifndef __CREQUESTQUEUE_H__
#define __CREQUESTQUEUE_H__

#include "_CRequestQueue.h"
#include "RequestQueue.h"

using Elastos::Utility::IObjectStringMap;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

/**
 * {@hide}
 */
CarClass(CRequestQueue), public RequestQueue
{
public:
    /**
     * A RequestQueue class instance maintains a set of queued
     * requests.  It orders them, makes the requests against HTTP
     * servers, and makes callbacks to supplied eventHandlers as data
     * is read.  It supports request prioritization, connection reuse
     * and pipelining.
     *
     * @param context application context
     */
    CARAPI constructor(
        /* [in] */ Elastos::Droid::Content::IContext* context);

    /**
     * A RequestQueue class instance maintains a set of queued
     * requests.  It orders them, makes the requests against HTTP
     * servers, and makes callbacks to supplied eventHandlers as data
     * is read.  It supports request prioritization, connection reuse
     * and pipelining.
     *
     * @param context application context
     * @param connectionCount The number of simultaneous connections
     */
    CARAPI constructor(
        /* [in] */ Elastos::Droid::Content::IContext* context,
        /* [in] */ Int32 connectionCount);

    /**
     * Enables data state and proxy tracking
     */
    CARAPI EnablePlatformNotifications();

    /**
     * If platform notifications have been enabled, call this method
     * to disable before destroying RequestQueue
     */
    CARAPI DisablePlatformNotifications();

    /**
     * used by webkit
     * @return proxy host if set, null otherwise
     */
    CARAPI GetProxyHost(
        /* [out] */ Org::Apache::Http::IHttpHost** host);

    /**
     * Queues an HTTP request
     * @param url The url to load.
     * @param method "GET" or "POST."
     * @param headers A hashmap of http headers.
     * @param eventHandler The event handler for handling returned
     * data.  Callbacks will be made on the supplied instance.
     * @param bodyProvider InputStream providing HTTP body, null if none
     * @param bodyLength length of body, must be 0 if bodyProvider is null
     */
    CARAPI QueueRequest(
        /* [in] */ const String& url,
        /* [in] */ const String& method,
        /* [in] */ IObjectStringMap* headers,
        /* [in] */ Elastos::Droid::Net::Http::IEventHandler* eventHandler,
        /* [in] */ Elastos::IO::IInputStream* bodyProvider,
        /* [in] */ Int32 bodyLength,
        /* [out] */ Elastos::Droid::Net::Http::IRequestHandle** handle);

    /**
     * Queues an HTTP request
     * @param url The url to load.
     * @param uri The uri of the url to load.
     * @param method "GET" or "POST."
     * @param headers A hashmap of http headers.
     * @param eventHandler The event handler for handling returned
     * data.  Callbacks will be made on the supplied instance.
     * @param bodyProvider InputStream providing HTTP body, null if none
     * @param bodyLength length of body, must be 0 if bodyProvider is null
     */
    CARAPI QueueRequest(
        /* [in] */ const String& url,
        /* [in] */ Elastos::Droid::Net::IWebAddress* uri,
        /* [in] */ const String& method,
        /* [in] */ IObjectStringMap* headers,
        /* [in] */ Elastos::Droid::Net::Http::IEventHandler* eventHandler,
        /* [in] */ Elastos::IO::IInputStream* bodyProvider,
        /* [in] */ Int32 bodyLength,
        /* [out] */ Elastos::Droid::Net::Http::IRequestHandle** handle);

    CARAPI QueueSynchronousRequest(
        /* [in] */ const String& url,
        /* [in] */ Elastos::Droid::Net::IWebAddress* uri,
        /* [in] */ const String& method,
        /* [in] */ IObjectStringMap* headers,
        /* [in] */ Elastos::Droid::Net::Http::IEventHandler* eventHandler,
        /* [in] */ Elastos::IO::IInputStream* bodyProvider,
        /* [in] */ Int32 bodyLength,
        /* [out] */ Elastos::Droid::Net::Http::IRequestHandle** handle);

    /*
     * RequestFeeder implementation
     */
    CARAPI GetRequest(
        /* [out] */ IRequest** request);

    /**
     * @return a request for given host if possible
     */
    CARAPI GetRequest(
        /* [in] */ Org::Apache::Http::IHttpHost* host,
        /* [out] */ IRequest** request);

    /**
     * @return true if a request for this host is available
     */
    CARAPI HaveRequest(
        /* [in] */ Org::Apache::Http::IHttpHost* host,
        /* [out] */ Boolean* haveRequest);

    /**
     * Put request back on head of queue
     */
    CARAPI RequeueRequest(
        /* [in] */ IRequest* request);

    /**
     * This must be called to cleanly shutdown RequestQueue
     */
    CARAPI Shutdown();

    CARAPI StartTiming();

    CARAPI StopTiming();

private:
    /* default simultaneous connection count */
    static const Int32 CONNECTION_COUNT;

};

}
}
}
}

#endif // __CREQUESTQUEUE_H__
