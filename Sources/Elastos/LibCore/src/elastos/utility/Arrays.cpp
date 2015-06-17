
#include "Arrays.h"
#include "ComparableTimSort.h"
#include "TimSort.h"
// #include "CArrayList.h"

using Elastos::Core::Object;

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
//     /* [in] */ ArrayOf<IInterface *> * storage)
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
//     /* [out] */ ArrayOf<IInterface *> ** outArray)
// {
//     VALIDATE_NOT_NULL(outArray)
//     *outArray = mA->Clone();
//     REFCOUNT_ADD(*outArray)
//     return NOERROR;
// }

// ECode Arrays::ArrayList::ToArray(
//     /* [in] */ ArrayOf<IInterface *> * contents,
//     /* [out] */ ArrayOf<IInterface *> ** outArray)
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

ECode Arrays::AsList(
    /* [in] */ ArrayOf<IInterface*>* array,
    /* [out] */ IList** outList)
{
    VALIDATE_NOT_NULL(outList)
    *outList = NULL;

    if (array) {
        // return new ArrayList<T>(array);
        // AutoPtr<CArrayList> al;
        // CArrayList::NewByFriend(array->GetLength(), (CArrayList**)&al);

        // for (Int32 i = 0; i < array->GetLength(); ++i) {
        //     al->Add(i, (*array)[i]);
        // }

        // *outList = (IList*)al->Probe(EIID_IList);
        // REFCOUNT_ADD(*outList)
    }

    return NOERROR;
}

ECode Arrays::BinarySearch(
    /* [in] */ ArrayOf<IInterface *> * array,
    /* [in] */ Int32 startIndex,
    /* [in] */ Int32 endIndex,
    /* [in] */ IInterface * value,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index)
    *index = -1;
    assert(array != NULL);
    VALIDATE_NOT_NULL(array)

    ECode ec = CheckBinarySearchBounds(startIndex, endIndex, array->GetLength());
    if (FAILED(ec)) {
        ALOGE("Arrays::BinarySearch: error %08x, startIndex: %d, endIndex: %d, array length: %d",
            ec, startIndex, endIndex, array->GetLength());
        return ec;
    }

    Int32 lo = startIndex;
    Int32 hi = endIndex - 1;
    Int32 mid, midValCmp;
    IComparable* comp;
    while (lo <= hi) {
        mid = (UInt32(lo + hi)) >> 1;
        comp = IComparable::Probe((*array)[mid]);
        if (comp == NULL) {
            ALOGE("Arrays::BinarySearch: object at %d does not implement IComparable.", mid);
        }
        assert(comp != NULL);
        comp->CompareTo(value, &midValCmp);

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

ECode Arrays::BinarySearch(
    /* [in] */ AutoPtr<ArrayOf<IInterface *> >& array,
    /* [in] */ Int32 startIndex,
    /* [in] */ Int32 endIndex,
    /* [in] */ IInterface * value,
    /* [out] */ Int32* index)
{
    return BinarySearch(array.Get(), startIndex, endIndex, value, index);
}

ECode Arrays::BinarySearch(
    /* [in] */ ArrayOf<IInterface *> * array,
    /* [in] */ IInterface * value,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index)
    *index = -1;
    assert(array != NULL);
    VALIDATE_NOT_NULL(array)

    return BinarySearch(array, 0, array->GetLength(), value, index);
}

ECode Arrays::BinarySearch(
    /* [in] */ AutoPtr<ArrayOf<IInterface *> >& array,
    /* [in] */ IInterface * value,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index)
    *index = -1;
    assert(array != NULL);
    VALIDATE_NOT_NULL(array)

    return BinarySearch(array.Get(), 0, array->GetLength(), value, index);
}

ECode Arrays::BinarySearch(
    /* [in] */ ArrayOf<IInterface *> * array,
    /* [in] */ Int32 startIndex,
    /* [in] */ Int32 endIndex,
    /* [in] */ IInterface* value,
    /* [in] */ IComparator* comparator,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index)
    *index = -1;
    assert(array != NULL);
    VALIDATE_NOT_NULL(array)

    if (comparator == NULL) {
        return BinarySearch(array, startIndex, endIndex, value, index);
    }

    FAIL_RETURN(CheckBinarySearchBounds(startIndex, endIndex, array->GetLength()));
    Int32 lo = startIndex;
    Int32 hi = endIndex - 1;
    Int32 mid, midValCmp;

    while (lo <= hi) {
        mid = (UInt32(lo + hi)) >> 1;
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

ECode Arrays::BinarySearch(
    /* [in] */ AutoPtr<ArrayOf<IInterface *> >& array,
    /* [in] */ Int32 startIndex,
    /* [in] */ Int32 endIndex,
    /* [in] */ IInterface* value,
    /* [in] */ IComparator* comparator,
    /* [out] */ Int32* index)
{
    return BinarySearch(array.Get(), startIndex, endIndex, value, comparator, index);
}

ECode Arrays::BinarySearch(
    /* [in] */ ArrayOf<IInterface *> * array,
    /* [in] */ IInterface* value,
    /* [in] */ IComparator* comparator,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index)
    *index = -1;
    assert(array != NULL);
    VALIDATE_NOT_NULL(array)

    return BinarySearch(array, 0, array->GetLength(), value, comparator, index);
}

