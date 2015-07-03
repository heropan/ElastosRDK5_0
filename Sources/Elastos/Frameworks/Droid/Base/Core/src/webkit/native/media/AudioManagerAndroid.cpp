
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Media {

//===============================================================
//             AudioManagerAndroid::NonThreadSafe
//===============================================================

AudioManagerAndroid::NonThreadSafe::NonThreadSafe()
{
    if (DEBUG) {
        Thread::CurrentThread()->GetId(&mThreadId);
    }
    else {
        // Avoids "Unread field" issue reported by findbugs.
        mThreadId = 0L;
    }
}

/**
 * Checks if the method is called on the valid thread.
 * Assigns the current thread if no thread was assigned.
 */
Boolean AudioManagerAndroid::NonThreadSafe::CalledOnValidThread()
{
    if (DEBUG) {
        Int64 id;
        Thread::CurrentThread()->GetId(&id);
        return mThreadId == id
    }

    return TRUE;
}

//===============================================================
//             AudioManagerAndroid::AudioDeviceName
//===============================================================

AudioManagerAndroid::AudioDeviceName::AudioDeviceName(
    /* [in] */ Int32 id,
    /* [in] */ String name)
    : mId(id)
    , mName(name)
{
}

//@CalledByNative("AudioDeviceName")
String AudioManagerAndroid::AudioDeviceName::Id()
{
    return String.valueOf(mId);
}

//@CalledByNative("AudioDeviceName")
String AudioManagerAndroid::AudioDeviceName::Name()
{
    return mName;
}

//===============================================================
//      AudioManagerAndroid::WiredHeadsetBroadcastReceiver
//===============================================================

const Int32 AudioManagerAndroid::WiredHeadsetBroadcastReceiver::STATE_UNPLUGGED;
const Int32 AudioManagerAndroid::WiredHeadsetBroadcastReceiver::STATE_PLUGGED;
const Int32 AudioManagerAndroid::WiredHeadsetBroadcastReceiver::HAS_NO_MIC;
const Int32 AudioManagerAndroid::WiredHeadsetBroadcastReceiver::HAS_MIC;

AudioManagerAndroid::WiredHeadsetBroadcastReceiver::WiredHeadsetBroadcastReceiver(
    /* [in] */ AudioManagerAndroid* owner)
    : mOwner(owner)
{
}

//@Override
ECode AudioManagerAndroid::WiredHeadsetBroadcastReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    VALIDATE_NOT_NULL(context);
    VALIDATE_NOT_NULL(intent);

    Int32 state;
    intent->GetIntExtra(String("state"), STATE_UNPLUGGED, &state);
    if (DEBUG) {
        Int32 microphone;
        intent->GetIntExtra(String("microphone"), HAS_NO_MIC, &microphone);
        String name;
        intent->GetStringExtra(String("name"), &name);
        logd("BroadcastReceiver.onReceive: a=" + intent.getAction() +
            ", s=" + state +
            ", m=" + microphone +
            ", n=" + name +
            ", sb=" + isInitialStickyBroadcast());
    }
    switch (state) {
        case STATE_UNPLUGGED: {
                Object::Autolock lock(mOwner->mLock);
                // Wired headset and earpiece are mutually exclusive.
                (*mAudioDevices)[DEVICE_WIRED_HEADSET] = FALSE;
                if (HasEarpiece()) {
                    (*mAudioDevices)[DEVICE_EARPIECE] = TRUE;
                }
            }
            break;
        case STATE_PLUGGED: {
                Object::Autolock lock(mOwner->mLock);
                // Wired headset and earpiece are mutually exclusive.
                (*mAudioDevices)[DEVICE_WIRED_HEADSET] = TRUE;
                (*mAudioDevices)[DEVICE_EARPIECE] = FALSE;
            }
            break;
        default:
            Loge("Invalid state");
            break;
    }

    // Update the existing device selection, but only if a specific
    // device has already been selected explicitly.
    if (DeviceHasBeenRequested()) {
        UpdateDeviceActivation();
    }
    else if (DEBUG) {
        ReportUpdate();
    }

    return NOERROR;
}

//=========================================================================
//         AudioManagerAndroid::BluetoothHeadsetBroadcastReceiver
//=========================================================================

AudioManagerAndroid::BluetoothHeadsetBroadcastReceiver::BluetoothHeadsetBroadcastReceiver(
    /* [in] */ AudioManagerAndroid* owner)
    : mOwner(owner)
{
}

//@Override
ECode AudioManagerAndroid::BluetoothHeadsetBroadcastReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    VALIDATE_NOT_NULL(context);
    VALIDATE_NOT_NULL(intent);

    // A change in connection state of the Headset profile has
    // been detected, e.g. BT headset has been connected or
    // disconnected. This broadcast is *not* sticky.
    Int32 profileState;
    intent->GetIntExtra(
        android::bluetooth::BluetoothHeadset::EXTRA_STATE,
        android::bluetooth::BluetoothHeadset::STATE_DISCONNECTED,
        &profileState);

    if (DEBUG) {
        logd("BroadcastReceiver.onReceive: a=" + intent.getAction() +
            ", s=" + profileState +
            ", sb=" + isInitialStickyBroadcast());
    }

    switch (profileState) {
        case android::bluetooth::BluetoothProfile::STATE_DISCONNECTED:
            // We do not have to explicitly call stopBluetoothSco()
            // since BT SCO will be disconnected automatically when
            // the BT headset is disabled.
            {
                Object::Autolock lock(mOwner->mLock);
                // Remove the BT device from the list of devices.
                mAudioDevices[DEVICE_BLUETOOTH_HEADSET] = FALSE;
            }
            break;
        case android::bluetooth::BluetoothProfile::STATE_CONNECTED: {
                Object::Autolock lock(mOwner->mLock);
                // Add the BT device to the list of devices.
                mAudioDevices[DEVICE_BLUETOOTH_HEADSET] = TRUE;
            }
            break;
        case android::bluetooth::BluetoothProfile::STATE_CONNECTING:
            // Bluetooth service is switching from off to on.
            break;
        case android::bluetooth::BluetoothProfile::STATE_DISCONNECTING:
            // Bluetooth service is switching from on to off.
            break;
        default:
            Loge("Invalid state");
            break;
    }

    // Update the existing device selection, but only if a specific
    // device has already been selected explicitly.
    if (DeviceHasBeenRequested()) {
        UpdateDeviceActivation();
    }
    else if (DEBUG) {
        ReportUpdate();
    }

    return NOERROR;
}

