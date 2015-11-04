
#include "elastos/droid/webkit/CookieManager.h"
#include "elastos/droid/webkit/CookieSyncManager.h"
#include "elastos/droid/webkit/WebViewDatabase.h"

namespace Elastos {
namespace Droid {
namespace Webkit {

CAR_INTERFACE_IMPL(CookieSyncManager, Object, ICookieSyncManager);

CookieSyncManager::CookieSyncManager()
{
}

CookieSyncManager::CookieSyncManager(
    /* [in] */ IContext* context)
    : WebSyncManager(context, String("CookieSyncManager"))
{
}

ECode CookieSyncManager::constructor()
{
    return NOERROR;
}

ECode CookieSyncManager::Sync()
{
    return CookieManager::GetInstance()->Flush();
}

ECode CookieSyncManager::ResetSync()
{
    return NOERROR;
}

ECode CookieSyncManager::StartSync()
{
    return NOERROR;
}

ECode CookieSyncManager::StopSync()
{
    return NOERROR;
}

ECode CookieSyncManager::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);
    *info = "CookieSyncManager";
    return NOERROR;
}

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