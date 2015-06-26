
#include "FileInputStream.h"
#include "CFile.h"
//#include "IoUtils.h"
#include "OsConstants.h"
#include "CLibcore.h"
#include "CIoBridge.h"
#include "NioUtils.h"
#include "CFileDescriptor.h"
#include "AutoLock.h"

using Elastos::Droid::System::OsConstants;
using Libcore::IO::ILibcore;
using Libcore::IO::CLibcore;
using Libcore::IO::IOs;
using Libcore::IO::IIoBridge;
using Libcore::IO::CIoBridge;

namespace Elastos {
namespace IO {

CAR_INTERFACE_IMPL(FileInputStream, InputStream, IFileInputStream)

FileInputStream::FileInputStream()
    : mShouldClose(FALSE)
{}

FileInputStream::~FileInputStream()
{
    // try {
    // TODO: can't call GetSelfLock() in destruct function
    //
    // try {
    //     if (guard != null) {
    //         guard.warnIfOpen();
    //     }
    //     close();
    // } finally {
    //     try {
    //         super.finalize();
    //     } catch (Throwable t) {
    //         // for consistency with the RI, we must override Object.finalize() to
    //         // remove the 'throws Throwable' clause.
    //         throw new AssertionError(t);
    //     }
    // }
}

ECode FileInputStream::constructor(
    /* [in] */ IFile* file)
{
    if (file == NULL) {
//        throw new NullPointerException("file == null");
        return E_NULL_POINTER_EXCEPTION;
    }
    CFileDescriptor::New((IFileDescriptor**)&mFd);
    String path;
    file->GetPath(&path);
    AutoPtr<CIoBridge> ioBridge;
    CIoBridge::AcquireSingletonByFriend((CIoBridge**)&ioBridge);
    AutoPtr<IFileDescriptor> fd;
    FAIL_RETURN(ioBridge->Open(path, OsConstants::_O_RDONLY, (IFileDescriptor**)&fd));
    Int32 ifd;
    fd->GetDescriptor(&ifd);
    mFd->SetDescriptor(ifd);
    mShouldClose = TRUE;
    return NOERROR;
}

ECode FileInputStream::constructor(
    /* [in] */ IFileDescriptor* fd)
{
    if (fd == NULL) {
//        throw new NullPointerException("fd == null");
        return E_NULL_POINTER_EXCEPTION;
    }
    mFd = fd;
    mShouldClose = FALSE;
    return NOERROR;
}

ECode FileInputStream::constructor(
    /* [in] */ const String& fileName)
{
    AutoPtr<IFile> file;
    CFile::New(fileName, (IFile**)&file);
    return constructor(file);
}

ECode FileInputStream::Available(
    /* [out] */ Int32* avail)
{
    VALIDATE_NOT_NULL(avail)

    Int32 fd;
    mFd->GetDescriptor(&fd);
    AutoPtr<CIoBridge> ioBridge;
    CIoBridge::AcquireSingletonByFriend((CIoBridge**)&ioBridge);
    // return IoUtils::Libcore2IoECode(ioBridge->Available(fd, avail));
}

ECode FileInputStream::Close()
{
    // guard.close();
    // synchronized (this) {
    //     if (channel != null) {
    //         channel.close();
    //     }
    //     if (shouldClose) {
    //         IoBridge.closeAndSignalBlockedThreads(fd);
    //     } else {
    //         // An owned fd has been invalidated by IoUtils.close, but
    //         // we need to explicitly stop using an unowned fd (http://b/4361076).
    //         fd = new FileDescriptor();
    //     }
    // }
}

ECode FileInputStream::GetChannel(
    /* [out] */ IFileChannel** channel)
{
    VALIDATE_NOT_NULL(channel)

    AutoLock lock(this);

    if (mChannel == NULL) {
        // mChannel = NioUtils::NewFileChannel(THIS_PROBE(IObject), mFd, OsConstants::sO_RDONLY);
    }
    *channel = mChannel;
    REFCOUNT_ADD(*channel)
    return NOERROR;
}

ECode FileInputStream::GetFD(
    /* [out] */ IFileDescriptor** fd)
{
    VALIDATE_NOT_NULL(fd)

    *fd = (IFileDescriptor*)mFd.Get();
    REFCOUNT_ADD(*fd);
    return NOERROR;
}

ECode FileInputStream::Read(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)

    // return Streams.readSingleByte(this);
    return NOERROR;;
}

ECode FileInputStream::Read(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 byteOffset,
    /* [in] */ Int32 byteCount,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number)
    *number = -1;
    VALIDATE_NOT_NULL(buffer);

    Int32 fd;
    mFd->GetDescriptor(&fd);
    AutoPtr<IIoBridge> ioBridge;
    CIoBridge::AcquireSingleton((IIoBridge**)&ioBridge);
    // return IoUtils::Libcore2IoECode(ioBridge->Read(fd,
    //         buffer, byteOffset, byteCount, number));
}

ECode FileInputStream::Skip(
    /* [in] */ Int64 byteCount,
    /* [out] */ Int64* number)
{
    VALIDATE_NOT_NULL(number)
    *number = 0;

    if (byteCount < 0) {
//      throw new IOException("byteCount < 0: " + byteCount);
        return E_IO_EXCEPTION;
    }
    // try {
        // Try lseek(2). That returns the new offset, but we'll throw an
        // exception if it couldn't perform exactly the seek we asked for.
        Int32 fd;
        AutoPtr<ILibcore> libcore;
        CLibcore::AcquireSingleton((ILibcore**)&libcore);
        AutoPtr<IOs> os;
        libcore->GetOs((IOs**)&os);
        // FAIL_RETURN(IoUtils::Libcore2IoECode(os->Lseek(fd, byteCount, OsConstants::_SEEK_CUR, number)));
        *number = byteCount;
        return NOERROR;
    // } catch (ErrnoException errnoException) {
    //     if (errnoException.errno == ESPIPE) {
    //         // You can't seek on a pipe, so fall back to the superclass' implementation.
    //         return super.skip(byteCount);
    //     }
    //     throw errnoException.rethrowAsIOException();
    // }
}

} // namespace IO
} // namespace Elastos
