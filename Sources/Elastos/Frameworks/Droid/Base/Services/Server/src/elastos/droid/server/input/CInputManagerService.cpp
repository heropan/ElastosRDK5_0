#include <elastos/droid/ext/frameworkdef.h>
#include <binder/Parcel.h>
#include <elastos/core/AutoLock.h>
#include "elastos/core/Math.h"
#include <elastos/core/StringUtils.h>
#define HASH_FOR_OS
#include <elastos/droid/ext/frameworkhash.h>
//#include "elastos/droid/app/CPendingIntentHelper.h"
#include <elastos/droid/Manifest.h>
#include "elastos/droid/os/Binder.h"
#include "elastos/droid/os/NativeMessageQueue.h"
#include "elastos/droid/os/UserHandle.h"
#include "elastos/droid/server/input/CInputManagerService.h"
#include "elastos/droid/server/DisplayThread.h"
#include "elastos/droid/server/LocalServices.h"
#include "elastos/droid/R.h"
//#include "elastos/droid/view/CViewConfigurationHelper.h"
#include "elastos/droid/view/NativeInputChannel.h"
#include "Elastos.Droid.Core.Server.h"
#include "Elastos.Droid.Internal.h"
#include "Elastos.Droid.Content.h"
#include "Elastos.Droid.View.h"
#include <elastos/utility/logging/Logger.h>
#include <elastos/utility/logging/Slogger.h>
#include <Elastos.CoreLibrary.Libcore.h>
#include <Elastos.CoreLibrary.IO.h>
#include <Elastos.Droid.Bluetooth.h>
#include "elastos/droid/utility/Xml.h"
#include "elastos/droid/view/InputChannel.h"
#include "elastos/droid/internal/utility/XmlUtils.h"
#include "elastos/droid/os/Process.h"
#include "Elastos.CoreLibrary.Core.h"
//#include "elastos/droid/provider/CSettingsSystem.h"
//#include "elastos/droid/os/CHandler.h"
//#include "elastos/droid/app/CPendingIntent.h"

using Elastos::Core::CArrayOf;
using Elastos::Core::CString;
using Elastos::Core::ICharSequence;
using Elastos::Core::IArrayOf;
using Elastos::Core::StringUtils;
using Elastos::Droid::App::INotification;
using Elastos::Droid::App::INotificationBuilder;
using Elastos::Droid::App::CNotificationBuilder;
using Elastos::Droid::App::IPendingIntent;
using Elastos::Droid::App::IPendingIntentHelper;
using Elastos::Droid::Bluetooth::IBluetoothDevice;
using Elastos::Droid::Bluetooth::IBluetoothAdapterHelper;
using Elastos::Droid::Bluetooth::CBluetoothAdapterHelper;
using Elastos::Droid::Content::IContentResolver;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::CIntent;
using Elastos::Droid::Content::CComponentName;
using Elastos::Droid::Content::IComponentName;
using Elastos::Droid::Content::CIntentFilter;
using Elastos::Droid::Content::IIntentFilter;
using Elastos::Droid::Content::Pm::IApplicationInfo;
using Elastos::Droid::Content::Pm::IComponentInfo;
using Elastos::Droid::Content::Pm::IResolveInfo;
using Elastos::Droid::Content::Pm::IPackageItemInfo;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Content::Res::ITypedArray;
using Elastos::Droid::Content::Res::IXmlResourceParser;
using Elastos::Droid::Hardware::Input::EIID_IIInputManager;
using Elastos::Droid::Hardware::Input::EIID_IInputManagerInternal;
using Elastos::Droid::Hardware::Input::IInputManager;
//using Elastos::Droid::Hardware::Input::CKeyboardLayout;
using Elastos::Droid::Hardware::Input::IKeyboardLayout;
using Elastos::Droid::Manifest;
using Elastos::Droid::Graphics::IRect;
using Elastos::Droid::Internal::Utility::XmlUtils;
using Elastos::Droid::Os::IHandler;
using Elastos::Droid::Os::Binder;
using Elastos::Droid::Os::CHandler;
using Elastos::Droid::Os::EIID_IBinder;
using Elastos::Droid::Os::NativeMessageQueue;
using Elastos::Droid::Os::CEnvironment;
using Elastos::Droid::Os::IEnvironment;
using Elastos::Droid::Os::IUserHandle;
using Elastos::Droid::Os::Process;
using Elastos::Droid::Os::IProcess;
using Elastos::Droid::Os::UserHandle;
//using Elastos::Droid::Provider::CSettingsSystem;
using Elastos::Droid::Provider::ISettingsSystem;
using Elastos::Droid::Utility::CSparseArray;
using Elastos::Droid::Utility::IAttributeSet;
using Elastos::Droid::Utility::Xml;
using Elastos::Droid::View::CViewConfigurationHelper;
using Elastos::Droid::View::IDisplay;
using Elastos::Droid::View::EIID_IInputDevice;
using Elastos::Droid::View::IMotionEvent;
using Elastos::Droid::View::CInputChannelHelper;
using Elastos::Droid::View::IInputChannelHelper;
using Elastos::Droid::View::IWindowManagerPolicy;
using Elastos::Droid::View::EIID_IIInputFilterHost;
using Elastos::Droid::View::NativeInputChannel;
using Elastos::Droid::View::CPointerIconHelper;
using Elastos::Droid::View::IPointerIcon;
using Elastos::Droid::View::IPointerIconHelper;
using Elastos::Droid::View::ISurface;
using Elastos::Droid::View::IViewConfigurationHelper;
using Elastos::Droid::Widget::CToastHelper;
using Elastos::Droid::Widget::IToastHelper;
using Elastos::Droid::R;
using Elastos::IO::ICloseable;
using Elastos::IO::CFile;
using Elastos::IO::IFile;
using Elastos::IO::CFileReader;
using Elastos::IO::IFileReader;
using Elastos::IO::IInputStream;
using Elastos::IO::CInputStreamReader;
using Elastos::IO::IInputStreamReader;
using Elastos::IO::IReader;
using Elastos::IO::ISerializable;
using Libcore::IO::CStreams;
using Libcore::IO::IStreams;
using Elastos::Utility::CArrayList;
using Elastos::Utility::IHashSet;
using Elastos::Utility::CHashSet;
using Elastos::Utility::IIterator;
using Elastos::Utility::IList;
using Elastos::Utility::IListIterator;
using Elastos::Utility::ISet;
using Elastos::Utility::Logging::Logger;
using Elastos::Utility::Logging::Slogger;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Input {

//==============================================================================
//  CInputManagerService::InputManagerHandler
//==============================================================================
CInputManagerService::InputManagerHandler::InputManagerHandler(
    /* [in] */ ILooper* looper,
    /* [in] */ CInputManagerService* host)
    : Handler(looper, NULL, TRUE/*async*/)
    , mHost(host)
{}

//@Override
ECode CInputManagerService::InputManagerHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    Int32 what;
    msg->GetWhat(&what);
    switch (what) {
        case MSG_DELIVER_INPUT_DEVICES_CHANGED:
        {
            AutoPtr<IInterface> obj;
            msg->GetObj((IInterface**)&obj);
            IArrayOf* array = IArrayOf::Probe(obj);
            AutoPtr< ArrayOf<IInputDevice*> > inputDevices;
            if (array != NULL) {
                Int32 size;
                array->GetLength(&size);
                inputDevices = ArrayOf<IInputDevice*>::Alloc(size);
                for (Int32 i = 0; i < size; i++) {
                    AutoPtr<IInterface> inputDevice;
                    array->Get(i, (IInterface**)&inputDevice);
                    inputDevices->Set(i, IInputDevice::Probe(inputDevice));
                }
            }
            mHost->DeliverInputDevicesChanged(inputDevices);
            break;
        }
        case MSG_SWITCH_KEYBOARD_LAYOUT:
        {
            Int32 arg1, arg2;
            msg->GetArg1(&arg1);
            msg->GetArg2(&arg2);
            mHost->HandleSwitchKeyboardLayout(arg1, arg2);
            break;
        }
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


//==============================================================================
//  CInputManagerService::InputFilterHost
//==============================================================================
CAR_INTERFACE_IMPL_2(CInputManagerService::InputFilterHost, Object, IIInputFilterHost, IBinder);

CInputManagerService::InputFilterHost::InputFilterHost()
    : mDisconnected(FALSE)
    , mHost(NULL)
{}

CInputManagerService::InputFilterHost::~InputFilterHost()
{}

ECode CInputManagerService::InputFilterHost::constructor(
    /* [in] */ IIInputManager* host)
{
    mHost = (CInputManagerService*)host;
    return NOERROR;
}

void CInputManagerService::InputFilterHost::DisconnectLocked()
{
    mDisconnected = TRUE;
}

ECode CInputManagerService::InputFilterHost::SendInputEvent(
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
            mHost->NativeInjectInputEvent(event, IDisplay::DEFAULT_DISPLAY, 0, 0,
                    IInputManager::INJECT_INPUT_EVENT_MODE_ASYNC, 0,
                    policyFlags | IWindowManagerPolicy::FLAG_FILTERED);
        }
    }
    return NOERROR;
}

ECode CInputManagerService::InputFilterHost::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    *str = "CInputManagerService::InputFilterHost";
    return NOERROR;
}


//==============================================================================
//  CInputManagerService::KeyboardLayoutDescriptor
//==============================================================================
String CInputManagerService::KeyboardLayoutDescriptor::Format(
    /* [in] */ const String& packageName,
    /* [in] */ const String& receiverName,
    /* [in] */ const String& keyboardName)
{
    return packageName + "/" + receiverName + "/" + keyboardName;
}

AutoPtr<CInputManagerService::KeyboardLayoutDescriptor>
CInputManagerService::KeyboardLayoutDescriptor::Parse(
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

    AutoPtr<KeyboardLayoutDescriptor> result = new KeyboardLayoutDescriptor();
    result->mPackageName = descriptor.Substring(0, pos);
    result->mReceiverName = descriptor.Substring(pos + 1, pos2);
    result->mKeyboardLayoutName = descriptor.Substring(pos2 + 1);
    return result;
}


//==============================================================================
//  CInputManagerService::InputDevicesChangedListenerRecord
//==============================================================================
CAR_INTERFACE_IMPL(CInputManagerService::InputDevicesChangedListenerRecord, Object, IProxyDeathRecipient);

CInputManagerService::InputDevicesChangedListenerRecord::InputDevicesChangedListenerRecord(
    /* [in] */ Int32 pid,
    /* [in] */ IInputDevicesChangedListener* listener,
    /* [in] */ CInputManagerService* host)
    : mPid(pid)
    , mListener(listener)
    , mHost(host)
{}

//@Override
ECode CInputManagerService::InputDevicesChangedListenerRecord::ProxyDied()
{
    if (DEBUG) {
        Slogger::D(TAG, "Input devices changed listener for pid %d died.", mPid);
    }
    mHost->OnInputDevicesChangedListenerDied(mPid);
    return NOERROR;
}

void CInputManagerService::InputDevicesChangedListenerRecord::NotifyInputDevicesChanged(
    /* [in] */ ArrayOf<Int32>* info)
{
    ECode ec = mListener->OnInputDevicesChanged(info);
    if (FAILED(ec)) {
        Slogger::W(TAG, "Failed to notify process %d that input devices changed, assuming it died.", mPid);
        ProxyDied();
    }
}


//==============================================================================
//  CInputManagerService::VibratorToken
//==============================================================================
CAR_INTERFACE_IMPL(CInputManagerService::VibratorToken, Object, IProxyDeathRecipient);

CInputManagerService::VibratorToken::VibratorToken(
    /* [in] */ Int32 deviceId,
    /* [in] */ IBinder* token,
    /* [in] */ Int32 tokenValue,
    /* [in] */ CInputManagerService* host)
    : mDeviceId(deviceId)
    , mToken(token)
    , mTokenValue(tokenValue)
    , mHost(host)
{}

//@Override
ECode CInputManagerService::VibratorToken::ProxyDied()
{
    if (DEBUG) {
        Slogger::D(TAG, "Vibrator token died.");
    }
    mHost->OnVibratorTokenDied(this);
    return NOERROR;
}


//==============================================================================
//  CInputManagerService::LocalService
//==============================================================================
CAR_INTERFACE_IMPL(CInputManagerService::LocalService, Object, IInputManagerInternal);

CInputManagerService::LocalService::LocalService(
    /* [in] */ CInputManagerService* host)
    : mHost(host)
{}

ECode CInputManagerService::LocalService::SetDisplayViewports(
    /* [in] */ IDisplayViewport* defaultViewport,
    /* [in] */ IDisplayViewport* externalTouchViewport)
{
    mHost->SetDisplayViewportsInternal(defaultViewport, externalTouchViewport);
    return NOERROR;
}

ECode CInputManagerService::LocalService::InjectInputEvent(
    /* [in] */ IInputEvent* event,
    /* [in] */ Int32 displayId,
    /* [in] */ Int32 mode,
    /* [out] */ Boolean* result)
{
    return mHost->InjectInputEventInternal(event, displayId, mode, result);
}

ECode CInputManagerService::LocalService::SetInteractive(
    /* [in] */ Boolean interactive)
{
    mHost->NativeSetInteractive(interactive);
    return NOERROR;
}

//------------------------------------------------------------------------------
//  CInputManagerService::Start_BroadcastReceiver
//------------------------------------------------------------------------------

