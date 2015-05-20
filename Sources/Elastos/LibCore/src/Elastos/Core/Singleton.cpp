
#include "Singleton.h"

namespace Elastos {
namespace Core {

Singleton* Singleton::sInstance = NULL;
_SingletonObjState_ Singleton::sInstanceState = _SingletonObjState_Uninitialize;
SpinLock Singleton::sLock;

UInt32 Singleton::AddRef()
{
    sLock.Lock();
    Int32 ref = Object::AddRef();
    sLock.Unlock();
    return (UInt32)ref;
}

UInt32 Singleton::Release()
{
    sLock.Lock();
    Int32 ref = Object::Release();
    if (ref > 0) {
        sLock.Unlock();
    }
    return (UInt32)ref;
}

void Singleton::OnLastStrongRef(
    /* [in] */ const void* id)
{
    sInstanceState = _SingletonObjState_Uninitialize;
    sInstance = NULL;
    sLock.Unlock();
}

} // namespace Core
} // namespace Elastos
