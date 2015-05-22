
#ifndef __ZIPFILE_H__
#define __ZIPFILE_H__

#ifdef ELASTOS_CORELIBRARY
#include "Elastos.CoreLibrary_server.h"
#include "CObjectContainer.h"
#include "CDataInputStream.h"
#include "CBufferedInputStream.h"
#include "CRandomAccessFile.h"
#include "CFile.h"
#else
#include "Elastos.CoreLibrary.h"
#endif
#include "CZipEntry.h"
#include "InflaterInputStream.h"
#include <cmdef.h>
#include <elastos/HashMap.h>
#include <elastos/Mutex.h>
#include <InputStream.h>

using Elastos::IO::IRandomAccessFile;
using Elastos::IO::InputStream;
using Elastos::IO::IInputStream;
using Elastos::IO::IFile;
using Elastos::Utility::HashMap;
using Elastos::Core::Threading::Mutex;

extern "C" const InterfaceID EIID_ZipFileRAFStream;

namespace Elastos {
namespace Utility {
namespace Zip {

/**
 * This class provides random read access to a <i>ZIP-archive</i> file.
 * <p>
 * While {@code ZipInputStream} provides stream based read access to a
 * <i>ZIP-archive</i>, this class implements more efficient (file based) access
 * and makes use of the <i>central directory</i> within a <i>ZIP-archive</i>.
 * <p>
 * Use {@code ZipOutputStream} if you want to create an archive.
 * <p>
 * A temporary ZIP file can be marked for automatic deletion upon closing it.
 *
 * @see ZipEntry
 * @see ZipOutputStream
 */
class ZipFile
{
public:
    /**
     * Wrap a stream around a RandomAccessFile.  The RandomAccessFile is shared
     * among all streams returned by getInputStream(), so we have to synchronize
     * access to it.  (We can optimize this by adding buffering here to reduce
     * collisions.)
     *
     * <p>We could support mark/reset, but we don't currently need them.
     */
    class RAFStream
        : public ElRefBase
        , public IInputStream
        , public InputStream
    {
    public:
        CAR_INTERFACE_DECL()

        RAFStream(
            /* [in] */ IRandomAccessFile* raf,
            /* [in] */ Int64 pos);

        CARAPI Available(
            /* [out] */ Int32* number);

        CARAPI Read(
            /* [out] */ Int32* value);

        CARAPI ReadBytesEx(
            /* [out] */ ArrayOf<Byte>* buffer,
            /* [in] */ Int32 offset,
            /* [in] */ Int32 length,
            /* [out] */ Int32* number);

        CARAPI Skip(
            /* [in] */ Int64 byteCount,
            /* [out] */ Int64* number);

        CARAPI Close();

        CARAPI Mark(
            /* [in] */ Int32 readLimit);

        CARAPI IsMarkSupported(
            /* [out] */ Boolean* supported);

        CARAPI ReadBytes(
            /* [out] */ ArrayOf<Byte>* buffer,
            /* [out] */ Int32* number);

        CARAPI Reset();

        CARAPI GetLock(
            /* [out] */ IInterface** lockobj);

    public:
        AutoPtr<IRandomAccessFile> mSharedRaf;
        Mutex mSharedRafLock;
        Mutex mLock;
        Int64 mOffset;
        Int64 mLength;
    };

    class ZipInflaterInputStream
        : public ElRefBase
        , public InflaterInputStream
        , public IInflaterInputStream
    {
    public:
        ZipInflaterInputStream(
            /* [in] */ IInputStream* is,
            /* [in] */ IInflater* inf,
            /* [in] */ Int32 bsize,
            /* [in] */ CZipEntry* entry);

        CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        //@Override
        CARAPI ReadBytesEx(
            /* [out] */ ArrayOf<Byte>* buffer,
            /* [in] */ Int32 off,
            /* [in] */ Int32 nbytes,
            /* [out] */ Int32* number);

        //@Override
        CARAPI Available(
            /* [out] */ Int32* number);

        CARAPI Close();

        CARAPI Mark(
            /* [in] */ Int32 mark);

        CARAPI IsMarkSupported(
            /* [out] */  Boolean* isMark);

        CARAPI Read(
            /* [out */ Int32* value);

        CARAPI ReadBytes(
            /* [out] */ ArrayOf<Byte>* buffer,
            /* [out] */ Int32 *number);

        CARAPI Reset();

        CARAPI Skip(
            /* [in] */ Int64 offset,
            /* [out] */ Int64* number);

        CARAPI GetLock(
            /* [out] */ IInterface** lockobj);

    public:
        AutoPtr<CZipEntry> mEntry;
        Int64 mBytesRead;
        Mutex mLock;
    };

public:
    ZipFile();

    virtual ~ZipFile();

