
#include "cmdef.h"
#include "CConcurrentHashMap.h"
#include "ObjectUtils.h"
#include <elastos/Math.h>

using Elastos::Core::ObjectUtils;
using Elastos::Utility::Concurrent::Locks::EIID_IReentrantLock;
using Elastos::IO::EIID_ISerializable;

namespace Elastos {
namespace Utility {
namespace Concurrent {

//===============================================================================
// CConcurrentHashMap::Segment
//===============================================================================

const Int32 CConcurrentHashMap::Segment::MAX_SCAN_RETRIES = 64; // = Runtime.getRuntime().availableProcessors() > 1 ? 64 : 1;

// TODO
Int64 CConcurrentHashMap::SBASE = 0;
Int32 CConcurrentHashMap::SSHIFT = 0;
Int64 CConcurrentHashMap::TBASE = 0;
Int32 CConcurrentHashMap::TSHIFT = 0;
// static {
//     int ss, ts;
//     try {
//         UNSAFE = sun.misc.Unsafe.getUnsafe();
//         Class<?> tc = HashEntry[].class;
//         Class<?> sc = Segment[].class;
//         TBASE = UNSAFE.arrayBaseOffset(tc);
//         SBASE = UNSAFE.arrayBaseOffset(sc);
//         ts = UNSAFE.arrayIndexScale(tc);
//         ss = UNSAFE.arrayIndexScale(sc);
//     } catch (Exception e) {
//         throw new Error(e);
//     }
//     if ((ss & (ss-1)) != 0 || (ts & (ts-1)) != 0)
//         throw new Error("data type scale not a power of two");
//     SSHIFT = 31 - Integer.numberOfLeadingZeros(ss);
//     TSHIFT = 31 - Integer.numberOfLeadingZeros(ts);
// }
CConcurrentHashMap::Segment::Segment(
    /* [in] */ Float lf,
    /* [in] */ Int32 threshold,
    /* [in] */ ArrayOf<HashEntry*>* tab)
    : mCount(0)
    , mModCount(0)
    , mThreshold(0)
    , mLoadFactor(0)
{
    mLoadFactor = lf;
    mThreshold = threshold;
    mTable = tab;
}

CAR_INTERFACE_IMPL(CConcurrentHashMap::Segment, IReentrantLock);

AutoPtr<IInterface> CConcurrentHashMap::Segment::Put(
    /* [in] */ IInterface* key,
    /* [in] */ Int32 hash,
    /* [in] */ IInterface* value,
    /* [in] */ Boolean onlyIfAbsent)
{
    Boolean isflag = FALSE;
    TryLock(&isflag);
    AutoPtr<HashEntry> node = isflag ? NULL : ScanAndLockForPut(key, hash, value);
    AutoPtr<IInterface> oldValue;
    // try {
    AutoPtr< ArrayOf<HashEntry*> > tab = mTable;
    Int32 index = (tab->GetLength() - 1) & hash;
    AutoPtr<HashEntry> first = EntryAt(tab, index);
    for (AutoPtr<HashEntry> e = first;;) {
        if (e != NULL) {
            AutoPtr<IInterface> k;
            if ((k = e->mKey) == key ||
                (e->mHash == hash && ObjectUtils::Equals(key, k))) {
                oldValue = e->mValue;
                if (!onlyIfAbsent) {
                    e->mValue = value;
                    ++mModCount;
                }
                break;
            }
            e = e->mNext;
        }
        else {
            if (node != NULL) {
                node->SetNext(first);
            }
            else {
                node = new HashEntry(hash, key, value, first);
            }
            Int32 c = mCount + 1;
            if (c > mThreshold && tab->GetLength() < MAXIMUM_CAPACITY) {
                Rehash(node);
            }
            else {
                SetEntryAt(tab, index, node);
            }
            ++mModCount;
            mCount = c;
            oldValue = NULL;
            break;
        }
    }
    // } finally {
    UnLock();
    // }
    return oldValue;
}

void CConcurrentHashMap::Segment::Rehash(
    /* [in] */ HashEntry* node)
{
    /*
     * Reclassify nodes in each list to new table.  Because we
     * are using power-of-two expansion, the elements from
     * each bin must either stay at same index, or move with a
     * power of two offset. We eliminate unnecessary node
     * creation by catching cases where old nodes can be
     * reused because their next fields won't change.
     * Statistically, at the default mThreshold, only about
     * one-sixth of them need cloning when a table
     * doubles. The nodes they replace will be garbage
     * collectable as soon as they are no longer referenced by
     * any reader thread that may be in the midst of
     * concurrently traversing table. Entry accesses use plain
     * array indexing because they are followed by volatile
     * table write.
     */
    AutoPtr< ArrayOf<HashEntry*> > oldTable = mTable;
    Int32 oldCapacity = oldTable->GetLength();
    Int32 newCapacity = oldCapacity << 1;
    mThreshold = (Int32)(newCapacity * mLoadFactor);
    AutoPtr< ArrayOf<HashEntry*> > newTable = ArrayOf<HashEntry*>::Alloc(newCapacity);
    Int32 sizeMask = newCapacity - 1;
    for (Int32 i = 0; i < oldCapacity ; i++) {
        AutoPtr<HashEntry> e = (*oldTable)[i];
        if (e != NULL) {
            AutoPtr<HashEntry> next = e->mNext;
            Int32 idx = e->mHash & sizeMask;
            if (next == NULL) {  //  Single node on list
                newTable->Set(idx, e);
            }
            else { // Reuse consecutive sequence at same slot
                AutoPtr<HashEntry> lastRun = e;
                Int32 lastIdx = idx;
                for (AutoPtr<HashEntry> last = next;
                     last != NULL;
                     last = last->mNext) {
                    Int32 k = last->mHash & sizeMask;
                    if (k != lastIdx) {
                        lastIdx = k;
                        lastRun = last;
                    }
                }
                newTable->Set(lastIdx, lastRun);
                // Clone remaining nodes
                for (AutoPtr<HashEntry> p = e; p != lastRun; p = p->mNext) {
                    AutoPtr<IInterface> v = p->mValue;
                    Int32 h = p->mHash;
                    Int32 k = h & sizeMask;
                    AutoPtr<HashEntry> n = (*newTable)[k];
                    newTable->Set(k, new HashEntry(h, p->mKey, v, n));
                }
            }
        }
    }
    Int32 nodeIndex = node->mHash & sizeMask; // add the new node
    node->SetNext((*newTable)[nodeIndex]);
    newTable->Set(nodeIndex, node);
    mTable = newTable;
}

AutoPtr<CConcurrentHashMap::HashEntry> CConcurrentHashMap::Segment::ScanAndLockForPut(
    /* [in] */ IInterface* key,
    /* [in] */ Int32 hash,
    /* [in] */ IInterface* value)
{
    AutoPtr<HashEntry> first = EntryForHash(this, hash);
    AutoPtr<HashEntry> e = first;
    AutoPtr<HashEntry> node = NULL;
    Int32 retries = -1; // negative while locating node
    Boolean isflag = FALSE;
    while (!(TryLock(&isflag), isflag)) {
        AutoPtr<HashEntry> f; // to recheck first below
        if (retries < 0) {
            if (e == NULL) {
                if (node == NULL) // speculatively create node
                    node = new HashEntry(hash, key, value, NULL);
                retries = 0;
            }
            else if (ObjectUtils::Equals(key, e->mKey))
                retries = 0;
            else
                e = e->mNext;
        }
        else if (++retries > MAX_SCAN_RETRIES) {
            Lock();
            break;
        }
        else if ((retries & 1) == 0 &&
                 (f = EntryForHash(this, hash)) != first) {
            e = first = f; // re-traverse if entry changed
            retries = -1;
        }
    }
    return node;
}

void CConcurrentHashMap::Segment::ScanAndLock(
    /* [in] */ IInterface* key,
    /* [in] */ Int32 hash)
{
    // similar to but simpler than scanAndLockForPut
    AutoPtr<HashEntry> first = EntryForHash(this, hash);
    AutoPtr<HashEntry> e = first;
    Int32 retries = -1;
    Boolean isflag = FALSE;
    while (!(TryLock(&isflag), isflag)) {
        AutoPtr<HashEntry> f;
        if (retries < 0) {
            if (e == NULL || ObjectUtils::Equals(key, e->mKey))
                retries = 0;
            else
                e = e->mNext;
        }
        else if (++retries > MAX_SCAN_RETRIES) {
            Lock();
            break;
        }
        else if ((retries & 1) == 0 &&
                 (f = EntryForHash(this, hash)) != first) {
            e = first = f;
            retries = -1;
        }
    }
}

AutoPtr<IInterface> CConcurrentHashMap::Segment::Remove(
    /* [in] */ IInterface* key,
    /* [in] */ Int32 hash,
    /* [in] */ IInterface* value)
{
    Boolean isflag = FALSE;
    TryLock(&isflag);
    if (!isflag) {
        ScanAndLock(key, hash);
    }
    AutoPtr<IInterface> oldValue = NULL;
    // try {
    AutoPtr< ArrayOf<HashEntry*> > tab = mTable;
    Int32 index = (tab->GetLength() - 1) & hash;
    AutoPtr<HashEntry> e = EntryAt(tab, index);
    AutoPtr<HashEntry> pred = NULL;
    while (e != NULL) {
        AutoPtr<IInterface> k;
        AutoPtr<HashEntry> next = e->mNext;
        if ((k = e->mKey) == key ||
            (e->mHash == hash && ObjectUtils::Equals(key, k))) {
            AutoPtr<IInterface> v = e->mValue;
            if (value == NULL || value == v || ObjectUtils::Equals(value, v)) {
                if (pred == NULL) {
                    SetEntryAt(tab, index, next);
                }
                else {
                    pred->SetNext(next);
                }
                ++mModCount;
                --mCount;
                oldValue = v;
            }
            break;
        }
        pred = e;
        e = next;
    }
    // } finally {
    UnLock();
    // }
    return oldValue;
}

Boolean CConcurrentHashMap::Segment::Replace(
    /* [in] */ IInterface* key,
    /* [in] */ Int32 hash,
    /* [in] */ IInterface* oldValue,
    /* [in] */ IInterface* newValue)
{
    Boolean isflag = FALSE;
    TryLock(&isflag);
    if (!isflag) {
        ScanAndLock(key, hash);
    }
    Boolean replaced = FALSE;
    // try {
    AutoPtr<HashEntry> e;
    for (e = EntryForHash(this, hash); e != NULL; e = e->mNext) {
        AutoPtr<IInterface> k;
        if ((k = e->mKey) == key ||
            (e->mHash == hash && ObjectUtils::Equals(key, k))) {
            if (ObjectUtils::Equals(oldValue, e->mValue)) {
                e->mValue = newValue;
                ++mModCount;
                replaced = true;
            }
            break;
        }
    }
    // } finally {
    UnLock();
    // }
    return replaced;
}

AutoPtr<IInterface> CConcurrentHashMap::Segment::Replace(
    /* [in] */ IInterface* key,
    /* [in] */ Int32 hash,
    /* [in] */ IInterface* value)
{
    Boolean isflag = FALSE;
    TryLock(&isflag);
    if (!isflag) {
        ScanAndLock(key, hash);
    }
    AutoPtr<IInterface> oldValue = NULL;
    // try {
    AutoPtr<HashEntry> e;
    for (e = EntryForHash(this, hash); e != NULL; e = e->mNext) {
        AutoPtr<IInterface> k;
        if ((k = e->mKey) == key ||
            (e->mHash == hash && ObjectUtils::Equals(key, k))) {
            oldValue = e->mValue;
            e->mValue = value;
            ++mModCount;
            break;
        }
    }
    // } finally {
    UnLock();
    // }
    return oldValue;
}

void CConcurrentHashMap::Segment::Clear()
{
    Lock();
    // try {
    AutoPtr< ArrayOf<HashEntry*> > tab = mTable;
    for (Int32 i = 0; i < tab->GetLength() ; i++) {
        SetEntryAt(tab, i, NULL);
    }
    ++mModCount;
    mCount = 0;
    // } finally {
    UnLock();
    // }
}

ECode CConcurrentHashMap::Segment::Lock()
{
    return ReentrantLock::Lock();
}

ECode CConcurrentHashMap::Segment::LockInterruptibly()
{
    return ReentrantLock::LockInterruptibly();
}

ECode CConcurrentHashMap::Segment::TryLock(
    /* [out] */ Boolean* result)
{
    return ReentrantLock::TryLock(result);
}

ECode CConcurrentHashMap::Segment::TryLockEx(
    /* [in] */ Int64 timeout,
    /* [in] */ ITimeUnit* unit,
    /* [out] */ Boolean* result)
{
    return ReentrantLock::TryLockEx(timeout, unit, result);
}

ECode CConcurrentHashMap::Segment::UnLock()
{
    return ReentrantLock::UnLock();
}

ECode CConcurrentHashMap::Segment::NewCondition(
    /* [out] */ ICondition** condition)
{
    return ReentrantLock::NewCondition(condition);
}

ECode CConcurrentHashMap::Segment::GetHoldCount(
    /* [out] */ Int32* count)
{
    return ReentrantLock::GetHoldCount(count);
}

ECode CConcurrentHashMap::Segment::IsLocked(
    /* [out] */ Boolean* result)
{
    return ReentrantLock::IsLocked(result);
}

ECode CConcurrentHashMap::Segment::IsFair(
    /* [out] */ Boolean* result)
{
    return ReentrantLock::IsFair(result);
}

ECode CConcurrentHashMap::Segment::HasQueuedThreads(
    /* [out] */ Boolean* result)
{
    return ReentrantLock::HasQueuedThreads(result);
}

ECode CConcurrentHashMap::Segment::HasQueuedThread(
    /* [in] */ IThread* thread,
    /* [out] */ Boolean* result)
{
    return ReentrantLock::HasQueuedThread(thread, result);
}

ECode CConcurrentHashMap::Segment::GetQueueLength(
    /* [out] */ Int32* result)
{
    return ReentrantLock::GetQueueLength(result);
}

ECode CConcurrentHashMap::Segment::HasWaiters(
    /* [in] */ ICondition* condition,
    /* [out] */ Boolean* result)
{
    return ReentrantLock::HasWaiters(condition, result);
}

ECode CConcurrentHashMap::Segment::GetWaitQueueLength(
    /* [in] */ ICondition* condition,
    /* [out] */ Int32* result)
{
    return ReentrantLock::GetWaitQueueLength(condition, result);
}

ECode CConcurrentHashMap::Segment::ToString(
    /* [out] */ String* info)
{
    return ReentrantLock::ToString(info);
}

ECode CConcurrentHashMap::Segment::IsHeldByCurrentThread(
    /* [out] */ Boolean* value)
{
    return ReentrantLock::IsHeldByCurrentThread(value);
}

//===============================================================================
// CConcurrentHashMap::HashIterator
//===============================================================================

CConcurrentHashMap::HashIterator::HashIterator(
    /* [in] */ CConcurrentHashMap* host)
{
    mHost = host;
    mNextSegmentIndex = mHost->mSegments->GetLength() - 1;
    mNextTableIndex = -1;
    Advance();
}

void CConcurrentHashMap::HashIterator::Advance()
{
    for (;;) {
        if (mNextTableIndex >= 0) {
            if ((mNextEntry = EntryAt(mCurrentTable,
                                     mNextTableIndex--)) != NULL)
                break;
        }
        else if (mNextSegmentIndex >= 0) {
            AutoPtr<Segment> seg = SegmentAt(mHost->mSegments, mNextSegmentIndex--);
            if (seg != NULL && (mCurrentTable = seg->mTable) != NULL)
                mNextTableIndex = mCurrentTable->GetLength() - 1;
        }
        else
            break;
    }
}

AutoPtr<CConcurrentHashMap::HashEntry> CConcurrentHashMap::HashIterator::NextEntry()
{
    AutoPtr<HashEntry> e = mNextEntry;
    if (e == NULL) {
        // throw new NoSuchElementException();
        return NULL;
    }
    mLastReturned = e; // cannot assign until after NULL check
    if ((mNextEntry = e->mNext) == NULL)
        Advance();
    return e;
}

ECode CConcurrentHashMap::HashIterator::HasNext(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)
    *value = mNextEntry != NULL;
    return NOERROR;
}

ECode CConcurrentHashMap::HashIterator::HasMoreElements(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)
    *value = mNextEntry != NULL;
    return NOERROR;
}

ECode CConcurrentHashMap::HashIterator::Remove()
{
    if (mLastReturned == NULL) {
        // throw new IllegalStateException();
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    AutoPtr<IInterface> outface;
    mHost->Remove(mLastReturned->mKey, (IInterface**)&outface);
    mLastReturned = NULL;
    return NOERROR;
}

//===============================================================================
// CConcurrentHashMap::KeyIterator
//===============================================================================

CConcurrentHashMap::KeyIterator::KeyIterator(
    /* [in] */ CConcurrentHashMap* host)
    : HashIterator(host)
{
}

CAR_INTERFACE_IMPL_2(CConcurrentHashMap::KeyIterator, IIterator, IEnumeration);

ECode CConcurrentHashMap::KeyIterator::HasNext(
    /* [out] */ Boolean* result)
{
    return HashIterator::HasNext(result);
}

ECode CConcurrentHashMap::KeyIterator::Remove()
{
    return HashIterator::Remove();
}

ECode CConcurrentHashMap::KeyIterator::HasMoreElements(
    /* [out] */ Boolean * value)
{
    return HashIterator::HasMoreElements(value);
}

ECode CConcurrentHashMap::KeyIterator::Next(
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object)

    AutoPtr<HashEntry> res = NextEntry();
    *object = res->mKey;
    INTERFACE_ADDREF(*object)
    return NOERROR;
}

ECode CConcurrentHashMap::KeyIterator::NextElement(
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object)

