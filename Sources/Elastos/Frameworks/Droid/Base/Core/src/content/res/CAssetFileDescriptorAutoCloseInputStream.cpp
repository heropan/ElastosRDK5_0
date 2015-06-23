
#include "ext/frameworkext.h"
#include "content/res/CAssetFileDescriptorAutoCloseInputStream.h"

using Elastos::Droid::Os::IParcelFileDescriptor;

namespace Elastos {
namespace Droid {
namespace Content {
namespace Res {

CAssetFileDescriptorAutoCloseInputStream::CAssetFileDescriptorAutoCloseInputStream()
    : mRemaining(0)
{}

ECode CAssetFileDescriptorAutoCloseInputStream::constructor(
    /* [in] */ IAssetFileDescriptor* fd)
{
    AutoPtr<IParcelFileDescriptor> pfd;
    fd->GetParcelFileDescriptor((IParcelFileDescriptor**)&pfd);
    FAIL_RETURN(ParcelFileDescriptor::AutoCloseInputStream::Init(pfd));

    Int64 startOffset = 0;
    fd->GetStartOffset(&startOffset);
    Int64 result;
    FAIL_RETURN(ParcelFileDescriptor::AutoCloseInputStream::Skip(startOffset, &result));
    fd->GetLength(&mRemaining);
    return NOERROR;
}

ECode CAssetFileDescriptorAutoCloseInputStream::Available(
    /* [out] */ Int32 * able)
{
    VALIDATE_NOT_NULL(able);

    if (mRemaining >= 0) {
        *able = mRemaining < 0x7fffffff ? (Int32)mRemaining : 0x7fffffff;
    }
    else {
        ParcelFileDescriptor::AutoCloseInputStream::Available(able);
    }
    return NOERROR;
}

ECode CAssetFileDescriptorAutoCloseInputStream::Read(
    /* [out] */ Int32 * result)
{
    VALIDATE_NOT_NULL(result);

    Int32 res = 0;
    AutoPtr< ArrayOf<Byte> > buffer = ArrayOf<Byte> ::Alloc(1);
    FAIL_RETURN(ReadBytes(buffer, 0, 1, &res));
    *result = res == -1 ? -1 : (*buffer)[0] & 0xff;
    return NOERROR;
}

ECode CAssetFileDescriptorAutoCloseInputStream::ReadBytes(
    /* [in] */ ArrayOf<Byte> * buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [out] */ Int32 * result)
{
    VALIDATE_NOT_NULL(result);

    if (mRemaining >= 0){
        if (mRemaining == 0) {
            *result = -1;
            return NOERROR;
        }
        if (count > mRemaining) {
            count = (Int32)mRemaining;
        }
        FAIL_RETURN(ParcelFileDescriptor::AutoCloseInputStream::ReadBytes(buffer, offset, count, result));
        if (*result >= 0) {
            mRemaining -= *result;
        }
        return NOERROR;
    }

    return ParcelFileDescriptor::AutoCloseInputStream::ReadBytes(buffer, offset, count, result);
}

ECode CAssetFileDescriptorAutoCloseInputStream::ReadBytes(
    /* [in] */ ArrayOf<Byte> * buffer,
    /* [out] */ Int32 * result)
{
    VALIDATE_NOT_NULL(result);

    return ReadBytes(buffer, 0, buffer->GetLength(), result);
}

ECode CAssetFileDescriptorAutoCloseInputStream::Skip(
    /* [in] */ Int64 count,
    /* [out] */ Int64 * result)
{
    VALIDATE_NOT_NULL(result);

    if (mRemaining >= 0) {
        if (mRemaining == 0) {
            *result = -1;
            return NOERROR;
        }
        if (count > mRemaining) {
            count = mRemaining;
        }
        FAIL_RETURN(ParcelFileDescriptor::AutoCloseInputStream::Skip(count, result));
        if (*result >= 0) {
            mRemaining -= *result;
        }
        return NOERROR;
    }

    return ParcelFileDescriptor::AutoCloseInputStream::Skip(count, result);
}

ECode CAssetFileDescriptorAutoCloseInputStream::Mark(
    /* [in] */ Int32 readlimit)
{
    if (mRemaining >= 0) {
        // Not supported.
        return NOERROR;
    }
    return ParcelFileDescriptor::AutoCloseInputStream::Mark(readlimit);
}

ECode CAssetFileDescriptorAutoCloseInputStream::IsMarkSupported(
    /* [out] */ Boolean * result)
{
    VALIDATE_NOT_NULL(result);

    if (mRemaining >= 0) {
        *result = FALSE;
        return NOERROR;
    }
    return ParcelFileDescriptor::AutoCloseInputStream::IsMarkSupported(result);
}

ECode CAssetFileDescriptorAutoCloseInputStream::Reset()
{
    Object::Autolock lock(mLock);

    if (mRemaining >= 0) {
        // Not supported.
        return NOERROR;
    }
    return ParcelFileDescriptor::AutoCloseInputStream::Reset();
}

ECode CAssetFileDescriptorAutoCloseInputStream::Close()
{
    return ParcelFileDescriptor::AutoCloseInputStream::Close();
}

ECode CAssetFileDescriptorAutoCloseInputStream::GetFD(
    /* [out] */ IFileDescriptor** fd)
{
    VALIDATE_NOT_NULL(fd);

    return ParcelFileDescriptor::AutoCloseInputStream::GetFD(fd);
}

PInterface CAssetFileDescriptorAutoCloseInputStream::Probe(
    /* [in] */ REIID riid)
{
    return _CAssetFileDescriptorAutoCloseInputStream::Probe(riid);
}

ECode CAssetFileDescriptorAutoCloseInputStream::GetLock(
    /* [out] */ IInterface** lockobj)
{
    VALIDATE_NOT_NULL(lockobj);

    AutoPtr<IInterface> obj = ParcelFileDescriptor::AutoCloseInputStream::GetLock();
    *lockobj = obj;
    REFCOUNT_ADD(*lockobj);
    return NOERROR;
}

}
}
}
}
