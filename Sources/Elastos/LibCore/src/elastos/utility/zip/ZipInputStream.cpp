
#include "ZipInputStream.h"
#include "CZipFile.h"
#include "CZipEntry.h"
#include "CCRC32.h"
//#include "CMemory.h"
#include "Math.h"
#include "CPushbackInputStream.h"
#include "CStreams.h"

using Elastos::Core::Math;
using Elastos::IO::EIID_IInputStream;
using Elastos::IO::IPushbackInputStream;
using Elastos::IO::CPushbackInputStream;
// using Libcore::IO::CMemory;
using Elastos::IO::ByteOrder_LITTLE_ENDIAN;
using Libcore::IO::IStreams;
using Libcore::IO::CStreams;

namespace Elastos {
namespace Utility {
namespace Zip {

const Int32 ZipInputStream::ZIPLocalHeaderVersionNeeded;

CAR_INTERFACE_IMPL(ZipInputStream, InflaterInputStream, IZipInputStream)

ZipInputStream::ZipInputStream()
    : mEntriesEnd(FALSE)
    , mHasDD(FALSE)
    , mEntryIn(0)
    , mInRead(0)
    , mLastRead(0)
{
    mHdrBuf = ArrayOf<Byte>::Alloc(IZipConstants::LOCHDR - IZipConstants::LOCVER);
    CCRC32::New((ICRC32**)&mCrc);
    mNameBuf = ArrayOf<Byte>::Alloc(256);
}

ZipInputStream::~ZipInputStream()
{
}

ECode ZipInputStream::Close()
{
    if (!mClosed) {
        FAIL_RETURN(CloseEntry()); // Close the current entry
        FAIL_RETURN(InflaterInputStream::Close());
    }
    return NOERROR;
}

ECode ZipInputStream::CloseEntry()
{
    FAIL_RETURN(CheckClosed());
    if (mCurrentEntry == NULL) {
        return NOERROR;
    }
//    if (mCurrentEntry instanceof java.util.jar.JarEntry) {
//        Attributes temp = ((JarEntry) mCurrentEntry).getAttributes();
//        if (temp != null && temp.containsKey("hidden")) {
//            return;
//        }
//    }

    /*
     * The following code is careful to leave the ZipInputStream in a
     * consistent state, even when close() results in an exception. It does
     * so by:
     *  - pushing bytes back into the source stream
     *  - reading a data descriptor footer from the source stream
     *  - resetting fields that manage the entry being closed
     */

    // Ensure all entry bytes are read
    //try {
    AutoPtr<IStreams> streams;
    CStreams::AcquireSingleton((IStreams**)&streams);
    ECode ec = streams->SkipAll(THIS_PROBE(IInputStream));
    // } catch (Exception e) {
    //     failure = e;
    // }

    Int32 inB, out;
    // if (mCurrentEntry->mCompressionMethod == IZipEntry::DEFLATED) {
    //     mInf->GetTotalIn(&inB);
    //     mInf->GetTotalOut(&out);
    // }
    // else {
    //     inB = mInRead;
    //     out = mInRead;
    // }

    Int32 diff = mEntryIn - inB;
    // Pushback any required bytes
    if (diff != 0) {
        IPushbackInputStream::Probe(mIn)->Unread(mBuf, mLen - diff, diff);
    }

    ECode newec = ReadAndVerifyDataDescriptor(inB, out);
    if (SUCCEEDED(ec)) ec = newec; // otherwise we're already going to throw

    mInf->Reset();
    mLastRead = mInRead = mEntryIn = mLen = 0;
    IChecksum::Probe(mCrc)->Reset();
    mCurrentEntry = NULL;

    return ec;
}

ECode ZipInputStream::ReadAndVerifyDataDescriptor(
    /* [in] */ Int32 inB,
    /* [in] */ Int32 out)
{
    if (mHasDD) {
        AutoPtr<IStreams> streams;
        CStreams::AcquireSingleton((IStreams**)&streams);
        FAIL_RETURN(streams->ReadFully(mIn, 0, IZipConstants::EXTHDR, mHdrBuf));
        AutoPtr<IMemory> memory;
        // CMemory::AcquireSingleton((IMemory**)&memory);
        Int32 sig;
        memory->PeekInt32(mHdrBuf, 0, ByteOrder_LITTLE_ENDIAN, &sig);
        if (sig != IZipConstants::EXTSIG) {
//            throw new ZipException(String.format("unknown format (EXTSIG=%x)", sig));
            return E_ZIP_EXCEPTION;
        }

        Int32 temp;
        // memory->PeekInt32(mHdrBuf, IZipConstants::EXTCRC, ByteOrder_LITTLE_ENDIAN, &temp);
        // mCurrentEntry->mCrc = temp & 0xffffffffll;

        // memory->PeekInt32(mHdrBuf, IZipConstants::EXTSIZ, ByteOrder_LITTLE_ENDIAN, &temp);
        // mCurrentEntry->mCompressedSize = temp & 0xffffffffll;

        // memory->PeekInt32(mHdrBuf, IZipConstants::EXTLEN, ByteOrder_LITTLE_ENDIAN, &temp);
        // mCurrentEntry->mSize = temp & 0xffffffffll;
    }

//     Int64 checksum;
//     IChecksum::Probe(mCrc)->GetValue(&checksum);
//     if (mCurrentEntry->mCrc != checksum) {
// //        throw new ZipException("CRC mismatch");
//         return E_ZIP_EXCEPTION;
//     }
//     if (mCurrentEntry->mCompressedSize != inB || mCurrentEntry->mSize != out) {
// //        throw new ZipException("Size mismatch");
//         return E_ZIP_EXCEPTION;
//     }
    return NOERROR;
}

ECode ZipInputStream::GetNextEntry(
    /* [out] */ IZipEntry** entry)
{
    VALIDATE_NOT_NULL(entry);
    *entry = NULL;

    FAIL_RETURN(CloseEntry());
    if (mEntriesEnd) {
        return NOERROR;
    }

    AutoPtr<IMemory> memory;
    // CMemory::AcquireSingleton((IMemory**)&memory);
    // Read the signature to see whether there's another local file header.
    AutoPtr<IStreams> streams;
    CStreams::AcquireSingleton((IStreams**)&streams);
    FAIL_RETURN(streams->ReadFully(mIn, 0, 4, mHdrBuf));

    Int32 hdr;
    memory->PeekInt32(mHdrBuf, 0, ByteOrder_LITTLE_ENDIAN, &hdr);
    if (hdr == IZipConstants::CENSIG) {
        mEntriesEnd = TRUE;
        return NOERROR;
    }
    else if (hdr != IZipConstants::LOCSIG) {
        return NOERROR;
    }

    // Read the local file header.
    FAIL_RETURN(streams->ReadFully(mIn, 0, (IZipConstants::LOCHDR - IZipConstants::LOCVER), mHdrBuf));
    Int32 version = PeekShort(memory, 0) & 0xff;
    if (version > ZIPLocalHeaderVersionNeeded) {
        return E_ZIP_EXCEPTION;
//        throw new ZipException("Cannot read local header version " + version);
    }

    Int32 flags = PeekShort(memory, IZipConstants::LOCFLG - IZipConstants::LOCVER);
    mHasDD = ((flags & CZipFile::GPBF_DATA_DESCRIPTOR_FLAG) != 0);
    Int32 ceLastModifiedTime = PeekShort(memory, IZipConstants::LOCTIM - IZipConstants::LOCVER);
    Int32 ceLastModifiedDate = PeekShort(memory, IZipConstants::LOCTIM - IZipConstants::LOCVER + 2);
    Int32 ceCompressionMethod = PeekShort(memory, IZipConstants::LOCHOW - IZipConstants::LOCVER);

    Int64 ceCrc = 0, ceCompressedSize = 0, ceSize = -1;
    if (!mHasDD) {
        Int32 temp;
        memory->PeekInt32(mHdrBuf, IZipConstants::LOCCRC - IZipConstants::LOCVER, ByteOrder_LITTLE_ENDIAN, &temp);
        ceCrc = temp & 0xffffffffL;

        memory->PeekInt32(mHdrBuf, IZipConstants::LOCSIZ - IZipConstants::LOCVER, ByteOrder_LITTLE_ENDIAN, &temp);
        ceCompressedSize = temp & 0xffffffffL;

        memory->PeekInt32(mHdrBuf, IZipConstants::LOCLEN - IZipConstants::LOCVER, ByteOrder_LITTLE_ENDIAN, &temp);
        ceSize = temp & 0xffffffffL;
    }

    Int32 nameLength = PeekShort(memory, IZipConstants::LOCNAM - IZipConstants::LOCVER);
    if (nameLength == 0) {
        return E_ZIP_EXCEPTION;
//        throw new ZipException("Entry is not named");
    }
    Int32 extraLength = PeekShort(memory, IZipConstants::LOCEXT - IZipConstants::LOCVER);
    if (nameLength > mNameBuf->GetLength()) {
        mNameBuf = ArrayOf<Byte>::Alloc(nameLength);
        // The bytes are modified UTF-8, so the number of chars will always be less than or
        // equal to the number of bytes. It's fine if this buffer is too long.
        //charBuf = new char[nameLength];
    }
    FAIL_RETURN(streams->ReadFully(mIn, 0, nameLength, mNameBuf));
    //mCurrentEntry = createZipEntry(ModifiedUtf8.decode(nameBuf, charBuf, 0, nameLength));
    String zipEntryName((const char *)mNameBuf->GetPayload(), nameLength);
    mCurrentEntry = CreateZipEntry(zipEntryName);
    // mCurrentEntry->mTime = ceLastModifiedTime;
    // mCurrentEntry->mModDate = ceLastModifiedDate;
    mCurrentEntry->SetMethod(ceCompressionMethod);
    if (ceSize != -1) {
        mCurrentEntry->SetCrc(ceCrc);
        mCurrentEntry->SetSize(ceSize);
        mCurrentEntry->SetCompressedSize(ceCompressedSize);
    }

    if (extraLength > 0) {
        AutoPtr<ArrayOf<Byte> > extraData = ArrayOf<Byte>::Alloc(extraLength);
        FAIL_RETURN(streams->ReadFully(mIn, 0, extraLength, extraData));
        mCurrentEntry->SetExtra(extraData);
    }

    *entry = (IZipEntry*)mCurrentEntry.Get();
    REFCOUNT_ADD(*entry);
    return NOERROR;
}

Int32 ZipInputStream::PeekShort(
    /* [in] */ IMemory* memory,
    /* [in] */ Int32 offset)
{
    Int16 value;
    memory->PeekInt16(mHdrBuf, offset, ByteOrder_LITTLE_ENDIAN, &value);
    return (value & 0xffff);
}

ECode ZipInputStream::Read(
        /* [out] */ ArrayOf<Byte>* buffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 byteCount,
        /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number)
    *number = -1;

    FAIL_RETURN(CheckClosed());

    if ((offset | byteCount) < 0 || offset > buffer->GetLength() ||
            buffer->GetLength() - offset < byteCount) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    Boolean finished;
    mInf->Finished(&finished);
    if (finished || mCurrentEntry == NULL) {
        *number = -1;
        return NOERROR;
    }

    // if (mCurrentEntry->mCompressionMethod == IZipEntry::STORED) {
    //     Int32 csize = (Int32)mCurrentEntry->mSize;
    //     if (mInRead >= csize) {
    //         *number = -1;
    //         return NOERROR;
    //     }
    //     if (mLastRead >= mLen) {
    //         mLastRead = 0;
    //         FAIL_RETURN(mIn->Read(mBuf, &mLen));
    //         if (mLen == -1) {
    //             mEof = TRUE;
    //             *number = -1;
    //             return NOERROR;
    //         }
    //         mEntryIn += mLen;
    //     }
    //     Int32 toRead = byteCount > (mLen - mLastRead) ? mLen - mLastRead : byteCount;
    //     if ((csize - mInRead) < toRead) {
    //         toRead = csize - mInRead;
    //     }
    //     memcpy(buffer->GetPayload() + offset, mBuf->GetPayload() + mLastRead, toRead);
    //     mLastRead += toRead;
    //     mInRead += toRead;
    //     IChecksum::Probe(mCrc)->Update(buffer, offset, toRead);
    //     *number = toRead;
    //     return NOERROR;
    // }
    Boolean value;
    mInf->NeedsInput(&value);
    if (value) {
        FAIL_RETURN(Fill());
        if (mLen > 0) {
            mEntryIn += mLen;
        }
    }
    Int32 read;
    if (FAILED(mInf->Inflate(buffer, offset, byteCount, &read))) {
        return E_ZIP_EXCEPTION;
    }
    mInf->Finished(&value);
    if (read == 0 && value) {
        *number = -1;
        return NOERROR;
    }
    IChecksum::Probe(mCrc)->Update(buffer, offset, read);
    *number = read;
    return NOERROR;
}

ECode ZipInputStream::Available(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number)
    FAIL_RETURN(CheckClosed());
    // The InflaterInputStream contract says we must only return 0 or 1.
    // *number = (mCurrentEntry == NULL || mInRead < mCurrentEntry->mSize) ? 1 : 0;
    return NOERROR;
}

AutoPtr<IZipEntry> ZipInputStream::CreateZipEntry(
    /* [in] */ const String& name)
{
    AutoPtr<IZipEntry> entry;
    CZipEntry::New(name, (IZipEntry**)&entry);
    return entry;
}

ECode ZipInputStream::CheckClosed()
{
    if (mClosed) {
//        throw new IOException("Stream is closed");
        return E_IO_EXCEPTION;
    }
    return NOERROR;
}

ECode ZipInputStream::constructor(
    /* [in] */ IInputStream* stream)
{
    if (stream == NULL) {
//        throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }

    AutoPtr<IInputStream> is;
    FAIL_RETURN(CPushbackInputStream::New(stream, BUF_SIZE, (IInputStream**)&is));
    AutoPtr<IInflater> inflater;
    CInflater::New(TRUE, (IInflater**)&inflater);
    return InflaterInputStream::constructor(is.Get(), inflater.Get());
}

} // namespace Zip
} // namespace Utility
} // namespace Elastos
