
#ifndef __CRESPONSEHEADERS_H__
#define __CRESPONSEHEADERS_H__

#include "_CResponseHeaders.h"
#include "CHeaderParserHelper.h"

using Elastos::Utility::IDate;
using Elastos::Utility::IMap;
using Elastos::Utility::ISet;

namespace Elastos {
namespace Net {
namespace Http {

CarClass(CResponseHeaders)
{
private:
    class CCacheControlHandler
        : public ElRefBase
        , public ICacheControlHandler
    {
    public:
        CAR_INTERFACE_DECL()

        CCacheControlHandler(CResponseHeaders* rh)
        {
            mRh = rh;
        }

        ECode Handle(
            /* [in] */ const String& directive,
            /* [in] */ const String& parameter)
        {
            if (directive.EqualsIgnoreCase("no-cache")) {
                mRh->mNoCache = TRUE;
            }
            else if (directive.EqualsIgnoreCase("no-store")) {
                mRh->mNoStore = TRUE;
            }
            else if (directive.EqualsIgnoreCase("max-age")) {
                AutoPtr<IHeaderParserHelper> hph;
                FAIL_RETURN(CHeaderParserHelper::AcquireSingleton((IHeaderParserHelper**)&hph));
                hph->ParseSeconds(parameter, &mRh->mMaxAgeSeconds);
            }
            else if (directive.EqualsIgnoreCase("s-maxage")) {
                AutoPtr<IHeaderParserHelper> hph;
                FAIL_RETURN(CHeaderParserHelper::AcquireSingleton((IHeaderParserHelper**)&hph));
                 hph->ParseSeconds(parameter, &mRh->mSMaxAgeSeconds);
            }
            else if (directive.EqualsIgnoreCase("public")) {
                mRh->mIsPublic = TRUE;
            }
            else if (directive.EqualsIgnoreCase("must-revalidate")) {
                mRh->mMustRevalidate = TRUE;
            }
            return NOERROR;
        }
    private:
        AutoPtr<CResponseHeaders> mRh;
    };

public:
    CARAPI constructor(
        /* [in] */ IURI* uri,
        /* [in] */ IRawHeaders* headers);

    CARAPI IsContentEncodingGzip(
        /* [out] */ Boolean* value);

    CARAPI StripContentEncoding();

    CARAPI StripContentLength();

    CARAPI IsChunked(
        /* [out] */ Boolean* value);

    CARAPI HasConnectionClose(
        /* [out] */ Boolean* value);

    CARAPI GetUri(
        /* [out] */ IURI** url);

    CARAPI GetHeaders(
        /* [out] */ IRawHeaders** rh);

    CARAPI GetServedDate(
        /* [out] */ IDate** adate);

    CARAPI GetLastModified(
        /* [out] */ IDate** adate);

    CARAPI GetExpires(
        /* [out] */ IDate** adate);

    CARAPI IsNoCache(
        /* [out] */ Boolean* value);

    CARAPI IsNoStore(
        /* [out] */ Boolean* value);

    CARAPI GetMaxAgeSeconds(
        /* [out] */ Int32* value);

    CARAPI GetSMaxAgeSeconds(
        /* [out] */ Int32* value);

    CARAPI IsPublic(
        /* [out] */ Boolean* value);

    CARAPI IsMustRevalidate(
        /* [out] */ Boolean* value);

    CARAPI GetEtag(
        /* [out] */ String* str);

    CARAPI GetVaryFields(
        /* [out] */ ISet** obj);

    CARAPI GetContentEncoding(
        /* [out] */ String* str);

    CARAPI GetContentLength(
        /* [out] */ Int32* value);

    CARAPI GetConnection(
        /* [out] */ String* str);

    CARAPI SetLocalTimestamps(
        /* [in] */ Int64 sentRequestMillis,
        /* [in] */ Int64 receivedResponseMillis);

    CARAPI IsCacheable(
        /* [in] */ IRequestHeaders* request,
        /* [out] */ Boolean* value);

    CARAPI HasVaryAll(
        /* [out] */ Boolean* value);

    CARAPI VaryMatches(
        /* [in] */ IMap* cachedRequest,
        /* [in] */ IMap* newRequest,
        /* [out] */ Boolean* value);

    CARAPI ChooseResponseSource(
        /* [in] */ Int64 nowMillis,
        /* [in] */ IRequestHeaders* request,
        /* [out] */ ResponseSource* ressou);

    CARAPI Validate(
        /* [in] */ IResponseHeaders* networkResponse,
        /* [out] */ Boolean* value);

    CARAPI Combine(
        /* [in] */ IResponseHeaders* network,
        /* [out] */ IResponseHeaders** outres);

private:
    CARAPI_(Int64) ComputeAge(
        /* [in] */ Int64 nowMillis);

    CARAPI_(Int64) ComputeFreshnessLifetime();

    CARAPI_(Boolean) IsFreshnessLifetimeHeuristic();

    static CARAPI_(Boolean) IsEndToEnd(
        /* [in] */ const String& fieldName);

private:
    /** HTTP header name for the local time when the request was sent. */
    static const String SENT_MILLIS; // = "X-Android-Sent-Millis";

    /** HTTP header name for the local time when the response was received. */
    static const String RECEIVED_MILLIS; // = "X-Android-Received-Millis";

    AutoPtr<IURI> mUri;
    AutoPtr<IRawHeaders> mHeaders;

    /** The server's time when this response was served, if known. */
    AutoPtr<IDate> mServedDate;

    /** The last modified date of the response, if known. */
    AutoPtr<IDate> mLastModified;

    /**
     * The expiration date of the response, if known. If both this field and the
     * max age are set, the max age is preferred.
     */
    AutoPtr<IDate> mExpires;

    /**
     * Extension header set by HttpURLConnectionImpl specifying the timestamp
     * when the HTTP request was first initiated.
     */
    Int64 mSentRequestMillis;

    /**
     * Extension header set by HttpURLConnectionImpl specifying the timestamp
     * when the HTTP response was first received.
     */
    Int64 mReceivedResponseMillis;

    /**
     * In the response, this field's name "no-cache" is misleading. It doesn't
     * prevent us from caching the response; it only means we have to validate
     * the response with the origin server before returning it. We can do this
     * with a conditional get.
     */
    Boolean mNoCache;

    /** If true, this response should not be cached. */
    Boolean mNoStore;

    /**
     * The duration past the response's served date that it can be served
     * without validation.
     */
    Int32 mMaxAgeSeconds;

    /**
     * The "s-maxage" directive is the max age for shared caches. Not to be
     * confused with "max-age" for non-shared caches, As in Firefox and Chrome,
     * this directive is not honored by this cache.
     */
    Int32 mSMaxAgeSeconds;

    /**
     * This request header field's name "only-if-cached" is misleading. It
     * actually means "do not use the network". It is set by a client who only
     * wants to make a request if it can be fully satisfied by the cache.
     * Cached responses that would require validation (ie. conditional gets) are
     * not permitted if this header is set.
     */
    Boolean mIsPublic;
    Boolean mMustRevalidate;
    String mEtag;
    Int32 mAgeSeconds;

    /** Case-insensitive set of field names. */
    AutoPtr<ISet> mVaryFields;

    String mContentEncoding;
    String mTransferEncoding;
    Int32 mContentLength;
    String mConnection;
};

} // namespace Http
} // namespace Net
} // namespace Elastos

#endif //__CRESPONSEHEADERS_H__
