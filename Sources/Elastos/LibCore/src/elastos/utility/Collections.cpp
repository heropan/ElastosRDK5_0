
#include "Collections.h"
#include "CBoolean.h"
#include "Arrays.h"
#include "CArrayList.h"
#include "CRandom.h"
#include "Autolock.h"

using Elastos::Core::IComparable;
using Elastos::Core::IBoolean;
using Elastos::Core::CBoolean;
using Elastos::Core::EIID_IComparator;
using Elastos::Utility::CRandom;
using Elastos::Utility::Arrays;
using Elastos::Utility::IArrayList;
using Elastos::Utility::CArrayList;
using Elastos::IO::EIID_ISerializable;

namespace Elastos {
namespace Utility {

const AutoPtr<IIterator> Collections::EMPTY_ITERATOR = new Collections::Iterator();
const AutoPtr<IEnumeration> Collections::EMPTY_ENUMERATION = new Collections::MyEnumeration();

//====================================================================
// Collections::Iterator::
//====================================================================
CAR_INTERFACE_IMPL(Collections::Iterator, Object, IIterator)

Collections::Iterator::Iterator()
{
}

ECode Collections::Iterator::HasNext(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;
    return NOERROR;
}

ECode Collections::Iterator::GetNext(
    /* [out] */ IInterface** object)
{
    return E_NO_SUCH_ELEMENT_EXCEPTION;
}

ECode Collections::Iterator::Remove()
{
    return E_ILLEGAL_STATE_EXCEPTION;
}

//====================================================================
// Collections::MyEnumeration::
//====================================================================
CAR_INTERFACE_IMPL(Collections::MyEnumeration, Object, IEnumeration)

Collections::MyEnumeration::MyEnumeration()
{
}

ECode Collections::MyEnumeration::HasMoreElements(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)
    *value = FALSE;
    return NOERROR;
}

ECode Collections::MyEnumeration::GetNextElement(
    /* [out] */ IInterface** inter)
{
    return E_NO_SUCH_ELEMENT_EXCEPTION;
}

//====================================================================
// Collections::MyEnumeration2::
//====================================================================
CAR_INTERFACE_IMPL(Collections::MyEnumeration2, Object, IEnumeration)

Collections::MyEnumeration2::MyEnumeration2(
    /* [in] */ ICollection* c)
{
    (IIterable::Probe(c))->GetIterator((IIterator**)&mIt);
}

ECode Collections::MyEnumeration2::HasMoreElements(
    /* [out] */ Boolean* value)
{
    return mIt->HasNext(value);
}

ECode Collections::MyEnumeration2::GetNextElement(
    /* [out] */ IInterface** inter)
{
    return mIt->GetNext(inter);
}

//====================================================================
// Collections::CopiesList::
//====================================================================
CAR_INTERFACE_IMPL(Collections::CopiesList, AbstractList, ISerializable)

Collections::CopiesList::CopiesList(
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

ECode Collections::CopiesList::Contains(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;
    if (mElement == NULL) {
        *result = object == NULL;
    }
    else {
        *result = Object::Equals(mElement, object);
    }
    return NOERROR;
}

ECode Collections::CopiesList::GetSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size)
    *size = mN;
    return NOERROR;
}

ECode Collections::CopiesList::Get(
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

ECode Collections::CopiesList::Add(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return AbstractList::Add(object, modified);
}

ECode Collections::CopiesList::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractList::AddAll(collection, modified);
}

ECode Collections::CopiesList::Clear()
{
    return AbstractList::Clear();
}

ECode Collections::CopiesList::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return AbstractList::ContainsAll(collection, result);
}

ECode Collections::CopiesList::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return AbstractList::Equals(object, result);
}

ECode Collections::CopiesList::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    return AbstractList::GetHashCode(hashCode);
}

ECode Collections::CopiesList::IsEmpty(
    /* [out] */ Boolean* result)
{
    return AbstractList::IsEmpty(result);
}

ECode Collections::CopiesList::Remove(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return AbstractList::Remove(object, modified);
}

ECode Collections::CopiesList::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractList::RemoveAll(collection, modified);
}

ECode Collections::CopiesList::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractList::RetainAll(collection, modified);
}

ECode Collections::CopiesList::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    return AbstractList::ToArray(array);
}

ECode Collections::CopiesList::ToArray(
    /* [in] */ ArrayOf<IInterface*>* inArray,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    return AbstractList::ToArray(inArray, outArray);
}

ECode Collections::CopiesList::Add(
    /* [in] */ Int32 location,
    /* [in] */ IInterface* object)
{
    return AbstractList::Add(location, object);
}

ECode Collections::CopiesList::AddAll(
    /* [in] */ Int32 location,
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractList::AddAll(location, collection, modified);
}

ECode Collections::CopiesList::IndexOf(
    /* [in] */ IInterface* object,
    /* [out] */ Int32* index)
{
    return AbstractList::IndexOf(object, index);
}

ECode Collections::CopiesList::LastIndexOf(
    /* [in] */ IInterface* object,
    /* [out] */ Int32* index)
{
    return AbstractList::LastIndexOf(object, index);
}

ECode Collections::CopiesList::GetListIterator(
    /* [out] */ IListIterator** it)
{
    return AbstractList::GetListIterator(it);
}

ECode Collections::CopiesList::GetListIterator(
    /* [in] */ Int32 location,
    /* [out] */ IListIterator** it)
{
    return AbstractList::GetListIterator(location, it);
}

ECode Collections::CopiesList::Remove(
    /* [in] */ Int32 location,
    /* [out] */ IInterface** object)
{
    return AbstractList::Remove(location, object);
}

ECode Collections::CopiesList::Set(
    /* [in] */ Int32 location,
    /* [in] */ IInterface* object,
    /* [out] */ IInterface** prevObject)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode Collections::CopiesList::GetSubList(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ IList** subList)
{
    return AbstractList::GetSubList(start, end, subList);
}

ECode Collections::CopiesList::GetIterator(
    /* [out] */ IIterator** it)
{
    return AbstractList::GetIterator(it);
}

//====================================================================
// Collections::EmptyList::
//====================================================================
CAR_INTERFACE_IMPL_2(Collections::EmptyList, AbstractList, IRandomAccess, ISerializable)

ECode Collections::EmptyList::Contains(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;
    return NOERROR;
}

ECode Collections::EmptyList::GetSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size)
    *size = 0;
    return NOERROR;
}

ECode Collections::EmptyList::Get(
    /* [in] */ Int32 location,
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object)
    *object = NULL;
    return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
}

ECode Collections::EmptyList::Set(
    /* [in] */ Int32 location,
    /* [in] */ IInterface* object,
    /* [out] */ IInterface** prevObject)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode Collections::EmptyList::GetIterator(
    /* [out] */ IIterator** it)
{
    return AbstractList::GetIterator(it);
}

ECode Collections::EmptyList::Add(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return AbstractList::Add(object, result);
}

ECode Collections::EmptyList::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return AbstractList::AddAll(collection, result);
}

ECode Collections::EmptyList::Clear()
{
    return AbstractList::Clear();
}

ECode Collections::EmptyList::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return AbstractList::ContainsAll(collection, result);
}

ECode Collections::EmptyList::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return AbstractList::Equals(object, result);
}

ECode Collections::EmptyList::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    return AbstractList::GetHashCode(hashCode);
}

ECode Collections::EmptyList::IsEmpty(
    /* [out] */ Boolean* result)
{
    return AbstractList::IsEmpty(result);
}

ECode Collections::EmptyList::Remove(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return AbstractList::Remove(object, result);
}

ECode Collections::EmptyList::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return AbstractList::RemoveAll(collection, result);
}

ECode  Collections::EmptyList::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return AbstractList::RetainAll(collection, result);
}

ECode Collections::EmptyList::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    return AbstractList::ToArray(array);
}

ECode Collections::EmptyList::ToArray(
    /* [in] */ ArrayOf<IInterface*>* contents,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    return AbstractList::ToArray(contents, outArray);
}

ECode Collections::EmptyList::Add(
    /* [in] */ Int32 location,
    /* [in] */ IInterface* object)
{
    return AbstractList::Add(location, object);
}

ECode Collections::EmptyList::AddAll(
    /* [in] */ Int32 location,
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return AbstractList::AddAll(location, collection, result);
}

ECode Collections::EmptyList::IndexOf(
    /* [in] */ IInterface* object,
    /* [out] */ Int32* index)
{
    return AbstractList::IndexOf(object, index);
}

ECode Collections::EmptyList::LastIndexOf(
    /* [in] */ IInterface* object,
    /* [out] */ Int32* index)
{
    return AbstractList::LastIndexOf(object, index);
}

ECode Collections::EmptyList::GetListIterator(
    /* [out] */ IListIterator** it)
{
    return AbstractList::GetListIterator(it);
}

ECode Collections::EmptyList::GetSubList(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ IList** subList)
{
    return AbstractList::GetSubList(start, end, subList);
}

ECode Collections::EmptyList::GetListIterator(
    /* [in] */ Int32 location,
    /* [out] */ IListIterator** listiterator)
{
    return AbstractList::GetListIterator(location, listiterator);
}

ECode Collections::EmptyList::Remove(
    /* [in] */ Int32 location,
    /* [out] */ IInterface** object)
{
    return AbstractList::Remove(location, object);
}

AutoPtr<IInterface> Collections::EmptyList::ReadResolve()
{
    return Collections::EMPTY_LIST;
}

//====================================================================
// Collections::EmptySet::
//====================================================================
CAR_INTERFACE_IMPL(Collections::EmptySet, AbstractSet, ISerializable)

ECode Collections::EmptySet::Contains(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;
    return NOERROR;
}

ECode Collections::EmptySet::GetSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size)
    *size = 0;
    return NOERROR;
}

ECode Collections::EmptySet::GetIterator(
    /* [out] */ IIterator** it)
{
    VALIDATE_NOT_NULL(it)
    *it = EMPTY_ITERATOR;
    REFCOUNT_ADD(*it)
    return NOERROR;
}

ECode Collections::EmptySet::Add(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::Add(object, modified);
}

ECode Collections::EmptySet::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::AddAll(collection, modified);
}

ECode Collections::EmptySet::Clear()
{
    return AbstractSet::Clear();
}

ECode Collections::EmptySet::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return AbstractSet::ContainsAll(collection, result);
}

ECode Collections::EmptySet::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return AbstractSet::Equals(object, result);
}

ECode Collections::EmptySet::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    return AbstractSet::GetHashCode(hashCode);
}

ECode Collections::EmptySet::IsEmpty(
    /* [out] */ Boolean* result)
{
    return AbstractSet::IsEmpty(result);
}

ECode Collections::EmptySet::Remove(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::Remove(object, modified);
}

ECode Collections::EmptySet::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::RemoveAll(collection, modified);
}

ECode Collections::EmptySet::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::RetainAll(collection, modified);
}

ECode Collections::EmptySet::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    return AbstractSet::ToArray(array);
}

ECode Collections::EmptySet::ToArray(
    /* [in] */ ArrayOf<IInterface*>* inArray,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    return AbstractSet::ToArray(inArray, outArray);
}

AutoPtr<IInterface> Collections::EmptySet::ReadResolve()
{
    return Collections::EMPTY_SET;
}

//====================================================================
// Collections::EmptyMap::
//====================================================================
CAR_INTERFACE_IMPL(Collections::EmptyMap, AbstractMap, ISerializable)

ECode Collections::EmptyMap::ContainsKey(
    /* [in] */ IInterface* key,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;
    return NOERROR;
}

ECode Collections::EmptyMap::ContainsValue(
    /* [in] */ IInterface* value,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;
    return NOERROR;
}

ECode Collections::EmptyMap::GetEntrySet(
    /* [out, callee] */ ISet** entries)
{
    VALIDATE_NOT_NULL(entries)
    *entries = EMPTY_SET;
    REFCOUNT_ADD(*entries)
    return NOERROR;
}

ECode Collections::EmptyMap::Get(
    /* [in] */ PInterface key,
    /* [out] */ PInterface* value)
{
    VALIDATE_NOT_NULL(value)
    *value = NULL;
    return NOERROR;
}

ECode Collections::EmptyMap::GetKeySet(
    /* [out, callee] */ ISet** keySet)
{
    VALIDATE_NOT_NULL(keySet)
    *keySet = EMPTY_SET;
    REFCOUNT_ADD(*keySet)
    return NOERROR;
}

ECode Collections::EmptyMap::GetValues(
    /* [out] */ ICollection** value)
{
    VALIDATE_NOT_NULL(value)
    *value = ICollection::Probe(EMPTY_LIST);
    REFCOUNT_ADD(*value)
    return NOERROR;
}

ECode Collections::EmptyMap::Clear()
{
    return AbstractMap::Clear();
}

ECode Collections::EmptyMap::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return AbstractMap::Equals(object, result);
}

ECode Collections::EmptyMap::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    return AbstractMap::GetHashCode(hashCode);
}

ECode Collections::EmptyMap::IsEmpty(
    /* [out] */ Boolean* result)
{
    return AbstractMap::IsEmpty(result);
}

ECode Collections::EmptyMap::Put(
    /* [in] */ PInterface key,
    /* [in] */ PInterface value,
    /* [out] */ PInterface* oldValue)
{
    return AbstractMap::Put(key, value, oldValue);
}

ECode Collections::EmptyMap::Put(
    /* [in] */ PInterface key,
    /* [in] */ PInterface value)
{
    assert(0 && "TODO");
    return NOERROR;
}

ECode Collections::EmptyMap::PutAll(
    /* [in] */ IMap* map)
{
    return AbstractMap::PutAll(map);
}

ECode Collections::EmptyMap::Remove(
    /* [in] */ PInterface key,
    /* [out] */ PInterface* value)
{
    return AbstractMap::Remove(key, value);
}

ECode Collections::EmptyMap::Remove(
    /* [in] */ PInterface key)
{
    assert(0 && "TODO");
    return NOERROR;
}

ECode Collections::EmptyMap::GetSize(
    /* [out] */ Int32* size)
{
    return AbstractMap::GetSize(size);
}

AutoPtr<IInterface> Collections::EmptyMap::ReadResolve()
{
    return Collections::EMPTY_MAP;
}

const AutoPtr<IList> Collections::EMPTY_LIST = new Collections::EmptyList();

const AutoPtr<ISet> Collections::EMPTY_SET = new Collections::EmptySet();

const AutoPtr<IMap> Collections::EMPTY_MAP = new Collections::EmptyMap();

//====================================================================
// Collections::ReverseComparator::
//====================================================================
CAR_INTERFACE_IMPL_2(Collections::ReverseComparator, Object, IComparator, ISerializable)

const AutoPtr<Collections::ReverseComparator> Collections::ReverseComparator::INSTANCE = new Collections::ReverseComparator();

ECode Collections::ReverseComparator::Compare(
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

AutoPtr<IInterface> Collections::ReverseComparator::ReadResolve()
{
    return INSTANCE->Probe(EIID_IInterface);
}

//====================================================================
// Collections::ReverseComparator2::
//====================================================================
CAR_INTERFACE_IMPL_2(Collections::ReverseComparator2, Object, IComparator, ISerializable)

Collections::ReverseComparator2::ReverseComparator2(
    /* [in] */ IComparator* comparator)
{
    mCmp = comparator;
}

ECode Collections::ReverseComparator2::Compare(
    /* [in] */ IInterface* lhs,
    /* [in] */ IInterface* rhs,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)
    return mCmp->Compare(rhs, lhs, result);
}

ECode Collections::ReverseComparator2::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    AutoPtr<IComparator> p = IComparator::Probe(object);
    if (p != NULL) {
        *result = Object::Equals(mCmp, ((ReverseComparator2*)p.Get())->mCmp);
    }
    *result = FALSE;
    return NOERROR;
}

ECode Collections::ReverseComparator2::GetHashCode(
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
// Collections::SingletonSet::Iterator::
//====================================================================
CAR_INTERFACE_IMPL(Collections::SingletonSet::Iterator, Object, IIterator)

Collections::SingletonSet::Iterator::Iterator(
    /* [in] */ SingletonSet* owner)
{
    mHasNext = TRUE;
    mOwner = owner;
}

ECode Collections::SingletonSet::Iterator::HasNext(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = mHasNext;
    return NOERROR;
}

ECode Collections::SingletonSet::Iterator::GetNext(
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

ECode Collections::SingletonSet::Iterator::Remove()
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

//====================================================================
// Collections::SingletonSet::
//====================================================================
CAR_INTERFACE_IMPL(Collections::SingletonSet , AbstractSet, ISerializable)

Collections::SingletonSet::SingletonSet(
    /* [in] */ IInterface* object)
{
    mElement = object;
}

ECode Collections::SingletonSet::Contains(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;
    if (mElement == NULL) {
        *result = object == NULL;
    }
    else {
        *result = Object::Equals(mElement, object);
    }
    return NOERROR;
}

ECode Collections::SingletonSet::GetSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size)
    *size = 1;
    return NOERROR;
}

ECode Collections::SingletonSet::GetIterator(
    /* [out] */ IIterator** it)
{
    VALIDATE_NOT_NULL(it)
    *it = new Iterator(this);
    REFCOUNT_ADD(*it)
    return NOERROR;
}

ECode Collections::SingletonSet::Add(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::Add(object, modified);
}

ECode Collections::SingletonSet::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::AddAll(collection, modified);
}

ECode Collections::SingletonSet::Clear()
{
    return AbstractSet::Clear();
}

ECode Collections::SingletonSet::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return AbstractSet::ContainsAll(collection, result);
}

ECode Collections::SingletonSet::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return AbstractSet::Equals(object, result);
}

ECode Collections::SingletonSet::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    return AbstractSet::GetHashCode(hashCode);
}

ECode Collections::SingletonSet::IsEmpty(
    /* [out] */ Boolean* result)
{
    return AbstractSet::IsEmpty(result);
}

ECode Collections::SingletonSet::Remove(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::Remove(object, modified);
}

ECode Collections::SingletonSet::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::RemoveAll(collection, modified);
}

ECode Collections::SingletonSet::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::RetainAll(collection, modified);
}

ECode Collections::SingletonSet::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    return AbstractSet::ToArray(array);
}

ECode Collections::SingletonSet::ToArray(
    /* [in] */ ArrayOf<IInterface*>* inArray,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    return AbstractSet::ToArray(inArray, outArray);
}

//====================================================================
// Collections::SingletonList::
//====================================================================
CAR_INTERFACE_IMPL(Collections::SingletonList, AbstractList, ISerializable)

Collections::SingletonList::SingletonList(
    /* [in] */ IInterface* object)
{
    mElement = object;
}

ECode Collections::SingletonList::Contains(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    if (mElement == NULL) {
        *result = object == NULL;
    }
    else {
        *result = Object::Equals(mElement, object);
    }
    return NOERROR;
}

