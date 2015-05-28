
#ifndef __UTILITY_CCOMPARABLETIMSORTHELPER_H__
#define __UTILITY_CCOMPARABLETIMSORTHELPER_H__

#include "_CComparableTimSortHelper.h"

namespace Elastos{
namespace Utility{

CarClass(CComparableTimSortHelper)
{
public:
    CARAPI Sort(
        /* [in] */ const ArrayOf<IInterface*>& a);

    CARAPI Sort(
        /* [in] */ const ArrayOf<IInterface*>& a,
        /* [in] */ Int32 lo,
        /* [in] */ Int32 hi);
};

} // namespace Utility
} // namespace Elastos

#endif //__UTILITY_CCOMPARABLETIMSORTHELPER_H__
