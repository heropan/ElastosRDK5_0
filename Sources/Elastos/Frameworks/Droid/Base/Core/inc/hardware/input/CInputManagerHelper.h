
#ifndef __CINPUTMANAGERHELPER_H__
#define __CINPUTMANAGERHELPER_H__

#include "_CInputManagerHelper.h"

namespace Elastos {
namespace Droid {
namespace Hardware {
namespace Input {

CarClass(CInputManagerHelper)
{
public:
    /**
     * Gets an instance of the input manager.
     *
     * @return The input manager instance.
     *
     * @hide
     */
    GetInstance(
        /* [out] */ IInputManager** manager);
};

} // namespace Input
} // namespace Hardware
} // namepsace Droid
} // namespace Elastos

#endif // __CINPUTMANAGERHELPER_H__
