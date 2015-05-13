
#include "ext/frameworkext.h"
#include "content/res/CAssetFileDescriptorAutoCloseOutputStream.h"
#include "elastos/Slogger.h"

using Elastos::Droid::Os::IParcelFileDescriptor;
using Elastos::Utility::Logging::Slogger;

namespace Elastos {
namespace Droid {
namespace Content {
namespace Res {

CAssetFileDescriptorAutoCloseOutputStream::CAssetFileDescriptorAutoCloseOutputStream()
    : mRemaining(0)
{}

ECode CAssetFileDescriptorAutoCloseOutputStream::constructor(
    /* [in] */ IAssetFileDescriptor* fd)
{
    AutoPtr<IParcelFileDescriptor> pfd;
    fd->GetParcelFileDescriptor((IParcelFileDescriptor**)&pfd);
    ParcelFileDescriptor::AutoCloseOutputStream::Init(pfd);

    Int64 startOffset = 0;
    fd->GetStartOffset(&startOffset);
    Int64 toOffset = 0;
    FAIL_RETURN(pfd->SeekTo(startOffset, &toOffset));
    if (toOffset < 0) {
        //throw new IOException("Unable to seek");
        return E_IO_EXCEPTION;
    }

    fd->GetLength(&mRemaining);
    return NOERROR;
}

ECode CAssetFileDescriptorAutoCloseOutputStream::WriteBytesEx(
    /* [in] */ const ArrayOf<Byte>& buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count)
{
    if (mRemaining >= 0) {
        if (mRemaining == 0) return NOERROR;
        if (count > mRemaining) count = (Int32)mRemaining;
        FAIL_RETURN(ParcelFileDescriptor::AutoCloseOutputStream::WriteBytesEx(buffer, offset, count));
        mRemaining -= count;
        return NOERROR;
    }

    return ParcelFileDescriptor::AutoCloseOutputStream::WriteBytesEx(buffer, offset, count);
}

ECode CAssetFileDescriptorAutoCloseOutputStream::WriteBytes(
    /* [in] */ const ArrayOf<Byte>& buffer)
{
    if (mRemaining >= 0) {
        if (mRemaining == 0) return NOERROR;
        Int32 count = buffer.GetLength();
        if (count > mRemaining) count = (Int32)mRemaining;
        FAIL_RETURN(ParcelFileDescriptor::AutoCloseOutputStream::WriteBytes(buffer));
        mRemaining -= count;
        return NOERROR;
    }

    return ParcelFileDescriptor::AutoCloseOutputStream::WriteBytes(buffer);
}

ECode CAssetFileDescriptorAutoCloseOutputStream::Write(
    /* [in] */ Int32 oneByte)
{
    if (mRemaining >= 0) {
        if (mRemaining == 0) return NOERROR;
        FAIL_RETURN(ParcelFileDescriptor::AutoCloseOutputStream::Write(oneByte));
        mRemaining--;
        return NOERROR;
    }

    return ParcelFileDescriptor::AutoCloseOutputStream::Write(oneByte);
}

ECode CAssetFileDescriptorAutoCloseOutputStream::Flush()
{
    return ParcelFileDescriptor::AutoCloseOutputStream::Flush();
}

ECode CAssetFileDescriptorAutoCloseOutputStream::Close()
{
    return ParcelFileDescriptor::AutoCloseOutputStream::Close();
}

ECode CAssetFileDescriptorAutoCloseOutputStream::CheckError(
    /* [out] */ Boolean* hasError)
{
    VALIDATE_NOT_NULL(hasError);

    return ParcelFileDescriptor::AutoCloseOutputStream::CheckError(hasError);
}

ECode CAssetFileDescriptorAutoCloseOutputStream::GetFD(
    /* [out] */ IFileDescriptor** fd)
{
    VALIDATE_NOT_NULL(fd);

    return ParcelFileDescriptor::AutoCloseOutputStream::GetFD(fd);
}

PInterface CAssetFileDescriptorAutoCloseOutputStream::Probe(
    /* [in] */ REIID riid)
{
    return _CAssetFileDescriptorAutoCloseOutputStream::Probe(riid);
}

ECode CAssetFileDescriptorAutoCloseOutputStream::GetLock(
    /* [out] */ IInterface** lockobj)
{
    VALIDATE_NOT_NULL(lockobj);

    AutoPtr<IInterface> obj = ParcelFileDescriptor::AutoCloseOutputStream::GetLock();
    *lockobj = obj;
    INTERFACE_ADDREF(*lockobj);
    return NOERROR;
}

}
}
}
}
