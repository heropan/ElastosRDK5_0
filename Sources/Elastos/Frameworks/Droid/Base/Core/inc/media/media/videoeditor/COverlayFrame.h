
#ifndef __ELASTOS_DROID_MEDIA_VIDEOEDITOR_COVERLAYFRAME_H__
#define __ELASTOS_DROID_MEDIA_VIDEOEDITOR_COVERLAYFRAME_H__

#include "_COverlayFrame.h"
#include <ext/frameworkext.h>
#include "media/media/videoeditor/Overlay.h"


using Elastos::Droid::Graphics::IBitmap;
using Elastos::Droid::Graphics::IPaint;

namespace Elastos {
namespace Droid {
namespace Media {
namespace Videoeditor {

CarClass(COverlayFrame), public Overlay
{
public:
    IOVERLAY_METHODS_DECL()

    COverlayFrame();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    /**
     * Constructor for an OverlayFrame
     *
     * @param mediaItem The media item owner
     * @param overlayId The overlay id
     * @param bitmap The bitmap to be used as an overlay. The size of the
     *      bitmap must equal to the size of the media item to which it is
     *      added. The bitmap is typically a decoded PNG file.
     * @param startTimeMs The overlay start time in milliseconds
     * @param durationMs The overlay duration in milliseconds
     *
     * @throws IllegalArgumentException if the file type is not PNG or the
     *      startTimeMs and durationMs are incorrect.
     */
    CARAPI constructor(
        /* [in] */ IMediaItem* mediaItem,
        /* [in] */ const String& overlayId,
        /* [in] */ IBitmap* bitmap,
        /* [in] */ Int64 startTimeMs,
        /* [in] */ Int64 durationMs);

    /**
     * Constructor for an OverlayFrame. This constructor can be used to
     * restore the overlay after it was saved internally by the video editor.
     *
     * @param mediaItem The media item owner
     * @param overlayId The overlay id
     * @param filename The file name that contains the overlay.
     * @param startTimeMs The overlay start time in milliseconds
     * @param durationMs The overlay duration in milliseconds
     *
     * @throws IllegalArgumentException if the file type is not PNG or the
     *      startTimeMs and durationMs are incorrect.
     */
    CARAPI constructor(
        /* [in] */ IMediaItem* mediaItem,
        /* [in] */ const String& overlayId,
        /* [in] */ const String& filename,
        /* [in] */ Int64 startTimeMs,
        /* [in] */ Int64 durationMs);

    /**
     * Get the overlay bitmap.
     *
     * @return Get the overlay bitmap
     */
    CARAPI GetBitmap(
        /* [out] */ IBitmap** result);

    /**
     * Set the overlay bitmap.
     *
     * @param bitmap The overlay bitmap.
     */
    CARAPI SetBitmap(
        /* [in] */ IBitmap* bitmap);

    /**
     * Get the overlay bitmap.
     *
     * @return Get the overlay bitmap as png file.
     */
    CARAPI GetBitmapImageFileName(
        /* [out] */ String* result);

    /**
     * Get the file name of this overlay
     */
    CARAPI GetFilename(
        /* [out] */ String* result);

    /*
     * Set the file name of this overlay
     */
    CARAPI SetFilename(
        /* [in] */ const String& filename);

    /**
     * Save the overlay to the project folder
     *
     * @param path The path where the overlay will be saved
     *
     * @return The filename
     * @throws FileNotFoundException if the bitmap cannot be saved
     * @throws IOException if the bitmap file cannot be saved
     */
    CARAPI Save(
        /* [in] */ const String& path,
        /* [out] */ String* result);

    /**
     * Get the OverlayFrame Height
     */
    CARAPI GetOverlayFrameHeight(
        /* [out] */ Int32* result);

    /**
     * Get the OverlayFrame Width
     */
    CARAPI GetOverlayFrameWidth(
        /* [out] */ Int32* result);

    /*
     * Set the OverlayFrame Height
     */
    CARAPI SetOverlayFrameHeight(
        /* [in] */ Int32 height);

    /*
     * Set the OverlayFrame Width
     */
    CARAPI SetOverlayFrameWidth(
        /* [in] */ Int32 width);

    /*
     * Set the resized RGB widht and height
     */
    CARAPI SetResizedRGBSize(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    /*
     * Get the resized RGB Height
     */
    CARAPI GetResizedRGBSizeHeight(
        /* [out] */ Int32* result);

    /*
     * Get the resized RGB Width
     */
    CARAPI GetResizedRGBSizeWidth(
        /* [out] */ Int32* result);

    /**
     * Delete the overlay files
     */
    CARAPI Invalidate();

    /**
     * Delete the overlay related files
     */
    CARAPI InvalidateGeneratedFiles();

    CARAPI GenerateOverlayWithRenderingMode(
        /* [in] */ IMediaItem* mediaItemsList,
        /* [in] */ IOverlayFrame* overlay,
        /* [in] */ Int32 height,
        /* [in] */ Int32 width);

private:
    /**
     *  Instance variables
     */
    AutoPtr<IBitmap> mBitmap;
    String mFilename;
    String mBitmapFileName;

    Int32 mOFWidth;
    Int32 mOFHeight;

    /**
     * resized RGB Image dimensions
     */
    Int32 mResizedRGBWidth;
    Int32 mResizedRGBHeight;

    /**
     *  The resize paint
     */
    static AutoPtr<IPaint> sResizePaint; // = new Paint(Paint.FILTER_BITMAP_FLAG);

};

} // namespace Videoeditor
} // namespace Media
} // namepsace Droid
} // namespace Elastos
#endif // __ELASTOS_DROID_MEDIA_VIDEOEDITOR_COVERLAYFRAME_H__

