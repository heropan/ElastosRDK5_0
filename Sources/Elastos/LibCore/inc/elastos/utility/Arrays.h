#ifndef __ELASTOS_UTILITY_ARRAYS_H__
#define __ELASTOS_UTILITY_ARRAYS_H__

//#include "AbstractList.h"
#include <elastos/core/Object.h>
#include <elastos/core/StringBuilder.h>
#include <elastos/core/Math.h>
#include <elastos/utility/DualPivotQuicksort.h>

using Elastos::Core::IComparator;
using Elastos::Core::IComparable;
using Elastos::Core::StringBuilder;
// using Elastos::IO::ISerializable;

namespace Elastos {
namespace Utility {

class Arrays
{
private:
    // class ArrayList
    //     : public AbstractList
    //     , public ISerializable
    //     , public IRandomAccess
    // {
    // public:
    //     CAR_INTERFACE_DECL()

    //     ArrayList(
    //         /* [in] */ ArrayOf<IInterface *> * storage);

    //     CARAPI Contains(
    //         /* [in] */ IInterface * object,
    //         /* [out] */ Boolean* result);

    //     CARAPI Get(
    //         /* [in] */ Int32 location,
    //         /* [out] */ IInterface ** result);

    //     CARAPI IndexOf(
    //         /* [in] */ IInterface * object,
    //         /* [out] */ Int32* result);

    //     CARAPI LastIndexOf(
    //         /* [in] */ IInterface * object,
    //         /* [out] */ Int32* index);

    //     CARAPI Set(
    //         /* [in] */ Int32 location,
    //         /* [in] */ IInterface * object,
    //         /* [out] */ IInterface ** result);

    //     CARAPI GetSize(
    //         /* [out] */ Int32* size);

    //     CARAPI ToArray(
    //         /* [out] */ ArrayOf<IInterface *> ** outArray);

    //     CARAPI ToArray(
    //         /* [in] */ ArrayOf<IInterface *> * contents,
    //         /* [out] */ ArrayOf<IInterface *> ** outArray);

    // private:
    //     AutoPtr<ArrayOf<IInterface *> > mA;
    // };

public:
    // static CARAPI AsList(
    //     /* [in] */ ArrayOf<IInterface *> * array,
    //     /* [out] */ IList** outList);

    /**
     * Performs a binary search for {@code value} in the ascending sorted array {@code array},
     * in the range specified by fromIndex (inclusive) and toIndex (exclusive).
     * Searching in an unsorted array has an undefined result. It's also undefined which element
     * is found if there are multiple occurrences of the same element.
     *
     * @param array the sorted array to search.
     * @param startIndex the inclusive start index.
     * @param endIndex the exclusive start index.
     * @param value the element to find.
     * @return the non-negative index of the element, or a negative index which
     *         is {@code -index - 1} where the element would be inserted.
     * @throws IllegalArgumentException if {@code startIndex > endIndex}
     * @throws ArrayIndexOutOfBoundsException if {@code startIndex < 0 || endIndex > array.length}
     * @since 1.6
     */
    template<typename T>
    static CARAPI BinarySearch(
        /* [in] */ ArrayOf<T>* array,
        /* [in] */ Int32 startIndex,
        /* [in] */ Int32 endIndex,
        /* [in] */ T value,
        /* [out] */ Int32* index);

    /**
     * Performs a binary search for {@code value} in the ascending sorted array {@code array}.
     * Searching in an unsorted array has an undefined result. It's also undefined which element
     * is found if there are multiple occurrences of the same element.
     *
     * @param array the sorted array to search.
     * @param value the element to find.
     * @return the non-negative index of the element, or a negative index which
     *         is {@code -index - 1} where the element would be inserted.
     */
    template<typename T>
    static CARAPI BinarySearch(
        /* [in] */ ArrayOf<T>* array,
        /* [in] */ T value,
        /* [out] */ Int32* index);

