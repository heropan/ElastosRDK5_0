
#include "DeflaterOutputStream.h"

using Elastos::IO::IStreams;
using Elastos::IO::CStreams;
using Elastos::IO::EIID_IOutputStream;

namespace Elastos {
namespace Utility {
namespace Zip {

const Int32 DeflaterOutputStream::BUF_SIZE;

CAR_INTERFACE_IMPL(DeflaterOutputStream, FilterOutputStream, IDeflaterOutputStream)

DeflaterOutputStream::DeflaterOutputStream()
    : mDone(FALSE)
    , mSyncFlush(FALSE)
{}

DeflaterOutputStream::~DeflaterOutputStream()
{
}

ECode DeflaterOutputStream::Deflate()
{
    Int32 byteCount;
    while (mDef->Deflate(mBuf, &byteCount), byteCount != 0) {
        FAIL_RETURN(mOut->Write(mBuf, 0, byteCount));
    }
    return NOERROR;
}

ECode DeflaterOutputStream::Close()
{
    // everything closed here should also be closed in ZipOutputStream.close()
    Boolean finished;
    mDef->Finished(&finished);
    if (!finished) {
        Finish();
    }
    mDef->End();
    return mOut->Close();
}

ECode DeflaterOutputStream::Finish()
{
    if (mDone) {
        return NOERROR;
    }
    mDef->Finish();
    Boolean finished;
    while (mDef->Finished(&finished), !finished) {
        Int32 byteCount;
        FAIL_RETURN(mDef->Deflate(mBuf, &byteCount));
        FAIL_RETURN(mOut->Write(mBuf, 0, byteCount));
    }
    mDone = TRUE;
    return NOERROR;
}

ECode DeflaterOutputStream::Write(
    /* [in] */ Int32 i)
{
    AutoPtr<IStreams> streams;
    CStreams::AcquireSingleton((IStreams**)&streams);
    return streams->WriteSingleByte(THIS_PROBE(IOutputStream), i);
}

ECode DeflaterOutputStream::Write(
    /* [in] */ const ArrayOf<Byte>& buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 byteCount)
{
    if (mDone) {
//        throw new IOException("attempt to write after finish");
        return E_IO_EXCEPTION;
    }
    //Arrays.checkOffsetAndCount(buffer.length, offset, byteCount);
    if ((offset | byteCount) < 0 || offset > buffer.GetLength() ||
            buffer.GetLength() - offset < byteCount) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new ArrayIndexOutOfBoundsException(arrayLength, offset, count);
    }

    Boolean need;
    mDef->NeedsInput(&need);
    if (!need) {
        return E_IO_EXCEPTION;
//        throw new IOException();
    }
    mDef->SetInput(buffer, offset, byteCount);
    return Deflate();
}

ECode DeflaterOutputStream::Flush()
{
    if (mSyncFlush) {
        Int32 byteCount;
        while (mDef->Deflate(mBuf, 0, mBuf->GetLength(),
            CDeflater::SYNC_FLUSH, &byteCount), byteCount != 0) {
            FAIL_RETURN(mOut->Write(mBuf, 0, byteCount));
        }
    }
    //return mOut->Flush();
    return E_NOT_IMPLEMENTED;
}

ECode DeflaterOutputStream::CheckError(
    /* [out] */ Boolean* hasError)
{
    assert(hasError != NULL);
    *hasError = FALSE;
    return NOERROR;
}

ECode DeflaterOutputStream::constructor(
    /* [in] */ IOutputStream* os,
    /* [in] */ IDeflater* def)
{
    return constructor(os, def, BUF_SIZE, FALSE);
}

ECode DeflaterOutputStream::constructor(
    /* [in] */ IOutputStream* os)
{
    AutoPtr<CDeflater> deflater;
    CDeflater::NewByFriend((CDeflater**)&deflater);
    return constructor(os, (IDeflater*)deflater.Get(), BUF_SIZE, FALSE);
}

ECode DeflaterOutputStream::constructor(
    /* [in] */ IOutputStream* os,
    /* [in] */ IDeflater* def,
    /* [in] */ Int32 bsize)
{
    return constructor(os, def, bsize, FALSE);
}

ECode DeflaterOutputStream::constructor(
    /* [in] */ IOutputStream* os,
    /* [in] */ Boolean syncFlush)
{
    AutoPtr<CDeflater> deflater;
    CDeflater::NewByFriend((CDeflater**)&deflater);
    return constructor(os, (IDeflater*)deflater.Get(), BUF_SIZE, syncFlush);
}

ECode DeflaterOutputStream::constructor(
    /* [in] */ IOutputStream* os,
    /* [in] */ IDeflater* def,
    /* [in] */ Boolean syncFlush)
{
    return constructor(os, def, BUF_SIZE, syncFlush);
}

ECode DeflaterOutputStream::constructor(
    /* [in] */ IOutputStream* os,
    /* [in] */ IDeflater* def,
    /* [in] */ Int32 bsize,
    /* [in] */ Boolean syncFlush)
{
    FilterOutputStream::constructor(os);
    if (os == NULL || def == NULL) {
//        throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }
    if (bsize <= 0) {
//        throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mDef = (CDeflater*)def;
    mSyncFlush = syncFlush;
    mBuf = ArrayOf<Byte>::Alloc(bsize);
    return NOERROR;
}

ECode DeflaterOutputStream::Write(
    /* [in] */ const ArrayOf<Byte> & buffer)
{
    // BEGIN android-note
    // changed array notation to be consistent with the rest of harmony
    // END android-note
    return Write(buffer, 0, buffer.GetLength());
}


} // namespace Zip
} // namespace Utility
} // namespace Elastos
