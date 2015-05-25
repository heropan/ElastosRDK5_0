
#include "CHttpResponseCache.h"
#include "HttpEngine.h"

using Elastos::Utility::ISet;

namespace Elastos {
namespace Net {
namespace Http {

const Int32 CHttpResponseCache::VERSION = 201105;
const Int32 CHttpResponseCache::ENTRY_METADATA = 0;
const Int32 CHttpResponseCache::ENTRY_BODY = 1;
const Int32 CHttpResponseCache::ENTRY_COUNT = 2;

CAR_INTERFACE_IMPL(CHttpResponseCache::CacheRequestImpl, ICacheRequest)

ECode CHttpResponseCache::constructor(
    /* [in] */ IFile* directory,
    /* [in] */ Int64 maxSize)
{
    mWriteSuccessCount = 0;
    mWriteAbortCount = 0;
    mNetworkCount = 0;
    mHitCount = 0;
    mRequestCount = 0;
    assert(0 && "TODO");
    // cache = DiskLruCache.open(directory, VERSION, ENTRY_COUNT, maxSize);
    return NOERROR;
}

// DiskLruCache getCache() {
//     return cache;
// }

ECode CHttpResponseCache::GetWriteAbortCount(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)

    Mutex::Autolock lock(mPlock);
    *value = mWriteAbortCount;
    return NOERROR;
}

ECode CHttpResponseCache::GetWriteSuccessCount(
    /* [out] */ Int32* value)
{
     VALIDATE_NOT_NULL(value)

    Mutex::Autolock lock(mPlock);
    *value = mWriteSuccessCount;
    return NOERROR;
}

ECode CHttpResponseCache::GetNetworkCount(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)

    Mutex::Autolock lock(mPlock);
    *value = mNetworkCount;
    return NOERROR;
}

ECode CHttpResponseCache::GetHitCount(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)

    Mutex::Autolock lock(mPlock);
    *value = mHitCount;
    return NOERROR;
}

ECode CHttpResponseCache::GetRequestCount(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)

    Mutex::Autolock lock(mPlock);
    *value = mRequestCount;
    return NOERROR;
}

ECode CHttpResponseCache::Get(
    /* [in] */ IURI* uri,
    /* [in] */ const String& requestMethod,
    /* [in] */ IMap* requestHeaders,
    /* [out] */ ICacheResponse** cache)
{
    assert(0 && "TODO");
    // String key = uriToKey(uri);
    // DiskLruCache.Snapshot snapshot;
    // Entry entry;
    // try {
    //     snapshot = cache.get(key);
    //     if (snapshot == null) {
    //         return null;
    //     }
    //     entry = new Entry(snapshot.getInputStream(ENTRY_METADATA));
    // } catch (IOException e) {
    //     // Give up because the cache cannot be read.
    //     return null;
    // }

    // if (!entry.matches(uri, requestMethod, requestHeaders)) {
    //     snapshot.close();
    //     return null;
    // }

    // return entry.isHttps()
    //         ? new EntrySecureCacheResponse(entry, snapshot)
    //         : new EntryCacheResponse(entry, snapshot);
    return NOERROR;
}

ECode CHttpResponseCache::Put(
    /* [in] */ IURI* uri,
    /* [in] */ IURLConnection* conn,
    /* [out] */ ICacheRequest** cache)
{
    VALIDATE_NOT_NULL(cache)

    AutoPtr<IHttpURLConnection> httpConnection = (IHttpURLConnection*) conn->Probe(EIID_IHttpURLConnection);
    if (httpConnection == NULL) {
        *cache = NULL;
        return NOERROR;
    }

    String requestMethod;
    httpConnection->GetRequestMethod(&requestMethod);
    String key = UriToKey(uri);

    if (requestMethod.Equals(HttpEngine::POST)
            || requestMethod.Equals(HttpEngine::PUT)
            || requestMethod.Equals(HttpEngine::DELETE)) {
        // try {
        assert(0 && "TODO");
        // ECode ec = mCache.remove(key);
        // } catch (IOException ignored) {
        //     // The cache cannot be written.
        // }
        *cache = NULL;
        return NOERROR;
    }
    else if (!requestMethod.Equals(HttpEngine::GET)) {

        /** Don't cache non-GET responses. We're technically allowed to cache
         * HEAD requests and some POST requests, but the complexity of doing
         * so is high and the benefit is low.
        */
        *cache = NULL;
        return NOERROR;
    }

    AutoPtr<IHttpEngine> httpEngine = GetHttpEngine(httpConnection);
    if (httpEngine == NULL) {
        // Don't cache unless the HTTP implementation is ours.
        *cache = NULL;
        return NOERROR;
    }

    AutoPtr<IResponseHeaders> response;
    httpEngine->GetResponseHeaders((IResponseHeaders**)&response);
    Boolean isflag = FALSE;
    if (response->HasVaryAll(&isflag), isflag) {
        *cache = NULL;
        return NOERROR;
    }

    AutoPtr<IRequestHeaders> requesthead;
    httpEngine->GetRequestHeaders((IRequestHeaders**)&requesthead);
    AutoPtr<IRawHeaders> rawhead;
    requesthead->GetHeaders((IRawHeaders**)&rawhead);
    AutoPtr<ISet> objc;
    response->GetVaryFields((ISet**)&objc);
    AutoPtr<IRawHeaders> varyHeaders;
    rawhead->GetAll(objc, (IRawHeaders**)&varyHeaders);
    assert(0 && "TODO");
    // Entry entry = new Entry(uri, varyHeaders, httpConnection);
    // DiskLruCache.Editor editor = null;
    // try {
    //     editor = cache.edit(key);
    //     if (editor == null) {
    //         return null;
    //     }
    //     entry.writeTo(editor);
    //     return new CacheRequestImpl(editor);
    // } catch (IOException e) {
    //     abortQuietly(editor);
    //     return null;
    // }
    return NOERROR;
}

