#ifndef __UTILITY_CARRAYS_H__
#define __UTILITY_CARRAYS_H__

#include "_Elastos_Utility_CArrays.h"
#include "AbstractList.h"
#include "StringBuilder.h"
#include "Singleton.h"

using Elastos::Core::IComparator;
using Elastos::Core::StringBuilder;
using Elastos::Core::Singleton;
using Elastos::IO::ISerializable;

namespace Elastos {
namespace Utility {

CarClass(CArrays)
    , public Singleton
    , public IArrays
{
private:
    class ArrayList
        : public AbstractList
        , public ISerializable
        , public IRandomAccess
    {
    public:
        CAR_INTERFACE_DECL()

        ArrayList(
            /* [in] */ ArrayOf<IInterface* >* storage);

        CARAPI Contains(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* result);

        CARAPI Get(
            /* [in] */ Int32 location,
            /* [out] */ IInterface** result);

        CARAPI IndexOf(
            /* [in] */ IInterface* object,
            /* [out] */ Int32* result);

        CARAPI LastIndexOf(
            /* [in] */ IInterface* object,
            /* [out] */ Int32* index);

        CARAPI Set(
            /* [in] */ Int32 location,
            /* [in] */ IInterface* object,
            /* [out] */ IInterface** result);

        CARAPI GetSize(
            /* [out] */ Int32* size);

        CARAPI ToArray(
            /* [out] */ ArrayOf<IInterface* >** outArray);

        CARAPI ToArray(
            /* [in] */ ArrayOf<IInterface* >* contents,
            /* [out] */ ArrayOf<IInterface* >** outArray);

    private:
        AutoPtr<ArrayOf<IInterface*> > mA;
    };

public:
    CAR_SINGLETON_DECL()

    CAR_INTERFACE_DECL()

    CARAPI AsList(
        /* [in] */ ArrayOf<IInterface*>* array,
        /* [out] */ IList** outList);

    CARAPI BinarySearchByte(
        /* [in] */ ArrayOf<Byte>* array,
        /* [in] */ Byte value,
        /* [out] */ Int32* index);

    CARAPI BinarySearchByte(
        /* [in] */ ArrayOf<Byte>* array,
        /* [in] */ Int32 startIndex,
        /* [in] */ Int32 endIndex,
        /* [in] */ Byte value,
        /* [out] */ Int32* index);

    CARAPI BinarySearchChar32(
        /* [in] */ ArrayOf<Char32>* array,
        /* [in] */ Char32 value,
        /* [out] */ Int32* index);

    CARAPI BinarySearchChar32(
        /* [in] */ ArrayOf<Char32>* array,
        /* [in] */ Int32 startIndex,
        /* [in] */ Int32 endIndex,
        /* [in] */ Char32 value,
        /* [out] */ Int32* index);

    CARAPI BinarySearchDouble(
        /* [in] */ ArrayOf<Double>* array,
        /* [in] */ Double value,
        /* [out] */ Int32* index);

    CARAPI BinarySearchDouble(
        /* [in] */ ArrayOf<Double>* array,
        /* [in] */ Int32 startIndex,
        /* [in] */ Int32 endIndex,
        /* [in] */ Double value,
        /* [out] */ Int32* index);

    CARAPI BinarySearchFloat(
        /* [in] */ ArrayOf<Float>* array,
        /* [in] */ Float value,
        /* [out] */ Int32* index);

    CARAPI BinarySearchFloat(
        /* [in] */ ArrayOf<Float>* array,
        /* [in] */ Int32 startIndex,
        /* [in] */ Int32 endIndex,
        /* [in] */ Float value,
        /* [out] */ Int32* index);

    CARAPI BinarySearchInt32(
        /* [in] */ ArrayOf<Int32>* array,
        /* [in] */ Int32 value,
        /* [out] */ Int32* index);

    CARAPI BinarySearchInt32(
        /* [in] */ ArrayOf<Int32>* array,
        /* [in] */ Int32 startIndex,
        /* [in] */ Int32 endIndex,
        /* [in] */ Int32 value,
        /* [out] */ Int32* index);

    CARAPI BinarySearchInt64(
        /* [in] */ ArrayOf<Int64>* array,
        /* [in] */ Int64 value,
        /* [out] */ Int32* index);

    CARAPI BinarySearchInt64(
        /* [in] */ ArrayOf<Int64>* array,
        /* [in] */ Int32 startIndex,
        /* [in] */ Int32 endIndex,
        /* [in] */ Int64 value,
        /* [out] */ Int32* index);

    CARAPI BinarySearchObject(
        /* [in] */ ArrayOf<IInterface* >* array,
        /* [in] */ IInterface* value,
        /* [out] */ Int32* index);

    CARAPI BinarySearchObject(
        /* [in] */ ArrayOf<IInterface* >* array,
        /* [in] */ Int32 startIndex,
        /* [in] */ Int32 endIndex,
        /* [in] */ IInterface* value,
        /* [out] */ Int32* index);

    CARAPI BinarySearch(
        /* [in] */ ArrayOf<IInterface* >* array,
        /* [in] */ IInterface* value,
        /* [in] */ IComparator* comparator,
        /* [out] */ Int32* index);

    CARAPI BinarySearch(
        /* [in] */ ArrayOf<IInterface* >* array,
        /* [in] */ Int32 startIndex,
        /* [in] */ Int32 endIndex,
        /* [in] */ IInterface* value,
        /* [in] */ IComparator* comparator,
        /* [out] */ Int32* index);

    CARAPI BinarySearchInt16(
        /* [in] */ ArrayOf<Int16>* array,
        /* [in] */ Int16 value,
        /* [out] */ Int32* index);

    CARAPI BinarySearchInt16(
        /* [in] */ ArrayOf<Int16>* array,
        /* [in] */ Int32 startIndex,
        /* [in] */ Int32 endIndex,
        /* [in] */ Int16 value,
        /* [out] */ Int32* index);

    CARAPI FillByte(
        /* [in] */ ArrayOf<Byte>* array,
        /* [in] */ Byte value);

    CARAPI FillByte(
        /* [in] */ ArrayOf<Byte>* array,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Byte value);

    CARAPI FillInt16(
        /* [in] */ ArrayOf<Int16>* array,
        /* [in] */ Int16 value);

    CARAPI FillInt16(
        /* [in] */ ArrayOf<Int16>* array,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Int16 value);

    CARAPI FillChar32(
        /* [in] */ ArrayOf<Char32>* array,
        /* [in] */ Char32 value);

    CARAPI FillChar32(
        /* [in] */ ArrayOf<Char32>* array,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Char32 value);

    CARAPI FillInt32(
        /* [in] */ ArrayOf<Int32>* array,
        /* [in] */ Int32 value);

    CARAPI FillInt32(
        /* [in] */ ArrayOf<Int32>* array,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Int32 value);

    CARAPI FillInt64(
        /* [in] */ ArrayOf<Int64>* array,
        /* [in] */ Int64 value);

    CARAPI FillInt64(
        /* [in] */ ArrayOf<Int64>* array,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Int64 value);

    CARAPI FillFloat(
        /* [in] */ ArrayOf<Float>* array,
        /* [in] */ Float value);

    CARAPI FillFloat(
        /* [in] */ ArrayOf<Float>* array,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Float value);

    CARAPI FillDouble(
        /* [in] */ ArrayOf<Double>* array,
        /* [in] */ Double value);

    CARAPI FillDouble(
        /* [in] */ ArrayOf<Double>* array,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Double value);

    CARAPI FillBoolean(
        /* [in] */ ArrayOf<Boolean>* array,
        /* [in] */ Boolean value);

    CARAPI FillBoolean(
        /* [in] */ ArrayOf<Boolean>* array,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Boolean value);

    CARAPI FillObject(
        /* [in] */ ArrayOf<IInterface* >* array,
        /* [in] */ IInterface* value);

    CARAPI FillObject(
        /* [in] */ ArrayOf<IInterface* >* array,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ IInterface* value);

    CARAPI HashCodeBoolean(
        /* [in] */ ArrayOf<Boolean>* array,
        /* [out] */ Int32* code);

    CARAPI HashCodeInt32(
        /* [in] */ ArrayOf<Int32>* array,
        /* [out] */ Int32* code);

    CARAPI HashCodeInt16(
        /* [in] */ ArrayOf<Int16>* array,
        /* [out] */ Int32* code);

    CARAPI HashCodeChar32(
        /* [in] */ ArrayOf<Char32>* array,
        /* [out] */ Int32* code);

    CARAPI HashCodeByte(
        /* [in] */ ArrayOf<Byte>* array,
        /* [out] */ Int32* code);

    CARAPI HashCodeInt64(
        /* [in] */ ArrayOf<Int64>* array,
        /* [out] */ Int32* code);

    CARAPI HashCodeFloat(
        /* [in] */ ArrayOf<Float>* array,
        /* [out] */ Int32* code);

    CARAPI HashCodeDouble(
        /* [in] */ ArrayOf<Double>* array,
        /* [out] */ Int32* code);

    CARAPI HashCodeObject(
        /* [in] */ ArrayOf<IInterface* >* array,
        /* [out] */ Int32* code);

    CARAPI DeepHashCode(
        /* [in] */ ArrayOf<IInterface* >* array,
        /* [out] */ Int32* code);

    CARAPI EqualsByte(
        /* [in] */ ArrayOf<Byte>* array1,
        /* [in] */ ArrayOf<Byte>* array2,
        /* [out] */ Boolean* result);

    CARAPI EqualsInt16(
        /* [in] */ ArrayOf<Int16>* array1,
        /* [in] */ ArrayOf<Int16>* array2,
        /* [out] */ Boolean* result);

    CARAPI EqualsChar32(
        /* [in] */ ArrayOf<Char32>* array1,
        /* [in] */ ArrayOf<Char32>* array2,
        /* [out] */ Boolean* result);

    CARAPI EqualsInt32(
        /* [in] */ ArrayOf<Int32>* array1,
        /* [in] */ ArrayOf<Int32>* array2,
        /* [out] */ Boolean* result);

    CARAPI EqualsInt64(
        /* [in] */ ArrayOf<Int64>* array1,
        /* [in] */ ArrayOf<Int64>* array2,
        /* [out] */ Boolean* result);

    CARAPI EqualsFloat(
        /* [in] */ ArrayOf<Float>* array1,
        /* [in] */ ArrayOf<Float>* array2,
        /* [out] */ Boolean* result);

    CARAPI EqualsDouble(
        /* [in] */ ArrayOf<Double>* array1,
        /* [in] */ ArrayOf<Double>* array2,
        /* [out] */ Boolean* result);

    CARAPI EqualsBoolean(
        /* [in] */ ArrayOf<Boolean>* array1,
        /* [in] */ ArrayOf<Boolean>* array2,
        /* [out] */ Boolean* result);

    CARAPI EqualsObject(
        /* [in] */ ArrayOf<IInterface* >* array1,
        /* [in] */ ArrayOf<IInterface* >* array2,
        /* [out] */ Boolean* result);

    CARAPI DeepEquals(
        /* [in] */ ArrayOf<IInterface* >* array1,
        /* [in] */ ArrayOf<IInterface* >* array2,
        /* [out] */ Boolean* result);

    CARAPI SortByte(
        /* [in] */ ArrayOf<Byte>* array);

    CARAPI SortByte(
        /* [in] */ ArrayOf<Byte>* array,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI CheckOffsetAndCount(
        /* [in] */ Int32 arrayLength,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count);

    CARAPI CheckStartAndEnd(
        /* [in] */ Int32 len,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI SortChar32(
        /* [in] */ ArrayOf<Char32>* array);

    CARAPI SortChar32(
        /* [in] */ ArrayOf<Char32>* array,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI SortDouble(
        /* [in] */ ArrayOf<Double>* array);

    CARAPI SortDouble(
        /* [in] */ ArrayOf<Double>* array,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI SortFloat(
        /* [in] */ ArrayOf<Float>* array);

    CARAPI SortFloat(
        /* [in] */ ArrayOf<Float>* array,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI SortInt32(
        /* [in] */ ArrayOf<Int32>* array);

    CARAPI SortInt32(
        /* [in] */ ArrayOf<Int32>* array,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI SortInt64(
        /* [in] */ ArrayOf<Int64>* array);

    CARAPI SortInt64(
        /* [in] */ ArrayOf<Int64>* array,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI SortInt16(
        /* [in] */ ArrayOf<Int16>* array);

    CARAPI SortInt16(
        /* [in] */ ArrayOf<Int16>* array,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI SortObject(
        /* [in] */ ArrayOf<IInterface* >* array);

    CARAPI SortObject(
        /* [in] */ ArrayOf<IInterface* >* array,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI Sort(
        /* [in] */ ArrayOf<IInterface* >* array,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ IComparator* comparator);

    CARAPI Sort(
        /* [in] */ ArrayOf<IInterface* >* array,
        /* [in] */ IComparator* comparator);

    CARAPI ToStringBoolean(
        /* [in] */ ArrayOf<Boolean>* array,
        /* [out] */ String* str);

    CARAPI ToStringByte(
        /* [in] */ ArrayOf<Byte>* array,
        /* [out] */ String* str);

    CARAPI ToStringChar32(
        /* [in] */ ArrayOf<Char32>* array,
        /* [out] */ String* str);

    CARAPI ToStringDouble(
        /* [in] */ ArrayOf<Double>* array,
        /* [out] */ String* str);

    CARAPI ToStringFloat(
        /* [in] */ ArrayOf<Float>* array,
        /* [out] */ String* str);

    CARAPI ToStringInt32(
        /* [in] */ ArrayOf<Int32>* array,
        /* [out] */ String* str);

    CARAPI ToStringInt64(
        /* [in] */ ArrayOf<Int64>* array,
        /* [out] */ String* str);

    CARAPI ToStringInt16(
        /* [in] */ ArrayOf<Int16>* array,
        /* [out] */ String* str);

    CARAPI ToStringObject(
        /* [in] */ ArrayOf<IInterface* >* array,
        /* [out] */ String* str);

    CARAPI DeepToString(
        /* [in] */ ArrayOf<IInterface* >* array,
        /* [out] */ String* str);

    CARAPI CopyOfBoolean(
        /* [in] */ ArrayOf<Boolean>* original,
        /* [in] */ Int32 newLength,
        /* [out, callee] */ ArrayOf<Boolean>** result);

    CARAPI CopyOfByte(
        /* [in] */ ArrayOf<Byte>* original,
        /* [in] */ Int32 newLength,
        /* [out, callee] */ ArrayOf<Byte>** result);

    CARAPI CopyOfChar32(
        /* [in] */ ArrayOf<Char32>* original,
        /* [in] */ Int32 newLength,
        /* [out, callee] */ ArrayOf<Char32>** result);

    CARAPI CopyOfDouble(
        /* [in] */ ArrayOf<Double>* original,
        /* [in] */ Int32 newLength,
        /* [out, callee] */ ArrayOf<Double>** result);

    CARAPI CopyOfFloat(
        /* [in] */ ArrayOf<Float>* original,
        /* [in] */ Int32 newLength,
        /* [out, callee] */ ArrayOf<Float>** result);

    CARAPI CopyOfInt32(
        /* [in] */ ArrayOf<Int32>* original,
        /* [in] */ Int32 newLength,
        /* [out, callee] */ ArrayOf<Int32>** result);

    CARAPI CopyOfInt64(
        /* [in] */ ArrayOf<Int64>* original,
        /* [in] */ Int32 newLength,
        /* [out, callee] */ ArrayOf<Int64>** result);

    CARAPI CopyOfInt16(
        /* [in] */ ArrayOf<Int16>* original,
        /* [in] */ Int32 newLength,
        /* [out, callee] */ ArrayOf<Int16>** result);

    CARAPI CopyOf(
        /* [in] */ ArrayOf<IInterface* >* original,
        /* [in] */ int newLength,
        /* [out, callee] */ ArrayOf<IInterface* >** result);

    //  CARAPI _CopyOf(
    //     /* [in] */ U[] original,
    //     /* [in] */ int newLength,
    //     /* [in] */ Class<? extends T[]> newType,
    //     /* [out, callee] */ <T, U> T[] result);

    CARAPI CopyOfRangeBoolean(
        /* [in] */ ArrayOf<Boolean>* original,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out, callee] */ ArrayOf<Boolean>** arrayCopy);

    CARAPI CopyOfRangeByte(
        /* [in] */ ArrayOf<Byte>* original,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out, callee] */ ArrayOf<Byte>** arrayCopy);

    CARAPI CopyOfRangeChar32(
        /* [in] */ ArrayOf<Char32>* original,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out, callee] */ ArrayOf<Char32>** arrayCopy);

    CARAPI CopyOfRangeDouble(
        /* [in] */ ArrayOf<Double>* original,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out, callee] */ ArrayOf<Double>** arrayCopy);

    CARAPI CopyOfRangeFloat(
        /* [in] */ ArrayOf<Float>* original,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out, callee] */ ArrayOf<Float>** arrayCopy);

    CARAPI CopyOfRangeInt32(
        /* [in] */ ArrayOf<Int32>* original,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out, callee] */ ArrayOf<Int32>** arrayCopy);

    CARAPI CopyOfRangeInt64(
        /* [in] */ ArrayOf<Int64>* original,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out, callee] */ ArrayOf<Int64>** arrayCopy);

    CARAPI CopyOfRangeInt16(
        /* [in] */ ArrayOf<Int16>* original,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out, callee] */ ArrayOf<Int16>** arrayCopy);

    CARAPI CopyOfRange(
        /* [in] */ ArrayOf<IInterface* >* original,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out, callee] */ ArrayOf<IInterface* >** arrayCopy);

    //  CARAPI _CopyOfRange(
    //     /* [in] */ U[] original,
    //     /* [in] */ Int32 start,
    //     /* [in] */ Int32 end,
    //     /* [in] */ Class<? extends T[]> newType,
    //     /* [out, callee] */ <T, U> T[] arrayCopy);

private:

    CARAPI CheckBinarySearchBounds(
        /* [in] */ Int32 startIndex,
        /* [in] */ Int32 endIndex,
        /* [in] */ Int32 length);

    CARAPI_(Int32) DeepHashCodeElement(
        /* [in] */ IInterface* element);

    CARAPI_(Boolean) DeepEqualsElements(
        /* [in] */ IInterface* e1,
        /* [in] */ IInterface* e2);

    CARAPI_(void) DeepToStringImpl(
        /* [in] */ ArrayOf<IInterface* >* array,
        /* [in] */ ArrayOf<IInterface* >* origArrays,
        /* [in] */ StringBuilder* sb);

    CARAPI_(Boolean) DeepToStringImplContains(
        /* [in] */ ArrayOf<IInterface* >* origArrays,
        /* [in] */ IInterface* array);

};

} // namespace Utility
} // namespace Elastos

#endif // __UTILITY_CARRAYS_H__