    /**
     * Performs a binary search for {@code value} in the ascending sorted array {@code array},
     * in the range specified by fromIndex (inclusive) and toIndex (exclusive).
     * Searching in an unsorted array has an undefined result. It's also undefined which element
     * is found if there are multiple occurrences of the same element.
     *
     * @param array the sorted array to search.
     * @param startIndex the inclusive start index.
     * @param endIndex the exclusive start index.
     * @param value the element to find.
     * @return the non-negative index of the element, or a negative index which
     *         is {@code -index - 1} where the element would be inserted.
     * @throws ClassCastException
     *         if an element in the array or the search element does not
     *         implement {@code Comparable}, or cannot be compared to each other.
     * @throws IllegalArgumentException if {@code startIndex > endIndex}
     * @throws ArrayIndexOutOfBoundsException if {@code startIndex < 0 || endIndex > array.length}
     * @since 1.6
     */
    static CARAPI BinarySearch(
        /* [in] */ ArrayOf<IInterface *> * array,
        /* [in] */ Int32 startIndex,
        /* [in] */ Int32 endIndex,
        /* [in] */ IInterface * value,
        /* [out] */ Int32* index);

    /**
     * Performs a binary search for {@code value} in the ascending sorted array {@code array}.
     * Searching in an unsorted array has an undefined result. It's also undefined which element
     * is found if there are multiple occurrences of the same element.
     *
     * @param array the sorted array to search.
     * @param value the element to find.
     * @return the non-negative index of the element, or a negative index which
     *         is {@code -index - 1} where the element would be inserted.
     * @throws ClassCastException
     *         if an element in the array or the search element does not
     *         implement {@code Comparable}, or cannot be compared to each other.
     */
    static CARAPI BinarySearch(
        /* [in] */ ArrayOf<IInterface *> * array,
        /* [in] */ IInterface * value,
        /* [out] */ Int32* index);

    /**
     * Performs a binary search for {@code value} in the ascending sorted array {@code array},
     * in the range specified by fromIndex (inclusive) and toIndex (exclusive),
     * using {@code comparator} to compare elements.
     * Searching in an unsorted array has an undefined result. It's also undefined which element
     * is found if there are multiple occurrences of the same element.
     *
     * @param array the sorted array to search.
     * @param startIndex the inclusive start index.
     * @param endIndex the exclusive start index.
     * @param value the element to find.
     * @param comparator the {@code Comparator} used to compare the elements.
     * @return the non-negative index of the element, or a negative index which
     *         is {@code -index - 1} where the element would be inserted.
     * @throws ClassCastException
     *         if an element in the array or the search element does not
     *         implement {@code Comparable}, or cannot be compared to each other.
     * @throws IllegalArgumentException if {@code startIndex > endIndex}
     * @throws ArrayIndexOutOfBoundsException if {@code startIndex < 0 || endIndex > array.length}
     * @since 1.6
     */
    static CARAPI BinarySearch(
        /* [in] */ ArrayOf<IInterface *> * array,
        /* [in] */ Int32 startIndex,
        /* [in] */ Int32 endIndex,
        /* [in] */ IInterface * value,
        /* [in] */ IComparator* comparator,
        /* [out] */ Int32* index);

    /**
     * Performs a binary search for {@code value} in the ascending sorted array {@code array},
     * using {@code comparator} to compare elements.
     * Searching in an unsorted array has an undefined result. It's also undefined which element
     * is found if there are multiple occurrences of the same element.
     *
     * @param array the sorted array to search.
     * @param value the element to find.
     * @param comparator the {@code Comparator} used to compare the elements.
     * @return the non-negative index of the element, or a negative index which
     *         is {@code -index - 1} where the element would be inserted.
     * @throws ClassCastException
     *         if an element in the array or the search element does not
     *         implement {@code Comparable}, or cannot be compared to each other.
     */
    static CARAPI BinarySearch(
        /* [in] */ ArrayOf<IInterface *> * array,
        /* [in] */ IInterface * value,
        /* [in] */ IComparator* comparator,
        /* [out] */ Int32* index);

    /**
     * Fills the specified array with the specified element.
     *
     * @param array
     *            the {@code byte} array to fill.
     * @param value
     *            the {@code byte} element.
     */
    template<typename T>
    static CARAPI Fill(
        /* [in] */ ArrayOf<T>* array,
        /* [in] */ T value);

    /**
     * Fills the specified range in the array with the specified element.
     *
     * @param array
     *            the {@code byte} array to fill.
     * @param start
     *            the first index to fill.
     * @param end
     *            the last + 1 index to fill.
     * @param value
     *            the {@code byte} element.
     * @throws IllegalArgumentException
     *                if {@code start > end}.
     * @throws ArrayIndexOutOfBoundsException
     *                if {@code start < 0} or {@code end > array.length}.
     */
    template<typename T>
    static CARAPI Fill(
        /* [in] */ ArrayOf<T>* array,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ T value);

