
#ifndef __ELASTOS_DROID_MEDIA_VIDEOEDITOR_CMEDIAVIDEOITEM_H__
#define __ELASTOS_DROID_MEDIA_VIDEOEDITOR_CMEDIAVIDEOITEM_H__

#include "_Elastos_Droid_Media_Videoeditor_CMediaVideoItem.h"
#include <ext/frameworkext.h>
#include "media/media/videoeditor/MediaItem.h"

#include "MediaArtistNativeHelper.h"

using Elastos::Droid::View::ISurfaceHolder;

namespace Elastos {
namespace Droid {
namespace Media {
namespace Videoeditor {
/**
 * This class represents a video clip item on the storyboard
 * {@hide}
 */
CarClass(CMediaVideoItem), public MediaItem
{
public:
    IMEDIAITEM_METHODS_DECL()

    CMediaVideoItem();

    CMediaVideoItem(
        /* [in] */ IVideoEditor* editor,
        /* [in] */ const String& mediaItemId,
        /* [in] */ const String& filename,
        /* [in] */ Int32 renderingMode);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    /**
     * Constructor
     *
     * @param editor The video editor reference
     * @param mediaItemId The MediaItem id
     * @param filename The image file name
     * @param renderingMode The rendering mode
     *
     * @throws IOException if the file cannot be opened for reading
     */
    CARAPI constructor(
        /* [in] */ IVideoEditor* editor,
        /* [in] */ const String& mediaItemId,
        /* [in] */ const String& filename,
        /* [in] */ Int32 renderingMode);

    /**
     * Constructor
     *
     * @param editor The video editor reference
     * @param mediaItemId The MediaItem id
     * @param filename The image file name
     * @param renderingMode The rendering mode
     * @param beginMs Start time in milliseconds. Set to 0 to extract from the
     *           beginning
     * @param endMs End time in milliseconds. Set to {@link #END_OF_FILE} to
     *           extract until the end
     * @param volumePercent in %/. 100% means no change; 50% means half value, 200%
     *            means double, 0% means silent.
     * @param muted true if the audio is muted
     * @param audioWaveformFilename The name of the audio waveform file
     *
     * @throws IOException if the file cannot be opened for reading
     */
    CARAPI constructor(
        /* [in] */ IVideoEditor* editor,
        /* [in] */ const String& mediaItemId,
        /* [in] */ const String& filename,
        /* [in] */ Int32 renderingMode,
        /* [in] */ Int64 beginMs,
        /* [in] */ Int64 endMs,
        /* [in] */ Int32 volumePercent,
        /* [in] */ Boolean muted,
        /* [in] */ const String& audioWaveformFilename);

    /**
     * Sets the start and end marks for trimming a video media item.
     * This method will adjust the duration of bounding transitions, effects
     * and overlays if the current duration of the transactions become greater
     * than the maximum allowable duration.
     *
     * @param beginMs Start time in milliseconds. Set to 0 to extract from the
     *           beginning
     * @param endMs End time in milliseconds. Set to {@link #END_OF_FILE} to
     *           extract until the end
     *
     * @throws IllegalArgumentException if the start time is greater or equal than
     *           end time, the end time is beyond the file duration, the start time
     *           is negative
     */
    CARAPI SetExtractBoundaries(
        /* [in] */ Int64 beginMs,
        /* [in] */ Int64 endMs);

    CARAPI GetBoundaryBeginTime(
        /* [out] */ Int64* result);

    CARAPI GetBoundaryEndTime(
        /* [out] */ Int64* result);

    CARAPI AddEffect(
        /* [in] */ IEffect* effect);

    CARAPI GetThumbnail(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int64 timeMs,
        /* [out] */ IBitmap** result);

    CARAPI GetThumbnailList(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int64 startMs,
        /* [in] */ Int64 endMs,
        /* [in] */ Int32 thumbnailCount,
        /* [in] */ ArrayOf<Int32>* indices,
        /* [in] */ IMediaItemGetThumbnailListCallback* listcallback);

    CARAPI InvalidateTransitions(
        /* [in ]*/ Int64 startTimeMs,
        /* [in ]*/ Int64 durationMs);

    CARAPI InvalidateTransitions(
        /* [in ]*/ Int64 oldStartTimeMs,
        /* [in ]*/ Int64 oldDurationMs,
        /* [in ]*/ Int64 newStartTimeMs,
        /* [in ]*/ Int64 newDurationMs);

    CARAPI GetAspectRatio(
        /* [out] */ Int32* result);

    CARAPI GetFileType(
        /* [out] */ Int32* result);

    CARAPI GetWidth(
        /* [out] */ Int32* result);

    CARAPI GetHeight(
        /* [out] */ Int32* result);

    CARAPI GetDuration(
        /* [out] */ Int64* result);

    CARAPI GetTimelineDuration(
        /* [out] */ Int64* result);

    /**
     * Render a frame according to the playback (in the native aspect ratio) for
     * the specified media item. All effects and overlays applied to the media
     * item are ignored. The extract boundaries are also ignored. This method
     * can be used to playback frames when implementing trimming functionality.
     *
     * @param surfaceHolder SurfaceHolder used by the application
     * @param timeMs time corresponding to the frame to display (relative to the
     *            the beginning of the media item).
     * @return The accurate time stamp of the frame that is rendered .
     * @throws IllegalStateException if a playback, preview or an export is
     *             already in progress
     * @throws IllegalArgumentException if time is negative or greater than the
     *             media item duration
     */
    CARAPI RenderFrame(
        /* [in] */ ISurfaceHolder* surfaceHolder,
        /* [in] */ Int64 timeMs,
        /* [out] */ Int64* result);

    /**
     * This API allows to generate a file containing the sample volume levels of
     * the Audio track of this media item. This function may take significant
     * time and is blocking. The file can be retrieved using
     * getAudioWaveformFilename().
     *
     * @param listener The progress listener
     *
     * @throws IOException if the output file cannot be created
     * @throws IllegalArgumentException if the mediaItem does not have a valid
     *             Audio track
     */
    CARAPI ExtractAudioWaveform(
        /* [in] */ IExtractAudioWaveformProgressListener* listener);

    /**
     * Get the audio waveform file name if {@link #extractAudioWaveform()} was
     * successful. The file format is as following:
     * <ul>
     *  <li>first 4 bytes provide the number of samples for each value, as big-endian signed</li>
     *  <li>4 following bytes is the total number of values in the file, as big-endian signed</li>
     *  <li>all values follow as bytes Name is unique.</li>
     *</ul>
     * @return the name of the file, null if the file has not been computed or
     *         if there is no Audio track in the mediaItem
     */
    CARAPI GetAudioWaveformFilename(
        /* [out] */ String* result);

    /**
     * Invalidate the AudioWaveform File
     */
    CARAPI Invalidate();

    /**
     * @return The waveform data
     */
    CARAPI GetWaveformData(
        /* [out] */ IWaveformData** result);

    /**
     * Set volume of the Audio track of this mediaItem
     *
     * @param volumePercent in %/. 100% means no change; 50% means half value, 200%
     *            means double, 0% means silent.
     * @throws UsupportedOperationException if volume value is not supported
     */
    CARAPI SetVolume(
        /* [in] */ Int32 volumePercent);

    /**
     * Get the volume value of the audio track as percentage. Call of this
     * method before calling setVolume will always return 100%
     *
     * @return the volume in percentage
     */
    CARAPI GetVolume(
        /* [out] */ Int32* result);

    /**
     * @param muted true to mute the media item
     */
    CARAPI SetMute(
        /* [in] */ Boolean muted);


    /**
     * @return true if the media item is muted
     */
    CARAPI IsMuted(
        /* [out] */ Boolean* result);

    /**
     * @return The video type
     */
    CARAPI GetVideoType(
        /* [out] */ Int32* result);

    /**
     * @return The video profile
     */
    CARAPI GetVideoProfile(
        /* [out] */ Int32* result);

    /**
     * @return The video profile
     */
    CARAPI GetVideoLevel(
        /* [out] */ Int32* result);

    /**
     * @return The video bitrate
     */
    CARAPI GetVideoBitrate(
        /* [out] */ Int32* result);

    /**
     * @return The audio bitrate
     */
    CARAPI GetAudioBitrate(
        /* [out] */ Int32* result);

    /**
     * @return The number of frames per second
     */
    CARAPI GetFps(
        /* [out] */ Int32* result);

    /**
     * @return The audio codec
     */
    CARAPI GetAudioType(
        /* [out] */ Int32* result);

    /**
     * @return The number of audio channels
     */
    CARAPI GetAudioChannels(
        /* [out] */ Int32* result);

    /**
     * @return The audio sample frequency
     */
    CARAPI GetAudioSamplingFrequency(
        /* [out] */ Int32* result);

    /**
     * @return The Video media item properties in ClipSettings class object
     * {@link android.media.videoeditor.MediaArtistNativeHelper.ClipSettings}
     */
    CARAPI GetVideoClipProperties(
        /* [out] */ MediaArtistNativeHelper::ClipSettings** settings);

    CARAPI SetGeneratedImageClip(
        /* [in] */ const String& generatedFilePath);

    CARAPI GetGeneratedImageClip(
        /* [out] */ String* result);

private:
    /**
     *  Instance variables
     */
    Int32 mWidth;
    Int32 mHeight;
    Int32 mAspectRatio;
    Int32 mFileType;
    Int32 mVideoType;
    Int32 mVideoProfile;
    Int32 mVideoLevel;
    Int32 mVideoBitrate;
    Int64 mDurationMs;
    Int32 mAudioBitrate;
    Int32 mFps;
    Int32 mAudioType;
    Int32 mAudioChannels;
    Int32 mAudioSamplingFrequency;
    Int64 mBeginBoundaryTimeMs;
    Int64 mEndBoundaryTimeMs;
    Int32 mVolumePercentage;
    Boolean mMuted;
    String mAudioWaveformFilename;
    AutoPtr<MediaArtistNativeHelper> mMANativeHelper;
    AutoPtr<IVideoEditorImpl> mVideoEditor;
    Int32 mVideoRotationDegree;
    /**
     *  The audio waveform data
     */
    AutoPtr<IWaveformData> mWaveformData;
};

} // namespace videoeditor
} // namespace Media
} // namepsace Droid
} // namespace Elastos
#endif // __ELASTOS_DROID_MEDIA_VIDEOEDITOR_CMEDIAVIDEOITEM_H__

