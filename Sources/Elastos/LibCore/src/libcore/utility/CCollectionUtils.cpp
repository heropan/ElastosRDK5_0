
#include "cmdef.h"
#include "CCollectionUtils.h"
#include "CCollections.h"

using Elastos::Utility::CCollections;
using Elastos::Utility::EIID_IIterator;
using Elastos::Utility::EIID_IIterable;

namespace Libcore {
namespace Utility {

//===============================================================================
// CCollectionUtils::_Iterator
//===============================================================================

CCollectionUtils::_Iterator::_Iterator(
    /* [in] */ IIterable* iterable,
    /* [in] */ Boolean trim)
    : mIter(iterable)
    , mTrim(trim)
    , mRemoveIsOkay(FALSE)
{
    mIter->GetIterator((IIterator**)&mDelegate);
}

CAR_INTERFACE_IMPL(CCollectionUtils::_Iterator, IIterator);

ECode CCollectionUtils::_Iterator::HasNext(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    ComputeNext();
    *result = mNext != NULL;
    return NOERROR;
}

ECode CCollectionUtils::_Iterator::Next(
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object)

    Boolean isflag = FALSE;
    if (HasNext(&isflag), !isflag) {
        // throw new IllegalStateException();
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    AutoPtr<IInterface> result = mNext;
    mRemoveIsOkay = TRUE;
    mNext = NULL;
    *object = result;
    INTERFACE_ADDREF(*object)
    return NOERROR;
}

ECode CCollectionUtils::_Iterator::Remove()
{
    if (!mRemoveIsOkay) {
        // throw new IllegalStateException();
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    return mDelegate->Remove();
}

ECode CCollectionUtils::_Iterator::ComputeNext()
{
    mRemoveIsOkay = FALSE;
    Boolean isflag = FALSE;
    while (mNext == NULL && (mDelegate->HasNext(&isflag), isflag)) {
        mDelegate->Next((IInterface**)&mNext);
        if (mTrim && mNext == NULL) {
            mDelegate->Remove();
        }
    }
    return NOERROR;
}

//===============================================================================
// CCollectionUtils::_Iterable
//===============================================================================

CCollectionUtils::_Iterable::_Iterable(
    /* [in] */ IIterable* iterable,
    /* [in] */ Boolean trim)
    : mIter(iterable)
    , mTrim(trim)
{
}

CAR_INTERFACE_IMPL(CCollectionUtils::_Iterable, IIterable);

ECode CCollectionUtils::_Iterable::GetIterator(
    /* [out] */ IIterator** it)
{
    VALIDATE_NOT_NULL(it)

    AutoPtr<IIterator> res = (IIterator*) new _Iterator(mIter, mTrim);
    *it = res;
    INTERFACE_ADDREF(*it)
    return NOERROR;
}

//===============================================================================
// CCollectionUtils
//===============================================================================

ECode CCollectionUtils::DereferenceIterable(
    /* [in] */ IIterable* iterable,
    /* [in] */ Boolean trim,
    /* [out] */ IIterable** outiter)
{
    return _DereferenceIterable(iterable, trim, outiter);
}

ECode CCollectionUtils::RemoveDuplicates(
    /* [in] */ IList* list,
    /* [in] */ IComparator* comparator)
{
    return _RemoveDuplicates(list, comparator);
}

ECode CCollectionUtils::_DereferenceIterable(
    /* [in] */ IIterable* iterable,
    /* [in] */ Boolean trim,
    /* [out] */ IIterable** outiter)
{
    VALIDATE_NOT_NULL(outiter)

    AutoPtr<IIterable> res = (IIterable*) new _Iterable(iterable, trim);
    *outiter = res;
    INTERFACE_ADDREF(*outiter)
    return NOERROR;
}

ECode CCollectionUtils::_RemoveDuplicates(
    /* [in] */ IList* list,
    /* [in] */ IComparator* comparator)
{
    FAIL_RETURN(CCollections::_SortEx(list, comparator));
    Int32 j = 1;
    Int32 listsize = 0;
    list->GetSize(&listsize);
    for (Int32 i = 1; i < listsize; i++) {
        Int32 comvalue = 0;
        AutoPtr<IInterface> joutface;
        list->Get(j - 1, (IInterface**)&joutface);
        AutoPtr<IInterface> ioutface;
        list->Get(i, (IInterface**)&ioutface);
        if (comparator->Compare(joutface, ioutface, &comvalue), comvalue != 0) {
            AutoPtr<IInterface> object;
            list->Get(i, (IInterface**)&object);
            list->Set(j++, object, (IInterface**)&ioutface);
        }
    }
    list->GetSize(&listsize);
    if (j < listsize) {
        AutoPtr<IList> outlist;
        list->SubList(j, listsize, (IList**)&outlist);
        outlist->Clear();
    }
    return NOERROR;
}

} // namespace Utility
} // namespace Libcore

