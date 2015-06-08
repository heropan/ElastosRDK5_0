
#include "CInet6AddressHelper.h"
#include "CInet6Address.h"
#include "NetworkInterface.h"

namespace Elastos {
namespace Net {

CAR_INTERFACE_IMPL(CInet6AddressHelper, Singleton, IInet6AddressHelper)

CAR_SINGLETON_IMPL(CInet6AddressHelper)

ECode CInet6AddressHelper::GetByAddress(
    /* [in] */ const String& host,
    /* [in] */ ArrayOf<Byte>* addr,
    /* [in] */ Int32 scope_id,
    /* [out] */ IInet6Address** address)
{
    VALIDATE_NOT_NULL(address);
    return CInet6Address::GetByAddress(host, addr, scope_id, address);
}

ECode CInet6AddressHelper::GetByAddress(
    /* [in] */ const String& host,
    /* [in] */ ArrayOf<Byte>* addr,
    /* [in] */ INetworkInterface* nif,
    /* [out] */ IInet6Address** address)
{
    VALIDATE_NOT_NULL(address);
    return CInet6Address::GetByAddress(host, addr, nif, address);
}

} // namespace Net
} // namespace Elastos
