
#include "CCollections.h"
#include "CBoolean.h"
#include "CArrays.h"
#include "CArrayList.h"
#include "CRandom.h"
#include <elastos/ObjectUtils.h>

using Elastos::Core::IComparable;
using Elastos::Core::IBoolean;
using Elastos::Core::CBoolean;
using Elastos::Core::EIID_IComparator;
using Elastos::Core::CRandom;
using Elastos::Core::ObjectUtils;
using Elastos::Utility::IArrays;
using Elastos::Utility::CArrays;
using Elastos::Utility::IArrayList;
using Elastos::Utility::CArrayList;
using Elastos::IO::EIID_ISerializable;

namespace Elastos {
namespace Utility {

const AutoPtr<IIterator> CCollections::EMPTY_ITERATOR = new CCollections::Iterator();
const AutoPtr<IEnumeration> CCollections::EMPTY_ENUMERATION = new CCollections::MyEnumeration();

//====================================================================
// CCollections::Iterator::
//====================================================================
CAR_INTERFACE_IMPL(CCollections::Iterator, IIterator)

CCollections::Iterator::Iterator()
{
}

ECode CCollections::Iterator::HasNext(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;
    return NOERROR;
}

ECode CCollections::Iterator::Next(
    /* [out] */ IInterface** object)
{
    return E_NO_SUCH_ELEMENT_EXCEPTION;
}

ECode CCollections::Iterator::Remove()
{
    return E_ILLEGAL_STATE_EXCEPTION;
}

//====================================================================
// CCollections::MyEnumeration::
//====================================================================
CAR_INTERFACE_IMPL(CCollections::MyEnumeration, IEnumeration)

CCollections::MyEnumeration::MyEnumeration()
{
}

ECode CCollections::MyEnumeration::HasMoreElements(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)
    *value = FALSE;
    return NOERROR;
}

ECode CCollections::MyEnumeration::NextElement(
    /* [out] */ IInterface** inter)
{
    return E_NO_SUCH_ELEMENT_EXCEPTION;
}

//====================================================================
// CCollections::MyEnumeration2::
//====================================================================
CAR_INTERFACE_IMPL(CCollections::MyEnumeration2, IEnumeration)

CCollections::MyEnumeration2::MyEnumeration2(
    /* [in] */ ICollection* c)
{
    c->GetIterator((IIterator**)&mIt);
}

ECode CCollections::MyEnumeration2::HasMoreElements(
    /* [out] */ Boolean* value)
{
    return mIt->HasNext(value);
}

ECode CCollections::MyEnumeration2::NextElement(
    /* [out] */ IInterface** inter)
{
    return mIt->Next(inter);
}

//====================================================================
// CCollections::CopiesList::
//====================================================================
UInt32 CCollections::CopiesList::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CCollections::CopiesList::Release()
{
    return ElRefBase::Release();
}

PInterface CCollections::CopiesList::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_IInterface == riid) {
        return (PInterface)(IList*)this;
    }
    else if (EIID_IIterable == riid) {
        return (IIterable*)(IList*)this;
    }
    else if (EIID_ICollection == riid) {
        return (ICollection*)(IList*)this;
    }
    else if (EIID_IList == riid) {
        return (IList*)this;
    }

    return NULL;
}

ECode CCollections::CopiesList::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID)

    if (pObject == (IInterface*)(IList*)this) {
        *pIID = EIID_IList;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

CCollections::CopiesList::CopiesList(
    /* [in] */ Int32 length,
    /* [in] */ IInterface* object)
{
    if (length < 0) {
        assert(0);
        return;
    }
    mN = length;
    mElement = object;
}

ECode CCollections::CopiesList::Contains(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;
    if (mElement == NULL) {
        *result = object == NULL;
    }
    else {
        *result = ObjectUtils::Equals(mElement, object);
    }
    return NOERROR;
}

ECode CCollections::CopiesList::GetSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size)
    *size = mN;
    return NOERROR;
}

ECode CCollections::CopiesList::Get(
    /* [in] */ Int32 location,
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object)
    if (location >= 0 && location < mN) {
        *object = mElement;
        REFCOUNT_ADD(*object)
        return NOERROR;
    }
    return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
}

ECode CCollections::CopiesList::Add(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return AbstractList::Add(object, modified);
}

ECode CCollections::CopiesList::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractList::AddAll(collection, modified);
}

ECode CCollections::CopiesList::Clear()
{
    return AbstractList::Clear();
}

ECode CCollections::CopiesList::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return AbstractList::ContainsAll(collection, result);
}

ECode CCollections::CopiesList::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return AbstractList::Equals(object, result);
}

ECode CCollections::CopiesList::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    return AbstractList::GetHashCode(hashCode);
}

ECode CCollections::CopiesList::IsEmpty(
    /* [out] */ Boolean* result)
{
    return AbstractList::IsEmpty(result);
}

ECode CCollections::CopiesList::Remove(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return AbstractList::Remove(object, modified);
}

ECode CCollections::CopiesList::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractList::RemoveAll(collection, modified);
}

ECode CCollections::CopiesList::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractList::RetainAll(collection, modified);
}

ECode CCollections::CopiesList::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    return AbstractList::ToArray(array);
}

ECode CCollections::CopiesList::ToArray(
    /* [in] */ ArrayOf<IInterface*>* inArray,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    return AbstractList::ToArray(inArray, outArray);
}

ECode CCollections::CopiesList::Add(
    /* [in] */ Int32 location,
    /* [in] */ IInterface* object)
{
    return AbstractList::Add(location, object);
}

ECode CCollections::CopiesList::AddAll(
    /* [in] */ Int32 location,
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractList::AddAll(location, collection, modified);
}

ECode CCollections::CopiesList::IndexOf(
    /* [in] */ IInterface* object,
    /* [out] */ Int32* index)
{
    return AbstractList::IndexOf(object, index);
}

ECode CCollections::CopiesList::LastIndexOf(
    /* [in] */ IInterface* object,
    /* [out] */ Int32* index)
{
    return AbstractList::LastIndexOf(object, index);
}

ECode CCollections::CopiesList::GetListIterator(
    /* [out] */ IListIterator** it)
{
    return AbstractList::GetListIterator(it);
}

ECode CCollections::CopiesList::GetListIterator(
    /* [in] */ Int32 location,
    /* [out] */ IListIterator** it)
{
    return AbstractList::GetListIterator(location, it);
}

ECode CCollections::CopiesList::Remove(
    /* [in] */ Int32 location,
    /* [out] */ IInterface** object)
{
    return AbstractList::Remove(location, object);
}

ECode CCollections::CopiesList::Set(
    /* [in] */ Int32 location,
    /* [in] */ IInterface* object,
    /* [out] */ IInterface** prevObject)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode CCollections::CopiesList::SubList(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ IList** subList)
{
    return AbstractList::SubList(start, end, subList);
}

ECode CCollections::CopiesList::GetIterator(
    /* [out] */ IIterator** it)
{
    return AbstractList::GetIterator(it);
}

//====================================================================
// CCollections::EmptyList::
//====================================================================

UInt32 CCollections::EmptyList::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CCollections::EmptyList::Release()
{
    return ElRefBase::Release();
}

PInterface CCollections::EmptyList::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_IInterface == riid) {
        return (PInterface)(IList*)this;
    }
    else if (EIID_IIterable == riid) {
        return (IIterable*)(IList*)this;
    }
    else if (EIID_ICollection == riid) {
        return (ICollection*)(IList*)this;
    }
    else if (EIID_IList == riid) {
        return (IList*)this;
    }
    else if (Elastos::IO::EIID_ISerializable == riid) {
        return (ISerializable*)this;
    }
    else if (EIID_IRandomAccess == riid) {
        return (IRandomAccess*)this;
    }

    return NULL;
}

ECode CCollections::EmptyList::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID)

    if (pObject == (IInterface*)(IList*)this) {
        *pIID = EIID_IList;
    }
    else if (pObject == (IInterface*)(ISerializable*)this) {
        *pIID = Elastos::IO::EIID_ISerializable;
    }
    else if (pObject == (IInterface*)(IRandomAccess*)this) {
        *pIID = EIID_IRandomAccess;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ECode CCollections::EmptyList::Contains(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;
    return NOERROR;
}

ECode CCollections::EmptyList::GetSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size)
    *size = 0;
    return NOERROR;
}

ECode CCollections::EmptyList::Get(
    /* [in] */ Int32 location,
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object)
    *object = NULL;
    return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
}

ECode CCollections::EmptyList::Set(
    /* [in] */ Int32 location,
    /* [in] */ IInterface* object,
    /* [out] */ IInterface** prevObject)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode CCollections::EmptyList::GetIterator(
    /* [out] */ IIterator** it)
{
    return AbstractList::GetIterator(it);
}

ECode CCollections::EmptyList::Add(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return AbstractList::Add(object, result);
}

ECode CCollections::EmptyList::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return AbstractList::AddAll(collection, result);
}

ECode CCollections::EmptyList::Clear()
{
    return AbstractList::Clear();
}

ECode CCollections::EmptyList::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return AbstractList::ContainsAll(collection, result);
}

ECode CCollections::EmptyList::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return AbstractList::Equals(object, result);
}

ECode CCollections::EmptyList::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    return AbstractList::GetHashCode(hashCode);
}

ECode CCollections::EmptyList::IsEmpty(
    /* [out] */ Boolean* result)
{
    return AbstractList::IsEmpty(result);
}

ECode CCollections::EmptyList::Remove(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return AbstractList::Remove(object, result);
}

ECode CCollections::EmptyList::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return AbstractList::RemoveAll(collection, result);
}

ECode  CCollections::EmptyList::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return AbstractList::RetainAll(collection, result);
}

ECode CCollections::EmptyList::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    return AbstractList::ToArray(array);
}

ECode CCollections::EmptyList::ToArray(
    /* [in] */ ArrayOf<IInterface*>* contents,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    return AbstractList::ToArray(contents, outArray);
}

ECode CCollections::EmptyList::Add(
    /* [in] */ Int32 location,
    /* [in] */ IInterface* object)
{
    return AbstractList::Add(location, object);
}

ECode CCollections::EmptyList::AddAll(
    /* [in] */ Int32 location,
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return AbstractList::AddAll(location, collection, result);
}

ECode CCollections::EmptyList::IndexOf(
    /* [in] */ IInterface* object,
    /* [out] */ Int32* index)
{
    return AbstractList::IndexOf(object, index);
}

ECode CCollections::EmptyList::LastIndexOf(
    /* [in] */ IInterface* object,
    /* [out] */ Int32* index)
{
    return AbstractList::LastIndexOf(object, index);
}

ECode CCollections::EmptyList::GetListIterator(
    /* [out] */ IListIterator** it)
{
    return AbstractList::GetListIterator(it);
}

ECode CCollections::EmptyList::SubList(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ IList** subList)
{
    return AbstractList::SubList(start, end, subList);
}

ECode CCollections::EmptyList::GetListIterator(
    /* [in] */ Int32 location,
    /* [out] */ IListIterator** listiterator)
{
    return AbstractList::GetListIterator(location, listiterator);
}

ECode CCollections::EmptyList::Remove(
    /* [in] */ Int32 location,
    /* [out] */ IInterface** object)
{
    return AbstractList::Remove(location, object);
}

AutoPtr<IInterface> CCollections::EmptyList::ReadResolve()
{
    return CCollections::EMPTY_LIST;
}

//====================================================================
// CCollections::EmptySet::
//====================================================================

UInt32 CCollections::EmptySet::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CCollections::EmptySet::Release()
{
    return ElRefBase::Release();
}

PInterface CCollections::EmptySet::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_IInterface == riid) {
        return (PInterface)(ISet*)this;
    }
    else if (EIID_IIterable == riid) {
        return (IIterable*)(ISet*)this;
    }
    else if (EIID_ICollection == riid) {
        return (ICollection*)(ISet*)this;
    }
    else if (EIID_ISet == riid) {
        return (ISet*)this;
    }
    else if (Elastos::IO::EIID_ISerializable == riid) {
        return (ISerializable*)this;
    }

    return NULL;
}

ECode CCollections::EmptySet::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID)

    if (pObject == (IInterface*)(ISet*)this) {
        *pIID = EIID_ISet;
    }
    else if (pObject == (IInterface*)(ISerializable*)this) {
        *pIID = Elastos::IO::EIID_ISerializable;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ECode CCollections::EmptySet::Contains(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;
    return NOERROR;
}

ECode CCollections::EmptySet::GetSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size)
    *size = 0;
    return NOERROR;
}

ECode CCollections::EmptySet::GetIterator(
    /* [out] */ IIterator** it)
{
    VALIDATE_NOT_NULL(it)
    *it = EMPTY_ITERATOR;
    REFCOUNT_ADD(*it)
    return NOERROR;
}

ECode CCollections::EmptySet::Add(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::Add(object, modified);
}

ECode CCollections::EmptySet::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::AddAll(collection, modified);
}

ECode CCollections::EmptySet::Clear()
{
    return AbstractSet::Clear();
}

ECode CCollections::EmptySet::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return AbstractSet::ContainsAll(collection, result);
}

ECode CCollections::EmptySet::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return AbstractSet::Equals(object, result);
}

ECode CCollections::EmptySet::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    return AbstractSet::GetHashCode(hashCode);
}

ECode CCollections::EmptySet::IsEmpty(
    /* [out] */ Boolean* result)
{
    return AbstractSet::IsEmpty(result);
}

ECode CCollections::EmptySet::Remove(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::Remove(object, modified);
}

ECode CCollections::EmptySet::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::RemoveAll(collection, modified);
}

ECode CCollections::EmptySet::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::RetainAll(collection, modified);
}

ECode CCollections::EmptySet::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    return AbstractSet::ToArray(array);
}

ECode CCollections::EmptySet::ToArray(
    /* [in] */ ArrayOf<IInterface*>* inArray,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    return AbstractSet::ToArray(inArray, outArray);
}

AutoPtr<IInterface> CCollections::EmptySet::ReadResolve()
{
    return CCollections::EMPTY_SET;
}

//====================================================================
// CCollections::EmptyMap::
//====================================================================

UInt32 CCollections::EmptyMap::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CCollections::EmptyMap::Release()
{
    return ElRefBase::Release();
}

PInterface CCollections::EmptyMap::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_IInterface == riid) {
        return (PInterface)(IMap*)this;
    }
    else if (EIID_IIterable == riid) {
        return (IIterable*)(IMap*)this;
    }
    else if (EIID_ICollection == riid) {
        return (ICollection*)(IMap*)this;
    }
    else if (EIID_IMap == riid) {
        return (IMap*)this;
    }
    else if (Elastos::IO::EIID_ISerializable == riid) {
        return (ISerializable*)this;
    }

    return NULL;
}

ECode CCollections::EmptyMap::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID)

    if (pObject == (IInterface*)(IMap*)this) {
        *pIID = EIID_IMap;
    }
    else if (pObject == (IInterface*)(ISerializable*)this) {
        *pIID = Elastos::IO::EIID_ISerializable;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ECode CCollections::EmptyMap::ContainsKey(
    /* [in] */ IInterface* key,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;
    return NOERROR;
}

ECode CCollections::EmptyMap::ContainsValue(
    /* [in] */ IInterface* value,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;
    return NOERROR;
}

ECode CCollections::EmptyMap::EntrySet(
    /* [out, callee] */ ISet** entries)
{
    VALIDATE_NOT_NULL(entries)
    *entries = EMPTY_SET;
    REFCOUNT_ADD(*entries)
    return NOERROR;
}

ECode CCollections::EmptyMap::Get(
    /* [in] */ PInterface key,
    /* [out] */ PInterface* value)
{
    VALIDATE_NOT_NULL(value)
    *value = NULL;
    return NOERROR;
}

ECode CCollections::EmptyMap::KeySet(
    /* [out, callee] */ ISet** keySet)
{
    VALIDATE_NOT_NULL(keySet)
    *keySet = EMPTY_SET;
    REFCOUNT_ADD(*keySet)
    return NOERROR;
}

ECode CCollections::EmptyMap::Values(
    /* [out] */ ICollection** value)
{
    VALIDATE_NOT_NULL(value)
    *value = EMPTY_LIST;
    REFCOUNT_ADD(*value)
    return NOERROR;
}

ECode CCollections::EmptyMap::Clear()
{
    return AbstractMap::Clear();
}

ECode CCollections::EmptyMap::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return AbstractMap::Equals(object, result);
}

ECode CCollections::EmptyMap::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    return AbstractMap::GetHashCode(hashCode);
}

ECode CCollections::EmptyMap::IsEmpty(
    /* [out] */ Boolean* result)
{
    return AbstractMap::IsEmpty(result);
}

ECode CCollections::EmptyMap::Put(
    /* [in] */ PInterface key,
    /* [in] */ PInterface value,
    /* [out] */ PInterface* oldValue)
{
    return AbstractMap::Put(key, value, oldValue);
}

ECode CCollections::EmptyMap::PutAll(
    /* [in] */ IMap* map)
{
    return AbstractMap::PutAll(map);
}

ECode CCollections::EmptyMap::Remove(
    /* [in] */ PInterface key,
    /* [out] */ PInterface* value)
{
    return AbstractMap::Remove(key, value);
}

ECode CCollections::EmptyMap::GetSize(
    /* [out] */ Int32* size)
{
    return AbstractMap::GetSize(size);
}

AutoPtr<IInterface> CCollections::EmptyMap::ReadResolve()
{
    return CCollections::EMPTY_MAP;
}

const AutoPtr<IList> CCollections::EMPTY_LIST = new CCollections::EmptyList();

const AutoPtr<ISet> CCollections::EMPTY_SET = new CCollections::EmptySet();

const AutoPtr<IMap> CCollections::EMPTY_MAP = new CCollections::EmptyMap();

//====================================================================
// CCollections::ReverseComparator::
//====================================================================
CAR_INTERFACE_IMPL_2(CCollections::ReverseComparator, IComparator, ISerializable)

const AutoPtr<CCollections::ReverseComparator> CCollections::ReverseComparator::INSTANCE = new CCollections::ReverseComparator();

ECode CCollections::ReverseComparator::Compare(
    /* [in] */ IInterface* lhs,
    /* [in] */ IInterface* rhs,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;
    AutoPtr<IComparable> c2 = IComparable::Probe(rhs);
    if (c2 != NULL) {
        return c2->CompareTo(lhs, result);
    }
    return NOERROR;
}

AutoPtr<IInterface> CCollections::ReverseComparator::ReadResolve()
{
    return INSTANCE->Probe(EIID_IInterface);
}

//====================================================================
// CCollections::ReverseComparator2::
//====================================================================
CAR_INTERFACE_IMPL_2(CCollections::ReverseComparator2, IComparator, ISerializable)

CCollections::ReverseComparator2::ReverseComparator2(
    /* [in] */ IComparator* comparator)
{
    mCmp = comparator;
}

ECode CCollections::ReverseComparator2::Compare(
    /* [in] */ IInterface* lhs,
    /* [in] */ IInterface* rhs,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)
    return mCmp->Compare(rhs, lhs, result);
}

ECode CCollections::ReverseComparator2::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    AutoPtr<IComparator> p = IComparator::Probe(object);
    if (p != NULL) {
        *result = ObjectUtils::Equals(mCmp, ((ReverseComparator2*)p.Get())->mCmp);
    }
    *result = FALSE;
    return NOERROR;
}

ECode CCollections::ReverseComparator2::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode)
    AutoPtr<IObject> o = IObject::Probe(mCmp);
    assert(o != NULL);
    Int32 code;
    o->GetHashCode(&code);
    *hashCode = ~code;
    return NOERROR;
}

//====================================================================
// CCollections::SingletonSet::Iterator::
//====================================================================
CAR_INTERFACE_IMPL(CCollections::SingletonSet::Iterator, IIterator)

CCollections::SingletonSet::Iterator::Iterator(
    /* [in] */ SingletonSet* owner)
{
    mHasNext = TRUE;
    mOwner = owner;
}

ECode CCollections::SingletonSet::Iterator::HasNext(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = mHasNext;
    return NOERROR;
}

ECode CCollections::SingletonSet::Iterator::Next(
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object)
    if (mHasNext) {
        mHasNext = FALSE;
        *object = mOwner->mElement;
        REFCOUNT_ADD(*object)
        return NOERROR;
    }
    return E_NO_SUCH_ELEMENT_EXCEPTION;
}

ECode CCollections::SingletonSet::Iterator::Remove()
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

//====================================================================
// CCollections::SingletonSet::
//====================================================================

UInt32 CCollections::SingletonSet::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CCollections::SingletonSet::Release()
{
    return ElRefBase::Release();
}

PInterface CCollections::SingletonSet::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_IInterface == riid) {
        return (PInterface)(ISet*)this;
    }
    else if (EIID_IIterable == riid) {
        return (IIterable*)(ISet*)this;
    }
    else if (EIID_ICollection == riid) {
        return (ICollection*)(ISet*)this;
    }
    else if (EIID_ISet == riid) {
        return (ISet*)this;
    }
    else if (Elastos::IO::EIID_ISerializable == riid) {
        return (ISerializable*)this;
    }

    return NULL;
}

ECode CCollections::SingletonSet::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID)

    if (pObject == (IInterface*)(ISet*)this) {
        *pIID = EIID_ISet;
    }
    else if (pObject == (IInterface*)(ISerializable*)this) {
        *pIID = Elastos::IO::EIID_ISerializable;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

CCollections::SingletonSet::SingletonSet(
    /* [in] */ IInterface* object)
{
    mElement = object;
}

ECode CCollections::SingletonSet::Contains(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;
    if (mElement == NULL) {
        *result = object == NULL;
    }
    else {
        *result = ObjectUtils::Equals(mElement, object);
    }
    return NOERROR;
}

ECode CCollections::SingletonSet::GetSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size)
    *size = 1;
    return NOERROR;
}

ECode CCollections::SingletonSet::GetIterator(
    /* [out] */ IIterator** it)
{
    VALIDATE_NOT_NULL(it)
    *it = new Iterator(this);
    REFCOUNT_ADD(*it)
    return NOERROR;
}

ECode CCollections::SingletonSet::Add(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::Add(object, modified);
}

ECode CCollections::SingletonSet::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::AddAll(collection, modified);
}

ECode CCollections::SingletonSet::Clear()
{
    return AbstractSet::Clear();
}

ECode CCollections::SingletonSet::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return AbstractSet::ContainsAll(collection, result);
}

ECode CCollections::SingletonSet::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return AbstractSet::Equals(object, result);
}

ECode CCollections::SingletonSet::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    return AbstractSet::GetHashCode(hashCode);
}

ECode CCollections::SingletonSet::IsEmpty(
    /* [out] */ Boolean* result)
{
    return AbstractSet::IsEmpty(result);
}

ECode CCollections::SingletonSet::Remove(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::Remove(object, modified);
}

ECode CCollections::SingletonSet::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::RemoveAll(collection, modified);
}

ECode CCollections::SingletonSet::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::RetainAll(collection, modified);
}

ECode CCollections::SingletonSet::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    return AbstractSet::ToArray(array);
}

ECode CCollections::SingletonSet::ToArray(
    /* [in] */ ArrayOf<IInterface*>* inArray,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    return AbstractSet::ToArray(inArray, outArray);
}

//====================================================================
// CCollections::SingletonList::
//====================================================================

UInt32 CCollections::SingletonList::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CCollections::SingletonList::Release()
{
    return ElRefBase::Release();
}

PInterface CCollections::SingletonList::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_IInterface == riid) {
        return (PInterface)(IList*)this;
    }
    else if (EIID_IIterable == riid) {
        return (IIterable*)(IList*)this;
    }
    else if (EIID_ICollection == riid) {
        return (ICollection*)(IList*)this;
    }
    else if (EIID_IList == riid) {
        return (IList*)this;
    }
    else if (Elastos::IO::EIID_ISerializable == riid) {
        return (ISerializable*)this;
    }

    return NULL;
}