//===========================================================================
//         AudioManagerAndroid::BluetoothScoIntentBroadcastReceiver
//===========================================================================

AudioManagerAndroid::BluetoothScoIntentBroadcastReceiver::BluetoothScoIntentBroadcastReceiver(
    /* [in] */ AudioManagerAndroid* owner)
    : mOwner(owner)
{
}

//@Override
ECode AudioManagerAndroid::BluetoothScoIntentBroadcastReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    VALIDATE_NOT_NULL(context);
    VALIDATE_NOT_NULL(intent);

    Int32 state;
    intent->GetIntExtra(
        AudioManager::EXTRA_SCO_AUDIO_STATE,
        AudioManager::SCO_AUDIO_STATE_DISCONNECTED,
        &state);

    if (DEBUG) {
        logd("BroadcastReceiver.onReceive: a=" + intent.getAction() +
            ", s=" + state +
            ", sb=" + isInitialStickyBroadcast());
    }

    switch (state) {
        case AudioManager::SCO_AUDIO_STATE_CONNECTED:
            mBluetoothScoState = STATE_BLUETOOTH_SCO_ON;
            break;
        case AudioManager::SCO_AUDIO_STATE_DISCONNECTED:
            mBluetoothScoState = STATE_BLUETOOTH_SCO_OFF;
            break;
        case AudioManager::SCO_AUDIO_STATE_CONNECTING:
            // do nothing
            break;
        default:
            Loge("Invalid state");
    }

    if (DEBUG) {
        ReportUpdate();
    }

    return NOERROR;
}


//===============================================================
//           AudioManagerAndroid::InnerContentObserver
//===============================================================

AudioManagerAndroid::InnerContentObserver::InnerContentObserver(
    /* [in] */ AudioManagerAndroid* owner)
    : mOwner(owner)
{
}

ECode AudioManagerAndroid::InnerContentObserver::OnChange(
    /* [in] */ Boolean selfChange)
{
    if (DEBUG) logd("SettingsObserver.onChange: " + selfChange);
    super.onChange(selfChange);

    // Ensure that the observer is activated during communication mode.
    Int32 mode;
    mAudioManager->GetMode(&mode, &mode);
    if (mode != IAudioManager::MODE_IN_COMMUNICATION) {
        // throw new IllegalStateException(
        //         "Only enable SettingsObserver in COMM mode");
        assert(0);
    }

    // Get stream volume for the voice stream and deliver callback if
    // the volume index is zero. It is not possible to move the volume
    // slider all the way down in communication mode but the callback
    // implementation can ensure that the volume is completely muted.
    Int32 volume;
    mAudioManager->GetStreamVolume(
        IAudioManager::STREAM_VOICE_CALL,
        &volume);
    if (DEBUG) logd("nativeSetMute: " + (volume == 0));
    NativeSetMute(mNativeAudioManagerAndroid, (volume == 0));

    return NOERROR;
}

//===============================================================
//                    AudioManagerAndroid
//===============================================================

template<typename T>
static AutoPtr< ArrayOf<T> > ArrayOf_Init(T array[])
{
    Int32 size = sizeof(T)/sizeof(T[0]);
    AutoPtr< ArrayOf<T> > retArray = ArrayOf<T>::Alloc(size);
    for (Int32 i = 0; i < size; ++i) {
        (*retArray)[i] = array[i]
    }

    return retArray;
}

const String AudioManagerAndroid::TAG("AudioManagerAndroid");
const Boolean AudioManagerAndroid::DEBUG = FALSE;

static String models[] = {
    String("GT-I9300"),  // Galaxy S3
    String("GT-I9500"),  // Galaxy S4
    String("GT-N7105"),  // Galaxy Note 2
    String("Nexus 4"),   // Nexus 4
    String("Nexus 5"),   // Nexus 5
    String("Nexus 7"),   // Nexus 7
    String("SM-N9005"),  // Galaxy Note 3
    String("SM-T310"),   // Galaxy Tab 3 8.0 (WiFi)
};

const AutoPtr< ArrayOf<String> > AudioManagerAndroid::SUPPORTED_AEC_MODELS = ArrayOf_Init(models);

// Supported audio device types.
const Int32 AudioManagerAndroid::DEVICE_DEFAULT;
const Int32 AudioManagerAndroid::DEVICE_INVALID;
const Int32 AudioManagerAndroid::DEVICE_SPEAKERPHONE;
const Int32 AudioManagerAndroid::DEVICE_WIRED_HEADSET;
const Int32 AudioManagerAndroid::DEVICE_EARPIECE;
const Int32 AudioManagerAndroid::DEVICE_BLUETOOTH_HEADSET;
const Int32 AudioManagerAndroid::DEVICE_COUNT;

// Maps audio device types to string values. This map must be in sync
// with the device types above.
// TODO(henrika): add support for proper detection of device names and
// localize the name strings by using resource strings.
// See http://crbug.com/333208 for details.
static String names[] = {
    String("Speakerphone"),
    String("Wired headset"),      // With or without microphone.
    String("Headset earpiece"),   // Only available on mobile phones.
    String("Bluetooth headset"),  // Requires BLUETOOTH permission.
};

const AutoPtr< ArrayOf<String> > AudioManagerAndroid::DEVICE_NAMES = ArrayOf_Init(names);

