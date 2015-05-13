
#include "cmdef.h"
#include "CArrays.h"
#include "CArrayOf.h"
#include "DualPivotQuicksort.h"
#include "ComparableTimSort.h"
#include "TimSort.h"
#include <elastos/Math.h>
#include <elastos/ObjectUtils.h>

using Elastos::Core::Math;
using Elastos::Core::IArrayOf;
using Elastos::Core::CArrayOf;
using Elastos::Core::IComparable;
using Elastos::Core::EIID_IInteger32;
using Elastos::Core::IInteger32;
using Elastos::Core::EIID_IChar32;
using Elastos::Core::IChar32;
using Elastos::Core::EIID_IBoolean;
using Elastos::Core::IBoolean;
using Elastos::Core::EIID_IByte;
using Elastos::Core::IByte;
using Elastos::Core::EIID_IInteger64;
using Elastos::Core::IInteger64;
using Elastos::Core::EIID_IFloat;
using Elastos::Core::IFloat;
using Elastos::Core::EIID_IDouble;
using Elastos::Core::IDouble;
using Elastos::Core::EIID_IInteger16;
using Elastos::Core::IInteger16;
using Elastos::Core::ObjectUtils;

namespace Elastos {
namespace Utility {

//====================================================================
// CArrays::ArrayList
//====================================================================

UInt32 CArrays::ArrayList::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CArrays::ArrayList::Release()
{
    return ElRefBase::Release();
}

PInterface CArrays::ArrayList::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_IInterface == riid) {
        return (PInterface)(IList*)this;
    }
    else if (EIID_IIterable == riid) {
        return (IIterable*)(IList*)this;
    }
    else if (EIID_ICollection == riid) {
        return (ICollection*)(IList*)this;
    }
    else if (EIID_IList == riid) {
        return (IList*)this;
    }
    else if (Elastos::IO::EIID_ISerializable == riid) {
        return (ISerializable*)this;
    }
    else if (EIID_IRandomAccess == riid) {
        return (IRandomAccess*)this;
    }

    return NULL;
}

ECode CArrays::ArrayList::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID)

    if (pObject == (IInterface*)(IList*)this) {
        *pIID = EIID_IList;
    }
    else if (pObject == (IInterface*)(ISerializable*)this) {
        *pIID = Elastos::IO::EIID_ISerializable;
    }
    else if (pObject == (IInterface*)(IRandomAccess*)this) {
        *pIID = EIID_IRandomAccess;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

CArrays::ArrayList::ArrayList(
    /* [in] */ ArrayOf<IInterface* >* storage)
{
    if (storage == NULL) {
//        throw new NullPointerException("storage == null");
        return;
    }
    mA = storage;
}

ECode CArrays::ArrayList::Contains(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    if (object != NULL) {
        for (Int32 i = 0; i < mA->GetLength(); i++) {
            AutoPtr<IInterface> element = (*mA)[i];
            if (ObjectUtils::Equals(object, element)) {
                *result = TRUE;
                return NOERROR;
            }
        }
    }
    else {
        for (Int32 i = 0; i < mA->GetLength(); i++) {
            AutoPtr<IInterface> element = (*mA)[i];
            if (element == NULL) {
                *result = TRUE;
                return NOERROR;
            }
        }
    }
    *result = FALSE;
    return NOERROR;
}

ECode CArrays::ArrayList::Get(
    /* [in] */ Int32 location,
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result)
    if (location < 0 || location >= mA->GetLength()) {
        *result = NULL;
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    *result = (*mA)[location];
    INTERFACE_ADDREF(*result)
    return NOERROR;
}

ECode CArrays::ArrayList::IndexOf(
    /* [in] */ IInterface* object,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)
    if (object != NULL) {
        for (Int32 i = 0; i < mA->GetLength(); i++) {
            if (ObjectUtils::Equals(object, (*mA)[i])) {
                *result = i;
                return NOERROR;
            }
        }
    }
    else {
        for (Int32 i = 0; i < mA->GetLength(); i++) {
            if ((*mA)[i] == NULL) {
                *result = i;
                return NOERROR;
            }
        }
    }
    *result = -1;
    return NOERROR;
}

ECode CArrays::ArrayList::LastIndexOf(
    /* [in] */ IInterface* object,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index)
    if (object != NULL) {
        for (Int32 i = mA->GetLength() - 1; i >= 0; i--) {
            if (ObjectUtils::Equals(object, (*mA)[i])) {
                *index = i;
                return NOERROR;
            }
        }
    }
    else {
        for (Int32 i = mA->GetLength() - 1; i >= 0; i--) {
            if ((*mA)[i] == NULL) {
                *index = i;
                return NOERROR;
            }
        }
    }
    *index = -1;
    return NOERROR;
}

ECode CArrays::ArrayList::Set(
    /* [in] */ Int32 location,
    /* [in] */ IInterface* object,
    /* [out] */ IInterface** preObject)
{
    VALIDATE_NOT_NULL(preObject)
    AutoPtr<IInterface> result = (*mA)[location];
    mA->Set(location, object);
    *preObject = result;
    INTERFACE_ADDREF(*preObject)
    return NOERROR;
}

ECode CArrays::ArrayList::GetSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size)
    *size = mA->GetLength();
    return NOERROR;
}

ECode CArrays::ArrayList::ToArray(
    /* [out] */ ArrayOf<IInterface* >** outArray)
{
    VALIDATE_NOT_NULL(outArray)
    *outArray = mA->Clone();
    INTERFACE_ADDREF(*outArray)
    return NOERROR;
}

ECode CArrays::ArrayList::ToArrayEx(
    /* [in] */ ArrayOf<IInterface* >* contents,
    /* [out] */ ArrayOf<IInterface* >** outArray)
{
    VALIDATE_NOT_NULL(outArray)
    Int32 size;
    GetSize(&size);
    AutoPtr<ArrayOf<IInterface*> > array = size > contents->GetLength() ?
            ArrayOf<IInterface*>::Alloc(size) : contents;
    array->Copy(mA, size);
    if (size < contents->GetLength()) {
        array->Set(size, NULL);
    }
    *outArray = array;
    INTERFACE_ADDREF(*outArray)
    return NOERROR;
}

ECode CArrays::ArrayList::GetIterator(
            /* [out] */ IIterator** it)
{
    return AbstractList::GetIterator(it);
}

ECode CArrays::ArrayList::Add(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return AbstractList::Add(object, modified);
}

ECode CArrays::ArrayList::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractList::AddAll(collection, modified);
}

ECode CArrays::ArrayList::Clear()
{
    return AbstractList::Clear();
}

ECode CArrays::ArrayList::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* res)
{
    return AbstractList::ContainsAll(collection, res);
}

ECode CArrays::ArrayList::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* res)
{
    return AbstractList::Equals(object, res);
}

ECode CArrays::ArrayList::GetHashCode(
    /* [in] */ Int32* hashCode)
{
    return AbstractList::GetHashCode(hashCode);
}

ECode CArrays::ArrayList::IsEmpty(
    /* [out] */ Boolean* res)
{
    return AbstractList::IsEmpty(res);
}

ECode CArrays::ArrayList::Remove(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return AbstractList::Remove(object, modified);
}

ECode CArrays::ArrayList::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractList::RemoveAll(collection, modified);
}

ECode CArrays::ArrayList::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractList::RetainAll(collection, modified);
}

ECode CArrays::ArrayList::AddEx(
    /* [in] */ Int32 location,
    /* [in] */ IInterface* object)
{
    return AbstractList::AddEx(location, object);
}

ECode CArrays::ArrayList::AddAllEx(
    /* [in] */ Int32 location,
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractList::AddAllEx(location, collection, modified);
}

ECode CArrays::ArrayList::GetListIterator(
    /* [out] */ IListIterator** it)
{
    return AbstractList::GetListIterator(it);
}

ECode CArrays::ArrayList::GetListIteratorEx(
    /* [in] */ Int32 location,
    /* [out] */ IListIterator** it)
{
    return AbstractList::GetListIteratorEx(location, it);
}

ECode CArrays::ArrayList::RemoveEx(
    /* [in] */ Int32 location,
    /* [out] */ IInterface** object)
{
    return AbstractList::RemoveEx(location, object);
}

ECode CArrays::ArrayList::SubList(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ IList** subList)
{
    return AbstractList::SubList(start, end, subList);
}

//====================================================================
// CArrays::
//====================================================================

// CArrays::CArrays()
// {
//     /* empty */
// }

ECode CArrays::_AsList(
    /* [in] */ ArrayOf<IInterface*>* array,
    /* [out] */ IList** outList)
{
    VALIDATE_NOT_NULL(outList)
    *outList = new ArrayList(array);
    INTERFACE_ADDREF(*outList)
    return NOERROR;
}

ECode CArrays::_BinarySearchByte(
    /* [in] */ ArrayOf<Byte>* array,
    /* [in] */ Byte value,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index)
    return _BinarySearchByteEx(array, 0, array->GetLength(), value, index);
}

ECode CArrays::_BinarySearchByteEx(
    /* [in] */ ArrayOf<Byte>* array,
    /* [in] */ Int32 startIndex,
    /* [in] */ Int32 endIndex,
    /* [in] */ Byte value,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index)
    FAIL_RETURN(CheckBinarySearchBounds(startIndex, endIndex, array->GetLength()));
    Int32 lo = startIndex;
    Int32 hi = endIndex - 1;

    while (lo <= hi) {
        Int32 mid = (UInt32(lo + hi)) >> 1;
        Byte midVal = (*array)[mid];

        if (midVal < value) {
            lo = mid + 1;
        }
        else if (midVal > value) {
            hi = mid - 1;
        }
        else {
            *index = mid;  // value found
            return NOERROR;
        }
    }
    *index = ~lo;  // value not present
    return NOERROR;
}

ECode CArrays::_BinarySearchChar32(
    /* [in] */ ArrayOf<Char32>* array,
    /* [in] */ Char32 value,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index)
    return _BinarySearchChar32Ex(array, 0, array->GetLength(), value, index);
}

ECode CArrays::_BinarySearchChar32Ex(
    /* [in] */ ArrayOf<Char32>* array,
    /* [in] */ Int32 startIndex,
    /* [in] */ Int32 endIndex,
    /* [in] */ Char32 value,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index)
    FAIL_RETURN(CheckBinarySearchBounds(startIndex, endIndex, array->GetLength()));
    Int32 lo = startIndex;
    Int32 hi = endIndex - 1;

    while (lo <= hi) {
        Int32 mid = (lo + hi) >> 1;    // Int32 mid = (lo + hi) >>> 1;
        Char32 midVal = (*array)[mid];

        if (midVal < value) {
            lo = mid + 1;
        }
        else if (midVal > value) {
            hi = mid - 1;
        }
        else {
            *index = mid;  // value found
            return NOERROR;
        }
    }
    *index = ~lo;  // value not present
    return NOERROR;
}

ECode CArrays::_BinarySearchDouble(
    /* [in] */ ArrayOf<Double>* array,
    /* [in] */ Double value,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index)
    return _BinarySearchDoubleEx(array, 0, array->GetLength(), value, index);
}

ECode CArrays::_BinarySearchDoubleEx(
    /* [in] */ ArrayOf<Double>* array,
    /* [in] */ Int32 startIndex,
    /* [in] */ Int32 endIndex,
    /* [in] */ Double value,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index)
    FAIL_RETURN(CheckBinarySearchBounds(startIndex, endIndex, array->GetLength()));
    Int32 lo = startIndex;
    Int32 hi = endIndex - 1;

    while (lo <= hi) {
        Int32 mid = (UInt32(lo + hi)) >> 1;
        Double midVal = (*array)[mid];

        if (midVal < value) {
            lo = mid + 1;
        }
        else if (midVal > value) {
            hi = mid - 1;
        }
        else if (midVal != 0 && midVal == value) {
            *index = mid;  // value found
            return NOERROR;
        }
        else { // Either midVal and value are == 0 or at least one is NaN
            Int64 midValBits = Elastos::Core::Math::DoubleToInt64Bits(midVal);
            Int64 valueBits  = Elastos::Core::Math::DoubleToInt64Bits(value);

            if (midValBits < valueBits) {
                lo = mid + 1; // (-0.0, 0.0) or (not NaN, NaN); midVal < val
            }
            else if (midValBits > valueBits) {
                hi = mid - 1; // (0.0, -0.0) or (NaN, not NaN); midVal > val
            }
            else {
                *index = mid; // bit patterns are equal; value found
                return NOERROR;
            }
        }
    }
    *index = ~lo;  // value not present
    return NOERROR;
}

ECode CArrays::_BinarySearchFloat(
    /* [in] */ ArrayOf<Float>* array,
    /* [in] */ Float value,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index)
    return _BinarySearchFloatEx(array, 0, array->GetLength(), value, index);
}

