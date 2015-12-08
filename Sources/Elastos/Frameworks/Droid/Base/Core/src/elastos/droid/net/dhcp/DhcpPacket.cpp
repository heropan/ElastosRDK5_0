
#include "elastos/droid/net/dhcp/DhcpPacket.h"

using Elastos::Net::IInet4AddressHelper;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Dhcp {

const Int32 DhcpPacket::ENCAP_L2 = 0;
const Int32 DhcpPacket::ENCAP_L3 = 1;
const Int32 DhcpPacket::ENCAP_BOOTP = 2;
const Int16 DhcpPacket::DHCP_CLIENT = 68;
const Byte DhcpPacket::IP_TYPE_UDP = 0x11;
const Int16 DhcpPacket::DHCP_SERVER = 67;
const Byte DhcpPacket::DHCP_BOOTREQUEST = 1;
const Byte DhcpPacket::DHCP_BOOTREPLY = 2;
const Byte DhcpPacket::CLIENT_ID_ETHER = 1;
const Byte DhcpPacket::IP_VERSION_HEADER_LEN = 0x45;
const Int16 DhcpPacket::IP_FLAGS_OFFSET = 0x4000;
const Byte DhcpPacket::IP_TOS_LOWDELAY = 0x10;
const Byte DhcpPacket::IP_TTL = 0x40;
const String DhcpPacket::TAG("DhcpPacket");
const Int32 DhcpPacket::MAX_LENGTH = 1500;
const Byte DhcpPacket::DHCP_SUBNET_MASK = 1;
const Byte DhcpPacket::DHCP_ROUTER = 3;
const Byte DhcpPacket::DHCP_DNS_SERVER = 6;
const Byte DhcpPacket::DHCP_HOST_NAME = 12;
const Byte DhcpPacket::DHCP_DOMAIN_NAME = 15;
const Byte DhcpPacket::DHCP_BROADCAST_ADDRESS = 28;
const Byte DhcpPacket::DHCP_REQUESTED_IP = 50;
const Byte DhcpPacket::DHCP_LEASE_TIME = 51;
const Byte DhcpPacket::DHCP_MESSAGE_TYPE = 53;
const Byte DhcpPacket::DHCP_MESSAGE_TYPE_DISCOVER = 1;
const Byte DhcpPacket::DHCP_MESSAGE_TYPE_OFFER = 2;
const Byte DhcpPacket::DHCP_MESSAGE_TYPE_REQUEST = 3;
const Byte DhcpPacket::DHCP_MESSAGE_TYPE_DECLINE = 4;
const Byte DhcpPacket::DHCP_MESSAGE_TYPE_ACK = 5;
const Byte DhcpPacket::DHCP_MESSAGE_TYPE_NAK = 6;
const Byte DhcpPacket::DHCP_MESSAGE_TYPE_INFORM = 8;
const Byte DhcpPacket::DHCP_SERVER_IDENTIFIER = 54;
const Byte DhcpPacket::DHCP_PARAMETER_LIST = 55;
const Byte DhcpPacket::DHCP_MESSAGE = 56;
const Byte DhcpPacket::DHCP_RENEWAL_TIME = 58;
const Byte DhcpPacket::DHCP_VENDOR_CLASS_ID = 60;
const Byte DhcpPacket::DHCP_CLIENT_IDENTIFIER = 61;

DhcpPacket::DhcpPacket()
    : mTransId(0)
    , mBroadcast(FALSE)
{}


ECode DhcpPacket::BuildPacket(
    /* [in] */ Int32 encap,
    /* [in] */ Int16 destUdp,
    /* [in] */ Int16 srcUdp,
    /* [out] */ IByteBuffer** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
#endif
}

ECode DhcpPacket::constructor(
    /* [in] */ Int32 transId,
    /* [in] */ IInetAddress* clientIp,
    /* [in] */ IInetAddress* yourIp,
    /* [in] */ IInetAddress* nextIp,
    /* [in] */ IInetAddress* relayIp,
    /* [in] */ ArrayOf<Byte>* clientMac,
    /* [in] */ Boolean broadcast)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mTransId = transId;
        mClientIp = clientIp;
        mYourIp = yourIp;
        mNextIp = nextIp;
        mRelayIp = relayIp;
        mClientMac = clientMac;
        mBroadcast = broadcast;
#endif
}

