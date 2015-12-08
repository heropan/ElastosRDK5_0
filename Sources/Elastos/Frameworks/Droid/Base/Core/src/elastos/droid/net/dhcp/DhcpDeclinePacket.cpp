
#include "elastos/droid/net/dhcp/DhcpDeclinePacket.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Dhcp {

DhcpDeclinePacket::DhcpDeclinePacket()
{}

ECode DhcpDeclinePacket::constructor(
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

ECode DhcpDeclinePacket::ToString(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        String s = super.toString();
        return s + " DECLINE";
#endif
}

ECode DhcpDeclinePacket::BuildPacket(
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

ECode DhcpDeclinePacket::FinishPacket(
    /* [in] */ IByteBuffer* buffer)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        // None needed
#endif
}

ECode DhcpDeclinePacket::DoNextOp(
    /* [in] */ IDhcpStateMachine* machine)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        machine.onDeclineReceived(mClientMac, mRequestedIp);
#endif
}


} // namespace Dhcp
} // namespace Net
} // namespace Droid
} // namespace Elastos
