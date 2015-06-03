
#include <HttpURLConnectionImpl.h>
#include <IoUtils.h>
#include <CHttpEngine.h>
#include <CURL.h>
#include <InetAddress.h>

using Elastos::IO::IoUtils;

namespace Elastos {
namespace Net {
namespace Http {

CAR_INTERFACE_IMPL(HttpURLConnectionImpl, IHttpURLConnectionImpl)

ECode HttpURLConnectionImpl::Connect()
{
    InitHttpEngine();
    // try {
    return mHttpEngine->SendRequest();
    // } catch (IOException e) {
    //     mHttpEngineFailure = e;
    //     throw e;
    // }
}

ECode HttpURLConnectionImpl::Disconnect()
{
    // Calling disconnect() before a connection exists should have no effect.
    if (mHttpEngine != NULL) {
        // We close the response body here instead of in
        // HttpEngine.release because that is called when input
        // has been completely read from the underlying socket.
        // However the response body can be a GZIPInputStream that
        // still has unread data.
        Boolean isflag = FALSE;
        if (mHttpEngine->HasResponse(&isflag), isflag) {
            AutoPtr<IInputStream> is;
            mHttpEngine->GetResponseBody((IInputStream**)&is);
            IoUtils::CloseQuietly(is);
        }
        mHttpEngine->ToRelease(FALSE);
    }
    return NOERROR;
}

ECode HttpURLConnectionImpl::GetErrorStream(
    /* [out] */ IInputStream** stream)
{
    VALIDATE_NOT_NULL(stream)

    // try {
    AutoPtr<IHttpEngine> response = GetResponse();
    Boolean isflag = FALSE;
    Int32 value = 0;
    if ((response->HasResponseBody(&isflag), isflag)
            && (response->GetResponseCode(&value), value >= HTTP_BAD_REQUEST)) {
        AutoPtr<IInputStream> is;
        response->GetResponseBody((IInputStream**)&is);
        *stream = is;
        REFCOUNT_ADD(*stream)
        return NOERROR;
    }
    *stream = NULL;
    // } catch (IOException e) {
    //     return null;
    // }
    return NOERROR;
}

ECode HttpURLConnectionImpl::GetHeaderField(
    /* [in] */ Int32 position,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)

    // try {
    AutoPtr<IHttpEngine> response = GetResponse();
    AutoPtr<IResponseHeaders> responseheaders;
    response->GetResponseHeaders((IResponseHeaders**)&responseheaders);
    AutoPtr<IRawHeaders> rhead;
    responseheaders->GetHeaders((IRawHeaders**)&rhead);
    return rhead->GetValue(position, str);
    // } catch (IOException e) {
    //     return null;
    // }
}

// Permission getPermission(){
//     String connectToAddress = getConnectToHost() + ":" + getConnectToPort();
//     return new SocketPermission(connectToAddress, "connect, resolve");
// }

ECode HttpURLConnectionImpl::GetHeaderField(
    /* [in] */ const String& fieldName,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)

    // try {
    AutoPtr<IHttpEngine> response = GetResponse();
    AutoPtr<IResponseHeaders> responseheaders;
    response->GetResponseHeaders((IResponseHeaders**)&responseheaders);
    AutoPtr<IRawHeaders> rawHeaders;
    responseheaders->GetHeaders((IRawHeaders**)&rawHeaders);
    return fieldName.IsNull()
            ? rawHeaders->GetStatusLine(str)
            : rawHeaders->Get(fieldName, str);
    // } catch (IOException e) {
    //     return null;
    // }
    return NOERROR;
}

ECode HttpURLConnectionImpl::GetHeaderFieldKey(
    /* [in] */ Int32 position,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)

    // try {
    AutoPtr<IHttpEngine> response = GetResponse();
    AutoPtr<IResponseHeaders> responseheaders;
    response->GetResponseHeaders((IResponseHeaders**)&responseheaders);
    AutoPtr<IRawHeaders> rawHeaders;
    responseheaders->GetHeaders((IRawHeaders**)&rawHeaders);
    return rawHeaders->GetFieldName(position, str);
    // } catch (IOException e) {
    //     return null;
    // }
}

