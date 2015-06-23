
#include "ext/frameworkext.h"
#include "CMediaImageItem.h"
#include <elastos/core/Math.h>
#include "graphics/CRect.h"
#include "graphics/CCanvas.h"
#include "graphics/CPaint.h"
#include "graphics/CBitmapFactoryOptions.h"
#include "graphics/CBitmapFactory.h"
#include "CMediaPropertiesHelper.h"
#include "Transition.h"
#include "CEffectKenBurns.h"
#include "CVideoEditorImpl.h"

using Elastos::Core::Math;
using Elastos::Core::IInteger32;
using Elastos::IO::IFile;
using Elastos::IO::CFile;
using Elastos::IO::IFileOutputStream;
using Elastos::IO::CFileOutputStream;
using Elastos::IO::IDataOutputStream;
using Elastos::IO::CDataOutputStream;
using Elastos::IO::IByteBuffer;
using Elastos::IO::IByteBufferHelper;
using Elastos::IO::CByteBufferHelper;
using Elastos::IO::IInt32Buffer;
using Elastos::Droid::Utility::IPair;
using Elastos::Droid::Graphics::IBitmapFactoryOptions;
using Elastos::Droid::Graphics::CBitmapFactoryOptions;
using Elastos::Droid::Graphics::IBitmapFactory;
using Elastos::Droid::Graphics::CBitmapFactory;
using Elastos::Droid::Graphics::IRect;
using Elastos::Droid::Graphics::CRect;
using Elastos::Droid::Graphics::ICanvas;
using Elastos::Droid::Graphics::CCanvas;
using Elastos::Droid::Graphics::CPaint;

