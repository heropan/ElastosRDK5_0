
#include "media/CAudioManager.h"
#include "R.h"
#include <elastos/utility/etl/List.h>
#include <elastos/utility/logging/Logger.h>
#include <elastos/core/StringUtils.h>
#include "os/CBinder.h"
#include "os/SystemClock.h"
#include "os/CLooperHelper.h"
#include "os/CServiceManager.h"
#include "os/SystemProperties.h"
#include "content/CIntent.h"
#include "provider/Settings.h"
#include "media/CAudioSystemHelper.h"
#include "media/CMediaRecorder.h"
#include "media/CMediaPlayer.h"
#include "media/CAudioManagerAudioFocusDispatcher.h"

using Elastos::Droid::Os::CBinder;
using Elastos::Droid::Os::ILooper;
using Elastos::Droid::Os::ILooperHelper;
using Elastos::Droid::Os::CLooperHelper;
using Elastos::Droid::Os::IHandler;
using Elastos::Droid::Os::IServiceManager;
using Elastos::Droid::Os::CServiceManager;
using Elastos::Droid::Os::SystemClock;
using Elastos::Droid::Os::SystemProperties;
using Elastos::Droid::View::IKeyEvent;
using Elastos::Droid::Widget::IVolumePanel;
using Elastos::Droid::View::IDisplayManagerAw;
using Elastos::Droid::Provider::Settings;
using Elastos::Droid::Provider::ISettingsSystem;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IContentResolver;
using Elastos::Droid::Content::CIntent;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::App::IPendingIntentHelper;
using Elastos::Utility::Logging::Logger;
using Elastos::Core::StringUtils;
using Elastos::Core::ICharSequence;
using Elastos::Core::CString;
using Elastos::Utility::Etl::List;

namespace Elastos {
namespace Droid {
namespace Media {

const Int32 CAudioManager::DEFAULT_STREAM_VOLUME[] = {
    4,  // STREAM_VOICE_CALL
    7,  // STREAM_SYSTEM
    5,  // STREAM_RING
    11, // STREAM_MUSIC
    6,  // STREAM_ALARM
    5,  // STREAM_NOTIFICATION
    7,  // STREAM_BLUETOOTH_SCO
    7,  // STREAM_SYSTEM_ENFORCED
    11, // STREAM_DTMF
    11  // STREAM_TTS
};

const String CAudioManager::TAG("CAudioManager");
const Boolean CAudioManager::DEBUG = FALSE;

const Int32 CAudioManager::RINGER_MODE_MAX = RINGER_MODE_NORMAL;
AutoPtr<IIAudioService> CAudioManager::sService;

const String CAudioManager::hdmiExpected  = String("audio.hdmi.expected");
const String CAudioManager::hdmiAvailable  = String("audio.hdmi.available");

//================================================================================
//                      CAudioManager::FocusEventHandlerDelegate::MyHandler
//================================================================================
ECode CAudioManager::FocusEventHandlerDelegate::MyHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    Int32 what;
    msg->GetWhat(&what);
    AutoPtr<IInterface> obj;
    msg->GetObj((IInterface**)&obj);
    ICharSequence* seq = ICharSequence::Probe(obj);
    String info;
    seq->ToString(&info);

