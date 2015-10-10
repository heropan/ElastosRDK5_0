#ifndef __ELASTOS_DROID_ACCESSIBILITYSERVICE_CUITESTAUTOMATIONBRIDGE_H__
#define __ELASTOS_DROID_ACCESSIBILITYSERVICE_CUITESTAUTOMATIONBRIDGE_H__

#include "_Elastos_Droid_AccessibilityService_CUiTestAutomationBridge.h"
#include "elastos/droid/ext/frameworkext.h"
#include <elastos/core/Object.h>

using Elastos::Droid::View::Accessibility::IAccessibilityEvent;
using Elastos::Droid::View::Accessibility::IAccessibilityNodeInfo;
using Elastos::Core::IRunnable;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Os::IHandlerThread;
using Elastos::Droid::Internal::Utility::IPredicate;
using Elastos::Core::Object;


namespace Elastos {
namespace Droid {
namespace AccessibilityService {

CarClass(CUiTestAutomationBridge)
{
private:
    class MyAccessibilityServiceCallbacks
        : public ElRefBase
        , public IAccessibilityServiceCallbacks
    {
    public:
        CAR_INTERFACE_DECL()

        MyAccessibilityServiceCallbacks(
            /* [in] */ CUiTestAutomationBridge* mHost);

        CARAPI OnAccessibilityEvent(
            /* [in] */ IAccessibilityEvent* event);

        CARAPI OnInterrupt();

        CARAPI OnServiceConnected();

        CARAPI OnSetConnectionId(
            /* [in] */ Int32 connectionId);

        CARAPI OnGesture(
            /* [in] */ Int32 gestureId,
            /* [out] */ Boolean* result);
    private:
        CUiTestAutomationBridge* mHost;
    };

public:
    CUiTestAutomationBridge();

    ~CUiTestAutomationBridge();

    /**
     * Gets the last received {@link AccessibilityEvent}.
     *
     * @return The event.
     */
    CARAPI GetLastAccessibilityEvent(
        /* [out] */ IAccessibilityEvent** event);

    /**
     * Callback for receiving an {@link AccessibilityEvent}.
     *
     * <strong>Note:</strong> This method is <strong>NOT</strong>
     * executed on the application main thread. The client are
     * responsible for proper synchronization.
     *
     * @param event The received event.
     */
    CARAPI OnAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

    /**
     * Callback for requests to stop feedback.
     *
     * <strong>Note:</strong> This method is <strong>NOT</strong>
     * executed on the application main thread. The client are
     * responsible for proper synchronization.
     */
    CARAPI OnInterrupt();

    /**
     * Connects this service.
     *
     * @throws IllegalStateException If already connected.
     */
    CARAPI Connect();

    /**
     * Disconnects this service.
     *
     * @throws IllegalStateException If already disconnected.
     */
    CARAPI Disconnect();

    /**
     * Gets whether this service is connected.
     *
     * @return True if connected.
     */
    CARAPI IsConnected(
        /* [out] */ Boolean* result);

    /**
     * Executes a command and waits for a specific accessibility event type up
     * to a given timeout.
     *
     * @param command The command to execute before starting to wait for the event.
     * @param predicate Predicate for recognizing the awaited event.
     * @param timeoutMillis The max wait time in milliseconds.
     */
    CARAPI ExecuteCommandAndWaitForAccessibilityEvent(
        /* [in] */ IRunnable* command,
        /* [in] */ IPredicate* predicate,
        /* [in] */ Int64 timeoutMillis,
        /* [out] */ IAccessibilityEvent** event);

    /**
     * Waits for the accessibility event stream to become idle, which is not to
     * have received a new accessibility event within <code>idleTimeout</code>,
     * and do so within a maximal global timeout as specified by
     * <code>globalTimeout</code>.
     *
     * @param idleTimeout The timeout between two event to consider the device idle.
     * @param globalTimeout The maximal global timeout in which to wait for idle.
     */
    CARAPI WaitForIdle(
        /* [in] */ Int64 idleTimeout,
        /* [in] */ Int64 globalTimeout);

    /**
     * Finds an {@link AccessibilityNodeInfo} by accessibility id in the active
     * window. The search is performed from the root node.
     *
     * @param accessibilityNodeId A unique view id or virtual descendant id for
     *     which to search.
     * @return The current window scale, where zero means a failure.
     */
    CARAPI FindAccessibilityNodeInfoByAccessibilityIdInActiveWindow(
        /* [in] */ Int64 accessibilityNodeId,
        /* [out] */ IAccessibilityNodeInfo** info);

    /**
     * Finds an {@link AccessibilityNodeInfo} by accessibility id.
     *
     * @param accessibilityWindowId A unique window id. Use {@link #ACTIVE_WINDOW_ID} to query
     *     the currently active window.
     * @param accessibilityNodeId A unique view id or virtual descendant id for
     *     which to search.
     * @return The current window scale, where zero means a failure.
     */
    CARAPI FindAccessibilityNodeInfoByAccessibilityId(
        /* [in] */ Int32 accessibilityWindowId,
        /* [in] */ Int64 accessibilityNodeId,
        /* [out] */ IAccessibilityNodeInfo** info);

    /**
     * Finds an {@link AccessibilityNodeInfo} by View id in the active
     * window. The search is performed from the root node.
     *
     * @param viewId The id of a View.
     * @return The current window scale, where zero means a failure.
     */
    CARAPI FindAccessibilityNodeInfoByViewIdInActiveWindow(
        /* [in] */ Int32 viewId,
        /* [out] */ IAccessibilityNodeInfo** info);

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
    CARAPI FindAccessibilityNodeInfoByViewId(
        /* [in] */ Int32 accessibilityWindowId,
        /* [in] */ Int64 accessibilityNodeId,
        /* [in] */ Int32 viewId,
        /* [out] */ IAccessibilityNodeInfo** info);

        /**
     * Finds {@link AccessibilityNodeInfo}s by View text in the active
     * window. The search is performed from the root node.
     *
     * @param text The searched text.
     * @return The current window scale, where zero means a failure.
     */
    CARAPI FindAccessibilityNodeInfosByTextInActiveWindow(
        /* [in] */ const String& text,
        /* [out] */ IObjectContainer** list);

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
    CARAPI FindAccessibilityNodeInfosByText(
        /* [in] */ Int32 accessibilityWindowId,
        /* [in] */ Int64 accessibilityNodeId,
        /* [in] */ const String& text,
        /* [out] */ IObjectContainer** list);

        /**
     * Performs an accessibility action on an {@link AccessibilityNodeInfo}
     * in the active window.
     *
     * @param accessibilityNodeId A unique node id (accessibility and virtual descendant id).
     * @param action The action to perform.
     * @param arguments Optional action arguments.
     * @return Whether the action was performed.
     */
    CARAPI PerformAccessibilityActionInActiveWindow(
        /* [in] */ Int64 accessibilityNodeId,
        /* [in] */ Int32 action,
        /* [in] */ IBundle* arguments,
        /* [out] */ Boolean* result);

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
    CARAPI PerformAccessibilityAction(
        /* [in] */ Int32 accessibilityWindowId,
        /* [in] */ Int64 accessibilityNodeId,
        /* [in] */ Int32 action,
        /* [in] */ IBundle* arguments,
        /* [out] */ Boolean* result);

        /**
     * Gets the root {@link AccessibilityNodeInfo} in the active window.
     *
     * @return The root info.
     */
    CARAPI GetRootAccessibilityNodeInfoInActiveWindow(
        /* [out] */ IAccessibilityNodeInfo** info);

private:
    CARAPI EnsureValidConnection(
        /* [in] */ Int32 connectionId);

public:
    static const String TAG;

    static const Int32 TIMEOUT_REGISTER_SERVICE;

    static const Int32 FIND_ACCESSIBILITY_NODE_INFO_PREFETCH_FLAGS;

    Object mLock;

    volatile Int32 mConnectionId;

    AutoPtr<IAccessibilityServiceClientWrapper> mListener;

    AutoPtr<IAccessibilityEvent> mLastEvent;

    volatile Boolean mWaitingForEventDelivery;

    volatile Boolean mUnprocessedEventAvailable;

    AutoPtr<IHandlerThread> mHandlerThread;

};

}
}
}

#endif // __ELASTOS_DROID_ACCESSIBILITYSERVICE_CUITESTAUTOMATIONBRIDGE_H__
