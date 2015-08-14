
#ifndef __ELASTOS_DROID_HARDWARE_INPUT_CINPUTMANAGERINPUTDEVICESCHANGEDLISTENER_H__
#define __ELASTOS_DROID_HARDWARE_INPUT_CINPUTMANAGERINPUTDEVICESCHANGEDLISTENER_H__

#include "_CInputManagerInputDevicesChangedListener.h"

namespace Elastos {
namespace Droid {
namespace Hardware {
namespace Input {

class CInputManager;

CarClass(CInputManagerInputDevicesChangedListener)
{
public:
    CARAPI constructor(
            /* [in] */ Handle32 owner);

    CARAPI OnInputDevicesChanged(
        /* [in] */ ArrayOf<Int32>* deviceIdAndGeneration);

private:
    CInputManager* mOwner;
};

} // namespace Input
} // namespace Hardware
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_HARDWARE_INPUT_CINPUTMANAGERINPUTDEVICESCHANGEDLISTENER_H__
