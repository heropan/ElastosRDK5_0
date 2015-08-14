
#ifndef __ELASTOS_DROID_MEDIA_VIDEOEDITOR_MEDIAARTISTNATIVEHELPER_H__
#define __ELASTOS_DROID_MEDIA_VIDEOEDITOR_MEDIAARTISTNATIVEHELPER_H__

#include <ext/frameworkext.h>

using Elastos::IO::IInt32Buffer;
using Elastos::Utility::Concurrent::ISemaphore;
using Elastos::Droid::View::ISurface;
using Elastos::Droid::Graphics::IBitmap;
using Elastos::Droid::Graphics::IPaint;

namespace Elastos {
namespace Droid {
namespace Media {
namespace Videoeditor {

/**
 *This class provide Native methods to be used by MediaArtist {@hide}
 */
class MediaArtistNativeHelper
    : public ElRefBase
{
public:
    /** Defines the version. */
    class Version
        : public ElRefBase
    {
    public:
        /** Method which returns the current VIDEOEDITOR version */
        CARAPI GetVersion(
            /* [out] */ Version** result);

    public:
        /** Major version number */
        Int32 major;

        /** Minor version number */
        Int32 minor;

        /** Revision number */
        Int32 revision;

    private:
        /** VIDEOEDITOR major version number */
        static const Int32 VIDEOEDITOR_MAJOR_VERSION;

        /** VIDEOEDITOR minor version number */
        static const Int32 VIDEOEDITOR_MINOR_VERSION;

        /** VIDEOEDITOR revision number */
        static const Int32 VIDEOEDITOR_REVISION_VERSION;
    };

    /**
     * Defines settings for the AlphaMagic transition
     */
    class AlphaMagicSettings
        : public ElRefBase
    {
    public:
        /** Name of the alpha file (JPEG file). */
        String file;

        /** Blending percentage [0..100] 0 = no blending. */
        Int32 blendingPercent;

        /** Invert the default rotation direction of the AlphaMagic effect. */
        Boolean invertRotation;

        Int32 rgbWidth;
        Int32 rgbHeight;
    };

    /** Defines the Slide transition settings. */
    class SlideTransitionSettings
        : public ElRefBase
    {
    public:
        /**
         * Direction of the slide transition. See {@link SlideDirection
         * SlideDirection} for valid values.
         */
        Int32 direction;
    };

    /**
     * Defines the settings of a single clip.
     */
    class ClipSettings
        : public ElRefBase
    {
    public:
        /**
         * The path to the clip file.
         * <p>
         * File format of the clip, it can be:
         * <ul>
         * <li>3GP file containing MPEG4/H263/H264 video and AAC/AMR audio
         * <li>JPG file
         * </ul>
         */

        String clipPath;

        /**
         * The path of the decoded file. This is used only for image files.
         */
        String clipDecodedPath;

        /**
         * The path of the Original file. This is used only for image files.
         */
        String clipOriginalPath;

        /**
         * File type of the clip. See {@link FileType FileType} for valid
         * values.
         */
        Int32 fileType;

        /** Begin of the cut in the clip in milliseconds. */
        Int32 beginCutTime;

        /**
         * End of the cut in the clip in milliseconds. Set both
         * <code>beginCutTime</code> and <code>endCutTime</code> to
         * <code>0</code> to get the full length of the clip without a cut. In
         * case of JPG clip, this is the duration of the JPEG file.
         */
        Int32 endCutTime;

        /**
         * Begin of the cut in the clip in percentage of the file duration.
         */
        Int32 beginCutPercent;

        /**
         * End of the cut in the clip in percentage of the file duration. Set
         * both <code>beginCutPercent</code> and <code>endCutPercent</code> to
         * <code>0</code> to get the full length of the clip without a cut.
         */
        Int32 endCutPercent;

        /** Enable panning and zooming. */
        Boolean panZoomEnabled;

        /** Zoom percentage at start of clip. 0 = no zoom, 100 = full zoom */
        Int32 panZoomPercentStart;

        /** Top left X coordinate at start of clip. */
        Int32 panZoomTopLeftXStart;

        /** Top left Y coordinate at start of clip. */
        Int32 panZoomTopLeftYStart;

        /** Zoom percentage at start of clip. 0 = no zoom, 100 = full zoom */
        Int32 panZoomPercentEnd;

        /** Top left X coordinate at end of clip. */
        Int32 panZoomTopLeftXEnd;

        /** Top left Y coordinate at end of clip. */
        Int32 panZoomTopLeftYEnd;

        /**
         * Set The media rendering. See {@link MediaRendering MediaRendering}
         * for valid values.
         */
        Int32 mediaRendering;

        /**
         * RGB width and Height
         */
        Int32 rgbWidth;
        Int32 rgbHeight;
        /**
         * Video rotation degree.
         */
        Int32 rotationDegree;
    };

    /**
     * Defines settings for a transition.
     */
    class TransitionSettings
        : public ElRefBase
    {
    public:
        /** Duration of the transition in msec. */
        Int32 duration;

        /**
         * Transition type for video. See {@link VideoTransition
         * VideoTransition} for valid values.
         */
        Int32 videoTransitionType;

        /**
         * Transition type for audio. See {@link AudioTransition
         * AudioTransition} for valid values.
         */
        Int32 audioTransitionType;

        /**
         * Transition behaviour. See {@link TransitionBehaviour
         * TransitionBehaviour} for valid values.
         */
        Int32 transitionBehaviour;

        /**
         * Settings for AlphaMagic transition. Only needs to be set if
         * <code>videoTransitionType</code> is set to
         * <code>VideoTransition.ALPHA_MAGIC</code>. See
         * {@link AlphaMagicSettings AlphaMagicSettings}.
         */
        AutoPtr<AlphaMagicSettings> alphaSettings;

        /**
         * Settings for the Slide transition. See
         * {@link SlideTransitionSettings SlideTransitionSettings}.
         */
        AutoPtr<SlideTransitionSettings> slideSettings;
    };

    /**
     * Defines settings for the background music.
     */
    class BackgroundMusicSettings
        : public ElRefBase
    {
    public:
        /** Background music file. */
        String file;

        /** File type. See {@link FileType FileType} for valid values. */
        Int32 fileType;

        /**
         * Insertion time in milliseconds, in the output video where the
         * background music must be inserted.
         */
        Int64 insertionTime;

        /**
         * Volume, as a percentage of the background music track, to use. If
         * this field is set to 100, the background music will replace the audio
         * from the video input file(s).
         */
        Int32 volumePercent;

        /**
         * Start time in milliseconds in the background muisc file from where
         * the background music should loop. Set both <code>beginLoop</code> and
         * <code>endLoop</code> to <code>0</code> to disable looping.
         */
        Int64 beginLoop;

        /**
         * End time in milliseconds in the background music file to where the
         * background music should loop. Set both <code>beginLoop</code> and
         * <code>endLoop</code> to <code>0</code> to disable looping.
         */
        Int64 endLoop;

        Boolean enableDucking;

        Int32 duckingThreshold;

        Int32 lowVolume;

        Boolean isLooping;
    };

    /** Defines the effect settings. */
    class EffectSettings
        : public ElRefBase
    {
    public:
        /** Start time of the effect in milliseconds. */
        Int32 startTime;

        /** Duration of the effect in milliseconds. */
        Int32 duration;

        /**
         * Video effect type. See {@link VideoEffect VideoEffect} for valid
         * values.
         */
        Int32 videoEffectType;

        /**
         * Audio effect type. See {@link AudioEffect AudioEffect} for valid
         * values.
         */
        Int32 audioEffectType;

        /**
         * Start time of the effect in percents of the duration of the clip. A
         * value of 0 percent means start time is from the beginning of the
         * clip.
         */
        Int32 startPercent;

        /**
         * Duration of the effect in percents of the duration of the clip.
         */
        Int32 durationPercent;

        /**
         * Framing file.
         * <p>
         * This field is only used when the field <code>videoEffectType</code>
         * is set to {@link VideoEffect#FRAMING VideoEffect.FRAMING}. Otherwise
         * this field is ignored.
         */
        String framingFile;

        /**
         * Framing buffer.
         * <p>
         * This field is only used when the field <code>videoEffectType</code>
         * is set to {@link VideoEffect#FRAMING VideoEffect.FRAMING}. Otherwise
         * this field is ignored.
         */
        AutoPtr<ArrayOf<Int32> > framingBuffer;

        /**
         * Bitmap type Can be from RGB_565 (4), ARGB_4444 (5), ARGB_8888 (6);
         **/

        Int32 bitmapType;

        Int32 width;

        Int32 height;

        /**
         * Top left x coordinate. This coordinate is used to set the x
         * coordinate of the picture in the framing file when the framing file
         * is selected. The x coordinate is also used to set the location of the
         * text in the text effect.
         * <p>
         * This field is only used when the field <code>videoEffectType</code>
         * is set to {@link VideoEffect#FRAMING VideoEffect.FRAMING} or
         * {@link VideoEffect#TEXT VideoEffect.TEXT}. Otherwise this field is
         * ignored.
         */
        Int32 topLeftX;

        /**
         * Top left y coordinate. This coordinate is used to set the y
         * coordinate of the picture in the framing file when the framing file
         * is selected. The y coordinate is also used to set the location of the
         * text in the text effect.
         * <p>
         * This field is only used when the field <code>videoEffectType</code>
         * is set to {@link VideoEffect#FRAMING VideoEffect.FRAMING} or
         * {@link VideoEffect#TEXT VideoEffect.TEXT}. Otherwise this field is
         * ignored.
         */
        Int32 topLeftY;

        /**
         * Should the frame be resized or not. If this field is set to
         * <link>true</code> then the frame size is matched with the output
         * video size.
         * <p>
         * This field is only used when the field <code>videoEffectType</code>
         * is set to {@link VideoEffect#FRAMING VideoEffect.FRAMING}. Otherwise
         * this field is ignored.
         */
        Boolean framingResize;

        /**
         * Size to which the framing buffer needs to be resized to
         * This is valid only if framingResize is true
         */
        Int32 framingScaledSize;
        /**
         * Text to insert in the video.
         * <p>
         * This field is only used when the field <code>videoEffectType</code>
         * is set to {@link VideoEffect#TEXT VideoEffect.TEXT}. Otherwise this
         * field is ignored.
         */
        String text;

        /**
         * Text attributes for the text to insert in the video.
         * <p>
         * This field is only used when the field <code>videoEffectType</code>
         * is set to {@link VideoEffect#TEXT VideoEffect.TEXT}. Otherwise this
         * field is ignored. For more details about this field see the
         * integration guide.
         */
        String textRenderingData;

        /** Width of the text buffer in pixels. */
        Int32 textBufferWidth;

        /** Height of the text buffer in pixels. */
        Int32 textBufferHeight;

        /**
         * Processing rate for the fifties effect. A high value (e.g. 30)
         * results in high effect strength.
         * <p>
         * This field is only used when the field <code>videoEffectType</code>
         * is set to {@link VideoEffect#FIFTIES VideoEffect.FIFTIES}. Otherwise
         * this field is ignored.
         */
        Int32 fiftiesFrameRate;

        /**
         * RGB 16 color of the RGB16 and gradient color effect.
         * <p>
         * This field is only used when the field <code>videoEffectType</code>
         * is set to {@link VideoEffect#COLORRGB16 VideoEffect.COLORRGB16} or
         * {@link VideoEffect#GRADIENT VideoEffect.GRADIENT}. Otherwise this
         * field is ignored.
         */
        Int32 rgb16InputColor;

        /**
         * Start alpha blending percentage.
         * <p>
         * This field is only used when the field <code>videoEffectType</code>
         * is set to {@link VideoEffect#TEXT VideoEffect.TEXT} or
         * {@link VideoEffect#FRAMING VideoEffect.FRAMING}. Otherwise this field
         * is ignored.
         */
        Int32 alphaBlendingStartPercent;

        /**
         * Middle alpha blending percentage.
         * <p>
         * This field is only used when the field <code>videoEffectType</code>
         * is set to {@link VideoEffect#TEXT VideoEffect.TEXT} or
         * {@link VideoEffect#FRAMING VideoEffect.FRAMING}. Otherwise this field
         * is ignored.
         */
        Int32 alphaBlendingMiddlePercent;

        /**
         * End alpha blending percentage.
         * <p>
         * This field is only used when the field <code>videoEffectType</code>
         * is set to {@link VideoEffect#TEXT VideoEffect.TEXT} or
         * {@link VideoEffect#FRAMING VideoEffect.FRAMING}. Otherwise this field
         * is ignored.
         */
        Int32 alphaBlendingEndPercent;

        /**
         * Duration, in percentage of effect duration of the fade-in phase.
         * <p>
         * This field is only used when the field <code>videoEffectType</code>
         * is set to {@link VideoEffect#TEXT VideoEffect.TEXT} or
         * {@link VideoEffect#FRAMING VideoEffect.FRAMING}. Otherwise this field
         * is ignored.
         */
        Int32 alphaBlendingFadeInTimePercent;

        /**
         * Duration, in percentage of effect duration of the fade-out phase.
         * <p>
         * This field is only used when the field <code>videoEffectType</code>
         * is set to {@link VideoEffect#TEXT VideoEffect.TEXT} or
         * {@link VideoEffect#FRAMING VideoEffect.FRAMING}. Otherwise this field
         * is ignored.
         */
        Int32 alphaBlendingFadeOutTimePercent;
    };

    /** Defines the clip properties for preview */
    class PreviewClips
        : public ElRefBase
    {
    public:
        /**
         * The path to the clip file.
         * <p>
         * File format of the clip, it can be:
         * <ul>
         * <li>3GP file containing MPEG4/H263 video and AAC/AMR audio
         * <li>JPG file
         * </ul>
         */
        String clipPath;

        /**
         * File type of the clip. See {@link FileType FileType} for valid
         * values.
         */
        Int32 fileType;

        /** Begin of the cut in the clip in milliseconds. */
        Int64 beginPlayTime;

        Int64 endPlayTime;

        /**
         * Set The media rendering. See {@link MediaRendering MediaRendering}
         * for valid values.
         */
        Int32 mediaRendering;
    };

    /** Defines the audio settings. */
    class AudioSettings
        : public ElRefBase
    {
    public:
        String pFile;

        /** < PCM file path */
        String Id;

        Boolean bRemoveOriginal;

        /** < If true, the original audio track is not taken into account */
        Int32 channels;

        /** < Number of channels (1=mono, 2=stereo) of BGM clip */
        Int32 Fs;

        /**
         * < Sampling audio frequency (8000 for amr, 16000 or more for aac) of
         * BGM clip
         */
        Int32 ExtendedFs;

        /** < Extended frequency for AAC+, eAAC+ streams of BGM clip */
        Int64 startMs;

        /** < Time, in milliseconds, at which the added audio track is inserted */
        Int64 beginCutTime;

        Int64 endCutTime;

        Int32 fileType;

        Int32 volume;

        /** < Volume, in percentage, of the added audio track */
        Boolean loop;

        /** < Looping on/off > **/

        /** Audio mix and Duck **/
        Int32 ducking_threshold;

        Int32 ducking_lowVolume;

        Boolean bInDucking_enable;

        String pcmFilePath;
    };

    /** Encapsulates preview clips and effect settings */
    class PreviewSettings
        : public ElRefBase
    {
    public:

        AutoPtr<ArrayOf<PreviewClips *> > previewClipsArray;

        /** The effect settings. */
        AutoPtr<ArrayOf<EffectSettings *> > effectSettingsArray;

    };

    /**
     * Defines the media properties.
     **/
    class Properties
        : public ElRefBase
    {
    public:
        /**
         * Duration of the media in milliseconds.
         */
        Int32 duration;

        /**
         * File type.
         */
        Int32 fileType;

        /**
         * Video format.
         */
        Int32 videoFormat;

        /**
         * Duration of the video stream of the media in milliseconds.
         */
        Int32 videoDuration;

        /**
         * Bitrate of the video stream of the media.
         */
        Int32 videoBitrate;

        /**
         * Width of the video frames or the width of the still picture in
         * pixels.
         */
        Int32 width;

        /**
         * Height of the video frames or the height of the still picture in
         * pixels.
         */
        Int32 height;

        /**
         * Average frame rate of video in the media in frames per second.
         */
        Float averageFrameRate;

        /**
         * Profile of the video in the media.
         */
        Int32 profile;

        /**
         * Level of the video in the media.
         */
        Int32 level;

        /**
         * Is Video Profile supported.
         */
        Boolean profileSupported;

        /**
         * Is Video Level supported.
         */
        Boolean levelSupported;

        /**
         * Audio format.
         */
        Int32 audioFormat;

        /**
         * Duration of the audio stream of the media in milliseconds.
         */
        Int32 audioDuration;

        /**
         * Bitrate of the audio stream of the media.
         */
        Int32 audioBitrate;

        /**
         * Number of audio channels in the media.
         */
        Int32 audioChannels;

        /**
         * Sampling frequency of the audio stream in the media in samples per
         * second.
         */
        Int32 audioSamplingFrequency;

        /**
         * Volume value of the audio track as percentage.
         */
        Int32 audioVolumeValue;

        /**
         * Video rotation degree.
         */
        Int32 videoRotation;

        String Id;
    };

    /** Encapsulates clip properties */
    class PreviewClipProperties
        : public ElRefBase
    {
    public:
        AutoPtr<ArrayOf<Properties*> > clipProperties;
    };

    /** Defines the editing settings. */
    class EditSettings
        : public ElRefBase
    {
    public:
        /**
         * Array of clip settings. There is one <code>clipSetting</code> for
         * each clip.
         */
        AutoPtr<ArrayOf<ClipSettings*> > clipSettingsArray;

        /**
         * Array of transition settings. If there are n clips (and thus n
         * <code>clipSettings</code>) then there are (n-1) transitions and (n-1)
         * <code>transistionSettings</code> in
         * <code>transistionSettingsArray</code>.
         */
        AutoPtr<ArrayOf<TransitionSettings*> > transitionSettingsArray;

        /** The effect settings. */
        AutoPtr<ArrayOf<EffectSettings*> > effectSettingsArray;

        /**
         * Video frame rate of the output clip. See {@link VideoFrameRate
         * VideoFrameRate} for valid values.
         */
        Int32 videoFrameRate;

        /** Output file name. Must be an absolute path. */
        String outputFile;

        /**
         * Size of the video frames in the output clip. See
         * {@link VideoFrameSize VideoFrameSize} for valid values.
         */
        Int32 videoFrameSize;

        /**
         * Format of the video stream in the output clip. See
         * {@link VideoFormat VideoFormat} for valid values.
         */
        Int32 videoFormat;

        /**
         * Profile of the video stream in the output clip.
         */
        Int32 videoProfile;

        /**
         * Level of the video stream in the output clip.
         */
        Int32 videoLevel;

        /**
         * Format of the audio stream in the output clip. See
         * {@link AudioFormat AudioFormat} for valid values.
         */
        Int32 audioFormat;

        /**
         * Sampling frequency of the audio stream in the output clip. See
         * {@link AudioSamplingFrequency AudioSamplingFrequency} for valid
         * values.
         */
        Int32 audioSamplingFreq;

        /**
         * Maximum file size. By setting this you can set the maximum size of
         * the output clip. Set it to <code>0</code> to let the class ignore
         * this filed.
         */
        Int32 maxFileSize;

        /**
         * Number of audio channels in output clip. Use <code>0</code> for none,
         * <code>1</code> for mono or <code>2</code> for stereo. None is only
         * allowed when the <code>audioFormat</code> field is set to
         * {@link AudioFormat#NO_AUDIO AudioFormat.NO_AUDIO} or
         * {@link AudioFormat#NULL_AUDIO AudioFormat.NULL_AUDIO} Mono is only
         * allowed when the <code>audioFormat</code> field is set to
         * {@link AudioFormat#AAC AudioFormat.AAC}
         */
        Int32 audioChannels;

        /** Video bitrate. See {@link Bitrate Bitrate} for valid values. */
        Int32 videoBitrate;

        /** Audio bitrate. See {@link Bitrate Bitrate} for valid values. */
        Int32 audioBitrate;

        /**
         * Background music settings. See {@link BackgroundMusicSettings
         * BackgroundMusicSettings} for valid values.
         */
        AutoPtr<BackgroundMusicSettings> backgroundMusicSettings;

        Int32 primaryTrackVolume;

    };

private:
    class MediaItemGetThumbnailListCallback
        : public ElRefBase
        , public IMediaItemGetThumbnailListCallback
    {
    public:
        MediaItemGetThumbnailListCallback(
            /* [in] */ MediaArtistNativeHelper* host,
            /* [in] */ ArrayOf<IBitmap*>* bitmaps)
             : mHost(host)
             , mResult(bitmaps)
        {}

        CAR_INTERFACE_DECL()

        OnThumbnail(
            /* [in] */ IBitmap* bitmap,
            /* [in] */ Int32 index);

    private:
        MediaArtistNativeHelper* mHost;
        AutoPtr<ArrayOf<IBitmap*> > mResult;
    };

    class MediaArtistNativeHelperNativeGetPixelsListCallback
        : public ElRefBase
        , public IMediaArtistNativeHelperNativeGetPixelsListCallback
    {
    public:
        MediaArtistNativeHelperNativeGetPixelsListCallback(
            /* [in] */  MediaArtistNativeHelper* host,
            Int32 width,
            Int32 height,
            IInt32Buffer* decBuffer,
            ArrayOf<Int32>* array,
            Int32 thumbnailSize,
            Boolean needToMassage,
            IBitmap* tmpBitmap,
            Int32 decWidth,
            Int32 decHeight,
            Int32 videoRotation,
            IMediaItemGetThumbnailListCallback* cb)
             : mHost(host)
             , mWidth(width)
             , mHeight(height)
             , mDecBuffer(decBuffer)
             , mDecArray(array)
             , mThumbnailSize(thumbnailSize)
             , mNeedToMassage(needToMassage)
             , mBitmap(tmpBitmap)
             , mDecWidth(decWidth)
             , mDecHeight(decHeight)
             , mVideoRotation(videoRotation)
             , mCallback(cb)
        {}

        CAR_INTERFACE_DECL()

        CARAPI OnThumbnail(
            /* [in] */ Int32 index);

    private:
        MediaArtistNativeHelper* mHost;
        Int32 mWidth;
        Int32 mHeight;
        AutoPtr<IInt32Buffer> mDecBuffer;
        Boolean mNeedToMassage;
        AutoPtr<IBitmap> mBitmap;
        AutoPtr<IMediaItemGetThumbnailListCallback> mCallback;
        AutoPtr<ArrayOf<Int32> > mDecArray;
        Int32 mThumbnailSize;
        Int32 mVideoRotation;
        Int32 mDecHeight;
        Int32 mDecWidth;
    };

public:
    /**
     * Constructor
     *
     * @param projectPath The path where the VideoEditor stores all files
     *        related to the project
     * @param lock The semaphore
     * @param veObj The video editor reference
     */
    MediaArtistNativeHelper(
        /* [in] */ const String& projectPath,
        /* [in] */ ISemaphore* lock,
        /* [in] */ IVideoEditor* veObj);

    /**
     * @return The project path
     */
    CARAPI GetProjectPath(
        /* [out] */ String* result);

    /**
     * @return The Audio Track PCM file path
     */
    CARAPI GetProjectAudioTrackPCMFilePath(
        /* [out] */ String* result);

    /**
     * Invalidates the PCM file
     */
    CARAPI InvalidatePcmFile();

    CARAPI AdjustEffectsStartTimeAndDuration(
        /* [in] */ EffectSettings* lEffect,
        /* [in] */ Int32 beginCutTime,
        /* [in] */ Int32 endCutTime);

    CARAPI GenerateClip(
        /* [in] */ EditSettings* editSettings,
        /* [out] */ Int32* result);

    CARAPI ReleaseNativeHelper();

    CARAPI GetEffectSettings(
        /* [in] */ IEffectColor* effects,
        /* [out] */ EffectSettings** result);

    CARAPI GetOverlaySettings(
        /* [in] */ IOverlayFrame* overlay,
        /* [out] */ EffectSettings** result);

    CARAPI NativeHelperGetAspectRatio(
        /* [out] */ Int32* result);

    CARAPI SetAudioCodec(
        /* [in] */ Int32 codec);

    CARAPI SetVideoCodec(
        /* [in] */ Int32 codec);

    CARAPI SetAudioflag(
        /* [in] */ Boolean flag);

    CARAPI GetAudioflag(
        /* [out] */ Boolean* result);

    CARAPI GetClosestVideoFrameRate(
        /* [in] */ Int32 averageFrameRate,
        /* [out] */ Int32* result);

    CARAPI InitClipSettings(
        /* [in] */ ClipSettings* lclipSettings);

    CARAPI GenerateEffectClip(
        /* [in] */ IMediaItem* lMediaItem,
        /* [in] */ ClipSettings* lclipSettings,
        /* [in] */ EditSettings* e,
        /* [in] */ const String& uniqueId,
        /* [in] */ Int32 clipNo,
        /* [out] */ String* result);

    CARAPI GenerateKenBurnsClip(
        /* [in] */ EditSettings* e,
        /* [in] */ IMediaImageItem* m,
        /* [out] */ String* result);

    CARAPI GenerateTransitionClip(
        /* [in] */ EditSettings* e,
        /* [in] */ const String& uniqueId,
        /* [in] */ IMediaItem* m1,
        /* [in] */ IMediaItem* m2,
        /* [in] */ ITransition* t,
        /* [out] */ String* result);

    CARAPI PreviewStoryBoard(
        /* [in] */ ArrayOf<IMediaItem*>* mediaItemsList,
        /* [in] */ ArrayOf<ITransition*>* mediaTransitionList,
        /* [in] */ ArrayOf<IAudioTrack2*>* mediaBGMList,
        /* [in] */ IVideoEditorMediaProcessingProgressListener* listener);

    CARAPI DoPreview(
        /* [in] */ ISurface* surface,
        /* [in] */ Int64 fromMs,
        /* [in] */ Int64 toMs,
        /* [in] */ Boolean loop,
        /* [in] */ Int32 callbackAfterFrameCount,
        /* [in] */ IVideoEditorPreviewProgressListener* listener);

    CARAPI StopPreview(
        /* [out] */ Int64* result);

    CARAPI RenderPreviewFrame(
        /* [in] */ ISurface* surface,
        /* [in] */ Int64 time,
        /* [in] */ Int32 surfaceWidth,
        /* [in] */ Int32 surfaceHeight,
        /* [in] */ IVideoEditorOverlayData* overlayData,
        /* [out] */ Int64* result);

    CARAPI RenderMediaItemPreviewFrame(
        /* [in] */ ISurface* surface,
        /* [in] */ const String& filepath,
        /* [in] */ Int64 time,
        /* [in] */ Int32 framewidth,
        /* [in] */ Int32 frameheight,
        /* [out] */ Int64* result);

    CARAPI SetGeneratePreview(
        /* [in] */ Boolean isRequired);

    CARAPI GetGeneratePreview(
        /* [out] */ Boolean* result);

    CARAPI GetAspectRatio(
        /* [in] */ Int32 w,
        /* [in] */ Int32 h,
        /* [out] */ Int32* result);

    CARAPI GetFileType(
        /* [in] */ Int32 fileType,
        /* [out] */ Int32* result);

    CARAPI GetVideoCodecType(
        /* [in] */ Int32 codecType,
        /* [out] */ Int32* result);

    CARAPI GetAudioCodecType(
        /* [in] */ Int32 codecType,
        /* [out] */ Int32* result);

    CARAPI GetFrameRate(
        /* [in] */ Int32 fps,
        /* [out] */ Int32* result);

    CARAPI GetMediaItemFileType(
        /* [in] */ Int32 fileType,
        /* [out] */ Int32* result);

    CARAPI GetMediaItemRenderingMode(
        /* [in] */ Int32 renderingMode,
        /* [out] */ Int32* result);

    CARAPI GetVideoTransitionBehaviour(
        /* [in] */ Int32 transitionType,
        /* [out] */ Int32* result);

    CARAPI GetSlideSettingsDirection(
        /* [in] */ Int32 slideDirection,
        /* [out] */ Int32* result);

    CARAPI Export(
        /* [in] */ const String& filePath,
        /* [in] */ const String& projectDir,
        /* [in] */ Int32 height,
        /* [in] */ Int32 bitrate,
        /* [in] */ ArrayOf<IMediaItem*>* mediaItemsList,
        /* [in] */ ArrayOf<ITransition*>* mediaTransitionList,
        /* [in] */ ArrayOf<IAudioTrack2*>* mediaBGMList,
        /* [in] */ IVideoEditorExportProgressListener* listener);

    CARAPI Stop(
        /* [in] */ const String& filename);

    CARAPI GetPixels(
        /* [in] */ const String& filename,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int64 timeMs,
        /* [in] */ Int32 videoRotation,
        /* [out] */ IBitmap** bitmap);

    CARAPI GetPixelsList(
        /* [in] */ const String& filename,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int64 startMs,
        /* [in] */ Int64 endMs,
        /* [in] */ Int32 thumbnailCount,
        /* [in] */ ArrayOf<Int32>* indices,
        /* [in] */ IMediaItemGetThumbnailListCallback* callback,
        /* [in] */ Int32 videoRotation);

    CARAPI GenerateAudioGraph(
        /* [in] */ const String& uniqueId,
        /* [in] */ const String& inFileName,
        /* [in] */ const String& OutAudiGraphFileName,
        /* [in] */ Int32 frameDuration,
        /* [in] */ Int32 audioChannels,
        /* [in] */ Int32 samplesCount,
        /* [in] */ IExtractAudioWaveformProgressListener* listener,
        /* [in] */ Boolean isVideo);

    CARAPI ClearPreviewSurface(
        /* [in] */ ISurface* surface);

    /**     Native Methods        */
    CARAPI_(AutoPtr<Properties>) GetMediaProperties(
        /* [in] */ const String& file);

private:
    CARAPI_(void) OnProgressUpdate(
        /* [in] */ Int32 taskId,
        /* [in] */ Int32 progress);

    CARAPI_(void) OnPreviewProgressUpdate(
        /* [in] */ Int32 progress,
        /* [in] */ Boolean isFinished,
        /* [in] */ Boolean updateOverlay,
        /* [in] */ const String& filename,
        /* [in] */ Int32 renderingMode,
        /* [in] */ Int32 error);

    CARAPI_(void) OnAudioGraphExtractProgressUpdate(
        /* [in] */ Int32 progress,
        /* [in] */ Boolean isVideo);

    CARAPI_(Int32) GetTransitionResolution(
        /* [in] */ IMediaItem* m1,
        /* [in] */ IMediaItem* m2);

    CARAPI_(Int32) PopulateEffects(
        /* [in] */ IMediaItem* m,
        /* [in] */ ArrayOf<EffectSettings*>* effectSettings,
        /* [in] */ Int32 i,
        /* [in] */ Int32 beginCutTime,
        /* [in] */ Int32 endCutTime,
        /* [in] */ Int32 storyBoardTime);

    CARAPI_(void) AdjustMediaItemBoundary(
        /* [in] */ ClipSettings* clipSettings,
        /* [in] */ Properties* clipProperties,
        /* [in] */ IMediaItem* m);

    CARAPI_(void) GenerateTransition(
        /* [in] */ ITransition* transition,
        /* [in] */ EditSettings* editSettings,
        /* [in] */ PreviewClipProperties* clipPropertiesArray,
        /* [in] */ Int32 index);

    CARAPI_(void) AdjustVolume(
        /* [in] */ IMediaItem* m,
        /* [in] */ PreviewClipProperties* clipProperties,
        /* [in] */ Int32 index);

    CARAPI_(void) CheckOddSizeImage(
        /* [in] */ IMediaItem* m,
        /* [in] */ PreviewClipProperties* clipProperties,
        /* [in] */ Int32 index);

    CARAPI_(Int32) PopulateMediaItemProperties(
        /* [in] */ IMediaItem* m,
        /* [in] */ Int32 index,
        /* [in] */ Int32 maxHeight);

    CARAPI_(void) PopulateBackgroundMusicProperties(
        /* [in] */ ArrayOf<IAudioTrack2*>* mediaBGMList);

    CARAPI_(Int32) GetTotalEffects(
        /* [in] */ ArrayOf<IMediaItem*>* mediaItemsList);

    CARAPI_(void) PreviewFrameEditInfo(
        /* [in] */ const String& filename,
        /* [in] */ Int32 renderingMode);

    CARAPI_(Int32) GetEffectColorType(
        /* [in] */ IEffectColor* effect);

    CARAPI_(Int32) FindVideoResolution(
        /* [in] */ Int32 aspectRatio,
        /* [in] */ Int32 height);

    CARAPI_(Int32) FindVideoBitrate(
        /* [in] */ Int32 videoFrameSize);

    CARAPI_(void) Lock();

    CARAPI_(void) Unlock();

    /**     Native Methods        */
    static CARAPI_(AutoPtr<Version>) GetVersion();

    CARAPI_(Int32) NativeGetPixels(
        /* [in] */ const String& fileName,
        /* [in] */ ArrayOf<Int32>* pixelArray,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int64 timeMS);

    CARAPI_(Int32) NativeGetPixelsList(
        /* [in] */ const String& fileName,
        /* [in] */ ArrayOf<Int32>* pixelArray,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int32 nosofTN,
        /* [in] */ Int64 startTimeMs,
        /* [in] */ Int64 endTimeMs,
        /* [in] */ ArrayOf<Int32>* indices,
        /* [in] */ IMediaArtistNativeHelperNativeGetPixelsListCallback* callback);

    CARAPI_(void) ReleaseResources();

    CARAPI_(void) NativeClearSurface(
        /* [in] */ ISurface* surface);

    CARAPI_(void) StopEncoding();

    CARAPI_(void) _init(
        /* [in] */ const String& tempPath,
        /* [in] */ const String& libraryPath);

    CARAPI_(void) NativeStartPreview(
        /* [in] */ ISurface* mSurface,
        /* [in] */ Int64 fromMs,
        /* [in] */ Int64 toMs,
        /* [in] */ Int32 callbackAfterFrameCount,
        /* [in] */ Boolean loop);

    CARAPI_(void) NativePopulateSettings(
        /* [in] */ EditSettings* editSettings,
        /* [in] */ PreviewClipProperties* mProperties,
        /* [in] */ AudioSettings* mAudioSettings);

    CARAPI_(Int32) NativeRenderPreviewFrame(
        /* [in] */ ISurface* mSurface,
        /* [in] */ Int64 timeMs,
        /* [in] */ Int32 surfaceWidth,
        /* [in] */ Int32 surfaceHeight);

    CARAPI_(Int32) NativeRenderMediaItemPreviewFrame(
        /* [in] */ ISurface* mSurface,
        /* [in] */ const String& filepath,
        /* [in] */ Int32 framewidth,
        /* [in] */ Int32 frameheight,
        /* [in] */ Int32 surfacewidth,
        /* [in] */ Int32 surfaceheight,
        /* [in] */ Int64 timeMs);

    CARAPI_(Int32) NativeStopPreview();

    CARAPI_(Int32) NativeGenerateAudioGraph(
        /* [in] */ const String& pcmFilePath,
        /* [in] */ const String& outGraphPath,
        /* [in] */ Int32 frameDuration,
        /* [in] */ Int32 channels,
        /* [in] */ Int32 sampleCount);

    CARAPI_(Int32) NativeGenerateRawAudio(
        /* [in] */ const String& InFileName,
        /* [in] */ const String& PCMFileName);

    CARAPI_(Int32) NativeGenerateClip(
        /* [in] */ EditSettings* editSettings);

public:
    static const Int32 TASK_LOADING_SETTINGS;
    static const Int32 TASK_ENCODING;

    // Processing indication
    static const Int32 PROCESSING_NONE;
    static const Int32 PROCESSING_AUDIO_PCM;
    static const Int32 PROCESSING_TRANSITION;
    static const Int32 PROCESSING_KENBURNS;
    static const Int32 PROCESSING_INTERMEDIATE1;
    static const Int32 PROCESSING_INTERMEDIATE2;
    static const Int32 PROCESSING_INTERMEDIATE3;
    static const Int32 PROCESSING_EXPORT;

private:
    static const String TAG;
    static const Int32 MAX_THUMBNAIL_PERMITTED;

    /**
     *  The resize paint
     */
    static AutoPtr<IPaint> sResizePaint;

    static const String AUDIO_TRACK_PCM_FILE;

    AutoPtr<IVideoEditor> mVideoEditor;
    /*
     *  Semaphore to control preview calls
     */
    AutoPtr<ISemaphore> mLock;

    AutoPtr<EditSettings> mStoryBoardSettings;

    String mOutputFilename;

    AutoPtr<PreviewClipProperties> mClipProperties;

    AutoPtr<EditSettings> mPreviewEditSettings;

    AutoPtr<AudioSettings> mAudioSettings;

    AutoPtr<IAudioTrack2> mAudioTrack;

    Boolean mInvalidatePreviewArray;

    Boolean mRegenerateAudio;

    String mExportFilename;
    Int32 mExportVideoCodec;
    Int32 mExportAudioCodec;
    Int32 mProgressToApp;

    String mRenderPreviewOverlayFile;
    Int32 mRenderPreviewRenderingMode;

    Boolean mIsFirstProgress;

    Int32 mProcessingState;
    AutoPtr<IInterface> mProcessingObject;
    AutoPtr<IVideoEditorPreviewProgressListener> mPreviewProgressListener;
    AutoPtr<IVideoEditorExportProgressListener> mExportProgressListener;
    AutoPtr<IExtractAudioWaveformProgressListener> mExtractAudioWaveformProgressListener;
    AutoPtr<IVideoEditorMediaProcessingProgressListener> mMediaProcessingProgressListener;
    String mProjectPath;

    Int64 mPreviewProgress;

    String mAudioTrackPCMFilePath;

    Int32 mTotalClips;

    Boolean mErrorFlagSet;

    // @SuppressWarnings("unused")
    Int32 mManualEditContext;

};

} // namespace Videoeditor
} // namespace Media
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_MEDIA_VIDEOEDITOR_MEDIAARTISTNATIVEHELPER_H__