static Int32 devices[] = {
    DEVICE_SPEAKERPHONE,
    DEVICE_WIRED_HEADSET,
    DEVICE_EARPIECE,
    DEVICE_BLUETOOTH_HEADSET,
};

// List of valid device types.
const AutoPtr< ArrayOf<Int32> > AudioManagerAndroid::VALID_DEVICES = ArrayOf_Init(devices);

// Bluetooth audio SCO states. Example of valid state sequence:
// SCO_INVALID -> SCO_TURNING_ON -> SCO_ON -> SCO_TURNING_OFF -> SCO_OFF.
const Int32 AudioManagerAndroid::STATE_BLUETOOTH_SCO_INVALID;
const Int32 AudioManagerAndroid::STATE_BLUETOOTH_SCO_OFF;
const Int32 AudioManagerAndroid::STATE_BLUETOOTH_SCO_ON;
const Int32 AudioManagerAndroid::STATE_BLUETOOTH_SCO_TURNING_ON;
const Int32 AudioManagerAndroid::STATE_BLUETOOTH_SCO_TURNING_OFF;

// Use 44.1kHz as the default sampling rate.
const Int32 AudioManagerAndroid::DEFAULT_SAMPLING_RATE;
// Randomly picked up frame size which is close to return value on N4.
// Return this value when getProperty(PROPERTY_OUTPUT_FRAMES_PER_BUFFER)
// fails.
const Int32 AudioManagerAndroid::DEFAULT_FRAME_PER_BUFFER;

static AutoPtr< ArrayOf<Boolean> > mAudioDevices_Init()
{
    AutoPtr< ArrayOf<Boolean> > array = ArrayOf<Boolean>::Alloc(DEVICE_COUNT);
    for (Int32 i = 0; i < DEVICE_COUNT; ++i) {
        (*array)[i] = FALSE;
    }

    return array;
}

AudioManagerAndroid::AudioManagerAndroid(
    /* [in] */ IContext* context,
    /* [in] */ Int64 nativeAudioManagerAndroid)
    : mContext(context)
    , mNativeAudioManagerAndroid(nativeAudioManagerAndroid)
    , mHasModifyAudioSettingsPermission(FALSE)
    , mHasRecordAudioPermission(FALSE)
    , mHasBluetoothPermission(FALSE)
    , mSavedAudioMode(IAudioManager::MODE_INVALID)
    , mBluetoothScoState(STATE_BLUETOOTH_SCO_INVALID)
    , mIsInitialized(FALSE)
    , mSavedIsSpeakerphoneOn(FALSE)
    , mSavedIsMicrophoneMute(FALSE)
    , mRequestedAudioDevice(DEVICE_INVALID)
    , mAudioDevices(mAudioDevices_Init())
    , mCurrentVolume(0)
{
    mContext->GetSystemService(IContext::AUDIO_SERVICE, (IInterface**)&mAudioManager);
    mContext->GetContentResolver((IContentObserver**)&mContentResolver);
}

Boolean AudioManagerAndroid::RunningOnJellyBeanOrHigher()
{
    return Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN;
}

Boolean AudioManagerAndroid::RunningOnJellyBeanMR1OrHigher()
{
    return Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN_MR1;
}

Boolean AudioManagerAndroid::RunningOnJellyBeanMR2OrHigher()
{
    return Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN_MR2;
}

/** Construction */
//@CalledByNative
AutoPtr<AudioManagerAndroid> AudioManagerAndroid::CreateAudioManagerAndroid(
    /* [in] */ IContext* context,
    /* [in] */ Int64 nativeAudioManagerAndroid)
{
    AutoPtr<AudioManagerAndroid> ret = AudioManagerAndroid(context, nativeAudioManagerAndroid);
    return ret;
}

/**
 * Saves the initial speakerphone and microphone state.
 * Populates the list of available audio devices and registers receivers
 * for broadcast intents related to wired headset and Bluetooth devices.
 */
//@CalledByNative
void AudioManagerAndroid::Init()
{
    CheckIfCalledOnValidThread();

    if (DEBUG) {
        Logd("init");
    }

    if (DEBUG)  {
        LogDeviceInfo();
    }

    if (mIsInitialized) {
        return;
    }

    // Check if process has MODIFY_AUDIO_SETTINGS and RECORD_AUDIO
    // permissions. Both are required for full functionality.
    mHasModifyAudioSettingsPermission = HasPermission(
            android::Manifest::permission::MODIFY_AUDIO_SETTINGS);
    if (DEBUG && !mHasModifyAudioSettingsPermission) {
        Logd("MODIFY_AUDIO_SETTINGS permission is missing");
    }
    mHasRecordAudioPermission = HasPermission(
            android::Manifest::permission::RECORD_AUDIO);
    if (DEBUG && !mHasRecordAudioPermission) {
        Logd("RECORD_AUDIO permission is missing");
    }

    // Initialize audio device list with things we know is always available.
    mAudioDevices[DEVICE_EARPIECE] = HasEarpiece();
    mAudioDevices[DEVICE_WIRED_HEADSET] = HasWiredHeadset();
    mAudioDevices[DEVICE_SPEAKERPHONE] = TRUE;

    // Register receivers for broadcast intents related to Bluetooth device
    // and Bluetooth SCO notifications. Requires BLUETOOTH permission.
    RegisterBluetoothIntentsIfNeeded();

    // Register receiver for broadcast intents related to adding/
    // removing a wired headset (Intent.ACTION_HEADSET_PLUG).
    RegisterForWiredHeadsetIntentBroadcast();

    mIsInitialized = TRUE;

    if (DEBUG) {
        ReportUpdate();
    }
}

/**
 * Unregister all previously registered intent receivers and restore
 * the stored state (stored in {@link #init()}).
 */
//@CalledByNative
void AudioManagerAndroid::Close()
{
    CheckIfCalledOnValidThread();
    if (DEBUG) Logd("close");
    if (!mIsInitialized)
        return;

    StopObservingVolumeChanges();
    UnregisterForWiredHeadsetIntentBroadcast();
    UnregisterBluetoothIntentsIfNeeded();

    mIsInitialized = FALSE;
}

/**
 * Saves current audio mode and sets audio mode to MODE_IN_COMMUNICATION
 * if input parameter is true. Restores saved audio mode if input parameter
 * is false.
 * Required permission: android.Manifest.permission.MODIFY_AUDIO_SETTINGS.
 */
//@CalledByNative
void AudioManagerAndroid::SetCommunicationAudioModeOn(boolean on)
{
    if (DEBUG) {
        Logd("setCommunicationAudioModeOn(" + on + ")");
    }

    // The MODIFY_AUDIO_SETTINGS permission is required to allow an
    // application to modify global audio settings.
    if (!mHasModifyAudioSettingsPermission) {
//        Log.w(TAG, "MODIFY_AUDIO_SETTINGS is missing => client will run " +
//                "with reduced functionality");
        return;
    }

    if (on) {
        if (mSavedAudioMode != IAudioManager::MODE_INVALID) {
            // throw new IllegalStateException("Audio mode has already been set");
            assert(0);
        }

        // Store the current audio mode the first time we try to
        // switch to communication mode.
        // try {
            mAudioManager->GetMode(&mSavedAudioMode);
        // } catch (SecurityException e) {
        //     logDeviceInfo();
        //     throw e;

        // }

        // Store microphone mute state and speakerphone state so it can
        // be restored when closing.
        mAudioManager->IsSpeakerphoneOn(&mSavedIsSpeakerphoneOn);
        mAudioManager->IsMicrophoneMute(&mSavedIsMicrophoneMute);

        // try {
            mAudioManager->SetMode(IAudioManager::MODE_IN_COMMUNICATION);
        // } catch (SecurityException e) {
        //     logDeviceInfo();
        //     throw e;
        // }

        // Start observing volume changes to detect when the
        // voice/communication stream volume is at its lowest level.
        // It is only possible to pull down the volume slider to about 20%
        // of the absolute minimum (slider at far left) in communication
        // mode but we want to be able to mute it completely.
        StartObservingVolumeChanges();
    }
    else {
        if (mSavedAudioMode == IAudioManager::MODE_INVALID) {
            // throw new IllegalStateException("Audio mode has not yet been set");
            assert(0);
        }

        StopObservingVolumeChanges();

        // Restore previously stored audio states.
        SetMicrophoneMute(mSavedIsMicrophoneMute);
        SetSpeakerphoneOn(mSavedIsSpeakerphoneOn);

        // Restore the mode that was used before we switched to
        // communication mode.
        // try {
            mAudioManager->SetMode(mSavedAudioMode);
        // } catch (SecurityException e) {
        //     logDeviceInfo();
        //     throw e;
        // }
        mSavedAudioMode = IAudioManager::MODE_INVALID;
    }
}

/**
 * Activates, i.e., starts routing audio to, the specified audio device.
 *
 * @param deviceId Unique device ID (integer converted to string)
 * representing the selected device. This string is empty if the so-called
 * default device is requested.
 * Required permissions: android.Manifest.permission.MODIFY_AUDIO_SETTINGS
 * and android.Manifest.permission.RECORD_AUDIO.
 */
//@CalledByNative
Boolean AudioManagerAndroid::SetDevice(
    /* [in] */ String deviceId)
{
    if (DEBUG) Logd("setDevice: " + deviceId);
    if (!mIsInitialized)
        return FALSE;
    if (!mHasModifyAudioSettingsPermission || !mHasRecordAudioPermission) {
//        Log.w(TAG, "Requires MODIFY_AUDIO_SETTINGS and RECORD_AUDIO");
//        Log.w(TAG, "Selected device will not be available for recording");
        return FALSE;
    }

    Int32 intDeviceId = deviceId.isEmpty() ?
        DEVICE_DEFAULT : Integer.parseInt(deviceId);

    if (intDeviceId == DEVICE_DEFAULT) {
        AutoPtr< ArrayOf<Boolean> > devices;
        {
            Object::Autolock lock(mLock);
            devices = mAudioDevices->Clone();
            mRequestedAudioDevice = DEVICE_DEFAULT;
        }
        Int32 defaultDevice = SelectDefaultDevice(devices);
        SetAudioDevice(defaultDevice);
        return TRUE;
    }

    // A non-default device is specified. Verify that it is valid
    // device, and if so, start using it.
    List<Integer> validIds = Arrays.asList(VALID_DEVICES);
    if (!validIds.contains(intDeviceId) || !mAudioDevices[intDeviceId]) {
        return FALSE;
    }

    {
        Object::Autolock lock(mLock);
        mRequestedAudioDevice = intDeviceId;
    }

    SetAudioDevice(intDeviceId);

    return TRUE;
}

/**
 * @return the current list of available audio devices.
 * Note that this call does not trigger any update of the list of devices,
 * it only copies the current state in to the output array.
 * Required permissions: android.Manifest.permission.MODIFY_AUDIO_SETTINGS
 * and android.Manifest.permission.RECORD_AUDIO.
 */
//@CalledByNative
AutoPtr< ArrayOf<AudioDeviceName> > AudioManagerAndroid::GetAudioInputDeviceNames()
{
    if (DEBUG) Logd("getAudioInputDeviceNames");
    if (!mIsInitialized)
        return NULL;
    if (!mHasModifyAudioSettingsPermission || !mHasRecordAudioPermission) {
//        Log.w(TAG, "Requires MODIFY_AUDIO_SETTINGS and RECORD_AUDIO");
//        Log.w(TAG, "No audio device will be available for recording");
        return NULL;
    }

    AutoPtr< ArrayOf<Boolean> > devices;
    {
        Object::Autolock lock(mLock);
        devices = mAudioDevices->Clone();
    }
    List<String> list = new ArrayList<String>();
    AutoPtr< ArrayOf<AudioDeviceName> > array = ArrayOf<AudioDeviceName>::Alloc(GetNumOfAudioDevices(devices));
    Int32 i = 0;
    for (Int32 id = 0; id < DEVICE_COUNT; ++id) {
        if ((*devices)[id]) {
            (*array)[i] = new AudioDeviceName(id, DEVICE_NAMES[id]);
            list.add(DEVICE_NAMES[id]);
            i++;
        }
    }

    if (DEBUG) Logd("getAudioInputDeviceNames: " + list);

    return array;
}

