
#include "elastos/droid/net/http/RequestHandle.h"

using Elastos::Droid::Net::IWebAddress;
using Elastos::Droid::Webkit::ICookieManager;

using Elastos::Security::IMessageDigest;
using Elastos::Utility::IHashMap;
using Elastos::Utility::IRandom;

using Org::Apache::Commons::Codec::Binary::IBase64;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

CAR_INTERFACE_IMPL(RequestHandle, Object, IRequestHandle)

const String RequestHandle::AUTHORIZATION_HEADER("Authorization");
const String RequestHandle::PROXY_AUTHORIZATION_HEADER("Proxy-Authorization");

RequestHandle::RequestHandle()
    : mBodyLength(0)
    , mRedirectCount(0)
{}

ECode RequestHandle::constructor(
    /* [in] */ IRequestQueue* requestQueue,
    /* [in] */ const String& url,
    /* [in] */ IWebAddress* uri,
    /* [in] */ const String& method,
    /* [in] */ IMap* headers,
    /* [in] */ IInputStream* bodyProvider,
    /* [in] */ Int32 bodyLength,
    /* [in] */ IRequest* request)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    if (headers == NULL) {
        CHashMap::New((IMap**)&mHeaders);
    }
    mBodyProvider = bodyProvider;
    mBodyLength = bodyLength;
    mMethod = method.IsNullOrEmpty() ? String("GET") : method;

    mUrl = url;
    mUri = uri;

    mRequestQueue = requestQueue;
    mRequest = (Request*)request;

    return NOERROR;
#endif
}

ECode RequestHandle::constructor(
    /* [in] */ IRequestQueue* requestQueue,
    /* [in] */ const String& url,
    /* [in] */ IWebAddress* uri,
    /* [in] */ const String& method,
    /* [in] */ IMap* headers,
    /* [in] */ IInputStream* bodyProvider,
    /* [in] */ Int32 bodyLength,
    /* [in] */ IRequest* request,
    /* [in] */ IConnection* conn)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    if (headers == NULL) {
        CHashMap::New((IMap**)&mHeaders);
    }
    mBodyProvider = bodyProvider;
    mBodyLength = bodyLength;
    mMethod = method.IsNullOrEmpty() ? String("GET") : method;

    mUrl = url;
    mUri = uri;

    mRequestQueue = requestQueue;
    mRequest = (Request*)request;
    AutoPtr<IHttpsConnection> httpsConn = (IHttpsConnection*)conn->Probe(EIID_IHttpsConnection);
    if (httpsConn != NULL) {
         mConnection = (Connection*)(HttpsConnection*)conn;
    } else {
        mConnection = (Connection*)(HttpConnection*)conn;
    }

    return NOERROR;
#endif
}

ECode RequestHandle::Cancel()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    if (mRequest != NULL) {
        // mRequest->Cancel();
    }
    return NOERROR;
#endif
}

ECode RequestHandle::PauseRequest(
    /* [in] */ Boolean pause)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    if (mRequest != NULL) {
        // mRequest->SetLoadingPaused(pause);
    }
    return NOERROR;
#endif
}

ECode RequestHandle::HandleSslErrorResponse(
    /* [in] */ Boolean proceed)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    if (mRequest != NULL) {
        // mRequest->HandleSslErrorResponse(proceed);
    }
    return NOERROR;
#endif
}

ECode RequestHandle::IsRedirectMax(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    VALIDATE_NOT_NULL(isRedirectMax);
    *isRedirectMax = mRedirectCount >= MAX_REDIRECT_COUNT;
    return NOERROR;
#endif
}

ECode RequestHandle::GetRedirectCount(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    VALIDATE_NOT_NULL(count);
    *count = mRedirectCount;
    return NOERROR;
#endif
}

ECode RequestHandle::SetRedirectCount(
    /* [in] */ Int32 count)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    mRedirectCount = count;
    return NOERROR;
#endif
}

