
#ifndef __ELASTOS_NET_HTTPURLCONNECTION_H__
#define __ELASTOS_NET_HTTPURLCONNECTION_H__

#include "URLConnection.h"

namespace Elastos {
namespace Net {

//extern "C" const InterfaceID EIID_HttpURLConnection;

class HttpURLConnection
    : public URLConnection
    , public IHttpURLConnection
{
public:
    CAR_INTERFACE_DECL()

    virtual CARAPI Disconnect() = 0;

    virtual CARAPI GetErrorStream(
        /* [out] */ IInputStream** stream);

    static CARAPI_(Boolean) GetFollowRedirects();

//        public java.security.Permission getPermission();

    virtual CARAPI GetRequestMethod(
        /* [out] */ String* method);

    virtual CARAPI GetResponseCode(
        /* [out] */ Int32* responseCode);

    virtual CARAPI GetResponseMessage(
        /* [out] */ String* message);

    static CARAPI SetFollowRedirects(
        /* [in] */ Boolean followRedirects);

    virtual CARAPI SetRequestMethod(
        /* [in] */ const String& method);

    virtual CARAPI UsingProxy(
        /* [out] */ Boolean* usingProxy) = 0;

    CARAPI GetContentEncoding(
        /* [out] */ String* encoding);

    virtual CARAPI GetInstanceFollowRedirects(
        /* [out] */ Boolean* followRedirects);

    virtual CARAPI SetInstanceFollowRedirects(
        /* [in] */ Boolean followRedirects);

    CARAPI GetHeaderFieldDate(
        /* [in] */ const String& field,
        /* [in] */ Int64 defaultValue,
        /* [out] */ Int64* value);

    virtual CARAPI SetFixedLengthStreamingMode(
        /* [in] */ Int32 contentLength);

    virtual CARAPI SetChunkedStreamingMode(
        /* [in] */ Int32 chunkLength);

protected:
    HttpURLConnection();

    CARAPI Init(
        /* [in] */ IURL* url);

protected:
    String mMethod;

    Int32 mResponseCode;

    String mResponseMessage;

    Boolean mInstanceFollowRedirects;

    Int32 mChunkLength;

    Int32 mFixedContentLength;

private:
    static Boolean sFollowRedirects;
};

} // namespace Net
} // namespace Elastos

#endif //__ELASTOS_NET_HTTPURLCONNECTION_H__
