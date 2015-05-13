
#include "cmdef.h"
#include "BufferedReader.h"
#include <elastos/StringBuilder.h>

using Elastos::Core::StringBuilder;

namespace Elastos {
namespace IO {

BufferedReader::BufferedReader()
    : mBuf(NULL)
    , mPos(0)
    , mEnd(0)
    , mMark(-1)
    , mMarkLimit(-1)
{
}

BufferedReader::~BufferedReader()
{
}

ECode BufferedReader::Init(
    /* [in] */ IReader* rin)
{
    return Init(rin, 8192);
}

ECode BufferedReader::Init(
    /* [in] */ IReader* rin,
    /* [in] */ Int32 size)
{
    VALIDATE_NOT_NULL(rin);

    AutoPtr<IInterface> obj;
    rin->GetLock((IInterface**)&obj);
    LockObject* lockobj = (LockObject*)obj.Get();
    Reader::Init(lockobj);

    if (size <= 0) {
//      throw new IllegalArgumentException("size <= 0");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mIn = rin;
    mBuf = ArrayOf<Char32>::Alloc(size);
    return NOERROR;
}

ECode BufferedReader::Close()
{
    Object::Autolock lock(mLock);

    if (!IsClosed()) {
        FAIL_RETURN(ICloseable::Probe(mIn)->Close());
        mBuf = NULL;
    }

    return NOERROR;
}

ECode BufferedReader::FillBuf(
    /* [out] */ Int32* number)
{
    assert(number != NULL);

    // assert(pos == end);

    if (mMark == -1 || (mPos - mMark >= mMarkLimit)) {
        /* mark isn't set or has exceeded its limit. use the whole buffer */
        Int32 result;
        FAIL_RETURN(mIn->ReadCharsEx(mBuf, 0, mBuf->GetLength(), &result));
        if (result > 0) {
            mMark = -1;
            mPos = 0;
            mEnd = result;
        }
        *number = result;
        return NOERROR;
    }

    if (mMark == 0 && mMarkLimit > mBuf->GetLength()) {
        /* the only way to make room when mark=0 is by growing the buffer */
        Int32 newLength = mBuf->GetLength() * 2;
        if (newLength > mMarkLimit) {
            newLength = mMarkLimit;
        }
        AutoPtr< ArrayOf<Char32> > newbuf = ArrayOf<Char32>::Alloc(newLength);
        newbuf->Copy(mBuf);
        mBuf = newbuf;
    }
    else if (mMark > 0) {
        /* make room by shifting the buffered data to left mark positions */
        mBuf->Copy(mBuf, mMark, mBuf->GetLength() - mMark);
        mPos -= mMark;
        mEnd -= mMark;
        mMark = 0;
    }

    /* Set the new position and mark position */
    Int32 count;
    FAIL_RETURN(mIn->ReadCharsEx(mBuf, mPos, mBuf->GetLength() - mPos, &count));
    if (count != -1) {
        mEnd += count;
    }
    *number = count;
    return NOERROR;
}

Boolean BufferedReader::IsClosed()
{
    return mBuf == NULL;
}

ECode BufferedReader::Mark(
    /* [in] */ Int32 markLimit)
{
    if (markLimit < 0) {
//      throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Object::Autolock lock(mLock);

    FAIL_RETURN(CheckNotClosed());
    mMarkLimit = markLimit;
    mMark = mPos;

    return NOERROR;
}

ECode BufferedReader::CheckNotClosed()
{
    if (IsClosed()) {
//      throw new IOException("BufferedReader is closed");
        return E_IO_EXCEPTION;
    }

    return NOERROR;
}

ECode BufferedReader::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    VALIDATE_NOT_NULL(supported)

    *supported = TRUE;
    return NOERROR;
}

ECode BufferedReader::Read(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)

    Object::Autolock lock(mLock);

    FAIL_RETURN(CheckNotClosed());
    /* Are there buffered characters available? */
    if (mPos < mEnd) {
        *value = (*mBuf)[mPos++];
        return NOERROR;
    }

    Int32 number = 0;
    FAIL_RETURN(FillBuf(&number));
    if (number != -1) {
        *value = (*mBuf)[mPos++];
        return NOERROR;
    }
    *value = -1;
    return NOERROR;
}

ECode BufferedReader::ReadCharsEx(
    /* [out] */ ArrayOf<Char32>* buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buffer)
    VALIDATE_NOT_NULL(number)

    Object::Autolock lock(mLock);