ECode RequestHandle::SetupRedirect(
    /* [in] */ const String& redirectTo,
    /* [in] */ Int32 statusCode,
    /* [in] */ IMap* cacheHeaders,
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    if (HttpLog::LOGV) {
        HttpLog::V(String("RequestHandle.setupRedirect(): redirectCount ") + StringUtils::Int32ToString(mRedirectCount));
    }

    // be careful and remove authentication headers, if any
    mHeaders.Erase(AUTHORIZATION_HEADER);
    mHeaders.Erase(PROXY_AUTHORIZATION_HEADER);

    if (++mRedirectCount == MAX_REDIRECT_COUNT) {
        // Way too many redirects -- fail out
        if (HttpLog::LOGV) {
            HttpLog::V(String("RequestHandle.setupRedirect(): too many redirects ")/* + mRequest*/);
        }
        // TODO:
        // mRequest->Error(IEventHandler::ERROR_REDIRECT_LOOP,
        //                R::string::httpErrorRedirectLoop);

        *result = FALSE;
        return NOERROR;
    }

    if (mUrl.StartWith("https:") && redirectTo.StartWith("http:")) {
        // implement http://www.w3.org/Protocols/rfc2616/rfc2616-sec15.html#sec15.1.3
        if (HttpLog::LOGV) {
            HttpLog::V(String("blowing away the referer on an https -> http redirect"));
        }
        mHeaders.Erase(String("Referer"));
    }

    mUrl = redirectTo;

    // TODO:
    // FAIL_RETURN(CWebAddress::New(mUrl, &mUri));

    // update the "Cookie" header based on the redirected url
    mHeaders.Erase(String("Cookie"));
    AutoPtr<ICookieManager> cookieManager;
    CCookieManagerHelper::GetInstance((ICookieManager**)&cookieManager);
    String cookie;
    cookieManager->GetCookie(mUri, &cookie);
    if (cookie.IsNullOrEmpty() && cookie.GetLength() > 0) {
        (*mHeaders)[String("Cookie")] = cookie;
    }

    if ((statusCode == 302 || statusCode == 303) && mMethod.Equals("POST")) {
        if (HttpLog::LOGV) {
            HttpLog::V(String("replacing POST with GET on redirect to ") + redirectTo);
        }
        mMethod = String("GET");
    }
    /* Only repost content on a 307.  If 307, reset the body
       provider so we can replay the body */
    if (statusCode == 307) {
        if (mBodyProvider != NULL){
            if(FAILED(mBodyProvider->Reset())) {
                if (HttpLog::LOGV) {
                    HttpLog::V(String("setupRedirect() failed to reset body provider"));
                }

                *result = FALSE;
                return E_IO_EXCEPTION;
            }
        }
    } else {
        mHeaders.Erase(String("Content-Type"));
        mBodyProvider = NULL;
    }

    // Update the cache headers for this URL
    Int32 size;
    cacheHeaders->GetSize(&size);
    if (size > 0) {
        AutoPtr<ArrayOf<String> > keys;
        AutoPtr<IObjectContainer> values;
        cacheHeaders->GetAllItems((ArrayOf<String>**)&keys, (IObjectContainer**)&values);

        for (Int32 i = 0; i < size; i++) {
            AutoPtr<ICharSequence> value;
            cacheHeaders->Get((*keys)[i], (IInterface**)(ICharSequence**)&value);
            String svalue;
            value->ToString(&svalue);

            (*mHeaders)[(*keys)[i]] = cookie;
        }
    }

    CreateAndQueueNewRequest();
    *result = TRUE;
    return NOERROR;
#endif
}

ECode RequestHandle::SetupBasicAuthResponse(
    /* [in] */ Boolean isProxy,
    /* [in] */ const String& username,
    /* [in] */ const String& password)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    String response;
    CRequestHandleHelper::ComputeBasicAuthResponse(username, password, &response);
    if (HttpLog::LOGV) {
        HttpLog::V(String("setupBasicAuthResponse(): response: ") + response);
    }
    String key;
    key = AuthorizationHeader(isProxy);
    (*mHeaders)[key] = String("Basic ") + response;
    SetupAuthResponse();
    return NOERROR;
#endif
}

ECode RequestHandle::SetupDigestAuthResponse(
    /* [in] */ Boolean isProxy,
    /* [in] */ const String& username,
    /* [in] */ const String& password,
    /* [in] */ const String& realm,
    /* [in] */ const String& nonce,
    /* [in] */ const String& QOP,
    /* [in] */ const String& algorithm,
    /* [in] */ const String& opaque)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    String response;
    ComputeDigestAuthResponse(username, password, realm, nonce, QOP,
        algorithm, opaque, &response);
    if (HttpLog::LOGV) {
        HttpLog::V(String("setupDigestAuthResponse(): response: ") + response);
    }
    String key;
    key = AuthorizationHeader(isProxy);
    (*mHeaders)[key] = String("Digest ") + response;
    SetupAuthResponse();
    return NOERROR;
