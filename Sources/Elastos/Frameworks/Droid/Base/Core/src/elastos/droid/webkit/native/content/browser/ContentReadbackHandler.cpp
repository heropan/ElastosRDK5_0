
#include "webkit/native/content/browser/ContentReadbackHandler.h"
#include "webkit/native/base/ThreadUtils.h"

using Elastos::Droid::Webkit::Base::ThreadUtils;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {

/**
 * Creates a {@link ContentReadbackHandler}.
 */
ContentReadbackHandler::ContentReadbackHandler()
    : mNextReadbackId(1)
    , mNativeContentReadbackHandler(0)
{
    assert(0);
//    mGetBitmapRequests = new SparseArray<GetBitmapCallback>();
}

/**
 * Initialize the native object.
 */
void ContentReadbackHandler::InitNativeContentReadbackHandler()
{
    mNativeContentReadbackHandler = NativeInit();
}

/**
 * Should be called when the ContentReadackHandler is not needed anymore.
 */
void ContentReadbackHandler::Destroy()
{
    if (mNativeContentReadbackHandler != 0) {
        NativeDestroy(mNativeContentReadbackHandler);
    }
    mNativeContentReadbackHandler = 0;
}


//@CalledByNative
void ContentReadbackHandler::NotifyGetBitmapFinished(
    /* [in] */ Int32 readbackId,
    /* [in] */ Boolean success,
    /* [in] */ IBitmap* bitmap)
{
    assert(0);
#if 0
    AutoPtr<GetBitmapCallback> callback = mGetBitmapRequests->Get(readbackId);
    if (callback != NULL) {
        mGetBitmapRequests->Delete(readbackId);
        callback->OnFinishGetBitmap(success, bitmap);
    }
    else {
        // readback Id is unregistered.
        assert false : "Readback finished for unregistered Id: " + readbackId;
    }
#endif
}

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
void ContentReadbackHandler::GetContentBitmapAsync(
    /* [in] */ Float scale,
    /* [in] */ IRect* srcRect,
    /* [in] */ ContentViewCore* view,
    /* [in] */ GetBitmapCallback* callback)
{
    assert(0);
#if 0
    if (!ReadyForReadback()) {
        callback->OnFinishGetBitmap(FALSE, NULL);
        return;
    }

    ThreadUtils::AssertOnUiThread();

    Int32 readbackId = mNextReadbackId++;
    mGetBitmapRequests->Put(readbackId, callback);
    Int32 width, height, top, left;
    srcRect->GetWidth(&width);
    srcRect->GetHeight(&height);
    srcRect->GetTop(&top);
    srcRect->GetLeft(&left);
    NativeGetContentBitmap(mNativeContentReadbackHandler, readbackId, scale,
            Bitmap.Config.ARGB_8888, top, left, width,
            height, view);
#endif
}

/**
 * Asynchronously, grab a bitmap of the current browser compositor root layer.
 *
 * @param windowAndroid The window that hosts the compositor.
 * @param callback      The callback to be executed after readback completes.
 */
// void ContentReadbackHandler::GetCompositorBitmapAsync(
//     /* [in] */ WindowAndroid* windowAndroid,
//     /* [in] */ GetBitmapCallback* callback)
// {
//     if (!ReadyForReadback()) {
//         callback->OnFinishGetBitmap(FALSE, NULL);
//         return;
//     }

//     ThreadUtils::AssertOnUiThread();

//     Int32 readbackId = mNextReadbackId++;
//     mGetBitmapRequests->Put(readbackId, callback);
//     NativeGetCompositorBitmap(mNativeContentReadbackHandler, readbackId,
//             windowAndroid->GetNativePointer());
// }

Int64 ContentReadbackHandler::NativeInit()
{
}

void ContentReadbackHandler::NativeDestroy(
    /* [in] */ Int64 nativeContentReadbackHandler)
{
}

void ContentReadbackHandler::NativeGetContentBitmap(
    /* [in] */ Int64 nativeContentReadbackHandler,
    /* [in] */ Int32 readback_id,
    /* [in] */ Float scale,
    /* [in] */ /*BitmapConfig*/Int32 config,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Float width,
    /* [in] */ Float height,
    /* [in] */ IInterface* contentViewCore)
{
}

void ContentReadbackHandler::NativeGetCompositorBitmap(
    /* [in] */ Int64 nativeContentReadbackHandler,
    /* [in] */ Int32 readback_id,
    /* [in] */ Int64 nativeWindowAndroid)
{
}

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