ECode CCollections::SingletonList::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID)

    if (pObject == (IInterface*)(IList*)this) {
        *pIID = EIID_IList;
    }
    else if (pObject == (IInterface*)(ISerializable*)this) {
        *pIID = Elastos::IO::EIID_ISerializable;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

CCollections::SingletonList::SingletonList(
    /* [in] */ IInterface* object)
{
    mElement = object;
}

ECode CCollections::SingletonList::Contains(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    if (mElement == NULL) {
        *result = object == NULL;
    }
    else {
        *result = ObjectUtils::Equals(mElement, object);
    }
    return NOERROR;
}

ECode CCollections::SingletonList::Get(
    /* [in] */ Int32 location,
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object)
    if (location == 0) {
        *object = mElement;
        REFCOUNT_ADD(*object)
        return NOERROR;
    }
    return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
}

ECode CCollections::SingletonList::GetSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size)
    *size = 1;
    return NOERROR;
}

ECode CCollections::SingletonList::Add(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return AbstractList::Add(object, modified);
}

ECode CCollections::SingletonList::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractList::AddAll(collection, modified);
}

ECode CCollections::SingletonList::Clear()
{
    return AbstractList::Clear();
}

ECode CCollections::SingletonList::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return AbstractList::ContainsAll(collection, result);
}

ECode CCollections::SingletonList::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return AbstractList::Equals(object, result);
}

ECode CCollections::SingletonList::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    return AbstractList::GetHashCode(hashCode);
}

ECode CCollections::SingletonList::IsEmpty(
    /* [out] */ Boolean* result)
{
    return AbstractList::IsEmpty(result);
}

ECode CCollections::SingletonList::Remove(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return AbstractList::Remove(object, modified);
}

ECode CCollections::SingletonList::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractList::RemoveAll(collection, modified);
}

ECode CCollections::SingletonList::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractList::RetainAll(collection, modified);
}

ECode CCollections::SingletonList::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    return AbstractList::ToArray(array);
}

ECode CCollections::SingletonList::ToArray(
    /* [in] */ ArrayOf<IInterface*>* inArray,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    return AbstractList::ToArray(inArray, outArray);
}

ECode CCollections::SingletonList::Add(
    /* [in] */ Int32 location,
    /* [in] */ IInterface* object)
{
    return AbstractList::Add(location, object);
}

ECode CCollections::SingletonList::AddAll(
    /* [in] */ Int32 location,
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractList::AddAll(location, collection, modified);
}

ECode CCollections::SingletonList::IndexOf(
    /* [in] */ IInterface* object,
    /* [out] */ Int32* index)
{
    return AbstractList::IndexOf(object, index);
}

ECode CCollections::SingletonList::LastIndexOf(
    /* [in] */ IInterface* object,
    /* [out] */ Int32* index)
{
    return AbstractList::LastIndexOf(object, index);
}

ECode CCollections::SingletonList::GetListIterator(
    /* [out] */ IListIterator** it)
{
    return AbstractList::GetListIterator(it);
}

ECode CCollections::SingletonList::GetListIterator(
    /* [in] */ Int32 location,
    /* [out] */ IListIterator** it)
{
    return AbstractList::GetListIterator(location, it);
}

ECode CCollections::SingletonList::Remove(
    /* [in] */ Int32 location,
    /* [out] */ IInterface** object)
{
    return AbstractList::Remove(location, object);
}

ECode CCollections::SingletonList::Set(
    /* [in] */ Int32 location,
    /* [in] */ IInterface* object,
    /* [out] */ IInterface** prevObject)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode CCollections::SingletonList::SubList(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ IList** subList)
{
    return AbstractList::SubList(start, end, subList);
}

ECode CCollections::SingletonList::GetIterator(
    /* [out] */ IIterator** it)
{
    return AbstractList::GetIterator(it);
}

//====================================================================
// CCollections::SingletonMap::MySet::Iterator::MyMapEntry::
//====================================================================
CCollections::SingletonMap::MySet::Iterator::MyMapEntry::MyMapEntry(
    /* [in] */ IInterface* k,
    /* [in] */ IInterface* v)
    : MapEntry(k, v)
{
}

ECode CCollections::SingletonMap::MySet::Iterator::MyMapEntry::SetValue(
    /* [in] */ IInterface* valueReplacer,
    /* [out] */ IInterface** valueReplacee)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

//====================================================================
// CCollections::SingletonMap::MySet::Iterator::
//====================================================================
CAR_INTERFACE_IMPL(CCollections::SingletonMap::MySet::Iterator,  IIterator)

CCollections::SingletonMap::MySet::Iterator::Iterator(
    /* [in] */ MySet* owner)
{
    mHasNext = TRUE;
    mOwner = owner;
}

ECode CCollections::SingletonMap::MySet::Iterator::HasNext(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = mHasNext;
    return NOERROR;
}

ECode CCollections::SingletonMap::MySet::Iterator::Next(
    /* [out] */ IMapEntry** object)
{
    VALIDATE_NOT_NULL(object)
    if (!mHasNext) {
        return E_NO_SUCH_ELEMENT_EXCEPTION;
    }

    mHasNext = FALSE;
    AutoPtr<IMapEntry> p = new MyMapEntry(mOwner->mOwner->mK, mOwner->mOwner->mV);
    *object = IMapEntry::Probe(p);
    REFCOUNT_ADD(*object)
    return NOERROR;
}

ECode CCollections::SingletonMap::MySet::Iterator::Next(
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object)
    AutoPtr<IMapEntry> p = IMapEntry::Probe(*object);
    return Next((IMapEntry**)&p);
}

ECode CCollections::SingletonMap::MySet::Iterator::Remove()
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

//====================================================================
// CCollections::SingletonMap::MySet::
//====================================================================

UInt32 CCollections::SingletonMap::MySet::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CCollections::SingletonMap::MySet::Release()
{
    return ElRefBase::Release();
}

PInterface CCollections::SingletonMap::MySet::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_IInterface == riid) {
        return (PInterface)(ISet*)this;
    }
    else if (EIID_IIterable == riid) {
        return (IIterable*)(ISet*)this;
    }
    else if (EIID_ICollection == riid) {
        return (ICollection*)(ISet*)this;
    }
    else if (EIID_ISet == riid) {
        return (ISet*)this;
    }

    return NULL;
}

ECode CCollections::SingletonMap::MySet::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID)

    if (pObject == (IInterface*)(ISet*)this) {
        *pIID = EIID_ISet;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

CCollections::SingletonMap::MySet::MySet(
    /* [in] */ SingletonMap* owner)
{
    mOwner = owner;
}

ECode CCollections::SingletonMap::MySet::Contains(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    AutoPtr<IMapEntry> entry = IMapEntry::Probe(object);
    if (entry != NULL) {
        AutoPtr<IInterface> key, value;
        entry->GetKey((IInterface**)&key);
        entry->GetValue((IInterface**)&value);
        Boolean a, b;
        mOwner->ContainsKey(key, &a);
        mOwner->ContainsValue(value, &b);
        *result = a && b;
        return NOERROR;
    }
    *result = FALSE;
    return NOERROR;
}

ECode CCollections::SingletonMap::MySet::GetSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size)
    *size = 1;
    return NOERROR;
}

ECode CCollections::SingletonMap::MySet::GetIterator(
    /* [out] */ IIterator** it)
{
    VALIDATE_NOT_NULL(*it)
    *it = new Iterator(this);
    REFCOUNT_ADD(*it)
    return NOERROR;
}

ECode CCollections::SingletonMap::MySet::Add(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::Add(object, modified);
}

ECode CCollections::SingletonMap::MySet::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::AddAll(collection, modified);
}

ECode CCollections::SingletonMap::MySet::Clear()
{
    return AbstractSet::Clear();
}

ECode CCollections::SingletonMap::MySet::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return AbstractSet::ContainsAll(collection, result);
}

ECode CCollections::SingletonMap::MySet::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return AbstractSet::Equals(object, result);
}

ECode CCollections::SingletonMap::MySet::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    return AbstractSet::GetHashCode(hashCode);
}

ECode CCollections::SingletonMap::MySet::IsEmpty(
    /* [out] */ Boolean* result)
{
    return AbstractSet::IsEmpty(result);
}

ECode CCollections::SingletonMap::MySet::Remove(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::Remove(object, modified);
}

ECode CCollections::SingletonMap::MySet::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::RemoveAll(collection, modified);
}

ECode CCollections::SingletonMap::MySet::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::RetainAll(collection, modified);
}

ECode CCollections::SingletonMap::MySet::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    return AbstractSet::ToArray(array);
}

ECode CCollections::SingletonMap::MySet::ToArray(
    /* [in] */ ArrayOf<IInterface*>* inArray,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    return AbstractSet::ToArray(inArray, outArray);
}

//====================================================================
// CCollections::SingletonMap::
//====================================================================

UInt32 CCollections::SingletonMap::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CCollections::SingletonMap::Release()
{
    return ElRefBase::Release();
}

PInterface CCollections::SingletonMap::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_IInterface == riid) {
        return (PInterface)(IMap*)this;
    }
    else if (EIID_IIterable == riid) {
        return (IIterable*)(IMap*)this;
    }
    else if (EIID_ICollection == riid) {
        return (ICollection*)(IMap*)this;
    }
    else if (EIID_IMap == riid) {
        return (IMap*)this;
    }
    else if (Elastos::IO::EIID_ISerializable == riid) {
        return (ISerializable*)this;
    }

    return NULL;
}

ECode CCollections::SingletonMap::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID)

    if (pObject == (IInterface*)(IMap*)this) {
        *pIID = EIID_IMap;
    }
    else if (pObject == (IInterface*)(ISerializable*)this) {
        *pIID = Elastos::IO::EIID_ISerializable;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

CCollections::SingletonMap::SingletonMap(
    /* [in] */ IInterface* key,
    /* [in] */ IInterface* value)
{
    mK = key;
    mV = value;
}

ECode CCollections::SingletonMap::ContainsKey(
    /* [in] */ IInterface* key,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    if (mK == NULL) {
        *result = key == NULL;
    }
    else {
        *result = ObjectUtils::Equals(mK, key);
    }
    return NOERROR;
}

ECode CCollections::SingletonMap::ContainsValue(
    /* [in] */ IInterface* value,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    if (mV == NULL) {
        *result = value == NULL;
    }
    else {
        *result = ObjectUtils::Equals(mV, value);
    }
    return NOERROR;
}

ECode CCollections::SingletonMap::Get(
    /* [in] */ PInterface key,
    /* [out] */ PInterface* value)
{
    VALIDATE_NOT_NULL(value)
    Boolean b;
    if ((ContainsKey(key, &b), b)) {
        *value = mV;
        REFCOUNT_ADD(*value)
        return NOERROR;
    }
    *value = NULL;
    return NOERROR;
}

ECode CCollections::SingletonMap::GetSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size)
    *size = 1;
    return NOERROR;
}

ECode CCollections::SingletonMap::EntrySet(
    /* [out, callee] */ ISet** entries)
{
    VALIDATE_NOT_NULL(entries)
    *entries = new MySet(this);
    REFCOUNT_ADD(*entries)
    return NOERROR;
}

ECode CCollections::SingletonMap::Clear()
{
    return AbstractMap::Clear();
}

ECode CCollections::SingletonMap::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return AbstractMap::Equals(object, result);
}

ECode CCollections::SingletonMap::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    return AbstractMap::GetHashCode(hashCode);
}

ECode CCollections::SingletonMap::IsEmpty(
    /* [out] */ Boolean* result)
{
    return AbstractMap::IsEmpty(result);
}

ECode CCollections::SingletonMap::KeySet(
    /* [out, callee] */ ISet** keySet)
{
    return AbstractMap::KeySet(keySet);
}

ECode CCollections::SingletonMap::Put(
    /* [in] */ PInterface key,
    /* [in] */ PInterface value,
    /* [out] */ PInterface* oldValue)
{
    return AbstractMap::Put(key, value, oldValue);
}

ECode CCollections::SingletonMap::PutAll(
    /* [in] */ IMap* map)
{
    return AbstractMap::PutAll(map);
}

ECode CCollections::SingletonMap::Remove(
    /* [in] */ PInterface key,
    /* [out] */ PInterface* value)
{
    return AbstractMap::Remove(key, value);
}

ECode CCollections::SingletonMap::Values(
    /* [out] */ ICollection** value)
{
    return AbstractMap::Values(value);
}

//====================================================================
// CCollections::SynchronizedCollection::
//====================================================================

UInt32 CCollections::SynchronizedCollection::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CCollections::SynchronizedCollection::Release()
{
    return ElRefBase::Release();
}

PInterface CCollections::SynchronizedCollection::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_IInterface == riid) {
        return (PInterface)(ICollection*)this;
    }
    else if (EIID_IIterable == riid) {
        return (IIterable*)(ICollection*)this;
    }
    else if (EIID_ICollection == riid) {
        return (ICollection*)this;
    }
    else if (Elastos::IO::EIID_ISerializable == riid) {
        return (ISerializable*)this;
    }

    return NULL;
}

ECode CCollections::SynchronizedCollection::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID)

    if (pObject == (IInterface*)(ICollection*)this) {
        *pIID = EIID_ICollection;
    }
    else if (pObject == (IInterface*)(ISerializable*)this) {
        *pIID = Elastos::IO::EIID_ISerializable;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

CCollections::SynchronizedCollection::SynchronizedCollection(
    /* [in] */ ICollection* collection)
{
    mC = collection;
    mMutex = new Mutex;
}

CCollections::SynchronizedCollection::SynchronizedCollection(
    /* [in] */ ICollection* collection,
    /* [in] */ Mutex* mutex)
{
    mC = collection;
    mMutex = mutex;
}

ECode CCollections::SynchronizedCollection::Add(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    VALIDATE_NOT_NULL(modified)
    Mutex::Autolock lock(mMutex);
    return mC->Add(object, modified);
}

ECode CCollections::SynchronizedCollection::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    VALIDATE_NOT_NULL(modified)
    Mutex::Autolock lock(mMutex);
    return mC->AddAll(collection, modified);
}

ECode CCollections::SynchronizedCollection::Clear()
{
    Mutex::Autolock lock(mMutex);
    return mC->Clear();
}

ECode CCollections::SynchronizedCollection::Contains(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    Mutex::Autolock lock(mMutex);
    return mC->Contains(object, result);
}

ECode CCollections::SynchronizedCollection::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    Mutex::Autolock lock(mMutex);
    return mC->ContainsAll(collection, result);
}

ECode CCollections::SynchronizedCollection::IsEmpty(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    Mutex::Autolock lock(mMutex);
    return mC->IsEmpty(result);
}

ECode CCollections::SynchronizedCollection::GetIterator(
    /* [out] */ IIterator** it)
{
    VALIDATE_NOT_NULL(it)
    Mutex::Autolock lock(mMutex);
    return mC->GetIterator(it);
}

ECode CCollections::SynchronizedCollection::Remove(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    VALIDATE_NOT_NULL(modified)
    Mutex::Autolock lock(mMutex);
    return mC->Remove(object, modified);
}

ECode CCollections::SynchronizedCollection::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    VALIDATE_NOT_NULL(modified)
    Mutex::Autolock lock(mMutex);
    return mC->RemoveAll(collection, modified);
}

ECode CCollections::SynchronizedCollection::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    VALIDATE_NOT_NULL(modified)
    Mutex::Autolock lock(mMutex);
    return mC->RetainAll(collection, modified);
}

ECode CCollections::SynchronizedCollection::GetSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size)
    Mutex::Autolock lock(mMutex);
    return mC->GetSize(size);
}

ECode CCollections::SynchronizedCollection::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    VALIDATE_NOT_NULL(*array)
    Mutex::Autolock lock(mMutex);
    return mC->ToArray(array);
}

ECode CCollections::SynchronizedCollection::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    Mutex::Autolock lock(mMutex);
    AutoPtr<IObject> iObj;
    iObj = IObject::Probe(mC);
    if (iObj != NULL) {
        return iObj->ToString(str);
    }
    *str = NULL;
    return NOERROR;
}

ECode CCollections::SynchronizedCollection::ToArray(
    /* [in] */ ArrayOf<IInterface*>* inArray,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    VALIDATE_NOT_NULL(outArray)
    Mutex::Autolock lock(mMutex);
    return mC->ToArray(inArray, outArray);
}

ECode CCollections::SynchronizedCollection::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = this->Probe(EIID_IInterface) == object;
    return NOERROR;
}

ECode CCollections::SynchronizedCollection::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode)
    *hashCode = (Int32)this;
    return NOERROR;
}

ECode CCollections::SynchronizedCollection::WriteObject(
    /* [in] */ IObjectOutputStream* stream)
{
    Mutex::Autolock lock(mMutex);
    return stream->DefaultWriteObject();
}

//====================================================================
// CCollections::SynchronizedRandomAccessList::
//====================================================================

UInt32 CCollections::SynchronizedRandomAccessList::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CCollections::SynchronizedRandomAccessList::Release()
{
    return ElRefBase::Release();
}

PInterface CCollections::SynchronizedRandomAccessList::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_IRandomAccess == riid) {
        return (IRandomAccess*)this;
    }
    return SynchronizedList::Probe(riid);
}

ECode CCollections::SynchronizedRandomAccessList::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID)

    if (pObject == (IInterface*)(IRandomAccess*)this) {
        *pIID = EIID_IRandomAccess;
    }
    return SynchronizedList::GetInterfaceID(pObject, pIID);
}

CCollections::SynchronizedRandomAccessList::SynchronizedRandomAccessList(
    /* [in] */ IList* l)
    : SynchronizedList(l)
{
}

CCollections::SynchronizedRandomAccessList::SynchronizedRandomAccessList(
    /* [in] */ IList* l,
    /* [in] */ Mutex* mutex)
    : SynchronizedList(l, mutex)
{
}

ECode CCollections::SynchronizedRandomAccessList::SubList(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ IList** subList)
{
    VALIDATE_NOT_NULL(*subList)
    Mutex::Autolock lock(mMutex);
    AutoPtr<IList> sub;
    mList->SubList(start, end, (IList**)&sub);
    *subList = new SynchronizedRandomAccessList(sub, mMutex);
    REFCOUNT_ADD(*subList)
    return NOERROR;
}

AutoPtr<IInterface> CCollections::SynchronizedRandomAccessList::WriteReplace()
{
    AutoPtr<SynchronizedList> list = new SynchronizedList(mList);
    return (IInterface*)list->Probe(EIID_IInterface);
}

//====================================================================
// CCollections::SynchronizedList::
//====================================================================

UInt32 CCollections::SynchronizedList::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CCollections::SynchronizedList::Release()
{
    return ElRefBase::Release();
}

PInterface CCollections::SynchronizedList::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_IInterface == riid) {
        return (PInterface)(IList*)this;
    }
    else if (EIID_IIterable == riid) {
        return (IIterable*)(IList*)this;
    }
    else if (EIID_ICollection == riid) {
        return (ICollection*)(IList*)this;
    }
    else if (EIID_IList == riid) {
        return (IList*)this;
    }
    else if (Elastos::IO::EIID_ISerializable == riid) {
        return (ISerializable*)this;
    }

    return NULL;
}

ECode CCollections::SynchronizedList::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID)

    if (pObject == (IInterface*)(IList*)this) {
        *pIID = EIID_IList;
    }
    else if (pObject == (IInterface*)(ISerializable*)this) {
        *pIID = Elastos::IO::EIID_ISerializable;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

CCollections::SynchronizedList::SynchronizedList(
    /* [in] */ IList* l)
    : SynchronizedCollection(l)
{
    mList = l;
}

CCollections::SynchronizedList::SynchronizedList(
    /* [in] */ IList* l,
    /* [in] */ Mutex* mutex)
    : SynchronizedCollection(l, mutex)
{
    mList = l;
}

ECode CCollections::SynchronizedList::Add(
    /* [in] */ Int32 location,
    /* [in] */ IInterface* object)
{
    Mutex::Autolock lock(mMutex);
    return mList->Add(location, object);
}

ECode CCollections::SynchronizedList::AddAll(
    /* [in] */ Int32 location,
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    VALIDATE_NOT_NULL(modified)
    Mutex::Autolock lock(mMutex);
    return mList->AddAll(location, collection, modified);
}

ECode CCollections::SynchronizedList::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    Mutex::Autolock lock(mMutex);
    return mList->Equals(object, result);
}

ECode CCollections::SynchronizedList::Get(
    /* [in] */ Int32 location,
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(*object)
    Mutex::Autolock lock(mMutex);
    return mList->Get(location, object);
}

ECode CCollections::SynchronizedList::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode)
    Mutex::Autolock lock(mMutex);
    return mList->GetHashCode(hashCode);
}

ECode CCollections::SynchronizedList::IndexOf(
    /* [in] */ IInterface* object,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index)
    Int32 size;
    AutoPtr<ArrayOf<IInterface*> > array;
    {
        Mutex::Autolock lock(mMutex);
        mList->GetSize(&size);
        array = ArrayOf<IInterface*>::Alloc(size);
        mList->ToArray((ArrayOf<IInterface*>**)&array);
    }
    if (object != NULL) {
        for (Int32 i = 0; i < size; i++) {
            if (ObjectUtils::Equals(object, (*array)[i])) {
                *index = i;
                return NOERROR;
            }
        }
    }
    else {
        for (Int32 i = 0; i < size; i++) {
            if ((*array)[i] == NULL) {
                *index = i;
                return NOERROR;
            }
        }
    }
    *index = -1;
    return NOERROR;
}

ECode CCollections::SynchronizedList::LastIndexOf(
    /* [in] */ IInterface* object,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index)
    Int32 size;
    AutoPtr<ArrayOf<IInterface*> > array;
    {
        Mutex::Autolock lock(mMutex);
        mList->GetSize(&size);
        array = ArrayOf<IInterface*>::Alloc(size);
        mList->ToArray((ArrayOf<IInterface*>**)&array);
    }
    if (object != NULL) {
        for (Int32 i = size - 1; i >= 0; i--) {
            if (ObjectUtils::Equals(object, (*array)[i])) {
                *index = i;
                return NOERROR;
            }
        }
    }
    else {
        for (Int32 i = size - 1; i >= 0; i--) {
            if ((*array)[i] == NULL) {
                *index = i;
                return NOERROR;
            }
        }
    }
    *index = -1;
    return NOERROR;
}

ECode CCollections::SynchronizedList::GetListIterator(
    /* [out] */ IListIterator** it)
{
    VALIDATE_NOT_NULL(it)
    Mutex::Autolock lock(mMutex);
    return mList->GetListIterator(it);
}

ECode CCollections::SynchronizedList::GetListIterator(
    /* [in] */ Int32 location,
    /* [out] */ IListIterator** it)
{
    VALIDATE_NOT_NULL(it)
    Mutex::Autolock lock(mMutex);
    return mList->GetListIterator(location, it);
}

ECode CCollections::SynchronizedList::Remove(
    /* [in] */ Int32 location,
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object)
    Mutex::Autolock lock(mMutex);
    return mList->Remove(location, object);
}

ECode CCollections::SynchronizedList::Set(
    /* [in] */ Int32 location,
    /* [in] */ IInterface* object,
    /* [out] */ IInterface** prevObject)
{
    VALIDATE_NOT_NULL(prevObject)
    Mutex::Autolock lock(mMutex);
    return mList->Set(location, object, prevObject);
}

