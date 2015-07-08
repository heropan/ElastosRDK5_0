
#include "ByteBufferAsInt32Buffer.h"

using Libcore::IO::ISizeOf;

namespace Elastos {
namespace IO {

// {b19aa0ae-9913-4cf6-8c33-852d2a6946bf}
extern "C" const InterfaceID EIID_ByteBufferAsInt32Buffer =
        { 0xb19aa0ae, 0x9913, 0x4cf6, { 0x8c, 0x33, 0x85, 0x2d, 0x2a, 0x69, 0x46, 0xbf } };

ByteBufferAsInt32Buffer::ByteBufferAsInt32Buffer(
    /* [in] */ ByteBuffer* byteBuffer)
{
    // super(byteBuffer.capacity() / ISizeOf::INT, byteBuffer.effectiveDirectAddress);
    // this.byteBuffer = byteBuffer;
    // this.byteBuffer.clear();
}

AutoPtr<IInt32Buffer> ByteBufferAsInt32Buffer::AsInt32Buffer(
    /* [in] */ ByteBuffer* byteBuffer)
{
    // ByteBuffer slice = byteBuffer.slice();
    // slice.order(byteBuffer.order());
    // return new ByteBufferAsIntBuffer(slice);
    return NULL;
}

ECode ByteBufferAsInt32Buffer::AsReadOnlyBuffer(
    /* [out] */ IInt32Buffer** buffer)
{
    // ByteBufferAsIntBuffer buf = new ByteBufferAsIntBuffer(byteBuffer.asReadOnlyBuffer());
    // buf.limit = limit;
    // buf.position = position;
    // buf.mark = mark;
    // buf.byteBuffer.order = byteBuffer.order;
    // return buf;
    return NOERROR;
}

ECode ByteBufferAsInt32Buffer::Compact()
{
    // if (byteBuffer.isReadOnly()) {
    //     throw new ReadOnlyBufferException();
    // }
    // byteBuffer.limit(limit * ISizeOf::INT);
    // byteBuffer.position(position * ISizeOf::INT);
    // byteBuffer.compact();
    // byteBuffer.clear();
    // position = limit - position;
    // limit = capacity;
    // mark = UNSET_MARK;
    // return this;
    return NOERROR;
}

ECode ByteBufferAsInt32Buffer::Duplicate(
    /* [out] */ IInt32Buffer** buffer)
{
    // ByteBuffer bb = byteBuffer.duplicate().order(byteBuffer.order());
    // ByteBufferAsIntBuffer buf = new ByteBufferAsIntBuffer(bb);
    // buf.limit = limit;
    // buf.position = position;
    // buf.mark = mark;
    // return buf;
    return NOERROR;
}

ECode ByteBufferAsInt32Buffer::Get(
    /* [out] */ Int32* value)
{
    // if (position == limit) {
    //     throw new BufferUnderflowException();
    // }
    // return byteBuffer.getInt(position++ * ISizeOf::INT);
    return NOERROR;
}

ECode ByteBufferAsInt32Buffer::Get(
    /* [in] */ Int32 index,
    /* [out] */ Int32* value)
{
    // checkIndex(index);
    // return byteBuffer.getInt(index * ISizeOf::INT);
    return NOERROR;
}

ECode ByteBufferAsInt32Buffer::Get(
    /* [out] */ ArrayOf<Int32>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 charCount)
{
    // byteBuffer.limit(limit * ISizeOf::INT);
    // byteBuffer.position(position * ISizeOf::INT);
    // if (byteBuffer instanceof DirectByteBuffer) {
    //     ((DirectByteBuffer) byteBuffer).get(dst, dstOffset, intCount);
    // } else {
    //     ((ByteArrayBuffer) byteBuffer).get(dst, dstOffset, intCount);
    // }
    // this.position += intCount;
    // return this;
    return NOERROR;
}

ECode ByteBufferAsInt32Buffer::IsDirect(
    /* [out] */ Boolean* rst)
{
    // return byteBuffer.isDirect();
    return NOERROR;
}

ECode ByteBufferAsInt32Buffer::IsReadOnly(
    /* [out] */ Boolean* rst)
{
    // return byteBuffer.isReadOnly();
    return NOERROR;
}

ECode ByteBufferAsInt32Buffer::GetOrder(
    /* [out] */ ByteOrder* byteOrder)
{
    // return byteBuffer.order();
    return NOERROR;
}

ECode ByteBufferAsInt32Buffer::Put(
    /* [in] */ Int32 c)
{
    // if (position == limit) {
    //     throw new BufferOverflowException();
    // }
    // byteBuffer.putInt(position++ * ISizeOf::INT, c);
    // return this;
    return NOERROR;
}

ECode ByteBufferAsInt32Buffer::Put(
    /* [in] */ Int32 index,
    /* [in] */ Int32 c)
{
    // checkIndex(index);
    // byteBuffer.putInt(index * ISizeOf::INT, c);
    // return this;
    return NOERROR;
}

ECode ByteBufferAsInt32Buffer::Put(
    /* [in] */ ArrayOf<Int32>* src,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 charCount)
{
    // byteBuffer.limit(limit * ISizeOf::INT);
    // byteBuffer.position(position * ISizeOf::INT);
    // if (byteBuffer instanceof DirectByteBuffer) {
    //     ((DirectByteBuffer) byteBuffer).put(src, srcOffset, intCount);
    // } else {
    //     ((ByteArrayBuffer) byteBuffer).put(src, srcOffset, intCount);
    // }
    // this.position += intCount;
    // return this;
    return NOERROR;
}

ECode ByteBufferAsInt32Buffer::Slice(
    /* [out] */ IInt32Buffer** buffer)
{
    // byteBuffer.limit(limit * ISizeOf::INT);
    // byteBuffer.position(position * ISizeOf::INT);
    // ByteBuffer bb = byteBuffer.slice().order(byteBuffer.order());
    // IntBuffer result = new ByteBufferAsIntBuffer(bb);
    // byteBuffer.clear();
    // return result;
    return NOERROR;
}

ECode ByteBufferAsInt32Buffer::ProtectedArray(
    /* [out, callee] */ ArrayOf<Int32>** array)
{
    // throw new UnsupportedOperationException();
    return NOERROR;
}

ECode ByteBufferAsInt32Buffer::ProtectedArrayOffset(
    /* [out] */ Int32* offset)
{
    // throw new UnsupportedOperationException();
    return NOERROR;
}

ECode ByteBufferAsInt32Buffer::ProtectedHasArray(
    /* [out] */ Boolean* result)
{
    // return false;
    return NOERROR;
}

} // namespace IO
} // namespace Elastos
