
#include <Elastos.CoreLibrary.Utility.h>
#include <Elastos.CoreLibrary.Net.h>
#include "elastos/droid/net/dhcp/DhcpNakPacket.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Dhcp {

DhcpNakPacket::DhcpNakPacket()
{}

ECode DhcpNakPacket::constructor(
    /* [in] */ Int32 transId,
    /* [in] */ IInetAddress* clientIp,
    /* [in] */ IInetAddress* yourIp,
    /* [in] */ IInetAddress* nextIp,
    /* [in] */ IInetAddress* relayIp,
    /* [in] */ ArrayOf<Byte>* clientMac)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        super(transId, Inet4Address.ANY, Inet4Address.ANY, nextIp, relayIp,
            clientMac, FALSE);
#endif
}

ECode DhcpNakPacket::ToString(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        String s = super.toString();
        return s + " NAK, reason " + (mMessage == NULL ? "(none)" : mMessage);
#endif
}

ECode DhcpNakPacket::BuildPacket(
    /* [in] */ Int32 encap,
    /* [in] */ Int16 destUdp,
    /* [in] */ Int16 srcUdp,
    /* [out] */ IByteBuffer** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        ByteBuffer result = ByteBuffer.allocate(MAX_LENGTH);
        InetAddress destIp = mClientIp;
        InetAddress srcIp = mYourIp;
        fillInPacket(encap, destIp, srcIp, destUdp, srcUdp, result,
            DHCP_BOOTREPLY, mBroadcast);
        result.flip();
        return result;
#endif
}

ECode DhcpNakPacket::FinishPacket(
    /* [in] */ IByteBuffer* buffer)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        addTlv(buffer, DHCP_MESSAGE_TYPE, DHCP_MESSAGE_TYPE_NAK);
        addTlv(buffer, DHCP_SERVER_IDENTIFIER, mServerIdentifier);
        addTlv(buffer, DHCP_MESSAGE, mMessage);
        addTlvEnd(buffer);
#endif
}

ECode DhcpNakPacket::DoNextOp(
    /* [in] */ DhcpStateMachine* machine)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        machine.onNakReceived();
#endif
}


} // namespace Dhcp
} // namespace Net
} // namespace Droid
} // namespace Elastos
