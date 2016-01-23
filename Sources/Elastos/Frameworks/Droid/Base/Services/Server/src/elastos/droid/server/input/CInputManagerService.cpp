#include "elastos/droid/server/input/InputManagerService.h"
#include <elastos/droid/server/LocalServices.h>
#include "Elastos.Droid.Internal.h"
#include "Elastos.Droid.Content.h"
#include "Elastos.Droid.View.h"
#include <elastos/utility/logging/Slogger.h>
#include <elastos/core/AutoLock.h>
//#include "elastos/droid/os/CHandler.h"
#include "Elastos.Droid.Core.Server.h"

using Elastos::Droid::View::IDisplay;
using Elastos::Droid::Os::IHandler;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Utility::Logging::Slogger;
using Elastos::Droid::View::IWindowManagerPolicy;
using Elastos::Droid::Hardware::Input::IInputManager;
using Elastos::Droid::Hardware::Input::EIID_IIInputManager;
using Elastos::Droid::View::EIID_IInputFilterHost;
using Elastos::Droid::Os::EIID_IBinder;
using Elastos::Droid::Os::CHandler;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Input {

CAR_INTERFACE_IMPL_3(InputManagerService, Object, IIInputManager, IInputManagerService, IBinder);

InputManagerService::InputManagerService()
{}

InputManagerService::~InputManagerService()
{}

ECode InputManagerService::constructor()
{
    return NOERROR;
}

ECode InputManagerService::constructor(
    /* [in] */ IContext* context)
{
    AutoPtr<IResources> resources;
    AutoPtr<ILooper> looper;
    AutoPtr<IMessageQueue> queue;

    mContext = context;
//    mHandler = new InputManagerHandler(DisplayThread.get().getLooper(), this);

    context->GetResources((IResources**)&resources);
//    resources->GetBoolean(R.bool.config_useDevInputEventForAudioJack, &mUseDevInputEventForAudioJack);

    Slogger::I(TAG, "Initializing input manager, mUseDevInputEventForAudioJack="
            + mUseDevInputEventForAudioJack);
    mHandler->GetLooper((ILooper**)&looper);
    looper->GetQueue((IMessageQueue**)&queue);
    mPtr = nativeInit(this, mContext, queue);

//    LocalServices::AddService(InputManagerInternal.class, new LocalService());
    return NOERROR;
}


/****************
 * InputManagerService::InputFilterHost::
 *************************************************************************************************/

CAR_INTERFACE_IMPL_2(InputManagerService::InputFilterHost, Object, IInputFilterHost, IBinder);

InputManagerService::InputFilterHost::InputFilterHost()
{}

InputManagerService::InputFilterHost::~InputFilterHost()
{}

ECode InputManagerService::InputFilterHost::constructor()
{
    return NOERROR;
}

ECode InputManagerService::InputFilterHost::constructor(
    /* [in] */ IIInputManager* parent)
{
    mHost = (InputManagerService*)parent;
    return NOERROR;
}

ECode InputManagerService::InputFilterHost::DisconnectLocked()
{
    mDisconnected = TRUE;
    return NOERROR;
}

ECode InputManagerService::InputFilterHost::SendInputEvent(
    /* [in] */ IInputEvent* event,
    /* [in] */ Int32 policyFlags)
{
    if (event == NULL) {
        //throw new IllegalArgumentException("event must not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Object& lock = mHost->mInputFilterLock;
    synchronized (lock) {
        if (!mDisconnected) {
            nativeInjectInputEvent(mHost->mPtr, event, IDisplay::DEFAULT_DISPLAY, 0, 0,
                    IInputManager::INJECT_INPUT_EVENT_MODE_ASYNC, 0,
                    policyFlags | IWindowManagerPolicy::FLAG_FILTERED);
        }
    }

    return NOERROR;
}

ECode InputManagerService::InputFilterHost::ToString(
    /* [out] */ String* str)
{
    return NOERROR;
}


/****************
 * InputManagerService::VibratorToken::
 *************************************************************************************************/

InputManagerService::VibratorToken::VibratorToken(
    /* [in] */ Int32 deviceId,
    /* [in] */ IBinder* token,
    /* [in] */ Int32 tokenValue)
{
    mDeviceId = deviceId;
    mToken = token;
    mTokenValue = tokenValue;
}

//@Override
ECode InputManagerService::VibratorToken::BinderDied()
{
    if (DEBUG) {
        Slogger::D(TAG, "Vibrator token died.");
    }
//    OnVibratorTokenDied(this);
    return NOERROR;
}


/****************
 * InputManagerService::LocalService::
 *************************************************************************************************/
InputManagerService::LocalService::LocalService(
    /* [in] */ InputManagerService* parent)
    : mHost(parent)
{}

void InputManagerService::LocalService::SetDisplayViewports(
    /* [in] */ IDisplayViewport* defaultViewport,
    /* [in] */ IDisplayViewport* externalTouchViewport)
{
    mHost->SetDisplayViewportsInternal(defaultViewport, externalTouchViewport);
}

Boolean InputManagerService::LocalService::InjectInputEvent(
    /* [in] */ IInputEvent* event,
    /* [in] */ Int32 displayId,
    /* [in] */ Int32 mode)
{
    return mHost->InjectInputEventInternal(event, IDisplay::DEFAULT_DISPLAY, mode);
}

void InputManagerService::LocalService::SetInteractive(
    /* [in] */ Boolean interactive)
{
    mHost->nativeSetInteractive(mHost->mPtr, interactive);
}


/****************
 * InputManagerService::InputManagerHandler::
 *************************************************************************************************/

InputManagerService::InputManagerHandler::InputManagerHandler(
    /* [in] */ ILooper* looper,
    /* [in] */ InputManagerService* parent)
    : mHost(parent)

{
    CHandler::New(looper, NULL, TRUE /*async*/, (IHandler**)&superHandler);
}

//@Override
ECode InputManagerService::InputManagerHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    Int32 what;

    msg->GetWhat(&what);

    switch (what) {
        case MSG_DELIVER_INPUT_DEVICES_CHANGED:
        {
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);

            mHost->DeliverInputDevicesChanged(NULL/*obj*/);
        }
        break;

        case MSG_SWITCH_KEYBOARD_LAYOUT:
        {
            Int32 arg1, arg2;

            msg->GetArg1(&arg1);
            msg->GetArg2(&arg2);

            mHost->HandleSwitchKeyboardLayout(arg1, arg2);
        }
        break;

        case MSG_RELOAD_KEYBOARD_LAYOUTS:
            mHost->ReloadKeyboardLayouts();
            break;
        case MSG_UPDATE_KEYBOARD_LAYOUTS:
            mHost->UpdateKeyboardLayouts();
            break;
        case MSG_RELOAD_DEVICE_ALIASES:
            mHost->ReloadDeviceAliases();
            break;
    }

    return NOERROR;
}