namespace Elastos {
namespace Droid {
namespace Media {
namespace Videoeditor {

const String CMediaImageItem::TAG("MediaImageItem");
AutoPtr<IPaint> CMediaImageItem::sResizePaint;

IMEDIAITEM_METHODS_IMPL(CMediaImageItem, MediaItem);

PInterface CMediaImageItem::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_MediaItem) {
        return reinterpret_cast<PInterface>((MediaItem*)this);
    }
    return _CMediaImageItem::Probe(riid);
}

CMediaImageItem::CMediaImageItem()
    : mWidth(0)
    , mHeight(0)
    , mAspectRatio(0)
    , mDurationMs(0)
    , mScaledWidth(0)
    , mScaledHeight(0)
    , mScaledFilename(NULL)
    , mDecodedFilename(NULL)
    , mGeneratedClipHeight(0)
    , mGeneratedClipWidth(0)
    , mFileName(NULL)
{
    CPaint::New(IPaint::FILTER_BITMAP_FLAG, (IPaint**)&sResizePaint);
}

ECode CMediaImageItem::constructor(
    /* [in] */ IVideoEditor* editor,
    /* [in] */ const String& mediaItemId,
    /* [in] */ const String& filename,
    /* [in] */ Int64 durationMs,
    /* [in] */ Int32 renderingMode)
{
    Init(editor, mediaItemId, filename, renderingMode);

    mVideoEditor = (IVideoEditorImpl*)IVideoEditorImpl::Probe(editor);
    ((CVideoEditorImpl*)mVideoEditor.Get())->GetNativeContext((MediaArtistNativeHelper**)&mMANativeHelper);
    //try {
        AutoPtr<MediaArtistNativeHelper::Properties> properties;
        properties = mMANativeHelper->GetMediaProperties(filename);

        Int32 type;
        mMANativeHelper->GetFileType(properties->fileType, &type);
        switch (type) {
            case IMediaProperties::FILE_JPEG:
            case IMediaProperties::FILE_PNG: {
                break;
            }

            default: {
            //    throw new IllegalArgumentException("Unsupported Input File Type");
            }
        }
    //} catch (Exception e) {
    //    throw new IllegalArgumentException("Unsupported file or file not found: " + filename);
    //}
    mFileName = filename;
    /**
     *  Determine the dimensions of the image
     */
    AutoPtr<IBitmapFactoryOptions> dbo;
    CBitmapFactoryOptions::New((IBitmapFactoryOptions**)&dbo);
    dbo->SetInJustDecodeBounds(TRUE);
    AutoPtr<IBitmapFactory> bitmapFactory;
    CBitmapFactory::AcquireSingleton((IBitmapFactory**)&bitmapFactory);
    AutoPtr<IBitmap> tmp;
    bitmapFactory->DecodeFile(filename, dbo, (IBitmap**)&tmp);

    dbo->GetOutWidth(&mWidth);
    dbo->GetOutHeight(&mHeight);
    mDurationMs = durationMs;
    String path, id;
    mMANativeHelper->GetProjectPath(&path);
    GetId(&id);
    mDecodedFilename = path + "/" + "decoded" + id + ".rgb";

    //try {
        mMANativeHelper->GetAspectRatio(mWidth, mHeight, &mAspectRatio);
    //} catch(IllegalArgumentException e) {
    //    throw new IllegalArgumentException ("NULL width and height");
    //}

    mGeneratedClipHeight = 0;
    mGeneratedClipWidth = 0;

    /**
     *  Images are stored in memory scaled to the maximum resolution to
     *  save memory.
     */
    AutoPtr<ArrayOf<IPair*> > resolutions;
    AutoPtr<IMediaPropertiesHelper> helper;
    CMediaPropertiesHelper::AcquireSingleton((IMediaPropertiesHelper**)&helper);
    helper->GetSupportedResolutions(mAspectRatio, (ArrayOf<IPair*>**)&resolutions);

    /**
     *  Get the highest resolution
     */
    AutoPtr<IPair> maxResolution = (*resolutions)[resolutions->GetLength() - 1];

    AutoPtr<IBitmap> imageBitmap;

    AutoPtr<IInteger32> iFirst;
    AutoPtr<IInteger32> iSecond;
    maxResolution->GetFirst((IInterface**)&iFirst);
    maxResolution->GetSecond((IInterface**)&iSecond);
    Int32 first, second;
    iFirst->GetValue(&first);
    iSecond->GetValue(&second);

    if (mWidth > first || mHeight > second) {
        /**
         *  We need to scale the image
         */
        imageBitmap = ScaleImage(filename, first, second);
        mScaledFilename = path + "/" + "scaled" + id + ".JPG";
        AutoPtr<IFile> file;
        CFile::New(mScaledFilename, (IFile**)&file);
        Boolean b;
        file->Exists(&b);
        if (!b) {
            mRegenerateClip = TRUE;
            AutoPtr<IFileOutputStream> steam;
            CFileOutputStream::New(mScaledFilename, (IFileOutputStream**)&steam);
            imageBitmap->Compress(Elastos::Droid::Graphics::BitmapCompressFormat_JPEG, 50, steam, &b);
            steam->Close();
        }
        Int32 width, height;
        imageBitmap->GetWidth(&width);
        imageBitmap->GetHeight(&height);
        mScaledWidth =  (width >> 1) << 1;
        mScaledHeight = (height >> 1) << 1;
    } else {
        mScaledFilename = filename;
        mScaledWidth =  (mWidth >> 1) << 1;
        mScaledHeight = (mHeight >> 1) << 1;
        bitmapFactory->DecodeFile(mScaledFilename, (IBitmap**)&imageBitmap);
    }
    Int32 newWidth = mScaledWidth;
    Int32 newHeight = mScaledHeight;
    AutoPtr<IFile> file;
    CFile::New(mDecodedFilename, (IFile**)&file);
    Boolean b;
    file->Exists(&b);
    if (!b) {
        AutoPtr<IFileOutputStream> fl;
        CFileOutputStream::New(mDecodedFilename, (IFileOutputStream**)&fl);
        AutoPtr<IDataOutputStream> dos;
        CDataOutputStream::New(fl, (IDataOutputStream**)&dos);
        AutoPtr<ArrayOf<Int32> > framingBuffer = ArrayOf<Int32>::Alloc(newWidth);
        AutoPtr<IByteBufferHelper> helper;
        CByteBufferHelper::AcquireSingleton((IByteBufferHelper**)&helper);
        AutoPtr<IByteBuffer> byteBuffer;
        helper->Allocate(framingBuffer->GetLength() * 4, (IByteBuffer**)&byteBuffer);
        AutoPtr<IInt32Buffer> intBuffer;
        AutoPtr<ArrayOf<Byte> > array;
        byteBuffer->GetArray((ArrayOf<Byte>**)&array);
        Int32 tmp = 0;
        while (tmp < newHeight) {
            imageBitmap->GetPixels(*framingBuffer, 0, mScaledWidth, 0, tmp, newWidth, 1);
            byteBuffer->AsInt32Buffer((IInt32Buffer**)&intBuffer);
            intBuffer->PutInt32s(*framingBuffer, 0, newWidth);
            dos->WriteBytes(*array);
            tmp += 1;
        }
        fl->Close();
    }
    imageBitmap->Recycle();
    return NOERROR;
}

ECode CMediaImageItem::GetFileType(
    /* [out] */ Int32* result)
{
    if (mFilename.EndWith(".jpg") || mFilename.EndWith(".jpeg")
            || mFilename.EndWith(".JPG") || mFilename.EndWith(".JPEG")) {
        *result = IMediaProperties::FILE_JPEG;
    } else if (mFilename.EndWith(".png") || mFilename.EndWith(".PNG")) {
        *result = IMediaProperties::FILE_PNG;
    } else {
        *result = IMediaProperties::FILE_UNSUPPORTED;
    }
    return NOERROR;
}

ECode CMediaImageItem::GetScaledImageFileName(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result)
    *result = mScaledFilename;
    return NOERROR;
}

ECode CMediaImageItem::GetGeneratedClipHeight(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)
    *result = mGeneratedClipHeight;
    return NOERROR;
}

ECode CMediaImageItem::GetGeneratedClipWidth(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)
    *result = mGeneratedClipWidth;
    return NOERROR;
}