ECode Collections::SingletonList::Get(
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

ECode Collections::SingletonList::GetSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size)
    *size = 1;
    return NOERROR;
}

ECode Collections::SingletonList::Add(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return AbstractList::Add(object, modified);
}

ECode Collections::SingletonList::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractList::AddAll(collection, modified);
}

ECode Collections::SingletonList::Clear()
{
    return AbstractList::Clear();
}

ECode Collections::SingletonList::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return AbstractList::ContainsAll(collection, result);
}

ECode Collections::SingletonList::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return AbstractList::Equals(object, result);
}

ECode Collections::SingletonList::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    return AbstractList::GetHashCode(hashCode);
}

ECode Collections::SingletonList::IsEmpty(
    /* [out] */ Boolean* result)
{
    return AbstractList::IsEmpty(result);
}

ECode Collections::SingletonList::Remove(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return AbstractList::Remove(object, modified);
}

ECode Collections::SingletonList::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractList::RemoveAll(collection, modified);
}

ECode Collections::SingletonList::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractList::RetainAll(collection, modified);
}

ECode Collections::SingletonList::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    return AbstractList::ToArray(array);
}

ECode Collections::SingletonList::ToArray(
    /* [in] */ ArrayOf<IInterface*>* inArray,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    return AbstractList::ToArray(inArray, outArray);
}

ECode Collections::SingletonList::Add(
    /* [in] */ Int32 location,
    /* [in] */ IInterface* object)
{
    return AbstractList::Add(location, object);
}

ECode Collections::SingletonList::AddAll(
    /* [in] */ Int32 location,
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractList::AddAll(location, collection, modified);
}

ECode Collections::SingletonList::IndexOf(
    /* [in] */ IInterface* object,
    /* [out] */ Int32* index)
{
    return AbstractList::IndexOf(object, index);
}

ECode Collections::SingletonList::LastIndexOf(
    /* [in] */ IInterface* object,
    /* [out] */ Int32* index)
{
    return AbstractList::LastIndexOf(object, index);
}

ECode Collections::SingletonList::GetListIterator(
    /* [out] */ IListIterator** it)
{
    return AbstractList::GetListIterator(it);
}

ECode Collections::SingletonList::GetListIterator(
    /* [in] */ Int32 location,
    /* [out] */ IListIterator** it)
{
    return AbstractList::GetListIterator(location, it);
}

ECode Collections::SingletonList::Remove(
    /* [in] */ Int32 location,
    /* [out] */ IInterface** object)
{
    return AbstractList::Remove(location, object);
}

ECode Collections::SingletonList::Set(
    /* [in] */ Int32 location,
    /* [in] */ IInterface* object,
    /* [out] */ IInterface** prevObject)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode Collections::SingletonList::GetSubList(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ IList** subList)
{
    return AbstractList::GetSubList(start, end, subList);
}

ECode Collections::SingletonList::GetIterator(
    /* [out] */ IIterator** it)
{
    return AbstractList::GetIterator(it);
}

//====================================================================
// Collections::SingletonMap::MySet::Iterator::MyMapEntry::
//====================================================================

Collections::SingletonMap::MySet::Iterator::MyMapEntry::MyMapEntry(
    /* [in] */ IInterface* k,
    /* [in] */ IInterface* v)
    : MapEntry(k, v)
{
}

ECode Collections::SingletonMap::MySet::Iterator::MyMapEntry::SetValue(
    /* [in] */ IInterface* valueReplacer,
    /* [out] */ IInterface** valueReplacee)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

//====================================================================
// Collections::SingletonMap::MySet::Iterator::
//====================================================================
CAR_INTERFACE_IMPL(Collections::SingletonMap::MySet::Iterator, Object ,IIterator)

Collections::SingletonMap::MySet::Iterator::Iterator(
    /* [in] */ MySet* owner)
{
    mHasNext = TRUE;
    mOwner = owner;
}

ECode Collections::SingletonMap::MySet::Iterator::HasNext(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = mHasNext;
    return NOERROR;
}

ECode Collections::SingletonMap::MySet::Iterator::GetNext(
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

ECode Collections::SingletonMap::MySet::Iterator::GetNext(
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object)
    AutoPtr<IMapEntry> p = IMapEntry::Probe(*object);
    return GetNext((IMapEntry**)&p);
}

ECode Collections::SingletonMap::MySet::Iterator::Remove()
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

//====================================================================
// Collections::SingletonMap::MySet::
//====================================================================

Collections::SingletonMap::MySet::MySet(
    /* [in] */ SingletonMap* owner)
{
    mOwner = owner;
}

ECode Collections::SingletonMap::MySet::Contains(
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

ECode Collections::SingletonMap::MySet::GetSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size)
    *size = 1;
    return NOERROR;
}

ECode Collections::SingletonMap::MySet::GetIterator(
    /* [out] */ IIterator** it)
{
    VALIDATE_NOT_NULL(*it)
    *it = new Iterator(this);
    REFCOUNT_ADD(*it)
    return NOERROR;
}

ECode Collections::SingletonMap::MySet::Add(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::Add(object, modified);
}

ECode Collections::SingletonMap::MySet::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::AddAll(collection, modified);
}

ECode Collections::SingletonMap::MySet::Clear()
{
    return AbstractSet::Clear();
}

ECode Collections::SingletonMap::MySet::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return AbstractSet::ContainsAll(collection, result);
}

ECode Collections::SingletonMap::MySet::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return AbstractSet::Equals(object, result);
}

ECode Collections::SingletonMap::MySet::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    return AbstractSet::GetHashCode(hashCode);
}

ECode Collections::SingletonMap::MySet::IsEmpty(
    /* [out] */ Boolean* result)
{
    return AbstractSet::IsEmpty(result);
}

ECode Collections::SingletonMap::MySet::Remove(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::Remove(object, modified);
}

ECode Collections::SingletonMap::MySet::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::RemoveAll(collection, modified);
}

ECode Collections::SingletonMap::MySet::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::RetainAll(collection, modified);
}

ECode Collections::SingletonMap::MySet::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    return AbstractSet::ToArray(array);
}

ECode Collections::SingletonMap::MySet::ToArray(
    /* [in] */ ArrayOf<IInterface*>* inArray,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    return AbstractSet::ToArray(inArray, outArray);
}

//====================================================================
// Collections::SingletonMap::
//====================================================================
CAR_INTERFACE_IMPL(Collections::SingletonMap, AbstractMap, ISerializable)

Collections::SingletonMap::SingletonMap(
    /* [in] */ IInterface* key,
    /* [in] */ IInterface* value)
{
    mK = key;
    mV = value;
}

ECode Collections::SingletonMap::ContainsKey(
    /* [in] */ IInterface* key,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    if (mK == NULL) {
        *result = key == NULL;
    }
    else {
        *result = Object::Equals(mK, key);
    }
    return NOERROR;
}

ECode Collections::SingletonMap::ContainsValue(
    /* [in] */ IInterface* value,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    if (mV == NULL) {
        *result = value == NULL;
    }
    else {
        *result = Object::Equals(mV, value);
    }
    return NOERROR;
}

ECode Collections::SingletonMap::Get(
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

ECode Collections::SingletonMap::GetSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size)
    *size = 1;
    return NOERROR;
}

ECode Collections::SingletonMap::GetEntrySet(
    /* [out, callee] */ ISet** entries)
{
    VALIDATE_NOT_NULL(entries)
    *entries = new MySet(this);
    REFCOUNT_ADD(*entries)
    return NOERROR;
}

ECode Collections::SingletonMap::Clear()
{
    return AbstractMap::Clear();
}

ECode Collections::SingletonMap::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return AbstractMap::Equals(object, result);
}

ECode Collections::SingletonMap::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    return AbstractMap::GetHashCode(hashCode);
}

ECode Collections::SingletonMap::IsEmpty(
    /* [out] */ Boolean* result)
{
    return AbstractMap::IsEmpty(result);
}

ECode Collections::SingletonMap::GetKeySet(
    /* [out, callee] */ ISet** keySet)
{
    return AbstractMap::GetKeySet(keySet);
}

ECode Collections::SingletonMap::Put(
    /* [in] */ PInterface key,
    /* [in] */ PInterface value,
    /* [out] */ PInterface* oldValue)
{
    return AbstractMap::Put(key, value, oldValue);
}

ECode Collections::SingletonMap::PutAll(
    /* [in] */ IMap* map)
{
    return AbstractMap::PutAll(map);
}

ECode Collections::SingletonMap::Remove(
    /* [in] */ PInterface key,
    /* [out] */ PInterface* value)
{
    return AbstractMap::Remove(key, value);
}

ECode Collections::SingletonMap::GetValues(
    /* [out] */ ICollection** value)
{
    return AbstractMap::GetValues(value);
}

//====================================================================
// Collections::SynchronizedCollection::
//====================================================================
CAR_INTERFACE_IMPL_3(Collections::SynchronizedCollection, Object, ICollection, IIterable, ISerializable)

Collections::SynchronizedCollection::SynchronizedCollection(
    /* [in] */ ICollection* collection)
    : mLock(NULL)
    , mIsStrongLock(FALSE)
    , mC(collection)
{
}

Collections::SynchronizedCollection::SynchronizedCollection(
    /* [in] */ ICollection* collection,
    /* [in] */ Object* lock)
    : mLock(lock)
    , mC(collection)
{
    if (lock != this) {
        mIsStrongLock = TRUE;
        REFCOUNT_ADD(mLock);
    }
}

Collections::SynchronizedCollection::~SynchronizedCollection()
{
    if (mIsStrongLock) {
        REFCOUNT_RELEASE(mLock);
    }
}

ECode Collections::SynchronizedCollection::Add(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    VALIDATE_NOT_NULL(modified);
    synchronized(mLock) {
        mC->Add(object, modified);
    }
    return NOERROR;
}

ECode Collections::SynchronizedCollection::Add(
    /* [in] */ IInterface* object)
{
    assert(0 && "TODO");
    return NOERROR;
}

ECode Collections::SynchronizedCollection::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    VALIDATE_NOT_NULL(modified);
    synchronized (mLock) {
        mC->AddAll(collection, modified);
    }
    return NOERROR;
}

ECode Collections::SynchronizedCollection::AddAll(
    /* [in] */ ICollection* collection)
{
    assert(0 && "TODO");
    return NOERROR;
}

ECode Collections::SynchronizedCollection::Clear()
{
    synchronized (mLock) {
        mC->Clear();
    }
    return NOERROR;
}

ECode Collections::SynchronizedCollection::Contains(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    synchronized (mLock) {
        mC->Contains(object, result);
    }
    return NOERROR;
}

ECode Collections::SynchronizedCollection::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    synchronized (mLock) {
        mC->ContainsAll(collection, result);
    }
    return NOERROR;
}

ECode Collections::SynchronizedCollection::IsEmpty(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    synchronized (mLock) {
        mC->IsEmpty(result);
    }
    return NOERROR;
}

ECode Collections::SynchronizedCollection::GetIterator(
    /* [out] */ IIterator** it)
{
    VALIDATE_NOT_NULL(it);
    synchronized (mLock) {
        (IIterable::Probe(mC))->GetIterator(it);
    }
    return NOERROR;
}

ECode Collections::SynchronizedCollection::Remove(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    VALIDATE_NOT_NULL(modified);
    synchronized (mLock) {
        mC->Remove(object, modified);
    }
    return NOERROR;
}

ECode Collections::SynchronizedCollection::Remove(
    /* [in] */ IInterface* object)
{
    assert(0 && "TODO");
    return NOERROR;
}

ECode Collections::SynchronizedCollection::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    VALIDATE_NOT_NULL(modified);
    synchronized (mLock) {
        mC->RemoveAll(collection, modified);
    }
    return NOERROR;
}

ECode Collections::SynchronizedCollection::RemoveAll(
    /* [in] */ ICollection* collection)
{
    assert(0 && "TODO");
    return NOERROR;
}

ECode Collections::SynchronizedCollection::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    VALIDATE_NOT_NULL(modified);
    synchronized (mLock) {
        mC->RetainAll(collection, modified);
    }
    return NOERROR;
}

ECode Collections::SynchronizedCollection::RetainAll(
    /* [in] */ ICollection* collection)
{
    assert(0 && "TODO");
    return NOERROR;
}

ECode Collections::SynchronizedCollection::GetSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);
    synchronized (mLock) {
        mC->GetSize(size);
    }
    return NOERROR;
}

ECode Collections::SynchronizedCollection::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    VALIDATE_NOT_NULL(*array);
    synchronized (mLock) {
        mC->ToArray(array);
    }
    return NOERROR;
}

ECode Collections::SynchronizedCollection::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    synchronized (mLock) {
        AutoPtr<IObject> iObj;
        iObj = IObject::Probe(mC);
        if (iObj != NULL) {
            return iObj->ToString(str);
        }
        *str = NULL;
    }
    return NOERROR;
}

ECode Collections::SynchronizedCollection::ToArray(
    /* [in] */ ArrayOf<IInterface*>* inArray,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    VALIDATE_NOT_NULL(outArray);
    synchronized (mLock) {
        mC->ToArray(inArray, outArray);
    }
    return NOERROR;
}

ECode Collections::SynchronizedCollection::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = this->Probe(EIID_IInterface) == object;
    return NOERROR;
}

ECode Collections::SynchronizedCollection::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode);
    *hashCode = (Int32)this;
    return NOERROR;
}

ECode Collections::SynchronizedCollection::WriteObject(
    /* [in] */ IObjectOutputStream* stream)
{
    synchronized (mLock) {
        stream->DefaultWriteObject();
    }
    return NOERROR;
}

//====================================================================
// Collections::SynchronizedRandomAccessList::
//====================================================================
CAR_INTERFACE_IMPL(Collections::SynchronizedRandomAccessList, SynchronizedList, IRandomAccess)

Collections::SynchronizedRandomAccessList::SynchronizedRandomAccessList(
    /* [in] */ IList* l)
    : SynchronizedList(l)
{
}

Collections::SynchronizedRandomAccessList::SynchronizedRandomAccessList(
    /* [in] */ IList* l,
    /* [in] */ Object* lock)
    : SynchronizedList(l, lock)
{
}

ECode Collections::SynchronizedRandomAccessList::GetSubList(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ IList** subList)
{
    VALIDATE_NOT_NULL(*subList);
    synchronized (mLock) {
        AutoPtr<IList> sub;
        mList->GetSubList(start, end, (IList**)&sub);
        *subList = new SynchronizedRandomAccessList(sub, mLock);
        REFCOUNT_ADD(*subList);
    }

    return NOERROR;
}

AutoPtr<IInterface> Collections::SynchronizedRandomAccessList::WriteReplace()
{
    AutoPtr<SynchronizedList> list = new SynchronizedList(mList);
    return (IInterface*)list->Probe(EIID_IInterface);
}

//====================================================================
// Collections::SynchronizedList::
//====================================================================
CAR_INTERFACE_IMPL(Collections::SynchronizedList, SynchronizedCollection, IList)

Collections::SynchronizedList::SynchronizedList(
    /* [in] */ IList* l)
    : SynchronizedCollection(ICollection::Probe(l))
{
    mList = l;
}

Collections::SynchronizedList::SynchronizedList(
    /* [in] */ IList* l,
    /* [in] */ Object* lock)
    : SynchronizedCollection(ICollection::Probe(l), lock)
{
    mList = l;
}

ECode Collections::SynchronizedList::Add(
    /* [in] */ Int32 location,
    /* [in] */ IInterface* object)
{
    synchronized (mLock) {
        mList->Add(location, object);
    }
    return NOERROR;
}

ECode Collections::SynchronizedList::Add(
    /* [in] */ IInterface* object)
{
    assert(0 && "TODO");
    return NOERROR;
}

ECode Collections::SynchronizedList::AddAll(
    /* [in] */ Int32 location,
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    VALIDATE_NOT_NULL(modified);
    synchronized (mLock) {
        mList->AddAll(location, collection, modified);
    }
    return NOERROR;
}

ECode Collections::SynchronizedList::AddAll(
    /* [in] */ Int32 location,
    /* [in] */ ICollection* collection)
{
    assert(0 && "TODO");
    return NOERROR;
}

ECode Collections::SynchronizedList::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    synchronized (mLock) {
        (ICollection::Probe(mList))->Equals(object, result);
    }
    return NOERROR;
}

ECode Collections::SynchronizedList::Get(
    /* [in] */ Int32 location,
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(*object);
    synchronized (mLock) {
        mList->Get(location, object);
    }
    return NOERROR;
}

ECode Collections::SynchronizedList::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode);
    synchronized (mLock) {
        (ICollection::Probe(mList))->GetHashCode(hashCode);
    }
    return NOERROR;
}

