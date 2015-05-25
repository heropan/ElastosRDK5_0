
#include "cmdef.h"
#include "ByteArrayInputStream.h"

namespace Elastos {
namespace IO {

ByteArrayInputStream::ByteArrayInputStream()
    : mPos(0)
    , mMark(0)
    , mCount(0)
{
}

ByteArrayInputStream::~ByteArrayInputStream()
{
}

ECode ByteArrayInputStream::Init(
    /* [in] */ ArrayOf<Byte>* buffer)
{
    VALIDATE_NOT_NULL(buffer);

    mMark = 0;
    mBuf = buffer;
    mCount = mBuf->GetLength();

    return NOERROR;
}

ECode ByteArrayInputStream::Init(
    /* [in] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length)
{
    VALIDATE_NOT_NULL(buffer);

    mBuf = buffer;
    mPos = offset;
    mMark = offset;
    mCount = offset + length > mBuf->GetLength() ?
            mBuf->GetLength() : offset + length;
    return NOERROR;
}

ECode ByteArrayInputStream::Available(
    /* [out] */ Int32* number)
{
    assert(number != NULL);
    *number = mCount - mPos;
    return NOERROR;
}

ECode ByteArrayInputStream::Close()
{
    return NOERROR;
}

CARAPI ByteArrayInputStream::Mark(
    /* [in] */ Int32 readLimit)
{
    mMark = mPos;
    return NOERROR;
}

CARAPI ByteArrayInputStream::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    VALIDATE_NOT_NULL(supported)

    *supported = TRUE;
    return NOERROR;
}

CARAPI ByteArrayInputStream::Read(
    /* [out] */ Int32* value)
{
    *value = mPos < mCount ? (*mBuf)[mPos++] & 0xFF : -1;
    return NOERROR;
}

CARAPI ByteArrayInputStream::ReadBytesEx(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number)

    // BEGIN android-note
    // changed array notation to be consistent with the rest of harmony
    // END android-note
    // BEGIN android-changed
    if (buffer == NULL) {
//      throw new NullPointerException("buffer == null");
        return E_NULL_POINTER_EXCEPTION;
    }
    // avoid int overflow
    // Exception priorities (in case of multiple errors) differ from
    // RI, but are spec-compliant.
    // removed redundant check, used (offset | length) < 0 instead of
    // (offset < 0) || (length < 0) to safe one operation
    if ((offset | length) < 0 || length > buffer->GetLength() - offset) {
//      throw new IndexOutOfBoundsException();
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    // END android-changed
    // Are there any bytes available?
    if (mPos >= mCount) {
        *number = -1;
        return NOERROR;
    }
    if (length == 0) {
        *number = 0;
        return NOERROR;
    }

    Int32 copylen = mCount - mPos < length ? mCount - mPos : length;
    memcpy(buffer->GetPayload() + offset, mBuf->GetPayload() + mPos, copylen);

    mPos += copylen;
    *number = copylen;

    return NOERROR;
}

CARAPI ByteArrayInputStream::Reset()
{
    mPos = mMark;
    return NOERROR;
}

CARAPI ByteArrayInputStream::Skip(
    /* [in] */ Int64 count,
    /* [out] */ Int64* number)
{
    assert(number != NULL);

    if (count <= 0) {
        *number = 0;
        return NOERROR;
    }
    Int32 temp = mPos;
    mPos = mCount - mPos < count ? mCount : (Int32)(mPos + count);
    *number = mPos - temp;
    return NOERROR;
}

} // namespace IO
} // namespace Elastos
