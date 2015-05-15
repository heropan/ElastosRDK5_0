
#ifndef __CSELECTORPROVIDERIMPL_H__
#define __CSELECTORPROVIDERIMPL_H__

#include "_CSelectorProviderImpl.h"

namespace Elastos {
namespace IO {

CarClass(CSelectorProviderImpl)
{
public:
    CARAPI OpenDatagramChannel(
        /* [out] */ Elastos::IO::Channels::IDatagramChannel ** ppChannel);

    CARAPI OpenPipe(
        /* [out] */ Elastos::IO::Channels::IPipe ** ppPipe);

    CARAPI OpenSelector(
        /* [out] */ Elastos::IO::Channels::Spi::IAbstractSelector ** ppSelector);

    CARAPI OpenServerSocketChannel(
        /* [out] */ Elastos::IO::Channels::IServerSocketChannel ** ppChannel);

    CARAPI OpenSocketChannel(
        /* [out] */ Elastos::IO::Channels::ISocketChannel ** ppSocketChannel);

    CARAPI InheritedChannel(
        /* [out] */ Elastos::IO::Channels::IChannel ** ppChanel);

    CARAPI constructor();

private:
    // TODO: Add your private member variables here.
};

} // namespace IO
} // namespace Elastos

#endif // __CSELECTORPROVIDERIMPL_H__
