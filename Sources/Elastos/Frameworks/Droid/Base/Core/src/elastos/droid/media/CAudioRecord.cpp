
#include "elastos/droid/media/CAudioRecord.h"
#include "elastos/droid/media/CMediaRecorder.h"
#include "elastos/droid/media/CMediaRecorderHelper.h"
#include <media/AudioRecord.h>
#include "elastos/droid/os/CLooperHelper.h"
#include "elastos/droid/os/Process.h"
#include "elastos/droid/os/ServiceManager.h"
#include "elastos/droid/privacy/CPrivacySettingsManager.h"
#include <elastos/core/StringUtils.h>
#include <elastos/core/StringBuilder.h>
#include <elastos/utility/logging/Logger.h>

using Elastos::Droid::Os::ServiceManager;
using Elastos::Droid::Os::EIID_IHandler;
using Elastos::Droid::Os::ILooperHelper;
using Elastos::Droid::Os::CLooperHelper;
using Elastos::Droid::Os::Process;
using Elastos::Droid::Privacy::IPrivacySettings;
using Elastos::Droid::Privacy::CPrivacySettingsManager;
using Elastos::Droid::Privacy::IIPrivacySettingsManager;
using Elastos::Utility::Logging::Logger;
using Elastos::Core::StringBuilder;
using Elastos::Core::StringUtils;


namespace Elastos {
namespace Droid {
namespace Media {

#define AUDIORECORD_SUCCESS                          0
#define AUDIORECORD_ERROR                           -1
#define AUDIORECORD_ERROR_BAD_VALUE                 -2
#define AUDIORECORD_ERROR_INVALID_OPERATION         -3
#define AUDIORECORD_ERROR_SETUP_ZEROFRAMECOUNT      -16
#define AUDIORECORD_ERROR_SETUP_INVALIDCHANNELMASK  -17
#define AUDIORECORD_ERROR_SETUP_INVALIDFORMAT       -18
#define AUDIORECORD_ERROR_SETUP_INVALIDSOURCE       -19
#define AUDIORECORD_ERROR_SETUP_NATIVEINITFAILED    -20

const String CAudioRecord::TAG("CAudioRecord");
const String CAudioRecord::PRIVACY_TAG("PM,AudioRecord");

const Int32 CAudioRecord::NATIVE_EVENT_MARKER = 2;
const Int32 CAudioRecord::NATIVE_EVENT_NEW_POS = 3;
const Int32 CAudioRecord::IS_ALLOWED = -1;
const Int32 CAudioRecord::IS_NOT_ALLOWED = -2;
const Int32 CAudioRecord::GOT_ERROR = -3;

//------------------------------------
//    CAudioRecord::NativeEventHandler
//------------------------------------

CAudioRecord::NativeEventHandler::NativeEventHandler(
    /* [in] */ IAudioRecord* recorder,
    /* [in] */ ILooper* looper,
    /* [in] */ CAudioRecord* owner)
    : HandlerBase(looper)
{
    mAudioRecord = recorder;
}

ECode CAudioRecord::NativeEventHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    Int32 what;
    msg->GetWhat(&what);

    AutoPtr<IAudioRecordOnRecordPositionUpdateListener> listener;

    {
        AutoLock lock(mOwner->mPositionListenerLock);
        listener = mOwner->mPositionListener;
    }

