#ifndef __ELASTOS_IO_FILECHANNELIMPL_H__
#define __ELASTOS_IO_FILECHANNELIMPL_H__

#include "Elastos.CoreLibrary_server.h"
#include "FileChannel.h"
#include "coredef.h"

using Elastos::Core::IComparator;
using Libcore::IO::IOsConstants;
using Elastos::IO::Channels::IFileLock;
using Elastos::IO::Channels::FileChannel;
using Elastos::IO::Channels::IReadableByteChannel;
using Elastos::IO::Channels::IWritableByteChannel;
using Elastos::IO::Channels::FileChannelMapMode;

namespace Elastos {
namespace IO {

class FileChannelImpl
    : public FileChannel
    , public IFileChannelImpl
    , public ElRefBase
{
public:

    CAR_INTERFACE_DECL()

    CARAPI IsOpen(
        /* [out] */ Boolean* rst);

    CARAPI Close();

    CARAPI Lock(
        /* [out] */ IFileLock** lock);

    CARAPI ReadByteBuffers(
        /* [in] */ const ArrayOf<IByteBuffer*>& buffers,
        /* [out] */ Int64* number);

    CARAPI TryLock(
        /* [out] */ IFileLock** lock);

    virtual CARAPI Lock(
        /* [in] */ Int64 position,
        /* [in] */ Int64 size,
        /* [in] */ Boolean shared,
        /* [out] */ IFileLock** lock);

    CARAPI TryLock(
        /* [in] */ Int64 position,
        /* [in] */ Int64 size,
        /* [in] */ Boolean shared,
        /* [out] */ IFileLock** lock);

    CARAPI Force(
        /* [in] */ Boolean metadata);

    CARAPI Map(
        /* [in] */ FileChannelMapMode mode,
        /* [in] */ Int64 position,
        /* [in] */ Int64 size,
        /* [out] */ IMappedByteBuffer** buffer);

    CARAPI GetPosition(
        /* [out] */ Int64* position);

    CARAPI SetPosition(
        /* [in] */ Int64 offset);

    CARAPI ReadByteBuffer(
        /* [in] */ IByteBuffer* buffer,
        /* [out] */ Int32* number);

    CARAPI ReadByteBuffer(
        /* [in] */ IByteBuffer* buffer,
        /* [in] */ Int64 position,
        /* [out] */ Int32* number);

    CARAPI ReadByteBuffers(
        /* [in] */ const ArrayOf<IByteBuffer*> &buffers,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ Int64* number);

    CARAPI TransferFrom(
        /* [in] */ IReadableByteChannel* src,
        /* [in] */ Int64 position,
        /* [in] */ Int64 count,
        /* [out] */ Int64* number);

    CARAPI TransferTo(
        /* [in] */ Int64 position,
        /* [in] */ Int64 count,
        /* [in] */ IWritableByteChannel* target,
        /* [out] */ Int64* number);

    CARAPI Truncate(
        /* [in] */ Int64 size,
        /* [out]*/ IFileChannel** channel);

    CARAPI GetSize(
        /* [out] */ Int64* size);

    CARAPI WriteByteBuffer(
        /* [in] */ IByteBuffer* buffer,
        /* [out] */ Int32* number);

    CARAPI WriteByteBuffers(
        /* [in] */ const ArrayOf<IByteBuffer*>& buffers,
        /* [out] */ Int64* number);

    CARAPI WriteByteBuffer(
        /* [in] */ IByteBuffer* buffer,
        /* [in] */ Int64 position,
        /* [out] */ Int32* number);

    CARAPI WriteByteBuffers(
        /* [in] */ const ArrayOf<IByteBuffer*> &buffers,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ Int64* number);

    FileChannelImpl(
        /* [in] */ IObject* stream,
        /* [in] */ IFileDescriptor* desc,
        /* [in] */ Int32 mode);

    CARAPI ReleaseLock(
        /* [in] */ IFileLock* lock);

    CARAPI GetFD(
        /* [out] */ IFileDescriptor** descriptor);

    CARAPI calculateTotalRemaining(
        /* [in] */ ArrayOf<IByteBuffer*> buffers,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [in] */ Boolean copyingIn,
        /* [out] */ Int32* ret);

protected:
    CARAPI ImplCloseChannel();

private:
    CARAPI CheckOpen();

    CARAPI CheckReadable();

    CARAPI CheckWritable();

    CARAPI BasicLock(
        /* [in] */ Int64 position,
        /* [in] */ Int64 size,
        /* [in] */ Boolean shared,
        /* [in] */ Boolean wait);

    CARAPI_(Int64) TranslateLockLength(
        /* [in] */ Int64 byteCount);
    CARAPI ReadImpl(
        /* [in] */ IByteBuffer* buffer,
        /* [in] */ Int64 position,
        /* [out] */ Int32* ret);

    //  CARAPI TransferIoVec(IoVec* ioVec, Int32* ret);

    CARAPI WriteImpl(
        /* [in] */ IByteBuffer* buffer,
        /* [in] */ Int64 position,
        /* [out] */ Int32* ret);

    CARAPI AddLock(
        /* [in] */ IFileLock* lock);

    void RemoveLock(
        /* [in] */ IFileLock* lock);

private:
    AutoPtr<IComparator> sCompator;
    AutoPtr<IObject> mStream;
    AutoPtr<IFileDescriptor> mFd;
    Int32 mMode;
//    Set<IFileLock*> mLocks;
    AutoPtr<IOsConstants> mOsConstants;
};

} // namespace IO
} // namespace Elastos

#endif // __ELASTOS_IO_BUFFER_H__
