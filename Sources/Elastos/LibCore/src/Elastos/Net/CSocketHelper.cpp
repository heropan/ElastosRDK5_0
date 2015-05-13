
#include "CSocketHelper.h"
#include "Socket.h"

namespace Elastos {
namespace Net {

ECode CSocketHelper::SetSocketImplFactory(
    /* [in] */ ISocketImplFactory* fac)
{
    return Socket::SetSocketImplFactory(fac);
}

} // namespace Net
} // namespace Elastos
