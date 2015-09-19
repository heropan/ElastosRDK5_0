
#ifndef __ELASTOS_DROID_CONTENT_PM_CRESOLVEINFODISPLAYNAMECOMPARATOR_H__
#define __ELASTOS_DROID_CONTENT_PM_CRESOLVEINFODISPLAYNAMECOMPARATOR_H__

#include "_Elastos_Droid_Content_Pm_CResolveInfoDisplayNameComparator.h"
#include <elastos/core/Object.h>

using Elastos::Text::ICollator;

namespace Elastos {
namespace Droid {
namespace Content {
namespace Pm {

CarClass(CResolveInfoDisplayNameComparator)
{
public:
    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

    CResolveInfoDisplayNameComparator();

    CARAPI constructor(
        /* [in] */ IPackageManager* pm);

    CARAPI Compare(
        /* [in] */ IResolveInfo* a,
        /* [in] */ IResolveInfo* b,
        /* [out] */ Int32* result);

private:
    AutoPtr<ICollator> mCollator;
    AutoPtr<IPackageManager> mPM;
};

} // namespace Pm
} // namespace Content
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_CONTENT_PM_CRESOLVEINFODISPLAYNAMECOMPARATOR_H__
