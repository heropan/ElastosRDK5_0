
#include "ForkJoinTask.h"
#include <Thread.h>
#include "CForkJoinWorkerThread.h"
#include "CSystem.h"

using Elastos::Core::ISystem;
using Elastos::Core::Thread;
using Elastos::Core::IThread;
using Elastos::Utility::Concurrent::CForkJoinWorkerThread;

namespace Elastos {
namespace Utility {
namespace Concurrent {

//====================================================================
// ForkJoinTask::
//====================================================================
Int32 ForkJoinTask::NORMAL      = -1;
Int32 ForkJoinTask::CANCELLED   = -2;
Int32 ForkJoinTask::EXCEPTIONAL = -3;
Int32 ForkJoinTask::SIGNAL      =  1;

CAR_INTERFACE_IMPL_3(ForkJoinTask, Object, IForkJoinTask, IFuture, ISerializable)

Int32 ForkJoinTask::SetCompletion(
    /* [in] */ const Int32& completion)
{
    for (Int32 s;;) {
        if ((s = mStatus) < 0)
            return s;
        assert(0 && "TODO");
        // if (UNSAFE.compareAndSwapInt(this, statusOffset, s, completion)) {
        //     if (s != 0)
        //         synchronized (this) { NotifyAll(); }
        //     return completion;
        // }
    }
}

ECode ForkJoinTask::TryAwaitDone(
    /* [in] */ Int64 millis)
{
    assert(0 && "TODO");
    // Int32 s;
    // if (((s = mStatus) > 0 ||
    //      (s == 0 &&
    //       UNSAFE.compareAndSwapInt(this, statusOffset, 0, SIGNAL))) &&
    //     mStatus > 0) {
    //     synchronized (this) {
    //         if (mStatus > 0)
    //             Wait(millis);
    //     }
    // }
    return NOERROR;
}

Int32 ForkJoinTask::ExternalAwaitDone()
{
    Int32 s;
    if ((s = mStatus) >= 0) {
        Boolean interrupted = FALSE;
        {
            Object::Autolock lock(this);
            while ((s = mStatus) >= 0) {
                if (s == 0) {
                    assert(0 && "TODO");
                //     UNSAFE.compareAndSwapInt(this, statusOffset,
                //                              0, SIGNAL);
                }
                else {
                    // ECode ec = Wait();
                    // if (ec != NOERROR) {
                    //     interrupted = TRUE;
                    // }
                }
            }
        }
        if (interrupted)
            Thread::GetCurrentThread()->Interrupt();
    }
    return s;
}

Int32 ForkJoinTask::ExternalInterruptibleAwaitDone(
    /* [in] */ const Int64& millis)
{
    Int32 s = 0;
    if (Thread::Interrupted())
        return s;
//        throw new InterruptedException();
    if ((s = mStatus) >= 0) {
        Object::Autolock lock(this);
        while ((s = mStatus) >= 0) {
            if (s == 0) {
                assert(0 && "TODO");
                // UNSAFE.compareAndSwapInt(this, statusOffset,
                //                          0, SIGNAL);
            }
            else {
                // Wait(millis);
                // if (millis > 0L)
                //     break;
            }
        }
    }
    return s;
}

ECode ForkJoinTask::DoExec()
{
    if (mStatus >= 0) {
        Boolean completed = FALSE;
//        try {
            ECode ec = Exec(&completed);
        // } catch (Throwable rex) {
        //     setExceptionalCompletion(rex);
            if (ec != NOERROR) {
                return ec;
            }
//        }
        if (completed)
            SetCompletion(NORMAL); // must be outside try block
    }
    return NOERROR;
}

Int32 ForkJoinTask::DoJoin()
{
    AutoPtr<IThread> t;
    AutoPtr<IForkJoinWorkerThread> w;
    Int32 s;
    Boolean completed = FALSE;
    t = Thread::GetCurrentThread();
    if (t->Probe(EIID_IForkJoinWorkerThread) != NULL ) {
        if ((s = mStatus) < 0)
            return s;
        w = (IForkJoinWorkerThread*)t->Probe(EIID_IForkJoinWorkerThread);
        Boolean b = FALSE;
        if ((w->UnpushTask(this, &b), b)) {
//            try {
                ECode ec = Exec(&completed);
                if (FAILED(ec)) {
                    return 0;
                }
//            } catch (Throwable rex) {
//                return setExceptionalCompletion(rex);
//            }
            if (completed)
                return SetCompletion(NORMAL);
        }
        Int32 num = 0;
        w->JoinTask(this, &num);
        return num;
    }
    else
        return ExternalAwaitDone();
}

Int32 ForkJoinTask::DoInvoke()
{
    Int32 s;
    Boolean completed;
    if ((s = mStatus) < 0)
        return s;
//    try {
        ECode ec = Exec(&completed);
        if (FAILED(ec)) {
            return 0;
        }
    // } catch (Throwable rex) {
    //     return SetExceptionalCompletion(rex);
    // }
    if (completed)
        return SetCompletion(NORMAL);
    else
        return DoJoin();
}

AutoPtr<ArrayOf<ForkJoinTask::ExceptionNode*> > ForkJoinTask::sExceptionTable;
AutoPtr<IReentrantLock> ForkJoinTask::sExceptionTableLock;
//AutoPtr<ReferenceQueue> ForkJoinTask::sExceptionTableRefQueue;

Int32 ForkJoinTask::EXCEPTION_MAP_CAPACITY = 32;

//====================================================================
// ForkJoinTask::ExceptionNode::
//====================================================================
ForkJoinTask::ExceptionNode::ExceptionNode(
    /* [in] */ IForkJoinTask* task,
    /* [in] */ IThrowable* ex,
    /* [in] */ ExceptionNode* next)
{
//    super(task, exceptionTableRefQueue);
    mEx = ex;
//    mNext = next;
    Thread::GetCurrentThread()->GetId(&mThrower);
}

//====================================================================
// ForkJoinTask::
//====================================================================

Int32 ForkJoinTask::SetExceptionalCompletion(
    /* [in] */ IThrowable* ex)
{
    AutoPtr<ISystem> sys;
    Elastos::Core::CSystem::AcquireSingleton((ISystem**)&sys);
    Int32 h;
    sys->IdentityHashCode(THIS_PROBE(IInterface), &h);
    AutoPtr<IReentrantLock> lock = sExceptionTableLock;
    lock->Lock();
    ExpungeStaleExceptions();
    AutoPtr<ArrayOf<ExceptionNode*> > t = sExceptionTable;
    Int32 i = h & (t->GetLength() - 1);
    // for (AutoPtr<ExceptionNode> e = (*t)[i]; ; e = e->mNext) {
    //     if (e == NULL) {
    //         (*t)[i] = new ExceptionNode(this, ex, (*t)[i]);
    //         break;
    //     }
    //     if (e->Get() == this) // already present
    //         break;
    // }
    lock->UnLock();
    return SetCompletion(EXCEPTIONAL);
}

void ForkJoinTask::ClearExceptionalCompletion()
{
    AutoPtr<ISystem> sys;
    Elastos::Core::CSystem::AcquireSingleton((ISystem**)&sys);
    Int32 h;
    sys->IdentityHashCode(THIS_PROBE(IInterface), &h);
    AutoPtr<IReentrantLock> lock = sExceptionTableLock;
    lock->Lock();
    AutoPtr<ArrayOf<ExceptionNode*> > t = sExceptionTable;
    Int32 i = h & (t->GetLength() - 1);
    // AutoPtr<ExceptionNode> e = (*t)[i];
    // AutoPtr<ExceptionNode> pred = NULL;
    // while (e != NULL) {
    //     AutoPtr<ExceptionNode> next = e->mNext;
    //     if (e->Get() == this) {
    //         if (pred == NULL)
    //             (*t)[i] = next;
    //         else
    //             pred->mNext = next;
    //         break;
    //     }
    //     pred = e;
    //     e = next;
    // }
    ExpungeStaleExceptions();
    mStatus = 0;
    lock->UnLock();
}

AutoPtr<IThrowable> ForkJoinTask::GetThrowableException()
{
    if (mStatus != EXCEPTIONAL)
        return NULL;
    AutoPtr<ISystem> sys;
    Elastos::Core::CSystem::AcquireSingleton((ISystem**)&sys);
    Int32 h;
    sys->IdentityHashCode(THIS_PROBE(IInterface), &h);
    assert(0 && "TODO");
    // AutoPtr<ExceptionNode> e;
    // AutoPtr<IReentrantLock> lock = sExceptionTableLock;
    // lock->Lock();
    // ExpungeStaleExceptions();
    // AutoPtr<ArrayOf<ExceptionNode*> > t = sExceptionTable;
    // e = (*t)[h & (t->GetLength() - 1)];
    // // while (e != NULL && e->Get() != this)
    // //     e = e->mNext;
    // lock->UnLock();
    // AutoPtr<IThrowable> ex;
    // if (e == NULL || (ex = e->mEx) == NULL)
    //     return NULL;
    // Int64 id = 0;
    // Thread::GetCurrentThread()->GetId(&id);
    // if (e->mThrower != id) {
    //     Class<? extends Throwable> ec = ex->GetClass();
    //     try {
    //         Constructor<?> noArgCtor = null;
    //         Constructor<?>[] cs = ec.getConstructors();// public ctors only
    //         for (Int32 i = 0; i < cs.length; ++i) {
    //             Constructor<?> c = cs[i];
    //             Class<?>[] ps = c.getParameterTypes();
    //             if (ps.length == 0)
    //                 noArgCtor = c;
    //             else if (ps.length == 1 && ps[0] == Throwable.class)
    //                 return (Throwable)(c.newInstance(ex));
    //         }
    //         if (noArgCtor != null) {
    //             Throwable wx = (Throwable)(noArgCtor.newInstance());
    //             wx.initCause(ex);
    //             return wx;
    //         }
    //     } catch (Exception ignore) {
    //     }
    // }
    // return ex;
}

void ForkJoinTask::ExpungeStaleExceptions()
{
    assert(0 && "TODO");
    // for (AutoPtr<IInterface> x; (x = sExceptionTableRefQueue->Poll()) != NULL;) {
    //     if (x->Probe(EIID_ExceptionNode) != NULL) {
    //         AutoPtr<ForkJoinTask> key = ((ExceptionNode*)x)->Get();
    //         AutoPtr<ArrayOf<ExceptionNode*> > t = sExceptionTable;
    //         Int32 i;// = System.identityHashCode(key) & (t.length - 1);
    //         AutoPtr<ExceptionNode> e = (*t)[i];
    //         AutoPtr<ExceptionNode> pred = NULL;
    //         while (e != NULL) {
    //             AutoPtr<ExceptionNode> next = e->mNext;
    //             if (Object::Equals(e, x)) {
    //                 if (pred == NULL)
    //                     (*t)[i] = next;
    //                 else
    //                     pred->mNext = next;
    //                 break;
    //             }
    //             pred = e;
    //             e = next;
    //         }
    //     }
    // }
}

void ForkJoinTask::HelpExpungeStaleExceptions()
{
    AutoPtr<IReentrantLock> lock = sExceptionTableLock;
    Boolean b = FALSE;
    if ((lock->TryLock(&b), b)) {
        ExpungeStaleExceptions();
        lock->UnLock();
    }
}

AutoPtr<IInterface> ForkJoinTask::ReportResult()
{
    Int32 s; AutoPtr<IThrowable> ex;
    if ((s = mStatus) == CANCELLED)
        return NULL;
//        return E_CANCELLATION_EXCEPTION;
    // if (s == EXCEPTIONAL && (ex = GetThrowableException()) != NULL)
    //     SneakyThrow::SneakyThrow(ex); // android-changed
    AutoPtr<IInterface> p;
    GetRawResult((IInterface**)&p);
    return p;
}

ECode ForkJoinTask::Fork(
    /* [out] */ IForkJoinTask** outtask)
{
    VALIDATE_NOT_NULL(outtask);
    AutoPtr<IThread> t = Thread::GetCurrentThread();
    AutoPtr<IForkJoinWorkerThread> ft = (IForkJoinWorkerThread*)t->Probe(EIID_IForkJoinWorkerThread);
    AutoPtr<IForkJoinTask> p = (IForkJoinTask*)this->Probe(EIID_IForkJoinTask);
    ft->PushTask(this);
    *outtask = this;
    REFCOUNT_ADD(*outtask);
    return NOERROR;
}

ECode ForkJoinTask::Join(
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface);
    if (DoJoin() != NORMAL) {
        AutoPtr<IInterface> p = ReportResult();
        *outface = p;
        REFCOUNT_ADD(*outface);
        return NOERROR;
    }
    else
        return GetRawResult(outface);
}

ECode ForkJoinTask::Invoke(
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface);
    if (DoInvoke() != NORMAL) {
        AutoPtr<IInterface> p = ReportResult();
        *outface = p;
        REFCOUNT_ADD(*outface);
        return NOERROR;
    }
    else
        return GetRawResult(outface);
}

ECode ForkJoinTask::Cancel(
    /* [in] */ Boolean mayInterruptIfRunning,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = SetCompletion(CANCELLED) == CANCELLED;
    return NOERROR;
}

ECode ForkJoinTask::CancelIgnoringExceptions()
{
    Boolean b = FALSE;
    return Cancel(FALSE, &b);
}

ECode ForkJoinTask::IsDone(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = mStatus < 0;
    return NOERROR;
}

ECode ForkJoinTask::IsCancelled(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = mStatus == CANCELLED;
    return NOERROR;
}

ECode ForkJoinTask::IsCompletedAbnormally(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value);
    *value = mStatus < NORMAL;
    return NOERROR;
}

