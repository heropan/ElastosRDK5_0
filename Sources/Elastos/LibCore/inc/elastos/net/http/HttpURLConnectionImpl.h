
#ifndef __HTTPURLCONNECTIONIMPL_H__
#define __HTTPURLCONNECTIONIMPL_H__


#include <Elastos.CoreLibrary_server.h>
#include <HttpURLConnection.h>

using Elastos::Utility::IMap;

namespace Elastos {
namespace Net {
namespace Http {

class HttpURLConnectionImpl
    : public ElRefBase
    , public HttpURLConnection
    , public IHttpURLConnectionImpl
{
public:
    CAR_INTERFACE_DECL()

    CARAPI Connect();

    CARAPI Disconnect();

    CARAPI GetErrorStream(
        /* [out] */ IInputStream** stream);

    CARAPI GetHeaderField(
        /* [in] */ Int32 position,
        /* [out] */ String* str);

    // Permission getPermission();

    CARAPI GetHeaderField(
        /* [in] */ const String& fieldName,
        /* [out] */ String* str);

    CARAPI GetHeaderFieldKey(
        /* [in] */ Int32 position,
        /* [out] */ String* str);

    CARAPI GetHeaderFields(
        /* [out] */ IMap** strmap);

    CARAPI GetRequestProperties(
        /* [out] */ IMap** strmap);

    CARAPI GetInputStream(
        /* [out] */ IInputStream** is);

    CARAPI GetOutputStream(
        /* [out] */ IOutputStream** os);

    CARAPI GetRequestProperty(
        /* [in] */ const String& field,
        /* [out] */ String* str);

    CARAPI UsingProxy(
        /* [out] */ Boolean* value);

    CARAPI GetResponseMessage(
        /* [out] */ String* str);

    CARAPI GetResponseCode(
        /* [out] */ Int32* value);

    CARAPI SetRequestProperty(
        /* [in] */ const String& field,
        /* [in] */ const String& newValue);

    CARAPI AddRequestProperty(
        /* [in] */ const String& field,
        /* [in] */ const String& value);

    CARAPI GetHttpEngine(
        /* [out] */ IHttpEngine** he);

    CARAPI ProcessAuthHeader(
        /* [in] */ Int32 responseCode,
        /* [in] */ IResponseHeaders* response,
        /* [in] */ IRawHeaders* successorRequestHeaders,
        /* [out] */ Boolean* value);

    CARAPI GetDefaultPort(
        /* [out] */ Int32* value);

    CARAPI GetFixedContentLength(
        /* [out] */ Int32* value);

    CARAPI GetChunkLength(
        /* [out] */ Int32* value);

    CARAPI GetProxy(
        /* [out] */ IProxy** pro);

    CARAPI SetProxy(
        /* [in] */ IProxy* proxy);

    CARAPI GetAllowUserInteraction(
        /* [out] */ Boolean* value);

    CARAPI GetContent(
        /* [out] */ IInterface** outface);

    CARAPI GetContentEncoding(
        /* [out] */ String* str);

    CARAPI GetContentLength(
        /* [out] */ Int32* value);

    CARAPI GetContentType(
        /* [out] */ String* str);

    CARAPI GetDate(
        /* [out] */ Int64* value);

    CARAPI GetDefaultUseCaches(
        /* [out] */ Boolean* value);

    CARAPI GetDoInput(
        /* [out] */ Boolean* value);

    CARAPI GetDoOutput(
        /* [out] */ Boolean* value);

    CARAPI GetExpiration(
        /* [out] */ Int64* value);

    CARAPI GetHeaderFieldByPosition(
        /* [in] */ Int32 invalue,
        /* [out] */ String* str);

    CARAPI GetHeaderFieldByKey(
        /* [in] */ const String& instr,
        /* [out] */ String* str);

    CARAPI GetHeaderFieldDate(
        /* [in] */ const String& instr,
        /* [in] */ Int64 invalue,
        /* [out] */ Int64* value);

    CARAPI GetHeaderFieldInt32(
        /* [in] */ const String& instr,
        /* [in] */ Int32 invalue,
        /* [out] */ Int32* value);

    CARAPI GetIfModifiedSince(
        /* [out] */ Int64* value);

    CARAPI GetLastModified(
        /* [out] */ Int64* value);

    CARAPI GetURL(
        /* [out] */ IURL** url);

    CARAPI GetUseCaches(
        /* [out] */ Boolean* value);

    CARAPI SetAllowUserInteraction(
        /* [in] */ Boolean value);

    CARAPI SetDefaultUseCaches(
        /* [in] */ Boolean value);

    CARAPI SetDoInput(
        /* [in] */ Boolean value);

    CARAPI SetDoOutput(
        /* [in] */ Boolean value);

    CARAPI SetIfModifiedSince(
        /* [in] */ Int64 value);

    CARAPI SetUseCaches(
        /* [in] */ Boolean value);

    CARAPI SetConnectTimeout(
        /* [in] */ Int32 value);

    CARAPI GetConnectTimeout(
        /* [out] */ Int32* value);

    CARAPI SetReadTimeout(
        /* [in] */ Int32 value);

    CARAPI GetReadTimeout(
        /* [out] */ Int32* value);

    CARAPI GetRequestMethod(
        /* [out] */ String* str);

    CARAPI SetRequestMethod(
        /* [in] */ const String& instr);

    CARAPI GetInstanceFollowRedirects(
        /* [in] */ Boolean* value);

    CARAPI SetInstanceFollowRedirects(
        /* [in] */ Boolean value);

    CARAPI SetFixedLengthStreamingMode(
        /* [in] */ Int32 value);

    CARAPI SetChunkedStreamingMode(
        /* [in] */ Int32 value);

    HttpURLConnectionImpl(
        /* [in] */ IURL* url,
        /* [in] */ Int32 port);

    HttpURLConnectionImpl(
        /* [in] */ IURL* url,
        /* [in] */ Int32 port,
        /* [in] */ IProxy* proxy);

    CARAPI_(AutoPtr<IHttpEngine>) NewHttpEngine(
        /* [in] */ const String& method,
        /* [in] */ IRawHeaders* requestHeaders,
        /* [in] */ IHttpConnection* connection,
        /* [in] */ IRetryableOutputStream* requestBody);

    enum Retry {
        NONE,
        SAME_CONNECTION,
        DIFFERENT_CONNECTION
    };
private:
    CARAPI_(String) GetConnectToHost();

    CARAPI_(Int32) GetConnectToPort();

    CARAPI_(void) InitHttpEngine();

    CARAPI_(AutoPtr<IHttpEngine>) GetResponse();

    CARAPI_(Retry) ProcessResponseHeaders();

    CARAPI_(String) GetAuthorizationCredentials(
        /* [in] */ IRawHeaders* responseHeaders,
        /* [in] */ const String& challengeHeader);

    CARAPI_(AutoPtr<IInetAddress>) GetConnectToInetAddress();

private:
    Int32 mDefaultPort;

    AutoPtr<IProxy> mProxy;

    AutoPtr<IRawHeaders> mRawRequestHeaders; // = new RawHeaders();

    Int32 mRedirectionCount;

    // protected IOException httpEngineFailure;
protected:
    AutoPtr<IHttpEngine> mHttpEngine;
};

} // namespace Http
} // namespace Net
} // namespace Elastos

#endif //__HTTPURLCONNECTIONIMPL_H__