//@CalledByNative
Int32 AudioManagerAndroid::GetNativeOutputSampleRate()
{
    if (RunningOnJellyBeanMR1OrHigher()) {
        String sampleRateString;
        mAudioManager->GetProperty(
                IAudioManager::PROPERTY_OUTPUT_SAMPLE_RATE, &sampleRateString);
        return (sampleRateString == NULL ?
                DEFAULT_SAMPLING_RATE : Integer.parseInt(sampleRateString));
    }
    else {
        return DEFAULT_SAMPLING_RATE;
    }
}

/**
* Returns the minimum frame size required for audio input.
*
* @param sampleRate sampling rate
* @param channels number of channels
*/
//@CalledByNative
Int32 AudioManagerAndroid::GetMinInputFrameSize(
    /* [in] */ Int32 sampleRate,
    /* [in] */ Int32 channels)
{
    Int32 channelConfig;
    if (channels == 1) {
        channelConfig = IAudioFormat::CHANNEL_IN_MONO;
    }
    else if (channels == 2) {
        channelConfig = IAudioFormat::CHANNEL_IN_STEREO;
    }
    else {
        return -1;
    }

    AutoPtr<IAudioRecordHelper> helper;
    CAudioRecordHelper::AcquireSingleton((IAudioRecordHelper**)&helper);
    Int32 result;
    helper->GetMinBufferSize(
            sampleRate, channelConfig, IAudioFormat::ENCODING_PCM_16BIT, &result);
    result = result / 2 / channels;

    return result;
}

/**
* Returns the minimum frame size required for audio output.
*
* @param sampleRate sampling rate
* @param channels number of channels
*/
//@CalledByNative
Int32 AudioManagerAndroid::GetMinOutputFrameSize(
    /* [in] */ Int32 sampleRate,
    /* [in] */ Int32 channels)
{
    Int32 channelConfig;
    if (channels == 1) {
        channelConfig = IAudioFormat::CHANNEL_OUT_MONO;
    }
    else if (channels == 2) {
        channelConfig = IAudioFormat::CHANNEL_OUT_STEREO;
    }
    else {
        return -1;
    }

    return /*AudioTrack.*/GetMinBufferSize(
            sampleRate, channelConfig, AudioFormat.ENCODING_PCM_16BIT) / 2 / channels;
}

//@CalledByNative
Boolean AudioManagerAndroid::IsAudioLowLatencySupported()
{
    AutoPtr<IPackageManager> pm;
    mContext->GetPackageManager((IPackageManager**)&pm);
    Boolean result = FALSE;
    pm->HasSystemFeature(
            IPackageManager::FEATURE_AUDIO_LOW_LATENCY, &result);
    return result;
}

//@CalledByNative
Int32 AudioManagerAndroid::GetAudioLowLatencyOutputFrameSize()
{
    String framesPerBuffer;
    mAudioManager->GetProperty(IAudioManager::PROPERTY_OUTPUT_FRAMES_PER_BUFFER, &framesPerBuffer);
    return (framesPerBuffer == NULL ?
            DEFAULT_FRAME_PER_BUFFER : Integer.parseInt(framesPerBuffer));
}

//@CalledByNative
Boolean AudioManagerAndroid::ShouldUseAcousticEchoCanceler()
{
    // AcousticEchoCanceler was added in API level 16 (Jelly Bean).
    if (!RunningOnJellyBeanOrHigher()) {
        return FALSE;
    }

    // Verify that this device is among the supported/tested models.
    List<String> supportedModels = Arrays.asList(SUPPORTED_AEC_MODELS);
    if (!supportedModels.contains(Build.MODEL)) {
        return FALSE;
    }

    AutoPtr<IAcousticEchoCancelerHelper> helper;
    CAcousticEchoCancelerHelper::AcquireSingleton((IAcousticEchoCancelerHelper**)&helper);
    Boolean isAvailable = FALSE;
    helper->IsAvailable(&isAvailable);
    if (DEBUG && isAvailable) {
        Logd("Approved for use of hardware acoustic echo canceler.");
    }

    // As a final check, verify that the device supports acoustic echo
    // cancellation.
    return isAvailable;
}

/**
 * Helper method for debugging purposes. Ensures that method is
 * called on same thread as this object was created on.
 */
void AudioManagerAndroid::CheckIfCalledOnValidThread()
{
    if (DEBUG && !mNonThreadSafe->CalledOnValidThread()) {
        // throw new IllegalStateException("Method is not called on valid thread");
        assert(0);
    }
}

/**
 * Register for BT intents if we have the BLUETOOTH permission.
 * Also extends the list of available devices with a BT device if one exists.
 */
void AudioManagerAndroid::RegisterBluetoothIntentsIfNeeded()
{
    // Check if this process has the BLUETOOTH permission or not.
    mHasBluetoothPermission = HasPermission(
            android::Manifest::permission::BLUETOOTH);

    // Add a Bluetooth headset to the list of available devices if a BT
    // headset is detected and if we have the BLUETOOTH permission.
    // We must do this initial check using a dedicated method since the
    // broadcasted intent BluetoothHeadset.ACTION_CONNECTION_STATE_CHANGED
    // is not sticky and will only be received if a BT headset is connected
    // after this method has been called.
    if (!mHasBluetoothPermission) {
//        Log.w(TAG, "Requires BLUETOOTH permission");
        return;
    }
    (*mAudioDevices)[DEVICE_BLUETOOTH_HEADSET] = HasBluetoothHeadset();

    // Register receivers for broadcast intents related to changes in
    // Bluetooth headset availability and usage of the SCO channel.
    RegisterForBluetoothHeadsetIntentBroadcast();
    RegisterForBluetoothScoIntentBroadcast();
}

