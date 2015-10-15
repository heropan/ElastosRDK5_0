
#ifndef __ELASTOS_DROID_INTERNAL_UTILITY_GROWING_ARRAYUTILS_H__
#define __ELASTOS_DROID_INTERNAL_UTILITY_GROWING_ARRAYUTILS_H__

#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/internal/utility/ArrayUtils.h"

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Utility {

/**
 * A helper class that aims to provide comparable growth performance to ArrayList, but on primitive
 * arrays. Common array operations are implemented for efficient use in dynamic containers.
 *
 * All methods in this class assume that the length of an array is equivalent to its capacity and
 * NOT the number of elements in the array. The current size of the array is always passed in as a
 * parameter.
 *
 * @hide
 */
class GrowingArrayUtils {
public:

    //TODO

    /**
     * Appends an element to the end of the array, growing the array if there is no more room.
     * @param array The array to which to append the element. This must NOT be null.
     * @param currentSize The number of elements in the array. Must be less than or equal to
     *                    array.length.
     * @param element The element to append.
     * @return the array to which the element was appended. This may be different than the given
     *         array.
     */
    template<typename T>
    static AutoPtr<ArrayOf<T> > Append(
        /* [in] */ ArrayOf<T>* array,
        /* [in] */ Int32 currentSize,
        /* [in] */ T element);

    template<typename T>
    static AutoPtr<ArrayOf<T> > Append(
        /* [in] */ AutoPtr< ArrayOf<T> >& array,
        /* [in] */ Int32 currentSize,
        /* [in] */ T element);

    // *
    //  * Inserts an element into the array at the specified index, growing the array if there is no
    //  * more room.
    //  *
    //  * @param array The array to which to append the element. Must NOT be null.
    //  * @param currentSize The number of elements in the array. Must be less than or equal to
    //  *                    array.length.
    //  * @param element The element to insert.
    //  * @return the array to which the element was appended. This may be different than the given
    //  *         array.

    // public static <T> T[] insert(T[] array, int currentSize, int index, T element) {
    //     assert currentSize <= array.length;

    //     if (currentSize + 1 <= array.length) {
    //         System.arraycopy(array, index, array, index + 1, currentSize - index);
    //         array[index] = element;
    //         return array;
    //     }

    //     @SuppressWarnings("unchecked")
    //     T[] newArray = ArrayUtils.newUnpaddedArray((Class<T>)array.getClass().getComponentType(),
    //             growSize(currentSize));
    //     System.arraycopy(array, 0, newArray, 0, index);
    //     newArray[index] = element;
    //     System.arraycopy(array, index, newArray, index + 1, array.length - index);
    //     return newArray;
    // }

    // /**
    //  * Primitive int version of {@link #insert(Object[], int, int, Object)}.
    //  */
    // public static int[] insert(int[] array, int currentSize, int index, int element) {
    //     assert currentSize <= array.length;

    //     if (currentSize + 1 <= array.length) {
    //         System.arraycopy(array, index, array, index + 1, currentSize - index);
    //         array[index] = element;
    //         return array;
    //     }

    //     int[] newArray = ArrayUtils.newUnpaddedIntArray(growSize(currentSize));
    //     System.arraycopy(array, 0, newArray, 0, index);
    //     newArray[index] = element;
    //     System.arraycopy(array, index, newArray, index + 1, array.length - index);
    //     return newArray;
    // }

    // /**
    //  * Primitive long version of {@link #insert(Object[], int, int, Object)}.
    //  */
    // public static long[] insert(long[] array, int currentSize, int index, long element) {
    //     assert currentSize <= array.length;

    //     if (currentSize + 1 <= array.length) {
    //         System.arraycopy(array, index, array, index + 1, currentSize - index);
    //         array[index] = element;
    //         return array;
    //     }

    //     long[] newArray = ArrayUtils.newUnpaddedLongArray(growSize(currentSize));
    //     System.arraycopy(array, 0, newArray, 0, index);
    //     newArray[index] = element;
    //     System.arraycopy(array, index, newArray, index + 1, array.length - index);
    //     return newArray;
    // }

    // /**
    //  * Primitive boolean version of {@link #insert(Object[], int, int, Object)}.
    //  */
    // public static boolean[] insert(boolean[] array, int currentSize, int index, boolean element) {
    //     assert currentSize <= array.length;

    //     if (currentSize + 1 <= array.length) {
    //         System.arraycopy(array, index, array, index + 1, currentSize - index);
    //         array[index] = element;
    //         return array;
    //     }

    //     boolean[] newArray = ArrayUtils.newUnpaddedBooleanArray(growSize(currentSize));
    //     System.arraycopy(array, 0, newArray, 0, index);
    //     newArray[index] = element;
    //     System.arraycopy(array, index, newArray, index + 1, array.length - index);
    //     return newArray;
    // }

    /**
     * Given the current size of an array, returns an ideal size to which the array should grow.
     * This is typically double the given size, but should not be relied upon to do so in the
     * future.
     */
    static Int32 GrowSize(
        /* [in] */ Int32 currentSize);

private:
    GrowingArrayUtils();
    GrowingArrayUtils(const GrowingArrayUtils&);
};

template<typename T>
AutoPtr<ArrayOf<T> > GrowingArrayUtils::Append(
    /* [in] */ ArrayOf<T>* array,
    /* [in] */ Int32 currentSize,
    /* [in] */ T element)
 {
    assert(array != NULL && currentSize <= array->GetLength());

    AutoPtr<ArrayOf<T> > result = array;
    if (currentSize + 1 > array->GetLength()) {
        // @SuppressWarnings("unchecked")
        result = ArrayUtils::NewUnpaddedArray(GrowSize(currentSize), element);
        result->Copy(array);
    }
    result->Set(currentSize, element);
    return result;
}

template<typename T>
AutoPtr<ArrayOf<T> > GrowingArrayUtils::Append(
    /* [in] */ AutoPtr< ArrayOf<T> >& array,
    /* [in] */ Int32 currentSize,
    /* [in] */ T element)
{
    return Append(array.Get(), currentSize, element);
}

} // namespace Utility
} // namespace Internal
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_INTERNAL_UTILITY_GROWING_ARRAYUTILS_H__
