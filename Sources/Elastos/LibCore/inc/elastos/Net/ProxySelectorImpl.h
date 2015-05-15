
#ifndef __PROXYSELECTORIMPL_H__
#define __PROXYSELECTORIMPL_H__

#include "ProxySelector.h"
#include "Elastos.Core_server.h"

namespace Elastos {
namespace Net {

class ProxySelectorImpl
    : public ElRefBase
    , public IProxySelector
    , public ProxySelector
{
public:
    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    //@Override
    CARAPI ConnectFailed(
        /* [in] */ IURI* uri,
        /* [in] */ ISocketAddress* sa,
        /* [in] */ ECode ec);

    //@Override
    CARAPI Select(
        /* [in] */ IURI* uri,
        /* [out] */ IObjectContainer** container);

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

private:
    CARAPI_(AutoPtr<IProxy>) SelectHttpProxy(
        /* [in] */ const String& uriHost);

    CARAPI_(AutoPtr<IProxy>) SelectHttpsProxy();

    CARAPI_(AutoPtr<IProxy>) SelectFtpProxy(
        /* [in] */ const String& uriHost);

    CARAPI_(AutoPtr<IProxy>) SelectSocksProxy();

    CARAPI_(Boolean) IsNonProxyHost(
        /* [in] */ const String& host,
        /* [in] */ const String& nonProxyHosts);

    CARAPI_(AutoPtr<IProxy>) CreateProxy(
        /* [in] */ ProxyType type,
        /* [in] */ const String& host,
        /* [in] */ const String& port,
        /* [in] */ Int32 defaultPort);

    CARAPI_(String) GetSystemProperty(
        /* [in] */ const String& property);

    CARAPI_(String) GetSystemProperty(
        /* [in] */ const String& property,
        /* [in] */ const String& defaultValue);

    CARAPI_(String) GetSystemPropertyOrAlternative(
        /* [in] */ const String& key,
        /* [in] */ const String& alternativeKey,
        /* [in] */ const String& defaultValue);

private:
    static const Int32 HTTP_PROXY_PORT = 80;

    static const Int32 HTTPS_PROXY_PORT = 443;

    static const Int32 FTP_PROXY_PORT = 80;

    static const Int32 SOCKS_PROXY_PORT = 1080;

    // Net properties read from net.properties file.
//    static AutoPtr<IProperties> mNetProps;// = null;
};

} // namespace Net
} // namespace Elastos

#endif //__PROXYSELECTORIMPL_H__
