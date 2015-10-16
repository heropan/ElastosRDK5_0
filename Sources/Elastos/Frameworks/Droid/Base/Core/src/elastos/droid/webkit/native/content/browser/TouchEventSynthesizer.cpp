
#include "webkit/native/content/browser/TouchEventSynthesizer.h"
#include "webkit/native/content/browser/ContentViewCore.h"
// TODO #include "view/CPointerCoords.h"
// TODO #include "view/CPointerProperties.h"
// TODO #include "view/CMotionEventHelper.h"

// TODO using Elastos::Droid::View::CPointerCoords;
// TODO using Elastos::Droid::View::CPointerProperties;
// TODO using Elastos::Droid::View::CMotionEventHelper;
using Elastos::Droid::View::IMotionEventHelper;
using Elastos::Droid::View::IInputEvent;
using Elastos::Droid::View::EIID_IInputEvent;

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
    : mContentViewCore(contentViewCore)
{
    mPointerProperties = ArrayOf<IPointerProperties*>::Alloc(MAX_NUM_POINTERS);
    mPointerCoords = ArrayOf<IPointerCoords*>::Alloc(MAX_NUM_POINTERS);
}

ECode TouchEventSynthesizer::SetPointer(
    /* [in] */ Int32 index,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 id)
{
    assert (0 <= index && index < MAX_NUM_POINTERS);

    // Convert coordinates from density independent pixels to density dependent pixels.
    Float scaleFactor = mContentViewCore->GetRenderCoordinates()->GetDeviceScaleFactor();

    AutoPtr<IPointerCoords> coords;
    assert(0);
    // TODO
    // CPointerCoords::New((IPointerCoords**)&coords);
    coords->SetX(scaleFactor * x);
    coords->SetY(scaleFactor * y);
    coords->SetPressure(1.0f);
    (*mPointerCoords)[index] = coords;

    AutoPtr<IPointerProperties> properties;
    assert(0);
    // TODO
    // CPointerProperties::New((IPointerProperties**)&properties);
    properties->SetId(id);
    (*mPointerProperties)[index] = properties;

    return NOERROR;
}

ECode TouchEventSynthesizer::Inject(
    /* [in] */ Int32 action,
    /* [in] */ Int32 pointerCount,
    /* [in] */ Int64 timeInMs)
{
    AutoPtr<IMotionEventHelper> motionEventHelper;
    assert(0);
    // TODO
    // CMotionEventHelper::AcquireSingleton((IMotionEventHelper**)&motionEventHelper);

    switch (action) {
        case ACTION_START: {
            mDownTimeInMs = timeInMs;
            AutoPtr<IMotionEvent> event;
            motionEventHelper->Obtain(
                    mDownTimeInMs, timeInMs, IMotionEvent::ACTION_DOWN, 1,
                    mPointerProperties, mPointerCoords,
                    0, 0, 1, 1, 0, 0, 0, 0,
                    (IMotionEvent**)&event);
            mContentViewCore->OnTouchEvent(event);
            AutoPtr<IInputEvent> inputEvent = (IInputEvent*)event->Probe(EIID_IInputEvent);
            inputEvent->Recycle();

            if (pointerCount > 1) {
                motionEventHelper->Obtain(
                        mDownTimeInMs, timeInMs,
                        IMotionEvent::ACTION_POINTER_DOWN, pointerCount,
                        mPointerProperties, mPointerCoords,
                        0, 0, 1, 1, 0, 0, 0, 0,
                        (IMotionEvent**)&event);
                mContentViewCore->OnTouchEvent(event);
                AutoPtr<IInputEvent> inputEvent = (IInputEvent*)event->Probe(EIID_IInputEvent);
                inputEvent->Recycle();
            }
            break;
        }
        case ACTION_MOVE: {
            AutoPtr<IMotionEvent> event;
            motionEventHelper->Obtain(mDownTimeInMs, timeInMs,
                    IMotionEvent::ACTION_MOVE,
                    pointerCount, mPointerProperties, mPointerCoords,
                    0, 0, 1, 1, 0, 0, 0, 0,
                    (IMotionEvent**)&event);
            mContentViewCore->OnTouchEvent(event);
            AutoPtr<IInputEvent> inputEvent = (IInputEvent*)event->Probe(EIID_IInputEvent);
            inputEvent->Recycle();
            break;
        }
        case ACTION_CANCEL: {
            AutoPtr<IMotionEvent> event;
            motionEventHelper->Obtain(
                    mDownTimeInMs, timeInMs, IMotionEvent::ACTION_CANCEL, 1,
                    mPointerProperties, mPointerCoords,
                    0, 0, 1, 1, 0, 0, 0, 0,
                    (IMotionEvent**)&event);
            mContentViewCore->OnTouchEvent(event);
            AutoPtr<IInputEvent> inputEvent = (IInputEvent*)event->Probe(EIID_IInputEvent);
            inputEvent->Recycle();
            break;
        }
        case ACTION_END: {
            if (pointerCount > 1) {
                AutoPtr<IMotionEvent> event;
                motionEventHelper->Obtain(
                    mDownTimeInMs, timeInMs, IMotionEvent::ACTION_POINTER_UP,
                    pointerCount, mPointerProperties, mPointerCoords,
                    0, 0, 1, 1, 0, 0, 0, 0,
                    (IMotionEvent**)&event);
                mContentViewCore->OnTouchEvent(event);
                AutoPtr<IInputEvent> inputEvent = (IInputEvent*)event->Probe(EIID_IInputEvent);
                inputEvent->Recycle();
            }

            AutoPtr<IMotionEvent> event;
            motionEventHelper->Obtain(
                    mDownTimeInMs, timeInMs, IMotionEvent::ACTION_UP, 1,
                    mPointerProperties, mPointerCoords,
                    0, 0, 1, 1, 0, 0, 0, 0,
                    (IMotionEvent**)&event);
            mContentViewCore->OnTouchEvent(event);
            AutoPtr<IInputEvent> inputEvent = (IInputEvent*)event->Probe(EIID_IInputEvent);
            inputEvent->Recycle();
            break;
        }
    }

    return NOERROR;
}

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
