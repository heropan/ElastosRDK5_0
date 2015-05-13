#include "AccessibilityService.h"
#include "accessibilityservice/CAccessibilityServiceClientWrapper.h"
#include "view/accessibility/CAccessibilityInteractionClientHelper.h"

using Elastos::Droid::Content::EIID_IContext;
using Elastos::Droid::View::Accessibility::IAccessibilityInteractionClient;
using Elastos::Droid::View::Accessibility::IAccessibilityInteractionClientHelper;
using Elastos::Droid::View::Accessibility::CAccessibilityInteractionClientHelper;

namespace Elastos {
namespace Droid {
namespace AccessibilityService {

const String AccessibilityService::TAG("AccessibilityService");

CAR_INTERFACE_IMPL(AccessibilityService::MyAccessibilityServiceCallbacks, IAccessibilityServiceCallbacks)

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
    return mHost->OnGesture(gestureId, result);
}

PInterface AccessibilityService::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IAccessibilityService*)this;
    }
    else if (riid == EIID_IObject) {
        return (IObject*)this;
    }
    if (riid == Elastos::Droid::App::EIID_IService) {
        return (IService*)(IObject *)this;
    }
    else if (riid == Elastos::Droid::Content::EIID_IContextWrapper) {
       return (IContextWrapper*)(IObject *)this;
    }
    else if (riid == Elastos::Droid::Content::EIID_IContext) {
       return (IContext*)(IObject *)this;
    }
    else if (riid == Elastos::Droid::Content::EIID_IComponentCallbacks2) {
       return (IComponentCallbacks2*)this;
    }
    else if (riid == EIID_IAccessibilityService) {
       return (IAccessibilityService*)this;
    }

    return NULL;
}

ECode AccessibilityService::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (NULL == pIID) return E_INVALID_ARGUMENT;

    if (pObject == (IInterface *)(IAccessibilityService *)this) {
        *pIID = EIID_IAccessibilityService;
        return NOERROR;
    }
    else if (pObject == (IInterface *)(IObject *)this) {
        *pIID = EIID_IObject;
        return NOERROR;
    }
    else if (pObject == (IContextWrapper *)(IObject *)this) {
        *pIID = Elastos::Droid::Content::EIID_IContextWrapper;
        return NOERROR;
    }
    else if (pObject == (IContext *)(IObject *)this) {
        *pIID = Elastos::Droid::Content::EIID_IContext;
        return NOERROR;
    }
    else if (pObject == (IComponentCallbacks2 *)(IObject *)this) {
        *pIID = Elastos::Droid::Content::EIID_IComponentCallbacks2;
        return NOERROR;
    }
    else if (pObject == (IService *)(IObject *)this) {
        *pIID = Elastos::Droid::App::EIID_IService;
        return NOERROR;
    }

    return E_INVALID_ARGUMENT;
}

/**
 * Callback for {@link android.view.accessibility.AccessibilityEvent}s.
 *
 * @param event An event.
 */
ECode AccessibilityService::OnAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    return NOERROR;
}

/**
 * Callback for interrupting the accessibility feedback.
 */
ECode AccessibilityService::OnInterrupt()
{
    return NOERROR;
}

/**
 * Gets the root node in the currently active window if this service
 * can retrieve window content.
 *
 * @return The root node if this service can retrieve window content.
 */
ECode AccessibilityService::GetRootInActiveWindow(
    /* [out] */ IAccessibilityNodeInfo** info)
{
    VALIDATE_NOT_NULL(info);

    AutoPtr<IAccessibilityInteractionClientHelper> helper;
    CAccessibilityInteractionClientHelper::AcquireSingleton((IAccessibilityInteractionClientHelper**)&helper);
    AutoPtr<IAccessibilityInteractionClient> client;
    helper->GetInstance((IAccessibilityInteractionClient**)&client);
    return client->GetRootInActiveWindow(mConnectionId, info);
}

