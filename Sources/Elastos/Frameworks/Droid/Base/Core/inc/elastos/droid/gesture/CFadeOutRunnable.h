#ifndef __ELASTOS_DROID_GESTURE_CFADEOUTRUNNABLE_H__
#define __GFADEOUTRUNNABLE_H__

#include "_Elastos_Droid_Gesture_CFadeOutRunnable.h"
#include "GestureOverlayView.h"

namespace Elastos {
namespace Droid {
namespace Gesture {

CarClass(CFadeOutRunnable), public GestureOverlayView::FadeOutRunnable
{
public:
    CARAPI constructor();

    CARAPI SetFireActionPerformed(
        /* [in] */ Boolean fireActionPerformed);

    CARAPI SetResetMultipleStrokes(
        /* [in] */ Boolean resetMultipleStrokes);

    CARAPI GetFireActionPerformed(
        /* [out] */ Boolean *fireActionPerformed);

    CARAPI GetResetMultipleStrokes(
        /* [out] */ Boolean *resetMultipleStrokes);

    CARAPI Run();

};

}
}
}

#endif
