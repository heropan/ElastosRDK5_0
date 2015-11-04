#include "elastos/droid/utility/CInt64Array.h"
#include "elastos/droid/internal/utility/ArrayUtils.h"
#include <libcore/utility/EmptyArray.h>

using Elastos::Droid::Internal::Utility::ArrayUtils;
using Libcore::Utility::EmptyArray;

namespace Elastos {
namespace Droid {
namespace Utility {

const Int32 CInt64Array::MIN_CAPACITY_INCREMENT = 12;

CAR_INTERFACE_IMPL_2(CInt64Array, Object, IInt64Array, ICloneable)

CAR_OBJECT_IMPL(CInt64Array)

CInt64Array::CInt64Array()
    : mSize(10)
{
    mValues = ArrayOf<Int64>::Alloc(0);
}

ECode CInt64Array::constructor()
{
    mSize = 10;
    return NOERROR;
}

ECode CInt64Array::constructor(
    /* [in] */ Int32 initialCapacity)
{
    if (initialCapacity == 0) {
        mValues = EmptyArray::INT64;
    }
    else {
        mValues = ArrayUtils::NewUnpaddedInt64Array(initialCapacity);
    }
    mSize = 0;
    return NOERROR;
}

ECode CInt64Array::Add(
    /* [in] */ Int64 value)
{
    return Add(mSize, value);
}

ECode CInt64Array::Add(
    /* [in] */ Int32 index,
    /* [in] */ Int64 value)
{
    if (index < 0 || index > mSize) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    EnsureCapacity(1);

    if (mSize - index != 0) {
        mValues->Copy(index + 1, mValues, index, mSize - index);
    }

    (*mValues)[index] = value;
    mSize++;
    return NOERROR;
}

ECode CInt64Array::AddAll(
    /* [in] */ IInt64Array* values)
{
    Int32 count;
    values->GetSize(&count);
    EnsureCapacity(count);
    AutoPtr<CInt64Array> _values = (CInt64Array*)values;
    mValues->Copy(mSize, _values->mValues, 0, count);
    mSize += count;
    return NOERROR;
}

void CInt64Array::EnsureCapacity(
    /* [in] */ Int32 count)
{
    Int32 currentSize = mSize;
    Int32 minCapacity = currentSize + count;
    if (minCapacity >= mValues->GetLength()) {
        Int32 targetCap = currentSize + (currentSize < (MIN_CAPACITY_INCREMENT / 2) ?
                MIN_CAPACITY_INCREMENT : currentSize >> 1);
        Int32 newCapacity = targetCap > minCapacity ? targetCap : minCapacity;
        AutoPtr< ArrayOf<Int64> > newValues = ArrayUtils::NewUnpaddedInt64Array(newCapacity);
        newValues->Copy(0, mValues, 0, currentSize);
        mValues = newValues;
    }
}

ECode CInt64Array::Clear()
{
    mSize = 0;
    return NOERROR;
}

ECode CInt64Array::Clone(
    /* [out] */ IInterface** clone)
{
    VALIDATE_NOT_NULL(clone);
    *clone = NULL;

    AutoPtr<CInt64Array> _clone;
    // try {
        _clone->mValues = mValues->Clone();
        _clone->mSize = mSize;
    // } catch (CloneNotSupportedException cnse) {
    //     /* ignore */
    // }
    *clone = TO_IINTERFACE((IInt64Array*)_clone.Get());
    REFCOUNT_ADD(*clone);
    return NOERROR;
}

ECode CInt64Array::Get(
    /* [in] */ Int32 index,
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value)
    *value = 0;

    if (index >= mSize) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    *value = (*mValues)[index];
    return NOERROR;
}

ECode CInt64Array::IndexOf(
    /* [in] */ Int64 value,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);

    const Int32 n = mSize;
    for (Int32 i = 0; i < n; ++i) {
        if ((*mValues)[i] == value) {
            *index = i;
            return NOERROR;
        }
    }
    *index = -1;
    return NOERROR;
}

ECode CInt64Array::Remove(
    /* [in] */ Int32 index)
{
    if (index >= mSize) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    mValues->Copy(index, mValues, index + 1, mSize - index - 1);
    mSize--;
    return NOERROR;
}

ECode CInt64Array::GetSize(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    *value = mSize;
    return NOERROR;
}

} // Utility
} // Droid
} // Elastos
