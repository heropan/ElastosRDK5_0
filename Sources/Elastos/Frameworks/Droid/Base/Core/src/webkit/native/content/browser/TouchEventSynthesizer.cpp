// wuweizuo automatic build .cpp file from .java file.

#include "TouchEventSynthesizer.h"

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {

//=====================================================================
//                        TouchEventSynthesizer
//=====================================================================
const Int32 TouchEventSynthesizer::MAX_NUM_POINTERS;
const Int32 TouchEventSynthesizer::ACTION_START;
const Int32 TouchEventSynthesizer::ACTION_MOVE;
const Int32 TouchEventSynthesizer::ACTION_CANCEL;
const Int32 TouchEventSynthesizer::ACTION_END;

TouchEventSynthesizer::TouchEventSynthesizer(
    /* [in] */ ContentViewCore* contentViewCore)
{
    // ==================before translated======================
    // mContentViewCore = contentViewCore;
    // mPointerProperties = new PointerProperties[MAX_NUM_POINTERS];
    // mPointerCoords = new PointerCoords[MAX_NUM_POINTERS];
}

ECode TouchEventSynthesizer::SetPointer(
    /* [in] */ Int32 index,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 id)
{
    // ==================before translated======================
    // assert (0 <= index && index < MAX_NUM_POINTERS);
    //
    // // Convert coordinates from density independent pixels to density dependent pixels.
    // float scaleFactor = mContentViewCore.getRenderCoordinates().getDeviceScaleFactor();
    //
    // PointerCoords coords = new PointerCoords();
    // coords.x = scaleFactor * x;
    // coords.y = scaleFactor * y;
    // coords.pressure = 1.0f;
    // mPointerCoords[index] = coords;
    //
    // PointerProperties properties = new PointerProperties();
    // properties.id = id;
    // mPointerProperties[index] = properties;
    assert(0);
    return NOERROR;
}

ECode TouchEventSynthesizer::Inject(
    /* [in] */ Int32 action,
    /* [in] */ Int32 pointerCount,
    /* [in] */ Int64 timeInMs)
{
    // ==================before translated======================
    // switch (action) {
    //     case ACTION_START: {
    //         mDownTimeInMs = timeInMs;
    //         MotionEvent event = MotionEvent.obtain(
    //                 mDownTimeInMs, timeInMs, MotionEvent.ACTION_DOWN, 1,
    //                 mPointerProperties, mPointerCoords,
    //                 0, 0, 1, 1, 0, 0, 0, 0);
    //         mContentViewCore.onTouchEvent(event);
    //         event.recycle();
    //
    //         if (pointerCount > 1) {
    //             event = MotionEvent.obtain(
    //                     mDownTimeInMs, timeInMs,
    //                     MotionEvent.ACTION_POINTER_DOWN, pointerCount,
    //                     mPointerProperties, mPointerCoords,
    //                     0, 0, 1, 1, 0, 0, 0, 0);
    //             mContentViewCore.onTouchEvent(event);
    //             event.recycle();
    //         }
    //         break;
    //     }
    //     case ACTION_MOVE: {
    //         MotionEvent event = MotionEvent.obtain(mDownTimeInMs, timeInMs,
    //                 MotionEvent.ACTION_MOVE,
    //                 pointerCount, mPointerProperties, mPointerCoords,
    //                 0, 0, 1, 1, 0, 0, 0, 0);
    //         mContentViewCore.onTouchEvent(event);
    //         event.recycle();
    //         break;
    //     }
    //     case ACTION_CANCEL: {
    //         MotionEvent event = MotionEvent.obtain(
    //                 mDownTimeInMs, timeInMs, MotionEvent.ACTION_CANCEL, 1,
    //                 mPointerProperties, mPointerCoords,
    //                 0, 0, 1, 1, 0, 0, 0, 0);
    //         mContentViewCore.onTouchEvent(event);
    //         event.recycle();
    //         break;
    //     }
    //     case ACTION_END: {
    //         if (pointerCount > 1) {
    //             MotionEvent event = MotionEvent.obtain(
    //                 mDownTimeInMs, timeInMs, MotionEvent.ACTION_POINTER_UP,
    //                 pointerCount, mPointerProperties, mPointerCoords,
    //                 0, 0, 1, 1, 0, 0, 0, 0);
    //             mContentViewCore.onTouchEvent(event);
    //             event.recycle();
    //         }
    //
    //         MotionEvent event = MotionEvent.obtain(
    //                 mDownTimeInMs, timeInMs, MotionEvent.ACTION_UP, 1,
    //                 mPointerProperties, mPointerCoords,
    //                 0, 0, 1, 1, 0, 0, 0, 0);
    //         mContentViewCore.onTouchEvent(event);
    //         event.recycle();
    //         break;
    //     }
    // }
    assert(0);
    return NOERROR;
}

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


