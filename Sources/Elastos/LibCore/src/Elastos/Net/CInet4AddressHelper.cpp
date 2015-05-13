
#include "CInet4AddressHelper.h"
#include "CInet4Address.h"

namespace Elastos {
namespace Net {

ECode CInet4AddressHelper::GetANY(
    /* [out] */ IInetAddress** result)
{
    return CInet4Address::GetANY(result);
}

ECode CInet4AddressHelper::GetLOOPBACK(
    /* [out] */ IInetAddress** result)
{
    return CInet4Address::GetLOOPBACK(result);
}

ECode CInet4AddressHelper::GetALL(
    /* [out] */ IInetAddress** result)
{
    return CInet4Address::GetALL(result);
}

} // namespace Net
} // namespace Elastos
