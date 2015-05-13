
#include "CHeaders.h"

using Org::Apache::Http::Message::IBasicHeaderValueParserHelper;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

const String CHeaders::LOGTAG = String("Http");
const Int32 CHeaders::HASH_TRANSFER_ENCODING = 1274458357;
const Int32 CHeaders::HASH_CONTENT_LEN = -1132779846;
const Int32 CHeaders::HASH_CONTENT_TYPE = 785670158;
const Int32 CHeaders::HASH_CONTENT_ENCODING = 2095084583;
const Int32 CHeaders::HASH_CONN_DIRECTIVE = -775651618;
const Int32 CHeaders::HASH_LOCATION = 1901043637;
const Int32 CHeaders::HASH_PROXY_CONNECTION = 285929373;
const Int32 CHeaders::HASH_WWW_AUTHENTICATE = -243037365;
const Int32 CHeaders::HASH_PROXY_AUTHENTICATE = -301767724;
const Int32 CHeaders::HASH_CONTENT_DISPOSITION = -1267267485;
const Int32 CHeaders::HASH_ACCEPT_RANGES = 1397189435;
const Int32 CHeaders::HASH_EXPIRES = -1309235404;
const Int32 CHeaders::HASH_CACHE_CONTROL = -208775662;
const Int32 CHeaders::HASH_LAST_MODIFIED = 150043680;
const Int32 CHeaders::HASH_ETAG = 3123477;
const Int32 CHeaders::HASH_SET_COOKIE = 1237214767;
const Int32 CHeaders::HASH_PRAGMA = -980228804;
const Int32 CHeaders::HASH_REFRESH = 1085444827;
const Int32 CHeaders::HASH_X_PERMITTED_CROSS_DOMAIN_POLICIES = -1345594014;
const Int32 CHeaders::IDX_TRANSFER_ENCODING = 0;
const Int32 CHeaders::IDX_CONTENT_LEN = 1;
const Int32 CHeaders::IDX_CONTENT_TYPE = 2;
const Int32 CHeaders::IDX_CONTENT_ENCODING = 3;
const Int32 CHeaders::IDX_CONN_DIRECTIVE = 4;
const Int32 CHeaders::IDX_LOCATION = 5;
const Int32 CHeaders::IDX_PROXY_CONNECTION = 6;
const Int32 CHeaders::IDX_WWW_AUTHENTICATE = 7;
const Int32 CHeaders::IDX_PROXY_AUTHENTICATE = 8;
const Int32 CHeaders::IDX_CONTENT_DISPOSITION = 9;
const Int32 CHeaders::IDX_ACCEPT_RANGES = 10;
const Int32 CHeaders::IDX_EXPIRES = 11;
const Int32 CHeaders::IDX_CACHE_CONTROL = 12;
const Int32 CHeaders::IDX_LAST_MODIFIED = 13;
const Int32 CHeaders::IDX_ETAG = 14;
const Int32 CHeaders::IDX_SET_COOKIE = 15;
const Int32 CHeaders::IDX_PRAGMA = 16;
const Int32 CHeaders::IDX_REFRESH = 17;
const Int32 CHeaders::IDX_X_PERMITTED_CROSS_DOMAIN_POLICIES = 18;
const Int32 CHeaders::HEADER_COUNT = 19;

const AutoPtr<ArrayOf<String> > CHeaders::mHeaderNames = InitHeaderNames();
AutoPtr<ArrayOf<String> > CHeaders::InitHeaderNames()
 {
    AutoPtr<ArrayOf<String> > sarray = ArrayOf<String>::Alloc(19);

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

    return sarray;
}

CHeaders::CHeaders()
    : mTransferEncoding(0)
    , mContentLength(0)
    , mConnectionType(0)
{
    mHeaders = ArrayOf<String>::Alloc(HEADER_COUNT);

    mExtraHeaderNames.Resize(4);
    mExtraHeaderValues.Resize(4);
}

CHeaders::~CHeaders()
{}

ECode CHeaders::constructor()
{
    mTransferEncoding = NO_TRANSFER_ENCODING;
    mContentLength = NO_CONTENT_LENGTH;
    mConnectionType = NO_CONN_TYPE;
    return NOERROR;
}

