
#include "view/SimpleOnScaleGestureListener.h"

namespace Elastos {
namespace Droid {
namespace View {

CAR_INTERFACE_IMPL(SimpleOnScaleGestureListener, IOnScaleGestureListener);

ECode SimpleOnScaleGestureListener::OnScale(
    /* [in ]*/ IScaleGestureDetector* detector,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = FALSE;
    return NOERROR;
}

ECode SimpleOnScaleGestureListener::OnScaleBegin(
    /* [in ]*/ IScaleGestureDetector* detector,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = TRUE;
    return NOERROR;
}

ECode SimpleOnScaleGestureListener::OnScaleEnd(
    /* [in ]*/ IScaleGestureDetector* detector)
{
    // Intentionally empty
    return NOERROR;
}

} // namespace View
} // namespace Droid
} // namespace Elastos
