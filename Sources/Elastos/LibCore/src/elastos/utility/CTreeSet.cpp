
#include "CTreeSet.h"
#include "CTreeMap.h"
#include "CBoolean.h"

using Elastos::Core::IBoolean;
using Elastos::Core::CBoolean;
using Elastos::Core::IComparable;

namespace Elastos {
namespace Utility {

PInterface CTreeSet::Probe(
    /* [in] */ REIID riid)
{
    return _CTreeSet::Probe(riid);
}

ECode CTreeSet::constructor()
{
    FAIL_RETURN(CTreeMap::New((INavigableMap**)&mBackingMap));
    return NOERROR;
}

ECode CTreeSet::constructor(
    /* [in] */ INavigableMap* navmap)
{
    mBackingMap = navmap;
    return NOERROR;
}

ECode CTreeSet::constructor(
    /* [in] */ ICollection* map)
{
    this->constructor();
    Boolean isflag = FALSE;
    return AddAll(map, &isflag);
}

ECode CTreeSet::constructor(
    /* [in] */ IComparator* comparator)
{
    return CTreeMap::New(comparator, (INavigableMap**)&mBackingMap);
}

ECode CTreeSet::constructor(
    /* [in] */ ISortedSet* set)
{
    AutoPtr<IComparator> rescom;
    set->Comparator((IComparator**)&rescom);
    this->constructor(rescom);
    AutoPtr<IIterator> it;
    if (IIterable::Probe(set) == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    IIterable::Probe(set)->GetIterator((IIterator**)&it);
    Boolean isflag = FALSE;
    while (it->HasNext(&isflag), isflag) {
        AutoPtr<IInterface> outface;
        it->Next((IInterface**)&outface);
        Add(outface, &isflag);
    }
    return NOERROR;
}

ECode CTreeSet::Add(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    VALIDATE_NOT_NULL(modified)

    AutoPtr<IBoolean> bltrue;
    CBoolean::New(TRUE, (IBoolean**)&bltrue);
    AutoPtr<IInterface> outface;
    mBackingMap->Put(object, bltrue, (IInterface**)&outface);
    *modified = outface == NULL;
    return NOERROR;
}

ECode CTreeSet::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    VALIDATE_NOT_NULL(modified)

    return AbstractSet::AddAll(collection, modified);
}

ECode CTreeSet::Clear()
{
    return mBackingMap->Clear();
}

ECode CTreeSet::Clone(
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object)

    // try {
    AutoPtr<INavigableSet> outset;
    FAIL_RETURN(CTreeSet::New((INavigableSet**)&outset));
    AutoPtr<CTreeSet> clone = (CTreeSet*)outset.Get();
    if (INavigableMap::Probe(mBackingMap)) {
        AutoPtr<IInterface> outface;
        ICloneable::Probe(mBackingMap)->Clone((IInterface**)&outface);
        clone->mBackingMap = INavigableMap::Probe(outface);
    }
    else {
        AutoPtr<INavigableMap> resmap;
        FAIL_RETURN(CTreeMap::New(mBackingMap, (INavigableMap**)&resmap));
        clone->mBackingMap = resmap;
    }
    *object = outset;
    REFCOUNT_ADD(*object)
    // } catch (CloneNotSupportedException e) {
    //     throw new AssertionError(e);
    // }
    return NOERROR;
}

ECode CTreeSet::Comparator(
    /* [out] */ IComparator** outcom)
{
    return mBackingMap->Comparator(outcom);
}

ECode CTreeSet::Contains(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return mBackingMap->ContainsKey(object, result);
}

ECode CTreeSet::IsEmpty(
    /* [out] */ Boolean* result)
{
    return mBackingMap->IsEmpty(result);
}

ECode CTreeSet::GetIterator(
    /* [out] */ IIterator** it)
{
    VALIDATE_NOT_NULL(it)

    AutoPtr<ISet> outset;
    mBackingMap->KeySet((ISet**)&outset);
    return outset->GetIterator(it);
}

ECode CTreeSet::DescendingIterator(
    /* [out] */ IIterator** outiter)
{
    VALIDATE_NOT_NULL(outiter)

    AutoPtr<INavigableSet> res;
    DescendingSet((INavigableSet**)&res);
    if (IIterable::Probe(res)) {
        *outiter = NULL;
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return IIterable::Probe(res)->GetIterator(outiter);
}

ECode CTreeSet::Remove(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    VALIDATE_NOT_NULL(modified)

    AutoPtr<IInterface> outface;
    mBackingMap->Remove(object, (IInterface**)&outface);
    *modified = outface != NULL;
    return NOERROR;
}

ECode CTreeSet::GetSize(
    /* [out] */ Int32* size)
{
    return mBackingMap->GetSize(size);
}

ECode CTreeSet::First(
    /* [out] */ IInterface** outface)
{
    return mBackingMap->FirstKey(outface);
}

ECode CTreeSet::Last(
    /* [out] */ IInterface** outface)
{
    return mBackingMap->LastKey(outface);
}

ECode CTreeSet::PollFirst(
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface)

    AutoPtr<IMapEntry> entry;
    mBackingMap->PollFirstEntry((IMapEntry**)&entry);
    if (entry == NULL) {
        *outface = NULL;
    }
    else {
        entry->GetKey(outface);
    }
    return NOERROR;
}

ECode CTreeSet::PollLast(
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface)