ECode Collections::SynchronizedList::IndexOf(
    /* [in] */ IInterface* object,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);
    Int32 size;
    AutoPtr<ArrayOf<IInterface*> > array;
    synchronized (mLock) {
        (ICollection::Probe(mList))->GetSize(&size);
        array = ArrayOf<IInterface*>::Alloc(size);
        (ICollection::Probe(mList))->ToArray((ArrayOf<IInterface*>**)&array);
    }
    if (object != NULL) {
        for (Int32 i = 0; i < size; i++) {
            if (Object::Equals(object, (*array)[i])) {
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

ECode Collections::SynchronizedList::LastIndexOf(
    /* [in] */ IInterface* object,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);
    Int32 size;
    AutoPtr<ArrayOf<IInterface*> > array;
    synchronized (mLock) {
        (ICollection::Probe(mList))->GetSize(&size);
        array = ArrayOf<IInterface*>::Alloc(size);
        (ICollection::Probe(mList))->ToArray((ArrayOf<IInterface*>**)&array);
    }
    if (object != NULL) {
        for (Int32 i = size - 1; i >= 0; i--) {
            if (Object::Equals(object, (*array)[i])) {
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

ECode Collections::SynchronizedList::GetListIterator(
    /* [out] */ IListIterator** it)
{
    VALIDATE_NOT_NULL(it);
    synchronized (mLock) {
        mList->GetListIterator(it);
    }
    return NOERROR;
}

ECode Collections::SynchronizedList::GetListIterator(
    /* [in] */ Int32 location,
    /* [out] */ IListIterator** it)
{
    VALIDATE_NOT_NULL(it);
    synchronized (mLock) {
        mList->GetListIterator(location, it);
    }
    return NOERROR;
}

ECode Collections::SynchronizedList::Remove(
    /* [in] */ Int32 location,
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object);
    synchronized (mLock) {
        mList->Remove(location, object);
    }
    return NOERROR;
}

ECode Collections::SynchronizedList::Remove(
    /* [in] */ Int32 location)
{
    assert(0 && "TODO");
    return NOERROR;
}

ECode Collections::SynchronizedList::Set(
    /* [in] */ Int32 location,
    /* [in] */ IInterface* object,
    /* [out] */ IInterface** prevObject)
{
    VALIDATE_NOT_NULL(prevObject);
    synchronized (mLock) {
        mList->Set(location, object, prevObject);
    }
    return NOERROR;
}

ECode Collections::SynchronizedList::Set(
    /* [in] */ Int32 location,
    /* [in] */ IInterface* object)
{
    assert(0 && "TODO");
    return NOERROR;
}

ECode Collections::SynchronizedList::GetSubList(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ IList** subList)
{
    synchronized (mLock) {
        AutoPtr<IList> sub;
        mList->GetSubList(start, end, (IList**)&sub);
        AutoPtr<IList> res = new SynchronizedList(sub, mLock);
        *subList = res;
        REFCOUNT_ADD(*subList)
    }
    return NOERROR;
}

ECode Collections::SynchronizedList::WriteObject(
    /* [in] */ IObjectOutputStream* stream)
{
    synchronized (mLock) {
        stream->DefaultWriteObject();
    }
    return NOERROR;
}

ECode Collections::SynchronizedList::Add(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return SynchronizedCollection::Add(object, modified);
}

ECode Collections::SynchronizedList::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return SynchronizedCollection::AddAll(collection, modified);
}

ECode Collections::SynchronizedList::Clear()
{
    return SynchronizedCollection::Clear();
}

ECode Collections::SynchronizedList::Contains(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return SynchronizedCollection::Contains(object, result);
}

ECode Collections::SynchronizedList::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return SynchronizedCollection::ContainsAll(collection, result);
}

ECode Collections::SynchronizedList::IsEmpty(
    /* [out] */ Boolean* result)
{
    return SynchronizedCollection::IsEmpty(result);
}

ECode Collections::SynchronizedList::GetIterator(
    /* [out] */ IIterator** it)
{
    return SynchronizedCollection::GetIterator(it);
}

ECode Collections::SynchronizedList::Remove(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return SynchronizedCollection::Remove(object, modified);
}

ECode Collections::SynchronizedList::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return SynchronizedCollection::RemoveAll(collection, modified);
}

ECode Collections::SynchronizedList::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return SynchronizedCollection::RetainAll(collection, modified);
}

ECode Collections::SynchronizedList::GetSize(
    /* [out] */ Int32* size)
{
    return SynchronizedCollection::GetSize(size);
}

ECode Collections::SynchronizedList::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    return SynchronizedCollection::ToArray(array);
}

ECode Collections::SynchronizedList::ToArray(
    /* [in] */ ArrayOf<IInterface*>* inArray,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    return SynchronizedCollection::ToArray(inArray, outArray);
}

AutoPtr<IInterface> Collections::SynchronizedList::ReadResolve()
{
    if (IRandomAccess::Probe(mList) != NULL) {
        AutoPtr<SynchronizedRandomAccessList> p = new SynchronizedRandomAccessList(mList, mLock);
        return (IInterface*)p->Probe(EIID_IInterface);
    }
    return (IInterface*)this->Probe(EIID_IInterface);
}

//====================================================================
// Collections::SynchronizedMap::
//====================================================================
CAR_INTERFACE_IMPL_2(Collections::SynchronizedMap, Object, IMap, ISerializable)

Collections::SynchronizedMap::SynchronizedMap(
    /* [in] */ IMap* map)
    : mLock(NULL)
    , mIsStrongLock(FALSE)
    , mM(map)
{
}

Collections::SynchronizedMap::SynchronizedMap(
    /* [in] */ IMap* map,
    /* [in] */ Object* lock)
    : mLock(lock)
    , mM(map)
{
    if (mLock && mLock != this) {
        REFCOUNT_ADD(mLock)
        mIsStrongLock = TRUE;
    }
}

Collections::SynchronizedMap::~SynchronizedMap()
{
    if (mIsStrongLock) {
        REFCOUNT_RELEASE(mLock)
    }
}

ECode Collections::SynchronizedMap::Clear()
{
    synchronized (mLock) {
        mM->Clear();
    }
    return NOERROR;
}

ECode Collections::SynchronizedMap::ContainsKey(
    /* [in] */ IInterface* key,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    synchronized (mLock) {
        mM->ContainsKey(key, result);
    }
    return NOERROR;
}

ECode Collections::SynchronizedMap::ContainsValue(
    /* [in] */ IInterface* value,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    synchronized (mLock) {
        mM->ContainsValue(value, result);
    }
    return NOERROR;
}

ECode Collections::SynchronizedMap::GetEntrySet(
    /* [out, callee] */ ISet** entries)
{
    VALIDATE_NOT_NULL(*entries);
    synchronized (mLock) {
        AutoPtr<ISet> entry;
        mM->GetEntrySet((ISet**)&entry);
        AutoPtr<ISet> res = new SynchronizedSet(entry, mLock);
        *entries = res;
        REFCOUNT_ADD(*entries)
    }
    return NOERROR;
}

ECode Collections::SynchronizedMap::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    synchronized (mLock) {
        mM->Equals(object, result);
    }
    return NOERROR;
}

ECode Collections::SynchronizedMap::Get(
    /* [in] */ PInterface key,
    /* [out] */ PInterface* value)
{
    VALIDATE_NOT_NULL(value);
    synchronized (mLock) {
        mM->Get(key, value);
    }
    return NOERROR;
}

ECode Collections::SynchronizedMap::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode);
    synchronized (mLock) {
        mM->GetHashCode(hashCode);
    }
    return NOERROR;
}

ECode Collections::SynchronizedMap::IsEmpty(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    synchronized (mLock) {
        mM->IsEmpty(result);
    }
    return NOERROR;
}

ECode Collections::SynchronizedMap::GetKeySet(
    /* [out, callee] */ ISet** keySet)
{
    VALIDATE_NOT_NULL(keySet);
    synchronized (mLock) {
        AutoPtr<ISet> key;
        mM->GetKeySet((ISet**)&key);
        *keySet = new SynchronizedSet(key, mLock);
        REFCOUNT_ADD(*keySet)
    }
    return NOERROR;
}

ECode Collections::SynchronizedMap::Put(
    /* [in] */ PInterface key,
    /* [in] */ PInterface value,
    /* [out] */ PInterface* oldValue)
{
    synchronized (mLock) {
        mM->Put(key, value, oldValue);
    }
    return NOERROR;
}

ECode Collections::SynchronizedMap::Put(
    /* [in] */ PInterface key,
    /* [in] */ PInterface value)
{
    assert(0 && "TODO");
    return NOERROR;
}

ECode Collections::SynchronizedMap::PutAll(
    /* [in] */ IMap* map)
{
    synchronized (mLock) {
        mM->PutAll(map);
    }
    return NOERROR;
}

ECode Collections::SynchronizedMap::Remove(
    /* [in] */ PInterface key,
    /* [out] */ PInterface* value)
{
    VALIDATE_NOT_NULL(value);
    synchronized (mLock) {
        mM->Remove(key, value);
    }
    return NOERROR;
}

ECode Collections::SynchronizedMap::Remove(
    /* [in] */ PInterface key)
{
    assert(0 && "TODO");
    return NOERROR;
}

ECode Collections::SynchronizedMap::GetSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);
    synchronized (mLock) {
        mM->GetSize(size);
    }
    return NOERROR;
}

ECode Collections::SynchronizedMap::GetValues(
    /* [out] */ ICollection** value)
{
    VALIDATE_NOT_NULL(value);
    synchronized (mLock) {
        AutoPtr<ICollection> v;
        mM->GetValues((ICollection**)&v);
        AutoPtr<ICollection> res = new SynchronizedCollection(v, mLock);
        *value = res;
        REFCOUNT_ADD(*value)
    }
    return NOERROR;
}

ECode Collections::SynchronizedMap::ToString(
    /* [out] */ String* str)
{
    synchronized (mLock) {
        AutoPtr<IObject> iObj = IObject::Probe(mM);
        if (iObj != NULL) {
            return iObj->ToString(str);
        }
        *str = NULL;
    }
    return NOERROR;
}

ECode Collections::SynchronizedMap::WriteObject(
    /* [in] */ IObjectOutputStream* stream)
{
    synchronized (mLock) {
        stream->DefaultWriteObject();
    }
    return NOERROR;
}

//====================================================================
// Collections::SynchronizedSet::
//====================================================================
CAR_INTERFACE_IMPL(Collections::SynchronizedSet, SynchronizedCollection, ISet)

Collections::SynchronizedSet::SynchronizedSet(
    /* [in] */ ISet* set)
    : SynchronizedCollection(ICollection::Probe(set))
{
}

Collections::SynchronizedSet::SynchronizedSet(
    /* [in] */ ISet* set,
    /* [in] */ Object* lock)
    : SynchronizedCollection(ICollection::Probe(set), mLock)
{
}

ECode Collections::SynchronizedSet::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    synchronized (mLock) {
        mC->Equals(object, result);
    }
    return NOERROR;
}

ECode Collections::SynchronizedSet::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode);
    synchronized (mLock) {
        mC->GetHashCode(hashCode);
    }
    return NOERROR;
}

ECode Collections::SynchronizedSet::GetIterator(
    /* [out] */ IIterator** it)
{
    return SynchronizedCollection::GetIterator(it);
}

ECode Collections::SynchronizedSet::Add(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return SynchronizedCollection::Add(object, modified);
}

ECode Collections::SynchronizedSet::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return SynchronizedCollection::AddAll(collection, modified);
}

ECode Collections::SynchronizedSet::Clear()
{
    return SynchronizedCollection::Clear();
}

ECode Collections::SynchronizedSet::Contains(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return SynchronizedCollection::Contains(object, result);
}

ECode Collections::SynchronizedSet::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return SynchronizedCollection::ContainsAll(collection, result);
}

ECode Collections::SynchronizedSet::IsEmpty(
    /* [out] */ Boolean* result)
{
    return SynchronizedCollection::IsEmpty(result);
}

ECode Collections::SynchronizedSet::Remove(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return SynchronizedCollection::Remove(object, modified);
}

ECode Collections::SynchronizedSet::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return SynchronizedCollection::RemoveAll(collection, modified);
}

ECode Collections::SynchronizedSet::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return SynchronizedCollection::RetainAll(collection, modified);
}

ECode Collections::SynchronizedSet::GetSize(
    /* [out] */ Int32* size)
{
    return SynchronizedCollection::GetSize(size);
}

ECode Collections::SynchronizedSet::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    return SynchronizedCollection::ToArray(array);
}

ECode Collections::SynchronizedSet::ToArray(
    /* [in] */ ArrayOf<IInterface*>* inArray,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    return SynchronizedCollection::ToArray(inArray, outArray);
}

ECode Collections::SynchronizedSet::WriteObject(
    /* [in] */ IObjectOutputStream* stream)
{
    synchronized (mLock) {
        stream->DefaultWriteObject();
    }
    return NOERROR;
}

//====================================================================
// Collections::SynchronizedSortedMap::
//====================================================================
CAR_INTERFACE_IMPL(Collections::SynchronizedSortedMap, SynchronizedMap, ISortedMap)

Collections::SynchronizedSortedMap::SynchronizedSortedMap(
    /* [in] */ ISortedMap* map)
    : SynchronizedMap(IMap::Probe(map))
{
    mSm = map;
}

Collections::SynchronizedSortedMap::SynchronizedSortedMap(
    /* [in] */ ISortedMap* map,
    /* [in] */ Object* lock)
    : SynchronizedMap(IMap::Probe(map), lock)
{
    mSm = map;
}

ECode Collections::SynchronizedSortedMap::GetComparator(
    /* [out] */ IComparator** comp)
{
    VALIDATE_NOT_NULL(comp);
    synchronized (mLock) {
        mSm->GetComparator(comp);
    }
    return NOERROR;
}

ECode Collections::SynchronizedSortedMap::GetFirstKey(
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface);
    synchronized (mLock) {
        mSm->GetFirstKey(outface);
    }
    return NOERROR;
}

ECode Collections::SynchronizedSortedMap::GetHeadMap(
    /* [in] */ IInterface* endKey,
    /* [out] */ ISortedMap** sortmap)
{
    VALIDATE_NOT_NULL(sortmap);
    synchronized (mLock) {
        AutoPtr<ISortedMap> map;
        mSm->GetHeadMap(endKey, (ISortedMap**)&map);
        AutoPtr<ISortedMap> res = new SynchronizedSortedMap(map, mLock);
        *sortmap = res;
        REFCOUNT_ADD(*sortmap)
    }
    return NOERROR;
}

ECode Collections::SynchronizedSortedMap::GetLastKey(
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface);
    synchronized (mLock) {
        mSm->GetLastKey(outface);
    }
    return NOERROR;
}

ECode Collections::SynchronizedSortedMap::GetSubMap(
    /* [in] */ IInterface* startKey,
    /* [in] */ IInterface* endKey,
    /* [out] */ ISortedMap** sortmap)
{
    VALIDATE_NOT_NULL(sortmap);
    synchronized (mLock) {
        AutoPtr<ISortedMap> map;
        mSm->GetSubMap(startKey, endKey, (ISortedMap**)&map);
        AutoPtr<ISortedMap> res = new SynchronizedSortedMap(map, mLock);
        *sortmap = res;
        REFCOUNT_ADD(*sortmap)
    }
    return NOERROR;
}

ECode Collections::SynchronizedSortedMap::GetTailMap(
    /* [in] */ IInterface* startKey,
    /* [out] */ ISortedMap** sortmap)
{
    VALIDATE_NOT_NULL(sortmap);
    synchronized (mLock) {
        AutoPtr<ISortedMap> map;
        mSm->GetTailMap(startKey, (ISortedMap**)&map);
        AutoPtr<ISortedMap> res = new SynchronizedSortedMap(map, mLock);
        *sortmap = res;
        REFCOUNT_ADD(*sortmap)
    }
    return NOERROR;
}

ECode Collections::SynchronizedSortedMap::Clear()
{
    return SynchronizedMap::Clear();
}

ECode Collections::SynchronizedSortedMap::ContainsKey(
    /* [in] */ IInterface* key,
    /* [out] */ Boolean* result)
{
    return SynchronizedMap::ContainsKey(key, result);
}

ECode Collections::SynchronizedSortedMap::ContainsValue(
    /* [in] */ IInterface* value,
    /* [out] */ Boolean* result)
{
    return SynchronizedMap::ContainsValue(value, result);
}

ECode Collections::SynchronizedSortedMap::GetEntrySet(
    /* [out, callee] */ ISet** entries)
{
    return SynchronizedMap::GetEntrySet(entries);
}

ECode Collections::SynchronizedSortedMap::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return SynchronizedMap::Equals(object, result);
}

ECode Collections::SynchronizedSortedMap::Get(
    /* [in] */ PInterface key,
    /* [out] */ PInterface* value)
{
    return SynchronizedMap::Get(key, value);
}

ECode Collections::SynchronizedSortedMap::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    return SynchronizedMap::GetHashCode(hashCode);
}

ECode Collections::SynchronizedSortedMap::IsEmpty(
    /* [out] */ Boolean* result)
{
    return SynchronizedMap::IsEmpty(result);
}

ECode Collections::SynchronizedSortedMap::GetKeySet(
    /* [out, callee] */ ISet** keySet)
{
    return SynchronizedMap::GetKeySet(keySet);
}

ECode Collections::SynchronizedSortedMap::GetValues(
    /* [out] */ ICollection** value)
{
    return SynchronizedMap::GetValues(value);
}

ECode Collections::SynchronizedSortedMap::Put(
    /* [in] */ PInterface key,
    /* [in] */ PInterface value,
    /* [out] */ PInterface* oldValue)
{
    return SynchronizedMap::Put(key, value, oldValue);
}

ECode Collections::SynchronizedSortedMap::Put(
    /* [in] */ PInterface key,
    /* [in] */ PInterface value)
{
    assert(0 && "TODO");
    return NOERROR;
}

ECode Collections::SynchronizedSortedMap::PutAll(
    /* [in] */ IMap* map)
{
    return SynchronizedMap::PutAll(map);
}

ECode Collections::SynchronizedSortedMap::Remove(
    /* [in] */ PInterface key,
    /* [out] */ PInterface* value)
{
    return SynchronizedMap::Remove(key, value);
}

ECode Collections::SynchronizedSortedMap::Remove(
    /* [in] */ PInterface key)
{
    assert(0 && "TODO");
    return NOERROR;
}

ECode Collections::SynchronizedSortedMap::GetSize(
    /* [out] */ Int32* size)
{
    return SynchronizedMap::GetSize(size);
}

ECode Collections::SynchronizedSortedMap::WriteObject(
    /* [in] */ IObjectOutputStream* stream)
{
    synchronized (mLock) {
        stream->DefaultWriteObject();
    }
    return NOERROR;
}

//====================================================================
// Collections::SynchronizedSortedSet::
//====================================================================
CAR_INTERFACE_IMPL(Collections::SynchronizedSortedSet, SynchronizedSet, ISortedSet)

Collections::SynchronizedSortedSet::SynchronizedSortedSet(
    /* [in] */ ISortedSet* set)
    : SynchronizedSet(ISet::Probe(set))
{
    mSs = set;
}

Collections::SynchronizedSortedSet::SynchronizedSortedSet(
    /* [in] */ ISortedSet* set,
    /* [in] */ Object* lock)
    : SynchronizedSet(ISet::Probe(set), lock)
{
    mSs = set;
}

ECode Collections::SynchronizedSortedSet::GetComparator(
    /* [out] */ IComparator** outcom)
{
    VALIDATE_NOT_NULL(outcom);
    synchronized (mLock) {
        mSs->GetComparator(outcom);
    }
    return NOERROR;
}

ECode Collections::SynchronizedSortedSet::GetFirst(
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface);
    synchronized (mLock) {
        mSs->GetFirst(outface);
    }
    return NOERROR;
}

ECode Collections::SynchronizedSortedSet::GetHeadSet(
    /* [in] */ IInterface* end,
    /* [out] */ ISortedSet** outsort)
{
    VALIDATE_NOT_NULL(outsort);
    synchronized (mLock) {
        AutoPtr<ISortedSet> sort;
        mSs->GetHeadSet(end, (ISortedSet**)&sort);
        AutoPtr<ISortedSet> res = new SynchronizedSortedSet(sort, mLock);
        *outsort = res;
        REFCOUNT_ADD(*outsort)
    }
    return NOERROR;
}

ECode Collections::SynchronizedSortedSet::GetLast(
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface);
    synchronized (mLock) {
        mSs->GetFirst(outface);
    }
    return NOERROR;
}

ECode Collections::SynchronizedSortedSet::GetSubSet(
    /* [in] */ IInterface* start,
    /* [in] */ IInterface* end,
    /* [out] */ ISortedSet** outsort)
{
    VALIDATE_NOT_NULL(outsort);
    synchronized (mLock) {
        AutoPtr<ISortedSet> set;
        mSs->GetSubSet(start, end, (ISortedSet**)&set);
        AutoPtr<ISortedSet> res = new SynchronizedSortedSet(set, mLock);
        *outsort = res;
        REFCOUNT_ADD(*outsort)
    }
    return NOERROR;
}

