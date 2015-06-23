
#include "ext/frameworkext.h"
#include "CMediaVideoItem.h"
#include "CWaveformData.h"
#include "CVideoEditorProfile.h"
#include "CVideoEditorProfileHelper.h"
#include "CVideoEditorImpl.h"
#include "Transition.h"

using Elastos::IO::IFile;
using Elastos::IO::CFile;

namespace Elastos {
namespace Droid {
namespace Media {
namespace Videoeditor {

IMEDIAITEM_METHODS_IMPL(CMediaVideoItem, MediaItem);

CMediaVideoItem::CMediaVideoItem()
    : mWidth(0)
    , mHeight(0)
    , mAspectRatio(0)
    , mFileType(0)
    , mVideoType(0)
    , mVideoProfile(0)
    , mVideoLevel(0)
    , mVideoBitrate(0)
    , mDurationMs(0)
    , mAudioBitrate(0)
    , mFps(0)
    , mAudioType(0)
    , mAudioChannels(0)
    , mAudioSamplingFrequency(0)
    , mBeginBoundaryTimeMs(0)
    , mEndBoundaryTimeMs(0)
    , mVolumePercentage(0)
    , mMuted(FALSE)
    , mAudioWaveformFilename(NULL)
    , mVideoRotationDegree(0)
{
}

CMediaVideoItem::CMediaVideoItem(
    /* [in] */ IVideoEditor* editor,
    /* [in] */ const String& mediaItemId,
    /* [in] */ const String& filename,
    /* [in] */ Int32 renderingMode)
    : mWidth(0)
    , mHeight(0)
    , mAspectRatio(0)
    , mFileType(0)
    , mVideoType(0)
    , mVideoProfile(0)
    , mVideoLevel(0)
    , mVideoBitrate(0)
    , mDurationMs(0)
    , mAudioBitrate(0)
    , mFps(0)
    , mAudioType(0)
    , mAudioChannels(0)
    , mAudioSamplingFrequency(0)
    , mBeginBoundaryTimeMs(0)
    , mEndBoundaryTimeMs(0)
    , mVolumePercentage(0)
    , mMuted(FALSE)
    , mAudioWaveformFilename(NULL)
    , mVideoRotationDegree(0)
{
}

PInterface CMediaVideoItem::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_MediaItem) {
        return reinterpret_cast<PInterface>((MediaItem*)this);
    }
    return _CMediaVideoItem::Probe(riid);
}

ECode CMediaVideoItem::constructor(
    /* [in] */ IVideoEditor* editor,
    /* [in] */ const String& mediaItemId,
    /* [in] */ const String& filename,
    /* [in] */ Int32 renderingMode)
{
    return constructor(editor, mediaItemId, filename, renderingMode, 0, END_OF_FILE, 100, FALSE, String(NULL));
}

ECode CMediaVideoItem::constructor(
    /* [in] */ IVideoEditor* editor,
    /* [in] */ const String& mediaItemId,
    /* [in] */ const String& filename,
    /* [in] */ Int32 renderingMode,
    /* [in] */ Int64 beginMs,
    /* [in] */ Int64 endMs,
    /* [in] */ Int32 volumePercent,
    /* [in] */ Boolean muted,
    /* [in] */ const String& audioWaveformFilename)
{
    Init(editor, mediaItemId, filename, renderingMode);

    AutoPtr<IVideoEditorImpl> impl = (IVideoEditorImpl*)IVideoEditorImpl::Probe(editor);
    if (impl != NULL) {
        ((CVideoEditorImpl*)impl.Get())->GetNativeContext((MediaArtistNativeHelper**)&mMANativeHelper);
        mVideoEditor = impl;
    }

    AutoPtr<MediaArtistNativeHelper::Properties> properties;
    //try {
         properties = mMANativeHelper->GetMediaProperties(filename);
    //} catch ( Exception e) {
    //    throw new IllegalArgumentException(e.getMessage() + " : " + filename);
    //}

    /** Check the platform specific maximum import resolution */
    AutoPtr<IVideoEditorProfileHelper> helper;
    CVideoEditorProfileHelper::AcquireSingleton((IVideoEditorProfileHelper**)&helper);
    AutoPtr<IVideoEditorProfile> veProfile;
    helper->Get((IVideoEditorProfile**)&veProfile);
    if (veProfile == NULL) {
        // throw new RuntimeException("Can't get the video editor profile");
        return E_RUNTIME_EXCEPTION;
    }
    Int32 maxInputWidth = ((CVideoEditorProfile*)veProfile.Get())->maxInputVideoFrameWidth;
    Int32 maxInputHeight = ((CVideoEditorProfile*)veProfile.Get())->maxInputVideoFrameHeight;
    if ((properties->width > maxInputWidth) ||
        (properties->height > maxInputHeight)) {
        // throw new IllegalArgumentException("Unsupported import resolution. Supported maximum width:" +
        //     maxInputWidth + " height:" + maxInputHeight + ", current width:" + properties->width +
        //     " height:" + properties->height);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    /** Check the platform specific maximum video profile and level */
    if (!properties->profileSupported) {
        // throw new IllegalArgumentException("Unsupported video profile " + properties->profile);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (!properties->levelSupported) {
        // throw new IllegalArgumentException("Unsupported video level " + properties->level);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Int32 fileType;
    mMANativeHelper->GetFileType(properties->fileType, &fileType);
    switch (fileType) {
        case IMediaProperties::FILE_3GP:
        case IMediaProperties::FILE_MP4:
        case IMediaProperties::FILE_M4V:
            break;

        default:
            // throw new IllegalArgumentException("Unsupported Input File Type");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Int32 codecType;
    mMANativeHelper->GetVideoCodecType(properties->videoFormat, &codecType);
    switch (codecType) {
        case IMediaProperties::VCODEC_H263:
        case IMediaProperties::VCODEC_H264:
        case IMediaProperties::VCODEC_MPEG4:
            break;

        default:
            // throw new IllegalArgumentException("Unsupported Video Codec Format in Input File");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    mWidth = properties->width;
    mHeight = properties->height;
    mMANativeHelper->GetAspectRatio(properties->width, properties->height, &mAspectRatio);
    mMANativeHelper->GetFileType(properties->fileType, &mFileType);
    mMANativeHelper->GetVideoCodecType(properties->videoFormat, &mVideoType);
    mVideoProfile = properties->profile;
    mVideoLevel = properties->level;
    mDurationMs = properties->videoDuration;
    mVideoBitrate = properties->videoBitrate;
    mAudioBitrate = properties->audioBitrate;
    mFps = (Int32)properties->averageFrameRate;
    mMANativeHelper->GetAudioCodecType(properties->audioFormat, &mAudioType);
    mAudioChannels = properties->audioChannels;
    mAudioSamplingFrequency =  properties->audioSamplingFrequency;
    mBeginBoundaryTimeMs = beginMs;
    mEndBoundaryTimeMs = endMs == END_OF_FILE ? mDurationMs : endMs;
    mVolumePercentage = volumePercent;
    mMuted = muted;
    mAudioWaveformFilename = audioWaveformFilename;
    if (audioWaveformFilename != NULL) {
        CWaveformData::New(audioWaveformFilename, (IWaveformData**)&mWaveformData);
    }
    else {
        mWaveformData = NULL;
    }
    mVideoRotationDegree = properties->videoRotation;

    return NOERROR;
}

ECode CMediaVideoItem::SetExtractBoundaries(
    /* [in] */ Int64 beginMs,
    /* [in] */ Int64 endMs)
{
    if (beginMs > mDurationMs) {
        // throw new IllegalArgumentException("setExtractBoundaries: Invalid start time");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (endMs > mDurationMs) {
        // throw new IllegalArgumentException("setExtractBoundaries: Invalid end time");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if ((endMs != -1) && (beginMs >= endMs) ) {
        // throw new IllegalArgumentException("setExtractBoundaries: Start time is greater than end time");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if ((beginMs < 0) || ((endMs != -1) && (endMs < 0))) {
        // throw new IllegalArgumentException("setExtractBoundaries: Start time or end time is negative");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    mMANativeHelper->SetGeneratePreview(TRUE);

    if (beginMs != mBeginBoundaryTimeMs) {
        if (mBeginTransition != NULL) {
            (reinterpret_cast<Transition*>(mBeginTransition->Probe(EIID_Transition)))->Invalidate();
        }
    }

    if (endMs != mEndBoundaryTimeMs) {
        if (mEndTransition != NULL) {
            (reinterpret_cast<Transition*>(mEndTransition->Probe(EIID_Transition)))->Invalidate();
        }
    }

    mBeginBoundaryTimeMs = beginMs;
    mEndBoundaryTimeMs = endMs;
    AdjustTransitions();
    ((CVideoEditorImpl*)mVideoEditor.Get())->UpdateTimelineDuration();
    return NOERROR;
    /**
     *  Note that the start and duration of any effects and overlays are
     *  not adjusted nor are they automatically removed if they fall
     *  outside the new boundaries.
     */
}

ECode CMediaVideoItem::GetBoundaryBeginTime(
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mBeginBoundaryTimeMs;
    return NOERROR;
}

ECode CMediaVideoItem::GetBoundaryEndTime(
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mEndBoundaryTimeMs;
    return NOERROR;
}

ECode CMediaVideoItem::AddEffect(
    /* [in] */ IEffect* effect)
{
    AutoPtr<IEffectKenBurns> ekb = (IEffectKenBurns*)IEffectKenBurns::Probe(effect);
    if (ekb != NULL) {
        // throw new IllegalArgumentException("Ken Burns effects cannot be applied to MediaVideoItem");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return MediaItem::AddEffect(effect);
}

ECode CMediaVideoItem::GetThumbnail(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int64 timeMs,
    /* [out] */ IBitmap** result)
{
    VALIDATE_NOT_NULL(result);

    if (timeMs > mDurationMs) {
        // throw new IllegalArgumentException("Time Exceeds duration");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (timeMs < 0) {
        // throw new IllegalArgumentException("Invalid Time duration");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if ((width <= 0) || (height <= 0)) {
        // throw new IllegalArgumentException("Invalid Dimensions");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (mVideoRotationDegree == 90 || mVideoRotationDegree == 270) {
        Int32 temp = width;
        width = height;
        height = temp;
    }

    String str;
    GetFilename(&str);
    return mMANativeHelper->GetPixels(str, width, height, timeMs, mVideoRotationDegree, result);
}

ECode CMediaVideoItem::GetThumbnailList(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int64 startMs,
    /* [in] */ Int64 endMs,
    /* [in] */ Int32 thumbnailCount,
    /* [in] */ ArrayOf<Int32>* indices,
    /* [in] */ IMediaItemGetThumbnailListCallback* callback)
{
    if (startMs > endMs) {
        // throw new IllegalArgumentException("Start time is greater than end time");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (endMs > mDurationMs) {
        // throw new IllegalArgumentException("End time is greater than file duration");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if ((height <= 0) || (width <= 0)) {
        // throw new IllegalArgumentException("Invalid dimension");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (mVideoRotationDegree == 90 || mVideoRotationDegree == 270) {
        Int32 temp = width;
        width = height;
        height = temp;
    }

    String str;
    GetFilename(&str);
    return mMANativeHelper->GetPixelsList(str, width, height,
            startMs, endMs, thumbnailCount, indices, callback,
            mVideoRotationDegree);
}

ECode CMediaVideoItem::InvalidateTransitions(
    /* [in] */ Int64 startTimeMs,
    /* [in] */ Int64 durationMs)
{
    /**
     *  Check if the item overlaps with the beginning and end transitions
     */
    if (mBeginTransition != NULL) {
        Int64 transitionDurationMs;
        mBeginTransition->GetDuration(&transitionDurationMs);
        if (IsOverlapping(startTimeMs, durationMs,
                mBeginBoundaryTimeMs, transitionDurationMs)) {
            (reinterpret_cast<Transition*>(mBeginTransition->Probe(EIID_Transition)))->Invalidate();
        }
    }

    if (mEndTransition != NULL) {
        Int64 transitionDurationMs;
        mEndTransition->GetDuration(&transitionDurationMs);
        if (IsOverlapping(startTimeMs, durationMs,
                mEndBoundaryTimeMs - transitionDurationMs, transitionDurationMs)) {
            (reinterpret_cast<Transition*>(mEndTransition->Probe(EIID_Transition)))->Invalidate();
        }
    }
    return NOERROR;
}

ECode CMediaVideoItem::InvalidateTransitions(
    /* [in] */ Int64 oldStartTimeMs,
    /* [in] */ Int64 oldDurationMs,
    /* [in] */ Int64 newStartTimeMs,
    /* [in] */ Int64 newDurationMs)
{
    /**
     *  Check if the item overlaps with the beginning and end transitions
     */
    if (mBeginTransition != NULL) {
        Int64 transitionDurationMs;
        mBeginTransition->GetDuration(&transitionDurationMs);
        Boolean oldOverlap = IsOverlapping(oldStartTimeMs, oldDurationMs,
                mBeginBoundaryTimeMs, transitionDurationMs);
        Boolean newOverlap = IsOverlapping(newStartTimeMs, newDurationMs,
                mBeginBoundaryTimeMs, transitionDurationMs);
        /**
         * Invalidate transition if:
         *
         * 1. New item overlaps the transition, the old one did not
         * 2. New item does not overlap the transition, the old one did
         * 3. New and old item overlap the transition if begin or end
         * time changed
         */
        if (newOverlap != oldOverlap) { // Overlap has changed
            (reinterpret_cast<Transition*>(mBeginTransition->Probe(EIID_Transition)))->Invalidate();
        }
        else if (newOverlap) { // Both old and new overlap
            if ((oldStartTimeMs != newStartTimeMs) ||
                    !(oldStartTimeMs + oldDurationMs > transitionDurationMs &&
                    newStartTimeMs + newDurationMs > transitionDurationMs)) {
                (reinterpret_cast<Transition*>(mBeginTransition->Probe(EIID_Transition)))->Invalidate();
            }
        }
    }

    if (mEndTransition != NULL) {
        Int64 transitionDurationMs;
        mEndTransition->GetDuration(&transitionDurationMs);
        Boolean oldOverlap = IsOverlapping(oldStartTimeMs, oldDurationMs,
                mEndBoundaryTimeMs - transitionDurationMs, transitionDurationMs);
        Boolean newOverlap = IsOverlapping(newStartTimeMs, newDurationMs,
                mEndBoundaryTimeMs - transitionDurationMs, transitionDurationMs);
        /**
         * Invalidate transition if:
         *
         * 1. New item overlaps the transition, the old one did not
         * 2. New item does not overlap the transition, the old one did
         * 3. New and old item overlap the transition if begin or end
         * time changed
         */
        if (newOverlap != oldOverlap) { // Overlap has changed
            (reinterpret_cast<Transition*>(mEndTransition->Probe(EIID_Transition)))->Invalidate();
        }
        else if (newOverlap) { // Both old and new overlap
            if ((oldStartTimeMs + oldDurationMs != newStartTimeMs + newDurationMs) ||
                    ((oldStartTimeMs > mEndBoundaryTimeMs - transitionDurationMs) ||
                    newStartTimeMs > mEndBoundaryTimeMs - transitionDurationMs)) {
                (reinterpret_cast<Transition*>(mEndTransition->Probe(EIID_Transition)))->Invalidate();
            }
        }
    }
    return NOERROR;
}

ECode CMediaVideoItem::GetAspectRatio(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mAspectRatio;
    return NOERROR;
}

ECode CMediaVideoItem::GetFileType(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mFileType;
    return NOERROR;
}

ECode CMediaVideoItem::GetWidth(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    if (mVideoRotationDegree == 90 ||mVideoRotationDegree == 270)
    {
        *result = mHeight;
    }
    else {
        *result = mWidth;
    }
    return NOERROR;
}

ECode CMediaVideoItem::GetHeight(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    if (mVideoRotationDegree == 90 || mVideoRotationDegree == 270)
    {
        *result = mWidth;
    }
    else {
        *result = mHeight;
    }
    return NOERROR;
}

ECode CMediaVideoItem::GetDuration(
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mDurationMs;
    return NOERROR;
}

ECode CMediaVideoItem::GetTimelineDuration(
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mEndBoundaryTimeMs - mBeginBoundaryTimeMs;
    return NOERROR;
}

ECode CMediaVideoItem::RenderFrame(
    /* [in] */ ISurfaceHolder* surfaceHolder,
    /* [in] */ Int64 timeMs,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);

    if (surfaceHolder == NULL) {
        // throw new IllegalArgumentException("Surface Holder is null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (timeMs > mDurationMs || timeMs < 0) {
        // throw new IllegalArgumentException("requested time not correct");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoPtr<ISurface> surface;
    surfaceHolder->GetSurface((ISurface**)&surface);
    if (surface == NULL) {
        // throw new RuntimeException("Surface could not be retrieved from Surface holder");
        return E_RUNTIME_EXCEPTION;
    }

    if (mFilename != NULL) {
        return mMANativeHelper->RenderMediaItemPreviewFrame(surface,
                mFilename,timeMs,mWidth,mHeight, result);
    } else {
        *result = 0;
        return NOERROR;
    }
}

ECode CMediaVideoItem::ExtractAudioWaveform(
    /* [in] */ IExtractAudioWaveformProgressListener* listener)
{
    Int32 frameDuration = 0;
    Int32 sampleCount = 0;
    String projectPath;
    mMANativeHelper->GetProjectPath(&projectPath);
    /**
     *  Waveform file does not exist
     */
    if (mAudioWaveformFilename == NULL ) {
        /**
         * Since audioWaveformFilename will not be supplied,it is  generated
         */
        String mAudioWaveFileName;

        String id;
        GetId(&id);
        mAudioWaveFileName = projectPath + "/" + "audioWaveformFile-" + id + ".dat";
        /**
         * Logic to get frame duration = (no. of frames per sample * 1000)/
         * sampling frequency
         */
        Int32 type;
        mMANativeHelper->GetAudioCodecType(mAudioType, &type);
        if (type == IMediaProperties::ACODEC_AMRNB ) {
            frameDuration = (IMediaProperties::SAMPLES_PER_FRAME_AMRNB * 1000)/
            IMediaProperties::DEFAULT_SAMPLING_FREQUENCY;
            sampleCount = IMediaProperties::SAMPLES_PER_FRAME_AMRNB;
        }
        else if (type == IMediaProperties::ACODEC_AMRWB ) {
            frameDuration = (IMediaProperties::SAMPLES_PER_FRAME_AMRWB * 1000)/
            IMediaProperties::DEFAULT_SAMPLING_FREQUENCY;
            sampleCount = IMediaProperties::SAMPLES_PER_FRAME_AMRWB;
        }
        else if (type == IMediaProperties::ACODEC_AAC_LC ) {
            frameDuration = (IMediaProperties::SAMPLES_PER_FRAME_AAC * 1000)/
            IMediaProperties::DEFAULT_SAMPLING_FREQUENCY;
            sampleCount = IMediaProperties::SAMPLES_PER_FRAME_AAC;
        }

        mMANativeHelper->GenerateAudioGraph(id,
                mFilename,
                mAudioWaveFileName,
                frameDuration,
                IMediaProperties::DEFAULT_CHANNEL_COUNT,
                sampleCount,
                listener,
                TRUE);
        /**
         * Record the generated file name
         */
        mAudioWaveformFilename = mAudioWaveFileName;
    }
    CWaveformData::New(mAudioWaveformFilename, (IWaveformData**)&mWaveformData);
    return NOERROR;
}

ECode CMediaVideoItem::GetAudioWaveformFilename(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mAudioWaveformFilename;
    return NOERROR;
}

ECode CMediaVideoItem::Invalidate()
{
    if (mAudioWaveformFilename != NULL) {
        AutoPtr<IFile> file;
        CFile::New(mAudioWaveformFilename, (IFile**)&file);
        Boolean b;
        file->Delete(&b);
        mAudioWaveformFilename = NULL;
    }
    return NOERROR;
}

ECode CMediaVideoItem::GetWaveformData(
    /* [out] */ IWaveformData** result)
{
    VALIDATE_NOT_NULL(result);

    if (mWaveformData == NULL) {
        *result = NULL;
        return NOERROR;
    }

    if (mWaveformData != NULL) {
        *result = mWaveformData;
        REFCOUNT_ADD(*result);
        return NOERROR;
    }
    else if (mAudioWaveformFilename != NULL) {
        //try {
            CWaveformData::New(mAudioWaveformFilename, (IWaveformData**)&mWaveformData);
        //} catch(IOException e) {
        //    throw e;
        //}
        *result = mWaveformData;
        REFCOUNT_ADD(*result);
        return NOERROR;
    }
    else {
        *result = NULL;
        return NOERROR;
    }
}

ECode CMediaVideoItem::SetVolume(
    /* [in] */ Int32 volumePercent)
{
    if ((volumePercent < 0) || (volumePercent > 100)) {
        // throw new IllegalArgumentException("Invalid volume");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    mVolumePercentage = volumePercent;

    return NOERROR;
}

ECode CMediaVideoItem::GetVolume(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mVolumePercentage;
    return NOERROR;
}

ECode CMediaVideoItem::SetMute(
    /* [in] */ Boolean muted)
{
    mMANativeHelper->SetGeneratePreview(TRUE);
    mMuted = muted;
    if (mBeginTransition != NULL) {
        (reinterpret_cast<Transition*>(mBeginTransition->Probe(EIID_Transition)))->Invalidate();
    }
    if (mEndTransition != NULL) {
        (reinterpret_cast<Transition*>(mEndTransition->Probe(EIID_Transition)))->Invalidate();
    }
    return NOERROR;
}

ECode CMediaVideoItem::IsMuted(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mMuted;
    return NOERROR;
}

ECode CMediaVideoItem::GetVideoType(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mVideoType;
    return NOERROR;
}

ECode CMediaVideoItem::GetVideoProfile(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mVideoProfile;
    return NOERROR;
}

ECode CMediaVideoItem::GetVideoLevel(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mVideoLevel;
    return NOERROR;
}

ECode CMediaVideoItem::GetVideoBitrate(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mVideoBitrate;
    return NOERROR;
}

ECode CMediaVideoItem::GetAudioBitrate(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mAudioBitrate;
    return NOERROR;
}

ECode CMediaVideoItem::GetFps(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mFps;
    return NOERROR;
}

ECode CMediaVideoItem::GetAudioType(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mAudioType;
    return NOERROR;
}

ECode CMediaVideoItem::GetAudioChannels(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mAudioChannels;
    return NOERROR;
}

ECode CMediaVideoItem::GetAudioSamplingFrequency(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mAudioSamplingFrequency;
    return NOERROR;
}

ECode CMediaVideoItem::GetVideoClipProperties(
    /* [out] */ MediaArtistNativeHelper::ClipSettings** settings)
{
    AutoPtr<MediaArtistNativeHelper::ClipSettings> clipSettings = new MediaArtistNativeHelper::ClipSettings();
    GetFilename(&clipSettings->clipPath);
    Int32 type;
    GetFileType(&type);
    mMANativeHelper->GetMediaItemFileType(type, &clipSettings->fileType);
    Int32 time;
    GetBoundaryBeginTime((Int64*)&time);
    clipSettings->beginCutTime = time;
    GetBoundaryEndTime((Int64*)&time);
    clipSettings->endCutTime = time;
    Int32 mode;
    GetRenderingMode(&mode);
    mMANativeHelper->GetMediaItemRenderingMode(mode, &clipSettings->mediaRendering);
    clipSettings->rotationDegree = mVideoRotationDegree;

    *settings = clipSettings;
    REFCOUNT_ADD(*settings);
    return NOERROR;
}

ECode CMediaVideoItem::SetGeneratedImageClip(
    /* [in] */ const String& generatedFilePath)
{
    return MediaItem::SetGeneratedImageClip(generatedFilePath);
}

ECode CMediaVideoItem::GetGeneratedImageClip(
    /* [out] */ String* result)
{
    return MediaItem::GetGeneratedImageClip(result);
}

} // namespace videoeditor
} // namespace Media
} // namepsace Droid
} // namespace Elastos