ECode HttpURLConnectionImpl::GetHeaderFields(
    /* [out] */ IMap** strmap)
{
    VALIDATE_NOT_NULL(strmap)

    // try {
    AutoPtr<IHttpEngine> response = GetResponse();
    AutoPtr<IResponseHeaders> responseheaders;
    response->GetResponseHeaders((IResponseHeaders**)&responseheaders);
    AutoPtr<IRawHeaders> rawHeaders;
    responseheaders->GetHeaders((IRawHeaders**)&rawHeaders);
    AutoPtr<IMap> osm;
    ECode ec = rawHeaders->ToMultimap((IMap**)&osm);
    *strmap = osm;
    REFCOUNT_ADD(*strmap)
    // } catch (IOException e) {
    //     return null;
    // }
    return ec;
}

ECode HttpURLConnectionImpl::GetRequestProperties(
    /* [out] */ IMap** strmap)
{
    VALIDATE_NOT_NULL(strmap)

    if (mConnected) {
        // throw new IllegalStateException("Cannot access request header fields after connection is set");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    AutoPtr<IMap> osm;
    ECode ec = mRawRequestHeaders->ToMultimap((IMap**)&osm);
    *strmap = osm;
    REFCOUNT_ADD(*strmap);
    return ec;
}

ECode HttpURLConnectionImpl::GetInputStream(
    /* [out] */ IInputStream** is)
{
    VALIDATE_NOT_NULL(is)

    if (!mDoInput) {
        // throw new ProtocolException("This protocol does not support input");
        return E_PROTOCOL_EXCEPTION;
    }

    AutoPtr<IHttpEngine> response = GetResponse();

    /*
     * if the requested file does not exist, throw an exception formerly the
     * Error page from the server was returned if the requested file was
     * text/html this has changed to return FileNotFoundException for all
     * file types
     */
    Int32 value = 0;
    if (GetResponseCode(&value), value >= HTTP_BAD_REQUEST) {
        // throw new FileNotFoundException(url.toString());
        return E_FILE_NOT_FOUND_EXCEPTION;
    }

    AutoPtr<IInputStream> result;
    response->GetResponseBody((IInputStream**)&result);
    if (result == NULL) {
        // throw new IOException("No response body exists; responseCode=" + getResponseCode());
        return E_IO_EXCEPTION;
    }
    *is = result;
    REFCOUNT_ADD(*is)

    return NOERROR;
}

ECode HttpURLConnectionImpl::GetOutputStream(
    /* [out] */ IOutputStream** os)
{
    Connect();

    AutoPtr<IOutputStream> result;
    mHttpEngine->GetRequestBody((IOutputStream**)&result);
    Boolean isflag = FALSE;
    if (result == NULL) {
        // throw new ProtocolException("method does not support a request body: " + method);
        return E_PROTOCOL_EXCEPTION;
    } else if (mHttpEngine->HasResponse(&isflag), isflag) {
        // throw new ProtocolException("cannot write request body after response has been read");
        return E_PROTOCOL_EXCEPTION;
    }

    *os = result;
    REFCOUNT_ADD(*os)

    return NOERROR;
}

ECode HttpURLConnectionImpl::GetRequestProperty(
    /* [in] */ const String& field,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)

    if (field.IsNull()) {
        *str = NULL;
        return NOERROR;
    }
    return mRawRequestHeaders->Get(field, str);
}

ECode HttpURLConnectionImpl::UsingProxy(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)

    ProxyType outtype;
    *value = (mProxy != NULL && (mProxy->GetType(&outtype), outtype) != ProxyType_DIRECT);
    return NOERROR;
}

ECode HttpURLConnectionImpl::GetResponseMessage(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)

    AutoPtr<IHttpEngine> response = GetResponse();
    AutoPtr<IResponseHeaders> responseheaders;
    response->GetResponseHeaders((IResponseHeaders**)&responseheaders);
    AutoPtr<IRawHeaders> rawHeaders;
    responseheaders->GetHeaders((IRawHeaders**)&rawHeaders);
    return rawHeaders->GetResponseMessage(str);
}

