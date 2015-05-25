
#include "CComparableTimSortHelper.h"
#include "ComparableTimSort.h"

namespace Elastos{
namespace Utility{

ECode CComparableTimSortHelper::Sort(
    /* [in] */ const ArrayOf<IInterface*>& a)
{
    return ComparableTimSort::Sort(a);
}

ECode CComparableTimSortHelper::SortEx(
    /* [in] */ const ArrayOf<IInterface*>& a,
    /* [in] */ Int32 lo,
    /* [in] */ Int32 hi)
{
    return ComparableTimSort::SortEx(a, lo, hi);
}

} // namespace Utility
} // namespace Elastos
