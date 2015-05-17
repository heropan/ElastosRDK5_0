
#include "cmdef.h"
#include "CForkJoinPool.h"
#include "CForkJoinWorkerThread.h"
#include "LockSupport.h"
#include "ForkJoinTask.h"
#include "TimeUnit.h"
#include "CArrays.h"
#include <elastos/ObjectUtils.h>
#include "CReentrantLock.h"
#include "CArrayList.h"
#include "CCollections.h"

using Elastos::Core::ObjectUtils;
using Elastos::Utility::CArrays;
using Elastos::Utility::CArrayList;
using Elastos::Utility::CCollections;
using Elastos::Utility::Concurrent::CForkJoinWorkerThread;
using Elastos::Utility::Concurrent::ForkJoinTask;
using Elastos::Utility::Concurrent::TimeUnit;
using Elastos::Utility::Concurrent::Locks::LockSupport;
using Elastos::Utility::Concurrent::Locks::CReentrantLock;

namespace Elastos {
namespace Utility {
namespace Concurrent {

//====================================================================
// CForkJoinPool::DefaultForkJoinWorkerThreadFactory::
//====================================================================

AutoPtr<IForkJoinWorkerThread> CForkJoinPool::DefaultForkJoinWorkerThreadFactory::NewThread(
    /* [in] */ IForkJoinPool* pool)
{
    AutoPtr<IForkJoinWorkerThread> p;
    CForkJoinWorkerThread::New(pool, (IForkJoinWorkerThread**)&p);
   return p;
}

//====================================================================
// CForkJoinPool::
//====================================================================

AutoPtr<IForkJoinPoolForkJoinWorkerThreadFactory> CForkJoinPool::mDefaultForkJoinWorkerThreadFactory;

//RuntimePermission modifyThreadPermission;

AutoPtr<IAtomicInteger32> CForkJoinPool::mPoolNumberGenerator;

AutoPtr<IRandom> CForkJoinPool::mWorkerSeedGenerator;

Int32 CForkJoinPool::INITIAL_QUEUE_CAPACITY = 8;

Int32 CForkJoinPool::MAXIMUM_QUEUE_CAPACITY = 1 << 24; // 16M

// bit positions/shifts for fields
Int32 CForkJoinPool::AC_SHIFT   = 48;
Int32 CForkJoinPool::TC_SHIFT   = 32;
Int32 CForkJoinPool::ST_SHIFT   = 31;
Int32 CForkJoinPool::EC_SHIFT   = 16;

// bounds
Int32 CForkJoinPool::MAX_ID     = 0x7fff;  // max poolIndex
Int32 CForkJoinPool::SMASK      = 0xffff;  // mask short bits
Int32 CForkJoinPool::SHORT_SIGN = 1 << 15;
Int32 CForkJoinPool::INT_SIGN   = 1 << 31;

// masks
Int64 CForkJoinPool::STOP_BIT   = 0x0001L << ST_SHIFT;
Int64 CForkJoinPool::AC_MASK    = ((Int64)SMASK) << AC_SHIFT;
Int64 CForkJoinPool::TC_MASK    = ((Int64)SMASK) << TC_SHIFT;

// units for incrementing and decrementing
Int64 CForkJoinPool::TC_UNIT    = 1L << TC_SHIFT;
Int64 CForkJoinPool::AC_UNIT    = 1L << AC_SHIFT;

// masks and units for dealing with u = (int)(ctl >>> 32)
Int32 CForkJoinPool::UAC_SHIFT  = AC_SHIFT - 32;
Int32 CForkJoinPool::UTC_SHIFT  = TC_SHIFT - 32;
Int32 CForkJoinPool::UAC_MASK   = SMASK << UAC_SHIFT;
Int32 CForkJoinPool::UTC_MASK   = SMASK << UTC_SHIFT;
Int32 CForkJoinPool::UAC_UNIT   = 1 << UAC_SHIFT;
Int32 CForkJoinPool::UTC_UNIT   = 1 << UTC_SHIFT;

// masks and units for dealing with e = (int)ctl
Int32 CForkJoinPool::E_MASK     = 0x7fffffff; // no STOP_BIT
Int32 CForkJoinPool::EC_UNIT    = 1 << EC_SHIFT;

Int32 CForkJoinPool::SG_UNIT = 1 << 16;

Int64 CForkJoinPool::SHRINK_RATE = 4L * 1000L * 1000L * 1000L; // 4 seconds

void CForkJoinPool::CheckPermission()
{
    assert(0 && "TODO");
    // SecurityManager security = System.getSecurityManager();
    // if (security != NULL)
    //     security->CheckPermission(modifyThreadPermission);
}

void CForkJoinPool::Work(
    /* [in] */ IForkJoinWorkerThread* w)
{
    Boolean swept = FALSE;                // true on empty scans
    Int64 c;
    AutoPtr<CForkJoinWorkerThread> p = (CForkJoinWorkerThread*)w;
    while (!p->mTerminate && (Int32)(c = mCtl) >= 0) {
        Int32 a;                            // active count
        if (!swept && (a = (Int32)(c >> AC_SHIFT)) <= 0)
            swept = Scan(w, a);
        else if (TryAwaitWork(w, c))
            swept = FALSE;
    }
}

// Signalling
void CForkJoinPool::SignalWork()
{
    assert(0 && "TODO");
    /*
     * The while condition is true if: (there is are too few total
     * workers OR there is at least one waiter) AND (there are too
     * few active workers OR the pool is terminating).  The value
     * of e distinguishes the remaining cases: zero (no waiters)
     * for create, negative if terminating (in which case do
     * nothing), else release a waiter. The secondary checks for
     * release (non-null array etc) can fail if the pool begins
     * terminating after the test, and don't impose any added cost
     * because JVMs must perform null and bounds checks anyway.
     */
    Int64 c; Int32 e, u;
    while ((((e = (Int32)(c = mCtl)) | (u = (Int32)(c >> 32))) &
            (INT_SIGN|SHORT_SIGN)) == (INT_SIGN|SHORT_SIGN) && e >= 0) {
        if (e > 0) {                         // release a waiting worker
            Int32 i;
            AutoPtr<IForkJoinWorkerThread> w;
            AutoPtr<ArrayOf<IForkJoinWorkerThread*> > ws;
            if ((ws = mWorkers) == NULL ||
                (i = ~e & SMASK) >= ws->GetLength() ||
                (w = (*ws)[i]) == NULL)
                break;
            AutoPtr<CForkJoinWorkerThread> p = (CForkJoinWorkerThread*)w.Get();
            Int64 nc = (((Int64)(p->mNextWait & E_MASK)) |
                      ((Int64)(u + UAC_UNIT) << 32));
            // if (p->mEventCount == e &&
            //     UNSAFE.compareAndSwapLong(this, ctlOffset, c, nc)) {
            //     p->mEventCount = (e + EC_UNIT) & E_MASK;
            //     if (p->mParked)
            //         UNSAFE.unpark(w);
            //     break;
            // }
        }
        // else if (UNSAFE.compareAndSwapLong
        //         (this, ctlOffset, c,
        //         (Int64)(((u + UTC_UNIT) & UTC_MASK) |
        //         ((u + UAC_UNIT) & UAC_MASK)) << 32)) {
        //     AddWorker();
        //     break;
        // }
    }
}

Boolean CForkJoinPool::TryReleaseWaiter()
{
    assert(0 && "TODO");
    Int64 c;
    Int32 e, i;
    AutoPtr<IForkJoinWorkerThread> w;
    AutoPtr<ArrayOf<IForkJoinWorkerThread*> > ws;
    if ((e = (Int32)(c = mCtl)) > 0 &&
        (Int32)(c >> AC_SHIFT) < 0 &&
        (ws = mWorkers) != NULL &&
        (i = ~e & SMASK) < ws->GetLength() &&
        (w = (*ws)[i]) != NULL) {
        AutoPtr<CForkJoinWorkerThread> p = (CForkJoinWorkerThread*)w.Get();
        Int64 nc = ((Int64)(p->mNextWait & E_MASK) |
                  ((c + AC_UNIT) & (AC_MASK|TC_MASK)));
        // if (p->mEventCount != e ||
        //     !UNSAFE.compareAndSwapLong(this, ctlOffset, c, nc))
        //     return FALSE;
        p->mEventCount = (e + EC_UNIT) & E_MASK;
        // if (w->mParked)
        //     UNSAFE.unpark(w);
    }
    return TRUE;
}

Boolean CForkJoinPool::Scan(
    /* [in] */ IForkJoinWorkerThread* w,
    /* [in] */ const Int32& a)
{
    Int32 g = mScanGuard; // mask 0 avoids useless scans if only one active
    Int32 m = (mParallelism == 1 - a && mBlockedCount == 0) ? 0 : g & SMASK;
    AutoPtr<ArrayOf<IForkJoinWorkerThread*> > ws = mWorkers;
    if (ws == NULL || ws->GetLength() <= m)         // staleness check
        return FALSE;
    AutoPtr<CForkJoinWorkerThread> cw = (CForkJoinWorkerThread*)w;
    for (Int32 r = cw->mSeed, k = r, j = -(m + m); j <= m + m; ++j) {
        AutoPtr<IForkJoinTask> t;
        AutoPtr<ArrayOf<IForkJoinTask*> > q;
        Int32 b, i;
        AutoPtr<IForkJoinWorkerThread> v = (*ws)[k & m];
        AutoPtr<CForkJoinWorkerThread> cv = (CForkJoinWorkerThread*)v.Get();
        if (cv != NULL && (b = cv->mQueueBase) != cv->mQueueTop &&
            (q = cv->mQueue) != NULL && (i = (q->GetLength() - 1) & b) >= 0) {
            assert(0 && "TODO");
            // Int64 u = (i << ASHIFT) + ABASE;
            // if ((t = (*q)[i]) != NULL && cv->mQueueBase == b &&
            //     UNSAFE.compareAndSwapObject(q, u, t, NULL)) {
            //     Int32 d = (cv->mQueueBase = b + 1) - cv->mQueueTop;
            //     cv->mStealHint = cw->mPoolIndex;
            //     if (d != 0)
            //         SignalWork();             // propagate if nonempty
            //     w->ExecTask(t);
            // }
            r ^= r << 13; r ^= r >> 17; cw->mSeed = r ^ (r << 5);
            return FALSE;                     // store next seed
        }
        else if (j < 0) {                     // xorshift
            r ^= r << 13; r ^= r >> 17; k = r ^= r << 5;
        }
        else
            ++k;
    }
    if (mScanGuard != g)                       // staleness check
        return FALSE;
    else {                                    // try to take submission
        AutoPtr<IForkJoinTask> t;
        AutoPtr<ArrayOf<IForkJoinTask*> > q;
        Int32 b, i;
        if ((b = mQueueBase) != mQueueTop &&
            (q = mSubmissionQueue) != NULL &&
            (i = (q->GetLength() - 1) & b) >= 0) {
            assert(0 && "TODO");
            // Int64 u = (i << ASHIFT) + ABASE;
            // if ((t = (*q)[i]) != NULL && mQueueBase == b &&
            //     UNSAFE.compareAndSwapObject(q, u, t, NULL)) {
            //     mQueueBase = b + 1;
            //     w->ExecTask(t);
            // }
            return FALSE;
        }
        return TRUE;                         // all queues empty
    }
}

Boolean CForkJoinPool::TryAwaitWork(
    /* [in] */ IForkJoinWorkerThread* w,
    /* [in] */ const Int64& c)
{
    assert(0 && "TODO");
    AutoPtr<CForkJoinWorkerThread> cw = (CForkJoinWorkerThread*)w;
    Int32 v = cw->mEventCount;
    cw->mNextWait = (Int32)c;                      // w's successor record
    Int64 nc = (Int64)(v & E_MASK) | ((c - AC_UNIT) & (AC_MASK|TC_MASK));
    // if (mCtl != c || !UNSAFE.compareAndSwapLong(this, ctlOffset, c, nc)) {
    //     Int64 d = mCtl; // return true if lost to a deq, to force scan
    //     return (Int32)d != (Int32)c && (d & AC_MASK) >= (c & AC_MASK);
    // }
    for (Int32 sc = cw->mStealCount; sc != 0;) {   // accumulate stealCount
        Int64 s = mStealCount;
        // if (UNSAFE.compareAndSwapLong(this, stealCountOffset, s, s + sc))
        //     sc = cw->mStealCount = 0;
        // else if (cw->mEventCount != v)
        //     return TRUE;                      // update next time
    }
    if ((!mShutdown || !TryTerminate(FALSE)) &&
        (Int32)c != 0 && mParallelism + (Int32)(nc >> AC_SHIFT) == 0 &&
        mBlockedCount == 0 && mQuiescerCount == 0)
        IdleAwaitWork(cw, nc, c, v);           // quiescent
    for (Boolean rescanned = FALSE;;) {
        if (cw->mEventCount != v)
            return TRUE;
        if (!rescanned) {
            Int32 g = mScanGuard, m = g & SMASK;
            AutoPtr<ArrayOf<IForkJoinWorkerThread*> > ws = mWorkers;
            if (ws != NULL && m < ws->GetLength()) {
                rescanned = TRUE;
                for (Int32 i = 0; i <= m; ++i) {
                    AutoPtr<IForkJoinWorkerThread> u = (*ws)[i];
                    AutoPtr<CForkJoinWorkerThread> cu = (CForkJoinWorkerThread*)u.Get();
                    if (cu != NULL) {
                        if (cu->mQueueBase != cu->mQueueTop &&
                            !TryReleaseWaiter())
                            rescanned = FALSE; // contended
                        if (cw->mEventCount != v)
                            return TRUE;
                    }
                }
            }
            if (mScanGuard != g ||              // stale
                (mQueueBase != mQueueTop && !TryReleaseWaiter()))
                rescanned = FALSE;
            if (!rescanned)
                Thread::Yield();                // reduce contention
            else
                Thread::Interrupted();          // clear before park
        }
        else {
            cw->mParked = TRUE;                   // must recheck
            if (cw->mEventCount != v) {
                cw->mParked = FALSE;
                return TRUE;
            }
            LockSupport::Park(THIS_PROBE(IInterface));
            rescanned = cw->mParked = FALSE;
        }
    }
}

void CForkJoinPool::IdleAwaitWork(
    /* [in] */ IForkJoinWorkerThread* w,
    /* [in] */ const Int64& currentCtl,
    /* [in] */ const Int64& prevCtl,
    /* [in] */ const Int32& v)
{
    assert(0 && "TODO");
    AutoPtr<CForkJoinWorkerThread> cw = (CForkJoinWorkerThread*)w;
    if (cw->mEventCount == v) {
        if (mShutdown)
            TryTerminate(FALSE);
        ForkJoinTask::HelpExpungeStaleExceptions(); // help clean weak refs
        while (mCtl == currentCtl) {
            Int64 startTime;// = System.nanoTime();
            cw->mParked = TRUE;
            if (cw->mEventCount == v)             // must recheck
                LockSupport::ParkNanos(THIS_PROBE(IInterface), SHRINK_RATE);
            cw->mParked = FALSE;
            if (cw->mEventCount != v)
                break;
            // else if (System.nanoTime() - startTime <
            //         SHRINK_RATE - (SHRINK_RATE / 10)) // timing slop
            //     Thread::Interrupted();          // spurious wakeup
            // else if (UNSAFE.compareAndSwapLong(this, ctlOffset,
            //                                   currentCtl, prevCtl)) {
            //     cw->mTerminate = TRUE;            // restore previous
            //     cw->mEventCount = ((Int32)currentCtl + EC_UNIT) & E_MASK;
            //     break;
            // }
        }
    }
}

void CForkJoinPool::AddSubmission(
    /* [in] */ IForkJoinTask* t)
{
    assert(0 && "TODO");
    AutoPtr<IReentrantLock> lock = mSubmissionLock;
    lock->Lock();
    AutoPtr<ArrayOf<IForkJoinTask*> > q;
    Int32 s, m;
    if ((q = mSubmissionQueue) != NULL) {    // ignore if queue removed
//        Int64 u = (((s = mQueueTop) & (m = q->GetLength()-1)) << ASHIFT)+ABASE;
//        UNSAFE.putOrderedObject(q, u, t);
        mQueueTop = s + 1;
        if (s - mQueueBase == m)
            GrowSubmissionQueue();
    }
    lock->UnLock();
    SignalWork();
}

ECode CForkJoinPool::GrowSubmissionQueue()
{
    AutoPtr<ArrayOf<IForkJoinTask*> > oldQ = mSubmissionQueue;
    Int32 size = oldQ != NULL ? oldQ->GetLength() << 1 : INITIAL_QUEUE_CAPACITY;
    if (size > MAXIMUM_QUEUE_CAPACITY)
        return E_REJECTED_EXECUTION_EXCEPTION;
    if (size < INITIAL_QUEUE_CAPACITY)
        size = INITIAL_QUEUE_CAPACITY;
    AutoPtr<ArrayOf<IForkJoinTask*> > q = mSubmissionQueue = ArrayOf<IForkJoinTask*>::Alloc(size);
    Int32 mask = size - 1;
    Int32 top = mQueueTop;
    Int32 oldMask;
    if (oldQ != NULL && (oldMask = oldQ->GetLength() - 1) >= 0) {
        assert(0 && "TODO");
        for (Int32 b = mQueueBase; b != top; ++b) {
//            Int64 u = ((b & oldMask) << ASHIFT) + ABASE;
//            AutoPtr<IInterface> x = UNSAFE.getObjectVolatile(oldQ, u);
            // if (x != NULL && UNSAFE.compareAndSwapObject(oldQ, u, x, NULL))
            //     UNSAFE.putObjectVolatile(q, ((b & mask) << ASHIFT) + ABASE, x);
        }
    }
    return NOERROR;
}

Boolean CForkJoinPool::TryPreBlock()
{
    assert(0 && "TODO");
    Int32 b = mBlockedCount;
    // if (UNSAFE.compareAndSwapInt(this, blockedCountOffset, b, b + 1)) {
    //     Int32 pc = mParallelism;
    //     do {
    //         AutoPtr<ArrayOf<IForkJoinWorkerThread*> > ws;
    //         AutoPtr<IForkJoinWorkerThread> w;
    //         Int32 e, ac, tc, i;
    //         Int64 c = mCtl;
    //         Int32 u = (Int32)(c >> 32);
    //         if ((e = (Int32)c) < 0) {
    //                                         // skip -- terminating
    //         }
    //         else if ((ac = (u >> UAC_SHIFT)) <= 0 && e != 0 &&
    //                 (ws = mWorkers) != NULL &&
    //                 (i = ~e & SMASK) < ws->GetLength() &&
    //                 (w = (*ws)[i]) != NULL) {
    //             Int64 nc = ((Int64)(w->mNextWait & E_MASK) |
    //                       (c & (AC_MASK|TC_MASK)));
    //             if (w->mEventCount == e &&
    //                 UNSAFE.compareAndSwapLong(this, ctlOffset, c, nc)) {
    //                 w->mEventCount = (e + EC_UNIT) & E_MASK;
    //                 if (w->mParked)
    //                     UNSAFE.unpark(w);
    //                 return TRUE;             // release an idle worker
    //             }
    //         }
    //         else if ((tc = (Int16)(u >> UTC_SHIFT)) >= 0 && ac + pc > 1) {
    //             Int64 nc = ((c - AC_UNIT) & AC_MASK) | (c & ~AC_MASK);
    //             if (UNSAFE.compareAndSwapLong(this, ctlOffset, c, nc))
    //                 return TRUE;             // no compensation needed
    //         }
    //         else if (tc + pc < MAX_ID) {
    //             Int64 nc = ((c + TC_UNIT) & TC_MASK) | (c & ~TC_MASK);
    //             if (UNSAFE.compareAndSwapLong(this, ctlOffset, c, nc)) {
    //                 AddWorker();
    //                 return TRUE;            // create a replacement
    //             }
    //         }
    //         // try to back out on any failure and let caller retry
    //     } while (!UNSAFE.compareAndSwapInt(this, blockedCountOffset,
    //                                       b = mBlockedCount, b - 1));
    // }
    return FALSE;
}

void CForkJoinPool::PostBlock()
{
    assert(0 && "TODO");
    // Int64 c;
    // do {} while (!UNSAFE.compareAndSwapLong(this, ctlOffset,  // no mask
    //                                         c = mCtl, c + AC_UNIT));
    // Int32 b;
    // do {} while (!UNSAFE.compareAndSwapInt(this, blockedCountOffset,
    //                                       b = mBlockedCount, b - 1));
}

void CForkJoinPool::TryAwaitJoin(
    /* [in] */ IForkJoinTask* joinMe)
{
    Thread::Interrupted(); // clear interrupts before checking termination
    AutoPtr<ForkJoinTask> cj = (ForkJoinTask*)joinMe;
    if (cj->mStatus >= 0) {
        if (TryPreBlock()) {
            joinMe->TryAwaitDone(0L);
            PostBlock();
        }
        else if ((mCtl & STOP_BIT) != 0L)
            joinMe->CancelIgnoringExceptions();
    }
}

ECode CForkJoinPool::TimedAwaitJoin(
    /* [in] */ IForkJoinTask* joinMe,
    /* [in] */ Int64 nanos)
{
    AutoPtr<ForkJoinTask> cj = (ForkJoinTask*)joinMe;
    while (cj->mStatus >= 0) {
        Thread::Interrupted();
        if ((mCtl & STOP_BIT) != 0L) {
            joinMe->CancelIgnoringExceptions();
            break;
        }
        if (TryPreBlock()) {
            Int64 last;// = System.nanoTime();
            while (cj->mStatus >= 0) {
                Int64 millis;
                TimeUnit::GetNANOSECONDS()->ToMillis(nanos, &millis);
                if (millis <= 0)
                    break;
                joinMe->TryAwaitDone(millis);
                if (cj->mStatus < 0)
                    break;
                if ((mCtl & STOP_BIT) != 0L) {
                    joinMe->CancelIgnoringExceptions();
                    break;
                }
                Int64 now;// = System.nanoTime();
                nanos -= now - last;
                last = now;
            }
            PostBlock();
            break;
        }
    }
    return NOERROR;
}

void CForkJoinPool::AwaitBlocker(
    /* [in] */ IForkJoinPoolManagedBlocker* blocker)
{
    Boolean b1 = FALSE;
    while (!(blocker->IsReleasable(&b1), b1)) {
        if (TryPreBlock()) {
            Boolean b2 = FALSE, b3 = FALSE;
            do {} while (!(blocker->IsReleasable(&b2), b2) && !(blocker->Block(&b3), b3));
            PostBlock();
            break;
        }
    }
}

void CForkJoinPool::AddWorker()
{
    assert(0 && "TODO");
//    AutoPtr<Throwable> ex = NULL;
    AutoPtr<IForkJoinWorkerThread> t;
//    try {
    mFactory->NewThread(this, (IForkJoinWorkerThread**)&t);
    // } catch (Throwable e) {
    //     ex = e;
    // }
    if (t == NULL) {  // null or exceptional factory return
        Int64 c;       // adjust counts
        // do {} while (!UNSAFE.compareAndSwapLong
        //             (this, ctlOffset, c = mCtl,
        //               (((c - AC_UNIT) & AC_MASK) |
        //               ((c - TC_UNIT) & TC_MASK) |
        //               (c & ~(AC_MASK|TC_MASK)))));
        // Propagate exception if originating from an external caller
        // if (!TryTerminate(FALSE) && ex != NULL &&
        //     !(Thread::GetCurrentThread() instanceof ForkJoinWorkerThread))
        //     SneakyThrow.sneakyThrow(ex); // android-changed
    }
    else
        t->Start();
}

String CForkJoinPool::NextWorkerName()
{
    assert(0 && "TODO");
//    for (Int32 n;;) {
        // if (UNSAFE.compareAndSwapInt(this, nextWorkerNumberOffset,
        //                             n = mNextWorkerNumber, ++n)) {
        //     *str = mWorkerNamePrefix + n;
        //     return NOERROR;
        // }
//    }
    return String(NULL);
}

Int32 CForkJoinPool::RegisterWorker(
    /* [in] */ IForkJoinWorkerThread* w)
{
    /*
     * In the typical case, a new worker acquires the lock, uses
     * next available index and returns quickly.  Since we should
     * not block callers (ultimately from signalWork or
     * tryPreBlock) waiting for the lock needed to do this, we
     * instead help release other workers while waiting for the
     * lock.
     */
    for (Int32 g;;) {
        AutoPtr<ArrayOf<IForkJoinWorkerThread*> > ws;
        assert(0 && "TODO");
        if (((g = mScanGuard) & SG_UNIT) == 0/* &&
            UNSAFE.compareAndSwapInt(this, scanGuardOffset,
                                     g, g | SG_UNIT)*/) {
            Int32 k = mNextWorkerIndex;
            if ((ws = mWorkers) != NULL) { // ignore on shutdown
                Int32 n = ws->GetLength();
                if (k < 0 || k >= n || (*ws)[k] != NULL) {
                    for (k = 0; k < n && (*ws)[k] != NULL; ++k)
                        ;
                    if (k == n) {
                        CArrays::_CopyOf((ArrayOf<IInterface*>*)ws.Get(), n << 1, (ArrayOf<IInterface*>**)&mWorkers);
                        ws = mWorkers;
                    }
                }
                (*ws)[k] = w;
                mNextWorkerIndex = k + 1;
                Int32 m = g & SMASK;
                g = (k > m) ? ((m << 1) + 1) & SMASK : g + (SG_UNIT<<1);
            }
            mScanGuard = g;
            return k;
        }
        else if ((ws = mWorkers) != NULL) { // help release others
            for (Int32 i = 0;i < ws->GetLength();i++) {
                AutoPtr<IForkJoinWorkerThread> u = (*ws)[i];
                AutoPtr<CForkJoinWorkerThread> cu = (CForkJoinWorkerThread*)u.Get();
                if (cu != NULL && cu->mQueueBase != cu->mQueueTop) {
                    if (TryReleaseWaiter())
                        break;
                }
            }
        }
    }
}

void CForkJoinPool::DeregisterWorker(
    /* [in] */ IForkJoinWorkerThread* w,
    /* [in] */ IThrowable* ex)
{
    AutoPtr<CForkJoinWorkerThread> cw = (CForkJoinWorkerThread*)w;
    Int32 idx = cw->mPoolIndex;
    Int32 sc = cw->mStealCount;
    Int32 steps = 0;
    // Remove from array, adjust worker counts and collect steal count.
    // We can intermix failed removes or adjusts with steal updates
    do {
        Int64 s, c;
        Int32 g;
        assert(0 && "TODO");
        if (steps == 0 && ((g = mScanGuard) & SG_UNIT) == 0/* &&
            UNSAFE.compareAndSwapInt(this, scanGuardOffset,
                                     g, g |= SG_UNIT)*/) {
            AutoPtr<ArrayOf<IForkJoinWorkerThread*> > ws = mWorkers;
            if (ws != NULL && idx >= 0 &&
                idx < ws->GetLength() &&
                ObjectUtils::Equals((*ws)[idx]->Probe(EIID_IInterface), w->Probe(EIID_IInterface)))
                (*ws)[idx] = NULL;    // verify
            mNextWorkerIndex = idx;
            mScanGuard = g + SG_UNIT;
            steps = 1;
        }
        // if (steps == 1 &&
        //     UNSAFE.compareAndSwapLong(this, ctlOffset, c = mCtl,
        //                               (((c - AC_UNIT) & AC_MASK) |
        //                                ((c - TC_UNIT) & TC_MASK) |
        //                                (c & ~(AC_MASK|TC_MASK)))))
        //     steps = 2;
        // if (sc != 0 &&
        //     UNSAFE.compareAndSwapLong(this, stealCountOffset,
        //                               s = mStealCount, s + sc))
        //     sc = 0;
    } while (steps != 2 || sc != 0);
    if (!TryTerminate(FALSE)) {
        if (ex != NULL)   // possibly replace if died abnormally
            SignalWork();
        else
            TryReleaseWaiter();
    }
}

Boolean CForkJoinPool::TryTerminate(
    /* [in] */ const Boolean& now)
{
    Int64 c;
    while (((c = mCtl) & STOP_BIT) == 0) {
        if (!now) {
            if ((Int32)(c >> AC_SHIFT) != -mParallelism)
                return FALSE;
            if (!mShutdown || mBlockedCount != 0 || mQuiescerCount != 0 ||
                mQueueBase != mQueueTop) {
                if (mCtl == c) // staleness check
                    return FALSE;
                continue;
            }
        }
        assert(0 && "TODO");
        // if (UNSAFE.compareAndSwapLong(this, ctlOffset, c, c | STOP_BIT))
        //     StartTerminating();
    }
    if ((Int16)(c >> TC_SHIFT) == -mParallelism) { // signal when 0 workers
        AutoPtr<IReentrantLock> lock = mSubmissionLock;
        lock->Lock();
        mTermination->SignalAll();
        lock->UnLock();
    }
    return TRUE;
}

void CForkJoinPool::StartTerminating()
{
    CancelSubmissions();
    for (Int32 pass = 0; pass < 3; ++pass) {
        AutoPtr<ArrayOf<IForkJoinWorkerThread*> > ws = mWorkers;
        if (ws != NULL) {
            for (Int32 i = 0;i < ws->GetLength();i++) {
                AutoPtr<IForkJoinWorkerThread> w = (*ws)[i];
                if (w != NULL) {
                    AutoPtr<CForkJoinWorkerThread> cw = (CForkJoinWorkerThread*)w.Get();
                    cw->mTerminate = TRUE;
                    if (pass > 0) {
                        w->CancelTasks();
                        Boolean b = FALSE;
                        if (pass > 1 && !(w->IsInterrupted(&b), b)) {
                            w->Interrupt();
                        }
                    }
                }
            }
            TerminateWaiters();
        }
    }
}

void CForkJoinPool::CancelSubmissions()
{
    while (mQueueBase != mQueueTop) {
        AutoPtr<IForkJoinTask> task = PollSubmission();
        if (task != NULL) {
            Boolean b = FALSE;
            task->Cancel(FALSE, &b);
        }
    }
}

void CForkJoinPool::TerminateWaiters()
{
    assert(0 && "TODO");
    // AutoPtr<ArrayOf<IForkJoinWorkerThread*> > ws = mWorkers;
    // if (ws != NULL) {
    //     AutoPtr<IForkJoinWorkerThread> w;
    //     Int64 c;
    //     Int32 i, e;
    //     Int32 n = ws->GetLength();
    //     while ((i = ~(e = (Int32)(c = mCtl)) & SMASK) < n &&
    //            (w = (*ws)[i]) != NULL && w->mEventCount == (e & E_MASK)) {
    //         if (UNSAFE.compareAndSwapLong(this, ctlOffset, c,
    //                                       (Int64)(w->mNextWait & E_MASK) |
    //                                       ((c + AC_UNIT) & AC_MASK) |
    //                                       (c & (TC_MASK|STOP_BIT)))) {
    //             w->mTerminate = TRUE;
    //             w->mEventCount = e + EC_UNIT;
    //             if (w->mParked)
    //                 UNSAFE.unpark(w);
    //         }
    //     }
    // }
}

void CForkJoinPool::AddQuiescerCount(
    /* [in] */ const Int32& delta)
{
    assert(0 && "TODO");
    // Int32 c;
    // do {} while (!UNSAFE.compareAndSwapInt(this, quiescerCountOffset,
    //                                       c = mQuiescerCount, c + delta));
}

void CForkJoinPool::AddActiveCount(
    /* [in] */ const Int32& delta)
{
    assert(0 && "TODO");
    Int64 d = (Int64)delta << AC_SHIFT;
    // Int64 c;
    // do {} while (!UNSAFE.compareAndSwapLong(this, ctlOffset,
    //                                         c = mCtl, c + d));
}

Int32 CForkJoinPool::IdlePerActive()
{
    // Approximate at powers of two for small values, saturate past 4
    Int32 p = mParallelism;
    Int32 a = p + (Int32)(mCtl >> AC_SHIFT);
    return (a > (p >>= 1) ? 0 :
            a > (p >>= 1) ? 1 :
            a > (p >>= 1) ? 2 :
            a > (p >>= 1) ? 4 :
            8);
}

ECode CForkJoinPool::constructor()
{
    Int32 ps = 4; //Runtime.getRuntime().availableProcessors()
    return constructor(ps,
        mDefaultForkJoinWorkerThreadFactory, NULL, FALSE);
}

ECode CForkJoinPool::constructor(
    /* [in] */ const Int32& parallelism)
{
    return constructor(parallelism, mDefaultForkJoinWorkerThreadFactory, NULL, FALSE);
}

ECode CForkJoinPool::constructor(
    /* [in] */ const Int32& parallelism,
    /* [in] */ IForkJoinPoolForkJoinWorkerThreadFactory* factory,
    /* [in] */ IThreadUncaughtExceptionHandler* handler,
    /* [in] */ const Boolean& asyncMode)
{
    CheckPermission();
    if (factory == NULL)
        return E_NULL_POINTER_EXCEPTION;
    if (parallelism <= 0 || parallelism > MAX_ID)
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    mParallelism = parallelism;
    mFactory = factory;
    mUeh = handler;
    mLocallyFifo = asyncMode;
    Int64 np = (Int64)(-parallelism); // offset ctl counts
    mCtl = ((np << AC_SHIFT) & AC_MASK) | ((np << TC_SHIFT) & TC_MASK);
    mSubmissionQueue = ArrayOf<IForkJoinTask*>::Alloc(INITIAL_QUEUE_CAPACITY);
    // initialize workers array with room for 2*parallelism if possible
    Int32 n = parallelism << 1;
    if (n >= MAX_ID)
        n = MAX_ID;
    else { // See Hackers Delight, sec 3.2, where n < (1 << 16)
        n |= n >> 1; n |= n >> 2; n |= n >> 4; n |= n >> 8;
    }
    mWorkers = ArrayOf<IForkJoinWorkerThread*>::Alloc(n + 1);
    CReentrantLock::New((IReentrantLock**)&mSubmissionLock);
    mSubmissionLock->NewCondition((ICondition**)&mTermination);
    StringBuilder sb("ForkJoinPool-");
    Int32 val;
    mPoolNumberGenerator->IncrementAndGet(&val);
    sb += val;
    sb += "-worker-";
    sb.ToString(&mWorkerNamePrefix);
    return NOERROR;
}

PInterface CForkJoinPool::Probe(
        /* [in] */ REIID riid)
{
    return _CForkJoinPool::Probe(riid);
}

ECode CForkJoinPool::Invoke(
    /* [in] */ IForkJoinTask* task,
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface);
    AutoPtr<IThread> t = Thread::GetCurrentThread();
    if (task == NULL)
        return E_NULL_POINTER_EXCEPTION;
    if (mShutdown)
        return E_REJECTED_EXECUTION_EXCEPTION;
    if (t->Probe(EIID_IForkJoinWorkerThread) != NULL ) {
        AutoPtr<IForkJoinWorkerThread> p = (IForkJoinWorkerThread*)t->Probe(EIID_IForkJoinWorkerThread);
        AutoPtr<CForkJoinWorkerThread> cp = (CForkJoinWorkerThread*)p.Get();
        if ( ObjectUtils::Equals(cp->mPool->Probe(EIID_IInterface), THIS_PROBE(IInterface)) )
            return task->Invoke(outface);  // bypass submit if in same pool
    }
    else {
        AddSubmission(task);
        return task->Join(outface);
    }
}

void CForkJoinPool::ForkOrSubmit(
    /* [in] */ IForkJoinTask* task)
{
    AutoPtr<IForkJoinWorkerThread> w;
    AutoPtr<IThread> t = Thread::GetCurrentThread();
    if (mShutdown) return;
//        return E_REJECTED_EXECUTION_EXCEPTION;
    if (t->Probe(EIID_IForkJoinWorkerThread) != NULL) {
        w = (IForkJoinWorkerThread*)t->Probe(EIID_IForkJoinWorkerThread);
        AutoPtr<CForkJoinWorkerThread> cw = (CForkJoinWorkerThread*)w.Get();
        if ( ObjectUtils::Equals(cw->mPool->Probe(EIID_IInterface), THIS_PROBE(IInterface)) )
            w->PushTask(task);
    }
    else
        AddSubmission(task);
}

ECode CForkJoinPool::ExecuteEx(
    /* [in] */ IForkJoinTask* task)
{
    if (task == NULL)
        return E_NULL_POINTER_EXCEPTION;
    ForkOrSubmit(task);
    return NOERROR;
}

ECode CForkJoinPool::Execute(
    /* [in] */ IRunnable* task)
{
    if (task == NULL)
        return E_NULL_POINTER_EXCEPTION;
    AutoPtr<IForkJoinTask> job;
    if (task->Probe(EIID_IForkJoinTask) != NULL) // avoid re-wrap
        job = (IForkJoinTask*) task;
    else
        job = ForkJoinTask::AdaptEx(task, NULL);
    ForkOrSubmit(job);
    return NOERROR;
}

ECode CForkJoinPool::SubmitEx3(
    /* [in] */ IForkJoinTask* task,
    /* [out] */ IForkJoinTask** outfork)
{
    VALIDATE_NOT_NULL(outfork);
    if (task == NULL)
        return E_NULL_POINTER_EXCEPTION;
    ForkOrSubmit(task);
    *outfork = task;
    INTERFACE_ADDREF(*outfork);
    return NOERROR;
}

ECode CForkJoinPool::Submit(
    /* [in] */ ICallable* task,
    /* [out] */ IFuture** outfork)
{
    VALIDATE_NOT_NULL(outfork);
    if (task == NULL)
        return E_NULL_POINTER_EXCEPTION;
    AutoPtr<IForkJoinTask> job = ForkJoinTask::AdaptEx2(task);
    ForkOrSubmit(job);
    *outfork = (IFuture*)job->Probe(EIID_IFuture);
    INTERFACE_ADDREF(*outfork);
    return NOERROR;
}

ECode CForkJoinPool::SubmitEx(
    /* [in] */ IRunnable* task,
    /* [in] */ IInterface* result,
    /* [out] */ IFuture** outfork)
{
    VALIDATE_NOT_NULL(outfork);
    if (task == NULL)
        return E_NULL_POINTER_EXCEPTION;
    AutoPtr<IForkJoinTask> job = ForkJoinTask::AdaptEx(task, result);
    ForkOrSubmit(job);
    *outfork = (IFuture*)job->Probe(EIID_IFuture);
    INTERFACE_ADDREF(*outfork);
    return NOERROR;
}

ECode CForkJoinPool::SubmitEx2(
    /* [in] */ IRunnable* task,
    /* [out] */ IFuture** outfork)
{
    VALIDATE_NOT_NULL(outfork);
    if (task == NULL)
        return E_NULL_POINTER_EXCEPTION;
    AutoPtr<IForkJoinTask> job;
    if (task->Probe(EIID_IForkJoinTask) != NULL) // avoid re-wrap
        job = (IForkJoinTask*) task;
    else
        job = ForkJoinTask::AdaptEx(task, NULL);
    ForkOrSubmit(job);
    *outfork = (IFuture*)job->Probe(EIID_IFuture);
    INTERFACE_ADDREF(*outfork);
    return NOERROR;
}

ECode CForkJoinPool::InvokeAll(
    /* [in] */ ICollection* tasks,
    /* [out] */ IList** futures)
{
    VALIDATE_NOT_NULL(futures);
    Int32 size;
    tasks->GetSize(&size);
    AutoPtr<IArrayList> forkJoinTasks;
    CArrayList::New(size, (IArrayList**)&forkJoinTasks);
    AutoPtr<ArrayOf<IInterface*> > arr;
    tasks->ToArray((ArrayOf<IInterface*>**)&arr);
    for (Int32 i = 0;i < size; i++) {
        AutoPtr<IInterface> p = (*arr)[i];
        AutoPtr<ICallable> task = (ICallable*)p->Probe(EIID_ICallable);
        Boolean b = FALSE;
        forkJoinTasks->Add(ForkJoinTask::AdaptEx2(task), &b);
    }
    assert(0 && "TODO");
//    AutoPtr<_InvokeAll> p = new _InvokeAll(this, forkJoinTasks);
//    Invoke(p);
    AutoPtr<IList> l = (IList*) forkJoinTasks->Probe(EIID_IList);
    *futures = l;
    INTERFACE_ADDREF(*futures);
    return NOERROR;
}

ECode CForkJoinPool::InvokeAllEx(
    /* [in] */ ICollection* tasks,
    /* [in] */ Int64 timeout,
    /* [in] */ ITimeUnit* unit,
    /* [out] */ IList** futures)
{
    VALIDATE_NOT_NULL(futures);
    return AbstractExecutorService::InvokeAllEx(tasks, timeout, unit, futures);
}

ECode CForkJoinPool::InvokeAny(
    /* [in] */ ICollection* tasks,
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);
    return AbstractExecutorService::InvokeAny(tasks, result);
}

ECode CForkJoinPool::InvokeAnyEx(
    /* [in] */ ICollection* tasks,
    /* [in] */ Int64 timeout,
    /* [in] */ ITimeUnit* unit,
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);
    return AbstractExecutorService::InvokeAnyEx(tasks, timeout, unit, result);
}

