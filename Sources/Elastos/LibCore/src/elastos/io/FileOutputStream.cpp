
#include "coredef.h"
#include "FileOutputStream.h"
#include "CFile.h"
#include "IoUtils.h"
#include "COsConstants.h"
#include "CLibcore.h"
#include "CIoBridge.h"

using Elastos::Droid::System::IOsConstants;
using Libcore::IO::COsConstants;
using Libcore::IO::ILibcore;
using Libcore::IO::CLibcore;
using Libcore::IO::IOs;
using Libcore::IO::IIoBridge;
using Libcore::IO::CIoBridge;

namespace Elastos {
namespace IO {

FileOutputStream::FileOutputStream()
{}

FileOutputStream::~FileOutputStream()
{
    // try {
    CloseInner();
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

ECode FileOutputStream::Init(
    /* [in] */ IFile* file)
{
    return Init(file, FALSE);
}

ECode FileOutputStream::Init(
    /* [in] */ IFile* file,
    /* [in] */ Boolean append)
{
    if (file == NULL) {
        // throw new NullPointerException("file == null");
        return E_NULL_POINTER_EXCEPTION;
    }
    AutoPtr<COsConstants> osConstans;
    COsConstants::AcquireSingletonByFriend((COsConstants**)&osConstans);
    Int32 m1, m2, m3, m4;
    osConstans->GetOsConstant(String("O_WRONLY"), &m1);
    osConstans->GetOsConstant(String("O_CREAT"), &m2);
    osConstans->GetOsConstant(String("O_APPEND"), &m3);
    osConstans->GetOsConstant(String("O_TRUNC"), &m4);
    mMode = m1 | m2 | (append ? m3 : m4);
    String path;
    file->GetAbsolutePath(&path);
    CFileDescriptor::NewByFriend((CFileDescriptor**)&mFd);
    AutoPtr<CIoBridge> ioBridge;
    CIoBridge::AcquireSingletonByFriend((CIoBridge**)&ioBridge);
    Int32 fd;
    FAIL_RETURN(IoUtils::Libcore2IoECode(ioBridge->Open(path, mMode, &fd)));
    mFd->SetDescriptor(fd);
    mShouldClose = TRUE;
    return NOERROR;
}

ECode FileOutputStream::Init(
    /* [in] */ IFileDescriptor* fd)
{
    if (fd == NULL) {
//        throw new NullPointerException("fd == null");
        return E_NULL_POINTER_EXCEPTION;
    }
    mFd = (CFileDescriptor*)fd;
    mShouldClose = FALSE;
    AutoPtr<COsConstants> osConstans;
    COsConstants::AcquireSingletonByFriend((COsConstants**)&osConstans);
    osConstans->GetOsConstant(String("O_WRONLY"), &mMode);
    // this.channel = NioUtils.newFileChannel(this, fd, mode);
    return NOERROR;
}

ECode FileOutputStream::Init(
    /* [in] */ const String& fileName)
{
    return Init(fileName, FALSE);
}

ECode FileOutputStream::Init(
    /* [in] */ const String& fileName,
    /* [in] */ Boolean append)
{
    AutoPtr<CFile> file;
    CFile::NewByFriend(fileName, (CFile**)&file);
    return Init((IFile*)file.Get(), append);
}

ECode FileOutputStream::Close()
{
    return CloseInner();
}

ECode FileOutputStream::GetChannel(
    /* [out] */ IFileChannel** channel)
{
    VALIDATE_NOT_NULL(channel)

    Mutex::Autolock lock(mLockInner);

    // if (channel == null) {
    //     channel = NioUtils.newFileChannel(this, fd, mode);
    // }
    *channel = mChannel;
    REFCOUNT_ADD(*channel);
    return NOERROR;
}

ECode FileOutputStream::GetFD(
    /* [out] */ IFileDescriptor** fd)
{
    VALIDATE_NOT_NULL(fd)

    *fd = mFd;
    REFCOUNT_ADD(*fd);
    return NOERROR;
}

ECode FileOutputStream::Flush()
{
    return mFd->Sync();
}

ECode FileOutputStream::Write(
    /* [in] */ Int32 oneByte)
{
    ArrayOf_<Byte, 1> bytes;
    (bytes)[0] = (Byte)oneByte;
    return WriteBytes(bytes, 0, 1);
}

ECode FileOutputStream::WriteBytes(
    /* [in] */ const ArrayOf<Byte>& buffer,
    /* [in] */ Int32 byteOffset,
    /* [in] */ Int32 byteCount)
{
    Int32 fd;
    mFd->GetDescriptor(&fd);
    AutoPtr<IIoBridge> ioBridge;
    CIoBridge::AcquireSingleton((IIoBridge**)&ioBridge);
    return IoUtils::Libcore2IoECode(ioBridge->Write(fd,
            buffer, byteOffset, byteCount));
}

ECode FileOutputStream::CloseInner()
{
    Object::Autolock lock(mLock);

    if (mShouldClose) {
        return IoUtils::Close(mFd);
    }
    else {
        // An owned fd has been invalidated by IoUtils.close, but
        // we need to explicitly stop using an unowned fd (http://b/4361076).
        mFd = NULL;
        return CFileDescriptor::NewByFriend((CFileDescriptor**)&mFd);
    }
}

} // namespace IO
} // namespace Elastos
