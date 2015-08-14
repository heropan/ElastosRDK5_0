
#ifndef __ELASTOS_DROID_WEBKIT_CONTENT_BROWSER_CONTENTREADBACKHANDLER_H__
#define __ELASTOS_DROID_WEBKIT_CONTENT_BROWSER_CONTENTREADBACKHANDLER_H__

// import android.graphics.Bitmap;
// import android.graphics.Rect;
// import android.util.SparseArray;

// import org.chromium.base.CalledByNative;
// import org.chromium.base.JNINamespace;
// import org.chromium.base.ThreadUtils;
// import org.chromium.ui.base.WindowAndroid;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {

/**
 * A class for reading back content.
 */
//@JNINamespace("content")
class ContentReadbackHandler
{
public:
    /**
     * A callback interface for content readback into a bitmap.
     */
    class GetBitmapCallback
    {
    public:
        /**
         * Called when the content readback finishes.
         * @param success    Indicates whether the readback succeeded or not.
         * @param bitmap     The {@link Bitmap} of the content.
         */
        virtual CARAPI_(void) OnFinishGetBitmap(
            /* [in] */ Boolean success, Bitmap bitmap) = 0;
    };

public:
    /**
     * Creates a {@link ContentReadbackHandler}.
     */
    ContentReadbackHandler();

    /**
     * Initialize the native object.
     */
    virtual CARAPI_(void) InitNativeContentReadbackHandler();

    /**
     * Should be called when the ContentReadackHandler is not needed anymore.
     */
    virtual CARAPI_(void) Destroy();

    /**
     * Asynchronously, generate and grab a bitmap representing what is currently on the screen
     * for {@code view}.
     *
     * @param scale The scale that should be applied to the content.
     * @param srcRect  A subrect of the original content to capture. If this is empty, it will grab
     *                 the whole surface.
     * @param view  The {@link ContentViewCore} to grab the bitmap from.
     * @param callback The callback to be executed after readback completes.
     */
    virtual CARAPI_(void) GetContentBitmapAsync(
        /* [in] */ Float scale,
        /* [in] */ IRect* srcRect,
        /* [in] */ ContentViewCore* view,
        /* [in] */ GetBitmapCallback* callback);

    /**
     * Asynchronously, grab a bitmap of the current browser compositor root layer.
     *
     * @param windowAndroid The window that hosts the compositor.
     * @param callback      The callback to be executed after readback completes.
     */
    virtual CARAPI_(void) GetCompositorBitmapAsync(
        /* [in] */ WindowAndroid* windowAndroid,
        /* [in] */ GetBitmapCallback* callback);

protected:
    /**
     * Implemented by the owner of this class to signal whether readback is possible or not.
     * @return Whether readback is possible or not.
     */
    virtual CARAPI_(Boolean) ReadyForReadback() = 0;

private:
    //@CalledByNative
    CARAPI_(void) NotifyGetBitmapFinished(
        /* [in] */ Int32 readbackId,
        /* [in] */ Boolean success,
        /* [in] */ IBitmap* bitmap);

    CARAPI_(Int64) NativeInit();

    CARAPI_(void) NativeDestroy(
        /* [in] */ Int64 nativeContentReadbackHandler);

    CARAPI_(void) NativeGetContentBitmap(
        /* [in] */ Int64 nativeContentReadbackHandler,
        /* [in] */ Int32 readback_id,
        /* [in] */ Float scale,
        /* [in] */ BitmapConfig config,
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ Float width,
        /* [in] */ Float height,
        /* [in] */ IInterface* contentViewCore);

    CARAPI_(void) NativeGetCompositorBitmap(
        /* [in] */ Int64 nativeContentReadbackHandler,
        /* [in] */ Int32 readback_id,
        /* [in] */ Int64 nativeWindowAndroid);

private:
    Int32 mNextReadbackId = 1;
    SparseArray<GetBitmapCallback> mGetBitmapRequests;

    Int64 mNativeContentReadbackHandler;
};

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__ELASTOS_DROID_WEBKIT_CONTENT_BROWSER_CONTENTREADBACKHANDLER_H__
