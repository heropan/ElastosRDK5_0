
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

ECode CCharBufferHelper::Wrap(
    /* [in] */ ArrayOf<Char32>* array,
    /* [out] */ ICharBuffer** charBuf)
{
    return CharBuffer::Wrap(array, charBuf);
}

ECode CCharBufferHelper::Wrap(
    /*[in]  */ ArrayOf<Char32>* array,
    /*[in]  */ Int32 start,
    /*[in]  */ Int32 len,
    /*[out] */ ICharBuffer** charBuf)
{
    return CharBuffer::Wrap(array, start, len, charBuf);
}

ECode CCharBufferHelper::Wrap(
    /* [in] */ ICharSequence* charSequence,
    /* [out] */ ICharBuffer** charBuf)
{
    return CharBuffer::Wrap(charSequence, charBuf);
}

ECode CCharBufferHelper::Wrap(
    /* [in] */ ICharSequence* cs,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ ICharBuffer** charBuf);
{
    return CharBuffer::Wrap(cs, start, end, charBuf);
}

} // namespace IO
} // namespace Elastos
