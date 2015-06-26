
#include "CNioUtilsHelper.h"
#include "NioUtils.h"

namespace Elastos {
namespace IO {

CAR_INTERFACE_IMPL(CNioUtilsHelper, Singleton, INioUtilsHelper)

CAR_SINGLETON_IMPL(CNioUtilsHelper)

ECode CNioUtilsHelper::FreeDirectBuffer(
    /* [in] */ IByteBuffer* buffer)
{
    return NioUtils::FreeDirectBuffer(buffer);
}

ECode CNioUtilsHelper::GetFD(
    /* [in] */ IFileChannel* fc,
    /* [out] */ IFileDescriptor** outfd)
{
    VALIDATE_NOT_NULL(outfd)
    AutoPtr<IFileDescriptor> fd = NioUtils::GetFD(fc);
    *outfd = fd;
    REFCOUNT_ADD(*outfd)
    return NOERROR;
}

ECode CNioUtilsHelper::NewFileChannel(
    /* [in] */ ICloseable* ioObject,
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int32 mode,
    /* [out] */ IFileChannel** outchannel)
{
    VALIDATE_NOT_NULL(outchannel)

    AutoPtr<IFileChannel> res = NioUtils::NewFileChannel(ioObject, fd, mode);
    *outchannel = res;
    REFCOUNT_ADD(*outchannel)
    return NOERROR;
}

ECode CNioUtilsHelper::GetUnsafeArray(
    /* [in] */ IByteBuffer* b,
    /* [out, callee] */ ArrayOf<Byte>** outbyte)
{
    VALIDATE_NOT_NULL(outbyte)

    AutoPtr< ArrayOf<Byte> > res = NioUtils::GetUnsafeArray(b);
    *outbyte = res;
    REFCOUNT_ADD(*outbyte)
    return NOERROR;
}

ECode CNioUtilsHelper::GetUnsafeArrayOffset(
    /* [in] */ IByteBuffer* b,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)

    *value = NioUtils::GetUnsafeArrayOffset(b);
    return NOERROR;
}

} // namespace IO
} // namespace Elastos
