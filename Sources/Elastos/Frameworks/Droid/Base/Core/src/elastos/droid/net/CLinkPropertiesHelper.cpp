
#include "ext/frameworkdef.h"
#include "CLinkPropertiesHelper.h"
#include "CLinkProperties.h"

namespace Elastos {
namespace Droid {
namespace Net {

CAR_INTERFACE_IMPL(CLinkPropertiesHelper, Singleton, ILinkPropertiesHelper)

CAR_SINGLETON_IMPL(CLinkPropertiesHelper)

ECode CLinkPropertiesHelper::IsValidMtu(
    /* [in] */ Int32 mtu,
    /* [in] */ Boolean ipv6,
    /* [out] */ Boolean* result)
{
    return LinkPropertiesHelper::IsValidMtu(mtu, ipv6, result);
}

} // namespace Net
} // namespace Droid
} // namespace Elastos
