
#include "cmdef.h"
#include "HttpEngine.h"
#include "IoUtils.h"
#include "HttpConnectionPool.h"
#include "CStreams.h"
#include "CFixedLengthOutputStream.h"
#include "CChunkedOutputStream.h"
#include "CGZIPInputStream.h"
#include "CFixedLengthInputStream.h"
#include "CChunkedInputStream.h"
#include "CookieHandler.h"
#include "CBufferedOutputStream.h"
#include "CDate.h"
#include "CSystem.h"
#include "elastos/StringUtils.h"

using Elastos::Core::ISystem;
using Elastos::Core::StringUtils;
using Elastos::IO::IoUtils;
using Elastos::IO::IStreams;
using Elastos::IO::CStreams;
using Elastos::IO::IBufferedOutputStream;
using Elastos::IO::CBufferedOutputStream;
using Elastos::Utility::IMap;
using Elastos::Utility::IDate;
using Elastos::Utility::CDate;
using Elastos::Utility::Zip::CGZIPInputStream;
using Elastos::Net::CookieHandler;

namespace Elastos {
namespace Net {
namespace Http {

const Int32 HttpEngine::DEFAULT_CHUNK_LENGTH = 1024;

const String HttpEngine::OPTIONS = String("OPTIONS");
const String HttpEngine::GET = String("GET");
const String HttpEngine::HEAD = String("HEAD");
const String HttpEngine::POST = String("POST");
const String HttpEngine::PUT = String("PUT");
const String HttpEngine::DELETE = String("DELETE");
const String HttpEngine::TRACE = String("TRACE");
const String HttpEngine::CONNECT = String("CONNECT");

const Int32 HttpEngine::HTTP_CONTINUE = 100;

const Int32 HttpEngine::MAX_REDIRECTS = 5;

const AutoPtr<ICacheResponse> HttpEngine::GATEWAY_TIMEOUT_RESPONSE; // = new CacheResponse() {
    //     @Override public Map<String, List<String>> getHeaders() throws IOException {
    //         Map<String, List<String>> result = new HashMap<String, List<String>>();
    //         result.put(null, Collections.singletonList("HTTP/1.1 504 Gateway Timeout"));
    //         return result;
    //     }
    //     @Override public InputStream getBody() throws IOException {
    //         return new ByteArrayInputStream(EmptyArray.BYTE);
    //     }
    // };

const Int32 HttpEngine::MAX_REQUEST_BUFFER_LENGTH = 32768;

ECode HttpEngine::Init(
    /* [in] */ IHttpURLConnectionImpl* policy,
    /* [in] */ const String& method,
    /* [in] */ IRawHeaders* requestHeaders,
    /* [in] */ IHttpConnection* connection,
    /* [in] */ IRetryableOutputStream* requestBodyOut)
{
    mSentRequestMillis = -1;
    mHttpMinorVersion = 1;
    mTransparentGzip = FALSE;
    mSendChunked = FALSE;
    mAutomaticallyReleaseConnectionToPool = FALSE;
    mConnectionReleased = FALSE;

    mPolicy = policy;
    mMethod = method;
    mConnection = connection;
    mRequestBodyOut = requestBodyOut;

    // try {
    AutoPtr<IURL> outuri;
    FAIL_RETURN(mPolicy->GetURL((IURL**)&outuri));
    FAIL_RETURN(outuri->ToURILenient((IURI**)&mUri));
    // } catch (URISyntaxException e) {
    //     throw new IOException(e);
    // }

    assert(0 && "TODO");
    // this.mRequestHeaders = new RequestHeaders(uri, new RawHeaders(mRequestHeaders));
    return NOERROR;
}

ECode HttpEngine::GetUri(
    /* [out] */ IURI** url)
{
    VALIDATE_NOT_NULL(url)

    *url = mUri;
    INTERFACE_ADDREF(*url)

    return NOERROR;
}

ECode HttpEngine::SendRequest()
{
    if (mResponseSource != NULL) {
        return NOERROR;
    }

    PrepareRawRequestHeaders();
    InitResponseSource();
    AutoPtr<IExtendedResponseCache> res = (IExtendedResponseCache*) mResponseCache->Probe(EIID_IExtendedResponseCache);
    if (res != NULL) {
        res->TrackResponse(mResponseSource);
    }

    /*
     * The raw response source may require the network, but the request
     * headers may forbid network use. In that case, dispose of the network
     * response and use a GATEWAY_TIMEOUT response instead, as specified
     * by http://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html#sec14.9.4.
     */
    Boolean isonlyflag = FALSE;
    if ((mRequestHeaders->IsOnlyIfCached(&isonlyflag), isonlyflag)
        && (mResponseSource == CONDITIONAL_CACHE || mResponseSource == NETWORK)) {
        if (mResponseSource == CONDITIONAL_CACHE) {
            IoUtils::CloseQuietly(mCachedResponseBody);
        }
        mResponseSource = CACHE;
        mCacheResponse = GATEWAY_TIMEOUT_RESPONSE;

        assert(0 && "TODO");
        // RawHeaders rawResponseHeaders = RawHeaders.fromMultimap(mCacheResponse.getHeaders());
        // setResponse(new ResponseHeaders(uri, rawResponseHeaders), mCacheResponse.getBody());
    }

    if (mResponseSource == CONDITIONAL_CACHE || mResponseSource == NETWORK) {
        SendSocketRequest();
    }
    else if (mConnection != NULL) {
        HttpConnectionPool::GetInstance()->Recycle(mConnection);
        mConnection = NULL;
    }

    return NOERROR;
}

ECode HttpEngine::GetRequestBody(
    /* [out] */ IOutputStream** os)
{
    VALIDATE_NOT_NULL(os)

    if (mResponseSource == NULL) {
        // throw new IllegalStateException();
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    *os = mRequestBodyOut;
    INTERFACE_ADDREF(*os)

    return NOERROR;
}

ECode HttpEngine::HasResponse(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)

    *value = mResponseHeaders != NULL;
    return NOERROR;
}

ECode HttpEngine::GetRequestHeaders(
    /* [out] */ IRequestHeaders** requesh)
{
    VALIDATE_NOT_NULL(requesh)

    *requesh = mRequestHeaders;
    INTERFACE_ADDREF(*requesh)

    return NOERROR;
}

ECode HttpEngine::GetResponseHeaders(
    /* [out] */ IResponseHeaders** rponh)
{
    VALIDATE_NOT_NULL(rponh)

    if (mResponseHeaders == NULL) {
        // throw new IllegalStateException();
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    *rponh = mResponseHeaders;
    INTERFACE_ADDREF(*rponh)

    return NOERROR;
}

ECode HttpEngine::GetResponseCode(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)

    if (mResponseHeaders == NULL) {
        // throw new IllegalStateException();
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    AutoPtr<IRawHeaders> rh;
    FAIL_RETURN(mResponseHeaders->GetHeaders((IRawHeaders**)&rh));
    return rh->GetResponseCode(value);
}

ECode HttpEngine::GetResponseBody(
    /* [out] */ IInputStream** is)
{
    VALIDATE_NOT_NULL(is)

    if (mResponseHeaders == NULL) {
        // throw new IllegalStateException();
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    *is = mResponseBodyIn;
    INTERFACE_ADDREF(*is)

    return NOERROR;
}

ECode HttpEngine::GetCacheResponse(
    /* [out] */ ICacheResponse** cr)
{
    VALIDATE_NOT_NULL(cr)

    *cr = mCacheResponse;
    INTERFACE_ADDREF(*cr)

    return NOERROR;
}

ECode HttpEngine::GetConnection(
    /* [out] */ IHttpConnection** hc)
{
    VALIDATE_NOT_NULL(hc)

    *hc = mConnection;
    INTERFACE_ADDREF(*hc)

    return NOERROR;
}

ECode HttpEngine::HasRecycledConnection(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)

    Boolean isflag = FALSE;
    *value = mConnection != NULL && (mConnection->IsRecycled(&isflag), isflag);
    return NOERROR;
}

ECode HttpEngine::AutomaticallyReleaseConnectionToPool()
{
    mAutomaticallyReleaseConnectionToPool = TRUE;
    if (mConnection != NULL && mConnectionReleased) {
        FAIL_RETURN(HttpConnectionPool::GetInstance()->Recycle(mConnection));
        mConnection = NULL;
    }

    return NOERROR;
}

ECode HttpEngine::ToRelease(
    /* [in] */ Boolean reusable)
{
    // If the response body comes from the cache, close it.
    if (mResponseBodyIn == mCachedResponseBody) {
        IoUtils::CloseQuietly(mResponseBodyIn);
    }

    if (!mConnectionReleased && mConnection != NULL) {
        mConnectionReleased = TRUE;

        // We cannot reuse sockets that have incomplete output.
        assert(0 && "TODO");
        // if (mRequestBodyOut != NULL && !mRequestBodyOut.closed) {
        //     reusable = false;
        // }

        // If the request specified that the mConnection shouldn't be reused,
        // don't reuse it. This advice doesn't apply to CONNECT requests because
        // the "Connection: close" header goes the origin server, not the proxy.
        Boolean reqflag = FALSE;
        if ((mRequestHeaders->HasConnectionClose(&reqflag), reqflag) && mMethod != CONNECT) {
            reusable = FALSE;
        }

        // If the response specified that the mConnection shouldn't be reused, don't reuse it.
        if (mResponseHeaders != NULL && (mResponseHeaders->HasConnectionClose(&reqflag), reqflag)) {
            reusable = FALSE;
        }

        if (IUnknownLengthHttpInputStream::Probe(mResponseBodyIn)) {
            reusable = FALSE;
        }

        if (reusable && mResponseBodyIn != NULL) {
            // We must discard the response body before the mConnection can be reused.
            AutoPtr<IStreams> streamout;
            FAIL_RETURN(CStreams::AcquireSingleton((IStreams**)&streamout));
            ECode ec = streamout->SkipAll(mResponseBodyIn);
            if (ec != NOERROR) {
                reusable = FALSE;
            }
        }

        if (!reusable) {
            mConnection->CloseSocketAndStreams();
            mConnection = NULL;
        }
        else if (mAutomaticallyReleaseConnectionToPool) {
            HttpConnectionPool::GetInstance()->Recycle(mConnection);
            mConnection = NULL;
        }
    }

    return NOERROR;
}

ECode HttpEngine::HasResponseBody(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)

    AutoPtr<IRawHeaders> rhead;
    mResponseHeaders->GetHeaders((IRawHeaders**)&rhead);
    Int32 responseCode = 0;
    rhead->GetResponseCode(&responseCode);

    // HEAD requests never yield a body regardless of the response headers.
    if (mMethod == HEAD) {
        *value = FALSE;
        return NOERROR;
    }

    if (mMethod != CONNECT
            && (responseCode < HTTP_CONTINUE || responseCode >= 200)
            && responseCode != 204 // HttpURLConnectionImpl.HTTP_NO_CONTENT
            && responseCode != 304 //HttpURLConnectionImpl.HTTP_NOT_MODIFIED
            ) {
        *value = TRUE;
        return NOERROR;
    }

    /*
     * If the Content-Length or Transfer-Encoding headers disagree with the
     * response code, the response is malformed. For best compatibility, we
     * honor the headers.
     */
    Int32 len = 0;
    Boolean isflag = FALSE;
    if ((mResponseHeaders->GetContentLength(&len), len) != -1
        || (mResponseHeaders->IsChunked(&isflag), isflag)) {
        *value = TRUE;
        return NOERROR;
    }

    *value = FALSE;
    return NOERROR;
}

ECode HttpEngine::ReadTrailers()
{
    AutoPtr<IRawHeaders> rhead;
    FAIL_RETURN(mResponseHeaders->GetHeaders((IRawHeaders**)&rhead));
    ReadHeaders(rhead);

    return NOERROR;
}

ECode HttpEngine::ReadResponse()
{
    Boolean isflag = FALSE;
    if ((HasResponse(&isflag), isflag)) {
        return NOERROR;
    }

    if (mResponseSource == NULL) {
        // throw new IllegalStateException("readResponse() without sendRequest()");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    if (!(mResponseSource == CONDITIONAL_CACHE || mResponseSource == NETWORK)) {
        return NOERROR;
    }

    if (mSentRequestMillis == -1) {
        AutoPtr<IRetryableOutputStream> res = (IRetryableOutputStream*) mRequestBodyOut->Probe(EIID_IRetryableOutputStream);
        Int32 conlen = 0;
        Int32 contentLength = res != NULL ? (res->ContentLength(&conlen), conlen) : -1;
        WriteRequestHeaders(contentLength);
    }

    if (mRequestBodyOut != NULL) {
        mRequestBodyOut->Close();
        AutoPtr<IRetryableOutputStream> resbody = (IRetryableOutputStream*) mRequestBodyOut->Probe(EIID_IRetryableOutputStream);
        if (resbody != NULL) {
            resbody->WriteToSocket(mRequestOut);
        }
    }

    assert(0 && "TODO");
    // mRequestOut->Flush();
    mRequestOut = mSocketOut;

    ReadResponseHeaders();

    AutoPtr<ISystem> system;
    Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);
    Int64 millis;
    system->GetCurrentTimeMillis(&millis);
    mResponseHeaders->SetLocalTimestamps(mSentRequestMillis, millis);

