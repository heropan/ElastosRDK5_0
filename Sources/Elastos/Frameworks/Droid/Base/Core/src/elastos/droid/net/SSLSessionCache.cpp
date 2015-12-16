
#include "elastos/droid/net/SSLSessionCache.h"

namespace Elastos {
namespace Droid {
namespace Net {

CAR_INTERFACE_IMPL(SSLSessionCache, Object, ISSLSessionCache)

const String SSLSessionCache::TAG("SSLSessionCache");

ECode SSLSessionCache::GetSessionCache(
    /* [out] */ ISSLClientSessionCache** result)
{
    VALIDATE_NOT_NULL(*result)

    *result = mSessionCache;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode SSLSessionCache::Install(
    /* [in] */ ISSLSessionCache* cache,
    /* [in] */ ISSLContext* context)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        SSLSessionContext clientContext = context.getClientSessionContext();
        if (IClientSessionContext::Probe(clientContext) != NULL) {
            ((ClientSessionContext) clientContext).setPersistentCache(
                    cache == NULL ? NULL : cache.mSessionCache);
        } else {
            throw new IllegalArgumentException("Incompatible SSLContext: " + context);
        }
#endif
}

ECode SSLSessionCache::constructor(
    /* [in] */ IInterface* cache)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mSessionCache = (SSLClientSessionCache) cache;
#endif
}

ECode SSLSessionCache::constructor(
    /* [in] */ IFile* dir)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mSessionCache = FileClientSessionCache.usingDirectory(dir);
#endif
}

ECode SSLSessionCache::constructor(
    /* [in] */ IContext* context)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        File dir = context.getDir("sslcache", Context.MODE_PRIVATE);
        SSLClientSessionCache cache = NULL;
        try {
            cache = FileClientSessionCache.usingDirectory(dir);
        } catch (IOException e) {
            Log.w(TAG, "Unable to create SSL session cache in " + dir, e);
        }
        mSessionCache = cache;
#endif
}

} // namespace Net
} // namespace Droid
} // namespace Elastos

#if 0 // old CSSLSessionCache.cpp
#include "elastos/droid/net/CSSLSessionCache.h"
//#include "CFileClientSessionCache.h"

using Elastos::IO::IFile;
using Elastos::Droid::Content::IContext;

namespace Elastos {
namespace Droid {
namespace Net {

const String CSSLSessionCache::TAG = "SSLSessionCache";

/**
 * Create a session cache using the specified directory.
 * Individual session entries will be files within the directory.
 * Multiple instances for the same directory share data internally.
 *
 * @param dir to store session files in (created if necessary)
 * @throws IOException if the cache can't be opened
 */
CSSLSessionCache::constructor(
    /* [in] */ IFile* dir)
{
    CFileClientSessionCache::UsingDirectory(dir, (ISSLClientSessionCache**)&mSessionCache);
}

/**
 * Create a session cache at the default location for this app.
 * Multiple instances share data internally.
 *
 * @param context for the application
 */
CSSLSessionCache::SSLSessionCache(
    /* [in] */ IContext* context)
{
    AutoPtr<IFile> dir;
    context->GetDir("sslcache", IContext::MODE_PRIVATE, (IFile**)&dir);
    AutoPtr<ISSLClientSessionCache> cache = NULL;
    //try {
    CFileClientSessionCache::UsingDirectory(dir, (ISSLClientSessionCache**)&cache);
    //} catch (IOException e) {
    //    Log.w(TAG, "Unable to create SSL session cache in " + dir, e);
    //}
    mSessionCache = cache;
}

} //namespace Net
} //namespace Droid
} //namespace Elastos
#endif
