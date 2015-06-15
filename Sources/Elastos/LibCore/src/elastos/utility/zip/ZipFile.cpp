
#include "ZipFile.h"
#include "CInflater.h"
#include "Math.h"
#include "CDataInputStream.h"
#include "CBufferedInputStream.h"
#include "CRandomAccessFile.h"
#include "CFile.h"
#include "CZipEntry.h"
#include "StringBuilder.h"
#include "CStreams.h"
#include "IoUtils.h"
#include "CLinkedHashMap.h"
#include "CStringWrapper.h"

using Elastos::Core::Math;
using Elastos::Core::ICharSequence;
using Elastos::Core::CStringWrapper;
using Elastos::IO::IDataInputStream;
using Elastos::IO::CDataInputStream;
using Elastos::IO::IDataInput;
using Elastos::IO::ByteOrder_LITTLE_ENDIAN;
using Elastos::IO::IBufferedInputStream;
using Elastos::IO::CBufferedInputStream;
using Elastos::IO::CRandomAccessFile;
using Elastos::IO::CFile;
using Elastos::IO::EIID_IDataInput;
using Elastos::IO::EIID_IInputStream;
using Elastos::IO::EIID_IFilterInputStream;
using Elastos::IO::EIID_ICloseable;
using Elastos::Core::StringBuilder;
using Libcore::IO::IoUtils;
using Libcore::IO::IStreams;
using Libcore::IO::CStreams;
using Libcore::IO::IBufferIterator;
using Libcore::IO::IHeapBufferIterator;

namespace Elastos {
namespace Utility {
namespace Zip {

extern "C" const InterfaceID EIID_ZipFileRAFStream =
    { 0x83f2304, 0xfc73, 0x4727, { 0x86, 0x6a, 0x81, 0xd5, 0xeb, 0x82, 0xf3, 0x43 } };

const Int32 ZipFile::GPBF_DATA_DESCRIPTOR_FLAG = 1 << 3;
const Int32 ZipFile::GPBF_UTF8_FLAG = 1 << 11;
const Int32 ZipFile::GPBF_UNSUPPORTED_MASK = GPBF_DATA_DESCRIPTOR_FLAG;

//=====================================================
//ZipFile::RAFStream
//=====================================================
ZipFile::RAFStream::RAFStream(
    /* [in] */ IRandomAccessFile* raf,
    /* [in] */ Int64 initialOffset)
    : mSharedRaf(raf)
    , mOffset(initialOffset)
    , mEndOffset(0)
{
    raf->GetLength(&mEndOffset);
}

ZipFile::RAFStream::RAFStream(
    /* [in] */ IRandomAccessFile* raf,
    /* [in] */ Int64 initialOffset,
    /* [in] */ Int64 endOffset)
    : mSharedRaf(raf)
    , mOffset(initialOffset)
    , mEndOffset(endOffset)
{
}

IInterface* ZipFile::RAFStream::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_ZipFileRAFStream) {
        return reinterpret_cast<IInterface*>(this);
    }

    return InputStream::Probe(riid);
}

ECode ZipFile::RAFStream::Available(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    *number = (mOffset < mEndOffset ? 1 : 0);
    return NOERROR;
}

ECode ZipFile::RAFStream::Read(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    AutoPtr<IStreams> streams;
    CStreams::AcquireSingleton((IStreams**)&streams);
    return streams->ReadSingleByte(THIS_PROBE(IInputStream), value);
}

ECode ZipFile::RAFStream::Read(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 byteOffset,
    /* [in] */ Int32 byteCount,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    *number = -1;
    VALIDATE_NOT_NULL(buffer);

    Object::Autolock locK(this);

    Int64 length = mEndOffset - mOffset;
    if (byteCount > length) {
        byteCount = (Int32)length;
    }

    mSharedRaf->Seek(mOffset);
    FAIL_RETURN(mSharedRaf->Read(buffer, mOffset, length, number));
    if (*number > 0) {
        mOffset += *number;
    }
    else *number = -1;

    return NOERROR;
}

ECode ZipFile::RAFStream::Skip(
    /* [in] */ Int64 byteCount,
    /* [out] */ Int64* number)
{
    VALIDATE_NOT_NULL(number);
    if (byteCount > mEndOffset - mOffset) {
        byteCount = mEndOffset - mOffset;
    }
    mOffset += byteCount;
    *number = byteCount;
    return NOERROR;
}

ECode ZipFile::RAFStream::Read(
    /* [in] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    *number = 0;
    VALIDATE_NOT_NULL(buffer);
    return Read(buffer, 0, buffer->GetLength(), number);
}

ECode ZipFile::RAFStream::Fill(
    /* [in] */ IInflater* inflater,
    /* [in] */ Int32 nativeEndBufSize,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)
    *result = 0;

