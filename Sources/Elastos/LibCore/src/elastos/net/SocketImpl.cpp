
#include "SocketImpl.h"
#include "StringUtils.h"

using Elastos::Core::StringUtils;

namespace Elastos {
namespace Net {

// 95a22b32-2670-4e61-879d-35e0aa78c579
extern "C" const InterfaceID EIID_SocketImpl =
    { 0x95a22b32, 0x2670, 0x4e61, { 0x87, 0x9d, 0x35, 0xe0, 0xaa, 0x78, 0xc5, 0x79 } };

CAR_INTERFACE_IMPL_2(SocketImpl, Object, ISocketImpl, ISocketOptions)

SocketImpl::SocketImpl()
    : mStreaming(TRUE)
    , mPort(0)
    , mLocalport(0)
{
}

ECode SocketImpl::GetFD(
    /* [out] */ IFileDescriptor** fileDescriptor)
{
    VALIDATE_NOT_NULL(fileDescriptor)

    *fileDescriptor = mFd;
    REFCOUNT_ADD(*fileDescriptor)
    return NOERROR;
}

ECode SocketImpl::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)

    AutoPtr<IInetAddress> netadd;
    GetInetAddress((IInetAddress**)&netadd);
    String outstr;
    netadd->ToString(&outstr);
    *str = String("Socket[address=") + outstr +
            String(",port=") + StringUtils::Int32ToString(mPort) +
            String(",localPort=") + StringUtils::Int32ToString(mLocalport) + String("]");
    return NOERROR;
}

ECode SocketImpl::GetFileDescriptor(
    /* [out] */ IFileDescriptor** fileDescriptor)
{
    VALIDATE_NOT_NULL(fileDescriptor);

    *fileDescriptor = mFd;
    REFCOUNT_ADD(*fileDescriptor);
    return NOERROR;
}

ECode SocketImpl::GetInetAddress(
    /* [out] */ IInetAddress** netAddress)
{
    VALIDATE_NOT_NULL(netAddress);

    *netAddress = mAddress;
    REFCOUNT_ADD(*netAddress);
    return NOERROR;
}

ECode SocketImpl::GetLocalPort(
    /* [out] */ Int32* num)
{
    VALIDATE_NOT_NULL(num);

    *num = mLocalport;
    return NOERROR;
}

ECode SocketImpl::GetPort(
    /* [out] */ Int32* port)
{
    VALIDATE_NOT_NULL(port);

    *port = mPort;
    return NOERROR;
}

ECode SocketImpl::ShutdownInput()
{
//    throw new IOException("Method has not been implemented");
    return E_IO_EXCEPTION;
}

ECode SocketImpl::ShutdownOutput()
{
//    throw new IOException("Method has not been implemented");
    return E_IO_EXCEPTION;
}

ECode SocketImpl::SupportsUrgentData(
    /* [out] */ Boolean* isSupports)
{
    VALIDATE_NOT_NULL(isSupports);

    *isSupports = FALSE;
    return NOERROR;
}

ECode SocketImpl::SetPerformancePreferences(
    /* [in] */ Int32 connectionTime,
    /* [in] */ Int32 latency,
    /* [in] */ Int32 bandwidth)
{
    return NOERROR;
}

} // namespace Net
} // namespace Elastos