#endif
}

ECode RequestHandle::SetupAuthResponse()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        // try {
            if (mBodyProvider != NULL) mBodyProvider->Reset();
        // } catch (java.io.IOException ex) {
            if (HttpLog::LOGV) {
                HttpLog::V("setupAuthResponse() failed to reset body provider");
            }
        // }
        CreateAndQueueNewRequest();
#endif
}

ECode RequestHandle::GetMethod(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    VALIDATE_NOT_NULL(method);
    *method = mMethod;
    return NOERROR;
#endif
}

ECode RequestHandle::ComputeBasicAuthResponse(
    /* [in] */ const String& username,
    /* [in] */ const String& password,
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    assert(username != NULL);
    assert(password != NULL);
    // encode username:password to base64
    *result = String(Base64.encodeBase64((username + ':' + password).getBytes()));
    return NOERROR;
#endif
}

ECode RequestHandle::WaitUntilComplete()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    mRequest->WaitUntilComplete();
    return NOERROR;
#endif
}

ECode RequestHandle::ProcessRequest()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    if (mConnection != NULL) {
        mConnection->ProcessRequests(mRequest);
    }
    return NOERROR;
#endif
}

ECode RequestHandle::ComputeDigestAuthResponse(
    /* [in] */ const String& username,
    /* [in] */ const String& password,
    /* [in] */ const String& realm,
    /* [in] */ const String& nonce,
    /* [in] */ const String& QOP,
    /* [in] */ const String& algorithm,
    /* [in] */ const String& opaque,
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    if (username.IsNullOrEmpty()
        || password.IsNullOrEmpty()
        || realm.IsNullOrEmpty())
    {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }


    String A1 = username + String(":") + realm + String(":") + password;
    String A2 = mMethod  + String(":") + mUrl;

    // because we do not preemptively send authorization headers, nc is always 1
    String nc = String("00000001");
    String cnonce;
    FAIL_RETURN(ComputeCnonce(&cnonce));
    String digest;
    FAIL_RETURN(ComputeDigest(A1, A2, nonce, QOP, nc, cnonce, &digest));

    String response = String("");
    response += String("username=") + DoubleQuote(username) + String(", ");
    response += String("realm=")    + DoubleQuote(realm)    + String(", ");
    response += String("nonce=")    + DoubleQuote(nonce)    + String(", ");
    response += String("uri=")      + DoubleQuote(mUrl)     + String(", ");
    response += String("response=") + DoubleQuote(digest) ;

    if (!opaque.IsNullOrEmpty()) {
        response += String(", opaque=") + DoubleQuote(opaque);
    }

     if (!algorithm.IsNullOrEmpty()) {
        response += String(", algorithm=") +  algorithm;
    }

    if (!QOP.IsNullOrEmpty()) {
        response += String(", qop=") + QOP + String(", nc=") + nc + String(", cnonce=") + DoubleQuote(cnonce);
    }

    *str = response;
    return NOERROR;
#endif
}

ECode RequestHandle::AuthorizationHeader(
    /* [in] */ Boolean isProxy,
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    if (!isProxy) {
        *result = AUTHORIZATION_HEADER;
        return NOERROR;
    } else {
        *result = PROXY_AUTHORIZATION_HEADER;
    }
    return NOERROR;
#endif
}

ECode RequestHandle::ComputeDigest(
    /* [in] */ const String& A1,
    /* [in] */ const String& A2,
    /* [in] */ const String& nonce,
    /* [in] */ const String& QOP,
    /* [in] */ const String& nc,
    /* [in] */ const String& cnonce,
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    if (HttpLog::LOGV) {
        HttpLog::V(String("computeDigest(): QOP: ") + QOP);
    }

    if (QOP == NULL) {
        String hA1;
        FAIL_RETURN(H(A1, &hA1));
        String hA2;
        FAIL_RETURN(H(A2, &hA2));
        return KD(hA1, nonce + String(":") + hA2, str);
    } else {
        if (QOP.EqualsIgnoreCase("auth")) {
            String hA1;
            FAIL_RETURN(H(A1, &hA1));
            String hA2;
            FAIL_RETURN(H(A2, &hA2));
            return KD(hA1, nonce + String(":") + nc + String(":") + cnonce + String(":") + QOP + String(":") + hA2, str);
        }
    }

    *str = String(NULL);
    return NOERROR;
#endif
}

