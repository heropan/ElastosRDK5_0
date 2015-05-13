
#include "cmdef.h"
#include "AbstractList.h"
#include <elastos/ObjectUtils.h>

using Elastos::Core::ObjectUtils;
using Elastos::Utility::IList;

namespace Elastos {
namespace Utility {

//=================================================
// AbstractList::SimpleListIterator
//=================================================
AbstractList::SimpleListIterator::SimpleListIterator(
    /* [in] */ AbstractList* owner)
    : mPos(-1)
    , mExpectedModeCount(owner->mModCount)
    , mLastPosition(-1)
    , mOwner(owner)
{}

CAR_INTERFACE_IMPL(AbstractList::SimpleListIterator, IIterator);

ECode AbstractList::SimpleListIterator::HasNext(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    Int32 size;
    mOwner->GetSize(&size);
    *result = mPos + 1 < size;
    return NOERROR;
}

ECode AbstractList::SimpleListIterator::Next(
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object);
    if (mExpectedModeCount== mOwner->mModCount) {
        // try {
        ECode ec = mOwner->Get(mPos + 1, object);
        if (FAILED(ec)) return E_NO_SUCH_ELEMENT_EXCEPTION;
        mLastPosition = ++mPos;
        return NOERROR;
        // } catch (IndexOutOfBoundsException e) {
            // throw new NoSuchElementException();
        // }
    }
    return E_CONCURRENT_MODIFICATION_EXCEPTION;
}

ECode AbstractList::SimpleListIterator::Remove()
{
    if (mLastPosition == -1) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    if (mExpectedModeCount != mOwner->mModCount) {
        return E_CONCURRENT_MODIFICATION_EXCEPTION;
    }

    // try {
    AutoPtr<IInterface> object;
    ECode ec = mOwner->RemoveEx(mLastPosition, (IInterface**)&object);
    if (FAILED(ec)) return E_CONCURRENT_MODIFICATION_EXCEPTION;
    // } catch (IndexOutOfBoundsException e) {
        // throw new ConcurrentModificationException();
    // }

    mExpectedModeCount = mOwner->mModCount;
    if (mPos == mLastPosition) {
        mPos--;
    }
    mLastPosition = -1;
    return NOERROR;
}


//=================================================
// AbstractList::FullListIterator
//=================================================
AbstractList::FullListIterator::FullListIterator(
    /* [in] */ Int32 start,
    /* [in] */ AbstractList* owner)
    :SimpleListIterator(owner)
{
    Int32 size;
    mOwner->GetSize(&size);
    if (start >= 0 && start <= size) {
        mPos = start - 1;
    }
}

CAR_INTERFACE_IMPL(AbstractList::FullListIterator, IListIterator);

ECode AbstractList::FullListIterator::Add(
    /* [in] */ IInterface* object)
{
    if (mExpectedModeCount == mOwner->mModCount) {
        // try {
        ECode ec = mOwner->AddEx(mPos + 1, object);
        if (FAILED(ec)) {
            return E_NO_SUCH_ELEMENT_EXCEPTION;
        }
        // } catch (IndexOutOfBoundsException e) {
            // throw new NoSuchElementException();
        // }
        mPos++;
        mLastPosition = -1;
        if (mOwner->mModCount != mExpectedModeCount) {
            mExpectedModeCount = mOwner->mModCount;
        }
        return NOERROR;
    }
    else {
        return E_CONCURRENT_MODIFICATION_EXCEPTION;
    }
}

ECode AbstractList::FullListIterator::HasPrevious(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mPos >= 0;
    return NOERROR;
}

ECode AbstractList::FullListIterator::NextIndex(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mPos + 1;
    return NOERROR;
}

ECode AbstractList::FullListIterator::Previous(
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object);
    if (mExpectedModeCount == mOwner->mModCount) {
        // try {
        ECode ec = mOwner->Get(mPos, object);
        if (FAILED(ec)) {
            return E_NO_SUCH_ELEMENT_EXCEPTION;
        }
        mLastPosition = mPos;
        mPos--;
        return NOERROR;
        // } catch (IndexOutOfBoundsException e) {
            // throw new NoSuchElementException();
        // }
    }
    return E_CONCURRENT_MODIFICATION_EXCEPTION;
}

ECode AbstractList::FullListIterator::PreviousIndex(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mPos;
    return NOERROR;
}

