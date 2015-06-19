
#include "Executors.h"
#include <Thread.h>
#include <StringBuilder.h>
#include <StringUtils.h>
#include "CThread.h"
#include "CAtomicInteger32.h"

using Elastos::Core::StringBuilder;
using Elastos::Core::StringUtils;
using Elastos::Core::CThread;
using Elastos::Core::Thread;
using Elastos::Utility::Concurrent::Atomic::CAtomicInteger32;

namespace Elastos {
namespace Utility {
namespace Concurrent {

//==============================================
//    Executors::RunnableAdapter
//===============================================
CAR_INTERFACE_IMPL(Executors::RunnableAdapter, Object, ICallable)

ECode Executors::RunnableAdapter::Call(
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);
    ECode ec = mTask->Run();
    *result = mResult;
    REFCOUNT_ADD(*result);
    return ec;
}


//==============================================
//    Executors::DefaultThreadFactory
//===============================================
static AutoPtr<IAtomicInteger32> InitsPoolNumber()
{
    AutoPtr<CAtomicInteger32> ai;
    CAtomicInteger32::NewByFriend(1, (CAtomicInteger32**)&ai);
    return (IAtomicInteger32*)ai.Get();
}

const AutoPtr<IAtomicInteger32> Executors::DefaultThreadFactory::sPoolNumber = InitsPoolNumber();

CAR_INTERFACE_IMPL(Executors::DefaultThreadFactory, Object, IThreadFactory)

Executors::DefaultThreadFactory::DefaultThreadFactory()
{
    CAtomicInteger32::New(1, (IAtomicInteger32**)&mThreadNumber);
    // SecurityManager s = System.getSecurityManager();
    // group = (s != null) ? s.getThreadGroup() :
    //                       Thread.currentThread().getThreadGroup();
    Thread::GetCurrentThread()->GetThreadGroup((IThreadGroup**)&mGroup);
    Int32 c;
    sPoolNumber->GetAndIncrement(&c);
    StringBuilder sb;
    sb += "pool-";
    sb += c;
    sb += "-thread-";
    mNamePrefix = sb.ToString();
}

ECode Executors::DefaultThreadFactory::NewThread(
    /* [in] */ IRunnable* r,
    /* [out] */ IThread** thread)
{
    VALIDATE_NOT_NULL(thread);
    Int32 c;
    mThreadNumber->GetAndIncrement(&c);
    FAIL_RETURN(CThread::New(mGroup, r,
            mNamePrefix + StringUtils::ToString(c), 0, thread));
    Boolean isDaemon;
    if ((*thread)->IsDaemon(&isDaemon), isDaemon) {
        (*thread)->SetDaemon(FALSE);
    }
    Int32 priority;
    if ((*thread)->GetPriority(&priority), priority != IThread::NORM_PRIORITY) {
        (*thread)->SetPriority(IThread::NORM_PRIORITY);
    }
    return NOERROR;
}

//==============================================
//    Executors::PrivilegedCallable
//===============================================
Executors::PrivilegedCallable::PrivilegedCallable(
    /* [in] */ ICallable* task)
{
    mTask = task;
//    this.acc = AccessController.getContext();
}

ECode Executors::PrivilegedCallable::Call(
    /* [out] */ IInterface** result)
{
    // try {
    //     return AccessController.doPrivileged(
    //         new PrivilegedExceptionAction<T>() {
    //             public T run() throws Exception {
    //                 return task.call();
    //             }
    //         }, acc);
    // } catch (PrivilegedActionException e) {
    //     throw e.getException();
    // }
    return NOERROR;
}

//==============================================
//    Executors::PrivilegedCallableUsingCurrentClassLoader::
//===============================================
Executors::PrivilegedCallableUsingCurrentClassLoader::PrivilegedCallableUsingCurrentClassLoader(
    /* [in] */ ICallable* task)
{
    // BEGIN android-removed
    // SecurityManager sm = System.getSecurityManager();
    // if (sm != null) {
    //     // Calls to getContextClassLoader from this class
    //     // never trigger a security check, but we check
    //     // whether our callers have this permission anyways.
    //     sm.checkPermission(SecurityConstants.GET_CLASSLOADER_PERMISSION);
    //
    //     // Whether setContextClassLoader turns out to be necessary
    //     // or not, we fail fast if permission is not available.
    //     sm.checkPermission(new RuntimePermission("setContextClassLoader"));
    // }
    // END android-removed
    mTask = task;
//    this.acc = AccessController.getContext();
//    this.ccl = Thread.currentThread().getContextClassLoader();
}

ECode Executors::PrivilegedCallableUsingCurrentClassLoader::Call(
    /* [out] */ IInterface** result)
{
    // try {
    //     return AccessController.doPrivileged(
    //         new PrivilegedExceptionAction<T>() {
    //             public T run() throws Exception {
    //                 Thread t = Thread.currentThread();
    //                 ClassLoader cl = t.getContextClassLoader();
    //                 if (ccl == cl) {
    //                     return task.call();
    //                 } else {
    //                     t.setContextClassLoader(ccl);
    //                     try {
    //                         return task.call();
    //                     } finally {
    //                         t.setContextClassLoader(cl);
    //                     }
    //                 }
    //             }
    //         }, acc);
    // } catch (PrivilegedActionException e) {
    //     throw e.getException();
    // }
    return NOERROR;
}

//==============================================
//    Executors::PrivilegedThreadFactory::
//===============================================
Executors::PrivilegedThreadFactory::PrivilegedThreadFactory()
{
    // BEGIN android-removed
    // SecurityManager sm = System.getSecurityManager();
    // if (sm != null) {
    //     // Calls to getContextClassLoader from this class
    //     // never trigger a security check, but we check
    //     // whether our callers have this permission anyways.
    //     sm.checkPermission(SecurityConstants.GET_CLASSLOADER_PERMISSION);
    //
    //     // Fail fast
    //     sm.checkPermission(new RuntimePermission("setContextClassLoader"));
    // }
    // END android-removed
//    this.acc = AccessController.getContext();
//    this.ccl = Thread.currentThread().getContextClassLoader();
}

ECode Executors::PrivilegedThreadFactory::NewThread(
    /* [in] */ IRunnable* r,
    /* [out] */ IThread** thread)
{
    // return super.newThread(new Runnable() {
    //     public void run() {
    //         AccessController.doPrivileged(new PrivilegedAction<Void>() {
    //             public Void run() {
    //                 Thread.currentThread().setContextClassLoader(ccl);
    //                 r.run();
    //                 return null;
    //             }
    //         }, acc);
    //     }
    // });
    return NOERROR;
}

//==============================================
//    Executors::DelegatedExecutorService::
//===============================================
Executors::DelegatedExecutorService::DelegatedExecutorService(
    /* [in] */ IExecutorService* executor)
{
    mE = executor;
}

ECode Executors::DelegatedExecutorService::Execute(
    /* [in] */ IRunnable* command)
{
    return (IExecutor::Probe(mE))->Execute(command);
}

void Executors::DelegatedExecutorService::Shutdown()
{
    mE->Shutdown();
}

AutoPtr<IList> Executors::DelegatedExecutorService::ShutdownNow()
{
    AutoPtr<IList> res;
    mE->ShutdownNow((IList**)&res);
    return res;
}

Boolean Executors::DelegatedExecutorService::IsShutdown()
{
    Boolean res = FALSE;
    mE->IsShutdown(&res);
    return res;
}

Boolean Executors::DelegatedExecutorService::IsTerminated()
{
    Boolean res = FALSE;
    mE->IsTerminated(&res);
    return res;
}

Boolean Executors::DelegatedExecutorService::AwaitTermination(
    /* [in] */ Int64 timeout,
    /* [in] */ ITimeUnit* unit)
{
    Boolean res = FALSE;
    mE->AwaitTermination(timeout, unit, &res);
    return res;
}

ECode Executors::DelegatedExecutorService::Submit(
    /* [in] */ IRunnable* task,
    /* [out] */ IFuture** future)
{
    return mE->Submit(task, future);
}

ECode Executors::DelegatedExecutorService::Submit(
    /* [in] */ ICallable* task,
    /* [out] */ IFuture** future)
{
    return mE->Submit(task, future);
}

ECode Executors::DelegatedExecutorService::Submit(
    /* [in] */ IRunnable* task,
    /* [in] */ IInterface* result,
    /* [out] */ IFuture** future)
{
    return mE->Submit(task, result, future);
}

ECode Executors::DelegatedExecutorService::InvokeAll(
    /* [in] */ ICollection* tasks,
    /* [out] */ IList** futures)
{
    return mE->InvokeAll(tasks, futures);
}

ECode Executors::DelegatedExecutorService::InvokeAll(
    /* [in] */ ICollection* tasks,
    /* [in] */ Int64 timeout,
    /* [in] */ ITimeUnit* unit,
    /* [out] */ IList** futures)
{
    return mE->InvokeAll(tasks, timeout, unit, futures);
}

ECode Executors::DelegatedExecutorService::InvokeAny(
    /* [in] */ ICollection* tasks,
    /* [out] */ IInterface** result)
{
    return mE->InvokeAny(tasks, result);
}

ECode Executors::DelegatedExecutorService::InvokeAny(
    /* [in] */ ICollection* tasks,
    /* [in] */ Int64 timeout,
    /* [in] */ ITimeUnit* unit,
    /* [out] */ IInterface** result)
{
    return mE->InvokeAny(tasks, timeout, unit, result);
}

//==============================================
//    Executors::FinalizableDelegatedExecutorService::
//===============================================
Executors::FinalizableDelegatedExecutorService::FinalizableDelegatedExecutorService(
    /* [in] */ IExecutorService* executor) : DelegatedExecutorService(executor)
{
}

void Executors::FinalizableDelegatedExecutorService::Finalize()
{
//    super.shutdown();
}

//==============================================
//    Executors::DelegatedScheduledExecutorService::
//===============================================

// Executors::DelegatedScheduledExecutorService::DelegatedScheduledExecutorService(
//      [in]  IScheduledExecutorService* executor)
// {
//     super(executor);
//     mE = executor;
// }

// ECode Executors::DelegatedScheduledExecutorService::Schedule(
//     /* [in] */ IRunnable* command,
//     /* [in] */ Int64 delay,
//     /* [in] */ ITimeUnit* unit,
//     /* [out] */ IScheduledFuture** result)
// {
//     return mE->Schedule(command, delay, unit);
// }

// ECode Executors::DelegatedScheduledExecutorService::Schedule(
//     /* [in] */ ICallable* callable,
//     /* [in] */ Int64 delay,
//     /* [in] */ ITimeUnit* unit,
//     /* [out] */ IScheduledFuture** result)
// {
//     return mE->Schedule(callable, delay, unit);
// }

// ECode Executors::DelegatedScheduledExecutorService::ScheduleAtFixedRate(
//     /* [in] */ IRunnable* command,
//     /* [in] */ Int64 initialDelay,
//     /* [in] */ Int64 period,
//     /* [in] */ ITimeUnit* unit,
//     /* [out] */ IScheduledFuture** result)
// {
//     return mE->ScheduleAtFixedRate(command, initialDelay, period, unit);
// }

// ECode Executors::DelegatedScheduledExecutorService::ScheduleWithFixedDelay(
//     /* [in] */ IRunnable* command,
//     /* [in] */ Int64 initialDelay,
//     /* [in] */ Int64 delay,
//     /* [in] */ ITimeUnit* unit,
//     /* [out] */ IScheduledFuture** result)
// {
//     return mE->ScheduleWithFixedDelay(command, initialDelay, delay, unit);
// }

//==============================================
//    Executors::
//===============================================
AutoPtr<IThreadFactory> Executors::GetDefaultThreadFactory()
{
    return new DefaultThreadFactory();
}

ECode Executors::Callable(
    /* [in] */ IRunnable* task,
    /* [in] */ IInterface* result,
    /* [out] */ ICallable** object)
{
    if (task == NULL) return E_NULL_POINTER_EXCEPTION;
    *object = new RunnableAdapter(task, result);
    REFCOUNT_ADD(*object);
    return NOERROR;
}

ECode Executors::NewFixedThreadPool(
    /* [in] */ Int32 nThreads,
    /* [out] */ IExecutorService** result)
{
    // return new ThreadPoolExecutor(nThreads, nThreads,
    //                               0L, TimeUnit.MILLISECONDS,
    //                               new LinkedBlockingQueue<Runnable>());
    return NOERROR;
}

ECode Executors::NewWorkStealingPool(
    /* [in] */ Int32 parallelism,
    /* [out] */ IExecutorService** result)
{
    // return new ForkJoinPool
    //     (parallelism,
    //      ForkJoinPool.defaultForkJoinWorkerThreadFactory,
    //      null, true);
    return NOERROR;
}

ECode Executors::NewWorkStealingPool(
    /* [out] */ IExecutorService** result)
{
    // return new ForkJoinPool
    //     (Runtime.getRuntime().availableProcessors(),
    //      ForkJoinPool.defaultForkJoinWorkerThreadFactory,
    //      null, true);
    return NOERROR;
}

ECode Executors::NewFixedThreadPool(
    /* [in] */ Int32 nThreads,
    /* [in] */ IThreadFactory* threadFactory,
    /* [out] */ IExecutorService** result)
{
    // return new ThreadPoolExecutor(nThreads, nThreads,
    //                               0L, TimeUnit.MILLISECONDS,
    //                               new LinkedBlockingQueue<Runnable>(),
    //                               threadFactory);
    return NOERROR;
}

ECode Executors::NewSingleThreadExecutor(
    /* [out] */ IExecutorService** result)
{
    // return new FinalizableDelegatedExecutorService
    //     (new ThreadPoolExecutor(1, 1,
    //                             0L, TimeUnit.MILLISECONDS,
    //                             new LinkedBlockingQueue<Runnable>()));
    return NOERROR;
}

ECode Executors::NewSingleThreadExecutor(
    /* [in] */ IThreadFactory* threadFactory,
    /* [out] */ IExecutorService** result)
{
    // return new FinalizableDelegatedExecutorService
    //     (new ThreadPoolExecutor(1, 1,
    //                             0L, TimeUnit.MILLISECONDS,
    //                             new LinkedBlockingQueue<Runnable>(),
    //                             threadFactory));
    return NOERROR;
}

ECode Executors::NewCachedThreadPool(
    /* [out] */ IExecutorService** result)
{
    // return new ThreadPoolExecutor(0, Integer.MAX_VALUE,
    //                               60L, TimeUnit.SECONDS,
    //                               new SynchronousQueue<Runnable>());
    return NOERROR;
}

ECode Executors::NewCachedThreadPool(
    /* [in] */ IThreadFactory* threadFactory,
    /* [out] */ IExecutorService** result)
{
    // return new ThreadPoolExecutor(0, Integer.MAX_VALUE,
    //                               60L, TimeUnit.SECONDS,
    //                               new SynchronousQueue<Runnable>(),
    //                               threadFactory);
    return NOERROR;
}

// ECode Executors::NewSingleThreadScheduledExecutor(
//     /* [out] */ IScheduledExecutorService** result)
// {
//     // return new DelegatedScheduledExecutorService
//     //     (new ScheduledThreadPoolExecutor(1));
//     return NOERROR;
// }

// ECode Executors::NewSingleThreadScheduledExecutor(
//     /* [in] */ IThreadFactory* threadFactory,
//     /* [out] */ IScheduledExecutorService** result)
// {
//     // return new DelegatedScheduledExecutorService
//     //     (new ScheduledThreadPoolExecutor(1, threadFactory));
//     return NOERROR;
// }

// ECode Executors::NewScheduledThreadPool(
//     /* [in] */ Int32 corePoolSize,
//     /* [out] */ IScheduledExecutorService** result)
// {
// //    return new ScheduledThreadPoolExecutor(corePoolSize);
//     return NOERROR;
// }

// ECode Executors::NewScheduledThreadPool(
//     /* [in] */ Int32 corePoolSize,
//     /* [in] */ IThreadFactory* threadFactory,
//     /* [out] */ IScheduledExecutorService** result)
// {
// //    return new ScheduledThreadPoolExecutor(corePoolSize, threadFactory);
//     return NOERROR;
// }

ECode Executors::UnconfigurableExecutorService(
    /* [in] */ IExecutorService* executor,
    /* [out] */ IExecutorService** result)
{
    // if (executor == null)
    //     throw new NullPointerException();
    // return new DelegatedExecutorService(executor);
    return NOERROR;
}

// ECode Executors::UnconfigurableScheduledExecutorService(
//     /* [in] */ IScheduledExecutorService* executor,
//     /* [out] */ IScheduledExecutorService** result)
// {
//     // if (executor == null)
//     //     throw new NullPointerException();
//     // return new DelegatedScheduledExecutorService(executor);
//     return NOERROR;
// }

ECode Executors::PrivilegedThreadFactory(
    /* [out] */ IThreadFactory** result)
{
//    return new PrivilegedThreadFactory();
    return NOERROR;
}

ECode Executors::Callable(
    /* [in] */ IRunnable* task,
    /* [out] */ ICallable** result)
{
    // if (task == null)
    //     throw new NullPointerException();
    // return new RunnableAdapter<Object>(task, null);
    return NOERROR;
}

ECode Executors::Callable(
    /* [in] */ IPrivilegedAction* action,
    /* [out] */ ICallable** result)
{
    // if (action == null)
    //     throw new NullPointerException();
    // return new Callable<Object>() {
    //     public Object call() { return action.run(); }};
    return NOERROR;
}

ECode Executors::Callable(
    /* [in] */ IPrivilegedExceptionAction* action,
    /* [out] */ ICallable** result)
{
    // if (action == null)
    //     throw new NullPointerException();
    // return new Callable<Object>() {
    //     public Object call() throws Exception { return action.run(); }};
    return NOERROR;
}

ECode Executors::PrivilegedCallable(
    /* [in] */ ICallable* callable,
    /* [out] */ ICallable** result)
{
    // if (callable == null)
    //     throw new NullPointerException();
    // return new PrivilegedCallable<T>(callable);
    return NOERROR;
}

ECode Executors::PrivilegedCallableUsingCurrentClassLoader(
    /* [in] */ ICallable* callable,
    /* [out] */ ICallable** result)
{
    // if (callable == null)
    //     throw new NullPointerException();
    // return new PrivilegedCallableUsingCurrentClassLoader<T>(callable);
    return NOERROR;
}

} // namespace Concurrent
} // namespace Utility
} // namespace Elastos