ECode Collections::SynchronizedSortedSet::GetTailSet(
    /* [in] */ IInterface* start,
    /* [out] */ ISortedSet** outsort)
{
    synchronized (mLock) {
        AutoPtr<ISortedSet> set;
        mSs->GetTailSet(start, (ISortedSet**)&set);
        AutoPtr<ISortedSet> res = new SynchronizedSortedSet(set, mLock);
        *outsort = res;
        REFCOUNT_ADD(*outsort)
    }
    return NOERROR;
}

ECode Collections::SynchronizedSortedSet::Add(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return SynchronizedSet::Add(object, modified);
}

ECode Collections::SynchronizedSortedSet::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return SynchronizedSet::AddAll(collection, modified);
}

ECode Collections::SynchronizedSortedSet::Clear()
{
    return SynchronizedSet::Clear();
}

ECode Collections::SynchronizedSortedSet::Contains(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return SynchronizedSet::Contains(object, result);
}

ECode Collections::SynchronizedSortedSet::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return SynchronizedSet::ContainsAll(collection, result);
}

ECode Collections::SynchronizedSortedSet::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return SynchronizedSet::Equals(object, result);
}

ECode Collections::SynchronizedSortedSet::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    return SynchronizedSet::GetHashCode(hashCode);
}

ECode Collections::SynchronizedSortedSet::GetIterator(
    /* [out] */ IIterator** it)
{
    return SynchronizedSet::GetIterator(it);
}

ECode Collections::SynchronizedSortedSet::IsEmpty(
    /* [out] */ Boolean* result)
{
    return SynchronizedSet::IsEmpty(result);
}

ECode Collections::SynchronizedSortedSet::Remove(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return SynchronizedSet::Remove(object, modified);
}

ECode Collections::SynchronizedSortedSet::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return SynchronizedSet::RemoveAll(collection, modified);
}

ECode Collections::SynchronizedSortedSet::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return SynchronizedSet::RetainAll(collection, modified);
}

ECode Collections::SynchronizedSortedSet::GetSize(
    /* [out] */ Int32* size)
{
    return SynchronizedSet::GetSize(size);
}

ECode Collections::SynchronizedSortedSet::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    return SynchronizedSet::ToArray(array);
}

ECode Collections::SynchronizedSortedSet::ToArray(
    /* [in] */ ArrayOf<IInterface*>* inArray,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    return SynchronizedSet::ToArray(inArray, outArray);
}

ECode Collections::SynchronizedSortedSet::WriteObject(
    /* [in] */ IObjectOutputStream* stream)
{
    synchronized (mLock) {
        stream->DefaultWriteObject();
    }
    return NOERROR;
}

//====================================================================
// Collections::UnmodifiableCollection::Iterator::
//====================================================================
CAR_INTERFACE_IMPL(Collections::UnmodifiableCollection::Iterator, Object, IIterator)

Collections::UnmodifiableCollection::Iterator::Iterator(
    /* [in] */ UnmodifiableCollection* owner)
{
    (IIterable::Probe(owner->mC))->GetIterator((IIterator**)&mIterator);
}

ECode Collections::UnmodifiableCollection::Iterator::HasNext(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return mIterator->HasNext(result);
}

ECode Collections::UnmodifiableCollection::Iterator::GetNext(
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object);
    return mIterator->GetNext(object);
}

ECode Collections::UnmodifiableCollection::Iterator::Remove()
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

//====================================================================
// Collections::UnmodifiableCollection::
//====================================================================
CAR_INTERFACE_IMPL_2(Collections::UnmodifiableCollection, Object, ICollection, ISerializable)

Collections::UnmodifiableCollection::UnmodifiableCollection(
    /* [in] */ ICollection* collection)
{
    mC = collection;
}

ECode Collections::UnmodifiableCollection::Add(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode Collections::UnmodifiableCollection::Add(
    /* [in] */ IInterface* object)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode Collections::UnmodifiableCollection::AddAll(
    /* [in] */ ICollection* collection)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode Collections::UnmodifiableCollection::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode Collections::UnmodifiableCollection::Clear()
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode Collections::UnmodifiableCollection::Contains(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return mC->Contains(object, result);
}

ECode Collections::UnmodifiableCollection::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return mC->ContainsAll(collection, result);
}

ECode Collections::UnmodifiableCollection::IsEmpty(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return mC->IsEmpty(result);
}

ECode Collections::UnmodifiableCollection::GetIterator(
    /* [out] */ IIterator** it)
{
    VALIDATE_NOT_NULL(it);
    AutoPtr<IIterator> res = new Iterator(this);
    *it = res;
    REFCOUNT_ADD(*it)
    return NOERROR;
}

ECode Collections::UnmodifiableCollection::Remove(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode Collections::UnmodifiableCollection::Remove(
    /* [in] */ IInterface* object)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode Collections::UnmodifiableCollection::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode Collections::UnmodifiableCollection::RemoveAll(
    /* [in] */ ICollection* collection)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode Collections::UnmodifiableCollection::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode Collections::UnmodifiableCollection::RetainAll(
    /* [in] */ ICollection* collection)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode Collections::UnmodifiableCollection::GetSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);
    return mC->GetSize(size);
}

ECode Collections::UnmodifiableCollection::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    VALIDATE_NOT_NULL(array);
    return mC->ToArray(array);
}

ECode Collections::UnmodifiableCollection::ToArray(
    /* [in] */ ArrayOf<IInterface*>* inArray,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    VALIDATE_NOT_NULL(outArray);
    return mC->ToArray(inArray, outArray);
}

ECode Collections::UnmodifiableCollection::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    AutoPtr<IObject> iObj = IObject::Probe(mC);
    if (iObj != NULL) {
        return iObj->ToString(str);
    }
    *str = NULL;
    return NOERROR;
}

ECode Collections::UnmodifiableCollection::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = this->Probe(EIID_IInterface) == object;
    return NOERROR;
}

ECode Collections::UnmodifiableCollection::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode);
    *hashCode = (Int32)this;
    return NOERROR;
}

//====================================================================
// Collections::UnmodifiableRandomAccessList::
//====================================================================
CAR_INTERFACE_IMPL(Collections::UnmodifiableRandomAccessList, UnmodifiableList, IRandomAccess)

Collections::UnmodifiableRandomAccessList::UnmodifiableRandomAccessList(
    /* [in] */ IList* l)
    : UnmodifiableList(l)
{
}

ECode Collections::UnmodifiableRandomAccessList::GetSubList(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ IList** subList)
{
    VALIDATE_NOT_NULL(subList);
    AutoPtr<IList> l;
    mList->GetSubList(start, end, (IList**)&l);
    AutoPtr<IList> res = new UnmodifiableRandomAccessList(l);
    *subList = res;
    REFCOUNT_ADD(*subList)
    return NOERROR;
}

AutoPtr<IInterface> Collections::UnmodifiableRandomAccessList::WriteReplace()
{
    AutoPtr<UnmodifiableList> res = new UnmodifiableList(mList);
    return res->Probe(EIID_IInterface);
}

//====================================================================
// Collections::UnmodifiableList::ListIterator::
//====================================================================
CAR_INTERFACE_IMPL(Collections::UnmodifiableList::ListIterator, Object, IListIterator)

Collections::UnmodifiableList::ListIterator::ListIterator(
    /* [in] */ UnmodifiableList* owner,
    /* [in] */ Int32 location)
{
    owner->mList->GetListIterator(location, (IListIterator**)&mIterator);
}

ECode Collections::UnmodifiableList::ListIterator::Add(
    /* [in] */ IInterface* object)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode Collections::UnmodifiableList::ListIterator::HasNext(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return (IIterator::Probe(mIterator))->HasNext(result);
}

ECode Collections::UnmodifiableList::ListIterator::HasPrevious(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return mIterator->HasPrevious(result);
}

ECode Collections::UnmodifiableList::ListIterator::GetNext(
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object);
    return (IIterator::Probe(mIterator))->GetNext(object);
}

ECode Collections::UnmodifiableList::ListIterator::GetNextIndex(
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);
    return mIterator->GetNextIndex(index);
}

ECode Collections::UnmodifiableList::ListIterator::GetPrevious(
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object);
    return mIterator->GetPrevious(object);
}

ECode Collections::UnmodifiableList::ListIterator::GetPreviousIndex(
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);
    return mIterator->GetPreviousIndex(index);
}

ECode Collections::UnmodifiableList::ListIterator::Remove()
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode Collections::UnmodifiableList::ListIterator::Set(
    /* [in] */ IInterface* object)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

//====================================================================
// Collections::UnmodifiableList::
//====================================================================
CAR_INTERFACE_IMPL(Collections::UnmodifiableList, UnmodifiableCollection, IList)

Collections::UnmodifiableList::UnmodifiableList(
    /* [in] */ IList* l)
    : UnmodifiableCollection(ICollection::Probe(l))
{
    mList = l;
}

ECode Collections::UnmodifiableList::Add(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return UnmodifiableCollection::Add(object, modified);
}

ECode Collections::UnmodifiableList::Add(
    /* [in] */ IInterface* object)
{
    assert(0 && "TODO");
    return NOERROR;
}

ECode Collections::UnmodifiableList::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return UnmodifiableCollection::AddAll(collection, modified);
}

ECode Collections::UnmodifiableList::AddAll(
    /* [in] */ Int32 location,
    /* [in] */ ICollection* collection)
{
    assert(0 && "TODO");
    return NOERROR;
}

ECode Collections::UnmodifiableList::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return (ICollection::Probe(mList))->Equals(object, result);
}

ECode Collections::UnmodifiableList::Get(
    /* [in] */ Int32 location,
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object);
    return mList->Get(location, object);
}

ECode Collections::UnmodifiableList::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode);
    return (ICollection::Probe(mList))->GetHashCode(hashCode);
}

ECode Collections::UnmodifiableList::IndexOf(
    /* [in] */ IInterface* object,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);
    return mList->IndexOf(object, index);
}

ECode Collections::UnmodifiableList::LastIndexOf(
    /* [in] */ IInterface* object,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);
    return mList->LastIndexOf(object, index);
}

ECode Collections::UnmodifiableList::GetListIterator(
    /* [out] */ IListIterator** it)
{
    VALIDATE_NOT_NULL(it);
    return GetListIterator(0, it);
}

ECode Collections::UnmodifiableList::GetListIterator(
    /* [in] */ Int32 location,
    /* [out] */ IListIterator** it)
{
    VALIDATE_NOT_NULL(it);
    AutoPtr<IListIterator> res = new ListIterator(this, location);
    *it = res;
    REFCOUNT_ADD(*it)
    return NOERROR;
}

ECode Collections::UnmodifiableList::Remove(
    /* [in] */ Int32 location,
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object);
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode Collections::UnmodifiableList::Remove(
    /* [in] */ Int32 location)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode Collections::UnmodifiableList::Set(
    /* [in] */ Int32 location,
    /* [in] */ IInterface* object,
    /* [out] */ IInterface** prevObject)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode Collections::UnmodifiableList::Set(
    /* [in] */ Int32 location,
    /* [in] */ IInterface* object)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode Collections::UnmodifiableList::GetSubList(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ IList** subList)
{
    VALIDATE_NOT_NULL(subList);
    AutoPtr<IList> sub;
    mList->GetSubList(start, end, (IList**)&sub);
    AutoPtr<IList> res = new UnmodifiableList(sub);
    *subList = res;
    REFCOUNT_ADD(*subList)
    return NOERROR;
}

ECode Collections::UnmodifiableList::Clear()
{
    return UnmodifiableCollection::Clear();
}

ECode Collections::UnmodifiableList::Contains(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return UnmodifiableCollection::Contains(object, result);
}

ECode Collections::UnmodifiableList::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return UnmodifiableCollection::ContainsAll(collection, result);
}

ECode Collections::UnmodifiableList::IsEmpty(
    /* [out] */ Boolean* result)
{
    return UnmodifiableCollection::IsEmpty(result);
}

ECode Collections::UnmodifiableList::GetIterator(
    /* [out] */ IIterator** it)
{
    return UnmodifiableCollection::GetIterator(it);
}

ECode Collections::UnmodifiableList::Remove(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return UnmodifiableCollection::Remove(object, modified);
}

ECode Collections::UnmodifiableList::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return UnmodifiableCollection::RemoveAll(collection, modified);
}

ECode Collections::UnmodifiableList::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return UnmodifiableCollection::RetainAll(collection, modified);
}

ECode Collections::UnmodifiableList::GetSize(
    /* [out] */ Int32* size)
{
    return UnmodifiableCollection::GetSize(size);
}

ECode Collections::UnmodifiableList::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    return UnmodifiableCollection::ToArray(array);
}

ECode Collections::UnmodifiableList::ToArray(
    /* [in] */ ArrayOf<IInterface*>* inArray,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    return UnmodifiableCollection::ToArray(inArray, outArray);
}

ECode Collections::UnmodifiableList::Add(
    /* [in] */ Int32 location,
    /* [in] */ IInterface* object)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode Collections::UnmodifiableList::AddAll(
    /* [in] */ Int32 location,
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

AutoPtr<IInterface> Collections::UnmodifiableList::ReadResolve()
{
    if (IRandomAccess::Probe(mList) != NULL ) {
        AutoPtr<UnmodifiableRandomAccessList> res = new UnmodifiableRandomAccessList(mList);
        return res->Probe(EIID_IInterface);
    }
    return this->Probe(EIID_IInterface);
}

//====================================================================
// Collections::UnmodifiableMap::UnmodifiableEntrySet::UnmodifiableMapEntry::
//====================================================================
CAR_INTERFACE_IMPL(Collections::UnmodifiableMap::UnmodifiableEntrySet::UnmodifiableMapEntry, Object, IMapEntry)

Collections::UnmodifiableMap::UnmodifiableEntrySet::UnmodifiableMapEntry::UnmodifiableMapEntry(
    /* [in] */ IMapEntry* entry)
{
    mMapEntry = entry;
}

ECode Collections::UnmodifiableMap::UnmodifiableEntrySet::UnmodifiableMapEntry::Equals(
    /* [in] */ IInterface* entry,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return mMapEntry->Equals(entry, result);
}

ECode Collections::UnmodifiableMap::UnmodifiableEntrySet::UnmodifiableMapEntry::GetKey(
    /* [out] */ PInterface* key)
{
    VALIDATE_NOT_NULL(key);
    return mMapEntry->GetKey(key);
}

ECode Collections::UnmodifiableMap::UnmodifiableEntrySet::UnmodifiableMapEntry::GetValue(
    /* [out] */ PInterface* value)
{
    VALIDATE_NOT_NULL(value);
    return mMapEntry->GetValue(value);
}

ECode Collections::UnmodifiableMap::UnmodifiableEntrySet::UnmodifiableMapEntry::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode);
    return mMapEntry->GetHashCode(hashCode);
}

ECode Collections::UnmodifiableMap::UnmodifiableEntrySet::UnmodifiableMapEntry::SetValue(
    /* [in] */ IInterface* valueReplacer,
    /* [out] */ IInterface** valueReplacee)
{
    VALIDATE_NOT_NULL(valueReplacee);
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode Collections::UnmodifiableMap::UnmodifiableEntrySet::UnmodifiableMapEntry::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    AutoPtr<IObject> iObj = IObject::Probe(mMapEntry);
    if (iObj != NULL) {
        return iObj->ToString(str);
    }
    *str = NULL;
    return NOERROR;
}

//====================================================================
// Collections::UnmodifiableMap::UnmodifiableEntrySet::Iterator::
//====================================================================
CAR_INTERFACE_IMPL(Collections::UnmodifiableMap::UnmodifiableEntrySet::Iterator, Object, IIterator)

Collections::UnmodifiableMap::UnmodifiableEntrySet::Iterator::Iterator(
    /* [in] */ UnmodifiableEntrySet* owner)
{
    (IIterable::Probe(owner->mC))->GetIterator((IIterator**)&mIterator);
}

ECode Collections::UnmodifiableMap::UnmodifiableEntrySet::Iterator::HasNext(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return mIterator->HasNext(result);
}

ECode Collections::UnmodifiableMap::UnmodifiableEntrySet::Iterator::GetNext(
    /* [out] */ IMapEntry** object)
{
    AutoPtr<IMapEntry> o;
    mIterator->GetNext((IMapEntry**)&o);
    AutoPtr<IMapEntry> res = new UnmodifiableMapEntry(o);
    *object = res;
    REFCOUNT_ADD(*object)
    return NOERROR;
}

ECode Collections::UnmodifiableMap::UnmodifiableEntrySet::Iterator::GetNext(
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object);
    AutoPtr<IMapEntry> o;
    GetNext((IMapEntry**)&o);
    *object = o->Probe(EIID_IInterface);
    REFCOUNT_ADD(*object)
    return NOERROR;
}

ECode Collections::UnmodifiableMap::UnmodifiableEntrySet::Iterator::Remove()
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

//====================================================================
// Collections::UnmodifiableMap::UnmodifiableEntrySet::
//====================================================================

Collections::UnmodifiableMap::UnmodifiableEntrySet::UnmodifiableEntrySet(
    /* [in] */ ISet* set)
    : UnmodifiableSet(set)
{
}

ECode Collections::UnmodifiableMap::UnmodifiableEntrySet::GetIterator(
    /* [out] */ IIterator** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<IIterator> res = new Iterator(this);
    *result = res;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode Collections::UnmodifiableMap::UnmodifiableEntrySet::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    VALIDATE_NOT_NULL(array);
    Int32 length;
    mC->GetSize(&length);
    AutoPtr<ArrayOf<IInterface*> > result = ArrayOf<IInterface*>::Alloc(length);
    AutoPtr<IIterator> it;
    GetIterator((IIterator**)&it);
    for (Int32 i = length; --i >= 0;) {
        it->GetNext(&(*result)[i]);
    }
    *array = result;
    REFCOUNT_ADD(*array)
    return NOERROR;
}

