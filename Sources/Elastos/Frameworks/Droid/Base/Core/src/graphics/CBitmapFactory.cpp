
#include "ext/frameworkext.h"
#include "graphics/CBitmapFactory.h"
#include "graphics/CBitmap.h"
#include "graphics/CBitmapFactoryOptions.h"
#include "graphics/NinePatch.h"
#include "graphics/CreateOutputStreamAdaptor.h"
#include "graphics/GraphicsNative.h"
#include "graphics/Utils.h"
#include "graphics/AutoDecoderCancel.h"
#include "graphics/NinePatchPeeker.h"
#include "graphics/NBitmapFactory.h"
#include "util/CTypedValue.h"
#include <elastos/utility/logging/Logger.h>
#include <skia/core/SkBitmap.h>
#include <skia/core/SkTemplates.h>
#include <skia/core/SkCanvas.h>
#include <skia/core/SkPixelRef.h>
#include <skia/core/SkStream.h>
#include <skia/images/SkImageDecoder.h>
#include <skia/images/SkImageRef_GlobalPool.h>
#include <skia/ports/SkImageRef_ashmem.h>
#include <androidfw/Asset.h>
#include <androidfw/ResourceTypes.h>

using Elastos::IO::IFileInputStream;
using Elastos::IO::CFileInputStream;
using Elastos::IO::IBufferedInputStream;
using Elastos::IO::CBufferedInputStream;
using Elastos::Utility::Logging::Logger;

using Elastos::Droid::Content::Res::IAssetInputStream;
using Elastos::Droid::Utility::CTypedValue;

