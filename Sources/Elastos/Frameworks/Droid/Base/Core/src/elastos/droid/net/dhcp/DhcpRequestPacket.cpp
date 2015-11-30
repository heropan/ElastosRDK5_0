
#include "elastos/droid/net/dhcp/DhcpRequestPacket.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Dhcp {

ECode DhcpRequestPacket::constructor(
    /* [in] */ Int32 transId,
    /* [in] */ IInetAddress* clientIp,
    /* [in] */ ArrayOf<Byte>* clientMac,
    /* [in] */ Boolean broadcast)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        super(transId, clientIp, Inet4Address.ANY, Inet4Address.ANY,
          Inet4Address.ANY, clientMac, broadcast);
#endif
}

ECode DhcpRequestPacket::ToString(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        String s = super.toString();
        return s + " REQUEST, desired IP " + mRequestedIp + " from host '"
            + mHostName + "', param list length "
            + (mRequestedParams == NULL ? 0 : mRequestedParams.length);
#endif
}

ECode DhcpRequestPacket::BuildPacket(
    /* [in] */ Int32 encap,
    /* [in] */ Int16 destUdp,
    /* [in] */ Int16 srcUdp,
    /* [out] */ IByteBuffer** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        ByteBuffer result = ByteBuffer.allocate(MAX_LENGTH);
        fillInPacket(encap, Inet4Address.ALL, Inet4Address.ANY, destUdp, srcUdp,
            result, DHCP_BOOTREQUEST, mBroadcast);
        result.flip();
        return result;
#endif
}

ECode DhcpRequestPacket::FinishPacket(
    /* [in] */ IByteBuffer* buffer)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        byte[] clientId = new byte[7];
        // assemble client identifier
        clientId[0] = CLIENT_ID_ETHER;
        System.arraycopy(mClientMac, 0, clientId, 1, 6);
        addTlv(buffer, DHCP_MESSAGE_TYPE, DHCP_MESSAGE_TYPE_REQUEST);
        addTlv(buffer, DHCP_PARAMETER_LIST, mRequestedParams);
        addTlv(buffer, DHCP_REQUESTED_IP, mRequestedIp);
        addTlv(buffer, DHCP_SERVER_IDENTIFIER, mServerIdentifier);
        addTlv(buffer, DHCP_CLIENT_IDENTIFIER, clientId);
        addTlvEnd(buffer);
#endif
}

ECode DhcpRequestPacket::DoNextOp(
    /* [in] */ IDhcpStateMachine* machine)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        InetAddress clientRequest =
            mRequestedIp == NULL ? mClientIp : mRequestedIp;
        Log.v(TAG, "requested IP is " + mRequestedIp + " and client IP is " +
            mClientIp);
        machine.onRequestReceived(mBroadcast, mTransId, mClientMac,
            clientRequest, mRequestedParams, mHostName);
#endif
}

} // namespace Dhcp
} // namespace Net
} // namespace Droid
} // namespace Elastos
