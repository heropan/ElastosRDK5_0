#ifndef __ELASTOS_UTILITY_TIMSORT_H__
#define __ELASTOS_UTILITY_TIMSORT_H__

#include <elastos/core/Object.h>

using Elastos::Core::IComparator;
using Elastos::Core::IComparable;
using Elastos::Core::Object;

namespace Elastos {
namespace Utility {

class TimSort
    : public Object
{
public:
    /*
     * The next two methods (which are package private and static) constitute
     * the entire API of this class.  Each of these methods obeys the contract
     * of the public method with the same signature in java.util.Arrays.
     */

    static CARAPI Sort(
        /* [in] */ ArrayOf<IInterface*>* a,
        /* [in] */ IComparator* c);

    static CARAPI Sort(
        /* [in] */ ArrayOf<IInterface*>* a,
        /* [in] */ Int32 lo,
        /* [in] */ Int32 hi,
        /* [in] */ IComparator* c);

private:
    /**
     * Creates a TimSort instance to maintain the state of an ongoing sort.
     *
     * @param a the array to be sorted
     */
    TimSort(
        /* [in] */ ArrayOf<IInterface*>* a,
        /* [in] */ IComparator* c);

    /**
     * Sorts the specified portion of the specified array using a binary
     * insertion sort.  This is the best method for sorting small numbers
     * of elements.  It requires O(n log n) compares, but O(n^2) data
     * movement (worst case).
     *
     * If the initial part of the specified range is already sorted,
     * this method can take advantage of it: the method assumes that the
     * elements from index {@code lo}, inclusive, to {@code start},
     * exclusive are already sorted.
     *
     * @param a the array in which a range is to be sorted
     * @param lo the index of the first element in the range to be sorted
     * @param hi the index after the last element in the range to be sorted
     * @param start the index of the first element in the range that is
     *        not already known to be sorted (@code lo <= start <= hi}
     */
    // @SuppressWarnings("fallthrough")
    static CARAPI BinarySort(
        /* [in] */ ArrayOf<IInterface*>* a,
        /* [in] */ Int32 lo,
        /* [in] */ Int32 hi,
        /* [in] */ Int32 start,
        /* [in] */ IComparator* c);

    /**
     * Returns the length of the run beginning at the specified position in
     * the specified array and reverses the run if it is descending (ensuring
     * that the run will always be ascending when the method returns).
     *
     * A run is the longest ascending sequence with:
     *
     *    a[lo] <= a[lo + 1] <= a[lo + 2] <= ...
     *
     * or the longest descending sequence with:
     *
     *    a[lo] >  a[lo + 1] >  a[lo + 2] >  ...
     *
     * For its intended use in a stable mergesort, the strictness of the
     * definition of "descending" is needed so that the call can safely
     * reverse a descending sequence without violating stability.
     *
     * @param a the array in which a run is to be counted and possibly reversed
     * @param lo index of the first element in the run
     * @param hi index after the last element that may be contained in the run.
              It is required that @code{lo < hi}.
     * @return  the length of the run beginning at the specified position in
     *          the specified array
     */
    // @SuppressWarnings("unchecked")
    static CARAPI_(Int32) CountRunAndMakeAscending(
        /* [in] */ ArrayOf<IInterface*>* a,
        /* [in] */ Int32 lo,
        /* [in] */ Int32 hi,
        /* [in] */ IComparator* c);

    /**
     * Reverse the specified range of the specified array.
     *
     * @param a the array in which a range is to be reversed
     * @param lo the index of the first element in the range to be reversed
     * @param hi the index after the last element in the range to be reversed
     */
    static CARAPI ReverseRange(
        /* [in] */ ArrayOf<IInterface*>* a,
        /* [in] */ Int32 lo,
        /* [in] */ Int32 hi);

    /**
     * Returns the minimum acceptable run length for an array of the specified
     * length. Natural runs shorter than this will be extended with
     * {@link #binarySort}.
     *
     * Roughly speaking, the computation is:
     *
     *  If n < MIN_MERGE, return n (it's too small to bother with fancy stuff).
     *  Else if n is an exact power of 2, return MIN_MERGE/2.
     *  Else return an int k, MIN_MERGE/2 <= k <= MIN_MERGE, such that n/k
     *   is close to, but strictly less than, an exact power of 2.
     *
     * For the rationale, see listsort.txt.
     *
     * @param n the length of the array to be sorted
     * @return the length of the minimum run to be merged
     */
    static CARAPI_(Int32) MinRunLength(
        /* [in] */ Int32 n);

    /**
     * Pushes the specified run onto the pending-run stack.
     *
     * @param runBase index of the first element in the run
     * @param runLen  the number of elements in the run
     */
    CARAPI PushRun(
        /* [in] */ Int32 runBase,
        /* [in] */ Int32 runLen);

    /**
     * Examines the stack of runs waiting to be merged and merges adjacent runs
     * until the stack invariants are reestablished:
     *
     *     1. runLen[i - 3] > runLen[i - 2] + runLen[i - 1]
     *     2. runLen[i - 2] > runLen[i - 1]
     *
     * This method is called each time a new run is pushed onto the stack,
     * so the invariants are guaranteed to hold for i < stackSize upon
     * entry to the method.
     */
    CARAPI MergeCollapse();

    /**
     * Merges all runs on the stack until only one remains.  This method is
     * called once, to complete the sort.
     */
    CARAPI MergeForceCollapse();

    /**
     * Merges the two runs at stack indices i and i+1.  Run i must be
     * the penultimate or antepenultimate run on the stack.  In other words,
     * i must be equal to stackSize-2 or stackSize-3.
     *
     * @param i stack index of the first of the two runs to merge
     */
    // @SuppressWarnings("unchecked")
    CARAPI MergeAt(
        /* [in] */ Int32 i);

    /**
     * Locates the position at which to insert the specified key into the
     * specified sorted range; if the range contains an element equal to key,
     * returns the index of the leftmost equal element.
     *
     * @param key the key whose insertion point to search for
     * @param a the array in which to search
     * @param base the index of the first element in the range
     * @param len the length of the range; must be > 0
     * @param hint the index at which to begin the search, 0 <= hint < n.
     *     The closer hint is to the result, the faster this method will run.
     * @return the int k,  0 <= k <= n such that a[b + k - 1] < key <= a[b + k],
     *    pretending that a[b - 1] is minus infinity and a[b + n] is infinity.
     *    In other words, key belongs at index b + k; or in other words,
     *    the first k elements of a should precede key, and the last n - k
     *    should follow it.
     */
    static CARAPI_(Int32) GallopLeft(
        /* [in] */ IInterface* key,
        /* [in] */ ArrayOf<IInterface*>* a,
        /* [in] */ Int32 base,
        /* [in] */ Int32 len,
        /* [in] */ Int32 hint,
        /* [in] */ IComparator* c);

    /**
     * Like gallopLeft, except that if the range contains an element equal to
     * key, gallopRight returns the index after the rightmost equal element.
     *
     * @param key the key whose insertion point to search for
     * @param a the array in which to search
     * @param base the index of the first element in the range
     * @param len the length of the range; must be > 0
     * @param hint the index at which to begin the search, 0 <= hint < n.
     *     The closer hint is to the result, the faster this method will run.
     * @return the int k,  0 <= k <= n such that a[b + k - 1] <= key < a[b + k]
     */
    static CARAPI_(Int32) GallopRight(
        /* [in] */ IInterface* key,
        /* [in] */ ArrayOf<IInterface*>* a,
        /* [in] */ Int32 base,
        /* [in] */ Int32 len,
        /* [in] */ Int32 hint,
        /* [in] */ IComparator* c);

    /**
     * Merges two adjacent runs in place, in a stable fashion.  The first
     * element of the first run must be greater than the first element of the
     * second run (a[base1] > a[base2]), and the last element of the first run
     * (a[base1 + len1-1]) must be greater than all elements of the second run.
     *
     * For performance, this method should be called only when len1 <= len2;
     * its twin, mergeHi should be called if len1 >= len2.  (Either method
     * may be called if len1 == len2.)
     *
     * @param base1 index of first element in first run to be merged
     * @param len1  length of first run to be merged (must be > 0)
     * @param base2 index of first element in second run to be merged
     *        (must be aBase + aLen)
     * @param len2  length of second run to be merged (must be > 0)
     */
    CARAPI MergeLo(
        /* [in] */ Int32 base1,
        /* [in] */ Int32 len1,
        /* [in] */ Int32 base2,
        /* [in] */ Int32 len2);

    /**
     * Like mergeLo, except that this method should be called only if
     * len1 >= len2; mergeLo should be called if len1 <= len2.  (Either method
     * may be called if len1 == len2.)
     *
     * @param base1 index of first element in first run to be merged
     * @param len1  length of first run to be merged (must be > 0)
     * @param base2 index of first element in second run to be merged
     *        (must be aBase + aLen)
     * @param len2  length of second run to be merged (must be > 0)
     */
    // @SuppressWarnings("unchecked")
    CARAPI MergeHi(
        /* [in] */ Int32 base1,
        /* [in] */ Int32 len1,
        /* [in] */ Int32 base2,
        /* [in] */ Int32 len2);

    /**
     * Ensures that the external array tmp has at least the specified
     * number of elements, increasing its size if necessary.  The size
     * increases exponentially to ensure amortized linear time complexity.
     *
     * @param minCapacity the minimum required capacity of the tmp array
     * @return tmp, whether or not it grew
     */
    CARAPI_(AutoPtr< ArrayOf<IInterface*> >) EnsureCapacity(
        /* [in] */ Int32 minCapacity);

private:
    /**
     * This is the minimum sized sequence that will be merged.  Shorter
     * sequences will be lengthened by calling binarySort.  If the entire
     * array is less than this length, no merges will be performed.
     *
     * This constant should be a power of two.  It was 64 in Tim Peter's C
     * implementation, but 32 was empirically determined to work better in
     * this implementation.  In the unlikely event that you set this constant
     * to be a number that's not a power of two, you'll need to change the
     * {@link #minRunLength} computation.
     *
     * If you decrease this constant, you must change the stackLen
     * computation in the TimSort constructor, or you risk an
     * ArrayOutOfBounds exception.  See listsort.txt for a discussion
     * of the minimum stack length required as a function of the length
     * of the array being sorted and the minimum merge sequence length.
     */
    static const Int32 MIN_MERGE = 32;

    /**
     * When we get into galloping mode, we stay there until both runs win less
     * often than MIN_GALLOP consecutive times.
     */
    static const Int32 MIN_GALLOP = 7;

    /**
     * Maximum initial size of tmp array, which is used for merging.  The array
     * can grow to accommodate demand.
     *
     * Unlike Tim's original C version, we do not allocate this much storage
     * when sorting smaller arrays.  This change was required for performance.
     */
    static const Int32 INITIAL_TMP_STORAGE_LENGTH = 256;

    /**
     * Asserts have been placed in if-statements for performace. To enable them,
     * set this field to true and enable them in VM with a command line flag.
     * If you modify this class, please do test the asserts!
     */
    static const Boolean DEBUG = FALSE;

private:
    /**
     * The array being sorted.
     */
    AutoPtr< ArrayOf<IInterface*> > mA;

    /**
     * The comparator for this sort.
     */
    AutoPtr<IComparator> mC;

    /**
     * This controls when we get *into* galloping mode.  It is initialized
     * to MIN_GALLOP.  The mergeLo and mergeHi methods nudge it higher for
     * random data, and lower for highly structured data.
     */
    Int32 mMinGallop; // = MIN_GALLOP;

    /**
     * Temp storage for merges.
     */
    AutoPtr< ArrayOf<IInterface*> > mTmp;

    /**
     * A stack of pending runs yet to be merged.  Run i starts at
     * address base[i] and extends for len[i] elements.  It's always
     * true (so long as the indices are in bounds) that:
     *
     *     runBase[i] + runLen[i] == runBase[i + 1]
     *
     * so we could cut the storage for this, but it's a minor amount,
     * and keeping all the info explicit simplifies the code.
     */
    Int32 mStackSize; // = 0;  // Number of pending runs on stack
    AutoPtr< ArrayOf<Int32> > mRunBase;
    AutoPtr< ArrayOf<Int32> > mRunLen;
};

} // namespace Utility
} // namespace Elastos

#endif // __ELASTOS_UTILITY_TIMSORT_H__