    AutoPtr<HashEntry> res = NextEntry();
    *object = res->mKey;
    INTERFACE_ADDREF(*object)
    return NOERROR;
}

//===============================================================================
// CConcurrentHashMap::ValueIterator
//===============================================================================

CConcurrentHashMap::ValueIterator::ValueIterator(
    /* [in] */ CConcurrentHashMap* host)
    : HashIterator(host)
{
}

CAR_INTERFACE_IMPL_2(CConcurrentHashMap::ValueIterator, IIterator, IEnumeration);

ECode CConcurrentHashMap::ValueIterator::HasNext(
    /* [out] */ Boolean* result)
{
    return HashIterator::HasNext(result);
}

ECode CConcurrentHashMap::ValueIterator::Remove()
{
    return HashIterator::Remove();
}

ECode CConcurrentHashMap::ValueIterator::HasMoreElements(
    /* [out] */ Boolean * value)
{
    return HashIterator::HasMoreElements(value);
}

ECode CConcurrentHashMap::ValueIterator::Next(
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object)

    AutoPtr<HashEntry> res = NextEntry();
    *object = res->mValue;
    INTERFACE_ADDREF(*object)
    return NOERROR;
}

ECode CConcurrentHashMap::ValueIterator::NextElement(
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object)

    AutoPtr<HashEntry> res = NextEntry();
    *object = res->mValue;
    INTERFACE_ADDREF(*object)
    return NOERROR;
}

