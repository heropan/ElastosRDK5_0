#ifndef __ELASTOS_DROID_ACCESSIBILITYSERVICE_CACCESSIBILITYSERVICE_H__
#define __ELASTOS_DROID_ACCESSIBILITYSERVICE_CACCESSIBILITYSERVICE_H__

#include "ext/frameworkext.h"
#include "app/Service.h"

#include "content/Context.h"

using Elastos::Droid::View::Accessibility::IAccessibilityEvent;
using Elastos::Droid::View::Accessibility::IAccessibilityNodeInfo;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::Context;
using Elastos::Droid::Os::IBinder;
using Elastos::Droid::App::Service;

namespace Elastos {
namespace Droid {
namespace AccessibilityService {

class AccessibilityService
    : public Elastos::Droid::App::Service
    , public IAccessibilityService
{
    class MyAccessibilityServiceCallbacks
        : public ElRefBase
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
    private:
        AccessibilityService* mHost;
    };

public:
    CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

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
     * Gets the root node in the currently active window if this service
     * can retrieve window content.
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
     * Gets the an {@link AccessibilityServiceInfo} describing this
     * {@link AccessibilityService}. This method is useful if one wants
     * to change some of the dynamically configurable properties at
     * runtime.
     *
     * @return The accessibility service info.
     *
     * @see AccessibilityNodeInfo
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

#endif // __ELASTOS_DROID_ACCESSIBILITYSERVICE_CACCESSIBILITYSERVICE_H__
