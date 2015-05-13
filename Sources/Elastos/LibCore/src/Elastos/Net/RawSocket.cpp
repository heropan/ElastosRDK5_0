
#include <cmdef.h>
#include <RawSocket.h>
#include <CCloseGuard.h>
#include <CIoBridge.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/rtnetlink.h>
#include <net/if.h>
#include <linux/if_ether.h>
#include <linux/if_packet.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <netinet/ip.h>
#include <linux/udp.h>
#include <CFileDescriptor.h>
#include <AsynchronousSocketCloseMonitor.h>

using Elastos::Core::CCloseGuard;
using Elastos::IO::CFileDescriptor;
using Libcore::IO::CIoBridge;

namespace Elastos {
namespace Net {

RawSocket::~RawSocket()
{
    // try {
    if (mGuard != NULL) {
        mGuard->WarnIfOpen();
    }
    Close();
    // } finally {
    //     super.finalize();
    // }
}

ECode RawSocket::Init(
    /* [in] */ const String& interfaceName,
    /* [in] */ Int16 protocolType)
{
    mMInterfaceName = interfaceName;
    mMProtocolType = protocolType;
    FAIL_RETURN(CFileDescriptor::New((IFileDescriptor**)&mFd));
    Create(mFd, mMProtocolType, mMInterfaceName);
    mGuard = CCloseGuard::Get();
    return mGuard->Open(String("close"));
}

ECode RawSocket::Read(
    /* [in] */ ArrayOf<Byte>* packet,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 byteCount,
    /* [in] */ Int32 destPort,
    /* [in] */ Int32 timeoutMillis,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number)

    if (packet == NULL) {
        // throw new NullPointerException("packet == null");
        return E_NULL_POINTER_EXCEPTION;
    }

    // Arrays.checkOffsetAndCount(packet.length, offset, byteCount);
    Int32 packetlen = packet->GetLength();
    if ((offset | byteCount) < 0 || offset > packetlen || packetlen - offset < byteCount) {
        // throw new ArrayIndexOutOfBoundsException(packetlen, offset, byteCount);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (destPort > 65535) {
        // throw new IllegalArgumentException("Port out of range: " + destPort);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    *number = RecvPacket(mFd, packet, offset, byteCount, destPort, timeoutMillis);
    return NOERROR;
}

ECode RawSocket::Write(
    /* [in] */ ArrayOf<Byte>* destMac,
    /* [in] */ ArrayOf<Byte>* packet,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 byteCount,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number)

    if (destMac == NULL) {
        // throw new NullPointerException("destMac == null");
        return E_NULL_POINTER_EXCEPTION;
    }

    if (packet == NULL) {
        // throw new NullPointerException("packet == null");
        return E_NULL_POINTER_EXCEPTION;
    }

    // Arrays.checkOffsetAndCount(packet.length, offset, byteCount);
    Int32 packetlen = packet->GetLength();
    if ((offset | byteCount) < 0 || offset > packetlen || packetlen - offset < byteCount) {
        // throw new ArrayIndexOutOfBoundsException(packetlen, offset, byteCount);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (destMac->GetLength() != 6) {
        // throw new IllegalArgumentException("MAC length must be 6: " + destMac.length);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    *number = SendPacket(mFd, mMInterfaceName, mMProtocolType, destMac, packet, offset, byteCount);
    return NOERROR;
}

ECode RawSocket::Close()
{
    mGuard->Close();
    Int32 intFd = 0;
    mFd->GetDescriptor(&intFd);
    return CIoBridge::_CloseSocket(intFd);
}

union sockunion {
    sockaddr sa;
    sockaddr_ll sll;
};

void RawSocket::Create(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int16 protocolType,
    /* [in] */ const String& interfaceName)
{
    if (interfaceName.IsNull()) {
        return;
    }

    sockunion su;
    memset(&su, 0, sizeof(su));
    su.sll.sll_family = PF_PACKET;
    su.sll.sll_protocol = htons(protocolType);
    su.sll.sll_ifindex = if_nametoindex(interfaceName.string());
    Int32 sock = socket(PF_PACKET, SOCK_DGRAM, htons(protocolType));

    if (sock == -1) {
        // ALOGE("Can't create socket %s", strerror(errno));
        printf("Can't create socket %s", strerror(errno));
        return;
    }

    // jniSetFileDescriptorOfFD(env, fileDescriptor, sock);
    fd->SetDescriptor(sock);

    assert(0 && "TODO");
    // if (!setBlocking(sock, FALSE)) {
    //     // ALOGE("Can't set non-blocking mode on socket %s", strerror(errno));
    //     printf("Can't set non-blocking mode on socket %s", strerror(errno));
    //     // jniThrowSocketException(env, errno);
    //     return;
    // }

    Int32 err = bind(sock, &su.sa, sizeof(su));
    if (err != 0) {
        // ALOGE("Socket bind error %s", strerror(errno));
        printf("Socket bind error %s", strerror(errno));
        // jniThrowSocketException(env, errno);
        return;
    }
}

#define NET_FAILURE_RETRY(fd, exp) ({               \
    typeof (exp) _rc;                               \
    do {                                            \
        _rc = (exp);                                \
        if (_rc == -1) {                            \
            if (fd == -1 || errno != EINTR) {       \
                break;                              \
            }                                       \
        }                                           \
    } while (_rc == -1);                            \
    _rc; })