ECode Collections::UnmodifiableMap::UnmodifiableEntrySet::ToArray(
    /* [in] */ ArrayOf<IInterface*>* inArray,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    VALIDATE_NOT_NULL(outArray);
    Int32 size, index = 0;
    mC->GetSize(&size);
    AutoPtr<IIterator> it;
    GetIterator((IIterator**)&it);
    if (size > inArray->GetLength()) {
        inArray = ArrayOf<IInterface*>::Alloc(size);
    }
    while (index < size) {
        AutoPtr<IInterface> o;
        it->GetNext((IInterface**)&o);
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
// Collections::UnmodifiableMap::
//====================================================================
CAR_INTERFACE_IMPL_2(Collections::UnmodifiableMap, Object, IMap, ISerializable)

Collections::UnmodifiableMap::UnmodifiableMap(
    /* [in] */ IMap* map)
{
    mM = map;
}

ECode Collections::UnmodifiableMap::Clear()
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode Collections::UnmodifiableMap::ContainsKey(
    /* [in] */ IInterface* key,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return mM->ContainsKey(key, result);
}

ECode Collections::UnmodifiableMap::ContainsValue(
    /* [in] */ IInterface* value,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return mM->ContainsValue(value, result);
}

ECode Collections::UnmodifiableMap::GetEntrySet(
    /* [out, callee] */ ISet** entries)
{
    VALIDATE_NOT_NULL(entries);
    AutoPtr<ISet> set;
    mM->GetEntrySet((ISet**)&set);
    AutoPtr<ISet> res = new UnmodifiableEntrySet(set);
    *entries = res;
    REFCOUNT_ADD(*entries)
    return NOERROR;
}

ECode Collections::UnmodifiableMap::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return mM->Equals(object, result);
}

ECode Collections::UnmodifiableMap::Get(
    /* [in] */ PInterface key,
    /* [out] */ PInterface* value)
{
    VALIDATE_NOT_NULL(value);
    return mM->Get(key, value);
}

ECode Collections::UnmodifiableMap::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode);
    return mM->GetHashCode(hashCode);
}

ECode Collections::UnmodifiableMap::IsEmpty(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return mM->IsEmpty(result);
}

ECode Collections::UnmodifiableMap::GetKeySet(
    /* [out, callee] */ ISet** keySet)
{
    VALIDATE_NOT_NULL(keySet);
    AutoPtr<ISet> set;
    mM->GetKeySet((ISet**)&set);
    AutoPtr<ISet> res = new UnmodifiableSet(set);
    *keySet = res;
    REFCOUNT_ADD(*keySet)
    return NOERROR;
}

ECode Collections::UnmodifiableMap::Put(
    /* [in] */ PInterface key,
    /* [in] */ PInterface value,
    /* [out] */ PInterface* oldValue)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode Collections::UnmodifiableMap::Put(
    /* [in] */ PInterface key,
    /* [in] */ PInterface value)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode Collections::UnmodifiableMap::PutAll(
    /* [in] */ IMap* map)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode Collections::UnmodifiableMap::Remove(
    /* [in] */ PInterface key,
    /* [out] */ PInterface* value)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode Collections::UnmodifiableMap::Remove(
    /* [in] */ PInterface key)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode Collections::UnmodifiableMap::GetSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);
    return mM->GetSize(size);
}

ECode Collections::UnmodifiableMap::GetValues(
    /* [out] */ ICollection** value)
{
    VALIDATE_NOT_NULL(value);
    AutoPtr<ICollection> v;
    mM->GetValues((ICollection**)&v);
    AutoPtr<ICollection> res = new UnmodifiableCollection(v);
    *value = res;
    REFCOUNT_ADD(*value)
    return NOERROR;
}

ECode Collections::UnmodifiableMap::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    AutoPtr<IObject> iObj;
    iObj = IObject::Probe(mM);
    if (iObj != NULL) {
        return iObj->ToString(str);
    }
    *str = NULL;
    return NOERROR;
}

//====================================================================
// Collections::UnmodifiableSet::
//====================================================================
CAR_INTERFACE_IMPL(Collections::UnmodifiableSet, UnmodifiableCollection, ISet)

Collections::UnmodifiableSet::UnmodifiableSet(
    /* [in] */ ISet* set)
    : UnmodifiableCollection(ICollection::Probe(set))
{
}

ECode Collections::UnmodifiableSet::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return mC->Equals(object, result);
}

ECode Collections::UnmodifiableSet::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode);
    return mC->GetHashCode(hashCode);
}

ECode Collections::UnmodifiableSet::Add(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return UnmodifiableCollection::Add(object, modified);
}

ECode Collections::UnmodifiableSet::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return UnmodifiableCollection::AddAll(collection, modified);
}

ECode Collections::UnmodifiableSet::Clear()
{
    return UnmodifiableCollection::Clear();
}

ECode Collections::UnmodifiableSet::Contains(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return UnmodifiableCollection::Contains(object, result);
}

ECode Collections::UnmodifiableSet::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return UnmodifiableCollection::ContainsAll(collection, result);
}

ECode Collections::UnmodifiableSet::IsEmpty(
    /* [out] */ Boolean* result)
{
    return UnmodifiableCollection::IsEmpty(result);
}

ECode Collections::UnmodifiableSet::GetIterator(
    /* [out] */ IIterator** it)
{
    return UnmodifiableCollection::GetIterator(it);
}

ECode Collections::UnmodifiableSet::Remove(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return UnmodifiableCollection::Remove(object, modified);
}

ECode Collections::UnmodifiableSet::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return UnmodifiableCollection::RemoveAll(collection, modified);
}

ECode Collections::UnmodifiableSet::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return UnmodifiableCollection::RetainAll(collection, modified);
}

ECode Collections::UnmodifiableSet::GetSize(
    /* [out] */ Int32* size)
{
    return UnmodifiableCollection::GetSize(size);
}

ECode Collections::UnmodifiableSet::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    return UnmodifiableCollection::ToArray(array);
}

ECode Collections::UnmodifiableSet::ToArray(
    /* [in] */ ArrayOf<IInterface*>* inArray,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    return UnmodifiableCollection::ToArray(inArray, outArray);
}

//====================================================================
// Collections::UnmodifiableSortedMap::
//====================================================================
CAR_INTERFACE_IMPL(Collections::UnmodifiableSortedMap, UnmodifiableMap, ISortedMap)

Collections::UnmodifiableSortedMap::UnmodifiableSortedMap(
    /* [in] */ ISortedMap* map)
    : UnmodifiableMap(IMap::Probe(map))
{
    mSm = map;
}

ECode Collections::UnmodifiableSortedMap::GetComparator(
    /* [out] */ IComparator** comp)
{
    VALIDATE_NOT_NULL(comp);
    return mSm->GetComparator(comp);
}

ECode Collections::UnmodifiableSortedMap::GetFirstKey(
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface);
    return mSm->GetFirstKey(outface);
}

ECode Collections::UnmodifiableSortedMap::GetHeadMap(
    /* [in] */ IInterface* endKey,
    /* [out] */ ISortedMap** sortmap)
{
    VALIDATE_NOT_NULL(sortmap);
    return mSm->GetHeadMap(endKey, sortmap);
}

ECode Collections::UnmodifiableSortedMap::GetLastKey(
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface);
    return mSm->GetLastKey(outface);
}

ECode Collections::UnmodifiableSortedMap::GetSubMap(
    /* [in] */ IInterface* startKey,
    /* [in] */ IInterface* endKey,
    /* [out] */ ISortedMap** sortmap)
{
    VALIDATE_NOT_NULL(sortmap);
    return mSm->GetSubMap(startKey, endKey, sortmap);
}

ECode Collections::UnmodifiableSortedMap::GetTailMap(
    /* [in] */ IInterface* startKey,
    /* [out] */ ISortedMap** sortmap)
{
    VALIDATE_NOT_NULL(sortmap);
    return mSm->GetTailMap(startKey, sortmap);
}

ECode Collections::UnmodifiableSortedMap::Clear()
{
    return UnmodifiableMap::Clear();
}

ECode Collections::UnmodifiableSortedMap::ContainsKey(
    /* [in] */ IInterface* key,
    /* [out] */ Boolean* result)
{
    return UnmodifiableMap::ContainsKey(key, result);
}

ECode Collections::UnmodifiableSortedMap::ContainsValue(
    /* [in] */ IInterface* value,
    /* [out] */ Boolean* result)
{
    return UnmodifiableMap::ContainsValue(value, result);
}

ECode Collections::UnmodifiableSortedMap::GetEntrySet(
    /* [out, callee] */ ISet** entries)
{
    return UnmodifiableMap::GetEntrySet(entries);
}

ECode Collections::UnmodifiableSortedMap::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return UnmodifiableMap::Equals(object, result);
}

ECode Collections::UnmodifiableSortedMap::Get(
    /* [in] */ PInterface key,
    /* [out] */ PInterface* value)
{
    return UnmodifiableMap::Get(key, value);
}

ECode Collections::UnmodifiableSortedMap::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    return UnmodifiableMap::GetHashCode(hashCode);
}

ECode Collections::UnmodifiableSortedMap::IsEmpty(
    /* [out] */ Boolean* result)
{
    return UnmodifiableMap::IsEmpty(result);
}

ECode Collections::UnmodifiableSortedMap::GetKeySet(
    /* [out, callee] */ ISet** keySet)
{
    return UnmodifiableMap::GetKeySet(keySet);
}

ECode Collections::UnmodifiableSortedMap::Put(
    /* [in] */ PInterface key,
    /* [in] */ PInterface value,
    /* [out] */ PInterface* oldValue)
{
    return UnmodifiableMap::Put(key, value, oldValue);
}

ECode Collections::UnmodifiableSortedMap::PutAll(
    /* [in] */ IMap* map)
{
    return UnmodifiableMap::PutAll(map);
}

ECode Collections::UnmodifiableSortedMap::Remove(
    /* [in] */ PInterface key,
    /* [out] */ PInterface* value)
{
    return UnmodifiableMap::Remove(key, value);
}

ECode Collections::UnmodifiableSortedMap::GetSize(
    /* [out] */ Int32* size)
{
    return UnmodifiableMap::GetSize(size);
}

ECode Collections::UnmodifiableSortedMap::GetValues(
    /* [out] */ ICollection** value)
{
    return UnmodifiableMap::GetValues(value);
}

//====================================================================
// Collections::UnmodifiableSortedSet::
//====================================================================
CAR_INTERFACE_IMPL(Collections::UnmodifiableSortedSet, UnmodifiableSet, ISortedSet)

Collections::UnmodifiableSortedSet::UnmodifiableSortedSet(
    /* [in] */ ISortedSet* set)
    : UnmodifiableSet(ISet::Probe(set))
{
    mSs = set;
}

ECode Collections::UnmodifiableSortedSet::GetComparator(
    /* [out] */ IComparator** outcom)
{
    VALIDATE_NOT_NULL(outcom);
    return mSs->GetComparator(outcom);
}

ECode Collections::UnmodifiableSortedSet::GetFirst(
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface);
    return mSs->GetFirst(outface);
}

ECode Collections::UnmodifiableSortedSet::GetHeadSet(
    /* [in] */ IInterface* end,
    /* [out] */ ISortedSet** outsort)
{
    VALIDATE_NOT_NULL(outsort);
    return mSs->GetHeadSet(end, outsort);
}

ECode Collections::UnmodifiableSortedSet::GetLast(
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface);
    return mSs->GetLast(outface);
}

ECode Collections::UnmodifiableSortedSet::GetSubSet(
    /* [in] */ IInterface* start,
    /* [in] */ IInterface* end,
    /* [out] */ ISortedSet** outsort)
{
    VALIDATE_NOT_NULL(outsort);
    return mSs->GetSubSet(start, end, outsort);
}

ECode Collections::UnmodifiableSortedSet::GetTailSet(
    /* [in] */ IInterface* start,
    /* [out] */ ISortedSet** outsort)
{
    VALIDATE_NOT_NULL(outsort);
    return mSs->GetTailSet(start, outsort);
}

ECode Collections::UnmodifiableSortedSet::Add(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return UnmodifiableCollection::Add(object, modified);
}

ECode Collections::UnmodifiableSortedSet::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return UnmodifiableCollection::AddAll(collection, modified);
}

ECode Collections::UnmodifiableSortedSet::Clear()
{
    return UnmodifiableCollection::Clear();
}

ECode Collections::UnmodifiableSortedSet::Contains(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return UnmodifiableCollection::Contains(object, result);
}

ECode Collections::UnmodifiableSortedSet::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return UnmodifiableCollection::ContainsAll(collection, result);
}

ECode Collections::UnmodifiableSortedSet::IsEmpty(
    /* [out] */ Boolean* result)
{
    return UnmodifiableCollection::IsEmpty(result);
}

ECode Collections::UnmodifiableSortedSet::GetIterator(
    /* [out] */ IIterator** it)
{
    return UnmodifiableCollection::GetIterator(it);
}

ECode Collections::UnmodifiableSortedSet::Remove(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return UnmodifiableCollection::Remove(object, modified);
}

ECode Collections::UnmodifiableSortedSet::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return UnmodifiableCollection::RemoveAll(collection, modified);
}

ECode Collections::UnmodifiableSortedSet::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return UnmodifiableCollection::RetainAll(collection, modified);
}

ECode Collections::UnmodifiableSortedSet::GetSize(
    /* [out] */ Int32* size)
{
    return UnmodifiableCollection::GetSize(size);
}

ECode Collections::UnmodifiableSortedSet::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    return UnmodifiableCollection::ToArray(array);
}

ECode Collections::UnmodifiableSortedSet::ToArray(
    /* [in] */ ArrayOf<IInterface*>* inArray,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    return UnmodifiableCollection::ToArray(inArray, outArray);
}

ECode Collections::UnmodifiableSortedSet::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return UnmodifiableSet::Equals(object, result);
}

ECode Collections::UnmodifiableSortedSet::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    return UnmodifiableSet::GetHashCode(hashCode);
}

//====================================================================
// Collections::
//====================================================================

ECode Collections::BinarySearch(
    /* [in] */ IList* list,
    /* [in] */ IInterface* object,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);
    if (list == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }
    Boolean b;
    if (((ICollection::Probe(list))->IsEmpty(&b), b)) {
        *index = -1;
        return NOERROR;
    }

    if (IRandomAccess::Probe(list) == NULL) {
        AutoPtr<IListIterator> it;
        list->GetListIterator((IListIterator**)&it);
        Boolean b;
        while (((IIterator::Probe(it))->HasNext(&b), b)) {
            Int32 result;
            AutoPtr<IInterface> o;
            (IIterator::Probe(it))->GetNext((IInterface**)&o);
            AutoPtr<IComparable> com = IComparable::Probe(o);
            com->CompareTo(object, &result);
            result = -result;
            if (result <= 0) {
                if (result == 0) {
                    return it->GetPreviousIndex(index);
                }
                Int32 preIndex;
                it->GetPreviousIndex(&preIndex);
                *index = -preIndex - 1;
                return NOERROR;
            }
        }
        Int32 num;
        (ICollection::Probe(list))->GetSize(&num);
        *index = -num - 1;
        return NOERROR;
    }

    Int32 low = 0, mid, high, result = -1;
    (ICollection::Probe(list))->GetSize(&mid);
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

ECode Collections::BinarySearch(
    /* [in] */ IList* list,
    /* [in] */ IInterface* object,
    /* [in] */ IComparator* comparator,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);
    if (comparator == NULL) {
        return BinarySearch(list, object, index);
    }
    if (IRandomAccess::Probe(list) == NULL) {
        AutoPtr<IListIterator> it;
        list->GetListIterator((IListIterator**)&it);
        Boolean b;
        while (((IIterator::Probe(it))->HasNext(&b), b)) {
            Int32 result;
            AutoPtr<IInterface> o;
            (IIterator::Probe(it))->GetNext((IInterface**)&o);
            comparator->Compare(o, object, &result);
            result = -result;
            if (result <= 0) {
                if (result == 0) {
                    return it->GetPreviousIndex(index);
                }
                Int32 preIndex;
                it->GetPreviousIndex(&preIndex);
                *index = -preIndex - 1;
                return NOERROR;
            }
        }
        Int32 size;
        (ICollection::Probe(list))->GetSize(&size);
        *index = -size - 1;
        return NOERROR;
    }

    Int32 low = 0, mid, high, result = -1;
    (ICollection::Probe(list))->GetSize(&mid);
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

ECode Collections::Copy(
    /* [in] */ IList* destination,
    /* [in] */ IList* source)
{
    ECode ec = 0;
    Int32 sizeDes, sizeSrc;
    (ICollection::Probe(destination))->GetSize(&sizeDes);
    (ICollection::Probe(source))->GetSize(&sizeSrc);
    if (sizeDes < sizeSrc) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    AutoPtr<IIterator> srcIt;
    (IIterable::Probe(source))->GetIterator((IIterator**)&srcIt);
    AutoPtr<IListIterator> destIt;
    destination->GetListIterator((IListIterator**)&destIt);
    Boolean b;
    while ((srcIt->HasNext(&b), b)) {
        AutoPtr<IInterface> o1;
        ec = (IIterator::Probe(destIt))->GetNext((IInterface**)&o1);
        if (ec != NOERROR) {
            return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
        }
        AutoPtr<IInterface> o2;
        srcIt->GetNext((IInterface**)&o2);
        destIt->Set(o2);
    }
    return NOERROR;
}

