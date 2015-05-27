
#ifndef __ELASTOS_CORE_SINGLETON_H__
#define __ELASTOS_CORE_SINGLETON_H__

#include <elastos/core/Object.h>
#include <spinlock.h>

using Elastos::Core::Object;

namespace Elastos {
namespace Core {

class Singleton : public Object
{
public:
    Singleton();

    virtual ~Singleton();

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    virtual CARAPI_(void) OnLastStrongRef(
        /* [in] */ const void* id) = 0;

protected:
    virtual CARAPI_(SpinLock&) GetSelfSpinLock() = 0;
};

} // namespace Core
} // namespace Elastos


#endif //__ELASTOS_CORE_SINGLETON_H__