ECode HttpURLConnectionImpl::GetResponseCode(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)

    AutoPtr<IHttpEngine> response = GetResponse();
    return response->GetResponseCode(value);
}

ECode HttpURLConnectionImpl::SetRequestProperty(
    /* [in] */ const String& field,
    /* [in] */ const String& newValue)
{
    if (mConnected) {
        // throw new IllegalStateException("Cannot set request property after connection is made");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    if (field.IsNull()) {
        // throw new NullPointerException("field == null");
        return E_NULL_POINTER_EXCEPTION;
    }
    return mRawRequestHeaders->Set(field, newValue);
}

ECode HttpURLConnectionImpl::AddRequestProperty(
    /* [in] */ const String& field,
    /* [in] */ const String& value)
{
    if (mConnected) {
        // throw new IllegalStateException("Cannot add request property after connection is made");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    if (field.IsNull()) {
        // throw new NullPointerException("field == null");
        return E_NULL_POINTER_EXCEPTION;
    }
    return mRawRequestHeaders->Add(field, value);
}

ECode HttpURLConnectionImpl::GetHttpEngine(
    /* [out] */ IHttpEngine** he)
{
    VALIDATE_NOT_NULL(he)

    *he = mHttpEngine;
    REFCOUNT_ADD(*he)

    return NOERROR;
}

ECode HttpURLConnectionImpl::ProcessAuthHeader(
    /* [in] */ Int32 responseCode,
    /* [in] */ IResponseHeaders* response,
    /* [in] */ IRawHeaders* successorRequestHeaders,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)

    if (responseCode != HTTP_PROXY_AUTH && responseCode != HTTP_UNAUTHORIZED) {
        // throw new IllegalArgumentException();
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    // keep asking for username/password until authorized
    String challengeHeader = responseCode == HTTP_PROXY_AUTH ? String("Proxy-Authenticate") : String("WWW-Authenticate");
    AutoPtr<IRawHeaders> rhead;
    response->GetHeaders((IRawHeaders**)&rhead);
    String credentials = GetAuthorizationCredentials(rhead, challengeHeader);
    if (credentials.IsNull()) {
        *value = FALSE; // could not find credentials, end request cycle
        return NOERROR;
    }

    // add authorization credentials, bypassing the already-connected check
    String fieldName = responseCode == HTTP_PROXY_AUTH ? String("Proxy-Authorization") : String("Authorization");
    successorRequestHeaders->Set(fieldName, credentials);
    *value = TRUE;
    return NOERROR;
}

ECode HttpURLConnectionImpl::GetDefaultPort(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)

    *value = mDefaultPort;
    return NOERROR;
}

ECode HttpURLConnectionImpl::GetFixedContentLength(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)

    *value = mFixedContentLength;
    return NOERROR;
}

ECode HttpURLConnectionImpl::GetChunkLength(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)

    *value = mChunkLength;
    return NOERROR;
}

ECode HttpURLConnectionImpl::GetProxy(
    /* [out] */ IProxy** pro)
{
    VALIDATE_NOT_NULL(pro)

    *pro = mProxy;
    return NOERROR;
}

ECode HttpURLConnectionImpl::SetProxy(
    /* [in] */ IProxy* proxy)
{
    mProxy = proxy;
    return NOERROR;
}

HttpURLConnectionImpl::HttpURLConnectionImpl(
    /* [in] */ IURL* url,
    /* [in] */ Int32 port)
{
    HttpURLConnection::Init(url);
    mDefaultPort = port;
}

HttpURLConnectionImpl::HttpURLConnectionImpl(
    /* [in] */ IURL* url,
    /* [in] */ Int32 port,
    /* [in] */ IProxy* proxy)
{
    HttpURLConnection::Init(url);
    mDefaultPort = port;
    mProxy = proxy;
}

