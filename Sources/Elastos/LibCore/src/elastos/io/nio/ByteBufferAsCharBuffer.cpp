
#include "ByteBufferAsCharBuffer.h"

namespace Elastos {
namespace IO {

// {88daddf0-a8d1-4752-8a42-cb439858875f}
extern "C" const InterfaceID EIID_ByteBufferAsCharBuffer =
        { 0x88daddf0, 0xa8d1, 0x4752, { 0x8a, 0x42, 0xcb, 0x43, 0x98, 0x58, 0x87, 0x5f } };

ByteBufferAsCharBuffer::ByteBufferAsCharBuffer(
    /* [in] */ ByteBuffer* byteBuffer)
{
    // super(byteBuffer.capacity() / SizeOf.CHAR, byteBuffer.effectiveDirectAddress);
    // this.byteBuffer = byteBuffer;
    // this.byteBuffer.clear();
}

AutoPtr<ICharBuffer> ByteBufferAsCharBuffer::AsCharBuffer(
    /* [in] */ ByteBuffer* byteBuffer)
{
    // ByteBuffer slice = byteBuffer.slice();
    // slice.order(byteBuffer.order());
    // return new ByteBufferAsCharBuffer(slice);
    return NULL;
}

ECode ByteBufferAsCharBuffer::AsReadOnlyBuffer(
    /* [out] */ ICharBuffer** buffer)
{
    // ByteBufferAsCharBuffer buf = new ByteBufferAsCharBuffer(byteBuffer.asReadOnlyBuffer());
    // buf.limit = limit;
    // buf.position = position;
    // buf.mark = mark;
    // buf.byteBuffer.order = byteBuffer.order;
    // return buf;
    return NOERROR;
}

ECode ByteBufferAsCharBuffer::Compact(
    /* [out] */ ICharBuffer** outbuf)
{
    // if (byteBuffer.isReadOnly()) {
    //     throw new ReadOnlyBufferException();
    // }
    // byteBuffer.limit(limit * SizeOf.CHAR);
    // byteBuffer.position(position * SizeOf.CHAR);
    // byteBuffer.compact();
    // byteBuffer.clear();
    // position = limit - position;
    // limit = capacity;
    // mark = UNSET_MARK;
    // return this;
    return NOERROR;
}

ECode ByteBufferAsCharBuffer::Duplicate(
    /* [out] */ ICharBuffer** buffer)
{
    // ByteBuffer bb = byteBuffer.duplicate().order(byteBuffer.order());
    // ByteBufferAsCharBuffer buf = new ByteBufferAsCharBuffer(bb);
    // buf.limit = limit;
    // buf.position = position;
    // buf.mark = mark;
    // return buf;
    return NOERROR;
}

ECode ByteBufferAsCharBuffer::Get(
    /* [out] */ Char32* value)
{
    // if (position == limit) {
    //     throw new BufferUnderflowException();
    // }
    // return byteBuffer.getChar(position++ * SizeOf.CHAR);
    return NOERROR;
}

ECode ByteBufferAsCharBuffer::Get(
    /* [in] */ Int32 index,
    /* [out] */ Char32* value)
{
    // checkIndex(index);
    // return byteBuffer.getChar(index * SizeOf.CHAR);
    return NOERROR;
}

ECode ByteBufferAsCharBuffer::Get(
    /* [out] */ ArrayOf<Char32>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 charCount)
{
    // byteBuffer.limit(limit * SizeOf.CHAR);
    // byteBuffer.position(position * SizeOf.CHAR);
    // if (byteBuffer instanceof DirectByteBuffer) {
    //     ((DirectByteBuffer) byteBuffer).get(dst, dstOffset, charCount);
    // } else {
    //     ((ByteArrayBuffer) byteBuffer).get(dst, dstOffset, charCount);
    // }
    // this.position += charCount;
    // return this;
    return NOERROR;
}

ECode ByteBufferAsCharBuffer::IsDirect(
    /* [out] */ Boolean* rst)
{
    // return byteBuffer.isDirect();
    return NOERROR;
}

ECode ByteBufferAsCharBuffer::IsReadOnly(
    /* [out] */ Boolean* rst)
{
    // return byteBuffer.isReadOnly();
    return NOERROR;
}

ECode ByteBufferAsCharBuffer::GetOrder(
    /* [out] */ ByteOrder* byteOrder)
{
    // return byteBuffer.order();
    return NOERROR;
}

ECode ByteBufferAsCharBuffer::Put(
    /* [in] */ Char32 c)
{
    // if (position == limit) {
    //     throw new BufferOverflowException();
    // }
    // byteBuffer.putChar(position++ * SizeOf.CHAR, c);
    // return this;
    return NOERROR;
}

ECode ByteBufferAsCharBuffer::Put(
    /* [in] */ Int32 index,
    /* [in] */ Char32 c)
{
    // checkIndex(index);
    // byteBuffer.putChar(index * SizeOf.CHAR, c);
    // return this;
    return NOERROR;
}

ECode ByteBufferAsCharBuffer::Put(
    /* [in] */ ArrayOf<Char32>* src,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 charCount)
{
    // byteBuffer.limit(limit * SizeOf.CHAR);
    // byteBuffer.position(position * SizeOf.CHAR);
    // if (byteBuffer instanceof DirectByteBuffer) {
    //     ((DirectByteBuffer) byteBuffer).put(src, srcOffset, charCount);
    // } else {
    //     ((ByteArrayBuffer) byteBuffer).put(src, srcOffset, charCount);
    // }
    // this.position += charCount;
    // return this;
    return NOERROR;
}

ECode ByteBufferAsCharBuffer::Slice(
    /* [out] */ ICharBuffer** buffer)
{
    // byteBuffer.limit(limit * SizeOf.CHAR);
    // byteBuffer.position(position * SizeOf.CHAR);
    // ByteBuffer bb = byteBuffer.slice().order(byteBuffer.order());
    // CharBuffer result = new ByteBufferAsCharBuffer(bb);
    // byteBuffer.clear();
    // return result;
    return NOERROR;
}

ECode ByteBufferAsCharBuffer::SubSequence(
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ ICharSequence** csq)
{
    // checkStartEndRemaining(start, end);
    // CharBuffer result = duplicate();
    // result.limit(position + end);
    // result.position(position + start);
    // return result;
    return NOERROR;
}

ECode ByteBufferAsCharBuffer::ProtectedArray(
    /* [out, callee] */ ArrayOf<Char32>** array)
{
    // throw new UnsupportedOperationException();
    return NOERROR;
}

ECode ByteBufferAsCharBuffer::ProtectedArrayOffset(
    /* [out] */ Int32* offset)
{
    // throw new UnsupportedOperationException();
    return NOERROR;
}

ECode ByteBufferAsCharBuffer::ProtectedHasArray(
    /* [out] */ Boolean* result)
{
    // return false;
    return NOERROR;
}

} // namespace IO
} // namespace Elastos
