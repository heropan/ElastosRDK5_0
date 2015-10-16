
#ifndef __ELASTOS_DROID_SERVER_DISPLAY_CDISPLAYMANAGERSERVICE_H__
#define __ELASTOS_DROID_SERVER_DISPLAY_CDISPLAYMANAGERSERVICE_H__

#include "elastos/droid/ext/frameworkdef.h"
#include "_Elastos_Droid_Server_Display_CDisplayManagerService.h"
#include "display/DisplayAdapter.h"
#include "display/DisplayDevice.h"
#include "display/DisplayViewport.h"
#include "display/WifiDisplayAdapter.h"
#include "display/LogicalDisplay.h"
#include "display/PersistentDataStore.h"
#include "elastos/droid/os/HandlerBase.h"
#include <elastos/utility/etl/HashMap.h>
#include <elastos/Core/Object.h>

using Elastos::Utility::Etl::HashMap;
using Elastos::Core::Object;
using Elastos::IO::IFileDescriptor;
using Elastos::Droid::Hardware::Display::IDisplayManagerCallback;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Display {

/**
 * Manages attached displays.
 * <p>
 * The {@link DisplayManagerService} manages the global lifecycle of displays,
 * decides how to configure logical displays based on the physical display devices currently
 * attached, sends notifications to the system and to applications when the state
 * changes, and so on.
 * </p><p>
 * The display manager service relies on a collection of {@link DisplayAdapter} components,
 * for discovering and configuring physical display devices attached to the system.
 * There are separate display adapters for each manner that devices are attached:
 * one display adapter for built-in local displays, one for simulated non-functional
 * displays when the system is headless, one for simulated overlay displays used for
 * development, one for wifi displays, etc.
 * </p><p>
 * Display adapters are only weakly coupled to the display manager service.
 * Display adapters communicate changes in display device state to the display manager
 * service asynchronously via a {@link DisplayAdapter.Listener} registered
 * by the display manager service.  This separation of concerns is important for
 * two main reasons.  First, it neatly encapsulates the responsibilities of these
 * two classes: display adapters handle individual display devices whereas
 * the display manager service handles the global state.  Second, it eliminates
 * the potential for deadlocks resulting from asynchronous display device discovery.
 * </p>
 *
 * <h3>Synchronization</h3>
 * <p>
 * Because the display manager may be accessed by multiple threads, the synchronization
 * story gets a little complicated.  In particular, the window manager may call into
 * the display manager while holding a surface transaction with the expectation that
 * it can apply changes immediately.  Unfortunately, that means we can't just do
 * everything asynchronously (*grump*).
 * </p><p>
 * To make this work, all of the objects that belong to the display manager must
 * use the same lock.  We call this lock the synchronization root and it has a unique
 * type {@link DisplayManagerService.SyncRoot}.  Methods that require this lock are
 * named with the "Locked" suffix.
 * </p><p>
 * Where things get tricky is that the display manager is not allowed to make
 * any potentially reentrant calls, especially into the window manager.  We generally
 * aCARAPI_(void) this by making all potentially reentrant out-calls asynchronous.
 * </p>
 */
CarClass(CDisplayManagerService)
{
private:

    class DisplayManagerHandler
        : public HandlerBase
    {
    public:
        DisplayManagerHandler(
            /* [in] */ ILooper* looper,
            /* [in] */ CDisplayManagerService* host);

        virtual CARAPI HandleMessage(
            /* [in] */ IMessage* msg);

        static String CodeToString(
            /* [in] */ Int32 what);
    private:
        CDisplayManagerService* mHost;
    };

    class DisplayAdapterListener
        : public ElRefBase
        , public IDisplayAdapterListener
    {
    public:
        CAR_INTERFACE_DECL()

        DisplayAdapterListener(
            /* [in] */ CDisplayManagerService* owner);

        //@Override
        CARAPI OnDisplayDeviceEvent(
            /* [in] */ Handle32 device,
            /* [in] */ Int32 event);

        //@Override
        CARAPI OnTraversalRequested();

    private:
        CDisplayManagerService* mHost;
    };

    class CallbackRecord
        : public ElRefBase
        , public IProxyDeathRecipient
    {
    public:
        CAR_INTERFACE_DECL();

        CallbackRecord(
            /* [in] */ CDisplayManagerService* owner,
            /* [in] */ Int32 pid,
            /* [in] */ IDisplayManagerCallback* callback);

        ~CallbackRecord();

        //@Override
        CARAPI ProxyDied();

        CARAPI_(void) NotifyDisplayEventAsync(
            /* [in] */ Int32 displayId,
            /* [in] */ Int32 event);

    private:
        CDisplayManagerService* mHost;
        const Int32 mPid;
        AutoPtr<IDisplayManagerCallback> mCallback;
    };

public:
    CDisplayManagerService();

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IHandler* mainHandler,
        /* [in] */ IHandler* uiHandler);

    /**
     * Pauses the boot process to wait for the first display to be initialized.
     */
    CARAPI WaitForDefaultDisplay(
        /* [out] */ Boolean* res);

    /**
     * Called during initialization to associate the display manager with the
     * window manager.
     */
    CARAPI SetWindowManager(
        /* [in] */ IDisplayManagerServiceWindowManagerFuncs* windowManagerFuncs);

    /**
     * Called during initialization to associate the display manager with the
     * input manager.
     */
    CARAPI SetInputManager(
        /* [in] */ IDisplayManagerServiceInputManagerFuncs* inputManagerFuncs);

    /**
     * Called when the system is ready to go.
     */
    CARAPI SystemReady(
        /* [in] */ Boolean safeMode,
        /* [in] */ Boolean onlyCore);

    /**
     * Returns TRUE if the device is headless.
     *
     * @return True if the device is headless.
     */
    CARAPI IsHeadless(
        /* [out] */ Boolean* res);

    /**
     * Registers a display transaction listener to provide the client a chance to
     * update its surfaces within the same transaction as any display layout updates.
     *
     * @param listener The listener to register.
     */
    CARAPI RegisterDisplayTransactionListener(
        /* [in] */ IDisplayTransactionListener* listener);

    /**
     * Unregisters a display transaction listener to provide the client a chance to
     * update its surfaces within the same transaction as any display layout updates.
     *
     * @param listener The listener to unregister.
     */
    CARAPI UnregisterDisplayTransactionListener(
        /* [in] */ IDisplayTransactionListener* listener);

    /**
     * Overrides the display information of a particular logical display.
     * This is used by the window manager to control the size and characteristics
     * of the default display.  It is expected to apply the requested change
     * to the display information synchronously so that applications will immediately
     * observe the new state.
     *
     * @param displayId The logical display id.
     * @param info The new data to be stored.
     */
    CARAPI SetDisplayInfoOverrideFromWindowManager(
        /* [in] */ Int32 displayId,
        /* [in] */ IDisplayInfo* info);

    /**
     * Called by the window manager to perform traversals while holding a
     * surface flinger transaction.
     */
    CARAPI PerformTraversalInTransactionFromWindowManager();

    /**
     * Called by the power manager to blank all displays.
     */
    CARAPI BlankAllDisplaysFromPowerManager();

    /**
     * Called by the power manager to unblank all displays.
     */
    CARAPI UnblankAllDisplaysFromPowerManager();

    /**
     * Returns information about the specified logical display.
     *
     * @param displayId The logical display id.
     * @return The logical display info, or NULL if the display does not exist.  The
     * returned object must be treated as immutable.
     */
    //@Override // Binder call
    CARAPI GetDisplayInfo(
        /* [in] */ Int32 displayId,
        /* [out] */ IDisplayInfo** displayInfo);

    /**
     * Returns the list of all display ids.
     */
    //@Override // Binder call
    CARAPI GetDisplayIds(
        /* [out, callee] */ ArrayOf<Int32>** displayIds);

    //@Override // Binder call
    CARAPI RegisterCallback(
        /* [in] */ IDisplayManagerCallback* callback);

    //@Override // Binder call
    CARAPI ScanWifiDisplays();

    //@Override // Binder call
    CARAPI ConnectWifiDisplay(
        /* [in] */ const String& address);

    //@Override // Binder call
    CARAPI DisconnectWifiDisplay();

    //@Override // Binder call
    CARAPI RenameWifiDisplay(
        /* [in] */ const String& address,
        /* [in] */ const String& alias);

    //@Override // Binder call
    CARAPI ForgetWifiDisplay(
        /* [in] */ const String& address);

    //@Override // Binder call
    CARAPI GetWifiDisplayStatus(
        /* [out] */ IWifiDisplayStatus** status);

    /**
     * Tells the display manager whether there is interesting unique content on the
     * specified logical display.  This is used to control automatic mirroring.
     * <p>
     * If the display has unique content, then the display manager arranges for it
     * to be presented on a physical display if appropriate.  Otherwise, the display manager
     * may choose to make the physical display mirror some other logical display.
     * </p>
     *
     * @param displayId The logical display id to update.
     * @param hasContent True if the logical display has content.
     * @param inTraversal True if called from WindowManagerService during a window traversal prior
     * to call to performTraversalInTransactionFromWindowManager.
     */
    CARAPI SetDisplayHasContent(
        /* [in] */ Int32 displayId,
        /* [in] */ Boolean hasContent,
        /* [in] */ Boolean inTraversal);

    //@Override // Binder call
    CARAPI Dump(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ IPrintWriter* pw,
        /* [in] */ ArrayOf<String>* args);

private:
    CARAPI_(void) OnCallbackDied(
        /* [in] */ Int32 pid);

    CARAPI_(Boolean) CanCallerConfigureWifiDisplay();

    CARAPI_(void) RegisterDefaultDisplayAdapter();

    CARAPI_(void) RegisterAdditionalDisplayAdapters();

    CARAPI_(void) RegisterOverlayDisplayAdapterLocked();

    CARAPI_(void) RegisterWifiDisplayAdapterLocked();

    CARAPI_(Boolean) ShouldRegisterNonEssentialDisplayAdaptersLocked();

    CARAPI_(void) RegisterDisplayAdapterLocked(
        /* [in] */ DisplayAdapter* adapter);

    CARAPI_(void) HandleDisplayDeviceAdded(
       /* [in] */ DisplayDevice* device);

    CARAPI_(void) HandleDisplayDeviceChanged(
        /* [in] */ DisplayDevice* device);

    CARAPI_(void) HandleDisplayDeviceRemoved(
        /* [in] */ DisplayDevice* device);

    // Adds a new logical display based on the given display device.
    // Sends notifications if needed.
    CARAPI_(void) AddLogicalDisplayLocked(
        /* [in] */ DisplayDevice* device);

    CARAPI_(Int32) AssignDisplayIdLocked(
        /* [in] */ Boolean isDefault);

    CARAPI_(Int32) AssignLayerStackLocked(
        /* [in] */ Int32 displayId);

    // Updates all existing logical displays given the current set of display devices.
    // Removes invalid logical displays.
    // Sends notifications if needed.
    CARAPI_(Boolean) UpdateLogicalDisplaysLocked();

    CARAPI_(void) PerformTraversalInTransactionLocked();

    CARAPI_(void) ClearViewportsLocked();

    CARAPI_(void) ConfigureDisplayInTransactionLocked(
        /* [in] */ DisplayDevice* device);

    static CARAPI_(void) SetViewportLocked(
        /* [in] */ DisplayViewport* viewport,
        /* [in] */ LogicalDisplay* display,
        /* [in] */ DisplayDevice* device);

    CARAPI_(AutoPtr<LogicalDisplay>) FindLogicalDisplayForDeviceLocked(
        /* [in] */ DisplayDevice* device);

    CARAPI_(void) SendDisplayEventLocked(
        /* [in] */ Int32 displayId,
        /* [in] */ Int32 event);

    // Requests that performTraversalsInTransactionFromWindowManager be called at a
    // later time to apply changes to surfaces and displays.
    CARAPI_(void) ScheduleTraversalLocked(
        /* [in] */ Boolean inTraversal);

    // Runs on Handler thread.
    // Delivers display event notifications to callbacks.
    CARAPI_(void) DeliverDisplayEvent(
        /* [in] */ Int32 displayId,
        /* [in] */ Int32 event);

public:
    // True if in safe mode.
    // This option may disable certain display adapters.
    Boolean mSafeMode;

    // True if we are in a special boot mode where only core applications and
    // services should be started.  This option may disable certain display adapters.
    Boolean mOnlyCore;

    // All callback records indexed by calling process id.
    HashMap<Int32, AutoPtr<CallbackRecord> > mCallbacks;

private:
    static const String TAG;
    static const Boolean DEBUG;

    // When this system property is set to 0, WFD is forcibly disabled on boot.
    // When this system property is set to 1, WFD is forcibly enabled on boot.
    // Otherwise WFD is enabled according to the value of config_enableWifiDisplay.
    static const String FORCE_WIFI_DISPLAY_ENABLE;

    static const String SYSTEM_HEADLESS;
    static const Int64 WAIT_FOR_DEFAULT_DISPLAY_TIMEOUT = 10000;

    static const Int32 MSG_REGISTER_DEFAULT_DISPLAY_ADAPTER = 1;
    static const Int32 MSG_REGISTER_ADDITIONAL_DISPLAY_ADAPTERS = 2;
    static const Int32 MSG_DELIVER_DISPLAY_EVENT = 3;
    static const Int32 MSG_REQUEST_TRAVERSAL = 4;
    static const Int32 MSG_UPDATE_VIEWPORT = 5;

    static const Int32 DISPLAY_BLANK_STATE_UNKNOWN = 0;
    static const Int32 DISPLAY_BLANK_STATE_BLANKED = 1;
    static const Int32 DISPLAY_BLANK_STATE_UNBLANKED = 2;

    AutoPtr<IContext> mContext;
    Boolean mHeadless;
    AutoPtr<DisplayManagerHandler> mHandler;
    AutoPtr<IHandler> mUiHandler;
    AutoPtr<DisplayAdapterListener> mDisplayAdapterListener;
    AutoPtr<IDisplayManagerServiceWindowManagerFuncs> mWindowManagerFuncs;
    AutoPtr<IDisplayManagerServiceInputManagerFuncs> mInputManagerFuncs;

    // The synchronization root for the display manager.
    // This lock guards most of the display manager's state.
    Object mSyncRoot;

    // True if the display manager service should pretend there is only one display
    // and only tell applications about the existence of the default logical display.
    // The display manager can still mirror content to secondary displays but applications
    // cannot present unique content on those displays.
    // Used for demonstration purposes only.
    Boolean mSingleDisplayDemoMode;

    // List of all currently registered display adapters.
    List< AutoPtr<DisplayAdapter> > mDisplayAdapters;

    // List of all currently connected display devices.
    List< AutoPtr<DisplayDevice> > mDisplayDevices;

    // List of all removed display devices.
    List< AutoPtr<DisplayDevice> > mRemovedDisplayDevices;

    // List of all logical displays indexed by logical display id.
    HashMap<Int32, AutoPtr<LogicalDisplay> > mLogicalDisplays;
    Int32 mNextNonDefaultDisplayId;

    // List of all display transaction listeners.
    List< AutoPtr<IDisplayTransactionListener> > mDisplayTransactionListeners;
    Object mDisplayTransactionListenersLock;

    // Set to TRUE if all displays have been blanked by the power manager.
    Int32 mAllDisplayBlankStateFromPowerManager;

    // Set to TRUE when there are pending display changes that have yet to be applied
    // to the surface flinger state.
    Boolean mPendingTraversal;

    // The Wifi display adapter, or NULL if not registered.
    AutoPtr<WifiDisplayAdapter> mWifiDisplayAdapter;

    // Viewports of the default display and the display that should receive touch
    // input from an external source.  Used by the input system.
    AutoPtr<DisplayViewport> mDefaultViewport;
    AutoPtr<DisplayViewport> mExternalTouchViewport;

    // Persistent data store for all internal settings maintained by the display manager service.
    AutoPtr<PersistentDataStore> mPersistentDataStore;

    // Temporary callback list, used when sending display events to applications.
    // May be used outside of the lock but only on the handler thread.
    List< AutoPtr<CallbackRecord> > mTempCallbacks;

    // Temporary display info, used for comparing display configurations.
    AutoPtr<IDisplayInfo> mTempDisplayInfo;

    // Temporary viewports, used when sending new viewport information to the
    // input system.  May be used outside of the lock but only on the handler thread.
    AutoPtr<DisplayViewport> mTempDefaultViewport;
    AutoPtr<DisplayViewport> mTempExternalTouchViewport;
};

} // namespace Display
} // namespace Server
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_SERVER_DISPLAY_CDISPLAYMANAGERSERVICE_H__
