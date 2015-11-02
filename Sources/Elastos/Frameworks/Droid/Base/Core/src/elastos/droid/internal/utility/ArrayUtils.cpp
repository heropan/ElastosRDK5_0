
#include "elastos/droid/internal/utility/ArrayUtils.h"
#include "elastos/droid/utility/CArraySet.h"
#include <elastos/core/Math.h>

using Elastos::Droid::Utility::CArraySet;
using Elastos::Utility::CArrayList;
using Elastos::Utility::ICollection;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Utility {

AutoPtr<ArrayOf<Byte> > ArrayUtils::NewUnpaddedByteArray(
    /* [in] */ Int32 minLen)
{
    AutoPtr<ArrayOf<Byte> > array = ArrayOf<Byte>::Alloc(minLen);
    return array;
}

AutoPtr<ArrayOf<Char32> > ArrayUtils::NewUnpaddedChar32Array(
    /* [in] */ Int32 minLen)
{
    AutoPtr<ArrayOf<Char32> > array = ArrayOf<Char32>::Alloc(minLen);
    return array;
}

AutoPtr<ArrayOf<Int32> > ArrayUtils::NewUnpaddedInt32Array(
    /* [in] */ Int32 minLen)
{
    AutoPtr<ArrayOf<Int32> > array = ArrayOf<Int32>::Alloc(minLen);
    return array;
}

AutoPtr<ArrayOf<Int64> > ArrayUtils::NewUnpaddedInt64Array(
    /* [in] */ Int32 minLen)
{
    AutoPtr<ArrayOf<Int64> > array = ArrayOf<Int64>::Alloc(minLen);
    return array;
}

AutoPtr<ArrayOf<Boolean> > ArrayUtils::NewUnpaddedBooleanArray(
    /* [in] */ Int32 minLen)
{
    AutoPtr<ArrayOf<Boolean> > array = ArrayOf<Boolean>::Alloc(minLen);
    return array;
}

AutoPtr<ArrayOf<Float> > ArrayUtils::NewUnpaddedFloatArray(
    /* [in] */ Int32 minLen)
{
    AutoPtr<ArrayOf<Float> > array = ArrayOf<Float>::Alloc(minLen);
    return array;
}

AutoPtr<ArrayOf<Double> > ArrayUtils::NewUnpaddedDoubleArray(
    /* [in] */ Int32 minLen)
{
    AutoPtr<ArrayOf<Double> > array = ArrayOf<Double>::Alloc(minLen);
    return array;
}

AutoPtr<ArrayOf<IInterface*> > ArrayUtils::NewUnpaddedObjectArray(
    /* [in] */ Int32 minLen)
{
    AutoPtr<ArrayOf<IInterface*> > array = ArrayOf<IInterface*>::Alloc(minLen);
    return array;
}

Boolean ArrayUtils::Equals(
    /* [in] */ ArrayOf<Byte>* array1,
    /* [in] */ ArrayOf<Byte>* array2,
    /* [in] */ Int32 length)
{
    if (length < 0) {
        // throw new IllegalArgumentException();
        assert(0);
    }

    if (array1 == array2) {
        return TRUE;
    }
    if (array1 == NULL || array2 == NULL || array1->GetLength() < length || array2->GetLength() < length) {
        return FALSE;
    }
    for (Int32 i = 0; i < length; i++) {
        if ((*array1)[i] != (*array2)[i]) {
            return FALSE;
        }
    }
    return TRUE;
}

Boolean ArrayUtils::Contains(
    /* [in] */ ArrayOf<Int32>* array,
    /* [in] */ Int32 value)
{
    if (array == NULL)
        return FALSE;

    for (Int32 i = 0; i < array->GetLength(); i++) {
        if ((*array)[i] == value) {
            return TRUE;
        }
    }
    return FALSE;
}

Boolean ArrayUtils::Contains(
    /* [in] */ ArrayOf<Int64>* array,
    /* [in] */ Int64 value)
{
    if (array == NULL)
        return FALSE;

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
    if (array == NULL)
        return 0;
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

AutoPtr< ArrayOf<Int64> > ArrayUtils::AppendInt64(
    /* [in] */ ArrayOf<Int64>* cur,
    /* [in] */ Int64 val)
{
    if (cur == NULL) {
        AutoPtr< ArrayOf<Int64> > ret = ArrayOf<Int64>::Alloc(1);
        (*ret)[0] = val;
        return ret;
    }
    const Int32 N = cur->GetLength();
    for (Int32 i = 0; i < N; i++) {
        if ((*cur)[i] == val) {
            return cur;
        }
    }
    AutoPtr< ArrayOf<Int64> > ret = ArrayOf<Int64>::Alloc(N + 1);
    ret->Copy(cur, N);
    (*ret)[N] = val;
    return ret;
}

AutoPtr< ArrayOf<Int64> > ArrayUtils::RemoveInt64(
    /* [in] */ ArrayOf<Int64>* cur,
    /* [in] */ Int64 val)
{
    if (cur == NULL) {
        return NULL;
    }
    const Int32 N = cur->GetLength();
    for (Int32 i = 0; i < N; i++) {
        if ((*cur)[i] == val) {
            AutoPtr< ArrayOf<Int64> > ret = ArrayOf<Int64>::Alloc(N - 1);
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

AutoPtr< ArrayOf<Int64> > ArrayUtils::CloneOrNull(
    /* [in] */ ArrayOf<Int64>* array)
{
    return (array != NULL) ? array->Clone() : NULL;
}

AutoPtr<IArraySet> ArrayUtils::Add(
    /* [in] */ IArraySet* _cur,
    /* [in] */ IInterface* val)
{
    AutoPtr<IArraySet> cur = _cur;
    if (cur == NULL) {
        CArraySet::New((IArraySet**)&cur);
    }
    ICollection::Probe(cur)->Add(val);
    return cur;
}

AutoPtr<IArraySet> ArrayUtils::Remove(
    /* [in] */ IArraySet* cur,
    /* [in] */ IInterface* val)
{
    if (cur == NULL) {
        return NULL;
    }
    ICollection::Probe(cur)->Remove(val);
    Boolean isEmpty;
    ICollection::Probe(cur)->IsEmpty(&isEmpty);
    if (isEmpty) {
        return NULL;
    }
    else {
        return cur;
    }
}

Boolean ArrayUtils::Contains(
    /* [in] */ IArraySet* cur,
    /* [in] */ IInterface* val)
{
    Boolean contains = FALSE;
    if (cur != NULL)
        ICollection::Probe(cur)->Contains(val, &contains);
    return contains;
}

AutoPtr<IArrayList> ArrayUtils::Add(
    /* [in] */ IArrayList* _cur,
    /* [in] */ IInterface* val)
{
    AutoPtr<IArrayList> cur = _cur;
    if (cur == NULL) {
        CArrayList::New((IArrayList**)&cur);
    }
    ICollection::Probe(cur)->Add(val);
    return cur;
}

AutoPtr<IArrayList> ArrayUtils::Remove(
    /* [in] */ IArrayList* cur,
    /* [in] */ IInterface* val)
{
    if (cur == NULL) {
        return NULL;
    }
    ICollection::Probe(cur)->Remove(val);
    Boolean isEmpty;
    ICollection::Probe(cur)->IsEmpty(&isEmpty);
    if (isEmpty) {
        return NULL;
    }
    else {
        return cur;
    }
}

Boolean ArrayUtils::Contains(
    /* [in] */ IArrayList* cur,
    /* [in] */ IInterface* val)
{
    Boolean contains = FALSE;
    if (cur != NULL)
        ICollection::Probe(cur)->Contains(val, &contains);
    return contains;
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

} // namespace Utility
} // namespace Internal
} // namespace Droid
} // namespace Elastos
