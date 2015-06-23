#include "CUiTestAutomationBridge.h"
#include "os/SystemClock.h"
#include "os/CHandlerThread.h"
#include "os/ServiceManager.h"
#include "CAccessibilityServiceInfo.h"
#include "accessibilityservice/CAccessibilityServiceClientWrapper.h"
#include "view/accessibility/CAccessibilityEventHelper.h"
#include "view/accessibility/CAccessibilityInteractionClientHelper.h"

using Elastos::Droid::Os::SystemClock;
using Elastos::Droid::Os::ServiceManager;
using Elastos::Droid::Os::ILooper;
using Elastos::Droid::Os::CHandlerThread;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::View::Accessibility::IAccessibilityEventHelper;
using Elastos::Droid::View::Accessibility::CAccessibilityEventHelper;
using Elastos::Droid::View::Accessibility::IIAccessibilityManager;
using Elastos::Droid::View::Accessibility::IAccessibilityInteractionClient;
using Elastos::Droid::View::Accessibility::IAccessibilityInteractionClientHelper;
using Elastos::Droid::View::Accessibility::CAccessibilityInteractionClientHelper;

namespace Elastos {
namespace Droid {
namespace AccessibilityService {

const String CUiTestAutomationBridge::TAG("CUiTestAutomationBridge");    //UiTestAutomationBridge.class.getSimpleName();

const Int32 CUiTestAutomationBridge::TIMEOUT_REGISTER_SERVICE = 5000;

const Int32 CUiTestAutomationBridge::FIND_ACCESSIBILITY_NODE_INFO_PREFETCH_FLAGS =
    IAccessibilityNodeInfo::FLAG_PREFETCH_PREDECESSORS
    | IAccessibilityNodeInfo::FLAG_PREFETCH_SIBLINGS
    | IAccessibilityNodeInfo::FLAG_PREFETCH_DESCENDANTS;

CAR_INTERFACE_IMPL(CUiTestAutomationBridge::MyAccessibilityServiceCallbacks, IAccessibilityServiceCallbacks)

CUiTestAutomationBridge::MyAccessibilityServiceCallbacks::MyAccessibilityServiceCallbacks(
    /* [in] */ CUiTestAutomationBridge* host)
    : mHost(host)
{
}

ECode CUiTestAutomationBridge::MyAccessibilityServiceCallbacks::OnAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    {
        Object::Autolock(mHost->mLock);

        AutoPtr<IAccessibilityEventHelper> helper;
        CAccessibilityEventHelper::AcquireSingleton((IAccessibilityEventHelper**)&helper);
        while (TRUE) {
            mHost->mLastEvent = NULL;
            helper->Obtain(event, (IAccessibilityEvent**)&mHost->mLastEvent);
            if (!mHost->mWaitingForEventDelivery) {
                mHost->mLock.NotifyAll();
                break;
            }

            if (!mHost->mUnprocessedEventAvailable) {
                mHost->mUnprocessedEventAvailable = TRUE;
                mHost->mLock.NotifyAll();
                break;
            }
            // try {
                mHost->mLock.Wait();
            // } catch (InterruptedException ie) {
            //      ignore
            // }
        }
    }
    return mHost->OnAccessibilityEvent(event);
}

ECode CUiTestAutomationBridge::MyAccessibilityServiceCallbacks::OnInterrupt()
{
    return mHost->OnInterrupt();
}

ECode CUiTestAutomationBridge::MyAccessibilityServiceCallbacks::OnServiceConnected()
{
    /* do nothing */
    return NOERROR;
}

ECode CUiTestAutomationBridge::MyAccessibilityServiceCallbacks::OnSetConnectionId(
    /* [in] */ Int32 connectionId)
{
    Object::Autolock(mHost->mLock);
    mHost->mConnectionId = connectionId;
    mHost->mLock.NotifyAll();
    return NOERROR;
}

ECode CUiTestAutomationBridge::MyAccessibilityServiceCallbacks::OnGesture(
    /* [in] */ Int32 gestureId,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    return NOERROR;
}

CUiTestAutomationBridge::CUiTestAutomationBridge()
    : mWaitingForEventDelivery(FALSE)
    , mUnprocessedEventAvailable(FALSE)
    , mConnectionId(IAccessibilityInteractionClient::NO_ID)
{
}

CUiTestAutomationBridge::~CUiTestAutomationBridge()
{
}

/**
 * Gets the last received {@link AccessibilityEvent}.
 *
 * @return The event.
 */
ECode CUiTestAutomationBridge::GetLastAccessibilityEvent(
    /* [out] */ IAccessibilityEvent** event)
{
    VALIDATE_NOT_NULL(event);
    *event = mLastEvent;
    REFCOUNT_ADD(*event);
    return NOERROR;
}

/**
 * Callback for receiving an {@link AccessibilityEvent}.
 *
 * <strong>Note:</strong> This method is <strong>NOT</strong>
 * executed on the application main thread. The client are
 * responsible for proper synchronization.
 *
 * @param event The received event.
 */
ECode CUiTestAutomationBridge::OnAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    return NOERROR;
}

/**
 * Callback for requests to stop feedback.
 *
 * <strong>Note:</strong> This method is <strong>NOT</strong>
 * executed on the application main thread. The client are
 * responsible for proper synchronization.
 */
ECode CUiTestAutomationBridge::OnInterrupt()
{
    return NOERROR;
}

/**
 * Connects this service.
 *
 * @throws IllegalStateException If already connected.
 */
ECode CUiTestAutomationBridge::Connect()
{
    Boolean b;
    IsConnected(&b);
    if (b) {
        // throw new IllegalStateException("Already connected.");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    // Serialize binder calls to a handler on a dedicated thread
    // different from the main since we expose APIs that block
    // the main thread waiting for a result the deliver of which
    // on the main thread will prevent that thread from waking up.
    // The serialization is needed also to ensure that events are
    // examined in delivery order. Otherwise, a fair locking
    // is needed for making sure the binder calls are interleaved
    // with check for the expected event and also to make sure the
    // binder threads are allowed to proceed in the received order.
    mHandlerThread = NULL;
    CHandlerThread::New(String("UiTestAutomationBridge"), (IHandlerThread**)&mHandlerThread);
    mHandlerThread->SetDaemon(TRUE);
    mHandlerThread->Start();
    AutoPtr<ILooper> looper;
    mHandlerThread->GetLooper((ILooper**)&looper);

    AutoPtr<MyAccessibilityServiceCallbacks> callbacks = new MyAccessibilityServiceCallbacks(this);
    CAccessibilityServiceClientWrapper::New(NULL, looper, callbacks, (IAccessibilityServiceClientWrapper**)&mListener);

    AutoPtr<IIAccessibilityManager> manager =
            (IIAccessibilityManager*)ServiceManager::GetService(IContext::ACCESSIBILITY_SERVICE).Get();

    AutoPtr<IAccessibilityServiceInfo> info;
    CAccessibilityServiceInfo::New((IAccessibilityServiceInfo**)&info);
    info->SetEventTypes(IAccessibilityEvent::TYPES_ALL_MASK);
    info->SetFeedbackType(IAccessibilityServiceInfo::FEEDBACK_GENERIC);
    Int32 flags;
    info->GetFlags(&flags);
    info->SetFlags(flags |= IAccessibilityServiceInfo::FLAG_INCLUDE_NOT_IMPORTANT_VIEWS);

    // try {
    if(FAILED(manager->RegisterUiTestAutomationService(mListener, info))) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    // } catch (RemoteException re) {
    //     throw new IllegalStateException("Cound not register UiAutomationService.", re);
    // }

    Object::Autolock lock(mLock);
    Int64 startTimeMillis = SystemClock::GetUptimeMillis();
    while (TRUE) {
        Boolean b;
        IsConnected(&b);
        if (!b) {
            return NOERROR;
        }
        Int64 elapsedTimeMillis = SystemClock::GetUptimeMillis() - startTimeMillis;
        Int64 remainingTimeMillis = TIMEOUT_REGISTER_SERVICE - elapsedTimeMillis;
        if (remainingTimeMillis <= 0) {
            // throw new IllegalStateException("Cound not register UiAutomationService.");
            return E_ILLEGAL_STATE_EXCEPTION;
        }
        // try {
        mLock.Wait(remainingTimeMillis);
        // } catch (InterruptedException ie) {
        //     /* ignore */
        // }
    }
    return NOERROR;
}

/**
 * Disconnects this service.
 *
 * @throws IllegalStateException If already disconnected.
 */
ECode CUiTestAutomationBridge::Disconnect()
{
    Boolean connetced;
    IsConnected(&connetced);
    if (!connetced) {
        // throw new IllegalStateException("Already disconnected.");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    Boolean b;
    mHandlerThread->Quit(&b);

    AutoPtr<IIAccessibilityManager> manager =
        (IIAccessibilityManager*)ServiceManager::GetService(IContext::ACCESSIBILITY_SERVICE).Get();

    // try {
    manager->UnregisterUiTestAutomationService(mListener);
    // } catch (RemoteException re) {
    //     Log.e(TAG, "Error while unregistering UiTestAutomationService", re);
    // }
    return NOERROR;
}

/**
 * Gets whether this service is connected.
 *
 * @return True if connected.
 */
ECode CUiTestAutomationBridge::IsConnected(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = (mConnectionId != IAccessibilityInteractionClient::NO_ID);
    return NOERROR;
}

/**
 * Executes a command and waits for a specific accessibility event type up
 * to a given timeout.
 *
 * @param command The command to execute before starting to wait for the event.
 * @param predicate Predicate for recognizing the awaited event.
 * @param timeoutMillis The max wait time in milliseconds.
 */
ECode CUiTestAutomationBridge::ExecuteCommandAndWaitForAccessibilityEvent(
    /* [in] */ IRunnable* command,
    /* [in] */ IPredicate* predicate,
    /* [in] */ Int64 timeoutMillis,
    /* [out] */ IAccessibilityEvent** event)
{
    VALIDATE_NOT_NULL(event);
    *event = NULL;

    // TODO: This is broken - remove from here when finalizing this as public APIs.
    Object::Autolock lock(mLock);
    // Prepare to wait for an event.
    mWaitingForEventDelivery = TRUE;
    mUnprocessedEventAvailable = FALSE;
    if (mLastEvent != NULL) {
        mLastEvent->Recycle();
        mLastEvent = NULL;
    }
    // Execute the command.
    command->Run();
    // Wait for the event.
    Int64 startTimeMillis = SystemClock::GetUptimeMillis();
    while (TRUE) {
        // If the expected event is received, that's it.
        Boolean b;
        predicate->Apply(mLastEvent, &b);
        if (mUnprocessedEventAvailable && b) {
            mWaitingForEventDelivery = FALSE;
            mUnprocessedEventAvailable = FALSE;
            mLock.NotifyAll();
            *event = mLastEvent;
            REFCOUNT_ADD(*event);
            return NOERROR;
        }
        // Ask for another event.
        mWaitingForEventDelivery = TRUE;
        mUnprocessedEventAvailable = FALSE;
        mLock.NotifyAll();
        // Check if timed out and if not wait.
        Int64 elapsedTimeMillis = SystemClock::GetUptimeMillis() - startTimeMillis;
        Int64 remainingTimeMillis = timeoutMillis - elapsedTimeMillis;
        if (remainingTimeMillis <= 0) {
            mWaitingForEventDelivery = FALSE;
            mUnprocessedEventAvailable = FALSE;
            mLock.NotifyAll();
            // throw new TimeoutException("Expacted event not received within: "
            //         + timeoutMillis + " ms.");
            return E_INVALID_ARGUMENT;  //return E_TIMEOUT_EXCEPTION;
        }
        // try {
        mLock.Wait(remainingTimeMillis);
        // } catch (InterruptedException ie) {
        //     /* ignore */
        // }
    }
    return NOERROR;
}

/**
 * Waits for the accessibility event stream to become idle, which is not to
 * have received a new accessibility event within <code>idleTimeout</code>,
 * and do so within a maximal global timeout as specified by
 * <code>globalTimeout</code>.
 *
 * @param idleTimeout The timeout between two event to consider the device idle.
 * @param globalTimeout The maximal global timeout in which to wait for idle.
 */
ECode CUiTestAutomationBridge::WaitForIdle(
    /* [in] */ Int64 idleTimeout,
    /* [in] */ Int64 globalTimeout)
{
    Int64 startTimeMillis = SystemClock::GetUptimeMillis();
    Int64 lastEventTime;
    if (mLastEvent != NULL)
    {
        mLastEvent->GetEventTime(&lastEventTime);
    }
    else
    {
        lastEventTime = SystemClock::GetUptimeMillis();
    }
    Object::Autolock lock(mLock);
    while (TRUE) {
        Int64 currentTimeMillis = SystemClock::GetUptimeMillis();
        Int64 sinceLastEventTimeMillis = currentTimeMillis - lastEventTime;
        if (sinceLastEventTimeMillis > idleTimeout) {
            return NOERROR;
        }
        if (mLastEvent != NULL) {
            mLastEvent->GetEventTime(&lastEventTime);
        }
        Int64 elapsedTimeMillis = SystemClock::GetUptimeMillis() - startTimeMillis;
        Int64 remainingTimeMillis = globalTimeout - elapsedTimeMillis;
        if (remainingTimeMillis <= 0) {
            return NOERROR;
        }
        // try {
        mLock.Wait(idleTimeout);
        // } catch (InterruptedException e) {
        //      /* ignore */
        // }
    }
    return NOERROR;
}

/**
 * Finds an {@link AccessibilityNodeInfo} by accessibility id in the active
 * window. The search is performed from the root node.
 *
 * @param accessibilityNodeId A unique view id or virtual descendant id for
 *     which to search.
 * @return The current window scale, where zero means a failure.
 */
ECode CUiTestAutomationBridge::FindAccessibilityNodeInfoByAccessibilityIdInActiveWindow(
    /* [in] */ Int64 accessibilityNodeId,
    /* [out] */ IAccessibilityNodeInfo** info)
{
    return FindAccessibilityNodeInfoByAccessibilityId(
        ACTIVE_WINDOW_ID, accessibilityNodeId, info);
}

/**
 * Finds an {@link AccessibilityNodeInfo} by accessibility id.
 *
 * @param accessibilityWindowId A unique window id. Use {@link #ACTIVE_WINDOW_ID} to query
 *     the currently active window.
 * @param accessibilityNodeId A unique view id or virtual descendant id for
 *     which to search.
 * @return The current window scale, where zero means a failure.
 */
ECode CUiTestAutomationBridge::FindAccessibilityNodeInfoByAccessibilityId(
    /* [in] */ Int32 accessibilityWindowId,
    /* [in] */ Int64 accessibilityNodeId,
    /* [out] */ IAccessibilityNodeInfo** info)
{
    // Cache the id to avoid locking
    Int32 connectionId = mConnectionId;
    EnsureValidConnection(connectionId);

    AutoPtr<IAccessibilityInteractionClientHelper> helper;
    CAccessibilityInteractionClientHelper::AcquireSingleton((IAccessibilityInteractionClientHelper**)&helper);
    AutoPtr<IAccessibilityInteractionClient> client;
    helper->GetInstance((IAccessibilityInteractionClient**)&client);

    return client->FindAccessibilityNodeInfoByAccessibilityId(
        mConnectionId, accessibilityWindowId, accessibilityNodeId,
        FIND_ACCESSIBILITY_NODE_INFO_PREFETCH_FLAGS, info);
}

/**
 * Finds an {@link AccessibilityNodeInfo} by View id in the active
 * window. The search is performed from the root node.
 *
 * @param viewId The id of a View.
 * @return The current window scale, where zero means a failure.
 */
ECode CUiTestAutomationBridge::FindAccessibilityNodeInfoByViewIdInActiveWindow(
    /* [in] */ Int32 viewId,
    /* [out] */ IAccessibilityNodeInfo** info)
{
    return FindAccessibilityNodeInfoByViewId(ACTIVE_WINDOW_ID,
        ROOT_NODE_ID, viewId, info);
}

    /**
 * Finds an {@link AccessibilityNodeInfo} by View id. The search is performed in
 * the window whose id is specified and starts from the node whose accessibility
 * id is specified.
 *
 * @param accessibilityWindowId A unique window id. Use
 *     {@link  #ACTIVE_WINDOW_ID} to query the currently active window.
 * @param accessibilityNodeId A unique view id or virtual descendant id from
 *     where to start the search. Use {@link  #ROOT_NODE_ID} to start from the root.
 * @param viewId The id of a View.
 * @return The current window scale, where zero means a failure.
 */
ECode CUiTestAutomationBridge::FindAccessibilityNodeInfoByViewId(
    /* [in] */ Int32 accessibilityWindowId,
    /* [in] */ Int64 accessibilityNodeId,
    /* [in] */ Int32 viewId,
    /* [out] */ IAccessibilityNodeInfo** info)
{
    VALIDATE_NOT_NULL(info);

    // Cache the id to avoid locking
    Int32 connectionId = mConnectionId;
    EnsureValidConnection(connectionId);

    AutoPtr<IAccessibilityInteractionClientHelper> helper;
    CAccessibilityInteractionClientHelper::AcquireSingleton((IAccessibilityInteractionClientHelper**)&helper);
    AutoPtr<IAccessibilityInteractionClient> client;
    helper->GetInstance((IAccessibilityInteractionClient**)&client);

    return client->FindAccessibilityNodeInfoByViewId(
        connectionId, accessibilityWindowId, accessibilityNodeId, viewId, info);
}

    /**
 * Finds {@link AccessibilityNodeInfo}s by View text in the active
 * window. The search is performed from the root node.
 *
 * @param text The searched text.
 * @return The current window scale, where zero means a failure.
 */
ECode CUiTestAutomationBridge::FindAccessibilityNodeInfosByTextInActiveWindow(
    /* [in] */ const String& text,
    /* [out] */ IObjectContainer** list)
{
    return FindAccessibilityNodeInfosByText(ACTIVE_WINDOW_ID,
        ROOT_NODE_ID, text, list);
}

    /**
 * Finds {@link AccessibilityNodeInfo}s by View text. The match is case
 * insensitive containment. The search is performed in the window whose
 * id is specified and starts from the node whose accessibility id is
 * specified.
 *
 * @param accessibilityWindowId A unique window id. Use
 *     {@link #ACTIVE_WINDOW_ID} to query the currently active window.
 * @param accessibilityNodeId A unique view id or virtual descendant id from
 *     where to start the search. Use {@link #ROOT_NODE_ID} to start from the root.
 * @param text The searched text.
 * @return The current window scale, where zero means a failure.
 */
ECode CUiTestAutomationBridge::FindAccessibilityNodeInfosByText(
    /* [in] */ Int32 accessibilityWindowId,
    /* [in] */ Int64 accessibilityNodeId,
    /* [in] */ const String& text,
    /* [out] */ IObjectContainer** list)
{
    VALIDATE_NOT_NULL(list);
    // Cache the id to avoid locking
    Int32 connectionId = mConnectionId;
    EnsureValidConnection(connectionId);

    AutoPtr<IAccessibilityInteractionClientHelper> helper;
    CAccessibilityInteractionClientHelper::AcquireSingleton((IAccessibilityInteractionClientHelper**)&helper);
    AutoPtr<IAccessibilityInteractionClient> client;
    helper->GetInstance((IAccessibilityInteractionClient**)&client);

    return client->FindAccessibilityNodeInfosByText(
        connectionId, accessibilityWindowId, accessibilityNodeId, text, list);
}

    /**
 * Performs an accessibility action on an {@link AccessibilityNodeInfo}
 * in the active window.
 *
 * @param accessibilityNodeId A unique node id (accessibility and virtual descendant id).
 * @param action The action to perform.
 * @param arguments Optional action arguments.
 * @return Whether the action was performed.
 */
ECode CUiTestAutomationBridge::PerformAccessibilityActionInActiveWindow(
    /* [in] */ Int64 accessibilityNodeId,
    /* [in] */ Int32 action,
    /* [in] */ IBundle* arguments,
    /* [out] */ Boolean* result)
{
    return PerformAccessibilityAction(ACTIVE_WINDOW_ID,
        accessibilityNodeId, action, arguments, result);
}

    /**
 * Performs an accessibility action on an {@link AccessibilityNodeInfo}.
 *
 * @param accessibilityWindowId A unique window id. Use
 *     {@link #ACTIVE_WINDOW_ID} to query the currently active window.
 * @param accessibilityNodeId A unique node id (accessibility and virtual descendant id).
 * @param action The action to perform.
 * @param arguments Optional action arguments.
 * @return Whether the action was performed.
 */
ECode CUiTestAutomationBridge::PerformAccessibilityAction(
    /* [in] */ Int32 accessibilityWindowId,
    /* [in] */ Int64 accessibilityNodeId,
    /* [in] */ Int32 action,
    /* [in] */ IBundle* arguments,
    /* [out] */ Boolean* result)
{
    // Cache the id to avoid locking
    Int32 connectionId = mConnectionId;
    EnsureValidConnection(connectionId);

    AutoPtr<IAccessibilityInteractionClientHelper> helper;
    CAccessibilityInteractionClientHelper::AcquireSingleton((IAccessibilityInteractionClientHelper**)&helper);
    AutoPtr<IAccessibilityInteractionClient> client;
    helper->GetInstance((IAccessibilityInteractionClient**)&client);
    return client->PerformAccessibilityAction(
        connectionId, accessibilityWindowId, accessibilityNodeId, action, arguments, result);
    return NOERROR;
}

    /**
 * Gets the root {@link AccessibilityNodeInfo} in the active window.
 *
 * @return The root info.
 */
ECode CUiTestAutomationBridge::GetRootAccessibilityNodeInfoInActiveWindow(
    /* [out] */ IAccessibilityNodeInfo** info)
{
    // Cache the id to avoid locking
    Int32 connectionId = mConnectionId;
    EnsureValidConnection(connectionId);

    AutoPtr<IAccessibilityInteractionClientHelper> helper;
    CAccessibilityInteractionClientHelper::AcquireSingleton((IAccessibilityInteractionClientHelper**)&helper);
    AutoPtr<IAccessibilityInteractionClient> client;
    helper->GetInstance((IAccessibilityInteractionClient**)&client);
    return client->FindAccessibilityNodeInfoByAccessibilityId(
        connectionId, ACTIVE_WINDOW_ID, ROOT_NODE_ID,
        IAccessibilityNodeInfo::FLAG_PREFETCH_DESCENDANTS, info);
}

ECode CUiTestAutomationBridge::EnsureValidConnection(
    /* [in] */ Int32 connectionId)
{
    if (connectionId == UNDEFINED) {
        // throw new IllegalStateException("UiAutomationService not connected."
        //         + " Did you call #register()?");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    return NOERROR;
}

}
}
}