ECode DhcpPacket::GetTransactionId(
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return mTransId;
#endif
}

ECode DhcpPacket::FillInPacket(
    /* [in] */ Int32 encap,
    /* [in] */ IInetAddress* destIp,
    /* [in] */ IInetAddress* srcIp,
    /* [in] */ Int16 destUdp,
    /* [in] */ Int16 srcUdp,
    /* [in] */ IByteBuffer* buf,
    /* [in] */ Byte requestCode,
    /* [in] */ Boolean broadcast)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        byte[] destIpArray = destIp.getAddress();
        byte[] srcIpArray = srcIp.getAddress();
        Int32 ipLengthOffset = 0;
        Int32 ipChecksumOffset = 0;
        Int32 endIpHeader = 0;
        Int32 udpHeaderOffset = 0;
        Int32 udpLengthOffset = 0;
        Int32 udpChecksumOffset = 0;
        buf.clear();
        buf.order(ByteOrder.BIG_ENDIAN);
        // if a full IP packet needs to be generated, put the IP & UDP
        // headers in place, and pre-populate with artificial values
        // needed to seed the IP checksum.
        if (encap == ENCAP_L3) {
            // fake IP header, used in the IP-header checksum
            buf.put(IP_VERSION_HEADER_LEN);
            buf.put(IP_TOS_LOWDELAY);    // tos: IPTOS_LOWDELAY
            ipLengthOffset = buf.position();
            buf.putShort((short)0);  // length
            buf.putShort((short)0);  // id
            buf.putShort(IP_FLAGS_OFFSET); // ip offset: don't fragment
            buf.put(IP_TTL);    // TTL: use default 64 from RFC1340
            buf.put(IP_TYPE_UDP);
            ipChecksumOffset = buf.position();
            buf.putShort((short) 0); // checksum
            buf.put(srcIpArray);
            buf.put(destIpArray);
            endIpHeader = buf.position();
            // UDP header
            udpHeaderOffset = buf.position();
            buf.putShort(srcUdp);
            buf.putShort(destUdp);
            udpLengthOffset = buf.position();
            buf.putShort((short) 0); // length
            udpChecksumOffset = buf.position();
            buf.putShort((short) 0); // UDP checksum -- initially zero
        }
        // DHCP payload
        buf.put(requestCode);
        buf.put((byte) 1); // Hardware Type: Ethernet
        buf.put((byte) mClientMac.length); // Hardware Address Length
        buf.put((byte) 0); // Hop Count
        buf.putInt(mTransId);  // Transaction ID
        buf.putShort((short) 0); // Elapsed Seconds
        if (broadcast) {
            buf.putShort((short) 0x8000); // Flags
        } else {
            buf.putShort((short) 0x0000); // Flags
        }
        buf.put(mClientIp.getAddress());
        buf.put(mYourIp.getAddress());
        buf.put(mNextIp.getAddress());
        buf.put(mRelayIp.getAddress());
        buf.put(mClientMac);
        buf.position(buf.position() +
                     (16 - mClientMac.length) // pad addr to 16 bytes
                     + 64     // empty server host name (64 bytes)
                     + 128);  // empty boot file name (128 bytes)
        buf.putInt(0x63825363); // magic number
        finishPacket(buf);
        // round up to an even number of octets
        if ((buf.position() & 1) == 1) {
            buf.put((byte) 0);
        }
        // If an IP packet is being built, the IP & UDP checksums must be
        // computed.
        if (encap == ENCAP_L3) {
            // fix UDP header: insert length
            short udpLen = (short)(buf.position() - udpHeaderOffset);
            buf.putShort(udpLengthOffset, udpLen);
            // fix UDP header: checksum
            // checksum for UDP at udpChecksumOffset
            Int32 udpSeed = 0;
            // apply IPv4 pseudo-header.  Read IP address src and destination
            // values from the IP header and accumulate checksum.
            udpSeed += intAbs(buf.getShort(ipChecksumOffset + 2));
            udpSeed += intAbs(buf.getShort(ipChecksumOffset + 4));
            udpSeed += intAbs(buf.getShort(ipChecksumOffset + 6));
            udpSeed += intAbs(buf.getShort(ipChecksumOffset + 8));
            // accumulate extra data for the pseudo-header
            udpSeed += IP_TYPE_UDP;
            udpSeed += udpLen;
            // and compute UDP checksum
            buf.putShort(udpChecksumOffset, (short) checksum(buf, udpSeed,
                                                             udpHeaderOffset,
                                                             buf.position()));
            // fix IP header: insert length
            buf.putShort(ipLengthOffset, (short)buf.position());
            // fixup IP-header checksum
            buf.putShort(ipChecksumOffset,
                         (short) checksum(buf, 0, 0, endIpHeader));
        }
