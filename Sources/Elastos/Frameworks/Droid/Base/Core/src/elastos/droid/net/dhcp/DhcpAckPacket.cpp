
#include "elastos/droid/net/dhcp/DhcpAckPacket.h"

using Elastos::Net::CInet4AddressHelper;
using Elastos::Net::IInet4AddressHelper;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Dhcp {

DhcpAckPacket::DhcpAckPacket()
{}

DhcpAckPacket::constructor(
    /* [in] */ Int32 transId,
    /* [in] */ Boolean broadcast,
    /* [in] */ IInetAddress* serverAddress,
    /* [in] */ IInetAddress* clientIp,
    /* [in] */ ArrayOf<Byte>* clientMac)
{
    AutoPtr<IInetAddress> any;
    AutoPtr<IInet4AddressHelper> inet4AddressHelper;
    CInet4AddressHelper::AcquireSingleton((IInet4AddressHelper**)&inet4AddressHelper);
    inet4AddressHelper->GetANY((IInetAddress**)&any);
    DhcpPacket::constructor(transId, any, clientIp, serverAddress, any, clientMac, broadcast);
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mBroadcast = broadcast;
        mSrcIp = serverAddress;
#endif
}

ECode DhcpAckPacket::ToString(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        String s = super.toString();
        String dnsServers = " DNS servers: ";
        for (InetAddress dnsServer: mDnsServers) {
            dnsServers += dnsServer.toString() + " ";
        }
        return s + " ACK: your new IP " + mYourIp +
                ", netmask " + mSubnetMask +
                ", gateway " + mGateway + dnsServers +
                ", lease time " + mLeaseTime;

#endif
}

ECode DhcpAckPacket::BuildPacket(
    /* [in] */ Int32 encap,
    /* [in] */ Int16 destUdp,
    /* [in] */ Int16 srcUdp,
    /* [out] */ IByteBuffer** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        ByteBuffer result = ByteBuffer.allocate(MAX_LENGTH);
        InetAddress destIp = mBroadcast ? Inet4Address.ALL : mYourIp;
        InetAddress srcIp = mBroadcast ? Inet4Address.ANY : mSrcIp;
        fillInPacket(encap, destIp, srcIp, destUdp, srcUdp, result,
            DHCP_BOOTREPLY, mBroadcast);
        result.flip();
        return result;

#endif
}

ECode DhcpAckPacket::FinishPacket(
    /* [in] */ IByteBuffer* buffer)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        addTlv(buffer, DHCP_MESSAGE_TYPE, DHCP_MESSAGE_TYPE_ACK);
        addTlv(buffer, DHCP_SERVER_IDENTIFIER, mServerIdentifier);
        addTlv(buffer, DHCP_LEASE_TIME, mLeaseTime);
        // the client should renew at 1/2 the lease-expiry interval
        if (mLeaseTime != null) {
            addTlv(buffer, DHCP_RENEWAL_TIME,
                Integer.valueOf(mLeaseTime.intValue() / 2));
        }
        addTlv(buffer, DHCP_SUBNET_MASK, mSubnetMask);
        addTlv(buffer, DHCP_ROUTER, mGateway);
        addTlv(buffer, DHCP_DOMAIN_NAME, mDomainName);
        addTlv(buffer, DHCP_BROADCAST_ADDRESS, mBroadcastAddress);
        addTlv(buffer, DHCP_DNS_SERVER, mDnsServers);
        addTlvEnd(buffer);

#endif
}

ECode DhcpAckPacket::GetInt(
    /* [in] */ IInteger32* v,
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (v == null) {
            return 0;
        } else {
            return v.intValue();
        }

#endif
}

ECode DhcpAckPacket::DoNextOp(
    /* [in] */ IDhcpStateMachine* machine)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        machine.onAckReceived(mYourIp, mSubnetMask, mGateway, mDnsServers,
            mServerIdentifier, getInt(mLeaseTime));

#endif
}


} // namespace Dhcp
} // namespace Net
} // namespace Droid
} // namespace Elastos
