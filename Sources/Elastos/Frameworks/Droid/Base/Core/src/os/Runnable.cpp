
#include "os/Runnable.h"
#include "ext/frameworkdef.h"

using Elastos::Core::EIID_IRunnable;

namespace Elastos {
namespace Droid {
namespace Os {

PInterface Runnable::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IRunnable) {
        return (IRunnable*)this;
    }

    return NULL;
}

UInt32 Runnable::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 Runnable::Release()
{
    return ElRefBase::Release();
}

ECode Runnable::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [out] */ InterfaceID* pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(IRunnable*)this) {
        *pIID = EIID_IRunnable;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

} // namespace Os
} // namespace Droid
} // namespace Elastos
