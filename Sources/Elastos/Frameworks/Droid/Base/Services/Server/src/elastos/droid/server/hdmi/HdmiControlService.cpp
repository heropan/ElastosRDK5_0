
#include "elastos/droid/server/hdmi/HdmiControlService.h"
#include <elastos/utility/Arrays.h>
#include "elastos/droid/server/hdmi/Constants.h"
#include "elastos/droid/server/hdmi/UnmodifiableSparseInt32Array.h"
#include "elastos/droid/server/hdmi/UnmodifiableSparseArray.h"
#include "elastos/droid/server/hdmi/HdmiMhlControllerStub.h"
#include "elastos/droid/server/hdmi/HdmiCecController.h"

// import static android.hardware.hdmi.IHdmiControlManager::DEVICE_EVENT_ADD_DEVICE;
// import static android.hardware.hdmi.IHdmiControlManager::DEVICE_EVENT_REMOVE_DEVICE;
// import static com.android.server.hdmi.Constants::DISABLED;
// import static com.android.server.hdmi.Constants::ENABLED;
// import static com.android.server.hdmi.Constants::OPTION_CEC_AUTO_WAKEUP;
// import static com.android.server.hdmi.Constants::OPTION_CEC_ENABLE;
// import static com.android.server.hdmi.Constants::OPTION_CEC_SERVICE_CONTROL;
// import static com.android.server.hdmi.Constants::OPTION_MHL_ENABLE;
// import static com.android.server.hdmi.Constants::OPTION_MHL_INPUT_SWITCHING;
// import static com.android.server.hdmi.Constants::OPTION_MHL_POWER_CHARGE;
// import libcore.util.EmptyArray;

using Elastos::Droid::Content::IBroadcastReceiver;
using Elastos::Droid::Content::IContentResolver;
using Elastos::Droid::Content::IIntentFilter;
using Elastos::Droid::Database::IContentObserver;
using Elastos::Droid::Hardware::Hdmi::IHdmiControlManager;
using Elastos::Droid::Hardware::Hdmi::IHdmiPortInfo;
using Elastos::Droid::Hardware::Hdmi::IIHdmiControlCallback;
using Elastos::Droid::Hardware::Hdmi::IIHdmiInputChangeListener;
using Elastos::Droid::Hardware::Hdmi::IIHdmiMhlVendorCommandListener;
using Elastos::Droid::Hardware::Hdmi::IIHdmiRecordListener;
using Elastos::Droid::Hardware::Hdmi::IIHdmiVendorCommandListener;
// using Elastos::Droid::Os::Build;
using Elastos::Droid::Os::IBinder;
using Elastos::Droid::Os::IPowerManager;
// using Elastos::Droid::Os::ISystemClock;
using Elastos::Droid::Os::ISystemProperties;
using Elastos::Droid::Os::IUserHandle;
// using Elastos::Droid::Provider::ISettings::Global;
using Elastos::Droid::Text::ITextUtils;
using Elastos::Droid::Utility::IArraySet;
using Elastos::Droid::Utility::ISlog;
using Elastos::Droid::Utility::ISparseArray;
using Elastos::Droid::Utility::ISparseInt32Array;
using Elastos::Droid::Internal::Utility::IIndentingPrintWriter;
using Elastos::Droid::Server::ISystemService;
using Elastos::Utility::Arrays;
using Elastos::Utility::ICollections;
using Elastos::Utility::ILocale;
using Elastos::Droid::Hardware::Hdmi::EIID_IIHdmiControlService;
using Elastos::Droid::Os::EIID_IBinder;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Hdmi {

//=============================================================================
// HdmiControlService::VendorCommandListenerRecord
//=============================================================================
CAR_INTERFACE_IMPL(HdmiControlService::VendorCommandListenerRecord, Object, IProxyDeathRecipient)

HdmiControlService::VendorCommandListenerRecord::VendorCommandListenerRecord(
    /* [in] */ IIHdmiVendorCommandListener* listener,
    /* [in] */ Int32 deviceType)
    : mListener(listener)
    , mDeviceType(deviceType)
{}

ECode HdmiControlService::VendorCommandListenerRecord::ProxyDied()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                synchronized(mLock) {
                    mVendorCommandListenerRecords->Remove(this);
                }

#endif
}

//=============================================================================
// HdmiControlService::HdmiControlBroadcastReceiver
//=============================================================================
ECode HdmiControlService::HdmiControlBroadcastReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                AssertRunOnServiceThread();
                switch (intent->GetAction()) {
                    case Intent::ACTION_SCREEN_OFF:
                        if (IsPowerOnOrTransient()) {
                            OnStandby();
                        }
                        break;
                    case Intent::ACTION_SCREEN_ON:
                        if (IsPowerStandbyOrTransient()) {
                            OnWakeUp();
                        }
                        break;
                    case Intent::ACTION_CONFIGURATION_CHANGED:
                        String language = Locale::GetDefault()->GetISO3Language();
                        if (!mLanguage->Equals(language)) {
                            OnLanguageChanged(language);
                        }
                        break;
                }

#endif
}

//=============================================================================
// HdmiControlService::SettingsObserver
//=============================================================================
HdmiControlService::SettingsObserver::SettingsObserver(
    /* [in] */ IHandler* handler)
{
    ContentObserver::constructor(handler);
}

ECode HdmiControlService::SettingsObserver::OnChange(
    /* [in] */ Boolean selfChange,
    /* [in] */ IUri* uri)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                String option = uri->GetLastPathSegment();
                Boolean enabled = ReadBooleanSetting(option, TRUE);
                switch (option) {
                    case Global::HDMI_CONTROL_ENABLED:
                        SetControlEnabled(enabled);
                        break;
                    case Global::HDMI_CONTROL_AUTO_WAKEUP_ENABLED:
                        AutoPtr<HdmiCecLocalDeviceTv> tv;
                        Tv((HdmiCecLocalDeviceTv**)&tv);
                        tv->SetAutoWakeup(enabled);
                        SetCecOption(OPTION_CEC_AUTO_WAKEUP, ToInt32(enabled));
                        break;
                    case Global::HDMI_CONTROL_AUTO_DEVICE_OFF_ENABLED:
                        AutoPtr<HdmiCecLocalDeviceTv> tv;
                        Tv((HdmiCecLocalDeviceTv**)&tv);
                        tv->SetAutoDeviceOff(enabled);
                        // No need to propagate to HAL.
                        break;
                    case Global::MHL_INPUT_SWITCHING_ENABLED:
                        SetMhlInputChangeEnabled(enabled);
                        break;
                    case Global::MHL_POWER_CHARGE_ENABLED:
                        mMhlController->SetOption(OPTION_MHL_POWER_CHARGE, ToInt32(enabled));
                        break;
                }

#endif
}

//=============================================================================
// HdmiControlService::HdmiMhlVendorCommandListenerRecord
//=============================================================================
CAR_INTERFACE_IMPL(HdmiControlService::HdmiMhlVendorCommandListenerRecord, Object, IProxyDeathRecipient)

HdmiControlService::HdmiMhlVendorCommandListenerRecord::HdmiMhlVendorCommandListenerRecord(
    /* [in] */ IIHdmiMhlVendorCommandListener* listener)
    : mListener(listener)
{

}

ECode HdmiControlService::HdmiMhlVendorCommandListenerRecord::ProxyDied()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                mMhlVendorCommandListenerRecords->Remove(this);

#endif
}

//=============================================================================
// HdmiControlService::HotplugEventListenerRecord
//=============================================================================
CAR_INTERFACE_IMPL(HdmiControlService::HotplugEventListenerRecord, Object, IProxyDeathRecipient)

HdmiControlService::HotplugEventListenerRecord::HotplugEventListenerRecord(
    /* [in] */ IIHdmiHotplugEventListener* listener)
    : mListener(listener)
{}

ECode HdmiControlService::HotplugEventListenerRecord::ProxyDied()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                synchronized(mLock) {
                    mHotplugEventListenerRecords->Remove(this);
                }

#endif
}

//=============================================================================
// HdmiControlService::DeviceEventListenerRecord
//=============================================================================
CAR_INTERFACE_IMPL(HdmiControlService::DeviceEventListenerRecord, Object, IProxyDeathRecipient)

HdmiControlService::DeviceEventListenerRecord::DeviceEventListenerRecord(
    /* [in] */ IIHdmiDeviceEventListener* listener)
    : mListener(listener)
{}

ECode HdmiControlService::DeviceEventListenerRecord::ProxyDied()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                synchronized(mLock) {
                    mDeviceEventListenerRecords->Remove(this);
                }

#endif
}

//=============================================================================
// HdmiControlService::SystemAudioModeChangeListenerRecord
//=============================================================================
CAR_INTERFACE_IMPL(HdmiControlService::SystemAudioModeChangeListenerRecord, Object, IProxyDeathRecipient)

HdmiControlService::SystemAudioModeChangeListenerRecord::SystemAudioModeChangeListenerRecord(
    /* [in] */ IIHdmiSystemAudioModeChangeListener* listener)
    : mListener(listener)
{}

ECode HdmiControlService::SystemAudioModeChangeListenerRecord::ProxyDied()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                synchronized(mLock) {
                    mSystemAudioModeChangeListenerRecords->Remove(this);
                }

#endif
}

//=============================================================================
// HdmiControlService::HdmiRecordListenerRecord
//=============================================================================
CAR_INTERFACE_IMPL(HdmiControlService::HdmiRecordListenerRecord, Object, IProxyDeathRecipient)

HdmiControlService::HdmiRecordListenerRecord::HdmiRecordListenerRecord(
    /* [in] */ IIHdmiRecordListener* listener)
    : mListener(listener)
{}

ECode HdmiControlService::HdmiRecordListenerRecord::ProxyDied()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                synchronized(mLock) {
                    mRecordListenerRecord = null;
                }

#endif
}

//=============================================================================
// HdmiControlService::BinderService
//=============================================================================
CAR_INTERFACE_IMPL_2(HdmiControlService::BinderService, Object, IIHdmiControlService, IBinder)

ECode HdmiControlService::BinderService::GetSupportedTypes(
    /* [out, callee] */ ArrayOf<Int32>** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                EnforceAccessPermission();
                // mLocalDevices is an unmodifiable list - no lock necesary.
                int[] localDevices = new int[mLocalDevices->Size()];
                for (Int32 i = 0; i < localDevices->GetLength(); ++i) {
                    AutoPtr<IInterface> obj;
                    mLocalDevices->Get(i, (IInterface**)&obj);
                    (*localDevices)[i] = I::Probe(obj);
                }
                return localDevices;

#endif
}

ECode HdmiControlService::BinderService::GetActiveSource(
    /* [out] */ IHdmiDeviceInfo** result)
{
    VALIDATE_NOT_NULL(result)

    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                EnforceAccessPermission();
                HdmiCecLocalDeviceTv tv = Tv();
                if (tv == null) {
                    Slogger::W(TAG, "Local tv device not available");
                    return null;
                }
                ActiveSource activeSource = tv->GetActiveSource();
                Boolean isValid;
                activeSource->IsValid(&isValid);
                if (isValid) {
                    return new HdmiDeviceInfo(activeSource->mLogicalAddress,
                            activeSource->mPhysicalAddress, IHdmiDeviceInfo::PORT_INVALID,
                            IHdmiDeviceInfo::DEVICE_INACTIVE, 0, "");
                }
                Int32 activePath;
                GetActivePath(&activePath);
                if (activePath != IHdmiDeviceInfo::PATH_INVALID) {
                    Int32 activePortId;
                    tv->GetActivePortId(&activePortId);
                    return new HdmiDeviceInfo(activePath, activePortId);
                }
                return null;

#endif
}

ECode HdmiControlService::BinderService::DeviceSelect(
    /* [in] */ Int32 deviceId,
    /* [in] */ IIHdmiControlCallback* callback)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                EnforceAccessPermission();
                RunOnServiceThread(new Runnable() {
                    @Override
                    public void Run() {
                        if (callback == null) {
                            Slogger::E(TAG, "Callback cannot be null");
                            return NOERROR;
                        }
                        HdmiCecLocalDeviceTv tv = Tv();
                        if (tv == null) {
                            Slogger::W(TAG, "Local tv device not available");
                            InvokeCallback(callback, IHdmiControlManager::RESULT_SOURCE_NOT_AVAILABLE);
                            return NOERROR;
                        }
                        HdmiMhlLocalDeviceStub device = mMhlController->GetLocalDeviceById(deviceId);
                        if (device != null) {
                            Int32 portId;
                            device->GetPortId(&portId);
                            Int32 activePortId;
                            tv->GetActivePortId(&activePortId);
                            if (portId == activePortId) {
                                InvokeCallback(callback, IHdmiControlManager::RESULT_SUCCESS);
                                return NOERROR;
                            }
                            // Upon selecting MHL device, we send RAP[Content On] to wake up
                            // the connected mobile device, start routing control to switch ports.
                            // callback is handled by MHL action.
                            device->TurnOn(callback);
                            Int32 portId;
                            device->GetPortId(&portId);
                            tv->DoManualPortSwitching(portId, null);
                            return NOERROR;
                        }
                        tv->DeviceSelect(deviceId, callback);
                    }
                });

#endif
}

ECode HdmiControlService::BinderService::PortSelect(
    /* [in] */ Int32 portId,
    /* [in] */ IIHdmiControlCallback* callback)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                EnforceAccessPermission();
                RunOnServiceThread(new Runnable() {
                    @Override
                    public void Run() {
                        if (callback == null) {
                            Slogger::E(TAG, "Callback cannot be null");
                            return NOERROR;
                        }
                        HdmiCecLocalDeviceTv tv = Tv();
                        if (tv == null) {
                            Slogger::W(TAG, "Local tv device not available");
                            InvokeCallback(callback, IHdmiControlManager::RESULT_SOURCE_NOT_AVAILABLE);
                            return NOERROR;
                        }
                        tv->DoManualPortSwitching(portId, callback);
                    }
                });

#endif
}

ECode HdmiControlService::BinderService::SendKeyEvent(
    /* [in] */ Int32 deviceType,
    /* [in] */ Int32 keyCode,
    /* [in] */ Boolean isPressed)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                EnforceAccessPermission();
                RunOnServiceThread(new Runnable() {
                    @Override
                    public void Run() {
                        HdmiMhlLocalDeviceStub device = mMhlController->GetLocalDevice(mActivePortId);
                        if (device != null) {
                            device->SendKeyEvent(keyCode, isPressed);
                            return NOERROR;
                        }
                        if (mCecController != null) {
                            HdmiCecLocalDevice localDevice = mCecController->GetLocalDevice(deviceType);
                            if (localDevice == null) {
                                Slogger::W(TAG, "Local device not available");
                                return NOERROR;
                            }
                            localDevice->SendKeyEvent(keyCode, isPressed);
                        }
                    }
                });

#endif
}

ECode HdmiControlService::BinderService::OneTouchPlay(
    /* [in] */ IIHdmiControlCallback* callback)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                EnforceAccessPermission();
                RunOnServiceThread(new Runnable() {
                    @Override
                    public void Run() {
                        HdmiControlService.this->OneTouchPlay(callback);
                    }
                });

#endif
}

ECode HdmiControlService::BinderService::QueryDisplayStatus(
    /* [in] */ IIHdmiControlCallback* callback)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                EnforceAccessPermission();
                RunOnServiceThread(new Runnable() {
                    @Override
                    public void Run() {
                        HdmiControlService.this->QueryDisplayStatus(callback);
                    }
                });

#endif
}

ECode HdmiControlService::BinderService::AddHotplugEventListener(
    /* [in] */ IIHdmiHotplugEventListener* listener)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                EnforceAccessPermission();
                HdmiControlService.this->AddHotplugEventListener(listener);

#endif
}

ECode HdmiControlService::BinderService::RemoveHotplugEventListener(
    /* [in] */ IIHdmiHotplugEventListener* listener)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                EnforceAccessPermission();
                HdmiControlService.this->RemoveHotplugEventListener(listener);

#endif
}

ECode HdmiControlService::BinderService::AddDeviceEventListener(
    /* [in] */ IIHdmiDeviceEventListener* listener)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                EnforceAccessPermission();
                HdmiControlService.this->AddDeviceEventListener(listener);

#endif
}

ECode HdmiControlService::BinderService::GetPortInfo(
    /* [out] */ IList** result)
{
    VALIDATE_NOT_NULL(result)

    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                EnforceAccessPermission();
                return HdmiControlService.this->GetPortInfo();

#endif
}

ECode HdmiControlService::BinderService::CanChangeSystemAudioMode(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                EnforceAccessPermission();
                HdmiCecLocalDeviceTv tv = Tv();
                if (tv == null) {
                    *result = FALSE;
                    return NOERROR;
                }
                return tv->HasSystemAudioDevice();

#endif
}

ECode HdmiControlService::BinderService::GetSystemAudioMode(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                EnforceAccessPermission();
                HdmiCecLocalDeviceTv tv = Tv();
                if (tv == null) {
                    *result = FALSE;
                    return NOERROR;
                }
                Boolean isSystemAudioActivated;
                tv->IsSystemAudioActivated(&isSystemAudioActivated);
                return isSystemAudioActivated;

#endif
}

ECode HdmiControlService::BinderService::SetSystemAudioMode(
    /* [in] */ Boolean enabled,
    /* [in] */ IIHdmiControlCallback* callback)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                EnforceAccessPermission();
                RunOnServiceThread(new Runnable() {
                    @Override
                    public void Run() {
                        HdmiCecLocalDeviceTv tv = Tv();
                        if (tv == null) {
                            Slogger::W(TAG, "Local tv device not available");
                            InvokeCallback(callback, IHdmiControlManager::RESULT_SOURCE_NOT_AVAILABLE);
                            return NOERROR;
                        }
                        tv->ChangeSystemAudioMode(enabled, callback);
                    }
                });

#endif
}

ECode HdmiControlService::BinderService::AddSystemAudioModeChangeListener(
    /* [in] */ IIHdmiSystemAudioModeChangeListener* listener)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                EnforceAccessPermission();
                HdmiControlService.this->AddSystemAudioModeChangeListner(listener);

#endif
}

ECode HdmiControlService::BinderService::RemoveSystemAudioModeChangeListener(
    /* [in] */ IIHdmiSystemAudioModeChangeListener* listener)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                EnforceAccessPermission();
                HdmiControlService.this->RemoveSystemAudioModeChangeListener(listener);

#endif
}

ECode HdmiControlService::BinderService::SetInputChangeListener(
    /* [in] */ IIHdmiInputChangeListener* listener)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                EnforceAccessPermission();
                HdmiControlService.this->SetInputChangeListener(listener);

#endif
}

ECode HdmiControlService::BinderService::GetInputDevices(
    /* [out] */ IList** result)
{
    VALIDATE_NOT_NULL(result)

    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                EnforceAccessPermission();
                // No need to hold the lock for obtaining TV device as the local device instance
                // is preserved while the HDMI control is enabled.
                HdmiCecLocalDeviceTv tv = Tv();
                synchronized(mLock) {
                    List<HdmiDeviceInfo> cecDevices = (tv == null)
                            ? Collections.<HdmiDeviceInfo>emptyList()
                            : tv->GetSafeExternalInputsLocked();
                    return HdmiUtils::MergeToUnmodifiableList(cecDevices, GetMhlDevicesLocked());
                }

#endif
}

ECode HdmiControlService::BinderService::SetSystemAudioVolume(
    /* [in] */ Int32 oldIndex,
    /* [in] */ Int32 newIndex,
    /* [in] */ Int32 maxIndex)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                EnforceAccessPermission();
                RunOnServiceThread(new Runnable() {
                    @Override
                    public void Run() {
                        HdmiCecLocalDeviceTv tv = Tv();
                        if (tv == null) {
                            Slogger::W(TAG, "Local tv device not available");
                            return NOERROR;
                        }
                        tv->ChangeVolume(oldIndex, newIndex - oldIndex, maxIndex);
                    }
                });

#endif
}

ECode HdmiControlService::BinderService::SetSystemAudioMute(
    /* [in] */ Boolean mute)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                EnforceAccessPermission();
                RunOnServiceThread(new Runnable() {
                    @Override
                    public void Run() {
                        HdmiCecLocalDeviceTv tv = Tv();
                        if (tv == null) {
                            Slogger::W(TAG, "Local tv device not available");
                            return NOERROR;
                        }
                        tv->ChangeMute(mute);
                    }
                });