#endif
}

ECode DhcpPacket::IntAbs(
    /* [in] */ Int16 v,
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (v < 0) {
            Int32 r = v + 65536;
            return r;
        } else {
            return(v);
        }
#endif
}

ECode DhcpPacket::Checksum(
    /* [in] */ IByteBuffer* buf,
    /* [in] */ Int32 seed,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ Int32* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        Int32 sum = seed;
        Int32 bufPosition = buf.position();
        // set position of original ByteBuffer, so that the ShortBuffer
        // will be correctly initialized
        buf.position(start);
        ShortBuffer shortBuf = buf.asShortBuffer();
        // re-set ByteBuffer position
        buf.position(bufPosition);
        short[] shortArray = new short[(end - start) / 2];
        shortBuf.get(shortArray);
        for (short s : shortArray) {
            sum += intAbs(s);
        }
        start += shortArray.length * 2;
        // see if a singleton byte remains
        if (end != start) {
            short b = buf.get(start);
            // make it unsigned
            if (b < 0) {
                b += 256;
            }
            sum += b * 256;
        }
        sum = ((sum >> 16) & 0xFFFF) + (sum & 0xFFFF);
        sum = ((sum + ((sum >> 16) & 0xFFFF)) & 0xFFFF);
        Int32 negated = ~sum;
        return intAbs((short) negated);
#endif
}

ECode DhcpPacket::AddTlv(
    /* [in] */ IByteBuffer* buf,
    /* [in] */ Byte type,
    /* [in] */ Byte value)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        buf.put(type);
        buf.put((byte) 1);
        buf.put(value);
#endif
}

ECode DhcpPacket::AddTlv(
    /* [in] */ IByteBuffer* buf,
    /* [in] */ Byte type,
    /* [in] */ ArrayOf<Byte>* payload)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (payload != NULL) {
            buf.put(type);
            buf.put((byte) payload.length);
            buf.put(payload);
        }
#endif
}

ECode DhcpPacket::AddTlv(
    /* [in] */ IByteBuffer* buf,
    /* [in] */ Byte type,
    /* [in] */ IInetAddress* addr)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (addr != NULL) {
            addTlv(buf, type, addr.getAddress());
        }
#endif
}

ECode DhcpPacket::AddTlv(
    /* [in] */ IByteBuffer* buf,
    /* [in] */ Byte type,
    /* [in] */ IList* addrs)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (addrs != NULL && addrs.size() > 0) {
            buf.put(type);
            buf.put((byte)(4 * addrs.size()));
            for (InetAddress addr : addrs) {
                buf.put(addr.getAddress());
            }
        }
#endif
}

ECode DhcpPacket::AddTlv(
    /* [in] */ IByteBuffer* buf,
    /* [in] */ Byte type,
    /* [in] */ IInteger32* value)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (value != NULL) {
            buf.put(type);
            buf.put((byte) 4);
            buf.putInt(value.intValue());
        }
#endif
}

