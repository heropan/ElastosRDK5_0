
#ifndef __IOUTILS_H__
#define __IOUTILS_H__

#include "Elastos.CoreLibrary_server.h"

using Elastos::Net::ISocket;

namespace Elastos {
namespace IO {

class IoUtils
{
public:
    /**
     * Calls close(2) on 'fd'. Also resets the internal int to -1.
     */
    static CARAPI Close(
        /* [in] */ IFileDescriptor* fd);

    /**
     * Closes 'closeable', ignoring any exceptions. Does nothing if 'closeable' is null.
     */
    static CARAPI CloseQuietly(
        /* [in] */ ICloseable* closeable);

    static CARAPI CloseQuietly(
        /* [in] */ ISocket* socket);

    static CARAPI CloseQuietlyEx(
        /* [in] */ IFileDescriptor* fd);

    /**
     * Sets 'fd' to be blocking or non-blocking, according to the state of 'blocking'.
     */
    static CARAPI SetBlocking(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ Boolean blocking);

    static CARAPI ReadFileAsByteArray(
        /* [in] */ const String& path,
        /* [out, callee] */ ArrayOf<Byte>** bytes);

    static CARAPI ReadFileAsString(
        /* [in] */ const String& path,
        /* [out] */ String* result);

    static CARAPI ReadFileAsBytes(
        /* [in] */ const String& path,
        /* [out] */ IUnsafeByteSequence** usbs);

    static CARAPI DeleteContents(
        /* [in] */ IFile* dir);

    static CARAPI Libcore2IoECode(
        /* [in] */ ECode ec);

private:
    IoUtils();
};

} // namespace IO
} // namespace Elastos

#endif //__IOUTILS_H__