ECode CArrays::_BinarySearchFloatEx(
    /* [in] */ ArrayOf<Float>* array,
    /* [in] */ Int32 startIndex,
    /* [in] */ Int32 endIndex,
    /* [in] */ Float value,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index)
    FAIL_RETURN(CheckBinarySearchBounds(startIndex, endIndex, array->GetLength()));
    Int32 lo = startIndex;
    Int32 hi = endIndex - 1;

    while (lo <= hi) {
        Int32 mid = (UInt32(lo + hi)) >> 1;
        Float midVal = (*array)[mid];

        if (midVal < value) {
            lo = mid + 1;
        }
        else if (midVal > value) {
            hi = mid - 1;
        }
        else if (midVal != 0 && midVal == value) {
            *index = mid;  // value found
            return NOERROR;
        }
        else { // Either midVal and value are == 0 or at least one is NaN
            Int32 midValBits = Elastos::Core::Math::FloatToInt32Bits(midVal);
            Int32 valueBits  = Elastos::Core::Math::FloatToInt32Bits(value);

            if (midValBits < valueBits) {
                lo = mid + 1; // (-0.0, 0.0) or (not NaN, NaN); midVal < val
            }
            else if (midValBits > valueBits) {
                hi = mid - 1; // (0.0, -0.0) or (NaN, not NaN); midVal > val
            }
            else {
                *index = mid; // bit patterns are equal; value found
                return NOERROR;
            }
        }
    }
    *index = ~lo;  // value not present
    return NOERROR;
}

ECode CArrays::_BinarySearchInt32(
    /* [in] */ ArrayOf<Int32>* array,
    /* [in] */ Int32 value,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index)
    return _BinarySearchInt32Ex(array, 0, array->GetLength(), value, index);
}

ECode CArrays::_BinarySearchInt32Ex(
    /* [in] */ ArrayOf<Int32>* array,
    /* [in] */ Int32 startIndex,
    /* [in] */ Int32 endIndex,
    /* [in] */ Int32 value,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index)
    FAIL_RETURN(CheckBinarySearchBounds(startIndex, endIndex, array->GetLength()));
    Int32 lo = startIndex;
    Int32 hi = endIndex - 1;

    while (lo <= hi) {
        Int32 mid = (UInt32(lo + hi)) >> 1;
        Int32 midVal = (*array)[mid];

        if (midVal < value) {
            lo = mid + 1;
        }
        else if (midVal > value) {
            hi = mid - 1;
        }
        else {
            *index = mid;
            return NOERROR;  // value found
        }
    }
    *index = ~lo;  // value not present
    return NOERROR;
}

ECode CArrays::_BinarySearchInt64(
    /* [in] */ ArrayOf<Int64>* array,
    /* [in] */ Int64 value,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index)
    return _BinarySearchInt64Ex(array, 0, array->GetLength(), value, index);
}

ECode CArrays::_BinarySearchInt64Ex(
    /* [in] */ ArrayOf<Int64>* array,
    /* [in] */ Int32 startIndex,
    /* [in] */ Int32 endIndex,
    /* [in] */ Int64 value,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index)
    FAIL_RETURN(CheckBinarySearchBounds(startIndex, endIndex, array->GetLength()));
    Int32 lo = startIndex;
    Int32 hi = endIndex - 1;

    while (lo <= hi) {
        Int32 mid = (UInt32(lo + hi)) >> 1;
        Int64 midVal = (*array)[mid];

        if (midVal < value) {
            lo = mid + 1;
        }
        else if (midVal > value) {
            hi = mid - 1;
        }
        else {
            *index = mid;  // value found
            return NOERROR;
        }
     }
     *index = ~lo;  // value not present
     return NOERROR;
}

ECode CArrays::_BinarySearchObject(
    /* [in] */ ArrayOf<IInterface* >* array,
    /* [in] */ IInterface* value,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index)
    return _BinarySearchObjectEx(array, 0, array->GetLength(), value, index);
}

ECode CArrays::_BinarySearchObjectEx(
    /* [in] */ ArrayOf<IInterface* >* array,
    /* [in] */ Int32 startIndex,
    /* [in] */ Int32 endIndex,
    /* [in] */ IInterface* value,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index)
    FAIL_RETURN(CheckBinarySearchBounds(startIndex, endIndex, array->GetLength()));
    Int32 lo = startIndex;
    Int32 hi = endIndex - 1;

    while (lo <= hi) {
        Int32 mid = (UInt32(lo + hi)) >> 1;
        AutoPtr<IComparable> p = IComparable::Probe((*array)[mid]);
        assert(p != NULL);
        Int32 midValCmp;
        p->CompareTo(value, &midValCmp);

        if (midValCmp < 0) {
            lo = mid + 1;
        }
        else if (midValCmp > 0) {
            hi = mid - 1;
        }
        else {
            *index = mid;  // value found
            return NOERROR;
        }
    }
    *index = ~lo;  // value not present
    return NOERROR;
}

ECode CArrays::_BinarySearch(
    /* [in] */ ArrayOf<IInterface* >* array,
    /* [in] */ IInterface* value,
    /* [in] */ IComparator* comparator,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index)
    return _BinarySearchEx(array, 0, array->GetLength(), value, comparator, index);
}

ECode CArrays::_BinarySearchEx(
    /* [in] */ ArrayOf<IInterface* >* array,
    /* [in] */ Int32 startIndex,
    /* [in] */ Int32 endIndex,
    /* [in] */ IInterface* value,
    /* [in] */ IComparator* comparator,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index)
    if (comparator == NULL) {
        return _BinarySearchObjectEx(array, startIndex, endIndex, value, index);
    }

    FAIL_RETURN(CheckBinarySearchBounds(startIndex, endIndex, array->GetLength()));
    Int32 lo = startIndex;
    Int32 hi = endIndex - 1;

    while (lo <= hi) {
        Int32 mid = (UInt32(lo + hi)) >> 1;
        Int32 midValCmp;
        comparator->Compare((*array)[mid], value, &midValCmp);

        if (midValCmp < 0) {
            lo = mid + 1;
        }
        else if (midValCmp > 0) {
            hi = mid - 1;
        }
        else {
            *index = mid;  // value found
            return NOERROR;
        }
    }
    *index = ~lo;  // value not present
    return NOERROR;
}

ECode CArrays::_BinarySearchInt16(
    /* [in] */ ArrayOf<Int16>* array,
    /* [in] */ Int16 value,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index)
    return _BinarySearchInt16Ex(array, 0, array->GetLength(), value, index);
}

ECode CArrays::_BinarySearchInt16Ex(
    /* [in] */ ArrayOf<Int16>* array,
    /* [in] */ Int32 startIndex,
    /* [in] */ Int32 endIndex,
    /* [in] */ Int16 value,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index)
    FAIL_RETURN(CheckBinarySearchBounds(startIndex, endIndex, array->GetLength()));
    Int32 lo = startIndex;
    Int32 hi = endIndex - 1;

    while (lo <= hi) {
        Int32 mid = (UInt32(lo + hi)) >> 1;
        Int16 midVal = (*array)[mid];

        if (midVal < value) {
            lo = mid + 1;
        }
        else if (midVal > value) {
            hi = mid - 1;
        }
        else {
            *index = mid;  // value found
            return NOERROR;
        }
    }
    *index = ~lo;  // value not present
    return NOERROR;
}

ECode CArrays::CheckBinarySearchBounds(
    /* [in] */ Int32 startIndex,
    /* [in] */ Int32 endIndex,
    /* [in] */ Int32 length)
{
    if (startIndex > endIndex) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (startIndex < 0 || endIndex > length) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    return NOERROR;
}

ECode CArrays::_FillByte(
    /* [in] */ ArrayOf<Byte>* array,
    /* [in] */ Byte value)
{
    for (Int32 i = 0; i < array->GetLength(); i++) {
        array->Set(i, value);
    }
    return NOERROR;
}

ECode CArrays::_FillByteEx(
    /* [in] */ ArrayOf<Byte>* array,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Byte value)
{
    FAIL_RETURN(_CheckStartAndEnd(array->GetLength(), start, end));
    for (Int32 i = start; i < end; i++) {
        array->Set(i, value);
    }
    return NOERROR;
}

ECode CArrays::_FillInt16(
    /* [in] */ ArrayOf<Int16>* array,
    /* [in] */ Int16 value)
{
    for (Int32 i = 0; i < array->GetLength(); i++) {
        array->Set(i, value);
    }
    return NOERROR;
}

ECode CArrays::_FillInt16Ex(
    /* [in] */ ArrayOf<Int16>* array,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int16 value)
{
    FAIL_RETURN(_CheckStartAndEnd(array->GetLength(), start, end));
    for (Int32 i = start; i < end; i++) {
        array->Set(i, value);
    }
    return NOERROR;
}

ECode CArrays::_FillChar32(
    /* [in] */ ArrayOf<Char32>* array,
    /* [in] */ Char32 value)
{
    for (Int32 i = 0; i < array->GetLength(); i++) {
        array->Set(i, value);
    }
    return NOERROR;
}

ECode CArrays::_FillChar32Ex(
    /* [in] */ ArrayOf<Char32>* array,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Char32 value)
{
    FAIL_RETURN(_CheckStartAndEnd(array->GetLength(), start, end));
    for (Int32 i = start; i < end; i++) {
        array->Set(i, value);
    }
    return NOERROR;
}

ECode CArrays::_FillInt32(
    /* [in] */ ArrayOf<Int32>* array,
    /* [in] */ Int32 value)
{
    for (Int32 i = 0; i < array->GetLength(); i++) {
        array->Set(i, value);
    }
    return NOERROR;
}

ECode CArrays::_FillInt32Ex(
    /* [in] */ ArrayOf<Int32>* array,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 value)
{
    FAIL_RETURN(_CheckStartAndEnd(array->GetLength(), start, end));
    for (Int32 i = start; i < end; i++) {
        array->Set(i, value);
    }
    return NOERROR;
}

ECode CArrays::_FillInt64(
    /* [in] */ ArrayOf<Int64>* array,
    /* [in] */ Int64 value)
{
    for (Int32 i = 0; i < array->GetLength(); i++) {
        array->Set(i, value);
    }
    return NOERROR;
}

ECode CArrays::_FillInt64Ex(
    /* [in] */ ArrayOf<Int64>* array,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int64 value)
{
    FAIL_RETURN(_CheckStartAndEnd(array->GetLength(), start, end));
    for (Int32 i = start; i < end; i++) {
        array->Set(i, value);
    }
    return NOERROR;
}

ECode CArrays::_FillFloat(
    /* [in] */ ArrayOf<Float>* array,
    /* [in] */ Float value)
{
    for (Int32 i = 0; i < array->GetLength(); i++) {
        array->Set(i, value);
    }
    return NOERROR;
}

ECode CArrays::_FillFloatEx(
    /* [in] */ ArrayOf<Float>* array,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Float value)
{
    FAIL_RETURN(_CheckStartAndEnd(array->GetLength(), start, end));
    for (Int32 i = start; i < end; i++) {
        array->Set(i, value);
    }
    return NOERROR;
}

ECode CArrays::_FillDouble(
    /* [in] */ ArrayOf<Double>* array,
    /* [in] */ Double value)
{
    for (Int32 i = 0; i < array->GetLength(); i++) {
        array->Set(i, value);
    }
    return NOERROR;
}

ECode CArrays::_FillDoubleEx(
    /* [in] */ ArrayOf<Double>* array,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Double value)
{
    FAIL_RETURN(_CheckStartAndEnd(array->GetLength(), start, end));
    for (Int32 i = start; i < end; i++) {
        array->Set(i, value);
    }
    return NOERROR;
}

ECode CArrays::_FillBoolean(
    /* [in] */ ArrayOf<Boolean>* array,
    /* [in] */ Boolean value)
{
    for (Int32 i = 0; i < array->GetLength(); i++) {
        array->Set(i, value);
    }
    return NOERROR;
}

ECode CArrays::_FillBooleanEx(
    /* [in] */ ArrayOf<Boolean>* array,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Boolean value)
{
    FAIL_RETURN(_CheckStartAndEnd(array->GetLength(), start, end));
    for (Int32 i = start; i < end; i++) {
        array->Set(i, value);
    }
    return NOERROR;
}

ECode CArrays::_FillObject(
    /* [in] */ ArrayOf<IInterface* >* array,
    /* [in] */ IInterface* value)
{
    for (Int32 i = 0; i < array->GetLength(); i++) {
        array->Set(i, value);
    }
    return NOERROR;
}

ECode CArrays::_FillObjectEx(
    /* [in] */ ArrayOf<IInterface* >* array,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ IInterface* value)
{
    FAIL_RETURN(_CheckStartAndEnd(array->GetLength(), start, end));
    for (Int32 i = start; i < end; i++) {
        array->Set(i, value);
    }
    return NOERROR;
}

ECode CArrays::_HashCodeBoolean(
    /* [in] */ ArrayOf<Boolean>* array,
    /* [out] */ Int32* code)
{
    VALIDATE_NOT_NULL(code)
    if (array == NULL) {
        *code = 0;
        return NOERROR;
    }
    Int32 hashCode = 1;
    for (Int32 i = 0; i < array->GetLength(); i++) {
        Boolean element = (*array)[i];
        // 1231, 1237 are hash code values for boolean value
        hashCode = 31 * hashCode + (element ? 1231 : 1237);
    }
    *code = hashCode;
    return NOERROR;
}

