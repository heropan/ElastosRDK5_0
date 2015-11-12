#include "elastos/droid/accessibilityservice/AccessibilityService.h"
#include "elastos/droid/accessibilityservice/CAccessibilityServiceClientWrapper.h"
#include "elastos/droid/view/accessibility/CAccessibilityInteractionClient.h"
#include <elastos/utility/logging/Logger.h>

using Elastos::Droid::Content::EIID_IContext;
using Elastos::Droid::View::Accessibility::CAccessibilityInteractionClient;
using Elastos::Droid::View::Accessibility::IAccessibilityInteractionClient;
using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace AccessibilityService {

const String AccessibilityService::TAG("AccessibilityService");

CAR_INTERFACE_IMPL_5(AccessibilityService, Object, IAccessibilityService, /*IService,*/ IContextWrapper, IContext
        , IComponentCallbacks2, IComponentCallbacks)

CAR_INTERFACE_IMPL(AccessibilityService::MyAccessibilityServiceCallbacks, Object, IAccessibilityServiceCallbacks)

AccessibilityService::MyAccessibilityServiceCallbacks::MyAccessibilityServiceCallbacks(
    /* [in] */ AccessibilityService* host)
    : mHost(host)
{
}

ECode AccessibilityService::MyAccessibilityServiceCallbacks::OnAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    return mHost->OnAccessibilityEvent(event);
}

ECode AccessibilityService::MyAccessibilityServiceCallbacks::OnInterrupt()
{
    return mHost->OnInterrupt();
}

ECode AccessibilityService::MyAccessibilityServiceCallbacks::OnServiceConnected()
{
    return mHost->OnServiceConnected();
}

ECode AccessibilityService::MyAccessibilityServiceCallbacks::OnSetConnectionId(
    /* [in] */ Int32 connectionId)
{
    mHost->mConnectionId = connectionId;
    return NOERROR;
}

ECode AccessibilityService::MyAccessibilityServiceCallbacks::OnGesture(
    /* [in] */ Int32 gestureId,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return mHost->OnGesture(gestureId, result);
}

ECode AccessibilityService::MyAccessibilityServiceCallbacks::OnKeyEvent(
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return mHost->OnKeyEvent(event, result);
}

ECode AccessibilityService::OnAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    return NOERROR;
}

ECode AccessibilityService::OnInterrupt()
{
    return NOERROR;
}

ECode AccessibilityService::GetWindows(
    /* [out] */ IList** windows)
{
    VALIDATE_NOT_NULL(windows)

    AutoPtr<IAccessibilityInteractionClient> client = CAccessibilityInteractionClient::GetInstance();
    return client->GetWindows(mConnectionId, windows);
}

ECode AccessibilityService::GetRootInActiveWindow(
    /* [out] */ IAccessibilityNodeInfo** info)
{
    VALIDATE_NOT_NULL(info);

    AutoPtr<IAccessibilityInteractionClient> client = CAccessibilityInteractionClient::GetInstance();
    return client->GetRootInActiveWindow(mConnectionId, info);
}

ECode AccessibilityService::PerformGlobalAction(
    /* [in] */ Int32 action,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    AutoPtr<IAccessibilityInteractionClient> client = CAccessibilityInteractionClient::GetInstance();
    AutoPtr<IIAccessibilityServiceConnection> connection;
    client->GetConnection(mConnectionId, (IIAccessibilityServiceConnection**)&connection);

    if (connection != NULL) {
        // try {
        ECode ec = connection->PerformGlobalAction(action, result);
        if (SUCCEEDED(ec)) return NOERROR;
        Logger::W(TAG, "Error while calling performGlobalAction. 0x%08x", ec);
        // } catch (RemoteException re) {
        //     Log.w(TAG, "Error while calling performGlobalAction", re);
        // }
    }

    *result = FALSE;
    return NOERROR;
}

ECode AccessibilityService::FindFocus(
    /* [in] */ Int32 focus,
    /* [out] */ IAccessibilityNodeInfo** ret)
{
    VALIDATE_NOT_NULL(ret);

    AutoPtr<IAccessibilityInteractionClient> client = CAccessibilityInteractionClient::GetInstance();
    return client->FindFocus(mConnectionId,
            IAccessibilityNodeInfo::ANY_WINDOW_ID,
            IAccessibilityNodeInfo::ROOT_NODE_ID, focus, ret);
}

ECode AccessibilityService::GetServiceInfo(
    /* [out] */ IAccessibilityServiceInfo** info)
{
    VALIDATE_NOT_NULL(info);

    AutoPtr<IAccessibilityInteractionClient> client = CAccessibilityInteractionClient::GetInstance();
    AutoPtr<IIAccessibilityServiceConnection> connection;
    client->GetConnection(mConnectionId, (IIAccessibilityServiceConnection**)&connection);

    if (connection != NULL) {
        // try {
        ECode ec = connection->GetServiceInfo(info);
        if (SUCCEEDED(ec)) return NOERROR;
        Logger::W(TAG, "Error while getting AccessibilityServiceInfo. 0x%08x", ec);
        // } catch (RemoteException re) {
        //     Log.w(TAG, "Error while getting AccessibilityServiceInfo", re);
        // }
    }
    *info = NULL;
    return NOERROR;
}

ECode AccessibilityService::SetServiceInfo(
    /* [in] */ IAccessibilityServiceInfo* info)
{
    mInfo = info;
    SendServiceInfo();
    return NOERROR;
}

ECode AccessibilityService::OnBind(
    /* [in] */ IIntent* intent,
    /* [out] */ IBinder** binder)
{
    VALIDATE_NOT_NULL(binder);
    *binder = NULL;
    assert(0 && "TODO");
    // AutoPtr<ILooper> looper;
    // Service::GetMainLooper((ILooper**)&looper);
    // AutoPtr<MyAccessibilityServiceCallbacks> callbacks = new MyAccessibilityServiceCallbacks(this);
    // AutoPtr<IIAccessibilityServiceClient> wrapper;
    // CAccessibilityServiceClientWrapper::New(THIS_PROBE(IContext), looper, callbacks,
    //         (IIAccessibilityServiceClient**)&wrapper);
    // *binder = IBinder::Probe(wrapper);
    // REFCOUNT_ADD(*binder);
    return NOERROR;
}

ECode AccessibilityService::OnServiceConnected()
{
    return NOERROR;
}

ECode AccessibilityService::OnGesture(
    /* [in] */ Int32 gestureId,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    return NOERROR;
}

ECode AccessibilityService::OnKeyEvent(
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    return NOERROR;
}

void AccessibilityService::SendServiceInfo()
{
    AutoPtr<IAccessibilityInteractionClient> client = CAccessibilityInteractionClient::GetInstance();
    AutoPtr<IIAccessibilityServiceConnection> connection;
    client->GetConnection(mConnectionId, (IIAccessibilityServiceConnection**)&connection);

    if (mInfo != NULL && connection != NULL) {
        // try {
        ECode ec = connection->SetServiceInfo(mInfo);
        if (ec == (ECode)E_REMOTE_EXCEPTION) {
            Logger::W(TAG, "Error while setting AccessibilityServiceInfo. 0x%08x", ec);
        }
        mInfo = NULL;
        client->ClearCache();
        // } catch (RemoteException re) {
        //     Log.w(TAG, "Error while setting AccessibilityServiceInfo", re);
        // }
    }
}

}
}
}
