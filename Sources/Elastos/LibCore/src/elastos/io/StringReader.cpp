
#include "coredef.h"
#include "StringReader.h"
#include <elastos/core/Character.h>

using Elastos::Core::Character;

namespace Elastos {
namespace IO {

CAR_INTERFACE_IMPL(StringReader, Reader, IStringReader)

StringReader::StringReader()
    : mMarkpos(-1)
    , mPos(0)
    , mCount(0)
{
}

StringReader::~StringReader()
{
}

ECode StringReader::constructor(
    /* [in] */ const String& str)
{
    if (str.IsNull()) {
        return E_NULL_POINTER_EXCEPTION;
    }
    mStr = str;
    if (mStr.IsNull())
        return E_OUT_OF_MEMORY_ERROR;

    mCount = mStr.GetLength();

    return NOERROR;
}

ECode StringReader::Close()
{
    mStr = NULL;

    return NOERROR;
}

Boolean StringReader::IsClosed()
{
    return mStr.IsNull();
}

ECode StringReader::Mark(
    /* [in] */ Int32 readLimit)
{
    if (readLimit < 0) {
//      throw new IllegalArgumentException("readLimit < 0: " + readLimit);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Object::Autolock lock(mLock);

    FAIL_RETURN(CheckNotClosed());
    mMarkpos = mPos;

    return NOERROR;
}

ECode StringReader::CheckNotClosed()
{
    if(IsClosed()) return E_IO_EXCEPTION;

    return NOERROR;
}

ECode StringReader::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    VALIDATE_NOT_NULL(supported)

    *supported = TRUE;

    return NOERROR;
}

ECode StringReader::Read(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)
    assert(mLock != NULL);

    Object::Autolock lock(mLock);

    FAIL_RETURN(CheckNotClosed());
    if (mPos != mCount) {
        *value = mStr.GetChar(mPos++);
        return NOERROR;
    }
    *value = -1;

    return NOERROR;
}

ECode StringReader::Read(
    /* [out] */ ArrayOf<Char32>* buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [out] */ Int32* number)
{
    // BEGIN android-note
    // changed array notation to be consistent with the rest of harmony
    // END android-note
    VALIDATE_NOT_NULL(buffer)
    VALIDATE_NOT_NULL(number)
    assert(mLock != NULL);

    Object::Autolock lock(mLock);

    if (offset < 0 || offset > buffer->GetLength()) {
//      throw new ArrayIndexOutOfBoundsException("Offset out of bounds: " + offset);
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    if (count < 0 || count > buffer->GetLength() - offset) {
//      throw new ArrayIndexOutOfBoundsException("Length out of bounds: " + len);
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    if (count == 0) {
        *number = 0;
        return NOERROR;
    }

    if (mPos == mCount) {
        *number = -1;
        return NOERROR;
    }

    Int32 end = mPos + count > mCount ? mCount : mPos + count;

    FAIL_RETURN(Character::ToChar32s(mStr, mPos, end - mPos, buffer, offset));

    Int32 read = end - mPos;
    mPos = end;
    *number = read;
    return NOERROR;
}

ECode StringReader::IsReady(
    /* [out] */ Boolean* ready)
{
    VALIDATE_NOT_NULL(ready)
    assert(mLock != NULL);

    Object::Autolock lock(mLock);

    FAIL_RETURN(CheckNotClosed());
    *ready = TRUE;

    return NOERROR;
}

ECode StringReader::Reset()
{
    assert(mLock != NULL);

    Object::Autolock lock(mLock);

    FAIL_RETURN(CheckNotClosed());
    mPos = mMarkpos != -1 ? mMarkpos : 0;

    return NOERROR;
}

ECode StringReader::Skip(
    /* [in] */ Int64 count,
    /* [out] */ Int64* number)
{
    VALIDATE_NOT_NULL(number)
    assert(mLock != NULL);

    Object::Autolock lock(mLock);

    FAIL_RETURN(CheckNotClosed());
    Int32 minSkip = -mPos;
    Int32 maxSkip = mCount - mPos;

    if (maxSkip == 0 || count > maxSkip) {
        count = maxSkip; // no rewinding if we're at the end
    } else if (count < minSkip) {
        count = minSkip;
    }

    mPos += count;
    *number = count;

    return NOERROR;
}

} // namespace IO
} // namespace Elastos
