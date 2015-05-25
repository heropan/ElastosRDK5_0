
#include "CRawSocket.h"

namespace Elastos {
namespace Net {

ECode CRawSocket::constructor(
    /* [in] */ const String& interfaceName,
    /* [in] */ Int16 protocolType)
{
    return RawSocket::Init(interfaceName, protocolType);
}

ECode CRawSocket::Read(
    /* [in] */ ArrayOf<Byte>* packet,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 byteCount,
    /* [in] */ Int32 destPort,
    /* [in] */ Int32 timeoutMillis,
    /* [out] */ Int32* number)
{
    return RawSocket::Read(packet, offset, byteCount, destPort, timeoutMillis, number);
}

ECode CRawSocket::Write(
    /* [in] */ ArrayOf<Byte>* destMac,
    /* [in] */ ArrayOf<Byte>* packet,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 byteCount,
    /* [out] */ Int32* number)
{
    return RawSocket::Write(destMac, packet, offset, byteCount, number);
}

ECode CRawSocket::Close()
{
    return RawSocket::Close();
}

} // namespace Net
} // namespace Elastos