ECode ForkJoinTask::IsCompletedNormally(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value);
    *value = mStatus == NORMAL;
    return NOERROR;
}

ECode ForkJoinTask::GetException(
    /* [out] */ IThrowable** res)
{
    VALIDATE_NOT_NULL(res);
    Int32 s = mStatus;
    assert(0 && "TODO");
    // *res = ((s >= NORMAL)    ? NULL :
    //         (s == CANCELLED) ? new CancellationException() :
    //         getThrowableException());
    return NOERROR;
}

ECode ForkJoinTask::CompleteExceptionally(
    /* [in] */ IThrowable* ex)
{
    assert(0 && "TODO");
    // SetExceptionalCompletion((ex instanceof RuntimeException) ||
    //                          (ex instanceof Error) ? ex :
    //                          new RuntimeException(ex));
    return NOERROR;
}

ECode ForkJoinTask::Complete(
    /* [in] */ IInterface* value)
{
//    try {
        SetRawResult(value);
    // } catch (Throwable rex) {
    //     SetExceptionalCompletion(rex);
    //     return;
    // }
    SetCompletion(NORMAL);
    return NOERROR;
}

ECode ForkJoinTask::Get(
    /* [out] */ IInterface** res)
{
    VALIDATE_NOT_NULL(res);
    AutoPtr<IThread> t = Thread::GetCurrentThread();
    Int32 s = ( t->Probe(EIID_IForkJoinWorkerThread) != NULL ) ?
        DoJoin() : ExternalInterruptibleAwaitDone(0L);
    AutoPtr<IThrowable> ex;
    if (s == CANCELLED)
        return E_CANCELLATION_EXCEPTION;
    if (s == EXCEPTIONAL && (ex = GetThrowableException()) != NULL)
        return E_EXECUTION_EXCEPTION;
    return GetRawResult(res);
}

