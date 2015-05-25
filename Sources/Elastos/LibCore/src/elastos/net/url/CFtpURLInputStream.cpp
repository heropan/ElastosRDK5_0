
#include "CFtpURLInputStream.h"
#include "cmdef.h"

namespace Elastos {
namespace Net {
namespace Url {

ECode CFtpURLInputStream::constructor(
    /* [in] */ IInputStream* is,
    /* [in] */ ISocket* controlSocket)
{
    return FtpURLInputStream::Init(is, controlSocket);
}

ECode CFtpURLInputStream::Available(
    /* [out] */ Int32* number)
{
    return FtpURLInputStream::Available(number);
}

ECode CFtpURLInputStream::Close()
{
    return FtpURLInputStream::Close();
}

ECode CFtpURLInputStream::Mark(
    /* [in] */ Int32 readLimit)
{
    return FtpURLInputStream::Mark(readLimit);
}

ECode CFtpURLInputStream::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    return FtpURLInputStream::IsMarkSupported(supported);
}

ECode CFtpURLInputStream::Read(
    /* [out] */ Int32* value)
{
    return FtpURLInputStream::Read(value);
}

ECode CFtpURLInputStream::ReadBytes(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    return FtpURLInputStream::ReadBytes(buffer, number);
}

ECode CFtpURLInputStream::ReadBytesEx(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ Int32* number)
{
    return FtpURLInputStream::ReadBytesEx(buffer, offset, length, number);
}

ECode CFtpURLInputStream::Reset()
{
    return FtpURLInputStream::Reset();
}

ECode CFtpURLInputStream::Skip(
    /* [in] */ Int64 byteCount,
    /* [out] */ Int64* number)
{
    return FtpURLInputStream::Skip(byteCount, number);
}

PInterface CFtpURLInputStream::Probe(
    /* [in] */ REIID riid)
{
    return _CFtpURLInputStream::Probe(riid);
}

ECode CFtpURLInputStream::GetLock(
    /* [out] */ IInterface** lockobj)
{
    VALIDATE_NOT_NULL(lockobj);

    AutoPtr<IInterface> obj = FtpURLInputStream::GetLock();
    *lockobj = obj;
    INTERFACE_ADDREF(*lockobj);
    return NOERROR;
}

} // namespace Url
} // namespace Net
} // namespace Elastos
