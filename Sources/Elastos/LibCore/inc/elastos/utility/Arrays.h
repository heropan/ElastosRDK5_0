#ifndef __ELASTOS_UTILITY_ARRAYS_H__
#define __ELASTOS_UTILITY_ARRAYS_H__

//#include "AbstractList.h"
#include <elastos/core/Object.h>
#include <elastos/core/StringBuilder.h>

using Elastos::Core::IComparator;
using Elastos::Core::StringBuilder;
using Elastos::IO::ISerializable;

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
    //         /* [in] */ ArrayOf<IInterface* >* storage);

    //     CARAPI Contains(
    //         /* [in] */ IInterface* object,
    //         /* [out] */ Boolean* result);

    //     CARAPI Get(
    //         /* [in] */ Int32 location,
    //         /* [out] */ IInterface** result);

    //     CARAPI IndexOf(
    //         /* [in] */ IInterface* object,
    //         /* [out] */ Int32* result);

    //     CARAPI LastIndexOf(
    //         /* [in] */ IInterface* object,
    //         /* [out] */ Int32* index);

    //     CARAPI Set(
    //         /* [in] */ Int32 location,
    //         /* [in] */ IInterface* object,
    //         /* [out] */ IInterface** result);

    //     CARAPI GetSize(
    //         /* [out] */ Int32* size);

    //     CARAPI ToArray(
    //         /* [out] */ ArrayOf<IInterface* >** outArray);

    //     CARAPI ToArray(
    //         /* [in] */ ArrayOf<IInterface* >* contents,
    //         /* [out] */ ArrayOf<IInterface* >** outArray);

    // private:
    //     AutoPtr<ArrayOf<IInterface*> > mA;
    // };

public:
    // static CARAPI AsList(
    //     /* [in] */ ArrayOf<IInterface*>* array,
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
        /* [in] */ Byte value,
        /* [out] */ Int32* index);

    template<typename T>
    static CARAPI BinarySearch(
        /* [in] */ ArrayOf<T>* array,
        /* [in] */ Byte value,
        /* [out] */ Int32* index);

    // static CARAPI BinarySearchObject(
    //     /* [in] */ ArrayOf<IInterface* >* array,
    //     /* [in] */ IInterface* value,
    //     /* [out] */ Int32* index);

    // static CARAPI BinarySearchObject(
    //     /* [in] */ ArrayOf<IInterface* >* array,
    //     /* [in] */ Int32 startIndex,
    //     /* [in] */ Int32 endIndex,
    //     /* [in] */ IInterface* value,
    //     /* [out] */ Int32* index);

    // static CARAPI BinarySearch(
    //     /* [in] */ ArrayOf<IInterface* >* array,
    //     /* [in] */ IInterface* value,
    //     /* [in] */ IComparator* comparator,
    //     /* [out] */ Int32* index);

    // static CARAPI BinarySearch(
    //     /* [in] */ ArrayOf<IInterface* >* array,
    //     /* [in] */ Int32 startIndex,
    //     /* [in] */ Int32 endIndex,
    //     /* [in] */ IInterface* value,
    //     /* [in] */ IComparator* comparator,
    //     /* [out] */ Int32* index);

    // static CARAPI FillByte(
    //     /* [in] */ ArrayOf<Byte>* array,
    //     /* [in] */ Byte value);

    // static CARAPI FillByte(
    //     /* [in] */ ArrayOf<Byte>* array,
    //     /* [in] */ Int32 start,
    //     /* [in] */ Int32 end,
    //     /* [in] */ Byte value);

    // static CARAPI FillInt16(
    //     /* [in] */ ArrayOf<Int16>* array,
    //     /* [in] */ Int16 value);

    // static CARAPI FillInt16(
    //     /* [in] */ ArrayOf<Int16>* array,
    //     /* [in] */ Int32 start,
    //     /* [in] */ Int32 end,
    //     /* [in] */ Int16 value);

    // static CARAPI FillChar32(
    //     /* [in] */ ArrayOf<Char32>* array,
    //     /* [in] */ Char32 value);

    // static CARAPI FillChar32(
    //     /* [in] */ ArrayOf<Char32>* array,
    //     /* [in] */ Int32 start,
    //     /* [in] */ Int32 end,
    //     /* [in] */ Char32 value);

    // static CARAPI FillInt32(
    //     /* [in] */ ArrayOf<Int32>* array,
    //     /* [in] */ Int32 value);

    // static CARAPI FillInt32(
    //     /* [in] */ ArrayOf<Int32>* array,
    //     /* [in] */ Int32 start,
    //     /* [in] */ Int32 end,
    //     /* [in] */ Int32 value);

    // static CARAPI FillInt64(
    //     /* [in] */ ArrayOf<Int64>* array,
    //     /* [in] */ Int64 value);

    // static CARAPI FillInt64(
    //     /* [in] */ ArrayOf<Int64>* array,
    //     /* [in] */ Int32 start,
    //     /* [in] */ Int32 end,
    //     /* [in] */ Int64 value);

    // static CARAPI FillFloat(
    //     /* [in] */ ArrayOf<Float>* array,
    //     /* [in] */ Float value);

    // static CARAPI FillFloat(
    //     /* [in] */ ArrayOf<Float>* array,
    //     /* [in] */ Int32 start,
    //     /* [in] */ Int32 end,
    //     /* [in] */ Float value);

    // static CARAPI FillDouble(
    //     /* [in] */ ArrayOf<Double>* array,
    //     /* [in] */ Double value);

    // static CARAPI FillDouble(
    //     /* [in] */ ArrayOf<Double>* array,
    //     /* [in] */ Int32 start,
    //     /* [in] */ Int32 end,
    //     /* [in] */ Double value);

    // static CARAPI FillBoolean(
    //     /* [in] */ ArrayOf<Boolean>* array,
    //     /* [in] */ Boolean value);

    // static CARAPI FillBoolean(
    //     /* [in] */ ArrayOf<Boolean>* array,
    //     /* [in] */ Int32 start,
    //     /* [in] */ Int32 end,
    //     /* [in] */ Boolean value);

    // static CARAPI FillObject(
    //     /* [in] */ ArrayOf<IInterface* >* array,
    //     /* [in] */ IInterface* value);

    // static CARAPI FillObject(
    //     /* [in] */ ArrayOf<IInterface* >* array,
    //     /* [in] */ Int32 start,
    //     /* [in] */ Int32 end,
    //     /* [in] */ IInterface* value);

    // static CARAPI HashCodeBoolean(
    //     /* [in] */ ArrayOf<Boolean>* array,
    //     /* [out] */ Int32* code);

    // static CARAPI HashCodeInt32(
    //     /* [in] */ ArrayOf<Int32>* array,
    //     /* [out] */ Int32* code);

    // static CARAPI HashCodeInt16(
    //     /* [in] */ ArrayOf<Int16>* array,
    //     /* [out] */ Int32* code);

    // static CARAPI HashCodeChar32(
    //     /* [in] */ ArrayOf<Char32>* array,
    //     /* [out] */ Int32* code);

    // static CARAPI HashCodeByte(
    //     /* [in] */ ArrayOf<Byte>* array,
    //     /* [out] */ Int32* code);

    // static CARAPI HashCodeInt64(
    //     /* [in] */ ArrayOf<Int64>* array,
    //     /* [out] */ Int32* code);

    // static CARAPI HashCodeFloat(
    //     /* [in] */ ArrayOf<Float>* array,
    //     /* [out] */ Int32* code);

    // static CARAPI HashCodeDouble(
    //     /* [in] */ ArrayOf<Double>* array,
    //     /* [out] */ Int32* code);

    // static CARAPI HashCodeObject(
    //     /* [in] */ ArrayOf<IInterface* >* array,
    //     /* [out] */ Int32* code);

    // static CARAPI DeepHashCode(
    //     /* [in] */ ArrayOf<IInterface* >* array,
    //     /* [out] */ Int32* code);

    // static CARAPI EqualsByte(
    //     /* [in] */ ArrayOf<Byte>* array1,
    //     /* [in] */ ArrayOf<Byte>* array2,
    //     /* [out] */ Boolean* result);

    // static CARAPI EqualsInt16(
    //     /* [in] */ ArrayOf<Int16>* array1,
    //     /* [in] */ ArrayOf<Int16>* array2,
    //     /* [out] */ Boolean* result);

    // static CARAPI EqualsChar32(
    //     /* [in] */ ArrayOf<Char32>* array1,
    //     /* [in] */ ArrayOf<Char32>* array2,
    //     /* [out] */ Boolean* result);

    // static CARAPI EqualsInt32(
    //     /* [in] */ ArrayOf<Int32>* array1,
    //     /* [in] */ ArrayOf<Int32>* array2,
    //     /* [out] */ Boolean* result);

    // static CARAPI EqualsInt64(
    //     /* [in] */ ArrayOf<Int64>* array1,
    //     /* [in] */ ArrayOf<Int64>* array2,
    //     /* [out] */ Boolean* result);

    // static CARAPI EqualsFloat(
    //     /* [in] */ ArrayOf<Float>* array1,
    //     /* [in] */ ArrayOf<Float>* array2,
    //     /* [out] */ Boolean* result);

    // static CARAPI EqualsDouble(
    //     /* [in] */ ArrayOf<Double>* array1,
    //     /* [in] */ ArrayOf<Double>* array2,
    //     /* [out] */ Boolean* result);

    // static CARAPI EqualsBoolean(
    //     /* [in] */ ArrayOf<Boolean>* array1,
    //     /* [in] */ ArrayOf<Boolean>* array2,
    //     /* [out] */ Boolean* result);

    // static CARAPI EqualsObject(
    //     /* [in] */ ArrayOf<IInterface* >* array1,
    //     /* [in] */ ArrayOf<IInterface* >* array2,
    //     /* [out] */ Boolean* result);

    // static CARAPI DeepEquals(
    //     /* [in] */ ArrayOf<IInterface* >* array1,
    //     /* [in] */ ArrayOf<IInterface* >* array2,
    //     /* [out] */ Boolean* result);

    // static CARAPI SortByte(
    //     /* [in] */ ArrayOf<Byte>* array);

    // static CARAPI SortByte(
    //     /* [in] */ ArrayOf<Byte>* array,
    //     /* [in] */ Int32 start,
    //     /* [in] */ Int32 end);

    // static CARAPI SortChar32(
    //     /* [in] */ ArrayOf<Char32>* array);

    // static CARAPI SortChar32(
    //     /* [in] */ ArrayOf<Char32>* array,
    //     /* [in] */ Int32 start,
    //     /* [in] */ Int32 end);

    // static CARAPI SortDouble(
    //     /* [in] */ ArrayOf<Double>* array);

    // static CARAPI SortDouble(
    //     /* [in] */ ArrayOf<Double>* array,
    //     /* [in] */ Int32 start,
    //     /* [in] */ Int32 end);

    // static CARAPI SortFloat(
    //     /* [in] */ ArrayOf<Float>* array);

    // static CARAPI SortFloat(
    //     /* [in] */ ArrayOf<Float>* array,
    //     /* [in] */ Int32 start,
    //     /* [in] */ Int32 end);

    // static CARAPI SortInt32(
    //     /* [in] */ ArrayOf<Int32>* array);

    // static CARAPI SortInt32(
    //     /* [in] */ ArrayOf<Int32>* array,
    //     /* [in] */ Int32 start,
    //     /* [in] */ Int32 end);

    // static CARAPI SortInt64(
    //     /* [in] */ ArrayOf<Int64>* array);

    // static CARAPI SortInt64(
    //     /* [in] */ ArrayOf<Int64>* array,
    //     /* [in] */ Int32 start,
    //     /* [in] */ Int32 end);

    // static CARAPI SortInt16(
    //     /* [in] */ ArrayOf<Int16>* array);

    // static CARAPI SortInt16(
    //     /* [in] */ ArrayOf<Int16>* array,
    //     /* [in] */ Int32 start,
    //     /* [in] */ Int32 end);

    // static CARAPI SortObject(
    //     /* [in] */ ArrayOf<IInterface* >* array);

    // static CARAPI SortObject(
    //     /* [in] */ ArrayOf<IInterface* >* array,
    //     /* [in] */ Int32 start,
    //     /* [in] */ Int32 end);

    // static CARAPI Sort(
    //     /* [in] */ ArrayOf<IInterface* >* array,
    //     /* [in] */ Int32 start,
    //     /* [in] */ Int32 end,
    //     /* [in] */ IComparator* comparator);

    // static CARAPI Sort(
    //     /* [in] */ ArrayOf<IInterface* >* array,
    //     /* [in] */ IComparator* comparator);

    // static CARAPI ToStringBoolean(
    //     /* [in] */ ArrayOf<Boolean>* array,
    //     /* [out] */ String* str);

    // static CARAPI ToStringByte(
    //     /* [in] */ ArrayOf<Byte>* array,
    //     /* [out] */ String* str);

    // static CARAPI ToStringChar32(
    //     /* [in] */ ArrayOf<Char32>* array,
    //     /* [out] */ String* str);

    // static CARAPI ToStringDouble(
    //     /* [in] */ ArrayOf<Double>* array,
    //     /* [out] */ String* str);

    // static CARAPI ToStringFloat(
    //     /* [in] */ ArrayOf<Float>* array,
    //     /* [out] */ String* str);

    // static CARAPI ToStringInt32(
    //     /* [in] */ ArrayOf<Int32>* array,
    //     /* [out] */ String* str);

    // static CARAPI ToStringInt64(
    //     /* [in] */ ArrayOf<Int64>* array,
    //     /* [out] */ String* str);

    // static CARAPI ToStringInt16(
    //     /* [in] */ ArrayOf<Int16>* array,
    //     /* [out] */ String* str);

    // static CARAPI ToStringObject(
    //     /* [in] */ ArrayOf<IInterface* >* array,
    //     /* [out] */ String* str);

    // static CARAPI DeepToString(
    //     /* [in] */ ArrayOf<IInterface* >* array,
    //     /* [out] */ String* str);

    // static CARAPI CopyOfBoolean(
    //     /* [in] */ ArrayOf<Boolean>* original,
    //     /* [in] */ Int32 newLength,
    //     /* [out, callee] */ ArrayOf<Boolean>** result);

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
    //     /* [in] */ ArrayOf<IInterface* >* original,
    //     /* [in] */ int newLength,
    //     /* [out, callee] */ ArrayOf<IInterface* >** result);

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
    //     /* [in] */ ArrayOf<IInterface* >* original,
    //     /* [in] */ Int32 start,
    //     /* [in] */ Int32 end,
    //     /* [out, callee] */ ArrayOf<IInterface* >** arrayCopy);

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

