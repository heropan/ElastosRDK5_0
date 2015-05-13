
#ifndef __CFORKJOINWORKERTHREAD_H__
#define __CFORKJOINWORKERTHREAD_H__

#include "_CForkJoinWorkerThread.h"
#include "Thread.h"

using Elastos::Core::IThrowable;
using Elastos::Core::IClassLoader;
using Elastos::Core::IRunnable;

namespace Elastos {
namespace Utility {
namespace Concurrent {

CarClass(CForkJoinWorkerThread), Thread
{
public:
    CARAPI GetPool(
        /* [out] */ IForkJoinPool** outpool);

    CARAPI GetPoolIndex(
        /* [out] */ Int32* value);

    CARAPI Run();

    // queue methods

    CARAPI PushTask(
        /* [in] */ IForkJoinTask* t);

    CARAPI DeqTask(
        /* [out] */ IForkJoinTask** outtask);

    CARAPI LocallyDeqTask(
        /* [out] */ IForkJoinTask** outtask);

    CARAPI UnpushTask(
        /* [in] */ IForkJoinTask* t,
        /* [out] */ Boolean* value);

    CARAPI PeekTask(
        /* [out] */ IForkJoinTask** outtask);

    // Support methods for ForkJoinPool

    CARAPI ExecTask(
        /* [in] */ IForkJoinTask* t);

    CARAPI CancelTasks();

    CARAPI DrainTasksTo(
        /* [in] */ ICollection* c,
        /* [out] */ Int32* value);

    // Support methods for ForkJoinTask

    CARAPI GetQueueSize(
        /* [out] */ Int32* value);

    CARAPI PollLocalTask(
        /* [out] */ IForkJoinTask** outtask);

    CARAPI PollTask(
        /* [out] */ IForkJoinTask** outtask);

    CARAPI JoinTask(
        /* [in] */ IForkJoinTask* joinMe,
        /* [out] */ Int32* value);

    CARAPI GetEstimatedSurplusTaskCount(
        /* [out] */ Int32* value);

    CARAPI HelpQuiescePool();

    CARAPI CheckAccess();

    CARAPI CountStackFrames(
        /* [out] */ Int32* number);

    CARAPI Destroy();

    CARAPI GetContextClassLoader(
        /* [out] */ IClassLoader** outload);

    CARAPI GetId(
        /* [out] */ Int64* id);

    CARAPI GetName(
        /* [out] */ String* name);

    CARAPI GetPriority(
        /* [out] */ Int32* priority);

    CARAPI GetState(
        /* [out] */ ThreadState* state);

    CARAPI GetThreadGroup(
        /* [out] */ IThreadGroup** group);

    CARAPI GetUncaughtExceptionHandler(
        /* [out] */ IThreadUncaughtExceptionHandler** handler);

    CARAPI Interrupt();

    CARAPI IsAlive(
        /* [out] */ Boolean* isAlive);

    CARAPI IsDaemon(
        /* [out] */ Boolean* isDaemon);

    CARAPI IsInterrupted(
        /* [out] */ Boolean* isInterrupted);

    CARAPI Join();

    CARAPI JoinEx(
        /* [in] */ Int64 millis);

    CARAPI JoinEx2(
        /* [in] */ Int64 millis,
        /* [in] */ Int32 nanos);

    CARAPI Resume();

    CARAPI SetContextClassLoader(
        /* [in] */ IClassLoader* cl);

    CARAPI SetDaemon(
        /* [in] */ Boolean isDaemon);

    CARAPI PushInterruptAction(
        /* [in] */ IRunnable* interruptAction);

    CARAPI PopInterruptAction(
        /* [in] */ IRunnable* interruptAction);

    CARAPI SetName(
        /* [in] */ const String& threadName);

    CARAPI SetPriority(
        /* [in] */ Int32 priority);

    CARAPI SetUncaughtExceptionHandler(
        /* [in] */ IThreadUncaughtExceptionHandler* handler);

    CARAPI Start();

    CARAPI Stop();

    CARAPI Suspend();

    CARAPI Unpark();

    CARAPI ParkFor(
        /* [in] */ Int64 nanos);

    CARAPI ParkUntil(
        /* [in] */ Int64 time);

    CARAPI Detach();

    CARAPI Lock();

    CARAPI Unlock();

    CARAPI Notify();

    CARAPI NotifyAll();

    CARAPI Wait();

    CARAPI WaitEx(
        /* [in] */ Int64 millis);

    CARAPI WaitEx2(
        /* [in] */ Int64 millis,
        /* [in] */ Int32 nanos);

    /**
     * Creates a ForkJoinWorkerThread operating in the given pool.
     *
     * @param pool the pool this thread works in
     * @throws NullPointerException if pool is null
     */
    CARAPI constructor(
        /* [in] */ IForkJoinPool* pool);

    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    // Run State management

    /**
     * Initializes internal state after construction but before
     * processing any tasks. If you override this method, you must
     * invoke {@code super.onStart()} at the beginning of the method.
     * Initialization requires care: Most fields must have legal
     * default values, to ensure that attempted accesses from other
     * threads work correctly even before this thread starts
     * processing tasks.
     */
    CARAPI_(void) OnStart();

    /**
     * Performs cleanup associated with termination of this worker
     * thread.  If you override this method, you must invoke
     * {@code super.onTermination} at the end of the overridden method.
     *
     * @param exception the exception causing this thread to abort due
     * to an unrecoverable error, or {@code null} if completed normally
     */
    CARAPI_(void) OnTermination(
        /* [in] */ IThrowable* exception);

private:
    // Randomization

    /**
     * Computes next value for random victim probes and backoffs.
     * Scans don't require a very high quality generator, but also not
     * a crummy one.  Marsaglia xor-shift is cheap and works well
     * enough.  Note: This is manually inlined in FJP.scan() to avoid
     * writes inside busy loops.
     */
    CARAPI_(Int32) NextSeed();

    /*
     * Intrinsics-based atomic writes for queue slots. These are
     * basically the same as methods in AtomicReferenceArray, but
     * specialized for (1) ForkJoinTask elements (2) requirement that
     * nullness and bounds checks have already been performed by
     * callers and (3) effective offsets are known not to overflow
     * from int to long (because of MAXIMUM_QUEUE_CAPACITY). We don't
     * need corresponding version for reads: plain array reads are OK
     * because they are protected by other volatile reads and are
     * confirmed by CASes.
     *
     * Most uses don't actually call these methods, but instead
     * contain inlined forms that enable more predictable
     * optimization.  We don't define the version of write used in
     * pushTask at all, but instead inline there a store-fenced array
     * slot write.
     *
     * Also in most methods, as a performance (not correctness) issue,
     * we'd like to encourage compilers not to arbitrarily postpone
     * setting queueTop after writing slot.  Currently there is no
     * intrinsic for arranging this, but using Unsafe putOrderedInt
     * may be a preferable strategy on some compilers even though its
     * main effect is a pre-, not post- fence. To simplify possible
     * changes, the option is left in comments next to the associated
     * assignments.
     */

    /**
     * CASes slot i of array q from t to null. Caller must ensure q is
     * non-null and index is in range.
     */
    static CARAPI_(Boolean) CasSlotNull(
        /* [in] */ ArrayOf<IForkJoinTask*>* q,
        /* [in] */ const Int32& i,
        /* [in] */ IForkJoinTask* t);

    /**
     * Performs a volatile write of the given task at given slot of
     * array q.  Caller must ensure q is non-null and index is in
     * range. This method is used only during resets and backouts.
     */
    static CARAPI_(void) WriteSlot(
        /* [in] */ ArrayOf<IForkJoinTask*>* q,
        /* [in] */ const Int32& i,
        /* [in] */ IForkJoinTask* t);

    /**
     * Returns a popped task, or null if empty.
     * Called only by this thread.
     */
    CARAPI_(AutoPtr<IForkJoinTask>) PopTask();

    /**
     * Creates or doubles queue array.  Transfers elements by
     * emulating steals (deqs) from old array and placing, oldest
     * first, into new array.
     */
    CARAPI_(void) GrowQueue();

    /**
     * If present, pops and executes the given task, or any other
     * cancelled task
     *
     * @return false if any other non-cancelled task exists in local queue
     */
    CARAPI_(Boolean) LocalHelpJoinTask(
        /* [in] */ IForkJoinTask* joinMe);

    /**
     * Tries to locate and execute tasks for a stealer of the given
     * task, or in turn one of its stealers, Traces
     * currentSteal->currentJoin links looking for a thread working on
     * a descendant of the given task and with a non-empty queue to
     * steal back and execute tasks from.  The implementation is very
     * branchy to cope with potential inconsistencies or loops
     * encountering chains that are stale, unknown, or of length
     * greater than MAX_HELP links.  All of these cases are dealt with
     * by just retrying by caller.
     *
     * @param joinMe the task to join
     * @return true if ran a task
     */
    CARAPI_(Boolean) HelpJoinTask(
        /* [in] */ IForkJoinTask* joinMe);

    /**
     * Performs an uncommon case for joinTask: If task t is at base of
     * some workers queue, steals and executes it.
     *
     * @param t the task
     * @return t's status
     */
    CARAPI_(Int32) TryDeqAndExec(
        /* [in] */ IForkJoinTask* t);

public:
    /**
     * The work-stealing queue array. Size must be a power of two.
     * Initialized when started (as opposed to when constructed), to
     * improve memory locality.
     */
    AutoPtr<ArrayOf<IForkJoinTask*> > mQueue;

    /**
     * The pool this thread works in. Accessed directly by ForkJoinTask.
     */
    AutoPtr<IForkJoinPool> mPool;

    /**
     * Index (mod queue.length) of next queue slot to push to or pop
     * from. It is written only by owner thread, and accessed by other
     * threads only after reading (volatile) queueBase.  Both queueTop
     * and queueBase are allowed to wrap around on overflow, but
     * (queueTop - queueBase) still estimates size.
     */
    Int32 mQueueTop;

    /**
     * Index (mod queue.length) of least valid queue slot, which is
     * always the next position to steal from if nonempty.
     */
    volatile Int32 mQueueBase;

    /**
     * The index of most recent stealer, used as a hint to avoid
     * traversal in method helpJoinTask. This is only a hint because a
     * worker might have had multiple steals and this only holds one
     * of them (usually the most current). Declared non-volatile,
     * relying on other prevailing sync to keep reasonably current.
     */
    Int32 mStealHint;

    /**
     * Index of this worker in pool array. Set once by pool before
     * running, and accessed directly by pool to locate this worker in
     * its workers array.
     */
    Int32 mPoolIndex;

    /**
     * Encoded record for pool task waits. Usages are always
     * surrounded by volatile reads/writes
     */
    Int32 mNextWait;

    /**
     * Complement of poolIndex, offset by count of entries of task
     * waits. Accessed by ForkJoinPool to manage event waiters.
     */
    volatile Int32 mEventCount;

    /**
     * Seed for random number generator for choosing steal victims.
     * Uses Marsaglia xorshift. Must be initialized as nonzero.
     */
    Int32 mSeed;

    /**
     * Number of steals. Directly accessed (and reset) by pool when
     * idle.
     */
    Int32 mStealCount;

    /**
     * True if this worker should or did terminate
     */
    volatile Boolean mTerminate;

    /**
     * Set to true before LockSupport.park; false on return
     */
    volatile Boolean mParked;

    /**
     * True if use local fifo, not default lifo, for local polling.
     * Shadows value from ForkJoinPool.
     */
    Boolean mLocallyFifo;

    /**
     * The task most recently stolen from another worker (or
     * submission queue).  All uses are surrounded by enough volatile
     * reads/writes to maintain as non-volatile.
     */
    AutoPtr<IForkJoinTask> mCurrentSteal;

    /**
     * The task currently being joined, set only when actively trying
     * to help other stealers in helpJoinTask. All uses are surrounded
     * by enough volatile reads/writes to maintain as non-volatile.
     */
    AutoPtr<IForkJoinTask> mCurrentJoin;

private:
    /**
     * Mask for pool indices encoded as shorts
     */
    static Int32 SMASK;

    /**
     * Capacity of work-stealing queue array upon initialization.
     * Must be a power of two. Initial size must be at least 4, but is
     * padded to minimize cache effects.
     */
    static Int32 INITIAL_QUEUE_CAPACITY;

    /**
     * Maximum size for queue array. Must be a power of two
     * less than or equal to 1 << (31 - width of array entry) to
     * ensure lack of index wraparound, but is capped at a lower
     * value to help users trap runaway computations.
     */
    static Int32 MAXIMUM_QUEUE_CAPACITY; // 16M

    /**
     * The maximum stolen->joining link depth allowed in helpJoinTask,
     * as well as the maximum number of retries (allowing on average
     * one staleness retry per level) per attempt to instead try
     * compensation.  Depths for legitimate chains are unbounded, but
     * we use a fixed constant to avoid (otherwise unchecked) cycles
     * and bound staleness of traversal parameters at the expense of
     * sometimes blocking when we could be helping.
     */
    static Int32 MAX_HELP;

    // // Unsafe mechanics
    // private static final sun.misc.Unsafe UNSAFE;
    // private static final long ABASE;
    // private static final int ASHIFT;

    // static {
    //     int s;
    //     try {
    //         UNSAFE = sun.misc.Unsafe.getUnsafe();
    //         Class<?> a = ForkJoinTask[].class;
    //         ABASE = UNSAFE.arrayBaseOffset(a);
    //         s = UNSAFE.arrayIndexScale(a);
    //     } catch (Exception e) {
    //         throw new Error(e);
    //     }
    //     if ((s & (s-1)) != 0)
    //         throw new Error("data type scale not a power of two");
    //     ASHIFT = 31 - Integer.numberOfLeadingZeros(s);
    // }
};

} // namespace Concurrent
} // namespace Utility
} // namespace Elastos

#endif //__CFORKJOINWORKERTHREAD_H__