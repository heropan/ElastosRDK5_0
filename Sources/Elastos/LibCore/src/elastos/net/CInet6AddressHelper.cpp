
#include "CInet6AddressHelper.h"
#include "CInet6Address.h"
#include "NetworkInterface.h"

namespace Elastos {
namespace Net {

ECode CInet6AddressHelper::GetByAddress(
    /* [in] */ const String& host,
    /* [in] */ ArrayOf<Byte>* addr,
    /* [in] */ Int32 scope_id,
    /* [out] */ IInet6Address** address)
{
    VALIDATE_NOT_NULL(address);
    return CInet6Address::GetByAddress(host, addr, scope_id, address);
}

ECode CInet6AddressHelper::GetByAddressEx(
    /* [in] */ const String& host,
    /* [in] */ ArrayOf<Byte>* addr,
    /* [in] */ INetworkInterface* nif,
    /* [out] */ IInet6Address** address)
{
    VALIDATE_NOT_NULL(address);
    return CInet6Address::GetByAddress(host, addr, nif, address);
}

ECode CInet6AddressHelper::GetANY(
    /* [out] */ IInetAddress** result)
{
    return CInet6Address::GetANY(result);
}

ECode CInet6AddressHelper::GetLOOPBACK(
    /* [out] */ IInetAddress** result)
{
    return CInet6Address::GetLOOPBACK(result);
}

} // namespace Net
} // namespace Elastos
