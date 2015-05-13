
#include "hardware/input/CInputManagerInputDevicesChangedListener.h"
#include "hardware/input/CInputManager.h"

namespace Elastos {
namespace Droid {
namespace Hardware {
namespace Input {

ECode CInputManagerInputDevicesChangedListener::constructor(
    /* [in] */ Handle32 owner)
{
    mOwner = (CInputManager*)owner;
    return NOERROR;
}

ECode CInputManagerInputDevicesChangedListener::OnInputDevicesChanged(
    /* [in] */ ArrayOf<Int32>* deviceIdAndGeneration)
{
    mOwner->OnInputDevicesChanged(deviceIdAndGeneration);
    return NOERROR;
}

} // namespace Input
} // namespace Hardware
} // namepsace Droid
} // namespace Elastos
