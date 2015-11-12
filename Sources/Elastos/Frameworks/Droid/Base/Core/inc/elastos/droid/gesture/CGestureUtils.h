#ifndef __ELASTOS_DROID_GESTURE_CGESTUREUTILS_H__
#define __ELASTOS_DROID_GESTURE_CGESTUREUTILS_H__

#include "elastos/droid/gesture/GestureUtils.h"

namespace Elastos {
namespace Droid {
namespace Gesture {

CarClass(CGestureUtils)
    , public GestureUtils
{
public:
    CAR_OBJECT_DECL();

    CGestureUtils();

    virtual ~CGestureUtils();
};

} // namespace Gesture
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_GESTURE_CGESTUREUTILS_H__