namespace Elastos {
namespace Droid {
namespace Graphics {

////////////////////////////////////////////////////////////////////////////////
// CBitmapFactory

static const String TAG = String("CBitmapFactory");
const Int32 CBitmapFactory::DECODE_BUFFER_SIZE = 16 * 1024;

CAR_SINGLETON_IMPL(CBitmapFactory);
CAR_INTERFACE_IMPL(CBitmapFactory, Singleton, IBitmapFactory);
ECode CBitmapFactory::SetDefaultDensity(
    /* [in] */ Int32 density)
{
    CBitmap::SetDefaultDensity(density);
    return NOERROR;
}

ECode CBitmapFactory::CreateScaledBitmap(
    /* [in] */ IBitmap* src,
    /* [in] */ Int32 dstWidth,
    /* [in] */ Int32 dstHeight,
    /* [in] */ Boolean filter,
    /* [out] */ IBitmap** bitmap)
{
    return CBitmap::CreateScaledBitmap(src, dstWidth, dstHeight, filter, bitmap);
}

ECode CBitmapFactory::CreateBitmap(
    /* [in] */ IBitmap* source,
    /* [out] */ IBitmap** bitmap)
{
    return CBitmap::CreateBitmap(source, bitmap);
}

ECode CBitmapFactory::CreateBitmap(
    /* [in] */ IBitmap* source,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [out] */ IBitmap** bitmap)
{
    return CBitmap::CreateBitmap(source, x, y, width, height, bitmap);
}

ECode CBitmapFactory::CreateBitmap(
    /* [in] */ IBitmap* source,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ IMatrix* m,
    /* [in] */ Boolean filter,
    /* [out] */ IBitmap** bitmap)
{
    return CBitmap::CreateBitmap(source, x, y, width, height, m, filter, bitmap);
}

ECode CBitmapFactory::CreateBitmap(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ BitmapConfig config,
    /* [out] */ IBitmap** bitmap)
{
    return CBitmap::CreateBitmap(width, height, config, bitmap);
}

ECode CBitmapFactory::CreateBitmap(
    /* [in] */ const ArrayOf<Int32>& colors,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 stride,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ BitmapConfig config,
    /* [out] */ IBitmap** bitmap)
{
    return CBitmap::CreateBitmap(colors, offset, stride, width, height, config, bitmap);
}

ECode CBitmapFactory::CreateBitmap(
    /* [in] */ const ArrayOf<Int32>& colors,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ BitmapConfig config,
    /* [out] */ IBitmap** bitmap)
{
    return CBitmap::CreateBitmap(colors, width, height, config, bitmap);
}

ECode CBitmapFactory::CreateBitmap(
    /* [in] */ IDisplayMetrics* display,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ BitmapConfig config,
    /* [out] */ IBitmap** bitmap)
{
    return CBitmap::CreateBitmap(display, width, height, config, bitmap);
}

ECode CBitmapFactory::CreateBitmap(
    /* [in] */ IDisplayMetrics* display,
    /* [in] */ const ArrayOf<Int32>& colors,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 stride,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ BitmapConfig config,
    /* [out] */ IBitmap** bitmap)
{
    return CBitmap::CreateBitmap(display, colors, offset, stride, width, height, config, bitmap);
}

ECode CBitmapFactory::CreateBitmap(
    /* [in] */ IDisplayMetrics* display,
    /* [in] */ const ArrayOf<Int32>& colors,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ BitmapConfig config,
    /* [out] */ IBitmap** bitmap)
{
    return CBitmap::CreateBitmap(display, colors, width, height, config, bitmap);
}

ECode CBitmapFactory::ScaleFromDensity(
    /* [in] */ Int32 size,
    /* [in] */ Int32 sdensity,
    /* [in] */ Int32 tdensity,
    /* [out] */ Int32* retSize)
{
    VALIDATE_NOT_NULL(retSize);

    *retSize = CBitmap::ScaleFromDensity(size, sdensity, tdensity);
    return NOERROR;
}

ECode CBitmapFactory::DecodeFile(
    /* [in] */ const String& pathName,
    /* [in] */ IBitmapFactoryOptions* opts,
    /* [out] */ IBitmap** bitmap)
{
    VALIDATE_NOT_NULL(bitmap);
    *bitmap = NULL;

    AutoPtr<IFileInputStream> fstream;
    ECode ec = CFileInputStream::New(pathName, (IFileInputStream**)&fstream);
    if (FAILED(ec)) {
        Logger::E(TAG, "BitmapFactory, Unable to decode stream");
        return ec;
    }

    ec = DecodeStream(IInputStream::Probe(fstream), NULL, opts, bitmap);
    if (FAILED(ec)) {
        Logger::E(TAG, "BitmapFactory, Unable to decode stream");
    }

    fstream->Close();
    return ec;
}

ECode CBitmapFactory::DecodeFile(
    /* [in] */ const String& pathName,
    /* [out] */ IBitmap** bitmap)
{
    VALIDATE_NOT_NULL(bitmap);
    return DecodeFile(pathName, NULL, bitmap);
}

ECode CBitmapFactory::DecodeResourceStream(
    /* [in] */ IResources* res,
    /* [in] */ ITypedValue* value,
    /* [in] */ IInputStream* is,
    /* [in] */ IRect* pad,
    /* [in] */ IBitmapFactoryOptions* _opts,
    /* [out] */ IBitmap** bitmap)
{
    VALIDATE_NOT_NULL(bitmap);

    AutoPtr<IBitmapFactoryOptions> opts = _opts;
    if (opts == NULL) {
        CBitmapFactoryOptions::New((IBitmapFactoryOptions**)&opts);
    }

    Int32 density;
    if ((opts->GetInDensity(&density), density == 0) && value != NULL) {
        Int32 density;
        value->GetDensity(&density);
        if (density == ITypedValue::DENSITY_DEFAULT) {
            opts->SetInDensity(IDisplayMetrics::DENSITY_DEFAULT);
        }
        else if (density != ITypedValue::DENSITY_NONE) {
            opts->SetInDensity(density);
        }
    }

    if ((opts->GetInTargetDensity(&density), density == 0) && res != NULL) {
        AutoPtr<IDisplayMetrics> dm;
        res->GetDisplayMetrics((IDisplayMetrics**)&dm);
        dm->GetDensityDpi(&density);
        opts->SetInTargetDensity(density);
    }

    return DecodeStream(is, pad, opts, bitmap);
}

ECode CBitmapFactory::DecodeResource(
    /* [in] */ IResources* res,
    /* [in] */ Int32 id,
    /* [in] */ IBitmapFactoryOptions* opts,
    /* [out] */ IBitmap** bitmap)
{
    AutoPtr<IInputStream> is;

    AutoPtr<ITypedValue> value;
    FAIL_RETURN(CTypedValue::New((ITypedValue**)&value));
    FAIL_RETURN(res->OpenRawResource(id, value.Get(), (IInputStream**)&is));

    /*  do nothing.
        If the exception happened on open, bm will be NULL.
        If it happened on close, bm is still valid.
    */
    DecodeResourceStream(res, value, is, NULL, opts, bitmap);
    if (is != NULL) is->Close();

    AutoPtr<IBitmap> bm;
    if (*bitmap == NULL && opts != NULL && (opts->GetInBitmap((IBitmap**)&bm), bm != NULL)) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    return NOERROR;
}

ECode CBitmapFactory::DecodeResource(
    /* [in] */ IResources* res,
    /* [in] */ Int32 id,
    /* [out] */ IBitmap** bitmap)
{
    return DecodeResource(res, id, NULL, bitmap);
}

ECode CBitmapFactory::DecodeByteArray(
    /* [in] */ const ArrayOf<Byte>& data,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [in] */ IBitmapFactoryOptions* opts,
    /* [out] */ IBitmap** bitmap)
{
    if ((offset | length) < 0 || data.GetLength() < offset + length) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    AutoPtr<IBitmap> bm = NativeDecodeByteArray(data, offset, length, opts);

    AutoPtr<IBitmap> inbm;
    if (bm == NULL && opts != NULL && (opts->GetInBitmap((IBitmap**)&inbm), inbm != NULL)) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    *bitmap = bm;
    REFCOUNT_ADD(*bitmap);
    return NOERROR;
}

ECode CBitmapFactory::DecodeByteArray(
    /* [in] */ const ArrayOf<Byte>& data,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ IBitmap** bitmap)
{
    return DecodeByteArray(data, offset, length, NULL, bitmap);
}

ECode CBitmapFactory::DecodeStream(
    /* [in] */ IInputStream* _is,
    /* [in] */ IRect* outPadding,
    /* [in] */ IBitmapFactoryOptions* opts,
    /* [out] */ IBitmap** bitmap)
{
    VALIDATE_NOT_NULL(bitmap);
    *bitmap = NULL;

    // we don't throw in this case, thus allowing the caller to only check
    // the cache, and not force the image to be decoded.
    if (_is == NULL) {
        return NOERROR;
    }

    // we need mark/reset to work properly
    AutoPtr<IInputStream> is = _is;
    Boolean supported = FALSE;
    is->IsMarkSupported(&supported);
    if (!supported) {
        is = NULL;
        FAIL_RETURN(CBufferedInputStream::New(_is, DECODE_BUFFER_SIZE, (IBufferedInputStream**)&is));
    }

    // so we can call reset() if a given codec gives up after reading up to
    // this many bytes. FIXME: need to find out from the codecs what this
    // value should be.
    is->Mark(1024);

    AutoPtr<IBitmap> bm;
    Boolean finish = TRUE;

    AutoPtr<IAssetInputStream> assertIs = IAssetInputStream::Probe(is);
    if (assertIs != NULL) {
        Int32 asset;
        assertIs->GetAssetInt32(&asset);

        Boolean bval = FALSE;
        if (opts != NULL) {
            opts->GetInScaled(&bval);
            if (bval) {
                AutoPtr<IBitmap> inbm;
                opts->GetInBitmap((IBitmap**)&inbm);
                bval = (inbm == NULL);
            }
        }
        if (opts == NULL || bval) {
            Float scale = 1.0f;
            Int32 targetDensity = 0;
            if (opts != NULL) {
                Int32 density;
                opts->GetInDensity(&density);
                opts->GetInTargetDensity(&targetDensity);
                if (density != 0 && targetDensity != 0) {
                    scale = targetDensity / (Float) density;
                }
            }

            bm = NativeDecodeAsset(asset, outPadding, opts, TRUE, scale);
            if (bm != NULL && targetDensity != 0) bm->SetDensity(targetDensity);

            finish = FALSE;
        }
        else {
            bm = NativeDecodeAsset(asset, outPadding, opts);
        }
    }
    else {
        // pass some temp storage down to the native code. 1024 is made up,
        // but should be large enough to avoid too many small calls back
        // into is.read(...) This number is not related to the value passed
        // to mark(...) above.
        AutoPtr< ArrayOf<Byte> > tempStorage;
        if (opts != NULL) opts->GetInTempStorage((ArrayOf<Byte>**)&tempStorage);
        if (tempStorage == NULL) tempStorage = ArrayOf<Byte>::Alloc(16 * 1024);

        Boolean bval = FALSE;
        if (opts != NULL) {
            opts->GetInScaled(&bval);
            if (bval) {
                AutoPtr<IBitmap> inbm;
                opts->GetInBitmap((IBitmap**)&inbm);
                bval = (inbm == NULL);
            }
        }
        if (opts == NULL || bval) {
            Float scale = 1.0f;
            Int32 targetDensity = 0;
            if (opts != NULL) {
                Int32 density;
                opts->GetInDensity(&density);
                opts->GetInTargetDensity(&targetDensity);
                if (density != 0 && targetDensity != 0) {
                    scale = targetDensity / (Float) density;
                }
            }

            bm = NativeDecodeStream(is, tempStorage, outPadding, opts, TRUE, scale);
            if (bm != NULL && targetDensity != 0) bm->SetDensity(targetDensity);

            finish = FALSE;
        }
        else {
            bm = NativeDecodeStream(is, tempStorage, outPadding, opts);
        }
    }

    AutoPtr<IBitmap> inbm;
    if (bm == NULL && opts != NULL && (opts->GetInBitmap((IBitmap**)&inbm), inbm != NULL)) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (finish) {
        AutoPtr<IBitmap> fbm = FinishDecode(bm, outPadding, opts);
        *bitmap = fbm;
    }
    else {
        *bitmap = bm;
    }

    REFCOUNT_ADD(*bitmap);
    return NOERROR;
}

AutoPtr<IBitmap> CBitmapFactory::FinishDecode(
    /* [in] */ IBitmap* _bm,
    /* [in] */ IRect* outPadding,
    /* [in] */ IBitmapFactoryOptions* opts)
{
    AutoPtr<IBitmap> bm = _bm;
    if (bm == NULL || opts == NULL) {
        return bm;
    }

    Int32 density;
    opts->GetInDensity(&density);
    if (density == 0) {
        return bm;
    }

    bm->SetDensity(density);
    Int32 targetDensity, screenDensity;
    opts->GetInTargetDensity(&targetDensity);
    opts->GetInScreenDensity(&screenDensity);
    if (targetDensity == 0 || density == targetDensity || density == screenDensity) {
        return bm;
    }
    AutoPtr< ArrayOf<Byte> > np;
    bm->GetNinePatchChunk((ArrayOf<Byte>**)&np);
    AutoPtr< ArrayOf<Int32> > lb;
    bm->GetLayoutBounds((ArrayOf<Int32>**)&lb);
    const Boolean isNinePatch = np != NULL && NinePatch::IsNinePatchChunk(np);
    Boolean inScaled;
    opts->GetInScaled(&inScaled);
    if (inScaled || isNinePatch) {
        Float scale = targetDensity / (Float)density;
        if (scale != 1.0f) {
            AutoPtr<IBitmap> oldBitmap = bm;
            Int32 width = 0, height = 0;
            bm->GetWidth(&width);
            bm->GetHeight(&height);
            bm = NULL;
            CBitmap::CreateScaledBitmap(oldBitmap,
                (Int32)(width * scale + 0.5f),
                (Int32)(height * scale + 0.5f), TRUE, (IBitmap**)&bm);
            if (bm != oldBitmap){
                oldBitmap->Recycle();
            }

            if (isNinePatch) {
                NativeScaleNinePatch(*np, scale, outPadding);
                bm->SetNinePatchChunk(np);
            }
            if (lb != NULL) {
                AutoPtr< ArrayOf<Int32> > newLb = ArrayOf<Int32>::Alloc(lb->GetLength());
                for (Int32 i = 0; i < lb->GetLength(); i++) {
                    (*newLb)[i] = (Int32)(((*lb)[i] * scale) + .5f);
                }
                bm->SetLayoutBounds(newLb);
            }
        }
        bm->SetDensity(targetDensity);
    }

    return bm;
}

ECode CBitmapFactory::DecodeStream(
    /* [in] */ IInputStream* is,
    /* [out] */ IBitmap** bitmap)
{
    return DecodeStream(is, NULL, NULL, bitmap);
}

ECode CBitmapFactory::DecodeFileDescriptor(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ IRect* outPadding,
    /* [in] */ IBitmapFactoryOptions* opts,
    /* [out] */ IBitmap** bitmap)
{
    VALIDATE_NOT_NULL(bitmap);

    if (NativeIsSeekable(fd)) {
        AutoPtr<IBitmap> bm = NativeDecodeFileDescriptor(fd, outPadding, opts);
        AutoPtr<IBitmap> inbm;
        if (bm == NULL && opts != NULL && (opts->GetInBitmap((IBitmap**)&inbm), inbm != NULL)) {
            Logger::E(TAG, "Problem decoding into existing bitmap");
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
        bm = FinishDecode(bm, outPadding, opts);
        *bitmap = bm;
        REFCOUNT_ADD(*bitmap);
        return NOERROR;
    }
    else{
        AutoPtr<IFileInputStream> fis;
        CFileInputStream::New(fd, (IFileInputStream**)&fis);
        ECode ec = DecodeStream(fis, outPadding, opts, bitmap);
        fis->Close();
        return ec;
    }
}

ECode CBitmapFactory::DecodeFileDescriptor(
    /* [in] */ IFileDescriptor* fd,
    /* [out] */ IBitmap** bitmap)
{
    return DecodeFileDescriptor(fd, NULL, NULL, bitmap);
}

static Boolean OptionsPurgeable(IBitmapFactoryOptions* options)
{
    if (options == NULL) return FALSE;

    Boolean value;
    options->GetInPurgeable(&value);
    return value;
}

static Boolean OptionsShareable(IBitmapFactoryOptions* options)
{
    if (options == NULL) return FALSE;

    Boolean value;
    options->GetInInputShareable(&value);
    return value;
}

static Boolean OptionsJustBounds(IBitmapFactoryOptions* options)
{
    if (options == NULL) return FALSE;

    Boolean value;
    options->GetInJustDecodeBounds(&value);
    return value;
}

static Boolean OptionsCancel(IBitmapFactoryOptions* options)
{
    if (options == NULL) return FALSE;

    Boolean value;
    options->GetCancel(&value);
    return value;
}

static void ScaleNinePatchChunk(android::Res_png_9patch* chunk, Float scale)
{
    chunk->paddingLeft = Int32(chunk->paddingLeft * scale + 0.5f);
    chunk->paddingTop = Int32(chunk->paddingTop * scale + 0.5f);
    chunk->paddingRight = Int32(chunk->paddingRight * scale + 0.5f);
    chunk->paddingBottom = Int32(chunk->paddingBottom * scale + 0.5f);

    for (Int32 i = 0; i < chunk->numXDivs; i++) {
        chunk->xDivs[i] = Int32(chunk->xDivs[i] * scale + 0.5f);
        if (i > 0 && chunk->xDivs[i] == chunk->xDivs[i - 1]) {
            chunk->xDivs[i]++;
        }
    }

    for (Int32 i = 0; i < chunk->numYDivs; i++) {
        chunk->yDivs[i] = Int32(chunk->yDivs[i] * scale + 0.5f);
        if (i > 0 && chunk->yDivs[i] == chunk->yDivs[i - 1]) {
            chunk->yDivs[i]++;
        }
    }
}

static SkPixelRef* InstallPixelRef(
    /* [in] */ SkBitmap* bitmap,
    /* [in] */ SkStream* stream,
    /* [in] */ Int32 sampleSize,
    /* [in] */ Boolean ditherImage)
{
    SkImageRef* pr;
    // only use ashmem for large images, since mmaps come at a price
    if (bitmap->getSize() >= 32 * 1024) {
        pr = new SkImageRef_ashmem(stream, bitmap->config(), sampleSize);
    }
    else {
        pr = new SkImageRef_GlobalPool(stream, bitmap->config(), sampleSize);
    }
    pr->setDitherImage(ditherImage);
    bitmap->setPixelRef(pr)->unref();
    pr->isOpaque(bitmap);
    return pr;
}

static AutoPtr<IBitmap> DoDecode(
    /* [in] */ SkStream* stream,
    /* [in] */ IRect* padding,
    /* [in] */ IBitmapFactoryOptions* options,
    /* [in] */ Boolean allowPurgeable,
    /* [in] */ Boolean forcePurgeable = FALSE,
    /* [in] */ Boolean applyScale = FALSE,
    /* [in] */ Float scale = 1.0f)
{
    Int32 sampleSize = 1;

    SkImageDecoder::Mode mode = SkImageDecoder::kDecodePixels_Mode;
    //SkBitmap::Config prefConfig = SkBitmap::kARGB_8888_Config;
    SkBitmap::Config prefConfig = SkBitmap::kNo_Config;

    Boolean doDither = TRUE;
    Boolean isMutable = FALSE;
    Boolean willScale = applyScale && scale != 1.0f;
    Boolean isPurgeable = !willScale &&
            (forcePurgeable || (allowPurgeable && OptionsPurgeable(options)));
    Boolean preferQualityOverSpeed = FALSE;

    AutoPtr<IBitmap> bm;

    if (options != NULL) {
        options->GetInSampleSize(&sampleSize);
        if (OptionsJustBounds(options)) {
            mode = SkImageDecoder::kDecodeBounds_Mode;
        }

        // initialize these, in case we fail later on
        options->SetOutWidth(-1);
        options->SetOutHeight(-1);
        options->SetOutMimeType(String(NULL));

        BitmapConfig config;
        options->GetInPreferredConfig(&config);
        prefConfig = GraphicsNative::GetNativeBitmapConfig(config);
        options->GetInMutable(&isMutable);
        options->GetInDither(&doDither);
        options->GetInPreferQualityOverSpeed(&preferQualityOverSpeed);
        options->GetInBitmap((IBitmap**)&bm);
    }

    if (willScale && bm != NULL) {
        Logger::E(TAG, "Cannot pre-scale a reused bitmap");
        return NULL;
    }

    SkImageDecoder* decoder = SkImageDecoder::Factory(stream);
    if (decoder == NULL) {
        Logger::E(TAG, "SkImageDecoder::Factory returned null");
        return NULL;
    }

    decoder->setSampleSize(sampleSize);
    decoder->setDitherImage(doDither);
    decoder->setPreferQualityOverSpeed(preferQualityOverSpeed);

    NinePatchPeeker peeker(decoder);
    GraphicsNative::DroidPixelAllocator allocator;

    SkBitmap* bitmap;
    if (bm == NULL) {
        bitmap = new SkBitmap;
    }
    else {
        if (sampleSize != 1) {
            Logger::E(TAG, "SkImageDecoder: Cannot reuse bitmap with sampleSize != 1");
            return NULL;
        }
        bitmap = (SkBitmap*)((CBitmap*)bm.Get())->mNativeBitmap;
        // config of supplied bitmap overrules config set in options
        prefConfig = bitmap->getConfig();
    }

    SkAutoTDelete<SkImageDecoder> add(decoder);
    SkAutoTDelete<SkBitmap> adb(bitmap, bm == NULL);

    decoder->setPeeker(&peeker);
    if (!isPurgeable) {
        decoder->setAllocator(&allocator);
    }

    AutoDecoderCancel adc(options, decoder);

    // To fix the race condition in case "requestCancelDecode"
    // happens earlier than AutoDecoderCancel object is added
    // to the gAutoDecoderCancelMutex linked list.
    if (OptionsCancel(options)) {
        Logger::E(TAG, "options has been cancelled.");
        return NULL;
    }

    SkImageDecoder::Mode decodeMode = mode;
    if (isPurgeable) {
        decodeMode = SkImageDecoder::kDecodeBounds_Mode;
    }

    SkBitmap* decoded;
    if (willScale) {
        decoded = new SkBitmap;
    }
    else {
        decoded = bitmap;
    }
    SkAutoTDelete<SkBitmap> adb2(willScale ? decoded : NULL);

    if (!decoder->decode(stream, decoded, prefConfig, decodeMode, bm != NULL)) {
        Logger::E(TAG, "decoder->decode returned FALSE");
        return NULL;
    }

    Int32 scaledWidth = decoded->width();
    Int32 scaledHeight = decoded->height();

    if (willScale && mode != SkImageDecoder::kDecodeBounds_Mode) {
        scaledWidth = Int32(scaledWidth * scale + 0.5f);
        scaledHeight = Int32(scaledHeight * scale + 0.5f);
    }

    // update options (if any)
    if (options != NULL) {
        options->SetOutWidth(scaledWidth);
        options->SetOutHeight(scaledHeight);
        options->SetOutMimeType(NBitmapFactory::GetMimeTypeString(decoder->getFormat()));
    }

    // if we're in justBounds mode, return now (skip the java bitmap)
    if (mode == SkImageDecoder::kDecodeBounds_Mode) {
        Logger::E(TAG, "we're in justBounds mode");
        return NULL;
    }

    AutoPtr< ArrayOf<Byte> > ninePatchChunk;
    if (peeker.mPatch != NULL) {
        if (willScale) {
            ScaleNinePatchChunk(peeker.mPatch, scale);
        }

        size_t ninePatchArraySize = peeker.mPatch->serializedSize();
        ninePatchChunk = ArrayOf<Byte>::Alloc(ninePatchArraySize);
        if (ninePatchChunk == NULL) {
            Logger::E(TAG, "ninePatchChunk == null");
            return NULL;
        }

        Byte* array = ninePatchChunk->GetPayload();
        peeker.mPatch->serialize(array);
    }

    AutoPtr< ArrayOf<Int32> > layoutBounds;
    if (peeker.mLayoutBounds != NULL) {
        layoutBounds = ArrayOf<Int32>::Alloc(4);
        if (layoutBounds == NULL) {
            Logger::E(TAG, "layoutBounds == null");
            return NULL;
        }

        if (willScale) {
            for (Int32 i = 0; i < 4; i++) {
                (*layoutBounds)[i] = (Int32)((((Int32*)peeker.mLayoutBounds)[i] * scale) + .5f);
            }
        }
        else {
            memcpy(layoutBounds->GetPayload(), (Int32*)peeker.mLayoutBounds, layoutBounds->GetLength());
        }
        if (bm != NULL) {
            bm->SetLayoutBounds(layoutBounds);
        }
    }

    if (willScale) {
        // This is weird so let me explain: we could use the scale parameter
        // directly, but for historical reasons this is how the corresponding
        // Dalvik code has always behaved. We simply recreate the behavior here.
        // The result is slightly different from simply using scale because of
        // the 0.5f rounding bias applied when computing the target image size
        const Float sx = scaledWidth / Float(decoded->width());
        const Float sy = scaledHeight / Float(decoded->height());

        SkBitmap::Config config = decoded->config();
        switch (config) {
            case SkBitmap::kNo_Config:
            case SkBitmap::kIndex8_Config:
            case SkBitmap::kRLE_Index8_Config:
                config = SkBitmap::kARGB_8888_Config;
                break;
            default:
                break;
        }

        bitmap->setConfig(config, scaledWidth, scaledHeight);
        bitmap->setIsOpaque(decoded->isOpaque());
        if (!bitmap->allocPixels(&allocator, NULL)) {
            Logger::E(TAG, "allocation failed for scaled bitma");
            return NULL;
        }
        bitmap->eraseColor(0);

        SkPaint paint;
        paint.setFilterBitmap(TRUE);

        SkCanvas canvas(*bitmap);
        canvas.scale(sx, sy);
        canvas.drawBitmap(*decoded, 0.0f, 0.0f, &paint);
    }

    if (padding != NULL) {
        if (peeker.mPatch != NULL) {
            padding->Set(peeker.mPatch->paddingLeft,
                         peeker.mPatch->paddingTop,
                         peeker.mPatch->paddingRight,
                         peeker.mPatch->paddingBottom);
        }
        else {
            padding->Set(-1, -1, -1, -1);
        }
    }

    SkPixelRef* pr = NULL;
    if (isPurgeable) {
        pr = InstallPixelRef(bitmap, stream, sampleSize, doDither);
    }
    else {
        // if we get here, we're in kDecodePixels_Mode and will therefore
        // already have a pixelref installed.
        pr = bitmap->pixelRef();
    }
    if (pr == NULL) {
        Logger::E(TAG, "Got null SkPixelRef");
        return NULL;
    }

    if (!isMutable) {
        // promise we will never change our pixels (great for sharing and pictures)
        pr->setImmutable();
    }

    // detach bitmap from its autodeleter, since we want to own it now
    adb.detach();

    if (bm != NULL) {
        // If a java bitmap was passed in for reuse, pass it back
        return bm;
    }

    // now create the java bitmap
    AutoPtr<CBitmap> bmObj;
    GraphicsNative::CreateBitmap(bitmap, allocator.getStorageObj(),
            isMutable, ninePatchChunk, layoutBounds, -1, (CBitmap**)&bmObj);
    return (IBitmap*)bmObj.Get();
}

AutoPtr<IBitmap> CBitmapFactory::NativeDecodeStream(
    /* [in] */ IInputStream* is,
    /* [in] */ ArrayOf<Byte>* storage,
    /* [in] */ IRect* outPadding,
    /* [in] */ IBitmapFactoryOptions* opts,
    /* [in] */ Boolean applyScale,
    /* [in] */ Float scale)
{
    AutoPtr<IBitmap> bm;
    SkStream* stream = CreateInputStreamAdaptor(is, storage, 0);

    if (stream) {
        // for now we don't allow purgeable with java inputstreams
        bm = DoDecode(stream, outPadding, opts, FALSE, FALSE, applyScale, scale);
        stream->unref();
    }
    return bm;
}

AutoPtr<IBitmap> CBitmapFactory::NativeDecodeStream(
    /* [in] */ IInputStream* is,
    /* [in] */ ArrayOf<Byte>* storage,
    /* [in] */ IRect* outPadding,
    /* [in] */ IBitmapFactoryOptions* opts)
{
    return NativeDecodeStream(is, storage, outPadding, opts, FALSE, 1.0f);
}

AutoPtr<IBitmap> CBitmapFactory::NativeDecodeFileDescriptor(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ IRect* padding,
    /* [in] */ IBitmapFactoryOptions* opts)
{
    Int32 descriptor = 0;
    fd->GetDescriptor(&descriptor);

    Boolean isPurgeable = OptionsPurgeable(opts);
    Boolean isShareable = OptionsShareable(opts);
    Boolean weOwnTheFD = FALSE;
    if (isPurgeable && isShareable) {
        Int32 newFD = ::dup(descriptor);
        if (-1 != newFD) {
            weOwnTheFD = TRUE;
            descriptor = newFD;
        }
    }

    SkFDStream* stream = new SkFDStream(descriptor, weOwnTheFD);
    SkAutoUnref aur(stream);
    if (!stream->isValid()) {
        return NULL;
    }

    /* Restore our offset when we leave, so we can be called more than once
       with the same descriptor. This is only required if we didn't dup the
       file descriptor, but it is OK to do it all the time.
    */
    AutoFDSeek as(descriptor);

    /* Allow purgeable iff we own the FD, i.e., in the puregeable and
       shareable case.
    */
    return DoDecode(stream, padding, opts, weOwnTheFD, FALSE);
}

static SkStream* CopyAssetToStream(
    /* [in] */ android::Asset* asset)
{
    // if we could "ref/reopen" the asset, we may not need to copy it here
    off_t size = asset->seek(0, SEEK_SET);
    if ((off_t)-1 == size) {
        Logger::E(TAG, "---- copyAsset: asset rewind failed\n");
        return NULL;
    }

    size = asset->getLength();
    if (size <= 0) {
        Logger::E(TAG, "---- copyAsset: asset->getLength() returned %d\n" + (Int32)size);
        return NULL;
    }

    SkStream* stream = new SkMemoryStream(size);
    void* data = const_cast<void*>(stream->getMemoryBase());
    off_t len = asset->read(data, size);
    if (len != size) {
        Logger::E(TAG, "---- copyAsset: asset->read(%d) returned %d\n" + (Int32)size + (Int32)len);
        delete stream;
        stream = NULL;
    }
    return stream;
}

AutoPtr<IBitmap> CBitmapFactory::NativeDecodeAsset(
    /* [in] */ Int32 native_asset,
    /* [in] */ IRect* padding,
    /* [in] */ IBitmapFactoryOptions* opts,
    /* [in] */ Boolean applyScale,
    /* [in] */ Float scale)
{
    SkStream* stream = NULL;
    android::Asset* asset = reinterpret_cast<android::Asset*>(native_asset);
    Boolean forcePurgeable = OptionsPurgeable(opts);
    if (forcePurgeable) {
        // if we could "ref/reopen" the asset, we may not need to copy it here
        // and we could assume optionsShareable, since assets are always RO
        stream = CopyAssetToStream(asset);
        if (NULL == stream) {
            return NULL;
        }
    }
    else {
        // since we know we'll be done with the asset when we return, we can
        // just use a simple wrapper
        stream = new AssetStreamAdaptor(asset);
    }
    SkAutoUnref aur(stream);
    return DoDecode(stream, padding, opts, TRUE, forcePurgeable, applyScale, scale);
}

AutoPtr<IBitmap> CBitmapFactory::NativeDecodeAsset(
    /* [in] */ Int32 native_asset,
    /* [in] */ IRect* padding,
    /* [in] */ IBitmapFactoryOptions* opts)
{
    return NativeDecodeAsset(native_asset, padding, opts, FALSE, 1.0f);
}

AutoPtr<IBitmap> CBitmapFactory::NativeDecodeByteArray(
    /* [in] */ const ArrayOf<Byte>& data,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [in] */ IBitmapFactoryOptions* options)
{
    /*  If optionsShareable() we could decide to just wrap the java array and
        share it, but that means adding a globalref to the java array object
        and managing its lifetime. For now we just always copy the array's data
        if optionsPurgeable().
     */
    Boolean purgeable = OptionsPurgeable(options) && !OptionsJustBounds(options);
    Byte* ar = data.GetPayload();
    SkStream* stream = new SkMemoryStream(ar + offset, length, purgeable);
    SkAutoUnref aur(stream);
    return DoDecode(stream, NULL, options, purgeable);
}

void CBitmapFactory::NativeScaleNinePatch(
    /* [in] */ const ArrayOf<Byte>& chunkObject,
    /* [in] */ Float scale,
    /* [in] */ IRect* padding)
{
    Byte* array = chunkObject.GetPayload();
    if (array != NULL) {
        Int32 chunkSize = chunkObject.GetLength();
        void* storage = alloca(chunkSize);
        android::Res_png_9patch* chunk = static_cast<android::Res_png_9patch*>(storage);
        memcpy(chunk, array, chunkSize);
        android::Res_png_9patch::deserialize(chunk);

        ScaleNinePatchChunk(chunk, scale);
        memcpy(array, chunk, chunkSize);

        if (padding) {
            padding->Set(chunk->paddingLeft, chunk->paddingTop,
                         chunk->paddingRight, chunk->paddingBottom);
        }
    }
}

Boolean CBitmapFactory::NativeIsSeekable(
    /* [in] */ IFileDescriptor* fd)
{
    assert(fd);

    Int32 descriptor = 0;
    fd->GetDescriptor(&descriptor);
    return ::lseek64(descriptor, 0, SEEK_CUR) != -1 ? TRUE : FALSE;
}

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos

