
#include "elastos/droid/net/http/Headers.h"

using Elastos::Droid::Utility::ILog;

using Org::Apache::Http::Entity::IContentLengthStrategy;
using Org::Apache::Http::IHeaderElement;
using Org::Apache::Http::Message::IBasicHeaderValueParser;
using Org::Apache::Http::Message::IParserCursor;
using Org::Apache::Http::Protocol::IHTTP;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

CAR_INTERFACE_IMPL(Headers, Object, IHeaders)

const String Headers::LOGTAG("Http");

const Int32 Headers::HASH_TRANSFER_ENCODING                     = 1274458357;
const Int32 Headers::HASH_CONTENT_LEN                           = -1132779846;
const Int32 Headers::HASH_CONTENT_TYPE                          = 785670158;
const Int32 Headers::HASH_CONTENT_ENCODING                      = 2095084583;
const Int32 Headers::HASH_CONN_DIRECTIVE                        = -775651618;
const Int32 Headers::HASH_LOCATION                              = 1901043637;
const Int32 Headers::HASH_PROXY_CONNECTION                      = 285929373;
const Int32 Headers::HASH_WWW_AUTHENTICATE                      = -243037365;
const Int32 Headers::HASH_PROXY_AUTHENTICATE                    = -301767724;
const Int32 Headers::HASH_CONTENT_DISPOSITION                   = -1267267485;
const Int32 Headers::HASH_ACCEPT_RANGES                         = 1397189435;
const Int32 Headers::HASH_EXPIRES                               = -1309235404;
const Int32 Headers::HASH_CACHE_CONTROL                         = -208775662;
const Int32 Headers::HASH_LAST_MODIFIED                         = 150043680;
const Int32 Headers::HASH_ETAG                                  = 3123477;
const Int32 Headers::HASH_SET_COOKIE                            = 1237214767;
const Int32 Headers::HASH_PRAGMA                                = -980228804;
const Int32 Headers::HASH_REFRESH                               = 1085444827;
const Int32 Headers::HASH_X_PERMITTED_CROSS_DOMAIN_POLICIES     = -1345594014;

const Int32 Headers::IDX_TRANSFER_ENCODING                  = 0;
const Int32 Headers::IDX_CONTENT_LEN                        = 1;
const Int32 Headers::IDX_CONTENT_TYPE                       = 2;
const Int32 Headers::IDX_CONTENT_ENCODING                   = 3;
const Int32 Headers::IDX_CONN_DIRECTIVE                     = 4;
const Int32 Headers::IDX_LOCATION                           = 5;
const Int32 Headers::IDX_PROXY_CONNECTION                   = 6;
const Int32 Headers::IDX_WWW_AUTHENTICATE                   = 7;
const Int32 Headers::IDX_PROXY_AUTHENTICATE                 = 8;
const Int32 Headers::IDX_CONTENT_DISPOSITION                = 9;
const Int32 Headers::IDX_ACCEPT_RANGES                      = 10;
const Int32 Headers::IDX_EXPIRES                            = 11;
const Int32 Headers::IDX_CACHE_CONTROL                      = 12;
const Int32 Headers::IDX_LAST_MODIFIED                      = 13;
const Int32 Headers::IDX_ETAG                               = 14;
const Int32 Headers::IDX_SET_COOKIE                         = 15;
const Int32 Headers::IDX_PRAGMA                             = 16;
const Int32 Headers::IDX_REFRESH                            = 17;
const Int32 Headers::IDX_X_PERMITTED_CROSS_DOMAIN_POLICIES  = 18;
const Int32 Headers::HEADER_COUNT                           = 19;

const AutoPtr<ArrayOf<String> > Headers::HEADER_NAMES = InitHeaderNames();

Headers::Headers()
    : mTransferEncoding(0)
    , mContentLength(0)
    , mConnectionType(0)
{
    mHeaders = ArrayOf<String>::Alloc(HEADER_COUNT);
#if 0 // TODO: Translated before. Need check.
    ArrayList<String> cookies = new ArrayList<String>(2);
    ArrayList<String> mExtraHeaderNames = new ArrayList<String>(4);
    ArrayList<String> mExtraHeaderValues = new ArrayList<String>(4);
#endif
}