    CInflater* inf = (CInflater*)inflater;
    using Elastos::Core::Math;

    synchronized (inf) {
        AutoPtr<IFileDescriptor> fd;
        mSharedRaf->GetFD((IFileDescriptor**)&fd);
        Int32 len = Math::Min((Int32) (mEndOffset - mOffset), nativeEndBufSize);
        Int32 cnt = inf->SetFileInput(fd, mOffset, nativeEndBufSize);
        // setFileInput read from the file, so we need to get the OS and RAFStream back
        // in sync...
        Int64 val;
        Skip(cnt, &val);
        *result = len;
    }
    return NOERROR;
}

//=====================================================
//ZipFile::ZipInflaterInputStream
//=====================================================

ZipFile::ZipInflaterInputStream::ZipInflaterInputStream(
    /* [in] */ IInputStream* is,
    /* [in] */ IInflater* inf,
    /* [in] */ Int32 bsize,
    /* [in] */ IZipEntry* entry)
    : mEntry(entry)
    , mBytesRead(0)
{
    ASSERT_SUCCEEDED(InflaterInputStream::constructor(is, inf, bsize));
}

//@Override
ECode ZipFile::ZipInflaterInputStream::Read(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 byteOffset,
    /* [in] */ Int32 byteCount,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number)
    *number = -1;
    Int32 i;
    FAIL_RETURN(InflaterInputStream::Read(buffer, byteOffset, byteCount, &i));
    if (i == -1) {
        Int64 size;
        mEntry->GetSize(&size);
        if (size != mBytesRead) {
            // throw new IOException("Size mismatch on inflated file: " + bytesRead + " vs "
            //                             + entry.size);
            return E_IO_EXCEPTION;
        }
    }
    else {
        mBytesRead += i;
    }

    *number = i;
    return NOERROR;
}

//@Override
ECode ZipFile::ZipInflaterInputStream::Available(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    if (mClosed) {
        // Our superclass will throw an exception, but there's a jtreg test that
        // explicitly checks that the InputStream returned from ZipFile.getInputStream
        // returns 0 even when closed.
        *number = 0;
        return NOERROR;
    }

    FAIL_RETURN(InflaterInputStream::Available(number));
    if (*number != 0) {
        Int64 size;
        mEntry->GetSize(&size);
        *number = size - mBytesRead;
    }
    return NOERROR;
}

//====================================================================
// ZipFile::Enumeration::
//====================================================================
CAR_INTERFACE_IMPL(ZipFile::Enumeration, Object, IEnumeration)

ZipFile::Enumeration::Enumeration(
    /* [in] */ IIterator* it)
{
    mIt = it;
}

ECode ZipFile::Enumeration::HasMoreElements(
    /* [out] */ Boolean * value)
{
    VALIDATE_NOT_NULL(value);
    // FAIL_RETURN(mHost->CheckNotClosed())
    return mIt->HasNext(value);
}

ECode ZipFile::Enumeration::GetNextElement(
    /* [out] */ IInterface ** inter)
{
    VALIDATE_NOT_NULL(inter);
    // FAIL_RETURN(mHost->CheckNotClosed())
    return mIt->GetNext(inter);
}

//====================================================================
// ZipFile::
//====================================================================

CAR_INTERFACE_IMPL_2(ZipFile, Object, IZipFile, ICloseable)

ZipFile::ZipFile()
{
    CLinkedHashMap::New((IMap**)&mEntries);
}

ZipFile::~ZipFile()
{
    if (mGuard != NULL) {
        mGuard->WarnIfOpen();
    }
    mEntries->Clear();
}

ECode ZipFile::constructor(
    /* [in] */ const String& name)
{
    AutoPtr<IFile> file;
    FAIL_RETURN(CFile::New(name, (IFile**)&file));
    return constructor(file, IZipFile::OPEN_READ);
}

ECode ZipFile::constructor(
    /* [in] */ IFile* file)
{
    return constructor(file, IZipFile::OPEN_READ);
}

ECode ZipFile::constructor(
    /* [in] */ IFile* file,
    /* [in] */ Int32 mode)
{
    VALIDATE_NOT_NULL(file);
    file->GetPath(&mFileName);
    if (mode != IZipFile::OPEN_READ
        && mode != (IZipFile::OPEN_READ | IZipFile::OPEN_DELETE)) {
//        throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if ((mode & IZipFile::OPEN_DELETE) != 0) {
        mFileToDeleteOnClose = file;
        mFileToDeleteOnClose->DeleteOnExit();
    }
    else {
        mFileToDeleteOnClose = NULL;
    }

    FAIL_RETURN(CRandomAccessFile::New(file, String("r"), (IRandomAccessFile**)&mRaf));

    // Make sure to close the RandomAccessFile if reading the central directory fails.
    Boolean mustCloseFile = TRUE;
    // try {
    ECode ec = ReadCentralDir();
    FAIL_GOTO(ec, _EXIT_)
        // Read succeeded so do not close the underlying RandomAccessFile.
        mustCloseFile = FALSE;
    // } finally {
_EXIT_:
        if (mustCloseFile) {
            IoUtils::CloseQuietly(ICloseable::Probe(mRaf));
        }
    // }

    return mGuard->Open(String("close"));
}