    FAIL_RETURN(CheckNotClosed());
    Int32 arrayLength = buffer->GetLength();
    if ((offset | length) < 0 || offset > arrayLength || arrayLength - offset < length) {
        // throw new ArrayIndexOutOfBoundsException(arrayLength, offset,
        //         count);
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    Int32 outstanding = length;
    while (outstanding > 0) {
        /*
         * If there are bytes in the buffer, grab those first.
         */
        Int32 available = mEnd - mPos;
        if (available > 0) {
            Int32 count = available >= outstanding ? outstanding : available;
            buffer->Copy(offset, mBuf, mPos, count);
            mPos += count;
            offset += count;
            outstanding -= count;
        }

        /*
         * Before attempting to read from the underlying stream, make
         * sure we really, really want to. We won't bother if we're
         * done, or if we've already got some bytes and reading from the
         * underlying stream would block.
         */
        Boolean ready;
        if (outstanding == 0 || (outstanding < length && (mIn->IsReady(&ready), !ready))) {
            break;
        }

        // assert(pos == end);

        /*
         * If we're unmarked and the requested size is greater than our
         * buffer, read the bytes directly into the caller's buffer. We
         * don't read into smaller buffers because that could result in
         * a many reads.
         */
        if ((mMark == -1 || (mPos - mMark >= mMarkLimit)) && outstanding >= mBuf->GetLength()) {
            Int32 count;
            FAIL_RETURN(mIn->ReadCharsEx(buffer, offset, outstanding, &count));
            if (count > 0) {
                outstanding -= count;
                mMark = -1;
            }
            break; // assume the source stream gave us all that it could
        }

        Int32 value;
        FAIL_RETURN(FillBuf(&value));
        if (value == -1) {
            break; // source is exhausted
        }
    }

    Int32 count = length - outstanding;
    *number = (count > 0 || count == length) ? count : -1;
    return NOERROR;
}

ECode BufferedReader::ChompNewline()
{
    Int32 number = 0;
    if ((mPos != mEnd || (FillBuf(&number), number != -1)) && (*mBuf)[mPos] == '\n') {
        mPos++;
    }
    return NOERROR;
}

ECode BufferedReader::ReadLine(
    /* [out] */ String* contents)
{
    Object::Autolock lock(mLock);

    FAIL_RETURN(CheckNotClosed());
    /* has the underlying stream been exhausted? */
    Int32 number;
    if (mPos == mEnd && (FillBuf(&number), number == -1)) {
        *contents = NULL;
        return NOERROR;
    }
    for (Int32 charPos = mPos; charPos < mEnd; charPos++) {
        Char32 ch = (*mBuf)[charPos];
        if (ch > '\r') {
            continue;
        }
        if (ch == '\n') {
            *contents = String(*mBuf, mPos, charPos - mPos);
            mPos = charPos + 1;
            return NOERROR;
        }
        else if (ch == '\r') {
            *contents = String(*mBuf, mPos, charPos - mPos);
            mPos = charPos + 1;
            if (((mPos < mEnd) || (FillBuf(&number), number != -1))
                    && ((*mBuf)[mPos] == '\n')) {
                mPos++;
            }
            return NOERROR;
        }
    }

    Char32 eol = '\0';
    StringBuilder result(80);
    /* Typical Line Length */

    result.AppendCharsEx(*mBuf, mPos, mEnd - mPos);
    while (TRUE) {
        mPos = mEnd;

        /* Are there buffered characters available? */
        if (eol == '\n') {
            *contents = result.ToString();
            return NOERROR;
        }
        // attempt to fill buffer
        FAIL_RETURN(FillBuf(&number));
        if (number == -1) {
            // characters or null.
            *contents = result.GetLength() > 0 || eol != '\0'
                    ? result.ToString() : String(NULL);
            return NOERROR;
        }
        for (Int32 charPos = mPos; charPos < mEnd; charPos++) {
            Char32 c = (*mBuf)[charPos];
            if (eol == '\0') {
                if ((c == '\n' || c == '\r')) {
                    eol = c;
                }
            }
            else if (eol == '\r' && c == '\n') {
                if (charPos > mPos) {
                    result.AppendCharsEx(*mBuf, mPos, charPos - mPos - 1);
                }
                mPos = charPos + 1;
                *contents = result.ToString();
                return NOERROR;
            }
            else {
                if (charPos > mPos) {
                    result.AppendCharsEx(*mBuf, mPos, charPos - mPos - 1);
                }
                mPos = charPos;
                *contents = result.ToString();
                return NOERROR;
            }
        }

        if (eol == '\0') {
            result.AppendCharsEx(*mBuf, mPos, mEnd - mPos);
        }
        else {
            result.AppendCharsEx(*mBuf, mPos, mEnd - mPos - 1);
        }
    }

    return NOERROR;
}

ECode BufferedReader::IsReady(
    /* [out] */ Boolean* ready)
{
    assert(ready != NULL);
    Object::Autolock lock(mLock);

    FAIL_RETURN(CheckNotClosed());
    Boolean isReady;
    *ready = ((mEnd - mPos) > 0) || (mIn->IsReady(&isReady), isReady);
    return NOERROR;
}

ECode BufferedReader::Reset()
{
    Object::Autolock lock(mLock);

    FAIL_RETURN(CheckNotClosed());
    if (mMark == -1) {
//      throw new IOException("Invalid mark");
        return E_IO_EXCEPTION;
    }
    mPos = mMark;
    return NOERROR;
}

ECode BufferedReader::Skip(
    /* [in] */ Int64 byteCount,
    /* [out] */ Int64* number)
{
    assert(number != NULL);

    if (byteCount < 0) {
//      throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Object::Autolock lock(mLock);

    FAIL_RETURN(CheckNotClosed());

    if (byteCount < 1) {
        *number = 0;
        return NOERROR;
    }
    if (mEnd - mPos >= byteCount) {
        mPos += byteCount;
        *number = byteCount;
        return NOERROR;
    }

    Int64 read = mEnd - mPos;
    mPos = mEnd;
    while (read < byteCount) {
        Int32 num;
        FAIL_RETURN(FillBuf(&num));
        if (num == -1) {
            *number = read;
            return NOERROR;
        }
        if (mEnd - mPos >= byteCount - read) {
            mPos += byteCount - read;
            *number = byteCount;
            return NOERROR;
        }
        // Couldn't get all the characters, skip what we read
        read += (mEnd - mPos);
        mPos = mEnd;
    }
    *number = byteCount;
    return NOERROR;
}

} // namespace IO
} // namespace Elastos
