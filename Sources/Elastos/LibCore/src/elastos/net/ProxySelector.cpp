
#include "ProxySelector.h"
// #include "CNetPermission.h"
#include "ProxySelectorImpl.h"
// #include <Com.Kortide.Platform.h>

namespace Elastos {
namespace Net {

AutoPtr<INetPermission> InitProxySelectorPermission(
    /* [in] */ const String& name)
{
    AutoPtr<INetPermission> netPermission;
    assert(0 && "TODO");
    // ASSERT_TRUE(CNetPermission::New(name, (INetPermission**)&netPermission));

    return netPermission;
}

AutoPtr<IProxySelector> ProxySelector::sDefaultSelector;

AutoPtr<INetPermission> ProxySelector::sGetProxySelectorPermission;

AutoPtr<INetPermission> ProxySelector::sSetProxySelectorPermission;

CAR_INTERFACE_IMPL(ProxySelector, Object, IProxySelect)

ECode ProxySelector::GetDefault(
    /* [out] */ IProxySelector** defaultSelector)
{
    VALIDATE_NOT_NULL(defaultSelector);
    // SecurityManager sm = System.getSecurityManager();
    // if (null != sm) {
    //     sm.checkPermission(getProxySelectorPermission);
    // }
    if (sDefaultSelector == NULL) {
        sDefaultSelector = new ProxySelectorImpl();
    }

    *defaultSelector = sDefaultSelector;
    REFCOUNT_ADD(*defaultSelector);
    return NOERROR;
}

ECode ProxySelector::SetDefault(
    /* [in] */ IProxySelector* selector)
{
    // SecurityManager sm = System.getSecurityManager();
    // if (null != sm) {
    //     sm.checkPermission(setProxySelectorPermission);
    // }
    sDefaultSelector = selector;
    return NOERROR;
}

} // namespace Net
} // namespace Elastos
