
#include "cmdef.h"
#include "ProxySelectorImpl.h"
#include "CProxy.h"
#include "CInetSocketAddress.h"
// #include <Com.Kortide.Platform.h>
#include <elastos/StringUtils.h>
#include <elastos/StringBuffer.h>
#include <CObjectContainer.h>

using Elastos::Core::CObjectContainer;
using Elastos::Core::StringBuffer;
using Elastos::Core::StringUtils;

namespace Elastos {
namespace Net {

const Int32 ProxySelectorImpl::HTTP_PROXY_PORT;
const Int32 ProxySelectorImpl::HTTPS_PROXY_PORT;
const Int32 ProxySelectorImpl::FTP_PROXY_PORT;
const Int32 ProxySelectorImpl::SOCKS_PROXY_PORT;

//    static AutoPtr<IProperties> mNetProps;// = null;

// read net.properties file
    // static {
    //     AccessController.doPrivileged(new java.security.PrivilegedAction() {
    //         public Object run() {
    //             File f = new File(System.getProperty("java.home")
    //                     + File.separator + "lib" + File.separator
    //                     + "net.properties");

    //             if (f.exists()) {
    //                 try {
    //                     FileInputStream fis = new FileInputStream(f);
    //                     InputStream is = new BufferedInputStream(fis);
    //                     netProps = new Properties();
    //                     netProps.load(is);
    //                     is.close();
    //                 } catch (IOException e) {
    //                 }
    //             }
    //             return null;
    //         }
    //     });
    // }

PInterface ProxySelectorImpl::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IProxySelector) {
        return (IProxySelector*)this;
    }

    return NULL;
}

UInt32 ProxySelectorImpl::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ProxySelectorImpl::Release()
{
    return ElRefBase::Release();
}

