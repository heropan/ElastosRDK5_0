
#include "webkit/CCacheManager.h"
#include "webkit/DebugFlags.h"
#include <elastos/Logger.h>

using Elastos::Core::CStringWrapper;
using Elastos::Core::EIID_IRunnable;
using Elastos::Core::ICharSequence;
using Elastos::Core::ISystem;
using Elastos::Core::CSystem;
using Elastos::IO::CFile;
using Elastos::IO::IFileInputStream;
using Elastos::IO::CFileInputStream;
using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Webkit {

struct ElaCacheManagerCallback CCacheManager::sElaCacheManagerCallback = {
    &CCacheManager::CreateCCacheManagerCacheResult,
    &CCacheManager::_GetCacheFileBaseDir,
    &CCacheManager::SetContentdisposition,
    &CCacheManager::SetContentLength,
    &CCacheManager::SetEtag,
    &CCacheManager::SetEncoding,
    &CCacheManager::SetExpires,
    &CCacheManager::SetExpiresString,
    &CCacheManager::SetHttpStatusCode,
    &CCacheManager::SetLastModified,
    &CCacheManager::SetLocalPath,
    &CCacheManager::SetLocation,
    &CCacheManager::SetMimeType,
};


//===============================================================
//              CCacheManager::CacheResult
//===============================================================
CAR_INTERFACE_IMPL_LIGHT(CCacheManager::CacheResult, ICacheManagerCacheResult)

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
    INTERFACE_ADDREF(*inStream);
    return NOERROR;
}

