
#include "cmdef.h"
#include "InputStream.h"
#include <elastos/Math.h>

namespace Elastos {
namespace IO {

InputStream::InputStream()
{
    mLock = new LockObject();
}

InputStream::~InputStream()
{
}

AutoPtr<IInterface> InputStream::GetLock()
{
    AutoPtr<IInterface> obj = (IInterface*)mLock.Get();
    return obj;
}

ECode InputStream::Available(
    /* [out] */ Int32* number)
{
    assert(number != NULL);
    *number = 0;
    return NOERROR;
}

ECode InputStream::Close()
{
    return NOERROR;
}

ECode InputStream::Mark(
    /* [in] */ Int32 readLimit)
{
    return NOERROR;
}

ECode InputStream::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    VALIDATE_NOT_NULL(supported);
    *supported = FALSE;
    return NOERROR;
}

ECode InputStream::ReadBytes(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    // BEGIN android-note
    // changed array notation to be consistent with the rest of harmony
    // END android-note
    return ReadBytesEx(buffer, 0, buffer->GetLength(), number);
}

ECode InputStream::ReadBytesEx(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ Int32* number)
{
    assert(buffer != NULL);
    assert(number != NULL);

    // BEGIN android-note
    // changed array notation to be consistent with the rest of harmony
    // END android-note
    // Force null check for b first!
    if (offset > buffer->GetLength() || offset < 0) {
//      throw new ArrayIndexOutOfBoundsException("Offset out of bounds: " + offset);
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    if (length < 0 || length > buffer->GetLength() - offset) {
//      throw new ArrayIndexOutOfBoundsException("Length out of bounds: " + length);
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    for (Int32 i = 0; i < length; i++) {
        Int32 c;
        ECode ec = Read(&c);
        if (ec == (ECode)E_IO_EXCEPTION) {
            if (i != 0) {
                *number = i;
                return NOERROR;
            }
            return ec;
        }
        if (c == -1) {
            *number = i == 0? -1 : i;
            return NOERROR;
        }
        (*buffer)[offset + i] = (Byte)c;
    }
    *number = length;
    return NOERROR;
}

ECode InputStream::Reset()
{
    return E_IO_EXCEPTION;
}

ECode InputStream::Skip(
    /* [in] */ Int64 byteCount,
    /* [out] */ Int64* number)
{
    //TODO:
    //
//  return Streams.skipByReading(this, byteCount);

    AutoPtr<ArrayOf<Byte> > buffer = ArrayOf<Byte>::Alloc(4096);

    Int64 skipped = 0;
    while (skipped < byteCount) {
        Int32 toRead = (Int32)Elastos::Core::Math::Min(byteCount - skipped, (Int64)buffer->GetLength());
        Int32 read = 0;
        FAIL_RETURN(ReadBytesEx(buffer.Get(), 0, toRead, &read));
        if (read == -1) {
            break;
        }
        skipped += read;
        if (read < toRead) {
            break;
        }
    }

    *number = skipped;

    return NOERROR;
}

} // namespace IO
} // namespace Elastos
