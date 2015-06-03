
#ifndef __DEFAULTFILENAMEMAP_H__
#define __DEFAULTFILENAMEMAP_H__

#include "Elastos.CoreLibrary_server.h"


namespace Elastos {
namespace Net {

class DefaultFileNameMap
    : public ElRefBase
    , public IFileNameMap
{
public:
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

#endif //__DEFAULTFILENAMEMAP_H__
