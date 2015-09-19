#ifndef __ELASTOS_DROID_GESTURE_CINSTANCE_H__
#define __ELASTOS_DROID_GESTURE_CINSTANCE_H__

#include "_Elastos_Droid_Gesture_CInstance.h"
#include "Instance.h"

namespace Elastos {
namespace Droid {
namespace Gesture {

CarClass(CInstance), public Instance
{
public:
    CARAPI Normalize();
};

}
}
}

#endif