//===============================================================================
// CConcurrentHashMap::WriteThroughEntry
//===============================================================================

CConcurrentHashMap::WriteThroughEntry::WriteThroughEntry(
    /* [in] */ IInterface* k,
    /* [in] */ IInterface* v,
    /* [in] */ CConcurrentHashMap* host)
    : SimpleEntry(k, v)
    , mHost(host)
{
}

ECode CConcurrentHashMap::WriteThroughEntry::SetValue(
    /* [in] */ IInterface* value,
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface)

    if (value == NULL) {
        // throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }
    AutoPtr<IInterface> v;
    SimpleEntry::SetValue(value, (IInterface**)&v);
    AutoPtr<IInterface> key;
    GetKey((IInterface**)&key);
    AutoPtr<IInterface> outface2;
    mHost->Put(key, value, (IInterface**)&outface2);
    *outface = v;
    INTERFACE_ADDREF(*outface);
    return NOERROR;
}

//===============================================================================
// CConcurrentHashMap::EntryIterator
//===============================================================================

CConcurrentHashMap::EntryIterator::EntryIterator(
    /* [in] */ CConcurrentHashMap* host)
    : HashIterator(host)
{
}

CAR_INTERFACE_IMPL(CConcurrentHashMap::EntryIterator, IIterator);