    /**
     * Closes this ZIP file. This method is idempotent.
     *
     * @throws IOException
     *             if an IOException occurs.
     */
    virtual CARAPI Close();

    /**
     * Returns an enumeration of the entries. The entries are listed in the
     * order in which they appear in the ZIP archive.
     *
     * @return the enumeration of the entries.
     * @throws IllegalStateException if this ZIP file has been closed.
     */
    virtual CARAPI GetEntries(
        /* [out] */ IObjectContainer** entries);

    /**
     * Gets the ZIP entry with the specified name from this {@code ZipFile}.
     *
     * @param entryName
     *            the name of the entry in the ZIP file.
     * @return a {@code ZipEntry} or {@code null} if the entry name does not
     *         exist in the ZIP file.
     * @throws IllegalStateException if this ZIP file has been closed.
     */
    virtual CARAPI GetEntry(
        /* [in] */ const String& entryName,
        /* [out] */ IZipEntry** entry);

    /**
     * Returns an input stream on the data of the specified {@code ZipEntry}.
     *
     * @param entry
     *            the ZipEntry.
     * @return an input stream of the data contained in the {@code ZipEntry}.
     * @throws IOException
     *             if an {@code IOException} occurs.
     * @throws IllegalStateException if this ZIP file has been closed.
     */
    virtual CARAPI GetInputStream(
        /* [in] */ IZipEntry* entry,
        /* [out] */ IInputStream** is);

    /**
     * Gets the file name of this {@code ZipFile}.
     *
     * @return the file name of this {@code ZipFile}.
     */
    virtual CARAPI GetName(
        /* [out] */ String* name);

    /**
     * Returns the number of {@code ZipEntries} in this {@code ZipFile}.
     *
     * @return the number of entries in this file.
     * @throws IllegalStateException if this ZIP file has been closed.
     */
    virtual CARAPI GetSize(
        /* [out] */ Int32* size);

protected:
    /**
     * Constructs a new {@code ZipFile} with the specified file.
     *
     * @param file
     *            the file to read from.
     * @throws ZipException
     *             if a ZIP error occurs.
     * @throws IOException
     *             if an {@code IOException} occurs.
     */
    CARAPI Init(
        /* [in] */ IFile* file);

    /**
     * Opens a file as <i>ZIP-archive</i>. "mode" must be {@code OPEN_READ} or
     * {@code OPEN_DELETE} . The latter sets the "delete on exit" flag through a
     * file.
     *
     * @param file
     *            the ZIP file to read.
     * @param mode
     *            the mode of the file open operation.
     * @throws IOException
     *             if an {@code IOException} occurs.
     */
    CARAPI Init(
        /* [in] */ IFile* file,
        /* [in] */ Int32 mode);

    /**
     * Opens a ZIP archived file.
     *
     * @param name
     *            the name of the ZIP file.
     * @throws IOException
     *             if an IOException occurs.
     */
    CARAPI Init(
        /* [in] */ const String& name);

private:
    CARAPI CheckNotClosed();

    /**
     * Find the central directory and read the contents.
     *
     * <p>The central directory can be followed by a variable-length comment
     * field, so we have to scan through it backwards.  The comment is at
     * most 64K, plus we have 18 bytes for the end-of-central-dir stuff
     * itself, plus apparently sometimes people throw random junk on the end
     * just for the fun of it.
     *
     * <p>This is all a little wobbly.  If the wrong value ends up in the EOCD
     * area, we're hosed. This appears to be the way that everybody handles
     * it though, so we're in good company if this fails.
     */
    CARAPI ReadCentralDir();

public:
    /**
     * General Purpose Bit Flags, Bit 3.
     * If this bit is set, the fields crc-32, compressed
     * size and uncompressed size are set to zero in the
     * local header.  The correct values are put in the
     * data descriptor immediately following the compressed
     * data.  (Note: PKZIP version 2.04g for DOS only
     * recognizes this bit for method 8 compression, newer
     * versions of PKZIP recognize this bit for any
     * compression method.)
     */
    static const Int32 GPBF_DATA_DESCRIPTOR_FLAG; // android-added

    /**
     * General Purpose Bit Flags, Bit 11.
     * Language encoding flag (EFS).  If this bit is set,
     * the filename and comment fields for this file
     * must be encoded using UTF-8.
     */
    static const Int32 GPBF_UTF8_FLAG; // android-added

private:
    String mFileName;

    AutoPtr<IFile> mFileToDeleteOnClose;

    AutoPtr<IRandomAccessFile> mRaf;
    Mutex mRafLock;

    HashMap<String, AutoPtr<IZipEntry> > mEntries;

//    CloseGuard guard = CloseGuard.get();
};

} // namespace Zip
} // namespace Utility
} // namespace Elastos

#endif //__ZIPFILE_H__