    AutoPtr<IMapEntry> entry;
    mBackingMap->PollLastEntry((IMapEntry**)&entry);
    if (entry == NULL) {
        *outface = NULL;
    }
    else {
        entry->GetKey(outface);
    }
    return NOERROR;
}

ECode CTreeSet::Higher(
    /* [in] */ IInterface* e,
    /* [out] */ IInterface** outface)
{
    return mBackingMap->HigherKey(e, outface);
}

ECode CTreeSet::Lower(
    /* [in] */ IInterface* e,
    /* [out] */ IInterface** outface)
{
    return mBackingMap->LowerKey(e, outface);
}

ECode CTreeSet::Ceiling(
    /* [in] */ IInterface* e,
    /* [out] */ IInterface** outface)
{
    return mBackingMap->CeilingKey(e, outface);
}

ECode CTreeSet::Floor(
    /* [in] */ IInterface* e,
    /* [out] */ IInterface** outface)
{
    return mBackingMap->FloorKey(e, outface);
}

ECode CTreeSet::DescendingSet(
    /* [out] */ INavigableSet** outnav)
{
    VALIDATE_NOT_NULL(outnav)

    if (mDescendingSet == NULL) {
        AutoPtr<INavigableMap> res;
        mBackingMap->DescendingMap((INavigableMap**)&res);
        CTreeSet::New(res, (INavigableSet**)&mDescendingSet);
    }
    *outnav = mDescendingSet;
    REFCOUNT_ADD(*outnav)
    return NOERROR;
}

