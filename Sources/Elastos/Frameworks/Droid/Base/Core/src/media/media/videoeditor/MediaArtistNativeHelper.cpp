
#include "ext/frameworkext.h"
#include "MediaArtistNativeHelper.h"
#include "graphics/CPaint.h"
#include "graphics/CCanvas.h"
#include "graphics/CMatrix.h"
#include "graphics/CBitmapFactory.h"
#include <elastos/utility/logging/Logger.h>
#include "CVideoEditorOverlayData.h"
#include "CMediaImageItem.h"
#include "CVideoEditorProfileHelper.h"
#include "Transition.h"
#include "CMediaVideoItem.h"
#include "COverlayFrame.h"
#include "CMediaPropertiesHelper.h"
#include "CVideoEditorProfile.h"

using Elastos::IO::IFile;
using Elastos::IO::CFile;
using Elastos::IO::IInt32Buffer;
using Elastos::IO::IInt32BufferHelper;
using Elastos::IO::CInt32BufferHelper;
using Elastos::Core::IInteger32;
using Elastos::Math::IBigDecimal;
using Elastos::Math::CBigDecimal;
using Elastos::Utility::Logging::Logger;
using Elastos::Droid::Utility::IPair;
using Elastos::Droid::Graphics::CPaint;
using Elastos::Droid::Graphics::ICanvas;
using Elastos::Droid::Graphics::CCanvas;
using Elastos::Droid::Graphics::IMatrix;
using Elastos::Droid::Graphics::CMatrix;
using Elastos::Droid::Graphics::IBitmapFactory;
using Elastos::Droid::Graphics::CBitmapFactory;

