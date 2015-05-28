
#include "CStack.h"
#include "ObjectUtils.h"

using Elastos::Core::ObjectUtils;

namespace Elastos {
namespace Utility {

ECode CStack::constructor()
{
    return _Vector::Init();
}

PInterface CStack::Probe(
    /* [in] */ REIID riid)
{
    return _CStack::Probe(riid);
}

UInt32 CStack::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CStack::Release()
{
    return ElRefBase::Release();
}

ECode CStack::Add(
    /* [in] */ Int32 location,
    /* [in] */ IInterface* object)
{
    return _Vector::Add(location, object);
}

ECode CStack::Add(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return _Vector::Add(object, modified);
}

ECode CStack::AddAll(
    /* [in] */ Int32 location,
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return _Vector::AddAll(location, collection, modified);
}

ECode CStack::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return _Vector::AddAll(collection, modified);
}

ECode CStack::AddElement(
    /* [in] */ IInterface* object)
{
    return _Vector::AddElement(object);
}

ECode CStack::Capacity(
    /* [out] */ Int32* value)
{
    return _Vector::Capacity(value);
}

ECode CStack::Clear()
{
    return _Vector::Clear();
}

ECode CStack::Clone(
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object)
    AutoPtr<CStack> stack;
    CStack::NewByFriend((CStack**)&stack);
    FAIL_RETURN(_Vector::Clone(stack));
    *object = (IVector*)stack;
    REFCOUNT_ADD(*object)
    return NOERROR;
}

ECode CStack::Contains(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return _Vector::Contains(object, result);
}

ECode CStack::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return _Vector::ContainsAll(collection, result);
}

ECode CStack::CopyInto(
    /* [in] */ ArrayOf<IInterface*>* elements)
{
    return _Vector::CopyInto(elements);
}

ECode CStack::ElementAt(
    /* [in] */ Int32 location,
    /* [out] */ IInterface** outface)
{
    return _Vector::ElementAt(location, outface);
}

ECode CStack::Elements(
    /* [out] */ IEnumeration** enu)
{
    return _Vector::Elements(enu);
}

ECode CStack::EnsureCapacity(
    /* [in] */ Int32 minimumCapacity)
{
    return _Vector::EnsureCapacity(minimumCapacity);
}

ECode CStack::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return _Vector::Equals(object, result);
}

ECode CStack::FirstElement(
    /* [out] */ IInterface** outface)
{
    return _Vector::FirstElement(outface);
}

ECode CStack::Get(
    /* [in] */ Int32 location,
    /* [out] */ IInterface** object)
{
    return _Vector::Get(location, object);
}

ECode CStack::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    return _Vector::GetHashCode(hashCode);
}

ECode CStack::IndexOf(
    /* [in] */ IInterface* object,
    /* [out] */ Int32* index)
{
    return _Vector::IndexOf(object, index);
}

ECode CStack::IndexOf(
    /* [in] */ IInterface* object,
    /* [in] */ Int32 location,
    /* [out] */ Int32* value)
{
    return _Vector::IndexOf(object, location, value);
}

ECode CStack::InsertElementAt(
    /* [in] */ IInterface* object,
    /* [in] */ Int32 location)
{
    return _Vector::InsertElementAt(object, location);
}

ECode CStack::IsEmpty(
    /* [out] */ Boolean* result)
{
    return _Vector::IsEmpty(result);
}

ECode CStack::LastElement(
    /* [out] */ IInterface** outface)
{
    return _Vector::LastElement(outface);
}

ECode CStack::LastIndexOf(
    /* [in] */ IInterface* object,
    /* [out] */ Int32* index)
{
    return _Vector::LastIndexOf(object, index);
}

ECode CStack::LastIndexOf(
    /* [in] */ IInterface* object,
    /* [in] */ Int32 location,
    /* [out] */ Int32* value)
{
    return _Vector::LastIndexOf(object, location, value);
}

ECode CStack::Remove(
    /* [in] */ Int32 location,
    /* [out] */ IInterface** object)
{
    return _Vector::Remove(location, object);
}

