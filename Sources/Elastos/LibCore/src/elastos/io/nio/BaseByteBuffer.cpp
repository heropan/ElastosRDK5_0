
#include "BaseByteBuffer.h"
// #include "CharToByteBufferAdapter.h"
// #include "FloatToByteBufferAdapter.h"
// #include "DoubleToByteBufferAdapter.h"
// #include "Int16ToByteBufferAdapter.h"
// #include "Int32ToByteBufferAdapter.h"
// #include "Int64ToByteBufferAdapter.h"

namespace Elastos {
namespace IO {

BaseByteBuffer::BaseByteBuffer(
    /* [in] */ Int32 capacity,
    /* [in] */ MemoryBlock* block)
    : ByteBuffer(capacity, block)
{}

ECode BaseByteBuffer::AsCharBuffer(
    /* [out] */ ICharBuffer** buffer)
{
    AutoPtr<IByteBuffer> byteBuf = (IByteBuffer*)this->Probe(EIID_IByteBuffer);
   //  return CharToByteBufferAdapter::AsCharBuffer(byteBuf, buffer);
}

ECode BaseByteBuffer::AsDoubleBuffer(
    /* [out] */ IDoubleBuffer** buffer)
{
    AutoPtr<IByteBuffer> byteBuf = (IByteBuffer*)this->Probe(EIID_IByteBuffer);
   //  return DoubleToByteBufferAdapter::AsDoubleBuffer(byteBuf, buffer);
}

ECode BaseByteBuffer::AsFloatBuffer(
    /* [out] */ IFloatBuffer** buffer)
{
    AutoPtr<IByteBuffer> byteBuf = (IByteBuffer*)this->Probe(EIID_IByteBuffer);
    // return FloatToByteBufferAdapter::AsFloatBuffer(byteBuf, buffer);
}

ECode BaseByteBuffer::AsInt16Buffer(
    /* [out] */ IInt16Buffer** buffer)
{
    AutoPtr<IByteBuffer> byteBuf = (IByteBuffer*)this->Probe(EIID_IByteBuffer);
    // return Int16ToByteBufferAdapter::AsInt16Buffer(byteBuf, buffer);
}

ECode BaseByteBuffer::AsInt32Buffer(
    /* [out] */ IInt32Buffer** buffer)
{
    AutoPtr<IByteBuffer> byteBuf = (IByteBuffer*)this->Probe(EIID_IByteBuffer);
    // return Int32ToByteBufferAdapter::AsInt32Buffer(byteBuf, buffer);
}

ECode BaseByteBuffer::AsInt64Buffer(
    /* [out] */ IInt64Buffer** buffer)
{
    AutoPtr<IByteBuffer> byteBuf = (IByteBuffer*)this->Probe(EIID_IByteBuffer);
    // return Int64ToByteBufferAdapter::AsInt64Buffer(byteBuf, buffer);
}

ECode BaseByteBuffer::GetChar(
    /* [out] */ Char32* value)
{
    return GetInt32((Int32*)value);
}

ECode BaseByteBuffer::GetChar(
    /* [in] */ Int32 index,
    /* [out] */ Char32* value)
{
    return GetInt32(index, (Int32*)value);
}

ECode BaseByteBuffer::PutChar(
    /* [in] */ Char32 value)
{
    return PutInt32((Int32)value);
}

ECode BaseByteBuffer::PutChar(
    /* [in] */ Int32 index,
    /* [in] */ Char32 value)
{
    return PutInt32(index, (Int32)value);
}

} // namespace IO
} // namespace Elastos
