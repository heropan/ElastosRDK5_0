
#include "CVector.h"
#include "Arrays.h"
#include "CCollections.h"
#include <elastos/StringBuilder.h>

using Elastos::Core::StringBuilder;
using Elastos::Utility::Arrays;

namespace Elastos {
namespace Utility {

//====================================================================
// _Vector::Enumeration
//====================================================================
CAR_INTERFACE_IMPL(_Vector::Enumeration, IEnumeration)

ECode _Vector::Enumeration::HasMoreElements(
    /* [out] */ Boolean* result)
{
    *result = mPos < mOwner->mElementCount;
    return NOERROR;
}

ECode _Vector::Enumeration::GetNextElement(
    /* [out] */ IInterface** out)
{
    Mutex::Autolock lock(mOwner->GetSelfLock());
    if (mPos < mOwner->mElementCount) {
        AutoPtr< ArrayOf<IInterface*> > a = mOwner->mElementData;
        *out = (*a)[mPos++];
        REFCOUNT_ADD(*out)
        return NOERROR;
    }
    return E_NO_SUCH_ELEMENT_EXCEPTION;
}

//====================================================================
// _Vector::
//====================================================================

const Int32 _Vector::DEFAULT_SIZE = 10;

_Vector::_Vector()
    : mElementCount(0)
    , mCapacityIncrement(0)
{}

ECode _Vector::Init(
    /* [in] */ Int32 capacity,
    /* [in] */ Int32 capacityIncrement)
{
    if (capacity < 0) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mElementData = NewElementArray(capacity);
    mElementCount = 0;
    mCapacityIncrement = capacityIncrement;
    return NOERROR;
}

ECode _Vector::Init(
    /* [in] */ ICollection* collection)
{
    VALIDATE_NOT_NULL(collection);
    Int32 size;
    collection->GetSize(&size);
    Init(size, 0);
    AutoPtr<IIterator> it;
    collection->GetIterator((IIterator**)&it);
    Boolean result;
    while ((it->HasNext(&result),result)) {
        AutoPtr<IInterface> next;
        it->GetNext((IInterface**)&next);
        mElementData->Set(mElementCount++, next);
    }
    return NOERROR;
}

AutoPtr<ArrayOf<IInterface*> > _Vector::NewElementArray(
    /* [in] */ Int32 size)
{
    AutoPtr<ArrayOf<IInterface*> > array = ArrayOf<IInterface*>::Alloc(size);
    return array;
}

ECode _Vector::Add(
    /* [in] */ Int32 location,
    /* [in] */ IInterface* object)
{
    return InsertElementAt(object, location);
}

ECode _Vector::Add(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    VALIDATE_NOT_NULL(modified)
    Mutex::Autolock lock(GetSelfLock());
    if (mElementCount == mElementData->GetLength()) {
        GrowByOne();
    }
    mElementData->Set(mElementCount++, object);
    mModCount++;
    *modified = TRUE;
    return NOERROR;
}

ECode _Vector::AddAll(
    /* [in] */ Int32 location,
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    VALIDATE_NOT_NULL(modified);
    VALIDATE_NOT_NULL(collection);
    Mutex::Autolock lock(GetSelfLock());
    if (location >= 0 && location <= mElementCount) {
        Int32 size;
        collection->GetSize(&size);
        if (size == 0) {
            *modified = FALSE;
            return NOERROR;
        }
        Int32 required = size - (mElementData->GetLength() - mElementCount);
        if (required > 0) {
            GrowBy(required);
        }
        Int32 count = mElementCount - location;
        if (count > 0) {
            mElementData->Copy(location + size, mElementData, location, count);
        }
        AutoPtr<IIterator> it;
        collection->GetIterator((IIterator**)&it);
        Boolean result = FALSE;
        while ((it->HasNext(&result),result)) {
            AutoPtr<IInterface> next;
            it->GetNext((IInterface**)&next);
            mElementData->Set(location++, next);
        }
        mElementCount += size;
        mModCount++;
        *modified = TRUE;
        return NOERROR;
    }
    return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
}

ECode _Vector::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    VALIDATE_NOT_NULL(modified)
    return AddAll(mElementCount, collection, modified);
}

ECode _Vector::AddElement(
    /* [in] */ IInterface* object)
{
    Mutex::Autolock lock(GetSelfLock());
    if (mElementCount == mElementData->GetLength()) {
        GrowByOne();
    }
    mElementData->Set(mElementCount++, object);
    mModCount++;
    return NOERROR;
}

ECode _Vector::Capacity(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)
    Mutex::Autolock lock(GetSelfLock());
    *value = mElementData->GetLength();
    return NOERROR;
}