CInputManagerService::Start_BroadcastReceiver::Start_BroadcastReceiver(
    /* [in] */ CInputManagerService* owner)
    : mOwner(owner)
{}
//@Override
ECode CInputManagerService::Start_BroadcastReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    mOwner->UpdatePointerSpeedFromSettings();
    mOwner->UpdateShowTouchesFromSettings();

    return NOERROR;
}

//------------------------------------------------------------------------------
//  CInputManagerService::SystemRunning_BroadcastReceiver_1
//------------------------------------------------------------------------------

CInputManagerService::SystemRunning_BroadcastReceiver_1::SystemRunning_BroadcastReceiver_1(
    /* [in] */ CInputManagerService* owner)
    : mOwner(owner)
{}

//@Override
ECode CInputManagerService::SystemRunning_BroadcastReceiver_1::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    mOwner->UpdateKeyboardLayouts();

    return NOERROR;
}

//------------------------------------------------------------------------------
//  CInputManagerService::SystemRunning_BroadcastReceiver_2
//------------------------------------------------------------------------------

CInputManagerService::SystemRunning_BroadcastReceiver_2::SystemRunning_BroadcastReceiver_2(
    /* [in] */ CInputManagerService* owner)
    : mOwner(owner)
{}

//@Override
ECode CInputManagerService::SystemRunning_BroadcastReceiver_2::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    mOwner->ReloadDeviceAliases();

    return NOERROR;
}

//------------------------------------------------------------------------------
//  CInputManagerService::UpdateKeyboardLayouts_KeyboardLayoutVisitor
//------------------------------------------------------------------------------

CAR_INTERFACE_IMPL(CInputManagerService::UpdateKeyboardLayouts_KeyboardLayoutVisitor, Object, IKeyboardLayoutVisitor);

CInputManagerService::UpdateKeyboardLayouts_KeyboardLayoutVisitor::UpdateKeyboardLayouts_KeyboardLayoutVisitor(
    /* [in] */ HashSet<String>& availableKeyboardLayouts)
    : mAvailableKeyboardLayouts(availableKeyboardLayouts)
{}

ECode CInputManagerService::UpdateKeyboardLayouts_KeyboardLayoutVisitor::VisitKeyboardLayout(
    /* [in] */ IResources* resources,
    /* [in] */ const String& descriptor,
    /* [in] */ const String& label,
    /* [in] */ const String& collection,
    /* [in] */ Int32 keyboardLayoutResId,
    /* [in] */ Int32 priority)
{
    mAvailableKeyboardLayouts.Insert(descriptor);

    return NOERROR;
}

//------------------------------------------------------------------------------
//  CInputManagerService::GetKeyboardLayouts_KeyboardLayoutVisitor
//------------------------------------------------------------------------------

CAR_INTERFACE_IMPL(CInputManagerService::GetKeyboardLayouts_KeyboardLayoutVisitor, Object, IKeyboardLayoutVisitor);

CInputManagerService::GetKeyboardLayouts_KeyboardLayoutVisitor::GetKeyboardLayouts_KeyboardLayoutVisitor(
     /* [in] */ List<AutoPtr<IKeyboardLayout> >& list)
     : mList(list)
{}

ECode CInputManagerService::GetKeyboardLayouts_KeyboardLayoutVisitor::VisitKeyboardLayout(
    /* [in] */ IResources* resources,
    /* [in] */ const String& descriptor,
    /* [in] */ const String& label,
    /* [in] */ const String& collection,
    /* [in] */ Int32 keyboardLayoutResId,
    /* [in] */ Int32 priority)
{
    AutoPtr<IKeyboardLayout> layout;
//TODO
//    CKeyboardLayout::New(descriptor, label, collection, priority, (IKeyboardLayout**)&layout);
    mList.PushBack(layout);

    return NOERROR;
}

//------------------------------------------------------------------------------
//  CInputManagerService::GetKeyboardLayout_KeyboardLayoutVisitor
//------------------------------------------------------------------------------

CAR_INTERFACE_IMPL(CInputManagerService::GetKeyboardLayout_KeyboardLayoutVisitor, Object, IKeyboardLayoutVisitor);

CInputManagerService::GetKeyboardLayout_KeyboardLayoutVisitor::GetKeyboardLayout_KeyboardLayoutVisitor(
    /* [in] */ IKeyboardLayout** layout)
    : mLayout(layout)
{}

ECode CInputManagerService::GetKeyboardLayout_KeyboardLayoutVisitor::VisitKeyboardLayout(
    /* [in] */ IResources* resources,
    /* [in] */ const String& descriptor,
    /* [in] */ const String& label,
    /* [in] */ const String& collection,
    /* [in] */ Int32 keyboardLayoutResId,
    /* [in] */ Int32 priority)
{
    assert(mLayout);

//TODO
//    kbLayout = CKeyboardLayout::New(descriptor, label, collection, priority, (IKeyboardLayout**)&mLayout);

    return NOERROR;
}

//------------------------------------------------------------------------------
//  CInputManagerService::GetKeyboardLayoutOverlay_KeyboardLayoutVisitor
//------------------------------------------------------------------------------

CAR_INTERFACE_IMPL(CInputManagerService::GetKeyboardLayoutOverlay_KeyboardLayoutVisitor, Object, IKeyboardLayoutVisitor);

CInputManagerService::GetKeyboardLayoutOverlay_KeyboardLayoutVisitor::GetKeyboardLayoutOverlay_KeyboardLayoutVisitor(
    /* [in] */ ArrayOf<String>* layouts)
    : mLayouts(layouts)
{}

ECode CInputManagerService::GetKeyboardLayoutOverlay_KeyboardLayoutVisitor::VisitKeyboardLayout(
    /* [in] */ IResources* resources,
    /* [in] */ const String& descriptor,
    /* [in] */ const String& label,
    /* [in] */ const String& collection,
    /* [in] */ Int32 keyboardLayoutResId,
    /* [in] */ Int32 priority)
{
    (*mLayouts)[0] = descriptor;

    AutoPtr<IInputStream> inputStream;
    resources->OpenRawResource(keyboardLayoutResId, (IInputStream**)&inputStream);
    AutoPtr<IInputStreamReader> reader;
    CInputStreamReader::New(inputStream, (IInputStreamReader**)&reader);
    AutoPtr<IStreams> stream;
    CStreams::AcquireSingleton((IStreams**)&stream);
    stream->ReadFully(IReader::Probe(reader), &((*mLayouts)[1]));

    return NOERROR;
}


//------------------------------------------------------------------------------
//  CInputManagerService::RegisterPointerSpeedSettingObserver_ContentObserver
//------------------------------------------------------------------------------

CInputManagerService::RegisterPointerSpeedSettingObserver_ContentObserver::RegisterPointerSpeedSettingObserver_ContentObserver(
    /* [in] */ CInputManagerService* owner,
    /* [in] */ IHandler* handler)
    : mOwner(owner)
{
    constructor(handler);
}

ECode CInputManagerService::RegisterPointerSpeedSettingObserver_ContentObserver::OnChange(
    /* [in] */ Boolean selfChange,
    /* [in] */ IUri* uri)
{
    mOwner->UpdatePointerSpeedFromSettings();
    return NOERROR;
}

//------------------------------------------------------------------------------
//  CInputManagerService::RegisterShowTouchesSettingObserver_ContentObserver
//------------------------------------------------------------------------------

CInputManagerService::RegisterShowTouchesSettingObserver_ContentObserver::RegisterShowTouchesSettingObserver_ContentObserver(
    /* [in] */ CInputManagerService* owner,
    /* [in] */ IHandler* handler)
    : mOwner(owner)
{
    constructor(handler);
}

ECode CInputManagerService::RegisterShowTouchesSettingObserver_ContentObserver::OnChange(
    /* [in] */ Boolean selfChange,
    /* [in] */ IUri* uri)
{
    mOwner->UpdateShowTouchesFromSettings();
    return NOERROR;
}


//==============================================================================
//  CInputManagerService
//==============================================================================
const String CInputManagerService::TAG("InputManager");
const Boolean CInputManagerService::DEBUG;
const String CInputManagerService::EXCLUDED_DEVICES_PATH("etc/excluded-input-devices.xml");

const Int32 CInputManagerService::MSG_DELIVER_INPUT_DEVICES_CHANGED;
const Int32 CInputManagerService::MSG_SWITCH_KEYBOARD_LAYOUT;
const Int32 CInputManagerService::MSG_RELOAD_KEYBOARD_LAYOUTS;
const Int32 CInputManagerService::MSG_UPDATE_KEYBOARD_LAYOUTS;
const Int32 CInputManagerService::MSG_RELOAD_DEVICE_ALIASES;

const Int32 CInputManagerService::INPUT_EVENT_INJECTION_SUCCEEDED;
const Int32 CInputManagerService::INPUT_EVENT_INJECTION_PERMISSION_DENIED;
const Int32 CInputManagerService::INPUT_EVENT_INJECTION_FAILED;
const Int32 CInputManagerService::INPUT_EVENT_INJECTION_TIMED_OUT;

const Int32 CInputManagerService::INJECTION_TIMEOUT_MILLIS;
const Int32 CInputManagerService::KEY_STATE_UNKNOWN;

const Int32 CInputManagerService::KEY_STATE_UP;

const Int32 CInputManagerService::KEY_STATE_DOWN;

const Int32 CInputManagerService::KEY_STATE_VIRTUAL;

const Int32 CInputManagerService::BTN_MOUSE;

const Int32 CInputManagerService::SW_LID;

const Int32 CInputManagerService::SW_KEYPAD_SLIDE;

const Int32 CInputManagerService::SW_HEADPHONE_INSERT;

const Int32 CInputManagerService::SW_MICROPHONE_INSERT;

const Int32 CInputManagerService::SW_LINEOUT_INSERT;

const Int32 CInputManagerService::SW_JACK_PHYSICAL_INSERT;

const Int32 CInputManagerService::SW_CAMERA_LENS_COVER;

const Int32 CInputManagerService::SW_LID_BIT;
const Int32 CInputManagerService::SW_KEYPAD_SLIDE_BIT;
const Int32 CInputManagerService::SW_HEADPHONE_INSERT_BIT;
const Int32 CInputManagerService::SW_MICROPHONE_INSERT_BIT;
const Int32 CInputManagerService::SW_LINEOUT_INSERT_BIT;
const Int32 CInputManagerService::SW_JACK_PHYSICAL_INSERT_BIT;
const Int32 CInputManagerService::SW_JACK_BITS;
const Int32 CInputManagerService::SW_CAMERA_LENS_COVER_BIT;

CAR_INTERFACE_IMPL_3(CInputManagerService, Object, IIInputManager, IWatchdogMonitor, IBinder);

CInputManagerService::CInputManagerService()
    : mSystemReady(FALSE)
    , mInputDevicesChangedPending(FALSE)
    , mKeyboardLayoutNotificationShown(FALSE)
    , mNextVibratorTokenValue(0)
    , mUseDevInputEventForAudioJack(FALSE)
{
    mDataStore = new PersistentDataStore();
    mInputDevices = ArrayOf<IInputDevice*>::Alloc(0);
    CSparseArray::New((ISparseArray**)&mInputDevicesChangedListeners);
    CArrayList::New((IArrayList**)&mTempInputDevicesChangedListenersToNotify);
    CArrayList::New((IArrayList**)&mTempFullKeyboards);
}

CInputManagerService::~CInputManagerService()
{}

ECode CInputManagerService::constructor(
    /* [in] */ IContext* context)
{
    mContext = context;
    AutoPtr<ILooper> looper;
    DisplayThread::Get()->GetLooper((ILooper**)&looper);
    mHandler = new InputManagerHandler(looper, this);

    AutoPtr<IResources> resources;
    context->GetResources((IResources**)&resources);
    resources->GetBoolean(R::bool_::config_useDevInputEventForAudioJack, &mUseDevInputEventForAudioJack);
    Slogger::I(TAG, "Initializing input manager, mUseDevInputEventForAudioJack=%d",
            mUseDevInputEventForAudioJack);
    looper = NULL;
    mHandler->GetLooper((ILooper**)&looper);
    AutoPtr<IMessageQueue> queue;
    looper->GetQueue((IMessageQueue**)&queue);
    NativeInit(this, mContext, queue);

    LocalServices::AddService(EIID_IInputManagerInternal, (IInputManagerInternal*)new LocalService(this));
    return NOERROR;
}

/**
 * these native functions come from ./frameworks/base/services/core/jni/com_android_server_input_InputManagerService.cpp
 */
void CInputManagerService::NativeInit(
    /* [in] */ CInputManagerService* service,
    /* [in] */ IContext* context,
    /* [in] */ IMessageQueue* messageQueue)
{
    AutoPtr<IMessageQueue> cq;
    mHandler->GetMessageQueue((IMessageQueue**)&cq);
    Handle32 hq;
    cq->GetNativeMessageQueue(&hq);
    assert(hq);
    NativeMessageQueue* queue = (NativeMessageQueue*)hq;
    mNativeInputManager = new NativeInputManager(mContext, this, queue->GetLooper());
}

ECode CInputManagerService::NativeStart()
{
    android::status_t result = mNativeInputManager->getInputManager()->start();
    if (result) {
        Logger::E(TAG, "Input manager could not be started.");
        return E_RUNTIME_EXCEPTION;
    }

    return NOERROR;
}