    AutoPtr<IOnAudioFocusChangeListener> listener;
    {
        AutoLock lock(mHost->mOwner->mFocusListenerLock);
        listener = mHost->mOwner->FindFocusListener(info);
    }
    if (listener != NULL) {
        listener->OnAudioFocusChange(what);
    }
    return NOERROR;
}

//================================================================================
//                      CAudioManager::FocusEventHandlerDelegate
//================================================================================
CAudioManager::FocusEventHandlerDelegate::FocusEventHandlerDelegate(
    /* [in] */ CAudioManager* owner)
    : mOwner(owner)
{
    AutoPtr<ILooper> looper;
    AutoPtr<ILooperHelper> looperHelper;
    CLooperHelper::AcquireSingleton((ILooperHelper**)&looperHelper);
    looperHelper->MyLooper((ILooper**)&looper);
    if (looper == NULL) {
        looperHelper->GetMainLooper((ILooper**)&looper);
    }

    if (looper != NULL) {
        // implement the event handler delegate to receive audio focus events
        mHandler = new MyHandler(looper, this);
    }
    else {
        mHandler = NULL;
    }
}

AutoPtr<IHandler> CAudioManager::FocusEventHandlerDelegate::GetHandler()
{
    return mHandler;
}

//================================================================================
//                      CAudioManager::CAudioManager
//================================================================================
CAudioManager::CAudioManager()
    : mVolumeKeyUpTime(0)
    , mUseMasterVolume(FALSE)
    , mUseVolumeKeySounds(FALSE)
{
    CBinder::New((IBinder**)&mICallBack);
    mAudioFocusEventHandlerDelegate = new FocusEventHandlerDelegate(this);
    CAudioManagerAudioFocusDispatcher::New(THIS_PROBE(IAudioManager), (IIAudioFocusDispatcher**)&mAudioFocusDispatcher);
}

ECode CAudioManager::constructor(
    /* [in] */ IContext* context)
{
    mContext = context;
    AutoPtr<IResources> resources;
    mContext->GetResources((IResources**)&resources);
    resources->GetBoolean(R::bool_::config_useMasterVolume, &mUseMasterVolume);
    resources->GetBoolean(R::bool_::config_useVolumeKeySounds, &mUseVolumeKeySounds);
    return NOERROR;
}

AutoPtr<IIAudioService> CAudioManager::GetService()
{
    if (sService != NULL) {
        return sService;
    }

    //todo: for java compatible
    //sService = (IAudioService)ElServiceManager.getService(Context.AUDIO_SERVICE);
    AutoPtr<IServiceManager> serviceManager;
    CServiceManager::AcquireSingleton((IServiceManager**)&serviceManager);
    AutoPtr<IInterface> obj;
    serviceManager->GetService(IContext::AUDIO_SERVICE, (IInterface**)&obj);
    sService = IIAudioService::Probe(obj.Get());

    if (sService == NULL) {
        Logger::E(TAG, "AudioService is not ready!");
    }
    return sService;
}

ECode CAudioManager::PreDispatchKeyEvent(
    /* [in] */ IKeyEvent* event,
    /* [in] */ Int32 stream)
{
    /*
     * If the user hits another key within the play sound delay, then
     * cancel the sound
     */
    Int32 keyCode;
    event->GetKeyCode(&keyCode);
    if (keyCode != IKeyEvent::KEYCODE_VOLUME_DOWN
        && keyCode != IKeyEvent::KEYCODE_VOLUME_UP
        && keyCode != IKeyEvent::KEYCODE_VOLUME_MUTE
        && mVolumeKeyUpTime + IVolumePanel::PLAY_SOUND_DELAY
            > SystemClock::GetUptimeMillis()) {
        /*
         * The user has hit another key during the delay (e.g., 300ms)
         * since the last volume key up, so cancel any sounds.
         */
        if (mUseMasterVolume) {
            AdjustMasterVolume(ADJUST_SAME, FLAG_REMOVE_SOUND_AND_VIBRATE);
        }
        else {
            AdjustSuggestedStreamVolume(ADJUST_SAME, stream, FLAG_REMOVE_SOUND_AND_VIBRATE);
        }
    }
    return NOERROR;
}

ECode CAudioManager::HandleKeyDown(
    /* [in] */ IKeyEvent* event,
    /* [in] */ Int32 stream)
{
    Int32 keyCode;
    event->GetKeyCode(&keyCode);
    switch (keyCode) {
        case IKeyEvent::KEYCODE_VOLUME_UP:
        case IKeyEvent::KEYCODE_VOLUME_DOWN:
        {
            /*
             * Adjust the volume in on key down since it is more
             * responsive to the user.
             */
            Int32 flags = FLAG_SHOW_UI | FLAG_VIBRATE;
            if (mUseMasterVolume) {
                AdjustMasterVolume(keyCode == IKeyEvent::KEYCODE_VOLUME_UP
                    ? ADJUST_RAISE : ADJUST_LOWER, flags);
            }
            else {
                AdjustSuggestedStreamVolume(keyCode == IKeyEvent::KEYCODE_VOLUME_UP
                    ? ADJUST_RAISE : ADJUST_LOWER, stream, flags);
            }
            break;
        }
        case IKeyEvent::KEYCODE_VOLUME_MUTE:
        {
            Int32 value;
            event->GetRepeatCount(&value);
            if (value == 0) {
                if (mUseMasterVolume) {
                    Boolean result;
                    IsMasterMute(&result);
                    SetMasterMute(!result);
                } else {
                    // TODO: Actually handle MUTE.
                }
            }
            break;
        }
    }
    return NOERROR;
}

ECode CAudioManager::HandleKeyUp(
    /* [in] */ IKeyEvent* event,
    /* [in] */ Int32 stream)
{
    Int32 keyCode;
    event->GetKeyCode(&keyCode);
    switch (keyCode) {
        case IKeyEvent::KEYCODE_VOLUME_UP:
        case IKeyEvent::KEYCODE_VOLUME_DOWN:
        {
            /*
             * Play a sound. This is done on key up since we don't want the
             * sound to play when a user holds down volume down to mute.
             */
            if (mUseVolumeKeySounds) {
                if (mUseMasterVolume) {
                    AdjustMasterVolume(ADJUST_SAME, FLAG_PLAY_SOUND);
                }
                else {
                    Int32 flags;
                    flags = FLAG_PLAY_SOUND;
                    AdjustSuggestedStreamVolume(ADJUST_SAME, stream, flags);
                }
            }
            mVolumeKeyUpTime = SystemClock::GetUptimeMillis();
            break;
        }
    }
    return NOERROR;
}

ECode CAudioManager::AdjustStreamVolume(
    /* [in] */ Int32 streamType,
    /* [in] */ Int32 direction,
    /* [in] */ Int32 flags)
{
    AutoPtr<IIAudioService> service = GetService();
    if (!service) return E_REMOTE_EXCEPTION;

    ECode ec = NOERROR;
    if (mUseMasterVolume) {
        ec = service->AdjustMasterVolume(direction, flags);
    }
    else {
        ec = service->AdjustStreamVolume(streamType, direction, flags);
    }
    if (FAILED(ec)) {
        Logger::E(TAG, "Dead object in AdjustStreamVolume");
        return E_REMOTE_EXCEPTION;
    }

    return NOERROR;
}

ECode CAudioManager::AdjustVolume(
    /* [in] */ Int32 direction,
    /* [in] */ Int32 flags)
{
    AutoPtr<IIAudioService> service = GetService();
    if (!service) return E_REMOTE_EXCEPTION;

    ECode ec = NOERROR;
    if (mUseMasterVolume) {
        ec = service->AdjustMasterVolume(direction, flags);
    }
    else {
        ec = service->AdjustVolume(direction, flags);
    }

    if (FAILED(ec)) {
        Logger::E(TAG, "Dead object in AdjustVolume");
        return E_REMOTE_EXCEPTION;
    }

    return NOERROR;
}

ECode CAudioManager::AdjustSuggestedStreamVolume(
    /* [in] */ Int32 direction,
    /* [in] */ Int32 suggestedStreamType,
    /* [in] */ Int32 flags)
{
    AutoPtr<IIAudioService> service = GetService();
    if (!service) return E_REMOTE_EXCEPTION;

    ECode ec = NOERROR;
    if (mUseMasterVolume) {
        ec = service->AdjustMasterVolume(direction, flags);
    }
    else {
        ec = service->AdjustSuggestedStreamVolume(
            direction, suggestedStreamType, flags);
    }

    if (FAILED(ec)) {
        Logger::E(TAG, "Dead object in AdjustSuggestedStreamVolume");
        return E_REMOTE_EXCEPTION;
    }
    return NOERROR;
}

ECode CAudioManager::AdjustMasterVolume(
    /* [in] */ Int32 steps,
    /* [in] */ Int32 flags)
{
    AutoPtr<IIAudioService> service = GetService();
    if (!service) return E_REMOTE_EXCEPTION;

    ECode ec = NOERROR;
    ec = service->AdjustMasterVolume(steps, flags);
    if (FAILED(ec)) {
        Logger::E(TAG, "Dead object in AdjustMasterVolume");
        return E_REMOTE_EXCEPTION;
    }

    return NOERROR;
}

ECode CAudioManager::GetRingerMode(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = RINGER_MODE_NORMAL;

    AutoPtr<IIAudioService> service = GetService();
    if (!service) return E_REMOTE_EXCEPTION;

    ECode ec = service->GetRingerMode(result);
    if (FAILED(ec)) {
        Logger::E(TAG, "Dead object in GetRingerMode");
        *result = RINGER_MODE_NORMAL;
        return E_REMOTE_EXCEPTION;
    }

    return NOERROR;
}

ECode CAudioManager::IsValidRingerMode(
    /* [in] */ Int32 ringerMode,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    if (ringerMode < 0 || ringerMode > RINGER_MODE_MAX) {
        *result = FALSE;
        return NOERROR;
    }
    *result = TRUE;
    return NOERROR;
}

ECode CAudioManager::GetStreamMaxVolume(
    /* [in] */ Int32 streamType,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = 0;

    AutoPtr<IIAudioService> service = GetService();
    if (!service) return E_REMOTE_EXCEPTION;

    ECode ec = NOERROR;
    if (mUseMasterVolume) {
        ec = service->GetMasterMaxVolume(result);
    }
    else {
        ec = service->GetStreamMaxVolume(streamType, result);
    }

    if (FAILED(ec)) {
        Logger::E(TAG, "Dead object in GetStreamMaxVolume");
        return E_REMOTE_EXCEPTION;
    }

    return NOERROR;
}

ECode CAudioManager::GetStreamVolume(
    /* [in] */ Int32 streamType,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = 0;

    AutoPtr<IIAudioService> service = GetService();
    if (!service) return E_REMOTE_EXCEPTION;

    ECode ec = NOERROR;
    if (mUseMasterVolume) {
        ec = service->GetMasterVolume(result);
    }
    else {
        ec = service->GetStreamVolume(streamType, result);
    }

    if (FAILED(ec)) {
        Logger::E(TAG, "Dead object in GetStreamVolume");
        return E_REMOTE_EXCEPTION;
    }

    return NOERROR;
}

ECode CAudioManager::GetLastAudibleStreamVolume(
    /* [in] */ Int32 streamType,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = 0;

    AutoPtr<IIAudioService> service = GetService();
    if (!service) return E_REMOTE_EXCEPTION;

    ECode ec = NOERROR;
    if (mUseMasterVolume) {
        ec = service->GetLastAudibleMasterVolume(result);
    }
    else {
        ec = service->GetLastAudibleStreamVolume(streamType, result);
    }

    if (FAILED(ec)) {
        Logger::E(TAG, "Dead object in GetLastAudibleStreamVolume");
        return E_REMOTE_EXCEPTION;
    }
    return NOERROR;
}

ECode CAudioManager::GetMasterStreamType(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = STREAM_RING;

    AutoPtr<IIAudioService> service = GetService();
    if (!service) return E_REMOTE_EXCEPTION;

    ECode ec = service->GetMasterStreamType(result);
    if (FAILED(ec)) {
        Logger::E(TAG, "Dead object in GetMasterStreamType");
        *result = STREAM_RING;
        return E_REMOTE_EXCEPTION;
    }

    return NOERROR;
}

ECode CAudioManager::SetRingerMode(
    /* [in] */ Int32 ringerMode)
{
    Boolean result;
    IsValidRingerMode(ringerMode, &result);
    if (!result) return NOERROR;

    AutoPtr<IIAudioService> service = GetService();
    if (!service) return E_REMOTE_EXCEPTION;

    ECode ec = service->SetRingerMode(ringerMode);
    if (FAILED(ec)) {
        Logger::E(TAG, "Dead object in SetRingerMode");
        return E_REMOTE_EXCEPTION;
    }
    return NOERROR;
}

ECode CAudioManager::SetStreamVolume(
    /* [in] */ Int32 streamType,
    /* [in] */ Int32 index,
    /* [in] */ Int32 flags)
{
    AutoPtr<IIAudioService> service = GetService();
    if (!service) return E_REMOTE_EXCEPTION;

    ECode ec = NOERROR;
    if (mUseMasterVolume) {
        ec = service->SetMasterVolume(index, flags);
    }
    else {
        ec = service->SetStreamVolume(streamType, index, flags);
    }

    if (FAILED(ec)) {
        Logger::E(TAG, "Dead object in SetStreamVolume");
        return E_REMOTE_EXCEPTION;
    }
    return NOERROR;
}

ECode CAudioManager::GetMasterMaxVolume(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = 0;

    AutoPtr<IIAudioService> service = GetService();
    if (!service) return E_REMOTE_EXCEPTION;

    ECode ec = service->GetMasterMaxVolume(result);
    if (FAILED(ec)) {
        Logger::E(TAG, "Dead object in GetMasterMaxVolume");
        return E_REMOTE_EXCEPTION;
    }

    return NOERROR;
}

ECode CAudioManager::GetMasterVolume(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = 0;

    AutoPtr<IIAudioService> service = GetService();
    if (!service) return E_REMOTE_EXCEPTION;

    ECode ec = service->GetMasterVolume(result);
    if (FAILED(ec)) {
        Logger::E(TAG, "Dead object in GetMasterVolume");
        *result = 0;
        return E_REMOTE_EXCEPTION;
    }

    return NOERROR;
}

ECode CAudioManager::GetLastAudibleMasterVolume(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = 0;

    AutoPtr<IIAudioService> service = GetService();
    ECode ec = NOERROR;
    ec = service->GetLastAudibleMasterVolume(result);
    if (FAILED(ec)) {
        Logger::E(TAG, "Dead object in GetLastAudibleMasterVolume");
        *result = 0;
        return E_REMOTE_EXCEPTION;
    }

    return NOERROR;
}

ECode CAudioManager::SetMasterVolume(
    /* [in] */ Int32 index,
    /* [in] */ Int32 flags)
{
    AutoPtr<IIAudioService> service = GetService();
    if (!service) return E_REMOTE_EXCEPTION;

    ECode ec = service->SetMasterVolume(index, flags);
    if (FAILED(ec)) {
        Logger::E(TAG, "Dead object in SetMasterVolume");
        return E_REMOTE_EXCEPTION;
    }
    return NOERROR;
}

ECode CAudioManager::SetStreamSolo(
    /* [in] */ Int32 streamType,
    /* [in] */ Boolean state)
{
    AutoPtr<IIAudioService> service = GetService();
    if (!service) return E_REMOTE_EXCEPTION;

    ECode ec = service->SetStreamSolo(streamType, state, mICallBack);
    if (FAILED(ec)) {
        Logger::E(TAG, "Dead object in SetStreamSolo");
        return E_REMOTE_EXCEPTION;
    }
    return NOERROR;
}

ECode CAudioManager::SetStreamMute(
    /* [in] */ Int32 streamType,
    /* [in] */ Boolean state)
{
    AutoPtr<IIAudioService> service = GetService();
    if (!service) return E_REMOTE_EXCEPTION;

    ECode ec = service->SetStreamMute(streamType, state, mICallBack);
    if (FAILED(ec)) {
        Logger::E(TAG, "Dead object in SetStreamMute");
        return E_REMOTE_EXCEPTION;
    }
    return NOERROR;
}

ECode CAudioManager::IsStreamMute(
    /* [in] */ Int32 streamType,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    AutoPtr<IIAudioService> service = GetService();
    if (!service) return E_REMOTE_EXCEPTION;

    ECode ec = service->IsStreamMute(streamType, result);
    if (FAILED(ec)) {
        Logger::E(TAG, "Dead object in IsStreamMute");
        *result = FALSE;
        return E_REMOTE_EXCEPTION;
    }
    return NOERROR;
}

ECode CAudioManager::SetMasterMute(
    /* [in] */ Boolean state)
{
    return SetMasterMute(state, FLAG_SHOW_UI);
}

ECode CAudioManager::SetMasterMute(
    /* [in] */ Boolean state,
    /* [in] */ Int32 flags)
{
    AutoPtr<IIAudioService> service = GetService();
    if (!service) return E_REMOTE_EXCEPTION;

    ECode ec = service->SetMasterMute(state, flags, mICallBack);
    if (FAILED(ec)) {
        Logger::E(TAG, "Dead object in SetMasterMuteEx");
        return E_REMOTE_EXCEPTION;
    }
    return NOERROR;
}

ECode CAudioManager::IsMasterMute(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;

    AutoPtr<IIAudioService> service = GetService();
    if (!service) return E_REMOTE_EXCEPTION;

    ECode ec = service->IsMasterMute(result);
    if (FAILED(ec)) {
        Logger::E(TAG, "Dead object in IsMasterMute");
        *result = FALSE;
        return E_REMOTE_EXCEPTION;
    }
    return NOERROR;
}

ECode CAudioManager::ForceVolumeControlStream(
    /* [in] */ Int32 streamType)
{
    AutoPtr<IIAudioService> service = GetService();
    if (!service) return E_REMOTE_EXCEPTION;

    ECode ec = service->ForceVolumeControlStream(streamType, mICallBack);
    if (FAILED(ec)) {
        Logger::E(TAG, "Dead object in ForceVolumeControlStream");
        return E_REMOTE_EXCEPTION;
    }
    return NOERROR;
}

ECode CAudioManager::ShouldVibrate(
    /* [in] */ Int32 vibrateType,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;

    AutoPtr<IIAudioService> service = GetService();
    if (!service) return E_REMOTE_EXCEPTION;

    ECode ec = service->ShouldVibrate(vibrateType, result);
    if (FAILED(ec)) {
        Logger::E(TAG, "Dead object in ShouldVibrate");
        *result = FALSE;
        return E_REMOTE_EXCEPTION;
    }
    return NOERROR;
}

ECode CAudioManager::GetVibrateSetting(
    /* [in] */ Int32 vibrateType,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = VIBRATE_SETTING_OFF;

    AutoPtr<IIAudioService> service = GetService();
    if (!service) return E_REMOTE_EXCEPTION;

    ECode ec = service->GetVibrateSetting(vibrateType, result);
    if (FAILED(ec)) {
        Logger::E(TAG, "Dead object in GetVibrateSetting");
        *result = VIBRATE_SETTING_OFF;
        return E_REMOTE_EXCEPTION;
    }
    return NOERROR;
}

ECode CAudioManager::SetVibrateSetting(
    /* [in] */ Int32 vibrateType,
    /* [in] */ Int32 vibrateSetting)
{
    AutoPtr<IIAudioService> service = GetService();
    if (!service) return E_REMOTE_EXCEPTION;

    ECode ec = service->SetVibrateSetting(vibrateType, vibrateSetting);
    if (FAILED(ec)) {
        Logger::E(TAG, "Dead object in SetVibrateSetting");
        return E_REMOTE_EXCEPTION;
    }
    return NOERROR;
}

ECode CAudioManager::SetSpeakerphoneOn(
    /* [in] */ Boolean on)
{
    AutoPtr<IIAudioService> service = GetService();
    if (!service) return E_REMOTE_EXCEPTION;

    ECode ec = service->SetSpeakerphoneOn(on);
    if (FAILED(ec)) {
        Logger::E(TAG, "Dead object in setSpeakerphoneOn");
        return E_REMOTE_EXCEPTION;
    }
    return NOERROR;
}

ECode CAudioManager::IsSpeakerphoneOn(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;

    AutoPtr<IIAudioService> service = GetService();
    if (!service) return E_REMOTE_EXCEPTION;

    ECode ec = service->IsSpeakerphoneOn(result);
    if (FAILED(ec)) {
        Logger::E(TAG, "Dead object in IsSpeakerphoneOn");
        *result = FALSE;
        return E_REMOTE_EXCEPTION;
    }
    return NOERROR;
}

ECode CAudioManager::IsBluetoothScoAvailableOffCall(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    AutoPtr<IResources> resources;
    mContext->GetResources((IResources**)&resources);
    return resources->GetBoolean(R::bool_::config_bluetooth_sco_off_call, result);
}

ECode CAudioManager::StartBluetoothSco()
{
    AutoPtr<IIAudioService> service = GetService();
    if (!service) return E_REMOTE_EXCEPTION;

    ECode ec = service->StartBluetoothSco(mICallBack);
    if (FAILED(ec)) {
        Logger::E(TAG, "Dead object in StartBluetoothSco");
        return E_REMOTE_EXCEPTION;
    }
    return NOERROR;
}

ECode CAudioManager::StopBluetoothSco()
{
    AutoPtr<IIAudioService> service = GetService();
    if (!service) return E_REMOTE_EXCEPTION;

    ECode ec = service->StopBluetoothSco(mICallBack);
    if (FAILED(ec)) {
        Logger::E(TAG, "Dead object in StopBluetoothSco");
        return E_REMOTE_EXCEPTION;
    }
    return NOERROR;
}

ECode CAudioManager::SetBluetoothScoOn(
    /* [in] */ Boolean on)
{
    AutoPtr<IIAudioService> service = GetService();
    if (!service) return E_REMOTE_EXCEPTION;

    ECode ec = service->SetBluetoothScoOn(on);
    if (FAILED(ec)) {
        Logger::E(TAG, "Dead object in SetBluetoothScoOn");
        return E_REMOTE_EXCEPTION;
    }
    return NOERROR;
}

ECode CAudioManager::IsBluetoothScoOn(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;

    AutoPtr<IIAudioService> service = GetService();
    if (!service) return E_REMOTE_EXCEPTION;

    ECode ec = service->IsBluetoothScoOn(result);
    if (FAILED(ec)) {
        Logger::E(TAG, "Dead object in IsBluetoothScoOn");
        *result = FALSE;
        return E_REMOTE_EXCEPTION;
    }
    return NOERROR;
}

ECode CAudioManager::SetBluetoothA2dpOn(
    /* [in] */ Boolean on)
{
    return NOERROR;
}

ECode CAudioManager::IsBluetoothA2dpOn(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    Int32 state;
    String deviceAddress("");
    AutoPtr<IAudioSystemHelper> audioSystemHelper;
    CAudioSystemHelper::AcquireSingleton((IAudioSystemHelper**)&audioSystemHelper);
    audioSystemHelper->GetDeviceConnectionState(DEVICE_OUT_BLUETOOTH_A2DP, deviceAddress, &state);
    if (state == IAudioSystem::DEVICE_STATE_UNAVAILABLE) {
        *result = FALSE;
    }
    else {
        *result = TRUE;
    }
    return NOERROR;
}

ECode CAudioManager::SetRemoteSubmixOn(
    /* [in] */ Boolean on,
    /* [in] */ Int32 address)
{
    AutoPtr<IIAudioService> service = GetService();
    if (!service) return E_REMOTE_EXCEPTION;

    ECode ec = service->SetRemoteSubmixOn(on, address);
    if (FAILED(ec)) {
        Logger::E(TAG, "Dead object in SetRemoteSubmixOn");
        return E_REMOTE_EXCEPTION;
    }
    return NOERROR;
}

ECode CAudioManager::SetWiredHeadsetOn(
    /* [in] */ Boolean on)
{
    return NOERROR;
}

ECode CAudioManager::IsWiredHeadsetOn(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    Int32 state1;
    Int32 state2;
    String deviceAddress("");
    AutoPtr<IAudioSystemHelper> audioSystemHelper;
    CAudioSystemHelper::AcquireSingleton((IAudioSystemHelper**)&audioSystemHelper);
    audioSystemHelper->GetDeviceConnectionState(DEVICE_OUT_BLUETOOTH_A2DP, deviceAddress, &state1);
    audioSystemHelper->GetDeviceConnectionState(DEVICE_OUT_BLUETOOTH_A2DP, deviceAddress, &state2);
    if (state1 == IAudioSystem::DEVICE_STATE_UNAVAILABLE &&
        state2 == IAudioSystem::DEVICE_STATE_UNAVAILABLE) {
        *result = FALSE;
    }
    else {
        *result = TRUE;
    }
    return NOERROR;
}

ECode CAudioManager::SetMicrophoneMute(
    /* [in] */ Boolean on)
{
    Int32 status;
    AutoPtr<IAudioSystemHelper> audioSystemHelper;
    CAudioSystemHelper::AcquireSingleton((IAudioSystemHelper**)&audioSystemHelper);
    //status return command completion status see AUDIO_STATUS_OK, see AUDIO_STATUS_ERROR
    return audioSystemHelper->MuteMicrophone(on);
}

ECode CAudioManager::IsMicrophoneMute(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    AutoPtr<IAudioSystemHelper> audioSystemHelper;
    CAudioSystemHelper::AcquireSingleton((IAudioSystemHelper**)&audioSystemHelper);
    return audioSystemHelper->IsMicrophoneMuted(result);
}

ECode CAudioManager::SetMode(
    /* [in] */ Int32 mode)
{
    AutoPtr<IIAudioService> service = GetService();
    if (!service) return E_REMOTE_EXCEPTION;

    ECode ec = service->SetMode(mode, mICallBack);
    if (FAILED(ec)) {
        Logger::E(TAG, "Dead object in SetMode");
        return E_REMOTE_EXCEPTION;
    }
    return NOERROR;
}

ECode CAudioManager::GetMode(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    AutoPtr<IIAudioService> service = GetService();
    if (!service) return E_REMOTE_EXCEPTION;

    ECode ec = service->GetMode(result);
    if (FAILED(ec)) {
        Logger::E(TAG, "Dead object in GetMode");
        *result = MODE_INVALID;
        return E_REMOTE_EXCEPTION;
    }
    return NOERROR;
}

ECode CAudioManager::SetRouting(
    /* [in] */ Int32 mode,
    /* [in] */ Int32 routes,
    /* [in] */ Int32 mask)
{
    return NOERROR;
}

ECode CAudioManager::GetRouting(
    /* [in] */ Int32 mode,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = -1;
    return NOERROR;
}

ECode CAudioManager::IsMusicActive(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    AutoPtr<IAudioSystemHelper> audioSystemHelper;
    CAudioSystemHelper::AcquireSingleton((IAudioSystemHelper**)&audioSystemHelper);
    audioSystemHelper->IsStreamActive(STREAM_MUSIC, 0, result);
    return NOERROR;
}

ECode CAudioManager::IsSpeechRecognitionActive(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    AutoPtr<IAudioSystemHelper> audioSystemHelper;
    CAudioSystemHelper::AcquireSingleton((IAudioSystemHelper**)&audioSystemHelper);
    audioSystemHelper->IsSourceActive(
        IAudioSource::VOICE_RECOGNITION, result);
    return NOERROR;
}

ECode CAudioManager::AdjustLocalOrRemoteStreamVolume(
    /* [in] */ Int32 streamType,
    /* [in] */ Int32 direction)
{
    if (streamType != STREAM_MUSIC) {
        Logger::W(TAG, "AdjustLocalOrRemoteStreamVolume() doesn't support stream %d", streamType);
    }

    AutoPtr<IIAudioService> service = GetService();
    if (!service) return E_REMOTE_EXCEPTION;

    // try {
    ECode ec = service->AdjustLocalOrRemoteStreamVolume(streamType, direction);
    if (FAILED(ec)) {
        Logger::E(TAG, "Dead object in AdjustLocalOrRemoteStreamVolume");
        return E_REMOTE_EXCEPTION;
    }
    //} catch (RemoteException e) {
    //    Log.e(TAG, "Dead object in adjustLocalOrRemoteStreamVolume", e);
    //}
    return NOERROR;
}

ECode CAudioManager::SetParameter(
    /* [in] */ const String& key,
    /* [in] */ const String& value)
{
    SetParameters(key + "=" + value);
    return NOERROR;
}

ECode CAudioManager::SetParameters(
    /* [in] */ const String& keyValuePairs)
{
    AutoPtr<IAudioSystemHelper> audioSystemHelper;
    CAudioSystemHelper::AcquireSingleton((IAudioSystemHelper**)&audioSystemHelper);
    Int32 value;
    //value return keyValuePairs list of parameters key value pairs in the form: key1=value1;key2=value2;...
    audioSystemHelper->SetParameters(keyValuePairs);
    return NOERROR;
}

ECode CAudioManager::GetParameters(
    /* [in] */ const String& keys,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);