ECode CCollections::SynchronizedList::SubList(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ IList** subList)
{
    Mutex::Autolock lock(mMutex);
    AutoPtr<IList> sub;
    mList->SubList(start, end, (IList**)&sub);
    AutoPtr<IList> res = new SynchronizedList(sub, mMutex);
    *subList = res;
    REFCOUNT_ADD(*subList)
    return NOERROR;
}

ECode CCollections::SynchronizedList::WriteObject(
    /* [in] */ IObjectOutputStream* stream)
{
    Mutex::Autolock lock(mMutex);
    return stream->DefaultWriteObject();
}

ECode CCollections::SynchronizedList::Add(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return SynchronizedCollection::Add(object, modified);
}

ECode CCollections::SynchronizedList::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return SynchronizedCollection::AddAll(collection, modified);
}

ECode CCollections::SynchronizedList::Clear()
{
    return SynchronizedCollection::Clear();
}

ECode CCollections::SynchronizedList::Contains(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return SynchronizedCollection::Contains(object, result);
}

ECode CCollections::SynchronizedList::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return SynchronizedCollection::ContainsAll(collection, result);
}

ECode CCollections::SynchronizedList::IsEmpty(
    /* [out] */ Boolean* result)
{
    return SynchronizedCollection::IsEmpty(result);
}

ECode CCollections::SynchronizedList::GetIterator(
    /* [out] */ IIterator** it)
{
    return SynchronizedCollection::GetIterator(it);
}

ECode CCollections::SynchronizedList::Remove(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return SynchronizedCollection::Remove(object, modified);
}

ECode CCollections::SynchronizedList::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return SynchronizedCollection::RemoveAll(collection, modified);
}

ECode CCollections::SynchronizedList::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return SynchronizedCollection::RetainAll(collection, modified);
}

ECode CCollections::SynchronizedList::GetSize(
    /* [out] */ Int32* size)
{
    return SynchronizedCollection::GetSize(size);
}

ECode CCollections::SynchronizedList::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    return SynchronizedCollection::ToArray(array);
}

ECode CCollections::SynchronizedList::ToArray(
    /* [in] */ ArrayOf<IInterface*>* inArray,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    return SynchronizedCollection::ToArray(inArray, outArray);
}

AutoPtr<IInterface> CCollections::SynchronizedList::ReadResolve()
{
    if (IRandomAccess::Probe(mList) != NULL) {
        AutoPtr<SynchronizedRandomAccessList> p = new SynchronizedRandomAccessList(mList, mMutex);
        return (IInterface*)p->Probe(EIID_IInterface);
    }
    return (IInterface*)this->Probe(EIID_IInterface);
}

//====================================================================
// CCollections::SynchronizedMap::
//====================================================================

UInt32 CCollections::SynchronizedMap::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CCollections::SynchronizedMap::Release()
{
    return ElRefBase::Release();
}

PInterface CCollections::SynchronizedMap::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_IInterface == riid) {
        return (PInterface)(IMap*)this;
    }
    else if (EIID_IIterable == riid) {
        return (IIterable*)(IMap*)this;
    }
    else if (EIID_ICollection == riid) {
        return (ICollection*)(IMap*)this;
    }
    else if (EIID_IMap == riid) {
        return (IMap*)this;
    }
    else if (Elastos::IO::EIID_ISerializable == riid) {
        return (ISerializable*)this;
    }

    return NULL;
}

ECode CCollections::SynchronizedMap::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID)

    if (pObject == (IInterface*)(IMap*)this) {
        *pIID = EIID_IMap;
    }
    else if (pObject == (IInterface*)(ISerializable*)this) {
        *pIID = Elastos::IO::EIID_ISerializable;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

CCollections::SynchronizedMap::SynchronizedMap(
    /* [in] */ IMap* map)
{
    mM = map;
    mMutex = new Mutex;
}

CCollections::SynchronizedMap::SynchronizedMap(
    /* [in] */ IMap* map,
    /* [in] */ Mutex* mutex)
{
    mM = map;
    mMutex = mutex;
}

ECode CCollections::SynchronizedMap::Clear()
{
    Mutex::Autolock lock(mMutex);
    return mM->Clear();
}

ECode CCollections::SynchronizedMap::ContainsKey(
    /* [in] */ IInterface* key,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    Mutex::Autolock lock(mMutex);
    return mM->ContainsKey(key, result);
}

ECode CCollections::SynchronizedMap::ContainsValue(
    /* [in] */ IInterface* value,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    Mutex::Autolock lock(mMutex);
    return mM->ContainsValue(value, result);
}

ECode CCollections::SynchronizedMap::EntrySet(
    /* [out, callee] */ ISet** entries)
{
    VALIDATE_NOT_NULL(*entries)
    Mutex::Autolock lock(mMutex);
    AutoPtr<ISet> entry;
    mM->EntrySet((ISet**)&entry);
    AutoPtr<ISet> res = new SynchronizedSet(entry, mMutex);
    *entries = res;
    REFCOUNT_ADD(*entries)
    return NOERROR;
}

ECode CCollections::SynchronizedMap::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    Mutex::Autolock lock(mMutex);
    return mM->Equals(object, result);
}

ECode CCollections::SynchronizedMap::Get(
    /* [in] */ PInterface key,
    /* [out] */ PInterface* value)
{
    VALIDATE_NOT_NULL(value)
    Mutex::Autolock lock(mMutex);
    return mM->Get(key, value);
}

ECode CCollections::SynchronizedMap::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode)
    Mutex::Autolock lock(mMutex);
    return mM->GetHashCode(hashCode);
}

ECode CCollections::SynchronizedMap::IsEmpty(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    Mutex::Autolock lock(mMutex);
    return mM->IsEmpty(result);
}

ECode CCollections::SynchronizedMap::KeySet(
    /* [out, callee] */ ISet** keySet)
{
    VALIDATE_NOT_NULL(keySet)
    Mutex::Autolock lock(mMutex);
    AutoPtr<ISet> key;
    mM->KeySet((ISet**)&key);
    *keySet = new SynchronizedSet(key, mMutex);
    REFCOUNT_ADD(*keySet)
    return NOERROR;
}

ECode CCollections::SynchronizedMap::Put(
    /* [in] */ PInterface key,
    /* [in] */ PInterface value,
    /* [out] */ PInterface* oldValue)
{
    Mutex::Autolock lock(mMutex);
    return mM->Put(key, value, oldValue);
}

ECode CCollections::SynchronizedMap::PutAll(
    /* [in] */ IMap* map)
{
    Mutex::Autolock lock(mMutex);
    return mM->PutAll(map);
}

ECode CCollections::SynchronizedMap::Remove(
    /* [in] */ PInterface key,
    /* [out] */ PInterface* value)
{
    VALIDATE_NOT_NULL(value)
    Mutex::Autolock lock(mMutex);
    return mM->Remove(key, value);
}

ECode CCollections::SynchronizedMap::GetSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size)
    Mutex::Autolock lock(mMutex);
    return mM->GetSize(size);
}

ECode CCollections::SynchronizedMap::Values(
    /* [out] */ ICollection** value)
{
    VALIDATE_NOT_NULL(value)
    Mutex::Autolock lock(mMutex);
    AutoPtr<ICollection> v;
    mM->Values((ICollection**)&v);
    AutoPtr<ICollection> res = new SynchronizedCollection(v, mMutex);
    *value = res;
    REFCOUNT_ADD(*value)
    return NOERROR;
}

ECode CCollections::SynchronizedMap::ToString(
    /* [out] */ String* str)
{
    Mutex::Autolock lock(mMutex);
    AutoPtr<IObject> iObj = IObject::Probe(mM);
    if (iObj != NULL) {
        return iObj->ToString(str);
    }
    *str = NULL;
    return NOERROR;
}

ECode CCollections::SynchronizedMap::WriteObject(
    /* [in] */ IObjectOutputStream* stream)
{
    Mutex::Autolock lock(mMutex);
    return stream->DefaultWriteObject();
}

//====================================================================
// CCollections::SynchronizedSet::
//====================================================================

UInt32 CCollections::SynchronizedSet::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CCollections::SynchronizedSet::Release()
{
    return ElRefBase::Release();
}

PInterface CCollections::SynchronizedSet::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_IInterface == riid) {
        return (PInterface)(ISet*)this;
    }
    else if (EIID_IIterable == riid) {
        return (IIterable*)(ISet*)this;
    }
    else if (EIID_ICollection == riid) {
        return (ICollection*)(ISet*)this;
    }
    else if (EIID_ISet == riid) {
        return (ISet*)this;
    }
    else if (Elastos::IO::EIID_ISerializable == riid) {
        return (ISerializable*)this;
    }

    return NULL;
}

ECode CCollections::SynchronizedSet::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID)

    if (pObject == (IInterface*)(ISet*)this) {
        *pIID = EIID_ISet;
    }
    else if (pObject == (IInterface*)(ISerializable*)this) {
        *pIID = Elastos::IO::EIID_ISerializable;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

CCollections::SynchronizedSet::SynchronizedSet(
    /* [in] */ ISet* set)
    : SynchronizedCollection(set)
{
}

CCollections::SynchronizedSet::SynchronizedSet(
    /* [in] */ ISet* set,
    /* [in] */ Mutex* mutex)
    : SynchronizedCollection(set, mutex)
{
}

ECode CCollections::SynchronizedSet::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    Mutex::Autolock lock(mMutex);
    return mC->Equals(object, result);
}

ECode CCollections::SynchronizedSet::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode)
    Mutex::Autolock lock(mMutex);
    return mC->GetHashCode(hashCode);
}

ECode CCollections::SynchronizedSet::GetIterator(
    /* [out] */ IIterator** it)
{
    return SynchronizedCollection::GetIterator(it);
}

ECode CCollections::SynchronizedSet::Add(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return SynchronizedCollection::Add(object, modified);
}

ECode CCollections::SynchronizedSet::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return SynchronizedCollection::AddAll(collection, modified);
}

ECode CCollections::SynchronizedSet::Clear()
{
    return SynchronizedCollection::Clear();
}

ECode CCollections::SynchronizedSet::Contains(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return SynchronizedCollection::Contains(object, result);
}

ECode CCollections::SynchronizedSet::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return SynchronizedCollection::ContainsAll(collection, result);
}

ECode CCollections::SynchronizedSet::IsEmpty(
    /* [out] */ Boolean* result)
{
    return SynchronizedCollection::IsEmpty(result);
}

ECode CCollections::SynchronizedSet::Remove(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return SynchronizedCollection::Remove(object, modified);
}

ECode CCollections::SynchronizedSet::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return SynchronizedCollection::RemoveAll(collection, modified);
}

ECode CCollections::SynchronizedSet::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return SynchronizedCollection::RetainAll(collection, modified);
}

ECode CCollections::SynchronizedSet::GetSize(
    /* [out] */ Int32* size)
{
    return SynchronizedCollection::GetSize(size);
}

ECode CCollections::SynchronizedSet::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    return SynchronizedCollection::ToArray(array);
}

ECode CCollections::SynchronizedSet::ToArray(
    /* [in] */ ArrayOf<IInterface*>* inArray,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    return SynchronizedCollection::ToArray(inArray, outArray);
}

ECode CCollections::SynchronizedSet::WriteObject(
    /* [in] */ IObjectOutputStream* stream)
{
    Mutex::Autolock lock(mMutex);
    return stream->DefaultWriteObject();
}

//====================================================================
// CCollections::SynchronizedSortedMap::
//====================================================================

UInt32 CCollections::SynchronizedSortedMap::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CCollections::SynchronizedSortedMap::Release()
{
    return ElRefBase::Release();
}

PInterface CCollections::SynchronizedSortedMap::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_ISortedMap == riid) {
        return (ISortedMap*)this;
    }
    return SynchronizedMap::Probe(riid);
}

ECode CCollections::SynchronizedSortedMap::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID)

    if (pObject == (IInterface*)(ISortedMap*)this) {
        *pIID = EIID_ISortedMap;
    }

    return SynchronizedMap::GetInterfaceID(pObject, pIID);
}

CCollections::SynchronizedSortedMap::SynchronizedSortedMap(
    /* [in] */ ISortedMap* map)
    : SynchronizedMap(map)
{
    mSm = map;
}

CCollections::SynchronizedSortedMap::SynchronizedSortedMap(
    /* [in] */ ISortedMap* map,
    /* [in] */ Mutex* mutex)
    : SynchronizedMap(map, mutex)
{
    mSm = map;
}

ECode CCollections::SynchronizedSortedMap::Comparator(
    /* [out] */ IComparator** comp)
{
    VALIDATE_NOT_NULL(comp)
    Mutex::Autolock lock(mMutex);
    return mSm->Comparator(comp);
}

ECode CCollections::SynchronizedSortedMap::FirstKey(
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface)
    Mutex::Autolock lock(mMutex);
    return mSm->FirstKey(outface);
}

ECode CCollections::SynchronizedSortedMap::HeadMap(
    /* [in] */ IInterface* endKey,
    /* [out] */ ISortedMap** sortmap)
{
    VALIDATE_NOT_NULL(sortmap)
    Mutex::Autolock lock(mMutex);
    AutoPtr<ISortedMap> map;
    mSm->HeadMap(endKey, (ISortedMap**)&map);
    AutoPtr<ISortedMap> res = new SynchronizedSortedMap(map, mMutex);
    *sortmap = res;
    REFCOUNT_ADD(*sortmap)
    return NOERROR;
}

ECode CCollections::SynchronizedSortedMap::LastKey(
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface)
    Mutex::Autolock lock(mMutex);
    return mSm->LastKey(outface);
}

ECode CCollections::SynchronizedSortedMap::SubMap(
    /* [in] */ IInterface* startKey,
    /* [in] */ IInterface* endKey,
    /* [out] */ ISortedMap** sortmap)
{
    VALIDATE_NOT_NULL(sortmap)
    Mutex::Autolock lock(mMutex);
    AutoPtr<ISortedMap> map;
    mSm->SubMap(startKey, endKey, (ISortedMap**)&map);
    AutoPtr<ISortedMap> res = new SynchronizedSortedMap(map, mMutex);
    *sortmap = res;
    REFCOUNT_ADD(*sortmap)
    return NOERROR;
}

ECode CCollections::SynchronizedSortedMap::TailMap(
    /* [in] */ IInterface* startKey,
    /* [out] */ ISortedMap** sortmap)
{
    VALIDATE_NOT_NULL(sortmap)
    Mutex::Autolock lock(mMutex);
    AutoPtr<ISortedMap> map;
    mSm->TailMap(startKey, (ISortedMap**)&map);
    AutoPtr<ISortedMap> res = new SynchronizedSortedMap(map, mMutex);
    *sortmap = res;
    REFCOUNT_ADD(*sortmap)
    return NOERROR;
}

ECode CCollections::SynchronizedSortedMap::Clear()
{
    return SynchronizedMap::Clear();
}

ECode CCollections::SynchronizedSortedMap::ContainsKey(
    /* [in] */ IInterface* key,
    /* [out] */ Boolean* result)
{
    return SynchronizedMap::ContainsKey(key, result);
}

ECode CCollections::SynchronizedSortedMap::ContainsValue(
    /* [in] */ IInterface* value,
    /* [out] */ Boolean* result)
{
    return SynchronizedMap::ContainsValue(value, result);
}

ECode CCollections::SynchronizedSortedMap::EntrySet(
    /* [out, callee] */ ISet** entries)
{
    return SynchronizedMap::EntrySet(entries);
}

ECode CCollections::SynchronizedSortedMap::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return SynchronizedMap::Equals(object, result);
}

ECode CCollections::SynchronizedSortedMap::Get(
    /* [in] */ PInterface key,
    /* [out] */ PInterface* value)
{
    return SynchronizedMap::Get(key, value);
}

ECode CCollections::SynchronizedSortedMap::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    return SynchronizedMap::GetHashCode(hashCode);
}

ECode CCollections::SynchronizedSortedMap::IsEmpty(
    /* [out] */ Boolean* result)
{
    return SynchronizedMap::IsEmpty(result);
}

ECode CCollections::SynchronizedSortedMap::KeySet(
    /* [out, callee] */ ISet** keySet)
{
    return SynchronizedMap::KeySet(keySet);
}

ECode CCollections::SynchronizedSortedMap::Values(
    /* [out] */ ICollection** value)
{
    return SynchronizedMap::Values(value);
}

ECode CCollections::SynchronizedSortedMap::Put(
    /* [in] */ PInterface key,
    /* [in] */ PInterface value,
    /* [out] */ PInterface* oldValue)
{
    return SynchronizedMap::Put(key, value, oldValue);
}

ECode CCollections::SynchronizedSortedMap::PutAll(
    /* [in] */ IMap* map)
{
    return SynchronizedMap::PutAll(map);
}

ECode CCollections::SynchronizedSortedMap::Remove(
    /* [in] */ PInterface key,
    /* [out] */ PInterface* value)
{
    return SynchronizedMap::Remove(key, value);
}

ECode CCollections::SynchronizedSortedMap::GetSize(
    /* [out] */ Int32* size)
{
    return SynchronizedMap::GetSize(size);
}

ECode CCollections::SynchronizedSortedMap::WriteObject(
    /* [in] */ IObjectOutputStream* stream)
{
    Mutex::Autolock lock(mMutex);
    return stream->DefaultWriteObject();
}

//====================================================================
// CCollections::SynchronizedSortedSet::
//====================================================================

UInt32 CCollections::SynchronizedSortedSet::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CCollections::SynchronizedSortedSet::Release()
{
    return ElRefBase::Release();
}

PInterface CCollections::SynchronizedSortedSet::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_ISortedSet == riid) {
        return (ISortedSet*)this;
    }
    return SynchronizedSet::Probe(riid);
}

ECode CCollections::SynchronizedSortedSet::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID)

    if (pObject == (IInterface*)(ISortedSet*)this) {
        *pIID = EIID_ISortedSet;
    }

    return SynchronizedSet::GetInterfaceID(pObject, pIID);
}

CCollections::SynchronizedSortedSet::SynchronizedSortedSet(
    /* [in] */ ISortedSet* set)
    : SynchronizedSet(set)
{
    mSs = set;
}

CCollections::SynchronizedSortedSet::SynchronizedSortedSet(
    /* [in] */ ISortedSet* set,
    /* [in] */ Mutex* mutex)
    : SynchronizedSet(set, mutex)
{
    mSs = set;
}

ECode CCollections::SynchronizedSortedSet::Comparator(
    /* [out] */ IComparator** outcom)
{
    VALIDATE_NOT_NULL(outcom)
    Mutex::Autolock lock(mMutex);
    return mSs->Comparator(outcom);
}

ECode CCollections::SynchronizedSortedSet::First(
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface)
    Mutex::Autolock lock(mMutex);
    return mSs->First(outface);
}

ECode CCollections::SynchronizedSortedSet::HeadSet(
    /* [in] */ IInterface* end,
    /* [out] */ ISortedSet** outsort)
{
    VALIDATE_NOT_NULL(outsort)
    Mutex::Autolock lock(mMutex);
    AutoPtr<ISortedSet> sort;
    mSs->HeadSet(end, (ISortedSet**)&sort);
    AutoPtr<ISortedSet> res = new SynchronizedSortedSet(sort, mMutex);
    *outsort = res;
    REFCOUNT_ADD(*outsort)
    return NOERROR;
}

ECode CCollections::SynchronizedSortedSet::Last(
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface)
    Mutex::Autolock lock(mMutex);
    return mSs->Last(outface);
}

ECode CCollections::SynchronizedSortedSet::SubSet(
    /* [in] */ IInterface* start,
    /* [in] */ IInterface* end,
    /* [out] */ ISortedSet** outsort)
{
    VALIDATE_NOT_NULL(outsort)
    Mutex::Autolock lock(mMutex);
    AutoPtr<ISortedSet> set;
    mSs->SubSet(start, end, (ISortedSet**)&set);
    AutoPtr<ISortedSet> res = new SynchronizedSortedSet(set, mMutex);
    *outsort = res;
    REFCOUNT_ADD(*outsort)
    return NOERROR;
}

ECode CCollections::SynchronizedSortedSet::TailSet(
    /* [in] */ IInterface* start,
    /* [out] */ ISortedSet** outsort)
{
    Mutex::Autolock lock(mMutex);
    AutoPtr<ISortedSet> set;
    mSs->TailSet(start, (ISortedSet**)&set);
    AutoPtr<ISortedSet> res = new SynchronizedSortedSet(set, mMutex);
    *outsort = res;
    REFCOUNT_ADD(*outsort)
    return NOERROR;
}

ECode CCollections::SynchronizedSortedSet::Add(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return SynchronizedSet::Add(object, modified);
}

ECode CCollections::SynchronizedSortedSet::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return SynchronizedSet::AddAll(collection, modified);
}

ECode CCollections::SynchronizedSortedSet::Clear()
{
    return SynchronizedSet::Clear();
}

ECode CCollections::SynchronizedSortedSet::Contains(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return SynchronizedSet::Contains(object, result);
}

ECode CCollections::SynchronizedSortedSet::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return SynchronizedSet::ContainsAll(collection, result);
}

ECode CCollections::SynchronizedSortedSet::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return SynchronizedSet::Equals(object, result);
}

ECode CCollections::SynchronizedSortedSet::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    return SynchronizedSet::GetHashCode(hashCode);
}

ECode CCollections::SynchronizedSortedSet::GetIterator(
    /* [out] */ IIterator** it)
{
    return SynchronizedSet::GetIterator(it);
}

ECode CCollections::SynchronizedSortedSet::IsEmpty(
    /* [out] */ Boolean* result)
{
    return SynchronizedSet::IsEmpty(result);
}

ECode CCollections::SynchronizedSortedSet::Remove(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return SynchronizedSet::Remove(object, modified);
}

ECode CCollections::SynchronizedSortedSet::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return SynchronizedSet::RemoveAll(collection, modified);
}

ECode CCollections::SynchronizedSortedSet::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return SynchronizedSet::RetainAll(collection, modified);
}

ECode CCollections::SynchronizedSortedSet::GetSize(
    /* [out] */ Int32* size)
{
    return SynchronizedSet::GetSize(size);
}

ECode CCollections::SynchronizedSortedSet::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    return SynchronizedSet::ToArray(array);
}

ECode CCollections::SynchronizedSortedSet::ToArray(
    /* [in] */ ArrayOf<IInterface*>* inArray,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    return SynchronizedSet::ToArray(inArray, outArray);
}

ECode CCollections::SynchronizedSortedSet::WriteObject(
    /* [in] */ IObjectOutputStream* stream)
{
    Mutex::Autolock lock(mMutex);
    return stream->DefaultWriteObject();
}

//====================================================================
// CCollections::UnmodifiableCollection::Iterator::
//====================================================================
CAR_INTERFACE_IMPL(CCollections::UnmodifiableCollection::Iterator, IIterator)

CCollections::UnmodifiableCollection::Iterator::Iterator(
    /* [in] */ UnmodifiableCollection* owner)
{
    owner->mC->GetIterator((IIterator**)&mIterator);
}

ECode CCollections::UnmodifiableCollection::Iterator::HasNext(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return mIterator->HasNext(result);
}

ECode CCollections::UnmodifiableCollection::Iterator::Next(
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object)
    return mIterator->Next(object);
}

ECode CCollections::UnmodifiableCollection::Iterator::Remove()
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

//====================================================================
// CCollections::UnmodifiableCollection::
//====================================================================

UInt32 CCollections::UnmodifiableCollection::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CCollections::UnmodifiableCollection::Release()
{
    return ElRefBase::Release();
}

PInterface CCollections::UnmodifiableCollection::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_IInterface == riid) {
        return (PInterface)(ICollection*)this;
    }
    else if (EIID_IIterable == riid) {
        return (IIterable*)(ICollection*)this;
    }
    else if (EIID_ICollection == riid) {
        return (ICollection*)this;
    }
    else if (Elastos::IO::EIID_ISerializable == riid) {
        return (ISerializable*)this;
    }

    return NULL;
}

