
#ifndef __HTTPCONNECTION_H__
#define __HTTPCONNECTION_H__


#include <Elastos.CoreLibrary_server.h>

using Elastos::IO::IInputStream;
using Elastos::IO::IOutputStream;

namespace Elastos {
namespace Net {
namespace Http {

class HttpConnection
    : public ElRefBase
    , public IHttpConnection
{
public:
    class Address
        : public ElRefBase
        , public IHttpConnectionAddress
    {
    public:
        CAR_INTERFACE_DECL()

        // Address(
        //     /* [in] */ IURI* uri,
        //     /* [in] */ ISSLSocketFactory* sslSocketFactory);

        // Address(
        //     /* [in] */ IURI* uri,
        //     /* [in] */ ISSLSocketFactory* sslSocketFactory,
        //     /* [in] */ IProxy* proxy,
        //     /* [in] */ Boolean requiresTunnel);

        CARAPI GetProxy(
            /* [out] */ IProxy** outproxy);

        CARAPI Equals(
            /* [in] */ IInterface* other,
            /* [out] */ Boolean* value);

        CARAPI GetHashCode(
            /* [out] */ Int32* value);

        CARAPI Connect(
            /* [in] */ Int32 connectTimeout,
            /* [out] */ IHttpConnection** hc);

    public:
        AutoPtr<IProxy> mProxy;
        Boolean mRequiresTunnel;
        String mUriHost;
        Int32 mUriPort;
        String mSocketHost;
        Int32 mSocketPort;
        // SSLSocketFactory sslSocketFactory;
    };

    CAR_INTERFACE_DECL()

    CARAPI CloseSocketAndStreams();

    CARAPI SetSoTimeout(
        /* [in] */ Int32 readTimeout);

    CARAPI GetOutputStream(
        /* [out] */ IOutputStream** os);

    CARAPI GetInputStream(
        /* [out] */ IInputStream** is);

    CARAPI GetAddress(
        /* [out] */ IHttpConnectionAddress** hca);

    // SetupSecureSocket(SSLSocketFactory sslSocketFactory, boolean tlsTolerant);

    // public SSLSocket verifySecureSocketHostname(HostnameVerifier hostnameVerifier);

    // public SSLSocket getSecureSocketIfConnected();

    CARAPI IsRecycled(
        /* [out] */ Boolean* value);

    CARAPI SetRecycled();

    // static HttpConnection connect(URI uri, SSLSocketFactory sslSocketFactory, Proxy proxy, boolean requiresTunnel, int connectTimeout);

    CARAPI_(AutoPtr<ISocket>) GetSocket();

    CARAPI_(Boolean) IsEligibleForRecycling();

private:
    HttpConnection(
        /* [in] */ IHttpConnectionAddress* config,
        /* [in] */ Int32 connectTimeout);

private:
    AutoPtr<IHttpConnectionAddress> mAddress;
    AutoPtr<ISocket> mSocket;
    AutoPtr<IInputStream> mInputStream;
    AutoPtr<IOutputStream> mOutputStream;
    // SSLSocket unverifiedSocket;
    // SSLSocket sslSocket;
    AutoPtr<IInputStream> mSslInputStream;
    AutoPtr<IOutputStream> mSslOutputStream;
    Boolean mRecycled; // = false;
};

} // namespace Http
} // namespace Net
} // namespace Elastos

#endif //__HTTPCONNECTION_H__
