
#include "app/NotificationManager.h"
#include "os/ServiceManager.h"
#ifdef DROID_CORE
#include "os/CUserHandleHelper.h"
#endif
#include <elastos/utility/logging/Slogger.h>

using Elastos::Utility::Logging::Slogger;
using Elastos::Droid::Os::IUserHandleHelper;
using Elastos::Droid::Os::CUserHandleHelper;

namespace Elastos {
namespace Droid {
namespace App {

const String NotificationManager::TAG = String("NotificationManager");
const Boolean NotificationManager::DEBUG = TRUE;
const Boolean NotificationManager::localLOGV = TRUE;

AutoPtr<IINotificationManager> NotificationManager::sService;

CAR_INTERFACE_IMPL(NotificationManager, INotificationManager);

/** @hide */
AutoPtr<IINotificationManager> NotificationManager::GetService()
{
    if (sService != NULL) {
        return sService;
    }

    sService = IINotificationManager::Probe(
            Elastos::Droid::Os::ServiceManager::GetService(String("notification")).Get());
    return sService;
}

AutoPtr<INotificationManager> NotificationManager::From(
    /* [in] */ IContext* context)
{
    AutoPtr<IInterface> obj;
    context->GetSystemService(IContext::NOTIFICATION_SERVICE, (IInterface**)&obj);
    AutoPtr<INotificationManager> mgr = INotificationManager::Probe(obj.Get());
    return mgr;
}

NotificationManager::NotificationManager(
    /* [in] */ IContext* context,
    /* [in] */ IHandler* handler)
    : mContext(context)
{}

ECode NotificationManager::Notify(
    /* [in] */ Int32 id,
    /* [in] */ INotification* notification)
{
    String nullStr;
    return Notify(nullStr, id, notification);
}

ECode NotificationManager::Notify(
    /* [in] */ const String& tag,
    /* [in] */ Int32 id,
    /* [in] */ INotification* notification)
{
    VALIDATE_NOT_NULL(notification);

    AutoPtr<IINotificationManager> service = GetService();
    String pkgName;
    mContext->GetPackageName(&pkgName);

    if (localLOGV) {
        String notificationStr;
        notification->ToString(&notificationStr);
        Slogger::V(TAG, "%s: notify(%d, %s)", pkgName.string(), id, notificationStr.string());
    }

    AutoPtr<IUri> sound;
    notification->GetSound((IUri**)&sound);
    if (sound != NULL) {
        AutoPtr<IUri> newSound;
        sound->GetCanonicalUri((IUri**)&newSound);
        notification->SetSound(newSound);
    }

    Int32 userId = 0;
    AutoPtr<IUserHandleHelper> handleHelper;
    FAIL_RETURN(CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&handleHelper));
    FAIL_RETURN(handleHelper->GetMyUserId(&userId));

    AutoPtr<ArrayOf<Int32> > idOut;
    FAIL_RETURN(service->EnqueueNotificationWithTag(
            pkgName, tag, id, notification, (ArrayOf<Int32>**)&idOut, userId));
    if (idOut == NULL || idOut->GetLength() == 0 || id != (*idOut)[0]) {
        Slogger::W(TAG, "notify: id corrupted: sent %d, got back %d",
            id, (idOut && idOut->GetLength() > 0) ? (*idOut)[0] : -1);
    }
    return NOERROR;
}

ECode NotificationManager::NotifyAsUser(
    /* [in] */ const String& tag,
    /* [in] */ Int32 id,
    /* [in] */ INotification* notification,
    /* [in] */ IUserHandle* user)
{
    AutoPtr<IINotificationManager> service = GetService();
    String pkgName;
    mContext->GetPackageName(&pkgName);
    AutoPtr<IUri> sound;
    notification->GetSound((IUri**)&sound);
    if (sound != NULL) {
        AutoPtr<IUri> newSound;
        sound->GetCanonicalUri((IUri**)&newSound);
        notification->SetSound(newSound);
    }

    if (localLOGV) {
        String notificationStr;
        notification->ToString(&notificationStr);
        Slogger::V(TAG, "%s: notify(%d, %s)", pkgName.string(), id, notificationStr.string());
    }

    Int32 identifier;
    user->GetIdentifier(&identifier);

    AutoPtr<ArrayOf<Int32> > idOut;
    FAIL_RETURN(service->EnqueueNotificationWithTag(
            pkgName, tag, id, notification, (ArrayOf<Int32>**)&idOut, identifier));
    if (idOut == NULL || idOut->GetLength() == 0 || id != (*idOut)[0]) {
        Slogger::W(TAG, "notify: id corrupted: sent %d, got back %d",
            id, (idOut && idOut->GetLength() > 0) ? (*idOut)[0] : -1);
    }
    return NOERROR;
}

ECode NotificationManager::Cancel(
    /* [in] */ Int32 id)
{
    return Cancel(String(NULL), id);
}

ECode NotificationManager::Cancel(
    /* [in] */ const String& tag,
    /* [in] */ Int32 id)
{
    AutoPtr<IINotificationManager> service = GetService();
    String cap;
    mContext->GetPackageName(&cap);
//    if (localLOGV) Log.v(TAG, pkg + ": cancel(" + id + ")");
    Int32 userId = 0;
    AutoPtr<IUserHandleHelper> handleHelper;
    FAIL_RETURN(CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&handleHelper));
    FAIL_RETURN(handleHelper->GetMyUserId(&userId));
    return service->CancelNotificationWithTag(cap, tag, id, userId);
}

ECode NotificationManager::CancelAsUser(
    /* [in] */ const String& tag,
    /* [in] */ Int32 id,
    /* [in] */ IUserHandle* user)
{
    AutoPtr<IINotificationManager> service = GetService();
    String cap;
    mContext->GetPackageName(&cap);
//    if (localLOGV) Log.v(TAG, pkg + ": cancel(" + id + ")");
    Int32 identifier;
    user->GetIdentifier(&identifier);
    return service->CancelNotificationWithTag(cap, tag, id, identifier);
}

ECode NotificationManager::CancelAll()
{
    AutoPtr<IINotificationManager> service = GetService();
    String cap;
    mContext->GetPackageName(&cap);
//    if (localLOGV) Log.v(TAG, pkg + ": cancelAll()");
    Int32 userId = 0;
    AutoPtr<IUserHandleHelper> handleHelper;
    FAIL_RETURN(CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&handleHelper));
    FAIL_RETURN(handleHelper->GetMyUserId(&userId));
    return service->CancelAllNotifications(cap, userId);
}

}
}
}
