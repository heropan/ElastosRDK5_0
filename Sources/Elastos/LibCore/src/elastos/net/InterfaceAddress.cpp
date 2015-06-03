
#include "InterfaceAddress.h"
#include "CInet4Address.h"
#include <elastos/core/StringUtils.h>

using Elastos::Core::StringUtils;

namespace Elastos {
namespace Net {

InterfaceAddress::InterfaceAddress(
    /* [in] */ IInetAddress* address,
    /* [in] */ IInetAddress* broadcastAddress,
    /* [in] */ IInetAddress* mask)
{
    mAddress = address;
    mBroadcastAddress = broadcastAddress;
    mPrefixLength = CountPrefixLength(mask);
}

InterfaceAddress::InterfaceAddress(
    /* [in] */ IInetAddress* address,
    /* [in] */ Int16 prefixLength)
{
    mAddress = address;
    mBroadcastAddress = NULL;
    mPrefixLength = prefixLength;
}

PInterface InterfaceAddress::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IInterfaceAddress*)this;
    }
    else if (riid == EIID_IInterfaceAddress) {
        return (IInterfaceAddress*)this;
    }

    return NULL;
}

UInt32 InterfaceAddress::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 InterfaceAddress::Release()
{
    return ElRefBase::Release();
}

ECode InterfaceAddress::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

Int16 InterfaceAddress::CountPrefixLength(
    /* [in] */ IInetAddress* mask)
{
    Int16 count = 0;
    ArrayOf<Byte>* maskBytes = ((InetAddress*)mask->Probe(EIID_InetAddress))->mIpAddress;
    for (Int32 i = 0; i < maskBytes->GetLength(); ++i) {
        Byte b = (*maskBytes)[i];
        for (Int32 j = 0; j < 8; ++j) {
            if ((b & (1 << j)) != 0) {
                ++count;
            }
        }
    }

    return count;
}

ECode InterfaceAddress::GetAddress(
    /* [out] */ IInetAddress** address)
{
    VALIDATE_NOT_NULL(address);
    *address = mAddress;
    REFCOUNT_ADD(*address);
    return NOERROR;
}

ECode InterfaceAddress::GetBroadcast(
    /* [out] */ IInetAddress** broadcast)
{
    VALIDATE_NOT_NULL(broadcast);
    *broadcast = mBroadcastAddress;
    REFCOUNT_ADD(*broadcast);
    return NOERROR;
}

ECode InterfaceAddress::GetNetworkPrefixLength(
    /* [out] */ Int16* length)
{
    VALIDATE_NOT_NULL(length);
    *length = mPrefixLength;
    return NOERROR;
}

ECode InterfaceAddress::Equals(
    /* [in] */ IInterface* obj,
    /* [out] */ Boolean* equals)
{
    VALIDATE_NOT_NULL(equals)

    if(obj == this)
    {
        *equals = FALSE;
        return NOERROR;
    }
    if(obj->Probe(EIID_IInterfaceAddress) == NULL)
    {
        *equals = FALSE;
        return NOERROR;
    }
    AutoPtr<InterfaceAddress> rhs =(InterfaceAddress*)(obj->Probe(EIID_IInterfaceAddress));
    Boolean addrEquals;
    Boolean broadcastEquals;
    if(mAddress != NULL){
        mAddress->Equals(rhs->mAddress, &addrEquals);
    }
    if(mBroadcastAddress != NULL){
        mBroadcastAddress->Equals(rhs->mBroadcastAddress, &broadcastEquals);
    }
    *equals = ((mAddress == NULL) ? rhs->mAddress == NULL : addrEquals) &&
    (rhs->mPrefixLength == mPrefixLength) &&
    ((mBroadcastAddress == NULL) ? rhs->mBroadcastAddress == NULL : broadcastEquals);
    return NOERROR;
}

/**
 * Returns a string containing this interface's address, prefix length, and broadcast address.
 * For example: {@code "/172.18.103.112/23 [/172.18.103.255]"} or
 * {@code "/0:0:0:0:0:0:0:1%1/128 [null]"}.
 */

ECode InterfaceAddress::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)

    String addr;
    String broadcast;
    mAddress->ToString(&addr);
    mBroadcastAddress->ToString(&broadcast);
    *str = addr + String("/") + StringUtils::Int32ToString(mPrefixLength) +
            String("[") + broadcast + String("]");
    return NOERROR;
}

} // namespace Net
} // namespace Elastos
