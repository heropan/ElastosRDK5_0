
#include "InflaterInputStream.h"
#include "CStreams.h"
// #include "CZipFile.h"
#include "CInflater.h"

using Elastos::IO::EIID_IInputStream;
using Libcore::IO::IStreams;
using Libcore::IO::CStreams;

namespace Elastos {
namespace Utility {
namespace Zip {


const Int32 InflaterInputStream::BUF_SIZE;

CAR_INTERFACE_IMPL(InflaterInputStream, FilterInputStream, IInflaterInputStream)

InflaterInputStream::InflaterInputStream()
    : mClosed(FALSE)
    , mEof(FALSE)
    , mNativeEndBufSize(0)
    , mLen(0)
{}

InflaterInputStream::~InflaterInputStream()
{
}

ECode InflaterInputStream::Read(
    /* [out] */ Int32* value)
{
    AutoPtr<IStreams> streams;
    CStreams::AcquireSingleton((IStreams**)&streams);
    return streams->ReadSingleByte(THIS_PROBE(IInputStream), value);
}

ECode InflaterInputStream::Read(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 byteCount,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    *number = -1;
    VALIDATE_NOT_NULL(buffer);

    FAIL_RETURN(CheckClosed());

    Int32 arrayLength = buffer->GetLength();
    if ((offset | byteCount) < 0 || offset > arrayLength || arrayLength - offset < byteCount) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    if (byteCount == 0) {
        *number = 0;
        return NOERROR;
    }

    if (mEof) {
        *number = -1;
        return NOERROR;
    }

    Boolean need;
    Int32 result;
    ECode ec = NOERROR;
    do {
        mInf->NeedsInput(&need);
        if (need) {
            FAIL_RETURN(Fill());
        }

        // Invariant: if reading returns -1 or throws, eof must be true.
        // It may also be true if the next read() should return -1.
        ec = mInf->Inflate(buffer, offset, byteCount, &result);
        if (ec == E_DATA_FORMAT_EXCEPTION) goto _EXIT_;

        ec = mInf->Finished(&mEof);
        if (ec == E_DATA_FORMAT_EXCEPTION) goto _EXIT_;

        if (result > 0) {
            *number = result;
            return NOERROR;
        }
        else if (mEof) {
            *number = -1;
            return NOERROR;
        }
        else if (mInf->NeedsDictionary(&need), need) {
            mEof = TRUE;
            *number = -1;
            return NOERROR;
        }
        else if (mLen == -1) {
            mEof = TRUE;
//            throw new EOFException();
            return E_EOF_EXCEPTION;
            // If result == 0, fill() and try again
        }
    }
    while (TRUE);
    return NOERROR;

_EXIT_:
    mEof = TRUE;
    if (mLen == -1) {
//      throw new EOFException();
        return E_EOF_EXCEPTION;
    }
//      throw (IOException) (new IOException().initCause(e));
    return E_IO_EXCEPTION;
}

ECode InflaterInputStream::Fill()
{
    FAIL_RETURN(CheckClosed());

    // if (mNativeEndBufSize > 0) {
    //     CZipFile::RAFStream* is = (CZipFile::RAFStream*)mIn->Probe(EIID_ZipFileRAFStream);
    //     Object::Autolock locK(is->sLock);
    //     Int64 len = is->mLength - is->mOffset;
    //     if (len > mNativeEndBufSize) len = mNativeEndBufSize;
    //     AutoPtr<IFileDescriptor> fd;
    //     is->mSharedRaf->GetFD((IFileDescriptor**)&fd);
    //     Int32 cnt = mInf->SetFileInput(fd, is->mOffset, mNativeEndBufSize);
    //     Int64 result;
    //     is->Skip(cnt, &result);
    // }
    // else {
    //     mIn->Read(mBuf, &mLen);
    //     if (mLen > 0) {
    //         mInf->SetInput(mBuf, 0, mLen);
    //     }
    // }
    return NOERROR;
}

ECode InflaterInputStream::Skip(
    /* [in] */ Int64 byteCount,
    /* [out] */ Int64* number)
{
    VALIDATE_NOT_NULL(number);
    *number = -1;

    if (byteCount < 0) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("byteCount < 0");
    }

    AutoPtr<IStreams> streams;
    CStreams::AcquireSingleton((IStreams**)&streams);
    IInputStream* is = THIS_PROBE(IInputStream);
    assert(is != NULL);
    return streams->SkipByReading(is, byteCount, number);
}

ECode InflaterInputStream::Available(
    /* [out] */ Int32* number)
{
    FAIL_RETURN(CheckClosed());
    if (mEof) {
        *number = 0;
        return NOERROR;
    }
    *number = 1;
    return NOERROR;
}

ECode InflaterInputStream::Close()
{
    if (!mClosed) {
        mInf->End();
        mClosed = TRUE;
        mEof = TRUE;
        ICloseable::Probe(mIn)->Close();
    }
    return NOERROR;
}

ECode InflaterInputStream::Mark(
    /* [in] */ Int32 readlimit)
{
    // do nothing
    return NOERROR;
}

ECode InflaterInputStream::Reset()
{
//    throw new IOException();
    return E_IO_EXCEPTION;
}

ECode InflaterInputStream::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    *supported = FALSE;
    return NOERROR;
}

ECode InflaterInputStream::CheckClosed()
{
    if (mClosed) {
//        throw new IOException("Stream is closed");
        return E_IO_EXCEPTION;
    }
    return NOERROR;
}

ECode InflaterInputStream::Read(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    return Read(buffer,0, buffer->GetLength(), number);
}

ECode InflaterInputStream::constructor(
    /* [in] */ IInputStream* is)
{
    AutoPtr<CInflater> inflater;
    CInflater::NewByFriend((CInflater**)&inflater);
    return constructor(is, (IInflater*)inflater.Get(), BUF_SIZE);
}

ECode InflaterInputStream::constructor(
    /* [in] */ IInputStream* is,
    /* [in] */ IInflater* inflater)
{
    return constructor(is, inflater, BUF_SIZE);
}

ECode InflaterInputStream::constructor(
    /* [in] */ IInputStream* is,
    /* [in] */ IInflater* inflater,
    /* [in] */ Int32 bsize)
{
    mIn = is;
    if (is == NULL || inflater == NULL) {
//        throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }
    if (bsize <= 0) {
//        throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    mInf = inflater;
    // if (is->Probe(EIID_ZipFileRAFStream)) {
    //     mNativeEndBufSize = bsize;
    // }
    // else {
    //     mBuf = ArrayOf<Byte>::Alloc(bsize);
    // }

    return NOERROR;
}


} // namespace Zip
} // namespace Utility
} // namespace Elastos