ECode Arrays::BinarySearch(
    /* [in] */ AutoPtr<ArrayOf<IInterface *> >& array,
    /* [in] */ IInterface* value,
    /* [in] */ IComparator* comparator,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index)
    *index = -1;
    assert(array != NULL);
    VALIDATE_NOT_NULL(array)

    return BinarySearch(array.Get(), 0, array->GetLength(), value, comparator, index);
}

Int32 Arrays::GetHashCode(
    /* [in] */ ArrayOf<Boolean>* array)
{
    if (array == NULL) {
        return 0;
    }

    Int32 hashCode = 1;
    for (Int32 i = 0; i < array->GetLength(); i++) {
        // 1231, 1237 are hash code values for boolean value
        hashCode = 31 * hashCode + ((*array)[i] ? 1231 : 1237);
    }
    return hashCode;
}

Int32 Arrays::GetHashCode(
    /* [in] */ ArrayOf<Int64>* array)
{
    if (array == NULL) {
        return 0;
    }

    Int32 hashCode = 1;
    Int64 elementValue;
    for (Int32 i = 0; i < array->GetLength(); i++) {
        elementValue = (*array)[i];
        /*
         * the hash code value for long value is (int) (value ^ (value >>>
         * 32))
         */
        hashCode = 31 * hashCode
                + (Int32) (elementValue ^ (elementValue >> 32));
    }
    return hashCode;
}

Int32 Arrays::GetHashCode(
    /* [in] */ ArrayOf<Float>* array)
{
    if (array == NULL) {
        return 0;
    }

    Int32 hashCode = 1;
    for (Int32 i = 0; i < array->GetLength(); i++) {
        /*
         * the hash code value for float value is
         * Float.floatToIntBits(value)
         */
        hashCode = 31 * hashCode + Elastos::Core::Math::FloatToInt32Bits((*array)[i]);
    }
    return hashCode;
}

Int32 Arrays::GetHashCode(
    /* [in] */ ArrayOf<Double>* array)
{
    if (array == NULL) {
        return 0;
    }

    Int32 hashCode = 1;
    Int64 v;
    for (Int32 i = 0; i < array->GetLength(); i++) {
        v = Elastos::Core::Math::DoubleToInt64Bits((*array)[i]);
        /*
         * the hash code value for double value is (int) (v ^ (v >>> 32))
         * where v = Double.doubleToLongBits(value)
         */
        hashCode = 31 * hashCode + (Int32) (v ^ (v >> 32));
    }
    return hashCode;
}

Int32 Arrays::GetHashCode(
    /* [in] */ ArrayOf<IInterface *> * array)
{
    if (array == NULL) {
        return 0;
    }

    Int32 hashCode = 1;
    IInterface* element;
    for (Int32 i = 0; i < array->GetLength(); i++) {
        element = (*array)[i];
        hashCode = 31 * hashCode + Object::GetHashCode(element);
    }
    return hashCode;
}

Int32 Arrays::GetHashCode(
    /* [in] */ AutoPtr<ArrayOf<IInterface *> >& array)
{
    return GetHashCode(array.Get());
}

Int32 Arrays::DeepGetHashCode(
    /* [in] */ ArrayOf<IInterface *> * array)
{
    if (array == NULL) {
        return 0;
    }

    Int32 hashCode = 1, elementHashCode;
    for (Int32 i = 0; i < array->GetLength(); i++) {
        elementHashCode = DeepGetHashCode((*array)[i]);
        hashCode = 31 * hashCode + elementHashCode;
    }
    return hashCode;
}

Int32 Arrays::DeepGetHashCode(
    /* [in] */ IInterface* element)
{
    if (element == NULL) {
        return 0;
    }

    if (IArrayOf::Probe(element) == NULL) {
        return Object::GetHashCode(element);
    }

    /*
     * element is an array
     */
    Int32 length;
    AutoPtr<IArrayOf> array = IArrayOf::Probe(element);
    array->GetLength(&length);

    Int32 hashCode = 1, elementHashCode;
    for (Int32 i = 0; i < length; ++i) {
        AutoPtr<IInterface> object;
        array->Get(i, (IInterface**)&object);
        elementHashCode = DeepGetHashCode(object);
        hashCode = 31 * hashCode + elementHashCode;
    }
    return hashCode;
}

