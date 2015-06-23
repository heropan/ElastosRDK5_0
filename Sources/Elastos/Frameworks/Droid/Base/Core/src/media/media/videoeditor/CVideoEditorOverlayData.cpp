
#include "CVideoEditorOverlayData.h"
#include "graphics/CPaint.h"
#include "graphics/CCanvas.h"
#include "graphics/CRect.h"

using Elastos::Droid::Graphics::CPaint;
using Elastos::Droid::Graphics::ICanvas;
using Elastos::Droid::Graphics::CCanvas;
using Elastos::Droid::Graphics::IRect;
using Elastos::Droid::Graphics::CRect;
using Elastos::Droid::Graphics::IColor;

namespace Elastos {
namespace Droid {
namespace Media {
namespace Videoeditor {

AutoPtr<IPaint> CVideoEditorOverlayData::sResizePaint;

CVideoEditorOverlayData::CVideoEditorOverlayData()
    : mRenderingMode(0)
    , mClear(FALSE)
{
    CPaint::New(IPaint::FILTER_BITMAP_FLAG, (IPaint**)&sResizePaint);
}

/**
 * Default constructor
 */
ECode CVideoEditorOverlayData::constructor()
{
    mOverlayBitmap = NULL;
    mRenderingMode = IMediaArtistNativeHelperMediaRendering::BLACK_BORDERS;
    mClear = FALSE;
    return NOERROR;
}

/**
 * Releases the bitmap
 */
ECode CVideoEditorOverlayData::ReleaseResources()
{
    if (mOverlayBitmap != NULL) {
        mOverlayBitmap->Recycle();
        mOverlayBitmap = NULL;
    }
    return NOERROR;
}

/**
 * Check if the overlay needs to be rendered
 *
 * @return true if rendering is needed
 */
ECode CVideoEditorOverlayData::NeedsRendering(
    /* [out] */ Boolean* result)
{
    *result = (mClear || mOverlayBitmap != NULL);
    return NOERROR;
}

/**
* Render the overlay by either clearing it or by
* rendering the overlay bitmap with the specified
* rendering mode
*
* @param destBitmap The destination bitmap
*/
ECode CVideoEditorOverlayData::RenderOverlay(
    /* [in] */ IBitmap* destBitmap)
{
    if (mClear) {
        destBitmap->EraseColor(IColor::TRANSPARENT);
    } else if (mOverlayBitmap != NULL) {
        AutoPtr<ICanvas> overlayCanvas;
        CCanvas::New(destBitmap, (ICanvas**)&overlayCanvas);
        AutoPtr<IRect> destRect;
        AutoPtr<IRect> srcRect;
        Int32 canvasWidth, canvasHeight;
        Int32 bitmapWidth, bitmapHeight;
        overlayCanvas->GetWidth(&canvasWidth);
        overlayCanvas->GetHeight(&canvasHeight);
        mOverlayBitmap->GetWidth(&bitmapWidth);
        mOverlayBitmap->GetHeight(&bitmapHeight);
        switch (mRenderingMode) {
            case IMediaArtistNativeHelperMediaRendering::RESIZING: {
                CRect::New(0, 0, canvasWidth, canvasHeight, (IRect**)&destRect);
                CRect::New(0, 0, bitmapWidth, bitmapHeight, (IRect**)&srcRect);
                break;
            }

            case IMediaArtistNativeHelperMediaRendering::BLACK_BORDERS: {
                Int32 left, right, top, bottom;
                Float aROverlayImage, aRCanvas;
                aROverlayImage = (Float)(bitmapWidth) /
                                 (Float)(bitmapHeight);

                aRCanvas = (Float)(canvasWidth) /
                                 (Float)(canvasHeight);

                if (aROverlayImage > aRCanvas) {
                    Int32 newHeight = ((canvasWidth * bitmapHeight)
                                     / bitmapWidth);
                    left = 0;
                    top  = (canvasHeight - newHeight) / 2;
                    right = canvasWidth;
                    bottom = top + newHeight;
                } else {
                    Int32 newWidth = ((canvasHeight * bitmapWidth)
                                        / bitmapHeight);
                    left = (canvasWidth - newWidth) / 2;
                    top  = 0;
                    right = left + newWidth;
                    bottom = canvasHeight;
                }

                CRect::New(left, top, right, bottom, (IRect**)&destRect);
                CRect::New(0, 0, bitmapWidth, bitmapHeight, (IRect**)&srcRect);
                break;
            }

            case IMediaArtistNativeHelperMediaRendering::CROPPING: {
                // Calculate the source rect
                Int32 left, right, top, bottom;
                Float aROverlayImage, aRCanvas;
                aROverlayImage = (Float)(bitmapWidth) /
                                 (Float)(bitmapHeight);
                aRCanvas = (Float)(canvasWidth) /
                                (Float)(canvasHeight);
                if (aROverlayImage < aRCanvas) {
                    Int32 newHeight = ((bitmapWidth * canvasHeight)
                               / canvasWidth);

                    left = 0;
                    top  = (bitmapHeight - newHeight) / 2;
                    right = bitmapWidth;
                    bottom = top + newHeight;
                } else {
                    Int32 newWidth = ((bitmapHeight * canvasWidth)
                                / canvasHeight);
                    left = (bitmapWidth - newWidth) / 2;
                    top  = 0;
                    right = left + newWidth;
                    bottom = bitmapHeight;
                }

                CRect::New(left, top, right, bottom, (IRect**)&srcRect);
                CRect::New(0, 0, canvasWidth, canvasHeight, (IRect**)&destRect);
                break;
            }

            default: {
                // throw new IllegalStateException("Rendering mode: " + mRenderingMode);
                return E_ILLEGAL_STATE_EXCEPTION;
            }
        }

        destBitmap->EraseColor(IColor::TRANSPARENT);
        overlayCanvas->DrawBitmap(mOverlayBitmap, srcRect, destRect, sResizePaint);

        mOverlayBitmap->Recycle();
    }
    return NOERROR;
}

/**
 * Store the overlay data
 *
 * @param overlayBitmap The overlay bitmap
 * @param renderingMode The rendering mode
 */
ECode CVideoEditorOverlayData::Set(
    /* [in] */ IBitmap* overlayBitmap,
    /* [in] */ Int32 renderingMode)
{
    mOverlayBitmap = overlayBitmap;
    mRenderingMode = renderingMode;
    mClear = FALSE;
    return NOERROR;
}

/**
 * Clear the overlay
 */
ECode CVideoEditorOverlayData::SetClear()
{
    mClear = TRUE;
    return NOERROR;
}

} // namespace Videoeditor
} // namespace Media
} // namepsace Droid
} // namespace Elastos
