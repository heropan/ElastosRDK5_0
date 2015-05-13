
#include "ext/frameworkext.h"
#include "content/res/CAssetFileDescriptor.h"
#include "os/CParcelFileDescriptor.h"
#include "os/CParcelFileDescriptorAutoCloseInputStream.h"
#include "os/CParcelFileDescriptorAutoCloseOutputStream.h"
#include "content/res/CAssetFileDescriptorAutoCloseInputStream.h"
#include "content/res/CAssetFileDescriptorAutoCloseOutputStream.h"

using Elastos::Droid::Os::IParcelFileDescriptorAutoCloseInputStream;
using Elastos::Droid::Os::IParcelFileDescriptorAutoCloseOutputStream;
using Elastos::Droid::Os::CParcelFileDescriptor;
using Elastos::Droid::Os::CParcelFileDescriptorAutoCloseInputStream;
using Elastos::Droid::Os::CParcelFileDescriptorAutoCloseOutputStream;
using Elastos::Droid::Content::Res::IAssetFileDescriptorAutoCloseInputStream;
using Elastos::Droid::Content::Res::IAssetFileDescriptorAutoCloseOutputStream;
using Elastos::Droid::Content::Res::CAssetFileDescriptorAutoCloseInputStream;
using Elastos::Droid::Content::Res::CAssetFileDescriptorAutoCloseOutputStream;

namespace Elastos {
namespace Droid {
namespace Content {
namespace Res {

CAssetFileDescriptor::CAssetFileDescriptor()
    : mStartOffset(0)
    , mLength(0)
{}

CAssetFileDescriptor::~CAssetFileDescriptor()
{}

ECode CAssetFileDescriptor::constructor()
{
    return NOERROR;
}

ECode CAssetFileDescriptor::constructor(
    /* [in] */ IParcelFileDescriptor * fd,
    /* [in] */ Int64 startOffset,
    /* [in] */ Int64 length)
{
    if (fd == NULL) {
        //throw new IllegalArgumentException("fd must not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (length < 0 && startOffset != 0) {
        //throw new IllegalArgumentException(
        //"startOffset must be 0 when using UNKNOWN_LENGTH");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mFd = fd;
    mStartOffset = startOffset;
    mLength = length;
    return NOERROR;
}

ECode CAssetFileDescriptor::GetParcelFileDescriptor(
    /* [out] */ IParcelFileDescriptor ** fd)
{
    VALIDATE_NOT_NULL(fd);
    *fd = mFd;
    INTERFACE_ADDREF(*fd);
    return NOERROR;
}

ECode CAssetFileDescriptor::GetFileDescriptor(
    /* [out] */ IFileDescriptor** fd)
{
    VALIDATE_NOT_NULL(fd);
    return mFd->GetFileDescriptor(fd);
}

ECode CAssetFileDescriptor::GetStartOffset(
    /* [out] */Int64 * startOffset)
{
    VALIDATE_NOT_NULL(startOffset);
    *startOffset = mStartOffset;
    return NOERROR;
}

ECode CAssetFileDescriptor::GetLength(
    /* [out] */Int64* length)
{
    VALIDATE_NOT_NULL(length);
    if (mLength >= 0) {
        *length = mLength;
        return NOERROR;
    }
    Int64 len = 0;
    mFd->GetStatSize(&len);
    *length = len >= 0 ? len : UNKNOWN_LENGTH;
    return NOERROR;
}

ECode CAssetFileDescriptor::GetDeclaredLength(
    /* [out] */Int64* length)
{
    VALIDATE_NOT_NULL(length)
    *length = mLength;
    return NOERROR;
}

ECode CAssetFileDescriptor::Close()
{
    return mFd->Close();
}

ECode CAssetFileDescriptor::CreateInputStream(
    /* [out] */ IFileInputStream** stream)
{
    VALIDATE_NOT_NULL(stream);
    if (mLength < 0) {
        AutoPtr<IParcelFileDescriptorAutoCloseInputStream> closeInputStream;
        FAIL_RETURN(CParcelFileDescriptorAutoCloseInputStream::New(mFd, (IParcelFileDescriptorAutoCloseInputStream**)&closeInputStream));
        *stream = IFileInputStream::Probe(closeInputStream);
        INTERFACE_ADDREF(*stream);
        return NOERROR;
    }
    AutoPtr<IAssetFileDescriptorAutoCloseInputStream> closeInputStream;
    FAIL_RETURN(CAssetFileDescriptorAutoCloseInputStream::New(this, (IAssetFileDescriptorAutoCloseInputStream**)&closeInputStream));
    *stream = IFileInputStream::Probe(closeInputStream);
    INTERFACE_ADDREF(*stream);
    return NOERROR;
}

ECode CAssetFileDescriptor::CreateOutputStream(
    /* [out] */ IFileOutputStream** stream)
{
    VALIDATE_NOT_NULL(stream);
    if (mLength < 0) {
        AutoPtr<IParcelFileDescriptorAutoCloseOutputStream> closeOutputStream;
        FAIL_RETURN(CParcelFileDescriptorAutoCloseOutputStream::New(mFd, (IParcelFileDescriptorAutoCloseOutputStream**)&closeOutputStream));
        *stream = IFileOutputStream::Probe(closeOutputStream);
        INTERFACE_ADDREF(*stream);
        return NOERROR;
    }
    AutoPtr<IAssetFileDescriptorAutoCloseOutputStream> closeOutputStream;
    FAIL_RETURN(CAssetFileDescriptorAutoCloseOutputStream::New(this, (IAssetFileDescriptorAutoCloseOutputStream**)&closeOutputStream));
    *stream = IFileOutputStream::Probe(closeOutputStream);
    INTERFACE_ADDREF(*stream);
    return NOERROR;
}

ECode CAssetFileDescriptor::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    FAIL_RETURN(CParcelFileDescriptor::New((IParcelFileDescriptor**)&mFd));
    IParcelable::Probe(mFd)->ReadFromParcel(source);
    FAIL_RETURN(source->ReadInt64(&mStartOffset));
    FAIL_RETURN(source->ReadInt64(&mLength));
    return NOERROR;
}

ECode CAssetFileDescriptor::WriteToParcel(
    /* [in] */ IParcel * dest)
{
    IParcelable::Probe(mFd)->WriteToParcel(dest);
    FAIL_RETURN(dest->WriteInt64(mStartOffset));
    FAIL_RETURN(dest->WriteInt64(mLength));
    return NOERROR;
}

} // namespace Res
} // namespace Content
} // namespace Droid
} // namespace Elastos