ECode ForkJoinTask::Get(
    /* [in] */ Int64 timeout,
    /* [in] */ ITimeUnit* unit,
    /* [out] */ IInterface** res)
{
    VALIDATE_NOT_NULL(res);
    AutoPtr<IThread> t = Thread::GetCurrentThread();
    if (t->Probe(EIID_IForkJoinWorkerThread) != NULL) {
        AutoPtr<IForkJoinWorkerThread> w = (IForkJoinWorkerThread*) t->Probe(EIID_IForkJoinWorkerThread);
        Int64 nanos;
        unit->ToNanos(timeout, &nanos);
        if (mStatus >= 0) {
            Boolean completed = FALSE, b = FALSE;
            if ((w->UnpushTask(this, &b), b)) {
//                try {
                    Exec(&completed);
                // } catch (Throwable rex) {
                //     SetExceptionalCompletion(rex);
                // }
            }
            if (completed)
                SetCompletion(NORMAL);
            else if (mStatus >= 0 && nanos > 0) {
                AutoPtr<CForkJoinWorkerThread> cw = (CForkJoinWorkerThread*)w.Get();
                cw->mPool->TimedAwaitJoin(this, nanos);
            }
        }
    }
    else {
        Int64 millis;// = unit.toMillis(timeout);
        if (millis > 0)
            ExternalInterruptibleAwaitDone(millis);
    }
    Int32 s = mStatus;
    if (s != NORMAL) {
        AutoPtr<IThrowable> ex;
        if (s == CANCELLED)
            return E_CANCELLATION_EXCEPTION;
        if (s != EXCEPTIONAL)
            return E_TIMEOUT_EXCEPTION;
        if ((ex = GetThrowableException()) != NULL)
            return E_EXECUTION_EXCEPTION;
    }
    return GetRawResult(res);
}