ECode AbstractList::FullListIterator::Set(
    /* [in] */ IInterface* object)
{
    if (mExpectedModeCount == mOwner->mModCount) {
        // try {
        ECode ec = mOwner->SetEx(mLastPosition, object);
        if (FAILED(ec)) {
            return E_ILLEGAL_STATE_EXCEPTION;
        }
        return NOERROR;
        // } catch (IndexOutOfBoundsException e) {
            // throw new IllegalStateException();
        // }
    }
    else {
        return E_CONCURRENT_MODIFICATION_EXCEPTION;
    }
}

ECode AbstractList::FullListIterator::HasNext(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return SimpleListIterator::HasNext(result);
}

ECode AbstractList::FullListIterator::Next(
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object);
    return SimpleListIterator::Next(object);
}

ECode AbstractList::FullListIterator::Remove()
{
    return SimpleListIterator::Remove();
}

SubAbstractList::SubAbstractListIterator::SubAbstractListIterator(
    /* [in] */ IListIterator* it,
    /* [in] */ SubAbstractList* list,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length)
    : mSubList(list)
    , mIterator(it)
    , mStart(offset)
    , mEnd(mStart + length)
{
}

CAR_INTERFACE_IMPL_LIGHT(SubAbstractList::SubAbstractListIterator, IListIterator);

ECode SubAbstractList::SubAbstractListIterator::Add(
    /* [in] */ IInterface* object)
{
    mIterator->Add(object);
    mSubList->SizeChanged(TRUE);
    mEnd++;
    return NOERROR;
}

ECode SubAbstractList::SubAbstractListIterator::HasNext(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    Int32 index;
    mIterator->NextIndex(&index);
    *result = index < mEnd;
    return NOERROR;
}

ECode SubAbstractList::SubAbstractListIterator::HasPrevious(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    Int32 previousindex;
    mIterator->PreviousIndex(&previousindex);
    *result = previousindex >= mStart;
    return NOERROR;
}

ECode SubAbstractList::SubAbstractListIterator::Next(
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object);
    Int32 index;
    mIterator->NextIndex(&index);
    if (index < mEnd) {
        return mIterator->Next(object);
    }
    return E_NO_SUCH_ELEMENT_EXCEPTION;
}

ECode SubAbstractList::SubAbstractListIterator::NextIndex(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    Int32 index;
    mIterator->NextIndex(&index);
    *result = index - mStart;
    return NOERROR;
}

ECode SubAbstractList::SubAbstractListIterator::Previous(
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object);
    Int32 previousindex;
    mIterator->PreviousIndex(&previousindex);
    if (previousindex >= mStart) {
       return mIterator->Previous(object);
    }
    return E_NO_SUCH_ELEMENT_EXCEPTION;
}

ECode SubAbstractList::SubAbstractListIterator::PreviousIndex(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    Int32 previousindex;
    mIterator->PreviousIndex(&previousindex);
    if (previousindex >= mStart) {
        *result = previousindex - mStart;
        return NOERROR;
    }
    *result = -1;
    return NOERROR;
}

ECode SubAbstractList::SubAbstractListIterator::Remove()
{
    mIterator->Remove();
    mSubList->SizeChanged(FALSE);
    mEnd--;
    return NOERROR;
}

ECode SubAbstractList::SubAbstractListIterator::Set(
    /* [in] */ IInterface* object)
{
    return mIterator->Set(object);
}

