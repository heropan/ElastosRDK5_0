
#include "util/FinitePool.h"
#include "ext/frameworkdef.h"
#include <elastos/Logger.h>

using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Utility {

const char* FinitePool::TAG = "FinitePool";

FinitePool::FinitePool(
    /* [in] */ IPoolableManager* manager)
    : mManager(manager)
    , mLimit(0)
    , mInfinite(TRUE)
{
}

FinitePool::FinitePool(
    /* [in] */ IPoolableManager* manager,
    /* [in] */ Int32 limit)
    : mManager(manager)
    , mLimit(limit)
    , mInfinite(FALSE)
{
    if (limit <= 0) {
        Logger::E("FinitePool", "The pool limit must be > 0");
        assert(0);
    }
}

PInterface FinitePool::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IPool) {
        return (IPool*)this;
    }

    return NULL;
}

UInt32 FinitePool::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 FinitePool::Release()
{
    return ElRefBase::Release();
}

ECode FinitePool::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [out] */ InterfaceID* pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(IPool*)this) {
        *pIID = EIID_IPool;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ECode FinitePool::Acquire(
    /* [out] */ IPoolable** element)
{
    VALIDATE_NOT_NULL(element);

    if (mRoot != NULL) {
        *element = mRoot;
        INTERFACE_ADDREF(*element);

        mRoot = NULL;
        (*element)->GetNextPoolable((IPoolable**)&mRoot);
        mPoolCount--;
    }
    else {
        FAIL_RETURN(mManager->NewInstance(element));
    }

    if (*element != NULL) {
        (*element)->SetNextPoolable(NULL);
        (*element)->SetPooled(FALSE);
        mManager->OnAcquired(*element);
    }

    return NOERROR;
}

ECode FinitePool::ReleaseElement(
    /* [in] */ IPoolable* element)
{
    Boolean isPooled;
    element->IsPooled(&isPooled);
    if (!isPooled) {
        if (mInfinite || mPoolCount < mLimit) {
            mPoolCount++;
            element->SetNextPoolable(mRoot);
            element->SetPooled(TRUE);
            mRoot = element;
        }
        mManager->OnReleased(element);
    } else {
        //Log.w(TAG, "Element is already in pool: " + element);
    }
    return NOERROR;
}

} // namespace Utility
} // namepsace Droid
} // namespace Elastos
