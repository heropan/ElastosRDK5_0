#ifndef __CIOUTILS_H__
#define __CIOUTILS_H__

#include "_CIoUtils.h"

namespace Elastos {
namespace IO {

CarClass(CIoUtils)
{
public:
    /**
     * Calls close(2) on 'fd'. Also resets the internal int to -1. Does nothing if 'fd' is null
     * or invalid.
     */
    CARAPI Close(
        /* [in] */ IFileDescriptor* fd);

    /**
     * Closes 'closeable', ignoring any checked exceptions. Does nothing if 'closeable' is null.
     */
    CARAPI CloseQuietly(
        /* [in] */ ICloseable* closeable);

    /**
     * Closes 'fd', ignoring any exceptions. Does nothing if 'fd' is null or invalid.
     */
    CARAPI CloseQuietlyEx(
        /* [in] */ IFileDescriptor* fd);

    /**
     * Sets 'fd' to be blocking or non-blocking, according to the state of 'blocking'.
     */
    CARAPI SetBlocking(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ Boolean blocking);

    /**
     * Returns the contents of 'path' as a byte array.
     */
    CARAPI ReadFileAsByteArray(
        /* [in] */ const String& path,
        /* [out, callee] */ ArrayOf<Byte>** bytes);

    /**
     * Returns the contents of 'path' as a string. The contents are assumed to be UTF-8.
     */
    CARAPI ReadFileAsString(
        /* [in] */ const String& path,
        /* [out] */ String* result);

    CARAPI ReadFileAsBytes(
        /* [in] */ const String& path,
        /* [out] */ IUnsafeByteSequence** usbs);

    /**
     * Recursively delete everything in {@code dir}.
     */
    // TODO: this should specify paths as Strings rather than as Files
    CARAPI DeleteContents(
        /* [in] */ IFile* dir);
};

} // namespace IO
} // namespace Elastos

#endif
