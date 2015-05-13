
#ifndef __CZIPENTRY_H__
#define __CZIPENTRY_H__

#include "_CZipEntry.h"
#include "ZipEntry.h"

namespace Elastos {
namespace Utility {
namespace Zip {

CarClass(CZipEntry) , public ZipEntry
{
public:
    /**
     * Gets the comment for this {@code ZipEntry}.
     *
     * @return the comment for this {@code ZipEntry}, or {@code null} if there
     *         is no comment. If we're reading an archive with
     *         {@code ZipInputStream} the comment is not available.
     */
    CARAPI GetComment(
        /* [out] */ String* comment);

    /**
     * Gets the compressed size of this {@code ZipEntry}.
     *
     * @return the compressed size, or -1 if the compressed size has not been
     *         set.
     */
    CARAPI GetCompressedSize(
        /* [out] */ Int64* size);

    /**
     * Gets the checksum for this {@code ZipEntry}.
     *
     * @return the checksum, or -1 if the checksum has not been set.
     */
    CARAPI GetCrc(
        /* [out] */ Int64* checksum);

    /**
     * Gets the extra information for this {@code ZipEntry}.
     *
     * @return a byte array containing the extra information, or {@code null} if
     *         there is none.
     */
    CARAPI GetExtra(
        /* [out, callee] */ ArrayOf<Byte>** extra);

    /**
     * Gets the compression method for this {@code ZipEntry}.
     *
     * @return the compression method, either {@code DEFLATED}, {@code STORED}
     *         or -1 if the compression method has not been set.
     */
    CARAPI GetMethod(
        /* [out] */ Int32* method);

    /**
     * Gets the name of this {@code ZipEntry}.
     *
     * @return the entry name.
     */
    CARAPI GetName(
        /* [out] */ String* name);

    /**
     * Gets the uncompressed size of this {@code ZipEntry}.
     *
     * @return the uncompressed size, or {@code -1} if the size has not been
     *         set.
     */
    CARAPI GetSize(
        /* [out] */ Int64* size);

    /**
     * Gets the last modification time of this {@code ZipEntry}.
     *
     * @return the last modification time as the number of milliseconds since
     *         Jan. 1, 1970.
     */
    CARAPI GetTime(
        /* [out] */ Int64* time);

    /**
     * Determine whether or not this {@code ZipEntry} is a directory.
     *
     * @return {@code true} when this {@code ZipEntry} is a directory, {@code
     *         false} otherwise.
     */
    CARAPI IsDirectory(
        /* [out] */ Boolean* isDirectory);

    /**
     * Sets the comment for this {@code ZipEntry}.
     *
     * @param string
     *            the comment for this entry.
     */
    CARAPI SetComment(
        /* [in] */ const String& comment);

    /**
     * Sets the compressed size for this {@code ZipEntry}.
     *
     * @param value
     *            the compressed size (in bytes).
     */
    CARAPI SetCompressedSize(
        /* [in] */ Int64 size);

    /**
     * Sets the checksum for this {@code ZipEntry}.
     *
     * @param value
     *            the checksum for this entry.
     * @throws IllegalArgumentException
     *             if {@code value} is < 0 or > 0xFFFFFFFFL.
     */
    CARAPI SetCrc(
        /* [in] */ Int64 checksum);

    /**
     * Sets the extra information for this {@code ZipEntry}.
     *
     * @param data
     *            a byte array containing the extra information.
     * @throws IllegalArgumentException
     *             when the length of data is greater than 0xFFFF bytes.
     */
    CARAPI SetExtra(
        /* [in] */ ArrayOf<Byte>* data);

    /**
     * Sets the compression method for this {@code ZipEntry}.
     *
     * @param value
     *            the compression method, either {@code DEFLATED} or {@code
     *            STORED}.
     * @throws IllegalArgumentException
     *             when value is not {@code DEFLATED} or {@code STORED}.
     */
    CARAPI SetMethod(
        /* [in] */ Int32 method);

    /**
     * Sets the uncompressed size of this {@code ZipEntry}.
     *
     * @param value
     *            the uncompressed size for this entry.
     * @throws IllegalArgumentException
     *             if {@code value} < 0 or {@code value} > 0xFFFFFFFFL.
     */
    CARAPI SetSize(
        /* [in] */ Int64 size);

    /**
     * Sets the modification time of this {@code ZipEntry}.
     *
     * @param value
     *            the modification time as the number of milliseconds since Jan.
     *            1, 1970.
     */
    CARAPI SetTime(
        /* [in] */ Int64 time);

    /**
     * Constructs a new {@code ZipEntry} with the specified name.
     *
     * @param name
     *            the name of the ZIP entry.
     * @throws IllegalArgumentException
     *             if the name length is outside the range (> 0xFFFF).
     */
    CARAPI constructor(
        /* [in] */ const String& name);

    /**
     * Constructs a new {@code ZipEntry} using the values obtained from {@code
     * ze}.
     *
     * @param ze
     *            the {@code ZipEntry} from which to obtain values.
     */
    CARAPI constructor(
        /* [in] */ IZipEntry* ze);

    /*
     * Internal constructor.  Creates a new ZipEntry by reading the
     * Central Directory Entry from "in", which must be positioned at
     * the CDE signature.
     *
     * On exit, "in" will be positioned at the start of the next entry.
     */
    CARAPI constructor (
        /* [in] */ const ArrayOf<Byte>& hdrBuf,
        /* [in] */ IInputStream *in);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);
};

} // namespace Zip
} // namespace Utility
} // namespace Elastos

#endif //__CZIPENTRY_H__
