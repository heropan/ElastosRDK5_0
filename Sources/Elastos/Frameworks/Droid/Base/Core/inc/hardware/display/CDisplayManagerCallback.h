#ifndef __ELASTOS_DROID_HARDWARE_DISPLAY_CDISPLAYMANAGERCALLBACK_H__
#define __ELASTOS_DROID_HARDWARE_DISPLAY_CDISPLAYMANAGERCALLBACK_H__

#include "_CDisplayManagerCallback.h"
#include "hardware/display/DisplayManagerGlobal.h"

namespace Elastos {
namespace Droid {
namespace Hardware {
namespace Display {

CarClass(CDisplayManagerCallback)
{
public:
    CARAPI constructor(
        /* [in] */ Handle32 owner);

    //@Override
    CARAPI OnDisplayEvent(
        /* [in] */ Int32 displayId,
        /* [in] */ Int32 event);

    CARAPI ToString(
        /* [out] */ String* str);

private:
    DisplayManagerGlobal* mOwner;
};

} // namespace Display
} // namespace Hardware
} // namepsace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_HARDWARE_DISPLAY_CDISPLAYMANAGERCALLBACK_H__
