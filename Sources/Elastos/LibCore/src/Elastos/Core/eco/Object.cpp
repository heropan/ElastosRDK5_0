
#include "Thread.h"
#include "Object.h"

namespace Elastos {
namespace Core {

Object::Object()
    : mNativeObject(NativeCreateObject())
{}

Object::~Object()
{
    NativeDestroyObject(mNativeObject);
}

CAR_INTERFACE_IMPL_3(Object, IObject, ISynchronize, IWeakReferenceSource)

ECode Object::Aggregate(
    /* [in] */ Int32 type,
    /* [in] */ IInterface* object)
{
    return E_NOT_IMPLEMENTED;
}

ECode Object::GetDomain(
    /* [out] */ IInterface** object)
{
    return E_NOT_IMPLEMENTED;
}

ECode Object::GetClassID(
    /* [out] */ ClassID* clsid)
{
    return E_NOT_IMPLEMENTED;
}

ECode Object::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode);
    *hashCode = (Int32)this;
    return NOERROR;
}

ECode Object::Equals(
    /* [in] */ IInterface* other,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = THIS_PROBE(IObject) == IObject::Probe(other);
    return NOERROR;
}

ECode Object::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);
    info->AppendFormat("Object[0x%08x]", this);
    return NOERROR;
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
    return WaitEx2(0, 0);
}

ECode Object::WaitEx(
    /* [in] */ Int64 millis)
{
    return WaitEx2(millis, 0);
}

ECode Object::WaitEx2(
    /* [in] */ Int64 millis,
    /* [in] */ Int32 nanos)
{
    return NativeObjectWait(mNativeObject, millis, nanos, TRUE);
}

ECode Object::GetWeakReference(
    /* [out] */ IWeakReference** weakReference)
{
    VALIDATE_NOT_NULL(weakReference);
    *weakReference = new WeakReferenceImpl(THIS_PROBE(IInterface), CreateWeak(this));
    (*weakReference)->AddRef();
    return NOERROR;
}

} // namespace Core
} // namespace Elastos