Boolean Arrays::Equals(
    /* [in] */ ArrayOf<Float>* array1,
    /* [in] */ ArrayOf<Float>* array2)
{
    if (array1 == array2) {
        return TRUE;
    }

    if (array1 == NULL || array2 == NULL || array1->GetLength() != array2->GetLength()) {
        return FALSE;
    }
    for (Int32 i = 0; i < array1->GetLength(); i++) {
        if (!Elastos::Core::Math::Equals((*array1)[i], (*array2)[i])) {
            return FALSE;
        }
    }
    return TRUE;
}

Boolean Arrays::Equals(
    /* [in] */ ArrayOf<Double>* array1,
    /* [in] */ ArrayOf<Double>* array2)
{
    if (array1 == array2) {
        return TRUE;
    }

    if (array1 == NULL || array2 == NULL || array1->GetLength() != array2->GetLength()) {
        return FALSE;
    }

    for (Int32 i = 0; i < array1->GetLength(); i++) {
        if (!Elastos::Core::Math::Equals((*array1)[i], (*array2)[i])) {
            return FALSE;
        }
    }

    return TRUE;
}

Boolean Arrays::Equals(
    /* [in] */ ArrayOf<IInterface *> * array1,
    /* [in] */ ArrayOf<IInterface *> * array2)
{
    if (array1 == array2) {
        return TRUE;
    }
    if (array1 == NULL || array2 == NULL || array1->GetLength() != array2->GetLength()) {
        return FALSE;
    }

    IInterface *e1, *e2;
    for (Int32 i = 0; i < array1->GetLength(); i++) {
        e1 = (*array1)[i];
        e2 = (*array2)[i];
        if (!(e1 == NULL ? e2 == NULL : Object::Equals(e1, e2))) {
            return FALSE;
        }
    }
    return TRUE;
}

Boolean Arrays::Equals(
    /* [in] */ AutoPtr<ArrayOf<IInterface *> >& array1,
    /* [in] */ ArrayOf<IInterface *> * array2)
{
    return Equals(array1.Get(), array2);
}

Boolean Arrays::Equals(
    /* [in] */ ArrayOf<IInterface *> * array1,
    /* [in] */ AutoPtr<ArrayOf<IInterface *> >& array2)
{
    return Equals(array1, array2.Get());
}

Boolean Arrays::Equals(
    /* [in] */ AutoPtr<ArrayOf<IInterface *> >& array1,
    /* [in] */ AutoPtr<ArrayOf<IInterface *> >& array2)
{
    return Equals(array1.Get(), array2.Get());
}

Boolean Arrays::DeepEquals(
    /* [in] */ ArrayOf<IInterface *> * array1,
    /* [in] */ ArrayOf<IInterface *> * array2)
{
    if (array1 == array2) {
        return TRUE;
    }
    if (array1 == NULL || array2 == NULL || array1->GetLength() != array2->GetLength()) {
        return FALSE;
    }

    for (Int32 i = 0; i < array1->GetLength(); i++) {
        if (!DeepEquals((*array1)[i], (*array2)[i])) {
            return FALSE;
        }
    }
    return TRUE;
}

Boolean Arrays::DeepEquals(
    /* [in] */ IInterface* e1,
    /* [in] */ IInterface* e2)
{
    if (e1 == NULL && e2 == NULL) {
        return TRUE;
    }

    if (e1 == NULL || e2 == NULL) {
        return FALSE;
    }

    if (e1->Probe(EIID_IInterface) == e2->Probe(EIID_IInterface)) {
        return TRUE;
    }

    if (IArrayOf::Probe(e1) == NULL || IArrayOf::Probe(e2) == NULL) {
        return Object::Equals(e1, e2);
    }

    /*
     * compare as arrays
     */
    AutoPtr<IArrayOf> a1 = IArrayOf::Probe(e1);
    AutoPtr<IArrayOf> a2 = IArrayOf::Probe(e2);

    Int32 len1, len2;
    a1->GetLength(&len1);
    a2->GetLength(&len2);
    if (len1 != len2) {
        return FALSE;
    }

    for (Int32 i = 0; i < len1; ++i) {
        AutoPtr<IInterface> o1, o2;
        a1->Get(i, (IInterface**)&o1);
        a2->Get(i, (IInterface**)&o2);

        if (DeepEquals(o1, o2) == FALSE) {
            return FALSE;
        }
    }

    return TRUE;
}

ECode Arrays::Sort(
    /* [in] */ ArrayOf<IInterface *> * array)
{
    return ComparableTimSort::Sort(array);
}

ECode Arrays::Sort(
    /* [in] */ AutoPtr<ArrayOf<IInterface *> >& array)
{
    return ComparableTimSort::Sort(array.Get());
}

