
#include "CIoUtils.h"
#include "IoUtils.h"

namespace Elastos {
namespace IO {

ECode CIoUtils::Close(
    /* [in] */ IFileDescriptor* fd)
{
    return IoUtils::Close(fd);
}

ECode CIoUtils::CloseQuietly(
    /* [in] */ ICloseable* closeable)
{
    return IoUtils::CloseQuietly(closeable);
}

ECode CIoUtils::CloseQuietly(
    /* [in] */ IFileDescriptor* fd)
{
    return IoUtils::CloseQuietlyEx(fd);
}

ECode CIoUtils::SetBlocking(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Boolean blocking)
{
    return IoUtils::SetBlocking(fd, blocking);
}

ECode CIoUtils::ReadFileAsByteArray(
    /* [in] */ const String& path,
    /* [out, callee] */ ArrayOf<Byte>** bytes)
{
    return IoUtils::ReadFileAsByteArray(path, bytes);
}

ECode CIoUtils::ReadFileAsString(
    /* [in] */ const String& path,
    /* [out] */ String* result)
{
    return IoUtils::ReadFileAsString(path, result);
}

ECode CIoUtils::ReadFileAsBytes(
    /* [in] */ const String& path,
    /* [out] */ IUnsafeByteSequence** usbs)
{
    return IoUtils::ReadFileAsBytes(path, usbs);
}

// TODO: this should specify paths as Strings rather than as Files
ECode CIoUtils::DeleteContents(
    /* [in] */ IFile* dir)
{
    return IoUtils::DeleteContents(dir);
}

} // namespace IO
} // namespace Elastos