ECode DhcpPacket::AddTlv(
    /* [in] */ IByteBuffer* buf,
    /* [in] */ Byte type,
    /* [in] */ const String& str)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (str != NULL) {
            buf.put(type);
            buf.put((byte) str.length());
            for (Int32 i = 0; i < str.length(); i++) {
                buf.put((byte) str.charAt(i));
            }
        }
#endif
}

ECode DhcpPacket::AddTlvEnd(
    /* [in] */ IByteBuffer* buf)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        buf.put((byte) 0xFF);
#endif
}

ECode DhcpPacket::MacToString(
    /* [in] */ ArrayOf<Byte>* mac,
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        String macAddr = "";
        for (Int32 i = 0; i < mac.length; i++) {
            String hexString = "0" + Integer.toHexString(mac[i]);
            // substring operation grabs the last 2 digits: this
            // allows signed bytes to be converted correctly.
            macAddr += hexString.substring(hexString.length() - 2);
            if (i != (mac.length - 1)) {
                macAddr += ":";
            }
        }
        return macAddr;
#endif
}

ECode DhcpPacket::ToString(
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        String macAddr = macToString(mClientMac);
        return macAddr;
#endif
}

ECode DhcpPacket::ReadIpAddress(
    /* [in] */ IByteBuffer* packet,
    /* [out] */ IInetAddress** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        InetAddress result = NULL;
        byte[] ipAddr = new byte[4];
        packet.get(ipAddr);
        try {
            result = InetAddress.getByAddress(ipAddr);
        } catch (UnknownHostException ex) {
            // ipAddr is numeric, so this should not be
            // triggered.  However, if it is, just nullify
            result = NULL;
        }
        return result;
#endif
}

ECode DhcpPacket::ReadAsciiString(
    /* [in] */ IByteBuffer* buf,
    /* [in] */ Int32 byteCount,
    /* [out] */ String* result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        byte[] bytes = new byte[byteCount];
        buf.get(bytes);
        return new String(bytes, 0, bytes.length, StandardCharsets.US_ASCII);
#endif
}

