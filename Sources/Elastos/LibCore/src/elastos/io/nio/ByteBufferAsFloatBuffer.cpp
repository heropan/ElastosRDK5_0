
#include "ByteBufferAsFloatBuffer.h"

namespace Elastos {
namespace IO {

// {37165a91-5ade-4b46-93e6-ab451eeb9817}
extern "C" const InterfaceID EIID_ByteBufferAsFloatBuffer =
        { 0x37165a91, 0x5ade, 0x4b46, { 0x93, 0xe6, 0xab, 0x45, 0x1e, 0xeb, 0x98, 0x17 } };

ByteBufferAsFloatBuffer::ByteBufferAsFloatBuffer(
    /* [in] */ ByteBuffer* byteBuffer)
{
    // super(byteBuffer.capacity() / SizeOf.FLOAT, byteBuffer.effectiveDirectAddress);
    // this.byteBuffer = byteBuffer;
    // this.byteBuffer.clear();
}

AutoPtr<IFloatBuffer> ByteBufferAsFloatBuffer::AsFloatBuffer(
    /* [in] */ ByteBuffer* byteBuffer)
{
    // ByteBuffer slice = byteBuffer.slice();
    // slice.order(byteBuffer.order());
    // return new ByteBufferAsFloatBuffer(slice);
    return NULL;
}

ECode ByteBufferAsFloatBuffer::AsReadOnlyBuffer(
    /* [out] */ IFloatBuffer** buffer)
{
    // ByteBufferAsFloatBuffer buf = new ByteBufferAsFloatBuffer(byteBuffer.asReadOnlyBuffer());
    // buf.limit = limit;
    // buf.position = position;
    // buf.mark = mark;
    // buf.byteBuffer.order = byteBuffer.order;
    // return buf;
    return NOERROR;
}

ECode ByteBufferAsFloatBuffer::Compact(
    /* [out] */ IFloatBuffer** outbuf)
{
    // if (byteBuffer.isReadOnly()) {
    //     throw new ReadOnlyBufferException();
    // }
    // byteBuffer.limit(limit * SizeOf.FLOAT);
    // byteBuffer.position(position * SizeOf.FLOAT);
    // byteBuffer.compact();
    // byteBuffer.clear();
    // position = limit - position;
    // limit = capacity;
    // mark = UNSET_MARK;
    // return this;
    return NOERROR;
}

ECode ByteBufferAsFloatBuffer::Duplicate(
    /* [out] */ IFloatBuffer** buffer)
{
    // ByteBuffer bb = byteBuffer.duplicate().order(byteBuffer.order());
    // ByteBufferAsFloatBuffer buf = new ByteBufferAsFloatBuffer(bb);
    // buf.limit = limit;
    // buf.position = position;
    // buf.mark = mark;
    // return buf;
    return NOERROR;
}

ECode ByteBufferAsFloatBuffer::Get(
    /* [out] */ Float* value)
{
    // if (position == limit) {
    //     throw new BufferUnderflowException();
    // }
    // return byteBuffer.getFloat(position++ * SizeOf.FLOAT);
    return NOERROR;
}

ECode ByteBufferAsFloatBuffer::Get(
    /* [in] */ Int32 index,
    /* [out] */ Float* value)
{
    // checkIndex(index);
    // return byteBuffer.getFloat(index * SizeOf.FLOAT);
    return NOERROR;
}

ECode ByteBufferAsFloatBuffer::Get(
    /* [out] */ ArrayOf<Float>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 charCount)
{
    // byteBuffer.limit(limit * SizeOf.FLOAT);
    // byteBuffer.position(position * SizeOf.FLOAT);
    // if (byteBuffer instanceof DirectByteBuffer) {
    //     ((DirectByteBuffer) byteBuffer).get(dst, dstOffset, floatCount);
    // } else {
    //     ((ByteArrayBuffer) byteBuffer).get(dst, dstOffset, floatCount);
    // }
    // this.position += floatCount;
    // return this;
    return NOERROR;
}

ECode ByteBufferAsFloatBuffer::IsDirect(
    /* [out] */ Boolean* rst)
{
    // return byteBuffer.isDirect();
    return NOERROR;
}

ECode ByteBufferAsFloatBuffer::IsReadOnly(
    /* [out] */ Boolean* rst)
{
    // return byteBuffer.isReadOnly();
    return NOERROR;
}

ECode ByteBufferAsFloatBuffer::GetOrder(
    /* [out] */ ByteOrder* byteOrder)
{
    // return byteBuffer.order();
    return NOERROR;
}

ECode ByteBufferAsFloatBuffer::Put(
    /* [in] */ Float c)
{
    // if (position == limit) {
    //     throw new BufferOverflowException();
    // }
    // byteBuffer.putFloat(position++ * SizeOf.FLOAT, c);
    // return this;
    return NOERROR;
}

ECode ByteBufferAsFloatBuffer::Put(
    /* [in] */ Int32 index,
    /* [in] */ Float c)
{
    // checkIndex(index);
    // byteBuffer.putFloat(index * SizeOf.FLOAT, c);
    // return this;
    return NOERROR;
}

ECode ByteBufferAsFloatBuffer::Put(
    /* [in] */ ArrayOf<Float>* src,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 charCount)
{
    // byteBuffer.limit(limit * SizeOf.FLOAT);
    // byteBuffer.position(position * SizeOf.FLOAT);
    // if (byteBuffer instanceof DirectByteBuffer) {
    //     ((DirectByteBuffer) byteBuffer).put(src, srcOffset, floatCount);
    // } else {
    //     ((ByteArrayBuffer) byteBuffer).put(src, srcOffset, floatCount);
    // }
    // this.position += floatCount;
    // return this;
    return NOERROR;
}

ECode ByteBufferAsFloatBuffer::Slice(
    /* [out] */ IFloatBuffer** buffer)
{
    // byteBuffer.limit(limit * SizeOf.FLOAT);
    // byteBuffer.position(position * SizeOf.FLOAT);
    // ByteBuffer bb = byteBuffer.slice().order(byteBuffer.order());
    // FloatBuffer result = new ByteBufferAsFloatBuffer(bb);
    // byteBuffer.clear();
    // return result;
    return NOERROR;
}

ECode ByteBufferAsFloatBuffer::ProtectedArray(
    /* [out, callee] */ ArrayOf<Float>** array)
{
    // throw new UnsupportedOperationException();
    return NOERROR;
}

ECode ByteBufferAsFloatBuffer::ProtectedArrayOffset(
    /* [out] */ Int32* offset)
{
    // throw new UnsupportedOperationException();
    return NOERROR;
}

ECode ByteBufferAsFloatBuffer::ProtectedHasArray(
    /* [out] */ Boolean* result)
{
    // return false;
    return NOERROR;
}

} // namespace IO
} // namespace Elastos
