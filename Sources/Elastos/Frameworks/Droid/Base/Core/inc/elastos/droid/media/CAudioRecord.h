
#ifndef __ELASTOS_DROID_MEDIA_CAUDIORECORD_H__
#define __ELASTOS_DROID_MEDIA_CAUDIORECORD_H__

#include "_Elastos_Droid_Media_CAudioRecord.h"
#include "elastos/droid/os/HandlerBase.h"

using Elastos::Droid::Os::HandlerBase;
using Elastos::IO::IByteBuffer;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Privacy::IPrivacySettingsManager;
using Elastos::Droid::Content::Pm::IIPackageManager;

namespace Elastos {
namespace Droid {
namespace Media {

CarClass(CAudioRecord)
{
private:
    //---------------------------------------------------------
    // Inner classes
    //---------------------------------------------------------
    /**
     * Helper class to handle the forwarding of native events to the appropriate listener
     * (potentially) handled in a different thread
     */
    class NativeEventHandler
        : public HandlerBase
    {
    public:
        NativeEventHandler(
            /* [in] */ IAudioRecord* recorder,
            /* [in] */ ILooper* looper,
            /* [in] */ CAudioRecord* owner);

        CARAPI HandleMessage(
            /* [in] */ IMessage* msg);

    private:
        AutoPtr<IAudioRecord> mAudioRecord;
        CAudioRecord* mOwner;
    };

public:
    CAudioRecord();

    ~CAudioRecord();

    //---------------------------------------------------------
    // Constructor, Finalize
    //---------------------------------------------------------
    /**
     * Class constructor.
     * @param audioSource the recording source. See {@link MediaRecorder.AudioSource} for
     *    recording source definitions.
     * @param sampleRateInHz the sample rate expressed in Hertz. 44100Hz is currently the only
     *   rate that is guaranteed to work on all devices, but other rates such as 22050,
     *   16000, and 11025 may work on some devices.
     * @param channelConfig describes the configuration of the audio channels.
     *   See {@link AudioFormat#CHANNEL_IN_MONO} and
     *   {@link AudioFormat#CHANNEL_IN_STEREO}.  {@link AudioFormat#CHANNEL_IN_MONO} is guaranteed
     *   to work on all devices.
     * @param audioFormat the format in which the audio data is represented.
     *   See {@link AudioFormat#ENCODING_PCM_16BIT} and
     *   {@link AudioFormat#ENCODING_PCM_8BIT}
     * @param bufferSizeInBytes the total size (in bytes) of the buffer where audio data is written
     *   to during the recording. New audio data can be read from this buffer in smaller chunks
     *   than this size. See {@link #getMinBufferSize(int, int, int)} to determine the minimum
     *   required buffer size for the successful creation of an AudioRecord instance. Using values
     *   smaller than getMinBufferSize() will result in an initialization failure.
     * @throws java.lang.IllegalArgumentException
     */
    CARAPI constructor( // throws IllegalArgumentException
        /* [in] */ Int32 audioSource,
        /* [in] */ Int32 sampleRateInHz,
        /* [in] */ Int32 channelConfig,
        /* [in] */ Int32 audioFormat,
        /* [in] */ Int32 bufferSizeInBytes);

    /**
     * Releases the native AudioRecord resources.
     * The object can no longer be used and the reference should be set to NULL
     * after a call to release()
     */
    CARAPI ReleaseResources();

    //---------------------------------------------------------
    // Getters
    //---------------------------------------------------------
    /**
     * Returns the configured audio data sample rate in Hz
     */
    CARAPI GetSampleRate(
        /* [out] */ Int32* result);

    /**
     * Returns the audio recording source.
     * @see MediaRecorder.AudioSource
     */
    CARAPI GetAudioSource(
        /* [out] */ Int32* result);

    /**
     * Returns the configured audio data format. See {@link AudioFormat#ENCODING_PCM_16BIT}
     * and {@link AudioFormat#ENCODING_PCM_8BIT}.
     */
    CARAPI GetAudioFormat(
        /* [out] */ Int32* result);

    /**
     * Returns the configured channel configuration.
     * See {@link AudioFormat#CHANNEL_IN_MONO}
     * and {@link AudioFormat#CHANNEL_IN_STEREO}.
     */
    CARAPI GetChannelConfiguration(
        /* [out] */ Int32* result);

    /**
     * Returns the configured number of channels.
     */
    CARAPI GetChannelCount(
        /* [out] */ Int32* result);

    /**
     * Returns the state of the AudioRecord instance. This is useful after the
     * AudioRecord instance has been created to check if it was initialized
     * properly. This ensures that the appropriate hardware resources have been
     * acquired.
     * @see AudioRecord#STATE_INITIALIZED
     * @see AudioRecord#STATE_UNINITIALIZED
     */
    CARAPI GetState(
        /* [out] */ Int32* result);

    /**
     * Returns the recording state of the AudioRecord instance.
     * @see AudioRecord#RECORDSTATE_STOPPED
     * @see AudioRecord#RECORDSTATE_RECORDING
     */
    CARAPI GetRecordingState(
        /* [out] */ Int32* result);

    /**
     * Returns the notification marker position expressed in frames.
     */
    CARAPI GetNotificationMarkerPosition(
        /* [out] */ Int32* result);

    /**
     * Returns the notification update period expressed in frames.
     */
    CARAPI GetPositionNotificationPeriod(
        /* [out] */ Int32* result);

    /**
     * Returns the audio session ID.
     *
     * @return the ID of the audio session this AudioRecord belongs to.
     */
    CARAPI GetAudioSessionId(
        /* [out] */ Int32* result);

    //---------------------------------------------------------
    // Transport control methods
    //---------------------------------------------------------
    /**
     * Starts recording from the AudioRecord instance.
     * @throws IllegalStateException
     */
    CARAPI StartRecording();

    /**
     * Starts recording from the AudioRecord instance when the specified synchronization event
     * occurs on the specified audio session.
     * @throws IllegalStateException
     * @param syncEvent event that triggers the capture.
     * @see MediaSyncEvent
     */
    CARAPI StartRecording(
        /* [in] */ IMediaSyncEvent* syncEvent);

    /**
     * Stops recording.
     * @throws IllegalStateException
     */
    CARAPI Stop();

    //---------------------------------------------------------
    // Audio data supply
    //---------------------------------------------------------
    /**
     * Reads audio data from the audio hardware for recording into a buffer.
     * @param audioData the array to which the recorded audio data is written.
     * @param offsetInBytes index in audioData from which the data is written expressed in bytes.
     * @param sizeInBytes the number of requested bytes.
     * @return the number of bytes that were read or or {@link #ERROR_INVALID_OPERATION}
     *    if the object wasn't properly initialized, or {@link #ERROR_BAD_VALUE} if
     *    the parameters don't resolve to valid data and indexes.
     *    The number of bytes will not exceed sizeInBytes.
     */
    CARAPI Read(
        /* [in] */ ArrayOf<Byte>* audioData,
        /* [in] */ Int32 offsetInBytes,
        /* [in] */ Int32 sizeInBytes,
        /* [out] */ Int32* result);

    /**
     * Reads audio data from the audio hardware for recording into a buffer.
     * @param audioData the array to which the recorded audio data is written.
     * @param offsetInShorts index in audioData from which the data is written expressed in shorts.
     * @param sizeInShorts the number of requested shorts.
     * @return the number of shorts that were read or or {@link #ERROR_INVALID_OPERATION}
     *    if the object wasn't properly initialized, or {@link #ERROR_BAD_VALUE} if
     *    the parameters don't resolve to valid data and indexes.
     *    The number of shorts will not exceed sizeInShorts.
     */
    CARAPI Read(
        /* [in] */ ArrayOf<Int16>* audioData,
        /* [in] */ Int32 offsetInShorts,
        /* [in] */ Int32 sizeInShorts,
        /* [out] */ Int32* result);

    /**
     * Reads audio data from the audio hardware for recording into a direct buffer. If this buffer
     * is not a direct buffer, this method will always return 0.
     * @param audioBuffer the direct buffer to which the recorded audio data is written.
     * @param sizeInBytes the number of requested bytes.
     * @return the number of bytes that were read or or {@link #ERROR_INVALID_OPERATION}
     *    if the object wasn't properly initialized, or {@link #ERROR_BAD_VALUE} if
     *    the parameters don't resolve to valid data and indexes.
     *    The number of bytes will not exceed sizeInBytes.
     */
    CARAPI Read(
        /* [in] */ IByteBuffer* audioBuffer,
        /* [in] */ Int32 sizeInBytes,
        /* [out] */ Int32* result);

    //---------------------------------------------------------
    // Initialization / configuration
    //---------------------------------------------------------
    /**
     * Sets the listener the AudioRecord notifies when a previously set marker is reached or
     * for each periodic record head position update.
     * @param listener
     */
    CARAPI SetRecordPositionUpdateListener(
        /* [in] */ IAudioRecordOnRecordPositionUpdateListener* listener);

    /**
     * Sets the listener the AudioRecord notifies when a previously set marker is reached or
     * for each periodic record head position update.
     * Use this method to receive AudioRecord events in the Handler associated with another
     * thread than the one in which you created the AudioTrack instance.
     * @param listener
     * @param handler the Handler that will receive the event notification messages.
     */
    CARAPI SetRecordPositionUpdateListener(
        /* [in] */ IAudioRecordOnRecordPositionUpdateListener* listener,
        /* [in] */ IHandler* handler);

    /**
     * Sets the marker position at which the listener is called, if set with
     * {@link #setRecordPositionUpdateListener(OnRecordPositionUpdateListener)} or
     * {@link #setRecordPositionUpdateListener(OnRecordPositionUpdateListener, Handler)}.
     * @param markerInFrames marker position expressed in frames
     * @return error code or success, see {@link #SUCCESS}, {@link #ERROR_BAD_VALUE},
     *  {@link #ERROR_INVALID_OPERATION}
     */
    CARAPI SetNotificationMarkerPosition(
        /* [in] */ Int32 markerInFrames,
        /* [out] */ Int32* result);

    /**
     * Sets the period at which the listener is called, if set with
     * {@link #setRecordPositionUpdateListener(OnRecordPositionUpdateListener)} or
     * {@link #setRecordPositionUpdateListener(OnRecordPositionUpdateListener, Handler)}.
     * @param periodInFrames update period expressed in frames
     * @return error code or success, see {@link #SUCCESS}, {@link #ERROR_INVALID_OPERATION}
     */
    CARAPI SetPositionNotificationPeriod(
        /* [in] */ Int32 periodInFrames,
        /* [out] */ Int32* result);

    /**
     * Returns the minimum buffer size required for the successful creation of an AudioRecord
     * object.
     * Note that this size doesn't guarantee a smooth recording under load, and higher values
     * should be chosen according to the expected frequency at which the AudioRecord instance
     * will be polled for new data.
     * @param sampleRateInHz the sample rate expressed in Hertz.
     * @param channelConfig describes the configuration of the audio channels.
     *   See {@link AudioFormat#CHANNEL_IN_MONO} and
     *   {@link AudioFormat#CHANNEL_IN_STEREO}
     * @param audioFormat the format in which the audio data is represented.
     *   See {@link AudioFormat#ENCODING_PCM_16BIT}.
     * @return {@link #ERROR_BAD_VALUE} if the recording parameters are not supported by the
     *  hardware, or an invalid parameter was passed,
     *  or {@link #ERROR} if the implementation was unable to query the hardware for its
     *  output properties,
     *   or the minimum buffer size expressed in bytes.
     * @see #AudioRecord(int, int, int, int, int) for more information on valid
     *   configuration values.
     */
    static CARAPI GetMinBufferSize(
        /* [in] */ Int32 sampleRateInHz,
        /* [in] */ Int32 channelConfig,
        /* [in] */ Int32 audioFormat,
        /* [out] */ Int32* result);

protected:
    //@Override
    CARAPI_(void) Finalize();

private:
    /**
     * {@hide}
     * @return package names of current process which is using this object or null if something went wrong
     */
    CARAPI_( AutoPtr< ArrayOf< String > > ) GetPackageName();

    /**
     * {@hide}
     * This method sets up all variables which are needed for privacy mode! It also writes to privacyMode, if everything was successfull or not!
     * -> privacyMode = true ok! otherwise false!
     * CALL THIS METHOD IN CONSTRUCTOR!
     */
    CARAPI_(void) Initiate();

    /**
     * {@hide}
     * This method should be used, because in some devices the uid has more than one package within!
     * @return IS_ALLOWED (-1) if all packages allowed, IS_NOT_ALLOWED(-2) if one of these packages not allowed, GOT_ERROR (-3) if something went wrong
     */
    CARAPI_(Int32) CheckIfPackagesAllowed();

    /**
     * Loghelper method, true = access successful, false = blocked access
     * {@hide}
     */
    CARAPI_(void) DataAccess(
        /* [in] */ Boolean success);

    // Convenience method for the constructor's parameter checks.
    // This is where constructor IllegalArgumentException-s are thrown
    // postconditions:
    //    mRecordSource is valid
    //    mChannelCount is valid
    //    mChannels is valid
    //    mAudioFormat is valid
    //    mSampleRate is valid
    CARAPI AudioParamCheck(
        /* [in] */ Int32 audioSource,
        /* [in] */ Int32 sampleRateInHz,
        /* [in] */ Int32 channelConfig,
        /* [in] */ Int32 audioFormat);

    // Convenience method for the contructor's audio buffer size check.
    // preconditions:
    //    mChannelCount is valid
    //    mAudioFormat is AudioFormat.ENCODING_PCM_8BIT OR AudioFormat.ENCODING_PCM_16BIT
    // postcondition:
    //    mNativeBufferSizeInBytes is valid (multiple of frame size, positive)
    CARAPI AudioBuffSizeCheck(
        /* [in] */ Int32 audioBufferSize);

    //---------------------------------------------------------
    // Java methods called from the native side
    //---------------------------------------------------------
    //@SuppressWarnings("unused")
    static CARAPI_(void) PostEventFromNative(
        /* [in] */ IInterface* audiorecord_ref,
        /* [in] */ Int32 what,
        /* [in] */ Int32 arg1,
        /* [in] */ Int32 arg2,
        /* [in] */ IInterface* obj);

    //---------------------------------------------------------
    // Native methods called from the Java side
    //---------------------------------------------------------
    static CARAPI_(void) recorderCallback(Int32 event, void* user, void *info);

    CARAPI_(Int32) Native_setup(
        /* [in] */ IInterface* audiorecord_this,
        /* [in] */ Int32 recordSource,
        /* [in] */ Int32 sampleRate,
        /* [in] */ Int32 nbChannels,
        /* [in] */ Int32 audioFormat,
        /* [in] */ Int32 buffSizeInBytes,
        /* [in] */ ArrayOf<Int32>* sessionId);

    CARAPI_(void) Native_finalize();

    CARAPI_(void) Native_release();

    CARAPI_(Int32) Native_start(
        /* [in] */ Int32 syncEvent,
        /* [in] */ Int32 sessionId);

    CARAPI_(void) Native_stop();

    CARAPI_(Int32) Native_read_in_byte_array(
        /* [in] */ ArrayOf<Byte>* audioData,
        /* [in] */ Int32 offsetInBytes,
        /* [in] */ Int32 sizeInBytes);

    CARAPI_(Int32) Native_read_in_short_array(
        /* [in] */ ArrayOf<Int16>* audioData,
        /* [in] */ Int32 offsetInShorts,
        /* [in] */ Int32 sizeInShorts);

    CARAPI_(Int32) Native_read_in_direct_buffer(
        /* [in] */ IInterface* buffer,
        /* [in] */ Int32 sizeInBytes);

    CARAPI_(Int32) Native_set_marker_pos(
        /* [in] */ Int32 marker);

    CARAPI_(Int32) Native_get_marker_pos();

    CARAPI_(Int32) Native_set_pos_update_period(
        /* [in] */ Int32 updatePeriod);

    CARAPI_(Int32) Native_get_pos_update_period();

    static CARAPI_(Int32) Native_get_min_buff_size(
        /* [in] */ Int32 sampleRateInHz,
        /* [in] */ Int32 channelCount,
        /* [in] */ Int32 audioFormat);

    //---------------------------------------------------------
    // Utility methods
    //---------------------------------------------------------
    static CARAPI_(void) Logd(
        /* [in] */ const String& msg);

    static CARAPI_(void) Loge(
        /* [in] */ const String& msg);

private:
    // Events:
    // to keep in sync with frameworks/base/include/media/AudioRecord.h
    /**
     * Event id denotes when record head has reached a previously set marker.
     */
    static const Int32 NATIVE_EVENT_MARKER;

    /**
     * Event id denotes when previously set update period has elapsed during recording.
     */
    static const Int32 NATIVE_EVENT_NEW_POS;

    static const String TAG; // = "AudioRecord-Java";

    //---------------------------------------------------------
    // Used exclusively by native code
    //---------------------------------------------------------
    /**
     * Accessed by native methods: provides access to C++ AudioRecord object
     */
    //@SuppressWarnings("unused")
    Int32 mNativeRecorderInJavaObj;

    /**
     * Accessed by native methods: provides access to the callback data.
     */
    //@SuppressWarnings("unused")
    Int32 mNativeCallbackCookie;


    //---------------------------------------------------------
    // Member variables
    //---------------------------------------------------------
    /**
     * The audio data sampling rate in Hz.
     */
    Int32 mSampleRate; // = 22050;

    /**
     * The number of input audio channels (1 is mono, 2 is stereo)
     */
    Int32 mChannelCount; // = 1;

    /**
     * The audio channel mask
     */
    Int32 mChannels; // = AudioFormat.CHANNEL_IN_MONO;

    /**
     * The current audio channel configuration
     */
    Int32 mChannelConfiguration; // = AudioFormat.CHANNEL_IN_MONO;

    /**
     * The encoding of the audio samples.
     * @see AudioFormat#ENCODING_PCM_8BIT
     * @see AudioFormat#ENCODING_PCM_16BIT
     */
    Int32 mAudioFormat; // = AudioFormat.ENCODING_PCM_16BIT;

    /**
     * Where the audio data is recorded from.
     */
    Int32 mRecordSource; // = MediaRecorder.AudioSource.DEFAULT;

    /**
     * Indicates the state of the AudioRecord instance.
     */
    Int32 mState; // = STATE_UNINITIALIZED;

    /**
     * Indicates the recording state of the AudioRecord instance.
     */
    Int32 mRecordingState; // = RECORDSTATE_STOPPED;

    /**
     * Lock to make sure mRecordingState updates are reflecting the actual state of the object.
     */
    Object mRecordingStateLock;

    /**
     * The listener the AudioRecord notifies when the record position reaches a marker
     * or for periodic updates during the progression of the record head.
     *  @see #setRecordPositionUpdateListener(OnRecordPositionUpdateListener)
     *  @see #setRecordPositionUpdateListener(OnRecordPositionUpdateListener, Handler)
     */
    AutoPtr<IAudioRecordOnRecordPositionUpdateListener> mPositionListener; // = NULL;

    /**
     * Lock to protect position listener updates against event notifications
     */
    Object mPositionListenerLock;

    /**
     * Handler for marker events coming from the native code
     */
    AutoPtr<NativeEventHandler> mEventHandler; // = NULL;

    /**
     * Looper associated with the thread that creates the AudioRecord instance
     */
    AutoPtr<ILooper> mInitializationLooper;// = NULL;

    /**
     * Size of the native audio buffer.
     */
    Int32 mNativeBufferSizeInBytes; // = 0;

    /**
     * Audio session ID
     */
    Int32 mSessionId; // = 0;

    static const Int32 IS_ALLOWED;
    static const Int32 IS_NOT_ALLOWED;
    static const Int32 GOT_ERROR;

    static const String PRIVACY_TAG;// = "PM,AudioRecord";
    AutoPtr< IContext > mContext;

    AutoPtr< IPrivacySettingsManager > mPrivacySettingsManager;

    Boolean mPrivacyMode;// = FALSE;

    AutoPtr<IIPackageManager> mPackageManager;
};

} // namespace Media
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_MEDIA_CAUDIORECORD_H__
