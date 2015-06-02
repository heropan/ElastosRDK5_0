
#ifndef __ELASTOS_UTILITY_ZIPINPUTSTREAM_H__
#define __ELASTOS_UTILITY_ZIPINPUTSTREAM_H__

#include "CMemory.h"
#include "CPushbackInputStream.h"
#include "InflaterInputStream.h"
#include "CZipEntry.h"
#include "CCRC32.h"

using Elastos::IO::IMemory;

namespace Elastos {
namespace Utility {
namespace Zip {

/**
 * This class provides an implementation of {@code FilterInputStream} that
 * uncompresses data from a <i>ZIP-archive</i> input stream.
 * <p>
 * A <i>ZIP-archive</i> is a collection of compressed (or uncompressed) files -
 * the so called ZIP entries. Therefore when reading from a {@code
 * ZipInputStream} first the entry's attributes will be retrieved with {@code
 * getNextEntry} before its data is read.
 * <p>
 * While {@code InflaterInputStream} can read a compressed <i>ZIP-archive</i>
 * entry, this extension can read uncompressed entries as well.
 * <p>
 * Use {@code ZipFile} if you can access the archive as a file directly.
 *
 * @see ZipEntry
 * @see ZipFile
 */
class ZipInputStream
    : public InflaterInputStream
    , public IZipInputStream
{
public:
    CAR_INTERFACE_DECL()

    ZipInputStream();

    virtual ~ZipInputStream();

    /**
     * Closes this {@code ZipInputStream}.
     *
     * @throws IOException
     *             if an {@code IOException} occurs.
     */
    //@Override
    CARAPI Close();

    /**
     * Closes the current ZIP entry and positions to read the next entry.
     *
     * @throws IOException
     *             if an {@code IOException} occurs.
     */
    virtual CARAPI CloseEntry();

    /**
     * Reads the next entry from this {@code ZipInputStream} or {@code null} if
     * no more entries are present.
     *
     * @return the next {@code ZipEntry} contained in the input stream.
     * @throws IOException
     *             if an {@code IOException} occurs.
     * @see ZipEntry
     */
    virtual CARAPI GetNextEntry(
        /* [out] */ IZipEntry** entry);

    /**
     * Reads up to the specified number of uncompressed bytes into the buffer
     * starting at the offset.
     *
     * @return the number of bytes read
     */
    //@Override
    CARAPI Read(
        /* [out] */ ArrayOf<Byte>* buffer,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 byteCount,
        /* [out] */ Int32* number);

    //@Override
    CARAPI Available(
        /* [out] */ Int32* number);

protected:
    /**
     * Constructs a new {@code ZipInputStream} from the specified input stream.
     *
     * @param stream
     *            the input stream to representing a ZIP archive.
     */
    CARAPI constructor(
        /* [in] */ IInputStream* stream);

    /**
     * creates a {@link ZipEntry } with the given name.
     *
     * @param name
     *            the name of the entry.
     * @return the created {@code ZipEntry}.
     */
    virtual CARAPI_(AutoPtr<CZipEntry>) CreateZipEntry(
        /* [in] */ const String& name);

private:
    CARAPI ReadAndVerifyDataDescriptor(
        /* [in] */ Int32 inB,
        /* [in] */ Int32 out);

    CARAPI_(Int32) PeekShort(
        /* [in] */ IMemory* memory,
        /* [in] */ Int32 offset);

    CARAPI CheckClosed();

public:
    static const Int32 ZIPLocalHeaderVersionNeeded = 20;

private:
    Boolean mEntriesEnd;

    Boolean mHasDD;

    Int32 mEntryIn;

    Int32 mInRead;
    Int32 mLastRead;

    AutoPtr<CZipEntry> mCurrentEntry;

    AutoPtr<ArrayOf<Byte> > mHdrBuf;

    AutoPtr<CCRC32> mCrc;

    AutoPtr<ArrayOf<Byte> > mNameBuf;
};

} // namespace Zip
} // namespace Utility
} // namespace Elastos

#endif //__ELASTOS_UTILITY_ZIPINPUTSTREAM_H__