AutoPtr<ArrayOf<String> > Headers::InitHeaderNames()
{
    AutoPtr<ArrayOf<String> > sarray = ArrayOf<String>::Alloc(19);

#if 0 // TODO: Translated before. Need check.
    (*sarray)[0]  = IHeaders::TRANSFER_ENCODING;
    (*sarray)[1]  = IHeaders::CONTENT_LEN;
    (*sarray)[2]  = IHeaders::CONTENT_TYPE;
    (*sarray)[3]  = IHeaders::CONTENT_ENCODING;
    (*sarray)[4]  = IHeaders::CONN_DIRECTIVE;
    (*sarray)[5]  = IHeaders::LOCATION;
    (*sarray)[6]  = IHeaders::PROXY_CONNECTION;
    (*sarray)[7]  = IHeaders::WWW_AUTHENTICATE;
    (*sarray)[8]  = IHeaders::PROXY_AUTHENTICATE;
    (*sarray)[9]  = IHeaders::CONTENT_DISPOSITION;
    (*sarray)[10] = IHeaders::ACCEPT_RANGES;
    (*sarray)[11] = IHeaders::EXPIRES;
    (*sarray)[12] = IHeaders::CACHE_CONTROL;
    (*sarray)[13] = IHeaders::LAST_MODIFIED;
    (*sarray)[14] = IHeaders::ETAG;
    (*sarray)[15] = IHeaders::SET_COOKIE;
    (*sarray)[16] = IHeaders::PRAGMA;
    (*sarray)[17] = IHeaders::REFRESH;
    (*sarray)[18] = IHeaders::X_PERMITTED_CROSS_DOMAIN_POLICIES;
#endif

    return sarray;
}

ECode Headers::constructor()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    mTransferEncoding = NO_TRANSFER_ENCODING;
    mContentLength = NO_CONTENT_LENGTH;
    mConnectionType = NO_CONN_TYPE;
    return NOERROR;
#endif
}

