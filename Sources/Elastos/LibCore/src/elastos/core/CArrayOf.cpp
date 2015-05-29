
#include "CArrayOf.h"

namespace Elastos {
namespace Core {

CArrayOf::CArrayOf()
    : mElements(NULL)
{}

CArrayOf::~CArrayOf()
{
}

ECode CArrayOf::constructor(
    /* [in] */ InterfaceID riid,
    /* [in] */ Int32 size)
{
    if (size < 0) return E_ILLEGAL_ARGUMENT_EXCEPTION;
    mElements = ArrayOf<IInterface *>::Alloc(size);
    mTypeId = riid;
    return NOERROR;
}

ECode CArrayOf::GetLength(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);

    *size = mElements == NULL ? -1 : mElements->GetLength();
    return NOERROR;
}

ECode CArrayOf::Get(
    /* [in] */ Int32 index,
    /* [out] */ IInterface** element)
{
    VALIDATE_NOT_NULL(element);

    if (index < 0 || mElements == NULL || index >= mElements->GetLength()) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    *element = (*mElements)[index];
    REFCOUNT_ADD(*element);
    return NOERROR;
}

ECode CArrayOf::Put(
    /* [in] */ Int32 index,
    /* [in] */ IInterface* element)
{
    if (index < 0 || mElements == NULL || index >= mElements->GetLength()
        || (element != NULL && element->Probe(mTypeId) == NULL)) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    mElements->Set(index, element);
    return NOERROR;
}

ECode CArrayOf::GetTypeId(
    /* [out] */ InterfaceID* id)
{
    VALIDATE_NOT_NULL(id);
    *id = mTypeId;
    return NOERROR;
}

} // namespace Core
} // namespace Elastos
