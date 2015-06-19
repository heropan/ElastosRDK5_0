
#include "CExchanger.h"
#include "LockSupport.h"
#include "Thread.h"
#include <Math.h>
#include "CAtomicInteger32.h"

using Elastos::Core::Math;
using Elastos::Utility::Concurrent::Locks::LockSupport;
using Elastos::Utility::Concurrent::Atomic::EIID_IAtomicReference;
using Elastos::Utility::Concurrent::Atomic::CAtomicInteger32;

namespace Elastos {
namespace Utility {
namespace Concurrent {

//====================================================================
// CExchanger::Node::
//====================================================================
CExchanger::Node::Node(
    /* [in] */ IInterface* item)
{
    mItem = item;
}

//====================================================================
// CExchanger::Slot::
//====================================================================
CExchanger::Slot::Slot()
    : mQ0(0), mQ1(0), mQ2(0), mQ3(0)
    , mQ4(0), mQ5(0), mQ6(0), mQ7(0)
    , mQ8(0), mQ9(0), mQa(0), mQb(0)
    , mQc(0), mQd(0), mQe(0)
{}

//====================================================================
// CExchanger::
//====================================================================
CAR_INTERFACE_IMPL(CExchanger, Object, IExchanger)

CAR_OBJECT_IMPL(CExchanger);

const Int32 CExchanger::sNCPU = 4;//= Runtime.getRuntime().availableProcessors();
const Int32 CExchanger::sCAPACITY = 32;
Int32 CExchanger::sFULL = Elastos::Core::Math::Max(0, Elastos::Core::Math::Min(sCAPACITY, sNCPU / 2) - 1);
Int32 CExchanger::sSPINS = (sNCPU == 1) ? 0 : 2000;
Int32 CExchanger::sTIMED_SPINS = sSPINS / 20;

AutoPtr<IInterface> CExchanger::sCANCEL = new CDummyObject();
AutoPtr<IInterface> CExchanger::sNULL_ITEM = new CDummyObject();

AutoPtr<IInterface> CExchanger::DoExchange(
    /* [in] */ IInterface* item,
    /* [in] */ Boolean timed,
    /* [in] */ Int64 nanos)
{
    AutoPtr<Node> me = new Node(item);                 // Create in case occupying
    Int32 index = HashIndex();                  // Index of current slot
    Int32 fails = 0;                            // Number of CAS failures
    Boolean b = FALSE;

    for (;;) {
        AutoPtr<IInterface> y;                             // Contents of current slot
        AutoPtr<Slot> slot = (*mArena)[index];
        if (slot == NULL)                     // Lazily initialize slots
            CreateSlot(index);                // Continue loop to reread
        else if ((slot->Get((IInterface**)&y), y) != NULL &&  // Try to fulfill
                (slot->CompareAndSet(y, NULL, &b), b)) {
            AutoPtr<Node> you = (Node*)y->Probe(EIID_IAtomicReference);               // Transfer item
            Boolean a = FALSE;
            if ((you->CompareAndSet(NULL, item, &a), a)) {
                LockSupport::Unpark(you->mWaiter);
                return you->mItem;
            }                                 // Else cancelled; continue
        }
        else if (y == NULL &&                 // Try to occupy
                (slot->CompareAndSet(NULL, me, &b), b)) {
            if (index == 0)                   // Blocking wait for slot 0
                return timed ?
                    AwaitNanos(me, slot, nanos) :
                    Await(me, slot);
            AutoPtr<IInterface> v = SpinWait(me, slot);    // Spin wait for non-0
            if (!Object::Equals(v, sCANCEL))
                return v;
            me = new Node(item);              // Throw away cancelled node
            Int32 m;
            mMax->Get(&m);
            if (m > (index >>= 1)) {           // Decrease index
                Boolean c = FALSE;
                mMax->CompareAndSet(m, m - 1, &c);  // Maybe shrink table
            }
        }
        else if (++fails > 1) {               // Allow 2 fails on 1st slot
            Int32 m;
            mMax->Get(&m);
            Boolean d = FALSE;
            if (fails > 3 && m < sFULL && mMax->CompareAndSet(m, m + 1, &d))
                index = m + 1;                // Grow on 3rd failed slot
            else if (--index < 0)
                index = m;                    // Circularly traverse
        }
    }
}

Int32 CExchanger::HashIndex()
{
    Int64 id = 0;
    Thread::GetCurrentThread()->GetId(&id);
    Int32 hash = (((Int32)(id ^ (id >> 32))) ^ 0x811c9dc5) * 0x01000193;

    Int32 m;
    mMax->Get(&m);
    Int32 nbits = (((0xfffffc00  >> m) & 4) | // Compute ceil(log2(m+1))
                 ((0x000001f8 >> m) & 2) | // The constants hold
                 ((0xffff00f2 >> m) & 1)); // a lookup table
    Int32 index;
    while ((index = hash & ((1 << nbits) - 1)) > m)       // May retry on
        hash = (hash >> nbits) | (hash << (33 - nbits)); // non-power-2 m
    return index;
}

void CExchanger::CreateSlot(
    /* [in] */ Int32 index)
{
    // Create slot outside of lock to narrow sync region
    AutoPtr<Slot> newSlot = new Slot();
    AutoPtr<ArrayOf<Slot*> > a = mArena;
    {
        Autolock lock(this);
        if ((*a)[index] == NULL)
            a->Set(index, newSlot);
    }
}

Boolean CExchanger::TryCancel(
    /* [in] */ Node* node,
    /* [in] */ Slot* slot)
{
    Boolean b = FALSE;
    if (!(node->CompareAndSet(NULL, sCANCEL, &b), b))
        return FALSE;
    AutoPtr<IInterface> s;
    slot->Get((IInterface**)&s);
    if (Object::Equals(s, node->Probe(EIID_IInterface))) { // pre-check to minimize contention
        Boolean a = FALSE;
        slot->CompareAndSet(node, NULL, &a);
    }
    return TRUE;
}

AutoPtr<IInterface> CExchanger::SpinWait(
    /* [in] */ Node* node,
    /* [in] */ Slot* slot)
{
    Int32 spins = sSPINS;
    for (;;) {
        AutoPtr<IInterface> v;
        node->Get((IInterface**)&v);
        if (v != NULL)
            return v;
        else if (spins > 0)
            --spins;
        else
            TryCancel(node, slot);
    }
}

AutoPtr<IInterface> CExchanger::Await(
    /* [in] */ Node* node,
    /* [in] */ Slot* slot)
{
    AutoPtr<IThread> w = Thread::GetCurrentThread();
    Int32 spins = sSPINS;
    for (;;) {
        AutoPtr<IInterface> v;
        node->Get((IInterface**)&v);
        Boolean b = FALSE;
        if (v != NULL)
            return v;
        else if (spins > 0)                 // Spin-wait phase
            --spins;
        else if (node->mWaiter == NULL)       // Set up to block next
            node->mWaiter = w;
        else if ((w->IsInterrupted(&b), b))         // Abort on interrupt
            TryCancel(node, slot);
        else                                // Block
            LockSupport::Park(node);
    }
}

AutoPtr<IInterface> CExchanger::AwaitNanos(
    /* [in] */ Node* node,
    /* [in] */ Slot* slot,
    /* [in] */ Int64 nanos)
{
    Int32 spins = sTIMED_SPINS;
    Int64 lastTime = 0;
    AutoPtr<IThread> w = NULL;
    for (;;) {
        AutoPtr<IInterface> v;
        node->Get((IInterface**)&v);
        if (v != NULL)
            return v;
        Int64 now;// = System.nanoTime();
        if (w == NULL)
            w = Thread::GetCurrentThread();
        else
            nanos -= now - lastTime;
        lastTime = now;
        Boolean b = FALSE;
        if (nanos > 0) {
            if (spins > 0)
                --spins;
            else if (node->mWaiter == NULL)
                node->mWaiter = w;
            else if ((w->IsInterrupted(&b), b))
                TryCancel(node, slot);
            else
                LockSupport::ParkNanos(node, nanos);
        }
        else if (TryCancel(node, slot) && !(w->IsInterrupted(&b), b))
            return ScanOnTimeout(node);
    }
}

AutoPtr<IInterface> CExchanger::ScanOnTimeout(
    /* [in] */ Node* node)
{
    AutoPtr<IInterface> y;
    for (Int32 j = mArena->GetLength() - 1; j >= 0; --j) {
        AutoPtr<Slot> slot = (*mArena)[j];
        if (slot != NULL) {
            while ((slot->Get((IInterface**)&y), y) != NULL) {
                Boolean a = FALSE;
                if ((slot->CompareAndSet(y, NULL, &a), a)) {
                    AutoPtr<Node> you = (Node*)y->Probe(EIID_IAtomicReference);
                    Boolean b = FALSE;
                    if ((you->CompareAndSet(NULL, node->mItem, &b), b)) {
                        LockSupport::Unpark(you->mWaiter);
                        return you->mItem;
                    }
                }
            }
        }
    }
    return sCANCEL;
}

ECode CExchanger::constructor()
{
    mArena = ArrayOf<Slot*>::Alloc(sCAPACITY);
    CAtomicInteger32::New((IAtomicInteger32**)&mMax);
    return NOERROR;
}

ECode CExchanger::Exchange(
    /* [in] */ IInterface* x,
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface);
    if (!Thread::Interrupted()) {
        AutoPtr<IInterface> v = DoExchange((x == NULL) ? sNULL_ITEM.Get() : x, FALSE, 0);
        if (Object::Equals(v, sNULL_ITEM->Probe(EIID_IInterface))) {
            *outface = NULL;
            return NOERROR;
        }
        if (!Object::Equals(v, sCANCEL->Probe(EIID_IInterface))) {
            *outface = v;
            REFCOUNT_ADD(*outface);
            return NOERROR;
        }
        Thread::Interrupted(); // Clear interrupt status on IE throw
    }
    return E_INTERRUPTED_EXCEPTION;
}

ECode CExchanger::Exchange(
    /* [in] */ IInterface* x,
    /* [in] */ Int64 timeout,
    /* [in] */ ITimeUnit* unit,
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface);
    *outface = NULL;

    if (!Thread::Interrupted()) {
        Int64 nanos;
        AutoPtr<IInterface> v = DoExchange((x == NULL) ? sNULL_ITEM.Get() : x,
                              TRUE, (unit->ToNanos(timeout, &nanos), nanos));
        if (Object::Equals(v, sNULL_ITEM->Probe(EIID_IInterface))) {
            return NOERROR;
        }
        if (!Object::Equals(v, sCANCEL->Probe(EIID_IInterface))) {
            *outface = v;
            REFCOUNT_ADD(*outface);
            return NOERROR;
        }
        if (!Thread::Interrupted())
            return E_TIMEOUT_EXCEPTION;
    }
    return E_INTERRUPTED_EXCEPTION;
}

} // namespace Concurrent
} // namespace Utility
} // namespace Elastos