#endif
}

ECode HdmiControlService::BinderService::SetArcMode(
    /* [in] */ Boolean enabled)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                EnforceAccessPermission();
                RunOnServiceThread(new Runnable() {
                    @Override
                    public void Run() {
                        HdmiCecLocalDeviceTv tv = Tv();
                        if (tv == null) {
                            Slogger::W(TAG, "Local tv device not available to change arc mode.");
                            return NOERROR;
                        }
                    }
                });

#endif
}

ECode HdmiControlService::BinderService::SetProhibitMode(
    /* [in] */ Boolean enabled)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                EnforceAccessPermission();
                if (!IsTvDevice()) {
                    return NOERROR;
                }
                HdmiControlService.this->SetProhibitMode(enabled);

#endif
}

ECode HdmiControlService::BinderService::AddVendorCommandListener(
    /* [in] */ IIHdmiVendorCommandListener* listener,
    /* [in] */ Int32 deviceType)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                EnforceAccessPermission();
                HdmiControlService.this->AddVendorCommandListener(listener, deviceType);

#endif
}

ECode HdmiControlService::BinderService::SendVendorCommand(
    /* [in] */ Int32 deviceType,
    /* [in] */ Int32 targetAddress,
    /* [in] */ ArrayOf<Byte>* params,
    /* [in] */ Boolean hasVendorId)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                EnforceAccessPermission();
                RunOnServiceThread(new Runnable() {
                    @Override
                    public void Run() {
                        AutoPtr<IHdmiCecLocalDevice> device = mCecController->GetLocalDevice(deviceType);
                        if (device == null) {
                            Slogger::W(TAG, "Local device not available");
                            return NOERROR;
                        }
                        if (hasVendorId) {
                            AutoPtr<IHdmiDeviceInfo> info;
                            device->GetDeviceInfo((IHdmiDeviceInfo**)&info);
                            Int32 logicalAddr;
                            info->GetLogicalAddress(&logicalAddr);
                            SendCecCommand(HdmiCecMessageBuilder::BuildVendorCommandWithId(
                                    logicalAddr, targetAddress,
                                    GetVendorId(), params));
                        } else {
                            AutoPtr<IHdmiCecMessage> cmd;
                            AutoPtr<IHdmiDeviceInfo> deviceInfo;
                            device->GetDeviceInfo((IHdmiDeviceInfo**)&deviceInfo);
                            Int32 logicalAddress;
                            deviceInfo->GetLogicalAddress(&logicalAddress);
                            HdmiCecMessageBuilder::BuildVendorCommand(logicalAddress, targetAddress, params, (IHdmiCecMessage**)&cmd);
                            SendCecCommand(cmd);
                        }
                    }
                });

#endif
}

ECode HdmiControlService::BinderService::SendStandby(
    /* [in] */ Int32 deviceType,
    /* [in] */ Int32 deviceId)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                EnforceAccessPermission();
                RunOnServiceThread(new Runnable() {
                    @Override
                    public void Run() {
                        AutoPtr<IHdmiCecLocalDevice> device = mCecController->GetLocalDevice(deviceType);
                        if (device == null) {
                            Slogger::W(TAG, "Local device not available");
                            return NOERROR;
                        }
                        device->SendStandby(deviceId);
                    }
                });

#endif
}

ECode HdmiControlService::BinderService::SetHdmiRecordListener(
    /* [in] */ IIHdmiRecordListener* listener)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                EnforceAccessPermission();
                HdmiControlService.this->SetHdmiRecordListener(listener);

#endif
}

ECode HdmiControlService::BinderService::StartOneTouchRecord(
    /* [in] */ Int32 recorderAddress,
    /* [in] */ ArrayOf<Byte>* recordSource)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                EnforceAccessPermission();
                RunOnServiceThread(new Runnable() {
                    @Override
                    public void Run() {
                        if (!IsTvDevice()) {
                            Slogger::W(TAG, "No TV is available.");
                            return NOERROR;
                        }
                        AutoPtr<HdmiCecLocalDeviceTv> tv;
                        Tv((HdmiCecLocalDeviceTv**)&tv);
                        tv->StartOneTouchRecord(recorderAddress, recordSource);
                    }
                });

#endif
}

ECode HdmiControlService::BinderService::StopOneTouchRecord(
    /* [in] */ Int32 recorderAddress)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                EnforceAccessPermission();
                RunOnServiceThread(new Runnable() {
                    @Override
                    public void Run() {
                        if (!IsTvDevice()) {
                            Slogger::W(TAG, "No TV is available.");
                            return NOERROR;
                        }
                        AutoPtr<HdmiCecLocalDeviceTv> tv;
                        Tv((HdmiCecLocalDeviceTv**)&tv);
                        tv->StopOneTouchRecord(recorderAddress);
                    }
                });

#endif
}

ECode HdmiControlService::BinderService::StartTimerRecording(
    /* [in] */ Int32 recorderAddress,
    /* [in] */ Int32 sourceType,
    /* [in] */ ArrayOf<Byte>* recordSource)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                EnforceAccessPermission();
                RunOnServiceThread(new Runnable() {
                    @Override
                    public void Run() {
                        if (!IsTvDevice()) {
                            Slogger::W(TAG, "No TV is available.");
                            return NOERROR;
                        }
                        AutoPtr<HdmiCecLocalDeviceTv> tv;
                        Tv((HdmiCecLocalDeviceTv**)&tv);
                        tv->StartTimerRecording(recorderAddress, sourceType, recordSource);
                    }
                });

#endif
}

ECode HdmiControlService::BinderService::ClearTimerRecording(
    /* [in] */ Int32 recorderAddress,
    /* [in] */ Int32 sourceType,
    /* [in] */ ArrayOf<Byte>* recordSource)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                EnforceAccessPermission();
                RunOnServiceThread(new Runnable() {
                    @Override
                    public void Run() {
                        if (!IsTvDevice()) {
                            Slogger::W(TAG, "No TV is available.");
                            return NOERROR;
                        }
                        AutoPtr<HdmiCecLocalDeviceTv> tv;
                        Tv((HdmiCecLocalDeviceTv**)&tv);
                        tv->ClearTimerRecording(recorderAddress, sourceType, recordSource);
                    }
                });

#endif
}

ECode HdmiControlService::BinderService::SendMhlVendorCommand(
    /* [in] */ Int32 portId,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [in] */ ArrayOf<Byte>* data)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                EnforceAccessPermission();
                RunOnServiceThread(new Runnable() {
                    @Override
                    public void Run() {
                        if (!IsControlEnabled()) {
                            Slogger::W(TAG, "Hdmi control is disabled.");
                            return ;
                        }
                        HdmiMhlLocalDeviceStub device = mMhlController->GetLocalDevice(portId);
                        if (device == null) {
                            Slogger::W(TAG, "Invalid port id:" + portId);
                            return NOERROR;
                        }
                        mMhlController->SendVendorCommand(portId, offset, length, data);
                    }
                });

#endif
}

ECode HdmiControlService::BinderService::AddHdmiMhlVendorCommandListener(
    /* [in] */ IIHdmiMhlVendorCommandListener* listener)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                EnforceAccessPermission();
                HdmiControlService.this->AddHdmiMhlVendorCommandListener(listener);

#endif
}

ECode HdmiControlService::BinderService::Dump(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ IPrintWriter* writer,
    /* [in] */ ArrayOf<String>* args)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                GetContext()->EnforceCallingOrSelfPermission(android.Manifest.permission.DUMP, TAG);
                IndentingPrintWriter pw = new IndentingPrintWriter(writer, "  ");
                pw->Println("mHdmiControlEnabled: " + mHdmiControlEnabled);
                pw->Println("mProhibitMode: " + mProhibitMode);
                if (mCecController != null) {
                    pw->Println("mCecController: ");
                    pw->IncreaseIndent();
                    mCecController->Dump(pw);
                    pw->DecreaseIndent();
                }
                pw->Println("mPortInfo: ");
                pw->IncreaseIndent();
                for (HdmiPortInfo hdmiPortInfo : mPortInfo) {
                    pw->Println("- " + hdmiPortInfo);
                }
                pw->DecreaseIndent();
                pw->Println("mPowerStatus: " + mPowerStatus);

#endif
}

ECode HdmiControlService::BinderService::ToString(
    /* [out] */ String* result)
{
    return IObject::Probe(TO_IINTERFACE(this))->ToString(result);
}

//=============================================================================
// HdmiControlService::InputChangeListenerRecord
//=============================================================================
CAR_INTERFACE_IMPL(HdmiControlService::InputChangeListenerRecord, Object, IProxyDeathRecipient)

HdmiControlService::InputChangeListenerRecord::InputChangeListenerRecord(
    /* [in] */ IIHdmiInputChangeListener* listener)
    : mListener(listener)
{}

ECode HdmiControlService::InputChangeListenerRecord::ProxyDied()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
                synchronized(mLock) {
                    mInputChangeListenerRecord = null;
                }

#endif
}

//=============================================================================
// HdmiControlService
//=============================================================================
const String HdmiControlService::TAG("HdmiControlService");
const String HdmiControlService::PERMISSION("android.permission.HDMI_CEC");
const Int32 HdmiControlService::INITIATED_BY_ENABLE_CEC = 0;
const Int32 HdmiControlService::INITIATED_BY_BOOT_UP = 1;
const Int32 HdmiControlService::INITIATED_BY_SCREEN_ON = 2;
const Int32 HdmiControlService::INITIATED_BY_WAKE_UP_MESSAGE = 3;
const Int32 HdmiControlService::INITIATED_BY_HOTPLUG = 4;

