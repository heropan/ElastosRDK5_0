
#include "CResponseHeaders.h"
#include "CHttpDate.h"
#include "StringUtils.h"
#include "elastos/Math.h"
#include "TimeUnit.h"
#include "CRawHeaders.h"
#include "CTreeSet.h"
#include "CStringWrapper.h"
#include "ObjectUtils.h"

using Elastos::Core::StringUtils;
using Elastos::Core::Math;
using Elastos::Core::ICharSequence;
using Elastos::Core::CStringWrapper;
using Elastos::Core::ObjectUtils;
using Elastos::Utility::INavigableSet;
using Elastos::Utility::CTreeSet;
using Elastos::Utility::Concurrent::TimeUnit;
using Elastos::Utility::Concurrent::ITimeUnit;

namespace Elastos {
namespace Net {
namespace Http {

CAR_INTERFACE_IMPL(CResponseHeaders::CCacheControlHandler, ICacheControlHandler)

const String CResponseHeaders::SENT_MILLIS = String("X-Android-Sent-Millis");

const String CResponseHeaders::RECEIVED_MILLIS = String("X-Android-Received-Millis");

ECode CResponseHeaders::constructor(
    /* [in] */ IURI* uri,
    /* [in] */ IRawHeaders* headers)
{
    mMaxAgeSeconds = -1;
    mSMaxAgeSeconds = -1;
    mAgeSeconds = -1;
    mContentLength = -1;
    mSentRequestMillis = 0;
    mReceivedResponseMillis = 0;
    mNoCache = FALSE;
    mNoStore = FALSE;
    mIsPublic = FALSE;
    mMustRevalidate = FALSE;
    CCollections::_GetEmptySet((ISet**)&mVaryFields);

    mUri = uri;
    mHeaders = headers;

    AutoPtr<ICacheControlHandler> handler = new CCacheControlHandler(this);

    Int32 headlen = 0;
    for (Int32 i = 0; i < (mHeaders->Length(&headlen), headlen); i++) {
        String fieldName;
        mHeaders->GetFieldName(i, &fieldName);
        String value;
        mHeaders->GetValue(i, &value);
        if (String("Cache-Control").EqualsIgnoreCase(fieldName)) {
            AutoPtr<IHeaderParserHelper> hph;
            FAIL_RETURN(CHeaderParserHelper::AcquireSingleton((IHeaderParserHelper**)&hph));
            hph->ParseCacheControl(value, handler);
        }
        else if (String("Date").EqualsIgnoreCase(fieldName)) {
            CHttpDate::_Parse(value, (IDate**)&mServedDate);
        }
        else if (String("Expires").EqualsIgnoreCase(fieldName)) {
            CHttpDate::_Parse(value, (IDate**)&mExpires);
        }
        else if (String("Last-Modified").EqualsIgnoreCase(fieldName)) {
            CHttpDate::_Parse(value, (IDate**)&mLastModified);
        }
        else if (String("ETag").EqualsIgnoreCase(fieldName)) {
            mEtag = value;
        }
        else if (String("Pragma").EqualsIgnoreCase(fieldName)) {
            if (value.EqualsIgnoreCase("no-cache")) {
                mNoCache = TRUE;
            }
        }
        else if (String("Age").EqualsIgnoreCase(fieldName)) {
            AutoPtr<IHeaderParserHelper> hph;
            FAIL_RETURN(CHeaderParserHelper::AcquireSingleton((IHeaderParserHelper**)&hph));
            hph->ParseSeconds(value, &mAgeSeconds);
        }
        else if (String("Vary").EqualsIgnoreCase(fieldName)) {
            // Replace the immutable empty set with something we can mutate.
            Boolean isflag = FALSE;
            if (mVaryFields->IsEmpty(&isflag), isflag) {
                AutoPtr<INavigableSet> ns;
                CTreeSet::New(/*String.CASE_INSENSITIVE_ORDER*/ (INavigableSet**)&ns);
                mVaryFields = ISet::Probe(ns);
            }
            AutoPtr< ArrayOf<String> > arrstr;
            StringUtils::Split(value, ",", (ArrayOf<String>**)&arrstr);
            for (Int32 i = 0; i < arrstr->GetLength(); ++i) {
                AutoPtr<ICharSequence> sq;
                CStringWrapper::New((*arrstr)[i].Trim(), (ICharSequence**)&sq);
                mVaryFields->Add(sq, &isflag);
            }
        }
        else if (String("Content-Encoding").EqualsIgnoreCase(fieldName)) {
            mContentEncoding = value;
        }
        else if (String("Transfer-Encoding").EqualsIgnoreCase(fieldName)) {
            mTransferEncoding = value;
        }
        else if (String("Content-Length").EqualsIgnoreCase(fieldName)) {
            // try {
            mContentLength = StringUtils::ParseInt32(value);
            // } catch (NumberFormatException ignored) {
            // }
        }
        else if (String("Connection").EqualsIgnoreCase(fieldName)) {
            mConnection = value;
        }
        else if (SENT_MILLIS.EqualsIgnoreCase(fieldName)) {
            mSentRequestMillis = StringUtils::ParseInt64(value);
        }
        else if (RECEIVED_MILLIS.EqualsIgnoreCase(fieldName)) {
            mReceivedResponseMillis = StringUtils::ParseInt64(value);
        }
    }
    return NOERROR;
}

ECode CResponseHeaders::IsContentEncodingGzip(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)

