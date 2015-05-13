
#ifndef __ELASTOS_DROID_OS_RUNNABLE_H__
#define __ELASTOS_DROID_OS_RUNNABLE_H__

#ifdef DROID_CORE
#include "Elastos.Droid.Core_server.h"
#else
#include "Elastos.Droid.Core.h"
#endif


using Elastos::Core::IRunnable;

namespace Elastos {
namespace Droid {
namespace Os {

class Runnable
    : public ElRefBase
    , public IRunnable
{
protected:
    Runnable() {}

    virtual ~Runnable() {}

public:
    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);
};

} // namespace Os
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_OS_RUNNABLE_H__