ECode CArrays::_HashCodeInt32(
    /* [in] */ ArrayOf<Int32>* array,
    /* [out] */ Int32* code)
{
    VALIDATE_NOT_NULL(code)
    if (array == NULL) {
        *code = 0;
        return NOERROR;
    }
    Int32 hashCode = 1;
    for (Int32 i = 0; i < array->GetLength(); i++) {
        Int32 element = (*array)[i];
        // the hash code value for integer value is integer value itself
        hashCode = 31 * hashCode + element;
    }
    *code = hashCode;
    return NOERROR;
}

ECode CArrays::_HashCodeInt16(
    /* [in] */ ArrayOf<Int16>* array,
    /* [out] */ Int32* code)
{
    VALIDATE_NOT_NULL(code)
    if (array == NULL) {
        *code = 0;
        return NOERROR;
    }
    Int32 hashCode = 1;
    for (Int32 i = 0; i < array->GetLength(); i++) {
        Int16 element = (*array)[i];
        // the hash code value for short value is its integer value
        hashCode = 31 * hashCode + element;
    }
    *code = hashCode;
    return NOERROR;
}

ECode CArrays::_HashCodeChar32(
    /* [in] */ ArrayOf<Char32>* array,
    /* [out] */ Int32* code)
{
    VALIDATE_NOT_NULL(code)
    if (array == NULL) {
        *code = 0;
        return NOERROR;
    }
    Int32 hashCode = 1;
    for (Int32 i = 0; i < array->GetLength(); i++) {
        Char32 element = (*array)[i];
        // the hash code value for char value is its integer value
        hashCode = 31 * hashCode + element;
    }
    *code = hashCode;
    return NOERROR;
}

ECode CArrays::_HashCodeByte(
    /* [in] */ ArrayOf<Byte>* array,
    /* [out] */ Int32* code)
{
    VALIDATE_NOT_NULL(code)
    if (array == NULL) {
        *code = 0;
        return NOERROR;
    }
    Int32 hashCode = 1;
    for (Int32 i = 0; i < array->GetLength(); i++) {
        Byte element = (*array)[i];
        // the hash code value for byte value is its integer value
        hashCode = 31 * hashCode + element;
    }
    *code = hashCode;
    return NOERROR;
}

ECode CArrays::_HashCodeInt64(
    /* [in] */ ArrayOf<Int64>* array,
    /* [out] */ Int32* code)
{
    VALIDATE_NOT_NULL(code)
    if (array == NULL) {
        *code = 0;
        return NOERROR;
    }
    Int32 hashCode = 1;
    for (Int32 i = 0; i < array->GetLength(); i++) {
        Int64 elementValue = (*array)[i];
        /*
         * the hash code value for long value is (int) (value ^ (value >>>
         * 32))
         */
        hashCode = 31 * hashCode
                + (Int32) (elementValue ^ (elementValue >> 32));
    }
    *code = hashCode;
    return NOERROR;
}

ECode CArrays::_HashCodeFloat(
    /* [in] */ ArrayOf<Float>* array,
    /* [out] */ Int32* code)
{
    VALIDATE_NOT_NULL(code)
    if (array == NULL) {
        *code = 0;
        return NOERROR;
    }
    Int32 hashCode = 1;
    for (Int32 i = 0; i < array->GetLength(); i++) {
        Float element = (*array)[i];
        /*
         * the hash code value for float value is
         * Float.floatToIntBits(value)
         */
        hashCode = 31 * hashCode + Elastos::Core::Math::FloatToInt32Bits(element);
    }
    *code = hashCode;
    return NOERROR;
}

ECode CArrays::_HashCodeDouble(
    /* [in] */ ArrayOf<Double>* array,
    /* [out] */ Int32* code)
{
    VALIDATE_NOT_NULL(code)
    if (array == NULL) {
        *code = 0;
        return NOERROR;
    }
    Int32 hashCode = 1;
    for (Int32 i = 0; i < array->GetLength(); i++) {
        Double element = (*array)[i];
        Int64 v = Elastos::Core::Math::DoubleToInt64Bits(element);
        /*
         * the hash code value for double value is (int) (v ^ (v >>> 32))
         * where v = Double.doubleToLongBits(value)
         */
        hashCode = 31 * hashCode + (Int32) (v ^ (v >> 32));
    }
    *code = hashCode;
    return NOERROR;
}

ECode CArrays::_HashCodeObject(
    /* [in] */ ArrayOf<IInterface* >* array,
    /* [out] */ Int32* code)
{
    VALIDATE_NOT_NULL(code)
    if (array == NULL) {
        *code = 0;
        return NOERROR;
    }
    Int32 hashCode = 1;
    for (Int32 i = 0; i < array->GetLength(); i++) {
        AutoPtr<IInterface> element = (*array)[i];
        Int32 elementHashCode;
        if (element == NULL) {
            elementHashCode = 0;
        }
        else {
            elementHashCode = ObjectUtils::GetHashCode(element);
        }
        hashCode = 31 * hashCode + elementHashCode;
    }
    *code = hashCode;
    return NOERROR;
}

ECode CArrays::_DeepHashCode(
    /* [in] */ ArrayOf<IInterface* >* array,
    /* [out] */ Int32* code)
{
    VALIDATE_NOT_NULL(code)
    if (array == NULL) {
        *code = 0;
        return NOERROR;
    }
    Int32 hashCode = 1;
    for (Int32 i = 0; i < array->GetLength(); i++) {
        AutoPtr<IInterface> element = (*array)[i];
        Int32 elementHashCode = DeepHashCodeElement(element);
        hashCode = 31 * hashCode + elementHashCode;
    }
    *code = hashCode;
    return NOERROR;
}

Int32 CArrays::DeepHashCodeElement(
    /* [in] */ IInterface* element)
{
    if (element == NULL) {
         return 0;
    }

    if (IArrayOf::Probe(element) == NULL) {
        return ObjectUtils::GetHashCode(element);
    }

    /*
     * element is an array
     */
    Int32 hashCode;
    Int32 size;
    AutoPtr<IArrayOf> p = IArrayOf::Probe(element);
    InterfaceID riid;
    p->GetTypeId(&riid);
    p->GetLength(&size);
    if (riid == EIID_IInteger32) {
        AutoPtr<ArrayOf<Int32> > arr = ArrayOf<Int32>::Alloc(size);
        for (Int32 i = 0; i < size; ++i) {
            AutoPtr<IInteger32> elem;
            p->Get(i, (IInterface**)&elem);
            Int32 realNum;
            elem->GetValue(&realNum);
            arr->Set(i, realNum);
        }
        _HashCodeInt32(arr, &hashCode);
        return hashCode;
    }
    if (riid == EIID_IChar32) {
        AutoPtr<ArrayOf<Char32> > arr = ArrayOf<Char32>::Alloc(size);
        for (Int32 i = 0; i < size; ++i) {
            AutoPtr<IChar32> elem;
            p->Get(i, (IInterface**)&elem);
            Char32 realNum;
            elem->GetValue(&realNum);
            arr->Set(i, realNum);
        }
        _HashCodeChar32(arr, &hashCode);
        return hashCode;
    }
    if (riid == EIID_IBoolean) {
        AutoPtr<ArrayOf<Boolean> > arr = ArrayOf<Boolean>::Alloc(size);
        for (Int32 i = 0; i < size; ++i) {
            AutoPtr<IBoolean> elem;
            p->Get(i, (IInterface**)&elem);
            Boolean realNum;
            elem->GetValue(&realNum);
            arr->Set(i, realNum);
        }
        _HashCodeBoolean(arr, &hashCode);
        return hashCode;
    }
    if (riid == EIID_IByte) {
        AutoPtr<ArrayOf<Byte> > arr = ArrayOf<Byte>::Alloc(size);
        for (Int32 i = 0; i < size; ++i) {
            AutoPtr<IByte> elem;
            p->Get(i, (IInterface**)&elem);
            Byte realNum;
            elem->GetValue(&realNum);
            arr->Set(i, realNum);
        }
        _HashCodeByte(arr, &hashCode);
        return hashCode;
    }
    if (riid == EIID_IInteger64) {
        AutoPtr<ArrayOf<Int64> > arr = ArrayOf<Int64>::Alloc(size);
        for (Int32 i = 0; i < size; ++i) {
            AutoPtr<IInteger64> elem;
            p->Get(i, (IInterface**)&elem);
            Int64 realNum;
            elem->GetValue(&realNum);
            arr->Set(i, realNum);
        }
        _HashCodeInt64(arr, &hashCode);
        return hashCode;
    }
    if (riid == EIID_IFloat) {
        AutoPtr<ArrayOf<Float> > arr = ArrayOf<Float>::Alloc(size);
        for (Int32 i = 0; i < size; ++i) {
            AutoPtr<IFloat> elem;
            p->Get(i, (IInterface**)&elem);
            Float realNum;
            elem->GetValue(&realNum);
            arr->Set(i, realNum);
        }
        _HashCodeFloat(arr, &hashCode);
        return hashCode;
    }
    if (riid == EIID_IDouble) {
        AutoPtr<ArrayOf<Double> > arr = ArrayOf<Double>::Alloc(size);
        for (Int32 i = 0; i < size; ++i) {
            AutoPtr<IDouble> elem;
            p->Get(i, (IInterface**)&elem);
            Double realNum;
            elem->GetValue(&realNum);
            arr->Set(i, realNum);
        }
        _HashCodeDouble(arr, &hashCode);
        return hashCode;
    }
    if (riid == EIID_IInteger16) {
        AutoPtr<ArrayOf<Int16> > arr = ArrayOf<Int16>::Alloc(size);
        for (Int32 i = 0; i < size; ++i) {
            AutoPtr<IInteger16> elem;
            p->Get(i, (IInterface**)&elem);
            Int16 realNum;
            elem->GetValue(&realNum);
            arr->Set(i, realNum);
        }
        _HashCodeInt16(arr, &hashCode);
        return hashCode;
    }

    AutoPtr<ArrayOf<IInterface*> > arr = ArrayOf<IInterface*>::Alloc(size);
    for (Int32 i = 0; i < size; ++i) {
        AutoPtr<IInterface> elem;
        p->Get(i, (IInterface**)&elem);
        arr->Set(i, elem);
    }
    _DeepHashCode(arr, &hashCode);
    return hashCode;
}

ECode CArrays::_EqualsByte(
    /* [in] */ ArrayOf<Byte>* array1,
    /* [in] */ ArrayOf<Byte>* array2,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    if (array1 == array2) {
        *result = TRUE;
        return NOERROR;
    }
    if (array1 == NULL || array2 == NULL || array1->GetLength() != array2->GetLength()) {
        *result = FALSE;
        return NOERROR;
    }
    for (Int32 i = 0; i < array1->GetLength(); i++) {
        if ((*array1)[i] != (*array2)[i]) {
            *result = FALSE;
            return NOERROR;
        }
    }
    *result = TRUE;
    return NOERROR;
}

ECode CArrays::_EqualsInt16(
    /* [in] */ ArrayOf<Int16>* array1,
    /* [in] */ ArrayOf<Int16>* array2,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    if (array1 == array2) {
        *result = TRUE;
        return NOERROR;
    }
    if (array1 == NULL || array2 == NULL || array1->GetLength() != array2->GetLength()) {
        *result = FALSE;
        return NOERROR;
    }
    for (Int32 i = 0; i < array1->GetLength(); i++) {
        if ((*array1)[i] != (*array2)[i]) {
            *result = FALSE;
            return NOERROR;
        }
    }
    *result = TRUE;
    return NOERROR;
}

ECode CArrays::_EqualsChar32(
    /* [in] */ ArrayOf<Char32>* array1,
    /* [in] */ ArrayOf<Char32>* array2,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    if (array1 == array2) {
        *result = TRUE;
        return NOERROR;
    }
    if (array1 == NULL || array2 == NULL || array1->GetLength() != array2->GetLength()) {
        *result = FALSE;
        return NOERROR;
    }
    for (Int32 i = 0; i < array1->GetLength(); i++) {
        if ((*array1)[i] != (*array2)[i]) {
            *result = FALSE;
            return NOERROR;
        }
    }
    *result = TRUE;
    return NOERROR;
}

ECode CArrays::_EqualsInt32(
    /* [in] */ ArrayOf<Int32>* array1,
    /* [in] */ ArrayOf<Int32>* array2,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    if (array1 == array2) {
        *result = TRUE;
        return NOERROR;
    }
    if (array1 == NULL || array2 == NULL || array1->GetLength() != array2->GetLength()) {
        *result = FALSE;
        return NOERROR;
    }
    for (Int32 i = 0; i < array1->GetLength(); i++) {
        if ((*array1)[i] != (*array2)[i]) {
            *result = FALSE;
            return NOERROR;
        }
    }
    *result = TRUE;
    return NOERROR;
}

ECode CArrays::_EqualsInt64(
    /* [in] */ ArrayOf<Int64>* array1,
    /* [in] */ ArrayOf<Int64>* array2,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    if (array1 == array2) {
        *result = TRUE;
        return NOERROR;
    }
    if (array1 == NULL || array2 == NULL || array1->GetLength() != array2->GetLength()) {
        *result = FALSE;
        return NOERROR;
    }
    for (Int32 i = 0; i < array1->GetLength(); i++) {
        if ((*array1)[i] != (*array2)[i]) {
            *result = FALSE;
            return NOERROR;
        }
    }
    *result = TRUE;
    return NOERROR;
}

