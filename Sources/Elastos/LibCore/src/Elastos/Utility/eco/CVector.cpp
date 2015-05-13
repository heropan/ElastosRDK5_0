
#include "CVector.h"

namespace Elastos {
namespace Utility {

ECode CVector::constructor()
{
    return Init();
}

ECode CVector::constructor(
    /* [in] */ Int32 capacity)
{
    return Init(capacity);
}

ECode CVector::constructor(
    /* [in] */ Int32 capacity,
    /* [in] */ Int32 capacityIncrement)
{
    return Init(capacity, capacityIncrement);
}

ECode CVector::constructor(
    /* [in] */ ICollection* collection)
{
    return Init(collection);
}

PInterface CVector::Probe(
    /* [in] */ REIID riid)
{
    return _CVector::Probe(riid);
}

UInt32 CVector::AddRef()
{
    return _CVector::AddRef();
}

UInt32 CVector::Release()
{
    return _CVector::Release();
}

ECode CVector::AddEx(
    /* [in] */ Int32 location,
    /* [in] */ IInterface* object)
{
    return _Vector::AddEx(location, object);
}

ECode CVector::Add(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return _Vector::Add(object, modified);
}

ECode CVector::AddAllEx(
    /* [in] */ Int32 location,
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return _Vector::AddAllEx(location, collection, modified);
}

ECode CVector::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return _Vector::AddAll(collection, modified);
}

ECode CVector::AddElement(
    /* [in] */ IInterface* object)
{
    return _Vector::AddElement(object);
}

ECode CVector::Capacity(
    /* [out] */ Int32* value)
{
    return _Vector::Capacity(value);
}

ECode CVector::Clear()
{
    return _Vector::Clear();
}

ECode CVector::Clone(
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object)
    // try {
    AutoPtr<CVector> v;
    CVector::NewByFriend((CVector**)&v);
    _Vector::Clone(v);
    *object = (IVector*)v;
    INTERFACE_ADDREF(*object)
    return NOERROR;
    // } catch (CloneNotSupportedException e) {
    //     throw new AssertionError(e);
    // }
}

ECode CVector::Contains(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return _Vector::Contains(object, result);
}

ECode CVector::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return _Vector::ContainsAll(collection, result);
}

ECode CVector::CopyInto(
    /* [in] */ ArrayOf<IInterface*>* elements)
{
    return _Vector::CopyInto(elements);
}

ECode CVector::ElementAt(
    /* [in] */ Int32 location,
    /* [out] */ IInterface** outface)
{
    return _Vector::ElementAt(location, outface);
}

ECode CVector::Elements(
    /* [out] */ IEnumeration** enu)
{
    return _Vector::Elements(enu);
}

ECode CVector::EnsureCapacity(
    /* [in] */ Int32 minimumCapacity)
{
    return _Vector::EnsureCapacity(minimumCapacity);
}

ECode CVector::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return _Vector::Equals(object, result);
}

ECode CVector::FirstElement(
    /* [out] */ IInterface** outface)
{
    return _Vector::FirstElement(outface);
}

ECode CVector::Get(
    /* [in] */ Int32 location,
    /* [out] */ IInterface** object)
{
    return _Vector::Get(location, object);
}

ECode CVector::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    return _Vector::GetHashCode(hashCode);
}

ECode CVector::IndexOf(
    /* [in] */ IInterface* object,
    /* [out] */ Int32* index)
{
    return _Vector::IndexOf(object, index);
}

ECode CVector::IndexOfEx(
    /* [in] */ IInterface* object,
    /* [in] */ Int32 location,
    /* [out] */ Int32* value)
{
    return _Vector::IndexOfEx(object, location, value);
}

ECode CVector::InsertElementAt(
    /* [in] */ IInterface* object,
    /* [in] */ Int32 location)
{
    return _Vector::InsertElementAt(object, location);
}

ECode CVector::IsEmpty(
    /* [out] */ Boolean* result)
{
    return _Vector::IsEmpty(result);
}

ECode CVector::LastElement(
    /* [out] */ IInterface** outface)
{
    return _Vector::LastElement(outface);
}

ECode CVector::LastIndexOf(
    /* [in] */ IInterface* object,
    /* [out] */ Int32* index)
{
    return _Vector::LastIndexOf(object, index);
}

ECode CVector::LastIndexOfEx(
    /* [in] */ IInterface* object,
    /* [in] */ Int32 location,
    /* [out] */ Int32* value)
{
    return _Vector::LastIndexOfEx(object, location, value);
}

ECode CVector::RemoveEx(
    /* [in] */ Int32 location,
    /* [out] */ IInterface** object)
{
    return _Vector::RemoveEx(location, object);
}

ECode CVector::Remove(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return _Vector::Remove(object, modified);
}

ECode CVector::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return _Vector::RemoveAll(collection, modified);
}

ECode CVector::RemoveAllElements()
{
    return _Vector::RemoveAllElements();
}

ECode CVector::RemoveElement(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* value)
{
    return _Vector::RemoveElement(object, value);
}

ECode CVector::RemoveElementAt(
    /* [in] */ Int32 location)
{
    return _Vector::RemoveElementAt(location);
}

ECode CVector::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return _Vector::RetainAll(collection, modified);
}

ECode CVector::Set(
    /* [in] */ Int32 location,
    /* [in] */ IInterface* object,
    /* [out] */ IInterface** prevObject)
{
    return _Vector::Set(location, object, prevObject);
}

ECode CVector::SetElementAt(
    /* [in] */ IInterface* object,
    /* [in] */ Int32 location)
{
    return _Vector::SetElementAt(object, location);
}

ECode CVector::SetSize(
    /* [in] */ Int32 length)
{
    return _Vector::SetSize(length);
}

ECode CVector::GetSize(
    /* [out] */ Int32* size)
{
    return _Vector::GetSize(size);
}

ECode CVector::SubList(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ IList** subList)
{
    return _Vector::SubList(start, end, subList);
}

ECode CVector::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    return _Vector::ToArray(array);
}

ECode CVector::ToArrayEx(
    /* [in] */ ArrayOf<IInterface*>* inArray,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    return _Vector::ToArrayEx(inArray, outArray);
}

CARAPI CVector::ToString(
    /* [out] */ String* result)
{
    return _Vector::ToString(result);
}

ECode CVector::TrimToSize()
{
    return _Vector::TrimToSize();
}

ECode CVector::GetIterator(
    /* [out] */ IIterator** it)
{
    return _Vector::GetIterator(it);
}

ECode CVector::GetListIterator(
    /* [out] */ IListIterator** it)
{
    return _Vector::GetListIterator(it);
}

ECode CVector::GetListIteratorEx(
    /* [in] */ Int32 location,
    /* [out] */ IListIterator** it)
{
    return _Vector::GetListIteratorEx(location, it);
}

Mutex* CVector::GetSelfLock()
{
    return &_m_syncLock;
}

} // namespace Utility
} // namespace Elastos