ECode CMediaImageItem::GetDecodedImageFileName(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result)
    *result = mDecodedFilename;
    return NOERROR;
}

ECode CMediaImageItem::GetWidth(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mWidth;
    return NOERROR;
}

ECode CMediaImageItem::GetHeight(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mHeight;
    return NOERROR;
}

ECode CMediaImageItem::GetScaledWidth(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mScaledWidth;
    return NOERROR;
}

ECode CMediaImageItem::GetScaledHeight(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mScaledHeight;
    return NOERROR;
}

ECode CMediaImageItem::GetAspectRatio(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mAspectRatio;
    return NOERROR;
}

ECode CMediaImageItem::SetDuration(
    /* [in] */ Int64 durationMs)
{
   if (durationMs == mDurationMs) {
        return NOERROR;
    }

    mMANativeHelper->SetGeneratePreview(TRUE);

    /**
     * Invalidate the end transitions if necessary.
     * This invalidation is necessary for the case in which an effect or
     * an overlay is overlapping with the end transition
     * (before the duration is changed) and it no longer overlaps with the
     * transition after the duration is increased.
     *
     * The beginning transition does not need to be invalidated at this time
     * because an effect or an overlay overlaps with the beginning
     * transition, the begin transition is unaffected by a media item
     * duration change.
     */
    InvalidateEndTransition();

    mDurationMs = durationMs;

    AdjustTransitions();
    AutoPtr<ArrayOf<IOverlay*> > adjustedOverlays = AdjustOverlays();
    AutoPtr<ArrayOf<IEffect*> > adjustedEffects = AdjustEffects();

    /**
     * Invalidate the beginning and end transitions after adjustments.
     * This invalidation is necessary for the case in which an effect or
     * an overlay was not overlapping with the beginning or end transitions
     * before the setDuration reduces the duration of the media item and
     * causes an overlap of the beginning and/or end transition with the
     * effect.
     */
    InvalidateBeginTransition(adjustedEffects, adjustedOverlays);
    InvalidateEndTransition();
    String str;
    GetGeneratedImageClip(&str);
    if (str != NULL) {
        /*
         *  Delete the file
         */
        AutoPtr<IFile> file;
        CFile::New(str, (IFile**)&file);
        Boolean b;
        file->Delete(&b);
        /*
         *  Invalidate the filename
         */
        SetGeneratedImageClip(String(NULL));
        MediaItem::SetRegenerateClip(TRUE);
    }
    ((CVideoEditorImpl*)mVideoEditor.Get())->UpdateTimelineDuration();
    return NOERROR;
}

void CMediaImageItem::InvalidateBeginTransition(
    /* [in] */ ArrayOf<IEffect*>* effects,
    /* [in] */ ArrayOf<IOverlay*>* overlays)
{
    if (mBeginTransition != NULL) {
        Transition* transition = reinterpret_cast<Transition*>(mBeginTransition->Probe(EIID_Transition));
        Boolean b;
        transition->IsGenerated(&b);
        if(b) {
            Int64 transitionDurationMs;
            transition->GetDuration(&transitionDurationMs);

            /**
             *  The begin transition must be invalidated if it overlaps with
             *  an effect.
             */
            for(Int32 i; i < effects->GetLength(); i++) {
                /**
                 *  Check if the effect overlaps with the begin transition
                 */
                Int64 startTime;
                (*effects)[i]->GetStartTime(&startTime);
                if (startTime < transitionDurationMs) {
                    transition->Invalidate();
                    break;
                }
            }

            if (b) {
                /**
                 *  The end transition must be invalidated if it overlaps with
                 *  an overlay.
                 */
                for(Int32 i; i < overlays->GetLength(); i++) {
                    /**
                     *  Check if the overlay overlaps with the end transition
                     */
                    Int64 startTime;
                    (*overlays)[i]->GetStartTime(&startTime);
                    if (startTime < transitionDurationMs) {
                        transition->Invalidate();
                        break;
                    }
                }
            }
        }
    }
}

void CMediaImageItem::InvalidateEndTransition()
{
    if (mEndTransition != NULL) {
        Boolean b;
        Transition* transition = reinterpret_cast<Transition*>(mEndTransition->Probe(EIID_Transition));
        transition->IsGenerated(&b);
        if(b) {
            Int64 transitionDurationMs;
            transition->GetDuration(&transitionDurationMs);

            /**
             *  The end transition must be invalidated if it overlaps with
             *  an effect.
             */
            AutoPtr<ArrayOf<IEffect*> > effects;
            GetAllEffects((ArrayOf<IEffect*>**)&effects);
            for(Int32 i; i < effects->GetLength(); i++) {
                /**
                 *  Check if the effect overlaps with the end transition
                 */
                Int64 startTime, duration;
                (*effects)[i]->GetStartTime(&startTime);
                (*effects)[i]->GetDuration(&duration);
                if (startTime + duration > mDurationMs - transitionDurationMs) {
                    transition->Invalidate();
                    break;
                }
            }

            if (b) {
                /**
                 *  The end transition must be invalidated if it overlaps with
                 *  an overlay.
                 */
                AutoPtr<ArrayOf<IOverlay*> > overlays;
                GetAllOverlays((ArrayOf<IOverlay*>**)&overlays);
                for(Int32 i; i < overlays->GetLength(); i++) {
                    /**
                     *  Check if the overlay overlaps with the end transition
                     */
                    Int64 startTime, duration;
                    (*overlays)[i]->GetStartTime(&startTime);
                    (*overlays)[i]->GetDuration(&duration);
                    if (startTime + duration > mDurationMs - transitionDurationMs) {
                        transition->Invalidate();
                        break;
                    }
                }
            }
        }
    }
}

