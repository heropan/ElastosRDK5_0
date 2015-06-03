
#include "CInetAddressHelper.h"

namespace Elastos {
namespace Net {

ECode CInetAddressHelper::GetAllByName(
    /* [in] */ const String& host,
    /* [out, callee] */ ArrayOf<IInetAddress*>** interAddress)
{
    return InetAddress::GetAllByName(host, interAddress);
}

ECode CInetAddressHelper::GetByName(
    /* [in] */ const String& host,
    /* [out] */ IInetAddress** interAddr)
{
    return InetAddress::GetByName(host, interAddr);
}

ECode CInetAddressHelper::GetLocalHost(
    /* [out] */ IInetAddress** interAddr)
{
    return InetAddress::GetLocalHost(interAddr);
}

ECode CInetAddressHelper::ClearDnsCache()
{
    return InetAddress::ClearDnsCache();
}

ECode CInetAddressHelper::IsNumeric(
    /* [in] */ const String& address,
    /* [out] */ Boolean* isNumeric)
{
    return InetAddress::IsNumeric(address, isNumeric);
}

ECode CInetAddressHelper::ParseNumericAddress(
    /* [in] */ const String& numericAddress,
    /* [out] */ IInetAddress** interAddr)
{
    return InetAddress::ParseNumericAddress(numericAddress, interAddr);
}

ECode CInetAddressHelper::GetLoopbackAddress(
    /* [out] */ IInetAddress** interAddr)
{
    return InetAddress::GetLoopbackAddress(interAddr);
}

ECode CInetAddressHelper::GetByAddress(
    /* [in] */ ArrayOf<Byte>* ipAddress,
    /* [out] */ IInetAddress** interAddr)
{
    return InetAddress::GetByAddress(ipAddress, interAddr);
}

ECode CInetAddressHelper::GetByAddress(
    /* [in] */ const String& hostName,
    /* [in] */ ArrayOf<Byte>* ipAddress,
    /* [out] */ IInetAddress** interAddr)
{
    return InetAddress::GetByAddress(hostName, ipAddress, interAddr);
}

} // namespace Net
} // namespace Elastos