    /**
     * Returns a hash code based on the contents of the given array. For any two
     * {@code boolean} arrays {@code a} and {@code b}, if
     * {@code Arrays.equals(a, b)} returns {@code true}, it means
     * that the return value of {@code Arrays.hashCode(a)} equals {@code Arrays.hashCode(b)}.
     * <p>
     * The value returned by this method is the same value as the
     * {@link List#hashCode()} method which is invoked on a {@link List}
     * containing a sequence of {@link Boolean} instances representing the
     * elements of array in the same order. If the array is {@code null}, the return
     * value is 0.
     *
     * @param array
     *            the array whose hash code to compute.
     * @return the hash code for {@code array}.
     */
    template<typename T>
    static CARAPI GetHashCode(
        /* [in] */ ArrayOf<T>* array,
        /* [out] */ Int32* code);

    static CARAPI GetHashCode(
        /* [in] */ ArrayOf<Boolean>* array,
        /* [out] */ Int32* code);

    static CARAPI GetHashCode(
        /* [in] */ ArrayOf<Int64>* array,
        /* [out] */ Int32* code);

    static CARAPI GetHashCode(
        /* [in] */ ArrayOf<Float>* array,
        /* [out] */ Int32* code);

    static CARAPI GetHashCode(
        /* [in] */ ArrayOf<Double>* array,
        /* [out] */ Int32* code);

    static CARAPI GetHashCode(
        /* [in] */ ArrayOf<IInterface *> * array,
        /* [out] */ Int32* code);

    // static CARAPI DeepHashCode(
    //     /* [in] */ ArrayOf<IInterface *> * array,
    //     /* [out] */ Int32* code);

    /**
     * Compares the two arrays.
     *
     * @param array1
     *            the first {@code byte} array.
     * @param array2
     *            the second {@code byte} array.
     * @return {@code true} if both arrays are {@code null} or if the arrays have the
     *         same length and the elements at each index in the two arrays are
     *         equal, {@code false} otherwise.
     */
    template<typename T>
    static CARAPI Equals(
        /* [in] */ ArrayOf<T>* array1,
        /* [in] */ ArrayOf<T>* array2,
        /* [out] */ Boolean* result);

    static CARAPI Equals(
        /* [in] */ ArrayOf<Float>* array1,
        /* [in] */ ArrayOf<Float>* array2,
        /* [out] */ Boolean* result);

    static CARAPI Equals(
        /* [in] */ ArrayOf<Double>* array1,
        /* [in] */ ArrayOf<Double>* array2,
        /* [out] */ Boolean* result);

    static CARAPI Equals(
        /* [in] */ ArrayOf<IInterface *> * array1,
        /* [in] */ ArrayOf<IInterface *> * array2,
        /* [out] */ Boolean* result);

    // static CARAPI DeepEquals(
    //     /* [in] */ ArrayOf<IInterface *> * array1,
    //     /* [in] */ ArrayOf<IInterface *> * array2,
    //     /* [out] */ Boolean* result);

    template<typename T>
    static CARAPI Sort(
        /* [in] */ ArrayOf<T>* array);

    template<typename T>
    static CARAPI Sort(
        /* [in] */ ArrayOf<T>* array,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    static CARAPI Sort(
        /* [in] */ ArrayOf<IInterface *> * array);

    static CARAPI Sort(
        /* [in] */ ArrayOf<IInterface *> * array,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    static CARAPI Sort(
        /* [in] */ ArrayOf<IInterface *> * array,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ IComparator* comparator);

    static CARAPI Sort(
        /* [in] */ ArrayOf<IInterface *> * array,
        /* [in] */ IComparator* comparator);

    template<typename T>
    static CARAPI ToString(
        /* [in] */ ArrayOf<T> * array,
        /* [out] */ String* str);

    static CARAPI ToString(
        /* [in] */ ArrayOf<Char32> * array,
        /* [out] */ String* str);

    // static CARAPI DeepToString(
    //     /* [in] */ ArrayOf<IInterface *> * array,
    //     /* [out] */ String* str);

    template<typename T>
    static CARAPI CopyOf(
        /* [in] */ ArrayOf<T> * original,
        /* [in] */ Int32 newLength,
        /* [out, callee] */ ArrayOf<T> ** result);

    template<typename T>
    static CARAPI CopyOfRange(
        /* [in] */ ArrayOf<T> * original,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out, callee] */ ArrayOf<T> ** result);

    template<typename T, typename U>
    static CARAPI CopyOf(
        /* [in] */ ArrayOf<T> * original,
        /* [in] */ Int32 newLength,
        /* [out, callee] */ ArrayOf<U> ** result);

    template<typename T, typename U>
    static CARAPI CopyOfRange(
        /* [in] */ ArrayOf<T> * original,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out, callee] */ ArrayOf<U> ** result);

