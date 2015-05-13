
#include "webkit/CookieManager.h"
#include "webkit/CookieSyncManager.h"
#include "webkit/WebViewDatabase.h"

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

} // namespace Webkit
} // namepsace Droid
} // namespace Elastos