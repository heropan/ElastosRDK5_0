#include "AbstractSequentialList.h"

namespace Elastos {
namespace Utility {

ECode AbstractSequentialList::AddEx(
    /* [in] */ Int32 location,
    /* [in] */ IInterface* object)
{
    AutoPtr<IListIterator> it;
    GetListIteratorEx(location, (IListIterator**)&it);
    return it->Add(object);
}

ECode AbstractSequentialList::AddAllEx(
    /* [in] */ Int32 location,
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(collection);
    VALIDATE_NOT_NULL(result);
    AutoPtr<IListIterator> it;
    GetListIteratorEx(0, (IListIterator**)&it);
    AutoPtr<IIterator> colIt;
    collection->GetIterator((IIterator**)&colIt);
    Int32 next;
    it->NextIndex(&next);
    Boolean hasnext = FALSE;
    while ((colIt->HasNext(&hasnext), hasnext)) {
        AutoPtr<IInterface> nextobject;
        colIt->Next((IInterface**)&nextobject);
        it->Add(nextobject);
    }
    Int32 nextindex;
    it->NextIndex(&nextindex);
    *result = next != nextindex;
    return NOERROR;
}

ECode AbstractSequentialList::Get(
    /* [in] */ Int32 location,
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object);
    // try {
    AutoPtr<IListIterator> listiterator;
    GetListIteratorEx(location, (IListIterator**)&listiterator);
    ECode ec = listiterator->Next(object);
    if (FAILED(ec)) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    return NOERROR;
    // } catch (NoSuchElementException e) {
        // throw new IndexOutOfBoundsException();
    // }
}

ECode AbstractSequentialList::GetIterator(
    /* [out] */ IIterator** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<IListIterator> listiterator;
    GetListIteratorEx(0, (IListIterator**)&listiterator);
    *result = listiterator;
    INTERFACE_ADDREF(*result);
    return NOERROR;
}

ECode AbstractSequentialList::RemoveEx(
    /* [in] */ Int32 location,
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object);
    // try {
    AutoPtr<IListIterator> it;
    GetListIteratorEx(location, (IListIterator**)&it);
    ECode ec = it->Next(object);
    if (FAILED(ec)) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    return it->Remove();
    // } catch (NoSuchElementException e) {
        // throw new IndexOutOfBoundsException();
    // }
}

ECode AbstractSequentialList::Set(
    /* [in] */ Int32 location,
    /* [in] */ IInterface* object,
    /* [out] */ IInterface** prevObject)
{
    VALIDATE_NOT_NULL(prevObject);
    AutoPtr<IListIterator> it;
    GetListIteratorEx(location, (IListIterator**)&it);
    Boolean hasnext = FALSE;
    it->HasNext(&hasnext);
    if (!hasnext) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    it->Next(prevObject);
    return it->Set(object);
}

} // Utility
} // Elastos