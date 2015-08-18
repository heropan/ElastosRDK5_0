#ifndef __ELASTOS_DROID_VIEW_ACCESSIBILITY_CACCESSIBILITYMANAGER_H__
#define __ELASTOS_DROID_VIEW_ACCESSIBILITY_CACCESSIBILITYMANAGER_H__

#include "ext/frameworkdef.h"
#include "_Elastos_Droid_View_Accessibility_CAccessibilityManager.h"
#include "os/HandlerBase.h"
#include <elastos/utility/etl/List.h>

using Elastos::Utility::Etl::List;
using Elastos::Droid::View::IIWindow;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Os::HandlerBase;

namespace Elastos {
namespace Droid {
namespace View {
namespace Accessibility {

CarClass(CAccessibilityManager)
{
private:
    class MyHandler : public HandlerBase
    {
    public:
        MyHandler(
            /* [in] */ CAccessibilityManager* host)
            : mHost(host)
        {}

        CARAPI HandleMessage(
            /* [in] */ IMessage* msg);
    private:
        CAccessibilityManager* mHost;
    };

public:
    CAccessibilityManager();

    CARAPI_(void) HandleDoSetState(
        /* [in] */ Int32 state);

    /**
     * Creates the singleton AccessibilityManager to be shared across users. This
     * has to be called before the local AccessibilityManager is created to ensure
     * it registers itself in the system correctly.
     * <p>
     * Note: Calling this method requires INTERACT_ACROSS_USERS_FULL or
     *       INTERACT_ACROSS_USERS permission.
     * </p>
     * @param context Context in which this manager operates.
     * @throws IllegalStateException if not called before the local
     *     AccessibilityManager is instantiated.
     *
     * @hide
     */
    static CARAPI CreateAsSharedAcrossUsers(
        /* [in] */ IContext* context);

    /**
     * Get an AccessibilityManager instance (create one if necessary).
     *
     * @param context Context in which this manager operates.
     *
     * @hide
     */
    static CARAPI GetInstance(
        /* [in] */ IContext* context,
        /* [out] */ IAccessibilityManager** manager);

    CARAPI IsEnabled(
        /* [out] */ Boolean* result);

    /**
     * Returns if the touch exploration in the system is enabled.
     *
     * @return True if touch exploration is enabled, false otherwise.
     */
    CARAPI IsTouchExplorationEnabled(
        /* [out] */ Boolean* result);

    /**
     * Returns the client interface this instance registers in
     * the centralized accessibility manager service.
     *
     * @return The client.
     *
     * @hide
     */
    CARAPI_(AutoPtr<IAccessibilityManagerClient>) GetClient();

    /**
     * Sends an {@link AccessibilityEvent}.
     *
     * @param event The event to send.
     *
     * @throws IllegalStateException if accessibility is not enabled.
     *
     * <strong>Note:</strong> The preferred mechanism for sending custom accessibility
     * events is through calling
     * {@link android.view.ViewParent#requestSendAccessibilityEvent(View, AccessibilityEvent)}
     * instead of this method to allow predecessors to augment/filter events sent by
     * their descendants.
     */
    CARAPI SendAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

    /**
     * Requests feedback interruption from all accessibility services.
     */
    CARAPI Interrupt();

    /**
     * Returns the {@link ServiceInfo}s of the installed accessibility services.
     *
     * @return An unmodifiable list with {@link ServiceInfo}s.
     *
     * @deprecated Use {@link #getInstalledAccessibilityServiceList()}
     */
    // @Deprecated
    CARAPI GetAccessibilityServiceList(
        /* [out] */ IObjectContainer** serviceList);

    /**
     * Returns the {@link AccessibilityServiceInfo}s of the installed accessibility services.
     *
     * @return An unmodifiable list with {@link AccessibilityServiceInfo}s.
     */
    CARAPI GetInstalledAccessibilityServiceList(
        /* [out] */ IObjectContainer** serviceList);

    /**
     * Returns the {@link AccessibilityServiceInfo}s of the enabled accessibility services
     * for a given feedback type.
     *
     * @param feedbackTypeFlags The feedback type flags.
     * @return An unmodifiable list with {@link AccessibilityServiceInfo}s.
     *
     * @see AccessibilityServiceInfo#FEEDBACK_AUDIBLE
     * @see AccessibilityServiceInfo#FEEDBACK_GENERIC
     * @see AccessibilityServiceInfo#FEEDBACK_HAPTIC
     * @see AccessibilityServiceInfo#FEEDBACK_SPOKEN
     * @see AccessibilityServiceInfo#FEEDBACK_VISUAL
     */
    CARAPI GetEnabledAccessibilityServiceList(
        /* [in] */ Int32 feedbackTypeFlags,
        /* [out] */ IObjectContainer** serviceList);

    /**
     * Registers an {@link AccessibilityStateChangeListener} for changes in
     * the global accessibility state of the system.
     *
     * @param listener The listener.
     * @return True if successfully registered.
     */
    CARAPI AddAccessibilityStateChangeListener(
        /* [in] */ IAccessibilityStateChangeListener* listener,
        /* [out] */ Boolean* add);

    /**
     * Unregisters an {@link AccessibilityStateChangeListener}.
     *
     * @param listener The listener.
     * @return True if successfully unregistered.
     */
    CARAPI RemoveAccessibilityStateChangeListener(
        /* [in] */ IAccessibilityStateChangeListener* listener,
        /* [out] */ Boolean* remove);

    /**
     * Adds an accessibility interaction connection interface for a given window.
     * @param windowToken The window token to which a connection is added.
     * @param connection The connection.
     *
     * @hide
     */
    CARAPI AddAccessibilityInteractionConnection(
        /* [in] */ IIWindow* windowToken,
        /* [in] */ IAccessibilityInteractionConnection* connection,
        /* [out] */ Int32* add);

    /**
     * Removed an accessibility interaction connection interface for a given window.
     * @param windowToken The window token to which a connection is removed.
     *
     * @hide
     */
    CARAPI RemoveAccessibilityInteractionConnection(
        /* [in] */ IIWindow* windowToken);

    CARAPI constructor(
        /* [in] */ IContext* base,
        /* [in] */ IIAccessibilityManager* service,
        /* [in] */ Int32 userId);

private:
    /**
     * Creates the singleton instance.
     *
     * @param context Context in which this manager operates.
     * @param userId The user id under which to operate.
     */
    static CARAPI CreateSingletonInstance(
        /* [in] */ IContext* context,
        /* [in] */ Int32 userId);

    /**
     * Sets the current state.
     *
     * @param stateFlags The state flags.
     */
    CARAPI_(void) SetState(
        /* [in] */ Int32 stateFlags);

    /**
     * Sets the enabled state.
     *
     * @param isEnabled The accessibility state.
     */
    CARAPI_(void) SetAccessibilityState(
        /* [in] */ Boolean isEnabled);

    /**
     * Notifies the registered {@link AccessibilityStateChangeListener}s.
     */
    CARAPI_(void) NotifyAccessibilityStateChanged();

public:
    /** @hide */
    static const Int32 STATE_FLAG_ACCESSIBILITY_ENABLED;

    /** @hide */
    static const Int32 STATE_FLAG_TOUCH_EXPLORATION_ENABLED;

    static AutoPtr<IAccessibilityManager> sInstance;
    static Object sInstanceSync;

    static const Int32 DO_SET_STATE;

    AutoPtr<IIAccessibilityManager> mService;

    Int32 mUserId;

    AutoPtr<IHandler> mHandler;
    Object mHandlerLock;

    Boolean mIsEnabled;

    Boolean mIsTouchExplorationEnabled;

    // CopyOnWriteArrayList<AutoPtr<IAccessibilityStateChangeListener> > mAccessibilityStateChangeListeners =
    //     new CopyOnWriteArrayList<AccessibilityStateChangeListener>();
    List<AutoPtr<IAccessibilityStateChangeListener> > mAccessibilityStateChangeListeners;

    AutoPtr<IAccessibilityManagerClient> mClient;

private:
    static const Boolean DEBUG;

    static const String localLOG_TAG;
};

} // Accessibility
} // View
} // Droid
} // Elastos

#endif //__ELASTOS_DROID_VIEW_ACCESSIBILITY_CACCESSIBILITYMANAGER_H__
