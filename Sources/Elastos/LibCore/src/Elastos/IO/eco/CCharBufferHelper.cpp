
#include "cmdef.h"
#include "CCharBufferHelper.h"
#include "BufferFactory.h"
#include "CharBuffer.h"

namespace Elastos {
namespace IO {

ECode CCharBufferHelper::Allocate(
    /* [in] */ Int32 capacity,
    /* [out] */ ICharBuffer** charBuf)
{
    return CharBuffer::Allocate(capacity, charBuf);
}

ECode CCharBufferHelper::WrapArray(
    /* [in] */ ArrayOf<Char32>* array,
    /* [out] */ ICharBuffer** charBuf)
{
    return CharBuffer::WrapArray(array, charBuf);
}

ECode CCharBufferHelper::WrapArrayEx(
    /* [in] */ ArrayOf<Char32>* array,
    /* [in] */ Int32 start,
    /* [in] */ Int32 charCount,
    /* [out] */ ICharBuffer** charBuf)
{
    return CharBuffer::WrapArrayEx(array, start, charCount, charBuf);
}

ECode CCharBufferHelper::WrapSequence(
    /* [in] */ ICharSequence* charSequence,
    /* [out] */ ICharBuffer**  charBuf)
{
    return CharBuffer::WrapSequence(charSequence, charBuf);
}

ECode CCharBufferHelper::WrapSequenceEx(
    /* [in] */ ICharSequence* cs,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ ICharBuffer** charBuf)
{
    return CharBuffer::WrapSequenceEx(cs, start, end, charBuf);
}

} // namespace IO
} // namespace Elastos