HdmiControlService::HdmiControlService()
    : mHdmiControlEnabled(FALSE)
    , mProhibitMode(FALSE)
    , mPowerStatus(IHdmiControlManager::POWER_STATUS_STANDBY)
    , mStandbyMessageReceived(FALSE)
    , mWakeUpMessageReceived(FALSE)
    , mActivePortId(Constants::INVALID_PORT_ID)
    , mMhlInputChangeEnabled(FALSE)
    , mLastInputMhl(Constants::INVALID_PORT_ID)
{
#if 0 // TODO: Translate codes below
    mIoThread = new HandlerThread("Hdmi Control Io Thread");
    mLock = new Object();
    CArrayList::New((IArrayList**)&mDeviceEventListenerRecords);
    CArrayList::New((IArrayList**)&mVendorCommandListenerRecords);
    CArrayList::New((IArrayList**)&mSystemAudioModeChangeListenerRecords);
    mHandler = new Handler();
    mHdmiControlBroadcastReceiver = new HdmiControlBroadcastReceiver();
    mLanguage = Locale::GetDefault()->GetISO3Language();
    CArrayList::New((IArrayList**)&mMhlVendorCommandListenerRecords);
    CArrayList::New((IArrayList**)&mHotplugEventListenerRecords);
#endif
}

ECode HdmiControlService::constructor(
    /* [in] */ IContext* context)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        super::constructor(context);
        AutoPtr<ISystemProperties> helper;
        CSystemProperties::AcquireSingleton((ISystemProperties**)&helper);
        mLocalDevices = GetIntList(helper->Get(Constants::PROPERTY_DEVICE_TYPE));
        mSettingsObserver = new SettingsObserver(mHandler);

#endif
}

ECode HdmiControlService::GetIntList(
    /* [in] */ const String& string,
    /* [out] */ IList** result)
{
    VALIDATE_NOT_NULL(result)

    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        AutoPtr<IArrayList> list;
        CArrayList::New((IArrayList**)&list);
        TextUtils::SimpleStringSplitter splitter = new TextUtils::SimpleStringSplitter(',');
        splitter->SetString(string);
        for (String item : splitter) {
            try {
                list->Add(Integer::ParseInt32(item));
            } catch (NumberFormatException e) {
                Slogger::W(TAG, "Can't parseInt: " + item);
            }
        }
        return Collections::UnmodifiableList(list);

#endif
}

ECode HdmiControlService::OnStart()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mIoThread->Start();
        mPowerStatus = IHdmiControlManager::POWER_STATUS_TRANSIENT_TO_ON;
        mProhibitMode = FALSE;
        mHdmiControlEnabled = ReadBooleanSetting(Global::HDMI_CONTROL_ENABLED, TRUE);
        mMhlInputChangeEnabled = ReadBooleanSetting(Global::MHL_INPUT_SWITCHING_ENABLED, TRUE);
        mCecController = HdmiCecController::Create(this);
        if (mCecController != NULL) {
            // TODO: Remove this as soon as OEM's HAL implementation is corrected.
            mCecController->SetOption(OPTION_CEC_ENABLE, ENABLED);
            // TODO: load value for mHdmiControlEnabled from preference.
            if (mHdmiControlEnabled) {
                InitializeCec(INITIATED_BY_BOOT_UP);
            }
        } else {
            Slogger::I(TAG, "Device does not support HDMI-CEC.");
        }
        mMhlController = HdmiMhlControllerStub::Create(this);
        Boolean isReady;
        mMhlController->IsReady(&isReady);
        if (!isReady) {
            Slogger::I(TAG, "Device does not support MHL-control.");
        }
        mMhlDevices = Collections::EmptyList();
        InitPortInfo();
        mMessageValidator = new HdmiCecMessageValidator(this);
        PublishBinderService(Context::HDMI_CONTROL_SERVICE, new BinderService());
        // Register broadcast receiver for power state change.
        if (mCecController != NULL) {
            IntentFilter filter = new IntentFilter();
            filter->AddAction(Intent::ACTION_SCREEN_OFF);
            filter->AddAction(Intent::ACTION_SCREEN_ON);
            filter->AddAction(Intent::ACTION_CONFIGURATION_CHANGED);
            GetContext()->RegisterReceiver(mHdmiControlBroadcastReceiver, filter);
        }

#endif
}

ECode HdmiControlService::OnInitializeCecComplete()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (mPowerStatus == IHdmiControlManager::POWER_STATUS_TRANSIENT_TO_ON) {
            mPowerStatus = IHdmiControlManager::POWER_STATUS_ON;
        }
        mWakeUpMessageReceived = FALSE;
        if (IsTvDevice()) {
            mCecController->SetOption(OPTION_CEC_AUTO_WAKEUP, ToInt32(tv()->GetAutoWakeup()));
            RegisterContentObserver();
        }

#endif
}

ECode HdmiControlService::RegisterContentObserver()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        ContentResolver resolver = GetContext()->GetContentResolver();
        AutoPtr<ArrayOf<String> > settings = new String[] {
                Global::HDMI_CONTROL_ENABLED,
                Global::HDMI_CONTROL_AUTO_WAKEUP_ENABLED,
                Global::HDMI_CONTROL_AUTO_DEVICE_OFF_ENABLED,
                Global::MHL_INPUT_SWITCHING_ENABLED,
                Global::MHL_POWER_CHARGE_ENABLED
        };
        for (String s : settings) {
            resolver->RegisterContentObserver(Global::GetUriFor(s), FALSE, mSettingsObserver,
                    UserHandle::USER_ALL);
        }

#endif
}

ECode HdmiControlService::ToInt32(
    /* [in] */ Boolean enabled,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)

    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return enabled ? ENABLED : DISABLED;

#endif
}

ECode HdmiControlService::ReadBooleanSetting(
    /* [in] */ const String& key,
    /* [in] */ Boolean defVal,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        ContentResolver cr = GetContext()->GetContentResolver();
        return Global::GetInt32(cr, key, ToInt32(defVal)) == ENABLED;

#endif
}

ECode HdmiControlService::WriteBooleanSetting(
    /* [in] */ const String& key,
    /* [in] */ Boolean value)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        ContentResolver cr = GetContext()->GetContentResolver();
        Global::PutInt32(cr, key, ToInt32(value));

#endif
}

ECode HdmiControlService::UnregisterSettingsObserver()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        GetContext()->GetContentResolver()->UnregisterContentObserver(mSettingsObserver);

#endif
}

ECode HdmiControlService::InitializeCec(
    /* [in] */ Int32 initiatedBy)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mCecController->SetOption(OPTION_CEC_SERVICE_CONTROL, ENABLED);
        InitializeLocalDevices(initiatedBy);

#endif
}

ECode HdmiControlService::InitializeLocalDevices(
    /* [in] */ Int32 initiatedBy)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        AssertRunOnServiceThread();
        // A container for [Device type, Local device info].
        AutoPtr<IArrayList> localDevices;
        CArrayList::New((IArrayList**)&localDevices);
        for (Int32 type : mLocalDevices) {
            AutoPtr<IHdmiCecLocalDevice> localDevice = mCecController->GetLocalDevice(type);
            if (localDevice == NULL) {
                localDevice = HdmiCecLocalDevice::Create(this, type);
            }
            localDevice->Init();
            localDevices->Add(localDevice);
        }
        // It's now safe to flush existing local devices from mCecController since they were
        // already moved to 'localDevices'.
        ClearLocalDevices();
        AllocateLogicalAddress(localDevices, initiatedBy);

#endif
}

ECode HdmiControlService::AllocateLogicalAddress(
    /* [in] */ IArrayList* allocatingDevices,
    /* [in] */ Int32 initiatedBy)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        AssertRunOnServiceThread();
        mCecController->ClearLogicalAddress();
        AutoPtr<IArrayList> allocatedDevices;
        CArrayList::New((IArrayList**)&allocatedDevices);
        int[] finished = new int[1];
        FOR_EACH(it : allocatingDevices) {
            AutoPtr<IHdmiCecLocalDevice> localDevice = it->GetNext();
            mCecController->AllocateLogicalAddress(localDevice->GetType(),
                    localDevice->GetPreferredAddress(), new AllocateAddressCallback() {
                @Override
                public void OnAllocated(Int32 deviceType, Int32 logicalAddress) {
                    if (logicalAddress == Constants::ADDR_UNREGISTERED) {
                        Slogger::E(TAG, "Failed to allocate address:[device_type:" + deviceType + "]");
                    } else {
                        // Set POWER_STATUS_ON to all local devices because they share lifetime
                        // with system.
                        HdmiDeviceInfo deviceInfo = CreateDeviceInfo(logicalAddress, deviceType,
                                IHdmiControlManager::POWER_STATUS_ON);
                        localDevice->SetDeviceInfo(deviceInfo);
                        mCecController->AddLocalDevice(deviceType, localDevice);
                        mCecController->AddLogicalAddress(logicalAddress);
                        allocatedDevices->Add(localDevice);
                    }
                    // Address allocation completed for all devices. Notify each device.
                    Int32 size;
                    allocatingDevices->GetSize(&size);
                    if (size == ++(*finished)[0]) {
                        if (initiatedBy != INITIATED_BY_HOTPLUG) {
                            // In case of the hotplug we don't call onInitializeCecComplete()
                            // since we reallocate the logical address only.
                            OnInitializeCecComplete();
                        }
                        NotifyAddressAllocated(allocatedDevices, initiatedBy);
                    }
                }
            });
        }

#endif
}

ECode HdmiControlService::NotifyAddressAllocated(
    /* [in] */ IArrayList* devices,
    /* [in] */ Int32 initiatedBy)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        AssertRunOnServiceThread();
        FOR_EACH(it : devices) {
            AutoPtr<IHdmiCecLocalDevice> device = it->GetNextt();
            AutoPtr<IHdmiDeviceInfo> deviceInfo;
            device->GetDeviceInfo((IHdmiDeviceInfo**)&deviceInfo);
            Int32 address;
            deviceInfo->GetLogicalAddress(&address);
            device->HandleAddressAllocated(address, initiatedBy);
        }

#endif
}