ECode CConcurrentHashMap::EntryIterator::HasNext(
    /* [out] */ Boolean* result)
{
    return HashIterator::HasNext(result);
}

ECode CConcurrentHashMap::EntryIterator::Remove()
{
    return HashIterator::Remove();
}

ECode CConcurrentHashMap::EntryIterator::Next(
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object)

    AutoPtr<HashEntry> e = NextEntry();
    AutoPtr<WriteThroughEntry> res = new WriteThroughEntry(e->mKey, e->mValue, mHost);
    *object = res->Probe(EIID_IInterface);
    INTERFACE_ADDREF(*object)
    return NOERROR;
}

//===============================================================================
// CConcurrentHashMap::_KeySet
//===============================================================================

CConcurrentHashMap::_KeySet::_KeySet(
    /* [in] */ CConcurrentHashMap* host)
{
}

CAR_INTERFACE_IMPL(CConcurrentHashMap::_KeySet, ISet)

ECode CConcurrentHashMap::_KeySet::GetIterator(
    /* [out] */ IIterator** result)
{
    VALIDATE_NOT_NULL(result)

    AutoPtr<IIterator> res = new KeyIterator(mHost);
    *result = res;
    INTERFACE_ADDREF(*result)
    return NOERROR;
}

ECode CConcurrentHashMap::_KeySet::GetSize(
    /* [out] */ Int32* size)
{
    return mHost->GetSize(size);
}

ECode CConcurrentHashMap::_KeySet::IsEmpty(
    /* [out] */ Boolean* result)
{
    return mHost->IsEmpty(result);
}

ECode CConcurrentHashMap::_KeySet::Contains(
    /* [in] */ IInterface* o,
    /* [out] */ Boolean* result)
{
    return mHost->ContainsKey(o, result);
}

ECode CConcurrentHashMap::_KeySet::Remove(
    /* [in] */ IInterface* o,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    AutoPtr<IInterface> outface;
    mHost->Remove(o, (IInterface**)&outface);
    *result = outface != NULL;
    return NOERROR;
}

ECode CConcurrentHashMap::_KeySet::Clear()
{
    return mHost->Clear();
}

ECode CConcurrentHashMap::_KeySet::Add(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::Add(object, modified);
}

ECode CConcurrentHashMap::_KeySet::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::AddAll(collection, modified);
}

ECode CConcurrentHashMap::_KeySet::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return AbstractSet::ContainsAll(collection, result);
}

ECode CConcurrentHashMap::_KeySet::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return AbstractSet::RemoveAll(collection, result);
}

ECode CConcurrentHashMap::_KeySet::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return AbstractSet::RetainAll(collection, result);
}

ECode CConcurrentHashMap::_KeySet::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    return AbstractSet::ToArray(array);
}

ECode CConcurrentHashMap::_KeySet::ToArrayEx(
    /* [in] */ ArrayOf<IInterface*>* contents,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    return AbstractSet::ToArrayEx(contents, outArray);
}

ECode CConcurrentHashMap::_KeySet::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return AbstractSet::Equals(object, result);
}

ECode CConcurrentHashMap::_KeySet::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    return AbstractSet::GetHashCode(hashCode);
}

//===============================================================================
// CConcurrentHashMap::_Values
//===============================================================================

CConcurrentHashMap::_Values::_Values(
    /* [in] */ CConcurrentHashMap* host)
    : mHost(host)
{
}

CAR_INTERFACE_IMPL(CConcurrentHashMap::_Values, ICollection);

ECode CConcurrentHashMap::_Values::GetIterator(
    /* [out] */ IIterator** result)
{
    VALIDATE_NOT_NULL(result)

    AutoPtr<IIterator> res = new ValueIterator(mHost);
    *result = res;
    INTERFACE_ADDREF(*result)
    return NOERROR;
}

ECode CConcurrentHashMap::_Values::GetSize(
    /* [out] */ Int32* size)
{
    return mHost->GetSize(size);
}

ECode CConcurrentHashMap::_Values::IsEmpty(
    /* [out] */ Boolean* result)
{
    return mHost->IsEmpty(result);
}

ECode CConcurrentHashMap::_Values::Contains(
    /* [in] */ IInterface* o,
    /* [out] */ Boolean* result)
{
    return mHost->ContainsValue(o, result);
}

ECode CConcurrentHashMap::_Values::Clear()
{
    return mHost->Clear();
}

ECode CConcurrentHashMap::_Values::Add(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return AbstractCollection::Add(object, modified);
}

ECode CConcurrentHashMap::_Values::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractCollection::AddAll(collection, modified);
}

ECode CConcurrentHashMap::_Values::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return AbstractCollection::ContainsAll(collection, result);
}

ECode CConcurrentHashMap::_Values::Remove(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return AbstractCollection::Remove(object, result);
}

ECode CConcurrentHashMap::_Values::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return AbstractCollection::RemoveAll(collection, result);
}

ECode CConcurrentHashMap::_Values::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return AbstractCollection::RetainAll(collection, result);
}

ECode CConcurrentHashMap::_Values::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    return AbstractCollection::ToArray(array);
}

ECode CConcurrentHashMap::_Values::ToArrayEx(
    /* [in] */ ArrayOf<IInterface*>* contents,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    return AbstractCollection::ToArrayEx(contents, outArray);
}

ECode CConcurrentHashMap::_Values::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    *result = ObjectUtils::Equals(object, THIS_PROBE(IInterface));
    return NOERROR;
}

ECode CConcurrentHashMap::_Values::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode)

    *hashCode = ObjectUtils::GetHashCode(THIS_PROBE(IInterface));
    return NOERROR;
}

//===============================================================================
// CConcurrentHashMap::_EntrySet
//===============================================================================

CConcurrentHashMap::_EntrySet::_EntrySet(
    /* [in] */ CConcurrentHashMap* host)
    : mHost(host)
{
}

