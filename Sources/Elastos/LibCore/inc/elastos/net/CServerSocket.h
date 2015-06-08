
#ifndef __ELASTOS_NET_CSERVERSOCKET_H__
#define __ELASTOS_NET_CSERVERSOCKET_H__

#include "_ELASTOS_NET_CServerSocket.h"
#include "ServerSocket.h"

namespace Elastos {
namespace Net {

CarClass(CServerSocket) , public ServerSocket
{
public:
    CAR_OBJECT_DECL()
};

} // namespace Net
} // namespace Elastos

#endif //__ELASTOS_NET_CSERVERSOCKET_H__
