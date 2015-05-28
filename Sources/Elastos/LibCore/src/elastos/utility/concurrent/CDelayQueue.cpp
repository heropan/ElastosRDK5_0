
#include "CDelayQueue.h"
#include <Math.h>
#include <elastos/ObjectUtils.h>
#include "TimeUnit.h"
#include "Thread.h"

using Elastos::Core::Math;
using Elastos::Core::ObjectUtils;

namespace Elastos {
namespace Utility {
namespace Concurrent {

//====================================================================
// CDelayQueue::
//====================================================================

ECode CDelayQueue::constructor()
{
    return NOERROR;
}


ECode CDelayQueue::constructor(
    /* [in] */ ICollection* c)
{
    Boolean b;
    return AbstractQueue::AddAll(c, &b);
}

PInterface CDelayQueue::Probe(
    /* [in] */ REIID riid)
{
    return _CDelayQueue::Probe(riid);
}

ECode CDelayQueue::Add(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    VALIDATE_NOT_NULL(modified);
    return Offer(object, modified);
}

ECode CDelayQueue::Offer(
    /* [in] */ IInterface* e,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<IReentrantLock> lock = mLock;
    lock->Lock();
    Boolean b = FALSE;
    mQ->Offer(e, &b);
    AutoPtr<IInterface> p;
    mQ->Peek((IInterface**)&p);
    if (ObjectUtils::Equals(p, e)) {
        mLeader = NULL;
        mAvailable->Signal();
    }
    *result = TRUE;
    lock->UnLock();
    return NOERROR;
}

ECode CDelayQueue::Put(
    /* [in] */ IInterface* e)
{
    Boolean b = FALSE;
    return Offer(e, &b);
}

ECode CDelayQueue::Offer(
    /* [in] */ IInterface* e,
    /* [in] */ Int64 timeout,
    /* [in] */ ITimeUnit* unit,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return Offer(e, result);
}

ECode CDelayQueue::Poll(
    /* [out] */ IInterface** e)
{
    VALIDATE_NOT_NULL(e);
    *e = NULL;

    AutoPtr<IReentrantLock> lock = mLock;
    lock->Lock();

    ECode ec = NOERROR;
    AutoPtr<IInterface> first;
    mQ->Peek((IInterface**)&first);
    if (first != NULL) {
        AutoPtr<ITimeUnit> tu = TimeUnit::GetNANOSECONDS();
        AutoPtr<IDelayed> d = IDelayed::Probe(first.Get());
        Int64 val = 0;
        d->GetDelay(tu, &val);
        if (val <= 0) {
            ec = mQ->Poll(e);
        }
    }

    lock->UnLock();
    return ec;
}

ECode CDelayQueue::Take(
    /* [out] */ IInterface** e)
{
    VALIDATE_NOT_NULL(e);
    *e = NULL;

    AutoPtr<IReentrantLock> lock = mLock;
    lock->LockInterruptibly();

    for (;;) {
        AutoPtr<IInterface> first;
        mQ->Peek((IInterface**)&first);
        if (first == NULL) {
            mAvailable->Await();
        }
        else {
            AutoPtr<ITimeUnit> tu = TimeUnit::GetNANOSECONDS();
            AutoPtr<IDelayed> d = IDelayed::Probe(first);
            Int64 delay = 0;
            d->GetDelay(tu, &delay);
            if (delay <= 0) {
                ECode ec = mQ->Poll(e);
                lock->UnLock();
                return ec;
            }
            else if (mLeader != NULL)
                mAvailable->Await();
            else {
                AutoPtr<IThread> thisThread = Thread::GetCurrentThread();
                mLeader = thisThread;
                Int64 val = 0;
                mAvailable->AwaitNanos(delay, &val);
                if (mLeader == thisThread ||
                    ObjectUtils::Equals(mLeader->Probe(EIID_IInterface), thisThread->Probe(EIID_IInterface)))
                    mLeader = NULL;
            }
        }
    }
    AutoPtr<IInterface> p;
    if (mLeader == NULL && (mQ->Peek((IInterface**)&p), p) != NULL)
        mAvailable->Signal();
    lock->UnLock();
    return NOERROR;
}

ECode CDelayQueue::Poll(
    /* [in] */ Int64 timeout,
    /* [in] */ ITimeUnit* unit,
    /* [out] */ IInterface** e)
{
    VALIDATE_NOT_NULL(e);
    *e = NULL;

    Int64 nanos;
    unit->ToNanos(timeout, &nanos);
    AutoPtr<IReentrantLock> lock = mLock;
    lock->LockInterruptibly();
    for (;;) {
        AutoPtr<IInterface> first;
        mQ->Peek((IInterface**)&first);
        if (first == NULL) {
            if (nanos <= 0) {
                lock->UnLock();
                return NOERROR;
            }
            else {
                Int64 val;
                mAvailable->AwaitNanos(nanos, &val);
                nanos = val;
            }
        }
        else {
            AutoPtr<ITimeUnit> tu = TimeUnit::GetNANOSECONDS();
            AutoPtr<IDelayed> d = IDelayed::Probe(first);
            Int64 delay = 0;
            d->GetDelay(tu, &delay);
            if (delay <= 0) {
                mQ->Poll(e);
                lock->UnLock();
                return NOERROR;
            }
            if (nanos <= 0) {
                lock->UnLock();
                return NOERROR;
            }
            if (nanos < delay || mLeader != NULL) {
                Int64 val;
                mAvailable->AwaitNanos(nanos, &val);
                nanos = val;
            }
            else {
                AutoPtr<IThread> thisThread = Thread::GetCurrentThread();
                mLeader = thisThread;
                Int64 timeLeft;
                mAvailable->AwaitNanos(delay, &timeLeft);
                nanos -= delay - timeLeft;
                if (mLeader == thisThread ||
                    ObjectUtils::Equals(mLeader->Probe(EIID_IInterface), thisThread->Probe(EIID_IInterface)))
                    mLeader = NULL;
            }
        }
    }

    AutoPtr<IInterface> o;
    if (mLeader == NULL && (mQ->Peek((IInterface**)&o), o) != NULL)
        mAvailable->Signal();
    lock->UnLock();
    return NOERROR;
}

ECode CDelayQueue::Peek(
    /* [out] */ IInterface** e)
{
    VALIDATE_NOT_NULL(e);
    AutoPtr<IReentrantLock> lock = mLock;
    lock->Lock();
    AutoPtr<IInterface> o;
    ECode ec = mQ->Peek((IInterface**)&o);
    lock->UnLock();
    return ec;
}

ECode CDelayQueue::GetSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);
    AutoPtr<IReentrantLock> lock = mLock;
    lock->Lock();
    ECode ec = mQ->GetSize(size);
    lock->UnLock();
    return ec;
}

AutoPtr<IInterface> CDelayQueue::PeekExpired()
{
    // assert lock.isHeldByCurrentThread();
    AutoPtr<IInterface> first;
    mQ->Peek((IInterface**)&first);
    if (first == NULL) {
        return NULL;
    }
    else {
        AutoPtr<ITimeUnit> tu = TimeUnit::GetNANOSECONDS();
        AutoPtr<IDelayed> d = IDelayed::Probe(first);
        Int64 val = 0;
        d->GetDelay(tu, &val);
        return val > 0 ? NULL : first;
    }
}

ECode CDelayQueue::DrainTo(
    /* [in] */ ICollection* c,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    *number = 0;

    if (c == NULL)
        return E_NULL_POINTER_EXCEPTION;
    if (ObjectUtils::Equals(c->Probe(EIID_IInterface), THIS_PROBE(IInterface)))
        return E_ILLEGAL_ARGUMENT_EXCEPTION;

    AutoPtr<IReentrantLock> lock = mLock;
    lock->Lock();
    Int32 n = 0;
    Boolean b;
    AutoPtr<IInterface> e;
    for (; (e = PeekExpired()) != NULL;) {
        c->Add(e, &b);       // In this order, in case add() throws.
        AutoPtr<IInterface> pRes;
        mQ->Poll((IInterface**)&pRes);
        ++n;
    }
    *number = n;
    lock->UnLock();
    return NOERROR;
}

ECode CDelayQueue::DrainTo(
    /* [in] */ ICollection* c,
    /* [in] */ Int32 maxElements,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    *number = 0;

    if (c == NULL)
        return E_NULL_POINTER_EXCEPTION;
    if (ObjectUtils::Equals(c->Probe(EIID_IInterface), THIS_PROBE(IInterface)))
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    if (maxElements <= 0) {
        return NOERROR;
    }

    AutoPtr<IReentrantLock> lock = mLock;
    lock->Lock();
    Int32 n = 0;
    Boolean b;
    AutoPtr<IInterface> e;
    for (; n < maxElements && (e = PeekExpired()) != NULL;) {
        c->Add(e, &b);       // In this order, in case add() throws.
        AutoPtr<IInterface> pRes;
        mQ->Poll((IInterface**)&pRes);
        ++n;
    }
    *number = n;
    lock->UnLock();
    return NOERROR;
}

ECode CDelayQueue::Clear()
{
    AutoPtr<IReentrantLock> lock = mLock;
    lock->Lock();
    ECode ec = mQ->Clear();
    lock->UnLock();
    return ec;
}

ECode CDelayQueue::RemainingCapacity(
    /* [out] */ Int32* capacity)
{
    VALIDATE_NOT_NULL(capacity);
    *capacity = Elastos::Core::Math::INT32_MAX_VALUE;
    return NOERROR;
}

ECode CDelayQueue::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    VALIDATE_NOT_NULL(array);
    AutoPtr<IReentrantLock> lock = mLock;
    lock->Lock();
    ECode ec = mQ->ToArray(array);
    lock->UnLock();
    return ec;
}

