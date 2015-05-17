
#include "CHashSet.h"

namespace Elastos {
namespace Utility {

PInterface CHashSet::Probe(
    /* [in] */ REIID riid)
{
    return _CHashSet::Probe(riid);
}

ECode CHashSet::constructor()
{
    return HashSet::Init();
}

ECode CHashSet::constructor(
    /* [in] */ Int32 capacity)
{
    return HashSet::Init(capacity);
}

ECode CHashSet::constructor(
    /* [in] */ Int32 capacity,
    /* [in] */ Float loadFactor)
{
    return HashSet::Init(capacity, loadFactor);
}

ECode CHashSet::constructor(
    /* [in] */ ICollection* collection)
{
    return HashSet::Init(collection);
}

ECode CHashSet::constructor(
    /* [in] */ IMap* backingMap)
{
    return HashSet::Init(backingMap);
}

ECode CHashSet::Add(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return HashSet::Add(object, modified);
}

ECode CHashSet::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return HashSet::AddAll(collection, modified);
}

ECode CHashSet::Clear()
{
    return HashSet::Clear();
}

ECode CHashSet::Clone(
    /* [out] */ IInterface** outface)
{
    return HashSet::Clone(outface);
}

ECode CHashSet::Contains(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return HashSet::Contains(object, result);
}

ECode CHashSet::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return HashSet::ContainsAll(collection, result);
}

ECode CHashSet::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return HashSet::Equals(object, result);
}

ECode CHashSet::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    return HashSet::GetHashCode(hashCode);
}

ECode CHashSet::IsEmpty(
    /* [out] */ Boolean* result)
{
    return HashSet::IsEmpty(result);
}

ECode CHashSet::Remove(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return HashSet::Remove(object, modified);
}

ECode CHashSet::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return HashSet::RemoveAll(collection, modified);
}

ECode CHashSet::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return HashSet::RetainAll(collection, modified);
}

ECode CHashSet::GetSize(
    /* [out] */ Int32* size)
{
    return HashSet::GetSize(size);
}

ECode CHashSet::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    return HashSet::ToArray(array);
}

ECode CHashSet::ToArrayEx(
    /* [in] */ ArrayOf<IInterface*>* inArray,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    return HashSet::ToArrayEx(inArray, outArray);
}

ECode CHashSet::GetIterator(
    /* [out] */ IIterator** it)
{
    return HashSet::GetIterator(it);
}

} // namespace Utility
} // namespace Elastos