ECode HdmiControlService::InitPortInfo()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        AssertRunOnServiceThread();
        AutoPtr<ArrayOf<HdmiPortInfo> > cecPortInfo = NULL;
        // CEC HAL provides majority of the info while MHL does only MHL support flag for
        // each port. Return empty array if CEC HAL didn't provide the info.
        if (mCecController != NULL) {
            cecPortInfo = mCecController->GetPortInfos();
        }
        if (cecPortInfo == NULL) {
            return NOERROR;
        }
        SparseArray<HdmiPortInfo> portInfoMap = new SparseArray<>();
        SparseIntArray portIdMap = new SparseIntArray();
        SparseArray<HdmiDeviceInfo> portDeviceMap = new SparseArray<>();
        for (HdmiPortInfo info : cecPortInfo) {
            portIdMap->Put(info->GetAddress(), info->GetId());
            portInfoMap->Put(info->GetId(), info);
            portDeviceMap->Put(info->GetId(), new HdmiDeviceInfo(info->GetAddress(), info->GetId()));
        }
        mPortIdMap = new UnmodifiableSparseIntArray(portIdMap);
        mPortInfoMap = new UnmodifiableSparseArray<>(portInfoMap);
        mPortDeviceMap = new UnmodifiableSparseArray<>(portDeviceMap);
        AutoPtr<ArrayOf<HdmiPortInfo> > mhlPortInfo = mMhlController->GetPortInfos();
        ArraySet<Integer> mhlSupportedPorts = new ArraySet<Integer>(mhlPortInfo->GetLength());
        for (HdmiPortInfo info : mhlPortInfo) {
            Boolean isMhlSupported;
            info->IsMhlSupported(&isMhlSupported);
            if (isMhlSupported) {
                mhlSupportedPorts->Add(info->GetId());
            }
        }
        // Build HDMI port info list with CEC port info plus MHL supported flag. We can just use
        // cec port info if we do not have have port that supports MHL.
        Boolean isEmpty;
        mhlSupportedPorts->IsEmpty(&isEmpty);
        if (isEmpty) {
            mPortInfo = Collections::UnmodifiableList(Arrays::AsList(cecPortInfo));
            return NOERROR;
        }
        ArrayList<HdmiPortInfo> result = new ArrayList<>(cecPortInfo->GetLength());
        for (HdmiPortInfo info : cecPortInfo) {
            if (mhlSupportedPorts->Contains(info->GetId())) {
                Boolean isArcSupported;
                info->IsArcSupported(&isArcSupported);
                Boolean isCecSupported;
                info->IsCecSupported(&isCecSupported);
                result->Add(new HdmiPortInfo(info->GetId(), info->GetType(), info->GetAddress(),
                        isCecSupported, TRUE, isArcSupported));
            } else {
                result->Add(info);
            }
        }
        mPortInfo = Collections::UnmodifiableList(result);

#endif
}

ECode HdmiControlService::GetPortInfo(
    /* [out] */ IList** result)
{
    VALIDATE_NOT_NULL(result)

    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return mPortInfo;

#endif
}

ECode HdmiControlService::GetPortInfo(
    /* [in] */ Int32 portId,
    /* [out] */ IHdmiPortInfo** result)
{
    VALIDATE_NOT_NULL(result)

    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return mPortInfoMap->Get(portId, NULL);

#endif
}

ECode HdmiControlService::PortIdToPath(
    /* [in] */ Int32 portId,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)

    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        HdmiPortInfo portInfo = GetPortInfo(portId);
        if (portInfo == NULL) {
            Slogger::E(TAG, "Cannot find the port info: " + portId);
            return Constants::INVALID_PHYSICAL_ADDRESS;
        }
        return portInfo->GetAddress();

#endif
}

ECode HdmiControlService::PathToPortId(
    /* [in] */ Int32 path,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)

    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        Int32 portAddress = path & Constants::ROUTING_PATH_TOP_MASK;
        return mPortIdMap->Get(portAddress, Constants::INVALID_PORT_ID);

#endif
}

ECode HdmiControlService::IsValidPortId(
    /* [in] */ Int32 portId,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return GetPortInfo(portId) != NULL;

#endif
}

ECode HdmiControlService::GetIoLooper(
    /* [out] */ ILooper** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return mIoThread->GetLooper(result);

#endif
}

ECode HdmiControlService::GetServiceLooper(
    /* [out] */ ILooper** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return mHandler->GetLooper(result);

#endif
}

ECode HdmiControlService::GetPhysicalAddress(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)

    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return mCecController->GetPhysicalAddress(result);

#endif
}

ECode HdmiControlService::GetVendorId(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)

    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        Int32 vendorId;
        mCecController->GetVendorId(&vendorId);
        return vendorId;

#endif
}

ECode HdmiControlService::GetDeviceInfo(
    /* [in] */ Int32 logicalAddress,
    /* [out] */ IHdmiDeviceInfo** result)
{
    VALIDATE_NOT_NULL(result)

    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        AssertRunOnServiceThread();
        HdmiCecLocalDeviceTv tv = Tv();
        if (tv == NULL) {
            *result = NULL;
            return NOERROR;
        }
        return tv->GetCecDeviceInfo(logicalAddress, result);

#endif
}

ECode HdmiControlService::GetCecVersion(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)

    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return mCecController->GetVersion();

#endif
}

ECode HdmiControlService::IsConnectedToArcPort(
    /* [in] */ Int32 physicalAddress,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        Int32 portId;
        PathToPortId(physicalAddress, &portId);
        if (portId != Constants::INVALID_PORT_ID) {
            AutoPtr<IInterface> obj;
            mPortInfoMap->Get(portId, (IInterface**)&obj);
            return I::Probe(obj)->IsArcSupported();
        }
        *result = FALSE;
        return NOERROR;

#endif
}

ECode HdmiControlService::RunOnServiceThread(
    /* [in] */ IRunnable* runnable)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mHandler->Post(runnable);

#endif
}

ECode HdmiControlService::RunOnServiceThreadAtFrontOfQueue(
    /* [in] */ IRunnable* runnable)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mHandler->PostAtFrontOfQueue(runnable);

#endif
}

ECode HdmiControlService::AssertRunOnServiceThread()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        AutoPtr<ILooper> looper;
        mHandler->GetLooper((ILooper**)&looper);
        if (Looper::GetMyLooper() != looper) {
            Logger::E(TAG, "Should run on service thread.");
            return E_ILLEGAL_STATE_EXCEPTION;
        }

#endif
}

ECode HdmiControlService::SendCecCommand(
    /* [in] */ IHdmiCecMessage* command,
    /* [in] */ IHdmiControlServiceSendMessageCallback* callback)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        AssertRunOnServiceThread();
        Boolean isValid;
        mMessageValidator->IsValid(command, &isValid);
        if (isValid == HdmiCecMessageValidator::OK) {
            mCecController->SendCommand(command, callback);
        } else {
            HdmiLogger::Error("Invalid message type:" + command);
            if (callback != NULL) {
                callback->OnSendCompleted(Constants::SEND_RESULT_FAILURE);
            }
        }

#endif
}

ECode HdmiControlService::SendCecCommand(
    /* [in] */ IHdmiCecMessage* command)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        AssertRunOnServiceThread();
        SendCecCommand(command, NULL);

#endif
}

ECode HdmiControlService::MaySendFeatureAbortCommand(
    /* [in] */ IHdmiCecMessage* command,
    /* [in] */ Int32 reason)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        AssertRunOnServiceThread();
        mCecController->MaySendFeatureAbortCommand(command, reason);

#endif
}

ECode HdmiControlService::HandleCecCommand(
    /* [in] */ IHdmiCecMessage* message,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        AssertRunOnServiceThread();
        Int32 errorCode = mMessageValidator->IsValid(message);
        if (errorCode != HdmiCecMessageValidator::OK) {
            // We'll not response on the messages with the invalid source or destination.
            if (errorCode == HdmiCecMessageValidator::ERROR_PARAMETER) {
                MaySendFeatureAbortCommand(message, Constants::ABORT_INVALID_OPERAND);
            }
            *result = TRUE;
            return NOERROR;
        }
        return DispatchMessageToLocalDevice(message);

#endif
}

ECode HdmiControlService::SetAudioReturnChannel(
    /* [in] */ Boolean enabled)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        mCecController->SetAudioReturnChannel(enabled);

#endif
}

ECode HdmiControlService::DispatchMessageToLocalDevice(
    /* [in] */ IHdmiCecMessage* message,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        AssertRunOnServiceThread();
        FOR_EACH(it : mCecController->GetLocalDeviceList()) {
            AutoPtr<IHdmiCecLocalDevice> device = it->GetNext();
            Int32 dest;
            message->GetDestination(&dest);
            if (device->DispatchMessage(message)
                    && dest != Constants::ADDR_BROADCAST) {
                *result = TRUE;
                return NOERROR;
            }
        }
        Int32 dest;
        message->GetDestination(&dest);
        if (dest != Constants::ADDR_BROADCAST) {
            HdmiLogger::Warning("Unhandled cec command:" + message);
        }
        *result = FALSE;
        return NOERROR;

#endif
}

ECode HdmiControlService::OnHotplug(
    /* [in] */ Int32 portId,
    /* [in] */ Boolean connected)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        AssertRunOnServiceThread();
        AutoPtr<IArrayList> localDevices;
        CArrayList::New((IArrayList**)&localDevices);
        for (Int32 type : mLocalDevices) {
            if (type == IHdmiDeviceInfo::DEVICE_TV) {
                // Skip the reallocation of the logical address on TV.
                continue;
            }
            AutoPtr<IHdmiCecLocalDevice> localDevice = mCecController->GetLocalDevice(type);
            if (localDevice == NULL) {
                localDevice = HdmiCecLocalDevice::Create(this, type);
                localDevice->Init();
            }
            localDevices->Add(localDevice);
        }
        AllocateLogicalAddress(localDevices, INITIATED_BY_HOTPLUG);
        FOR_EACH(it : mCecController->GetLocalDeviceList()) {
            AutoPtr<IHdmiCecLocalDevice> device = it->GetNext();
            device->OnHotplug(portId, connected);
        }
        AnnounceHotplugEvent(portId, connected);

#endif
}

ECode HdmiControlService::PollDevices(
    /* [in] */ IHdmiControlServiceDevicePollingCallback* callback,
    /* [in] */ Int32 sourceAddress,
    /* [in] */ Int32 pickStrategy,
    /* [in] */ Int32 retryCount)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        AssertRunOnServiceThread();
        mCecController->PollDevices(callback, sourceAddress, CheckPollStrategy(pickStrategy),
                retryCount);

#endif
}

