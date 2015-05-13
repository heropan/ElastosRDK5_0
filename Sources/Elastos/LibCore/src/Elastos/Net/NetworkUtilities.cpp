
#include "NetworkUtilities.h"

ECode ByteArrayToSocketAddress(const ArrayOf<Byte>& byteArray, Int32 port, sockaddr_storage* ss)
{
    // Convert the IP address bytes to the proper IP address type.
    size_t addressLength = byteArray.GetLength();
    memset(ss, 0, sizeof(*ss));
    if (addressLength == 4) {
        // IPv4 address.
        sockaddr_in* sin = reinterpret_cast<sockaddr_in*>(ss);
        sin->sin_family = AF_INET;
        sin->sin_port = htons(port);
        Byte* dst = reinterpret_cast<Byte*>(&sin->sin_addr.s_addr);
        memcpy(dst, byteArray.GetPayload(), 4 * sizeof(Byte));
    }
    else if (addressLength == 16) {
        // IPv6 address.
        sockaddr_in6* sin6 = reinterpret_cast<sockaddr_in6*>(ss);
        sin6->sin6_family = AF_INET6;
        sin6->sin6_port = htons(port);
        Byte* dst = reinterpret_cast<Byte*>(&sin6->sin6_addr.s6_addr);
        memcpy(dst, byteArray.GetPayload(), 16 * sizeof(Byte));
    }
    else {
        // We can't throw SocketException. We aren't meant to see bad addresses, so seeing one
        // really does imply an internal error.
        // TODO: fix the code (native and Java) so we don't paint ourselves into this corner.
//        char buf[64];
//        snprintf(buf, sizeof(buf), "byteArrayToSocketAddress bad array length (%i)", addressLength);
//        jniThrowException(env, "java/lang/IllegalArgumentException", buf);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

ECode SocketAddressToByteArray(sockaddr_storage* ss, ArrayOf<Byte>* ipAddress)
{
    void* rawAddress;
    size_t addressLength;
    if (ss->ss_family == AF_INET) {
        sockaddr_in* sin = reinterpret_cast<sockaddr_in*>(ss);
        rawAddress = &sin->sin_addr.s_addr;
        addressLength = 4;
    }
    else if (ss->ss_family == AF_INET6) {
        sockaddr_in6* sin6 = reinterpret_cast<sockaddr_in6*>(ss);
        rawAddress = &sin6->sin6_addr.s6_addr;
        addressLength = 16;
    }
    else {
        // We can't throw SocketException. We aren't meant to see bad addresses, so seeing one
        // really does imply an internal error.
        // TODO: fix the code (native and Java) so we don't paint ourselves into this corner.
//        char buf[64];
//        snprintf(buf, sizeof(buf), "socketAddressToByteArray bad ss_family (%i)", ss->ss_family);
//        jniThrowException(env, "java/lang/IllegalArgumentException", buf);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (ipAddress->GetLength() < addressLength) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    memcpy(ipAddress->GetPayload(), rawAddress, addressLength);
    return NOERROR;
}