/****************
 * InputManagerService::KeyboardLayoutDescriptor::
 *************************************************************************************************/

String InputManagerService::KeyboardLayoutDescriptor::Format(
    /* [in] */ const String& packageName,
    /* [in] */ const String& receiverName,
    /* [in] */ const String& keyboardName)
{
    return packageName + "/" + receiverName + "/" + keyboardName;
}

InputManagerService::KeyboardLayoutDescriptor* InputManagerService::KeyboardLayoutDescriptor::parse(
            /* [in] */ const String& descriptor)
{
    Int32 pos = descriptor.IndexOf('/');
    if (pos < 0 || pos + 1 == descriptor.GetLength()) {
        return NULL;
    }
    Int32 pos2 = descriptor.IndexOf('/', pos + 1);
    if (pos2 < pos + 2 || pos2 + 1 == descriptor.GetLength()) {
        return NULL;
    }

    KeyboardLayoutDescriptor* result = new KeyboardLayoutDescriptor();
    result->mPackageName = descriptor.Substring(0, pos);
    result->mReceiverName = descriptor.Substring(pos + 1, pos2);
    result->mKeyboardLayoutName = descriptor.Substring(pos2 + 1);
    return result;
}


/****************
 * InputManagerService::InputDevicesChangedListenerRecord::
 *************************************************************************************************/

