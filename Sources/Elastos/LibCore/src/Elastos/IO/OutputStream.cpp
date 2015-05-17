
#include "OutputStream.h"

namespace Elastos {
namespace IO {

OutputStream::OutputStream()
{
    mLock = new LockObject();
}

OutputStream::~OutputStream()
{
}

AutoPtr<IInterface> OutputStream::GetLock()
{
    AutoPtr<IInterface> obj = (IInterface*)mLock.Get();
    return obj;
}

ECode OutputStream::Close()
{
    return NOERROR;
}

ECode OutputStream::Flush()
{
    return NOERROR;
}

ECode OutputStream::WriteBytes(
    /* [in] */ const ArrayOf<Byte> & buffer)
{
    // BEGIN android-note
    // changed array notation to be consistent with the rest of harmony
    // END android-note
    return WriteBytesEx(buffer, 0, buffer.GetLength());
}

ECode OutputStream::WriteBytesEx(
    /* [in] */ const ArrayOf<Byte>& buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count)
{
    // BEGIN android-note
    // changed array notation to be consistent with the rest of harmony
    // END android-note
    // avoid int overflow, check null buffer
    // BEGIN android-changed
    // Exception priorities (in case of multiple errors) differ from
    // RI, but are spec-compliant.
    // removed redundant check, made implicit null check explicit,
    // used (offset | count) < 0 instead of (offset < 0) || (count < 0)
    // to safe one operation
    if ((offset | count) < 0 || count > buffer.GetLength() - offset) {
//      throw new IndexOutOfBoundsException();
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    // END android-changed
    for (Int32 i = offset; i < offset + count; i++) {
        Write(buffer[i]);
    }
    return NOERROR;
}

ECode OutputStream::CheckError(
    /* [out] */ Boolean* hasError)
{
    assert(hasError != NULL);
    *hasError = FALSE;
    return NOERROR;
}

} // namespace IO
} // namespace Elastos
