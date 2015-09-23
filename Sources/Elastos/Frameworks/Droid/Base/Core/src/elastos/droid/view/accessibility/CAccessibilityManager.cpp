
#include "view/accessibility/CAccessibilityManager.h"
#include "view/accessibility/CAccessibilityManagerClient.h"
#include "os/UserHandle.h"
#include "os/SystemClock.h"
#include "os/Binder.h"
#include "os/ServiceManager.h"
#include <elastos/utility/logging/Slogger.h>

using Elastos::Utility::Logging::Slogger;

using Elastos::Droid::Os::IUserHandle;
using Elastos::Droid::Os::UserHandle;
using Elastos::Droid::Os::ILooper;
using Elastos::Droid::Os::SystemClock;
using Elastos::Droid::Os::ServiceManager;
using Elastos::Droid::Os::Binder;
using Elastos::Droid::Content::Pm::IResolveInfo;
using Elastos::Droid::Content::Pm::IServiceInfo;
using Elastos::Droid::AccessibilityService::IAccessibilityServiceInfo;

namespace Elastos {
namespace Droid {
namespace View {
namespace Accessibility {

const Int32 CAccessibilityManager::STATE_FLAG_ACCESSIBILITY_ENABLED = 0x00000001;
const Int32 CAccessibilityManager::STATE_FLAG_TOUCH_EXPLORATION_ENABLED = 0x00000002;
const Int32 CAccessibilityManager::DO_SET_STATE = 10;
const Boolean CAccessibilityManager::DEBUG = FALSE;
const String CAccessibilityManager::localLOG_TAG = String("AccessibilityManager");

AutoPtr<IAccessibilityManager> CAccessibilityManager::sInstance = NULL;
Mutex CAccessibilityManager::sInstanceSync;

ECode CAccessibilityManager::MyHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    Int32 what, arg1;
    msg->GetWhat(&what);
    msg->GetArg1(&arg1);

    switch(what) {
        case CAccessibilityManager::DO_SET_STATE:
            mHost->HandleDoSetState(arg1);
            break;
    }