InputManagerService::InputDevicesChangedListenerRecord::InputDevicesChangedListenerRecord(
    /* [in] */ Int32 pid,
    /* [in] */ IInputDevicesChangedListener* listener,
    /* [in] */ InputManagerService* parent)
    : mPid(pid), mListener(listener), mHost(parent)
{}

//@Override
void InputManagerService::InputDevicesChangedListenerRecord::BinderDied()
{
    if (DEBUG) {
        Slogger::D(TAG, "Input devices changed listener for pid %d died.", mPid);
    }
    mHost->OnInputDevicesChangedListenerDied(mPid);
}

ECode InputManagerService::InputDevicesChangedListenerRecord::NotifyInputDevicesChanged(
    /* [in] */ ArrayOf<Int32>* info)
{
    ECode ec;

    ec = mListener->OnInputDevicesChanged(info);
    if (FAILED(ec)) {
        Slogger::W(TAG, "Failed to notify process %d that input devices changed, assuming it died.", mPid);
        BinderDied();
        return E_REMOTE_EXCEPTION;
    }

    return NOERROR;
}


/****************
 * InputManagerService::
 *************************************************************************************************/

const String InputManagerService::TAG("InputManager");
const String InputManagerService::EXCLUDED_DEVICES_PATH("etc/excluded-input-devices.xml");

/**
 * these native functions come from ./frameworks/base/services/core/jni/com_android_server_input_InputManagerService.cpp
 */

Int64 InputManagerService::nativeInit(
    InputManagerService* service,
    IContext* context,
    IMessageQueue* messageQueue)
{
/*
static jlong nativeInit(JNIEnv* env, jclass clazz,
        jobject serviceObj, jobject contextObj, jobject messageQueueObj) {
    sp<MessageQueue> messageQueue = android_os_MessageQueue_getMessageQueue(env, messageQueueObj);
    if (messageQueue == NULL) {
        jniThrowRuntimeException(env, "MessageQueue is not initialized.");
        return 0;
    }

    NativeInputManager* im = new NativeInputManager(contextObj, serviceObj,
            messageQueue->getLooper());
    im->incStrong(0);
    return reinterpret_cast<jlong>(im);
}
    NativeMessageQueue* nativeMQ;
    Int64 messageQueue;

    messageQueue->GetMPtr(&messageQueue);
    if (messageQueue == NULL) {
        //jniThrowRuntimeException(env, "MessageQueue is not initialized.");
        return 0;
    }

    nativeMQ = reinterpret_cast<NativeMessageQueue*>(messageQueue);

    NativeInputManager* im = new NativeInputManager(context, service, nativeMQ->GetLooper());
*/
}



void InputManagerService::nativeStart(Int64 ptr)
{
/*
    NativeInputManager* im = reinterpret_cast<NativeInputManager*>(ptr);

    status_t result = im->getInputManager()->start();
    if (result) {
        jniThrowRuntimeException(env, "Input manager could not be started.");
    }
*/
}

void InputManagerService::nativeSetDisplayViewport(Int64 ptr, Boolean external,
        Int32 displayId, Int32 rotation,
        Int32 logicalLeft, Int32 logicalTop, Int32 logicalRight, Int32 logicalBottom,
        Int32 physicalLeft, Int32 physicalTop, Int32 physicalRight, Int32 physicalBottom,
        Int32 deviceWidth, Int32 deviceHeight)
{

}

Int32 InputManagerService::nativeGetScanCodeState(Int64 ptr,
        Int32 deviceId, Int32 sourceMask, Int32 scanCode)
{

}

Int32 InputManagerService::nativeGetKeyCodeState(Int64 ptr,
        Int32 deviceId, Int32 sourceMask, Int32 keyCode)
{

}

Int32 InputManagerService::nativeGetSwitchState(Int64 ptr,
        Int32 deviceId, Int32 sourceMask, Int32 sw)
{

}