AutoPtr<ArrayOf<IEffect*> > CMediaImageItem::AdjustEffects()
{
    List<AutoPtr<IEffect> > adjustedEffectsList;
    AutoPtr<ArrayOf<IEffect*> > effects;
    GetAllEffects((ArrayOf<IEffect*>**)&effects);
    for(Int32 i; i < effects->GetLength(); i++) {
        /**
         *  Adjust the start time if necessary
         */
        Int64 startTime, duration;
        (*effects)[i]->GetStartTime(&startTime);
        GetDuration(&duration);
        Int64 effectStartTimeMs;
        if (startTime > duration) {
            effectStartTimeMs = 0;
        } else {
            effectStartTimeMs = startTime;
        }

        /**
         *  Adjust the duration if necessary
         */
        Int64 effectDurationMs;
        Int64 eDuration;
        (*effects)[i]->GetDuration(&eDuration);
        if (effectStartTimeMs + eDuration > duration) {
            effectDurationMs = duration - effectStartTimeMs;
        } else {
            effectDurationMs = eDuration;
        }

        if (effectStartTimeMs != startTime ||
                effectDurationMs != eDuration) {
            (*effects)[i]->SetStartTimeAndDuration(effectStartTimeMs, effectDurationMs);
            adjustedEffectsList.PushBack((*effects)[i]);
        }
    }

    AutoPtr<ArrayOf<IEffect*> > adjustedEffects = ArrayOf<IEffect*>::Alloc(adjustedEffectsList.GetSize());
    List<AutoPtr<IEffect> >::Iterator it = adjustedEffectsList.Begin();
    for(Int32 i = 0; it != adjustedEffectsList.End(); it++, i++) {
        adjustedEffects->Set(i, *it);
    }
    return adjustedEffects;
}

AutoPtr<ArrayOf<IOverlay*> > CMediaImageItem::AdjustOverlays()
{
    List<AutoPtr<IOverlay> > adjustedOverlaysList;
    AutoPtr<ArrayOf<IOverlay*> > overlays;
    GetAllOverlays((ArrayOf<IOverlay*>**)&overlays);
    for(Int32 i; i < overlays->GetLength(); i++) {
        /**
         *  Adjust the start time if necessary
         */
        Int64 startTime, duration;
        (*overlays)[i]->GetStartTime(&startTime);
        GetDuration(&duration);
        Int64 overlayStartTimeMs;
        if (startTime > duration) {
            overlayStartTimeMs = 0;
        } else {
            overlayStartTimeMs = startTime;
        }

        /**
         *  Adjust the duration if necessary
         */
        Int64 OverlayDurationMs;
        Int64 oDuration;
        (*overlays)[i]->GetDuration(&oDuration);
        if (overlayStartTimeMs + oDuration > duration) {
            OverlayDurationMs = duration - overlayStartTimeMs;
        } else {
            OverlayDurationMs = oDuration;
        }

        if (overlayStartTimeMs != startTime ||
                OverlayDurationMs != oDuration) {
            (*overlays)[i]->SetStartTimeAndDuration(overlayStartTimeMs, OverlayDurationMs);
            adjustedOverlaysList.PushBack((*overlays)[i]);
        }
    }

    AutoPtr<ArrayOf<IOverlay*> > adjustedOverlays = ArrayOf<IOverlay*>::Alloc(adjustedOverlaysList.GetSize());
    List<AutoPtr<IOverlay> >::Iterator it = adjustedOverlaysList.Begin();
    for(Int32 i = 0; it != adjustedOverlaysList.End(); it++, i++) {
        adjustedOverlays->Set(i, *it);
    }
    return adjustedOverlays;
}