SubAbstractList::SubAbstractList(
    /* [in] */ AbstractList* list,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
    : mFulllist(list)
    , mOffset(start)
    , mSize(end - start)
{
    mModCount = mFulllist->mModCount;
}

CAR_INTERFACE_IMPL(SubAbstractList, IList);

ECode SubAbstractList::AddEx(
    /* [in] */ Int32 location,
    /* [in] */ IInterface* object)
{
    if (mModCount == mFulllist->mModCount) {
        if (location >= 0 && location <= mSize) {
            mFulllist->AddEx(location + mOffset, object);
            mSize++;
            mModCount = mFulllist->mModCount;
            return NOERROR;
        }
        else {
            return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
        }
    }
    else {
        return E_CONCURRENT_MODIFICATION_EXCEPTION;
    }
}

ECode SubAbstractList::AddAllEx(
    /* [in] */ Int32 location,
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(collection);
    VALIDATE_NOT_NULL(result);
    if (mModCount == mFulllist->mModCount) {
        if (location >= 0 && location <= mSize) {
            *result = FALSE;
            mFulllist->AddAllEx(location + mOffset, collection, result);
            if (*result) {
                Int32 collectionsize;
                collection->GetSize(&collectionsize);
                mSize += collectionsize;
                mModCount += mFulllist->mModCount;
            }
            return NOERROR;
        }
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    return E_CONCURRENT_MODIFICATION_EXCEPTION;
}

ECode SubAbstractList::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    if (mModCount == mFulllist->mModCount) {
        *result = FALSE;
        mFulllist->AddAllEx(mOffset + mSize, collection, result);
        if (*result) {
            Int32 collectionsize;
            collection->GetSize(&collectionsize);
            mSize += collectionsize;
            mModCount = mFulllist->mModCount;
        }
        return NOERROR;
    }
    return E_CONCURRENT_MODIFICATION_EXCEPTION;
}

ECode SubAbstractList::Get(
    /* [in] */ Int32 location,
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object);
    if (mModCount == mFulllist->mModCount) {
        if (location >= 0 && location < mSize) {
            return mFulllist->Get(location + mOffset, object);
        }
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    return E_CONCURRENT_MODIFICATION_EXCEPTION;
}

ECode SubAbstractList::GetIterator(
    /* [out] */ IIterator** it)
{
    VALIDATE_NOT_NULL(it);
    AutoPtr<IListIterator> listit;
    GetListIteratorEx(0, (IListIterator**)&listit);
    *it = listit;
    INTERFACE_ADDREF(*it);
    return NOERROR;
}

ECode SubAbstractList::GetListIteratorEx(
    /* [in] */ Int32 location,
    /* [out] */ IListIterator** listiterator)
{
    VALIDATE_NOT_NULL(listiterator);
    if (mModCount == mFulllist->mModCount) {
        if (location >= 0 && location <= mSize) {
            AutoPtr<IListIterator> listiteratorex;
            mFulllist->GetListIteratorEx(location + mOffset, (IListIterator**)&listiteratorex);
            AutoPtr<SubAbstractListIterator> subabstractlistiterator =
                      new SubAbstractListIterator(listiteratorex, this, mOffset, mSize);
            *listiterator = subabstractlistiterator;
            INTERFACE_ADDREF(*listiterator);
            return NOERROR;
        }
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    return E_CONCURRENT_MODIFICATION_EXCEPTION;
}

ECode SubAbstractList::RemoveEx(
    /* [in] */ Int32 location,
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object);
    if (mModCount == mFulllist->mModCount) {
        if (location >= 0 && location < mSize) {
            mFulllist->RemoveEx(location + mOffset, object);
            mSize--;
            mModCount =  mFulllist->mModCount;
            return NOERROR;
        }
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    return E_CONCURRENT_MODIFICATION_EXCEPTION;
}

ECode SubAbstractList::RemoveRange(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    if (start != end) {
        if (mModCount == mFulllist->mModCount) {
            mFulllist->RemoveRange(start + mOffset, end + mOffset);
            mSize -= (end - start);
            mModCount = mFulllist->mModCount;
            return NOERROR;
        }
        else {
            return E_CONCURRENT_MODIFICATION_EXCEPTION;
        }
    }
    return NOERROR;
}

ECode SubAbstractList::Set(
    /* [in] */ Int32 location,
    /* [in] */ IInterface* inobject,
    /* [out] */ IInterface** outobject)
{
    VALIDATE_NOT_NULL(outobject);
    if (mModCount == mFulllist->mModCount) {
        if (location >= 0 && location < mSize) {
           return mFulllist->Set(location + mOffset, inobject, outobject);
        }
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    return E_CONCURRENT_MODIFICATION_EXCEPTION;
}

ECode SubAbstractList::GetSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);
    if (mModCount == (mFulllist->mModCount)) {
        *size = mSize;
        return NOERROR;
    }
    return E_CONCURRENT_MODIFICATION_EXCEPTION;
}

ECode SubAbstractList::SizeChanged(
    /* [in] */ Boolean increment)
{
    if (increment) {
        mSize++;
    }
    else {
        mSize--;
    }
    mModCount = mFulllist->mModCount;
    return NOERROR;
}

ECode SubAbstractList::Contains(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return AbstractList::Contains(object, result);
}

ECode SubAbstractList::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return AbstractList::ContainsAll(collection, result);
}

ECode SubAbstractList::IsEmpty(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return AbstractList::IsEmpty(result);
}

ECode SubAbstractList::Remove(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return AbstractList::Remove(object, result);
}

ECode SubAbstractList::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return AbstractList::RemoveAll(collection, result);
}

