
#ifndef __ELASTOS_DROID_NET_CPROXY_H__
#define __ELASTOS_DROID_NET_CPROXY_H__

#include "_Elastos_Droid_Net_CProxy.h"
#include <elastos/core/Singleton.h>

using Org::Apache::Http::IHttpHost;
using Elastos::Droid::Content::IContext;

namespace Elastos {
namespace Droid {
namespace Net {

/**
 * A convenience class for accessing the user and default proxy
 * settings.
 */
CarClass(CProxy)
    , public Singleton
    , public IProxy
{
public:
    CAR_INTERFACE_DECL()

    CAR_SINGLETON_DECL()

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
        /* [out] */ Elastos::Net::IProxy** result);

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
        /* [out] */ String* result);

    /**
     * Return the proxy port set by the user.
     * @param ctx A Context used to get the settings for the proxy port.
     * @return The port number to use or -1 if no proxy is to be used.
     * @deprecated Use standard java vm proxy values to find the host, port
     *         and exclusion list.  This call ignores the exclusion list.
     */
    CARAPI GetPort(
        /* [in] */ IContext* ctx,
        /* [out] */ Int32* result);

    /**
     * Return the default proxy host specified by the carrier.
     * @return String containing the host name or null if there is no proxy for
     * this carrier.
     * @deprecated Use standard java vm proxy values to find the host, port and
     *         exclusion list.  This call ignores the exclusion list and no
     *         longer reports only mobile-data apn-based proxy values.
     */
    CARAPI GetDefaultHost(
        /* [out] */ String* result);

    /**
     * Return the default proxy port specified by the carrier.
     * @return The port number to be used with the proxy host or -1 if there is
     * no proxy for this carrier.
     * @deprecated Use standard java vm proxy values to find the host, port and
     *         exclusion list.  This call ignores the exclusion list and no
     *         longer reports only mobile-data apn-based proxy values.
     */
    CARAPI GetDefaultPort(
        /* [out] */ Int32* result);

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
        /* [out] */ IHttpHost** result);

    /**
     * Validate syntax of hostname, port and exclusion list entries
     * {@hide}
     */
    CARAPI Validate(
        /* [in] */ const String& hostname,
        /* [in] */ const String& port,
        /* [in] */ const String& exclList,
        /* [out] */ Int32* result);

    /** @hide */
    CARAPI SetHttpProxySystemProperty(
        /* [in] */ IProxyInfo* p);

    /** @hide */
    CARAPI SetHttpProxySystemProperty(
        /* [in] */ const String& host,
        /* [in] */ const String& port,
        /* [in] */ const String& exclList,
        /* [in] */ IUri* pacFileUrl);

};

} // namespace Net
} // namespace Droid
} // namespace Elastos

#endif //  __ELASTOS_DROID_NET_CPROXY_H__

