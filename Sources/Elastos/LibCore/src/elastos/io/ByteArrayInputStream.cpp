
#include "ByteArrayInputStream.h"

namespace Elastos {
namespace IO {

CAR_INTERFACE_IMPL(ByteArrayInputStream, InputStream, IByteArrayInputStream)

ByteArrayInputStream::ByteArrayInputStream()
    : mPos(0)
    , mMark(0)
    , mCount(0)
{
}

ByteArrayInputStream::~ByteArrayInputStream()
{
}

ECode ByteArrayInputStream::constructor(
    /* [in] */ ArrayOf<Byte>* buffer)
{
    VALIDATE_NOT_NULL(buffer);

    mMark = 0;
    mBuf = buffer;
    mCount = mBuf->GetLength();

    return NOERROR;
}

ECode ByteArrayInputStream::constructor(
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
    VALIDATE_NOT_NULL(number)

    Object::Autolock lock(this);

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
    Object::Autolock lock(this);

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
    VALIDATE_NOT_NULL(value);

    Object::Autolock lock(this);

    *value = mPos < mCount ? (*mBuf)[mPos++] & 0xFF : -1;
    return NOERROR;
}

CARAPI ByteArrayInputStream::Read(
    /* [in] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 byteOffset,
    /* [in] */ Int32 byteCount,
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

    Object::Autolock lock(this);

    // avoid int overflow
    // Exception priorities (in case of multiple errors) differ from
    // RI, but are spec-compliant.
    // removed redundant check, used (offset | length) < 0 instead of
    // (offset < 0) || (length < 0) to safe one operation
    if ((byteOffset | byteCount) < 0 || byteCount > buffer->GetLength() - byteOffset) {
//      throw new IndexOutOfBoundsException();
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    // END android-changed
    // Are there any bytes available?
    if (mPos >= mCount) {
        *number = -1;
        return NOERROR;
    }
    if (byteCount == 0) {
        *number = 0;
        return NOERROR;
    }

    Int32 copylen = mCount - mPos < byteCount ? mCount - mPos : byteCount;
    memcpy(buffer->GetPayload() + byteOffset, mBuf->GetPayload() + mPos, copylen);

    mPos += copylen;
    *number = copylen;

    return NOERROR;
}

CARAPI ByteArrayInputStream::Reset()
{
    Object::Autolock lock(this);

    mPos = mMark;
    return NOERROR;
}

CARAPI ByteArrayInputStream::Skip(
    /* [in] */ Int64 count,
    /* [out] */ Int64* number)
{
    assert(number != NULL);

    Object::Autolock lock(this);

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
