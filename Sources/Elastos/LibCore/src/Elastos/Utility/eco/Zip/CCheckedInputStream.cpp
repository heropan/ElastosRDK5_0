
#include "cmdef.h"
#include "CCheckedInputStream.h"
#include <elastos/Math.h>

using Elastos::Core::Math;
using Elastos::IO::IStreams;
using Elastos::IO::CStreams;
using Elastos::IO::EIID_IInputStream;

namespace Elastos {
namespace Utility {
namespace Zip {

PInterface CCheckedInputStream::Probe(
    /* [in] */ REIID riid)
{
    return _CCheckedInputStream::Probe(riid);
}

ECode CCheckedInputStream::GetLock(
    /* [out] */ IInterface** lockobj)
{
    VALIDATE_NOT_NULL(lockobj);

    AutoPtr<IInterface> obj = FilterInputStream::GetLock();
    *lockobj = obj;
    INTERFACE_ADDREF(*lockobj);
    return NOERROR;
}

ECode CCheckedInputStream::Read(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    FAIL_RETURN(mIn->Read(value));
    if (*value != -1) {
        mCheck->Update(*value);
    }
    return NOERROR;
}

ECode CCheckedInputStream::ReadBytesEx(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 off,
    /* [in] */ Int32 nbytes,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buffer);
    VALIDATE_NOT_NULL(number);

    FAIL_RETURN(mIn->ReadBytesEx(buffer, off, nbytes, number));
    if (*number != -1) {
        mCheck->UpdateEx2(*buffer, off, *number);
    }
    return NOERROR;
}

ECode CCheckedInputStream::GetChecksum(
    /* [out] */ IChecksum** checksum)
{
    VALIDATE_NOT_NULL(checksum);

    *checksum = mCheck;
    INTERFACE_ADDREF(*checksum);
    return NOERROR;
}

ECode CCheckedInputStream::Skip(
    /* [in] */ Int64 byteCount,
    /* [out] */ Int64* number)
{
    VALIDATE_NOT_NULL(number);
    AutoPtr<IStreams> streams;
    CStreams::AcquireSingleton((IStreams**)&streams);
    return streams->SkipByReading(THIS_PROBE(IInputStream), byteCount, number);
}

ECode CCheckedInputStream::Available(
    /* [out] */ Int32* number)
{
    return FilterInputStream::Available(number);
}

ECode CCheckedInputStream::Close()
{
    return FilterInputStream::Close();
}

ECode CCheckedInputStream::Mark(
    /* [in] */ Int32 readLimit)
{
    Object::Autolock lock(mLock);

    return FilterInputStream::Mark(readLimit);
}

ECode CCheckedInputStream::Reset()
{
    Object::Autolock lock(mLock);

    return FilterInputStream::Reset();
}

ECode CCheckedInputStream::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    return FilterInputStream::IsMarkSupported(supported);
}

ECode CCheckedInputStream::ReadBytes(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    VALIDATE_NOT_NULL(buffer);
    return ReadBytesEx(buffer, 0, buffer->GetLength(), number);
}

/**
 * Constructs a new {@code CheckedInputStream} on {@code InputStream}
 * {@code is}. The checksum will be calculated using the algorithm
 * implemented by {@code csum}.
 *
 * <p><strong>Warning:</strong> passing a null source creates an invalid
 * {@code CheckedInputStream}. All operations on such a stream will fail.
 *
 * @param is
 *            the input stream to calculate checksum from.
 * @param csum
 *            an entity implementing the checksum algorithm.
 */
ECode CCheckedInputStream::constructor(
    /* [in] */ IInputStream* is,
    /* [in] */ IChecksum* csum)
{
    FilterInputStream::Init(is);
    mCheck = csum;
    return NOERROR;
}


} // namespace Zip
} // namespace Utility
} // namespace Elastos
