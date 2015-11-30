
#include "elastos/droid/net/Proxy.h"

namespace Elastos {
namespace Droid {
namespace Net {

const String Proxy::NAME_IP_REGEX = String("[a-zA-Z0-9]+(\\-[a-zA-Z0-9]+)*(\\.[a-zA-Z0-9]+(\\-[a-zA-Z0-9]+)*)*");
const String Proxy::EXCL_REGEX = String("[a-zA-Z0-9*]+(\\-[a-zA-Z0-9*]+)*(\\.[a-zA-Z0-9*]+(\\-[a-zA-Z0-9*]+)*)*");
const String Proxy::EXCLLIST_REGEXP = String("^$|^") + EXCL_REGEX + "(," + EXCL_REGEX + ")*$";
const Boolean Proxy::DEBUG = FALSE;
const String Proxy::TAG = String("Proxy");
AutoPtr<IConnectivityManager> Proxy::sConnectivityManager = NULL;
const String Proxy::HOSTNAME_REGEXP = String("^$|^") + NAME_IP_REGEX + "$";

const AutoPtr<IPattern> Proxy::HOSTNAME_PATTERN = getHOSTNAME_PATTERN();
const AutoPtr<IPattern> Proxy::EXCLLIST_PATTERN = getEXCLLIST_PATTERN();
const AutoPtr<IProxySelector> Proxy::DEFAULT_PROXY_SELECTOR = getDEFAULT_PROXY_SELECTOR();

AutoPtr<IPattern> Proxy::getHOSTNAME_PATTERN()
{
    AutoPtr<IPattern> rev;
#if 0 // TODO: Translate codes below
        HOSTNAME_PATTERN = Pattern.compile(HOSTNAME_REGEXP);
#else
    assert(0);
#endif
    return rev;
}

AutoPtr<IPattern> Proxy::getEXCLLIST_PATTERN()
{
    AutoPtr<IPattern> rev;
#if 0 // TODO: Translate codes below
        EXCLLIST_PATTERN = Pattern.compile(EXCLLIST_REGEXP);
#else
    assert(0);
#endif
    return rev;
}

AutoPtr<IProxySelector> Proxy::getDEFAULT_PROXY_SELECTOR()
{
    AutoPtr<IProxySelector> rev;
#if 0 // TODO: Translate codes below
        sDefaultProxySelector = ProxySelector.getDefault();
#else
    assert(0);
#endif
    return rev;
}

ECode Proxy::GetProxy(
    /* [in] */ IContext* ctx,
    /* [in] */ const String& url,
    /* [out] */ Elastos::Net::IProxy** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        String host = "";
        if ((url != NULL) && !isLocalHost(host)) {
            URI uri = URI.create(url);
            ProxySelector proxySelector = ProxySelector.getDefault();
            List<java.net.Proxy> proxyList = proxySelector.select(uri);
            if (proxyList.size() > 0) {
                return proxyList.get(0);
            }
        }
        return java.net.Proxy.NO_PROXY;
#endif
}

ECode Proxy::GetHost(
    /* [in] */ IContext* ctx,
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        java.net.Proxy proxy = getProxy(ctx, NULL);
        if (proxy == java.net.Proxy.NO_PROXY) return NULL;
        try {
            return ((InetSocketAddress)(proxy.address())).getHostName();
        } catch (Exception e) {
            return NULL;
        }
#endif
}

ECode Proxy::GetPort(
    /* [in] */ IContext* ctx,
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        java.net.Proxy proxy = getProxy(ctx, NULL);
        if (proxy == java.net.Proxy.NO_PROXY) return -1;
        try {
            return ((InetSocketAddress)(proxy.address())).getPort();
        } catch (Exception e) {
            return -1;
        }
#endif
}

ECode Proxy::GetDefaultHost(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        String host = System.getProperty("http.proxyHost");
        if (TextUtils.isEmpty(host)) return NULL;
        return host;
#endif
}

ECode Proxy::GetDefaultPort(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (getDefaultHost() == NULL) return -1;
        try {
            return Integer.parseInt(System.getProperty("http.proxyPort"));
        } catch (NumberFormatException e) {
            return -1;
        }
#endif
}

ECode Proxy::GetPreferredHttpHost(
    /* [in] */ IContext* context,
    /* [in] */ const String& url,
    /* [out] */ IHttpHost** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        java.net.Proxy prefProxy = getProxy(context, url);
        if (prefProxy.equals(java.net.Proxy.NO_PROXY)) {
            return NULL;
        } else {
            InetSocketAddress sa = (InetSocketAddress)prefProxy.address();
            return new HttpHost(sa.getHostName(), sa.getPort(), "http");
        }
#endif
}

ECode Proxy::IsLocalHost(
    /* [in] */ const String& host,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (host == NULL) {
            return FALSE;
        }
        try {
            if (host != NULL) {
                if (host.equalsIgnoreCase("localhost")) {
                    return TRUE;
                }
                if (NetworkUtils.numericToInetAddress(host).isLoopbackAddress()) {
                    return TRUE;
                }
            }
        } catch (IllegalArgumentException iex) {
        }
        return FALSE;
#endif
}

ECode Proxy::Validate(
    /* [in] */ const String& hostname,
    /* [in] */ const String& port,
    /* [in] */ const String& exclList,
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        Matcher match = HOSTNAME_PATTERN.matcher(hostname);
        Matcher listMatch = EXCLLIST_PATTERN.matcher(exclList);
        if (!match.matches()) return PROXY_HOSTNAME_INVALID;
        if (!listMatch.matches()) return PROXY_EXCLLIST_INVALID;
        if (hostname.length() > 0 && port.length() == 0) return PROXY_PORT_EMPTY;
        if (port.length() > 0) {
            if (hostname.length() == 0) return PROXY_HOSTNAME_EMPTY;
            Int32 portVal = -1;
            try {
                portVal = Integer.parseInt(port);
            } catch (NumberFormatException ex) {
                return PROXY_PORT_INVALID;
            }
            if (portVal <= 0 || portVal > 0xFFFF) return PROXY_PORT_INVALID;
        }
        return PROXY_VALID;
#endif
}

ECode Proxy::SetHttpProxySystemProperty(
    /* [in] */ IProxyInfo* p)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        String host = NULL;
        String port = NULL;
        String exclList = NULL;
        Uri pacFileUrl = Uri.EMPTY;
        if (p != NULL) {
            host = p.getHost();
            port = Integer.toString(p.getPort());
            exclList = p.getExclusionListAsString();
            pacFileUrl = p.getPacFileUrl();
        }
        setHttpProxySystemProperty(host, port, exclList, pacFileUrl);
#endif
}

ECode Proxy::SetHttpProxySystemProperty(
    /* [in] */ const String& host,
    /* [in] */ const String& port,
    /* [in] */ const String& exclList,
    /* [in] */ IUri* pacFileUrl)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (exclList != NULL) exclList = exclList.replace(",", "|");
        if (FALSE) Log.d(TAG, "setHttpProxySystemProperty :"+host+":"+port+" - "+exclList);
        if (host != NULL) {
            System.setProperty("http.proxyHost", host);
            System.setProperty("https.proxyHost", host);
        } else {
            System.clearProperty("http.proxyHost");
            System.clearProperty("https.proxyHost");
        }
        if (port != NULL) {
            System.setProperty("http.proxyPort", port);
            System.setProperty("https.proxyPort", port);
        } else {
            System.clearProperty("http.proxyPort");
            System.clearProperty("https.proxyPort");
        }
        if (exclList != NULL) {
            System.setProperty("http.nonProxyHosts", exclList);
            System.setProperty("https.nonProxyHosts", exclList);
        } else {
            System.clearProperty("http.nonProxyHosts");
            System.clearProperty("https.nonProxyHosts");
        }
        if (!Uri.EMPTY.equals(pacFileUrl)) {
            ProxySelector.setDefault(new PacProxySelector());
        } else {
            ProxySelector.setDefault(sDefaultProxySelector);
        }
#endif
}


} // namespace Net
} // namespace Droid
} // namespace Elastos