ECode SubAbstractList::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return AbstractList::RetainAll(collection, result);
}

ECode SubAbstractList::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    VALIDATE_NOT_NULL(array);
    return AbstractList::ToArray(array);
}

ECode SubAbstractList::ToArrayEx(
    /* [in] */ ArrayOf<IInterface*>* contents,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    VALIDATE_NOT_NULL(outArray);
    return AbstractList::ToArrayEx(contents, outArray);
}

ECode SubAbstractList::ToString(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    return AbstractList::ToString(result);
}

ECode SubAbstractList::Add(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    VALIDATE_NOT_NULL(modified);
    return AbstractList::Add(object, modified);
}

ECode SubAbstractList::Clear()
{
    return AbstractList::Clear();
}

ECode SubAbstractList::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return AbstractList::Equals(object, result);
}

ECode SubAbstractList::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode);
    return AbstractList::GetHashCode(hashCode);
}

ECode SubAbstractList::IndexOf(
    /* [in] */ IInterface* object,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);
    return AbstractList::IndexOf(object, index);
}

ECode SubAbstractList::LastIndexOf(
    /* [in] */ IInterface* object,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);
    return AbstractList::LastIndexOf(object, index);
}

ECode SubAbstractList::GetListIterator(
    /* [out] */ IListIterator** it)
{
    VALIDATE_NOT_NULL(it);
    return AbstractList::GetListIterator(it);
}

ECode SubAbstractList::SubList(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ IList** subList)
{
    VALIDATE_NOT_NULL(subList);
    return AbstractList::SubList(start, end, subList);
}

ECode AbstractList::AddEx(
    /* [in] */ Int32 location,
    /* [in] */ IInterface* object)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode AbstractList::Add(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    Int32 size;
    GetSize(&size);
    FAIL_RETURN(AddEx(size, object));
    *result = TRUE;
    return NOERROR;
}

ECode AbstractList::AddAllEx(
    /* [in] */ Int32 location,
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(collection);
    VALIDATE_NOT_NULL(result);
    AutoPtr<IIterator> it;
    collection->GetIterator((IIterator**)&it);
    Boolean hasnext = FALSE;
    while ((it->HasNext(&hasnext), hasnext)) {
        AutoPtr<IInterface> nextobject;
        it->Next((IInterface**)&nextobject);
        FAIL_RETURN(AddEx(location++, nextobject));
    }
    Boolean flag = FALSE;
    collection->IsEmpty(&flag);
    *result = !flag;
    return NOERROR;
}

ECode AbstractList::Clear()
{
    Int32 size;
    GetSize(&size);
    return RemoveRange(0, size);
}

ECode AbstractList::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    if (this->Probe(EIID_IInterface) == object) {
        *result = TRUE;
        return NOERROR;
    }
    AutoPtr<IList> list = (IList*)(object->Probe(EIID_IList));
    if (list == NULL) {
        *result = FALSE;
        return NOERROR;
    }

    Int32 listsize;
    list->GetSize(&listsize);
    Int32 size;
    GetSize(&size);
    if (listsize != size) {
        *result = FALSE;
        return NOERROR;
    }
    AutoPtr<IIterator> it1;
    GetIterator((IIterator**)&it1);
    AutoPtr<IIterator> it2;
    list->GetIterator((IIterator**)&it2);
    Boolean hasnext = FALSE;
    while ((it1->HasNext(&hasnext), hasnext)) {
        AutoPtr<IInterface> e1;
        it1->Next((IInterface**)&e1);
        AutoPtr<IInterface> e2;
        it2->Next((IInterface**)&e2);
        if (!ObjectUtils::Equals(e1, e2)) {
            *result = FALSE;
            return NOERROR;
        }
    }
    *result = TRUE;
    return NOERROR;
}

ECode AbstractList::GetHashCode(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = 1;
    AutoPtr<IIterator> it;
    GetIterator((IIterator**)&it);
    Boolean hasnext = FALSE;
    while ((it->HasNext(&hasnext), hasnext)) {
        AutoPtr<IInterface> object;
        it->Next((IInterface**)&object);
        if (object == NULL) {
            *result = 31 * (*result);
        }
        else {
            Int32 ret = ObjectUtils::GetHashCode(object);
            *result = 31 * (*result) + ret;
        }
    }
    return NOERROR;
}