ECode _Vector::Clear()
{
    return RemoveAllElements();
}

ECode _Vector::Clone(
    /* [in] */ _Vector* object)
{
    VALIDATE_NOT_NULL(object)
    Mutex::Autolock lock(GetSelfLock());;
    object->mElementData = mElementData->Clone();
    return NOERROR;
}

ECode _Vector::Contains(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    Int32 val;
    FAIL_RETURN(IndexOf(object, 0, &val));
    *result = (val != -1);
    return NOERROR;
}

ECode _Vector::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    Mutex::Autolock lock(GetSelfLock());
    return AbstractList::ContainsAll(collection, result);
}

ECode _Vector::CopyInto(
    /* [in] */ ArrayOf<IInterface*>* elements)
{
    VALIDATE_NOT_NULL(elements);
    Mutex::Autolock lock(GetSelfLock());
    elements->Copy(mElementData, mElementCount);
    return NOERROR;
}

ECode _Vector::ElementAt(
    /* [in] */ Int32 location,
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface)
    Mutex::Autolock lock(GetSelfLock());
    if (location >= 0 && location < mElementCount) {
        *outface = (*mElementData)[location];
        REFCOUNT_ADD(*outface)
        return NOERROR;
    }
    return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
}

ECode _Vector::GetElements(
    /* [out] */ IEnumeration** enu)
{
    VALIDATE_NOT_NULL(enu)
    *enu = new Enumeration(this);
    REFCOUNT_ADD(*enu)
    return NOERROR;
}

ECode _Vector::EnsureCapacity(
    /* [in] */ Int32 minimumCapacity)
{
    Mutex::Autolock lock(GetSelfLock());
    if (mElementData->GetLength() < minimumCapacity) {
        Int32 next = (mCapacityIncrement <= 0 ? mElementData->GetLength()
                : mCapacityIncrement)
                + mElementData->GetLength();
        Grow(minimumCapacity > next ? minimumCapacity : next);
    }
    return NOERROR;
}

ECode _Vector::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    Mutex::Autolock lock(GetSelfLock());
    if (THIS_PROBE(IInterface) == object) {
        *result = TRUE;
        return NOERROR;
    }

    if (IList::Probe(object) != NULL) {
        AutoPtr<IList> list = IList::Probe(object);
        Int32 size;
        list->GetSize(&size);
        if (size != mElementCount) {
            *result = FALSE;
            return NOERROR;
        }

        Int32 index = 0;
        Boolean hasNext = FALSE;
        AutoPtr<IIterator> it;
        GetIterator((IIterator**)&it);
        while ((it->HasNext(&hasNext), hasNext)) {
            AutoPtr<IInterface> e1 = (*mElementData)[index++];
            AutoPtr<IInterface> e2;
            it->GetNext((IInterface**)&e2);
            if (!(e1 == NULL ? e2 == NULL : Object::Equals(e1, e2))) {
                *result = FALSE;
                return NOERROR;
            }
        }
        *result = TRUE;
        return NOERROR;
    }
    *result = FALSE;
    return NOERROR;
}

ECode _Vector::FirstElement(
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface)
    Mutex::Autolock lock(GetSelfLock());
    if (mElementCount > 0) {
        *outface = (*mElementData)[0];
        REFCOUNT_ADD(*outface)
        return NOERROR;
    }
    *outface = NULL;
    return E_NO_SUCH_ELEMENT_EXCEPTION;
}

ECode _Vector::Get(
    /* [in] */ Int32 location,
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object)
    return ElementAt(location, object);
}