ECode CCollections::UnmodifiableCollection::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID)

    if (pObject == (IInterface*)(ICollection*)this) {
        *pIID = EIID_ICollection;
    }
    else if (pObject == (IInterface*)(ISerializable*)this) {
        *pIID = Elastos::IO::EIID_ISerializable;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

CCollections::UnmodifiableCollection::UnmodifiableCollection(
    /* [in] */ ICollection* collection)
{
    mC = collection;
}

ECode CCollections::UnmodifiableCollection::Add(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode CCollections::UnmodifiableCollection::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode CCollections::UnmodifiableCollection::Clear()
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode CCollections::UnmodifiableCollection::Contains(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return mC->Contains(object, result);
}

ECode CCollections::UnmodifiableCollection::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return mC->ContainsAll(collection, result);
}

ECode CCollections::UnmodifiableCollection::IsEmpty(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return mC->IsEmpty(result);
}

ECode CCollections::UnmodifiableCollection::GetIterator(
    /* [out] */ IIterator** it)
{
    VALIDATE_NOT_NULL(it)
    AutoPtr<IIterator> res = new Iterator(this);
    *it = res;
    REFCOUNT_ADD(*it)
    return NOERROR;
}

ECode CCollections::UnmodifiableCollection::Remove(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode CCollections::UnmodifiableCollection::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode CCollections::UnmodifiableCollection::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode CCollections::UnmodifiableCollection::GetSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size)
    return mC->GetSize(size);
}

ECode CCollections::UnmodifiableCollection::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    VALIDATE_NOT_NULL(array)
    return mC->ToArray(array);
}

ECode CCollections::UnmodifiableCollection::ToArray(
    /* [in] */ ArrayOf<IInterface*>* inArray,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    VALIDATE_NOT_NULL(outArray)
    return mC->ToArray(inArray, outArray);
}

ECode CCollections::UnmodifiableCollection::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    AutoPtr<IObject> iObj = IObject::Probe(mC);
    if (iObj != NULL) {
        return iObj->ToString(str);
    }
    *str = NULL;
    return NOERROR;
}

ECode CCollections::UnmodifiableCollection::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = this->Probe(EIID_IInterface) == object;
    return NOERROR;
}

ECode CCollections::UnmodifiableCollection::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode);
    *hashCode = (Int32)this;
    return NOERROR;
}

//====================================================================
// CCollections::UnmodifiableRandomAccessList::
//====================================================================

UInt32 CCollections::UnmodifiableRandomAccessList::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CCollections::UnmodifiableRandomAccessList::Release()
{
    return ElRefBase::Release();
}

PInterface CCollections::UnmodifiableRandomAccessList::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_IRandomAccess == riid) {
        return (IRandomAccess*)this;
    }
    return UnmodifiableList::Probe(riid);
}

ECode CCollections::UnmodifiableRandomAccessList::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID)

    if (pObject == (IInterface*)(IRandomAccess*)this) {
        *pIID = EIID_IRandomAccess;
    }
    return UnmodifiableList::GetInterfaceID(pObject, pIID);
}

CCollections::UnmodifiableRandomAccessList::UnmodifiableRandomAccessList(
    /* [in] */ IList* l)
    : UnmodifiableList(l)
{
}

ECode CCollections::UnmodifiableRandomAccessList::SubList(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ IList** subList)
{
    VALIDATE_NOT_NULL(subList)
    AutoPtr<IList> l;
    mList->SubList(start, end, (IList**)&l);
    AutoPtr<IList> res = new UnmodifiableRandomAccessList(l);
    *subList = res;
    REFCOUNT_ADD(*subList)
    return NOERROR;
}

AutoPtr<IInterface> CCollections::UnmodifiableRandomAccessList::WriteReplace()
{
    AutoPtr<UnmodifiableList> res = new UnmodifiableList(mList);
    return res->Probe(EIID_IInterface);
}

//====================================================================
// CCollections::UnmodifiableList::ListIterator::
//====================================================================

UInt32 CCollections::UnmodifiableList::ListIterator::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CCollections::UnmodifiableList::ListIterator::Release()
{
    return ElRefBase::Release();
}

PInterface CCollections::UnmodifiableList::ListIterator::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_IInterface == riid) {
        return (PInterface)(IList*)this;
    }
    else if (EIID_IIterable == riid) {
        return (IIterable*)(IList*)this;
    }
    else if (EIID_IListIterator == riid) {
        return (IListIterator*)this;
    }

    return NULL;
}

ECode CCollections::UnmodifiableList::ListIterator::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID)

    if (pObject == (IInterface*)(IListIterator*)this) {
        *pIID = EIID_IListIterator;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

CCollections::UnmodifiableList::ListIterator::ListIterator(
    /* [in] */ UnmodifiableList* owner,
    /* [in] */ Int32 location)
{
    owner->mList->GetListIterator(location, (IListIterator**)&mIterator);
}

ECode CCollections::UnmodifiableList::ListIterator::Add(
    /* [in] */ IInterface* object)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode CCollections::UnmodifiableList::ListIterator::HasNext(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return mIterator->HasNext(result);
}

ECode CCollections::UnmodifiableList::ListIterator::HasPrevious(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return mIterator->HasPrevious(result);
}

ECode CCollections::UnmodifiableList::ListIterator::Next(
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object)
    return mIterator->Next(object);
}

ECode CCollections::UnmodifiableList::ListIterator::NextIndex(
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index)
    return mIterator->NextIndex(index);
}

ECode CCollections::UnmodifiableList::ListIterator::Previous(
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object)
    return mIterator->Previous(object);
}

ECode CCollections::UnmodifiableList::ListIterator::PreviousIndex(
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index)
    return mIterator->PreviousIndex(index);
}

ECode CCollections::UnmodifiableList::ListIterator::Remove()
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode CCollections::UnmodifiableList::ListIterator::Set(
    /* [in] */ IInterface* object)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

//====================================================================
// CCollections::UnmodifiableList::
//====================================================================

UInt32 CCollections::UnmodifiableList::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CCollections::UnmodifiableList::Release()
{
    return ElRefBase::Release();
}

PInterface CCollections::UnmodifiableList::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_IInterface == riid) {
        return (PInterface)(IList*)this;
    }
    else if (EIID_IIterable == riid) {
        return (IIterable*)(IList*)this;
    }
    else if (EIID_ICollection == riid) {
        return (ICollection*)(IList*)this;
    }
    else if (EIID_IList == riid) {
        return (IList*)this;
    }
    else if (Elastos::IO::EIID_ISerializable == riid) {
        return (ISerializable*)this;
    }

    return NULL;
}

ECode CCollections::UnmodifiableList::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID)

    if (pObject == (IInterface*)(IList*)this) {
        *pIID = EIID_IList;
    }
    else if (pObject == (IInterface*)(ISerializable*)this) {
        *pIID = Elastos::IO::EIID_ISerializable;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

CCollections::UnmodifiableList::UnmodifiableList(
    /* [in] */ IList* l)
    : UnmodifiableCollection(l)
{
    mList = l;
}

ECode CCollections::UnmodifiableList::Add(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return UnmodifiableCollection::Add(object, modified);
}

ECode CCollections::UnmodifiableList::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return UnmodifiableCollection::AddAll(collection, modified);
}

ECode CCollections::UnmodifiableList::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return mList->Equals(object, result);
}

ECode CCollections::UnmodifiableList::Get(
    /* [in] */ Int32 location,
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object)
    return mList->Get(location, object);
}

ECode CCollections::UnmodifiableList::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode)
    return mList->GetHashCode(hashCode);
}

ECode CCollections::UnmodifiableList::IndexOf(
    /* [in] */ IInterface* object,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index)
    return mList->IndexOf(object, index);
}

ECode CCollections::UnmodifiableList::LastIndexOf(
    /* [in] */ IInterface* object,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index)
    return mList->LastIndexOf(object, index);
}

ECode CCollections::UnmodifiableList::GetListIterator(
    /* [out] */ IListIterator** it)
{
    VALIDATE_NOT_NULL(it)
    return GetListIterator(0, it);
}

ECode CCollections::UnmodifiableList::GetListIterator(
    /* [in] */ Int32 location,
    /* [out] */ IListIterator** it)
{
    VALIDATE_NOT_NULL(it)
    AutoPtr<IListIterator> res = new ListIterator(this, location);
    *it = res;
    REFCOUNT_ADD(*it)
    return NOERROR;
}

ECode CCollections::UnmodifiableList::Remove(
    /* [in] */ Int32 location,
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object)
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode CCollections::UnmodifiableList::Set(
    /* [in] */ Int32 location,
    /* [in] */ IInterface* object,
    /* [out] */ IInterface** prevObject)
{
    VALIDATE_NOT_NULL(prevObject)
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode CCollections::UnmodifiableList::SubList(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ IList** subList)
{
    VALIDATE_NOT_NULL(subList)
    AutoPtr<IList> sub;
    mList->SubList(start, end, (IList**)&sub);
    AutoPtr<IList> res = new UnmodifiableList(sub);
    *subList = res;
    REFCOUNT_ADD(*subList)
    return NOERROR;
}

ECode CCollections::UnmodifiableList::Clear()
{
    return UnmodifiableCollection::Clear();
}

ECode CCollections::UnmodifiableList::Contains(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return UnmodifiableCollection::Contains(object, result);
}

ECode CCollections::UnmodifiableList::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return UnmodifiableCollection::ContainsAll(collection, result);
}

ECode CCollections::UnmodifiableList::IsEmpty(
    /* [out] */ Boolean* result)
{
    return UnmodifiableCollection::IsEmpty(result);
}

ECode CCollections::UnmodifiableList::GetIterator(
    /* [out] */ IIterator** it)
{
    return UnmodifiableCollection::GetIterator(it);
}

ECode CCollections::UnmodifiableList::Remove(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return UnmodifiableCollection::Remove(object, modified);
}

ECode CCollections::UnmodifiableList::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return UnmodifiableCollection::RemoveAll(collection, modified);
}

ECode CCollections::UnmodifiableList::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return UnmodifiableCollection::RetainAll(collection, modified);
}

ECode CCollections::UnmodifiableList::GetSize(
    /* [out] */ Int32* size)
{
    return UnmodifiableCollection::GetSize(size);
}

ECode CCollections::UnmodifiableList::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    return UnmodifiableCollection::ToArray(array);
}

ECode CCollections::UnmodifiableList::ToArray(
    /* [in] */ ArrayOf<IInterface*>* inArray,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    return UnmodifiableCollection::ToArray(inArray, outArray);
}

ECode CCollections::UnmodifiableList::Add(
    /* [in] */ Int32 location,
    /* [in] */ IInterface* object)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode CCollections::UnmodifiableList::AddAll(
    /* [in] */ Int32 location,
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

AutoPtr<IInterface> CCollections::UnmodifiableList::ReadResolve()
{
    if (IRandomAccess::Probe(mList) != NULL ) {
        AutoPtr<UnmodifiableRandomAccessList> res = new UnmodifiableRandomAccessList(mList);
        return res->Probe(EIID_IInterface);
    }
    return this->Probe(EIID_IInterface);
}

//====================================================================
// CCollections::UnmodifiableMap::UnmodifiableEntrySet::UnmodifiableMapEntry::
//====================================================================
CAR_INTERFACE_IMPL(CCollections::UnmodifiableMap::UnmodifiableEntrySet::UnmodifiableMapEntry, IMapEntry)

CCollections::UnmodifiableMap::UnmodifiableEntrySet::UnmodifiableMapEntry::UnmodifiableMapEntry(
    /* [in] */ IMapEntry* entry)
{
    mMapEntry = entry;
}

ECode CCollections::UnmodifiableMap::UnmodifiableEntrySet::UnmodifiableMapEntry::Equals(
    /* [in] */ IInterface* entry,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return mMapEntry->Equals(entry, result);
}

ECode CCollections::UnmodifiableMap::UnmodifiableEntrySet::UnmodifiableMapEntry::GetKey(
    /* [out] */ PInterface* key)
{
    VALIDATE_NOT_NULL(key)
    return mMapEntry->GetKey(key);
}

ECode CCollections::UnmodifiableMap::UnmodifiableEntrySet::UnmodifiableMapEntry::GetValue(
    /* [out] */ PInterface* value)
{
    VALIDATE_NOT_NULL(value)
    return mMapEntry->GetValue(value);
}

ECode CCollections::UnmodifiableMap::UnmodifiableEntrySet::UnmodifiableMapEntry::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode)
    return mMapEntry->GetHashCode(hashCode);
}

ECode CCollections::UnmodifiableMap::UnmodifiableEntrySet::UnmodifiableMapEntry::SetValue(
    /* [in] */ IInterface* valueReplacer,
    /* [out] */ IInterface** valueReplacee)
{
    VALIDATE_NOT_NULL(valueReplacee)
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode CCollections::UnmodifiableMap::UnmodifiableEntrySet::UnmodifiableMapEntry::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    AutoPtr<IObject> iObj = IObject::Probe(mMapEntry);
    if (iObj != NULL) {
        return iObj->ToString(str);
    }
    *str = NULL;
    return NOERROR;
}

//====================================================================
// CCollections::UnmodifiableMap::UnmodifiableEntrySet::Iterator::
//====================================================================
CAR_INTERFACE_IMPL(CCollections::UnmodifiableMap::UnmodifiableEntrySet::Iterator, IIterator)

CCollections::UnmodifiableMap::UnmodifiableEntrySet::Iterator::Iterator(
    /* [in] */ UnmodifiableEntrySet* owner)
{
    owner->mC->GetIterator((IIterator**)&mIterator);
}

ECode CCollections::UnmodifiableMap::UnmodifiableEntrySet::Iterator::HasNext(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return mIterator->HasNext(result);
}

ECode CCollections::UnmodifiableMap::UnmodifiableEntrySet::Iterator::Next(
    /* [out] */ IMapEntry** object)
{
    AutoPtr<IMapEntry> o;
    mIterator->Next((IMapEntry**)&o);
    AutoPtr<IMapEntry> res = new UnmodifiableMapEntry(o);
    *object = res;
    REFCOUNT_ADD(*object)
    return NOERROR;
}

ECode CCollections::UnmodifiableMap::UnmodifiableEntrySet::Iterator::Next(
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object)
    AutoPtr<IMapEntry> o;
    Next((IMapEntry**)&o);
    *object = o->Probe(EIID_IInterface);
    REFCOUNT_ADD(*object)
    return NOERROR;
}

ECode CCollections::UnmodifiableMap::UnmodifiableEntrySet::Iterator::Remove()
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

//====================================================================
// CCollections::UnmodifiableMap::UnmodifiableEntrySet::
//====================================================================

CCollections::UnmodifiableMap::UnmodifiableEntrySet::UnmodifiableEntrySet(
    /* [in] */ ISet* set)
    : UnmodifiableSet(set)
{
}

ECode CCollections::UnmodifiableMap::UnmodifiableEntrySet::GetIterator(
    /* [out] */ IIterator** result)
{
    VALIDATE_NOT_NULL(result)
    AutoPtr<IIterator> res = new Iterator(this);
    *result = res;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode CCollections::UnmodifiableMap::UnmodifiableEntrySet::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    VALIDATE_NOT_NULL(array)
    Int32 length;
    mC->GetSize(&length);
    AutoPtr<ArrayOf<IInterface*> > result = ArrayOf<IInterface*>::Alloc(length);
    AutoPtr<IIterator> it;
    GetIterator((IIterator**)&it);
    for (Int32 i = length; --i >= 0;) {
        it->Next(&(*result)[i]);
    }
    *array = result;
    REFCOUNT_ADD(*array)
    return NOERROR;
}

ECode CCollections::UnmodifiableMap::UnmodifiableEntrySet::ToArray(
    /* [in] */ ArrayOf<IInterface*>* inArray,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    VALIDATE_NOT_NULL(outArray)
    Int32 size, index = 0;
    mC->GetSize(&size);
    AutoPtr<IIterator> it;
    GetIterator((IIterator**)&it);
    if (size > inArray->GetLength()) {
        inArray = ArrayOf<IInterface*>::Alloc(size);
    }
    while (index < size) {
        AutoPtr<IInterface> o;
        it->Next((IInterface**)&o);
        inArray->Set(index++, o);
    }
    if (index < inArray->GetLength()) {
        inArray->Set(index, NULL);
    }
    *outArray = inArray;
    REFCOUNT_ADD(*outArray)
    return NOERROR;
}

//====================================================================
// CCollections::UnmodifiableMap::
//====================================================================

UInt32 CCollections::UnmodifiableMap::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CCollections::UnmodifiableMap::Release()
{
    return ElRefBase::Release();
}

PInterface CCollections::UnmodifiableMap::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_IInterface == riid) {
        return (PInterface)(IMap*)this;
    }
    else if (EIID_IIterable == riid) {
        return (IIterable*)(IMap*)this;
    }
    else if (EIID_ICollection == riid) {
        return (ICollection*)(IMap*)this;
    }
    else if (EIID_IMap == riid) {
        return (IMap*)this;
    }
    else if (Elastos::IO::EIID_ISerializable == riid) {
        return (ISerializable*)this;
    }

    return NULL;
}

ECode CCollections::UnmodifiableMap::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID)

    if (pObject == (IInterface*)(IMap*)this) {
        *pIID = EIID_IMap;
    }
    else if (pObject == (IInterface*)(ISerializable*)this) {
        *pIID = Elastos::IO::EIID_ISerializable;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

CCollections::UnmodifiableMap::UnmodifiableMap(
    /* [in] */ IMap* map)
{
    mM = map;
}

ECode CCollections::UnmodifiableMap::Clear()
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode CCollections::UnmodifiableMap::ContainsKey(
    /* [in] */ IInterface* key,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return mM->ContainsKey(key, result);
}

ECode CCollections::UnmodifiableMap::ContainsValue(
    /* [in] */ IInterface* value,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return mM->ContainsValue(value, result);
}

ECode CCollections::UnmodifiableMap::EntrySet(
    /* [out, callee] */ ISet** entries)
{
    VALIDATE_NOT_NULL(entries)
    AutoPtr<ISet> set;
    mM->EntrySet((ISet**)&set);
    AutoPtr<ISet> res = new UnmodifiableEntrySet(set);
    *entries = res;
    REFCOUNT_ADD(*entries)
    return NOERROR;
}

ECode CCollections::UnmodifiableMap::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return mM->Equals(object, result);
}

ECode CCollections::UnmodifiableMap::Get(
    /* [in] */ PInterface key,
    /* [out] */ PInterface* value)
{
    VALIDATE_NOT_NULL(value)
    return mM->Get(key, value);
}

ECode CCollections::UnmodifiableMap::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode)
    return mM->GetHashCode(hashCode);
}

ECode CCollections::UnmodifiableMap::IsEmpty(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return mM->IsEmpty(result);
}

ECode CCollections::UnmodifiableMap::KeySet(
    /* [out, callee] */ ISet** keySet)
{
    VALIDATE_NOT_NULL(keySet)
    AutoPtr<ISet> set;
    mM->KeySet((ISet**)&set);
    AutoPtr<ISet> res = new UnmodifiableSet(set);
    *keySet = res;
    REFCOUNT_ADD(*keySet)
    return NOERROR;
}

ECode CCollections::UnmodifiableMap::Put(
    /* [in] */ PInterface key,
    /* [in] */ PInterface value,
    /* [out] */ PInterface* oldValue)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode CCollections::UnmodifiableMap::PutAll(
    /* [in] */ IMap* map)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode CCollections::UnmodifiableMap::Remove(
    /* [in] */ PInterface key,
    /* [out] */ PInterface* value)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode CCollections::UnmodifiableMap::GetSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size)
    return mM->GetSize(size);
}

ECode CCollections::UnmodifiableMap::Values(
    /* [out] */ ICollection** value)
{
    VALIDATE_NOT_NULL(value)
    AutoPtr<ICollection> v;
    mM->Values((ICollection**)&v);
    AutoPtr<ICollection> res = new UnmodifiableCollection(v);
    *value = res;
    REFCOUNT_ADD(*value)
    return NOERROR;
}

ECode CCollections::UnmodifiableMap::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    AutoPtr<IObject> iObj;
    iObj = IObject::Probe(mM);
    if (iObj != NULL) {
        return iObj->ToString(str);
    }
    *str = NULL;
    return NOERROR;
}

//====================================================================
// CCollections::UnmodifiableSet::
//====================================================================

UInt32 CCollections::UnmodifiableSet::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CCollections::UnmodifiableSet::Release()
{
    return ElRefBase::Release();
}

PInterface CCollections::UnmodifiableSet::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_IInterface == riid) {
        return (PInterface)(ISet*)this;
    }
    else if (EIID_IIterable == riid) {
        return (IIterable*)(ISet*)this;
    }
    else if (EIID_ICollection == riid) {
        return (ICollection*)(ISet*)this;
    }
    else if (EIID_ISet == riid) {
        return (ISet*)this;
    }
    else if (Elastos::IO::EIID_ISerializable == riid) {
        return (ISerializable*)this;
    }

    return NULL;
}

ECode CCollections::UnmodifiableSet::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID)

    if (pObject == (IInterface*)(ISet*)this) {
        *pIID = EIID_ISet;
    }
    else if (pObject == (IInterface*)(ISerializable*)this) {
        *pIID = Elastos::IO::EIID_ISerializable;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

CCollections::UnmodifiableSet::UnmodifiableSet(
    /* [in] */ ISet* set)
    : UnmodifiableCollection(set)
{
}

ECode CCollections::UnmodifiableSet::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return mC->Equals(object, result);
}

ECode CCollections::UnmodifiableSet::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode)
    return mC->GetHashCode(hashCode);
}

ECode CCollections::UnmodifiableSet::Add(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return UnmodifiableCollection::Add(object, modified);
}

ECode CCollections::UnmodifiableSet::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return UnmodifiableCollection::AddAll(collection, modified);
}

ECode CCollections::UnmodifiableSet::Clear()
{
    return UnmodifiableCollection::Clear();
}

ECode CCollections::UnmodifiableSet::Contains(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return UnmodifiableCollection::Contains(object, result);
}

ECode CCollections::UnmodifiableSet::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return UnmodifiableCollection::ContainsAll(collection, result);
}

ECode CCollections::UnmodifiableSet::IsEmpty(
    /* [out] */ Boolean* result)
{
    return UnmodifiableCollection::IsEmpty(result);
}

ECode CCollections::UnmodifiableSet::GetIterator(
    /* [out] */ IIterator** it)
{
    return UnmodifiableCollection::GetIterator(it);
}

ECode CCollections::UnmodifiableSet::Remove(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return UnmodifiableCollection::Remove(object, modified);
}

ECode CCollections::UnmodifiableSet::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return UnmodifiableCollection::RemoveAll(collection, modified);
}

ECode CCollections::UnmodifiableSet::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return UnmodifiableCollection::RetainAll(collection, modified);
}

ECode CCollections::UnmodifiableSet::GetSize(
    /* [out] */ Int32* size)
{
    return UnmodifiableCollection::GetSize(size);
}

ECode CCollections::UnmodifiableSet::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    return UnmodifiableCollection::ToArray(array);
}

ECode CCollections::UnmodifiableSet::ToArray(
    /* [in] */ ArrayOf<IInterface*>* inArray,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    return UnmodifiableCollection::ToArray(inArray, outArray);
}

//====================================================================
// CCollections::UnmodifiableSortedMap::
//====================================================================

UInt32 CCollections::UnmodifiableSortedMap::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CCollections::UnmodifiableSortedMap::Release()
{
    return ElRefBase::Release();
}

