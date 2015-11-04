#ifndef __ELASTOS_DROID_ACCESSIBILITYSERVICE_ACCESSIBILITYSERVICE_H__
#define __ELASTOS_DROID_ACCESSIBILITYSERVICE_ACCESSIBILITYSERVICE_H__

#include "elastos/droid/ext/frameworkext.h"
// #include "elastos/droid/app/Service.h"
#include <elastos/core/Object.h>

// using Elastos::Droid::App::Service;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IContextWrapper;
using Elastos::Droid::Content::EIID_IContextWrapper;
using Elastos::Droid::Content::IComponentCallbacks;
using Elastos::Droid::Content::EIID_IComponentCallbacks;
using Elastos::Droid::Content::IComponentCallbacks2;
using Elastos::Droid::Content::EIID_IComponentCallbacks2;
using Elastos::Droid::Os::IBinder;
using Elastos::Droid::View::IKeyEvent;
using Elastos::Droid::View::Accessibility::IAccessibilityEvent;
using Elastos::Droid::View::Accessibility::IAccessibilityNodeInfo;
using Elastos::Utility::IList;

namespace Elastos {
namespace Droid {
namespace AccessibilityService {

class AccessibilityService
    // TODO,  public Elastos::Droid::App::Service replace Object
    // , public Elastos.Droid.App.IService
    : public Object
    , public IAccessibilityService
    , public IContextWrapper
    , public IContext
    , public IComponentCallbacks2
    , public IComponentCallbacks
{
public:
    class MyAccessibilityServiceCallbacks
        : public Object
        , public IAccessibilityServiceCallbacks
    {
    public:
        CAR_INTERFACE_DECL()

        MyAccessibilityServiceCallbacks(
            /* [in] */ AccessibilityService* host);

        CARAPI OnAccessibilityEvent(
            /* [in] */ IAccessibilityEvent* event);

        CARAPI OnInterrupt();

        CARAPI OnServiceConnected();

        CARAPI OnSetConnectionId(
            /* [in] */ Int32 connectionId);

        CARAPI OnGesture(
            /* [in] */ Int32 gestureId,
            /* [out] */ Boolean* result);

        CARAPI OnKeyEvent(
            /* [in] */ IKeyEvent* event,
            /* [out] */ Boolean* result);

    private:
        AccessibilityService* mHost;
    };

public:
    CAR_INTERFACE_DECL()

    /**
     * Callback for {@link android.view.accessibility.AccessibilityEvent}s.
     *
     * @param event An event.
     */
    CARAPI OnAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

    /**
     * Callback for interrupting the accessibility feedback.
     */
    CARAPI OnInterrupt();

    /**
     * Gets the windows on the screen. This method returns only the windows
     * that a sighted user can interact with, as opposed to all windows.
     * For example, if there is a modal dialog shown and the user cannot touch
     * anything behind it, then only the modal window will be reported
     * (assuming it is the top one). For convenience the returned windows
     * are ordered in a descending layer order, which is the windows that
     * are higher in the Z-order are reported first. Since the user can always
     * interact with the window that has input focus by typing, the focused
     * window is always returned (even if covered by a modal window).
     * <p>
     * <strong>Note:</strong> In order to access the windows your service has
     * to declare the capability to retrieve window content by setting the
     * {@link android.R.styleable#AccessibilityService_canRetrieveWindowContent}
     * property in its meta-data. For details refer to {@link #SERVICE_META_DATA}.
     * Also the service has to opt-in to retrieve the interactive windows by
     * setting the {@link AccessibilityServiceInfo#FLAG_RETRIEVE_INTERACTIVE_WINDOWS}
     * flag.
     * </p>
     *
     * @return The windows if there are windows and the service is can retrieve
     *         them, otherwise an empty list.
     */
    CARAPI GetWindows(
        /* [out] */ IList** windows);

    /**
     * Gets the root node in the currently active window if this service
     * can retrieve window content. The active window is the one that the user
     * is currently touching or the window with input focus, if the user is not
     * touching any window.
     * <p>
     * <strong>Note:</strong> In order to access the root node your service has
     * to declare the capability to retrieve window content by setting the
     * {@link android.R.styleable#AccessibilityService_canRetrieveWindowContent}
     * property in its meta-data. For details refer to {@link #SERVICE_META_DATA}.
     * </p>
     *
     * @return The root node if this service can retrieve window content.
     */
    CARAPI GetRootInActiveWindow(
        /* [out] */ IAccessibilityNodeInfo** info);

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
    CARAPI PerformGlobalAction(
        /* [in] */ Int32 action,
        /* [out] */ Boolean* result);

    /**
     * Find the view that has the specified focus type. The search is performed
     * across all windows.
     * <p>
     * <strong>Note:</strong> In order to access the windows your service has
     * to declare the capability to retrieve window content by setting the
     * {@link android.R.styleable#AccessibilityService_canRetrieveWindowContent}
     * property in its meta-data. For details refer to {@link #SERVICE_META_DATA}.
     * Also the service has to opt-in to retrieve the interactive windows by
     * setting the {@link AccessibilityServiceInfo#FLAG_RETRIEVE_INTERACTIVE_WINDOWS}
     * flag.Otherwise, the search will be performed only in the active window.
     * </p>
     *
     * @param focus The focus to find. One of {@link AccessibilityNodeInfo#FOCUS_INPUT} or
     *         {@link AccessibilityNodeInfo#FOCUS_ACCESSIBILITY}.
     * @return The node info of the focused view or null.
     *
     * @see AccessibilityNodeInfo#FOCUS_INPUT
     * @see AccessibilityNodeInfo#FOCUS_ACCESSIBILITY
     */
    CARAPI FindFocus(
        /* [in] */ Int32 focus,
        /* [out] */ IAccessibilityNodeInfo** ret);

    /**
     * Gets the an {@link AccessibilityServiceInfo} describing this
     * {@link AccessibilityService}. This method is useful if one wants
     * to change some of the dynamically configurable properties at
     * runtime.
     *
     * @return The accessibility service info.
     *
     * @see AccessibilityServiceInfo
     */
    CARAPI GetServiceInfo(
        /* [out] */ IAccessibilityServiceInfo** info);

    /**
     * Sets the {@link AccessibilityServiceInfo} that describes this service.
     * <p>
     * Note: You can call this method any time but the info will be picked up after
     *       the system has bound to this service and when this method is called thereafter.
     *
     * @param info The info.
     */
    CARAPI SetServiceInfo(
        /* [in] */ IAccessibilityServiceInfo* info);

    CARAPI OnBind(
        /* [in] */ IIntent* intent,
        /* [out] */ IBinder** binder);

protected:
    CARAPI OnServiceConnected();

    CARAPI OnGesture(
        /* [in] */ Int32 gestureId,
        /* [out] */ Boolean* result);

    /**
     * Callback that allows an accessibility service to observe the key events
     * before they are passed to the rest of the system. This means that the events
     * are first delivered here before they are passed to the device policy, the
     * input method, or applications.
     * <p>
     * <strong>Note:</strong> It is important that key events are handled in such
     * a way that the event stream that would be passed to the rest of the system
     * is well-formed. For example, handling the down event but not the up event
     * and vice versa would generate an inconsistent event stream.
     * </p>
     * <p>
     * <strong>Note:</strong> The key events delivered in this method are copies
     * and modifying them will have no effect on the events that will be passed
     * to the system. This method is intended to perform purely filtering
     * functionality.
     * <p>
     *
     * @param event The event to be processed.
     * @return If true then the event will be consumed and not delivered to
     *         applications, otherwise it will be delivered as usual.
     */
    CARAPI OnKeyEvent(
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* result);

private:
    /**
     * Sets the {@link AccessibilityServiceInfo} for this service if the latter is
     * properly set and there is an {@link IIAccessibilityServiceConnection} to the
     * AccessibilityManagerService.
     */
    CARAPI_(void) SendServiceInfo();

public:
    static const String TAG;
    Int32 mConnectionId;
    AutoPtr<IAccessibilityServiceInfo> mInfo;
};

}
}
}

#endif // __ELASTOS_DROID_ACCESSIBILITYSERVICE_ACCESSIBILITYSERVICE_H__
