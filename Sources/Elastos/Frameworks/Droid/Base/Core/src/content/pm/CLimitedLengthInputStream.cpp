
#include "content/pm/CLimitedLengthInputStream.h"
#include "elastos/Logger.h"
#include "ext/frameworkext.h"

using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Content {
namespace Pm {

CLimitedLengthInputStream::CLimitedLengthInputStream()
    : mEnd(0)
    , mOffset(0)
{}

CLimitedLengthInputStream::~CLimitedLengthInputStream()
{}

PInterface CLimitedLengthInputStream::Probe(
    /* [in] */ REIID riid)
{
    return _CLimitedLengthInputStream::Probe(riid);
}

ECode CLimitedLengthInputStream::constructor(
    /* [in] */ IInputStream* inStream,
    /* [in] */ Int64 offset,
    /* [in] */ Int64 length)
{
    FilterInputStream::Init(inStream);

    if (inStream == NULL) {
        Logger::E(String("CLimitedLengthInputStream"), String("in == null"));
        return E_IO_EXCEPTION;
    }

    if (offset < 0) {
        Logger::E(String("CLimitedLengthInputStream"), String("offset < 0"));
        return E_IO_EXCEPTION;
    }

    if (length < 0) {
        Logger::E(String("CLimitedLengthInputStream"), String("length < 0"));
        return E_IO_EXCEPTION;
    }

    if (length > MAX_VALUE - offset) {
        Logger::E(String("CLimitedLengthInputStream"), String("offset + length > Long.MAX_VALUE"));
        return E_IO_EXCEPTION;
    }

    mEnd = offset + length;

    Int64 outSkip;
    Skip(offset, &outSkip);
    mOffset = offset;

    return NOERROR;
}

ECode CLimitedLengthInputStream::Read(
    /* [out] */ Int32* value)
{
    if (mOffset >= mEnd) {
        *value = -1;
        return NOERROR;
    }

    mOffset++;
    return FilterInputStream::Read(value);
}

ECode CLimitedLengthInputStream::ReadBytes(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    return ReadBytesEx(buffer, 0, buffer->GetLength(), number);
}

ECode CLimitedLengthInputStream::ReadBytesEx(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 byteCount,
    /* [out] */ Int32* number)
{
    if (mOffset >= mEnd) {
        *number = -1;
        return NOERROR;
    }

    const Int32 arrayLength = buffer->GetLength();
    // Arrays.checkOffsetAndCount(arrayLength, offset, byteCount);
    if ((offset | byteCount) < 0 || offset > arrayLength || arrayLength - offset < byteCount) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    if (mOffset > MAX_VALUE - byteCount) {
        return E_IO_EXCEPTION;
    }

    if (mOffset + byteCount > mEnd) {
        byteCount = (Int32) (mEnd - mOffset);
    }

    Int32 numRead;
    FilterInputStream::ReadBytesEx(buffer, offset, byteCount, &numRead);
    mOffset += numRead;

    *number = numRead;
    return NOERROR;
}

ECode CLimitedLengthInputStream::Close()
{
    return FilterInputStream::Close();
}

ECode CLimitedLengthInputStream::Available(
    /* [out] */ Int32* number)
{
    return FilterInputStream::Available(number);
}

ECode CLimitedLengthInputStream::Mark(
    /* [in] */ Int32 readLimit)
{
    return FilterInputStream::Mark(readLimit);
}

ECode CLimitedLengthInputStream::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    return FilterInputStream::IsMarkSupported(supported);
}

ECode CLimitedLengthInputStream::Reset()
{
    return FilterInputStream::Reset();
}

ECode CLimitedLengthInputStream::Skip(
    /* [in] */ Int64 byteCount,
    /* [out] */ Int64* number)
{
    return FilterInputStream::Skip(byteCount, number);
}

ECode CLimitedLengthInputStream::GetLock(
    /* [out] */ IInterface** lockobj)
{
    VALIDATE_NOT_NULL(lockobj);

    AutoPtr<IInterface> obj = FilterInputStream::GetLock();
    *lockobj = obj;
    INTERFACE_ADDREF(*lockobj);
    return NOERROR;
}

}
}
}
}

