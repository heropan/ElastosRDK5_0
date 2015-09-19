
#ifndef __ELASTOS_DROID_UTILITY_SPARSEINT32ARRAY_H__
#define __ELASTOS_DROID_UTILITY_SPARSEINT32ARRAY_H__

#include "utility/ArrayUtils.h"

namespace Elastos {
namespace Droid {
namespace Utility {

/**
 * SparseIntArrays map integers to integers.  Unlike a normal array of integers,
 * there can be gaps in the indices.  It is intended to be more efficient
 * than using a HashMap to map Integers to Integers.
 */
class SparseInt32Array : public ElRefBase
{
public:
    /**
     * Creates a new SparseIntArray containing no mappings.
     */
    //SparseInt32Array();

    /**
     * Creates a new SparseIntArray containing no mappings that will not
     * require any additional memory allocation to store the specified
     * number of mappings.
     */
    SparseInt32Array(
        /* [in] */ Int32 initialCapacity = 10);

    ~SparseInt32Array();

    Clone(
        /* [out] */ SparseInt32Array** array);

    /**
     * Gets the Int32 mapped from the specified key, or <code>0</code>
     * if no such mapping has been made.
     */
    CARAPI_(Int32) Get(
        /* [in] */ Int32 key);

    /**
     * Gets the Int32 mapped from the specified key, or the specified value
     * if no such mapping has been made.
     */
    CARAPI_(Int32) Get(
        /* [in] */ Int32 key,
        /* [in] */ Int32 valueIfKeyNotFound);

    /**
     * Removes the mapping from the specified key, if there was any.
     */
    CARAPI_(void) Delete(
        /* [in] */ Int32 key);

    /**
     * Removes the mapping at the given index.
     */
    CARAPI_(void) RemoveAt(
        /* [in] */ Int32 index);

    /**
     * Adds a mapping from the specified key to the specified value,
     * replacing the previous mapping from the specified key if there
     * was one.
     */
    CARAPI_(void) Put(
        /* [in] */ Int32 key,
        /* [in] */ Int32 value);

    /**
     * Returns the number of key-value mappings that this SparseIntArray
     * currently stores.
     */
    CARAPI_(Int32) Size();

    /**
     * Given an index in the range <code>0...size()-1</code>, returns
     * the key from the <code>index</code>th key-value mapping that this
     * SparseIntArray stores.
     */
    CARAPI_(Int32) KeyAt(
        /* [in] */ Int32 index);

    /**
     * Given an index in the range <code>0...size()-1</code>, returns
     * the value from the <code>index</code>th key-value mapping that this
     * SparseIntArray stores.
     */
    CARAPI_(Int32) ValueAt(
        /* [in] */ Int32 index);

    /**
     * Returns the index for which {@link #keyAt} would return the
     * specified key, or a negative number if the specified
     * key is not mapped.
     */
    CARAPI_(Int32) IndexOfKey(
        /* [in] */ Int32 key);

    /**
     * Returns an index for which {@link #valueAt} would return the
     * specified key, or a negative number if no keys map to the
     * specified value.
     * Beware that this is a linear search, unlike lookups by key,
     * and that multiple keys can map to the same value and this will
     * find only one of them.
     */
    CARAPI_(Int32) IndexOfValue(
        /* [in] */ Int32 value);

    /**
     * Removes all key-value mappings from this SparseIntArray.
     */
    CARAPI_(void) Clear();

    /**
     * Puts a key/value pair into the array, optimizing for the case where
     * the key is greater than all existing keys in the array.
     */
    CARAPI_(void) Append(
        /* [in] */ Int32 key,
        /* [in] */ Int32 value);

private:
    static CARAPI_(Int32) BinarySearch(
        /* [in] */ ArrayOf<Int32>* a,
        /* [in] */ Int32 start,
        /* [in] */ Int32 len,
        /* [in] */ Int32 key);

    CARAPI_(void) CheckIntegrity();

    AutoPtr<ArrayOf<Int32> > mKeys;
    AutoPtr<ArrayOf<Int32> > mValues;
    Int32 mSize;
};

} // namespace Utility
} // namespace Droid
} // namespace Elastos

#endif