    if (mResponseSource == CONDITIONAL_CACHE) {
        Boolean isflag = FALSE;
        if (mCachedResponseHeaders->Validate(mResponseHeaders, &isflag), isflag) {
            ToRelease(TRUE);
            AutoPtr<IResponseHeaders> combinedHeaders;
            mCachedResponseHeaders->Combine(mResponseHeaders, (IResponseHeaders**)&combinedHeaders);
            SetResponse(combinedHeaders, mCachedResponseBody);
            AutoPtr<IExtendedResponseCache> httpResponseCache = (IExtendedResponseCache*) mResponseCache->Probe(EIID_IExtendedResponseCache);
            if (httpResponseCache != NULL) {
                httpResponseCache->TrackConditionalCacheHit();
                httpResponseCache->Update(mCacheResponse, GetHttpConnectionToCache());
            }
            return NOERROR;
        } else {
            IoUtils::CloseQuietly(mCachedResponseBody);
        }
    }

    if (HasResponseBody(&isflag), isflag) {
        MaybeCache(); // reentrant. this calls into user code which may call back into this!
    }

    InitContentStream(GetTransferStream());
    return NOERROR;
}

void HttpEngine::Connect()
{
    if (mConnection == NULL) {
        mConnection = OpenSocketConnection();
    }
}

AutoPtr<IHttpConnection> HttpEngine::OpenSocketConnection()
{
    assert(0 && "TODO");
    AutoPtr<IHttpConnection> result;
    // = HttpConnection.connect(uri, getSslSocketFactory(),
    //         mPolicy->GetProxy(), requiresTunnel(), mPolicy->GetConnectTimeout());
    // Proxy proxy = result.getAddress().getProxy();
    // if (proxy != null) {
    //     mPolicy->SetProxy(proxy);
    // }
    // result.setSoTimeout(mPolicy->GetReadTimeout());
    return result;
}

ECode HttpEngine::InitRequestBodyOut()
{
    Int32 chunkLength = 0;
    mPolicy->GetChunkLength(&chunkLength);
    Boolean isflag = FALSE;
    if (chunkLength > 0 || (mRequestHeaders->IsChunked(&isflag), isflag)) {
        mSendChunked = TRUE;
        if (chunkLength == -1) {
            chunkLength = DEFAULT_CHUNK_LENGTH;
        }
    }

    if (mSocketOut == NULL) {
        // throw new IllegalStateException("No socket to write to; was a POST cached?");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    if (mHttpMinorVersion == 0) {
        mSendChunked = FALSE;
    }

    Int32 fixedContentLength = 0;
    mPolicy->GetFixedContentLength(&fixedContentLength);
    assert(0 && "TODO");
    if (mRequestBodyOut != NULL) {
        // request body was already initialized by the predecessor HTTP engine
    } else if (fixedContentLength != -1) {
        WriteRequestHeaders(fixedContentLength);
        FAIL_RETURN(CFixedLengthOutputStream::New(mRequestOut, fixedContentLength, (IFixedLengthOutputStream**)&mRequestBodyOut));
    } else if (mSendChunked) {
        WriteRequestHeaders(-1);
        FAIL_RETURN(CChunkedOutputStream::New(mRequestOut, chunkLength, (IChunkedOutputStream**)&mRequestBodyOut));
    } else if ((mRequestHeaders->GetContentLength(&fixedContentLength), fixedContentLength) != -1) {
        WriteRequestHeaders(fixedContentLength);
        // FAIL_RETURN(CRetryableOutputStream::New(fixedContentLength, (IRetryableOutputStream**)&mRequestBodyOut));
    } else {
        // FAIL_RETURN(CRetryableOutputStream::New((IRetryableOutputStream**)&mRequestBodyOut));
    }

    return NOERROR;
}

Boolean HttpEngine::AcceptCacheResponseType(
    /* [in] */ ICacheResponse* mCacheResponse)
{
    return TRUE;
}

AutoPtr<IHttpURLConnection> HttpEngine::GetHttpConnectionToCache()
{
    return mPolicy;
}

AutoPtr<IRawHeaders> HttpEngine::GetNetworkRequestHeaders()
{
    AutoPtr<IRawHeaders> rhead;
    mResponseHeaders->GetHeaders((IRawHeaders**)&rhead);

    rhead->SetStatusLine(GetRequestLine());

    Int32 fixedContentLength = 0;
    mPolicy->GetFixedContentLength(&fixedContentLength);
    if (fixedContentLength != -1) {
        mRequestHeaders->SetContentLength(fixedContentLength);
    } else if (mSendChunked) {
        mRequestHeaders->SetChunked();
    } else if (IRetryableOutputStream::Probe(mRequestBodyOut)) {
        Int32 contentLength = 0;
        IRetryableOutputStream::Probe(mRequestBodyOut)->ContentLength(&contentLength);
        mRequestHeaders->SetContentLength(contentLength);
    }

    mRequestHeaders->GetHeaders((IRawHeaders**)&rhead);

    return rhead;
}

Boolean HttpEngine::IncludeAuthorityInRequestLine()
{
    Boolean isflag = FALSE;
    mPolicy->UsingProxy(&isflag);

    return isflag;
}

// SSLSocketFactory getSslSocketFactory()

String HttpEngine::GetDefaultUserAgent()
{
    AutoPtr<ISystem> system;
    Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);