ECode CArrays::_EqualsFloat(
    /* [in] */ ArrayOf<Float>* array1,
    /* [in] */ ArrayOf<Float>* array2,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    if (array1 == array2) {
        *result = TRUE;
        return NOERROR;
    }
    if (array1 == NULL || array2 == NULL || array1->GetLength() != array2->GetLength()) {
        *result = FALSE;
        return NOERROR;
    }
    for (Int32 i = 0; i < array1->GetLength(); i++) {
        Int32 num1 = Elastos::Core::Math::FloatToInt32Bits((*array1)[i]);
        Int32 num2 = Elastos::Core::Math::FloatToInt32Bits((*array2)[i]);
        if (num1 != num2) {
            *result = FALSE;
            return NOERROR;
        }
    }
    *result = TRUE;
    return NOERROR;
}

ECode CArrays::_EqualsDouble(
    /* [in] */ ArrayOf<Double>* array1,
    /* [in] */ ArrayOf<Double>* array2,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    if (array1 == array2) {
        *result = TRUE;
        return NOERROR;
    }
    if (array1 == NULL || array2 == NULL || array1->GetLength() != array2->GetLength()) {
        *result = FALSE;
        return NOERROR;
    }
    for (Int32 i = 0; i < array1->GetLength(); i++) {
        Int64 num1 = Elastos::Core::Math::DoubleToInt64Bits((*array1)[i]);
        Int64 num2 = Elastos::Core::Math::DoubleToInt64Bits((*array2)[i]);
        if (num1 != num2) {
            *result = FALSE;
            return NOERROR;
        }
    }
    *result = TRUE;
    return NOERROR;
}

ECode CArrays::_EqualsBoolean(
    /* [in] */ ArrayOf<Boolean>* array1,
    /* [in] */ ArrayOf<Boolean>* array2,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    if (array1 == array2) {
        *result = TRUE;
        return NOERROR;
    }
    if (array1 == NULL || array2 == NULL || array1->GetLength() != array2->GetLength()) {
        *result = FALSE;
        return NOERROR;
    }
    for (Int32 i = 0; i < array1->GetLength(); i++) {
        if ((*array1)[i] != (*array2)[i]) {
            *result = FALSE;
            return NOERROR;
        }
    }
    *result = TRUE;
    return NOERROR;
}

ECode CArrays::_EqualsObject(
    /* [in] */ ArrayOf<IInterface* >* array1,
    /* [in] */ ArrayOf<IInterface* >* array2,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    if (array1 == array2) {
        *result = TRUE;
        return NOERROR;
    }
    if (array1 == NULL || array2 == NULL || array1->GetLength() != array2->GetLength()) {
        *result = FALSE;
        return NOERROR;
    }
    for (Int32 i = 0; i < array1->GetLength(); i++) {
        AutoPtr<IInterface> e1 = (*array1)[i];
        AutoPtr<IInterface> e2 = (*array2)[i];
        if (!(e1 == NULL ? e2 == NULL : ObjectUtils::Equals(e1, e2))) {
            *result = FALSE;
            return NOERROR;
        }
    }
    *result = TRUE;
    return NOERROR;
}

ECode CArrays::_DeepEquals(
    /* [in] */ ArrayOf<IInterface* >* array1,
    /* [in] */ ArrayOf<IInterface* >* array2,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    if (array1 == array2) {
        *result = TRUE;
        return NOERROR;
    }
    if (array1 == NULL || array2 == NULL || array1->GetLength() != array2->GetLength()) {
        *result = FALSE;
        return NOERROR;
    }
    for (Int32 i = 0; i < array1->GetLength(); i++) {
        AutoPtr<IInterface> e1 = (*array1)[i];
        AutoPtr<IInterface> e2 = (*array2)[i];
        if (!DeepEqualsElements(e1, e2)) {
            *result = FALSE;
            return NOERROR;
        }
    }
    *result = TRUE;
    return NOERROR;
}

Boolean CArrays::DeepEqualsElements(
    /* [in] */ IInterface* e1,
    /* [in] */ IInterface* e2)
{
    if (e1->Probe(EIID_IInterface) == e2->Probe(EIID_IInterface)) {
        return TRUE;
    }

    if (e1 == NULL || e2 == NULL) {
        return FALSE;
    }

    if (IArrayOf::Probe(e1) == NULL || IArrayOf::Probe(e2) == NULL) {
        return ObjectUtils::Equals(e1, e2);
    }

    /*
     * compare as arrays
     */
    AutoPtr<IArrayOf> p1 = IArrayOf::Probe(e1);
    AutoPtr<IArrayOf> p2 = IArrayOf::Probe(e2);
    InterfaceID riid1, riid2;
    p1->GetTypeId(&riid1);
    p2->GetTypeId(&riid2);
    if (riid1 != riid2) {
        return FALSE;
    }
    Boolean result;
    Int32 size;
    if (riid1 == EIID_IInterface) {
        p1->GetLength(&size);
        AutoPtr<ArrayOf<IInterface*> > first = ArrayOf<IInterface*>::Alloc(size);
        for (Int32 i = 0; i < size; ++i) {
            AutoPtr<IInterface> elem;
            p1->Get(i, (IInterface**)&elem);
            first->Set(i, elem);
        }

        p2->GetLength(&size);
        AutoPtr<ArrayOf<IInterface*> > sencond = ArrayOf<IInterface*>::Alloc(size);
        for (Int32 i = 0; i < size; ++i) {
            AutoPtr<IInterface> elem;
            p2->Get(i, (IInterface**)&elem);
            sencond->Set(i, elem);
        }
        _DeepEquals(first, sencond, &result);
        return result;
    }

    if (riid1 == EIID_IInteger32) {
        p1->GetLength(&size);
        AutoPtr<ArrayOf<Int32> > first = ArrayOf<Int32>::Alloc(size);
        for (Int32 i = 0; i < size; ++i) {
            AutoPtr<IInteger32> elem;
            p1->Get(i, (IInterface**)&elem);
            Int32 realNum;
            elem->GetValue(&realNum);
            first->Set(i, realNum);
        }

        p2->GetLength(&size);
        AutoPtr<ArrayOf<Int32> > sencond = ArrayOf<Int32>::Alloc(size);
        for (Int32 i = 0; i < size; ++i) {
            AutoPtr<IInteger32> elem;
            p2->Get(i, (IInterface**)&elem);
            Int32 realNum;
            elem->GetValue(&realNum);
            sencond->Set(i, realNum);
        }
        _EqualsInt32(first, sencond, &result);
        return result;
    }
    if (riid1 == EIID_IChar32) {
        p1->GetLength(&size);
        AutoPtr<ArrayOf<Char32> > first = ArrayOf<Char32>::Alloc(size);
        for (Int32 i = 0; i < size; ++i) {
            AutoPtr<IChar32> elem;
            p1->Get(i, (IInterface**)&elem);
            Char32 realNum;
            elem->GetValue(&realNum);
            first->Set(i, realNum);
        }

        p2->GetLength(&size);
        AutoPtr<ArrayOf<Char32> > sencond = ArrayOf<Char32>::Alloc(size);
        for (Int32 i = 0; i < size; ++i) {
            AutoPtr<IChar32> elem;
            p2->Get(i, (IInterface**)&elem);
            Char32 realNum;
            elem->GetValue(&realNum);
            sencond->Set(i, realNum);
        }
        _EqualsChar32(first, sencond, &result);
        return result;
    }
    if (riid1 == EIID_IBoolean) {
        p1->GetLength(&size);
        AutoPtr<ArrayOf<Boolean> > first = ArrayOf<Boolean>::Alloc(size);
        for (Int32 i = 0; i < size; ++i) {
            AutoPtr<IBoolean> elem;
            p1->Get(i, (IInterface**)&elem);
            Boolean realNum;
            elem->GetValue(&realNum);
            first->Set(i, realNum);
        }

        p2->GetLength(&size);
        AutoPtr<ArrayOf<Boolean> > sencond = ArrayOf<Boolean>::Alloc(size);
        for (Int32 i = 0; i < size; ++i) {
            AutoPtr<IBoolean> elem;
            p2->Get(i, (IInterface**)&elem);
            Boolean realNum;
            elem->GetValue(&realNum);
            sencond->Set(i, realNum);
        }
        _EqualsBoolean(first, sencond, &result);
        return result;
    }
    if (riid1 == EIID_IByte) {
        p1->GetLength(&size);
        AutoPtr<ArrayOf<Byte> > first = ArrayOf<Byte>::Alloc(size);
        for (Int32 i = 0; i < size; ++i) {
            AutoPtr<IByte> elem;
            p1->Get(i, (IInterface**)&elem);
            Byte realNum;
            elem->GetValue(&realNum);
            first->Set(i, realNum);
        }

        p2->GetLength(&size);
        AutoPtr<ArrayOf<Byte> > sencond = ArrayOf<Byte>::Alloc(size);
        for (Int32 i = 0; i < size; ++i) {
            AutoPtr<IByte> elem;
            p2->Get(i, (IInterface**)&elem);
            Byte realNum;
            elem->GetValue(&realNum);
            sencond->Set(i, realNum);
        }
        _EqualsByte(first, sencond, &result);
        return result;
    }
    if (riid1 == EIID_IInteger64) {
        p1->GetLength(&size);
        AutoPtr<ArrayOf<Int64> > first = ArrayOf<Int64>::Alloc(size);
        for (Int32 i = 0; i < size; ++i) {
            AutoPtr<IInteger64> elem;
            p1->Get(i, (IInterface**)&elem);
            Int64 realNum;
            elem->GetValue(&realNum);
            first->Set(i, realNum);
        }

        p2->GetLength(&size);
        AutoPtr<ArrayOf<Int64> > sencond = ArrayOf<Int64>::Alloc(size);
        for (Int32 i = 0; i < size; ++i) {
            AutoPtr<IInteger64> elem;
            p2->Get(i, (IInterface**)&elem);
            Int64 realNum;
            elem->GetValue(&realNum);
            sencond->Set(i, realNum);
        }
        _EqualsInt64(first, sencond, &result);
        return result;
    }
    if (riid1 == EIID_IFloat) {
        p1->GetLength(&size);
        AutoPtr<ArrayOf<Float> > first = ArrayOf<Float>::Alloc(size);
        for (Int32 i = 0; i < size; ++i) {
            AutoPtr<IFloat> elem;
            p1->Get(i, (IInterface**)&elem);
            Float realNum;
            elem->GetValue(&realNum);
            first->Set(i, realNum);
        }

        p2->GetLength(&size);
        AutoPtr<ArrayOf<Float> > sencond = ArrayOf<Float>::Alloc(size);
        for (Int32 i = 0; i < size; ++i) {
            AutoPtr<IFloat> elem;
            p2->Get(i, (IInterface**)&elem);
            Float realNum;
            elem->GetValue(&realNum);
            sencond->Set(i, realNum);
        }
        _EqualsFloat(first, sencond, &result);
        return result;
    }
    if (riid1 == EIID_IDouble) {
        p1->GetLength(&size);
        AutoPtr<ArrayOf<Double> > first = ArrayOf<Double>::Alloc(size);
        for (Int32 i = 0; i < size; ++i) {
            AutoPtr<IDouble> elem;
            p1->Get(i, (IInterface**)&elem);
            Double realNum;
            elem->GetValue(&realNum);
            first->Set(i, realNum);
        }

        p2->GetLength(&size);
        AutoPtr<ArrayOf<Double> > sencond = ArrayOf<Double>::Alloc(size);
        for (Int32 i = 0; i < size; ++i) {
            AutoPtr<IDouble> elem;
            p2->Get(i, (IInterface**)&elem);
            Double realNum;
            elem->GetValue(&realNum);
            sencond->Set(i, realNum);
        }
        _EqualsDouble(first, sencond, &result);
        return result;
    }
    if (riid1 == EIID_IInteger16) {
        p1->GetLength(&size);
        AutoPtr<ArrayOf<Int16> > first = ArrayOf<Int16>::Alloc(size);
        for (Int32 i = 0; i < size; ++i) {
            AutoPtr<IInteger16> elem;
            p1->Get(i, (IInterface**)&elem);
            Int16 realNum;
            elem->GetValue(&realNum);
            first->Set(i, realNum);
        }

        p2->GetLength(&size);
        AutoPtr<ArrayOf<Int16> > sencond = ArrayOf<Int16>::Alloc(size);
        for (Int32 i = 0; i < size; ++i) {
            AutoPtr<IInteger16> elem;
            p2->Get(i, (IInterface**)&elem);
            Int16 realNum;
            elem->GetValue(&realNum);
            sencond->Set(i, realNum);
        }
        _EqualsInt16(first, sencond, &result);
        return result;
    }
    p1->GetLength(&size);
    AutoPtr<ArrayOf<IInterface*> > first = ArrayOf<IInterface*>::Alloc(size);
    for (Int32 i = 0; i < size; ++i) {
        AutoPtr<IInterface> elem;
        p1->Get(i, (IInterface**)&elem);
        first->Set(i, elem);
    }

    p2->GetLength(&size);
    AutoPtr<ArrayOf<IInterface*> > sencond = ArrayOf<IInterface*>::Alloc(size);
    for (Int32 i = 0; i < size; ++i) {
        AutoPtr<IInterface> elem;
        p2->Get(i, (IInterface**)&elem);
        sencond->Set(i, elem);
    }
    _DeepEquals(first, sencond, &result);
    return result;
}

