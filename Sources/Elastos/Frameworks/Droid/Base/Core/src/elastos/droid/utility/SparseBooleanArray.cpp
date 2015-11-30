
#include "elastos/droid/utility/SparseBooleanArray.h"
#include "elastos/droid/utility/CSparseBooleanArray.h"
#include "elastos/droid/utility/ContainerHelpers.h"
#include "elastos/droid/internal/utility/ArrayUtils.h"
#include "elastos/droid/internal/utility/GrowingArrayUtils.h"
#include <elastos/core/StringBuilder.h>
#include <libcore/utility/EmptyArray.h>

using Elastos::Droid::Internal::Utility::ArrayUtils;
using Elastos::Droid::Internal::Utility::GrowingArrayUtils;
using Elastos::Core::EIID_ICloneable;
using Elastos::Core::StringBuilder;
using Libcore::Utility::EmptyArray;

namespace Elastos {
namespace Droid {
namespace Utility {

CAR_INTERFACE_IMPL_2(SparseBooleanArray, Object, ISparseBooleanArray, ICloneable);

SparseBooleanArray::SparseBooleanArray()
    : mSize(0)
{}

ECode SparseBooleanArray::constructor()
{
    return constructor(10);
}

ECode SparseBooleanArray::constructor(
    /* [in] */ Int32 initialCapacity)
{
    if (initialCapacity == 0) {
        mKeys = EmptyArray::INT32;
        mValues = EmptyArray::BOOLEAN;
    }
    else {
        mKeys = ArrayUtils::NewUnpaddedInt32Array(initialCapacity);
        mValues = ArrayOf<Boolean>::Alloc(mKeys->GetLength());
    }
    mSize = 0;
    return NOERROR;
}

ECode SparseBooleanArray::Clone(
    /* [out] */ IInterface** clone)
{
    VALIDATE_NOT_NULL(clone);
    *clone = NULL;

    AutoPtr<CSparseBooleanArray> cloneObj;
    CSparseBooleanArray::NewByFriend((CSparseBooleanArray**)&cloneObj);
    // try {
    cloneObj->mKeys = mKeys->Clone();
    cloneObj->mValues = mValues->Clone();
    cloneObj->mSize = mSize;
    // } catch (CloneNotSupportedException cnse) {
    //     /* ignore */
    // }
    *clone = (ISparseBooleanArray*)cloneObj.Get();
    REFCOUNT_ADD(*clone);
    return NOERROR;
}

ECode SparseBooleanArray::Get(
    /* [in] */ Int32 key,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value);
    return Get(key, FALSE, value);
}

ECode SparseBooleanArray::Get(
    /* [in] */ Int32 key,
    /* [in] */ Boolean valueIfKeyNotFound,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value);
    Int32 i = ContainerHelpers::BinarySearch(mKeys, mSize, key);

    if (i < 0 ) {
        *value = valueIfKeyNotFound;
        return NOERROR;
    }
    else {
        *value = (*mValues)[i];
        return NOERROR;
    }
}

ECode SparseBooleanArray::Delete(
    /* [in] */ Int32 key)
{
    Int32 i = ContainerHelpers::BinarySearch(mKeys, mSize, key);

    if (i >= 0) {
        mKeys->Copy(i, mKeys, i + 1, mSize - (i + 1));
        mValues->Copy(i, mValues, i + 1, mSize - (i + 1));
        mSize--;
    }
    return NOERROR;
}

ECode SparseBooleanArray::RemoveAt(
    /* [in] */ Int32 index)
{
    mKeys->Copy(index, mKeys, index + 1, mSize - (index + 1));
    mValues->Copy(index, mValues, index + 1, mSize - (index + 1));
    mSize--;
    return NOERROR;
}

ECode SparseBooleanArray::Put(
    /* [in] */ Int32 key,
    /* [in] */ Boolean value)
{
    Int32 i = ContainerHelpers::BinarySearch(mKeys, mSize, key);

    if (i >= 0) {
        mValues->Set(i, value);
    }
    else {
        i = ~i;

        mKeys = GrowingArrayUtils::Insert(mKeys, mSize, i, key);
        mValues = GrowingArrayUtils::Insert(mValues, mSize, i, value);
        mSize++;
    }
    return NOERROR;
}

ECode SparseBooleanArray::GetSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);
    *size = mSize;
    return NOERROR;
}

ECode SparseBooleanArray::KeyAt(
    /* [in] */ Int32 index,
    /* [out] */ Int32* key)
{
    VALIDATE_NOT_NULL(key);
    *key = (*mKeys)[index];
    return NOERROR;
}

ECode SparseBooleanArray::ValueAt(
    /* [in] */ Int32 index,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value);
    *value = (*mValues)[index];
    return NOERROR;
}

ECode SparseBooleanArray::SetValueAt(
    /* [in] */ Int32 index,
    /* [in] */ Boolean value)
{
    mValues->Set(index, value);
    return NOERROR;
}

ECode SparseBooleanArray::IndexOfKey(
    /* [in] */ Int32 key,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);

    *index = ContainerHelpers::BinarySearch(mKeys, mSize, key);
    return NOERROR;
}

ECode SparseBooleanArray::IndexOfValue(
    /* [in] */ Boolean value,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);

    for (Int32 i = 0; i < mSize; i++) {
        if ((*mValues)[i] == value) {
            *index = i;
            return NOERROR;
        }
    }

    *index = -1;
    return NOERROR;
}

ECode SparseBooleanArray::Clear()
{
    mSize = 0;
    return NOERROR;
}

ECode SparseBooleanArray::Append(
    /* [in] */ Int32 key,
    /* [in] */ Boolean value)
{
    if (mSize != 0 && key <= (*mKeys)[mSize - 1]) {
        Put(key, value);
        return NOERROR;
    }

    mKeys = GrowingArrayUtils::Append(mKeys, mSize, key);
    mValues = GrowingArrayUtils::Append(mValues, mSize, value);
    mSize++;

    return NOERROR;
}

ECode SparseBooleanArray::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    if (mSize <= 0) {
        *str = "{}";
        return NOERROR;
    }

    StringBuilder buffer(mSize * 28);
    buffer.AppendChar('{');
    for (Int32 i = 0; i < mSize; i++) {
        if (i > 0) {
            buffer.Append(", ");
        }
        Int32 key;
        KeyAt(i, &key);
        buffer.Append(key);
        buffer.AppendChar('=');
        Boolean value;
        ValueAt(i, &value);
        buffer.Append(value);
    }
    buffer.AppendChar('}');
    *str = buffer.ToString();
    return NOERROR;
}

} // namespace Utility
} // namespace Droid
} // namespace Elastos
