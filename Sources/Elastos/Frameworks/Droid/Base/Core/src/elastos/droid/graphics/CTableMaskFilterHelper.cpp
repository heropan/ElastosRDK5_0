
#include "ext/frameworkext.h"
#include "graphics/CTableMaskFilterHelper.h"
#include "graphics/CTableMaskFilter.h"

namespace Elastos {
namespace Droid {
namespace Graphics {

ECode CTableMaskFilterHelper::CreateClipTable(
    /* [in] */ Int32 min,
    /* [in] */ Int32 max,
    /* [out] */ ITableMaskFilter** filter)
{
    VALIDATE_NOT_NULL(filter);

    return CTableMaskFilter::CreateClipTable(min, max, filter);
}

ECode CTableMaskFilterHelper::CreateGammaTable(
    /* [in] */ Float gamma,
    /* [out] */ ITableMaskFilter** filter)
{
    VALIDATE_NOT_NULL(filter);

    return CTableMaskFilter::CreateGammaTable(gamma, filter);
}

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos
