
#include "ext/frameworkext.h"
#include "graphics/BitmapRegionDecoder.h"
#include "graphics/CRect.h"
#include "graphics/AutoDecoderCancel.h"
#include "graphics/Utils.h"
#include "graphics/CreateOutputStreamAdaptor.h"
#include "graphics/GraphicsNative.h"
#include "graphics/NBitmapFactory.h"
#include <skia/core/SkTemplates.h>
#include <skia/core/SkTScopedPtr.h>
#include <skia/images/SkBitmapRegionDecoder.h>
#include <skia/images/SkImageDecoder.h>
#include <androidfw/Asset.h>
#include <sys/stat.h>
#include <unistd.h>

using Elastos::IO::IBufferedInputStream;
using Elastos::IO::CBufferedInputStream;
using Elastos::IO::IFileInputStream;
using Elastos::IO::CFileInputStream;

using Elastos::Droid::Content::Res::IAssetInputStream;

namespace Elastos {
namespace Droid {
namespace Graphics {

CAR_INTERFACE_IMPL(BitmapRegionDecoder, Object, BitmapRegionDecoder);
/*  Private constructor that must receive an already allocated native
region decoder int (pointer).
*/
BitmapRegionDecoder::BitmapRegionDecoder(
    /* [in] */ Int32 decoder)
    : mNativeBitmapRegionDecoder(decoder)
    , mRecycled(FALSE)
{}

BitmapRegionDecoder::~BitmapRegionDecoder()
{
    // try {
    Recycle();
    // } finally {
    //     super.finalize();
    // }
}

ECode BitmapRegionDecoder::NewInstance(
    /* [in] */ const ArrayOf<Byte>& data,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [in] */ Boolean isShareable,
    /* [out] */ IBitmapRegionDecoder** decoder)
{
    VALIDATE_NOT_NULL(decoder);

    if ((offset | length) < 0 || data.GetLength() < offset + length) {
        // throw new ArrayIndexOutOfBoundsException();
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    return NativeNewInstance(data, offset, length, isShareable, decoder);
}

ECode BitmapRegionDecoder::NewInstance(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Boolean isShareable,
    /* [out] */ IBitmapRegionDecoder** decoder)
{
    VALIDATE_NOT_NULL(decoder);

    return NativeNewInstance(fd, isShareable, decoder);
}

ECode BitmapRegionDecoder::NewInstance(
    /* [in] */ IInputStream* _is,
    /* [in] */ Boolean isShareable,
    /* [out] */ IBitmapRegionDecoder** decoder)
{
    VALIDATE_NOT_NULL(decoder);

    AutoPtr<IInputStream> is = _is;
    Boolean support;
    if (is->IsMarkSupported(&support), !support) {
        is = NULL;
        CBufferedInputStream::New(_is, 16 * 1024, (IBufferedInputStream**)&is);
    }

    if (IAssetInputStream::Probe(is.Get()) != NULL) {
        Int32 value;
        IAssetInputStream::Probe(is.Get())->GetAssetInt32(&value);
        return NativeNewInstance(value, isShareable, decoder);
    }
    else {
        // pass some temp storage down to the native code. 1024 is made up,
        // but should be large enough to avoid too many small calls back
        // into is.read(...).
        AutoPtr< ArrayOf<Byte> > tempStorage = ArrayOf<Byte>::Alloc(16 * 1024);
        return NativeNewInstance(is, tempStorage, isShareable, decoder);
    }
}

ECode BitmapRegionDecoder::NewInstance(
    /* [in] */ const String& pathName,
    /* [in] */ Boolean isShareable,
    /* [out] */ IBitmapRegionDecoder** decoder)
{
    VALIDATE_NOT_NULL(decoder);

    AutoPtr<IInputStream> stream;

    // try {
    ECode ec = CFileInputStream::New(pathName, (IFileInputStream**)&stream);
    if (SUCCEEDED(ec)) ec = NewInstance(stream, isShareable, decoder);
    // } finally {
    //     if (stream != null) {
    //         try {
    //             stream.close();
    //         } catch (IOException e) {
    //             // do nothing here
    //         }
    //     }
    // }

    if (stream != NULL) {
        stream->Close();
    }
    return ec;
}

/**
* Decodes a rectangle region in the image specified by rect.
*
* @param rect The rectangle that specified the region to be decode.
* @param options null-ok; Options that control downsampling.
*             inPurgeable is not supported.
* @param ppBitmap The decoded bitmap, or null if the image data could not be
*         decoded.
*/
ECode BitmapRegionDecoder::DecodeRegion(
    /* [in] */ IRect* rect,
    /* [in] */ IBitmapFactoryOptions* options,
    /* [out] */ IBitmap** bitmap)
{
    VALIDATE_NOT_NULL(bitmap);

    AutoLock lock(mNativeLock);

    FAIL_RETURN(CheckRecycled("decodeRegion called on recycled region decoder"));
    Int32 width, height;
    GetWidth(&width);
    GetHeight(&height);
    CRect* rectObj = (CRect*)rect;
    if (rectObj->mRight <= 0 || rectObj->mBottom <= 0 || rectObj->mLeft >= width
            || rectObj->mTop >= height) {
        // throw new IllegalArgumentException("rectangle is outside the image");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NativeDecodeRegion(mNativeBitmapRegionDecoder, rectObj->mLeft, rectObj->mTop,
            rectObj->mRight - rectObj->mLeft, rectObj->mBottom - rectObj->mTop, options, bitmap);
}

/** @param pWidth the original image's width */
ECode BitmapRegionDecoder::GetWidth(
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);

    AutoLock lock(mNativeLock);

    FAIL_RETURN(CheckRecycled("getWidth called on recycled region decoder"));
    *width = NativeGetWidth(mNativeBitmapRegionDecoder);
    return NOERROR;
}

/** @param pHeight the original image's height */
ECode BitmapRegionDecoder::GetHeight(
    /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height);