Boolean InputManagerService::nativeHasKeys(Int64 ptr,
        Int32 deviceId, Int32 sourceMask, ArrayOf<Int32>* keyCodes, ArrayOf<Boolean> keyExists)
{

}

void InputManagerService::nativeRegisterInputChannel(Int64 ptr, IInputChannel* inputChannel,
        InputWindowHandle* inputWindowHandle, Boolean monitor)
{

}

void InputManagerService::nativeUnregisterInputChannel(Int64 ptr, IInputChannel* inputChannel)
{

}

void InputManagerService::nativeSetInputFilterEnabled(Int64 ptr, Boolean enable)
{

}

Int32 InputManagerService::nativeInjectInputEvent(Int64 ptr, IInputEvent* event, Int32 displayId,
        Int32 injectorPid, Int32 injectorUid, Int32 syncMode, Int32 timeoutMillis,
        Int32 policyFlags)
{

}

void InputManagerService::nativeSetInputWindows(Int64 ptr, ArrayOf<InputWindowHandle> windowHandles)
{

}

void InputManagerService::nativeSetInputDispatchMode(Int64 ptr, Boolean enabled, Boolean frozen)
{

}

void InputManagerService::nativeSetSystemUiVisibility(Int64 ptr, Int32 visibility)
{

}

void InputManagerService::nativeSetFocusedApplication(Int64 ptr,
        InputApplicationHandle* application)
{

}

Boolean InputManagerService::nativeTransferTouchFocus(Int64 ptr,
        IInputChannel* fromChannel, IInputChannel* toChannel)
{

}

void InputManagerService::nativeSetPointerSpeed(Int64 ptr, Int32 speed)
{

}

void InputManagerService::nativeSetShowTouches(Int64 ptr, Boolean enabled)
{

}

void InputManagerService::nativeSetInteractive(Int64 ptr, Boolean interactive)
{

}

void InputManagerService::nativeReloadCalibration(Int64 ptr)
{

}

void InputManagerService::nativeVibrate(Int64 ptr, Int32 deviceId, ArrayOf<Int64>* pattern,
        Int32 repeat, Int32 token)
{

}

void InputManagerService::nativeCancelVibrate(Int64 ptr, Int32 deviceId, Int32 token)
{

}

void InputManagerService::nativeReloadKeyboardLayouts(Int64 ptr)
{

}

void InputManagerService::nativeReloadDeviceAliases(Int64 ptr)
{

}

String InputManagerService::nativeDump(Int64 ptr)
{

}

void InputManagerService::nativeMonitor(Int64 ptr)
{

}


ECode InputManagerService::SetWindowManagerCallbacks(
    /* [in] */ IWindowManagerCallbacks* cbacks)
{

}

ECode InputManagerService::SetWiredAccessoryCallbacks(
    /* [in] */ IWiredAccessoryCallbacks* cbacks)
{
    return NOERROR;
}

ECode InputManagerService::Start()
{
    return NOERROR;
}

// TODO(BT) Pass in paramter for bluetooth system
ECode InputManagerService::SystemRunning()
{
    return NOERROR;
}

/**
 * Gets the current state of a key or button by key code.
 * @param deviceId The input device id, or -1 to consult all devices.
 * @param sourceMask The input sources to consult, or {@link InputDevice#SOURCE_ANY} to
 * consider all input sources.  An input device is consulted if at least one of its
 * non-class input source bits matches the specified source mask.
 * @param keyCode The key code to check.
 * @return The key state.
 */
ECode InputManagerService::GetKeyCodeState(
    /* [in] */ Int32 deviceId,
    /* [in] */ Int32 sourceMask,
    /* [in] */ Int32 keyCode,
    /* [out] */ Int32 *ret)
{
    return NOERROR;
}

/**
 * Gets the current state of a key or button by scan code.
 * @param deviceId The input device id, or -1 to consult all devices.
 * @param sourceMask The input sources to consult, or {@link InputDevice#SOURCE_ANY} to
 * consider all input sources.  An input device is consulted if at least one of its
 * non-class input source bits matches the specified source mask.
 * @param scanCode The scan code to check.
 * @return The key state.
 */
