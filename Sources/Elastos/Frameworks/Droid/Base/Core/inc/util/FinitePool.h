
#ifndef __ELASTOS_DROID_UTILITY_FINITEPOOL_H__
#define __ELASTOS_DROID_UTILITY_FINITEPOOL_H__

#include "Elastos.Droid.Core_server.h"

namespace Elastos {
namespace Droid {
namespace Utility {

class FinitePool : public ElRefBase, public IPool
{
public:
    FinitePool(
        /* [in] */ IPoolableManager* manager);

    FinitePool(
        /* [in] */ IPoolableManager* manager,
        /* [in] */ Int32 limit);

    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    CARAPI Acquire(
        /* [out] */ IPoolable** element);

    CARAPI ReleaseElement(
        /* [in] */ IPoolable* element);

private:
    static const char* TAG;
    /**
     * Factory used to create new pool objects
     */
    AutoPtr<IPoolableManager> mManager;
    /**
     * Maximum number of objects in the pool
     */
    Int32 mLimit;
    /**
     * If true, mLimit is ignored
     */
    Boolean mInfinite;

    /**
     * Next object to acquire
     */
    AutoPtr<IPoolable> mRoot;
    /**
     * Number of objects in the pool
     */
    Int32 mPoolCount;
};

} // namespace Utility
} // namepsace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_UTILITY_FINITEPOOL_H__