ECode Collections::Enumeration(
    /* [in] */ ICollection* collection,
    /* [out] */ IEnumeration** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<ICollection> c = collection;
    AutoPtr<IEnumeration> res = new MyEnumeration2(c);
    *result = res;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode Collections::Fill(
    /* [in] */ IList* list,
    /* [in] */ IInterface* object)
{
    AutoPtr<IListIterator> it;
    list->GetListIterator((IListIterator**)&it);
    Boolean b;
    while (((IIterator::Probe(it))->HasNext(&b), b)) {
        AutoPtr<IInterface> o;
        (IIterator::Probe(it))->GetNext((IInterface**)&o);
        it->Set(object);
    }
    return NOERROR;
}

ECode Collections::Max(
    /* [in] */ ICollection* collection,
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<IIterator> it;
    (IIterable::Probe(collection))->GetIterator((IIterator**)&it);
    AutoPtr<IInterface> max;
    it->GetNext((IInterface**)&max);
    Boolean b;
    while ((it->HasNext(&b), b)) {
        AutoPtr<IInterface> next;
        it->GetNext((IInterface**)&next);
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

ECode Collections::Max(
    /* [in] */ ICollection* collection,
    /* [in] */ IComparator* comparator,
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);
    if (comparator == NULL) {
        return Max(collection, result);
    }

    AutoPtr<IIterator> it;
    (IIterable::Probe(collection))->GetIterator((IIterator**)&it);
    AutoPtr<IInterface> max;
    it->GetNext((IInterface**)&max);
    Boolean b;
    while ((it->HasNext(&b), b)) {
        AutoPtr<IInterface> next;
        it->GetNext((IInterface**)&next);
        Int32 res;
        if ((comparator->Compare(max, next, &res), res) < 0) {
            max = next;
        }
    }
    *result = max;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode Collections::Min(
    /* [in] */ ICollection* collection,
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<IIterator> it;
    (IIterable::Probe(collection))->GetIterator((IIterator**)&it);
    AutoPtr<IInterface> min;
    it->GetNext((IInterface**)&min);
    Boolean b;
    while ((it->HasNext(&b), b)) {
        AutoPtr<IInterface> next;
        it->GetNext((IInterface**)&next);
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

ECode Collections::Min(
    /* [in] */ ICollection* collection,
    /* [in] */ IComparator* comparator,
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);
    if (comparator == NULL) {
        return Min(collection, result);
    }

    AutoPtr<IIterator> it;
    (IIterable::Probe(collection))->GetIterator((IIterator**)&it);
    AutoPtr<IInterface> min;
    it->GetNext((IInterface**)&min);
    Boolean b;
    while ((it->HasNext(&b), b)) {
        AutoPtr<IInterface> next;
        it->GetNext((IInterface**)&next);
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

ECode Collections::NCopies(
    /* [in] */ Int32 length,
    /* [in] */ IInterface* object,
    /* [out] */ IList** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<IList> res = new CopiesList(length, object);
    *result = res;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode Collections::Reverse(
    /* [in] */ IList* list)
{
    Int32 size;
    (ICollection::Probe(list))->GetSize(&size);
    AutoPtr<IListIterator> front;
    list->GetListIterator((IListIterator**)&front);
    AutoPtr<IListIterator> back;
    list->GetListIterator(size, (IListIterator**)&back);
    for (Int32 i = 0; i < size / 2; i++) {
        AutoPtr<IInterface> frontNext;
        (IIterator::Probe(front))->GetNext((IInterface**)&frontNext);
        AutoPtr<IInterface> backPrev;
        back->GetPrevious((IInterface**)&backPrev);
        front->Set(backPrev);
        back->Set(frontNext);
    }
    return NOERROR;
}

ECode Collections::ReverseOrder(
    /* [out] */ IComparator** result)
{
    VALIDATE_NOT_NULL(result);
    *result = ReverseComparator::INSTANCE;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode Collections::ReverseOrder(
    /* [in] */ IComparator* c,
    /* [out] */ IComparator** result)
{
    VALIDATE_NOT_NULL(result);
    if (c == NULL) {
        return ReverseOrder(result);
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

ECode Collections::Shuffle(
    /* [in] */ IList* list)
{
    AutoPtr<IRandom> r;
    CRandom::New((IRandom**)&r);
    return Shuffle(list, r);
}

ECode Collections::Shuffle(
    /* [in] */ IList* list,
    /* [in] */ IRandom* random)
{
    AutoPtr<IList> objectList = list;

    if (IRandomAccess::Probe(list) != NULL) {
        Int32 i;
        (ICollection::Probe(objectList))->GetSize(&i);
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
        (ICollection::Probe(objectList))->ToArray((ArrayOf<IInterface*>**)&array);
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
        while (((IIterator::Probe(it))->HasNext(&b), b)) {
            AutoPtr<IInterface> o;
            (IIterator::Probe(it))->GetNext((IInterface**)&o);
            it->Set((*array)[i++]);
        }
    }
    return NOERROR;
}

ECode Collections::NewSingleton(
    /* [in] */ IInterface* object,
    /* [out] */ ISet** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<ISet> res = new SingletonSet(object);
    *result = res;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode Collections::NewSingletonList(
    /* [in] */ IInterface* object,
    /* [out] */ IList** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<IList> res = new SingletonList(object);
    *result = res;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode Collections::NewSingletonMap(
    /* [in] */ IInterface* key,
    /* [in] */ IInterface* value,
    /* [out] */ IMap** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<IMap> res = new SingletonMap(key, value);
    *result = res;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode Collections::Sort(
    /* [in] */ IList* list)
{
    AutoPtr<ArrayOf<IInterface*> > array;
    (ICollection::Probe(list))->ToArray((ArrayOf<IInterface*>**)&array);
    Arrays::Sort(array.Get());
    Int32 i = 0;
    AutoPtr<IListIterator> it;
    list->GetListIterator((IListIterator**)&it);
    Boolean b;
    while (((IIterator::Probe(it))->HasNext(&b), b)) {
        AutoPtr<IInterface> o;
        (IIterator::Probe(it))->GetNext((IInterface**)&o);
        it->Set((*array)[i++]);
    }
    return NOERROR;
}

ECode Collections::Sort(
    /* [in] */ IList* list,
    /* [in] */ IComparator* comparator)
{
    Int32 num;
    (ICollection::Probe(list))->GetSize(&num);
    AutoPtr<ArrayOf<IInterface*> > arr = ArrayOf<IInterface*>::Alloc(num);
    AutoPtr<ArrayOf<IInterface*> > array;
    (ICollection::Probe(list))->ToArray(arr, (ArrayOf<IInterface*>**)&array);
    Arrays::Sort(array, comparator);
    AutoPtr<IListIterator> it;
    list->GetListIterator((IListIterator**)&it);
    Boolean b;
    while (((IIterator::Probe(it))->HasNext(&b), b)) {
        Int32 i = 0;
        AutoPtr<IInterface> o;
        (IIterator::Probe(it))->GetNext((IInterface**)&o);
        it->Set((*array)[i++]);
    }
    return NOERROR;
}

ECode Collections::Swap(
    /* [in] */ IList* list,
    /* [in] */ Int32 index1,
    /* [in] */ Int32 index2)
{
    if (list == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }
    Int32 size;
    (ICollection::Probe(list))->GetSize(&size);
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

ECode Collections::ReplaceAll(
    /* [in] */ IList* list,
    /* [in] */ IInterface* obj,
    /* [in] */ IInterface* obj2,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
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

ECode Collections::Rotate(
    /* [in] */ IList* lst,
    /* [in] */ Int32 dist)
{
    AutoPtr<IList> list = lst;
    Int32 size;
    (ICollection::Probe(list))->GetSize(&size);

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
        list->GetSubList(0, divideIndex, (IList**)&sublist1);
        AutoPtr<IList> sublist2;
        list->GetSubList(divideIndex, size, (IList**)&sublist2);
        Reverse(sublist1);
        Reverse(sublist2);
        Reverse(list);
    }
    return NOERROR;
}

ECode Collections::IndexOfSubList(
    /* [in] */ IList* list,
    /* [in] */ IList* sublist,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    Int32 size, sublistSize;
    (ICollection::Probe(list))->GetSize(&size);
    (ICollection::Probe(list))->GetSize(&sublistSize);

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
        (IIterator::Probe(listIt))->GetNext((IInterface**)&o);
        if ((firstObj == NULL) ? o == NULL : Object::Equals(firstObj, o)) {

            // iterate through the elements in sublist to see
            // if they are included in the same order in the list
            AutoPtr<IListIterator> sublistIt;
            sublist->GetListIterator(1, (IListIterator**)&sublistIt);
            Boolean difFound = FALSE, subHasNext, listHasNext;
            while (((IIterator::Probe(sublistIt))->HasNext(&subHasNext), subHasNext)) {
                AutoPtr<IInterface> element;
                (IIterator::Probe(sublistIt))->GetNext((IInterface**)&element);
                if (!((IIterator::Probe(listIt))->HasNext(&listHasNext), listHasNext)) {
                    *result = -1;
                    return NOERROR;
                }
                AutoPtr<IInterface> o;
                (IIterator::Probe(listIt))->GetNext((IInterface**)&o);
                if ((element == NULL) ? o != NULL : !Object::Equals(element, o)) {
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

ECode Collections::LastIndexOfSubList(
    /* [in] */ IList* list,
    /* [in] */ IList* sublist,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    Int32 sublistSize, size;
    (ICollection::Probe(sublist))->GetSize(&sublistSize);
    (ICollection::Probe(list))->GetSize(&size);

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
        listIt->GetPrevious((IInterface**)&o);
        if ((lastObj == NULL) ? o == NULL : Object::Equals(lastObj, o)) {
            // iterate through the elements in sublist to see
            // if they are included in the same order in the list
            AutoPtr<IListIterator> sublistIt;
            sublist->GetListIterator(sublistSize - 1, (IListIterator**)&sublist);
            Boolean difFound = FALSE;
            Boolean subPrevious, listPrevious;
            while ((sublistIt->HasPrevious(&subPrevious), subPrevious)) {
                AutoPtr<IInterface> element;
                sublistIt->GetPrevious((IInterface**)&element);
                if (!(listIt->HasPrevious(&listPrevious), listPrevious)) {
                    *result = -1;
                    return NOERROR;
                }

                AutoPtr<IInterface> o;
                listIt->GetPrevious((IInterface**)&o);
                if ((element == NULL) ? o != NULL : !Object::Equals(element, o)) {
                    difFound = TRUE;
                    break;
                }
            }
            // All elements of sublist are found in main list
            // starting from listIt.nextIndex().
            if (!difFound) {
                return listIt->GetNextIndex(result);
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

ECode Collections::NewList(
    /* [in] */ IEnumeration* enumeration,
    /* [out] */ IArrayList** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<IArrayList> list;
    CArrayList::New((IArrayList**)&list);
    Boolean b;
    while ((enumeration->HasMoreElements(&b), b)) {
        AutoPtr<IInterface> o;
        enumeration->GetNextElement((IInterface**)&o);
        Boolean modified;
        (IList::Probe(list))->Add(o, &modified);
    }
    *result = list;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode Collections::NewSynchronizedCollection(
    /* [in] */ ICollection* collection,
    /* [out] */ ICollection** result)
{
    VALIDATE_NOT_NULL(result);
    if (collection == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }
    AutoPtr<ICollection> res = new SynchronizedCollection(collection);
    *result = res;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode Collections::NewSynchronizedList(
    /* [in] */ IList* list,
    /* [out] */ IList** result)
{
    VALIDATE_NOT_NULL(result);
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

ECode Collections::NewSynchronizedMap(
    /* [in] */ IMap* map,
    /* [out] */ IMap** result)
{
    VALIDATE_NOT_NULL(result);
    if (map == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }
    AutoPtr<IMap> res = new SynchronizedMap(map);
    *result = res;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode Collections::NewSynchronizedSet(
    /* [in] */ ISet* set,
    /* [out] */ ISet** result)
{
    VALIDATE_NOT_NULL(result);
    if (set == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }
    AutoPtr<ISet> res = new SynchronizedSet(set);
    *result = res;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode Collections::NewSynchronizedSortedMap(
    /* [in] */ ISortedMap* map,
    /* [out] */ ISortedMap** result)
{
    VALIDATE_NOT_NULL(result);
    if (map == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }
    AutoPtr<ISortedMap> res = new SynchronizedSortedMap(map);
    *result = res;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode Collections::NewSynchronizedSortedSet(
    /* [in] */ ISortedSet* set,
    /* [out] */ ISortedSet** result)
{
    VALIDATE_NOT_NULL(result);
    if (set == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }
    AutoPtr<ISortedSet> res = new SynchronizedSortedSet(set);
    *result = res;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode Collections::NewUnmodifiableCollection(
    /* [in] */ ICollection* collection,
    /* [out] */ ICollection** result)
{
    VALIDATE_NOT_NULL(result);
    if (collection == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }
    AutoPtr<ICollection> res = new UnmodifiableCollection(collection);
    *result = res;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode Collections::NewUnmodifiableList(
    /* [in] */ IList* list,
    /* [out] */ IList** result)
{
    VALIDATE_NOT_NULL(result);
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

ECode Collections::NewUnmodifiableMap(
    /* [in] */ IMap* map,
    /* [out] */ IMap** result)
{
    VALIDATE_NOT_NULL(result);
    if (map == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }
    AutoPtr<IMap> res = new UnmodifiableMap(map);
    *result = res;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode Collections::NewUnmodifiableSet(
    /* [in] */ ISet* set,
    /* [out] */ ISet** result)
{
    VALIDATE_NOT_NULL(result);
    if (set == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }
    AutoPtr<ISet> res = new UnmodifiableSet(set);
    *result = res;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode Collections::NewUnmodifiableSortedMap(
    /* [in] */ ISortedMap* map,
    /* [out] */ ISortedMap** result)
{
    VALIDATE_NOT_NULL(result);
    if (map == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }
    AutoPtr<ISortedMap> res = new UnmodifiableSortedMap(map);
    *result = res;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode Collections::NewUnmodifiableSortedSet(
    /* [in] */ ISortedSet* set,
    /* [out] */ ISortedSet** result)
{
    VALIDATE_NOT_NULL(result);
    if (set == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }
    AutoPtr<ISortedSet> res = new UnmodifiableSortedSet(set);
    *result = res;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode Collections::Frequency(
    /* [in] */ ICollection* c,
    /* [in] */ IInterface* o,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
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
    (IIterable::Probe(c))->GetIterator((IIterator**)&itr);
    while ((itr->HasNext(&b), b)) {
        AutoPtr<IInterface> e;
        itr->GetNext((IInterface**)&e);
        if ((o == NULL) ? e == NULL : Object::Equals(o, e)) {
            num++;
        }
    }
    *result = num;
    return NOERROR;
}

ECode Collections::GetEmptyList(
    /* [out] */ IList** result)
{
    VALIDATE_NOT_NULL(result);
    *result = EMPTY_LIST;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode Collections::GetEmptySet(
    /* [out] */ ISet** result)
{
    VALIDATE_NOT_NULL(result);
    *result = EMPTY_SET;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode Collections::GetEmptyMap(
    /* [out] */ IMap** result)
{
    VALIDATE_NOT_NULL(result);
    *result = EMPTY_MAP;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode Collections::GetEmptyEnumeration(
    /* [out] */ IEnumeration** result)
{
    VALIDATE_NOT_NULL(result);
    *result = EMPTY_ENUMERATION;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode Collections::GetEmptyIterator(
    /* [out] */ IIterator** result)
{
    VALIDATE_NOT_NULL(result);
    *result = EMPTY_ITERATOR;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode Collections::GetEmptyListIterator(
    /* [out] */ IListIterator** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<IList> l;
    GetEmptyList((IList**)&l);
    return l->GetListIterator(result);
}

ECode Collections::NewCheckedCollection(
    /* [in] */ ICollection* c,
    /* [in] */ const InterfaceID& type,
    /* [out] */ ICollection** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<ICollection> res = new CheckedCollection(c, type);
    *result = res;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode Collections::NewCheckedMap(
    /* [in] */ IMap* m,
    /* [in] */ const InterfaceID& keyType,
    /* [in] */ const InterfaceID& valueType,
    /* [out] */ IMap** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<IMap> res = new CheckedMap(m, keyType, valueType);
    *result = res;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode Collections::NewCheckedList(
    /* [in] */ IList* list,
    /* [in] */ const InterfaceID& type,
    /* [out] */ IList** result)
{
    VALIDATE_NOT_NULL(result);
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

ECode Collections::NewCheckedSet(
    /* [in] */ ISet* s,
    /* [in] */ const InterfaceID& type,
    /* [out] */ ISet** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<ISet> res = new CheckedSet(s, type);
    *result = res;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode Collections::NewCheckedSortedMap(
    /* [in] */ ISortedMap* m,
    /* [in] */ const InterfaceID& keyType,
    /* [in] */ const InterfaceID& valueType,
    /* [out] */ ISortedMap** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<ISortedMap> res = new CheckedSortedMap(m, keyType, valueType);
    *result = res;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode Collections::NewCheckedSortedSet(
    /* [in] */ ISortedSet* s,
    /* [in] */ const InterfaceID& type,
    /* [out] */ ISortedSet** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<ISortedSet> res = new CheckedSortedSet(s, type);
    *result = res;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode Collections::AddAll(
    /* [in] */ ICollection* c,
    /* [in] */ ArrayOf<IInterface*>* a,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    Boolean modified = FALSE;
    for (Int32 i = 0; i < a->GetLength(); i++) {
        Boolean b;
        c->Add((*a)[i], &b);
        modified |= b;
    }
    *result = modified;
    return NOERROR;
}

ECode Collections::Disjoint(
    /* [in] */ ICollection* c1,
    /* [in] */ ICollection* c2,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
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
    (IIterable::Probe(c1))->GetIterator((IIterator**)&it);
    Boolean b;
    while ((it->HasNext(&b), b)) {
        AutoPtr<IInterface> o;
        it->GetNext((IInterface**)&o);
        Boolean a;
        if ((c2->Contains(o, &a), a)) {
            *result = FALSE;
            return NOERROR;
        }
    }
    *result = TRUE;
    return NOERROR;
}

ECode Collections::CheckType(
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

ECode Collections::NewSetFromMap(
    /* [in] */ IMap* map,
    /* [out] */ ISet** result)
{
    VALIDATE_NOT_NULL(result);
    Boolean isEmpty = FALSE;
    if ((map->IsEmpty(&isEmpty), isEmpty)) {
        AutoPtr<ISet> res = new SetFromMap(map);
        *result = res;
        REFCOUNT_ADD(*result)
        return NOERROR;
    }
    return E_ILLEGAL_ARGUMENT_EXCEPTION;
}

ECode Collections::NewAsLifoQueue(
    /* [in] */ IDeque* deque,
    /* [out] */ IQueue** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<IQueue> res = new AsLIFOQueue(deque);
    *result = res;
    REFCOUNT_ADD(*result)
    return NOERROR;
}


//====================================================================
// Collections::SetFromMap::
//====================================================================
CAR_INTERFACE_IMPL(Collections::SetFromMap, AbstractSet, ISerializable)

Collections::SetFromMap::SetFromMap(
    /* [in] */ IMap* map)
{
    mM = map;
    map->GetKeySet((ISet**)&mBackingSet);
}

ECode Collections::SetFromMap::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value);
    return (ICollection::Probe(mBackingSet))->Equals(object, value);
}

ECode Collections::SetFromMap::GetHashCode(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    return (ICollection::Probe(mBackingSet))->GetHashCode(value);
}

ECode Collections::SetFromMap::Add(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    VALIDATE_NOT_NULL(modified);
    AutoPtr<IBoolean> value;
    CBoolean::New(TRUE, (IBoolean**)&value);
    AutoPtr<IInterface> oldValue;
    mM->Put(object, value, (IInterface**)&oldValue);
    *modified = oldValue == NULL;
    return NOERROR;
}

ECode Collections::SetFromMap::Clear()
{
    return mM->Clear();
}

ECode Collections::SetFromMap::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    AutoPtr<IObject> iObj;
    iObj = IObject::Probe(mBackingSet);
    if (iObj != NULL) {
        return iObj->ToString(str);
    }
    *str = NULL;
    return NOERROR;
}

ECode Collections::SetFromMap::Contains(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return (ICollection::Probe(mBackingSet))->Contains(object, result);
}

ECode Collections::SetFromMap::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return (ICollection::Probe(mBackingSet))->ContainsAll(collection, result);
}

ECode Collections::SetFromMap::IsEmpty(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return mM->IsEmpty(result);
}

ECode Collections::SetFromMap::Remove(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<IInterface> value;
    FAIL_RETURN(mM->Remove(object, (IInterface**)&value));
    *result = value != NULL;
    return  NOERROR;
}

ECode Collections::SetFromMap::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return (ICollection::Probe(mBackingSet))->RetainAll(collection, result);
}

ECode Collections::SetFromMap::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    VALIDATE_NOT_NULL(array);
    return (ICollection::Probe(mBackingSet))->ToArray(array);
}

ECode Collections::SetFromMap::ToArray(
    /* [in] */ ArrayOf<IInterface*>* contents,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    VALIDATE_NOT_NULL(outArray);
    return (ICollection::Probe(mBackingSet))->ToArray(contents, outArray);
}

ECode Collections::SetFromMap::GetIterator(
    /* [out] */ IIterator** result)
{
    VALIDATE_NOT_NULL(result);
    return (IIterable::Probe(mBackingSet))->GetIterator(result);
}

ECode Collections::SetFromMap::GetSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);
    return mM->GetSize(size);
}

ECode Collections::SetFromMap::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::AddAll(collection, modified);
}

ECode Collections::SetFromMap::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractSet::RemoveAll(collection, modified);
}

ECode Collections::SetFromMap::ReadObject(
    /* [in] */ IObjectInputStream* stream)
{
    stream->DefaultReadObject();
    return mM->GetKeySet((ISet**)&mBackingSet);
}

//====================================================================
// Collections::AsLIFOQueue::
//====================================================================
CAR_INTERFACE_IMPL(Collections::AsLIFOQueue, AbstractQueue, ISerializable)

Collections::AsLIFOQueue::AsLIFOQueue(
    /* [in] */ IDeque* deque)
{
    mQ = deque;
}

ECode Collections::AsLIFOQueue::GetIterator(
    /* [out] */ IIterator** result)
{
    VALIDATE_NOT_NULL(result);
    return (IIterable::Probe(mQ))->GetIterator(result);
}

ECode Collections::AsLIFOQueue::GetSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);
    return (ICollection::Probe(mQ))->GetSize(size);
}

ECode Collections::AsLIFOQueue::Offer(
    /* [in] */ IInterface* obj,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return mQ->OfferFirst(obj, result);
}

ECode Collections::AsLIFOQueue::Peek(
    /* [out] */ IInterface** obj)
{
    VALIDATE_NOT_NULL(obj);
    return mQ->PeekFirst(obj);
}

ECode Collections::AsLIFOQueue::Poll(
    /* [out] */ IInterface** obj)
{
    VALIDATE_NOT_NULL(obj);
    return mQ->PollFirst(obj);
}

ECode Collections::AsLIFOQueue::Add(
    /* [in] */ IInterface* obj,
    /* [out] */ Boolean* result)
{
    return (ICollection::Probe(mQ))->Add(obj, result);
}

ECode Collections::AsLIFOQueue::Clear()
{
    return (ICollection::Probe(mQ))->Clear();
}

ECode Collections::AsLIFOQueue::GetElement(
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object);
    return mQ->GetFirst(object);
}

ECode Collections::AsLIFOQueue::Remove(
    /* [out] */ IInterface** obj)
{
    VALIDATE_NOT_NULL(obj);
    return mQ->Pop(obj);
}

ECode Collections::AsLIFOQueue::Contains(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return (ICollection::Probe(mQ))->Contains(object, result);
}

ECode Collections::AsLIFOQueue::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return (ICollection::Probe(mQ))->ContainsAll(collection, result);
}

ECode Collections::AsLIFOQueue::IsEmpty(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return (ICollection::Probe(mQ))->IsEmpty(result);
}

ECode Collections::AsLIFOQueue::Remove(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return (ICollection::Probe(mQ))->Remove(object, result);
}

ECode Collections::AsLIFOQueue::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return (ICollection::Probe(mQ))->RemoveAll(collection, result);
}

ECode Collections::AsLIFOQueue::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return (ICollection::Probe(mQ))->RetainAll(collection, result);
}

ECode Collections::AsLIFOQueue::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    VALIDATE_NOT_NULL(array);
    return (ICollection::Probe(mQ))->ToArray(array);
}

ECode Collections::AsLIFOQueue::ToArray(
    /* [in] */ ArrayOf<IInterface*>* contents,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    VALIDATE_NOT_NULL(outArray);
    return (ICollection::Probe(mQ))->ToArray(contents, outArray);
}

ECode Collections::AsLIFOQueue::ToString(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<IObject> iObj;
    iObj = IObject::Probe(mQ);
    if (iObj != NULL) {
        return iObj->ToString(result);
    }
    *result = NULL;
    return NOERROR;
}

ECode Collections::AsLIFOQueue::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return AbstractQueue::AddAll(collection, modified);
}

ECode Collections::AsLIFOQueue::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = this->Probe(EIID_IInterface) == object;
    return NOERROR;
}

ECode Collections::AsLIFOQueue::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode);
    *hashCode = (Int32)this;
    return NOERROR;
}

//====================================================================
// Collections::CheckedCollection::
//====================================================================
CAR_INTERFACE_IMPL_2(Collections::CheckedCollection, Object, ICollection, ISerializable)

Collections::CheckedCollection::CheckedCollection(
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

ECode Collections::CheckedCollection::GetSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);
    return mC->GetSize(size);
}

ECode Collections::CheckedCollection::IsEmpty(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return mC->IsEmpty(result);
}

ECode Collections::CheckedCollection::Contains(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return mC->Contains(object, result);
}

ECode Collections::CheckedCollection::GetIterator(
    /* [out] */ IIterator** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<IIterator> i;
    (IIterable::Probe(mC))->GetIterator((IIterator**)&i);
    AutoPtr<IListIterator> res = IListIterator::Probe(i);
    if (res != NULL) {
        AutoPtr<IListIterator> result = new CheckedListIterator(res, mType);
        i = IIterator::Probe(result);
    }
    *result = i;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode Collections::CheckedCollection::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    VALIDATE_NOT_NULL(array);
    return mC->ToArray(array);
}

ECode Collections::CheckedCollection::ToArray(
    /* [in] */ ArrayOf<IInterface*>* inArray,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    VALIDATE_NOT_NULL(outArray);
    return mC->ToArray(inArray, outArray);
}

ECode Collections::CheckedCollection::Add(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    VALIDATE_NOT_NULL(modified);
    return mC->Add(object, modified);
}

ECode Collections::CheckedCollection::Add(
    /* [in] */ IInterface* object)
{
    assert(0 && "TODO");
    return NOERROR;
}

ECode Collections::CheckedCollection::Remove(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    VALIDATE_NOT_NULL(modified);
    return mC->Remove(object, modified);
}

ECode Collections::CheckedCollection::Remove(
    /* [in] */ IInterface* object)
{
    assert(0 && "TODO");
    return NOERROR;
}

ECode Collections::CheckedCollection::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return mC->ContainsAll(collection, result);
}

ECode Collections::CheckedCollection::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    VALIDATE_NOT_NULL(modified);
    AutoPtr<ArrayOf<IInterface*> > array;
    collection->ToArray((ArrayOf<IInterface*>**)&array);
    for (Int32 i = 0; i < array->GetLength(); ++i) {
        AutoPtr<IInterface> o = (*array)[i];
        FAIL_RETURN(Collections::CheckType(o, mType));
    }
    return mC->AddAll(collection, modified);
}

ECode Collections::CheckedCollection::AddAll(
    /* [in] */ ICollection* collection)
{
    assert(0 && "TODO");
    return NOERROR;
}

ECode Collections::CheckedCollection::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    VALIDATE_NOT_NULL(modified);
    return mC->RemoveAll(collection, modified);
}

ECode Collections::CheckedCollection::RemoveAll(
    /* [in] */ ICollection* collection)
{
    assert(0 && "TODO");
    return NOERROR;
}

ECode Collections::CheckedCollection::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    VALIDATE_NOT_NULL(modified);
    return mC->RetainAll(collection, modified);
}

ECode Collections::CheckedCollection::RetainAll(
    /* [in] */ ICollection* collection)
{
    assert(0 && "TODO");
    return NOERROR;
}

ECode Collections::CheckedCollection::Clear()
{
    return mC->Clear();
}

ECode Collections::CheckedCollection::ToString(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<IObject> iObj = IObject::Probe(mC);
    if (iObj != NULL) {
        return iObj->ToString(result);
    }
    *result = NULL;
    return NOERROR;
}

ECode Collections::CheckedCollection::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = this->Probe(EIID_IInterface) == object;
    return NOERROR;
}

ECode Collections::CheckedCollection::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode);
    *hashCode = (Int32)this;
    return NOERROR;
}

//====================================================================
// Collections::CheckedListIterator::
//====================================================================
CAR_INTERFACE_IMPL(Collections::CheckedListIterator, Object, IListIterator)

Collections::CheckedListIterator::CheckedListIterator(
    /* [in] */ IListIterator* i,
    /* [in] */ InterfaceID type)
{
   mI = i;
   mType = type;
}

ECode Collections::CheckedListIterator::HasNext(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return (IIterator::Probe(mI))->HasNext(result);
}

ECode Collections::CheckedListIterator::GetNext(
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object);
    return (IIterator::Probe(mI))->GetNext(object);
}

ECode Collections::CheckedListIterator::Remove()
{
    return (IIterator::Probe(mI))->Remove();
}

ECode Collections::CheckedListIterator::HasPrevious(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return mI->HasPrevious(result);
}

ECode Collections::CheckedListIterator::GetPrevious(
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(*object);
    return mI->GetPrevious(object);
}

ECode Collections::CheckedListIterator::GetNextIndex(
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);
    return mI->GetNextIndex(index);
}

ECode Collections::CheckedListIterator::GetPreviousIndex(
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);
    return mI->GetPreviousIndex(index);
}

ECode Collections::CheckedListIterator::Set(
    /* [in] */ IInterface* object)
{
    FAIL_RETURN(Collections::CheckType(object, mType));
    return mI->Set(object);
}

ECode Collections::CheckedListIterator::Add(
    /* [in] */ IInterface* object)
{
    FAIL_RETURN(Collections::CheckType(object, mType));
    return mI->Add(object);
}

//====================================================================
// Collections::CheckedList::
//====================================================================
CAR_INTERFACE_IMPL(Collections::CheckedList, CheckedCollection, IList)

Collections::CheckedList::CheckedList(
    /* [in] */ IList* l,
    /* [in] */ InterfaceID type)
    : CheckedCollection(ICollection::Probe(l), type)
{
}

ECode Collections::CheckedList::AddAll(
    /* [in] */ Int32 location,
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    AutoPtr<ArrayOf<IInterface*> > array;
    collection->ToArray((ArrayOf<IInterface*>**)&array);
    for (Int32 i = 0; i < array->GetLength();i++) {
        AutoPtr<IInterface> o = (*array)[i];
        FAIL_RETURN(Collections::CheckType(o, mType));
    }
    return mL->AddAll(location, collection, modified);
}

ECode Collections::CheckedList::AddAll(
    /* [in] */ Int32 location,
    /* [in] */ ICollection* collection)
{
    assert(0 && "TODO");
    return NOERROR;
}

ECode Collections::CheckedList::Get(
    /* [in] */ Int32 location,
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object);
    return mL->Get(location, object);
}

ECode Collections::CheckedList::Set(
    /* [in] */ Int32 location,
    /* [in] */ IInterface* object,
    /* [out] */ IInterface** prevObject)
{
    VALIDATE_NOT_NULL(prevObject);
    FAIL_RETURN(Collections::CheckType(object, mType));
    return mL->Set(location, object, prevObject);
}

ECode Collections::CheckedList::Set(
    /* [in] */ Int32 location,
    /* [in] */ IInterface* object)
{
    assert(0 && "TODO");
    return NOERROR;
}

ECode Collections::CheckedList::Add(
    /* [in] */ Int32 location,
    /* [in] */ IInterface* object)
{
    FAIL_RETURN(Collections::CheckType(object, mType));
    return mL->Add(location, object);
}

ECode Collections::CheckedList::Add(
    /* [in] */ IInterface* object)
{
    assert(0 && "TODO");
    return NOERROR;
}

ECode Collections::CheckedList::Remove(
    /* [in] */ Int32 location,
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object);
    return mL->Remove(location, object);
}

ECode Collections::CheckedList::Remove(
    /* [in] */ Int32 location)
{
    assert(0 && "TODO");
    return NOERROR;
}

ECode Collections::CheckedList::IndexOf(
    /* [in] */ IInterface* object,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);
    return mL->IndexOf(object, index);
}

ECode Collections::CheckedList::LastIndexOf(
    /* [in] */ IInterface* object,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);
    return mL->LastIndexOf(object, index);
}

ECode Collections::CheckedList::GetListIterator(
    /* [out] */ IListIterator** it)
{
    VALIDATE_NOT_NULL(it);
    AutoPtr<IListIterator> iterator;
    mL->GetListIterator((IListIterator**)&iterator);
    AutoPtr<IListIterator> res = new CheckedListIterator(iterator, mType);
    *it = res;
    REFCOUNT_ADD(*it)
    return NOERROR;
}

ECode Collections::CheckedList::GetListIterator(
    /* [in] */ Int32 location,
    /* [out] */ IListIterator** it)
{
    VALIDATE_NOT_NULL(it);
    AutoPtr<IListIterator> iterator;
    mL->GetListIterator(location, (IListIterator**)&iterator);
    AutoPtr<IListIterator> res = new CheckedListIterator(iterator, mType);
    *it = res;
    REFCOUNT_ADD(*it)
    return NOERROR;
}

ECode Collections::CheckedList::GetSubList(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ IList** subList)
{
    VALIDATE_NOT_NULL(subList);
    AutoPtr<IList> l;
    mL->GetSubList(start, end, (IList**)&l);
    CheckedList(l, mType);
    *subList = l;
    REFCOUNT_ADD(*subList)
    return NOERROR;
}

ECode Collections::CheckedList::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return (ICollection::Probe(mL))->Equals(object, result);
}

ECode Collections::CheckedList::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode);
    return (ICollection::Probe(mL))->GetHashCode(hashCode);
}

ECode Collections::CheckedList::Contains(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return CheckedCollection::Contains(object, result);
}

ECode Collections::CheckedList::GetSize(
    /* [out] */ Int32* size)
{
    return CheckedCollection::GetSize(size);
}

ECode Collections::CheckedList::GetIterator(
    /* [out] */ IIterator** it)
{
    return CheckedCollection::GetIterator(it);
}

ECode Collections::CheckedList::Add(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return CheckedCollection::Add(object, modified);
}

ECode Collections::CheckedList::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return CheckedCollection::AddAll(collection, modified);
}

ECode Collections::CheckedList::Clear()
{
    return CheckedCollection::Clear();
}

ECode Collections::CheckedList::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return CheckedCollection::ContainsAll(collection, result);
}

ECode Collections::CheckedList::IsEmpty(
    /* [out] */ Boolean* result)
{
    return CheckedCollection::IsEmpty(result);
}

ECode Collections::CheckedList::Remove(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return CheckedCollection::Remove(object, modified);
}

ECode Collections::CheckedList::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return CheckedCollection::RemoveAll(collection, modified);
}

ECode Collections::CheckedList::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return CheckedCollection::RetainAll(collection, modified);
}

ECode Collections::CheckedList::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    return CheckedCollection::ToArray(array);
}

ECode Collections::CheckedList::ToArray(
    /* [in] */ ArrayOf<IInterface*>* inArray,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    return CheckedCollection::ToArray(inArray, outArray);
}


//====================================================================
// Collections::CheckedRandomAccessList::
//====================================================================
CAR_INTERFACE_IMPL(Collections::CheckedRandomAccessList, CheckedList, IRandomAccess)

Collections::CheckedRandomAccessList::CheckedRandomAccessList(
    /* [in] */ IList* l,
    /* [in] */ InterfaceID type)
    : CheckedList(l, type)
{
}

//====================================================================
// Collections::CheckedSet::
//====================================================================
CAR_INTERFACE_IMPL(Collections::CheckedSet, CheckedCollection, ISet)

Collections::CheckedSet::CheckedSet(
    /* [in] */ ISet* s,
    /* [in] */ InterfaceID type)
    : CheckedCollection(ICollection::Probe(s), type)
{
}

ECode Collections::CheckedSet::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return mC->Equals(object, result);
}

ECode Collections::CheckedSet::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode);
    return mC->GetHashCode(hashCode);
}

