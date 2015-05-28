
#include "ZipOutputStream.h"
#include "CZipFile.h"
#include <elastos/Algorithm.h>
#include "CSystem.h"

using Elastos::Core::ISystem;
using Elastos::IO::CByteArrayOutputStream;

namespace Elastos {
namespace Utility {
namespace Zip {


const Int32 ZipOutputStream::ZIPLocalHeaderVersionNeeded;

ZipOutputStream::ZipOutputStream()
    : mCompressMethod(IDeflater::DEFLATED)
    , mCompressLevel(IDeflater::DEFAULT_COMPRESSION)
    , mOffset(0)
    , mCurOffset(0)
    , mNameLength(0)
{
    CByteArrayOutputStream::New((IByteArrayOutputStream**)&mCDir);
    CCRC32::NewByFriend((CCRC32**)&mCrc);
}

ZipOutputStream::~ZipOutputStream()
{
    mEntries.Clear();
}

/**
 * Constructs a new {@code ZipOutputStream} with the specified output
 * stream.
 *
 * @param p1
 *            the {@code OutputStream} to write the data to.
 */
ECode ZipOutputStream::Init(
    /* [in] */ IOutputStream* p1)
{
    AutoPtr<CDeflater> deflater;
    CDeflater::NewByFriend(IDeflater::DEFAULT_COMPRESSION, TRUE, (CDeflater**)&deflater);
    return DeflaterOutputStream::Init(p1, (IDeflater*)deflater.Get());
}

ECode ZipOutputStream::Close()
{
    // don't call super.close() because that calls finish() conditionally
    if (mOut != NULL) {
        FAIL_RETURN(Finish());
        FAIL_RETURN(mDef->End());
        FAIL_RETURN(mOut->Close());
        mOut = NULL;
    }
    return NOERROR;
}

ECode ZipOutputStream::CloseEntry()
{
    FAIL_RETURN(CheckClosed());
    if (mCurrentEntry == NULL) {
        return NOERROR;
    }

    Int32 method;
    mCurrentEntry->GetMethod(&method);
    if (method == IZipOutputStream::DEFLATED) {
        FAIL_RETURN(DeflaterOutputStream::Finish());
    }

    // Verify values for STORED types
    if (method == IZipOutputStream::STORED) {
        Int64 value;
        mCrc->GetValue(&value);
        if (value != mCurrentEntry->mCrc) {
//            throw new ZipException("CRC mismatch");
            return E_ZIP_EXCEPTION;
        }
        if (mCurrentEntry->mSize != mCrc->mTbytes) {
//           throw new ZipException("Size mismatch");
            return E_ZIP_EXCEPTION;
        }
    }
    mCurOffset = IZipConstants::LOCHDR;

    // Write the DataDescriptor
    if (method != IZipOutputStream::STORED) {
        mCurOffset += IZipConstants::EXTHDR;
        WriteInt64(mOut, IZipConstants::EXTSIG);
        mCrc->GetValue(&mCurrentEntry->mCrc);
        WriteInt64(mOut, mCurrentEntry->mCrc);
        mDef->GetTotalOut((Int32*)&mCurrentEntry->mCompressedSize);
        WriteInt64(mOut, mCurrentEntry->mCompressedSize);
        mDef->GetTotalIn((Int32*)&mCurrentEntry->mSize);
        WriteInt64(mOut, mCurrentEntry->mSize);
    }
    // Update the CentralDirectory
    // http://www.pkware.com/documents/casestudies/APPNOTE.TXT
    Int32 flags = method == IZipOutputStream::STORED ? 0 : CZipFile::GPBF_DATA_DESCRIPTOR_FLAG;
    // Since gingerbread, we always set the UTF-8 flag on individual files.
    // Some tools insist that the central directory also have the UTF-8 flag.
    // http://code.google.com/p/android/issues/detail?id=20214
    flags |= CZipFile::GPBF_UTF8_FLAG;
    WriteInt64(mCDir, IZipConstants::CENSIG);
    WriteInt16(mCDir, ZIPLocalHeaderVersionNeeded); // Version created
    WriteInt16(mCDir, ZIPLocalHeaderVersionNeeded); // Version to extract
    WriteInt16(mCDir, flags);
    WriteInt16(mCDir, method);
    WriteInt16(mCDir, mCurrentEntry->mTime);
    WriteInt16(mCDir, mCurrentEntry->mModDate);
    Int64 checksum;
    mCrc->GetValue(&checksum);
    WriteInt64(mCDir, checksum);
    if (method == IZipOutputStream::DEFLATED) {
        Int32 value;
        mDef->GetTotalOut(&value);
        mCurOffset += WriteInt64(mCDir, value);
        mDef->GetTotalIn(&value);
        WriteInt64(mCDir, value);
    }
    else {
        mCurOffset += WriteInt64(mCDir, mCrc->mTbytes);
        WriteInt64(mCDir, mCrc->mTbytes);
    }
    mCurOffset += WriteInt16(mCDir, mNameLength);
    if (mCurrentEntry->mExtra != NULL) {
        mCurOffset += WriteInt16(mCDir, mCurrentEntry->mExtra->GetLength());
    }
    else {
        WriteInt16(mCDir, 0);
    }
    String c;
    mCurrentEntry->GetComment(&c);
    if (!c.IsNull()) {
        WriteInt16(mCDir, c.GetLength());
    }
    else {
        WriteInt16(mCDir, 0);
    }
    WriteInt16(mCDir, 0); // Disk Start
    WriteInt16(mCDir, 0); // Internal File Attributes
    WriteInt64(mCDir, 0); // External File Attributes
    WriteInt64(mCDir, mOffset);
    mCDir->WriteBytes(*mNameBytes);
    mNameBytes = NULL;
    if (mCurrentEntry->mExtra != NULL) {
        mCDir->WriteBytes(*mCurrentEntry->mExtra);
    }
    mOffset +=mCurOffset;
    if (!c.IsNull()) {
        mCDir->WriteBytes(ArrayOf<Byte>(
            reinterpret_cast<Byte*>(const_cast<char*>((const char*)c)), c.GetByteLength()));
    }
    mCurrentEntry = NULL;
    mCrc->Reset();
    mDef->Reset();
    mDone = FALSE;
    return NOERROR;
}

ECode ZipOutputStream::Finish()
{
    // TODO: is there a bug here? why not checkClosed?
    if (mOut == NULL) {
//        throw new IOException("Stream is closed");
        return E_IO_EXCEPTION;
    }
    if (mCDir == NULL) {
        return NOERROR;
    }
    if (mEntries.IsEmpty()) {
//        throw new ZipException("No entries");
        return E_ZIP_EXCEPTION;
    }
    if (mCurrentEntry != NULL) {
        FAIL_RETURN(CloseEntry());
    }
    Int32 cdirSize;
    mCDir->GetSize(&cdirSize);
    // Write Central Dir End
    WriteInt64(mCDir, IZipConstants::ENDSIG);
    WriteInt16(mCDir, 0); // Disk Number
    WriteInt16(mCDir, 0); // Start Disk
    WriteInt16(mCDir, mEntries.GetSize()); // Number of entries
    WriteInt16(mCDir, mEntries.GetSize()); // Number of entries
    WriteInt64(mCDir, cdirSize); // Size of central dir
    WriteInt64(mCDir, mOffset); // Offset of central dir
    if (!mComment.IsNull()) {
        WriteInt16(mCDir, mComment.GetLength());
        mCDir->WriteBytes(ArrayOf<Byte>(
                reinterpret_cast<Byte*>(const_cast<char*>((const char*)mComment)),
                mComment.GetByteLength()));
    }
    else {
        WriteInt16(mCDir, 0);
    }
    // Write the central dir
    AutoPtr<ArrayOf<Byte> > bytes;
    mCDir->ToByteArray((ArrayOf<Byte> **)&bytes);
    mOut->WriteBytes(*bytes);
    mCDir = NULL;
    return NOERROR;
}

ECode ZipOutputStream::PutNextEntry(
    /* [in] */ IZipEntry* _ze)
{
    CZipEntry* ze = (CZipEntry*)_ze;
    if (mCurrentEntry != NULL) {
        FAIL_RETURN(CloseEntry());
    }
    Int32 method;
    ze->GetMethod(&method);
    if (method == IZipOutputStream::STORED ||
        (mCompressMethod == IZipOutputStream::STORED && method == -1)) {
        if (ze->mCrc == -1) {
//            throw new ZipException("CRC mismatch");
            return E_ZIP_EXCEPTION;
        }
        if (ze->mSize == -1 && ze->mCompressedSize == -1) {
//            throw new ZipException("Size mismatch");
            return E_ZIP_EXCEPTION;
        }
        if (ze->mSize != ze->mCompressedSize && ze->mCompressedSize != -1 && ze->mSize != -1) {
//            throw new ZipException("Size mismatch");
            return E_ZIP_EXCEPTION;
        }
    }
    FAIL_RETURN(CheckClosed());
    Vector<String>::Iterator it = Find(mEntries.Begin(), mEntries.End(), ze->mName);
    if (it != mEntries.End()) {
//        throw new ZipException("Entry already exists: " + ze.name);
        return E_ZIP_EXCEPTION;
    }
    if (mEntries.GetSize() == 64*1024-1) {
        // TODO: support Zip64.
        return E_ZIP_EXCEPTION;
//        throw new ZipException("Too many entries for the zip file format's 16-bit entry count");
    }
    mNameLength = GetUtf8Count(ze->mName);
    if (mNameLength > 0xffff) {
//        throw new IllegalArgumentException("Name too long: " + nameLength + " UTF-8 bytes");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    mDef->SetLevel(mCompressLevel);
    mCurrentEntry = ze;
    mEntries.PushBack(mCurrentEntry->mName);
    mCurrentEntry->GetMethod(&method);
    if (method == -1) {
        mCurrentEntry->SetMethod(mCompressMethod);
    }
    // Local file header.
    // http://www.pkware.com/documents/casestudies/APPNOTE.TXT
    Int32 flags = method == IZipOutputStream::STORED ? 0 : CZipFile::GPBF_DATA_DESCRIPTOR_FLAG;
    // Java always outputs UTF-8 filenames. (Before Java 7, the RI didn't set this flag and used
    // modified UTF-8. From Java 7, it sets this flag and uses normal UTF-8.)
    flags |= CZipFile::GPBF_UTF8_FLAG;
    WriteInt64(mOut, IZipConstants::LOCSIG); // Entry header
    WriteInt16(mOut, ZIPLocalHeaderVersionNeeded); // Extraction version
    WriteInt16(mOut, flags);
    WriteInt16(mOut, method);
    Int64 time;
    mCurrentEntry->GetTime(&time);
    if (time == -1) {
        AutoPtr<ISystem> system;
        Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);
        Int64 millis;
        system->GetCurrentTimeMillis(&millis);
        mCurrentEntry->SetTime(millis);
    }
    WriteInt16(mOut, mCurrentEntry->mTime);
    WriteInt16(mOut, mCurrentEntry->mModDate);

    if (method == IZipOutputStream::STORED) {
        if (mCurrentEntry->mSize == -1) {
            mCurrentEntry->mSize = mCurrentEntry->mCompressedSize;
        }
        else if (mCurrentEntry->mCompressedSize == -1) {
            mCurrentEntry->mCompressedSize = mCurrentEntry->mSize;
        }
        WriteInt64(mOut, mCurrentEntry->mCrc);
        WriteInt64(mOut, mCurrentEntry->mSize);
        WriteInt64(mOut, mCurrentEntry->mSize);
    }
    else {
        WriteInt64(mOut, 0);
        WriteInt64(mOut, 0);
        WriteInt64(mOut, 0);
    }
    WriteInt16(mOut, mNameLength);
    if (mCurrentEntry->mExtra != NULL) {
        WriteInt16(mOut, mCurrentEntry->mExtra->GetLength());
    }
    else {
        WriteInt16(mOut, 0);
    }
    mNameBytes = ToUTF8Bytes(mCurrentEntry->mName, mNameLength);
    mOut->WriteBytes(*mNameBytes);
    if (mCurrentEntry->mExtra != NULL) {
        mOut->WriteBytes(*mCurrentEntry->mExtra);
    }
    return NOERROR;
}

ECode ZipOutputStream::SetComment(
    /* [in] */ const String& comment)
{
    if (!comment.IsNull() && comment.GetLength() > 0xFFFF) {
    //        throw new IllegalArgumentException("Comment too long: " + comment.length() + " characters");
         return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    mComment = comment;
    return NOERROR;
}

ECode ZipOutputStream::SetLevel(
    /* [in] */ Int32 level)
{
    if (level < IDeflater::DEFAULT_COMPRESSION
            || level > IDeflater::BEST_COMPRESSION) {
//        throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mCompressLevel = level;
    return NOERROR;
}

ECode ZipOutputStream::SetMethod(
    /* [in] */ Int32 method)
{
    if (method != IZipOutputStream::STORED && method != IZipOutputStream::DEFLATED) {
//        throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mCompressMethod = method;
    return NOERROR;
}

Int64 ZipOutputStream::WriteInt64(
    /* [in] */ IOutputStream* os,
    /* [in] */ Int64 i)
{
    // Write out the long value as an unsigned int
    os->Write((Int32)(i & 0xFF));
    os->Write((Int32)(i >> 8) & 0xFF);
    os->Write((Int32)(i >> 16) & 0xFF);
    os->Write((Int32)(i >> 24) & 0xFF);
    return i;
}

Int32 ZipOutputStream::WriteInt16(
    /* [in] */ IOutputStream* os,
    /* [in] */ Int32 i)
{
    os->Write(i & 0xFF);
    os->Write((i >> 8) & 0xFF);
    return i;

}

ECode ZipOutputStream::WriteBytes(
    /* [in] */ const ArrayOf<Byte>& buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 byteCount)
{
//    Arrays.checkOffsetAndCount(buffer.length, offset, byteCount);
    if ((offset | byteCount) < 0 || offset > buffer.GetLength() ||
            buffer.GetLength() - offset < byteCount) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
//        throw new ArrayIndexOutOfBoundsException(arrayLength, offset, count);
    }

    if (mCurrentEntry == NULL) {
//        throw new ZipException("No active entry");
        return E_ZIP_EXCEPTION;
    }

    Int32 method;
    mCurrentEntry->GetMethod(&method);
    if (method == IZipOutputStream::STORED) {
        FAIL_RETURN(mOut->WriteBytes(buffer, offset, byteCount));
    }
    else {
        FAIL_RETURN(DeflaterOutputStream::WriteBytes(buffer, offset, byteCount));
    }
    return mCrc->Update(buffer, offset, byteCount);
}

Int32 ZipOutputStream::GetUtf8Count(
    /* [in] */ const String& value)
{
    return value.GetLength();
}

AutoPtr<ArrayOf<Byte> > ZipOutputStream::ToUTF8Bytes(
    /* [in] */ const String& value,
    /* [in] */ Int32 length)
{
    Int32 byteLength = value.ToByteIndex(length);
    assert(value.GetByteLength() >= byteLength);
    AutoPtr<ArrayOf<Byte> > result = ArrayOf<Byte>::Alloc(length);
    memcpy(result->GetPayload(), (const char*)value, byteLength);
    return result;
}

ECode ZipOutputStream::CheckClosed()
{
    if (mCDir == NULL) {
//        throw new IOException("Stream is closed");
        return E_IO_EXCEPTION;
    }
    return NOERROR;
}


} // namespace Zip
} // namespace Utility
} // namespace Elastos
