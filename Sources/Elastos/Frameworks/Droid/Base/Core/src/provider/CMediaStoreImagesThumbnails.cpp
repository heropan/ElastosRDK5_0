
#include "provider/CMediaStoreImagesThumbnails.h"
#include "ext/frameworkext.h"
#include "net/CUriHelper.h"
#include "provider/CMediaStoreInternalThumbnails.h"

using Elastos::Droid::Net::IUriHelper;
using Elastos::Droid::Net::CUriHelper;

namespace Elastos {
namespace Droid {
namespace Provider {

ECode CMediaStoreImagesThumbnails::GetINTERNAL_CONTENT_URI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);
    return GetContentUri(String("internal"), uri);
}

ECode CMediaStoreImagesThumbnails::GetEXTERNAL_CONTENT_URI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);
    return GetContentUri(String("external"), uri);
}

ECode CMediaStoreImagesThumbnails::Query(
    /* [in] */ IContentResolver* cr,
    /* [in] */ IUri* uri,
    /* [in] */ ArrayOf<String>* projection,
    /* [out] */ ICursor** cursor)
{
    VALIDATE_NOT_NULL(cursor);
    return cr->Query(uri, projection, String(NULL), NULL,
               IMediaStoreImagesThumbnails::DEFAULT_SORT_ORDER, cursor);
}

ECode CMediaStoreImagesThumbnails::QueryMiniThumbnails(
    /* [in] */ IContentResolver* cr,
    /* [in] */ IUri* uri,
    /* [in] */ Int32 kind,
    /* [in] */ ArrayOf<String>* projection,
    /* [out] */ ICursor** cursor)
{
    VALIDATE_NOT_NULL(cursor);
    return cr->Query(uri, projection, String("kind = ") + kind,
            NULL, IMediaStoreImagesThumbnails::DEFAULT_SORT_ORDER, cursor);
}

ECode CMediaStoreImagesThumbnails::QueryMiniThumbnail(
    /* [in] */ IContentResolver* cr,
    /* [in] */ Int64 origId,
    /* [in] */ Int32 kind,
    /* [in] */ ArrayOf<String>* projection,
    /* [out] */ ICursor** cursor)
{
    VALIDATE_NOT_NULL(cursor);
    AutoPtr<IUri> uri;
    GetEXTERNAL_CONTENT_URI((IUri**)&uri);
    return cr->Query(uri, projection,
            IMediaStoreImagesThumbnails::IMAGE_ID + " = " + origId + " AND " + IMediaStoreImagesThumbnails::KIND + " = " +
            kind, NULL, String(NULL), cursor);
}

ECode CMediaStoreImagesThumbnails::CancelThumbnailRequest(
    /* [in] */ IContentResolver* cr,
    /* [in] */ Int64 origId)
{
    AutoPtr<IUri> uri;
    GetEXTERNAL_CONTENT_URI((IUri**)&uri);

    AutoPtr<IMediaStoreInternalThumbnails> thumbnails;
    CMediaStoreInternalThumbnails::AcquireSingleton((IMediaStoreInternalThumbnails**)&thumbnails);
    return thumbnails->CancelThumbnailRequest(cr, origId, uri,
        IMediaStoreInternalThumbnails::DEFAULT_GROUP_ID);
}

ECode CMediaStoreImagesThumbnails::GetThumbnail(
    /* [in] */ IContentResolver* cr,
    /* [in] */ Int64 origId,
    /* [in] */ Int32 kind,
    /* [in] */ IBitmapFactoryOptions* options,
    /* [out] */ IBitmap** bitmap)
{
    AutoPtr<IUri> uri;
    GetEXTERNAL_CONTENT_URI((IUri**)&uri);

    AutoPtr<IMediaStoreInternalThumbnails> thumbnails;
    CMediaStoreInternalThumbnails::AcquireSingleton((IMediaStoreInternalThumbnails**)&thumbnails);
    return thumbnails->GetThumbnail(cr, origId,
        IMediaStoreInternalThumbnails::DEFAULT_GROUP_ID, kind, options,
            uri, FALSE, bitmap);
}

ECode CMediaStoreImagesThumbnails::CancelThumbnailRequest(
    /* [in] */ IContentResolver* cr,
    /* [in] */ Int64 origId,
    /* [in] */ Int64 groupId)
{
    AutoPtr<IUri> uri;
    GetEXTERNAL_CONTENT_URI((IUri**)&uri);

    AutoPtr<IMediaStoreInternalThumbnails> thumbnails;
    CMediaStoreInternalThumbnails::AcquireSingleton((IMediaStoreInternalThumbnails**)&thumbnails);
    return thumbnails->CancelThumbnailRequest(cr, origId, uri, groupId);
}

ECode CMediaStoreImagesThumbnails::GetThumbnail(
    /* [in] */ IContentResolver* cr,
    /* [in] */ Int64 origId,
    /* [in] */ Int64 groupId,
    /* [in] */ Int32 kind,
    /* [in] */ IBitmapFactoryOptions* options,
    /* [out] */ IBitmap** bitmap)
{
    AutoPtr<IUri> uri;
    GetEXTERNAL_CONTENT_URI((IUri**)&uri);

    AutoPtr<IMediaStoreInternalThumbnails> thumbnails;
    CMediaStoreInternalThumbnails::AcquireSingleton((IMediaStoreInternalThumbnails**)&thumbnails);
    return thumbnails->GetThumbnail(cr, origId, groupId, kind, options,
            uri, FALSE, bitmap);
}

ECode CMediaStoreImagesThumbnails::GetContentUri(
    /* [in] */ const String& volumeName,
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<IUriHelper> helper;
    CUriHelper::AcquireSingleton((IUriHelper**)&helper);
    return helper->Parse(IMediaStore::CONTENT_AUTHORITY_SLASH + volumeName +
            "/images/thumbnails", uri);
}


} //namespace Provider
} //namespace Droid
} //namespace Elastos