void CInputManagerService::NativeSetDisplayViewport(
    /* [in] */ Boolean external,
    /* [in] */ Int32 displayId,
    /* [in] */ Int32 rotation,
    /* [in] */ Int32 logicalLeft,
    /* [in] */ Int32 logicalTop,
    /* [in] */ Int32 logicalRight,
    /* [in] */ Int32 logicalBottom,
    /* [in] */ Int32 physicalLeft,
    /* [in] */ Int32 physicalTop,
    /* [in] */ Int32 physicalRight,
    /* [in] */ Int32 physicalBottom,
    /* [in] */ Int32 deviceWidth,
    /* [in] */ Int32 deviceHeight)
{
    android::DisplayViewport v;
    v.displayId = displayId;
    v.orientation = rotation;
    v.logicalLeft = logicalLeft;
    v.logicalTop = logicalTop;
    v.logicalRight = logicalRight;
    v.logicalBottom = logicalBottom;
    v.physicalLeft = physicalLeft;
    v.physicalTop = physicalTop;
    v.physicalRight = physicalRight;
    v.physicalBottom = physicalBottom;
    v.deviceWidth = deviceWidth;
    v.deviceHeight = deviceHeight;
    mNativeInputManager->setDisplayViewport(external, v);
}

Int32 CInputManagerService::NativeGetScanCodeState(
    /* [in] */ Int32 deviceId,
    /* [in] */ Int32 sourceMask,
    /* [in] */ Int32 scanCode)
{
    return mNativeInputManager->getInputManager()->getReader()->getScanCodeState(
            deviceId, uint32_t(sourceMask), scanCode);
}

Int32 CInputManagerService::NativeGetKeyCodeState(
    /* [in] */ Int32 deviceId,
    /* [in] */ Int32 sourceMask,
    /* [in] */ Int32 keyCode)
{
    return mNativeInputManager->getInputManager()->getReader()->getKeyCodeState(
            deviceId, uint32_t(sourceMask), keyCode);
}

Int32 CInputManagerService::NativeGetSwitchState(
    /* [in] */ Int32 deviceId,
    /* [in] */ Int32 sourceMask,
    /* [in] */ Int32 sw)
{
    return mNativeInputManager->getInputManager()->getReader()->getSwitchState(
            deviceId, uint32_t(sourceMask), sw);
}

Boolean CInputManagerService::NativeHasKeys(
    /* [in] */ Int32 deviceId,
    /* [in] */ Int32 sourceMask,
    /* [in] */ const ArrayOf<Int32>& keyCodes,
    /* [in] */ ArrayOf<Boolean>* keyExists)
{
    int32_t* codes = (int32_t*)keyCodes.GetPayload();
    uint8_t* flags = (uint8_t*)keyExists->GetPayload();
    Int32 numCodes = keyCodes.GetLength();
    if (numCodes == keyExists->GetLength()) {
        return mNativeInputManager->getInputManager()->getReader()->hasKeys(
                deviceId, uint32_t(sourceMask), numCodes, codes, flags);
    }

    return FALSE;
}

static void HandleInputChannelDisposed(
    /* [in] */ IInputChannel* inputChannelObj,
    /* [in] */ const android::sp<android::InputChannel>& inputChannel,
    /* [in] */ void* data)
{
    NativeInputManager* im = static_cast<NativeInputManager*>(data);
    im->unregisterInputChannel(inputChannel);
}

