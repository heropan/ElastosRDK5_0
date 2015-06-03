
#include "DatagramSocketImpl.h"
#include "CIoBridge.h"

using Libcore::IO::IIoBridge;
using Libcore::IO::CIoBridge;

namespace Elastos {
namespace Net {

DatagramSocketImpl::DatagramSocketImpl()
    : mLocalPort(-1)
{}

ECode DatagramSocketImpl::GetFileDescriptor(
    /* [out] */ IFileDescriptor** fileDescriptor)
{
    VALIDATE_NOT_NULL(fileDescriptor);
    *fileDescriptor = mFd;
    REFCOUNT_ADD(*fileDescriptor);
    return NOERROR;
}

ECode DatagramSocketImpl::GetLocalAddress(
    /* [out] */ IInetAddress** address)
{
    VALIDATE_NOT_NULL(address);

    Int32 iFd;
    mFd->GetDescriptor(&iFd);
    return CIoBridge::_GetSocketLocalAddress(iFd, address);
}

ECode DatagramSocketImpl::GetLocalPort(
    /* [out] */ Int32* port)
{
    VALIDATE_NOT_NULL(port);
    *port = mLocalPort;
    return NOERROR;
}

ECode DatagramSocketImpl::Connect(
    /* [in] */ IInetAddress* inetAddr,
    /* [in] */ Int32 port)
{
    // do nothing
    return NOERROR;
}

ECode DatagramSocketImpl::Disconnect()
{
    // do nothing
    return NOERROR;
}

} // namespace Net
} // namespace Elastos