ECode CHttpResponseCache::TrackResponse(
    /* [in] */ ResponseSource source)
{
    Mutex::Autolock lock(mPlock);
    mRequestCount++;

    switch (source) {
        case CACHE:
            mHitCount++;
            break;
        case CONDITIONAL_CACHE:
        case NETWORK:
            mNetworkCount++;
            break;
    }

    return NOERROR;
}

ECode CHttpResponseCache::TrackConditionalCacheHit()
{
    Mutex::Autolock lock(mPlock);
    mHitCount++;
    return NOERROR;
}

ECode CHttpResponseCache::Update(
    /* [in] */ ICacheResponse* conditionalCacheHit,
    /* [in] */ IHttpURLConnection* httpConnection)
{
    AutoPtr<IHttpEngine> httpEngine = GetHttpEngine(httpConnection);
    AutoPtr<IURI> uri;
    httpEngine->GetUri((IURI**)&uri);
    AutoPtr<IResponseHeaders> response;
    httpEngine->GetResponseHeaders((IResponseHeaders**)&response);
    AutoPtr<IRequestHeaders> requesthead;
    httpEngine->GetRequestHeaders((IRequestHeaders**)&requesthead);
    AutoPtr<IRawHeaders> rawhead;
    requesthead->GetHeaders((IRawHeaders**)&rawhead);
    AutoPtr<ISet> objc;
    response->GetVaryFields((ISet**)&objc);
    AutoPtr<IRawHeaders> varyHeaders;
    rawhead->GetAll(objc, (IRawHeaders**)&varyHeaders);

    assert(0 && "TODO");
    // Entry entry = new Entry(uri, varyHeaders, httpConnection);
    // DiskLruCache.Snapshot snapshot = (conditionalCacheHit instanceof EntryCacheResponse)
    //         ? ((EntryCacheResponse) conditionalCacheHit).snapshot
    //         : ((EntrySecureCacheResponse) conditionalCacheHit).snapshot;
    // DiskLruCache.Editor editor = null;
    // try {
    //     editor = snapshot.edit(); // returns null if snapshot is not current
    //     if (editor != null) {
    //         entry.writeTo(editor);
    //         editor.commit();
    //     }
    // } catch (IOException e) {
    //     abortQuietly(editor);
    // }
    return NOERROR;
}

String CHttpResponseCache::UriToKey(
    /* [in] */ IURI* uri)
{
    assert(0 && "TODO");
    // try {
    //     MessageDigest messageDigest = MessageDigest.getInstance("MD5");
    //     byte[] md5bytes = messageDigest.digest(uri.toString().getBytes(Charsets.UTF_8));
    //     return IntegralToString.bytesToHexString(md5bytes, false);
    // } catch (NoSuchAlgorithmException e) {
    //     throw new AssertionError(e);
    // }
    return String(NULL);
}

// void abortQuietly(DiskLruCache.Editor editor) {
//         // Give up because the cache cannot be written.
//         try {
//             if (editor != null) {
//                 editor.abort();
//             }
//         } catch (IOException ignored) {
//         }
// }

AutoPtr<IHttpEngine> CHttpResponseCache::GetHttpEngine(
    /* [in] */ IHttpURLConnection* httpConnection)
{
    AutoPtr<IHttpURLConnectionImpl> res = (IHttpURLConnectionImpl*) httpConnection->Probe(EIID_IHttpURLConnectionImpl);
    if (res) {
        AutoPtr<IHttpEngine> outhttp;
        res->GetHttpEngine((IHttpEngine**)&outhttp);
        return outhttp;
    }
    assert(0 && "TODO");
    // else if (httpConnection instanceof HttpsURLConnectionImpl) {
    //     return ((HttpsURLConnectionImpl) httpConnection).getHttpEngine();
    // } else {
    //     return null;
    // }
    return NULL;
}

// static InputStream newBodyInputStream(final DiskLruCache.Snapshot snapshot)

} // namespace Http
} // namespace Net
} // namespace Elastos