/** Unregister for BT intents if a registration has been made. */
void AudioManagerAndroid::UnregisterBluetoothIntentsIfNeeded()
{
    if (mHasBluetoothPermission) {
        mAudioManager->StopBluetoothSco();
        UnregisterForBluetoothHeadsetIntentBroadcast();
        UnregisterForBluetoothScoIntentBroadcast();
    }
}

/** Sets the speaker phone mode. */
void AudioManagerAndroid::SetSpeakerphoneOn(
    /* [in] */ Boolean on)
{
    Boolean wasOn;
    mAudioManager->IsSpeakerphoneOn(&wasOn);
    if (wasOn == on) {
        return;
    }

    mAudioManager->SetSpeakerphoneOn(on);
}

/** Sets the microphone mute state. */
void AudioManagerAndroid::SetMicrophoneMute(
    /* [in] */ Boolean on)
{
    Boolean wasMuted;
    mAudioManager->IsMicrophoneMute(&wasMuted);
    if (wasMuted == on) {
        return;
    }

    mAudioManager->SetMicrophoneMute(on);
}

/** Gets  the current microphone mute state. */
Boolean AudioManagerAndroid::IsMicrophoneMute()
{
    Boolean result = FALSE;
    mAudioManager->IsMicrophoneMute(&result);
    return result;
}

/** Gets the current earpiece state. */
Boolean AudioManagerAndroid::HasEarpiece()
{
    AutoPtr<IPackageManager> pm;
    mContext->GetPackageManager((IPackageManager**)&pm);
    Boolean result = FALSE;
    pm->HasSystemFeature(
        IPackageManager::FEATURE_TELEPHONY, &result);
    return result;
}

/**
 * Checks whether a wired headset is connected or not.
 * This is not a valid indication that audio playback is actually over
 * the wired headset as audio routing depends on other conditions. We
 * only use it as an early indicator (during initialization) of an attached
 * wired headset.
 */
//@Deprecated
Boolean AudioManagerAndroid::HasWiredHeadset()
{
    Boolean result = FALSE;
    mAudioManager->IsWiredHeadsetOn(&result);
    return result;
}

/** Checks if the process has as specified permission or not. */
Boolean AudioManagerAndroid::HasPermission(
    /* [in] */ String permission)
{
    Boolean result = FALSE;
    Int32 permission;
    mContext->CheckPermission(
            permission,
            Process::MyPid(),
            Process::MyUid(), permission);

    return permission == IPackageManager::PERMISSION_GRANTED;
}

/**
 * Gets the current Bluetooth headset state.
 * android.bluetooth.BluetoothAdapter.getProfileConnectionState() requires
 * the BLUETOOTH permission.
 */
Boolean AudioManagerAndroid::HasBluetoothHeadset()
{
    if (!mHasBluetoothPermission) {
//        Log.w(TAG, "hasBluetoothHeadset() requires BLUETOOTH permission");
        return FALSE;
    }

    // To get a BluetoothAdapter representing the local Bluetooth adapter,
    // when running on JELLY_BEAN_MR1 (4.2) and below, call the static
    // getDefaultAdapter() method; when running on JELLY_BEAN_MR2 (4.3) and
    // higher, retrieve it through getSystemService(String) with
    // BLUETOOTH_SERVICE.
    AutoPtr<IBluetoothAdapter> btAdapter;
    if (RunningOnJellyBeanMR2OrHigher()) {
        // Use BluetoothManager to get the BluetoothAdapter for
        // Android 4.3 and above.
        AutoPtr<IBluetoothManager> btManager;
        mContext->GetSystemService(IContext::BLUETOOTH_SERVICE, (IInterface**)&btManager);
        btManager->GetAdapter((IBluetoothManager**)&btAdapter);
    }
    else {
        // Use static method for Android 4.2 and below to get the
        // BluetoothAdapter.
        AutoPtr<IBluetoothAdapterHelper> helper;
        CBluetoothAdapterHelper::AcquireSingleton((IBluetoothAdapterHelper**)&helper);
        helper->GetDefaultAdapter((IBluetoothAdapter**)&btAdapter);
    }

    if (btAdapter == NULL) {
        // Bluetooth not supported on this platform.
        return FALSE;
    }

    Int32 profileConnectionState;
    btAdapter->GetProfileConnectionState(
            android::bluetooth::BluetoothProfile::HEADSET, &profileConnectionState);

    // Ensure that Bluetooth is enabled and that a device which supports the
    // headset and handsfree profile is connected.
    // TODO(henrika): it is possible that btAdapter.isEnabled() is
    // redundant. It might be sufficient to only check the profile state.
    Boolean isEnabled = FALSE;
    btAdapter->IsEnabled(&isEnabled);
    return isEnabled && profileConnectionState ==
        android::bluetooth::BluetoothProfile::STATE_CONNECTED;
}

/**
 * Registers receiver for the broadcasted intent when a wired headset is
 * plugged in or unplugged. The received intent will have an extra
 * 'state' value where 0 means unplugged, and 1 means plugged.
 */
void AudioManagerAndroid::RegisterForWiredHeadsetIntentBroadcast()
{
    AutoPtr<IIntentFilter> filter;
    CIntentFilter::New(IIntent::ACTION_HEADSET_PLUG, (IIntentFilter**)&filter);

    /** Receiver which handles changes in wired headset availability. */
    mWiredHeadsetReceiver = new WiredHeadsetBroadcastReceiver(this);

    // Note: the intent we register for here is sticky, so it'll tell us
    // immediately what the last action was (plugged or unplugged).
    // It will enable us to set the speakerphone correctly.
    mContext->RegisterReceiver(mWiredHeadsetReceiver, filter);
}

