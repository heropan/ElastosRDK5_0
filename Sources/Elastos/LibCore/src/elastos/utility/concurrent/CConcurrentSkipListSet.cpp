
#include "CConcurrentSkipListSet.h"
#include <elastos/ObjectUtils.h>
#include "CBoolean.h"
#include "CConcurrentSkipListMap.h"

using Elastos::Core::ObjectUtils;
using Elastos::Core::IBoolean;
using Elastos::Core::CBoolean;
using Elastos::Utility::Concurrent::CConcurrentSkipListMap;

namespace Elastos {
namespace Utility {
namespace Concurrent {

//====================================================================
// CConcurrentSkipListSet::
//====================================================================

ECode CConcurrentSkipListSet::constructor()
{
    return CConcurrentSkipListMap::New((INavigableMap**)&mM);
}

ECode CConcurrentSkipListSet::constructor(
    /* [in] */ IComparator* comparator)
{
    return CConcurrentSkipListMap::New(comparator, (INavigableMap**)&mM);
}

ECode CConcurrentSkipListSet::constructor(
    /* [in] */ ICollection* c)
{
    CConcurrentSkipListMap::New((INavigableMap**)&mM);
    Boolean b;
    return AddAll(c, &b);
}

ECode CConcurrentSkipListSet::constructor(
    /* [in] */ ISortedSet* s)
{
    AutoPtr<IComparator> comp;
    s->Comparator((IComparator**)&comp);
    CConcurrentSkipListMap::New(comp, (INavigableMap**)&mM);
    Boolean b;
    return AddAll(s, &b);
}

ECode CConcurrentSkipListSet::constructor(
    /* [in] */ INavigableMap* m)
{
    mM = m;
    return NOERROR;
}

PInterface CConcurrentSkipListSet::Probe(
    /* [in] */ REIID riid)
{
    return _CConcurrentSkipListSet::Probe(riid);
}

ECode CConcurrentSkipListSet::Clone(
    /* [out] */ IInterface** res)
{
    VALIDATE_NOT_NULL(res);
    AutoPtr<INavigableSet> clone;
    CConcurrentSkipListSet::New((INavigableSet**)&clone);
    AutoPtr<CConcurrentSkipListSet> p = (CConcurrentSkipListSet*)clone.Get();
    AutoPtr<INavigableMap> map;
    CConcurrentSkipListMap::New(mM, (INavigableMap**)&map);
    p->SetMap(map);
    *res = p->Probe(EIID_IInterface);
    REFCOUNT_ADD(*res);
    return NOERROR;
}

/* ---------------- Set operations -------------- */

ECode CConcurrentSkipListSet::GetSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);
    return mM->GetSize(size);
}

ECode CConcurrentSkipListSet::IsEmpty(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return mM->IsEmpty(result);
}

ECode CConcurrentSkipListSet::Contains(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return mM->ContainsKey(object, result);
}

ECode CConcurrentSkipListSet::Add(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    VALIDATE_NOT_NULL(modified);
    AutoPtr<IBoolean> b;
    CBoolean::New(TRUE, (IBoolean**)&b);
    AutoPtr<IConcurrentMap> map = (IConcurrentMap*)mM->Probe(EIID_IConcurrentMap);
    AutoPtr<IInterface> res;
    map->PutIfAbsent(object, b, (IInterface**)&res);
    *modified = res == NULL;
    return NOERROR;
}

ECode CConcurrentSkipListSet::Remove(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    VALIDATE_NOT_NULL(modified);
    AutoPtr<IBoolean> b;
    CBoolean::New(TRUE, (IBoolean**)&b);
    AutoPtr<IConcurrentMap> map = (IConcurrentMap*)mM->Probe(EIID_IConcurrentMap);
    return map->Remove(object, b, modified);
}

ECode CConcurrentSkipListSet::Clear()
{
    return mM->Clear();
}

ECode CConcurrentSkipListSet::GetIterator(
    /* [out] */ IIterator** it)
{
    VALIDATE_NOT_NULL(it);
    AutoPtr<INavigableSet> s;
    mM->NavigableKeySet((INavigableSet**)&s);
    return s->GetIterator(it);
}

ECode CConcurrentSkipListSet::DescendingIterator(
    /* [out] */ IIterator** outiter)
{
    VALIDATE_NOT_NULL(outiter);
    AutoPtr<INavigableSet> s;
    mM->DescendingKeySet((INavigableSet**)&s);
    return s->GetIterator(outiter);
}

/* ---------------- AbstractSet Overrides -------------- */

ECode CConcurrentSkipListSet::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // Override AbstractSet version to avoid calling size()
    if (ObjectUtils::Equals(object, THIS_PROBE(IInterface))) {
        *result = TRUE;
        return NOERROR;
    }
    if (object->Probe(EIID_ISet) == NULL) {
        *result = FALSE;
        return NOERROR;
    }
    AutoPtr<ICollection> c = (ICollection*) object->Probe(EIID_ICollection);
    Boolean a = FALSE, b = FALSE;
    ContainsAll(c, &a);
    c->ContainsAll(this, &b);
    *result = a && b;
    return NOERROR;
}