    AutoPtr<IAudioSystemHelper> audioSystemHelper;
    CAudioSystemHelper::AcquireSingleton((IAudioSystemHelper**)&audioSystemHelper);
    audioSystemHelper->GetParameters(keys, result);
    return NOERROR;
}

ECode CAudioManager::PlaySoundEffect(
    /* [in] */ Int32 effectType)
{
    if (effectType < 0 || effectType >= NUM_SOUND_EFFECTS) {
        return NOERROR;
    }

    if (!QuerySoundEffectsEnabled()) {
        return NOERROR;
    }

    AutoPtr<IIAudioService> service = GetService();
    if (!service) return E_REMOTE_EXCEPTION;

    ECode ec = service->PlaySoundEffect(effectType);
    if (FAILED(ec)) {
        Logger::E(TAG, "Dead object in PlaySoundEffect");
        return E_REMOTE_EXCEPTION;
    }
    return NOERROR;
}

ECode CAudioManager::PlaySoundEffect(
    /* [in] */ Int32 effectType,
    /* [in] */ Float volume)
{
    if (effectType < 0 || effectType >= NUM_SOUND_EFFECTS) {
        return NOERROR;
    }

    AutoPtr<IIAudioService> service = GetService();
    if (!service) return E_REMOTE_EXCEPTION;

    ECode ec = service->PlaySoundEffectVolume(effectType, volume);
    if (FAILED(ec)) {
        Logger::E(TAG, "Dead object in PlaySoundEffectEx");
        return E_REMOTE_EXCEPTION;
    }
    return NOERROR;
}

Boolean CAudioManager::QuerySoundEffectsEnabled()
{
    // todo: Eric audio service ready?
    AutoPtr<IContentResolver> contentResolver;
    mContext->GetContentResolver((IContentResolver**)&contentResolver);
    return Settings::System::GetInt32(contentResolver, ISettingsSystem::SOUND_EFFECTS_ENABLED, 0) != 0;
}

ECode CAudioManager::LoadSoundEffects()
{
    AutoPtr<IIAudioService> service = GetService();
    if (!service) return E_REMOTE_EXCEPTION;

    Boolean result;
    ECode ec = service->LoadSoundEffects(&result);
    if (FAILED(ec)) {
        Logger::E(TAG, "Dead object in LoadSoundEffects");
        return E_REMOTE_EXCEPTION;
    }
    return NOERROR;
}

ECode CAudioManager::UnloadSoundEffects()
{
    AutoPtr<IIAudioService> service = GetService();
    if (!service) return E_REMOTE_EXCEPTION;

    ECode ec = service->UnloadSoundEffects();
    if (FAILED(ec)) {
        Logger::E(TAG, "Dead object in UnloadSoundEffects");
        return E_REMOTE_EXCEPTION;
    }
    return NOERROR;
}

AutoPtr<IOnAudioFocusChangeListener> CAudioManager::FindFocusListener(
    /* [in] */ const String& id)
{
    HashMap<String, AutoPtr<IOnAudioFocusChangeListener> >::Iterator it =
            mAudioFocusIdListenerMap.Find(id);
    if (it != mAudioFocusIdListenerMap.End()) {
        return it->mSecond;
    }
    return NULL;
}

String CAudioManager::GetIdForAudioFocusListener(
    /* [in] */ IOnAudioFocusChangeListener* l)
{
    String msg("AudioManager:0x");
    msg += StringUtils::Int32ToHexString((Int32)this);
    if (l != NULL) {
        msg += ", OnAudioFocusChangeListener:0x";
        msg += StringUtils::Int32ToHexString((Int32)l);
    }
    return msg;
}

ECode CAudioManager::RegisterAudioFocusListener(
    /* [in] */ IOnAudioFocusChangeListener* l)
{
    String key = GetIdForAudioFocusListener(l);
    AutoLock lock(mFocusListenerLock);
    HashMap<String, AutoPtr<IOnAudioFocusChangeListener> >::Iterator it =
        mAudioFocusIdListenerMap.Find(key);
    if (it != mAudioFocusIdListenerMap.End()) {
        return NOERROR;
    }
    mAudioFocusIdListenerMap[key] = l;
    return NOERROR;
}

ECode CAudioManager::UnregisterAudioFocusListener(
    /* [in] */ IOnAudioFocusChangeListener* l)
{
    String key = GetIdForAudioFocusListener(l);
    AutoLock lock(mFocusListenerLock);
    HashMap<String, AutoPtr<IOnAudioFocusChangeListener> >::Iterator it =
    mAudioFocusIdListenerMap.Find(key);
    if (it != mAudioFocusIdListenerMap.End())
        mAudioFocusIdListenerMap.Erase(it);
    return NOERROR;
}

ECode CAudioManager::RequestAudioFocus(
    /* [in] */ IOnAudioFocusChangeListener* l,
    /* [in] */ Int32 streamType,
    /* [in] */ Int32 durationHint,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = AUDIOFOCUS_REQUEST_FAILED;

    Int32 status = AUDIOFOCUS_REQUEST_FAILED;
    if ((durationHint < AUDIOFOCUS_GAIN) || (durationHint > AUDIOFOCUS_GAIN_TRANSIENT_MAY_DUCK)) {
        Logger::E(TAG, "Invalid duration hint, audio focus request denied");
        *result = status;
        return NOERROR;
    }

    RegisterAudioFocusListener(l);
    //TODO protect request by permission check?
    AutoPtr<IIAudioService> service = GetService();
    if (!service) return E_REMOTE_EXCEPTION;

    ECode ec = NOERROR;
    String packageName;
    do {
        ec = mContext->GetPackageName(&packageName);
        if (FAILED(ec)) break;

        ec = service->RequestAudioFocus(streamType, durationHint, mICallBack,
             mAudioFocusDispatcher, GetIdForAudioFocusListener(l),
             packageName /* package name */, &status);
        if (FAILED(ec)) break;
    } while(0);

    if (FAILED(ec)) {
        Logger::E(TAG, "Can't call RequestAudioFocus() from AudioService due to ");
        return E_REMOTE_EXCEPTION;
    }

    *result = status;
    return NOERROR;
}

ECode CAudioManager::RequestAudioFocusForCall(
    /* [in] */ Int32 streamType,
    /* [in] */ Int32 durationHint)
{
    AutoPtr<IIAudioService> service = GetService();
    if (!service) return E_REMOTE_EXCEPTION;

    Int32 focus;
    String callingPackageName("system");
    ECode ec = service->RequestAudioFocus(streamType, durationHint, mICallBack, NULL,
        IAudioService::IN_VOICE_COMM_FOCUS_ID,
        callingPackageName /* dump-friendly package name */, &focus);
    if (FAILED(ec)) {
        Logger::E(TAG, "Can't call RequestAudioFocusForCall() on AudioService due to ");
        return E_REMOTE_EXCEPTION;
    }
    return NOERROR;
}

ECode CAudioManager::AbandonAudioFocusForCall()
{
    AutoPtr<IIAudioService> service = GetService();
    if (!service) return E_REMOTE_EXCEPTION;

    Int32 focus;
    ECode ec = service->AbandonAudioFocus(NULL, IAudioService::IN_VOICE_COMM_FOCUS_ID, &focus);
    if (FAILED(ec)) {
        Logger::E(TAG, "Can't call AbandonAudioFocusForCall() on AudioService due to ");
        return E_REMOTE_EXCEPTION;
    }
    return NOERROR;
}

ECode CAudioManager::AbandonAudioFocus(
    /* [in] */ IOnAudioFocusChangeListener* l,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = AUDIOFOCUS_REQUEST_FAILED;

    UnregisterAudioFocusListener(l);
    AutoPtr<IIAudioService> service = GetService();
    if (!service) return E_REMOTE_EXCEPTION;

    Int32 status = AUDIOFOCUS_REQUEST_FAILED;
    ECode ec = service->AbandonAudioFocus(mAudioFocusDispatcher,
        GetIdForAudioFocusListener(l),&status);
    if (FAILED(ec)) {
        Logger::E(TAG, "Can't call AbandonAudioFocus() on AudioService due to ");
        return E_REMOTE_EXCEPTION;
    }
    return NOERROR;
}

//====================================================================
// Remote Control

ECode CAudioManager::RegisterMediaButtonEventReceiver(
    /* [in] */ IComponentName* eventReceiver)
{
    if (eventReceiver == NULL) {
        return NOERROR;
    }
    String cpnName, contextName;
    eventReceiver->GetPackageName(&cpnName);
    mContext->GetPackageName(&contextName);
    if (!cpnName.Equals(contextName)) {
        Logger::E(TAG, "RegisterMediaButtonEventReceiver() error: "
            "receiver and context package names don't match");
        return NOERROR;
    }

    // construct a PendingIntent for the media button and register it
    AutoPtr<IIntent> mediaButtonIntent ;
    CIntent::New(IIntent::ACTION_MEDIA_BUTTON, (IIntent**)&mediaButtonIntent);
    //     the associated intent will be handled by the component being registered
    mediaButtonIntent->SetComponent(eventReceiver);
    AutoPtr<IPendingIntentHelper> pi;
    AutoPtr<IPendingIntent> pendingIntent;
    pi->GetBroadcast(mContext,
        0/*requestCode, ignored*/, mediaButtonIntent,
        0/*flags*/, (IPendingIntent**)&pendingIntent);
    return RegisterMediaButtonIntent(pendingIntent, eventReceiver);
}

ECode CAudioManager::RegisterMediaButtonIntent(
    /* [in] */ IPendingIntent* pi,
    /* [in] */ IComponentName* eventReceiver)
{
    if ((pi == NULL) || (eventReceiver == NULL)) {
        Logger::E(TAG, "Cannot call RegisterMediaButtonIntent() with a NULL parameter");
        return NOERROR;
    }

    AutoPtr<IIAudioService> service = GetService();
    if (!service) return E_REMOTE_EXCEPTION;

    // pi != null
    ECode ec = service->RegisterMediaButtonIntent(pi, eventReceiver);
    if (FAILED(ec)) {
        Logger::E(TAG, "Dead object in RegisterMediaButtonIntent");
        return E_REMOTE_EXCEPTION;
    }
    return NOERROR;
}

ECode CAudioManager::RegisterMediaButtonEventReceiverForCalls(
    /* [in] */ IComponentName* eventReceiver)
{
    if (eventReceiver == NULL) {
        return NOERROR;
    }

    AutoPtr<IIAudioService> service = GetService();
    if (!service) return E_REMOTE_EXCEPTION;

    // eventReceiver != NULL
    ECode ec = service->RegisterMediaButtonEventReceiverForCalls(eventReceiver);
    if (FAILED(ec)) {
        Logger::E(TAG, "Dead object in RegisterMediaButtonEventReceiverForCalls");
        return E_REMOTE_EXCEPTION;
    }
    return NOERROR;
}

ECode CAudioManager::UnregisterMediaButtonEventReceiverForCalls()
{
    AutoPtr<IIAudioService> service = GetService();
    if (!service) return E_REMOTE_EXCEPTION;

    ECode ec = service->UnregisterMediaButtonEventReceiverForCalls();
    if (FAILED(ec)) {
        Logger::E(TAG, "Dead object in UnregisterMediaButtonEventReceiverForCalls");
        return E_REMOTE_EXCEPTION;
    }
    return NOERROR;
}

ECode CAudioManager::UnregisterMediaButtonEventReceiver(
    /* [in] */ IComponentName* eventReceiver)
{
    if (eventReceiver == NULL) {
        return NOERROR;
    }
    // construct a PendingIntent for the media button and unregister it
    AutoPtr<IIntent> mediaButtonIntent;
    CIntent::New(IIntent::ACTION_MEDIA_BUTTON, (IIntent**)&mediaButtonIntent);
    //     the associated intent will be handled by the component being registered
    mediaButtonIntent->SetComponent(eventReceiver);
    AutoPtr<IPendingIntent> pendingIntent;
    AutoPtr<IPendingIntentHelper> pi;
    pi->GetBroadcast(mContext,
        0/*requestCode, ignored*/, mediaButtonIntent,
        0/*flags*/, (IPendingIntent**)&pendingIntent);
    return UnregisterMediaButtonIntent(pendingIntent, eventReceiver);
}

ECode CAudioManager::UnregisterMediaButtonIntent(
    /* [in] */ IPendingIntent* pi,
    /* [in] */ IComponentName* eventReceiver)
{
    AutoPtr<IIAudioService> service = GetService();
    if (!service) return E_REMOTE_EXCEPTION;

    ECode ec = service->UnregisterMediaButtonIntent(pi, eventReceiver);
    if (FAILED(ec)) {
        Logger::E(TAG, "Dead object in UnregisterMediaButtonIntent");
        return E_REMOTE_EXCEPTION;
    }
    return NOERROR;
}

ECode CAudioManager::RegisterRemoteControlClient(
    /* [in] */ IRemoteControlClient* rcClient)
{
    AutoPtr<IPendingIntent> pendIntent;
    rcClient->GetRcMediaIntent((IPendingIntent**)&pendIntent);
    if (rcClient == NULL || pendIntent == NULL) {
        return NOERROR;
    }

    AutoPtr<IIAudioService> service = GetService();
    if (!service) return E_REMOTE_EXCEPTION;

    ECode ec = NOERROR;
    do {
        Int32 rcseId;
        AutoPtr<IIRemoteControlClient> remoteControlClient;
        ec = rcClient->GetIRemoteControlClient((IIRemoteControlClient**)&remoteControlClient);
        if (FAILED(ec)) break;

        String packageName("");
        ec = mContext->GetPackageName(&packageName);
        if (FAILED(ec)) break;

        ec = service->RegisterRemoteControlClient(
                pendIntent,            /* mediaIntent   */
                remoteControlClient,  /* rcClient      */
                // used to match media button event receiver and audio focus
                packageName,           /* packageName   */
                &rcseId);
        if (FAILED(ec)) break;

        ec = rcClient->SetRcseId(rcseId);
        if (FAILED(ec)) break;
    } while(0);

    if (FAILED(ec)) {
        Logger::E(TAG, "Dead object in RegisterRemoteControlClient");
        return E_REMOTE_EXCEPTION;
    }
    return NOERROR;
}

ECode CAudioManager::UnregisterRemoteControlClient(
    /* [in] */ IRemoteControlClient* rcClient)
{
    AutoPtr<IPendingIntent> pendIntent;
    rcClient->GetRcMediaIntent((IPendingIntent**)&pendIntent);
    if (rcClient == NULL || pendIntent == NULL) {
        return NOERROR;
    }

    AutoPtr<IIAudioService> service = GetService();
    if (!service) return E_REMOTE_EXCEPTION;

    ECode ec = NOERROR;
    do {
        AutoPtr<IIRemoteControlClient> remoteControlClient;
        ec = rcClient->GetIRemoteControlClient((IIRemoteControlClient**)&remoteControlClient);
        if (FAILED(ec)) break;

        ec = service->UnregisterRemoteControlClient(
            pendIntent, /* mediaIntent   */
            remoteControlClient); /* rcClient      */
        if (FAILED(ec)) break;
    } while(0);

    if (FAILED(ec)) {
        Logger::E(TAG, "Dead object in UnregisterRemoteControlClient");
        return E_REMOTE_EXCEPTION;
    }
    return NOERROR;
}

ECode CAudioManager::RegisterRemoteControlDisplay(
    /* [in] */ IIRemoteControlDisplay* rcd)
{
    if (rcd == NULL) {
        return NOERROR;
    }

    AutoPtr<IIAudioService> service = GetService();
    if (!service) return E_REMOTE_EXCEPTION;

    ECode ec = service->RegisterRemoteControlDisplay(rcd);
    if (FAILED(ec)) {
        Logger::E(TAG, "Dead object in RegisterRemoteControlDisplay");
        return E_REMOTE_EXCEPTION;
    }
    return NOERROR;
}

ECode CAudioManager::UnregisterRemoteControlDisplay(
    /* [in] */ IIRemoteControlDisplay* rcd)
{
    if (rcd == NULL) {
        return NOERROR;
    }

    AutoPtr<IIAudioService> service = GetService();
    if (!service) return E_REMOTE_EXCEPTION;


    ECode ec = service->UnregisterRemoteControlDisplay(rcd);
    if (FAILED(ec)) {
        Logger::E(TAG, "Dead object in UnregisterRemoteControlDisplay ");
        return E_REMOTE_EXCEPTION;
    }
    return NOERROR;
}

ECode CAudioManager::RemoteControlDisplayUsesBitmapSize(
    /* [in] */ IIRemoteControlDisplay* rcd,
    /* [in] */ Int32 w,
    /* [in] */ Int32 h)
{
    if (rcd == NULL) {
        return NOERROR;
    }

    AutoPtr<IIAudioService> service = GetService();
    if (!service) return E_REMOTE_EXCEPTION;

    ECode ec = service->RemoteControlDisplayUsesBitmapSize(rcd, w, h);
    if (FAILED(ec)) {
        Logger::E(TAG, "Dead object in RemoteControlDisplayUsesBitmapSize ");
        return E_REMOTE_EXCEPTION;
    }
    return NOERROR;
}

ECode CAudioManager::ReloadAudioSettings()
{
    AutoPtr<IIAudioService> service = GetService();
    if (!service) return E_REMOTE_EXCEPTION;

    ECode ec = service->ReloadAudioSettings();
    if (FAILED(ec)) {
        Logger::E(TAG, "Dead object in ReloadAudioSettings");
        return E_REMOTE_EXCEPTION;
    }
    return NOERROR;
}

ECode CAudioManager::IsSilentMode(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    Int32 ringerMode;
    GetRingerMode(&ringerMode);
    *result = (ringerMode == RINGER_MODE_SILENT)
        || (ringerMode == RINGER_MODE_VIBRATE);
    return NOERROR;
}

ECode CAudioManager::GetDevicesForStream(
    /* [in] */ Int32 streamType,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    switch (streamType) {
        case STREAM_VOICE_CALL:
        case STREAM_SYSTEM:
        case STREAM_RING:
        case STREAM_MUSIC:
        case STREAM_ALARM:
        case STREAM_NOTIFICATION:
        case STREAM_DTMF:
        {
            AutoPtr<IAudioSystemHelper> audioSystemHelper;
            CAudioSystemHelper::AcquireSingleton((IAudioSystemHelper**)&audioSystemHelper);
            return audioSystemHelper->GetDevicesForStream(streamType, result);
        }

        default:
        *result = 0;
        return NOERROR;
    }
}

ECode CAudioManager::SetWiredDeviceConnectionState(
    /* [in] */ Int32 device,
    /* [in] */ Int32 state,
    /* [in] */ const String& name)
{
    AutoPtr<IIAudioService> service = GetService();
    if (!service) return E_REMOTE_EXCEPTION;

    ECode ec = service->SetWiredDeviceConnectionState(device, state, name);
    if (FAILED(ec)) {
        Logger::E(TAG, "Dead object in SetWiredDeviceConnectionState ");
        return E_REMOTE_EXCEPTION;
    }
    return NOERROR;
}

ECode CAudioManager::SetBluetoothA2dpDeviceConnectionState(
    /* [in] */ IBluetoothDevice* device,
    /* [in] */ Int32 state,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = 0;

    AutoPtr<IIAudioService> service = GetService();
    if (!service) return E_REMOTE_EXCEPTION;

    Int32 delay;
    ECode ec = service->SetBluetoothA2dpDeviceConnectionState(device, state, &delay);
    if (FAILED(ec)) {
        Logger::E(TAG, "Dead object in SetBluetoothA2dpDeviceConnectionState ");
        *result = 0;
        return E_REMOTE_EXCEPTION;
    }

    *result = delay;
    return NOERROR;
}

ECode CAudioManager::GetRingtonePlayer(
    /* [out] */ IIRingtonePlayer** result)
{
    VALIDATE_NOT_NULL(result);
    *result = NULL;

    AutoPtr<IIAudioService> service = GetService();
    if (!service) return E_REMOTE_EXCEPTION;

    ECode ec = service->GetRingtonePlayer(result);
    if (FAILED(ec)) {
        *result = NULL;
        return E_REMOTE_EXCEPTION;
    }

    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CAudioManager::GetProperty(
    /* [in] */ const String& key,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    *result = String(NULL);

    if (PROPERTY_OUTPUT_SAMPLE_RATE.Equals(key)) {
        Int32 outputSampleRate;
        AutoPtr<IAudioSystemHelper> audioSystemHelper;
        CAudioSystemHelper::AcquireSingleton((IAudioSystemHelper**)&audioSystemHelper);
        audioSystemHelper->GetPrimaryOutputSamplingRate(&outputSampleRate);
        if (outputSampleRate > 0) {
            *result = StringUtils::Int32ToString(outputSampleRate);
        }
        return NOERROR;
    }
    else if (PROPERTY_OUTPUT_FRAMES_PER_BUFFER.Equals(key)) {
        Int32 outputFramesPerBuffer;
        AutoPtr<IAudioSystemHelper> audioSystemHelper;
        CAudioSystemHelper::AcquireSingleton((IAudioSystemHelper**)&audioSystemHelper);
        audioSystemHelper->GetPrimaryOutputFrameCount(&outputFramesPerBuffer);
        if (outputFramesPerBuffer > 0) {
            *result = StringUtils::Int32ToString(outputFramesPerBuffer);
        }
        return NOERROR;
    }
    else {
        // NULL or unknown key
        return NOERROR;
    }
}

void CAudioManager::WriteFileVal(
    /* [in] */ const String& filename,
    /* [in] */ Int32 avail)
{
    SystemProperties::Set(filename, StringUtils::Int32ToString(avail));
}

Int32 CAudioManager::ReadFileVal(
    /* [in] */ const String& filename)
{
    return SystemProperties::GetInt32(filename, 0);
}

ECode CAudioManager::SetHdmiAvailable(
    /* [in] */ Boolean val)
{
    Int32 avail = (val == TRUE) ? 1 : 0;
    WriteFileVal(hdmiAvailable, avail);
    return NOERROR;
}

ECode CAudioManager::GetHdmiAvailable(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    Int32 ret = ReadFileVal(hdmiAvailable);
    *result = (ret == 1) ? TRUE : FALSE;
    return NOERROR;
}

ECode CAudioManager::GetHdmiExpected(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    Int32 ret = ReadFileVal(hdmiExpected);
    *result = (ret == 1) ? TRUE : FALSE;
    return NOERROR;
}

//static
ECode CAudioManager::SetHdmiExpected(
    /* [in] */ Boolean val)
{
    Int32 avail = (val == TRUE) ? 1 : 0;
    WriteFileVal(hdmiExpected, avail);
    return NOERROR;
}

// static
ECode CAudioManager::GetAudioDevices(
    /* [in] */ const String& devType,
    /* [out, callle] */ ArrayOf<String>** result)
{
    VALIDATE_NOT_NULL(result);
    *result = NULL;

    if (!devType.Equals(IAudioManager::AUDIO_INPUT_TYPE)
        && !devType.Equals(IAudioManager::AUDIO_OUTPUT_TYPE)){
        return NOERROR;
    }

    String listStr;
    GetParameters(devType, &listStr);
    if (listStr.IsNull()) {
        return NOERROR;
    }

    Logger::D(TAG, "type %s list %s", devType.string(), listStr.string());
    AutoPtr< ArrayOf<String> > audioList;
    StringUtils::Split(listStr, ",", (ArrayOf<String>**)&audioList);

    List<String> list;
    Int32 length = (audioList != NULL) ? audioList->GetLength() : 0;
    for (Int32 i = 0; i < length; ++i) {
        if (!(*audioList)[i].IsNullOrEmpty()) {
            list.PushBack((*audioList)[i]);
        }
    }

    List<String>::Iterator it;
    if (devType.Equals(IAudioManager::AUDIO_OUTPUT_ACTIVE)){
        it = Find(list.Begin(), list.End(), IAudioManager::AUDIO_NAME_HDMI);
        if (it == list.End()) {
            Boolean temp = FALSE;
            GetHdmiExpected(&temp);
            if (temp) {
                list.PushBack(AUDIO_NAME_HDMI);
            }
        }
    }

    AutoPtr<ArrayOf<String> > audioDevList = ArrayOf<String>::Alloc(list.GetSize());
    it = list.Begin();
    for (Int32 i = 0; it != list.End(); ++it, ++i) {
        audioList->Set(i, *it);
    }

    *result = audioDevList;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

// static
ECode CAudioManager::GetActiveAudioDevices(
    /* [in] */ const String& devType,
    /* [out, callle] */ ArrayOf<String>** result)
{
    VALIDATE_NOT_NULL(result);
    *result = NULL;

    if (!devType.Equals(IAudioManager::AUDIO_INPUT_ACTIVE)
        && !devType.Equals(IAudioManager::AUDIO_OUTPUT_ACTIVE)){
        return NOERROR;
    }

    String listStr;
    GetParameters(devType, &listStr);
    if (listStr.IsNull()) {
        return NOERROR;
    }

    Logger::D(TAG, "type %s list %s", devType.string(), listStr.string());
    AutoPtr< ArrayOf<String> > audioList;
    StringUtils::Split(listStr, ",", (ArrayOf<String>**)&audioList);

    List<String> list;
    Int32 length = (audioList != NULL) ? audioList->GetLength() : 0;
    for (Int32 i = 0; i < length; ++i) {
        if (!(*audioList)[i].IsNullOrEmpty()) {
            list.PushBack((*audioList)[i]);
        }
    }

    List<String>::Iterator it;
    if (devType.Equals(IAudioManager::AUDIO_OUTPUT_ACTIVE)){
        it = Find(list.Begin(), list.End(), IAudioManager::AUDIO_NAME_HDMI);
        if (it == list.End()) {
            Boolean temp = FALSE;
            GetHdmiExpected(&temp);
            if (temp) {
                list.PushBack(AUDIO_NAME_HDMI);
            }
        }
    }

    AutoPtr<ArrayOf<String> > audioDevList = ArrayOf<String>::Alloc(list.GetSize());
    it = list.Begin();
    for (Int32 i = 0; it != list.End(); ++it, ++i) {
        audioList->Set(i, *it);
    }

    *result = audioDevList;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CAudioManager::SetAudioDeviceActive(
    /* [in] */ const ArrayOf<String>& _devices,
    /* [in] */ const String& state)
{
    if (_devices.GetLength() == 0 ||
        (!state.Equals(IAudioManager::AUDIO_INPUT_ACTIVE)
        && !state.Equals(IAudioManager::AUDIO_OUTPUT_ACTIVE))){
        return NOERROR;
    }

    AutoPtr<ArrayOf<String> > devices = _devices.Clone();

    String audio;

    AutoPtr<IContentResolver> contentResolver;
    mContext->GetContentResolver((IContentResolver**)&contentResolver);
    Int32 value = Settings::System::GetInt32(contentResolver, ISettingsSystem::ENABLE_PASS_THROUGH, 0);

    Boolean enablePassThrough = (value == 1);
    if (enablePassThrough && state.Equals(AUDIO_OUTPUT_ACTIVE)) {
        AutoPtr<IInterface> obj;
        mContext->GetSystemService(IContext::DISPLAY_SERVICE_AW, (IInterface**)&obj);
        AutoPtr<IDisplayManagerAw> dm = IDisplayManagerAw::Probe(obj.Get());
        Boolean hdmi;
        GetHdmiAvailable(&hdmi);
        Logger::D(TAG, "Hdmi status %d, size %d", hdmi, devices->GetLength());
        Int32 count = devices->GetLength();
        if(count > 0) {
            Logger::D(TAG, "HDMI %s", (*devices)[0].string());
        }

        Boolean contains;
        if (devices->Contains(IAudioManager::AUDIO_NAME_SPDIF)) {
            audio = IAudioManager::AUDIO_NAME_SPDIF;
            CMediaPlayer::SetRawDataMode(IMediaPlayer::AUDIO_DATA_MODE_SPDIF_RAW);
        }
        else if (devices->Contains(IAudioManager::AUDIO_NAME_HDMI)) {
            if (hdmi) {
                audio = IAudioManager::AUDIO_NAME_HDMI;
                CMediaPlayer::SetRawDataMode(IMediaPlayer::AUDIO_DATA_MODE_HDMI_RAW);
                SetHdmiExpected(FALSE);
            }
            else{
                SetHdmiExpected(TRUE);
            }
        }
        else {
            audio = IAudioManager::AUDIO_NAME_CODEC;
            CMediaPlayer::SetRawDataMode(IMediaPlayer::AUDIO_DATA_MODE_PCM);
        }
    }
    else {
        if (state.Equals(IAudioManager::AUDIO_OUTPUT_ACTIVE)){
            Boolean available;
            GetHdmiAvailable(&available);

            if (DEBUG) {
                StringBuilder sb("hdmiAvailable=");
                sb += available;
                sb += " list={";
                for (Int32 i = 0; i < devices->GetLength(); ++i) {
                    if (i != 0)
                        sb += ", ";
                    sb += (*devices)[i];
                }
                sb += "}";
                Logger::D(TAG, sb.ToString().string());
            }

            Int32 index = devices->IndexOf(IAudioManager::AUDIO_NAME_HDMI);
            if (!available && index > 0) {
                SetHdmiExpected(TRUE);
                (*devices)[index] = NULL;
            }
            else {
                SetHdmiExpected(FALSE);
            }
        }

        StringBuilder sb;
        Boolean first = TRUE;
        for (Int32 i = 0; i < devices->GetLength(); ++i) {
            if (!(*devices)[i].IsNullOrEmpty()) {
                if (!first) {
                    sb += ",";
                }
                else {
                    first = FALSE;
                }

                sb += (*devices)[i];
            }
        }
        audio = sb.ToString();
    }

    if (!enablePassThrough) {
        CMediaPlayer::SetRawDataMode(IMediaPlayer::AUDIO_DATA_MODE_PCM);
    }
    return SetParameter(state, audio);
}

} // namespace Media
} // namepsace Droid
} // namespace Elastos