ECode InputManagerService::GetScanCodeState(
    /* [in] */ Int32 deviceId,
    /* [in] */ Int32 sourceMask,
    /* [in] */ Int32 scanCode,
    /* [out] */ Int32 *ret)
{
    return NOERROR;
}

/**
 * Gets the current state of a switch by switch code.
 * @param deviceId The input device id, or -1 to consult all devices.
 * @param sourceMask The input sources to consult, or {@link InputDevice#SOURCE_ANY} to
 * consider all input sources.  An input device is consulted if at least one of its
 * non-class input source bits matches the specified source mask.
 * @param switchCode The switch code to check.
 * @return The switch state.
 */
ECode InputManagerService::GetSwitchState(
    /* [in] */ Int32 deviceId,
    /* [in] */ Int32 sourceMask,
    /* [in] */ Int32 switchCode,
    /* [out] */ Int32 *ret)
{
    return NOERROR;
}

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
ECode InputManagerService::HasKeys(
    /* [in] */ Int32 deviceId,
    /* [in] */ Int32 sourceMask,
    /* [in] */ const ArrayOf<Int32>& keyCodes,
    /* [in] */ ArrayOf<Boolean>* keyExists,
    /* [out] */ Boolean* hasKeys)
{
    return NOERROR;
}

/**
 * Creates an input channel that will receive all input from the input dispatcher.
 * @param inputChannelName The input channel name.
 * @return The input channel.
 */
ECode InputManagerService::MonitorInput(
    /* [in] */ String inputChannelName,
    /* [in] */ IInputChannel* ic)
{
    return NOERROR;
}

/**
 * Registers an input channel so that it can be used as an input event target.
 * @param inputChannel The input channel to register.
 * @param inputWindowHandle The handle of the input window associated with the
 * input channel, or null if none.
 */
ECode InputManagerService::RegisterInputChannel(
    /* [in] */ IInputChannel* inputChannel,
    /* [in] */ InputWindowHandle* inputWindowHandle)
{
    return NOERROR;
}

/**
 * Unregisters an input channel.
 * @param inputChannel The input channel to unregister.
 */
ECode InputManagerService::UnregisterInputChannel(
    /* [in] */ IInputChannel* inputChannel)
{
    return NOERROR;
}

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
ECode InputManagerService::SetInputFilter(
    /* [in] */ IInputFilter* filter)
{
    synchronized (mInputFilterLock) {
        IInputFilter* oldFilter = mInputFilter;
        if (oldFilter == filter) {
            return NOERROR; // nothing to do
        }

        if (oldFilter != NULL) {
            mInputFilter = NULL;
            mInputFilterHost->DisconnectLocked();
            mInputFilterHost = NULL;
            //try {
//                oldFilter->Uninstall();
            //} catch (RemoteException re) {
                /* ignore */
            //}
        }

        if (filter != NULL) {
            mInputFilter = filter;
            mInputFilterHost = new InputFilterHost();
            mInputFilterHost->constructor(IIInputManager::Probe(this));
            //try {
//                filter->Install(mInputFilterHost);
            //} catch (RemoteException re) {
                /* ignore */
            //}
        }

        nativeSetInputFilterEnabled(mPtr, filter != NULL);
    }
    return NOERROR;
}

//// @Override // Binder call
ECode InputManagerService::InjectInputEvent(
    /* [in] */ IInputEvent* event,
    /* [in] */ Int32 mode,
    /* [out] */ Boolean* injectIt)
{
    return NOERROR;
}

/**
 * Gets information about the input device with the specified id.
 * @param deviceId The device id.
 * @return The input device or null if not found.
 */
//// @Override // Binder call
ECode InputManagerService::GetInputDevice(
    /* [in] */ Int32 deviceId,
    /* [out, callee] */ IInputDevice** inputDevice)
{
    return NOERROR;
}

/**
 * Gets the ids of all input devices in the system.
 * @return The input device ids.
 */