//====================================================================
// CForkJoinPool::_InvokeAll::
//====================================================================

CForkJoinPool::_InvokeAll::_InvokeAll(
    /* [in] */ CForkJoinPool* owner,
    /* [in] */ IArrayList* tasks)
{
    mOwner = owner;
    mTasks = tasks;
}

void CForkJoinPool::_InvokeAll::Compute()
{
    AutoPtr<IList> l;
    mOwner->InvokeAll(mTasks, (IList**)&l);
}

//====================================================================
// CForkJoinPool::
//====================================================================

ECode CForkJoinPool::GetFactory(
    /* [out] */ IForkJoinPoolForkJoinWorkerThreadFactory** res)
{
    VALIDATE_NOT_NULL(res);
    *res = mFactory;
    INTERFACE_ADDREF(*res);
    return NOERROR;
}

ECode CForkJoinPool::GetUncaughtExceptionHandler(
    /* [out] */ IThreadUncaughtExceptionHandler** res)
{
    VALIDATE_NOT_NULL(res);
    *res = mUeh;
    INTERFACE_ADDREF(*res);
    return NOERROR;
}

ECode CForkJoinPool::GetParallelism(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    *value = mParallelism;
    return NOERROR;
}

ECode CForkJoinPool::GetPoolSize(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    *value = mParallelism + (Int16)(mCtl >> TC_SHIFT);
    return NOERROR;
}

