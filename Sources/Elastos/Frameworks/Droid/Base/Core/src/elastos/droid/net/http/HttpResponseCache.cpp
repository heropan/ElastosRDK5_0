
#include "_Elastos.Droid.Core.h"
#include "elastos/droid/net/http/HttpResponseCache.h"

using Elastos::Droid::Content::IContext;

using Elastos::IO::EIID_ICloseable;
using Elastos::Net::IHttpURLConnection;
using Elastos::Utility::IList;
using Elastosx::Net::Ssl::IHttpsURLConnection;

using Org::Apache::Http::Impl::Client::IDefaultHttpClient;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

CAR_INTERFACE_IMPL_2(HttpResponseCache, ResponseCache, IHttpResponseCache, ICloseable)

ECode HttpResponseCache::constructor(
    /* [in] */ Com::Squareup::Okhttp::IHttpResponseCache* delegate)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        this.delegate = delegate;
#endif
}

ECode HttpResponseCache::GetInstalled(
    /* [out] */ IHttpResponseCache** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        ResponseCache installed = ResponseCache.getDefault();
        if (installed instanceof com.android.okhttp.HttpResponseCache) {
            return new HttpResponseCache(
                    (com.android.okhttp.HttpResponseCache) installed);
        }
        return NULL;
#endif
}

ECode HttpResponseCache::Install(
    /* [in] */ IFile* directory,
    /* [in] */ Int64 maxSize,
    /* [out] */ IHttpResponseCache** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        ResponseCache installed = ResponseCache.getDefault();
        if (installed instanceof com.android.okhttp.HttpResponseCache) {
            com.android.okhttp.HttpResponseCache installedCache =
                    (com.android.okhttp.HttpResponseCache) installed;
            // don't close and reopen if an equivalent cache is already installed
            if (installedCache.getDirectory().equals(directory)
                    && installedCache.getMaxSize() == maxSize
                    && !installedCache.isClosed()) {
                return new HttpResponseCache(installedCache);
            } else {
                // The HttpResponseCache that owns this object is about to be replaced.
                installedCache.close();
            }
        }
        com.android.okhttp.HttpResponseCache responseCache =
                new com.android.okhttp.HttpResponseCache(directory, maxSize);
        ResponseCache.setDefault(responseCache);
        return new HttpResponseCache(responseCache);
#endif
}

ECode HttpResponseCache::Get(
    /* [in] */ IURI* uri,
    /* [in] */ const String& requestMethod,
    /* [in] */ IMap* requestHeaders,
    /* [out] */ ICacheResponse** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return delegate.get(uri, requestMethod, requestHeaders);
#endif
}

ECode HttpResponseCache::Put(
    /* [in] */ IURI* uri,
    /* [in] */ IURLConnection* urlConnection,
    /* [out] */ ICacheRequest** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return delegate.put(uri, urlConnection);
#endif
}

ECode HttpResponseCache::Size(
    /* [out] */ Int64* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return delegate.getSize();
#endif
}

ECode HttpResponseCache::MaxSize(
    /* [out] */ Int64* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return delegate.getMaxSize();
#endif
}

ECode HttpResponseCache::Flush()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        try {
            delegate.flush();
        } catch (IOException ignored) {
        }
#endif
}

ECode HttpResponseCache::GetNetworkCount(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return delegate.getNetworkCount();
#endif
}

ECode HttpResponseCache::GetHitCount(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return delegate.getHitCount();
#endif
}

ECode HttpResponseCache::GetRequestCount(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return delegate.getRequestCount();
#endif
}

ECode HttpResponseCache::Close()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (ResponseCache.getDefault() == this.delegate) {
            ResponseCache.setDefault(NULL);
        }
        delegate.close();
#endif
}

ECode HttpResponseCache::Delete()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (ResponseCache.getDefault() == this.delegate) {
            ResponseCache.setDefault(NULL);
        }
        delegate.delete();
#endif
}

} // namespace Http
} // namespace Net
} // namespace Droid
} // namespace Elastos