/** Unregister receiver for broadcasted ACTION_HEADSET_PLUG intent. */
void AudioManagerAndroid::UnregisterForWiredHeadsetIntentBroadcast()
{
    mContext->UnregisterReceiver(mWiredHeadsetReceiver);
    mWiredHeadsetReceiver = NULL;
}

/**
 * Registers receiver for the broadcasted intent related to BT headset
 * availability or a change in connection state of the local Bluetooth
 * adapter. Example: triggers when the BT device is turned on or off.
 * BLUETOOTH permission is required to receive this one.
 */
void AudioManagerAndroid::RegisterForBluetoothHeadsetIntentBroadcast()
{
    AutoPtr<IIntentFilter> filter;
    CIntentFilter::New(
        android::bluetooth::BluetoothHeadset::ACTION_CONNECTION_STATE_CHANGED,
        (IIntentFilter**)&filter);

    /** Receiver which handles changes in BT headset availability. */
    mBluetoothHeadsetReceiver = new BluetoothHeadsetBroadcastReceiver(this);

    mContext->RegisterReceiver(mBluetoothHeadsetReceiver, filter);
}

void AudioManagerAndroid::UnregisterForBluetoothHeadsetIntentBroadcast()
{
    mContext->UnregisterReceiver(mBluetoothHeadsetReceiver);
    mBluetoothHeadsetReceiver = NULL;
}

/**
 * Registers receiver for the broadcasted intent related the existence
 * of a BT SCO channel. Indicates if BT SCO streaming is on or off.
 */
void AudioManagerAndroid::RegisterForBluetoothScoIntentBroadcast()
{
    AutoPtr<IIntentFilter> filter;
    CIntentFilter::New(
        AudioManager::ACTION_SCO_AUDIO_STATE_UPDATED,
        (IIntentFilter**)&filter);

    /** BroadcastReceiver implementation which handles changes in BT SCO. */
    mBluetoothScoReceiver = new BluetoothScoIntentBroadcastReceiver(this);

    mContext->RegisterReceiver(mBluetoothScoReceiver, filter);
}

void AudioManagerAndroid::UnregisterForBluetoothScoIntentBroadcast()
{
    mContext->UnregisterReceiver(mBluetoothScoReceiver);
    mBluetoothScoReceiver = NULL;
}

/** Enables BT audio using the SCO audio channel. */
void AudioManagerAndroid::StartBluetoothSco()
{
    if (!mHasBluetoothPermission) {
        return;
    }

    if (mBluetoothScoState == STATE_BLUETOOTH_SCO_ON ||
        mBluetoothScoState == STATE_BLUETOOTH_SCO_TURNING_ON) {
        // Unable to turn on BT in this state.
        return;
    }

    // Check if audio is already routed to BT SCO; if so, just update
    // states but don't try to enable it again.
    Boolean isBluetoothScoOn;
    mAudioManager->IsBluetoothScoOn(&isBluetoothScoOn);
    if (isBluetoothScoOn) {
        mBluetoothScoState = STATE_BLUETOOTH_SCO_ON;
        return;
    }

    if (DEBUG) logd("startBluetoothSco: turning BT SCO on...");
    mBluetoothScoState = STATE_BLUETOOTH_SCO_TURNING_ON;
    mAudioManager->StartBluetoothSco();
}

/** Disables BT audio using the SCO audio channel. */
void AudioManagerAndroid::StopBluetoothSco()
{
    if (!mHasBluetoothPermission) {
        return;
    }

    if (mBluetoothScoState != STATE_BLUETOOTH_SCO_ON &&
        mBluetoothScoState != STATE_BLUETOOTH_SCO_TURNING_ON) {
        // No need to turn off BT in this state.
        return;
    }

    Boolean isBluetoothScoOn = FALSE;
    mAudioManager->IsBluetoothScoOn(&isBluetoothScoOn);
    if (!isBluetoothScoOn) {
        // TODO(henrika): can we do anything else than logging here?
        loge("Unable to stop BT SCO since it is already disabled");
        return;
    }

    if (DEBUG) logd("stopBluetoothSco: turning BT SCO off...");
    mBluetoothScoState = STATE_BLUETOOTH_SCO_TURNING_OFF;
    mAudioManager->StopBluetoothSco();
}

/**
 * Changes selection of the currently active audio device.
 *
 * @param device Specifies the selected audio device.
 */
void AudioManagerAndroid::SetAudioDevice(
    /* [in] */ Int32 device)
{
    if (DEBUG) logd("setAudioDevice(device=" + device + ")");

    // Ensure that the Bluetooth SCO audio channel is always disabled
    // unless the BT headset device is selected.
    if (device == DEVICE_BLUETOOTH_HEADSET) {
        StartBluetoothSco();
    }
    else {
        StopBluetoothSco();
    }

    switch (device) {
        case DEVICE_BLUETOOTH_HEADSET:
            break;
        case DEVICE_SPEAKERPHONE:
            SetSpeakerphoneOn(TRUE);
            break;
        case DEVICE_WIRED_HEADSET:
            SetSpeakerphoneOn(FALSE);
            break;
        case DEVICE_EARPIECE:
            SetSpeakerphoneOn(FALSE);
            break;
        default:
            loge("Invalid audio device selection");
            break;
    }

    ReportUpdate();
}

/**
 * Use a special selection scheme if the default device is selected.
 * The "most unique" device will be selected; Wired headset first,
 * then Bluetooth and last the speaker phone.
 */
Int32 AudioManagerAndroid::SelectDefaultDevice(
    /* [in] */ ArrayOf<Boolean>* devices)
{
    if ((*devices)[DEVICE_WIRED_HEADSET]) {
        return DEVICE_WIRED_HEADSET;
    }
    else if ((*devices)[DEVICE_BLUETOOTH_HEADSET]) {
        // TODO(henrika): possibly need improvements here if we are
        // in a state where Bluetooth is turning off.
        return DEVICE_BLUETOOTH_HEADSET;
    }

    return DEVICE_SPEAKERPHONE;
}