AutoPtr<IHttpEngine> HttpURLConnectionImpl::NewHttpEngine(
    /* [in] */ const String& method,
    /* [in] */ IRawHeaders* requestHeaders,
    /* [in] */ IHttpConnection* connection,
    /* [in] */ IRetryableOutputStream* requestBody)
{
    AutoPtr<IHttpEngine> hc;
    CHttpEngine::New((IHttpURLConnectionImpl*)this->Probe(EIID_IHttpURLConnectionImpl), method, requestHeaders, connection, requestBody, (IHttpEngine**)&hc);
    return hc;
}

String HttpURLConnectionImpl::GetConnectToHost()
{
    AutoPtr<IInetSocketAddress> saddr;
    mProxy->GetAddress((ISocketAddress**)&saddr);
    String str1;
    saddr->GetHostName(&str1);
    AutoPtr<IURL> url;
    HttpURLConnection::GetURL((IURL**)&url);
    String str2;
    url->GetHost(&str2);
    Boolean isflag =  FALSE;
    UsingProxy(&isflag);
    return isflag ? str1 : str2;
}

Int32 HttpURLConnectionImpl::GetConnectToPort()
{
    Boolean isflag =  FALSE;
    UsingProxy(&isflag);
    AutoPtr<IInetSocketAddress> saddr;
    mProxy->GetAddress((ISocketAddress**)&saddr);
    Int32 addport = 0;
    saddr->GetPort(&addport);
    AutoPtr<IURL> url;
    HttpURLConnection::GetURL((IURL**)&url);
    Int32 urlport = 0;
    url->GetPort(&urlport);
    Int32 hostPort = isflag ? addport : urlport;
    Int32 defport = 0;
    return hostPort < 0 ? (GetDefaultPort(&defport), defport) : hostPort;
}

void HttpURLConnectionImpl::InitHttpEngine()
{
    // if (mHttpEngineFailure != NULL) {
    //     throw mHttpEngineFailure;
    // } else
    if (mHttpEngine != NULL) {
        return;
    }

    mConnected = TRUE;
    // try {
    if (mDoOutput) {
        if (mMethod == HttpEngine::GET) {
            // they are requesting a stream to write to. This implies a POST method
            mMethod = HttpEngine::POST;
        } else if (mMethod != HttpEngine::POST && mMethod != HttpEngine::PUT) {
            // If the request method is neither POST nor PUT, then you're not writing
            // throw new ProtocolException(method + " does not support writing");
            return;
        }
    }
    mHttpEngine = NewHttpEngine(mMethod, mRawRequestHeaders, NULL, NULL);
    // } catch (IOException e) {
    //     mHttpEngineFailure = e;
    //     throw e;
    // }
}