    String agent;
    system->GetProperty(String("http.agent"), &agent);
    if (agent.IsNull()) {
        String version;
        system->GetProperty(String("java.version"), &version);
        agent = String("Java") + version;
    }
    return agent;
}

String HttpEngine::GetOriginAddress(
    /* [in] */ IURL* url)
{
    Int32 port = 0;
    url->GetPort(&port);
    String result;
    url->GetHost(&result);
    Int32 defport = 0;
    if (port > 0 && port != (mPolicy->GetDefaultPort(&defport), defport)) {
        result = result + String(":") + StringUtils::Int32ToString(port);
    }

    return result;
}

Boolean HttpEngine::RequiresTunnel()
{
    return FALSE;
}

ECode HttpEngine::InitResponseSource()
{
    mResponseSource = NETWORK;
    Boolean isflag =  FALSE;
    if (!(mPolicy->GetUseCaches(&isflag), isflag) || mResponseCache == NULL) {
        return NOERROR;
    }

    AutoPtr<ICacheResponse> candidate;
    AutoPtr<IRawHeaders> rhead;
    mRequestHeaders->GetHeaders((IRawHeaders**)&rhead);
    AutoPtr<IMap> osm;
    rhead->ToMultimap((IMap**)&osm);
    mResponseCache->Get(mUri, mMethod, osm, (ICacheResponse**)&candidate);
    if (candidate == NULL) {
        return NOERROR;
    }

    AutoPtr<IMap> responseHeadersMap;
    candidate->GetHeaders((IMap**)&responseHeadersMap);
    mCachedResponseBody = NULL;
    candidate->GetBody((IInputStream**)&mCachedResponseBody);
    if (!AcceptCacheResponseType(candidate)
            || responseHeadersMap == NULL
            || mCachedResponseBody == NULL) {
        IoUtils::CloseQuietly(mCachedResponseBody);
        return NOERROR;
    }

    assert(0 && "TODO");
    // AutoPtr<IRawHeaders> rawResponseHeaders = RawHeaders::FromMultimap(responseHeadersMap);
    // mCachedResponseHeaders = new ResponseHeaders(uri, rawResponseHeaders);
    AutoPtr<ISystem> system;
    Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);

