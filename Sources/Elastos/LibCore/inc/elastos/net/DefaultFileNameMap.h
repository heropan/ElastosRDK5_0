
#ifndef __ELASTOS_NET_DEFAULTFILENAMEMAP_H__
#define __ELASTOS_NET_DEFAULTFILENAMEMAP_H__

#include "Elastos.CoreLibrary_server.h"
#include <elastos/core/Object.h>

using Elastos::Core::Object;

namespace Elastos {
namespace Net {

class DefaultFileNameMap
    : public Object
    , public IFileNameMap
{
public:
    CAR_INTERFACE_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    CARAPI GetContentTypeFor(
        /* [in] */ const String& filename,
        /* [out] */ String* type);
};

} // namespace Net
} // namespace Elastos

#endif //__ELASTOS_NET_DEFAULTFILENAMEMAP_H__
