
#include "view/SimpleOnGestureListener.h"

namespace Elastos {
namespace Droid {
namespace View {

CAR_INTERFACE_IMPL_2(
    SimpleOnGestureListener, IOnGestureListener, IOnDoubleTapListener);

ECode SimpleOnGestureListener::OnSingleTapUp(
    /* [in] */ IMotionEvent* e,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = FALSE;
    return NOERROR;
}

ECode SimpleOnGestureListener::OnLongPress(
    /* [in] */ IMotionEvent* e)
{
    return NOERROR;
}

ECode SimpleOnGestureListener::OnScroll(
    /* [in] */ IMotionEvent* e1,
    /* [in] */ IMotionEvent* e2,
    /* [in] */ Float distanceX,
    /* [in] */ Float distanceY,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = FALSE;
    return NOERROR;
}

ECode SimpleOnGestureListener::OnFling(
    /* [in] */ IMotionEvent* e1,
    /* [in] */ IMotionEvent* e2,
    /* [in] */ Float velocityX,
    /* [in] */ Float velocityY,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = FALSE;
    return NOERROR;
}

ECode SimpleOnGestureListener::OnShowPress(
    /* [in] */ IMotionEvent* e)
{
    return NOERROR;
}

ECode SimpleOnGestureListener::OnDown(
    /* [in] */ IMotionEvent* e,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = FALSE;
    return NOERROR;
}

ECode SimpleOnGestureListener::OnDoubleTap(
    /* [in] */ IMotionEvent* e,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = FALSE;
    return NOERROR;
}

ECode SimpleOnGestureListener::OnDoubleTapEvent(
    /* [in] */ IMotionEvent* e,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = FALSE;
    return NOERROR;
}

ECode SimpleOnGestureListener::OnSingleTapConfirmed(
    /* [in] */ IMotionEvent* e,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = FALSE;
    return NOERROR;
}

} // namespace View
} // namespace Droid
} // namespace Elastos
