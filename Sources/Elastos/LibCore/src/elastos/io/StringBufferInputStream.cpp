
#include "coredef.h"
#include "StringBufferInputStream.h"

namespace Elastos {
namespace IO {

StringBufferInputStream::StringBufferInputStream()
    : mCount(0)
    , mPos(0)
{}

StringBufferInputStream::~StringBufferInputStream()
{}

ECode StringBufferInputStream::Init(
    /* [in] */ const String& str)
{
    if (str.IsNull()) {
        return E_NULL_POINTER_EXCEPTION;
    }
    mBuffer = str;
    mCount = mBuffer.GetLength();
    return NOERROR;
}

ECode StringBufferInputStream::Available(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number)

    Object::Autolock lock(mLock);

    *number = mCount - mPos;

    return NOERROR;
}

ECode StringBufferInputStream::Read(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)

    Object::Autolock lock(mLock);

    *value = mPos < mCount ? mBuffer.GetChar(mPos++) & 0xFF : -1;
    return NOERROR;
}

ECode StringBufferInputStream::ReadBytes(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buffer)
    VALIDATE_NOT_NULL(number)

    // BEGIN android-note
    // changed array notation to be consistent with the rest of harmony
    // END android-note
    // According to 22.7.6 should return -1 before checking other
    // parameters.
    Object::Autolock lock(mLock);

    if (buffer == NULL) {
//      throw new NullPointerException("buffer == null");
        return E_NULL_POINTER_EXCEPTION;
    }
    // avoid int overflow
    if (offset < 0 || offset > buffer->GetLength()) {
//      throw new ArrayIndexOutOfBoundsException("Offset out of bounds: " + offset);
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    if (length < 0 || length > buffer->GetLength() - offset) {
//      throw new ArrayIndexOutOfBoundsException("Length out of bounds: " + length);
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    if (length == 0) {
        *number = 0;
        return NOERROR;
    }

    // TODO: convert Char32 to byte? is it right?
    Int32 copylen = mCount - mPos < length ? mCount - mPos : length;
    for (Int32 i = 0; i < copylen; i++) {
        (*buffer)[offset + i] = (Byte)mBuffer.GetChar(mPos + i);
    }
    mPos += copylen;
    *number = mPos <= mCount ? copylen : -1;
    return NOERROR;
}

ECode StringBufferInputStream::Reset()
{
    Object::Autolock lock(mLock);
    mPos = 0;
    return NOERROR;
}

ECode StringBufferInputStream::Skip(
    /* [in] */ Int64 count,
    /* [out] */ Int64* number)
{
    VALIDATE_NOT_NULL(number)

    Object::Autolock lock(mLock);

    if (count <= 0) {
        *number = 0;
        return NOERROR;
    }

    Int32 numskipped;
    if (mCount - mPos < count) {
        numskipped = mCount - mPos;
        mPos = mCount;
    }
    else {
        numskipped = (Int32)count;
        mPos += count;
    }
    *number = numskipped;

    return NOERROR;
}

} // namespace IO
} // namespace Elastos