ECode ForkJoinTask::QuietlyJoin()
{
    DoJoin();
    return NOERROR;
}

ECode ForkJoinTask::QuietlyInvoke()
{
    DoInvoke();
    return NOERROR;
}

ECode ForkJoinTask::Reinitialize()
{
    if (mStatus == EXCEPTIONAL)
        ClearExceptionalCompletion();
    else
        mStatus = 0;
    return NOERROR;
}

ECode ForkJoinTask::TryUnfork(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value);
    AutoPtr<IThread> t = Thread::GetCurrentThread();
    AutoPtr<IForkJoinWorkerThread> ft = (IForkJoinWorkerThread*)t->Probe(EIID_IForkJoinWorkerThread);
    return ft->UnpushTask(this, value);
}

void ForkJoinTask::InvokeAll(
    /* [in] */ IForkJoinTask* t1,
    /* [in] */ IForkJoinTask* t2)
{
    AutoPtr<IForkJoinTask> p;
    t2->Fork((IForkJoinTask**)&p);
    AutoPtr<IInterface> p2, p3;
    t1->Invoke((IInterface**)&p2);
    t2->Join((IInterface**)&p3);
}

void ForkJoinTask::InvokeAll(
    /* [in] */ ArrayOf<IForkJoinTask*>* tasks)
{
    AutoPtr<IThrowable> ex = NULL;
    Int32 last = tasks->GetLength() - 1;
    for (Int32 i = last; i >= 0; --i) {
        AutoPtr<IForkJoinTask> t = (*tasks)[i];
        AutoPtr<ForkJoinTask> ct = (ForkJoinTask*)t.Get();
        if (t == NULL) {
            // if (ex == NULL)
            //     ex = new NullPointerException();
        }
        else if (i != 0) {
            AutoPtr<IForkJoinTask> p;
            t->Fork((IForkJoinTask**)&p);
        }
        else if (ct->DoInvoke() < NORMAL && ex == NULL)
            t->GetException((IThrowable**)&ex);
    }
    for (Int32 i = 1; i <= last; ++i) {
        AutoPtr<IForkJoinTask> t = (*tasks)[i];
        AutoPtr<ForkJoinTask> ct = (ForkJoinTask*)t.Get();
        if (t != NULL) {
            if (ex != NULL) {
                Boolean b = FALSE;
                t->Cancel(FALSE, &b);
            }
            else if (ct->DoJoin() < NORMAL)
                t->GetException((IThrowable**)&ex);
        }
    }
    // if (ex != NULL)
    //     SneakyThrow.sneakyThrow(ex); // android-changed
}