    *value = String("gzip").EqualsIgnoreCase(mContentEncoding);
    return NOERROR;
}

ECode CResponseHeaders::StripContentEncoding()
{
    mContentEncoding = String(NULL);
    return mHeaders->RemoveAll(String("Content-Encoding"));
}

ECode CResponseHeaders::StripContentLength()
{
    mContentLength = -1;
    return mHeaders->RemoveAll(String("Content-Length"));
}

ECode CResponseHeaders::IsChunked(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)

    *value = String("chunked").EqualsIgnoreCase(mTransferEncoding);
    return NOERROR;
}

ECode CResponseHeaders::HasConnectionClose(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)

    *value = String("close").EqualsIgnoreCase(mConnection);
    return NOERROR;
}

ECode CResponseHeaders::GetUri(
    /* [out] */ IURI** url)
{
    VALIDATE_NOT_NULL(url)

    *url = mUri;
    INTERFACE_ADDREF(*url)
    return NOERROR;
}

ECode CResponseHeaders::GetHeaders(
    /* [out] */ IRawHeaders** rh)
{
    VALIDATE_NOT_NULL(rh)

    *rh = mHeaders;
    INTERFACE_ADDREF(*rh)
    return NOERROR;
}

ECode CResponseHeaders::GetServedDate(
    /* [out] */ IDate** adate)
{
    VALIDATE_NOT_NULL(adate)

    *adate = mServedDate;
    INTERFACE_ADDREF(*adate)
    return NOERROR;
}

ECode CResponseHeaders::GetLastModified(
    /* [out] */ IDate** adate)
{
    VALIDATE_NOT_NULL(adate)

    *adate = mLastModified;
    INTERFACE_ADDREF(*adate)
    return NOERROR;
}

ECode CResponseHeaders::GetExpires(
    /* [out] */ IDate** adate)
{
    VALIDATE_NOT_NULL(adate)

    *adate = mExpires;
    INTERFACE_ADDREF(*adate)
    return NOERROR;
}

ECode CResponseHeaders::IsNoCache(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)

    *value = mNoCache;
    return NOERROR;
}

ECode CResponseHeaders::IsNoStore(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)

    *value = mNoStore;
    return NOERROR;
}

ECode CResponseHeaders::GetMaxAgeSeconds(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)

    *value = mMaxAgeSeconds;
    return NOERROR;
}

ECode CResponseHeaders::GetSMaxAgeSeconds(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)

    *value = mSMaxAgeSeconds;
    return NOERROR;
}

ECode CResponseHeaders::IsPublic(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)

    *value = mIsPublic;
    return NOERROR;
}

ECode CResponseHeaders::IsMustRevalidate(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)

    *value = mMustRevalidate;
    return NOERROR;
}

ECode CResponseHeaders::GetEtag(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)

    *str = mEtag;
    return NOERROR;
}

