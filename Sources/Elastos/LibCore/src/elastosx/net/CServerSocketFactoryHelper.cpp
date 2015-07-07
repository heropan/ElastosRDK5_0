
#include "CServerSocketFactoryHelper.h"
#include "ServerSocketFactory.h"

namespace Elastosx {
namespace Net {

CAR_INTERFACE_IMPL(CServerSocketFactoryHelper, Singleton, IServerSocketFactoryHelper)

CAR_SINGLETON_IMPL(CServerSocketFactoryHelper)

ECode CServerSocketFactoryHelper::GetDefault(
    /* [out] */ IServerSocketFactory** factory)
{
    VALIDATE_NOT_NULL(factory)

    return ServerSocketFactory::GetDefault(factory);
}

} // namespace Net
} // namespace Elastosx
