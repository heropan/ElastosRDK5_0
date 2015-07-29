
#include "provider/CMediaStoreVideoThumbnails.h"
#include "ext/frameworkext.h"
#include "net/CUriHelper.h"
#include "provider/CMediaStoreInternalThumbnails.h"

using Elastos::Droid::Net::IUriHelper;
using Elastos::Droid::Net::CUriHelper;

namespace Elastos {
namespace Droid {
namespace Provider {

ECode CMediaStoreVideoThumbnails::GetINTERNAL_CONTENT_URI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);
    return GetContentUri(String("internal"), uri);
}

ECode CMediaStoreVideoThumbnails::GetEXTERNAL_CONTENT_URI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);
    return GetContentUri(String("external"), uri);
}

ECode CMediaStoreVideoThumbnails::CancelThumbnailRequest(
    /* [in] */ IContentResolver* cr,
    /* [in] */ Int64 origId)
{
    AutoPtr<IUri> uri;
    GetEXTERNAL_CONTENT_URI((IUri**)&uri);

    AutoPtr<IMediaStoreInternalThumbnails> thumbnails;
    CMediaStoreInternalThumbnails::AcquireSingleton((IMediaStoreInternalThumbnails**)&thumbnails);
    return thumbnails->CancelThumbnailRequest(cr, origId,
            uri, IMediaStoreInternalThumbnails::DEFAULT_GROUP_ID);
}

ECode CMediaStoreVideoThumbnails::GetThumbnail(
    /* [in] */ IContentResolver* cr,
    /* [in] */ Int64 origId,
    /* [in] */ Int32 kind,
    /* [in] */ IBitmapFactoryOptions* options,
    /* [out] */ IBitmap** bitmap)
{
    AutoPtr<IUri> uri;
    GetEXTERNAL_CONTENT_URI((IUri**)&uri);

    VALIDATE_NOT_NULL(bitmap);
    AutoPtr<IMediaStoreInternalThumbnails> thumbnails;
    CMediaStoreInternalThumbnails::AcquireSingleton((IMediaStoreInternalThumbnails**)&thumbnails);
    return thumbnails->GetThumbnail(cr, origId,
            IMediaStoreInternalThumbnails::DEFAULT_GROUP_ID, kind, options,
            uri, TRUE, bitmap);
}

ECode CMediaStoreVideoThumbnails::GetThumbnail(
    /* [in] */ IContentResolver* cr,
    /* [in] */ Int64 origId,
    /* [in] */ Int64 groupId,
    /* [in] */ Int32 kind,
    /* [in] */ IBitmapFactoryOptions* options,
    /* [out] */ IBitmap** bitmap)
{
    AutoPtr<IUri> uri;
    GetEXTERNAL_CONTENT_URI((IUri**)&uri);

    VALIDATE_NOT_NULL(bitmap);
    AutoPtr<IMediaStoreInternalThumbnails> thumbnails;
    CMediaStoreInternalThumbnails::AcquireSingleton((IMediaStoreInternalThumbnails**)&thumbnails);
    return thumbnails->GetThumbnail(cr, origId, groupId, kind, options,
            uri, TRUE, bitmap);
}

ECode CMediaStoreVideoThumbnails::CancelThumbnailRequest(
    /* [in] */ IContentResolver* cr,
    /* [in] */ Int64 origId,
    /* [in] */ Int64 groupId)
{
    AutoPtr<IUri> uri;
    GetEXTERNAL_CONTENT_URI((IUri**)&uri);

    AutoPtr<IMediaStoreInternalThumbnails> thumbnails;
    CMediaStoreInternalThumbnails::AcquireSingleton((IMediaStoreInternalThumbnails**)&thumbnails);
    return thumbnails->CancelThumbnailRequest(cr, origId,
            uri, groupId);
}

ECode CMediaStoreVideoThumbnails::GetContentUri(
    /* [in] */ const String& volumeName,
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<IUriHelper> helper;
    CUriHelper::AcquireSingleton((IUriHelper**)&helper);
    return helper->Parse(IMediaStore::CONTENT_AUTHORITY_SLASH + volumeName +
            "/video/thumbnails", uri);
}

} //namespace Provider
} //namespace Droid
} //namespace Elastos