private:

    static CARAPI CheckOffsetAndCount(
        /* [in] */ Int32 arrayLength,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count);

    static CARAPI CheckBinarySearchBounds(
        /* [in] */ Int32 startIndex,
        /* [in] */ Int32 endIndex,
        /* [in] */ Int32 length);

    // static CARAPI_(Int32) DeepHashCodeElement(
    //     /* [in] */ IInterface* element);

    // static CARAPI_(Boolean) DeepEqualsElements(
    //     /* [in] */ IInterface* e1,
    //     /* [in] */ IInterface* e2);

    // static CARAPI_(void) DeepToStringImpl(
    //     /* [in] */ ArrayOf<IInterface* >* array,
    //     /* [in] */ ArrayOf<IInterface* >* origArrays,
    //     /* [in] */ StringBuilder* sb);

    // static CARAPI_(Boolean) DeepToStringImplContains(
    //     /* [in] */ ArrayOf<IInterface* >* origArrays,
    //     /* [in] */ IInterface* array);

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
    /* [in] */ Byte value,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index)
    assert(array != NULL);

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
Int32 Arrays::BinarySearch(
    /* [in] */ ArrayOf<T>* array,
    /* [in] */ Byte value,
    /* [out] */ Int32* index)
{
    assert(array != NULL);
    return BinarySearch(array, 0, array->GetLength(), value, index);
}

} // namespace Utility
} // namespace Elastos

#endif // __ELASTOS_UTILITY_ARRAYS_H__