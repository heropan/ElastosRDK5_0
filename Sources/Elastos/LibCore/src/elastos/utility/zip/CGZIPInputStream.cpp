
#include "CGZIPInputStream.h"
#include "Memory.h"
#include "CInflater.h"
#include "Arrays.h"

using Libcore::IO::Memory;
using Elastos::IO::ByteOrder_LITTLE_ENDIAN;

namespace Elastos {
namespace Utility {
namespace Zip {

const Int32 CGZIPInputStream::FCOMMENT;
const Int32 CGZIPInputStream::FEXTRA;
const Int32 CGZIPInputStream::FHCRC;
const Int32 CGZIPInputStream::FNAME;

CAR_INTERFACE_IMPL(CGZIPInputStream, InflaterInputStream, IGZIPInputStream)

CAR_OBJECT_IMPL(CGZIPInputStream)

CGZIPInputStream::CGZIPInputStream()
    : mEos(FALSE)
{
    CCRC32::NewByFriend((CCRC32**)&mCrc);
}

ECode CGZIPInputStream::Close()
{
    mEos = TRUE;
    return InflaterInputStream::Close();
}

ECode CGZIPInputStream::Read(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 byteCount,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    *number = -1;
    VALIDATE_NOT_NULL(buffer);

    if (mClosed) {
//        throw new IOException("Stream is closed");
        return E_IO_EXCEPTION;
    }
    if (mEos) {
        return NOERROR;
    }
    FAIL_RETURN(Arrays::CheckOffsetAndCount(buffer->GetLength(), offset, byteCount))

    Int32 bytesRead;
    InflaterInputStream::Read(buffer, offset, byteCount, &bytesRead);
    mEos = mEof;

    if (bytesRead != -1) {
        mCrc->Update(buffer, offset, bytesRead);
    }

    if (mEos) {
        FAIL_RETURN(VerifyCrc());
    }

    *number = bytesRead;
    return NOERROR;
}

ECode CGZIPInputStream::VerifyCrc()
{
    // Get non-compressed bytes read by fill
    Int32 size;
    mInf->GetRemaining(&size);
    const Int32 trailerSize = 8; // crc (4 bytes) + total out (4 bytes)
    AutoPtr<ArrayOf<Byte> > b = ArrayOf<Byte>::Alloc(trailerSize);
    Int32 copySize = (size > trailerSize) ? trailerSize : size;

    memcpy(mBuf->GetPayload() + mLen - size, b->GetPayload(), copySize);
    FAIL_RETURN(ReadFully(copySize, trailerSize - copySize, b));

    Int64 checksum;
    mCrc->GetValue(&checksum);

    Int32 temp = Memory::PeekInt32(b, 0, ByteOrder_LITTLE_ENDIAN);
    if (temp != (Int32)checksum) {
//        throw new IOException("CRC mismatch");
        return E_IO_EXCEPTION;
    }

    Int32 value;
    mInf->GetTotalOut(&value);
    temp = Memory::PeekInt32(b, 4, ByteOrder_LITTLE_ENDIAN);
    if (temp != value) {
//        throw new IOException("Size mismatch");
        return E_IO_EXCEPTION;
    }
    return NOERROR;
}

ECode CGZIPInputStream::ReadFully(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ ArrayOf<Byte>* buffer)
{
    Int32 result;
    while (length > 0) {
        FAIL_RETURN(mIn->Read(buffer, offset, length, &result));
        if (result == -1) {
//            throw new EOFException();
            return E_EOF_EXCEPTION;
        }
        offset += result;
        length -= result;
    }
    return NOERROR;
}

ECode CGZIPInputStream::ReadZeroTerminated(
    /* [in] */ Boolean hcrc)
{
    Int32 result;
    FAIL_RETURN(mIn->Read(&result));
    while (result > 0) {
        if (hcrc) {
            mCrc->Update(result);
        }
        FAIL_RETURN(mIn->Read(&result));
    }
    if (result == -1) {
//        throw new EOFException();
        return E_EOF_EXCEPTION;
    }
    // Add the zero
    if (hcrc) {
        mCrc->Update(result);
    }
    return NOERROR;
}

/**
 * Construct a {@code GZIPInputStream} to read from GZIP data from the
 * underlying stream.
 *
 * @param is
 *            the {@code InputStream} to read data from.
 * @throws IOException
 *             if an {@code IOException} occurs.
 */
ECode CGZIPInputStream::constructor(
    /* [in] */ IInputStream* is)
{
    return constructor(is, BUF_SIZE);
}

/**
 * Construct a {@code GZIPInputStream} to read from GZIP data from the
 * underlying stream. Set the internal buffer size to {@code size}.
 *
 * @param is
 *            the {@code InputStream} to read data from.
 * @param size
 *            the internal read buffer size.
 * @throws IOException
 *             if an {@code IOException} occurs.
 */
ECode CGZIPInputStream::constructor(
    /* [in] */ IInputStream* is,
    /* [in] */ Int32 size)
{
    AutoPtr<CInflater> inflater;
    CInflater::NewByFriend(TRUE, (CInflater**)&inflater);
    FAIL_RETURN(InflaterInputStream::constructor(is, inflater.Get(), size));
    AutoPtr<ArrayOf<Byte> > header = ArrayOf<Byte>::Alloc(10);
    FAIL_RETURN(ReadFully(0, header->GetLength(), header));
    Int16 magic = Memory::PeekInt16(header, 0, ByteOrder_LITTLE_ENDIAN);
    if (magic != MAGIC) {
//        throw new IOException(String.format("unknown format (magic number %x)", magic));
        return E_IO_EXCEPTION;
    }

    Int32 flags = (*header)[3];
    Boolean hcrc = (flags & FHCRC) != 0;
    if (hcrc) {
        mCrc->Update(header, 0, header->GetLength());
    }
    if ((flags & FEXTRA) != 0) {
        FAIL_RETURN(ReadFully(0, 2, header));
        if (hcrc) {
            mCrc->Update(header, 0, 2);
        }
        Int16 temp = Memory::PeekInt16(header, 0, ByteOrder_LITTLE_ENDIAN);
        Int32 length = temp & 0xffff;
        while (length > 0) {
            Int32 max = length > mBuf->GetLength() ? mBuf->GetLength() : length;
            Int32 result;
            FAIL_RETURN(mIn->Read(mBuf, 0, max, &result));
            if (result == -1) {
    //            throw new EOFException();
                return E_EOF_EXCEPTION;
            }
            if (hcrc) {
                mCrc->Update(mBuf, 0, result);
            }
            length -= result;
        }
    }
    if ((flags & FNAME) != 0) {
        FAIL_RETURN(ReadZeroTerminated(hcrc));
    }
    if ((flags & FCOMMENT) != 0) {
        FAIL_RETURN(ReadZeroTerminated(hcrc));
    }
    if (hcrc) {
        FAIL_RETURN(ReadFully(0, 2, header));

        Int16 crc16 = Memory::PeekInt16(header, 0, ByteOrder_LITTLE_ENDIAN);
        Int64 value;
        mCrc->GetValue(&value);
        if ((Int16)value != crc16) {
    //        throw new IOException("CRC mismatch");
            return E_IO_EXCEPTION;
        }
        mCrc->Reset();
    }
    return NOERROR;
}

} // namespace Zip
} // namespace Utility
} // namespace Elastos
