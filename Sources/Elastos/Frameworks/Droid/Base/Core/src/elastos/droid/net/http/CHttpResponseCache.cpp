
#include "CHttpResponseCache.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

CHttpResponseCache::CHttpResponseCache()
{}

CHttpResponseCache::CHttpResponseCache(
    /* [in] */ IFile* directory,
    /* [in] */ Int64 maxSize)
{
    // ASSERT_SUCCEEDED(!Elastos::Net::Http::CHttpResponseCache(directory, maxSize,
    //     (Elastos::Net::Http::IHttpResponseCache**)&mDelegate));
}

ECode CHttpResponseCache::Get(
    /* [in] */ Elastos::Net::IURI* uri,
    /* [in] */ const String& requestMethod,
    /* [in] */ IObjectStringMap* requestHeaders, /*Map<String, AutoPtr<List<String> > >*/
    /* [out] */ Elastos::Net::ICacheResponse** response)
{
    // return mDelegate->Get(uri, requestMethod, requestHeaders, response);
    return E_NOT_IMPLEMENTED;
}

ECode CHttpResponseCache::Put(
    /* [in] */ Elastos::Net::IURI* uri,
    /* [in] */ Elastos::Net::IURLConnection* urlConnection,
    /* [out] */ Elastos::Net::ICacheRequest** request)
{
    // return mDelegate->Put(uri, requestMethod, requestHeaders, request);
    return E_NOT_IMPLEMENTED;
}

ECode CHttpResponseCache::Size(
    /* [out] */ Int64* size)
{
    // AutoPtr<Elastos::IO::IDiskLruCache> cache;
    // mDelegate->GetCache((Elastos::IO::IDiskLruCache**)&cache);
    // return cache->Size(size);
    return E_NOT_IMPLEMENTED;
}

ECode CHttpResponseCache::MaxSize(
    /* [out] */ Int64* maxSize)
{
    // AutoPtr<Elastos::IO::IDiskLruCache> cache;
    // mDelegate->GetCache((Elastos::IO::IDiskLruCache**)&cache);
    // return cache->MaxSize(maxSize);
    return E_NOT_IMPLEMENTED;
}

ECode CHttpResponseCache::Flush()
{
    // AutoPtr<Elastos::IO::IDiskLruCache> cache;
    // mDelegate->GetCache((Elastos::IO::IDiskLruCache**)&cache);
    // return cache->Flush();
    return E_NOT_IMPLEMENTED;
}

ECode CHttpResponseCache::GetNetworkCount(
    /* [out] */ Int32* count)
{
    // return mDelegate->GetNetworkCount(count);
    return E_NOT_IMPLEMENTED;
}

ECode CHttpResponseCache::GetHitCount(
    /* [out] */ Int32* count)
{
    // return mDelegate->GetHitCount(count);
    return E_NOT_IMPLEMENTED;
}

ECode CHttpResponseCache::GetRequestCount(
    /* [out] */ Int32* count)
{
    // return mDelegate->GetRequestCount(count);
    return E_NOT_IMPLEMENTED;
}

ECode CHttpResponseCache::TrackResponse(
    /* [in] */ Elastos::Net::IResponseSource* source)
{
    // return mDelegate->TrackResponse(source);
    return E_NOT_IMPLEMENTED;
}

ECode CHttpResponseCache::TrackConditionalCacheHit()
{
    // return mDelegate->TrackConditionalCacheHit();
    return E_NOT_IMPLEMENTED;
}

ECode CHttpResponseCache::Update(
    /* [in] */ Elastos::Net::ICacheResponse* conditionalCacheHit,
    /* [in] */Elastos::Net::IHttpURLConnection* connection)
{
    // return mDelegate->Update(conditionalCacheHit, connection);
    return E_NOT_IMPLEMENTED;
}

ECode CHttpResponseCache::Close()
{
    // AutoPtr<Elastos::Net::Http::IResponseCacheHelper> helper;
    // Elastos::Net::Http::CResponseCacheHelper::AcquireSingleton(
    //     (Elastos::Net::Http::IResponseCacheHelper**)&helper);
    // AutoPtr<Elastos::Net::Http::IResponseCache> repCache;
    // helper->GetDefault(&repCache);

    // AutoPtr<IHttpResponseCache> hRepCache = repCache->Probe(EIID_IHttpResponseCache);
    // AutoPtr<IHttpResponseCache> hRepCacheThis = this->Probe(EIID_IHttpResponseCache);

    // if (hRepCache == hRepCacheThis) {
    //     helper->SetDefault(NULL);
    // }

    // AutoPtr<Elastos::IO::IDiskLruCache> cache;
    // mDelegate->GetCache((Elastos::IO::IDiskLruCache**)&cache);
    // return cache->Close();
    return E_NOT_IMPLEMENTED;
}

ECode CHttpResponseCache::Delete()
{
    // AutoPtr<Elastos::Net::Http::IResponseCacheHelper> helper;
    // Elastos::Net::Http::CResponseCacheHelper::AcquireSingleton(
    //     (Elastos::Net::Http::IResponseCacheHelper**)&helper);
    // AutoPtr<Elastos::Net::Http::IResponseCache> repCache;
    // helper->GetDefault(&repCache);

    // AutoPtr<IHttpResponseCache> hRepCache = repCache->Probe(EIID_IHttpResponseCache);
    // AutoPtr<IHttpResponseCache> hRepCacheThis = this->Probe(EIID_IHttpResponseCache);

    // if (hRepCache == hRepCacheThis) {
    //     helper->SetDefault(NULL);
    // }

    // AutoPtr<Elastos::IO::IDiskLruCache> cache;
    // mDelegate->GetCache((Elastos::IO::IDiskLruCache**)&cache);
    // return cache->Delete();
    return E_NOT_IMPLEMENTED;
}

}
}
}
}