PInterface CCollections::UnmodifiableSortedMap::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_ISortedMap == riid) {
        return (ISortedMap*)this;
    }
    return UnmodifiableMap::Probe(riid);
}

ECode CCollections::UnmodifiableSortedMap::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID)

    if (pObject == (IInterface*)(ISortedMap*)this) {
        *pIID = EIID_ISortedMap;
    }

    return UnmodifiableMap::GetInterfaceID(pObject, pIID);
}

CCollections::UnmodifiableSortedMap::UnmodifiableSortedMap(
    /* [in] */ ISortedMap* map)
    : UnmodifiableMap(map)
{
    mSm = map;
}

ECode CCollections::UnmodifiableSortedMap::Comparator(
    /* [out] */ IComparator** comp)
{
    VALIDATE_NOT_NULL(comp)
    return mSm->Comparator(comp);
}

ECode CCollections::UnmodifiableSortedMap::FirstKey(
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface)
    return mSm->FirstKey(outface);
}

ECode CCollections::UnmodifiableSortedMap::HeadMap(
    /* [in] */ IInterface* endKey,
    /* [out] */ ISortedMap** sortmap)
{
    VALIDATE_NOT_NULL(sortmap)
    return mSm->HeadMap(endKey, sortmap);
}

ECode CCollections::UnmodifiableSortedMap::LastKey(
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface)
    return mSm->LastKey(outface);
}

ECode CCollections::UnmodifiableSortedMap::SubMap(
    /* [in] */ IInterface* startKey,
    /* [in] */ IInterface* endKey,
    /* [out] */ ISortedMap** sortmap)
{
    VALIDATE_NOT_NULL(sortmap)
    return mSm->SubMap(startKey, endKey, sortmap);
}

ECode CCollections::UnmodifiableSortedMap::TailMap(
    /* [in] */ IInterface* startKey,
    /* [out] */ ISortedMap** sortmap)
{
    VALIDATE_NOT_NULL(sortmap)
    return mSm->TailMap(startKey, sortmap);
}

ECode CCollections::UnmodifiableSortedMap::Clear()
{
    return UnmodifiableMap::Clear();
}

ECode CCollections::UnmodifiableSortedMap::ContainsKey(
    /* [in] */ IInterface* key,
    /* [out] */ Boolean* result)
{
    return UnmodifiableMap::ContainsKey(key, result);
}

ECode CCollections::UnmodifiableSortedMap::ContainsValue(
    /* [in] */ IInterface* value,
    /* [out] */ Boolean* result)
{
    return UnmodifiableMap::ContainsValue(value, result);
}

ECode CCollections::UnmodifiableSortedMap::EntrySet(
    /* [out, callee] */ ISet** entries)
{
    return UnmodifiableMap::EntrySet(entries);
}

ECode CCollections::UnmodifiableSortedMap::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return UnmodifiableMap::Equals(object, result);
}

ECode CCollections::UnmodifiableSortedMap::Get(
    /* [in] */ PInterface key,
    /* [out] */ PInterface* value)
{
    return UnmodifiableMap::Get(key, value);
}

ECode CCollections::UnmodifiableSortedMap::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    return UnmodifiableMap::GetHashCode(hashCode);
}

ECode CCollections::UnmodifiableSortedMap::IsEmpty(
    /* [out] */ Boolean* result)
{
    return UnmodifiableMap::IsEmpty(result);
}

ECode CCollections::UnmodifiableSortedMap::KeySet(
    /* [out, callee] */ ISet** keySet)
{
    return UnmodifiableMap::KeySet(keySet);
}

ECode CCollections::UnmodifiableSortedMap::Put(
    /* [in] */ PInterface key,
    /* [in] */ PInterface value,
    /* [out] */ PInterface* oldValue)
{
    return UnmodifiableMap::Put(key, value, oldValue);
}

ECode CCollections::UnmodifiableSortedMap::PutAll(
    /* [in] */ IMap* map)
{
    return UnmodifiableMap::PutAll(map);
}

ECode CCollections::UnmodifiableSortedMap::Remove(
    /* [in] */ PInterface key,
    /* [out] */ PInterface* value)
{
    return UnmodifiableMap::Remove(key, value);
}

ECode CCollections::UnmodifiableSortedMap::GetSize(
    /* [out] */ Int32* size)
{
    return UnmodifiableMap::GetSize(size);
}

ECode CCollections::UnmodifiableSortedMap::Values(
    /* [out] */ ICollection** value)
{
    return UnmodifiableMap::Values(value);
}

//====================================================================
// CCollections::UnmodifiableSortedSet::
//====================================================================

UInt32 CCollections::UnmodifiableSortedSet::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CCollections::UnmodifiableSortedSet::Release()
{
    return ElRefBase::Release();
}

PInterface CCollections::UnmodifiableSortedSet::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_ISortedSet == riid) {
        return (ISortedSet*)this;
    }
    return UnmodifiableSet::Probe(riid);
}

ECode CCollections::UnmodifiableSortedSet::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID)

    if (pObject == (IInterface*)(ISortedSet*)this) {
        *pIID = EIID_ISortedSet;
    }

    return UnmodifiableSet::GetInterfaceID(pObject, pIID);
}

CCollections::UnmodifiableSortedSet::UnmodifiableSortedSet(
    /* [in] */ ISortedSet* set)
    : UnmodifiableSet(set)
{
    mSs = set;
}

ECode CCollections::UnmodifiableSortedSet::Comparator(
    /* [out] */ IComparator** outcom)
{
    VALIDATE_NOT_NULL(outcom)
    return mSs->Comparator(outcom);
}

ECode CCollections::UnmodifiableSortedSet::First(
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface)
    return mSs->First(outface);
}

ECode CCollections::UnmodifiableSortedSet::HeadSet(
    /* [in] */ IInterface* end,
    /* [out] */ ISortedSet** outsort)
{
    VALIDATE_NOT_NULL(outsort)
    return mSs->HeadSet(end, outsort);
}

ECode CCollections::UnmodifiableSortedSet::Last(
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface)
    return mSs->Last(outface);
}

ECode CCollections::UnmodifiableSortedSet::SubSet(
    /* [in] */ IInterface* start,
    /* [in] */ IInterface* end,
    /* [out] */ ISortedSet** outsort)
{
    VALIDATE_NOT_NULL(outsort)
    return mSs->SubSet(start, end, outsort);
}

ECode CCollections::UnmodifiableSortedSet::TailSet(
    /* [in] */ IInterface* start,
    /* [out] */ ISortedSet** outsort)
{
    VALIDATE_NOT_NULL(outsort)
    return mSs->TailSet(start, outsort);
}

ECode CCollections::UnmodifiableSortedSet::Add(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return UnmodifiableCollection::Add(object, modified);
}

ECode CCollections::UnmodifiableSortedSet::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return UnmodifiableCollection::AddAll(collection, modified);
}

ECode CCollections::UnmodifiableSortedSet::Clear()
{
    return UnmodifiableCollection::Clear();
}

ECode CCollections::UnmodifiableSortedSet::Contains(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return UnmodifiableCollection::Contains(object, result);
}

ECode CCollections::UnmodifiableSortedSet::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return UnmodifiableCollection::ContainsAll(collection, result);
}

ECode CCollections::UnmodifiableSortedSet::IsEmpty(
    /* [out] */ Boolean* result)
{
    return UnmodifiableCollection::IsEmpty(result);
}

ECode CCollections::UnmodifiableSortedSet::GetIterator(
    /* [out] */ IIterator** it)
{
    return UnmodifiableCollection::GetIterator(it);
}

ECode CCollections::UnmodifiableSortedSet::Remove(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return UnmodifiableCollection::Remove(object, modified);
}

ECode CCollections::UnmodifiableSortedSet::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return UnmodifiableCollection::RemoveAll(collection, modified);
}

ECode CCollections::UnmodifiableSortedSet::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return UnmodifiableCollection::RetainAll(collection, modified);
}

ECode CCollections::UnmodifiableSortedSet::GetSize(
    /* [out] */ Int32* size)
{
    return UnmodifiableCollection::GetSize(size);
}

ECode CCollections::UnmodifiableSortedSet::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    return UnmodifiableCollection::ToArray(array);
}

ECode CCollections::UnmodifiableSortedSet::ToArray(
    /* [in] */ ArrayOf<IInterface*>* inArray,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    return UnmodifiableCollection::ToArray(inArray, outArray);
}

ECode CCollections::UnmodifiableSortedSet::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return UnmodifiableSet::Equals(object, result);
}

ECode CCollections::UnmodifiableSortedSet::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    return UnmodifiableSet::GetHashCode(hashCode);
}

//====================================================================
// CCollections::
//====================================================================

// CCollections::CCollections()
// {}

ECode CCollections::_BinarySearch(
    /* [in] */ IList* list,
    /* [in] */ IInterface* object,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index)
    if (list == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }
    Boolean b;
    if ((list->IsEmpty(&b), b)) {
        *index = -1;
        return NOERROR;
    }

    if (IRandomAccess::Probe(list) == NULL) {
        AutoPtr<IListIterator> it;
        list->GetListIterator((IListIterator**)&it);
        Boolean b;
        while ((it->HasNext(&b), b)) {
            Int32 result;
            AutoPtr<IInterface> o;
            it->Next((IInterface**)&o);
            AutoPtr<IComparable> com = IComparable::Probe(o);
            com->CompareTo(object, &result);
            result = -result;
            if (result <= 0) {
                if (result == 0) {
                    return it->PreviousIndex(index);
                }
                Int32 preIndex;
                it->PreviousIndex(&preIndex);
                *index = -preIndex - 1;
                return NOERROR;
            }
        }
        Int32 num;
        list->GetSize(&num);
        *index = -num - 1;
        return NOERROR;
    }

    Int32 low = 0, mid, high, result = -1;
    list->GetSize(&mid);
    high = mid - 1;
    while (low <= high) {
        mid = (UInt32(low + high)) >> 1;
        AutoPtr<IInterface> o;
        list->Get(mid, (IInterface**)&o);
        AutoPtr<IComparable> com = IComparable::Probe(o);
        assert(com != NULL);
        com->CompareTo(object, &result);
        result = -result;
        if (result > 0) {
            low = mid + 1;
        }
        else if (result == 0) {
            *index = mid;
            return NOERROR;
        }
        else {
            high = mid - 1;
        }
    }
    *index = -mid - (result < 0 ? 1 : 2);
    return NOERROR;
}

ECode CCollections::_BinarySearch(
    /* [in] */ IList* list,
    /* [in] */ IInterface* object,
    /* [in] */ IComparator* comparator,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index)
    if (comparator == NULL) {
        return _BinarySearch(list, object, index);
    }
    if (IRandomAccess::Probe(list) == NULL) {
        AutoPtr<IListIterator> it;
        list->GetListIterator((IListIterator**)&it);
        Boolean b;
        while ((it->HasNext(&b), b)) {
            Int32 result;
            AutoPtr<IInterface> o;
            it->Next((IInterface**)&o);
            comparator->Compare(o, object, &result);
            result = -result;
            if (result <= 0) {
                if (result == 0) {
                    return it->PreviousIndex(index);
                }
                Int32 preIndex;
                it->PreviousIndex(&preIndex);
                *index = -preIndex - 1;
                return NOERROR;
            }
        }
        Int32 size;
        list->GetSize(&size);
        *index = -size - 1;
        return NOERROR;
    }

    Int32 low = 0, mid, high, result = -1;
    list->GetSize(&mid);
    high = mid - 1;
    while (low <= high) {
        mid = (UInt32(low + high)) >> 1;
        AutoPtr<IInterface> o;
        list->Get(mid, (IInterface**)&o);
        comparator->Compare(o, object, &result);
        result = -result;
        if (result > 0) {
            low = mid + 1;
        }
        else if (result == 0) {
            *index = mid;
            return NOERROR;
        }
        else {
            high = mid - 1;
        }
    }
    *index = -mid - (result < 0 ? 1 : 2);
    return NOERROR;
}

ECode CCollections::_Copy(
    /* [in] */ IList* destination,
    /* [in] */ IList* source)
{
    ECode ec = 0;
    Int32 sizeDes, sizeSrc;
    destination->GetSize(&sizeDes);
    source->GetSize(&sizeSrc);
    if (sizeDes < sizeSrc) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    AutoPtr<IIterator> srcIt;
    source->GetIterator((IIterator**)&srcIt);
    AutoPtr<IListIterator> destIt;
    destination->GetListIterator((IListIterator**)&destIt);
    Boolean b;
    while ((srcIt->HasNext(&b), b)) {
        AutoPtr<IInterface> o1;
        ec = destIt->Next((IInterface**)&o1);
        if (ec != NOERROR) {
            return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
        }
        AutoPtr<IInterface> o2;
        srcIt->Next((IInterface**)&o2);
        destIt->Set(o2);
    }
    return NOERROR;
}

