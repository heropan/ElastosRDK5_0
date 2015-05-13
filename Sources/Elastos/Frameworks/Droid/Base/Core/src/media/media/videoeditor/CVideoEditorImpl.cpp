#include "CVideoEditorImpl.h"
#include "os/CSystemProperties.h"
#include "util/Xml.h"
#include <elastos/StringUtils.h>
#include "MediaItem.h"
#include "CMediaImageItem.h"
#include "COverlayFrame.h"
#include "Transition.h"
#include "CTransitionAlpha.h"
#include "CTransitionCrossfade.h"
#include "CTransitionSliding.h"
#include "CTransitionFadeBlack.h"
#include "CMediaVideoItem.h"
#include "CAudioTrack2.h"
#include "CEffectColor.h"
#include "CEffectKenBurns.h"
#include "COverlayFrame.h"
#include "graphics/CBitmapFactory.h"
#include "graphics/CRect.h"
//#include "media/media/CMediaMetadataRetriever.h"

using Org::Xmlpull::V1::IXmlSerializer;
using Org::Kxml2::IO::CKXmlSerializer;
using Elastos::IO::IFile;
using Elastos::IO::CFile;
using Elastos::IO::IStringWriter;
using Elastos::IO::CStringWriter;
using Elastos::IO::IFileInputStream;
using Elastos::IO::CFileInputStream;
using Elastos::IO::IFileOutputStream;
using Elastos::IO::CFileOutputStream;
using Elastos::IO::IFlushable;
using Elastos::Core::CObjectContainer;
using Elastos::Core::StringUtils;
using Elastos::Core::ICharSequence;
using Elastos::Core::IBoolean;
using Elastos::Core::CBoolean;
using Elastos::Utility::Concurrent::ITimeUnit;
using Elastos::Utility::Concurrent::ITimeUnitHelper;
using Elastos::Utility::Concurrent::CTimeUnitHelper;
using Elastos::Utility::Concurrent::CSemaphore;
using Elastos::Droid::Os::ISystemProperties;
using Elastos::Droid::Os::CSystemProperties;
using Elastos::Droid::Utility::Xml;
using Elastos::Droid::Graphics::IRect;
using Elastos::Droid::Graphics::CRect;
using Elastos::Droid::Graphics::IBitmapFactory;
using Elastos::Droid::Graphics::CBitmapFactory;
using Elastos::Droid::Media::IMediaMetadataRetriever;
//using Elastos::Droid::Media::CMediaMetadataRetriever;

