#include "view/CSimpleOnScaleGestureListener.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace View {

ECode CSimpleOnScaleGestureListener::OnScale(
    /* [in ]*/ IScaleGestureDetector* detector,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = FALSE;
    return NOERROR;
}

ECode CSimpleOnScaleGestureListener::OnScaleBegin(
    /* [in ]*/ IScaleGestureDetector* detector,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = TRUE;
    return NOERROR;
}

ECode CSimpleOnScaleGestureListener::OnScaleEnd(
    /* [in ]*/ IScaleGestureDetector* detector)
{
    // Intentionally empty
    return NOERROR;
}

} // namespace View
} // namespace Droid
} // namespace Elastos
