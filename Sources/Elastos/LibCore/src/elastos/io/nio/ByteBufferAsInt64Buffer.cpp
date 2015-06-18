
#include "ByteBufferAsInt64Buffer.h"

namespace Elastos {
namespace IO {

// {fef44de4-8334-411b-ae5b-d3977d03c99b}
extern "C" const InterfaceID EIID_ByteBufferAsInt64Buffer =
        { 0xfef44de4, 0x8334, 0x411b, { 0xae, 0x5b, 0xd3, 0x97, 0x7d, 0x03, 0xc9, 0x9b } };

ByteBufferAsInt64Buffer::ByteBufferAsInt64Buffer(
    /* [in] */ ByteBuffer* byteBuffer)
{
    // super(byteBuffer.capacity() / SizeOf.LONG, byteBuffer.effectiveDirectAddress);
    // this.byteBuffer = byteBuffer;
    // this.byteBuffer.clear();
}

AutoPtr<IInt64Buffer> ByteBufferAsInt64Buffer::AsInt64Buffer(
    /* [in] */ ByteBuffer* byteBuffer)
{
    // ByteBuffer slice = byteBuffer.slice();
    // slice.order(byteBuffer.order());
    // return new ByteBufferAsLongBuffer(slice);
}

ECode ByteBufferAsInt64Buffer::AsReadOnlyBuffer(
    /* [out] */ IInt64Buffer** buffer)
{
    // ByteBufferAsLongBuffer buf = new ByteBufferAsLongBuffer(byteBuffer.asReadOnlyBuffer());
    // buf.limit = limit;
    // buf.position = position;
    // buf.mark = mark;
    // buf.byteBuffer.order = byteBuffer.order;
    // return buf;
    return NOERROR;
}

ECode ByteBufferAsInt64Buffer::Compact(
    /* [out] */ IInt64Buffer** outbuf)
{
    // if (byteBuffer.isReadOnly()) {
    //     throw new ReadOnlyBufferException();
    // }
    // byteBuffer.limit(limit * SizeOf.LONG);
    // byteBuffer.position(position * SizeOf.LONG);
    // byteBuffer.compact();
    // byteBuffer.clear();
    // position = limit - position;
    // limit = capacity;
    // mark = UNSET_MARK;
    // return this;
    return NOERROR;
}

ECode ByteBufferAsInt64Buffer::Duplicate(
    /* [out] */ IInt64Buffer** buffer)
{
    // ByteBuffer bb = byteBuffer.duplicate().order(byteBuffer.order());
    // ByteBufferAsLongBuffer buf = new ByteBufferAsLongBuffer(bb);
    // buf.limit = limit;
    // buf.position = position;
    // buf.mark = mark;
    // return buf;
    return NOERROR;
}

ECode ByteBufferAsInt64Buffer::Get(
    /* [out] */ Int64* value)
{
    // if (position == limit) {
    //     throw new BufferUnderflowException();
    // }
    // return byteBuffer.getLong(position++ * SizeOf.LONG);
    return NOERROR;
}

ECode ByteBufferAsInt64Buffer::Get(
    /* [in] */ Int32 index,
    /* [out] */ Int64* value)
{
    // checkIndex(index);
    // return byteBuffer.getLong(index * SizeOf.LONG);
    return NOERROR;
}

ECode ByteBufferAsInt64Buffer::Get(
    /* [out] */ ArrayOf<Int64>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 charCount)
{
    // byteBuffer.limit(limit * SizeOf.LONG);
    // byteBuffer.position(position * SizeOf.LONG);
    // if (byteBuffer instanceof DirectByteBuffer) {
    //     ((DirectByteBuffer) byteBuffer).get(dst, dstOffset, longCount);
    // } else {
    //     ((ByteArrayBuffer) byteBuffer).get(dst, dstOffset, longCount);
    // }
    // this.position += longCount;
    // return this;
    return NOERROR;
}

ECode ByteBufferAsInt64Buffer::IsDirect(
    /* [out] */ Boolean* rst)
{
    // return byteBuffer.isDirect();
    return NOERROR;
}

ECode ByteBufferAsInt64Buffer::IsReadOnly(
    /* [out] */ Boolean* rst)
{
    // return byteBuffer.isReadOnly();
    return NOERROR;
}

ECode ByteBufferAsInt64Buffer::GetOrder(
    /* [out] */ ByteOrder* byteOrder)
{
    // return byteBuffer.order();
    return NOERROR;
}

ECode ByteBufferAsInt64Buffer::Put(
    /* [in] */ Int64 c)
{
    // if (position == limit) {
    //     throw new BufferOverflowException();
    // }
    // byteBuffer.putLong(position++ * SizeOf.LONG, c);
    // return this;
    return NOERROR;
}

ECode ByteBufferAsInt64Buffer::Put(
    /* [in] */ Int32 index,
    /* [in] */ Int64 c)
{
    // checkIndex(index);
    // byteBuffer.putLong(index * SizeOf.LONG, c);
    // return this;
    return NOERROR;
}

ECode ByteBufferAsInt64Buffer::Put(
    /* [in] */ ArrayOf<Int64>* src,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 charCount)
{
    // byteBuffer.limit(limit * SizeOf.LONG);
    // byteBuffer.position(position * SizeOf.LONG);
    // if (byteBuffer instanceof DirectByteBuffer) {
    //     ((DirectByteBuffer) byteBuffer).put(src, srcOffset, longCount);
    // } else {
    //     ((ByteArrayBuffer) byteBuffer).put(src, srcOffset, longCount);
    // }
    // this.position += longCount;
    // return this;
    return NOERROR;
}

ECode ByteBufferAsInt64Buffer::Slice(
    /* [out] */ IInt64Buffer** buffer)
{
    // byteBuffer.limit(limit * SizeOf.LONG);
    // byteBuffer.position(position * SizeOf.LONG);
    // ByteBuffer bb = byteBuffer.slice().order(byteBuffer.order());
    // LongBuffer result = new ByteBufferAsLongBuffer(bb);
    // byteBuffer.clear();
    // return result;
    return NOERROR;
}

ECode ByteBufferAsInt64Buffer::ProtectedArray(
    /* [out, callee] */ ArrayOf<Int64>** array)
{
    // throw new UnsupportedOperationException();
    return NOERROR;
}

ECode ByteBufferAsInt64Buffer::ProtectedArrayOffset(
    /* [out] */ Int32* offset)
{
    // throw new UnsupportedOperationException();
    return NOERROR;
}

ECode ByteBufferAsInt64Buffer::ProtectedHasArray(
    /* [out] */ Boolean* result)
{
    // return false;
    return NOERROR;
}

} // namespace IO
} // namespace Elastos
