
#include "cmdef.h"
#include "CChunkedInputStream.h"
#include <elastos/Math.h>
#include <elastos/StringUtils.h>
#include <CStreams.h>

using Elastos::IO::IStreams;
using Elastos::IO::CStreams;
using Elastos::Core::StringUtils;

namespace Elastos {
namespace Net {
namespace Http {


const Int32 CChunkedInputStream::MIN_LAST_CHUNK_LENGTH = String("\r\n0\r\n\r\n").GetLength();
const Int32 CChunkedInputStream::NO_CHUNK_YET = -1;

ECode CChunkedInputStream::constructor(
    /* [in] */ IInputStream* is,
    /* [in] */ ICacheRequest* cacheRequest,
    /* [in] */ IHttpEngine* httpEngine)
{
    mBytesRemainingInChunk = NO_CHUNK_YET;
    mHasMoreChunks = TRUE;
    return AbstractHttpInputStream::Init(is, httpEngine, cacheRequest);
}

ECode CChunkedInputStream::Available(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number)

    FAIL_RETURN(AbstractHttpInputStream::CheckNotClosed());
    if (!mHasMoreChunks || mBytesRemainingInChunk == NO_CHUNK_YET) {
        *number = 0;
        return NOERROR;
    }
    Int32 invalue = 0;
    mIn->Available(&invalue);
    *number = Elastos::Core::Math::Min(invalue, mBytesRemainingInChunk);
    return NOERROR;
}

ECode CChunkedInputStream::Mark(
    /* [in] */ Int32 readLimit)
{
    return AbstractHttpInputStream::Mark(readLimit);
}

ECode CChunkedInputStream::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    return AbstractHttpInputStream::IsMarkSupported(supported);
}

ECode CChunkedInputStream::Read(
    /* [out] */ Int32* value)
{
    return AbstractHttpInputStream::Read(value);
}

ECode CChunkedInputStream::ReadBytes(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    return AbstractHttpInputStream::ReadBytes(buffer, number);
}

ECode CChunkedInputStream::ReadBytesEx(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number)

    // Arrays.checkOffsetAndCount(buffer.length, offset, length);
    Int32 packetlen = buffer->GetLength();
    if ((offset | length) < 0 || offset > packetlen || packetlen - offset < length) {
        // throw new ArrayIndexOutOfBoundsException(packetlen, offset, byteCount);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    FAIL_RETURN(AbstractHttpInputStream::CheckNotClosed());

    if (!mHasMoreChunks) {
        *number = -1;
        return NOERROR;
    }
    if (mBytesRemainingInChunk == 0 || mBytesRemainingInChunk == NO_CHUNK_YET) {
        ReadChunkSize();
        if (!mHasMoreChunks) {
            *number = -1;
            return NOERROR;
        }
    }
    Int32 read = 0;
    mIn->ReadBytesEx(buffer, offset, Elastos::Core::Math::Min(length, mBytesRemainingInChunk), &read);
    if (read == -1) {
        UnexpectedEndOfInput(); // the server didn't supply the promised chunk length
        // throw new IOException("unexpected end of stream");
        return E_IO_EXCEPTION;
    }
    mBytesRemainingInChunk -= read;
    CacheWrite(buffer, offset, read);

    /*
     * If we're at the end of a chunk and the next chunk size is readable,
     * read it! Reading the last chunk causes the underlying connection to
     * be recycled and we want to do that as early as possible. Otherwise
     * self-delimiting streams like gzip will never be recycled.
     * http://code.google.com/p/android/issues/detail?id=7059
     */
    Int32 invalue = 0;
    mIn->Available(&invalue);
    if (mBytesRemainingInChunk == 0 && invalue >= MIN_LAST_CHUNK_LENGTH) {
        ReadChunkSize();
    }

    *number = read;
    return NOERROR;
}

ECode CChunkedInputStream::Reset()
{
    return AbstractHttpInputStream::Reset();
}

ECode CChunkedInputStream::Skip(
    /* [in] */ Int64 byteCount,
    /* [out] */ Int64* number)
{
    return AbstractHttpInputStream::Skip(byteCount, number);
}

ECode CChunkedInputStream::Close()
{
    if (mClosed) {
        return NOERROR;
    }

    mClosed = true;
    if (mHasMoreChunks) {
        UnexpectedEndOfInput();
    }
    return NOERROR;
}

void CChunkedInputStream::ReadChunkSize()
{
    // read the suffix of the previous chunk
    AutoPtr<IStreams> stream;
    CStreams::AcquireSingleton((IStreams**)&stream);
    if (mBytesRemainingInChunk != NO_CHUNK_YET) {
        String str;
        stream->ReadAsciiLine(mIn, &str);
    }
    String chunkSizeString;
    stream->ReadAsciiLine(mIn, &chunkSizeString);
    Int32 index = chunkSizeString.IndexOf(";");
    if (index != -1) {
        chunkSizeString = chunkSizeString.Substring(0, index);
    }
    // try {
    mBytesRemainingInChunk = StringUtils::ParseInt32(chunkSizeString.Trim(), 16);
    // } catch (NumberFormatException e) {
    //     throw new IOException("Expected a hex chunk size, but was " + chunkSizeString);
    // }
    if (mBytesRemainingInChunk == 0) {
        mHasMoreChunks = FALSE;
        mHttpEngine->ReadTrailers();
        EndOfInput(TRUE);
    }
}

PInterface CChunkedInputStream::Probe(
    /* [in] */ REIID riid)
{
    return _CChunkedInputStream::Probe(riid);
}

ECode CChunkedInputStream::GetLock(
    /* [out] */ IInterface** lockobj)
{
    VALIDATE_NOT_NULL(lockobj);

    AutoPtr<IInterface> obj = AbstractHttpInputStream::GetLock();
    *lockobj = obj;
    INTERFACE_ADDREF(*lockobj);
    return NOERROR;
}

} // namespace Http
} // namespace Net
} // namespace Elastos