/** Returns true if setDevice() has been called with a valid device id. */
Boolean AudioManagerAndroid::DeviceHasBeenRequested()
{
    Object::Autolock lock(mLock);
    return (mRequestedAudioDevice != DEVICE_INVALID);
}

/**
 * Updates the active device given the current list of devices and
 * information about if a specific device has been selected or if
 * the default device is selected.
 */
void AudioManagerAndroid::UpdateDeviceActivation()
{
    AutoPtr< ArrayOf<Boolean> > devices;
    Int32 requested = DEVICE_INVALID;

    {
        Object::Autolock lock(mLock);
        requested = mRequestedAudioDevice;
        devices = mAudioDevices->Clone();
    }

    if (requested == DEVICE_INVALID) {
        loge("Unable to activate device since no device is selected");
        return;
    }

    // Update default device if it has been selected explicitly, or
    // the selected device has been removed from the list.
    if (requested == DEVICE_DEFAULT || !(*devices)[requested]) {
        // Get default device given current list and activate the device.
        Int32 defaultDevice = SelectDefaultDevice(devices);
        SetAudioDevice(defaultDevice);
    }
    else {
        // Activate the selected device since we know that it exists in
        // the list.
        SetAudioDevice(requested);
    }
}

/** Returns number of available devices */
Int32 AudioManagerAndroid::GetNumOfAudioDevices(
    /* [in] */ ArrayOf<Boolean>* devices)
{
    Int32 count = 0;
    for (Int32 i = 0; i < DEVICE_COUNT; ++i) {
        if ((*devices)[i])
            ++count;
    }

    return count;
}

/**
 * For now, just log the state change but the idea is that we should
 * notify a registered state change listener (if any) that there has
 * been a change in the state.
 * TODO(henrika): add support for state change listener.
 */
void AudioManagerAndroid::ReportUpdate()
{
    Object::Autolock lock(mLock);
    List<String> devices = new ArrayList<String>();
    for (Int32 i = 0; i < DEVICE_COUNT; ++i) {
        if (mAudioDevices[i])
            devices.add(DEVICE_NAMES[i]);
    }

    if (DEBUG) {
        logd("reportUpdate: requested=" + mRequestedAudioDevice +
            ", btSco=" + mBluetoothScoState +
            ", devices=" + devices);
    }
}

/** Information about the current build, taken from system properties. */
void AudioManagerAndroid::LogDeviceInfo()
{
    logd("Android SDK: " + Build.VERSION.SDK_INT + ", " +
        "Release: " + Build.VERSION.RELEASE + ", " +
        "Brand: " + Build.BRAND + ", " +
        "CPU_ABI: " + Build.CPU_ABI + ", " +
        "Device: " + Build.DEVICE + ", " +
        "Id: " + Build.ID + ", " +
        "Hardware: " + Build.HARDWARE + ", " +
        "Manufacturer: " + Build.MANUFACTURER + ", " +
        "Model: " + Build.MODEL + ", " +
        "Product: " + Build.PRODUCT);
}

/** Trivial helper method for debug logging */
void AudioManagerAndroid::Logd(
    /* [in] */ String msg)
{
//    Log.d(TAG, msg);
}

/** Trivial helper method for error logging */
void AudioManagerAndroid::Loge(
    /* [in] */ String msg)
{
//    Log.e(TAG, msg);
}

/** Start thread which observes volume changes on the voice stream. */
void AudioManagerAndroid::StartObservingVolumeChanges()
{
    if (DEBUG) logd("startObservingVolumeChanges");
    if (mSettingsObserverThread != NULL)
        return;

    CHandlerThread::New(String("SettingsObserver"), (IHandlerThread**)&mSettingsObserverThread);
    mSettingsObserverThread->Start();

    mSettingsObserver = new ContentObserver(
        new Handler(mSettingsObserverThread.getLooper())) {

            @Override
            public void onChange(boolean selfChange) {
                if (DEBUG) logd("SettingsObserver.onChange: " + selfChange);
                super.onChange(selfChange);

                // Ensure that the observer is activated during communication mode.
                if (mAudioManager.getMode() != AudioManager.MODE_IN_COMMUNICATION) {
                    throw new IllegalStateException(
                            "Only enable SettingsObserver in COMM mode");
                }

                // Get stream volume for the voice stream and deliver callback if
                // the volume index is zero. It is not possible to move the volume
                // slider all the way down in communication mode but the callback
                // implementation can ensure that the volume is completely muted.
                int volume = mAudioManager.getStreamVolume(
                    AudioManager.STREAM_VOICE_CALL);
                if (DEBUG) logd("nativeSetMute: " + (volume == 0));
                nativeSetMute(mNativeAudioManagerAndroid, (volume == 0));
            }
    };

    mContentResolver->RegisterContentObserver(
        Settings.System.CONTENT_URI, TRUE, mSettingsObserver);
}

/** Quit observer thread and stop listening for volume changes. */
void AudioManagerAndroid::StopObservingVolumeChanges()
{
    if (DEBUG) logd("stopObservingVolumeChanges");
    if (mSettingsObserverThread == NULL)
        return;

    mContentResolver->UnregisterContentObserver(mSettingsObserver);
    mSettingsObserver = NULL;

    mSettingsObserverThread->Quit();
    // try {
        mSettingsObserverThread->Join();
    // } catch (InterruptedException e) {
    //     Log.e(TAG, "Thread.join() exception: ", e);
    // }
    mSettingsObserverThread = NULL;
}

void AudioManagerAndroid::NativeSetMute(
    /* [in] */ Int64 nativeAudioManagerAndroid,
    /* [in] */ Boolean muted)
{
}

} // namespace Media
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