Int32 RawSocket::SendPacket(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ const String& interfaceName,
    /* [in] */ Int16 protocolType,
    /* [in] */ ArrayOf<Byte>* destMac,
    /* [in] */ ArrayOf<Byte>* packet,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 byteCount)
{
    if (fd == NULL) {
        return 0;
    }

    if (interfaceName.IsNull()) {
        return 0;
    }

    if (packet == NULL) {
        return 0;
    }

    if (destMac == NULL) {
        return 0;
    }

    sockunion su;
    memset(&su, 0, sizeof(su));
    su.sll.sll_hatype = htons(1); // ARPHRD_ETHER
    su.sll.sll_halen = destMac->GetLength();
    memcpy(&su.sll.sll_addr, destMac, destMac->GetLength());
    su.sll.sll_family = AF_PACKET;
    su.sll.sll_protocol = htons(protocolType);
    su.sll.sll_ifindex = if_nametoindex(interfaceName.string());

    Int32 err;
    {
        Int32 intFd = (Int32)fd;
        AsynchronousSocketCloseMonitor monitor(intFd);
        err = NET_FAILURE_RETRY(fd, sendto(intFd, packet + offset,
            byteCount, 0, &su.sa, sizeof(su)));
    }

    return err;
}

Int32 RawSocket::RecvPacket(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ ArrayOf<Byte>* packet,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 byteCount,
    /* [in] */ Int32 destPort,
    /* [in] */ Int32 timeoutMillis)
{
    if (fd == -1) {
        return 0;
    }

    if (packet == NULL) {
        return 0;
    }

    packet += offset;

    pollfd fds[1];
    Int32 intFd = 0;
    fd->GetDescriptor(&intFd);
    fds[0].fd = intFd;
    fds[0].events = POLLIN;
    Int32 retval = poll(fds, 1, timeoutMillis);
    if (retval <= 0) {
        return 0;
    }

    UInt32 size = 0;
    {
        Int32 packetSize = byteCount;
        AsynchronousSocketCloseMonitor monitor(intFd);
        size = NET_FAILURE_RETRY(fd, read(intFd, packet, packetSize));
    }

    if (destPort != -1) {
        // quick check for UDP type & UDP port
        // the packet is an IP header, UDP header, and UDP payload
        if ((size < (sizeof(struct iphdr) + sizeof(struct udphdr)))) {
          return 0;  // runt packet
        }

        u_int8_t ip_proto = ((iphdr *) packet)->protocol;
        if (ip_proto != IPPROTO_UDP) {
          return 0;  // something other than UDP
        }

        __be16 port = htons((reinterpret_cast<udphdr*>(packet + sizeof(iphdr)))->dest);
        if (destPort != port) {
          return 0; // something other than requested port
        }
    }

    return size;
}

} // namespace Net
} // namespace Elastos