ECode Headers::ParseHeader(
    /* [in] */ ICharArrayBuffer* buffer)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    Int32 pos;
    // TODO:
    // AutoPtr<ICharArrayBuffersHelper> helper;
    // CCharArrayBuffersHelper::AcquireSingleton((ICharArrayBuffersHelper**)&helper);
    // helper->SetLowercaseIndexOf(buffer, ':', &pos);
    if (pos == -1) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    String name;
    // buffer->SubstringTrimmed(0, pos, &name);
    if (name.GetLength() == 0) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    pos++;

    String val;
    // buffer->SubstringTrimmed(pos, buffer.GetLength(), &val);
    // if (HttpLog.LOGV) {
    //     HttpLog.v("hdr " + buffer.length() + " " + buffer);
    // }

    switch (name.GetHashCode()) {
    case HASH_TRANSFER_ENCODING: {
        if (name.Equals(TRANSFER_ENCODING)) {
            (*mHeaders)[IDX_TRANSFER_ENCODING] = val;
            // AutoPtr<ArrayOf<IHeaderElement> > encodings;
            // AutoPtr<IBasicHeaderValueParserHelper> bhpHelper;
            // CBasicHeaderValueParserHelper::AcquireSingleton((IBasicHeaderValueParserHelper**)&bhpHelper);
            // AutoPtr<IBasicHeaderValueParser> bhpDefault;
            // bhpHelper->GetDEFAULT((IBasicHeaderValueParser**)&bhpDefault);
            // AutoPtr<IParserCursor> cursor;
            // CParserCursor::New(pos, buffer->GetLength(), (IParserCursor**)&cursor);
            // bhpDefault->ParseElements(buffer, cursor);
            // // The chunked encoding must be the last one applied RFC2616,
            // // 14.41
            // Int32 len = encodings->GetLength();
            // if (HTTP::IDENTITY_CODING.EqualsIgnoreCase(val)) {
            //     mTransferEncoding = IContentLengthStrategy::IDENTITY;
            // } else if ((len > 0)
            //         && (HTTP::CHUNK_CODING.EqualsIgnoreCase(encodings[len - 1]->GetName()))) {
            //     mTransferEncoding = IContentLengthStrategy::CHUNKED;
            // } else {
            //     mTransferEncoding = IContentLengthStrategy::IDENTITY;
            // }
        }
        break;
    }
    case HASH_CONTENT_LEN: {
        if (name.Equals(CONTENT_LEN)) {
            (*mHeaders)[IDX_CONTENT_LEN] = val;
            mContentLength = StringUtils::ParseInt64(val);
        }
        break;
    }
    case HASH_CONTENT_TYPE: {
        if (name.Equals(CONTENT_TYPE)) {
            (*mHeaders)[IDX_CONTENT_TYPE] = val;
        }
        break;
    }
    case HASH_CONTENT_ENCODING: {
        if (name.Equals(CONTENT_ENCODING)) {
            (*mHeaders)[IDX_CONTENT_ENCODING] = val;
        }
        break;
    }
    case HASH_CONN_DIRECTIVE: {
        if (name.Equals(CONN_DIRECTIVE)) {
            (*mHeaders)[IDX_CONN_DIRECTIVE] = val;
            SetConnectionType(buffer, pos);
        }
        break;
    }
    case HASH_LOCATION: {
        if (name.Equals(LOCATION)) {
            (*mHeaders)[IDX_LOCATION] = val;
        }
        break;
    }
    case HASH_PROXY_CONNECTION: {
        if (name.Equals(PROXY_CONNECTION)) {
            (*mHeaders)[IDX_PROXY_CONNECTION] = val;
            SetConnectionType(buffer, pos);
        }
        break;
    }
    case HASH_WWW_AUTHENTICATE: {
        if (name.Equals(WWW_AUTHENTICATE)) {
            (*mHeaders)[IDX_WWW_AUTHENTICATE] = val;
        }
        break;
    }
    case HASH_PROXY_AUTHENTICATE: {
        if (name.Equals(PROXY_AUTHENTICATE)) {
            (*mHeaders)[IDX_PROXY_AUTHENTICATE] = val;
        }
        break;
    }
    case HASH_CONTENT_DISPOSITION: {
        if (name.Equals(CONTENT_DISPOSITION)) {
            (*mHeaders)[IDX_CONTENT_DISPOSITION] = val;
        }
        break;
    }
    case HASH_ACCEPT_RANGES: {
        if (name.Equals(ACCEPT_RANGES)) {
            (*mHeaders)[IDX_ACCEPT_RANGES] = val;
        }
        break;
    }
    case HASH_EXPIRES: {
        if (name.Equals(EXPIRES)) {
            (*mHeaders)[IDX_EXPIRES] = val;
        }
        break;
    }
    case HASH_CACHE_CONTROL: {
        if (name.Equals(CACHE_CONTROL)) {
            // In case where we receive more than one header, create a ',' separated list.
            // This should be ok, according to RFC 2616 chapter 4.2
            if (!((*mHeaders)[IDX_CACHE_CONTROL]).IsNullOrEmpty() &&
                ((*mHeaders)[IDX_CACHE_CONTROL]).GetLength() > 0) {
                String tmp = (*mHeaders)[IDX_CACHE_CONTROL];
                (*mHeaders)[IDX_CACHE_CONTROL] += (String(",") + val);
            } else {
                (*mHeaders)[IDX_CACHE_CONTROL] = val;
            }
        }
        break;
    }
    case HASH_LAST_MODIFIED: {
        if (name.Equals(LAST_MODIFIED)) {
            (*mHeaders)[IDX_LAST_MODIFIED] = val;
        }
        break;
    }
    case HASH_ETAG: {
        if (name.Equals(ETAG)) {
            (*mHeaders)[IDX_ETAG] = val;
        }
        break;
    }
    case HASH_SET_COOKIE: {
        if (name.Equals(SET_COOKIE)) {
            (*mHeaders)[IDX_SET_COOKIE] = val;
            mCookies.PushBack(val);
        }
        break;
    }
    case HASH_PRAGMA: {
        if (name.Equals(PRAGMA)) {
            (*mHeaders)[IDX_PRAGMA] = val;
        }
        break;
    }
    case HASH_REFRESH: {
        if (name.Equals(REFRESH)) {
            (*mHeaders)[IDX_REFRESH] = val;
        }
        break;
    }
    case HASH_X_PERMITTED_CROSS_DOMAIN_POLICIES: {
        if (name.Equals(X_PERMITTED_CROSS_DOMAIN_POLICIES)) {
            (*mHeaders)[IDX_X_PERMITTED_CROSS_DOMAIN_POLICIES] = val;
        }
        break;
    }
    default: {
        mExtraHeaderNames.PushBack(name);
        mExtraHeaderValues.PushBack(val);
    }
    }

    return NOERROR;