ECode ProxySelectorImpl::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode ProxySelectorImpl::ConnectFailed(
    /* [in] */ IURI* uri,
    /* [in] */ ISocketAddress* sa,
    /* [in] */ ECode ec)
{
    if (uri == NULL || sa == NULL || SUCCEEDED(ec)) {
//        throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    return NOERROR;
}

ECode ProxySelectorImpl::Select(
    /* [in] */ IURI* uri,
    /* [out] */ IObjectContainer** container)
{
    VALIDATE_NOT_NULL(container);

    // argument check
    if (uri == NULL) {
//        throw new IllegalArgumentException("uri == null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    // check scheme
    String scheme;
    uri->GetScheme(&scheme);
    if (scheme.IsNull()) {
//        throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    String host;
    uri->GetHost(&host);
    AutoPtr<IProxy> proxy = CProxy::NO_PROXY;

    if (CString("http").Equals(scheme)) {
        proxy = SelectHttpProxy(host);
    }
    else if (CString("https").Equals(scheme)) {
        proxy = SelectHttpsProxy();
    }
    else if (CString("ftp").Equals(scheme)) {
        proxy = SelectFtpProxy(host);
    }
    else if (CString("socket").Equals(scheme)) {
        proxy = SelectSocksProxy();
    }

    FAIL_RETURN(CObjectContainer::New(container));
    (*container)->Add(proxy);
    return NOERROR;
}

AutoPtr<IProxy> ProxySelectorImpl::SelectHttpProxy(
    /* [in] */ const String& uriHost)
{
    String host;
    String port;
    ProxyType type = ProxyType_DIRECT;

    String nonProxyHosts = GetSystemProperty(String("http.nonProxyHosts"));
    // if host is in non proxy host list, returns Proxy.NO_PROXY
    if (IsNonProxyHost(uriHost, nonProxyHosts)) {
        return CProxy::NO_PROXY;
    }

    host = GetSystemProperty(String("http.proxyHost"));
    if (!host.IsNull()) {
        // case 1: http.proxyHost is set, use exact http proxy
        type = ProxyType_HTTP;
        port = GetSystemPropertyOrAlternative(String("http.proxyPort"),
                String("proxyPort"), StringUtils::Int32ToString(HTTP_PROXY_PORT));
    }
    else if (!(host = GetSystemProperty(String("proxyHost"), String(NULL))).IsNull()) {
        // case 2: proxyHost is set, use exact http proxy
        type = ProxyType_HTTP;
        port = GetSystemPropertyOrAlternative(String("proxyPort"),
                String("http.proxyPort"), StringUtils::Int32ToString(HTTP_PROXY_PORT));

    }
    else if (!(host = GetSystemProperty(String("socksProxyHost"))).IsNull()) {
        // case 3: use socks proxy instead
        type = ProxyType_SOCKS;
        port = GetSystemProperty(
                String("socksProxyPort"), StringUtils::Int32ToString(SOCKS_PROXY_PORT));
    }
    Int32 defaultPort = (type == ProxyType_SOCKS) ? SOCKS_PROXY_PORT
            : HTTP_PROXY_PORT;
    return CreateProxy(type, host, port, defaultPort);
}

/*
 * Gets proxy for https request.
 */
AutoPtr<IProxy> ProxySelectorImpl::SelectHttpsProxy()
{
    String host;
    String port;
    ProxyType type = ProxyType_DIRECT;

    host = GetSystemProperty(String("https.proxyHost"));
    if (!host.IsNull()) {
        // case 1: use exact https proxy
        type = ProxyType_HTTP;
        port = GetSystemProperty(
                String("https.proxyPort"), StringUtils::Int32ToString(HTTPS_PROXY_PORT));
    }
    else {
        host = GetSystemProperty(String("socksProxyHost"));
        if (!host.IsNull()) {
            // case 2: use socks proxy instead
            type = ProxyType_SOCKS;
            port = GetSystemProperty(
                    String("socksProxyPort"), StringUtils::Int32ToString(SOCKS_PROXY_PORT));
        }
    }
    Int32 defaultPort = (type == ProxyType_SOCKS) ? SOCKS_PROXY_PORT
            : HTTPS_PROXY_PORT;
    return CreateProxy(type, host, port, defaultPort);
}

/*
 * Gets proxy for ftp request.
 */
AutoPtr<IProxy> ProxySelectorImpl::SelectFtpProxy(
    /* [in] */ const String& uriHost)
{
    String host;
    String port;
    ProxyType type = ProxyType_DIRECT;
    String nonProxyHosts = GetSystemProperty(String("ftp.nonProxyHosts"));
    // if host is in non proxy host list, returns Proxy.NO_PROXY
    if (IsNonProxyHost(uriHost, nonProxyHosts)) {
        return CProxy::NO_PROXY;
    }

    host = GetSystemProperty(String("ftp.proxyHost"));
    if (!host.IsNull()) {
        // case 1: use exact ftp proxy
        type = ProxyType_HTTP;
        port = GetSystemProperty(
                String("ftp.proxyPort"), StringUtils::Int32ToString(FTP_PROXY_PORT));
    }
    else {
        host = GetSystemProperty(String("socksProxyHost"));
        if (!host.IsNull()) {
            // case 2: use socks proxy instead
            type = ProxyType_SOCKS;
            port = GetSystemProperty(
                    String("socksProxyPort"), StringUtils::Int32ToString(SOCKS_PROXY_PORT));
        }
    }
    Int32 defaultPort = (type == ProxyType_SOCKS) ? SOCKS_PROXY_PORT
            : FTP_PROXY_PORT;
    return CreateProxy(type, host, port, defaultPort);
}

/*
 * Gets proxy for socks request.
 */
AutoPtr<IProxy> ProxySelectorImpl::SelectSocksProxy()
{
    String host;
    String port;
    ProxyType type = ProxyType_DIRECT;

    host = GetSystemProperty(String("socksProxyHost"));
    if (!host.IsNull()) {
        type = ProxyType_SOCKS;
        port = GetSystemProperty(
                String("socksProxyPort"), StringUtils::Int32ToString(SOCKS_PROXY_PORT));
    }
    return CreateProxy(type, host, port, SOCKS_PROXY_PORT);
}

/*
 * checks whether the host needs proxy. return true if it doesn't need a
 * proxy.
 */
Boolean ProxySelectorImpl::IsNonProxyHost(
    /* [in] */ const String& host,
    /* [in] */ const String& nonProxyHosts)
{
    // nonProxyHosts is not set
    if (host.IsNull() || nonProxyHosts.IsNull()) {
        return FALSE;
    }
    // Construct regex expression of nonProxyHosts
    Char32 ch;
    StringBuffer buf;
    AutoPtr<ArrayOf<Char32> > char32Array = nonProxyHosts.GetChars();
    for (Int32 i = 0; (UInt32)i < char32Array->GetLength(); i++) {
        ch = (*char32Array)[i];
        switch (ch) {
            case '.':
                buf += "\\.";
                break;
            case '*':
                buf += ".*";
                break;
            default:
                buf += ch;
        }
    }
    String nonProxyHostsReg;
    buf.ToString(&nonProxyHostsReg);
    // check whether the host is the nonProxyHosts.
    Boolean isflag = FALSE;
    StringUtils::Matches(host, nonProxyHostsReg, &isflag);
    return isflag;
}

/*
 * Create Proxy by "type","host" and "port".
 */
AutoPtr<IProxy> ProxySelectorImpl::CreateProxy(
    /* [in] */ ProxyType type,
    /* [in] */ const String& host,
    /* [in] */ const String& port,
    /* [in] */ Int32 defaultPort)
{
    AutoPtr<IProxy> proxy;
    if (type == ProxyType_DIRECT) {
        proxy = CProxy::NO_PROXY;
    }
    else {
        Int32 iPort;
//        try {
            // BEGIN android-changed
        iPort = StringUtils::ParseInt32(port);
        if (iPort == -1) {
            iPort = defaultPort;
        }
            // END android-changed
//        } catch (NumberFormatException e) {
//            iPort = defaultPort;
//        }
        AutoPtr<IInetSocketAddress> address;
        ASSERT_SUCCEEDED(CInetSocketAddress::CreateUnresolved(
                host, iPort, (IInetSocketAddress**)&address));
        ASSERT_SUCCEEDED(CProxy::New(type,
                ISocketAddress::Probe(address), (IProxy**)&proxy));
    }
    return proxy;
}

/*
 * gets system property, privileged operation. If the value of the property
 * is null or empty String, it returns defaultValue.
 */
String ProxySelectorImpl::GetSystemProperty(
    /* [in] */ const String& property)
{
    return GetSystemProperty(property, String(NULL));
}

/*
 * gets system property, privileged operation. If the value of the property
 * is null or empty String, it returns defaultValue.
 */
String ProxySelectorImpl::GetSystemProperty(
    /* [in] */ const String& property,
    /* [in] */ const String& defaultValue)
{
    // String value = AccessController.doPrivileged(new PriviAction<String>(
    //         property));
    // if (value.IsNull() || value.IsEmpty()) {
    //     value = (!mNetProps.IsNull())
    //             ? netProps.getProperty(property, defaultValue)
    //             : defaultValue;
    // }
    // return value;
    return String(NULL);
}

/*
 * gets system property, privileged operation. If the value of "key"
 * property is null, then retrieve value from "alternative" property.
 * Finally, if the value is null or empty String, it returns defaultValue.
 */
String ProxySelectorImpl::GetSystemPropertyOrAlternative(
    /* [in] */ const String& key,
    /* [in] */ const String& alternativeKey,
    /* [in] */ const String& defaultValue)
{
    String value = GetSystemProperty(key);
    if (value.IsNull()) {
        value = GetSystemProperty(alternativeKey);
        if (value.IsNull()) {
            value = defaultValue;
        }
    }
    return value;
}

} // namespace Net
} // namespace Elastos