ECode CResponseHeaders::GetVaryFields(
    /* [out] */ ISet** obj)
{
    VALIDATE_NOT_NULL(obj)

    *obj = mVaryFields;
    INTERFACE_ADDREF(*obj)
    return NOERROR;
}

ECode CResponseHeaders::GetContentEncoding(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)

    *str = mContentEncoding;
    return NOERROR;
}

ECode CResponseHeaders::GetContentLength(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)

    *value = mContentLength;
    return NOERROR;
}

ECode CResponseHeaders::GetConnection(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)

    *str = mConnection;
    return NOERROR;
}

ECode CResponseHeaders::SetLocalTimestamps(
    /* [in] */ Int64 sentRequestMillis,
    /* [in] */ Int64 receivedResponseMillis)
{
    mSentRequestMillis = sentRequestMillis;
    FAIL_RETURN(mHeaders->Add(SENT_MILLIS, StringUtils::Int64ToString(mSentRequestMillis)));
    mReceivedResponseMillis = receivedResponseMillis;
    return mHeaders->Add(RECEIVED_MILLIS, StringUtils::Int64ToString(mReceivedResponseMillis));
}

ECode CResponseHeaders::IsCacheable(
    /* [in] */ IRequestHeaders* request,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)
    /*
     * Always go to network for uncacheable response codes (RFC 2616, 13.4),
     * This implementation doesn't support caching partial content.
     */
    Int32 responseCode = 0;
    mHeaders->GetResponseCode(&responseCode);
    if (responseCode != IHttpURLConnection::HTTP_OK
            && responseCode != IHttpURLConnection::HTTP_NOT_AUTHORITATIVE
            && responseCode != IHttpURLConnection::HTTP_MULT_CHOICE
            && responseCode != IHttpURLConnection::HTTP_MOVED_PERM
            && responseCode != IHttpURLConnection::HTTP_GONE) {
        *value = FALSE;
        return NOERROR;
    }

    /*
     * Responses to authorized requests aren't cacheable unless they include
     * a 'public', 'must-revalidate' or 's-maxage' directive.
     */
    Boolean isflag = FALSE;
    if ((request->HasAuthorization(&isflag), isflag)
            && !mIsPublic
            && !mMustRevalidate
            && mSMaxAgeSeconds == -1) {
        *value = FALSE;
        return NOERROR;
    }

    if (mNoStore) {
        *value = FALSE;
    }

    *value = TRUE;
    return NOERROR;
}

ECode CResponseHeaders::HasVaryAll(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)

    AutoPtr<ICharSequence> sq;
    CStringWrapper::New(String("*"), (ICharSequence**)&sq);
    return mVaryFields->Contains(sq, value);
}

ECode CResponseHeaders::VaryMatches(
    /* [in] */ IMap* cachedRequest,
    /* [in] */ IMap* newRequest,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)

    AutoPtr< ArrayOf<IInterface*> > outarr;
    mVaryFields->ToArray((ArrayOf<IInterface*>**)&outarr);
    for (Int32 i =0; i < outarr->GetLength(); ++i) {
        AutoPtr<IInterface> outface1;
        AutoPtr<IInterface> outface2;
        cachedRequest->Get((*outarr)[i], (IInterface**)&outface1);
        newRequest->Get((*outarr)[i], (IInterface**)&outface2);
        if (!ObjectUtils::Equals(outface1, outface2)) {
            *value = FALSE;
            return NOERROR;
        }
    }
    *value = TRUE;
    return NOERROR;
}

