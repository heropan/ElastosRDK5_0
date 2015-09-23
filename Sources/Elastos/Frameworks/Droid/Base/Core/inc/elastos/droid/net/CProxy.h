
#ifndef __ELASTOS_DROID_NET_CPROXY_H__
#define __ELASTOS_DROID_NET_CPROXY_H__

#include "_Elastos_Droid_Net_CProxy.h"
#include "elastos/droid/ext/frameworkext.h"
#include <elastos/utility/etl/List.h>

using Elastos::Net::IInetAddress;
using Elastos::Net::IInetSocketAddress;
using Elastos::Net::ISocketAddress;
using Elastos::Net::IProxySelector;
using Elastos::Net::IURI;
using Elastos::Droid::Content::IContext;
using Elastos::Utility::Regex::IMatcher;
using Elastos::Utility::Regex::IPattern;
using Elastos::Utility::Regex::IPatternHelper;
using Elastos::Utility::Etl::List;
using Org::Apache::Http::IHttpHost;
using Org::Apache::Http::Conn::Routing::IHttpRoutePlanner;
using Org::Apache::Http::Protocol::IHttpContext;
using namespace Org::Apache::Http;

namespace Elastos {
namespace Droid {
namespace Net {

/**
 * A convenience class for accessing the user and default proxy
 * settings.
 */

CarClass(CProxy)
{
public:

    CProxy();
    ~CProxy();

    /**
    * Return the proxy object to be used for the URL given as parameter.
    * @param ctx A Context used to get the settings for the proxy host.
    * @param url A URL to be accessed. Used to evaluate exclusion list.
    * @return Proxy (java.net) object containing the host name. If the
    *         user did not set a hostname it returns the default host.
    *         A null value means that no host is to be used.
    * {@hide}
    */
    CARAPI GetProxy(
        /* [in] */ IContext* ctx,
        /* [in] */ const String& url,
        /* [out] */ Elastos::Net::IProxy** outProxy);

    /**
    * Return the proxy host set by the user.
    * @param ctx A Context used to get the settings for the proxy host.
    * @return String containing the host name. If the user did not set a host
    *         name it returns the default host. A null value means that no
    *         host is to be used.
    * @deprecated Use standard java vm proxy values to find the host, port
    *         and exclusion list.  This call ignores the exclusion list.
    */
    CARAPI GetHost(
        /* [in] */ IContext* ctx,
        /* [out] */ String* host);

    /**
    * Return the proxy port set by the user.
    * @param ctx A Context used to get the settings for the proxy port.
    * @return The port number to use or -1 if no proxy is to be used.
    * @deprecated Use standard java vm proxy values to find the host, port
    *         and exclusion list.  This call ignores the exclusion list.
    */
    CARAPI GetPort(
        /* [in] */ IContext* ctx,
        /* [out] */ Int32* portNum);

    /**
    * Return the default proxy host specified by the carrier.
    * @return String containing the host name or null if there is no proxy for
    * this carrier.
    * @deprecated Use standard java vm proxy values to find the host, port and
    *         exclusion list.  This call ignores the exclusion list and no
    *         longer reports only mobile-data apn-based proxy values.
    */
    CARAPI GetDefaultHost(
        /* [out] */ String* host);

    /**
    * Return the default proxy port specified by the carrier.
    * @return The port number to be used with the proxy host or -1 if there is
    * no proxy for this carrier.
    * @deprecated Use standard java vm proxy values to find the host, port and
    *         exclusion list.  This call ignores the exclusion list and no
    *         longer reports only mobile-data apn-based proxy values.
    */
    CARAPI GetDefaultPort(
        /* [out] */ Int32* portNum);

    /**
    * Returns the preferred proxy to be used by clients. This is a wrapper
    * around {@link android.net.Proxy#getHost()}.
    *
    * @param context the context which will be passed to
    * {@link android.net.Proxy#getHost()}
    * @param url the target URL for the request
    * @note Calling this method requires permission
    * android.permission.ACCESS_NETWORK_STATE
    * @return The preferred proxy to be used by clients, or null if there
    * is no proxy.
    * {@hide}
    */
    CARAPI GetPreferredHttpHost(
        /* [in] */ IContext* context,
        /* [in] */ const String& url,
        /* [out] */ IHttpHost** httpHost);

    /**
    * Validate syntax of hostname, port and exclusion list entries
    * {@hide}
    */
    CARAPI Validate(
        /* [in] */ const String& hostname,
        /* [in] */ const String& port,
        /* [in] */ const String& exclList);

    /** @hide */
    CARAPI GetAndroidProxySelectorRoutePlanner(
        /* [in] */ IContext* context,
        /* [out] */ IHttpRoutePlanner** planner);

    /** @hide */
    CARAPI SetHttpProxySystemProperty(
        /* [in] */ IProxyProperties* p);

    /** @hide */
    CARAPI SetHttpProxySystemProperty(
        /* [in] */ const String& host,
        /* [in] */ const String& port,
        /* [in] */ const String& exclList);


private:

    // Set to true to enable extra debugging.
    static const Boolean DEBUG;
    static const String TAG;

    AutoPtr<Elastos::Net::IProxyHelper> proxyhelper;
    static AutoPtr<IConnectivityManager> mConnectivityManager;
    // Hostname / IP REGEX validation
    // Matches blank input, ips, and domain names
    static const String NAME_IP_REGEX;

    static const String HOSTNAME_REGEXP;

    static AutoPtr<IPattern> HOSTNAME_PATTERN;

    static const String EXCLLIST_REGEXP;

    static AutoPtr<IPattern> EXCLLIST_PATTERN;

    class AndroidProxySelectorRoutePlanner
        : public ElRefBase
        // , public org.apache.http.impl.conn.ProxySelectorRoutePlanner
   {
    public:
    //     AndroidProxySelectorRoutePlanner(
    //         /* [in] */ ISchemeRegistry * schreg,
    //         /* [in] */ IProxySelector * prosel,
    //         /* [in] */ IContext * context,
    //         /* [in] */ CProxy * host);

        ~AndroidProxySelectorRoutePlanner();

        //@Override
        //CARAPI_(AutoPtr<IHttpRoute>) DetermineRoute(
        //        /* [in] */ IHttpHost* target,
        //        /* [in] */ IHttpRequest* request,
        //        /* [in] */ IHttpContext* context);

    protected:
        //@Override
        CARAPI_(AutoPtr<Elastos::Net::IProxy>) ChooseProxy(
            /* [in] */ List<Elastos::Net::IProxy*> proxies,
            /* [in] */ IHttpHost* target,
            /* [in] */ IHttpRequest* request,
            /* [in] */ IHttpContext* context);
        //@Override
        CARAPI_(AutoPtr<IHttpHost>) DetermineProxy(
            /* [in] */ IHttpHost* target,
            /* [in] */ IHttpRequest* request,
            /* [in] */ IHttpContext* context);

    private:
        AutoPtr<IContext> mContext;
        CProxy* mHost;
    };

    static Boolean IsLocalHost(
        /* [in] */ const String& host);

};

} //namespace Net
} //namespace Droid
} //namespace Elastos

#endif // __ELASTOS_DROID_NET_CPROXY_H__