ECode CForkJoinPool::GetAsyncMode(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value);
    *value = mLocallyFifo;
    return NOERROR;
}

ECode CForkJoinPool::GetRunningThreadCount(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    Int32 r = mParallelism + (Int32)(mCtl >> AC_SHIFT);
    *value = (r <= 0) ? 0 : r; // suppress momentarily negative values
    return NOERROR;
}

ECode CForkJoinPool::GetActiveThreadCount(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    Int32 r = mParallelism + (Int32)(mCtl >> AC_SHIFT) + mBlockedCount;
    *value = (r <= 0) ? 0 : r; // suppress momentarily negative values
    return NOERROR;
}

ECode CForkJoinPool::IsQuiescent(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value);
    *value = mParallelism + (Int32)(mCtl >> AC_SHIFT) + mBlockedCount == 0;
    return NOERROR;
}

ECode CForkJoinPool::GetStealCount(
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value);
    *value = mStealCount;
    return NOERROR;
}

ECode CForkJoinPool::GetQueuedTaskCount(
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value);
    Int64 count = 0;
    AutoPtr<ArrayOf<IForkJoinWorkerThread*> > ws;
    if ((Int16)(mCtl >> TC_SHIFT) > -mParallelism &&
        (ws = mWorkers) != NULL) {
        for (Int32 i = 0; i < ws->GetLength();i++) {
            AutoPtr<IForkJoinWorkerThread> w = (*ws)[i];
            if (w != NULL) {
                AutoPtr<CForkJoinWorkerThread> cw = (CForkJoinWorkerThread*)w.Get();
                count -= cw->mQueueBase - cw->mQueueTop; // must read base first
            }
        }
    }
    *value = count;
    return NOERROR;
}

