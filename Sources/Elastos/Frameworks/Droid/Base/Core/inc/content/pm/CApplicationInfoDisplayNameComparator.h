
#ifndef __ELASTOS_DROID_CONTENT_PM_CAPPLICATIONINFODISPLAYNAMECOMPARATOR_H__
#define __ELASTOS_DROID_CONTENT_PM_CAPPLICATIONINFODISPLAYNAMECOMPARATOR_H__

#include "_CApplicationInfoDisplayNameComparator.h"

namespace Elastos {
namespace Droid {
namespace Content {
namespace Pm {

CarClass(CApplicationInfoDisplayNameComparator)
{
public:
    CARAPI constructor(
        /* [in] */ IPackageManager * pm);

    CARAPI Compare(
        /* [in] */ IApplicationInfo* aa,
        /* [in] */ IApplicationInfo* ab,
        /* [out] */ Int32* result);

private:
    // private final Collator   sCollator = Collator.getInstance();
    AutoPtr<IPackageManager> mPM;
};

} // namespace Pm
} // namespace Content
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_CONTENT_PM_CAPPLICATIONINFODISPLAYNAMECOMPARATOR_H__
