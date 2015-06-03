
#include "CFixedLengthOutputStream.h"

namespace Elastos {
namespace Net {
namespace Http {

ECode CFixedLengthOutputStream::constructor(
    /* [in] */ IOutputStream* socketOut,
    /* [in] */ Int32 bytesRemaining)
{
    mSocketOut = socketOut;
    mBytesRemaining = bytesRemaining;

    return NOERROR;
}

ECode CFixedLengthOutputStream::Write(
    /* [in] */ Int32 oneByte)
{
    return AbstractHttpOutputStream::Write(oneByte);
}

ECode CFixedLengthOutputStream::WriteBytes(
    /* [in] */ const ArrayOf<Byte>& buffer)
{
    return AbstractHttpOutputStream::WriteBytes(buffer);
}

ECode CFixedLengthOutputStream::WriteBytes(
    /* [in] */ const ArrayOf<Byte>& buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count)
{
    FAIL_RETURN(AbstractHttpOutputStream::CheckNotClosed());

    // Arrays.checkOffsetAndCount(buffer.length, offset, count);
    Int32 packetlen = buffer.GetLength();
    if ((offset | count) < 0 || offset > packetlen || packetlen - offset < count) {
        // throw new ArrayIndexOutOfBoundsException(packetlen, offset, byteCount);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (count > mBytesRemaining) {
        // throw new IOException("expected " + bytesRemaining + " bytes but received " + count);
        return E_IO_EXCEPTION;
    }
    mSocketOut->WriteBytes(buffer, offset, count);
    mBytesRemaining -= count;
    return NOERROR;
}

ECode CFixedLengthOutputStream::CheckError(
    /* [out] */ Boolean* hasError)
{
    return AbstractHttpOutputStream::CheckError(hasError);
}

ECode CFixedLengthOutputStream::Close()
{
    if (mClosed) {
        return NOERROR;
    }
    mClosed = TRUE;
    if (mBytesRemaining > 0) {
        // throw new IOException("unexpected end of stream");
        return E_IO_EXCEPTION;
    }
    return NOERROR;
}

ECode CFixedLengthOutputStream::Flush()
{
    if (mClosed) {
        return NOERROR; // don't throw; this stream might have been closed on the caller's behalf
    }
    // socketOut.flush();
    return NOERROR;
}


} // namespace Http
} // namespace Net
} // namespace Elastos