namespace Elastos {
namespace Droid {
namespace Media {
namespace Videoeditor {

const String CVideoEditorImpl::TAG("VideoEditorImpl");
const String CVideoEditorImpl::PROJECT_FILENAME("videoeditor->Xml");
const String CVideoEditorImpl::TAG_PROJECT("project");
const String CVideoEditorImpl::TAG_MEDIA_ITEMS("media_items");
const String CVideoEditorImpl::TAG_MEDIA_ITEM("media_item");
const String CVideoEditorImpl::TAG_TRANSITIONS("transitions");
const String CVideoEditorImpl::TAG_TRANSITION("transition");
const String CVideoEditorImpl::TAG_OVERLAYS("overlays");
const String CVideoEditorImpl::TAG_OVERLAY("overlay");
const String CVideoEditorImpl::TAG_OVERLAY_USER_ATTRIBUTES("overlay_user_attributes");
const String CVideoEditorImpl::TAG_EFFECTS("effects");
const String CVideoEditorImpl::TAG_EFFECT("effect");
const String CVideoEditorImpl::TAG_AUDIO_TRACKS("audio_tracks");
const String CVideoEditorImpl::TAG_AUDIO_TRACK("audio_track");

const String CVideoEditorImpl::ATTR_ID("id");
const String CVideoEditorImpl::ATTR_FILENAME("filename");
const String CVideoEditorImpl::ATTR_AUDIO_WAVEFORM_FILENAME("waveform");
const String CVideoEditorImpl::ATTR_RENDERING_MODE("rendering_mode");
const String CVideoEditorImpl::ATTR_ASPECT_RATIO("aspect_ratio");
const String CVideoEditorImpl::ATTR_REGENERATE_PCM("regeneratePCMFlag");
const String CVideoEditorImpl::ATTR_TYPE("type");
const String CVideoEditorImpl::ATTR_DURATION("duration");
const String CVideoEditorImpl::ATTR_START_TIME("start_time");
const String CVideoEditorImpl::ATTR_BEGIN_TIME("begin_time");
const String CVideoEditorImpl::ATTR_END_TIME("end_time");
const String CVideoEditorImpl::ATTR_VOLUME("volume");
const String CVideoEditorImpl::ATTR_BEHAVIOR("behavior");
const String CVideoEditorImpl::ATTR_DIRECTION("direction");
const String CVideoEditorImpl::ATTR_BLENDING("blending");
const String CVideoEditorImpl::ATTR_INVERT("invert");
const String CVideoEditorImpl::ATTR_MASK("mask");
const String CVideoEditorImpl::ATTR_BEFORE_MEDIA_ITEM_ID("before_media_item");
const String CVideoEditorImpl::ATTR_AFTER_MEDIA_ITEM_ID("after_media_item");
const String CVideoEditorImpl::ATTR_COLOR_EFFECT_TYPE("color_type");
const String CVideoEditorImpl::ATTR_COLOR_EFFECT_VALUE("color_value");
const String CVideoEditorImpl::ATTR_START_RECT_LEFT("start_l");
const String CVideoEditorImpl::ATTR_START_RECT_TOP("start_t");
const String CVideoEditorImpl::ATTR_START_RECT_RIGHT("start_r");
const String CVideoEditorImpl::ATTR_START_RECT_BOTTOM("start_b");
const String CVideoEditorImpl::ATTR_END_RECT_LEFT("end_l");
const String CVideoEditorImpl::ATTR_END_RECT_TOP("end_t");
const String CVideoEditorImpl::ATTR_END_RECT_RIGHT("end_r");
const String CVideoEditorImpl::ATTR_END_RECT_BOTTOM("end_b");
const String CVideoEditorImpl::ATTR_LOOP("loop");
const String CVideoEditorImpl::ATTR_MUTED("muted");
const String CVideoEditorImpl::ATTR_DUCK_ENABLED("ducking_enabled");
const String CVideoEditorImpl::ATTR_DUCK_THRESHOLD("ducking_threshold");
const String CVideoEditorImpl::ATTR_DUCKED_TRACK_VOLUME("ducking_volume");
const String CVideoEditorImpl::ATTR_GENERATED_IMAGE_CLIP("generated_image_clip");
const String CVideoEditorImpl::ATTR_IS_IMAGE_CLIP_GENERATED("is_image_clip_generated");
const String CVideoEditorImpl::ATTR_GENERATED_TRANSITION_CLIP("generated_transition_clip");
const String CVideoEditorImpl::ATTR_IS_TRANSITION_GENERATED("is_transition_generated");
const String CVideoEditorImpl::ATTR_OVERLAY_RGB_FILENAME("overlay_rgb_filename");
const String CVideoEditorImpl::ATTR_OVERLAY_FRAME_WIDTH("overlay_frame_width");
const String CVideoEditorImpl::ATTR_OVERLAY_FRAME_HEIGHT("overlay_frame_height");
const String CVideoEditorImpl::ATTR_OVERLAY_RESIZED_RGB_FRAME_WIDTH("resized_RGBframe_width");
const String CVideoEditorImpl::ATTR_OVERLAY_RESIZED_RGB_FRAME_HEIGHT("resized_RGBframe_height");
const Int32 CVideoEditorImpl::ENGINE_ACCESS_MAX_TIMEOUT_MS = 500;

CVideoEditorImpl::CVideoEditorImpl()
    : mProjectPath(NULL)
    , mDurationMs(0)
    , mAspectRatio(0)
    , mPreviewInProgress(FALSE)
    , mMallocDebug(FALSE)
{
}

ECode CVideoEditorImpl::constructor(
    /* [in] */ const String& projectPath)
{
    String s;
    AutoPtr<ISystemProperties> properties;
    CSystemProperties::AcquireSingleton((ISystemProperties**)&properties);
    properties->Get(String("libc->Debug->Malloc"), &s);
    if (s.Equals("1")) {
        mMallocDebug = TRUE;
        //try {
            DumpHeap(String("HeapAtStart"));
        //} catch (Exception ex) {
        //    Log->E(TAG, "dumpHeap returned error in constructor");
        //}
    }
    else {
        mMallocDebug = FALSE;
    }
    CSemaphore::New(1, TRUE, (ISemaphore**)&mLock);
    mMANativeHelper = new MediaArtistNativeHelper(projectPath, mLock, this);
    mProjectPath = projectPath;
    AutoPtr<IFile> projectXml;
    CFile::New(projectPath, PROJECT_FILENAME, (IFile**)&projectXml);
    Boolean b;
    projectXml->Exists(&b);
    if (b) {
        //try {
            Load();
        //} catch (Exception ex) {
        //    ex->PrintStackTrace();
        //    throw new IOException(ex->ToString());
        //}
    }
    else {
        mAspectRatio = IMediaProperties::ASPECT_RATIO_16_9;
        mDurationMs = 0;
    }
    return NOERROR;
}

ECode CVideoEditorImpl::GetNativeContext(
    /* [out] */ MediaArtistNativeHelper** result)
{
    VALIDATE_NOT_NULL(result);
    *result = mMANativeHelper;
    return NOERROR;
}

ECode CVideoEditorImpl::AddAudioTrack(
    /* [in] */ IAudioTrack2* audioTrack)
{
    Mutex::Autolock lock(&_m_syncLock);

    if (audioTrack == NULL) {
        //throw new IllegalArgumentException("Audio Track is NULL");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (mAudioTracks.GetSize() == 1) {
        //throw new IllegalArgumentException("No more tracks can be added");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    mMANativeHelper->SetGeneratePreview(TRUE);

    /*
     * Add the audio track to AudioTrack list
     */
    mAudioTracks.PushBack(audioTrack);

    /*
     * Form the audio PCM file path
     */
    String id;
    audioTrack->GetId(&id);
    String audioTrackPCMFilePath = mProjectPath + "/"
                + "AudioPcm" + id + "->Pcm";

    /*
     * Create PCM only if not generated in previous session
     */
    AutoPtr<IFile> file;
    CFile::New(audioTrackPCMFilePath, (IFile**)&file);
    Boolean b;
    file->Exists(&b);
    if (b) {
        mMANativeHelper->SetAudioflag(FALSE);
    }
    return NOERROR;
}

ECode CVideoEditorImpl::AddMediaItem(
    /* [in] */ IMediaItem* mediaItem)
{
    Mutex::Autolock lock(&_m_syncLock);
    /*
     * Validate Media Item
     */
    if (mediaItem == NULL) {
        //throw new IllegalArgumentException("Media item is NULL");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    /*
     * Add the Media item to MediaItem list
     */

    List<AutoPtr<IMediaItem> >::Iterator it = mMediaItems.Begin();
    for(; it != mMediaItems.End(); it++) {
        if(*it == mediaItem) break;
    }
    if (it != mMediaItems.End()) {
        //throw new IllegalArgumentException("Media item already exists: " + mediaItem->GetId());
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    mMANativeHelper->SetGeneratePreview(TRUE);

    /*
     *  Invalidate the end transition if necessary
     */
    Int32 mediaItemsCount = mMediaItems.GetSize();
    if (mediaItemsCount > 0) {
        RemoveTransitionAfter(mediaItemsCount - 1);
    }

    /*
     *  Add the new media item
     */
    mMediaItems.PushBack(mediaItem);

    ComputeTimelineDuration();

    /*
     *  Generate project thumbnail only from first media Item on storyboard
     */
    if (mMediaItems.GetSize() == 1) {
        GenerateProjectThumbnail();
    }
    return NOERROR;
}

ECode CVideoEditorImpl::AddTransition(
    /* [in] */ ITransition* transition)
{
    Mutex::Autolock lock(&_m_syncLock);

    if (transition == NULL) {
        //throw new IllegalArgumentException("NULL Transition");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoPtr<IMediaItem> beforeMediaItem;
    transition->GetBeforeMediaItem((IMediaItem**)&beforeMediaItem);
    AutoPtr<IMediaItem> afterMediaItem;
    transition->GetAfterMediaItem((IMediaItem**)&afterMediaItem);
    /*
     * Check if the MediaItems are in sequence
     */
    if (mMediaItems.IsEmpty()) {
        //throw new IllegalArgumentException("No media items are added");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if ((afterMediaItem != NULL) &&  (beforeMediaItem != NULL)) {
        List<AutoPtr<IMediaItem> >::Iterator itAfter = mMediaItems.Begin();
        for(; itAfter != mMediaItems.End(); itAfter++) {
            if(*itAfter == afterMediaItem) break;
        }
        List<AutoPtr<IMediaItem> >::Iterator itBefore = mMediaItems.Begin();
        for(; itBefore != mMediaItems.End(); itBefore++) {
            if(*itBefore == beforeMediaItem) break;
        }

        if((itAfter == mMediaItems.End()) || (itBefore == mMediaItems.End())) {
            //throw new IllegalArgumentException
            //    ("Either of the mediaItem is not found in the list");
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }

        if (itAfter != (--itBefore)) {
            //throw new IllegalArgumentException("MediaItems are not in sequence");
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
    }

    mMANativeHelper->SetGeneratePreview(TRUE);

    mTransitions.PushBack(transition);
    /*
     *  Cross reference the transitions
     */
    if (afterMediaItem != NULL) {
        /*
         *  If a transition already exists at the specified position then
         *  invalidate it->
         */
        AutoPtr<ITransition> eTransition;
        afterMediaItem->GetEndTransition((ITransition**)&eTransition);
        if (eTransition != NULL) {
            (reinterpret_cast<Transition*>(eTransition->Probe(EIID_Transition)))->Invalidate();
            mTransitions.Remove(eTransition);
        }
        ((MediaItem*)afterMediaItem.Get())->SetEndTransition(transition);
    }

    if (beforeMediaItem != NULL) {
        /*
         *  If a transition already exists at the specified position then
         *  invalidate it->
         */
        AutoPtr<ITransition> bTransition;
        beforeMediaItem->GetBeginTransition((ITransition**)&bTransition);
        if (bTransition != NULL) {
            (reinterpret_cast<Transition*>(bTransition->Probe(EIID_Transition)))->Invalidate();
            mTransitions.Remove(bTransition);
        }
        ((MediaItem*)beforeMediaItem.Get())->SetBeginTransition(transition);
    }

    ComputeTimelineDuration();
    return NOERROR;
}

ECode CVideoEditorImpl::CancelExport(
    /* [in] */ const String& filename)
{
    if (mMANativeHelper != NULL && filename != NULL) {
        mMANativeHelper->Stop(filename);
    }
    return NOERROR;
}

ECode CVideoEditorImpl::ExportEx(
    /* [in] */ const String& filename,
    /* [in] */ Int32 height,
    /* [in] */ Int32 bitrate,
    /* [in] */ Int32 audioCodec,
    /* [in] */ Int32 videoCodec,
    /* [in] */ IVideoEditorExportProgressListener* listener)
{
    Int32 audcodec = 0;
    Int32 vidcodec = 0;
    if (filename == NULL) {
        //throw new IllegalArgumentException("export: filename is NULL");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoPtr<IFile> tempPathFile;
    CFile::New(filename, (IFile**)&tempPathFile);
    if (tempPathFile == NULL) {
        //throw new IOException(filename + "can not be created");
        return E_IO_EXCEPTION;
    }

    if (mMediaItems.GetSize() == 0) {
        //throw new IllegalStateException("No MediaItems added");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    switch (height) {
        case IMediaProperties::HEIGHT_144:
            break;
        case IMediaProperties::HEIGHT_288:
            break;
        case IMediaProperties::HEIGHT_360:
            break;
        case IMediaProperties::HEIGHT_480:
            break;
        case IMediaProperties::HEIGHT_720:
            break;
        case IMediaProperties::HEIGHT_1080:
            break;

        default: {
            String message = String("Unsupported height value ") + height;
            //throw new IllegalArgumentException(message);
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
    }

    switch (bitrate) {
        case IMediaProperties::BITRATE_28K:
            break;
        case IMediaProperties::BITRATE_40K:
            break;
        case IMediaProperties::BITRATE_64K:
            break;
        case IMediaProperties::BITRATE_96K:
            break;
        case IMediaProperties::BITRATE_128K:
            break;
        case IMediaProperties::BITRATE_192K:
            break;
        case IMediaProperties::BITRATE_256K:
            break;
        case IMediaProperties::BITRATE_384K:
            break;
        case IMediaProperties::BITRATE_512K:
            break;
        case IMediaProperties::BITRATE_800K:
            break;
        case IMediaProperties::BITRATE_2M:
            break;
        case IMediaProperties::BITRATE_5M:
            break;
        case IMediaProperties::BITRATE_8M:
            break;

        default: {
            String message = String("Unsupported bitrate value ") + bitrate;
            //throw new IllegalArgumentException(message);
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
    }
    ComputeTimelineDuration();
    Int64 audioBitrate = IMediaArtistNativeHelperBitrate::BR_96_KBPS;
    Int64 fileSize = (mDurationMs * (bitrate + audioBitrate)) / 8000;
    if (MAX_SUPPORTED_FILE_SIZE <= fileSize) {
        //throw new IllegalStateException("Export Size is more than 2GB");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    switch (audioCodec) {
        case IMediaProperties::ACODEC_AAC_LC:
            audcodec = IMediaArtistNativeHelperAudioFormat::AAC;
            break;
        case IMediaProperties::ACODEC_AMRNB:
            audcodec = IMediaArtistNativeHelperAudioFormat::AMR_NB;
            break;

        default: {
            // String message = "Unsupported audio codec type " + audioCodec;
            //throw new IllegalArgumentException(message);
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
    }

    switch (videoCodec) {
        case IMediaProperties::VCODEC_H263:
            vidcodec = IMediaArtistNativeHelperVideoFormat::H263;
            break;
        case IMediaProperties::VCODEC_H264:
            vidcodec = IMediaArtistNativeHelperVideoFormat::H264;
            break;
        case IMediaProperties::VCODEC_MPEG4:
            vidcodec = IMediaArtistNativeHelperVideoFormat::MPEG4;
            break;

        default: {
            String message = String("Unsupported video codec type ") + videoCodec;
            //throw new IllegalArgumentException(message);
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
    }

    Boolean semAcquireDone = FALSE;
    //try {
        Lock();
        semAcquireDone = TRUE;

        if (mMANativeHelper == NULL) {
            //throw new IllegalStateException("The video editor is not initialized");
            return E_ILLEGAL_STATE_EXCEPTION;
        }
        mMANativeHelper->SetAudioCodec(audcodec);
        mMANativeHelper->SetVideoCodec(vidcodec);

        AutoPtr<ArrayOf<IMediaItem*> > mediaItemArray = ArrayOf<IMediaItem*>::Alloc(mMediaItems.GetSize());
        List<AutoPtr<IMediaItem> >::Iterator iter1 = mMediaItems.Begin();
        for(Int32 i = 0; iter1 != mMediaItems.End(); ++iter1) {
            mediaItemArray->Set(i++, *iter1);
        }
        AutoPtr<ArrayOf<ITransition*> > transitionArray = ArrayOf<ITransition*>::Alloc(mTransitions.GetSize());
        List<AutoPtr<ITransition> >::Iterator iter2 = mTransitions.Begin();
        for(Int32 i = 0; iter2 != mTransitions.End(); ++iter2) {
            transitionArray->Set(i++, *iter2);
        }
        AutoPtr<ArrayOf<IAudioTrack2*> > audioTrackArray = ArrayOf<IAudioTrack2*>::Alloc(mAudioTracks.GetSize());
        List<AutoPtr<IAudioTrack2> >::Iterator iter3 = mAudioTracks.Begin();
        for(Int32 i = 0; iter3 != mAudioTracks.End(); ++iter3) {
            audioTrackArray->Set(i++, *iter3);
        }

        mMANativeHelper->Export(filename, mProjectPath, height,bitrate,
                           mediaItemArray, transitionArray, audioTrackArray, listener);
    //} catch (InterruptedException  ex) {
    //    Log->E(TAG, "Sem acquire NOT successful in export");
    //} finally {
        if (semAcquireDone) {
            Unlock();
        }
    //}
    return NOERROR;
}

ECode CVideoEditorImpl::Export(
    /* [in] */ const String& filename,
    /* [in] */ Int32 height,
    /* [in] */ Int32 bitrate,
    /* [in] */ IVideoEditorExportProgressListener* listener)
{
    Int32 defaultAudiocodec = IMediaArtistNativeHelperAudioFormat::AAC;
    Int32 defaultVideocodec = IMediaArtistNativeHelperVideoFormat::MPEG4;

    return ExportEx(filename, height, bitrate, defaultAudiocodec, defaultVideocodec, listener);
}

ECode CVideoEditorImpl::GeneratePreview(
    /* [in] */ IVideoEditorMediaProcessingProgressListener* listener)
{
    Boolean semAcquireDone = FALSE;
    //try {
        Lock();
        semAcquireDone = TRUE;

        if (mMANativeHelper == NULL) {
            //throw new IllegalStateException("The video editor is not initialized");
            return E_ILLEGAL_STATE_EXCEPTION;
        }

        if ((mMediaItems.GetSize() > 0) || (mAudioTracks.GetSize() > 0)) {

            AutoPtr<ArrayOf<IMediaItem*> > mediaItemArray = ArrayOf<IMediaItem*>::Alloc(mMediaItems.GetSize());
            List<AutoPtr<IMediaItem> >::Iterator iter1 = mMediaItems.Begin();
            for(Int32 i = 0; iter1 != mMediaItems.End(); ++iter1) {
                mediaItemArray->Set(i++, *iter1);
            }
            AutoPtr<ArrayOf<ITransition*> > transitionArray = ArrayOf<ITransition*>::Alloc(mTransitions.GetSize());
            List<AutoPtr<ITransition> >::Iterator iter2 = mTransitions.Begin();
            for(Int32 i = 0; iter2 != mTransitions.End(); ++iter2) {
                transitionArray->Set(i++, *iter2);
            }
            AutoPtr<ArrayOf<IAudioTrack2*> > audioTrackArray = ArrayOf<IAudioTrack2*>::Alloc(mAudioTracks.GetSize());
            List<AutoPtr<IAudioTrack2> >::Iterator iter3 = mAudioTracks.Begin();
            for(Int32 i = 0; iter3 != mAudioTracks.End(); ++iter3) {
                audioTrackArray->Set(i++, *iter3);
            }

            mMANativeHelper->PreviewStoryBoard(mediaItemArray, transitionArray, audioTrackArray, listener);
        }
    //} catch (InterruptedException  ex) {
    //    Log->E(TAG, "Sem acquire NOT successful in previewStoryBoard");
    //} finally {
        if (semAcquireDone) {
            Unlock();
        }
    //}
    return NOERROR;
}

ECode CVideoEditorImpl::GetAllAudioTracks(
    /* [out] */ IObjectContainer** result)
{
    VALIDATE_NOT_NULL(result);

    AutoPtr<IObjectContainer> container;
    CObjectContainer::New((IObjectContainer**)&container);
    List<AutoPtr<IAudioTrack2> >::Iterator it = mAudioTracks.Begin();
    for (; it != mAudioTracks.End(); ++it) {
        container->Add((IInterface*)(*it));
    }

    *result = container;
    INTERFACE_ADDREF(*result);
    return NOERROR;
}

ECode CVideoEditorImpl::GetAllMediaItems(
    /* [out] */ IObjectContainer** result)
{
    VALIDATE_NOT_NULL(result);

    AutoPtr<IObjectContainer> container;
    CObjectContainer::New((IObjectContainer**)&container);
    List<AutoPtr<IMediaItem> >::Iterator it = mMediaItems.Begin();
    for (; it != mMediaItems.End(); ++it) {
        container->Add((IInterface*)(*it));
    }

    *result = container;
    INTERFACE_ADDREF(*result);
    return NOERROR;
}

ECode CVideoEditorImpl::GetAllTransitions(
    /* [out] */ IObjectContainer** result)
{
    VALIDATE_NOT_NULL(result);

    AutoPtr<IObjectContainer> container;
    CObjectContainer::New((IObjectContainer**)&container);
    List<AutoPtr<ITransition> >::Iterator it = mTransitions.Begin();
    for (; it != mTransitions.End(); ++it) {
        container->Add((IInterface*)(*it));
    }

    *result = container;
    INTERFACE_ADDREF(*result);
    return NOERROR;
}

ECode CVideoEditorImpl::GetAspectRatio(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mAspectRatio;
    return NOERROR;
}

ECode CVideoEditorImpl::GetAudioTrack(
    /* [in] */ const String& audioTrackId,
    /* [out] */ IAudioTrack2** result)
{
    VALIDATE_NOT_NULL(result);

    List<AutoPtr<IAudioTrack2> >::Iterator it = mAudioTracks.Begin();
    for (; it != mAudioTracks.End(); ++it) {
        String id;
        (*it)->GetId(&id);
        if(id.Equals(audioTrackId)) {
            *result = *it;
            INTERFACE_ADDREF(*result);
            return NOERROR;
        }
    }

    *result = NULL;
    return NOERROR;
}

ECode CVideoEditorImpl::GetDuration(
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);

    /**
     *  Since MediaImageItem can change duration we need to compute the
     *  duration here
     */
    ComputeTimelineDuration();
    *result = mDurationMs;
    return NOERROR;
}

ECode CVideoEditorImpl::UpdateTimelineDuration()
{
    ComputeTimelineDuration();
    return NOERROR;
}

ECode CVideoEditorImpl::GetMediaItem(
    /* [in] */ const String& mediaItemId,
    /* [out] */ IMediaItem** result)
{
    VALIDATE_NOT_NULL(result);

    Mutex::Autolock lock(&_m_syncLock);
    List<AutoPtr<IMediaItem> >::Iterator it = mMediaItems.Begin();
    for (; it != mMediaItems.End(); ++it) {
        String id;
        (*it)->GetId(&id);
        if(id.Equals(mediaItemId)) {
            *result = *it;
            INTERFACE_ADDREF(*result);
            return NOERROR;
        }
    }

    *result = NULL;
    return NOERROR;
}

ECode CVideoEditorImpl::GetPath(
    /* [out]*/ String* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mProjectPath;
    return NOERROR;
}

ECode CVideoEditorImpl::GetTransition(
    /* [in] */ const String& transitionId,
    /* [out] */ ITransition** result)
{
    VALIDATE_NOT_NULL(result);

    List<AutoPtr<ITransition> >::Iterator it = mTransitions.Begin();
    for (; it != mTransitions.End(); ++it) {
        String id;
        (*it)->GetId(&id);
        if(id.Equals(transitionId)) {
            *result = *it;
            INTERFACE_ADDREF(*result);
            return NOERROR;
        }
    }

    *result = NULL;
    return NOERROR;
}

ECode CVideoEditorImpl::InsertAudioTrack(
    /* [in] */ IAudioTrack2* audioTrack,
    /* [in] */ const String& afterAudioTrackId)
{
    Mutex::Autolock lock(&_m_syncLock);

    if (mAudioTracks.GetSize() == 1) {
        //throw new IllegalArgumentException("No more tracks can be added");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (afterAudioTrackId == NULL) {
        mMANativeHelper->SetGeneratePreview(TRUE);
        mAudioTracks.Insert(0, audioTrack);
    }
    else {
        List<AutoPtr<IAudioTrack2> >::Iterator it = mAudioTracks.Begin();
        for (Int32 i = 0; it != mAudioTracks.End(); ++it, ++i) {
            String id;
            (*it)->GetId(&id);
            if(id.Equals(afterAudioTrackId)) {
                mMANativeHelper->SetGeneratePreview(TRUE);
                mAudioTracks.Insert(i + 1, audioTrack);
                return NOERROR;
            }
        }
        //throw new IllegalArgumentException("AudioTrack not found: " + afterAudioTrackId);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

ECode CVideoEditorImpl::InsertMediaItem(
    /* [in] */ IMediaItem* mediaItem,
    /* [in] */ const String& afterMediaItemId)
{
    Mutex::Autolock lock(&_m_syncLock);

    List<AutoPtr<IMediaItem> >::Iterator it = mMediaItems.Begin();
    for (; it != mMediaItems.End(); ++it) {
        if(*it == mediaItem)break;
    }
    if (it != mMediaItems.End()) {
        //throw new IllegalArgumentException("Media item already exists: " + mediaItem->GetId());
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (afterMediaItemId == NULL) {
        mMANativeHelper->SetGeneratePreview(TRUE);
        if (mMediaItems.GetSize() > 0) {
            /**
             *  Invalidate the transition at the beginning of the timeline
             */
            RemoveTransitionBefore(0);
        }

        mMediaItems.Insert(0, mediaItem);
        ComputeTimelineDuration();
        GenerateProjectThumbnail();
    }
    else {
        List<AutoPtr<IMediaItem> >::Iterator it = mMediaItems.Begin();
        for (Int32 i = 0; it != mMediaItems.End(); ++it, ++i) {
            String id;
            (*it)->GetId(&id);
            if(id.Equals(afterMediaItemId)) {
                mMANativeHelper->SetGeneratePreview(TRUE);
                /**
                 *  Invalidate the transition at this position
                 */
                RemoveTransitionAfter(i);
                /**
                 *  Insert the new media item
                 */
                mMediaItems.Insert(i + 1, mediaItem);
                ComputeTimelineDuration();
                return NOERROR;
            }
        }

        //throw new IllegalArgumentException("MediaItem not found: " + afterMediaItemId);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

ECode CVideoEditorImpl::MoveAudioTrack(
    /* [in] */ const String& audioTrackId,
    /* [in] */ const String& afterAudioTrackId)
{
    Mutex::Autolock lock(&_m_syncLock);

    //throw new IllegalStateException("Not supported");
    return E_ILLEGAL_STATE_EXCEPTION;
}

ECode CVideoEditorImpl::MoveMediaItem(
    /* [in] */ const String& mediaItemId,
    /* [in] */ const String& afterMediaItemId)
{
    Mutex::Autolock lock(&_m_syncLock);

    AutoPtr<IMediaItem> moveMediaItem;
    moveMediaItem = RemoveMediaItem(mediaItemId, TRUE);
    if (moveMediaItem == NULL) {
        //throw new IllegalArgumentException("Target MediaItem not found: " + mediaItemId);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (afterMediaItemId == NULL) {
        if (mMediaItems.GetSize() > 0) {
            mMANativeHelper->SetGeneratePreview(TRUE);

            /**
             *  Invalidate adjacent transitions at the insertion point
             */
            RemoveTransitionBefore(0);

            /**
             *  Insert the media item at the new position
             */
            mMediaItems.Insert(0, moveMediaItem);
            ComputeTimelineDuration();

            GenerateProjectThumbnail();
        }
        else{
            //throw new IllegalStateException("Cannot move media item (it is the only item)");
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
    }
    else {
        List<AutoPtr<IMediaItem> >::Iterator it = mMediaItems.Begin();
        for (Int32 i = 0; it != mMediaItems.End(); ++it, ++i) {
            String id;
            (*it)->GetId(&id);
            if(id.Equals(afterMediaItemId)) {
                mMANativeHelper->SetGeneratePreview(TRUE);
                /**
                 *  Invalidate the transition at this position
                 */
                RemoveTransitionAfter(i);
                /**
                 *  Insert the new media item
                 */
                mMediaItems.Insert(i + 1, moveMediaItem);
                ComputeTimelineDuration();
                return NOERROR;
            }
        }

        //throw new IllegalArgumentException("MediaItem not found: " + afterMediaItemId);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

ECode CVideoEditorImpl::ReleaseResources()
{
    Int64 result;
    StopPreview(&result);

    Boolean semAcquireDone = FALSE;
    //try {
        Lock();
        semAcquireDone = TRUE;

        if (mMANativeHelper != NULL) {
            mMediaItems.Clear();
            mAudioTracks.Clear();
            mTransitions.Clear();
            mMANativeHelper->ReleaseNativeHelper();
            mMANativeHelper = NULL;
        }
    //} catch (Exception  ex) {
    //    Log->E(TAG, "Sem acquire NOT successful in export", ex);
    //} finally {
        if (semAcquireDone) {
            Unlock();
        }
    //}
    if (mMallocDebug) {
        //try {
            DumpHeap(String("HeapAtEnd"));
        //} catch (Exception ex) {
        //    Log->E(TAG, "dumpHeap returned error in release");
        //}
    }

    return NOERROR;
}

ECode CVideoEditorImpl::RemoveAllMediaItems()
{
    Mutex::Autolock lock(&_m_syncLock);

    mMANativeHelper->SetGeneratePreview(TRUE);

    mMediaItems.Clear();

    /**
     *  Invalidate all transitions
     */
    List<AutoPtr<ITransition> >::Iterator it = mTransitions.Begin();
    for(; it != mTransitions.End(); it++) {
        (reinterpret_cast<Transition*>((*it)->Probe(EIID_Transition)))->Invalidate();
    }
    mTransitions.Clear();

    mDurationMs = 0;
    /**
     * If a thumbnail already exists, then delete it
     */
    AutoPtr<IFile> file;
    CFile::New(mProjectPath + "/" + IVideoEditor::THUMBNAIL_FILENAME, (IFile**)&file);
    Boolean b;
    file->Exists(&b);
    if (b) {
        file->Delete(&b);
    }
    return NOERROR;
}

ECode CVideoEditorImpl::RemoveAudioTrack(
    /* [in] */ const String& audioTrackId,
    /* [out] */ IAudioTrack2** result)
{
    VALIDATE_NOT_NULL(result);

    Mutex::Autolock lock(&_m_syncLock);

    AutoPtr<IAudioTrack2> audioTrack;
    GetAudioTrack(audioTrackId, (IAudioTrack2**)&audioTrack);
    if (audioTrack != NULL) {
        mMANativeHelper->SetGeneratePreview(TRUE);
        mAudioTracks.Remove(audioTrack);
        audioTrack->Invalidate();
        mMANativeHelper->InvalidatePcmFile();
        mMANativeHelper->SetAudioflag(TRUE);
    }
    else {
        //throw new IllegalArgumentException(" No more audio tracks");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    *result = audioTrack;
    INTERFACE_ADDREF(*result);
    return NOERROR;
}

ECode CVideoEditorImpl::RemoveMediaItem(
    /* [in] */ const String& mediaItemId,
    /* [out] */ IMediaItem** result)
{
    VALIDATE_NOT_NULL(result);

    Mutex::Autolock lock(&_m_syncLock);

    String firstItemString;
    mMediaItems[0]->GetId(&firstItemString);
    AutoPtr<IMediaItem> mediaItem;
    GetMediaItem(mediaItemId, (IMediaItem**)&mediaItem);
    if (mediaItem != NULL) {
        mMANativeHelper->SetGeneratePreview(TRUE);
        /**
         *  Remove the media item
         */
        mMediaItems.Remove(mediaItem);

        AutoPtr<IMediaImageItem> item = IMediaImageItem::Probe(mediaItem);
        if (item != NULL) {
            ((CMediaImageItem*)item.Get())->Invalidate();
        }

        AutoPtr<ArrayOf<IOverlay*> > overlays;
        mediaItem->GetAllOverlays((ArrayOf<IOverlay*>**)&overlays);
        if (overlays->GetLength() > 0) {
            for(Int32 i = 0; i < overlays->GetLength(); i++) {
                AutoPtr<IOverlayFrame> frame = IOverlayFrame::Probe((*overlays)[i]);
                if (frame != NULL) {
                    ((COverlayFrame*)frame.Get())->Invalidate();
                }
            }
        }

        /**
         *  Remove the adjacent transitions
         */
        RemoveAdjacentTransitions(mediaItem);
        ComputeTimelineDuration();
    }

    /**
     * If string equals first mediaItem, then
     * generate Project thumbnail
     */
    if (firstItemString.Equals(mediaItemId)) {
        GenerateProjectThumbnail();
    }

    AutoPtr<IMediaVideoItem> item = IMediaVideoItem::Probe(mediaItem);
    if (item != NULL) {
        /**
         * Delete the graph file
         */
        ((CMediaVideoItem*)item.Get())->Invalidate();
    }
    *result = mediaItem;
    INTERFACE_ADDREF(*result);
    return NOERROR;
}

AutoPtr<IMediaItem> CVideoEditorImpl::RemoveMediaItem(
    /* [in] */ const String& mediaItemId,
    /* [in] */ Boolean flag)
{
    Mutex::Autolock lock(&_m_syncLock);

    String firstItemString;
    mMediaItems[0]->GetId(&firstItemString);

    AutoPtr<IMediaItem> mediaItem;
    GetMediaItem(mediaItemId, (IMediaItem**)&mediaItem);
    if (mediaItem != NULL) {
        mMANativeHelper->SetGeneratePreview(TRUE);
        /**
         *  Remove the media item
         */
        mMediaItems.Remove(mediaItem);
        /**
         *  Remove the adjacent transitions
         */
        RemoveAdjacentTransitions(mediaItem);
        ComputeTimelineDuration();
    }

    /**
     * If string equals first mediaItem, then
     * generate Project thumbail
     */
    if (firstItemString.Equals(mediaItemId)) {
        GenerateProjectThumbnail();
    }
    return mediaItem;
}

ECode CVideoEditorImpl::RemoveTransition(
    /* [in] */ const String& transitionId,
    /* [out] */ ITransition** result)
{
    VALIDATE_NOT_NULL(result);

    Mutex::Autolock lock(&_m_syncLock);

    AutoPtr<ITransition> transition;
    GetTransition(transitionId, (ITransition**)&transition);
    if (transition == NULL) {
        //throw new IllegalStateException("Transition not found: " + transitionId);
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    mMANativeHelper->SetGeneratePreview(TRUE);

    /**
     *  Remove the transition references
     */
    AutoPtr<IMediaItem> afterMediaItem;
    transition->GetAfterMediaItem((IMediaItem**)&afterMediaItem);
    if (afterMediaItem != NULL) {
        ((MediaItem*)afterMediaItem.Get())->SetEndTransition(NULL);
    }

    AutoPtr<IMediaItem> beforeMediaItem;
    transition->GetBeforeMediaItem((IMediaItem**)&beforeMediaItem);
    if (beforeMediaItem != NULL) {
        ((MediaItem*)beforeMediaItem.Get())->SetBeginTransition(NULL);
    }

    mTransitions.Remove(transition);
    (reinterpret_cast<Transition*>(transition->Probe(EIID_Transition)))->Invalidate();
    ComputeTimelineDuration();

    *result = transition;
    INTERFACE_ADDREF(*result);
    return NOERROR;
}

ECode CVideoEditorImpl::RenderPreviewFrame(
    /* [in] */ ISurfaceHolder* surfaceHolder,
    /* [in] */ Int64 timeMs,
    /* [in] */ IVideoEditorOverlayData* overlayData,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);

    if (surfaceHolder == NULL) {
        //throw new IllegalArgumentException("Surface Holder is NULL");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoPtr<ISurface> surface;
    surfaceHolder->GetSurface((ISurface**)&surface);
    if (surface == NULL) {
        //throw new IllegalArgumentException("Surface could not be retrieved from Surface holder");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Boolean b;
    surface->IsValid(&b);
    if (b == FALSE) {
        //throw new IllegalStateException("Surface is not valid");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    if (timeMs < 0) {
        //throw new IllegalArgumentException("requested time not correct");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    } else if (timeMs > mDurationMs) {
        //throw new IllegalArgumentException("requested time more than duration");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Int64 re = 0;

    Boolean semAcquireDone = FALSE;
    //try {
        semAcquireDone = Lock(ENGINE_ACCESS_MAX_TIMEOUT_MS);
        if (semAcquireDone == FALSE) {
            //throw new IllegalStateException("Timeout waiting for semaphore");
            return E_ILLEGAL_STATE_EXCEPTION;
        }

        if (mMANativeHelper == NULL) {
            //throw new IllegalStateException("The video editor is not initialized");
            return E_ILLEGAL_STATE_EXCEPTION;
        }

        if (mMediaItems.GetSize() > 0) {
            AutoPtr<IRect> frame;
            surfaceHolder->GetSurfaceFrame((IRect**)&frame);
            Int32 width, height;
            frame->GetWidth(&width);
            frame->GetHeight(&height);
            mMANativeHelper->RenderPreviewFrame(surface,
                    timeMs, width, height, overlayData, &re);
        } else {
            re = 0;
        }
    //} catch (InterruptedException ex) {
    //    Log->W(TAG, "The thread was interrupted", new Throwable());
        //throw new IllegalStateException("The thread was interrupted");
    //} finally {
        if (semAcquireDone) {
            Unlock();
        }
    //}
    *result = re;
    return NOERROR;
}

void CVideoEditorImpl::Load()
{
    AutoPtr<IFile> file;
    CFile::New(mProjectPath, PROJECT_FILENAME, (IFile**)&file);
    /**
     *  Load the metadata
     */
    AutoPtr<IFileInputStream> fis;
    CFileInputStream::New(file, (IFileInputStream**)&fis);
    //try {
        AutoPtr<List<String> > ignoredMediaItems = new List<String>();

        AutoPtr<IXmlPullParser> parser = Xml::NewPullParser();
        parser->SetInputEx(fis, String("UTF-8"));
        Int32 eventType;
        parser->GetEventType(&eventType);
        String name;
        AutoPtr<IMediaItem> currentMediaItem;
        AutoPtr<IOverlay> currentOverlay;
        Boolean regenerateProjectThumbnail = FALSE;
        while (eventType != IXmlPullParser::END_DOCUMENT) {
            switch (eventType) {
                case IXmlPullParser::START_TAG: {
                    parser->GetName(&name);
                    if (name.Equals(TAG_PROJECT)) {
                        String str;
                        parser->GetAttributeValueEx(String(""), ATTR_ASPECT_RATIO, &str);
                        mAspectRatio = StringUtils::ParseInt32(str);

                        parser->GetAttributeValueEx(String(""), ATTR_REGENERATE_PCM, &str);
                        Boolean mRegenPCM = str.EqualsIgnoreCase("TRUE") ? TRUE : FALSE;
                        mMANativeHelper->SetAudioflag(mRegenPCM);
                    }
                    else if (name.Equals(TAG_MEDIA_ITEM)) {
                        String mediaItemId;
                        parser->GetAttributeValueEx(String(""), ATTR_ID, &mediaItemId);
                        //try {
                            currentMediaItem = ParseMediaItem(parser);
                            mMediaItems.PushBack(currentMediaItem);
                        //} catch (Exception ex) {
                        //    Log->W(TAG, "Cannot load media item: " + mediaItemId, ex);
                            currentMediaItem = NULL;

                            // First media item is invalid, mark for project thumbnail removal
                            if (mMediaItems.GetSize() == 0) {
                                regenerateProjectThumbnail = TRUE;
                            }
                            // Ignore the media item
                            ignoredMediaItems->PushBack(mediaItemId);
                        //}
                    }
                    else if (name.Equals(TAG_TRANSITION)) {
                        //try {
                            AutoPtr<ITransition> transition = ParseTransition(parser,
                                    ignoredMediaItems);
                            // The transition will be NULL if the bounding
                            // media items are ignored
                            if (transition != NULL) {
                                mTransitions.PushBack(transition);
                            }
                        //} catch (Exception ex) {
                        //    Log->W(TAG, "Cannot load transition", ex);
                        //}
                    }
                    else if (name.Equals(TAG_OVERLAY)) {
                        if (currentMediaItem != NULL) {
                            //try {
                                currentOverlay = ParseOverlay(parser, currentMediaItem);
                                currentMediaItem->AddOverlay(currentOverlay);
                            //} catch (Exception ex) {
                            //    Log->W(TAG, "Cannot load overlay", ex);
                            //}
                        }
                    }
                    else if (name.Equals(TAG_OVERLAY_USER_ATTRIBUTES)) {
                        if (currentOverlay != NULL) {
                            Int32 attributesCount;
                            parser->GetAttributeCount(&attributesCount);
                            for (Int32 i = 0; i < attributesCount; i++) {
                                String attrName, attrValue;
                                parser->GetAttributeName(i, &attrName);
                                parser->GetAttributeValue(i, &attrValue);
                                currentOverlay->SetUserAttribute(attrName, attrValue);
                            }
                        }
                    }
                    else if (name.Equals(TAG_EFFECT)) {
                        if (currentMediaItem != NULL) {
                            //try {
                                AutoPtr<IEffect> effect = ParseEffect(parser, currentMediaItem);
                                currentMediaItem->AddEffect(effect);
                                AutoPtr<IMediaImageItem> item = IMediaImageItem::Probe(currentMediaItem);
                                AutoPtr<IEffectKenBurns> ekb = IEffectKenBurns::Probe(effect);
                                if (ekb != NULL) {
                                    String str;
                                    parser->GetAttributeValueEx(String(""), ATTR_IS_IMAGE_CLIP_GENERATED, &str);
                                    Boolean isImageClipGenerated = str.EqualsIgnoreCase("TRUE") ? TRUE : FALSE;
                                    if(isImageClipGenerated) {
                                        String filename;
                                        parser->GetAttributeValueEx(String(""), ATTR_GENERATED_IMAGE_CLIP, &filename);
                                        AutoPtr<IFile> file;
                                        CFile::New(filename, (IFile**)&file);
                                        Boolean b;
                                        file->Exists(&b);
                                        if (b) {
                                            ((CMediaImageItem*)item.Get())->SetGeneratedImageClip(filename);
                                            ((CMediaImageItem*)item.Get())->SetRegenerateClip(FALSE);
                                         } else {
                                           ((CMediaImageItem*)item.Get())->SetGeneratedImageClip(String(NULL));
                                           ((CMediaImageItem*)item.Get())->SetRegenerateClip(TRUE);
                                         }
                                    }
                                    else {
                                        ((CMediaImageItem*)item.Get())->SetGeneratedImageClip(String(NULL));
                                        ((CMediaImageItem*)item.Get())->SetRegenerateClip(TRUE);
                                    }
                                }
                            //} catch (Exception ex) {
                            //    Log->W(TAG, "Cannot load effect", ex);
                            //}
                        }
                    }
                    else if (name.Equals(TAG_AUDIO_TRACK)) {
                        //try {
                            AutoPtr<IAudioTrack2> audioTrack = ParseAudioTrack(parser);
                            AddAudioTrack(audioTrack);
                        //} catch (Exception ex) {
                        //    Log->W(TAG, "Cannot load audio track", ex);
                        //}
                    }
                    break;
                }

                case IXmlPullParser::END_TAG: {
                    parser->GetName(&name);
                    if (name.Equals(TAG_MEDIA_ITEM)) {
                        currentMediaItem = NULL;
                    }
                    else if (name.Equals(TAG_OVERLAY)) {
                        currentOverlay = NULL;
                    }
                    break;
                }

                default: {
                    break;
                }
            }
            parser->Next(&eventType);
        }
        ComputeTimelineDuration();
        // Regenerate project thumbnail
        if (regenerateProjectThumbnail) {
            GenerateProjectThumbnail();
            regenerateProjectThumbnail = FALSE;
        }
    //} finally {
        //if (fis != NULL) {
        //    fis->Close();
        //}
    //}
}

AutoPtr<IMediaItem> CVideoEditorImpl::ParseMediaItem(
    /* [in] */ IXmlPullParser* parser)
{
    String mediaItemId;
    parser->GetAttributeValueEx(String(""), ATTR_ID, &mediaItemId);
    String type;
    parser->GetAttributeValueEx(String(""), ATTR_TYPE, &type);
    String filename;
    parser->GetAttributeValueEx(String(""), ATTR_FILENAME, &filename);
    String str;
    parser->GetAttributeValueEx(String(""), ATTR_RENDERING_MODE, &str);
    Int32 renderingMode = StringUtils::ParseInt32(str);

    AutoPtr<IMediaItem> currentMediaItem;
assert(0);
    // if (MediaImageItem->Class->GetSimpleName()->Equals(type)) {
    if(type.Equals("MediaImageItem")) {
        parser->GetAttributeValueEx(String(""), ATTR_DURATION, &str);
        Int64 durationMs = StringUtils::ParseInt64(str);
        CMediaImageItem::New(this, mediaItemId, filename,
            durationMs, renderingMode, (IMediaImageItem**)&currentMediaItem);
    }
    // else if (MediaVideoItem->Class->GetSimpleName()->Equals(type)) {
    else if(type.Equals("MediaVideoItem")) {
        parser->GetAttributeValueEx(String(""), ATTR_BEGIN_TIME, &str);
        Int64 beginMs = StringUtils::ParseInt64(str);
        parser->GetAttributeValueEx(String(""), ATTR_END_TIME, &str);
        Int64 endMs = StringUtils::ParseInt64(str);
        parser->GetAttributeValueEx(String(""), ATTR_VOLUME, &str);
        Int32 volume = StringUtils::ParseInt32(str);
        parser->GetAttributeValueEx(String(""), ATTR_MUTED, &str);
        Boolean muted = str.EqualsIgnoreCase("TRUE") ? TRUE : FALSE;
        String audioWaveformFilename;
        parser->GetAttributeValueEx(String(""),
                ATTR_AUDIO_WAVEFORM_FILENAME, &audioWaveformFilename);
        CMediaVideoItem::New(this, mediaItemId, filename, renderingMode,
            beginMs, endMs, volume, muted, audioWaveformFilename, (IMediaVideoItem**)&currentMediaItem);

        parser->GetAttributeValueEx(String(""), ATTR_BEGIN_TIME, &str);
        Int64 beginTimeMs = StringUtils::ParseInt64(str);
        parser->GetAttributeValueEx(String(""), ATTR_END_TIME, &str);
        Int64 endTimeMs = StringUtils::ParseInt64(str);
        AutoPtr<IMediaVideoItem> item = IMediaVideoItem::Probe(currentMediaItem);
        item->SetExtractBoundaries(beginTimeMs, endTimeMs);

        parser->GetAttributeValueEx(String(""), ATTR_VOLUME, &str);
        Int32 volumePercent = StringUtils::ParseInt32(str);
        item->SetVolume(volumePercent);
    } else {
        //throw new IllegalArgumentException("Unknown media item type: " + type);
        return NULL;
    }

    return currentMediaItem;
}

AutoPtr<ITransition> CVideoEditorImpl::ParseTransition(
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ List<String>* ignoredMediaItems)
{
    String transitionId;
    parser->GetAttributeValueEx(String(""), ATTR_ID, &transitionId);
    String type;
    parser->GetAttributeValueEx(String(""), ATTR_TYPE, &type);
    String str;
    parser->GetAttributeValueEx(String(""), ATTR_DURATION, &str);
    Int64 durationMs = StringUtils::ParseInt64(str);
    parser->GetAttributeValueEx(String(""), ATTR_BEHAVIOR, &str);
    Int32 behavior = StringUtils::ParseInt32(str);

    String beforeMediaItemId;
    parser->GetAttributeValueEx(String(""), ATTR_BEFORE_MEDIA_ITEM_ID, &str);
    AutoPtr<IMediaItem> beforeMediaItem;
    if (beforeMediaItemId != NULL) {
        List<String>::Iterator it = ignoredMediaItems->Begin();
        for(; it != ignoredMediaItems->End(); it++) {
            if(beforeMediaItemId.Equals(*it)) {
                // This transition is ignored
                return NULL;
            }
        }

        GetMediaItem(beforeMediaItemId, (IMediaItem**)&beforeMediaItem);
    }
    else {
        beforeMediaItem = NULL;
    }

    String afterMediaItemId;
    parser->GetAttributeValueEx(String(""), ATTR_AFTER_MEDIA_ITEM_ID, &afterMediaItemId);
    AutoPtr<IMediaItem> afterMediaItem;
    if (afterMediaItemId != NULL) {
        List<String>::Iterator it = ignoredMediaItems->Begin();
        for(; it != ignoredMediaItems->End(); it++) {
            if(afterMediaItemId.Equals(*it)) {
                // This transition is ignored
                return NULL;
            }
        }

        GetMediaItem(afterMediaItemId, (IMediaItem**)&afterMediaItem);
    }
    else {
        afterMediaItem = NULL;
    }

    AutoPtr<ITransition> transition;
assert(0);
    // if (TransitionAlpha->Class->GetSimpleName()->Equals(type)) {
    if(type.Equals("TransitionAlpha")) {
        parser->GetAttributeValueEx(String(""), ATTR_BLENDING, &str);
        Int32 blending = StringUtils::ParseInt32(str);
        String maskFilename;
        parser->GetAttributeValueEx(String(""), ATTR_MASK, &maskFilename);
        parser->GetAttributeValueEx(String(""), ATTR_INVERT, &str);
        Boolean invert = str.EqualsIgnoreCase("TRUE") ? TRUE : FALSE;
        CTransitionAlpha::New(transitionId, afterMediaItem, beforeMediaItem,
                durationMs, behavior, maskFilename, blending, invert, (ITransitionAlpha**)&transition);
    }
    // else if (TransitionCrossfade->Class->GetSimpleName()->Equals(type)) {
    else if(type.Equals("TransitionCrossfade")) {
        CTransitionCrossfade::New(transitionId, afterMediaItem, beforeMediaItem,
                durationMs, behavior, (ITransitionCrossfade**)&transition);
    }
    // else if (TransitionSliding->Class->GetSimpleName()->Equals(type)) {
    else if(type.Equals("TransitionSliding")) {
        parser->GetAttributeValueEx(String(""), ATTR_DIRECTION, &str);
        Int32 direction = StringUtils::ParseInt32(str);
        CTransitionSliding::New(transitionId, afterMediaItem, beforeMediaItem,
                durationMs, behavior, direction, (ITransitionSliding**)&transition);
    }
    // else if (TransitionFadeBlack->Class->GetSimpleName()->Equals(type)) {
    else if(type.Equals("TransitionFadeBlack")) {
        CTransitionFadeBlack::New(transitionId, afterMediaItem, beforeMediaItem,
                durationMs, behavior, (ITransitionFadeBlack**)&transition);
    }
    else {
        //throw new IllegalArgumentException("Invalid transition type: " + type);
        return NULL;
    }

    parser->GetAttributeValueEx(String(""), ATTR_IS_TRANSITION_GENERATED, &str);
    Boolean isTransitionGenerated = str.EqualsIgnoreCase("TRUE") ? TRUE : FALSE;
    if (isTransitionGenerated == TRUE) {
        String transitionFile;
        parser->GetAttributeValueEx(String(""), ATTR_GENERATED_TRANSITION_CLIP, &transitionFile);

        AutoPtr<IFile> file;
        CFile::New(transitionFile, (IFile**)&file);
        Boolean b;
        file->Exists(&b);
        if (b) {
            (reinterpret_cast<Transition*>(transition->Probe(EIID_Transition)))->SetFilename(transitionFile);
        }
        else {
            (reinterpret_cast<Transition*>(transition->Probe(EIID_Transition)))->SetFilename(String(NULL));
        }
    }

    // Use the transition
    if (beforeMediaItem != NULL) {
        ((MediaItem*)beforeMediaItem.Get())->SetBeginTransition(transition);
    }

    if (afterMediaItem != NULL) {
        ((MediaItem*)afterMediaItem.Get())->SetEndTransition(transition);
    }

    return transition;
}

AutoPtr<IOverlay> CVideoEditorImpl::ParseOverlay(
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IMediaItem* mediaItem)
{
    String overlayId;
    parser->GetAttributeValueEx(String(""), ATTR_ID, &overlayId);
    String type;
    parser->GetAttributeValueEx(String(""), ATTR_TYPE, &type);
    String str;
    parser->GetAttributeValueEx(String(""), ATTR_DURATION, &str);
    Int64 durationMs = StringUtils::ParseInt64(str);
    parser->GetAttributeValueEx(String(""), ATTR_BEGIN_TIME, &str);
    Int64 startTimeMs = StringUtils::ParseInt64(str);

    AutoPtr<IOverlay> overlay;
assert(0);
    // if (OverlayFrame->Class->GetSimpleName()->Equals(type)) {
    if(type.Equals("OverlayFrame")) {
        String filename;
        parser->GetAttributeValueEx(String(""), ATTR_FILENAME, &filename);
        COverlayFrame::New(mediaItem, overlayId, filename, startTimeMs, durationMs, (IOverlayFrame**)&overlay);
    }
    else {
        //throw new IllegalArgumentException("Invalid overlay type: " + type);
        return NULL;
    }

    String overlayRgbFileName;
    parser->GetAttributeValueEx(String(""), ATTR_OVERLAY_RGB_FILENAME, &overlayRgbFileName);
    if (!overlayRgbFileName.IsNull()) {
        AutoPtr<IOverlayFrame> of = IOverlayFrame::Probe(overlay);
        ((COverlayFrame*)of.Get())->SetFilename(overlayRgbFileName);

        parser->GetAttributeValueEx(String(""), ATTR_OVERLAY_FRAME_WIDTH, &str);
        Int32 overlayFrameWidth = StringUtils::ParseInt32(str);
        parser->GetAttributeValueEx(String(""), ATTR_OVERLAY_FRAME_HEIGHT, &str);
        Int32 overlayFrameHeight = StringUtils::ParseInt32(str);

        ((COverlayFrame*)of.Get())->SetOverlayFrameWidth(overlayFrameWidth);
        ((COverlayFrame*)of.Get())->SetOverlayFrameHeight(overlayFrameHeight);

        parser->GetAttributeValueEx(String(""), ATTR_OVERLAY_RESIZED_RGB_FRAME_WIDTH, &str);
        Int32 resizedRGBFrameWidth = StringUtils::ParseInt32(str);
        parser->GetAttributeValueEx(String(""), ATTR_OVERLAY_RESIZED_RGB_FRAME_HEIGHT, &str);
        Int32 resizedRGBFrameHeight = StringUtils::ParseInt32(str);

        ((COverlayFrame*)of.Get())->SetResizedRGBSize(resizedRGBFrameWidth, resizedRGBFrameHeight);
    }

    return overlay;
}

AutoPtr<IEffect> CVideoEditorImpl::ParseEffect(
    /* [in] */ IXmlPullParser* parser,
    /* [in] */ IMediaItem* mediaItem)
{
    String effectId;
    parser->GetAttributeValueEx(String(""), ATTR_ID, &effectId);
    String type;
    parser->GetAttributeValueEx(String(""), ATTR_TYPE, &type);
    String str;
    parser->GetAttributeValueEx(String(""), ATTR_DURATION, &str);
    Int64 durationMs = StringUtils::ParseInt64(str);
    parser->GetAttributeValueEx(String(""), ATTR_BEGIN_TIME, &str);
    Int64 startTimeMs = StringUtils::ParseInt64(str);

    AutoPtr<IEffect> effect;
assert(0);
    // if (EffectColor->Class->GetSimpleName()->Equals(type)) {
    if(type.Equals("EffectColor")) {
        parser->GetAttributeValueEx(String(""), ATTR_COLOR_EFFECT_TYPE, &str);
        Int32 colorEffectType = StringUtils::ParseInt32(str);
        Int32 color;
        if (colorEffectType == IEffectColor::TYPE_COLOR
                || colorEffectType == IEffectColor::TYPE_GRADIENT) {
            parser->GetAttributeValueEx(String(""), ATTR_COLOR_EFFECT_VALUE, &str);
            color = StringUtils::ParseInt32(str);
        }
        else {
            color = 0;
        }
        CEffectColor::New(mediaItem, effectId, startTimeMs,
                durationMs, colorEffectType, color, (IEffectColor**)&effect);
    }
    // else if (EffectKenBurns->Class->GetSimpleName()->Equals(type)) {
    else if(type.Equals("EffectKenBurns")) {
        AutoPtr<IRect> startRect;
        String rl, rt, rr, rb;
        parser->GetAttributeValueEx(String(""), ATTR_START_RECT_LEFT, &rl);
        parser->GetAttributeValueEx(String(""), ATTR_START_RECT_TOP, &rt);
        parser->GetAttributeValueEx(String(""), ATTR_START_RECT_RIGHT, &rr);
        parser->GetAttributeValueEx(String(""), ATTR_START_RECT_BOTTOM, &rb);
        CRect::New(
                StringUtils::ParseInt32(rl),
                StringUtils::ParseInt32(rt),
                StringUtils::ParseInt32(rr),
                StringUtils::ParseInt32(rb),
                (IRect**)&startRect);
        AutoPtr<IRect> endRect;
        parser->GetAttributeValueEx(String(""), ATTR_END_RECT_LEFT, &rl);
        parser->GetAttributeValueEx(String(""), ATTR_END_RECT_TOP, &rt);
        parser->GetAttributeValueEx(String(""), ATTR_END_RECT_RIGHT, &rr);
        parser->GetAttributeValueEx(String(""), ATTR_END_RECT_BOTTOM, &rb);
        CRect::New(
                StringUtils::ParseInt32(rl),
                StringUtils::ParseInt32(rt),
                StringUtils::ParseInt32(rr),
                StringUtils::ParseInt32(rb),
                (IRect**)&endRect);
        CEffectKenBurns::New(mediaItem, effectId, startRect, endRect,
                                    startTimeMs, durationMs, (IEffectKenBurns**)&effect);
    }
    else {
        //throw new IllegalArgumentException("Invalid effect type: " + type);
        return NULL;
    }

    return effect;
}

AutoPtr<IAudioTrack2> CVideoEditorImpl::ParseAudioTrack(
    /* [in] */ IXmlPullParser* parser)
{
    String audioTrackId;
    parser->GetAttributeValueEx(String(""), ATTR_ID, &audioTrackId);
    String filename;
    parser->GetAttributeValueEx(String(""), ATTR_FILENAME, &filename);
    String str;
    parser->GetAttributeValueEx(String(""), ATTR_START_TIME, &str);
    Int64 startTimeMs = StringUtils::ParseInt64(str);
    parser->GetAttributeValueEx(String(""), ATTR_BEGIN_TIME, &str);
    Int64 beginMs = StringUtils::ParseInt64(str);
    parser->GetAttributeValueEx(String(""), ATTR_END_TIME, &str);
    Int64 endMs = StringUtils::ParseInt64(str);
    parser->GetAttributeValueEx(String(""), ATTR_VOLUME, &str);
    Int32 volume = StringUtils::ParseInt32(str);
    parser->GetAttributeValueEx(String(""), ATTR_MUTED, &str);
    Boolean muted = str.EqualsIgnoreCase("TRUE") ? TRUE : FALSE;
    parser->GetAttributeValueEx(String(""), ATTR_LOOP, &str);
    Boolean loop = str.EqualsIgnoreCase("TRUE") ? TRUE : FALSE;
    parser->GetAttributeValueEx(String(""), ATTR_DUCK_ENABLED, &str);
    Boolean duckingEnabled = str.EqualsIgnoreCase("TRUE") ? TRUE : FALSE;
    parser->GetAttributeValueEx(String(""), ATTR_DUCK_THRESHOLD, &str);
    Int32 duckThreshold = StringUtils::ParseInt32(str);
    parser->GetAttributeValueEx(String(""), ATTR_DUCKED_TRACK_VOLUME, &str);
    Int32 duckedTrackVolume = StringUtils::ParseInt32(str);

    String waveformFilename;
    parser->GetAttributeValueEx(String(""), ATTR_AUDIO_WAVEFORM_FILENAME, &waveformFilename);
    AutoPtr<IAudioTrack2> audioTrack;
    CAudioTrack2::New(this, audioTrackId,
        filename, startTimeMs,
        beginMs, endMs, loop,
        volume, muted,
        duckingEnabled,
        duckThreshold,
        duckedTrackVolume,
        waveformFilename,
        (IAudioTrack2**)&audioTrack);

    return audioTrack;
}

ECode CVideoEditorImpl::Save()
{
    AutoPtr<IXmlSerializer> serializer;
    CKXmlSerializer::New((IXmlSerializer**)&serializer);
    AutoPtr<IStringWriter> writer;
    CStringWriter::New((IStringWriter**)&writer);
    serializer->SetOutputEx(writer);
    AutoPtr<IBoolean> ib;
    CBoolean::New(TRUE, (IBoolean**)&ib);
    serializer->StartDocument(String("UTF-8"), ib);

    serializer->WriteStartTag(String(""), TAG_PROJECT);
    serializer->WriteAttribute(String(""),
                         ATTR_ASPECT_RATIO, StringUtils::Int32ToString(mAspectRatio));

    Boolean b;
    mMANativeHelper->GetAudioflag(&b);
    serializer->WriteAttribute(String(""), ATTR_REGENERATE_PCM,
                    StringUtils::BooleanToString(b));

    serializer->WriteStartTag(String(""), TAG_MEDIA_ITEMS);

    String str;
    Int64 value64;
    Int32 value32;
    List<AutoPtr<IMediaItem> >::Iterator it = mMediaItems.Begin();
    for(; it != mMediaItems.End(); it++) {
        AutoPtr<IMediaItem> mediaItem = *it;
        serializer->WriteStartTag(String(""), TAG_MEDIA_ITEM);
        mediaItem->GetId(&str);
        serializer->WriteAttribute(String(""), ATTR_ID, str);
//TODO: Need GetClass()
        // serializer->WriteAttribute(String(""), ATTR_TYPE, mediaItem->GetClass()->GetSimpleName());
        mediaItem->GetFilename(&str);
        serializer->WriteAttribute(String(""), ATTR_FILENAME, str);
        mediaItem->GetRenderingMode(&value32);
        serializer->WriteAttribute(String(""), ATTR_RENDERING_MODE, StringUtils::Int32ToString(value32));

        AutoPtr<IMediaVideoItem> mvi = IMediaVideoItem::Probe(mediaItem);
        AutoPtr<IMediaImageItem> mii = IMediaImageItem::Probe(mediaItem);
        if(mvi != NULL) {
            mvi->GetBoundaryBeginTime(&value64);
            serializer->WriteAttribute(String(""), ATTR_BEGIN_TIME,
                                 StringUtils::Int64ToString(value64));
            mvi->GetBoundaryEndTime(&value64);
            serializer->WriteAttribute(String(""), ATTR_END_TIME,
                                   StringUtils::Int64ToString(value64));
            mvi->GetVolume(&value32);
            serializer->WriteAttribute(String(""), ATTR_VOLUME,
                                         StringUtils::Int32ToString(value32));
            mvi->IsMuted(&b);
            serializer->WriteAttribute(String(""), ATTR_MUTED,
                                           StringUtils::BooleanToString(b));
            ((CMediaVideoItem*)mvi.Get())->GetAudioWaveformFilename(&str);
            if (!str.IsNull()) {
                serializer->WriteAttribute(String(""), ATTR_AUDIO_WAVEFORM_FILENAME, str);
            }
        }
        else if (mii != NULL) {
            mediaItem->GetTimelineDuration(&value64);
            serializer->WriteAttribute(String(""), ATTR_DURATION,
                    StringUtils::Int64ToString(value64));
        }

        AutoPtr<ArrayOf<IOverlay*> > overlays;
        mediaItem->GetAllOverlays((ArrayOf<IOverlay*>**)&overlays);
        if (overlays->GetLength() > 0) {
            serializer->WriteStartTag(String(""), TAG_OVERLAYS);
            for(Int32 i = 0; i < overlays->GetLength(); i++) {
                AutoPtr<IOverlay> overlay = (*overlays)[i];
                serializer->WriteStartTag(String(""), TAG_OVERLAY);
                overlay->GetId(&str);
                serializer->WriteAttribute(String(""), ATTR_ID, str);
//TODO: Need GetClass()
                // serializer->WriteAttribute(String(""), ATTR_TYPE, overlay->GetClass()->GetSimpleName());
                overlay->GetStartTime(&value64);
                serializer->WriteAttribute(String(""), ATTR_BEGIN_TIME,
                                     StringUtils::Int64ToString(value64));
                overlay->GetDuration(&value64);
                serializer->WriteAttribute(String(""), ATTR_DURATION,
                                      StringUtils::Int64ToString(value64));
                AutoPtr<IOverlayFrame> overlayFrame = IOverlayFrame::Probe(overlay);
                if (overlayFrame != NULL) {
                    GetPath(&str);
                    String tmpStr;
                    ((COverlayFrame*)overlayFrame.Get())->Save(str, &tmpStr);
                    ((COverlayFrame*)overlayFrame.Get())->GetBitmapImageFileName(&str);
                    if (!str.IsNull()) {
                        serializer->WriteAttribute(String(""), ATTR_FILENAME, str);
                    }

                    ((COverlayFrame*)overlayFrame.Get())->GetFilename(&str);
                    if (!str.IsNull()) {
                        serializer->WriteAttribute(String(""),
                                             ATTR_OVERLAY_RGB_FILENAME, str);
                        ((COverlayFrame*)overlayFrame.Get())->GetOverlayFrameWidth(&value32);
                        serializer->WriteAttribute(String(""), ATTR_OVERLAY_FRAME_WIDTH,
                                             StringUtils::Int32ToString(value32));
                        ((COverlayFrame*)overlayFrame.Get())->GetOverlayFrameHeight(&value32);
                        serializer->WriteAttribute(String(""), ATTR_OVERLAY_FRAME_HEIGHT,
                                             StringUtils::Int32ToString(value32));
                        ((COverlayFrame*)overlayFrame.Get())->GetResizedRGBSizeWidth(&value32);
                        serializer->WriteAttribute(String(""), ATTR_OVERLAY_RESIZED_RGB_FRAME_WIDTH,
                                             StringUtils::Int32ToString(value32));
                        ((COverlayFrame*)overlayFrame.Get())->GetResizedRGBSizeHeight(&value32);
                        serializer->WriteAttribute(String(""), ATTR_OVERLAY_RESIZED_RGB_FRAME_HEIGHT,
                                             StringUtils::Int32ToString(value32));

                    }

                }

                /**
                 *  Save the user attributes
                 */
                serializer->WriteStartTag(String(""), TAG_OVERLAY_USER_ATTRIBUTES);
                AutoPtr<IObjectStringMap> userAttributes;
                overlay->GetUserAttributes((IObjectStringMap**)&userAttributes);
                AutoPtr<ArrayOf<String> > keys;
                userAttributes->GetKeys((ArrayOf<String>**)&keys);
                for(Int32 j = 0; j < keys->GetLength(); j++) {
                    AutoPtr<ICharSequence> cs;
                    userAttributes->Get((*keys)[j], (IInterface**)&cs);
                    if(cs != NULL) {
                        cs->ToString(&str);
                        serializer->WriteAttribute(String(""), (*keys)[j], str);
                    }
                }
                serializer->WriteEndTag(String(""), TAG_OVERLAY_USER_ATTRIBUTES);

                serializer->WriteEndTag(String(""), TAG_OVERLAY);
            }
            serializer->WriteEndTag(String(""), TAG_OVERLAYS);
        }

        AutoPtr<ArrayOf<IEffect*> > effects;
        mediaItem->GetAllEffects((ArrayOf<IEffect*>**)&effects);
        if (effects->GetLength() > 0) {
            serializer->WriteStartTag(String(""), TAG_EFFECTS);
            for(Int32 i = 0; i < effects->GetLength(); i++) {
                AutoPtr<IEffect> effect = (*effects)[i];
                serializer->WriteStartTag(String(""), TAG_EFFECT);
                effect->GetId(&str);
                serializer->WriteAttribute(String(""), ATTR_ID, str);
//TODO: Need GetClass()
                // serializer->WriteAttribute(String(""), ATTR_TYPE, effect->GetClass()->GetSimpleName());
                effect->GetStartTime(&value64);
                serializer->WriteAttribute(String(""), ATTR_BEGIN_TIME,
                        StringUtils::Int64ToString(value64));
                effect->GetDuration(&value64);
                serializer->WriteAttribute(String(""), ATTR_DURATION,
                                       StringUtils::Int64ToString(value64));

                AutoPtr<IEffectColor> colorEffect = IEffectColor::Probe(effect);
                AutoPtr<IEffectKenBurns> effectKenBurns = IEffectKenBurns::Probe(effect);
                if (colorEffect != NULL) {
                    Int32 type;
                    colorEffect->GetType(&type);
                    serializer->WriteAttribute(String(""), ATTR_COLOR_EFFECT_TYPE,
                            StringUtils::Int32ToString(type));
                    if (type == IEffectColor::TYPE_COLOR ||
                            type == IEffectColor::TYPE_GRADIENT) {
                        Int32 color;
                        colorEffect->GetColor(&color);
                        serializer->WriteAttribute(String(""), ATTR_COLOR_EFFECT_VALUE,
                                StringUtils::Int32ToString(color));
                    }
                }
                else if (effectKenBurns != NULL) {
                    AutoPtr<IRect> startRect;
                    effectKenBurns->GetStartRect((IRect**)&startRect);
                    serializer->WriteAttribute(String(""), ATTR_START_RECT_LEFT,
                            StringUtils::Int32ToString(((CRect*)startRect.Get())->mLeft));
                    serializer->WriteAttribute(String(""), ATTR_START_RECT_TOP,
                            StringUtils::Int32ToString(((CRect*)startRect.Get())->mTop));
                    serializer->WriteAttribute(String(""), ATTR_START_RECT_RIGHT,
                            StringUtils::Int32ToString(((CRect*)startRect.Get())->mRight));
                    serializer->WriteAttribute(String(""), ATTR_START_RECT_BOTTOM,
                            StringUtils::Int32ToString(((CRect*)startRect.Get())->mBottom));

                    AutoPtr<IRect> endRect;
                    effectKenBurns->GetEndRect((IRect**)&endRect);
                    serializer->WriteAttribute(String(""), ATTR_END_RECT_LEFT,
                                            StringUtils::Int32ToString(((CRect*)endRect.Get())->mLeft));
                    serializer->WriteAttribute(String(""), ATTR_END_RECT_TOP,
                                             StringUtils::Int32ToString(((CRect*)endRect.Get())->mTop));
                    serializer->WriteAttribute(String(""), ATTR_END_RECT_RIGHT,
                                           StringUtils::Int32ToString(((CRect*)endRect.Get())->mRight));
                    serializer->WriteAttribute(String(""), ATTR_END_RECT_BOTTOM,
                            StringUtils::Int32ToString(((CRect*)endRect.Get())->mBottom));
                    AutoPtr<IMediaItem> mItem;
                    effect->GetMediaItem((IMediaItem**)&mItem);
                    AutoPtr<IMediaImageItem> mii = IMediaImageItem::Probe(mItem);
                    ((CMediaImageItem*)mii.Get())->GetGeneratedImageClip(&str);
                    if(str != NULL) {
                       serializer->WriteAttribute(String(""), ATTR_IS_IMAGE_CLIP_GENERATED,
                               StringUtils::BooleanToString(TRUE));
                       serializer->WriteAttribute(String(""), ATTR_GENERATED_IMAGE_CLIP, str);
                    }
                    else {
                        serializer->WriteAttribute(String(""), ATTR_IS_IMAGE_CLIP_GENERATED,
                             StringUtils::BooleanToString(FALSE));
                    }
                }

                serializer->WriteEndTag(String(""), TAG_EFFECT);
            }
            serializer->WriteEndTag(String(""), TAG_EFFECTS);
        }

        serializer->WriteEndTag(String(""), TAG_MEDIA_ITEM);
    }
    serializer->WriteEndTag(String(""), TAG_MEDIA_ITEMS);

    serializer->WriteStartTag(String(""), TAG_TRANSITIONS);

    List<AutoPtr<ITransition> >::Iterator itTransition = mTransitions.Begin();
    for(; itTransition != mTransitions.End(); itTransition++) {
        AutoPtr<ITransition> transition = *itTransition;
        serializer->WriteStartTag(String(""), TAG_TRANSITION);
        transition->GetId(&str);
        serializer->WriteAttribute(String(""), ATTR_ID, str);
//TODO: Need GetClass()
        // serializer->WriteAttribute(String(""), ATTR_TYPE, transition->GetClass()->GetSimpleName());
        transition->GetDuration(&value64);
        serializer->WriteAttribute(String(""), ATTR_DURATION, StringUtils::Int64ToString(value64));
        transition->GetBehavior(&value32);
        serializer->WriteAttribute(String(""), ATTR_BEHAVIOR, StringUtils::Int32ToString(value32));
        (reinterpret_cast<Transition*>((transition)->Probe(EIID_Transition)))->IsGenerated(&b);
        serializer->WriteAttribute(String(""), ATTR_IS_TRANSITION_GENERATED,
                                StringUtils::BooleanToString(b));
        if (b == TRUE) {
            (reinterpret_cast<Transition*>((transition)->Probe(EIID_Transition)))->GetFilename(&str);
            serializer->WriteAttribute(String(""), ATTR_GENERATED_TRANSITION_CLIP, str);
        }
        AutoPtr<IMediaItem> afterMediaItem;
        transition->GetAfterMediaItem((IMediaItem**)&afterMediaItem);
        if (afterMediaItem != NULL) {
            afterMediaItem->GetId(&str);
            serializer->WriteAttribute(String(""), ATTR_AFTER_MEDIA_ITEM_ID, str);
        }

        AutoPtr<IMediaItem> beforeMediaItem;
        transition->GetBeforeMediaItem((IMediaItem**)&beforeMediaItem);
        if (beforeMediaItem != NULL) {
            beforeMediaItem->GetId(&str);
            serializer->WriteAttribute(String(""), ATTR_BEFORE_MEDIA_ITEM_ID, str);
        }

        AutoPtr<ITransitionSliding> ts = ITransitionSliding::Probe(transition);
        AutoPtr<ITransitionAlpha> ta = ITransitionAlpha::Probe(transition);
        if (ts != NULL) {
            ts->GetDirection(&value32);
            serializer->WriteAttribute(String(""), ATTR_DIRECTION,
                    StringUtils::Int32ToString(value32));
        }
        else if (ta != NULL) {
            ta->GetBlendingPercent(&value32);
            serializer->WriteAttribute(String(""), ATTR_BLENDING,
                                 StringUtils::Int32ToString(value32));
            ta->IsInvert(&b);
            serializer->WriteAttribute(String(""), ATTR_INVERT,
                                           StringUtils::BooleanToString(b));
            ta->GetMaskFilename(&str);
            if (!str.IsNull()) {
                serializer->WriteAttribute(String(""), ATTR_MASK, str);
            }
        }
        serializer->WriteEndTag(String(""), TAG_TRANSITION);
    }
    serializer->WriteEndTag(String(""), TAG_TRANSITIONS);
    serializer->WriteStartTag(String(""), TAG_AUDIO_TRACKS);

    List<AutoPtr<IAudioTrack2> >::Iterator itAudioTrack = mAudioTracks.Begin();
    for(; itAudioTrack != mAudioTracks.End(); itAudioTrack++) {
        AutoPtr<IAudioTrack2> at = *itAudioTrack;
        serializer->WriteStartTag(String(""), TAG_AUDIO_TRACK);
        at->GetId(&str);
        serializer->WriteAttribute(String(""), ATTR_ID, str);
        at->GetFilename(&str);
        serializer->WriteAttribute(String(""), ATTR_FILENAME, str);
        at->GetStartTime(&value64);
        serializer->WriteAttribute(String(""), ATTR_START_TIME, StringUtils::Int64ToString(value64));
        at->GetBoundaryBeginTime(&value64);
        serializer->WriteAttribute(String(""), ATTR_BEGIN_TIME, StringUtils::Int64ToString(value64));
        at->GetBoundaryEndTime(&value64);
        serializer->WriteAttribute(String(""), ATTR_END_TIME, StringUtils::Int64ToString(value64));
        at->GetVolume(&value32);
        serializer->WriteAttribute(String(""), ATTR_VOLUME, StringUtils::Int32ToString(value32));
        at->IsDuckingEnabled(&b);
        serializer->WriteAttribute(String(""), ATTR_DUCK_ENABLED,
                                   StringUtils::BooleanToString(b));
        at->GetDuckedTrackVolume(&value32);
        serializer->WriteAttribute(String(""), ATTR_DUCKED_TRACK_VOLUME,
                               StringUtils::Int32ToString(value32));
        at->GetDuckingThreshhold(&value32);
        serializer->WriteAttribute(String(""), ATTR_DUCK_THRESHOLD,
                               StringUtils::Int32ToString(value32));
        at->IsMuted(&b);
        serializer->WriteAttribute(String(""), ATTR_MUTED, StringUtils::BooleanToString(b));
        at->IsLooping(&b);
        serializer->WriteAttribute(String(""), ATTR_LOOP, StringUtils::BooleanToString(b));
        at->GetAudioWaveformFilename(&str);
        if (str != NULL) {
            serializer->WriteAttribute(String(""), ATTR_AUDIO_WAVEFORM_FILENAME, str);
        }

        serializer->WriteEndTag(String(""), TAG_AUDIO_TRACK);
    }
    serializer->WriteEndTag(String(""), TAG_AUDIO_TRACKS);

    serializer->WriteEndTag(String(""), TAG_PROJECT);
    serializer->EndDocument();

    /**
     *  Save the metadata XML file
     */
    AutoPtr<IFileOutputStream> out;
    AutoPtr<IFile> file;
    GetPath(&str);
    CFile::New(str, PROJECT_FILENAME, (IFile**)&file);
    CFileOutputStream::New(file, (IFileOutputStream**)&out);
    writer->ToString(&str);
//TODO: Need String.GetBytes()
    // out->Write(str.GetBytes());
    IFlushable::Probe(out)->Flush();
    out->Close();

    return NOERROR;
}

ECode CVideoEditorImpl::SetAspectRatio(
    /* [in] */ Int32 aspectRatio)
{
    mAspectRatio = aspectRatio;
    /**
     *  Invalidate all transitions
     */
    mMANativeHelper->SetGeneratePreview(TRUE);

    List<AutoPtr<ITransition> >::Iterator it = mTransitions.Begin();
    for(; it != mTransitions.End(); it++) {
        (reinterpret_cast<Transition*>((*it)->Probe(EIID_Transition)))->Invalidate();
    }

    List<AutoPtr<IMediaItem> >::Iterator itMediaItem = mMediaItems.Begin();
    for(; itMediaItem != mMediaItems.End(); itMediaItem++) {
        AutoPtr<ArrayOf<IOverlay*> > overlays;
        (*itMediaItem)->GetAllOverlays((ArrayOf<IOverlay*>**)&overlays);
        for(Int32 i = 0; i < overlays->GetLength(); i++) {
            AutoPtr<IOverlayFrame> of = IOverlayFrame::Probe((*overlays)[i]);
            ((COverlayFrame*)of.Get())->InvalidateGeneratedFiles();
        }
    }
    return NOERROR;
}

ECode CVideoEditorImpl::StartPreview(
    /* [in] */ ISurfaceHolder* surfaceHolder,
    /* [in] */ Int64 fromMs,
    /* [in] */ Int64 toMs,
    /* [in] */ Boolean loop,
    /* [in] */ Int32 callbackAfterFrameCount,
    /* [in] */ IVideoEditorPreviewProgressListener* listener)
{

    if (surfaceHolder == NULL) {
        //throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoPtr<ISurface> surface;
    surfaceHolder->GetSurface((ISurface**)&surface);
    if (surface == NULL) {
        //throw new IllegalArgumentException("Surface could not be retrieved from surface holder");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Boolean b;
    surface->IsValid(&b);
    if (b == FALSE) {
        //throw new IllegalStateException("Surface is not valid");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    if (listener == NULL) {
        //throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (fromMs >= mDurationMs) {
        //throw new IllegalArgumentException("Requested time not correct");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (fromMs < 0) {
        //throw new IllegalArgumentException("Requested time not correct");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Boolean semAcquireDone = FALSE;
    if (!mPreviewInProgress) {
        //try{
            semAcquireDone = Lock(ENGINE_ACCESS_MAX_TIMEOUT_MS);
            if (semAcquireDone == FALSE) {
                //throw new IllegalStateException("Timeout waiting for semaphore");
                return E_ILLEGAL_STATE_EXCEPTION;

            }

            if (mMANativeHelper == NULL) {
                //throw new IllegalStateException("The video editor is not initialized");
                return E_ILLEGAL_STATE_EXCEPTION;

            }

            if (mMediaItems.GetSize() > 0) {
                mPreviewInProgress = TRUE;

                AutoPtr<ArrayOf<IMediaItem*> > mediaItemArray = ArrayOf<IMediaItem*>::Alloc(mMediaItems.GetSize());
                List<AutoPtr<IMediaItem> >::Iterator iter1 = mMediaItems.Begin();
                for(Int32 i = 0; iter1 != mMediaItems.End(); ++iter1) {
                    mediaItemArray->Set(i++, *iter1);
                }
                AutoPtr<ArrayOf<ITransition*> > transitionArray = ArrayOf<ITransition*>::Alloc(mTransitions.GetSize());
                List<AutoPtr<ITransition> >::Iterator iter2 = mTransitions.Begin();
                for(Int32 i = 0; iter2 != mTransitions.End(); ++iter2) {
                    transitionArray->Set(i++, *iter2);
                }
                AutoPtr<ArrayOf<IAudioTrack2*> > audioTrackArray = ArrayOf<IAudioTrack2*>::Alloc(mAudioTracks.GetSize());
                List<AutoPtr<IAudioTrack2> >::Iterator iter3 = mAudioTracks.Begin();
                for(Int32 i = 0; iter3 != mAudioTracks.End(); ++iter3) {
                    audioTrackArray->Set(i++, *iter3);
                }

                mMANativeHelper->PreviewStoryBoard(mediaItemArray, transitionArray,
                                                  audioTrackArray, NULL);
                mMANativeHelper->DoPreview(surface, fromMs, toMs, loop,
                                 callbackAfterFrameCount, listener);
            }
            /**
             *  Release The lock on complete by calling stopPreview
             */
        //} catch (InterruptedException ex) {
        //    Log.w(TAG, "The thread was interrupted", new Throwable());
            //throw new IllegalStateException("The thread was interrupted");
            // return E_ILLEGAL_STATE_EXCEPTION;
        //}
     }
     else {
        //throw new IllegalStateException("Preview already in progress");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    return NOERROR;
}

ECode CVideoEditorImpl::StopPreview(
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);

    Int64 re = 0;
    if (mPreviewInProgress) {
        //try {
            mMANativeHelper->StopPreview(&re);
            /**
             *  release on complete by calling stopPreview
             */
        //    } finally {
                mPreviewInProgress = FALSE;
                Unlock();
        //    }
        *result = re;
        return NOERROR;
    }
    else {
        *result = 0;
        return NOERROR;
    }
}

void CVideoEditorImpl::RemoveAdjacentTransitions(
    /* [in] */ IMediaItem* mediaItem)
{
    AutoPtr<ITransition> beginTransition;
    mediaItem->GetBeginTransition((ITransition**)&beginTransition);
    if (beginTransition != NULL) {
        AutoPtr<IMediaItem> item;
        beginTransition->GetAfterMediaItem((IMediaItem**)&item);
        if (item != NULL) {
            ((MediaItem*)item.Get())->SetEndTransition(NULL);
        }
        (reinterpret_cast<Transition*>(beginTransition->Probe(EIID_Transition)))->Invalidate();
        mTransitions.Remove(beginTransition);
    }

    AutoPtr<ITransition> endTransition;
    mediaItem->GetEndTransition((ITransition**)&endTransition);
    if (endTransition != NULL) {
        AutoPtr<IMediaItem> item;
        endTransition->GetBeforeMediaItem((IMediaItem**)&item);
        if (item != NULL) {
            ((MediaItem*)item.Get())->SetBeginTransition(NULL);
        }
        (reinterpret_cast<Transition*>(endTransition->Probe(EIID_Transition)))->Invalidate();
        mTransitions.Remove(endTransition);
    }

    ((MediaItem*)mediaItem)->SetBeginTransition(NULL);
    ((MediaItem*)mediaItem)->SetEndTransition(NULL);
}

void CVideoEditorImpl::RemoveTransitionBefore(
    /* [in] */ Int32 index)
{
    AutoPtr<IMediaItem> mediaItem = mMediaItems[index];
    List<AutoPtr<ITransition> >::Iterator it = mTransitions.Begin();
    for(; it != mTransitions.End(); it++) {
        AutoPtr<ITransition> t = *it;
        AutoPtr<IMediaItem> item;
        t->GetBeforeMediaItem((IMediaItem**)&item);
        if (item == mediaItem) {
            mMANativeHelper->SetGeneratePreview(TRUE);
            mTransitions.Remove(*it);
            (reinterpret_cast<Transition*>(t->Probe(EIID_Transition)))->Invalidate();
            ((MediaItem*)mediaItem.Get())->SetBeginTransition(NULL);
            if (index > 0) {
                ((MediaItem*)(mMediaItems[index - 1]).Get())->SetEndTransition(NULL);
            }
            break;
        }
    }
}

void CVideoEditorImpl::RemoveTransitionAfter(
    /* [in] */ Int32 index)
{
    AutoPtr<IMediaItem> mediaItem = mMediaItems[index];
    List<AutoPtr<ITransition> >::Iterator it = mTransitions.Begin();
    for(; it != mTransitions.End(); it++) {
        AutoPtr<ITransition> t = *it;
        AutoPtr<IMediaItem> item;
        t->GetAfterMediaItem((IMediaItem**)&item);
        if (item == mediaItem) {
            mMANativeHelper->SetGeneratePreview(TRUE);
            mTransitions.Remove(*it);
            (reinterpret_cast<Transition*>(t->Probe(EIID_Transition)))->Invalidate();
            ((MediaItem*)mediaItem.Get())->SetEndTransition(NULL);
            /**
             *  Invalidate the reference in the next media item
             */
            if (index < mMediaItems.GetSize() - 1) {
                ((MediaItem*)(mMediaItems[index + 1]).Get())->SetBeginTransition(NULL);
            }
            break;
        }
    }
}

void CVideoEditorImpl::ComputeTimelineDuration()
{
    mDurationMs = 0;
    List<AutoPtr<IMediaItem> >::Iterator it = mMediaItems.Begin();
    for(Int32 i = 0; it != mMediaItems.End(); it++, i++) {
        AutoPtr<IMediaItem> mediaItem = *it;
        Int64 value;
        mediaItem->GetTimelineDuration(&value);
        mDurationMs += value;
        AutoPtr<ITransition> transition;
        mediaItem->GetEndTransition((ITransition**)&transition);
        if (transition != NULL) {
            if (i < mMediaItems.GetSize() - 1) {
                transition->GetDuration(&value);
                mDurationMs -= value;
            }
        }
    }
}

void CVideoEditorImpl::GenerateProjectThumbnail()
{
    /*
     * If a thumbnail already exists, then delete it first
     */
    AutoPtr<IFile> file;
    CFile::New(mProjectPath + "/" + THUMBNAIL_FILENAME, (IFile**)&file);
    Boolean b;
    file->Exists(&b);
    if (b) {
        file->Delete(&b);
    }
    /*
     * Generate a new thumbnail for the project from first media Item
     */
    if (mMediaItems.GetSize() > 0) {
        AutoPtr<IMediaItem> mI = mMediaItems[0];
        /*
         * Keep aspect ratio of the image
         */
        Int32 height = 480;
        Int32 mw, mh;
        mI->GetWidth(&mw);
        mI->GetHeight(&mh);
        Int32 width = mw * height / mh;

        AutoPtr<IBitmap> projectBitmap;
        String filename;
        mI->GetFilename(&filename);

        AutoPtr<IMediaVideoItem> item = IMediaVideoItem::Probe(mI);
        if (item != NULL) {
            AutoPtr<IMediaMetadataRetriever> retriever;
//TODO: Need media/CMediaMetadataRetriever
            // CMediaMetadataRetriever::New((IMediaMetadataRetriever**)&retriever);
            retriever->SetDataSource(filename);
            AutoPtr<IBitmap> bitmap;
            retriever->GetFrameAtTimeEx2((IBitmap**)&bitmap);
            retriever->ReleaseResources();
            retriever = NULL;
            if (bitmap == NULL) {
                // String msg = "Thumbnail extraction from " +
                //                 filename + " failed";
                // throw new IllegalArgumentException(msg);
                return;
            }
            // Resize the thumbnail to the target size
            AutoPtr<IBitmapFactory> factory;
            CBitmapFactory::AcquireSingleton((IBitmapFactory**)&factory);
            factory->CreateScaledBitmap(bitmap, width, height, TRUE, (IBitmap**)&projectBitmap);
        }
        else {
            //try {
                mI->GetThumbnail(width, height, 500, (IBitmap**)&projectBitmap);
            //} catch (IllegalArgumentException e) {
                // String msg = "Project thumbnail extraction from " +
                //                 filename + " failed";
                // throw new IllegalArgumentException(msg);
                // return;
            //} catch (IOException e) {
                // String msg = "IO Error creating project thumbnail";
                // throw new IllegalArgumentException(msg);
                // return;
            //}
        }

        //try {
            AutoPtr<IFileOutputStream> stream;
            CFileOutputStream::New(mProjectPath + "/" + THUMBNAIL_FILENAME, (IFileOutputStream**)&stream);
            projectBitmap->Compress(Elastos::Droid::Graphics::BitmapCompressFormat_JPEG, 100, stream, &b);
            IFlushable::Probe(stream)->Flush();
            stream->Close();
        //} catch (IOException e) {
            //throw new IllegalArgumentException ("Error creating project thumbnail");
            //return E_ILLEGAL_ARGUMENT_EXCEPTION;
        //} finally {
            projectBitmap->Recycle();
        //}
    }
}

ECode CVideoEditorImpl::ClearSurface(
    /* [in] */ ISurfaceHolder* surfaceHolder)
{
    if (surfaceHolder == NULL) {
        //throw new IllegalArgumentException("Invalid surface holder");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoPtr<ISurface> surface;
    surfaceHolder->GetSurface((ISurface**)&surface);
    if (surface == NULL) {
        //throw new IllegalArgumentException("Surface could not be retrieved from surface holder");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Boolean b;
    surface->IsValid(&b);
    if (b == FALSE) {
        //throw new IllegalStateException("Surface is not valid");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    if (mMANativeHelper != NULL) {
        mMANativeHelper->ClearPreviewSurface(surface);
    }
    else {
    //    Log->W(TAG, "Native helper was not ready!");
    }

    return NOERROR;
}

void CVideoEditorImpl::Lock()
{
    // if (Log->IsLoggable(TAG, Log->DEBUG)) {
        //Log->D(TAG, "lock: grabbing semaphore", new Throwable());
    // }
    mLock->Acquire();
    // if (Log->IsLoggable(TAG, Log->DEBUG)) {
        //Log->D(TAG, "lock: grabbed semaphore");
    // }
}

Boolean CVideoEditorImpl::Lock(
        /* [in] */ Int64 timeoutMs)
{
    // if (Log->IsLoggable(TAG, Log->DEBUG)) {
       // Log->D(TAG, "lock: grabbing semaphore with timeout " + timeoutMs, new Throwable());
    // }

    AutoPtr<ITimeUnitHelper> tuHelper;
    CTimeUnitHelper::AcquireSingleton((ITimeUnitHelper**)&tuHelper);
    AutoPtr<ITimeUnit> seconds;
    tuHelper->GetMILLISECONDS((ITimeUnit**)&seconds);

    Boolean acquireSem;
    mLock->TryAcquireEx(timeoutMs, seconds, &acquireSem);
    // if (Log->IsLoggable(TAG, Log->DEBUG)) {
        //Log->D(TAG, "lock: grabbed semaphore status " + acquireSem);
    // }

    return acquireSem;
}

void CVideoEditorImpl::Unlock()
{
    // if (Log->IsLoggable(TAG, Log->DEBUG)) {
       // Log->D(TAG, "unlock: releasing semaphore");
    // }
    mLock->ToRelease();
}

void CVideoEditorImpl::DumpHeap(
    /* [in] */ const String& filename)
{
    /* Cleanup as much as possible before dump
     */
//TODO:
    // System->Gc();
    // System->RunFinalization();
    // Thread->Sleep(1000);
    // String state = Environment->GetExternalStorageState();
    // if (Environment->MEDIA_MOUNTED->Equals(state)) {
    //     String extDir =
    //      Environment->GetExternalStorageDirectory()->ToString();

    //     /* If dump file already exists, then delete it first
    //     */
    //     if ((new File(extDir + "/" + filename + "->Dump"))->Exists()) {
    //         (new File(extDir + "/" + filename + "->Dump"))->Delete();
    //     }
    //     /* Dump native heap
    //     */
    //     FileOutputStream ost =
    //      new FileOutputStream(extDir + "/" + filename + "->Dump");
    //     Debug->DumpNativeHeap(ost->GetFD());
    //     ost->Close();
    // }
}

} // namespace Videoeditor
} // namespace Media
} // namepsace Droid
} // namespace Elastos
