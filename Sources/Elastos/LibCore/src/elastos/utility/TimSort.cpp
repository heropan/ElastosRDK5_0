
#include "TimSort.h"
#include "CArrays.h"
#include "Math.h"

namespace Elastos {
namespace Utility {

const Int32 TimSort::MIN_MERGE;

const Int32 TimSort::MIN_GALLOP;

const Int32 TimSort::INITIAL_TMP_STORAGE_LENGTH;

const Boolean TimSort::DEBUG;


TimSort::TimSort(
    /* [in] */ ArrayOf<IInterface*>* a,
    /* [in] */ IComparator* c)
{
    mA = a;
    mC = c;

    // Allocate temp storage (which may be increased later if necessary)
    Int32 len = mA->GetLength();
    // @SuppressWarnings({"unchecked", "UnnecessaryLocalVariable"})
    AutoPtr< ArrayOf<IInterface*> > newArray = ArrayOf<IInterface*>::Alloc(len < 2 * INITIAL_TMP_STORAGE_LENGTH ?
                                    (UInt32)len >> 1 : INITIAL_TMP_STORAGE_LENGTH);
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

ECode TimSort::Sort(
    /* [in] */ ArrayOf<IInterface*>* a,
    /* [in] */ IComparator* c)
{
    return Sort(a, 0, a->GetLength(), c);
}

ECode TimSort::Sort(
    /* [in] */ ArrayOf<IInterface*>* a,
    /* [in] */ Int32 lo,
    /* [in] */ Int32 hi,
    /* [in] */ IComparator* c)
{
    if (c == NULL) {
        return CArrays::_SortObject(a, lo, hi);
    }

    VALIDATE_NOT_NULL(CArrays::_CheckStartAndEnd(a->GetLength(), lo, hi));
    Int32 nRemaining  = hi - lo;
    if (nRemaining < 2)
        return NOERROR;  // Arrays of size 0 and 1 are always sorted

    // If array is small, do a "mini-TimSort" with no merges
    if (nRemaining < MIN_MERGE) {
        Int32 initRunLen = CountRunAndMakeAscending(a, lo, hi, c);
        BinarySort(a, lo, hi, lo + initRunLen, c);
        return NOERROR;
    }

    /**
     * March over the array once, left to right, finding natural runs,
     * extending short natural runs to minRun elements, and merging runs
     * to maintain stack invariant.
     */
    TimSort* ts = new TimSort(a, c);
    Int32 minRun = MinRunLength(nRemaining);
    do {
        // Identify next run
        Int32 runLen = CountRunAndMakeAscending(a, lo, hi, c);

        // If run is short, extend to min(minRun, nRemaining)
        if (runLen < minRun) {
            Int32 force = nRemaining <= minRun ? nRemaining : minRun;
            BinarySort(a, lo, lo + force, lo + runLen, c);
            runLen = force;
        }

        // Push run onto pending-run stack, and maybe merge
        ts->PushRun(lo, runLen);
        ts->MergeCollapse();

        // Advance to find next run
        lo += runLen;
        nRemaining -= runLen;
    } while (nRemaining != 0);

    // Merge all remaining runs to complete sort
    if (DEBUG) {
        assert(lo == hi);
    }
    ts->MergeForceCollapse();
    if (DEBUG) {
        assert(ts->mStackSize == 1);
    }
    if (ts) {
        delete ts;
    }

    return NOERROR;
}

ECode TimSort::BinarySort(
    /* [in] */ ArrayOf<IInterface*>* a,
    /* [in] */ Int32 lo,
    /* [in] */ Int32 hi,
    /* [in] */ Int32 start,
    /* [in] */ IComparator* c)
{
    if (DEBUG) {
        assert(lo <= start && start <= hi);
    }
    if (start == lo) {
        start++;
    }
    for ( ; start < hi; start++) {
        AutoPtr<IInterface> pivot = (*a)[start];

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
            Int32 mid = (UInt32)(left + right) >> 1;
            Int32 isflag = 0;
            if (c->Compare(pivot, (*a)[mid], &isflag), isflag < 0) {
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
         * Slide elements over to make room for pivot.
         */
        Int32 n = start - left;  // The number of elements to move
        // Switch is just an optimization for arraycopy in default case
        switch(n) {
            case 2:
                a->Set(left + 2, (*a)[left + 1]);
            case 1:
                a->Set(left + 1, (*a)[left]);
                break;
            default:
                a->Copy(left +1, a, left, n);
        }
        a->Set(left, pivot);
    }
    return NOERROR;
}

Int32 TimSort::CountRunAndMakeAscending(
    /* [in] */ ArrayOf<IInterface*>* a,
    /* [in] */ Int32 lo,
    /* [in] */ Int32 hi,
    /* [in] */ IComparator* c)
{
    if (DEBUG) {
        assert(lo < hi);
    }
    Int32 runHi = lo + 1;
    if (runHi == hi)
        return 1;

    // Find end of run, and reverse range if descending
    Int32 isflag = 0;
    if (c->Compare((*a)[runHi++], (*a)[lo], &isflag), isflag < 0) { // Descending
        while(runHi < hi && (c->Compare((*a)[runHi], (*a)[runHi - 1], &isflag), isflag < 0))
            runHi++;
        ReverseRange(a, lo, runHi);
    }
    else {                              // Ascending
        while (runHi < hi && (c->Compare((*a)[runHi], (*a)[runHi - 1], &isflag), isflag >= 0))
            runHi++;
    }

    return runHi - lo;
}

ECode TimSort::ReverseRange(
    /* [in] */ ArrayOf<IInterface*>* a,
    /* [in] */ Int32 lo,
    /* [in] */ Int32 hi)
{
    hi--;
    while (lo < hi) {
        AutoPtr<IInterface> t = (*a)[lo];
        a->Set(lo++, (*a)[hi]);
        a->Set(hi--, t);
    }
    return NOERROR;
}

Int32 TimSort::MinRunLength(
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

ECode TimSort::PushRun(
    /* [in] */ Int32 runBase,
    /* [in] */ Int32 runLen)
{
    (*mRunBase)[mStackSize] = runBase;
    (*mRunLen)[mStackSize] = runLen;
    mStackSize++;
    return NOERROR;
}

ECode TimSort::MergeCollapse()
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
    return NOERROR;
}

ECode TimSort::MergeForceCollapse()
{
    while (mStackSize > 1) {
        Int32 n = mStackSize - 2;
        if (n > 0 && (*mRunLen)[n - 1] < (*mRunLen)[n + 1]) {
            n--;
        }
        MergeAt(n);
    }
    return NOERROR;
}

ECode TimSort::MergeAt(
    /* [in] */ Int32 i)
{
    if (DEBUG) {
        assert(mStackSize >= 2);
    }
    if (DEBUG) {
        assert(i >= 0);
    }
    if (DEBUG) {
        assert(i == (mStackSize - 2) || (i == mStackSize) - 3);
    }

    Int32 base1 = (*mRunBase)[i];
    Int32 len1 = (*mRunLen)[i];
    Int32 base2 = (*mRunBase)[i + 1];
    Int32 len2 = (*mRunLen)[i + 1];
    if (DEBUG) {
        assert(len1 > 0 && len2 > 0);
    }
    if (DEBUG) {
        assert((base1 + len1) == base2);
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
    Int32 k = GallopRight((*mA)[base2], mA, base1, len1, 0, mC);
    if (DEBUG) {
        assert(k >= 0);
    }
    base1 += k;
    len1 -= k;
    if (len1 == 0) {
        return NOERROR;
    }

    /*
     * Find where the last element of run1 goes in run2. Subsequent elements
     * in run2 can be ignored (because they're already in place).
     */
    len2 = GallopLeft((*mA)[base1 + len1 - 1], mA, base2, len2, len2 - 1, mC);
    if (DEBUG) {
        assert(len2 >= 0);
    }
    if (len2 == 0) {
        return NOERROR;
    }

    // Merge remaining runs, using tmp array with min(len1, len2) elements
    if (len1 <= len2) {
        MergeLo(base1, len1, base2, len2);
    }
    else {
        MergeHi(base1, len1, base2, len2);
    }
    return NOERROR;
}

Int32 TimSort::GallopLeft(
    /* [in] */ IInterface* key,
    /* [in] */ ArrayOf<IInterface*>* a,
    /* [in] */ Int32 base,
    /* [in] */ Int32 len,
    /* [in] */ Int32 hint,
    /* [in] */ IComparator* c)
{
    if (DEBUG) {
        assert(len > 0 && hint >= 0 && hint < len);
    }
    Int32 lastOfs = 0;
    Int32 ofs = 1;
    Int32 isflag = 0;
    if (c->Compare(key, (*a)[base + hint], &isflag), isflag > 0) {
        // Gallop right until a[base+hint+lastOfs] < key <= a[base+hint+ofs]
        Int32 maxOfs = len - hint;
        while (ofs < maxOfs && (c->Compare(key, (*a)[base + hint + ofs], &isflag), isflag > 0)) {
            lastOfs = ofs;
            ofs = (ofs * 2) + 1;
            if (ofs <= 0) {  // int overflow
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
        const Int32 maxOfs = hint + 1;
        while (ofs < maxOfs && (c->Compare(key, (*a)[base + hint - ofs], &isflag), isflag <= 0)) {
            lastOfs = ofs;
            ofs = (ofs * 2) + 1;
            if (ofs <= 0) {  // int overflow
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
        Int32 m = lastOfs + (UInt32(ofs - lastOfs) >> 1);

        if (c->Compare(key, (*a)[base + m], &isflag), isflag > 0) {
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

Int32 TimSort::GallopRight(
    /* [in] */ IInterface* key,
    /* [in] */ ArrayOf<IInterface*>* a,
    /* [in] */ Int32 base,
    /* [in] */ Int32 len,
    /* [in] */ Int32 hint,
    /* [in] */ IComparator* c)
{
    if (DEBUG) {
        assert(len > 0 && hint >= 0 && hint < len);
    }

    Int32 ofs = 1;
    Int32 lastOfs = 0;
    Int32 isflag = 0;
    if (c->Compare(key, (*a)[base + hint], &isflag), isflag < 0) {
        // Gallop left until a[b+hint - ofs] <= key < a[b+hint - lastOfs]
        Int32 maxOfs = hint + 1;
        while (ofs < maxOfs && (c->Compare(key, (*a)[base + hint - ofs], &isflag), isflag < 0)) {
            lastOfs = ofs;
            ofs = (ofs * 2) + 1;
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
        while (ofs < maxOfs && (c->Compare(key, (*a)[base + hint + ofs], &isflag), isflag >= 0)) {
            lastOfs = ofs;
            ofs = (ofs * 2) + 1;
            if (ofs <= 0) {  // int overflow
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
        Int32 m = lastOfs + (UInt32(ofs - lastOfs) >> 1);

        if (c->Compare(key, (*a)[base + m], &isflag), isflag < 0) {
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

ECode TimSort::MergeLo(
    /* [in] */ Int32 base1,
    /* [in] */ Int32 len1,
    /* [in] */ Int32 base2,
    /* [in] */ Int32 len2)
{
    if (DEBUG) {
        assert(len1 > 0 && len2 > 0 && (base1 + len1) == base2);
    }

    // Copy first run into temp array
    AutoPtr< ArrayOf<IInterface*> > a = mA; // For performance
    AutoPtr< ArrayOf<IInterface*> > tmp = EnsureCapacity(len1);
    tmp->Copy(0, a, base1, len1);

    Int32 cursor1 = 0;       // Indexes into tmp array
    Int32 cursor2 = base2;   // Indexes int a
    Int32 dest = base1;      // Indexes int a

    // Move first element of second run and deal with degenerate cases
    a->Set(dest++, (*a)[cursor2++]);
    if (--len2 == 0) {
        a->Copy(dest, tmp, cursor1, len1);
        return NOERROR;
    }
    if (len1 == 1) {
        a->Copy(dest, a, cursor2, len2);
        a->Set(dest + len2, (*tmp)[cursor1]); // Last elt of run 1 to end of merge
        return NOERROR;
    }

    AutoPtr<IComparator> c = mC;  // Use local variable for performance
    Int32 minGallop = mMinGallop;    //  "    "       "     "      "
outer:
    while (TRUE) {
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
            Int32 isflag = 0;
            if (c->Compare((*a)[cursor2], (*tmp)[cursor1], &isflag), isflag < 0) {
                a->Set(dest++, (*a)[cursor2++]);
                count2++;
                count1 = 0;
                if (--len2 == 0)
                    goto outer;
            }
            else {
                a->Set(dest++, (*tmp)[cursor1++]);
                count1++;
                count2 = 0;
                if (--len1 == 1)
                    goto outer;
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
            count1 = GallopRight((*a)[cursor2], tmp, cursor1, len1, 0, c);
            if (count1 != 0) {
                a->Copy(dest, tmp, cursor1,count1);
                dest += count1;
                cursor1 += count1;
                len1 -= count1;
                if (len1 <= 1) // len1 == 1 || len1 == 0
                    goto outer;
            }
            a->Set(dest++, (*a)[cursor2++]);
            if (--len2 == 0)
                goto outer;

            count2 = GallopLeft((*tmp)[cursor1], a, cursor2, len2, 0, c);
            if (count2 != 0) {
                a->Copy(dest, a, cursor2, count2);
                dest += count2;
                cursor2 += count2;
                len2 -= count2;
                if (len2 == 0)
                    goto outer;
            }
            a->Set(dest++, (*tmp)[cursor1++]);
            if (--len1 == 1) {
                goto outer;
            }
            minGallop--;
        } while (count1 >= MIN_GALLOP || count2 >= MIN_GALLOP);
        if (minGallop < 0)
            minGallop = 0;
        minGallop += 2;  // Penalize for leaving gallop mode
    }  // End of "outer" loop
    mMinGallop = minGallop < 1 ? 1 : minGallop;  // Write back to field

    if (len1 == 1) {
        if (DEBUG) {
            assert(len2 > 0);
        }
        a->Copy(dest, a, cursor2, len2);
        a->Set(dest + len2, (*tmp)[cursor1]); //  Last elt of run 1 to end of merge
    }
    else if (len1 == 0) {
        // throw new IllegalArgumentException(
        //     "Comparison method violates its general contract!");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
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
    return NOERROR;
}

ECode TimSort::MergeHi(
    /* [in] */ Int32 base1,
    /* [in] */ Int32 len1,
    /* [in] */ Int32 base2,
    /* [in] */ Int32 len2)
{
    if (DEBUG) {
        assert(len1 > 0 && len2 > 0 && (base1 + len1) == base2);
    }

    // Copy second run into temp array
    AutoPtr< ArrayOf<IInterface*> > a = mA; // For performance
    AutoPtr< ArrayOf<IInterface*> > tmp = EnsureCapacity(len2);
    tmp->Copy(0, a, base2, len2);

    Int32 cursor1 = base1 + len1 - 1;  // Indexes into a
    Int32 cursor2 = len2 - 1;          // Indexes into tmp array
    Int32 dest = base2 + len2 - 1;     // Indexes into a

    // Move last element of first run and deal with degenerate cases
    a->Set(dest--, (*a)[cursor1--]);
    if (--len1 == 0) {
        a->Copy(dest - (len2 - 1), tmp, 0, len2);
        return NOERROR;
    }
    if (len2 == 1) {
        dest -= len1;
        cursor1 -= len1;
        a->Copy(dest + 1, a, cursor1 + 1, len1);
        a->Set(dest, (*tmp)[cursor2]);
        return NOERROR;
    }

    AutoPtr<IComparator> c = mC;  // Use local variable for performance
    Int32 minGallop = mMinGallop;    //  "    "       "     "      "
outer:
    while (TRUE) {
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
            Int32 isflag = 0;
            if (c->Compare((*tmp)[cursor2], (*a)[cursor1], &isflag), isflag < 0) {
                a->Set(dest--, (*a)[cursor1--]);
                count1++;
                count2 = 0;
                if (--len1 == 0)
                    goto outer;
            }
            else {
                a->Set(dest--, (*tmp)[cursor2--]);
                count2++;
                count1 = 0;
                if (--len2 == 1)
                    goto outer;
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
            count1 = len1 - GallopRight((*tmp)[cursor2], a, base1, len1, len1 - 1, c);
            if (count1 != 0) {
                dest -= count1;
                cursor1 -= count1;
                len1 -= count1;
                a->Copy(dest + 1, a, cursor1 + 1, count1);
                if (len1 == 0)
                    goto outer;
            }
            a->Set(dest--, (*tmp)[cursor2--]);
            if (--len2 == 1)
                goto outer;

            count2 = len2 - GallopLeft((*a)[cursor1], tmp, 0, len2, len2 - 1, c);
            if (count2 != 0) {
                dest -= count2;
                cursor2 -= count2;
                len2 -= count2;
                a->Copy(dest + 1, tmp, cursor2 + 1, count2);
                if (len2 <= 1)  // len2 == 1 || len2 == 0
                    goto outer;
            }
            a->Set(dest--, (*a)[cursor1--]);
            if (--len1 == 0)
                goto outer;
            minGallop--;
        } while (count1 >= MIN_GALLOP || count2 >= MIN_GALLOP);
        if (minGallop < 0)
            minGallop = 0;
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
        a->Set(dest, (*tmp)[cursor2]);  // Move first elt of run2 to front of merge
    }
    else if (len2 == 0) {
        // throw new IllegalArgumentException(
        //     "Comparison method violates its general contract!");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
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
    return NOERROR;
}

AutoPtr< ArrayOf<IInterface*> > TimSort::EnsureCapacity(
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

        if (newSize < 0) {// Not bloody likely!
            newSize = minCapacity;
        }
        else {
            newSize = Elastos::Core::Math::Min(newSize, (UInt32)mA->GetLength() >> 1);
        }

        // @SuppressWarnings({"unchecked", "UnnecessaryLocalVariable"})
        AutoPtr< ArrayOf<IInterface*> > newArray = ArrayOf<IInterface*>::Alloc(newSize);
        mTmp = newArray;
    }
    return mTmp;
}

} // namespace Utility
} // namespace Elastos
