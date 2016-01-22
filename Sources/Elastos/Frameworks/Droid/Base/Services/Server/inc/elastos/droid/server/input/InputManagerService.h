#ifndef __ELASTOS_DROID_Service_Input_input_InputManagerService_H__
#define __ELASTOS_DROID_Service_Input_input_InputManagerService_H__

#include "_Elastos.Droid.Server.h"
#include "elastos/core/Object.h"
//#include "elastos/droid/app/CPendingIntent.h"
#include "elastos/droid/os/Handler.h"
#include "elastos/droid/server/input/PersistentDataStore.h"
#include "elastos/droid/server/input/InputApplicationHandle.h"
#include "elastos/droid/server/input/InputWindowHandle.h"
#include "elastos/droid/hardware/input/InputDeviceIdentifier.h"
#include "elastos/droid/widget/Toast.h"
#include "Elastos.Droid.Utility.h"
#include "Elastos.Droid.Text.h"
#include <Elastos.CoreLibrary.Utility.h>

using Elastos::Droid::App::INotificationManager;
using Elastos::Droid::App::IPendingIntent;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::Pm::IActivityInfo;
using Elastos::Droid::Content::Pm::IPackageManager;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Hardware::Display::IDisplayViewport;
using Elastos::Droid::Hardware::Input::IInputDevicesChangedListener;
using Elastos::Droid::Hardware::Input::IKeyboardLayout;
using Elastos::Droid::Hardware::Input::ITouchCalibration;
using Elastos::Droid::Hardware::Input::IInputDevicesChangedListener;
using Elastos::Droid::Hardware::Input::IInputDeviceIdentifier;
using Elastos::Droid::Hardware::Input::IIInputManager;
using Elastos::Droid::Os::IBinder;
using Elastos::Droid::Os::Handler;
using Elastos::Droid::Os::IHandler;
using Elastos::Droid::Os::IMessageQueue;
using Elastos::Droid::Text::IInputFilter;
using Elastos::Droid::Utility::ISparseArray;
using Elastos::Droid::View::IInputDevice;
using Elastos::Droid::View::IInputEvent;
using Elastos::Droid::View::IInputEvent;
using Elastos::Droid::View::IInputFilterHost;
using Elastos::Droid::View::IInputDevice;
using Elastos::Droid::View::IPointerIcon;
using Elastos::Droid::View::IInputChannel;
using Elastos::Droid::View::IKeyEvent;
using Elastos::Droid::Widget::IToast;
using Elastos::IO::IFileDescriptor;
using Elastos::IO::IPrintWriter;

