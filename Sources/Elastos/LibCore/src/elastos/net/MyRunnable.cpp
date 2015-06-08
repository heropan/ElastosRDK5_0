
#include "MyRunnable.h"
#include <elastos/Thread.h>
#include <stdio.h>

using Elastos::Core::EIID_IRunnable;

CAR_INTERFACE_IMPL(MyRunnable, Object, IRunnable)

MyRunnable::MyRunnable()
    : mStop(FALSE)
{
}

UInt32 MyRunnable::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 MyRunnable::Release()
{
    return ElRefBase::Release();
}

PInterface MyRunnable::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IRunnable) {
        return (IRunnable*)this;
    }

    return NULL;
}

ECode MyRunnable::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (pIID == NULL) {
        return E_INVALID_ARGUMENT;
    }

    if (pObject == (IInterface*)(IRunnable*)this) {
        *pIID = EIID_IRunnable;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

ECode MyRunnable::Run()
{

    return NOERROR;
}

Mutex* MyRunnable::GetSelfLock()
{
    return &mLock;
}