//// @Override // Binder call
ECode InputManagerService::GetInputDeviceIds(
    /* [out, callee] */ ArrayOf<Int32>** deviceIds)
{
    return NOERROR;
}

/**
 * Gets all input devices in the system.
 * @return The array of input devices.
 */
ECode InputManagerService::GetInputDevices(
    /* [out] */ ArrayOf<IInputDevice*>* inputDevices )
{
    return NOERROR;
}

// @Override // Binder call
ECode InputManagerService::RegisterInputDevicesChangedListener(
    /* [in] */ IInputDevicesChangedListener* listener)
{
    return NOERROR;
}

// @Override // Binder call & native callback
ECode InputManagerService::GetTouchCalibrationForInputDevice(
    /* [in] */ const String& inputDeviceDescriptor,
    /* [in] */ Int32 surfaceRotation,
    /* [out] */ ITouchCalibration** inputDevice)
{
    return NOERROR;
}

// @Override // Binder call
ECode InputManagerService::SetTouchCalibrationForInputDevice(
    /* [in] */ const String& inputDeviceDescriptor,
    /* [in] */ Int32 surfaceRotation,
    /* [in] */ ITouchCalibration* calibration)
{
    return NOERROR;
}

// @Override // Binder call
ECode InputManagerService::GetKeyboardLayouts(
    /* [out, callee] */ ArrayOf<IKeyboardLayout*>** kbLayouts)
{
    return NOERROR;
}

// @Override // Binder call
ECode InputManagerService::GetKeyboardLayout(
    /* [in] */ const String& keyboardLayoutDescriptor,
    /* [out, callee] */ IKeyboardLayout** kbLayout)
{
    return NOERROR;
}

// @Override // Binder call
ECode InputManagerService::GetCurrentKeyboardLayoutForInputDevice(
    /* [in] */ IInputDeviceIdentifier* identifier,
    /* [out, callee] */ String* kbLayout)
{
    return NOERROR;
}

// @Override // Binder call
ECode InputManagerService::SetCurrentKeyboardLayoutForInputDevice(
    /* [in] */ IInputDeviceIdentifier* identifier,
    /* [in] */ const String& keyboardLayoutDescriptor)
{
    return NOERROR;
}

// @Override // Binder call
ECode InputManagerService::GetKeyboardLayoutsForInputDevice(
    /* [in] */ IInputDeviceIdentifier* identifier,
    /* [out, callee] */ ArrayOf<String>** kbLayouts)
{
    return NOERROR;
}

// @Override // Binder call
ECode InputManagerService::AddKeyboardLayoutForInputDevice(
    /* [in] */ IInputDeviceIdentifier* identifier,
    /* [in] */ const String& keyboardLayoutDescriptor)
{
    return NOERROR;
}

// @Override // Binder call
ECode InputManagerService::RemoveKeyboardLayoutForInputDevice(
    /* [in] */ IInputDeviceIdentifier* identifier,
    /* [in] */ const String& keyboardLayoutDescriptor)
{
    return NOERROR;
}

ECode InputManagerService::SwitchKeyboardLayout(
    /* [in] */ Int32 deviceId,
    /* [in] */ Int32 direction)
{
    return NOERROR;
}

ECode InputManagerService::SetInputWindows(
    /* [in] */ ArrayOf<InputWindowHandle*> windowHandles)
{
    return NOERROR;
}

ECode InputManagerService::SetFocusedApplication(
    /* [in] */ InputApplicationHandle* application)
{
    return NOERROR;
}

ECode InputManagerService::SetInputDispatchMode(
    /* [in] */ Boolean enabled,
    /* [in] */ Boolean frozen)
{
    return NOERROR;
}

ECode InputManagerService::SetSystemUiVisibility(
    /* [in] */ Int32 visibility)
{
    return NOERROR;
}

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
ECode InputManagerService::TransferTouchFocus(
    /* [in] */ IInputChannel* fromChannel,
    /* [in] */ IInputChannel* toChannel,
    /* [out] */ Boolean* transferIt)
{
    return NOERROR;
}

// @Override // Binder call
ECode InputManagerService::TryPointerSpeed(
    /* [in] */ Int32 speed)
{
    return NOERROR;
}

