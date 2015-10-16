
#include "CRequestHandle.h"
#include "HttpConnection.h"
#include "HttpsConnection.h"
#include "elastos/droid/R.h"
#include "HttpLog.h"
#include <elastos/core/StringBuilder.h>
#include <elastos/core/Math.h>

using Elastos::Webkit::ICookieManager;
using Elastos::Webkit::ICookieManagerHelper;
using Elastos::Core::ICharSequence;
using Elastos::Security::IMessageDigest;
using Elastos::Security::IMessageDigestHelper;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

const String CRequestHandle::AUTHORIZATION_HEADER = String("Authorization");
const String CRequestHandle::PROXY_AUTHORIZATION_HEADER = String("Proxy-Authorization");

CRequestHandle::CRequestHandle()
    : mRequest(NULL)
    , mBodyLength(0)
    , mRedirectCount(0)
    , mConnection(NULL)
{}

CRequestHandle::~CRequestHandle()
{}

ECode CRequestHandle::constructor(
    /* [in] */ Elastos::Droid::Net::Http::IRequestQueue* requestQueue,
    /* [in] */ const String& url,
    /* [in] */ Elastos::Droid::Net::IWebAddress* uri,
    /* [in] */ const String& method,
    /* [in] */ IObjectStringMap* headers,
    /* [in] */ Elastos::IO::IInputStream* bodyProvider,
    /* [in] */ Int32 bodyLength,
    /* [in] */ IRequest* request)
{
    // if (headers == null) {
    //     headers = new HashMap<String, String>();
    // }
    mBodyProvider = bodyProvider;
    mBodyLength = bodyLength;
    mMethod = method.IsNullOrEmpty() ? String("GET") : method;

    mUrl = url;
    mUri = uri;

    mRequestQueue = requestQueue;
    mRequest = (Request*)request;

    return NOERROR;
}

ECode CRequestHandle::constructor(
    /* [in] */ Elastos::Droid::Net::Http::IRequestQueue* requestQueue,
    /* [in] */ const String& url,
    /* [in] */ Elastos::Droid::Net::IWebAddress* uri,
    /* [in] */ const String& method,
    /* [in] */ IObjectStringMap* headers,
    /* [in] */ Elastos::IO::IInputStream* bodyProvider,
    /* [in] */ Int32 bodyLength,
    /* [in] */ IRequest* request,
    /* [in] */ IConnection* conn)
{
    // if (headers == null) {
    //     headers = new HashMap<String, String>();
    // }
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
}

ECode CRequestHandle::Cancel()
{
    if (mRequest != NULL) {
        // mRequest->Cancel();
    }
    return NOERROR;
}

ECode CRequestHandle::PauseRequest(
    /* [in] */ Boolean pause)
{
    if (mRequest != NULL) {
        // mRequest->SetLoadingPaused(pause);
    }
    return NOERROR;
}

ECode CRequestHandle::HandleSslErrorResponse(
    /* [in] */ Boolean proceed)
{
    if (mRequest != NULL) {
        // mRequest->HandleSslErrorResponse(proceed);
    }
    return NOERROR;
}

ECode CRequestHandle::IsRedirectMax(
    /* [out] */ Boolean* isRedirectMax)
{
    VALIDATE_NOT_NULL(isRedirectMax);
    *isRedirectMax = mRedirectCount >= MAX_REDIRECT_COUNT;
    return NOERROR;
}

ECode CRequestHandle::GetRedirectCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    *count = mRedirectCount;
    return NOERROR;
}

ECode CRequestHandle::SetRedirectCount(
    /* [in] */ Int32 count)
{
    mRedirectCount = count;
    return NOERROR;
}

ECode CRequestHandle::SetupRedirect(
    /* [in] */ const String& redirectTo,
    /* [in] */ Int32 statusCode,
    /* [in] */ IObjectStringMap* cacheHeaders,
    /* [out] */ Boolean* result)
{
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
    AutoPtr<ICookieManagerHelper> helper;
    // TODO:
    // CCookieManagerHelper::AcquireSingleton((ICookieManagerHelper**)&helper);
    AutoPtr<ICookieManager> cookieManager;
    helper->GetInstance((ICookieManager**)&cookieManager);
    String cookie;
    // TODO:
    // cookieManager->GetCookie(mUri, &cookie);
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
            // TODO:
            // cacheHeaders->Get((*keys)[i], (IInterface**)(ICharSequence**)&value);
            String svalue;
            value->ToString(&svalue);

            (*mHeaders)[(*keys)[i]] = cookie;
        }
    }

    // TODO:
    // CreateAndQueueNewRequest();
    *result = TRUE;
    return NOERROR;
}

