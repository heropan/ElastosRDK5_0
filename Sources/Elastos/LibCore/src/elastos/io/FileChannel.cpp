#include "FileChannel.h"
#include <coredef.h>

namespace Elastos {
namespace IO {
namespace Channels {

FileChannel::FileChannel()
    : AbstractInterruptibleChannel()
{}

ECode FileChannel::Lock(
    /*[out]*/ IFileLock** lock)
{
    //return this->LockEx(0L, 2E(sizeof(Int64) - 1), FALSE, lock);
    return this->LockEx(0L, LONG_MAX, FALSE, lock);
}

ECode FileChannel::ReadByteBuffers(
    /* [in] */ const ArrayOf<IByteBuffer*> &buffers,
    /* [out] */ Int64* number)
{
    return ReadByteBuffersEx(buffers, 0, 0, number);
}

ECode FileChannel::TryLock(
    /* [out] */ IFileLock** lock)
{
    return this->TryLockEx(0L, LONG_MAX, FALSE, lock);
}

ECode FileChannel::WriteByteBuffers(
    /* [in] */ const ArrayOf<IByteBuffer*> &buffers,
    /* [out] */ Int64* number)
{
    return this->WriteByteBuffersEx(buffers, 0, buffers.GetLength(), number);
}

} // namespace Channels
} // namespace IO
} // namespace Elastos
