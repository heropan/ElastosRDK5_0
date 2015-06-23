#include "Pipe.h"

namespace Elastos{
namespace IO{
namespace Channels{

//==========================================================
//       Pipe::SinkChannel
//==========================================================
Pipe::SinkChannel::SinkChannel(
    /* [in] */ ISelectorProvider* provider)
{
    // super(provider);
}

CAR_INTERFACE_IMPL_2(Pipe::SinkChannel, Object, IWritableByteChannel, IGatheringByteChannel)

ECode Pipe::SinkChannel::ValidOps(
    /* [out] */ Int32* value)
{
    // return SelectionKey.OP_WRITE;
    return E_NOT_IMPLEMENTED;
}

//==========================================================
//       Pipe::SourceChannel
//==========================================================

Pipe::SourceChannel::SourceChannel(
    /* [in] */ ISelectorProvider* provider)
{
    // super(provider);
}

CAR_INTERFACE_IMPL_2(Pipe::SourceChannel, Object, IReadableByteChannel, IScatteringByteChannel)

ECode Pipe::SourceChannel::ValidOps(
    /* [out] */ Int32* value)
{
    // return SelectionKey.OP_READ;
    return E_NOT_IMPLEMENTED;
}

//==========================================================
//       Pipe
//==========================================================
Pipe::Pipe()
{

}

CAR_INTERFACE_IMPL(Pipe, Object, IPipe)

ECode Pipe::Open(IPipe** pipe)
{
    // return SelectorProvider.provider().openPipe();
    return E_NOT_IMPLEMENTED;
}

} // namespace Channels
} // namespace IO
} // namespace Elastoss
