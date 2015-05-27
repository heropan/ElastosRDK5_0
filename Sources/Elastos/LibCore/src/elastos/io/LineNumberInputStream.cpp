
#include "coredef.h"
#include "LineNumberInputStream.h"
#include <elastos/Character.h>

using Elastos::Core::Character;

namespace Elastos {
namespace IO {

LineNumberInputStream::LineNumberInputStream()
    : mLineNumber(0)
    , mMarkedLineNumber(-1)
    , mLastChar(-1)
    , mMarkedLastChar(0)
{
}

LineNumberInputStream::~LineNumberInputStream()
{
}

ECode LineNumberInputStream::Init(
    /* [in] */ IInputStream* in)
{
    return FilterInputStream::Init(in);
}

ECode LineNumberInputStream::Available(
    /* [out] */ Int32* number)
{
    assert(number != NULL);

    FAIL_RETURN(mIn->Available(number));
    *number = *number / 2 + (mLastChar == -1 ? 0 : 1);

    return NOERROR;
}

ECode LineNumberInputStream::GetLineNumber(
    /* [out] */ Int32* lineNumber)
{
    assert(lineNumber != NULL);

    *lineNumber = mLineNumber;

    return NOERROR;
}

ECode LineNumberInputStream::Mark(
    /* [in] */ Int32 readLimit)
{
    FAIL_RETURN(mIn->Mark(readLimit));
    mMarkedLineNumber = mLineNumber;
    mMarkedLastChar = mLastChar;

    return NOERROR;
}

ECode LineNumberInputStream::Read(
    /* [out] */ Int32* value)
{
    assert(value != NULL);

    Int32 currentChar = mLastChar;
    if (currentChar == -1) {
        FAIL_RETURN(mIn->Read(&currentChar));
    }
    else {
        mLastChar = -1;
    }

    switch (currentChar) {
        case '\r':
            currentChar = '\n';
            FAIL_RETURN(mIn->Read(&mLastChar));
            if (mLastChar == '\n') {
                mLastChar = -1;
            }
            // fall through
        case '\n':
            mLineNumber++;
    }
    *value = currentChar;

    return NOERROR;
}

ECode LineNumberInputStream::ReadBytes(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ Int32* number)
{
    assert(buffer != NULL);
    assert(number != NULL);

    if (offset > buffer->GetLength() || offset < 0) {
//      throw new ArrayIndexOutOfBoundsException("Offset out of bounds: " + offset);
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    if (length < 0 || length > buffer->GetLength() - offset) {
//      throw new ArrayIndexOutOfBoundsException("Length out of bounds: " + length);
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    for (Int32 i = 0; i < length; i++) {
        Int32 currentChar;
        ECode ec = Read(&currentChar);
        if(ec != NOERROR){
            if(i != 0)
                *number = i;
            return ec;
        }
        if (currentChar == -1) {
            *number = i == 0 ? -1 : i;
            return NOERROR;
        }
        (*buffer)[offset + i] = (Byte)currentChar;
    }
    *number = length;

    return NOERROR;
}

ECode LineNumberInputStream::Reset()
{
    FAIL_RETURN(mIn->Reset());
    mLineNumber = mMarkedLineNumber;
    mLastChar = mMarkedLastChar;

    return NOERROR;
}

ECode LineNumberInputStream::SetLineNumber(
    /* [in] */ Int32 lineNumber)
{
    mLineNumber = lineNumber;

    return NOERROR;
}

ECode LineNumberInputStream::Skip(
    /* [in] */ Int64 count,
    /* [out] */ Int64* number)
{
    //TODO:
    //
    //return Streams.skipByReading(this, byteCount);
    assert(number != NULL);

    if (count <= 0) {
        *number = 0;
        return NOERROR;
    }
    for (Int32 i = 0; i < count; i++) {
        Int32 currentChar;
        FAIL_RETURN(Read(&currentChar));
        if (currentChar == -1) {
            *number = i;
            return NOERROR;
        }
    }
    *number = count;

    return NOERROR;
}

} // namespace IO
} // namespace Elastos