ECode CConcurrentSkipListSet::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    // Override AbstractSet version to avoid unnecessary call to size()
    AutoPtr<ArrayOf<IInterface*> > arr;
    collection->ToArray((ArrayOf<IInterface*>**)&arr);
    Boolean modified = FALSE;
    for (Int32 i = 0;i < arr->GetLength();i++) {
        AutoPtr<IInterface> e = (*arr)[i];
        Boolean b = FALSE;
        Remove(e, &b);
        if (b) modified = TRUE;
    }
    *res = modified;
    return NOERROR;
}

/* ---------------- Relational operations -------------- */

ECode CConcurrentSkipListSet::Lower(
    /* [in] */ IInterface* e,
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface);
    return mM->LowerKey(e, outface);
}

ECode CConcurrentSkipListSet::Floor(
    /* [in] */ IInterface* e,
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface);
    return mM->FloorKey(e, outface);
}

ECode CConcurrentSkipListSet::Ceiling(
    /* [in] */ IInterface* e,
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface);
    return mM->CeilingKey(e, outface);
}

ECode CConcurrentSkipListSet::Higher(
    /* [in] */ IInterface* e,
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface);
    return mM->HigherKey(e, outface);
}

ECode CConcurrentSkipListSet::PollFirst(
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface);
    AutoPtr<IMapEntry> e;
    mM->PollFirstEntry((IMapEntry**)&e);
    if (e == NULL) {
        *outface = NULL;
        return NOERROR;
    }
    else {
        return e->GetKey(outface);
    }
}

ECode CConcurrentSkipListSet::PollLast(
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface);
    AutoPtr<IMapEntry> e;
    mM->PollLastEntry((IMapEntry**)&e);
    if (e == NULL) {
        *outface = NULL;
        return NOERROR;
    }
    else {
        return e->GetKey(outface);
    }
}

/* ---------------- SortedSet operations -------------- */

ECode CConcurrentSkipListSet::Comparator(
    /* [out] */ IComparator** outcom)
{
    VALIDATE_NOT_NULL(outcom);
    return mM->Comparator(outcom);
}

ECode CConcurrentSkipListSet::First(
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface);
    return mM->FirstKey(outface);
}

ECode CConcurrentSkipListSet::Last(
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface);
    return mM->LastKey(outface);
}

ECode CConcurrentSkipListSet::SubSet(
    /* [in] */ IInterface* fromElement,
    /* [in] */ Boolean fromInclusive,
    /* [in] */ IInterface* toElement,
    /* [in] */ Boolean toInclusive,
    /* [out] */ INavigableSet** outnav)
{
    VALIDATE_NOT_NULL(outnav);
    AutoPtr<INavigableMap> map;
    mM->SubMap(fromElement, fromInclusive,
                toElement, toInclusive, (INavigableMap**)&map);
    AutoPtr<INavigableSet> p;
    CConcurrentSkipListSet::New(map, (INavigableSet**)&p);
    *outnav = p.Get();
    REFCOUNT_ADD(*outnav);
    return NOERROR;
}

ECode CConcurrentSkipListSet::HeadSet(
    /* [in] */ IInterface* toElement,
    /* [in] */ Boolean inclusive,
    /* [out] */ INavigableSet** outnav)
{
    VALIDATE_NOT_NULL(outnav);
    AutoPtr<INavigableMap> map;
    mM->HeadMap(toElement, inclusive, (INavigableMap**)&map);
    AutoPtr<INavigableSet> p;
    CConcurrentSkipListSet::New(map, (INavigableSet**)&p);
    *outnav = p.Get();
    REFCOUNT_ADD(*outnav);
    return NOERROR;
}