Int32 CMediaImageItem::GetWidthByAspectRatioAndHeight(
    /* [in] */ Int32 aspectRatio,
    /* [in] */ Int32 height)
{
    Int32 width = 0;

    switch (aspectRatio) {
        case IMediaProperties::ASPECT_RATIO_3_2:
            if (height == IMediaProperties::HEIGHT_480)
                width = 720;
            else if (height == IMediaProperties::HEIGHT_720)
                width = 1080;
            break;

        case IMediaProperties::ASPECT_RATIO_16_9:
            if (height == IMediaProperties::HEIGHT_360)
                width = 640;
            else if (height == IMediaProperties::HEIGHT_480)
                width = 854;
            else if (height == IMediaProperties::HEIGHT_720)
                width = 1280;
            else if (height == IMediaProperties::HEIGHT_1080)
                width = 1920;
            break;

        case IMediaProperties::ASPECT_RATIO_4_3:
            if (height == IMediaProperties::HEIGHT_480)
                width = 640;
            if (height == IMediaProperties::HEIGHT_720)
                width = 960;
            break;

        case IMediaProperties::ASPECT_RATIO_5_3:
            if (height == IMediaProperties::HEIGHT_480)
                width = 800;
            break;

        case IMediaProperties::ASPECT_RATIO_11_9:
            if (height == IMediaProperties::HEIGHT_144)
                width = 176;
            break;

        default : {
        //    throw new IllegalArgumentException("Illegal arguments for aspectRatio");
        }
    }

    return width;
}

ECode CMediaImageItem::SetGeneratedImageClip(
    /* [in] */ const String& generatedFilePath)
{
    MediaItem::SetGeneratedImageClip(generatedFilePath);

    // set the Kenburns clip width and height
    GetScaledHeight(&mGeneratedClipHeight);
    Int32 ratio;
    mVideoEditor->GetAspectRatio(&ratio);
    mGeneratedClipWidth = GetWidthByAspectRatioAndHeight(ratio, mGeneratedClipHeight);
}

ECode CMediaImageItem::GetGeneratedImageClip(
    /* [out] */ String* result)
{
    return MediaItem::GetGeneratedImageClip(result);
}

ECode CMediaImageItem::GetDuration(
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mDurationMs;
    return NOERROR;
}

ECode CMediaImageItem::GetTimelineDuration(
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mDurationMs;
    return NOERROR;
}

ECode CMediaImageItem::GetThumbnail(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int64 timeMs,
    /* [out] */ IBitmap** result)
{
    VALIDATE_NOT_NULL(result);

    AutoPtr<IBitmap> temp;
    String str;
    GetGeneratedImageClip(&str);
    if (str != NULL) {
        mMANativeHelper->GetPixels(str, width, height, timeMs, 0, (IBitmap**)&temp);
    }
    else {
        temp = ScaleImage(mFilename, width, height);
    }

    *result = temp;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CMediaImageItem::GetThumbnailList(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int64 startMs,
    /* [in] */ Int64 endMs,
    /* [in] */ Int32 thumbnailCount,
    /* [in] */ ArrayOf<Int32>* indices,
    /* [in] */ IMediaItemGetThumbnailListCallback* callback)
{
    String str;
    GetGeneratedImageClip(&str);
    if(str != NULL) {
        AutoPtr<IBitmap> thumbnail = ScaleImage(mFilename, width, height);
        for (Int32 i = 0; i < indices->GetLength(); i++) {
            callback->OnThumbnail(thumbnail, (*indices)[i]);
        }
    }
    else {
        if (startMs > endMs) {
        //    throw new IllegalArgumentException("Start time is greater than end time");
        }

        if (endMs > mDurationMs) {
        //    throw new IllegalArgumentException("End time is greater than file duration");
        }

        mMANativeHelper->GetPixelsList(str, width, height, startMs, endMs, thumbnailCount, indices, callback, 0);
    }

    return NOERROR;
}

ECode CMediaImageItem::InvalidateTransitions(
    /* [in] */ Int64 startTimeMs,
    /* [in] */ Int64 durationMs)
{
    /**
     *  Check if the item overlaps with the beginning and end transitions
     */
    if (mBeginTransition != NULL) {
        Int64 bDuration;
        mBeginTransition->GetDuration(&bDuration);
        if (IsOverlapping(startTimeMs, durationMs, 0, bDuration)) {
            (reinterpret_cast<Transition*>(mBeginTransition->Probe(EIID_Transition)))->Invalidate();
        }
    }

    if (mEndTransition != NULL) {
        Int64 transitionDurationMs, duration;
        mEndTransition->GetDuration(&transitionDurationMs);
        GetDuration(&duration);
        if (IsOverlapping(startTimeMs, durationMs,
                duration - transitionDurationMs, transitionDurationMs)) {
            (reinterpret_cast<Transition*>(mEndTransition->Probe(EIID_Transition)))->Invalidate();
        }
    }
    return NOERROR;
}

ECode CMediaImageItem::InvalidateTransitions(
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
        Boolean oldOverlap = IsOverlapping(oldStartTimeMs, oldDurationMs, 0,
                transitionDurationMs);
        Boolean newOverlap = IsOverlapping(newStartTimeMs, newDurationMs, 0,
                transitionDurationMs);
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
        } else if (newOverlap) { // Both old and new overlap
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
                mDurationMs - transitionDurationMs, transitionDurationMs);
        Boolean newOverlap = IsOverlapping(newStartTimeMs, newDurationMs,
                mDurationMs - transitionDurationMs, transitionDurationMs);
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
        } else if (newOverlap) { // Both old and new overlap
            if ((oldStartTimeMs + oldDurationMs != newStartTimeMs + newDurationMs) ||
                    ((oldStartTimeMs > mDurationMs - transitionDurationMs) ||
                    newStartTimeMs > mDurationMs - transitionDurationMs)) {
                (reinterpret_cast<Transition*>(mEndTransition->Probe(EIID_Transition)))->Invalidate();
            }
        }
    }
    return NOERROR;
}