AutoPtr<IHttpEngine> HttpURLConnectionImpl::GetResponse()
{
    InitHttpEngine();

    Boolean isflag = FALSE;
    if (mHttpEngine->HasResponse(&isflag), isflag) {
        return mHttpEngine;
    }

    while (TRUE) {
        // try {
        ECode ec1 = mHttpEngine->SendRequest();
        ECode ec2 = mHttpEngine->ReadResponse();
        // } catch (IOException e) {
        if (ec1 != NOERROR || ec2 != NOERROR) {
            /*
             * If the connection was recycled, its staleness may have caused
             * the failure. Silently retry with a different connection.
             */
            AutoPtr<IOutputStream> requestBody;
            mHttpEngine->GetRequestBody((IOutputStream**)&requestBody);

            if ((mHttpEngine->HasRecycledConnection(&isflag), isflag)
                    && (requestBody == NULL || IRetryableOutputStream::Probe(requestBody))) {
                mHttpEngine->ToRelease(FALSE);
                mHttpEngine = NewHttpEngine(mMethod, mRawRequestHeaders, NULL,
                        IRetryableOutputStream::Probe(requestBody));
                continue;
            }
            // mHttpEngineFailure = e;
            // throw e;
        }

        Retry retry = ProcessResponseHeaders();
        if (retry == NONE) {
            mHttpEngine->AutomaticallyReleaseConnectionToPool();
            return mHttpEngine;
        }

        /*
         * The first request was insufficient. Prepare for another...
         */

        String retryMethod = mMethod;
        AutoPtr<IOutputStream> requestBody;
        mHttpEngine->GetRequestBody((IOutputStream**)&requestBody);

        /*
         * Although RFC 2616 10.3.2 specifies that a HTTP_MOVED_PERM
         * redirect should keep the same method, Chrome, Firefox and the
         * RI all issue GETs when following any redirect.
         */
        Int32 responseCode = 0;
        GetResponseCode(&responseCode);
        if (responseCode == HTTP_MULT_CHOICE || responseCode == HTTP_MOVED_PERM
                || responseCode == HTTP_MOVED_TEMP || responseCode == HTTP_SEE_OTHER) {
            retryMethod = HttpEngine::GET;
            requestBody = NULL;
        }

        if (requestBody != NULL && !(IRetryableOutputStream::Probe(requestBody))) {
            // throw new HttpRetryException("Cannot retry streamed HTTP body", mHttpEngine.getResponseCode());
            return NULL;
        }

        if (retry == DIFFERENT_CONNECTION) {
            mHttpEngine->AutomaticallyReleaseConnectionToPool();
        }

        mHttpEngine->ToRelease(TRUE);
        AutoPtr<IHttpConnection> hcoon;
        mHttpEngine->GetConnection((IHttpConnection**)&hcoon);
        mHttpEngine = NewHttpEngine(retryMethod, mRawRequestHeaders, hcoon, IRetryableOutputStream::Probe(requestBody));
    }
}

HttpURLConnectionImpl::Retry HttpURLConnectionImpl::ProcessResponseHeaders()
{
    Int32 responseCode = 0;
    GetResponseCode(&responseCode);
    Boolean isflag = FALSE;
    switch (responseCode) {
        case HTTP_PROXY_AUTH:
            if (UsingProxy(&isflag), !isflag) {
                // throw new IOException("Received HTTP_PROXY_AUTH (407) code while not using proxy");
                assert(0 && "Received HTTP_PROXY_AUTH (407) code while not using proxy!!!\n");
            }
            // fall-through
        case HTTP_UNAUTHORIZED:
        {
            Boolean credentialsFound = FALSE;
            AutoPtr<IResponseHeaders> reshead;
            mHttpEngine->GetResponseHeaders((IResponseHeaders**)&reshead);
            ProcessAuthHeader(responseCode, reshead, mRawRequestHeaders, &credentialsFound);
            return credentialsFound ? SAME_CONNECTION : NONE;
        }
        case HTTP_MULT_CHOICE:
        case HTTP_MOVED_PERM:
        case HTTP_MOVED_TEMP:
        case HTTP_SEE_OTHER:
        {
            if (HttpURLConnection::GetInstanceFollowRedirects(&isflag), !isflag) {
                return NONE;
            }
            if (++mRedirectionCount > HttpEngine::MAX_REDIRECTS) {
                // throw new ProtocolException("Too many redirects");
                assert(0 && "Too many redirects!!!\n");
            }
            String location;
            GetHeaderField(String("Location"), &location);
            if (location.IsNull()) {
                return NONE;
            }
            AutoPtr<IURL> previousUrl = mUrl;
            mUrl = NULL;
            CURL::New(previousUrl, location, (IURL**)&mUrl);
            String str1;
            String str2;
            if (!(previousUrl->GetProtocol(&str1), str1).Equals((mUrl->GetProtocol(&str2), str2))) {
                return NONE; // the scheme changed; don't retry.
            }
            Int32 value1 = 0;
            Int32 value2 = 0;
            if ((previousUrl->GetHost(&str1), str1).Equals((mUrl->GetHost(&str2), str2))
                    && (previousUrl->GetEffectivePort(&value1), value1) == (mUrl->GetEffectivePort(&value2), value2)) {
                return SAME_CONNECTION;
            }
            else {
                return DIFFERENT_CONNECTION;
            }
        }
        default:
            return NONE;
    }
}

