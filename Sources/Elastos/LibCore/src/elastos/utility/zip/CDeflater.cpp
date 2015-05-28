
#include "CDeflater.h"

namespace Elastos {
namespace Utility {
namespace Zip {


const Int32 CDeflater::NO_FLUSH;
const Int32 CDeflater::SYNC_FLUSH;
const Int32 CDeflater::FULL_FLUSH;
const Int32 CDeflater::FINISH;

CDeflater::CDeflater()
    : mFlushParm(NO_FLUSH)
    , mFinished(FALSE)
    , mCompressLevel(DEFAULT_COMPRESSION)
    , mStrategy(DEFAULT_STRATEGY)
    , mStreamHandle(NULL)
    , mInRead(0)
    , mInLength(0)
{}

CDeflater::~CDeflater()
{
//    try {
    // if (guard != null) {
    //     guard.warnIfOpen();
    // }
    End(); // to allow overriding classes to clean up
//    endImpl(); // in case those classes don't call super.end()
//    } finally {
//        try {
//            super.finalize();
//        } catch (Throwable t) {
//            throw new AssertionError(t);
//        }
//    }
}

ECode CDeflater::Deflate(
    /* [out] */ ArrayOf<Byte>* buf,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buf);
    VALIDATE_NOT_NULL(number);

    return Deflate(buf, 0, buf->GetLength(), number);
}

ECode CDeflater::Deflate(
    /* [out] */ ArrayOf<Byte>* buf,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 byteCount,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buf);
    VALIDATE_NOT_NULL(number);

    Mutex::Autolock lock(&_m_syncLock);

    return DeflateImplLocked(offset, byteCount, mFlushParm, buf, number);
}

ECode CDeflater::Deflate(
    /* [out] */ ArrayOf<Byte>* buf,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 byteCount,
    /* [in] */ Int32 flush,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(buf);
    VALIDATE_NOT_NULL(number);

    Mutex::Autolock lock(&_m_syncLock);

    if (flush != NO_FLUSH && flush != SYNC_FLUSH && flush != FULL_FLUSH) {
//        throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return DeflateImplLocked(offset, byteCount, flush, buf, number);
}

ECode CDeflater::DeflateImplLocked(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 byteCount,
    /* [in] */ Int32 flush,
    /* [out] */ ArrayOf<Byte>* buf,
    /* [out] */ Int32* number)
{
    FAIL_RETURN(CheckOpen());

//    Arrays.checkOffsetAndCount(buf.length, offset, byteCount);
    if ((offset | byteCount) < 0 || offset > buf->GetLength() ||
            buf->GetLength() - offset < byteCount) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new ArrayIndexOutOfBoundsException(arrayLength, offset, count);
    }
    if (mInputBuffer == NULL) {
        AutoPtr<ArrayOf<Byte> > emptyByteArray = ArrayOf<Byte>::Alloc(0);
        SetInput(*emptyByteArray);
    }
    *number = DeflateImplLocked(offset, byteCount, flush, mStreamHandle, buf);
    return NOERROR;
}

Int32 CDeflater::DeflateImplLocked(
    /* [in] */ Int32 offset,
    /* [in] */ Int32 byteCount,
    /* [in] */ Int32 flushParm,
    /* [in] */ NativeZipStream* stream,
    /* [out] */ ArrayOf<Byte>* buf)
{
    // /* We need to get the number of bytes already read */
    // Int32 inBytes = mInRead;

    // stream->mStream.avail_out = byteCount;
    // Int32 sin = stream->mStream.total_in;
    // Int32 sout = stream->mStream.total_out;
    if (buf == NULL) {
        return -1;
    }
    stream->mStream.next_out = reinterpret_cast<Bytef*>(buf->GetPayload() + offset);
    stream->mStream.avail_out = byteCount;

    Bytef* initialNextIn = stream->mStream.next_in;
    Bytef* initialNextOut = stream->mStream.next_out;

    Int32 err = deflate(&stream->mStream, flushParm);
    switch (err) {
    case Z_OK:
        break;
    case Z_STREAM_END:
        mFinished = TRUE;
        break;
    case Z_BUF_ERROR:
        // zlib reports this "if no progress is possible (for example avail_in or avail_out was
        // zero) ... Z_BUF_ERROR is not fatal, and deflate() can be called again with more
        // input and more output space to continue compressing".
        break;
    default:
//        throwExceptionForZlibError(env, "java/util/zip/DataFormatException", err);
        return -1;
    }

    Int32 bytesRead = (Int32)stream->mStream.next_in - (Int32)initialNextIn;
    Int32 bytesWritten = (Int32)stream->mStream.next_out - (Int32)initialNextOut;
    mInRead += bytesRead;
    return bytesWritten;
}

void CDeflater::EndImplLocked(
    /* [in] */ NativeZipStream* stream)
{
    deflateEnd(&stream->mStream);
    delete stream;
}

ECode CDeflater::End()
{
    Mutex::Autolock lock(&_m_syncLock);

//    guard.close();
    EndImplLocked();
    return NOERROR;
}

void CDeflater::EndImplLocked()
{
    if (mStreamHandle != NULL) {
        EndImplLocked(mStreamHandle);
        mInputBuffer = NULL;
        mStreamHandle = NULL;
    }
}

ECode CDeflater::Finish()
{
    Mutex::Autolock lock(&_m_syncLock);

    mFlushParm = FINISH;
    return NOERROR;
}

ECode CDeflater::Finished(
    /* [out] */ Boolean* finished)
{
    VALIDATE_NOT_NULL(finished);

    Mutex::Autolock lock(&_m_syncLock);
    *finished = mFinished;
    return NOERROR;
}

ECode CDeflater::GetAdler(
    /* [out] */ Int32* checksum)
{
    VALIDATE_NOT_NULL(checksum);

    Mutex::Autolock lock(&_m_syncLock);

    FAIL_RETURN(CheckOpen());

    *checksum = GetAdlerImplLocked(mStreamHandle);
    return NOERROR;
}

Int32 CDeflater::GetAdlerImplLocked(
    /* [in] */ NativeZipStream* stream)
{
    return stream->mStream.adler;
}

ECode CDeflater::GetTotalIn(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);

    Mutex::Autolock lock(&_m_syncLock);

    FAIL_RETURN(CheckOpen());

    *number = (Int32)GetTotalInImplLocked(mStreamHandle);
    return NOERROR;
}

Int64 CDeflater::GetTotalInImplLocked(
    /* [in] */ NativeZipStream* stream)
{
    return stream->mStream.total_in;
}

ECode CDeflater::GetTotalOut(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);

    Mutex::Autolock lock(&_m_syncLock);

    FAIL_RETURN(CheckOpen());

    *number = (Int32)GetTotalOutImplLocked(mStreamHandle);
    return NOERROR;
}

Int64 CDeflater::GetTotalOutImplLocked(
    /* [in] */ NativeZipStream* stream)
{
    return stream->mStream.total_out;
}

ECode CDeflater::NeedsInput(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    Mutex::Autolock lock(&_m_syncLock);

    if (mInputBuffer == NULL) {
        *result = TRUE;
        return NOERROR;
    }
    *result = mInRead == mInLength;
    return NOERROR;
}

ECode CDeflater::Reset()
{
    Mutex::Autolock lock(&_m_syncLock);

    FAIL_RETURN(CheckOpen());

    mFlushParm = NO_FLUSH;
    mFinished = FALSE;
    mInputBuffer = NULL;
    return ResetImplLocked(mStreamHandle);
}

ECode CDeflater::ResetImplLocked(
    /* [in] */ NativeZipStream* stream)
{
    Int32 err = deflateReset(&stream->mStream);
    if (err != Z_OK) {
//        throwExceptionForZlibError(env, "java/lang/IllegalArgumentException", err);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

ECode CDeflater::SetDictionary(
    /* [in] */ const ArrayOf<Byte>& buf)
{
    return SetDictionary(buf, 0, buf.GetLength());
}

ECode CDeflater::SetDictionary(
    /* [in] */ const ArrayOf<Byte>& buf,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 byteCount)
{
    Mutex::Autolock lock(&_m_syncLock);

    FAIL_RETURN(CheckOpen());
    //Arrays.checkOffsetAndCount(buf.length, offset, byteCount);
    if ((offset | byteCount) < 0 || offset > buf.GetLength() ||
            buf.GetLength() - offset < byteCount) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new ArrayIndexOutOfBoundsException(arrayLength, offset, byteCount);
    }
    SetDictionaryImplLocked(buf, offset, byteCount, mStreamHandle);
    return NOERROR;
}

void CDeflater::SetDictionaryImplLocked(
    /* [in] */ const ArrayOf<Byte>& buf,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 byteCount,
    /* [in] */ NativeZipStream* stream)
{
    stream->SetDictionary(buf, offset, byteCount, FALSE);
}

ECode CDeflater::SetInput(
    /* [in] */ const ArrayOf<Byte>& buf)
{
    return SetInput(buf, 0, buf.GetLength());
}

ECode CDeflater::SetInput(
    /* [in] */ const ArrayOf<Byte>& buf,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 byteCount)
{
    Mutex::Autolock lock(&_m_syncLock);

    FAIL_RETURN(CheckOpen());
    //Arrays.checkOffsetAndCount(buf.length, offset, byteCount);
    if ((offset | byteCount) < 0 || offset > buf.GetLength() ||
            buf.GetLength() - offset < byteCount) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new ArrayIndexOutOfBoundsException(arrayLength, offset, byteCount);
    }

    mInLength = byteCount;
    mInRead = 0;
    if (mInputBuffer == NULL) {
        FAIL_RETURN(SetLevelsImplLocked(mCompressLevel,
                    mStrategy, mStreamHandle));
    }
    mInputBuffer = buf.Clone();
    SetInputImplLocked(buf, offset, byteCount, mStreamHandle);
    return NOERROR;
}

ECode CDeflater::SetLevelsImplLocked(
    /* [in] */ Int32 level,
    /* [in] */ Int32 strategy,
    /* [in] */ NativeZipStream* stream)
{
    // The deflateParams documentation says that avail_out must never be 0 because it may be
    // necessary to flush, but the Java API ensures that we only get here if there's nothing
    // to flush. To be on the safe side, make sure that we're not pointing to a no longer valid
    // buffer.
    stream->mStream.next_out = reinterpret_cast<Bytef*>(NULL);
    stream->mStream.avail_out = 0;
    Int32 err = deflateParams(&stream->mStream, level, strategy);
    if (err != Z_OK) {
//        throwExceptionForZlibError(env, "java/lang/IllegalStateException", err);
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    return NOERROR;
}

void CDeflater::SetInputImplLocked(
    /* [in] */ const ArrayOf<Byte>& buf,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 byteCount,
    /* [in] */ NativeZipStream* stream)
{
    stream->SetInput(const_cast<ArrayOf<Byte>*>(&buf), offset, byteCount);
}

ECode CDeflater::SetLevel(
    /* [in] */ Int32 level)
{
    Mutex::Autolock lock(&_m_syncLock);

    if (level < DEFAULT_COMPRESSION ||
            level > BEST_COMPRESSION) {
//        throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (mInputBuffer != NULL) {
//        throw new IllegalStateException();
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    mCompressLevel = level;
    return NOERROR;
}

ECode CDeflater::SetStrategy(
    /* [in] */ Int32 strategy)
{
    Mutex::Autolock lock(&_m_syncLock);

    if (strategy < DEFAULT_STRATEGY ||
        strategy > HUFFMAN_ONLY) {
//        throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (mInputBuffer != NULL) {
//        throw new IllegalStateException();
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    mStrategy = strategy;
    return NOERROR;
}

ECode CDeflater::GetBytesRead(
    /* [out] */ Int64* number)
{
    VALIDATE_NOT_NULL(number);

    Mutex::Autolock lock(&_m_syncLock);

    FAIL_RETURN(CheckOpen());
    *number = GetTotalInImplLocked(mStreamHandle);
    return NOERROR;
}

ECode CDeflater::GetBytesWritten(
    /* [out] */ Int64* number)
{
    VALIDATE_NOT_NULL(number);

    Mutex::Autolock lock(&_m_syncLock);

    FAIL_RETURN(CheckOpen());
    *number = GetTotalOutImplLocked(mStreamHandle);
    return NOERROR;
}

ECode CDeflater::CreateStream(
    /* [in] */ Int32 level,
    /* [in] */ Int32 strategy,
    /* [in] */ Boolean noHeader)
{
    NativeZipStream* stream = new NativeZipStream;
    if (stream == NULL) {
        mStreamHandle = NULL;
        return E_OUT_OF_MEMORY_ERROR;
    }
    /*
     * See zlib.h for documentation of the deflateInit2 windowBits and memLevel parameters.
     *
     * zconf.h says the "requirements for deflate are (in bytes):
     *         (1 << (windowBits+2)) +  (1 << (memLevel+9))
     * that is: 128K for windowBits=15  +  128K for memLevel = 8  (default values)
     * plus a few kilobytes for small objects."
     */
    Int32 windowBits = noHeader ? -DEF_WBITS : DEF_WBITS;
    Int32 memLevel = DEF_MEM_LEVEL;

    Int32 err = deflateInit2(&stream->mStream, level, Z_DEFLATED, windowBits,
            memLevel, strategy);
    if (err != Z_OK) {
        delete stream;
        mStreamHandle = NULL;
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throwExceptionForZlibError(env, "java/lang/IllegalArgumentException", err);
    }
    mStreamHandle = stream;
    return NOERROR;
}

ECode CDeflater::CheckOpen()
{
    if (mStreamHandle == NULL) {
        return E_ILLEGAL_STATE_EXCEPTION;
//        throw new IllegalStateException("attempt to use Deflater after calling end");
    }
    return NOERROR;
}

ECode CDeflater::constructor()
{
    return constructor(DEFAULT_COMPRESSION, FALSE);
}

ECode CDeflater::constructor(
    /* [in] */ Int32 level)
{
    return constructor(level, FALSE);
}

ECode CDeflater::constructor(
    /* [in] */ Int32 level,
    /* [in] */ Boolean noHeader)
{
    if (level < DEFAULT_COMPRESSION ||
        level > BEST_COMPRESSION) {
//        throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mCompressLevel = level;
    return CreateStream(mCompressLevel, mStrategy, noHeader);
}


} // namespace Zip
} // namespace Utility
} // namespace Elastos
