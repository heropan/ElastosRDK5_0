
#ifndef __ELASTOS_CORE_SINGLETON_H__
#define __ELASTOS_CORE_SINGLETON_H__

#include "Object.h"
#include <spinlock.h>

namespace Elastos {
namespace Core {

class Singleton : public Object
{
public:
    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI_(void) OnLastStrongRef(
        /* [in] */ const void* id);

public:
    static Singleton* sInstance;
    static _SingletonObjState_ sInstanceState;
    static SpinLock sLock;
};

} // namespace Core
} // namespace Elastos


#endif //__ELASTOS_CORE_SINGLETON_H__
