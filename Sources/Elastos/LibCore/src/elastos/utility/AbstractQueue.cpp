#include "AbstractQueue.h"

namespace Elastos {
namespace Utility {

AbstractQueue::AbstractQueue()
{}

ECode AbstractQueue::Add(
    /* [in] */ IInterface* obj,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;

    Boolean r;
    if (FAILED(Offer(obj, &r))) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    *result = TRUE;
    return NOERROR;
}

ECode AbstractQueue::Remove(
    /* [out] */ IInterface** obj)
{
    VALIDATE_NOT_NULL(obj);
    *obj = NULL;

    AutoPtr<IInterface> x;
    Poll((IInterface**)&x);
    if (x == NULL) {
        return E_NO_SUCH_ELEMENT_EXCEPTION;
    }

    *obj = x;
    INTERFACE_ADDREF(*obj);
    return NOERROR;
}

ECode AbstractQueue::Element(
    /* [out] */ IInterface** obj)
{
    VALIDATE_NOT_NULL(obj);
    *obj = NULL;

    AutoPtr<IInterface> x;
    Peek((IInterface**)&x);
    if (x == NULL) {
        return E_NO_SUCH_ELEMENT_EXCEPTION;
    }

    *obj = x;
    INTERFACE_ADDREF(*obj);
    return NOERROR;
}

ECode AbstractQueue::Clear()
{
    AutoPtr<IInterface> x;
    while (Poll((IInterface**)&x), x != NULL) {
        x = NULL;
    }
    return NOERROR;
}

ECode AbstractQueue::AddAll(
    /* [in] */ ICollection* c,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;

    VALIDATE_NOT_NULL(c);

    if (c == this->Probe(EIID_ICollection)) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Boolean modified = FALSE;
    AutoPtr<IIterator> it;
    c->GetIterator((IIterator**)&it);
    Boolean hasNext, added;
    while (it->HasNext(&hasNext), hasNext) {
        AutoPtr<IInterface> e;
        it->Next((IInterface**)&e);
        if (Add(e, &added), added) {
            modified = TRUE;
        }
    }

    *result = modified;
    return NOERROR;
}

} // Utility
} // Elastos
