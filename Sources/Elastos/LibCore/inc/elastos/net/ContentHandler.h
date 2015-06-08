
#ifndef __ELASTOS_NET_CONTENTHANDLER_H__
#define __ELASTOS_NET_CONTENTHANDLER_H__

#include "Elastos.CoreLibrary_server.h"
#include <elastos/core/Object.h>

using Elastos::Core::Object;

namespace Elastos {
namespace Net {

extern "C"  const InterfaceID EIID_ContentHandler;

class ContentHandler : public Object
{
public:
    CARAPI GetClassID(
        /* [out] */ ClassID* clsid);

    CARAPI ToString(
        /* [out] */ String* result);

    virtual CARAPI GetContent(
        /* [in] */ IURLConnection* uConn,
        /* [out] */ IInterface** obj) = 0;

    virtual CARAPI GetContent(
        /* [in] */ IURLConnection* uConn,
        /* [in] */ const ArrayOf<InterfaceID>& types,
        /* [out] */ IInterface** obj);
};

} // namespace Net
} // namespace Elastos

#endif //__ELASTOS_NET_CONTENTHANDLER_H__
