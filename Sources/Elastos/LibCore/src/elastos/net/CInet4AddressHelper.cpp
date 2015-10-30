
#include "elastos/net/CInet4AddressHelper.h"
#include "elastos/net/CInet4Address.h"

namespace Elastos {
namespace Net {

CAR_INTERFACE_IMPL(CInet4AddressHelper, Singleton, IInet4AddressHelper)

CAR_SINGLETON_IMPL(CInet4AddressHelper)

ECode CInet4AddressHelper::GetANY(
    /* [out] */ IInetAddress** result)
{
    return CInet4Address::GetANY(result);
}

ECode CInet4AddressHelper::GetALL(
    /* [out] */ IInetAddress** result)
{
    return CInet4Address::GetALL(result);
}

ECode CInet4AddressHelper::GetLOOPBACK(
    /* [out] */ IInetAddress** result)
{
    return CInet4Address::GetLOOPBACK(result);
}


} // namespace Net
} // namespace Elastos
