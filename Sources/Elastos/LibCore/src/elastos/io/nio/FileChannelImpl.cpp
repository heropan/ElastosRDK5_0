#include "FileChannelImpl.h"
#include "Math.h"
#include "FileLock.h"
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
using Elastos::IO::Channels::IFileLock;
using Elastos::IO::Channels::FileLock;
using Elastos::IO::Channels::IChannel;
using Elastos::IO::Channels::EIID_IFileChannel;
using Elastos::IO::MappedByteBufferAdapter;
using Elastos::Droid::System::IStructStat;

namespace Elastos {
namespace IO {

class FileLockImpl: public FileLock, public IFileLock
{
public:
    ECode Channel(IFileChannel** channel)
    {
        return FileLock::Channel(channel);
    }

    ECode GetPosition(Int64* position)
    {
        return FileLock::GetPosition(position);
    }

    ECode GetSize(Int64* size)
    {
        return FileLock::GetSize(size);
    }

    ECode IsShared(Boolean* shared)
    {
        return FileLock::IsShared(shared);
    }

    ECode Overlaps(Int64 start, Int64 length, Boolean* result)
    {
        return FileLock::Overlaps(start, length, result);
    }

    ECode ToString(String* string)
    {
        return FileLock::ToString(string);
    }

    FileLockImpl(IFileChannel* channel, Int64 position, Int64 size,
            Boolean shared)
            : FileLock(channel, position, size, shared), mIsReleased(FALSE)
    {
        assert(NULL != channel);
    }

    ECode IsValid(Boolean* ret)
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

