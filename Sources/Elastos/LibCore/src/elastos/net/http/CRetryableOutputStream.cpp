
#include "cmdef.h"
#include "CRetryableOutputStream.h"
#include "CByteArrayOutputStream.h"

using Elastos::IO::CByteArrayOutputStream;

namespace Elastos {
namespace Net {
namespace Http {

ECode CRetryableOutputStream::constructor()
{
    mLimit = -1;
    FAIL_RETURN(CByteArrayOutputStream::New((IByteArrayOutputStream**)&mContent));
    return NOERROR;
}

ECode CRetryableOutputStream::constructor(
    /* [in] */ Int32 limi)
{
    mLimit = limi;
    FAIL_RETURN(CByteArrayOutputStream::New(mLimit, (IByteArrayOutputStream**)&mContent));
    return NOERROR;
}

ECode CRetryableOutputStream::Write(
    /* [in] */ Int32 value)
{
    return AbstractHttpOutputStream::Write(value);
}

ECode CRetryableOutputStream::WriteBytes(
    /* [in] */ const ArrayOf<Byte>& buffer)
{
    return AbstractHttpOutputStream::WriteBytes(buffer);
}

ECode CRetryableOutputStream::WriteBytesEx(
    /* [in] */ const ArrayOf<Byte>& buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count)
{
    CheckNotClosed();
    // Arrays.checkOffsetAndCount(buffer.length, offset, count);
    Int32 packetlen = buffer.GetLength();
    if ((offset | count) < 0 || offset > packetlen || packetlen - offset < count) {
        // throw new ArrayIndexOutOfBoundsException(packetlen, offset, byteCount);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Int32 sizevalue = 0;
    if (mLimit != -1 && (mContent->GetSize(&sizevalue), sizevalue) > mLimit - count) {
        // throw new IOException("exceeded content-length limit of " + limit + " bytes");
        return E_IO_EXCEPTION;
    }
    return mContent->WriteBytesEx(buffer, offset, count);
}

ECode CRetryableOutputStream::CheckError(
    /* [out] */ Boolean* hasError)
{
    return AbstractHttpOutputStream::CheckError(hasError);
}

ECode CRetryableOutputStream::Close()
{
    if (mClosed) {
        return NOERROR;
    }
    mClosed = true;
    Int32 value = 0;
    if ((mContent->GetSize(&value), value) < mLimit) {
        // throw new IOException("content-length promised " + limit + " bytes, but received " + content.size());
        return E_IO_EXCEPTION;
    }
    return NOERROR;
}

ECode CRetryableOutputStream::Flush()
{
    return AbstractHttpOutputStream::Flush();
}

ECode CRetryableOutputStream::ContentLength(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)

    Close();
    return mContent->GetSize(value);
}

ECode CRetryableOutputStream::WriteToSocket(
    /* [in] */ IOutputStream* socketOut)
{
    return mContent->WriteTo(socketOut);
}

ECode CRetryableOutputStream::GetLock(
    /* [out] */ IInterface** lockobj)
{
    VALIDATE_NOT_NULL(lockobj);

    AutoPtr<IInterface> obj = AbstractHttpOutputStream::GetLock();
    *lockobj = obj;
    INTERFACE_ADDREF(*lockobj);
    return NOERROR;
}

PInterface CRetryableOutputStream::Probe(
    /* [in] */ REIID riid)
{
    return _CRetryableOutputStream::Probe(riid);
}

} // namespace Http
} // namespace Net
} // namespace Elastos