namespace Elastos {
namespace Droid {
namespace Media {
namespace Videoeditor {

/** VIDEOEDITOR major version number */
const Int32 MediaArtistNativeHelper::Version::VIDEOEDITOR_MAJOR_VERSION = 0;

/** VIDEOEDITOR minor version number */
const Int32 MediaArtistNativeHelper::Version::VIDEOEDITOR_MINOR_VERSION = 0;

/** VIDEOEDITOR revision number */
const Int32 MediaArtistNativeHelper::Version::VIDEOEDITOR_REVISION_VERSION = 1;

const Int32 MediaArtistNativeHelper::TASK_LOADING_SETTINGS = 1;
const Int32 MediaArtistNativeHelper::TASK_ENCODING = 2;

// Processing indication
const Int32 MediaArtistNativeHelper::PROCESSING_NONE = 0;
const Int32 MediaArtistNativeHelper::PROCESSING_AUDIO_PCM = 1;
const Int32 MediaArtistNativeHelper::PROCESSING_TRANSITION = 2;
const Int32 MediaArtistNativeHelper::PROCESSING_KENBURNS = 3;
const Int32 MediaArtistNativeHelper::PROCESSING_INTERMEDIATE1 = 11;
const Int32 MediaArtistNativeHelper::PROCESSING_INTERMEDIATE2 = 12;
const Int32 MediaArtistNativeHelper::PROCESSING_INTERMEDIATE3 = 13;
const Int32 MediaArtistNativeHelper::PROCESSING_EXPORT = 20;

const String MediaArtistNativeHelper::TAG("MediaArtistNativeHelper");
const Int32 MediaArtistNativeHelper::MAX_THUMBNAIL_PERMITTED = 8;
const String MediaArtistNativeHelper::AUDIO_TRACK_PCM_FILE("AudioPcm.pcm");
AutoPtr<IPaint> MediaArtistNativeHelper::sResizePaint;

CAR_INTERFACE_IMPL(MediaArtistNativeHelper::MediaItemGetThumbnailListCallback, IMediaItemGetThumbnailListCallback)

ECode MediaArtistNativeHelper::MediaItemGetThumbnailListCallback::OnThumbnail(
    /* [in] */ IBitmap* bitmap,
    /* [in] */ Int32 index)
{
    (*mResult)[0] = bitmap;
    return NOERROR;
}

CAR_INTERFACE_IMPL(MediaArtistNativeHelper::MediaArtistNativeHelperNativeGetPixelsListCallback, IMediaArtistNativeHelperNativeGetPixelsListCallback)

ECode MediaArtistNativeHelper::MediaArtistNativeHelperNativeGetPixelsListCallback::OnThumbnail(
    /* [in] */ Int32 index)
{
    // This is the bitmap we will output to the client
    AutoPtr<IBitmapFactory> bitmapFactory;
    CBitmapFactory::AcquireSingleton((IBitmapFactory**)&bitmapFactory);
    AutoPtr<IBitmap> outBitmap;
    bitmapFactory->CreateBitmap(mWidth, mHeight, Elastos::Droid::Graphics::BitmapConfig_ARGB_8888, (IBitmap**)&outBitmap);

    // Copy int[] to IntBuffer
    mDecBuffer->Rewind();
    mDecBuffer->PutInt32s(*mDecArray, 0, mThumbnailSize);
    mDecBuffer->Rewind();

    if (!mNeedToMassage) {
        // We can directly read the decoded result to output bitmap
        outBitmap->CopyPixelsFromBuffer(mDecBuffer);
    }
    else {
        // Copy the decoded result to an intermediate bitmap first
        mBitmap->CopyPixelsFromBuffer(mDecBuffer);

        // Create a canvas to resize/rotate the bitmap
        // First scale the decoded bitmap to (0,0)-(1,1), rotate it
        // with (0.5, 0.5) as center, then scale it to
        // (outWidth, outHeight).
        AutoPtr<ICanvas> canvas;
        CCanvas::New(outBitmap, (ICanvas**)&canvas);
        AutoPtr<IMatrix> m;
        CMatrix::New((IMatrix**)&m);
        Float sx = 1 / mDecWidth;
        Float sy = 1 / mDecHeight;
        Boolean b;
        m->PostScale(sx, sy, &b);
        m->PostRotate(mVideoRotation, 0.5f, 0.5f, &b);
        m->PostScale(mWidth, mHeight, &b);
        canvas->DrawBitmap(mBitmap, m, sResizePaint);
    }
    mCallback->OnThumbnail(outBitmap, index);
    return NOERROR;
}

ECode MediaArtistNativeHelper::Version::GetVersion(
    /* [out] */ Version** result)
{
    AutoPtr<Version> version = new Version();

    version->major = Version::VIDEOEDITOR_MAJOR_VERSION;
    version->minor = Version::VIDEOEDITOR_MINOR_VERSION;
    version->revision = Version::VIDEOEDITOR_REVISION_VERSION;

    *result = version;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

MediaArtistNativeHelper::MediaArtistNativeHelper(
    /* [in] */ const String& projectPath,
    /* [in] */ ISemaphore* lock,
    /* [in] */ IVideoEditor* veObj)
    : mOutputFilename(NULL)
    , mInvalidatePreviewArray(TRUE)
    , mRegenerateAudio(TRUE)
    , mExportFilename(NULL)
    , mExportVideoCodec(0)
    , mExportAudioCodec(0)
    , mProgressToApp(0)
    , mRenderPreviewOverlayFile(NULL)
    , mRenderPreviewRenderingMode(0)
    , mIsFirstProgress(FALSE)
    , mProcessingState(0)
    , mProjectPath(NULL)
    , mPreviewProgress(0)
    , mAudioTrackPCMFilePath(NULL)
    , mTotalClips(0)
    , mErrorFlagSet(FALSE)
    , mManualEditContext(0)
{
    CPaint::New(IPaint::FILTER_BITMAP_FLAG, (IPaint**)&sResizePaint);

    mProjectPath = projectPath;
    if (veObj != NULL) {
        mVideoEditor = veObj;
    }
    else {
        mVideoEditor = NULL;
        //throw new IllegalArgumentException("video editor object is NULL");
    }
    if (mStoryBoardSettings == NULL) {
        mStoryBoardSettings = new EditSettings();
    }

    mLock = lock;

    _init(mProjectPath, String("NULL"));
}

ECode MediaArtistNativeHelper::GetProjectPath(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mProjectPath;
    return NOERROR;
}

ECode MediaArtistNativeHelper::GetProjectAudioTrackPCMFilePath(
        /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mAudioTrackPCMFilePath;
    return NOERROR;
}

ECode MediaArtistNativeHelper::InvalidatePcmFile()
{
    if (mAudioTrackPCMFilePath != NULL) {
        AutoPtr<IFile> file;
        CFile::New(mAudioTrackPCMFilePath, (IFile**)&file);
        Boolean b;
        file->Delete(&b);
        mAudioTrackPCMFilePath = NULL;
    }

    return NOERROR;
}

void MediaArtistNativeHelper::OnProgressUpdate(
    /* [in] */ Int32 taskId,
    /* [in] */ Int32 progress)
{
    if (mProcessingState == PROCESSING_EXPORT) {
        if (mExportProgressListener != NULL) {
            if (mProgressToApp < progress) {
                mExportProgressListener->OnProgress(mVideoEditor, mOutputFilename, progress);
                /* record previous progress */
                mProgressToApp = progress;
            }
        }
    }
    else {
        // Adapt progress depending on current state
        Int32 actualProgress = 0;
        Int32 action = 0;

        if (mProcessingState == PROCESSING_AUDIO_PCM) {
            action = IVideoEditorMediaProcessingProgressListener::ACTION_DECODE;
        } else {
            action = IVideoEditorMediaProcessingProgressListener::ACTION_ENCODE;
        }

        switch (mProcessingState) {
            case PROCESSING_AUDIO_PCM:
                actualProgress = progress;
                break;
            case PROCESSING_TRANSITION:
                actualProgress = progress;
                break;
            case PROCESSING_KENBURNS:
                actualProgress = progress;
                break;
            case PROCESSING_INTERMEDIATE1:
                if ((progress == 0) && (mProgressToApp != 0)) {
                    mProgressToApp = 0;
                }
                if ((progress != 0) || (mProgressToApp != 0)) {
                    actualProgress = progress/4;
                }
                break;
            case PROCESSING_INTERMEDIATE2:
                if ((progress != 0) || (mProgressToApp != 0)) {
                    actualProgress = 25 + progress/4;
                }
                break;
            case PROCESSING_INTERMEDIATE3:
                if ((progress != 0) || (mProgressToApp != 0)) {
                    actualProgress = 50 + progress/2;
                }
                break;
            case PROCESSING_NONE:

            default:
                Logger::E(TAG, "ERROR unexpected State= %d", mProcessingState);
                return;
        }
        if ((mProgressToApp != actualProgress) && (actualProgress != 0)) {

            mProgressToApp = actualProgress;

            if (mMediaProcessingProgressListener != NULL) {
                // Send the progress indication
                mMediaProcessingProgressListener->OnProgress(mProcessingObject, action,
                                                            actualProgress);
            }
        }
        /* avoid 0 in next intermediate call */
        if (mProgressToApp == 0) {
            if (mMediaProcessingProgressListener != NULL) {
                /*
                 *  Send the progress indication
                 */
                mMediaProcessingProgressListener->OnProgress(mProcessingObject, action,
                                                            actualProgress);
            }
            mProgressToApp = 1;
        }
    }
}

void MediaArtistNativeHelper::OnPreviewProgressUpdate(
    /* [in] */ Int32 progress,
    /* [in] */ Boolean isFinished,
    /* [in] */ Boolean updateOverlay,
    /* [in] */ const String& filename,
    /* [in] */ Int32 renderingMode,
    /* [in] */ Int32 error)
{
    if (mPreviewProgressListener != NULL) {
        if (mIsFirstProgress) {
            mPreviewProgressListener->OnStart(mVideoEditor);
            mIsFirstProgress = FALSE;
        }

        AutoPtr<IVideoEditorOverlayData> overlayData;
        if (updateOverlay) {
            CVideoEditorOverlayData::New((IVideoEditorOverlayData**)&overlayData);
            if (filename != NULL) {

                AutoPtr<IBitmapFactory> factory;
                CBitmapFactory::AcquireSingleton((IBitmapFactory**)&factory);
                AutoPtr<IBitmap> bitmap;
                factory->DecodeFile(filename, (IBitmap**)&bitmap);
                ((CVideoEditorOverlayData*)overlayData.Get())->Set(bitmap, renderingMode);
            } else {
                ((CVideoEditorOverlayData*)overlayData.Get())->SetClear();
            }
        } else {
            overlayData = NULL;
        }

        if (progress != 0) {
            mPreviewProgress = progress;
        }

        if (isFinished) {
            mPreviewProgressListener->OnStop(mVideoEditor);
        } else if (error != 0) {
            mPreviewProgressListener->OnError(mVideoEditor, error);
        } else {
            mPreviewProgressListener->OnProgress(mVideoEditor, progress, overlayData);
        }
    }
}

ECode MediaArtistNativeHelper::ReleaseNativeHelper()
{
    ReleaseResources();
    return NOERROR;
}

void MediaArtistNativeHelper::OnAudioGraphExtractProgressUpdate(
    /* [in] */ Int32 progress,
    /* [in] */ Boolean isVideo)
{
    if ((mExtractAudioWaveformProgressListener != NULL) && (progress > 0)) {
        mExtractAudioWaveformProgressListener->OnProgress(progress);
    }
}

ECode MediaArtistNativeHelper::GetEffectSettings(
    /* [in] */ IEffectColor* effects,
    /* [out] */ EffectSettings** result)
{
    AutoPtr<EffectSettings> effectSettings = new EffectSettings();
    effects->GetStartTime((Int64*)&effectSettings->startTime);
    effects->GetDuration((Int64*)&effectSettings->duration);
    effectSettings->videoEffectType = GetEffectColorType(effects);
    effectSettings->audioEffectType = 0;
    effectSettings->startPercent = 0;
    effectSettings->durationPercent = 0;
    effectSettings->framingFile = NULL;
    effectSettings->topLeftX = 0;
    effectSettings->topLeftY = 0;
    effectSettings->framingResize = FALSE;
    effectSettings->text = NULL;
    effectSettings->textRenderingData = NULL;
    effectSettings->textBufferWidth = 0;
    effectSettings->textBufferHeight = 0;

    Int32 type;
    effects->GetType(&type);
    if (type == IEffectColor::TYPE_FIFTIES) {
        effectSettings->fiftiesFrameRate = 15;
    }
    else {
        effectSettings->fiftiesFrameRate = 0;
    }

    if ((effectSettings->videoEffectType == IMediaArtistNativeHelperVideoEffect::COLORRGB16)
            || (effectSettings->videoEffectType == IMediaArtistNativeHelperVideoEffect::GRADIENT)) {
        effects->GetColor(&effectSettings->rgb16InputColor);
    }

    effectSettings->alphaBlendingStartPercent = 0;
    effectSettings->alphaBlendingMiddlePercent = 0;
    effectSettings->alphaBlendingEndPercent = 0;
    effectSettings->alphaBlendingFadeInTimePercent = 0;
    effectSettings->alphaBlendingFadeOutTimePercent = 0;
    *result = effectSettings;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode MediaArtistNativeHelper::GetOverlaySettings(
    /* [in] */ IOverlayFrame* overlay,
    /* [out] */ EffectSettings** result)
{
    AutoPtr<EffectSettings> effectSettings = new EffectSettings();
    AutoPtr<IBitmap> bitmap;

    overlay->GetStartTime((Int64*)&effectSettings->startTime);
    overlay->GetDuration((Int64*)&effectSettings->duration);
    effectSettings->videoEffectType = IMediaArtistNativeHelperVideoEffect::FRAMING;
    effectSettings->audioEffectType = 0;
    effectSettings->startPercent = 0;
    effectSettings->durationPercent = 0;
    effectSettings->framingFile = NULL;

    overlay->GetBitmap((IBitmap**)&bitmap);
    if (bitmap != NULL) {
        ((COverlayFrame*)overlay)->GetFilename(&effectSettings->framingFile);

        if (effectSettings->framingFile == NULL) {
            //try {
                String tmpStr;
                ((COverlayFrame*)overlay)->Save(mProjectPath, &tmpStr);
            //} catch (IOException e) {
            //    Log.e(TAG, "getOverlaySettings : File not found");
            //}
            ((COverlayFrame*)overlay)->GetFilename(&effectSettings->framingFile);
        }
        Elastos::Droid::Graphics::BitmapConfig config;
        bitmap->GetConfig(&config);
        if (config == Elastos::Droid::Graphics::BitmapConfig_ARGB_8888)
            effectSettings->bitmapType = 6;
        else if (config == Elastos::Droid::Graphics::BitmapConfig_ARGB_4444)
            effectSettings->bitmapType = 5;
        else if (config == Elastos::Droid::Graphics::BitmapConfig_RGB_565)
            effectSettings->bitmapType = 4;
        else if (config == Elastos::Droid::Graphics::BitmapConfig_ALPHA_8)
            //throw new RuntimeException("Bitmap config not supported");

        bitmap->GetWidth(&effectSettings->width);
        bitmap->GetHeight(&effectSettings->height);
        effectSettings->framingBuffer = ArrayOf<Int32>::Alloc(effectSettings->width);
        Int32 tmp = 0;
        Int16 maxAlpha = 0;
        Int16 minAlpha = (Int16)0xFF;
        Int16 alpha = 0;
        while (tmp < effectSettings->height) {
            bitmap->GetPixels(*effectSettings->framingBuffer, 0,
                             effectSettings->width, 0, tmp,
                             effectSettings->width, 1);
            for (Int32 i = 0; i < effectSettings->width; i++) {
                alpha = (Int16)(((*effectSettings->framingBuffer)[i] >> 24) & 0xFF);
                if (alpha > maxAlpha) {
                    maxAlpha = alpha;
                }
                if (alpha < minAlpha) {
                    minAlpha = alpha;
                }
            }
            tmp += 1;
        }
        alpha = (Int16)((maxAlpha + minAlpha) / 2);
        alpha = (Int16)((alpha * 100) / 256);
        effectSettings->alphaBlendingEndPercent = alpha;
        effectSettings->alphaBlendingMiddlePercent = alpha;
        effectSettings->alphaBlendingStartPercent = alpha;
        effectSettings->alphaBlendingFadeInTimePercent = 100;
        effectSettings->alphaBlendingFadeOutTimePercent = 100;
        effectSettings->framingBuffer = NULL;

        /*
         * Set the resized RGB file dimensions
         */
        ((COverlayFrame*)overlay)->GetResizedRGBSizeWidth(&effectSettings->width);
        if(effectSettings->width == 0) {
            bitmap->GetWidth(&effectSettings->width);
        }

        ((COverlayFrame*)overlay)->GetResizedRGBSizeHeight(&effectSettings->height);
        if(effectSettings->height == 0) {
            bitmap->GetHeight(&effectSettings->height);
        }

    }

    effectSettings->topLeftX = 0;
    effectSettings->topLeftY = 0;

    effectSettings->framingResize = TRUE;
    effectSettings->text = NULL;
    effectSettings->textRenderingData = NULL;
    effectSettings->textBufferWidth = 0;
    effectSettings->textBufferHeight = 0;
    effectSettings->fiftiesFrameRate = 0;
    effectSettings->rgb16InputColor = 0;
    Int32 mediaItemHeight;
    Int32 aspectRatio;
    AutoPtr<IMediaItem> item;
    overlay->GetMediaItem((IMediaItem**)&item);
    if ( IMediaImageItem::Probe(item) != NULL) {
        String str;
        ((CMediaImageItem*)IMediaImageItem::Probe(item))->GetGeneratedImageClip(&str);
        if (!str.IsNull()) {
            // Ken Burns was applied
            ((CMediaImageItem*)IMediaImageItem::Probe(item))->GetGeneratedClipHeight(&mediaItemHeight);
            Int32 value;
            ((CMediaImageItem*)IMediaImageItem::Probe(item))->GetGeneratedClipWidth(&value);
            GetAspectRatio(value, mediaItemHeight, &aspectRatio);
        }
        else {
            //For image get the scaled height. Aspect ratio would remain the same
            ((CMediaImageItem*)IMediaImageItem::Probe(item))->GetScaledHeight(&mediaItemHeight);
            item->GetAspectRatio(&aspectRatio);
        }
    }
    else {
        item->GetAspectRatio(&aspectRatio);
        item->GetHeight(&mediaItemHeight);
    }
    effectSettings->framingScaledSize = FindVideoResolution(aspectRatio, mediaItemHeight);
    *result = effectSettings;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode MediaArtistNativeHelper::NativeHelperGetAspectRatio(
    /* [out] */ Int32* result)
{
    return mVideoEditor->GetAspectRatio(result);
}

ECode MediaArtistNativeHelper::SetAudioCodec(
    /* [in] */ Int32 codec)
{
    mExportAudioCodec = codec;
    return NOERROR;
}

ECode MediaArtistNativeHelper::SetVideoCodec(
    /* [in] */ Int32 codec)
{
    mExportVideoCodec = codec;
    return NOERROR;
}

ECode MediaArtistNativeHelper::SetAudioflag(
    /* [in] */ Boolean flag)
{
    //check if the file exists.
    AutoPtr<IFile> file;
    CFile::New(mProjectPath + "/" + AUDIO_TRACK_PCM_FILE, (IFile**)&file);
    Boolean b;
    file->Exists(&b);
    if (!b) {
        flag = TRUE;
    }
    mRegenerateAudio = flag;
    return NOERROR;
}

ECode MediaArtistNativeHelper::GetAudioflag(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mRegenerateAudio;
    return NOERROR;
}

ECode MediaArtistNativeHelper::GetClosestVideoFrameRate(
    /* [in] */ Int32 averageFrameRate,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    if (averageFrameRate >= 25) {
        *result = IMediaArtistNativeHelperVideoFrameRate::FR_30_FPS;
    } else if (averageFrameRate >= 20) {
        *result = IMediaArtistNativeHelperVideoFrameRate::FR_25_FPS;
    } else if (averageFrameRate >= 15) {
        *result = IMediaArtistNativeHelperVideoFrameRate::FR_20_FPS;
    } else if (averageFrameRate >= 12) {
        *result = IMediaArtistNativeHelperVideoFrameRate::FR_15_FPS;
    } else if (averageFrameRate >= 10) {
        *result = IMediaArtistNativeHelperVideoFrameRate::FR_12_5_FPS;
    } else if (averageFrameRate >= 7) {
        *result = IMediaArtistNativeHelperVideoFrameRate::FR_10_FPS;
    } else if (averageFrameRate >= 5) {
        *result = IMediaArtistNativeHelperVideoFrameRate::FR_7_5_FPS;
    } else {
        *result = -1;
    }
    return NOERROR;
}

ECode MediaArtistNativeHelper::AdjustEffectsStartTimeAndDuration(
        /* [in] */ EffectSettings* lEffect,
        /* [in] */ Int32 beginCutTime,
        /* [in] */ Int32 endCutTime)
{
    Int32 effectStartTime = 0;
    Int32 effectDuration = 0;

    /**
     * cbct -> clip begin cut time
     * cect -> clip end cut time
     ****************************************
     *  |                                 |
     *  |         cbct        cect        |
     *  | <-1-->   |           |          |
     *  |       <--|-2->       |          |
     *  |          | <---3---> |          |
     *  |          |        <--|-4--->    |
     *  |          |           | <--5-->  |
     *  |      <---|------6----|---->     |
     *  |                                 |
     *  < : effectStart
     *  > : effectStart + effectDuration
     ****************************************
     **/

    /** 1 & 5 */
    /**
     * Effect falls out side the trim duration. In such a case effects shall
     * not be applied.
     */
    if ((lEffect->startTime > endCutTime)
            || ((lEffect->startTime + lEffect->duration) <= beginCutTime)) {

        effectStartTime = 0;
        effectDuration = 0;

        lEffect->startTime = effectStartTime;
        lEffect->duration = effectDuration;
        return NOERROR;
    }

    /** 2 */
    if ((lEffect->startTime < beginCutTime)
            && ((lEffect->startTime + lEffect->duration) > beginCutTime)
            && ((lEffect->startTime + lEffect->duration) <= endCutTime)) {
        effectStartTime = 0;
        effectDuration = lEffect->duration;

        effectDuration -= (beginCutTime - lEffect->startTime);
        lEffect->startTime = effectStartTime;
        lEffect->duration = effectDuration;
        return NOERROR;
    }

    /** 3 */
    if ((lEffect->startTime >= beginCutTime)
            && ((lEffect->startTime + lEffect->duration) <= endCutTime)) {
        effectStartTime = lEffect->startTime - beginCutTime;
        lEffect->startTime = effectStartTime;
        lEffect->duration = lEffect->duration;
        return NOERROR;
    }

    /** 4 */
    if ((lEffect->startTime >= beginCutTime)
            && ((lEffect->startTime + lEffect->duration) > endCutTime)) {
        effectStartTime = lEffect->startTime - beginCutTime;
        effectDuration = endCutTime - lEffect->startTime;
        lEffect->startTime = effectStartTime;
        lEffect->duration = effectDuration;
        return NOERROR;
    }

    /** 6 */
    if ((lEffect->startTime < beginCutTime)
            && ((lEffect->startTime + lEffect->duration) > endCutTime)) {
        effectStartTime = 0;
        effectDuration = endCutTime - beginCutTime;
        lEffect->startTime = effectStartTime;
        lEffect->duration = effectDuration;
        return NOERROR;
    }

}

ECode MediaArtistNativeHelper::GenerateClip(
        /* [in] */ EditSettings* editSettings,
        /* [out] */ Int32* result)
{
    //try {
        *result = NativeGenerateClip(editSettings);
    //} catch (IllegalArgumentException ex) {
    //    Log.e(TAG, "Illegal Argument exception in load settings");
    //    return -1;
    //} catch (IllegalStateException ex) {
    //    Log.e(TAG, "Illegal state exception in load settings");
    //    return -1;
    //} catch (RuntimeException ex) {
    //    Log.e(TAG, "Runtime exception in load settings");
    //    return -1;
    //}
    return NOERROR;
}

ECode MediaArtistNativeHelper::InitClipSettings(
    /* [in] */ ClipSettings* lclipSettings)
{
    lclipSettings->clipPath = NULL;
    lclipSettings->clipDecodedPath = NULL;
    lclipSettings->clipOriginalPath = NULL;
    lclipSettings->fileType = 0;
    lclipSettings->endCutTime = 0;
    lclipSettings->beginCutTime = 0;
    lclipSettings->beginCutPercent = 0;
    lclipSettings->endCutPercent = 0;
    lclipSettings->panZoomEnabled = FALSE;
    lclipSettings->panZoomPercentStart = 0;
    lclipSettings->panZoomTopLeftXStart = 0;
    lclipSettings->panZoomTopLeftYStart = 0;
    lclipSettings->panZoomPercentEnd = 0;
    lclipSettings->panZoomTopLeftXEnd = 0;
    lclipSettings->panZoomTopLeftYEnd = 0;
    lclipSettings->mediaRendering = 0;
    lclipSettings->rotationDegree = 0;
    return NOERROR;
}

ECode MediaArtistNativeHelper::GenerateEffectClip(
        /* [in] */ IMediaItem* lMediaItem,
        /* [in] */ ClipSettings* lclipSettings,
        /* [in] */ EditSettings* e,
        /* [in] */ const String& uniqueId,
        /* [in] */ Int32 clipNo,
        /* [out] */ String* result)
{
    Int32 err = 0;
    AutoPtr<EditSettings> editSettings;
    String EffectClipPath;
    Int32 outVideoProfile = 0;
    Int32 outVideoLevel = 0;
    editSettings = new EditSettings();

    editSettings->clipSettingsArray = ArrayOf<ClipSettings*>::Alloc(1);
    editSettings->clipSettingsArray->Set(0, lclipSettings);

    editSettings->backgroundMusicSettings = NULL;
    editSettings->transitionSettingsArray = NULL;
    editSettings->effectSettingsArray = e->effectSettingsArray;

    String id;
    lMediaItem->GetId(&id);
    EffectClipPath = mProjectPath + "/" + "ClipEffectIntermediate" + "_" + id + uniqueId + ".3gp";

    AutoPtr<IFile> tmpFile;
    CFile::New(EffectClipPath, (IFile**)&tmpFile);
    Boolean b;
    tmpFile->Exists(&b);
    if (b)
    {
        tmpFile->Delete(&b);
    }

    AutoPtr<IVideoEditorProfileHelper> helper;
    CVideoEditorProfileHelper::AcquireSingleton((IVideoEditorProfileHelper**)&helper);
    helper->GetExportProfile(IMediaArtistNativeHelperVideoFormat::H264, &outVideoProfile);
    helper->GetExportLevel(IMediaArtistNativeHelperVideoFormat::H264, &outVideoLevel);
    editSettings->videoProfile = outVideoProfile;
    editSettings->videoLevel = outVideoLevel;

    if (IMediaVideoItem::Probe(lMediaItem) != NULL) {
        AutoPtr<IMediaVideoItem> m = (IMediaVideoItem*)IMediaVideoItem::Probe(lMediaItem);

        editSettings->audioFormat = IMediaArtistNativeHelperAudioFormat::AAC;
        editSettings->audioChannels = 2;
        editSettings->audioBitrate = IMediaArtistNativeHelperBitrate::BR_64_KBPS;
        editSettings->audioSamplingFreq = IMediaArtistNativeHelperAudioSamplingFrequency::FREQ_32000;

        editSettings->videoFormat = IMediaArtistNativeHelperVideoFormat::H264;
        editSettings->videoFrameRate = IMediaArtistNativeHelperVideoFrameRate::FR_30_FPS;
        Int32 ratio, height;
        mVideoEditor->GetAspectRatio(&ratio);
        m->GetHeight(&height);
        editSettings->videoFrameSize = FindVideoResolution(ratio, height);
        editSettings->videoBitrate = FindVideoBitrate(editSettings->videoFrameSize);
    }
    else {
        AutoPtr<IMediaImageItem> m = (IMediaImageItem*)IMediaImageItem::Probe(lMediaItem);
        editSettings->audioBitrate = IMediaArtistNativeHelperBitrate::BR_64_KBPS;
        editSettings->audioChannels = 2;
        editSettings->audioFormat = IMediaArtistNativeHelperAudioFormat::AAC;
        editSettings->audioSamplingFreq = IMediaArtistNativeHelperAudioSamplingFrequency::FREQ_32000;

        editSettings->videoFormat = IMediaArtistNativeHelperVideoFormat::H264;
        editSettings->videoFrameRate = IMediaArtistNativeHelperVideoFrameRate::FR_30_FPS;
        Int32 ratio, height;
        mVideoEditor->GetAspectRatio(&ratio);
        m->GetScaledHeight(&height);
        editSettings->videoFrameSize = FindVideoResolution(ratio, height);
        editSettings->videoBitrate = FindVideoBitrate(editSettings->videoFrameSize);
    }

    editSettings->outputFile = EffectClipPath;

    if (clipNo == 1) {
        mProcessingState  = PROCESSING_INTERMEDIATE1;
    }
    else if (clipNo == 2) {
        mProcessingState  = PROCESSING_INTERMEDIATE2;
    }
    mProcessingObject = lMediaItem;
    GenerateClip(editSettings, &err);
    mProcessingState  = PROCESSING_NONE;

    if (err == 0) {
        lclipSettings->clipPath = EffectClipPath;
        lclipSettings->fileType = IMediaArtistNativeHelperFileType::THREE_GPP;
        *result = EffectClipPath;
        return NOERROR;
    } else {
        // throw new RuntimeException("preview generation cannot be completed");
        return E_RUNTIME_EXCEPTION;
    }
}

ECode MediaArtistNativeHelper::GenerateKenBurnsClip(
        /* [in] */ EditSettings* e,
        /* [in] */ IMediaImageItem* m,
        /* [out] */ String* result)
{
    String output;
    Int32 err = 0;
    Int32 outVideoProfile = 0;
    Int32 outVideoLevel = 0;

    e->backgroundMusicSettings = NULL;
    e->transitionSettingsArray = NULL;
    e->effectSettingsArray = NULL;
    String id;
    m->GetId(&id);
    output = mProjectPath + "/" + "ImageClip-" + id + ".3gp";


    AutoPtr<IFile> tmpFile;
    CFile::New(output, (IFile**)&tmpFile);
    Boolean b;
    tmpFile->Exists(&b);
    if (b)
    {
        tmpFile->Delete(&b);
    }

    AutoPtr<IVideoEditorProfileHelper> helper;
    CVideoEditorProfileHelper::AcquireSingleton((IVideoEditorProfileHelper**)&helper);
    helper->GetExportProfile(IMediaArtistNativeHelperVideoFormat::H264, &outVideoProfile);
    helper->GetExportLevel(IMediaArtistNativeHelperVideoFormat::H264, &outVideoLevel);
    e->videoProfile = outVideoProfile;
    e->videoLevel = outVideoLevel;

    e->outputFile = output;
    e->audioBitrate = IMediaArtistNativeHelperBitrate::BR_64_KBPS;
    e->audioChannels = 2;
    e->audioFormat = IMediaArtistNativeHelperAudioFormat::AAC;
    e->audioSamplingFreq = IMediaArtistNativeHelperAudioSamplingFrequency::FREQ_32000;

    e->videoFormat = IMediaArtistNativeHelperVideoFormat::H264;
    e->videoFrameRate = IMediaArtistNativeHelperVideoFrameRate::FR_30_FPS;
    Int32 ratio, height;
    mVideoEditor->GetAspectRatio(&ratio);
    m->GetScaledHeight(&height);
    e->videoFrameSize = FindVideoResolution(ratio, height);
    e->videoBitrate = FindVideoBitrate(e->videoFrameSize);

    mProcessingState  = PROCESSING_KENBURNS;
    mProcessingObject = m;
    GenerateClip(e, &err);
    // Reset the processing state and check for errors
    mProcessingState  = PROCESSING_NONE;
    if (err != 0) {
        // throw new RuntimeException("preview generation cannot be completed");
        return E_RUNTIME_EXCEPTION;
    }
    *result = output;
    return NOERROR;
}

Int32 MediaArtistNativeHelper::GetTransitionResolution(
    /* [in] */ IMediaItem* m1,
    /* [in] */ IMediaItem* m2)
{
    Int32 clip1Height = 0;
    Int32 clip2Height = 0;
    Int32 videoSize = 0;

    Int32 ratio;
    mVideoEditor->GetAspectRatio(&ratio);
    if (m1 != NULL && m2 != NULL) {
        if (IMediaVideoItem::Probe(m1) != NULL) {
            m1->GetHeight(&clip1Height);
        }
        else if (IMediaImageItem::Probe(m1) != NULL) {
            ((IMediaImageItem*)IMediaImageItem::Probe(m1))->GetScaledHeight(&clip1Height);
        }
        if (IMediaVideoItem::Probe(m2) != NULL) {
            m2->GetHeight(&clip2Height);
        }
        else if (IMediaImageItem::Probe(m2) != NULL ) {
            ((IMediaImageItem*)IMediaImageItem::Probe(m2))->GetScaledHeight(&clip2Height);
        }
        if (clip1Height > clip2Height) {
            videoSize = FindVideoResolution(ratio, clip1Height);
        }
        else {
            videoSize = FindVideoResolution(ratio, clip2Height);
        }
    }
    else if (m1 == NULL && m2 != NULL) {
        if (IMediaVideoItem::Probe(m2) != NULL) {
            m2->GetHeight(&clip2Height);
        } else if (IMediaImageItem::Probe(m2) != NULL) {
            ((IMediaImageItem*)IMediaImageItem::Probe(m2))->GetScaledHeight(&clip2Height);
        }
        videoSize = FindVideoResolution(ratio, clip2Height);
    }
    else if (m1 != NULL && m2 == NULL) {
        if (IMediaVideoItem::Probe(m1) != NULL) {
            m1->GetHeight(&clip1Height);
        }
        else if (IMediaImageItem::Probe(m1) != NULL) {
            ((IMediaImageItem*)IMediaImageItem::Probe(m1))->GetScaledHeight(&clip1Height);
        }
        videoSize = FindVideoResolution(ratio, clip1Height);
    }
    return videoSize;
}

ECode MediaArtistNativeHelper::GenerateTransitionClip(
    /* [in] */ EditSettings* e,
    /* [in] */ const String& uniqueId,
    /* [in] */ IMediaItem* m1,
    /* [in] */ IMediaItem* m2,
    /* [in] */ ITransition* t,
    /* [out] */ String* result)
{
    String outputFilename;
    Int32 err = 0;
    Int32 outVideoProfile = 0;
    Int32 outVideoLevel = 0;
    outputFilename = mProjectPath + "/" + uniqueId + ".3gp";

    AutoPtr<IVideoEditorProfileHelper> helper;
    CVideoEditorProfileHelper::AcquireSingleton((IVideoEditorProfileHelper**)&helper);
    helper->GetExportProfile(IMediaArtistNativeHelperVideoFormat::H264, &outVideoProfile);
    helper->GetExportLevel(IMediaArtistNativeHelperVideoFormat::H264, &outVideoLevel);
    e->videoProfile = outVideoProfile;
    e->videoLevel = outVideoLevel;

    e->outputFile = outputFilename;
    e->audioBitrate = IMediaArtistNativeHelperBitrate::BR_64_KBPS;
    e->audioChannels = 2;
    e->audioFormat = IMediaArtistNativeHelperAudioFormat::AAC;
    e->audioSamplingFreq = IMediaArtistNativeHelperAudioSamplingFrequency::FREQ_32000;

    e->videoFormat = IMediaArtistNativeHelperVideoFormat::H264;
    e->videoFrameRate = IMediaArtistNativeHelperVideoFrameRate::FR_30_FPS;
    e->videoFrameSize = GetTransitionResolution(m1, m2);
    e->videoBitrate = FindVideoBitrate(e->videoFrameSize);

    AutoPtr<IFile> file;
    CFile::New(outputFilename, (IFile**)&file);
    Boolean b;
    file->Exists(&b);
    if (b)
    {
        file->Delete(&b);
    }

    mProcessingState  = PROCESSING_INTERMEDIATE3;
    mProcessingObject = t;
    GenerateClip(e,&err);
    // Reset the processing state and check for errors
    mProcessingState  = PROCESSING_NONE;
    if (err != 0) {
        // throw new RuntimeException("preview generation cannot be completed");
        return E_RUNTIME_EXCEPTION;
    }
    *result = outputFilename;
    return NOERROR;
}

Int32 MediaArtistNativeHelper::PopulateEffects(
    /* [in] */ IMediaItem* m,
    /* [in] */ ArrayOf<EffectSettings*>* effectSettings,
    /* [in] */ Int32 i,
    /* [in] */ Int32 beginCutTime,
    /* [in] */ Int32 endCutTime,
    /* [in] */ Int32 storyBoardTime)
{
    AutoPtr<ITransition> bTransition;
    m->GetBeginTransition((ITransition**)&bTransition);
    AutoPtr<ITransition> eTransition;
    m->GetEndTransition((ITransition**)&eTransition);
    Int64 bDuration = 0;
    Int64 eDuration = 0;
    if(bTransition != NULL) {
        bTransition->GetDuration(&bDuration);
    }
    if(eTransition != NULL) {
        eTransition->GetDuration(&eDuration);
    }
    if(bTransition != NULL && bDuration > 0 && eTransition != NULL && eDuration > 0) {
        beginCutTime += bDuration;
        endCutTime -= eDuration;
    } else if (bTransition == NULL && eTransition != NULL && eDuration > 0) {
        endCutTime -= eDuration;
    } else if (bTransition == NULL && eTransition != NULL && bDuration > 0) {
        beginCutTime += bDuration;
    }

    AutoPtr<ArrayOf<IEffect*> > effects;
    m->GetAllEffects((ArrayOf<IEffect*>**)&effects);
    AutoPtr<ArrayOf<IOverlay*> > overlays;
    m->GetAllOverlays((ArrayOf<IOverlay*>**)&overlays);

    for(Int32 j = 0; j < overlays->GetLength(); j++) {
        AutoPtr<IOverlayFrame> overlay = IOverlayFrame::Probe((*overlays)[j]);
        AutoPtr<EffectSettings> settings;
        GetOverlaySettings(overlay, (EffectSettings**)&settings);
        effectSettings->Set(i, settings);
        AdjustEffectsStartTimeAndDuration((*effectSettings)[i], beginCutTime, endCutTime);
        (*effectSettings)[i]->startTime += storyBoardTime;
        i++;
    }

    for(Int32 j = 0; j < effects->GetLength(); j++) {
        AutoPtr<IEffectColor> ec = (IEffectColor*)IEffectColor::Probe((*effects)[j]);
        if(ec != NULL) {
            AutoPtr<EffectSettings> settings;
            GetEffectSettings(ec, (EffectSettings**)&settings);
            effectSettings->Set(i, settings);
            AdjustEffectsStartTimeAndDuration((*effectSettings)[i], beginCutTime, endCutTime);
            (*effectSettings)[i]->startTime += storyBoardTime;
            i++;
        }
    }

    return i;
}

void MediaArtistNativeHelper::AdjustMediaItemBoundary(
    /* [in] */ ClipSettings* clipSettings,
    /* [in] */ Properties* clipProperties,
    /* [in] */ IMediaItem* m)
{
    AutoPtr<ITransition> bTransition;
    m->GetBeginTransition((ITransition**)&bTransition);
    AutoPtr<ITransition> eTransition;
    m->GetEndTransition((ITransition**)&eTransition);
    Int64 bDuration = 0;
    Int64 eDuration = 0;
    if(bTransition != NULL) {
        bTransition->GetDuration(&bDuration);
    }
    if(eTransition != NULL) {
        eTransition->GetDuration(&eDuration);
    }
    if(bTransition != NULL && bDuration > 0 && eTransition != NULL && eDuration > 0) {
        clipSettings->beginCutTime += bDuration;
        clipSettings->endCutTime -= eDuration;
    } else if (bTransition == NULL && eTransition != NULL && eDuration > 0) {
        clipSettings->endCutTime -= eDuration;
    } else if (bTransition == NULL && eTransition != NULL && bDuration > 0) {
        clipSettings->beginCutTime += bDuration;
    }

    clipProperties->duration = clipSettings->endCutTime - clipSettings->beginCutTime;

    if (clipProperties->videoDuration != 0) {
        clipProperties->videoDuration = clipSettings->endCutTime - clipSettings->beginCutTime;
    }

    if (clipProperties->audioDuration != 0) {
        clipProperties->audioDuration = clipSettings->endCutTime - clipSettings->beginCutTime;
    }
}

void MediaArtistNativeHelper::GenerateTransition(
    /* [in] */ ITransition* transition,
    /* [in] */ EditSettings* editSettings,
    /* [in] */ PreviewClipProperties* clipPropertiesArray,
    /* [in] */ Int32 index)
{
    Boolean b;
    (reinterpret_cast<Transition*>((transition)->Probe(EIID_Transition)))->IsGenerated(&b);
    if (!b) {
        (reinterpret_cast<Transition*>((transition)->Probe(EIID_Transition)))->Generate();
    }
    AutoPtr<ClipSettings> cs = new ClipSettings();
    editSettings->clipSettingsArray->Set(index, cs);
    String fileName;
    (reinterpret_cast<Transition*>((transition)->Probe(EIID_Transition)))->GetFilename(&fileName);
    cs->clipPath = fileName;
    cs->fileType = IMediaArtistNativeHelperFileType::THREE_GPP;
    cs->beginCutTime = 0;
    Int64 duration;
    transition->GetDuration(&duration);
    cs->endCutTime = (Int32)duration;
    cs->mediaRendering = IMediaArtistNativeHelperMediaRendering::BLACK_BORDERS;

    //try {
        AutoPtr<Properties> properties = GetMediaProperties(fileName);
        clipPropertiesArray->clipProperties->Set(index, properties);

    //} catch (Exception e) {
    //    throw new IllegalArgumentException("Unsupported file or file not found");
    //}

    properties->Id = String(NULL);
    properties->audioVolumeValue = 100;
    properties->duration = (Int32)duration;
    if (properties->videoDuration != 0) {
        properties->videoDuration = (Int32)duration;
    }

    if (properties->audioDuration != 0) {
        properties->audioDuration = (Int32)duration;
    }
}

void MediaArtistNativeHelper::AdjustVolume(
    /* [in] */ IMediaItem* m,
    /* [in] */ PreviewClipProperties* clipProperties,
    /* [in] */ Int32 index)
{
    AutoPtr<IMediaVideoItem> item = (IMediaVideoItem*)IMediaVideoItem::Probe(m);
    AutoPtr<IMediaImageItem> aItem = (IMediaImageItem*)IMediaImageItem::Probe(m);
    if (item != NULL) {
        Boolean videoMuted;
        item->IsMuted(&videoMuted);
        if (videoMuted == FALSE) {
            Int32 vol;
            item->GetVolume(&vol);
            (*mClipProperties->clipProperties)[index]->audioVolumeValue = vol;
        }
        else {
            (*mClipProperties->clipProperties)[index]->audioVolumeValue = 0;
        }
    }
    else if (aItem != NULL) {
        (*mClipProperties->clipProperties)[index]->audioVolumeValue = 0;
    }
}

void MediaArtistNativeHelper::CheckOddSizeImage(
    /* [in] */ IMediaItem* m,
    /* [in] */ PreviewClipProperties* clipProperties,
    /* [in] */ Int32 index)
{
    AutoPtr<IMediaVideoItem> item = (IMediaVideoItem*)IMediaVideoItem::Probe(m);
    if (item != NULL) {
        Int32 width = (*mClipProperties->clipProperties)[index]->width;
        Int32 height = (*mClipProperties->clipProperties)[index]->height;

        if ((width % 2) != 0) {
            width -= 1;
        }
        if ((height % 2) != 0) {
            height -= 1;
        }
        (*mClipProperties->clipProperties)[index]->width = width;
        (*mClipProperties->clipProperties)[index]->height = height;
    }
}

Int32 MediaArtistNativeHelper::PopulateMediaItemProperties(
    /* [in] */ IMediaItem* m,
    /* [in] */ Int32 index,
    /* [in] */ Int32 maxHeight)
{
    AutoPtr<ClipSettings> cs = new ClipSettings();
    mPreviewEditSettings->clipSettingsArray->Set(index, cs);
    AutoPtr<IMediaVideoItem> item = (IMediaVideoItem*)IMediaVideoItem::Probe(m);
    AutoPtr<IMediaImageItem> iItem = (IMediaImageItem*)IMediaImageItem::Probe(m);
    cs = NULL;
    if (item != NULL) {
        ((CMediaVideoItem*)item.Get())->GetVideoClipProperties((ClipSettings**)&cs);
        mPreviewEditSettings->clipSettingsArray->Set(index, cs);
        Int32 height;
        item->GetHeight(&height);
        if (height > maxHeight) {
            maxHeight = height;
        }
    }
    else if (iItem != NULL) {
        ((CMediaImageItem*)iItem.Get())->GetImageClipProperties((ClipSettings**)&cs);
        mPreviewEditSettings->clipSettingsArray->Set(index, cs);
        Int32 height;
        iItem->GetHeight(&height);
        if (height > maxHeight) {
            maxHeight = height;
        }
    }
    /** + Handle the image files here */
    if ((*mPreviewEditSettings->clipSettingsArray)[index]->fileType == IMediaArtistNativeHelperFileType::JPG) {
        String fileName;
        ((CMediaImageItem*)iItem.Get())->GetDecodedImageFileName(&fileName);
        (*mPreviewEditSettings->clipSettingsArray)[index]->clipDecodedPath = fileName;

        (*mPreviewEditSettings->clipSettingsArray)[index]->clipOriginalPath =
                     (*mPreviewEditSettings->clipSettingsArray)[index]->clipPath;
    }
    return maxHeight;
}

void MediaArtistNativeHelper::PopulateBackgroundMusicProperties(
    /* [in] */ ArrayOf<IAudioTrack2*>* mediaBGMList)
{
    if(mediaBGMList->GetLength() == 1) {
        mAudioTrack = (*mediaBGMList)[0];
    } else {
        mAudioTrack = NULL;
    }

    if (mAudioTrack != NULL) {
        mAudioSettings = new AudioSettings();
        AutoPtr<Properties> mAudioProperties = new Properties();
        mAudioSettings->pFile = NULL;
        mAudioTrack->GetId(&mAudioSettings->Id);
        //try {
            String filename;
            mAudioTrack->GetFilename(&filename);
            mAudioProperties = GetMediaProperties(filename);
        //} catch (Exception e) {
        //   throw new IllegalArgumentException("Unsupported file or file not found");
        //}
        mAudioSettings->bRemoveOriginal = FALSE;
        mAudioSettings->channels = mAudioProperties->audioChannels;
        mAudioSettings->Fs = mAudioProperties->audioSamplingFrequency;
        mAudioTrack->IsLooping(&mAudioSettings->loop);
        mAudioSettings->ExtendedFs = 0;
        mAudioTrack->GetFilename(&mAudioSettings->pFile);
        mAudioTrack->GetStartTime(&mAudioSettings->startMs);
        mAudioTrack->GetBoundaryBeginTime(&mAudioSettings->beginCutTime);
        mAudioTrack->GetBoundaryEndTime(&mAudioSettings->endCutTime);
        Boolean b;
        mAudioTrack->IsMuted(&b);
        if (b) {
            mAudioSettings->volume = 0;
        }
        else {
            mAudioTrack->GetVolume(&mAudioSettings->volume);
        }
        mAudioSettings->fileType = mAudioProperties->fileType;
        mAudioTrack->GetDuckedTrackVolume(&mAudioSettings->ducking_lowVolume);
        mAudioTrack->GetDuckingThreshhold(&mAudioSettings->ducking_threshold);
        mAudioTrack->IsDuckingEnabled(&mAudioSettings->bInDucking_enable);
        mAudioTrackPCMFilePath = mProjectPath + "/" + AUDIO_TRACK_PCM_FILE;
        mAudioSettings->pcmFilePath = mAudioTrackPCMFilePath;

        mPreviewEditSettings->backgroundMusicSettings = new BackgroundMusicSettings();
        mPreviewEditSettings->backgroundMusicSettings->file = mAudioTrackPCMFilePath;
        mPreviewEditSettings->backgroundMusicSettings->fileType = mAudioProperties->fileType;
        mAudioTrack->GetStartTime(&mPreviewEditSettings->backgroundMusicSettings->insertionTime);
        mAudioTrack->GetVolume(&mPreviewEditSettings->backgroundMusicSettings->volumePercent);
        mAudioTrack->GetBoundaryBeginTime(&mPreviewEditSettings->backgroundMusicSettings->beginLoop);
        mAudioTrack->GetBoundaryEndTime(&mPreviewEditSettings->backgroundMusicSettings->endLoop);
        mAudioTrack->IsDuckingEnabled(&mPreviewEditSettings->backgroundMusicSettings->enableDucking);
        mAudioTrack->GetDuckingThreshhold(&mPreviewEditSettings->backgroundMusicSettings->duckingThreshold);
        mAudioTrack->GetDuckedTrackVolume(&mPreviewEditSettings->backgroundMusicSettings->lowVolume);
        mAudioTrack->IsLooping(&mPreviewEditSettings->backgroundMusicSettings->isLooping);
        mPreviewEditSettings->primaryTrackVolume = 100;
        mProcessingState  = PROCESSING_AUDIO_PCM;
        mProcessingObject = mAudioTrack;
    }
    else {
        mAudioSettings = NULL;
        mPreviewEditSettings->backgroundMusicSettings = NULL;
        mAudioTrackPCMFilePath = NULL;
    }
}

Int32 MediaArtistNativeHelper::GetTotalEffects(
    /* [in] */ ArrayOf<IMediaItem*>* mediaItemsList)
{
    Int32 totalEffects = 0;
    for(Int32 i = 0; i < mediaItemsList->GetLength(); i++) {
        AutoPtr<IMediaItem> t = (*mediaItemsList)[i];
        AutoPtr<ArrayOf<IEffect*> > effects;
        t->GetAllEffects((ArrayOf<IEffect*>**)&effects);
        totalEffects += effects->GetLength();

        AutoPtr<ArrayOf<IOverlay*> > overlays;
        t->GetAllOverlays((ArrayOf<IOverlay*>**)&overlays);
        totalEffects += overlays->GetLength();

        for(Int32 j = 0; j < effects->GetLength(); j++) {
            AutoPtr<IEffectKenBurns> e = IEffectKenBurns::Probe((*effects)[j]);
            if(e != NULL) {
                totalEffects--;
            }
        }
    }

    return totalEffects;
}

ECode MediaArtistNativeHelper::PreviewStoryBoard(
    /* [in] */ ArrayOf<IMediaItem*>* mediaItemsList,
    /* [in] */ ArrayOf<ITransition*>* mediaTransitionList,
    /* [in] */ ArrayOf<IAudioTrack2*>* mediaBGMList,
    /* [in] */ IVideoEditorMediaProcessingProgressListener* listener)
{
    if (mInvalidatePreviewArray) {
        Int32 previewIndex = 0;
        Int32 totalEffects = 0;
        Int32 storyBoardTime = 0;
        Int32 maxHeight = 0;
        Int32 beginCutTime = 0;
        Int32 endCutTime = 0;
        Int32 effectIndex = 0;
        AutoPtr<ITransition> lTransition;
        AutoPtr<IMediaItem> lMediaItem;
        mPreviewEditSettings = new EditSettings();
        mClipProperties = new PreviewClipProperties();
        mTotalClips = 0;

        mTotalClips = mediaItemsList->GetLength();
        for(Int32 i = 0; i < mediaTransitionList->GetLength(); i++) {
            Int64 val;
            (*mediaTransitionList)[i]->GetDuration(&val);
            if(val > 0) {
                mTotalClips++;
            }
        }

        totalEffects = GetTotalEffects(mediaItemsList);

        mPreviewEditSettings->clipSettingsArray = ArrayOf<ClipSettings*>::Alloc(mTotalClips);
        mPreviewEditSettings->effectSettingsArray = ArrayOf<EffectSettings*>::Alloc(totalEffects);
        mClipProperties->clipProperties = ArrayOf<Properties*>::Alloc(mTotalClips);

        /** record the call back progress listener */
        mMediaProcessingProgressListener = listener;
        mProgressToApp = 0;

        if (mediaItemsList->GetLength() > 0) {
            for (Int32 i = 0; i < mediaItemsList->GetLength(); i++) {
                /* Get the Media Item from the list */
                lMediaItem = (*mediaItemsList)[i];
                AutoPtr<IMediaVideoItem> mvi = IMediaVideoItem::Probe(lMediaItem);
                AutoPtr<IMediaImageItem> mii = IMediaImageItem::Probe(lMediaItem);
                if (mvi != NULL) {
                    mvi->GetBoundaryBeginTime((Int64*)&beginCutTime);
                    mvi->GetBoundaryEndTime((Int64*)&endCutTime);
                } else if (mii != NULL) {
                    beginCutTime = 0;
                    mii->GetTimelineDuration((Int64*)&endCutTime);
                }
                /* Get the transition associated with Media Item */
                lMediaItem->GetBeginTransition((ITransition**)&lTransition);
                if (lTransition != NULL) {
                    Int64 duration;
                    lTransition->GetDuration(&duration);
                    if(duration > 0) {
                        /* generate transition clip */
                        GenerateTransition(lTransition, mPreviewEditSettings,
                                       mClipProperties, previewIndex);
                        storyBoardTime += (*mClipProperties->clipProperties)[previewIndex]->duration;
                        previewIndex++;
                    }
                }
                /* Populate media item properties */
                maxHeight = PopulateMediaItemProperties(lMediaItem, previewIndex, maxHeight);
                /* Get the clip properties of the media item. */
                if (mii != NULL) {
                    Int32 tmpCnt = 0;
                    Boolean bEffectKbPresent = FALSE;
                    AutoPtr<ArrayOf<IEffect*> > effectList;
                    lMediaItem->GetAllEffects((ArrayOf<IEffect*>**)&effectList);
                    /**
                     * Check if Ken Burns effect is present
                     */
                    while (tmpCnt < effectList->GetLength()) {
                        AutoPtr<IEffectKenBurns> ekb = IEffectKenBurns::Probe((*effectList)[tmpCnt]);
                        if (ekb != NULL) {
                            bEffectKbPresent = TRUE;
                            break;
                        }
                        tmpCnt++;
                    }

                    if (bEffectKbPresent) {
                        //try {
                            String clip;
                            ((CMediaImageItem*)mii.Get())->GetGeneratedImageClip(&clip);
                            if(clip != NULL) {
                                mClipProperties->clipProperties->Set(previewIndex, GetMediaProperties(clip));
                            }
                            else {
                                String filename;
                                ((CMediaImageItem*)mii.Get())->GetScaledImageFileName(&filename);
                                mClipProperties->clipProperties->Set(previewIndex, GetMediaProperties(filename));
                                mii->GetScaledWidth(&(*mClipProperties->clipProperties)[previewIndex]->width);
                                mii->GetScaledHeight(&(*mClipProperties->clipProperties)[previewIndex]->height);
                            }
                        //} catch (Exception e) {
                        //   throw new IllegalArgumentException("Unsupported file or file not found");
                        //}
                    } else {
                        //try {
                            String filename;
                            ((CMediaImageItem*)mii.Get())->GetScaledImageFileName(&filename);
                            mClipProperties->clipProperties->Set(previewIndex, GetMediaProperties(filename));
                        //} catch (Exception e) {
                        //    throw new IllegalArgumentException("Unsupported file or file not found");
                        //}
                        mii->GetScaledWidth(&(*mClipProperties->clipProperties)[previewIndex]->width);
                        mii->GetScaledHeight(&(*mClipProperties->clipProperties)[previewIndex]->height);
                    }
                } else {
                    //try {
                        String filename;
                        lMediaItem->GetFilename(&filename);
                        mClipProperties->clipProperties->Set(previewIndex, GetMediaProperties(filename));
                    //  } catch (Exception e) {
                    //      throw new IllegalArgumentException("Unsupported file or file not found");
                    //  }
                }
                lMediaItem->GetId(&(*mClipProperties->clipProperties)[previewIndex]->Id);
                CheckOddSizeImage(lMediaItem, mClipProperties, previewIndex);
                AdjustVolume(lMediaItem, mClipProperties, previewIndex);

                /*
                 * Adjust media item start time and end time w.r.t to begin
                 * and end transitions associated with media item
                 */

                AdjustMediaItemBoundary((*mPreviewEditSettings->clipSettingsArray)[previewIndex],
                        (*mClipProperties->clipProperties)[previewIndex], lMediaItem);

                /*
                 * Get all the effects and overlays for that media item and
                 * adjust start time and duration of effects
                 */

                effectIndex = PopulateEffects(lMediaItem,
                        mPreviewEditSettings->effectSettingsArray, effectIndex, beginCutTime,
                        endCutTime, storyBoardTime);
                storyBoardTime += (*mClipProperties->clipProperties)[previewIndex]->duration;
                previewIndex++;

                /* Check if there is any end transition at last media item */

                if (i == (mediaItemsList->GetLength() - 1)) {
                    lMediaItem->GetEndTransition((ITransition**)&lTransition);
                    if (lTransition != NULL) {
                        Int64 duration;
                        lTransition->GetDuration(&duration);
                        if (duration > 0) {
                            GenerateTransition(lTransition, mPreviewEditSettings, mClipProperties,
                                previewIndex);
                            break;
                        }
                    }
                }
            }

            if (!mErrorFlagSet) {
                Int32 ratio;
                mVideoEditor->GetAspectRatio(&ratio);
                mPreviewEditSettings->videoFrameSize = FindVideoResolution(ratio, maxHeight);
                PopulateBackgroundMusicProperties(mediaBGMList);

                /** call to native populate settings */
                //try {
                    NativePopulateSettings(mPreviewEditSettings, mClipProperties, mAudioSettings);
                //} catch (IllegalArgumentException ex) {
                //    Log.e(TAG, "Illegal argument exception in nativePopulateSettings");
                //    throw ex;
                //} catch (IllegalStateException ex) {
                //    Log.e(TAG, "Illegal state exception in nativePopulateSettings");
                //    throw ex;
                //} catch (RuntimeException ex) {
                //    Log.e(TAG, "Runtime exception in nativePopulateSettings");
                //    throw ex;
                //}
                mInvalidatePreviewArray = FALSE;
                mProcessingState  = PROCESSING_NONE;
            }
        }
        if (mErrorFlagSet) {
            mErrorFlagSet = FALSE;
            // throw new RuntimeException("preview generation cannot be completed");
            return E_RUNTIME_EXCEPTION;
        }
    }

    return NOERROR;
}

ECode MediaArtistNativeHelper::DoPreview(
        /* [in] */ ISurface* surface,
        /* [in] */ Int64 fromMs,
        /* [in] */ Int64 toMs,
        /* [in] */ Boolean loop,
        /* [in] */ Int32 callbackAfterFrameCount,
        /* [in] */ IVideoEditorPreviewProgressListener* listener)
{
    mPreviewProgress = fromMs;
    mIsFirstProgress = TRUE;
    mPreviewProgressListener = listener;

    if (!mInvalidatePreviewArray) {
        //try {
            /** Modify the image files names to rgb image files. */
            for (Int32 clipCnt = 0; clipCnt < mPreviewEditSettings->clipSettingsArray->GetLength();
                clipCnt++) {
                if ((*mPreviewEditSettings->clipSettingsArray)[clipCnt]->fileType == IMediaArtistNativeHelperFileType::JPG) {
                    (*mPreviewEditSettings->clipSettingsArray)[clipCnt]->clipPath =
                        (*mPreviewEditSettings->clipSettingsArray)[clipCnt]->clipDecodedPath;
                }
            }
            NativePopulateSettings(mPreviewEditSettings, mClipProperties, mAudioSettings);
            NativeStartPreview(surface, fromMs, toMs, callbackAfterFrameCount, loop);
        //} catch (IllegalArgumentException ex) {
        //    Log.e(TAG, "Illegal argument exception in nativeStartPreview");
        //    throw ex;
        //} catch (IllegalStateException ex) {
        //    Log.e(TAG, "Illegal state exception in nativeStartPreview");
        //    throw ex;
        //} catch (RuntimeException ex) {
        //    Log.e(TAG, "Runtime exception in nativeStartPreview");
        //    throw ex;
        //}
    } else {
    //    throw new IllegalStateException("generatePreview is in progress");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    return NOERROR;
}

ECode MediaArtistNativeHelper::StopPreview(
        /* [out] */ Int64* result)
{
    *result = NativeStopPreview();
    return NOERROR;
}

ECode MediaArtistNativeHelper::RenderPreviewFrame(
        /* [in] */ ISurface* surface,
        /* [in] */ Int64 time,
        /* [in] */ Int32 surfaceWidth,
        /* [in] */ Int32 surfaceHeight,
        /* [in] */ IVideoEditorOverlayData* overlayData,
        /* [out] */ Int64* result)
{
    if (mInvalidatePreviewArray) {
        // if (Log.isLoggable(TAG, Log.DEBUG)) {
        //     Log.d(TAG, "Call generate preview first");
        // }
        // throw new IllegalStateException("Call generate preview first");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    Int64 timeMs = 0;
    //try {
        for (Int32 clipCnt = 0; clipCnt < mPreviewEditSettings->clipSettingsArray->GetLength();
              clipCnt++) {
            if ((*mPreviewEditSettings->clipSettingsArray)[clipCnt]->fileType == IMediaArtistNativeHelperFileType::JPG) {
                (*mPreviewEditSettings->clipSettingsArray)[clipCnt]->clipPath =
                    (*mPreviewEditSettings->clipSettingsArray)[clipCnt]->clipDecodedPath;
            }
        }

        // Reset the render preview frame params that shall be set by native.
        mRenderPreviewOverlayFile = NULL;
        mRenderPreviewRenderingMode = IMediaArtistNativeHelperMediaRendering::RESIZING;

        NativePopulateSettings(mPreviewEditSettings, mClipProperties, mAudioSettings);

        timeMs = NativeRenderPreviewFrame(surface, time, surfaceWidth, surfaceHeight);

        if (mRenderPreviewOverlayFile != NULL) {
            AutoPtr<IBitmapFactory> factory;
            CBitmapFactory::AcquireSingleton((IBitmapFactory**)&factory);
            AutoPtr<IBitmap> bitmap;
            factory->DecodeFile(mRenderPreviewOverlayFile, (IBitmap**)&bitmap);
            ((CVideoEditorOverlayData*)overlayData)->Set(bitmap, mRenderPreviewRenderingMode);
        }
        else {
            ((CVideoEditorOverlayData*)overlayData)->SetClear();
        }
    //} catch (IllegalArgumentException ex) {
    //    Log.e(TAG, "Illegal Argument exception in nativeRenderPreviewFrame");
    //    throw ex;
    //} catch (IllegalStateException ex) {
    //    Log.e(TAG, "Illegal state exception in nativeRenderPreviewFrame");
    //    throw ex;
    //} catch (RuntimeException ex) {
    //    Log.e(TAG, "Runtime exception in nativeRenderPreviewFrame");
    //    throw ex;
    //}

    *result = timeMs;
    return NOERROR;
}

void MediaArtistNativeHelper::PreviewFrameEditInfo(
    /* [in] */ const String& filename,
    /* [in] */ Int32 renderingMode)
{
    mRenderPreviewOverlayFile = filename;
    mRenderPreviewRenderingMode = renderingMode;
}

ECode MediaArtistNativeHelper::RenderMediaItemPreviewFrame(
        /* [in] */ ISurface* surface,
        /* [in] */ const String& filepath,
        /* [in] */ Int64 time,
        /* [in] */ Int32 framewidth,
        /* [in] */ Int32 frameheight,
        /* [out] */ Int64* result)
{
    //try {
        *result = (Int64)NativeRenderMediaItemPreviewFrame(surface, filepath, framewidth,
                frameheight, 0, 0, time);
    //} catch (IllegalArgumentException ex) {
    //    Log.e(TAG, "Illegal Argument exception in renderMediaItemPreviewFrame");
    //    throw ex;
    //} catch (IllegalStateException ex) {
    //    Log.e(TAG, "Illegal state exception in renderMediaItemPreviewFrame");
    //    throw ex;
    //} catch (RuntimeException ex) {
    //    Log.e(TAG, "Runtime exception in renderMediaItemPreviewFrame");
    //    throw ex;
    //}

    return NOERROR;
}

ECode MediaArtistNativeHelper::SetGeneratePreview(
        /* [in] */ Boolean isRequired)
{
    Boolean semAcquiredDone = FALSE;
    //try {
        Lock();
        semAcquiredDone = TRUE;
        mInvalidatePreviewArray = isRequired;
    //} catch (InterruptedException ex) {
    //    Log.e(TAG, "Runtime exception in renderMediaItemPreviewFrame");
    //} finally {
        if (semAcquiredDone) {
            Unlock();
        }
    //}
    return NOERROR;
}

ECode MediaArtistNativeHelper::GetGeneratePreview(
        /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mInvalidatePreviewArray;
    return NOERROR;
}

ECode MediaArtistNativeHelper::GetAspectRatio(
        /* [in] */ Int32 w,
        /* [in] */ Int32 h,
        /* [out] */ Int32* result)
{
    Double apRatio = (Double)(w) / (Double)(h);
    AutoPtr<IBigDecimal> bd;
    CBigDecimal::New(apRatio, (IBigDecimal**)&bd);
    bd->SetScale(3, IBigDecimal::ROUND_HALF_UP, (IBigDecimal**)&bd);
    bd->DoubleValue(&apRatio);
    *result = IMediaProperties::ASPECT_RATIO_16_9;
    if (apRatio >= 1.7) {
        *result = IMediaProperties::ASPECT_RATIO_16_9;
    } else if (apRatio >= 1.6) {
        *result = IMediaProperties::ASPECT_RATIO_5_3;
    } else if (apRatio >= 1.5) {
        *result = IMediaProperties::ASPECT_RATIO_3_2;
    } else if (apRatio > 1.3) {
        *result = IMediaProperties::ASPECT_RATIO_4_3;
    } else if (apRatio >= 1.2) {
        *result = IMediaProperties::ASPECT_RATIO_11_9;
    }
    return NOERROR;
}

ECode MediaArtistNativeHelper::GetFileType(
        /* [in] */ Int32 fileType,
        /* [out] */ Int32* result)
{
    *result = -1;
    switch (fileType) {
        case IMediaArtistNativeHelperFileType::UNSUPPORTED:
            *result = IMediaProperties::FILE_UNSUPPORTED;
            break;
        case IMediaArtistNativeHelperFileType::THREE_GPP:
            *result = IMediaProperties::FILE_3GP;
            break;
        case IMediaArtistNativeHelperFileType::MP4:
            *result = IMediaProperties::FILE_MP4;
            break;
        case IMediaArtistNativeHelperFileType::JPG:
            *result = IMediaProperties::FILE_JPEG;
            break;
        case IMediaArtistNativeHelperFileType::PNG:
            *result = IMediaProperties::FILE_PNG;
            break;
        case IMediaArtistNativeHelperFileType::MP3:
            *result = IMediaProperties::FILE_MP3;
            break;
        case IMediaArtistNativeHelperFileType::M4V:
            *result = IMediaProperties::FILE_M4V;
            break;
        case IMediaArtistNativeHelperFileType::AMR:
            *result = IMediaProperties::FILE_AMR;
            break;

        default:
            *result = -1;
    }
    return NOERROR;
}

ECode MediaArtistNativeHelper::GetVideoCodecType(
        /* [in] */ Int32 codecType,
        /* [out] */ Int32* result)
{
    Int32 retValue = -1;
    switch (codecType) {
        case IMediaArtistNativeHelperVideoFormat::H263:
            retValue = IMediaProperties::VCODEC_H263;
            break;
        case IMediaArtistNativeHelperVideoFormat::H264:
            retValue = IMediaProperties::VCODEC_H264;
            break;
        case IMediaArtistNativeHelperVideoFormat::MPEG4:
            retValue = IMediaProperties::VCODEC_MPEG4;
            break;
        case IMediaArtistNativeHelperVideoFormat::UNSUPPORTED:

        default:
            retValue = -1;
    }
    *result = retValue;
    return NOERROR;
}

ECode MediaArtistNativeHelper::GetAudioCodecType(
        /* [in] */ Int32 codecType,
        /* [out] */ Int32* result)
{
    Int32 retValue = -1;
    switch (codecType) {
        case IMediaArtistNativeHelperAudioFormat::AMR_NB:
            retValue = IMediaProperties::ACODEC_AMRNB;
            break;
        case IMediaArtistNativeHelperAudioFormat::AAC:
            retValue = IMediaProperties::ACODEC_AAC_LC;
            break;
        case IMediaArtistNativeHelperAudioFormat::MP3:
            retValue = IMediaProperties::ACODEC_MP3;
            break;

        default:
            retValue = -1;
    }
    *result = retValue;
    return NOERROR;
}

ECode MediaArtistNativeHelper::GetFrameRate(
        /* [in] */ Int32 fps,
        /* [out] */ Int32* result)
{
    Int32 retValue = -1;
    switch (fps) {
        case IMediaArtistNativeHelperVideoFrameRate::FR_5_FPS:
            retValue = 5;
            break;
        case IMediaArtistNativeHelperVideoFrameRate::FR_7_5_FPS:
            retValue = 8;
            break;
        case IMediaArtistNativeHelperVideoFrameRate::FR_10_FPS:
            retValue = 10;
            break;
        case IMediaArtistNativeHelperVideoFrameRate::FR_12_5_FPS:
            retValue = 13;
            break;
        case IMediaArtistNativeHelperVideoFrameRate::FR_15_FPS:
            retValue = 15;
            break;
        case IMediaArtistNativeHelperVideoFrameRate::FR_20_FPS:
            retValue = 20;
            break;
        case IMediaArtistNativeHelperVideoFrameRate::FR_25_FPS:
            retValue = 25;
            break;
        case IMediaArtistNativeHelperVideoFrameRate::FR_30_FPS:
            retValue = 30;
            break;

        default:
            retValue = -1;
    }
    *result = retValue;
    return NOERROR;
}

ECode MediaArtistNativeHelper::GetMediaItemFileType(
        /* [in] */ Int32 fileType,
        /* [out] */ Int32* result)
{
    Int32 retValue = -1;

    switch (fileType) {
        case IMediaProperties::FILE_UNSUPPORTED:
            retValue = IMediaArtistNativeHelperFileType::UNSUPPORTED;
            break;
        case IMediaProperties::FILE_3GP:
            retValue = IMediaArtistNativeHelperFileType::THREE_GPP;
            break;
        case IMediaProperties::FILE_MP4:
            retValue = IMediaArtistNativeHelperFileType::MP4;
            break;
        case IMediaProperties::FILE_JPEG:
            retValue = IMediaArtistNativeHelperFileType::JPG;
            break;
        case IMediaProperties::FILE_PNG:
            retValue = IMediaArtistNativeHelperFileType::PNG;
            break;
        case IMediaProperties::FILE_M4V:
            retValue = IMediaArtistNativeHelperFileType::M4V;
            break;

        default:
            retValue = -1;
    }
    *result = retValue;
    return NOERROR;
}

ECode MediaArtistNativeHelper::GetMediaItemRenderingMode(
        /* [in] */ Int32 renderingMode,
        /* [out] */ Int32* result)
{
    Int32 retValue = -1;
    switch (renderingMode) {
        case IMediaItem::RENDERING_MODE_BLACK_BORDER:
            retValue = IMediaArtistNativeHelperMediaRendering::BLACK_BORDERS;
            break;
        case IMediaItem::RENDERING_MODE_STRETCH:
            retValue = IMediaArtistNativeHelperMediaRendering::RESIZING;
            break;
        case IMediaItem::RENDERING_MODE_CROPPING:
            retValue = IMediaArtistNativeHelperMediaRendering::CROPPING;
            break;

        default:
            retValue = -1;
    }
    *result = retValue;
    return NOERROR;
}

ECode MediaArtistNativeHelper::GetVideoTransitionBehaviour(
        /* [in] */ Int32 transitionType,
        /* [out] */ Int32* result)
{
    Int32 retValue = -1;
    switch (transitionType) {
        case ITransition::BEHAVIOR_SPEED_UP:
            retValue = IMediaArtistNativeHelperTransitionBehaviour::SPEED_UP;
            break;
        case ITransition::BEHAVIOR_SPEED_DOWN:
            retValue = IMediaArtistNativeHelperTransitionBehaviour::SPEED_DOWN;
            break;
        case ITransition::BEHAVIOR_LINEAR:
            retValue = IMediaArtistNativeHelperTransitionBehaviour::LINEAR;
            break;
        case ITransition::BEHAVIOR_MIDDLE_SLOW:
            retValue = IMediaArtistNativeHelperTransitionBehaviour::SLOW_MIDDLE;
            break;
        case ITransition::BEHAVIOR_MIDDLE_FAST:
            retValue = IMediaArtistNativeHelperTransitionBehaviour::FAST_MIDDLE;
            break;

        default:
            retValue = -1;
    }
    *result = retValue;
    return NOERROR;
}

ECode MediaArtistNativeHelper::GetSlideSettingsDirection(
        /* [in] */ Int32 slideDirection,
        /* [out] */ Int32* result)
{
    Int32 retValue = -1;
    switch (slideDirection) {
        case ITransitionSliding::DIRECTION_RIGHT_OUT_LEFT_IN:
            retValue = IMediaArtistNativeHelperSlideDirection::RIGHT_OUT_LEFT_IN;
            break;
        case ITransitionSliding::DIRECTION_LEFT_OUT_RIGHT_IN:
            retValue = IMediaArtistNativeHelperSlideDirection::LEFT_OUT_RIGTH_IN;
            break;
        case ITransitionSliding::DIRECTION_TOP_OUT_BOTTOM_IN:
            retValue = IMediaArtistNativeHelperSlideDirection::TOP_OUT_BOTTOM_IN;
            break;
        case ITransitionSliding::DIRECTION_BOTTOM_OUT_TOP_IN:
            retValue = IMediaArtistNativeHelperSlideDirection::BOTTOM_OUT_TOP_IN;
            break;

        default:
            retValue = -1;
    }
    *result = retValue;
    return NOERROR;
}

Int32 MediaArtistNativeHelper::GetEffectColorType(
    /* [in] */ IEffectColor* effect)
{
    Int32 retValue = -1;
    Int32 type;
    effect->GetType(&type);
    Int32 color;
    effect->GetColor(&color);
    switch (type) {
        case IEffectColor::TYPE_COLOR:
            if (color == IEffectColor::GREEN) {
                retValue = IMediaArtistNativeHelperVideoEffect::GREEN;
            } else if (color == IEffectColor::PINK) {
                retValue = IMediaArtistNativeHelperVideoEffect::PINK;
            } else if (color == IEffectColor::GRAY) {
                retValue = IMediaArtistNativeHelperVideoEffect::BLACK_AND_WHITE;
            } else {
                retValue = IMediaArtistNativeHelperVideoEffect::COLORRGB16;
            }
            break;
        case IEffectColor::TYPE_GRADIENT:
            retValue = IMediaArtistNativeHelperVideoEffect::GRADIENT;
            break;
        case IEffectColor::TYPE_SEPIA:
            retValue = IMediaArtistNativeHelperVideoEffect::SEPIA;
            break;
        case IEffectColor::TYPE_NEGATIVE:
            retValue = IMediaArtistNativeHelperVideoEffect::NEGATIVE;
            break;
        case IEffectColor::TYPE_FIFTIES:
            retValue = IMediaArtistNativeHelperVideoEffect::FIFTIES;
            break;

        default:
            retValue = -1;
    }
    return retValue;
}

Int32 MediaArtistNativeHelper::FindVideoResolution(
    /* [in] */ Int32 aspectRatio,
    /* [in] */ Int32 height)
{
    Int32 retValue = IMediaArtistNativeHelperVideoFrameSize::SIZE_UNDEFINED;
    AutoPtr<ArrayOf<IPair*> > resolutions;
    AutoPtr<IPair> maxResolution;

    switch (aspectRatio) {
        case IMediaProperties::ASPECT_RATIO_3_2:
            if (height == IMediaProperties::HEIGHT_480)
                retValue = IMediaArtistNativeHelperVideoFrameSize::NTSC;
            else if (height == IMediaProperties::HEIGHT_720)
                retValue = IMediaArtistNativeHelperVideoFrameSize::W720p;
            break;
        case IMediaProperties::ASPECT_RATIO_16_9:
            if (height == IMediaProperties::HEIGHT_480)
                retValue = IMediaArtistNativeHelperVideoFrameSize::WVGA16x9;
            else if (height == IMediaProperties::HEIGHT_720)
                retValue = IMediaArtistNativeHelperVideoFrameSize::V720p;
            else if (height == IMediaProperties::HEIGHT_1080)
                retValue = IMediaArtistNativeHelperVideoFrameSize::V1080p;
            break;
        case IMediaProperties::ASPECT_RATIO_4_3:
            if (height == IMediaProperties::HEIGHT_480)
                retValue = IMediaArtistNativeHelperVideoFrameSize::VGA;
            else if (height == IMediaProperties::HEIGHT_720)
                retValue = IMediaArtistNativeHelperVideoFrameSize::S720p;
            break;
        case IMediaProperties::ASPECT_RATIO_5_3:
            if (height == IMediaProperties::HEIGHT_480)
                retValue = IMediaArtistNativeHelperVideoFrameSize::WVGA;
            break;
        case IMediaProperties::ASPECT_RATIO_11_9:
            if (height == IMediaProperties::HEIGHT_144)
                retValue = IMediaArtistNativeHelperVideoFrameSize::QCIF;
            else if (height == IMediaProperties::HEIGHT_288)
                retValue = IMediaArtistNativeHelperVideoFrameSize::CIF;
            break;
    }
    if (retValue == IMediaArtistNativeHelperVideoFrameSize::SIZE_UNDEFINED) {
        AutoPtr<IMediaPropertiesHelper> helper;
        CMediaPropertiesHelper::AcquireSingleton((IMediaPropertiesHelper**)&helper);
        Int32 ratio;
        mVideoEditor->GetAspectRatio(&ratio);
        helper->GetSupportedResolutions(ratio, (ArrayOf<IPair*>**)&resolutions);
        // Get the highest resolution
        maxResolution = (*resolutions)[resolutions->GetLength() - 1];
        AutoPtr<IInteger32> second;
        maxResolution->GetSecond((IInterface**)&second);
        Int32 val;
        second->GetValue(&val);
        retValue = FindVideoResolution(ratio, val);
    }

    return retValue;
}

Int32 MediaArtistNativeHelper::FindVideoBitrate(
    /* [in] */ Int32 videoFrameSize)
{
    switch (videoFrameSize) {
        case IMediaArtistNativeHelperVideoFrameSize::SQCIF:
        case IMediaArtistNativeHelperVideoFrameSize::QQVGA:
        case IMediaArtistNativeHelperVideoFrameSize::QCIF:
            return IMediaArtistNativeHelperBitrate::BR_128_KBPS;
        case IMediaArtistNativeHelperVideoFrameSize::QVGA:
        case IMediaArtistNativeHelperVideoFrameSize::CIF:
            return IMediaArtistNativeHelperBitrate::BR_384_KBPS;
        case IMediaArtistNativeHelperVideoFrameSize::VGA:
        case IMediaArtistNativeHelperVideoFrameSize::WVGA:
        case IMediaArtistNativeHelperVideoFrameSize::NTSC:
        case IMediaArtistNativeHelperVideoFrameSize::nHD:
        case IMediaArtistNativeHelperVideoFrameSize::WVGA16x9:
            return IMediaArtistNativeHelperBitrate::BR_2_MBPS;
        case IMediaArtistNativeHelperVideoFrameSize::V720p:
        case IMediaArtistNativeHelperVideoFrameSize::W720p:
        case IMediaArtistNativeHelperVideoFrameSize::S720p:
            return IMediaArtistNativeHelperBitrate::BR_5_MBPS;
        case IMediaArtistNativeHelperVideoFrameSize::V1080p:
        default:
            return IMediaArtistNativeHelperBitrate::BR_8_MBPS;
    }
}

ECode MediaArtistNativeHelper::Export(
        /* [in] */ const String& filePath,
        /* [in] */ const String& projectDir,
        /* [in] */ Int32 height,
        /* [in] */ Int32 bitrate,
        /* [in] */ ArrayOf<IMediaItem*>* mediaItemsList,
        /* [in] */ ArrayOf<ITransition*>* mediaTransitionList,
        /* [in] */ ArrayOf<IAudioTrack2*>* mediaBGMList,
        /* [in] */ IVideoEditorExportProgressListener* listener)
{
    Int32 outBitrate = 0;
    mExportFilename = filePath;
    PreviewStoryBoard(mediaItemsList, mediaTransitionList, mediaBGMList, NULL);
    mExportProgressListener = listener;
    Int32 outVideoProfile = 0;
    Int32 outVideoLevel = 0;

    /** Check the platform specific maximum export resolution */
    AutoPtr<IVideoEditorProfileHelper> helper;
    CVideoEditorProfileHelper::AcquireSingleton((IVideoEditorProfileHelper**)&helper);
    AutoPtr<IVideoEditorProfile> veProfile;
    helper->Get((IVideoEditorProfile**)&veProfile);
    if (veProfile == NULL) {
        //throw new RuntimeException("Can't get the video editor profile");
    }
    Int32 maxOutputHeight = ((CVideoEditorProfile*)veProfile.Get())->maxOutputVideoFrameHeight;
    Int32 maxOutputWidth = ((CVideoEditorProfile*)veProfile.Get())->maxOutputVideoFrameWidth;
    if (height > maxOutputHeight) {
        //throw new IllegalArgumentException("Unsupported export resolution. Supported maximum width:" +
        //        maxOutputWidth + " height:" + maxOutputHeight +
        //       " current height:" + height);
    }
    helper->GetExportProfile(mExportVideoCodec, &outVideoProfile);
    helper->GetExportLevel(mExportVideoCodec, &outVideoLevel);

    mProgressToApp = 0;

    switch (bitrate) {
        case IMediaProperties::BITRATE_28K:
            outBitrate = IMediaArtistNativeHelperBitrate::BR_32_KBPS;
            break;
        case IMediaProperties::BITRATE_40K:
            outBitrate = IMediaArtistNativeHelperBitrate::BR_48_KBPS;
            break;
        case IMediaProperties::BITRATE_64K:
            outBitrate = IMediaArtistNativeHelperBitrate::BR_64_KBPS;
            break;
        case IMediaProperties::BITRATE_96K:
            outBitrate = IMediaArtistNativeHelperBitrate::BR_96_KBPS;
            break;
        case IMediaProperties::BITRATE_128K:
            outBitrate = IMediaArtistNativeHelperBitrate::BR_128_KBPS;
            break;
        case IMediaProperties::BITRATE_192K:
            outBitrate = IMediaArtistNativeHelperBitrate::BR_192_KBPS;
            break;
        case IMediaProperties::BITRATE_256K:
            outBitrate = IMediaArtistNativeHelperBitrate::BR_256_KBPS;
            break;
        case IMediaProperties::BITRATE_384K:
            outBitrate = IMediaArtistNativeHelperBitrate::BR_384_KBPS;
            break;
        case IMediaProperties::BITRATE_512K:
            outBitrate = IMediaArtistNativeHelperBitrate::BR_512_KBPS;
            break;
        case IMediaProperties::BITRATE_800K:
            outBitrate = IMediaArtistNativeHelperBitrate::BR_800_KBPS;
            break;
        case IMediaProperties::BITRATE_2M:
            outBitrate = IMediaArtistNativeHelperBitrate::BR_2_MBPS;
            break;
        case IMediaProperties::BITRATE_5M:
            outBitrate = IMediaArtistNativeHelperBitrate::BR_5_MBPS;
            break;
        case IMediaProperties::BITRATE_8M:
            outBitrate = IMediaArtistNativeHelperBitrate::BR_8_MBPS;
            break;

        default:
            //throw new IllegalArgumentException("Argument Bitrate incorrect");
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    //mPreviewEditSettings.videoFrameRate = VideoFrameRate.FR_30_FPS;
    mPreviewEditSettings->videoFrameRate = IMediaArtistNativeHelperVideoFrameRate::FR_15_FPS;
    mPreviewEditSettings->outputFile = mOutputFilename = filePath;

    Int32 aspectRatio;
    mVideoEditor->GetAspectRatio(&aspectRatio);
    mPreviewEditSettings->videoFrameSize = FindVideoResolution(aspectRatio, height);
    mPreviewEditSettings->videoFormat = mExportVideoCodec;
    mPreviewEditSettings->audioFormat = mExportAudioCodec;
    mPreviewEditSettings->videoProfile = outVideoProfile;
    mPreviewEditSettings->videoLevel = outVideoLevel;
    mPreviewEditSettings->audioSamplingFreq = IMediaArtistNativeHelperAudioSamplingFrequency::FREQ_32000;
    mPreviewEditSettings->maxFileSize = 0;
    mPreviewEditSettings->audioChannels = 2;
    mPreviewEditSettings->videoBitrate = outBitrate;
    mPreviewEditSettings->audioBitrate = IMediaArtistNativeHelperBitrate::BR_96_KBPS;

    mPreviewEditSettings->transitionSettingsArray = ArrayOf<TransitionSettings*>::Alloc(mTotalClips - 1);
    for (Int32 index = 0; index < mTotalClips - 1; index++) {
        (*mPreviewEditSettings->transitionSettingsArray)[index] = new TransitionSettings();
        (*mPreviewEditSettings->transitionSettingsArray)[index]->videoTransitionType =
            IMediaArtistNativeHelperVideoTransition::NONE;
        (*mPreviewEditSettings->transitionSettingsArray)[index]->audioTransitionType =
            IMediaArtistNativeHelperAudioTransition::NONE;
    }

    for (Int32 clipCnt = 0; clipCnt < mPreviewEditSettings->clipSettingsArray->GetLength(); clipCnt++) {
        if ((*mPreviewEditSettings->clipSettingsArray)[clipCnt]->fileType == IMediaArtistNativeHelperFileType::JPG) {
            (*mPreviewEditSettings->clipSettingsArray)[clipCnt]->clipPath =
            (*mPreviewEditSettings->clipSettingsArray)[clipCnt]->clipOriginalPath;
        }
    }
    NativePopulateSettings(mPreviewEditSettings, mClipProperties, mAudioSettings);

    Int32 err = 0;
    //try {
        mProcessingState  = PROCESSING_EXPORT;
        mProcessingObject = NULL;
        GenerateClip(mPreviewEditSettings, &err);
        mProcessingState  = PROCESSING_NONE;
    //} catch (IllegalArgumentException ex) {
    //    Log.e(TAG, "IllegalArgument for generateClip");
    //    throw ex;
    //} catch (IllegalStateException ex) {
    //    Log.e(TAG, "IllegalStateExceptiont for generateClip");
    //    throw ex;
    //} catch (RuntimeException ex) {
    //    Log.e(TAG, "RuntimeException for generateClip");
    //    throw ex;
    //}

    if (err != 0) {
        // Log.e(TAG, "RuntimeException for generateClip");
        // throw new RuntimeException("generateClip failed with error=" + err);
        return E_RUNTIME_EXCEPTION;
    }

    mExportProgressListener = NULL;
    return NOERROR;
}

ECode MediaArtistNativeHelper::Stop(
    /* [in] */ const String& filename)
{
    //try {
        StopEncoding();
        AutoPtr<IFile> file;
        CFile::New(mExportFilename, (IFile**)&file);
        Boolean b;
        return file->Delete(&b);
    //} catch (IllegalStateException ex) {
    //    Log.e(TAG, "Illegal state exception in unload settings");
    //    throw ex;
    //} catch (RuntimeException ex) {
    //    Log.e(TAG, "Runtime exception in unload settings");
    //    throw ex;
    //}
}

ECode MediaArtistNativeHelper::GetPixels(
        /* [in] */ const String& filename,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int64 timeMs,
        /* [in] */ Int32 videoRotation,
        /* [out] */ IBitmap** bitmap)
{
    AutoPtr<ArrayOf<IBitmap*> > result = ArrayOf<IBitmap*>::Alloc(1);
    AutoPtr<ArrayOf<Int32> > indices = ArrayOf<Int32>::Alloc(1);
    indices->Set(0, 0);
    AutoPtr<MediaItemGetThumbnailListCallback> cb = new MediaItemGetThumbnailListCallback(this, result);
    GetPixelsList(filename, width, height, timeMs, timeMs, 1, indices, cb, videoRotation);
    *bitmap = (*result)[0];
    return NOERROR;
}

ECode MediaArtistNativeHelper::GetPixelsList(
        /* [in] */ const String& filename,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int64 startMs,
        /* [in] */ Int64 endMs,
        /* [in] */ Int32 thumbnailCount,
        /* [in] */ ArrayOf<Int32>* indices,
        /* [in] */ IMediaItemGetThumbnailListCallback* callback,
        /* [in] */ Int32 videoRotation)
{
    // The decoder needs output width and height as even
    Int32 decWidth = (width + 1) & 0xFFFFFFFE;
    Int32 decHeight = (height + 1) & 0xFFFFFFFE;
    Int32 thumbnailSize = decWidth * decHeight;

    // We convert the decoder output (in int[]) to a bitmap by first
    // copy it into an IntBuffer, then use Bitmap.copyPixelsFromBuffer to
    // copy it to the bitmap.
    AutoPtr<ArrayOf<Int32> > decArray = ArrayOf<Int32>::Alloc(thumbnailSize);
    AutoPtr<IInt32BufferHelper> helper;
    CInt32BufferHelper::AcquireSingleton((IInt32BufferHelper**)&helper);
    AutoPtr<IInt32Buffer> decBuffer;
    helper->Allocate(thumbnailSize, (IInt32Buffer**)&decBuffer);

    // If we need to resize and/or rotate the decoder output, we need a
    // temporary bitmap to hold the decoded output.
    Boolean needToMassage =
            (decWidth != width || decHeight != height || videoRotation != 0);
    AutoPtr<IBitmap> tmpBitmap;
    if(needToMassage) {
        AutoPtr<IBitmapFactory> bitmapFactory;
        CBitmapFactory::AcquireSingleton((IBitmapFactory**)&bitmapFactory);
        bitmapFactory->CreateBitmap(decWidth, decHeight, Elastos::Droid::Graphics::BitmapConfig_ARGB_8888, (IBitmap**)&tmpBitmap);
    } else {
        tmpBitmap = NULL;
    }

    // The final output bitmap width/height may swap because of rotation.
    Boolean needToSwapWH = (videoRotation == 90 || videoRotation == 270);
    Int32 outWidth = needToSwapWH ? height : width;
    Int32 outHeight = needToSwapWH ? width : height;

    AutoPtr<MediaArtistNativeHelperNativeGetPixelsListCallback> cb =
    new MediaArtistNativeHelperNativeGetPixelsListCallback(this, outWidth, outHeight, decBuffer,
        decArray, thumbnailSize, needToMassage, tmpBitmap, decWidth, decHeight, videoRotation, callback);
    NativeGetPixelsList(filename, decArray, decWidth, decHeight,
            thumbnailCount, startMs, endMs, indices, cb);

    if (tmpBitmap != NULL) {
        tmpBitmap->Recycle();
    }
    return NOERROR;
}

ECode MediaArtistNativeHelper::GenerateAudioGraph(
        /* [in] */ const String& uniqueId,
        /* [in] */ const String& inFileName,
        /* [in] */ const String& OutAudiGraphFileName,
        /* [in] */ Int32 frameDuration,
        /* [in] */ Int32 audioChannels,
        /* [in] */ Int32 samplesCount,
        /* [in] */ IExtractAudioWaveformProgressListener* listener,
        /* [in] */ Boolean isVideo)
{
    String tempPCMFileName;

    mExtractAudioWaveformProgressListener = listener;

    /**
     * In case of Video, first call will generate the PCM file to make the
     * audio graph
     */
    if (isVideo) {
        tempPCMFileName = mProjectPath + "/" + uniqueId + ".pcm";
    }
    else {
        tempPCMFileName = mAudioTrackPCMFilePath;
    }

    /**
     * For Video item, generate the PCM
     */
    if (isVideo) {
        NativeGenerateRawAudio(inFileName, tempPCMFileName);
    }

    NativeGenerateAudioGraph(tempPCMFileName, OutAudiGraphFileName, frameDuration,
            audioChannels, samplesCount);

    /**
     * Once the audio graph file is generated, delete the pcm file
     */
    if (isVideo) {
        AutoPtr<IFile> file;
        CFile::New(tempPCMFileName, (IFile**)&file);
        Boolean b;
        file->Delete(&b);
    }
    return NOERROR;
}

ECode MediaArtistNativeHelper::ClearPreviewSurface(
    /* [in] */ ISurface* surface)
{
    NativeClearSurface(surface);
    return NOERROR;
}

void MediaArtistNativeHelper::Lock()
{
    // if (Log.isLoggable(TAG, Log.DEBUG)) {
    //     Log.d(TAG, "lock: grabbing semaphore", new Throwable());
    // }
    mLock->Acquire();
    // if (Log.isLoggable(TAG, Log.DEBUG)) {
    //     Log.d(TAG, "lock: grabbed semaphore");
    // }
}

void MediaArtistNativeHelper::Unlock()
{
    // if (Log.isLoggable(TAG, Log.DEBUG)) {
    //     Log.d(TAG, "unlock: releasing semaphore");
    // }
    mLock->ToRelease();
}

/* JNI method */
AutoPtr<MediaArtistNativeHelper::Properties> MediaArtistNativeHelper::GetMediaProperties(
        /* [in] */ const String& file)
{
    // jobject object = M4OSA_NULL;
    // jclass clazz = pEnv->FindClass(PROPERTIES_CLASS_NAME);
    // jfieldID fid;
    // Boolean needToBeLoaded = TRUE;
    // ManualEditContext* pContext = M4OSA_NULL;
    // M4OSA_ERR          result   = M4NO_ERROR;
    // Int32 profile = 0;
    // Int32 level = 0;
    // Int32 videoFormat = 0;

    // // Get the context.
    // pContext = (ManualEditContext*)videoEditClasses_getContext(&needToBeLoaded, pEnv, thiz);

    // videoEditJava_checkAndThrowIllegalStateException(&needToBeLoaded, pEnv,
    //                                          (M4OSA_NULL == clazz),
    //                                          "not initialized");

    // object = videoEditProp_getProperties(pEnv,thiz,file);

    // if (object != M4OSA_NULL) {
    //     fid = pEnv->GetFieldID(clazz,"profile","I");
    //     profile = pEnv->GetIntField(object,fid);
    //     fid = pEnv->GetFieldID(clazz,"level","I");
    //     level = pEnv->GetIntField(object,fid);
    //     fid = pEnv->GetFieldID(clazz,"videoFormat","I");
    //     videoFormat = pEnv->GetIntField(object,fid);

    //     result = checkClipVideoProfileAndLevel(pContext->decoders, videoFormat, profile, level);

    //     fid = pEnv->GetFieldID(clazz,"profileSupported","Z");
    //     if (M4VSS3GPP_ERR_EDITING_UNSUPPORTED_VIDEO_PROFILE == result) {
    //         pEnv->SetBooleanField(object,fid,FALSE);
    //     }

    //     fid = pEnv->GetFieldID(clazz,"levelSupported","Z");
    //     if (M4VSS3GPP_ERR_EDITING_UNSUPPORTED_VIDEO_LEVEL == result) {
    //         pEnv->SetBooleanField(object,fid,FALSE);
    //     }
    // }
    // return object;
    return NULL;
}

AutoPtr<MediaArtistNativeHelper::Version> MediaArtistNativeHelper::GetVersion()
{
    // Boolean           isSuccessful          = TRUE;
    // jobject        version         = NULL;
    // M4_VersionInfo versionInfo     = {0, 0, 0, 0};
    // M4OSA_ERR      result          = M4NO_ERROR;

    // VIDEOEDIT_LOG_API(ANDROID_LOG_INFO, "VIDEO_EDITOR", "videoEditor_getVersion()");

    // versionInfo.m_structSize = sizeof(versionInfo);
    // versionInfo.m_major = VIDEOEDITOR_VERSION_MAJOR;
    // versionInfo.m_minor = VIDEOEDITOR_VERSION_MINOR;
    // versionInfo.m_revision = VIDEOEDITOR_VERSION_REVISION;

    // VIDEOEDIT_LOG_API(ANDROID_LOG_INFO, "VIDEO_EDITOR", "videoEditor_getVersion() major %d,\
    //  minor %d, revision %d", versionInfo.m_major, versionInfo.m_minor, versionInfo.m_revision);

    // // Create a version object.
    // videoEditClasses_createVersion(&isSuccessful, pEnv, &versionInfo, &version);

    // // Return the version object.
    // return(version);
    return NULL;
}

Int32 MediaArtistNativeHelper::NativeGetPixels(
        /* [in] */ const String& fileName,
        /* [in] */ ArrayOf<Int32>* pixelArray,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int64 timeMS)
{
    // M4OSA_ERR       err = M4NO_ERROR;
    // M4OSA_Context   mContext = M4OSA_NULL;
    // jint*           m_dst32 = M4OSA_NULL;


    // // Add a text marker (the condition must always be TRUE).
    // ADD_TEXT_MARKER_FUN(NULL != env)

    // const char *pString = env->GetStringUTFChars(path, NULL);
    // if (pString == M4OSA_NULL) {
    //     if (env != NULL) {
    //         jniThrowException(env, "java/lang/RuntimeException", "Input string NULL");
    //     }
    //     return M4ERR_ALLOC;
    // }

    // err = ThumbnailOpen(&mContext,(const M4OSA_Char*)pString, M4OSA_FALSE);
    // if (err != M4NO_ERROR || mContext == M4OSA_NULL) {
    //     if (pString != NULL) {
    //         env->ReleaseStringUTFChars(path, pString);
    //     }
    //     if (env != NULL) {
    //         jniThrowException(env, "java/lang/RuntimeException", "ThumbnailOpen failed");
    //     }
    // }

    // m_dst32 = env->GetIntArrayElements(pixelArray, NULL);

    // err = ThumbnailGetPixels32(mContext, (M4OSA_Int32 *)m_dst32, width,height,&timeMS,0);
    // if (err != M4NO_ERROR ) {
    //     if (env != NULL) {
    //         jniThrowException(env, "java/lang/RuntimeException",\
    //             "ThumbnailGetPixels32 failed");
    //     }
    // }
    // env->ReleaseIntArrayElements(pixelArray, m_dst32, 0);

    // ThumbnailClose(mContext);
    // if (pString != NULL) {
    //     env->ReleaseStringUTFChars(path, pString);
    // }

    // return timeMS;
    return 0;
}

Int32 MediaArtistNativeHelper::NativeGetPixelsList(
    /* [in] */ const String& fileName,
    /* [in] */ ArrayOf<Int32>* pixelArray,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 nosofTN,
    /* [in] */ Int64 startTimeMs,
    /* [in] */ Int64 endTimeMs,
    /* [in] */ ArrayOf<Int32>* indices,
    /* [in] */ IMediaArtistNativeHelperNativeGetPixelsListCallback* callback)
{
    // M4OSA_ERR           err = M4NO_ERROR;
    // M4OSA_Context       mContext = M4OSA_NULL;

    // const char *pString = env->GetStringUTFChars(path, NULL);
    // if (pString == M4OSA_NULL) {
    //     jniThrowException(env, "java/lang/RuntimeException", "Input string null");
    //     return M4ERR_ALLOC;
    // }

    // err = ThumbnailOpen(&mContext,(const M4OSA_Char*)pString, M4OSA_FALSE);
    // if (err != M4NO_ERROR || mContext == M4OSA_NULL) {
    //     jniThrowException(env, "java/lang/RuntimeException", "ThumbnailOpen failed");
    //     if (pString != NULL) {
    //         env->ReleaseStringUTFChars(path, pString);
    //     }
    //     return err;
    // }

    // jlong duration = (endTime - startTime);
    // M4OSA_UInt32 tolerance = duration / (2 * noOfThumbnails);
    // jint* m_dst32 = env->GetIntArrayElements(pixelArray, NULL);
    // jint* indices = env->GetIntArrayElements(indexArray, NULL);
    // jsize len = env->GetArrayLength(indexArray);

    // jclass cls = env->GetObjectClass(callback);
    // jmethodID mid = env->GetMethodID(cls, "onThumbnail", "(I)V");

    // for (int i = 0; i < len; i++) {
    //     int k = indices[i];
    //     M4OSA_UInt32 timeMS = startTime;
    //     timeMS += (2 * k + 1) * duration / (2 * noOfThumbnails);
    //     err = ThumbnailGetPixels32(mContext, ((M4OSA_Int32 *)m_dst32),
    //         width, height, &timeMS, tolerance);
    //     if (err != M4NO_ERROR) {
    //         break;
    //     }
    //     env->CallVoidMethod(callback, mid, (jint)k);
    //     if (env->ExceptionCheck()) {
    //         err = M4ERR_ALLOC;
    //         break;
    //     }
    // }

    // env->ReleaseIntArrayElements(pixelArray, m_dst32, 0);
    // env->ReleaseIntArrayElements(indexArray, indices, 0);

    // ThumbnailClose(mContext);
    // if (pString != NULL) {
    //     env->ReleaseStringUTFChars(path, pString);
    // }

    // if (err != M4NO_ERROR && !env->ExceptionCheck()) {
    //     jniThrowException(env, "java/lang/RuntimeException",\
    //             "ThumbnailGetPixels32 failed");
    // }

    // return err;
    return 0;
}

void MediaArtistNativeHelper::ReleaseResources()
{
   // bool               released = TRUE;
   //  ManualEditContext* pContext = M4OSA_NULL;
   //  M4OSA_ERR          result   = M4NO_ERROR;

   //  VIDEOEDIT_LOG_API(ANDROID_LOG_INFO, "VIDEO_EDITOR", "videoEditor_release()");

   //  // Add a text marker (the condition must always be TRUE).
   //  ADD_TEXT_MARKER_FUN(NULL != pEnv)

   //  // Get the context.
   //  pContext = (ManualEditContext*)videoEditClasses_getContext(&released, pEnv, thiz);

   //  // If context is not set, return (we consider release already happened)
   //  if (pContext == NULL) {
   //      ALOGV("videoEditor_release Nothing to do, context is aleady NULL");
   //      return;
   //  }


   //  // Check if the context is valid (required because the context is dereferenced).
   //  if (released)
   //  {
   //      if (pContext->state != ManualEditState_INITIALIZED)
   //      {
   //          // Change state and get Lock
   //          // This will ensure the generateClip function exits if it is running
   //          pContext->state = ManualEditState_STOPPING;
   //          AutoLock autoLock(pContext->mLock);
   //      }

   //      // Reset the context.
   //      videoEditClasses_setContext(&released, pEnv, thiz, (void *)M4OSA_NULL);

   //      // Check if the command should be closed.
   //      if (pContext->state != ManualEditState_INITIALIZED)
   //      {
   //          // Close the command.
   //          ALOGV("videoEditor_release Calling M4xVSS_CloseCommand() state =%d",
   //              pContext->state);
   //          result = M4xVSS_CloseCommand(pContext->engineContext);
   //          ALOGV("videoEditor_release M4xVSS_CloseCommand() returned 0x%x",
   //              (unsigned int)result);

   //          // Check if the command could be closed.
   //          videoEditJava_checkAndThrowRuntimeException(&released, pEnv,
   //              (M4NO_ERROR != result), result);
   //      }

   //      // Cleanup the engine.
   //      ALOGV("videoEditor_release Calling M4xVSS_CleanUp()");
   //      result = M4xVSS_CleanUp(pContext->engineContext);
   //      ALOGV("videoEditor_release M4xVSS_CleanUp() returned 0x%x", (unsigned int)result);

   //      // Check if the cleanup succeeded.
   //      videoEditJava_checkAndThrowRuntimeException(&released, pEnv,
   //          (M4NO_ERROR != result), result);

   //      // Free the edit settings.
   //      videoEditClasses_freeEditSettings(&pContext->pEditSettings);
   //      pContext->pEditSettings = M4OSA_NULL;


   //      if(pContext->mPreviewController != M4OSA_NULL)
   //      {
   //          delete pContext->mPreviewController;
   //          pContext->mPreviewController = M4OSA_NULL;
   //      }

   //      // Free the mAudioSettings context.
   //      if(pContext->mAudioSettings != M4OSA_NULL)
   //      {
   //          if (pContext->mAudioSettings->pFile != NULL) {
   //              free(pContext->mAudioSettings->pFile);
   //              pContext->mAudioSettings->pFile = M4OSA_NULL;
   //          }
   //          if (pContext->mAudioSettings->pPCMFilePath != NULL) {
   //              free(pContext->mAudioSettings->pPCMFilePath);
   //              pContext->mAudioSettings->pPCMFilePath = M4OSA_NULL;
   //          }

   //          free(pContext->mAudioSettings);
   //          pContext->mAudioSettings = M4OSA_NULL;
   //      }
   //      // Free video Decoders capabilities
   //      if (pContext->decoders != M4OSA_NULL) {
   //          VideoDecoder *pDecoder = NULL;
   //          VideoComponentCapabilities *pComponents = NULL;
   //          int32_t decoderNumber = pContext->decoders->decoderNumber;
   //          if (pContext->decoders->decoder != NULL &&
   //              decoderNumber > 0) {
   //              pDecoder = pContext->decoders->decoder;
   //              for (int32_t k = 0; k < decoderNumber; k++) {
   //                  // free each component
   //                  ALOGV("decoder index :%d",k);
   //                  if (pDecoder != NULL &&
   //                      pDecoder->component != NULL &&
   //                      pDecoder->componentNumber > 0) {
   //                      ALOGV("component number %d",pDecoder->componentNumber);
   //                      int32_t componentNumber =
   //                         pDecoder->componentNumber;

   //                      pComponents = pDecoder->component;
   //                      for (int32_t i = 0; i< componentNumber; i++) {
   //                          ALOGV("component index :%d",i);
   //                          if (pComponents != NULL &&
   //                              pComponents->profileLevel != NULL) {
   //                              free(pComponents->profileLevel);
   //                              pComponents->profileLevel = NULL;
   //                          }
   //                          pComponents++;
   //                      }
   //                      free(pDecoder->component);
   //                      pDecoder->component = NULL;
   //                  }

   //                  pDecoder++;
   //              }
   //              free(pContext->decoders->decoder);
   //              pContext->decoders->decoder = NULL;
   //          }
   //          free(pContext->decoders);
   //          pContext->decoders = NULL;
   //      }

   //      videoEditor_freeContext(pEnv, &pContext);
   //  }
}

void MediaArtistNativeHelper::NativeClearSurface(
    /* [in] */ ISurface* surface)
{
    // bool needToBeLoaded = TRUE;
    // M4OSA_ERR result = M4NO_ERROR;
    // VideoEditor_renderPreviewFrameStr frameStr;
    // const char* pMessage = NULL;
    // // Let the size be QVGA
    // int width = 320;
    // int height = 240;
    // ManualEditContext* pContext = M4OSA_NULL;

    // // Get the context.
    // pContext = (ManualEditContext*)videoEditClasses_getContext(&needToBeLoaded, pEnv, thiz);
    // VIDEOEDIT_LOG_FUNCTION(ANDROID_LOG_INFO,
    //                             "VIDEO_EDITOR","pContext = 0x%x",pContext);

    // // Make sure that the context was set.
    // videoEditJava_checkAndThrowIllegalStateException(&needToBeLoaded, pEnv,
    //                                          (M4OSA_NULL == pContext),
    //                                          "not initialized");

    // // Make sure that the context was set.
    // videoEditJava_checkAndThrowIllegalStateException(&needToBeLoaded, pEnv,
    //                              (M4OSA_NULL == pContext->mPreviewController),
    //                              "not initialized");

    // // Validate the surface parameter.
    // videoEditJava_checkAndThrowIllegalArgumentException(&needToBeLoaded, pEnv,
    //                                             (NULL == surface),
    //                                             "surface is NULL");

    // jclass surfaceClass = pEnv->FindClass("android/view/Surface");
    // videoEditJava_checkAndThrowIllegalStateException(&needToBeLoaded, pEnv,
    //                                          (M4OSA_NULL == surfaceClass),
    //                                          "not initialized");

    // jfieldID surface_native =
    //         pEnv->GetFieldID(surfaceClass, ANDROID_VIEW_SURFACE_JNI_ID, "I");
    // videoEditJava_checkAndThrowIllegalStateException(&needToBeLoaded, pEnv,
    //                                          (M4OSA_NULL == surface_native),
    //                                          "not initialized");

    // Surface* const p = (Surface*)pEnv->GetIntField(surface, surface_native);
    // sp<Surface> previewSurface = sp<Surface>(p);
    // // Validate the mSurface's mNativeSurface field
    // videoEditJava_checkAndThrowIllegalStateException(&needToBeLoaded, pEnv,
    //                                             (NULL == previewSurface.get()),
    //                                             "mNativeSurface is NULL");

    // frameStr.pBuffer = M4OSA_NULL;
    // frameStr.timeMs = 0;
    // frameStr.uiSurfaceWidth = width;
    // frameStr.uiSurfaceHeight = height;
    // frameStr.uiFrameWidth = width;
    // frameStr.uiFrameHeight = height;
    // frameStr.bApplyEffect = M4OSA_FALSE;
    // frameStr.clipBeginCutTime = 0;
    // frameStr.clipEndCutTime = 0;

    // result = pContext->mPreviewController->clearSurface(previewSurface,
    //                                                           &frameStr);
    // videoEditJava_checkAndThrowRuntimeException(&needToBeLoaded, pEnv,
    //         (M4NO_ERROR != result), result);
}

void MediaArtistNativeHelper::StopEncoding()
{
    // bool               stopped  = TRUE;
    // ManualEditContext* pContext = M4OSA_NULL;
    // M4OSA_ERR          result   = M4NO_ERROR;

    // ALOGV("videoEditor_stopEncoding START");

    // // Get the context.
    // pContext = (ManualEditContext*)videoEditClasses_getContext(&stopped, pEnv, thiz);

    // // Change state and get Lock
    // // This will ensure the generateClip function exits
    // pContext->state = ManualEditState_STOPPING;
    // AutoLock autoLock(pContext->mLock);

    // // Make sure that the context was set.
    // videoEditJava_checkAndThrowIllegalStateException(&stopped, pEnv,
    //                                          (M4OSA_NULL == pContext),
    //                                          "not initialized");

    // if (stopped) {

    //     // Check if the command should be closed.
    //     if (pContext->state != ManualEditState_INITIALIZED)
    //     {
    //         // Close the command.
    //         ALOGV("videoEditor_stopEncoding Calling M4xVSS_CloseCommand()");
    //         result = M4xVSS_CloseCommand(pContext->engineContext);
    //         ALOGV("videoEditor_stopEncoding M4xVSS_CloseCommand() returned 0x%x",
    //             (unsigned int)result);
    //     }

    //     // Check if the command could be closed.
    //     videoEditJava_checkAndThrowRuntimeException(&stopped, pEnv,
    //         (M4NO_ERROR != result), result);

    //     // Free the edit settings.
    //     videoEditClasses_freeEditSettings(&pContext->pEditSettings);

    //     // Set the state to initialized.
    //     pContext->state = ManualEditState_INITIALIZED;
    // }
}

void MediaArtistNativeHelper::_init(
    /* [in] */ const String& tempPath,
    /* [in] */ const String& libraryPath)
{
    // bool                  initialized            = TRUE;
    // ManualEditContext*    pContext               = M4OSA_NULL;
    // VideoEditJava_EngineMethodIds methodIds              = {NULL};
    // M4OSA_Char*           pLibraryPath           = M4OSA_NULL;
    // M4OSA_Char*           pTextRendererPath      = M4OSA_NULL;
    // M4OSA_UInt32          textRendererPathLength = 0;
    // M4OSA_ERR             result                 = M4NO_ERROR;

    // VIDEOEDIT_LOG_API(ANDROID_LOG_INFO, "VIDEO_EDITOR", "videoEditor_init()");

    // // Add a text marker (the condition must always be TRUE).
    // ADD_TEXT_MARKER_FUN(NULL != pEnv)

    // // Get the context.
    // pContext = (ManualEditContext*)videoEditClasses_getContext(&initialized, pEnv, thiz);

    // // Get the engine method ids.
    // videoEditJava_getEngineMethodIds(&initialized, pEnv, &methodIds);

    // // Validate the tempPath parameter.
    // videoEditJava_checkAndThrowIllegalArgumentException(&initialized, pEnv,
    //                                             (NULL == tempPath),
    //                                             "tempPath is NULL");

    // // Make sure that the context was not set already.
    // videoEditJava_checkAndThrowIllegalStateException(&initialized, pEnv,
    //                                          (M4OSA_NULL != pContext),
    //                                          "already initialized");

    // // Check if the initialization succeeded (required because of dereferencing of psContext,
    // // and freeing when initialization fails).
    // if (initialized)
    // {
    //     // Allocate a new context.
    //     pContext = new ManualEditContext;

    //     // Check if the initialization succeeded (required because of dereferencing of psContext).
    //     //if (initialized)
    //     if (pContext != NULL)
    //     {
    //         // Set the state to not initialized.
    //         pContext->state = ManualEditState_NOT_INITIALIZED;

    //         // Allocate a file read pointer structure.
    //         pContext->initParams.pFileReadPtr =
    //          (M4OSA_FileReadPointer*)videoEditOsal_alloc(&initialized, pEnv,
    //           sizeof(M4OSA_FileReadPointer), "FileReadPointer");

    //         // Allocate a file write pointer structure.
    //         pContext->initParams.pFileWritePtr =
    //          (M4OSA_FileWriterPointer*)videoEditOsal_alloc(&initialized, pEnv,
    //           sizeof(M4OSA_FileWriterPointer), "FileWriterPointer");

    //         // Get the temp path.
    //         M4OSA_Char* tmpString =
    //             (M4OSA_Char *)videoEditJava_getString(&initialized, pEnv, tempPath,
    //             NULL, M4OSA_NULL);
    //         M4OSA_UInt32 length = strlen((const char *)tmpString);
    //         // Malloc additional 2 bytes for beginning and tail separator.
    //         M4OSA_UInt32 pathLength = length + 2;

    //         pContext->initParams.pTempPath = (M4OSA_Char *)
    //              M4OSA_32bitAlignedMalloc(pathLength, 0x0, (M4OSA_Char *)"tempPath");

    //         //initialize the first char. so that strcat works.
    //         M4OSA_Char *ptmpChar = (M4OSA_Char*)pContext->initParams.pTempPath;
    //         ptmpChar[0] = 0x00;
    //         strncat((char *)pContext->initParams.pTempPath, (const char *)tmpString,
    //             length);
    //         strncat((char *)pContext->initParams.pTempPath, (const char *)"/", (size_t)1);
    //         free(tmpString);
    //         tmpString = NULL;
    //         pContext->mIsUpdateOverlay = FALSE;
    //         pContext->mOverlayFileName = NULL;
    //         pContext->decoders = NULL;
    //     }

    //     // Check if the initialization succeeded
    //     // (required because of dereferencing of pContext, pFileReadPtr and pFileWritePtr).
    //     if (initialized)
    //     {

    //         // Initialize the OSAL file system function pointers.
    //         videoEditOsal_getFilePointers(pContext->initParams.pFileReadPtr ,
    //                                       pContext->initParams.pFileWritePtr);

    //         // Set the UTF8 conversion functions.
    //         pContext->initParams.pConvToUTF8Fct   = videoEditor_toUTF8Fct;
    //         pContext->initParams.pConvFromUTF8Fct = videoEditor_fromUTF8Fct;

    //         // Set the callback method ids.
    //         pContext->onProgressUpdateMethodId = methodIds.onProgressUpdate;

    //         // Set the virtual machine.
    //         pEnv->GetJavaVM(&(pContext->pVM));

    //         // Create a global reference to the engine object.
    //         pContext->engine = pEnv->NewGlobalRef(thiz);

    //         // Check if the global reference could be created.
    //         videoEditJava_checkAndThrowRuntimeException(&initialized, pEnv,
    //          (NULL == pContext->engine), M4NO_ERROR);
    //     }

    //     // Check if the initialization succeeded (required because of dereferencing of pContext).
    //     if (initialized)
    //     {
    //         // Log the API call.
    //         VIDEOEDIT_LOG_API(ANDROID_LOG_INFO, "VIDEO_EDITOR", "M4xVSS_Init()");

    //         // Initialize the visual studio library.
    //         result = M4xVSS_Init(&pContext->engineContext, &pContext->initParams);

    //         // Log the result.
    //         VIDEOEDIT_LOG_RESULT(ANDROID_LOG_INFO, "VIDEO_EDITOR",
    //          videoEditOsal_getResultString(result));

    //         // Check if the library could be initialized.
    //         videoEditJava_checkAndThrowRuntimeException(&initialized, pEnv,
    //          (M4NO_ERROR != result), result);

    //         // Get platform video decoder capablities.
    //         result = M4xVSS_getVideoDecoderCapabilities(&pContext->decoders);

    //         videoEditJava_checkAndThrowRuntimeException(&initialized, pEnv,
    //          (M4NO_ERROR != result), result);
    //     }

    //     if(initialized)
    //     {
    //         pContext->mPreviewController = new VideoEditorPreviewController();
    //         videoEditJava_checkAndThrowIllegalStateException(&initialized, pEnv,
    //                              (M4OSA_NULL == pContext->mPreviewController),
    //                              "not initialized");
    //         pContext->mAudioSettings =
    //          (M4xVSS_AudioMixingSettings *)
    //          M4OSA_32bitAlignedMalloc(sizeof(M4xVSS_AudioMixingSettings),0x0,
    //          (M4OSA_Char *)"mAudioSettings");
    //         videoEditJava_checkAndThrowIllegalStateException(&initialized, pEnv,
    //                                  (M4OSA_NULL == pContext->mAudioSettings),
    //                                  "not initialized");
    //         pContext->mAudioSettings->pFile = M4OSA_NULL;
    //         pContext->mAudioSettings->pPCMFilePath = M4OSA_NULL;
    //         pContext->mAudioSettings->bRemoveOriginal = 0;
    //         pContext->mAudioSettings->uiNbChannels = 0;
    //         pContext->mAudioSettings->uiSamplingFrequency = 0;
    //         pContext->mAudioSettings->uiExtendedSamplingFrequency = 0;
    //         pContext->mAudioSettings->uiAddCts = 0;
    //         pContext->mAudioSettings->uiAddVolume = 0;
    //         pContext->mAudioSettings->beginCutMs = 0;
    //         pContext->mAudioSettings->endCutMs = 0;
    //         pContext->mAudioSettings->fileType = 0;
    //         pContext->mAudioSettings->bLoop = 0;
    //         pContext->mAudioSettings->uiInDucking_lowVolume  = 0;
    //         pContext->mAudioSettings->bInDucking_enable  = 0;
    //         pContext->mAudioSettings->uiBTChannelCount  = 0;
    //         pContext->mAudioSettings->uiInDucking_threshold = 0;
    //     }
    //     // Check if the library could be initialized.
    //     if (initialized)
    //     {
    //         // Set the state to initialized.
    //         pContext->state = ManualEditState_INITIALIZED;
    //     }

    //     // Set the context.
    //     videoEditClasses_setContext(&initialized, pEnv, thiz, (void* )pContext);
    //     pLibraryPath = M4OSA_NULL;

    //     pContext->pEditSettings = M4OSA_NULL;
    //     // Cleanup if anything went wrong during initialization.
    //     if (!initialized)
    //     {
    //         // Free the context.
    //         videoEditor_freeContext(pEnv, &pContext);
    //     }
    // }
}

void MediaArtistNativeHelper::NativeStartPreview(
    /* [in] */ ISurface* mSurface,
    /* [in] */ Int64 fromMs,
    /* [in] */ Int64 toMs,
    /* [in] */ Int32 callbackAfterFrameCount,
    /* [in] */ Boolean loop)
{
    // bool needToBeLoaded = TRUE;
    // M4OSA_ERR result = M4NO_ERROR;
    // VIDEOEDIT_LOG_FUNCTION(ANDROID_LOG_INFO, "VIDEO_EDITOR", "videoEditor_startPreview()");

    // ManualEditContext* pContext = M4OSA_NULL;
    // // Get the context.
    // pContext = (ManualEditContext*)videoEditClasses_getContext(&needToBeLoaded, pEnv, thiz);

    // // Make sure that the context was set.
    // videoEditJava_checkAndThrowIllegalStateException(&needToBeLoaded, pEnv,
    //                                          (M4OSA_NULL == pContext),
    //                                          "not initialized");

    // videoEditJava_checkAndThrowIllegalStateException(&needToBeLoaded, pEnv,
    //                                  (M4OSA_NULL == pContext->mAudioSettings),
    //                                  "not initialized");
    // // Make sure that the context was set.
    // videoEditJava_checkAndThrowIllegalStateException(&needToBeLoaded, pEnv,
    //                              (M4OSA_NULL == pContext->mPreviewController),
    //                              "not initialized");

    // // Validate the mSurface parameter.
    // videoEditJava_checkAndThrowIllegalArgumentException(&needToBeLoaded, pEnv,
    //                                             (NULL == mSurface),
    //                                             "mSurface is NULL");

    // jclass surfaceClass = pEnv->FindClass("android/view/Surface");
    // videoEditJava_checkAndThrowIllegalStateException(&needToBeLoaded, pEnv,
    //                                          (M4OSA_NULL == surfaceClass),
    //                                          "not initialized");
    // //jfieldID surface_native = pEnv->GetFieldID(surfaceClass, "mSurface", "I");
    // jfieldID surface_native
    //     = pEnv->GetFieldID(surfaceClass, ANDROID_VIEW_SURFACE_JNI_ID, "I");

    // videoEditJava_checkAndThrowIllegalStateException(&needToBeLoaded, pEnv,
    //                                          (M4OSA_NULL == surface_native),
    //                                          "not initialized");

    // Surface* const p = (Surface*)pEnv->GetIntField(mSurface, surface_native);

    // sp<Surface> previewSurface = sp<Surface>(p);
    // // Validate the mSurface's mNativeSurface field
    // videoEditJava_checkAndThrowIllegalStateException(&needToBeLoaded, pEnv,
    //                                             (NULL == previewSurface.get()),
    //                                             "mNativeSurface is NULL");

    // result =  pContext->mPreviewController->setSurface(previewSurface);
    // videoEditJava_checkAndThrowRuntimeException(&needToBeLoaded, pEnv,
    //     (M4NO_ERROR != result), result);
    // VIDEOEDIT_LOG_FUNCTION(ANDROID_LOG_INFO, "VIDEO_EDITOR", "fromMs=%ld, toMs=%ld",
    //     (M4OSA_UInt32)fromMs, (M4OSA_Int32)toMs);

    // result = pContext->mPreviewController->startPreview((M4OSA_UInt32)fromMs,
    //                                             (M4OSA_Int32)toMs,
    //                                             (M4OSA_UInt16)callbackInterval,
    //                                             (M4OSA_Bool)loop);
    // videoEditJava_checkAndThrowRuntimeException(&needToBeLoaded, pEnv, (M4NO_ERROR != result), result);
}

void MediaArtistNativeHelper::NativePopulateSettings(
        /* [in] */ EditSettings* editSettings,
        /* [in] */ PreviewClipProperties* mProperties,
        /* [in] */ AudioSettings* mAudioSettings)
{
//     VIDEOEDIT_LOG_FUNCTION(ANDROID_LOG_INFO, "VIDEO_EDITOR",
//             "videoEditor_populateSettings()");

//     bool                needToBeLoaded  = TRUE;
//     ManualEditContext*  pContext        = M4OSA_NULL;
//     M4OSA_ERR           result          = M4NO_ERROR;
//     jstring             strPath         = M4OSA_NULL;
//     jstring             strPCMPath      = M4OSA_NULL;
//     jobjectArray        propertiesClipsArray           = M4OSA_NULL;
//     jobject             properties      = M4OSA_NULL;
//     jint*               bitmapArray     =  M4OSA_NULL;
//     jobjectArray        effectSettingsArray = M4OSA_NULL;
//     jobject             effectSettings  = M4OSA_NULL;
//     jintArray           pixelArray      = M4OSA_NULL;
//     int width = 0;
//     int height = 0;
//     int nbOverlays = 0;
//     int i,j = 0;
//     int *pOverlayIndex = M4OSA_NULL;
//     M4OSA_Char* pTempChar = M4OSA_NULL;

//     // Add a code marker (the condition must always be TRUE).
//     ADD_CODE_MARKER_FUN(NULL != pEnv)

//     // Validate the settings parameter.
//     videoEditJava_checkAndThrowIllegalArgumentException(&needToBeLoaded, pEnv,
//                                                 (NULL == settings),
//                                                 "settings is NULL");
//     // Get the context.
//     pContext =
//             (ManualEditContext*)videoEditClasses_getContext(&needToBeLoaded, pEnv, thiz);

//     // Make sure that the context was set.
//     videoEditJava_checkAndThrowIllegalStateException(&needToBeLoaded, pEnv,
//                                              (M4OSA_NULL == pContext),
//                                              "not initialized");
//     // Make sure that the context was set.
//     videoEditJava_checkAndThrowIllegalStateException(&needToBeLoaded, pEnv,
//                                  (M4OSA_NULL == pContext->mPreviewController),
//                                  "not initialized");
//     jclass mPreviewClipPropClazz = pEnv->FindClass(PREVIEW_PROPERTIES_CLASS_NAME);
//     videoEditJava_checkAndThrowIllegalStateException(&needToBeLoaded, pEnv,
//                                      (M4OSA_NULL == mPreviewClipPropClazz),
//                                      "not initialized");

//     jfieldID fid = pEnv->GetFieldID(mPreviewClipPropClazz,"clipProperties",
//             "[L"PROPERTIES_CLASS_NAME";"  );
//     videoEditJava_checkAndThrowIllegalStateException(&needToBeLoaded, pEnv,
//                                      (M4OSA_NULL == fid),
//                                      "not initialized");

//     propertiesClipsArray = (jobjectArray)pEnv->GetObjectField(object, fid);
//     videoEditJava_checkAndThrowIllegalStateException(&needToBeLoaded, pEnv,
//                                      (M4OSA_NULL == propertiesClipsArray),
//                                      "not initialized");

//     jclass engineClass = pEnv->FindClass(MANUAL_EDIT_ENGINE_CLASS_NAME);
//     videoEditJava_checkAndThrowIllegalStateException(&needToBeLoaded, pEnv,
//                                      (M4OSA_NULL == engineClass),
//                                      "not initialized");

//     pContext->onPreviewProgressUpdateMethodId = pEnv->GetMethodID(engineClass,
//             "onPreviewProgressUpdate",     "(IZZLjava/lang/String;II)V");
//     // Check if the context is valid (required because the context is dereferenced).
//     if (needToBeLoaded) {
//         // Make sure that we are in a correct state.
//         videoEditJava_checkAndThrowIllegalStateException(&needToBeLoaded, pEnv,
//                              (pContext->state != ManualEditState_INITIALIZED),
//                              "settings already loaded");
//         if (needToBeLoaded) {
//             // Retrieve the edit settings.
//             if (pContext->pEditSettings != M4OSA_NULL) {
//                 videoEditClasses_freeEditSettings(&pContext->pEditSettings);
//                 pContext->pEditSettings = M4OSA_NULL;
//             }
//             videoEditClasses_getEditSettings(&needToBeLoaded, pEnv,
//                 settings, &pContext->pEditSettings,FALSE);
//         }
//     }

//     if (needToBeLoaded == FALSE) {
//         j = 0;
//         while (j < pContext->pEditSettings->nbEffects)
//         {
//             if (pContext->pEditSettings->Effects[j].xVSS.pFramingFilePath != M4OSA_NULL) {
//                 if (pContext->pEditSettings->Effects[j].xVSS.pFramingBuffer != M4OSA_NULL) {
//                     free(pContext->pEditSettings->\
//                     Effects[j].xVSS.pFramingBuffer);
//                     pContext->pEditSettings->Effects[j].xVSS.pFramingBuffer = M4OSA_NULL;
//                 }
//             }
//           j++;
//         }
//         return;
//     }

//     M4OSA_TRACE1_0("videoEditorC_getEditSettings done");

//     pContext->previewFrameEditInfoId = pEnv->GetMethodID(engineClass,
//         "previewFrameEditInfo", "(Ljava/lang/String;I)V");

//     if ( pContext->pEditSettings != NULL )
//     {
//         // Check if the edit settings could be retrieved.
//         jclass mEditClazz = pEnv->FindClass(EDIT_SETTINGS_CLASS_NAME);
//         if(mEditClazz == M4OSA_NULL)
//         {
//             M4OSA_TRACE1_0("cannot find object field for mEditClazz");
//             goto videoEditor_populateSettings_cleanup;
//         }
//         jclass mEffectsClazz = pEnv->FindClass(EFFECT_SETTINGS_CLASS_NAME);
//         if(mEffectsClazz == M4OSA_NULL)
//         {
//             M4OSA_TRACE1_0("cannot find object field for mEffectsClazz");
//             goto videoEditor_populateSettings_cleanup;
//         }
//         fid = pEnv->GetFieldID(mEditClazz,"effectSettingsArray", "[L"EFFECT_SETTINGS_CLASS_NAME";"  );
//         if(fid == M4OSA_NULL)
//         {
//             M4OSA_TRACE1_0("cannot find field for effectSettingsArray Array");
//             goto videoEditor_populateSettings_cleanup;
//         }
//         effectSettingsArray = (jobjectArray)pEnv->GetObjectField(settings, fid);
//         if(effectSettingsArray == M4OSA_NULL)
//         {
//             M4OSA_TRACE1_0("cannot find object field for effectSettingsArray");
//             goto videoEditor_populateSettings_cleanup;
//         }

//         //int overlayIndex[pContext->pEditSettings->nbEffects];
//         if (pContext->pEditSettings->nbEffects > 0)
//         {
//             pOverlayIndex
//             = (int*) M4OSA_32bitAlignedMalloc(pContext->pEditSettings->nbEffects * sizeof(int), 0,
//                 (M4OSA_Char*)"pOverlayIndex");
//             if (pOverlayIndex == M4OSA_NULL) {
//                 videoEditJava_checkAndThrowRuntimeException(&needToBeLoaded, pEnv,
//                     M4OSA_TRUE, M4ERR_ALLOC);
//                 goto videoEditor_populateSettings_cleanup;
//             }
//         }

//         i = 0;
//         j = 0;
//         M4OSA_TRACE1_1("no of effects = %d",pContext->pEditSettings->nbEffects);
//         while (j < pContext->pEditSettings->nbEffects)
//         {
//             if (pContext->pEditSettings->Effects[j].xVSS.pFramingFilePath != M4OSA_NULL)
//             {
//                 pOverlayIndex[nbOverlays] = j;

//                 M4xVSS_FramingStruct *aFramingCtx = M4OSA_NULL;
//                 aFramingCtx
//                 = (M4xVSS_FramingStruct*)M4OSA_32bitAlignedMalloc(sizeof(M4xVSS_FramingStruct), M4VS,
//                   (M4OSA_Char*)"M4xVSS_internalDecodeGIF: Context of the framing effect");
//                 if (aFramingCtx == M4OSA_NULL)
//                 {
//                     M4OSA_TRACE1_0("Allocation error in videoEditor_populateSettings");
//                     videoEditJava_checkAndThrowRuntimeException(&needToBeLoaded, pEnv,
//                         M4OSA_TRUE, M4ERR_ALLOC);
//                     goto videoEditor_populateSettings_cleanup;
//                 }

//                 aFramingCtx->pCurrent = M4OSA_NULL; /* Only used by the first element of the chain */
//                 aFramingCtx->previousClipTime = -1;
//                 aFramingCtx->FramingYuv = M4OSA_NULL;
//                 aFramingCtx->FramingRgb = M4OSA_NULL;
//                 aFramingCtx->topleft_x
//                     = pContext->pEditSettings->Effects[j].xVSS.topleft_x;
//                 aFramingCtx->topleft_y
//                     = pContext->pEditSettings->Effects[j].xVSS.topleft_y;


//                  VIDEOEDIT_LOG_FUNCTION(ANDROID_LOG_INFO, "VIDEO_EDITOR", "OF u_width %d",
//                                         pContext->pEditSettings->Effects[j].xVSS.pFramingBuffer->u_width);
//                  VIDEOEDIT_LOG_FUNCTION(ANDROID_LOG_INFO, "VIDEO_EDITOR", "OF u_height() %d",
//                                         pContext->pEditSettings->Effects[j].xVSS.pFramingBuffer->u_height);
//                  VIDEOEDIT_LOG_FUNCTION(ANDROID_LOG_INFO, "VIDEO_EDITOR", "OF rgbType() %d",
//                                         pContext->pEditSettings->Effects[j].xVSS.rgbType);

//                  aFramingCtx->width = pContext->pEditSettings->Effects[j].xVSS.pFramingBuffer->u_width;
//                  aFramingCtx->height = pContext->pEditSettings->Effects[j].xVSS.pFramingBuffer->u_height;

//                 result = M4xVSS_internalConvertARGB888toYUV420_FrammingEffect(pContext->engineContext,
//                     &(pContext->pEditSettings->Effects[j]),aFramingCtx,
//                 pContext->pEditSettings->Effects[j].xVSS.framingScaledSize);
//                 videoEditJava_checkAndThrowRuntimeException(&needToBeLoaded, pEnv,
//                                             (M4NO_ERROR != result), result);
//                 if (needToBeLoaded == FALSE) {
//                     M4OSA_TRACE1_1("M4xVSS_internalConvertARGB888toYUV420_FrammingEffect returned 0x%x", result);
//                     if (aFramingCtx != M4OSA_NULL) {
//                         free(aFramingCtx);
//                         aFramingCtx = M4OSA_NULL;
//                     }
//                     goto videoEditor_populateSettings_cleanup;
//                 }

//                 //framing buffers are resized to fit the output video resolution.
//                 pContext->pEditSettings->Effects[j].xVSS.pFramingBuffer->u_width =
//                     aFramingCtx->FramingRgb->u_width;
//                 pContext->pEditSettings->Effects[j].xVSS.pFramingBuffer->u_height =
//                     aFramingCtx->FramingRgb->u_height;

//                 VIDEOEDIT_LOG_FUNCTION(ANDROID_LOG_INFO, "VIDEO_EDITOR", "A framing Context aFramingCtx->width = %d",
//                     aFramingCtx->FramingRgb->u_width);

//                 VIDEOEDIT_LOG_FUNCTION(ANDROID_LOG_INFO, "VIDEO_EDITOR", "A framing Context aFramingCtx->height = %d",
//                     aFramingCtx->FramingRgb->u_height);


//                 width = pContext->pEditSettings->Effects[j].xVSS.pFramingBuffer->u_width;
//                 height = pContext->pEditSettings->Effects[j].xVSS.pFramingBuffer->u_height;

//                 //RGB 565
//                 pContext->pEditSettings->Effects[j].xVSS.pFramingBuffer->u_stride = width * 2;

//                 //for RGB565
//                 pContext->pEditSettings->Effects[j].xVSS.pFramingBuffer->u_topleft = 0;
//                 pContext->pEditSettings->Effects[j].xVSS.pFramingBuffer->pac_data =
//                             (M4VIFI_UInt8 *)M4OSA_32bitAlignedMalloc(width*height*2,
//                             0x00,(M4OSA_Char *)"pac_data buffer");

//                 if (pContext->pEditSettings->Effects[j].xVSS.pFramingBuffer->pac_data == M4OSA_NULL) {
//                     M4OSA_TRACE1_0("Failed to allocate memory for framing buffer");
//                     videoEditJava_checkAndThrowRuntimeException(&needToBeLoaded, pEnv,
//                                             M4OSA_TRUE, M4ERR_ALLOC);
//                     goto videoEditor_populateSettings_cleanup;
//                 }

//                 memcpy((void *)&pContext->pEditSettings->\
//                     Effects[j].xVSS.pFramingBuffer->\
//                     pac_data[0],(void *)&aFramingCtx->FramingRgb->pac_data[0],(width*height*2));

//                 //As of now rgb type is 565
//                 pContext->pEditSettings->Effects[j].xVSS.rgbType =
//                     (M4VSS3GPP_RGBType) M4VSS3GPP_kRGB565;

//                 if (aFramingCtx->FramingYuv != M4OSA_NULL )
//                 {
//                     if (aFramingCtx->FramingYuv[0].pac_data != M4OSA_NULL) {
//                         free(aFramingCtx->FramingYuv[0].pac_data);
//                         aFramingCtx->FramingYuv[0].pac_data = M4OSA_NULL;
//                     }
//                     if (aFramingCtx->FramingYuv[1].pac_data != M4OSA_NULL) {
//                         free(aFramingCtx->FramingYuv[1].pac_data);
//                         aFramingCtx->FramingYuv[1].pac_data = M4OSA_NULL;
//                     }
//                     if (aFramingCtx->FramingYuv[2].pac_data != M4OSA_NULL) {
//                         free(aFramingCtx->FramingYuv[2].pac_data);
//                         aFramingCtx->FramingYuv[2].pac_data = M4OSA_NULL;
//                     }

//                     free(aFramingCtx->FramingYuv);
//                     aFramingCtx->FramingYuv = M4OSA_NULL;
//                 }
//                 if (aFramingCtx->FramingRgb->pac_data != M4OSA_NULL) {
//                     free(aFramingCtx->FramingRgb->pac_data);
//                     aFramingCtx->FramingRgb->pac_data = M4OSA_NULL;
//                 }
//                 if (aFramingCtx->FramingRgb != M4OSA_NULL) {
//                     free(aFramingCtx->FramingRgb);
//                     aFramingCtx->FramingRgb = M4OSA_NULL;
//                 }
//                 if (aFramingCtx != M4OSA_NULL) {
//                     free(aFramingCtx);
//                     aFramingCtx = M4OSA_NULL;
//                 }
//                 nbOverlays++;
//             }
//             j++;
//         }

//         // Check if the edit settings could be retrieved.
//         M4OSA_TRACE1_1("total clips are = %d",pContext->pEditSettings->uiClipNumber);
//         for (i = 0; i < pContext->pEditSettings->uiClipNumber; i++) {
//             M4OSA_TRACE1_1("clip no = %d",i);
//             properties = pEnv->GetObjectArrayElement(propertiesClipsArray, i);
//             videoEditJava_checkAndThrowIllegalStateException(&needToBeLoaded, pEnv,
//                 (M4OSA_NULL == properties),
//                 "not initialized");
//             if (needToBeLoaded) {
//                 getClipSetting(pEnv,properties, pContext->pEditSettings->pClipList[i]);
//                 pEnv->DeleteLocalRef(properties);
//             } else {
//                 pEnv->DeleteLocalRef(properties);
//                 goto videoEditor_populateSettings_cleanup;
//             }
//         }

//         if (needToBeLoaded) {
//             // Log the edit settings.
//             VIDEOEDIT_LOG_EDIT_SETTINGS(pContext->pEditSettings);
//         }
//     }
//     /* free previous allocations , if any */
//     if (pContext->mAudioSettings != M4OSA_NULL) {
//         if (pContext->mAudioSettings->pFile != NULL) {
//             free(pContext->mAudioSettings->pFile);
//             pContext->mAudioSettings->pFile = M4OSA_NULL;
//         }
//         if (pContext->mAudioSettings->pPCMFilePath != NULL) {
//             free(pContext->mAudioSettings->pPCMFilePath);
//             pContext->mAudioSettings->pPCMFilePath = M4OSA_NULL;
//         }
//     }

//     if (audioSettingObject != M4OSA_NULL) {
//         jclass audioSettingClazz = pEnv->FindClass(AUDIO_SETTINGS_CLASS_NAME);
//         videoEditJava_checkAndThrowIllegalStateException(&needToBeLoaded, pEnv,
//                                          (M4OSA_NULL == audioSettingClazz),
//                                          "not initialized");

//         videoEditJava_checkAndThrowIllegalStateException(&needToBeLoaded, pEnv,
//                                      (M4OSA_NULL == pContext->mAudioSettings),
//                                      "not initialized");

//         if (needToBeLoaded == FALSE) {
//             goto videoEditor_populateSettings_cleanup;
//         }

//         fid = pEnv->GetFieldID(audioSettingClazz,"bRemoveOriginal","Z");
//         pContext->mAudioSettings->bRemoveOriginal =
//             pEnv->GetBooleanField(audioSettingObject,fid);
//         M4OSA_TRACE1_1("bRemoveOriginal = %d",pContext->mAudioSettings->bRemoveOriginal);

//         fid = pEnv->GetFieldID(audioSettingClazz,"channels","I");
//         pContext->mAudioSettings->uiNbChannels = pEnv->GetIntField(audioSettingObject,fid);
//         M4OSA_TRACE1_1("uiNbChannels = %d",pContext->mAudioSettings->uiNbChannels);

//         fid = pEnv->GetFieldID(audioSettingClazz,"Fs","I");
//         pContext->mAudioSettings->uiSamplingFrequency = pEnv->GetIntField(audioSettingObject,fid);
//         M4OSA_TRACE1_1("uiSamplingFrequency = %d",pContext->mAudioSettings->uiSamplingFrequency);

//         fid = pEnv->GetFieldID(audioSettingClazz,"ExtendedFs","I");
//         pContext->mAudioSettings->uiExtendedSamplingFrequency =
//          pEnv->GetIntField(audioSettingObject,fid);
//         M4OSA_TRACE1_1("uiExtendedSamplingFrequency = %d",
//         pContext->mAudioSettings->uiExtendedSamplingFrequency);

//         fid = pEnv->GetFieldID(audioSettingClazz,"startMs","J");
//         pContext->mAudioSettings->uiAddCts
//             = pEnv->GetLongField(audioSettingObject,fid);
//         M4OSA_TRACE1_1("uiAddCts = %d",pContext->mAudioSettings->uiAddCts);

//         fid = pEnv->GetFieldID(audioSettingClazz,"volume","I");
//         pContext->mAudioSettings->uiAddVolume
//             = pEnv->GetIntField(audioSettingObject,fid);
//         M4OSA_TRACE1_1("uiAddVolume = %d",pContext->mAudioSettings->uiAddVolume);

//         fid = pEnv->GetFieldID(audioSettingClazz,"loop","Z");
//         pContext->mAudioSettings->bLoop
//             = pEnv->GetBooleanField(audioSettingObject,fid);
//         M4OSA_TRACE1_1("bLoop = %d",pContext->mAudioSettings->bLoop);

//         fid = pEnv->GetFieldID(audioSettingClazz,"beginCutTime","J");
//         pContext->mAudioSettings->beginCutMs
//             = pEnv->GetLongField(audioSettingObject,fid);
//         M4OSA_TRACE1_1("begin cut time = %d",pContext->mAudioSettings->beginCutMs);

//         fid = pEnv->GetFieldID(audioSettingClazz,"endCutTime","J");
//         pContext->mAudioSettings->endCutMs
//             = pEnv->GetLongField(audioSettingObject,fid);
//         M4OSA_TRACE1_1("end cut time = %d",pContext->mAudioSettings->endCutMs);

//         fid = pEnv->GetFieldID(audioSettingClazz,"fileType","I");
//         pContext->mAudioSettings->fileType
//             = pEnv->GetIntField(audioSettingObject,fid);
//         M4OSA_TRACE1_1("fileType = %d",pContext->mAudioSettings->fileType);

//         fid = pEnv->GetFieldID(audioSettingClazz,"pFile","Ljava/lang/String;");
//         strPath = (jstring)pEnv->GetObjectField(audioSettingObject,fid);
//         pTempChar = (M4OSA_Char*)pEnv->GetStringUTFChars(strPath, M4OSA_NULL);
//         if (pTempChar != NULL) {
//             pContext->mAudioSettings->pFile = (M4OSA_Char*) M4OSA_32bitAlignedMalloc(
//                 (M4OSA_UInt32)(strlen((const char*)pTempChar))+1 /* +1 for NULL termination */, 0,
//                 (M4OSA_Char*)"strPath allocation " );
//             if (pContext->mAudioSettings->pFile != M4OSA_NULL) {
//                 memcpy((void *)pContext->mAudioSettings->pFile ,
//                     (void *)pTempChar , strlen((const char*)pTempChar));
//                 ((M4OSA_Int8 *)(pContext->mAudioSettings->pFile))[strlen((const char*)pTempChar)] = '\0';
//                 pEnv->ReleaseStringUTFChars(strPath,(const char *)pTempChar);
//             } else {
//                 pEnv->ReleaseStringUTFChars(strPath,(const char *)pTempChar);
//                 VIDEOEDIT_LOG_ERROR(ANDROID_LOG_INFO, "VIDEO_EDITOR",
//                     "regenerateAudio() Malloc failed for pContext->mAudioSettings->pFile ");
//                 videoEditJava_checkAndThrowRuntimeException(&needToBeLoaded, pEnv,
//                     M4OSA_TRUE, M4ERR_ALLOC);
//                 goto videoEditor_populateSettings_cleanup;
//             }
//         }
//         M4OSA_TRACE1_1("file name = %s",pContext->mAudioSettings->pFile);
//         VIDEOEDIT_LOG_API(ANDROID_LOG_INFO, "VIDEOEDITOR", "regenerateAudio() file name = %s",\
//         pContext->mAudioSettings->pFile);

//         fid = pEnv->GetFieldID(audioSettingClazz,"pcmFilePath","Ljava/lang/String;");
//         strPCMPath = (jstring)pEnv->GetObjectField(audioSettingObject,fid);
//         pTempChar = (M4OSA_Char*)pEnv->GetStringUTFChars(strPCMPath, M4OSA_NULL);
//         if (pTempChar != NULL) {
//             pContext->mAudioSettings->pPCMFilePath = (M4OSA_Char*) M4OSA_32bitAlignedMalloc(
//                 (M4OSA_UInt32)(strlen((const char*)pTempChar))+1 /* +1 for NULL termination */, 0,
//                 (M4OSA_Char*)"strPCMPath allocation " );
//             if (pContext->mAudioSettings->pPCMFilePath != M4OSA_NULL) {
//                 memcpy((void *)pContext->mAudioSettings->pPCMFilePath ,
//                     (void *)pTempChar , strlen((const char*)pTempChar));
//                 ((M4OSA_Int8 *)(pContext->mAudioSettings->pPCMFilePath))[strlen((const char*)pTempChar)] = '\0';
//                 pEnv->ReleaseStringUTFChars(strPCMPath,(const char *)pTempChar);
//             } else {
//                 pEnv->ReleaseStringUTFChars(strPCMPath,(const char *)pTempChar);
//                 VIDEOEDIT_LOG_ERROR(ANDROID_LOG_INFO, "VIDEO_EDITOR",
//                     "regenerateAudio() Malloc failed for pContext->mAudioSettings->pPCMFilePath ");
//                 videoEditJava_checkAndThrowRuntimeException(&needToBeLoaded, pEnv,
//                     M4OSA_TRUE, M4ERR_ALLOC);
//                 goto videoEditor_populateSettings_cleanup;
//             }
//         }
//         VIDEOEDIT_LOG_API(ANDROID_LOG_INFO, "VIDEOEDITOR", "pPCMFilePath -- %s ",\
//         pContext->mAudioSettings->pPCMFilePath);

//         fid = pEnv->GetFieldID(engineClass,"mRegenerateAudio","Z");
//         bool regenerateAudio = pEnv->GetBooleanField(thiz,fid);

//         VIDEOEDIT_LOG_API(ANDROID_LOG_INFO, "VIDEOEDITOR", "regenerateAudio -- %d ",\
//         regenerateAudio);

//         if (regenerateAudio) {
//             M4OSA_TRACE1_0("Calling Generate Audio now");
//             result = videoEditor_generateAudio(pEnv,
//                         pContext,
//                         (M4OSA_Char*)pContext->mAudioSettings->pFile,
//                         (M4OSA_Char*)pContext->mAudioSettings->pPCMFilePath);

//             videoEditJava_checkAndThrowRuntimeException(&needToBeLoaded, pEnv,
//                 (M4NO_ERROR != result), result);
//             if (needToBeLoaded == FALSE) {
//                 goto videoEditor_populateSettings_cleanup;
//             }

//             regenerateAudio = FALSE;
//             pEnv->SetBooleanField(thiz,fid,regenerateAudio);
//         }

//         /* Audio mix and duck */
//         fid = pEnv->GetFieldID(audioSettingClazz,"ducking_threshold","I");
//         pContext->mAudioSettings->uiInDucking_threshold
//             = pEnv->GetIntField(audioSettingObject,fid);

//         M4OSA_TRACE1_1("ducking threshold = %d",
//             pContext->mAudioSettings->uiInDucking_threshold);

//         fid = pEnv->GetFieldID(audioSettingClazz,"ducking_lowVolume","I");
//         pContext->mAudioSettings->uiInDucking_lowVolume
//             = pEnv->GetIntField(audioSettingObject,fid);

//         M4OSA_TRACE1_1("ducking lowVolume = %d",
//             pContext->mAudioSettings->uiInDucking_lowVolume);

//         fid = pEnv->GetFieldID(audioSettingClazz,"bInDucking_enable","Z");
//         pContext->mAudioSettings->bInDucking_enable
//             = pEnv->GetBooleanField(audioSettingObject,fid);
//         M4OSA_TRACE1_1("ducking lowVolume = %d",
//             pContext->mAudioSettings->bInDucking_enable);

//     } else {
//         if (pContext->mAudioSettings != M4OSA_NULL) {
//             pContext->mAudioSettings->pFile = M4OSA_NULL;
//             pContext->mAudioSettings->pPCMFilePath = M4OSA_NULL;
//             pContext->mAudioSettings->bRemoveOriginal = 0;
//             pContext->mAudioSettings->uiNbChannels = 0;
//             pContext->mAudioSettings->uiSamplingFrequency = 0;
//             pContext->mAudioSettings->uiExtendedSamplingFrequency = 0;
//             pContext->mAudioSettings->uiAddCts = 0;
//             pContext->mAudioSettings->uiAddVolume = 0;
//             pContext->mAudioSettings->beginCutMs = 0;
//             pContext->mAudioSettings->endCutMs = 0;
//             pContext->mAudioSettings->fileType = 0;
//             pContext->mAudioSettings->bLoop = 0;
//             pContext->mAudioSettings->uiInDucking_lowVolume  = 0;
//             pContext->mAudioSettings->bInDucking_enable  = 0;
//             pContext->mAudioSettings->uiBTChannelCount  = 0;
//             pContext->mAudioSettings->uiInDucking_threshold = 0;

//             fid = pEnv->GetFieldID(engineClass,"mRegenerateAudio","Z");
//             bool regenerateAudio = pEnv->GetBooleanField(thiz,fid);
//             if (!regenerateAudio) {
//                 regenerateAudio = TRUE;
//                 pEnv->SetBooleanField(thiz,fid,regenerateAudio);
//             }
//         }
//     }

//     if (pContext->pEditSettings != NULL)
//     {
//         result = pContext->mPreviewController->loadEditSettings(pContext->pEditSettings,
//             pContext->mAudioSettings);
//         videoEditJava_checkAndThrowRuntimeException(&needToBeLoaded, pEnv,
//                                             (M4NO_ERROR != result), result);

//         if (needToBeLoaded) {
//             pContext->mPreviewController->setJniCallback((void*)pContext,
//             (jni_progress_callback_fct)jniPreviewProgressCallback);
//         }
//     }

// videoEditor_populateSettings_cleanup:
//         j = 0;
//         while (j < nbOverlays)
//         {
//             if (pContext->pEditSettings->Effects[pOverlayIndex[j]].xVSS.pFramingBuffer->pac_data != \
//                 M4OSA_NULL) {
//                 free(pContext->pEditSettings->\
//                 Effects[pOverlayIndex[j]].xVSS.pFramingBuffer->pac_data);
//                 pContext->pEditSettings->\
//                 Effects[pOverlayIndex[j]].xVSS.pFramingBuffer->pac_data = M4OSA_NULL;
//             }
//             j++;
//         }

//         j = 0;
//         while (j < pContext->pEditSettings->nbEffects)
//         {
//             if (pContext->pEditSettings->Effects[j].xVSS.pFramingFilePath != M4OSA_NULL) {
//                 if (pContext->pEditSettings->Effects[j].xVSS.pFramingBuffer != M4OSA_NULL) {
//                     free(pContext->pEditSettings->\
//                     Effects[j].xVSS.pFramingBuffer);
//                     pContext->pEditSettings->Effects[j].xVSS.pFramingBuffer = M4OSA_NULL;
//                 }
//             }
//           j++;
//         }

//     if (pOverlayIndex != M4OSA_NULL)
//     {
//         free(pOverlayIndex);
//         pOverlayIndex = M4OSA_NULL;
//     }
//     return;
}

Int32 MediaArtistNativeHelper::NativeRenderPreviewFrame(
    /* [in] */ ISurface* mSurface,
    /* [in] */ Int64 timeMs,
    /* [in] */ Int32 surfaceWidth,
    /* [in] */ Int32 surfaceHeight)
{
//     bool needToBeLoaded = TRUE;
//     M4OSA_ERR result = M4NO_ERROR;
//     M4OSA_UInt32 timeMs = (M4OSA_UInt32)fromMs;
//     M4OSA_UInt32 i=0,tnTimeMs = 0, framesizeYuv =0;
//     M4VIFI_UInt8 *pixelArray = M4OSA_NULL;
//     M4OSA_UInt32    iCurrentClipIndex = 0, uiNumberOfClipsInStoryBoard =0,
//                     uiClipDuration = 0, uiTotalClipDuration = 0,
//                     iIncrementedDuration = 0;
//     VideoEditor_renderPreviewFrameStr frameStr;
//     M4OSA_Context tnContext = M4OSA_NULL;
//     const char* pMessage = NULL;
//     M4VIFI_ImagePlane *yuvPlane = NULL;
//     VideoEditorCurretEditInfo  currEditInfo;

//     VIDEOEDIT_LOG_FUNCTION(ANDROID_LOG_INFO,
//         "VIDEO_EDITOR", "surfaceWidth = %d",surfaceWidth);
//     VIDEOEDIT_LOG_FUNCTION(ANDROID_LOG_INFO,
//         "VIDEO_EDITOR", "surfaceHeight = %d",surfaceHeight);
//     ManualEditContext* pContext = M4OSA_NULL;
//     // Get the context.
//     pContext =
//             (ManualEditContext*)videoEditClasses_getContext(&needToBeLoaded, pEnv, thiz);
//     VIDEOEDIT_LOG_FUNCTION(ANDROID_LOG_INFO,
//                                 "VIDEO_EDITOR","pContext = 0x%x",pContext);

//     // Make sure that the context was set.
//     videoEditJava_checkAndThrowIllegalStateException(&needToBeLoaded, pEnv,
//                                              (M4OSA_NULL == pContext),
//                                              "not initialized");

//     // Make sure that the context was set.
//     videoEditJava_checkAndThrowIllegalStateException(&needToBeLoaded, pEnv,
//                                  (M4OSA_NULL == pContext->mPreviewController),
//                                  "not initialized");

//     // Validate the mSurface parameter.
//     videoEditJava_checkAndThrowIllegalArgumentException(&needToBeLoaded, pEnv,
//                                                 (NULL == mSurface),
//                                                 "mSurface is NULL");
//     jclass surfaceClass = pEnv->FindClass("android/view/Surface");
//     videoEditJava_checkAndThrowIllegalStateException(&needToBeLoaded, pEnv,
//                                              (M4OSA_NULL == surfaceClass),
//                                              "not initialized");

//     jfieldID surface_native =
//             pEnv->GetFieldID(surfaceClass, ANDROID_VIEW_SURFACE_JNI_ID, "I");
//     videoEditJava_checkAndThrowIllegalStateException(&needToBeLoaded, pEnv,
//                                              (M4OSA_NULL == surface_native),
//                                              "not initialized");

//     Surface* const p = (Surface*)pEnv->GetIntField(mSurface, surface_native);
//     sp<Surface> previewSurface = sp<Surface>(p);
//     // Validate the mSurface's mNativeSurface field
//     videoEditJava_checkAndThrowIllegalStateException(&needToBeLoaded, pEnv,
//                                                 (NULL == previewSurface.get()),
//                                                 "mNativeSurface is NULL");

//     /* Determine the total number of clips, total duration*/
//     uiNumberOfClipsInStoryBoard = pContext->pEditSettings->uiClipNumber;

//     for (i = 0; i < uiNumberOfClipsInStoryBoard; i++) {
//         uiClipDuration = pContext->pEditSettings->pClipList[i]->uiEndCutTime -
//             pContext->pEditSettings->pClipList[i]->uiBeginCutTime;
//         uiTotalClipDuration += uiClipDuration;
//     }

//     /* determine the clip whose thumbnail needs to be rendered*/
//     if (timeMs == 0) {
//         iCurrentClipIndex = 0;
//         i=0;
//     } else {
//         VIDEOEDIT_LOG_FUNCTION(ANDROID_LOG_INFO, "VIDEO_EDITOR",
//             "videoEditor_renderPreviewFrame() timeMs=%d", timeMs);

//         if (timeMs > uiTotalClipDuration) {
//             VIDEOEDIT_LOG_FUNCTION(ANDROID_LOG_INFO, "VIDEO_EDITOR",
//                 "videoEditor_renderPreviewFrame() timeMs > uiTotalClipDuration");
//             pMessage = videoEditJava_getErrorName(M4ERR_PARAMETER);
//             jniThrowException(pEnv, "java/lang/IllegalArgumentException", pMessage);
//             return -1;
//         }

//         for (i = 0; i < uiNumberOfClipsInStoryBoard; i++) {
//             if (timeMs <= (iIncrementedDuration +
//                           (pContext->pEditSettings->pClipList[i]->uiEndCutTime -
//                            pContext->pEditSettings->pClipList[i]->uiBeginCutTime)))
//             {
//                 iCurrentClipIndex = i;
//                 VIDEOEDIT_LOG_FUNCTION(ANDROID_LOG_INFO, "VIDEO_EDITOR",
//                     "videoEditor_renderPreviewFrame() iCurrentClipIndex=%d for timeMs=%d",
//                     iCurrentClipIndex, timeMs);
//                 break;
//             }
//             else {
//                 iIncrementedDuration = iIncrementedDuration +
//                     (pContext->pEditSettings->pClipList[i]->uiEndCutTime -
//                     pContext->pEditSettings->pClipList[i]->uiBeginCutTime);
//             }
//         }
//     }
//     /* If timestamp is beyond story board duration, return*/
//     if (i >= uiNumberOfClipsInStoryBoard) {
//         if (timeMs == iIncrementedDuration) {
//             iCurrentClipIndex = i-1;
//         } else {
//            return -1;
//         }
//     }

//     /*+ Handle the image files here */
//       if (pContext->pEditSettings->pClipList[iCurrentClipIndex]->FileType ==
//           /*M4VIDEOEDITING_kFileType_JPG*/ M4VIDEOEDITING_kFileType_ARGB8888 ) {
//           VIDEOEDIT_LOG_FUNCTION(ANDROID_LOG_INFO, "VIDEO_EDITOR", " iCurrentClipIndex %d ", iCurrentClipIndex);
//           VIDEOEDIT_LOG_FUNCTION(ANDROID_LOG_INFO, "VIDEO_EDITOR",
//                 "  Height = %d",
//                 pContext->pEditSettings->pClipList[iCurrentClipIndex]->ClipProperties.uiVideoHeight);

//           VIDEOEDIT_LOG_FUNCTION(ANDROID_LOG_INFO, "VIDEO_EDITOR",
//                 "  Width = %d",
//                 pContext->pEditSettings->pClipList[iCurrentClipIndex]->ClipProperties.uiVideoWidth);

//           LvGetImageThumbNail((const char *)pContext->pEditSettings->\
//           pClipList[iCurrentClipIndex]->pFile,
//             pContext->pEditSettings->pClipList[iCurrentClipIndex]->ClipProperties.uiVideoHeight,
//             pContext->pEditSettings->pClipList[iCurrentClipIndex]->ClipProperties.uiVideoWidth,
//             (M4OSA_Void **)&frameStr.pBuffer);
//             tnTimeMs = (M4OSA_UInt32)timeMs;

//           frameStr.videoRotationDegree = 0;
//     } else {
//         /* Handle 3gp/mp4 Clips here */
//         /* get thumbnail*/
//         result = ThumbnailOpen(&tnContext,
//             (const M4OSA_Char*)pContext->pEditSettings->\
//             pClipList[iCurrentClipIndex]->pFile, M4OSA_TRUE);
//         if (result != M4NO_ERROR || tnContext  == M4OSA_NULL) {
//             return -1;
//         }

//         /* timeMs is relative to storyboard; in this api it shud be relative to this clip */
//         if ((i >= uiNumberOfClipsInStoryBoard) &&
//             (timeMs == iIncrementedDuration)) {
//             tnTimeMs = pContext->pEditSettings->\
//             pClipList[iCurrentClipIndex]->uiEndCutTime;
//         } else {
//             tnTimeMs = pContext->pEditSettings->\
//             pClipList[iCurrentClipIndex]->uiBeginCutTime
//             + (timeMs - iIncrementedDuration);
//         }

//         VIDEOEDIT_LOG_API(ANDROID_LOG_INFO, "VIDEO_EDITOR",
//             "video width = %d",pContext->pEditSettings->pClipList[iCurrentClipIndex]->\
//             ClipProperties.uiVideoWidth);
//         VIDEOEDIT_LOG_API(ANDROID_LOG_INFO, "VIDEO_EDITOR",
//             "video height = %d",pContext->pEditSettings->pClipList[iCurrentClipIndex]->\
//             ClipProperties.uiVideoHeight);
//         VIDEOEDIT_LOG_API(ANDROID_LOG_INFO, "VIDEO_EDITOR",
//             "current clip index = %d",iCurrentClipIndex);

//         M4OSA_UInt32 width = pContext->pEditSettings->pClipList[iCurrentClipIndex]->\
//             ClipProperties.uiVideoWidth;
//         M4OSA_UInt32 height = pContext->pEditSettings->pClipList[iCurrentClipIndex]->\
//             ClipProperties.uiVideoHeight;

//         framesizeYuv = width * height * 1.5;

//         pixelArray = (M4VIFI_UInt8 *)M4OSA_32bitAlignedMalloc(framesizeYuv, M4VS,
//             (M4OSA_Char*)"videoEditor pixelArray");
//         if (pixelArray == M4OSA_NULL) {
//             VIDEOEDIT_LOG_FUNCTION(ANDROID_LOG_INFO, "VIDEO_EDITOR",
//                 "videoEditor_renderPreviewFrame() malloc error");
//             ThumbnailClose(tnContext);
//             pMessage = videoEditJava_getErrorName(M4ERR_ALLOC);
//             jniThrowException(pEnv, "java/lang/RuntimeException", pMessage);
//             return -1;
//         }

//         result = ThumbnailGetPixels16(tnContext, (M4OSA_Int16 *)pixelArray,
//             pContext->pEditSettings->pClipList[iCurrentClipIndex]->\
//             ClipProperties.uiVideoWidth,
//             pContext->pEditSettings->pClipList[iCurrentClipIndex]->\
//             ClipProperties.uiVideoHeight,
//             &tnTimeMs, 0);
//         if (result != M4NO_ERROR) {
//             free(pixelArray);
//             ThumbnailClose(tnContext);
//             return -1;
//         }

//         ThumbnailClose(tnContext);
//         tnContext = M4OSA_NULL;

// #ifdef DUMPTOFILE
//         {
//             M4OSA_Context fileContext;
//             M4OSA_Char* fileName = (M4OSA_Char*)"/mnt/sdcard/FirstRGB565.raw";
//             remove((const char *)fileName);
//             M4OSA_fileWriteOpen(&fileContext, (M4OSA_Void*) fileName,\
//                 M4OSA_kFileWrite|M4OSA_kFileCreate);
//             M4OSA_fileWriteData(fileContext, (M4OSA_MemAddr8) pixelArray,
//                 framesizeYuv);
//             M4OSA_fileWriteClose(fileContext);
//         }
// #endif

//         /**
//         * Allocate output YUV planes
//         */
//         yuvPlane = (M4VIFI_ImagePlane*)M4OSA_32bitAlignedMalloc(3*sizeof(M4VIFI_ImagePlane), M4VS,
//             (M4OSA_Char*)"videoEditor_renderPreviewFrame Output plane YUV");
//         if (yuvPlane == M4OSA_NULL) {
//             VIDEOEDIT_LOG_FUNCTION(ANDROID_LOG_INFO, "VIDEO_EDITOR",
//                 "videoEditor_renderPreviewFrame() malloc error for yuv plane");
//             free(pixelArray);
//             pMessage = videoEditJava_getErrorName(M4ERR_ALLOC);
//             jniThrowException(pEnv, "java/lang/RuntimeException", pMessage);
//             return -1;
//         }

//         yuvPlane[0].u_width = width;
//         yuvPlane[0].u_height = height;
//         yuvPlane[0].u_topleft = 0;
//         yuvPlane[0].u_stride = width;
//         yuvPlane[0].pac_data = (M4VIFI_UInt8*)pixelArray;

//         yuvPlane[1].u_width = width>>1;
//         yuvPlane[1].u_height = height>>1;
//         yuvPlane[1].u_topleft = 0;
//         yuvPlane[1].u_stride = width>>1;
//         yuvPlane[1].pac_data = yuvPlane[0].pac_data
//                     + yuvPlane[0].u_width * yuvPlane[0].u_height;
//         yuvPlane[2].u_width = (width)>>1;
//         yuvPlane[2].u_height = (height)>>1;
//         yuvPlane[2].u_topleft = 0;
//         yuvPlane[2].u_stride = (width)>>1;
//         yuvPlane[2].pac_data = yuvPlane[1].pac_data
//                     + yuvPlane[1].u_width * yuvPlane[1].u_height;

// #ifdef DUMPTOFILE
//         {
//             M4OSA_Context fileContext;
//             M4OSA_Char* fileName = (M4OSA_Char*)"/mnt/sdcard/ConvertedYuv.yuv";
//             remove((const char *)fileName);
//             M4OSA_fileWriteOpen(&fileContext, (M4OSA_Void*) fileName,\
//                 M4OSA_kFileWrite|M4OSA_kFileCreate);
//             M4OSA_fileWriteData(fileContext,
//                 (M4OSA_MemAddr8) yuvPlane[0].pac_data, framesizeYuv);
//             M4OSA_fileWriteClose(fileContext);
//         }
// #endif

//         /* Fill up the render structure*/
//         frameStr.pBuffer = (M4OSA_Void*)yuvPlane[0].pac_data;

//         frameStr.videoRotationDegree = pContext->pEditSettings->\
//             pClipList[iCurrentClipIndex]->ClipProperties.videoRotationDegrees;
//     }

//     frameStr.timeMs = timeMs;    /* timestamp on storyboard*/
//     frameStr.uiSurfaceWidth =
//         pContext->pEditSettings->pClipList[iCurrentClipIndex]->\
//         ClipProperties.uiVideoWidth;
//     frameStr.uiSurfaceHeight =
//         pContext->pEditSettings->pClipList[iCurrentClipIndex]->\
//         ClipProperties.uiVideoHeight;
//     frameStr.uiFrameWidth =
//         pContext->pEditSettings->pClipList[iCurrentClipIndex]->\
//         ClipProperties.uiVideoWidth;
//     frameStr.uiFrameHeight =
//         pContext->pEditSettings->pClipList[iCurrentClipIndex]->\
//         ClipProperties.uiVideoHeight;
//     if (pContext->pEditSettings->nbEffects > 0) {
//         frameStr.bApplyEffect = M4OSA_TRUE;
//     } else {
//         frameStr.bApplyEffect = M4OSA_FALSE;
//     }
//     frameStr.clipBeginCutTime = iIncrementedDuration;
//     frameStr.clipEndCutTime =
//         iIncrementedDuration +
//         (pContext->pEditSettings->pClipList[iCurrentClipIndex]->uiEndCutTime -\
//         pContext->pEditSettings->pClipList[iCurrentClipIndex]->uiBeginCutTime);

//     pContext->mPreviewController->setPreviewFrameRenderingMode(
//         pContext->pEditSettings->\
//         pClipList[iCurrentClipIndex]->xVSS.MediaRendering,
//         pContext->pEditSettings->xVSS.outputVideoSize);
//     result = pContext->mPreviewController->renderPreviewFrame(previewSurface,
//                                                               &frameStr, &currEditInfo);

//     if (currEditInfo.overlaySettingsIndex != -1) {
//         char tmpOverlayFilename[100];
//         char *extPos = NULL;
//         jstring tmpOverlayString;
//         int tmpRenderingMode = 0;

//         strncpy (tmpOverlayFilename,
//                 (const char*)pContext->pEditSettings->Effects[currEditInfo.overlaySettingsIndex].xVSS.pFramingFilePath, 99);

//         //Change the name to png file
//         extPos = strstr(tmpOverlayFilename, ".rgb");
//         if (extPos != NULL) {
//             *extPos = '\0';
//         } else {
//             ALOGE("ERROR the overlay file is incorrect");
//         }

//         strcat(tmpOverlayFilename, ".png");

//         tmpRenderingMode = pContext->pEditSettings->pClipList[iCurrentClipIndex]->xVSS.MediaRendering;
//         tmpOverlayString = pEnv->NewStringUTF(tmpOverlayFilename);
//         pEnv->CallVoidMethod(pContext->engine,
//             pContext->previewFrameEditInfoId,
//             tmpOverlayString, tmpRenderingMode);

//     }

//     videoEditJava_checkAndThrowRuntimeException(&needToBeLoaded, pEnv,
//             (M4NO_ERROR != result), result);

//     free(frameStr.pBuffer);
//     if (pContext->pEditSettings->pClipList[iCurrentClipIndex]->FileType !=
//             M4VIDEOEDITING_kFileType_ARGB8888) {
//         free(yuvPlane);
//     }

//     return tnTimeMs;
    return 0;
}

Int32 MediaArtistNativeHelper::NativeRenderMediaItemPreviewFrame(
    /* [in] */ ISurface* mSurface,
    /* [in] */ const String& filepath,
    /* [in] */ Int32 framewidth,
    /* [in] */ Int32 frameheight,
    /* [in] */ Int32 surfacewidth,
    /* [in] */ Int32 surfaceheight,
    /* [in] */ Int64 timeMs)
{
//     bool needToBeLoaded = TRUE;
//     M4OSA_ERR result = M4NO_ERROR;
//     M4OSA_UInt32 timeMs = (M4OSA_UInt32)fromMs;
//     M4OSA_UInt32 framesizeYuv =0;
//     M4VIFI_UInt8 *pixelArray = M4OSA_NULL;
//     VideoEditor_renderPreviewFrameStr frameStr;
//     M4OSA_Context tnContext = M4OSA_NULL;
//     const char* pMessage = NULL;
//     M4VIFI_ImagePlane yuvPlane[3], rgbPlane;

//     ManualEditContext* pContext = M4OSA_NULL;
//     // Get the context.
//     pContext =
//             (ManualEditContext*)videoEditClasses_getContext(&needToBeLoaded,
//                                                       pEnv, thiz);

//     // Make sure that the context was set.
//     videoEditJava_checkAndThrowIllegalStateException(&needToBeLoaded, pEnv,
//                                              (M4OSA_NULL == pContext),
//                                              "not initialized");

//     // Make sure that the context was set.
//     videoEditJava_checkAndThrowIllegalStateException(&needToBeLoaded, pEnv,
//                                  (M4OSA_NULL == pContext->mPreviewController),
//                                  "not initialized");

//     // Validate the mSurface parameter.
//     videoEditJava_checkAndThrowIllegalArgumentException(&needToBeLoaded, pEnv,
//                                                 (NULL == mSurface),
//                                                 "mSurface is NULL");
//     jclass surfaceClass = pEnv->FindClass("android/view/Surface");
//     videoEditJava_checkAndThrowIllegalStateException(&needToBeLoaded, pEnv,
//                                              (M4OSA_NULL == surfaceClass),
//                                              "not initialized");

//     jfieldID surface_native =
//             pEnv->GetFieldID(surfaceClass, ANDROID_VIEW_SURFACE_JNI_ID, "I");
//     videoEditJava_checkAndThrowIllegalStateException(&needToBeLoaded, pEnv,
//                                              (M4OSA_NULL == surface_native),
//                                              "not initialized");

//     Surface* const p = (Surface*)pEnv->GetIntField(mSurface, surface_native);
//     sp<Surface> previewSurface = sp<Surface>(p);


//     const char *pString = pEnv->GetStringUTFChars(filePath, NULL);
//     if (pString == M4OSA_NULL) {
//         if (pEnv != NULL) {
//             jniThrowException(pEnv, "java/lang/RuntimeException", "Input string NULL");
//         }
//     }
//     VIDEOEDIT_LOG_FUNCTION(ANDROID_LOG_INFO, "VIDEO_EDITOR",
//         "videoEditor_renderMediaItemPreviewFrame() timeMs=%d", timeMs);
//     /* get thumbnail*/
//     result = ThumbnailOpen(&tnContext,(const M4OSA_Char*)pString, M4OSA_TRUE);
//     if (result != M4NO_ERROR || tnContext  == M4OSA_NULL) {
//         return timeMs;
//     }

//     framesizeYuv = ((frameWidth)*(frameHeight)*1.5);

//     pixelArray = (M4VIFI_UInt8 *)M4OSA_32bitAlignedMalloc(framesizeYuv, M4VS,\
//         (M4OSA_Char*)"videoEditor pixelArray");
//     if (pixelArray == M4OSA_NULL) {
//         VIDEOEDIT_LOG_FUNCTION(ANDROID_LOG_INFO, "VIDEO_EDITOR",
//             "videoEditor_renderPreviewFrame() malloc error");
//         ThumbnailClose(tnContext);
//         pMessage = videoEditJava_getErrorName(M4ERR_ALLOC);
//         jniThrowException(pEnv, "java/lang/RuntimeException", pMessage);
//         return timeMs;
//     }

//     result = ThumbnailGetPixels16(tnContext, (M4OSA_Int16 *)pixelArray,
//                                                 frameWidth,
//                                                 frameHeight, &timeMs, 0);
//     if (result != M4NO_ERROR) {
//         free(pixelArray);
//         ThumbnailClose(tnContext);
//         return fromMs;
//     }

// #ifdef DUMPTOFILESYSTEM
//     {
//         M4OSA_Context fileContext;
//         M4OSA_Char* fileName = (M4OSA_Char*)"/mnt/sdcard/FirstRGB565.rgb";
//         M4OSA_fileWriteOpen(&fileContext, (M4OSA_Void*) fileName,\
//             M4OSA_kFileWrite|M4OSA_kFileCreate);
//         M4OSA_fileWriteData(fileContext, (M4OSA_MemAddr8) pixelArray,
//                             framesizeRgb);
//         M4OSA_fileWriteClose(fileContext);
//     }
// #endif

//     yuvPlane[0].pac_data = (M4VIFI_UInt8*)pixelArray;
//     yuvPlane[0].u_height = frameHeight;
//     yuvPlane[0].u_width = frameWidth;
//     yuvPlane[0].u_stride = yuvPlane[0].u_width;
//     yuvPlane[0].u_topleft = 0;

//     yuvPlane[1].u_height = frameHeight/2;
//     yuvPlane[1].u_width = frameWidth/2;
//     yuvPlane[1].u_stride = yuvPlane[1].u_width;
//     yuvPlane[1].u_topleft = 0;
//     yuvPlane[1].pac_data = yuvPlane[0].pac_data
//                 + yuvPlane[0].u_width*yuvPlane[0].u_height;

//     yuvPlane[2].u_height = frameHeight/2;
//     yuvPlane[2].u_width = frameWidth/2;
//     yuvPlane[2].u_stride = yuvPlane[2].u_width;
//     yuvPlane[2].u_topleft = 0;
//     yuvPlane[2].pac_data = yuvPlane[0].pac_data
//         + yuvPlane[0].u_width*yuvPlane[0].u_height + \
//         (yuvPlane[0].u_width/2)*(yuvPlane[0].u_height/2);
// #ifdef DUMPTOFILESYSTEM
//     {
//         M4OSA_Context fileContext;
//         M4OSA_Char* fileName = (M4OSA_Char*)"/mnt/sdcard/ConvertedYuv.yuv";
//         M4OSA_fileWriteOpen(&fileContext, (M4OSA_Void*) fileName,\
//             M4OSA_kFileWrite|M4OSA_kFileCreate);
//         M4OSA_fileWriteData(fileContext, (M4OSA_MemAddr8) yuvPlane[0].pac_data,
//                             framesizeYuv);
//         M4OSA_fileWriteClose(fileContext);
//     }
// #endif

//     /* Fill up the render structure*/
//     frameStr.pBuffer = (M4OSA_Void*)yuvPlane[0].pac_data;
//     frameStr.timeMs = timeMs;    /* timestamp on storyboard*/
//     frameStr.uiSurfaceWidth = frameWidth;
//     frameStr.uiSurfaceHeight = frameHeight;
//     frameStr.uiFrameWidth = frameWidth;
//     frameStr.uiFrameHeight = frameHeight;
//     frameStr.bApplyEffect = M4OSA_FALSE;
//     // clip begin cuttime and end cuttime set to 0
//     // as its only required when effect needs to be applied while rendering
//     frameStr.clipBeginCutTime = 0;
//     frameStr.clipEndCutTime = 0;

//     /*  pContext->mPreviewController->setPreviewFrameRenderingMode(M4xVSS_kBlackBorders,
//     (M4VIDEOEDITING_VideoFrameSize)(M4VIDEOEDITING_kHD960+1));*/
//     result
//     = pContext->mPreviewController->renderPreviewFrame(previewSurface,&frameStr, NULL);
//     videoEditJava_checkAndThrowRuntimeException(&needToBeLoaded, pEnv,
//                                                 (M4NO_ERROR != result), result);

//     /* free the pixelArray and yuvPlane[0].pac_data */
//     free(yuvPlane[0].pac_data);

//     ThumbnailClose(tnContext);

//     if (pString != NULL) {
//         pEnv->ReleaseStringUTFChars(filePath, pString);
//     }

//     return timeMs;
    return 0;
}

Int32 MediaArtistNativeHelper::NativeStopPreview()
{
    // ManualEditContext* pContext = M4OSA_NULL;
    // bool needToBeLoaded = TRUE;
    // M4OSA_UInt32 lastProgressTimeMs = 0;

    // // Get the context.
    // pContext =
    //         (ManualEditContext*)videoEditClasses_getContext(&needToBeLoaded, pEnv, thiz);

    // // Make sure that the context was set.
    // videoEditJava_checkAndThrowIllegalStateException(&needToBeLoaded, pEnv,
    //                                          (M4OSA_NULL == pContext),
    //                                          "not initialized");
    // lastProgressTimeMs = pContext->mPreviewController->stopPreview();

    // if (pContext->mOverlayFileName != NULL) {
    //     free(pContext->mOverlayFileName);
    //     pContext->mOverlayFileName = NULL;
    // }

    // return lastProgressTimeMs;
    return 0;
}

Int32 MediaArtistNativeHelper::NativeGenerateAudioGraph(
    /* [in] */ const String& pcmFilePath,
    /* [in] */ const String& outGraphPath,
    /* [in] */ Int32 frameDuration,
    /* [in] */ Int32 channels,
    /* [in] */ Int32 sampleCount)
{
    // M4OSA_ERR result = M4NO_ERROR;
    // ManualEditContext* pContext = M4OSA_NULL;
    // bool needToBeLoaded = TRUE;
    // const char *pPCMFilePath, *pStringOutAudioGraphFile;

    // VIDEOEDIT_LOG_API(ANDROID_LOG_INFO, "VIDEO_EDITOR",
    //     "videoEditor_generateAudioWaveFormSync() ");

    // /* Get the context. */
    // pContext = (ManualEditContext*)videoEditClasses_getContext(&needToBeLoaded, pEnv, thiz);
    // if (pContext == M4OSA_NULL) {
    //     VIDEOEDIT_LOG_API(ANDROID_LOG_INFO, "VIDEO_EDITOR",
    //         "videoEditor_generateAudioWaveFormSync() - pContext is NULL ");
    // }

    // VIDEOEDIT_LOG_API(ANDROID_LOG_INFO, "VIDEO_EDITOR",
    //     "videoEditor_generateAudioWaveFormSync Retrieving pStringOutAudioGraphFile");

    // pPCMFilePath = pEnv->GetStringUTFChars(pcmfilePath, NULL);
    // if (pPCMFilePath == M4OSA_NULL) {
    //     jniThrowException(pEnv, "java/lang/RuntimeException",
    //         "Input string PCMFilePath is NULL");
    //     result = M4ERR_PARAMETER;
    //     goto out;
    // }

    // pStringOutAudioGraphFile = pEnv->GetStringUTFChars(outGraphfilePath, NULL);
    // if (pStringOutAudioGraphFile == M4OSA_NULL) {
    //     jniThrowException(pEnv, "java/lang/RuntimeException",
    //         "Input string outGraphfilePath is NULL");
    //     result = M4ERR_PARAMETER;
    //     goto out2;
    // }

    // VIDEOEDIT_LOG_API(ANDROID_LOG_INFO, "VIDEO_EDITOR",
    //     "videoEditor_generateAudioWaveFormSync Generate the waveform data %s %d %d %d",
    //     pStringOutAudioGraphFile, frameDuration, channels, samplesCount);

    // /* Generate the waveform */
    // result = M4MA_generateAudioGraphFile(pEnv, (M4OSA_Char*) pPCMFilePath,
    //     (M4OSA_Char*) pStringOutAudioGraphFile,
    //     (M4OSA_UInt32) samplesCount,
    //     (M4OSA_UInt32) channels,
    //     (M4OSA_UInt32)frameDuration,
    //     pContext);

    // pEnv->ReleaseStringUTFChars(outGraphfilePath, pStringOutAudioGraphFile);

    // out2:
    // if (pPCMFilePath != NULL) {
    //     pEnv->ReleaseStringUTFChars(pcmfilePath, pPCMFilePath);
    // }

    // out:
    // VIDEOEDIT_LOG_FUNCTION(ANDROID_LOG_INFO, "VIDEO_EDITOR",
    //     "videoEditor_generateAudioWaveFormSync pContext->bSkipState ");

    // return result;
    return 0;
}

Int32 MediaArtistNativeHelper::NativeGenerateRawAudio(
    /* [in] */ const String& InFileName,
    /* [in] */ const String& PCMFileName)
{
    // M4OSA_ERR result = M4NO_ERROR;
    // bool               loaded   = TRUE;
    // ManualEditContext* pContext = M4OSA_NULL;



    // const char *pInputFile = pEnv->GetStringUTFChars(infilePath, NULL);
    // if (pInputFile == M4OSA_NULL) {
    //     if (pEnv != NULL) {
    //         jniThrowException(pEnv, "java/lang/RuntimeException", "Input string NULL");
    //     }
    // }

    // const char *pStringOutPCMFilePath = pEnv->GetStringUTFChars(pcmfilePath, NULL);
    // if (pStringOutPCMFilePath == M4OSA_NULL) {
    //     if (pEnv != NULL) {
    //         jniThrowException(pEnv, "java/lang/RuntimeException", "Input string NULL");
    //     }
    // }

    // VIDEOEDIT_LOG_FUNCTION(ANDROID_LOG_INFO,
    //     "VIDEO_EDITOR", "videoEditor_generateAudioRawFile infilePath %s",
    //     pInputFile);
    // VIDEOEDIT_LOG_FUNCTION(ANDROID_LOG_INFO,
    //     "VIDEO_EDITOR", "videoEditor_generateAudioRawFile pcmfilePath %s",
    //     pStringOutPCMFilePath);
    // // Get the context.
    // pContext = (ManualEditContext*)videoEditClasses_getContext(&loaded, pEnv, thiz);

    // result = videoEditor_generateAudio( pEnv, pContext, (M4OSA_Char*)pInputFile,
    //     (M4OSA_Char*)pStringOutPCMFilePath);

    // if (pInputFile != NULL) {
    //     pEnv->ReleaseStringUTFChars(infilePath, pInputFile);
    // }
    // if (pStringOutPCMFilePath != NULL) {
    //     pEnv->ReleaseStringUTFChars(pcmfilePath, pStringOutPCMFilePath);
    // }

    // return result;
    return 0;
}

Int32 MediaArtistNativeHelper::NativeGenerateClip(
    /* [in] */ EditSettings* editSettings)
{
    // bool               loaded   = TRUE;
    // ManualEditContext* pContext = M4OSA_NULL;
    // M4OSA_ERR          result   = M4NO_ERROR;

    // ALOGV("videoEditor_generateClip START");

    // // Get the context.
    // pContext = (ManualEditContext*)videoEditClasses_getContext(&loaded, pEnv, thiz);

    // AutoLock autoLock(pContext->mLock);

    // // Validate the settings parameter.
    // videoEditJava_checkAndThrowIllegalArgumentException(&loaded, pEnv,
    //                                             (NULL == settings),
    //                                             "settings is NULL");

    // // Make sure that the context was set.
    // videoEditJava_checkAndThrowIllegalStateException(&loaded, pEnv,
    //                                          (M4OSA_NULL == pContext),
    //                                          "not initialized");

    // // Load the clip settings
    // ALOGV("videoEditor_generateClip Calling videoEditor_loadSettings");
    // videoEditor_loadSettings(pEnv, thiz, settings);
    // ALOGV("videoEditor_generateClip videoEditor_loadSettings returned");

    // // Generate the clip
    // ALOGV("videoEditor_generateClip Calling LVME_processClip");
    // result = videoEditor_processClip(pEnv, thiz, 0 /*item id is unused*/);
    // ALOGV("videoEditor_generateClip videoEditor_processClip returned 0x%x", result);

    // if (pContext->state != ManualEditState_INITIALIZED) {
    //     // Free up memory (whatever the result)
    //     videoEditor_unloadSettings(pEnv, thiz);
    // }

    // ALOGV("videoEditor_generateClip END 0x%x", (unsigned int) result);
    // return result;
    return 0;
}

} // namespace Videoeditor
} // namespace Media
} // namepsace Droid
} // namespace Elastos
