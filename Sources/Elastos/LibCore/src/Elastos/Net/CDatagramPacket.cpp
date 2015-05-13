
#include "cmdef.h"
#include "CDatagramPacket.h"
#include "CInetSocketAddress.h"

namespace Elastos {
namespace Net {

CDatagramPacket::CDatagramPacket()
    : mLength(0)
    , mUserSuppliedLength(0)
    , mPort(-1)
    , mOffset(0)
{}

CDatagramPacket::~CDatagramPacket()
{
}

ECode CDatagramPacket::constructor(
    /* [in] */ ArrayOf<Byte>* data,
    /* [in] */ Int32 length)
{
    return constructor(data, 0, length);;
}

ECode CDatagramPacket::constructor(
    /* [in] */ ArrayOf<Byte>* data,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length)
{
    return SetDataEx(data, offset, length);
}

ECode CDatagramPacket::constructor(
    /* [in] */ ArrayOf<Byte>* data,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [in] */ IInetAddress* host,
    /* [in] */ Int32 aPort)
{
    FAIL_RETURN(constructor(data, offset, length));
    SetPort(aPort);
    mAddress = host;
    return NOERROR;
}

ECode CDatagramPacket::constructor(
    /* [in] */ ArrayOf<Byte>* data,
    /* [in] */ Int32 length,
    /* [in] */ IInetAddress* host,
    /* [in] */ Int32 port)
{
    return constructor(data, 0, length, host, port);
}

ECode CDatagramPacket::constructor(
    /* [in] */ ArrayOf<Byte>* data,
    /* [in] */ Int32 length,
    /* [in] */ ISocketAddress* sockAddr)
{
    FAIL_RETURN(constructor(data, 0, length));
    return SetSocketAddress(sockAddr);
}

ECode CDatagramPacket::constructor(
    /* [in] */ ArrayOf<Byte>* data,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [in] */ ISocketAddress* sockAddr)
{
    FAIL_RETURN(constructor(data, offset, length));
    return SetSocketAddress(sockAddr);
}

ECode CDatagramPacket::GetAddress(
    /* [out] */ IInetAddress** address)
{
    VALIDATE_NOT_NULL(address);
    Mutex::Autolock lock(_m_syncLock);

    *address = mAddress;
    INTERFACE_ADDREF(*address);
    return NOERROR;
}

ECode CDatagramPacket::GetData(
    /* [out, callee] */ ArrayOf<Byte>** data)
{
    VALIDATE_NOT_NULL(data);
    Mutex::Autolock lock(_m_syncLock);

    *data = mData;
    INTERFACE_ADDREF(*data);
    return NOERROR;
}

ECode CDatagramPacket::GetLength(
    /* [out] */ Int32* length)
{
    VALIDATE_NOT_NULL(length);
    Mutex::Autolock lock(_m_syncLock);

    *length = mLength;
    return NOERROR;
}

ECode CDatagramPacket::GetOffset(
    /* [out] */ Int32* offset)
{
    VALIDATE_NOT_NULL(offset);
    Mutex::Autolock lock(_m_syncLock);

    *offset = mOffset;
    return NOERROR;
}

ECode CDatagramPacket::ResetLengthForReceive()
{
    mLength = mUserSuppliedLength;
    return NOERROR;
}

ECode CDatagramPacket::SetReceivedLength(
    /* [in] */ Int32 length)
{
    mLength = length;
    return NOERROR;
}

ECode CDatagramPacket::GetPort(
    /* [out] */ Int32* port)
{
    VALIDATE_NOT_NULL(port);
    Mutex::Autolock lock(_m_syncLock);

    *port = mPort;
    return NOERROR;
}

ECode CDatagramPacket::SetAddress(
    /* [in] */ IInetAddress* addr)
{
    Mutex::Autolock lock(_m_syncLock);

    mAddress = addr;
    return NOERROR;
}

ECode CDatagramPacket::SetDataEx(
    /* [in] */ ArrayOf<Byte>* buf,
    /* [in] */ Int32 anOffset,
    /* [in] */ Int32 aLength)
{
    Mutex::Autolock lock(_m_syncLock);

    if (0 > anOffset || anOffset > buf->GetLength() || 0 > aLength
            || aLength > buf->GetLength() - anOffset) {
//        throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    mData = buf;
    mOffset = anOffset;
    mLength = aLength;
    mUserSuppliedLength = aLength;
    return NOERROR;
}

ECode CDatagramPacket::SetData(
    /* [in] */ ArrayOf<Byte>* buf)
{
    Mutex::Autolock lock(_m_syncLock);

    mLength = buf->GetLength(); // This will check for null
    mUserSuppliedLength = mLength;
    mData = buf;
    mOffset = 0;
    return NOERROR;
}

ECode CDatagramPacket::SetLength(
    /* [in] */ Int32 len)
{
    Mutex::Autolock lock(_m_syncLock);

    if (0 > len || mOffset + len > mData->GetLength()) {
//        throw new IndexOutOfBoundsException();
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    mLength = len;
    mUserSuppliedLength = len;
    return NOERROR;
}

ECode CDatagramPacket::SetPort(
    /* [in] */ Int32 aPort)
{
    Mutex::Autolock lock(_m_syncLock);

    if (aPort < 0 || aPort > 65535) {
//        throw new IllegalArgumentException("Port out of range: " + aPort);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mPort = aPort;
    return NOERROR;
}

ECode CDatagramPacket::GetSocketAddress(
    /* [out] */ ISocketAddress** sockAddr)
{
    VALIDATE_NOT_NULL(sockAddr);
    Mutex::Autolock lock(_m_syncLock);

    AutoPtr<IInetAddress> addr;
    GetAddress((IInetAddress**)&addr);
    Int32 port;
    GetPort(&port);
    AutoPtr<IInetSocketAddress> sa;
    FAIL_RETURN(CInetSocketAddress::New(addr, port, (IInetSocketAddress**)&sa));
    *sockAddr = ISocketAddress::Probe(sa);
    INTERFACE_ADDREF(*sockAddr);
    return NOERROR;
}

ECode CDatagramPacket::SetSocketAddress(
    /* [in] */ ISocketAddress* sockAddr)
{
    Mutex::Autolock lock(_m_syncLock);

    if (sockAddr == NULL || IInetSocketAddress::Probe(sockAddr) == NULL) {
//        throw new IllegalArgumentException("Socket address not an InetSocketAddress: " +
//                (sockAddr == null ? null : sockAddr.getClass()));
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    IInetSocketAddress* inetAddr = IInetSocketAddress::Probe(sockAddr);
    Boolean isUnresolved = FALSE;
    inetAddr->IsUnresolved(&isUnresolved);
    if (isUnresolved) {
        // throw new IllegalArgumentException("Socket address unresolved: " + sockAddr);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    inetAddr->GetPort(&mPort);
    mAddress = NULL;
    inetAddr->GetAddress((IInetAddress**)&mAddress);
    return NOERROR;
}

} // namespace Net
} // namespace Elastos