ECode CHeaders::ParseHeader(
    /* [in] */ Org::Apache::Http::Util::ICharArrayBuffer* buffer)
{
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
}

ECode CHeaders::GetTransferEncoding(
    /* [out] */ Int64* encoding)
{
    VALIDATE_NOT_NULL(encoding);
    *encoding = mTransferEncoding;
    return NOERROR;
}

ECode CHeaders::GetContentLength(
    /* [out] */ Int64* length)
{
    VALIDATE_NOT_NULL(length);
    *length = mContentLength;
    return NOERROR;
}

ECode CHeaders::GetConnectionType(
    /* [out] */ Int32* type)
{
    VALIDATE_NOT_NULL(type);
    *type = mConnectionType;
    return NOERROR;
}

ECode CHeaders::GetContentType(
    /* [out] */ String* type)
{
    VALIDATE_NOT_NULL(type);
    *type = (*mHeaders)[IDX_CONTENT_TYPE];
    return NOERROR;
}

ECode CHeaders::GetContentEncoding(
    /* [out] */ String* encoding)
{
    VALIDATE_NOT_NULL(encoding);
    *encoding = (*mHeaders)[IDX_CONTENT_ENCODING];
    return NOERROR;
}

ECode CHeaders::GetLocation(
    /* [out] */ String* location)
{
    VALIDATE_NOT_NULL(location);
    *location = (*mHeaders)[IDX_LOCATION];
    return NOERROR;
}

ECode CHeaders::GetWwwAuthenticate(
    /* [out] */ String* authenticate)
{
    VALIDATE_NOT_NULL(authenticate);
    *authenticate = (*mHeaders)[IDX_WWW_AUTHENTICATE];
    return NOERROR;
}

ECode CHeaders::GetProxyAuthenticate(
    /* [out] */ String* authenticate)
{
    VALIDATE_NOT_NULL(authenticate);
    *authenticate = (*mHeaders)[IDX_PROXY_AUTHENTICATE];
    return NOERROR;
}

ECode CHeaders::GetContentDisposition(
    /* [out] */ String* disposition)
{
    VALIDATE_NOT_NULL(disposition);
    *disposition = (*mHeaders)[IDX_CONTENT_DISPOSITION];
    return NOERROR;
}

ECode CHeaders::GetAcceptRanges(
    /* [out] */ String* ranges)
{
    VALIDATE_NOT_NULL(ranges);
    *ranges = (*mHeaders)[IDX_ACCEPT_RANGES];
    return NOERROR;
}

ECode CHeaders::GetExpires(
    /* [out] */ String* expires)
{
    VALIDATE_NOT_NULL(expires);
    *expires = (*mHeaders)[IDX_EXPIRES];
    return NOERROR;
}

ECode CHeaders::GetCacheControl(
    /* [out] */ String* control)
{
    VALIDATE_NOT_NULL(control);
    *control = (*mHeaders)[IDX_CACHE_CONTROL];
    return NOERROR;
}

ECode CHeaders::GetLastModified(
    /* [out] */ String* modified)
{
    VALIDATE_NOT_NULL(modified);
    *modified = (*mHeaders)[IDX_LAST_MODIFIED];
    return NOERROR;
}

ECode CHeaders::GetEtag(
    /* [out] */ String* etag)
{
    VALIDATE_NOT_NULL(etag);
    *etag = (*mHeaders)[IDX_ETAG];
    return NOERROR;
}

ECode CHeaders::GetSetCookie(
    /* [out] */ IObjectContainer** cookie)
{
    VALIDATE_NOT_NULL(cookie);

    CObjectContainer::New(cookie);
    List<String>::Iterator it;
    AutoPtr<ICharSequence> cs;
    for (it = mCookies.Begin(); it != mCookies.End(); ++it) {
        String s = *it;
        cs = NULL;
        CStringWrapper::New(s, (ICharSequence**)&cs);
        (*cookie)->Add(cs);
    }
    return NOERROR;
}

