
#ifndef __ZIPOUTPUTSTREAM_H__
#define __ZIPOUTPUTSTREAM_H__

#ifdef ELASTOS_CORE
#include "Elastos.CoreLibrary_server.h"
#include "CByteArrayOutputStream.h"
#else
#include "Elastos.CoreLibrary.h"
#endif

#include "cmdef.h"
#include "DeflaterOutputStream.h"
#include "CZipEntry.h"
#include "CCRC32.h"
#include <elastos/Vector.h>

using Elastos::Utility::Vector;
using Elastos::IO::IOutputStream;
using Elastos::IO::IByteArrayOutputStream;

namespace Elastos {
namespace Utility {
namespace Zip {

class ZipOutputStream : public DeflaterOutputStream
{
public:
    ZipOutputStream();

    virtual ~ZipOutputStream();

    /**
     * Closes the current {@code ZipEntry}, if any, and the underlying output
     * stream. If the stream is already closed this method does nothing.
     *
     * @throws IOException
     *             If an error occurs closing the stream.
     */
    //@Override
    CARAPI Close();

    /**
     * Closes the current {@code ZipEntry}. Any entry terminal data is written
     * to the underlying stream.
     *
     * @throws IOException
     *             If an error occurs closing the entry.
     */
    virtual CARAPI CloseEntry();

    /**
     * Indicates that all entries have been written to the stream. Any terminal
     * information is written to the underlying stream.
     *
     * @throws IOException
     *             if an error occurs while terminating the stream.
     */
    //@Override
    CARAPI Finish();

    /**
     * Writes entry information to the underlying stream. Data associated with
     * the entry can then be written using {@code write()}. After data is
     * written {@code closeEntry()} must be called to complete the writing of
     * the entry to the underlying stream.
     *
     * @param ze
     *            the {@code ZipEntry} to store.
     * @throws IOException
     *             If an error occurs storing the entry.
     * @see #write
     */
    virtual CARAPI PutNextEntry(
        /* [in] */ IZipEntry* ze);

    /**
     * Sets the {@code ZipFile} comment associated with the file being written.
     *
     * @param comment
     *            the comment associated with the file.
     */
    virtual CARAPI SetComment(
        /* [in] */ const String& comment);

    /**
     * Sets the compression level to be used for writing entry data. This level
     * may be set on a per entry basis. The level must have a value between -1
     * and 8 according to the {@code Deflater} compression level bounds.
     *
     * @param level
     *            the compression level (ranging from -1 to 8).
     * @see Deflater
     */
    virtual CARAPI SetLevel(
        /* [in] */ Int32 level);

    /**
     * Sets the compression method to be used when compressing entry data.
     * method must be one of {@code STORED} (for no compression) or {@code
     * DEFLATED}.
     *
     * @param method
     *            the compression method to use.
     */
    virtual CARAPI SetMethod(
        /* [in] */ Int32 method);

    /**
     * Writes data for the current entry to the underlying stream.
     *
     * @exception IOException
     *                If an error occurs writing to the stream
     */
    //@Override
    CARAPI WriteBytesEx(
        /* [in] */ const ArrayOf<Byte>& buffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 byteCount);

    static CARAPI_(Int32) GetUtf8Count(
        /* [in] */ const String& value);

    static CARAPI_(AutoPtr<ArrayOf<Byte> >) ToUTF8Bytes(
        /* [in] */ const String& value,
        /* [in] */ Int32 length);

protected:
    /**
     * Constructs a new {@code ZipOutputStream} with the specified output
     * stream.
     *
     * @param p1
     *            the {@code OutputStream} to write the data to.
     */
    CARAPI Init(
        /* [in] */ IOutputStream* p1);

private:
    CARAPI_(Int64) WriteInt64(
        /* [in] */ IOutputStream* os,
        /* [in] */ Int64 i);

    CARAPI_(Int32) WriteInt16(
        /* [in] */ IOutputStream* os,
        /* [in] */ Int32 i);

    CARAPI CheckClosed();

protected:
    static const Int32 ZIPLocalHeaderVersionNeeded = 20;

private:
    String mComment;

    Vector<String> mEntries;//why not use hashset?

    Int32 mCompressMethod;

    Int32 mCompressLevel;

    AutoPtr<IByteArrayOutputStream> mCDir;

    AutoPtr<CZipEntry> mCurrentEntry;

    AutoPtr<CCRC32> mCrc;

    Int32 mOffset;
    Int32 mCurOffset;
    Int32 mNameLength;

    AutoPtr<ArrayOf<Byte> > mNameBytes;
};

} // namespace Zip
} // namespace Utility
} // namespace Elastos

#endif //__ZIPOUTPUTSTREAM_H__
