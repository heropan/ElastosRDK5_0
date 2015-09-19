
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

const String JavaBrowserViewRendererHelper::LOGTAG("JavaBrowserViewRendererHelper");

// Should never be instantiated.
JavaBrowserViewRendererHelper::JavaBrowserViewRendererHelper()
{
}

/**
 * Provides a Bitmap object with a given width and height used for auxiliary rasterization.
 * |canvas| is optional and if supplied indicates the Canvas that this Bitmap will be
 * drawn into. Note the Canvas will not be modified in any way.
 */
//@CalledByNative
AutoPtr<IBitmap> JavaBrowserViewRendererHelper::CreateBitmap(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ ICanvas* canvas)
{
    if (canvas != NULL) {
        // When drawing into a Canvas, there is a maximum size imposed
        // on Bitmaps that can be drawn. Respect that limit.
        Int32 w, h;
        canvas->GetMaximumBitmapWidth(&w);
        canvas->GetMaximumBitmapHeight(&h);
        width = Math::Min(width, w);
        height = Math::Min(height, h);
    }

    AutoPtr<IBitmap> bitmap;
    // try {
        bitmap = Bitmap.createBitmap(width, height, Bitmap.Config.ARGB_8888);
    // } catch (OutOfMemoryError e) {
    //     android.util.Log.w(LOGTAG, "Error allocating bitmap");
    // }

    return bitmap;
}

/**
 * Draws a provided bitmap into a canvas.
 * Used for convenience from the native side and other static helper methods.
 */
//@CalledByNative
void JavaBrowserViewRendererHelper::DrawBitmapIntoCanvas(
    /* [in] */ IBitmap* bitmap,
    /* [in] */ ICanvas* canvas,
    /* [in] */ Int32 scroll_x,
    /* [in] */ Int32 scroll_y)
{
    canvas->Translate(scroll_x, scroll_y);
    canvas->DrawBitmap(bitmap, 0, 0, NULL);
}

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos