
#include "CInflater.h"
#include <elastos/Math.h>
#include <unistd.h>
#include <errno.h>

using Elastos::Core::Math;

namespace Elastos {
namespace Utility {
namespace Zip {

CInflater::CInflater()
    : mInLength(0)
    , mInRead(0)
    , mFinished(FALSE)
    , mNeedsDictionary(FALSE)
    , mStreamHandle(NULL)
{}

CInflater::~CInflater()
{
//    try {
    // if (guard != NULL) {
    //     guard.warnIfOpen();
    // }
    End();
//    } finally {
//        try {
//            super.finalize();
//        } catch (Throwable t) {
//            throw new AssertionError(t);
//        }
//    }
}

ECode CInflater::CreateStream(
    /* [in] */ Boolean noHeader)
{
    NativeZipStream* stream = new NativeZipStream;
    if (stream == NULL) {
//        jniThrowOutOfMemoryError(env, NULL);
        mStreamHandle = NULL;
        return E_OUT_OF_MEMORY_ERROR;
    }
    stream->mStream.adler = 1;

    /*
     * See zlib.h for documentation of the inflateInit2 windowBits parameter.
     *
     * zconf.h says the "requirements for inflate are (in bytes) 1 << windowBits
     * that is, 32K for windowBits=15 (default value) plus a few kilobytes
     * for small objects." This means that we can happily use the default
     * here without worrying about memory consumption.
     */
    Int32 err = inflateInit2(&stream->mStream, noHeader ? -DEF_WBITS : DEF_WBITS);
    if (err != Z_OK) {
//        throwExceptionForZlibError(env, "java/lang/IllegalArgumentException", err);
        delete stream;
        mStreamHandle = NULL;
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mStreamHandle = stream;
    return NOERROR;
}

ECode CInflater::End()
{
    Mutex::Autolock lock(&_m_syncLock);

    //guard.close();
    if (mStreamHandle != NULL) {
        EndImplLocked(mStreamHandle);
        mInRead = 0;
        mInLength = 0;
        mStreamHandle = NULL;
    }

    return NOERROR;
}

void CInflater::EndImplLocked(
    /* [in] */ NativeZipStream* stream)
{
    inflateEnd(&stream->mStream);
    delete stream;
}

ECode CInflater::Finished(
    /* [out] */ Boolean* finished)
{
    VALIDATE_NOT_NULL(finished);

    Mutex::Autolock lock(&_m_syncLock);

    *finished = mFinished;
    return NOERROR;
}

ECode CInflater::GetAdler(
    /* [out] */ Int32* checksum)
{
    VALIDATE_NOT_NULL(checksum);

    Mutex::Autolock lock(&_m_syncLock);

    FAIL_RETURN(CheckOpen());
    *checksum = GetAdlerImplLocked(mStreamHandle);

    return NOERROR;
}

Int32 CInflater::GetAdlerImplLocked(
    /* [in] */ NativeZipStream* stream)
{
    return stream->mStream.adler;
}

ECode CInflater::GetBytesRead(
    /* [out] */ Int64* number)
{
    VALIDATE_NOT_NULL(number);

    Mutex::Autolock lock(&_m_syncLock);

    FAIL_RETURN(CheckOpen());
    *number = GetTotalInImplLocked(mStreamHandle);

    return NOERROR;
}

ECode CInflater::GetBytesWritten(
    /* [out] */ Int64* number)
{
    VALIDATE_NOT_NULL(number);

    Mutex::Autolock lock(&_m_syncLock);

    FAIL_RETURN(CheckOpen());
    *number = GetTotalOutImplLocked(mStreamHandle);
    return NOERROR;
}

ECode CInflater::GetRemaining(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);

    Mutex::Autolock lock(&_m_syncLock);

    *number = mInLength - mInRead;

    return NOERROR;
}

ECode CInflater::GetTotalIn(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);

    Mutex::Autolock lock(&_m_syncLock);

    FAIL_RETURN(CheckOpen());
    *number = (Int32)Elastos::Core::Math::Min(
            GetTotalInImplLocked(mStreamHandle), (Int64)Elastos::Core::Math::INT32_MAX_VALUE);
    return NOERROR;
}

Int64 CInflater::GetTotalInImplLocked(
    /* [in] */ NativeZipStream* stream)
{
    return stream->mStream.total_in;
}

ECode CInflater::GetTotalOut(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);

    Mutex::Autolock lock(&_m_syncLock);

