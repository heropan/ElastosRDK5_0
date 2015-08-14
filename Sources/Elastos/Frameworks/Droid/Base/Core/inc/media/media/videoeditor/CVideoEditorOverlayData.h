
#ifndef __ELASTOS_DROID_MEDIA_VIDEOEDITOR_CVIDEOEDITOROVERLAYDATA_H__
#define __ELASTOS_DROID_MEDIA_VIDEOEDITOR_CVIDEOEDITOROVERLAYDATA_H__

#include "_CVideoEditorOverlayData.h"
#include <ext/frameworkext.h>

using Elastos::Droid::Graphics::IBitmap;
using Elastos::Droid::Graphics::IPaint;

namespace Elastos {
namespace Droid {
namespace Media {
namespace Videoeditor {

/**
 * This is the super class for all effects. An effect can only be applied to a
 * single media item.
 * {@hide}
 */
CarClass(CVideoEditorOverlayData)
{
public:
    CVideoEditorOverlayData();

    /**
     * Default constructor
     */
    CARAPI constructor();

    /**
     * Releases the bitmap
     */
    CARAPI ReleaseResources();

    /**
     * Check if the overlay needs to be rendered
     *
     * @return true if rendering is needed
     */
    CARAPI NeedsRendering(
        /* [out] */ Boolean* result);

    /**
    * Render the overlay by either clearing it or by
    * rendering the overlay bitmap with the specified
    * rendering mode
    *
    * @param destBitmap The destination bitmap
    */
    CARAPI RenderOverlay(
        /* [in] */ IBitmap* destBitmap);

    /**
     * Store the overlay data
     *
     * @param overlayBitmap The overlay bitmap
     * @param renderingMode The rendering mode
     */
    CARAPI Set(
        /* [in] */ IBitmap* overlayBitmap,
        /* [in] */ Int32 renderingMode);

    /**
     * Clear the overlay
     */
    CARAPI SetClear();

private:
    // Instance variables
    AutoPtr<IBitmap> mOverlayBitmap;
    Int32 mRenderingMode;
    Boolean mClear;

    static AutoPtr<IPaint> sResizePaint;    // = new Paint(Paint.FILTER_BITMAP_FLAG);

};

} // namespace Videoeditor
} // namespace Media
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_MEDIA_VIDEOEDITOR_CVIDEOEDITOROVERLAYDATA_H__

