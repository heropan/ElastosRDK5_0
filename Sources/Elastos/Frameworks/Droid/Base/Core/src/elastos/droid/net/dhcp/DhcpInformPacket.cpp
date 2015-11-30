
#include "elastos/droid/net/dhcp/DhcpInformPacket.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Dhcp {

DhcpInformPacket::DhcpInformPacket()
{}

ECode DhcpInformPacket::constructor(
    /* [in] */ Int32 transId,
    /* [in] */ IInetAddress* clientIp,
    /* [in] */ IInetAddress* yourIp,
    /* [in] */ IInetAddress* nextIp,
    /* [in] */ IInetAddress* relayIp,
    /* [in] */ ArrayOf<Byte>* clientMac)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        super(transId, clientIp, yourIp, nextIp, relayIp, clientMac, FALSE);
#endif
}

ECode DhcpInformPacket::ToString(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        String s = super.toString();
        return s + " INFORM";
#endif
}

ECode DhcpInformPacket::BuildPacket(
    /* [in] */ Int32 encap,
    /* [in] */ Int16 destUdp,
    /* [in] */ Int16 srcUdp,
    /* [out] */ IByteBuffer** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        ByteBuffer result = ByteBuffer.allocate(MAX_LENGTH);
        fillInPacket(encap, mClientIp, mYourIp, destUdp, srcUdp, result,
            DHCP_BOOTREQUEST, FALSE);
        result.flip();
        return result;
#endif
}

ECode DhcpInformPacket::FinishPacket(
    /* [in] */ IByteBuffer* buffer)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        byte[] clientId = new byte[7];
        clientId[0] = CLIENT_ID_ETHER;
        System.arraycopy(mClientMac, 0, clientId, 1, 6);
        addTlv(buffer, DHCP_MESSAGE_TYPE, DHCP_MESSAGE_TYPE_REQUEST);
        addTlv(buffer, DHCP_PARAMETER_LIST, mRequestedParams);
        addTlvEnd(buffer);
#endif
}

ECode DhcpInformPacket::DoNextOp(
    /* [in] */ DhcpStateMachine* machine)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        InetAddress clientRequest =
            mRequestedIp == NULL ? mClientIp : mRequestedIp;
        machine.onInformReceived(mTransId, mClientMac, clientRequest,
            mRequestedParams);
#endif
}

} // namespace Dhcp
} // namespace Net
} // namespace Droid
} // namespace Elastos
