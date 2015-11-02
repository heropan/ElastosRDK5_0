#include "elastos/droid/view/accessibility/CRangeInfoHelper.h"
#include "elastos/droid/view/accessibility/CRangeInfo.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Accessibility {

CAR_INTERFACE_IMPL(CRangeInfoHelper, Singleton, IRangeInfoHelper)

CAR_SINGLETON_IMPL(CRangeInfoHelper)

ECode CRangeInfoHelper::Obtain(
    /* [in] */ IRangeInfo* other,
    /* [out] */ IRangeInfo** info)
{
    VALIDATE_NOT_NULL(info);
    return CRangeInfo::Obtain(other, info);
}

ECode CRangeInfoHelper::Obtain(
    /* [in] */ Int32 type,
    /* [in] */ Float min,
    /* [in] */ Float max,
    /* [in] */ Float current,
    /* [out] */ IRangeInfo** info)
{
    VALIDATE_NOT_NULL(info);
    return CRangeInfo::Obtain(type, min, max, current, info);
}

} // Accessibility
} // View
} // Droid
} // Elastos
