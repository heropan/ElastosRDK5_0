
#ifndef __CREQUESTHEADERS_H__
#define __CREQUESTHEADERS_H__

#include "_CRequestHeaders.h"
#include "cmdef.h"
#include "CHeaderParserHelper.h"

using Elastos::Utility::IMap;
using Elastos::Utility::IDate;

namespace Elastos {
namespace Net {
namespace Http {

CarClass(CRequestHeaders)
{
private:
    class CCacheControlHandler
        : public ElRefBase
        , public ICacheControlHandler
    {
    public:
        CAR_INTERFACE_DECL()

        CCacheControlHandler(CRequestHeaders* rh)
        {
            mRh = rh;
        }

        ECode Handle(
            /* [in] */ const String& directive,
            /* [in] */ const String& parameter)
        {
            if (directive.EqualsIgnoreCase("no-cache")) {
                mRh->mNoCache = TRUE;
                return NOERROR;
            }
            else if (directive.EqualsIgnoreCase("max-age")) {
                AutoPtr<IHeaderParserHelper> hph;
                FAIL_RETURN(CHeaderParserHelper::AcquireSingleton((IHeaderParserHelper**)&hph));
                return hph->ParseSeconds(parameter, &mRh->mMaxAgeSeconds);
            }
            else if (directive.EqualsIgnoreCase("max-stale")) {
                AutoPtr<IHeaderParserHelper> hph;
                FAIL_RETURN(CHeaderParserHelper::AcquireSingleton((IHeaderParserHelper**)&hph));
                return hph->ParseSeconds(parameter, &mRh->mMaxStaleSeconds);
            }
            else if (directive.EqualsIgnoreCase("min-fresh")) {
                AutoPtr<IHeaderParserHelper> hph;
                FAIL_RETURN(CHeaderParserHelper::AcquireSingleton((IHeaderParserHelper**)&hph));
                return hph->ParseSeconds(parameter, &mRh->mMinFreshSeconds);
            }
            else if (directive.EqualsIgnoreCase("only-if-cached")) {
                mRh->mOnlyIfCached = TRUE;
                return NOERROR;
            }
            return NOERROR;
        }
    private:
        AutoPtr<CRequestHeaders> mRh;
    };

public:
    CARAPI constructor(
        /* [in] */ IURI* uri,
        /* [in] */ IRawHeaders* headers);

    CARAPI IsChunked(
        /* [out] */ Boolean* value);

    CARAPI HasConnectionClose(
        /* [out] */ Boolean* value);

    CARAPI GetUri(
        /* [out] */ IURI** url);

    CARAPI GetHeaders(
        /* [out] */ IRawHeaders** rh);

    CARAPI IsNoCache(
        /* [out] */ Boolean* value);

    CARAPI GetMaxAgeSeconds(
        /* [out] */ Int32* value);

    CARAPI GetMaxStaleSeconds(
        /* [out] */ Int32* value);

    CARAPI GetMinFreshSeconds(
        /* [out] */ Int32* value);

    CARAPI IsOnlyIfCached(
        /* [out] */ Boolean* value);

    CARAPI HasAuthorization(
        /* [out] */ Boolean* value);

    CARAPI GetContentLength(
        /* [out] */ Int32* value);

    CARAPI GetTransferEncoding(
        /* [out] */ String* str);

    CARAPI GetUserAgent(
        /* [out] */ String* str);

    CARAPI GetHost(
        /* [out] */ String* str);

    CARAPI GetConnection(
        /* [out] */ String* str);

    CARAPI GetAcceptEncoding(
        /* [out] */ String* str);

    CARAPI GetContentType(
        /* [out] */ String* str);

    CARAPI GetIfModifiedSince(
        /* [out] */ String* str);

    CARAPI GetIfNoneMatch(
        /* [out] */ String* str);

    CARAPI GetProxyAuthorization(
        /* [out] */ String* str);

    CARAPI SetChunked();

    CARAPI SetContentLength(
        /* [in] */ Int32 contentLength);

    CARAPI SetUserAgent(
        /* [in] */ const String& userAgent);

    CARAPI SetHost(
        /* [in] */ const String& host);

    CARAPI SetConnection(
        /* [in] */ const String& connection);

    CARAPI SetAcceptEncoding(
        /* [in] */ const String& acceptEncoding);

    CARAPI SetContentType(
        /* [in] */ const String& contentType);

    CARAPI SetIfModifiedSince(
        /* [in] */ IDate* date);

    CARAPI SetIfNoneMatch(
        /* [in] */ const String& ifNoneMatch);

    CARAPI HasConditions(
        /* [out] */ Boolean* value);

    CARAPI AddCookies(
        /* [in] */ IMap* allCookieHeaders);

private:
    AutoPtr<IURI> mUri;
    AutoPtr<IRawHeaders> mHeaders;

    /** Don't use a cache to satisfy this request. */
    Boolean mNoCache;
    Int32 mMaxAgeSeconds; // = -1;
    Int32 mMaxStaleSeconds; // = -1;
    Int32 mMinFreshSeconds; // = -1;

    /**
     * This field's name "only-if-cached" is misleading. It actually means "do
     * not use the network". It is set by a client who only wants to make a
     * request if it can be fully satisfied by the cache. Cached responses that
     * would require validation (ie. conditional gets) are not permitted if this
     * header is set.
     */
    Boolean mOnlyIfCached;

    /**
     * True if the request contains an authorization field. Although this isn't
     * necessarily a shared cache, it follows the spec's strict requirements for
     * shared caches.
     */
    Boolean mHasAuthorization;

    Int32 mContentLength; // = -1;
    String mTransferEncoding;
    String mUserAgent;
    String mHost;
    String mConnection;
    String mAcceptEncoding;
    String mContentType;
    String mIfModifiedSince;
    String mIfNoneMatch;
    String mProxyAuthorization;
};

} // namespace Http
} // namespace Net
} // namespace Elastos

#endif //__CREQUESTHEADERS_H__
