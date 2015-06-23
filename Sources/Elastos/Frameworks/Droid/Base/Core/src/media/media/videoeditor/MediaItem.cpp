#include "MediaItem.h"
#include "CVideoEditorImpl.h"
#include "COverlayFrame.h"
#include "Transition.h"
#include "CMediaImageItem.h"

using Elastos::IO::IFile;
using Elastos::IO::CFile;
using Elastos::IO::IDataOutputStream;
using Elastos::IO::CDataOutputStream;
using Elastos::IO::IFileOutputStream;
using Elastos::IO::CFileOutputStream;
using Elastos::IO::IByteBufferHelper;
using Elastos::IO::CByteBufferHelper;
using Elastos::IO::IByteBuffer;
using Elastos::IO::IInt32Buffer;

namespace Elastos {
namespace Droid {
namespace Media {
namespace Videoeditor {

//6286d865-5661-494b-b5e7-87c12e686ad2
extern "C" const InterfaceID EIID_MediaItem =
        { 0x6286d865, 0x5661, 0x494b, { 0xb5, 0xe7, 0x87, 0xc1, 0x2e, 0x68, 0x6a, 0xd2 } };

//============================================================================================
// MediaItem::GetThumbnailListCallback
//============================================================================================
CAR_INTERFACE_IMPL(MediaItem::GetThumbnailListCallback, IMediaItemGetThumbnailListCallback)

ECode MediaItem::GetThumbnailListCallback::OnThumbnail(
    /* [in] */ IBitmap* bitmap,
    /* [in] */ Int32 index)
{
    mBitmaps->Set(index, bitmap);
    return NOERROR;
}

MediaItem::MediaItem()
{}

MediaItem::MediaItem(
    /* [in] */ IVideoEditor* editor,
    /* [in] */ const String& mediaItemId,
    /* [in] */ const String& filename,
    /* [in] */ Int32 renderingMode)
    : mUniqueId(NULL)
    , mRenderingMode(0)
    , mProjectPath(NULL)
    , mBlankFrameGenerated(FALSE)
    , mBlankFrameFilename(NULL)
    , mFilename(NULL)
    , mGeneratedImageClip(NULL)
    , mRegenerateClip(FALSE)
{
    Init(editor, mediaItemId, filename, renderingMode);
}

ECode MediaItem::Init(
    /* [in] */ IVideoEditor* editor,
    /* [in] */ const String& mediaItemId,
    /* [in] */ const String& filename,
    /* [in] */ Int32 renderingMode)
{
    if (filename == NULL) {
        //throw new IllegalArgumentException("MediaItem : filename is NULL");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    AutoPtr<IFile> file;
    CFile::New(filename, (IFile**)&file);
    Boolean b;
    file->Exists(&b);
    if (!b) {
        //throw new IOException(filename + " not found ! ");
        return E_IO_EXCEPTION;
    }

    /*Compare file_size with 2GB*/
    Int64 len;
    file->GetLength(&len);
    if (IVideoEditor::MAX_SUPPORTED_FILE_SIZE <= len) {
        //throw new IllegalArgumentException("File size is more than 2GB");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mUniqueId = mediaItemId;
    mFilename = filename;
    mRenderingMode = renderingMode;
    ((CVideoEditorImpl*)IVideoEditorImpl::Probe(editor))->GetNativeContext((MediaArtistNativeHelper**)&mMANativeHelper);
    editor->GetPath(&mProjectPath);
    return NOERROR;
}

ECode MediaItem::GetId(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mUniqueId;
    return NOERROR;
}

ECode MediaItem::GetFilename(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mFilename;
    return NOERROR;
}

ECode MediaItem::SetRenderingMode(
    /* [in] */ Int32 renderingMode)
{
    switch (renderingMode) {
        case IMediaItem::RENDERING_MODE_BLACK_BORDER:
        case IMediaItem::RENDERING_MODE_STRETCH:
        case IMediaItem::RENDERING_MODE_CROPPING:
            break;

        default:
            // throw new IllegalArgumentException("Invalid Rendering Mode");
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    mMANativeHelper->SetGeneratePreview(TRUE);

    mRenderingMode = renderingMode;
    if (mBeginTransition != NULL) {
        (reinterpret_cast<Transition*>(mBeginTransition->Probe(EIID_Transition)))->Invalidate();
    }

    if (mEndTransition != NULL) {
        (reinterpret_cast<Transition*>(mEndTransition->Probe(EIID_Transition)))->Invalidate();
    }

    List<AutoPtr<IOverlay> >::Iterator it = mOverlays.Begin();
    for (; it != mOverlays.End(); it++) {
        AutoPtr<IOverlay> ol = *it;
        ((COverlayFrame*)IOverlayFrame::Probe(ol))->InvalidateGeneratedFiles();
    }

    return NOERROR;
}

ECode MediaItem::GetRenderingMode(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mRenderingMode;
    return NOERROR;
}

ECode MediaItem::SetBeginTransition(
    /* [in] */ ITransition* transition)
{
    mBeginTransition = transition;
    return NOERROR;
}

ECode MediaItem::GetBeginTransition(
    /* [out] */ ITransition** result)
{
    VALIDATE_NOT_NULL(result);

    *result = mBeginTransition;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode MediaItem::SetEndTransition(
    /* [in] */ ITransition* transition)
{
    mEndTransition = transition;
    return NOERROR;
}

ECode MediaItem::GetEndTransition(
    /* [out] */ ITransition** result)
{
    VALIDATE_NOT_NULL(result);

    *result = mEndTransition;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode MediaItem::AddEffect(
    /* [in] */ IEffect* effect)
{
    if (effect == NULL) {
        // throw new IllegalArgumentException("NULL effect cannot be applied");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoPtr<IMediaItem> mi;
    effect->GetMediaItem((IMediaItem**)&mi);
    if ((reinterpret_cast<MediaItem*>(mi->Probe(EIID_MediaItem))) != this) {
        // throw new IllegalArgumentException("Media item mismatch");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    List<AutoPtr<IEffect> >::Iterator it = mEffects.Begin();
    for(; it != mEffects.End(); it++) {
        if(*it == effect) {
            // throw new IllegalArgumentException("Effect already exists: " + effect.getId());
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
    }

    Int64 sTime, eDuration, duration;
    effect->GetStartTime(&sTime);
    effect->GetDuration(&eDuration);
    GetDuration(&duration);
    if (sTime + eDuration > duration) {
        // throw new IllegalArgumentException(
        // "Effect start time + effect duration > media clip duration");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    mMANativeHelper->SetGeneratePreview(TRUE);

    mEffects.PushBack(effect);

    InvalidateTransitions(sTime, eDuration);

    AutoPtr<IEffectKenBurns> ekb = (IEffectKenBurns*)IEffectKenBurns::Probe(effect);
    if (ekb != NULL) {
        mRegenerateClip = TRUE;
    }

    return NOERROR;
}

ECode MediaItem::RemoveEffect(
    /* [in] */ const String& effectId,
    /* [out] */ IEffect** result)
{
    VALIDATE_NOT_NULL(result);

    List<AutoPtr<IEffect> >::Iterator it = mEffects.Begin();
    for(; it != mEffects.End(); it++) {
        AutoPtr<IEffect> effect = *it;
        String id;
        effect->GetId(&id);
        if(id.Equals(effectId)) {
            mMANativeHelper->SetGeneratePreview(TRUE);

            mEffects.Remove(effect);

            Int64 sTime, eDuration;
            effect->GetStartTime(&sTime);
            effect->GetDuration(&eDuration);
            InvalidateTransitions(sTime, eDuration);

            AutoPtr<IEffectKenBurns> ekb = (IEffectKenBurns*)IEffectKenBurns::Probe(effect);
            if (ekb != NULL) {
                if (mGeneratedImageClip != NULL) {
                    /**
                     *  Delete the file
                     */
                    AutoPtr<IFile> file;
                    CFile::New(mGeneratedImageClip, (IFile**)&file);
                    Boolean b;
                    file->Delete(&b);
                    /**
                     *  Invalidate the filename
                     */
                    mGeneratedImageClip = NULL;
                }
                mRegenerateClip = FALSE;
            }
            *result = effect;
            REFCOUNT_ADD(*result);
            return NOERROR;
        }
    }
    *result = NULL;
    return NOERROR;
}

ECode MediaItem::SetGeneratedImageClip(
    /* [in] */ const String& generatedFilePath)
{
    mGeneratedImageClip = generatedFilePath;
    return NOERROR;
}


ECode MediaItem::GetGeneratedImageClip(
        /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result)
    *result = mGeneratedImageClip;
    return NOERROR;
}

ECode MediaItem::GetEffect(
    /* [in] */ const String& effectId,
    /* [out] */ IEffect** result)
{
    VALIDATE_NOT_NULL(result);

    List<AutoPtr<IEffect> >::Iterator it = mEffects.Begin();
    for(; it != mEffects.End(); it++) {
        AutoPtr<IEffect> effect = *it;
        String id;
        effect->GetId(&id);
        if(id.Equals(effectId)) {
            *result = effect;
            REFCOUNT_ADD(*result);
            return NOERROR;
        }
    }
    *result = NULL;
    return NOERROR;
}

ECode MediaItem::GetAllEffects(
    /* [out] */ ArrayOf<IEffect*>** result)
{
    VALIDATE_NOT_NULL(result);

    Int32 size = mEffects.GetSize();
    if (size > 0) {
        AutoPtr<ArrayOf<IEffect*> > tmp = ArrayOf<IEffect*>::Alloc(size);
        List<AutoPtr<IEffect> >::Iterator it = mEffects.Begin();
        for (Int32 i = 0; it != mEffects.End(); it++, i++) {
            tmp->Set(i, *it);
        }
        *result = tmp;
        REFCOUNT_ADD(*result);
    }
    return NOERROR;
}

ECode MediaItem::AddOverlay(
    /* [in] */ IOverlay* overlay)
{
    if (overlay == NULL) {
        // throw new IllegalArgumentException("NULL Overlay cannot be applied");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoPtr<IMediaItem> mi;
    overlay->GetMediaItem((IMediaItem**)&mi);
    if ((reinterpret_cast<MediaItem*>(mi->Probe(EIID_MediaItem))) != this) {
        // throw new IllegalArgumentException("Media item mismatch");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    List<AutoPtr<IOverlay> >::Iterator it = mOverlays.Begin();
    for(; it != mOverlays.End(); it++) {
        if(*it == overlay) {
            // throw new IllegalArgumentException("Effect already exists: " + effect.getId());
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
    }

    Int64 sTime, eDuration, duration;
    overlay->GetStartTime(&sTime);
    overlay->GetDuration(&eDuration);
    GetDuration(&duration);
    if (sTime + eDuration > duration) {
        // throw new IllegalArgumentException(
        // "Overlay start time + overlay duration > media clip duration");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    AutoPtr<IOverlayFrame> frame = (IOverlayFrame*)IOverlayFrame::Probe(overlay);
    if(frame != NULL) {
        AutoPtr<IBitmap> bitmap;
        frame->GetBitmap((IBitmap**)&bitmap);
        if(bitmap != NULL) {
            // throw new IllegalArgumentException("Overlay bitmap not specified");
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }

        Int32 scaledWidth, scaledHeight;
        AutoPtr<IMediaVideoItem> item = THIS_PROBE(IMediaVideoItem);
        if (item != NULL) {
            GetWidth(&scaledWidth);
            GetHeight(&scaledHeight);
        }
        else {
            AutoPtr<IMediaImageItem> iItem = THIS_PROBE(IMediaImageItem);
            iItem->GetScaledWidth(&scaledWidth);
            iItem->GetScaledHeight(&scaledHeight);
        }

        /**
         * The dimensions of the overlay bitmap must be the same as the
         * media item dimensions
         */
        Int32 width, height;
        bitmap->GetWidth(&width);
        bitmap->GetHeight(&height);
        if (width != scaledWidth || height != scaledHeight) {
            // throw new IllegalArgumentException(
            // "Bitmap dimensions must match media item dimensions");
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }

        mMANativeHelper->SetGeneratePreview(TRUE);
        String strResult;
        ((COverlayFrame*)IOverlayFrame::Probe(overlay))->Save(mProjectPath, &strResult);

        mOverlays.PushBack(overlay);
        InvalidateTransitions(sTime, eDuration);
        return NOERROR;
    }
    else {
        // throw new IllegalArgumentException("Overlay not supported");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
}

ECode MediaItem::SetRegenerateClip(
    /* [in] */ Boolean flag)
{
    mRegenerateClip = flag;
    return NOERROR;
}

ECode MediaItem::GetRegenerateClip(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mRegenerateClip;
    return NOERROR;
}

ECode MediaItem::RemoveOverlay(
    /* [in] */ const String& overlayId,
    /* [out] */ IOverlay** result)
{
    VALIDATE_NOT_NULL(result);

    List<AutoPtr<IOverlay> >::Iterator it = mOverlays.Begin();
    for(; it != mOverlays.End(); it++) {
        AutoPtr<IOverlay> overlay = *it;
        String id;
        overlay->GetId(&id);
        if(id.Equals(overlayId)) {
            mMANativeHelper->SetGeneratePreview(TRUE);

            mOverlays.Remove(overlay);

            Int64 sTime, eDuration;
            overlay->GetStartTime(&sTime);
            overlay->GetDuration(&eDuration);
            InvalidateTransitions(sTime, eDuration);
            *result = overlay;
            REFCOUNT_ADD(*result);
            return NOERROR;
        }
    }
    *result = NULL;
    return NOERROR;
}

ECode MediaItem::GetOverlay(
    /* [in] */ const String& overlayId,
    /* [out] */ IOverlay** result)
{
    VALIDATE_NOT_NULL(result);

    List<AutoPtr<IOverlay> >::Iterator it = mOverlays.Begin();
    for(; it != mOverlays.End(); it++) {
        AutoPtr<IOverlay> overlay = *it;
        String id;
        overlay->GetId(&id);
        if(id.Equals(overlayId)) {
            *result = overlay;
            REFCOUNT_ADD(*result);
            return NOERROR;
        }
    }
    *result = NULL;
    return NOERROR;
}

ECode MediaItem::GetAllOverlays(
    /* [out] */ ArrayOf<IOverlay*>** result)
{
    VALIDATE_NOT_NULL(result);

    Int32 size = mOverlays.GetSize();
    if (size > 0) {
        AutoPtr<ArrayOf<IOverlay*> > tmp = ArrayOf<IOverlay*>::Alloc(size);
        List<AutoPtr<IOverlay> >::Iterator it = mOverlays.Begin();
        for (Int32 i = 0; it != mOverlays.End(); it++, i++) {
            tmp->Set(i, *it);
        }
        *result = tmp;
        REFCOUNT_ADD(*result);
    }
    return NOERROR;
}

ECode MediaItem::GetThumbnailList(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int64 startMs,
    /* [in] */ Int64 endMs,
    /* [in] */ Int32 thumbnailCount,
    /* [out,callee] */ ArrayOf<IBitmap*>** result)
{
    VALIDATE_NOT_NULL(result);

    AutoPtr<ArrayOf<IBitmap*> > bitmaps = ArrayOf<IBitmap*>::Alloc(thumbnailCount);
    AutoPtr<ArrayOf<Int32> > indices = ArrayOf<Int32>::Alloc(thumbnailCount);
    for (Int32 i = 0; i < thumbnailCount; i++) {
        (*indices)[i] = i;
    }

    AutoPtr<GetThumbnailListCallback> cb = new GetThumbnailListCallback(this, bitmaps);
    GetThumbnailList(width, height, startMs, endMs, thumbnailCount, indices, cb);

    *result = bitmaps;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode MediaItem::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    if (!IMediaItem::Probe(object)) {
        *result = FALSE;
        return NOERROR;
    }
    *result = mUniqueId.Equals(((MediaItem*)IMediaItem::Probe(object))->mUniqueId);
    return NOERROR;
}

ECode MediaItem::GetHashCode(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)
    *result = mUniqueId.GetHashCode();
    return NOERROR;
}

Boolean MediaItem::IsOverlapping(
    /* [in]*/ Int64 startTimeMs1,
    /* [in]*/ Int64 durationMs1,
    /* [in]*/ Int64 startTimeMs2,
    /* [in]*/ Int64 durationMs2)
{
    if (startTimeMs1 + durationMs1 <= startTimeMs2) {
        return FALSE;
    }
    else if (startTimeMs1 >= startTimeMs2 + durationMs2) {
        return FALSE;
    }

    return TRUE;
}

void MediaItem::AdjustTransitions()
{
    /**
     *  Check if the duration of transitions need to be adjusted
     */
    if (mBeginTransition != NULL) {
        Int64 maxDurationMs;
        mBeginTransition->GetMaximumDuration(&maxDurationMs);
        Int64 duration;
        mBeginTransition->GetDuration(&duration);
        if (duration > maxDurationMs) {
            mBeginTransition->SetDuration(maxDurationMs);
        }
    }

    if (mEndTransition != NULL) {
        Int64 maxDurationMs;
        mEndTransition->GetMaximumDuration(&maxDurationMs);
        Int64 duration;
        mEndTransition->GetDuration(&duration);
        if (duration > maxDurationMs) {
            mEndTransition->SetDuration(maxDurationMs);
        }
    }
}

ECode MediaItem::GetNativeContext(
    /* [out] */ MediaArtistNativeHelper** helper)
{
    VALIDATE_NOT_NULL(helper);
    *helper = mMANativeHelper;
    return NOERROR;
}

ECode MediaItem::InitClipSettings(
    /* [in] */ MediaArtistNativeHelper::ClipSettings* clipSettings)
{
    clipSettings->clipPath = NULL;
    clipSettings->clipDecodedPath = NULL;
    clipSettings->clipOriginalPath = NULL;
    clipSettings->fileType = 0;
    clipSettings->endCutTime = 0;
    clipSettings->beginCutTime = 0;
    clipSettings->beginCutPercent = 0;
    clipSettings->endCutPercent = 0;
    clipSettings->panZoomEnabled = FALSE;
    clipSettings->panZoomPercentStart = 0;
    clipSettings->panZoomTopLeftXStart = 0;
    clipSettings->panZoomTopLeftYStart = 0;
    clipSettings->panZoomPercentEnd = 0;
    clipSettings->panZoomTopLeftXEnd = 0;
    clipSettings->panZoomTopLeftYEnd = 0;
    clipSettings->mediaRendering = 0;
    clipSettings->rgbWidth = 0;
    clipSettings->rgbHeight = 0;

    return NOERROR;
}

ECode MediaItem::GetClipSettings(
    /* [out] */ MediaArtistNativeHelper::ClipSettings** cs)
{
    VALIDATE_NOT_NULL(cs);

    AutoPtr<MediaArtistNativeHelper::ClipSettings> clipSettings = new MediaArtistNativeHelper::ClipSettings();
    InitClipSettings(clipSettings);
    AutoPtr<IMediaVideoItem> mVI = THIS_PROBE(IMediaVideoItem);
    AutoPtr<IMediaImageItem> mII = THIS_PROBE(IMediaImageItem);
    if (mVI != NULL) {
        mVI->GetFilename(&clipSettings->clipPath);
        Int32 type;
        mVI->GetFileType(&type);
        mMANativeHelper->GetMediaItemFileType(type, &clipSettings->fileType);
        mVI->GetBoundaryBeginTime((Int64*)&clipSettings->beginCutTime);
        mVI->GetBoundaryEndTime((Int64*)&clipSettings->endCutTime);
        Int32 mode;
        mVI->GetRenderingMode(&mode);
        mMANativeHelper->GetMediaItemRenderingMode(mode, &clipSettings->mediaRendering);
    }
    else if (mII != NULL) {
        ((CMediaImageItem*)mII.Get())->GetImageClipProperties((MediaArtistNativeHelper::ClipSettings**)&clipSettings);
    }
    *cs = clipSettings;
    return NOERROR;
}

ECode MediaItem::GenerateBlankFrame(
    /* [in] */ MediaArtistNativeHelper::ClipSettings* clipSettings)
{
    if (!mBlankFrameGenerated) {
        Int32 mWidth = 64;
        Int32 mHeight = 64;
        mBlankFrameFilename = mProjectPath + "/" + "ghost.rgb";
        AutoPtr<IFileOutputStream> fl;
        //try {
             CFileOutputStream::New(mBlankFrameFilename, (IFileOutputStream**)&fl);
        //} catch (IOException e) {
            /* catch IO exception */
        //}
        AutoPtr<IDataOutputStream> dos;
        CDataOutputStream::New(fl, (IDataOutputStream**)&dos);

        AutoPtr<ArrayOf<Int32> > framingBuffer = ArrayOf<Int32>::Alloc(mWidth);

        AutoPtr<IByteBufferHelper> helper;
        CByteBufferHelper::AcquireSingleton((IByteBufferHelper**)&helper);
        AutoPtr<IByteBuffer> byteBuffer;

        helper->Allocate(framingBuffer->GetLength() * 4, (IByteBuffer**)&byteBuffer);
        AutoPtr<IInt32Buffer> intBuffer;

        AutoPtr<ArrayOf<Byte> > array;
        byteBuffer->GetArray((ArrayOf<Byte>**)&array);
        Int32 tmp = 0;
        while(tmp < mHeight) {
            byteBuffer->AsInt32Buffer((IInt32Buffer**)&intBuffer);
            intBuffer->PutInt32s(*framingBuffer, 0, mWidth);
            //try {
                dos->WriteBytes(*array);
            //} catch (IOException e) {
                /* catch file write error */
            //}
            tmp += 1;
        }

        //try {
            fl->Close();
        //} catch (IOException e) {
            /* file close error */
        //}
        mBlankFrameGenerated = TRUE;
    }

    clipSettings->clipPath = mBlankFrameFilename;
    clipSettings->fileType = IMediaArtistNativeHelperFileType::JPG;
    clipSettings->beginCutTime = 0;
    clipSettings->endCutTime = 0;
    clipSettings->mediaRendering = IMediaArtistNativeHelperMediaRendering::RESIZING;

    clipSettings->rgbWidth = 64;
    clipSettings->rgbHeight = 64;

    return NOERROR;
}

/**
 * Invalidates the blank frame generated
 */
ECode MediaItem::InvalidateBlankFrame()
{
    if (mBlankFrameFilename != NULL) {
        AutoPtr<IFile> file;
        CFile::New(mBlankFrameFilename, (IFile**)&file);
        Boolean b;
        file->Exists(&b);
        if (b) {
            file->Delete(&b);
            mBlankFrameFilename = NULL;
        }
    }
    return NOERROR;
}

} // namespace videoeditor
} // namespace Media
} // namepsace Droid
} // namespace Elastos