String RequestHandle::KD(
    /* [in] */ const String& secret,
    /* [in] */ const String& data)
{
    return String(NULL);
#if 0 // TODO: Translate codes below
    return H(secret + String(":") + data, str);
#endif
}

String RequestHandle::H(
    /* [in] */ const String& param)
{
    return String(NULL);
#if 0 // TODO: Translate codes below
    if (param != NULL) {
        AutoPtr<IMessageDigestHelper> helper;
        CMessageDigest::AcquireSingleton((IMessageDigestHelper**)&helper);
        AutoPtr<IMessageDigest> md5;
        FAIL_RETURN(helper->GetInstance(String("MD5"), (IMessageDigest**)&md5));

        AutoPtr<ArrayOf<Byte> > d;

        FAIL_RETURN(md5->Digest(param.getBytes(), &d));
        if (d != NULL) {
            return BufferToHex(d, str);
        }
    }

    return NULL;
#endif
}

ECode RequestHandle::BufferToHex(
    /* [in] */ ArrayOf<Byte>* buffer,
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    const Char16 hexChars[] =
            { '0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f' };

    if (buffer != NULL) {
        Int32 length = buffer->GetLength();
        if (length > 0) {
            AutoPtr<StringBuilder> hex = new StringBuilder(2 * length);

            // TODO:
            // for (Int32 i = 0; i < length; ++i) {
                // Byte l = (Byte) (buffer[i] & 0x0F);
                // Byte h = (Byte)((buffer[i] & 0xF0) >> 4);

                // hex->Append(hexChars[h]);
                // hex->Append(hexChars[l]);
            // }

            *str = hex->ToString();
            return NOERROR;
        } else {
            *str = String("");
            return NOERROR;
        }
    }

    *str = String(NULL);
    return E_ILLEGAL_ARGUMENT_EXCEPTION;;
#endif
}

ECode RequestHandle::ComputeCnonce(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    // Random* rand = new Random();
    Int32 nextInt;
    // nextInt = rand->NextInt();
    nextInt = (nextInt == 0x80000000/*IInteger::MIN_VALUE*/) ?
            0x7FFFFFFF/*IInteger::MAX_VALUE*/ : Elastos::Core::Math::Abs(nextInt);
    *str = StringUtils::ToString(nextInt, 16);
    return NOERROR;
#endif
}

ECode RequestHandle::DoubleQuote(
    /* [in] */ const String& param,
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    if (param != NULL) {
        return String("\"") + param + String("\"");
    }

    return String(NULL);
#endif
}

ECode RequestHandle::CreateAndQueueNewRequest()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
    // // mConnection is non-null if and only if the requests are synchronous.
    AutoPtr<IObjectStringMap> headersMap;
    CObjectStringMap::New((IObjectStringMap**)&headersMap);
    if (mHeaders != NULL) {
        HashMap<String, String>::Iterator it = mHeaders.Begin();
        for (; it != mHeaders.End(); ++it) {
            String key = it->mFirst;
            AutoPtr<ICharSequence> value;
            CString::New(it->mSecond, (ICharSequence**)&value);
            headersMap->Put(key, value);
        }
    }
    if (mConnection != NULL) {
        AutoPtr<IRequestHandle> newHandle;
        mRequestQueue->QueueSynchronousRequest(mUrl, mUri, mMethod, headersMap,
            mRequest->mEventHandler, mBodyProvider, mBodyLength, (IRequestHandle**)&newHandle);
        mRequest = ((CRequestHandle*)(IRequestHandle*)newHandle)->mRequest;
        mConnection = ((CRequestHandle*)(IRequestHandle*)newHandle)->mConnection;
        newHandle->ProcessRequest();
        return NOERROR;
    }

    AutoPtr<IRequestHandle> reqHandle;
    mRequestQueue->QueueRequest(mUrl, mUri, mMethod, headersMap, mRequest->mEventHandler,
        mBodyProvider, mBodyLength, (IRequestHandle**)&reqHandle);
    mRequest = ((CRequestHandle*)(IRequestHandle*)reqHandle)->mRequest;

    return NOERROR;
#endif
}

} // namespace Http
} // namespace Net
} // namespace Droid
} // namespace Elastos