    // static CARAPI CopyOfByte(
    //     /* [in] */ ArrayOf<Byte>* original,
    //     /* [in] */ Int32 newLength,
    //     /* [out, callee] */ ArrayOf<Byte>** result);

    // static CARAPI CopyOfChar32(
    //     /* [in] */ ArrayOf<Char32>* original,
    //     /* [in] */ Int32 newLength,
    //     /* [out, callee] */ ArrayOf<Char32>** result);

    // static CARAPI CopyOfDouble(
    //     /* [in] */ ArrayOf<Double>* original,
    //     /* [in] */ Int32 newLength,
    //     /* [out, callee] */ ArrayOf<Double>** result);

    // static CARAPI CopyOfFloat(
    //     /* [in] */ ArrayOf<Float>* original,
    //     /* [in] */ Int32 newLength,
    //     /* [out, callee] */ ArrayOf<Float>** result);

    // static CARAPI CopyOfInt32(
    //     /* [in] */ ArrayOf<Int32>* original,
    //     /* [in] */ Int32 newLength,
    //     /* [out, callee] */ ArrayOf<Int32>** result);

    // static CARAPI CopyOfInt64(
    //     /* [in] */ ArrayOf<Int64>* original,
    //     /* [in] */ Int32 newLength,
    //     /* [out, callee] */ ArrayOf<Int64>** result);

    // static CARAPI CopyOfInt16(
    //     /* [in] */ ArrayOf<Int16>* original,
    //     /* [in] */ Int32 newLength,
    //     /* [out, callee] */ ArrayOf<Int16>** result);

    // static CARAPI CopyOf(
    //     /* [in] */ ArrayOf<IInterface *> * original,
    //     /* [in] */ int newLength,
    //     /* [out, callee] */ ArrayOf<IInterface *> ** result);

    //  static CARAPI _CopyOf(
    //     /* [in] */ U[] original,
    //     /* [in] */ int newLength,
    //     /* [in] */ Class<? extends T[]> newType,
    //     /* [out, callee] */ <T, U> T[] result);

    // static CARAPI CopyOfRangeBoolean(
    //     /* [in] */ ArrayOf<Boolean>* original,
    //     /* [in] */ Int32 start,
    //     /* [in] */ Int32 end,
    //     /* [out, callee] */ ArrayOf<Boolean>** arrayCopy);

    // static CARAPI CopyOfRangeByte(
    //     /* [in] */ ArrayOf<Byte>* original,
    //     /* [in] */ Int32 start,
    //     /* [in] */ Int32 end,
    //     /* [out, callee] */ ArrayOf<Byte>** arrayCopy);

    // static CARAPI CopyOfRangeChar32(
    //     /* [in] */ ArrayOf<Char32>* original,
    //     /* [in] */ Int32 start,
    //     /* [in] */ Int32 end,
    //     /* [out, callee] */ ArrayOf<Char32>** arrayCopy);

    // static CARAPI CopyOfRangeDouble(
    //     /* [in] */ ArrayOf<Double>* original,
    //     /* [in] */ Int32 start,
    //     /* [in] */ Int32 end,
    //     /* [out, callee] */ ArrayOf<Double>** arrayCopy);

    // static CARAPI CopyOfRangeFloat(
    //     /* [in] */ ArrayOf<Float>* original,
    //     /* [in] */ Int32 start,
    //     /* [in] */ Int32 end,
    //     /* [out, callee] */ ArrayOf<Float>** arrayCopy);

    // static CARAPI CopyOfRangeInt32(
    //     /* [in] */ ArrayOf<Int32>* original,
    //     /* [in] */ Int32 start,
    //     /* [in] */ Int32 end,
    //     /* [out, callee] */ ArrayOf<Int32>** arrayCopy);