ECode CResponseHeaders::ChooseResponseSource(
    /* [in] */ Int64 nowMillis,
    /* [in] */ IRequestHeaders* request,
    /* [out] */ ResponseSource* ressou)
{
    VALIDATE_NOT_NULL(ressou)

    /*
     * If this response shouldn't have been stored, it should never be used
     * as a response source. This check should be redundant as long as the
     * persistence store is well-behaved and the rules are constant.
     */
    Boolean isflag = FALSE;
    if (!(IsCacheable(request, &isflag), isflag)) {
        *ressou = NETWORK;
        return NOERROR;
    }
    Boolean isflag2 = FALSE;
    if ((request->IsNoCache(&isflag), isflag) || (request->HasConditions(&isflag2), isflag2)) {
        *ressou = NETWORK;
    }

    Int64 ageMillis = ComputeAge(nowMillis);
    Int64 freshMillis = ComputeFreshnessLifetime();

    AutoPtr<ITimeUnit> timeseconds = TimeUnit::GetSECONDS();
    Int32 agevalue = 0;
    if ((request->GetMaxAgeSeconds(&agevalue), agevalue) != -1) {
        Int64 secondvalue = 0;
        freshMillis = Elastos::Core::Math::Min(freshMillis,
                (timeseconds->ToMillis((Int64)agevalue, &secondvalue), secondvalue));
    }

    Int64 minFreshMillis = 0;
    if ((request->GetMinFreshSeconds(&agevalue), agevalue) != -1) {
        timeseconds->ToMillis(agevalue, &minFreshMillis);
    }

    Int64 maxStaleMillis = 0;
    if (!mMustRevalidate && (request->GetMaxStaleSeconds(&agevalue), agevalue) != -1) {
        timeseconds->ToMillis(agevalue, &maxStaleMillis);
    }

    if (!mNoCache && ageMillis + minFreshMillis < freshMillis + maxStaleMillis) {
        if (ageMillis + minFreshMillis >= freshMillis) {
            mHeaders->Add(String("Warning"), String("110 HttpURLConnection \"Response is stale\""));
        }

        AutoPtr<ITimeUnit> timeshours = TimeUnit::GetHOURS();
        Int64 hourvalue = 0;
        if (ageMillis > (timeshours->ToMillis(24, &hourvalue), hourvalue) && IsFreshnessLifetimeHeuristic()) {
            mHeaders->Add(String("Warning"), String("113 HttpURLConnection \"Heuristic expiration\""));
        }
        *ressou = CACHE;
    }

    if (mLastModified != NULL) {
        request->SetIfModifiedSince(mLastModified);
    }
    else if (mServedDate != NULL) {
        request->SetIfModifiedSince(mServedDate);
    }

    if (mEtag != NULL) {
        request->SetIfNoneMatch(mEtag);
    }

    *ressou = (request->HasConditions(&isflag), isflag)
            ? CONDITIONAL_CACHE
            : NETWORK;
    return NOERROR;
}

ECode CResponseHeaders::Validate(
    /* [in] */ IResponseHeaders* networkResponse,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)

    Int32 codevalue = 0;
    if ((((CResponseHeaders*)networkResponse)->mHeaders->GetResponseCode(&codevalue), codevalue) == IHttpURLConnection::HTTP_NOT_MODIFIED) {
        *value = TRUE;
        return NOERROR;
    }

    /*
     * The HTTP spec says that if the network's response is older than our
     * cached response, we may return the cache's response. Like Chrome (but
     * unlike Firefox), this client prefers to return the newer response.
     */
    Int64 time1 = 0;
    Int64 time2 = 0;
    if (mLastModified != NULL
            && ((CResponseHeaders*)networkResponse)->mLastModified != NULL
            && (((CResponseHeaders*)networkResponse)->mLastModified->GetTime(&time1), time1) < (mLastModified->GetTime(&time2), time2)) {
        *value = TRUE;
        return NOERROR;
    }

    *value = FALSE;
    return NOERROR;
}

