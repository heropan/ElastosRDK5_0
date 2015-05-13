#ifndef __CLEGACYSENSORMANAGERROTATIONWATCHER_H__
#define __CLEGACYSENSORMANAGERROTATIONWATCHER_H__

#include "_CLegacySensorManagerRotationWatcher.h"

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
