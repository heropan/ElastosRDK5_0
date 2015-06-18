
#include "ByteBufferAsInt16Buffer.h"

namespace Elastos {
namespace IO {

// {82131487-1a59-4024-89de-850c64b9d84d}
extern "C" const InterfaceID EIID_ByteBufferAsInt16Buffer =
        { 0x82131487, 0x1a59, 0x4024, { 0x89, 0xde, 0x85, 0x0c, 0x64, 0xb9, 0xd8, 0x4d } };

ByteBufferAsInt16Buffer::ByteBufferAsInt16Buffer(
    /* [in] */ ByteBuffer* byteBuffer)
{
    // super(byteBuffer.capacity() / SizeOf.SHORT, byteBuffer.effectiveDirectAddress);
    // this.byteBuffer = byteBuffer;
    // this.byteBuffer.clear();
}

AutoPtr<IInt16Buffer> ByteBufferAsInt16Buffer::AsInt16Buffer(
    /* [in] */ ByteBuffer* byteBuffer)
{
    // ByteBuffer slice = byteBuffer.slice();
    // slice.order(byteBuffer.order());
    // return new ByteBufferAsShortBuffer(slice);
}

ECode ByteBufferAsInt16Buffer::AsReadOnlyBuffer(
    /* [out] */ IInt16Buffer** buffer)
{
    // ByteBufferAsShortBuffer buf = new ByteBufferAsShortBuffer(byteBuffer.asReadOnlyBuffer());
    // buf.limit = limit;
    // buf.position = position;
    // buf.mark = mark;
    // buf.byteBuffer.order = byteBuffer.order;
    // return buf;
    return NOERROR;
}

ECode ByteBufferAsInt16Buffer::Compact(
    /* [out] */ IInt16Buffer** outbuf)
{
    // if (byteBuffer.isReadOnly()) {
    //     throw new ReadOnlyBufferException();
    // }
    // byteBuffer.limit(limit * SizeOf.SHORT);
    // byteBuffer.position(position * SizeOf.SHORT);
    // byteBuffer.compact();
    // byteBuffer.clear();
    // position = limit - position;
    // limit = capacity;
    // mark = UNSET_MARK;
    // return this;
    return NOERROR;
}

ECode ByteBufferAsInt16Buffer::Duplicate(
    /* [out] */ IInt16Buffer** buffer)
{
    // ByteBuffer bb = byteBuffer.duplicate().order(byteBuffer.order());
    // ByteBufferAsShortBuffer buf = new ByteBufferAsShortBuffer(bb);
    // buf.limit = limit;
    // buf.position = position;
    // buf.mark = mark;
    // return buf;
    return NOERROR;
}

ECode ByteBufferAsInt16Buffer::Get(
    /* [out] */ Int16* value)
{
    // if (position == limit) {
    //     throw new BufferUnderflowException();
    // }
    // return byteBuffer.getShort(position++ * SizeOf.SHORT);
    return NOERROR;
}

ECode ByteBufferAsInt16Buffer::Get(
    /* [in] */ Int32 index,
    /* [out] */ Int16* value)
{
    // checkIndex(index);
    // return byteBuffer.getShort(index * SizeOf.SHORT);
    return NOERROR;
}

ECode ByteBufferAsInt16Buffer::Get(
    /* [out] */ ArrayOf<Int16>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 charCount)
{
    // byteBuffer.limit(limit * SizeOf.SHORT);
    // byteBuffer.position(position * SizeOf.SHORT);
    // if (byteBuffer instanceof DirectByteBuffer) {
    //     ((DirectByteBuffer) byteBuffer).get(dst, dstOffset, shortCount);
    // } else {
    //     ((ByteArrayBuffer) byteBuffer).get(dst, dstOffset, shortCount);
    // }
    // this.position += shortCount;
    // return this;
    return NOERROR;
}

ECode ByteBufferAsInt16Buffer::IsDirect(
    /* [out] */ Boolean* rst)
{
    // return byteBuffer.isDirect();
    return NOERROR;
}

ECode ByteBufferAsInt16Buffer::IsReadOnly(
    /* [out] */ Boolean* rst)
{
    // return byteBuffer.isReadOnly();
    return NOERROR;
}

ECode ByteBufferAsInt16Buffer::GetOrder(
    /* [out] */ ByteOrder* byteOrder)
{
    // return byteBuffer.order();
    return NOERROR;
}

ECode ByteBufferAsInt16Buffer::Put(
    /* [in] */ Int16 c)
{
    // if (position == limit) {
    //     throw new BufferOverflowException();
    // }
    // byteBuffer.putShort(position++ * SizeOf.SHORT, c);
    // return this;
    return NOERROR;
}

ECode ByteBufferAsInt16Buffer::Put(
    /* [in] */ Int32 index,
    /* [in] */ Int16 c)
{
    // checkIndex(index);
    // byteBuffer.putShort(index * SizeOf.SHORT, c);
    // return this;
    return NOERROR;
}

ECode ByteBufferAsInt16Buffer::Put(
    /* [in] */ ArrayOf<Int16>* src,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 charCount)
{
    // byteBuffer.limit(limit * SizeOf.SHORT);
    // byteBuffer.position(position * SizeOf.SHORT);
    // if (byteBuffer instanceof DirectByteBuffer) {
    //     ((DirectByteBuffer) byteBuffer).put(src, srcOffset, shortCount);
    // } else {
    //     ((ByteArrayBuffer) byteBuffer).put(src, srcOffset, shortCount);
    // }
    // this.position += shortCount;
    // return this;
    return NOERROR;
}

ECode ByteBufferAsInt16Buffer::Slice(
    /* [out] */ IInt16Buffer** buffer)
{
    // byteBuffer.limit(limit * SizeOf.SHORT);
    // byteBuffer.position(position * SizeOf.SHORT);
    // ByteBuffer bb = byteBuffer.slice().order(byteBuffer.order());
    // ShortBuffer result = new ByteBufferAsShortBuffer(bb);
    // byteBuffer.clear();
    // return result;
    return NOERROR;
}

ECode ByteBufferAsInt16Buffer::ProtectedArray(
    /* [out, callee] */ ArrayOf<Int16>** array)
{
    // throw new UnsupportedOperationException();
    return NOERROR;
}

ECode ByteBufferAsInt16Buffer::ProtectedArrayOffset(
    /* [out] */ Int32* offset)
{
    // throw new UnsupportedOperationException();
    return NOERROR;
}

ECode ByteBufferAsInt16Buffer::ProtectedHasArray(
    /* [out] */ Boolean* result)
{
    // return false;
    return NOERROR;
}

} // namespace IO
} // namespace Elastos
