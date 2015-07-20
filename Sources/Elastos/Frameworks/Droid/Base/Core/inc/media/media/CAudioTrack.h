
#ifndef __ELASTOS_DROID_MEDIA_CAUDIOTRACK_H__
#define __ELASTOS_DROID_MEDIA_CAUDIOTRACK_H__

#include "_CAudioTrack.h"
#include "ext/frameworkext.h"
#include "os/HandlerBase.h"

using Elastos::Droid::Os::HandlerBase;

namespace Elastos {
namespace Droid {
namespace Media {

CarClass(CAudioTrack)
{
private:
    class EventHandler
        : public HandlerBase
    {
    friend class CAudioTrack;

    public:
        EventHandler(
            /* [in] */ ILooper* looper,
            /* [in] */ CAudioTrack* track);

        //@Override
        virtual CARAPI HandleMessage(
            /* [in] */ IMessage* msg);

    private:
        CAudioTrack* mAudioTrack;
    };

    class NativeEventHandlerDelegate : public ElRefBase
    {
        friend class CAudioTrack;

    public:
        NativeEventHandlerDelegate(
            /* [in] */ CAudioTrack* track,
            /* [in] */ IHandler* handler);

        CARAPI_(AutoPtr<IHandler>) GetHandler();

    private:
        CAudioTrack* mAudioTrack;
        AutoPtr<IHandler> mHandler;
    };

public:
    static CARAPI GetMinVolume(
        /* [out] */ Float* minVolume);

    static CARAPI GetMaxVolume(
        /* [out] */ Float* maxVolume);

    static CARAPI GetNativeOutputSampleRate(
        /* [in] */ Int32 streamType,
        /* [out] */ Int32* rate);

    static CARAPI GetMinBufferSize(
        /* [in] */ Int32 sampleRateInHz,
        /* [in] */ Int32 channelConfig,
        /* [in] */ Int32 audioFormat,
        /* [out] */ Int32* size);

    static CARAPI PostEventFromNative(
        /* [in] */ IInterface* audiotrack_ref,
        /* [in] */ Int32 what,
        /* [in] */ Int32 arg1,
        /* [in] */ Int32 arg2,
        /* [in] */ IInterface* obj);

    static CARAPI_(Int32) NativeGetOutputSampleRate(
        /* [in] */ Int32 javaStreamType);

    static CARAPI_(Int32) NativeGetMinBuffSize(
        /* [in] */ Int32 sampleRateInHertz,
        /* [in] */ Int32 nbChannels,
        /* [in] */ Int32 audioFormat);

public:
    CAudioTrack();

    ~CAudioTrack();

    //--------------------------------------------------------------------------
    // Constructor, Finalize
    //--------------------
    /**
     * Class constructor.
     * @param streamType the type of the audio stream. See
     *   {@link AudioManager#STREAM_VOICE_CALL}, {@link AudioManager#STREAM_SYSTEM},
     *   {@link AudioManager#STREAM_RING}, {@link AudioManager#STREAM_MUSIC},
     *   {@link AudioManager#STREAM_ALARM}, and {@link AudioManager#STREAM_NOTIFICATION}.
     * @param sampleRateInHz the sample rate expressed in Hertz.
     * @param channelConfig describes the configuration of the audio channels.
     *   See {@link AudioFormat#CHANNEL_OUT_MONO} and
     *   {@link AudioFormat#CHANNEL_OUT_STEREO}
     * @param audioFormat the format in which the audio data is represented.
     *   See {@link AudioFormat#ENCODING_PCM_16BIT} and
     *   {@link AudioFormat#ENCODING_PCM_8BIT}
     * @param bufferSizeInBytes the total size (in bytes) of the buffer where audio data is read
     *   from for playback. If using the AudioTrack in streaming mode, you can write data into
     *   this buffer in smaller chunks than this size. If using the AudioTrack in static mode,
     *   this is the maximum size of the sound that will be played for this instance.
     *   See {@link #getMinBufferSize(int, int, int)} to determine the minimum required buffer size
     *   for the successful creation of an AudioTrack instance in streaming mode. Using values
     *   smaller than getMinBufferSize() will result in an initialization failure.
     * @param mode streaming or static buffer. See {@link #MODE_STATIC} and {@link #MODE_STREAM}
     * @throws java.lang.IllegalArgumentException
     */
    CARAPI constructor(
        /* [in] */ Int32 streamType,
        /* [in] */ Int32 sampleRateInHz,
        /* [in] */ Int32 channelConfig,
        /* [in] */ Int32 audioFormat,
        /* [in] */ Int32 bufferSizeInBytes,
        /* [in] */ Int32 mode);

    /**
     * Class constructor with audio session. Use this constructor when the AudioTrack must be
     * attached to a particular audio session. The primary use of the audio session ID is to
     * associate audio effects to a particular instance of AudioTrack: if an audio session ID
     * is provided when creating an AudioEffect, this effect will be applied only to audio tracks
     * and media players in the same session and not to the output mix.
     * When an AudioTrack is created without specifying a session, it will create its own session
     * which can be retreived by calling the {@link #getAudioSessionId()} method.
     * If a non-zero session ID is provided, this AudioTrack will share effects attached to this
     * session
     * with all other media players or audio tracks in the same session, otherwise a new session
     * will be created for this track if none is supplied.
     * @param streamType the type of the audio stream. See
     *   {@link AudioManager#STREAM_VOICE_CALL}, {@link AudioManager#STREAM_SYSTEM},
     *   {@link AudioManager#STREAM_RING}, {@link AudioManager#STREAM_MUSIC},
     *   {@link AudioManager#STREAM_ALARM}, and {@link AudioManager#STREAM_NOTIFICATION}.
     * @param sampleRateInHz the sample rate expressed in Hertz.
     * @param channelConfig describes the configuration of the audio channels.
     *   See {@link AudioFormat#CHANNEL_OUT_MONO} and
     *   {@link AudioFormat#CHANNEL_OUT_STEREO}
     * @param audioFormat the format in which the audio data is represented.
     *   See {@link AudioFormat#ENCODING_PCM_16BIT} and
     *   {@link AudioFormat#ENCODING_PCM_8BIT}
     * @param bufferSizeInBytes the total size (in bytes) of the buffer where audio data is read
     *   from for playback. If using the AudioTrack in streaming mode, you can write data into
     *   this buffer in smaller chunks than this size. If using the AudioTrack in static mode,
     *   this is the maximum size of the sound that will be played for this instance.
     *   See {@link #getMinBufferSize(int, int, int)} to determine the minimum required buffer size
     *   for the successful creation of an AudioTrack instance in streaming mode. Using values
     *   smaller than getMinBufferSize() will result in an initialization failure.
     * @param mode streaming or static buffer. See {@link #MODE_STATIC} and {@link #MODE_STREAM}
     * @param sessionId Id of audio session the AudioTrack must be attached to
     * @throws java.lang.IllegalArgumentException
     */
    CARAPI constructor(
        /* [in] */ Int32 streamType,
        /* [in] */ Int32 sampleRateInHz,
        /* [in] */ Int32 channelConfig,
        /* [in] */ Int32 audioFormat,
        /* [in] */ Int32 bufferSizeInBytes,
        /* [in] */ Int32 mode,
        /* [in] */ Int32 sessionId);

    CARAPI ReleaseResources();

    CARAPI GetSampleRate(
        /* [out] */ Int32* rate);

    CARAPI GetPlaybackRate(
        /* [out] */ Int32* rate);

    CARAPI GetAudioFormat(
        /* [out] */ Int32* format);

    CARAPI GetStreamType(
        /* [out] */ Int32* type);

    CARAPI GetChannelConfiguration(
        /* [out] */ Int32* config);

    CARAPI GetChannelCount(
        /* [out] */ Int32* count);

    CARAPI GetState(
        /* [out] */ Int32* state);

    CARAPI GetPlayState(
        /* [out] */ Int32* state);

    CARAPI GetNotificationMarkerPosition(
        /* [out] */ Int32* pos);

    CARAPI GetPositionNotificationPeriod(
        /* [out] */ Int32* period);

    CARAPI GetPlaybackHeadPosition(
        /* [out] */ Int32* position);

    CARAPI GetAudioSessionId(
        /* [out] */ Int32* id);

    CARAPI SetPlaybackPositionUpdateListener(
        /* [in] */ IOnPlaybackPositionUpdateListener* listener);

    CARAPI SetPlaybackPositionUpdateListener(
        /* [in] */ IOnPlaybackPositionUpdateListener* listener,
        /* [in] */ IHandler* handler);

    CARAPI SetStereoVolume(
        /* [in] */ Float leftVolume,
        /* [in] */ Float rightVolume,
        /* [out] */ Int32* result);

    CARAPI SetPlaybackRate(
        /* [in] */ Int32 sampleRateInHz,
        /* [out] */ Int32* rate);

    CARAPI SetNotificationMarkerPosition(
        /* [in] */ Int32 markerInFrames,
        /* [out] */ Int32* result);

    CARAPI SetPositionNotificationPeriod(
        /* [in] */ Int32 periodInFrames,
        /* [out] */ Int32* result);

    CARAPI SetPlaybackHeadPosition(
        /* [in] */ Int32 positionInFrames,
        /* [out] */ Int32* result);

    CARAPI SetLoopPoints(
        /* [in] */ Int32 startInFrames,
        /* [in] */ Int32 endInFrames,
        /* [in] */ Int32 loopCount,
        /* [out] */ Int32* result);

    CARAPI Play();

    CARAPI Stop();

    CARAPI Pause();

    CARAPI Flush();

    CARAPI Write(
        /* [in] */ ArrayOf<Byte>* audioData,
        /* [in] */ Int32 offsetInBytes,
        /* [in] */ Int32 sizeInBytes,
        /* [out] */ Int32* num);

    CARAPI Write(
        /* [in] */ ArrayOf<Int16>* audioData,
        /* [in] */ Int32 offsetInShorts,
        /* [in] */ Int32 sizeInShorts,
        /* [out] */ Int32* num);

    CARAPI ReloadStaticData(
        /* [out] */ Int32* result);

    CARAPI AttachAuxEffect(
        /* [in] */ Int32 effectId,
        /* [out] */ Int32* result);

    CARAPI SetAuxEffectSendLevel(
        /* [in] */ Float level,
        /* [out] */ Int32* result);


protected:
    CARAPI_(Int32) GetNativeFrameCount();

    CARAPI_(void) SetState(
        /* [in] */ Int32 state);

private:
    // CARAPI_(Int32) AndroidMediaTranslateErrorCode(
    //     /* [in] */ Int32 code);

    CARAPI AudioParamCheck(
        /* [in] */ Int32 streamType,
        /* [in] */ Int32 sampleRateInHz,
        /* [in] */ Int32 channelConfig,
        /* [in] */ Int32 audioFormat,
        /* [in] */ Int32 mode);

    /*static*/ CARAPI_(Boolean) IsMultichannelConfigSupported(
        /* [in] */ Int32 channelConfig);

    CARAPI AudioBuffSizeCheck(
        /* [in] */ Int32 audioBufferSize);

    CARAPI_(Int32) NativeSetup(
        /* [in] */ Int32 streamType,
        /* [in] */ Int32 sampleRate,
        /* [in] */ Int32 nbChannels,
        /* [in] */ Int32 audioFormat,
        /* [in] */ Int32 buffSizeInBytes,
        /* [in] */ Int32 mode,
        /* [in] */ Int32* nSession);

    CARAPI_(void) NativeFinalize();

    CARAPI_(void) NativeRelease();

    CARAPI_(void) NativeStart();

    CARAPI_(void) NativeStop();

    CARAPI_(void) NativePause();

    CARAPI_(void) NativeFlush();

    CARAPI_(Int32) NativeWriteByte(
        /* [in] */ ArrayOf<Byte>* javaAudioData,
        /* [in] */ Int32 offsetInBytes,
        /* [in] */ Int32 sizeInBytes,
        /* [in] */ Int32 javaAudioFormat);

    CARAPI_(Int32) NativeWriteInt16(
        /* [in] */ ArrayOf<Int16>* javaAudioData,
        /* [in] */ Int32 offsetInShorts,
        /* [in] */ Int32 sizeInShorts,
        /* [in] */ Int32 javaAudioFormat);

    CARAPI_(Int32) NativeReloadStatic();

    CARAPI_(Int32) NativeGetNativeFrameCount();

    CARAPI_(void) NativeSetVolume(
        /* [in] */ Float left,
        /* [in] */ Float right);

    CARAPI_(Int32) NativeSetPlaybackRate(
        /* [in] */ Int32 sampleRateInHz);
    CARAPI_(Int32) NativeGetPlaybackRate();

    CARAPI_(Int32) NativeSetMarkerPos(
        /* [in] */ Int32 markerPos);
    CARAPI_(Int32) NativeGetMarkerPos();

    CARAPI_(Int32) NativeSetPosUpdatePeriod(
        /* [in] */ Int32 periodInFrames);
    CARAPI_(Int32) NativeGetPosUpdatePeriod();

    CARAPI_(Int32) NativeSetPosition(
        /* [in] */ Int32 position);
    CARAPI_(Int32) NativeGetPosition();

    CARAPI_(Int32) NativeSetLoop(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Int32 loopCount);

    CARAPI_(Int32) NativeAttachAuxEffect(
        /* [in] */ Int32 effectId);

    CARAPI_(void) NativeSetAuxEffectSendLevel(
        /* [in] */ Float level);

private:
    friend class CAudioTrackHelper;
    /** Minimum value for a channel volume */
    static const Float VOLUME_MIN; // = 0.0f;
    /** Maximum value for a channel volume */
    static const Float VOLUME_MAX; // = 1.0f;

    static const Int32 ERROR_NATIVESETUP_AUDIOSYSTEM        ; // = -16;
    static const Int32 ERROR_NATIVESETUP_INVALIDCHANNELMASK ; // = -17;
    static const Int32 ERROR_NATIVESETUP_INVALIDFORMAT      ; // = -18;
    static const Int32 ERROR_NATIVESETUP_INVALIDSTREAMTYPE  ; // = -19;
    static const Int32 ERROR_NATIVESETUP_NATIVEINITFAILED   ; // = -20;

    // Events:
    // to keep in sync with frameworks/base/include/media/AudioTrack.h
    /**
     * Event id denotes when playback head has reached a previously set marker.
     */
    static const Int32 NATIVE_EVENT_MARKER ; // = 3;
    /**
     * Event id denotes when previously set update period has elapsed during playback.
     */
    static const Int32 NATIVE_EVENT_NEW_POS; // = 4;

    // mask of all the channels supported by this implementation
    static const Int32 SUPPORTED_OUT_CHANNELS;

    static const String TAG;

    //--------------------------------------------------------------------------
    // Member variables
    //--------------------

    /**
     * Indicates the state of the AudioTrack instance.
     */
    Int32 mState;
    /**
     * Indicates the play state of the AudioTrack instance.
     */
    Int32 mPlayState;
    /**
     * Lock to make sure mPlayState updates are reflecting the actual state of the object.
     */
    Object mPlayStateLock;
    /**
     * The listener the AudioTrack notifies when the playback position reaches a marker
     * or for periodic updates during the progression of the playback head.
     *  @see #setPlaybackPositionUpdateListener(OnPlaybackPositionUpdateListener)
     */
    AutoPtr<IOnPlaybackPositionUpdateListener> mPositionListener;
    /**
     * Lock to protect event listener updates against event notifications.
     */
    Object mPositionListenerLock;
    /**
     * Size of the native audio buffer.
     */
    Int32 mNativeBufferSizeInBytes;
    /**
     * Handler for marker events coming from the native code.
     */
    AutoPtr<NativeEventHandlerDelegate> mEventHandlerDelegate;
    /**
     * Looper associated with the thread that creates the AudioTrack instance.
     */
    AutoPtr<ILooper> mInitializationLooper;
    /**
     * The audio data sampling rate in Hz.
     */
    Int32 mSampleRate;
    /**
     * The number of audio output channels (1 is mono, 2 is stereo).
     */
    Int32 mChannelCount;
    /**
     * The audio channel mask.
     */
    Int32 mChannels;
    /**
     * The type of the audio stream to play. See
     *   {@link AudioManager#STREAM_VOICE_CALL}, {@link AudioManager#STREAM_SYSTEM},
     *   {@link AudioManager#STREAM_RING}, {@link AudioManager#STREAM_MUSIC} and
     *   {@link AudioManager#STREAM_ALARM}
     */
    Int32 mStreamType;
    /**
     * The way audio is consumed by the hardware, streaming or static.
     */
    Int32 mDataLoadMode;
    /**
     * The current audio channel configuration.
     */
    Int32 mChannelConfiguration;
     /**
     * The encoding of the audio samples.
     * @see AudioFormat#ENCODING_PCM_8BIT
     * @see AudioFormat#ENCODING_PCM_16BIT
     */
    Int32 mAudioFormat;
    /**
     * Audio session ID
     */
    Int32 mSessionId;

    /**
     * Accessed by native methods: provides access to C++ AudioTrack object.
     */
    Int32 mNativeTrack;
    /**
     * Accessed by native methods: provides access to the JNI data (i.e. resources used by
     * the native AudioTrack object, but not stored in it).
     */
    Int32 mNativeData;
};

} // namespace Media
} // namepsace Droid
} // namespace Elastos
#endif // __ELASTOS_DROID_MEDIA_CAUDIOTRACK_H__