ECode CHeaders::GetPragma(
    /* [out] */ String* sPragma)
{
    VALIDATE_NOT_NULL(sPragma);
    *sPragma = (*mHeaders)[IDX_PRAGMA];
    return NOERROR;
}

ECode CHeaders::GetRefresh(
    /* [out] */ String* refresh)
{
    VALIDATE_NOT_NULL(refresh);
    *refresh = (*mHeaders)[IDX_REFRESH];
    return NOERROR;
}

ECode CHeaders::GetXPermittedCrossDomainPolicies(
    /* [out] */ String* policies)
{
    VALIDATE_NOT_NULL(policies);
    *policies = (*mHeaders)[IDX_X_PERMITTED_CROSS_DOMAIN_POLICIES];
    return NOERROR;
}

ECode CHeaders::SetContentLength(
    /* [in] */ Int64 length)
{
    mContentLength = length;
    return NOERROR;
}

ECode CHeaders::SetContentType(
    /* [in] */ const String& type)
{
    (*mHeaders)[IDX_CONTENT_TYPE] = type;
    return NOERROR;
}

ECode CHeaders::SetContentEncoding(
    /* [in] */ const String& encoding)
{
    (*mHeaders)[IDX_CONTENT_ENCODING] = encoding;
    return NOERROR;
}

ECode CHeaders::SetLocation(
    /* [in] */ const String& location)
{
    (*mHeaders)[IDX_LOCATION] = location;
    return NOERROR;
}

ECode CHeaders::SetWwwAuthenticate(
    /* [in] */ const String& authenticate)
{
    (*mHeaders)[IDX_WWW_AUTHENTICATE] = authenticate;
    return NOERROR;
}

ECode CHeaders::SetProxyAuthenticate(
    /* [in] */ const String& authenticate)
{
    (*mHeaders)[IDX_PROXY_AUTHENTICATE] = authenticate;
    return NOERROR;
}

ECode CHeaders::SetContentDisposition(
    /* [in] */ const String& disposition)
{
    (*mHeaders)[IDX_CONTENT_DISPOSITION] = disposition;
    return NOERROR;
}

ECode CHeaders::SetAcceptRanges(
    /* [in] */ const String& ranges)
{
    (*mHeaders)[IDX_ACCEPT_RANGES] = ranges;
    return NOERROR;
}

ECode CHeaders::SetExpires(
    /* [in] */ const String& expires)
{
    (*mHeaders)[IDX_EXPIRES] = expires;
    return NOERROR;
}

ECode CHeaders::SetCacheControl(
    /* [in] */ const String& control)
{
    (*mHeaders)[IDX_CACHE_CONTROL] = control;
    return NOERROR;
}

ECode CHeaders::SetLastModified(
    /* [in] */ const String& modified)
{
    (*mHeaders)[IDX_LAST_MODIFIED] = modified;
    return NOERROR;
}

ECode CHeaders::SetEtag(
    /* [in] */ const String& etage)
{
    (*mHeaders)[IDX_ETAG] = etage;
    return NOERROR;
}

ECode CHeaders::SetXPermittedCrossDomainPolicies(
    /* [in] */ const String& policies)
{
    (*mHeaders)[IDX_X_PERMITTED_CROSS_DOMAIN_POLICIES] = policies;
    return NOERROR;
}

ECode CHeaders::GetHeaders(
    /* [out] */ IHeaderCallback** hcb)
{
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
}

ECode CHeaders::SetConnectionType(
    /* [in] */ Org::Apache::Http::Util::ICharArrayBuffer* buffer,
    /* [in] */ Int32 pos)
{
    // TODO:
    // AutoPtr<ICharArrayBuffersHelper> helper;
    // CCharArrayBuffersHelper::AcquireSingleton((ICharArrayBuffersHelper**)&helper);
    // if (helper->ContainsIgnoreCaseTrimmed(buffer, pos, HTTP::CONN_CLOSE)) {
    //     mConnectionType = CONN_CLOSE;
    // } else if (helper->ContainsIgnoreCaseTrimmed(buffer, pos, HTTP::CONN_KEEP_ALIVE)) {
    //     mConnectionType = CONN_KEEP_ALIVE;
    // }

    return NOERROR;
}

}
}
}
}

