
#include "elastos/droid/webkit/CookieManager.h"
#include "elastos/droid/webkit/CookieSyncManager.h"
#include "elastos/droid/webkit/WebViewDatabase.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

CookieSyncManager::CookieSyncManager(
    /* [in] */ IContext* context)
    : WebSyncManager(context, String("CookieSyncManager"))
{
}

CAR_INTERFACE_IMPL(CookieSyncManager, ICookieSyncManager);

void CookieSyncManager::SyncFromRamToFlash()
{
//    if (DebugFlags.COOKIE_SYNC_MANAGER) {
//        Log.v(LOGTAG, "CookieSyncManager::syncFromRamToFlash STARTS");
//    }

    AutoPtr<ICookieManager> manager = CookieManager::GetInstance();

    assert(0);
//    if (!manager->AcceptCookie()) {
//        return;
//    }

//    manager->FlushCookieStore();

//    if (DebugFlags.COOKIE_SYNC_MANAGER) {
//        Log.v(LOGTAG, "CookieSyncManager::syncFromRamToFlash DONE");
//    }
}

ECode CookieSyncManager::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);
    *info = "CookieSyncManager";
    return NOERROR;
}

} // namespace Webkit
} // namepsace Droid
} // namespace Elastos