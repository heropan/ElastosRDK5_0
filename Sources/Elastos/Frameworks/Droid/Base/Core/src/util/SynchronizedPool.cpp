
#include "util/SynchronizedPool.h"
#include "ext/frameworkdef.h"

namespace Elastos {
namespace Droid {
namespace Utility {

SynchronizedPool::SynchronizedPool(
    /* [in] */IPool* pool)
    : mPool(pool)
    , mLockRef(mLock)
{
}

SynchronizedPool::SynchronizedPool(
    /* [in] */ IPool* pool,
    /* [in] */ Mutex& lock)
    : mPool(pool)
    , mLockRef(lock)
{
}

PInterface SynchronizedPool::Probe(
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

UInt32 SynchronizedPool::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 SynchronizedPool::Release()
{
    return ElRefBase::Release();
}

ECode SynchronizedPool::GetInterfaceID(
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

ECode SynchronizedPool::Acquire(
    /* [out] */ IPoolable** element)
{
    VALIDATE_NOT_NULL(element);

    Mutex::Autolock lock(mLockRef);

    return mPool->Acquire(element);
}

ECode SynchronizedPool::ReleaseElement(
    /* [in] */ IPoolable* element)
{
    Mutex::Autolock lock(mLockRef);

    return mPool->ReleaseElement(element);
}

} // namespace Utility
} // namepsace Droid
} // namespace Elastos
