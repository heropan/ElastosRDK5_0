
#include "ext/frameworkext.h"
#include "os/CParcelFileDescriptorAutoCloseInputStream.h"

using Elastos::IO::IFileInputStream;
using Elastos::IO::EIID_IFileInputStream;

namespace Elastos {
namespace Droid {
namespace Os {

CParcelFileDescriptorAutoCloseInputStream::CParcelFileDescriptorAutoCloseInputStream()
{}

ECode CParcelFileDescriptorAutoCloseInputStream::constructor(
    /* [in] */ IParcelFileDescriptor* fd)
{
    return ParcelFileDescriptor::AutoCloseInputStream::Init(fd);
}

ECode CParcelFileDescriptorAutoCloseInputStream::Close()
{
    return ParcelFileDescriptor::AutoCloseInputStream::Close();
}

ECode CParcelFileDescriptorAutoCloseInputStream::GetFD(
    /* [out] */ IFileDescriptor** fd)
{
    VALIDATE_NOT_NULL(fd);
    return ParcelFileDescriptor::AutoCloseInputStream::GetFD(fd);
}

ECode CParcelFileDescriptorAutoCloseInputStream::Available(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    return ParcelFileDescriptor::AutoCloseInputStream::Available(number);
}

ECode CParcelFileDescriptorAutoCloseInputStream::Mark(
    /* [in] */ Int32 readLimit)
{
    return ParcelFileDescriptor::AutoCloseInputStream::Mark(readLimit);
}

ECode CParcelFileDescriptorAutoCloseInputStream::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    VALIDATE_NOT_NULL(supported);
    return ParcelFileDescriptor::AutoCloseInputStream::IsMarkSupported(supported);
}

ECode CParcelFileDescriptorAutoCloseInputStream::Read(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    return ParcelFileDescriptor::AutoCloseInputStream::Read(value);
}

ECode CParcelFileDescriptorAutoCloseInputStream::ReadBytes(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    return ParcelFileDescriptor::AutoCloseInputStream::ReadBytes(buffer,number);
}

ECode CParcelFileDescriptorAutoCloseInputStream::ReadBytesEx(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    return ParcelFileDescriptor::AutoCloseInputStream::ReadBytesEx(buffer, offset, length, number);
}

ECode CParcelFileDescriptorAutoCloseInputStream::Reset()
{
    return ParcelFileDescriptor::AutoCloseInputStream::Reset();
}

ECode CParcelFileDescriptorAutoCloseInputStream::Skip(
    /* [in] */ Int64 byteCount,
    /* [out] */ Int64* number)
{
    VALIDATE_NOT_NULL(number);
    return ParcelFileDescriptor::AutoCloseInputStream::Skip(byteCount, number);
}

PInterface CParcelFileDescriptorAutoCloseInputStream::Probe(
    /* [in] */ REIID riid)
{
    return _CParcelFileDescriptorAutoCloseInputStream::Probe(riid);
}

ECode CParcelFileDescriptorAutoCloseInputStream::GetLock(
    /* [out] */ IInterface** lockobj)
{
    VALIDATE_NOT_NULL(lockobj);

    AutoPtr<IInterface> obj = ParcelFileDescriptor::AutoCloseInputStream::GetLock();
    *lockobj = obj;
    INTERFACE_ADDREF(*lockobj);
    return NOERROR;
}

} // namespace Os
} // namespace Droid
} // namespace Elastos