String HttpURLConnectionImpl::GetAuthorizationCredentials(
    /* [in] */ IRawHeaders* responseHeaders,
    /* [in] */ const String& challengeHeader)
{
    assert(0 && "TODO");
    // List<Challenge> challenges = HeaderParser.parseChallenges(responseHeaders, challengeHeader);
    // if (challenges.isEmpty()) {
    //     throw new IOException("No authentication challenges found");
    // }

    // for (Challenge challenge : challenges) {
    //     // use the global authenticator to get the password
    //     PasswordAuthentication auth = Authenticator.requestPasswordAuthentication(
    //             getConnectToInetAddress(), getConnectToPort(), url.getProtocol(),
    //             challenge.realm, challenge.scheme);
    //     if (auth == null) {
    //         continue;
    //     }

    //     // base64 encode the username and password
    //     String usernameAndPassword = auth.getUserName() + ":" + new String(auth.getPassword());
    //     byte[] bytes = usernameAndPassword.getBytes(Charsets.ISO_8859_1);
    //     String encoded = Base64.encode(bytes);
    //     return challenge.scheme + " " + encoded;
    // }

    // return null;
    return String(NULL);
}

AutoPtr<IInetAddress> HttpURLConnectionImpl::GetConnectToInetAddress()
{
    Boolean isflag = FALSE;
    UsingProxy(&isflag);
    AutoPtr<IInetAddress> saddr1;
    mProxy->GetAddress((ISocketAddress**)&saddr1);
    AutoPtr<IURL> url;
    HttpURLConnection::GetURL((IURL**)&url);
    String hostname;
    url->GetHost(&hostname);
    AutoPtr<IInetAddress> saddr2;
    InetAddress::GetByName(hostname, (IInetAddress**)&saddr2);
    return isflag ? saddr1 : saddr2;
}

ECode HttpURLConnectionImpl::GetAllowUserInteraction(
    /* [out] */ Boolean* value)
{
    return HttpURLConnection::GetAllowUserInteraction(value);
}

ECode HttpURLConnectionImpl::GetContent(
    /* [out] */ IInterface** outface)
{
    return HttpURLConnection::GetContent(outface);
}

ECode HttpURLConnectionImpl::GetContentEncoding(
    /* [out] */ String* str)
{
    return HttpURLConnection::GetContentEncoding(str);
}

ECode HttpURLConnectionImpl::GetContentLength(
    /* [out] */ Int32* value)
{
    return HttpURLConnection::GetContentLength(value);
}

ECode HttpURLConnectionImpl::GetContentType(
    /* [out] */ String* str)
{
    return HttpURLConnection::GetContentType(str);
}

ECode HttpURLConnectionImpl::GetDate(
    /* [out] */ Int64* value)
{
    return HttpURLConnection::GetDate(value);
}

ECode HttpURLConnectionImpl::GetDefaultUseCaches(
    /* [out] */ Boolean* value)
{
    return HttpURLConnection::GetDefaultUseCaches(value);
}

ECode HttpURLConnectionImpl::GetDoInput(
    /* [out] */ Boolean* value)
{
    return HttpURLConnection::GetDoInput(value);
}

ECode HttpURLConnectionImpl::GetDoOutput(
    /* [out] */ Boolean* value)
{
    return HttpURLConnection::GetDoOutput(value);
}

ECode HttpURLConnectionImpl::GetExpiration(
    /* [out] */ Int64* value)
{
    return HttpURLConnection::GetExpiration(value);
}

ECode HttpURLConnectionImpl::GetHeaderFieldByPosition(
    /* [in] */ Int32 invalue,
    /* [out] */ String* str)
{
    return HttpURLConnection::GetHeaderFieldByPosition(invalue, str);
}

ECode HttpURLConnectionImpl::GetHeaderFieldByKey(
    /* [in] */ const String& instr,
    /* [out] */ String* str)
{
    return HttpURLConnection::GetHeaderFieldByKey(instr, str);
}