ECode CMediaImageItem::Invalidate()
{
    String str;
    GetGeneratedImageClip(&str);
    if (str != NULL) {
        AutoPtr<IFile> file;
        CFile::New(str, (IFile**)&file);
        Boolean b;
        file->Delete(&b);
        SetGeneratedImageClip(String(NULL));
        SetRegenerateClip(TRUE);
    }

    if (mScaledFilename != NULL) {
        if(mFileName != mScaledFilename) {
            AutoPtr<IFile> file;
            CFile::New(mScaledFilename, (IFile**)&file);
            Boolean b;
            file->Delete(&b);
        }
        mScaledFilename = NULL;
    }

    if (mDecodedFilename != NULL) {
        AutoPtr<IFile> file;
        CFile::New(mDecodedFilename, (IFile**)&file);
        Boolean b;
        file->Delete(&b);
        mDecodedFilename = NULL;
    }
    return NOERROR;
}

AutoPtr<MediaArtistNativeHelper::ClipSettings> CMediaImageItem::GetKenBurns(
    /* [in] */ IEffectKenBurns* effectKB)
{
    Int32 PanZoomXa;
    Int32 PanZoomXb;
    Int32 width = 0, height = 0;
    AutoPtr<IRect> start;
    CRect::New((IRect**)&start);
    AutoPtr<IRect> end;
    CRect::New((IRect**)&end);
    AutoPtr<MediaArtistNativeHelper::ClipSettings> clipSettings = new MediaArtistNativeHelper::ClipSettings();
    /**
     *  image:
    ---------------------------------------
   |    Xa                                  |
   | Ya ---------------                     |
   |    |                |                  |
   |    |                |                  |
   |     ---------------    Xb       ratioB |
   |        ratioA           -------        |
   |                  Yb    |        |      |
   |                        |        |      |
   |                         -------        |
    ---------------------------------------
     */

    ((CEffectKenBurns*)effectKB)->GetKenBurnsSettings(start, end);
    Int32 sLeft, sRight, sTop, sBottom;
    Int32 eLeft, eRight, eTop, eBottom;
    start->Get(&sLeft, &sRight, &sTop, &sBottom);
    end->Get(&eLeft, &eRight, &eTop, &eBottom);
    GetWidth(&width);
    GetHeight(&height);
    if ((sLeft < 0) || (sLeft > width) || (sRight < 0) || (sRight > width)
            || (sTop < 0) || (sTop > height) || (sBottom < 0)
            || (sBottom > height) || (eLeft < 0) || (eLeft > width)
            || (eRight < 0) || (eRight > width) || (eTop < 0) || (eTop > height)
            || (eBottom < 0) || (eBottom > height)) {
        // throw new IllegalArgumentException("Illegal arguments for KebBurns");
        return NULL;
    }

    Int32 mode;
    GetRenderingMode(&mode);
    Int32 duration;
    GetTimelineDuration((Int64*)&duration);
    if (((width - (sRight - sLeft) == 0) || (height - (sBottom - sTop) == 0))
            && ((width - (eRight - eLeft) == 0) || (height - (eBottom - eTop) == 0))) {
        SetRegenerateClip(FALSE);
        GetDecodedImageFileName(&clipSettings->clipPath);
        clipSettings->fileType = IMediaArtistNativeHelperFileType::JPG;
        clipSettings->beginCutTime = 0;
        clipSettings->endCutTime = duration;
        clipSettings->beginCutPercent = 0;
        clipSettings->endCutPercent = 0;
        clipSettings->panZoomEnabled = FALSE;
        clipSettings->panZoomPercentStart = 0;
        clipSettings->panZoomTopLeftXStart = 0;
        clipSettings->panZoomTopLeftYStart = 0;
        clipSettings->panZoomPercentEnd = 0;
        clipSettings->panZoomTopLeftXEnd = 0;
        clipSettings->panZoomTopLeftYEnd = 0;
        mMANativeHelper->GetMediaItemRenderingMode(mode, &clipSettings->mediaRendering);

        GetScaledWidth(&clipSettings->rgbWidth);
        GetScaledHeight(&clipSettings->rgbHeight);

        return clipSettings;
    }

    Int32 sWidth, eWidth;
    start->GetWidth(&sWidth);
    end->GetWidth(&eWidth);
    PanZoomXa = (1000 * sWidth) / width;
    PanZoomXb = (1000 * eWidth) / width;

    GetDecodedImageFileName(&clipSettings->clipPath);
    Int32 type;
    GetFileType(&type);
    mMANativeHelper->GetMediaItemFileType(type, &clipSettings->fileType);
    clipSettings->beginCutTime = 0;
    clipSettings->endCutTime = duration;
    clipSettings->beginCutPercent = 0;
    clipSettings->endCutPercent = 0;
    clipSettings->panZoomEnabled = TRUE;
    clipSettings->panZoomPercentStart = PanZoomXa;
    clipSettings->panZoomTopLeftXStart = (sLeft * 1000) / width;
    clipSettings->panZoomTopLeftYStart = (sTop * 1000) / height;
    clipSettings->panZoomPercentEnd = PanZoomXb;
    clipSettings->panZoomTopLeftXEnd = (eLeft * 1000) / width;
    clipSettings->panZoomTopLeftYEnd = (eTop * 1000) / height;
    mMANativeHelper->GetMediaItemRenderingMode(mode, &clipSettings->mediaRendering);

    GetScaledWidth(&clipSettings->rgbWidth);
    GetScaledHeight(&clipSettings->rgbHeight);

    return clipSettings;
}

