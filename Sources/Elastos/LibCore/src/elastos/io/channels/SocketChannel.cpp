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
}

CAR_INTERFACE_IMPL_3(SocketChannel, Object, IByteChannel, IScatteringByteChannel, IGatheringByteChannel)

ECode SocketChannel::Open(
    /* [out] */ ISocketChannel** channel)
{
    // return SelectorProvider.provider().openSocketChannel();
    return E_NOT_IMPLEMENTED;
}

ECode SocketChannel::Open(
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

ECode SocketChannel::GetValidOps(
    /* [out] */ Int32* ret)
{
    VALIDATE_NOT_NULL(ret)

    *ret = ISelectionKey::OP_CONNECT | ISelectionKey::OP_READ
            | ISelectionKey::OP_WRITE;
    return NOERROR;
}

ECode SocketChannel::Read(
    /* [in] */ ArrayOf<IByteBuffer*>* targets,
    /* [out] */ Int64* number)
{
    return Read(targets, 0, targets->GetLength(), number);
}

ECode SocketChannel::Write(
    /* [in] */ ArrayOf<IByteBuffer*>* buffers,
    /* [out] */ Int64* number)
{
    return Write(buffers, 0 , buffers->GetLength(), number);
}

} // namespace Channels
} // namespace IO
} // namespace Elastos

