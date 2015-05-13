
#include "ext/frameworkext.h"
#include "content/pm/CApplicationInfoDisplayNameComparator.h"

namespace Elastos {
namespace Droid {
namespace Content {
namespace Pm {

ECode CApplicationInfoDisplayNameComparator::constructor(
    /* [in] */ IPackageManager* pm)
{
    mPM = pm;
    return NOERROR;
}

ECode CApplicationInfoDisplayNameComparator::Compare(
    /* [in] */ IApplicationInfo* aa,
    /* [in] */ IApplicationInfo* ab,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    assert(0);
    return E_NOT_IMPLEMENTED;
}

} // namespace Pm
} // namespace Content
} // namespace Droid
} // namespace Elastos