AutoPtr<ICollection> ForkJoinTask::InvokeAll(
    /* [in] */ ICollection* tasks)
{
    if ((tasks->Probe(EIID_IRandomAccess) == NULL) || (tasks->Probe(EIID_IList) == NULL)) {
        Int32 size;
        tasks->GetSize(&size);
        AutoPtr<ArrayOf<IForkJoinTask*> > arr = ArrayOf<IForkJoinTask*>::Alloc(size);
        AutoPtr<ArrayOf<IForkJoinTask*> > arrOut;
        tasks->ToArray((ArrayOf<IInterface*>*)arr.Get(), (ArrayOf<IInterface*>**)&arrOut);
        InvokeAll(arrOut);
        return tasks;
    }
    AutoPtr<IList> ts = (IList*)tasks->Probe(EIID_IList);
    AutoPtr<IThrowable> ex;
    Int32 last = 0;
    ts->GetSize(&last);
    last -= 1;
    for (Int32 i = last; i >= 0; --i) {
        AutoPtr<IForkJoinTask> t;
        ts->Get(i, (IForkJoinTask**)&t);
        AutoPtr<ForkJoinTask> ct = (ForkJoinTask*)t.Get();
        if (t == NULL) {
            // if (ex == null)
            //     ex = new NullPointerException();
        }
        else if (i != 0) {
            AutoPtr<IForkJoinTask> p;
            t->Fork((IForkJoinTask**)&p);
        }
        else if (ct->DoInvoke() < NORMAL && ex == NULL)
            t->GetException((IThrowable**)&ex);
    }
    for (Int32 i = 1; i <= last; ++i) {
        AutoPtr<IForkJoinTask> t;
        ts->Get(i, (IForkJoinTask**)&t);
        AutoPtr<ForkJoinTask> ct = (ForkJoinTask*)t.Get();
        if (t != NULL) {
            if (ex != NULL) {
                Boolean b = FALSE;
                t->Cancel(FALSE, &b);
            }
            else if (ct->DoJoin() < NORMAL)
                t->GetException((IThrowable**)&ex);
        }
    }
    // if (ex != NULL)
    //     SneakyThrow.sneakyThrow(ex); // android-changed
    return tasks;
}