#endif
}

ECode Headers::GetTransferEncoding(
    /* [out] */ Int64* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(encoding);
    *encoding = mTransferEncoding;
    return NOERROR;
#endif
}

ECode Headers::GetContentLength(
    /* [out] */ Int64* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(length);
    *length = mContentLength;
    return NOERROR;
#endif
}

ECode Headers::GetConnectionType(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(type);
    *type = mConnectionType;
    return NOERROR;
#endif
}

ECode Headers::GetContentType(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(type);
    *type = (*mHeaders)[IDX_CONTENT_TYPE];
    return NOERROR;
#endif
}

ECode Headers::GetContentEncoding(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(encoding);
    *encoding = (*mHeaders)[IDX_CONTENT_ENCODING];
    return NOERROR;
#endif
}

ECode Headers::GetLocation(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(location);
    *location = (*mHeaders)[IDX_LOCATION];
    return NOERROR;
#endif
}

ECode Headers::GetWwwAuthenticate(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(authenticate);
    *authenticate = (*mHeaders)[IDX_WWW_AUTHENTICATE];
    return NOERROR;
#endif
}

ECode Headers::GetProxyAuthenticate(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(authenticate);
    *authenticate = (*mHeaders)[IDX_PROXY_AUTHENTICATE];
    return NOERROR;
#endif
}

ECode Headers::GetContentDisposition(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(disposition);
    *disposition = (*mHeaders)[IDX_CONTENT_DISPOSITION];
    return NOERROR;
#endif
}

ECode Headers::GetAcceptRanges(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(ranges);
    *ranges = (*mHeaders)[IDX_ACCEPT_RANGES];
    return NOERROR;
#endif
}

ECode Headers::GetExpires(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(expires);
    *expires = (*mHeaders)[IDX_EXPIRES];
    return NOERROR;
#endif
}

ECode Headers::GetCacheControl(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(control);
    *control = (*mHeaders)[IDX_CACHE_CONTROL];
    return NOERROR;
#endif
}

ECode Headers::GetLastModified(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(modified);
    *modified = (*mHeaders)[IDX_LAST_MODIFIED];
    return NOERROR;
#endif
}

ECode Headers::GetEtag(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(etag);
    *etag = (*mHeaders)[IDX_ETAG];
    return NOERROR;
#endif
}

ECode Headers::GetSetCookie(
    /* [out] */ IArrayList** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(cookie);

    CObjectContainer::New(cookie);
    List<String>::Iterator it;
    AutoPtr<ICharSequence> cs;
    for (it = mCookies.Begin(); it != mCookies.End(); ++it) {
        String s = *it;
        cs = NULL;
        CString::New(s, (ICharSequence**)&cs);
        (*cookie)->Add(cs);
    }
    return NOERROR;
#endif
}

ECode Headers::GetPragma(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(sPragma);
    *sPragma = (*mHeaders)[IDX_PRAGMA];
    return NOERROR;
#endif
}

ECode Headers::GetRefresh(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(refresh);
    *refresh = (*mHeaders)[IDX_REFRESH];
    return NOERROR;
#endif
}

ECode Headers::GetXPermittedCrossDomainPolicies(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(policies);
    *policies = (*mHeaders)[IDX_X_PERMITTED_CROSS_DOMAIN_POLICIES];
    return NOERROR;
#endif
}

ECode Headers::SetContentLength(
    /* [in] */ Int64 value)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    mContentLength = length;
    return NOERROR;
#endif
}

ECode Headers::SetContentType(
    /* [in] */ const String& value)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    (*mHeaders)[IDX_CONTENT_TYPE] = type;
    return NOERROR;
