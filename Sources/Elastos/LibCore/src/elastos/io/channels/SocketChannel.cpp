#include "SocketChannel.h"

namespace Elastos {
namespace IO {
namespace Channels {

SocketChannel::SocketChannel()
{}

SocketChannel::SocketChannel(
    /* [in] */ ISelectorProvider* provider)
    : AbstractSelectableChannel(provider)
{
    assert(NULL != provider);
}

CAR_INTERFACE_IMPL_2(SocketChannel, Object, IScatteringByteChannel, IGatheringByteChannel)

ECode SocketChannel::Open(
    /* [out] */ ISocketChannel** channel)
{
   return E_NOT_IMPLEMENTED;
}

ECode Open(
    /* [in] */ ISocketAddress* addr,
    /* [out] */ ISocketChannel** channel)
{
    ECode ecRet = SocketChannel::Open(channel);
    if(NOERROR != ecRet)
    {
        Boolean bRet;
        // (*channel)->Connect(addr, &bRet);
        assert(0 && "TOOD");
    }
    return ecRet;
}

ECode SocketChannel::ValidOps(
    /* [out] */ Int32* ret)
{
    VALIDATE_NOT_NULL(ret)

    *ret = ISelectionKey::OP_CONNECT | ISelectionKey::OP_READ
            | ISelectionKey::OP_WRITE;
    return NOERROR;
}

ECode SocketChannel::ReadByteBuffers(
    /* [in] */ const ArrayOf<IByteBuffer*>& targets,
    /* [out] */ Int64* number)
{
    return ReadByteBuffers(targets, 0, targets.GetLength(), number);
}

ECode SocketChannel::WriteByteBuffers(
    /* [in] */ const ArrayOf<IByteBuffer*>& buffers,
    /* [out] */ Int64* number)
{
    return WriteByteBuffers(buffers, 0 , buffers.GetLength(), number);
}

} // namespace Channels
} // namespace IO
} // namespace Elastos

