#ifndef __ELASTOS_DROID_GESTURE_CGESTURESTROKE_H__
#define __ELASTOS_DROID_GESTURE_CGESTURESTROKE_H__

#include "elastos/droid/gesture/GestureStroke.h"

namespace Elastos {
namespace Droid {
namespace Gesture {
/**
 * A gesture stroke started on a touch down and ended on a touch up. A stroke
 * consists of a sequence of timed points. One or multiple strokes form a gesture.
 */
CarClass(CGestureStroke)
    , public GestureStroke
{
public:
    CAR_OBJECT_DECL();

    CGestureStroke();
    virtual ~CGestureStroke();
};

} // namespace Gesture
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_GESTURE_CGESTURESTROKE_H__
