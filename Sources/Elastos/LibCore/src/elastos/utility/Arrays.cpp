
#include "Arrays.h"
// #include "CArrayOf.h"
// #include "CArrayList.h"
// #include "DualPivotQuicksort.h"
// #include "ComparableTimSort.h"
// #include "TimSort.h"
// #include "Math.h"

// using Elastos::Core::Math;
// using Elastos::Core::IArrayOf;
// using Elastos::Core::CArrayOf;
// using Elastos::Core::IComparable;
// using Elastos::Core::EIID_IInteger32;
// using Elastos::Core::IInteger32;
// using Elastos::Core::EIID_IChar32;
// using Elastos::Core::IChar32;
// using Elastos::Core::EIID_IBoolean;
// using Elastos::Core::IBoolean;
// using Elastos::Core::EIID_IByte;
// using Elastos::Core::IByte;
// using Elastos::Core::EIID_IInteger64;
// using Elastos::Core::IInteger64;
// using Elastos::Core::EIID_IFloat;
// using Elastos::Core::IFloat;
// using Elastos::Core::EIID_IDouble;
// using Elastos::Core::IDouble;
// using Elastos::Core::EIID_IInteger16;
// using Elastos::Core::IInteger16;
// using Elastos::IO::EIID_ISerializable;

namespace Elastos {
namespace Utility {

ECode Arrays::CheckBinarySearchBounds(
    /* [in] */ Int32 startIndex,
    /* [in] */ Int32 endIndex,
    /* [in] */ Int32 length)
{
    if (startIndex > endIndex) {
        ALOGE("Arrays::CheckBinarySearchBounds: E_ILLEGAL_ARGUMENT_EXCEPTION: len: %d, start: %d, end: %d",
            length, startIndex, endIndex);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (startIndex < 0 || endIndex > length) {
        ALOGE("Arrays::CheckBinarySearchBounds: E_INDEX_OUT_OF_BOUNDS_EXCEPTION: len: %d, start: %d, end: %d",
            length, startIndex, endIndex);
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    return NOERROR;
}

ECode Arrays::CheckStartAndEnd(
    /* [in] */ Int32 len,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    if (start < 0 || end > len) {
        ALOGE("Arrays::CheckStartAndEnd: E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION: len: %d, start: %d, end: %d",
            len, start, end);
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    if (start > end) {
        ALOGE("Arrays::CheckStartAndEnd: E_ILLEGAL_ARGUMENT_EXCEPTION: len: %d, start: %d, end: %d",
            len, start, end);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

ECode Arrays::CheckOffsetAndCount(
    /* [in] */ Int32 arrayLength,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count)
{
    if ((offset | count) < 0 || offset > arrayLength || arrayLength - offset < count) {
        ALOGE("Arrays::CheckOffsetAndCount: E_INDEX_OUT_OF_BOUNDS_EXCEPTION: arrayLength: %d, offset: %d, count: %d",
            arrayLength, offset, count);
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    return NOERROR;
}

//====================================================================
// Arrays::ArrayList
//====================================================================
// CAR_INTERFACE_IMPL_2(Arrays::ArrayList, AbstractList, ISerializable, IRandomAccess)

// Arrays::ArrayList::ArrayList(
//     /* [in] */ ArrayOf<IInterface* >* storage)
// {
//     if (storage == NULL) {
// //        throw new NullPointerException("storage == null");
//         return;
//     }
//     mA = storage;
// }

// ECode Arrays::ArrayList::Contains(
//     /* [in] */ IInterface* object,
//     /* [out] */ Boolean* result)
// {
//     VALIDATE_NOT_NULL(result)
//     if (object != NULL) {
//         for (Int32 i = 0; i < mA->GetLength(); i++) {
//             AutoPtr<IInterface> element = (*mA)[i];
//             if (Object::Equals(object, element)) {
//                 *result = TRUE;
//                 return NOERROR;
//             }
//         }
//     }
//     else {
//         for (Int32 i = 0; i < mA->GetLength(); i++) {
//             AutoPtr<IInterface> element = (*mA)[i];
//             if (element == NULL) {
//                 *result = TRUE;
//                 return NOERROR;
//             }
//         }
//     }
//     *result = FALSE;
//     return NOERROR;
// }

// ECode Arrays::ArrayList::Get(
//     /* [in] */ Int32 location,
//     /* [out] */ IInterface** result)
// {
//     VALIDATE_NOT_NULL(result)
//     if (location < 0 || location >= mA->GetLength()) {
//         *result = NULL;
//         return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//     }
//     *result = (*mA)[location];
//     REFCOUNT_ADD(*result)
//     return NOERROR;
// }

// ECode Arrays::ArrayList::IndexOf(
//     /* [in] */ IInterface* object,
//     /* [out] */ Int32* result)
// {
//     VALIDATE_NOT_NULL(result)
//     if (object != NULL) {
//         for (Int32 i = 0; i < mA->GetLength(); i++) {
//             if (Object::Equals(object, (*mA)[i])) {
//                 *result = i;
//                 return NOERROR;
//             }
//         }
//     }
//     else {
//         for (Int32 i = 0; i < mA->GetLength(); i++) {
//             if ((*mA)[i] == NULL) {
//                 *result = i;
//                 return NOERROR;
//             }
//         }
//     }
//     *result = -1;
//     return NOERROR;
// }

// ECode Arrays::ArrayList::LastIndexOf(
//     /* [in] */ IInterface* object,
//     /* [out] */ Int32* index)
// {
//     VALIDATE_NOT_NULL(index)
//     if (object != NULL) {
//         for (Int32 i = mA->GetLength() - 1; i >= 0; i--) {
//             if (Object::Equals(object, (*mA)[i])) {
//                 *index = i;
//                 return NOERROR;
//             }
//         }
//     }
//     else {
//         for (Int32 i = mA->GetLength() - 1; i >= 0; i--) {
//             if ((*mA)[i] == NULL) {
//                 *index = i;
//                 return NOERROR;
//             }
//         }
//     }
//     *index = -1;
//     return NOERROR;
// }

// ECode Arrays::ArrayList::Set(
//     /* [in] */ Int32 location,
//     /* [in] */ IInterface* object,
//     /* [out] */ IInterface** preObject)
// {
//     VALIDATE_NOT_NULL(preObject)
//     AutoPtr<IInterface> result = (*mA)[location];
//     mA->Set(location, object);
//     *preObject = result;
//     REFCOUNT_ADD(*preObject)
//     return NOERROR;
// }

// ECode Arrays::ArrayList::GetSize(
//     /* [out] */ Int32* size)
// {
//     VALIDATE_NOT_NULL(size)
//     *size = mA->GetLength();
//     return NOERROR;
// }

// ECode Arrays::ArrayList::ToArray(
//     /* [out] */ ArrayOf<IInterface* >** outArray)
// {
//     VALIDATE_NOT_NULL(outArray)
//     *outArray = mA->Clone();
//     REFCOUNT_ADD(*outArray)
//     return NOERROR;
// }

// ECode Arrays::ArrayList::ToArray(
//     /* [in] */ ArrayOf<IInterface* >* contents,
//     /* [out] */ ArrayOf<IInterface* >** outArray)
// {
//     VALIDATE_NOT_NULL(outArray)
//     Int32 size;
//     GetSize(&size);
//     AutoPtr<ArrayOf<IInterface*> > array = size > contents->GetLength() ?
//             ArrayOf<IInterface*>::Alloc(size) : contents;
//     array->Copy(mA, size);
//     if (size < contents->GetLength()) {
//         array->Set(size, NULL);
//     }
//     *outArray = array;
//     REFCOUNT_ADD(*outArray)
//     return NOERROR;
// }

//====================================================================
// Arrays
//====================================================================

// ECode Arrays::AsList(
//     /* [in] */ ArrayOf<IInterface*>* array,
//     /* [out] */ IList** outList)
// {
//     VALIDATE_NOT_NULL(outList)
//     *outList = NULL;

//     if (array) {
//         // return new ArrayList<T>(array);
//         AutoPtr<CArrayList> al;
//         CArrayList::NewByFriend(array->GetLength(), (CArrayList**)&al);

//         for (Int32 i = 0; i < array->GetLength(); ++i) {
//             al->Add(i, (*array)[i]);
//         }

//         *outList = (IList*)al->Probe(EIID_IList);
//         REFCOUNT_ADD(*outList)
//     }

//     return NOERROR;
// }

// ECode Arrays::BinarySearchObject(
//     /* [in] */ ArrayOf<IInterface* >* array,
//     /* [in] */ IInterface* value,
//     /* [out] */ Int32* index)
// {
//     VALIDATE_NOT_NULL(index)
//     return BinarySearchObject(array, 0, array->GetLength(), value, index);
// }

// ECode Arrays::BinarySearchObject(
//     /* [in] */ ArrayOf<IInterface* >* array,
//     /* [in] */ Int32 startIndex,
//     /* [in] */ Int32 endIndex,
//     /* [in] */ IInterface* value,
//     /* [out] */ Int32* index)
// {
//     VALIDATE_NOT_NULL(index)
//     FAIL_RETURN(CheckBinarySearchBounds(startIndex, endIndex, array->GetLength()));
//     Int32 lo = startIndex;
//     Int32 hi = endIndex - 1;

//     while (lo <= hi) {
//         Int32 mid = (UInt32(lo + hi)) >> 1;
//         AutoPtr<IComparable> p = IComparable::Probe((*array)[mid]);
//         assert(p != NULL);
//         Int32 midValCmp;
//         p->CompareTo(value, &midValCmp);

//         if (midValCmp < 0) {
//             lo = mid + 1;
//         }
//         else if (midValCmp > 0) {
//             hi = mid - 1;
//         }
//         else {
//             *index = mid;  // value found
//             return NOERROR;
//         }
//     }
//     *index = ~lo;  // value not present
//     return NOERROR;
// }

// ECode Arrays::BinarySearch(
//     /* [in] */ ArrayOf<IInterface* >* array,
//     /* [in] */ IInterface* value,
//     /* [in] */ IComparator* comparator,
//     /* [out] */ Int32* index)
// {
//     VALIDATE_NOT_NULL(index)
//     return BinarySearch(array, 0, array->GetLength(), value, comparator, index);
// }

// ECode Arrays::BinarySearch(
//     /* [in] */ ArrayOf<IInterface* >* array,
//     /* [in] */ Int32 startIndex,
//     /* [in] */ Int32 endIndex,
//     /* [in] */ IInterface* value,
//     /* [in] */ IComparator* comparator,
//     /* [out] */ Int32* index)
// {
//     VALIDATE_NOT_NULL(index)
//     if (comparator == NULL) {
//         return BinarySearchObject(array, startIndex, endIndex, value, index);
//     }

//     FAIL_RETURN(CheckBinarySearchBounds(startIndex, endIndex, array->GetLength()));
//     Int32 lo = startIndex;
//     Int32 hi = endIndex - 1;

//     while (lo <= hi) {
//         Int32 mid = (UInt32(lo + hi)) >> 1;
//         Int32 midValCmp;
//         comparator->Compare((*array)[mid], value, &midValCmp);

//         if (midValCmp < 0) {
//             lo = mid + 1;
//         }
//         else if (midValCmp > 0) {
//             hi = mid - 1;
//         }
//         else {
//             *index = mid;  // value found
//             return NOERROR;
//         }
//     }
//     *index = ~lo;  // value not present
//     return NOERROR;
// }

// // ECode Arrays::BinarySearchInt16(
// //     /* [in] */ ArrayOf<Int16>* array,
// //     /* [in] */ Int16 value,
// //     /* [out] */ Int32* index)
// // {
// //     VALIDATE_NOT_NULL(index)
// //     return BinarySearchInt16(array, 0, array->GetLength(), value, index);
// // }

// // ECode Arrays::BinarySearchInt16(
// //     /* [in] */ ArrayOf<Int16>* array,
// //     /* [in] */ Int32 startIndex,
// //     /* [in] */ Int32 endIndex,
// //     /* [in] */ Int16 value,
// //     /* [out] */ Int32* index)
// // {
// //     VALIDATE_NOT_NULL(index)
// //     FAIL_RETURN(CheckBinarySearchBounds(startIndex, endIndex, array->GetLength()));
// //     Int32 lo = startIndex;
// //     Int32 hi = endIndex - 1;

// //     while (lo <= hi) {
// //         Int32 mid = (UInt32(lo + hi)) >> 1;
// //         Int16 midVal = (*array)[mid];

// //         if (midVal < value) {
// //             lo = mid + 1;
// //         }
// //         else if (midVal > value) {
// //             hi = mid - 1;
// //         }
// //         else {
// //             *index = mid;  // value found
// //             return NOERROR;
// //         }
// //     }
// //     *index = ~lo;  // value not present
// //     return NOERROR;
// // }

// ECode Arrays::FillByte(
//     /* [in] */ ArrayOf<Byte>* array,
//     /* [in] */ Byte value)
// {
//     for (Int32 i = 0; i < array->GetLength(); i++) {
//         array->Set(i, value);
//     }
//     return NOERROR;
// }

// ECode Arrays::FillByte(
//     /* [in] */ ArrayOf<Byte>* array,
//     /* [in] */ Int32 start,
//     /* [in] */ Int32 end,
//     /* [in] */ Byte value)
// {
//     FAIL_RETURN(CheckStartAndEnd(array->GetLength(), start, end));
//     for (Int32 i = start; i < end; i++) {
//         array->Set(i, value);
//     }
//     return NOERROR;
// }

// ECode Arrays::FillInt16(
//     /* [in] */ ArrayOf<Int16>* array,
//     /* [in] */ Int16 value)
// {
//     for (Int32 i = 0; i < array->GetLength(); i++) {
//         array->Set(i, value);
//     }
//     return NOERROR;
// }

// ECode Arrays::FillInt16(
//     /* [in] */ ArrayOf<Int16>* array,
//     /* [in] */ Int32 start,
//     /* [in] */ Int32 end,
//     /* [in] */ Int16 value)
// {
//     FAIL_RETURN(CheckStartAndEnd(array->GetLength(), start, end));
//     for (Int32 i = start; i < end; i++) {
//         array->Set(i, value);
//     }
//     return NOERROR;
// }

// ECode Arrays::FillChar32(
//     /* [in] */ ArrayOf<Char32>* array,
//     /* [in] */ Char32 value)
// {
//     for (Int32 i = 0; i < array->GetLength(); i++) {
//         array->Set(i, value);
//     }
//     return NOERROR;
// }

// ECode Arrays::FillChar32(
//     /* [in] */ ArrayOf<Char32>* array,
//     /* [in] */ Int32 start,
//     /* [in] */ Int32 end,
//     /* [in] */ Char32 value)
// {
//     FAIL_RETURN(CheckStartAndEnd(array->GetLength(), start, end));
//     for (Int32 i = start; i < end; i++) {
//         array->Set(i, value);
//     }
//     return NOERROR;
// }

// ECode Arrays::FillInt32(
//     /* [in] */ ArrayOf<Int32>* array,
//     /* [in] */ Int32 value)
// {
//     for (Int32 i = 0; i < array->GetLength(); i++) {
//         array->Set(i, value);
//     }
//     return NOERROR;
// }

// ECode Arrays::FillInt32(
//     /* [in] */ ArrayOf<Int32>* array,
//     /* [in] */ Int32 start,
//     /* [in] */ Int32 end,
//     /* [in] */ Int32 value)
// {
//     FAIL_RETURN(CheckStartAndEnd(array->GetLength(), start, end));
//     for (Int32 i = start; i < end; i++) {
//         array->Set(i, value);
//     }
//     return NOERROR;
// }

// ECode Arrays::FillInt64(
//     /* [in] */ ArrayOf<Int64>* array,
//     /* [in] */ Int64 value)
// {
//     for (Int32 i = 0; i < array->GetLength(); i++) {
//         array->Set(i, value);
//     }
//     return NOERROR;
// }

// ECode Arrays::FillInt64(
//     /* [in] */ ArrayOf<Int64>* array,
//     /* [in] */ Int32 start,
//     /* [in] */ Int32 end,
//     /* [in] */ Int64 value)
// {
//     FAIL_RETURN(CheckStartAndEnd(array->GetLength(), start, end));
//     for (Int32 i = start; i < end; i++) {
//         array->Set(i, value);
//     }
//     return NOERROR;
// }

// ECode Arrays::FillFloat(
//     /* [in] */ ArrayOf<Float>* array,
//     /* [in] */ Float value)
// {
//     for (Int32 i = 0; i < array->GetLength(); i++) {
//         array->Set(i, value);
//     }
//     return NOERROR;
// }

// ECode Arrays::FillFloat(
//     /* [in] */ ArrayOf<Float>* array,
//     /* [in] */ Int32 start,
//     /* [in] */ Int32 end,
//     /* [in] */ Float value)
// {
//     FAIL_RETURN(CheckStartAndEnd(array->GetLength(), start, end));
//     for (Int32 i = start; i < end; i++) {
//         array->Set(i, value);
//     }
//     return NOERROR;
// }

// ECode Arrays::FillDouble(
//     /* [in] */ ArrayOf<Double>* array,
//     /* [in] */ Double value)
// {
//     for (Int32 i = 0; i < array->GetLength(); i++) {
//         array->Set(i, value);
//     }
//     return NOERROR;
// }

// ECode Arrays::FillDouble(
//     /* [in] */ ArrayOf<Double>* array,
//     /* [in] */ Int32 start,
//     /* [in] */ Int32 end,
//     /* [in] */ Double value)
// {
//     FAIL_RETURN(CheckStartAndEnd(array->GetLength(), start, end));
//     for (Int32 i = start; i < end; i++) {
//         array->Set(i, value);
//     }
//     return NOERROR;
// }

// ECode Arrays::FillBoolean(
//     /* [in] */ ArrayOf<Boolean>* array,
//     /* [in] */ Boolean value)
// {
//     for (Int32 i = 0; i < array->GetLength(); i++) {
//         array->Set(i, value);
//     }
//     return NOERROR;
// }

// ECode Arrays::FillBoolean(
//     /* [in] */ ArrayOf<Boolean>* array,
//     /* [in] */ Int32 start,
//     /* [in] */ Int32 end,
//     /* [in] */ Boolean value)
// {
//     FAIL_RETURN(CheckStartAndEnd(array->GetLength(), start, end));
//     for (Int32 i = start; i < end; i++) {
//         array->Set(i, value);
//     }
//     return NOERROR;
// }

// ECode Arrays::FillObject(
//     /* [in] */ ArrayOf<IInterface* >* array,
//     /* [in] */ IInterface* value)
// {
//     for (Int32 i = 0; i < array->GetLength(); i++) {
//         array->Set(i, value);
//     }
//     return NOERROR;
// }

// ECode Arrays::FillObject(
//     /* [in] */ ArrayOf<IInterface* >* array,
//     /* [in] */ Int32 start,
//     /* [in] */ Int32 end,
//     /* [in] */ IInterface* value)
// {
//     FAIL_RETURN(CheckStartAndEnd(array->GetLength(), start, end));
//     for (Int32 i = start; i < end; i++) {
//         array->Set(i, value);
//     }
//     return NOERROR;
// }

// ECode Arrays::HashCodeBoolean(
//     /* [in] */ ArrayOf<Boolean>* array,
//     /* [out] */ Int32* code)
// {
//     VALIDATE_NOT_NULL(code)
//     if (array == NULL) {
//         *code = 0;
//         return NOERROR;
//     }
//     Int32 hashCode = 1;
//     for (Int32 i = 0; i < array->GetLength(); i++) {
//         Boolean element = (*array)[i];
//         // 1231, 1237 are hash code values for boolean value
//         hashCode = 31 * hashCode + (element ? 1231 : 1237);
//     }
//     *code = hashCode;
//     return NOERROR;
// }

// ECode Arrays::HashCodeInt32(
//     /* [in] */ ArrayOf<Int32>* array,
//     /* [out] */ Int32* code)
// {
//     VALIDATE_NOT_NULL(code)
//     if (array == NULL) {
//         *code = 0;
//         return NOERROR;
//     }
//     Int32 hashCode = 1;
//     for (Int32 i = 0; i < array->GetLength(); i++) {
//         Int32 element = (*array)[i];
//         // the hash code value for integer value is integer value itself
//         hashCode = 31 * hashCode + element;
//     }
//     *code = hashCode;
//     return NOERROR;
// }

// ECode Arrays::HashCodeInt16(
//     /* [in] */ ArrayOf<Int16>* array,
//     /* [out] */ Int32* code)
// {
//     VALIDATE_NOT_NULL(code)
//     if (array == NULL) {
//         *code = 0;
//         return NOERROR;
//     }
//     Int32 hashCode = 1;
//     for (Int32 i = 0; i < array->GetLength(); i++) {
//         Int16 element = (*array)[i];
//         // the hash code value for short value is its integer value
//         hashCode = 31 * hashCode + element;
//     }
//     *code = hashCode;
//     return NOERROR;
// }

// ECode Arrays::HashCodeChar32(
//     /* [in] */ ArrayOf<Char32>* array,
//     /* [out] */ Int32* code)
// {
//     VALIDATE_NOT_NULL(code)
//     if (array == NULL) {
//         *code = 0;
//         return NOERROR;
//     }
//     Int32 hashCode = 1;
//     for (Int32 i = 0; i < array->GetLength(); i++) {
//         Char32 element = (*array)[i];
//         // the hash code value for char value is its integer value
//         hashCode = 31 * hashCode + element;
//     }
//     *code = hashCode;
//     return NOERROR;
// }

// ECode Arrays::HashCodeByte(
//     /* [in] */ ArrayOf<Byte>* array,
//     /* [out] */ Int32* code)
// {
//     VALIDATE_NOT_NULL(code)
//     if (array == NULL) {
//         *code = 0;
//         return NOERROR;
//     }
//     Int32 hashCode = 1;
//     for (Int32 i = 0; i < array->GetLength(); i++) {
//         Byte element = (*array)[i];
//         // the hash code value for byte value is its integer value
//         hashCode = 31 * hashCode + element;
//     }
//     *code = hashCode;
//     return NOERROR;
// }

// ECode Arrays::HashCodeInt64(
//     /* [in] */ ArrayOf<Int64>* array,
//     /* [out] */ Int32* code)
// {
//     VALIDATE_NOT_NULL(code)
//     if (array == NULL) {
//         *code = 0;
//         return NOERROR;
//     }
//     Int32 hashCode = 1;
//     for (Int32 i = 0; i < array->GetLength(); i++) {
//         Int64 elementValue = (*array)[i];
//         /*
//          * the hash code value for long value is (int) (value ^ (value >>>
//          * 32))
//          */
//         hashCode = 31 * hashCode
//                 + (Int32) (elementValue ^ (elementValue >> 32));
//     }
//     *code = hashCode;
//     return NOERROR;
// }

// ECode Arrays::HashCodeFloat(
//     /* [in] */ ArrayOf<Float>* array,
//     /* [out] */ Int32* code)
// {
//     VALIDATE_NOT_NULL(code)
//     if (array == NULL) {
//         *code = 0;
//         return NOERROR;
//     }
//     Int32 hashCode = 1;
//     for (Int32 i = 0; i < array->GetLength(); i++) {
//         Float element = (*array)[i];
//         /*
//          * the hash code value for float value is
//          * Float.floatToIntBits(value)
//          */
//         hashCode = 31 * hashCode + Elastos::Core::Math::FloatToInt32Bits(element);
//     }
//     *code = hashCode;
//     return NOERROR;
// }

// ECode Arrays::HashCodeDouble(
//     /* [in] */ ArrayOf<Double>* array,
//     /* [out] */ Int32* code)
// {
//     VALIDATE_NOT_NULL(code)
//     if (array == NULL) {
//         *code = 0;
//         return NOERROR;
//     }
//     Int32 hashCode = 1;
//     for (Int32 i = 0; i < array->GetLength(); i++) {
//         Double element = (*array)[i];
//         Int64 v = Elastos::Core::Math::DoubleToInt64Bits(element);
//         /*
//          * the hash code value for double value is (int) (v ^ (v >>> 32))
//          * where v = Double.doubleToLongBits(value)
//          */
//         hashCode = 31 * hashCode + (Int32) (v ^ (v >> 32));
//     }
//     *code = hashCode;
//     return NOERROR;
// }

// ECode Arrays::HashCodeObject(
//     /* [in] */ ArrayOf<IInterface* >* array,
//     /* [out] */ Int32* code)
// {
//     VALIDATE_NOT_NULL(code)
//     if (array == NULL) {
//         *code = 0;
//         return NOERROR;
//     }
//     Int32 hashCode = 1;
//     for (Int32 i = 0; i < array->GetLength(); i++) {
//         AutoPtr<IInterface> element = (*array)[i];
//         Int32 elementHashCode;
//         if (element == NULL) {
//             elementHashCode = 0;
//         }
//         else {
//             elementHashCode = Object::GetHashCode(element);
//         }
//         hashCode = 31 * hashCode + elementHashCode;
//     }
//     *code = hashCode;
//     return NOERROR;
// }

// ECode Arrays::DeepHashCode(
//     /* [in] */ ArrayOf<IInterface* >* array,
//     /* [out] */ Int32* code)
// {
//     VALIDATE_NOT_NULL(code)
//     if (array == NULL) {
//         *code = 0;
//         return NOERROR;
//     }
//     Int32 hashCode = 1;
//     for (Int32 i = 0; i < array->GetLength(); i++) {
//         AutoPtr<IInterface> element = (*array)[i];
//         Int32 elementHashCode = DeepHashCodeElement(element);
//         hashCode = 31 * hashCode + elementHashCode;
//     }
//     *code = hashCode;
//     return NOERROR;
// }

// Int32 Arrays::DeepHashCodeElement(
//     /* [in] */ IInterface* element)
// {
//     if (element == NULL) {
//          return 0;
//     }

//     if (IArrayOf::Probe(element) == NULL) {
//         return Object::GetHashCode(element);
//     }

//     /*
//      * element is an array
//      */
//     Int32 hashCode;
//     Int32 size;
//     AutoPtr<IArrayOf> p = IArrayOf::Probe(element);
//     InterfaceID riid;
//     p->GetTypeId(&riid);
//     p->GetLength(&size);
//     if (riid == EIID_IInteger32) {
//         AutoPtr<ArrayOf<Int32> > arr = ArrayOf<Int32>::Alloc(size);
//         for (Int32 i = 0; i < size; ++i) {
//             AutoPtr<IInteger32> elem;
//             p->Get(i, (IInterface**)&elem);
//             Int32 realNum;
//             elem->GetValue(&realNum);
//             arr->Set(i, realNum);
//         }
//         HashCodeInt32(arr, &hashCode);
//         return hashCode;
//     }
//     if (riid == EIID_IChar32) {
//         AutoPtr<ArrayOf<Char32> > arr = ArrayOf<Char32>::Alloc(size);
//         for (Int32 i = 0; i < size; ++i) {
//             AutoPtr<IChar32> elem;
//             p->Get(i, (IInterface**)&elem);
//             Char32 realNum;
//             elem->GetValue(&realNum);
//             arr->Set(i, realNum);
//         }
//         HashCodeChar32(arr, &hashCode);
//         return hashCode;
//     }
//     if (riid == EIID_IBoolean) {
//         AutoPtr<ArrayOf<Boolean> > arr = ArrayOf<Boolean>::Alloc(size);
//         for (Int32 i = 0; i < size; ++i) {
//             AutoPtr<IBoolean> elem;
//             p->Get(i, (IInterface**)&elem);
//             Boolean realNum;
//             elem->GetValue(&realNum);
//             arr->Set(i, realNum);
//         }
//         HashCodeBoolean(arr, &hashCode);
//         return hashCode;
//     }
//     if (riid == EIID_IByte) {
//         AutoPtr<ArrayOf<Byte> > arr = ArrayOf<Byte>::Alloc(size);
//         for (Int32 i = 0; i < size; ++i) {
//             AutoPtr<IByte> elem;
//             p->Get(i, (IInterface**)&elem);
//             Byte realNum;
//             elem->GetValue(&realNum);
//             arr->Set(i, realNum);
//         }
//         HashCodeByte(arr, &hashCode);
//         return hashCode;
//     }
//     if (riid == EIID_IInteger64) {
//         AutoPtr<ArrayOf<Int64> > arr = ArrayOf<Int64>::Alloc(size);
//         for (Int32 i = 0; i < size; ++i) {
//             AutoPtr<IInteger64> elem;
//             p->Get(i, (IInterface**)&elem);
//             Int64 realNum;
//             elem->GetValue(&realNum);
//             arr->Set(i, realNum);
//         }
//         HashCodeInt64(arr, &hashCode);
//         return hashCode;
//     }
//     if (riid == EIID_IFloat) {
//         AutoPtr<ArrayOf<Float> > arr = ArrayOf<Float>::Alloc(size);
//         for (Int32 i = 0; i < size; ++i) {
//             AutoPtr<IFloat> elem;
//             p->Get(i, (IInterface**)&elem);
//             Float realNum;
//             elem->GetValue(&realNum);
//             arr->Set(i, realNum);
//         }
//         HashCodeFloat(arr, &hashCode);
//         return hashCode;
//     }
//     if (riid == EIID_IDouble) {
//         AutoPtr<ArrayOf<Double> > arr = ArrayOf<Double>::Alloc(size);
//         for (Int32 i = 0; i < size; ++i) {
//             AutoPtr<IDouble> elem;
//             p->Get(i, (IInterface**)&elem);
//             Double realNum;
//             elem->GetValue(&realNum);
//             arr->Set(i, realNum);
//         }
//         HashCodeDouble(arr, &hashCode);
//         return hashCode;
//     }
//     if (riid == EIID_IInteger16) {
//         AutoPtr<ArrayOf<Int16> > arr = ArrayOf<Int16>::Alloc(size);
//         for (Int32 i = 0; i < size; ++i) {
//             AutoPtr<IInteger16> elem;
//             p->Get(i, (IInterface**)&elem);
//             Int16 realNum;
//             elem->GetValue(&realNum);
//             arr->Set(i, realNum);
//         }
//         HashCodeInt16(arr, &hashCode);
//         return hashCode;
//     }

//     AutoPtr<ArrayOf<IInterface*> > arr = ArrayOf<IInterface*>::Alloc(size);
//     for (Int32 i = 0; i < size; ++i) {
//         AutoPtr<IInterface> elem;
//         p->Get(i, (IInterface**)&elem);
//         arr->Set(i, elem);
//     }
//     DeepHashCode(arr, &hashCode);
//     return hashCode;
// }

// ECode Arrays::EqualsByte(
//     /* [in] */ ArrayOf<Byte>* array1,
//     /* [in] */ ArrayOf<Byte>* array2,
//     /* [out] */ Boolean* result)
// {
//     VALIDATE_NOT_NULL(result)
//     if (array1 == array2) {
//         *result = TRUE;
//         return NOERROR;
//     }
//     if (array1 == NULL || array2 == NULL || array1->GetLength() != array2->GetLength()) {
//         *result = FALSE;
//         return NOERROR;
//     }
//     for (Int32 i = 0; i < array1->GetLength(); i++) {
//         if ((*array1)[i] != (*array2)[i]) {
//             *result = FALSE;
//             return NOERROR;
//         }
//     }
//     *result = TRUE;
//     return NOERROR;
// }

// ECode Arrays::EqualsInt16(
//     /* [in] */ ArrayOf<Int16>* array1,
//     /* [in] */ ArrayOf<Int16>* array2,
//     /* [out] */ Boolean* result)
// {
//     VALIDATE_NOT_NULL(result)
//     if (array1 == array2) {
//         *result = TRUE;
//         return NOERROR;
//     }
//     if (array1 == NULL || array2 == NULL || array1->GetLength() != array2->GetLength()) {
//         *result = FALSE;
//         return NOERROR;
//     }
//     for (Int32 i = 0; i < array1->GetLength(); i++) {
//         if ((*array1)[i] != (*array2)[i]) {
//             *result = FALSE;
//             return NOERROR;
//         }
//     }
//     *result = TRUE;
//     return NOERROR;
// }

// ECode Arrays::EqualsChar32(
//     /* [in] */ ArrayOf<Char32>* array1,
//     /* [in] */ ArrayOf<Char32>* array2,
//     /* [out] */ Boolean* result)
// {
//     VALIDATE_NOT_NULL(result)
//     if (array1 == array2) {
//         *result = TRUE;
//         return NOERROR;
//     }
//     if (array1 == NULL || array2 == NULL || array1->GetLength() != array2->GetLength()) {
//         *result = FALSE;
//         return NOERROR;
//     }
//     for (Int32 i = 0; i < array1->GetLength(); i++) {
//         if ((*array1)[i] != (*array2)[i]) {
//             *result = FALSE;
//             return NOERROR;
//         }
//     }
//     *result = TRUE;
//     return NOERROR;
// }

// ECode Arrays::EqualsInt32(
//     /* [in] */ ArrayOf<Int32>* array1,
//     /* [in] */ ArrayOf<Int32>* array2,
//     /* [out] */ Boolean* result)
// {
//     VALIDATE_NOT_NULL(result)
//     if (array1 == array2) {
//         *result = TRUE;
//         return NOERROR;
//     }
//     if (array1 == NULL || array2 == NULL || array1->GetLength() != array2->GetLength()) {
//         *result = FALSE;
//         return NOERROR;
//     }
//     for (Int32 i = 0; i < array1->GetLength(); i++) {
//         if ((*array1)[i] != (*array2)[i]) {
//             *result = FALSE;
//             return NOERROR;
//         }
//     }
//     *result = TRUE;
//     return NOERROR;
// }

// ECode Arrays::EqualsInt64(
//     /* [in] */ ArrayOf<Int64>* array1,
//     /* [in] */ ArrayOf<Int64>* array2,
//     /* [out] */ Boolean* result)
// {
//     VALIDATE_NOT_NULL(result)
//     if (array1 == array2) {
//         *result = TRUE;
//         return NOERROR;
//     }
//     if (array1 == NULL || array2 == NULL || array1->GetLength() != array2->GetLength()) {
//         *result = FALSE;
//         return NOERROR;
//     }
//     for (Int32 i = 0; i < array1->GetLength(); i++) {
//         if ((*array1)[i] != (*array2)[i]) {
//             *result = FALSE;
//             return NOERROR;
//         }
//     }
//     *result = TRUE;
//     return NOERROR;
// }

// ECode Arrays::EqualsFloat(
//     /* [in] */ ArrayOf<Float>* array1,
//     /* [in] */ ArrayOf<Float>* array2,
//     /* [out] */ Boolean* result)
// {
//     VALIDATE_NOT_NULL(result)
//     if (array1 == array2) {
//         *result = TRUE;
//         return NOERROR;
//     }
//     if (array1 == NULL || array2 == NULL || array1->GetLength() != array2->GetLength()) {
//         *result = FALSE;
//         return NOERROR;
//     }
//     for (Int32 i = 0; i < array1->GetLength(); i++) {
//         Int32 num1 = Elastos::Core::Math::FloatToInt32Bits((*array1)[i]);
//         Int32 num2 = Elastos::Core::Math::FloatToInt32Bits((*array2)[i]);
//         if (num1 != num2) {
//             *result = FALSE;
//             return NOERROR;
//         }
//     }
//     *result = TRUE;
//     return NOERROR;
// }

// ECode Arrays::EqualsDouble(
//     /* [in] */ ArrayOf<Double>* array1,
//     /* [in] */ ArrayOf<Double>* array2,
//     /* [out] */ Boolean* result)
// {
//     VALIDATE_NOT_NULL(result)
//     if (array1 == array2) {
//         *result = TRUE;
//         return NOERROR;
//     }
//     if (array1 == NULL || array2 == NULL || array1->GetLength() != array2->GetLength()) {
//         *result = FALSE;
//         return NOERROR;
//     }
//     for (Int32 i = 0; i < array1->GetLength(); i++) {
//         Int64 num1 = Elastos::Core::Math::DoubleToInt64Bits((*array1)[i]);
//         Int64 num2 = Elastos::Core::Math::DoubleToInt64Bits((*array2)[i]);
//         if (num1 != num2) {
//             *result = FALSE;
//             return NOERROR;
//         }
//     }
//     *result = TRUE;
//     return NOERROR;
// }

// ECode Arrays::EqualsBoolean(
//     /* [in] */ ArrayOf<Boolean>* array1,
//     /* [in] */ ArrayOf<Boolean>* array2,
//     /* [out] */ Boolean* result)
// {
//     VALIDATE_NOT_NULL(result)
//     if (array1 == array2) {
//         *result = TRUE;
//         return NOERROR;
//     }
//     if (array1 == NULL || array2 == NULL || array1->GetLength() != array2->GetLength()) {
//         *result = FALSE;
//         return NOERROR;
//     }
//     for (Int32 i = 0; i < array1->GetLength(); i++) {
//         if ((*array1)[i] != (*array2)[i]) {
//             *result = FALSE;
//             return NOERROR;
//         }
//     }
//     *result = TRUE;
//     return NOERROR;
// }

// ECode Arrays::EqualsObject(
//     /* [in] */ ArrayOf<IInterface* >* array1,
//     /* [in] */ ArrayOf<IInterface* >* array2,
//     /* [out] */ Boolean* result)
// {
//     VALIDATE_NOT_NULL(result)
//     if (array1 == array2) {
//         *result = TRUE;
//         return NOERROR;
//     }
//     if (array1 == NULL || array2 == NULL || array1->GetLength() != array2->GetLength()) {
//         *result = FALSE;
//         return NOERROR;
//     }
//     for (Int32 i = 0; i < array1->GetLength(); i++) {
//         AutoPtr<IInterface> e1 = (*array1)[i];
//         AutoPtr<IInterface> e2 = (*array2)[i];
//         if (!(e1 == NULL ? e2 == NULL : Object::Equals(e1, e2))) {
//             *result = FALSE;
//             return NOERROR;
//         }
//     }
//     *result = TRUE;
//     return NOERROR;
// }

// ECode Arrays::DeepEquals(
//     /* [in] */ ArrayOf<IInterface* >* array1,
//     /* [in] */ ArrayOf<IInterface* >* array2,
//     /* [out] */ Boolean* result)
// {
//     VALIDATE_NOT_NULL(result)
//     if (array1 == array2) {
//         *result = TRUE;
//         return NOERROR;
//     }
//     if (array1 == NULL || array2 == NULL || array1->GetLength() != array2->GetLength()) {
//         *result = FALSE;
//         return NOERROR;
//     }
//     for (Int32 i = 0; i < array1->GetLength(); i++) {
//         AutoPtr<IInterface> e1 = (*array1)[i];
//         AutoPtr<IInterface> e2 = (*array2)[i];
//         if (!DeepEqualsElements(e1, e2)) {
//             *result = FALSE;
//             return NOERROR;
//         }
//     }
//     *result = TRUE;
//     return NOERROR;
// }

// Boolean Arrays::DeepEqualsElements(
//     /* [in] */ IInterface* e1,
//     /* [in] */ IInterface* e2)
// {
//     if (e1->Probe(EIID_IInterface) == e2->Probe(EIID_IInterface)) {
//         return TRUE;
//     }

//     if (e1 == NULL || e2 == NULL) {
//         return FALSE;
//     }

//     if (IArrayOf::Probe(e1) == NULL || IArrayOf::Probe(e2) == NULL) {
//         return Object::Equals(e1, e2);
//     }

//     /*
//      * compare as arrays
//      */
//     AutoPtr<IArrayOf> p1 = IArrayOf::Probe(e1);
//     AutoPtr<IArrayOf> p2 = IArrayOf::Probe(e2);
//     InterfaceID riid1, riid2;
//     p1->GetTypeId(&riid1);
//     p2->GetTypeId(&riid2);
//     if (riid1 != riid2) {
//         return FALSE;
//     }
//     Boolean result;
//     Int32 size;
//     if (riid1 == EIID_IInterface) {
//         p1->GetLength(&size);
//         AutoPtr<ArrayOf<IInterface*> > first = ArrayOf<IInterface*>::Alloc(size);
//         for (Int32 i = 0; i < size; ++i) {
//             AutoPtr<IInterface> elem;
//             p1->Get(i, (IInterface**)&elem);
//             first->Set(i, elem);
//         }

//         p2->GetLength(&size);
//         AutoPtr<ArrayOf<IInterface*> > sencond = ArrayOf<IInterface*>::Alloc(size);
//         for (Int32 i = 0; i < size; ++i) {
//             AutoPtr<IInterface> elem;
//             p2->Get(i, (IInterface**)&elem);
//             sencond->Set(i, elem);
//         }
//         DeepEquals(first, sencond, &result);
//         return result;
//     }

//     if (riid1 == EIID_IInteger32) {
//         p1->GetLength(&size);
//         AutoPtr<ArrayOf<Int32> > first = ArrayOf<Int32>::Alloc(size);
//         for (Int32 i = 0; i < size; ++i) {
//             AutoPtr<IInteger32> elem;
//             p1->Get(i, (IInterface**)&elem);
//             Int32 realNum;
//             elem->GetValue(&realNum);
//             first->Set(i, realNum);
//         }

//         p2->GetLength(&size);
//         AutoPtr<ArrayOf<Int32> > sencond = ArrayOf<Int32>::Alloc(size);
//         for (Int32 i = 0; i < size; ++i) {
//             AutoPtr<IInteger32> elem;
//             p2->Get(i, (IInterface**)&elem);
//             Int32 realNum;
//             elem->GetValue(&realNum);
//             sencond->Set(i, realNum);
//         }
//         EqualsInt32(first, sencond, &result);
//         return result;
//     }
//     if (riid1 == EIID_IChar32) {
//         p1->GetLength(&size);
//         AutoPtr<ArrayOf<Char32> > first = ArrayOf<Char32>::Alloc(size);
//         for (Int32 i = 0; i < size; ++i) {
//             AutoPtr<IChar32> elem;
//             p1->Get(i, (IInterface**)&elem);
//             Char32 realNum;
//             elem->GetValue(&realNum);
//             first->Set(i, realNum);
//         }

//         p2->GetLength(&size);
//         AutoPtr<ArrayOf<Char32> > sencond = ArrayOf<Char32>::Alloc(size);
//         for (Int32 i = 0; i < size; ++i) {
//             AutoPtr<IChar32> elem;
//             p2->Get(i, (IInterface**)&elem);
//             Char32 realNum;
//             elem->GetValue(&realNum);
//             sencond->Set(i, realNum);
//         }
//         EqualsChar32(first, sencond, &result);
//         return result;
//     }
//     if (riid1 == EIID_IBoolean) {
//         p1->GetLength(&size);
//         AutoPtr<ArrayOf<Boolean> > first = ArrayOf<Boolean>::Alloc(size);
//         for (Int32 i = 0; i < size; ++i) {
//             AutoPtr<IBoolean> elem;
//             p1->Get(i, (IInterface**)&elem);
//             Boolean realNum;
//             elem->GetValue(&realNum);
//             first->Set(i, realNum);
//         }

//         p2->GetLength(&size);
//         AutoPtr<ArrayOf<Boolean> > sencond = ArrayOf<Boolean>::Alloc(size);
//         for (Int32 i = 0; i < size; ++i) {
//             AutoPtr<IBoolean> elem;
//             p2->Get(i, (IInterface**)&elem);
//             Boolean realNum;
//             elem->GetValue(&realNum);
//             sencond->Set(i, realNum);
//         }
//         EqualsBoolean(first, sencond, &result);
//         return result;
//     }
//     if (riid1 == EIID_IByte) {
//         p1->GetLength(&size);
//         AutoPtr<ArrayOf<Byte> > first = ArrayOf<Byte>::Alloc(size);
//         for (Int32 i = 0; i < size; ++i) {
//             AutoPtr<IByte> elem;
//             p1->Get(i, (IInterface**)&elem);
//             Byte realNum;
//             elem->GetValue(&realNum);
//             first->Set(i, realNum);
//         }

//         p2->GetLength(&size);
//         AutoPtr<ArrayOf<Byte> > sencond = ArrayOf<Byte>::Alloc(size);
//         for (Int32 i = 0; i < size; ++i) {
//             AutoPtr<IByte> elem;
//             p2->Get(i, (IInterface**)&elem);
//             Byte realNum;
//             elem->GetValue(&realNum);
//             sencond->Set(i, realNum);
//         }
//         EqualsByte(first, sencond, &result);
//         return result;
//     }
//     if (riid1 == EIID_IInteger64) {
//         p1->GetLength(&size);
//         AutoPtr<ArrayOf<Int64> > first = ArrayOf<Int64>::Alloc(size);
//         for (Int32 i = 0; i < size; ++i) {
//             AutoPtr<IInteger64> elem;
//             p1->Get(i, (IInterface**)&elem);
//             Int64 realNum;
//             elem->GetValue(&realNum);
//             first->Set(i, realNum);
//         }

//         p2->GetLength(&size);
//         AutoPtr<ArrayOf<Int64> > sencond = ArrayOf<Int64>::Alloc(size);
//         for (Int32 i = 0; i < size; ++i) {
//             AutoPtr<IInteger64> elem;
//             p2->Get(i, (IInterface**)&elem);
//             Int64 realNum;
//             elem->GetValue(&realNum);
//             sencond->Set(i, realNum);
//         }
//         EqualsInt64(first, sencond, &result);
//         return result;
//     }
//     if (riid1 == EIID_IFloat) {
//         p1->GetLength(&size);
//         AutoPtr<ArrayOf<Float> > first = ArrayOf<Float>::Alloc(size);
//         for (Int32 i = 0; i < size; ++i) {
//             AutoPtr<IFloat> elem;
//             p1->Get(i, (IInterface**)&elem);
//             Float realNum;
//             elem->GetValue(&realNum);
//             first->Set(i, realNum);
//         }

//         p2->GetLength(&size);
//         AutoPtr<ArrayOf<Float> > sencond = ArrayOf<Float>::Alloc(size);
//         for (Int32 i = 0; i < size; ++i) {
//             AutoPtr<IFloat> elem;
//             p2->Get(i, (IInterface**)&elem);
//             Float realNum;
//             elem->GetValue(&realNum);
//             sencond->Set(i, realNum);
//         }
//         EqualsFloat(first, sencond, &result);
//         return result;
//     }
//     if (riid1 == EIID_IDouble) {
//         p1->GetLength(&size);
//         AutoPtr<ArrayOf<Double> > first = ArrayOf<Double>::Alloc(size);
//         for (Int32 i = 0; i < size; ++i) {
//             AutoPtr<IDouble> elem;
//             p1->Get(i, (IInterface**)&elem);
//             Double realNum;
//             elem->GetValue(&realNum);
//             first->Set(i, realNum);
//         }

//         p2->GetLength(&size);
//         AutoPtr<ArrayOf<Double> > sencond = ArrayOf<Double>::Alloc(size);
//         for (Int32 i = 0; i < size; ++i) {
//             AutoPtr<IDouble> elem;
//             p2->Get(i, (IInterface**)&elem);
//             Double realNum;
//             elem->GetValue(&realNum);
//             sencond->Set(i, realNum);
//         }
//         EqualsDouble(first, sencond, &result);
//         return result;
//     }
//     if (riid1 == EIID_IInteger16) {
//         p1->GetLength(&size);
//         AutoPtr<ArrayOf<Int16> > first = ArrayOf<Int16>::Alloc(size);
//         for (Int32 i = 0; i < size; ++i) {
//             AutoPtr<IInteger16> elem;
//             p1->Get(i, (IInterface**)&elem);
//             Int16 realNum;
//             elem->GetValue(&realNum);
//             first->Set(i, realNum);
//         }

//         p2->GetLength(&size);
//         AutoPtr<ArrayOf<Int16> > sencond = ArrayOf<Int16>::Alloc(size);
//         for (Int32 i = 0; i < size; ++i) {
//             AutoPtr<IInteger16> elem;
//             p2->Get(i, (IInterface**)&elem);
//             Int16 realNum;
//             elem->GetValue(&realNum);
//             sencond->Set(i, realNum);
//         }
//         EqualsInt16(first, sencond, &result);
//         return result;
//     }
//     p1->GetLength(&size);
//     AutoPtr<ArrayOf<IInterface*> > first = ArrayOf<IInterface*>::Alloc(size);
//     for (Int32 i = 0; i < size; ++i) {
//         AutoPtr<IInterface> elem;
//         p1->Get(i, (IInterface**)&elem);
//         first->Set(i, elem);
//     }

//     p2->GetLength(&size);
//     AutoPtr<ArrayOf<IInterface*> > sencond = ArrayOf<IInterface*>::Alloc(size);
//     for (Int32 i = 0; i < size; ++i) {
//         AutoPtr<IInterface> elem;
//         p2->Get(i, (IInterface**)&elem);
//         sencond->Set(i, elem);
//     }
//     DeepEquals(first, sencond, &result);
//     return result;
// }

// ECode Arrays::SortByte(
//     /* [in] */ ArrayOf<Byte>* array)
// {
//     return DualPivotQuicksort::Sort(array);
// }

// ECode Arrays::SortByte(
//     /* [in] */ ArrayOf<Byte>* array,
//     /* [in] */ Int32 start,
//     /* [in] */ Int32 end)
// {
//     return DualPivotQuicksort::Sort(array, start, end);
// }

// ECode Arrays::SortChar32(
//     /* [in] */ ArrayOf<Char32>* array)
// {
//     return DualPivotQuicksort::Sort(array);
// }

// ECode Arrays::SortChar32(
//     /* [in] */ ArrayOf<Char32>* array,
//     /* [in] */ Int32 start,
//     /* [in] */ Int32 end)
// {
//     return DualPivotQuicksort::Sort(array, start, end);
// }

// ECode Arrays::SortDouble(
//     /* [in] */ ArrayOf<Double>* array)
// {
//     return DualPivotQuicksort::Sort(array);
// }

// ECode Arrays::SortDouble(
//     /* [in] */ ArrayOf<Double>* array,
//     /* [in] */ Int32 start,
//     /* [in] */ Int32 end)
// {
//     return DualPivotQuicksort::Sort(array, start, end);
// }

// ECode Arrays::SortFloat(
//     /* [in] */ ArrayOf<Float>* array)
// {
//     return DualPivotQuicksort::Sort(array);
// }

// ECode Arrays::SortFloat(
//     /* [in] */ ArrayOf<Float>* array,
//     /* [in] */ Int32 start,
//     /* [in] */ Int32 end)
// {
//     return DualPivotQuicksort::Sort(array, start, end);
// }

// ECode Arrays::SortInt32(
//     /* [in] */ ArrayOf<Int32>* array)
// {
//     return DualPivotQuicksort::Sort(array);
// }

// ECode Arrays::SortInt32(
//     /* [in] */ ArrayOf<Int32>* array,
//     /* [in] */ Int32 start,
//     /* [in] */ Int32 end)
// {
//     return DualPivotQuicksort::Sort(array, start, end);
// }

// ECode Arrays::SortInt64(
//     /* [in] */ ArrayOf<Int64>* array)
// {
//     return DualPivotQuicksort::Sort(array);
// }

// ECode Arrays::SortInt64(
//     /* [in] */ ArrayOf<Int64>* array,
//     /* [in] */ Int32 start,
//     /* [in] */ Int32 end)
// {
//     return DualPivotQuicksort::Sort(array, start, end);
// }

// ECode Arrays::SortInt16(
//     /* [in] */ ArrayOf<Int16>* array)
// {
//     return DualPivotQuicksort::Sort(array);
// }

// ECode Arrays::SortInt16(
//     /* [in] */ ArrayOf<Int16>* array,
//     /* [in] */ Int32 start,
//     /* [in] */ Int32 end)
// {
//     return DualPivotQuicksort::Sort(array, start, end);
// }

// ECode Arrays::SortObject(
//     /* [in] */ ArrayOf<IInterface* >* array)
// {
//     return ComparableTimSort::Sort(*array);
// }

// ECode Arrays::SortObject(
//     /* [in] */ ArrayOf<IInterface* >* array,
//     /* [in] */ Int32 start,
//     /* [in] */ Int32 end)
// {
//     return ComparableTimSort::Sort(*array, start, end);
// }

// ECode Arrays::Sort(
//     /* [in] */ ArrayOf<IInterface* >* array,
//     /* [in] */ Int32 start,
//     /* [in] */ Int32 end,
//     /* [in] */ IComparator* comparator)
// {
//     return TimSort::Sort(array, start, end, comparator);
// }

// ECode Arrays::Sort(
//     /* [in] */ ArrayOf<IInterface* >* array,
//     /* [in] */ IComparator* comparator)
// {
//     return TimSort::Sort(array, comparator);
// }

// ECode Arrays::ToStringBoolean(
//     /* [in] */ ArrayOf<Boolean>* array,
//     /* [out] */ String* str)
// {
//     VALIDATE_NOT_NULL(str)
//     if (array == NULL) {
//         *str = String("NULL");
//         return NOERROR;
//     }
//     if (array->GetLength() == 0) {
//         *str = String("[]");
//         return NOERROR;
//     }
//     StringBuilder sb(array->GetLength() * 7);
//     sb.AppendChar('[');
//     sb.Append((*array)[0]);
//     for (Int32 i = 1; i < array->GetLength(); i++) {
//         sb.Append(", ");
//         sb.Append((*array)[i]);
//     }
//     sb.AppendChar(']');
//     *str = sb.ToString();
//     return NOERROR;
// }

// ECode Arrays::ToStringByte(
//     /* [in] */ ArrayOf<Byte>* array,
//     /* [out] */ String* str)
// {
//     VALIDATE_NOT_NULL(str)
//     if (array == NULL) {
//         *str = String("NULL");
//         return NOERROR;
//     }
//     if (array->GetLength() == 0) {
//         *str = String("[]");
//         return NOERROR;
//     }
//     StringBuilder sb(array->GetLength() * 6);
//     sb.AppendChar('[');
//     sb.Append((*array)[0]);
//     for (Int32 i = 1; i < array->GetLength(); i++) {
//         sb.Append(", ");
//         sb.Append((*array)[i]);
//     }
//     sb.AppendChar(']');
//     *str = sb.ToString();
//     return NOERROR;
// }

// ECode Arrays::ToStringChar32(
//     /* [in] */ ArrayOf<Char32>* array,
//     /* [out] */ String* str)
// {
//     VALIDATE_NOT_NULL(str)
//     if (array == NULL) {
//         *str = String("NULL");
//         return NOERROR;
//     }
//     if (array->GetLength() == 0) {
//         *str = String("[]");
//         return NOERROR;
//     }
//     StringBuilder sb(array->GetLength() * 3);
//     sb.AppendChar('[');
//     sb.AppendChar((*array)[0]);
//     for (Int32 i = 1; i < array->GetLength(); i++) {
//         sb.Append(", ");
//         sb.AppendChar((*array)[i]);
//     }
//     sb.AppendChar(']');
//     *str = sb.ToString();
//     return NOERROR;
// }

// ECode Arrays::ToStringDouble(
//     /* [in] */ ArrayOf<Double>* array,
//     /* [out] */ String* str)
// {
//     VALIDATE_NOT_NULL(str)
//     if (array == NULL) {
//         *str = String("NULL");
//         return NOERROR;
//     }
//     if (array->GetLength() == 0) {
//         *str = String("[]");
//         return NOERROR;
//     }
//     StringBuilder sb(array->GetLength() * 7);
//     sb.AppendChar('[');
//     sb.Append((*array)[0]);
//     for (Int32 i = 1; i < array->GetLength(); i++) {
//         sb.Append(", ");
//         sb.Append((*array)[i]);
//     }
//     sb.AppendChar(']');
//     *str = sb.ToString();
//     return NOERROR;
// }

// ECode Arrays::ToStringFloat(
//     /* [in] */ ArrayOf<Float>* array,
//     /* [out] */ String* str)
// {
//     VALIDATE_NOT_NULL(str)
//     if (array == NULL) {
//         *str = String("NULL");
//         return NOERROR;
//     }
//     if (array->GetLength() == 0) {
//         *str = String("[]");
//         return NOERROR;
//     }
//     StringBuilder sb(array->GetLength() * 7);
//     sb.AppendChar('[');
//     sb.Append((*array)[0]);
//     for (Int32 i = 1; i < array->GetLength(); i++) {
//         sb.Append(", ");
//         sb.Append((*array)[i]);
//     }
//     sb.AppendChar(']');
//     *str = sb.ToString();
//     return NOERROR;
// }

// ECode Arrays::ToStringInt32(
//     /* [in] */ ArrayOf<Int32>* array,
//     /* [out] */ String* str)
// {
//     VALIDATE_NOT_NULL(str)
//     if (array == NULL) {
//         *str = String("NULL");
//         return NOERROR;
//     }
//     if (array->GetLength() == 0) {
//         *str = String("[]");
//         return NOERROR;
//     }
//     StringBuilder sb(array->GetLength() * 6);
//     sb.AppendChar('[');
//     sb.Append((*array)[0]);
//     for (Int32 i = 1; i < array->GetLength(); i++) {
//         sb.Append(", ");
//         sb.Append((*array)[i]);
//     }
//     sb.AppendChar(']');
//     *str = sb.ToString();
//     return NOERROR;
// }

// ECode Arrays::ToStringInt64(
//     /* [in] */ ArrayOf<Int64>* array,
//     /* [out] */ String* str)
// {
//     VALIDATE_NOT_NULL(str)
//     if (array == NULL) {
//         *str = String("NULL");
//         return NOERROR;
//     }
//     if (array->GetLength() == 0) {
//         *str = String("[]");
//         return NOERROR;
//     }
//     StringBuilder sb(array->GetLength() * 6);
//     sb.AppendChar('[');
//     sb.Append((*array)[0]);
//     for (Int32 i = 1; i < array->GetLength(); i++) {
//         sb.Append(", ");
//         sb.Append((*array)[i]);
//     }
//     sb.AppendChar(']');
//     *str = sb.ToString();
//     return NOERROR;
// }

// ECode Arrays::ToStringInt16(
//     /* [in] */ ArrayOf<Int16>* array,
//     /* [out] */ String* str)
// {
//     VALIDATE_NOT_NULL(str)
//     if (array == NULL) {
//         *str = String("NULL");
//         return NOERROR;
//     }
//     if (array->GetLength() == 0) {
//         *str = String("[]");
//         return NOERROR;
//     }
//     StringBuilder sb(array->GetLength() * 6);
//     sb.Append('[');
//     sb.Append((*array)[0]);
//     for (Int32 i = 1; i < array->GetLength(); i++) {
//         sb.Append(", ");
//         sb.Append((*array)[i]);
//     }
//     sb.Append(']');
//     *str = sb.ToString();
//     return NOERROR;
// }

// ECode Arrays::ToStringObject(
//     /* [in] */ ArrayOf<IInterface* >* array,
//     /* [out] */ String* str)
// {
//     VALIDATE_NOT_NULL(str)
//     if (array == NULL) {
//         *str = String("NULL");
//         return NOERROR;
//     }
//     if (array->GetLength() == 0) {
//         *str = String("[]");
//         return NOERROR;
//     }
//     StringBuilder sb(array->GetLength() * 7);
//     sb.Append('[');
//     sb.Append((*array)[0]);
//     for (Int32 i = 1; i < array->GetLength(); i++) {
//         sb.Append(", ");
//         sb.Append((*array)[i]);
//     }
//     sb.Append(']');
//     *str = sb.ToString();
//     return NOERROR;
// }

// ECode Arrays::DeepToString(
//     /* [in] */ ArrayOf<IInterface* >* array,
//     /* [out] */ String* str)
// {
//     VALIDATE_NOT_NULL(str)
//     // Special case null to prevent NPE
//     if (array == NULL) {
//         *str = String("NULL");
//         return NOERROR;
//     }
//     // delegate this to the recursive method
//     StringBuilder buf(array->GetLength() * 9);
//     AutoPtr<ArrayOf<IInterface*> > origArrays = ArrayOf<IInterface*>::Alloc(1);
//     AutoPtr<IArrayOf> pArray;
//     CArrayOf::New(EIID_IInterface, array->GetLength(), (IArrayOf**)&pArray);
//     for (Int32 i = 0; i < array->GetLength(); ++i) {
//         pArray->Put(i, (*array)[i]);
//     }
//     origArrays->Set(0, pArray);
//     DeepToStringImpl(array, origArrays, &buf);
//     *str = buf.ToString();
//     return NOERROR;
// }

// void Arrays::DeepToStringImpl(
//     /* [in] */ ArrayOf<IInterface* >* array,
//     /* [in] */ ArrayOf<IInterface* >* origArrays,
//     /* [in] */ StringBuilder* sb)
// {
//     if (array == NULL) {
//         sb->Append("NULL");
//         return;
//     }

//     sb->AppendChar('[');

//     for (Int32 i = 0; i < array->GetLength(); i++) {
//         if (i != 0) {
//             sb->Append(", ");
//         }
//         // establish current element
//         AutoPtr<IInterface> elem = (*array)[i];
//         if (elem == NULL) {
//             // element is null
//             sb->Append("NULL");
//         }
//         else {
//             // get the Class of the current element
//             if (IArrayOf::Probe(elem) != NULL) {
//                 // element is an array type

//                 // get the declared Class of the array (element)
//                 AutoPtr<IArrayOf> p = IArrayOf::Probe(elem);
//                 InterfaceID riid;
//                 p->GetTypeId(&riid);
//                 Int32 length;
//                 p->GetLength(&length);
//                 String str;
//                 // element is a primitive array
//                 if (riid == EIID_IBoolean) {
//                     AutoPtr<ArrayOf<Boolean> > tmpArray = ArrayOf<Boolean>::Alloc(length);
//                     for (Int32 i = 0; i < length; ++i) {
//                         AutoPtr<IBoolean> pContent;
//                         p->Get(i, (IInterface**)&pContent);
//                         Boolean content;
//                         pContent->GetValue(&content);
//                         tmpArray->Set(i, content);
//                     }
//                     ToStringBoolean((ArrayOf<Boolean>*) tmpArray, &str);
//                     sb->Append(str);
//                 }
//                 else if (riid == EIID_IByte) {
//                     AutoPtr<ArrayOf<Byte> > tmpArray = ArrayOf<Byte>::Alloc(length);
//                     for (Int32 i = 0; i < length; ++i) {
//                         AutoPtr<IByte> pContent;
//                         p->Get(i, (IInterface**)&pContent);
//                         Byte content;
//                         pContent->GetValue(&content);
//                         tmpArray->Set(i, content);
//                     }
//                     ToStringByte((ArrayOf<Byte>*) tmpArray, &str);
//                     sb->Append(str);
//                 }
//                 else if (riid == EIID_IChar32) {
//                     AutoPtr<ArrayOf<Char32> > tmpArray = ArrayOf<Char32>::Alloc(length);
//                     for (Int32 i = 0; i < length; ++i) {
//                         AutoPtr<IChar32> pContent;
//                         p->Get(i, (IInterface**)&pContent);
//                         Char32 content;
//                         pContent->GetValue(&content);
//                         tmpArray->Set(i, content);
//                     }
//                     ToStringChar32((ArrayOf<Char32>*) tmpArray, &str);
//                     sb->Append(str);
//                 }
//                 else if (riid == EIID_IDouble) {
//                     AutoPtr<ArrayOf<Double> > tmpArray = ArrayOf<Double>::Alloc(length);
//                     for (Int32 i = 0; i < length; ++i) {
//                         AutoPtr<IDouble> pContent;
//                         p->Get(i, (IInterface**)&pContent);
//                         Double content;
//                         pContent->GetValue(&content);
//                         tmpArray->Set(i, content);
//                     }
//                     ToStringDouble((ArrayOf<Double>*) tmpArray, &str);
//                     sb->Append(str);
//                 }
//                 else if (riid == EIID_IFloat) {
//                     AutoPtr<ArrayOf<Float> > tmpArray = ArrayOf<Float>::Alloc(length);
//                     for (Int32 i = 0; i < length; ++i) {
//                         AutoPtr<IFloat> pContent;
//                         p->Get(i, (IInterface**)&pContent);
//                         Float content;
//                         pContent->GetValue(&content);
//                         tmpArray->Set(i, content);
//                     }
//                     ToStringFloat((ArrayOf<Float>*) tmpArray, &str);
//                     sb->Append(str);
//                 }
//                 else if (riid == EIID_IInteger32) {
//                     AutoPtr<ArrayOf<Int32> > tmpArray = ArrayOf<Int32>::Alloc(length);
//                     for (Int32 i = 0; i < length; ++i) {
//                         AutoPtr<IInteger32> pContent;
//                         p->Get(i, (IInterface**)&pContent);
//                         Int32 content;
//                         pContent->GetValue(&content);
//                         tmpArray->Set(i, content);
//                     }
//                     ToStringInt32((ArrayOf<Int32>*) tmpArray, &str);
//                     sb->Append(str);
//                 }
//                 else if (riid == EIID_IInteger64) {
//                     AutoPtr<ArrayOf<Int64> > tmpArray = ArrayOf<Int64>::Alloc(length);
//                     for (Int32 i = 0; i < length; ++i) {
//                         AutoPtr<IInteger64> pContent;
//                         p->Get(i, (IInterface**)&pContent);
//                         Int64 content;
//                         pContent->GetValue(&content);
//                         tmpArray->Set(i, content);
//                     }
//                     ToStringInt64((ArrayOf<Int64>*) tmpArray, &str);
//                     sb->Append(str);
//                 }
//                 else if (riid == EIID_IInteger16) {
//                     AutoPtr<ArrayOf<Int16> > tmpArray = ArrayOf<Int16>::Alloc(length);
//                     for (Int32 i = 0; i < length; ++i) {
//                         AutoPtr<IInteger16> pContent;
//                         p->Get(i, (IInterface**)&pContent);
//                         Int16 content;
//                         pContent->GetValue(&content);
//                         tmpArray->Set(i, content);
//                     }
//                     ToStringInt16((ArrayOf<Int16>*) tmpArray, &str);
//                     sb->Append(str);
//                 }
//                 else if (riid == EIID_IInterface) {
//                     // element is an Object[], so we assert that
//                     //assert elem instanceof Object[];
//                     if (DeepToStringImplContains(origArrays, elem)) {
//                         sb->Append("[...]");
//                     }
//                     else {
//                         AutoPtr<ArrayOf<IInterface*> > newArray = ArrayOf<IInterface*>::Alloc(length);
//                         for (Int32 i = 0; i < length; ++i) {
//                             AutoPtr<IInterface> pContent;
//                             p->Get(i, (IInterface**)&pContent);
//                             newArray->Set(i, pContent);
//                         }
//                         AutoPtr<ArrayOf<IInterface*> > newOrigArrays = ArrayOf<IInterface*>::Alloc(origArrays->GetLength() + 1);
//                         newOrigArrays->Copy(origArrays, origArrays->GetLength());
//                         newOrigArrays->Set(origArrays->GetLength(), elem);
//                         // make the recursive call to this method
//                         DeepToStringImpl(newArray, newOrigArrays, sb);
//                     }
//                 }
//                 // else {
//                 //     // no other possible primitives, so we assert that
//                 //     throw new AssertionError();
//                 // }
//             }
//             else { // element is NOT an array, just an Object
//                 sb->Append((*array)[i]);
//             }
//         }
//     }
//     sb->AppendChar(']');
// }

// Boolean Arrays::DeepToStringImplContains(
//     /* [in] */ ArrayOf<IInterface* >* origArrays,
//     /* [in] */ IInterface* array)
// {
//     if (origArrays == NULL || origArrays->GetLength() == 0) {
//         return FALSE;
//     }
//     for (Int32 i = 0; i < origArrays->GetLength(); i++) {
//         AutoPtr<IInterface> element = (*origArrays)[i];
//         if (element.Get() == array) {
//             return TRUE;
//         }
//     }
//     return FALSE;
// }

// ECode Arrays::CopyOfBoolean(
//     /* [in] */ ArrayOf<Boolean>* original,
//     /* [in] */ Int32 newLength,
//     /* [out, callee] */ ArrayOf<Boolean>** result)
// {
//     VALIDATE_NOT_NULL(result)
//     if (newLength < 0) {
//         return E_NEGATIVE_ARRAY_SIZE_EXCEPTION;
//     }
//     return CopyOfRangeBoolean(original, 0, newLength, result);
// }

// ECode Arrays::CopyOfByte(
//     /* [in] */ ArrayOf<Byte>* original,
//     /* [in] */ Int32 newLength,
//     /* [out, callee] */ ArrayOf<Byte>** result)
// {
//     VALIDATE_NOT_NULL(result)
//     if (newLength < 0) {
//         return E_NEGATIVE_ARRAY_SIZE_EXCEPTION;
//     }
//     return CopyOfRangeByte(original, 0, newLength, result);
// }

// ECode Arrays::CopyOfChar32(
//     /* [in] */ ArrayOf<Char32>* original,
//     /* [in] */ Int32 newLength,
//     /* [out, callee] */ ArrayOf<Char32>** result)
// {
//     VALIDATE_NOT_NULL(result)
//     if (newLength < 0) {
//         return E_NEGATIVE_ARRAY_SIZE_EXCEPTION;
//     }
//     return CopyOfRangeChar32(original, 0, newLength, result);
// }

// ECode Arrays::CopyOfDouble(
//     /* [in] */ ArrayOf<Double>* original,
//     /* [in] */ Int32 newLength,
//     /* [out, callee] */ ArrayOf<Double>** result)
// {
//     VALIDATE_NOT_NULL(result)
//     if (newLength < 0) {
//         return E_NEGATIVE_ARRAY_SIZE_EXCEPTION;
//     }
//     return CopyOfRangeDouble(original, 0, newLength, result);
// }

// ECode Arrays::CopyOfFloat(
//     /* [in] */ ArrayOf<Float>* original,
//     /* [in] */ Int32 newLength,
//     /* [out, callee] */ ArrayOf<Float>** result)
// {
//     VALIDATE_NOT_NULL(result)
//     if (newLength < 0) {
//         return E_NEGATIVE_ARRAY_SIZE_EXCEPTION;
//     }
//     return CopyOfRangeFloat(original, 0, newLength, result);
// }

// ECode Arrays::CopyOfInt32(
//     /* [in] */ ArrayOf<Int32>* original,
//     /* [in] */ Int32 newLength,
//     /* [out, callee] */ ArrayOf<Int32>** result)
// {
//     VALIDATE_NOT_NULL(result)
//     if (newLength < 0) {
//         return E_NEGATIVE_ARRAY_SIZE_EXCEPTION;
//     }
//     return CopyOfRangeInt32(original, 0, newLength, result);
// }

// ECode Arrays::CopyOfInt64(
//     /* [in] */ ArrayOf<Int64>* original,
//     /* [in] */ Int32 newLength,
//     /* [out, callee] */ ArrayOf<Int64>** result)
// {
//     VALIDATE_NOT_NULL(result)
//     if (newLength < 0) {
//         return E_NEGATIVE_ARRAY_SIZE_EXCEPTION;
//     }
//     return CopyOfRangeInt64(original, 0, newLength, result);
// }

// ECode Arrays::CopyOfInt16(
//     /* [in] */ ArrayOf<Int16>* original,
//     /* [in] */ Int32 newLength,
//     /* [out, callee] */ ArrayOf<Int16>** result)
// {
//     VALIDATE_NOT_NULL(result)
//     if (newLength < 0) {
//         return E_NEGATIVE_ARRAY_SIZE_EXCEPTION;
//     }
//     return CopyOfRangeInt16(original, 0, newLength, result);
// }

// ECode Arrays::CopyOf(
//     /* [in] */ ArrayOf<IInterface* >* original,
//     /* [in] */ Int32 newLength,
//     /* [out, callee] */ ArrayOf<IInterface* >** result)
// {
//     VALIDATE_NOT_NULL(result)
//     if (original == NULL) {
//         return E_NULL_POINTER_EXCEPTION;
//     }
//     if (newLength < 0) {
//         return E_NEGATIVE_ARRAY_SIZE_EXCEPTION;
//     }
//     return CopyOfRange(original, 0, newLength, result);
// }

// // ECode Arrays::_CopyOf(
// //     /* [in] */ U[] original,
// //     /* [in] */ Int32 newLength,
// //     /* [in] */ Class<? extends T[]> newType,
// //     /* [out, callee] */ <T, U> T[] result)
// // {
// //     // We use the null pointer check in copyOfRange for exception priority compatibility.
// //     if (newLength < 0) {
// //         return E_NEGATIVE_ARRAY_SIZE_EXCEPTION;
// //     }
// //     return _CopyOfRange(original, 0, newLength, newType, result);
// // }

// ECode Arrays::CopyOfRangeBoolean(
//     /* [in] */ ArrayOf<Boolean>* original,
//     /* [in] */ Int32 start,
//     /* [in] */ Int32 end,
//     /* [out, callee] */ ArrayOf<Boolean>** arrayCopy)
// {
//     VALIDATE_NOT_NULL(arrayCopy)
//     if (start > end) {
//         return E_ILLEGAL_ARGUMENT_EXCEPTION;
//     }
//     Int32 originalLength = original->GetLength();
//     if (start < 0 || start > originalLength) {
//         return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//     }
//     Int32 resultLength = end - start;
//     Int32 copyLength = Elastos::Core::Math::Min(resultLength, originalLength - start);
//     AutoPtr<ArrayOf<Boolean> > result = ArrayOf<Boolean>::Alloc(resultLength);
//     result->Copy(0, original, start, copyLength);
//     *arrayCopy = result;
//     REFCOUNT_ADD(*arrayCopy)
//     return NOERROR;
// }

// ECode Arrays::CopyOfRangeByte(
//     /* [in] */ ArrayOf<Byte>* original,
//     /* [in] */ Int32 start,
//     /* [in] */ Int32 end,
//     /* [out, callee] */ ArrayOf<Byte>** arrayCopy)
// {
//     VALIDATE_NOT_NULL(arrayCopy)
//     if (start > end) {
//         return E_ILLEGAL_ARGUMENT_EXCEPTION;
//     }
//     Int32 originalLength = original->GetLength();
//     if (start < 0 || start > originalLength) {
//         return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//     }
//     Int32 resultLength = end - start;
//     Int32 copyLength = Elastos::Core::Math::Min(resultLength, originalLength - start);
//     AutoPtr<ArrayOf<Byte> > result = ArrayOf<Byte>::Alloc(resultLength);
//     result->Copy(0, original, start, copyLength);
//     *arrayCopy = result;
//     REFCOUNT_ADD(*arrayCopy)
//     return NOERROR;
// }

// ECode Arrays::CopyOfRangeChar32(
//     /* [in] */ ArrayOf<Char32>* original,
//     /* [in] */ Int32 start,
//     /* [in] */ Int32 end,
//     /* [out, callee] */ ArrayOf<Char32>** arrayCopy)
// {
//     VALIDATE_NOT_NULL(arrayCopy)
//     if (start > end) {
//         return E_ILLEGAL_ARGUMENT_EXCEPTION;
//     }
//     Int32 originalLength = original->GetLength();
//     if (start < 0 || start > originalLength) {
//         return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//     }
//     Int32 resultLength = end - start;
//     Int32 copyLength = Elastos::Core::Math::Min(resultLength, originalLength - start);
//     AutoPtr<ArrayOf<Char32> > result = ArrayOf<Char32>::Alloc(resultLength);
//     result->Copy(0, original, start, copyLength);
//     *arrayCopy = result;
//     REFCOUNT_ADD(*arrayCopy)
//     return NOERROR;
// }

// ECode Arrays::CopyOfRangeDouble(
//     /* [in] */ ArrayOf<Double>* original,
//     /* [in] */ Int32 start,
//     /* [in] */ Int32 end,
//     /* [out, callee] */ ArrayOf<Double>** arrayCopy)
// {
//     VALIDATE_NOT_NULL(arrayCopy)
//     if (start > end) {
//         return E_ILLEGAL_ARGUMENT_EXCEPTION;
//     }
//     Int32 originalLength = original->GetLength();
//     if (start < 0 || start > originalLength) {
//         return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//     }
//     Int32 resultLength = end - start;
//     Int32 copyLength = Elastos::Core::Math::Min(resultLength, originalLength - start);
//     AutoPtr<ArrayOf<Double> > result = ArrayOf<Double>::Alloc(resultLength);
//     result->Copy(0, original, start, copyLength);
//     *arrayCopy = result;
//     REFCOUNT_ADD(*arrayCopy)
//     return NOERROR;
// }

// ECode Arrays::CopyOfRangeFloat(
//     /* [in] */ ArrayOf<Float>* original,
//     /* [in] */ Int32 start,
//     /* [in] */ Int32 end,
//     /* [out, callee] */ ArrayOf<Float>** arrayCopy)
// {
//     VALIDATE_NOT_NULL(arrayCopy)
//     if (start > end) {
//         return E_ILLEGAL_ARGUMENT_EXCEPTION;
//     }
//     Int32 originalLength = original->GetLength();
//     if (start < 0 || start > originalLength) {
//         return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//     }
//     Int32 resultLength = end - start;
//     Int32 copyLength = Elastos::Core::Math::Min(resultLength, originalLength - start);
//     AutoPtr<ArrayOf<Float> > result = ArrayOf<Float>::Alloc(resultLength);
//     result->Copy(0, original, start, copyLength);
//     *arrayCopy = result;
//     REFCOUNT_ADD(*arrayCopy)
//     return NOERROR;
// }

// ECode Arrays::CopyOfRangeInt32(
//     /* [in] */ ArrayOf<Int32>* original,
//     /* [in] */ Int32 start,
//     /* [in] */ Int32 end,
//     /* [out, callee] */ ArrayOf<Int32>** arrayCopy)
// {
//     VALIDATE_NOT_NULL(arrayCopy)
//     if (start > end) {
//         return E_ILLEGAL_ARGUMENT_EXCEPTION;
//     }
//     Int32 originalLength = original->GetLength();
//     if (start < 0 || start > originalLength) {
//         return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//     }
//     Int32 resultLength = end - start;
//     Int32 copyLength = Elastos::Core::Math::Min(resultLength, originalLength - start);
//     AutoPtr<ArrayOf<Int32> > result = ArrayOf<Int32>::Alloc(resultLength);
//     result->Copy(0, original, start, copyLength);
//     *arrayCopy = result;
//     REFCOUNT_ADD(*arrayCopy)
//     return NOERROR;
// }

// ECode Arrays::CopyOfRangeInt64(
//     /* [in] */ ArrayOf<Int64>* original,
//     /* [in] */ Int32 start,
//     /* [in] */ Int32 end,
//     /* [out, callee] */ ArrayOf<Int64>** arrayCopy)
// {
//     VALIDATE_NOT_NULL(arrayCopy)
//     if (start > end) {
//         return E_ILLEGAL_ARGUMENT_EXCEPTION;
//     }
//     Int32 originalLength = original->GetLength();
//     if (start < 0 || start > originalLength) {
//         return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//     }
//     Int32 resultLength = end - start;
//     Int32 copyLength = Elastos::Core::Math::Min(resultLength, originalLength - start);
//     AutoPtr<ArrayOf<Int64> > result = ArrayOf<Int64>::Alloc(resultLength);
//     result->Copy(0, original, start, copyLength);
//     *arrayCopy = result;
//     REFCOUNT_ADD(*arrayCopy)
//     return NOERROR;
// }

// ECode Arrays::CopyOfRangeInt16(
//     /* [in] */ ArrayOf<Int16>* original,
//     /* [in] */ Int32 start,
//     /* [in] */ Int32 end,
//     /* [out, callee] */ ArrayOf<Int16>** arrayCopy)
// {
//     VALIDATE_NOT_NULL(arrayCopy)
//     if (start > end) {
//         return E_ILLEGAL_ARGUMENT_EXCEPTION;
//     }
//     Int32 originalLength = original->GetLength();
//     if (start < 0 || start > originalLength) {
//         return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//     }
//     Int32 resultLength = end - start;
//     Int32 copyLength = Elastos::Core::Math::Min(resultLength, originalLength - start);
//     AutoPtr<ArrayOf<Int16> > result = ArrayOf<Int16>::Alloc(resultLength);
//     result->Copy(0, original, start, copyLength);
//     *arrayCopy = result;
//     REFCOUNT_ADD(*arrayCopy)
//     return NOERROR;
// }

// ECode Arrays::CopyOfRange(
//     /* [in] */ ArrayOf<IInterface* >* original,
//     /* [in] */ Int32 start,
//     /* [in] */ Int32 end,
//     /* [out, callee] */ ArrayOf<IInterface* >** arrayCopy)
// {
//     VALIDATE_NOT_NULL(arrayCopy)
//     Int32 originalLength = original->GetLength(); // For exception priority compatibility.
//     if (start > end) {
//         return E_ILLEGAL_ARGUMENT_EXCEPTION;
//     }
//     if (start < 0 || start > originalLength) {
//         return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//     }
//     Int32 resultLength = end - start;
//     Int32 copyLength = Elastos::Core::Math::Min(resultLength, originalLength - start);
//     AutoPtr<ArrayOf<IInterface*> > result = ArrayOf<IInterface*>::Alloc(resultLength);
//     result->Copy(0, original, start, copyLength);
//     *arrayCopy = result;
//     REFCOUNT_ADD(*arrayCopy)
//     return NOERROR;
// }

// ECode Arrays::_CopyOfRange(
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
//     REFCOUNT_ADD(*arrayCopy)
//     return NOERROR;
// }

} // namespace Utility
} // namespace Elastos