ECode CTreeSet::SubSet(
    /* [in] */ IInterface* fromElement,
    /* [in] */ Boolean fromInclusive,
    /* [in] */ IInterface* toElement,
    /* [in] */ Boolean toInclusive,
    /* [out] */ INavigableSet** outnav)
{
    VALIDATE_NOT_NULL(outnav)

    AutoPtr<IComparator> c;
    mBackingMap->Comparator((IComparator**)&c);
    Int32 compare = 0;
    if (c == NULL) {
        if (IComparable::Probe(fromElement) == NULL) {
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
        IComparable::Probe(fromElement)->CompareTo(toElement, &compare);
    }
    else {
        c->Compare(fromElement, toElement, &compare);
    }
    if (compare <= 0) {
        AutoPtr<CTreeSet> outset;
        AutoPtr<INavigableMap> resmap;
        mBackingMap->SubMap(fromElement, fromInclusive, toElement, toInclusive, (INavigableMap**)&resmap);
        FAIL_RETURN(CTreeSet::NewByFriend(resmap, (CTreeSet**)&outset));
        *outnav = INavigableSet::Probe(outset);
        REFCOUNT_ADD(*outnav)
        return NOERROR;
    }
    return E_ILLEGAL_ARGUMENT_EXCEPTION;
}

ECode CTreeSet::HeadSet(
    /* [in] */ IInterface* toElement,
    /* [in] */ Boolean inclusive,
    /* [out] */ INavigableSet** outnav)
{
    VALIDATE_NOT_NULL(outnav)

    // Check for errors
    AutoPtr<IComparator> c;
    mBackingMap->Comparator((IComparator**)&c);
    Int32 compare = 0;
    if (c == NULL) {
        if (IComparable::Probe(toElement) == NULL) {
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
        IComparable::Probe(toElement)->CompareTo(toElement, &compare);
    }
    else {
        c->Compare(toElement, toElement, &compare);
    }
    AutoPtr<CTreeSet> resset;
    AutoPtr<INavigableMap> resmap;
    mBackingMap->HeadMap(toElement, inclusive, (INavigableMap**)&resmap);
    FAIL_RETURN(CTreeSet::NewByFriend(resmap, (CTreeSet**)&resset));
    *outnav = INavigableSet::Probe(resset);
    REFCOUNT_ADD(*outnav)
    return NOERROR;
}

ECode CTreeSet::TailSet(
    /* [in] */ IInterface* fromElement,
    /* [in] */ Boolean inclusive,
    /* [out] */ INavigableSet** outnav)
{
    VALIDATE_NOT_NULL(outnav)

    // Check for errors
    AutoPtr<IComparator> c;
    mBackingMap->Comparator((IComparator**)&c);
    Int32 compare = 0;
    if (c == NULL) {
        if (IComparable::Probe(fromElement) == NULL) {
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
        IComparable::Probe(fromElement)->CompareTo(fromElement, &compare);
    }
    else {
        c->Compare(fromElement, fromElement, &compare);
    }
    AutoPtr<INavigableMap> resmap;
    AutoPtr<CTreeSet> resset;
    mBackingMap->TailMap(fromElement, inclusive, (INavigableMap**)&resmap);
    FAIL_RETURN(CTreeSet::NewByFriend(resmap, (CTreeSet**)&resset));
    *outnav = INavigableSet::Probe(resset);
    REFCOUNT_ADD(*outnav)
    return NOERROR;
}

ECode CTreeSet::SubSet(
    /* [in] */ IInterface* start,
    /* [in] */ IInterface* end,
    /* [out] */ ISortedSet** outsort)
{
    return SubSet(start, TRUE, end, FALSE, (INavigableSet**)outsort);
}

ECode CTreeSet::HeadSet(
    /* [in] */ IInterface* end,
    /* [out] */ ISortedSet** outsort)
{
    return HeadSet(end, FALSE, (INavigableSet**)outsort);
}

ECode CTreeSet::TailSet(
    /* [in] */ IInterface* start,
    /* [out] */ ISortedSet** outsort)
{
    return TailSet(start, TRUE, (INavigableSet**)outsort);
}

void CTreeSet::WriteObject(
    /* [in] */ IObjectOutputStream* stream)
{
    stream->DefaultWriteObject();
    AutoPtr<IComparator> rescom;
    mBackingMap->Comparator((IComparator**)&rescom);
    assert(0 && "TODO");
    // stream->WriteObject(rescom);
    Int32 size = 0;
    mBackingMap->GetSize(&size);
    stream->Write(size);
    if (size > 0) {
        AutoPtr<ISet> outset;
        mBackingMap->KeySet((ISet**)&outset);
        AutoPtr<IIterator> it;
        outset->GetIterator((IIterator**)&it);
        Boolean isflag = FALSE;
        while (it->HasNext(&isflag), isflag) {
            assert(0 && "TODO");
            AutoPtr<IInterface> outface;
            it->Next((IInterface**)&outface);
            // stream->WriteObject(outface);
        }
    }
}

void CTreeSet::ReadObject(
    /* [in] */ IObjectInputStream* stream)
{
    stream->DefaultReadObject();
    AutoPtr<IInterface> outface;
    assert(0 && "TODO");
    // stream->ReadObject((IInterface**)&outface);
    AutoPtr<INavigableMap> map;
    CTreeMap::New(IComparator::Probe(outface), (INavigableMap**)&map);
    Int32 size = 0;
    stream->Read(&size);
    if (size > 0) {
        for (Int32 i = 0; i < size; i++) {
            AutoPtr<IInterface> elem;
            assert(0 && "TODO");
            // stream->ReadObject((IInterface**)&elem);
            AutoPtr<IBoolean> botrue;
            CBoolean::New(TRUE, (IBoolean**)&botrue);
            AutoPtr<IInterface> outface2;
            map->Put(elem, botrue, (IInterface**)&outface2);
        }
    }
    mBackingMap = map;
}

ECode CTreeSet::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return AbstractSet::ContainsAll(collection, result);
}

ECode CTreeSet::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return AbstractSet::Equals(object, result);
}

ECode CTreeSet::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    return AbstractSet::GetHashCode(hashCode);
}

ECode CTreeSet::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::RemoveAll(collection, modified);
}

ECode CTreeSet::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::RetainAll(collection, modified);
}

ECode CTreeSet::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    return AbstractSet::ToArray(array);
}

ECode CTreeSet::ToArray(
    /* [in] */ ArrayOf<IInterface*>* inArray,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    return AbstractSet::ToArray(inArray, outArray);
}

} // namespace Utility
} // namespace Elastos