ECode ZipFile::Close()
{
    mGuard->Close();
    AutoPtr<IRandomAccessFile> raf = mRaf;

    if (raf != NULL) { // Only close initialized instances
        {
            Object::Autolock locK(this);
            mRaf = NULL;
            ICloseable::Probe(raf)->Close();
        }
        if (mFileToDeleteOnClose != NULL) {
            Boolean result;
            mFileToDeleteOnClose->Delete(&result);
            mFileToDeleteOnClose = NULL;
        }
    }
    return NOERROR;
}

ECode ZipFile::CheckNotClosed()
{
   if (mRaf == NULL) {
       // throw new IllegalStateException("Zip file closed");
       return E_ILLEGAL_STATE_EXCEPTION;
   }
   return NOERROR;
}

ECode ZipFile::GetEntries(
    /* [out] */ IEnumeration** entries)
{
    VALIDATE_NOT_NULL(entries);
    *entries = NULL;
    FAIL_RETURN(CheckNotClosed());

    AutoPtr<ICollection> vals;
    mEntries->GetValues((ICollection**)&vals);
    AutoPtr<IIterator> iterator;
    IIterable::Probe(vals)->GetIterator((IIterator**)&iterator);
    AutoPtr<Enumeration> res = new Enumeration(iterator);
    *entries = res.Get();
    REFCOUNT_ADD(*entries);
    return NOERROR;
}

ECode ZipFile::GetEntry(
    /* [in] */ const String& entryName,
    /* [out] */ IZipEntry** entry)
{
    VALIDATE_NOT_NULL(entry)
    *entry = NULL;
    FAIL_RETURN(CheckNotClosed());

    if (entryName.IsNull()) {
         return E_NULL_POINTER_EXCEPTION;
//         throw new NullPointerException("entryName == null");
    }

    AutoPtr<ICharSequence> csq;
    CStringWrapper::New(entryName, (ICharSequence**)&csq);
    AutoPtr<IInterface> obj;
    mEntries->Get(TO_IINTERFACE(csq), (IInterface**)&obj);
    AutoPtr<IZipEntry> ze = IZipEntry::Probe(obj);
    if (ze == NULL) {
        String name(entryName);
        name += "/";
        csq = NULL;
        CStringWrapper::New(name, (ICharSequence**)&csq);
        obj = NULL;
        mEntries->Get(TO_IINTERFACE(csq), (IInterface**)&obj);
        ze = IZipEntry::Probe(obj);
    }

    *entry = ze;
    REFCOUNT_ADD(*entry)
    return NOERROR;
}

ECode ZipFile::ZipFile::GetComment(
    /* [out] */ String* comment)
{
    VALIDATE_NOT_NULL(comment)
    *comment = mComment;
    return NOERROR;
}

ECode ZipFile::GetInputStream(
    /* [in] */ IZipEntry* entry,
    /* [out] */ IInputStream** is)
{
    VALIDATE_NOT_NULL(is);
    *is = NULL;
    VALIDATE_NOT_NULL(entry);

    // Make sure this ZipEntry is in this Zip file.  We run it through the name lookup.
    String name;
    entry->GetName(&name);
    AutoPtr<IZipEntry> ze;
    GetEntry(name, (IZipEntry**)&ze);
    if (ze == NULL) {
        *is = NULL;
        return NOERROR;
    }

    // Create an InputStream at the right part of the file.
    AutoPtr<IRandomAccessFile> raf = mRaf;
    Object::Autolock locK(this);
    // We don't know the entry data's start position. All we have is the
    // position of the entry's local header. At position 28 we find the
    // length of the extra data. In some cases this length differs from
    // the one coming in the central header.
    AutoPtr<RAFStream> rafstrm = new RAFStream(raf, ((CZipEntry*)ze.Get())->mLocalHeaderRelOffset + 28);
    AutoPtr<IDataInputStream> dis;
    CDataInputStream::New((IInputStream*)rafstrm, (IDataInputStream**)&dis);
    AutoPtr<IDataInput> di = (IDataInput*)dis->Probe(EIID_IDataInput);
    //Int32 localExtraLenOrWhatever = Short.reverseBytes(is.readShort());
    Int16 value;
    di->ReadInt16(&value);
    Int32 localExtraLenOrWhatever = (Int16)((value << 8) | ((((UInt16)value) >> 8) & 0xFF));
    ICloseable::Probe(dis)->Close();

    // Skip the name and this "extra" data or whatever it is:
    Int64 number;
    rafstrm->Skip(((CZipEntry*)ze.Get())->mNameLength + localExtraLenOrWhatever, &number);
    if (((CZipEntry*)ze.Get())->mCompressionMethod == IZipEntry::DEFLATED) {
        Int64 size;
        ze->GetSize(&size);
        Int32 bufSize = Elastos::Core::Math::Max(1024, (Int32)Elastos::Core::Math::Min(size, 65535ll));
        AutoPtr<IInflater> i;
        CInflater::New(TRUE, (IInflater**)&i);
        *is = (IInputStream*)new ZipInflaterInputStream(rafstrm, i, bufSize, (CZipEntry*)ze.Get());
    }
    else {
        *is = (IInputStream*)rafstrm;
    }

    REFCOUNT_ADD(*is);
    return NOERROR;
}

