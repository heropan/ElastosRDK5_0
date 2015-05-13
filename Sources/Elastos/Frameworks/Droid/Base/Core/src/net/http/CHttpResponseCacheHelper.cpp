
#include "CHttpResponseCacheHelper.h"
#include "CHttpResponseCache.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

ECode CHttpResponseCacheHelper::GetInstalled(
    /* [out] */ IHttpResponseCache** cache)
{
    // AutoPtr<Elastos::Net::Http::IResponseCacheHelper> helper;
    // Elastos::Net::Http::CResponseCacheHelper::AcquireSingleton(
    //     (Elastos::Net::Http::IResponseCacheHelper**)&helper);
    // AutoPtr<Elastos::Net::Http::IResponseCache> installed;
    // helper->GetDefault(&installed);

    // *cache = installed->Probe(EIID_IHttpResponseCache);
    return NOERROR;
}

ECode CHttpResponseCacheHelper::Install(
    /* [in] */ Elastos::IO::IFile* directory,
    /* [in] */ Int64 maxSize,
    /* [out] */ Elastos::Droid::Net::Http::IHttpResponseCache** cache)
{
    VALIDATE_NOT_NULL(cache);

    AutoPtr<IHttpResponseCache> installed;
    GetInstalled((IHttpResponseCache**)&installed);
    if (installed != NULL) {
    //     // don't close and reopen if an equivalent cache is already installed
    //     AutoPtr<Elastos::IO::IDiskLruCache> installedCache;
    //     installed->mDelegate->GetCache((Elastos::IO::IDiskLruCache**)&installedCache);
        AutoPtr<Elastos::IO::IFile> file;
    //     installedCache->GetDirectory((IFile**)&file)
    //     if (file->Equals(directory)
    //             && installedCache->MaxSize() == maxSize
    //             && !installedCache->IsClosed()) {
    //         *cache = installed;
    //     } else {
    //         IoUtils::CloseQuietly(installed);
    //     }
    }

    AutoPtr<IHttpResponseCache> result;
    CHttpResponseCache* cresult = new CHttpResponseCache(directory, maxSize);
    result = (IHttpResponseCache*)cresult;

    // AutoPtr<Elastos::Net::Http::IResponseCacheHelper> helper;
    // Elastos::Net::Http::CResponseCacheHelper::AcquireSingleton(
    //     (Elastos::Net::Http::IResponseCacheHelper**)&helper);
    // helper->SetDefault(result);

    *cache = result;
    INTERFACE_ADDREF(*cache);
    return NOERROR;
}

}
}
}
}

