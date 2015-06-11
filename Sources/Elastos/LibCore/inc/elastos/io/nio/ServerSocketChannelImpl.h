#ifndef __ELASTOS_IO_SERVERSOCKETCHANNELIMPL_H__
#define __ELASTOS_NET_SERVERSOCKETCHANNELIMPL_H__

#include "ServerSocketChannel.h"
#include "ServerSocket.h"

using Elastos::IO::IFileDescriptorChannel;

namespace Elastos {
namespace IO {

class ServerSocketAdapter : public ServerSocket
{

};

class ServerSocketChannelImpl
    : public ServerSocketChannel
    , public IFileDescriptorChannel
{

};

} // namespace IO
} // namespace Elastos

#endif // __ELASTOS_NET_SERVERSOCKETCHANNELIMPL_H__
