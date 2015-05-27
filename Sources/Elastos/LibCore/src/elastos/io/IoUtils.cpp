
#include "coredef.h"
#include "IoUtils.h"
#include "CFileDescriptor.h"
#include "CFile.h"
#include "CRandomAccessFile.h"
#include "CUnsafeByteSequence.h"
#include <fcntl.h>
#include "CLibcore.h"

using Libcore::IO::ILibcore;
using Libcore::IO::CLibcore;
using Libcore::IO::IOs;

namespace Elastos {
namespace IO {

IoUtils::IoUtils()
{}

/**
 * Calls close(2) on 'fd'. Also resets the internal int to -1.
 */
ECode IoUtils::Close(
    /* [in] */ IFileDescriptor* fd)
{
    // try {
    Boolean isValid;
    if (fd != NULL && (fd->Valid(&isValid), isValid)) {
        Int32 nativeFd;
        fd->GetDescriptor(&nativeFd);
        fd->SetDescriptor(-1);
        AutoPtr<CLibcore> obj;
        CLibcore::AcquireSingletonByFriend((CLibcore**)&obj);
        AutoPtr<ILibcore> libcore = (ILibcore*)obj;
        AutoPtr<IOs> os;
        libcore->GetOs((IOs**)&os);
        return Libcore2IoECode(os->Close(nativeFd));
    }
    return NOERROR;
    // } catch (ErrnoException errnoException) {
    //     throw errnoException.rethrowAsIOException();
    // }
}

/**
 * Closes 'closeable', ignoring any exceptions. Does nothing if 'closeable' is null.
 */
ECode IoUtils::CloseQuietly(
    /* [in] */ ICloseable* closeable)
{
    if (closeable != NULL) {
        // try {
        closeable->Close();
        // } catch (IOException ignored) {
        // }
    }
    return NOERROR;
}

ECode IoUtils::CloseQuietly(
    /* [in] */ ISocket* socket)
{
    if (socket != NULL) {
        // try {
        return socket->Close();
        // } catch (Exception ignored) {
        // }
    }
    return NOERROR;
}

ECode IoUtils::CloseQuietly(
    /* [in] */ IFileDescriptor* fd)
{
    return Close(fd);
}

/**
 * Sets 'fd' to be blocking or non-blocking, according to the state of 'blocking'.
 */
ECode IoUtils::SetBlocking(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Boolean blocking)
{
    // try {
    Int32 flags;
    Int32 desciptor;
    fd->GetDescriptor(&desciptor);
    AutoPtr<ILibcore> libcore;
    CLibcore::AcquireSingleton((ILibcore**)&libcore);
    AutoPtr<IOs> os;
    libcore->GetOs((IOs**)&os);
    FAIL_RETURN(os->FcntlVoid(desciptor, F_GETFL, &flags));
    if (!blocking) {
        flags |= O_NONBLOCK;
    }
    else {
        flags &= ~O_NONBLOCK;
    }
    Int32 temp;
    return os->FcntlInt64(desciptor, F_SETFL, flags, &temp);
    // } catch (ErrnoException errnoException) {
    //     throw errnoException.rethrowAsIOException();
    // }
}

/**
 * Returns the contents of 'path' as a byte array.
 */
ECode IoUtils::ReadFileAsByteArray(
    /* [in] */ const String& path,
    /* [out, callee] */ ArrayOf<Byte>** bytes)
{
    VALIDATE_NOT_NULL(bytes);
    AutoPtr<IUnsafeByteSequence> usbs;
    ReadFileAsBytes(path, (IUnsafeByteSequence**)&usbs);
    return usbs->ToByteArray(bytes);
}

/**
 * Returns the contents of 'path' as a string. The contents are assumed to be UTF-8.
 */
ECode IoUtils::ReadFileAsString(
    /* [in] */ const String& path,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<IUnsafeByteSequence> usbs;
    ReadFileAsBytes(path, (IUnsafeByteSequence**)&usbs);
    return usbs->ToString(result);
}

ECode IoUtils::ReadFileAsBytes(
    /* [in] */ const String& path,
    /*[out] */ IUnsafeByteSequence** usbs)
{
    VALIDATE_NOT_NULL(usbs);
    AutoPtr<IRandomAccessFile> f;
    AutoPtr<IUnsafeByteSequence> bytes;
    Int64 fileLength;
    CRandomAccessFile::New(path, String("r"), (IRandomAccessFile**)&f);
    f->GetLength(&fileLength);
    CUnsafeByteSequence::New((Int32)fileLength,(IUnsafeByteSequence**)&bytes);
    AutoPtr<ArrayOf<Byte> > buffer= ArrayOf<Byte>::Alloc(8192);
    Int32 byteCount;
    while(TRUE) {
        ECode ec = f->ReadBytes(buffer, &byteCount);
        if(FAILED(ec)) {
            CloseQuietly(f);
            return ec;
        }

        if (byteCount == -1) {
            *usbs = bytes;
            INTERFACE_ADDREF(*usbs);
            return NOERROR;
        }

        ec = bytes->Write(buffer, 0, byteCount);
        if (FAILED(ec)) {
            CloseQuietly(f);
            return ec;
        }
    }
    return NOERROR;
}

/**
 * Recursively delete everything in {@code dir}.
 */
// TODO: this should specify paths as Strings rather than as Files
ECode IoUtils::DeleteContents(
    /* [in] */ IFile* dir)
{
    return NOERROR;
}

ECode IoUtils::Libcore2IoECode(
    /* [in] */ ECode ec)
{
    switch(ec) {
    case NOERROR:
        return NOERROR;
    case E_LIBCORE_IO_EXCEPTION:
        return E_IO_EXCEPTION;
    case E_LIBCORE_FILE_NOT_FOUND_EXCEPTION:
        return E_FILE_NOT_FOUND_EXCEPTION;
    default:
        return E_FAIL;
    }
}

} // namespace IO
} // namespace Elastos
