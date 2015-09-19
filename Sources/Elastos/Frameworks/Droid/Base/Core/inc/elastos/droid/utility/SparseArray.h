
#ifndef __ELASTOS_DROID_UTILITY_SPARSEARRAY_H__
#define __ELASTOS_DROID_UTILITY_SPARSEARRAY_H__

#include "utility/ArrayUtils.h"

namespace Elastos {
namespace Droid {
namespace Utility {

/**
 * SparseArrays map integers to Objects.  Unlike a normal array of Objects,
 * there can be gaps in the indices.  It is intended to be more efficient
 * than using a HashMap to map Integers to Objects.
 */
class SparseArray : public ElRefBase
{
    /**
     * Creates a new SparseArray containing no mappings that will not
     * require any additional memory allocation to store the specified
     * number of mappings.
     */
public:
    SparseArray(
        /* [in] */ Int32 initialCapacity = 10);

    ~SparseArray();

    /**
     * Gets the Object mapped from the specified key, or <code>null</code>
     * if no such mapping has been made.
     */
    CARAPI_(IInterface*) Get(
        /* [in] */ Int32 key);

    /**
     * Gets the Object mapped from the specified key, or the specified Object
     * if no such mapping has been made.
     */
    CARAPI_(IInterface*) Get(
        /* [in] */ Int32 key,
        /* [in] */ IInterface* valueIfKeyNotFound);

    /**
     * Removes the mapping from the specified key, if there was any.
     */
    CARAPI_(void) Delete(
        /* [in] */ Int32 key);

    /**
     * Alias for {@link #delete(Int32)}.
     */
    CARAPI_(void) Remove(
        /* [in] */ Int32 key);

    /**
     * Removes the mapping at the specified index.
     * @hide
     */
    CARAPI_(void) RemoveAt(
        /* [in] */ Int32 index);

private:
    CARAPI_(void) Gc();

    /**
     * Adds a mapping from the specified key to the specified value,
     * replacing the previous mapping from the specified key if there
     * was one.
     */
public:
    CARAPI_(void) Put(
        /* [in] */ Int32 key,
        /* [in] */ IInterface* value);

    /**
     * Returns the number of key-value mappings that this SparseArray
     * currently stores.
     */
    CARAPI_(Int32) Size();

    /**
     * Given an index in the range <code>0...size()-1</code>, returns
     * the key from the <code>index</code>th key-value mapping that this
     * SparseArray stores.
     */
    CARAPI_(Int32) KeyAt(
        /* [in] */ Int32 index);

    /**
     * Given an index in the range <code>0...size()-1</code>, returns
     * the value from the <code>index</code>th key-value mapping that this
     * SparseArray stores.
     */
    CARAPI_(IInterface*) ValueAt(
        /* [in] */ Int32 index);

    /**
     * Given an index in the range <code>0...size()-1</code>, sets a new
     * value for the <code>index</code>th key-value mapping that this
     * SparseArray stores.
     */
    CARAPI_(void) SetValueAt(
        /* [in] */ Int32 index,
        /* [in] */ IInterface* value);

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
        /* [in] */ IInterface* value);

    /**
     * Removes all key-value mappings from this SparseArray.
     */
    CARAPI_(void) Clear();

    /**
     * Puts a key/value pair into the array, optimizing for the case where
     * the key is greater than all existing keys in the array.
     */
    CARAPI_(void) Append(
        /* [in] */ Int32 key,
        /* [in] */ IInterface* value);

private:
    static CARAPI_(Int32) BinarySearch(
        /* [in] */ ArrayOf<Int32>* a,
        /* [in] */ Int32 start,
        /* [in] */ Int32 len,
        /* [in] */ Int32 key);

private:
    Boolean mGarbage;
    AutoPtr<ArrayOf<Int32> > mKeys;
    AutoPtr<ArrayOf<IInterface*> > mValues;
    Int32 mSize;
};

}
}
}

#endif