ECode Collections::CheckedSet::Contains(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return CheckedCollection::Contains(object, result);
}

ECode Collections::CheckedSet::GetSize(
    /* [out] */ Int32* size)
{
    return CheckedCollection::GetSize(size);
}

ECode Collections::CheckedSet::GetIterator(
    /* [out] */ IIterator** it)
{
    return CheckedCollection::GetIterator(it);
}

ECode Collections::CheckedSet::Add(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return CheckedCollection::Add(object, modified);
}

ECode Collections::CheckedSet::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return CheckedCollection::AddAll(collection, modified);
}

ECode Collections::CheckedSet::Clear()
{
    return CheckedCollection::Clear();
}

ECode Collections::CheckedSet::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return CheckedCollection::ContainsAll(collection, result);
}

ECode Collections::CheckedSet::IsEmpty(
    /* [out] */ Boolean* result)
{
    return CheckedCollection::IsEmpty(result);
}

ECode Collections::CheckedSet::Remove(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return CheckedCollection::Remove(object, modified);
}

ECode Collections::CheckedSet::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return CheckedCollection::RemoveAll(collection, modified);
}

ECode Collections::CheckedSet::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return CheckedCollection::RetainAll(collection, modified);
}

ECode Collections::CheckedSet::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    return CheckedCollection::ToArray(array);
}

ECode Collections::CheckedSet::ToArray(
    /* [in] */ ArrayOf<IInterface*>* inArray,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    return CheckedCollection::ToArray(inArray, outArray);
}

//====================================================================
// Collections::CheckedMap::CheckedEntry::
//====================================================================
CAR_INTERFACE_IMPL(Collections::CheckedMap::CheckedEntry, Object, IMapEntry)

Collections::CheckedMap::CheckedEntry::CheckedEntry(
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

ECode Collections::CheckedMap::CheckedEntry::GetKey(
    /* [out] */ PInterface* key)
{
    VALIDATE_NOT_NULL(key);
    return mE->GetKey(key);
}

ECode Collections::CheckedMap::CheckedEntry::GetValue(
    /* [out] */ PInterface* value)
{
    VALIDATE_NOT_NULL(value);
    return mE->GetValue(value);
}

ECode Collections::CheckedMap::CheckedEntry::SetValue(
    /* [in] */ IInterface* valueReplacer,
    /* [out] */ IInterface** valueReplacee)
{
    VALIDATE_NOT_NULL(valueReplacee);
    FAIL_RETURN(Collections::CheckType(valueReplacer, mValueType));
    return mE->SetValue(valueReplacer, valueReplacee);
}

ECode Collections::CheckedMap::CheckedEntry::Equals(
    /* [in] */ IInterface* entry,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return mE->Equals(entry, result);
}

ECode Collections::CheckedMap::CheckedEntry::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode);
    return mE->GetHashCode(hashCode);
}

