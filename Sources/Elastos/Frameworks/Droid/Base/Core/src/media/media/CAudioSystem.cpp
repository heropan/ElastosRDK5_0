
#include "media/CAudioSystem.h"
#include <elastos/utility/logging/Logger.h>

#include <media/AudioSystem.h>

using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Media {

static const Boolean DBG = TRUE;

const Int32 CAudioSystem::NUM_STREAM_TYPES = 10;
AutoPtr<IAudioSystemErrorCallback> CAudioSystem::mErrorCallback;
const Int32 CAudioSystem::NUM_DEVICE_STATES = 1;
const Int32 CAudioSystem::NUM_FORCE_CONFIG = 12;
const Int32 CAudioSystem::NUM_FORCE_USE = 5;
Mutex CAudioSystem::sStaticAudioSystemLock;
Boolean CAudioSystem::sInitAudioSystem = FALSE;

static void android_media_AudioSystem_error_callback(android::status_t err)
{
    Int32 error = IAudioSystem::AUDIO_STATUS_ERROR;
    switch (err) {
        case android::DEAD_OBJECT:
            error = IAudioSystem::AUDIO_STATUS_SERVER_DIED;
            break;
        case android::NO_ERROR:
            error = IAudioSystem::AUDIO_STATUS_OK;
            break;
        default:
            error = IAudioSystem::AUDIO_STATUS_ERROR;
            break;
    }

    CAudioSystem::ErrorCallbackFromNative(error);
}

CAudioSystem::CAudioSystem()
{
    if (!sInitAudioSystem) {
        android::AudioSystem::setErrorCallback(android_media_AudioSystem_error_callback);
        sInitAudioSystem = TRUE;
    }
}

ECode CAudioSystem::GetNumStreamTypes(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = NUM_STREAM_TYPES;
    return NOERROR;
}

ECode CAudioSystem::MuteMicrophone(
    /* [in] */ Boolean on)
{
    return CheckAudioSystemCommand(android::AudioSystem::muteMicrophone(on));
}

ECode CAudioSystem::IsMicrophoneMuted(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    bool state = FALSE;
    android::AudioSystem::isMicrophoneMuted(&state);
    *result = (Boolean)state;
    return NOERROR;
}

ECode CAudioSystem::IsStreamActive(
    /* [in] */ Int32 stream,
    /* [in] */ Int32 inPastMs,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    bool state = FALSE;
    android::AudioSystem::isStreamActive((audio_stream_type_t)stream, &state, inPastMs);
    *result = (Boolean)state;
    return NOERROR;
}

ECode CAudioSystem::IsSourceActive(
    /* [in] */ Int32 source,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    bool state = FALSE;
    android::AudioSystem::isSourceActive((audio_source_t)source, &state);
    *result = (Boolean)state;
    return NOERROR;
}

ECode CAudioSystem::SetParameters(
    /* [in] */ const String& keyValuePairs)
{
    const char* c_keyValuePairs = keyValuePairs.string();
    android::String8 c_keyValuePairs8;
    if (keyValuePairs) {
        c_keyValuePairs8 = android::String8(c_keyValuePairs);
    }
    return CheckAudioSystemCommand(
        android::AudioSystem::setParameters(0, c_keyValuePairs8));
}

ECode CAudioSystem::GetParameters(
    /* [in] */ const String& keys,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);

    const char* c_keys = keys.string();
    android::String8 c_keys8;
    if (keys) {
        c_keys8 = android::String8(c_keys);
    }
    *result = android::AudioSystem::getParameters(0, c_keys8).string();
    return NOERROR;
}

ECode CAudioSystem::SetErrorCallback(
    /* [in] */ IAudioSystemErrorCallback* cb)
{
    {
        Mutex::Autolock lock(sStaticAudioSystemLock);
        mErrorCallback = cb;
    }
    // Calling a method on AudioFlinger here makes sure that we bind to IAudioFlinger
    // binder interface death. Not doing that would result in not being notified of
    // media_server process death if no other method is called on AudioSystem that reaches
    // to AudioFlinger.
    Boolean result = FALSE;
    IsMicrophoneMuted(&result);
    return NOERROR;
}

