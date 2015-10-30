
#ifndef __ELASTOS_DROID_WEBKIT_CCACHEMANAGER_H__
#define __ELASTOS_DROID_WEBKIT_CCACHEMANAGER_H__

#include "elastos/droid/ext/frameworkext.h"
#include "_Elastos_Droid_Webkit_CCacheManager.h"
#include "elastos/droid/os/Runnable.h"

using Elastos::IO::IFile;
using Elastos::IO::IInputStream;
using Elastos::IO::IOutputStream;
using Elastos::Utility::IMap;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Net::Http::IHeaders;
using Elastos::Droid::Os::Runnable;

#ifdef __cplusplus
extern "C"
{
#endif
    //must be called first
    extern void Elastos_CacheManager_Init(Int32 callbacks);
    extern Int32 Elastos_CacheManager_GetCacheResult(const Elastos::String& url);
#ifdef __cplusplus
}
#endif

namespace Elastos {
namespace Droid {
namespace Webkit {

struct ElaCacheManagerCallback
{
    Int32 (*createCCacheManagerCacheResult)();
    Elastos::String (*getCacheFileBaseDir)();
    void (*setContentdisposition)(Int32 obj, const Elastos::String& contentdisposition);
    void (*setContentLength)(Int32 obj, Int64 contentLength);
    void (*setEtag)(Int32 obj, const Elastos::String& etag);
    void (*setEncoding)(Int32 obj, const Elastos::String& encoding);
    void (*setExpires)(Int32 obj, Int64 expires);
    void (*setExpiresString)(Int32 obj, const Elastos::String& expiresString);
    void (*setHttpStatusCode)(Int32 obj, Int32 httpStatusCode);
    void (*setLastModified)(Int32 obj, const Elastos::String& lastModified);
    void (*setLocalPath)(Int32 obj, const Elastos::String& localPath);
    void (*setLocation)(Int32 obj, const Elastos::String& location);
    void (*setMimeType)(Int32 obj, const Elastos::String& mimeType);
};

CarClass(CCacheManager)
    , public Object
    , public ICacheManager
{
public:
    class CacheResult
        : public Object
        , public ICacheManagerCacheResult
    {
    public:
        CacheResult()
            : mHttpStatusCode(0)
            , mContentLength(0)
            , mExpires(0)
        {}

        CAR_INTERFACE_DECL()

        CARAPI GetHttpStatusCode(
            /* [out] */ Int32* statusCode);

        CARAPI GetContentLength(
            /* [out] */ Int64* length);

        CARAPI GetLocalPath(
            /* [out] */ String* path);

        CARAPI GetExpires(
            /* [out] */ Int64* expires);

        CARAPI GetExpiresString(
            /* [out] */ String* expires);

        CARAPI GetLastModified(
            /* [out] */ String* lastModified);

        CARAPI GetETag(
            /* [out] */ String* etag);

        CARAPI GetMimeType(
            /* [out] */ String* mimeType);

        CARAPI GetLocation(
            /* [out] */ String* location);

        CARAPI GetEncoding(
            /* [out] */ String* encoding);

        CARAPI GetContentDisposition(
            /* [out] */ String* contentdisposition);

        CARAPI GetInputStream(
            /* [out] */ IInputStream** inStream);

        CARAPI GetOutputStream(
            /* [out] */ IOutputStream** outStream);

        CARAPI SetInputStream(
            /* [in] */ IInputStream* stream);

        CARAPI SetEncoding(
            /* [in] */ const String& encoding);

        CARAPI SetContentLength(
            /* [in] */ Int64 contentLength);

    public:
        // these fields are saved to the database
        Int32 mHttpStatusCode;
        Int64 mContentLength;
        Int64 mExpires;
        String mExpiresString;
        String mLocalPath;
        String mLastModified;
        String mEtag;
        String mMimeType;
        String mLocation;
        String mEncoding;
        String mContentdisposition;
        String mCrossDomain;

        // these fields are NOT saved to the database
        AutoPtr<IInputStream> mInStream;
        AutoPtr<IOutputStream> mOutStream;
        AutoPtr<IFile> mOutFile;
    };

private:
    class InnerRunnable
        : public Runnable
    {
    public:
        InnerRunnable() {}

        CARAPI Run();
    };

public:
    CAR_OBJECT_DECL()

    CAR_INTERFACE_DECL()

    /**
     * Initializes the HTTP cache. This method must be called before any
     * CacheManager methods are used. Note that this is called automatically
     * when a {@link WebView} is created.
     *
     * @param context the application context
     */
    static CARAPI_(void) Init(
        /* [in] */ IContext* context);

    static CARAPI_(AutoPtr<IFile>) GetCacheFileBaseDir();

    CARAPI GetCacheFileBaseDir(
        /* [out] */ IFile** dir);

    static CARAPI_(Boolean) CacheDisabled();

    CARAPI CacheDisabled(
        /* [out] */ Boolean* disabled);

    static CARAPI_(Boolean) StartCacheTransaction();

    CARAPI StartCacheTransaction(
        /* [out] */ Boolean* result);

    static CARAPI_(Boolean) EndCacheTransaction();

    CARAPI EndCacheTransaction(
        /* [out] */ Boolean* result);

    static CARAPI_(AutoPtr<ICacheManagerCacheResult>) GetCacheFile(
        /* [in] */ const String& url,
        /* [in] */ IMap* headers);

    CARAPI GetCacheFile(
        /* [in] */ const String& url,
        /* [in] */ IMap* headers,
        /* [out] */ ICacheManagerCacheResult** cacheFile);

    static CARAPI_(AutoPtr<ICacheManagerCacheResult>) GetCacheFile(
        /* [in] */ const String& url,
        /* [in] */ Int64 postIdentifier,
        /* [in] */ IMap* headers);

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
    static CARAPI_(AutoPtr<ICacheManagerCacheResult>) CreateCacheFile(
        /* [in] */ const String& url,
        /* [in] */ Int32 statusCode,
        /* [in] */ IHeaders* headers,
        /* [in] */ const String& mimeType,
        /* [in] */ Boolean forceCache);

    static CARAPI_(void) _SaveCacheFile(
        /* [in] */ const String& url,
        /* [in] */ ICacheManagerCacheResult* cacheResult);

    CARAPI SaveCacheFile(
        /* [in] */ const String& url,
        /* [in] */ ICacheManagerCacheResult* cacheResult);

    static CARAPI_(void) SaveCacheFile(
        /* [in] */ const String& url,
        /* [in] */ Int64 postIdentifier,
        /* [in] */ ICacheManagerCacheResult* cacheRet);

    /**
     * Removes all cache files.
     *
     * @return whether the removal succeeded
     */
    static CARAPI_(Boolean) RemoveAllCacheFiles();

private:
    static CARAPI_(AutoPtr<CacheResult>) NativeGetCacheResult(
        /* [in] */ const String& url);

    static CARAPI_(Int32) CreateCCacheManagerCacheResult();

    static CARAPI_(String) _GetCacheFileBaseDir();

    static CARAPI_(void) SetContentdisposition(
        /* [in] */ Int32 obj,
        /* [in] */ const Elastos::String& contentdisposition);

    static CARAPI_(void) SetContentLength(
        /* [in] */ Int32 obj,
        /* [in] */ Int64 contentLength);

    static CARAPI_(void) SetEtag(
        /* [in] */ Int32 obj,
        /* [in] */ const Elastos::String& etag);

    static CARAPI_(void) SetEncoding(
        /* [in] */ Int32 obj,
        /* [in] */ const Elastos::String& encoding);

    static CARAPI_(void) SetExpires(
        /* [in] */ Int32 obj,
        /* [in] */ Int64 expires);

    static CARAPI_(void) SetExpiresString(
        /* [in] */ Int32 obj,
        /* [in] */ const Elastos::String& expiresString);

    static CARAPI_(void) SetHttpStatusCode(
        /* [in] */ Int32 obj,
        /* [in] */ Int32 httpStatusCode);

    static CARAPI_(void) SetLastModified(
        /* [in] */ Int32 obj,
        /* [in] */ const Elastos::String& lastModified);

    static CARAPI_(void) SetLocalPath(
        /* [in] */ Int32 obj,
        /* [in] */ const Elastos::String& localPath);

    static CARAPI_(void) SetLocation(
        /* [in] */ Int32 obj,
        /* [in] */ const Elastos::String& location);

    static CARAPI_(void) SetMimeType(
        /* [in] */ Int32 obj,
        /* [in] */ const Elastos::String& mimeType);

public:
    static const String HEADER_KEY_IFMODIFIEDSINCE;
    static const String HEADER_KEY_IFNONEMATCH;
    static struct ElaCacheManagerCallback sElaCacheManagerCallback;

private:
    static const String LOGTAG;
    static AutoPtr<IFile> sBaseDir;
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // __CCACHEMANAGERHELPER_H__