//====================================================================
// Collections::CheckedMap::CheckedEntrySet::CheckedEntryIterator::
//====================================================================
CAR_INTERFACE_IMPL(Collections::CheckedMap::CheckedEntrySet::CheckedEntryIterator, Object, IIterator)

Collections::CheckedMap::CheckedEntrySet::CheckedEntryIterator::CheckedEntryIterator(
    /* [in] */ IIterator* i,
    /* [in] */ InterfaceID valueType)
{
    mI = i;
    mValueType = valueType;
}

ECode Collections::CheckedMap::CheckedEntrySet::CheckedEntryIterator::HasNext(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return mI->HasNext(result);
}

ECode Collections::CheckedMap::CheckedEntrySet::CheckedEntryIterator::Remove()
{
    return mI->Remove();
}

ECode Collections::CheckedMap::CheckedEntrySet::CheckedEntryIterator::GetNext(
    /* [out] */ IMapEntry** object)
{
    VALIDATE_NOT_NULL(object);
    AutoPtr<IInterface> o;
    mI->GetNext((IInterface**)&o);
    AutoPtr<IMapEntry> res = new CheckedEntry(IMapEntry::Probe(o), mValueType);
    *object = res;
    REFCOUNT_ADD(*object)
    return NOERROR;
}

ECode Collections::CheckedMap::CheckedEntrySet::CheckedEntryIterator::GetNext(
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object);
    AutoPtr<IMapEntry> res;
    GetNext((IMapEntry**)&res);
    *object = res->Probe(EIID_IInterface);
    REFCOUNT_ADD(*object)
    return NOERROR;
}

//====================================================================
// Collections::CheckedMap::CheckedEntrySet::
//====================================================================
CAR_INTERFACE_IMPL(Collections::CheckedMap::CheckedEntrySet, Object, ISet)

Collections::CheckedMap::CheckedEntrySet::CheckedEntrySet(
    /* [in] */ ISet* s,
    /* [in] */ InterfaceID valueType)
{
    mS = s;
    mValueType = valueType;
}

ECode Collections::CheckedMap::CheckedEntrySet::GetIterator(
    /* [out] */ IIterator** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<IIterator> it;
    (IIterable::Probe(mS))->GetIterator((IIterator**)&it);
    AutoPtr<IIterator> res = new CheckedEntryIterator(it, mValueType);
    *result = res;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode Collections::CheckedMap::CheckedEntrySet::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    VALIDATE_NOT_NULL(array);
    Int32 thisSize;
    GetSize(&thisSize);
    AutoPtr<ArrayOf<IInterface*> > arr = ArrayOf<IInterface*>::Alloc(thisSize);
    AutoPtr<IIterator> it;
    GetIterator((IIterator**)&it);
    for (Int32 i = 0; i < thisSize; i++) {
        AutoPtr<IInterface> o;
        it->GetNext((IInterface**)&o);
        arr->Set(i, o);
    }
    *array = arr;
    REFCOUNT_ADD(*array)
    return NOERROR;
}

ECode Collections::CheckedMap::CheckedEntrySet::ToArray(
    /* [in] */ ArrayOf<IInterface*>* inArray,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    VALIDATE_NOT_NULL(outArray);
    Int32 thisSize;
    GetSize(&thisSize);
    if (inArray->GetLength() < thisSize) {
        inArray = ArrayOf<IInterface*>::Alloc(thisSize);
    }
    AutoPtr<IIterator> it;
    GetIterator((IIterator**)&it);
    for (Int32 i = 0; i < thisSize; i++) {
        AutoPtr<IInterface> o;
        it->GetNext((IInterface**)&o);
        inArray->Set(i, o);
    }
    if (thisSize < inArray->GetLength()) {
        inArray->Set(thisSize, NULL);
    }
    *outArray = inArray;
    REFCOUNT_ADD(*outArray)
    return NOERROR;
}

ECode Collections::CheckedMap::CheckedEntrySet::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    VALIDATE_NOT_NULL(modified);
    return (ICollection::Probe(mS))->RetainAll(collection, modified);
}

ECode Collections::CheckedMap::CheckedEntrySet::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    VALIDATE_NOT_NULL(modified);
    return (ICollection::Probe(mS))->RemoveAll(collection, modified);
}

ECode Collections::CheckedMap::CheckedEntrySet::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return (ICollection::Probe(mS))->ContainsAll(collection, result);
}

ECode Collections::CheckedMap::CheckedEntrySet::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    *modified = FALSE;
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode Collections::CheckedMap::CheckedEntrySet::Remove(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    VALIDATE_NOT_NULL(modified);
    return (ICollection::Probe(mS))->Remove(object, modified);
}

ECode Collections::CheckedMap::CheckedEntrySet::Contains(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return (ICollection::Probe(mS))->Contains(object, result);
}

ECode Collections::CheckedMap::CheckedEntrySet::Add(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    *modified = FALSE;
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode Collections::CheckedMap::CheckedEntrySet::IsEmpty(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return (ICollection::Probe(mS))->IsEmpty(result);
}

ECode Collections::CheckedMap::CheckedEntrySet::Clear()
{
    return (ICollection::Probe(mS))->Clear();
}

ECode Collections::CheckedMap::CheckedEntrySet::GetSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);
    return (ICollection::Probe(mS))->GetSize(size);
}

ECode Collections::CheckedMap::CheckedEntrySet::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode);
    return (ICollection::Probe(mS))->GetHashCode(hashCode);
}

ECode Collections::CheckedMap::CheckedEntrySet::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return (ICollection::Probe(mS))->Equals(object, result);
}

//====================================================================
// Collections::CheckedMap::
//====================================================================
CAR_INTERFACE_IMPL_2(Collections::CheckedMap, Object, IMap, ISerializable)

Collections::CheckedMap::CheckedMap(
    /* [in] */ IMap* m,
    /* [in] */ InterfaceID keyType,
    /* [in] */ InterfaceID valueType)
    : mM(m)
    , mKeyType(keyType)
    , mValueType(valueType)
{
    assert(mM);
}

ECode Collections::CheckedMap::GetSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);
    return mM->GetSize(size);
}

ECode Collections::CheckedMap::IsEmpty(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return mM->IsEmpty(result);
}

ECode Collections::CheckedMap::ContainsKey(
    /* [in] */ IInterface* key,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return mM->ContainsKey(key, result);
}

ECode Collections::CheckedMap::ContainsValue(
    /* [in] */ IInterface* value,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return mM->ContainsValue(value, result);
}

ECode Collections::CheckedMap::Get(
    /* [in] */ PInterface key,
    /* [out] */ PInterface* value)
{
    VALIDATE_NOT_NULL(value);
    return mM->Get(key, value);
}

ECode Collections::CheckedMap::Put(
    /* [in] */ PInterface key,
    /* [in] */ PInterface value,
    /* [out] */ PInterface* oldValue)
{
    FAIL_RETURN(Collections::CheckType(key, mKeyType));
    FAIL_RETURN(Collections::CheckType(value, mValueType));
    return mM->Put(key, value, oldValue);
}

ECode Collections::CheckedMap::Put(
    /* [in] */ PInterface key,
    /* [in] */ PInterface value)
{
    assert(0 && "TODO");
    return NOERROR;
}

ECode Collections::CheckedMap::Remove(
    /* [in] */ PInterface key,
    /* [out] */ PInterface* value)
{
    VALIDATE_NOT_NULL(value);
    return mM->Remove(key, value);
}

ECode Collections::CheckedMap::Remove(
    /* [in] */ PInterface key)
{
    assert(0 && "TODO");
    return NOERROR;
}

ECode Collections::CheckedMap::PutAll(
    /* [in] */ IMap* map)
{
    Int32 size;
    map->GetSize(&size);
    if (size == 0) {
        return NOERROR;
    }
    AutoPtr<ArrayOf<IMapEntry*> > entries = ArrayOf<IMapEntry*>::Alloc(size);
    AutoPtr<ISet> entry;
    map->GetEntrySet((ISet**)&entry);
    AutoPtr<IIterator> it;
    (IIterable::Probe(entry))->GetIterator((IIterator**)&it);
    for (Int32 i = 0; i < size; i++) {
        AutoPtr<IInterface> o;
        it->GetNext((IInterface**)&o);
        AutoPtr<IMapEntry> e = IMapEntry::Probe(o);
        if (e != NULL) {
            AutoPtr<IInterface> key;
            e->GetKey((IInterface**)&key);
            FAIL_RETURN(Collections::CheckType(key, mKeyType));
            AutoPtr<IInterface> val;
            e->GetValue((IInterface**)&val);
            FAIL_RETURN(Collections::CheckType(val, mValueType));
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

ECode Collections::CheckedMap::Clear()
{
    return mM->Clear();
}

ECode Collections::CheckedMap::GetKeySet(
    /* [out, callee] */ ISet** keySet)
{
    VALIDATE_NOT_NULL(keySet);
    return mM->GetKeySet(keySet);
}

ECode Collections::CheckedMap::GetValues(
    /* [out] */ ICollection** value)
{
    VALIDATE_NOT_NULL(value);
    return mM->GetValues(value);
}

ECode Collections::CheckedMap::GetEntrySet(
    /* [out, callee] */ ISet** entries)
{
    VALIDATE_NOT_NULL(entries);
    AutoPtr<ISet> set;
    mM->GetEntrySet((ISet**)&set);
    AutoPtr<ISet> res = new CheckedEntrySet(set, mValueType);
    *entries = res;
    REFCOUNT_ADD(*entries)
    return NOERROR;
}

ECode Collections::CheckedMap::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return mM->Equals(object, result);
}

ECode Collections::CheckedMap::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode);
    return mM->GetHashCode(hashCode);
}

ECode Collections::CheckedMap::ToString(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<IObject> iObj;
    iObj = IObject::Probe(mM);
    if (iObj != NULL) {
        return iObj->ToString(result);
    }
    *result = NULL;
    return NOERROR;
}

//====================================================================
// Collections::CheckedSortedSet::
//====================================================================
CAR_INTERFACE_IMPL(Collections::CheckedSortedSet, CheckedSet, ISortedSet)

Collections::CheckedSortedSet::CheckedSortedSet(
    /* [in] */ ISortedSet* s,
    /* [in] */ InterfaceID type)
    : CheckedSet(ISet::Probe(s), type)
{
    mSs = s;
}

ECode Collections::CheckedSortedSet::GetComparator(
    /* [out] */ IComparator** outcom)
{
    VALIDATE_NOT_NULL(outcom);
    return mSs->GetComparator(outcom);
}

ECode Collections::CheckedSortedSet::GetSubSet(
    /* [in] */ IInterface* start,
    /* [in] */ IInterface* end,
    /* [out] */ ISortedSet** outsort)
{
    VALIDATE_NOT_NULL(outsort);
    AutoPtr<ISortedSet> set;
    mSs->GetSubSet(start, end, (ISortedSet**)&set);
    AutoPtr<ISortedSet> res = new CheckedSortedSet(set, mType);
    *outsort = res;
    REFCOUNT_ADD(*outsort)
    return NOERROR;
}

ECode Collections::CheckedSortedSet::GetHeadSet(
    /* [in] */ IInterface* end,
    /* [out] */ ISortedSet** outsort)
{
    VALIDATE_NOT_NULL(outsort);
    AutoPtr<ISortedSet> set;
    mSs->GetHeadSet(end, (ISortedSet**)&set);
    AutoPtr<ISortedSet> res = new CheckedSortedSet(set, mType);
    *outsort = res;
    REFCOUNT_ADD(*outsort)
    return NOERROR;
}

ECode Collections::CheckedSortedSet::GetTailSet(
    /* [in] */ IInterface* start,
    /* [out] */ ISortedSet** outsort)
{
    VALIDATE_NOT_NULL(outsort);
    AutoPtr<ISortedSet> set;
    mSs->GetTailSet(start, (ISortedSet**)&set);
    AutoPtr<ISortedSet> res = new CheckedSortedSet(set, mType);
    *outsort = res;
    REFCOUNT_ADD(*outsort)
    return NOERROR;
}

ECode Collections::CheckedSortedSet::GetFirst(
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface);
    return mSs->GetFirst(outface);
}

ECode Collections::CheckedSortedSet::GetLast(
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface);
    return mSs->GetLast(outface);
}

ECode Collections::CheckedSortedSet::GetIterator(
    /* [out] */ IIterator** result)
{
    return CheckedSet::GetIterator(result);
}

ECode Collections::CheckedSortedSet::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    return CheckedSet::ToArray(array);
}

ECode Collections::CheckedSortedSet::ToArray(
    /* [in] */ ArrayOf<IInterface*>* inArray,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    return CheckedSet::ToArray(inArray, outArray);
}

ECode Collections::CheckedSortedSet::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return CheckedSet::RetainAll(collection, modified);
}

ECode Collections::CheckedSortedSet::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return CheckedSet::RemoveAll(collection, modified);
}

ECode Collections::CheckedSortedSet::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    return CheckedSet::ContainsAll(collection, result);
}

ECode Collections::CheckedSortedSet::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    return CheckedSet::AddAll(collection, modified);
}

ECode Collections::CheckedSortedSet::Remove(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return CheckedSet::Remove(object, modified);
}

ECode Collections::CheckedSortedSet::Contains(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return CheckedSet::Contains(object, result);
}

ECode Collections::CheckedSortedSet::Add(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return CheckedSet::Add(object, modified);
}

ECode Collections::CheckedSortedSet::IsEmpty(
    /* [out] */ Boolean* result)
{
    return CheckedSet::IsEmpty(result);
}

ECode Collections::CheckedSortedSet::Clear()
{
    return CheckedSet::Clear();
}

ECode Collections::CheckedSortedSet::GetSize(
    /* [out] */ Int32* size)
{
    return CheckedSet::GetSize(size);
}

ECode Collections::CheckedSortedSet::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    return CheckedSet::GetHashCode(hashCode);
}

ECode Collections::CheckedSortedSet::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return CheckedSet::Equals(object, result);
}

//====================================================================
// Collections::CheckedSortedMap::
//====================================================================
CAR_INTERFACE_IMPL(Collections::CheckedSortedMap, CheckedMap, ISortedMap)

Collections::CheckedSortedMap::CheckedSortedMap(
    /* [in] */ ISortedMap* m,
    /* [in] */ InterfaceID keyType,
    /* [in] */ InterfaceID valueType)
    : CheckedMap(IMap::Probe(m), keyType, valueType)
{
    mSm = m;
}

ECode Collections::CheckedSortedMap::GetComparator(
    /* [out] */ IComparator** comp)
{
    VALIDATE_NOT_NULL(comp);
    return mSm->GetComparator(comp);
}

ECode Collections::CheckedSortedMap::GetSubMap(
    /* [in] */ IInterface* startKey,
    /* [in] */ IInterface* endKey,
    /* [out] */ ISortedMap** sortmap)
{
    VALIDATE_NOT_NULL(sortmap);
    AutoPtr<ISortedMap> map;
    mSm->GetSubMap(startKey, endKey, (ISortedMap**)&map);
    AutoPtr<ISortedMap> res = new CheckedSortedMap(map, mKeyType, mValueType);
    *sortmap = res;
    REFCOUNT_ADD(*sortmap)
    return NOERROR;
}

ECode Collections::CheckedSortedMap::GetHeadMap(
    /* [in] */ IInterface* endKey,
    /* [out] */ ISortedMap** sortmap)
{
    VALIDATE_NOT_NULL(sortmap);
    AutoPtr<ISortedMap> map;
    mSm->GetHeadMap(endKey, (ISortedMap**)&map);
    AutoPtr<ISortedMap> res = new CheckedSortedMap(map, mKeyType, mValueType);
    *sortmap = res;
    REFCOUNT_ADD(*sortmap)
    return NOERROR;
}

ECode Collections::CheckedSortedMap::GetTailMap(
    /* [in] */ IInterface* startKey,
    /* [out] */ ISortedMap** sortmap)
{
    VALIDATE_NOT_NULL(sortmap);
    AutoPtr<ISortedMap> map;
    mSm->GetTailMap(startKey, (ISortedMap**)&map);
    AutoPtr<ISortedMap> res = new CheckedSortedMap(map, mKeyType, mValueType);
    *sortmap = res;
    REFCOUNT_ADD(*sortmap)
    return NOERROR;
}

ECode Collections::CheckedSortedMap::GetFirstKey(
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface);
    return mSm->GetFirstKey(outface);
}

ECode Collections::CheckedSortedMap::GetLastKey(
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface);
    return mSm->GetLastKey(outface);
}

ECode Collections::CheckedSortedMap::GetSize(
    /* [out] */ Int32* size)
{
    return CheckedMap::GetSize(size);
}

ECode Collections::CheckedSortedMap::IsEmpty(
    /* [out] */ Boolean* result)
{
    return CheckedMap::IsEmpty(result);
}

ECode Collections::CheckedSortedMap::ContainsKey(
    /* [in] */ IInterface* key,
    /* [out] */ Boolean* result)
{
    return CheckedMap::ContainsKey(key, result);
}

ECode Collections::CheckedSortedMap::ContainsValue(
    /* [in] */ IInterface* value,
    /* [out] */ Boolean* result)
{
    return CheckedMap::ContainsValue(value, result);
}

ECode Collections::CheckedSortedMap::Get(
    /* [in] */ PInterface key,
    /* [out] */ PInterface* value)
{
    return CheckedMap::Get(key, value);
}

ECode Collections::CheckedSortedMap::Put(
    /* [in] */ PInterface key,
    /* [in] */ PInterface value,
    /* [out] */ PInterface* oldValue)
{
    return CheckedMap::Put(key, value, oldValue);
}

ECode Collections::CheckedSortedMap::Remove(
    /* [in] */ PInterface key,
    /* [out] */ PInterface* value)
{
    return CheckedMap::Remove(key, value);
}

ECode Collections::CheckedSortedMap::PutAll(
    /* [in] */ IMap* map)
{
    return CheckedMap::PutAll(map);
}

ECode Collections::CheckedSortedMap::Clear()
{
    return CheckedMap::Clear();
}

ECode Collections::CheckedSortedMap::GetKeySet(
    /* [out, callee] */ ISet** keySet)
{
    return CheckedMap::GetKeySet(keySet);
}

ECode Collections::CheckedSortedMap::GetValues(
    /* [out] */ ICollection** value)
{
    return CheckedMap::GetValues(value);
}

ECode Collections::CheckedSortedMap::GetEntrySet(
    /* [out, callee] */ ISet** entries)
{
    return CheckedMap::GetEntrySet(entries);
}

ECode Collections::CheckedSortedMap::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return CheckedMap::Equals(object, result);
}

ECode Collections::CheckedSortedMap::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    return CheckedMap::GetHashCode(hashCode);
}

} // namespace Utility
} // namespace Elastos
