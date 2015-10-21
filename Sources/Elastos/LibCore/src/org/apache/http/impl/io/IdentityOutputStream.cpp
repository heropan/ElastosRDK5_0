
#include "org/apache/http/impl/io/IdentityOutputStream.h"
#include "elastos/utility/logging/Logger.h"

using Elastos::Utility::Logging::Logger;

namespace Org {
namespace Apache {
namespace Http {
namespace Impl {
namespace IO {

IdentityOutputStream::IdentityOutputStream(
    /* [in] */ ISessionOutputBuffer* out)
    : OutputStream()
    , mClosed(FALSE)
{
    if (out == NULL) {
        Logger::E("IdentityOutputStream", "Session output buffer may not be null");
        assert(0);
        // throw new IllegalArgumentException("Session output buffer may not be null");
    }
    mOut = out;
}

ECode IdentityOutputStream::Close()
{
    if (!mClosed) {
        mClosed = TRUE;
        mOut->Flush();
    }
    return NOERROR;
}

ECode IdentityOutputStream::Flush()
{
    return mOut->Flush();
}

ECode IdentityOutputStream::Write(
    /* [in] */ ArrayOf<Byte>* b,
    /* [in] */ Int32 off,
    /* [in] */ Int32 len)
{
    if (mClosed) {
        Logger::E("IdentityOutputStream", "Attempted write to closed stream.");
        return E_IO_EXCEPTION;
    }
    return mOut->Write(b, off, len);
}

ECode IdentityOutputStream::Write(
    /* [in] */ ArrayOf<Byte>* buffer)
{
    return Write(buffer, 0, buffer->GetLength());
}

ECode IdentityOutputStream::Write(
    /* [in] */ Int32 b)
{
    if (mClosed) {
        Logger::E("IdentityOutputStream", "Attempted write to closed stream.");
        return E_IO_EXCEPTION;
    }
    return mOut->Write(b);
}

} // namespace IO
} // namespace Impl
} // namespace Http
} // namespace Apache
} // namespace Org