ECode CArrays::_SortByte(
    /* [in] */ ArrayOf<Byte>* array)
{
    return DualPivotQuicksort::Sort(array);
}

ECode CArrays::_SortByteEx(
    /* [in] */ ArrayOf<Byte>* array,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return DualPivotQuicksort::Sort(array, start, end);
}

ECode CArrays::_CheckOffsetAndCount(
    /* [in] */ Int32 arrayLength,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count)
{
    if ((offset | count) < 0 || offset > arrayLength || arrayLength - offset < count) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    return NOERROR;
}

ECode CArrays::_CheckStartAndEnd(
    /* [in] */ Int32 len,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    if (start < 0 || end > len) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    if (start > end) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

ECode CArrays::_SortChar32(
    /* [in] */ ArrayOf<Char32>* array)
{
    return DualPivotQuicksort::Sort(array);
}

ECode CArrays::_SortChar32Ex(
    /* [in] */ ArrayOf<Char32>* array,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return DualPivotQuicksort::Sort(array, start, end);
}

ECode CArrays::_SortDouble(
    /* [in] */ ArrayOf<Double>* array)
{
    return DualPivotQuicksort::Sort(array);
}

ECode CArrays::_SortDoubleEx(
    /* [in] */ ArrayOf<Double>* array,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return DualPivotQuicksort::Sort(array, start, end);
}

ECode CArrays::_SortFloat(
    /* [in] */ ArrayOf<Float>* array)
{
    return DualPivotQuicksort::Sort(array);
}

ECode CArrays::_SortFloatEx(
    /* [in] */ ArrayOf<Float>* array,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return DualPivotQuicksort::Sort(array, start, end);
}

ECode CArrays::_SortInt32(
    /* [in] */ ArrayOf<Int32>* array)
{
    return DualPivotQuicksort::Sort(array);
}

ECode CArrays::_SortInt32Ex(
    /* [in] */ ArrayOf<Int32>* array,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return DualPivotQuicksort::Sort(array, start, end);
}

ECode CArrays::_SortInt64(
    /* [in] */ ArrayOf<Int64>* array)
{
    return DualPivotQuicksort::Sort(array);
}

ECode CArrays::_SortInt64Ex(
    /* [in] */ ArrayOf<Int64>* array,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return DualPivotQuicksort::Sort(array, start, end);
}

ECode CArrays::_SortInt16(
    /* [in] */ ArrayOf<Int16>* array)
{
    return DualPivotQuicksort::Sort(array);
}

ECode CArrays::_SortInt16Ex(
    /* [in] */ ArrayOf<Int16>* array,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return DualPivotQuicksort::Sort(array, start, end);
}

ECode CArrays::_SortObject(
    /* [in] */ ArrayOf<IInterface* >* array)
{
    return ComparableTimSort::Sort(*array);
}

ECode CArrays::_SortObjectEx(
    /* [in] */ ArrayOf<IInterface* >* array,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return ComparableTimSort::SortEx(*array, start, end);
}

ECode CArrays::_SortEx(
    /* [in] */ ArrayOf<IInterface* >* array,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ IComparator* comparator)
{
    return TimSort::Sort(array, start, end, comparator);
}

ECode CArrays::_Sort(
    /* [in] */ ArrayOf<IInterface* >* array,
    /* [in] */ IComparator* comparator)
{
    return TimSort::Sort(array, comparator);
}

ECode CArrays::_ToStringBoolean(
    /* [in] */ ArrayOf<Boolean>* array,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    if (array == NULL) {
        *str = String("NULL");
        return NOERROR;
    }
    if (array->GetLength() == 0) {
        *str = String("[]");
        return NOERROR;
    }
    StringBuilder sb(array->GetLength() * 7);
    sb.AppendChar('[');
    sb.AppendBoolean((*array)[0]);
    for (Int32 i = 1; i < array->GetLength(); i++) {
        sb.AppendCStr(", ");
        sb.AppendBoolean((*array)[i]);
    }
    sb.AppendChar(']');
    *str = sb.ToString();
    return NOERROR;
}

ECode CArrays::_ToStringByte(
    /* [in] */ ArrayOf<Byte>* array,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    if (array == NULL) {
        *str = String("NULL");
        return NOERROR;
    }
    if (array->GetLength() == 0) {
        *str = String("[]");
        return NOERROR;
    }
    StringBuilder sb(array->GetLength() * 6);
    sb.AppendChar('[');
    sb.AppendInt32((*array)[0]);
    for (Int32 i = 1; i < array->GetLength(); i++) {
        sb.AppendCStr(", ");
        sb.AppendInt32((*array)[i]);
    }
    sb.AppendChar(']');
    *str = sb.ToString();
    return NOERROR;
}

ECode CArrays::_ToStringChar32(
    /* [in] */ ArrayOf<Char32>* array,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    if (array == NULL) {
        *str = String("NULL");
        return NOERROR;
    }
    if (array->GetLength() == 0) {
        *str = String("[]");
        return NOERROR;
    }
    StringBuilder sb(array->GetLength() * 3);
    sb.AppendChar('[');
    sb.AppendChar((*array)[0]);
    for (Int32 i = 1; i < array->GetLength(); i++) {
        sb.AppendCStr(", ");
        sb.AppendChar((*array)[i]);
    }
    sb.AppendChar(']');
    *str = sb.ToString();
    return NOERROR;
}

ECode CArrays::_ToStringDouble(
    /* [in] */ ArrayOf<Double>* array,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    if (array == NULL) {
        *str = String("NULL");
        return NOERROR;
    }
    if (array->GetLength() == 0) {
        *str = String("[]");
        return NOERROR;
    }
    StringBuilder sb(array->GetLength() * 7);
    sb.AppendChar('[');
    sb.AppendDouble((*array)[0]);
    for (Int32 i = 1; i < array->GetLength(); i++) {
        sb.AppendCStr(", ");
        sb.AppendDouble((*array)[i]);
    }
    sb.AppendChar(']');
    *str = sb.ToString();
    return NOERROR;
}

ECode CArrays::_ToStringFloat(
    /* [in] */ ArrayOf<Float>* array,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    if (array == NULL) {
        *str = String("NULL");
        return NOERROR;
    }
    if (array->GetLength() == 0) {
        *str = String("[]");
        return NOERROR;
    }
    StringBuilder sb(array->GetLength() * 7);
    sb.AppendChar('[');
    sb.AppendFloat((*array)[0]);
    for (Int32 i = 1; i < array->GetLength(); i++) {
        sb.AppendCStr(", ");
        sb.AppendFloat((*array)[i]);
    }
    sb.AppendChar(']');
    *str = sb.ToString();
    return NOERROR;
}

ECode CArrays::_ToStringInt32(
    /* [in] */ ArrayOf<Int32>* array,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    if (array == NULL) {
        *str = String("NULL");
        return NOERROR;
    }
    if (array->GetLength() == 0) {
        *str = String("[]");
        return NOERROR;
    }
    StringBuilder sb(array->GetLength() * 6);
    sb.AppendChar('[');
    sb.AppendInt32((*array)[0]);
    for (Int32 i = 1; i < array->GetLength(); i++) {
        sb.AppendCStr(", ");
        sb.AppendInt32((*array)[i]);
    }
    sb.AppendChar(']');
    *str = sb.ToString();
    return NOERROR;
}

ECode CArrays::_ToStringInt64(
    /* [in] */ ArrayOf<Int64>* array,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    if (array == NULL) {
        *str = String("NULL");
        return NOERROR;
    }
    if (array->GetLength() == 0) {
        *str = String("[]");
        return NOERROR;
    }
    StringBuilder sb(array->GetLength() * 6);
    sb.AppendChar('[');
    sb.AppendInt64((*array)[0]);
    for (Int32 i = 1; i < array->GetLength(); i++) {
        sb.AppendCStr(", ");
        sb.AppendInt64((*array)[i]);
    }
    sb.AppendChar(']');
    *str = sb.ToString();
    return NOERROR;
}

ECode CArrays::_ToStringInt16(
    /* [in] */ ArrayOf<Int16>* array,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    if (array == NULL) {
        *str = String("NULL");
        return NOERROR;
    }
    if (array->GetLength() == 0) {
        *str = String("[]");
        return NOERROR;
    }
    StringBuilder sb(array->GetLength() * 6);
    sb.AppendChar('[');
    sb.AppendInt32((*array)[0]);
    for (Int32 i = 1; i < array->GetLength(); i++) {
        sb.AppendCStr(", ");
        sb.AppendInt32((*array)[i]);
    }
    sb.AppendChar(']');
    *str = sb.ToString();
    return NOERROR;
}

ECode CArrays::_ToStringObject(
    /* [in] */ ArrayOf<IInterface* >* array,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    if (array == NULL) {
        *str = String("NULL");
        return NOERROR;
    }
    if (array->GetLength() == 0) {
        *str = String("[]");
        return NOERROR;
    }
    StringBuilder sb(array->GetLength() * 7);
    sb.AppendChar('[');
    sb.AppendObject((*array)[0]);
    for (Int32 i = 1; i < array->GetLength(); i++) {
        sb.AppendCStr(", ");
        sb.AppendObject((*array)[i]);
    }
    sb.AppendChar(']');
    *str = sb.ToString();
    return NOERROR;
}

ECode CArrays::_DeepToString(
    /* [in] */ ArrayOf<IInterface* >* array,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    // Special case null to prevent NPE
    if (array == NULL) {
        *str = String("NULL");
        return NOERROR;
    }
    // delegate this to the recursive method
    StringBuilder buf(array->GetLength() * 9);
    AutoPtr<ArrayOf<IInterface*> > origArrays = ArrayOf<IInterface*>::Alloc(1);
    AutoPtr<IArrayOf> pArray;
    CArrayOf::New(EIID_IInterface, array->GetLength(), (IArrayOf**)&pArray);
    for (Int32 i = 0; i < array->GetLength(); ++i) {
        pArray->Put(i, (*array)[i]);
    }
    origArrays->Set(0, pArray);
    DeepToStringImpl(array, origArrays, &buf);
    *str = buf.ToString();
    return NOERROR;
}

void CArrays::DeepToStringImpl(
    /* [in] */ ArrayOf<IInterface* >* array,
    /* [in] */ ArrayOf<IInterface* >* origArrays,
    /* [in] */ StringBuilder* sb)
{
    if (array == NULL) {
        sb->AppendCStr("NULL");
        return;
    }

    sb->AppendChar('[');

    for (Int32 i = 0; i < array->GetLength(); i++) {
        if (i != 0) {
            sb->AppendCStr(", ");
        }
        // establish current element
        AutoPtr<IInterface> elem = (*array)[i];
        if (elem == NULL) {
            // element is null
            sb->AppendCStr("NULL");
        }
        else {
            // get the Class of the current element
            if (IArrayOf::Probe(elem) != NULL) {
                // element is an array type

                // get the declared Class of the array (element)
                AutoPtr<IArrayOf> p = IArrayOf::Probe(elem);
                InterfaceID riid;
                p->GetTypeId(&riid);
                Int32 length;
                p->GetLength(&length);
                String str;
                // element is a primitive array
                if (riid == EIID_IBoolean) {
                    AutoPtr<ArrayOf<Boolean> > tmpArray = ArrayOf<Boolean>::Alloc(length);
                    for (Int32 i = 0; i < length; ++i) {
                        AutoPtr<IBoolean> pContent;
                        p->Get(i, (IInterface**)&pContent);
                        Boolean content;
                        pContent->GetValue(&content);
                        tmpArray->Set(i, content);
                    }
                    _ToStringBoolean((ArrayOf<Boolean>*) tmpArray, &str);
                    sb->AppendString(str);
                }
                else if (riid == EIID_IByte) {
                    AutoPtr<ArrayOf<Byte> > tmpArray = ArrayOf<Byte>::Alloc(length);
                    for (Int32 i = 0; i < length; ++i) {
                        AutoPtr<IByte> pContent;
                        p->Get(i, (IInterface**)&pContent);
                        Byte content;
                        pContent->GetValue(&content);
                        tmpArray->Set(i, content);
                    }
                    _ToStringByte((ArrayOf<Byte>*) tmpArray, &str);
                    sb->AppendString(str);
                }
                else if (riid == EIID_IChar32) {
                    AutoPtr<ArrayOf<Char32> > tmpArray = ArrayOf<Char32>::Alloc(length);
                    for (Int32 i = 0; i < length; ++i) {
                        AutoPtr<IChar32> pContent;
                        p->Get(i, (IInterface**)&pContent);
                        Char32 content;
                        pContent->GetValue(&content);
                        tmpArray->Set(i, content);
                    }
                    _ToStringChar32((ArrayOf<Char32>*) tmpArray, &str);
                    sb->AppendString(str);
                }
                else if (riid == EIID_IDouble) {
                    AutoPtr<ArrayOf<Double> > tmpArray = ArrayOf<Double>::Alloc(length);
                    for (Int32 i = 0; i < length; ++i) {
                        AutoPtr<IDouble> pContent;
                        p->Get(i, (IInterface**)&pContent);
                        Double content;
                        pContent->GetValue(&content);
                        tmpArray->Set(i, content);
                    }
                    _ToStringDouble((ArrayOf<Double>*) tmpArray, &str);
                    sb->AppendString(str);
                }
                else if (riid == EIID_IFloat) {
                    AutoPtr<ArrayOf<Float> > tmpArray = ArrayOf<Float>::Alloc(length);
                    for (Int32 i = 0; i < length; ++i) {
                        AutoPtr<IFloat> pContent;
                        p->Get(i, (IInterface**)&pContent);
                        Float content;
                        pContent->GetValue(&content);
                        tmpArray->Set(i, content);
                    }
                    _ToStringFloat((ArrayOf<Float>*) tmpArray, &str);
                    sb->AppendString(str);
                }
                else if (riid == EIID_IInteger32) {
                    AutoPtr<ArrayOf<Int32> > tmpArray = ArrayOf<Int32>::Alloc(length);
                    for (Int32 i = 0; i < length; ++i) {
                        AutoPtr<IInteger32> pContent;
                        p->Get(i, (IInterface**)&pContent);
                        Int32 content;
                        pContent->GetValue(&content);
                        tmpArray->Set(i, content);
                    }
                    _ToStringInt32((ArrayOf<Int32>*) tmpArray, &str);
                    sb->AppendString(str);
                }
                else if (riid == EIID_IInteger64) {
                    AutoPtr<ArrayOf<Int64> > tmpArray = ArrayOf<Int64>::Alloc(length);
                    for (Int32 i = 0; i < length; ++i) {
                        AutoPtr<IInteger64> pContent;
                        p->Get(i, (IInterface**)&pContent);
                        Int64 content;
                        pContent->GetValue(&content);
                        tmpArray->Set(i, content);
                    }
                    _ToStringInt64((ArrayOf<Int64>*) tmpArray, &str);
                    sb->AppendString(str);
                }
                else if (riid == EIID_IInteger16) {
                    AutoPtr<ArrayOf<Int16> > tmpArray = ArrayOf<Int16>::Alloc(length);
                    for (Int32 i = 0; i < length; ++i) {
                        AutoPtr<IInteger16> pContent;
                        p->Get(i, (IInterface**)&pContent);
                        Int16 content;
                        pContent->GetValue(&content);
                        tmpArray->Set(i, content);
                    }
                    _ToStringInt16((ArrayOf<Int16>*) tmpArray, &str);
                    sb->AppendString(str);
                }
                else if (riid == EIID_IInterface) {
                    // element is an Object[], so we assert that
                    //assert elem instanceof Object[];
                    if (DeepToStringImplContains(origArrays, elem)) {
                        sb->AppendCStr("[...]");
                    }
                    else {
                        AutoPtr<ArrayOf<IInterface*> > newArray = ArrayOf<IInterface*>::Alloc(length);
                        for (Int32 i = 0; i < length; ++i) {
                            AutoPtr<IInterface> pContent;
                            p->Get(i, (IInterface**)&pContent);
                            newArray->Set(i, pContent);
                        }
                        AutoPtr<ArrayOf<IInterface*> > newOrigArrays = ArrayOf<IInterface*>::Alloc(origArrays->GetLength() + 1);
                        newOrigArrays->Copy(origArrays, origArrays->GetLength());
                        newOrigArrays->Set(origArrays->GetLength(), elem);
                        // make the recursive call to this method
                        DeepToStringImpl(newArray, newOrigArrays, sb);
                    }
                }
                // else {
                //     // no other possible primitives, so we assert that
                //     throw new AssertionError();
                // }
            }
            else { // element is NOT an array, just an Object
                sb->AppendObject((*array)[i]);
            }
        }
    }
    sb->AppendChar(']');
}

Boolean CArrays::DeepToStringImplContains(
    /* [in] */ ArrayOf<IInterface* >* origArrays,
    /* [in] */ IInterface* array)
{
    if (origArrays == NULL || origArrays->GetLength() == 0) {
        return FALSE;
    }
    for (Int32 i = 0; i < origArrays->GetLength(); i++) {
        AutoPtr<IInterface> element = (*origArrays)[i];
        if (element.Get() == array) {
            return TRUE;
        }
    }
    return FALSE;
}

ECode CArrays::_CopyOfBoolean(
    /* [in] */ ArrayOf<Boolean>* original,
    /* [in] */ Int32 newLength,
    /* [out, callee] */ ArrayOf<Boolean>** result)
{
    VALIDATE_NOT_NULL(result)
    if (newLength < 0) {
        return E_NEGATIVE_ARRAY_SIZE_EXCEPTION;
    }
    return _CopyOfRangeBoolean(original, 0, newLength, result);
}

ECode CArrays::_CopyOfByte(
    /* [in] */ ArrayOf<Byte>* original,
    /* [in] */ Int32 newLength,
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    VALIDATE_NOT_NULL(result)
    if (newLength < 0) {
        return E_NEGATIVE_ARRAY_SIZE_EXCEPTION;
    }
    return _CopyOfRangeByte(original, 0, newLength, result);
}

ECode CArrays::_CopyOfChar32(
    /* [in] */ ArrayOf<Char32>* original,
    /* [in] */ Int32 newLength,
    /* [out, callee] */ ArrayOf<Char32>** result)
{
    VALIDATE_NOT_NULL(result)
    if (newLength < 0) {
        return E_NEGATIVE_ARRAY_SIZE_EXCEPTION;
    }
    return _CopyOfRangeChar32(original, 0, newLength, result);
}

ECode CArrays::_CopyOfDouble(
    /* [in] */ ArrayOf<Double>* original,
    /* [in] */ Int32 newLength,
    /* [out, callee] */ ArrayOf<Double>** result)
{
    VALIDATE_NOT_NULL(result)
    if (newLength < 0) {
        return E_NEGATIVE_ARRAY_SIZE_EXCEPTION;
    }
    return _CopyOfRangeDouble(original, 0, newLength, result);
}

ECode CArrays::_CopyOfFloat(
    /* [in] */ ArrayOf<Float>* original,
    /* [in] */ Int32 newLength,
    /* [out, callee] */ ArrayOf<Float>** result)
{
    VALIDATE_NOT_NULL(result)
    if (newLength < 0) {
        return E_NEGATIVE_ARRAY_SIZE_EXCEPTION;
    }
    return _CopyOfRangeFloat(original, 0, newLength, result);
}

ECode CArrays::_CopyOfInt32(
    /* [in] */ ArrayOf<Int32>* original,
    /* [in] */ Int32 newLength,
    /* [out, callee] */ ArrayOf<Int32>** result)
{
    VALIDATE_NOT_NULL(result)
    if (newLength < 0) {
        return E_NEGATIVE_ARRAY_SIZE_EXCEPTION;
    }
    return _CopyOfRangeInt32(original, 0, newLength, result);
}

ECode CArrays::_CopyOfInt64(
    /* [in] */ ArrayOf<Int64>* original,
    /* [in] */ Int32 newLength,
    /* [out, callee] */ ArrayOf<Int64>** result)
{
    VALIDATE_NOT_NULL(result)
    if (newLength < 0) {
        return E_NEGATIVE_ARRAY_SIZE_EXCEPTION;
    }
    return _CopyOfRangeInt64(original, 0, newLength, result);
}

ECode CArrays::_CopyOfInt16(
    /* [in] */ ArrayOf<Int16>* original,
    /* [in] */ Int32 newLength,
    /* [out, callee] */ ArrayOf<Int16>** result)
{
    VALIDATE_NOT_NULL(result)
    if (newLength < 0) {
        return E_NEGATIVE_ARRAY_SIZE_EXCEPTION;
    }
    return _CopyOfRangeInt16(original, 0, newLength, result);
}

ECode CArrays::_CopyOf(
    /* [in] */ ArrayOf<IInterface* >* original,
    /* [in] */ Int32 newLength,
    /* [out, callee] */ ArrayOf<IInterface* >** result)
{
    VALIDATE_NOT_NULL(result)
    if (original == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }
    if (newLength < 0) {
        return E_NEGATIVE_ARRAY_SIZE_EXCEPTION;
    }
    return _CopyOfRange(original, 0, newLength, result);
}

// ECode CArrays::_CopyOfEx(
//     /* [in] */ U[] original,
//     /* [in] */ Int32 newLength,
//     /* [in] */ Class<? extends T[]> newType,
//     /* [out, callee] */ <T, U> T[] result)
// {
//     // We use the null pointer check in copyOfRange for exception priority compatibility.
//     if (newLength < 0) {
//         return E_NEGATIVE_ARRAY_SIZE_EXCEPTION;
//     }
//     return _CopyOfRangeEx(original, 0, newLength, newType, result);
// }

ECode CArrays::_CopyOfRangeBoolean(
    /* [in] */ ArrayOf<Boolean>* original,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out, callee] */ ArrayOf<Boolean>** arrayCopy)
{
    VALIDATE_NOT_NULL(arrayCopy)
    if (start > end) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Int32 originalLength = original->GetLength();
    if (start < 0 || start > originalLength) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    Int32 resultLength = end - start;
    Int32 copyLength = Elastos::Core::Math::Min(resultLength, originalLength - start);
    AutoPtr<ArrayOf<Boolean> > result = ArrayOf<Boolean>::Alloc(resultLength);
    result->Copy(0, original, start, copyLength);
    *arrayCopy = result;
    INTERFACE_ADDREF(*arrayCopy)
    return NOERROR;
}

