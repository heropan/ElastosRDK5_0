
#include "coredef.h"
#include "ComparableTimSort.h"
#include "Math.h"

using Elastos::Core::IComparable;
using Elastos::Core::EIID_IComparable;

namespace Elastos{
namespace Utility{

const Int32 ComparableTimSort::MIN_MERGE = 32;
const Int32 ComparableTimSort::MIN_GALLOP = 7;
const Int32 ComparableTimSort::INITIAL_TMP_STORAGE_LENGTH = 256;
const Boolean ComparableTimSort::DEBUG = FALSE;


ECode ComparableTimSort::Sort(
    /* [in] */ const ArrayOf<IInterface*>& a)
{
    return Sort(a, 0, a.GetLength());
}

ECode ComparableTimSort::Sort(
    /* [in] */ const ArrayOf<IInterface*>& a,
    /* [in] */ Int32 lo,
    /* [in] */ Int32 hi)
{
    // Arrays.checkStartAndEnd(a.length, lo, hi);
    if (lo < 0 || hi > a.GetLength()) {
        // throw new ArrayIndexOutOfBoundsException("start < 0 || end > len." + " start=" + start + ", end=" + end + ", len=" + len);
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    if (lo > hi) {
        // throw new IllegalArgumentException("start > end: " + start + " > " + end);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Int32 nRemaining  = hi - lo;
    if (nRemaining < 2)
        return NOERROR;  // Arrays of size 0 and 1 are always sorted

    // If array is small, do a "mini-TimSort" with no merges
    if (nRemaining < MIN_MERGE) {
        Int32 initRunLen = CountRunAndMakeAscending(a, lo, hi);
        BinarySort(a, lo, hi, lo + initRunLen);
        return NOERROR;
    }

    /**
     * March over the array once, left to right, finding natural runs,
     * extending short natural runs to minRun elements, and merging runs
     * to maintain stack invariant.
     */
    ComparableTimSort ts(a);
    Int32 minRun = MinRunLength(nRemaining);
    do {
        // Identify next run
        Int32 runLen = CountRunAndMakeAscending(a, lo, hi);

        // If run is short, extend to min(minRun, nRemaining)
        if (runLen < minRun) {
            Int32 force = nRemaining <= minRun ? nRemaining : minRun;
            BinarySort(a, lo, lo + force, lo + runLen);
            runLen = force;
        }

        // Push run onto pending-run stack, and maybe merge
        ts.PushRun(lo, runLen);
        ts.MergeCollapse();

        // Advance to find next run
        lo += runLen;
        nRemaining -= runLen;
    } while (nRemaining != 0);

    // Merge all remaining runs to complete sort
    if (DEBUG) {
        assert(lo == hi);
    }
    ts.MergeForceCollapse();
    if (DEBUG) {
        assert(ts.mStackSize == 1);
    }
    return NOERROR;
}

ComparableTimSort::ComparableTimSort(
    /* [in] */ const ArrayOf<IInterface*>& a)
{
    mStackSize = 0;
    mMinGallop = MIN_GALLOP;

    mA = a.Clone();

    // Allocate temp storage (which may be increased later if necessary)
    Int32 len = a.GetLength();
    // @SuppressWarnings({"unchecked", "UnnecessaryLocalVariable"})
    AutoPtr<ArrayOf<IInterface*> > newArray = ArrayOf<IInterface*>::Alloc(len < 2 * INITIAL_TMP_STORAGE_LENGTH ?
                                                                            len >> 1 : INITIAL_TMP_STORAGE_LENGTH);
    mTmp = newArray;

    /*
     * Allocate runs-to-be-merged stack (which cannot be expanded).  The
     * stack length requirements are described in listsort.txt.  The C
     * version always uses the same stack length (85), but this was
     * measured to be too expensive when sorting "mid-sized" arrays (e.g.,
     * 100 elements) in Java.  Therefore, we use smaller (but sufficiently
     * large) stack lengths for smaller arrays.  The "magic numbers" in the
     * computation below must be changed if MIN_MERGE is decreased.  See
     * the MIN_MERGE declaration above for more information.
     */
    Int32 stackLen = (len <    120  ?  5 :
                    len <   1542  ? 10 :
                    len < 119151  ? 19 : 40);
    mRunBase = ArrayOf<Int32>::Alloc(stackLen);
    mRunLen = ArrayOf<Int32>::Alloc(stackLen);
}

void ComparableTimSort::BinarySort(
    /* [in] */ const ArrayOf<IInterface*>& a,
    /* [in] */ Int32 lo,
    /* [in] */ Int32 hi,
    /* [in] */ Int32 start)
{
    if (DEBUG) {
        assert(lo <= start && start <= hi);
    }
    if (start == lo) {
        start++;
    }
    for ( ; start < hi; start++) {
        // @SuppressWarnings("unchecked")
        AutoPtr<IComparable> pivot = (IComparable*) a[start]->Probe(EIID_IComparable);

        // Set left (and right) to the index where a[start] (pivot) belongs
        Int32 left = lo;
        Int32 right = start;
        if (DEBUG) {
            assert(left <= right);
        }
        /*
         * Invariants:
         *   pivot >= all in [lo, left).
         *   pivot <  all in [right, start).
         */
        while (left < right) {
            Int32 mid = (left + right) >> 1;
            Int32 flagvalue = 0;
            if ((pivot->CompareTo(a[mid], &flagvalue), flagvalue) < 0) {
                right = mid;
            }
            else {
                left = mid + 1;
            }
        }
        if (DEBUG) {
            assert(left == right);
        }

        /*
         * The invariants still hold: pivot >= all in [lo, left) and
         * pivot < all in [left, start), so pivot belongs at left.  Note
         * that if there are elements equal to pivot, left points to the
         * first slot after them -- that's why this sort is stable.
         * Slide elements over to make room to make room for pivot.
         */
        Int32 n = start - left;  // The number of elements to move
        // Switch is just an optimization for arraycopy in default case
        switch(n) {
            case 2:
                (const_cast<ArrayOf<IInterface*>* >(&a))->Set(left + 2, a[left + 1]);
            case 1:
                (const_cast<ArrayOf<IInterface*>* >(&a))->Set(left + 1, a[left]);
                break;
            default:
                (const_cast<ArrayOf<IInterface*>* >(&a))->Copy(left + 1, const_cast<ArrayOf<IInterface*>* >(&a), left, n);
        }
        const_cast<ArrayOf<IInterface*>* >(&a)->Set(left, pivot);
    }
}

Int32 ComparableTimSort::CountRunAndMakeAscending(
    /* [in] */ const ArrayOf<IInterface*>& a,
    /* [in] */ Int32 lo,
    /* [in] */ Int32 hi)
{
    if (DEBUG) {
        assert(lo < hi);
    }
    Int32 runHi = lo + 1;
    if (runHi == hi) {
        return 1;
    }

    // Find end of run, and reverse range if descending
    Int32 flagvalue = 0;
    if ((((IComparable*) a[runHi++]->Probe(EIID_IComparable))->CompareTo(a[lo], &flagvalue), flagvalue) < 0) { // Descending
        while(runHi < hi && (((IComparable*) a[runHi]->Probe(EIID_IComparable))->CompareTo(a[runHi - 1], &flagvalue), flagvalue) < 0) {
            runHi++;
        }
        ReverseRange(a, lo, runHi);
    }
    else {                              // Ascending
        while (runHi < hi && (((IComparable*) a[runHi]->Probe(EIID_IComparable))->CompareTo(a[runHi - 1], &flagvalue), flagvalue) >= 0) {
            runHi++;
        }
    }

    return runHi - lo;
}

void ComparableTimSort::ReverseRange(
    /* [in] */ const ArrayOf<IInterface*>& a,
    /* [in] */ Int32 lo,
    /* [in] */ Int32 hi)
{
    hi--;
    while (lo < hi) {
        AutoPtr<IInterface> t = a[lo];
        (const_cast<ArrayOf<IInterface*>* >(&a))->Set(lo++, a[hi]);
        (const_cast<ArrayOf<IInterface*>* >(&a))->Set(hi--, t);
    }
}

Int32 ComparableTimSort::MinRunLength(
    /* [in] */ Int32 n)
{
    if (DEBUG) {
        assert(n >= 0);
    }

    Int32 r = 0;      // Becomes 1 if any 1 bits are shifted off
    while (n >= MIN_MERGE) {
        r |= (n & 1);
        n >>= 1;
    }
    return n + r;
}

void ComparableTimSort::PushRun(
    /* [in] */ Int32 runBase,
    /* [in] */ Int32 runLen)
{
    (*mRunBase)[mStackSize] = runBase;
    (*mRunLen)[mStackSize] = runLen;
    mStackSize++;
}

void ComparableTimSort::MergeCollapse()
{
    while (mStackSize > 1) {
        Int32 n = mStackSize - 2;
        if (n > 0 && (*mRunLen)[n-1] <= (*mRunLen)[n] + (*mRunLen)[n+1]) {
            if ((*mRunLen)[n - 1] < (*mRunLen)[n + 1]) {
                n--;
            }
            MergeAt(n);
        }
        else if ((*mRunLen)[n] <= (*mRunLen)[n + 1]) {
            MergeAt(n);
        }
        else {
            break; // Invariant is established
        }
    }
}

void ComparableTimSort::MergeForceCollapse()
{
    while (mStackSize > 1) {
        Int32 n = mStackSize - 2;
        if (n > 0 && (*mRunLen)[n - 1] < (*mRunLen)[n + 1]) {
            n--;
        }
        MergeAt(n);
    }
}

void ComparableTimSort::MergeAt(
    /* [in] */ Int32 i)
{
    if (DEBUG) {
        assert(mStackSize >= 2);
    }
    if (DEBUG) {
        assert(i >= 0);
    }
    if (DEBUG) {
        assert(i == mStackSize - 2 || i == mStackSize - 3);
    }

    Int32 base1 = (*mRunBase)[i];
    Int32 len1 = (*mRunLen)[i];
    Int32 base2 = (*mRunBase)[i + 1];
    Int32 len2 = (*mRunLen)[i + 1];
    if (DEBUG) {
        assert(len1 > 0 && len2 > 0);
    }

    if (DEBUG) {
        assert(base1 + len1 == base2);
    }

    /*
     * Record the length of the combined runs; if i is the 3rd-last
     * run now, also slide over the last run (which isn't involved
     * in this merge).  The current run (i+1) goes away in any case.
     */
    (*mRunLen)[i] = len1 + len2;
    if (i == mStackSize - 3) {
        (*mRunBase)[i + 1] = (*mRunBase)[i + 2];
        (*mRunLen)[i + 1] = (*mRunLen)[i + 2];
    }
    mStackSize--;

    /*
     * Find where the first element of run2 goes in run1. Prior elements
     * in run1 can be ignored (because they're already in place).
     */
    Int32 k = GallopRight((IComparable*) (*mA)[base2]->Probe(EIID_IComparable), *mA, base1, len1, 0);
    if (DEBUG) {
        assert(k >= 0);
    }

    base1 += k;
    len1 -= k;
    if (len1 == 0) {
        return;
    }

    /*
     * Find where the last element of run1 goes in run2. Subsequent elements
     * in run2 can be ignored (because they're already in place).
     */
    len2 = GallopLeft((IComparable*) (*mA)[base1 + len1 - 1]->Probe(EIID_IComparable), *mA, base2, len2, len2 - 1);
    if (DEBUG) {
        assert(len2 >= 0);
    }

    if (len2 == 0) {
        return;
    }

    // Merge remaining runs, using tmp array with min(len1, len2) elements
    if (len1 <= len2) {
        MergeLo(base1, len1, base2, len2);
    }
    else {
        MergeHi(base1, len1, base2, len2);
    }
}

Int32 ComparableTimSort::GallopLeft(
    /* [in] */ IComparable* key,
    /* [in] */ const ArrayOf<IInterface*>& a,
    /* [in] */ Int32 base,
    /* [in] */ Int32 len,
    /* [in] */ Int32 hint)
{
    if (DEBUG) {
        assert(len > 0 && hint >= 0 && hint < len);
    }

    Int32 lastOfs = 0;
    Int32 ofs = 1;
    Int32 flagvalue = 0;
    if ((key->CompareTo(a[base + hint], &flagvalue), flagvalue) > 0) {
        // Gallop right until a[base+hint+lastOfs] < key <= a[base+hint+ofs]
        Int32 maxOfs = len - hint;
        while (ofs < maxOfs && (key->CompareTo(a[base + hint + ofs], &flagvalue), flagvalue) > 0) {
            lastOfs = ofs;
            ofs = (ofs << 1) + 1;
            if (ofs <= 0) {   // int overflow
                ofs = maxOfs;
            }
        }
        if (ofs > maxOfs) {
            ofs = maxOfs;
        }

        // Make offsets relative to base
        lastOfs += hint;
        ofs += hint;
    }
    else { // key <= a[base + hint]
        // Gallop left until a[base+hint-ofs] < key <= a[base+hint-lastOfs]
        Int32 maxOfs = hint + 1;
        while (ofs < maxOfs && (key->CompareTo(a[base + hint - ofs], &flagvalue), flagvalue) <= 0) {
            lastOfs = ofs;
            ofs = (ofs << 1) + 1;
            if (ofs <= 0) {   // int overflow
                ofs = maxOfs;
            }
        }
        if (ofs > maxOfs) {
            ofs = maxOfs;
        }

        // Make offsets relative to base
        Int32 tmp = lastOfs;
        lastOfs = hint - ofs;
        ofs = hint - tmp;
    }
    if (DEBUG) {
        assert(-1 <= lastOfs && lastOfs < ofs && ofs <= len);
    }

    /*
     * Now a[base+lastOfs] < key <= a[base+ofs], so key belongs somewhere
     * to the right of lastOfs but no farther right than ofs.  Do a binary
     * search, with invariant a[base + lastOfs - 1] < key <= a[base + ofs].
     */
    lastOfs++;
    while (lastOfs < ofs) {
        Int32 m = lastOfs + ((ofs - lastOfs) >> 1);

        if ((key->CompareTo(a[base + m], &flagvalue), flagvalue) > 0) {
            lastOfs = m + 1;  // a[base + m] < key
        }
        else {
            ofs = m;          // key <= a[base + m]
        }
    }
    if (DEBUG) {
        assert(lastOfs == ofs);    // so a[base + ofs - 1] < key <= a[base + ofs]
    }
    return ofs;
}

Int32 ComparableTimSort::GallopRight(
    /* [in] */ IComparable* key,
    /* [in] */ const ArrayOf<IInterface*>& a,
    /* [in] */ Int32 base,
    /* [in] */ Int32 len,
    /* [in] */ Int32 hint)
{
    if (DEBUG) {
        assert(len > 0 && hint >= 0 && hint < len);
    }

    Int32 ofs = 1;
    Int32 lastOfs = 0;
    Int32 flagvalue = 0;
    if ((key->CompareTo(a[base + hint], &flagvalue), flagvalue) < 0) {
        // Gallop left until a[b+hint - ofs] <= key < a[b+hint - lastOfs]
        Int32 maxOfs = hint + 1;
        while (ofs < maxOfs && (key->CompareTo(a[base + hint - ofs], &flagvalue), flagvalue) < 0) {
            lastOfs = ofs;
            ofs = (ofs << 1) + 1;
            if (ofs <= 0) {   // int overflow
                ofs = maxOfs;
            }
        }
        if (ofs > maxOfs) {
            ofs = maxOfs;
        }

        // Make offsets relative to b
        Int32 tmp = lastOfs;
        lastOfs = hint - ofs;
        ofs = hint - tmp;
    }
    else { // a[b + hint] <= key
        // Gallop right until a[b+hint + lastOfs] <= key < a[b+hint + ofs]
        Int32 maxOfs = len - hint;
        while (ofs < maxOfs && (key->CompareTo(a[base + hint + ofs], &flagvalue), flagvalue) >= 0) {
            lastOfs = ofs;
            ofs = (ofs << 1) + 1;
            if (ofs <= 0) {   // int overflow
                ofs = maxOfs;
            }
        }
        if (ofs > maxOfs) {
            ofs = maxOfs;
        }

        // Make offsets relative to b
        lastOfs += hint;
        ofs += hint;
    }
    if (DEBUG) {
        assert(-1 <= lastOfs && lastOfs < ofs && ofs <= len);
    }

    /*
     * Now a[b + lastOfs] <= key < a[b + ofs], so key belongs somewhere to
     * the right of lastOfs but no farther right than ofs.  Do a binary
     * search, with invariant a[b + lastOfs - 1] <= key < a[b + ofs].
     */
    lastOfs++;
    while (lastOfs < ofs) {
        Int32 m = lastOfs + ((ofs - lastOfs) >> 1);

        if ((key->CompareTo(a[base + m], &flagvalue), flagvalue) < 0) {
            ofs = m;          // key < a[b + m]
        }
        else {
            lastOfs = m + 1;  // a[b + m] <= key
        }
    }
    if (DEBUG) {
        assert(lastOfs == ofs);    // so a[b + ofs - 1] <= key < a[b + ofs]
    }
    return ofs;
}

void ComparableTimSort::MergeLo(
    /* [in] */ Int32 base1,
    /* [in] */ Int32 len1,
    /* [in] */ Int32 base2,
    /* [in] */ Int32 len2)
{
    if (DEBUG) {
        assert(len1 > 0 && len2 > 0 && base1 + len1 == base2);
    }

    // Copy first run into temp array
    AutoPtr<ArrayOf<IInterface*> > a = mA; // For performance
    AutoPtr<ArrayOf<IInterface*> > tmp = EnsureCapacity(len1);
    tmp->Copy(0, a, base1, len1);

    Int32 cursor1 = 0;       // Indexes into tmp array
    Int32 cursor2 = base2;   // Indexes int a
    Int32 dest = base1;      // Indexes int a

    // Move first element of second run and deal with degenerate cases
    (*a)[dest++] = (*a)[cursor2++];
    if (--len2 == 0) {
        a->Copy(dest, tmp, cursor1, len1);
        return;
    }
    if (len1 == 1) {
        a->Copy(dest, a, cursor2, len2);
        (*a)[dest + len2] = (*tmp)[cursor1]; // Last elt of run 1 to end of merge
        return;
    }

    Int32 minGallop = mMinGallop;  // Use local variable for performance
outer:
    while (true) {
        Int32 count1 = 0; // Number of times in a row that first run won
        Int32 count2 = 0; // Number of times in a row that second run won

        /*
         * Do the straightforward thing until (if ever) one run starts
         * winning consistently.
         */
        do {
            if (DEBUG) {
                assert(len1 > 1 && len2 > 0);
            }

            Int32 flagvalue = 0;
            if (((IComparable*) (*a)[cursor2]->Probe(EIID_IComparable))->CompareTo((*tmp)[cursor1], &flagvalue), flagvalue < 0) {
                (*a)[dest++] = (*a)[cursor2++];
                count2++;
                count1 = 0;
                if (--len2 == 0) {
                    goto outer;
                }
            }
            else {
                (*a)[dest++] = (*tmp)[cursor1++];
                count1++;
                count2 = 0;
                if (--len1 == 1) {
                    goto outer;
                }
            }
        } while ((count1 | count2) < minGallop);

        /*
         * One run is winning so consistently that galloping may be a
         * huge win. So try that, and continue galloping until (if ever)
         * neither run appears to be winning consistently anymore.
         */
        do {
            if (DEBUG) {
                assert(len1 > 1 && len2 > 0);
            }
            count1 = GallopRight((IComparable*) (*a)[cursor2]->Probe(EIID_IComparable), *tmp, cursor1, len1, 0);
            if (count1 != 0) {
                a->Copy(dest, tmp, cursor1, count1);
                dest += count1;
                cursor1 += count1;
                len1 -= count1;
                if (len1 <= 1) {  // len1 == 1 || len1 == 0
                    goto outer;
                }
            }
            (*a)[dest++] = (*a)[cursor2++];
            if (--len2 == 0) {
                goto outer;
            }

            count2 = GallopLeft((IComparable*) (*tmp)[cursor1]->Probe(EIID_IComparable), *a, cursor2, len2, 0);
            if (count2 != 0) {
                a->Copy(dest, a, cursor2, count2);
                dest += count2;
                cursor2 += count2;
                len2 -= count2;
                if (len2 == 0) {
                    goto outer;
                }
            }
            (*a)[dest++] = (*tmp)[cursor1++];
            if (--len1 == 1) {
                goto outer;
            }
            minGallop--;
        } while (count1 >= MIN_GALLOP || count2 >= MIN_GALLOP);
        if (minGallop < 0) {
            minGallop = 0;
        }
        minGallop += 2;  // Penalize for leaving gallop mode
    }  // End of "outer" loop
    mMinGallop = minGallop < 1 ? 1 : minGallop;  // Write back to field

    if (len1 == 1) {
        if (DEBUG) {
            assert(len2 > 0);
        }
        a->Copy(dest, a, cursor2, len2);
        (*a)[dest + len2] = (*tmp)[cursor1]; //  Last elt of run 1 to end of merge
    }
    else if (len1 == 0) {
        // throw new IllegalArgumentException("Comparison method violates its general contract!");
        return;
    }
    else {
        if (DEBUG) {
            assert(len2 == 0);
        }
        if (DEBUG) {
            assert(len1 > 1);
        }
        a->Copy(dest, tmp, cursor1, len1);
    }
}

void ComparableTimSort::MergeHi(
    /* [in] */ Int32 base1,
    /* [in] */ Int32 len1,
    /* [in] */ Int32 base2,
    /* [in] */ Int32 len2)
{
    if (DEBUG) {
        assert(len1 > 0 && len2 > 0 && base1 + len1 == base2);
    }

    // Copy second run into temp array
    AutoPtr<ArrayOf<IInterface*> > a = mA; // For performance
    AutoPtr<ArrayOf<IInterface*> > tmp = EnsureCapacity(len2);
    tmp->Copy(0, a, base2, len2);

    Int32 cursor1 = base1 + len1 - 1;  // Indexes into a
    Int32 cursor2 = len2 - 1;          // Indexes into tmp array
    Int32 dest = base2 + len2 - 1;     // Indexes into a

    // Move last element of first run and deal with degenerate cases
    (*a)[dest--] = (*a)[cursor1--];
    if (--len1 == 0) {
        a->Copy(dest - (len2 - 1), tmp, 0, len2);
        return;
    }
    if (len2 == 1) {
        dest -= len1;
        cursor1 -= len1;
        a->Copy(dest + 1, a, cursor1 + 1, len1);
        (*a)[dest] = (*tmp)[cursor2];
        return;
    }

    Int32 minGallop = mMinGallop;  // Use local variable for performance
outer:
    while (true) {
        Int32 count1 = 0; // Number of times in a row that first run won
        Int32 count2 = 0; // Number of times in a row that second run won

        /*
         * Do the straightforward thing until (if ever) one run
         * appears to win consistently.
         */
        do {
            if (DEBUG) {
                assert(len1 > 0 && len2 > 1);
            }
            Int32 flagvalue = 0;
            if (((IComparable*) (*tmp)[cursor2]->Probe(EIID_IComparable))->CompareTo((*a)[cursor1], &flagvalue), flagvalue < 0) {
                (*a)[dest--] = (*a)[cursor1--];
                count1++;
                count2 = 0;
                if (--len1 == 0) {
                    goto outer;
                }
            }
            else {
                (*a)[dest--] = (*tmp)[cursor2--];
                count2++;
                count1 = 0;
                if (--len2 == 1) {
                    goto outer;
                }
            }
        } while ((count1 | count2) < minGallop);

        /*
         * One run is winning so consistently that galloping may be a
         * huge win. So try that, and continue galloping until (if ever)
         * neither run appears to be winning consistently anymore.
         */
        do {
            if (DEBUG) {
                assert(len1 > 0 && len2 > 1);
            }
            count1 = len1 - GallopRight((IComparable*) (*tmp)[cursor2]->Probe(EIID_IComparable), *a, base1, len1, len1 - 1);
            if (count1 != 0) {
                dest -= count1;
                cursor1 -= count1;
                len1 -= count1;
                a->Copy(dest + 1, a, cursor1 + 1, count1);
                if (len1 == 0) {
                    goto outer;
                }
            }
            (*a)[dest--] = (*tmp)[cursor2--];
            if (--len2 == 1) {
                goto outer;
            }

            count2 = len2 - GallopLeft((IComparable*) (*a)[cursor1]->Probe(EIID_IComparable), *tmp, 0, len2, len2 - 1);
            if (count2 != 0) {
                dest -= count2;
                cursor2 -= count2;
                len2 -= count2;
                a->Copy(dest + 1, tmp, cursor2 + 1, count2);
                if (len2 <= 1) {
                    goto outer; // len2 == 1 || len2 == 0
                }
            }
            (*a)[dest--] = (*a)[cursor1--];
            if (--len1 == 0) {
                goto outer;
            }
            minGallop--;
        } while (count1 >= MIN_GALLOP || count2 >= MIN_GALLOP);
        if (minGallop < 0) {
            minGallop = 0;
        }
        minGallop += 2;  // Penalize for leaving gallop mode
    }  // End of "outer" loop
    mMinGallop = minGallop < 1 ? 1 : minGallop;  // Write back to field

    if (len2 == 1) {
        if (DEBUG) {
            assert(len1 > 0);
        }
        dest -= len1;
        cursor1 -= len1;
        a->Copy(dest + 1, a, cursor1 + 1, len1);
        (*a)[dest] = (*tmp)[cursor2];  // Move first elt of run2 to front of merge
    }
    else if (len2 == 0) {
        // throw new IllegalArgumentException("Comparison method violates its general contract!");
        return;
    }
    else {
        if (DEBUG) {
            assert(len1 == 0);
        }
        if (DEBUG) {
            assert(len2 > 0);
        }
        a->Copy(dest - (len2 - 1), tmp, 0, len2);
    }
}

AutoPtr<ArrayOf<IInterface*> > ComparableTimSort::EnsureCapacity(
    /* [in] */ Int32 minCapacity)
{
    if (mTmp->GetLength() < minCapacity) {
        // Compute smallest power of 2 > minCapacity
        Int32 newSize = minCapacity;
        newSize |= newSize >> 1;
        newSize |= newSize >> 2;
        newSize |= newSize >> 4;
        newSize |= newSize >> 8;
        newSize |= newSize >> 16;
        newSize++;

        if (newSize < 0) { // Not bloody likely!
            newSize = minCapacity;
        }
        else {
            newSize = Elastos::Core::Math::Min(newSize, mA->GetLength() >> 1);
        }

        // @SuppressWarnings({"unchecked", "UnnecessaryLocalVariable"})
        AutoPtr<ArrayOf<IInterface*> > newArray = ArrayOf<IInterface*>::Alloc(newSize);
        mTmp = NULL;
        mTmp = newArray;
    }
    return mTmp;
}

} // namespace Utility
} // namespace Elastos
