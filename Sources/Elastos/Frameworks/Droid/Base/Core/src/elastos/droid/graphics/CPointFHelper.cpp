
#include "ext/frameworkext.h"
#include "graphics/CPointFHelper.h"
#include "graphics/CPointF.h"

namespace Elastos {
namespace Droid {
namespace Graphics {

ECode CPointFHelper::Length(
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [out] */ Float* length)
{
    VALIDATE_NOT_NULL(length);

    *length = CPointF::Length(x, y);
    return NOERROR;
}

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos
