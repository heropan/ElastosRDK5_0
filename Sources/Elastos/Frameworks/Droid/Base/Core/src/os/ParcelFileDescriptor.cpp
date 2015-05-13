
#include "ext/frameworkext.h"
#include <sys/stat.h>
#include <unistd.h>
#include "os/ParcelFileDescriptor.h"
#include "os/CParcelFileDescriptor.h"
#include "os/CParcel.h"
#include "elastos/Slogger.h"

using Elastos::IO::CFileDescriptor;
using Elastos::IO::IIoUtils;
using Elastos::IO::CIoUtils;
using Libcore::IO::IIoBridge;
using Libcore::IO::CIoBridge;
using Libcore::IO::IOsConstants;
using Libcore::IO::COsConstants;
using Libcore::IO::ILibcore;
using Libcore::IO::CLibcore;
using Libcore::IO::IOs;
using Elastos::Utility::Logging::Slogger;

namespace Elastos {
namespace Droid {
namespace Os {

static const String TAG("ParcelFileDescriptor");

static ECode Libcore2IoECode(
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

ParcelFileDescriptor::AutoCloseInputStream::AutoCloseInputStream()
    : mShouldClose(FALSE)
{
    mLock = new LockObject();
}

ECode ParcelFileDescriptor::AutoCloseInputStream::Init(
    /* [in] */ IParcelFileDescriptor* pfd)
{
    AutoPtr<IFileDescriptor> fd;
    pfd->GetFileDescriptor((IFileDescriptor**)&fd);
    if (fd == NULL) {
        //throw new NullPointerException("fd == null");
        return E_NULL_POINTER_EXCEPTION;
    }
    mFd = fd;
    mShouldClose = FALSE;
    mPfd = pfd;
    return NOERROR;
}

ECode ParcelFileDescriptor::AutoCloseInputStream::Close()
{
    //try {
    mPfd->Close();
    //} finally {
    //super.close();
    // BEGIN android-changed
    Object::Autolock lock(mLock);

    // if (mChannel != NULL) {
    //     mChannel->Close();
    // }
    if (mShouldClose) {
        AutoPtr<IIoUtils> mIu;
        CIoUtils::AcquireSingleton((IIoUtils**)&mIu);
        return mIu->Close(mFd);
    }
    else {
        // An owned fd has been invalidated by IoUtils.close, but
        // we need to explicitly stop using an unowned fd (http://b/4361076).
        //CFileDescriptor::NewByFriend((CFileDescriptor**)&mFd);
        mFd = NULL;
        return CFileDescriptor::New((IFileDescriptor**)&mFd);
    }
}

ECode ParcelFileDescriptor::AutoCloseInputStream::GetFD(
    /* [out] */ IFileDescriptor** fd)
{
    VALIDATE_NOT_NULL(fd);
    *fd = mFd;
    INTERFACE_ADDREF(*fd);
    return NOERROR;
}

ECode ParcelFileDescriptor::AutoCloseInputStream::Available(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    Int32 fd;
    mFd->GetDescriptor(&fd);
    AutoPtr<IIoBridge> ioBridge;
    CIoBridge::AcquireSingleton((IIoBridge**)&ioBridge);
    return Libcore2IoECode(ioBridge->Available(fd, number));
}

ECode ParcelFileDescriptor::AutoCloseInputStream::Mark(
    /* [in] */ Int32 readLimit)
{
    return NOERROR;
}

ECode ParcelFileDescriptor::AutoCloseInputStream::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    VALIDATE_NOT_NULL(supported);
    *supported = FALSE;
    return NOERROR;
}

ECode ParcelFileDescriptor::AutoCloseInputStream::Read(
    /* [out] */ Int32* value)
{
    ArrayOf_<Byte, 1> readed;
    Int32 result;
    FAIL_RETURN(ReadBytesEx(&readed, 0, 1, &result));
    *value = result != -1 ? readed[0] & 0xff : -1;
    return NOERROR;;
}

ECode ParcelFileDescriptor::AutoCloseInputStream::ReadBytes(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    return ReadBytesEx(buffer, 0, buffer->GetLength(), number);
}

ECode ParcelFileDescriptor::AutoCloseInputStream::ReadBytesEx(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ Int32* number)
{
    Int32 fd;
    mFd->GetDescriptor(&fd);
    AutoPtr<IIoBridge> ioBridge;
    CIoBridge::AcquireSingleton((IIoBridge**)&ioBridge);
    return Libcore2IoECode(ioBridge->Read(fd,
            buffer, offset, length, number));
}

ECode ParcelFileDescriptor::AutoCloseInputStream::Reset()
{
    return E_IO_EXCEPTION;
}

ECode ParcelFileDescriptor::AutoCloseInputStream::Skip(
    /* [in] */ Int64 byteCount,
    /* [out] */ Int64* number)
{
    VALIDATE_NOT_NULL(number);
    *number = 0;

    if (byteCount < 0) {
        Slogger::E(TAG, "byteCount < 0: %lld", byteCount);
        return E_IO_EXCEPTION;
    }
    // Try lseek(2). That returns the new offset, but we'll throw an
    // exception if it couldn't perform exactly the seek we asked for.
    Int32 fd;
    mFd->GetDescriptor(&fd);
    AutoPtr<IOsConstants> osConstans;
    COsConstants::AcquireSingleton((IOsConstants**)&osConstans);
    Int32 mode;
    osConstans->GetOsConstant(String("SEEK_CUR"), &mode);
    AutoPtr<ILibcore> libcore;
    CLibcore::AcquireSingleton((ILibcore**)&libcore);
    AutoPtr<IOs> os;
    libcore->GetOs((IOs**)&os);
    return Libcore2IoECode(os->Lseek(fd, byteCount, mode, number));
    // } catch (ErrnoException errnoException) {
    //     if (errnoException.errno == ESPIPE) {
    //         // You can't seek on a pipe, so fall back to the superclass' implementation.
    //         return super.skip(byteCount);
    //     }
    //     throw errnoException.rethrowAsIOException();
    // }
}

AutoPtr<IInterface> ParcelFileDescriptor::AutoCloseInputStream::GetLock()
{
    AutoPtr<IInterface> obj = (IInterface*)mLock.Get();
    return obj;
}

ParcelFileDescriptor::AutoCloseOutputStream::AutoCloseOutputStream()
    : mShouldClose(FALSE)
{
    mLock = new LockObject();
}

ECode ParcelFileDescriptor::AutoCloseOutputStream::Init(
    /* [in] */ IParcelFileDescriptor* pfd)
{
    AutoPtr<IFileDescriptor> fd;
    pfd->GetFileDescriptor((IFileDescriptor**)&fd);
    if (fd == NULL) {
        //throw new NullPointerException("fd == null");
        return E_NULL_POINTER_EXCEPTION;
    }
    mFd = fd;
    mShouldClose = FALSE;
    AutoPtr<IOsConstants> osConstans;
    COsConstants::AcquireSingleton((IOsConstants**)&osConstans);
    osConstans->GetOsConstant(String("O_WRONLY"), &mMode);
    // this.channel = NioUtils.newFileChannel(this, fd, mode);

    mPfd = pfd;
    return NOERROR;
}

ECode ParcelFileDescriptor::AutoCloseOutputStream::Close()
{
    //try {
    mPfd->Close();
    //} finally {
    //super.close();
    Object::Autolock lock(mLock);

    // if (channel != null) {
    //     channel.close();
    // }
    if (mShouldClose) {
        // try {
        AutoPtr<IIoUtils> mIu;
        CIoUtils::AcquireSingleton((IIoUtils**)&mIu);
        ECode ec = mIu->Close(mFd);
        if (SUCCEEDED(ec)) {
            mFd = NULL;
        }
        return ec;
        // } catch (ErrnoException errnoException) {
        //     throw errnoException.rethrowAsIOException();
        // }
    }
    else {
        // An owned fd has been invalidated by IoUtils.close, but
        // we need to explicitly stop using an unowned fd (http://b/4361076).
        mFd = NULL;
        //CFileDescriptor::NewByFriend((CFileDescriptor**)&mFd);
        return CFileDescriptor::New((IFileDescriptor**)&mFd);
    }
}

ECode ParcelFileDescriptor::AutoCloseOutputStream::Flush()
{
    return mFd->Sync();
}

ECode ParcelFileDescriptor::AutoCloseOutputStream::GetFD(
    /* [out] */ IFileDescriptor** fd)
{
    VALIDATE_NOT_NULL(fd);
    *fd = mFd;
    INTERFACE_ADDREF(*fd);
    return NOERROR;
}

ECode ParcelFileDescriptor::AutoCloseOutputStream::Write(
    /* [in] */ Int32 oneByte)
{
    ArrayOf_<Byte, 1> bytes;
    (bytes)[0] = (Byte)oneByte;
    return WriteBytesEx(bytes, 0, 1);
}

ECode ParcelFileDescriptor::AutoCloseOutputStream::WriteBytes(
    /* [in] */ const ArrayOf<Byte>& buffer)
{
    return WriteBytesEx(buffer, 0, buffer.GetLength());
}

ECode ParcelFileDescriptor::AutoCloseOutputStream::WriteBytesEx(
    /* [in] */ const ArrayOf<Byte>& buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count)
{
    Int32 fd;
    mFd->GetDescriptor(&fd);
    AutoPtr<IIoBridge> ioBridge;
    CIoBridge::AcquireSingleton((IIoBridge**)&ioBridge);
    return Libcore2IoECode(ioBridge->Write(fd,
            buffer, offset, count));
}

ECode ParcelFileDescriptor::AutoCloseOutputStream::CheckError(
    /* [out] */ Boolean* hasError)
{
    *hasError = FALSE;
    return NOERROR;
}

AutoPtr<IInterface> ParcelFileDescriptor::AutoCloseOutputStream::GetLock()
{
    AutoPtr<IInterface> obj = (IInterface*)mLock.Get();
    return obj;
}


ParcelFileDescriptor::ParcelFileDescriptor()
    : mClosed(FALSE)
{}

ParcelFileDescriptor::~ParcelFileDescriptor()
{
    if (!mClosed) {
        Close();
    }
}

ParcelFileDescriptor::ParcelFileDescriptor(
    /* [in] */ IParcelFileDescriptor* descriptor)
{
    Init(descriptor);
}

ParcelFileDescriptor::ParcelFileDescriptor(
    /* [in] */ IFileDescriptor* descriptor)
{
    Init(descriptor);
}

ECode ParcelFileDescriptor::Init(
    /* [in] */ IParcelFileDescriptor* descriptor)
{
    mWrapped = descriptor;
    assert(mWrapped != NULL);
    descriptor->GetFileDescriptor((IFileDescriptor**)&mFileDescriptor);
    return NOERROR;
}

ECode ParcelFileDescriptor::Init(
    /* [in] */ IFileDescriptor* descriptor)
{
    if (descriptor == NULL) {
        //throw new NullPointerException("descriptor must not be null");
        return E_NULL_POINTER_EXCEPTION;
    }
    mFileDescriptor = descriptor;
    mWrapped = NULL;
    return NOERROR;
}

ECode ParcelFileDescriptor::Open(
    /* [in] */ IFile* file,
    /* [in] */ Int32 mode,
    /* [out] */ IParcelFileDescriptor** descriptor)
{
    VALIDATE_NOT_NULL(descriptor);
    *descriptor = NULL;

    assert(file != NULL);
    String path;
    file->GetPath(&path);

    //TODO
    // SecurityManager security = System.getSecurityManager();
    // if (security != NULL) {
    //     security.checkRead(path);
    //     if ((mode&MODE_WRITE_ONLY) != 0) {
    //         security.checkWrite(path);
    //     }
    // }

    if ((mode & IParcelFileDescriptor::MODE_READ_WRITE) == 0) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoPtr<IFileDescriptor> fd;
    CParcel::OpenFileDescriptor(path, mode, (IFileDescriptor**)&fd);
    if (fd == NULL) {
        return NOERROR;
    }
    return CParcelFileDescriptor::New(fd, descriptor);
}

ECode ParcelFileDescriptor::AdoptFd(
    /* [in] */ Int32 fd,
    /* [out] */ IParcelFileDescriptor** descriptor)
{
    VALIDATE_NOT_NULL(descriptor);
    AutoPtr<IFileDescriptor> fdesc = GetFileDescriptorFromFdNoDup(fd);
    return CParcelFileDescriptor::New(fdesc, descriptor);
}

ECode ParcelFileDescriptor::CreatePipe(
    /* [out, callee] */ ArrayOf<IParcelFileDescriptor*>** _pfds)
{
    VALIDATE_NOT_NULL(_pfds)
    *_pfds = NULL;
    AutoPtr< ArrayOf<IFileDescriptor*> > fds = ArrayOf<IFileDescriptor*>::Alloc(2);
    FAIL_RETURN(CreatePipeNative(fds))
    AutoPtr< ArrayOf<IParcelFileDescriptor*> > pfds = ArrayOf<IParcelFileDescriptor*>::Alloc(2);
    AutoPtr<IParcelFileDescriptor> pfd1, pfd2;
    CParcelFileDescriptor::New((*fds)[0], (IParcelFileDescriptor**)&pfd1);
    pfds->Set(0, pfd1);
    CParcelFileDescriptor::New((*fds)[1], (IParcelFileDescriptor**)&pfd2);
    pfds->Set(1, pfd2);
    *_pfds = pfds;
    ARRAYOF_ADDREF(*_pfds)
    return NOERROR;
}

ECode ParcelFileDescriptor::Dup(
    /* [in] */ IFileDescriptor* fd,
    /* [out] */ IParcelFileDescriptor** descriptor)
{
    VALIDATE_NOT_NULL(descriptor);

    AutoPtr<IFileDescriptor> pFd;
    CParcel::DupFileDescriptor(fd, (IFileDescriptor**)&pFd);
    if (pFd == NULL) {
        *descriptor = NULL;
        return NOERROR;
    }
    return CParcelFileDescriptor::New(pFd, descriptor);
}

ECode ParcelFileDescriptor::CreatePipeNative(
    /* [in] */ ArrayOf<IFileDescriptor*>* outFds)
{
    int fds[2];
    if (pipe(fds) < 0) {
        // int therr = errno;
        return E_IO_EXCEPTION;
    }

    for (Int32 i = 0; i < 2; i++) {
        AutoPtr<IFileDescriptor> fd;
        CFileDescriptor::New((IFileDescriptor**)&fd);
        fd->SetDescriptor(fds[i]);
        outFds->Set(i, fd);
    }
    return NOERROR;
}

AutoPtr<IFileDescriptor> ParcelFileDescriptor::GetFileDescriptorFromFdNoDup(
    /* [in] */ Int32 fd)
{
    AutoPtr<IFileDescriptor> fdesc;
    CFileDescriptor::New((IFileDescriptor**)&fdesc);
    fdesc->SetDescriptor(fd);
    return fdesc;
}


ECode ParcelFileDescriptor::Close()
{
    if (mClosed) return NOERROR;
    mClosed = TRUE;
    // mGuard.close();

    if (mWrapped != NULL) {
        // If this is a proxy to another file descriptor, just call through to its
        // close method.
        mWrapped->Close();
    } else {
        CParcel::CloseFileDescriptor(mFileDescriptor);
    }
    return NOERROR;
}

ECode ParcelFileDescriptor::GetFileDescriptor(
    /* [out] */ IFileDescriptor** des)
{
    VALIDATE_NOT_NULL(des);
    *des = mFileDescriptor.Get();
    INTERFACE_ADDREF(*des);
    return NOERROR;
}

ECode ParcelFileDescriptor::GetStatSize(
    /* [out] */ Int64* len)
{
    VALIDATE_NOT_NULL(len);
    Int32 fd = NativeGetFd();
    if (fd < 0) {
        *len = -1;
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    struct stat st;
    if (fstat(fd, &st) != 0) {
        *len = -1;
        return NOERROR;
    }

    if (S_ISREG(st.st_mode) || S_ISLNK(st.st_mode)) {
        *len = st.st_size;
        return NOERROR;
    }

    *len = -1;
    return NOERROR;
}

ECode ParcelFileDescriptor::SeekTo(
    /* [in] */ Int64 startOffset,
    /* [out] */ Int64* toOffset)
{
    VALIDATE_NOT_NULL(toOffset);
    Int32 fd = NativeGetFd();
    if (fd < 0) {
        *toOffset = -1;
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    *toOffset = lseek(fd, startOffset, SEEK_SET);
    return NOERROR;
}

ECode ParcelFileDescriptor::GetFd(
    /* [out] */ Int32* fd)
{
    if (mClosed) {
        // throw new IllegalStateException("Already closed");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    *fd = NativeGetFd();
    return NOERROR;
}

Int32 ParcelFileDescriptor::NativeGetFd()
{
    if (mFileDescriptor == NULL) return -1;
    return CParcel::GetFDFromFileDescriptor(mFileDescriptor);
}

ECode ParcelFileDescriptor::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    Int32 fd;
    FAIL_RETURN(source->ReadFileDescriptor(&fd));
    fd = dup(fd);
    if (fd < 0) return E_IO_EXCEPTION;
    if (mFileDescriptor == NULL) {
        CFileDescriptor::New((IFileDescriptor**)&mFileDescriptor);
    }
    mFileDescriptor->SetDescriptor(fd);
    return NOERROR;
}

ECode ParcelFileDescriptor::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    Int32 fd = -1;
    if (mFileDescriptor != NULL) {
        mFileDescriptor->GetDescriptor(&fd);
    }
    ECode ec = dest->WriteDupFileDescriptor(fd);
    if (/*(flags & PARCELABLE_WRITE_RETURN_VALUE) != 0 &&*/ !mClosed) {
        // try {
            Close();
        // } catch (IOException e) {
        //     // Empty
        // }
    }

    return ec;
}

} // namespace Os
} // namespace Droid
} // namespace Elastos
