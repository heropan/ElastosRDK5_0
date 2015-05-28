
#ifndef __CSELECTORPROVIDERIMPL_H__
#define __CSELECTORPROVIDERIMPL_H__

#include "_Elastos_IO_CSelectorProviderImpl.h"
#include "Object.h"

using Elastos::Core::Object;
using Elastos::IO::Channels::IDatagramChannel;
using Elastos::IO::Channels::IPipe;
using Elastos::IO::Channels::Spi::IAbstractSelector;
using Elastos::IO::Channels::IServerSocketChannel;
using Elastos::IO::Channels::ISocketChannel;
using Elastos::IO::Channels::IChannel;

namespace Elastos {
namespace IO {

CarClass(CSelectorProviderImpl), public Object
{
public:
    CAR_OBJECT_DECL()

    CARAPI OpenDatagramChannel(
        /* [out] */ IDatagramChannel ** ppChannel);

    CARAPI OpenPipe(
        /* [out] */ IPipe ** ppPipe);

    CARAPI OpenSelector(
        /* [out] */ IAbstractSelector ** ppSelector);

    CARAPI OpenServerSocketChannel(
        /* [out] */ IServerSocketChannel ** ppChannel);

    CARAPI OpenSocketChannel(
        /* [out] */ ISocketChannel ** ppSocketChannel);

    CARAPI InheritedChannel(
        /* [out] */ IChannel ** ppChanel);

    CARAPI constructor();

private:
    // TODO: Add your private member variables here.
};

} // namespace IO
} // namespace Elastos

#endif // __CSELECTORPROVIDERIMPL_H__
