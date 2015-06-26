#include "UnsafeArrayList.h"

namespace Elastos {
namespace Utility {

CAR_INTERFACE_IMPL(UnsafeArrayList, AbstractList, IUnsafeArrayList)

UnsafeArrayList::UnsafeArrayList()
    : mElementType(EIID_IInterface)
    , mArray(NULL)
    , mSize(0)
{}

UnsafeArrayList::UnsafeArrayList(
    /* [in] */ const InterfaceID& elementType,
    /* [in] */ Int32 initialCapacity)
    : mElementType(elementType)
    , mArray(ArrayOf<IInterface*>::Alloc(initialCapacity))
    , mSize(0)
{
}

ECode UnsafeArrayList::constructor()
{
    AbstractList::constructor();
    mElementType = EIID_IInterface;
    mArray = NULL;
    mSize = 0;
    return NOERROR;
}

ECode UnsafeArrayList::constructor(
    /* [in] */ const InterfaceID& elementType,
    /* [in] */ Int32 initialCapacity)
{
    AbstractList::constructor();
    mElementType = elementType;
    mArray = ArrayOf<IInterface*>::Alloc(initialCapacity);
    mSize = 0;
    return NOERROR;
}

ECode UnsafeArrayList::Add(
    /* [in] */ IInterface* element,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res)
    if (mSize == mArray->GetLength()) {
        AutoPtr< ArrayOf <IInterface*> > newArray = ArrayOf <IInterface*>::Alloc(mSize * 2);
        for(Int32 i = 0; i < mArray->GetLength(); i++) {
            newArray->Set(i, (*mArray)[i]);
        }
        mArray = newArray;
    }
    (*mArray)[mSize++] = element;
    ++mModCount;
    *res = TRUE;
    return NOERROR;
}

ECode UnsafeArrayList::GetArrays(
    /* [out, callee] */ ArrayOf<IInterface*> ** array)
{
    VALIDATE_NOT_NULL(array)

    *array = mArray;
    REFCOUNT_ADD(*array)
    return NOERROR;
}

ECode UnsafeArrayList::Get(
    /* [in] */ Int32 position,
    /* [out] */ IInterface** element)
{
    VALIDATE_NOT_NULL(element)

    *element = (*mArray)[position];
    REFCOUNT_ADD(*element)
    return NOERROR;
}

ECode UnsafeArrayList::GetSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size)

    *size = mSize;
    return NOERROR;
}

ECode UnsafeArrayList::Set(
    /* [in] */ Int32 location,
    /* [in] */ IInterface* object,
    /* [out] */ IInterface** prevObject)
{
    VALIDATE_NOT_NULL(prevObject)

    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

} // namespace Utility
} // namespace Elastos