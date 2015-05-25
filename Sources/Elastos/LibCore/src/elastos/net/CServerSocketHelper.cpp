
#include "CServerSocketHelper.h"
#include "ServerSocket.h"

namespace Elastos {
namespace Net {

ECode CServerSocketHelper::SetSocketFactory(
    /* [in] */ ISocketImplFactory* aFactory)
{
    return ServerSocket::SetSocketFactory(aFactory);
}

} // namespace Net
} // namespace Elastos