ECode InputManagerService::UpdatePointerSpeedFromSettings()
{
    return NOERROR;
}

ECode InputManagerService::UpdateShowTouchesFromSettings()
{
    return NOERROR;
}

// Binder call
// @Override
ECode InputManagerService::Vibrate(
    /* [in] */ Int32 deviceId,
    /* [in] */ const ArrayOf<Int64>& pattern,
    /* [in] */ Int32 repeat,
    /* [in] */ IBinder* token)
{
    return NOERROR;
}

// Binder call
// @Override
ECode InputManagerService::CancelVibrate(
    /* [in] */ Int32 deviceId,
    /* [in] */ IBinder* token)
{
    return NOERROR;
}

// @Override
ECode InputManagerService::Dump(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ IPrintWriter* pw,
    /* [in] */ ArrayOf<String>* args)
{
    return NOERROR;
}

// Called by the heartbeat to ensure locks are not held indefinitely (for deadlock detection).
// @Override
ECode InputManagerService::Monitor()
{
    return NOERROR;
}

ECode InputManagerService::ToString(
    /* [out] */ String* str)
{
    return NOERROR;
}


void InputManagerService::ReloadKeyboardLayouts()
{

}

void InputManagerService::ReloadDeviceAliases()
{

}

void InputManagerService::SetDisplayViewportsInternal(
    /* [in] */ IDisplayViewport* defaultViewport,
    /* [in] */ IDisplayViewport* externalTouchViewport)
{
}

void InputManagerService::SetDisplayViewport(
    /* [in] */ Boolean external,
    /* [in] */ IDisplayViewport* viewport)
{
}

Boolean InputManagerService::InjectInputEventInternal(
    /* [in] */ IInputEvent* event,
    /* [in] */ Int32 displayId,
    /* [in] */ Int32 mode)
{
}

Boolean InputManagerService::OnInputDevicesChangedListenerDied(
    /* [in] */ Int32 pid)
{
}

// Must be called on handler.
Boolean InputManagerService::DeliverInputDevicesChanged(
    /* [in] */ ArrayOf<IInputDevice>* oldInputDevices)
{
}

// Must be called on handler.
void InputManagerService::ShowMissingKeyboardLayoutNotification(
    /* [in] */ IInputDevice* device)
{
}

// Must be called on handler.
void InputManagerService::HideMissingKeyboardLayoutNotification()
{
}

// Must be called on handler.
void InputManagerService::UpdateKeyboardLayouts()
{
}

static Boolean ContainsInputDeviceWithDescriptor(
    /* [in] */ ArrayOf<IInputDevice>* inputDevices,
    /* [in] */ String descriptor)
{
}


void InputManagerService::VisitAllKeyboardLayouts(
    /* [in] */ IKeyboardLayoutVisitor* visitor)
{
}

void InputManagerService::VisitKeyboardLayout(
    /* [in] */ String keyboardLayoutDescriptor,
    /* [in] */ IKeyboardLayoutVisitor* visitor)
{
}

void InputManagerService::VisitKeyboardLayoutsInPackage(
    /* [in] */ IPackageManager* pm,
    /* [in] */ IActivityInfo* receiver,
    /* [in] */ String keyboardName,
    /* [in] */ Int32 requestedPriority,
    /* [in] */ IKeyboardLayoutVisitor* visitor)
{
}

/**
 * Builds a layout descriptor for the vendor/product. This returns the
 * descriptor for ids that aren't useful (such as the default 0, 0).
 */
String InputManagerService::GetLayoutDescriptor(
    /* [in] */ IInputDeviceIdentifier* identifier)
{
}

// Must be called on handler.
void InputManagerService::HandleSwitchKeyboardLayout(
    /* [in] */ Int32 deviceId,
    /* [in] */ Int32 direction)
{
}

void InputManagerService::SetPointerSpeedUnchecked(
    /* [in] */ Int32 speed)
{
}

void InputManagerService::RegisterPointerSpeedSettingObserver()
{
}