/**
 * Performs a global action. Such an action can be performed
 * at any moment regardless of the current application or user
 * location in that application. For example going back, going
 * home, opening recents, etc.
 *
 * @param action The action to perform.
 * @return Whether the action was successfully performed.
 *
 * @see #GLOBAL_ACTION_BACK
 * @see #GLOBAL_ACTION_HOME
 * @see #GLOBAL_ACTION_NOTIFICATIONS
 * @see #GLOBAL_ACTION_RECENTS
 */
ECode AccessibilityService::PerformGlobalAction(
    /* [in] */ Int32 action,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;

    AutoPtr<IAccessibilityInteractionClientHelper> helper;
    CAccessibilityInteractionClientHelper::AcquireSingleton((IAccessibilityInteractionClientHelper**)&helper);
    AutoPtr<IAccessibilityInteractionClient> client;
    helper->GetInstance((IAccessibilityInteractionClient**)&client);
    AutoPtr<IAccessibilityServiceConnection> connection;
    client->GetConnection(mConnectionId, (IAccessibilityServiceConnection**)&connection);

    if (connection != NULL) {
        // try {
        return connection->PerformGlobalAction(action, result);
        // } catch (RemoteException re) {
        //     Log.w(TAG, "Error while calling performGlobalAction", re);
        // }
    }
    return NOERROR;
}

/**
 * Gets the an {@link AccessibilityServiceInfo} describing this
 * {@link AccessibilityService}. This method is useful if one wants
 * to change some of the dynamically configurable properties at
 * runtime.
 *
 * @return The accessibility service info.
 *
 * @see AccessibilityNodeInfo
 */
ECode AccessibilityService::GetServiceInfo(
    /* [out] */ IAccessibilityServiceInfo** info)
{
    VALIDATE_NOT_NULL(info);
    *info = NULL;

    AutoPtr<IAccessibilityInteractionClientHelper> helper;
    CAccessibilityInteractionClientHelper::AcquireSingleton((IAccessibilityInteractionClientHelper**)&helper);
    AutoPtr<IAccessibilityInteractionClient> client;
    helper->GetInstance((IAccessibilityInteractionClient**)&client);
    AutoPtr<IAccessibilityServiceConnection> connection;
    client->GetConnection(mConnectionId, (IAccessibilityServiceConnection**)&connection);

    if (connection != NULL) {
        // try {
        connection->GetServiceInfo(info);
        return NOERROR;
        // } catch (RemoteException re) {
        //     Log.w(TAG, "Error while getting AccessibilityServiceInfo", re);
        // }
    }
    return NOERROR;
}

/**
 * Sets the {@link AccessibilityServiceInfo} that describes this service.
 * <p>
 * Note: You can call this method any time but the info will be picked up after
 *       the system has bound to this service and when this method is called thereafter.
 *
 * @param info The info.
 */
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
    AutoPtr<ILooper> looper;
    Service::GetMainLooper((ILooper**)&looper);
    AutoPtr<MyAccessibilityServiceCallbacks> callbacks = new MyAccessibilityServiceCallbacks(this);
    AutoPtr<IAccessibilityServiceClientWrapper> wrapper;
    CAccessibilityServiceClientWrapper::New(THIS_PROBE(IContext), looper, callbacks,
        (IAccessibilityServiceClientWrapper**)&wrapper);
    *binder = IBinder::Probe(wrapper);
    INTERFACE_ADDREF(*binder);
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

/**
 * Sets the {@link AccessibilityServiceInfo} for this service if the latter is
 * properly set and there is an {@link IAccessibilityServiceConnection} to the
 * AccessibilityManagerService.
 */
void AccessibilityService::SendServiceInfo()
{
    AutoPtr<IAccessibilityInteractionClientHelper> helper;
    CAccessibilityInteractionClientHelper::AcquireSingleton((IAccessibilityInteractionClientHelper**)&helper);
    AutoPtr<IAccessibilityInteractionClient> client;
    helper->GetInstance((IAccessibilityInteractionClient**)&client);
    AutoPtr<IAccessibilityServiceConnection> connection;
    client->GetConnection(mConnectionId, (IAccessibilityServiceConnection**)&connection);

    if (mInfo != NULL && connection != NULL) {
        // try {
        connection->SetServiceInfo(mInfo);
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
