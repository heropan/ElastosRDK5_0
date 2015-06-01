
#include "DeflaterInputStream.h"
#include <elastos/core/Math.h>

using Elastos::Core::Math;
using Elastos::IO::IStreams;
using Elastos::IO::CStreams;
using Elastos::IO::EIID_IInputStream;

namespace Elastos {
namespace Utility {
namespace Zip {

const Int32 DeflaterInputStream::DEFAULT_BUFFER_SIZE;

CAR_INTERFACE_IMPL(DeflaterInputStream, FilterInputStream, IDeflaterInputStream)

DeflaterInputStream::DeflaterInputStream()
    : mClosed(FALSE)
    , mAvailable(TRUE)
{}

DeflaterInputStream::~DeflaterInputStream()
{
}

ECode DeflaterInputStream::Close()
{
    mClosed = TRUE;
    FAIL_RETURN(mDef->End());
    return mIn->Close();
}

ECode DeflaterInputStream::Read(
    /* [out] */ Int32* value)
{
    AutoPtr<IStreams> streams;
    CStreams::AcquireSingleton((IStreams**)&streams);
    return streams->ReadSingleByte(THIS_PROBE(IInputStream), value);
}

ECode DeflaterInputStream::ReadBytes(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 byteCount,
    /* [out] */ Int32* number)
{
    FAIL_RETURN(CheckClosed());
//    Arrays.checkOffsetAndCount(buffer.length, offset, byteCount);
    if ((offset | byteCount) < 0 || offset > buffer->GetLength() ||
            buffer->GetLength() - offset < byteCount) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new ArrayIndexOutOfBoundsException(arrayLength, offset, count);
    }
    if (byteCount == 0) {
        *number = 0;
        return NOERROR;
    }

    if (!mAvailable) {
        *number = -1;
        return NOERROR;
    }

    Int32 count = 0;
    Boolean finished;
    while (count < byteCount && (mDef->Finished(&finished), !finished)) {
        Boolean needs;
        mDef->NeedsInput(&needs);
        if (needs) {
            // read data from input stream
            Int32 bytesRead;
            mIn->ReadBytes(mBuf, &bytesRead);
            if (bytesRead == -1) {
                mDef->Finish();
            }
            else {
                mDef->SetInput(*mBuf, 0, bytesRead);
            }
        }
        Int32 bytesDeflated;
        mDef->Deflate(mBuf, 0, Elastos::Core::Math::Min(mBuf->GetLength(), byteCount - count),
                &bytesDeflated);
        if (bytesDeflated == -1) {
            break;
        }
        memcpy(buffer->GetPayload() + offset + count, mBuf->GetPayload(), byteCount);
        count += byteCount;
    }
    if (count == 0) {
        count = -1;
        mAvailable = FALSE;
    }
    *number = count;
    return NOERROR;
}

ECode DeflaterInputStream::Skip(
    /* [in] */ Int64 byteCount,
    /* [out] */ Int64* number)
{
    byteCount = Elastos::Core::Math::Min((Int64)Elastos::Core::Math::INT32_MAX_VALUE, byteCount);
    AutoPtr<IStreams> streams;
    CStreams::AcquireSingleton((IStreams**)&streams);
    return streams->SkipByReading(THIS_PROBE(IInputStream),
            byteCount, number);
}

ECode DeflaterInputStream::Available(
    /* [out] */ Int32* number)
{
    FAIL_RETURN(CheckClosed());
    *number = mAvailable ? 1 : 0;
    return NOERROR;
}

ECode DeflaterInputStream::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    *supported = FALSE;
    return NOERROR;
}

ECode DeflaterInputStream::Mark(
    /* [in] */ Int32 limit)
{
    return NOERROR;
}

ECode DeflaterInputStream::Reset()
{
    return E_IO_EXCEPTION;
}

ECode DeflaterInputStream::CheckClosed()
{
    if (mClosed) {
//        throw new IOException("Stream is closed");
        return E_IO_EXCEPTION;
    }
    return NOERROR;
}

ECode DeflaterInputStream::ReadBytes(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    VALIDATE_NOT_NULL(buffer);
    return ReadBytes(buffer, 0, buffer->GetLength(), number);
}

ECode DeflaterInputStream::Init(
    /* [in] */ IInputStream* in)
{
    AutoPtr<CDeflater> deflater;
    CDeflater::NewByFriend((CDeflater**)&deflater);
    return Init(in, (IDeflater*)deflater.Get(), DEFAULT_BUFFER_SIZE);
}

ECode DeflaterInputStream::Init(
    /* [in] */ IInputStream* in,
    /* [in] */ IDeflater* deflater)
{
    return Init(in, deflater, DEFAULT_BUFFER_SIZE);
}

ECode DeflaterInputStream::Init(
    /* [in] */ IInputStream* in,
    /* [in] */ IDeflater* deflater,
    /* [in] */ Int32 bufferSize)
{
    FilterInputStream::constructor(in);
    if (in == NULL || deflater == NULL) {
//        throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }
    if (bufferSize <= 0) {
//        throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mDef = (CDeflater*)deflater;
    mBuf = ArrayOf<Byte>::Alloc(bufferSize);
    return NOERROR;
}

} // namespace Zip
} // namespace Utility
} // namespace Elastos
