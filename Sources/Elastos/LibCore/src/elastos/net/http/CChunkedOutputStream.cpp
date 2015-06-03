
#include "CChunkedOutputStream.h"
#include <elastos/Math.h>
#include <CByteArrayOutputStream.h>

using Elastos::IO::CByteArrayOutputStream;

namespace Elastos {
namespace Net {
namespace Http {

const Byte CChunkedOutputStream::CRLF[2] = { '\r', '\n' };
const Byte CChunkedOutputStream::HEX_DIGITS[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
const Byte CChunkedOutputStream::FINAL_CHUNK[5] = { '0', '\r', '\n', '\r', '\n' };


ECode CChunkedOutputStream::constructor(
    /* [in] */ IOutputStream* socketOut,
    /* [in] */ Int32 maxChunkLength)
{
    mHex[0] = 0;
    mHex[1] = 0;
    mHex[2] = 0;
    mHex[3] = 0;
    mHex[4] = 0;
    mHex[5] = 0;
    mHex[6] = 0;
    mHex[7] = 0;
    mHex[8] = '\r';
    mHex[9] = '\n';
    mSocketOut = socketOut;
    mMaxChunkLength = Elastos::Core::Math::Max(1, DataLength(maxChunkLength));
    FAIL_RETURN(CByteArrayOutputStream::New(mMaxChunkLength, (IByteArrayOutputStream**)&mBufferedChunk));

    return NOERROR;
}

ECode CChunkedOutputStream::Write(
    /* [in] */ Int32 oneByte)
{
    return AbstractHttpOutputStream::Write(oneByte);
}

ECode CChunkedOutputStream::WriteBytes(
    /* [in] */ const ArrayOf<Byte>& buffer)
{
    return AbstractHttpOutputStream::WriteBytes(buffer);
}

ECode CChunkedOutputStream::WriteBytes(
    /* [in] */ const ArrayOf<Byte>& buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count)
{
    FAIL_RETURN(AbstractHttpOutputStream::CheckNotClosed());
    // Arrays.checkOffsetAndCount(buffer.length, offset, count);
    Int32 packetlen = buffer.GetLength();
    if ((offset | count) < 0 || offset > packetlen || packetlen - offset < count) {
        // throw new ArrayIndexOutOfBoundsException(packetlen, offset, byteCount);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    while (count > 0) {
        Int32 numBytesWritten = 0;
        Int32 bufsize = 0;
        if ((mBufferedChunk->GetSize(&bufsize), bufsize) > 0 || count < mMaxChunkLength) {
            // fill the buffered chunk and then maybe write that to the stream
            numBytesWritten = Elastos::Core::Math::Min(count, mMaxChunkLength - bufsize);
            // TODO: skip unnecessary copies from buffer->bufferedChunk?
            mBufferedChunk->WriteBytes(buffer, offset, numBytesWritten);
            if (bufsize == mMaxChunkLength) {
                WriteBufferedChunkToSocket();
            }

        } else {
            // write a single chunk of size maxChunkLength to the stream
            numBytesWritten = mMaxChunkLength;
            WriteHex(numBytesWritten);
            mSocketOut->WriteBytes(buffer, offset, numBytesWritten);
            mSocketOut->Write((Int32)CRLF);
        }

        offset += numBytesWritten;
        count -= numBytesWritten;
    }

    return NOERROR;
}

ECode CChunkedOutputStream::CheckError(
    /* [out] */ Boolean* hasError)
{
    return AbstractHttpOutputStream::CheckError(hasError);
}

ECode CChunkedOutputStream::Close()
{
    if (mClosed) {
        return NOERROR;
    }
    mClosed = TRUE;
    WriteBufferedChunkToSocket();
    return mSocketOut->Write(FINAL_CHUNK);
}

ECode CChunkedOutputStream::Flush()
{
    if (mClosed) {
        return NOERROR; // don't throw; this stream might have been closed on the caller's behalf
    }
    WriteBufferedChunkToSocket();
    // mSocketOut->Flush();
    return NOERROR;
}

Int32 CChunkedOutputStream::DataLength(
    /* [in] */ Int32 dataPlusHeaderLength)
{
    Int32 headerLength = 4; // "\r\n" after the size plus another "\r\n" after the data
    for (Int32 i = dataPlusHeaderLength - headerLength; i > 0; i >>= 4) {
        headerLength++;
    }
    return dataPlusHeaderLength - headerLength;
}

void CChunkedOutputStream::WriteHex(
    /* [in] */ Int32 i)
{
    Int32 cursor = 8;
    do {
        mHex[--cursor] = HEX_DIGITS[i & 0xf];
    } while ((i >>= 4) != 0);

    AutoPtr< ArrayOf<Byte> > res = ArrayOf<Byte>::Alloc(10);
    for (Int32 i = 0; i < res->GetLength(); ++i) {
        (*res)[i] = mHex[i];
    }
    mSocketOut->WriteBytes(*res, cursor, res->GetLength() - cursor);
}

void CChunkedOutputStream::WriteBufferedChunkToSocket()
{
    Int32 size = 0;
    mBufferedChunk->GetSize(&size);
    if (size <= 0) {
        return;
    }

    WriteHex(size);
    mBufferedChunk->WriteTo(mSocketOut);
    mBufferedChunk->Reset();
    mSocketOut->Write((Int32)CRLF);
}


} // namespace Http
} // namespace Net
} // namespace Elastos
