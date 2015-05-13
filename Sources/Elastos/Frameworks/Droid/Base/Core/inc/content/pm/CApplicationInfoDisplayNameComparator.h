
#ifndef __CAPPLICATIONINFODISPLAYNAMECOMPARATOR_H__
#define __CAPPLICATIONINFODISPLAYNAMECOMPARATOR_H__

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

#endif // __CAPPLICATIONINFODISPLAYNAMECOMPARATOR_H__