ECode ZipFile::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = mFileName;
    return NOERROR;
}

ECode ZipFile::GetSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);
    *size = -1;
    FAIL_RETURN(CheckNotClosed());
    return mEntries->GetSize(size);
}

ECode ZipFile::ReadCentralDir()
{
    /*
     * Scan back, looking for the End Of Central Directory field.  If
     * the archive doesn't have a comment, we'll hit it on the first
     * try.
     *
     * No need to synchronize mRaf here -- we only do this when we
     * first open the Zip file.
     */
    Int64 len;
    mRaf->GetLength(&len);
    Int64 scanOffset = len - IZipConstants::ENDHDR;
    if (scanOffset < 0) {
        return E_ZIP_EXCEPTION;
//        throw new ZipException("File too short to be a zip file: " + mRaf.length());
    }
    Int64 stopOffset = scanOffset - 65536;
    if (stopOffset < 0) {
        stopOffset = 0;
    }

    AutoPtr<IDataInput> di = IDataInput::Probe(mRaf);
    const Int32 ENDHEADERMAGIC = 0x06054b50;
    Int32 value;
    while(TRUE) {
        mRaf->Seek(scanOffset);
        di->ReadInt32(&value);
        if (Elastos::Core::Math::ReverseBytes(value) == ENDHEADERMAGIC) {
            break;
        }

        scanOffset--;
        if (scanOffset < stopOffset) {
            return E_ZIP_EXCEPTION;
//            throw new ZipException("EOCD not found; not a Zip archive?");
        }
    }

    // Read the End Of Central Directory. We could use ENDHDR instead of the magic number 18,
    // but we don't actually need all the header.
    AutoPtr<ArrayOf<Byte> > eocd = ArrayOf<Byte>::Alloc(18);
    di->ReadFully(eocd, 0, eocd->GetLength());

    // Pull out the information we need.
    AutoPtr<IBufferIterator> it;
    // CHeapBufferIterator::New(eocd, 0, eocd->GetLength(), ByteOrder_LITTLE_ENDIAN,
    //         (IBufferIterator**)&it);

    Int16 temp16;
    it->ReadInt16(&temp16);
    Int32 diskNumber = temp16 & 0xffff;
    it->ReadInt16(&temp16);
    Int32 diskWithCentralDir = temp16 & 0xffff;
    it->ReadInt16(&temp16);
    Int32 numEntries = temp16 & 0xffff;
    it->ReadInt16(&temp16);
    Int32 totalNumEntries = temp16 & 0xffff;
    it->Skip(4); // Ignore centralDirSize.
    Int32 centralDirOffset;
    it->ReadInt32(&centralDirOffset);

    if (numEntries != totalNumEntries || diskNumber != 0 || diskWithCentralDir != 0) {
        return E_ZIP_EXCEPTION;
//        throw new ZipException("spanned archives not supported");
    }

    // Seek to the first CDE and read all entries.
    AutoPtr<RAFStream> rafs = new RAFStream(mRaf, centralDirOffset);
    AutoPtr<IBufferedInputStream> bin;
    FAIL_RETURN(CBufferedInputStream::New((IInputStream*)rafs, 4096,
            (IBufferedInputStream**)&bin));
    IInputStream* is = IInputStream::Probe(bin.Get());

    AutoPtr<ICharset> charset; //StandardCharsets.UTF_8
    String name;
    AutoPtr<ArrayOf<Byte> > hdrBuf = ArrayOf<Byte>::Alloc(IZipConstants::CENHDR); // Reuse the same buffer for each entry.
    for (Int32 i = 0; i < numEntries; ++i) {
        AutoPtr<IZipEntry> newEntry;
        FAIL_RETURN(CZipEntry::New(*hdrBuf, is, charset, (IZipEntry**)&newEntry));
        newEntry->GetName(&name);
        // mEntries[name] = newEntry;
    }

    return NOERROR;
}


} // namespace Zip
} // namespace Utility
} // namespace Elastos
