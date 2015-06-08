
#ifndef __ELASTOS_NET_CDATAGRAMSOCKET_H__
#define __ELASTOS_NET_CDATAGRAMSOCKET_H__

#include "DatagramSocket.h"
#include "_ELASTOS_NET_CDatagramSocket.h"

using Elastos::IO::IFileDescriptor;

namespace Elastos {
namespace Net {

CarClass(CDatagramSocket) , public DatagramSocket
{
public:
    CAR_OBJECT_DECL()
};

} // namespace Net
} // namespace Elastos

#endif //__ELASTOS_NET_CDATAGRAMSOCKET_H__