ECode DhcpPacket::DecodeFullPacket(
    /* [in] */ IByteBuffer* packet,
    /* [in] */ Int32 pktType,
    /* [out] */ DhcpPacket** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        // bootp parameters
        Int32 transactionId;
        InetAddress clientIp;
        InetAddress yourIp;
        InetAddress nextIp;
        InetAddress relayIp;
        byte[] clientMac;
        List<InetAddress> dnsServers = new ArrayList<InetAddress>();
        InetAddress gateway = NULL; // aka router
        Integer leaseTime = NULL;
        InetAddress serverIdentifier = NULL;
        InetAddress netMask = NULL;
        String message = NULL;
        String vendorId = NULL;
        byte[] expectedParams = NULL;
        String hostName = NULL;
        String domainName = NULL;
        InetAddress ipSrc = NULL;
        InetAddress ipDst = NULL;
        InetAddress bcAddr = NULL;
        InetAddress requestedIp = NULL;
        // dhcp options
        byte dhcpType = (byte) 0xFF;
        packet.order(ByteOrder.BIG_ENDIAN);
        // check to see if we need to parse L2, IP, and UDP encaps
        if (pktType == ENCAP_L2) {
            // System.out.println("buffer len " + packet.limit());
            byte[] l2dst = new byte[6];
            byte[] l2src = new byte[6];
            packet.get(l2dst);
            packet.get(l2src);
            short l2type = packet.getShort();
            if (l2type != 0x0800)
                return NULL;
        }
        if ((pktType == ENCAP_L2) || (pktType == ENCAP_L3)) {
            // assume l2type is 0x0800, i.e. IP
            byte ipType = packet.get();
            // System.out.println("ipType is " + ipType);
            byte ipDiffServicesField = packet.get();
            short ipTotalLength = packet.getShort();
            short ipIdentification = packet.getShort();
            byte ipFlags = packet.get();
            byte ipFragOffset = packet.get();
            byte ipTTL = packet.get();
            byte ipProto = packet.get();
            short ipChksm = packet.getShort();
            ipSrc = readIpAddress(packet);
            ipDst = readIpAddress(packet);
            if (ipProto != IP_TYPE_UDP) // UDP
                return NULL;
            // assume UDP
            short udpSrcPort = packet.getShort();
            short udpDstPort = packet.getShort();
            short udpLen = packet.getShort();
            short udpChkSum = packet.getShort();
            if ((udpSrcPort != DHCP_SERVER) && (udpSrcPort != DHCP_CLIENT))
                return NULL;
        }
        // assume bootp
        byte type = packet.get();
        byte hwType = packet.get();
        byte addrLen = packet.get();
        byte hops = packet.get();
        transactionId = packet.getInt();
        short elapsed = packet.getShort();
        short bootpFlags = packet.getShort();
        boolean broadcast = (bootpFlags & 0x8000) != 0;
        byte[] ipv4addr = new byte[4];
        try {
            packet.get(ipv4addr);
            clientIp = InetAddress.getByAddress(ipv4addr);
            packet.get(ipv4addr);
            yourIp = InetAddress.getByAddress(ipv4addr);
            packet.get(ipv4addr);
            nextIp = InetAddress.getByAddress(ipv4addr);
            packet.get(ipv4addr);
            relayIp = InetAddress.getByAddress(ipv4addr);
        } catch (UnknownHostException ex) {
            return NULL;
        }
        clientMac = new byte[addrLen];
        packet.get(clientMac);
        // skip over address padding (16 octets allocated)
        packet.position(packet.position() + (16 - addrLen)
                        + 64    // skip server host name (64 chars)
                        + 128); // skip boot file name (128 chars)
        Int32 dhcpMagicCookie = packet.getInt();
        if (dhcpMagicCookie !=  0x63825363)
            return NULL;
        // parse options
        boolean notFinishedOptions = TRUE;
        while ((packet.position() < packet.limit()) && notFinishedOptions) {
            byte optionType = packet.get();
            if (optionType == (byte) 0xFF) {
                notFinishedOptions = FALSE;
            } else {
                byte optionLen = packet.get();
                Int32 expectedLen = 0;
                switch(optionType) {
                    case DHCP_SUBNET_MASK:
                        netMask = readIpAddress(packet);
                        expectedLen = 4;
                        break;
                    case DHCP_ROUTER:
                        gateway = readIpAddress(packet);
                        expectedLen = 4;
                        break;
                    case DHCP_DNS_SERVER:
                        expectedLen = 0;
                        for (expectedLen = 0; expectedLen < optionLen;
                             expectedLen += 4) {
                            dnsServers.add(readIpAddress(packet));
                        }
                        break;
                    case DHCP_HOST_NAME:
                        expectedLen = optionLen;
                        hostName = readAsciiString(packet, optionLen);
                        break;
                    case DHCP_DOMAIN_NAME:
                        expectedLen = optionLen;
                        domainName = readAsciiString(packet, optionLen);
                        break;
                    case DHCP_BROADCAST_ADDRESS:
                        bcAddr = readIpAddress(packet);
                        expectedLen = 4;
                        break;
                    case DHCP_REQUESTED_IP:
                        requestedIp = readIpAddress(packet);
                        expectedLen = 4;
                        break;
                    case DHCP_LEASE_TIME:
                        leaseTime = Integer.valueOf(packet.getInt());
                        expectedLen = 4;
                        break;
                    case DHCP_MESSAGE_TYPE:
                        dhcpType = packet.get();
                        expectedLen = 1;
                        break;
                    case DHCP_SERVER_IDENTIFIER:
                        serverIdentifier = readIpAddress(packet);
                        expectedLen = 4;
                        break;
                    case DHCP_PARAMETER_LIST:
                        expectedParams = new byte[optionLen];
                        packet.get(expectedParams);
                        expectedLen = optionLen;
                        break;
                    case DHCP_MESSAGE:
                        expectedLen = optionLen;
                        message = readAsciiString(packet, optionLen);
                        break;
                    case DHCP_VENDOR_CLASS_ID:
                        expectedLen = optionLen;
                        vendorId = readAsciiString(packet, optionLen);
                        break;
                    case DHCP_CLIENT_IDENTIFIER: { // Client identifier
                        byte[] id = new byte[optionLen];
                        packet.get(id);
                        expectedLen = optionLen;
                    } break;
                    default:
                        // ignore any other parameters
                        for (Int32 i = 0; i < optionLen; i++) {
                            expectedLen++;
                            byte throwaway = packet.get();
                        }
                }
                if (expectedLen != optionLen) {
                    return NULL;
                }
            }
        }
        DhcpPacket newPacket;
        switch(dhcpType) {
            case -1: return NULL;
            case DHCP_MESSAGE_TYPE_DISCOVER:
                newPacket = new DhcpDiscoverPacket(
                    transactionId, clientMac, broadcast);
                break;
            case DHCP_MESSAGE_TYPE_OFFER:
                newPacket = new DhcpOfferPacket(
                    transactionId, broadcast, ipSrc, yourIp, clientMac);
                break;
            case DHCP_MESSAGE_TYPE_REQUEST:
                newPacket = new DhcpRequestPacket(
                    transactionId, clientIp, clientMac, broadcast);
                break;
            case DHCP_MESSAGE_TYPE_DECLINE:
                newPacket = new DhcpDeclinePacket(
                    transactionId, clientIp, yourIp, nextIp, relayIp,
                    clientMac);
                break;
            case DHCP_MESSAGE_TYPE_ACK:
                newPacket = new DhcpAckPacket(
                    transactionId, broadcast, ipSrc, yourIp, clientMac);
                break;
            case DHCP_MESSAGE_TYPE_NAK:
                newPacket = new DhcpNakPacket(
                    transactionId, clientIp, yourIp, nextIp, relayIp,
                    clientMac);
                break;
            case DHCP_MESSAGE_TYPE_INFORM:
                newPacket = new DhcpInformPacket(
                    transactionId, clientIp, yourIp, nextIp, relayIp,
                    clientMac);
                break;
            default:
                System.out.println("Unimplemented type: " + dhcpType);
                return NULL;
        }
        newPacket.mBroadcastAddress = bcAddr;
        newPacket.mDnsServers = dnsServers;
        newPacket.mDomainName = domainName;
        newPacket.mGateway = gateway;
        newPacket.mHostName = hostName;
        newPacket.mLeaseTime = leaseTime;
        newPacket.mMessage = message;
        newPacket.mRequestedIp = requestedIp;
        newPacket.mRequestedParams = expectedParams;
        newPacket.mServerIdentifier = serverIdentifier;
        newPacket.mSubnetMask = netMask;
        return newPacket;
#endif
}

