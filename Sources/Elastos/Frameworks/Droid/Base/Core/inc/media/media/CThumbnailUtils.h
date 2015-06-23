
#ifndef __CTHUMBNAILUTILS_H__
#define __CTHUMBNAILUTILS_H__

#include "_CThumbnailUtils.h"
#include "ext/frameworkext.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Graphics::IBitmap;

namespace Elastos {
namespace Droid {
namespace Media {

CarClass(CThumbnailUtils)
{
public:
    /**
     * This method first examines if the thumbnail embedded in EXIF is bigger than our target
     * size. If not, then it'll create a thumbnail from original image. Due to efficiency
     * consideration, we want to let MediaThumbRequest avoid calling this method twice for
     * both kinds, so it only requests for MICRO_KIND and set saveImage to true.
     *
     * This method always returns a "square thumbnail" for MICRO_KIND thumbnail.
     *
     * @param filePath the path of image file
     * @param kind could be MINI_KIND or MICRO_KIND
     * @return Bitmap, or null on failures
     *
     * @hide This method is only used by media framework and media provider internally.
     */
    CARAPI CreateImageThumbnail(
        /* [in] */ const String& filePath,
        /* [in] */ Int32 kind,
        /* [out] */ IBitmap** result);

    /**
     * Create a video thumbnail for a video. May return null if the video is
     * corrupt or the format is not supported.
     *
     * @param filePath the path of video file
     * @param kind could be MINI_KIND or MICRO_KIND
     */
    CARAPI CreateVideoThumbnail(
        /* [in] */ const String& filePath,
        /* [in] */ Int32 kind,
        /* [out] */ IBitmap** result);

    /**
     * Creates a centered bitmap of the desired size.
     *
     * @param source original bitmap source
     * @param width targeted width
     * @param height targeted height
     */
    CARAPI ExtractThumbnail(
        /* [in] */ IBitmap* source,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [out] */ IBitmap** result);

    /**
     * Creates a centered bitmap of the desired size.
     *
     * @param source original bitmap source
     * @param width targeted width
     * @param height targeted height
     * @param options options used during thumbnail extraction
     */
    CARAPI ExtractThumbnail(
        /* [in] */ IBitmap* source,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int32 options,
        /* [out] */ IBitmap** result);
};

} // namespace Media
} // namepsace Droid
} // namespace Elastos

#endif // __CTHUMBNAILUTILS_H__