CAR_INTERFACE_IMPL(CConcurrentHashMap::_EntrySet, ISet);

ECode CConcurrentHashMap::_EntrySet::GetIterator(
    /* [out] */ IIterator** result)
{
    VALIDATE_NOT_NULL(result)

    AutoPtr<IIterator> res = new EntryIterator(mHost);
    *result = res;
    INTERFACE_ADDREF(*result)
    return NOERROR;
}

ECode CConcurrentHashMap::_EntrySet::Contains(
    /* [in] */ IInterface* o,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    if (!(IMapEntry::Probe(o))) {
        *result = FALSE;
        return NOERROR;
    }
    AutoPtr<IMapEntry> e = IMapEntry::Probe(o);
    AutoPtr<IInterface> keyface;
    e->GetKey((IInterface**)&keyface);
    AutoPtr<IInterface> v;
    mHost->Get(keyface, (IInterface**)&v);
    AutoPtr<IInterface> valueface;
    e->GetValue((IInterface**)&valueface);
    *result = v != NULL && ObjectUtils::Equals(v, valueface);
    return NOERROR;
}

ECode CConcurrentHashMap::_EntrySet::Remove(
    /* [in] */ IInterface* o,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    if (!(IMapEntry::Probe(o))) {
        *result = FALSE;
        return NOERROR;
    }
    AutoPtr<IMapEntry> e = IMapEntry::Probe(o);
    AutoPtr<IInterface> keyface;
    AutoPtr<IInterface> valueface;
    AutoPtr<IInterface> outface;
    e->GetKey((IInterface**)&keyface);
    e->GetValue((IInterface**)&valueface);
    return mHost->RemoveEx(keyface, valueface, result);
}

ECode CConcurrentHashMap::_EntrySet::GetSize(
    /* [out] */ Int32* size)
{
    return mHost->GetSize(size);
}

ECode CConcurrentHashMap::_EntrySet::IsEmpty(
    /* [out] */ Boolean* result)
{
    return mHost->IsEmpty(result);
}

ECode CConcurrentHashMap::_EntrySet::Clear()
{
    return mHost->Clear();
}

ECode CConcurrentHashMap::_EntrySet::Add(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::Add(object, modified);
}

ECode CConcurrentHashMap::_EntrySet::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::AddAll(collection, modified);
}

ECode CConcurrentHashMap::_EntrySet::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return AbstractSet::ContainsAll(collection, result);
}

ECode CConcurrentHashMap::_EntrySet::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return AbstractSet::Equals(object, result);
}

ECode CConcurrentHashMap::_EntrySet::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    return AbstractSet::GetHashCode(hashCode);
}

ECode CConcurrentHashMap::_EntrySet::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::RemoveAll(collection, modified);
}

ECode CConcurrentHashMap::_EntrySet::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::RetainAll(collection, modified);
}

ECode CConcurrentHashMap::_EntrySet::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    return AbstractSet::ToArray(array);
}

ECode CConcurrentHashMap::_EntrySet::ToArrayEx(
    /* [in] */ ArrayOf<IInterface*>* inArray,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    return AbstractSet::ToArrayEx(inArray, outArray);
}

//===============================================================================
// CConcurrentHashMap
//===============================================================================

