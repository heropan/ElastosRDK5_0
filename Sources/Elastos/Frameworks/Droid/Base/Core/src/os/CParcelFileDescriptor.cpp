
#include "ext/frameworkext.h"
#include "os/CParcelFileDescriptor.h"

namespace Elastos {
namespace Droid {
namespace Os {

ECode CParcelFileDescriptor::constructor()
{
    return NOERROR;
}

ECode CParcelFileDescriptor::constructor(
    /* [in] */ IParcelFileDescriptor* descriptor)
{
    return ParcelFileDescriptor::Init(descriptor);
}

ECode CParcelFileDescriptor::constructor(
    /* [in] */ IFileDescriptor* descriptor)
{
    return ParcelFileDescriptor::Init(descriptor);
}

ECode CParcelFileDescriptor::Close()
{
    return ParcelFileDescriptor::Close();
}

ECode CParcelFileDescriptor::GetFileDescriptor(
    /* [out] */ IFileDescriptor** des)
{
    VALIDATE_NOT_NULL(des);
    return ParcelFileDescriptor::GetFileDescriptor(des);
}

ECode CParcelFileDescriptor::GetStatSize(
    /* [out] */ Int64* len)
{
    VALIDATE_NOT_NULL(len);
    return ParcelFileDescriptor::GetStatSize(len);
}

ECode CParcelFileDescriptor::SeekTo(
    /* [in] */ Int64 startOffset,
    /* [out] */ Int64* toOffset)
{
    VALIDATE_NOT_NULL(toOffset);
    return ParcelFileDescriptor::SeekTo(startOffset, toOffset);
}

ECode CParcelFileDescriptor::GetFd(
    /* [out] */ Int32* fd)
{
    VALIDATE_NOT_NULL(fd);
    return ParcelFileDescriptor::GetFd(fd);
}

ECode CParcelFileDescriptor::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    return ParcelFileDescriptor::ReadFromParcel(source);
}

ECode CParcelFileDescriptor::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return ParcelFileDescriptor::WriteToParcel(dest);
}

} // namespace Os
} // namespace Droid
} // namespace Elastos