ECode CAudioSystem::GetDeviceName(
    /* [in] */ Int32 device,
    /* [out] */ String* result)
{
    switch(device) {
    case DEVICE_OUT_EARPIECE:
        *result = DEVICE_OUT_EARPIECE_NAME;
        return NOERROR;

    case DEVICE_OUT_SPEAKER:
        *result = DEVICE_OUT_SPEAKER_NAME;
        return NOERROR;

    case DEVICE_OUT_WIRED_HEADSET:
        *result = DEVICE_OUT_WIRED_HEADSET_NAME;
        return NOERROR;

    case DEVICE_OUT_WIRED_HEADPHONE:
        *result = DEVICE_OUT_WIRED_HEADPHONE_NAME;
        return NOERROR;

    case DEVICE_OUT_BLUETOOTH_SCO:
        *result = DEVICE_OUT_BLUETOOTH_SCO_NAME;
        return NOERROR;

    case DEVICE_OUT_BLUETOOTH_SCO_HEADSET:
        *result = DEVICE_OUT_BLUETOOTH_SCO_HEADSET_NAME;
        return NOERROR;

    case DEVICE_OUT_BLUETOOTH_SCO_CARKIT:
        *result = DEVICE_OUT_BLUETOOTH_SCO_CARKIT_NAME;
        return NOERROR;

    case DEVICE_OUT_BLUETOOTH_A2DP:
        *result = DEVICE_OUT_BLUETOOTH_A2DP_NAME;
        return NOERROR;

    case DEVICE_OUT_BLUETOOTH_A2DP_HEADPHONES:
        *result = DEVICE_OUT_BLUETOOTH_A2DP_HEADPHONES_NAME;
        return NOERROR;

    case DEVICE_OUT_BLUETOOTH_A2DP_SPEAKER:
        *result = DEVICE_OUT_BLUETOOTH_A2DP_SPEAKER_NAME;
        return NOERROR;

    case DEVICE_OUT_AUX_DIGITAL:
        *result = DEVICE_OUT_AUX_DIGITAL_NAME;
        return NOERROR;

    case DEVICE_OUT_ANLG_DOCK_HEADSET:
        *result = DEVICE_OUT_ANLG_DOCK_HEADSET_NAME;
        return NOERROR;

    case DEVICE_OUT_DGTL_DOCK_HEADSET:
        *result = DEVICE_OUT_DGTL_DOCK_HEADSET_NAME;
        return NOERROR;

    case DEVICE_OUT_USB_ACCESSORY:
        *result = DEVICE_OUT_USB_ACCESSORY_NAME;
        return NOERROR;

    case DEVICE_OUT_USB_DEVICE:
        *result = DEVICE_OUT_USB_DEVICE_NAME;
        return NOERROR;

    case DEVICE_OUT_REMOTE_SUBMIX:
        *result = DEVICE_OUT_REMOTE_SUBMIX_NAME;
        return NOERROR;

    case DEVICE_OUT_DEFAULT:
    default:
        *result = "";
        return NOERROR;
    }
}

ECode CAudioSystem::SetDeviceConnectionState(
    /* [in] */ Int32 device,
    /* [in] */ Int32 state,
    /* [in] */ const String& device_address)
{
    const char *c_address = device_address.string();
    return CheckAudioSystemCommand(
        android::AudioSystem::setDeviceConnectionState(
            static_cast<audio_devices_t>(device),
            static_cast<audio_policy_dev_state_t>(state),
            c_address));
}

ECode CAudioSystem::GetDeviceConnectionState(
    /* [in] */ Int32 device,
    /* [in] */ const String& device_address,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    const char *c_address = device_address.string();
    *result = static_cast<Int32>(
        android::AudioSystem::getDeviceConnectionState(
            static_cast<audio_devices_t>(device),
            c_address));
    return NOERROR;
}

ECode CAudioSystem::SetPhoneState(
    /* [in] */ Int32 state)
{
    return CheckAudioSystemCommand(
        android::AudioSystem::setPhoneState((audio_mode_t)state));
}

ECode CAudioSystem::SetForceUse(
    /* [in] */ Int32 usage,
    /* [in] */ Int32 config)
{
    return CheckAudioSystemCommand(
        android::AudioSystem::setForceUse(
            static_cast<audio_policy_force_use_t>(usage),
            static_cast<audio_policy_forced_cfg_t>(config)));
}