ECode _Vector::Grow(
    /* [in] */ Int32 newCapacity)
{
    AutoPtr< ArrayOf<IInterface*> > newData = NewElementArray(newCapacity);
    // Assumes elementCount is <= newCapacity
    assert(mElementCount <= newCapacity);
    newData->Copy(mElementData, mElementCount);
    mElementData = newData;
    return NOERROR;
}

ECode _Vector::GrowByOne()
{
    Int32 adding = 0;
    if (mCapacityIncrement <= 0) {
        if ((adding = mElementData->GetLength()) == 0) {
            adding = 1;
        }
    }
    else {
        adding = mCapacityIncrement;
    }

    AutoPtr< ArrayOf<IInterface*> > newData = NewElementArray(mElementData->GetLength() + adding);
    newData->Copy(mElementData, mElementCount);
    mElementData = newData;
    return NOERROR;
}

ECode _Vector::GrowBy(
    /* [in] */ Int32 required)
{
    Int32 adding = 0;
    if (mCapacityIncrement <= 0) {
        if ((adding = mElementData->GetLength()) == 0) {
            adding = required;
        }
        while (adding < required) {
            adding += adding;
        }
    }
    else {
        adding = (required / mCapacityIncrement) * mCapacityIncrement;
        if (adding < required) {
            adding += mCapacityIncrement;
        }
    }
    AutoPtr< ArrayOf<IInterface*> > newData = NewElementArray(mElementData->GetLength() + adding);
    newData->Copy(mElementData, mElementCount);
    mElementData = newData;
    return NOERROR;
}

ECode _Vector::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode)

    Mutex::Autolock lock(GetSelfLock());
    Int32 result = 1;
    for (Int32 i = 0; i < mElementCount; i++) {
        result = (31 * result)
                + ((*mElementData)[i] == NULL ? 0 : Object::GetHashCode((*mElementData)[i]));
    }
    *hashCode = result;
    return NOERROR;
}

ECode _Vector::IndexOf(
    /* [in] */ IInterface* object,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index)
    return IndexOf(object, 0, index);
}

ECode _Vector::IndexOf(
    /* [in] */ IInterface* object,
    /* [in] */ Int32 location,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)
    if (location < 0) {
        *value = -1;
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    Mutex::Autolock lock(GetSelfLock());
    if (object != NULL) {
        for (Int32 i = location; i < mElementCount; i++) {
            if (Object::Equals(object, (*mElementData)[i])) {
                *value = i;
                return NOERROR;
            }
        }
    }
    else {
        for (Int32 i = location; i < mElementCount; i++) {
            if ((*mElementData)[i] == NULL) {
                *value = i;
                return NOERROR;
            }
        }
    }

    *value = -1;
    return NOERROR;
}

ECode _Vector::InsertElementAt(
    /* [in] */ IInterface* object,
    /* [in] */ Int32 location)
{
    Mutex::Autolock lock(GetSelfLock());
    if (location >= 0 && location <= mElementCount) {
        if (mElementCount == mElementData->GetLength()) {
            GrowByOne();
        }
        Int32 count = mElementCount - location;
        if (count > 0) {
            mElementData->Copy(location + 1, mElementData, location, count);
        }
        mElementData->Set(location, object);
        mElementCount++;
        mModCount++;
        return NOERROR;
    }
    return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
}

ECode _Vector::IsEmpty(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    Mutex::Autolock lock(GetSelfLock());
    *result = (mElementCount == 0);
    return NOERROR;
}

ECode _Vector::LastElement(
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface)
    Mutex::Autolock lock(GetSelfLock());
    if (mElementCount == 0 || mElementData->GetLength() < mElementCount) {
        return E_NO_SUCH_ELEMENT_EXCEPTION;
    }
    *outface = (*mElementData)[mElementCount - 1];
    REFCOUNT_ADD(*outface)
    return NOERROR;
}

ECode _Vector::LastIndexOf(
    /* [in] */ IInterface* object,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index)
    return LastIndexOf(object, mElementCount - 1, index);
}