ECode CForkJoinPool::GetQueuedSubmissionCount(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    *value = -mQueueBase + mQueueTop;
    return NOERROR;
}

ECode CForkJoinPool::HasQueuedSubmissions(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value);
    *value = mQueueBase != mQueueTop;
    return NOERROR;
}

AutoPtr<IForkJoinTask> CForkJoinPool::PollSubmission()
{
    assert(0 && "TODO");
    // AutoPtr<IForkJoinTask> t;
    // AutoPtr<ArrayOf<IForkJoinTask*> > q;
    // Int32 b, i;
    // while ((b = mQueueBase) != mQueueTop &&
    //       (q = mSubmissionQueue) != NULL &&
    //       (i = (q->GetLength() - 1) & b) >= 0) {
    //     Int64 u = (i << ASHIFT) + ABASE;
    //     if ((t = (*q)[i]) != NULL &&
    //         mQueueBase == b &&
    //         UNSAFE.compareAndSwapObject(q, u, t, NULL)) {
    //         mQueueBase = b + 1;
    //         return t;
    //     }
    // }
    return NULL;
}

Int32 CForkJoinPool::DrainTasksTo(
    /* [in] */ ICollection* c)
{
    Int32 count = 0;
    while (mQueueBase != mQueueTop) {
        AutoPtr<IForkJoinTask> t = PollSubmission();
        if (t != NULL) {
            Boolean b = FALSE;
            c->Add(t, &b);
            ++count;
        }
    }
    AutoPtr<ArrayOf<IForkJoinWorkerThread*> > ws;
    if ((Int16)(mCtl >> TC_SHIFT) > -mParallelism &&
        (ws = mWorkers) != NULL) {
        for (Int32 i = 0;i < ws->GetLength();i++) {
            AutoPtr<IForkJoinWorkerThread> w = (*ws)[i];
            if (w != NULL) {
                Int32 num;
                w->DrainTasksTo(c, &num);
                count += num;
            }
        }
    }
    return count;
}