    Int64 now;
    system->GetCurrentTimeMillis(&now);
    mResponseSource = NULL;
    mCachedResponseHeaders->ChooseResponseSource(now, mRequestHeaders, &mResponseSource);
    if (mResponseSource == CACHE) {
        mCacheResponse = candidate;
        SetResponse(mCachedResponseHeaders, mCachedResponseBody);
    } else if (mResponseSource == CONDITIONAL_CACHE) {
        mCacheResponse = candidate;
    } else if (mResponseSource == NETWORK) {
        IoUtils::CloseQuietly(mCachedResponseBody);
    } else {
        // throw new AssertionError();
        return E_ASSERTION_ERROR_EXCEPTION;
    }

    return NOERROR;
}

ECode HttpEngine::SendSocketRequest()
{
    if (mConnection == NULL) {
        Connect();
    }

    if (mSocketOut != NULL || mRequestOut != NULL || mSocketIn != NULL) {
        // throw new IllegalStateException();
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    mConnection->GetOutputStream((IOutputStream**)&mSocketOut);
    mRequestOut = mSocketOut;
    mConnection->GetInputStream((IInputStream**)&mSocketIn);

    if (HasRequestBody()) {
        InitRequestBodyOut();
    }
    return NOERROR;
}

ECode HttpEngine::SetResponse(
    /* [in] */ IResponseHeaders* headers,
    /* [in] */ IInputStream* body)
{
    if (mResponseBodyIn != NULL) {
        // throw new IllegalStateException();
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    mResponseHeaders = headers;
    AutoPtr<IRawHeaders> rhead;
    mResponseHeaders->GetHeaders((IRawHeaders**)&rhead);
    rhead->GetHttpMinorVersion(&mHttpMinorVersion);
    if (body != NULL) {
        InitContentStream(body);
    }

    return NOERROR;
}

Boolean HttpEngine::HasRequestBody()
{
    return mMethod == POST || mMethod == PUT;
}

void HttpEngine::MaybeCache()
{
    // Never cache responses to proxy CONNECT requests.
    if (mMethod == CONNECT) {
        return;
    }

    // Are we caching at all?
    Boolean isflag = FALSE;
    if (!(mPolicy->GetUseCaches(&isflag), isflag) || mResponseCache == NULL) {
        return;
    }

    // Should we cache this response for this request?
    if (!mResponseHeaders->IsCacheable(mRequestHeaders, &isflag), isflag) {
        return;
    }

    // Offer this request to the cache.
    mCacheRequest = NULL;
    mResponseCache->Put(mUri, GetHttpConnectionToCache(), (ICacheRequest**)&mCacheRequest);
}

void HttpEngine::InitContentStream(
    /* [in] */ IInputStream* transferStream)
{
    Boolean isflag = FALSE;
    if (mTransparentGzip && (mResponseHeaders->IsContentEncodingGzip(&isflag), isflag)) {
        /*
         * If the response was transparently gzipped, remove the gzip header field
         * so clients don't double decompress. http://b/3009828
         *
         * Also remove the Content-Length in this case because it contains the length
         * of the gzipped response. This isn't terribly useful and is dangerous because
         * clients can query the content length, but not the content encoding.
         */
        mResponseHeaders->StripContentEncoding();
        mResponseHeaders->StripContentLength();
        mResponseBodyIn = NULL;
        CGZIPInputStream::New(transferStream, (IInputStream**)&mResponseBodyIn);
    } else {
        mResponseBodyIn = transferStream;
    }
}

AutoPtr<IInputStream> HttpEngine::GetTransferStream()
{
    AutoPtr<IInputStream> instream;
    Boolean isflag = FALSE;
    if (!(HasResponseBody(&isflag), isflag)) {
        CFixedLengthInputStream::New(mSocketIn, mCacheRequest, this->Probe(EIID_IHttpEngine), 0, (IFixedLengthInputStream**)&instream);
        return instream;
    }

    if (mResponseHeaders->IsChunked(&isflag), isflag) {
        CChunkedInputStream::New(mSocketIn, mCacheRequest, this->Probe(EIID_IHttpEngine), (IChunkedInputStream**)&instream);
        return instream;
    }

    Int32 len = 0;
    if ((mResponseHeaders->GetContentLength(&len), len) != -1) {
        CFixedLengthInputStream::New(mSocketIn, mCacheRequest, this->Probe(EIID_IHttpEngine), len, (IFixedLengthInputStream**)&instream);
        return instream;
    }

    /*
     * Wrap the input stream from the HttpConnection (rather than
     * just returning "mSocketIn" directly here), so that we can control
     * its use after the reference escapes.
     */
    assert(0 && "TODO");
    // return new UnknownLengthHttpInputStream(mSocketIn, mCacheRequest, this);
    return NOERROR;
}

ECode HttpEngine::ReadResponseHeaders()
{
    AutoPtr<IRawHeaders> headers;
    assert(0 && "TODO");
    // do {
    //     headers = new RawHeaders();
    //     headers.setStatusLine(Streams.readAsciiLine(mSocketIn));
    //     readHeaders(headers);
    // } while (headers.getResponseCode() == HTTP_CONTINUE);
    // setResponse(new ResponseHeaders(uri, headers), null);
    return NOERROR;
}

void HttpEngine::ReadHeaders(
    /* [in] */ IRawHeaders* headers)
{
    // parse the result headers until the first blank line
    String line;
    AutoPtr<IStreams> istream;
    CStreams::AcquireSingleton((IStreams**)&istream);
    while (!(istream->ReadAsciiLine(mSocketIn, &line), line.IsEmpty())) {
        headers->AddLine(line);
    }

    AutoPtr<ICookieHandler> cookieHandler;
    CookieHandler::GetDefault((ICookieHandler**)&cookieHandler);
    if (cookieHandler != NULL) {
        AutoPtr<IMap> osm;
        headers->ToMultimap((IMap**)&osm);
        cookieHandler->Put(mUri, osm);
    }
}

ECode HttpEngine::WriteRequestHeaders(
    /* [in] */ Int32 contentLength)
{
    if (mSentRequestMillis != -1) {
        // throw new IllegalStateException();
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    AutoPtr<IRawHeaders> headersToSend = GetNetworkRequestHeaders();
    String headstr;
    headersToSend->ToHeaderString(&headstr);
    AutoPtr<ArrayOf<Char32> > bytes32 = headstr.GetChars(); // .getBytes(Charsets.ISO_8859_1);
    AutoPtr<ArrayOf<Byte> > bytes = ArrayOf<Byte>::Alloc(bytes32->GetLength());
    for (Int32 i = 0; i < bytes32->GetLength(); ++i) {
        (*bytes)[i] = (*bytes32)[i];
    }

    if (contentLength != -1 && bytes->GetLength() + contentLength <= MAX_REQUEST_BUFFER_LENGTH) {
        mRequestOut = NULL;
        CBufferedOutputStream::New(mSocketOut, bytes->GetLength() + contentLength, (IBufferedOutputStream**)&mRequestOut);
    }

    AutoPtr<ISystem> system;
    Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);
    mSentRequestMillis;
    system->GetCurrentTimeMillis(&mSentRequestMillis);
    return mRequestOut->Write(bytes);
}

void HttpEngine::PrepareRawRequestHeaders()
{
    AutoPtr<IRawHeaders> rhead;
    mRequestHeaders->GetHeaders((IRawHeaders**)&rhead);
    rhead->SetStatusLine(GetRequestLine());

    String outstr;
    if (mRequestHeaders->GetUserAgent(&outstr), outstr.IsNull()) {
        mRequestHeaders->SetUserAgent(GetDefaultUserAgent());
    }

    if (mRequestHeaders->GetHost(&outstr), outstr.IsNull()) {
        AutoPtr<IURL> iurl;
        mPolicy->GetURL((IURL**)&iurl);
        mRequestHeaders->SetHost(GetOriginAddress(iurl));
    }

    if (mHttpMinorVersion > 0 && (mRequestHeaders->GetConnection(&outstr), outstr.IsNull())) {
        mRequestHeaders->SetConnection(String("Keep-Alive"));
    }

    if (mRequestHeaders->GetAcceptEncoding(&outstr), outstr.IsNull()) {
        mTransparentGzip = TRUE;
        mRequestHeaders->SetAcceptEncoding(String("gzip"));
    }

    if (HasRequestBody() && (mRequestHeaders->GetContentType(&outstr), outstr.IsNull())) {
        mRequestHeaders->SetContentType(String("application/x-www-form-urlencoded"));
    }

    Int64 ifModifiedSince = 0;
    mPolicy->GetIfModifiedSince(&ifModifiedSince);
    if (ifModifiedSince != 0) {
        AutoPtr<IDate> adate;
        CDate::New(ifModifiedSince, (IDate**)&adate);
        mRequestHeaders->SetIfModifiedSince(adate);
    }

    AutoPtr<ICookieHandler> cookieHandler;
    CookieHandler::GetDefault((ICookieHandler**)&cookieHandler);
    if (cookieHandler != NULL) {
        AutoPtr<IRawHeaders> rhead;
        mRequestHeaders->GetHeaders((IRawHeaders**)&rhead);
        AutoPtr<IMap> osm;
        rhead->ToMultimap((IMap**)&osm);
        AutoPtr<IMap> outmap;
        cookieHandler->Get(mUri, osm, (IMap**)&outmap);
        mRequestHeaders->AddCookies(outmap);
    }
}

String HttpEngine::GetRequestLine()
{
    String protocol = (mHttpMinorVersion == 0) ? String("HTTP/1.0") : String("HTTP/1.1");
    return mMethod + String(" ") + RequestString() + String(" ") + protocol;
}

String HttpEngine::RequestString()
{
    AutoPtr<IURL> url;
    mPolicy->GetURL((IURL**)&url);
    String fileOnly;
    if (IncludeAuthorityInRequestLine()) {
        url->ToString(&fileOnly);
    } else {
        url->GetFile(&fileOnly);
        if (fileOnly.IsNull()) {
            fileOnly = String("/");
        } else if (!fileOnly.StartWith("/")) {
            fileOnly = String("/") + fileOnly;
        }
    }

    return fileOnly;
}

} // namespace Http
} // namespace Net
} // namespace Elastos
