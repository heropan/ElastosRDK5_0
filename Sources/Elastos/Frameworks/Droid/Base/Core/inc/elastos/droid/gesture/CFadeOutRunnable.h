#ifndef __ELASTOS_DROID_GESTURE_CFADEOUTRUNNABLE_H__
#define __GFADEOUTRUNNABLE_H__

#include "_Elastos_Droid_Gesture_CFadeOutRunnable.h"
#include "elastos/droid/gesture/GestureOverlayView.h"

namespace Elastos {
namespace Droid {
namespace Gesture {

CarClass(CFadeOutRunnable)
    , public GestureOverlayView::FadeOutRunnable
{
public:
    CAR_OBJECT_DECL();

    CFadeOutRunnable();

    virtual ~CFadeOutRunnable();

};

} // namespace Gesture
} // namespace Droid
} // namespace Elastos

#endif