    ECode ReleaseLock()
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

protected:

private:
    Boolean mIsReleased;
};
// end FileLockImpl

CAR_INTERFACE_IMPL(FileChannelImpl, Object, IFileChannelImpl)

FileChannelImpl::FileChannelImpl(
    /* [in] */ IObject* stream,
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

ECode FileChannelImpl::ReadByteBuffers(
    /* [in] */ const ArrayOf<IByteBuffer*>& buffers,
    /* [out] */ Int64* number)
{
    return FileChannel::ReadByteBuffers(buffers , number);
}

ECode FileChannelImpl::WriteByteBuffers(
    /* [in] */ const ArrayOf<IByteBuffer*>& buffers,
    /* [out] */ Int64* number)
{
    return FileChannel::WriteByteBuffers(buffers , number);
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
    Int32 accMode, wrOnly;
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
    Int32 accMode, rdOnly;
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
    Int32 ACCMOD, RDONLY, WRONLY;
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

    //FileLock *pendingLock = new FileLockImpl(this, position, size, shared);

   // AddLock(pendingLock);
    printf("ERROR: AddLock Not Implemented Yet\n");

   // IStructFlock* flock =
    printf("ERROR: Need StructFlock.\n");

    //delete pendingLock;
    // pendingLock = NULL;

    assert(0);
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
//    ECode ecRet = CheckOpen();
//    if(NOERROR != ecRet)
//    {
//        return ecRet;
//    }
    return E_NOT_IMPLEMENTED;
}

ECode FileChannelImpl::TryLock(
    /* [in] */ Int64 position,
    /* [in] */ Int64 size,
    /* [in] */ Boolean shared,
    /* [out] */ IFileLock** lock)
{
    return E_NOT_IMPLEMENTED;
}

ECode FileChannelImpl::ReleaseLock(
    /* [in] */ IFileLock* lock)
{
    return E_NOT_IMPLEMENTED;
}

ECode FileChannelImpl:: Force(
    /* [in] */ Boolean metadata)
{
   ECode ecRet = CheckOpen();
   if(NOERROR != ecRet)
       return ecRet;

   Int32 ACCMODE, RDONLY;
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
       Int32 fd;
       mFd->GetDescriptor(&fd);

        if (metadata)
        {
            ecRet = os->Fsync(fd);
            if (NOERROR != ecRet)
            {
                return ecRet;
            }
        }
        else
        {
            ecRet = os->Fdatasync(fd);
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

    Int32 ACCESSMODE, RDONLY, WRONLY;
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

    Int32 fd;

    Int64 nSize;
    ecRet = GetSize(&nSize);

    if(position + size > nSize)
    {
       mFd->GetDescriptor(&fd);
       ecRet = os->Ftruncate(fd, position + size);
       if(NOERROR != ecRet)
           return ecRet;
    }

    Int32 SC_PAGE_SIZE;
    Int64 nPageSize;

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

    Int32 seek_cur;
    assert(0 && "TODO");
    // ecRet = mOsConstants->GetOsConstant(String("SEEK_CUR"), &seek_cur);
    assert(NOERROR == ecRet);

    Int32 fd;
    mFd->GetDescriptor(&fd);
    return os->Lseek(fd, 0L, seek_cur, position);
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

    Int32 seek_cur;
    Int64 result;
    assert(0 && "TODO");
    // ecRet = mOsConstants->GetOsConstant(String("SEEK_CUR"), &seek_cur);
    assert(NOERROR == ecRet);
    Int32 fd;
    mFd->GetDescriptor(&fd);
    ecRet = os->Lseek(fd, newPosition, seek_cur, &result);
    assert(NOERROR == ecRet);
    return NOERROR;
}

ECode FileChannelImpl::ReadByteBuffer(
   /* [in] */ IByteBuffer* buffer,
   /* [out] */ Int32* number)
{
    return ReadImpl(buffer, -1, number);
}

ECode FileChannelImpl::ReadByteBuffer(
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
//    buffer->CheckWritable();

    return E_NOT_IMPLEMENTED;

}

ECode FileChannelImpl::ReadByteBuffers(
    /* [in] */ const ArrayOf<IByteBuffer*> &buffers,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ Int64* number)
{

    if ((offset | length) < 0 || offset > buffers.GetLength()
            || buffers.GetLength() - offset < length)
    {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    ECode ecRet = CheckOpen();
    if(NOERROR != ecRet)
        return ecRet;

    ecRet = CheckReadable();
    if(NOERROR != ecRet)
        return ecRet;

    //TransferIoVec();

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
    Int32 fd;
    mFd->GetDescriptor(&fd);
    os->Fstat(fd, (IStructStat**)&stat);
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

        ecRet = WriteByteBuffer(IByteBuffer::Probe(buffer), position, (Int32*)number);
        assert(0 && "TODO");
        // NioUtils::FreeDirectBuffer((ByteBuffer*)buffer->Probe(EIID_ByteBuffer));
        return ecRet;
    }

    AutoPtr<IByteBuffer> buffer;
    ByteBuffer::Allocate((Int32)count, (IByteBuffer**)&buffer);
    Int32 nRead;
    src->Read(buffer, &nRead);
    IBuffer::Probe(buffer)->Flip();
    return WriteByteBuffer(buffer, position, (Int32*)number);
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
        Int32 infdnum = 0;
        mFd->GetDescriptor(&infdnum);
        // try {
        Begin();
            // try {
        assert(0 && "TODO");
        // FAIL_RETURN(CLibcore::sOs->Sendfile(outFdnum, infdnum, &position, count, number));
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

        Int32 fd;
        mFd->GetDescriptor(&fd);
        ecRet = os->Ftruncate(fd, nSize);
    }

    *channel = (IFileChannel*) this->Probe(EIID_IFileChannel);
    REFCOUNT_ADD(*channel)

    return ecRet;
}

ECode FileChannelImpl::WriteByteBuffer(
    /* [in] */ IByteBuffer* buffer,
    /* [out] */ Int32* number)
{
    return WriteImpl(buffer, -1, number);
}

ECode FileChannelImpl::WriteByteBuffer(
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
        Int32 fd;
        mFd->GetDescriptor(&fd);
        if (-1 == position) {
            assert(0 && "TODO");
            // ecRet = CLibcore::sOs->Write(fd, buffer, &bytesWritten);
        }
        else {
            assert(0 && "TODO");
            // ecRet = CLibcore::sOs->Pwrite(fd, buffer, position, &bytesWritten);
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

ECode FileChannelImpl::WriteByteBuffers(
    /* [in] */ const ArrayOf<IByteBuffer*> &buffers,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ Int64* number)
{
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

ECode  FileChannelImpl::AddLock(
    /* [in] */ IFileLock* lock)
{
    return E_NOT_IMPLEMENTED;
}

void  FileChannelImpl::RemoveLock(
    /* [in] */ IFileLock* lock)
{
   printf("ERROR: Not Implemented Yet\n");
   assert(0);
}

} // namespace IO
} // namespace Elastos
