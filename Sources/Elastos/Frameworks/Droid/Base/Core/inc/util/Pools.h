
#ifndef __ELASTOS_DROID_UTILITY_POOLS_H__
#define __ELASTOS_DROID_UTILITY_POOLS_H__

#ifdef DROID_CORE
#include "Elastos.Droid.Core_server.h"
#else
#include "Elastos.Droid.Core.h"
#endif

namespace Elastos {
namespace Droid {
namespace Utility {

class Pools
{
private:
    Pools() {}

public:
    static CARAPI_(AutoPtr<IPool>) AcquireSimplePool(
        /* [in] */ IPoolableManager* manager);

    static CARAPI_(AutoPtr<IPool>) AcquireFinitePool(
        /* [in] */ IPoolableManager* manager,
        /* [in] */ Int32 limit);

    static CARAPI_(AutoPtr<IPool>) AcquireSynchronizedPool(
        /* [in] */ IPool* pool);

    static CARAPI_(AutoPtr<IPool>) AcquireSynchronizedPool(
        /* [in] */ IPool* pool,
        /* [in] */ Object& lock);
};

} // namespace Utility
} // namepsace Droid
} // namespace Elastos

#endif//__ELASTOS_DROID_UTILITY_POOLS_H__
