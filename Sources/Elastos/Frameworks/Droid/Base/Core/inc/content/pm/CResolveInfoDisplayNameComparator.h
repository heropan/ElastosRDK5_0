
#ifndef __CRESOLVEINFODISPLAYNAMECOMPARATOR_H__
#define __CRESOLVEINFODISPLAYNAMECOMPARATOR_H__

#include "_CResolveInfoDisplayNameComparator.h"

using Elastos::Text::ICollator;

namespace Elastos {
namespace Droid {
namespace Content {
namespace Pm {

CarClass(CResolveInfoDisplayNameComparator)
{
public:
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

#endif // __CRESOLVEINFODISPLAYNAMECOMPARATOR_H__