ECode CInputManagerService::NativeRegisterInputChannel(
    /* [in] */ IInputChannel* inputChannelObj,
    /* [in] */ InputWindowHandle* inputWindowHandleObj,
    /* [in] */ Boolean monitor)
{
    Handle32 nativeInputChannel;
    //inputChannelObj->GetNativeInputChannel(&nativeInputChannel);
    Elastos::Droid::View::InputChannel* otherObj = (Elastos::Droid::View::InputChannel*)inputChannelObj;
    nativeInputChannel = (Handle32)(otherObj->mNative);

    android::sp<android::InputChannel> inputChannel =
        ((NativeInputChannel*)nativeInputChannel)->getInputChannel();
    if (inputChannel == NULL) {
        Logger::E(TAG, "inputChannel is not initialized");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    android::sp<android::InputWindowHandle> inputWindowHandle =
            InputWindowHandle::GetHandle(inputWindowHandleObj);

    android::status_t status = mNativeInputManager->registerInputChannel(
            inputChannel, inputWindowHandle, monitor);
    if (status) {
        Logger::E(TAG, "Failed to register input channel.  status=%d", status);
        return E_RUNTIME_EXCEPTION;
    }

    if (!monitor) {
        ((NativeInputChannel*)nativeInputChannel)->setDisposeCallback(
            HandleInputChannelDisposed, (void*)mNativeInputManager.get());
    }

    return NOERROR;
}

ECode CInputManagerService::NativeUnregisterInputChannel(
    /* [in] */ IInputChannel* inputChannelObj)
{
    Handle32 nativeInputChannel;
    //inputChannelObj->GetNativeInputChannel(&nativeInputChannel);
    Elastos::Droid::View::InputChannel* otherObj = (Elastos::Droid::View::InputChannel*)inputChannelObj;
    nativeInputChannel = (Handle32)(otherObj->mNative);

    android::sp<android::InputChannel> inputChannel =
        ((NativeInputChannel*)nativeInputChannel)->getInputChannel();
    if (inputChannel == NULL) {
        Logger::E(TAG, "inputChannel is not initialized");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    ((NativeInputChannel*)nativeInputChannel)->setDisposeCallback(NULL, NULL);

    android::status_t status = mNativeInputManager->unregisterInputChannel(inputChannel);
    if (status && status != android::BAD_VALUE) { // ignore already unregistered channel
        Logger::E(TAG, "Failed to unregister input channel.  status=%d", status);
        return E_RUNTIME_EXCEPTION;
    }

    return NOERROR;
}

void CInputManagerService::NativeSetInputFilterEnabled(
    /* [in] */ Boolean enable)
{
    mNativeInputManager->getInputManager()->getDispatcher()->setInputFilterEnabled(enable);
}

static void KeyEvent_toNative(
        /* [in] */ IKeyEvent* event,
        /* [in] */ android::KeyEvent* androidEvent)
{
    Int32 deviceId;
    event->GetKeyboardDevice(&deviceId);
    Int32 source;
    IInputEvent::Probe(event)->GetSource(&source);
    Int32 metaState;
    event->GetMetaState(&metaState);
    Int32 action;
    event->GetAction(&action);
    Int32 keyCode;
    event->GetKeyCode(&keyCode);
    Int32 scanCode;
    event->GetScanCode(&scanCode);
    Int32 repeatCount;
    event->GetRepeatCount(&repeatCount);
    Int32 flags;
    event->GetFlags(&flags);
    Int64 downTime;
    event->GetDownTime(&downTime);
    Int64 eventTime;
    IInputEvent::Probe(event)->GetEventTime(&eventTime);

    androidEvent->initialize(deviceId, source, action, flags, keyCode, scanCode, metaState, repeatCount,
            milliseconds_to_nanoseconds(downTime),
            milliseconds_to_nanoseconds(eventTime));
}

static void MotionEvent_toNative(
    /* [in] */ IMotionEvent* event,
    /* [in] */ android::MotionEvent* androidEvent)
{

    AutoPtr<IParcel> parcel;
    CParcel::New((IParcel**)&parcel);
    IParcelable::Probe(event)->WriteToParcel(parcel);

    android::Parcel* androidParcel;
    parcel->GetElementPayload((Handle32*)&androidParcel);

    androidParcel->setDataPosition(0);
    androidParcel->readInt32();
    androidEvent->readFromParcel(androidParcel);
}

Int32 CInputManagerService::NativeInjectInputEvent(
    /* [in] */ IInputEvent* event,
    /* [in] */ Int32 displayId,
    /* [in] */ Int32 injectorPid,
    /* [in] */ Int32 injectorUid,
    /* [in] */ Int32 syncMode,
    /* [in] */ Int32 timeoutMillis,
    /* [in] */ Int32 policyFlags)
{
    if (IKeyEvent::Probe(event)) {
        android::KeyEvent keyEvent;
        KeyEvent_toNative(IKeyEvent::Probe(event), &keyEvent);

        return mNativeInputManager->getInputManager()->getDispatcher()->injectInputEvent(
                &keyEvent, displayId, injectorPid, injectorUid, syncMode, timeoutMillis,
                uint32_t(policyFlags));
    }
    else if (IMotionEvent::Probe(event)) {
        android::MotionEvent motionEvent;
        MotionEvent_toNative(IMotionEvent::Probe(event), &motionEvent);

        return mNativeInputManager->getInputManager()->getDispatcher()->injectInputEvent(
                &motionEvent, displayId, injectorPid, injectorUid, syncMode, timeoutMillis,
                uint32_t(policyFlags));
    }
    else {
        Logger::E(TAG, "Invalid input event type.");
        return INPUT_EVENT_INJECTION_FAILED;
    }
}

void CInputManagerService::NativeSetInputWindows(
    /* [in] */ ArrayOf<InputWindowHandle*>* windowHandles)
{
    mNativeInputManager->setInputWindows(windowHandles);
}

void CInputManagerService::NativeSetInputDispatchMode(
    /* [in] */ Boolean enabled,
    /* [in] */ Boolean frozen)
{
    mNativeInputManager->setInputDispatchMode(enabled, frozen);
}

void CInputManagerService::NativeSetSystemUiVisibility(
    /* [in] */ Int32 visibility)
{
    mNativeInputManager->setSystemUiVisibility(visibility);
}

void CInputManagerService::NativeSetFocusedApplication(
    /* [in] */ InputApplicationHandle* application)
{
    mNativeInputManager->setFocusedApplication(application);
}

Boolean CInputManagerService::NativeTransferTouchFocus(
    /* [in] */ IInputChannel* fromChannelObj,
    /* [in] */ IInputChannel* toChannelObj)
{
    Handle32 nativeInputChannel;
    //fromChannelObj->GetNativeInputChannel(&nativeInputChannel);
    Elastos::Droid::View::InputChannel* otherObj = (Elastos::Droid::View::InputChannel*)fromChannelObj;
    nativeInputChannel = (Handle32)(otherObj->mNative);

    android::sp<android::InputChannel> fromChannel =
        ((NativeInputChannel*)nativeInputChannel)->getInputChannel();

    //toChannelObj->GetNativeInputChannel(&nativeInputChannel);
    otherObj = (Elastos::Droid::View::InputChannel*)toChannelObj;
    nativeInputChannel = (Handle32)(otherObj->mNative);

    android::sp<android::InputChannel> toChannel =
        ((NativeInputChannel*)nativeInputChannel)->getInputChannel();


    if (fromChannel == NULL || toChannel == NULL) {
        return false;
    }

    return mNativeInputManager->getInputManager()->getDispatcher()->
            transferTouchFocus(fromChannel, toChannel);
}

void CInputManagerService::NativeSetPointerSpeed(
    /* [in] */ Int32 speed)
{
    mNativeInputManager->setPointerSpeed(speed);
}

void CInputManagerService::NativeSetShowTouches(
    /* [in] */ Boolean enabled)
{
    mNativeInputManager->setShowTouches(enabled);
}

void CInputManagerService::NativeSetInteractive(
    /* [in] */ Boolean interactive)
{

}

void CInputManagerService::NativeReloadCalibration()
{

}

void CInputManagerService::NativeVibrate(
    /* [in] */ Int32 deviceId,
    /* [in] */ const ArrayOf<Int64>& pattern,
    /* [in] */ Int32 repeat,
    /* [in] */ Int32 token)
{
    size_t patternSize = pattern.GetLength();
    if (patternSize > MAX_VIBRATE_PATTERN_SIZE) {
        ALOGI("Skipped requested vibration because the pattern size is %d "
                "which is more than the maximum supported size of %d.",
                patternSize, MAX_VIBRATE_PATTERN_SIZE);
        return; // limit to reasonable size
    }

    nsecs_t patternTmp[patternSize];
    for (size_t i = 0; i < patternSize; i++) {
        patternTmp[i] = Elastos::Core::Math::Max(Int64(0), Elastos::Core::Math::Min(pattern[i],
            MAX_VIBRATE_PATTERN_DELAY_NSECS / 1000000LL)) * 1000000LL;
    }

    mNativeInputManager->getInputManager()->getReader()->vibrate(
        deviceId, patternTmp, patternSize, repeat, token);
}

void CInputManagerService::NativeCancelVibrate(
    /* [in] */ Int32 deviceId,
    /* [in] */ Int32 token)
{
    mNativeInputManager->getInputManager()->getReader()->cancelVibrate(deviceId, token);
}

void CInputManagerService::NativeReloadKeyboardLayouts()
{
    mNativeInputManager->getInputManager()->getReader()->requestRefreshConfiguration(
        android::InputReaderConfiguration::CHANGE_KEYBOARD_LAYOUTS);
}

void CInputManagerService::NativeReloadDeviceAliases()
{
    mNativeInputManager->getInputManager()->getReader()->requestRefreshConfiguration(
        android::InputReaderConfiguration::CHANGE_DEVICE_ALIAS);
}

String CInputManagerService::NativeDump()
{
    android::String8 dump;
    mNativeInputManager->dump(dump);
    return String(dump.string());
}

void CInputManagerService::NativeMonitor()
{
    mNativeInputManager->getInputManager()->getReader()->monitor();
    mNativeInputManager->getInputManager()->getDispatcher()->monitor();
}


void CInputManagerService::SetWindowManagerCallbacks(
    /* [in] */ IWindowManagerCallbacks* cbacks)
{
    mWindowManagerCallbacks = cbacks;
}

void CInputManagerService::SetWiredAccessoryCallbacks(
    /* [in] */ IWiredAccessoryCallbacks* cbacks)
{
    mWiredAccessoryCallbacks = cbacks;
}

ECode CInputManagerService::Start()
{
    Slogger::I(TAG, "Starting input manager");
    FAIL_RETURN(NativeStart());

    // Add ourself to the Watchdog monitors.
    // AutoPtr<IWatchdog> watchdog;
    // CWatchdog::AcquireSingleton((IWatchdog**)&watchdog);
    // watchdog->AddMonitor(IMonitor::Probe(this));

    RegisterPointerSpeedSettingObserver();
    RegisterShowTouchesSettingObserver();

    AutoPtr<IIntentFilter> intentFilter;
    CIntentFilter::New(IIntent::ACTION_USER_SWITCHED, (IIntentFilter**)&intentFilter);
    AutoPtr<Start_BroadcastReceiver> receiver = new Start_BroadcastReceiver(this);
    AutoPtr<IIntent> result;
    mContext->RegisterReceiver(
        receiver, intentFilter, String(NULL),
        mHandler, (IIntent**)&result);

    UpdatePointerSpeedFromSettings();
    UpdateShowTouchesFromSettings();

    return NOERROR;
}

// TODO(BT) Pass in paramter for bluetooth system
void CInputManagerService::SystemRunning()
{
    if (DEBUG) {
        Slogger::D(TAG, "System ready.");
    }
    AutoPtr<IInterface> service;
    mContext->GetSystemService(IContext::NOTIFICATION_SERVICE, (IInterface**)&service);
    mNotificationManager = INotificationManager::Probe(service);
    mSystemReady = TRUE;

    AutoPtr<IIntentFilter> filter;
    CIntentFilter::New(IIntent::ACTION_PACKAGE_ADDED, (IIntentFilter**)&filter);
    filter->AddAction(IIntent::ACTION_PACKAGE_REMOVED);
    filter->AddAction(IIntent::ACTION_PACKAGE_CHANGED);
    filter->AddAction(IIntent::ACTION_PACKAGE_REPLACED);
    filter->AddDataScheme(String("package"));
    AutoPtr<SystemRunning_BroadcastReceiver_1> myBREx = new SystemRunning_BroadcastReceiver_1(this);
    AutoPtr<IIntent> result;
    mContext->RegisterReceiver(
        myBREx.Get(), filter, String(NULL), mHandler, (IIntent**)&result);

    filter = NULL;
    result = NULL;
    CIntentFilter::New(IBluetoothDevice::ACTION_ALIAS_CHANGED, (IIntentFilter**)&filter);
    AutoPtr<SystemRunning_BroadcastReceiver_2> myBREx2 = new SystemRunning_BroadcastReceiver_2(this);
    mContext->RegisterReceiver(
        myBREx2, filter,String(NULL), mHandler, (IIntent**)&result);

    Boolean bval;
    mHandler->SendEmptyMessage(MSG_RELOAD_DEVICE_ALIASES, &bval);
    mHandler->SendEmptyMessage(MSG_UPDATE_KEYBOARD_LAYOUTS, &bval);
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
Int32 CInputManagerService::GetKeyCodeState(
    /* [in] */ Int32 deviceId,
    /* [in] */ Int32 sourceMask,
    /* [in] */ Int32 keyCode)
{
    return NativeGetKeyCodeState(deviceId, sourceMask, keyCode);
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
Int32 CInputManagerService::GetScanCodeState(
    /* [in] */ Int32 deviceId,
    /* [in] */ Int32 sourceMask,
    /* [in] */ Int32 scanCode)
{
    return NativeGetScanCodeState(deviceId, sourceMask, scanCode);
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
Int32 CInputManagerService::GetSwitchState(
    /* [in] */ Int32 deviceId,
    /* [in] */ Int32 sourceMask,
    /* [in] */ Int32 switchCode)
{
    return NativeGetSwitchState(deviceId, sourceMask, switchCode);
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
ECode CInputManagerService::HasKeys(
    /* [in] */ Int32 deviceId,
    /* [in] */ Int32 sourceMask,
    /* [in] */ const ArrayOf<Int32>& keyCodes,
    /* [in] */ ArrayOf<Boolean>* keyExists,
    /* [out] */ Boolean* hasKeys)
{
    VALIDATE_NOT_NULL(hasKeys);
    *hasKeys = FALSE;

    if (keyCodes.GetLength() <= 0) {
        Slogger::E(TAG, "keyCodes must not be NULL");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (keyExists->GetLength() < keyCodes.GetLength()) {
        Slogger::E(TAG, "keyExists must not be NULL and must be at "
            "least as large as keyCodes.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    *hasKeys = NativeHasKeys(deviceId, sourceMask, keyCodes, keyExists);
    return NOERROR;
}

/**
 * Creates an input channel that will receive all input from the input dispatcher.
 * @param inputChannelName The input channel name.
 * @return The input channel.
 */
ECode CInputManagerService::MonitorInput(
    /* [in] */ const String& inputChannelName,
    /* [out] */ IInputChannel** ic)
{
    VALIDATE_NOT_NULL(ic);
    *ic = NULL;

    if (inputChannelName.IsNull()) {
        Slogger::E(TAG, "inputChannelName must not be NULL");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoPtr<IInputChannelHelper> icHelper;
    CInputChannelHelper::AcquireSingleton((IInputChannelHelper**)&icHelper);
    AutoPtr<IInputChannel> inputChannel0;
    AutoPtr<IInputChannel> inputChannel1;
    AutoPtr<ArrayOf<IInputChannel*> > icp;
    FAIL_RETURN(icHelper->OpenInputChannelPair(inputChannelName, (ArrayOf<IInputChannel*>**)&icp));
    inputChannel0 = (*icp)[0];
    inputChannel1 = (*icp)[1];
    FAIL_RETURN(NativeRegisterInputChannel(inputChannel0, NULL, TRUE));
    FAIL_RETURN(inputChannel0->Dispose()); // don't need to retain the Java object reference
    *ic = inputChannel1;
    REFCOUNT_ADD(*ic);

    return NOERROR;
}

/**
 * Registers an input channel so that it can be used as an input event target.
 * @param inputChannel The input channel to register.
 * @param inputWindowHandle The handle of the input window associated with the
 * input channel, or null if none.
 */
ECode CInputManagerService::RegisterInputChannel(
    /* [in] */ IInputChannel* inputChannel,
    /* [in] */ InputWindowHandle* inputWindowHandle)
{
    if (inputChannel == NULL) {
        Slogger::E(TAG, "inputChannel must not be NULL");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    return NativeRegisterInputChannel(inputChannel, inputWindowHandle, FALSE);
}

/**
 * Unregisters an input channel.
 * @param inputChannel The input channel to unregister.
 */
ECode CInputManagerService::UnregisterInputChannel(
    /* [in] */ IInputChannel* inputChannel)
{
    if (inputChannel == NULL) {
        Slogger::E(TAG, "inputChannel must not be NULL");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    return NativeUnregisterInputChannel(inputChannel);
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
void CInputManagerService::SetInputFilter(
    /* [in] */ IIInputFilter* filter)
{
    synchronized (mInputFilterLock) {
        IIInputFilter* oldFilter = mInputFilter;
        if (oldFilter == filter) {
            return; // nothing to do
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

        NativeSetInputFilterEnabled(filter != NULL);
    }
}

//// @Override // Binder call
ECode CInputManagerService::InjectInputEvent(
    /* [in] */ IInputEvent* event,
    /* [in] */ Int32 mode,
    /* [out] */ Boolean* injectIt)
{
    InjectInputEventInternal(event, IDisplay::DEFAULT_DISPLAY, mode, injectIt);
    return NOERROR;
}

/**
 * Gets information about the input device with the specified id.
 * @param deviceId The device id.
 * @return The input device or null if not found.
 */
//// @Override // Binder call
ECode CInputManagerService::GetInputDevice(
    /* [in] */ Int32 deviceId,
    /* [out, callee] */ IInputDevice** inputDevice)
{
    VALIDATE_NOT_NULL(inputDevice);
    *inputDevice = NULL;

    synchronized (mInputDevicesLock) {
        Int32 count = mInputDevices->GetLength();
        for (Int32 i = 0; i < count; i++) {
            AutoPtr<IInputDevice> device = (*mInputDevices)[i];
            Int32 id;
            device->GetId(&id);
            if (id == deviceId) {
                *inputDevice = device;
                REFCOUNT_ADD(*inputDevice);

                return NOERROR;
            }
        }
    }

    return NOERROR;
}

/**
 * Gets the ids of all input devices in the system.
 * @return The input device ids.
 */
//// @Override // Binder call
ECode CInputManagerService::GetInputDeviceIds(
    /* [out, callee] */ ArrayOf<Int32>** deviceIds)
{
    VALIDATE_NOT_NULL(deviceIds);

    synchronized (mInputDevicesLock) {
        Int32 count = mInputDevices->GetLength();
        *deviceIds = ArrayOf<Int32>::Alloc(count);
        if (*deviceIds == NULL)
            return E_OUT_OF_MEMORY_ERROR;

        REFCOUNT_ADD(*deviceIds);
        for (Int32 i = 0; i < count; i++) {
            Int32 id;
            (*mInputDevices)[i]->GetId(&id);
            (**deviceIds)[i] = id;
        }
    }

    return NOERROR;
}

/**
 * Gets all input devices in the system.
 * @return The array of input devices.
 */
ECode CInputManagerService::GetInputDevices(
    /* [out, callee] */ ArrayOf<IInputDevice*>** inputDevices )
{
    synchronized (mInputDevicesLock) {
        *inputDevices = mInputDevices;
    }

    return NOERROR;
}

// @Override // Binder call
ECode CInputManagerService::RegisterInputDevicesChangedListener(
    /* [in] */ IInputDevicesChangedListener* listener)
{
    if (listener == NULL) {
        Slogger::E(TAG, "listener must not be NULL");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    synchronized (mInputDevicesLock) {

        Int32 callingPid = Binder::GetCallingPid();
        AutoPtr<IInterface> outface;

        mInputDevicesChangedListeners->Get(callingPid, (IInterface**)&outface);

        if (outface != NULL) {
            Slogger::E(TAG, "The calling process has already "
                "registered an InputDevicesChangedListener.");
            return E_SECURITY_EXCEPTION;
        }

        AutoPtr<InputDevicesChangedListenerRecord> record =
            new InputDevicesChangedListenerRecord(callingPid, listener, this);

        AutoPtr<IProxy> proxy = (IProxy*)listener->Probe(EIID_IProxy);
        if (proxy != NULL && FAILED(proxy->LinkToDeath(record, 0))) {
            // give up
            return E_RUNTIME_EXCEPTION;
        }

        mInputDevicesChangedListeners->Put(callingPid, TO_IINTERFACE(record));
    }

    return NOERROR;
}

// @Override // Binder call & native callback
ECode CInputManagerService::GetTouchCalibrationForInputDevice(
    /* [in] */ const String& inputDeviceDescriptor,
    /* [in] */ Int32 surfaceRotation,
    /* [out] */ ITouchCalibration** inputDevice)
{
    if (inputDeviceDescriptor == NULL) {
        //throw new IllegalArgumentException("inputDeviceDescriptor must not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    synchronized (mDataStore) {
        *inputDevice = mDataStore->GetTouchCalibration(inputDeviceDescriptor, surfaceRotation);
    }

    return NOERROR;
}

// @Override // Binder call
ECode CInputManagerService::SetTouchCalibrationForInputDevice(
    /* [in] */ const String& inputDeviceDescriptor,
    /* [in] */ Int32 surfaceRotation,
    /* [in] */ ITouchCalibration* calibration)
{
    if (!CheckCallingPermission(Manifest::permission::SET_INPUT_CALIBRATION,
            String("setTouchCalibrationForInputDevice()")) ) {
        //throw new SecurityException("Requires SET_INPUT_CALIBRATION permission");
        return E_SECURITY_EXCEPTION;
    }
    if (inputDeviceDescriptor == NULL) {
        //throw new IllegalArgumentException("inputDeviceDescriptor must not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (calibration == NULL) {
        //throw new IllegalArgumentException("calibration must not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (surfaceRotation < ISurface::ROTATION_0 || surfaceRotation > ISurface::ROTATION_270) {
        //throw new IllegalArgumentException("surfaceRotation value out of bounds");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    synchronized (mDataStore) {
        if (mDataStore->SetTouchCalibration(inputDeviceDescriptor, surfaceRotation,
                calibration)) {
            NativeReloadCalibration();
        }
        mDataStore->SaveIfNeeded();
    }

    return NOERROR;
}

// @Override // Binder call
ECode CInputManagerService::GetKeyboardLayouts(
    /* [out, callee] */ ArrayOf<IKeyboardLayout*>** kbLayouts)
{
    VALIDATE_NOT_NULL(kbLayouts);

    List<AutoPtr<IKeyboardLayout> > list;
    AutoPtr<GetKeyboardLayouts_KeyboardLayoutVisitor> myKVEx = new GetKeyboardLayouts_KeyboardLayoutVisitor(list);
    VisitAllKeyboardLayouts(myKVEx);

    *kbLayouts = ArrayOf<IKeyboardLayout*>::Alloc(list.GetSize());
    if (*kbLayouts == NULL)
        return E_OUT_OF_MEMORY_ERROR;

    REFCOUNT_ADD(*kbLayouts);

    List<AutoPtr<IKeyboardLayout> >::Iterator iter = list.Begin();
    for (Int32 i = 0; iter != list.End(); ++iter, ++i) {
        (*kbLayouts)->Set(i, iter->Get());
    }

    return NOERROR;
}

// @Override // Binder call
ECode CInputManagerService::GetKeyboardLayout(
    /* [in] */ const String& keyboardLayoutDescriptor,
    /* [out, callee] */ IKeyboardLayout** kbLayout)
{
    VALIDATE_NOT_NULL(kbLayout);
    *kbLayout = NULL;

    if (keyboardLayoutDescriptor.IsNull()) {
        Slogger::E(TAG, "keyboardLayoutDescriptor must not be NULL");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoPtr<GetKeyboardLayout_KeyboardLayoutVisitor> myKVEx2 = new GetKeyboardLayout_KeyboardLayoutVisitor(kbLayout);
    VisitKeyboardLayout(keyboardLayoutDescriptor, myKVEx2);
    if (*kbLayout == NULL) {
        Logger::W(TAG, "Could not get keyboard kbLayout with descriptor '%d'.",
            keyboardLayoutDescriptor.string());
    }

    return NOERROR;
}

// @Override // Binder call
ECode CInputManagerService::GetCurrentKeyboardLayoutForInputDevice(
    /* [in] */ IInputDeviceIdentifier* identifier,
    /* [out, callee] */ String* kbLayout)
{
    String key;
    GetLayoutDescriptor(identifier, &key);

    synchronized (mDataStore) {
        // try loading it using the layout descriptor if we have it
        String layout = mDataStore->GetCurrentKeyboardLayout(key);
        String str;

        identifier->GetDescriptor(&str);
        if (layout == NULL && !key.Equals(str)) {
            // if it doesn't exist fall back to the device descriptor
            layout = mDataStore->GetCurrentKeyboardLayout(str);
        }
        if (DEBUG) {
            Slogger::D(TAG, "Loaded keyboard layout id for %s and got %s", key.string(), layout.string());
        }
        *kbLayout = layout;
    }

    return NOERROR;
}

// @Override // Binder call
ECode CInputManagerService::SetCurrentKeyboardLayoutForInputDevice(
    /* [in] */ IInputDeviceIdentifier* identifier,
    /* [in] */ const String& keyboardLayoutDescriptor)
{
    if (!CheckCallingPermission(
        Elastos::Droid::Manifest::permission::SET_KEYBOARD_LAYOUT,
        String("setCurrentKeyboardLayoutForInputDevice()"))) {
        Slogger::E(TAG, "Requires SET_KEYBOARD_LAYOUT permission");
        return E_SECURITY_EXCEPTION;
    }

    if (keyboardLayoutDescriptor.IsNull()) {
        Slogger::E(TAG, "keyboardLayoutDescriptor must not be NULL");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    String key;
    GetLayoutDescriptor(identifier, &key);

    synchronized (mDataStore) {
        if (mDataStore->SetCurrentKeyboardLayout(key, keyboardLayoutDescriptor)) {

            if (DEBUG) {
                Slogger::D(TAG, "Saved keyboard layout using %s", key.string());
            }

            Boolean result;
            mHandler->SendEmptyMessage(MSG_RELOAD_KEYBOARD_LAYOUTS, &result);
        }
        mDataStore->SaveIfNeeded();
    }

    return NOERROR;
}

// @Override // Binder call
ECode CInputManagerService::GetKeyboardLayoutsForInputDevice(
    /* [in] */ IInputDeviceIdentifier* identifier,
    /* [out, callee] */ ArrayOf<String>** kbLayouts)
{
    String key;
    GetLayoutDescriptor(identifier, &key);
    synchronized (mDataStore) {

        AutoPtr<ArrayOf<String> > layouts;
        String str;

        layouts = mDataStore->GetKeyboardLayouts(key);
        identifier->GetDescriptor(&str);
        if ((layouts == NULL || layouts->GetLength() == 0) && !key.Equals(str)) {
            layouts = mDataStore->GetKeyboardLayouts(str);
        }
        *kbLayouts = layouts;
    }

    return NOERROR;
}

// @Override // Binder call
ECode CInputManagerService::AddKeyboardLayoutForInputDevice(
    /* [in] */ IInputDeviceIdentifier* identifier,
    /* [in] */ const String& keyboardLayoutDescriptor)
{
    if (!CheckCallingPermission(
        Elastos::Droid::Manifest::permission::SET_KEYBOARD_LAYOUT,
        String("addKeyboardLayoutForInputDevice()"))) {
        Slogger::E(TAG, "Requires SET_KEYBOARD_LAYOUT permission");
        return E_SECURITY_EXCEPTION;
    }

    if (keyboardLayoutDescriptor.IsNull()) {
        Slogger::E(TAG, "keyboardLayoutDescriptor must not be NULL");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    String key;
    GetLayoutDescriptor(identifier, &key);
    synchronized (mDataStore) {
        String oldLayout = mDataStore->GetCurrentKeyboardLayout(key);
        String str;
        identifier->GetDescriptor(&str);
        if (oldLayout == NULL && !key.Equals(str)) {
            oldLayout = mDataStore->GetCurrentKeyboardLayout(str);
        }
        if (mDataStore->AddKeyboardLayout(key, keyboardLayoutDescriptor)
            && !oldLayout.Equals(mDataStore->GetCurrentKeyboardLayout(key))) {
            Boolean result;
            mHandler->SendEmptyMessage(MSG_RELOAD_KEYBOARD_LAYOUTS, &result);
        }

        mDataStore->SaveIfNeeded();
    }

    return NOERROR;
}

// @Override // Binder call
ECode CInputManagerService::RemoveKeyboardLayoutForInputDevice(
    /* [in] */ IInputDeviceIdentifier* identifier,
    /* [in] */ const String& keyboardLayoutDescriptor)
{
    if (!CheckCallingPermission(
        Elastos::Droid::Manifest::permission::SET_KEYBOARD_LAYOUT,
        String("removeKeyboardLayoutForInputDevice()"))) {
        Slogger::E(TAG, "Requires SET_KEYBOARD_LAYOUT permission");
        return E_SECURITY_EXCEPTION;
    }

    if (keyboardLayoutDescriptor.IsNull()) {
        Slogger::E(TAG, "keyboardLayoutDescriptor must not be NULL");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    String key;
    GetLayoutDescriptor(identifier, &key);
    synchronized (mDataStore) {
        String oldLayout = mDataStore->GetCurrentKeyboardLayout(key);
        String str;
        identifier->GetDescriptor(&str);
        if (oldLayout == NULL && !key.Equals(str)) {
            oldLayout = mDataStore->GetCurrentKeyboardLayout(str);
        }

        Boolean removed;
        removed = mDataStore->RemoveKeyboardLayout(key, keyboardLayoutDescriptor);
        if (!key.Equals(str)) {
            // We need to remove from both places to ensure it is gone
            removed |= mDataStore->RemoveKeyboardLayout(str, keyboardLayoutDescriptor);
        }

        if (removed && !oldLayout.Equals(mDataStore->GetCurrentKeyboardLayout(key))) {
            Boolean result;
            mHandler->SendEmptyMessage(MSG_RELOAD_KEYBOARD_LAYOUTS, &result);
        }
        mDataStore->SaveIfNeeded();
    }

    return NOERROR;
}

void CInputManagerService::SwitchKeyboardLayout(
    /* [in] */ Int32 deviceId,
    /* [in] */ Int32 direction)
{
    AutoPtr<IMessage> msg;
    mHandler->ObtainMessage(MSG_SWITCH_KEYBOARD_LAYOUT, (IMessage**)&msg);
    msg->SetArg1(deviceId);
    msg->SetArg2(direction);
    Boolean result;
    mHandler->SendMessage(msg, &result);
}

void CInputManagerService::SetInputWindows(
    /* [in] */ ArrayOf<InputWindowHandle*>* windowHandles)
{
    NativeSetInputWindows(windowHandles);
}

void CInputManagerService::SetFocusedApplication(
    /* [in] */ InputApplicationHandle* application)
{
    NativeSetFocusedApplication(application);
}

void CInputManagerService::SetInputDispatchMode(
    /* [in] */ Boolean enabled,
    /* [in] */ Boolean frozen)
{
    NativeSetInputDispatchMode(enabled, frozen);
}

void CInputManagerService::SetSystemUiVisibility(
    /* [in] */ Int32 visibility)
{
    NativeSetSystemUiVisibility(visibility);
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
ECode CInputManagerService::TransferTouchFocus(
    /* [in] */ IInputChannel* fromChannel,
    /* [in] */ IInputChannel* toChannel,
    /* [out] */ Boolean* transferIt)
{
    if (fromChannel == NULL) {
        Slogger::E(TAG, "fromChannel must not be NULL");
        assert(0);
        return FALSE;
        //return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (toChannel == NULL) {
        Slogger::E(TAG, "toChannel must not be NULL");
        assert(0);
        return FALSE;
        //return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    return NativeTransferTouchFocus(fromChannel, toChannel);
}

// @Override // Binder call
ECode CInputManagerService::TryPointerSpeed(
    /* [in] */ Int32 speed)
{
    if (!CheckCallingPermission(
        Elastos::Droid::Manifest::permission::SET_POINTER_SPEED,
        String("tryPointerSpeed()"))) {
        Slogger::E(TAG, "Requires SET_POINTER_SPEED permission");
        return E_SECURITY_EXCEPTION;
    }

    if (speed < IInputManager::MIN_POINTER_SPEED
        || speed > IInputManager::MAX_POINTER_SPEED) {
        Slogger::E(TAG, "speed out of range");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    SetPointerSpeedUnchecked(speed);

    return NOERROR;
}

void CInputManagerService::UpdatePointerSpeedFromSettings()
{
    Int32 speed = GetPointerSpeedSetting();
    SetPointerSpeedUnchecked(speed);
}

void CInputManagerService::UpdateShowTouchesFromSettings()
{
    Int32 setting = GetShowTouchesSetting(0);
    NativeSetShowTouches(setting != 0);
}

// Binder call
// @Override
ECode CInputManagerService::Vibrate(
    /* [in] */ Int32 deviceId,
    /* [in] */ const ArrayOf<Int64>& pattern,
    /* [in] */ Int32 repeat,
    /* [in] */ IBinder* token)
{
    if (repeat >= pattern.GetLength()) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    AutoPtr<VibratorToken> v;
    synchronized (mVibratorLock) {
        HashMap<AutoPtr<IBinder>, AutoPtr<VibratorToken> >::Iterator find
                = mVibratorTokens.Find(token);
        if (find == mVibratorTokens.End()) {
            v = new VibratorToken(deviceId, token, mNextVibratorTokenValue++, this);
            AutoPtr<IProxy> proxy = (IProxy*)token->Probe(EIID_IProxy);
            if (FAILED(proxy->LinkToDeath(v, 0))) {
                // give up
                return E_RUNTIME_EXCEPTION;
            }
            mVibratorTokens[token] =  v;
        }
    }

    synchronized (v) {
        v->mVibrating = TRUE;
        NativeVibrate(deviceId, pattern, repeat, v->mTokenValue);
    }

    return NOERROR;
}

// Binder call
// @Override
ECode CInputManagerService::CancelVibrate(
    /* [in] */ Int32 deviceId,
    /* [in] */ IBinder* token)
{
    AutoPtr<VibratorToken> v;

    synchronized (mVibratorLock) {
        HashMap<AutoPtr<IBinder>, AutoPtr<VibratorToken> >::Iterator find
                = mVibratorTokens.Find(token);
        if (find == mVibratorTokens.End() || find->mSecond->mDeviceId != deviceId) {
            return NOERROR; // nothing to cancel
        }
        v = find->mSecond;
    }

    CancelVibrateIfNeeded(v);

    return NOERROR;
}

// @Override
ECode CInputManagerService::Dump(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ IPrintWriter* pw,
    /* [in] */ ArrayOf<String>* args)
{
    Int32 result;
    FAIL_RETURN(mContext->CheckCallingOrSelfPermission(Elastos::Droid::Manifest::permission::DUMP, &result));
    if (result != IPackageManager::PERMISSION_GRANTED) {
        pw->Print(
            String("Permission Denial: can't dump InputManager from from pid=")
            + StringUtils::ToString(Binder::GetCallingPid())
            + ", uid=" + StringUtils::ToString(Binder::GetCallingUid()));
        return NOERROR;
    }

    pw->Print(String("INPUT MANAGER (dumpsys input)\n"));
    String dumpStr = NativeDump();
    if (!dumpStr.IsNull()) {
        pw->Print(dumpStr);
    }

    return NOERROR;
}

// Called by the heartbeat to ensure locks are not held indefinitely (for deadlock detection).
// @Override
ECode CInputManagerService::Monitor()
{
    synchronized (mInputFilterLock) {
        ;
    }
    NativeMonitor();

    return NOERROR;
}

ECode CInputManagerService::ToString(
    /* [out] */ String* str)
{
    *str = String("");
    return NOERROR;
}


void CInputManagerService::ReloadKeyboardLayouts()
{
    mNativeInputManager->getInputManager()->getReader()->requestRefreshConfiguration(
        android::InputReaderConfiguration::CHANGE_KEYBOARD_LAYOUTS);
}

void CInputManagerService::ReloadDeviceAliases()
{
    mNativeInputManager->getInputManager()->getReader()->requestRefreshConfiguration(
        android::InputReaderConfiguration::CHANGE_DEVICE_ALIAS);
}

void CInputManagerService::SetDisplayViewportsInternal(
    /* [in] */ IDisplayViewport* defaultViewport,
    /* [in] */ IDisplayViewport* externalTouchViewport)
{
/*
    AutoPtr<DisplayViewport> dvp;
    if (defaultViewport)
        dvp = reinterpret_cast<DisplayViewport*>(defaultViewport->Probe(EIID_DisplayViewport));

    AutoPtr<DisplayViewport> etvp;
    if (externalTouchViewport)
        etvp = reinterpret_cast<DisplayViewport*>(externalTouchViewport->Probe(EIID_DisplayViewport));

    if (dvp && dvp->mValid) {
        SetDisplayViewport(FALSE, dvp);
    }

    if (etvp && etvp->mValid) {
        SetDisplayViewport(TRUE, etvp);
    }
    else if (dvp && dvp->mValid) {
        SetDisplayViewport(TRUE, dvp);
    }
*/
}

void CInputManagerService::SetDisplayViewport(
    /* [in] */ Boolean external,
    /* [in] */ IDisplayViewport* viewport)
{
    Int32 logicalLeft, logicalTop, logicalRight, logicalBottom;
    AutoPtr<IRect> rect;

    viewport->GetLogicalFrame((IRect**)&rect);

    rect->GetLeft(&logicalLeft);
    rect->GetTop(&logicalTop);
    rect->GetRight(&logicalRight);
    rect->GetBottom(&logicalBottom);

    Int32 physicalLeft, physicalTop, physicalRight, physicalBottom;
    AutoPtr<IRect> rectP;
    viewport->GetPhysicalFrame((IRect**)&rectP);

    rectP->GetLeft(&physicalLeft);
    rectP->GetTop(&physicalTop);
    rectP->GetRight(&physicalRight);
    rectP->GetBottom(&physicalBottom);

    Int32 displayId, orientation, width, height;
    viewport->GetDisplayId(&displayId);
    viewport->GetOrientation(&orientation);
    viewport->GetDeviceWidth(&width);
    viewport->GetDeviceHeight(&height);

    NativeSetDisplayViewport(external,
        displayId, orientation,
        logicalLeft, logicalTop, logicalRight, logicalBottom,
        physicalLeft, physicalTop, physicalRight, physicalBottom,
        width, height);
}

ECode CInputManagerService::InjectInputEventInternal(
    /* [in] */ IInputEvent* event,
    /* [in] */ Int32 displayId,
    /* [in] */ Int32 mode,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;

    if (event == NULL) {
        Slogger::E(TAG, "event must not be NULL");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (mode != IInputManager::INJECT_INPUT_EVENT_MODE_ASYNC
        && mode != IInputManager::INJECT_INPUT_EVENT_MODE_WAIT_FOR_FINISH
        && mode != IInputManager::INJECT_INPUT_EVENT_MODE_WAIT_FOR_RESULT) {
        Slogger::E(TAG, "mode is invalid");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Int32 pid = Binder::GetCallingPid();
    Int32 uid = Binder::GetCallingUid();
    Int64 ident = Binder::ClearCallingIdentity();
    Int32 res;

    res = NativeInjectInputEvent(event, displayId, pid, uid, mode,
            INJECTION_TIMEOUT_MILLIS, IWindowManagerPolicy::FLAG_DISABLE_KEY_REPEAT);

    Binder::RestoreCallingIdentity(ident);

    switch (res) {
        case INPUT_EVENT_INJECTION_PERMISSION_DENIED:
            Slogger::W(TAG, "Input event injection from pid %d permission denied.", pid);
            Slogger::E(TAG, "Injecting to another application requires INJECT_EVENTS permission");

            return E_SECURITY_EXCEPTION;

        case INPUT_EVENT_INJECTION_SUCCEEDED:
            *result = TRUE;
            break;

        case INPUT_EVENT_INJECTION_TIMED_OUT:
            Slogger::W(TAG, "Input event injection from pid %d timed out.", pid);
            *result = FALSE;
            break;

        case INPUT_EVENT_INJECTION_FAILED:
        default:
            Slogger::W(TAG, "Input event injection from pid %d failed", pid);
            *result = FALSE;
            break;
    }

    return NOERROR;
}

void CInputManagerService::OnInputDevicesChangedListenerDied(
    /* [in] */ Int32 pid)
{
    synchronized (mInputDevicesLock) {
        mInputDevicesChangedListeners->Remove(pid);
    }
}

// Must be called on handler.
void CInputManagerService::DeliverInputDevicesChanged(
    /* [in] */ ArrayOf<IInputDevice*>* oldInputDevices)
{
    // Scan for changes.
    Int32 numFullKeyboardsAdded = 0;
    mTempInputDevicesChangedListenersToNotify.Clear();
    mTempFullKeyboards.Clear();
    AutoPtr<ArrayOf<Int32> > deviceIdAndGeneration;
    synchronized (mInputDevicesLock) {
        if (!mInputDevicesChangedPending) {
            return;
        }
        mInputDevicesChangedPending = FALSE;

        Int32 size;
        mInputDevicesChangedListeners->GetSize(&size);
        for (Int32 i = 0;  i < size;  i++) {
            AutoPtr<IInterface> outface;
            mInputDevicesChangedListeners->Get(i, (IInterface**)&outface);
            mTempInputDevicesChangedListenersToNotify->Add((IInterface*)outface);
        }

        Int32 numDevices = mInputDevices->GetLength();
        deviceIdAndGeneration = ArrayOf<Int32>::Alloc(numDevices * 2);
        for (Int32 i = 0; i < numDevices; i++) {
            AutoPtr<IInputDevice> inputDevice = (*mInputDevices)[i];
            Int32 temp;
            inputDevice->GetId(&temp);
            (*deviceIdAndGeneration)[i * 2] = temp;
            inputDevice->GetGeneration(&temp);
            (*deviceIdAndGeneration)[i * 2 + 1] = temp;

            Boolean isVirtual, isFullKeyboard;
            if (!(inputDevice->IsVirtual(&isVirtual), isVirtual)
                && (inputDevice->IsFullKeyboard(&isFullKeyboard), isFullKeyboard)) {
                String descriptor;
                inputDevice->GetDescriptor(&descriptor);
                if (!ContainsInputDeviceWithDescriptor(oldInputDevices, descriptor)) {
                    mTempFullKeyboards->Add(numFullKeyboardsAdded, inputDevice);
                    numFullKeyboardsAdded++;
                }
                else {
                    mTempFullKeyboards->Add(inputDevice);
                }
            }
        }
    }

    // Notify listeners.
    Int32 numListeners;
    mInputDevicesChangedListeners->GetSize(&numListeners);
    for (int i = 0; i < numListeners; i++) {

        AutoPtr<InputDevicesChangedListenerRecord> inputDeviceRec;

        mTempInputDevicesChangedListenersToNotify->Get(i, (IInterface**)&inputDeviceRec);
            inputDeviceRec->NotifyInputDevicesChanged(deviceIdAndGeneration);
    }
    mTempInputDevicesChangedListenersToNotify.Clear();

    // Check for missing keyboard layouts.
    if (mNotificationManager != NULL) {
        Int32 numFullKeyboards;
        mTempFullKeyboards->GetSize(&numFullKeyboards);

        Boolean missingLayoutForExternalKeyboard = FALSE;
        Boolean missingLayoutForExternalKeyboardAdded = FALSE;
        Boolean multipleMissingLayoutsForExternalKeyboardsAdded = FALSE;
        AutoPtr<IInputDevice> keyboardMissingLayout;
        synchronized (mDataStore) {

            for (int i = 0; i < numFullKeyboards; i++) {
                AutoPtr<IInputDevice> inputDevice;
                String layout;
                AutoPtr<IInputDeviceIdentifier> identifier;

                mTempFullKeyboards->Get(i, (IInterface**)&inputDevice);

                inputDevice->GetIdentifier((IInputDeviceIdentifier**)&identifier);
                GetCurrentKeyboardLayoutForInputDevice(identifier, &layout);

                if (layout == NULL) {
                    missingLayoutForExternalKeyboard = TRUE;
                    if (i < numFullKeyboardsAdded) {
                        missingLayoutForExternalKeyboardAdded = TRUE;
                    }
                    if (keyboardMissingLayout == NULL) {
                        keyboardMissingLayout = inputDevice;
                    } else {
                        multipleMissingLayoutsForExternalKeyboardsAdded = TRUE;
                    }
                }
            }
        }

        if (missingLayoutForExternalKeyboard) {
            if (missingLayoutForExternalKeyboardAdded) {
                if (multipleMissingLayoutsForExternalKeyboardsAdded) {
                    // We have more than one keyboard missing a layout, so drop the
                    // user at the generic input methods page so they can pick which
                    // one to set.
                    ShowMissingKeyboardLayoutNotification(NULL);
                } else {
                    ShowMissingKeyboardLayoutNotification(keyboardMissingLayout);
                }
            }
        }
        else if (mKeyboardLayoutNotificationShown) {
            HideMissingKeyboardLayoutNotification();
        }
    }
    mTempFullKeyboards.Clear();
}

// Must be called on handler.
void CInputManagerService::ShowMissingKeyboardLayoutNotification(
    /* [in] */ IInputDevice* device)
{
    if (!mKeyboardLayoutNotificationShown) {
        if (mKeyboardLayoutIntent == NULL) {
            AutoPtr<IIntent> intent;
            CIntent::New(String("android.settings.INPUT_METHOD_SETTINGS"), (IIntent**)&intent);
            if (device != NULL) {
                AutoPtr<IInputDeviceIdentifier> identifier;
                device->GetIdentifier((IInputDeviceIdentifier**)&identifier);
                intent->PutExtra(String("input_device_identifier"), ISerializable::Probe(identifier));
            }
            intent->SetFlags(IIntent::FLAG_ACTIVITY_NEW_TASK
                    | IIntent::FLAG_ACTIVITY_RESET_TASK_IF_NEEDED
                    | IIntent::FLAG_ACTIVITY_CLEAR_TOP);

            AutoPtr<IPendingIntentHelper> piHelper;
            AutoPtr<IPendingIntent> keyboardLayoutIntent;
//TODO
//            CPendingIntentHelper::AcquireSingleton((IPendingIntentHelper**)&piHelper);
            piHelper->GetActivityAsUser(
                mContext, 0, intent, 0, NULL, (IUserHandle*)UserHandle::USER_CURRENT,
                (IPendingIntent**)&keyboardLayoutIntent);
        }

        AutoPtr<IResources> r;
        mContext->GetResources((IResources**)&r);
        AutoPtr<INotificationBuilder> builder;
        CNotificationBuilder::New(mContext, (INotificationBuilder**)&builder);
        String str;
        r->GetString(R::string::select_keyboard_layout_notification_title, &str);
        AutoPtr<ICharSequence> cs;
        CString::New(str, (ICharSequence**)&cs);
        builder->SetContentTitle(cs);
        r->GetString(R::string::select_keyboard_layout_notification_message, &str);
        cs = NULL;
        CString::New(str, (ICharSequence**)&cs);
        builder->SetContentText(cs);
        builder->SetContentIntent(mKeyboardLayoutIntent);
        builder->SetSmallIcon(R::drawable::ic_settings_language);
        builder->SetPriority(INotification::PRIORITY_LOW);

        Int32 color;
        r->GetColor(R::color::system_notification_accent_color, &color);
        builder->SetColor(color);

        AutoPtr<INotification> notification;
        builder->Build((INotification**)&notification);
        mNotificationManager->NotifyAsUser(String(NULL),
            R::string::select_keyboard_layout_notification_title,
            notification, UserHandle::ALL);

        mKeyboardLayoutNotificationShown = TRUE;
    }
}

// Must be called on handler.
void CInputManagerService::HideMissingKeyboardLayoutNotification()
{
    if (mKeyboardLayoutNotificationShown) {
        mKeyboardLayoutNotificationShown = FALSE;
        mNotificationManager->CancelAsUser(String(NULL),
            R::string::select_keyboard_layout_notification_title,
            UserHandle::ALL);
    }
}

// Must be called on handler.
void CInputManagerService::UpdateKeyboardLayouts()
{
    // Scan all input devices state for keyboard layouts that have been uninstalled.
    HashSet<String> availableKeyboardLayouts;
    AutoPtr<UpdateKeyboardLayouts_KeyboardLayoutVisitor> myKV = new UpdateKeyboardLayouts_KeyboardLayoutVisitor(availableKeyboardLayouts);
    VisitAllKeyboardLayouts(myKV);

    synchronized (mDataStore) {
        Int32 capacity = availableKeyboardLayouts.GetSize();
        AutoPtr<IHashSet> hashSet;
        CHashSet::New(capacity, (IHashSet**)&hashSet);

        HashSet<String>::Iterator it = availableKeyboardLayouts.Begin();
        for (; it != availableKeyboardLayouts.End(); ++it) {
            AutoPtr<ICharSequence> cs;
            String str = *it;
            CString::New(str, (ICharSequence**)&cs);
            hashSet->Add(cs);
        }

        mDataStore->RemoveUninstalledKeyboardLayouts(ISet::Probe(hashSet));
        mDataStore->SaveIfNeeded();
    }

    // Reload keyboard layouts.
    ReloadKeyboardLayouts();
}

Boolean CInputManagerService::ContainsInputDeviceWithDescriptor(
    /* [in] */ ArrayOf<IInputDevice*>* inputDevices,
    /* [in] */ const String& descriptor)
{
    Int32 numDevices = inputDevices->GetLength();
    for (Int32 i = 0; i < numDevices; i++) {
        String descriptor2;
        (*inputDevices)[i]->GetDescriptor(&descriptor2);
        if (descriptor2.Equals(descriptor)) {
            return TRUE;
        }
    }
    return FALSE;
}


void CInputManagerService::VisitAllKeyboardLayouts(
    /* [in] */ IKeyboardLayoutVisitor* visitor)
{
    AutoPtr<IPackageManager> pm;
    mContext->GetPackageManager((IPackageManager**)&pm);
    AutoPtr<IIntent> intent;
    CIntent::New(IInputManager::ACTION_QUERY_KEYBOARD_LAYOUTS, (IIntent**)&intent);

    AutoPtr<IList> listcontainer;
    pm->QueryBroadcastReceivers(
        intent, IPackageManager::GET_META_DATA, (IList**)&listcontainer);
    AutoPtr<IList> enumerator;
    AutoPtr<IListIterator> ilt;

    listcontainer->GetListIterator((IListIterator**)&ilt);
    Boolean hasNext = FALSE;
    AutoPtr<IIterator> it = IIterator::Probe(ilt);
    while (it->HasNext(&hasNext), hasNext) {
        AutoPtr<IResolveInfo> resolveInfo;
        it->GetNext((IInterface**)&resolveInfo);

        AutoPtr<IActivityInfo> activityInfo;
        resolveInfo->GetActivityInfo((IActivityInfo**)&activityInfo);
        Int32 priority;
        resolveInfo->GetPriority(&priority);
        VisitKeyboardLayoutsInPackage(pm, activityInfo, String(NULL), priority, visitor);
    }
}

void CInputManagerService::VisitKeyboardLayout(
    /* [in] */ const String& keyboardLayoutDescriptor,
    /* [in] */ IKeyboardLayoutVisitor* visitor)
{
    AutoPtr<KeyboardLayoutDescriptor> d =
        KeyboardLayoutDescriptor::Parse(keyboardLayoutDescriptor);
    if (d != NULL) {
        AutoPtr<IPackageManager> pm;
        mContext->GetPackageManager((IPackageManager**)&pm);
        AutoPtr<IComponentName> componentName;
        CComponentName::New(
            d->mPackageName, d->mReceiverName, (IComponentName**)&componentName);
        AutoPtr<IActivityInfo> receiver;
        pm->GetReceiverInfo(
            componentName, IPackageManager::GET_META_DATA, (IActivityInfo**)&receiver);
        VisitKeyboardLayoutsInPackage(pm, receiver, d->mKeyboardLayoutName, 0, visitor);
    }
}

void CInputManagerService::VisitKeyboardLayoutsInPackage(
    /* [in] */ IPackageManager* pm,
    /* [in] */ IActivityInfo* receiver,
    /* [in] */ const String& keyboardName,
    /* [in] */ Int32 requestedPriority,
    /* [in] */ IKeyboardLayoutVisitor* visitor)
{
    AutoPtr<IBundle> metaData;
    IPackageItemInfo::Probe(receiver)->GetMetaData((IBundle**)&metaData);
    if (metaData == NULL) {
        return;
    }

    String packageName, receiverName;
    IPackageItemInfo::Probe(receiver)->GetPackageName(&packageName);
    IPackageItemInfo::Probe(receiver)->GetName(&receiverName);

    Int32 configResId;
    metaData->GetInt32(IInputManager::META_DATA_KEYBOARD_LAYOUTS, &configResId);
    if (configResId == 0) {
        Logger::W(TAG, "Missing meta-data '%s' on receiver %s/%s",
            IInputManager::META_DATA_KEYBOARD_LAYOUTS.string(),
            packageName.string(), receiverName.string());
        return;
    }

    AutoPtr<ICharSequence> receiverLabel;
    IPackageItemInfo::Probe(receiver)->LoadLabel(pm, (ICharSequence**)&receiverLabel);
    String collection("");
    if (receiverLabel != NULL) {
        receiverLabel->ToString(&collection);
    }
    Int32 priority;
    AutoPtr<IApplicationInfo> info;
    Int32 flags;
    IComponentInfo::Probe(receiver)->GetApplicationInfo((IApplicationInfo**)&info);
    info->GetFlags(&flags);

    if ((flags & IApplicationInfo::FLAG_SYSTEM) != 0) {
        priority = requestedPriority;
    } else {
        priority = 0;
    }

    ECode ec;
    AutoPtr<IApplicationInfo> appInfo;
    AutoPtr<IResources> resources;
    AutoPtr<IXmlResourceParser> parser;
    AutoPtr<IXmlPullParser> pullParser;

    ec = IComponentInfo::Probe(receiver)->GetApplicationInfo((IApplicationInfo**)&appInfo);
    if (FAILED(ec))
        goto _Exit3_;

    ec = pm->GetResourcesForApplication(appInfo, (IResources**)&resources);
    if (FAILED(ec))
        goto _Exit3_;

    ec = resources->GetXml(configResId, (IXmlResourceParser**)&parser);
    if (FAILED(ec))
        goto _Exit3_;

    pullParser = IXmlPullParser::Probe(parser);
    ec = XmlUtils::BeginDocument(pullParser, String("keyboard-layouts"));
    if (FAILED(ec))
        goto _Exit2_;

    for (;;) {
        ec = XmlUtils::NextElement(pullParser);
        if (FAILED(ec))
            goto _Exit2_;

        String element;
        ec = pullParser->GetName(&element);
        if (FAILED(ec))
            goto _Exit2_;

        if (element.IsNull()) {
            break;
        }
        if (element.Equals("keyboard-layout")) {
            String name;
            String label;
            Int32 keyboardLayoutResId;

            Int32 size = sizeof(R::styleable::KeyboardLayout) / sizeof(R::styleable::KeyboardLayout[0]);

            AutoPtr<ArrayOf<Int32> > layout = ArrayOf<Int32>::Alloc(size);
            layout->Copy(R::styleable::KeyboardLayout, size);

            AutoPtr<ITypedArray> a;
            ec = resources->ObtainAttributes(
                IAttributeSet::Probe(parser), layout, (ITypedArray**)&a);

            if (FAILED(ec))
                goto _Exit2_;

            ec = a->GetString(R::styleable::KeyboardLayout_name, &name);
            if (FAILED(ec))
                goto _Exit1_;

            ec = a->GetString(R::styleable::KeyboardLayout_label, &label);
            if (FAILED(ec))
                goto _Exit1_;

            ec = a->GetResourceId(
                R::styleable::KeyboardLayout_keyboardLayout, 0,
                &keyboardLayoutResId);
            if (FAILED(ec))
                goto _Exit1_;

            if (name.IsNull() || label.IsNull() || keyboardLayoutResId == 0) {
                Logger::W(
                    TAG, "Missing required 'name', 'label' or 'keyboardLayout' "
                    "attributes in keyboard layout resource from receiver %s/%s",
                    packageName.string(), receiverName.string());
            }
            else {
                String descriptor = KeyboardLayoutDescriptor::Format(
                        packageName, receiverName, name);
                if (keyboardName.IsNull() || name.Equals(keyboardName)) {
                    visitor->VisitKeyboardLayout(
                        resources, descriptor, label, collection, keyboardLayoutResId, priority);
                }
            }
_Exit1_:
            a->Recycle();
        }
        else {
            Logger::W(TAG, "Skipping unrecognized element '%s"
                "' in keyboard layout resource from receiver %s/%s",
                element.string(), packageName.string(), receiverName.string());
        }
    }
_Exit2_:
    parser->Close();

_Exit3_:
    if (FAILED(ec)) {
        Logger::W(TAG, "Could not parse keyboard layout resource from receiver %s, %s",
            packageName.string(), receiverName.string());
    }
}

/**
 * Builds a layout descriptor for the vendor/product. This returns the
 * descriptor for ids that aren't useful (such as the default 0, 0).
 */
ECode CInputManagerService::GetLayoutDescriptor(
    /* [in] */ IInputDeviceIdentifier* identifier,
    /* [out] */ String* layoutDescriptor)
{
    String str;
    identifier->GetDescriptor(&str);
    if (identifier == NULL || str == NULL) {
        //throw new IllegalArgumentException("identifier and descriptor must not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Int32 idVendor, idProduct;
    identifier->GetVendorId(&idVendor);
    identifier->GetProductId(&idProduct);

    if (idVendor == 0 && idProduct == 0) {
        identifier->GetDescriptor(layoutDescriptor);
        return NOERROR;
    }

    *layoutDescriptor = String("vendor:") + StringUtils::ToString(idVendor) + String(",product:") + StringUtils::ToString(idProduct);

    return NOERROR;
}

// Must be called on handler.
void CInputManagerService::HandleSwitchKeyboardLayout(
    /* [in] */ Int32 deviceId,
    /* [in] */ Int32 direction)
{
    AutoPtr<IInputDevice> device;
    GetInputDevice(deviceId, (IInputDevice**)&device);
    if (device != NULL) {
        AutoPtr<IInputDeviceIdentifier> identifier;
        device->GetIdentifier((IInputDeviceIdentifier**)&identifier);
        Boolean changed;
        String key;
        GetLayoutDescriptor((IInputDeviceIdentifier*)identifier, &key);

        String keyboardLayoutDescriptor;
        synchronized (mDataStore) {
            changed = mDataStore->SwitchKeyboardLayout(key, direction);
            keyboardLayoutDescriptor = mDataStore->GetCurrentKeyboardLayout(key);
            mDataStore->SaveIfNeeded();
        }

        if (changed) {
            if (mSwitchedKeyboardLayoutToast != NULL) {
                mSwitchedKeyboardLayoutToast->Cancel();
                mSwitchedKeyboardLayoutToast = NULL;
            }
            if (keyboardLayoutDescriptor != NULL) {
                AutoPtr<IKeyboardLayout> keyboardLayout;
                GetKeyboardLayout(keyboardLayoutDescriptor, (IKeyboardLayout**)&keyboardLayout);
                if (keyboardLayout != NULL) {
                    AutoPtr<IToastHelper> toastHelper;
                    CToastHelper::AcquireSingleton((IToastHelper**)&toastHelper);
                    String label;
                    keyboardLayout->GetLabel(&label);
                    AutoPtr<ICharSequence> labelcs;
                    CString::New(label, (ICharSequence**)&labelcs);
                    toastHelper->MakeText(
                        mContext, labelcs, IToast::LENGTH_SHORT,
                        (IToast**)&mSwitchedKeyboardLayoutToast);
                    mSwitchedKeyboardLayoutToast->Show();
                }
            }

            ReloadKeyboardLayouts();
        }
    }
}

void CInputManagerService::SetPointerSpeedUnchecked(
    /* [in] */ Int32 speed)
{
    speed = Elastos::Core::Math::Min(Elastos::Core::Math::Max(speed, IInputManager::MIN_POINTER_SPEED),
            IInputManager::MAX_POINTER_SPEED);
    NativeSetPointerSpeed(speed);
}

void CInputManagerService::RegisterPointerSpeedSettingObserver()
{
//TODO
/*
    AutoPtr<RegisterPointerSpeedSettingObserver_ContentObserver> settingsObserver =
                new RegisterPointerSpeedSettingObserver_ContentObserver(this, mHandler);

    AutoPtr<IContentResolver> resolver;
    mContext->GetContentResolver((IContentResolver**)&resolver);
    AutoPtr<ISettingsSystem> ss;
    CSettingsSystem::AcquireSingleton((ISettingsSystem**)&ss);
    AutoPtr<IUri> uri;
    ss->GetUriForEx(ISettingsSystem::POINTER_SPEED, (IUri**)&uri);
    resolver->RegisterContentObserverEx(uri, TRUE, settingsObserver, IUserHandle::USER_ALL);
*/
}

Int32 CInputManagerService::GetPointerSpeedSetting()
{

    Int32 speed = IInputManager::DEFAULT_POINTER_SPEED;
//TODO
/*
    AutoPtr<ISettingsSystem> ss;
    CSettingsSystem::AcquireSingleton((ISettingsSystem**)&ss);
    AutoPtr<IContentResolver> resolver;
    mContext->GetContentResolver((IContentResolver**)&resolver);
    ss->GetInt32ForUserEx(
        resolver, ISettingsSystem::POINTER_SPEED,
        IUserHandle::USER_CURRENT, &speed);
*/
    return speed;
}

void CInputManagerService::RegisterShowTouchesSettingObserver()
{
    AutoPtr<IContentObserver> settingsObserver =
        new RegisterShowTouchesSettingObserver_ContentObserver(this, mHandler);
//TODO
/*
    AutoPtr<IContentResolver> resolver;
    mContext->GetContentResolver((IContentResolver**)&resolver);
    AutoPtr<ISettingsSystem> ss;
    CSettingsSystem::AcquireSingleton((ISettingsSystem**)&ss);
    AutoPtr<IUri> uri;
    ss->GetUriForEx(ISettingsSystem::SHOW_TOUCHES, (IUri**)&uri);
    resolver->RegisterContentObserverEx(uri, TRUE, settingsObserver, IUserHandle::USER_ALL);
*/
}

Int32 CInputManagerService::GetShowTouchesSetting(
    /* [in] */ Int32 defaultValue)
{
    Int32 result = defaultValue;
//TODO
/*
    AutoPtr<ISettingsSystem> ss;
    CSettingsSystem::AcquireSingleton((ISettingsSystem**)&ss);
    AutoPtr<IContentResolver> resolver;
    mContext->GetContentResolver((IContentResolver**)&resolver);
    ss->GetInt32ForUserEx(
        resolver, ISettingsSystem::SHOW_TOUCHES,
        IUserHandle::USER_CURRENT, &result);
*/
    return result;
}

void CInputManagerService::OnVibratorTokenDied(
    /* [in] */ VibratorToken* v)
{
    synchronized (mVibratorLock) {
        mVibratorTokens.Erase(v->mToken);
    }

    CancelVibrateIfNeeded(v);
}

void CInputManagerService::CancelVibrateIfNeeded(
    /* [in] */ VibratorToken* v)
{
    synchronized (v) {
        if (v->mVibrating) {
            NativeCancelVibrate(v->mDeviceId, v->mTokenValue);
            v->mVibrating = FALSE;
        }
    }
}

Boolean CInputManagerService::CheckCallingPermission(
    /* [in] */ const String& permission,
    /* [in] */ const String& func)
{
    // Quick check: if the calling permission is me, it's all okay.
    Int32 pid = Process::MyPid();
    if (Binder::GetCallingPid() == pid) {
        return TRUE;
    }

    Int32 value;
    FAIL_RETURN(mContext->CheckCallingPermission(permission, &value));
    if (value == IPackageManager::PERMISSION_GRANTED) {
        return TRUE;
    }

    String msg = String("Permission Denial: ") + func + " from pid="
            + StringUtils::ToString(Binder::GetCallingPid())
            + ", uid=" + StringUtils::ToString(Binder::GetCallingUid())
            + " requires " + permission;
    Slogger::W(TAG, msg);

    return FALSE;
}

// Native callback.
void CInputManagerService::NotifyConfigurationChanged(
    /* [in] */ Int64 whenNanos)
{
    mWindowManagerCallbacks->NotifyConfigurationChanged();
}

// Native callback.
void CInputManagerService::NotifyInputDevicesChanged(
    /* [in] */ ArrayOf<IInputDevice*>* inputDevices)
{
    synchronized (mInputDevicesLock) {

        if (!mInputDevicesChangedPending) {
            mInputDevicesChangedPending = TRUE;
            AutoPtr<IArrayOf> container;
            Int32 length = inputDevices->GetLength();
            CArrayOf::New(EIID_IInputDevice, length, (IArrayOf**)&container);
            for (Int32 i = 0; i < length; ++i) {
                container->Set(i, (*inputDevices)[i]);
            }

            AutoPtr<IMessage> msg;
            mHandler->ObtainMessage(MSG_DELIVER_INPUT_DEVICES_CHANGED, (IMessage**)&msg);
            msg->SetObj(container);
            Boolean result;
            mHandler->SendMessage(msg, &result);
        }

        mInputDevices = inputDevices;
    }
}

// Native callback.
void CInputManagerService::NotifySwitch(
    /* [in] */ Int64 whenNanos,
    /* [in] */ Int32 switchValues,
    /* [in] */ Int32 switchMask)
{
    if (DEBUG) {
        Slogger::D(TAG, "notifySwitch: values=%x, mask=%x",
            switchValues, switchMask);
    }

    if ((switchMask & SW_LID_BIT) != 0) {
        Boolean lidOpen = ((switchValues & SW_LID_BIT) == 0);
        mWindowManagerCallbacks->NotifyLidSwitchChanged(whenNanos, lidOpen);
    }

    if ((switchMask & SW_CAMERA_LENS_COVER_BIT) != 0) {
        Boolean lensCovered = ((switchValues & SW_CAMERA_LENS_COVER_BIT) != 0);
        mWindowManagerCallbacks->NotifyCameraLensCoverSwitchChanged(whenNanos, lensCovered);
    }

    if (mUseDevInputEventForAudioJack && (switchMask & SW_JACK_BITS) != 0) {
        mWiredAccessoryCallbacks->NotifyWiredAccessoryChanged(
            whenNanos, switchValues, switchMask);
    }
}

// Native callback.
void CInputManagerService::NotifyInputChannelBroken(
    /* [in] */ InputWindowHandle* inputWindowHandle)
{
    mWindowManagerCallbacks->NotifyInputChannelBroken(inputWindowHandle);
}

// Native callback.
Int64 CInputManagerService::NotifyANR(
    /* [in] */ InputApplicationHandle* inputApplicationHandle,
    /* [in] */ InputWindowHandle* inputWindowHandle,
    /* [in] */ const String& reason)
{
    Int64 ret;

    mWindowManagerCallbacks->NotifyANR(inputApplicationHandle, inputWindowHandle, reason, &ret);

    return ret;
}

// Native callback.
Boolean CInputManagerService::FilterInputEvent(
    /* [in] */ IInputEvent* event,
    /* [in] */ Int32 policyFlags)
{
    synchronized (mInputFilterLock) {
        if (mInputFilter != NULL) {
            mInputFilter->FilterInputEvent(event, policyFlags);
            return FALSE;
        }
    }
    event->Recycle();
    return TRUE;
}

// Native callback.
Int32 CInputManagerService::InterceptKeyBeforeQueueing(
    /* [in] */ IKeyEvent* event,
    /* [in] */ Int32 policyFlags)
{
    Int32 ret;

    mWindowManagerCallbacks->InterceptKeyBeforeQueueing(event, policyFlags, &ret);

    return ret;
}

// Native callback.
Int32 CInputManagerService::InterceptMotionBeforeQueueingNonInteractive(
    /* [in] */ Int64 whenNanos,
    /* [in] */ Int32 policyFlags)
{
    //no implementation in InputManagerService.java
    return 0;
}

// Native callback.
Int64 CInputManagerService::InterceptKeyBeforeDispatching(
    /* [in] */ InputWindowHandle* focus,
    /* [in] */ IKeyEvent* event,
    /* [in] */ Int32 policyFlags)
{
    Int64 ret;

    mWindowManagerCallbacks->InterceptKeyBeforeDispatching(focus, event, policyFlags, &ret);

    return ret;
}

// Native callback.
AutoPtr<IKeyEvent> CInputManagerService::DispatchUnhandledKey(
    /* [in] */ InputWindowHandle* focus,
    /* [in] */ IKeyEvent* event,
    /* [in] */ Int32 policyFlags)
{
    AutoPtr<IKeyEvent> keyEvent;

    mWindowManagerCallbacks->DispatchUnhandledKey(focus, event, policyFlags, (IKeyEvent**)&keyEvent);

    return keyEvent;
}

// Native callback.
Boolean CInputManagerService::CheckInjectEventsPermission(
    /* [in] */ Int32 injectorPid,
    /* [in] */ Int32 injectorUid)
{
    Int32 value;
    FAIL_RETURN(mContext->CheckPermission(
        Elastos::Droid::Manifest::permission::INJECT_EVENTS,
        injectorPid, injectorUid, &value));
    return value == IPackageManager::PERMISSION_GRANTED;
}

// Native callback.
Int32 CInputManagerService::GetVirtualKeyQuietTimeMillis()
{
    AutoPtr<IResources> resources;
    mContext->GetResources((IResources**)&resources);
    Int32 value = 0;
    resources->GetInteger(R::integer::config_virtualKeyQuietTimeMillis, &value);
    return value;
}

// Native callback.
AutoPtr< ArrayOf<String> > CInputManagerService::GetExcludedDeviceNames()
{
    //TODO::
    //
    return ArrayOf<String>::Alloc(0);

    List<String> names;

    // Read partner-provided list of excluded input devices
    AutoPtr<IXmlPullParser> parser;;

    // Environment.getRootDirectory() is a fancy way of saying ANDROID_ROOT or "/system".
    AutoPtr<IEnvironment> env;
    CEnvironment::AcquireSingleton((IEnvironment**)&env);
    AutoPtr<IFile> root;
    env->GetRootDirectory((IFile**)&root);
    AutoPtr<IFile> confFile;
    CFile::New(root, EXCLUDED_DEVICES_PATH, (IFile**)&confFile);

    AutoPtr<IFileReader> confreader;

    ECode ec = CFileReader::New(confFile, (IFileReader**)&confreader);
    if (FAILED(ec))
        goto _Exit_;

    ec = Xml::NewPullParser((IXmlPullParser**)&parser);
    if (FAILED(ec))
        goto _Exit_;

    ec = parser->SetInput(IReader::Probe(confreader));
    if (FAILED(ec))
        goto _Exit_;

    ec = XmlUtils::BeginDocument(parser, String("devices"));
    if (FAILED(ec))
        goto _Exit_;

    while (TRUE) {
        ec = XmlUtils::NextElement(parser);
        if (FAILED(ec))
            goto _Exit_;

        String name;
        ec = parser->GetName(&name);
        if (FAILED(ec))
            goto _Exit_;

        if (!name.Equals("device")) {
            break;
        }

        ec = parser->GetAttributeValue(String(NULL), String("name"), &name);
        if (FAILED(ec))
            goto _Exit_;

        if (name != NULL) {
            names.PushBack(name);
        }
    }
_Exit_:
    if (ec == (ECode)E_FILE_NOT_FOUND_EXCEPTION) {
        // It's ok if the file does not exist.
    }
    else if (FAILED(ec)) {
        String path;
        confFile->GetAbsolutePath(&path);
        Slogger::E(TAG, "Exception while parsing '%s'", path.string());
    }

    if (confreader != NULL)
        ICloseable::Probe(confreader)->Close();

    AutoPtr<ArrayOf<String> > array = ArrayOf<String>::Alloc(names.GetSize());
    List<String>::Iterator iter = names.Begin();
    for (Int32 i = 0; iter != names.End(); ++iter)
        (*array)[i] = *iter;

    return array;
}

// Native callback.
Int32 CInputManagerService::GetKeyRepeatTimeout()
{
    AutoPtr<IViewConfigurationHelper> vh;
    CViewConfigurationHelper::AcquireSingleton((IViewConfigurationHelper**)&vh);
    Int32 value;
    vh->GetKeyRepeatTimeout(&value);
    return value;
}

// Native callback.
Int32 CInputManagerService::GetKeyRepeatDelay()
{
    AutoPtr<IViewConfigurationHelper> vh;
    CViewConfigurationHelper::AcquireSingleton((IViewConfigurationHelper**)&vh);
    Int32 value;
    vh->GetKeyRepeatDelay(&value);
    return value;
}

// Native callback.
Int32 CInputManagerService::GetHoverTapTimeout()
{
    AutoPtr<IViewConfigurationHelper> vh;
    CViewConfigurationHelper::AcquireSingleton((IViewConfigurationHelper**)&vh);
    Int32 value;
    vh->GetHoverTapTimeout(&value);
    return value;
}

// Native callback.
Int32 CInputManagerService::GetHoverTapSlop()
{
    AutoPtr<IViewConfigurationHelper> vh;
    CViewConfigurationHelper::AcquireSingleton((IViewConfigurationHelper**)&vh);
    Int32 value;
    vh->GetHoverTapSlop(&value);
    return value;
}

// Native callback.
Int32 CInputManagerService::GetDoubleTapTimeout()
{
    AutoPtr<IViewConfigurationHelper> vh;
    CViewConfigurationHelper::AcquireSingleton((IViewConfigurationHelper**)&vh);
    Int32 value;
    vh->GetDoubleTapTimeout(&value);
    return value;
}

// Native callback.
Int32 CInputManagerService::GetLongPressTimeout()
{
    AutoPtr<IViewConfigurationHelper> vh;
    CViewConfigurationHelper::AcquireSingleton((IViewConfigurationHelper**)&vh);
    Int32 value;
    vh->GetLongPressTimeout(&value);
    return value;
}

// Native callback.
Int32 CInputManagerService::GetPointerLayer()
{
    Int32 ret;

    mWindowManagerCallbacks->GetPointerLayer(&ret);

    return ret;
}

// Native callback.
AutoPtr<IPointerIcon> CInputManagerService::GetPointerIcon()
{
    AutoPtr<IPointerIconHelper> ph;
    CPointerIconHelper::AcquireSingleton((IPointerIconHelper**)&ph);
    AutoPtr<IPointerIcon> icon;
    ph->GetDefaultIcon(mContext, (IPointerIcon**)&icon);

    return icon;
}

// Native callback.
AutoPtr< ArrayOf<String> > CInputManagerService::GetKeyboardLayoutOverlay(
    /* [in] */ IInputDeviceIdentifier* identifier)
{
    if (!mSystemReady) {
        return NULL;
    }

    String keyboardLayoutDescriptor;
    GetCurrentKeyboardLayoutForInputDevice(identifier, &keyboardLayoutDescriptor);
    if (keyboardLayoutDescriptor.IsNull()) {
        return NULL;
    }

    AutoPtr<ArrayOf<String> > result = ArrayOf<String>::Alloc(2);
    AutoPtr<GetKeyboardLayoutOverlay_KeyboardLayoutVisitor> myKVEx3 = new GetKeyboardLayoutOverlay_KeyboardLayoutVisitor(result);
    VisitKeyboardLayout(keyboardLayoutDescriptor, myKVEx3);
    if ((*result)[0].IsNull()) {
        Logger::W(TAG, "Could not get keyboard layout with descriptor '%s'.",
            keyboardLayoutDescriptor.string());
        return NULL;
    }

    return result;
}

// Native callback.
String CInputManagerService::GetDeviceAlias(
    /* [in] */ const String& uniqueId)
{
    AutoPtr<IBluetoothAdapterHelper> bh;
    CBluetoothAdapterHelper::AcquireSingleton((IBluetoothAdapterHelper**)&bh);
    Boolean res;
    bh->CheckBluetoothAddress(uniqueId, &res);
    if (res) {
        // TODO(BT) mBluetoothService.getRemoteAlias(uniqueId)
        return String(NULL);
    }
    return String(NULL);
}

} // Input
} // Server
} // Droid
} // Elastos