ECode AbstractList::IndexOf(
    /* [in] */ IInterface* object,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<IListIterator> it;
    GetListIterator((IListIterator**)&it);
    Boolean hasnext = FALSE;
    if (object != NULL) {
        while ((it->HasNext(&hasnext), hasnext)) {
            AutoPtr<IInterface> nextobject;
            it->Next((IInterface**)&nextobject);
            if (ObjectUtils::Equals(object, nextobject)) {
                Int32 previousindex;
                it->PreviousIndex(&previousindex);
                *result = previousindex;
                return NOERROR;
            }
        }
    } else {
        while ((it->HasNext(&hasnext), hasnext)) {
            AutoPtr<IInterface> nextobject;
            it->Next((IInterface**)&nextobject);
            if (nextobject == NULL) {
                Int32 previousindex;
                it->PreviousIndex(&previousindex);
                *result = previousindex;
                return NOERROR;
            }
        }
    }
    *result = -1;
    return NOERROR;
}

ECode AbstractList::GetIterator(
    /* [out] */ IIterator** it)
{
    VALIDATE_NOT_NULL(it);
    AutoPtr<SimpleListIterator> simplelistiterator =
        new AbstractList::SimpleListIterator(this);
    *it = simplelistiterator;
    INTERFACE_ADDREF(*it);
    return NOERROR;
}

ECode AbstractList::LastIndexOf(
    /* [in] */ IInterface* object,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    Int32 size;
    GetSize(&size);
    AutoPtr<IListIterator> it;
    GetListIteratorEx(size, (IListIterator**)&it);
    Boolean hasprevious = FALSE;
    if (object != NULL) {
        while((it->HasPrevious(&hasprevious), hasprevious)) {
            AutoPtr<IInterface> previousobject;
            it->Previous((IInterface**)&previousobject);
            if (ObjectUtils::Equals(object, previousobject)) {
                Int32 nextindex;
                it->NextIndex(&nextindex);
                *result = nextindex;
                return NOERROR;
            }
        }
    }
    else {
        while ((it->HasPrevious(&hasprevious), hasprevious)) {
            AutoPtr<IInterface> previousobject;
            it->Previous((IInterface**)&previousobject);
            if (previousobject == NULL) {
                Int32 nextindex;
                it->NextIndex(&nextindex);
                *result = nextindex;
                return NOERROR;
            }
        }
    }
    *result = -1;
    return NOERROR;
}

ECode AbstractList::GetListIterator(
    /* [out] */ IListIterator** listiterator)
{
    VALIDATE_NOT_NULL(listiterator);
    return GetListIteratorEx(0, listiterator);
}

ECode AbstractList::GetListIteratorEx(
    /* [in] */ Int32 location,
    /* [out] */ IListIterator** listiterator)
{
    VALIDATE_NOT_NULL(listiterator);
    AutoPtr<FullListIterator> full = new FullListIterator(location, this);
    *listiterator = full;
    INTERFACE_ADDREF(*listiterator);
    return NOERROR;
}

ECode AbstractList::RemoveEx(
    /* [in] */ Int32 location,
    /* [out] */ IInterface** object)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode AbstractList::RemoveRange(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    AutoPtr<IIterator> it;
    AutoPtr<IListIterator> listit;
    GetListIteratorEx(start, (IListIterator**)&listit);
    it = listit;
    for (Int32 i = start; i < end; i++) {
        AutoPtr<IInterface> nextobject;
        it->Next((IInterface**)&nextobject);
        it->Remove();
    }
    return NOERROR;
}

ECode AbstractList::SetEx(
    /* [in] */ Int32 location,
    /* [in] */ IInterface* object)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode AbstractList::SubList(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ IList** list)
{
    VALIDATE_NOT_NULL(list);
    Int32 size;
    GetSize(&size);
    if (start >= 0 && end <= size) {
        if (start <= end) {
            if (THIS_PROBE(IRandomAccess)) {
                AutoPtr<SubAbstractListRandomAccess> sublistrandom = new SubAbstractListRandomAccess(this, start, end);
                *list = sublistrandom;
                INTERFACE_ADDREF(*list);
                return NOERROR;
            }
            AutoPtr<SubAbstractList> sublist = new SubAbstractList(this, start, end);
            *list = sublist;
            INTERFACE_ADDREF(*list);
            return NOERROR;
        }
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
}

SubAbstractListRandomAccess::SubAbstractListRandomAccess(
    /* [in] */ AbstractList* list,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
    : SubAbstractList(list, start, end)
{
}

CAR_INTERFACE_IMPL(SubAbstractListRandomAccess, IList);

} // Utility
} // Elastos
