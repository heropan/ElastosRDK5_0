
#include "CThreadPoolExecutor.h"
#include "CAtomicInteger32.h"
#include "Executors.h"
#include "TimeUnit.h"
#include <elastos/Thread.h>
#include "CArrayList.h"

using Elastos::Core::EIID_IRunnable;
using Elastos::Core::Thread;
using Elastos::Utility::CArrayList;
using Elastos::Utility::Concurrent::Atomic::CAtomicInteger32;

namespace Elastos {
namespace Utility {
namespace Concurrent {

const Int32 CThreadPoolExecutor::COUNT_BITS;
const Int32 CThreadPoolExecutor::CAPACITY;

// runState is stored in the high-order bits
const Int32 CThreadPoolExecutor::RUNNING;
const Int32 CThreadPoolExecutor::SHUTDOWN;
const Int32 CThreadPoolExecutor::STOP;
const Int32 CThreadPoolExecutor::TIDYING;
const Int32 CThreadPoolExecutor::TERMINATED;
const AutoPtr<IRejectedExecutionHandler> CThreadPoolExecutor::sDefaultHandler =
        new CThreadPoolExecutor::AbortPolicy();
const Boolean CThreadPoolExecutor::ONLY_ONE;

//==========================================================
//         CThreadPoolExecutor::Worker
//==========================================================
CAR_INTERFACE_IMPL(CThreadPoolExecutor::Worker, AbstractQueuedSynchronizer, IRunnable);

CThreadPoolExecutor::Worker::Worker(
    /* [in] */ IRunnable* firstTask,
    /* [in] */ CThreadPoolExecutor* owner)
    : mFirstTask(firstTask)
    , mOwner(owner)
{
    AutoPtr<IThreadFactory> factory;
    mOwner->GetThreadFactory((IThreadFactory**)&factory);
    factory->NewThread((IRunnable*)this, (IThread**)&mThread);
}

Boolean CThreadPoolExecutor::Worker::TryAcquire(
    /* [in] */ Int32 unused)
{
    if (CompareAndSetState(0, 1)) {
        AutoPtr<IThread> t = Thread::GetCurrentThread();
        SetExclusiveOwnerThread(t);
        return TRUE;
    }
    return FALSE;
}

Boolean CThreadPoolExecutor::Worker::TryRelease(
    /* [in] */ Int32 unused)
{
    SetExclusiveOwnerThread(NULL);
    SetState(0);
    return TRUE;
}


//==========================================================
//         CThreadPoolExecutor::CallerRunsPolicy
//==========================================================
CAR_INTERFACE_IMPL(CThreadPoolExecutor::CallerRunsPolicy, Object, IRejectedExecutionHandler);

ECode CThreadPoolExecutor::CallerRunsPolicy::RejectedExecution(
    /* [in] */ IRunnable* r,
    /* [in] */ IThreadPoolExecutor* e)
{
    Boolean isShutdown;
    if (e->IsShutdown(&isShutdown), !isShutdown) {
        r->Run();
    }
    return NOERROR;
}


//==========================================================
//         CThreadPoolExecutor::AbortPolicy
//==========================================================
CAR_INTERFACE_IMPL(CThreadPoolExecutor::AbortPolicy, Object, IRejectedExecutionHandler);


//==========================================================
//         CThreadPoolExecutor::DiscardPolicy
//==========================================================
CAR_INTERFACE_IMPL(CThreadPoolExecutor::DiscardPolicy, Object, IRejectedExecutionHandler);


//==========================================================
//         CThreadPoolExecutor::DiscardOldestPolicy
//==========================================================
CAR_INTERFACE_IMPL(CThreadPoolExecutor::DiscardOldestPolicy, Object, IRejectedExecutionHandler);

ECode CThreadPoolExecutor::DiscardOldestPolicy::RejectedExecution(
    /* [in] */ IRunnable* r,
    /* [in] */ IThreadPoolExecutor* e)
{
    Boolean isShutdown;
    if (e->IsShutdown(&isShutdown), !isShutdown) {
        AutoPtr<IBlockingQueue> q;
        e->GetQueue((IBlockingQueue**)&q);
        AutoPtr<IInterface> obj;
        q->Poll((IInterface**)&obj);
        e->Execute(r);
    }
    return NOERROR;
}


//==========================================================
//         CThreadPoolExecutor
//==========================================================
CAR_INTERFACE_IMPL(CThreadPoolExecutor, AbstractExecutorService, IThreadPoolExecutor)

CAR_OBJECT_IMPL(CThreadPoolExecutor);

CThreadPoolExecutor::CThreadPoolExecutor()
    : mLargestPoolSize(0)
    , mCompletedTaskCount(0)
    , mKeepAliveTime(0)
    , mAllowCoreThreadTimeOut(FALSE)
    , mCorePoolSize(0)
    , mMaximumPoolSize(0)
{
    ASSERT_SUCCEEDED(CAtomicInteger32::New(CtlOf(RUNNING, 0), (IAtomicInteger32**)&mCtl));
}

CThreadPoolExecutor::~CThreadPoolExecutor()
{
    Shutdown();
}

ECode CThreadPoolExecutor::constructor(
    /* [in] */ Int32 corePoolSize,
    /* [in] */ Int32 maximumPoolSize,
    /* [in] */ Int64 keepAliveTime,
    /* [in] */ ITimeUnit* unit,
    /* [in] */ IBlockingQueue* workQueue)
{
    AutoPtr<IThreadFactory> factory = Executors::GetDefaultThreadFactory();
    return constructor(corePoolSize, maximumPoolSize, keepAliveTime, unit, workQueue,
            factory, sDefaultHandler);
}

ECode CThreadPoolExecutor::constructor(
    /* [in] */ Int32 corePoolSize,
    /* [in] */ Int32 maximumPoolSize,
    /* [in] */ Int64 keepAliveTime,
    /* [in] */ ITimeUnit* unit,
    /* [in] */ IBlockingQueue* workQueue,
    /* [in] */ IThreadFactory* threadFactory)
{
    return constructor(corePoolSize, maximumPoolSize, keepAliveTime, unit, workQueue,
             threadFactory, sDefaultHandler);
}

ECode CThreadPoolExecutor::constructor(
    /* [in] */ Int32 corePoolSize,
    /* [in] */ Int32 maximumPoolSize,
    /* [in] */ Int64 keepAliveTime,
    /* [in] */ ITimeUnit* unit,
    /* [in] */ IBlockingQueue* workQueue,
    /* [in] */ IRejectedExecutionHandler* handler)
{
    AutoPtr<IThreadFactory> factory = Executors::GetDefaultThreadFactory();
    return constructor(corePoolSize, maximumPoolSize, keepAliveTime, unit, workQueue,
             factory, handler);
}

ECode CThreadPoolExecutor::constructor(
    /* [in] */ Int32 corePoolSize,
    /* [in] */ Int32 maximumPoolSize,
    /* [in] */ Int64 keepAliveTime,
    /* [in] */ ITimeUnit* unit,
    /* [in] */ IBlockingQueue* workQueue,
    /* [in] */ IThreadFactory* threadFactory,
    /* [in] */ IRejectedExecutionHandler* handler)
{
     if (corePoolSize < 0 ||
        maximumPoolSize <= 0 ||
        maximumPoolSize < corePoolSize ||
        keepAliveTime < 0) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (workQueue == NULL || threadFactory == NULL || handler == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }
    mCorePoolSize = corePoolSize;
    mMaximumPoolSize = maximumPoolSize;
    mWorkQueue = workQueue;
    unit->ToNanos(keepAliveTime, &mKeepAliveTime);
    mThreadFactory = threadFactory;
    mHandler = handler;
    return NOERROR;
}

Boolean CThreadPoolExecutor::CompareAndIncrementWorkerCount(
    /* [in] */ Int32 expect)
{
    Boolean result;
    mCtl->CompareAndSet(expect, expect + 1, &result);
    return result;
}

Boolean CThreadPoolExecutor::CompareAndDecrementWorkerCount(
    /* [in] */ Int32 expect)
{
    Boolean result;
    mCtl->CompareAndSet(expect, expect - 1, &result);
    return result;
}

void CThreadPoolExecutor::DecrementWorkerCount()
{
    Int32 value;
    do {} while (mCtl->Get(&value), !CompareAndDecrementWorkerCount(value));
}

void CThreadPoolExecutor::AdvanceRunState(
    /* [in] */ Int32 targetState)
{
    for (;;) {
        Int32 c;
        mCtl->Get(&c);
        Boolean result;
        if (RunStateAtLeast(c, targetState) ||
            (mCtl->CompareAndSet(c, CtlOf(targetState, WorkerCountOf(c)), &result), result))
            break;
    }
}

void CThreadPoolExecutor::TryTerminate()
{
    for (;;) {
        Int32 c;
        mCtl->Get(&c);
        Boolean isEmpty;
        if (IsRunning(c) ||
            RunStateAtLeast(c, TIDYING) ||
            (RunStateOf(c) == SHUTDOWN && (mWorkQueue->IsEmpty(&isEmpty), !isEmpty))) {
            return;
        }
        if (WorkerCountOf(c) != 0) { // Eligible to terminate
            InterruptIdleWorkers(ONLY_ONE);
            return;
        }

        Autolock lock(mMainLock);

        Boolean result;
        if (mCtl->CompareAndSet(c, CtlOf(TIDYING, 0), &result), result) {
            Terminated();
            mCtl->Set(CtlOf(TERMINATED, 0));
            mTermination.Broadcast();
            return;
        }
        // else retry on failed CAS
    }
}

ECode CThreadPoolExecutor::CheckShutdownAccess()
{
    // TODO:
    // SecurityManager security = System.getSecurityManager();
    // if (security != null) {
    //     security.checkPermission(shutdownPerm);
    //     final ReentrantLock mainLock = this.mainLock;
    //     mainLock.lock();
    //     try {
    //         for (Worker w : workers)
    //             security.checkAccess(w.thread);
    //     } finally {
    //         mainLock.unlock();
    //     }
    // }
    return NOERROR;
}

void CThreadPoolExecutor::InterruptWorkers()
{
    Autolock lock(mMainLock);

    HashSet< AutoPtr<Worker>, HashWorker >::Iterator it;
    for (it = mWorkers.Begin(); it != mWorkers.End(); ++it) {
        AutoPtr<Worker> w = *it;
        w->mThread->Interrupt();
    }
}

void CThreadPoolExecutor::InterruptIdleWorkers(
    /* [in] */ Boolean onlyOne)
{
    Autolock lock(mMainLock);

    HashSet< AutoPtr<Worker>, HashWorker >::Iterator it;
    for (it = mWorkers.Begin(); it != mWorkers.End(); ++it) {
        AutoPtr<Worker> w = *it;
        AutoPtr<IThread> t = w->mThread;
        Boolean result;
        if ((t->IsInterrupted(&result), !result) && w->TryLock()) {
            t->Interrupt();
            w->Unlock();
        }
        if (onlyOne) break;
    }
}

void CThreadPoolExecutor::InterruptIdleWorkers()
{
    InterruptIdleWorkers(FALSE);
}

void CThreadPoolExecutor::ClearInterruptsForTaskRun()
{
    Int32 value;
    mCtl->Get(&value);
    if (RunStateLessThan(value, STOP) &&
            Thread::Interrupted() &&
            RunStateAtLeast(value, STOP)) {
        Thread::GetCurrentThread()->Interrupt();
    }
}

void CThreadPoolExecutor::Reject(
    /* [in] */ IRunnable* command)
{
    mHandler->RejectedExecution(command, this);
}

Boolean CThreadPoolExecutor::IsRunningOrShutdown(
    /* [in] */ Boolean shutdownOK)
{
    Int32 value;
    mCtl->Get(&value);
    Int32 rs = RunStateOf(value);
    return rs == RUNNING || (rs == SHUTDOWN && shutdownOK);
}

AutoPtr<IList> CThreadPoolExecutor::DrainQueue()
{
    AutoPtr<IBlockingQueue> q = mWorkQueue;
    AutoPtr<IArrayList> taskList;
    CArrayList::New((IArrayList**)&taskList);
    Int32 number;
    assert(0);
    // q->DrainTo(taskList, &number);
    Boolean isEmpty;
    if (q->IsEmpty(&isEmpty), !isEmpty) {
        AutoPtr< ArrayOf<IInterface*> > runnables;
        q->ToArray((ArrayOf<IInterface*>**)&runnables);
        for (Int32 i = 0; i < runnables->GetLength(); i++) {
            AutoPtr<IRunnable> r = IRunnable::Probe((*runnables)[i]);
            Boolean result;
            if (q->Remove(r, &result), result) {
                Boolean b = FALSE;
                taskList->Add(r, &b);
            }
        }
    }
    AutoPtr<IList> res = (IList*)taskList->Probe(EIID_IList);
    return res;
}

Boolean CThreadPoolExecutor::AddWorker(
    /* [in] */ IRunnable* firstTask,
    /* [in] */ Boolean core)
{
RETRY:
    for (;;) {
        Int32 c;
        mCtl->Get(&c);
        Int32 rs = RunStateOf(c);

        // Check if queue empty only if necessary.
        Boolean isEmpty;
        if (rs >= SHUTDOWN &&
            ! (rs == SHUTDOWN &&
               firstTask == NULL &&
               (mWorkQueue->IsEmpty(&isEmpty), !isEmpty))) {
            return FALSE;
        }

        for (;;) {
            Int32 wc = WorkerCountOf(c);
            if (wc >= CAPACITY ||
                wc >= (core ? mCorePoolSize : mMaximumPoolSize)) {
                return FALSE;
            }
            if (CompareAndIncrementWorkerCount(c)) {
                goto NEXT;
            }
            mCtl->Get(&c);  // Re-read ctl
            if (RunStateOf(c) != rs) {
                goto RETRY;
            }
            // else CAS failed due to workerCount change; retry inner loop
        }
    }
NEXT:
    AutoPtr<Worker> w = new Worker(firstTask, this);
    AutoPtr<IThread> t = w->mThread;

    Int32 c;
    {
        Autolock lock(mMainLock);

        // Recheck while holding lock.
        // Back out on ThreadFactory failure or if
        // shut down before lock acquired.
        mCtl->Get(&c);
        Int32 rs = RunStateOf(c);

        if (t == NULL ||
            (rs >= SHUTDOWN &&
             ! (rs == SHUTDOWN &&
                firstTask == NULL))) {
            DecrementWorkerCount();
            TryTerminate();
            return FALSE;
        }

        mWorkers.Insert(w);

        Int32 s = mWorkers.GetSize();
        if (s > mLargestPoolSize) {
            mLargestPoolSize = s;
        }
    }

    t->Start();
    // It is possible (but unlikely) for a thread to have been
    // added to workers, but not yet started, during transition to
    // STOP, which could result in a rare missed interrupt,
    // because Thread.interrupt is not guaranteed to have any effect
    // on a non-yet-started Thread (see Thread#interrupt).
    mCtl->Get(&c);
    Boolean isInterrupted;
    if (RunStateOf(c) == STOP && (t->IsInterrupted(&isInterrupted), !isInterrupted)) {
        t->Interrupt();
    }

    return TRUE;
}

void CThreadPoolExecutor::ProcessWorkerExit(
    /* [in] */ Worker* w,
    /* [in] */ Boolean completedAbruptly)
{
    if (completedAbruptly) { // If abrupt, then workerCount wasn't adjusted
        DecrementWorkerCount();
    }

    {
        Autolock lock(mMainLock);

        mCompletedTaskCount += w->mCompletedTasks;
        mWorkers.Erase(w);
    }

    TryTerminate();

    Int32 c;
    mCtl->Get(&c);
    if (RunStateLessThan(c, STOP)) {
        if (!completedAbruptly) {
            Int32 min = mAllowCoreThreadTimeOut ? 0 : mCorePoolSize;
            Boolean isEmpty;
            if (min == 0 && (mWorkQueue->IsEmpty(&isEmpty), !isEmpty)) {
                min = 1;
            }
            if (WorkerCountOf(c) >= min) {
                return; // replacement not needed
            }
        }
        AddWorker(NULL, FALSE);
    }
}

AutoPtr<IRunnable> CThreadPoolExecutor::GetTask()
{
    Boolean timedOut = FALSE; // Did the last poll() time out?

RETRY:
    for (;;) {
        Int32 c;
        mCtl->Get(&c);
        Int32 rs = RunStateOf(c);

        // Check if queue empty only if necessary.
        Boolean isEmpty;
        if (rs >= SHUTDOWN && (rs >= STOP || (mWorkQueue->IsEmpty(&isEmpty), isEmpty))) {
            DecrementWorkerCount();
            return NULL;
        }

        Boolean timed;      // Are workers subject to culling?

        for (;;) {
            Int32 wc = WorkerCountOf(c);
            timed = mAllowCoreThreadTimeOut || wc > mCorePoolSize;

            if (wc <= mMaximumPoolSize && ! (timedOut && timed)) {
                break;
            }
            if (CompareAndDecrementWorkerCount(c)) {
                return NULL;
            }
            mCtl->Get(&c);  // Re-read ctl
            if (RunStateOf(c) != rs) {
                goto RETRY;
            }
            // else CAS failed due to workerCount change; retry inner loop
        }

        // try {
        ECode ec = NOERROR;
        AutoPtr<IInterface> obj;
        if (timed) {
            AutoPtr<ITimeUnit> nanos = TimeUnit::GetNANOSECONDS();
            ec = mWorkQueue->Poll(mKeepAliveTime, nanos, (IInterface**)&obj);
        }
        else {
            ec = mWorkQueue->Take((IInterface**)&obj);
        }
        if (FAILED(ec)) {
            timedOut = FALSE;
        }
        else {
            AutoPtr<IRunnable> r = IRunnable::Probe(obj);
            if (r != NULL) return r;
            timedOut = TRUE;
        }
    }
}

ECode CThreadPoolExecutor::RunWorker(
    /* [in] */ Worker* w)
{
    AutoPtr<IRunnable> task = w->mFirstTask;
    w->mFirstTask = NULL;
    Boolean completedAbruptly = TRUE;
    ECode ec = NOERROR;
    while (task != NULL || (task = GetTask()) != NULL) {
        w->Lock();
        ClearInterruptsForTaskRun();
        // try {
        BeforeExecute(w->mThread, task);
            // try {
        ec = task->Run();
        AfterExecute(task, ec);
            // } catch (RuntimeException x) {
            //     thrown = x; throw x;
            // } catch (Error x) {
            //     thrown = x; throw x;
            // } catch (Throwable x) {
            //     thrown = x; throw new Error(x);
            // } finally {
            //     afterExecute(task, thrown);
            // }
        // } finally {
        task = NULL;
        w->mCompletedTasks++;
        w->Unlock();
        if (FAILED(ec)) goto EXIT2;
        // }
    }
    completedAbruptly = FALSE;
EXIT2:
    ProcessWorkerExit(w, completedAbruptly);
    return ec;
}

ECode CThreadPoolExecutor::Execute(
    /* [in] */ IRunnable* command)
{
    if (command == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }
    /*
     * Proceed in 3 steps:
     *
     * 1. If fewer than corePoolSize threads are running, try to
     * start a new thread with the given command as its first
     * task.  The call to addWorker atomically checks runState and
     * workerCount, and so prevents false alarms that would add
     * threads when it shouldn't, by returning false.
     *
     * 2. If a task can be successfully queued, then we still need
     * to double-check whether we should have added a thread
     * (because existing ones died since last checking) or that
     * the pool shut down since entry into this method. So we
     * recheck state and if necessary roll back the enqueuing if
     * stopped, or start a new thread if there are none.
     *
     * 3. If we cannot queue task, then we try to add a new
     * thread.  If it fails, we know we are shut down or saturated
     * and so reject the task.
     */
    Int32 c;
    mCtl->Get(&c);
    if (WorkerCountOf(c) < mCorePoolSize) {
        if (AddWorker(command, TRUE)) {
            return NOERROR;
        }
        mCtl->Get(&c);
    }
    Boolean result;
    if (IsRunning(c) && (mWorkQueue->Offer(command, &result), &result)) {
        Int32 recheck;
        mCtl->Get(&recheck);
        if (!IsRunning(recheck) && (Remove(command, &result), result)) {
            Reject(command);
        }
        else if (WorkerCountOf(recheck) == 0) {
            AddWorker(NULL, FALSE);
        }
    }
    else if (!AddWorker(command, FALSE)) {
        Reject(command);
    }
    return NOERROR;
}

ECode CThreadPoolExecutor::Shutdown()
{
    {
        Autolock lock(mMainLock);

        FAIL_RETURN(CheckShutdownAccess());
        AdvanceRunState(SHUTDOWN);
        InterruptIdleWorkers();
        OnShutdown(); // hook for ScheduledThreadPoolExecutor
    }
    TryTerminate();
    return NOERROR;
}

ECode CThreadPoolExecutor::ShutdownNow(
    /* [out] */ IList** tasks)
{
    if (tasks != NULL) {
        *tasks = NULL;
    }

    {
        Autolock lock(mMainLock);

        FAIL_RETURN(CheckShutdownAccess());
        AdvanceRunState(STOP);
        InterruptWorkers();
        AutoPtr<IList> t = DrainQueue();
        if (tasks != NULL) {
            *tasks = t;
            REFCOUNT_ADD(*tasks);
        }
    }

    TryTerminate();
    return NOERROR;
}

ECode CThreadPoolExecutor::IsShutdown(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    Int32 c;
    mCtl->Get(&c);
    *result = !IsRunning(c);
    return NOERROR;
}

ECode CThreadPoolExecutor::IsTerminating(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    Int32 c;
    mCtl->Get(&c);
    *result = !IsRunning(c) && RunStateLessThan(c, TERMINATED);
    return NOERROR;
}

ECode CThreadPoolExecutor::IsTerminated(
    /* [out] */ Boolean* result)
{
    Int32 c;
    mCtl->Get(&c);
    *result = RunStateAtLeast(c, TERMINATED);
    return NOERROR;
}

ECode CThreadPoolExecutor::AwaitTermination(
    /* [in] */ Int64 timeout,
    /* [in] */ ITimeUnit* unit,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    Int64 nanos;
    unit->ToNanos(timeout, &nanos);
    Autolock lock(mMainLock);

    for (;;) {
        Int32 c;
        mCtl->Get(&c);
        if (RunStateAtLeast(c, TERMINATED)) {
            *result = TRUE;
            return NOERROR;
        }
        if (nanos <= 0) {
            *result = FALSE;
            return NOERROR;
        }
        mTermination.WaitNanos(mMainLock, nanos, &nanos);
    }
}

ECode CThreadPoolExecutor::SetThreadFactory(
    /* [in] */ IThreadFactory* threadFactory)
{
    if (threadFactory == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }
    mThreadFactory = threadFactory;
    return NOERROR;
}

ECode CThreadPoolExecutor::GetThreadFactory(
    /* [out] */ IThreadFactory** threadFactory)
{
    VALIDATE_NOT_NULL(threadFactory);
    *threadFactory = mThreadFactory;
    REFCOUNT_ADD(*threadFactory);
    return NOERROR;
}

ECode CThreadPoolExecutor::SetRejectedExecutionHandler(
    /* [in] */ IRejectedExecutionHandler* handler)
{
    if (handler == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }
    mHandler = handler;
    return NOERROR;
}

ECode CThreadPoolExecutor::GetRejectedExecutionHandler(
    /* [out] */ IRejectedExecutionHandler** handler)
{
    VALIDATE_NOT_NULL(handler);
    *handler = mHandler;
    REFCOUNT_ADD(*handler);
    return NOERROR;
}

ECode CThreadPoolExecutor::SetCorePoolSize(
    /* [in] */ Int32 corePoolSize)
{
    if (corePoolSize < 0) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Int32 delta = corePoolSize - mCorePoolSize;
    mCorePoolSize = corePoolSize;
    Int32 c;
    mCtl->Get(&c);
    if (WorkerCountOf(c) > corePoolSize) {
        InterruptIdleWorkers();
    }
    else if (delta > 0) {
        // We don't really know how many new threads are "needed".
        // As a heuristic, prestart enough new workers (up to new
        // core size) to handle the current number of tasks in
        // queue, but stop if queue becomes empty while doing so.
        Int32 size;
        mWorkQueue->GetSize(&size);
        Int32 k = Elastos::Core::Math::Min(delta, size);
        while (k-- > 0 && AddWorker(NULL, TRUE)) {
            Boolean isEmpty;
            if (mWorkQueue->IsEmpty(&isEmpty), isEmpty) {
                break;
            }
        }
    }
    return NOERROR;
}

ECode CThreadPoolExecutor::GetCorePoolSize(
    /* [out] */ Int32* corePoolSize)
{
    VALIDATE_NOT_NULL(corePoolSize);
    *corePoolSize = mCorePoolSize;
    return NOERROR;
}

ECode CThreadPoolExecutor::PrestartCoreThread(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    Int32 c;
    mCtl->Get(&c);
    *result = WorkerCountOf(c) < mCorePoolSize &&
            AddWorker(NULL, TRUE);
    return NOERROR;
}

void CThreadPoolExecutor::EnsurePrestart()
{
    Int32 c;
    mCtl->Get(&c);
    Int32 wc = WorkerCountOf(c);
    if (wc < mCorePoolSize) {
        AddWorker(NULL, TRUE);
    }
    else if (wc == 0) {
        AddWorker(NULL, FALSE);
    }
}

ECode CThreadPoolExecutor::PrestartAllCoreThreads(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    Int32 n = 0;
    while (AddWorker(NULL, TRUE)) {
        ++n;
    }
    *number = n;
    return NOERROR;
}

ECode CThreadPoolExecutor::AllowsCoreThreadTimeOut(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mAllowCoreThreadTimeOut;
    return NOERROR;
}

ECode CThreadPoolExecutor::AllowCoreThreadTimeOut(
    /* [in] */ Boolean value)
{
    if (value && mKeepAliveTime <= 0) {
        // throw new IllegalArgumentException("Core threads must have nonzero keep alive times");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (value != mAllowCoreThreadTimeOut) {
        mAllowCoreThreadTimeOut = value;
        if (value) {
            InterruptIdleWorkers();
        }
    }
    return NOERROR;
}

ECode CThreadPoolExecutor::SetMaximumPoolSize(
    /* [in] */ Int32 maximumPoolSize)
{
    if (maximumPoolSize <= 0 || maximumPoolSize < mCorePoolSize) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mMaximumPoolSize = maximumPoolSize;
    Int32 c;
    mCtl->Get(&c);
    if (WorkerCountOf(c) > maximumPoolSize) {
        InterruptIdleWorkers();
    }
    return NOERROR;
}

ECode CThreadPoolExecutor::GetMaximumPoolSize(
    /* [out] */ Int32* maximumPoolSize)
{
    VALIDATE_NOT_NULL(maximumPoolSize);
    *maximumPoolSize = mMaximumPoolSize;
    return NOERROR;
}

ECode CThreadPoolExecutor::SetKeepAliveTime(
    /* [in] */ Int64 time,
    /* [in] */ ITimeUnit* unit)
{
    if (time < 0) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Boolean result;
    if (time == 0 && (AllowsCoreThreadTimeOut(&result), result)) {
        // throw new IllegalArgumentException("Core threads must have nonzero keep alive times");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Int64 keepAliveTime;
    unit->ToNanos(time, &keepAliveTime);
    Int64 delta = keepAliveTime - mKeepAliveTime;
    mKeepAliveTime = keepAliveTime;
    if (delta < 0) {
        InterruptIdleWorkers();
    }
    return NOERROR;
}

ECode CThreadPoolExecutor::GetKeepAliveTime(
    /* [in] */ ITimeUnit* unit,
    /* [out] */ Int64* time)
{
    VALIDATE_NOT_NULL(time);
    AutoPtr<ITimeUnit> nanos = TimeUnit::GetNANOSECONDS();
    return unit->Convert(mKeepAliveTime, nanos, time);
}

ECode CThreadPoolExecutor::GetQueue(
    /* [out] */ IBlockingQueue** queue)
{
    VALIDATE_NOT_NULL(queue);
    *queue = mWorkQueue;
    REFCOUNT_ADD(*queue);
    return NOERROR;
}

ECode CThreadPoolExecutor::Remove(
    /* [in] */ IRunnable* task,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    ECode ec = mWorkQueue->Remove(task, result);
    TryTerminate(); // In case SHUTDOWN and now empty
    return ec;
}

ECode CThreadPoolExecutor::Purge()
{
    AutoPtr<IBlockingQueue> q = mWorkQueue;
    // try {
    AutoPtr<IIterator> it;
    q->GetIterator((IIterator**)&it);
    Boolean hasNext;
    while (it->HasNext(&hasNext), hasNext) {
        AutoPtr<IInterface> obj;
        it->GetNext((IInterface**)&obj);
        IRunnable* r = IRunnable::Probe(obj);
        Boolean isCancelled;
        if (IFuture::Probe(r) &&
                (IFuture::Probe(r)->IsCancelled(&isCancelled), isCancelled)) {
            it->Remove();
        }
    }
    // } catch (ConcurrentModificationException fallThrough) {
    //     // Take slow path if we encounter interference during traversal.
    //     // Make copy for traversal and call remove for cancelled entries.
    //     // The slow path is more likely to be O(N*N).
    //     for (Object r : q.toArray())
    //         if (r instanceof Future<?> && ((Future<?>)r).isCancelled())
    //             q.remove(r);
    // }

    TryTerminate(); // In case SHUTDOWN and now empty
    return NOERROR;
}

ECode CThreadPoolExecutor::GetPoolSize(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    Autolock lock(mMainLock);
    // Remove rare and surprising possibility of
    // isTerminated() && getPoolSize() > 0
    Int32 c;
    mCtl->Get(&c);
    *number = RunStateAtLeast(c, TIDYING) ? 0 : mWorkers.GetSize();
    return NOERROR;
}

ECode CThreadPoolExecutor::GetActiveCount(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    Autolock lock(mMainLock);
    Int32 n = 0;
    HashSet< AutoPtr<Worker>, HashWorker >::Iterator it;
    for (it = mWorkers.Begin(); it != mWorkers.End(); ++it) {
        AutoPtr<Worker> w = *it;
        if (w->IsLocked()) {
            ++n;
        }
    }
    *number = n;
    return NOERROR;
}

ECode CThreadPoolExecutor::GetLargestPoolSize(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    Autolock lock(mMainLock);
    *number = mLargestPoolSize;
    return NOERROR;
}

ECode CThreadPoolExecutor::GetTaskCount(
    /* [out] */ Int64* number)
{
    VALIDATE_NOT_NULL(number);
    Autolock lock(mMainLock);
    Int64 n = mCompletedTaskCount;
    HashSet< AutoPtr<Worker>, HashWorker >::Iterator it;
    for (it = mWorkers.Begin(); it != mWorkers.End(); ++it) {
        AutoPtr<Worker> w = *it;
        n += w->mCompletedTasks;
        if (w->IsLocked()) {
            ++n;
        }
    }
    Int32 size;
    mWorkQueue->GetSize(&size);
    *number = n + size;
    return NOERROR;
}

ECode CThreadPoolExecutor::GetCompletedTaskCount(
    /* [out] */ Int64* number)
{
    VALIDATE_NOT_NULL(number);
    Autolock lock(mMainLock);
    Int64 n = mCompletedTaskCount;
    HashSet< AutoPtr<Worker>, HashWorker >::Iterator it;
    for (it = mWorkers.Begin(); it != mWorkers.End(); ++it) {
        AutoPtr<Worker> w = *it;
        n += w->mCompletedTasks;
    }
    *number = n;
    return NOERROR;
}

ECode CThreadPoolExecutor::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    // long ncompleted;
    // int nworkers, nactive;
    // final ReentrantLock mainLock = this.mainLock;
    // mainLock.lock();
    // try {
    //     ncompleted = completedTaskCount;
    //     nactive = 0;
    //     nworkers = workers.size();
    //     for (Worker w : workers) {
    //         ncompleted += w.completedTasks;
    //         if (w.isLocked())
    //             ++nactive;
    //     }
    // } finally {
    //     mainLock.unlock();
    // }
    // int c = ctl.get();
    // String rs = (runStateLessThan(c, SHUTDOWN) ? "Running" :
    //              (runStateAtLeast(c, TERMINATED) ? "Terminated" :
    //               "Shutting down"));
    // return super.toString() +
    //     "[" + rs +
    //     ", pool size = " + nworkers +
    //     ", active threads = " + nactive +
    //     ", queued tasks = " + workQueue.size() +
    //     ", completed tasks = " + ncompleted +
    //     "]";
    assert(0);
    return E_NOT_IMPLEMENTED;
}

} // namespace Concurrent
} // namespace Utility
} // namespace Elastos
