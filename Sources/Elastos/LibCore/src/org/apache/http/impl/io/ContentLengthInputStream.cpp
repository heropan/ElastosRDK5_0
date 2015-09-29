
#include "ContentLengthInputStream.h"
#include "Logger.h"
#include "elastos/core/Math.h"

using Elastos::Core::Math;
using Elastos::Utility::Logging::Logger;

namespace Org {
namespace Apache {
namespace Http {
namespace Impl {
namespace IO {

const Int32 ContentLengthInputStream::BUFFER_SIZE;

ContentLengthInputStream::ContentLengthInputStream(
    /* [in] */ ISessionInputBuffer* in,
    /* [in] */ Int64 contentLength)
    : InputStream()
    , mContentLength(0)
    , mPos(0)
    , mClosed(FALSE)
{
    if (in == NULL) {
        Logger::E("ContentLengthInputStream", "Session input buffer may not be null");
        assert(0);
        // throw new IllegalArgumentException("Session input buffer may not be null");
    }
    if (contentLength < 0) {
        Logger::E("ContentLengthInputStream", "Content length may not be negative");
        assert(0);
        // throw new IllegalArgumentException("Content length may not be negative");
    }
    mIn = in;
    mContentLength = contentLength;
}

ECode ContentLengthInputStream::Close()
{
    if (!mClosed) {
        // try {
        AutoPtr< ArrayOf<Byte> > buffer = ArrayOf<Byte>::Alloc(BUFFER_SIZE);
        Int32 value;
        while (Read(buffer, &value), value >= 0) {
        }
        // } finally {
        //     // close after above so that we don't throw an exception trying
        //     // to read after closed!
        //     closed = true;
        // }
        closed = TRUE;
    }
    return NOERROR;
}

ECode ContentLengthInputStream::Read(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)
    *value = -1;
    if (mClosed) {
        Logger::E("ContentLengthInputStream", "Attempted read from closed stream.");
        return E_IO_EXCEPTION;
    }
    if (mPos >= mContentLength) {
        *value = -1;
        return NOERROR;
    }
    mPos++;
    return mIn->Read(value);
}

ECode ContentLengthInputStream::Read(
    /* [in] */ ArrayOf<Byte>* b,
    /* [in] */ Int32 off,
    /* [in] */ Int32 len,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number)
    *number = -1;
    if (mClosed) {
        Logger::E("ContentLengthInputStream", "Attempted read from closed stream.");
        return E_IO_EXCEPTION;
    }

    if (mPos >= mContentLength) {
        *value = -1;
        return NOERROR;
    }

    if (mPos + len > mContentLength) {
        len = (Int32)(mContentLength - mPos);
    }
    Int32 count;
    FAIL_RETURN(mIn->Read(b, off, len, &count))
    mPos += count;
    *number = count;
    return NOERROR;
}

ECode ContentLengthInputStream::Read(
    /* [in] */ ArrayOf<Byte>* b,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number)
    return Read(b, 0, b->GetLength(), number);
}

ECode ContentLengthInputStream::Skip(
    /* [in] */ Int64 n,
    /* [out] */ Int64* number)
{
    VALIDATE_NOT_NULL(number)
    if (n <= 0) {
        *number = 0;
        return NOERROR;
    }
    AutoPtr< ArrayOf<Byte> > buffer = ArrayOf<Byte>::Alloc(BUFFER_SIZE);
    // make sure we don't skip more bytes than are
    // still available
    Int64 remaining = Math::Min(n, mContentLength - mPos);
    // skip and keep track of the bytes actually skipped
    Int64 count = 0;
    while (remaining > 0) {
        Int32 l;
        Read(buffer, 0, (Int32)Elastos::Core::Math::Min(BUFFER_SIZE, remaining));
        if (l == -1) {
            break;
        }
        count += l;
        remaining -= l;
    }
    mPos += count;
    *number = count;
    return NOERROR;
}

} // namespace IO
} // namespace Impl
} // namespace Http
} // namespace Apache
} // namespace Org