ECode DhcpPacket::DecodeFullPacket(
    /* [in] */ ArrayOf<Byte>* packet,
    /* [in] */ Int32 pktType,
    /* [out] */ DhcpPacket** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        ByteBuffer buffer = ByteBuffer.wrap(packet).order(ByteOrder.BIG_ENDIAN);
        return decodeFullPacket(buffer, pktType);
#endif
}

ECode DhcpPacket::BuildDiscoverPacket(
    /* [in] */ Int32 encap,
    /* [in] */ Int32 transactionId,
    /* [in] */ ArrayOf<Byte>* clientMac,
    /* [in] */ Boolean broadcast,
    /* [in] */ ArrayOf<Byte>* expectedParams,
    /* [out] */ IByteBuffer** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        DhcpPacket pkt = new DhcpDiscoverPacket(
            transactionId, clientMac, broadcast);
        pkt.mRequestedParams = expectedParams;
        return pkt.buildPacket(encap, DHCP_SERVER, DHCP_CLIENT);
#endif
}

ECode DhcpPacket::BuildOfferPacket(
    /* [in] */ Int32 encap,
    /* [in] */ Int32 transactionId,
    /* [in] */ Boolean broadcast,
    /* [in] */ IInetAddress* serverIpAddr,
    /* [in] */ IInetAddress* clientIpAddr,
    /* [in] */ ArrayOf<Byte>* mac,
    /* [in] */ IInteger32* timeout,
    /* [in] */ IInetAddress* netMask,
    /* [in] */ IInetAddress* bcAddr,
    /* [in] */ IInetAddress* gateway,
    /* [in] */ IList* dnsServers,
    /* [in] */ IInetAddress* dhcpServerIdentifier,
    /* [in] */ const String& domainName,
    /* [out] */ IByteBuffer** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        DhcpPacket pkt = new DhcpOfferPacket(
            transactionId, broadcast, serverIpAddr, clientIpAddr, mac);
        pkt.mGateway = gateway;
        pkt.mDnsServers = dnsServers;
        pkt.mLeaseTime = timeout;
        pkt.mDomainName = domainName;
        pkt.mServerIdentifier = dhcpServerIdentifier;
        pkt.mSubnetMask = netMask;
        pkt.mBroadcastAddress = bcAddr;
        return pkt.buildPacket(encap, DHCP_CLIENT, DHCP_SERVER);
#endif
}