ECode CResponseHeaders::Combine(
    /* [in] */ IResponseHeaders* network,
    /* [out] */ IResponseHeaders** outres)
{
    VALIDATE_NOT_NULL(outres)

    AutoPtr<IRawHeaders> result;
    FAIL_RETURN(CRawHeaders::New((IRawHeaders**)&result));
    String statusline;
    mHeaders->GetStatusLine(&statusline);
    result->SetStatusLine(statusline);

    Int32 headlen = 0;
    for (Int32 i = 0; i < (mHeaders->Length(&headlen), headlen); i++) {
        String fieldName;
        mHeaders->GetFieldName(i, &fieldName);
        String value;
        mHeaders->GetValue(i, &value);
        if (fieldName.Equals("Warning") && value.StartWith("1")) {
            continue; // drop 100-level freshness warnings
        }
        String outstr;
        if (!IsEndToEnd(fieldName) || (((CResponseHeaders*)network)->mHeaders->Get(fieldName, &outstr), outstr.IsNull())) {
            result->Add(fieldName, value);
        }
    }

    for (Int32 i = 0; i < (((CResponseHeaders*)network)->mHeaders->Length(&headlen), headlen); i++) {
        String fieldName;
        ((CResponseHeaders*)network)->mHeaders->GetFieldName(i, &fieldName);
        if (IsEndToEnd(fieldName)) {
            String outstr;
            result->Add(fieldName, (((CResponseHeaders*)network)->mHeaders->GetValue(i, &outstr), outstr));
        }
    }

    AutoPtr<CResponseHeaders> outrh;
    FAIL_RETURN(CResponseHeaders::NewByFriend(mUri, result, (CResponseHeaders**)&outrh));
    *outres = (IResponseHeaders*) outrh.Get();
    INTERFACE_ADDREF(*outres)

    return NOERROR;
}

Int64 CResponseHeaders::ComputeAge(
    /* [in] */ Int64 nowMillis)
{
    Int64 timevalue = 0;
    Int64 apparentReceivedAge = mServedDate != NULL
            ? Elastos::Core::Math::Max(0L, mReceivedResponseMillis - (mServedDate->GetTime(&timevalue), timevalue))
            : 0;
    AutoPtr<ITimeUnit> timeseconds = TimeUnit::GetSECONDS();
    Int64 receivedAge = mAgeSeconds != -1
            ? Elastos::Core::Math::Max(apparentReceivedAge, (timeseconds->ToMillis(mAgeSeconds, &timevalue), timevalue))
            : apparentReceivedAge;
    Int64 responseDuration = mReceivedResponseMillis - mSentRequestMillis;
    Int64 residentDuration = nowMillis - mReceivedResponseMillis;
    return receivedAge + responseDuration + residentDuration;
}

Int64 CResponseHeaders::ComputeFreshnessLifetime()
{
    Int64 resultvalue = 0;
    String outstr;
    if (mMaxAgeSeconds != -1) {
        AutoPtr<ITimeUnit> timeseconds = TimeUnit::GetSECONDS();
        timeseconds->ToMillis(mMaxAgeSeconds, &resultvalue);
        return resultvalue;
    }
    else if (mExpires != NULL) {
        Int64 servedMillis = mServedDate != NULL ? (mServedDate->GetTime(&resultvalue), resultvalue) : mReceivedResponseMillis;
        Int64 delta = (mExpires->GetTime(&resultvalue), resultvalue) - servedMillis;
        return delta > 0 ? delta : 0;
    }
    else if (mLastModified != NULL && (mUri->GetRawQuery(&outstr), outstr.IsNull())) {
        /*
         * As recommended by the HTTP RFC and implemented in Firefox, the
         * max age of a document should be defaulted to 10% of the
         * document's age at the time it was served. Default expiration
         * dates aren't used for URIs containing a query.
         */
        Int64 servedMillis = mServedDate != NULL ? (mServedDate->GetTime(&resultvalue), resultvalue) : mSentRequestMillis;
        Int64 delta = servedMillis - (mLastModified->GetTime(&resultvalue), resultvalue);
        return delta > 0 ? (delta / 10) : 0;
    }
    return 0;
}

Boolean CResponseHeaders::IsFreshnessLifetimeHeuristic()
{
    return mMaxAgeSeconds == -1 && mExpires == NULL;
}

Boolean CResponseHeaders::IsEndToEnd(
    /* [in] */ const String& fieldName)
{
    return !fieldName.EqualsIgnoreCase("Connection")
            && !fieldName.EqualsIgnoreCase("Keep-Alive")
            && !fieldName.EqualsIgnoreCase("Proxy-Authenticate")
            && !fieldName.EqualsIgnoreCase("Proxy-Authorization")
            && !fieldName.EqualsIgnoreCase("TE")
            && !fieldName.EqualsIgnoreCase("Trailers")
            && !fieldName.EqualsIgnoreCase("Transfer-Encoding")
            && !fieldName.EqualsIgnoreCase("Upgrade");
}

} // namespace Http
} // namespace Net
} // namespace Elastos
