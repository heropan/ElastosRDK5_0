
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
#include "utility/CTypedValue.h"
// #include "content/res/CAssetManager.h"
#include <elastos/utility/logging/Logger.h>
#include <skia/core/SkBitmap.h>
#include <skia/core/SkTemplates.h>
#include <skia/core/SkCanvas.h>
#include <skia/core/SkPixelRef.h>
#include <skia/core/SkStream.h>
#include <skia/utils/SkFrontBufferedStream.h>
#include <skia/core/SkImageDecoder.h>
#include <skia/core/SkMath.h>
#include <skia/core/SkUtils.h>
#include <skia/core/SkMath.h>
#include <androidfw/Asset.h>
#include <androidfw/ResourceTypes.h>
#include <cutils/compiler.h>

using Elastos::Droid::Content::Res::IAssetInputStream;
// using Elastos::Droid::Content::Res::CAssetManager;
using Elastos::Droid::Utility::CTypedValue;

using Elastos::IO::ICloseable;
using Elastos::IO::IFileInputStream;
using Elastos::IO::CFileInputStream;
using Elastos::IO::IBufferedInputStream;
using Elastos::IO::CBufferedInputStream;
using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Graphics {

////////////////////////////////////////////////////////////////////////////////
// CBitmapFactory

static const String TAG = String("CBitmapFactory");
// const Int32 CBitmapFactory::DECODE_BUFFER_SIZE = 16 * 1024;
const Int32 CBitmapFactory::DECODE_BUFFER_SIZE = 300 * 1024;

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
    /* [in] */ ArrayOf<Int32>* colors,
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
    /* [in] */ ArrayOf<Int32>* colors,
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
    /* [in] */ ArrayOf<Int32>* colors,
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
    /* [in] */ ArrayOf<Int32>* colors,
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

    //ACTIONS_CODE_START
    AutoPtr<IBitmapFactoryOptions> options = opts;
    if (options == NULL) {
        CBitmapFactoryOptions::New((IBitmapFactoryOptions**)&options);
        ((CBitmapFactoryOptions*)options.Get())->mNewOptsFlag = TRUE;
    }
    //ACTIONS_CODE_END

    AutoPtr<IFileInputStream> fstream;
    ECode ec = CFileInputStream::New(pathName, (IFileInputStream**)&fstream);
    if (FAILED(ec)) {
        Logger::E(TAG, "BitmapFactory, Unable to decode stream");
        return ec;
    }

    ec = DecodeStream(IInputStream::Probe(fstream), NULL, options, bitmap);
    if (FAILED(ec)) {
        Logger::E(TAG, "BitmapFactory, Unable to decode stream");
    }

    ICloseable::Probe(fstream)->Close();
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
    if (is != NULL) ICloseable::Probe(is)->Close();

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
    /* [in] */ ArrayOf<Byte>* data,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [in] */ IBitmapFactoryOptions* _opts,
    /* [out] */ IBitmap** bitmap)
{
    if ((offset | length) < 0 || data->GetLength() < offset + length) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    //ACTIONS_CODE_START
    AutoPtr<IBitmapFactoryOptions> opts = _opts;
     if (opts == NULL) {
        CBitmapFactoryOptions::New((IBitmapFactoryOptions**)&opts);
        ((CBitmapFactoryOptions*)opts.Get())->mNewOptsFlag = TRUE;
    }
    //ACTIONS_CODE_END

    // Trace.traceBegin(Trace.TRACE_TAG_GRAPHICS, "decodeBitmap");
    // try {
    AutoPtr<IBitmap> bm = NativeDecodeByteArray(data, offset, length, opts);
    AutoPtr<IBitmap> inbm;
    if (bm == NULL && opts != NULL && (opts->GetInBitmap((IBitmap**)&inbm), inbm != NULL)) {
        // throw new IllegalArgumentException("Problem decoding into existing bitmap");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    SetDensityFromOptions(bm, opts);
    // } finally {
    //     Trace.traceEnd(Trace.TRACE_TAG_GRAPHICS);
    // }

    *bitmap = bm;
    REFCOUNT_ADD(*bitmap);
    return NOERROR;
}

ECode CBitmapFactory::DecodeByteArray(
    /* [in] */ ArrayOf<Byte>* data,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ IBitmap** bitmap)
{
    return DecodeByteArray(data, offset, length, NULL, bitmap);
}

void CBitmapFactory::SetDensityFromOptions(
    /* [in] */ IBitmap* outputBitmap,
    /* [in] */ IBitmapFactoryOptions* opts)
{
    if (outputBitmap == NULL || opts == NULL) return;

    Int32 density = 0;
    opts->GetInDensity(&density);
    AutoPtr<IBitmap> inBitmap;
    if (density != 0) {
        outputBitmap->SetDensity(density);
        Int32 targetDensity = 0;
        opts->GetInTargetDensity(&targetDensity);
        Int32 inScreenDensity = 0;
        if (targetDensity == 0 || density == targetDensity || density == (opts->GetInScreenDensity(&inScreenDensity), inScreenDensity)) {
            return;
        }

        AutoPtr<ArrayOf<Byte> > np;
        outputBitmap->GetNinePatchChunk((ArrayOf<Byte>**)&np);
        Boolean isNinePatch = np != NULL && NinePatch::IsNinePatchChunk(np);
        Boolean inScaled = FALSE;
        if ((opts->GetInScaled(&inScaled), inScaled) || isNinePatch) {
            outputBitmap->SetDensity(targetDensity);
        }
    } else if ((opts->GetInBitmap((IBitmap**)&inBitmap), inBitmap.Get()) != NULL) {
        // bitmap was reused, ensure density is reset
        outputBitmap->SetDensity(CBitmap::GetDefaultDensity());
    }
}

ECode CBitmapFactory::DecodeStream(
    /* [in] */ IInputStream* _is,
    /* [in] */ IRect* outPadding,
    /* [in] */ IBitmapFactoryOptions* _opts,
    /* [out] */ IBitmap** bitmap)
{
    VALIDATE_NOT_NULL(bitmap);
    *bitmap = NULL;

    // we don't throw in this case, thus allowing the caller to only check
    // the cache, and not force the image to be decoded.
    if (_is == NULL) {
        return NOERROR;
    }

    //ACTIONS_CODE_START
    AutoPtr<IInputStream> is = _is;
    Boolean supported = FALSE;
    is->IsMarkSupported(&supported);
    if (!supported) {
        is = NULL;
        FAIL_RETURN(CBufferedInputStream::New(_is, DECODE_BUFFER_SIZE, (IBufferedInputStream**)&is));
    }

    is->Mark(DECODE_BUFFER_SIZE);
    //ACTIONS_CODE_END
    AutoPtr<IBitmap> bm;
    //ACTIONS_CODE_START
    AutoPtr<IBitmapFactoryOptions> opts = _opts;
    if (opts == NULL) {
        CBitmapFactoryOptions::New((IBitmapFactoryOptions**)&opts);
        ((CBitmapFactoryOptions*)opts.Get())->mNewOptsFlag = TRUE;
    }
    //ACTIONS_CODE_END

    // Trace.traceBegin(Trace.TRACE_TAG_GRAPHICS, "decodeBitmap");
    // try {
    if (IAssetInputStream::Probe(is)) {
        Int64 asset = 0;
        assert(0 && "TODO");
        // ((CAssetManager::AssetInputStream*)IAssetInputStream::Probe(is))->GetNativeAsset(&asset);
        bm = NativeDecodeAsset(asset, outPadding, opts);
    } else {
        bm = DecodeStreamInternal(is, outPadding, opts);
    }

    AutoPtr<IBitmap> inBitmap;
    if (bm == NULL && opts != NULL && (opts->GetInBitmap((IBitmap**)&inBitmap), inBitmap.Get()) != NULL) {
        // throw new IllegalArgumentException("Problem decoding into existing bitmap");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    SetDensityFromOptions(bm, opts);
    // } finally {
    //     Trace.traceEnd(Trace.TRACE_TAG_GRAPHICS);
    // }

    *bitmap = bm;
    REFCOUNT_ADD(*bitmap);
    return NOERROR;
}

AutoPtr<IBitmap> CBitmapFactory::DecodeStreamInternal(
    /* [in] */ IInputStream* is,
    /* [in] */ IRect* outPadding,
    /* [in] */ IBitmapFactoryOptions* opts)
{
    // ASSERT(is != null);
    AutoPtr<ArrayOf<Byte> > tempStorage;
    if (opts != NULL) {
        opts->GetInTempStorage((ArrayOf<Byte>**)&tempStorage);
    }
    if (tempStorage == NULL) {
        tempStorage = ArrayOf<Byte>::Alloc(DECODE_BUFFER_SIZE);
    }
    AutoPtr<IBitmap> mp = NativeDecodeStream(is, tempStorage, outPadding, opts);
    return mp;
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
    /* [in] */ IBitmapFactoryOptions* _opts,
    /* [out] */ IBitmap** bitmap)
{
    VALIDATE_NOT_NULL(bitmap);

    AutoPtr<IBitmap> bm;
    //ACTIONS_CODE_START
    AutoPtr<IBitmapFactoryOptions> opts = _opts;
    if (opts == NULL) {
        CBitmapFactoryOptions::New((IBitmapFactoryOptions**)&opts);
        ((CBitmapFactoryOptions*)opts.Get())->mNewOptsFlag = TRUE;
    }
    //ACTIONS_CODE_END
    // Trace.traceBegin(Trace.TRACE_TAG_GRAPHICS, "decodeFileDescriptor");
    // try {
    if (NativeIsSeekable(fd)) {
        bm = NativeDecodeFileDescriptor(fd, outPadding, opts);
    } else {
        AutoPtr<IFileInputStream> fis;
        CFileInputStream::New(fd, (IFileInputStream**)&fis);
        // try {
        bm = DecodeStreamInternal(IInputStream::Probe(fis), outPadding, opts);
        // } finally {
        //     try {
        ICloseable::Probe(fis)->Close();
        //     } catch (Throwable t) {/* ignore */}
        // }
    }

    AutoPtr<IBitmap> inbm;
    if (bm == NULL && opts != NULL && (opts->GetInBitmap((IBitmap**)&inbm), inbm) != NULL) {
        Logger::E(TAG, "Problem decoding into existing bitmap");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    SetDensityFromOptions(bm, opts);
    // } finally {
    //     Trace.traceEnd(Trace.TRACE_TAG_GRAPHICS);
    // }
    *bitmap = bm;
    REFCOUNT_ADD(*bitmap);
    return NOERROR;
}

ECode CBitmapFactory::DecodeFileDescriptor(
    /* [in] */ IFileDescriptor* fd,
    /* [out] */ IBitmap** bitmap)
{
    return DecodeFileDescriptor(fd, NULL, NULL, bitmap);
}

static void ScaleDivRange(int32_t* divs, int count, float scale, int maxValue) {
    for (int i = 0; i < count; i++) {
        divs[i] = int32_t(divs[i] * scale + 0.5f);
        if (i > 0 && divs[i] == divs[i - 1]) {
            divs[i]++; // avoid collisions
        }
    }

    if (CC_UNLIKELY(divs[count - 1] > maxValue)) {
        // if the collision avoidance above put some divs outside the bounds of the bitmap,
        // slide outer stretchable divs inward to stay within bounds
        int highestAvailable = maxValue;
        for (int i = count - 1; i >= 0; i--) {
            divs[i] = highestAvailable;
            if (i > 0 && divs[i] <= divs[i-1]){
                // keep shifting
                highestAvailable = divs[i] - 1;
            } else {
                break;
            }
        }
    }
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

static void ScaleNinePatchChunk(android::Res_png_9patch* chunk, float scale,
        int scaledWidth, int scaledHeight) {
    chunk->paddingLeft = int(chunk->paddingLeft * scale + 0.5f);
    chunk->paddingTop = int(chunk->paddingTop * scale + 0.5f);
    chunk->paddingRight = int(chunk->paddingRight * scale + 0.5f);
    chunk->paddingBottom = int(chunk->paddingBottom * scale + 0.5f);

    ScaleDivRange(chunk->getXDivs(), chunk->numXDivs, scale, scaledWidth);
    ScaleDivRange(chunk->getYDivs(), chunk->numYDivs, scale, scaledHeight);
}

static SkColorType ColorTypeForScaledOutput(SkColorType colorType) {
    switch (colorType) {
        case kUnknown_SkColorType:
        case kIndex_8_SkColorType:
            return kN32_SkColorType;
        default:
            break;
    }
    return colorType;
}

class ScaleCheckingAllocator : public SkBitmap::HeapAllocator {
public:
    ScaleCheckingAllocator(float scale, int size)
            : mScale(scale), mSize(size) {
    }

    virtual bool allocPixelRef(SkBitmap* bitmap, SkColorTable* ctable) {
        // accounts for scale in final allocation, using eventual size and config
        const int bytesPerPixel = SkColorTypeBytesPerPixel(
                ColorTypeForScaledOutput(bitmap->colorType()));
        const int requestedSize = bytesPerPixel *
                int(bitmap->width() * mScale + 0.5f) *
                int(bitmap->height() * mScale + 0.5f);
        if (requestedSize > mSize) {
            ALOGW("bitmap for alloc reuse (%d bytes) can't fit scaled bitmap (%d bytes)",
                    mSize, requestedSize);
            return false;
        }
        return SkBitmap::HeapAllocator::allocPixelRef(bitmap, ctable);
    }
private:
    const float mScale;
    const int mSize;
};

class RecyclingPixelAllocator : public SkBitmap::Allocator {
public:
    RecyclingPixelAllocator(SkPixelRef* pixelRef, unsigned int size)
            : mPixelRef(pixelRef), mSize(size) {
        SkSafeRef(mPixelRef);
    }

    ~RecyclingPixelAllocator() {
        SkSafeUnref(mPixelRef);
    }

    virtual bool allocPixelRef(SkBitmap* bitmap, SkColorTable* ctable) {
        const SkImageInfo& info = bitmap->info();
        if (info.fColorType == kUnknown_SkColorType) {
            ALOGW("unable to reuse a bitmap as the target has an unknown bitmap configuration");
            return false;
        }

        const int64_t size64 = info.getSafeSize64(bitmap->rowBytes());
        if (!sk_64_isS32(size64)) {
            ALOGW("bitmap is too large");
            return false;
        }

        const size_t size = sk_64_asS32(size64);
        if (size > mSize) {
            ALOGW("bitmap marked for reuse (%d bytes) can't fit new bitmap (%d bytes)",
                    mSize, size);
            return false;
        }

        // Create a new pixelref with the new ctable that wraps the previous pixelref
        assert(0 && "TODO");
        // SkPixelRef* pr = new android::AndroidPixelRef(*static_cast<android::AndroidPixelRef*>(mPixelRef),
        //         info, bitmap->rowBytes(), ctable);
        // bitmap->setPixelRef(pr)->unref();

        // since we're already allocated, we lockPixels right away
        // HeapAllocator/JavaPixelAllocator behaves this way too
        bitmap->lockPixels();
        return true;
    }

private:
    SkPixelRef* const mPixelRef;
    const unsigned int mSize;
};

static AutoPtr<IBitmap> DoDecode(
    /* [in] */ SkStreamRewindable* stream,
    /* [in] */ IRect* padding,
    /* [in] */ IBitmapFactoryOptions* options)
{
    assert(0 && "TODO");
    // int sampleSize = 1;

    // SkImageDecoder::Mode decodeMode = SkImageDecoder::kDecodePixels_Mode;
    // SkColorType prefColorType = kN32_SkColorType;

    // bool doDither = true;
    // bool isMutable = false;
    // float scale = 1.0f;
    // bool preferQualityOverSpeed = false;
    // bool requireUnpremultiplied = false;


    // jobject javaBitmap = NULL;

    // if (options != NULL) {
    //     sampleSize = env->GetIntField(options, gOptions_sampleSizeFieldID);
    //     if (optionsJustBounds(env, options)) {
    //         decodeMode = SkImageDecoder::kDecodeBounds_Mode;
    //     }

    //     // initialize these, in case we fail later on
    //     env->SetIntField(options, gOptions_widthFieldID, -1);
    //     env->SetIntField(options, gOptions_heightFieldID, -1);
    //     env->SetObjectField(options, gOptions_mimeFieldID, 0);

    //     jobject jconfig = env->GetObjectField(options, gOptions_configFieldID);
    //     prefColorType = GraphicsJNI::getNativeBitmapColorType(env, jconfig);
    //     isMutable = env->GetBooleanField(options, gOptions_mutableFieldID);
    //     doDither = env->GetBooleanField(options, gOptions_ditherFieldID);
    //     preferQualityOverSpeed = env->GetBooleanField(options,
    //             gOptions_preferQualityOverSpeedFieldID);
    //     requireUnpremultiplied = !env->GetBooleanField(options, gOptions_premultipliedFieldID);
    //     javaBitmap = env->GetObjectField(options, gOptions_bitmapFieldID);

    //     if (env->GetBooleanField(options, gOptions_scaledFieldID)) {
    //         const int density = env->GetIntField(options, gOptions_densityFieldID);
    //         const int targetDensity = env->GetIntField(options, gOptions_targetDensityFieldID);
    //         const int screenDensity = env->GetIntField(options, gOptions_screenDensityFieldID);
    //         if (density != 0 && targetDensity != 0 && density != screenDensity) {
    //             scale = (float) targetDensity / density;
    //         }
    //     }
    // }

    // const bool willScale = scale != 1.0f;

    // SkImageDecoder* decoder = SkImageDecoder::Factory(stream);
    // if (decoder == NULL) {
    //     return nullObjectReturn("SkImageDecoder::Factory returned null");
    // }

    // decoder->setSampleSize(sampleSize);
    // decoder->setDitherImage(doDither);
    // decoder->setPreferQualityOverSpeed(preferQualityOverSpeed);
    // decoder->setRequireUnpremultipliedColors(requireUnpremultiplied);

    // SkBitmap* outputBitmap = NULL;
    // unsigned int existingBufferSize = 0;
    // if (javaBitmap != NULL) {
    //     outputBitmap = (SkBitmap*) env->GetLongField(javaBitmap, gBitmap_nativeBitmapFieldID);
    //     if (outputBitmap->isImmutable()) {
    //         ALOGW("Unable to reuse an immutable bitmap as an image decoder target.");
    //         javaBitmap = NULL;
    //         outputBitmap = NULL;
    //     } else {
    //         existingBufferSize = GraphicsJNI::getBitmapAllocationByteCount(env, javaBitmap);
    //     }
    // }

    // SkAutoTDelete<SkBitmap> adb(outputBitmap == NULL ? new SkBitmap : NULL);
    // if (outputBitmap == NULL) outputBitmap = adb.get();

    // NinePatchPeeker peeker(decoder);
    // decoder->setPeeker(&peeker);

    // JavaPixelAllocator javaAllocator(env);
    // RecyclingPixelAllocator recyclingAllocator(outputBitmap->pixelRef(), existingBufferSize);
    // ScaleCheckingAllocator scaleCheckingAllocator(scale, existingBufferSize);
    // SkBitmap::Allocator* outputAllocator = (javaBitmap != NULL) ?
    //         (SkBitmap::Allocator*)&recyclingAllocator : (SkBitmap::Allocator*)&javaAllocator;
    // if (decodeMode != SkImageDecoder::kDecodeBounds_Mode) {
    //     if (!willScale) {
    //         // If the java allocator is being used to allocate the pixel memory, the decoder
    //         // need not write zeroes, since the memory is initialized to 0.
    //         decoder->setSkipWritingZeroes(outputAllocator == &javaAllocator);
    //         decoder->setAllocator(outputAllocator);
    //     } else if (javaBitmap != NULL) {
    //         // check for eventual scaled bounds at allocation time, so we don't decode the bitmap
    //         // only to find the scaled result too large to fit in the allocation
    //         decoder->setAllocator(&scaleCheckingAllocator);
    //     }
    // }

    // // Only setup the decoder to be deleted after its stack-based, refcounted
    // // components (allocators, peekers, etc) are declared. This prevents RefCnt
    // // asserts from firing due to the order objects are deleted from the stack.
    // SkAutoTDelete<SkImageDecoder> add(decoder);

    // AutoDecoderCancel adc(options, decoder);

    // // To fix the race condition in case "requestCancelDecode"
    // // happens earlier than AutoDecoderCancel object is added
    // // to the gAutoDecoderCancelMutex linked list.
    // if (options != NULL && env->GetBooleanField(options, gOptions_mCancelID)) {
    //     return nullObjectReturn("gOptions_mCancelID");
    // }

    // SkBitmap decodingBitmap;
    // if (!decoder->decode(stream, &decodingBitmap, prefColorType, decodeMode)) {
    //     return nullObjectReturn("decoder->decode returned false");
    // }

    // int scaledWidth = decodingBitmap.width();
    // int scaledHeight = decodingBitmap.height();

    // if (willScale && decodeMode != SkImageDecoder::kDecodeBounds_Mode) {
    //     scaledWidth = int(scaledWidth * scale + 0.5f);
    //     scaledHeight = int(scaledHeight * scale + 0.5f);
    // }

    // // update options (if any)
    // if (options != NULL) {
    //     env->SetIntField(options, gOptions_widthFieldID, scaledWidth);
    //     env->SetIntField(options, gOptions_heightFieldID, scaledHeight);
    //     env->SetObjectField(options, gOptions_mimeFieldID,
    //             getMimeTypeString(env, decoder->getFormat()));
    // }

    // // if we're in justBounds mode, return now (skip the java bitmap)
    // if (decodeMode == SkImageDecoder::kDecodeBounds_Mode) {
    //     return NULL;
    // }

    // jbyteArray ninePatchChunk = NULL;
    // if (peeker.mPatch != NULL) {
    //     if (willScale) {
    //         scaleNinePatchChunk(peeker.mPatch, scale, scaledWidth, scaledHeight);
    //     }

    //     size_t ninePatchArraySize = peeker.mPatch->serializedSize();
    //     ninePatchChunk = env->NewByteArray(ninePatchArraySize);
    //     if (ninePatchChunk == NULL) {
    //         return nullObjectReturn("ninePatchChunk == null");
    //     }

    //     jbyte* array = (jbyte*) env->GetPrimitiveArrayCritical(ninePatchChunk, NULL);
    //     if (array == NULL) {
    //         return nullObjectReturn("primitive array == null");
    //     }

    //     memcpy(array, peeker.mPatch, peeker.mPatchSize);
    //     env->ReleasePrimitiveArrayCritical(ninePatchChunk, array, 0);
    // }

    // jobject ninePatchInsets = NULL;
    // if (peeker.mHasInsets) {
    //     ninePatchInsets = env->NewObject(gInsetStruct_class, gInsetStruct_constructorMethodID,
    //             peeker.mOpticalInsets[0], peeker.mOpticalInsets[1], peeker.mOpticalInsets[2], peeker.mOpticalInsets[3],
    //             peeker.mOutlineInsets[0], peeker.mOutlineInsets[1], peeker.mOutlineInsets[2], peeker.mOutlineInsets[3],
    //             peeker.mOutlineRadius, peeker.mOutlineAlpha, scale);
    //     if (ninePatchInsets == NULL) {
    //         return nullObjectReturn("nine patch insets == null");
    //     }
    //     if (javaBitmap != NULL) {
    //         env->SetObjectField(javaBitmap, gBitmap_ninePatchInsetsFieldID, ninePatchInsets);
    //     }
    // }

    // if (willScale) {
    //     // This is weird so let me explain: we could use the scale parameter
    //     // directly, but for historical reasons this is how the corresponding
    //     // Dalvik code has always behaved. We simply recreate the behavior here.
    //     // The result is slightly different from simply using scale because of
    //     // the 0.5f rounding bias applied when computing the target image size
    //     const float sx = scaledWidth / float(decodingBitmap.width());
    //     const float sy = scaledHeight / float(decodingBitmap.height());

    //     // TODO: avoid copying when scaled size equals decodingBitmap size
    //     SkColorType colorType = colorTypeForScaledOutput(decodingBitmap.colorType());
    //     // FIXME: If the alphaType is kUnpremul and the image has alpha, the
    //     // colors may not be correct, since Skia does not yet support drawing
    //     // to/from unpremultiplied bitmaps.
    //     outputBitmap->setInfo(SkImageInfo::Make(scaledWidth, scaledHeight,
    //             colorType, decodingBitmap.alphaType()));
    //     if (!outputBitmap->allocPixels(outputAllocator, NULL)) {
    //         return nullObjectReturn("allocation failed for scaled bitmap");
    //     }

    //     // If outputBitmap's pixels are newly allocated by Java, there is no need
    //     // to erase to 0, since the pixels were initialized to 0.
    //     if (outputAllocator != &javaAllocator) {
    //         outputBitmap->eraseColor(0);
    //     }

    //     SkPaint paint;
    //     paint.setFilterLevel(SkPaint::kLow_FilterLevel);

    //     SkCanvas canvas(*outputBitmap);
    //     canvas.scale(sx, sy);
    //     canvas.drawBitmap(decodingBitmap, 0.0f, 0.0f, &paint);
    // } else {
    //     outputBitmap->swap(decodingBitmap);
    // }

    // if (padding) {
    //     if (peeker.mPatch != NULL) {
    //         GraphicsJNI::set_jrect(env, padding,
    //                 peeker.mPatch->paddingLeft, peeker.mPatch->paddingTop,
    //                 peeker.mPatch->paddingRight, peeker.mPatch->paddingBottom);
    //     } else {
    //         GraphicsJNI::set_jrect(env, padding, -1, -1, -1, -1);
    //     }
    // }

    // // if we get here, we're in kDecodePixels_Mode and will therefore
    // // already have a pixelref installed.
    // if (outputBitmap->pixelRef() == NULL) {
    //     return nullObjectReturn("Got null SkPixelRef");
    // }

    // if (!isMutable && javaBitmap == NULL) {
    //     // promise we will never change our pixels (great for sharing and pictures)
    //     outputBitmap->setImmutable();
    // }

    // // detach bitmap from its autodeleter, since we want to own it now
    // adb.detach();

    // if (javaBitmap != NULL) {
    //     bool isPremultiplied = !requireUnpremultiplied;
    //     GraphicsJNI::reinitBitmap(env, javaBitmap, outputBitmap, isPremultiplied);
    //     outputBitmap->notifyPixelsChanged();
    //     // If a java bitmap was passed in for reuse, pass it back
    //     return javaBitmap;
    // }

    // int bitmapCreateFlags = 0x0;
    // if (isMutable) bitmapCreateFlags |= GraphicsJNI::kBitmapCreateFlag_Mutable;
    // if (!requireUnpremultiplied) bitmapCreateFlags |= GraphicsJNI::kBitmapCreateFlag_Premultiplied;

    // // now create the java bitmap
    // return GraphicsJNI::createBitmap(env, outputBitmap, javaAllocator.getStorageObj(),
    //         bitmapCreateFlags, ninePatchChunk, ninePatchInsets, -1);
    return NULL;
}

// Need to buffer enough input to be able to rewind as much as might be read by a decoder
// trying to determine the stream's format. Currently the most is 64, read by
// SkImageDecoder_libwebp.
// FIXME: Get this number from SkImageDecoder

//actions_code(lishiyuan)
//#define BYTES_TO_BUFFER 64
#define BYTES_TO_BUFFER (300*1024)

AutoPtr<IBitmap> CBitmapFactory::NativeDecodeStream(
    /* [in] */ IInputStream* is,
    /* [in] */ ArrayOf<Byte>* storage,
    /* [in] */ IRect* outPadding,
    /* [in] */ IBitmapFactoryOptions* opts)
{
    AutoPtr<IBitmap> bitmap;
    assert(0 && "TODO");
    // SkAutoTUnref<SkStream> stream(CreateJavaInputStreamAdaptor(env, is, storage));

    // if (stream.get()) {
    //     SkAutoTUnref<SkStreamRewindable> bufferedStream(
    //             SkFrontBufferedStream::Create(stream, BYTES_TO_BUFFER));
    //     SkASSERT(bufferedStream.get() != NULL);
    //     bitmap = doDecode(env, bufferedStream, padding, options);
    // }
    return bitmap;
}

AutoPtr<IBitmap> CBitmapFactory::NativeDecodeFileDescriptor(
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ IRect* padding,
    /* [in] */ IBitmapFactoryOptions* opts)
{
    assert(0 && "TODO");
    // NPE_CHECK_RETURN_ZERO(env, fileDescriptor);

    // jint descriptor = jniGetFDFromFileDescriptor(env, fileDescriptor);

    // struct stat fdStat;
    // if (fstat(descriptor, &fdStat) == -1) {
    //     doThrowIOE(env, "broken file descriptor");
    //     return nullObjectReturn("fstat return -1");
    // }

    // // Restore the descriptor's offset on exiting this function.
    // AutoFDSeek autoRestore(descriptor);

    // FILE* file = fdopen(descriptor, "r");
    // if (file == NULL) {
    //     return nullObjectReturn("Could not open file");
    // }

    // SkAutoTUnref<SkFILEStream> fileStream(new SkFILEStream(file,
    //                      SkFILEStream::kCallerRetains_Ownership));

    // // Use a buffered stream. Although an SkFILEStream can be rewound, this
    // // ensures that SkImageDecoder::Factory never rewinds beyond the
    // // current position of the file descriptor.
    // SkAutoTUnref<SkStreamRewindable> stream(SkFrontBufferedStream::Create(fileStream,
    //         BYTES_TO_BUFFER));

    // return doDecode(env, stream, padding, bitmapFactoryOptions);
    return NULL;
}

AutoPtr<IBitmap> CBitmapFactory::NativeDecodeAsset(
    /* [in] */ Int64 native_asset,
    /* [in] */ IRect* padding,
    /* [in] */ IBitmapFactoryOptions* opts)
{
    android::Asset* asset = reinterpret_cast<android::Asset*>(native_asset);
    // since we know we'll be done with the asset when we return, we can
    // just use a simple wrapper
    assert(0 && "TODO");
    // SkAutoTUnref<SkStreamRewindable> stream(new AssetStreamAdaptor(asset,
    //         AssetStreamAdaptor::kNo_OwnAsset, AssetStreamAdaptor::kNo_HasMemoryBase));
    // return DoDecode(stream, padding, options);
    return NULL;
}

AutoPtr<IBitmap> CBitmapFactory::NativeDecodeByteArray(
    /* [in] */ ArrayOf<Byte>* data,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [in] */ IBitmapFactoryOptions* options)
{
    assert(0 && "TODO");
    // AutoJavaByteArray ar(env, byteArray);
    // SkMemoryStream* stream = new SkMemoryStream(ar.ptr() + offset, length, false);
    // SkAutoUnref aur(stream);
    // return doDecode(env, stream, NULL, options);
    return NULL;
}

// void CBitmapFactory::NativeScaleNinePatch(
//     /* [in] */ ArrayOf<Byte>* chunkObject,
//     /* [in] */ Float scale,
//     /* [in] */ IRect* padding)
// {
//     Byte* array = chunkObject.GetPayload();
//     if (array != NULL) {
//         Int32 chunkSize = chunkObject.GetLength();
//         void* storage = alloca(chunkSize);
//         android::Res_png_9patch* chunk = static_cast<android::Res_png_9patch*>(storage);
//         memcpy(chunk, array, chunkSize);
//         android::Res_png_9patch::deserialize(chunk);

//         ScaleNinePatchChunk(chunk, scale);
//         memcpy(array, chunk, chunkSize);

//         if (padding) {
//             padding->Set(chunk->paddingLeft, chunk->paddingTop,
//                          chunk->paddingRight, chunk->paddingBottom);
//         }
//     }
// }

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

