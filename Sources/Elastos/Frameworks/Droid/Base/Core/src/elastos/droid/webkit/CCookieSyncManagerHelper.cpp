
#include "elastos/droid/webkit/CCookieSyncManagerHelper.h"
#include "elastos/droid/webkit/CookieSyncManager.h"
#include <elastos/core/AutoLock.h>

using Elastos::Core::AutoLock;

namespace Elastos {
namespace Droid {
namespace Webkit {

CAR_INTERFACE_IMPL(CCookieSyncManagerHelper, Object, ICookieSyncManagerHelper);

CAR_SINGLETON_IMPL(CCookieSyncManagerHelper);

ECode CCookieSyncManagerHelper::GetInstance(
    /* [out] */ ICookieSyncManager** instance)
{
    AutoLock lock(this);
    VALIDATE_NOT_NULL(instance);
    CheckInstanceIsCreated();
    *instance = sRef;
    REFCOUNT_ADD(*instance);
    return NOERROR;
}

ECode CCookieSyncManagerHelper::CreateInstance(
    /* [in] */ IContext* context,
    /* [out] */ ICookieSyncManager** instance)
{
    AutoLock lock(this);
    VALIDATE_NOT_NULL(context);
    VALIDATE_NOT_NULL(instance);

    if (sRef == NULL) {
        sRef = new CookieSyncManager(context);
    }
    *instance = sRef;
    REFCOUNT_ADD(*instance);

    return NOERROR;
}

ECode CCookieSyncManagerHelper::CheckInstanceIsCreated()
{
    if (sRef == NULL) {
        return E_NOT_IMPLEMENTED;
    }
    return NOERROR;
}

} // namespace Webkit
} // namespace Droid
} // namespace Elastos