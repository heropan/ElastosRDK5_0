
#ifndef __CMEDIASTOREVIDEOTHUMBNAILS_H__
#define __CMEDIASTOREVIDEOTHUMBNAILS_H__

#include "_CMediaStoreVideoThumbnails.h"

using Elastos::Droid::Net::IUri;
using Elastos::Droid::Content::IContentResolver;
using Elastos::Droid::Graphics::IBitmap;
using Elastos::Droid::Graphics::IBitmapFactoryOptions;

namespace Elastos {
namespace Droid {
namespace Provider {

CarClass(CMediaStoreVideoThumbnails)
{
public:
    /**
     * The content:// style URI for the internal storage.
     */
    CARAPI GetINTERNALCONTENTURI(
        /* [out] */ IUri** uri);

    /**
     * The content:// style URI for the "primary" external storage
     * volume.
     */
    CARAPI GetEXTERNALCONTENTURI(
        /* [out] */ IUri** uri);

    /**
     * This method cancels the thumbnail request so clients waiting for getThumbnail will be
     * interrupted and return immediately. Only the original process which made the getThumbnail
     * requests can cancel their own requests.
     *
     * @param cr ContentResolver
     * @param origId original video id
     */
    CARAPI CancelThumbnailRequest(
        /* [in] */ IContentResolver* cr,
        /* [in] */ Int64 origId);

    /**
     * This method checks if the thumbnails of the specified image (origId) has been created.
     * It will be blocked until the thumbnails are generated.
     *
     * @param cr ContentResolver used to dispatch queries to MediaProvider.
     * @param origId Original image id associated with thumbnail of interest.
     * @param kind The type of thumbnail to fetch. Should be either MINI_KIND or MICRO_KIND.
     * @param options this is only used for MINI_KIND when decoding the Bitmap
     * @return A Bitmap instance. It could be null if the original image
     *         associated with origId doesn't exist or memory is not enough.
     */
    CARAPI GetThumbnail(
        /* [in] */ IContentResolver* cr,
        /* [in] */ Int64 origId,
        /* [in] */ Int32 kind,
        /* [in] */ IBitmapFactoryOptions* options,
        /* [out] */ IBitmap** bitmap);

    /**
     * This method checks if the thumbnails of the specified image (origId) has been created.
     * It will be blocked until the thumbnails are generated.
     *
     * @param cr ContentResolver used to dispatch queries to MediaProvider.
     * @param origId Original image id associated with thumbnail of interest.
     * @param groupId the id of group to which this request belongs
     * @param kind The type of thumbnail to fetch. Should be either MINI_KIND or MICRO_KIND
     * @param options this is only used for MINI_KIND when decoding the Bitmap
     * @return A Bitmap instance. It could be null if the original image associated with
     *         origId doesn't exist or memory is not enough.
     */
    CARAPI GetThumbnailEx(
        /* [in] */ IContentResolver* cr,
        /* [in] */ Int64 origId,
        /* [in] */ Int64 groupId,
        /* [in] */ Int32 kind,
        /* [in] */ IBitmapFactoryOptions* options,
        /* [out] */ IBitmap** bitmap);

    /**
     * This method cancels the thumbnail request so clients waiting for getThumbnail will be
     * interrupted and return immediately. Only the original process which made the getThumbnail
     * requests can cancel their own requests.
     *
     * @param cr ContentResolver
     * @param origId original video id
     * @param groupId the same groupId used in getThumbnail.
     */
    CARAPI CancelThumbnailRequestEx(
        /* [in] */ IContentResolver* cr,
        /* [in] */ Int64 origId,
        /* [in] */ Int64 groupId);

    /**
     * Get the content:// style URI for the image media table on the
     * given volume.
     *
     * @param volumeName the name of the volume to get the URI for
     * @return the URI to the image media table on the given volume
     */
    CARAPI GetContentUri(
        /* [in] */ const String& volumeName,
        /* [out] */ IUri** uri);
};

}
}
}

#endif //__CMEDIASTOREVIDEOTHUMBNAILS_H__