ECode Arrays::Sort(
    /* [in] */ ArrayOf<IInterface *> * array,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return ComparableTimSort::Sort(array, start, end);
}

ECode Arrays::Sort(
    /* [in] */ AutoPtr<ArrayOf<IInterface *> >& array,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return ComparableTimSort::Sort(array.Get(), start, end);
}

ECode Arrays::Sort(
    /* [in] */ ArrayOf<IInterface *> * array,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ IComparator* comparator)
{
    return TimSort::Sort(array, start, end, comparator);
}

ECode Arrays::Sort(
    /* [in] */ AutoPtr<ArrayOf<IInterface *> >& array,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ IComparator* comparator)
{
    return TimSort::Sort(array.Get(), start, end, comparator);
}

ECode Arrays::Sort(
    /* [in] */ ArrayOf<IInterface *> * array,
    /* [in] */ IComparator* comparator)
{
    return TimSort::Sort(array, comparator);
}

ECode Arrays::Sort(
    /* [in] */ AutoPtr<ArrayOf<IInterface *> >& array,
    /* [in] */ IComparator* comparator)
{
    return TimSort::Sort(array.Get(), comparator);
}

String Arrays::ToString(
    /* [in] */ ArrayOf<Char32>* array)
{
    if (array == NULL) {
        return String("NULL");
    }
    if (array->GetLength() == 0) {
        return String("[]");
    }
    StringBuilder sb(array->GetLength() * 3);
    sb.AppendChar('[');
    sb.AppendChar((*array)[0]);
    for (Int32 i = 1; i < array->GetLength(); i++) {
        sb.Append(", ");
        sb.AppendChar((*array)[i]);
    }
    sb.AppendChar(']');
    return sb.ToString();
}

String Arrays::DeepToString(
    /* [in] */ ArrayOf<IInterface *> * array)
{
    // Special case null to prevent NPE
    if (array == NULL) {
        return String("NULL");
    }

    // delegate this to the recursive method
    StringBuilder sb(array->GetLength() * 9);
    sb.AppendChar('[');
    for (Int32 i = 0; i < array->GetLength(); ++i) {
        DeepToString((*array)[i], array, sb);
    }
    sb.AppendChar(']');
    return sb.ToString();
}

ECode Arrays::DeepToString(
    /* [in] */ IInterface * obj,
    /* [in] */ ArrayOf<IInterface*> * origArray,
    /* [in] */ StringBuilder& sb)
{
    IArrayOf* array = IArrayOf::Probe(obj);
    if (array == NULL) {
        String info = Object::ToString(obj);
        sb.Append(info);
        return NOERROR;
    }

    if (DeepToStringContains(origArray, obj)) {
        sb.Append("[...]");
    }
    else {
        sb.AppendChar('[');
        Int32 length;
        array->GetLength(&length);
        for (Int32 i = 0; i < length; ++i) {
            AutoPtr<IInterface> element;
            array->Get(i, (IInterface**)&element);
            DeepToString(element, origArray, sb);
        }
        sb.AppendChar(']');
    }

    return NOERROR;
}

ECode Arrays::DeepToString(
    /* [in] */ IInterface * obj,
    /* [in] */ IArrayOf * origArray,
    /* [in] */ StringBuilder& sb)
{
    IArrayOf* array = IArrayOf::Probe(obj);
    if (array == NULL) {
        String info = Object::ToString(obj);
        sb.Append(info);
        return NOERROR;
    }

    if (DeepToStringContains(origArray, obj)) {
        sb.Append("[...]");
    }
    else {
        sb.AppendChar('[');
        Int32 length;
        array->GetLength(&length);
        for (Int32 i = 0; i < length; ++i) {
            AutoPtr<IInterface> element;
            array->Get(i, (IInterface**)&element);
            DeepToString(element, origArray, sb);
        }
        sb.AppendChar(']');
    }

    return NOERROR;
}

Boolean Arrays::DeepToStringContains(
    /* [in] */ ArrayOf<IInterface *> * origArray,
    /* [in] */ IInterface* array)
{
    if (origArray == NULL || origArray->GetLength() == 0) {
        return FALSE;
    }

    for (Int32 i = 0; i < origArray->GetLength(); ++i) {
        if ((*origArray)[i] == array) {
            return TRUE;
        }
    }
    return FALSE;
}

Boolean Arrays::DeepToStringContains(
    /* [in] */ IArrayOf * origArray,
    /* [in] */ IInterface * obj)
{
    if (origArray == NULL) {
        return FALSE;
    }

    Int32 length;
    origArray->GetLength(&length);
    for (Int32 i = 0; i < length; ++i) {
        AutoPtr<IInterface> element;
        origArray->Get(i, (IInterface**)&element);
        if (element.Get() == obj) {
            return TRUE;
        }
    }

    return FALSE;
}


} // namespace Utility
} // namespace Elastos