ECode CDelayQueue::ToArray(
    /* [in] */ ArrayOf<IInterface*>* inArray,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    VALIDATE_NOT_NULL(outArray);
    AutoPtr<IReentrantLock> lock = mLock;
    lock->Lock();
    ECode ec = mQ->ToArray(inArray, outArray);
    lock->UnLock();
    return ec;
}

ECode CDelayQueue::Remove(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    VALIDATE_NOT_NULL(modified);
    AutoPtr<IReentrantLock> lock = mLock;
    lock->Lock();
    ECode ec = mQ->Remove(object, modified);
    lock->UnLock();
    return ec;
}

void CDelayQueue::RemoveEQ(
    /* [in] */ IInterface* o)
{
    AutoPtr<IReentrantLock> lock = mLock;
    lock->Lock();
    AutoPtr<IIterator> it;
    mQ->GetIterator((IIterator**)&it);
    Boolean b = FALSE;
    for (; (it->HasNext(&b), b); ) {
        AutoPtr<IInterface> nt;
        it->Next((IInterface**)&nt);
        if (ObjectUtils::Equals(o, nt)) {
            it->Remove();
            break;
        }
    }
    lock->UnLock();
}

ECode CDelayQueue::GetIterator(
    /* [out] */ IIterator** it)
{
    VALIDATE_NOT_NULL(it);
    AutoPtr<ArrayOf<IInterface*> > arr;
    ToArray((ArrayOf<IInterface*>**)&arr);
    AutoPtr<IWeakReference> wr;
    GetWeakReference((IWeakReference**)&wr);
    AutoPtr<Itr> p = new Itr(wr.Get(), arr);
    *it = (IIterator*)p.Get();
    REFCOUNT_ADD(*it);
    return NOERROR;
}