ECode HttpURLConnectionImpl::GetHeaderFieldDate(
    /* [in] */ const String& instr,
    /* [in] */ Int64 invalue,
    /* [out] */ Int64* value)
{
    return HttpURLConnection::GetHeaderFieldDate(instr, invalue, value);
}

ECode HttpURLConnectionImpl::GetHeaderFieldInt32(
    /* [in] */ const String& instr,
    /* [in] */ Int32 invalue,
    /* [out] */ Int32* value)
{
    return HttpURLConnection::GetHeaderFieldInt32(instr, invalue, value);
}

ECode HttpURLConnectionImpl::GetIfModifiedSince(
    /* [out] */ Int64* value)
{
    return HttpURLConnection::GetIfModifiedSince(value);
}

ECode HttpURLConnectionImpl::GetLastModified(
    /* [out] */ Int64* value)
{
    return HttpURLConnection::GetLastModified(value);
}

ECode HttpURLConnectionImpl::GetURL(
    /* [out] */ IURL** url)
{
    return HttpURLConnection::GetURL(url);
}

ECode HttpURLConnectionImpl::GetUseCaches(
    /* [out] */ Boolean* value)
{
    return HttpURLConnection::GetUseCaches(value);
}

ECode HttpURLConnectionImpl::SetAllowUserInteraction(
    /* [in] */ Boolean value)
{
    return HttpURLConnection::SetAllowUserInteraction(value);
}

ECode HttpURLConnectionImpl::SetDefaultUseCaches(
    /* [in] */ Boolean value)
{
    return HttpURLConnection::SetDefaultUseCaches(value);
}

ECode HttpURLConnectionImpl::SetDoInput(
    /* [in] */ Boolean value)
{
    return HttpURLConnection::SetDoInput(value);
}

ECode HttpURLConnectionImpl::SetDoOutput(
    /* [in] */ Boolean value)
{
    return HttpURLConnection::SetDoOutput(value);
}

ECode HttpURLConnectionImpl::SetIfModifiedSince(
    /* [in] */ Int64 value)
{
    return HttpURLConnection::SetIfModifiedSince(value);
}

ECode HttpURLConnectionImpl::SetUseCaches(
    /* [in] */ Boolean value)
{
    return HttpURLConnection::SetUseCaches(value);
}

ECode HttpURLConnectionImpl::SetConnectTimeout(
    /* [in] */ Int32 value)
{
    return HttpURLConnection::SetConnectTimeout(value);
}

ECode HttpURLConnectionImpl::GetConnectTimeout(
    /* [out] */ Int32* value)
{
    return HttpURLConnection::GetConnectTimeout(value);
}

ECode HttpURLConnectionImpl::SetReadTimeout(
    /* [in] */ Int32 value)
{
    return HttpURLConnection::SetReadTimeout(value);
}

ECode HttpURLConnectionImpl::GetReadTimeout(
    /* [out] */ Int32* value)
{
    return HttpURLConnection::GetReadTimeout(value);
}

ECode HttpURLConnectionImpl::GetRequestMethod(
    /* [out] */ String* str)
{
    return HttpURLConnection::GetRequestMethod(str);
}

ECode HttpURLConnectionImpl::SetRequestMethod(
    /* [in] */ const String& instr)
{
    return HttpURLConnection::SetRequestMethod(instr);
}

ECode HttpURLConnectionImpl::GetInstanceFollowRedirects(
    /* [in] */ Boolean* value)
{
    return HttpURLConnection::GetInstanceFollowRedirects(value);
}

ECode HttpURLConnectionImpl::SetInstanceFollowRedirects(
    /* [in] */ Boolean value)
{
    return HttpURLConnection::SetInstanceFollowRedirects(value);
}

ECode HttpURLConnectionImpl::SetFixedLengthStreamingMode(
    /* [in] */ Int32 value)
{
    return HttpURLConnection::SetFixedLengthStreamingMode(value);
}

ECode HttpURLConnectionImpl::SetChunkedStreamingMode(
    /* [in] */ Int32 value)
{
    return HttpURLConnection::SetChunkedStreamingMode(value);
}

} // namespace Http
} // namespace Net
} // namespace Elastos
