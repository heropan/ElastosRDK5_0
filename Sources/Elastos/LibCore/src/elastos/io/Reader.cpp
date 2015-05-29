
#include "coredef.h"
#include "Reader.h"
#include <elastos/core/Character.h>
#include <elastos/core/Math.h>

namespace Elastos {
namespace IO {

CAR_INTERFACE_IMPL_2(Reader, Object, IReadable, IReader)

Reader::Reader()
    : mLock(this->Probe(EIID_IObject))
{
}

Reader::Reader(
    /* [in] */ IObject* lock)
    : mLock(NULL)
{
    assert(lock != NULL);
    mLock = lock;
    mLock->AddRef();
}

Reader::~Reader()
{
    if (mLock != this->Probe(EIID_IObject)) {
        mLock->Release();
        mLock = NULL;
    }
}

ECode Reader::Mark(
    /* [in] */ Int32 readLimit)
{
//    throw new IOException();
    return E_IO_EXCEPTION;
}

ECode Reader::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    assert(supported != NULL);

    *supported = FALSE;
    return NOERROR;
}

ECode Reader::Read(
    /* [out] */ Int32* value)
{
    assert(value != NULL);

    Object::Autolock lock(mLock);

    ArrayOf_<Char32, 1> buf;
    Int32 number;
    FAIL_RETURN(Read(&buf, 0, 1, &number));
    if (number != -1) {
        *value = buf[0];
    }
    else
        *value = -1;

    return NOERROR;
}

ECode Reader::Read(
    /* [out] */ ArrayOf<Char32>* buffer,
    /* [out] */ Int32* number)
{
    assert(buffer != NULL);
    assert(number != NULL);

    // BEGIN android-note
    // changed array notation to be consistent with the rest of harmony
    // END android-note
    return Read(buffer, 0, buffer->GetLength(), number);
}

ECode Reader::IsReady(
    /* [out] */ Boolean* ready)
{
    assert(ready != NULL);

    *ready = FALSE;
    return NOERROR;
}

ECode Reader::Reset()
{
    return E_IO_EXCEPTION;
}

ECode Reader::Skip(
    /* [in] */ Int64 count,
    /* [out] */ Int64* number)
{
    assert(number != NULL);

    if (count < 0) {
//      throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Object::Autolock lock(mLock);

    Int64 skipped = 0;
    Int32 toRead = count < 512 ? (Int32)count : 512;
    AutoPtr<ArrayOf<Char32> > charsSkipped = ArrayOf<Char32>::Alloc(toRead);
    while (skipped < count) {
        Int32 read;
        FAIL_RETURN(Read(charsSkipped.Get(), 0, toRead, &read));

        if (read == -1) {
            *number = skipped;
            return NOERROR;
        }
        skipped += read;
        if (read < toRead) {
            *number = skipped;
            return NOERROR;
        }
        if (count - skipped < toRead) {
            toRead = (Int32)(count - skipped);
        }
    }
    *number = skipped;
    return NOERROR;
}

ECode Reader::ReadCharBuffer(
    /* [in] */ ICharBuffer* target,
    /* [out] */ Int32* number)
{
    assert(number != NULL);

    Int32 length;
    target->GetLength(&length);
    AutoPtr<ArrayOf<Char32> > buf = ArrayOf<Char32>::Alloc(length);
    FAIL_RETURN(Read(buf, number));
    length = Elastos::Core::Math::Min(length, *number);
    if (length > 0) {
        target->PutChars(*buf, 0, length);
    }
    *number = length;

    return NOERROR;
}

ECode Reader::constructor(
    /* [in] */ IObject* lock)
{
    assert(lock != NULL);
    assert(lock != this->Probe(EIID_IObject));
    mLock = lock;
    mLock->AddRef();
    return NOERROR;
}

} // namespace IO
} // namespace Elastos
