
#include <CProxyHelper.h>
#include <CProxy.h>

namespace Elastos {
namespace Net {

ECode CProxyHelper::GetNO_PROXY(
    /* [out] */ IProxy** proxy)
{
    return CProxy::GetNO_PROXY(proxy);
}

} // namespace Net
} // namespace Elastos