ECode HdmiControlService::CheckPollStrategy(
    /* [in] */ Int32 pickStrategy,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)

    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        Int32 strategy = pickStrategy & Constants::POLL_STRATEGY_MASK;
        if (strategy == 0) {
            Logger::E(TAG, "Invalid poll strategy:" + pickStrategy);
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
        Int32 iterationStrategy = pickStrategy & Constants::POLL_ITERATION_STRATEGY_MASK;
        if (iterationStrategy == 0) {
            Logger::E(TAG, "Invalid iteration strategy:" + pickStrategy);
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
        return strategy | iterationStrategy;

#endif
}

ECode HdmiControlService::GetAllLocalDevices(
    /* [out] */ IList** result)
{
    VALIDATE_NOT_NULL(result)

    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        AssertRunOnServiceThread();
        return mCecController->GetLocalDeviceList();

#endif
}

ECode HdmiControlService::GetServiceLock(
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result)

    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return mLock;

#endif
}

ECode HdmiControlService::SetAudioStatus(
    /* [in] */ Boolean mute,
    /* [in] */ Int32 volume)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        AudioManager audioManager = GetAudioManager();
        Boolean muted = audioManager->IsStreamMute(AudioManager::STREAM_MUSIC);
        if (mute) {
            if (!muted) {
                audioManager->SetStreamMute(AudioManager::STREAM_MUSIC, TRUE);
            }
        } else {
            if (muted) {
                audioManager->SetStreamMute(AudioManager::STREAM_MUSIC, FALSE);
            }
            // FLAG_HDMI_SYSTEM_AUDIO_VOLUME prevents audio manager from announcing
            // volume change notification back to hdmi control service.
            audioManager->SetStreamVolume(AudioManager::STREAM_MUSIC, volume,
                    AudioManager::FLAG_SHOW_UI | AudioManager::FLAG_HDMI_SYSTEM_AUDIO_VOLUME);
        }

#endif
}

ECode HdmiControlService::AnnounceSystemAudioModeChange(
    /* [in] */ Boolean enabled)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        synchronized(mLock) {
            for (SystemAudioModeChangeListenerRecord record :
                    mSystemAudioModeChangeListenerRecords) {
                InvokeSystemAudioModeChangeLocked(record.mListener, enabled);
            }
        }

#endif
}

ECode HdmiControlService::CreateDeviceInfo(
    /* [in] */ Int32 logicalAddress,
    /* [in] */ Int32 deviceType,
    /* [in] */ Int32 powerStatus,
    /* [out] */ IHdmiDeviceInfo** result)
{
    VALIDATE_NOT_NULL(result)

    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        // TODO: find better name instead of model name.
        String displayName = Build::MODEL;
        Int32 physicalAddr;
        GetPhysicalAddress(&physicalAddr);
        Int32 portId;
        PathToPortId(physicalAddr, &portId);
        return new HdmiDeviceInfo(logicalAddress,
                physicalAddr, portId, deviceType,
                GetVendorId(), displayName);

#endif
}

ECode HdmiControlService::HandleMhlHotplugEvent(
    /* [in] */ Int32 portId,
    /* [in] */ Boolean connected)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        AssertRunOnServiceThread();
        if (connected) {
            HdmiMhlLocalDeviceStub newDevice = new HdmiMhlLocalDeviceStub(this, portId);
            HdmiMhlLocalDeviceStub oldDevice = mMhlController->AddLocalDevice(newDevice);
            if (oldDevice != NULL) {
                oldDevice->OnDeviceRemoved();
                Slogger::I(TAG, "Old device of port " + portId + " is removed");
            }
        } else {
            HdmiMhlLocalDeviceStub device = mMhlController->RemoveLocalDevice(portId);
            if (device != NULL) {
                device->OnDeviceRemoved();
                // There is no explicit event for device removal.
                // Hence we remove the device on hotplug event.
                HdmiDeviceInfo deviceInfo = device->GetInfo();
                if (deviceInfo != NULL) {
                    InvokeDeviceEventListeners(deviceInfo, DEVICE_EVENT_REMOVE_DEVICE);
                    UpdateSafeMhlInput();
                }
            } else {
                Slogger::W(TAG, "No device to remove:[portId=" + portId);
            }
        }
        AnnounceHotplugEvent(portId, connected);

#endif
}

ECode HdmiControlService::HandleMhlBusModeChanged(
    /* [in] */ Int32 portId,
    /* [in] */ Int32 busmode)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        AssertRunOnServiceThread();
        HdmiMhlLocalDeviceStub device = mMhlController->GetLocalDevice(portId);
        if (device != NULL) {
            device->SetBusMode(busmode);
        } else {
            Slogger::W(TAG, "No mhl device exists for bus mode change[portId:" + portId +
                    ", busmode:" + busmode + "]");
        }

#endif
}

ECode HdmiControlService::HandleMhlBusOvercurrent(
    /* [in] */ Int32 portId,
    /* [in] */ Boolean on)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        AssertRunOnServiceThread();
        HdmiMhlLocalDeviceStub device = mMhlController->GetLocalDevice(portId);
        if (device != NULL) {
            device->OnBusOvercurrentDetected(on);
        } else {
            Slogger::W(TAG, "No mhl device exists for bus overcurrent event[portId:" + portId + "]");
        }

#endif
}

ECode HdmiControlService::HandleMhlDeviceStatusChanged(
    /* [in] */ Int32 portId,
    /* [in] */ Int32 adopterId,
    /* [in] */ Int32 deviceId)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        AssertRunOnServiceThread();
        HdmiMhlLocalDeviceStub device = mMhlController->GetLocalDevice(portId);
        // Hotplug event should already have been called before device status change event.
        if (device != NULL) {
            device->SetDeviceStatusChange(adopterId, deviceId);
            InvokeDeviceEventListeners(device->GetInfo(), DEVICE_EVENT_ADD_DEVICE);
            UpdateSafeMhlInput();
        } else {
            Slogger::W(TAG, "No mhl device exists for device status event[portId:"
                    + portId + ", adopterId:" + adopterId + ", deviceId:" + deviceId + "]");
        }

#endif
}

ECode HdmiControlService::UpdateSafeMhlInput()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        AssertRunOnServiceThread();
        List<HdmiDeviceInfo> inputs = Collections::EmptyList();
        SparseArray<HdmiMhlLocalDeviceStub> devices = mMhlController->GetAllLocalDevices();
        Int32 size;
        devices->GetSize(&size);
        for (Int32 i = 0; i < size; ++i) {
            HdmiMhlLocalDeviceStub device = devices->ValueAt(i);
            HdmiDeviceInfo info = device->GetInfo();
            if (info != NULL) {
                Boolean isEmpty;
                inputs->IsEmpty(&isEmpty);
                if (isEmpty) {
                    CArrayList::New((IArrayList**)&inputs);
                }
                inputs->Add(device->GetInfo());
            }
        }
        synchronized(mLock) {
            mMhlDevices = inputs;
        }

#endif
}

ECode HdmiControlService::GetMhlDevicesLocked(
    /* [out] */ IList** result)
{
    VALIDATE_NOT_NULL(result)

    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return mMhlDevices;

#endif
}

ECode HdmiControlService::EnforceAccessPermission()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        GetContext()->EnforceCallingOrSelfPermission(PERMISSION, TAG);

#endif
}

ECode HdmiControlService::OneTouchPlay(
    /* [in] */ IIHdmiControlCallback* callback)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        AssertRunOnServiceThread();
        HdmiCecLocalDevicePlayback source = Playback();
        if (source == NULL) {
            Slogger::W(TAG, "Local playback device not available");
            InvokeCallback(callback, IHdmiControlManager::RESULT_SOURCE_NOT_AVAILABLE);
            return NOERROR;
        }
        source->OneTouchPlay(callback);

#endif
}

ECode HdmiControlService::QueryDisplayStatus(
    /* [in] */ IIHdmiControlCallback* callback)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        AssertRunOnServiceThread();
        HdmiCecLocalDevicePlayback source = Playback();
        if (source == NULL) {
            Slogger::W(TAG, "Local playback device not available");
            InvokeCallback(callback, IHdmiControlManager::RESULT_SOURCE_NOT_AVAILABLE);
            return NOERROR;
        }
        source->QueryDisplayStatus(callback);

#endif
}

ECode HdmiControlService::AddHotplugEventListener(
    /* [in] */ IIHdmiHotplugEventListener* listener)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        HotplugEventListenerRecord record = new HotplugEventListenerRecord(listener);
        try {
            listener->AsBinder()->LinkToDeath(record, 0);
        } catch (RemoteException e) {
            Slogger::W(TAG, "Listener already died");
            return NOERROR;
        }
        synchronized(mLock) {
            mHotplugEventListenerRecords->Add(record);
        }

#endif
}

ECode HdmiControlService::RemoveHotplugEventListener(
    /* [in] */ IIHdmiHotplugEventListener* listener)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        synchronized(mLock) {
            for (HotplugEventListenerRecord record : mHotplugEventListenerRecords) {
                if (record->mMListener->AsBinder() == listener->AsBinder()) {
                    listener->AsBinder()->UnlinkToDeath(record, 0);
                    mHotplugEventListenerRecords->Remove(record);
                    break;
                }
            }
        }

#endif
}

ECode HdmiControlService::AddDeviceEventListener(
    /* [in] */ IIHdmiDeviceEventListener* listener)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        DeviceEventListenerRecord record = new DeviceEventListenerRecord(listener);
        try {
            listener->AsBinder()->LinkToDeath(record, 0);
        } catch (RemoteException e) {
            Slogger::W(TAG, "Listener already died");
            return NOERROR;
        }
        synchronized(mLock) {
            mDeviceEventListenerRecords->Add(record);
        }

#endif
}

ECode HdmiControlService::InvokeDeviceEventListeners(
    /* [in] */ IHdmiDeviceInfo* device,
    /* [in] */ Int32 status)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        synchronized(mLock) {
            for (DeviceEventListenerRecord record : mDeviceEventListenerRecords) {
                try {
                    record->mMListener->OnStatusChanged(device, status);
                } catch (RemoteException e) {
                    Slogger::E(TAG, "Failed to report device event:" + e);
                }
            }
        }

#endif
}

