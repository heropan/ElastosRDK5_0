
#ifndef __ELASTOS_UTILITY_CZIPENTRY_H__
#define __ELASTOS_UTILITY_CZIPENTRY_H__

#include "_Elastos_Utility_Zip_CZipEntry.h"
#include "ZipEntry.h"

namespace Elastos {
namespace Utility {
namespace Zip {

CarClass(CZipEntry) , public ZipEntry
{
public:
    CAR_OBJECT_DECL()

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
};

} // namespace Zip
} // namespace Utility
} // namespace Elastos

#endif //__ELASTOS_UTILITY_CZIPENTRY_H__
