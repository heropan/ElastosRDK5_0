
#include "elastos/droid/webkit/CCacheManager.h"
#include "elastos/droid/webkit/DebugFlags.h"
#include <elastos/utility/logging/Logger.h>

using Elastos::Core::CString;
using Elastos::Core::CSystem;
using Elastos::Core::CThread;
using Elastos::Core::EIID_IRunnable;
using Elastos::Core::ICharSequence;
using Elastos::Core::IString;
using Elastos::Core::ISystem;
using Elastos::Core::IThread;
using Elastos::IO::CFile;
using Elastos::IO::CFileInputStream;
using Elastos::IO::IFileInputStream;
using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Webkit {

//===============================================================
//              CCacheManager::CacheResult
//===============================================================

CAR_INTERFACE_IMPL(CCacheManager::CacheResult, Object, ICacheManagerCacheResult)

CCacheManager::CacheResult::CacheResult()
    : mHttpStatusCode(0)
    , mContentLength(0)
    , mExpires(0)
{
}

ECode CCacheManager::CacheResult::GetHttpStatusCode(
    /* [out] */ Int32* statusCode)
{
    VALIDATE_NOT_NULL(statusCode);
    *statusCode = mHttpStatusCode;
    return NOERROR;
}

ECode CCacheManager::CacheResult::GetContentLength(
    /* [out] */ Int64* length)
{
    VALIDATE_NOT_NULL(length);
    *length = mContentLength;
    return NOERROR;
}

ECode CCacheManager::CacheResult::GetLocalPath(
    /* [out] */ String* path)
{
    VALIDATE_NOT_NULL(path);
    *path = mLocalPath;
    return NOERROR;
}

ECode CCacheManager::CacheResult::GetExpires(
    /* [out] */ Int64* expires)
{
    VALIDATE_NOT_NULL(expires);
    *expires = mExpires;
    return NOERROR;
}

ECode CCacheManager::CacheResult::GetExpiresString(
    /* [out] */ String* expires)
{
    VALIDATE_NOT_NULL(expires);
    *expires = mExpiresString;
    return NOERROR;
}

ECode CCacheManager::CacheResult::GetLastModified(
    /* [out] */ String* lastModified)
{
    VALIDATE_NOT_NULL(lastModified);
    *lastModified = mLastModified;
    return NOERROR;
}

ECode CCacheManager::CacheResult::GetETag(
    /* [out] */ String* etag)
{
    VALIDATE_NOT_NULL(etag);
    *etag = mEtag;
    return NOERROR;
}

ECode CCacheManager::CacheResult::GetMimeType(
    /* [out] */ String* mimeType)
{
    VALIDATE_NOT_NULL(mimeType);
    *mimeType = mMimeType;
    return NOERROR;
}

ECode CCacheManager::CacheResult::GetLocation(
    /* [out] */ String* location)
{
    VALIDATE_NOT_NULL(location);
    *location = mLocation;
    return NOERROR;
}

ECode CCacheManager::CacheResult::GetEncoding(
    /* [out] */ String* encoding)
{
    VALIDATE_NOT_NULL(encoding);
    *encoding = mEncoding;
    return NOERROR;
}

ECode CCacheManager::CacheResult::GetContentDisposition(
    /* [out] */ String* contentdisposition)
{
    VALIDATE_NOT_NULL(contentdisposition);
    *contentdisposition = mContentdisposition;
    return NOERROR;
}

ECode CCacheManager::CacheResult::GetInputStream(
    /* [out] */ IInputStream** inStream)
{
    VALIDATE_NOT_NULL(inStream);
    *inStream = mInStream;
    REFCOUNT_ADD(*inStream);
    return NOERROR;
}

ECode CCacheManager::CacheResult::GetOutputStream(
    /* [out] */ IOutputStream** outStream)
{
    VALIDATE_NOT_NULL(outStream);
    *outStream = mOutStream;
    REFCOUNT_ADD(*outStream);
    return NOERROR;
}

ECode CCacheManager::CacheResult::SetInputStream(
    /* [in] */ IInputStream* stream)
{
    mInStream = stream;
    return NOERROR;
}

ECode CCacheManager::CacheResult::SetEncoding(
    /* [in] */ const String& encoding)
{
    mEncoding = encoding;
    return NOERROR;
}

ECode CCacheManager::CacheResult::SetContentLength(
    /* [in] */ Int64 contentLength)
{
    mContentLength = contentLength;
    return NOERROR;
}

//===============================================================
//                       CCacheManager
//===============================================================

CAR_INTERFACE_IMPL(CCacheManager, Object, ICacheManager);

CAR_OBJECT_IMPL(CCacheManager);

ECode CCacheManager::constructor()
{
    return NOERROR;
}

AutoPtr<IFile> CCacheManager::GetCacheFileBaseDir()
{
    return NULL;
}

ECode CCacheManager::GetCacheFileBaseDir(
    /* [out] */ IFile** dir)
{
    VALIDATE_NOT_NULL(dir);
    *dir = GetCacheFileBaseDir();
    REFCOUNT_ADD(*dir);
    return NOERROR;
}

Boolean CCacheManager::CacheDisabled()
{
    return FALSE;
}

ECode CCacheManager::CacheDisabled(
    /* [out] */ Boolean* disabled)
{
    VALIDATE_NOT_NULL(disabled);
    *disabled = CacheDisabled();
    return NOERROR;
}

Boolean CCacheManager::StartCacheTransaction()
{
    return FALSE;
}

ECode CCacheManager::StartCacheTransaction(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = StartCacheTransaction();
    return NOERROR;
}

Boolean CCacheManager::EndCacheTransaction()
{
    return FALSE;
}

ECode CCacheManager::EndCacheTransaction(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = EndCacheTransaction();
    return NOERROR;
}

AutoPtr<ICacheManagerCacheResult> CCacheManager::GetCacheFile(
    /* [in] */ const String& url,
    /* [in] */ IMap* headers)
{
    return NULL;
}

ECode CCacheManager::GetCacheFile(
    /* [in] */ const String& url,
    /* [in] */ IMap* headers,
    /* [out] */ ICacheManagerCacheResult** cacheResult)
{
    VALIDATE_NOT_NULL(cacheResult);
    *cacheResult = GetCacheFile(url,  headers);
    REFCOUNT_ADD(*cacheResult);
    return NOERROR;
}

/**
 * Given a URL and its full headers, gets a CacheResult if a local cache
 * can be stored. Otherwise returns null. The mimetype is passed in so that
 * the function can use the mimetype that will be passed to WebCore which
 * could be different from the mimetype defined in the headers.
 * forceCache is for out-of-package callers to force creation of a
 * CacheResult, and is used to supply surrogate responses for URL
 * interception.
 *
 * @return a CacheResult for a given URL
 */
AutoPtr<ICacheManagerCacheResult> CCacheManager::CreateCacheFile(
    /* [in] */ const String& url,
    /* [in] */ Int32 statusCode,
    /* [in] */ IHeaders* headers,
    /* [in] */ const String& mimeType,
    /* [in] */ Boolean forceCache)
{
    // This method is public but hidden. We break functionality.
    return NULL;
}

void CCacheManager::_SaveCacheFile(
    /* [in] */ const String& url,
    /* [in] */ ICacheManagerCacheResult* cacheResult)
{
    SaveCacheFile(url, 0, cacheResult);
}

ECode CCacheManager::SaveCacheFile(
    /* [in] */ const String& url,
    /* [in] */ ICacheManagerCacheResult* cacheResult)
{
    _SaveCacheFile(url, cacheResult);
    return NOERROR;
}

void CCacheManager::SaveCacheFile(
    /* [in] */ const String& url,
    /* [in] */ Int64 postIdentifier,
    /* [in] */ ICacheManagerCacheResult* cacheRet)
{
    //try {
    AutoPtr<IOutputStream> outStream;
    cacheRet->GetOutputStream((IOutputStream**)&outStream);
    outStream->Close();
    //} catch (IOException e) {
    //    return;
    //}

    // This method is exposed in the public API but the API provides no
    // way to obtain a new CacheResult object with a non-null output
    // stream ...
    // - CacheResult objects returned by getCacheFile() have a null
    //   output stream.
    // - new CacheResult objects have a null output stream and no
    //   setter is provided.
    // Since this method throws a null pointer exception in this case,
    // it is effectively useless from the point of view of the public
    // API.
    //
    // With the Chromium HTTP stack we continue to throw the same
    // exception for 'backwards compatibility' with the Android HTTP
    // stack.
    //
    // This method is not used from within this package, and for public API
    // use, we should already have thrown an exception above.
    assert(FALSE);
}

} // namespace Webkit
} // namespace Droid
} // namespace Elastos
