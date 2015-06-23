
#include "ext/frameworkext.h"
#include "COverlayFrame.h"
#include "graphics/CRect.h"
#include "graphics/CBitmapFactory.h"
#include "graphics/CCanvas.h"
#include "CMediaPropertiesHelper.h"
#include "MediaArtistNativeHelper.h"
#include "MediaItem.h"

using Elastos::Core::IInteger32;
using Elastos::IO::IFile;
using Elastos::IO::CFile;
using Elastos::IO::IFlushable;
using Elastos::IO::IInt32Buffer;
using Elastos::IO::IByteBuffer;
using Elastos::IO::IByteBufferHelper;
using Elastos::IO::CByteBufferHelper;
using Elastos::IO::IFileOutputStream;
using Elastos::IO::CFileOutputStream;
using Elastos::IO::IDataOutputStream;
using Elastos::IO::CDataOutputStream;
using Elastos::Droid::Utility::IPair;
using Elastos::Droid::Graphics::IBitmapFactory;
using Elastos::Droid::Graphics::CBitmapFactory;
using Elastos::Droid::Graphics::ICanvas;
using Elastos::Droid::Graphics::CCanvas;
using Elastos::Droid::Graphics::IRect;
using Elastos::Droid::Graphics::CRect;

namespace Elastos {
namespace Droid {
namespace Media {
namespace Videoeditor {

AutoPtr<IPaint> COverlayFrame::sResizePaint;

IOVERLAY_METHODS_IMPL(COverlayFrame, Overlay);

PInterface COverlayFrame::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_Overlay) {
        return reinterpret_cast<PInterface>((Overlay*)this);
    }
    return _COverlayFrame::Probe(riid);
}

COverlayFrame::COverlayFrame()
    : mFilename(NULL)
    , mBitmapFileName(NULL)
    , mOFWidth(0)
    , mOFHeight(0)
    , mResizedRGBWidth(0)
    , mResizedRGBHeight(0)
{
}

ECode COverlayFrame::constructor(
    /* [in] */ IMediaItem* mediaItem,
    /* [in] */ const String& overlayId,
    /* [in] */ IBitmap* bitmap,
    /* [in] */ Int64 startTimeMs,
    /* [in] */ Int64 durationMs)
{
    Init(mediaItem, overlayId, startTimeMs, durationMs);
    mBitmap = bitmap;
    return NOERROR;
}

ECode COverlayFrame::constructor(
    /* [in] */ IMediaItem* mediaItem,
    /* [in] */ const String& overlayId,
    /* [in] */ const String& filename,
    /* [in] */ Int64 startTimeMs,
    /* [in] */ Int64 durationMs)
{
    Init(mediaItem, overlayId, startTimeMs, durationMs);
    mBitmapFileName = filename;
    AutoPtr<IBitmapFactory> bitmapFactory;
    CBitmapFactory::AcquireSingleton((IBitmapFactory**)&bitmapFactory);
    AutoPtr<IBitmap> tmp;
    return bitmapFactory->DecodeFile(filename, (IBitmap**)&mBitmap);
}

ECode COverlayFrame::GetBitmap(
    /* [out] */ IBitmap** result)
{
    VALIDATE_NOT_NULL(result);

    *result = mBitmap;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode COverlayFrame::GetBitmapImageFileName(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mBitmapFileName;
    return NOERROR;
}

ECode COverlayFrame::SetBitmap(
        /* [in] */ IBitmap* bitmap)
{
    AutoPtr<IMediaItem> mediaItem;
    GetMediaItem((IMediaItem**)&mediaItem);
    AutoPtr<MediaArtistNativeHelper> helper;
    ((MediaItem*)mediaItem.Get())->GetNativeContext((MediaArtistNativeHelper**)&helper);
    helper->SetGeneratePreview(TRUE);

    Invalidate();

    mBitmap = bitmap;
    if (mFilename != NULL) {
        /**
         *  Delete the file
         */
        AutoPtr<IFile> file;
        CFile::New(mFilename, (IFile**)&file);
        Boolean b;
        file->Delete(&b);
        /**
         *  Invalidate the filename
         */
        mFilename = NULL;
    }

    /**
     *  Invalidate the transitions if necessary
     */
    return ((MediaItem*)mediaItem.Get())->InvalidateTransitions(mStartTimeMs, mDurationMs);
}

ECode COverlayFrame::GetFilename(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mFilename;
    return NOERROR;
}

ECode COverlayFrame::SetFilename(
    /* [in] */ const String& filename)
{
    mFilename = filename;
    return NOERROR;
}

ECode COverlayFrame::Save(
    /* [in] */ const String& path,
    /* [out] */ String* result)
{
    if (mFilename != NULL) {
        *result = mFilename;
        return NOERROR;
    }

    // Create the compressed PNG file
    String id;
    GetId(&id);
    mBitmapFileName = path + "/" + "Overlay" + id + ".png";
    AutoPtr<IFile> file;
    CFile::New(mBitmapFileName, (IFile**)&file);
    Boolean b;
    file->Exists(&b);
    if (!b) {
        AutoPtr<IFileOutputStream> out;
        CFileOutputStream::New(mBitmapFileName, (IFileOutputStream**)&out);
        mBitmap->Compress(Elastos::Droid::Graphics::BitmapCompressFormat_JPEG, 100, out, &b);
        IFlushable::Probe(out)->Flush();
        out->Close();
    }

    mBitmap->GetWidth(&mOFWidth);
    mBitmap->GetHeight(&mOFHeight);

    mFilename = path + "/" + "Overlay" + id + ".rgb";

    /* resize and save rgb as per project aspect ratio */
    AutoPtr<MediaArtistNativeHelper> nativeHelper;
    AutoPtr<IMediaItem> mediaItem;
    Overlay::GetMediaItem((IMediaItem**)&mediaItem);
    ((MediaItem*)mediaItem.Get())->GetNativeContext((MediaArtistNativeHelper**)&nativeHelper);

    /* get height and width for story board aspect ratio */
    AutoPtr<IPair> maxResolution;
    AutoPtr<ArrayOf<IPair*> > resolutions;
    AutoPtr<IMediaPropertiesHelper> helper;
    CMediaPropertiesHelper::AcquireSingleton((IMediaPropertiesHelper**)&helper);
    Int32 ratio;
    nativeHelper->NativeHelperGetAspectRatio(&ratio);
    helper->GetSupportedResolutions(ratio, (ArrayOf<IPair*>**)&resolutions);

    // Get the highest resolution
    maxResolution = (*resolutions)[resolutions->GetLength() - 1];

    /* Generate the rgb file with rendering mode */
    AutoPtr<IMediaItem> mi;
    Overlay::GetMediaItem((IMediaItem**)&mi);
    AutoPtr<IInteger32> iFirst;
    AutoPtr<IInteger32> iSecond;
    maxResolution->GetFirst((IInterface**)&iFirst);
    maxResolution->GetSecond((IInterface**)&iSecond);
    Int32 first, second;
    iFirst->GetValue(&first);
    iSecond->GetValue(&second);
    GenerateOverlayWithRenderingMode(mi, this,
            second /* max Height */ ,
            first /* max Width */);

    *result = mFilename;
    return NOERROR;
}

ECode COverlayFrame::GetOverlayFrameHeight(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mOFHeight;
    return NOERROR;
}

ECode COverlayFrame::GetOverlayFrameWidth(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mOFWidth;
    return NOERROR;
}

ECode COverlayFrame::SetOverlayFrameHeight(
    /* [in] */ Int32 height)
{
    mOFHeight = height;
    return NOERROR;
}

ECode COverlayFrame::SetOverlayFrameWidth(
    /* [in] */ Int32 width)
{
    mOFWidth = width;
    return NOERROR;
}

ECode COverlayFrame::SetResizedRGBSize(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    mResizedRGBWidth = width;
    mResizedRGBHeight = height;
    return NOERROR;
}

ECode COverlayFrame::GetResizedRGBSizeHeight(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mResizedRGBHeight;
    return NOERROR;
}

ECode COverlayFrame::GetResizedRGBSizeWidth(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mResizedRGBWidth;
    return NOERROR;
}

ECode COverlayFrame::Invalidate()
{
    if (mBitmap != NULL) {
        mBitmap->Recycle();
        mBitmap = NULL;
    }

    if (mFilename != NULL) {
        AutoPtr<IFile> file;
        CFile::New(mFilename, (IFile**)&file);
        Boolean b;
        file->Delete(&b);
        mFilename = NULL;
    }

    if (mBitmapFileName != NULL) {
        AutoPtr<IFile> file;
        CFile::New(mBitmapFileName, (IFile**)&file);
        Boolean b;
        file->Delete(&b);
        mBitmapFileName = NULL;
    }

    return NOERROR;
}

ECode COverlayFrame::InvalidateGeneratedFiles()
{
    if (mFilename != NULL) {
        AutoPtr<IFile> file;
        CFile::New(mFilename, (IFile**)&file);
        Boolean b;
        file->Delete(&b);
        mFilename = NULL;
    }

    if (mBitmapFileName != NULL) {
        AutoPtr<IFile> file;
        CFile::New(mBitmapFileName, (IFile**)&file);
        Boolean b;
        file->Delete(&b);
        mBitmapFileName = NULL;
    }

    return NOERROR;
}

ECode COverlayFrame::GenerateOverlayWithRenderingMode(
    /* [in] */ IMediaItem* mediaItemsList,
    /* [in] */ IOverlayFrame* overlay,
    /* [in] */ Int32 height,
    /* [in] */ Int32 width)
{
    AutoPtr<IMediaItem> t = mediaItemsList;

    /* get the rendering mode */
    Int32 renderMode;
    t->GetRenderingMode(&renderMode);

    AutoPtr<IBitmap> overlayBitmap;
    overlay->GetBitmap((IBitmap**)&overlayBitmap);

    /*
     * Check if the resize of Overlay is needed with rendering mode applied
     * because of change in export dimensions
     */
    Int32 resizedRGBFileHeight;
    ((COverlayFrame*)overlay)->GetResizedRGBSizeHeight(&resizedRGBFileHeight);
    Int32 resizedRGBFileWidth;
    ((COverlayFrame*)overlay)->GetResizedRGBSizeWidth(&resizedRGBFileWidth);

    /* Get original bitmap width if it is not resized */
    if(resizedRGBFileWidth == 0) {
        overlayBitmap->GetWidth(&resizedRGBFileWidth);
    }
    /* Get original bitmap height if it is not resized */
    if(resizedRGBFileHeight == 0) {
        overlayBitmap->GetHeight(&resizedRGBFileHeight);
    }

    String name;
    ((COverlayFrame*)overlay)->GetFilename(&name);
    AutoPtr<IFile> file;
    CFile::New(name, (IFile**)&file);
    Boolean b;
    file->Exists(&b);

    if (resizedRGBFileWidth != width || resizedRGBFileHeight != height || (!b)) {
        /*
         *  Create the canvas bitmap
         */
        AutoPtr<IBitmapFactory> bitmapFactory;
        CBitmapFactory::AcquireSingleton((IBitmapFactory**)&bitmapFactory);
        AutoPtr<IBitmap> destBitmap;
        bitmapFactory->CreateBitmap(width, height, Elastos::Droid::Graphics::BitmapConfig_ARGB_8888, (IBitmap**)&destBitmap);

        AutoPtr<ICanvas> overlayCanvas;
        CCanvas::New(destBitmap, (ICanvas**)&overlayCanvas);
        AutoPtr<IRect> destRect;
        AutoPtr<IRect> srcRect;

        Int32 canvasWidth, canvasHeight;
        overlayCanvas->GetWidth(&canvasWidth);
        overlayCanvas->GetHeight(&canvasHeight);
        Int32 bitmapWidth, bitmapHeight;
        overlayBitmap->GetWidth(&bitmapWidth);
        overlayBitmap->GetHeight(&bitmapHeight);
        switch (renderMode) {
            case IMediaItem::RENDERING_MODE_STRETCH: {
                CRect::New(0, 0, canvasWidth, canvasHeight, (IRect**)&destRect);
                CRect::New(0, 0, bitmapWidth, bitmapHeight, (IRect**)&srcRect);
                break;
            }

            case IMediaItem::RENDERING_MODE_BLACK_BORDER: {
                Int32 left, right, top, bottom;
                Float aROverlayImage, aRCanvas;
                aROverlayImage = (Float)bitmapWidth /
                                 (Float)bitmapHeight;

                aRCanvas = (Float)canvasWidth /
                                 (Float)canvasHeight;

                if (aROverlayImage > aRCanvas) {
                    Int32 newHeight = ((canvasWidth * bitmapHeight)
                                     / bitmapWidth);
                    left = 0;
                    top  = (canvasHeight - newHeight) / 2;
                    right = canvasWidth;
                    bottom = top + newHeight;
                }
                else {
                    Int32 newWidth = ((canvasHeight * bitmapWidth)
                                        / bitmapHeight);
                    left = (canvasWidth - newWidth) / 2;
                    top  = 0;
                    right = left + newWidth;
                    bottom = canvasHeight;
                }

                CRect::New(left, top, right, bottom, (IRect**)&destRect);
                CRect::New(0, 0, bitmapWidth, bitmapHeight, (IRect**)&srcRect);
                break;
            }

            case IMediaItem::RENDERING_MODE_CROPPING: {
                // Calculate the source rect
                Int32 left, right, top, bottom;
                Float aROverlayImage, aRCanvas;
                aROverlayImage = (Float)(bitmapWidth) /
                                 (Float)(bitmapHeight);
                aRCanvas = (Float)(canvasWidth) /
                                (Float)(canvasHeight);
                if (aROverlayImage < aRCanvas) {
                    Int32 newHeight = ((bitmapWidth * canvasHeight)
                               / canvasWidth);

                    left = 0;
                    top  = (bitmapHeight - newHeight) / 2;
                    right = bitmapWidth;
                    bottom = top + newHeight;
                }
                else {
                    Int32 newWidth = ((bitmapHeight * canvasWidth)
                                / canvasHeight);
                    left = (bitmapWidth - newWidth) / 2;
                    top  = 0;
                    right = left + newWidth;
                    bottom = bitmapHeight;
                }

                CRect::New(left, top, right, bottom, (IRect**)&srcRect);
                CRect::New(0, 0, canvasWidth, canvasHeight, (IRect**)&destRect);
                break;
            }

            default: {
                // throw new IllegalStateException("Rendering mode: " + renderMode);
                return E_ILLEGAL_STATE_EXCEPTION;
            }
        }

        overlayCanvas->DrawBitmap(overlayBitmap, srcRect, destRect, sResizePaint);
        overlayCanvas->SetBitmap(NULL);

        /*
         * Write to the dest file
         */
        String outFileName;
        ((COverlayFrame*)overlay)->GetFilename(&outFileName);

        /*
         * Save the image to same rgb file
         */
        if (outFileName != NULL) {
            AutoPtr<IFile> file;
            CFile::New(outFileName, (IFile**)&file);
            Boolean b;
            file->Delete(&b);
        }

        AutoPtr<IFileOutputStream> fl;
        CFileOutputStream::New(outFileName, (IFileOutputStream**)&fl);
        AutoPtr<IDataOutputStream> dos;
        CDataOutputStream::New(fl, (IDataOutputStream**)&dos);

        /*
         * Populate the rgb file with bitmap data
         */
        AutoPtr<ArrayOf<Int32> > framingBuffer = ArrayOf<Int32>::Alloc(width);
        AutoPtr<IByteBuffer> byteBuffer;
        AutoPtr<IByteBufferHelper> helper;
        CByteBufferHelper::AcquireSingleton((IByteBufferHelper**)&helper);
        helper->Allocate(framingBuffer->GetLength() * 4, (IByteBuffer**)&byteBuffer);
        AutoPtr<IInt32Buffer> intBuffer;

        AutoPtr<ArrayOf<Byte> > array;
        byteBuffer->GetArray((ArrayOf<Byte>**)&array);
        Int32 tmp = 0;
        while(tmp < height) {
            destBitmap->GetPixels(*framingBuffer, 0, width, 0, tmp, width, 1);
            byteBuffer->AsInt32Buffer((IInt32Buffer**)&intBuffer);
            intBuffer->PutInt32s(*framingBuffer, 0, width);
            dos->WriteBytes(*array);
            tmp += 1;
        }
        IFlushable::Probe(fl)->Flush();
        fl->Close();

        /*
         * Set the resized RGB width and height
         */
        ((COverlayFrame*)overlay)->SetResizedRGBSize(width, height);
    }
    return NOERROR;
}

} // namespace Videoeditor
} // namespace Media
} // namepsace Droid
} // namespace Elastos
