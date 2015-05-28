
#ifndef __CFORKJOINPOOL_H__
#define __CFORKJOINPOOL_H__

#include "_CForkJoinPool.h"
#include "AbstractExecutorService.h"

using Elastos::Core::IThrowable;
using Elastos::Core::IRandom;
using Elastos::Utility::Concurrent::Atomic::IAtomicInteger32;
using Elastos::Utility::Concurrent::Locks::IReentrantLock;
using Elastos::Utility::Concurrent::Locks::ICondition;

namespace Elastos {
namespace Utility {
namespace Concurrent {

CarClass(CForkJoinPool), public AbstractExecutorService
{
public:
    /**
     * Default ForkJoinWorkerThreadFactory implementation; creates a
     * new ForkJoinWorkerThread.
     */
    class DefaultForkJoinWorkerThreadFactory
        : public IForkJoinPoolForkJoinWorkerThreadFactory
    {
    public:
        AutoPtr<IForkJoinWorkerThread> NewThread(
            /* [in] */ IForkJoinPool* pool);
    };

    class _InvokeAll
//        : public RecursiveAction
    {
    public:
        _InvokeAll(
            /* [in] */ CForkJoinPool* owner,
            /* [in] */ IArrayList* tasks);

        void Compute();

    public:
//        private static final long serialVersionUID = -7914297376763021607L;
        AutoPtr<IArrayList> mTasks;
        CForkJoinPool* mOwner;
    };

public:
    // Exported methods

    // Constructors

    /**
     * Creates a {@code ForkJoinPool} with parallelism equal to {@link
     * java.lang.Runtime#availableProcessors}, using the {@linkplain
     * #defaultForkJoinWorkerThreadFactory default thread factory},
     * no UncaughtExceptionHandler, and non-async LIFO processing mode.
     */
    CARAPI constructor();

    /**
     * Creates a {@code ForkJoinPool} with the indicated parallelism
     * level, the {@linkplain
     * #defaultForkJoinWorkerThreadFactory default thread factory},
     * no UncaughtExceptionHandler, and non-async LIFO processing mode.
     *
     * @param parallelism the parallelism level
     * @throws IllegalArgumentException if parallelism less than or
     *         equal to zero, or greater than implementation limit
     */
    CARAPI constructor(
        /* [in] */ const Int32& parallelism);

    /**
     * Creates a {@code ForkJoinPool} with the given parameters.
     *
     * @param parallelism the parallelism level. For default value,
     * use {@link java.lang.Runtime#availableProcessors}.
     * @param factory the factory for creating new threads. For default value,
     * use {@link #defaultForkJoinWorkerThreadFactory}.
     * @param handler the handler for internal worker threads that
     * terminate due to unrecoverable errors encountered while executing
     * tasks. For default value, use {@code null}.
     * @param asyncMode if true,
     * establishes local first-in-first-out scheduling mode for forked
     * tasks that are never joined. This mode may be more appropriate
     * than default locally stack-based mode in applications in which
     * worker threads only process event-style asynchronous tasks.
     * For default value, use {@code false}.
     * @throws IllegalArgumentException if parallelism less than or
     *         equal to zero, or greater than implementation limit
     * @throws NullPointerException if the factory is null
     */
    CARAPI constructor(
        /* [in] */ const Int32& parallelism,
        /* [in] */ IForkJoinPoolForkJoinWorkerThreadFactory* factory,
        /* [in] */ IThreadUncaughtExceptionHandler* handler,
        /* [in] */ const Boolean& asyncMode);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    // Execution methods

    CARAPI Invoke(
        /* [in] */ IForkJoinTask* task,
        /* [out] */ IInterface** outface);

    CARAPI Execute(
        /* [in] */ IForkJoinTask* task);

    // AbstractExecutorService methods

    CARAPI Execute(
        /* [in] */ IRunnable* command);

    CARAPI Submit(
        /* [in] */ IForkJoinTask* task,
        /* [out] */ IForkJoinTask** outfork);

    CARAPI Submit(
        /* [in] */ ICallable* task,
        /* [out] */ IFuture** outfork);

    CARAPI Submit(
        /* [in] */ IRunnable* task,
        /* [in] */ IInterface* result,
        /* [out] */ IFuture** outfork);

    CARAPI Submit(
        /* [in] */ IRunnable* task,
        /* [out] */ IFuture** outfork);

    /**
     * Returns the factory used for constructing new workers.
     *
     * @return the factory used for constructing new workers
     */
    CARAPI GetFactory(
        /* [out] */ IForkJoinPoolForkJoinWorkerThreadFactory** res);

    /**
     * Returns the handler for internal worker threads that terminate
     * due to unrecoverable errors encountered while executing tasks.
     *
     * @return the handler, or {@code null} if none
     */
    CARAPI GetUncaughtExceptionHandler(
        /* [out] */ IThreadUncaughtExceptionHandler** res);

    CARAPI GetParallelism(
        /* [out] */ Int32* value);

    CARAPI GetPoolSize(
        /* [out] */ Int32* value);

    CARAPI GetAsyncMode(
        /* [out] */ Boolean* value);

    CARAPI GetRunningThreadCount(
        /* [out] */ Int32* value);

    CARAPI GetActiveThreadCount(
        /* [out] */ Int32* value);

    CARAPI IsQuiescent(
        /* [out] */ Boolean* value);

    CARAPI GetStealCount(
        /* [out] */ Int64* value);

    CARAPI GetQueuedTaskCount(
        /* [out] */ Int64* value);

    CARAPI GetQueuedSubmissionCount(
        /* [out] */ Int32* value);

    CARAPI HasQueuedSubmissions(
        /* [out] */ Boolean* value);

    CARAPI ToString(
        /* [out] */ String* str);

    CARAPI Shutdown();

    CARAPI ShutdownNow(
        /* [out] */ IList** tasks);

    CARAPI IsTerminated(
        /* [out] */ Boolean* result);

    CARAPI IsTerminating(
        /* [out] */ Boolean* value);

    CARAPI IsAtLeastTerminating(
        /* [out] */ Boolean* value);

    CARAPI IsShutdown(
        /* [out] */ Boolean* result);

    CARAPI AwaitTermination(
        /* [in] */ Int64 timeout,
        /* [in] */ ITimeUnit* unit,
        /* [out] */ Boolean* result);

    CARAPI InvokeAll(
        /* [in] */ ICollection* tasks,
        /* [out] */ IList** futures);

    CARAPI InvokeAll(
        /* [in] */ ICollection* tasks,
        /* [in] */ Int64 timeout,
        /* [in] */ ITimeUnit* unit,
        /* [out] */ IList** futures);

    CARAPI InvokeAny(
        /* [in] */ ICollection* tasks,
        /* [out] */ IInterface** result);

    CARAPI InvokeAny(
        /* [in] */ ICollection* tasks,
        /* [in] */ Int64 timeout,
        /* [in] */ ITimeUnit* unit,
        /* [out] */ IInterface** result);

    /**
     * Blocks in accord with the given blocker.  If the current thread
     * is a {@link ForkJoinWorkerThread}, this method possibly
     * arranges for a spare thread to be activated if necessary to
     * ensure sufficient parallelism while the current thread is blocked.
     *
     * <p>If the caller is not a {@link ForkJoinTask}, this method is
     * behaviorally equivalent to
     *  <pre> {@code
     * while (!blocker.isReleasable())
     *   if (blocker.block())
     *     return;
     * }</pre>
     *
     * If the caller is a {@code ForkJoinTask}, then the pool may
     * first be expanded to ensure parallelism, and later adjusted.
     *
     * @param blocker the blocker
     * @throws InterruptedException if blocker.block did so
     */
    static CARAPI_(void) ManagedBlock(
        /* [in] */ IForkJoinPoolManagedBlocker* blocker);

    /**
     * Top-level loop for worker threads: On each step: if the
     * previous step swept through all queues and found no tasks, or
     * there are excess threads, then possibly blocks. Otherwise,
     * scans for and, if found, executes a task. Returns when pool
     * and/or worker terminate.
     *
     * @param w the worker
     */
    CARAPI_(void) Work(
        /* [in] */ IForkJoinWorkerThread* w);

    // Signalling

    /**
     * Wakes up or creates a worker.
     */
    CARAPI_(void) SignalWork();

    // Scanning for tasks

    /**
     * Possibly blocks waiting for the given task to complete, or
     * cancels the task if terminating.  Fails to wait if contended.
     *
     * @param joinMe the task
     */
    CARAPI_(void) TryAwaitJoin(
        /* [in] */ IForkJoinTask* joinMe);

    CARAPI TimedAwaitJoin(
        /* [in] */ IForkJoinTask* joinMe,
        /* [in] */ Int64 nanos);

    /**
     * Callback from ForkJoinWorkerThread constructor to assign a
     * public name
     */
    CARAPI_(String) NextWorkerName();

    /**
     * Callback from ForkJoinWorkerThread constructor to
     * determine its poolIndex and record in workers array.
     *
     * @param w the worker
     * @return the worker's pool index
     */
    CARAPI_(Int32) RegisterWorker(
        /* [in] */ IForkJoinWorkerThread* w);

    /**
     * Final callback from terminating worker.  Removes record of
     * worker from array, and adjusts counts. If pool is shutting
     * down, tries to complete termination.
     *
     * @param w the worker
     */
    CARAPI_(void) DeregisterWorker(
        /* [in] */ IForkJoinWorkerThread* w,
        /* [in] */ IThrowable* ex);

    // misc ForkJoinWorkerThread support

    /**
     * Increments or decrements quiescerCount. Needed only to prevent
     * triggering shutdown if a worker is transiently inactive while
     * checking quiescence.
     *
     * @param delta 1 for increment, -1 for decrement
     */
    CARAPI_(void) AddQuiescerCount(
        /* [in] */ const Int32& delta);

    /**
     * Directly increments or decrements active count without queuing.
     * This method is used to transiently assert inactivation while
     * checking quiescence.
     *
     * @param delta 1 for increment, -1 for decrement
     */
    CARAPI_(void) AddActiveCount(
        /* [in] */ const Int32& delta);

    /**
     * Returns the approximate (non-atomic) number of idle threads per
     * active thread.
     */
    CARAPI_(Int32) IdlePerActive();

    /**
     * Removes and returns the next unexecuted submission if one is
     * available.  This method may be useful in extensions to this
     * class that re-assign work in systems with multiple pools.
     *
     * @return the next submission, or {@code null} if none
     */
    CARAPI_(AutoPtr<IForkJoinTask>) PollSubmission();

    /**
     * Removes all available unexecuted submitted and forked tasks
     * from scheduling queues and adds them to the given collection,
     * without altering their execution status. These may include
     * artificially generated or wrapped tasks. This method is
     * designed to be invoked only when the pool is known to be
     * quiescent. Invocations at other times may not remove all
     * tasks. A failure encountered while attempting to add elements
     * to collection {@code c} may result in elements being in
     * neither, either or both collections when the associated
     * exception is thrown.  The behavior of this operation is
     * undefined if the specified collection is modified while the
     * operation is in progress.
     *
     * @param c the collection to transfer elements into
     * @return the number of elements transferred
     */
    CARAPI_(Int32) DrainTasksTo(
        /* [in] */ ICollection* c);

    // AbstractExecutorService overrides.  These rely on undocumented
    // fact that ForkJoinTask.adapt returns ForkJoinTasks that also
    // implement RunnableFuture.

    CARAPI_(AutoPtr<IRunnableFuture>) NewTaskFor(
        /* [in] */ IRunnable* runnable,
        /* [in] */ IInterface* value);

    CARAPI_(AutoPtr<IRunnableFuture>) NewTaskFor(
        /* [in] */ ICallable* callable);

private:
    /**
     * If there is a security manager, makes sure caller has
     * permission to modify threads.
     */
    static CARAPI_(void) CheckPermission();

    /**
     * Variant of signalWork to help release waiters on rescans.
     * Tries once to release a waiter if active count < 0.
     *
     * @return false if failed due to contention, else true
     */
    CARAPI_(Boolean) TryReleaseWaiter();

    // Scanning for tasks

    /**
     * Scans for and, if found, executes one task. Scans start at a
     * random index of workers array, and randomly select the first
     * (2*#workers)-1 probes, and then, if all empty, resort to 2
     * circular sweeps, which is necessary to check quiescence. and
     * taking a submission only if no stealable tasks were found.  The
     * steal code inside the loop is a specialized form of
     * ForkJoinWorkerThread.deqTask, followed bookkeeping to support
     * helpJoinTask and signal propagation. The code for submission
     * queues is almost identical. On each steal, the worker completes
     * not only the task, but also all local tasks that this task may
     * have generated. On detecting staleness or contention when
     * trying to take a task, this method returns without finishing
     * sweep, which allows global state rechecks before retry.
     *
     * @param w the worker
     * @param a the number of active workers
     * @return true if swept all queues without finding a task
     */
    CARAPI_(Boolean) Scan(
        /* [in] */ IForkJoinWorkerThread* w,
        /* [in] */ const Int32& a);

    /**
     * Tries to enqueue worker w in wait queue and await change in
     * worker's eventCount.  If the pool is quiescent and there is
     * more than one worker, possibly terminates worker upon exit.
     * Otherwise, before blocking, rescans queues to avoid missed
     * signals.  Upon finding work, releases at least one worker
     * (which may be the current worker). Rescans restart upon
     * detected staleness or failure to release due to
     * contention. Note the unusual conventions about Thread.interrupt
     * here and elsewhere: Because interrupts are used solely to alert
     * threads to check termination, which is checked here anyway, we
     * clear status (using Thread.interrupted) before any call to
     * park, so that park does not immediately return due to status
     * being set via some other unrelated call to interrupt in user
     * code.
     *
     * @param w the calling worker
     * @param c the ctl value on entry
     * @return true if waited or another thread was released upon enq
     */
    CARAPI_(Boolean) TryAwaitWork(
        /* [in] */ IForkJoinWorkerThread* w,
        /* [in] */ const Int64& c);

    /**
     * If inactivating worker w has caused pool to become
     * quiescent, check for pool termination, and wait for event
     * for up to SHRINK_RATE nanosecs (rescans are unnecessary in
     * this case because quiescence reflects consensus about lack
     * of work). On timeout, if ctl has not changed, terminate the
     * worker. Upon its termination (see deregisterWorker), it may
     * wake up another worker to possibly repeat this process.
     *
     * @param w the calling worker
     * @param currentCtl the ctl value after enqueuing w
     * @param prevCtl the ctl value if w terminated
     * @param v the eventCount w awaits change
     */
    CARAPI_(void) IdleAwaitWork(
        /* [in] */ IForkJoinWorkerThread* w,
        /* [in] */ const Int64& currentCtl,
        /* [in] */ const Int64& prevCtl,
        /* [in] */ const Int32& v);

    // Submissions

    /**
     * Enqueues the given task in the submissionQueue.  Same idea as
     * ForkJoinWorkerThread.pushTask except for use of submissionLock.
     *
     * @param t the task
     */
    CARAPI_(void) AddSubmission(
        /* [in] */ IForkJoinTask* t);

    //  (pollSubmission is defined below with exported methods)

    /**
     * Creates or doubles submissionQueue array.
     * Basically identical to ForkJoinWorkerThread version.
     */
    CARAPI GrowSubmissionQueue();

    // Blocking support

    /**
     * Tries to increment blockedCount, decrement active count
     * (sometimes implicitly) and possibly release or create a
     * compensating worker in preparation for blocking. Fails
     * on contention or termination.
     *
     * @return true if the caller can block, else should recheck and retry
     */
    CARAPI_(Boolean) TryPreBlock();

    /**
     * Decrements blockedCount and increments active count.
     */
    CARAPI_(void) PostBlock();

    /**
     * If necessary, compensates for blocker, and blocks.
     */
    CARAPI_(void) AwaitBlocker(
        /* [in] */ IForkJoinPoolManagedBlocker* blocker);

    // Creating, registering and deregistring workers

    /**
     * Tries to create and start a worker; minimally rolls back counts
     * on failure.
     */
    CARAPI_(void) AddWorker();

    // Shutdown and termination

    /**
     * Possibly initiates and/or completes termination.
     *
     * @param now if true, unconditionally terminate, else only
     * if shutdown and empty queue and no active workers
     * @return true if now terminating or terminated
     */
    CARAPI_(Boolean) TryTerminate(
        /* [in] */ const Boolean& now);

    /**
     * Runs up to three passes through workers: (0) Setting
     * termination status for each worker, followed by wakeups up to
     * queued workers; (1) helping cancel tasks; (2) interrupting
     * lagging threads (likely in external tasks, but possibly also
     * blocked in joins).  Each pass repeats previous steps because of
     * potential lagging thread creation.
     */
    CARAPI_(void) StartTerminating();

    /**
     * Polls and cancels all submissions. Called only during termination.
     */
    CARAPI_(void) CancelSubmissions();

    /**
     * Tries to set the termination status of waiting workers, and
     * then wakes them up (after which they will terminate).
     */
    CARAPI_(void) TerminateWaiters();

    /**
     * Unless terminating, forks task if within an ongoing FJ
     * computation in the current pool, else submits as external task.
     */
    CARAPI_(void) ForkOrSubmit(
        /* [in] */ IForkJoinTask* task);

public:
    /**
     * Creates a new ForkJoinWorkerThread. This factory is used unless
     * overridden in ForkJoinPool constructors.
     */
    static AutoPtr<IForkJoinPoolForkJoinWorkerThreadFactory>
        mDefaultForkJoinWorkerThreadFactory;

    /**
     * Permission required for callers of methods that may start or
     * kill threads.
     */
//    static AutoPtr<IRuntimePermission> modifyThreadPermission;

    /**
     * Generator for assigning sequence numbers as pool names.
     */
    static AutoPtr<IAtomicInteger32> mPoolNumberGenerator;

    /**
     * Generator for initial random seeds for worker victim
     * selection. This is used only to create initial seeds. Random
     * steals use a cheaper xorshift generator per steal attempt. We
     * don't expect much contention on seedGenerator, so just use a
     * plain Random.
     */
    static AutoPtr<IRandom> mWorkerSeedGenerator;

    /**
     * Array holding all worker threads in the pool.  Initialized upon
     * construction. Array size must be a power of two.  Updates and
     * replacements are protected by scanGuard, but the array is
     * always kept in a consistent enough state to be randomly
     * accessed without locking by workers performing work-stealing,
     * as well as other traversal-based methods in this class, so long
     * as reads memory-acquire by first reading ctl. All readers must
     * tolerate that some array slots may be null.
     */
    AutoPtr<ArrayOf<IForkJoinWorkerThread*> > mWorkers;

    /**
     * Initial size for submission queue array. Must be a power of
     * two.  In many applications, these always stay small so we use a
     * small initial cap.
     */
    static Int32 INITIAL_QUEUE_CAPACITY;

    /**
     * Maximum size for submission queue array. Must be a power of two
     * less than or equal to 1 << (31 - width of array entry) to
     * ensure lack of index wraparound, but is capped at a lower
     * value to help users trap runaway computations.
     */
    static Int32 MAXIMUM_QUEUE_CAPACITY; // 16M

    /**
     * Array serving as submission queue. Initialized upon construction.
     */
    AutoPtr<ArrayOf<IForkJoinTask*> > mSubmissionQueue;

    /**
     * Lock protecting submissions array for addSubmission
     */
    AutoPtr<IReentrantLock> mSubmissionLock;

    /**
     * Condition for awaitTermination, using submissionLock for
     * convenience.
     */
    AutoPtr<ICondition> mTermination;

    /**
     * Creation factory for worker threads.
     */
    AutoPtr<IForkJoinPoolForkJoinWorkerThreadFactory> mFactory;

    /**
     * The uncaught exception handler used when any worker abruptly
     * terminates.
     */
    AutoPtr<IThreadUncaughtExceptionHandler> mUeh;

    /**
     * Prefix for assigning names to worker threads
     */
    String mWorkerNamePrefix;

    /**
     * Sum of per-thread steal counts, updated only when threads are
     * idle or terminating.
     */
    volatile Int64 mStealCount;

    /**
     * Main pool control -- a long packed with:
     * AC: Number of active running workers minus target parallelism (16 bits)
     * TC: Number of total workers minus target parallelism (16 bits)
     * ST: true if pool is terminating (1 bit)
     * EC: the wait count of top waiting thread (15 bits)
     * ID: ~poolIndex of top of Treiber stack of waiting threads (16 bits)
     *
     * When convenient, we can extract the upper 32 bits of counts and
     * the lower 32 bits of queue state, u = (int)(ctl >>> 32) and e =
     * (int)ctl.  The ec field is never accessed alone, but always
     * together with id and st. The offsets of counts by the target
     * parallelism and the positionings of fields makes it possible to
     * perform the most common checks via sign tests of fields: When
     * ac is negative, there are not enough active workers, when tc is
     * negative, there are not enough total workers, when id is
     * negative, there is at least one waiting worker, and when e is
     * negative, the pool is terminating.  To deal with these possibly
     * negative fields, we use casts in and out of "short" and/or
     * signed shifts to maintain signedness.
     */
    volatile Int64 mCtl;

    // bit positions/shifts for fields
    static Int32  AC_SHIFT;
    static Int32  TC_SHIFT;
    static Int32  ST_SHIFT;
    static Int32  EC_SHIFT;

    // bounds
    static Int32  MAX_ID;  // max poolIndex
    static Int32  SMASK;  // mask short bits
    static Int32  SHORT_SIGN;
    static Int32  INT_SIGN;

    // masks
    static Int64 STOP_BIT;
    static Int64 AC_MASK;
    static Int64 TC_MASK;

    // units for incrementing and decrementing
    static Int64 TC_UNIT;
    static Int64 AC_UNIT;

    // masks and units for dealing with u = (int)(ctl >>> 32)
    static Int32  UAC_SHIFT;
    static Int32  UTC_SHIFT;
    static Int32  UAC_MASK;
    static Int32  UTC_MASK;
    static Int32  UAC_UNIT;
    static Int32  UTC_UNIT;

    // masks and units for dealing with e = (int)ctl
    static Int32  E_MASK; // no STOP_BIT
    static Int32  EC_UNIT;

    /**
     * The target parallelism level.
     */
    Int32 mParallelism;

    /**
     * Index (mod submission queue length) of next element to take
     * from submission queue. Usage is identical to that for
     * per-worker queues -- see ForkJoinWorkerThread internal
     * documentation.
     */
    volatile Int32 mQueueBase;

    /**
     * Index (mod submission queue length) of next element to add
     * in submission queue. Usage is identical to that for
     * per-worker queues -- see ForkJoinWorkerThread internal
     * documentation.
     */
    Int32 mQueueTop;

    /**
     * True when shutdown() has been called.
     */
    volatile Boolean mShutdown;

    /**
     * True if use local fifo, not default lifo, for local polling.
     * Read by, and replicated by ForkJoinWorkerThreads.
     */
    Boolean mLocallyFifo;

    /**
     * The number of threads in ForkJoinWorkerThreads.helpQuiescePool.
     * When non-zero, suppresses automatic shutdown when active
     * counts become zero.
     */
    volatile Int32 mQuiescerCount;

    /**
     * The number of threads blocked in join.
     */
    volatile Int32 mBlockedCount;

    /**
     * Counter for worker Thread names (unrelated to their poolIndex)
     */
    volatile Int32 mNextWorkerNumber;

    /**
     * The index for the next created worker. Accessed under scanGuard.
     */
    Int32 mNextWorkerIndex;

    /**
     * SeqLock and index masking for updates to workers array.  Locked
     * when SG_UNIT is set. Unlocking clears bit by adding
     * SG_UNIT. Staleness of read-only operations can be checked by
     * comparing scanGuard to value before the reads. The low 16 bits
     * (i.e, anding with SMASK) hold (the smallest power of two
     * covering all worker indices, minus one, and is used to avoid
     * dealing with large numbers of null slots when the workers array
     * is overallocated.
     */
    volatile Int32 mScanGuard;

    static Int32 SG_UNIT;

    /**
     * The wakeup interval (in nanoseconds) for a worker waiting for a
     * task when the pool is quiescent to instead try to shrink the
     * number of workers.  The exact value does not matter too
     * much. It must be short enough to release resources during
     * sustained periods of idleness, but not so short that threads
     * are continually re-created.
     */
    static Int64 SHRINK_RATE; // 4 seconds

    // // Unsafe mechanics
    // private static final sun.misc.Unsafe UNSAFE;
    // private static final long ctlOffset;
    // private static final long stealCountOffset;
    // private static final long blockedCountOffset;
    // private static final long quiescerCountOffset;
    // private static final long scanGuardOffset;
    // private static final long nextWorkerNumberOffset;
    // private static final long ABASE;
    // private static final int ASHIFT;

    // static {
    //     poolNumberGenerator = new AtomicInteger();
    //     workerSeedGenerator = new Random();
    //     modifyThreadPermission = new RuntimePermission("modifyThread");
    //     defaultForkJoinWorkerThreadFactory =
    //         new DefaultForkJoinWorkerThreadFactory();
    //     try {
    //         UNSAFE = sun.misc.Unsafe.getUnsafe();
    //         Class<?> k = ForkJoinPool.class;
    //         ctlOffset = UNSAFE.objectFieldOffset
    //             (k.getDeclaredField("ctl"));
    //         stealCountOffset = UNSAFE.objectFieldOffset
    //             (k.getDeclaredField("stealCount"));
    //         blockedCountOffset = UNSAFE.objectFieldOffset
    //             (k.getDeclaredField("blockedCount"));
    //         quiescerCountOffset = UNSAFE.objectFieldOffset
    //             (k.getDeclaredField("quiescerCount"));
    //         scanGuardOffset = UNSAFE.objectFieldOffset
    //             (k.getDeclaredField("scanGuard"));
    //         nextWorkerNumberOffset = UNSAFE.objectFieldOffset
    //             (k.getDeclaredField("nextWorkerNumber"));
    //     } catch (Exception e) {
    //         throw new Error(e);
    //     }
    //     Class<?> a = ForkJoinTask[].class;
    //     ABASE = UNSAFE.arrayBaseOffset(a);
    //     int s = UNSAFE.arrayIndexScale(a);
    //     if ((s & (s-1)) != 0)
    //         throw new Error("data type scale not a power of two");
    //     ASHIFT = 31 - Integer.numberOfLeadingZeros(s);
    // }
};

} // namespace Concurrent
} // namespace Utility
} // namespace Elastos

#endif //__CFORKJOINPOOL_H__