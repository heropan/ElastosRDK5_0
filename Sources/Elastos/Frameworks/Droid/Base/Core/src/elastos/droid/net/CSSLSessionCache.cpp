
#include "net/CSSLSessionCache.h"
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