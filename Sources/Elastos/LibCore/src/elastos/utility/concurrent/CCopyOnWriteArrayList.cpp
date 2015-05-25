
#include "CCopyOnWriteArrayList.h"

namespace Elastos {
namespace Utility {
namespace Concurrent {

PInterface CCopyOnWriteArrayList::Probe(
    /* [in] */ REIID riid)
{
    return _CCopyOnWriteArrayList::Probe(riid);
}

ECode CCopyOnWriteArrayList::constructor()
{
    return CopyOnWriteArrayList::Init();
}

ECode CCopyOnWriteArrayList::constructor(
    /* [in] */ ICollection* collection)
{
    return CopyOnWriteArrayList::Init(collection);
}

ECode CCopyOnWriteArrayList::constructor(
    /* [in] */ ArrayOf<IInterface*>* array)
{
    return CopyOnWriteArrayList::Init(array);
}

ECode CCopyOnWriteArrayList::Add(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return CopyOnWriteArrayList::Add(object, modified);
}

ECode CCopyOnWriteArrayList::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return CopyOnWriteArrayList::AddAll(collection, modified);
}

ECode CCopyOnWriteArrayList::Clear()
{
    return CopyOnWriteArrayList::Clear();
}

ECode CCopyOnWriteArrayList::Contains(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return CopyOnWriteArrayList::Contains(object, result);
}

ECode CCopyOnWriteArrayList::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return CopyOnWriteArrayList::ContainsAll(collection, result);
}

ECode CCopyOnWriteArrayList::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return CopyOnWriteArrayList::Equals(object, result);
}

ECode CCopyOnWriteArrayList::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    return CopyOnWriteArrayList::GetHashCode(hashCode);
}

ECode CCopyOnWriteArrayList::IsEmpty(
    /* [out] */ Boolean* result)
{
    return CopyOnWriteArrayList::IsEmpty(result);
}

ECode CCopyOnWriteArrayList::Remove(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return CopyOnWriteArrayList::Remove(object, modified);
}

ECode CCopyOnWriteArrayList::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return CopyOnWriteArrayList::RemoveAll(collection, modified);
}

ECode CCopyOnWriteArrayList::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return CopyOnWriteArrayList::RetainAll(collection, modified);
}

ECode CCopyOnWriteArrayList::GetSize(
    /* [out] */ Int32* size)
{
    return CopyOnWriteArrayList::GetSize(size);
}

ECode CCopyOnWriteArrayList::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    return CopyOnWriteArrayList::ToArray(array);
}

ECode CCopyOnWriteArrayList::ToArrayEx(
    /* [in] */ ArrayOf<IInterface*>* inArray,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    return CopyOnWriteArrayList::ToArrayEx(inArray, outArray);
}

ECode CCopyOnWriteArrayList::GetIterator(
    /* [out] */ IIterator** it)
{
    return CopyOnWriteArrayList::GetIterator(it);
}

ECode CCopyOnWriteArrayList::AddEx(
    /* [in] */ Int32 location,
    /* [in] */ IInterface* object)
{
    return CopyOnWriteArrayList::AddEx(location, object);
}

ECode CCopyOnWriteArrayList::AddAllEx(
    /* [in] */ Int32 location,
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return CopyOnWriteArrayList::AddAllEx(location, collection, modified);
}

ECode CCopyOnWriteArrayList::Get(
    /* [in] */ Int32 location,
    /* [out] */ IInterface** object)
{
    return CopyOnWriteArrayList::Get(location, object);
}

ECode CCopyOnWriteArrayList::IndexOf(
    /* [in] */ IInterface* object,
    /* [out] */ Int32* index)
{
    return CopyOnWriteArrayList::IndexOf(object, index);
}

ECode CCopyOnWriteArrayList::LastIndexOf(
    /* [in] */ IInterface* object,
    /* [out] */ Int32* index)
{
    return CopyOnWriteArrayList::LastIndexOf(object, index);
}

ECode CCopyOnWriteArrayList::GetListIterator(
    /* [out] */ IListIterator** it)
{
    return CopyOnWriteArrayList::GetListIterator(it);
}

ECode CCopyOnWriteArrayList::GetListIteratorEx(
    /* [in] */ Int32 location,
    /* [out] */ IListIterator** it)
{
    return CopyOnWriteArrayList::GetListIteratorEx(location, it);
}

ECode CCopyOnWriteArrayList::RemoveEx(
    /* [in] */ Int32 location,
    /* [out] */ IInterface** object)
{
    return CopyOnWriteArrayList::RemoveEx(location, object);
}

ECode CCopyOnWriteArrayList::Set(
    /* [in] */ Int32 location,
    /* [in] */ IInterface* object,
    /* [out] */ IInterface** prevObject)
{
    return CopyOnWriteArrayList::Set(location, object, prevObject);
}

ECode CCopyOnWriteArrayList::SubList(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ IList** subList)
{
    return CopyOnWriteArrayList::SubList(start, end, subList);
}

ECode CCopyOnWriteArrayList::IndexOfEx(
    /* [in] */ IInterface* object,
    /* [in] */ Int32 from,
    /* [out] */ Int32* value)
{
    return CopyOnWriteArrayList::IndexOfEx(object, from, value);
}

ECode CCopyOnWriteArrayList::LastIndexOfEx(
    /* [in] */ IInterface* object,
    /* [in] */ Int32 to,
    /* [out] */ Int32* value)
{
    return CopyOnWriteArrayList::LastIndexOfEx(object, to, value);
}

ECode CCopyOnWriteArrayList::AddAllAbsent(
    /* [in] */ ICollection* collection,
    /* [out] */ Int32* value)
{
    return CopyOnWriteArrayList::AddAllAbsent(collection, value);
}

ECode CCopyOnWriteArrayList::AddIfAbsent(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* value)
{
    return CopyOnWriteArrayList::AddIfAbsent(object, value);
}

ECode CCopyOnWriteArrayList::GetArray(
    /* [out, callee] */ ArrayOf<IInterface*>** outarr)
{
    return CopyOnWriteArrayList::GetArray(outarr);
}

ECode CCopyOnWriteArrayList::Clone(
    /* [out] */ IInterface** object)
{
    return CopyOnWriteArrayList::Clone(object);
}

} // namespace Concurrent
} // namespace Utility
} // namespace Elastos