ECode DhcpPacket::BuildAckPacket(
    /* [in] */ Int32 encap,
    /* [in] */ Int32 transactionId,
    /* [in] */ Boolean broadcast,
    /* [in] */ IInetAddress* serverIpAddr,
    /* [in] */ IInetAddress* clientIpAddr,
    /* [in] */ ArrayOf<Byte>* mac,
    /* [in] */ IInteger32* timeout,
    /* [in] */ IInetAddress* netMask,
    /* [in] */ IInetAddress* bcAddr,
    /* [in] */ IInetAddress* gateway,
    /* [in] */ IList* dnsServers,
    /* [in] */ IInetAddress* dhcpServerIdentifier,
    /* [in] */ const String& domainName,
    /* [out] */ IByteBuffer** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        DhcpPacket pkt = new DhcpAckPacket(
            transactionId, broadcast, serverIpAddr, clientIpAddr, mac);
        pkt.mGateway = gateway;
        pkt.mDnsServers = dnsServers;
        pkt.mLeaseTime = timeout;
        pkt.mDomainName = domainName;
        pkt.mSubnetMask = netMask;
        pkt.mServerIdentifier = dhcpServerIdentifier;
        pkt.mBroadcastAddress = bcAddr;
        return pkt.buildPacket(encap, DHCP_CLIENT, DHCP_SERVER);
#endif
}

ECode DhcpPacket::BuildNakPacket(
    /* [in] */ Int32 encap,
    /* [in] */ Int32 transactionId,
    /* [in] */ IInetAddress* serverIpAddr,
    /* [in] */ IInetAddress* clientIpAddr,
    /* [in] */ ArrayOf<Byte>* mac,
    /* [out] */ IByteBuffer** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        DhcpPacket pkt = new DhcpNakPacket(transactionId, clientIpAddr,
            serverIpAddr, serverIpAddr, serverIpAddr, mac);
        pkt.mMessage = "requested address not available";
        pkt.mRequestedIp = clientIpAddr;
        return pkt.buildPacket(encap, DHCP_CLIENT, DHCP_SERVER);
#endif
}

ECode DhcpPacket::BuildRequestPacket(
    /* [in] */ Int32 encap,
    /* [in] */ Int32 transactionId,
    /* [in] */ IInetAddress* clientIp,
    /* [in] */ Boolean broadcast,
    /* [in] */ ArrayOf<Byte>* clientMac,
    /* [in] */ IInetAddress* requestedIpAddress,
    /* [in] */ IInetAddress* serverIdentifier,
    /* [in] */ ArrayOf<Byte>* requestedParams,
    /* [in] */ const String& hostName,
    /* [out] */ IByteBuffer** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        DhcpPacket pkt = new DhcpRequestPacket(transactionId, clientIp,
            clientMac, broadcast);
        pkt.mRequestedIp = requestedIpAddress;
        pkt.mServerIdentifier = serverIdentifier;
        pkt.mHostName = hostName;
        pkt.mRequestedParams = requestedParams;
        ByteBuffer result = pkt.buildPacket(encap, DHCP_SERVER, DHCP_CLIENT);
        return result;
#endif
}


} // namespace Dhcp
} // namespace Net
} // namespace Droid
} // namespace Elastos
