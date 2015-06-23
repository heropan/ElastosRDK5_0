
#include "util/ArrayUtils.h"
#include <elastos/core/Math.h>

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Utility {

Int32 ArrayUtils::IdealByteArraySize(
    /* [in] */ Int32 need)
{
    for (Int32 i = 4; i < 32; i++) {
        if (need <= (1 << i) - 12) {
            return (1 << i) - 12;
        }
    }

    return need;
}

Int32 ArrayUtils::IdealBooleanArraySize(
    /* [in] */ Int32 need)
{
    return IdealByteArraySize(need);
}

Int32 ArrayUtils::IdealInt16ArraySize(
    /* [in] */ Int32 need)
{
    return IdealByteArraySize(need * 2) / 2;
}

Int32 ArrayUtils::IdealCharArraySize(
    /* [in] */ Int32 need)
{
    return IdealByteArraySize(need * 4) / 4;
}

Int32 ArrayUtils::IdealInt32ArraySize(
    /* [in] */ Int32 need)
{
    return IdealByteArraySize(need * 4) / 4;
}

Int32 ArrayUtils::IdealFloatArraySize(
    /* [in] */ Int32 need)
{
    return IdealByteArraySize(need * 4) / 4;
}

Int32 ArrayUtils::IdealObjectArraySize(
    /* [in] */ Int32 need)
{
    return IdealByteArraySize(need * 4) / 4;
}

Int32 ArrayUtils::IdealInt64ArraySize(
    /* [in] */ Int32 need)
{
    return IdealByteArraySize(need * 8) / 8;
}

void ArrayUtils::Sort(
    /* [in] */ ArrayOf<Int32> *array)
{
    if (array != NULL) {
        return QuickSort(array, 0, array->GetLength() - 1);
    }
}

void ArrayUtils::Sort(
    /* [in] */ ArrayOf<Int32> *array,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length)
{
    if (array != NULL) {
        return QuickSort(array, offset, offset + length - 1);
    }
}

void ArrayUtils::QuickSort(
    /* [in] */ ArrayOf<Int32> *array,
    /* [in] */ Int32 low,
    /* [in] */ Int32 high)
{
    if (array == NULL || low >= high || low < 0)
        return;

    high = Elastos::Core::Math::Min(high, array->GetLength() - 1);
    Int32 privot = (*array)[low];
    Int32 i = low, j = high;
    Int32 result;
    while (i < j) {
        while (i < j) {
            if ((*array)[j] >= privot){
                --j;
            }
        }

        if (i < j) {
            (*array)[i++] = (*array)[j];
        }

        while (i < j) {
            if ((*array)[i] <= privot){
                ++i;
            }
        }

        if (i < j) {
            (*array)[j--] = (*array)[i];
        }
    }

    (*array)[i] = privot;

    QuickSort(array, low, i - 1);
    QuickSort(array, i + 1, high);
}

Boolean ArrayUtils::Contains(
    /* [in] */ ArrayOf<Int32>* array,
    /* [in] */ Int32 value)
{
    for (Int32 i = 0; i < array->GetLength(); i++) {
        if ((*array)[i] == value) {
            return TRUE;
        }
    }
    return FALSE;
}

Int64 ArrayUtils::Total(
    /* [in] */ ArrayOf<Int64> *array)
{
    Int64 total = 0;
    for (Int32 i = 0; i < array->GetLength(); i++) {
        total += (*array)[i];
    }
    return total;
}

AutoPtr< ArrayOf<Int32> > ArrayUtils::AppendInt32(
    /* [in] */ ArrayOf<Int32>* cur,
    /* [in] */ Int32 val)
{
    if (cur == NULL) {
        AutoPtr< ArrayOf<Int32> > ret = ArrayOf<Int32>::Alloc(1);
        (*ret)[0] = val;
        return ret;
    }
    const Int32 N = cur->GetLength();
    for (Int32 i = 0; i < N; i++) {
        if ((*cur)[i] == val) {
            return cur;
        }
    }
    AutoPtr< ArrayOf<Int32> > ret = ArrayOf<Int32>::Alloc(N + 1);
    ret->Copy(cur, N);
    (*ret)[N] = val;
    return ret;
}

AutoPtr< ArrayOf<Int32> > ArrayUtils::RemoveInt32(
    /* [in] */ ArrayOf<Int32>* cur,
    /* [in] */ Int32 val)
{
    if (cur == NULL) {
        return NULL;
    }
    const Int32 N = cur->GetLength();
    for (Int32 i = 0; i < N; i++) {
        if ((*cur)[i] == val) {
            AutoPtr< ArrayOf<Int32> > ret = ArrayOf<Int32>::Alloc(N - 1);
            if (i > 0) {
                ret->Copy(cur, i);
            }
            if (i < (N - 1)) {
                ret->Copy(i, cur, i + 1, N - i -1);
            }
            return ret;
        }
    }
    return cur;
}

} // namespace Utility
} // namespace Internal
} // namespace Droid
} // namespace Elastos