ECode CArrays::_CopyOfRangeByte(
    /* [in] */ ArrayOf<Byte>* original,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out, callee] */ ArrayOf<Byte>** arrayCopy)
{
    VALIDATE_NOT_NULL(arrayCopy)
    if (start > end) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Int32 originalLength = original->GetLength();
    if (start < 0 || start > originalLength) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    Int32 resultLength = end - start;
    Int32 copyLength = Elastos::Core::Math::Min(resultLength, originalLength - start);
    AutoPtr<ArrayOf<Byte> > result = ArrayOf<Byte>::Alloc(resultLength);
    result->Copy(0, original, start, copyLength);
    *arrayCopy = result;
    INTERFACE_ADDREF(*arrayCopy)
    return NOERROR;
}

ECode CArrays::_CopyOfRangeChar32(
    /* [in] */ ArrayOf<Char32>* original,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out, callee] */ ArrayOf<Char32>** arrayCopy)
{
    VALIDATE_NOT_NULL(arrayCopy)
    if (start > end) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Int32 originalLength = original->GetLength();
    if (start < 0 || start > originalLength) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    Int32 resultLength = end - start;
    Int32 copyLength = Elastos::Core::Math::Min(resultLength, originalLength - start);
    AutoPtr<ArrayOf<Char32> > result = ArrayOf<Char32>::Alloc(resultLength);
    result->Copy(0, original, start, copyLength);
    *arrayCopy = result;
    INTERFACE_ADDREF(*arrayCopy)
    return NOERROR;
}

ECode CArrays::_CopyOfRangeDouble(
    /* [in] */ ArrayOf<Double>* original,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out, callee] */ ArrayOf<Double>** arrayCopy)
{
    VALIDATE_NOT_NULL(arrayCopy)
    if (start > end) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Int32 originalLength = original->GetLength();
    if (start < 0 || start > originalLength) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    Int32 resultLength = end - start;
    Int32 copyLength = Elastos::Core::Math::Min(resultLength, originalLength - start);
    AutoPtr<ArrayOf<Double> > result = ArrayOf<Double>::Alloc(resultLength);
    result->Copy(0, original, start, copyLength);
    *arrayCopy = result;
    INTERFACE_ADDREF(*arrayCopy)
    return NOERROR;
}

ECode CArrays::_CopyOfRangeFloat(
    /* [in] */ ArrayOf<Float>* original,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out, callee] */ ArrayOf<Float>** arrayCopy)
{
    VALIDATE_NOT_NULL(arrayCopy)
    if (start > end) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Int32 originalLength = original->GetLength();
    if (start < 0 || start > originalLength) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    Int32 resultLength = end - start;
    Int32 copyLength = Elastos::Core::Math::Min(resultLength, originalLength - start);
    AutoPtr<ArrayOf<Float> > result = ArrayOf<Float>::Alloc(resultLength);
    result->Copy(0, original, start, copyLength);
    *arrayCopy = result;
    INTERFACE_ADDREF(*arrayCopy)
    return NOERROR;
}

ECode CArrays::_CopyOfRangeInt32(
    /* [in] */ ArrayOf<Int32>* original,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out, callee] */ ArrayOf<Int32>** arrayCopy)
{
    VALIDATE_NOT_NULL(arrayCopy)
    if (start > end) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Int32 originalLength = original->GetLength();
    if (start < 0 || start > originalLength) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    Int32 resultLength = end - start;
    Int32 copyLength = Elastos::Core::Math::Min(resultLength, originalLength - start);
    AutoPtr<ArrayOf<Int32> > result = ArrayOf<Int32>::Alloc(resultLength);
    result->Copy(0, original, start, copyLength);
    *arrayCopy = result;
    INTERFACE_ADDREF(*arrayCopy)
    return NOERROR;
}

