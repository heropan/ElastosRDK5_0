
#include "cmdef.h"
#include "CFixedLengthInputStream.h"
#include <elastos/Math.h>

namespace Elastos {
namespace Net {
namespace Http {

ECode CFixedLengthInputStream::constructor(
    /* [in] */ IInputStream* is,
    /* [in] */ ICacheRequest* cacheRequest,
    /* [in] */ IHttpEngine* httpEngine,
    /* [in] */ Int32 length)
{
    AbstractHttpInputStream::Init(is, httpEngine, cacheRequest);
    mBytesRemaining = length;
    if (mBytesRemaining == 0) {
        AbstractHttpInputStream::EndOfInput(TRUE);
    }

    return NOERROR;
}

ECode CFixedLengthInputStream::Available(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number)

    FAIL_RETURN(AbstractHttpInputStream::CheckNotClosed());
    Int32 availvalue = 0;
    mIn->Available(&availvalue);
    *number = mBytesRemaining == 0 ? 0 : Elastos::Core::Math::Min(availvalue, mBytesRemaining);

    return NOERROR;
}

ECode CFixedLengthInputStream::Mark(
    /* [in] */ Int32 readLimit)
{
    return AbstractHttpInputStream::Mark(readLimit);
}

ECode CFixedLengthInputStream::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    return AbstractHttpInputStream::IsMarkSupported(supported);
}

ECode CFixedLengthInputStream::Read(
    /* [out] */ Int32* value)
{
    return AbstractHttpInputStream::Read(value);
}

ECode CFixedLengthInputStream::ReadBytes(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    return AbstractHttpInputStream::ReadBytes(buffer, number);
}

ECode CFixedLengthInputStream::ReadBytesEx(
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

    FAIL_RETURN(AbstractHttpInputStream::CheckNotClosed());
    if (mBytesRemaining == 0) {
        *number = -1;
        return NOERROR;
    }
    Int32 read = 0;
    mIn->ReadBytesEx(buffer, offset, Elastos::Core::Math::Min(length, mBytesRemaining), &read);
    if (read == -1) {
        AbstractHttpInputStream::UnexpectedEndOfInput(); // the server didn't supply the promised content length
        // throw new IOException("unexpected end of stream");
        return E_IO_EXCEPTION;
    }
    mBytesRemaining -= read;
    AbstractHttpInputStream::CacheWrite(buffer, offset, read);
    if (mBytesRemaining == 0) {
        AbstractHttpInputStream::EndOfInput(TRUE);
    }
    *number = read;
    return NOERROR;
}

ECode CFixedLengthInputStream::Reset()
{
    return AbstractHttpInputStream::Reset();
}

ECode CFixedLengthInputStream::Skip(
    /* [in] */ Int64 byteCount,
    /* [out] */ Int64* number)
{
    return AbstractHttpInputStream::Skip(byteCount, number);
}

ECode CFixedLengthInputStream::Close()
{
    if (mClosed) {
        return NOERROR;
    }
    mClosed = TRUE;
    if (mBytesRemaining != 0) {
        AbstractHttpInputStream::UnexpectedEndOfInput();
    }
    return NOERROR;
}

PInterface CFixedLengthInputStream::Probe(
    /* [in] */ REIID riid)
{
    return _CFixedLengthInputStream::Probe(riid);
}

ECode CFixedLengthInputStream::GetLock(
    /* [out] */ IInterface** lockobj)
{
    VALIDATE_NOT_NULL(lockobj);

    AutoPtr<IInterface> obj = AbstractHttpInputStream::GetLock();
    *lockobj = obj;
    INTERFACE_ADDREF(*lockobj);
    return NOERROR;
}

} // namespace Http
} // namespace Net
} // namespace Elastos