ECode CConcurrentHashMap::constructor(
    /* [in] */ Int32 initialCapacity,
    /* [in] */ Float mLoadFactor,
    /* [in] */ Int32 concurrencyLevel)
{
    if (!(mLoadFactor > 0) || initialCapacity < 0 || concurrencyLevel <= 0) {
        // throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (concurrencyLevel > MAX_SEGMENTS)
        concurrencyLevel = MAX_SEGMENTS;
    // Find power-of-two sizes best matching arguments
    Int32 sshift = 0;
    Int32 ssize = 1;
    while (ssize < concurrencyLevel) {
        ++sshift;
        ssize <<= 1;
    }
    mSegmentShift = 32 - sshift;
    mSegmentMask = ssize - 1;
    if (initialCapacity > MAXIMUM_CAPACITY)
        initialCapacity = MAXIMUM_CAPACITY;
    Int32 c = initialCapacity / ssize;
    if (c * ssize < initialCapacity)
        ++c;
    Int32 cap = MIN_SEGMENT_TABLE_CAPACITY;
    while (cap < c)
        cap <<= 1;
    // create mSegments and mSegments[0]
    AutoPtr< ArrayOf<HashEntry*> > outentry = ArrayOf<HashEntry*>::Alloc(cap);
    AutoPtr<Segment> s0 =
        new Segment(mLoadFactor, (Int32)(cap * mLoadFactor), outentry);
    AutoPtr< ArrayOf< AutoPtr<Segment> > > ss = ArrayOf< AutoPtr<Segment> >::Alloc(ssize);
    assert(0 && "TODO");
    // UNSAFE.putOrderedObject(ss, SBASE, s0); // ordered write of mSegments[0]
    mSegments = ss;
    return NOERROR;
}

ECode CConcurrentHashMap::constructor(
    /* [in] */ Int32 initialCapacity,
    /* [in] */ Float mLoadFactor)
{
    return this->constructor(initialCapacity, mLoadFactor, DEFAULT_CONCURRENCY_LEVEL);
}

ECode CConcurrentHashMap::constructor(
    /* [in] */ Int32 initialCapacity)
{
    return this->constructor(initialCapacity, DEFAULT_LOAD_FACTOR, DEFAULT_CONCURRENCY_LEVEL);
}

ECode CConcurrentHashMap::constructor()
{
    return this->constructor(DEFAULT_INITIAL_CAPACITY, DEFAULT_LOAD_FACTOR, DEFAULT_CONCURRENCY_LEVEL);
}

ECode CConcurrentHashMap::constructor(
    /* [in] */ IMap* m)
{
    Int32 mapsize = 0;
    m->GetSize(&mapsize);
    FAIL_RETURN(this->constructor(Elastos::Core::Math::Max((Int32) (mapsize / DEFAULT_LOAD_FACTOR) + 1,
                  DEFAULT_INITIAL_CAPACITY),
         DEFAULT_LOAD_FACTOR, DEFAULT_CONCURRENCY_LEVEL));
    return PutAll(m);
}

PInterface CConcurrentHashMap::Probe(
    /* [in] */ REIID riid)
{
    return _CConcurrentHashMap::Probe(riid);
}

ECode CConcurrentHashMap::PutIfAbsent(
    /* [in] */ IInterface* key,
    /* [in] */ IInterface* value,
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface)

    AutoPtr<Segment> s;
    if (value == NULL) {
        // throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }
    Int32 hash = Hash(ObjectUtils::GetHashCode(key));
    Int32 j = (hash >> mSegmentShift) & mSegmentMask;
    assert(0 && "TODO");
    // if ((s = (Segment<K,V>)UNSAFE.getObject
    //      (mSegments, (j << SSHIFT) + SBASE)) == NULL)
    //     s = ensureSegment(j);
    *outface = s->Put(key, hash, value, TRUE);
    INTERFACE_ADDREF(*outface)
    return NOERROR;
}

ECode CConcurrentHashMap::RemoveEx(
    /* [in] */ IInterface* key,
    /* [in] */ IInterface* value,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    Int32 hash = Hash(ObjectUtils::GetHashCode(key));
    AutoPtr<Segment> s;
    *result = value != NULL && (s = SegmentForHash(hash)) != NULL &&
        s->Remove(key, hash, value) != NULL;
    return NOERROR;
}

ECode CConcurrentHashMap::Replace(
    /* [in] */ IInterface* key,
    /* [in] */ IInterface* oldValue,
    /* [in] */ IInterface* newValue,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    Int32 hash = Hash(ObjectUtils::GetHashCode(key));
    if (oldValue == NULL || newValue == NULL) {
        // throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }
    AutoPtr<Segment> s = SegmentForHash(hash);
    *result = s != NULL && s->Replace(key, hash, oldValue, newValue);
    return NOERROR;
}

ECode CConcurrentHashMap::ReplaceEx(
    /* [in] */ IInterface* key,
    /* [in] */ IInterface* value,
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result)

    Int32 hash = Hash(ObjectUtils::GetHashCode(key));
    if (value == NULL) {
        // throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }
    AutoPtr<Segment> s = SegmentForHash(hash);
    *result = s == NULL ? NULL : s->Replace(key, hash, value);
    INTERFACE_ADDREF(*result)
    return NOERROR;
}

ECode CConcurrentHashMap::Clear()
{
    AutoPtr< ArrayOf< AutoPtr<Segment> > > segments = mSegments;
    for (Int32 j = 0; j < segments->GetLength(); ++j) {
        AutoPtr<Segment> s = SegmentAt(segments, j);
        if (s != NULL)
            s->Clear();
    }
    return NOERROR;
}

ECode CConcurrentHashMap::ContainsKey(
    /* [in] */ IInterface* key,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    AutoPtr<Segment> s; // same as get() except no need for volatile value read
    AutoPtr< ArrayOf<HashEntry*> > tab;
    Int32 h = Hash(ObjectUtils::GetHashCode(key));
    Int64 u = (((h >> mSegmentShift) & mSegmentMask) << SSHIFT) + SBASE;
    assert(0 && "TODO");
    // if ((s = (Segment<K,V>)UNSAFE.getObjectVolatile(mSegments, u)) != NULL &&
    //     (tab = s.table) != NULL) {
    //     for (AutoPtr<HashEntry> e = (HashEntry<K,V>) UNSAFE.getObjectVolatile
    //              (tab, ((Int64)(((tab.length - 1) & h)) << TSHIFT) + TBASE);
    //          e != NULL; e = e->mNext) {
    //         K k;
    //         if ((k = e.key) == key || (e->mHash == h && key.equals(k)))
    //             return true;
    //     }
    // }
    *result = FALSE;
    return NOERROR;
}

ECode CConcurrentHashMap::ContainsValue(
    /* [in] */ IInterface* value,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    // Same idea as size()
    if (value == NULL) {
        // throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }
    AutoPtr< ArrayOf< AutoPtr<Segment> > > segments = mSegments;
    Int64 previousSum = 0L;
    Int32 lockCount = 0;
    // try {
    for (Int32 retries = -1; ; retries++) {
        Int64 sum = 0L;    // sum of modCounts
        for (Int32 j = 0; j < segments->GetLength(); j++) {
            AutoPtr<Segment> segment;
            if (retries == RETRIES_BEFORE_LOCK) {
                segment = EnsureSegment(j);
                segment->Lock();
                lockCount++;
            }
            else {
                segment = SegmentAt(segments, j);
                if (segment == NULL)
                    continue;
            }
            AutoPtr< ArrayOf<HashEntry*> > tab = segment->mTable;
            if (tab != NULL) {
                for (Int32 i = 0 ; i < tab->GetLength(); i++) {
                    AutoPtr<HashEntry> e;
                    for (e = EntryAt(tab, i); e != NULL; e = e->mNext) {
                        AutoPtr<IInterface> v = e->mValue;
                        if (v != NULL && ObjectUtils::Equals(value, v))
                            *result = TRUE;
                            return NOERROR;
                    }
                }
                sum += segment->mModCount;
            }
        }
        if ((retries >= 0 && sum == previousSum) || lockCount > 0) {
            *result = FALSE;
            return NOERROR;
        }
        previousSum = sum;
    }
    // } finally {
    for (Int32 j = 0; j < lockCount; j++)
        (*segments)[j]->UnLock();
    // }
    return NOERROR;
}

ECode CConcurrentHashMap::Contains(
    /* [in] */ IInterface* value,
    /* [out] */ Boolean* result)
{
    return ContainsValue(value, result);
}

ECode CConcurrentHashMap::EntrySet(
    /* [out] */ ISet** entries)
{
    VALIDATE_NOT_NULL(entries)

    AutoPtr<ISet> es = mEntrySet;
    *entries = (es != NULL) ? es : (mEntrySet = (ISet*) new _EntrySet(this));
    INTERFACE_ADDREF(*entries)
    return NOERROR;
}

ECode CConcurrentHashMap::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return AbstractMap::Equals(object, result);
}

ECode CConcurrentHashMap::Get(
    /* [in] */ IInterface* key,
    /* [out] */ IInterface** value)
{
    VALIDATE_NOT_NULL(value)

    AutoPtr<Segment> s; // manually integrate access methods to reduce overhead
    AutoPtr< ArrayOf<HashEntry*> > tab;
    Int32 h = Hash(ObjectUtils::GetHashCode(key));
    Int64 u = (((h >> mSegmentShift) & mSegmentMask) << SSHIFT) + SBASE;
    assert(0 && "TODO");
    // if ((s = (Segment<K,V>)UNSAFE.getObjectVolatile(mSegments, u)) != NULL &&
    //     (tab = s.table) != NULL) {
    //     for (AutoPtr<HashEntry> e = (HashEntry<K,V>) UNSAFE.getObjectVolatile
    //              (tab, ((Int64)(((tab.length - 1) & h)) << TSHIFT) + TBASE);
    //          e != NULL; e = e->mNext) {
    //         K k;
    //         if ((k = e.key) == key || (e->mHash == h && key.equals(k)))
    //             return e.value;
    //     }
    // }
    *value = NULL;
    return NOERROR;
}

ECode CConcurrentHashMap::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    return AbstractMap::GetHashCode(hashCode);
}

ECode CConcurrentHashMap::IsEmpty(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    /*
     * Sum per-segment modCounts to avoid mis-reporting when
     * elements are concurrently added and removed in one segment
     * while checking another, in which case the table was never
     * actually empty at any point. (The sum ensures accuracy up
     * through at least 1<<31 per-segment modifications before
     * recheck.)  Methods size() and containsValue() use similar
     * constructions for stability checks.
     */
    Int64 sum = 0L;
    AutoPtr< ArrayOf< AutoPtr<Segment> > > segments = mSegments;
    for (Int32 j = 0; j < segments->GetLength(); ++j) {
        AutoPtr<Segment> seg = SegmentAt(segments, j);
        if (seg != NULL) {
            if (seg->mCount != 0) {
                *result = FALSE;
                return NOERROR;
            }
            sum += seg->mModCount;
        }
    }
    if (sum != 0L) { // recheck unless no modifications
        for (Int32 j = 0; j < segments->GetLength(); ++j) {
            AutoPtr<Segment> seg = SegmentAt(segments, j);
            if (seg != NULL) {
                if (seg->mCount != 0) {
                    *result = FALSE;
                    return NOERROR;
                }
                sum -= seg->mModCount;
            }
        }
        if (sum != 0L) {
            *result = FALSE;
            return NOERROR;
        }
    }
    *result = TRUE;
    return NOERROR;
}

ECode CConcurrentHashMap::KeySet(
    /* [out] */ ISet** keySet)
{
    VALIDATE_NOT_NULL(keySet)

    AutoPtr<ISet> ks = mKeySet;
    *keySet = (ks != NULL) ? ks : (mKeySet = (ISet*) new _KeySet(this));
    INTERFACE_ADDREF(*keySet)
    return NOERROR;
}

ECode CConcurrentHashMap::Put(
    /* [in] */ IInterface* key,
    /* [in] */ IInterface* value,
    /* [out] */ IInterface** oldValue)
{
    if (value == NULL) {
        if (oldValue) {
            *oldValue = NULL;
        }
        // throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }

    Int32 hash = Hash(ObjectUtils::GetHashCode(key));
    Int32 j = (hash >> mSegmentShift) & mSegmentMask;
    AutoPtr<Segment> s;
    assert(0 && "TODO");
    // if ((s = (Segment<K,V>)UNSAFE.getObject          // nonvolatile; recheck
    //      (mSegments, (j << SSHIFT) + SBASE)) == NULL) //  in ensureSegment
    //     s = ensureSegment(j);
    if (oldValue) {
        AutoPtr<IInterface> obj = s->Put(key, hash, value, FALSE);
        *oldValue = obj;
        INTERFACE_ADDREF(*oldValue)
    }
    else {
        s->Put(key, hash, value, FALSE);
    }
    return NOERROR;
}

ECode CConcurrentHashMap::PutAll(
    /* [in] */ IMap* map)
{
    VALIDATE_NOT_NULL(map)

    AutoPtr<ISet> outset;
    map->EntrySet((ISet**)&outset);
    AutoPtr< ArrayOf<IInterface*> > outarr;
    outset->ToArray((ArrayOf<IInterface*>**)&outarr);
    for (Int32 i = 0; i < outarr->GetLength(); i++) {
        AutoPtr<IMapEntry> e = IMapEntry::Probe((*outarr)[i]);
        AutoPtr<IInterface> keyface;
        AutoPtr<IInterface> valueface;
        e->GetKey((IInterface**)&keyface);
        e->GetValue((IInterface**)&valueface);
        AutoPtr<IInterface> outface;
        Put(keyface, valueface, (IInterface**)&outface);
    }

    return NOERROR;
}

ECode CConcurrentHashMap::Remove(
    /* [in] */ IInterface* key,
    /* [out] */ IInterface** value)
{
    VALIDATE_NOT_NULL(value)

    Int32 hash = Hash(ObjectUtils::GetHashCode(key));
    AutoPtr<Segment> s = SegmentForHash(hash);
    *value = s == NULL ? NULL : s->Remove(key, hash, NULL);
    INTERFACE_ADDREF(*value)
    return NOERROR;
}

ECode CConcurrentHashMap::GetSize(
    /* [out] */ Int32* outsize)
{
    VALIDATE_NOT_NULL(outsize)

    // Try a few times to get accurate mCount. On failure due to
    // continuous async changes in table, resort to locking.
    AutoPtr< ArrayOf< AutoPtr<Segment> > > segments = mSegments;
    Int32 segmentCount = segments->GetLength();

    Int64 previousSum = 0L;
    for (Int32 retries = -1; retries < RETRIES_BEFORE_LOCK; retries++) {
        Int64 sum = 0L;    // sum of modCounts
        Int64 size = 0L;
        for (Int32 i = 0; i < segmentCount; i++) {
            AutoPtr<Segment> segment = SegmentAt(segments, i);
            if (segment != NULL) {
                sum += segment->mModCount;
                size += segment->mCount;
            }
        }
        if (sum == previousSum) {
            *outsize = ((size >> 31) == 0) ? (Int32) size : Elastos::Core::Math::INT32_MAX_VALUE;
            return NOERROR;
        }
        previousSum = sum;
    }

    Int64 size = 0L;
    for (Int32 i = 0; i < segmentCount; i++) {
        AutoPtr<Segment> segment = EnsureSegment(i);
        segment->Lock();
        size += segment->mCount;
    }
    for (Int32 i = 0; i < segmentCount; i++) {
        (*segments)[i]->UnLock();
    }
    *outsize = ((size >> 31) == 0) ? (Int32) size : Elastos::Core::Math::INT32_MAX_VALUE;
    return NOERROR;
}

ECode CConcurrentHashMap::Values(
    /* [out] */ ICollection** value)
{
    VALIDATE_NOT_NULL(value)

    AutoPtr<ICollection> vs = mValues;
    *value = (vs != NULL) ? vs : (mValues = (ICollection*) new _Values(this));
    INTERFACE_ADDREF(*value)
    return NOERROR;
}

ECode CConcurrentHashMap::Keys(
    /* [out] */ IEnumeration** outenum)
{
    VALIDATE_NOT_NULL(outenum)

    AutoPtr<KeyIterator> res = new KeyIterator(this);
    *outenum = (IEnumeration*) res->Probe(EIID_IEnumeration);
    INTERFACE_ADDREF(*outenum)
    return NOERROR;
}

ECode CConcurrentHashMap::Elements(
    /* [out] */ IEnumeration** outenum)
{
    VALIDATE_NOT_NULL(outenum)

    AutoPtr<ValueIterator> res = new ValueIterator(this);
    *outenum = (IEnumeration*) res->Probe(EIID_IEnumeration);
    INTERFACE_ADDREF(*outenum)
    return NOERROR;
}

AutoPtr<CConcurrentHashMap::HashEntry> CConcurrentHashMap::EntryAt(
    /* [in] */ ArrayOf<HashEntry*>* tab,
    /* [in] */ Int32 i)
{
    assert(0 && "TODO");
    // return (tab == NULL) ? NULL :
    //     (HashEntry<K,V>) UNSAFE.getObjectVolatile
    //     (tab, ((Int64)i << TSHIFT) + TBASE);
    return NULL;
}

void CConcurrentHashMap::SetEntryAt(
    /* [in] */ ArrayOf<HashEntry*>* tab,
    /* [in] */ Int32 i,
    /* [in] */ HashEntry* e)
{
    assert(0 && "TODO");
    // UNSAFE.putOrderedObject(tab, ((Int64)i << TSHIFT) + TBASE, e);
}

AutoPtr<CConcurrentHashMap::Segment> CConcurrentHashMap::SegmentAt(
    /* [in] */ ArrayOf< AutoPtr<Segment> >* ss,
    /* [in] */ Int32 j)
{
    Int64 u = (j << SSHIFT) + SBASE;
    assert(0 && "TODO");
    // return ss == NULL ? NULL :
    //     (Segment<K,V>) UNSAFE.getObjectVolatile(ss, u);
    return NULL;
}

AutoPtr<CConcurrentHashMap::HashEntry> CConcurrentHashMap::EntryForHash(
    /* [in] */ Segment* seg,
    /* [in] */ Int32 h)
{
    AutoPtr< ArrayOf<HashEntry*> > tab;
    assert(0 && "TODO");
    // return (seg == NULL || (tab = seg.table) == NULL) ? NULL :
    //     (HashEntry<K,V>) UNSAFE.getObjectVolatile
    //     (tab, ((Int64)(((tab.length - 1) & h)) << TSHIFT) + TBASE);
    return NULL;
}

AutoPtr<CConcurrentHashMap::Segment> CConcurrentHashMap::EnsureSegment(
    /* [in] */ Int32 k)
{
    AutoPtr< ArrayOf< AutoPtr<Segment> > > ss = mSegments;
    Int64 u = (k << SSHIFT) + SBASE; // raw offset
    AutoPtr<Segment> seg;
    assert(0 && "TODO");
    // if ((seg = (Segment<K,V>)UNSAFE.getObjectVolatile(ss, u)) == NULL) {
    //     Segment<K,V> proto = ss[0]; // use segment 0 as prototype
    //     Int32 cap = proto.table.length;
    //     float lf = proto.mLoadFactor;
    //     Int32 mThreshold = (Int32)(cap * lf);
    //     AutoPtr< ArrayOf<HashEntry*> > tab = (AutoPtr< ArrayOf<HashEntry*> >)new HashEntry<?,?>[cap];
    //     if ((seg = (Segment<K,V>)UNSAFE.getObjectVolatile(ss, u))
    //         == NULL) { // recheck
    //         Segment<K,V> s = new Segment<K,V>(lf, mThreshold, tab);
    //         while ((seg = (Segment<K,V>)UNSAFE.getObjectVolatile(ss, u))
    //                == NULL) {
    //             if (UNSAFE.compareAndSwapObject(ss, u, NULL, seg = s))
    //                 break;
    //         }
    //     }
    // }
    // return seg;
    return NULL;
}

AutoPtr<CConcurrentHashMap::Segment> CConcurrentHashMap::SegmentForHash(
    /* [in] */ Int32 h)
{
    Int64 u = (((h >> mSegmentShift) & mSegmentMask) << SSHIFT) + SBASE;
    assert(0 && "TODO");
    // return (Segment<K,V>) UNSAFE.getObjectVolatile(mSegments, u);
    return NULL;
}

Int32 CConcurrentHashMap::Hash(
    /* [in] */ Int32 h)
{
    // Spread bits to regularize both segment and index locations,
    // using variant of single-word Wang/Jenkins hash.
    h += (h <<  15) ^ 0xffffcd7d;
    h ^= (h >> 10);
    h += (h <<   3);
    h ^= (h >>  6);
    h += (h <<   2) + (h << 14);
    return h ^ (h >> 16);
}

ECode CConcurrentHashMap::WriteObject(
    /* [in] */ IObjectOutputStream* s)
{
    // force all segments for serialization compatibility
    for (Int32 k = 0; k < mSegments->GetLength(); ++k) {
        EnsureSegment(k);
    }
    s->DefaultWriteObject();

    AutoPtr< ArrayOf< AutoPtr<Segment> > > segments = mSegments;
    for (Int32 k = 0; k < segments->GetLength(); ++k) {
        AutoPtr<Segment> seg = SegmentAt(segments, k);
        seg->Lock();
        // try {
        AutoPtr< ArrayOf<HashEntry*> > tab = seg->mTable;
        for (Int32 i = 0; i < tab->GetLength(); ++i) {
            AutoPtr<HashEntry> e;
            for (e = EntryAt(tab, i); e != NULL; e = e->mNext) {
                assert(0 && "TODO");
                // s.writeObject(e.key);
                // s.writeObject(e.value);
            }
        }
        // } finally {
        seg->UnLock();
        // }
    }
    assert(0 && "TODO");
    // s.writeObject(null);
    // s.writeObject(null);
    return NOERROR;
}

ECode CConcurrentHashMap::ReadObject(
    /* [in] */ IObjectInputStream* s)
{
    s->DefaultReadObject();

    // Re-initialize segments to be minimally sized, and let grow.
    Int32 cap = MIN_SEGMENT_TABLE_CAPACITY;
    AutoPtr< ArrayOf< AutoPtr<Segment> > > segments = mSegments;
    for (Int32 k = 0; k < segments->GetLength(); ++k) {
        AutoPtr<Segment> seg = (*segments)[k];
        if (seg != NULL) {
            seg->mThreshold = (Int32)(cap * seg->mLoadFactor);
            seg->mTable = ArrayOf<HashEntry*>::Alloc(cap);
        }
    }

    // Read the keys and values, and put the mappings in the table
    assert(0 && "TODO");
    // for (;;) {
    //     K key = (K) s.readObject();
    //     V value = (V) s.readObject();
    //     if (key == null)
    //         break;
    //     put(key, value);
    // }
    return NOERROR;
}

} // namespace Concurrent
} // namespace Utility
} // namespace Elastos
