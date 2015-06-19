
#ifndef __ELASTOS_UTILITY_CEXCHANGER_H__
#define __ELASTOS_UTILITY_CEXCHANGER_H__

#include "_Elastos_Utility_Concurrent_CExchanger.h"
#include "AtomicReference.h"
#include "Object.h"

using Elastos::Utility::Concurrent::Atomic::IAtomicReference;
using Elastos::Utility::Concurrent::Atomic::AtomicReference;
using Elastos::Utility::Concurrent::Atomic::IAtomicInteger32;

namespace Elastos {
namespace Utility {
namespace Concurrent {

CarClass(CExchanger)
    , public IExchanger
    , public Object
{
private:
    class CDummyObject
        : public Object
    {
    };

    /**
     * Nodes hold partially exchanged data.  This class
     * opportunistically subclasses AtomicReference to represent the
     * hole.  So get() returns hole, and compareAndSet CAS'es value
     * into hole.  This class cannot be parameterized as "V" because
     * of the use of non-V CANCEL sentinels.
     */
    class Node
        : public AtomicReference
    {
    public:
        /**
         * Creates node with given item and empty hole.
         * @param item the item
         */
        Node(
            /* [in] */ IInterface* item);

        CARAPI Get(
            /* [out] */ IInterface** outface);

        CARAPI Set(
            /* [in] */ IInterface* newValue);

        CARAPI LazySet(
            /* [in] */ IInterface* newValue);

        CARAPI CompareAndSet(
            /* [in] */ IInterface* expect,
            /* [in] */ IInterface* update,
            /* [out] */ Boolean* value);

        CARAPI WeakCompareAndSet(
            /* [in] */ IInterface* expect,
            /* [in] */ IInterface* update,
            /* [out] */ Boolean* value);

        CARAPI GetAndSet(
            /* [in] */ IInterface* newValue,
            /* [out] */ IInterface** outface);

        CARAPI ToString(
            /* [out] */ String* str);

    public:
        /** The element offered by the Thread creating this node. */
        AutoPtr<IInterface> mItem;

        /** The Thread waiting to be signalled; null until waiting. */
        volatile AutoPtr<IThread> mWaiter;
    };

    /**
     * A Slot is an AtomicReference with heuristic padding to lessen
     * cache effects of this heavily CAS'ed location.  While the
     * padding adds noticeable space, all slots are created only on
     * demand, and there will be more than one of them only when it
     * would improve throughput more than enough to outweigh using
     * extra space.
     */
    class Slot
        : public AtomicReference
    {
    public:
        Slot();

        CARAPI Get(
            /* [out] */ IInterface** outface);

        CARAPI Set(
            /* [in] */ IInterface* newValue);

        CARAPI LazySet(
            /* [in] */ IInterface* newValue);

        CARAPI CompareAndSet(
            /* [in] */ IInterface* expect,
            /* [in] */ IInterface* update,
            /* [out] */ Boolean* value);

        CARAPI WeakCompareAndSet(
            /* [in] */ IInterface* expect,
            /* [in] */ IInterface* update,
            /* [out] */ Boolean* value);

        CARAPI GetAndSet(
            /* [in] */ IInterface* newValue,
            /* [out] */ IInterface** outface);

        CARAPI ToString(
            /* [out] */ String* str);

    private:
        // Improve likelihood of isolation on <= 128 byte cache lines.
        // We used to target 64 byte cache lines, but some x86s (including
        // i7 under some BIOSes) actually use 128 byte cache lines.
        Int64 mQ0, mQ1, mQ2, mQ3, mQ4, mQ5, mQ6, mQ7, mQ8, mQ9, mQa, mQb, mQc, mQd, mQe;
    };

public:
    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

    /**
     * Creates a new Exchanger.
     */
    CARAPI constructor();

    CARAPI Exchange(
        /* [in] */ IInterface* x,
        /* [out] */ IInterface** outface);

    CARAPI Exchange(
        /* [in] */ IInterface* x,
        /* [in] */ Int64 timeout,
        /* [in] */ ITimeUnit* unit,
        /* [out] */ IInterface** outface);

private:
    /**
     * Main exchange function, handling the different policy variants.
     * Uses Object, not "V" as argument and return value to simplify
     * handling of sentinel values.  Callers from public methods decode
     * and cast accordingly.
     *
     * @param item the (non-null) item to exchange
     * @param timed true if the wait is timed
     * @param nanos if timed, the maximum wait time
     * @return the other thread's item, or CANCEL if interrupted or timed out
     */
    CARAPI_(AutoPtr<IInterface>) DoExchange(
        /* [in] */ IInterface* item,
        /* [in] */ Boolean timed,
        /* [in] */ Int64 nanos);

    /**
     * Returns a hash index for the current thread.  Uses a one-step
     * FNV-1a hash code (http://www.isthe.com/chongo/tech/comp/fnv/)
     * based on the current thread's Thread.getId().  These hash codes
     * have more uniform distribution properties with respect to small
     * moduli (here 1-31) than do other simple hashing functions.
     *
     * <p>To return an index between 0 and max, we use a cheap
     * approximation to a mod operation, that also corrects for bias
     * due to non-power-of-2 remaindering (see {@link
     * java.util.Random#nextInt}).  Bits of the hashcode are masked
     * with "nbits", the ceiling power of two of table size (looked up
     * in a table packed into three ints).  If too large, this is
     * retried after rotating the hash by nbits bits, while forcing new
     * top bit to 0, which guarantees eventual termination (although
     * with a non-random-bias).  This requires an average of less than
     * 2 tries for all table sizes, and has a maximum 2% difference
     * from perfectly uniform slot probabilities when applied to all
     * possible hash codes for sizes less than 32.
     *
     * @return a per-thread-random index, 0 <= index < max
     */
    CARAPI_(Int32) HashIndex();

    /**
     * Creates a new slot at given index.  Called only when the slot
     * appears to be null.  Relies on double-check using builtin
     * locks, since they rarely contend.  This in turn relies on the
     * arena array being declared volatile.
     *
     * @param index the index to add slot at
     */
    CARAPI_(void) CreateSlot(
        /* [in] */ Int32 index);

    /**
     * Tries to cancel a wait for the given node waiting in the given
     * slot, if so, helping clear the node from its slot to avoid
     * garbage retention.
     *
     * @param node the waiting node
     * @param slot the slot it is waiting in
     * @return true if successfully cancelled
     */
    static CARAPI_(Boolean) TryCancel(
        /* [in] */ Node* node,
        /* [in] */ Slot* slot);

    // Three forms of waiting. Each just different enough not to merge
    // code with others.

    /**
     * Spin-waits for hole for a non-0 slot.  Fails if spin elapses
     * before hole filled.  Does not check interrupt, relying on check
     * in public exchange method to abort if interrupted on entry.
     *
     * @param node the waiting node
     * @return on success, the hole; on failure, CANCEL
     */
    static CARAPI_(AutoPtr<IInterface>) SpinWait(
        /* [in] */ Node* node,
        /* [in] */ Slot* slot);

    /**
     * Waits for (by spinning and/or blocking) and gets the hole
     * filled in by another thread.  Fails if interrupted before
     * hole filled.
     *
     * When a node/thread is about to block, it sets its waiter field
     * and then rechecks state at least one more time before actually
     * parking, thus covering race vs fulfiller noticing that waiter
     * is non-null so should be woken.
     *
     * Thread interruption status is checked only surrounding calls to
     * park.  The caller is assumed to have checked interrupt status
     * on entry.
     *
     * @param node the waiting node
     * @return on success, the hole; on failure, CANCEL
     */
    static CARAPI_(AutoPtr<IInterface>) Await(
        /* [in] */ Node* node,
        /* [in] */ Slot* slot);

    /**
     * Waits for (at index 0) and gets the hole filled in by another
     * thread.  Fails if timed out or interrupted before hole filled.
     * Same basic logic as untimed version, but a bit messier.
     *
     * @param node the waiting node
     * @param nanos the wait time
     * @return on success, the hole; on failure, CANCEL
     */
    CARAPI_(AutoPtr<IInterface>) AwaitNanos(
        /* [in] */ Node* node,
        /* [in] */ Slot* slot,
        /* [in] */ Int64 nanos);

    /**
     * Sweeps through arena checking for any waiting threads.  Called
     * only upon return from timeout while waiting in slot 0.  When a
     * thread gives up on a timed wait, it is possible that a
     * previously-entered thread is still waiting in some other
     * slot.  So we scan to check for any.  This is almost always
     * overkill, but decreases the likelihood of timeouts when there
     * are other threads present to far less than that in lock-based
     * exchangers in which earlier-arriving threads may still be
     * waiting on entry locks.
     *
     * @param node the waiting node
     * @return another thread's item, or CANCEL
     */
    CARAPI_(AutoPtr<IInterface>) ScanOnTimeout(
        /* [in] */ Node* node);

private:
    /** The number of CPUs, for sizing and spin control */
    static const Int32 sNCPU;

    /**
     * The capacity of the arena.  Set to a value that provides more
     * than enough space to handle contention.  On small machines
     * most slots won't be used, but it is still not wasted because
     * the extra space provides some machine-level address padding
     * to minimize interference with heavily CAS'ed Slot locations.
     * And on very large machines, performance eventually becomes
     * bounded by memory bandwidth, not numbers of threads/CPUs.
     * This constant cannot be changed without also modifying
     * indexing and hashing algorithms.
     */
    static const Int32 sCAPACITY;

    /**
     * The value of "max" that will hold all threads without
     * contention.  When this value is less than CAPACITY, some
     * otherwise wasted expansion can be avoided.
     */
    static Int32 sFULL;

    /**
     * The number of times to spin (doing nothing except polling a
     * memory location) before blocking or giving up while waiting to
     * be fulfilled.  Should be zero on uniprocessors.  On
     * multiprocessors, this value should be large enough so that two
     * threads exchanging items as fast as possible block only when
     * one of them is stalled (due to GC or preemption), but not much
     * longer, to avoid wasting CPU resources.  Seen differently, this
     * value is a little over half the number of cycles of an average
     * context switch time on most systems.  The value here is
     * approximately the average of those across a range of tested
     * systems.
     */
    static Int32 sSPINS;

    /**
     * The number of times to spin before blocking in timed waits.
     * Timed waits spin more slowly because checking the time takes
     * time.  The best value relies mainly on the relative rate of
     * System.nanoTime vs memory accesses.  The value is empirically
     * derived to work well across a variety of systems.
     */
    static Int32 sTIMED_SPINS;

    /**
     * Sentinel item representing cancellation of a wait due to
     * interruption, timeout, or elapsed spin-waits.  This value is
     * placed in holes on cancellation, and used as a return value
     * from waiting methods to indicate failure to set or get hole.
     */
    static AutoPtr<IInterface> sCANCEL;

    /**
     * Value representing null arguments/returns from public
     * methods.  This disambiguates from internal requirement that
     * holes start out as null to mean they are not yet set.
     */
    static AutoPtr<IInterface> sNULL_ITEM;

    /**
     * Slot array.  Elements are lazily initialized when needed.
     * Declared volatile to enable double-checked lazy construction.
     */
    volatile AutoPtr<ArrayOf<Slot*> > mArena;

    /**
     * The maximum slot index being used.  The value sometimes
     * increases when a thread experiences too many CAS contentions,
     * and sometimes decreases when a spin-wait elapses.  Changes
     * are performed only via compareAndSet, to avoid stale values
     * when a thread happens to stall right before setting.
     */
    AutoPtr<IAtomicInteger32> mMax;
};

} // namespace Concurrent
} // namespace Utility
} // namespace Elastos

#endif //__ELASTOS_UTILITY_CEXCHANGER_H__