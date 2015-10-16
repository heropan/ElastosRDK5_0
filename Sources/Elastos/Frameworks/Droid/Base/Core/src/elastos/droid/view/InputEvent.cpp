
#include "elastos/droid/view/InputEvent.h"
#include "elastos/droid/view/CInputDevice.h"
#include "elastos/droid/ext/frameworkext.h"
#include <elastos/utility/logging/Logger.h>

using namespace Elastos::Utility::Logging;

namespace Elastos {
namespace Droid {
namespace View {

const Int32 InputEvent::PARCEL_TOKEN_MOTION_EVENT;
const Int32 InputEvent::PARCEL_TOKEN_KEY_EVENT;

Int32 InputEvent::sNextSeq = 0;
Mutex InputEvent::sNextSeqLock;
const Boolean InputEvent::TRACK_RECYCLED_LOCATION;

InputEvent::InputEvent()
    : mRecycled(FALSE)
{
    AutoLock lock(sNextSeqLock);
    mSeq = ++sNextSeq;
}

ECode InputEvent::GetDevice(
    /* [out] */ IInputDevice** device)
{
    assert(device);
    Int32 deviceId;
    GetDeviceId(&deviceId);
    return CInputDevice::GetDevice(deviceId, device);
}

ECode InputEvent::Recycle()
{
    // if (TRACK_RECYCLED_LOCATION) {
    //     if (mRecycledLocation != null) {
    //         throw new RuntimeException(toString() + " recycled twice!", mRecycledLocation);
    //     }
    //     mRecycledLocation = new RuntimeException("Last recycled here");
    // }
    // else {
        if (mRecycled) {
            Logger::E(String("InputEvent"), String("recycled twice!"));
            return E_RUNTIME_EXCEPTION;
            //throw new RuntimeException(toString() + " recycled twice!");
        }
        mRecycled = TRUE;
    //}

    return NOERROR;
}

ECode InputEvent::RecycleIfNeededAfterDispatch()
{
    return Recycle();
}

void InputEvent::PrepareForReuse()
{
    mRecycled = FALSE;
    //mRecycledLocation = NULL;
    AutoLock lock(sNextSeqLock);
    mSeq = ++sNextSeq;
}

ECode InputEvent::GetSequenceNumber(
    /* [out] */ Int32* seq)
{
    assert(seq);
    *seq = mSeq;

    return NOERROR;
}

}   //namespace View
}   //namespace Droid
}   //namespace Elastos
