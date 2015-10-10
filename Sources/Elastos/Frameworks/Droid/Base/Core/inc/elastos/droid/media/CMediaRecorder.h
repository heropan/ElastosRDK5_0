
#ifndef __ELASTOS_DROID_MEDIA_CMEDIARECORDER_H__
#define __ELASTOS_DROID_MEDIA_CMEDIARECORDER_H__

#include "_Elastos_Droid_Media_CMediaRecorder.h"
#include "elastos/droid/ext/frameworkext.h"
#include "os/HandlerBase.h"

using Elastos::Droid::Hardware::IHardwareCamera;
using Elastos::Droid::View::ISurface;
using Elastos::Droid::Os::HandlerBase;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::Pm::IIPackageManager;
using Elastos::Droid::Privacy::IPrivacySettingsManager;
using Elastos::IO::IFileDescriptor;


namespace Elastos {
namespace Droid {
namespace Media {

/**
 * Used to record audio and video. The recording control is based on a
 * simple state machine (see below).
 *
 * <p><img src="{@docRoot}images/mediarecorder_state_diagram.gif" border="0" />
 * </p>
 *
 * <p>A common case of using MediaRecorder to record audio works as follows:
 *
 * <pre>MediaRecorder recorder = new MediaRecorder();
 * recorder.setAudioSource(MediaRecorder.AudioSource.MIC);
 * recorder.setOutputFormat(MediaRecorder.OutputFormat.THREE_GPP);
 * recorder.setAudioEncoder(MediaRecorder.AudioEncoder.AMR_NB);
 * recorder.setOutputFile(PATH_NAME);
 * recorder.prepare();
 * recorder.start();   // Recording is now started
 * ...
 * recorder.stop();
 * recorder.reset();   // You can reuse the object by going back to setAudioSource() step
 * recorder.release(); // Now the object cannot be reused
 * </pre>
 *
 * <p>Applications may want to register for informational and error
 * events in order to be informed of some internal update and possible
 * runtime errors during recording. Registration for such events is
 * done by setting the appropriate listeners (via calls
 * (to {@link #setOnInfoListener(OnInfoListener)}setOnInfoListener and/or
 * {@link #setOnErrorListener(OnErrorListener)}setOnErrorListener).
 * In order to receive the respective callback associated with these listeners,
 * applications are required to create MediaRecorder objects on threads with a
 * Looper running (the main UI thread by default already has a Looper running).
 *
 * <p><strong>Note:</strong> Currently, MediaRecorder does not work on the emulator.
 *
 * <div class="special reference">
 * <h3>Developer Guides</h3>
 * <p>For more information about how to use MediaRecorder for recording video, read the
 * <a href="{@docRoot}guide/topics/media/camera.html#capture-video">Camera</a> developer guide.
 * For more information about how to use MediaRecorder for recording sound, read the
 * <a href="{@docRoot}guide/topics/media/audio-capture.html">Audio Capture</a> developer guide.</p>
 * </div>
 */
CarClass(CMediaRecorder)
{
public:
    class PrivacyRunner : public ThreadBase {
    public:
        PrivacyRunner(
            /* [in] */ CMediaRecorder* owner);

        //@Override /*implements Thread*/
        virtual CARAPI Run();

        CARAPI_(void) SetDelay(
            /* [in] */ Int64 delay)
        {
            mDelay = delay;
        }

        CARAPI_(Int64) GetDelay()
        {
            return mDelay;
        }
    private:
        CMediaRecorder* mMediaRecorder;
        Int32 mDelay;

        static const Int64 OFFSET_DELAY;// = 2500;
    };

    class EventHandler
        : public HandlerBase
    {
    public:
        EventHandler(
            /* [in] */ CMediaRecorder* host,
            /* [in] */ ILooper* looper);

        //@Override
        CARAPI HandleMessage(
            /* [in] */ IMessage* msg);

    private:
        /* Do not change these values without updating their counterparts
         * in include/media/mediarecorder.h!
         */
        static const Int32 MEDIA_RECORDER_EVENT_LIST_START; // = 1;
        static const Int32 MEDIA_RECORDER_EVENT_ERROR; // = 1;
        static const Int32 MEDIA_RECORDER_EVENT_INFO; // = 2;
        static const Int32 MEDIA_RECORDER_EVENT_LIST_END  ; // = 99;

        /* Events related to individual tracks */
        static const Int32 MEDIA_RECORDER_TRACK_EVENT_LIST_START; // = 100;
        static const Int32 MEDIA_RECORDER_TRACK_EVENT_ERROR     ; // = 100;
        static const Int32 MEDIA_RECORDER_TRACK_EVENT_INFO      ; // = 101;
        static const Int32 MEDIA_RECORDER_TRACK_EVENT_LIST_END  ; // = 1000;

        CMediaRecorder* mMediaRecorder;
    };

public:
    CMediaRecorder();

    ~CMediaRecorder();

    /**
     * Default constructor.
     */
    CARAPI constructor();

    /**
     * Sets a Camera to use for recording. Use this function to switch
     * quickly between preview and capture mode without a teardown of
     * the camera object. {@link android.hardware.Camera#unlock()} should be
     * called before this. Must call before prepare().
     *
     * @param c the Camera to use for recording
     */
    CARAPI SetCamera(
        /* [in] */ IHardwareCamera* c);

    /**
     * Sets a Surface to show a preview of recorded media (video). Calls this
     * before prepare() to make sure that the desirable preview display is
     * set. If {@link #setCamera(Camera)} is used and the surface has been
     * already set to the camera, application do not need to call this. If
     * this is called with non-null surface, the preview surface of the camera
     * will be replaced by the new surface. If this method is called with null
     * surface or not called at all, media recorder will not change the preview
     * surface of the camera.
     *
     * @param sv the Surface to use for the preview
     * @see android.hardware.Camera#setPreviewDisplay(android.view.SurfaceHolder)
     */
    CARAPI SetPreviewDisplay(
        /* [in] */ ISurface* sv);

    /**
     * Sets the audio source to be used for recording. If this method is not
     * called, the output file will not contain an audio track. The source needs
     * to be specified before setting recording-parameters or encoders. Call
     * this only before setOutputFormat().
     *
     * @param audio_source the audio source to use
     * @throws IllegalStateException if it is called after setOutputFormat()
     * @see android.media.MediaRecorder.AudioSource
     */
    CARAPI SetAudioSource(
        /* [in] */ Int32 audio_source);

    /**
     * Gets the maximum value for audio sources.
     * @see android.media.MediaRecorder.AudioSource
     */
    static CARAPI GetAudioSourceMax(
        /* [out] */ Int32* result);


    /**
     * Sets the video source to be used for recording. If this method is not
     * called, the output file will not contain an video track. The source needs
     * to be specified before setting recording-parameters or encoders. Call
     * this only before setOutputFormat().
     *
     * @param video_source the video source to use
     * @throws IllegalStateException if it is called after setOutputFormat()
     * @see android.media.MediaRecorder.VideoSource
     */
    CARAPI SetVideoSource(
        /* [in] */ Int32 video_source);

    /**
     * Uses the settings from a CamcorderProfile object for recording. This method should
     * be called after the video AND audio sources are set, and before setOutputFile().
     * If a time lapse CamcorderProfile is used, audio related source or recording
     * parameters are ignored.
     *
     * @param profile the CamcorderProfile to use
     * @see android.media.CamcorderProfile
     */
    CARAPI SetProfile(
        /* [in] */ ICamcorderProfile* profile);

    /**
     * Set video frame capture rate. This can be used to set a different video frame capture
     * rate than the recorded video's playback rate. This method also sets the recording mode
     * to time lapse. In time lapse video recording, only video is recorded. Audio related
     * parameters are ignored when a time lapse recording session starts, if an application
     * sets them.
     *
     * @param fps Rate at which frames should be captured in frames per second.
     * The fps can go as low as desired. However the fastest fps will be limited by the hardware.
     * For resolutions that can be captured by the video camera, the fastest fps can be computed using
     * {@link android.hardware.Camera.Parameters#getPreviewFpsRange(int[])}. For higher
     * resolutions the fastest fps may be more restrictive.
     * Note that the recorder cannot guarantee that frames will be captured at the
     * given rate due to camera/encoder limitations. However it tries to be as close as
     * possible.
     */
    CARAPI SetCaptureRate(
        /* [in] */ Double fps);

    /**
     * Sets the orientation hint for output video playback.
     * This method should be called before prepare(). This method will not
     * trigger the source video frame to rotate during video recording, but to
     * add a composition matrix containing the rotation angle in the output
     * video if the output format is OutputFormat.THREE_GPP or
     * OutputFormat.MPEG_4 so that a video player can choose the proper
     * orientation for playback. Note that some video players may choose
     * to ignore the compostion matrix in a video during playback.
     *
     * @param degrees the angle to be rotated clockwise in degrees.
     * The supported angles are 0, 90, 180, and 270 degrees.
     * @throws IllegalArgumentException if the angle is not supported.
     *
     */
    CARAPI SetOrientationHint(
        /* [in] */ Int32 degrees);

    /**
     * Set and store the geodata (latitude and longitude) in the output file.
     * This method should be called before prepare(). The geodata is
     * stored in udta box if the output format is OutputFormat.THREE_GPP
     * or OutputFormat.MPEG_4, and is ignored for other output formats.
     * The geodata is stored according to ISO-6709 standard.
     *
     * @param latitude latitude in degrees. Its value must be in the
     * range [-90, 90].
     * @param longitude longitude in degrees. Its value must be in the
     * range [-180, 180].
     *
     * @throws IllegalArgumentException if the given latitude or
     * longitude is out of range.
     *
     */
    CARAPI SetLocation(
        /* [in] */ Float latitude,
        /* [in] */ Float longitude);

    /**
     * Sets the format of the output file produced during recording. Call this
     * after setAudioSource()/setVideoSource() but before prepare().
     *
     * <p>It is recommended to always use 3GP format when using the H.263
     * video encoder and AMR audio encoder. Using an MPEG-4 container format
     * may confuse some desktop players.</p>
     *
     * @param output_format the output format to use. The output format
     * needs to be specified before setting recording-parameters or encoders.
     * @throws IllegalStateException if it is called after prepare() or before
     * setAudioSource()/setVideoSource().
     * @see android.media.MediaRecorder.OutputFormat
     */
    CARAPI SetOutputFormat(
        /* [in] */ Int32 output_format);

    /**
     * Sets the width and height of the video to be captured.  Must be called
     * after setVideoSource(). Call this after setOutFormat() but before
     * prepare().
     *
     * @param width the width of the video to be captured
     * @param height the height of the video to be captured
     * @throws IllegalStateException if it is called after
     * prepare() or before setOutputFormat()
     */
    CARAPI SetVideoSize(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    /**
     * Sets the frame rate of the video to be captured.  Must be called
     * after setVideoSource(). Call this after setOutFormat() but before
     * prepare().
     *
     * @param rate the number of frames per second of video to capture
     * @throws IllegalStateException if it is called after
     * prepare() or before setOutputFormat().
     *
     * NOTE: On some devices that have auto-frame rate, this sets the
     * maximum frame rate, not a constant frame rate. Actual frame rate
     * will vary according to lighting conditions.
     */
    CARAPI SetVideoFrameRate(
        /* [in] */ Int32 rate);

    /**
     * Sets the maximum duration (in ms) of the recording session.
     * Call this after setOutFormat() but before prepare().
     * After recording reaches the specified duration, a notification
     * will be sent to the {@link android.media.MediaRecorder.OnInfoListener}
     * with a "what" code of {@link #MEDIA_RECORDER_INFO_MAX_DURATION_REACHED}
     * and recording will be stopped. Stopping happens asynchronously, there
     * is no guarantee that the recorder will have stopped by the time the
     * listener is notified.
     *
     * @param max_duration_ms the maximum duration in ms (if zero or negative, disables the duration limit)
     *
     */
    CARAPI SetMaxDuration(
        /* [in] */ Int32 max_duration_ms);

    /**
     * Sets the maximum filesize (in bytes) of the recording session.
     * Call this after setOutFormat() but before prepare().
     * After recording reaches the specified filesize, a notification
     * will be sent to the {@link android.media.MediaRecorder.OnInfoListener}
     * with a "what" code of {@link #MEDIA_RECORDER_INFO_MAX_FILESIZE_REACHED}
     * and recording will be stopped. Stopping happens asynchronously, there
     * is no guarantee that the recorder will have stopped by the time the
     * listener is notified.
     *
     * @param max_filesize_bytes the maximum filesize in bytes (if zero or negative, disables the limit)
     *
     */
    CARAPI SetMaxFileSize(
        /* [in] */ Int64 max_filesize_bytes);

    /**
     * Sets the audio encoder to be used for recording. If this method is not
     * called, the output file will not contain an audio track. Call this after
     * setOutputFormat() but before prepare().
     *
     * @param audio_encoder the audio encoder to use.
     * @throws IllegalStateException if it is called before
     * setOutputFormat() or after prepare().
     * @see android.media.MediaRecorder.AudioEncoder
     */
    CARAPI SetAudioEncoder(
        /* [in] */ Int32 audio_encoder);

    /**
     * Sets the video encoder to be used for recording. If this method is not
     * called, the output file will not contain an video track. Call this after
     * setOutputFormat() and before prepare().
     *
     * @param video_encoder the video encoder to use.
     * @throws IllegalStateException if it is called before
     * setOutputFormat() or after prepare()
     * @see android.media.MediaRecorder.VideoEncoder
     */
    CARAPI SetVideoEncoder(
        /* [in] */ Int32 video_encoder);

    /**
     * Sets the audio sampling rate for recording. Call this method before prepare().
     * Prepare() may perform additional checks on the parameter to make sure whether
     * the specified audio sampling rate is applicable. The sampling rate really depends
     * on the format for the audio recording, as well as the capabilities of the platform.
     * For instance, the sampling rate supported by AAC audio coding standard ranges
     * from 8 to 96 kHz. Please consult with the related audio coding standard for the
     * supported audio sampling rate.
     *
     * @param samplingRate the sampling rate for audio in samples per second.
     */
    CARAPI SetAudioSamplingRate(
        /* [in] */ Int32 samplingRate);

    /**
     * Sets the number of audio channels for recording. Call this method before prepare().
     * Prepare() may perform additional checks on the parameter to make sure whether the
     * specified number of audio channels are applicable.
     *
     * @param numChannels the number of audio channels. Usually it is either 1 (mono) or 2
     * (stereo).
     */
    CARAPI SetAudioChannels(
        /* [in] */ Int32 numChannels);

    /**
     * Sets the audio encoding bit rate for recording. Call this method before prepare().
     * Prepare() may perform additional checks on the parameter to make sure whether the
     * specified bit rate is applicable, and sometimes the passed bitRate will be clipped
     * internally to ensure the audio recording can proceed smoothly based on the
     * capabilities of the platform.
     *
     * @param bitRate the audio encoding bit rate in bits per second.
     */
    CARAPI SetAudioEncodingBitRate(
        /* [in] */ Int32 bitRate);

    /**
     * Sets the video encoding bit rate for recording. Call this method before prepare().
     * Prepare() may perform additional checks on the parameter to make sure whether the
     * specified bit rate is applicable, and sometimes the passed bitRate will be
     * clipped internally to ensure the video recording can proceed smoothly based on
     * the capabilities of the platform.
     *
     * @param bitRate the video encoding bit rate in bits per second.
     */
    CARAPI SetVideoEncodingBitRate(
        /* [in] */ Int32 bitRate);

    /**
     * Currently not implemented. It does nothing.
     * @deprecated Time lapse mode video recording using camera still image capture
     * is not desirable, and will not be supported.
     * @hide
     */
    CARAPI SetAuxiliaryOutputFile(
        /* [in] */ IFileDescriptor* fd);

    /**
     * Currently not implemented. It does nothing.
     * @deprecated Time lapse mode video recording using camera still image capture
     * is not desirable, and will not be supported.
     * @hide
     */
    CARAPI SetAuxiliaryOutputFile(
        /* [in] */ const String& path);

    /**
     * Pass in the file descriptor of the file to be written. Call this after
     * setOutputFormat() but before prepare().
     *
     * @param fd an open file descriptor to be written into.
     * @throws IllegalStateException if it is called before
     * setOutputFormat() or after prepare()
     */
    CARAPI SetOutputFile(
        /* [in] */ IFileDescriptor* fd);

    /**
     * Sets the path of the output file to be produced. Call this after
     * setOutputFormat() but before prepare().
     *
     * @param path The pathname to use.
     * @throws IllegalStateException if it is called before
     * setOutputFormat() or after prepare()
     */
    CARAPI SetOutputFile(
        /* [in] */ const String& path);

    /**
     * Prepares the recorder to begin capturing and encoding data. This method
     * must be called after setting up the desired audio and video sources,
     * encoders, file format, etc., but before start().
     *
     * @throws IllegalStateException if it is called after
     * start() or before setOutputFormat().
     * @throws IOException if prepare fails otherwise.
     */
    CARAPI Prepare();

    /**
     * Begins capturing and encoding data to the file specified with
     * setOutputFile(). Call this after prepare().
     *
     * <p>Since API level 13, if applications set a camera via
     * {@link #setCamera(Camera)}, the apps can use the camera after this method
     * call. The apps do not need to lock the camera again. However, if this
     * method fails, the apps should still lock the camera back. The apps should
     * not start another recording session during recording.
     *
     * @throws IllegalStateException if it is called before
     * prepare().
     */
    CARAPI Start();

    /**
     * Stops recording. Call this after start(). Once recording is stopped,
     * you will have to configure it again as if it has just been constructed.
     * Note that a RuntimeException is intentionally thrown to the
     * application, if no valid audio/video data has been received when stop()
     * is called. This happens if stop() is called immediately after
     * start(). The failure lets the application take action accordingly to
     * clean up the output file (delete the output file, for instance), since
     * the output file is not properly constructed when this happens.
     *
     * @throws IllegalStateException if it is called before start()
     */
    CARAPI Stop();

    /**
     * Restarts the MediaRecorder to its idle state. After calling
     * this method, you will have to configure it again as if it had just been
     * constructed.
     */
    CARAPI Reset();

    /**
     * Returns the maximum absolute amplitude that was sampled since the last
     * call to this method. Call this only after the setAudioSource().
     *
     * @return the maximum absolute amplitude measured since the last call, or
     * 0 when called for the first time
     * @throws IllegalStateException if it is called before
     * the audio source has been set.
     */
    CARAPI GetMaxAmplitude(
        /* [out] */ Int32* result);

    /**
     * Register a callback to be invoked when an error occurs while
     * recording.
     *
     * @param l the callback that will be run
     */
    CARAPI SetOnErrorListener(
        /* [in] */ IMediaRecorderOnErrorListener* l);

    /**
     * Register a callback to be invoked when an informational event occurs while
     * recording.
     *
     * @param listener the callback that will be run
     */
    CARAPI SetOnInfoListener(
        /* [in] */ IMediaRecorderOnInfoListener* listener);

     /**
      * Releases resources associated with this MediaRecorder object.
      * It is good practice to call this method when you're done
      * using the MediaRecorder. In particular, whenever an Activity
      * of an application is paused (its onPause() method is called),
      * or stopped (its onStop() method is called), this method should be
      * invoked to release the MediaRecorder object, unless the application
      * has a special need to keep the object around. In addition to
      * unnecessary resources (such as memory and instances of codecs)
      * being held, failure to call this method immediately if a
      * MediaRecorder object is no longer needed may also lead to
      * continuous battery consumption for mobile devices, and recording
      * failure for other applications if no multiple instances of the
      * same codec are supported on a device. Even if multiple instances
      * of the same codec are supported, some performance degradation
      * may be expected when unnecessary multiple instances are used
      * at the same time.
      */
    CARAPI ReleaseResources();

private:
    // native implementation
    CARAPI NativeSetOutputFile(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ Int64 offset,
        /* [in] */ Int64 length);

    CARAPI NativePrepare();

    CARAPI NativeReset();

    CARAPI NativeSetup();

    CARAPI NativeFinalize();

    CARAPI SetParameter(
        /* [in] */ const char* nameValuePair);

    /**
     * Called from native code when an interesting event happens.  This method
     * just uses the EventHandler system to post the event back to the main app thread.
     * We use a weak reference to the original MediaRecorder object so that the native
     * code is safe from the object disappearing from underneath it.  (This is
     * the cookie passed to native_setup().)
     */
    static CARAPI PostEventFromNative(
        /* [in] */ IWeakReference* mediarecorder_ref,
        /* [in] */ Int32 what,
        /* [in] */ Int32 arg1,
        /* [in] */ Int32 arg2,
        /* [in] */ IInterface* obj);

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //BEGIN PRIVACY

    CARAPI MakePrivacyPath(
        /* [in] */ const String& path);

    /**
     * PrivacyStop. Should be called within privacyRunner
     */
    CARAPI PrivacyStop();

    /**
     * This method search automatically the current package path and return it. If we haven't found any path, we return the path to SDcard if we are able to write to it.
     * If we're not able to write to sdCard -> return null
     * @return internal path to package directory or path to SDCard if package not found and we have rights to save files on SDCard. If something went wrong or we couldn't find
     * anything of it -> return null
     */
    CARAPI_(String) GetPrivacyPath();

    /**
     * This method does exactly what the method getPrivacyPath() does, but it returns an FileDescriptor to path
     * @return FileDescriptor to privacyFile or null if something went wrong
     */
    CARAPI_( AutoPtr< IFileDescriptor > ) GetPrivacyFileDescriptor();

    /**
     * {@hide}
     * @return package names of current process which is using this object or null if something went wrong
     */
    CARAPI_( AutoPtr< ArrayOf<String> > ) GetPackageName();

    /**
     * {@hide}
     * This method should be used, because in some devices the uid has more than one package within!
     * @param privacySetting the Mode which has to be tested -> MODE_RECORD_AUDIO, MODE_RECORD_BOTH
     * @return IS_ALLOWED (-1) if all packages allowed, IS_NOT_ALLOWED(-2) if one of these packages not allowed, GOT_ERROR (-3) if something went wrong
     */
    CARAPI_(Int32) CheckIfPackagesAllowed(
        /* [in] */ Int32 privacySetting);

    /**
     * {@hide}
     * This method sets up all variables which are needed for privacy mode! It also writes to privacyMode, if everything was successfull or not!
     * -> privacyMode = true ok! otherwise false!
     * CALL THIS METHOD IN CONSTRUCTOR!
     */
    CARAPI Initiate();

     /**
     * Loghelper method, true = access successful, false = blocked access.
     * {@hide}
     */
    CARAPI_(void) DataAccess(
        /* [in] */ Boolean success,
        /* [in] */ Int32 micOrBoth);

    //END PRIVACY
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

public:
    // The two fields below are accessed by native methods
    Handle32 mNativeContext;

    //@SuppressWarnings("unused")
    AutoPtr<ISurface> mSurface;

    static const String TAG;

private:
    friend class JNIMediaRecorderListener;

    String mPath;
    AutoPtr<IFileDescriptor> mFd;
    AutoPtr<EventHandler> mEventHandler;
    AutoPtr<IMediaRecorderOnErrorListener> mOnErrorListener;
    AutoPtr<IMediaRecorderOnInfoListener> mOnInfoListener;

    ////////////////////////////////////////////////////////////////////////////////
    //BEGIN PRIVACY

    /** default value of privacy path. You have to add the package name at the end to write file in directory of the app itself*/
    static const String PRIVACY_PATH_DEF;// = "/data/data/";

    /**
     * This variable will be set if user use path to save file. Only if user is not allowed!
     */
    String mPrivacyPath;

    /**
     * Path where Filedescriptor linked to.
     */
    String mFileDescriptorPath;

    /**
     * This variable will be set if user use FileDescriptor so save file. Only if user is not allowed!
     */
    AutoPtr< IFileDescriptor > mFileDescriptor;

    AutoPtr< PrivacyRunner > mRunner;

    Boolean mDeletedFile;// = FALSE;

    static const Int32 STATE_RECORD_AUDIO; // = 0;
    static const Int32 STATE_RECORD_BOTH; // = 1;
    static const Int32 MODE_RECORD_AUDIO; // = 2;
    static const Int32 MODE_RECORD_BOTH; // = 3;
    static const Int32 IS_ALLOWED; // = -1;
    static const Int32 IS_NOT_ALLOWED; // = -2;
    static const Int32 GOT_ERROR; // = -3;

    static const Int32 MIC_DATA_ACCESS; // = 10;
    static const Int32 BOTH_DATA_ACCESS; // = 11;

    static const String PRIVACY_TAG;// = "PM,MediaRecorder";

    /**
     * {@hide} This context will ever be null, because we dont need it but pass it to the pSetMan!
     */
    AutoPtr< IContext > context;

    AutoPtr< IPrivacySettingsManager > mPrivacySettingsManager;

    AutoPtr< IIPackageManager > mPackageManager;

    Boolean mPrivacyMode;// = FALSE;

    Boolean mStoppedStream;// = FALSE;

    Int32 ACTUAL_STATE;// = STATE_RECORD_AUDIO;

    //END PRIVACY
    ///////////////////////////////////////////////////////////////////////////////////////////////////
};

} // namespace Media
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_MEDIA_CMEDIARECORDER_H__
