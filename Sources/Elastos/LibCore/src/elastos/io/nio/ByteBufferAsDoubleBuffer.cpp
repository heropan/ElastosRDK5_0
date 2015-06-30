
#include "ByteBufferAsDoubleBuffer.h"

namespace Elastos {
namespace IO {

// {b4b1403f-82a0-4b7e-943c-fe094c26c067}
extern "C" const InterfaceID EIID_ByteBufferAsDoubleBuffer =
        { 0xb4b1403f, 0x82a0, 0x4b7e, { 0x94, 0x3c, 0xfe, 0x09, 0x4c, 0x26, 0xc0, 0x67 } };

ByteBufferAsDoubleBuffer::ByteBufferAsDoubleBuffer(
    /* [in] */ ByteBuffer* byteBuffer)
{

}

AutoPtr<IDoubleBuffer> ByteBufferAsDoubleBuffer::AsDoubleBuffer(
    /* [in] */ ByteBuffer* byteBuffer)
{
    // ByteBuffer slice = byteBuffer.slice();
    // slice.order(byteBuffer.order());
    // return new ByteBufferAsDoubleBuffer(slice);
    return NULL;
}

ECode ByteBufferAsDoubleBuffer::AsReadOnlyBuffer(
    /* [out] */ IDoubleBuffer** buffer)
{
    // ByteBufferAsDoubleBuffer buf = new ByteBufferAsDoubleBuffer(byteBuffer.asReadOnlyBuffer());
    // buf.limit = limit;
    // buf.position = position;
    // buf.mark = mark;
    // buf.byteBuffer.order = byteBuffer.order;
    // return buf;
    return NOERROR;
}

ECode ByteBufferAsDoubleBuffer::Compact(
    /* [out] */ IDoubleBuffer** outbuf)
{
    // if (byteBuffer.isReadOnly()) {
    //     throw new ReadOnlyBufferException();
    // }
    // byteBuffer.limit(limit * SizeOf.DOUBLE);
    // byteBuffer.position(position * SizeOf.DOUBLE);
    // byteBuffer.compact();
    // byteBuffer.clear();
    // position = limit - position;
    // limit = capacity;
    // mark = UNSET_MARK;
    // return this;
    return NOERROR;
}

ECode ByteBufferAsDoubleBuffer::Duplicate(
    /* [out] */ IDoubleBuffer** buffer)
{
    // ByteBuffer bb = byteBuffer.duplicate().order(byteBuffer.order());
    // ByteBufferAsDoubleBuffer buf = new ByteBufferAsDoubleBuffer(bb);
    // buf.limit = limit;
    // buf.position = position;
    // buf.mark = mark;
    // return buf;
    return NOERROR;
}

ECode ByteBufferAsDoubleBuffer::Get(
    /* [out] */ Double* value)
{
    // if (position == limit) {
    //     throw new BufferUnderflowException();
    // }
    // return byteBuffer.getDouble(position++ * SizeOf.DOUBLE);
    return NOERROR;
}

ECode ByteBufferAsDoubleBuffer::Get(
    /* [in] */ Int32 index,
    /* [out] */ Double* value)
{
    // checkIndex(index);
    // return byteBuffer.getDouble(index * SizeOf.DOUBLE);
    return NOERROR;
}

ECode ByteBufferAsDoubleBuffer::Get(
    /* [out] */ ArrayOf<Double>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 charCount)
{
    // byteBuffer.limit(limit * SizeOf.DOUBLE);
    // byteBuffer.position(position * SizeOf.DOUBLE);
    // if (byteBuffer instanceof DirectByteBuffer) {
    //     ((DirectByteBuffer) byteBuffer).get(dst, dstOffset, doubleCount);
    // } else {
    //     ((ByteArrayBuffer) byteBuffer).get(dst, dstOffset, doubleCount);
    // }
    // this.position += doubleCount;
    // return this;
    return NOERROR;
}

ECode ByteBufferAsDoubleBuffer::IsDirect(
    /* [out] */ Boolean* rst)
{
    // return byteBuffer.isDirect();
    return NOERROR;
}

ECode ByteBufferAsDoubleBuffer::IsReadOnly(
    /* [out] */ Boolean* rst)
{
    // return byteBuffer.isReadOnly();
    return NOERROR;
}

ECode ByteBufferAsDoubleBuffer::GetOrder(
    /* [out] */ ByteOrder* byteOrder)
{
    // return byteBuffer.order();
    return NOERROR;
}

ECode ByteBufferAsDoubleBuffer::Put(
    /* [in] */ Double c)
{
    // if (position == limit) {
    //     throw new BufferOverflowException();
    // }
    // byteBuffer.putDouble(position++ * SizeOf.DOUBLE, c);
    // return this;
    return NOERROR;
}

ECode ByteBufferAsDoubleBuffer::Put(
    /* [in] */ Int32 index,
    /* [in] */ Double c)
{
    // checkIndex(index);
    // byteBuffer.putDouble(index * SizeOf.DOUBLE, c);
    // return this;
    return NOERROR;
}

ECode ByteBufferAsDoubleBuffer::Put(
    /* [in] */ ArrayOf<Double>* src,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 charCount)
{
    // byteBuffer.limit(limit * SizeOf.DOUBLE);
    // byteBuffer.position(position * SizeOf.DOUBLE);
    // if (byteBuffer instanceof DirectByteBuffer) {
    //     ((DirectByteBuffer) byteBuffer).put(src, srcOffset, doubleCount);
    // } else {
    //     ((ByteArrayBuffer) byteBuffer).put(src, srcOffset, doubleCount);
    // }
    // this.position += doubleCount;
    // return this;
    return NOERROR;
}

ECode ByteBufferAsDoubleBuffer::Slice(
    /* [out] */ IDoubleBuffer** buffer)
{
    // byteBuffer.limit(limit * SizeOf.DOUBLE);
    // byteBuffer.position(position * SizeOf.DOUBLE);
    // ByteBuffer bb = byteBuffer.slice().order(byteBuffer.order());
    // DoubleBuffer result = new ByteBufferAsDoubleBuffer(bb);
    // byteBuffer.clear();
    // return result;
    return NOERROR;
}

ECode ByteBufferAsDoubleBuffer::ProtectedArray(
    /* [out, callee] */ ArrayOf<Double>** array)
{
    // throw new UnsupportedOperationException();
    return NOERROR;
}

ECode ByteBufferAsDoubleBuffer::ProtectedArrayOffset(
    /* [out] */ Int32* offset)
{
    // throw new UnsupportedOperationException();
    return NOERROR;
}

ECode ByteBufferAsDoubleBuffer::ProtectedHasArray(
    /* [out] */ Boolean* result)
{
    // return false;
    return NOERROR;
}

} // namespace IO
} // namespace Elastos