ECode CCacheManager::CacheResult::GetOutputStream(
    /* [out] */ IOutputStream** outStream)
{
    VALIDATE_NOT_NULL(outStream);
    *outStream = mOutStream;
    INTERFACE_ADDREF(*outStream);
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
//              CCacheManager::InnerRunnable
//===============================================================
ECode CCacheManager::InnerRunnable::Run()
{
    // delete all cache files
    //try {
    AutoPtr< ArrayOf<String> > files;
    CCacheManager::sBaseDir->List((ArrayOf<String>**)&files);
    // if mBaseDir doesn't exist, files can be null.
    if (files != NULL) {
        Int32 length = files->GetLength();
        for (Int32 i = 0; i < length; i++) {
            AutoPtr<IFile> f;
            CFile::New(CCacheManager::sBaseDir, (*files)[i], (IFile**)&f);
            Boolean result = FALSE;
            f->Delete(&result);
            if (!result) {
                String path;
                f->GetPath(&path);
                Logger::E(LOGTAG, "%s delete failed.", path.string());
            }
        }
    }
    //} catch (SecurityException e) {
    //    // Ignore SecurityExceptions.
    //}
    return NOERROR;
}


//===============================================================
//                       CCacheManager
//===============================================================
const String CCacheManager::LOGTAG("cache");
const String CCacheManager::HEADER_KEY_IFMODIFIEDSINCE("if-modified-since");
const String CCacheManager::HEADER_KEY_IFNONEMATCH("if-none-match");
AutoPtr<IFile> CCacheManager::sBaseDir;

/**
 * Initializes the HTTP cache. This method must be called before any
 * CacheManager methods are used. Note that this is called automatically
 * when a {@link WebView} is created.
 *
 * @param context the application context
 */
void CCacheManager::Init(
    /* [in] */ IContext* context)
{
    //initialize the callbacks
    Elastos_CacheManager_Init((Int32)&CCacheManager::sElaCacheManagerCallback);

    // This isn't actually where the real cache lives, but where we put files for the
    // purpose of getCacheFile().
    AutoPtr<IFile> cacheDir;
    context->GetCacheDir((IFile**)&cacheDir);
    CFile::New(cacheDir, String("webviewCacheChromiumStaging"), (IFile**)&sBaseDir);
    Boolean b = FALSE;
    if (sBaseDir->Exists(&b), !b) {
        sBaseDir->Mkdirs(&b);
    }
}

AutoPtr<IFile> CCacheManager::GetCacheFileBaseDir()
{
    return sBaseDir;
}

ECode CCacheManager::GetCacheFileBaseDir(
    /* [out] */ IFile** dir)
{
    VALIDATE_NOT_NULL(dir);
    *dir = sBaseDir;
    INTERFACE_ADDREF(*dir);
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
    *disabled = FALSE;
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
    *result = FALSE;
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
    *result = FALSE;
    return NOERROR;
}

AutoPtr<ICacheManagerCacheResult> CCacheManager::GetCacheFile(
    /* [in] */ const String& url,
    /* [in] */ IMap* headers)
{
    return GetCacheFile(url, 0, headers);
}

ECode CCacheManager::GetCacheFile(
    /* [in] */ const String& url,
    /* [in] */ IMap* headers,
    /* [out] */ ICacheManagerCacheResult** cacheResult)
{
    VALIDATE_NOT_NULL(cacheResult);
    AutoPtr<ICacheManagerCacheResult> _cacheResult = GetCacheFile(url, headers);
    *cacheResult = _cacheResult.Get();
    INTERFACE_ADDREF(*cacheResult);
    return NOERROR;
}

AutoPtr<ICacheManagerCacheResult> CCacheManager::GetCacheFile(
    /* [in] */ const String& url,
    /* [in] */ Int64 postIdentifier,
    /* [in] */ IMap* headers)
{
    AutoPtr<CacheResult> result = NativeGetCacheResult(url);
    if (result == NULL) {
        return NULL;
    }

    // A temporary local file will have been created native side and localPath set
    // appropriately.
    String localPath;
    result->GetLocalPath(&localPath);
    AutoPtr<IFile> src;
    CFile::New(sBaseDir, localPath, (IFile**)&src);
    //try {
        // Open the file here so that even if it is deleted, the content
        // is still readable by the caller until close() is called.
    AutoPtr<IFileInputStream> inStream;
    ECode ec = CFileInputStream::New(src, (IFileInputStream**)&inStream);
    if (FAILED(ec)) {
        Logger::V(LOGTAG, "getCacheFile(): Failed to open file: %s", localPath.string());
        // TODO: The files in the cache directory can be removed by the
        // system. If it is gone, what should we do?
        return NULL;
    }
    result->mInStream = IInputStream::Probe(inStream);
    //} catch (FileNotFoundException e) {
       // Log.v(LOGTAG, "getCacheFile(): Failed to open file: " + e);
        // TODO: The files in the cache directory can be removed by the
        // system. If it is gone, what should we do?
    //    return null;
    //}

    // A null value for headers is used by CACHE_MODE_CACHE_ONLY to imply
    // that we should provide the cache result even if it is expired.
    // Note that a negative expires value means a time in the far future.
    AutoPtr<ISystem> system;
    Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);
    Int64 now;
    if (headers != NULL && result->mExpires >= 0
            && result->mExpires <= (system->GetCurrentTimeMillis(&now), now)) {
        if (result->mLastModified.IsNull() && result->mEtag.IsNull()) {
            return NULL;
        }
        // Return HEADER_KEY_IFNONEMATCH or HEADER_KEY_IFMODIFIEDSINCE
        // for requesting validation.
        if (!result->mEtag.IsNull()) {
            AutoPtr<ICharSequence> key, value, oldValue;
            CStringWrapper::New(HEADER_KEY_IFNONEMATCH, (ICharSequence**)&key);
            CStringWrapper::New(result->mEtag, (ICharSequence**)&value);
            headers->Put(key, value, (IInterface**)&oldValue);
        }
        if (!result->mLastModified.IsNull()) {
            AutoPtr<ICharSequence> key, value, oldValue;
            CStringWrapper::New(HEADER_KEY_IFMODIFIEDSINCE, (ICharSequence**)&key);
            CStringWrapper::New(result->mLastModified, (ICharSequence**)&value);
            headers->Put(key, value, (IInterface**)&oldValue);
        }
    }

    if (DebugFlags::CACHE_MANAGER) {
        Logger::V(LOGTAG, "getCacheFile for url %s", url.string());
    }

    return (ICacheManagerCacheResult*)result.Get();
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

/**
 * Removes all cache files.
 *
 * @return whether the removal succeeded
 */
Boolean CCacheManager::RemoveAllCacheFiles()
{
    AutoPtr<IRunnable> clearCache = new InnerRunnable();
    AutoPtr<IThread> t;
    CThread::New(clearCache, (IThread**)&t);
    t->Start();
    return TRUE;
}

AutoPtr<CCacheManager::CacheResult> CCacheManager::NativeGetCacheResult(
    /* [in] */ const String& url)
{
    //got the object created by CreateCCacheManagerCacheResult
    Int32 obj = Elastos_CacheManager_GetCacheResult(url);
    //check if the CacheResult got
    if (0 == obj) {
        return NULL;
    }
    AutoPtr<CacheResult> cacheResult = (CacheResult*)obj;
    //handover the lifecycle management to autoptr,release it
    cacheResult->Release();//the countpart is AddRef in CreateCCacheManagerCacheResult
    return cacheResult;
}

Elastos::String CCacheManager::_GetCacheFileBaseDir()
{
    static Elastos::String baseDir;
    if (baseDir.IsNullOrEmpty()) {
        AutoPtr<IFile> fileObject = CCacheManager::GetCacheFileBaseDir();
        fileObject->GetPath(&baseDir);
    }
    return baseDir;
}
/**
  * create a new CCacheManagerCacheResult object
  * this method is a callback function which will be invoked by lib layer
  */
Int32 CCacheManager::CreateCCacheManagerCacheResult()
{
    AutoPtr<CacheResult> cacheResult = new CacheResult();
    //the object should be survived at the end of this function,see NativeGetCacheResult
    cacheResult->AddRef();//the countpart is Release in NativeGetCacheResult
    return (Int32)(cacheResult.Get());
}

void CCacheManager::SetContentdisposition(
    /* [in] */ Int32 obj,
    /* [in] */ const String& contentdisposition)
{
   CacheResult* cacheResult = (CacheResult*)obj;
   cacheResult->mContentdisposition = contentdisposition;
   return;
}

void CCacheManager::SetContentLength(
    /* [in] */ Int32 obj,
    /* [in] */ Int64 contentLength)
{
   CacheResult* cacheResult = (CacheResult*)obj;
   cacheResult->SetContentLength(contentLength);
   return;
}

void CCacheManager::SetEtag(
    /* [in] */  Int32 obj,
    /* [in] */ const String& etag)
{
   CacheResult* cacheResult = (CacheResult*)obj;
   cacheResult->mEtag = etag;
   return;
}

void CCacheManager::SetEncoding(
    /* [in] */  Int32 obj,
    /* [in] */ const String& encoding)
{
   CacheResult* cacheResult = (CacheResult*)obj;
   cacheResult->mEncoding = encoding;
   return;
}

void CCacheManager::SetExpires(
    /* [in] */ Int32 obj,
    /* [in] */ Int64 expires)
{
   CacheResult* cacheResult = (CacheResult*)obj;
   cacheResult->mExpires = expires;
   return;
}

void CCacheManager::SetExpiresString(
    /* [in] */  Int32 obj,
    /* [in] */ const String& expiresString)
{
   CacheResult* cacheResult = (CacheResult*)obj;
   cacheResult->mExpiresString = expiresString;
   return;
}

void CCacheManager::SetHttpStatusCode(
    /* [in] */ Int32 obj,
    /* [in] */ Int32 httpStatusCode)
{
   CacheResult* cacheResult = (CacheResult*)obj;
   cacheResult->mHttpStatusCode = httpStatusCode;
   return;
}

void CCacheManager::SetLastModified(
    /* [in] */ Int32 obj,
    /* [in] */ const String& lastModified)
{
   CacheResult* cacheResult = (CacheResult*)obj;
   cacheResult->mLastModified = lastModified;
   return;
}

void CCacheManager::SetLocalPath(
    /* [in] */  Int32 obj,
    /* [in] */ const String& localPath)
{
   CacheResult* cacheResult = (CacheResult*)obj;
   cacheResult->mLocalPath = localPath;
   return;
}

void CCacheManager::SetLocation(
    /* [in] */ Int32 obj,
    /* [in] */ const String& location)
{
   CacheResult* cacheResult = (CacheResult*)obj;
   cacheResult->mLocation = location;
   return;
}

void CCacheManager::SetMimeType(
    /* [in] */ Int32 obj,
    /* [in] */ const String& mimeType)
{
   CacheResult* cacheResult = (CacheResult*)obj;
   cacheResult->mMimeType = mimeType;
   return;
}

} // namespace Webkit
} // namespace Droid
} // namespace Elastos