ECode CConcurrentSkipListSet::TailSet(
    /* [in] */ IInterface* fromElement,
    /* [in] */ Boolean inclusive,
    /* [out] */ INavigableSet** outnav)
{
    VALIDATE_NOT_NULL(outnav);
    AutoPtr<INavigableMap> map;
    mM->TailMap(fromElement, inclusive, (INavigableMap**)&map);
    AutoPtr<INavigableSet> p;
    CConcurrentSkipListSet::New(map, (INavigableSet**)&p);
    *outnav = p.Get();
    REFCOUNT_ADD(*outnav);
    return NOERROR;
}

ECode CConcurrentSkipListSet::SubSet(
    /* [in] */ IInterface* start,
    /* [in] */ IInterface* end,
    /* [out] */ ISortedSet** outsort)
{
    VALIDATE_NOT_NULL(outsort);
    AutoPtr<INavigableSet> s;
    SubSet(start, TRUE, end, FALSE, (INavigableSet**)&s);
    AutoPtr<ISortedSet> p = (ISortedSet*)s->Probe(EIID_ISortedSet);
    *outsort = p;
    REFCOUNT_ADD(*outsort);
    return NOERROR;
}

ECode CConcurrentSkipListSet::HeadSet(
    /* [in] */ IInterface* end,
    /* [out] */ ISortedSet** outsort)
{
    VALIDATE_NOT_NULL(outsort);
    AutoPtr<INavigableSet> s;
    HeadSet(end, FALSE, (INavigableSet**)&s);
    AutoPtr<ISortedSet> p = (ISortedSet*)s->Probe(EIID_ISortedSet);
    *outsort = p;
    REFCOUNT_ADD(*outsort);
    return NOERROR;
}

ECode CConcurrentSkipListSet::TailSet(
    /* [in] */ IInterface* start,
    /* [out] */ ISortedSet** outsort)
{
    VALIDATE_NOT_NULL(outsort);
    AutoPtr<INavigableSet> s;
    TailSet(start, TRUE, (INavigableSet**)&s);
    AutoPtr<ISortedSet> p = (ISortedSet*)s->Probe(EIID_ISortedSet);
    *outsort = p;
    REFCOUNT_ADD(*outsort);
    return NOERROR;
}

ECode CConcurrentSkipListSet::DescendingSet(
    /* [out] */ INavigableSet** outnav)
{
    VALIDATE_NOT_NULL(outnav);
    AutoPtr<INavigableMap> map;
    mM->DescendingMap((INavigableMap**)&map);
    AutoPtr<INavigableSet> p;
    CConcurrentSkipListSet::New(map, (INavigableSet**)&p);
    *outnav = p.Get();
    REFCOUNT_ADD(*outnav);
    return NOERROR;
}

ECode CConcurrentSkipListSet::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::AddAll(collection, modified);
}

ECode CConcurrentSkipListSet::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return AbstractSet::ContainsAll(collection, result);
}

ECode CConcurrentSkipListSet::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::RetainAll(collection, modified);
}

ECode CConcurrentSkipListSet::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    return AbstractSet::ToArray(array);
}

ECode CConcurrentSkipListSet::ToArray(
    /* [in] */ ArrayOf<IInterface*>* inArray,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    return AbstractSet::ToArray(inArray, outArray);
}

void CConcurrentSkipListSet::SetMap(
    /* [in] */ INavigableMap* map)
{
    assert(0 && "TODO");
//    UNSAFE.putObjectVolatile(this, mapOffset, map);
}

// private static final sun.misc.Unsafe UNSAFE;
// private static final long mapOffset;
// static {
//     try {
//         UNSAFE = sun.misc.Unsafe.getUnsafe();
//         Class<?> k = ConcurrentSkipListSet.class;
//         mapOffset = UNSAFE.objectFieldOffset
//             (k.getDeclaredField("m"));
//     } catch (Exception e) {
//         throw new Error(e);
//     }
// }

} // namespace Concurrent
} // namespace Utility
} // namespace Elastos