    switch(what) {
        case NATIVE_EVENT_MARKER: {
            if (listener != NULL) {
                listener->OnMarkerReached(mAudioRecord);
            }
            break;
        }
        case NATIVE_EVENT_NEW_POS: {
            if (listener != NULL) {
                listener->OnPeriodicNotification(mAudioRecord);
            }
            break;
        }
        default: {
            Logger::E(TAG, "[ android.media.AudioRecord.NativeEventHandler ] " \
                "Unknown event type: %d", what);
            break;
        }
    }
    return NOERROR;
}

//------------------------------------
//    CAudioRecord
//------------------------------------

CAudioRecord::CAudioRecord()
    : mNativeRecorderInJavaObj(0)
    , mNativeCallbackCookie(0)
    , mSampleRate(22050)
    , mChannelCount(1)
    , mChannels(IAudioFormat::CHANNEL_IN_MONO)
    , mChannelConfiguration(IAudioFormat::CHANNEL_IN_MONO)
    , mAudioFormat(IAudioFormat::ENCODING_PCM_16BIT)
    , mRecordSource(IAudioSource::DEFAULT)
    , mState(STATE_UNINITIALIZED)
    , mRecordingState(RECORDSTATE_STOPPED)
    , mNativeBufferSizeInBytes(0)
    , mSessionId(0)
    , mPrivacyMode(FALSE)
{}

CAudioRecord::~CAudioRecord()
{
    Finalize();
}

void CAudioRecord::Finalize()
{
    Native_finalize();
}

ECode CAudioRecord::constructor(
    /* [in] */ Int32 audioSource,
    /* [in] */ Int32 sampleRateInHz,
    /* [in] */ Int32 channelConfig,
    /* [in] */ Int32 audioFormat,
    /* [in] */ Int32 bufferSizeInBytes)
{
    mState = STATE_UNINITIALIZED;
    mRecordingState = RECORDSTATE_STOPPED;

    AutoPtr<ILooperHelper> looperHelper;
    CLooperHelper::AcquireSingleton((ILooperHelper**)&looperHelper);
    looperHelper->MyLooper((ILooper**)&mInitializationLooper);
    // remember which looper is associated with the AudioRecord instanciation
    if (mInitializationLooper == NULL) {
        looperHelper->GetMainLooper((ILooper**)&mInitializationLooper);
    }

    FAIL_RETURN(AudioParamCheck(audioSource, sampleRateInHz, channelConfig, audioFormat));

    FAIL_RETURN(AudioBuffSizeCheck(bufferSizeInBytes));

    // native initialization
    AutoPtr< ArrayOf<Int32> > session = ArrayOf<Int32>::Alloc(1);
    (*session)[0] = 0;

    //TODO: update native initialization when information about hardware init failure
    //      due to capture device already open is available.
    Int32 initResult = Native_setup(THIS_PROBE(IAudioRecord), mRecordSource, mSampleRate,
        mChannels, mAudioFormat, mNativeBufferSizeInBytes, session);

    if (initResult != SUCCESS) {
        Loge(String("Error code ")
            + StringUtils::Int32ToString(initResult)
            + String(" when initializing native AudioRecord object."));
        return NOERROR; // with mState == STATE_UNINITIALIZED
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //BEGIN PRIVACY

    Initiate();

    //END PRIVACY
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    mSessionId = (*session)[0];
    mState = STATE_INITIALIZED;
    return NOERROR;
}

//---------------------------------------------------------
// Transport control methods
//--------------------
ECode CAudioRecord::StartRecording() // throws IllegalStateException
{
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //BEGIN PRIVACY
    //now check if everything was ok in constructor!

    if(!mPrivacyMode){
        Initiate();
    }

    if ((mState != STATE_INITIALIZED) || (CheckIfPackagesAllowed() == IS_NOT_ALLOWED)) { //If applicaton is not allowed -> throw exception!
        DataAccess(FALSE);
        AutoPtr< ArrayOf<String> > packageName = GetPackageName();
        if (packageName != NULL && packageName->GetLength() > 0) {
            AutoPtr<IPrivacySettings> ps;
            mPrivacySettingsManager->GetSettings((*packageName)[0], Process::MyUid(), (IPrivacySettings**)&ps);
            mPrivacySettingsManager->Notification((*packageName)[0], 0,
                IPrivacySettings::EMPTY, IPrivacySettings::DATA_RECORD_AUDIO,
                String(NULL), ps);
            // throw(new IllegalStateException("startRecording() called on an "+"uninitialized AudioRecord."));
            return E_ILLEGAL_STATE_EXCEPTION;
        }
    }

    DataAccess(TRUE);
    AutoPtr< ArrayOf<String> > packageName = GetPackageName();
    if (packageName != NULL && packageName->GetLength() > 0) {
        AutoPtr<IPrivacySettings> ps;
        mPrivacySettingsManager->GetSettings((*packageName)[0], Process::MyUid(), (IPrivacySettings**)&ps);
        mPrivacySettingsManager->Notification((*packageName)[0], 0,
            IPrivacySettings::REAL, IPrivacySettings::DATA_RECORD_AUDIO,
            String(NULL), ps);
    }
    //END PRIVACY
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // start recording
    {
        AutoLock lock(mRecordingStateLock);

        if (Native_start(IMediaSyncEvent::SYNC_EVENT_NONE, 0) == SUCCESS) {
            mRecordingState = RECORDSTATE_RECORDING;
        }
    }
    return NOERROR;
}

ECode CAudioRecord::StartRecording( // throws IllegalStateException
    /* [in] */ IMediaSyncEvent* syncEvent)
{
    if (mState != STATE_INITIALIZED) {
        /*throw(new IllegalStateException("startRecording() called on an "
                +"uninitialized AudioRecord."));*/
        Logger::E(TAG, "startRecording() called on an uninitialized AudioRecord.");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    // start recording
    Int32 tempValue1;
    Int32 tempValue2;
    syncEvent->GetType(&tempValue1);
    syncEvent->GetAudioSessionId(&tempValue2);
    {
        AutoLock lock(mRecordingStateLock);

        if (Native_start(tempValue1, tempValue2) == SUCCESS) {
            mRecordingState = RECORDSTATE_RECORDING;
        }
    }
    return NOERROR;
}

ECode CAudioRecord::Stop() // throws IllegalStateException
{
    if (mState != STATE_INITIALIZED) {
        //throw(new IllegalStateException("stop() called on an uninitialized AudioRecord."));
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    // stop recording
    {
        AutoLock lock(mRecordingStateLock);

        Native_stop();
        mRecordingState = RECORDSTATE_STOPPED;
    }
    return NOERROR;
}

//---------------------------------------------------------
// Audio data supply
//--------------------
ECode CAudioRecord::Read(
    /* [in] */ ArrayOf<Byte>* audioData,
    /* [in] */ Int32 offsetInBytes,
    /* [in] */ Int32 sizeInBytes,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    if (mState != STATE_INITIALIZED) {
        *result = ERROR_INVALID_OPERATION;
        return NOERROR;
    }

    if ((audioData == NULL) || (offsetInBytes < 0 ) || (sizeInBytes < 0)
        || (offsetInBytes + sizeInBytes > audioData->GetLength())) {
        *result = ERROR_BAD_VALUE;
        return NOERROR;
    }

    *result = Native_read_in_byte_array(audioData, offsetInBytes, sizeInBytes);
    return NOERROR;
}

ECode CAudioRecord::Read(
    /* [in] */ ArrayOf<Int16>* audioData,
    /* [in] */ Int32 offsetInShorts,
    /* [in] */ Int32 sizeInShorts,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    if (mState != STATE_INITIALIZED) {
        *result = ERROR_INVALID_OPERATION;
        return NOERROR;
    }

    if ( (audioData == NULL) || (offsetInShorts < 0 ) || (sizeInShorts < 0)
        || (offsetInShorts + sizeInShorts > audioData->GetLength())) {
        *result = ERROR_BAD_VALUE;
        return NOERROR;
    }

    *result = Native_read_in_short_array(audioData, offsetInShorts, sizeInShorts);
    return NOERROR;
}

ECode CAudioRecord::Read(
    /* [in] */ IByteBuffer* audioBuffer,
    /* [in] */ Int32 sizeInBytes,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    if (mState != STATE_INITIALIZED) {
        *result = ERROR_INVALID_OPERATION;
        return NOERROR;
    }

    if ( (audioBuffer == NULL) || (sizeInBytes < 0) ) {
        *result = ERROR_BAD_VALUE;
        return NOERROR;
    }

    *result = Native_read_in_direct_buffer(audioBuffer, sizeInBytes);
    return NOERROR;
}

//--------------------------------------------------------------------------
// Initialization / configuration
//--------------------
ECode CAudioRecord::SetRecordPositionUpdateListener(
    /* [in] */ IAudioRecordOnRecordPositionUpdateListener* listener)
{
    return SetRecordPositionUpdateListener(listener, NULL);
}

ECode CAudioRecord::SetRecordPositionUpdateListener(
    /* [in] */ IAudioRecordOnRecordPositionUpdateListener* listener,
    /* [in] */ IHandler* handler)
{
    {
        AutoLock lock(mPositionListenerLock);

        mPositionListener = listener;

        if (listener != NULL) {
            if (handler != NULL) {
                AutoPtr<ILooper> looper;
                handler->GetLooper((ILooper**)&looper);
                mEventHandler = new NativeEventHandler(this, looper, this);
            }
            else {
                // no given handler, use the looper the AudioRecord was created in
                mEventHandler = new NativeEventHandler(this, mInitializationLooper, this);
            }
        }
        else {
            mEventHandler = NULL;
        }
    }
    return NOERROR;
}

ECode CAudioRecord::SetNotificationMarkerPosition(
    /* [in] */ Int32 markerInFrames,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = Native_set_marker_pos(markerInFrames);
    return NOERROR;
}

ECode CAudioRecord::SetPositionNotificationPeriod(
    /* [in] */ Int32 periodInFrames,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = Native_set_pos_update_period(periodInFrames);
    return NOERROR;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//BEGIN PRIVACY
AutoPtr<ArrayOf<String> > CAudioRecord::GetPackageName()
{
    // try{
    if (mPackageManager == NULL){
        AutoPtr<IInterface> b = ServiceManager::GetService(String("package"));
        mPackageManager = IIPackageManager::Probe(b);
        assert(mPackageManager != NULL);
    }

    AutoPtr< ArrayOf<String> > packageNames;
    Int32 uid = Process::MyUid();
    mPackageManager->GetPackagesForUid(uid, (ArrayOf<String>**)&packageNames);
    return packageNames;
    // }
    // catch(Exception e){
        // e.printStackTrace();
        Logger::E(PRIVACY_TAG,"something went wrong with getting package name");
        // return null;
    // }
}

void CAudioRecord::Initiate()
{
    // try{
    AutoPtr<IInterface> b = ServiceManager::GetService(String("privacy"));
    AutoPtr<IIPrivacySettingsManager> psm = IIPrivacySettingsManager::Probe(b);

    mContext = NULL;
    ECode ec = CPrivacySettingsManager::New(mContext, psm, (IPrivacySettingsManager**)&mPrivacySettingsManager);
    if (mPackageManager == NULL){
        b = ServiceManager::GetService(String("package"));
        mPackageManager = IIPackageManager::Probe(b);
    }
    mPrivacyMode = TRUE;
    if (FAILED(ec)) {
        Logger::E(PRIVACY_TAG, "Something went wrong with initalize variables");
        mPrivacyMode = FALSE;
    }

    // }
    // catch(Exception e){
    //     e.printStackTrace();
    //     Log.e(PRIVACY_TAG, "Something went wrong with initalize variables");
    //     mPrivacyMode = false;
    // }
}

Int32 CAudioRecord::CheckIfPackagesAllowed()
{
    // try{
    //boolean isAllowed = false;
    if (mPrivacySettingsManager != NULL) {
        AutoPtr< ArrayOf<String> > packageNames = GetPackageName();
        if (packageNames != NULL) {
            Byte t;
            Int32 uid = Process::MyUid();
            for (Int32 i = 0; i < packageNames->GetLength(); i++){
                AutoPtr< IPrivacySettings > pSet;
                mPrivacySettingsManager->GetSettings((*packageNames)[i], uid, (IPrivacySettings**)&pSet);
                if (pSet != NULL) {
                    pSet->GetRecordAudioSetting(&t);
                    if (t != IPrivacySettings::REAL) {
                        //if pSet is NULL, we allow application to access to mic
                        return IS_NOT_ALLOWED;
                    }
                    pSet = NULL;
                }
            }
            return IS_ALLOWED;
        }
        else{
           Logger::E(PRIVACY_TAG, "return GOT_ERROR, because packageNames are NULL");
            return GOT_ERROR;
        }
    }
    else{
        Logger::E(PRIVACY_TAG,"return GOT_ERROR, because mPrivacySettingsManager is NULL");
        return GOT_ERROR;
    }
    // }
    // catch (Exception e){
    //     e.printStackTrace();
    //     Log.e(PRIVACY_TAG,"Got exception in checkIfPackagesAllowed");
    //     return GOT_ERROR;
    // }
    return 0;
}

void CAudioRecord::DataAccess(
    /* [in] */ Boolean success)
{
    AutoPtr< ArrayOf<String> > packageNames = GetPackageName();
    if(success && packageNames != NULL){
        for (Int32 i = 0; i < packageNames->GetLength();i++) {
            Logger::I(PRIVACY_TAG, "Allowed Package: -%s- accessing microphone.",
                (*packageNames)[i].string());
        }
    }
    else if (packageNames != NULL){
        for (Int32 i = 0; i < packageNames->GetLength();i++) {
            Logger::I(PRIVACY_TAG, "Blocked Package: -%s- accessing microphone.",
                (*packageNames)[i].string());
        }
    }
}
//END PRIVACY
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ECode CAudioRecord::AudioParamCheck(
    /* [in] */ Int32 audioSource,
    /* [in] */ Int32 sampleRateInHz,
    /* [in] */ Int32 channelConfig,
    /* [in] */ Int32 audioFormat)
{
    //--------------
    // audio source
    AutoPtr<IMediaRecorderHelper> mediaRecorderHelper;
    CMediaRecorderHelper::AcquireSingleton((IMediaRecorderHelper**)&mediaRecorderHelper);
    Int32 tempValue;
    if ( (audioSource < IAudioSource::DEFAULT) ||
         (audioSource > (mediaRecorderHelper->GetAudioSourceMax(&tempValue), tempValue)) ) {
        //throw (new IllegalArgumentException("Invalid audio source."));
        Logger::E(TAG, "Invalid audio source.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    else {
        mRecordSource = audioSource;
    }

    //--------------
    // sample rate
    if ( (sampleRateInHz < 4000) || (sampleRateInHz > 48000) ) {
        /*throw (new IllegalArgumentException(sampleRateInHz
                + "Hz is not a supported sample rate."));*/
        Logger::E(TAG, "Hz is not a supported sample rate.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    else {
        mSampleRate = sampleRateInHz;
    }

    //--------------
    // channel config
    mChannelConfiguration = channelConfig;

    switch (channelConfig) {
        case IAudioFormat::CHANNEL_IN_DEFAULT: // AudioFormat.CHANNEL_CONFIGURATION_DEFAULT
        case IAudioFormat::CHANNEL_IN_MONO:
        case IAudioFormat::CHANNEL_CONFIGURATION_MONO: {
            mChannelCount = 1;
            mChannels = IAudioFormat::CHANNEL_IN_MONO;
            break;
        }

        case IAudioFormat::CHANNEL_IN_STEREO:
        case IAudioFormat::CHANNEL_CONFIGURATION_STEREO: {
            mChannelCount = 2;
            mChannels = IAudioFormat::CHANNEL_IN_STEREO;
            break;
        }

        default: {
            mChannelCount = 0;
            mChannels = IAudioFormat::CHANNEL_INVALID;
            mChannelConfiguration = IAudioFormat::CHANNEL_INVALID;
            //throw (new IllegalArgumentException("Unsupported channel configuration."));
            Logger::E(TAG, "Unsupported channel configuration.");
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
    }

    //--------------
    // audio format
    switch (audioFormat) {
        case IAudioFormat::ENCODING_DEFAULT: {
            mAudioFormat = IAudioFormat::ENCODING_PCM_16BIT;
            break;
        }
        case IAudioFormat::ENCODING_PCM_16BIT:
        case IAudioFormat::ENCODING_PCM_8BIT: {
            mAudioFormat = audioFormat;
            break;
        }
        default:
        {
            mAudioFormat = IAudioFormat::ENCODING_INVALID;
            /*throw (new IllegalArgumentException("Unsupported sample encoding."
                    + " Should be ENCODING_PCM_8BIT or ENCODING_PCM_16BIT."));*/
            Logger::E(TAG, "Unsupported sample encoding. Should be ENCODING_PCM_8BIT or ENCODING_PCM_16BIT.");
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
    }
    return NOERROR;
}

ECode CAudioRecord::AudioBuffSizeCheck(
    /* [in] */ Int32 audioBufferSize)
{
    // NB: this section is only valid with PCM data.
    // To update when supporting compressed formats
    Int32 frameSizeInBytes = mChannelCount
        * (mAudioFormat == IAudioFormat::ENCODING_PCM_8BIT ? 1 : 2);
    if ((audioBufferSize % frameSizeInBytes != 0) || (audioBufferSize < 1)) {
        Logger::E(TAG, "Invalid audio buffer size.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mNativeBufferSizeInBytes = audioBufferSize;
    return NOERROR;
}

ECode CAudioRecord::ReleaseResources()
{
//    try {
    Stop();
//    } catch(IllegalStateException ise) {
        // don't raise an exception, we're releasing the resources.
//    }
    Native_release();
    mState = STATE_UNINITIALIZED;
    return NOERROR;
}

ECode CAudioRecord::GetSampleRate(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mSampleRate;
    return NOERROR;
}

ECode CAudioRecord::GetAudioSource(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mRecordSource;
    return NOERROR;
}

ECode CAudioRecord::GetAudioFormat(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mAudioFormat;
    return NOERROR;
}

ECode CAudioRecord::GetChannelConfiguration(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mChannelConfiguration;
    return NOERROR;
}

ECode CAudioRecord::GetChannelCount(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mChannelCount;
    return NOERROR;
}

ECode CAudioRecord::GetState(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mState;
    return NOERROR;
}

ECode CAudioRecord::GetRecordingState(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mRecordingState;
    return NOERROR;
}

ECode CAudioRecord::GetNotificationMarkerPosition(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = Native_get_marker_pos();
    return NOERROR;
}

ECode CAudioRecord::GetPositionNotificationPeriod(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = Native_get_pos_update_period();
    return NOERROR;
}

ECode CAudioRecord::GetMinBufferSize(
    /* [in] */ Int32 sampleRateInHz,
    /* [in] */ Int32 channelConfig,
    /* [in] */ Int32 audioFormat,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    Int32 channelCount = 0;
    switch(channelConfig) {
        case IAudioFormat::CHANNEL_IN_DEFAULT: // AudioFormat.CHANNEL_CONFIGURATION_DEFAULT
        case IAudioFormat::CHANNEL_IN_MONO:
        case IAudioFormat::CHANNEL_CONFIGURATION_MONO:
        {
            channelCount = 1;
            break;
        }
        case IAudioFormat::CHANNEL_IN_STEREO:
        case IAudioFormat::CHANNEL_CONFIGURATION_STEREO:
        {
            channelCount = 2;
            break;
        }
        case IAudioFormat::CHANNEL_INVALID:
        default:
        {
            Loge(String("getMinBufferSize(): Invalid channel configuration."));
            *result = IAudioRecord::ERROR_BAD_VALUE;
            return NOERROR;
        }
    }

    // PCM_8BIT is not supported at the moment
    if (audioFormat != IAudioFormat::ENCODING_PCM_16BIT) {
        Loge(String("getMinBufferSize(): Invalid audio format."));
        *result = IAudioRecord::ERROR_BAD_VALUE;
        return NOERROR;
    }

    Int32 size = Native_get_min_buff_size(sampleRateInHz, channelCount, audioFormat);
    if (size == 0) {
        *result = IAudioRecord::ERROR_BAD_VALUE;
        return NOERROR;
    }
    else if (size == -1) {
        *result = IAudioRecord::ERROR;
        return NOERROR;
    }
    else {
        *result = size;
        return NOERROR;
    }
    return NOERROR;
}

ECode CAudioRecord::GetAudioSessionId(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mSessionId;
    return NOERROR;
}

//---------------------------------------------------------
// Java methods called from the native side
//--------------------
void CAudioRecord::PostEventFromNative(
    /* [in] */ IInterface* audiorecord_ref,
    /* [in] */ Int32 what,
    /* [in] */ Int32 arg1,
    /* [in] */ Int32 arg2,
    /* [in] */ IInterface* obj)
{
    StringBuilder sb("Event posted from the native side: event=");
    sb += what;
    sb += " args=";
    sb += arg1;
    sb += " ";
    sb += arg2;
    Logd(sb.ToString());

    AutoPtr<IAudioRecord> recorder = IAudioRecord::Probe(audiorecord_ref);
    if (recorder == NULL) {
        return;
    }

    CAudioRecord* car = (CAudioRecord*)recorder.Get();
    if (car->mEventHandler != NULL) {
        AutoPtr<IMessage> m;
        car->mEventHandler->ObtainMessage(what, arg1, arg2, obj, (IMessage**)&m);
        Boolean result;
        car->mEventHandler->SendMessage(m, &result);
    }
}

//-------------------------------------------------
//    Native Method : android_media_AudioRecord.cpp
//-------------------------------------------------

struct audiorecord_callback_cookie
{
    AutoPtr<IWeakReference> mAudioRecordRef;
    Boolean                 mBusy;
};

Int32 translateRecorderErrorCode(Int32 code)
{
    switch(code) {
    case android::NO_ERROR:
        return AUDIORECORD_SUCCESS;
    case android::BAD_VALUE:
        return AUDIORECORD_ERROR_BAD_VALUE;
    case android::INVALID_OPERATION:
        return AUDIORECORD_ERROR_INVALID_OPERATION;
    default:
        return AUDIORECORD_ERROR;
    }
}

void CAudioRecord::recorderCallback(Int32 event, void* user, void *info)
{
    if (event == android::AudioRecord::EVENT_MORE_DATA) {
        // set size to 0 to signal we're not using the callback to read more data
        android::AudioRecord::Buffer* pBuff = (android::AudioRecord::Buffer*)info;
        pBuff->size = 0;

    } else if (event == android::AudioRecord::EVENT_MARKER) {
        audiorecord_callback_cookie *callbackInfo = (audiorecord_callback_cookie *)user;
        if (user) {
            PostEventFromNative(callbackInfo->mAudioRecordRef, event, 0,0, NULL);
        }

    } else if (event == android::AudioRecord::EVENT_NEW_POS) {
        audiorecord_callback_cookie *callbackInfo = (audiorecord_callback_cookie *)user;
        if (user) {
            PostEventFromNative(callbackInfo->mAudioRecordRef, event, 0,0, NULL);
        }
    }
}

Int32 CAudioRecord::Native_setup(
    /* [in] */ IInterface* audiorecord_this,
    /* [in] */ Int32 recordSource,
    /* [in] */ Int32 sampleRate,
    /* [in] */ Int32 channels,
    /* [in] */ Int32 audioFormat,
    /* [in] */ Int32 buffSizeInBytes,
    /* [in] */ ArrayOf<Int32>* session)
{
    //LOGV(">> Entering android_media_AudioRecord_setup");
    //LOGV("sampleRate=%d, audioFormat=%d, channels=%x, buffSizeInBytes=%d",
    //     sampleRateInHertz, audioFormat, channels,     buffSizeInBytes);

    if (!audio_is_input_channel(channels)) {
        Logger::E(TAG, "Error creating AudioRecord: channel count is not 1 or 2.");
        return AUDIORECORD_ERROR_SETUP_INVALIDCHANNELMASK;
    }
    uint32_t nbChannels = popcount(channels);

    // compare the format against the Java constants
    if ((audioFormat != IAudioFormat::ENCODING_PCM_16BIT)
        && (audioFormat != IAudioFormat::ENCODING_PCM_8BIT)) {
        Logger::E(TAG, "Error creating AudioRecord: unsupported audio format.");
        return AUDIORECORD_ERROR_SETUP_INVALIDFORMAT;
    }

   Int32 bytesPerSample = audioFormat == IAudioFormat::ENCODING_PCM_16BIT ? 2 : 1;
   audio_format_t format = audioFormat == IAudioFormat::ENCODING_PCM_16BIT ?
           AUDIO_FORMAT_PCM_16_BIT : AUDIO_FORMAT_PCM_8_BIT;

    if (buffSizeInBytes == 0) {
        Logger::E(TAG, "Error creating AudioRecord: frameCount is 0.");
        return AUDIORECORD_ERROR_SETUP_ZEROFRAMECOUNT;
    }
    Int32 frameSize = nbChannels * bytesPerSample;
    size_t frameCount = buffSizeInBytes / frameSize;

    if (recordSource >= AUDIO_SOURCE_CNT) {
        Logger::E(TAG, "Error creating AudioRecord: unknown source.");
        return AUDIORECORD_ERROR_SETUP_INVALIDSOURCE;
    }

    if (session == NULL) {
        ALOGE("Error creating AudioRecord: invalid session ID pointer");
        return AUDIORECORD_ERROR;
    }

    Int32 sessionId = (*session)[0];

    audiorecord_callback_cookie *lpCallbackData = NULL;
    android::AudioRecord* lpRecorder = NULL;

    // create an uninitialized AudioRecord object
    lpRecorder = new android::AudioRecord();
        if(lpRecorder == NULL) {
        Logger::E(TAG, "Error creating AudioRecord instance.");
        return AUDIORECORD_ERROR_SETUP_NATIVEINITFAILED;
    }

    // create the callback information:
    // this data will be passed with every AudioRecord callback
    lpCallbackData = new audiorecord_callback_cookie;
    // we use a weak reference so the AudioRecord object can be garbage collected.
    // lpCallbackData->audioRecord_ref = env->NewGlobalRef(weak_this);
    GetWeakReference((IWeakReference**)&lpCallbackData->mAudioRecordRef);
    lpCallbackData->mBusy = FALSE;
    lpRecorder->set((audio_source_t)recordSource,
       sampleRate,
       format,        // word length, PCM
       channels,
       frameCount,
       recorderCallback,// callback_t
       lpCallbackData,// void* user
       0,             // notificationFrames,
       TRUE,         // threadCanCallJava)
       sessionId);

    if(lpRecorder->initCheck() != android::NO_ERROR) {
        Logger::E(TAG, "Error creating AudioRecord instance: initialization check failed.");
        goto native_init_failure;
    }

    // read the audio session ID back from AudioRecord in case a new session was created during set()
    (*session)[0] = lpRecorder->getSessionId();

    // {   // scope for the lock
    //     AutoLock l(sLock);
    //     sAudioRecordCallBackCookies.add(lpCallbackData);
    // }

    // save our newly created C++ AudioRecord in the "nativeRecorderInJavaObj" field
    // of the Java object
    mNativeRecorderInJavaObj = (Int32)lpRecorder;

    // save our newly created callback information in the "nativeCallbackCookie" field
    // of the Java object (in mNativeCallbackCookie) so we can free the memory in finalize()
    mNativeCallbackCookie = (Int32)lpCallbackData;

    return AUDIORECORD_SUCCESS;

    // failure:
native_init_failure:
    delete lpCallbackData;
    delete lpRecorder;

    mNativeRecorderInJavaObj = 0;
    mNativeCallbackCookie = 0;

    return AUDIORECORD_ERROR_SETUP_NATIVEINITFAILED;
}

void CAudioRecord::Native_finalize()
{
    Native_release();
}

void CAudioRecord::Native_release()
{
    // serialize access. Ugly, but functional.
    AutoLock lock(&_m_syncLock);

    android::AudioRecord* lpRecorder = (android::AudioRecord *)mNativeRecorderInJavaObj;
    audiorecord_callback_cookie* lpCookie = (audiorecord_callback_cookie *)mNativeCallbackCookie;

    // reset the native resources in the Java object so any attempt to access
    // them after a call to release fails.
    mNativeRecorderInJavaObj = 0;
    mNativeCallbackCookie = 0;

    // delete the AudioRecord object
    if (lpRecorder) {
        // LOGV("About to delete lpRecorder: %x\n", (int)lpRecorder);
        lpRecorder->stop();
        delete lpRecorder;
    }

    // delete the callback information
    if (lpCookie) {
        // LOGV("deleting lpCookie: %x\n", (int)lpCookie);
        delete lpCookie;
    }
}

Int32 CAudioRecord::Native_start(
    /* [in] */ Int32 syncEvent,
    /* [in] */ Int32 sessionId)
{
    android::AudioRecord *lpRecorder = (android::AudioRecord *)mNativeRecorderInJavaObj;
    if (lpRecorder == NULL) {
        //jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return AUDIORECORD_ERROR;
    }

    return translateRecorderErrorCode(lpRecorder->start());
}

void CAudioRecord::Native_stop()
{
    android::AudioRecord *lpRecorder = (android::AudioRecord *)mNativeRecorderInJavaObj;
    if (lpRecorder == NULL) {
        //jniThrowException(env, "java/lang/IllegalStateException", NULL);
        return;
    }

    lpRecorder->stop();
    //LOGV("Called lpRecorder->stop()");
}

Int32 CAudioRecord::Native_read_in_byte_array(
    /* [in] */ ArrayOf<Byte>* audioData,
    /* [in] */ Int32 offsetInBytes,
    /* [in] */ Int32 sizeInBytes)
{
    Byte* recordBuff = NULL;
    android::AudioRecord *lpRecorder = NULL;

    // get the audio recorder from which we'll read new audio samples
    lpRecorder = (android::AudioRecord *)mNativeRecorderInJavaObj;
    if (lpRecorder == NULL) {
        //LOGE("Unable to retrieve AudioRecord object, can't record");
        return 0;
    }

    if (!audioData) {
        //LOGE("Invalid Java array to store recorded audio, can't record");
        return 0;
    }

    // get the pointer to where we'll record the audio
    recordBuff = audioData->GetPayload();

    if (recordBuff == NULL) {
        //LOGE("Error retrieving destination for recorded audio data, can't record");
        return 0;
    }

    // read the new audio data from the native AudioRecord object
    ssize_t recorderBuffSize = lpRecorder->frameCount() * lpRecorder->frameSize();
    ssize_t readSize = lpRecorder->read(recordBuff + offsetInBytes,
                            sizeInBytes > (Int32)recorderBuffSize ?
                            (Int32)recorderBuffSize : sizeInBytes );

    return (Int32)readSize;
}

Int32 CAudioRecord::Native_read_in_short_array(
    /* [in] */ ArrayOf<Int16>* audioData,
    /* [in] */ Int32 offsetInShorts,
    /* [in] */ Int32 sizeInShorts)
{
    return (Native_read_in_byte_array((ArrayOf<Byte>*)audioData,
            offsetInShorts * 2, sizeInShorts * 2) / 2);
}

Int32 CAudioRecord::Native_read_in_direct_buffer(
    /* [in] */ IInterface* buffer,
    /* [in] */ Int32 sizeInBytes)
{
    // get the audio recorder from which we'll read new audio samples
    android::AudioRecord *lpRecorder = (android::AudioRecord *)mNativeRecorderInJavaObj;
    if(lpRecorder == NULL) {
        return 0;
    }

    // direct buffer and direct access supported?
    // long capacity = env->GetDirectBufferCapacity(buffer);
    AutoPtr<IByteBuffer> byteBuf = IByteBuffer::Probe(buffer);
    AutoPtr<ArrayOf<Byte> > bytes;
    byteBuf->GetArray((ArrayOf<Byte>**)&bytes);
    Int64 capacity = bytes->GetLength();
    if(capacity == -1) {
        // buffer direct access is not supported
        ALOGE("Buffer direct access is not supported, can't record");
        return 0;
    }
    //LOGV("capacity = %ld", capacity);
    // jbyte* nativeFromJavaBuf = (jbyte*) env->GetDirectBufferAddress(buffer);
    void* nativeFromJavaBuf = bytes->GetPayload();
    if(nativeFromJavaBuf == NULL) {
        ALOGE("Buffer direct access is not supported, can't record");
        return 0;
    }

    // read new data from the recorder
    return lpRecorder->read(nativeFromJavaBuf,
                                   capacity < sizeInBytes ? capacity : sizeInBytes);
}

Int32 CAudioRecord::Native_set_marker_pos(
    /* [in] */ Int32 marker)
{
    android::AudioRecord *lpRecorder = (android::AudioRecord *)mNativeRecorderInJavaObj;

    if (lpRecorder) {
        return translateRecorderErrorCode(lpRecorder->setMarkerPosition(marker));
    } else {
        // jniThrowException(env, "java/lang/IllegalStateException",
        //     "Unable to retrieve AudioRecord pointer for setMarkerPosition()");
        return AUDIORECORD_ERROR;
    }
}

Int32 CAudioRecord::Native_get_marker_pos()
{
    android::AudioRecord *lpRecorder = (android::AudioRecord *)mNativeRecorderInJavaObj;
    uint32_t markerPos = 0;

    if (lpRecorder) {
        lpRecorder->getMarkerPosition(&markerPos);
        return (Int32)markerPos;
    } else {
        // jniThrowException(env, "java/lang/IllegalStateException",
        //     "Unable to retrieve AudioRecord pointer for getMarkerPosition()");
        return AUDIORECORD_ERROR;
    }
}

Int32 CAudioRecord::Native_set_pos_update_period(
    /* [in] */ Int32 updatePeriod)
{
    android::AudioRecord *lpRecorder = (android::AudioRecord *)mNativeRecorderInJavaObj;

    if (lpRecorder) {
        return translateRecorderErrorCode(lpRecorder->setPositionUpdatePeriod(updatePeriod));
    } else {
        // jniThrowException(env, "java/lang/IllegalStateException",
        //     "Unable to retrieve AudioRecord pointer for setPositionUpdatePeriod()");
        return AUDIORECORD_ERROR;
    }
}

Int32 CAudioRecord::Native_get_pos_update_period()
{
    android::AudioRecord *lpRecorder = (android::AudioRecord *)mNativeRecorderInJavaObj;
    uint32_t period = 0;

    if (lpRecorder) {
        lpRecorder->getPositionUpdatePeriod(&period);
        return (Int32)period;
    } else {
        // jniThrowException(env, "java/lang/IllegalStateException",
        //     "Unable to retrieve AudioRecord pointer for getPositionUpdatePeriod()");
        return AUDIORECORD_ERROR;
    }
}

/*static*/
Int32 CAudioRecord::Native_get_min_buff_size(
    /* [in] */ Int32 sampleRateInHz,
    /* [in] */ Int32 channelCount,
    /* [in] */ Int32 audioFormat)
{
    // LOGV(">> android_media_AudioRecord_get_min_buff_size(%d, %d, %d)", sampleRateInHertz, nbChannels, audioFormat);

    Int32 frameCount = 0;
    android::status_t result = android::AudioRecord::getMinFrameCount(&frameCount,
            sampleRateInHz,
            (audioFormat == IAudioFormat::ENCODING_PCM_16BIT ?
            AUDIO_FORMAT_PCM_16_BIT : AUDIO_FORMAT_PCM_8_BIT),
            channelCount);

    if (result == android::BAD_VALUE) {
        return 0;
    }
    if (result != android::NO_ERROR) {
        return -1;
    }
    return frameCount * channelCount * (audioFormat == IAudioFormat::ENCODING_PCM_16BIT ? 2 : 1);
}

//-------------------------------------------------

/*static*/
void CAudioRecord::Logd(
    /* [in] */ const String& msg)
{
    Logger::D(TAG, String("[ android.media.AudioRecord ] ") + msg);
}

/*static*/
void CAudioRecord::Loge(
    /* [in] */ const String& msg)
{
    Logger::E(TAG, String("[ android.media.AudioRecord ] ") + msg);
}

} // namespace Media
} // namepsace Droid
} // namespace Elastos
