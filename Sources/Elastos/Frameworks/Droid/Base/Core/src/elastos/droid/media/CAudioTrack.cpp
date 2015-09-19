
#include "media/CAudioTrack.h"
//#include "media/ElAudioSystem.h"
#include <media/AudioTrack.h>
#include <media/AudioSystem.h>
#include <binder/MemoryHeapBase.h>
#include <binder/MemoryBase.h>
#include "os/Looper.h"
#include <system/audio.h>
#include <elastos/core/Math.h>
#include <elastos/utility/logging/Logger.h>

using Elastos::Droid::Os::Looper;
using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Media {

const String CAudioTrack::TAG("CAudioTrack");

const Float CAudioTrack::VOLUME_MIN = 0.0f;
const Float CAudioTrack::VOLUME_MAX = 1.0f;

const Int32 CAudioTrack::ERROR_NATIVESETUP_AUDIOSYSTEM         = -16;
const Int32 CAudioTrack::ERROR_NATIVESETUP_INVALIDCHANNELMASK  = -17;
const Int32 CAudioTrack::ERROR_NATIVESETUP_INVALIDFORMAT       = -18;
const Int32 CAudioTrack::ERROR_NATIVESETUP_INVALIDSTREAMTYPE   = -19;
const Int32 CAudioTrack::ERROR_NATIVESETUP_NATIVEINITFAILED    = -20;

const Int32 CAudioTrack::NATIVE_EVENT_MARKER  = 3;
const Int32 CAudioTrack::NATIVE_EVENT_NEW_POS = 4;

const Int32 CAudioTrack::SUPPORTED_OUT_CHANNELS =
    IAudioFormat::CHANNEL_OUT_FRONT_LEFT |
    IAudioFormat::CHANNEL_OUT_FRONT_RIGHT |
    IAudioFormat::CHANNEL_OUT_FRONT_CENTER |
    IAudioFormat::CHANNEL_OUT_LOW_FREQUENCY |
    IAudioFormat::CHANNEL_OUT_BACK_LEFT |
    IAudioFormat::CHANNEL_OUT_BACK_RIGHT |
    IAudioFormat::CHANNEL_OUT_BACK_CENTER;


//===================================================================================
//              CAudioTrack::EventHandler
//===================================================================================

CAudioTrack::EventHandler::EventHandler(
    /* [in] */ ILooper* looper,
    /* [in] */ CAudioTrack* track)
    : HandlerBase(looper)
    , mAudioTrack(track)
{
}

ECode CAudioTrack::EventHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    if (mAudioTrack == NULL) {
        return NOERROR;
    }

    Int32 what;
    msg->GetWhat(&what);

    AutoPtr<IOnPlaybackPositionUpdateListener> listener;

    {
        AutoLock lock(mAudioTrack->mPositionListenerLock);
        listener = mAudioTrack->mPositionListener;
    }

    switch(what) {
    case CAudioTrack::NATIVE_EVENT_MARKER:
        if (listener != NULL) {
            listener->OnMarkerReached(
                (IAudioTrack*)(mAudioTrack->Probe(EIID_IAudioTrack)));
        }
        break;
    case CAudioTrack::NATIVE_EVENT_NEW_POS:
        if (listener != NULL) {
            listener->OnPeriodicNotification(
                (IAudioTrack*)(mAudioTrack->Probe(EIID_IAudioTrack)));
        }
        break;
    default:
        Logger::E(CAudioTrack::TAG, "[ android.media.AudioTrack.NativeEventHandler ] "
            "Unknown event type: %d", what);
        break;
    }
    return NOERROR;
}

//===================================================================================
//              CAudioTrack::NativeEventHandlerDelegate
//===================================================================================

CAudioTrack::NativeEventHandlerDelegate::NativeEventHandlerDelegate(
    /* [in] */ CAudioTrack* track,
    /* [in] */ IHandler* handler)
    : mAudioTrack(track)
{
    // find the looper for our new event handler
    AutoPtr<ILooper> looper;
    if (handler != NULL) {
        handler->GetLooper((ILooper**)&looper);
    }
    else {
        // no given handler, use the looper the AudioTrack was created in
        looper = mAudioTrack->mInitializationLooper;
    }

    // construct the event handler with this looper
    if (looper != NULL) {
        // implement the event handler delegate
        mHandler = new EventHandler(looper, mAudioTrack);
    }
    else {
        mHandler = NULL;
    }
}

AutoPtr<IHandler> CAudioTrack::NativeEventHandlerDelegate::GetHandler()
{
    return mHandler;
}

//===================================================================================
//              CAudioTrack
//===================================================================================

CAudioTrack::CAudioTrack()
    : mState(STATE_UNINITIALIZED)
    , mPlayState(PLAYSTATE_STOPPED)
    , mNativeBufferSizeInBytes(0)
    , mSampleRate(22050)
    , mChannelCount(1)
    , mChannels(IAudioFormat::CHANNEL_OUT_MONO)
    , mStreamType(IAudioManager::STREAM_MUSIC)
    , mDataLoadMode(MODE_STREAM)
    , mChannelConfiguration(IAudioFormat::CHANNEL_OUT_MONO)
    , mAudioFormat(IAudioFormat::ENCODING_PCM_16BIT)
    , mSessionId(0)
    , mNativeTrack(0)
    , mNativeData(0)
{}

CAudioTrack::~CAudioTrack()
{
    NativeFinalize();
}

ECode CAudioTrack::constructor(
    /* [in] */ Int32 streamType,
    /* [in] */ Int32 sampleRateInHz,
    /* [in] */ Int32 channelConfig,
    /* [in] */ Int32 audioFormat,
    /* [in] */ Int32 bufferSizeInBytes,
    /* [in] */ Int32 mode)
{
    return constructor(streamType, sampleRateInHz, channelConfig,
        audioFormat, bufferSizeInBytes, mode, 0);
}

ECode CAudioTrack::constructor(
    /* [in] */ Int32 streamType,
    /* [in] */ Int32 sampleRateInHz,
    /* [in] */ Int32 channelConfig,
    /* [in] */ Int32 audioFormat,
    /* [in] */ Int32 bufferSizeInBytes,
    /* [in] */ Int32 mode,
    /* [in] */ Int32 sessionId)
{
    mState = STATE_UNINITIALIZED;

    // remember which looper is associated with the AudioTrack instantiation
    mInitializationLooper = Looper::GetMyLooper();
    if (mInitializationLooper == NULL) {
        mInitializationLooper = Looper::GetMainLooper();
    }

    FAIL_RETURN(AudioParamCheck(streamType, sampleRateInHz, channelConfig, audioFormat, mode));

    FAIL_RETURN(AudioBuffSizeCheck(bufferSizeInBytes));

    if (sessionId < 0) {
        Logger::E(TAG, "Invalid audio session ID: %d", sessionId);
        //throw (new IllegalArgumentException("Invalid audio session ID: "+sessionId));
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Int32 session[] = { sessionId };
    // native initialization
    Int32 initResult = NativeSetup(mStreamType, mSampleRate, mChannels,
            mAudioFormat, mNativeBufferSizeInBytes, mDataLoadMode, session);
    if (initResult != IAudioTrack::SUCCESS) {
        Logger::E(TAG, "Error code %d  when initializing AudioTrack.", initResult);
        //loge("Error code "+initResult+" when initializing AudioTrack.");
        return NOERROR; // with mState == STATE_UNINITIALIZED
    }

    mSessionId = session[0];

    if (mDataLoadMode == MODE_STATIC) {
        mState = STATE_NO_STATIC_DATA;
    }
    else {
        mState = STATE_INITIALIZED;
    }
    return NOERROR;
}

// Convenience method for the constructor's parameter checks.
// This is where constructor IllegalArgumentException-s are thrown
// postconditions:
//    mStreamType is valid
//    mChannelCount is valid
//    mChannels is valid
//    mAudioFormat is valid
//    mSampleRate is valid
//    mDataLoadMode is valid
ECode CAudioTrack::AudioParamCheck(
    /* [in] */ Int32 streamType,
    /* [in] */ Int32 sampleRateInHz,
    /* [in] */ Int32 channelConfig,
    /* [in] */ Int32 audioFormat,
    /* [in] */ Int32 mode)
{
    using Elastos::Core::Math;

    //--------------
    // stream type
    if( (streamType != IAudioManager::STREAM_ALARM)
        && (streamType != IAudioManager::STREAM_MUSIC)
        && (streamType != IAudioManager::STREAM_RING)
        && (streamType != IAudioManager::STREAM_SYSTEM)
        && (streamType != IAudioManager::STREAM_VOICE_CALL)
        && (streamType != IAudioManager::STREAM_NOTIFICATION)
        && (streamType != IAudioManager::STREAM_BLUETOOTH_SCO)
        && (streamType != IAudioManager::STREAM_DTMF)) {
        Logger::E(TAG, "Invalid stream type. %d", streamType);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    else {
        mStreamType = streamType;
    }

    //--------------
    // sample rate, note these values are subject to change
    if ( (sampleRateInHz < 4000) || (sampleRateInHz > 48000) ) {
        Logger::E(TAG, "%d Hz is not a supported sample rate.", sampleRateInHz);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    else {
        mSampleRate = sampleRateInHz;
    }

    //--------------
    // channel config
    mChannelConfiguration = channelConfig;

    switch (channelConfig) {
    case IAudioFormat::CHANNEL_OUT_DEFAULT: //AudioFormat.CHANNEL_CONFIGURATION_DEFAULT
    case IAudioFormat::CHANNEL_OUT_MONO:
    case IAudioFormat::CHANNEL_CONFIGURATION_MONO:
        mChannelCount = 1;
        mChannels = IAudioFormat::CHANNEL_OUT_MONO;
        break;
    case IAudioFormat::CHANNEL_OUT_STEREO:
    case IAudioFormat::CHANNEL_CONFIGURATION_STEREO:
        mChannelCount = 2;
        mChannels = IAudioFormat::CHANNEL_OUT_STEREO;
        break;
    default:
        if (!IsMultichannelConfigSupported(channelConfig)) {
            // input channel configuration features unsupported channels
            mChannelCount = 0;
            mChannels = IAudioFormat::CHANNEL_INVALID;
            mChannelConfiguration = IAudioFormat::CHANNEL_CONFIGURATION_INVALID;
            //throw(new IllegalArgumentException("Unsupported channel configuration."));
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
        else {
            mChannels = channelConfig;
            mChannelCount = Math::BitCount(channelConfig);
        }
    }

    //--------------
    // audio format
    switch (audioFormat) {
    case IAudioFormat::ENCODING_DEFAULT:
        mAudioFormat = IAudioFormat::ENCODING_PCM_16BIT;
        break;
    case IAudioFormat::ENCODING_PCM_16BIT:
    case IAudioFormat::ENCODING_PCM_8BIT:
        mAudioFormat = audioFormat;
        break;
    default:
        mAudioFormat = IAudioFormat::ENCODING_INVALID;
        Logger::E(TAG, "Unsupported sample encoding."
            " Should be ENCODING_PCM_8BIT or ENCODING_PCM_16BIT.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    //--------------
    // audio load mode
    if ( (mode != MODE_STREAM) && (mode != MODE_STATIC) ) {
        Logger::E(TAG, "Invalid mode.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    else {
        mDataLoadMode = mode;
    }
    return NOERROR;
}

Boolean CAudioTrack::IsMultichannelConfigSupported(
    /* [in] */ Int32 channelConfig)
{
    // check for unsupported channels
    if ((channelConfig & SUPPORTED_OUT_CHANNELS) != channelConfig) {
        Logger::E(TAG, "Channel configuration features unsupported channels");
        return FALSE;
    }

    // check for unsupported multichannel combinations:
    // - FL/FR must be present
    // - L/R channels must be paired (e.g. no single L channel)
    const Int32 frontPair = IAudioFormat::CHANNEL_OUT_FRONT_LEFT
        | IAudioFormat::CHANNEL_OUT_FRONT_RIGHT;
    if ((channelConfig & frontPair) != frontPair) {
        Logger::E(TAG, "Front channels must be present in multichannel configurations");
        return FALSE;
    }
    const Int32 backPair = IAudioFormat::CHANNEL_OUT_BACK_LEFT
        | IAudioFormat::CHANNEL_OUT_BACK_RIGHT;
    if ((channelConfig & backPair) != 0) {
        if ((channelConfig & backPair) != backPair) {
            Logger::E(TAG, "Rear channels can't be used independently");
            return FALSE;
        }
    }
    return TRUE;
}

ECode CAudioTrack::AudioBuffSizeCheck(
    /* [in] */ Int32 audioBufferSize)
{
    // NB: this section is only valid with PCM data.
    //     To update when supporting compressed formats
    Int32 frameSizeInBytes = mChannelCount *
        (mAudioFormat == IAudioFormat::ENCODING_PCM_8BIT ? 1 : 2);
    if ((audioBufferSize % frameSizeInBytes != 0) || (audioBufferSize < 1)) {
        Logger::E(TAG, "Invalid audio buffer size.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    mNativeBufferSizeInBytes = audioBufferSize;
    return NOERROR;
}

ECode CAudioTrack::ReleaseResources()
{
    // even though native_release() stops the native AudioTrack, we need to stop
    // AudioTrack subclasses too.
//    try {
    Stop();
//    } catch(IllegalStateException ise) {
//        // don't raise an exception, we're releasing the resources.
//    }

    NativeRelease();
    mState = STATE_UNINITIALIZED;
    return NOERROR;
}

//--------------------------------------------------------------------------
// Getters
//--------------------

ECode CAudioTrack::GetMinVolume(
    /* [out] */ Float* minVolume)
{
    VALIDATE_NOT_NULL(minVolume);
    *minVolume = VOLUME_MIN;
    return NOERROR;
}

ECode CAudioTrack::GetMaxVolume(
    /* [out] */ Float* maxVolume)
{
    VALIDATE_NOT_NULL(maxVolume);
    *maxVolume = VOLUME_MAX;
    return NOERROR;
}

ECode CAudioTrack::GetSampleRate(
    /* [out] */ Int32* rate)
{
    VALIDATE_NOT_NULL(rate);
    *rate = mSampleRate;
    return NOERROR;
}

ECode CAudioTrack::GetPlaybackRate(
    /* [out] */ Int32* rate)
{
    assert(mNativeTrack != 0);
    VALIDATE_NOT_NULL(rate);
    *rate = NativeGetPlaybackRate();
    return NOERROR;
}

ECode CAudioTrack::GetAudioFormat(
    /* [out] */ Int32* format)
{
    VALIDATE_NOT_NULL(format);
    *format = mAudioFormat;
    return NOERROR;
}

ECode CAudioTrack::GetStreamType(
    /* [out] */ Int32* type)
{
    VALIDATE_NOT_NULL(type);
    *type = mStreamType;
    return NOERROR;
}

ECode CAudioTrack::GetChannelConfiguration(
    /* [out] */ Int32* config)
{
    VALIDATE_NOT_NULL(config);
    *config = mChannelConfiguration;
    return NOERROR;
}

ECode CAudioTrack::GetChannelCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    *count = mChannelCount;
    return NOERROR;
}

ECode CAudioTrack::GetState(
    /* [out] */ Int32* state)
{
    VALIDATE_NOT_NULL(state);
    *state = mState;
    return NOERROR;
}

ECode CAudioTrack::GetPlayState(
    /* [out] */ Int32* state)
{
    VALIDATE_NOT_NULL(state);
    {
        AutoLock lock(&mPlayStateLock);
        *state = mPlayState;
    }
    return NOERROR;
}

Int32 CAudioTrack::GetNativeFrameCount()
{
    assert(mNativeTrack != 0);
    return NativeGetNativeFrameCount();
}

ECode CAudioTrack::GetNotificationMarkerPosition(
    /* [out] */ Int32* pos)
{
    assert(mNativeTrack != 0);
    VALIDATE_NOT_NULL(pos);
    *pos = NativeGetMarkerPos();
    return NOERROR;
}

ECode CAudioTrack::GetPositionNotificationPeriod(
    /* [out] */ Int32* period)
{
    assert(mNativeTrack != 0);
    VALIDATE_NOT_NULL(period);
    *period = NativeGetPosUpdatePeriod();
    return NOERROR;
}

ECode CAudioTrack::GetPlaybackHeadPosition(
    /* [out] */ Int32* position)
{
    assert(mNativeTrack != 0);
    VALIDATE_NOT_NULL(position);
    *position = NativeGetPosition();
    return NOERROR;
}

ECode CAudioTrack::GetNativeOutputSampleRate(
    /* [in] */ Int32 streamType,
    /* [out] */ Int32* rate)
{
    VALIDATE_NOT_NULL(rate);
    *rate = NativeGetOutputSampleRate(streamType);
    return NOERROR;
}

ECode CAudioTrack::GetMinBufferSize(
    /* [in] */ Int32 sampleRateInHz,
    /* [in] */ Int32 channelConfig,
    /* [in] */ Int32 audioFormat,
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);
    Int32 channelCount = 0;
    switch(channelConfig) {
    case IAudioFormat::CHANNEL_OUT_MONO:
    case IAudioFormat::CHANNEL_CONFIGURATION_MONO:
        channelCount = 1;
        break;
    case IAudioFormat::CHANNEL_OUT_STEREO:
    case IAudioFormat::CHANNEL_CONFIGURATION_STEREO:
        channelCount = 2;
        break;
    default:
        if ((channelConfig & SUPPORTED_OUT_CHANNELS) != channelConfig) {
        // input channel configuration features unsupported channels
        Logger::E(TAG, "getMinBufferSize(): Invalid channel configuration.");
        *size = IAudioTrack::ERROR_BAD_VALUE;
        return NOERROR;
    }
        else {
            channelCount = Elastos::Core::Math::BitCount(channelConfig);
            return NOERROR;
        }
     }

    if ((audioFormat != IAudioFormat::ENCODING_PCM_16BIT)
            && (audioFormat != IAudioFormat::ENCODING_PCM_8BIT)) {
        Logger::E(TAG, "getMinBufferSize(): Invalid audio format.");
        *size = IAudioTrack::ERROR_BAD_VALUE;
        return NOERROR;
    }

    if ( (sampleRateInHz < 4000) || (sampleRateInHz > 48000) ) {
        Logger::E(TAG, "getMinBufferSize(): %d Hz is not a supported sample rate.", sampleRateInHz);
        *size = IAudioTrack::ERROR_BAD_VALUE;
        return NOERROR;
    }

    Int32 bufferSize = NativeGetMinBuffSize(sampleRateInHz, channelCount, audioFormat);
    if ((bufferSize == -1) || (bufferSize == 0)) {
        Logger::E(TAG, "getMinBufferSize(): error querying hardware");
        *size = IAudioTrack::ERROR;
        return NOERROR;
    }
    else {
        *size = bufferSize;
    }
    return NOERROR;
}

ECode CAudioTrack::GetAudioSessionId(
    /* [out] */ Int32 * id)
{
    VALIDATE_NOT_NULL(id);
    *id = mSessionId;
    return NOERROR;
}

//--------------------------------------------------------------------------
// Initialization / configuration
//--------------------
ECode CAudioTrack::SetPlaybackPositionUpdateListener(
    /* [in] */ IOnPlaybackPositionUpdateListener *listener)
{
    return SetPlaybackPositionUpdateListener(listener, NULL);
}

ECode CAudioTrack::SetPlaybackPositionUpdateListener(
    /* [in] */ IOnPlaybackPositionUpdateListener *listener,
    /* [in] */ IHandler *handler)
{
    {
        AutoLock lock(&mPositionListenerLock);

        mPositionListener = listener;
    }
    if (listener != NULL) {
        mEventHandlerDelegate = new NativeEventHandlerDelegate(this, handler);
    }
    return NOERROR;
}

ECode CAudioTrack::SetStereoVolume(
    /* [in] */ Float leftVolume,
    /* [in] */ Float rightVolume,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    if (mState != STATE_INITIALIZED) {
        *result = IAudioTrack::ERROR_INVALID_OPERATION;
        return NOERROR;
    }

    // clamp the volumes
    Float minVolume, maxVolume;
    GetMinVolume(&minVolume);
    GetMaxVolume(&maxVolume);

    if (leftVolume < minVolume) {
        leftVolume = minVolume;
    }
    if (leftVolume > maxVolume) {
        leftVolume = maxVolume;
    }
    if (rightVolume < minVolume) {
        rightVolume = minVolume;
    }
    if (rightVolume > maxVolume) {
        rightVolume = maxVolume;
    }

    NativeSetVolume(leftVolume, rightVolume);

    *result = IAudioTrack::SUCCESS;
    return NOERROR;
}

ECode CAudioTrack::SetPlaybackRate(
    /* [in] */ Int32 sampleRateInHz,
    /* [out] */ Int32* rate)
{
    VALIDATE_NOT_NULL(rate);
    if (mState != STATE_INITIALIZED) {
        *rate = IAudioTrack::ERROR_INVALID_OPERATION;
        return NOERROR;
    }
    if (sampleRateInHz <= 0) {
        *rate = IAudioTrack::ERROR_BAD_VALUE;
        return NOERROR;
    }
    *rate = NativeSetPlaybackRate(sampleRateInHz);
    return NOERROR;
}

ECode CAudioTrack::SetNotificationMarkerPosition(
    /* [in] */ Int32 markerInFrames,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    if (mState != STATE_INITIALIZED) {
        *result = IAudioTrack::ERROR_INVALID_OPERATION;
        return NOERROR;
    }
    *result = NativeSetMarkerPos(markerInFrames);
    return NOERROR;
}

ECode CAudioTrack::SetPositionNotificationPeriod(
    /* [in] */ Int32 periodInFrames,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    if (mState != STATE_INITIALIZED) {
        *result = IAudioTrack::ERROR_INVALID_OPERATION;
        return NOERROR;
    }
    *result = NativeSetPosUpdatePeriod(periodInFrames);
    return NOERROR;
}

ECode CAudioTrack::SetPlaybackHeadPosition(
    /* [in] */ Int32 positionInFrames,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    AutoLock lock(&mPositionListenerLock);

    if ((mPlayState == PLAYSTATE_STOPPED) || (mPlayState == PLAYSTATE_PAUSED)) {
        *result = NativeSetPosition(positionInFrames);
    }
    else {
        *result = IAudioTrack::ERROR_INVALID_OPERATION;
    }
    return NOERROR;
}

ECode CAudioTrack::SetLoopPoints(
    /* [in] */ Int32 startInFrames,
    /* [in] */ Int32 endInFrames,
    /* [in] */ Int32 loopCount,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    if (mDataLoadMode == MODE_STREAM) {
        *result = IAudioTrack::ERROR_INVALID_OPERATION;
        return NOERROR;
    }
    *result = NativeSetLoop(startInFrames, endInFrames, loopCount);
    return NOERROR;
}

/**
 * Sets the initialization state of the instance. To be used in an AudioTrack subclass
 * constructor to set a subclass-specific post-initialization state.
 * @param state the state of the AudioTrack instance
 */
void CAudioTrack::SetState(
    /* [in] */ Int32 state)
{
    mState = state;
}

//---------------------------------------------------------
// Transport control methods
//--------------------
ECode CAudioTrack::Play()
{
    if (mState != STATE_INITIALIZED) {
        Logger::E(TAG, "play() called on uninitialized AudioTrack.");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    AutoLock lock(&mPlayStateLock);

    NativeStart();
    mPlayState = PLAYSTATE_PLAYING;
    return NOERROR;
}

ECode CAudioTrack::Stop()
{
    if (mState != STATE_INITIALIZED) {
        Logger::E(TAG, "stop() called on uninitialized AudioTrack.");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    // stop playing
    AutoLock lock(&mPlayStateLock);

    NativeStop();
    mPlayState = PLAYSTATE_STOPPED;
    return NOERROR;
}

ECode CAudioTrack::Pause()
{
    if (mState != STATE_INITIALIZED) {
        Logger::E(TAG, "pause() called on uninitialized AudioTrack.");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    //logd("pause()");

    // pause playback
    AutoLock lock(&mPlayStateLock);

    NativePause();
    mPlayState = PLAYSTATE_PAUSED;
    return NOERROR;
}

//---------------------------------------------------------
// Audio data supply
//--------------------
ECode CAudioTrack::Flush()
{
    if (mState == STATE_INITIALIZED) {
         // flush the data in native layer
         NativeFlush();
     }
    return NOERROR;
}

ECode CAudioTrack::Write(
    /* [in] */ ArrayOf<Byte>* audioData,
    /* [in] */ Int32 offsetInBytes,
    /* [in] */ Int32 sizeInBytes,
    /* [out] */ Int32* num)
{
    VALIDATE_NOT_NULL(num);
    if ((mDataLoadMode == MODE_STATIC) && (mState == STATE_NO_STATIC_DATA) && (sizeInBytes > 0)) {
        mState = STATE_INITIALIZED;
    }

    if (mState != STATE_INITIALIZED) {
        *num = IAudioTrack::ERROR_INVALID_OPERATION;
        return NOERROR;
    }

    if ( (audioData == NULL) || (offsetInBytes < 0 ) || (sizeInBytes < 0)
            || (offsetInBytes + sizeInBytes > audioData->GetLength() )) {
        *num = IAudioTrack::ERROR_BAD_VALUE;
        return NOERROR;
    }

    *num = NativeWriteByte(audioData, offsetInBytes, sizeInBytes, mAudioFormat);
    return NOERROR;
}

ECode CAudioTrack::Write(
    /* [in] */ ArrayOf<Int16>* audioData,
    /* [in] */ Int32 offsetInShorts,
    /* [in] */ Int32 sizeInShorts,
    /* [out] */ Int32* num)
{
    VALIDATE_NOT_NULL(num);
    if ((mDataLoadMode == MODE_STATIC)
        && (mState == STATE_NO_STATIC_DATA)
        && (sizeInShorts > 0)) {
        mState = STATE_INITIALIZED;
    }

    if (mState != STATE_INITIALIZED) {
        *num = IAudioTrack::ERROR_INVALID_OPERATION;
        return NOERROR;
    }

    if ( (audioData == NULL) || (offsetInShorts < 0 ) || (sizeInShorts < 0)
            || (offsetInShorts + sizeInShorts > audioData->GetLength())) {
        *num = IAudioTrack::ERROR_BAD_VALUE;
        return NOERROR;
    }

    *num = NativeWriteInt16(audioData, offsetInShorts, sizeInShorts, mAudioFormat);
    return NOERROR;
}

ECode CAudioTrack::ReloadStaticData(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    if (mDataLoadMode == MODE_STREAM) {
        *result = IAudioTrack::ERROR_INVALID_OPERATION;
        return NOERROR;
    }
    *result = NativeReloadStatic();
    return NOERROR;
}

//--------------------------------------------------------------------------
// Audio effects management
//--------------------
ECode CAudioTrack::AttachAuxEffect(
    /* [in] */ Int32 effectId,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    if (mState != STATE_INITIALIZED) {
        *result = IAudioTrack::ERROR_INVALID_OPERATION;
        return NOERROR;
    }
    *result = NativeAttachAuxEffect(effectId);
    return NOERROR;
}

ECode CAudioTrack::SetAuxEffectSendLevel(
    /* [in] */ Float level,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    if (mState != STATE_INITIALIZED) {
        *result = IAudioTrack::ERROR_INVALID_OPERATION;
        return NOERROR;
    }

    // clamp the level
    Float minVolume, maxVolume;
    GetMinVolume(&minVolume);
    GetMaxVolume(&maxVolume);
    if (level < minVolume) {
        level = minVolume;
    }
    if (level > maxVolume) {
        level = maxVolume;
    }
    NativeSetAuxEffectSendLevel(level);
    *result = IAudioTrack::SUCCESS;
    return NOERROR;
}

ECode CAudioTrack::PostEventFromNative(
    /* [in] */ IInterface* ref,
    /* [in] */ Int32 what,
    /* [in] */ Int32 arg1,
    /* [in] */ Int32 arg2,
    /* [in] */ IInterface* obj)
{
    //logd("Event posted from the native side: event="+ what + " args="+ arg1+" "+arg2);
    IAudioTrack* iat = IAudioTrack::Probe(ref);
    if (iat == NULL) return NOERROR;

    AutoPtr<CAudioTrack> track = (CAudioTrack*)iat;

    if (track->mEventHandlerDelegate != NULL) {
        AutoPtr<IHandler> handler = track->mEventHandlerDelegate->GetHandler();
        AutoPtr<IMessage> message;
        handler->ObtainMessage(what, arg1, arg2, obj, (IMessage**)&message);

        Boolean bval;
        return handler->SendMessage(message, &bval);
    }
    return NOERROR;
}

#define DEFAULT_OUTPUT_SAMPLE_RATE 44100;

// //Native method
// #define SUCCESS                         0
// #define ERROR                           -1
// #define ERROR_BAD_VALUE                 -2
// #define ERROR_INVALID_OPERATION         -3
// #define ERROR_SETUP_AUDIOSYSTEM         -16
// #define ERROR_SETUP_INVALIDCHANNELMASK -17
// #define ERROR_SETUP_INVALIDFORMAT       -18
// #define ERROR_SETUP_INVALIDSTREAMTYPE   -19
// #define ERROR_SETUP_NATIVEINITFAILED    -20

// struct fields_t
// {
//     // these fields provide access from C++ to the...
//     Int32       PCM16;                 //...  format constants
//     Int32       PCM8;                  //...  format constants
//     Int32       STREAM_VOICE_CALL;     //...  stream type constants
//     Int32       STREAM_SYSTEM;         //...  stream type constants
//     Int32       STREAM_RING;           //...  stream type constants
//     Int32       STREAM_MUSIC;          //...  stream type constants
//     Int32       STREAM_ALARM;          //...  stream type constants
//     Int32       STREAM_NOTIFICATION;   //...  stream type constants
//     Int32       STREAM_BLUETOOTH_SCO;  //...  stream type constants
//     Int32       STREAM_DTMF;           //...  stream type constants
//     Int32       MODE_STREAM;           //...  memory mode
//     Int32       MODE_STATIC;           //...  memory mode
// };

// static fields_t AudioTrackFields;

struct audiotrack_callback_cookie
{
    IAudioTrack* mAudioTrackRef;
};

class AudioTrackJniStorage
{
public:
    AudioTrackJniStorage()
    {
        mCallbackData.mAudioTrackRef = NULL;
        mStreamType = AUDIO_STREAM_DEFAULT;
    }

    ~AudioTrackJniStorage()
    {
        mMemBase.clear();
        mMemHeap.clear();
    }

    CARAPI_(Boolean) AllocSharedMem(
        /* [in] */ Int32 sizeInBytes)
    {
        mMemHeap = new android::MemoryHeapBase(sizeInBytes, 0, "AudioTrack Heap Base");
        if (mMemHeap->getHeapID() < 0) {
            return FALSE;
        }
        mMemBase = new android::MemoryBase(mMemHeap, 0, sizeInBytes);
        return TRUE;
    }

public:
    android::sp<android::MemoryHeapBase> mMemHeap;
    android::sp<android::MemoryBase> mMemBase;
    struct audiotrack_callback_cookie mCallbackData;
    Int32 mStreamType;
};

static Int32 AndroidMediaTranslateErrorCode(
    /* [in] */ Int32 code)
{
    switch(code) {
    case android::NO_ERROR:
    return IAudioTrack::SUCCESS;
    case android::BAD_VALUE:
        return IAudioTrack::ERROR_BAD_VALUE;
    case android::INVALID_OPERATION:
        return IAudioTrack::ERROR_INVALID_OPERATION;
    default:
        return IAudioTrack::ERROR;
    }
}

static void audioCallback(int event, void* user, void *info)
{
    if (event == android::AudioTrack::EVENT_MORE_DATA) {
        // set size to 0 to signal we're not using the callback to write more data
        android::AudioTrack::Buffer* pBuff = (android::AudioTrack::Buffer*)info;
        pBuff->size = 0;

    }
    else if (event == android::AudioTrack::EVENT_MARKER) {
        audiotrack_callback_cookie* callbackInfo = (audiotrack_callback_cookie *)user;
        if (user) {
            CAudioTrack::PostEventFromNative(
                callbackInfo->mAudioTrackRef, event, 0, 0, NULL);
        }
    }
    else if (event == android::AudioTrack::EVENT_NEW_POS) {
        audiotrack_callback_cookie* callbackInfo = (audiotrack_callback_cookie *)user;
        if (user) {
            CAudioTrack::PostEventFromNative(
                callbackInfo->mAudioTrackRef, event, 0, 0, NULL);
        }
    }
}

Int32 CAudioTrack::NativeSetup(
    /* [in] */ Int32 streamType,
    /* [in] */ Int32 sampleRateInHertz,
    /* [in] */ Int32 channels,
    /* [in] */ Int32 audioFormat,
    /* [in] */ Int32 buffSizeInBytes,
    /* [in] */ Int32 memoryMode,
    /* [in] */ Int32* nSession)
{
    //LOGV("sampleRate=%d, audioFormat(from Java)=%d, channels=%x, buffSize=%d",sampleRateInHertz, audioFormat, channels, buffSizeInBytes);
    int afSampleRate;
    int afFrameCount;

    if (android::AudioSystem::getOutputFrameCount(&afFrameCount, (audio_stream_type_t)streamType) != android::NO_ERROR) {
        //LOGE("Error creating AudioTrack: Could not get AudioSystem frame count.");
        return ERROR_NATIVESETUP_AUDIOSYSTEM;
    }
    if (android::AudioSystem::getOutputSamplingRate(&afSampleRate, (audio_stream_type_t)streamType) != android::NO_ERROR) {
        //LOGE("Error creating AudioTrack: Could not get AudioSystem sampling rate.");
        return ERROR_NATIVESETUP_AUDIOSYSTEM;
    }

    // Java channel masks don't map directly to the native definition, but it's a simple shift
    // to skip the two deprecated channel configurations "default" and "mono".
    uint32_t nativeChannelMask = ((uint32_t)channels) >> 2;

    if (!audio_is_output_channel(nativeChannelMask)) {
        // ALOGE("Error creating AudioTrack: invalid channel mask.");
        return ERROR_NATIVESETUP_INVALIDCHANNELMASK;
    }

    Int32 nbChannels = popcount(nativeChannelMask);

    // check the stream type
    audio_stream_type_t atStreamType;
    switch (streamType) {
    case AUDIO_STREAM_VOICE_CALL:
    case AUDIO_STREAM_SYSTEM:
    case AUDIO_STREAM_RING:
    case AUDIO_STREAM_MUSIC:
    case AUDIO_STREAM_ALARM:
    case AUDIO_STREAM_NOTIFICATION:
    case AUDIO_STREAM_BLUETOOTH_SCO:
    case AUDIO_STREAM_DTMF:
        atStreamType = (audio_stream_type_t)streamType;
        break;
    default:
        // ALOGE("Error creating AudioTrack: unknown stream type.");
        return ERROR_NATIVESETUP_INVALIDSTREAMTYPE;
    }

    // check the format.
    // This function was called from Java, so we compare the format against the Java constants
    if ((audioFormat != IAudioFormat::ENCODING_PCM_16BIT) && (audioFormat != IAudioFormat::ENCODING_PCM_8BIT)) {
        //LOGE("Error creating AudioTrack: unsupported audio format.");
        return ERROR_NATIVESETUP_INVALIDFORMAT;
    }

    // for the moment 8bitPCM in MODE_STATIC is not supported natively in the AudioTrack C++ class
    // so we declare everything as 16bitPCM, the 8->16bit conversion for MODE_STATIC will be handled
    // in android_media_AudioTrack_native_write()
    if ((audioFormat == IAudioFormat::ENCODING_PCM_8BIT)
        && (memoryMode == MODE_STATIC)) {
        //LOGV("android_media_AudioTrack_native_setup(): requesting MODE_STATIC for 8bit \ buff size of %dbytes, switching to 16bit, buff size of %dbytes",buffSizeInBytes, 2*buffSizeInBytes);
        audioFormat = IAudioFormat::ENCODING_PCM_16BIT;
        // we will need twice the memory to store the data
        buffSizeInBytes *= 2;
    }

    // compute the frame count
    Int32 bytesPerSample = audioFormat == IAudioFormat::ENCODING_PCM_16BIT ? 2 : 1;
    audio_format_t format = audioFormat == IAudioFormat::ENCODING_PCM_16BIT ?
            AUDIO_FORMAT_PCM_16_BIT : AUDIO_FORMAT_PCM_8_BIT;
    Int32 frameCount = buffSizeInBytes / (nbChannels * bytesPerSample);

    AudioTrackJniStorage* lpJniStorage = new AudioTrackJniStorage();

    // initialize the callback information:
    // this data will be passed with every AudioTrack callback
    // we use a weak reference so the AudioTrack object can be garbage collected.
    lpJniStorage->mCallbackData.mAudioTrackRef = this;

    lpJniStorage->mStreamType = atStreamType;

    if (nSession == NULL) {
    //    LOGE("Error creating AudioTrack: invalid session ID pointer");
        delete lpJniStorage;
        return IAudioTrack::ERROR;
    }

    Int32 sessionId = nSession[0];

    // create the native AudioTrack object
    android::AudioTrack* lpTrack = new android::AudioTrack();
    if (lpTrack == NULL) {
    //    LOGE("Error creating uninitialized AudioTrack");
        goto native_track_failure;
    }

    // initialize the native AudioTrack object
    if (memoryMode == MODE_STREAM) {

        lpTrack->set(
            atStreamType,// stream type
            sampleRateInHertz,
            format,// word length, PCM
            nativeChannelMask,
            frameCount,
            AUDIO_OUTPUT_FLAG_NONE,// flags
            audioCallback, &(lpJniStorage->mCallbackData),//callback, callback data (user)
            0,// notificationFrames == 0 since not using EVENT_MORE_DATA to feed the AudioTrack
            0,// shared mem
            true,// thread can call Java
            sessionId);// audio session ID
    }
    else if (memoryMode == MODE_STATIC) {
        // AudioTrack is using shared memory

        if (!lpJniStorage->AllocSharedMem(buffSizeInBytes)) {
        //    LOGE("Error creating AudioTrack in static mode: error creating mem heap base");
            goto native_init_failure;
        }

        lpTrack->set(
            atStreamType,// stream type
            sampleRateInHertz,
            format,// word length, PCM
            nativeChannelMask,
            frameCount,
            AUDIO_OUTPUT_FLAG_NONE,// flags
            audioCallback, &(lpJniStorage->mCallbackData),//callback, callback data (user));
            0,// notificationFrames == 0 since not using EVENT_MORE_DATA to feed the AudioTrack
            lpJniStorage->mMemBase,// shared mem
            true,// thread can call Java
            sessionId);// audio session ID
    }

    if (lpTrack->initCheck() != android::NO_ERROR) {
        //LOGE("Error initializing AudioTrack");
        goto native_init_failure;
    }

    // read the audio session ID back from AudioTrack in case we create a new session
    nSession[0] = lpTrack->getSessionId();

    // save our newly created C++ AudioTrack in the "nativeTrackInJavaObj" field
    // of the Java object (in mNativeTrackInJavaObj)
    mNativeTrack = (Int32)lpTrack;

    // save the JNI resources so we can free them later
    //LOGV("storing lpJniStorage: %x\n", (int)lpJniStorage);
    mNativeData = (Int32)lpJniStorage;

    return IAudioTrack::SUCCESS;

    // failures:
native_init_failure:
    delete lpTrack;
    mNativeTrack = 0;

native_track_failure:
    delete lpJniStorage;
    mNativeData = 0;
    return ERROR_NATIVESETUP_NATIVEINITFAILED;
}

void CAudioTrack::NativeFinalize()
{
    //LOGV("android_media_AudioTrack_native_finalize jobject: %x\n", (int)thiz);

    // delete the AudioTrack object
    android::AudioTrack* lpTrack = (android::AudioTrack *)mNativeTrack;
    if (lpTrack) {
        //LOGV("deleting lpTrack: %x\n", (int)lpTrack);
        lpTrack->stop();
        delete lpTrack;
    }

    // delete the JNI data
    AudioTrackJniStorage* pJniStorage = (AudioTrackJniStorage *)mNativeData;
    if (pJniStorage) {
        //LOGV("deleting pJniStorage: %x\n", (int)pJniStorage);
        delete pJniStorage;
    }
}

void CAudioTrack::NativeRelease()
{
    // do everything a call to finalize would
    NativeFinalize();
    // + reset the native resources in the Java object so any attempt to access
    // them after a call to release fails.
    mNativeTrack = 0;
    mNativeData = 0;
}

void CAudioTrack::NativeStart()
{
    android::AudioTrack* lpTrack = (android::AudioTrack *)mNativeTrack;
    assert(lpTrack != NULL);

    lpTrack->start();
}

void CAudioTrack::NativeStop()
{
    android::AudioTrack* lpTrack = (android::AudioTrack *)mNativeTrack;
    assert(lpTrack != NULL);

    lpTrack->stop();
}

void CAudioTrack::NativePause()
{
    android::AudioTrack* lpTrack = (android::AudioTrack *)mNativeTrack;
    assert(lpTrack != NULL);

    lpTrack->pause();
}

void CAudioTrack::NativeFlush()
{
    android::AudioTrack* lpTrack = (android::AudioTrack *)mNativeTrack;
    assert(lpTrack != NULL);

    lpTrack->flush();
}

static Int32 WriteToTrack(
    /* [in] */ android::AudioTrack* pTrack,
    /* [in] */ Int32 audioFormat,
    /* [in] */ Byte* data,
    /* [in] */ Int32 offsetInBytes,
    /* [in] */ Int32 sizeInBytes)
{
    // give the data to the native AudioTrack object (the data starts at the offset)
    ssize_t written = 0;
    // regular write() or copy the data to the AudioTrack's shared memory?
    if (pTrack->sharedBuffer() == 0) {
        written = pTrack->write(data + offsetInBytes, sizeInBytes);
    }
    else {
        if (audioFormat == IAudioFormat::ENCODING_PCM_16BIT) {
            // writing to shared memory, check for capacity
            if ((size_t)sizeInBytes > pTrack->sharedBuffer()->size()) {
                sizeInBytes = pTrack->sharedBuffer()->size();
            }
            memcpy(pTrack->sharedBuffer()->pointer(), data + offsetInBytes, sizeInBytes);
            written = sizeInBytes;
        }
        else if (audioFormat == IAudioFormat::ENCODING_PCM_8BIT) {
            // data contains 8bit data we need to expand to 16bit before copying
            // to the shared memory
            // writing to shared memory, check for capacity,
            // note that input data will occupy 2X the input space due to 8 to 16bit conversion
            if (((size_t)sizeInBytes) * 2 > pTrack->sharedBuffer()->size()) {
                sizeInBytes = pTrack->sharedBuffer()->size() / 2;
            }
            Int32 count = sizeInBytes;
            int16_t *dst = (int16_t *)pTrack->sharedBuffer()->pointer();
            const int8_t *src = (const int8_t *)(data + offsetInBytes);
            while(count--) {
                *dst++ = (int16_t)(*src++^0x80) << 8;
            }
            // even though we wrote 2*sizeInBytes, we only report sizeInBytes as written to hide
            // the 8bit mixer restriction from the user of this function
            written = sizeInBytes;
        }
    }
    return written;
}

Int32 CAudioTrack::NativeWriteByte(
    /* [in] */ ArrayOf<Byte>* audioData,
    /* [in] */ Int32 offsetInBytes,
    /* [in] */ Int32 sizeInBytes,
    /* [in] */ Int32 javaAudioFormat)
{
    Byte* cAudioData = NULL;
    android::AudioTrack *lpTrack = NULL;

    //LOGV("android_media_AudioTrack_native_write(offset=%d, sizeInBytes=%d) called",offsetInBytes, sizeInBytes);
    lpTrack = (android::AudioTrack *)mNativeTrack;
    assert(lpTrack != NULL);

    // get the pointer for the audio data from the java array
    if (audioData) {
         cAudioData = audioData->GetPayload();
        if (cAudioData == NULL) {
            //LOGE("Error retrieving source of audio data to play, can't play");
            return 0; // out of memory or no data to load
        }
    }
    else {
        //LOGE("NULL java array of audio data to play, can't play");
        return 0;
    }

    Int32 written = WriteToTrack(lpTrack, javaAudioFormat, cAudioData, offsetInBytes, sizeInBytes);

    //LOGV("write wrote %d (tried %d) bytes in the native AudioTrack with offset %d",
    //     (int)written, (int)(sizeInBytes), (int)offsetInBytes);
    return written;
}

Int32 CAudioTrack::NativeWriteInt16(
    /* [in] */ ArrayOf<Int16>* javaAudioData,
    /* [in] */ Int32 offsetInShorts,
    /* [in] */ Int32 sizeInShorts,
    /* [in] */ Int32 javaAudioFormat)
{
    return (NativeWriteByte((ArrayOf<Byte>*)javaAudioData,
            offsetInShorts * 2, sizeInShorts * 2, javaAudioFormat)
            / 2);
}

Int32 CAudioTrack::NativeReloadStatic()
{
    android::AudioTrack* lpTrack = (android::AudioTrack *)mNativeTrack;
    assert(lpTrack != NULL);

    return AndroidMediaTranslateErrorCode(lpTrack->reload());
}

Int32 CAudioTrack::NativeGetNativeFrameCount()
{
    android::AudioTrack* lpTrack = (android::AudioTrack *)mNativeTrack;
    assert(lpTrack != NULL);

    return lpTrack->frameCount();
}

void CAudioTrack::NativeSetVolume(
    /* [in] */ Float left,
    /* [in] */ Float right)
{
    android::AudioTrack* lpTrack = (android::AudioTrack *)mNativeTrack;
    assert(lpTrack != NULL);

    lpTrack->setVolume(left, right);
}

Int32 CAudioTrack::NativeSetPlaybackRate(
    /* [in] */ Int32 sampleRateInHz)
{
    android::AudioTrack* lpTrack = (android::AudioTrack *)mNativeTrack;
    assert(lpTrack != NULL);

    return AndroidMediaTranslateErrorCode(lpTrack->setSampleRate(sampleRateInHz));
}

Int32 CAudioTrack::NativeGetPlaybackRate()
{
    android::AudioTrack* lpTrack = (android::AudioTrack *)mNativeTrack;
    assert(lpTrack != NULL);

    return (Int32)lpTrack->getSampleRate();
}

Int32 CAudioTrack::NativeSetMarkerPos(
    /* [in] */ Int32 markerPos)
{
    android::AudioTrack* lpTrack = (android::AudioTrack *)mNativeTrack;
    assert(lpTrack != NULL);

    return AndroidMediaTranslateErrorCode(lpTrack->setMarkerPosition(markerPos));
}

Int32 CAudioTrack::NativeGetMarkerPos()
{
    uint32_t position = 0;

    android::AudioTrack* lpTrack = (android::AudioTrack *)mNativeTrack;
    assert(lpTrack != NULL);

    lpTrack->getMarkerPosition(&position);
    return (Int32)position;
}

Int32 CAudioTrack::NativeSetPosUpdatePeriod(
    /* [in] */ Int32 periodInFrames)
{
    android::AudioTrack* lpTrack = (android::AudioTrack *)mNativeTrack;
    assert(lpTrack != NULL);

    return AndroidMediaTranslateErrorCode(lpTrack->setPositionUpdatePeriod(periodInFrames));
}

Int32 CAudioTrack::NativeGetPosUpdatePeriod()
{
    uint32_t period = 0;

    android::AudioTrack* lpTrack = (android::AudioTrack *)mNativeTrack;
    assert(lpTrack != NULL);

    lpTrack->getPositionUpdatePeriod(&period);
    return (Int32)period;
}

Int32 CAudioTrack::NativeSetPosition(
    /* [in] */ Int32 position)
{
    android::AudioTrack* lpTrack = (android::AudioTrack *)mNativeTrack;
    assert(lpTrack != NULL);

    return AndroidMediaTranslateErrorCode(lpTrack->setPosition(position));
}

Int32 CAudioTrack::NativeGetPosition()
{
    uint32_t position = 0;

    android::AudioTrack* lpTrack = (android::AudioTrack *)mNativeTrack;
    assert(lpTrack != NULL);

    lpTrack->getPosition(&position);
    return (Int32)position;
}

Int32 CAudioTrack::NativeSetLoop(
    /* [in] */ Int32 loopStart,
    /* [in] */ Int32 loopEnd,
    /* [in] */ Int32 loopCount)
{
    android::AudioTrack* lpTrack = (android::AudioTrack *)mNativeTrack;
    assert(lpTrack != NULL);

    return AndroidMediaTranslateErrorCode(lpTrack->setLoop(loopStart, loopEnd, loopCount));
}

Int32 CAudioTrack::NativeGetOutputSampleRate(
   /* [in] */ Int32 javaStreamType)
{
    Int32 afSamplingRate;
    // convert the stream type from Java to native value
    // FIXME: code duplication with android_media_AudioTrack_native_setup()
    audio_stream_type_t nativeStreamType;
    switch (javaStreamType) {
    case AUDIO_STREAM_VOICE_CALL:
    case AUDIO_STREAM_SYSTEM:
    case AUDIO_STREAM_RING:
    case AUDIO_STREAM_MUSIC:
    case AUDIO_STREAM_ALARM:
    case AUDIO_STREAM_NOTIFICATION:
    case AUDIO_STREAM_BLUETOOTH_SCO:
    case AUDIO_STREAM_DTMF:
        nativeStreamType = (audio_stream_type_t) javaStreamType;
        break;
    default:
        nativeStreamType = AUDIO_STREAM_DEFAULT;
        break;
    }

    if (android::AudioSystem::getOutputSamplingRate(&afSamplingRate, nativeStreamType) != android::NO_ERROR) {
        // LOGE("AudioSystem::getOutputSamplingRate() for stream type %d failed in AudioTrack JNI",
        //     nativeStreamType);
        return DEFAULT_OUTPUT_SAMPLE_RATE;
    }
    else {
        return afSamplingRate;
    }
}

// returns the minimum required size for the successful creation of a streaming AudioTrack
// returns -1 if there was an error querying the hardware.
Int32 CAudioTrack::NativeGetMinBuffSize(
    /* [in] */ Int32 sampleRateInHertz,
    /* [in] */ Int32 nbChannels,
    /* [in] */ Int32 audioFormat)
{
    Int32 frameCount = 0;
    if (android::AudioTrack::getMinFrameCount(&frameCount, AUDIO_STREAM_DEFAULT,
            sampleRateInHertz) != android::NO_ERROR) {
        return -1;
    }
    return frameCount * nbChannels * (audioFormat == IAudioFormat::ENCODING_PCM_16BIT ? 2 : 1);
}

Int32 CAudioTrack::NativeAttachAuxEffect(
    /* [in] */ Int32 effectId)
{
    android::AudioTrack* lpTrack = (android::AudioTrack *)mNativeTrack;
    assert(lpTrack != NULL);

    return AndroidMediaTranslateErrorCode(lpTrack->attachAuxEffect(effectId));
}

void CAudioTrack::NativeSetAuxEffectSendLevel(
    /* [in] */ Float level)
{
    android::AudioTrack* lpTrack = (android::AudioTrack *)mNativeTrack;
    assert(lpTrack != NULL);

    lpTrack->setAuxEffectSendLevel(level);
}

} // namespace Media
} // namepsace Droid
} // namespace Elastos
