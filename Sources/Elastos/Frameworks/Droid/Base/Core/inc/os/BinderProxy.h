#ifndef __BINDERPROXY_H__
#define __BINDERPROXY_H__

#include "Elastos.Droid.Core_server.h"

namespace Elastos {
namespace Droid {
namespace Os {

extern const InterfaceID EIID_BinderProxy;

class BinderProxy
    : public ElRefBase
    , public IBinder
{
public:
    BinderProxy();

    ~BinderProxy();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface* pObject,
        /* [in] */ InterfaceID* pIID);

    CARAPI ToString(
        /* [out] */ String* str);

    CARAPI GetHashCode(
        /* [out] */ Int32* hash);
private:
    CARAPI_(void) Destroy();

public:
    Int32 mObject;
};

} // namespace Os
} // namespace Droid
} // namespace Elastos

#endif //__BINDERPROXY_H__