    AutoLock lock(mNativeLock);

    FAIL_RETURN(CheckRecycled("getHeight called on recycled region decoder"));
    *height = NativeGetHeight(mNativeBitmapRegionDecoder);
    return NOERROR;
}

/**
* Frees up the memory associated with this region decoder, and mark the
* region decoder as "dead", meaning it will throw an exception if decodeRegion(),
* getWidth() or getHeight() is called.
*
* <p>This operation cannot be reversed, so it should only be called if you are
* sure there are no further uses for the region decoder. This is an advanced call,
* and normally need not be called, since the normal GC process will free up this
* memory when there are no more references to this region decoder.
*/
ECode BitmapRegionDecoder::Recycle()
{
    AutoLock lock(mNativeLock);

    if (!mRecycled) {
        NativeClean(mNativeBitmapRegionDecoder);
        mRecycled = TRUE;
    }
    return NOERROR;
}

/**
* @param pResult true if this region decoder has been recycled.
* If so, then it is an error to try use its method.
*
* @return true if the region decoder has been recycled
*/
ECode BitmapRegionDecoder::IsRecycled(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mRecycled;
    return NOERROR;
}

/**
* Called by methods that want to throw an exception if the region decoder
* has already been recycled.
*/
ECode BitmapRegionDecoder::CheckRecycled(
    /* [in] */ CString errorMessage)
{
    if (mRecycled) {
        // throw new IllegalStateException(errorMessage);
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    return NOERROR;
}

static SkMemoryStream* BuildSkMemoryStream(
    /* [in] */ SkStream *stream)
{
    size_t bufferSize = 4096;
    size_t streamLen = 0;
    size_t len;
    char* data = (char*)sk_malloc_throw(bufferSize);

    while ((len = stream->read(data + streamLen,
                    bufferSize - streamLen)) != 0) {
        streamLen += len;
        if (streamLen == bufferSize) {
            bufferSize *= 2;
            data = (char*)sk_realloc_throw(data, bufferSize);
        }
    }
    data = (char*)sk_realloc_throw(data, streamLen);

    SkMemoryStream* streamMem = new SkMemoryStream();
    streamMem->setMemoryOwned(data, streamLen);
    return streamMem;
}

static ECode DoBuildTileIndex(
    /* [in] */ SkStream* stream,
    /* [out] */ IBitmapRegionDecoder** _decoder)
{
    assert(_decoder != NULL);

    SkImageDecoder* decoder = SkImageDecoder::Factory(stream);
    int width, height;
    if (NULL == decoder) {
        *_decoder = NULL;
        return E_IO_EXCEPTION;
    }

    GraphicsNative::DroidPixelAllocator* cppAllocator = new GraphicsNative::DroidPixelAllocator();
    decoder->setAllocator(cppAllocator);
    cppAllocator->unref();

    if (!decoder->buildTileIndex(stream, &width, &height)) {
        // char msg[100];
        // snprintf(msg, sizeof(msg), "Image failed to decode using %s decoder",
        //         decoder->getFormatName());
        // doThrowIOE(env, msg);
        // return nullObjectReturn("decoder->buildTileIndex returned false");
        *_decoder = NULL;
        return E_IO_EXCEPTION;
    }

    SkBitmapRegionDecoder *bm = new SkBitmapRegionDecoder(decoder, stream, width, height);

    return GraphicsNative::CreateBitmapRegionDecoder(bm, _decoder);
}

static Boolean OptionsCancel(IBitmapFactoryOptions* options)
{
    if (options == NULL) return FALSE;

    Boolean value;
    options->GetCancel(&value);
    return value;
}

ECode BitmapRegionDecoder::NativeDecodeRegion(
    /* [in] */ Int32 brd,
    /* [in] */ Int32 startX,
    /* [in] */ Int32 startY,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ IBitmapFactoryOptions* options,
    /* [out] */ IBitmap** bitmap)
{
    AutoPtr<IBitmap> tileBitmap;
    SkImageDecoder* decoder = ((SkBitmapRegionDecoder*)brd)->getDecoder();
    Int32 sampleSize = 1;
    SkBitmap::Config prefConfig = SkBitmap::kNo_Config;
    Boolean doDither = TRUE;
    Boolean preferQualityOverSpeed = FALSE;

    if (NULL != options) {
        options->GetInSampleSize(&sampleSize);
        // initialize these, in case we fail later on
        options->SetOutWidth(-1);
        options->SetOutHeight(-1);
        options->SetOutMimeType(String(NULL));

        BitmapConfig config;
        options->GetInPreferredConfig(&config);
        prefConfig = GraphicsNative::GetNativeBitmapConfig(config);
        options->GetInDither(&doDither);
        options->GetInPreferQualityOverSpeed(&preferQualityOverSpeed);
        // Get the bitmap for re-use if it exists.
        options->GetInBitmap((IBitmap**)&tileBitmap);
    }

    decoder->setDitherImage(doDither);
    decoder->setPreferQualityOverSpeed(preferQualityOverSpeed);
    AutoDecoderCancel adc(options, decoder);

    // To fix the race condition in case "requestCancelDecode"
    // happens earlier than AutoDecoderCancel object is added
    // to the gAutoDecoderCancelMutex linked list.
    if (OptionsCancel(options)) {
        *bitmap = NULL;
        return NOERROR;
    }

    SkIRect region;
    region.fLeft = startX;
    region.fTop = startY;
    region.fRight = startX + width;
    region.fBottom = startY + height;
    SkBitmap* nativeBitmap = NULL;
    SkTScopedPtr<SkBitmap> adb;

    if (tileBitmap != NULL) {
        // Re-use bitmap.
        nativeBitmap = (SkBitmap*)((CBitmap*)tileBitmap.Get())->mNativeBitmap;
    }
    if (nativeBitmap == NULL) {
        nativeBitmap = new SkBitmap;
        adb.reset(nativeBitmap);
    }

    if (!((SkBitmapRegionDecoder*)brd)->decodeRegion(nativeBitmap, region, prefConfig, sampleSize)) {
        *bitmap = NULL;
        return NOERROR;
    }

    // update options (if any)
    if (NULL != options) {
        options->SetOutWidth(nativeBitmap->width());
        options->SetOutHeight(nativeBitmap->height());
        // TODO: set the mimeType field with the data from the codec.
        // but how to reuse a set of strings, rather than allocating new one
        // each time?
        options->SetOutMimeType(NBitmapFactory::GetMimeTypeString(decoder->getFormat()));
    }

   if (tileBitmap != NULL) {
        *bitmap = tileBitmap;
        REFCOUNT_ADD(*bitmap);
        return NOERROR;
    }

    // detach bitmap from its autotdeleter, since we want to own it now
    adb.release();

    GraphicsNative::DroidPixelAllocator* allocator = (GraphicsNative::DroidPixelAllocator*)decoder->getAllocator();
    AutoPtr< ArrayOf<Byte> > buff = allocator->getStorageObjAndReset();
    AutoPtr<CBitmap> bitmapObj;
    FAIL_RETURN(GraphicsNative::CreateBitmap(nativeBitmap, buff, FALSE, NULL, NULL, -1, (CBitmap**)&bitmapObj));
    *bitmap = (IBitmap*)bitmapObj.Get();
    REFCOUNT_ADD(*bitmap);
    return NOERROR;
}

Int32 BitmapRegionDecoder::NativeGetWidth(
    /* [in] */ Int32 brd)
{
    return ((SkBitmapRegionDecoder*)brd)->getWidth();
}

Int32 BitmapRegionDecoder::NativeGetHeight(
    /* [in] */ Int32 brd)
{
    return ((SkBitmapRegionDecoder*)brd)->getHeight();
}

void BitmapRegionDecoder::NativeClean(
    /* [in] */ Int32 brd)
{
    delete (SkBitmapRegionDecoder*)brd;
}

ECode BitmapRegionDecoder::NativeNewInstance(
    /* [in] */ const ArrayOf<Byte>& data,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [in] */ Boolean isShareable,
    /* [out] */ IBitmapRegionDecoder** decoder)
{
    /*  If isShareable we could decide to just wrap the java array and
    share it, but that means adding a globalref to the java array object
    For now we just always copy the array's data if isShareable.
    */
    SkStream* stream = new SkMemoryStream(data.GetPayload() + offset, length, TRUE);
    return DoBuildTileIndex(stream, decoder);
}

ECode BitmapRegionDecoder::NativeNewInstance(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ Boolean isShareable,
    /* [out] */ IBitmapRegionDecoder** decoder)
{
    if (fd == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }

    Int32 descriptor;
    fd->GetDescriptor(&descriptor);
    SkStream *stream = NULL;
    struct stat fdStat;
    Int32 newFD;
    if (fstat(descriptor, &fdStat) == -1) {
        *decoder = NULL;
        return E_IO_EXCEPTION;
    }

    if (isShareable &&
            S_ISREG(fdStat.st_mode) &&
            (newFD = ::dup(descriptor)) != -1) {
        SkFDStream* fdStream = new SkFDStream(newFD, true);
        if (!fdStream->isValid()) {
            fdStream->unref();
            *decoder = NULL;
            return NOERROR;
        }
        stream = fdStream;
    }
    else {
        /* Restore our offset when we leave, so we can be called more than once
           with the same descriptor. This is only required if we didn't dup the
           file descriptor, but it is OK to do it all the time.
        */
        AutoFDSeek as(descriptor);

        SkFDStream* fdStream = new SkFDStream(descriptor, false);
        if (!fdStream->isValid()) {
            fdStream->unref();
            *decoder = NULL;
            return NOERROR;
        }
        stream = BuildSkMemoryStream(fdStream);
        fdStream->unref();
    }

    return DoBuildTileIndex(stream, decoder);
}

ECode BitmapRegionDecoder::NativeNewInstance(
    /* [in] */ IInputStream* is,
    /* [in] */ ArrayOf<Byte>* storage,
    /* [in] */ Boolean isShareable,
    /* [out] */ IBitmapRegionDecoder** decoder)
{
    ECode ec = NOERROR;
    *decoder = NULL;
    SkStream* stream = CreateInputStreamAdaptor(is, storage, 1024);

    if (stream) {
        // for now we don't allow shareable with java inputstreams
        SkMemoryStream *mStream = BuildSkMemoryStream(stream);
        ec = DoBuildTileIndex(mStream, decoder);
        stream->unref();
    }
    return ec;
}

ECode BitmapRegionDecoder::NativeNewInstance(
    /* [in] */ Int32 nativeAsset,
    /* [in] */ Boolean isShareable,
    /* [out] */ IBitmapRegionDecoder** decoder)
{
    SkStream* stream, *assStream;
    android::Asset* asset = reinterpret_cast<android::Asset*>(nativeAsset);
    assStream = new AssetStreamAdaptor(asset);
    stream = BuildSkMemoryStream(assStream);
    assStream->unref();
    return DoBuildTileIndex(stream, decoder);
}

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos
