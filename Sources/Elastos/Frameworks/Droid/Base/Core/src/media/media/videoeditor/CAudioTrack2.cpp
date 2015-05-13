
#include "ext/frameworkext.h"
#include "CAudioTrack2.h"
#include "MediaArtistNativeHelper.h"
#include "CWaveformData.h"
#include "CVideoEditorImpl.h"

using Elastos::Droid::Media::Videoeditor::MediaArtistNativeHelper;
using Elastos::Droid::Media::Videoeditor::CWaveformData;
using Elastos::IO::IFile;
using Elastos::IO::CFile;

namespace Elastos {
namespace Droid {
namespace Media {
namespace Videoeditor {

CAudioTrack2::CAudioTrack2()
    : mUniqueId(NULL)
    , mFilename(NULL)
    , mStartTimeMs(0)
    , mTimelineDurationMs(0)
    , mVolumePercent(0)
    , mBeginBoundaryTimeMs(0)
    , mEndBoundaryTimeMs(0)
    , mLoop(FALSE)
    , mMuted(FALSE)
    , mDurationMs(0)
    , mAudioChannels(0)
    , mAudioType(0)
    , mAudioBitrate(0)
    , mAudioSamplingFrequency(0)
    , mDuckingThreshold(0)
    , mDuckedTrackVolume(0)
    , mIsDuckingEnabled(FALSE)
    , mAudioWaveformFilename(NULL)
{
}

ECode CAudioTrack2::constructor(
    /* [in] */ IVideoEditor* editor,
    /* [in] */ const String& audioTrackId,
    /* [in] */ const String& fileName)
{
    return Init(editor, audioTrackId, fileName, 0, 0, IMediaItem::END_OF_FILE, FALSE, 100, FALSE, FALSE, 0, 0, String(NULL));
}

ECode CAudioTrack2::constructor(
    /* [in] */ IVideoEditor* editor,
    /* [in] */ const String& audioTrackId,
    /* [in] */ const String& fileName,
    /* [in] */ Int64 startTimeMs,
    /* [in] */ Int64 beginMs,
    /* [in] */ Int64 endMs,
    /* [in] */ Boolean loop,
    /* [in] */ Int32 volume,
    /* [in] */ Boolean muted,
    /* [in] */ Boolean duckingEnabled,
    /* [in] */ Int32 duckThreshold,
    /* [in] */ Int32 duckedTrackVolume,
    /* [in] */ const String& audioWaveformFilename)
{
    return Init(editor, audioTrackId, fileName, startTimeMs, beginMs, endMs, loop, volume, muted,
        duckingEnabled, duckThreshold, duckedTrackVolume, audioWaveformFilename);
}

ECode CAudioTrack2::Init(
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
    /* [in] */ const String& audioWaveformFilename)
{
    AutoPtr<MediaArtistNativeHelper::Properties> properties;

    AutoPtr<IFile> file;
    CFile::New(filename, (IFile**)&file);
    Boolean b;
    file->Exists(&b);
    if (!b) {
    //    throw new IOException(filename + " not found ! ");
    }

    /*Compare file_size with 2GB*/
    Int64 len;
    file->GetLength(&len);
    if (IVideoEditor::MAX_SUPPORTED_FILE_SIZE <= len) {
    //    throw new IllegalArgumentException("File size is more than 2GB");
    }

    if (!IVideoEditorImpl::Probe(editor)) {
        ((CVideoEditorImpl*)IVideoEditorImpl::Probe(editor))->GetNativeContext((MediaArtistNativeHelper**)&mMANativeHelper);
    } else {
    //    throw new IllegalArgumentException("editor is not of type VideoEditorImpl");
    }
    //try {
        properties = ((MediaArtistNativeHelper*)mMANativeHelper)->GetMediaProperties(filename);
    //} catch (Exception e) {
    //    throw new IllegalArgumentException(e.getMessage() + " : " + filename);
    //}
    Int32 fileType;
    (MediaArtistNativeHelper*)mMANativeHelper->GetFileType(properties->fileType, &fileType);
    switch (fileType) {
        case IMediaProperties::FILE_3GP:
        case IMediaProperties::FILE_MP4:
        case IMediaProperties::FILE_MP3:
        case IMediaProperties::FILE_AMR:
            break;

        default: {
        //    throw new IllegalArgumentException("Unsupported input file type: " + fileType);
        }
    }
    Int32 type;
    (MediaArtistNativeHelper*)mMANativeHelper->GetAudioCodecType(properties->audioFormat, &type);
    switch (type) {
        case IMediaProperties::ACODEC_AMRNB:
        case IMediaProperties::ACODEC_AMRWB:
        case IMediaProperties::ACODEC_AAC_LC:
        case IMediaProperties::ACODEC_MP3:
            break;
        default:
        //    throw new IllegalArgumentException("Unsupported Audio Codec Format in Input File");
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (endMs == IMediaItem::END_OF_FILE) {
        endMs = properties->audioDuration;
    }

    mUniqueId = audioTrackId;
    mFilename = filename;
    mStartTimeMs = startTimeMs;
    mDurationMs = properties->audioDuration;
    mAudioChannels = properties->audioChannels;
    mAudioBitrate = properties->audioBitrate;
    mAudioSamplingFrequency = properties->audioSamplingFrequency;
    mAudioType = properties->audioFormat;
    mTimelineDurationMs = endMs - beginMs;
    mVolumePercent = volume;

    mBeginBoundaryTimeMs = beginMs;
    mEndBoundaryTimeMs = endMs;

    mLoop = loop;
    mMuted = muted;
    mIsDuckingEnabled = duckingEnabled;
    mDuckingThreshold = duckThreshold;
    mDuckedTrackVolume = duckedTrackVolume;

    mAudioWaveformFilename = audioWaveformFilename;
    if (audioWaveformFilename != NULL) {
        CWaveformData::New(audioWaveformFilename, (IWaveformData**)&mWaveformData);
    } else {
        mWaveformData = NULL;
    }
    return NOERROR;
}

ECode CAudioTrack2::GetId(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mUniqueId;
    return NOERROR;
}

ECode CAudioTrack2::GetFilename(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mFilename;
    return NOERROR;
}

ECode CAudioTrack2::GetAudioChannels(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mAudioChannels;
    return NOERROR;
}

ECode CAudioTrack2::GetAudioType(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mAudioType;
    return NOERROR;
}

ECode CAudioTrack2::GetAudioSamplingFrequency(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mAudioSamplingFrequency;
    return NOERROR;
}

ECode CAudioTrack2::GetAudioBitrate(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mAudioBitrate;
    return NOERROR;
}

ECode CAudioTrack2::SetVolume(
    /* [in] */ Int32 volumePercent)
{
    if (volumePercent > IMediaProperties::AUDIO_MAX_VOLUME_PERCENT) {
    //    throw new IllegalArgumentException("Volume set exceeds maximum allowed value");
    }

    if (volumePercent < 0) {
    //    throw new IllegalArgumentException("Invalid Volume ");
    }

    /**
     *  Force update of preview settings
     */
    ((MediaArtistNativeHelper*)mMANativeHelper)->SetGeneratePreview(TRUE);

    mVolumePercent = volumePercent;

    return NOERROR;
}

ECode CAudioTrack2::GetVolume(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mVolumePercent;
    return NOERROR;
}

ECode CAudioTrack2::SetMute(
    /* [in] */ Boolean muted)
{
    /**
     *  Force update of preview settings
     */
    ((MediaArtistNativeHelper*)mMANativeHelper)->SetGeneratePreview(TRUE);
    mMuted = muted;

    return NOERROR;
}

ECode CAudioTrack2::IsMuted(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mMuted;
    return NOERROR;
}

ECode CAudioTrack2::GetStartTime(
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mStartTimeMs;
    return NOERROR;
}

ECode CAudioTrack2::GetDuration(
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mDurationMs;
    return NOERROR;
}

ECode CAudioTrack2::GetTimelineDuration(
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mTimelineDurationMs;
    return NOERROR;
}

ECode CAudioTrack2::SetExtractBoundaries(
    /* [in] */ Int64 beginMs,
    /* [in] */ Int64 endMs)
{
    if (beginMs > mDurationMs) {
    //    throw new IllegalArgumentException("Invalid start time");
    }
    if (endMs > mDurationMs) {
    //    throw new IllegalArgumentException("Invalid end time");
    }
    if (beginMs < 0) {
    //    throw new IllegalArgumentException("Invalid start time; is < 0");
    }
    if (endMs < 0) {
    //    throw new IllegalArgumentException("Invalid end time; is < 0");
    }

    /**
     *  Force update of preview settings
     */
    ((MediaArtistNativeHelper*)mMANativeHelper)->SetGeneratePreview(TRUE);

    mBeginBoundaryTimeMs = beginMs;
    mEndBoundaryTimeMs = endMs;

    mTimelineDurationMs = mEndBoundaryTimeMs - mBeginBoundaryTimeMs;

    return NOERROR;
}

ECode CAudioTrack2::GetBoundaryBeginTime(
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mBeginBoundaryTimeMs;
    return NOERROR;
}

ECode CAudioTrack2::GetBoundaryEndTime(
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mEndBoundaryTimeMs;
    return NOERROR;
}

ECode CAudioTrack2::EnableLoop()
{
    if (!mLoop) {
        /**
         *  Force update of preview settings
         */
        ((MediaArtistNativeHelper*)mMANativeHelper)->SetGeneratePreview(TRUE);
        mLoop = TRUE;
    }
    return NOERROR;
}

ECode CAudioTrack2::DisableLoop(){
    if (mLoop) {
        /**
         *  Force update of preview settings
         */
        ((MediaArtistNativeHelper*)mMANativeHelper)->SetGeneratePreview(TRUE);
        mLoop = FALSE;
    }
    return NOERROR;
}

ECode CAudioTrack2::IsLooping(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mLoop;
    return NOERROR;
}

ECode CAudioTrack2::DisableDucking()
{
    if (mIsDuckingEnabled) {
        /**
         *  Force update of preview settings
         */
        ((MediaArtistNativeHelper*)mMANativeHelper)->SetGeneratePreview(TRUE);
        mIsDuckingEnabled = FALSE;
    }
    return NOERROR;
}

ECode CAudioTrack2::EnableDucking(
    /* [in] */ Int32 threshold,
    /* [in] */ Int32 duckedTrackVolume)
{
    if (threshold < 0 || threshold > 90) {
    //    throw new IllegalArgumentException("Invalid threshold value: " + threshold);
    }

    if (duckedTrackVolume < 0 || duckedTrackVolume > 100) {
    //    throw new IllegalArgumentException("Invalid duckedTrackVolume value: " + duckedTrackVolume);
    }

    /**
     *  Force update of preview settings
     */
    ((MediaArtistNativeHelper*)mMANativeHelper)->SetGeneratePreview(TRUE);

    mDuckingThreshold = threshold;
    mDuckedTrackVolume = duckedTrackVolume;
    mIsDuckingEnabled = TRUE;
    return NOERROR;
}

ECode CAudioTrack2::IsDuckingEnabled(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mIsDuckingEnabled;
    return NOERROR;
}

ECode CAudioTrack2::GetDuckingThreshhold(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mDuckingThreshold;
    return NOERROR;
}

ECode CAudioTrack2::GetDuckedTrackVolume(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mDuckedTrackVolume;
    return NOERROR;
}

ECode CAudioTrack2::ExtractAudioWaveform(
    /* [in] */ IExtractAudioWaveformProgressListener* listener){
    if (mAudioWaveformFilename == NULL) {
        /**
         *  AudioWaveformFilename is generated
         */
        String projectPath;
        ((MediaArtistNativeHelper*)mMANativeHelper)->GetProjectPath(&projectPath);
        String audioWaveFilename = projectPath + "/audioWaveformFile-" + mUniqueId + ".dat";

        /**
         * Logic to get frame duration = (no. of frames per sample * 1000)/
         * sampling frequency
         */
        Int32 frameDuration;
        Int32 sampleCount;
        Int32 codecType;
        ((MediaArtistNativeHelper*)mMANativeHelper)->GetAudioCodecType(mAudioType, &codecType);
        switch (codecType) {
            case IMediaProperties::ACODEC_AMRNB: {
                frameDuration = (IMediaProperties::SAMPLES_PER_FRAME_AMRNB * 1000)
                / IMediaProperties::DEFAULT_SAMPLING_FREQUENCY;
                sampleCount = IMediaProperties::SAMPLES_PER_FRAME_AMRNB;
                break;
            }

            case IMediaProperties::ACODEC_AMRWB: {
                frameDuration = (IMediaProperties::SAMPLES_PER_FRAME_AMRWB * 1000)
                / IMediaProperties::DEFAULT_SAMPLING_FREQUENCY;
                sampleCount = IMediaProperties::SAMPLES_PER_FRAME_AMRWB;
                break;
            }

            case IMediaProperties::ACODEC_AAC_LC: {
                frameDuration = (IMediaProperties::SAMPLES_PER_FRAME_AAC * 1000)
                / IMediaProperties::DEFAULT_SAMPLING_FREQUENCY;
                sampleCount = IMediaProperties::SAMPLES_PER_FRAME_AAC;
                break;
            }

            case IMediaProperties::ACODEC_MP3: {
                frameDuration = (IMediaProperties::SAMPLES_PER_FRAME_MP3 * 1000)
                / IMediaProperties::DEFAULT_SAMPLING_FREQUENCY;
                sampleCount = IMediaProperties::SAMPLES_PER_FRAME_MP3;
                break;
            }

            default: {
            //    throw new IllegalStateException("Unsupported codec type: " + codecType);
            }
        }

        ((MediaArtistNativeHelper*)mMANativeHelper)->GenerateAudioGraph( mUniqueId,
                mFilename,
                audioWaveFilename,
                frameDuration,
                IMediaProperties::DEFAULT_CHANNEL_COUNT,
                sampleCount,
                listener,
                FALSE);
        /**
         *  Record the generated file name
         */
        mAudioWaveformFilename = audioWaveFilename;
    }
    CWaveformData::New(mAudioWaveformFilename, (IWaveformData**)&mWaveformData);
    return NOERROR;
}

ECode CAudioTrack2::GetAudioWaveformFilename(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mAudioWaveformFilename;
    return NOERROR;
}

ECode CAudioTrack2::Invalidate()
{
    if (mAudioWaveformFilename != NULL) {
        AutoPtr<IFile> file;
        CFile::New(mAudioWaveformFilename, (IFile**)&file);
        Boolean b;
        file->Delete(&b);
        mAudioWaveformFilename = NULL;
        mWaveformData = NULL;
    }
    return NOERROR;
}

ECode CAudioTrack2::GetWaveformData(
    /* [out] */ IWaveformData** result)
{
    VALIDATE_NOT_NULL(result);
    *result = NULL;

    if (mWaveformData == NULL) {
        *result = NULL;
        return NOERROR;
    }

    if (mWaveformData != NULL) {
        *result = mWaveformData;
        INTERFACE_ADDREF(*result);
        return NOERROR;
    }
    else if (mAudioWaveformFilename != NULL) {
        // try {
        CWaveformData::New(mAudioWaveformFilename, (IWaveformData**)&mWaveformData);
        // } catch (IOException e) {
        //     throw e;
        // }
        *result = mWaveformData;
        INTERFACE_ADDREF(*result);
        return NOERROR;
    }
    else {
        *result = NULL;
        return NOERROR;
    }
}

ECode CAudioTrack2::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    if (!IAudioTrack2::Probe(object)) {
        *result = FALSE;
        return NOERROR;
    }
    *result = mUniqueId.Equals(((CAudioTrack2*)IEffect::Probe(object))->mUniqueId);
    return NOERROR;
}

ECode CAudioTrack2::GetHashCode(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)
    *result = mUniqueId.GetHashCode();
    return NOERROR;
}


}// namespace Videoeditor
} // namespace Media
} // namepsace Droid
} // namespace Elastos
