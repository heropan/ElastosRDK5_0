#ifndef __ELASTOS_DROID_HARDWARE_CLEGACYSENSORMANAGERROTATIONWATCHER_H__
#define __ELASTOS_DROID_HARDWARE_CLEGACYSENSORMANAGERROTATIONWATCHER_H__

#include "_Elastos_Droid_Hardware_CLegacySensorManagerRotationWatcher.h"

namespace Elastos {
namespace Droid {
namespace Hardware {

CarClass(CLegacySensorManagerRotationWatcher)
{
public:
    CLegacySensorManagerRotationWatcher();

    CARAPI constructor();

    CARAPI OnRotationChanged(
        /* [in] */ Int32 rotation);
};

} // namespace Hardware
} // namespace Droid
} // namespace Elastos

#endif
