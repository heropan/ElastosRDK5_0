
#ifndef __ELASTOS_DROID_CONTENT_PM_CPACKAGEITEMINFODISPLAYNAMECOMPARATOR_H__
#define __ELASTOS_DROID_CONTENT_PM_CPACKAGEITEMINFODISPLAYNAMECOMPARATOR_H__

#include "_Elastos_Droid_Content_Pm_CPackageItemInfoDisplayNameComparator.h"
#include <elastos/core/Object.h>

namespace Elastos {
namespace Droid {
namespace Content {
namespace Pm {

CarClass(CPackageItemInfoDisplayNameComparator)
{
public:
    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

    CARAPI constructor(
        /* [in] */ IPackageManager* pm);

    CARAPI Compare(
        /* [in] */ IPackageItemInfo* aa,
        /* [in] */ IPackageItemInfo* ab,
        /* [out] */ Int32* result);

private:
    // private final Collator   sCollator = Collator.getInstance();
    AutoPtr<IPackageManager> mPM;
};

} // namespace Pm
} // namespace Content
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_CONTENT_PM_CPACKAGEITEMINFODISPLAYNAMECOMPARATOR_H__
