
#include "cmdef.h"
#include "CByteBufferHelper.h"
#include "ByteBuffer.h"

namespace Elastos {
namespace IO {

ECode CByteBufferHelper::Allocate(
    /* [in] */ Int32 capacity,
    /* [out] */ IByteBuffer** byteBuf)
{
    return ByteBuffer::Allocate(capacity, byteBuf);
}

ECode CByteBufferHelper::AllocateDirect(
    /* [in] */ Int32 capacity,
    /* [out] */ IByteBuffer** byteBuf)
{
    return ByteBuffer::AllocateDirect(capacity, byteBuf);
}

ECode CByteBufferHelper::WrapArray(
    /* [in] */ ArrayOf<Byte>* array,
    /* [out] */ IByteBuffer** byteBuf)
{
    return ByteBuffer::WrapArray(array, byteBuf);
}

ECode CByteBufferHelper::WrapArrayEx(
    /* [in] */ ArrayOf<Byte>* array,
    /* [in] */ Int32 start,
    /* [in] */ Int32 byteCount,
    /* [out] */ IByteBuffer** byteBuf)
{
    return ByteBuffer::WrapArrayEx(array, start, byteCount, byteBuf);
}

} // namespace IO
} // namespace Elastos
