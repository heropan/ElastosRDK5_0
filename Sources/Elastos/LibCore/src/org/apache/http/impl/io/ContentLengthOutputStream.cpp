
#include "ChunkedOutputStream.h"
#include <elastos/Logger.h>
#include <elastos/core/StringUtils.h>

using Elastos::Core::StringUtils;
using Elastos::Utility::Logging::Logger;

namespace Org {
namespace Apache {
namespace Http {
namespace Impl {
namespace IO {

ChunkedOutputStream::ChunkedOutputStream(
    /* [in] */ ISessionOutputBuffer* out,
    /* [in] */ Int64 contentLength)
    : OutputStream()
    , mContentLength(0)
    , mTotal(0)
    , mClosed(FALSE)
{
    if (out == NULL) {
        Logger::E("ChunkedOutputStream", "Session output buffer may not be null");
        assert(0);
        // throw new IllegalArgumentException("Session output buffer may not be null");
    }
    if (contentLength < 0) {
        Logger::E("ChunkedOutputStream", "Content length may not be negative");
        assert(0);
        // throw new IllegalArgumentException("Content length may not be negative");
    }
    mOut = out;
    mContentLength = contentLength;
}

ECode ChunkedOutputStream::Close()
{
    if (!mClosed) {
        mClosed = TRUE;
        mOut->Flush();
    }
    return NOERROR;
}

ECode ChunkedOutputStream::Flush()
{
    return mOut->Flush();
}

ECode ChunkedOutputStream::Write(
    /* [in] */ ArrayOf<Byte>* b,
    /* [in] */ Int32 off,
    /* [in] */ Int32 len)
{
    if (mClosed) {
        Logger::E("ChunkedOutputStream", "Attempted write to closed stream.");
        return E_IO_EXCEPTION;
    }
    if (mTotal < mContentLength) {
        Int64 max = mContentLength - mTotal;
        if (len > max) {
            len = (Int32)max;
        }
        FAIL_RETURN(mOut->Write(b, off, len))
        mTotal += len;
    }
    return NOERROR;
}

ECode ChunkedOutputStream::Write(
    /* [in] */ ArrayOf<Byte>* buffer)
{
    return Write(b, 0, b->GetLength());
}

ECode ChunkedOutputStream::Write(
    /* [in] */ Int32 b)
{
    if (mClosed) {
        Logger::E("ChunkedOutputStream", "Attempted write to closed stream.");
        return E_IO_EXCEPTION;
    }
    if (mTotal < mContentLength) {
        FAIL_RETURN(mOut->Write(b))
        mTotal++;
    }
    return NOERROR;
}

} // namespace IO
} // namespace Impl
} // namespace Http
} // namespace Apache
} // namespace Org