    return NOERROR;
}

CAccessibilityManager::CAccessibilityManager()
    : mUserId(0)
    , mIsEnabled(FALSE)
    , mIsTouchExplorationEnabled(FALSE)
{
    CAccessibilityManagerClient::New((Handle32)this,
            (IAccessibilityManagerClient**)&mClient);
}

void CAccessibilityManager::HandleDoSetState(
    /* [in] */ Int32 state)
{
    SetState(state);
}

ECode CAccessibilityManager::CreateAsSharedAcrossUsers(
    /* [in] */ IContext* context)
{
    AutoLock lock(sInstanceSync);
    if (sInstance != NULL) {
        Slogger::E(localLOG_TAG, "AccessibilityManager already created.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
        // throw new IllegalStateException("AccessibilityManager already created.");
    }
    return CreateSingletonInstance(context, IUserHandle::USER_CURRENT);
}

ECode CAccessibilityManager::GetInstance(
    /* [in] */ IContext* context,
    /* [out] */ IAccessibilityManager** manager)
{
    VALIDATE_NOT_NULL(manager);
    *manager = NULL;

    AutoLock lock(sInstanceSync);
    if (sInstance == NULL) {
        FAIL_RETURN(CreateSingletonInstance(context, UserHandle::GetMyUserId()));
    }
    *manager = sInstance;
    REFCOUNT_ADD(*manager);
    return NOERROR;
}

ECode CAccessibilityManager::CreateSingletonInstance(
    /* [in] */ IContext* context,
    /* [in] */ Int32 userId)
{
    assert(sInstance == NULL);
    AutoPtr<IIAccessibilityManager> service =
            (IIAccessibilityManager*)ServiceManager::GetService(
                    IContext::ACCESSIBILITY_SERVICE).Get();
    assert(service != NULL);
    AutoPtr<CAccessibilityManager> cinstance;
    CAccessibilityManager::NewByFriend(context, service, userId,
            (CAccessibilityManager**)&cinstance);
    sInstance = cinstance;
    return NOERROR;
}

ECode CAccessibilityManager::IsEnabled(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    AutoLock lock(mHandlerLock);
    *result = mIsEnabled;
    return NOERROR;
}

ECode CAccessibilityManager::IsTouchExplorationEnabled(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    AutoLock lock(mHandlerLock);
    *result = mIsTouchExplorationEnabled;
    return NOERROR;
}

AutoPtr<IAccessibilityManagerClient> CAccessibilityManager::GetClient()
{
    return mClient;
}

ECode CAccessibilityManager::SendAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    if (!mIsEnabled) {
        Slogger::E(localLOG_TAG, "Accessibility off. Did you forget to check that?");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
        // throw new IllegalStateException("Accessibility off. Did you forget to check that?");
    }
    Boolean doRecycle = FALSE;
    // try {
    event->SetEventTime(SystemClock::GetUptimeMillis());
    // it is possible that this manager is in the same process as the service but
    // client using it is called through Binder from another process. Example: MMS
    // app adds a SMS notification and the NotificationManagerService calls this method
    Int64 identityToken = Binder::ClearCallingIdentity();
    ECode ec = mService->SendAccessibilityEvent(event, mUserId, &doRecycle);
    Binder::RestoreCallingIdentity(identityToken);
    if (FAILED(ec)) {
        Slogger::E(localLOG_TAG, "Error during sending %p 0x%08x", event, ec);
        return ec;
    }

    if (doRecycle) {
        event->Recycle();
    }
    return NOERROR;
    // if (DEBUG) {
    //     Log.i(localLOG_TAG, event + " sent");
    // }
    // } catch (RemoteException re) {
    //     Log.e(localLOG_TAG, "Error during sending " + event + " ", re);
    // } finally {
    //     if (doRecycle) {
    //         event.recycle();
    //     }
    // }
}

ECode CAccessibilityManager::Interrupt()
{
    if (!mIsEnabled) {
        Slogger::E(localLOG_TAG, "Accessibility off. Did you forget to check that?");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
        // throw new IllegalStateException("Accessibility off. Did you forget to check that?");
    }
    // try {
    ECode ec = mService->Interrupt(mUserId);
    if (FAILED(ec)) {
        Slogger::E(localLOG_TAG, "Error while requesting interrupt from all services. 0x%08x", ec);
        return ec;
    }
    return NOERROR;
    // if (DEBUG) {
    //     Log.i(localLOG_TAG, "Requested interrupt from all services");
    // }
    // } catch (RemoteException re) {
    //     Log.e(localLOG_TAG, "Error while requesting interrupt from all services. ", re);
    // }
}

ECode CAccessibilityManager::GetAccessibilityServiceList(
    /* [out] */ IObjectContainer** serviceList)
{
    VALIDATE_NOT_NULL(serviceList);

    AutoPtr<IObjectContainer> infos;
    FAIL_RETURN(GetInstalledAccessibilityServiceList((IObjectContainer**)&infos));
    AutoPtr<IObjectContainer> services;
    CObjectContainer::New((IObjectContainer**)&services);
    AutoPtr<IObjectEnumerator> objEmu;
    infos->GetObjectEnumerator((IObjectEnumerator**)&objEmu);
    Boolean isSucceeded;
    while (objEmu->MoveNext(&isSucceeded), isSucceeded) {
        AutoPtr<IAccessibilityServiceInfo> info;
        objEmu->Current((IInterface**)&info);
        AutoPtr<IResolveInfo> ri;
        ASSERT_SUCCEEDED(info->GetResolveInfo((IResolveInfo**)&ri));
        AutoPtr<IServiceInfo> serviceInfo;
        ri->GetServiceInfo((IServiceInfo**)&serviceInfo);
        services->Add(serviceInfo);
    }
    *serviceList = services;
    REFCOUNT_ADD(*serviceList);
    return NOERROR;
}

ECode CAccessibilityManager::GetInstalledAccessibilityServiceList(
    /* [out] */ IObjectContainer** serviceList)
{
    VALIDATE_NOT_NULL(serviceList);

    // try {
    ECode ec = mService->GetInstalledAccessibilityServiceList(mUserId, serviceList);
    if (FAILED(ec)) {
        Slogger::E(localLOG_TAG, "Error while obtaining the installed AccessibilityServices. 0x%08x", ec);
        return ec;
    }
    // if (DEBUG) {
    //     Log.i(localLOG_TAG, "Installed AccessibilityServices " + services);
    // }
    // } catch (RemoteException re) {
    //     Log.e(localLOG_TAG, "Error while obtaining the installed AccessibilityServices. ", re);
    // }
    return NOERROR;
}

ECode CAccessibilityManager::GetEnabledAccessibilityServiceList(
    /* [in] */ Int32 feedbackTypeFlags,
    /* [out] */ IObjectContainer** serviceList)
{
    VALIDATE_NOT_NULL(serviceList);

    // try {
    ECode ec = mService->GetEnabledAccessibilityServiceList(feedbackTypeFlags,
            mUserId, serviceList);
    if (FAILED(ec)) {
        Slogger::E(localLOG_TAG, "Error while obtaining the installed AccessibilityServices. 0x%08x", ec);
        return ec;
    }
    // if (DEBUG) {
    //     Log.i(localLOG_TAG, "Installed AccessibilityServices " + services);
    // }
    // } catch (RemoteException re) {
    //     Log.e(localLOG_TAG, "Error while obtaining the installed AccessibilityServices. ", re);
    // }
    return NOERROR;
}

ECode CAccessibilityManager::AddAccessibilityStateChangeListener(
    /* [in] */ IAccessibilityStateChangeListener* listener,
    /* [out] */ Boolean* add)
{
    VALIDATE_NOT_NULL(add);
    mAccessibilityStateChangeListeners.PushBack(listener);
    *add = TRUE;
    return NOERROR;
}

ECode CAccessibilityManager::RemoveAccessibilityStateChangeListener(
    /* [in] */ IAccessibilityStateChangeListener* listener,
    /* [out] */ Boolean* remove)
{
    VALIDATE_NOT_NULL(remove);
    mAccessibilityStateChangeListeners.Remove(listener);
    *remove = TRUE;
    return NOERROR;
}

void CAccessibilityManager::SetState(
    /* [in] */ Int32 stateFlags)
{
    Boolean accessibilityEnabled = (stateFlags & STATE_FLAG_ACCESSIBILITY_ENABLED) != 0;
    SetAccessibilityState(accessibilityEnabled);
    mIsTouchExplorationEnabled = (stateFlags & STATE_FLAG_TOUCH_EXPLORATION_ENABLED) != 0;
}

void CAccessibilityManager::SetAccessibilityState(
    /* [in] */ Boolean isEnabled)
{
    AutoLock lock(mHandlerLock);
    if (isEnabled != mIsEnabled) {
        mIsEnabled = isEnabled;
        NotifyAccessibilityStateChanged();
    }
}

void CAccessibilityManager::NotifyAccessibilityStateChanged()
{
    List<AutoPtr<IAccessibilityStateChangeListener> >::Iterator it
            = mAccessibilityStateChangeListeners.Begin();
    for (; it != mAccessibilityStateChangeListeners.End(); ++it) {
        (*it)->OnAccessibilityStateChanged(mIsEnabled);
    }
}

ECode CAccessibilityManager::AddAccessibilityInteractionConnection(
    /* [in] */ IIWindow* windowToken,
    /* [in] */ IAccessibilityInteractionConnection* connection,
    /* [out] */ Int32* add)
{
    VALIDATE_NOT_NULL(add);
    // try {
    ECode ec = mService->AddAccessibilityInteractionConnection(windowToken, connection, mUserId, add);
    if (FAILED(ec)) {
        Slogger::E(localLOG_TAG, "Error while adding an accessibility interaction connection. 0x%08x", ec);
        *add = IView::NO_ID;
        return ec;
    }
    return NOERROR;
    // } catch (RemoteException re) {
    //     Log.e(localLOG_TAG, "Error while adding an accessibility interaction connection. ", re);
    // }
    // return View.NO_ID;
}

ECode CAccessibilityManager::RemoveAccessibilityInteractionConnection(
    /* [in] */ IIWindow* windowToken)
{
    // try {
    ECode ec = mService->RemoveAccessibilityInteractionConnection(windowToken);
    if (FAILED(ec)) {
        Slogger::E(localLOG_TAG, "Error while removing an accessibility interaction connection. 0x%08x", ec);
    }
    return NOERROR;
    // } catch (RemoteException re) {
    //     Log.e(localLOG_TAG, "Error while removing an accessibility interaction connection. ", re);
    // }
}

ECode CAccessibilityManager::constructor(
    /* [in] */ IContext* base,
    /* [in] */ IIAccessibilityManager* service,
    /* [in] */ Int32 userId)
{
    assert(base != NULL && service != NULL);
    AutoPtr<ILooper> looper;
    base->GetMainLooper((ILooper**)&looper);

    mHandler = new MyHandler(this);
    mService = service;
    mUserId = userId;

    // try {
    Int32 stateFlags;
    ECode ec = mService->AddClient(mClient, userId, &stateFlags);
    if (FAILED(ec)) {
        Slogger::E(localLOG_TAG, "AccessibilityManagerService is dead 0x%08x", ec);
        return ec;
    }

    SetState(stateFlags);
    return NOERROR;
    // } catch (RemoteException re) {
    //     Log.e(localLOG_TAG, "AccessibilityManagerService is dead", re);
    // }
}

} // Accessibility
} // View
} // Droid
} // Elastos
