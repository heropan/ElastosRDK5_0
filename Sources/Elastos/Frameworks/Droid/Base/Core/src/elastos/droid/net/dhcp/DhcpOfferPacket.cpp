
#include "elastos/droid/net/dhcp/DhcpOfferPacket.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Dhcp {

DhcpOfferPacket::DhcpOfferPacket()
{}

ECode DhcpOfferPacket::constructor(
    /* [in] */ Int32 transId,
    /* [in] */ Boolean broadcast,
    /* [in] */ IInetAddress* serverAddress,
    /* [in] */ IInetAddress* clientIp,
    /* [in] */ ArrayOf<Byte>* clientMac)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        super(transId, Inet4Address.ANY, clientIp, Inet4Address.ANY,
            Inet4Address.ANY, clientMac, broadcast);
        mSrcIp = serverAddress;

#endif
}

ECode DhcpOfferPacket::ToString(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        String s = super.toString();
        String dnsServers = ", DNS servers: ";
        if (mDnsServers != null) {
            for (InetAddress dnsServer: mDnsServers) {
                dnsServers += dnsServer + " ";
            }
        }
        return s + " OFFER, ip " + mYourIp + ", mask " + mSubnetMask +
                dnsServers + ", gateway " + mGateway +
                " lease time " + mLeaseTime + ", domain " + mDomainName;

#endif
}

ECode DhcpOfferPacket::BuildPacket(
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

ECode DhcpOfferPacket::FinishPacket(
    /* [in] */ IByteBuffer* buffer)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        addTlv(buffer, DHCP_MESSAGE_TYPE, DHCP_MESSAGE_TYPE_OFFER);
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

ECode DhcpOfferPacket::DoNextOp(
    /* [in] */ IDhcpStateMachine* machine)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        machine.onOfferReceived(mBroadcast, mTransId, mClientMac, mYourIp,
            mServerIdentifier);

#endif
}


} // namespace Dhcp
} // namespace Net
} // namespace Droid
} // namespace Elastos