ECode CStack::Remove(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return _Vector::Remove(object, modified);
}

ECode CStack::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return _Vector::RemoveAll(collection, modified);
}

ECode CStack::RemoveAllElements()
{
    return _Vector::RemoveAllElements();
}

ECode CStack::RemoveElement(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* value)
{
    return _Vector::RemoveElement(object, value);
}

ECode CStack::RemoveElementAt(
    /* [in] */ Int32 location)
{
    return _Vector::RemoveElementAt(location);
}

ECode CStack::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return _Vector::RetainAll(collection, modified);
}

ECode CStack::Set(
    /* [in] */ Int32 location,
    /* [in] */ IInterface* object,
    /* [out] */ IInterface** prevObject)
{
    return _Vector::Set(location, object, prevObject);
}

ECode CStack::SetElementAt(
    /* [in] */ IInterface* object,
    /* [in] */ Int32 location)
{
    return _Vector::SetElementAt(object, location);
}

ECode CStack::SetSize(
    /* [in] */ Int32 length)
{
    return _Vector::SetSize(length);
}

ECode CStack::GetSize(
    /* [out] */ Int32* size)
{
    return _Vector::GetSize(size);
}

ECode CStack::SubList(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ IList** subList)
{
    return _Vector::SubList(start, end, subList);
}

ECode CStack::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    return _Vector::ToArray(array);
}

ECode CStack::ToArray(
    /* [in] */ ArrayOf<IInterface*>* inArray,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    return _Vector::ToArray(inArray, outArray);
}

CARAPI CStack::ToString(
    /* [out] */ String* result)
{
    return _Vector::ToString(result);
}

ECode CStack::TrimToSize()
{
    return _Vector::TrimToSize();
}

ECode CStack::GetIterator(
    /* [out] */ IIterator** it)
{
    return _Vector::GetIterator(it);
}

ECode CStack::GetListIterator(
    /* [out] */ IListIterator** it)
{
    return _Vector::GetListIterator(it);
}

ECode CStack::GetListIterator(
    /* [in] */ Int32 location,
    /* [out] */ IListIterator** it)
{
    return _Vector::GetListIterator(location, it);
}

ECode CStack::Empty(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)

    return IsEmpty(value);
}

ECode CStack::Peek(
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface)
    Mutex::Autolock lock(GetSelfLock());
    if (mElementCount == 0 || mElementData->GetLength() < mElementCount) {
        return E_EMPTY_STACK_EXCEPTION;
    }

    *outface = (*mElementData)[mElementCount - 1];
    REFCOUNT_ADD(*outface)

    return NOERROR;
}

ECode CStack::Pop(
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface)
    Mutex::Autolock lock(GetSelfLock());
    if (mElementCount == 0) {
        return E_EMPTY_STACK_EXCEPTION;
    }
    Int32 index = --mElementCount;
    AutoPtr<IInterface> obj = (*mElementData)[index];
    mElementData->Set(index, NULL);
    mModCount++;
    *outface = obj;
    REFCOUNT_ADD(*outface)
    return NOERROR;
}

ECode CStack::Push(
    /* [in] */ IInterface* object)
{
    return AddElement(object);
}

ECode CStack::Search(
    /* [in] */ IInterface* o,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)
    Mutex::Autolock lock(GetSelfLock());
    AutoPtr< ArrayOf<IInterface*> > dumpArray = mElementData;
    Int32 size = mElementCount;
    if (o != NULL) {
        for (Int32 i = size - 1; i >= 0; i--) {
            if (ObjectUtils::Equals(o, (*dumpArray)[i])) {
                *value = size - i;
                return NOERROR;
            }
        }
    }
    else {
        for (Int32 i = size - 1; i >= 0; i--) {
            if ((*dumpArray)[i] == NULL) {
                *value = size - i;
                return NOERROR;
            }
        }
    }
    *value = -1;
    return NOERROR;
}

Mutex* CStack::GetSelfLock()
{
    return &_m_syncLock;
}

} // namespace Utility
} // namespace Elastos