ECode _Vector::LastIndexOf(
    /* [in] */ IInterface* object,
    /* [in] */ Int32 location,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)
    Mutex::Autolock lock(GetSelfLock());
    if (location >= 0 && location < mElementCount) {
        if (object != NULL) {
            for (Int32 i = location; i >= 0; i--) {
                if (Object::Equals(object, (*mElementData)[i])) {
                    *value = i;
                    return NOERROR;
                }
            }
        }
        else {
            for (Int32 i = location; i >= 0; i--) {
                if ((*mElementData)[i] == NULL) {
                    *value = i;
                    return NOERROR;
                }
            }
        }
        *value = -1;
        return NOERROR;
    }
    return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
}

ECode _Vector::Remove(
    /* [in] */ Int32 location,
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object)
    Mutex::Autolock lock(GetSelfLock());
    if (location >= 0 && location < mElementCount) {
        AutoPtr<IInterface> result = (*mElementData)[location];
        mElementCount--;
        Int32 size = mElementCount - location;
        if (size > 0) {
            mElementData->Copy(location, mElementData, location + 1, size);
        }
        mElementData->Set(mElementCount, NULL);
        mModCount++;
        *object = result;
        REFCOUNT_ADD(*object)
        return NOERROR;
    }
    return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
}

ECode _Vector::Remove(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    VALIDATE_NOT_NULL(modified)
    return RemoveElement(object, modified);
}

ECode _Vector::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    VALIDATE_NOT_NULL(modified)
    Mutex::Autolock lock(GetSelfLock());
    return AbstractList::RemoveAll(collection, modified);
}

ECode _Vector::RemoveAllElements()
{
    Mutex::Autolock lock(GetSelfLock());
    for (Int32 i = 0; i < mElementCount; i++) {
        mElementData->Set(i, NULL);
    }
    mModCount++;
    mElementCount = 0;
    return NOERROR;
}

ECode _Vector::RemoveElement(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)
    Mutex::Autolock lock(GetSelfLock());
    Int32 index;
    IndexOf(object, 0, &index);
    if (index == -1) {
        *value = FALSE;
        return NOERROR;
    }
    RemoveElementAt(index);
    *value = TRUE;
    return NOERROR;
}

ECode _Vector::RemoveElementAt(
    /* [in] */ Int32 location)
{
    Mutex::Autolock lock(GetSelfLock());
    if (location >= 0 && location < mElementCount) {
        mElementCount--;
        Int32 size = mElementCount - location;
        if (size > 0) {
            mElementData->Copy(location, mElementData, location + 1, size);
        }
        mElementData->Set(mElementCount, NULL);
        mModCount++;
        return NOERROR;
    }
    return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
}

ECode _Vector::RemoveRange(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    if (start >= 0 && start <= end && end <= mElementCount) {
        if (start == end) {
            return NOERROR;
        }
        if (end != mElementCount) {
            mElementData->Copy(start, mElementData, end, mElementCount - end);
            Int32 newCount = mElementCount - (end - start);
            for (Int32 i = newCount; i < mElementCount; ++i) {
                mElementData->Set(i, NULL);
            }
            mElementCount = newCount;
        }
        else {
            for (Int32 i = start; i < mElementCount; ++i) {
                mElementData->Set(i, NULL);
            }
            mElementCount = start;
        }
        mModCount++;
        return NOERROR;
    }
    return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
}

ECode _Vector::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    VALIDATE_NOT_NULL(modified)
    Mutex::Autolock lock(GetSelfLock());
    return AbstractList::RetainAll(collection, modified);
}

ECode _Vector::Set(
    /* [in] */ Int32 location,
    /* [in] */ IInterface* object,
    /* [out] */ IInterface** prevObject)
{
    VALIDATE_NOT_NULL(prevObject)
    Mutex::Autolock lock(GetSelfLock());
    if (location >= 0 && location < mElementCount) {
        AutoPtr<IInterface> result = (*mElementData)[location];
        mElementData->Set(location, object);
        *prevObject = result;
        REFCOUNT_ADD(*prevObject)
        return NOERROR;
    }
    return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
}