ECode CCollections::_NewEnumeration(
    /* [in] */ ICollection* collection,
    /* [out] */ IEnumeration** result)
{
    VALIDATE_NOT_NULL(result)
    AutoPtr<ICollection> c = collection;
    AutoPtr<IEnumeration> res = new MyEnumeration2(c);
    *result = res;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode CCollections::_Fill(
    /* [in] */ IList* list,
    /* [in] */ IInterface* object)
{
    AutoPtr<IListIterator> it;
    list->GetListIterator((IListIterator**)&it);
    Boolean b;
    while ((it->HasNext(&b), b)) {
        AutoPtr<IInterface> o;
        it->Next((IInterface**)&o);
        it->Set(object);
    }
    return NOERROR;
}

ECode CCollections::_Max(
    /* [in] */ ICollection* collection,
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result)
    AutoPtr<IIterator> it;
    collection->GetIterator((IIterator**)&it);
    AutoPtr<IInterface> max;
    it->Next((IInterface**)&max);
    Boolean b;
    while ((it->HasNext(&b), b)) {
        AutoPtr<IInterface> next;
        it->Next((IInterface**)&next);
        AutoPtr<IComparable> res = IComparable::Probe(max);
        Int32 val;
        res->CompareTo(next, &val);
        if (val < 0) {
            max = next;
        }
    }
    *result = max;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode CCollections::_Max(
    /* [in] */ ICollection* collection,
    /* [in] */ IComparator* comparator,
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result)
    if (comparator == NULL) {
        return _Max(collection, result);
    }

    AutoPtr<IIterator> it;
    collection->GetIterator((IIterator**)&it);
    AutoPtr<IInterface> max;
    it->Next((IInterface**)&max);
    Boolean b;
    while ((it->HasNext(&b), b)) {
        AutoPtr<IInterface> next;
        it->Next((IInterface**)&next);
        Int32 res;
        if ((comparator->Compare(max, next, &res), res) < 0) {
            max = next;
        }
    }
    *result = max;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode CCollections::_Min(
    /* [in] */ ICollection* collection,
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result)
    AutoPtr<IIterator> it;
    collection->GetIterator((IIterator**)&it);
    AutoPtr<IInterface> min;
    it->Next((IInterface**)&min);
    Boolean b;
    while ((it->HasNext(&b), b)) {
        AutoPtr<IInterface> next;
        it->Next((IInterface**)&next);
        AutoPtr<IComparable> com = IComparable::Probe(min);
        Int32 res;
        if ((com->CompareTo(next, &res), res) > 0) {
            min = next;
        }
    }
    *result = min;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode CCollections::_Min(
    /* [in] */ ICollection* collection,
    /* [in] */ IComparator* comparator,
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result)
    if (comparator == NULL) {
        return _Min(collection, result);
    }

    AutoPtr<IIterator> it;
    collection->GetIterator((IIterator**)&it);
    AutoPtr<IInterface> min;
    it->Next((IInterface**)&min);
    Boolean b;
    while ((it->HasNext(&b), b)) {
        AutoPtr<IInterface> next;
        it->Next((IInterface**)&next);
        Int32 num = 0;
        comparator->Compare(min, next, &num);
        if (num > 0) {
            min = next;
        }
    }
    *result = min;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode CCollections::_NCopies(
    /* [in] */ Int32 length,
    /* [in] */ IInterface* object,
    /* [out] */ IList** result)
{
    VALIDATE_NOT_NULL(result)
    AutoPtr<IList> res = new CopiesList(length, object);
    *result = res;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode CCollections::_Reverse(
    /* [in] */ IList* list)
{
    Int32 size;
    list->GetSize(&size);
    AutoPtr<IListIterator> front;
    list->GetListIterator((IListIterator**)&front);
    AutoPtr<IListIterator> back;
    list->GetListIterator(size, (IListIterator**)&back);
    for (Int32 i = 0; i < size / 2; i++) {
        AutoPtr<IInterface> frontNext;
        front->Next((IInterface**)&frontNext);
        AutoPtr<IInterface> backPrev;
        back->Previous((IInterface**)&backPrev);
        front->Set(backPrev);
        back->Set(frontNext);
    }
    return NOERROR;
}

ECode CCollections::_ReverseOrder(
    /* [out] */ IComparator** result)
{
    VALIDATE_NOT_NULL(result)
    *result = ReverseComparator::INSTANCE;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode CCollections::_ReverseOrder(
    /* [in] */ IComparator* c,
    /* [out] */ IComparator** result)
{
    VALIDATE_NOT_NULL(result)
    if (c == NULL) {
        return _ReverseOrder(result);
    }
    AutoPtr<IComparator> p = IComparator::Probe(c);
    if (p != NULL) { // ???
        *result = ((ReverseComparator2*)p.Get())->mCmp;
        REFCOUNT_ADD(*result)
        return NOERROR;
    }
    AutoPtr<ReverseComparator2> res = new ReverseComparator2(c);
    *result = res;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode CCollections::_Shuffle(
    /* [in] */ IList* list)
{
    AutoPtr<IRandom> r;
    CRandom::New((IRandom**)&r);
    return _Shuffle(list, r);
}

ECode CCollections::_Shuffle(
    /* [in] */ IList* list,
    /* [in] */ IRandom* random)
{
    AutoPtr<IList> objectList = list;

    if (IRandomAccess::Probe(list) != NULL) {
        Int32 i;
        objectList->GetSize(&i);
        for (i = i - 1; i > 0; i--) {
            Int32 index;
            random->NextInt32(i + 1, &index);
            AutoPtr<IInterface> o;
            objectList->Get(index, (IInterface**)&o);
            AutoPtr<IInterface> pre, pre2;
            objectList->Set(i, o, (IInterface**)&pre);
            objectList->Set(index, pre, (IInterface**)&pre2);
        }
    }
    else {
        AutoPtr<ArrayOf<IInterface*> > array;
        objectList->ToArray((ArrayOf<IInterface*>**)&array);
        for (Int32 i = array->GetLength() - 1; i > 0; i--) {
            Int32 index;
            random->NextInt32(i + 1, &index);
            AutoPtr<IInterface> temp = (*array)[i];
            array->Set(i, (*array)[index]);
            array->Set(index, temp);
        }

        Int32 i = 0;
        AutoPtr<IListIterator> it;
        objectList->GetListIterator((IListIterator**)&it);
        Boolean b;
        while ((it->HasNext(&b), b)) {
            AutoPtr<IInterface> o;
            it->Next((IInterface**)&o);
            it->Set((*array)[i++]);
        }
    }
    return NOERROR;
}

ECode CCollections::_NewSingleton(
    /* [in] */ IInterface* object,
    /* [out] */ ISet** result)
{
    VALIDATE_NOT_NULL(result)
    AutoPtr<ISet> res = new SingletonSet(object);
    *result = res;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode CCollections::_NewSingletonList(
    /* [in] */ IInterface* object,
    /* [out] */ IList** result)
{
    VALIDATE_NOT_NULL(result)
    AutoPtr<IList> res = new SingletonList(object);
    *result = res;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode CCollections::_NewSingletonMap(
    /* [in] */ IInterface* key,
    /* [in] */ IInterface* value,
    /* [out] */ IMap** result)
{
    VALIDATE_NOT_NULL(result)
    AutoPtr<IMap> res = new SingletonMap(key, value);
    *result = res;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode CCollections::_Sort(
    /* [in] */ IList* list)
{
    AutoPtr<ArrayOf<IInterface*> > array;
    list->ToArray((ArrayOf<IInterface*>**)&array);
    CArrays::_SortObject(array);
    Int32 i = 0;
    AutoPtr<IListIterator> it;
    list->GetListIterator((IListIterator**)&it);
    Boolean b;
    while ((it->HasNext(&b), b)) {
        AutoPtr<IInterface> o;
        it->Next((IInterface**)&o);
        it->Set((*array)[i++]);
    }
    return NOERROR;
}

ECode CCollections::_Sort(
    /* [in] */ IList* list,
    /* [in] */ IComparator* comparator)
{
    Int32 num;
    list->GetSize(&num);
    AutoPtr<ArrayOf<IInterface*> > arr = ArrayOf<IInterface*>::Alloc(num);
    AutoPtr<ArrayOf<IInterface*> > array;
    list->ToArray(arr, (ArrayOf<IInterface*>**)&array);
    CArrays::_Sort(array, comparator);
    AutoPtr<IListIterator> it;
    list->GetListIterator((IListIterator**)&it);
    Boolean b;
    while ((it->HasNext(&b), b)) {
        Int32 i = 0;
        AutoPtr<IInterface> o;
        it->Next((IInterface**)&o);
        it->Set((*array)[i++]);
    }
    return NOERROR;
}

ECode CCollections::_Swap(
    /* [in] */ IList* list,
    /* [in] */ Int32 index1,
    /* [in] */ Int32 index2)
{
    if (list == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }
    Int32 size;
    list->GetSize(&size);
    if (index1 < 0 || index1 >= size || index2 < 0 || index2 >= size) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    if (index1 == index2) {
        return NOERROR;
    }
    AutoPtr<IList> rawList = list;
    AutoPtr<IInterface> o;
    rawList->Get(index2, (IInterface**)&o);
    AutoPtr<IInterface> old, old2;
    rawList->Set(index1, o, (IInterface**)&old);
    rawList->Set(index2, old, (IInterface**)&old2);
    return NOERROR;
}

ECode CCollections::_ReplaceAll(
    /* [in] */ IList* list,
    /* [in] */ IInterface* obj,
    /* [in] */ IInterface* obj2,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    Int32 index;
    Boolean found = FALSE;
    while ((list->IndexOf(obj, &index), index) > -1) {
        found = TRUE;
        AutoPtr<IInterface> pre;
        list->Set(index, obj2, (IInterface**)&pre);
    }
    *result = found;
    return NOERROR;
}

ECode CCollections::_Rotate(
    /* [in] */ IList* lst,
    /* [in] */ Int32 dist)
{
    AutoPtr<IList> list = lst;
    Int32 size;
    list->GetSize(&size);

    // Can't sensibly rotate an empty collection
    if (size == 0) {
        return NOERROR;
    }

    // normalize the distance
    Int32 normdist;
    if (dist > 0) {
        normdist = dist % size;
    }
    else {
        normdist = size - ((dist % size) * (-1));
    }

    if (normdist == 0 || normdist == size) {
        return NOERROR;
    }

    if (IRandomAccess::Probe(list) != NULL) {
        // make sure each element gets juggled
        // with the element in the position it is supposed to go to
        AutoPtr<IInterface> temp;
        list->Get(0, (IInterface**)&temp);
        Int32 index = 0, beginIndex = 0;
        for (Int32 i = 0; i < size; i++) {
            index = (index + normdist) % size;
            AutoPtr<IInterface> temp2;
            list->Set(index, temp, (IInterface**)&temp2);
            if (index == beginIndex) {
                index = ++beginIndex;
                temp = NULL;
                list->Get(beginIndex, (IInterface**)&temp);
            }
        }
    }
    else {
        Int32 divideIndex = (size - normdist) % size;
        AutoPtr<IList> sublist1;
        list->SubList(0, divideIndex, (IList**)&sublist1);
        AutoPtr<IList> sublist2;
        list->SubList(divideIndex, size, (IList**)&sublist2);
        _Reverse(sublist1);
        _Reverse(sublist2);
        _Reverse(list);
    }
    return NOERROR;
}

ECode CCollections::_IndexOfSubList(
    /* [in] */ IList* list,
    /* [in] */ IList* sublist,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)
    Int32 size, sublistSize;
    list->GetSize(&size);
    sublist->GetSize(&sublistSize);

    if (sublistSize > size) {
        *result = -1;
        return NOERROR;
    }

    if (sublistSize == 0) {
        *result = 0;
        return NOERROR;
    }

    // find the first element of sublist in the list to get a head start
    AutoPtr<IInterface> firstObj;
    sublist->Get(0, (IInterface**)&firstObj);
    Int32 index;
    list->IndexOf(firstObj, &index);
    if (index == -1) {
        *result = -1;
        return NOERROR;
    }

    while (index < size && (size - index >= sublistSize)) {
        AutoPtr<IListIterator> listIt;
        list->GetListIterator(index, (IListIterator**)&listIt);

        AutoPtr<IInterface> o;
        listIt->Next((IInterface**)&o);
        if ((firstObj == NULL) ? o == NULL : ObjectUtils::Equals(firstObj, o)) {

            // iterate through the elements in sublist to see
            // if they are included in the same order in the list
            AutoPtr<IListIterator> sublistIt;
            sublist->GetListIterator(1, (IListIterator**)&sublistIt);
            Boolean difFound = FALSE, subHasNext, listHasNext;
            while ((sublistIt->HasNext(&subHasNext), subHasNext)) {
                AutoPtr<IInterface> element;
                sublistIt->Next((IInterface**)&element);
                if (!(listIt->HasNext(&listHasNext), listHasNext)) {
                    *result = -1;
                    return NOERROR;
                }
                AutoPtr<IInterface> o;
                listIt->Next((IInterface**)&o);
                if ((element == NULL) ? o != NULL : !ObjectUtils::Equals(element, o)) {
                    difFound = TRUE;
                    break;
                }
            }
            // All elements of sublist are found in main list
            // starting from index.
            if (!difFound) {
                *result = index;
                return NOERROR;
            }
        }
        // This was not the sequence we were looking for,
        // continue search for the firstObj in main list
        // at the position after index.
        index++;
    }
    *result = -1;
    return NOERROR;
}

ECode CCollections::_LastIndexOfSubList(
    /* [in] */ IList* list,
    /* [in] */ IList* sublist,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)
    Int32 sublistSize, size;
    sublist->GetSize(&sublistSize);
    list->GetSize(&size);

    if (sublistSize > size) {
        *result = -1;
        return NOERROR;
    }

    if (sublistSize == 0) {
        *result = size;
        return NOERROR;
    }

    // find the last element of sublist in the list to get a head start
    AutoPtr<IInterface> lastObj;
    sublist->Get(sublistSize - 1, (IInterface**)&lastObj);
    Int32 index;
    list->LastIndexOf(lastObj, &index);

    while ((index > -1) && (index + 1 >= sublistSize)) {
        AutoPtr<IListIterator> listIt;
        list->GetListIterator(index + 1, (IListIterator**)&listIt);

        AutoPtr<IInterface> o;
        listIt->Previous((IInterface**)&o);
        if ((lastObj == NULL) ? o == NULL : ObjectUtils::Equals(lastObj, o)) {
            // iterate through the elements in sublist to see
            // if they are included in the same order in the list
            AutoPtr<IListIterator> sublistIt;
            sublist->GetListIterator(sublistSize - 1, (IListIterator**)&sublist);
            Boolean difFound = FALSE;
            Boolean subPrevious, listPrevious;
            while ((sublistIt->HasPrevious(&subPrevious), subPrevious)) {
                AutoPtr<IInterface> element;
                sublistIt->Previous((IInterface**)&element);
                if (!(listIt->HasPrevious(&listPrevious), listPrevious)) {
                    *result = -1;
                    return NOERROR;
                }

                AutoPtr<IInterface> o;
                listIt->Previous((IInterface**)&o);
                if ((element == NULL) ? o != NULL : !ObjectUtils::Equals(element, o)) {
                    difFound = TRUE;
                    break;
                }
            }
            // All elements of sublist are found in main list
            // starting from listIt.nextIndex().
            if (!difFound) {
                return listIt->NextIndex(result);
            }
        }
        // This was not the sequence we were looking for,
        // continue search for the lastObj in main list
        // at the position before index.
        index--;
    }
    *result = -1;
    return NOERROR;
}

ECode CCollections::_NewList(
    /* [in] */ IEnumeration* enumeration,
    /* [out] */ IArrayList** result)
{
    VALIDATE_NOT_NULL(result)
    AutoPtr<IArrayList> list;
    CArrayList::New((IArrayList**)&list);
    Boolean b;
    while ((enumeration->HasMoreElements(&b), b)) {
        AutoPtr<IInterface> o;
        enumeration->NextElement((IInterface**)&o);
        Boolean modified;
        list->Add(o, &modified);
    }
    *result = list;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode CCollections::_NewSynchronizedCollection(
    /* [in] */ ICollection* collection,
    /* [out] */ ICollection** result)
{
    VALIDATE_NOT_NULL(result)
    if (collection == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }
    AutoPtr<ICollection> res = new SynchronizedCollection(collection);
    *result = res;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode CCollections::_NewSynchronizedList(
    /* [in] */ IList* list,
    /* [out] */ IList** result)
{
    VALIDATE_NOT_NULL(result)
    if (list == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }
    if (IRandomAccess::Probe(list) != NULL) {
        AutoPtr<IList> res = new SynchronizedRandomAccessList(list);
        *result = res;
        REFCOUNT_ADD(*result)
        return NOERROR;
    }
    AutoPtr<IList> res = new SynchronizedList(list);
    *result = res;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode CCollections::_NewSynchronizedMap(
    /* [in] */ IMap* map,
    /* [out] */ IMap** result)
{
    VALIDATE_NOT_NULL(result)
    if (map == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }
    AutoPtr<IMap> res = new SynchronizedMap(map);
    *result = res;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode CCollections::_NewSynchronizedSet(
    /* [in] */ ISet* set,
    /* [out] */ ISet** result)
{
    VALIDATE_NOT_NULL(result)
    if (set == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }
    AutoPtr<ISet> res = new SynchronizedSet(set);
    *result = res;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode CCollections::_NewSynchronizedSortedMap(
    /* [in] */ ISortedMap* map,
    /* [out] */ ISortedMap** result)
{
    VALIDATE_NOT_NULL(result)
    if (map == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }
    AutoPtr<ISortedMap> res = new SynchronizedSortedMap(map);
    *result = res;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode CCollections::_NewSynchronizedSortedSet(
    /* [in] */ ISortedSet* set,
    /* [out] */ ISortedSet** result)
{
    VALIDATE_NOT_NULL(result)
    if (set == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }
    AutoPtr<ISortedSet> res = new SynchronizedSortedSet(set);
    *result = res;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode CCollections::_NewUnmodifiableCollection(
    /* [in] */ ICollection* collection,
    /* [out] */ ICollection** result)
{
    VALIDATE_NOT_NULL(result)
    if (collection == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }
    AutoPtr<ICollection> res = new UnmodifiableCollection(collection);
    *result = res;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode CCollections::_NewUnmodifiableList(
    /* [in] */ IList* list,
    /* [out] */ IList** result)
{
    VALIDATE_NOT_NULL(result)
    if (list == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }
    if (IRandomAccess::Probe(list) != NULL) {
        AutoPtr<IList> res = new UnmodifiableRandomAccessList(list);
        *result = res;
        REFCOUNT_ADD(*result)
        return NOERROR;
    }
    AutoPtr<IList> res = new UnmodifiableList(list);
    *result = res;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode CCollections::_NewUnmodifiableMap(
    /* [in] */ IMap* map,
    /* [out] */ IMap** result)
{
    VALIDATE_NOT_NULL(result)
    if (map == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }
    AutoPtr<IMap> res = new UnmodifiableMap(map);
    *result = res;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode CCollections::_NewUnmodifiableSet(
    /* [in] */ ISet* set,
    /* [out] */ ISet** result)
{
    VALIDATE_NOT_NULL(result)
    if (set == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }
    AutoPtr<ISet> res = new UnmodifiableSet(set);
    *result = res;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode CCollections::_NewUnmodifiableSortedMap(
    /* [in] */ ISortedMap* map,
    /* [out] */ ISortedMap** result)
{
    VALIDATE_NOT_NULL(result)
    if (map == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }
    AutoPtr<ISortedMap> res = new UnmodifiableSortedMap(map);
    *result = res;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode CCollections::_NewUnmodifiableSortedSet(
    /* [in] */ ISortedSet* set,
    /* [out] */ ISortedSet** result)
{
    VALIDATE_NOT_NULL(result)
    if (set == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }
    AutoPtr<ISortedSet> res = new UnmodifiableSortedSet(set);
    *result = res;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode CCollections::_Frequency(
    /* [in] */ ICollection* c,
    /* [in] */ IInterface* o,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)
    if (c == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }
    Boolean b;
    if ((c->IsEmpty(&b), b)) {
        *result = 0;
        return NOERROR;
    }
    Int32 num = 0;
    AutoPtr<IIterator> itr;
    c->GetIterator((IIterator**)&itr);
    while ((itr->HasNext(&b), b)) {
        AutoPtr<IInterface> e;
        itr->Next((IInterface**)&e);
        if ((o == NULL) ? e == NULL : ObjectUtils::Equals(o, e)) {
            num++;
        }
    }
    *result = num;
    return NOERROR;
}

ECode CCollections::_GetEmptyList(
    /* [out] */ IList** result)
{
    VALIDATE_NOT_NULL(result)
    *result = EMPTY_LIST;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode CCollections::_GetEmptySet(
    /* [out] */ ISet** result)
{
    VALIDATE_NOT_NULL(result)
    *result = EMPTY_SET;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode CCollections::_GetEmptyMap(
    /* [out] */ IMap** result)
{
    VALIDATE_NOT_NULL(result)
    *result = EMPTY_MAP;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode CCollections::_GetEmptyEnumeration(
    /* [out] */ IEnumeration** result)
{
    VALIDATE_NOT_NULL(result)
    *result = EMPTY_ENUMERATION;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode CCollections::_GetEmptyIterator(
    /* [out] */ IIterator** result)
{
    VALIDATE_NOT_NULL(result)
    *result = EMPTY_ITERATOR;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode CCollections::_GetEmptyListIterator(
    /* [out] */ IListIterator** result)
{
    VALIDATE_NOT_NULL(result)
    AutoPtr<IList> l;
    _GetEmptyList((IList**)&l);
    return l->GetListIterator(result);
}

ECode CCollections::_NewCheckedCollection(
    /* [in] */ ICollection* c,
    /* [in] */ const InterfaceID& type,
    /* [out] */ ICollection** result)
{
    VALIDATE_NOT_NULL(result)
    AutoPtr<ICollection> res = new CheckedCollection(c, type);
    *result = res;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode CCollections::_NewCheckedMap(
    /* [in] */ IMap* m,
    /* [in] */ const InterfaceID& keyType,
    /* [in] */ const InterfaceID& valueType,
    /* [out] */ IMap** result)
{
    VALIDATE_NOT_NULL(result)
    AutoPtr<IMap> res = new CheckedMap(m, keyType, valueType);
    *result = res;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode CCollections::_NewCheckedList(
    /* [in] */ IList* list,
    /* [in] */ const InterfaceID& type,
    /* [out] */ IList** result)
{
    VALIDATE_NOT_NULL(result)
    if (IRandomAccess::Probe(list) != NULL) {
        AutoPtr<IList> res = new CheckedRandomAccessList(list, type);
        *result = res;
        REFCOUNT_ADD(*result)
        return NOERROR;
    }
    AutoPtr<IList> res = new CheckedList(list, type);
    *result = res;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode CCollections::_NewCheckedSet(
    /* [in] */ ISet* s,
    /* [in] */ const InterfaceID& type,
    /* [out] */ ISet** result)
{
    VALIDATE_NOT_NULL(result)
    AutoPtr<ISet> res = new CheckedSet(s, type);
    *result = res;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode CCollections::_NewCheckedSortedMap(
    /* [in] */ ISortedMap* m,
    /* [in] */ const InterfaceID& keyType,
    /* [in] */ const InterfaceID& valueType,
    /* [out] */ ISortedMap** result)
{
    VALIDATE_NOT_NULL(result)
    AutoPtr<ISortedMap> res = new CheckedSortedMap(m, keyType, valueType);
    *result = res;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode CCollections::_NewCheckedSortedSet(
    /* [in] */ ISortedSet* s,
    /* [in] */ const InterfaceID& type,
    /* [out] */ ISortedSet** result)
{
    VALIDATE_NOT_NULL(result)
    AutoPtr<ISortedSet> res = new CheckedSortedSet(s, type);
    *result = res;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode CCollections::_AddAll(
    /* [in] */ ICollection* c,
    /* [in] */ ArrayOf<IInterface*>* a,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    Boolean modified = FALSE;
    for (Int32 i = 0; i < a->GetLength(); i++) {
        Boolean b;
        c->Add((*a)[i], &b);
        modified |= b;
    }
    *result = modified;
    return NOERROR;
}

ECode CCollections::_Disjoint(
    /* [in] */ ICollection* c1,
    /* [in] */ ICollection* c2,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    Int32 n1, n2;
    c1->GetSize(&n1);
    c2->GetSize(&n2);
    if (((ISet::Probe(c1) != NULL) && (ISet::Probe(c2) == NULL))
            || (n2 > n1)) {
        AutoPtr<ICollection> tmp = c1;
        c1 = c2;
        c2 = tmp;
    }
    AutoPtr<IIterator> it;
    c1->GetIterator((IIterator**)&it);
    Boolean b;
    while ((it->HasNext(&b), b)) {
        AutoPtr<IInterface> o;
        it->Next((IInterface**)&o);
        Boolean a;
        if ((c2->Contains(o, &a), a)) {
            *result = FALSE;
            return NOERROR;
        }
    }
    *result = TRUE;
    return NOERROR;
}

ECode CCollections::_CheckType(
    /* [in] */ IInterface* obj,
    /* [in] */ const InterfaceID& type)
{
    if (obj != NULL && obj->Probe(type) == NULL) {
        // throw new ClassCastException("Attempt to insert element of type " + obj.getClass() +
        //         " into collection of type " + type);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

ECode CCollections::_NewSetFromMap(
    /* [in] */ IMap* map,
    /* [out] */ ISet** result)
{
    VALIDATE_NOT_NULL(result)
    Boolean isEmpty = FALSE;
    if ((map->IsEmpty(&isEmpty), isEmpty)) {
        AutoPtr<ISet> res = new SetFromMap(map);
        *result = res;
        REFCOUNT_ADD(*result)
        return NOERROR;
    }
    return E_ILLEGAL_ARGUMENT_EXCEPTION;
}

ECode CCollections::_NewAsLifoQueue(
    /* [in] */ IDeque* deque,
    /* [out] */ IQueue** result)
{
    VALIDATE_NOT_NULL(result)
    AutoPtr<IQueue> res = new AsLIFOQueue(deque);
    *result = res;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode CCollections::BinarySearch(
    /* [in] */ IList* list,
    /* [in] */ IInterface* object,
    /* [out] */ Int32* index)
{
    return _BinarySearch(list, object, index);
}

ECode CCollections::BinarySearch(
    /* [in] */ IList* list,
    /* [in] */ IInterface* object,
    /* [in] */ IComparator* comparator,
    /* [out] */ Int32* index)
{
    return _BinarySearch(list, object, comparator, index);
}

ECode CCollections::Copy(
    /* [in] */ IList* destination,
    /* [in] */ IList* source)
{
    return _Copy(destination, source);
}

ECode CCollections::NewEnumeration(
    /* [in] */ ICollection* collection,
    /* [out] */ IEnumeration** result)
{
    return _NewEnumeration(collection, result);
}

ECode CCollections::Fill(
    /* [in] */ IList* list,
    /* [in] */ IInterface* object)
{
    return _Fill(list, object);
}

ECode CCollections::Max(
    /* [in] */ ICollection* collection,
    /* [out] */ IInterface** result)
{
    return _Max(collection, result);
}

ECode CCollections::Max(
    /* [in] */ ICollection* collection,
    /* [in] */ IComparator* comparator,
    /* [out] */ IInterface** result)
{
    return _Max(collection, comparator, result);
}

ECode CCollections::Min(
    /* [in] */ ICollection* collection,
    /* [out] */ IInterface** result)
{
    return _Min(collection, result);
}

ECode CCollections::Min(
    /* [in] */ ICollection* collection,
    /* [in] */ IComparator* comparator,
    /* [out] */ IInterface** result)
{
    return _Min(collection, comparator, result);
}

ECode CCollections::NCopies(
    /* [in] */ Int32 length,
    /* [in] */ IInterface* object,
    /* [out] */ IList** result)
{
    return _NCopies(length, object, result);
}

ECode CCollections::Reverse(
    /* [in] */ IList* list)
{
    return _Reverse(list);
}

ECode CCollections::ReverseOrder(
    /* [out] */ IComparator** result)
{
    return _ReverseOrder(result);
}

ECode CCollections::ReverseOrder(
    /* [in] */ IComparator* c,
    /* [out] */ IComparator** result)
{
    return _ReverseOrder(c, result);
}

ECode CCollections::Shuffle(
    /* [in] */ IList* list)
{
    return _Shuffle(list);
}

ECode CCollections::Shuffle(
    /* [in] */ IList* list,
    /* [in] */ IRandom* random)
{
    return _Shuffle(list, random);
}

ECode CCollections::NewSingleton(
    /* [in] */ IInterface* object,
    /* [out] */ ISet** result)
{
    return _NewSingleton(object, result);
}

ECode CCollections::NewSingletonList(
    /* [in] */ IInterface* object,
    /* [out] */ IList** result)
{
    return _NewSingletonList(object, result);
}

ECode CCollections::NewSingletonMap(
    /* [in] */ IInterface* key,
    /* [in] */ IInterface* value,
    /* [out] */ IMap** result)
{
    return _NewSingletonMap(key, value, result);
}

ECode CCollections::Sort(
    /* [in] */ IList* list)
{
    return _Sort(list);
}

ECode CCollections::Sort(
    /* [in] */ IList* list,
    /* [in] */ IComparator* comparator)
{
    return _Sort(list, comparator);
}

ECode CCollections::Swap(
    /* [in] */ IList* list,
    /* [in] */ Int32 index1,
    /* [in] */ Int32 index2)
{
    return _Swap(list, index1, index2);
}

ECode CCollections::ReplaceAll(
    /* [in] */ IList* list,
    /* [in] */ IInterface* obj,
    /* [in] */ IInterface* obj2,
    /* [out] */ Boolean* result)
{
    return _ReplaceAll(list, obj, obj2, result);
}

ECode CCollections::Rotate(
    /* [in] */ IList* lst,
    /* [in] */ Int32 dist)
{
    return _Rotate(lst, dist);
}

ECode CCollections::IndexOfSubList(
    /* [in] */ IList* list,
    /* [in] */ IList* sublist,
    /* [out] */ Int32* result)
{
    return _IndexOfSubList(list, sublist, result);
}

ECode CCollections::LastIndexOfSubList(
    /* [in] */ IList* list,
    /* [in] */ IList* sublist,
    /* [out] */ Int32* result)
{
    return _LastIndexOfSubList(list, sublist, result);
}

ECode CCollections::NewList(
    /* [in] */ IEnumeration* enumeration,
    /* [out] */ IArrayList** result)
{
    return _NewList(enumeration, result);
}

ECode CCollections::NewSynchronizedCollection(
    /* [in] */ ICollection* collection,
    /* [out] */ ICollection** result)
{
    return _NewSynchronizedCollection(collection, result);
}

ECode CCollections::NewSynchronizedList(
    /* [in] */ IList* list,
    /* [out] */ IList** result)
{
    return _NewSynchronizedList(list, result);
}

ECode CCollections::NewSynchronizedMap(
    /* [in] */ IMap* map,
    /* [out] */ IMap** result)
{
    return _NewSynchronizedMap(map, result);
}

ECode CCollections::NewSynchronizedSet(
    /* [in] */ ISet* set,
    /* [out] */ ISet** result)
{
    return _NewSynchronizedSet(set, result);
}

ECode CCollections::NewSynchronizedSortedMap(
    /* [in] */ ISortedMap* map,
    /* [out] */ ISortedMap** result)
{
    return _NewSynchronizedSortedMap(map, result);
}

ECode CCollections::NewSynchronizedSortedSet(
    /* [in] */ ISortedSet* set,
    /* [out] */ ISortedSet** result)
{
    return _NewSynchronizedSortedSet(set, result);
}

ECode CCollections::NewUnmodifiableCollection(
    /* [in] */ ICollection* collection,
    /* [out] */ ICollection** result)
{
    return _NewUnmodifiableCollection(collection, result);
}

ECode CCollections::NewUnmodifiableList(
    /* [in] */ IList* list,
    /* [out] */ IList** result)
{
    return _NewUnmodifiableList(list, result);
}

ECode CCollections::NewUnmodifiableMap(
    /* [in] */ IMap* map,
    /* [out] */ IMap** result)
{
    return _NewUnmodifiableMap(map, result);
}

ECode CCollections::NewUnmodifiableSet(
    /* [in] */ ISet* set,
    /* [out] */ ISet** result)
{
    return _NewUnmodifiableSet(set, result);
}

ECode CCollections::NewUnmodifiableSortedMap(
    /* [in] */ ISortedMap* map,
    /* [out] */ ISortedMap** result)
{
    return _NewUnmodifiableSortedMap(map, result);
}

ECode CCollections::NewUnmodifiableSortedSet(
    /* [in] */ ISortedSet* set,
    /* [out] */ ISortedSet** result)
{
    return _NewUnmodifiableSortedSet(set, result);
}

ECode CCollections::Frequency(
    /* [in] */ ICollection* c,
    /* [in] */ IInterface* o,
    /* [out] */ Int32* result)
{
    return _Frequency(c, o, result);
}

ECode CCollections::GetEmptyList(
    /* [out] */ IList** result)
{
    return _GetEmptyList(result);
}

ECode CCollections::GetEmptySet(
    /* [out] */ ISet** result)
{
    return _GetEmptySet(result);
}

ECode CCollections::GetEmptyMap(
    /* [out] */ IMap** result)
{
    return _GetEmptyMap(result);
}

ECode CCollections::GetEmptyEnumeration(
    /* [out] */ IEnumeration** result)
{
    return _GetEmptyEnumeration(result);
}

ECode CCollections::GetEmptyIterator(
    /* [out] */ IIterator** result)
{
    return _GetEmptyIterator(result);
}

ECode CCollections::GetEmptyListIterator(
    /* [out] */ IListIterator** result)
{
    return _GetEmptyListIterator(result);
}

ECode CCollections::NewCheckedCollection(
    /* [in] */ ICollection* c,
    /* [in] */ const InterfaceID& type,
    /* [out] */ ICollection** result)
{
    return _NewCheckedCollection(c, type, result);
}

ECode CCollections::NewCheckedMap(
    /* [in] */ IMap* m,
    /* [in] */ const InterfaceID& keyType,
    /* [in] */ const InterfaceID& valueType,
    /* [out] */ IMap** result)
{
    return _NewCheckedMap(m, keyType, valueType, result);
}

ECode CCollections::NewCheckedList(
    /* [in] */ IList* list,
    /* [in] */ const InterfaceID& type,
    /* [out] */ IList** result)
{
    return _NewCheckedList(list, type, result);
}

ECode CCollections::NewCheckedSet(
    /* [in] */ ISet* s,
    /* [in] */ const InterfaceID& type,
    /* [out] */ ISet** result)
{
    return _NewCheckedSet(s, type, result);
}

ECode CCollections::NewCheckedSortedMap(
    /* [in] */ ISortedMap* m,
    /* [in] */ const InterfaceID& keyType,
    /* [in] */ const InterfaceID& valueType,
    /* [out] */ ISortedMap** result)
{
    return _NewCheckedSortedMap(m, keyType, valueType, result);
}

ECode CCollections::NewCheckedSortedSet(
    /* [in] */ ISortedSet* s,
    /* [in] */ const InterfaceID& type,
    /* [out] */ ISortedSet** result)
{
    return _NewCheckedSortedSet(s, type, result);
}

ECode CCollections::AddAll(
    /* [in] */ ICollection* c,
    /* [in] */ ArrayOf<IInterface*>* a,
    /* [out] */ Boolean* result)
{
    return _AddAll(c, a, result);
}

ECode CCollections::Disjoint(
    /* [in] */ ICollection* c1,
    /* [in] */ ICollection* c2,
    /* [out] */ Boolean* result)
{
    return _Disjoint(c1, c2, result);
}

ECode CCollections::CheckType(
    /* [in] */ IInterface* obj,
    /* [in] */ const InterfaceID& type)
{
    return _CheckType(obj, type);
}

ECode CCollections::NewSetFromMap(
    /* [in] */ IMap* map,
    /* [out] */ ISet** result)
{
    return _NewSetFromMap(map, result);
}

ECode CCollections::NewAsLifoQueue(
    /* [in] */ IDeque* deque,
    /* [out] */ IQueue** result)
{
    return _NewAsLifoQueue(deque, result);
}

//====================================================================
// CCollections::SetFromMap::
//====================================================================

UInt32 CCollections::SetFromMap::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CCollections::SetFromMap::Release()
{
    return ElRefBase::Release();
}

PInterface CCollections::SetFromMap::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_IInterface == riid) {
        return (PInterface)(ISet*)this;
    }
    else if (EIID_IIterable == riid) {
        return (IIterable*)(ISet*)this;
    }
    else if (EIID_ICollection == riid) {
        return (ICollection*)(ISet*)this;
    }
    else if (EIID_ISet == riid) {
        return (ISet*)this;
    }
    else if (Elastos::IO::EIID_ISerializable == riid) {
        return (ISerializable*)this;
    }

    return NULL;
}

ECode CCollections::SetFromMap::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID)

    if (pObject == (IInterface*)(ISet*)this) {
        *pIID = EIID_ISet;
    }
    else if (pObject == (IInterface*)(ISerializable*)this) {
        *pIID = Elastos::IO::EIID_ISerializable;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

CCollections::SetFromMap::SetFromMap(
    /* [in] */ IMap* map)
{
    mM = map;
    map->KeySet((ISet**)&mBackingSet);
}

ECode CCollections::SetFromMap::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)
    return mBackingSet->Equals(object, value);
}

ECode CCollections::SetFromMap::GetHashCode(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)
    return mBackingSet->GetHashCode(value);
}

ECode CCollections::SetFromMap::Add(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    VALIDATE_NOT_NULL(modified)
    AutoPtr<IBoolean> value;
    CBoolean::New(TRUE, (IBoolean**)&value);
    AutoPtr<IInterface> oldValue;
    mM->Put(object, value, (IInterface**)&oldValue);
    *modified = oldValue == NULL;
    return NOERROR;
}

ECode CCollections::SetFromMap::Clear()
{
    return mM->Clear();
}

ECode CCollections::SetFromMap::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    AutoPtr<IObject> iObj;
    iObj = IObject::Probe(mBackingSet);
    if (iObj != NULL) {
        return iObj->ToString(str);
    }
    *str = NULL;
    return NOERROR;
}

ECode CCollections::SetFromMap::Contains(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return mBackingSet->Contains(object, result);
}

ECode CCollections::SetFromMap::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return mBackingSet->ContainsAll(collection, result);
}

ECode CCollections::SetFromMap::IsEmpty(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return mM->IsEmpty(result);
}

ECode CCollections::SetFromMap::Remove(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    AutoPtr<IInterface> value;
    FAIL_RETURN(mM->Remove(object, (IInterface**)&value));
    *result = value != NULL;
    return  NOERROR;
}

ECode CCollections::SetFromMap::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return mBackingSet->RetainAll(collection, result);
}

ECode CCollections::SetFromMap::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    VALIDATE_NOT_NULL(array)
    return mBackingSet->ToArray(array);
}

ECode CCollections::SetFromMap::ToArray(
    /* [in] */ ArrayOf<IInterface*>* contents,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    VALIDATE_NOT_NULL(outArray)
    return mBackingSet->ToArray(contents, outArray);
}

ECode CCollections::SetFromMap::GetIterator(
    /* [out] */ IIterator** result)
{
    VALIDATE_NOT_NULL(result)
    return mBackingSet->GetIterator(result);
}

ECode CCollections::SetFromMap::GetSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size)
    return mM->GetSize(size);
}

ECode CCollections::SetFromMap::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::AddAll(collection, modified);
}

ECode CCollections::SetFromMap::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::RemoveAll(collection, modified);
}

ECode CCollections::SetFromMap::ReadObject(
    /* [in] */ IObjectInputStream* stream)
{
    stream->DefaultReadObject();
    return mM->KeySet((ISet**)&mBackingSet);
}

//====================================================================
// CCollections::AsLIFOQueue::
//====================================================================

UInt32 CCollections::AsLIFOQueue::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CCollections::AsLIFOQueue::Release()
{
    return ElRefBase::Release();
}

PInterface CCollections::AsLIFOQueue::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_IInterface == riid) {
        return (PInterface)(IQueue*)this;
    }
    else if (EIID_IIterable == riid) {
        return (IIterable*)(IQueue*)this;
    }
    else if (EIID_ICollection == riid) {
        return (ICollection*)(IQueue*)this;
    }
    else if (EIID_IQueue == riid) {
        return (IQueue*)this;
    }
    else if (Elastos::IO::EIID_ISerializable == riid) {
        return (ISerializable*)this;
    }

    return NULL;
}

ECode CCollections::AsLIFOQueue::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID)

    if (pObject == (IInterface*)(IQueue*)this) {
        *pIID = EIID_IQueue;
    }
    else if (pObject == (IInterface*)(ISerializable*)this) {
        *pIID = Elastos::IO::EIID_ISerializable;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

CCollections::AsLIFOQueue::AsLIFOQueue(
    /* [in] */ IDeque* deque)
{
    mQ = deque;
}

ECode CCollections::AsLIFOQueue::GetIterator(
    /* [out] */ IIterator** result)
{
    VALIDATE_NOT_NULL(result)
    return mQ->GetIterator(result);
}

ECode CCollections::AsLIFOQueue::GetSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size)
    return mQ->GetSize(size);
}

ECode CCollections::AsLIFOQueue::Offer(
    /* [in] */ IInterface* obj,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return mQ->OfferFirst(obj, result);
}

ECode CCollections::AsLIFOQueue::Peek(
    /* [out] */ IInterface** obj)
{
    VALIDATE_NOT_NULL(obj)
    return mQ->PeekFirst(obj);
}

ECode CCollections::AsLIFOQueue::Poll(
    /* [out] */ IInterface** obj)
{
    VALIDATE_NOT_NULL(obj)
    return mQ->PollFirst(obj);
}

ECode CCollections::AsLIFOQueue::Add(
    /* [in] */ IInterface* obj,
    /* [out] */ Boolean* result)
{
    return mQ->Add(obj, result);
}

ECode CCollections::AsLIFOQueue::Clear()
{
    return mQ->Clear();
}

ECode CCollections::AsLIFOQueue::Element(
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object)
    return mQ->GetFirst(object);
}

ECode CCollections::AsLIFOQueue::Remove(
    /* [out] */ IInterface** obj)
{
    VALIDATE_NOT_NULL(obj)
    return mQ->Pop(obj);
}

ECode CCollections::AsLIFOQueue::Contains(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return mQ->Contains(object, result);
}

ECode CCollections::AsLIFOQueue::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return mQ->ContainsAll(collection, result);
}

ECode CCollections::AsLIFOQueue::IsEmpty(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return mQ->IsEmpty(result);
}

ECode CCollections::AsLIFOQueue::Remove(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return mQ->Remove(object, result);
}

ECode CCollections::AsLIFOQueue::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return mQ->RemoveAll(collection, result);
}

ECode CCollections::AsLIFOQueue::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return mQ->RetainAll(collection, result);
}

ECode CCollections::AsLIFOQueue::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    VALIDATE_NOT_NULL(array)
    return mQ->ToArray(array);
}

ECode CCollections::AsLIFOQueue::ToArray(
    /* [in] */ ArrayOf<IInterface*>* contents,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    VALIDATE_NOT_NULL(outArray)
    return mQ->ToArray(contents, outArray);
}

ECode CCollections::AsLIFOQueue::ToString(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result)
    AutoPtr<IObject> iObj;
    iObj = IObject::Probe(mQ);
    if (iObj != NULL) {
        return iObj->ToString(result);
    }
    *result = NULL;
    return NOERROR;
}

ECode CCollections::AsLIFOQueue::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractQueue::AddAll(collection, modified);
}

ECode CCollections::AsLIFOQueue::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = this->Probe(EIID_IInterface) == object;
    return NOERROR;
}

ECode CCollections::AsLIFOQueue::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode)
    *hashCode = (Int32)this;
    return NOERROR;
}

//====================================================================
// CCollections::CheckedCollection::
//====================================================================

UInt32 CCollections::CheckedCollection::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CCollections::CheckedCollection::Release()
{
    return ElRefBase::Release();
}

PInterface CCollections::CheckedCollection::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_IInterface == riid) {
        return (PInterface)(ICollection*)this;
    }
    else if (EIID_IIterable == riid) {
        return (IIterable*)(ICollection*)this;
    }
    else if (EIID_ICollection == riid) {
        return (ICollection*)this;
    }
    else if (Elastos::IO::EIID_ISerializable == riid) {
        return (ISerializable*)this;
    }

    return NULL;
}

ECode CCollections::CheckedCollection::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID)

    if (pObject == (IInterface*)(ICollection*)this) {
        *pIID = EIID_ICollection;
    }
    else if (pObject == (IInterface*)(ISerializable*)this) {
        *pIID = Elastos::IO::EIID_ISerializable;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

CCollections::CheckedCollection::CheckedCollection(
    /* [in] */ ICollection* c,
    /* [in] */ InterfaceID type)
{
   if (c == NULL) {
       //throw new NullPointerException("c == null");
       return;
   }
//   else if (type == NULL) {
       //throw new NullPointerException("type == null");
//   }
   mC = c;
   mType = type;
}

ECode CCollections::CheckedCollection::GetSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size)
    return mC->GetSize(size);
}

