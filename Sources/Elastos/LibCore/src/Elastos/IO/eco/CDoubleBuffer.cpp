
#include "cmdef.h"
#include "CDoubleBufferHelper.h"

namespace Elastos {
namespace IO {

ECode CDoubleBufferHelper::constructor()
{
    return NOERROR;
}

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
    /*[in]  */ ArrayOf<Char32>* array,
    /*[in]  */ Int32 start,
    /*[in]  */ Int32 len,
    /*[out] */ ICharBuffer** charBuf)
{
    return CharBuffer::WrapArrayEx(array, start, len, charBuf);
}

ECode CCharBufferHelper::WrapSequence(
    /* [in] */ ICharSequence* charSequence,
    /* [out] */ ICharBuffer** charBuf)
{
    return CharBuffer::WrapSequence(charSequence, charBuf);
}

ECode CCharBufferHelper::WrapSequenceEx(
    /* [in] */ ICharSequence* cs,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ ICharBuffer** charBuf);
{
    return CharBuffer::WrapSequenceEx(cs, start, end, charBuf);
}

} // namespace IO
} // namespace Elastos