ECode CForkJoinPool::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    Int64 st, qt;
    Int32 qs;
    GetStealCount(&st);
    GetQueuedTaskCount(&qt);
    GetQueuedSubmissionCount(&qs);
    Int32 pc = mParallelism;
    Int64 c = mCtl;
    Int32 tc = pc + (Int16)(c >> TC_SHIFT);
    Int32 rc = pc + (Int32)(c >> AC_SHIFT);
    if (rc < 0) // ignore transient negative
        rc = 0;
    Int32 ac = rc + mBlockedCount;
    String level;
    if ((c & STOP_BIT) != 0)
        level = (tc == 0) ? "Terminated" : "Terminating";
    else
        level = mShutdown ? "Shutting down" : "Running";

    StringBuilder sb("[");
    sb += level;
    sb += ", parallelism = "; sb += pc;
    sb += ", size = "; sb += tc;
    sb += ", active = "; sb += ac;
    sb += ", running = "; sb += rc;
    sb += ", steals = "; sb += st;
    sb += ", tasks = "; sb += qt;
    sb += ", submissions = "; sb += qs;
    sb += "]";
    sb.ToString(str);
    return NOERROR;
}

ECode CForkJoinPool::Shutdown()
{
    CheckPermission();
    mShutdown = TRUE;
    TryTerminate(FALSE);
    return NOERROR;
}