Int32 InputManagerService::GetPointerSpeedSetting()
{
}

void InputManagerService::RegisterShowTouchesSettingObserver()
{
}

Int32 InputManagerService::GetShowTouchesSetting(
    /* [in] */ Int32 defaultValue)
{
}

void InputManagerService::OnVibratorTokenDied(
    /* [in] */ VibratorToken* v)
{
}

void InputManagerService::CancelVibrateIfNeeded(
    /* [in] */ VibratorToken* v)
{
}

Boolean InputManagerService::CheckCallingPermission(
    /* [in] */ String permission,
    /* [in] */ String func)
{
}

// Native callback.
void InputManagerService::NotifyConfigurationChanged(
    /* [in] */ Int64 whenNanos)
{
}

// Native callback.
void InputManagerService::NotifyInputDevicesChanged(
    /* [in] */ ArrayOf<IInputDevice>* inputDevices)
{
}

// Native callback.
void InputManagerService::NotifySwitch(
    /* [in] */ Int64 whenNanos,
    /* [in] */ Int32 switchValues,
    /* [in] */ Int32 switchMask)
{
}

// Native callback.
void InputManagerService::NotifyInputChannelBroken(
    /* [in] */ InputWindowHandle* inputWindowHandle)
{
}

// Native callback.
Int64 InputManagerService::NotifyANR(
    /* [in] */ InputApplicationHandle* inputApplicationHandle,
    /* [in] */ InputWindowHandle* inputWindowHandle,
    /* [in] */ String reason)
{
}

// Native callback.
Boolean InputManagerService::FilterInputEvent(
    /* [in] */ IInputEvent* event,
    /* [in] */ Int32 policyFlags)
{
}

// Native callback.
Int32 InputManagerService::InterceptKeyBeforeQueueing(
    /* [in] */ IKeyEvent* event,
    /* [in] */ Int32 policyFlags)
{
}

// Native callback.
Int32 InputManagerService::InterceptMotionBeforeQueueingNonInteractive(
    /* [in] */ Int64 whenNanos,
    /* [in] */ Int32 policyFlags)
{
}

// Native callback.
Int64 InputManagerService::InterceptKeyBeforeDispatching(
    /* [in] */ InputWindowHandle* focus,
    /* [in] */ IKeyEvent* event,
    /* [in] */ Int32 policyFlags)
{
}

// Native callback.
IKeyEvent* InputManagerService::DispatchUnhandledKey(
    /* [in] */ InputWindowHandle* focus,
    /* [in] */ IKeyEvent* event,
    /* [in] */ Int32 policyFlags)
{
    return NULL;
}

// Native callback.
Boolean InputManagerService::CheckInjectEventsPermission(
    /* [in] */ Int32 injectorPid,
    /* [in] */ Int32 injectorUid)
{
}

// Native callback.
Int32 InputManagerService::GetVirtualKeyQuietTimeMillis()
{
}

// Native callback.
ArrayOf<String>* InputManagerService::GetExcludedDeviceNames()
{
}

// Native callback.
Int32 InputManagerService::GetKeyRepeatTimeout()
{
}

// Native callback.
Int32 InputManagerService::GetKeyRepeatDelay()
{
}

// Native callback.
Int32 InputManagerService::GetHoverTapTimeout()
{
}

// Native callback.
Int32 InputManagerService::GetHoverTapSlop()
{
}

// Native callback.
Int32 InputManagerService::GetDoubleTapTimeout()
{
}

// Native callback.
Int32 InputManagerService::GetLongPressTimeout()
{
}

// Native callback.
Int32 InputManagerService::GetPointerLayer()
{
}

// Native callback.
IPointerIcon* InputManagerService::GetPointerIcon()
{
}

// Native callback.
ArrayOf<String>* InputManagerService::GetKeyboardLayoutOverlay(
    /* [in] */ IInputDeviceIdentifier* identifier)
{
}

// Native callback.
String InputManagerService::GetDeviceAlias(
    /* [in] */ String uniqueId)
{
}

} // Input
} // Server
} // Droid
} // Elastos
