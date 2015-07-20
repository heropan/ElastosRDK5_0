
#ifndef __SYNCHRONIZEDPOOL_H__
#define __SYNCHRONIZEDPOOL_H__

#include "Elastos.Droid.Core_server.h"

namespace Elastos {
namespace Droid {
namespace Utility {

class SynchronizedPool : public ElRefBase, public IPool
{
public:
    SynchronizedPool(
        /* [in] */ IPool* pool);

    SynchronizedPool(
        /* [in] */ IPool* pool,
        /* [in] */ Object& lock);

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
    AutoPtr<IPool> mPool;
    Object mLock;
    Object& mLockRef;
};

} // namespace Utility
} // namepsace Droid
} // namespace Elastos

#endif //__SYNCHRONIZEDPOOL_H__
