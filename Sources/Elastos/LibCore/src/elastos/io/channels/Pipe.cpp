#include "Pipe.h"
#include "SelectorProvider.h"

using Elastos::IO::Channels::Spi::SelectorProvider;

namespace Elastos{
namespace IO{
namespace Channels{

//==========================================================
//       Pipe::SinkChannel
//==========================================================
Pipe::SinkChannel::SinkChannel()
{}

ECode Pipe::SinkChannel::constructor(
    /* [in] */ ISelectorProvider* provider)
{
    return AbstractSelectableChannel::constructor(provider);
}

CAR_INTERFACE_IMPL_2(Pipe::SinkChannel, Object, IWritableByteChannel, IGatheringByteChannel)

ECode Pipe::SinkChannel::GetValidOps(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)

    *value = ISelectionKey::OP_WRITE;
    return NOERROR;
}

//==========================================================
//       Pipe::SourceChannel
//==========================================================
Pipe::SourceChannel::SourceChannel()
{}

Pipe::SourceChannel::constructor(
    /* [in] */ ISelectorProvider* provider)
{
    return AbstractSelectableChannel::constructor(provider);
}

CAR_INTERFACE_IMPL_2(Pipe::SourceChannel, Object, IReadableByteChannel, IScatteringByteChannel)

ECode Pipe::SourceChannel::GetValidOps(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)

    *value = ISelectionKey::OP_READ;
    return NOERROR;
}

//==========================================================
//       Pipe
//==========================================================
Pipe::Pipe()
{
}

CAR_INTERFACE_IMPL(Pipe, Object, IPipe)

ECode Pipe::Open(
    /* [out] */ IPipe** pipe)
{
    AutoPtr<ISelectorProvider> sp;
    FAIL_RETURN(SelectorProvider::GetProvider((ISelectorProvider**)&sp));
    return sp->OpenPipe(pipe);
}

} // namespace Channels
} // namespace IO
} // namespace Elastoss
