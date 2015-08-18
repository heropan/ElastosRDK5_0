
#ifndef __ELASTOS_DROID_MEDIA_VIDEOEDITOR_CVIDEOEDITORAUDIOTRACK_H__
#define __ELASTOS_DROID_MEDIA_VIDEOEDITOR_CVIDEOEDITORAUDIOTRACK_H__

#include "_Elastos_Droid_Media_Videoeditor_CAudioTrack2.h"
#include <ext/frameworkext.h>
#include "MediaArtistNativeHelper.h"

using Elastos::Droid::Media::Videoeditor::MediaArtistNativeHelper;

namespace Elastos {
namespace Droid {
namespace Media {
namespace Videoeditor {
/**
 * This class allows to handle an audio track. This audio file is mixed with the
 * audio samples of the media items.
 * {@hide}
 */
CarClass(CAudioTrack2)
{
public:
    CAudioTrack2();

    /**
     * Constructor
     *
     * @param editor The video editor reference
     * @param audioTrackId The audio track id
     * @param filename The absolute file name
     *
     * @throws IOException if file is not found
     * @throws IllegalArgumentException if file format is not supported or if
     *         the codec is not supported or if editor is not of type
     *         VideoEditorImpl.
     */
    CARAPI constructor(
        /* [in] */ IVideoEditor* editor,
        /* [in] */ const String& audioTrackId,
        /* [in] */ const String& filename);

    /**
     * Constructor
     *
     * @param editor The video editor reference
     * @param audioTrackId The audio track id
     * @param filename The audio filename. In case file contains Audio and Video,
     *         only the Audio stream will be used as Audio Track.
     * @param startTimeMs the start time in milliseconds (relative to the
     *         timeline)
     * @param beginMs start time in the audio track in milliseconds (relative to
     *         the beginning of the audio track)
     * @param endMs end time in the audio track in milliseconds (relative to the
     *         beginning of the audio track)
     * @param loop true to loop the audio track
     * @param volume The volume in percentage
     * @param muted true if the audio track is muted
     * @param threshold Ducking will be activated when the relative energy in
     *         the media items audio signal goes above this value. The valid
     *         range of values is 0 to 90.
     * @param duckedTrackVolume The relative volume of the audio track when
     *         ducking is active. The valid range of values is 0 to 100.
     * @param audioWaveformFilename The name of the waveform file
     *
     * @throws IOException if file is not found
     * @throws IllegalArgumentException if file format is not supported or if
     *             the codec is not supported or if editor is not of type
     *             VideoEditorImpl.
     */
    CARAPI constructor(
        /* [in] */ IVideoEditor* editor,
        /* [in] */ const String& audioTrackId,
        /* [in] */ const String& filename,
        /* [in] */ Int64 startTimeMs,
        /* [in] */ Int64 beginMs,
        /* [in] */ Int64 endMs,
        /* [in] */ Boolean loop,
        /* [in] */ Int32 volume,
        /* [in] */ Boolean muted,
        /* [in] */ Boolean duckingEnabled,
        /* [in] */ Int32 duckThreshold,
        /* [in] */ Int32 duckedTrackVolume,
        /* [in] */ const String& audioWaveformFilename);

    /**
     * Get the id of the audio track
     *
     * @return The id of the audio track
     */
    CARAPI GetId(
        /* [out] */ String* result);

    /**
     * Get the filename for this audio track source.
     *
     * @return The filename as an absolute file name
     */
    CARAPI GetFilename(
        /* [out] */ String* result);

    /**
     * Get the number of audio channels in the source of this audio track
     *
     * @return The number of audio channels in the source of this audio track
     */
    CARAPI GetAudioChannels(
        /* [out] */ Int32* result);

    /**
     * Get the audio codec of the source of this audio track
     *
     * @return The audio codec of the source of this audio track
     * {@link android.media.videoeditor.MediaProperties}
     */
    CARAPI GetAudioType(
        /* [out] */ Int32* result);


    /**
     * Get the audio sample frequency of the audio track
     *
     * @return The audio sample frequency of the audio track
     */
    CARAPI GetAudioSamplingFrequency(
        /* [out] */ Int32* result);

    /**
     * Get the audio bitrate of the audio track
     *
     * @return The audio bitrate of the audio track
     */
    CARAPI GetAudioBitrate(
        /* [out] */ Int32* result);

    /**
     * Set the volume of this audio track as percentage of the volume in the
     * original audio source file.
     *
     * @param volumePercent Percentage of the volume to apply. If it is set to
     *         0, then volume becomes mute. It it is set to 100, then volume
     *         is same as original volume. It it is set to 200, then volume
     *         is doubled (provided that volume amplification is supported)
     *
     * @throws UnsupportedOperationException if volume amplification is
     *         requested and is not supported.
     */
    CARAPI SetVolume(
        /* [in] */ Int32 volumePercent);

    /**
     * Get the volume of the audio track as percentage of the volume in the
     * original audio source file.
     *
     * @return The volume in percentage
     */
    CARAPI GetVolume(
        /* [out] */ Int32* result);

    /**
     * Mute/Unmute the audio track
     *
     * @param muted true to mute the audio track. SetMute(true) will make
     *         the volume of this Audio Track to 0.
     */
    CARAPI SetMute(
        /* [in] */ Boolean muted);


    /**
     * Check if the audio track is muted
     *
     * @return true if the audio track is muted
     */
    CARAPI IsMuted(
        /* [out] */ Boolean* result);

    /**
     * Get the start time of this audio track relative to the storyboard
     * timeline.
     *
     * @return The start time in milliseconds
     */
    CARAPI GetStartTime(
        /* [out] */ Int64* result);

    /**
     * Get the audio track duration
     *
     * @return The duration in milliseconds. This value represents actual audio
     *         track duration. This value is not effected by 'enableLoop' or
     *         'setExtractBoundaries'.
     */
    CARAPI GetDuration(
        /* [out] */ Int64* result);

    /**
     * Get the audio track timeline duration
     *
     * @return The timeline duration as defined by the begin and end boundaries
     */
    CARAPI GetTimelineDuration(
        /* [out] */ Int64* result);

    /**
     * Sets the start and end marks for trimming an audio track
     *
     * @param beginMs start time in the audio track in milliseconds (relative to
     *         the beginning of the audio track)
     * @param endMs end time in the audio track in milliseconds (relative to the
     *         beginning of the audio track)
     */
    CARAPI SetExtractBoundaries(
        /* [in] */ Int64 beginMs,
        /* [in] */ Int64 endMs);

    /**
     * Get the boundary begin time
     *
     * @return The boundary begin time
     */
    CARAPI GetBoundaryBeginTime(
        /* [out] */ Int64* result);

    /**
     * Get the boundary end time
     *
     * @return The boundary end time
     */
    CARAPI GetBoundaryEndTime(
        /* [out] */ Int64* result);

    /**
     * Enable the loop mode for this audio track. Note that only one of the
     * audio tracks in the timeline can have the loop mode enabled. When looping
     * is enabled the samples between mBeginBoundaryTimeMs and
     * mEndBoundaryTimeMs are looped.
     */
    CARAPI EnableLoop();

    /**
     * Disable the loop mode
     */
    CARAPI DisableLoop();

    /**
     * Check if looping is enabled
     *
     * @return true if looping is enabled
     */
    CARAPI IsLooping(
        /* [out] */ Boolean* result);

    /**
     * Disable the audio duck effect
     */
    CARAPI DisableDucking();

    /**
     * Enable ducking by specifying the required parameters
     *
     * @param threshold Ducking will be activated when the energy in
     *         the media items audio signal goes above this value. The valid
     *         range of values is 0db to 90dB. 0dB is equivalent to disabling
     *         ducking.
     * @param duckedTrackVolume The relative volume of the audio track when ducking
     *         is active. The valid range of values is 0 to 100.
     */
    CARAPI EnableDucking(
        /* [in] */ Int32 threshold,
        /* [in] */ Int32 duckedTrackVolume);

    /**
     * Check if ducking is enabled
     *
     * @return true if ducking is enabled
     */
    CARAPI IsDuckingEnabled(
        /* [out] */ Boolean* result);

    /**
     * Get the ducking threshold.
     *
     * @return The ducking threshold
     */
    CARAPI GetDuckingThreshhold(
        /* [out] */ Int32* result);

    /**
     * Get the ducked track volume.
     *
     * @return The ducked track volume
     */
    CARAPI GetDuckedTrackVolume(
        /* [out] */ Int32* result);

    /**
     * This API allows to generate a file containing the sample volume levels of
     * this audio track object. This function may take significant time and is
     * blocking. The filename can be retrieved using getAudioWaveformFilename().
     *
     * @param listener The progress listener
     *
     * @throws IOException if the output file cannot be created
     * @throws IllegalArgumentException if the audio file does not have a valid
     *         audio track
     * @throws IllegalStateException if the codec type is unsupported
     */
    CARAPI ExtractAudioWaveform(
        /* [in] */ IExtractAudioWaveformProgressListener* listener);

    /**
     * Get the audio waveform file name if extractAudioWaveform was successful.
     *
     * @return the name of the file, null if the file does not exist
     */
    CARAPI GetAudioWaveformFilename(
        /* [out] */ String* result);

    /**
     * Delete the waveform file
     */
    CARAPI Invalidate();

    /**
     * Get the audio waveform data.
     *
     * @return The waveform data
     *
     * @throws IOException if the waveform file cannot be found
     */
    CARAPI GetWaveformData(
        /* [out] */ IWaveformData** result);

    CARAPI Equals(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* result);

    CARAPI GetHashCode(
        /* [out] */ Int32* result);

protected:
    CARAPI Init(
        /* [in] */ IVideoEditor* editor,
        /* [in] */ const String& audioTrackId,
        /* [in] */ const String& filename,
        /* [in] */ Int64 startTimeMs,
        /* [in] */ Int64 beginMs,
        /* [in] */ Int64 endMs,
        /* [in] */ Boolean loop,
        /* [in] */ Int32 volume,
        /* [in] */ Boolean muted,
        /* [in] */ Boolean duckingEnabled,
        /* [in] */ Int32 duckThreshold,
        /* [in] */ Int32 duckedTrackVolume,
        /* [in] */ const String& audioWaveformFilename);

private:
    /**
     *  Instance variables
     *  Private object for calling native methods via MediaArtistNativeHelper
     */
    AutoPtr<MediaArtistNativeHelper> mMANativeHelper;
    String mUniqueId;
    String mFilename;
    Int64 mStartTimeMs;
    Int64 mTimelineDurationMs;
    Int32 mVolumePercent;
    Int64 mBeginBoundaryTimeMs;
    Int64 mEndBoundaryTimeMs;
    Boolean mLoop;
    Boolean mMuted;
    Int64 mDurationMs;
    Int32 mAudioChannels;
    Int32 mAudioType;
    Int32 mAudioBitrate;
    Int32 mAudioSamplingFrequency;
    /**
     *  Ducking variables
     */
    Int32 mDuckingThreshold;
    Int32 mDuckedTrackVolume;
    Boolean mIsDuckingEnabled;

    /**
     *  The audio waveform filename
     */
    String mAudioWaveformFilename;

    /**
     *  The audio waveform data
     */
    AutoPtr<IWaveformData> mWaveformData;
};

} // namespace Videoeditor
} // namespace Media
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_MEDIA_VIDEOEDITOR_CVIDEOEDITORAUDIOTRACK_H__

