
#include "Thread.h"
#include "Object.h"

namespace Elastos {
namespace Core {
namespace Threading {

Object::Object()
    : mNativeObject(NativeCreateObject())
{}

Object::~Object()
{
    NativeDestroyObject(mNativeObject);
}

ECode Object::Lock()
{
    // the following codes should be remove
    AutoPtr<IThread> t;
    Thread::Attach((IThread**)&t);
    return NativeLockObject(mNativeObject);
}

ECode Object::Unlock()
{
    // the following codes should be remove
    AutoPtr<IThread> t;
    Thread::Attach((IThread**)&t);
    return NativeUnlockObject(mNativeObject);
}

ECode Object::Notify()
{
    return NativeObjectNotify(mNativeObject);
}

ECode Object::NotifyAll()
{
    return NativeObjectNotifyAll(mNativeObject);
}

ECode Object::Wait()
{
    return Wait(0, 0);
}

ECode Object::Wait(
    /* [in] */ Int64 millis)
{
    return Wait(millis, 0);
}

ECode Object::Wait(
    /* [in] */ Int64 millis,
    /* [in] */ Int32 nanos)
{
    return NativeObjectWait(mNativeObject, millis, nanos, TRUE);
}

UInt32 LockObject::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 LockObject::Release()
{
    return ElRefBase::Release();
}

PInterface LockObject::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }

    return NULL;
}

ECode LockObject::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    assert(0);
    return  NOERROR;
}

} // namespace Threading
} // namespace Core
} // namespace Elastos