#if 0 // old CProxy.cpp
#include "elastos/droid/net/CProxy.h"
#include "elastos/droid/net/NetworkUtils.h"
#include "elastos/droid/net/CConnectivityManager.h"
#include "elastos/droid/text/TextUtils.h"
#include <elastos/core/StringUtils.h>

using Elastos::Core::StringUtils;
using Elastos::Core::ICharSequence;
using Elastos::Core::CString;
using Elastos::Core::ISystem;
using Elastos::Core::CSystem;
using Elastos::Utility::Regex::CPatternHelper;
using Elastos::Net::CURI;
using Elastos::Droid::Text::TextUtils;
using Elastos::Droid::Net::NetworkUtils;
using Elastos::Droid::Net::CConnectivityManager;

namespace Elastos {
namespace Droid {
namespace Net {

const String CProxy::TAG("Proxy");
const Boolean CProxy::DEBUG= FALSE;

// Hostname / IP REGEX validation
// Matches blank input, ips, and domain names
const String CProxy::NAME_IP_REGEX("[a-zA-Z0-9]+(\\-[a-zA-Z0-9]+)*(\\.[a-zA-Z0-9]+(\\-[a-zA-Z0-9]+)*)*");
const String CProxy::HOSTNAME_REGEXP = String("^$|^") + CProxy::NAME_IP_REGEX +  String("$");
const String CProxy::EXCLLIST_REGEXP = String("$|^(.?") + NAME_IP_REGEX  +  String(")+(,(.?") + NAME_IP_REGEX +  String("))*$");
AutoPtr<IConnectivityManager> CProxy::mConnectivityManager = NULL;
AutoPtr<IPattern> CProxy::HOSTNAME_PATTERN = NULL;
AutoPtr<IPattern> CProxy::EXCLLIST_PATTERN = NULL;


/**
 * A convenience class for accessing the user and default proxy
 * settings.
 */

CProxy::CProxy()
{
    AutoPtr<IPatternHelper> patterhelper;
    AutoPtr<IIConnectivityManager> temp;
    CConnectivityManager::New(temp, (IConnectivityManager**)&mConnectivityManager);
    CPatternHelper::AcquireSingleton((IPatternHelper**)&patterhelper);
    patterhelper->Compile(HOSTNAME_REGEXP, (IPattern**)&HOSTNAME_PATTERN);
    patterhelper->Compile(EXCLLIST_REGEXP, (IPattern**)&EXCLLIST_PATTERN);
    Elastos::Net::CProxyHelper::AcquireSingleton((Elastos::Net::IProxyHelper**)&proxyhelper);
}

CProxy::~CProxy(){}

/**
* Return the proxy object to be used for the URL given as parameter.
* @param ctx A Context used to get the settings for the proxy host.
* @param url A URL to be accessed. Used to evaluate exclusion list.
* @return Proxy (java.net) object containing the host name. If the
*         user did not set a hostname it returns the default host.
*         A null value means that no host is to be used.
* {@hide}
*/
ECode CProxy::GetProxy(
    /* [in] */ IContext* ctx,
    /* [in] */ const String& url,
    /* [out] */ Elastos::Net::IProxy** outProxy)
{
    VALIDATE_NOT_NULL(outProxy);
    String host;
    if (!(url.IsNull()))
    {
        AutoPtr<IURI> uri;
        CURI::New((IURI**)&uri);
        if(uri != NULL)
            uri->GetHost(&host);
    }

    if (!IsLocalHost(host))
    {
        if (mConnectivityManager == NULL) {
            ctx->GetSystemService(IContext::CONNECTIVITY_SERVICE, (IInterface**)&mConnectivityManager);
        }
        if (mConnectivityManager == NULL) {
            return proxyhelper->GetNO_PROXY(outProxy);
        }
        AutoPtr<IProxyProperties> proxyProperties;
        mConnectivityManager->GetProxy((IProxyProperties**)&proxyProperties);

        if (proxyProperties != NULL) {
            Boolean bol;
            proxyProperties->IsExcluded(host, &bol);
            if (!bol) {
                return proxyProperties->MakeProxy(outProxy);
            }
        }
    }
    return proxyhelper->GetNO_PROXY(outProxy);
}

/**
* Return the proxy host set by the user.
* @param ctx A Context used to get the settings for the proxy host.
* @return String containing the host name. If the user did not set a host
*         name it returns the default host. A null value means that no
*         host is to be used.
* @deprecated Use standard java vm proxy values to find the host, port
*         and exclusion list.  This call ignores the exclusion list.
*/
ECode CProxy::GetHost(
    /* [in] */ IContext* ctx,
    /* [out] */ String* host)
{
    VALIDATE_NOT_NULL(host);
    AutoPtr<Elastos::Net::IProxy> proxy;
    AutoPtr<Elastos::Net::IProxy> noproxy;
    proxyhelper->GetNO_PROXY((Elastos::Net::IProxy**)&noproxy);
    String str;
    GetProxy(ctx, str, (Elastos::Net::IProxy**)&proxy);
    if (proxy == noproxy) {
        *host = NULL;
        return NOERROR;
    }
    AutoPtr<ISocketAddress> socketAddress;
    proxy->GetAddress((ISocketAddress**)&socketAddress);
    AutoPtr<IInetSocketAddress> address = IInetSocketAddress::Probe(socketAddress);
    return address->GetHostName(host);
}

/**
* Return the proxy port set by the user.
* @param ctx A Context used to get the settings for the proxy port.
* @return The port number to use or -1 if no proxy is to be used.
* @deprecated Use standard java vm proxy values to find the host, port
*         and exclusion list.  This call ignores the exclusion list.
*/
ECode CProxy::GetPort(
    /* [in] */ IContext* ctx,
    /* [out] */ Int32* portNum)
{
    VALIDATE_NOT_NULL(portNum);
    AutoPtr<Elastos::Net::IProxy> proxy;
    AutoPtr<Elastos::Net::IProxy> noproxy;
    proxyhelper->GetNO_PROXY((Elastos::Net::IProxy**)&noproxy);
    String str;
    GetProxy(ctx, str, (Elastos::Net::IProxy**)&proxy);
    if (proxy == noproxy) {
         *portNum = -1;
         return NOERROR;
    }
    AutoPtr<ISocketAddress> socketAddress;
    proxy->GetAddress((ISocketAddress**)&socketAddress);
    AutoPtr<IInetSocketAddress> inetaddress;
    AutoPtr<IInetSocketAddress> address = IInetSocketAddress::Probe(socketAddress);
    return address->GetPort(portNum);
}

/**
* Return the default proxy host specified by the carrier.
* @return String containing the host name or null if there is no proxy for
* this carrier.
* @deprecated Use standard java vm proxy values to find the host, port and
*         exclusion list.  This call ignores the exclusion list and no
*         longer reports only mobile-data apn-based proxy values.
*/
ECode CProxy::GetDefaultHost(
    /* [out] */ String* host)
{
    VALIDATE_NOT_NULL(host);
    AutoPtr<ISystem> system;
    Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);
    system->GetProperty(String("http.proxyHost"), host);
    return NOERROR;
}

/**
* Return the default proxy port specified by the carrier.
* @return The port number to be used with the proxy host or -1 if there is
* no proxy for this carrier.
* @deprecated Use standard java vm proxy values to find the host, port and
*         exclusion list.  This call ignores the exclusion list and no
*         longer reports only mobile-data apn-based proxy values.
*/
ECode CProxy::GetDefaultPort(
    /* [out] */ Int32* port)
{
    VALIDATE_NOT_NULL(port);
    String host;
    GetDefaultHost(&host);
    if (host.IsNull()) {
        *port = -1;
        return NOERROR;
    }

    AutoPtr<ISystem> system;
    Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);
    String portS;
    system->GetProperty(String("http.proxyPort"), &portS);
    *port = StringUtils::ParseInt32(portS);
    return NOERROR;
}

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
ECode CProxy::GetPreferredHttpHost(
    /* [in] */ IContext* context,
    /* [in] */ const String& url,
    /* [out] */ IHttpHost** httpHost)
{
    VALIDATE_NOT_NULL(httpHost);
    AutoPtr<Elastos::Net::IProxy> prefProxy;
    AutoPtr<Elastos::Net::IProxy> noproxy;
    proxyhelper->GetNO_PROXY((Elastos::Net::IProxy**)&noproxy);
    GetProxy(context, url, (Elastos::Net::IProxy**)&prefProxy);
    if (prefProxy == noproxy) {
        *httpHost = NULL;
        return NOERROR;
    } else {
        AutoPtr<ISocketAddress> socketaddress;
        prefProxy->GetAddress((ISocketAddress**)&socketaddress);
        AutoPtr<IInetSocketAddress> sa = IInetSocketAddress::Probe(socketaddress);
        String hostName;
        sa->GetHostName(&hostName);
        Int32 portNum;
        sa->GetPort(&portNum);
        assert(0);
        // TODO:
        // return CHttpHost::New(hostName, portNum, "http", httpHost);
    }
    return NOERROR;
}

Boolean CProxy::IsLocalHost(
    /* [in] */ const String& host)
{
    if (host.IsNull()) {
        return FALSE;
    }

    if (!(host.IsNull())) {
        if (host.EqualsIgnoreCase("localhost")) {
            return TRUE;
        }
        AutoPtr<IInetAddress> address;
        NetworkUtils::NumericToInetAddress(host, (IInetAddress**)&address);
        Boolean isLoopback;
        address->IsLoopbackAddress(&isLoopback);
        if (isLoopback == TRUE) {
            return TRUE;
        }
    }

    return FALSE;
}

/**
* Validate syntax of hostname, port and exclusion list entries
* {@hide}
*/
ECode CProxy::Validate(
    /* [in] */ const String& hostname,
    /* [in] */ const String& port,
    /* [in] */ const String& exclList)
{
    AutoPtr<IMatcher> match;
    HOSTNAME_PATTERN->Matcher(hostname, (IMatcher**)&match);
    AutoPtr<IMatcher> listMatch;
    EXCLLIST_PATTERN->Matcher(exclList, (IMatcher**)&listMatch);

    Boolean isMatch;
    match->Matches(&isMatch);
    if (!isMatch) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Boolean isListMatch;
    listMatch->Matches(&isListMatch);
    if (!isListMatch) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Int32 hostlength = hostname.GetLength();
    Int32 portlength = port.GetLength();
    if (hostlength > 0 && portlength == 0) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (portlength > 0) {
        if (hostlength == 0) {
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
        Int32 portVal = -1;
        portVal = StringUtils::ParseInt32(port);

        if (portVal <= 0 || portVal > 0xFFFF) {
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
    }
    return NOERROR;
}

/** @hide */
ECode CProxy::GetAndroidProxySelectorRoutePlanner(
    /* [in] */ IContext* context,
    /* [out] */ IHttpRoutePlanner** planner)
{
//    VALIDATE_NOT_NULL(planner);
//    AutoPtr<ISchemeRegistry> reg;
//    CSchemeRegistry::New(&reg);
//    AutoPtr<IProxySelector> sel;
//    ProxySelector::GetDefault((IProxySelector**)&sel);
//    (AndroidProxySelectorRoutePlanner*)*planner = new AndroidProxySelectorRoutePlanner(reg, sel, context, this);
//    REFCOUNT_ADD(*planner);
//    return NOERROR;
    assert(0);
    return E_NOT_IMPLEMENTED;
}

/** @hide */
ECode CProxy::SetHttpProxySystemProperty(
    /* [in] */ IProxyProperties* proxyp)
{
    String host;
    String port;
    String exclList;
    if (proxyp != NULL) {
        proxyp->GetHost(&host);
        Int32 portNum;
        proxyp->GetPort(&portNum);
        port = StringUtils::ToString(portNum);
        proxyp->GetExclusionList(&exclList);
    }
    SetHttpProxySystemProperty(host, port, exclList);
    return NOERROR;
}

/** @hide */
ECode CProxy::SetHttpProxySystemProperty(
    /* [in] */ const String& host,
    /* [in] */ const String& port,
    /* [in] */ const String& exclList)
{
    // AutoPtr<ISystem> system;
    // Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);
    // String host;
    // system->GetProperty(String("http.proxyHost"), &host);

//    if (!(exclList.IsNull()))
//    exclList = exclList.el(",","|");
//    if (FALSE)
//    Log.d(TAG, "setHttpProxySystemProperty :"+host+":"+port+" - "+exclList);
//    if (host != NULL) {
//    System::SetProperty("http.proxyHost", host);
//    System::SetProperty("https.proxyHost", host);
//    } else {
//    System::ClearProperty("http.proxyHost");
//    System::ClearProperty("https.proxyHost");
//    }
//    if (port != NULL) {
//    System::SetProperty("http.proxyPort", port);
//    System::SetProperty("https.proxyPort", port);
//    } else {
//    System::ClearProperty("http.proxyPort");
//    System::ClearProperty("https.proxyPort");
//    }
//    if (exclList != NULL) {
//    System::SetProperty("http.nonProxyHosts", exclList);
//    System::SetProperty("https.nonProxyHosts", exclList);
//    } else {
//    System::ClearProperty("http.nonProxyHosts");
//    System::ClearProperty("https.nonProxyHosts");
//    }
//    return NOERROR;
    assert(0);
    return E_NOT_IMPLEMENTED;
}

// CProxy::AndroidProxySelectorRoutePlanner::AndroidProxySelectorRoutePlanner(
//     /* [in] */ ISchemeRegistry * schreg,
//     /* [in] */ IProxySelector * prosel,
//     /* [in] */ IContext * context,
//     /* [in] */ CProxy * host)
// {
//     /*// super(schreg, prosel) of org.apache.http.impl.conn.ProxySelectorRoutePlanner...
//     ProxySelectorRoutePlanner::AndroidProxySelectorRoutePlanner(schreg, prosel);//??
//     mContext = context;
//     mHost = host;*/
// }

CProxy::AndroidProxySelectorRoutePlanner::~AndroidProxySelectorRoutePlanner()
{}

//@Override
//IHttpRoute* CProxy::AndroidProxySelectorRoutePlanner::DetermineRoute(
//    /* [in] */ IHttpHost* target,
//    /* [in] */ IHttpRequest* request,
//    /* [in] */ IHttpContext* context)
//{
//    String hostName;
//    target->GetHostName(&hostName);

//    AutoPtr<IHttpHost> proxy;
//    mHost->GetPreferredHttpHost(mContext, hostName, &proxy);
//    AutoPtr<IHttpRoute> route;
//    if (proxy == NULL) {
//        CHttpRoute::New(target, &route);
//   } else {
//        CHttpRoute::New(target, NULL, proxy, FALSE, &route);
//    }
//    return route;
//    assert(0);
//}

//@Override
AutoPtr<Elastos::Net::IProxy> CProxy::AndroidProxySelectorRoutePlanner::ChooseProxy(
    /* [in] */ List<Elastos::Net::IProxy*> proxies,
    /* [in] */ IHttpHost* target,
    /* [in] */ IHttpRequest* request,
    /* [in] */ IHttpContext* context)
{
//String hostName;
//target->GetHostName(&hostName);
//    AutoPtr<Elastos::Net::IProxy> proxy;
//    mHost->GetProxy(mContext, &hostName, &proxy);
//    return proxy;
    assert(0);
    return NULL;
}

//@Override
AutoPtr<IHttpHost> CProxy::AndroidProxySelectorRoutePlanner::DetermineProxy(
    /* [in] */ IHttpHost* target,
    /* [in] */ IHttpRequest* request,
    /* [in] */ IHttpContext* context)
{
//    AutoPtr<IHttpHost> host;
//    String hostName;
//     target->GetHostName(&hostName);
//     mHost->GetPreferredHttpHost(mContext, &hostName, &host);
//     return host;
    assert(0);
    return NULL;
}


} //namespace Net
} //namespace Droid
} //namespace Elastos
#endif