ECode CArrays::_CopyOfRangeInt64(
    /* [in] */ ArrayOf<Int64>* original,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out, callee] */ ArrayOf<Int64>** arrayCopy)
{
    VALIDATE_NOT_NULL(arrayCopy)
    if (start > end) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Int32 originalLength = original->GetLength();
    if (start < 0 || start > originalLength) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    Int32 resultLength = end - start;
    Int32 copyLength = Elastos::Core::Math::Min(resultLength, originalLength - start);
    AutoPtr<ArrayOf<Int64> > result = ArrayOf<Int64>::Alloc(resultLength);
    result->Copy(0, original, start, copyLength);
    *arrayCopy = result;
    INTERFACE_ADDREF(*arrayCopy)
    return NOERROR;
}

ECode CArrays::_CopyOfRangeInt16(
    /* [in] */ ArrayOf<Int16>* original,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out, callee] */ ArrayOf<Int16>** arrayCopy)
{
    VALIDATE_NOT_NULL(arrayCopy)
    if (start > end) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Int32 originalLength = original->GetLength();
    if (start < 0 || start > originalLength) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    Int32 resultLength = end - start;
    Int32 copyLength = Elastos::Core::Math::Min(resultLength, originalLength - start);
    AutoPtr<ArrayOf<Int16> > result = ArrayOf<Int16>::Alloc(resultLength);
    result->Copy(0, original, start, copyLength);
    *arrayCopy = result;
    INTERFACE_ADDREF(*arrayCopy)
    return NOERROR;
}

ECode CArrays::_CopyOfRange(
    /* [in] */ ArrayOf<IInterface* >* original,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out, callee] */ ArrayOf<IInterface* >** arrayCopy)
{
    VALIDATE_NOT_NULL(arrayCopy)
    Int32 originalLength = original->GetLength(); // For exception priority compatibility.
    if (start > end) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (start < 0 || start > originalLength) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    Int32 resultLength = end - start;
    Int32 copyLength = Elastos::Core::Math::Min(resultLength, originalLength - start);
    AutoPtr<ArrayOf<IInterface*> > result = ArrayOf<IInterface*>::Alloc(resultLength);
    result->Copy(0, original, start, copyLength);
    *arrayCopy = result;
    INTERFACE_ADDREF(*arrayCopy)
    return NOERROR;
}

// ECode CArrays::_CopyOfRangeEx(
//     /* [in] */ U[] original,
//     /* [in] */ Int32 start,
//     /* [in] */ Int32 end,
//     /* [in] */ Class<? extends T[]> newType,
//     /* [out, callee] */ <T, U> T[] arrayCopy)
// {
//     if (start > end) {
//         return E_ILLEGAL_ARGUMENT_EXCEPTION;
//     }
//     Int32 originalLength = original->GetLength();
//     if (start < 0 || start > originalLength) {
//         return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//     }
//     Int32 resultLength = end - start;
//     Int32 copyLength = Elastos::Core::Math::Min(resultLength, originalLength - start);
//     AutoPtr<ArrayOf<T> > result = ArrayOf<T>::Alloc(resultLength);
//     result->Copy(0, original, start, copyLength);
//     *arrayCopy = result;
//     INTERFACE_ADDREF(*arrayCopy)
//     return NOERROR;
// }

ECode CArrays::AsList(
    /* [in] */ ArrayOf<IInterface*>* array,
    /* [out] */ IList** outList)
{
    return _AsList(array, outList);
}

ECode CArrays::BinarySearchByte(
    /* [in] */ ArrayOf<Byte>* array,
    /* [in] */ Byte value,
    /* [out] */ Int32* index)
{
    return _BinarySearchByte(array, value, index);
}

ECode CArrays::BinarySearchByteEx(
    /* [in] */ ArrayOf<Byte>* array,
    /* [in] */ Int32 startIndex,
    /* [in] */ Int32 endIndex,
    /* [in] */ Byte value,
    /* [out] */ Int32* index)
{
    return _BinarySearchByteEx(array, startIndex, endIndex, value, index);
}

ECode CArrays::BinarySearchChar32(
    /* [in] */ ArrayOf<Char32>* array,
    /* [in] */ Char32 value,
    /* [out] */ Int32* index)
{
    return _BinarySearchChar32(array, value, index);
}

ECode CArrays::BinarySearchChar32Ex(
    /* [in] */ ArrayOf<Char32>* array,
    /* [in] */ Int32 startIndex,
    /* [in] */ Int32 endIndex,
    /* [in] */ Char32 value,
    /* [out] */ Int32* index)
{
    return _BinarySearchChar32Ex(array, startIndex, endIndex, value, index);
}

ECode CArrays::BinarySearchDouble(
    /* [in] */ ArrayOf<Double>* array,
    /* [in] */ Double value,
    /* [out] */ Int32* index)
{
    return _BinarySearchDouble(array, value, index);
}

ECode CArrays::BinarySearchDoubleEx(
    /* [in] */ ArrayOf<Double>* array,
    /* [in] */ Int32 startIndex,
    /* [in] */ Int32 endIndex,
    /* [in] */ Double value,
    /* [out] */ Int32* index)
{
    return _BinarySearchDoubleEx(array, startIndex, endIndex, value, index);
}

ECode CArrays::BinarySearchFloat(
    /* [in] */ ArrayOf<Float>* array,
    /* [in] */ Float value,
    /* [out] */ Int32* index)
{
    return _BinarySearchFloat(array, value, index);
}

ECode CArrays::BinarySearchFloatEx(
    /* [in] */ ArrayOf<Float>* array,
    /* [in] */ Int32 startIndex,
    /* [in] */ Int32 endIndex,
    /* [in] */ Float value,
    /* [out] */ Int32* index)
{
    return _BinarySearchFloatEx(array, startIndex, endIndex, value, index);
}

ECode CArrays::BinarySearchInt32(
    /* [in] */ ArrayOf<Int32>* array,
    /* [in] */ Int32 value,
    /* [out] */ Int32* index)
{
    return _BinarySearchInt32(array, value, index);
}

ECode CArrays::BinarySearchInt32Ex(
    /* [in] */ ArrayOf<Int32>* array,
    /* [in] */ Int32 startIndex,
    /* [in] */ Int32 endIndex,
    /* [in] */ Int32 value,
    /* [out] */ Int32* index)
{
    return _BinarySearchInt32Ex(array, startIndex, endIndex, value, index);
}

ECode CArrays::BinarySearchInt64(
    /* [in] */ ArrayOf<Int64>* array,
    /* [in] */ Int64 value,
    /* [out] */ Int32* index)
{
    return _BinarySearchInt64(array, value, index);
}

ECode CArrays::BinarySearchInt64Ex(
    /* [in] */ ArrayOf<Int64>* array,
    /* [in] */ Int32 startIndex,
    /* [in] */ Int32 endIndex,
    /* [in] */ Int64 value,
    /* [out] */ Int32* index)
{
    return _BinarySearchInt64Ex(array, startIndex, endIndex, value, index);
}

ECode CArrays::BinarySearchObject(
    /* [in] */ ArrayOf<IInterface* >* array,
    /* [in] */ IInterface* value,
    /* [out] */ Int32* index)
{
    return _BinarySearchObject(array, value, index);
}

ECode CArrays::BinarySearchObjectEx(
    /* [in] */ ArrayOf<IInterface* >* array,
    /* [in] */ Int32 startIndex,
    /* [in] */ Int32 endIndex,
    /* [in] */ IInterface* value,
    /* [out] */ Int32* index)
{
    return _BinarySearchObjectEx(array, startIndex, endIndex, value, index);
}

ECode CArrays::BinarySearch(
    /* [in] */ ArrayOf<IInterface* >* array,
    /* [in] */ IInterface* value,
    /* [in] */ IComparator* comparator,
    /* [out] */ Int32* index)
{
    return _BinarySearch(array, value, comparator, index);
}

ECode CArrays::BinarySearchEx(
    /* [in] */ ArrayOf<IInterface* >* array,
    /* [in] */ Int32 startIndex,
    /* [in] */ Int32 endIndex,
    /* [in] */ IInterface* value,
    /* [in] */ IComparator* comparator,
    /* [out] */ Int32* index)
{
    return _BinarySearchEx(array, startIndex, endIndex, value, comparator, index);
}

ECode CArrays::BinarySearchInt16(
    /* [in] */ ArrayOf<Int16>* array,
    /* [in] */ Int16 value,
    /* [out] */ Int32* index)
{
    return _BinarySearchInt16(array, value, index);
}

ECode CArrays::BinarySearchInt16Ex(
    /* [in] */ ArrayOf<Int16>* array,
    /* [in] */ Int32 startIndex,
    /* [in] */ Int32 endIndex,
    /* [in] */ Int16 value,
    /* [out] */ Int32* index)
{
    return _BinarySearchInt16Ex(array, startIndex, endIndex, value, index);
}

ECode CArrays::FillByte(
    /* [in] */ ArrayOf<Byte>* array,
    /* [in] */ Byte value)
{
    return _FillByte(array, value);
}

ECode CArrays::FillByteEx(
    /* [in] */ ArrayOf<Byte>* array,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Byte value)
{
    return _FillByteEx(array, start, end, value);
}

ECode CArrays::FillInt16(
    /* [in] */ ArrayOf<Int16>* array,
    /* [in] */ Int16 value)
{
    return _FillInt16(array, value);
}

ECode CArrays::FillInt16Ex(
    /* [in] */ ArrayOf<Int16>* array,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int16 value)
{
    return _FillInt16Ex(array, start, end, value);
}

ECode CArrays::FillChar32(
    /* [in] */ ArrayOf<Char32>* array,
    /* [in] */ Char32 value)
{
    return _FillChar32(array, value);
}

ECode CArrays::FillChar32Ex(
    /* [in] */ ArrayOf<Char32>* array,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Char32 value)
{
    return _FillChar32Ex(array, start, end, value);
}

ECode CArrays::FillInt32(
    /* [in] */ ArrayOf<Int32>* array,
    /* [in] */ Int32 value)
{
    return _FillInt32(array, value);
}

ECode CArrays::FillInt32Ex(
    /* [in] */ ArrayOf<Int32>* array,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 value)
{
    return _FillInt32Ex(array, start, end, value);
}

ECode CArrays::FillInt64(
    /* [in] */ ArrayOf<Int64>* array,
    /* [in] */ Int64 value)
{
    return _FillInt64(array, value);
}

ECode CArrays::FillInt64Ex(
    /* [in] */ ArrayOf<Int64>* array,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int64 value)
{
    return _FillInt64Ex(array, start, end, value);
}

ECode CArrays::FillFloat(
    /* [in] */ ArrayOf<Float>* array,
    /* [in] */ Float value)
{
    return _FillFloat(array, value);
}

ECode CArrays::FillFloatEx(
    /* [in] */ ArrayOf<Float>* array,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Float value)
{
    return _FillFloatEx(array, start, end, value);
}

ECode CArrays::FillDouble(
    /* [in] */ ArrayOf<Double>* array,
    /* [in] */ Double value)
{
    return _FillDouble(array, value);
}

ECode CArrays::FillDoubleEx(
    /* [in] */ ArrayOf<Double>* array,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Double value)
{
    return _FillDoubleEx(array, start, end, value);
}

ECode CArrays::FillBoolean(
    /* [in] */ ArrayOf<Boolean>* array,
    /* [in] */ Boolean value)
{
    return _FillBoolean(array, value);
}

ECode CArrays::FillBooleanEx(
    /* [in] */ ArrayOf<Boolean>* array,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Boolean value)
{
    return _FillBooleanEx(array, start, end, value);
}

ECode CArrays::FillObject(
    /* [in] */ ArrayOf<IInterface* >* array,
    /* [in] */ IInterface* value)
{
    return _FillObject(array, value);
}

ECode CArrays::FillObjectEx(
    /* [in] */ ArrayOf<IInterface* >* array,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ IInterface* value)
{
    return _FillObjectEx(array, start, end, value);
}

ECode CArrays::HashCodeBoolean(
    /* [in] */ ArrayOf<Boolean>* array,
    /* [out] */ Int32* code)
{
    return _HashCodeBoolean(array, code);
}

ECode CArrays::HashCodeInt32(
    /* [in] */ ArrayOf<Int32>* array,
    /* [out] */ Int32* code)
{
    return _HashCodeInt32(array, code);
}

ECode CArrays::HashCodeInt16(
    /* [in] */ ArrayOf<Int16>* array,
    /* [out] */ Int32* code)
{
    return _HashCodeInt16(array, code);
}

ECode CArrays::HashCodeChar32(
    /* [in] */ ArrayOf<Char32>* array,
    /* [out] */ Int32* code)
{
    return _HashCodeChar32(array, code);
}

ECode CArrays::HashCodeByte(
    /* [in] */ ArrayOf<Byte>* array,
    /* [out] */ Int32* code)
{
    return _HashCodeByte(array, code);
}

ECode CArrays::HashCodeInt64(
    /* [in] */ ArrayOf<Int64>* array,
    /* [out] */ Int32* code)
{
    return _HashCodeInt64(array, code);
}

ECode CArrays::HashCodeFloat(
    /* [in] */ ArrayOf<Float>* array,
    /* [out] */ Int32* code)
{
    return _HashCodeFloat(array, code);
}

