
#ifndef __SIMPLEONGESTURELISTENER_H__
#define __SIMPLEONGESTURELISTENER_H__

#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace View {

class SimpleOnGestureListener :
    public ElRefBase,
    public IOnGestureListener,
    public IOnDoubleTapListener
{
public:
    CAR_INTERFACE_DECL();

    CARAPI OnSingleTapUp(
        /* [in] */ IMotionEvent* e,
        /* [out] */ Boolean* res);

    CARAPI OnLongPress(
        /* [in] */ IMotionEvent* e);

    CARAPI OnScroll(
        /* [in] */ IMotionEvent* e1,
        /* [in] */ IMotionEvent* e2,
        /* [in] */ Float distanceX,
        /* [in] */ Float distanceY,
        /* [out] */ Boolean* res);

    CARAPI OnFling(
        /* [in] */ IMotionEvent* e1,
        /* [in] */ IMotionEvent* e2,
        /* [in] */ Float velocityX,
        /* [in] */ Float velocityY,
        /* [out] */ Boolean* res);

    CARAPI OnShowPress(
        /* [in] */ IMotionEvent* e);

    CARAPI OnDown(
        /* [in] */ IMotionEvent* e,
        /* [out] */ Boolean* res);

    CARAPI OnDoubleTap(
        /* [in] */ IMotionEvent* e,
        /* [out] */ Boolean* res);

    CARAPI OnDoubleTapEvent(
        /* [in] */ IMotionEvent* e,
        /* [out] */ Boolean* res);

    CARAPI OnSingleTapConfirmed(
        /* [in] */ IMotionEvent* e,
        /* [out] */ Boolean* res);
};

} // namespace View
} // namespace Droid
} // namespace Elastos

#endif //__SIMPLEONGESTURELISTENER_H__