#endif
}

ECode Headers::SetContentEncoding(
    /* [in] */ const String& value)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    (*mHeaders)[IDX_CONTENT_ENCODING] = encoding;
    return NOERROR;
#endif
}

ECode Headers::SetLocation(
    /* [in] */ const String& value)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    (*mHeaders)[IDX_LOCATION] = location;
    return NOERROR;
#endif
}

ECode Headers::SetWwwAuthenticate(
    /* [in] */ const String& value)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    (*mHeaders)[IDX_WWW_AUTHENTICATE] = authenticate;
    return NOERROR;
#endif
}

ECode Headers::SetProxyAuthenticate(
    /* [in] */ const String& value)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    (*mHeaders)[IDX_PROXY_AUTHENTICATE] = authenticate;
    return NOERROR;
#endif
}

ECode Headers::SetContentDisposition(
    /* [in] */ const String& value)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    (*mHeaders)[IDX_CONTENT_DISPOSITION] = disposition;
    return NOERROR;
#endif
}

ECode Headers::SetAcceptRanges(
    /* [in] */ const String& value)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    (*mHeaders)[IDX_ACCEPT_RANGES] = ranges;
    return NOERROR;
#endif
}

ECode Headers::SetExpires(
    /* [in] */ const String& value)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    (*mHeaders)[IDX_EXPIRES] = expires;
    return NOERROR;
#endif
}

ECode Headers::SetCacheControl(
    /* [in] */ const String& value)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    (*mHeaders)[IDX_CACHE_CONTROL] = control;
    return NOERROR;
#endif
}

ECode Headers::SetLastModified(
    /* [in] */ const String& value)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    (*mHeaders)[IDX_LAST_MODIFIED] = modified;
    return NOERROR;
#endif
}

ECode Headers::SetEtag(
    /* [in] */ const String& value)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    (*mHeaders)[IDX_ETAG] = etage;
    return NOERROR;
#endif
}

ECode Headers::SetXPermittedCrossDomainPolicies(
    /* [in] */ const String& value)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    (*mHeaders)[IDX_X_PERMITTED_CROSS_DOMAIN_POLICIES] = policies;
    return NOERROR;
#endif
}

ECode Headers::GetHeaders(
    /* [in] */ IHeaderCallback* hcb)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    VALIDATE_NOT_NULL(hcb);
    *hcb = NULL;
    assert(0 && "TODO create hcb");

    for (Int32 i = 0; i < HEADER_COUNT; i++) {
        String h = mHeaders[i];
        if (!h.IsNullOrEmpty()) {
            (*hcb)->Header(mHeaderNames[i], h);
        }
    }

    List<String>::Iterator it1 = mExtraHeaderNames.Begin();
    List<String>::Iterator it2 = mExtraHeaderValues.Begin();
    for (; it1 != mExtraHeaderNames.End() && it2 != mExtraHeaderValues.End(); ++it1, ++it2) {}
        if (FALSE) {
            // HttpLog.v("Headers.getHeaders() extra: " + i + " " +
            //           mExtraHeaderNames.get(i) + " " + mExtraHeaderValues.get(i));
        }
        (*hcb)->Header(*it1, *it2);
    }
    return NOERROR;
#endif
}

ECode Headers::SetConnectionType(
    /* [in] */ ICharArrayBuffer* buffer,
    /* [in] */ Int32 pos)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translated before. Need check.
    // TODO:
    // AutoPtr<ICharArrayBuffersHelper> helper;
    // CCharArrayBuffersHelper::AcquireSingleton((ICharArrayBuffersHelper**)&helper);
    // if (helper->ContainsIgnoreCaseTrimmed(buffer, pos, HTTP::CONN_CLOSE)) {
    //     mConnectionType = CONN_CLOSE;
    // } else if (helper->ContainsIgnoreCaseTrimmed(buffer, pos, HTTP::CONN_KEEP_ALIVE)) {
    //     mConnectionType = CONN_KEEP_ALIVE;
    // }

    return NOERROR;
#endif
}

} // namespace Http
} // namespace Net
} // namespace Droid
} // namespace Elastos