    FAIL_RETURN(CheckOpen());
    *number = (Int32)Elastos::Core::Math::Min(
            GetTotalOutImplLocked(mStreamHandle), (Int64)Elastos::Core::Math::INT32_MAX_VALUE);
    return NOERROR;
}

Int64 CInflater::GetTotalOutImplLocked(
    /* [in] */ NativeZipStream* stream)
{
    return stream->mStream.total_out;
}

ECode CInflater::Inflate(
    /* [out] */ ArrayOf<Byte>* buf,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buf);
    VALIDATE_NOT_NULL(number);

    return Inflate(buf, 0, buf->GetLength(), number);
}

ECode CInflater::Inflate(
    /* [out] */ ArrayOf<Byte>* buf,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 byteCount,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    *number = -1;
    VALIDATE_NOT_NULL(buf);

    Mutex::Autolock lock(&_m_syncLock);

    Int32 arrayLength = buf->GetLength();
    if ((offset | byteCount) < 0 || offset > arrayLength || arrayLength - offset < byteCount) {
//        throw new ArrayIndexOutOfBoundsException();
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    FAIL_RETURN(CheckOpen());

    Boolean need;
    NeedsInput(&need);
    if (need) {
        *number = 0;
        return NOERROR;
    }

    Boolean neededDict = mNeedsDictionary;
    mNeedsDictionary = FALSE;
    Int32 result;
    FAIL_RETURN(InflateImplLocked(offset, byteCount, buf, mStreamHandle, &result));
    if (mNeedsDictionary && neededDict) {
//        throw new DataFormatException("Needs dictionary");
        return E_DATA_FORMAT_EXCEPTION;
    }

    *number = result;
    return NOERROR;
}

ECode CInflater::InflateImplLocked(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 byteCount,
    /* [out] */ ArrayOf<Byte>* buf,
    /* [in] */ NativeZipStream* stream,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = -1;
    if (buf == NULL) return NOERROR;

    stream->mStream.next_out = reinterpret_cast<Bytef*>(buf->GetPayload() + offset);
    stream->mStream.avail_out = byteCount;

    Bytef* initialNextIn = stream->mStream.next_in;
    Bytef* initialNextOut = stream->mStream.next_out;

    Int32 err = inflate(&stream->mStream, Z_SYNC_FLUSH);
    if (err != Z_OK) {
        if (err == Z_STREAM_ERROR) {
            *result =0;
            return NOERROR;
        }
        if (err == Z_STREAM_END) {
            mFinished = TRUE;
        }
        else if (err == Z_NEED_DICT) {
            mNeedsDictionary = TRUE;
        }
        else {
            // PFL_EX(" DataFormatException: err: %d", err);
//            throwExceptionForZlibError(env, "java/util/zip/DataFormatException", err);
            *result = -1;
            return E_DATA_FORMAT_EXCEPTION;
        }
    }

    Int32 bytesRead = stream->mStream.next_in - initialNextIn;
    Int32 bytesWritten = stream->mStream.next_out - initialNextOut;

    mInRead += bytesRead;
    *result = bytesWritten;
    PFL_EX(" >> bytesWritten: %d", bytesWritten);
    return NOERROR;
}

ECode CInflater::NeedsDictionary(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    Mutex::Autolock lock(&_m_syncLock);

    *result = mNeedsDictionary;
    return NOERROR;
}

ECode CInflater::NeedsInput(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    Mutex::Autolock lock(&_m_syncLock);

    *result = mInRead == mInLength;
    return NOERROR;
}

ECode CInflater::Reset()
{
    Mutex::Autolock lock(&_m_syncLock);

    FAIL_RETURN(CheckOpen());
    mFinished = FALSE;
    mNeedsDictionary = FALSE;
    mInLength = mInRead = 0;
    return ResetImplLocked(mStreamHandle);
}

ECode CInflater::ResetImplLocked(
    /* [in] */ NativeZipStream* stream)
{
    Int32 err = inflateReset(&stream->mStream);
    if (err != Z_OK) {
//        throwExceptionForZlibError(env, "java/lang/IllegalArgumentException", err);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

ECode CInflater::SetDictionary(
    /* [in] */ const ArrayOf<Byte>& buf)
{
    return SetDictionary(buf, 0, buf.GetLength());
}

ECode CInflater::SetDictionary(
    /* [in] */ const ArrayOf<Byte>& buf,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 byteCount)
{
    Mutex::Autolock lock(&_m_syncLock);

    FAIL_RETURN(CheckOpen());
//    Arrays.checkOffsetAndCount(dictionary.length, offset, byteCount);
    if (offset <= buf.GetLength() && byteCount >= 0 && offset >= 0
            && buf.GetLength() - offset >= byteCount) {
        SetDictionaryImplLocked(buf, offset, byteCount, mStreamHandle);
        return NOERROR;
    }
    else {
//        throw new ArrayIndexOutOfBoundsException();
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
}

void CInflater::SetDictionaryImplLocked(
    /* [in] */ const ArrayOf<Byte>& buf,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 byteCount,
    /* [in] */ NativeZipStream* stream)
{
    stream->SetDictionary(buf, offset, byteCount, TRUE);
}

ECode CInflater::SetInput(
    /* [in] */ const ArrayOf<Byte>& buf)
{
    return SetInput(buf, 0, buf.GetLength());
}

ECode CInflater::SetInput(
    /* [in] */ const ArrayOf<Byte>& buf,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 byteCount)
{
    Mutex::Autolock lock(&_m_syncLock);

    FAIL_RETURN(CheckOpen());

    Int32 arrayLength = buf.GetLength();
    if ((offset | byteCount) < 0 || offset > arrayLength || arrayLength - offset < byteCount)
    {
//        throw new ArrayIndexOutOfBoundsException();
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    mInRead = 0;
    mInLength = byteCount;
    SetInputImplLocked(buf, offset, byteCount, mStreamHandle);
    return NOERROR;
}

Int32 CInflater::SetFileInput(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int64 offset,
    /* [in] */ Int32 byteCount)
{
    Mutex::Autolock lock(&_m_syncLock);

    FAIL_RETURN(CheckOpen());
    mInRead = 0;
    mInLength = SetFileInputImplLocked(fd, offset, byteCount, mStreamHandle);
    return mInLength;
}

void CInflater::SetInputImplLocked(
    /* [in] */ const ArrayOf<Byte>& buf,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 byteCount,
    /* [in] */ NativeZipStream* stream)
{
    stream->SetInput(&buf, offset, byteCount);
}

Int32 CInflater::SetFileInputImplLocked(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int64 offset,
    /* [in] */ Int32 byteCount,
    /* [in] */ NativeZipStream* stream)
{

    // We reuse the existing native buffer if it's large enough.
    // TODO: benchmark.
    if (stream->mInCap < byteCount) {
        stream->SetInput(NULL, 0, byteCount);
    }
    else {
        stream->mStream.next_in = reinterpret_cast<Bytef*>(stream->mInput->GetPayload());
        stream->mStream.avail_in = byteCount;
    }

    // As an Android-specific optimization, we read directly onto the native heap.
    // The original code used Java to read onto the Java heap and then called setInput(byte[]).
    // TODO: benchmark.
    Int32 _fd;
    fd->GetDescriptor(&_fd);
    Int32 rc = TEMP_FAILURE_RETRY(lseek(_fd, offset, SEEK_SET));
    if (rc == -1) {
//        jniThrowIOException(env, errno);
        return 0;
    }
    Int32 totalByteCount = 0;
    Bytef* dst = reinterpret_cast<Bytef*>(stream->mInput->GetPayload());
    ssize_t len;
    while ((len = TEMP_FAILURE_RETRY(read(_fd, dst, byteCount))) > 0) {
        dst += len;
        byteCount -= len;
        totalByteCount += len;
    }
    if (len == -1) {
//        jniThrowIOException(env, errno);
        return 0;
    }
    return totalByteCount;
}

/**
 * This constructor creates an inflater that expects a header from the input
 * stream. Use {@code Inflater(boolean)} if the input comes without a ZLIB
 * header.
 */
ECode CInflater::constructor()
{
    return constructor(FALSE);
}

/**
 * This constructor allows to create an inflater that expects no header from
 * the input stream.
 *
 * @param noHeader
 *            {@code true} indicates that no ZLIB header comes with the
 *            input.
 */
ECode CInflater::constructor(
    /* [in] */ Boolean onHeader)
{
    return CreateStream(onHeader);
}

ECode CInflater::CheckOpen()
{
    if (mStreamHandle == NULL) {
        return E_ILLEGAL_STATE_EXCEPTION;
//        throw new IllegalStateException("attempt to use Inflater after calling end");
    }
    return NOERROR;
}


} // namespace Zip
} // namespace Utility
} // namespace Elastos
