
#include "CUnknownLengthHttpInputStream.h"

namespace Elastos {
namespace Net {
namespace Http {


ECode CUnknownLengthHttpInputStream::constructor(
    /* [in] */ IInputStream* is,
    /* [in] */ ICacheRequest* cacheRequest,
    /* [in] */ IHttpEngine* httpEngine)
{
    return AbstractHttpInputStream::Init(is, httpEngine, cacheRequest);
}

ECode CUnknownLengthHttpInputStream::Available(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number)

    FAIL_RETURN(CheckNotClosed());
    Int32 value = 0;
    *number = mIn == NULL ? 0 : (mIn->Available(&value), value);
    return NOERROR;
}

ECode CUnknownLengthHttpInputStream::Mark(
    /* [in] */ Int32 readLimit)
{
    return AbstractHttpInputStream::Mark(readLimit);
}

ECode CUnknownLengthHttpInputStream::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    return AbstractHttpInputStream::IsMarkSupported(supported);
}

ECode CUnknownLengthHttpInputStream::Read(
    /* [out] */ Int32* value)
{
    return AbstractHttpInputStream::Read(value);
}

ECode CUnknownLengthHttpInputStream::ReadBytes(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    return AbstractHttpInputStream::ReadBytes(buffer, number);
}

ECode CUnknownLengthHttpInputStream::ReadBytes(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number)

    // Arrays.checkOffsetAndCount(buffer.length, offset, count);
    Int32 packetlen = buffer->GetLength();
    if ((offset | length) < 0 || offset > packetlen || packetlen - offset < length) {
        // throw new ArrayIndexOutOfBoundsException(packetlen, offset, byteCount);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    FAIL_RETURN(CheckNotClosed());
    if (mIn == NULL || mInputExhausted) {
        *number = -1;
        return NOERROR;
    }
    Int32 read = 0;
    mIn->ReadBytes(buffer, offset, length, &read);
    if (read == -1) {
        mInputExhausted = TRUE;
        EndOfInput(FALSE);
        *number = -1;
        return NOERROR;
    }
    CacheWrite(buffer, offset, read);
    *number = read;
    return NOERROR;
}

ECode CUnknownLengthHttpInputStream::Reset()
{
    return AbstractHttpInputStream::Reset();
}

ECode CUnknownLengthHttpInputStream::Skip(
    /* [in] */ Int64 byteCount,
    /* [out] */ Int64* number)
{
    return AbstractHttpInputStream::Skip(byteCount, number);
}

ECode CUnknownLengthHttpInputStream::Close()
{
    if (mClosed) {
        return NOERROR;
    }
    mClosed = TRUE;
    if (!mInputExhausted) {
        UnexpectedEndOfInput();
    }
    return NOERROR;
}

} // namespace Http
} // namespace Net
} // namespace Elastos