ECode CMediaImageItem::GenerateKenburnsClip(
    /* [in] */ IEffectKenBurns* effectKB,
    /* [out] */ MediaArtistNativeHelper::ClipSettings** settings)
{
    AutoPtr<MediaArtistNativeHelper::EditSettings> editSettings = new MediaArtistNativeHelper::EditSettings();
    editSettings->clipSettingsArray = ArrayOf<MediaArtistNativeHelper::ClipSettings*>::Alloc(1);
    String output;
    AutoPtr<MediaArtistNativeHelper::ClipSettings> clipSettings = new MediaArtistNativeHelper::ClipSettings();
    InitClipSettings(clipSettings);
    editSettings->clipSettingsArray->Set(0, GetKenBurns(effectKB));
    String str;
    GetGeneratedImageClip(&str);
    Boolean b;
    GetRegenerateClip(&b);
    if ((str == NULL) && (b)) {
        mMANativeHelper->GenerateKenBurnsClip(editSettings, this, &output);
        SetGeneratedImageClip(output);
        SetRegenerateClip(FALSE);
        clipSettings->clipPath = output;
        clipSettings->fileType = IMediaArtistNativeHelperFileType::THREE_GPP;

        GetScaledHeight(&mGeneratedClipHeight);
        Int32 ratio;
        mVideoEditor->GetAspectRatio(&ratio);
        mGeneratedClipWidth = GetWidthByAspectRatioAndHeight(ratio, mGeneratedClipHeight);
    } else {
        if (str == NULL) {
            GetDecodedImageFileName(&clipSettings->clipPath);
            clipSettings->fileType = IMediaArtistNativeHelperFileType::JPG;

            GetScaledWidth(&clipSettings->rgbWidth);
            GetScaledHeight(&clipSettings->rgbHeight);

        } else {
            clipSettings->clipPath = str;
            clipSettings->fileType = IMediaArtistNativeHelperFileType::THREE_GPP;
        }
    }
    Int32 mode;
    GetRenderingMode(&mode);
    mMANativeHelper->GetMediaItemRenderingMode(mode, &clipSettings->mediaRendering);
    clipSettings->beginCutTime = 0;
    Int32 duration;
    GetTimelineDuration((Int64*)&duration);
    clipSettings->endCutTime = duration;

    *settings = clipSettings;
    REFCOUNT_ADD(*settings);
    return NOERROR;
}

ECode CMediaImageItem::GetImageClipProperties(
    /* [out] */ MediaArtistNativeHelper::ClipSettings** settings)
{
    AutoPtr<MediaArtistNativeHelper::ClipSettings> clipSettings = new MediaArtistNativeHelper::ClipSettings();
    AutoPtr<IEffectKenBurns> effectKB;
    Boolean effectKBPresent = FALSE;

    AutoPtr<ArrayOf<IEffect*> > effects;
    GetAllEffects((ArrayOf<IEffect*>**)&effects);
    for(Int32 i = 0; i < effects->GetLength(); i++) {
        AutoPtr<IEffectKenBurns> ekb = (IEffectKenBurns*)IEffectKenBurns::Probe((*effects)[i]);
        if(ekb != NULL) {
            effectKBPresent = TRUE;
            break;
        }
    }

    if (effectKBPresent) {
        GenerateKenburnsClip(effectKB, (MediaArtistNativeHelper::ClipSettings**)&clipSettings);
    } else {
        /**
         * Init the clip settings object
         */
        InitClipSettings(clipSettings);
        GetDecodedImageFileName(&clipSettings->clipPath);
        clipSettings->fileType = IMediaArtistNativeHelperFileType::JPG;
        clipSettings->beginCutTime = 0;
        Int32 duration;
        GetTimelineDuration((Int64*)&duration);
        clipSettings->endCutTime = duration;
        Int32 mode;
        GetRenderingMode(&mode);
        mMANativeHelper->GetMediaItemRenderingMode(mode, &clipSettings->mediaRendering);
        GetScaledWidth(&clipSettings->rgbWidth);
        GetScaledHeight(&clipSettings->rgbHeight);
    }

    *settings = clipSettings;
    REFCOUNT_ADD(*settings);
    return NOERROR;
}