void ForkJoinTask::HelpQuiesce()
{
    AutoPtr<IThread> t = Thread::GetCurrentThread();
    AutoPtr<IForkJoinWorkerThread> ft = (IForkJoinWorkerThread*)t->Probe(EIID_IForkJoinWorkerThread);
    ft->HelpQuiescePool();
}

AutoPtr<IForkJoinPool> ForkJoinTask::GetPool()
{
    AutoPtr<IThread> t = Thread::GetCurrentThread();
    AutoPtr<IForkJoinWorkerThread> ft = (IForkJoinWorkerThread*)t->Probe(EIID_IForkJoinWorkerThread);
    AutoPtr<IForkJoinPool> p;
    if (ft != NULL) {
        AutoPtr<CForkJoinWorkerThread> cft = (CForkJoinWorkerThread*)ft.Get();
        p = cft->mPool;
    }
    else
        p = NULL;
    return p;
}

Boolean ForkJoinTask::InForkJoinPool()
{
    AutoPtr<IThread> t = Thread::GetCurrentThread();
    AutoPtr<IForkJoinWorkerThread> ft = (IForkJoinWorkerThread*)t->Probe(EIID_IForkJoinWorkerThread);
    return ft == NULL ? FALSE : TRUE;
}

Int32 ForkJoinTask::GetQueuedTaskCount()
{
    AutoPtr<IThread> t = Thread::GetCurrentThread();
    AutoPtr<IForkJoinWorkerThread> ft = (IForkJoinWorkerThread*)t->Probe(EIID_IForkJoinWorkerThread);
    if (ft != NULL) {
        Int32 s = 0;
        ft->GetQueueSize(&s);
        return s;
    }
    return -1;
}

Int32 ForkJoinTask::GetSurplusQueuedTaskCount()
{
    AutoPtr<IThread> t = Thread::GetCurrentThread();
    AutoPtr<IForkJoinWorkerThread> ft = (IForkJoinWorkerThread*)t->Probe(EIID_IForkJoinWorkerThread);
    if (ft != NULL) {
        Int32 s = 0;
        ft->GetEstimatedSurplusTaskCount(&s);
        return s;
    }
    return -1;
}

AutoPtr<IForkJoinTask> ForkJoinTask::Adapt(
    /* [in] */ IRunnable* runnable)
{
    AutoPtr<AdaptedRunnable> p = new AdaptedRunnable(runnable, NULL);
    AutoPtr<IForkJoinTask> res = (IForkJoinTask*)p->Probe(EIID_IForkJoinTask);
    return res;
}

AutoPtr<IForkJoinTask> ForkJoinTask::Adapt(
    /* [in] */ IRunnable* runnable,
    /* [in] */ IInterface* result)
{
    AutoPtr<AdaptedRunnable> p = new AdaptedRunnable(runnable, result);
    AutoPtr<IForkJoinTask> res = (IForkJoinTask*)p->Probe(EIID_IForkJoinTask);
    return res;
}

