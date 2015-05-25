
#include "AbstractHttpOutputStream.h"

namespace Elastos {
namespace Net {
namespace Http {

AbstractHttpOutputStream::AbstractHttpOutputStream()
    : mClosed(FALSE)
{
}

ECode AbstractHttpOutputStream::Write(
    /* [in] */ Int32 value)
{
    AutoPtr< ArrayOf<Byte> > data = ArrayOf<Byte>::Alloc(value);
    return WriteBytes(*data);
}

ECode AbstractHttpOutputStream::CheckNotClosed()
{
    if (mClosed) {
        // throw new IOException("stream closed");
        return E_IO_EXCEPTION;
    }

    return NOERROR;
}

} // namespace Http
} // namespace Net
} // namespace Elastos
