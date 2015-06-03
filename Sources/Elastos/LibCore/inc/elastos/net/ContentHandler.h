
#ifndef __CONTENTHANDLER_H__
#define __CONTENTHANDLER_H__

#include "Elastos.CoreLibrary_server.h"

namespace Elastos {
namespace Net {

class ContentHandler
{
public:
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

#endif //__CONTENTHANDLER_H__
