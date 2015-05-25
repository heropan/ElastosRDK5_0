
#include "CRequestHeaders.h"
#include "StringUtils.h"
#include "CHttpDate.h"

using Elastos::Core::StringUtils;

namespace Elastos {
namespace Net {
namespace Http {

CAR_INTERFACE_IMPL(CRequestHeaders::CCacheControlHandler, ICacheControlHandler)

ECode CRequestHeaders::constructor(
    /* [in] */ IURI* uri,
    /* [in] */ IRawHeaders* headers)
{
    mUri = uri;
    mHeaders = headers;

    AutoPtr<ICacheControlHandler> handler = new CCacheControlHandler(this);
    Int32 headlen = 0;
    for (Int32 i = 0; i < (headers->Length(&headlen), headlen); i++) {
        String fieldName;
        headers->GetFieldName(i, &fieldName);
        String value;
        headers->GetValue(i, &value);
        if (String("Cache-Control").EqualsIgnoreCase(fieldName)) {
            AutoPtr<IHeaderParserHelper> hph;
            FAIL_RETURN(CHeaderParserHelper::AcquireSingleton((IHeaderParserHelper**)&hph));
            hph->ParseCacheControl(value, handler);
        }
        else if (String("Pragma").EqualsIgnoreCase(fieldName)) {
            if (value.EqualsIgnoreCase("no-cache")) {
                mNoCache = TRUE;
            }
        }
        else if (String("If-None-Match").EqualsIgnoreCase(fieldName)) {
            mIfNoneMatch = value;
        }
        else if (String("If-Modified-Since").EqualsIgnoreCase(fieldName)) {
            mIfModifiedSince = value;
        }
        else if (String("Authorization").EqualsIgnoreCase(fieldName)) {
            mHasAuthorization = TRUE;
        }
        else if (String("Content-Length").EqualsIgnoreCase(fieldName)) {
            // try {
            mContentLength = StringUtils::ParseInt32(value);
            // } catch (NumberFormatException ignored) {
            // }
        }
        else if (String("Transfer-Encoding").EqualsIgnoreCase(fieldName)) {
            mTransferEncoding = value;
        }
        else if (String("User-Agent").EqualsIgnoreCase(fieldName)) {
            mUserAgent = value;
        }
        else if (String("Host").EqualsIgnoreCase(fieldName)) {
            mHost = value;
        }
        else if (String("Connection").EqualsIgnoreCase(fieldName)) {
            mConnection = value;
        }
        else if (String("Accept-Encoding").EqualsIgnoreCase(fieldName)) {
            mAcceptEncoding = value;
        }
        else if (String("Content-Type").EqualsIgnoreCase(fieldName)) {
            mContentType = value;
        }
        else if (String("Proxy-Authorization").EqualsIgnoreCase(fieldName)) {
            mProxyAuthorization = value;
        }
    }
    return NOERROR;
}

ECode CRequestHeaders::IsChunked(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)

    *value = String("chunked").EqualsIgnoreCase(mTransferEncoding);
    return NOERROR;
}

ECode CRequestHeaders::HasConnectionClose(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)

    *value = String("close").EqualsIgnoreCase(mConnection);
    return NOERROR;
}

ECode CRequestHeaders::GetUri(
    /* [out] */ IURI** url)
{
    VALIDATE_NOT_NULL(url)

    *url = mUri;
    return NOERROR;
}

ECode CRequestHeaders::GetHeaders(
    /* [out] */ IRawHeaders** rh)
{
    VALIDATE_NOT_NULL(rh)

    *rh = mHeaders;
    INTERFACE_ADDREF(*rh)

    return NOERROR;
}

ECode CRequestHeaders::IsNoCache(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)

    *value = mNoCache;
    return NOERROR;
}

ECode CRequestHeaders::GetMaxAgeSeconds(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)

    *value = mMaxAgeSeconds;
    return NOERROR;
}

ECode CRequestHeaders::GetMaxStaleSeconds(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)

    *value = mMaxStaleSeconds;
    return NOERROR;
}

ECode CRequestHeaders::GetMinFreshSeconds(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)

    *value = mMinFreshSeconds;
    return NOERROR;
}

ECode CRequestHeaders::IsOnlyIfCached(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)

    *value = mOnlyIfCached;
    return NOERROR;
}

ECode CRequestHeaders::HasAuthorization(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)

    *value = mHasAuthorization;
    return NOERROR;
}

ECode CRequestHeaders::GetContentLength(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)

    *value = mContentLength;
    return NOERROR;
}

ECode CRequestHeaders::GetTransferEncoding(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)

    *str = mTransferEncoding;
    return NOERROR;
}

ECode CRequestHeaders::GetUserAgent(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)

    *str = mUserAgent;
    return NOERROR;
}

