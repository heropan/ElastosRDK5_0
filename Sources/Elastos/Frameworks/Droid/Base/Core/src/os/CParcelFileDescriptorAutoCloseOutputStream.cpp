
#include "ext/frameworkext.h"
#include "os/CParcelFileDescriptorAutoCloseOutputStream.h"

using Elastos::IO::IFileOutputStream;
using Elastos::IO::EIID_IFileOutputStream;

namespace Elastos {
namespace Droid {
namespace Os {

CParcelFileDescriptorAutoCloseOutputStream::CParcelFileDescriptorAutoCloseOutputStream()
{}

ECode CParcelFileDescriptorAutoCloseOutputStream::constructor(
    /* [in] */ IParcelFileDescriptor* pfd)
{
    return ParcelFileDescriptor::AutoCloseOutputStream::Init(pfd);
}

ECode CParcelFileDescriptorAutoCloseOutputStream::Flush()
{
    return ParcelFileDescriptor::AutoCloseOutputStream::Flush();
}

ECode CParcelFileDescriptorAutoCloseOutputStream::Close()
{
    return ParcelFileDescriptor::AutoCloseOutputStream::Close();
}

ECode CParcelFileDescriptorAutoCloseOutputStream::GetFD(
    /* [out] */ IFileDescriptor** fd)
{
    VALIDATE_NOT_NULL(fd);
    return ParcelFileDescriptor::AutoCloseOutputStream::GetFD(fd);
}

ECode CParcelFileDescriptorAutoCloseOutputStream::Write(
    /* [in] */ Int32 oneByte)
{
    return ParcelFileDescriptor::AutoCloseOutputStream::Write(oneByte);
}

ECode CParcelFileDescriptorAutoCloseOutputStream::WriteBytes(
    /* [in] */ const ArrayOf<Byte>& buffer)
{
    return ParcelFileDescriptor::AutoCloseOutputStream::WriteBytes(buffer);
}

ECode CParcelFileDescriptorAutoCloseOutputStream::WriteBytes(
    /* [in] */ const ArrayOf<Byte>& buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count)
{
    return ParcelFileDescriptor::AutoCloseOutputStream::WriteBytes(buffer, offset, count);
}

ECode CParcelFileDescriptorAutoCloseOutputStream::CheckError(
    /* [out] */ Boolean* hasError)
{
    VALIDATE_NOT_NULL(hasError);
    return ParcelFileDescriptor::AutoCloseOutputStream::CheckError(hasError);
}

PInterface CParcelFileDescriptorAutoCloseOutputStream::Probe(
    /* [in] */ REIID riid)
{
    return _CParcelFileDescriptorAutoCloseOutputStream::Probe(riid);
}

ECode CParcelFileDescriptorAutoCloseOutputStream::GetLock(
    /* [out] */ IInterface** lockobj)
{
    VALIDATE_NOT_NULL(lockobj);

    AutoPtr<IInterface> obj = ParcelFileDescriptor::AutoCloseOutputStream::GetLock();
    *lockobj = obj;
    REFCOUNT_ADD(*lockobj);
    return NOERROR;
}

} // namespace Os
} // namespace Droid
} // namespace Elastos