AutoPtr<IForkJoinTask> ForkJoinTask::Adapt(
    /* [in] */ ICallable* callable)
{
    AutoPtr<AdaptedCallable> p = new AdaptedCallable(callable);
    AutoPtr<IForkJoinTask> res = (IForkJoinTask*)p->Probe(EIID_IForkJoinTask);
    return res;
}

AutoPtr<IForkJoinTask> ForkJoinTask::PeekNextLocalTask()
{
    AutoPtr<IThread> t = Thread::GetCurrentThread();
    AutoPtr<IForkJoinWorkerThread> ft = (IForkJoinWorkerThread*)t->Probe(EIID_IForkJoinWorkerThread);
    if (ft != NULL) {
        AutoPtr<IForkJoinTask> res;
        ft->PeekTask((IForkJoinTask**)&res);
        return res;
    }
    return NULL;
}

AutoPtr<IForkJoinTask> ForkJoinTask::PollNextLocalTask()
{
    AutoPtr<IThread> t = Thread::GetCurrentThread();
    AutoPtr<IForkJoinWorkerThread> ft = (IForkJoinWorkerThread*)t->Probe(EIID_IForkJoinWorkerThread);
    if (ft != NULL) {
        AutoPtr<IForkJoinTask> res;
        ft->PollLocalTask((IForkJoinTask**)&res);
        return res;
    }
    return NULL;
}

AutoPtr<IForkJoinTask> ForkJoinTask::PollTask()
{
    AutoPtr<IThread> t = Thread::GetCurrentThread();
    AutoPtr<IForkJoinWorkerThread> ft = (IForkJoinWorkerThread*)t->Probe(EIID_IForkJoinWorkerThread);
    if (ft != NULL) {
        AutoPtr<IForkJoinTask> res;
        ft->PollTask((IForkJoinTask**)&res);
        return res;
    }
    return NULL;
}

void ForkJoinTask::WriteObject(
    /* [in] */ IObjectOutputStream* s)
{
    s->DefaultWriteObject();
//    s->WriteObject(GetException());
}

void ForkJoinTask::ReadObject(
    /* [in] */ IObjectInputStream* s)
{
    s->DefaultReadObject();
    AutoPtr<IInterface> ex;// = s->ReadObject();
    if (ex != NULL)
        SetExceptionalCompletion((IThrowable*)ex.Get());
}

//====================================================================
// AdaptedRunnable::
//====================================================================
AdaptedRunnable::AdaptedRunnable(
    /* [in] */ IRunnable* runnable,
    /* [in] */ IInterface* result)
{
    if (runnable == NULL) return;// throw new NullPointerException();
    mRunnable = runnable;
    mResultOnCompletion = result;
}

ECode AdaptedRunnable::GetRawResult(
    /* [out] */ IInterface** outface)
{
    *outface = mResult;
    REFCOUNT_ADD(*outface);
    return NOERROR;
}

ECode AdaptedRunnable::SetRawResult(
    /* [in] */ IInterface* v)
{
    mResult = v;
    return NOERROR;
}

ECode AdaptedRunnable::Exec(
    /* [out] */ Boolean* res)
{
    mRunnable->Run();
    mResult = mResultOnCompletion;
    *res = TRUE;
    return NOERROR;
}

void AdaptedRunnable::Run()
{
    AutoPtr<IInterface> p;
    Invoke((IInterface**)&p);
}

//====================================================================
// AdaptedCallable::
//====================================================================
AdaptedCallable::AdaptedCallable(
    /* [in] */ ICallable* callable)
{
    if (callable == NULL) return;//throw new NullPointerException();
    mCallable = callable;
}

ECode AdaptedCallable::GetRawResult(
    /* [out] */ IInterface** outface)
{
    *outface = mResult;
    REFCOUNT_ADD(*outface);
    return NOERROR;
}

ECode AdaptedCallable::SetRawResult(
    /* [in] */ IInterface* v)
{
    mResult = v;
    return NOERROR;
}

ECode AdaptedCallable::Exec(
    /* [out] */ Boolean* res)
{
    ECode ec = mCallable->Call((IInterface**)&mResult);
    *res = FAILED(ec) ? FALSE : TRUE;
    return NOERROR;
}

void AdaptedCallable::Run()
{
    AutoPtr<IInterface> p;
    Invoke((IInterface**)&p);
}

} // namespace Concurrent
} // namespace Utility
} // namespace Elastos