ECode HdmiControlService::AddSystemAudioModeChangeListner(
    /* [in] */ IIHdmiSystemAudioModeChangeListener* listener)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        SystemAudioModeChangeListenerRecord record = new SystemAudioModeChangeListenerRecord(
                listener);
        try {
            listener->AsBinder()->LinkToDeath(record, 0);
        } catch (RemoteException e) {
            Slogger::W(TAG, "Listener already died");
            return NOERROR;
        }
        synchronized(mLock) {
            mSystemAudioModeChangeListenerRecords->Add(record);
        }

#endif
}

ECode HdmiControlService::RemoveSystemAudioModeChangeListener(
    /* [in] */ IIHdmiSystemAudioModeChangeListener* listener)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        synchronized(mLock) {
            for (SystemAudioModeChangeListenerRecord record :
                    mSystemAudioModeChangeListenerRecords) {
                if (record->mMListener->AsBinder() == listener) {
                    listener->AsBinder()->UnlinkToDeath(record, 0);
                    mSystemAudioModeChangeListenerRecords->Remove(record);
                    break;
                }
            }
        }

#endif
}

ECode HdmiControlService::SetInputChangeListener(
    /* [in] */ IIHdmiInputChangeListener* listener)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        synchronized(mLock) {
            mInputChangeListenerRecord = new InputChangeListenerRecord(listener);
            try {
                listener->AsBinder()->LinkToDeath(mInputChangeListenerRecord, 0);
            } catch (RemoteException e) {
                Slogger::W(TAG, "Listener already died");
                return NOERROR;
            }
        }

#endif
}

ECode HdmiControlService::InvokeInputChangeListener(
    /* [in] */ IHdmiDeviceInfo* info)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        synchronized(mLock) {
            if (mInputChangeListenerRecord != NULL) {
                try {
                    mInputChangeListenerRecord->mMListener->OnChanged(info);
                } catch (RemoteException e) {
                    Slogger::W(TAG, "Exception thrown by IHdmiInputChangeListener: " + e);
                }
            }
        }

#endif
}

ECode HdmiControlService::SetHdmiRecordListener(
    /* [in] */ IIHdmiRecordListener* listener)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        synchronized(mLock) {
            mRecordListenerRecord = new HdmiRecordListenerRecord(listener);
            try {
                listener->AsBinder()->LinkToDeath(mRecordListenerRecord, 0);
            } catch (RemoteException e) {
                Slogger::W(TAG, "Listener already died.", e);
            }
        }

#endif
}

ECode HdmiControlService::InvokeRecordRequestListener(
    /* [in] */ Int32 recorderAddress,
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        synchronized(mLock) {
            if (mRecordListenerRecord != NULL) {
                try {
                    return mRecordListenerRecord->mMListener->GetOneTouchRecordSource(recorderAddress);
                } catch (RemoteException e) {
                    Slogger::W(TAG, "Failed to start record.", e);
                }
            }
            return EmptyArray::BYTE;
        }

#endif
}

ECode HdmiControlService::InvokeOneTouchRecordResult(
    /* [in] */ Int32 result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        synchronized(mLock) {
            if (mRecordListenerRecord != NULL) {
                try {
                    mRecordListenerRecord->mMListener->OnOneTouchRecordResult(result);
                } catch (RemoteException e) {
                    Slogger::W(TAG, "Failed to call onOneTouchRecordResult.", e);
                }
            }
        }

#endif
}

ECode HdmiControlService::InvokeTimerRecordingResult(
    /* [in] */ Int32 result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        synchronized(mLock) {
            if (mRecordListenerRecord != NULL) {
                try {
                    mRecordListenerRecord->mMListener->OnTimerRecordingResult(result);
                } catch (RemoteException e) {
                    Slogger::W(TAG, "Failed to call onTimerRecordingResult.", e);
                }
            }
        }

#endif
}

ECode HdmiControlService::InvokeClearTimerRecordingResult(
    /* [in] */ Int32 result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        synchronized(mLock) {
            if (mRecordListenerRecord != NULL) {
                try {
                    mRecordListenerRecord->mMListener->OnClearTimerRecordingResult(result);
                } catch (RemoteException e) {
                    Slogger::W(TAG, "Failed to call onClearTimerRecordingResult.", e);
                }
            }
        }

#endif
}

ECode HdmiControlService::InvokeCallback(
    /* [in] */ IIHdmiControlCallback* callback,
    /* [in] */ Int32 result)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        try {
            callback->OnComplete(result);
        } catch (RemoteException e) {
            Slogger::E(TAG, "Invoking callback failed:" + e);
        }

#endif
}

ECode HdmiControlService::InvokeSystemAudioModeChangeLocked(
    /* [in] */ IIHdmiSystemAudioModeChangeListener* listener,
    /* [in] */ Boolean enabled)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        try {
            listener->OnStatusChanged(enabled);
        } catch (RemoteException e) {
            Slogger::E(TAG, "Invoking callback failed:" + e);
        }

#endif
}

ECode HdmiControlService::AnnounceHotplugEvent(
    /* [in] */ Int32 portId,
    /* [in] */ Boolean connected)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        HdmiHotplugEvent event = new HdmiHotplugEvent(portId, connected);
        synchronized(mLock) {
            for (HotplugEventListenerRecord record : mHotplugEventListenerRecords) {
                InvokeHotplugEventListenerLocked(record->mMListener, event);
            }
        }

#endif
}

ECode HdmiControlService::InvokeHotplugEventListenerLocked(
    /* [in] */ IIHdmiHotplugEventListener* listener,
    /* [in] */ IHdmiHotplugEvent* event)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        try {
            listener->OnReceived(event);
        } catch (RemoteException e) {
            Slogger::E(TAG, "Failed to report hotplug event:" + event->ToString(), e);
        }

#endif
}

ECode HdmiControlService::Tv(
    /* [out] */ HdmiCecLocalDeviceTv** result)
{
    VALIDATE_NOT_NULL(result)

    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return (HdmiCecLocalDeviceTv) mCecController->GetLocalDevice(IHdmiDeviceInfo::DEVICE_TV);

#endif
}

ECode HdmiControlService::IsTvDevice(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return Tv() != NULL;

#endif
}

ECode HdmiControlService::Playback(
    /* [out] */ HdmiCecLocalDevicePlayback** result)
{
    VALIDATE_NOT_NULL(result)

    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return (HdmiCecLocalDevicePlayback)
                mCecController->GetLocalDevice(IHdmiDeviceInfo::DEVICE_PLAYBACK);

#endif
}

ECode HdmiControlService::GetAudioManager(
    /* [out] */ IAudioManager** result)
{
    VALIDATE_NOT_NULL(result)

    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        return (AudioManager) GetContext()->GetSystemService(Context::AUDIO_SERVICE);

#endif
}

ECode HdmiControlService::IsControlEnabled(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        synchronized(mLock) {
            return mHdmiControlEnabled;
        }

#endif
}

ECode HdmiControlService::GetPowerStatus(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)

    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        AssertRunOnServiceThread();
        return mPowerStatus;

#endif
}

ECode HdmiControlService::IsPowerOnOrTransient(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        AssertRunOnServiceThread();
        return mPowerStatus == IHdmiControlManager::POWER_STATUS_ON
                || mPowerStatus == IHdmiControlManager::POWER_STATUS_TRANSIENT_TO_ON;

#endif
}

ECode HdmiControlService::IsPowerStandbyOrTransient(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        AssertRunOnServiceThread();
        return mPowerStatus == IHdmiControlManager::POWER_STATUS_STANDBY
                || mPowerStatus == IHdmiControlManager::POWER_STATUS_TRANSIENT_TO_STANDBY;

#endif
}

ECode HdmiControlService::IsPowerStandby(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        AssertRunOnServiceThread();
        return mPowerStatus == IHdmiControlManager::POWER_STATUS_STANDBY;

#endif
}

ECode HdmiControlService::WakeUp()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        AssertRunOnServiceThread();
        mWakeUpMessageReceived = TRUE;
        PowerManager pm = (PowerManager) GetContext()->GetSystemService(Context::POWER_SERVICE);
        pm->WakeUp(SystemClock::GetUptimeMillis());
        // PowerManger will send the broadcast Intent::ACTION_SCREEN_ON and after this gets
        // the intent, the sequence will continue at onWakeUp().

#endif
}

ECode HdmiControlService::Standby()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        AssertRunOnServiceThread();
        mStandbyMessageReceived = TRUE;
        PowerManager pm = (PowerManager) GetContext()->GetSystemService(Context::POWER_SERVICE);
        pm->GoToSleep(SystemClock::GetUptimeMillis(), PowerManager::GO_TO_SLEEP_REASON_HDMI, 0);
        // PowerManger will send the broadcast Intent.ACTION_SCREEN_OFF and after this gets
        // the intent, the sequence will continue at onStandby().

#endif
}

ECode HdmiControlService::OnWakeUp()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        AssertRunOnServiceThread();
        mPowerStatus = IHdmiControlManager::POWER_STATUS_TRANSIENT_TO_ON;
        if (mCecController != NULL) {
            if (mHdmiControlEnabled) {
                Int32 startReason = INITIATED_BY_SCREEN_ON;
                if (mWakeUpMessageReceived) {
                    startReason = INITIATED_BY_WAKE_UP_MESSAGE;
                }
                InitializeCec(startReason);
            }
        } else {
            Slogger::I(TAG, "Device does not support HDMI-CEC.");
        }
        // TODO: Initialize MHL local devices.

#endif
}

ECode HdmiControlService::OnStandby()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        AssertRunOnServiceThread();
        mPowerStatus = IHdmiControlManager::POWER_STATUS_TRANSIENT_TO_STANDBY;
        List<HdmiCecLocalDevice> devices = GetAllLocalDevices();
        DisableDevices(new PendingActionClearedCallback() {
            @Override
            public void OnCleared(IHdmiCecLocalDevice* device) {
                Slogger::V(TAG, "On standby-action cleared:" + device->mMDeviceType);
                devices->Remove(device);
                Boolean isEmpty;
                devices->IsEmpty(&isEmpty);
                if (isEmpty) {
                    OnStandbyCompleted();
                    // We will not clear local devices here, since some OEM/SOC will keep passing
                    // the received packets until the application processor enters to the sleep
                    // actually.
                }
            }
        });

