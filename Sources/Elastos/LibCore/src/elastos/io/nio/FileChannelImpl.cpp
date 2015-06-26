#include "FileChannelImpl.h"
#include "Math.h"
#include "MemoryBlock.h"
#include "MappedByteBufferAdapter.h"
#include "NioUtils.h"
// #include "CLibcore.h"
#include "OsConstants.h"

using Libcore::IO::IOs;
using Libcore::IO::ILibcore;
// using Libcore::IO::CLibcore;
using Elastos::IO::NioUtils;
using Elastos::Droid::System::OsConstants;
using Elastos::Droid::System::IStructStat;
using Elastos::IO::MemoryBlock;
using Elastos::IO::Channels::EIID_IFileLock;
using Elastos::IO::Channels::IChannel;
using Elastos::IO::Channels::EIID_IFileChannel;
using Elastos::IO::MappedByteBufferAdapter;
using Elastos::Droid::System::IStructStat;

namespace Elastos {
namespace IO {

//==========================================================
//       FileChannelImpl::FileLockImpl
//==========================================================
FileChannelImpl::FileLockImpl::FileLockImpl(
    /* [in] */ IFileChannel* channel,
    /* [in] */ Int64 position,
    /* [in] */ Int64 size,
    /* [in] */ Boolean shared)
    : FileLock(channel, position, size, shared)
    , mIsReleased(FALSE)
{
}

CAR_INTERFACE_IMPL(FileChannelImpl::FileLockImpl, Object, IFileLock)

ECode FileChannelImpl::FileLockImpl::Channel(
    /* [out] */ IFileChannel** channel)
{
    return FileLock::Channel(channel);
}

ECode FileChannelImpl::FileLockImpl::GetPosition(
    /* [out] */ Int64* position)
{
    return FileLock::GetPosition(position);
}

ECode FileChannelImpl::FileLockImpl::GetSize(
    /* [out] */ Int64* size)
{
    return FileLock::GetSize(size);
}

ECode FileChannelImpl::FileLockImpl::IsShared(
    /* [out] */ Boolean* shared)
{
    return FileLock::IsShared(shared);
}

ECode FileChannelImpl::FileLockImpl::Overlaps(
    /* [in] */ Int64 start,
    /* [in] */ Int64 length,
    /* [out] */ Boolean* result)
{
    return FileLock::Overlaps(start, length, result);
}

ECode FileChannelImpl::FileLockImpl::ToString(
    /* [out] */ String* string)
{
    return FileLock::ToString(string);
}

ECode FileChannelImpl::FileLockImpl::IsValid(
    /* [out] */ Boolean* ret)
{
    ECode ecRet;
    AutoPtr<IFileChannel> channel;
    ecRet = Channel((IFileChannel**)&channel);
    if (NOERROR != ecRet)
        return ecRet;

    Boolean isOpen;
    ecRet = IChannel::Probe(channel)->IsOpen(&isOpen);
    if (NOERROR != ecRet)
        return ecRet;

    *ret = mIsReleased && isOpen;
    return NOERROR;
}

ECode FileChannelImpl::FileLockImpl::ReleaseLock()
{
    ECode ecRet;
    AutoPtr<IFileChannel> channel;
    ecRet = Channel((IFileChannel**)&channel);
    if (NOERROR != ecRet)
        return ecRet;

    Boolean isOpen;
    ecRet = IChannel::Probe(channel)->IsOpen(&isOpen);
    if (NOERROR != ecRet)
        return ecRet;

    if (!isOpen) {
        return E_CLOSED_CHANNEL_EXCEPTION;
    }

    if (!mIsReleased) {
        IFileChannelImpl* impl = IFileChannelImpl::Probe(channel);
        impl->ReleaseLock(this);
        mIsReleased = TRUE;
    }
    return NOERROR;
}

//==========================================================
//       FileChannelImpl
//==========================================================

CAR_INTERFACE_IMPL(FileChannelImpl, Object, IFileChannelImpl)

FileChannelImpl::FileChannelImpl(
    /* [in] */ ICloseable* stream,
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Int32 mode)
    : mStream(stream)
    , mFd(fd)
    , mMode(mode)
{
    assert(NULL != stream);
    assert(NULL != fd);
    assert(0 && "TODO");
    // AutoPtr<COsConstants> obj;
    // COsConstants::AcquireSingletonByFriend((COsConstants**)&obj);
    // mOsConstants = (IOsConstants*)obj.Get();
}

ECode FileChannelImpl::IsOpen(
    /* [out] */ Boolean* rst)
{
    return FileChannel::IsOpen(rst);
}

ECode FileChannelImpl::Close()
{
    return FileChannel::Close();
}

ECode FileChannelImpl::Lock(
    /* [out] */ IFileLock** lock)
{
    return FileChannel::Lock(lock);
}

ECode FileChannelImpl::TryLock(
    /* [out] */ IFileLock** lock)
{
    return FileChannel::TryLock(lock);
}

ECode FileChannelImpl::Read(
    /* [in] */ ArrayOf<IByteBuffer*>* buffers,
    /* [out] */ Int64* number)
{
    return FileChannel::Read(buffers , number);
}

ECode FileChannelImpl::Write(
    /* [in] */ ArrayOf<IByteBuffer*>* buffers,
    /* [out] */ Int64* number)
{
    return FileChannel::Write(buffers , number);
}

ECode FileChannelImpl::CheckOpen()
{
    Boolean bOpenFlag;
    FileChannel::IsOpen(&bOpenFlag);
    if (!bOpenFlag)
    {
        return E_CLOSED_CHANNEL_EXCEPTION;
    }
    return NOERROR;
}

ECode FileChannelImpl::CheckReadable()
{
    Int32 accMode = 0;
    Int32 wrOnly = 0;
    assert(0 && "TODO");
    // mOsConstants->GetOsConstant(String("O_ACCMODE"), &accMode);
    // mOsConstants->GetOsConstant(String("O_WRONLY"), &wrOnly);

    if ((mMode & accMode) == wrOnly)
    {
        return E_NON_READABLE_CHANNEL_EXCEPTION;
    }
    return NOERROR;
}

ECode FileChannelImpl::CheckWritable()
{
    Int32 accMode = 0;
    Int32 rdOnly = 0;
    assert(0 && "TODO");
    // mOsConstants->GetOsConstant(String("O_ACCMODE"), &accMode);
    // mOsConstants->GetOsConstant(String("O_RDONLY"), &rdOnly);
    if ((mMode & accMode) == rdOnly)
    {
        return E_NON_WRITABLE_CHANNEL_EXCEPTION;
    }
    return NOERROR;
}

ECode FileChannelImpl::ImplCloseChannel()
{
    if (ICloseable::Probe(mStream) != NULL)
    {
        return ((ICloseable*) mStream.Get())->Close();
    }
    return NOERROR;
}

ECode FileChannelImpl::BasicLock(
    /* [in] */ Int64 position,
    /* [in] */ Int64 size,
    /* [in] */ Boolean shared,
    /* [in] */ Boolean wait)
{
    Int32 ACCMOD = 0;
    Int32 RDONLY = 0;
    Int32 WRONLY = 0;
    assert(0 && "TODO");
    // mOsConstants->GetOsConstant(String("O_ACCMODE"), &ACCMOD);
    // mOsConstants->GetOsConstant(String("O_RDONLY"), &RDONLY);
    // mOsConstants->GetOsConstant(String("O_WRONLY"), &WRONLY);
    Int32 accessMode = mMode & ACCMOD;
    if (RDONLY == accessMode)
    {
        if (!shared)
        {
            return E_NON_WRITABLE_CHANNEL_EXCEPTION;
        }

    }
    else if (WRONLY == accessMode)
    {
        if (shared)
        {
            return E_NON_WRITABLE_CHANNEL_EXCEPTION;
        }
    }

    if(position < 0 || size < 0)
    {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    assert(0 && "TODO");
    // FileLock pendingLock = new FileLockImpl(this, position, size, shared);
    // addLock(pendingLock);

    // StructFlock flock = new StructFlock();
    // flock.l_type = (short) (shared ? F_RDLCK : F_WRLCK);
    // flock.l_whence = (short) SEEK_SET;
    // flock.l_start = position;
    // flock.l_len = translateLockLength(size);

    // boolean success = false;
    // try {
    //     success = (Libcore.os.fcntlFlock(fd, wait ? F_SETLKW64 : F_SETLK64, flock) != -1);
    // } catch (ErrnoException errnoException) {
    //     throw errnoException.rethrowAsIOException();
    // } finally {
    //     if (!success) {
    //         removeLock(pendingLock);
    //     }
    // }
    // return success ? pendingLock : null;
    return NOERROR;
}

Int64 FileChannelImpl::TranslateLockLength(
    /* [in] */ Int64 byteCount)
{
    // FileChannel uses Long.MAX_VALUE to mean "lock the whole file" where POSIX uses 0.
    return (byteCount == Elastos::Core::Math::INT64_MAX_VALUE) ? 0 : byteCount;
}

ECode FileChannelImpl::Lock(
    /* [in] */ Int64 pos,
    /* [in] */ Int64 size,
    /* [in] */ Boolean shared,
    /* [in] */ IFileLock** lock)
{
    assert(0 && "TODO");
    // checkOpen();
    // FileLock resultLock = null;
    // {
    //     boolean completed = false;
    //     try {
    //         begin();
    //         resultLock = basicLock(position, size, shared, true);
    //         completed = true;
    //     } finally {
    //         try {
    //             end(completed);
    //         } catch (ClosedByInterruptException e) {
    //             throw new FileLockInterruptionException();
    //         }
    //     }
    // }
    // return resultLock;
    return E_NOT_IMPLEMENTED;
}

ECode FileChannelImpl::TryLock(
    /* [in] */ Int64 position,
    /* [in] */ Int64 size,
    /* [in] */ Boolean shared,
    /* [out] */ IFileLock** lock)
{
    assert(0 && "TODO");
    // checkOpen();
    // return basicLock(position, size, shared, false);
    return E_NOT_IMPLEMENTED;
}

ECode FileChannelImpl::ReleaseLock(
    /* [in] */ IFileLock* lock)
{
    assert(0 && "TODO");
    // checkOpen();

    // StructFlock flock = new StructFlock();
    // flock.l_type = (short) F_UNLCK;
    // flock.l_whence = (short) SEEK_SET;
    // flock.l_start = lock.position();
    // flock.l_len = translateLockLength(lock.size());
    // try {
    //     Libcore.os.fcntlFlock(fd, F_SETLKW64, flock);
    // } catch (ErrnoException errnoException) {
    //     throw errnoException.rethrowAsIOException();
    // }

    // removeLock(lock);
    return E_NOT_IMPLEMENTED;
}

ECode FileChannelImpl::Force(
    /* [in] */ Boolean metadata)
{
    ECode ecRet = CheckOpen();
    if(NOERROR != ecRet)
       return ecRet;

    Int32 ACCMODE = 0;
    Int32 RDONLY = 0;
    assert(0 && "TODO");
    // mOsConstants->GetOsConstant(String("O_ACCMODE"), &ACCMODE);
    // mOsConstants->GetOsConstant(String("O_RDONLY"), &RDONLY);

    if( (mMode & ACCMODE) != RDONLY)
    {
        AutoPtr<ILibcore> libcore;
        assert(0 && "TODO");
        // CLibcore::AcquireSingleton((ILibcore**)&libcore);
        AutoPtr<IOs> os;
        libcore->GetOs((IOs**)&os);
        if (metadata)
        {
            ecRet = os->Fsync(mFd);
            if (NOERROR != ecRet)
            {
                return ecRet;
            }
        }
        else
        {
            ecRet = os->Fdatasync(mFd);
            if (NOERROR != ecRet)
            {
                return ecRet;
            }
        }
    }

    return NOERROR;
}

ECode FileChannelImpl::Map(
    /* [in] */ FileChannelMapMode mode,
    /* [in] */ Int64 position,
    /* [in] */ Int64 size,
    /* [out] */ IMappedByteBuffer** buffer)
{
    ECode ecRet;
    ecRet = CheckOpen();

    assert(mode >= Elastos::IO::Channels::FileChannelMapMode_READ_ONLY &&
          mode <= Elastos::IO::Channels::FileChannelMapMode_PRIVATE);

    if(position < 0 || size < 0 || size > Elastos::Core::Math::INT32_MAX_VALUE)
    {
       return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Int32 ACCESSMODE = 0;
    Int32 RDONLY = 0;
    Int32 WRONLY = 0;
    assert(0 && "TODO");
    // mOsConstants->GetOsConstant(String("O_ACCMODE"), &ACCESSMODE);
    // mOsConstants->GetOsConstant(String("O_RDONLY"), &RDONLY);
    // mOsConstants->GetOsConstant(String("O_WRONLY"), &WRONLY);
    Int32 accessMode = (mode & ACCESSMODE);
    if(accessMode == RDONLY)
    {
       if(mode != Elastos::IO::Channels::FileChannelMapMode_READ_ONLY)
       {
           return E_NON_WRITABLE_CHANNEL_EXCEPTION;
       }
    } else if(accessMode == WRONLY) {
       return E_NON_READABLE_CHANNEL_EXCEPTION;
    }

    assert(0 && "TODO");
    // AutoPtr<CLibcore> obj;
    // CLibcore::AcquireSingletonByFriend((CLibcore**)&obj);
    AutoPtr<ILibcore> libcore; // = (ILibcore*)obj.Get();
    AutoPtr<IOs> os;
    libcore->GetOs((IOs**)&os);

    Int64 nSize;
    ecRet = GetSize(&nSize);

    if(position + size > nSize)
    {
        ecRet = os->Ftruncate(mFd, position + size);
        if(NOERROR != ecRet)
           return ecRet;
    }

    Int32 SC_PAGE_SIZE = 0;
    Int64 nPageSize = 0;

    assert(0 && "TODO");
    // ecRet = mOsConstants->GetOsConstant(String("_SC_PAGE_SIZE"), &SC_PAGE_SIZE);
    assert(NOERROR == ecRet);

    ecRet = os->Sysconf(SC_PAGE_SIZE, &nPageSize);
    assert(NOERROR == ecRet);

    Int64 alignment = position - position % nPageSize;
    Int32 offset = (Int32)(position - alignment);

    AutoPtr<MemoryBlock> block;
    MemoryBlock::Mmap(mFd.Get(), alignment, size + offset, mode, (MemoryBlock**)&block);

    AutoPtr<IMappedByteBuffer> mbb = new MappedByteBufferAdapter(block, (Int32)size, offset, mode);
    *buffer = mbb;
    REFCOUNT_ADD(*buffer);

    return NOERROR;
}

ECode FileChannelImpl::GetPosition(
    /* [out] */ Int64* position)
{
    ECode ecRet = CheckOpen();
    assert(0 && "TODO");
    // AutoPtr<CLibcore> obj;
    // CLibcore::AcquireSingletonByFriend((CLibcore**)&obj);
    AutoPtr<ILibcore> libcore; // = (ILibcore*)obj.Get();
    AutoPtr<IOs> os;
    libcore->GetOs((IOs**)&os);

    Int32 seek_cur = 0;
    assert(0 && "TODO");
    // ecRet = mOsConstants->GetOsConstant(String("SEEK_CUR"), &seek_cur);
    assert(NOERROR == ecRet);

    return os->Lseek(mFd, 0L, seek_cur, position);
}

ECode FileChannelImpl::SetPosition(
    /* [in] */ Int64 newPosition)
{
    ECode ecRet;
    ecRet = CheckOpen();
    if(newPosition < 0)
        return E_ILLEGAL_ARGUMENT_EXCEPTION;

    assert(0 && "TODO");
    // AutoPtr<CLibcore> obj;
    // CLibcore::AcquireSingletonByFriend((CLibcore**)&obj);
    AutoPtr<ILibcore> libcore; // = (ILibcore*)obj.Get();
    AutoPtr<IOs> os;
    libcore->GetOs((IOs**)&os);

    Int32 seek_cur = 0;
    Int64 result = 0;
    assert(0 && "TODO");
    // ecRet = mOsConstants->GetOsConstant(String("SEEK_CUR"), &seek_cur);
    assert(NOERROR == ecRet);
    ecRet = os->Lseek(mFd, newPosition, seek_cur, &result);
    assert(NOERROR == ecRet);
    return NOERROR;
}

ECode FileChannelImpl::Read(
   /* [in] */ IByteBuffer* buffer,
   /* [out] */ Int32* number)
{
    return ReadImpl(buffer, -1, number);
}

ECode FileChannelImpl::Read(
    /* [in] */ IByteBuffer* buffer,
    /* [in] */ Int64 position,
    /* [out] */ Int32* number)
{
    if(position < 0)
        return E_ILLEGAL_ARGUMENT_EXCEPTION;

    return ReadImpl(buffer, position, number);
}

ECode FileChannelImpl::ReadImpl(
    /* [in] */ IByteBuffer* buffer,
    /* [in] */ Int64 position,
    /* [out] */ Int32* ret)
{
    assert(0 && "TODO");
    // buffer.checkWritable();
    // checkOpen();
    // checkReadable();
    // if (!buffer.hasRemaining()) {
    //     return 0;
    // }
    // int bytesRead = 0;
    // boolean completed = false;
    // try {
    //     begin();
    //     try {
    //         if (position == -1) {
    //             bytesRead = Libcore.os.read(fd, buffer);
    //         } else {
    //             bytesRead = Libcore.os.pread(fd, buffer, position);
    //         }
    //         if (bytesRead == 0) {
    //             bytesRead = -1;
    //         }
    //     } catch (ErrnoException errnoException) {
    //         if (errnoException.errno == EAGAIN) {
    //             // We don't throw if we try to read from an empty non-blocking pipe.
    //             bytesRead = 0;
    //         } else {
    //             throw errnoException.rethrowAsIOException();
    //         }
    //     }
    //     completed = true;
    // } finally {
    //     end(completed && bytesRead >= 0);
    // }
    // if (bytesRead > 0) {
    //     buffer.position(buffer.position() + bytesRead);
    // }
    // return bytesRead;
    return E_NOT_IMPLEMENTED;

}

ECode FileChannelImpl::TransferIoVec(
    /* [in] */ IoVec* ioVec,
    /* [out] */ Int32* ret)
{
    VALIDATE_NOT_NULL(ret)

    if (ioVec->Init(ret), *ret == 0) {
        *ret = 0;
        return NOERROR;
    }
    Int32 bytesTransferred = 0;
    Boolean completed = FALSE;
    // try {
    Begin();
    ioVec->DoTransfer(mFd, &bytesTransferred);
    completed = TRUE;
    // } finally {
    End(completed);
    // }
    ioVec->DidTransfer(bytesTransferred);
    *ret = bytesTransferred;
    return NOERROR;
}

ECode FileChannelImpl::Read(
    /* [in] */ ArrayOf<IByteBuffer*>* buffers,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ Int64* number)
{

    if ((offset | length) < 0 || offset > buffers->GetLength()
            || buffers->GetLength() - offset < length)
    {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    ECode ecRet = CheckOpen();
    if(NOERROR != ecRet)
        return ecRet;

    ecRet = CheckReadable();
    if(NOERROR != ecRet)
        return ecRet;

    // return transferIoVec(new IoVec(buffers, offset, length, IoVec.Direction.READV));
    return E_NOT_IMPLEMENTED;
}

ECode FileChannelImpl::GetSize(
    /* [out] */ Int64* size)
{
    VALIDATE_NOT_NULL(size)

    ECode ecRet = CheckOpen();
    if(NOERROR != ecRet)
       return ecRet;

   assert(0 && "TODO");
    // AutoPtr<CLibcore> obj;
    // CLibcore::AcquireSingletonByFriend((CLibcore**)&obj);
    AutoPtr<ILibcore> libcore; // = (ILibcore*)obj.Get();
    AutoPtr<IOs> os;
    libcore->GetOs((IOs**)&os);

    AutoPtr<IStructStat> stat;
    os->Fstat(mFd, (IStructStat**)&stat);
    return stat->GetSize(size);
}

ECode FileChannelImpl::TransferFrom(
    /* [in] */ IReadableByteChannel* src,
    /* [in] */ Int64 position,
    /* [in] */ Int64 count,
    /* [out] */ Int64* number)
{
    VALIDATE_NOT_NULL(number)

    ECode ecRet;
    ecRet = CheckOpen();
    if(NOERROR != ecRet)
        return ecRet;

    Boolean isOpen;
    IChannel::Probe(src)->IsOpen(&isOpen);
    if(!isOpen)
        return E_CLOSED_CHANNEL_EXCEPTION;

    ecRet = CheckWritable();
    if(NOERROR != ecRet)
        return ecRet;

    if(position < 0 || count < 0 || count > Elastos::Core::Math::INT32_MAX_VALUE)
        return E_ILLEGAL_ARGUMENT_EXCEPTION;

    Int64 nSize;
    GetSize(&nSize);
    if(position > nSize)
    {
        *number = 0;
        return NOERROR;
    }

    if(IFileChannel::Probe(src) != NULL)
    {
        IFileChannel *fileSrc = (IFileChannel*)src;
        Int64 size;
        fileSrc->GetSize(&size);
        Int64 filePosition;
        fileSrc->GetPosition(&filePosition);
        count = Elastos::Core::Math::Min(count, size-filePosition);
        AutoPtr<IMappedByteBuffer> buffer;
        fileSrc->Map(Elastos::IO::Channels::FileChannelMapMode_READ_ONLY, filePosition, count, (IMappedByteBuffer**)&buffer);
        ecRet = fileSrc->SetPosition(filePosition + count);
        if(NOERROR != ecRet)
            return ecRet;

        ecRet = Write(IByteBuffer::Probe(buffer), position, (Int32*)number);
        assert(0 && "TODO");
        // NioUtils::FreeDirectBuffer((ByteBuffer*)buffer->Probe(EIID_ByteBuffer));
        return ecRet;
    }

    AutoPtr<IByteBuffer> buffer;
    ByteBuffer::Allocate((Int32)count, (IByteBuffer**)&buffer);
    Int32 nRead;
    src->Read(buffer, &nRead);
    IBuffer::Probe(buffer)->Flip();
    return Write(buffer, position, (Int32*)number);
}

ECode FileChannelImpl::TransferTo(
    /* [in] */ Int64 position,
    /* [in] */ Int64 count,
    /* [in] */ IWritableByteChannel* target,
    /* [out] */ Int64* number)
{
    VALIDATE_NOT_NULL(number)

    ECode ecRet = CheckOpen();
    if(NOERROR != ecRet)
        return ecRet;

    Boolean isOpen;
    IChannel::Probe(target)->IsOpen(&isOpen);
    if(!isOpen)
        return E_CLOSED_CHANNEL_EXCEPTION;

    ecRet = CheckReadable();
    if(NOERROR != ecRet)
        return ecRet;

    if(IFileChannelImpl::Probe(target) != NULL)
    {
        ecRet = ((FileChannelImpl*)target)->CheckWritable();
        if(NOERROR != ecRet)
            return ecRet;
    }

    if(position < 0 || count < 0)
    {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Int64 nSize;
    GetSize(&nSize);
    if(count == 0 || position >= nSize)
    {
        *number = 0;
        return NOERROR;
    }

    count = Elastos::Core::Math::Min(count, nSize-position);

    // Try sendfile(2) first...
    Boolean completed = FALSE;
    if (IFileDescriptorChannel::Probe(target)) {
        AutoPtr<IFileDescriptor> outFd;
        IFileDescriptorChannel::Probe(target)->GetFD((IFileDescriptor**)&outFd);
        Int32 outFdnum = 0;
        outFd->GetDescriptor(&outFdnum);
        // try {
        Begin();
            // try {
        assert(0 && "TODO");
        // FAIL_RETURN(CLibcore::sOs->Sendfile(outFdnum, mFd, &position, count, number));
        completed = TRUE;
        return NOERROR;
            // } catch (ErrnoException errnoException) {
            //     // If the OS doesn't support what we asked for, we want to fall through and
            //     // try a different approach. If it does support it, but it failed, we're done.
            //     if (errnoException.errno != ENOSYS && errnoException.errno != EINVAL) {
            //         throw errnoException.rethrowAsIOException();
            //     }
            // }
        // } finally {
        End(completed);
        // }
    }
    // ...fall back to write(2).
    AutoPtr<IMappedByteBuffer> buffer;
    // try {
    FAIL_RETURN(Map(Elastos::IO::Channels::FileChannelMapMode_READ_ONLY, position, count, (IMappedByteBuffer**)&buffer));
    Int32 ouvalue = 0;
    target->Write(IByteBuffer::Probe(buffer), &ouvalue);
    *number = ouvalue;
    return NOERROR;
    // } finally {
    assert(0 && "TODO");
    // NioUtils::FreeDirectBuffer((ByteBuffer*)buffer->Probe(EIID_ByteBuffer));
    // }
    return NOERROR;
}

ECode FileChannelImpl::Truncate(
    /* [in] */ Int64 size,
    /* [out]*/ IFileChannel** channel)
{
    VALIDATE_NOT_NULL(channel)

    ECode ecRet = CheckOpen();
    if(NOERROR != ecRet)
       return ecRet;

    if(size < 0)
       return E_ILLEGAL_ARGUMENT_EXCEPTION;

    ecRet = CheckWritable();
    if(NOERROR != ecRet)
       return ecRet;

    Int64 nSize;
    ecRet = GetSize(&nSize);
    assert(NOERROR == ecRet);

    if (size < nSize) {
        assert(0 && "TODO");
        // AutoPtr<CLibcore> obj;
        // CLibcore::AcquireSingletonByFriend((CLibcore**)&obj);
        AutoPtr<ILibcore> libcore; // = (ILibcore*)obj.Get();
        AutoPtr<IOs> os;
        libcore->GetOs((IOs**)&os);

        ecRet = os->Ftruncate(mFd, nSize);
    }

    *channel = (IFileChannel*) this->Probe(EIID_IFileChannel);
    REFCOUNT_ADD(*channel)

    return ecRet;
}

ECode FileChannelImpl::Write(
    /* [in] */ IByteBuffer* buffer,
    /* [out] */ Int32* number)
{
    return WriteImpl(buffer, -1, number);
}

ECode FileChannelImpl::Write(
    /* [in] */ IByteBuffer* buffer,
    /* [in] */ Int64 position,
    /* [out] */ Int32* number)
{
    if(position < 0)
    return E_ILLEGAL_ARGUMENT_EXCEPTION;

    return WriteImpl(buffer, position, number);
}

ECode FileChannelImpl::WriteImpl(
    /* [in] */ IByteBuffer* buffer,
    /* [in] */ Int64 position,
    /* [out] */ Int32* ret)
{
    VALIDATE_NOT_NULL(ret)

    ECode ecRet = CheckOpen();
    if(NOERROR != ecRet)
        return ecRet;

    ecRet = CheckWritable();
    if(NOERROR != ecRet)
        return ecRet;

    if(NULL == buffer)
        return E_NULL_POINTER_EXCEPTION;

    Boolean hasRemaining;
    IBuffer::Probe(buffer)->HasRemaining(&hasRemaining);
    if(!hasRemaining)
    {
        *ret = 0;
        return NOERROR;
    }

    Int32 bytesWritten = 0;
    Boolean completed = FALSE;

    Begin();
    {
        if (-1 == position) {
            assert(0 && "TODO");
            // ecRet = CLibcore::sOs->Write(mFd, buffer, &bytesWritten);
        }
        else {
            assert(0 && "TODO");
            // ecRet = CLibcore::sOs->Pwrite(mFd, buffer, position, &bytesWritten);
        }

        completed = TRUE;
    }
    End(completed);

    if(NOERROR != ecRet)
        return ecRet;

    if(bytesWritten > 0)
    {
        Int32 pos;
        IBuffer::Probe(buffer)->GetPosition(&pos);
        IBuffer::Probe(buffer)->SetPosition(pos + bytesWritten);
    }

    *ret = bytesWritten;
    return NOERROR;
}

ECode FileChannelImpl::Write(
    /* [in] */ ArrayOf<IByteBuffer*>* buffers,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ Int64* number)
{
    // Arrays.checkOffsetAndCount(buffers.length, offset, length);
    // checkOpen();
    // checkWritable();
    // return transferIoVec(new IoVec(buffers, offset, length, IoVec.Direction.WRITEV));
    return E_NOT_IMPLEMENTED;
}

ECode FileChannelImpl::calculateTotalRemaining(
    /* [in] */ ArrayOf<IByteBuffer*> buffers,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [in] */ Boolean copyingIn,
    /* [out] */ Int32* ret)
{
    VALIDATE_NOT_NULL(ret)

    Int32 count, i, remaining;
    Int32 LEN = offset + length;
    for(i=offset; i<LEN; ++i)
    {
        IBuffer::Probe(buffers[i])->GetRemaining(&remaining);
        count += remaining;

        if(copyingIn)
        {
            //buffers[i]->CheckWritable();
            printf("ERROR: IByteBuffer::CheckWritable() Not Public.");
            printf("%s:%d\n", __FILE__, __LINE__);
        }
    }

    *ret = count;
    return NOERROR;
}

ECode FileChannelImpl::GetFD(
    /* [out] */ IFileDescriptor** descriptor)
{
    VALIDATE_NOT_NULL(descriptor)

    (*descriptor) = mFd;
    REFCOUNT_ADD(*descriptor)
    return NOERROR;
}

ECode FileChannelImpl::AddLock(
    /* [in] */ IFileLock* lock)
{
    // long lockEnd = lock.position() + lock.size();
    // for (FileLock existingLock : locks) {
    //     if (existingLock.position() > lockEnd) {
    //         // This, and all remaining locks, start beyond our end (so
    //         // cannot overlap).
    //         break;
    //     }
    //     if (existingLock.overlaps(lock.position(), lock.size())) {
    //         throw new OverlappingFileLockException();
    //     }
    // }
    // locks.add(lock);
    return E_NOT_IMPLEMENTED;
}

void FileChannelImpl::RemoveLock(
    /* [in] */ IFileLock* lock)
{
    // locks.remove(lock);
}

} // namespace IO
} // namespace Elastos