    // static CARAPI CopyOfRangeInt64(
    //     /* [in] */ ArrayOf<Int64>* original,
    //     /* [in] */ Int32 start,
    //     /* [in] */ Int32 end,
    //     /* [out, callee] */ ArrayOf<Int64>** arrayCopy);

    // static CARAPI CopyOfRangeInt16(
    //     /* [in] */ ArrayOf<Int16>* original,
    //     /* [in] */ Int32 start,
    //     /* [in] */ Int32 end,
    //     /* [out, callee] */ ArrayOf<Int16>** arrayCopy);

    // static CARAPI CopyOfRange(
    //     /* [in] */ ArrayOf<IInterface *> * original,
    //     /* [in] */ Int32 start,
    //     /* [in] */ Int32 end,
    //     /* [out, callee] */ ArrayOf<IInterface *> ** arrayCopy);

    //  static CARAPI _CopyOfRange(
    //     /* [in] */ U[] original,
    //     /* [in] */ Int32 start,
    //     /* [in] */ Int32 end,
    //     /* [in] */ Class<? extends T[]> newType,
    //     /* [out, callee] */ <T, U> T[] arrayCopy);

public:
    static CARAPI CheckStartAndEnd(
        /* [in] */ Int32 len,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    static CARAPI CheckOffsetAndCount(
        /* [in] */ Int32 arrayLength,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count);

private:
    static CARAPI CheckBinarySearchBounds(
        /* [in] */ Int32 startIndex,
        /* [in] */ Int32 endIndex,
        /* [in] */ Int32 length);

    // static CARAPI_(Int32) DeepHashCodeElement(
    //     /* [in] */ IInterface * element);

    // static CARAPI_(Boolean) DeepEqualsElements(
    //     /* [in] */ IInterface * e1,
    //     /* [in] */ IInterface * e2);

    // static CARAPI_(void) DeepToStringImpl(
    //     /* [in] */ ArrayOf<IInterface *> * array,
    //     /* [in] */ ArrayOf<IInterface *> * origArrays,
    //     /* [in] */ StringBuilder* sb);

    // static CARAPI_(Boolean) DeepToStringImplContains(
    //     /* [in] */ ArrayOf<IInterface *> * origArrays,
    //     /* [in] */ IInterface * array);

private:
    Arrays();
    Arrays(const Arrays&);
};

//=========================================================================

template<typename T>
ECode Arrays::BinarySearch(
    /* [in] */ ArrayOf<T>* array,
    /* [in] */ Int32 startIndex,
    /* [in] */ Int32 endIndex,
    /* [in] */ T value,
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
    Int32 mid;

    T midVal;
    while (lo <= hi) {
        mid = (UInt32(lo + hi)) >> 1;
        midVal = (*array)[mid];

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

template<typename T>
ECode Arrays::BinarySearch(
    /* [in] */ ArrayOf<T>* array,
    /* [in] */ T value,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index)
    *index = -1;
    assert(array != NULL);
    VALIDATE_NOT_NULL(array)

    return BinarySearch(array, 0, array->GetLength(), value, index);
}

template<typename T>
ECode Arrays::Fill(
    /* [in] */ ArrayOf<T>* array,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ T value)
{
    assert(array != NULL);
    VALIDATE_NOT_NULL(array);

    FAIL_RETURN(CheckStartAndEnd(array->GetLength(), start, end));
    for (Int32 i = start; i < end; i++) {
        array->Set(i, value);
    }
    return NOERROR;
}


template<typename T>
ECode Arrays::Fill(
    /* [in] */ ArrayOf<T>* array,
    /* [in] */ T value)
{
    assert(array != NULL);
    VALIDATE_NOT_NULL(array);

    for (Int32 i = 0; i < array->GetLength(); i++) {
        array->Set(i, value);
    }
    return NOERROR;
}

template<typename T>
ECode Arrays::GetHashCode(
    /* [in] */ ArrayOf<T>* array,
    /* [out] */ Int32* hash)
{
    VALIDATE_NOT_NULL(hash)
    *hash = 0;

    if (array == NULL) {
        return NOERROR;
    }

    Int32 hashCode = 1;
    for (Int32 i = 0; i < array->GetLength(); i++) {
        hashCode = 31 * hashCode + (Int32)(*array)[i];
    }
    *hash = hashCode;
    return NOERROR;
}

template<typename T>
ECode Arrays::Equals(
    /* [in] */ ArrayOf<T>* array1,
    /* [in] */ ArrayOf<T>* array2,
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

template<typename T>
ECode Arrays::Sort(
    /* [in] */ ArrayOf<T> * array)
{
    return DualPivotQuicksort::Sort(array);
}

template<typename T>
ECode Arrays::Sort(
    /* [in] */ ArrayOf<T> * array,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    return DualPivotQuicksort::Sort(array, start, end);
}

template<typename T>
ECode Arrays::ToString(
    /* [in] */ ArrayOf<T>* array,
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
    sb.Append((*array)[0]);
    for (Int32 i = 1; i < array->GetLength(); i++) {
        sb.Append(", ");
        sb.Append((*array)[i]);
    }
    sb.AppendChar(']');
    *str = sb.ToString();
    return NOERROR;
}

template<typename T>
ECode Arrays::CopyOfRange(
    /* [in] */ ArrayOf<T> * original,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out, callee] */ ArrayOf<T> ** result)
{
    VALIDATE_NOT_NULL(result)
    *result = NULL;
    assert(original);
    VALIDATE_NOT_NULL(original)

    if (start > end) {
        ALOGE("Arrays::CopyOfRange: error, E_ILLEGAL_ARGUMENT_EXCEPTION, start %d, end %d", start, end);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Int32 originalLength = original->GetLength();
    if (start < 0 || start > originalLength) {
        ALOGE("Arrays::CopyOfRange: error, E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION, start %d, originalLength %d",
            start, originalLength);
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    Int32 resultLength = end - start;
    Int32 copyLength = Elastos::Core::Math::Min(resultLength, originalLength - start);
    AutoPtr<ArrayOf<T> > array = ArrayOf<T>::Alloc(resultLength);
    array->Copy(original, start, copyLength);
    *result = result;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

template<typename T>
ECode Arrays::CopyOf(
    /* [in] */ ArrayOf<T> * original,
    /* [in] */ Int32 newLength,
    /* [out, callee] */ ArrayOf<T> ** result)
{
    if (newLength < 0) {
        ALOGE("Arrays::CopyOf: error, E_NAGATIVE_ARRAY_SIZE_EXCEPTION, newLength %d", newLength);
        return E_NAGATIVE_ARRAY_SIZE_EXCEPTION;
    }

    return CopyOfRange(original, 0, newLength, result);
}

template<typename T, typename U>
ECode Arrays::CopyOfRange(
    /* [in] */ ArrayOf<T> * original,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out, callee] */ ArrayOf<U> ** result)
{
    VALIDATE_NOT_NULL(result)
    *result = NULL;
    assert(original);
    VALIDATE_NOT_NULL(original)

    if (start > end) {
        ALOGE("Arrays::CopyOfRange: error, E_ILLEGAL_ARGUMENT_EXCEPTION, start %d, end %d", start, end);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Int32 originalLength = original->GetLength();
    if (start < 0 || start > originalLength) {
        ALOGE("Arrays::CopyOfRange: error, E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION, start %d, originalLength %d",
            start, originalLength);
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    Int32 resultLength = end - start;
    Int32 copyLength = Elastos::Core::Math::Min(resultLength, originalLength - start);
    AutoPtr<ArrayOf<U> > array = ArrayOf<U>::Alloc(resultLength);
    for (Int32 i = 0; i < original->GetLength(); ++i) {
        array->Set(i, (U)(*original)[i]); // TODO: only work with primitives
    }

    *result = result;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

template<typename T, typename U>
ECode Arrays::CopyOf(
    /* [in] */ ArrayOf<T> * original,
    /* [in] */ Int32 newLength,
    /* [out, callee] */ ArrayOf<U> ** result)
{
    if (newLength < 0) {
        ALOGE("Arrays::CopyOf: error, E_NAGATIVE_ARRAY_SIZE_EXCEPTION, newLength %d", newLength);
        return E_NAGATIVE_ARRAY_SIZE_EXCEPTION;
    }

    return CopyOfRange(original, 0, newLength, result);
}

} // namespace Utility
} // namespace Elastos

#endif // __ELASTOS_UTILITY_ARRAYS_H__