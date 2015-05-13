
#include "ext/frameworkext.h"
#include "content/pm/CPackageItemInfoDisplayNameComparator.h"

namespace Elastos {
namespace Droid {
namespace Content {
namespace Pm {

ECode CPackageItemInfoDisplayNameComparator::constructor(
    /* [in] */ IPackageManager* pm)
{
    mPM = pm;
    return NOERROR;
}

ECode CPackageItemInfoDisplayNameComparator::Compare(
    /* [in] */ IPackageItemInfo* aa,
    /* [in] */ IPackageItemInfo* ab,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    // CharSequence  sa = aa.loadLabel(mPM);
    // if (sa == null) sa = aa.name;
    // CharSequence  sb = ab.loadLabel(mPM);
    // if (sb == null) sb = ab.name;
    // return sCollator.compare(sa.toString(), sb.toString());
    return NOERROR;
}

} // namespace Pm
} // namespace Content
} // namespace Droid
} // namespace Elastos
