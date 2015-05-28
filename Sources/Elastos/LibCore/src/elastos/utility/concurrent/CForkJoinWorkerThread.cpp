
#include "CForkJoinWorkerThread.h"
#include "CForkJoinPool.h"
#include "ForkJoinTask.h"
#include <elastos/ObjectUtils.h>

using Elastos::Core::ObjectUtils;
using Elastos::Utility::Concurrent::CForkJoinPool;
using Elastos::Utility::Concurrent::ForkJoinTask;

namespace Elastos {
namespace Utility {
namespace Concurrent {

//====================================================================
// CForkJoinWorkerThread::
//====================================================================

Int32 CForkJoinWorkerThread::SMASK  = 0xffff;

Int32 CForkJoinWorkerThread::INITIAL_QUEUE_CAPACITY = 1 << 13;

Int32 CForkJoinWorkerThread::MAXIMUM_QUEUE_CAPACITY = 1 << 24; // 16M

Int32 CForkJoinWorkerThread::MAX_HELP = 16;

ECode CForkJoinWorkerThread::constructor(
    /* [in] */ IForkJoinPool* pool)
{
    AutoPtr<CForkJoinPool> cp = (CForkJoinPool*)pool;
    Thread::Init(cp->NextWorkerName());
    mPool = pool;
    Int32 k = cp->RegisterWorker(this);
    mPoolIndex = k;
    mEventCount = ~k & SMASK; // clear wait count
    mLocallyFifo = cp->mLocallyFifo;
    AutoPtr<IThreadUncaughtExceptionHandler> ueh = cp->mUeh;
    if (ueh != NULL)
        Thread::SetUncaughtExceptionHandler(ueh);
    Thread::SetDaemon(TRUE);
    return NOERROR;
}

PInterface CForkJoinWorkerThread::Probe(
    /* [in] */ REIID riid)
{
    return _CForkJoinWorkerThread::Probe(riid);
}

ECode CForkJoinWorkerThread::GetPool(
    /* [out] */ IForkJoinPool** outpool)
{
    VALIDATE_NOT_NULL(outpool);
    *outpool = mPool;
    REFCOUNT_ADD(*outpool);
    return NOERROR;
}

ECode CForkJoinWorkerThread::GetPoolIndex(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    *value = mPoolIndex;
    return NOERROR;
}

Int32 CForkJoinWorkerThread::NextSeed()
{
    Int32 r = mSeed;
    r ^= r << 13;
    r ^= r >> 17;
    r ^= r << 5;
    return mSeed = r;
}

void CForkJoinWorkerThread::OnStart()
{
    mQueue = ArrayOf<IForkJoinTask*>::Alloc(INITIAL_QUEUE_CAPACITY);
    Int32 r;
    CForkJoinPool::mWorkerSeedGenerator->NextInt32(&r);
    mSeed = (r == 0) ? 1 : r; //  must be nonzero
}

void CForkJoinWorkerThread::OnTermination(
    /* [in] */ IThrowable* exception)
{
//    try {
        mTerminate = TRUE;
        CancelTasks();
        AutoPtr<CForkJoinPool> cpool = (CForkJoinPool*)mPool.Get();
        cpool->DeregisterWorker(this, exception);
    // } catch (Throwable ex) {        // Shouldn't ever happen
    //     if (exception == NULL)      // but if so, at least rethrown
    //         exception = ex;
//    } finally {
//        if (exception != NULL)
//            SneakyThrow.sneakyThrow(exception); // android-changed
//    }
}

ECode CForkJoinWorkerThread::Run()
{
    AutoPtr<IThrowable> exception;
//    try {
        OnStart();
        AutoPtr<CForkJoinPool> cpool = (CForkJoinPool*)mPool.Get();
        cpool->Work(this);
    // } catch (Throwable ex) {
    //     exception = ex;
    // } finally {
        OnTermination(exception);
//    }
    return NOERROR;
}

Boolean CForkJoinWorkerThread::CasSlotNull(
    /* [in] */ ArrayOf<IForkJoinTask*>* q,
    /* [in] */ const Int32& i,
    /* [in] */ IForkJoinTask* t)
{
    assert(0 && "TODO");
//    return UNSAFE.compareAndSwapObject(q, (i << ASHIFT) + ABASE, t, NULL);
    return FALSE;
}

void CForkJoinWorkerThread::WriteSlot(
    /* [in] */ ArrayOf<IForkJoinTask*>* q,
    /* [in] */ const Int32& i,
    /* [in] */ IForkJoinTask* t)
{
    assert(0 && "TODO");
//    UNSAFE.putObjectVolatile(q, (i << ASHIFT) + ABASE, t);
}

ECode CForkJoinWorkerThread::PushTask(
    /* [in] */ IForkJoinTask* t)
{
    AutoPtr<ArrayOf<IForkJoinTask*> > q;
    Int32 s, m;
    if ((q = mQueue) != NULL) {    // ignore if queue removed
        assert(0 && "TODO");
//        Int64 u = (((s = mQueueTop) & (m = q->GetLength() - 1)) << ASHIFT) + ABASE;
//        UNSAFE.putOrderedObject(q, u, t);
        mQueueTop = s + 1;         // or use putOrderedInt
        if ((s -= mQueueBase) <= 2) {
            AutoPtr<CForkJoinPool> cpool = (CForkJoinPool*)mPool.Get();
            cpool->SignalWork();
        }
        else if (s == m)
            GrowQueue();
    }
    return NOERROR;
}

void CForkJoinWorkerThread::GrowQueue()
{
    AutoPtr<ArrayOf<IForkJoinTask*> > oldQ = mQueue;
    Int32 size = oldQ != NULL ? oldQ->GetLength() << 1 : INITIAL_QUEUE_CAPACITY;
    if (size > MAXIMUM_QUEUE_CAPACITY)
        return;
//        throw new RejectedExecutionException("Queue capacity exceeded");
    if (size < INITIAL_QUEUE_CAPACITY)
        size = INITIAL_QUEUE_CAPACITY;
    mQueue = ArrayOf<IForkJoinTask*>::Alloc(size);
    AutoPtr<ArrayOf<IForkJoinTask*> > q = mQueue;
    Int32 mask = size - 1;
    Int32 top = mQueueTop;
    Int32 oldMask;
    if (oldQ != NULL && (oldMask = oldQ->GetLength() - 1) >= 0) {
        for (Int32 b = mQueueBase; b != top; ++b) {
            assert(0 && "TODO");
            // Int64 u = ((b & oldMask) << ASHIFT) + ABASE;
            // AutoPtr<IInterface> x = UNSAFE.getObjectVolatile(oldQ, u);
            // if (x != NULL && UNSAFE.compareAndSwapObject(oldQ, u, x, NULL))
            //     UNSAFE.putObjectVolatile
            //         (q, ((b & mask) << ASHIFT) + ABASE, x);
        }
    }
}

ECode CForkJoinWorkerThread::DeqTask(
    /* [out] */ IForkJoinTask** outtask)
{
    VALIDATE_NOT_NULL(outtask);
    AutoPtr<IForkJoinTask> t;
    AutoPtr<ArrayOf<IForkJoinTask*> > q;
    Int32 b, i;
    assert(0 && "TODO");
    if (mQueueTop != (b = mQueueBase) &&
        (q = mQueue) != NULL && // must read q after b
        (i = (q->GetLength() - 1) & b) >= 0 &&
        (t = (*q)[i]) != NULL && mQueueBase == b/* &&
        UNSAFE.compareAndSwapObject(q, (i << ASHIFT) + ABASE, t, NULL)*/) {
        mQueueBase = b + 1;
        *outtask = t;
        REFCOUNT_ADD(*outtask);
        return NOERROR;
    }
    *outtask = NULL;
    return NOERROR;
}

ECode CForkJoinWorkerThread::LocallyDeqTask(
    /* [out] */ IForkJoinTask** outtask)
{
    VALIDATE_NOT_NULL(outtask);
    AutoPtr<IForkJoinTask> t;
    Int32 m, b, i;
    AutoPtr<ArrayOf<IForkJoinTask*> > q = mQueue;
    if (q != NULL && (m = q->GetLength() - 1) >= 0) {
        while (mQueueTop != (b = mQueueBase)) {
            assert(0 && "TODO");
            if ((t = (*q)[i = m & b]) != NULL &&
                mQueueBase == b/* &&
                UNSAFE.compareAndSwapObject(q, (i << ASHIFT) + ABASE,
                                            t, NULL)*/) {
                mQueueBase = b + 1;
                *outtask = t;
                REFCOUNT_ADD(*outtask);
                return NOERROR;
            }
        }
    }
    *outtask = NULL;
    return NOERROR;
}

AutoPtr<IForkJoinTask> CForkJoinWorkerThread::PopTask()
{
    Int32 m;
    AutoPtr<ArrayOf<IForkJoinTask*> > q = mQueue;
    if (q != NULL && (m = q->GetLength() - 1) >= 0) {
        for (Int32 s; (s = mQueueTop) != mQueueBase;) {
            Int32 i = m & --s;
            assert(0 && "TODO");
//            Int64 u = (i << ASHIFT) + ABASE; // raw offset
            AutoPtr<IForkJoinTask> t = (*q)[i];
            if (t == NULL)   // lost to stealer
                break;
            // if (UNSAFE.compareAndSwapObject(q, u, t, NULL)) {
            //     mQueueTop = s; // or putOrderedInt
            //     return t;
            // }
        }
    }
    return NULL;
}

ECode CForkJoinWorkerThread::UnpushTask(
    /* [in] */ IForkJoinTask* t,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value);
    AutoPtr<ArrayOf<IForkJoinTask*> > q;
    Int32 s;
    assert(0 && "TODO");
    if ((q = mQueue) != NULL && (s = mQueueTop) != mQueueBase/* &&
        UNSAFE.compareAndSwapObject
        (q, (((q->GetLength() - 1) & --s) << ASHIFT) + ABASE, t, NULL)*/) {
        mQueueTop = s; // or putOrderedInt
        *value = TRUE;
        return NOERROR;
    }
    *value = FALSE;
    return NOERROR;
}

ECode CForkJoinWorkerThread::PeekTask(
    /* [out] */ IForkJoinTask** outtask)
{
    VALIDATE_NOT_NULL(outtask);
    Int32 m;
    AutoPtr<ArrayOf<IForkJoinTask*> > q = mQueue;
    if (q == NULL || (m = q->GetLength() - 1) < 0) {
        *outtask = NULL;
        return NOERROR;
    }
    Int32 i = mLocallyFifo ? mQueueBase : (mQueueTop - 1);
    *outtask = (*q)[i & m];
    REFCOUNT_ADD(*outtask);
    return NOERROR;
}

ECode CForkJoinWorkerThread::ExecTask(
    /* [in] */ IForkJoinTask* t)
{
    mCurrentSteal = t;
    for (;;) {
        if (t != NULL)
            t->DoExec();
        if (mQueueTop == mQueueBase)
            break;
        AutoPtr<IForkJoinTask> p;
        if (mLocallyFifo) {
            LocallyDeqTask((IForkJoinTask**)&p);
        }
        else {
            p = PopTask();
        }
        t = p.Get();
    }
    ++mStealCount;
    mCurrentSteal = NULL;
    return NOERROR;
}

ECode CForkJoinWorkerThread::CancelTasks()
{
    AutoPtr<ForkJoinTask> cj = (ForkJoinTask*)mCurrentJoin.Get(); // try to cancel ongoing tasks
    if (cj != NULL && cj->mStatus >= 0)
        cj->CancelIgnoringExceptions();
    AutoPtr<ForkJoinTask> cs = (ForkJoinTask*)mCurrentSteal.Get();
    if (cs != NULL && cs->mStatus >= 0)
        cs->CancelIgnoringExceptions();
    while (mQueueBase != mQueueTop) {
        AutoPtr<IForkJoinTask> t;
        DeqTask((IForkJoinTask**)&t);
        if (t != NULL)
            t->CancelIgnoringExceptions();
    }
    return NOERROR;
}

ECode CForkJoinWorkerThread::DrainTasksTo(
    /* [in] */ ICollection* c,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    Int32 n = 0;
    while (mQueueBase != mQueueTop) {
        AutoPtr<IForkJoinTask> t;
        DeqTask((IForkJoinTask**)&t);
        if (t != NULL) {
            Boolean b = FALSE;
            c->Add(t, &b);
            ++n;
        }
    }
    *value = n;
    return NOERROR;
}

ECode CForkJoinWorkerThread::GetQueueSize(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    *value = mQueueTop - mQueueBase;
    return NOERROR;
}

ECode CForkJoinWorkerThread::PollLocalTask(
    /* [out] */ IForkJoinTask** outtask)
{
    VALIDATE_NOT_NULL(outtask);
    AutoPtr<IForkJoinTask> p;
    if (mLocallyFifo)
        LocallyDeqTask((IForkJoinTask**)&p);
    else
        p = PopTask();
    *outtask = p.Get();
    REFCOUNT_ADD(*outtask);
    return NOERROR;
}

ECode CForkJoinWorkerThread::PollTask(
    /* [out] */ IForkJoinTask** outtask)
{
    VALIDATE_NOT_NULL(outtask);
    AutoPtr<ArrayOf<IForkJoinWorkerThread*> > ws;
    AutoPtr<IForkJoinTask> t;
    PollLocalTask((IForkJoinTask**)&t);
    AutoPtr<CForkJoinPool> cpool = (CForkJoinPool*)mPool.Get();
    if (t != NULL || (ws = cpool->mWorkers) == NULL) {
        *outtask = t;
        REFCOUNT_ADD(*outtask);
        return NOERROR;
    }
    Int32 n = ws->GetLength(); // cheap version of FJP.scan
    Int32 steps = n << 1;
    Int32 r = NextSeed();
    Int32 i = 0;
    while (i < steps) {
        AutoPtr<IForkJoinWorkerThread> w = (*ws)[(i++ + r) & (n - 1)];
        AutoPtr<CForkJoinWorkerThread> cw = (CForkJoinWorkerThread*)w.Get();
        if (w != NULL && cw->mQueueBase != cw->mQueueTop && cw->mQueue != NULL) {
            t = NULL;
            w->DeqTask((IForkJoinTask**)&t);
            if (t != NULL) {
                *outtask = t;
                REFCOUNT_ADD(*outtask);
                return NOERROR;
            }
            i = 0;
        }
    }
    *outtask = NULL;
    return NOERROR;
}

ECode CForkJoinWorkerThread::JoinTask(
    /* [in] */ IForkJoinTask* joinMe,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    AutoPtr<IForkJoinTask> prevJoin = mCurrentJoin;
    mCurrentJoin = joinMe;
    for (Int32 s, retries = MAX_HELP;;) {
        AutoPtr<ForkJoinTask> cj = (ForkJoinTask*)joinMe;
        if ((s = cj->mStatus) < 0) {
            mCurrentJoin = prevJoin;
            *value = s;
            return NOERROR;
        }
        if (retries > 0) {
            if (mQueueTop != mQueueBase) {
                if (!LocalHelpJoinTask(joinMe))
                    retries = 0;           // cannot help
            }
            else if (retries == MAX_HELP >> 1) {
                --retries;                 // check uncommon case
                if (TryDeqAndExec(joinMe) >= 0)
                    Thread::Yield();        // for politeness
            }
            else
                retries = HelpJoinTask(joinMe) ? MAX_HELP : retries - 1;
        }
        else {
            retries = MAX_HELP;           // restart if not done
            AutoPtr<CForkJoinPool> cpool = (CForkJoinPool*)mPool.Get();
            cpool->TryAwaitJoin(joinMe);
        }
    }
    return NOERROR;
}

Boolean CForkJoinWorkerThread::LocalHelpJoinTask(
    /* [in] */ IForkJoinTask* joinMe)
{
    Int32 s, i;
    AutoPtr<ArrayOf<IForkJoinTask*> > q;
    AutoPtr<IForkJoinTask> t;
    if ((s = mQueueTop) != mQueueBase && (q = mQueue) != NULL &&
        (i = (q->GetLength() - 1) & --s) >= 0 &&
        (t = (*q)[i]) != NULL) {
        AutoPtr<ForkJoinTask> ct = (ForkJoinTask*)t.Get();
        if (!ObjectUtils::Equals(t->Probe(EIID_IInterface), joinMe->Probe(EIID_IInterface)) &&
             ct->mStatus >= 0)
            return FALSE;
        assert(0 && "TODO");
        // if (UNSAFE.compareAndSwapObject
        //     (q, (i << ASHIFT) + ABASE, t, NULL)) {
        //     mQueueTop = s;           // or putOrderedInt
        //     t->DoExec();
        // }
    }
    return TRUE;
}

Boolean CForkJoinWorkerThread::HelpJoinTask(
    /* [in] */ IForkJoinTask* joinMe)
{
    Boolean helped = FALSE;
    AutoPtr<CForkJoinPool> cpool = (CForkJoinPool*)mPool.Get();
    Int32 m = cpool->mScanGuard & SMASK;
    AutoPtr<ArrayOf<IForkJoinWorkerThread*> > ws = cpool->mWorkers;
    AutoPtr<ForkJoinTask> cj = (ForkJoinTask*)joinMe;
    if (ws != NULL && ws->GetLength() > m && cj->mStatus >= 0) {
        Int32 levels = MAX_HELP;              // remaining chain length
        AutoPtr<IForkJoinTask> task = joinMe;      // base of chain
        outer:for (AutoPtr<IForkJoinWorkerThread> thread = this;;) {
            // Try to find v, the stealer of task, by first using hint
            AutoPtr<CForkJoinWorkerThread> ct = (CForkJoinWorkerThread*)thread.Get();
            AutoPtr<IForkJoinWorkerThread> v = (*ws)[ct->mStealHint & m];
            AutoPtr<CForkJoinWorkerThread> cv = (CForkJoinWorkerThread*)v.Get();
            if (v == NULL || cv->mCurrentSteal != task) {
                for (Int32 j = 0; ;) {        // search array
                    if ((v = (*ws)[j]) != NULL) {
                        cv = (CForkJoinWorkerThread*)v.Get();
                        if (ObjectUtils::Equals(cv->mCurrentSteal->Probe(EIID_IInterface), task->Probe(EIID_IInterface))) {
                            ct->mStealHint = j;
                            break;              // save hint for next time
                        }
                    }
                    if (++j > m)
                        goto outer;        // can't find stealer
                }
            }
            // Try to help v, using specialized form of deqTask
            for (;;) {
                AutoPtr<ArrayOf<IForkJoinTask*> > q;
                Int32 b, i;
                if (cj->mStatus < 0)
                    goto outer;
                if ((b = cv->mQueueBase) == cv->mQueueTop ||
                    (q = cv->mQueue) == NULL ||
                    (i = (q->GetLength()-1) & b) < 0)
                    break;                  // empty
                assert(0 && "TODO");
                // Int64 u = (i << ASHIFT) + ABASE;
                // AutoPtr<IForkJoinTask> t = (*q)[i];
                // if (task->mStatus < 0)
                //     goto outer;            // stale
                // if (t != NULL && cv->mQueueBase == b &&
                //     UNSAFE.compareAndSwapObject(q, u, t, NULL)) {
                //     cv->mQueueBase = b + 1;
                //     cv->mStealHint = mPoolIndex;
                //     AutoPtr<IForkJoinTask> ps = mCurrentSteal;
                //     mCurrentSteal = t;
                //     t->DoExec();
                //     mCurrentSteal = ps;
                //     helped = TRUE;
                // }
            }
            // Try to descend to find v's stealer
            AutoPtr<IForkJoinTask> next = cv->mCurrentJoin;
            AutoPtr<ForkJoinTask> ctask = (ForkJoinTask*)task.Get();
            if (--levels > 0 && ctask->mStatus >= 0 &&
                next != NULL &&
                !ObjectUtils::Equals(next->Probe(EIID_IInterface), task->Probe(EIID_IInterface))) {
                task = next;
                thread = v;
            }
            else
                break;  // max levels, stale, dead-end, or cyclic
        }
    }
    return helped;
}

Int32 CForkJoinWorkerThread::TryDeqAndExec(
    /* [in] */ IForkJoinTask* t)
{
    AutoPtr<CForkJoinPool> cpool = (CForkJoinPool*)mPool.Get();
    Int32 m = cpool->mScanGuard & SMASK;
    AutoPtr<ArrayOf<IForkJoinWorkerThread*> > ws = cpool->mWorkers;
    AutoPtr<ForkJoinTask> ct = (ForkJoinTask*)t;
    if (ws != NULL && ws->GetLength() > m && ct->mStatus >= 0) {
        for (Int32 j = 0; j <= m; ++j) {
            AutoPtr<ArrayOf<IForkJoinTask*> > q;
            Int32 b, i;
            AutoPtr<IForkJoinWorkerThread> v = (*ws)[j];
            AutoPtr<CForkJoinWorkerThread> cv = (CForkJoinWorkerThread*)v.Get();
            if (v != NULL &&
                (b = cv->mQueueBase) != cv->mQueueTop &&
                (q = cv->mQueue) != NULL &&
                (i = (q->GetLength() - 1) & b) >= 0 &&
                ObjectUtils::Equals((*q)[i]->Probe(EIID_IInterface), t->Probe(EIID_IInterface))) {
                assert(0 && "TODO");
                // Int64 u = (i << ASHIFT) + ABASE;
                // if (v->mQueueBase == b &&
                //     UNSAFE.compareAndSwapObject(q, u, t, NULL)) {
                //     v->mQueueBase = b + 1;
                //     v->mStealHint = mPoolIndex;
                //     AutoPtr<IForkJoinTask> ps = mCurrentSteal;
                //     mCurrentSteal = t;
                //     t->DoExec();
                //     mCurrentSteal = ps;
                // }
                break;
            }
        }
    }
    return ct->mStatus;
}

ECode CForkJoinWorkerThread::GetEstimatedSurplusTaskCount(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    AutoPtr<CForkJoinPool> cpool = (CForkJoinPool*)mPool.Get();
    *value = mQueueTop - mQueueBase - cpool->IdlePerActive();
    return NOERROR;
}

ECode CForkJoinWorkerThread::HelpQuiescePool()
{
    Boolean active = TRUE;
    AutoPtr<IForkJoinTask> ps = mCurrentSteal; // to restore below
    AutoPtr<CForkJoinPool> p = (CForkJoinPool*)mPool.Get();
    p->AddQuiescerCount(1);
    for (;;) {
        AutoPtr<ArrayOf<IForkJoinWorkerThread*> > ws = p->mWorkers;
        AutoPtr<IForkJoinWorkerThread> v;
        Int32 n;
        if (mQueueTop != mQueueBase)
            v = this;
        else if (ws != NULL && (n = ws->GetLength()) > 1) {
            AutoPtr<IForkJoinWorkerThread> w;
            Int32 r = NextSeed(); // cheap version of FJP.scan
            Int32 steps = n << 1;
            for (Int32 i = 0; i < steps; ++i) {
                if ((w = (*ws)[(i + r) & (n - 1)]) != NULL) {
                    AutoPtr<CForkJoinWorkerThread> cw = (CForkJoinWorkerThread*)w.Get();
                    if (cw->mQueueBase != cw->mQueueTop) {
                        v = w;
                        break;
                    }
                }
            }
        }
        if (v != NULL) {
            AutoPtr<IForkJoinTask> t;
            if (!active) {
                active = TRUE;
                p->AddActiveCount(1);
            }

            AutoPtr<IForkJoinTask> p1;
            if (mLocallyFifo)
                LocallyDeqTask((IForkJoinTask**)&p1);
            else
                p1 = PopTask();

            AutoPtr<IForkJoinTask> p2;
            if (v != this)
                v->DeqTask((IForkJoinTask**)&p2);
            else
                p2 = p1;

            if ((t = p2) != NULL) {
                mCurrentSteal = t;
                t->DoExec();
                mCurrentSteal = ps;
            }
        }
        else {
            if (active) {
                active = FALSE;
                p->AddActiveCount(-1);
            }
            Boolean b = FALSE;
            if ((p->IsQuiescent(&b), b)) {
                p->AddActiveCount(1);
                p->AddQuiescerCount(-1);
                break;
            }
        }
    }
    return NOERROR;
}

ECode CForkJoinWorkerThread::CheckAccess()
{
    return Thread::CheckAccess();
}

ECode CForkJoinWorkerThread::CountStackFrames(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    return Thread::CountStackFrames(number);
}

ECode CForkJoinWorkerThread::Destroy()
{
    return Thread::Destroy();
}

ECode CForkJoinWorkerThread::GetContextClassLoader(
    /* [out] */ IClassLoader** outload)
{
    VALIDATE_NOT_NULL(outload);
    return Thread::GetContextClassLoader(outload);
}

ECode CForkJoinWorkerThread::GetId(
    /* [out] */ Int64* id)
{
    VALIDATE_NOT_NULL(id);
    return Thread::GetId(id);
}

ECode CForkJoinWorkerThread::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    return Thread::GetName(name);
}

ECode CForkJoinWorkerThread::GetPriority(
    /* [out] */ Int32* priority)
{
    VALIDATE_NOT_NULL(priority);
    return Thread::GetPriority(priority);
}

ECode CForkJoinWorkerThread::GetState(
    /* [out] */ ThreadState* state)
{
    VALIDATE_NOT_NULL(state);
    return Thread::GetState(state);
}

ECode CForkJoinWorkerThread::GetThreadGroup(
    /* [out] */ IThreadGroup** group)
{
    VALIDATE_NOT_NULL(group);
    return Thread::GetThreadGroup(group);
}

ECode CForkJoinWorkerThread::GetUncaughtExceptionHandler(
    /* [out] */ IThreadUncaughtExceptionHandler** handler)
{
    VALIDATE_NOT_NULL(handler);
    return Thread::GetUncaughtExceptionHandler(handler);
}

ECode CForkJoinWorkerThread::Interrupt()
{
    return Thread::Interrupt();
}

ECode CForkJoinWorkerThread::IsAlive(
    /* [out] */ Boolean* isAlive)
{
    VALIDATE_NOT_NULL(isAlive);
    return Thread::IsAlive(isAlive);
}

ECode CForkJoinWorkerThread::IsDaemon(
    /* [out] */ Boolean* isDaemon)
{
    VALIDATE_NOT_NULL(isDaemon);
    return Thread::IsDaemon(isDaemon);
}

ECode CForkJoinWorkerThread::IsInterrupted(
    /* [out] */ Boolean* isInterrupted)
{
    VALIDATE_NOT_NULL(isInterrupted);
    return Thread::IsInterrupted(isInterrupted);
}

ECode CForkJoinWorkerThread::Join()
{
    return Thread::Join();
}

ECode CForkJoinWorkerThread::Join(
    /* [in] */ Int64 millis)
{
    return Thread::Join(millis);
}

ECode CForkJoinWorkerThread::Join(
    /* [in] */ Int64 millis,
    /* [in] */ Int32 nanos)
{
    return Thread::Join(millis, nanos);
}

ECode CForkJoinWorkerThread::Resume()
{
    return Thread::Resume();
}

ECode CForkJoinWorkerThread::SetContextClassLoader(
    /* [in] */ IClassLoader* cl)
{
    return Thread::SetContextClassLoader(cl);
}

ECode CForkJoinWorkerThread::SetDaemon(
    /* [in] */ Boolean isDaemon)
{
    return Thread::SetDaemon(isDaemon);
}

ECode CForkJoinWorkerThread::PushInterruptAction(
    /* [in] */ IRunnable* interruptAction)
{
    return Thread::PushInterruptAction(interruptAction);
}

ECode CForkJoinWorkerThread::PopInterruptAction(
    /* [in] */ IRunnable* interruptAction)
{
    return Thread::PopInterruptAction(interruptAction);
}

ECode CForkJoinWorkerThread::SetName(
    /* [in] */ const String& threadName)
{
    return Thread::SetName(threadName);
}

ECode CForkJoinWorkerThread::SetPriority(
    /* [in] */ Int32 priority)
{
    return Thread::SetPriority(priority);
}

ECode CForkJoinWorkerThread::SetUncaughtExceptionHandler(
    /* [in] */ IThreadUncaughtExceptionHandler* handler)
{
    return Thread::SetUncaughtExceptionHandler(handler);
}

ECode CForkJoinWorkerThread::Start()
{
    return Thread::Start();
}

ECode CForkJoinWorkerThread::Stop()
{
    return Thread::Stop();
}

ECode CForkJoinWorkerThread::Suspend()
{
    return Thread::Suspend();
}

ECode CForkJoinWorkerThread::Unpark()
{
    return Thread::Unpark();
}

ECode CForkJoinWorkerThread::ParkFor(
    /* [in] */ Int64 nanos)
{
    return Thread::ParkFor(nanos);
}

ECode CForkJoinWorkerThread::ParkUntil(
    /* [in] */ Int64 time)
{
    return Thread::ParkUntil(time);
}

ECode CForkJoinWorkerThread::Detach()
{
    return Thread::Detach();
}

ECode CForkJoinWorkerThread::Lock()
{
    return Thread::Lock();
}

ECode CForkJoinWorkerThread::Unlock()
{
    return Thread::Unlock();
}

ECode CForkJoinWorkerThread::Notify()
{
    return Thread::Notify();
}

ECode CForkJoinWorkerThread::NotifyAll()
{
    return Thread::NotifyAll();
}

ECode CForkJoinWorkerThread::Wait()
{
    return Thread::Wait();
}

ECode CForkJoinWorkerThread::Wait(
    /* [in] */ Int64 millis)
{
    return Thread::Wait(millis);
}

ECode CForkJoinWorkerThread::Wait(
    /* [in] */ Int64 millis,
    /* [in] */ Int32 nanos)
{
    return Thread::Wait(millis, nanos);
}

} // namespace Concurrent
} // namespace Utility
} // namespace Elastos
