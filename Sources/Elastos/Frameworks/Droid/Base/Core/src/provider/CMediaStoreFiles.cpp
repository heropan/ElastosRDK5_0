
#include "provider/CMediaStoreFiles.h"
#include "ext/frameworkext.h"
#include "net/CUriHelper.h"

using Elastos::Droid::Net::IUriHelper;
using Elastos::Droid::Net::CUriHelper;

namespace Elastos {
namespace Droid {
namespace Provider {

ECode CMediaStoreFiles::GetContentUri(
    /* [in] */ const String& volumeName,
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<IUriHelper> helper;
    CUriHelper::AcquireSingleton((IUriHelper**)&helper);
    return helper->Parse(IMediaStore::CONTENT_AUTHORITY_SLASH + volumeName +
            "/file", uri);
}

ECode CMediaStoreFiles::GetContentUriEx(
    /* [in] */ const String& volumeName,
    /* [in] */ Int64 rowId,
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<IUriHelper> helper;
    CUriHelper::AcquireSingleton((IUriHelper**)&helper);
    return helper->Parse(IMediaStore::CONTENT_AUTHORITY_SLASH + volumeName
            + "/file/" + rowId, uri);
}

ECode CMediaStoreFiles::GetMtpObjectsUri(
    /* [in] */ const String& volumeName,
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<IUriHelper> helper;
    CUriHelper::AcquireSingleton((IUriHelper**)&helper);
    return helper->Parse(IMediaStore::CONTENT_AUTHORITY_SLASH + volumeName +
            "/object", uri);
}

ECode CMediaStoreFiles::GetMtpObjectsUriEx(
    /* [in] */ const String& volumeName,
    /* [in] */ Int64 fileId,
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<IUriHelper> helper;
    CUriHelper::AcquireSingleton((IUriHelper**)&helper);
    return helper->Parse(IMediaStore::CONTENT_AUTHORITY_SLASH + volumeName
            + "/object/" + fileId, uri);
}

ECode CMediaStoreFiles::GetMtpReferencesUri(
    /* [in] */ const String& volumeName,
    /* [in] */ Int64 fileId,
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<IUriHelper> helper;
    CUriHelper::AcquireSingleton((IUriHelper**)&helper);
    return helper->Parse(IMediaStore::CONTENT_AUTHORITY_SLASH + volumeName
            + "/object/" + fileId + "/references", uri);
}

} //namespace Provider
} //namespace Droid
} //namespace Elastos
