
#include "SelectorProviderImpl.h"

namespace Elastos {
namespace IO {

ECode SelectorProviderImpl::OpenDatagramChannel(
    /* [out] */ Elastos::IO::Channels::IDatagramChannel** ppChannel)
{
    // return new DatagramChannelImpl(this);
    return E_NOT_IMPLEMENTED;
}

ECode SelectorProviderImpl::OpenPipe(
    /* [out] */ Elastos::IO::Channels::IPipe** ppPipe)
{
    // return new PipeImpl(this);
    return E_NOT_IMPLEMENTED;
}

ECode SelectorProviderImpl::OpenSelector(
    /* [out] */ Elastos::IO::Channels::Spi::IAbstractSelector** ppSelector)
{
    // return new SelectorImpl(this);
    return E_NOT_IMPLEMENTED;
}

ECode SelectorProviderImpl::OpenServerSocketChannel(
    /* [out] */ Elastos::IO::Channels::IServerSocketChannel** ppChannel)
{
    // return new ServerSocketChannelImpl(this);
    return E_NOT_IMPLEMENTED;
}

ECode SelectorProviderImpl::OpenSocketChannel(
    /* [out] */ Elastos::IO::Channels::ISocketChannel** ppSocketChannel)
{
    // return new SocketChannelImpl(this);
    return E_NOT_IMPLEMENTED;
}

ECode SelectorProviderImpl::InheritedChannel(
    /* [out] */ Elastos::IO::Channels::IChannel** ppChanel)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode SelectorProviderImpl::constructor()
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

} // namespace IO
} // namespace Elastos