using Elastos::Droid::Server::Input::IInputManagerService;
using Elastos::Droid::Server::Input::IWindowManagerCallbacks;
using Elastos::Droid::Server::Input::IWiredAccessoryCallbacks;
using Elastos::Droid::Server::Input::IKeyboardLayoutVisitor;
using Elastos::Droid::Server::Input::IInputWindowHandle;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Input {

/*
 * Wraps the C++ InputManager and provides its callbacks.
 * public class InputManagerService extends IInputManager.Stub
 *       implements Watchdog.Monitor {
 */
class InputManagerService
    : public Object
    , public IIInputManager
    , public IInputManagerService
    , public IBinder
{
public:
    /**
     * Hosting interface for input filters to call back into the input manager.
     */
    //  extends IInputFilterHost.Stub
    class InputFilterHost
        : public Object
        , public IInputFilterHost
        , public IBinder
    {
    public:
        CAR_INTERFACE_DECL();

        InputFilterHost();

        virtual ~InputFilterHost();

        CARAPI constructor();

        CARAPI constructor(
            /* [in] */ IIInputManager* parent);

        CARAPI DisconnectLocked();

        CARAPI SendInputEvent(
            /* [in] */ IInputEvent* event,
            /* [in] */ Int32 policyFlags);

        CARAPI ToString(
            /* [out] */ String* str);

    public:
        Boolean mDisconnected;

    private:
        InputManagerService* mHost;
    };

    // implements DeathRecipient
    class VibratorToken
        : public Object
    {
    public:
        Int32 mDeviceId;
        AutoPtr<IBinder> mToken;
        Int32 mTokenValue;

        Boolean mVibrating;

        VibratorToken(
            /* [in] */ Int32 deviceId,
            /* [in] */ IBinder* token,
            /* [in] */ Int32 tokenValue);

        //@Override
        CARAPI BinderDied();
    };

    // extends InputManagerInternal
    class LocalService
        : public Object
    {
    public:
        LocalService(
            /* [in] */ InputManagerService* parent);

        //@Override
        void SetDisplayViewports(
            /* [in] */ IDisplayViewport* defaultViewport,
            /* [in] */ IDisplayViewport* externalTouchViewport);

        Boolean InjectInputEvent(
            /* [in] */ IInputEvent* event,
            /* [in] */ Int32 displayId,
            /* [in] */ Int32 mode);

        //@Override
        void SetInteractive(
            /* [in] */ Boolean interactive);

    private:
        InputManagerService* mHost;
    };


    /**
     * Private handler for the input manager.
     */
    class InputManagerHandler
        : public Handler
    {
    public:
        InputManagerHandler(
            /* [in] */ ILooper* looper,
            /* [in] */ InputManagerService* parent);

        //@Override
        CARAPI HandleMessage(
            /* [in] */ IMessage* msg);

    private:
        AutoPtr<IHandler> superHandler;
        InputManagerService* mHost;
    };


    //private static final
    class KeyboardLayoutDescriptor
        : public Object
    {
    public:
        String mPackageName;
        String mReceiverName;
        String mKeyboardLayoutName;

        static String Format(
                    /* [in] */ const String& packageName,
                    /* [in] */ const String& receiverName,
                    /* [in] */ const String& keyboardName);

        static KeyboardLayoutDescriptor* parse(
                    /* [in] */ const String& descriptor);
    };

    //  implements DeathRecipient
    class InputDevicesChangedListenerRecord
        : public Object
    {
    public:
        InputDevicesChangedListenerRecord(
            /* [in] */ Int32 pid,
            /* [in] */ IInputDevicesChangedListener* listener,
            /* [in] */ InputManagerService* parent);

        //@Override
        void BinderDied();

        NotifyInputDevicesChanged(
                    ArrayOf<Int32>* info);
    private:
        Int32 mPid;
        AutoPtr<IInputDevicesChangedListener> mListener;

    private:
        InputManagerService* mHost;
    };

public:
    CAR_INTERFACE_DECL();

    InputManagerService();

    virtual ~InputManagerService();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI SetWindowManagerCallbacks(
        /* [in] */ IWindowManagerCallbacks* cbacks);

    CARAPI SetWiredAccessoryCallbacks(
        /* [in] */ IWiredAccessoryCallbacks* cbacks);

    CARAPI Start();

    // TODO(BT) Pass in paramter for bluetooth system
    CARAPI SystemRunning();

    /**
     * Gets the current state of a key or button by key code.
     * @param deviceId The input device id, or -1 to consult all devices.
     * @param sourceMask The input sources to consult, or {@link InputDevice#SOURCE_ANY} to
     * consider all input sources.  An input device is consulted if at least one of its
     * non-class input source bits matches the specified source mask.
     * @param keyCode The key code to check.
     * @return The key state.
     */
    CARAPI GetKeyCodeState(
        /* [in] */ Int32 deviceId,
        /* [in] */ Int32 sourceMask,
        /* [in] */ Int32 keyCode,
        /* [out] */ Int32 *ret);

    /**
     * Gets the current state of a key or button by scan code.
     * @param deviceId The input device id, or -1 to consult all devices.
     * @param sourceMask The input sources to consult, or {@link InputDevice#SOURCE_ANY} to
     * consider all input sources.  An input device is consulted if at least one of its
     * non-class input source bits matches the specified source mask.
     * @param scanCode The scan code to check.
     * @return The key state.
     */
    CARAPI GetScanCodeState(
        /* [in] */ Int32 deviceId,
        /* [in] */ Int32 sourceMask,
        /* [in] */ Int32 scanCode,
        /* [out] */ Int32 *ret);

    /**
     * Gets the current state of a switch by switch code.
     * @param deviceId The input device id, or -1 to consult all devices.
     * @param sourceMask The input sources to consult, or {@link InputDevice#SOURCE_ANY} to
     * consider all input sources.  An input device is consulted if at least one of its
     * non-class input source bits matches the specified source mask.
     * @param switchCode The switch code to check.
     * @return The switch state.
     */
    CARAPI GetSwitchState(
        /* [in] */ Int32 deviceId,
        /* [in] */ Int32 sourceMask,
        /* [in] */ Int32 switchCode,
        /* [out] */ Int32 *ret);

    /**
     * Determines whether the specified key codes are supported by a particular device.
     * @param deviceId The input device id, or -1 to consult all devices.
     * @param sourceMask The input sources to consult, or {@link InputDevice#SOURCE_ANY} to
     * consider all input sources.  An input device is consulted if at least one of its
     * non-class input source bits matches the specified source mask.
     * @param keyCodes The array of key codes to check.
     * @param keyExists An array at least as large as keyCodes whose entries will be set
     * to true or false based on the presence or absence of support for the corresponding
     * key codes.
     * @return True if the lookup was successful, false otherwise.
     */
    // @Override // Binder call
    CARAPI HasKeys(
        /* [in] */ Int32 deviceId,
        /* [in] */ Int32 sourceMask,
        /* [in] */ const ArrayOf<Int32>& keyCodes,
        /* [in] */ ArrayOf<Boolean>* keyExists,
        /* [out] */ Boolean* hasKeys);

    /**
     * Creates an input channel that will receive all input from the input dispatcher.
     * @param inputChannelName The input channel name.
     * @return The input channel.
     */
    CARAPI MonitorInput(
        /* [in] */ String inputChannelName,
        /* [in] */ IInputChannel* ic);

    /**
     * Registers an input channel so that it can be used as an input event target.
     * @param inputChannel The input channel to register.
     * @param inputWindowHandle The handle of the input window associated with the
     * input channel, or null if none.
     */
    CARAPI RegisterInputChannel(
        /* [in] */ IInputChannel* inputChannel,
        /* [in] */ InputWindowHandle* inputWindowHandle);

    /**
     * Unregisters an input channel.
     * @param inputChannel The input channel to unregister.
     */
    CARAPI UnregisterInputChannel(
        /* [in] */ IInputChannel* inputChannel);

    /**
     * Sets an input filter that will receive all input events before they are dispatched.
     * The input filter may then reinterpret input events or inject new ones.
     *
     * To ensure consistency, the input dispatcher automatically drops all events
     * in progress whenever an input filter is installed or uninstalled.  After an input
     * filter is uninstalled, it can no longer send input events unless it is reinstalled.
     * Any events it attempts to send after it has been uninstalled will be dropped.
     *
     * @param filter The input filter, or null to remove the current filter.
     */
    CARAPI SetInputFilter(
        /* [in] */ IInputFilter* filter);

    //// @Override // Binder call
    CARAPI InjectInputEvent(
        /* [in] */ IInputEvent* event,
        /* [in] */ Int32 mode,
        /* [out] */ Boolean* injectIt);

    /**
     * Gets information about the input device with the specified id.
     * @param deviceId The device id.
     * @return The input device or null if not found.
     */
    //// @Override // Binder call
    CARAPI GetInputDevice(
        /* [in] */ Int32 deviceId,
        /* [out, callee] */ IInputDevice** inputDevice);

    /**
     * Gets the ids of all input devices in the system.
     * @return The input device ids.
     */
    //// @Override // Binder call
    CARAPI GetInputDeviceIds(
        /* [out, callee] */ ArrayOf<Int32>** deviceIds);

    /**
     * Gets all input devices in the system.
     * @return The array of input devices.
     */
    CARAPI GetInputDevices(
        /* [out] */ ArrayOf<IInputDevice*>* inputDevices );

    // @Override // Binder call
    CARAPI RegisterInputDevicesChangedListener(
        /* [in] */ IInputDevicesChangedListener* listener);

    // @Override // Binder call & native callback
    CARAPI GetTouchCalibrationForInputDevice(
        /* [in] */ const String& inputDeviceDescriptor,
        /* [in] */ Int32 surfaceRotation,
        /* [out] */ ITouchCalibration** inputDevice);

    // @Override // Binder call
    CARAPI SetTouchCalibrationForInputDevice(
        /* [in] */ const String& inputDeviceDescriptor,
        /* [in] */ Int32 surfaceRotation,
        /* [in] */ ITouchCalibration* calibration);

    // @Override // Binder call
    CARAPI GetKeyboardLayouts(
        /* [out, callee] */ ArrayOf<IKeyboardLayout*>** kbLayouts);

    // @Override // Binder call
    CARAPI GetKeyboardLayout(
        /* [in] */ const String& keyboardLayoutDescriptor,
        /* [out, callee] */ IKeyboardLayout** kbLayout);

    // @Override // Binder call
    CARAPI GetCurrentKeyboardLayoutForInputDevice(
        /* [in] */ IInputDeviceIdentifier* identifier,
        /* [out, callee] */ String* kbLayout);

    // @Override // Binder call
    CARAPI SetCurrentKeyboardLayoutForInputDevice(
        /* [in] */ IInputDeviceIdentifier* identifier,
        /* [in] */ const String& keyboardLayoutDescriptor);

    // @Override // Binder call
    CARAPI GetKeyboardLayoutsForInputDevice(
        /* [in] */ IInputDeviceIdentifier* identifier,
        /* [out, callee] */ ArrayOf<String>** kbLayouts);

    // @Override // Binder call
    CARAPI AddKeyboardLayoutForInputDevice(
        /* [in] */ IInputDeviceIdentifier* identifier,
        /* [in] */ const String& keyboardLayoutDescriptor);

    // @Override // Binder call
    CARAPI RemoveKeyboardLayoutForInputDevice(
        /* [in] */ IInputDeviceIdentifier* identifier,
        /* [in] */ const String& keyboardLayoutDescriptor);

    CARAPI SwitchKeyboardLayout(
        /* [in] */ Int32 deviceId,
        /* [in] */ Int32 direction);

    CARAPI SetInputWindows(
        /* [in] */ ArrayOf<InputWindowHandle*> windowHandles);

    CARAPI SetFocusedApplication(
        /* [in] */ InputApplicationHandle* application);

    CARAPI SetInputDispatchMode(
        /* [in] */ Boolean enabled,
        /* [in] */ Boolean frozen);

    CARAPI SetSystemUiVisibility(
        /* [in] */ Int32 visibility);

    /**
     * Atomically transfers touch focus from one window to another as identified by
     * their input channels.  It is possible for multiple windows to have
     * touch focus if they support split touch dispatch
     * {@link android.view.WindowManager.LayoutParams#FLAG_SPLIT_TOUCH} but this
     * method only transfers touch focus of the specified window without affecting
     * other windows that may also have touch focus at the same time.
     * @param fromChannel The channel of a window that currently has touch focus.
     * @param toChannel The channel of the window that should receive touch focus in
     * place of the first.
     * @return True if the transfer was successful.  False if the window with the
     * specified channel did not actually have touch focus at the time of the request.
     */
    CARAPI TransferTouchFocus(
        /* [in] */ IInputChannel* fromChannel,
        /* [in] */ IInputChannel* toChannel,
        /* [out] */ Boolean* transferIt);

    // @Override // Binder call
    CARAPI TryPointerSpeed(
        /* [in] */ Int32 speed);

    CARAPI UpdatePointerSpeedFromSettings();

    CARAPI UpdateShowTouchesFromSettings();

    // Binder call
    // @Override
    CARAPI Vibrate(
        /* [in] */ Int32 deviceId,
        /* [in] */ const ArrayOf<Int64>& pattern,
        /* [in] */ Int32 repeat,
        /* [in] */ IBinder* token);

    // Binder call
    // @Override
    CARAPI CancelVibrate(
        /* [in] */ Int32 deviceId,
        /* [in] */ IBinder* token);

    // @Override
    CARAPI Dump(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ IPrintWriter* pw,
        /* [in] */ ArrayOf<String>* args);

    // Called by the heartbeat to ensure locks are not held indefinitely (for deadlock detection).
    // @Override
    CARAPI Monitor();

    CARAPI ToString(
        /* [out] */ String* str);

public:

    const static String TAG;
    const static Boolean DEBUG = FALSE;

    const static String EXCLUDED_DEVICES_PATH;

    const static Int32 MSG_DELIVER_INPUT_DEVICES_CHANGED = 1;
    const static Int32 MSG_SWITCH_KEYBOARD_LAYOUT = 2;
    const static Int32 MSG_RELOAD_KEYBOARD_LAYOUTS = 3;
    const static Int32 MSG_UPDATE_KEYBOARD_LAYOUTS = 4;
    const static Int32 MSG_RELOAD_DEVICE_ALIASES = 5;

    // Pointer to native input manager service object.
    Int64 mPtr;

    IContext* mContext;
    InputManagerHandler* mHandler;

    IWindowManagerCallbacks* mWindowManagerCallbacks;
    IWiredAccessoryCallbacks* mWiredAccessoryCallbacks;
    Boolean mSystemReady;
    INotificationManager* mNotificationManager;

    // Persistent data store.  Must be locked each time during use.
    PersistentDataStore* mDataStore;    // = new PersistentDataStore();

    // List of currently registered input devices changed listeners by process id.
    IInterface *mInputDevicesLock;              // = new Object();
    Boolean mInputDevicesChangedPending;        // guarded by mInputDevicesLock
    ArrayOf<IInputDevice*>* mInputDevices;      // = new InputDevice[0];

    // SparseArray<InputDevicesChangedListenerRecord> mInputDevicesChangedListeners =
    //        new SparseArray<InputDevicesChangedListenerRecord>(); // guarded by mInputDevicesLock
    AutoPtr<ISparseArray> mInputDevicesChangedListeners;

    //final ArrayList<InputDevicesChangedListenerRecord>
    //        mTempInputDevicesChangedListenersToNotify =
    //                new ArrayList<InputDevicesChangedListenerRecord>(); // handler thread only
    AutoPtr<IArrayList> mTempInputDevicesChangedListenersToNotify;

    //final ArrayList<InputDevice>
    //        mTempFullKeyboards = new ArrayList<InputDevice>(); // handler thread only
    AutoPtr<IArrayList> mTempFullKeyboards;

    Boolean mKeyboardLayoutNotificationShown;
    AutoPtr<IPendingIntent> mKeyboardLayoutIntent;
    AutoPtr<IToast> mSwitchedKeyboardLayoutToast;

    // State for vibrator tokens.
    AutoPtr<IInterface> mVibratorLock;      // = new Object();
    HashMap<IBinder *, VibratorToken *> mVibratorTokens;    // = new HashMap<IBinder, VibratorToken>();
    Int32 mNextVibratorTokenValue;

    // State for the currently installed input filter.
    Object mInputFilterLock;                    // = new Object();
    AutoPtr<IInputFilter> mInputFilter;         // guarded by mInputFilterLock
    AutoPtr<InputFilterHost> mInputFilterHost;  // guarded by mInputFilterLock

    static Int64 nativeInit(
                    InputManagerService* service,
                    IContext* context,
                    IMessageQueue* messageQueue);
    static void nativeStart(Int64 ptr);
    static void nativeSetDisplayViewport(Int64 ptr, Boolean external,
                    Int32 displayId, Int32 rotation,
                    Int32 logicalLeft, Int32 logicalTop, Int32 logicalRight, Int32 logicalBottom,
                    Int32 physicalLeft, Int32 physicalTop, Int32 physicalRight, Int32 physicalBottom,
                    Int32 deviceWidth, Int32 deviceHeight);

    static Int32 nativeGetScanCodeState(Int64 ptr,
                    Int32 deviceId, Int32 sourceMask, Int32 scanCode);
    static Int32 nativeGetKeyCodeState(Int64 ptr,
                    Int32 deviceId, Int32 sourceMask, Int32 keyCode);
    static Int32 nativeGetSwitchState(Int64 ptr,
                    Int32 deviceId, Int32 sourceMask, Int32 sw);
    static Boolean nativeHasKeys(Int64 ptr,
                    Int32 deviceId, Int32 sourceMask, ArrayOf<Int32>* keyCodes, ArrayOf<Boolean> keyExists);
    static void nativeRegisterInputChannel(Int64 ptr, IInputChannel* inputChannel,
                    InputWindowHandle* inputWindowHandle, Boolean monitor);
    static void nativeUnregisterInputChannel(Int64 ptr, IInputChannel* inputChannel);
    static void nativeSetInputFilterEnabled(Int64 ptr, Boolean enable);
    static Int32 nativeInjectInputEvent(Int64 ptr, IInputEvent* event, Int32 displayId,
                    Int32 injectorPid, Int32 injectorUid, Int32 syncMode, Int32 timeoutMillis,
                    Int32 policyFlags);
    static void nativeSetInputWindows(Int64 ptr, ArrayOf<InputWindowHandle> windowHandles);
    static void nativeSetInputDispatchMode(Int64 ptr, Boolean enabled, Boolean frozen);
    static void nativeSetSystemUiVisibility(Int64 ptr, Int32 visibility);
    static void nativeSetFocusedApplication(Int64 ptr,
                    InputApplicationHandle* application);
    static Boolean nativeTransferTouchFocus(Int64 ptr,
                    IInputChannel* fromChannel, IInputChannel* toChannel);
    static void nativeSetPointerSpeed(Int64 ptr, Int32 speed);
    static void nativeSetShowTouches(Int64 ptr, Boolean enabled);
    static void nativeSetInteractive(Int64 ptr, Boolean interactive);
    static void nativeReloadCalibration(Int64 ptr);
    static void nativeVibrate(Int64 ptr, Int32 deviceId, ArrayOf<Int64>* pattern,
                    Int32 repeat, Int32 token);
    static void nativeCancelVibrate(Int64 ptr, Int32 deviceId, Int32 token);
    static void nativeReloadKeyboardLayouts(Int64 ptr);
    static void nativeReloadDeviceAliases(Int64 ptr);
    static String nativeDump(Int64 ptr);
    static void nativeMonitor(Int64 ptr);

    // Input event injection constants defined in InputDispatcher.h.
    const static Int32 INPUT_EVENT_INJECTION_SUCCEEDED = 0;
    const static Int32 INPUT_EVENT_INJECTION_PERMISSION_DENIED = 1;
    const static Int32 INPUT_EVENT_INJECTION_FAILED = 2;
    const static Int32 INPUT_EVENT_INJECTION_TIMED_OUT = 3;

    // Maximum number of milliseconds to wait for input event injection.
    const static Int32 INJECTION_TIMEOUT_MILLIS = 30 * 1000;

    // Key states (may be returned by queries about the current state of a
    // particular key code, scan code or switch).

    /** The key state is unknown or the requested key itself is not supported. */
    const static Int32 KEY_STATE_UNKNOWN = -1;

    /** The key is up. /*/
    const static Int32 KEY_STATE_UP = 0;

    /** The key is down. */
    const static Int32 KEY_STATE_DOWN = 1;

    /** The key is down but is a virtual key press that is being emulated by the system. */
    const static Int32 KEY_STATE_VIRTUAL = 2;

    /** Scan code: Mouse / trackball button. */
    const static Int32 BTN_MOUSE = 0x110;

    // Switch code values must match bionic/libc/kernel/common/linux/input.h
    /** Switch code: Lid switch.  When set, lid is shut. */
    const static Int32 SW_LID = 0x00;

    /** Switch code: Keypad slide.  When set, keyboard is exposed. */
    const static Int32 SW_KEYPAD_SLIDE = 0x0a;

    /** Switch code: Headphone.  When set, headphone is inserted. */
    const static Int32 SW_HEADPHONE_INSERT = 0x02;

    /** Switch code: Microphone.  When set, microphone is inserted. */
    const static Int32 SW_MICROPHONE_INSERT = 0x04;

    /** Switch code: Line out.  When set, Line out (hi-Z) is inserted. */
    const static Int32 SW_LINEOUT_INSERT = 0x06;

    /** Switch code: Headphone/Microphone Jack.  When set, something is inserted. */
    const static Int32 SW_JACK_PHYSICAL_INSERT = 0x07;

    /** Switch code: Camera lens cover. When set the lens is covered. */
    const static Int32 SW_CAMERA_LENS_COVER = 0x09;

    const static Int32 SW_LID_BIT = 1 << SW_LID;
    const static Int32 SW_KEYPAD_SLIDE_BIT = 1 << SW_KEYPAD_SLIDE;
    const static Int32 SW_HEADPHONE_INSERT_BIT = 1 << SW_HEADPHONE_INSERT;
    const static Int32 SW_MICROPHONE_INSERT_BIT = 1 << SW_MICROPHONE_INSERT;
    const static Int32 SW_LINEOUT_INSERT_BIT = 1 << SW_LINEOUT_INSERT;
    const static Int32 SW_JACK_PHYSICAL_INSERT_BIT = 1 << SW_JACK_PHYSICAL_INSERT;
    const static Int32 SW_JACK_BITS =
            SW_HEADPHONE_INSERT_BIT | SW_MICROPHONE_INSERT_BIT | SW_JACK_PHYSICAL_INSERT_BIT | SW_LINEOUT_INSERT_BIT;
    const static Int32 SW_CAMERA_LENS_COVER_BIT = 1 << SW_CAMERA_LENS_COVER;

    /** Whether to use the dev/input/event or uevent subsystem for the audio jack. */
    Boolean mUseDevInputEventForAudioJack;

private:
    void ReloadKeyboardLayouts();

    void ReloadDeviceAliases();

    void SetDisplayViewportsInternal(
        /* [in] */ IDisplayViewport* defaultViewport,
        /* [in] */ IDisplayViewport* externalTouchViewport);

    void SetDisplayViewport(
        /* [in] */ Boolean external,
        /* [in] */ IDisplayViewport* viewport);

    Boolean InjectInputEventInternal(
        /* [in] */ IInputEvent* event,
        /* [in] */ Int32 displayId,
        /* [in] */ Int32 mode);

    Boolean OnInputDevicesChangedListenerDied(
        /* [in] */ Int32 pid);

    // Must be called on handler.
    Boolean DeliverInputDevicesChanged(
        /* [in] */ ArrayOf<IInputDevice>* oldInputDevices);

    // Must be called on handler.
    void ShowMissingKeyboardLayoutNotification(
        /* [in] */ IInputDevice* device);

    // Must be called on handler.
    void HideMissingKeyboardLayoutNotification();

    // Must be called on handler.
    void UpdateKeyboardLayouts();

    static Boolean ContainsInputDeviceWithDescriptor(
        /* [in] */ ArrayOf<IInputDevice>* inputDevices,
        /* [in] */ String descriptor);


    void VisitAllKeyboardLayouts(
        /* [in] */ IKeyboardLayoutVisitor* visitor);

    void VisitKeyboardLayout(
        /* [in] */ String keyboardLayoutDescriptor,
        /* [in] */ IKeyboardLayoutVisitor* visitor);

    void VisitKeyboardLayoutsInPackage(
        /* [in] */ IPackageManager* pm,
        /* [in] */ IActivityInfo* receiver,
        /* [in] */ String keyboardName,
        /* [in] */ Int32 requestedPriority,
        /* [in] */ IKeyboardLayoutVisitor* visitor);

    /**
     * Builds a layout descriptor for the vendor/product. This returns the
     * descriptor for ids that aren't useful (such as the default 0, 0).
     */
    String GetLayoutDescriptor(
        /* [in] */ IInputDeviceIdentifier* identifier);

    // Must be called on handler.
    void HandleSwitchKeyboardLayout(
        /* [in] */ Int32 deviceId,
        /* [in] */ Int32 direction);

    void SetPointerSpeedUnchecked(
        /* [in] */ Int32 speed);

    void RegisterPointerSpeedSettingObserver();

    Int32 GetPointerSpeedSetting();

    void RegisterShowTouchesSettingObserver();

    Int32 GetShowTouchesSetting(
        /* [in] */ Int32 defaultValue);

    void OnVibratorTokenDied(
        /* [in] */ VibratorToken* v);

    void CancelVibrateIfNeeded(
        /* [in] */ VibratorToken* v);

    Boolean CheckCallingPermission(
        /* [in] */ String permission,
        /* [in] */ String func);

    // Native callback.
    void NotifyConfigurationChanged(
        /* [in] */ Int64 whenNanos);

    // Native callback.
    void NotifyInputDevicesChanged(
        /* [in] */ ArrayOf<IInputDevice>* inputDevices);

    // Native callback.
    void NotifySwitch(
        /* [in] */ Int64 whenNanos,
        /* [in] */ Int32 switchValues,
        /* [in] */ Int32 switchMask);

    // Native callback.
    void NotifyInputChannelBroken(
        /* [in] */ InputWindowHandle* inputWindowHandle);

    // Native callback.
    Int64 NotifyANR(
        /* [in] */ InputApplicationHandle* inputApplicationHandle,
        /* [in] */ InputWindowHandle* inputWindowHandle,
        /* [in] */ String reason);

    // Native callback.
    Boolean FilterInputEvent(
        /* [in] */ IInputEvent* event,
        /* [in] */ Int32 policyFlags);

    // Native callback.
    Int32 InterceptKeyBeforeQueueing(
        /* [in] */ IKeyEvent* event,
        /* [in] */ Int32 policyFlags);

    // Native callback.
    Int32 InterceptMotionBeforeQueueingNonInteractive(
        /* [in] */ Int64 whenNanos,
        /* [in] */ Int32 policyFlags);

    // Native callback.
    Int64 InterceptKeyBeforeDispatching(
        /* [in] */ InputWindowHandle* focus,
        /* [in] */ IKeyEvent* event,
        /* [in] */ Int32 policyFlags);

    // Native callback.
    IKeyEvent* DispatchUnhandledKey(
        /* [in] */ InputWindowHandle* focus,
        /* [in] */ IKeyEvent* event,
        /* [in] */ Int32 policyFlags);

    // Native callback.
    Boolean CheckInjectEventsPermission(
        /* [in] */ Int32 injectorPid,
        /* [in] */ Int32 injectorUid);

    // Native callback.
    Int32 GetVirtualKeyQuietTimeMillis();

    // Native callback.
    ArrayOf<String>* GetExcludedDeviceNames();

    // Native callback.
    Int32 GetKeyRepeatTimeout();

    // Native callback.
    Int32 GetKeyRepeatDelay();

    // Native callback.
    Int32 GetHoverTapTimeout();

    // Native callback.
    Int32 GetHoverTapSlop();

    // Native callback.
    Int32 GetDoubleTapTimeout();

    // Native callback.
    Int32 GetLongPressTimeout();

    // Native callback.
    Int32 GetPointerLayer();

    // Native callback.
    IPointerIcon* GetPointerIcon();

    // Native callback.
    ArrayOf<String>* GetKeyboardLayoutOverlay(
        /* [in] */ IInputDeviceIdentifier* identifier);

    // Native callback.
    String GetDeviceAlias(
        /* [in] */ String uniqueId);
};

} // Input
} // Server
} // Droid
} // Elastos

#endif // __ELASTOS_DROID_Service_Input_input_InputManagerService_H__
