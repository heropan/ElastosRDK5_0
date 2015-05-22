
#ifndef __INTERFACEADDRESS_H__
#define __INTERFACEADDRESS_H__

#include "Elastos.CoreLibrary_server.h"
#include <elautoptr.h>
#include <elrefbase.h>

namespace Elastos {
namespace Net {

class InterfaceAddress
    : public ElRefBase
    , public IInterfaceAddress
{
public:
    InterfaceAddress(
        /* [in] */ IInetAddress* address,
        /* [in] */ IInetAddress* broadcastAddress,
        /* [in] */ IInetAddress* mask);

    InterfaceAddress(
        /* [in] */ IInetAddress* address,
        /* [in] */ Int16 prefixLength);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    CARAPI GetAddress(
        /* [out] */ IInetAddress** address);

    CARAPI GetBroadcast(
        /* [out] */ IInetAddress** broadcast);

    CARAPI GetNetworkPrefixLength(
        /* [out] */ Int16* length);

    CARAPI Equals(
        /* [in] */ IInterface* obj,
        /* [out] */ Boolean* equals);

    CARAPI ToString(
        /* [out] */ String* str);

private:
    static CARAPI_(Int16) CountPrefixLength(
        /* [in] */ IInetAddress* mask);

public:
    Int32 mIndex;

    String mName;

    AutoPtr<IInetAddress> mAddress;

private:
    AutoPtr<IInetAddress> mBroadcastAddress;

    Int16 mPrefixLength;
};

} // namespace Net
} // namespace Elastos

#endif //__INTERFACEADDRESS_H__