ECode _Vector::SetElementAt(
    /* [in] */ IInterface* object,
    /* [in] */ Int32 location)
{
    Mutex::Autolock lock(GetSelfLock());
    if (location >=0 && location < mElementCount) {
        mElementData->Set(location, object);
        return NOERROR;
    }
    return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
}

ECode _Vector::SetSize(
    /* [in] */ Int32 length)
{
    Mutex::Autolock lock(GetSelfLock());
    if (length == mElementCount){
        return NOERROR;
    }
    EnsureCapacity(length);
    if (mElementCount > length) {
        FAIL_RETURN(Arrays::Fill(mElementData.Get(), length, mElementCount, NULL));
    }
    mElementCount = length;
    mModCount++;
    return NOERROR;
}

ECode _Vector::GetSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size)
    Mutex::Autolock lock(GetSelfLock());
    *size = mElementCount;
    return NOERROR;
}

ECode _Vector::GetSubList(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ IList** subList)
{
    VALIDATE_NOT_NULL(subList)
    Mutex::Autolock lock(GetSelfLock());
    AutoPtr<IList> ssList;
    AbstractList::GetSubList(start, end, (IList**)&ssList);
    *subList = new CCollections::SynchronizedRandomAccessList(ssList, GetSelfLock());
    REFCOUNT_ADD(*subList);
    return NOERROR;
}

ECode _Vector::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    VALIDATE_NOT_NULL(array)
    Mutex::Autolock lock(GetSelfLock());
    AutoPtr< ArrayOf<IInterface*> > result = ArrayOf<IInterface*>::Alloc(mElementCount);
    result->Copy(mElementData, mElementCount);
    *array = result;
    REFCOUNT_ADD(*array)
    return NOERROR;
}

ECode _Vector::ToArray(
    /* [in] */ ArrayOf<IInterface*>* inArray,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    VALIDATE_NOT_NULL(outArray)
    Mutex::Autolock lock(GetSelfLock());
    AutoPtr< ArrayOf<IInterface*> > contents = inArray;
    if (mElementCount > contents->GetLength()) {
        contents = ArrayOf<IInterface*>::Alloc(mElementCount);
    }
    contents->Copy(mElementData, mElementCount);
    if (mElementCount < contents->GetLength()) {
        contents->Set(mElementCount, NULL);
    }
    *outArray = contents;
    REFCOUNT_ADD(*outArray)
    return NOERROR;
}

CARAPI _Vector::ToString(
    /* [out] */ String* result)
{
    Mutex::Autolock lock(GetSelfLock());
    if (mElementCount == 0) {
        *result = String("[]");
        return NOERROR;
    }
    Int32 length = mElementCount - 1;
    StringBuilder buffer(mElementCount * 16);
    buffer.AppendChar('[');
    for (Int32 i = 0; i < length; i++) {
        AutoPtr<IInterface> ele = (*mElementData)[i];
        if (ele->Probe(EIID_IInterface) == Probe(EIID_IInterface)) {
            buffer.AppendString(String("(this Collection)"));
        }
        else {
            buffer.AppendObject((*mElementData)[i]);
        }
        buffer.AppendString(String(", "));
    }
    AutoPtr<IInterface> lastEle = (*mElementData)[length];
    if (lastEle->Probe(EIID_IInterface) == Probe(EIID_IInterface)) {
        buffer.AppendString(String("(this Collection)"));
    }
    else {
        buffer.AppendObject((*mElementData)[length]);
    }
    buffer.AppendChar(']');
    *result = buffer.ToString();
    return NOERROR;
}

ECode _Vector::TrimToSize()
{
    Mutex::Autolock lock(GetSelfLock());
    if (mElementData->GetLength() != mElementCount) {
        Grow(mElementCount);
    }
    return NOERROR;
}

ECode _Vector::WriteObject(
    /* [in] */ IObjectOutputStream* stream)
{
    Mutex::Autolock lock(GetSelfLock());
    return stream->DefaultWriteObject();
}

} // namespace Utility
} // namespace Elastos
