
#include <Elastos.CoreLibrary.Utility.h>
#include <Elastos.CoreLibrary.Net.h>
#include "elastos/droid/net/dhcp/DhcpDiscoverPacket.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Dhcp {

DhcpDiscoverPacket::DhcpDiscoverPacket()
{}

ECode DhcpDiscoverPacket::constructor(
    /* [in] */ Int32 transId,
    /* [in] */ ArrayOf<Byte>* clientMac,
    /* [in] */ Boolean broadcast)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        super(transId, Inet4Address.ANY, Inet4Address.ANY, Inet4Address.ANY,
            Inet4Address.ANY, clientMac, broadcast);
#endif
}

ECode DhcpDiscoverPacket::ToString(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        String s = super.toString();
        return s + " DISCOVER " +
                (mBroadcast ? "broadcast " : "unicast ");
#endif
}

ECode DhcpDiscoverPacket::BuildPacket(
    /* [in] */ Int32 encap,
    /* [in] */ Int16 destUdp,
    /* [in] */ Int16 srcUdp,
    /* [out] */ IByteBuffer** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        ByteBuffer result = ByteBuffer.allocate(MAX_LENGTH);
        InetAddress destIp = Inet4Address.ALL;
        fillInPacket(encap, Inet4Address.ALL, Inet4Address.ANY, destUdp, srcUdp,
            result, DHCP_BOOTREQUEST, TRUE);
        result.flip();
        return result;
#endif
}

ECode DhcpDiscoverPacket::FinishPacket(
    /* [in] */ IByteBuffer* buffer)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        addTlv(buffer, DHCP_MESSAGE_TYPE, DHCP_MESSAGE_TYPE_DISCOVER);
        addTlv(buffer, DHCP_PARAMETER_LIST, mRequestedParams);
        addTlvEnd(buffer);
#endif
}

ECode DhcpDiscoverPacket::DoNextOp(
    /* [in] */ DhcpStateMachine* machine)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        // currently omitted: host name
        machine.onDiscoverReceived(mBroadcast, mTransId, mClientMac,
            mRequestedParams);
#endif
}


} // namespace Dhcp
} // namespace Net
} // namespace Droid
} // namespace Elastos
