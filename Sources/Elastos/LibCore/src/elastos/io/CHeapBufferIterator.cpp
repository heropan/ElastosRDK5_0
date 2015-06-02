
#include "coredef.h"
#include "CHeapBufferIterator.h"
#include "Memory.h"

namespace Elastos {
namespace IO {

ECode CHeapBufferIterator::constructor(
    /* [in/out] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 byteCount,
    /* [in] */ ByteOrder order)
{
    mBuffer = buffer;
    mOffset = offset;
    mByteCount = byteCount;
    mOrder = order;
    mPosition = 0;
    return NOERROR;
}

ECode CHeapBufferIterator::Seek(
    /* [in] */ Int32 offset)
{
    mPosition = offset;
    return NOERROR;
}

ECode CHeapBufferIterator::Skip(
    /* [in] */ Int32 byteCount)
{
    mPosition += byteCount;
    return NOERROR;
}

ECode CHeapBufferIterator::ReadByteArray(
    /* [out] */ ArrayOf<Byte>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 byteCount)
{
    VALIDATE_NOT_NULL(assert)
    dst->Copy(dstOffset, mBuffer, mOffset + mPosition, byteCount);
    mPosition += byteCount;
    return NOERROR;
}

ECode CHeapBufferIterator::ReadByte(
    /* [out] */ Byte* result)
{
    VALIDATE_NOT_NULL(result);
    *result = (*mBuffer)[mOffset + mPosition];
    ++mPosition;
    return NOERROR;
}

ECode CHeapBufferIterator::ReadInt32(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = Memory::PeekInt32(*mBuffer, mOffset + mPosition, mOrder);
    mPosition += sizeof(Int32);
    return NOERROR;
}

ECode CHeapBufferIterator::ReadInt32Array(
    /* [out] */ ArrayOf<Int32>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 intCount)
{
    VALIDATE_NOT_NULL(dst)
    Int32 byteCount = intCount * sizeof(Int32);
    // TODO:
    Memory::UnsafeBulkGet((Byte*)dst->GetPayload(), dstOffset, byteCount,
            *mBuffer, mOffset + mPosition, sizeof(Int32), FALSE/*needsSwap*/);
    mPosition += byteCount;
    return NOERROR;
}

ECode CHeapBufferIterator::ReadInt16(
    /* [out] */ Int16* result)
{
    VALIDATE_NOT_NULL(result);
    *result = Memory::PeekInt16(*mBuffer, mOffset + mPosition, mOrder);
    mPosition += sizeof(Int16);
    return NOERROR;
}

} // namespace IO
} // namespace Elastos