ECode CForkJoinPool::ShutdownNow(
    /* [out] */ IList** tasks)
{
    VALIDATE_NOT_NULL(tasks);
    CheckPermission();
    mShutdown = TRUE;
    TryTerminate(TRUE);
    return CCollections::_GetEmptyList(tasks);
}

ECode CForkJoinPool::IsTerminated(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    Int64 c = mCtl;
    *result = ((c & STOP_BIT) != 0L &&
            (Int16)(c >> TC_SHIFT) == -mParallelism);
    return NOERROR;
}

ECode CForkJoinPool::IsTerminating(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value);
    Int64 c = mCtl;
    *value = ((c & STOP_BIT) != 0L &&
            (Int16)(c >> TC_SHIFT) != -mParallelism);
    return NOERROR;
}

ECode CForkJoinPool::IsAtLeastTerminating(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value);
    *value = (mCtl & STOP_BIT) != 0L;
    return NOERROR;
}

ECode CForkJoinPool::IsShutdown(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mShutdown;
    return NOERROR;
}

ECode CForkJoinPool::AwaitTermination(
    /* [in] */ Int64 timeout,
    /* [in] */ ITimeUnit* unit,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    Int64 nanos;
    unit->ToNanos(timeout, &nanos);
    AutoPtr<IReentrantLock> lock = mSubmissionLock;
    lock->Lock();
    for (;;) {
        Boolean b = FALSE;
        if ((IsTerminated(&b), b)) {
            *result = TRUE;
            lock->UnLock();
            return NOERROR;
        }
        if (nanos <= 0) {
            *result = FALSE;
            lock->UnLock();
            return NOERROR;
        }
        Int64 num = 0;
        mTermination->AwaitNanos(nanos, &num);
        nanos = num;
    }
}