AutoPtr<IBitmap> CMediaImageItem::ScaleImage(
    /* [in] */ const String& filename,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    AutoPtr<IBitmapFactoryOptions> dbo;
    CBitmapFactoryOptions::New((IBitmapFactoryOptions**)&dbo);
    dbo->SetInJustDecodeBounds(TRUE);
    AutoPtr<IBitmapFactory> bitmapFactory;
    CBitmapFactory::AcquireSingleton((IBitmapFactory**)&bitmapFactory);
    AutoPtr<IBitmap> tmp;
    bitmapFactory->DecodeFile(filename, dbo, (IBitmap**)&tmp);

    Int32 nativeWidth;
    dbo->GetOutWidth(&nativeWidth);
    Int32 nativeHeight;
    dbo->GetOutHeight(&nativeHeight);
    // if (Log.isLoggable(TAG, Log.DEBUG)) {
    //     Log.d(TAG, "generateThumbnail: Input: " + nativeWidth + "x" + nativeHeight + ", resize to: " + width + "x" + height);
    // }

    AutoPtr<IBitmap> srcBitmap;
    Float bitmapWidth, bitmapHeight;
    if (nativeWidth > width || nativeHeight > height) {
        Float dx = ((Float)nativeWidth) / ((Float)width);
        Float dy = ((Float)nativeHeight) / ((Float)height);

        if (dx > dy) {
            bitmapWidth = width;

            if (((Float)nativeHeight / dx) < (Float)height) {
                bitmapHeight = (Float)Elastos::Core::Math::Ceil(nativeHeight / dx);
            } else { // value equals the requested height
                bitmapHeight = (Float)Elastos::Core::Math::Floor(nativeHeight / dx);
            }

        } else {
            if (((Float)nativeWidth / dy) > (Float)width) {
                bitmapWidth = (Float)Elastos::Core::Math::Floor(nativeWidth / dy);
            } else { // value equals the requested width
                bitmapWidth = (Float)Elastos::Core::Math::Ceil(nativeWidth / dy);
            }

            bitmapHeight = height;
        }

        /**
         *  Create the bitmap from file
         */
        Int32 sampleSize = (Int32) Elastos::Core::Math::Ceil(Elastos::Core::Math::Max( (Float) nativeWidth / bitmapWidth,(Float) nativeHeight / bitmapHeight));
        NextPowerOf2(sampleSize, &sampleSize);
        AutoPtr<IBitmapFactoryOptions> options;
        CBitmapFactoryOptions::New((IBitmapFactoryOptions**)&options);
        options->SetInSampleSize(sampleSize);
        bitmapFactory->DecodeFile(filename, options, (IBitmap**)&srcBitmap);
    }
    else {
        bitmapWidth = width;
        bitmapHeight = height;
        bitmapFactory->DecodeFile(filename, (IBitmap**)&srcBitmap);

    }

    if (srcBitmap == NULL) {
        // Log.e(TAG, "generateThumbnail: Cannot decode image bytes");
        // throw new IOException("Cannot decode file: " + mFilename);
        return NULL;
    }

    /**
     *  Create the canvas bitmap
     */
    AutoPtr<IBitmapFactory> factory;
    ASSERT_SUCCEEDED(CBitmapFactory::AcquireSingleton((IBitmapFactory**)&factory));
    AutoPtr<IBitmap> bitmap;
    factory->CreateBitmap((Int32)bitmapWidth, (Int32)bitmapHeight,
            Elastos::Droid::Graphics::BitmapConfig_ARGB_8888, (IBitmap**)&bitmap);

    AutoPtr<ICanvas> canvas;
    CCanvas::New(bitmap, (ICanvas**)&canvas);
    Int32 wid, hei;
    srcBitmap->GetWidth(&wid);
    srcBitmap->GetHeight(&hei);
    AutoPtr<IRect> ra;
    CRect::New(0, 0, wid, hei, (IRect**)&ra);
    AutoPtr<IRect> rb;
    CRect::New(0, 0, (Int32)bitmapWidth, (Int32)bitmapHeight, (IRect**)&rb);
    canvas->DrawBitmap(srcBitmap, ra, rb, sResizePaint);
    canvas->SetBitmap(NULL);
    /**
     *  Release the source bitmap
     */
    srcBitmap->Recycle();
    return bitmap;
}

ECode CMediaImageItem::NextPowerOf2(
    /* [in] */ Int32 n,
    /* [out] */ Int32* result)
{
    n -= 1;
    n |= ((unsigned)n) >> 16;
    n |= ((unsigned)n) >> 8;
    n |= ((unsigned)n) >> 4;
    n |= ((unsigned)n) >> 2;
    n |= ((unsigned)n) >> 1;
    *result = n + 1;
    return NOERROR;
}

ECode CMediaImageItem::AddEffect(                                                                      \
    /* [in] */ IEffect* effect)                                                                  \
{                                                                                                \
    return MediaItem::AddEffect(effect);                                                        \
}                                                                                                \


} // namespace Videoeditor
} // namespace Media
} // namepsace Droid
} // namespace Elastos
