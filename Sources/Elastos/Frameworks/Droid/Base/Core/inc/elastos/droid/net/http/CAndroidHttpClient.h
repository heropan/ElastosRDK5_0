
#ifndef __ELASTOS_DROID_NET_HTTP_CANDROIDHTTPCLIENT_H__
#define __ELASTOS_DROID_NET_HTTP_CANDROIDHTTPCLIENT_H__

#include "_Elastos_Droid_Net_Http_CAndroidHttpClient.h"
#include "ext/frameworkext.h"
// #include "DefaultHttpClient.h"

using namespace Elastos::Core;
using namespace Org::Apache::Http;
using namespace Org::Apache::Http::Protocol;

using Org::Apache::Http::Conn::IClientConnectionManager;
using Org::Apache::Http::Client::Methods::IHttpUriRequest;
using Org::Apache::Http::Params::IHttpParams;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

/**
 * Implementation of the Apache {@link DefaultHttpClient} that is configured with
 * reasonable default settings and registered schemes for Android, and
 * also lets the user add {@link HttpRequestInterceptor} classes.
 * Don't create this directly, use the {@link #newInstance} factory method.
 *
 * <p>This client processes cookies but does not retain them by default.
 * To retain cookies, simply add a cookie store to the HttpContext:</p>
 *
 * <pre>context.setAttribute(ClientContext.COOKIE_STORE, cookieStore);</pre>
 */
CarClass(CAndroidHttpClient)
{
    friend class CAndroidHttpClientHelper;

private:
    class HttpRequestInterceptor
        : public ElRefBase
        , public IObject
        , public IHttpRequestInterceptor
    {
    public:
        CAR_INTERFACE_DECL();

        CARAPI Aggregate(
            /* [in] */ AggrType aggrType,
            /* [in] */ PInterface pObject);

        CARAPI GetDomain(
            /* [out] */ PInterface *ppObject);

        CARAPI GetClassID(
            /* [out] */ ClassID *pCLSID);

        CARAPI Equals(
            /* [in] */ IInterface* other,
            /* [out] */ Boolean * result);

        CARAPI GetHashCode(
            /* [out] */ Int32* hash);

        CARAPI ToString(
            /* [out] */ String* info);

        CARAPI Process(
           /* [in] */ IHttpRequest* request,
           /* [in] */ IHttpContext* context);
    };

    class LocalDefaultHttpClient
        // TODO:
        // : public Org::Apache::Http::Impl::Client::DefaultHttpClient
    {
    protected:
        CARAPI CreateHttpProcessor(
            /* [out] */ IBasicHttpProcessor** processor);

        CARAPI CreateHttpContext(
            /* [out] */ IHttpContext** httpContext);
    };

    /**
     * Logging tag and level.
     */
    class LoggingConfiguration
    {
    private:
        const String mTag;
        const Int32 mLevel;

    public:
        LoggingConfiguration(
            /* [in] */ const String& tag,
            /* [in] */ Int32 level);

        /**
         * Returns true if logging is turned on for this configuration.
         */
        Boolean IsLoggable();

        /**
         * Prints a message using this configuration.
         */
        CARAPI Println(
            /* [in] */ const String& message);
    };

    /**
     * Logs cURL commands equivalent to requests.
     */
    class CurlLogger
        : public ElRefBase
        , public IObject
        , public IHttpRequestInterceptor
    {
    public:
        CurlLogger(
            /* [in] */ LoggingConfiguration* parent);

        CAR_INTERFACE_DECL();

        CARAPI Aggregate(
            /* [in] */ AggrType aggrType,
            /* [in] */ PInterface pObject);

        CARAPI GetDomain(
            /* [out] */ PInterface *ppObject);

        CARAPI GetClassID(
            /* [out] */ ClassID *pCLSID);

        CARAPI Process(
           /* [in] */ IHttpRequest* request,
           /* [in] */ IHttpContext* context);

    private:
        LoggingConfiguration* mLogConfig;
    };

public:
    CAndroidHttpClient();

    ~CAndroidHttpClient();

    constructor(
        /* [in] */ IClientConnectionManager* ccm,
        /* [in] */ IHttpParams* params);

    CARAPI Close();

    /**
     * Enables cURL request logging for this client.
     *
     * @param name to log messages with
     * @param level at which to log messages (see {@link android.util.Log})
     */
    CARAPI EnableCurlLogging(
        /* [in] */ const String& name,
        /* [in] */ Int32 level);

    /**
     * Disables cURL logging for this client.
     */
    CARAPI DisableCurlLogging();

    CARAPI GetParams(
        /* [out] */ IHttpParams** params);

    CARAPI GetConnectionManager(
        /* [out] */ IClientConnectionManager** manager);

    CARAPI Execute(
        /* [in] */ IHttpUriRequest* request,
        /* [out] */ IHttpResponse** response);

    CARAPI Execute(
        /* [in] */ IHttpUriRequest* request,
        /* [in] */ IHttpContext* context,
        /* [out] */ IHttpResponse** response);

    CARAPI Execute(
        /* [in] */ IHttpHost* target,
        /* [in] */ IHttpRequest* request,
        /* [out] */ IHttpResponse** response);

    CARAPI Execute(
        /* [in] */ IHttpHost* target,
        /* [in] */ IHttpRequest* request,
        /* [in] */ IHttpContext* context,
        /* [out] */ IHttpResponse** response);

    // CARAPI Execute(
    //     /* [in] */ IHttpUriRequest* request,
    //     /* [in] */ IResponseHandler<? extends T>* responseHandler
    //     /* [out] */ <T> T** response);

    // CARAPI Execute(
    //     /* [in] */ IHttpUriRequest* request,
    //     /* [in] */ IResponseHandler<? extends T>* responseHandler,
    //     /* [in] */ IHttpContext* context,
    //     /* [out] */ <T> T** response);

    // CARAPI Execute(
    //     /* [in] */ IHttpHost* target,
    //     /* [in] */ IHttpRequest* request,
    //     /* [in] */ IResponseHandler<? extends T>* responseHandler,
    //     /* [out] */ <T> T** response);

    // CARAPI Execute(
    //     /* [in] */ IHttpHost* target,
    //     /* [in] */ IHttpRequest* request,
    //     /* [in] */ IResponseHandler<? extends T>* responseHandler,
    //     /* [in] */ IHttpContext* context,
    //     /* [out] */ <T> T** response);

private:
    static ArrayOf<String>* InitTextContentTypes();

    CAndroidHttpClient(
        /* [in] */ IClientConnectionManager* ccm,
        /* [in] */ IHttpParams* params);

    /**
     * Generates a cURL command equivalent to the given request.
     */
    static CARAPI ToCurl(
        /* [in] */ IHttpUriRequest* request,
        /* [in] */ Boolean logAuthToken,
        /* [out] */ String* curl);

    static CARAPI_(Boolean) IsBinaryContent(
        /* [in] */ IHttpUriRequest* request);

private:
    // Default connection and socket timeout of 60 seconds.  Tweak to taste.
    static const Int32 SOCKET_OPERATION_TIMEOUT;

    static const String TAG;

    static AutoPtr<ArrayOf<String> > mTextContentTypes;

    static const HttpRequestInterceptor* mThreadCheckInterceptor;

    AutoPtr<IHttpClient> mDelegate;

    /** cURL logging configuration. */
    /*volatile*/ LoggingConfiguration* mCurlConfiguration;

};

}
}
}
}

#endif // __ELASTOS_DROID_NET_HTTP_CANDROIDHTTPCLIENT_H__