void CForkJoinPool::ManagedBlock(
    /* [in] */ IForkJoinPoolManagedBlocker* blocker)
{
    AutoPtr<IThread> t = Thread::GetCurrentThread();
    if (t->Probe(EIID_IForkJoinWorkerThread) != NULL) {
        AutoPtr<IForkJoinWorkerThread> w = (IForkJoinWorkerThread*) t->Probe(EIID_IForkJoinWorkerThread);
        AutoPtr<CForkJoinWorkerThread> cw = (CForkJoinWorkerThread*) w.Get();
        AutoPtr<CForkJoinPool> cp = (CForkJoinPool*)cw->mPool.Get();
        cp->AwaitBlocker(blocker);
    }
    else {
        Boolean a = FALSE, b = FALSE;
        do {} while (!(blocker->IsReleasable(&a), a) && !(blocker->Block(&b), b));
    }
}

AutoPtr<IRunnableFuture> CForkJoinPool::NewTaskFor(
    /* [in] */ IRunnable* runnable,
    /* [in] */ IInterface* value)
{
    AutoPtr<IForkJoinTask> p = ForkJoinTask::AdaptEx(runnable, value);
    AutoPtr<IFuture> pf = (IFuture*)p->Probe(EIID_IFuture);
    return (IRunnableFuture*)pf.Get();
}

AutoPtr<IRunnableFuture> CForkJoinPool::NewTaskFor(
    /* [in] */ ICallable* callable)
{
    AutoPtr<IForkJoinTask> p = ForkJoinTask::AdaptEx2(callable);
    AutoPtr<IFuture> pf = (IFuture*)p->Probe(EIID_IFuture);
    return (IRunnableFuture*)pf.Get();
}

} // namespace Concurrent
} // namespace Utility
} // namespace Elastos