ECode CArrays::HashCodeDouble(
    /* [in] */ ArrayOf<Double>* array,
    /* [out] */ Int32* code)
{
    return _HashCodeDouble(array, code);
}

ECode CArrays::HashCodeObject(
    /* [in] */ ArrayOf<IInterface* >* array,
    /* [out] */ Int32* code)
{
    return _HashCodeObject(array, code);
}

ECode CArrays::DeepHashCode(
    /* [in] */ ArrayOf<IInterface* >* array,
    /* [out] */ Int32* code)
{
    return _DeepHashCode(array, code);
}

ECode CArrays::EqualsByte(
    /* [in] */ ArrayOf<Byte>* array1,
    /* [in] */ ArrayOf<Byte>* array2,
    /* [out] */ Boolean* result)
{
    return _EqualsByte(array1, array2, result);
}

ECode CArrays::EqualsInt16(
    /* [in] */ ArrayOf<Int16>* array1,
    /* [in] */ ArrayOf<Int16>* array2,
    /* [out] */ Boolean* result)
{
    return _EqualsInt16(array1, array2, result);
}

ECode CArrays::EqualsChar32(
    /* [in] */ ArrayOf<Char32>* array1,
    /* [in] */ ArrayOf<Char32>* array2,
    /* [out] */ Boolean* result)
{
    return _EqualsChar32(array1, array2, result);
}

ECode CArrays::EqualsInt32(
    /* [in] */ ArrayOf<Int32>* array1,
    /* [in] */ ArrayOf<Int32>* array2,
    /* [out] */ Boolean* result)
{
    return _EqualsInt32(array1, array2, result);
}

ECode CArrays::EqualsInt64(
    /* [in] */ ArrayOf<Int64>* array1,
    /* [in] */ ArrayOf<Int64>* array2,
    /* [out] */ Boolean* result)
{
    return _EqualsInt64(array1, array2, result);
}

ECode CArrays::EqualsFloat(
    /* [in] */ ArrayOf<Float>* array1,
    /* [in] */ ArrayOf<Float>* array2,
    /* [out] */ Boolean* result)
{
    return _EqualsFloat(array1, array2, result);
}

ECode CArrays::EqualsDouble(
    /* [in] */ ArrayOf<Double>* array1,
    /* [in] */ ArrayOf<Double>* array2,
    /* [out] */ Boolean* result)
{
    return _EqualsDouble(array1, array2, result);
}

ECode CArrays::EqualsBoolean(
    /* [in] */ ArrayOf<Boolean>* array1,
    /* [in] */ ArrayOf<Boolean>* array2,
    /* [out] */ Boolean* result)
{
    return _EqualsBoolean(array1, array2, result);
}

ECode CArrays::EqualsObject(
    /* [in] */ ArrayOf<IInterface* >* array1,
    /* [in] */ ArrayOf<IInterface* >* array2,
    /* [out] */ Boolean* result)
{
    return _EqualsObject(array1, array2, result);
}

ECode CArrays::DeepEquals(
    /* [in] */ ArrayOf<IInterface* >* array1,
    /* [in] */ ArrayOf<IInterface* >* array2,
    /* [out] */ Boolean* result)
{
    return _DeepEquals(array1, array2, result);
}

ECode CArrays::SortByte(
    /* [in] */ ArrayOf<Byte>* array)
{
    return _SortByte(array);
}

ECode CArrays::SortByteEx(
    /* [in] */ ArrayOf<Byte>* array,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return _SortByteEx(array, start, end);
}

ECode CArrays::CheckOffsetAndCount(
    /* [in] */ Int32 arrayLength,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count)
{
    return _CheckOffsetAndCount(arrayLength, offset, count);
}

ECode CArrays::CheckStartAndEnd(
    /* [in] */ Int32 len,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return _CheckStartAndEnd(len, start, end);
}

ECode CArrays::SortChar32(
    /* [in] */ ArrayOf<Char32>* array)
{
    return _SortChar32(array);
}

ECode CArrays::SortChar32Ex(
    /* [in] */ ArrayOf<Char32>* array,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return _SortChar32Ex(array, start, end);
}

ECode CArrays::SortDouble(
    /* [in] */ ArrayOf<Double>* array)
{
    return _SortDouble(array);
}

ECode CArrays::SortDoubleEx(
    /* [in] */ ArrayOf<Double>* array,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return _SortDoubleEx(array, start, end);
}

ECode CArrays::SortFloat(
    /* [in] */ ArrayOf<Float>* array)
{
    return _SortFloat(array);
}

ECode CArrays::SortFloatEx(
    /* [in] */ ArrayOf<Float>* array,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return _SortFloatEx(array, start, end);
}

ECode CArrays::SortInt32(
    /* [in] */ ArrayOf<Int32>* array)
{
    return _SortInt32(array);
}

ECode CArrays::SortInt32Ex(
    /* [in] */ ArrayOf<Int32>* array,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return _SortInt32Ex(array, start, end);
}

ECode CArrays::SortInt64(
    /* [in] */ ArrayOf<Int64>* array)
{
    return _SortInt64(array);
}

ECode CArrays::SortInt64Ex(
    /* [in] */ ArrayOf<Int64>* array,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return _SortInt64Ex(array, start, end);
}

ECode CArrays::SortInt16(
    /* [in] */ ArrayOf<Int16>* array)
{
    return _SortInt16(array);
}

ECode CArrays::SortInt16Ex(
    /* [in] */ ArrayOf<Int16>* array,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return _SortInt16Ex(array, start, end);
}

ECode CArrays::SortObject(
    /* [in] */ ArrayOf<IInterface* >* array)
{
    return _SortObject(array);
}

ECode CArrays::SortObjectEx(
    /* [in] */ ArrayOf<IInterface* >* array,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return _SortObjectEx(array, start, end);
}

ECode CArrays::SortEx(
    /* [in] */ ArrayOf<IInterface* >* array,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ IComparator* comparator)
{
    return _SortEx(array, start, end, comparator);
}

ECode CArrays::Sort(
    /* [in] */ ArrayOf<IInterface* >* array,
    /* [in] */ IComparator* comparator)
{
    return _Sort(array, comparator);
}

ECode CArrays::ToStringBoolean(
    /* [in] */ ArrayOf<Boolean>* array,
    /* [out] */ String* str)
{
    return _ToStringBoolean(array, str);
}

ECode CArrays::ToStringByte(
    /* [in] */ ArrayOf<Byte>* array,
    /* [out] */ String* str)
{
    return _ToStringByte(array, str);
}

ECode CArrays::ToStringChar32(
    /* [in] */ ArrayOf<Char32>* array,
    /* [out] */ String* str)
{
    return _ToStringChar32(array, str);
}

ECode CArrays::ToStringDouble(
    /* [in] */ ArrayOf<Double>* array,
    /* [out] */ String* str)
{
    return _ToStringDouble(array, str);
}

ECode CArrays::ToStringFloat(
    /* [in] */ ArrayOf<Float>* array,
    /* [out] */ String* str)
{
    return _ToStringFloat(array, str);
}

ECode CArrays::ToStringInt32(
    /* [in] */ ArrayOf<Int32>* array,
    /* [out] */ String* str)
{
    return _ToStringInt32(array, str);
}

ECode CArrays::ToStringInt64(
    /* [in] */ ArrayOf<Int64>* array,
    /* [out] */ String* str)
{
    return _ToStringInt64(array, str);
}

ECode CArrays::ToStringInt16(
    /* [in] */ ArrayOf<Int16>* array,
    /* [out] */ String* str)
{
    return _ToStringInt16(array, str);
}

ECode CArrays::ToStringObject(
    /* [in] */ ArrayOf<IInterface* >* array,
    /* [out] */ String* str)
{
    return _ToStringObject(array, str);
}

ECode CArrays::DeepToString(
    /* [in] */ ArrayOf<IInterface* >* array,
    /* [out] */ String* str)
{
    return _DeepToString(array, str);
}

ECode CArrays::CopyOfBoolean(
    /* [in] */ ArrayOf<Boolean>* original,
    /* [in] */ Int32 newLength,
    /* [out, callee] */ ArrayOf<Boolean>** result)
{
    return _CopyOfBoolean(original, newLength, result);
}

ECode CArrays::CopyOfByte(
    /* [in] */ ArrayOf<Byte>* original,
    /* [in] */ Int32 newLength,
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    return _CopyOfByte(original, newLength, result);
}

ECode CArrays::CopyOfChar32(
    /* [in] */ ArrayOf<Char32>* original,
    /* [in] */ Int32 newLength,
    /* [out, callee] */ ArrayOf<Char32>** result)
{
    return _CopyOfChar32(original, newLength, result);
}

ECode CArrays::CopyOfDouble(
    /* [in] */ ArrayOf<Double>* original,
    /* [in] */ Int32 newLength,
    /* [out, callee] */ ArrayOf<Double>** result)
{
    return _CopyOfDouble(original, newLength, result);
}

ECode CArrays::CopyOfFloat(
    /* [in] */ ArrayOf<Float>* original,
    /* [in] */ Int32 newLength,
    /* [out, callee] */ ArrayOf<Float>** result)
{
    return _CopyOfFloat(original, newLength, result);
}

ECode CArrays::CopyOfInt32(
    /* [in] */ ArrayOf<Int32>* original,
    /* [in] */ Int32 newLength,
    /* [out, callee] */ ArrayOf<Int32>** result)
{
    return _CopyOfInt32(original, newLength, result);
}

ECode CArrays::CopyOfInt64(
    /* [in] */ ArrayOf<Int64>* original,
    /* [in] */ Int32 newLength,
    /* [out, callee] */ ArrayOf<Int64>** result)
{
    return _CopyOfInt64(original, newLength, result);
}

ECode CArrays::CopyOfInt16(
    /* [in] */ ArrayOf<Int16>* original,
    /* [in] */ Int32 newLength,
    /* [out, callee] */ ArrayOf<Int16>** result)
{
    return _CopyOfInt16(original, newLength, result);
}

ECode CArrays::CopyOf(
    /* [in] */ ArrayOf<IInterface* >* original,
    /* [in] */ Int32 newLength,
    /* [out, callee] */ ArrayOf<IInterface* >** result)
{
    return _CopyOf(original, newLength, result);
}

// ECode CArrays::CopyOfEx(
//     /* [in] */ U[] original,
//     /* [in] */ Int32 newLength,
//     /* [in] */ Class<? extends T[]> newType,
//     /* [out, callee] */ <T, U> T[] result)
// {
//     return _CopyOfEx(original, newLength, newType, result);
// }

ECode CArrays::CopyOfRangeBoolean(
    /* [in] */ ArrayOf<Boolean>* original,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out, callee] */ ArrayOf<Boolean>** arrayCopy)
{
    return _CopyOfRangeBoolean(original, start, end, arrayCopy);
}

ECode CArrays::CopyOfRangeByte(
    /* [in] */ ArrayOf<Byte>* original,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out, callee] */ ArrayOf<Byte>** arrayCopy)
{
    return _CopyOfRangeByte(original, start, end, arrayCopy);
}

ECode CArrays::CopyOfRangeChar32(
    /* [in] */ ArrayOf<Char32>* original,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out, callee] */ ArrayOf<Char32>** arrayCopy)
{
    return _CopyOfRangeChar32(original, start, end, arrayCopy);
}

ECode CArrays::CopyOfRangeDouble(
    /* [in] */ ArrayOf<Double>* original,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out, callee] */ ArrayOf<Double>** arrayCopy)
{
    return _CopyOfRangeDouble(original, start, end, arrayCopy);
}

ECode CArrays::CopyOfRangeFloat(
    /* [in] */ ArrayOf<Float>* original,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out, callee] */ ArrayOf<Float>** arrayCopy)
{
    return _CopyOfRangeFloat(original, start, end, arrayCopy);
}

ECode CArrays::CopyOfRangeInt32(
    /* [in] */ ArrayOf<Int32>* original,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out, callee] */ ArrayOf<Int32>** arrayCopy)
{
    return _CopyOfRangeInt32(original, start, end, arrayCopy);
}

ECode CArrays::CopyOfRangeInt64(
    /* [in] */ ArrayOf<Int64>* original,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out, callee] */ ArrayOf<Int64>** arrayCopy)
{
    return _CopyOfRangeInt64(original, start, end, arrayCopy);
}

ECode CArrays::CopyOfRangeInt16(
    /* [in] */ ArrayOf<Int16>* original,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out, callee] */ ArrayOf<Int16>** arrayCopy)
{
    return _CopyOfRangeInt16(original, start, end, arrayCopy);
}

ECode CArrays::CopyOfRange(
    /* [in] */ ArrayOf<IInterface* >* original,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out, callee] */ ArrayOf<IInterface* >** arrayCopy)
{
    return _CopyOfRange(original, start, end, arrayCopy);
}

// ECode CArrays::CopyOfRangeEx(
//     /* [in] */ U[] original,
//     /* [in] */ Int32 start,
//     /* [in] */ Int32 end,
//     /* [in] */ Class<? extends T[]> newType,
//     /* [out, callee] */ <T, U> T[] arrayCopy)
// {
//     return _CopyOfRangeEx(original, start, end, newType, arrayCopy);
// }

} // namespace Utility
} // namespace Elastos