#endif
}

ECode HdmiControlService::OnLanguageChanged(
    /* [in] */ const String& language)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        AssertRunOnServiceThread();
        mLanguage = language;
        if (IsTvDevice()) {
            AutoPtr<HdmiCecLocalDeviceTv> tv;
            Tv((HdmiCecLocalDeviceTv**)&tv);
            Boolean isBroadcastMenuLanguage;
            tv->BroadcastMenuLanguage(language, &isBroadcastMenuLanguage);
        }

#endif
}

ECode HdmiControlService::GetLanguage(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result)

    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        AssertRunOnServiceThread();
        return mLanguage;

#endif
}

ECode HdmiControlService::DisableDevices(
    /* [in] */ IHdmiCecLocalDevicePendingActionClearedCallback* callback)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        if (mCecController != NULL) {
            for_each (it : mCecController->GetLocalDeviceList()) {
                AutoPtr<IHdmiCecLocalDevice> device = it->GetNext();
                device->DisableDevice(mStandbyMessageReceived, callback);
            }
            if (IsTvDevice()) {
                UnregisterSettingsObserver();
            }
        }
        mMhlController->ClearAllLocalDevices();

#endif
}

ECode HdmiControlService::ClearLocalDevices()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        AssertRunOnServiceThread();
        if (mCecController == NULL) {
            return NOERROR;
        }
        mCecController->ClearLogicalAddress();
        mCecController->ClearLocalDevices();

#endif
}

ECode HdmiControlService::OnStandbyCompleted()
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        AssertRunOnServiceThread();
        Slogger::V(TAG, "onStandbyCompleted");
        if (mPowerStatus != IHdmiControlManager::POWER_STATUS_TRANSIENT_TO_STANDBY) {
            return NOERROR;
        }
        mPowerStatus = IHdmiControlManager::POWER_STATUS_STANDBY;
        FOR_EACH(it : mCecController->GetLocalDeviceList()) {
            AutoPtr<IHdmiCecLocalDevice> device = it->GetNext();
            device->OnStandby(mStandbyMessageReceived);
        }
        mStandbyMessageReceived = FALSE;
        mCecController->SetOption(OPTION_CEC_SERVICE_CONTROL, DISABLED);

#endif
}

ECode HdmiControlService::AddVendorCommandListener(
    /* [in] */ IIHdmiVendorCommandListener* listener,
    /* [in] */ Int32 deviceType)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        VendorCommandListenerRecord record = new VendorCommandListenerRecord(listener, deviceType);
        try {
            listener->AsBinder()->LinkToDeath(record, 0);
        } catch (RemoteException e) {
            Slogger::W(TAG, "Listener already died");
            return NOERROR;
        }
        synchronized(mLock) {
            mVendorCommandListenerRecords->Add(record);
        }

#endif
}

ECode HdmiControlService::InvokeVendorCommandListeners(
    /* [in] */ Int32 deviceType,
    /* [in] */ Int32 srcAddress,
    /* [in] */ ArrayOf<Byte>* params,
    /* [in] */ Boolean hasVendorId,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        synchronized(mLock) {
            Boolean isEmpty;
            mVendorCommandListenerRecords->IsEmpty(&isEmpty);
            if (isEmpty) {
                *result = FALSE;
                return NOERROR;
            }
            for (VendorCommandListenerRecord record : mVendorCommandListenerRecords) {
                if (record->mMDeviceType != deviceType) {
                    continue;
                }
                try {
                    record->mMListener->OnReceived(srcAddress, params, hasVendorId);
                } catch (RemoteException e) {
                    Slogger::E(TAG, "Failed to notify vendor command reception", e);
                }
            }
            *result = TRUE;
            return NOERROR;
        }

#endif
}

ECode HdmiControlService::AddHdmiMhlVendorCommandListener(
    /* [in] */ IIHdmiMhlVendorCommandListener* listener)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        HdmiMhlVendorCommandListenerRecord record =
                new HdmiMhlVendorCommandListenerRecord(listener);
        try {
            listener->AsBinder()->LinkToDeath(record, 0);
        } catch (RemoteException e) {
            Slogger::W(TAG, "Listener already died.");
            return NOERROR;
        }
        synchronized(mLock) {
            mMhlVendorCommandListenerRecords->Add(record);
        }

#endif
}

ECode HdmiControlService::InvokeMhlVendorCommandListeners(
    /* [in] */ Int32 portId,
    /* [in] */ Int32 offest,
    /* [in] */ Int32 length,
    /* [in] */ ArrayOf<Byte>* data)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        synchronized(mLock) {
            for (HdmiMhlVendorCommandListenerRecord record : mMhlVendorCommandListenerRecords) {
                try {
                    record->mMListener->OnReceived(portId, offest, length, data);
                } catch (RemoteException e) {
                    Slogger::E(TAG, "Failed to notify MHL vendor command", e);
                }
            }
        }

#endif
}

ECode HdmiControlService::IsProhibitMode(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        synchronized(mLock) {
            return mProhibitMode;
        }

#endif
}

ECode HdmiControlService::SetProhibitMode(
    /* [in] */ Boolean enabled)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        synchronized(mLock) {
            mProhibitMode = enabled;
        }

#endif
}

ECode HdmiControlService::SetCecOption(
    /* [in] */ Int32 key,
    /* [in] */ Int32 value)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        AssertRunOnServiceThread();
        mCecController->SetOption(key, value);

#endif
}

ECode HdmiControlService::SetControlEnabled(
    /* [in] */ Boolean enabled)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        AssertRunOnServiceThread();
        Int32 value = ToInt32(enabled);
        mCecController->SetOption(OPTION_CEC_ENABLE, value);
        mMhlController->SetOption(OPTION_MHL_ENABLE, value);
        synchronized(mLock) {
            mHdmiControlEnabled = enabled;
        }
        if (enabled) {
            InitializeCec(INITIATED_BY_ENABLE_CEC);
        } else {
            DisableDevices(new PendingActionClearedCallback() {
                @Override
                public void OnCleared(IHdmiCecLocalDevice* device) {
                    AssertRunOnServiceThread();
                    ClearLocalDevices();
                }
            });
        }

#endif
}

ECode HdmiControlService::SetActivePortId(
    /* [in] */ Int32 portId)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        AssertRunOnServiceThread();
        mActivePortId = portId;
        // Resets last input for MHL, which stays valid only after the MHL device was selected,
        // and no further switching is done.
        SetLastInputForMhl(Constants::INVALID_PORT_ID);

#endif
}

ECode HdmiControlService::SetLastInputForMhl(
    /* [in] */ Int32 portId)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        AssertRunOnServiceThread();
        mLastInputMhl = portId;

#endif
}

ECode HdmiControlService::GetLastInputForMhl(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)

    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        AssertRunOnServiceThread();
        return mLastInputMhl;

#endif
}

ECode HdmiControlService::ChangeInputForMhl(
    /* [in] */ Int32 portId,
    /* [in] */ Boolean contentOn)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        AssertRunOnServiceThread();
        AutoPtr<HdmiCecLocalDeviceTv> tv;
        Tv((HdmiCecLocalDeviceTv**)&tv);
        Int32 activePortId;
        tv->GetActivePortId(&activePortId);
        const Int32 lastInput = contentOn ? activePortId : Constants::INVALID_PORT_ID;
        tv->DoManualPortSwitching(portId, new IHdmiControlCallback.Stub() {
            @Override
            public void OnComplete(Int32 result) throws RemoteException {
                // Keep the last input to switch back later when RAP[ContentOff] is received.
                // This effectively sets the port to invalid one if the switching is for
                // RAP[ContentOff].
                SetLastInputForMhl(lastInput);
            }
        });
        // MHL device is always directly connected to the port. Update the active port ID to avoid
        // unnecessary post-routing control task.
        tv->SetActivePortId(portId);
        // The port is either the MHL-enabled port where the mobile device is connected, or
        // the last port to go back to when turnoff command is received. Note that the last port
        // may not be the MHL-enabled one. In this case the device info to be passed to
        // input change listener should be the one describing the corresponding HDMI port.
        HdmiMhlLocalDeviceStub device = mMhlController->GetLocalDevice(portId);
        AutoPtr<IInterface> obj;
        mPortDeviceMap->Get(portId, (IInterface**)&obj);
        HdmiDeviceInfo info = (device != NULL && device->GetInfo() != NULL)
                ? device->GetInfo()
                : I::Probe(obj);
        InvokeInputChangeListener(info);

#endif
}

ECode HdmiControlService::SetMhlInputChangeEnabled(
    /* [in] */ Boolean enabled)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
       mMhlController->SetOption(OPTION_MHL_INPUT_SWITCHING, ToInt32(enabled));
        synchronized(mLock) {
            mMhlInputChangeEnabled = enabled;
        }

#endif
}

ECode HdmiControlService::IsMhlInputChangeEnabled(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        synchronized(mLock) {
            return mMhlInputChangeEnabled;
        }

#endif
}

ECode HdmiControlService::DisplayOsd(
    /* [in] */ Int32 messageId)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        AssertRunOnServiceThread();
        Intent intent = new Intent(IHdmiControlManager::ACTION_OSD_MESSAGE);
        intent->PutExtra(IHdmiControlManager::EXTRA_MESSAGE_ID, messageId);
        GetContext()->SendBroadcastAsUser(intent, UserHandle::ALL,
                HdmiControlService::PERMISSION);

#endif
}

ECode HdmiControlService::DisplayOsd(
    /* [in] */ Int32 messageId,
    /* [in] */ Int32 extra)
{
    return E_NOT_IMPLEMENTED;
#if 0 // TODO: Translate codes below
        AssertRunOnServiceThread();
        Intent intent = new Intent(IHdmiControlManager::ACTION_OSD_MESSAGE);
        intent->PutExtra(IHdmiControlManager::EXTRA_MESSAGE_ID, messageId);
        intent->PutExtra(IHdmiControlManager::EXTRA_MESSAGE_EXTRAM_PARAM1, extra);
        GetContext()->SendBroadcastAsUser(intent, UserHandle::ALL,
                HdmiControlService::PERMISSION);

#endif
}

} // namespace Hdmi
} // namespace Server
} // namespace Droid
} // namespace Elastos
