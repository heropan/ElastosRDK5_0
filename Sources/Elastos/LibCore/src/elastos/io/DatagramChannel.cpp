
#include "coredef.h"
#include "DatagramChannel.h"
#include "SelectionKey.h"
#include "SelectorProvider.h"

using Elastos::IO::Channels::Spi::SelectorProvider;

namespace Elastos {
namespace IO {
namespace Channels {

DatagramChannel::DatagramChannel(
    /* [in] */ ISelectorProvider* provider)
    : AbstractSelectableChannel(provider)
{
    assert(NULL != provider);
}

DatagramChannel::~DatagramChannel()
{
}

ECode DatagramChannel::Open(IDatagramChannel** channel)
{
    VALIDATE_NOT_NULL(channel)

    AutoPtr<ISelectorProvider> sp;
    FAIL_RETURN(SelectorProvider::Provider((ISelectorProvider**)&sp));
    return sp->OpenDatagramChannel(channel);
}

Int32 DatagramChannel::ValidOps()
{
    return (SelectionKey::sOP_READ | SelectionKey::sOP_WRITE);
}

ECode DatagramChannel::ReadByteBuffers(
    /* [in] */ const ArrayOf<IByteBuffer*>& targets,
    /* [out] */ Int64* nRead)
{
    return ReadByteBuffers(targets, 0, targets.GetLength(), nRead);
}

ECode DatagramChannel::WriteByteBuffers(
    /* [in] */ const ArrayOf<IByteBuffer*>& targets,
    /* [out] */ Int64* nWrite)
{
    return WriteByteBuffers(targets, 0, targets.GetLength(), nWrite);
}

} // namespace Channels
} // namespace IO
} // namespace Elastos