ECode CAudioSystem::GetForceUse(
    /* [in] */ Int32 usage,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    *result = static_cast<Int32>(
        android::AudioSystem::getForceUse(
            static_cast<audio_policy_force_use_t>(usage)));
    return NOERROR;
}

ECode CAudioSystem::InitStreamVolume(
    /* [in] */ Int32 stream,
    /* [in] */ Int32 indexMin,
    /* [in] */ Int32 indexMax)
{
    return CheckAudioSystemCommand(
        android::AudioSystem::initStreamVolume(
            static_cast<audio_stream_type_t>(stream),
            indexMin, indexMax));
}

ECode CAudioSystem::SetStreamVolumeIndex(
    /* [in] */ Int32 stream,
    /* [in] */ Int32 index,
    /* [in] */ Int32 device)
{
    ECode ec = CheckAudioSystemCommand(
        android::AudioSystem::setStreamVolumeIndex(
            static_cast<audio_stream_type_t>(stream),
            index, (audio_devices_t)device));
    if (FAILED(ec)) {
        Logger::E("CAudioSystem",
            "Failed to SetStreamVolumeIndex stream %d, index %d, device %d",
            stream, index, device);
    }

    return ec;
}

ECode CAudioSystem::GetStreamVolumeIndex(
    /* [in] */ Int32 stream,
    /* [in] */ Int32 device,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    Int32 index = 0;
    if (android::AudioSystem::getStreamVolumeIndex(
       static_cast<audio_stream_type_t>(stream),
       &index, (audio_devices_t)device) != android::NO_ERROR)
    {
        index = -1;
    }
    *result = index;
    return NOERROR;
}

ECode CAudioSystem::SetMasterVolume(
    /* [in] */ Float value)
{
    return CheckAudioSystemCommand(
        android::AudioSystem::setMasterVolume(value));
}

ECode CAudioSystem::GetMasterVolume(
    /* [out] */ Float* result)
{
    VALIDATE_NOT_NULL(result);

    Float value = 0.0;
    if (android::AudioSystem::getMasterVolume(&value) != android::NO_ERROR) {
        value = -1.0;
    }
    *result = value;
    return NOERROR;
}

ECode CAudioSystem::SetMasterMute(
    /* [in] */ Boolean mute)
{
    return CheckAudioSystemCommand(
        android::AudioSystem::setMasterMute(mute));
}

ECode CAudioSystem::GetMasterMute(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    bool mute = FALSE;
    if (android::AudioSystem::getMasterMute(&mute) != android::NO_ERROR) {
        mute = FALSE;
    }
    *result = (Boolean)mute;
    return NOERROR;
}

ECode CAudioSystem::GetDevicesForStream(
    /* [in] */ Int32 stream,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    *result = (Int32)android::AudioSystem::getDevicesForStream(
        static_cast<audio_stream_type_t>(stream));
    return NOERROR;
}

ECode CAudioSystem::GetPrimaryOutputSamplingRate(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    *result = (Int32)android::AudioSystem::getPrimaryOutputSamplingRate();
    return NOERROR;
}

ECode CAudioSystem::GetPrimaryOutputFrameCount(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    *result = (Int32)android::AudioSystem::getPrimaryOutputFrameCount();
    return NOERROR;
}

void CAudioSystem::ErrorCallbackFromNative(
    /* [in] */ Int32 error)
{
    if (DBG) Logger::E("CAudioSystem", "ErrorCallbackFromNative error: %d", error);
    AutoPtr<IAudioSystemErrorCallback> errorCallback;
    {
        Mutex::Autolock lock(sStaticAudioSystemLock);
        if (mErrorCallback != NULL) {
            errorCallback = mErrorCallback;
        }
    }
    if (errorCallback != NULL) {
        errorCallback->OnError(error);
    }
}

ECode CAudioSystem::CheckAudioSystemCommand(Int32 status)
{
    if (status == android::NO_ERROR) {
        return NOERROR;
    } else {
        Logger::E("CAudioSystem", "AudioSystem operator failed status %d", status);
        return E_FAIL;
    }
}

} // namespace Media
} // namepsace Droid
} // namespace Elastos
