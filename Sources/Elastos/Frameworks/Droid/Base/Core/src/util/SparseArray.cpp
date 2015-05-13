
#include "util/SparseArray.h"

using Elastos::Droid::Internal::Utility::ArrayUtils;

namespace Elastos {
namespace Droid {
namespace Utility {

/**
 * Creates a new SparseArray containing no mappings that will not
 * require any additional memory allocation to store the specified
 * number of mappings.
 */

SparseArray::SparseArray(
    /* [in] */ Int32 initialCapacity)
{
    mGarbage = FALSE;

    initialCapacity = ArrayUtils::IdealInt32ArraySize(initialCapacity);

    mKeys = ArrayOf<Int32>::Alloc(initialCapacity);
    mValues = ArrayOf<IInterface*>::Alloc(initialCapacity);
    mSize = 0;
}

SparseArray::~SparseArray()
{
}

/**
 * Gets the Object mapped from the specified key, or <code>NULL</code>
 * if no such mapping has been made.
 */
IInterface* SparseArray::Get(
    /* [in] */ Int32 key)
{
    return Get(key, NULL);
}

/**
 * Gets the Object mapped from the specified key, or the specified Object
 * if no such mapping has been made.
 */
IInterface* SparseArray::Get(
    /* [in] */ Int32 key,
    /* [in] */ IInterface* valueIfKeyNotFound)
{
    Int32 i = BinarySearch(mKeys, 0, mSize, key);

    if (i < 0 || (*mValues)[i] == NULL) {
        return valueIfKeyNotFound;
    } else {
        return (*mValues)[i];
    }
}

/**
 * Removes the mapping from the specified key, if there was any.
 */
void SparseArray::Delete(
    /* [in] */ Int32 key)
{
    Int32 i = BinarySearch(mKeys, 0, mSize, key);

    if (i >= 0) {
        if ((*mValues)[i] != NULL) {
            mValues->Set(i, NULL);
            mGarbage = TRUE;
        }
    }
}

/**
 * Alias for {@link #delete(Int32)}.
 */
void SparseArray::Remove(
    /* [in] */ Int32 key)
{
    Delete(key);
}

/**
 * Removes the mapping at the specified index.
 * @hide
 */
void SparseArray::RemoveAt(
    /* [in] */ Int32 index)
{
    if ((*mValues)[index] != NULL) {
        mValues->Set(index, NULL);
        mGarbage = TRUE;
    }
}

void SparseArray::Gc()
{
    // Log.e("SparseArray", "gc start with " + mSize);

    Int32 n = mSize;
    Int32 o = 0;
    ArrayOf<Int32>* keys = mKeys;
    ArrayOf<IInterface*>* values = mValues;

    for (Int32 i = 0; i < n; i++) {
        IInterface* val = (*values)[i];

        if (val != NULL) {
            if (i != o) {
                (*keys)[o] = (*keys)[i];
                values->Set(o, val);
            }

            o++;
        }
    }

    mGarbage = FALSE;
    mSize = o;

    // Log.e("SparseArray", "gc end with " + mSize);
}

/**
 * Adds a mapping from the specified key to the specified value,
 * replacing the previous mapping from the specified key if there
 * was one.
 */
void SparseArray::Put(
    /* [in] */ Int32 key,
    /* [in] */ IInterface* value)
{
    Int32 i = BinarySearch(mKeys, 0, mSize, key);

    if (i >= 0) {
        mValues->Set(i, value);
    } else {
        i = ~i;

        if (i < mSize && (*mValues)[i] == NULL) {
            (*mKeys)[i] = key;
            mValues->Set(i, value);
            return;
        }

        if (mGarbage && mSize >= mKeys->GetLength()) {
            Gc();

            // Search again because indices may have changed.
            i = ~BinarySearch(mKeys, 0, mSize, key);
        }

        if (mSize >= mKeys->GetLength()) {
            Int32 n = ArrayUtils::IdealInt32ArraySize(mSize + 1);

            AutoPtr<ArrayOf<Int32> > nkeys = ArrayOf<Int32>::Alloc(n);
            AutoPtr<ArrayOf<IInterface*> > nvalues = ArrayOf<IInterface*>::Alloc(n);

            // Log.e("SparseArray", "grow " + mKeys.length + " to " + n);
            //System.arraycopy(mKeys, 0, nkeys, 0, mKeys.length);
            nkeys->Copy(mKeys);
            nvalues->Copy(mValues);

            mKeys = nkeys;
            mValues = nvalues;
        }

        if (mSize - i != 0) {
            // Log.e("SparseArray", "move " + (mSize - i));
            //System.arraycopy(mKeys, i, mKeys, i + 1, mSize - i);
            //System.arraycopy(mValues, i, mValues, i + 1, mSize - i);

            for (Int32 j = 0; j < mSize - i; j++) {
                (*mKeys)[1 + mSize - j] = (*mKeys)[mSize - j];
                mValues->Set(1 + mSize - j, (*mValues)[mSize - j]);
            }
        }

        (*mKeys)[i] = key;
        mValues->Set(i, value);
        mSize++;
    }
}

/**
 * Returns the number of key-value mappings that this SparseArray
 * currently stores.
 */
Int32 SparseArray::Size()
{
    if (mGarbage) {
        Gc();
    }

    return mSize;
}

/**
 * Given an index in the range <code>0...size()-1</code>, returns
 * the key from the <code>index</code>th key-value mapping that this
 * SparseArray stores.
 */
Int32 SparseArray::KeyAt(
    /* [in] */ Int32 index)
{
    if (mGarbage) {
        Gc();
    }

    return (*mKeys)[index];
}

/**
 * Given an index in the range <code>0...size()-1</code>, returns
 * the value from the <code>index</code>th key-value mapping that this
 * SparseArray stores.
 */
IInterface* SparseArray::ValueAt(
    /* [in] */ Int32 index)
{
    if (mGarbage) {
        Gc();
    }

    return (*mValues)[index];
}

/**
 * Given an index in the range <code>0...size()-1</code>, sets a new
 * value for the <code>index</code>th key-value mapping that this
 * SparseArray stores.
 */
void SparseArray::SetValueAt(
    /* [in] */ Int32 index,
    /* [in] */ IInterface* value)
{
    if (mGarbage) {
        Gc();
    }

    (*mValues)[index] = value;
}

/**
 * Returns the index for which {@link #keyAt} would return the
 * specified key, or a negative number if the specified
 * key is not mapped.
 */
Int32 SparseArray::IndexOfKey(
    /* [in] */ Int32 key)
{
    if (mGarbage) {
        Gc();
    }

    return BinarySearch(mKeys, 0, mSize, key);
}

/**
 * Returns an index for which {@link #valueAt} would return the
 * specified key, or a negative number if no keys map to the
 * specified value.
 * Beware that this is a linear search, unlike lookups by key,
 * and that multiple keys can map to the same value and this will
 * find only one of them.
 */
Int32 SparseArray::IndexOfValue(
    /* [in] */ IInterface* value)
{
    if (mGarbage) {
        Gc();
    }

    for (Int32 i = 0; i < mSize; i++)
        if ((*mValues)[i] == value)
            return i;

    return -1;
}

/**
 * Removes all key-value mappings from this SparseArray.
 */
void SparseArray::Clear()
{
    Int32 n = mSize;
    ArrayOf<IInterface*>* values = mValues;

    for (Int32 i = 0; i < n; i++) {
        values->Set(i, NULL);
    }

    mSize = 0;
    mGarbage = FALSE;
}

/**
 * Puts a key/value pair into the array, optimizing for the case where
 * the key is greater than all existing keys in the array.
 */
void SparseArray::Append(
    /* [in] */ Int32 key,
    /* [in] */ IInterface* value)
{
    if (mSize != 0 && key <= (*mKeys)[mSize - 1]) {
        Put(key, value);
        return;
    }

    if (mGarbage && mSize >= mKeys->GetLength()) {
        Gc();
    }

    Int32 pos = mSize;
    if (pos >= mKeys->GetLength()) {
        Int32 n = ArrayUtils::IdealInt32ArraySize(pos + 1);

        AutoPtr<ArrayOf<Int32> > nkeys = ArrayOf<Int32>::Alloc(n);
        AutoPtr<ArrayOf<IInterface*> > nvalues = ArrayOf<IInterface*>::Alloc(n);

        // Log.e("SparseArray", "grow " + mKeys.length + " to " + n);
        nkeys->Copy(mKeys);
        nvalues->Copy(mValues);
        mKeys = nkeys;
        mValues = nvalues;
    }

    (*mKeys)[pos] = key;
    mValues->Set(pos, value);
    mSize = pos + 1;
}

Int32 SparseArray::BinarySearch(
    /* [in] */ ArrayOf<Int32>* a,
    /* [in] */ Int32 start,
    /* [in] */ Int32 len,
    /* [in] */ Int32 key)
{
    Int32 high = start + len, low = start - 1, guess;

    while (high - low > 1) {
        guess = (high + low) / 2;

        if ((*a)[guess] < key)
            low = guess;
        else
            high = guess;
    }

    if (high == start + len)
        return ~(start + len);
    else if ((*a)[high] == key)
        return high;
    else
        return ~high;
}

}
}
}