ECode CRequestHandle::SetupBasicAuthResponse(
    /* [in] */ Boolean isProxy,
    /* [in] */ const String& username,
    /* [in] */ const String& password)
{
    AutoPtr<IRequestHandleHelper> helper;
    // CRequestHandleHelper::AcquireSingleton((IRequestHandleHelper**)&helper);
    String response;
    helper->ComputeBasicAuthResponse(username, password, &response);
    if (HttpLog::LOGV) {
        HttpLog::V(String("setupBasicAuthResponse(): response: ") + response);
    }
    String key;
    // TODO:
    // key = AuthorizationHeader(isProxy);
    (*mHeaders)[key] = String("Basic ") + response;
    // SetupAuthResponse();
    return NOERROR;
}

ECode CRequestHandle::SetupDigestAuthResponse(
    /* [in] */ Boolean isProxy,
    /* [in] */ const String& username,
    /* [in] */ const String& password,
    /* [in] */ const String& realm,
    /* [in] */ const String& nonce,
    /* [in] */ const String& QOP,
    /* [in] */ const String& algorithm,
    /* [in] */ const String& opaque)
{
    String response;
    ComputeDigestAuthResponse(username, password, realm, nonce, QOP,
        algorithm, opaque, &response);
    if (HttpLog::LOGV) {
        HttpLog::V(String("setupDigestAuthResponse(): response: ") + response);
    }
    String key;
    // TODO:
    // key = AuthorizationHeader(isProxy);
    (*mHeaders)[key] = String("Digest ") + response;
    // SetupAuthResponse();
    return NOERROR;
}

ECode CRequestHandle::GetMethod(
    /* [out] */ String* method)
{
    VALIDATE_NOT_NULL(method);
    *method = mMethod;
    return NOERROR;
}

ECode CRequestHandle::WaitUntilComplete()
{
    // return mRequest->WaitUntilComplete();
    return E_NOT_IMPLEMENTED;
}

ECode CRequestHandle::ProcessRequest()
{
    if (mConnection != NULL) {
        mConnection->ProcessRequests(mRequest);
    }
    return NOERROR;
}

ECode CRequestHandle::ComputeDigestAuthResponse(
    /* [in] */ const String& username,
    /* [in] */ const String& password,
    /* [in] */ const String& realm,
    /* [in] */ const String& nonce,
    /* [in] */ const String& QOP,
    /* [in] */ const String& algorithm,
    /* [in] */ const String& opaque,
    /* [out] */ String* str)
{
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
}

ECode CRequestHandle::ComputeDigest(
    /* [in] */ const String& A1,
    /* [in] */ const String& A2,
    /* [in] */ const String& nonce,
    /* [in] */ const String& QOP,
    /* [in] */ const String& nc,
    /* [in] */ const String& cnonce,
    /* [out] */ String* str)
{
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
}

ECode CRequestHandle::KD(
    /* [in] */ const String& secret,
    /* [in] */ const String& data,
    /* [out] */ String* str)
{
    return H(secret + String(":") + data, str);
}

ECode CRequestHandle::H(
    /* [in] */ const String& param,
    /* [out] */ String* str)
{
    if (param != NULL) {
        AutoPtr<IMessageDigestHelper> helper;
        // TODO:
        // CMessageDigest::AcquireSingleton((IMessageDigestHelper**)&helper);
        AutoPtr<IMessageDigest> md5;
        FAIL_RETURN(helper->GetInstance(String("MD5"), (IMessageDigest**)&md5));

        AutoPtr<ArrayOf<Byte> > d;

        // FAIL_RETURN(md5->Digest(param.getBytes(), &d));
        if (d != NULL) {
            return BufferToHex(d, str);
        }
    }

    return NULL;
}

ECode CRequestHandle::BufferToHex(
    /* [in] */ ArrayOf<Byte>* buffer,
    /* [out] */ String* str)
{
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
}

ECode CRequestHandle::ComputeCnonce(
    /* [out] */ String* str)
{
    // Random* rand = new Random();
    Int32 nextInt;
    // nextInt = rand->NextInt();
    nextInt = (nextInt == 0x80000000/*IInteger::MIN_VALUE*/) ?
            0x7FFFFFFF/*IInteger::MAX_VALUE*/ : Elastos::Core::Math::Abs(nextInt);
    *str = StringUtils::ToString(nextInt, 16);
    return NOERROR;
}

String CRequestHandle::DoubleQuote(
    /* [in] */ const String& param)
{
    if (param != NULL) {
        return String("\"") + param + String("\"");
    }

    return String(NULL);
}

ECode CRequestHandle::CreateAndQueueNewRequest()
{
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
}

}
}
}
}