ECode CDelayQueue::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractQueue::AddAll(collection, modified);
}

ECode CDelayQueue::Contains(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return AbstractQueue::Contains(object, result);
}

ECode CDelayQueue::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return AbstractQueue::ContainsAll(collection, result);
}

ECode CDelayQueue::IsEmpty(
    /* [out] */ Boolean* result)
{
    return AbstractQueue::IsEmpty(result);
}

ECode CDelayQueue::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractQueue::RemoveAll(collection, modified);
}

ECode CDelayQueue::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractQueue::RetainAll(collection, modified);
}

ECode CDelayQueue::Remove(
    /* [out] */ IInterface** e)
{
    return AbstractQueue::Remove(e);
}

ECode CDelayQueue::Element(
    /* [out] */ IInterface** e)
{
    return AbstractQueue::Element(e);
}

//====================================================================
// CDelayQueue::Itr::
//====================================================================
CAR_INTERFACE_IMPL(CDelayQueue::Itr, IIterator);

CDelayQueue::Itr::Itr(
    /* [in] */ IWeakReference* owner,
    /* [in] */ ArrayOf<IInterface*>* array)
{
    mLastRet = -1;
    mArray = array;
    mWeakOwner = owner;
}

ECode CDelayQueue::Itr::HasNext(
    /* [out] */ Boolean* res)
{
    *res = mCursor < mArray->GetLength();
    return NOERROR;
}

ECode CDelayQueue::Itr::Next(
    /* [out] */ IInterface** res)
{
    if (mCursor >= mArray->GetLength())
        return E_NO_SUCH_METHOD_EXCEPTION;
    mLastRet = mCursor;
    *res = (*mArray)[mCursor++];
    REFCOUNT_ADD(*res);
    return NOERROR;
}

ECode CDelayQueue::Itr::Remove()
{
    if (mLastRet < 0)
        return E_ILLEGAL_STATE_EXCEPTION;

    AutoPtr<IBlockingQueue> queue;
    mWeakOwner->Resolve(EIID_IBlockingQueue, (IInterface**)&queue);
    if (queue) {
        CDelayQueue* dq = (CDelayQueue*)queue.Get();
        dq->RemoveEQ((*mArray)[mLastRet]);
    }

    mLastRet = -1;
    return NOERROR;
}

} // namespace Concurrent
} // namespace Utility
} // namespace Elastos