ECode CCollections::CheckedCollection::IsEmpty(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return mC->IsEmpty(result);
}

ECode CCollections::CheckedCollection::Contains(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return mC->Contains(object, result);
}

ECode CCollections::CheckedCollection::GetIterator(
    /* [out] */ IIterator** result)
{
    VALIDATE_NOT_NULL(result)
    AutoPtr<IIterator> i;
    mC->GetIterator((IIterator**)&i);
    AutoPtr<IListIterator> res = IListIterator::Probe(i);
    if (res != NULL) {
        i = new CheckedListIterator(res, mType);
    }
    *result = i;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode CCollections::CheckedCollection::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    VALIDATE_NOT_NULL(array)
    return mC->ToArray(array);
}

ECode CCollections::CheckedCollection::ToArray(
    /* [in] */ ArrayOf<IInterface*>* inArray,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    VALIDATE_NOT_NULL(outArray)
    return mC->ToArray(inArray, outArray);
}

ECode CCollections::CheckedCollection::Add(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    VALIDATE_NOT_NULL(modified)
    return mC->Add(object, modified);
}

ECode CCollections::CheckedCollection::Remove(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    VALIDATE_NOT_NULL(modified)
    return mC->Remove(object, modified);
}

ECode CCollections::CheckedCollection::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return mC->ContainsAll(collection, result);
}

ECode CCollections::CheckedCollection::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    VALIDATE_NOT_NULL(modified)
    AutoPtr<ArrayOf<IInterface*> > array;
    collection->ToArray((ArrayOf<IInterface*>**)&array);
    for (Int32 i = 0; i < array->GetLength(); ++i) {
        AutoPtr<IInterface> o = (*array)[i];
        FAIL_RETURN(CCollections::_CheckType(o, mType));
    }
    return mC->AddAll(collection, modified);
}

ECode CCollections::CheckedCollection::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    VALIDATE_NOT_NULL(modified)
    return mC->RemoveAll(collection, modified);
}

ECode CCollections::CheckedCollection::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    VALIDATE_NOT_NULL(modified)
    return mC->RetainAll(collection, modified);
}

ECode CCollections::CheckedCollection::Clear()
{
    return mC->Clear();
}

ECode CCollections::CheckedCollection::ToString(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result)
    AutoPtr<IObject> iObj = IObject::Probe(mC);
    if (iObj != NULL) {
        return iObj->ToString(result);
    }
    *result = NULL;
    return NOERROR;
}

ECode CCollections::CheckedCollection::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = this->Probe(EIID_IInterface) == object;
    return NOERROR;
}

ECode CCollections::CheckedCollection::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode)
    *hashCode = (Int32)this;
    return NOERROR;
}

//====================================================================
// CCollections::CheckedListIterator::
//====================================================================

UInt32 CCollections::CheckedListIterator::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CCollections::CheckedListIterator::Release()
{
    return ElRefBase::Release();
}

PInterface CCollections::CheckedListIterator::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_IInterface == riid) {
        return (PInterface)(IList*)this;
    }
    else if (EIID_IIterable == riid) {
        return (IIterable*)(IList*)this;
    }
    else if (EIID_IListIterator == riid) {
        return (IListIterator*)this;
    }

    return NULL;
}

ECode CCollections::CheckedListIterator::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID)

    if (pObject == (IInterface*)(IListIterator*)this) {
        *pIID = EIID_IListIterator;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

CCollections::CheckedListIterator::CheckedListIterator(
    /* [in] */ IListIterator* i,
    /* [in] */ InterfaceID type)
{
   mI = i;
   mType = type;
}

ECode CCollections::CheckedListIterator::HasNext(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return mI->HasNext(result);
}

ECode CCollections::CheckedListIterator::Next(
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object)
    return mI->Next(object);
}

ECode CCollections::CheckedListIterator::Remove()
{
    return mI->Remove();
}

ECode CCollections::CheckedListIterator::HasPrevious(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return mI->HasPrevious(result);
}

ECode CCollections::CheckedListIterator::Previous(
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(*object)
    return mI->Previous(object);
}

ECode CCollections::CheckedListIterator::NextIndex(
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index)
    return mI->NextIndex(index);
}

ECode CCollections::CheckedListIterator::PreviousIndex(
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index)
    return mI->PreviousIndex(index);
}

ECode CCollections::CheckedListIterator::Set(
    /* [in] */ IInterface* object)
{
    FAIL_RETURN(CCollections::_CheckType(object, mType));
    return mI->Set(object);
}

ECode CCollections::CheckedListIterator::Add(
    /* [in] */ IInterface* object)
{
    FAIL_RETURN(CCollections::_CheckType(object, mType));
    return mI->Add(object);
}

//====================================================================
// CCollections::CheckedList::
//====================================================================

UInt32 CCollections::CheckedList::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CCollections::CheckedList::Release()
{
    return ElRefBase::Release();
}

PInterface CCollections::CheckedList::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_IInterface == riid) {
        return (PInterface)(IList*)this;
    }
    else if (EIID_IIterable == riid) {
        return (IIterable*)(IList*)this;
    }
    else if (EIID_ICollection == riid) {
        return (ICollection*)(IList*)this;
    }
    else if (EIID_IList == riid) {
        return (IList*)this;
    }
    else if (Elastos::IO::EIID_ISerializable == riid) {
        return (ISerializable*)this;
    }

    return NULL;
}

ECode CCollections::CheckedList::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID)

    if (pObject == (IInterface*)(IList*)this) {
        *pIID = EIID_IList;
    }
    else if (pObject == (IInterface*)(ISerializable*)this) {
        *pIID = Elastos::IO::EIID_ISerializable;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

CCollections::CheckedList::CheckedList(
    /* [in] */ IList* l,
    /* [in] */ InterfaceID type)
    : CheckedCollection(l, type)
{
    mL = l;
}

ECode CCollections::CheckedList::AddAll(
    /* [in] */ Int32 location,
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    AutoPtr<ArrayOf<IInterface*> > array;
    collection->ToArray((ArrayOf<IInterface*>**)&array);
    for (Int32 i = 0; i < array->GetLength();i++) {
        AutoPtr<IInterface> o = (*array)[i];
        FAIL_RETURN(CCollections::_CheckType(o, mType));
    }
    return mL->AddAll(location, collection, modified);
}

ECode CCollections::CheckedList::Get(
    /* [in] */ Int32 location,
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object)
    return mL->Get(location, object);
}

ECode CCollections::CheckedList::Set(
    /* [in] */ Int32 location,
    /* [in] */ IInterface* object,
    /* [out] */ IInterface** prevObject)
{
    VALIDATE_NOT_NULL(prevObject)
    FAIL_RETURN(CCollections::_CheckType(object, mType));
    return mL->Set(location, object, prevObject);
}

ECode CCollections::CheckedList::Add(
    /* [in] */ Int32 location,
    /* [in] */ IInterface* object)
{
    FAIL_RETURN(CCollections::_CheckType(object, mType));
    return mL->Add(location, object);
}

ECode CCollections::CheckedList::Remove(
    /* [in] */ Int32 location,
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object)
    return mL->Remove(location, object);
}

ECode CCollections::CheckedList::IndexOf(
    /* [in] */ IInterface* object,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index)
    return mL->IndexOf(object, index);
}

ECode CCollections::CheckedList::LastIndexOf(
    /* [in] */ IInterface* object,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index)
    return mL->LastIndexOf(object, index);
}

ECode CCollections::CheckedList::GetListIterator(
    /* [out] */ IListIterator** it)
{
    VALIDATE_NOT_NULL(it)
    AutoPtr<IListIterator> iterator;
    mL->GetListIterator((IListIterator**)&iterator);
    AutoPtr<IListIterator> res = new CheckedListIterator(iterator, mType);
    *it = res;
    REFCOUNT_ADD(*it)
    return NOERROR;
}

ECode CCollections::CheckedList::GetListIterator(
    /* [in] */ Int32 location,
    /* [out] */ IListIterator** it)
{
    VALIDATE_NOT_NULL(it)
    AutoPtr<IListIterator> iterator;
    mL->GetListIterator(location, (IListIterator**)&iterator);
    AutoPtr<IListIterator> res = new CheckedListIterator(iterator, mType);
    *it = res;
    REFCOUNT_ADD(*it)
    return NOERROR;
}

ECode CCollections::CheckedList::SubList(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ IList** subList)
{
    VALIDATE_NOT_NULL(subList)
    AutoPtr<IList> l;
    mL->SubList(start, end, (IList**)&l);
    CheckedList(l, mType);
    *subList = l;
    REFCOUNT_ADD(*subList)
    return NOERROR;
}

ECode CCollections::CheckedList::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return mL->Equals(object, result);
}

ECode CCollections::CheckedList::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode)
    return mL->GetHashCode(hashCode);
}

ECode CCollections::CheckedList::Contains(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return CheckedCollection::Contains(object, result);
}

ECode CCollections::CheckedList::GetSize(
    /* [out] */ Int32* size)
{
    return CheckedCollection::GetSize(size);
}

ECode CCollections::CheckedList::GetIterator(
    /* [out] */ IIterator** it)
{
    return CheckedCollection::GetIterator(it);
}

ECode CCollections::CheckedList::Add(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return CheckedCollection::Add(object, modified);
}

ECode CCollections::CheckedList::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return CheckedCollection::AddAll(collection, modified);
}

ECode CCollections::CheckedList::Clear()
{
    return CheckedCollection::Clear();
}

ECode CCollections::CheckedList::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return CheckedCollection::ContainsAll(collection, result);
}

ECode CCollections::CheckedList::IsEmpty(
    /* [out] */ Boolean* result)
{
    return CheckedCollection::IsEmpty(result);
}

ECode CCollections::CheckedList::Remove(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return CheckedCollection::Remove(object, modified);
}

ECode CCollections::CheckedList::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return CheckedCollection::RemoveAll(collection, modified);
}

ECode CCollections::CheckedList::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return CheckedCollection::RetainAll(collection, modified);
}

ECode CCollections::CheckedList::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    return CheckedCollection::ToArray(array);
}

ECode CCollections::CheckedList::ToArray(
    /* [in] */ ArrayOf<IInterface*>* inArray,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    return CheckedCollection::ToArray(inArray, outArray);
}


//====================================================================
// CCollections::CheckedRandomAccessList::
//====================================================================

UInt32 CCollections::CheckedRandomAccessList::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CCollections::CheckedRandomAccessList::Release()
{
    return ElRefBase::Release();
}

PInterface CCollections::CheckedRandomAccessList::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_IRandomAccess == riid) {
        return (IRandomAccess*)this;
    }
    return CheckedList::Probe(riid);
}

ECode CCollections::CheckedRandomAccessList::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID)

    if (pObject == (IInterface*)(IRandomAccess*)this) {
        *pIID = EIID_IRandomAccess;
    }
    return CheckedList::GetInterfaceID(pObject, pIID);
}

CCollections::CheckedRandomAccessList::CheckedRandomAccessList(
    /* [in] */ IList* l,
    /* [in] */ InterfaceID type)
    : CheckedList(l, type)
{
}

//====================================================================
// CCollections::CheckedSet::
//====================================================================

UInt32 CCollections::CheckedSet::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CCollections::CheckedSet::Release()
{
    return ElRefBase::Release();
}

PInterface CCollections::CheckedSet::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_IInterface == riid) {
        return (PInterface)(ISet*)this;
    }
    else if (EIID_IIterable == riid) {
        return (IIterable*)(ISet*)this;
    }
    else if (EIID_ICollection == riid) {
        return (ICollection*)(ISet*)this;
    }
    else if (EIID_ISet == riid) {
        return (ISet*)this;
    }
    else if (Elastos::IO::EIID_ISerializable == riid) {
        return (ISerializable*)this;
    }

    return NULL;
}

ECode CCollections::CheckedSet::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID)

    if (pObject == (IInterface*)(ISet*)this) {
        *pIID = EIID_ISet;
    }
    else if (pObject == (IInterface*)(ISerializable*)this) {
        *pIID = Elastos::IO::EIID_ISerializable;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

CCollections::CheckedSet::CheckedSet(
    /* [in] */ ISet* s,
    /* [in] */ InterfaceID type)
    : CheckedCollection(s, type)
{
}

ECode CCollections::CheckedSet::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return mC->Equals(object, result);
}

ECode CCollections::CheckedSet::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode)
    return mC->GetHashCode(hashCode);
}

ECode CCollections::CheckedSet::Contains(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return CheckedCollection::Contains(object, result);
}

ECode CCollections::CheckedSet::GetSize(
    /* [out] */ Int32* size)
{
    return CheckedCollection::GetSize(size);
}

ECode CCollections::CheckedSet::GetIterator(
    /* [out] */ IIterator** it)
{
    return CheckedCollection::GetIterator(it);
}

ECode CCollections::CheckedSet::Add(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return CheckedCollection::Add(object, modified);
}