ECode CRequestHeaders::GetHost(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)

    *str = mHost;
    return NOERROR;
}

ECode CRequestHeaders::GetConnection(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)

    *str = mConnection;
    return NOERROR;
}

ECode CRequestHeaders::GetAcceptEncoding(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)

    *str = mAcceptEncoding;
    return NOERROR;
}

ECode CRequestHeaders::GetContentType(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)

    *str = mContentType;
    return NOERROR;
}

ECode CRequestHeaders::GetIfModifiedSince(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)

    *str = mIfModifiedSince;
    return NOERROR;
}

ECode CRequestHeaders::GetIfNoneMatch(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)

    *str = mIfNoneMatch;
    return NOERROR;
}

ECode CRequestHeaders::GetProxyAuthorization(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)

    *str = mProxyAuthorization;
    return NOERROR;
}

ECode CRequestHeaders::SetChunked()
{
    if (!mTransferEncoding.IsNull()) {
        mHeaders->RemoveAll(String("Transfer-Encoding"));
    }
    mHeaders->Add(String("Transfer-Encoding"), String("chunked"));
    mTransferEncoding = String("chunked");
    return NOERROR;
}

ECode CRequestHeaders::SetContentLength(
    /* [in] */ Int32 contentLength)
{
    if (mContentLength != -1) {
        mHeaders->RemoveAll(String("Content-Length"));
    }
    mHeaders->Add(String("Content-Length"), StringUtils::Int32ToString(contentLength));
    mContentLength = contentLength;
    return NOERROR;
}

ECode CRequestHeaders::SetUserAgent(
    /* [in] */ const String& userAgent)
{
    if (!mUserAgent.IsNull()) {
        mHeaders->RemoveAll(String("User-Agent"));
    }
    mHeaders->Add(String("User-Agent"), userAgent);
    mUserAgent = userAgent;
    return NOERROR;
}

ECode CRequestHeaders::SetHost(
    /* [in] */ const String& host)
{
    if (!mHost.IsNull()) {
        mHeaders->RemoveAll(String("Host"));
    }
    mHeaders->Add(String("Host"), host);
    mHost = host;
    return NOERROR;
}

ECode CRequestHeaders::SetConnection(
    /* [in] */ const String& connection)
{
    if (!mConnection.IsNull()) {
        mHeaders->RemoveAll(String("Connection"));
    }
    mHeaders->Add(String("Connection"), connection);
    mConnection = connection;
    return NOERROR;
}

ECode CRequestHeaders::SetAcceptEncoding(
    /* [in] */ const String& acceptEncoding)
{
    if (!mAcceptEncoding.IsNull()) {
        mHeaders->RemoveAll(String("Accept-Encoding"));
    }
    mHeaders->Add(String("Accept-Encoding"), acceptEncoding);
    mAcceptEncoding = acceptEncoding;
    return NOERROR;
}

ECode CRequestHeaders::SetContentType(
    /* [in] */ const String& contentType)
{
    if (!mContentType.IsNull()) {
        mHeaders->RemoveAll(String("Content-Type"));
    }
    mHeaders->Add(String("Content-Type"), contentType);
    mContentType = contentType;
    return NOERROR;
}

ECode CRequestHeaders::SetIfModifiedSince(
    /* [in] */ IDate* date)
{
    if (!mIfModifiedSince.IsNull()) {
        mHeaders->RemoveAll(String("If-Modified-Since"));
    }
    String formattedDate;
    CHttpDate::_Format(date, &formattedDate);
    mHeaders->Add(String("If-Modified-Since"), formattedDate);
    mIfModifiedSince = formattedDate;
    return NOERROR;
}

ECode CRequestHeaders::SetIfNoneMatch(
    /* [in] */ const String& ifNoneMatch)
{
    if (!mIfNoneMatch.IsNull()) {
        mHeaders->RemoveAll(String("If-None-Match"));
    }
    mHeaders->Add(String("If-None-Match"), ifNoneMatch);
    mIfNoneMatch = ifNoneMatch;
    return NOERROR;
}

ECode CRequestHeaders::HasConditions(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)

    *value = !mIfModifiedSince.IsNull() || !mIfNoneMatch.IsNull();
    return NOERROR;
}

ECode CRequestHeaders::AddCookies(
    /* [in] */ IMap* allCookieHeaders)
{
    VALIDATE_NOT_NULL(allCookieHeaders)

    assert(0 && "TODO");
    // for (Map.Entry<String, List<String>> entry : allCookieHeaders.entrySet()) {
    //     String key = entry.getKey();
    //     if ("Cookie".equalsIgnoreCase(key) || "Cookie2".equalsIgnoreCase(key)) {
    //         headers.addAll(key, entry.getValue());
    //     }
    // }
    return NOERROR;
}

} // namespace Http
} // namespace Net
} // namespace Elastos