ECode CCollections::CheckedSet::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return CheckedCollection::AddAll(collection, modified);
}

ECode CCollections::CheckedSet::Clear()
{
    return CheckedCollection::Clear();
}

ECode CCollections::CheckedSet::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return CheckedCollection::ContainsAll(collection, result);
}

ECode CCollections::CheckedSet::IsEmpty(
    /* [out] */ Boolean* result)
{
    return CheckedCollection::IsEmpty(result);
}

ECode CCollections::CheckedSet::Remove(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return CheckedCollection::Remove(object, modified);
}

ECode CCollections::CheckedSet::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return CheckedCollection::RemoveAll(collection, modified);
}

ECode CCollections::CheckedSet::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return CheckedCollection::RetainAll(collection, modified);
}

ECode CCollections::CheckedSet::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    return CheckedCollection::ToArray(array);
}

ECode CCollections::CheckedSet::ToArray(
    /* [in] */ ArrayOf<IInterface*>* inArray,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    return CheckedCollection::ToArray(inArray, outArray);
}

//====================================================================
// CCollections::CheckedMap::CheckedEntry::
//====================================================================
CAR_INTERFACE_IMPL(CCollections::CheckedMap::CheckedEntry, IMapEntry)

CCollections::CheckedMap::CheckedEntry::CheckedEntry(
    /* [in] */ IMapEntry* e,
    /* [in] */ InterfaceID valueType)
{
    if (e == NULL) {
        //throw new NullPointerException("e == null");
        return;
    }
    mE = e;
    mValueType = valueType;
}

ECode CCollections::CheckedMap::CheckedEntry::GetKey(
    /* [out] */ PInterface* key)
{
    VALIDATE_NOT_NULL(key)
    return mE->GetKey(key);
}

ECode CCollections::CheckedMap::CheckedEntry::GetValue(
    /* [out] */ PInterface* value)
{
    VALIDATE_NOT_NULL(value)
    return mE->GetValue(value);
}

ECode CCollections::CheckedMap::CheckedEntry::SetValue(
    /* [in] */ IInterface* valueReplacer,
    /* [out] */ IInterface** valueReplacee)
{
    VALIDATE_NOT_NULL(valueReplacee)
    FAIL_RETURN(CCollections::_CheckType(valueReplacer, mValueType));
    return mE->SetValue(valueReplacer, valueReplacee);
}

ECode CCollections::CheckedMap::CheckedEntry::Equals(
    /* [in] */ IInterface* entry,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return mE->Equals(entry, result);
}

ECode CCollections::CheckedMap::CheckedEntry::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode)
    return mE->GetHashCode(hashCode);
}

//====================================================================
// CCollections::CheckedMap::CheckedEntrySet::CheckedEntryIterator::
//====================================================================
CAR_INTERFACE_IMPL(CCollections::CheckedMap::CheckedEntrySet::CheckedEntryIterator, IIterator)

CCollections::CheckedMap::CheckedEntrySet::CheckedEntryIterator::CheckedEntryIterator(
    /* [in] */ IIterator* i,
    /* [in] */ InterfaceID valueType)
{
    mI = i;
    mValueType = valueType;
}

ECode CCollections::CheckedMap::CheckedEntrySet::CheckedEntryIterator::HasNext(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return mI->HasNext(result);
}

ECode CCollections::CheckedMap::CheckedEntrySet::CheckedEntryIterator::Remove()
{
    return mI->Remove();
}

ECode CCollections::CheckedMap::CheckedEntrySet::CheckedEntryIterator::Next(
    /* [out] */ IMapEntry** object)
{
    VALIDATE_NOT_NULL(object)
    AutoPtr<IInterface> o;
    mI->Next((IInterface**)&o);
    AutoPtr<IMapEntry> res = new CheckedEntry(IMapEntry::Probe(o), mValueType);
    *object = res;
    REFCOUNT_ADD(*object)
    return NOERROR;
}

ECode CCollections::CheckedMap::CheckedEntrySet::CheckedEntryIterator::Next(
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object)
    AutoPtr<IMapEntry> res;
    Next((IMapEntry**)&res);
    *object = res->Probe(EIID_IInterface);
    REFCOUNT_ADD(*object)
    return NOERROR;
}

//====================================================================
// CCollections::CheckedMap::CheckedEntrySet::
//====================================================================

UInt32 CCollections::CheckedMap::CheckedEntrySet::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CCollections::CheckedMap::CheckedEntrySet::Release()
{
    return ElRefBase::Release();
}

PInterface CCollections::CheckedMap::CheckedEntrySet::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_IInterface == riid) {
        return (PInterface)(ISet*)this;
    }
    else if (EIID_IIterable == riid) {
        return (IIterable*)(ISet*)this;
    }
    else if (EIID_ICollection == riid) {
        return (ICollection*)(ISet*)this;
    }
    else if (EIID_ISet == riid) {
        return (ISet*)this;
    }

    return NULL;
}

ECode CCollections::CheckedMap::CheckedEntrySet::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID)

    if (pObject == (IInterface*)(ISet*)this) {
        *pIID = EIID_ISet;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

CCollections::CheckedMap::CheckedEntrySet::CheckedEntrySet(
    /* [in] */ ISet* s,
    /* [in] */ InterfaceID valueType)
{
    mS = s;
    mValueType = valueType;
}

ECode CCollections::CheckedMap::CheckedEntrySet::GetIterator(
    /* [out] */ IIterator** result)
{
    VALIDATE_NOT_NULL(result)
    AutoPtr<IIterator> it;
    mS->GetIterator((IIterator**)&it);
    AutoPtr<IIterator> res = new CheckedEntryIterator(it, mValueType);
    *result = res;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode CCollections::CheckedMap::CheckedEntrySet::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    VALIDATE_NOT_NULL(array)
    Int32 thisSize;
    GetSize(&thisSize);
    AutoPtr<ArrayOf<IInterface*> > arr = ArrayOf<IInterface*>::Alloc(thisSize);
    AutoPtr<IIterator> it;
    GetIterator((IIterator**)&it);
    for (Int32 i = 0; i < thisSize; i++) {
        AutoPtr<IInterface> o;
        it->Next((IInterface**)&o);
        arr->Set(i, o);
    }
    *array = arr;
    REFCOUNT_ADD(*array)
    return NOERROR;
}

ECode CCollections::CheckedMap::CheckedEntrySet::ToArray(
    /* [in] */ ArrayOf<IInterface*>* inArray,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    VALIDATE_NOT_NULL(outArray)
    Int32 thisSize;
    GetSize(&thisSize);
    if (inArray->GetLength() < thisSize) {
        inArray = ArrayOf<IInterface*>::Alloc(thisSize);
    }
    AutoPtr<IIterator> it;
    GetIterator((IIterator**)&it);
    for (Int32 i = 0; i < thisSize; i++) {
        AutoPtr<IInterface> o;
        it->Next((IInterface**)&o);
        inArray->Set(i, o);
    }
    if (thisSize < inArray->GetLength()) {
        inArray->Set(thisSize, NULL);
    }
    *outArray = inArray;
    REFCOUNT_ADD(*outArray)
    return NOERROR;
}

ECode CCollections::CheckedMap::CheckedEntrySet::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    VALIDATE_NOT_NULL(modified)
    return mS->RetainAll(collection, modified);
}

ECode CCollections::CheckedMap::CheckedEntrySet::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    VALIDATE_NOT_NULL(modified)
    return mS->RemoveAll(collection, modified);
}

ECode CCollections::CheckedMap::CheckedEntrySet::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return mS->ContainsAll(collection, result);
}

ECode CCollections::CheckedMap::CheckedEntrySet::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    *modified = FALSE;
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode CCollections::CheckedMap::CheckedEntrySet::Remove(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    VALIDATE_NOT_NULL(modified)
    return mS->Remove(object, modified);
}

ECode CCollections::CheckedMap::CheckedEntrySet::Contains(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return mS->Contains(object, result);
}

ECode CCollections::CheckedMap::CheckedEntrySet::Add(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    *modified = FALSE;
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode CCollections::CheckedMap::CheckedEntrySet::IsEmpty(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return mS->IsEmpty(result);
}

ECode CCollections::CheckedMap::CheckedEntrySet::Clear()
{
    return mS->Clear();
}

ECode CCollections::CheckedMap::CheckedEntrySet::GetSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size)
    return mS->GetSize(size);
}

ECode CCollections::CheckedMap::CheckedEntrySet::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode)
    return mS->GetHashCode(hashCode);
}

ECode CCollections::CheckedMap::CheckedEntrySet::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return mS->Equals(object, result);
}

//====================================================================
// CCollections::CheckedMap::
//====================================================================

UInt32 CCollections::CheckedMap::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CCollections::CheckedMap::Release()
{
    return ElRefBase::Release();
}

PInterface CCollections::CheckedMap::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_IInterface == riid) {
        return (PInterface)(IMap*)this;
    }
    else if (EIID_IIterable == riid) {
        return (IIterable*)(IMap*)this;
    }
    else if (EIID_ICollection == riid) {
        return (ICollection*)(IMap*)this;
    }
    else if (EIID_IMap == riid) {
        return (IMap*)this;
    }
    else if (Elastos::IO::EIID_ISerializable == riid) {
        return (ISerializable*)this;
    }

    return NULL;
}

ECode CCollections::CheckedMap::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID)

    if (pObject == (IInterface*)(IMap*)this) {
        *pIID = EIID_IMap;
    }
    else if (pObject == (IInterface*)(ISerializable*)this) {
        *pIID = Elastos::IO::EIID_ISerializable;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

CCollections::CheckedMap::CheckedMap(
    /* [in] */ IMap* m,
    /* [in] */ InterfaceID keyType,
    /* [in] */ InterfaceID valueType)
{
    if (mM == NULL) {
        //throw new NullPointerException("m == null");
        return;
    }
    //else if (mKeyType == NULL) {
        //throw new NullPointerException("keyType == null");
    //}
    //else if (mValueType == NULL) {
        //throw new NullPointerException("valueType == null");
    //}
    mM = m;
    mKeyType = keyType;
    mValueType = valueType;
}

ECode CCollections::CheckedMap::GetSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size)
    return mM->GetSize(size);
}

ECode CCollections::CheckedMap::IsEmpty(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return mM->IsEmpty(result);
}

ECode CCollections::CheckedMap::ContainsKey(
    /* [in] */ IInterface* key,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return mM->ContainsKey(key, result);
}

ECode CCollections::CheckedMap::ContainsValue(
    /* [in] */ IInterface* value,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return mM->ContainsValue(value, result);
}

ECode CCollections::CheckedMap::Get(
    /* [in] */ PInterface key,
    /* [out] */ PInterface* value)
{
    VALIDATE_NOT_NULL(value)
    return mM->Get(key, value);
}

ECode CCollections::CheckedMap::Put(
    /* [in] */ PInterface key,
    /* [in] */ PInterface value,
    /* [out] */ PInterface* oldValue)
{
    FAIL_RETURN(CCollections::_CheckType(key, mKeyType));
    FAIL_RETURN(CCollections::_CheckType(value, mValueType));
    return mM->Put(key, value, oldValue);
}

ECode CCollections::CheckedMap::Remove(
    /* [in] */ PInterface key,
    /* [out] */ PInterface* value)
{
    VALIDATE_NOT_NULL(value)
    return mM->Remove(key, value);
}

ECode CCollections::CheckedMap::PutAll(
    /* [in] */ IMap* map)
{
    Int32 size;
    map->GetSize(&size);
    if (size == 0) {
        return NOERROR;
    }
    AutoPtr<ArrayOf<IMapEntry*> > entries = ArrayOf<IMapEntry*>::Alloc(size);
    AutoPtr<ISet> entry;
    map->EntrySet((ISet**)&entry);
    AutoPtr<IIterator> it;
    entry->GetIterator((IIterator**)&it);
    for (Int32 i = 0; i < size; i++) {
        AutoPtr<IInterface> o;
        it->Next((IInterface**)&o);
        AutoPtr<IMapEntry> e = IMapEntry::Probe(o);
        if (e != NULL) {
            AutoPtr<IInterface> key;
            e->GetKey((IInterface**)&key);
            FAIL_RETURN(CCollections::_CheckType(key, mKeyType));
            AutoPtr<IInterface> val;
            e->GetValue((IInterface**)&val);
            FAIL_RETURN(CCollections::_CheckType(val, mValueType));
            entries->Set(i, e);
        }
    }
    for (Int32 i = 0; i < size; i++) {
        AutoPtr<IInterface> key;
        (*entries)[i]->GetKey((IInterface**)&key);
        AutoPtr<IInterface> value;
        (*entries)[i]->GetValue((IInterface**)&value);
        AutoPtr<IInterface> old;
        mM->Put(key, value, (IInterface**)&old);
    }
    return NOERROR;
}

ECode CCollections::CheckedMap::Clear()
{
    return mM->Clear();
}

ECode CCollections::CheckedMap::KeySet(
    /* [out, callee] */ ISet** keySet)
{
    VALIDATE_NOT_NULL(keySet)
    return mM->KeySet(keySet);
}

ECode CCollections::CheckedMap::Values(
    /* [out] */ ICollection** value)
{
    VALIDATE_NOT_NULL(value)
    return mM->Values(value);
}

ECode CCollections::CheckedMap::EntrySet(
    /* [out, callee] */ ISet** entries)
{
    VALIDATE_NOT_NULL(entries)
    AutoPtr<ISet> set;
    mM->EntrySet((ISet**)&set);
    AutoPtr<ISet> res = new CheckedEntrySet(set, mValueType);
    *entries = res;
    REFCOUNT_ADD(*entries)
    return NOERROR;
}

ECode CCollections::CheckedMap::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return mM->Equals(object, result);
}

ECode CCollections::CheckedMap::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode)
    return mM->GetHashCode(hashCode);
}

ECode CCollections::CheckedMap::ToString(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result)
    AutoPtr<IObject> iObj;
    iObj = IObject::Probe(mM);
    if (iObj != NULL) {
        return iObj->ToString(result);
    }
    *result = NULL;
    return NOERROR;
}

//====================================================================
// CCollections::CheckedSortedSet::
//====================================================================

UInt32 CCollections::CheckedSortedSet::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CCollections::CheckedSortedSet::Release()
{
    return ElRefBase::Release();
}

PInterface CCollections::CheckedSortedSet::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_ISortedSet == riid) {
        return (ISortedSet*)this;
    }
    return CheckedSet::Probe(riid);
}

ECode CCollections::CheckedSortedSet::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID)

    if (pObject == (IInterface*)(ISortedSet*)this) {
        *pIID = EIID_ISortedSet;
    }

    return CheckedSet::GetInterfaceID(pObject, pIID);
}

CCollections::CheckedSortedSet::CheckedSortedSet(
    /* [in] */ ISortedSet* s,
    /* [in] */ InterfaceID type) : CheckedSet(s, type)
{
    mSs = s;
}

ECode CCollections::CheckedSortedSet::Comparator(
    /* [out] */ IComparator** outcom)
{
    VALIDATE_NOT_NULL(outcom)
    return mSs->Comparator(outcom);
}

ECode CCollections::CheckedSortedSet::SubSet(
    /* [in] */ IInterface* start,
    /* [in] */ IInterface* end,
    /* [out] */ ISortedSet** outsort)
{
    VALIDATE_NOT_NULL(outsort)
    AutoPtr<ISortedSet> set;
    mSs->SubSet(start, end, (ISortedSet**)&set);
    AutoPtr<ISortedSet> res = new CheckedSortedSet(set, mType);
    *outsort = res;
    REFCOUNT_ADD(*outsort)
    return NOERROR;
}

ECode CCollections::CheckedSortedSet::HeadSet(
    /* [in] */ IInterface* end,
    /* [out] */ ISortedSet** outsort)
{
    VALIDATE_NOT_NULL(outsort)
    AutoPtr<ISortedSet> set;
    mSs->HeadSet(end, (ISortedSet**)&set);
    AutoPtr<ISortedSet> res = new CheckedSortedSet(set, mType);
    *outsort = res;
    REFCOUNT_ADD(*outsort)
    return NOERROR;
}

ECode CCollections::CheckedSortedSet::TailSet(
    /* [in] */ IInterface* start,
    /* [out] */ ISortedSet** outsort)
{
    VALIDATE_NOT_NULL(outsort)
    AutoPtr<ISortedSet> set;
    mSs->TailSet(start, (ISortedSet**)&set);
    AutoPtr<ISortedSet> res = new CheckedSortedSet(set, mType);
    *outsort = res;
    REFCOUNT_ADD(*outsort)
    return NOERROR;
}

ECode CCollections::CheckedSortedSet::First(
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface)
    return mSs->First(outface);
}

ECode CCollections::CheckedSortedSet::Last(
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface)
    return mSs->Last(outface);
}

ECode CCollections::CheckedSortedSet::GetIterator(
    /* [out] */ IIterator** result)
{
    return CheckedSet::GetIterator(result);
}

ECode CCollections::CheckedSortedSet::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    return CheckedSet::ToArray(array);
}

ECode CCollections::CheckedSortedSet::ToArray(
    /* [in] */ ArrayOf<IInterface*>* inArray,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    return CheckedSet::ToArray(inArray, outArray);
}

ECode CCollections::CheckedSortedSet::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return CheckedSet::RetainAll(collection, modified);
}

ECode CCollections::CheckedSortedSet::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return CheckedSet::RemoveAll(collection, modified);
}

ECode CCollections::CheckedSortedSet::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return CheckedSet::ContainsAll(collection, result);
}

ECode CCollections::CheckedSortedSet::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return CheckedSet::AddAll(collection, modified);
}

ECode CCollections::CheckedSortedSet::Remove(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return CheckedSet::Remove(object, modified);
}

ECode CCollections::CheckedSortedSet::Contains(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return CheckedSet::Contains(object, result);
}

ECode CCollections::CheckedSortedSet::Add(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return CheckedSet::Add(object, modified);
}

ECode CCollections::CheckedSortedSet::IsEmpty(
    /* [out] */ Boolean* result)
{
    return CheckedSet::IsEmpty(result);
}

ECode CCollections::CheckedSortedSet::Clear()
{
    return CheckedSet::Clear();
}

ECode CCollections::CheckedSortedSet::GetSize(
    /* [out] */ Int32* size)
{
    return CheckedSet::GetSize(size);
}

ECode CCollections::CheckedSortedSet::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    return CheckedSet::GetHashCode(hashCode);
}

ECode CCollections::CheckedSortedSet::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return CheckedSet::Equals(object, result);
}

//====================================================================
// CCollections::CheckedSortedMap::
//====================================================================

UInt32 CCollections::CheckedSortedMap::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CCollections::CheckedSortedMap::Release()
{
    return ElRefBase::Release();
}

PInterface CCollections::CheckedSortedMap::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_ISortedMap == riid) {
        return (ISortedMap*)this;
    }
    return CheckedMap::Probe(riid);
}

ECode CCollections::CheckedSortedMap::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID)

    if (pObject == (IInterface*)(ISortedMap*)this) {
        *pIID = EIID_ISortedMap;
    }

    return CheckedMap::GetInterfaceID(pObject, pIID);
}

CCollections::CheckedSortedMap::CheckedSortedMap(
    /* [in] */ ISortedMap* m,
    /* [in] */ InterfaceID keyType,
    /* [in] */ InterfaceID valueType)
    : CheckedMap(m, keyType, valueType)
{
    mSm = m;
}

ECode CCollections::CheckedSortedMap::Comparator(
    /* [out] */ IComparator** comp)
{
    VALIDATE_NOT_NULL(comp)
    return mSm->Comparator(comp);
}

ECode CCollections::CheckedSortedMap::SubMap(
    /* [in] */ IInterface* startKey,
    /* [in] */ IInterface* endKey,
    /* [out] */ ISortedMap** sortmap)
{
    VALIDATE_NOT_NULL(sortmap)
    AutoPtr<ISortedMap> map;
    mSm->SubMap(startKey, endKey, (ISortedMap**)&map);
    AutoPtr<ISortedMap> res = new CheckedSortedMap(map, mKeyType, mValueType);
    *sortmap = res;
    REFCOUNT_ADD(*sortmap)
    return NOERROR;
}

ECode CCollections::CheckedSortedMap::HeadMap(
    /* [in] */ IInterface* endKey,
    /* [out] */ ISortedMap** sortmap)
{
    VALIDATE_NOT_NULL(sortmap)
    AutoPtr<ISortedMap> map;
    mSm->HeadMap(endKey, (ISortedMap**)&map);
    AutoPtr<ISortedMap> res = new CheckedSortedMap(map, mKeyType, mValueType);
    *sortmap = res;
    REFCOUNT_ADD(*sortmap)
    return NOERROR;
}

ECode CCollections::CheckedSortedMap::TailMap(
    /* [in] */ IInterface* startKey,
    /* [out] */ ISortedMap** sortmap)
{
    VALIDATE_NOT_NULL(sortmap)
    AutoPtr<ISortedMap> map;
    mSm->TailMap(startKey, (ISortedMap**)&map);
    AutoPtr<ISortedMap> res = new CheckedSortedMap(map, mKeyType, mValueType);
    *sortmap = res;
    REFCOUNT_ADD(*sortmap)
    return NOERROR;
}

ECode CCollections::CheckedSortedMap::FirstKey(
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface)
    return mSm->FirstKey(outface);
}

ECode CCollections::CheckedSortedMap::LastKey(
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface)
    return mSm->LastKey(outface);
}

ECode CCollections::CheckedSortedMap::GetSize(
    /* [out] */ Int32* size)
{
    return CheckedMap::GetSize(size);
}

ECode CCollections::CheckedSortedMap::IsEmpty(
    /* [out] */ Boolean* result)
{
    return CheckedMap::IsEmpty(result);
}

ECode CCollections::CheckedSortedMap::ContainsKey(
    /* [in] */ IInterface* key,
    /* [out] */ Boolean* result)
{
    return CheckedMap::ContainsKey(key, result);
}

ECode CCollections::CheckedSortedMap::ContainsValue(
    /* [in] */ IInterface* value,
    /* [out] */ Boolean* result)
{
    return CheckedMap::ContainsValue(value, result);
}

ECode CCollections::CheckedSortedMap::Get(
    /* [in] */ PInterface key,
    /* [out] */ PInterface* value)
{
    return CheckedMap::Get(key, value);
}

ECode CCollections::CheckedSortedMap::Put(
    /* [in] */ PInterface key,
    /* [in] */ PInterface value,
    /* [out] */ PInterface* oldValue)
{
    return CheckedMap::Put(key, value, oldValue);
}

ECode CCollections::CheckedSortedMap::Remove(
    /* [in] */ PInterface key,
    /* [out] */ PInterface* value)
{
    return CheckedMap::Remove(key, value);
}

ECode CCollections::CheckedSortedMap::PutAll(
    /* [in] */ IMap* map)
{
    return CheckedMap::PutAll(map);
}

ECode CCollections::CheckedSortedMap::Clear()
{
    return CheckedMap::Clear();
}

ECode CCollections::CheckedSortedMap::KeySet(
    /* [out, callee] */ ISet** keySet)
{
    return CheckedMap::KeySet(keySet);
}

ECode CCollections::CheckedSortedMap::Values(
    /* [out] */ ICollection** value)
{
    return CheckedMap::Values(value);
}

ECode CCollections::CheckedSortedMap::EntrySet(
    /* [out, callee] */ ISet** entries)
{
    return CheckedMap::EntrySet(entries);
}

ECode CCollections::CheckedSortedMap::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return CheckedMap::Equals(object, result);
}

ECode CCollections::CheckedSortedMap::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    return CheckedMap::GetHashCode(hashCode);
}

} // namespace Utility
} // namespace Elastos
