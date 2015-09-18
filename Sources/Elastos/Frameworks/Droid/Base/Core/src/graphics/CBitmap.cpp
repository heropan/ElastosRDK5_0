
#include "ext/frameworkext.h"
#include "graphics/CBitmap.h"
#include "graphics/CPaint.h"
#include "graphics/CCanvas.h"
#include "graphics/CMatrix.h"
#include "graphics/CRect.h"
#include "graphics/CRectF.h"
#include "graphics/GraphicsNative.h"
#include "graphics/CreateOutputStreamAdaptor.h"
#include "util/CDisplayMetrics.h"
#include "util/droid_nio_utils.h"
#include <elastos/core/Math.h>
#include <elastos/utility/logging/Logger.h>
#include <skia/core/SkUnPreMultiply.h>
#include <skia/images/SkImageEncoder.h>
#include <skia/core/SkDither.h>
#include <skia/core/SkBitmap.h>

using Elastos::Utility::Logging::Logger;
using Elastos::Droid::Utility::CDisplayMetrics;
using Elastos::Droid::Utility::AutoBufferPointer;

namespace Elastos {
namespace Droid {
namespace Graphics {

static const char* TAG = "CBitmap";
const Int32 CBitmap::DENSITY_NONE;
AutoPtr<IMatrix> CBitmap::sScaleMatrix;
Int32 CBitmap::sDefaultDensity = -1;
Int32 CBitmap::WORKING_COMPRESS_STORAGE = 4096;
Mutex CBitmap::sClassLock;

struct ElaBitmapCallback Init_BitmapCallback()
{
    static struct ElaBitmapCallback t =
    {
        &CBitmap::CreateBitmap,
        &CBitmap::CreateBitmap
    };
    Elastos_Bitmap_InitCallback((Int32)&t);
    return t;
}
struct ElaBitmapCallback sElaBitmapCallback = Init_BitmapCallback();

CAR_OBJECT_IMPL(CBitmap);
CAR_INTERFACE_IMPL_2(CBitmap, Object, IBitmap, IParcelable);
CBitmap::CBitmap()
    : mNativeBitmap(0)
    , mDensity(GetDefaultDensity())
    , mIsMutable(FALSE)
    , mWidth(-1)
    , mHeight(-1)
    , mRecycled(FALSE)
{}

CBitmap::~CBitmap()
{
    if (mNativeBitmap != 0) {
        NativeDestructor(mNativeBitmap);
    }
}

ECode CBitmap::constructor()
{
    return NOERROR;
}

ECode CBitmap::constructor(
    /* [in] */ Int32 nativeBitmap,
    /* [in] */ ArrayOf<Byte>* buffer,
    /* [in] */ Boolean isMutable,
    /* [in] */ ArrayOf<Byte>* ninePatchChunk,
    /* [in] */ Int32 density)
{
    return constructor(nativeBitmap, buffer, isMutable, ninePatchChunk, NULL, density);
}

ECode CBitmap::constructor(
    /* [in] */ Int32 nativeBitmap,
    /* [in] */ ArrayOf<Byte>* buffer,
    /* [in] */ Boolean isMutable,
    /* [in] */ ArrayOf<Byte>* ninePatchChunk,
    /* [in] */ ArrayOf<Int32>* layoutBounds,
    /* [in] */ Int32 density)
{
    if (nativeBitmap == 0) {
        // throw new RuntimeException("internal error: native bitmap is 0");
        return E_RUNTIME_EXCEPTION;
    }

    mBuffer = buffer;
    // we delete this in our finalizer
    mNativeBitmap = nativeBitmap;

    mIsMutable = isMutable;
    mNinePatchChunk = ninePatchChunk;
    mLayoutBounds = layoutBounds;
    if (density >= 0) {
        mDensity = density;
    }
    return NOERROR;
}

void CBitmap::SetDefaultDensity(
    /* [in] */ Int32 density)
{
    sDefaultDensity = density;
}

Int32 CBitmap::GetDefaultDensity()
{
    if (sDefaultDensity >= 0) {
        return sDefaultDensity;
    }
    sDefaultDensity = CDisplayMetrics::DENSITY_DEVICE;
    return sDefaultDensity;
}

ECode CBitmap::GetDensity(
    /* [out] */ Int32* density)
{
    VALIDATE_NOT_NULL(density);
    *density = mDensity;
    return NOERROR;
}

ECode CBitmap::SetDensity(
    /* [in] */ Int32 density)
{
    mDensity = density;
    return NOERROR;
}

ECode CBitmap::SetNinePatchChunk(
    /* [in] */ ArrayOf<Byte>* chunk)
{
    mNinePatchChunk = chunk;
    return NOERROR;
}

ECode CBitmap::SetLayoutBounds(
    /* [in] */ ArrayOf<Int32>* bounds)
{
    mLayoutBounds = bounds;
    return NOERROR;
}

ECode CBitmap::Recycle()
{
    if (!mRecycled) {
        if (NativeRecycle(mNativeBitmap)) {
            // return value indicates whether native pixel object was actually recycled.
            // FALSE indicates that it is still in use at the native level and these
            // objects should not be collected now. They will be collected later when the
            // Bitmap itself is collected.
            mBuffer = NULL;
            mNinePatchChunk = NULL;
        }
        mRecycled = TRUE;
    }
    return NOERROR;
}

ECode CBitmap::IsRecycled(
    /* [out] */ Boolean* isRecycled)
{
    VALIDATE_NOT_NULL(isRecycled);
    *isRecycled = mRecycled;
    return NOERROR;
}

ECode CBitmap::GetGenerationId(
    /* [out] */ Int32* id)
{
    VALIDATE_NOT_NULL(id);
    *id = NativeGenerationId(mNativeBitmap);
    return NOERROR;
}

ECode CBitmap::CheckRecycled(
    /* [in] */ CString errorMessage)
{
    if (mRecycled) {
        // throw new IllegalStateException(errorMessage);
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    return NOERROR;
}

ECode CBitmap::CheckXYSign(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    if (x < 0) {
        // throw new IllegalArgumentException("x must be >= 0");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (y < 0) {
        // throw new IllegalArgumentException("y must be >= 0");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

ECode CBitmap::CheckWidthHeight(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    if (width <= 0) {
        // throw new IllegalArgumentException("width must be > 0");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (height <= 0) {
        // throw new IllegalArgumentException("height must be > 0");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

BitmapConfig CBitmap::NativeToConfig(
    /* [in] */ Int32 ni)
{
    switch(ni) {
        case 0:
        case 1:
            return BitmapConfig_NONE;
        case 2:
            return BitmapConfig_ALPHA_8;
        case 3:
            return BitmapConfig_NONE;
        case 4:
            return BitmapConfig_RGB_565;
        case 5:
            return BitmapConfig_ARGB_4444;
        case 6:
            return BitmapConfig_ARGB_8888;
        default:
            assert(FALSE && "native config value invalid.");
            return BitmapConfig_NONE;
    }
}

ECode CBitmap::CopyPixelsToBuffer(
    /* [in] */ IBuffer* dst)
{
    Int32 elements = 0;
    dst->GetRemaining(&elements);
    Int32 shift = 0;
    if (dst->Probe(Elastos::IO::EIID_IByteBuffer)) {
        shift = 0;
    }
    else if (dst->Probe(Elastos::IO::EIID_IInt16Buffer)) {
        shift = 1;
    }
    else if (dst->Probe(Elastos::IO::EIID_IInt32Buffer)) {
        shift = 2;
    }
    else {
        Logger::E(TAG, "unsupported Buffer subclass");
        return E_RUNTIME_EXCEPTION;
    }

    Int64 bufferSize = (Int64)elements << shift;
    Int32 pixelSize32 = 0;
    GetByteCount(&pixelSize32);
    Int64 pixelSize =(Int64)pixelSize32;

    if (bufferSize < pixelSize) {
        Logger::E(TAG, "Buffer not large enough for pixels");
        return E_RUNTIME_EXCEPTION;
    }

    NativeCopyPixelsToBuffer(mNativeBitmap, dst);

    // now update the buffer's position
    Int32 position = 0;
    dst->GetPosition(&position);
    position += pixelSize >> shift;
    dst->SetPosition(position);
    return NOERROR;
}

ECode CBitmap::CopyPixelsFromBuffer(
    /* [in] */ IBuffer* src)
{
    FAIL_RETURN(CheckRecycled("copyPixelsFromBuffer called on recycled bitmap"));

    Int32 elements;
    src->GetRemaining(&elements);
    Int32 shift = 0;
    if (src->Probe(Elastos::IO::EIID_IByteBuffer)) {
        shift = 0;
    }
    else if (src->Probe(Elastos::IO::EIID_IInt16Buffer)) {
        shift = 1;
    }
    else if (src->Probe(Elastos::IO::EIID_IInt32Buffer)) {
        shift = 2;
    }
    else {
        Logger::E(TAG, "unsupported Buffer subclass");
        return E_RUNTIME_EXCEPTION;
    }

    Int64 bufferBytes = (Int64)elements << shift;

    Int32 bitmapBytes32=0;
    GetByteCount(&bitmapBytes32);
    Int64 bitmapBytes =(Int64)bitmapBytes32;

    if (bufferBytes < bitmapBytes) {
        Logger::E(TAG, "Buffer not large enough for pixels");
        return E_RUNTIME_EXCEPTION;
    }

    NativeCopyPixelsFromBuffer(mNativeBitmap, src);

    // now update the buffer's position
    Int32 position;
    src->GetPosition(&position);
    position += bitmapBytes >> shift;
    src->SetPosition(position);
    return NOERROR;
}

ECode CBitmap::Copy(
    /* [in] */ BitmapConfig config,
    /* [in] */ Boolean isMutable,
    /* [out] */ IBitmap** bitmap)
{
    VALIDATE_NOT_NULL(bitmap);

    FAIL_RETURN(CheckRecycled("Can't copy a recycled bitmap"));

    AutoPtr<CBitmap> b;
    FAIL_RETURN(NativeCopy(mNativeBitmap, (Int32)config, isMutable, (CBitmap**)&b));
    if (b != NULL) {
        b->mDensity = mDensity;
    }
    *bitmap = (IBitmap*)b.Get();
    REFCOUNT_ADD(*bitmap);
    return NOERROR;
}

ECode CBitmap::CreateScaledBitmap(
    /* [in] */ IBitmap* src,
    /* [in] */ Int32 dstWidth,
    /* [in] */ Int32 dstHeight,
    /* [in] */ Boolean filter,
    /* [out] */ IBitmap** bitmap)
{
    VALIDATE_NOT_NULL(bitmap);

    AutoPtr<IMatrix> m;
    {
        AutoLock lock(sClassLock);

        // small pool of just 1 matrix
        m = sScaleMatrix;
        sScaleMatrix = NULL;
    }

    if (m == NULL) {
        CMatrix::New((IMatrix**)&m);
    }

    Int32 width = 0, height = 0;
    src->GetWidth(&width);
    src->GetHeight(&height);
    Float sx = dstWidth  / (Float)width;
    Float sy = dstHeight / (Float)height;
    m->SetScale(sx, sy);
    FAIL_RETURN(CreateBitmap(src, 0, 0, width, height, m, filter, bitmap));

    {
        AutoLock lock(sClassLock);

        // do we need to check for NULL? why not just assign everytime?
        if (sScaleMatrix == NULL) {
            sScaleMatrix = m;
        }
    }

    return NOERROR;
}

ECode CBitmap::CreateBitmap(
    /* [in] */ IBitmap* src,
    /* [out] */ IBitmap** bitmap)
{
    VALIDATE_NOT_NULL(bitmap);

    Int32 width = 0, height = 0;
    src->GetWidth(&width);
    src->GetHeight(&height);
    return CreateBitmap(src, 0, 0, width, height, bitmap);
}

ECode CBitmap::CreateBitmap(
    /* [in] */ IBitmap* source,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [out] */ IBitmap** bitmap)
{
    VALIDATE_NOT_NULL(bitmap);

    return CreateBitmap(source, x, y, width, height, NULL, FALSE, bitmap);
}

ECode CBitmap::CreateBitmap(
    /* [in] */ IBitmap* source,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ IMatrix* m,
    /* [in] */ Boolean filter,
    /* [out] */ IBitmap** bitmap)
{
    VALIDATE_NOT_NULL(bitmap);

    FAIL_RETURN(CheckXYSign(x, y));
    FAIL_RETURN(CheckWidthHeight(width, height));
    Int32 sourceWidth = 0, sourceHeight = 0;
    source->GetWidth(&sourceWidth);
    source->GetHeight(&sourceHeight);
    if (x + width > sourceWidth) {
        Logger::E(TAG, "x + width must be <= bitmap->Width()");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (y + height > sourceHeight) {
        Logger::E(TAG, "y + height must be <= bitmap->Height()");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    // check if we can just return our argument unchanged
    Boolean sourceIsMutable = FALSE;
    Boolean isIdentity = FALSE;
    if ((source->IsMutable(&sourceIsMutable), !sourceIsMutable)
        && x == 0 && y == 0 && width == sourceWidth && height == sourceHeight
        && (m == NULL || (m->IsIdentity(&isIdentity), isIdentity))) {
        *bitmap = source;
        REFCOUNT_ADD(*bitmap);
        return NOERROR;
    }

    Int32 neww = width;
    Int32 newh = height;
    AutoPtr<CCanvas> canvas;
    FAIL_RETURN(CCanvas::NewByFriend((CCanvas**)&canvas));
    AutoPtr<IBitmap> bmp;
    AutoPtr<CPaint> paint;

    AutoPtr<CRect> srcR;
    FAIL_RETURN(CRect::NewByFriend(x, y, x + width, y + height, (CRect**)&srcR));
    AutoPtr<CRectF> dstR;
    FAIL_RETURN(CRectF::NewByFriend(0, 0, width, height, (CRectF**)&dstR));

    BitmapConfig newConfig = BitmapConfig_ARGB_8888;
    BitmapConfig config;
    source->GetConfig(&config);
    // GIF files generate null configs, assume ARGB_8888
    if (config != BitmapConfig_NONE) {
        switch (config) {
            case BitmapConfig_RGB_565:
                newConfig = BitmapConfig_RGB_565;
                break;
            case BitmapConfig_ALPHA_8:
                newConfig = BitmapConfig_ALPHA_8;
                break;
            //noinspection deprecation
            case BitmapConfig_ARGB_4444:
            case BitmapConfig_ARGB_8888:
            default:
                newConfig = BitmapConfig_ARGB_8888;
                break;
        }
    }

    if (m == NULL || (m->IsIdentity(&isIdentity), isIdentity)) {
        Boolean hasAlpha = FALSE;
        source->HasAlpha(&hasAlpha);
        FAIL_RETURN(CreateBitmap(neww, newh, newConfig, hasAlpha, (IBitmap**)&bmp));
        paint = NULL; // not needed
    }
    else {
        Boolean isStays = FALSE;
        m->RectStaysRect(&isStays);
        Boolean transformed = !isStays;

        AutoPtr<CRectF> deviceR;
        FAIL_RETURN(CRectF::NewByFriend((CRectF**)&deviceR));
        Boolean result = FALSE;
        FAIL_RETURN(m->MapRect((IRectF*)deviceR.Get(), (IRectF*)dstR.Get(), &result));

        Float w = 0, h = 0;
        deviceR->GetWidth(&w);
        deviceR->GetHeight(&h);
        neww = Elastos::Core::Math::Round(w);
        newh = Elastos::Core::Math::Round(h);

        Boolean hasAlpha = FALSE;
        source->HasAlpha(&hasAlpha);
        FAIL_RETURN(CreateBitmap(neww, newh, transformed ? BitmapConfig_ARGB_8888 : newConfig,
                transformed || hasAlpha, (IBitmap**)&bmp));

        canvas->Translate(-deviceR->mLeft, -deviceR->mTop);
        canvas->Concat(m);

        FAIL_RETURN(CPaint::NewByFriend((CPaint**)&paint));
        paint->SetFilterBitmap(filter);
        if (transformed) {
            paint->SetAntiAlias(TRUE);
        }
    }

    // The new bitmap was created from a known bitmap source so assume that
    // they use the same density
    Int32 density;
    source->GetDensity(&density);
    bmp->SetDensity(density);

    canvas->SetBitmap(bmp);
    FAIL_RETURN(canvas->DrawBitmap(source,
        (IRect*)srcR.Get(), (IRectF*)dstR.Get(), (IPaint*)paint.Get()));
    canvas->SetBitmap(NULL);

    *bitmap = (IBitmap*)bmp.Get();
    REFCOUNT_ADD(*bitmap);
    return NOERROR;
}

ECode CBitmap::CreateBitmap(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ BitmapConfig config,
    /* [out] */ IBitmap** bitmap)
{
    VALIDATE_NOT_NULL(bitmap);
    return CreateBitmap(width, height, config, TRUE, bitmap);
}

ECode CBitmap::CreateBitmap(
    /* [in] */ IDisplayMetrics* display,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ BitmapConfig config,
    /* [out] */ IBitmap** bitmap)
{
    VALIDATE_NOT_NULL(bitmap);
    return CreateBitmap(display, width, height, config, TRUE, bitmap);
}

ECode CBitmap::CreateBitmap(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ BitmapConfig config,
    /* [in] */ Boolean hasAlpha,
    /* [out] */ IBitmap** bitmap)
{
    VALIDATE_NOT_NULL(bitmap);
    return CreateBitmap(NULL, width, height, config, hasAlpha, bitmap);
}

ECode CBitmap::CreateBitmap(
    /* [in] */ IDisplayMetrics* display,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ BitmapConfig config,
    /* [in] */ Boolean hasAlpha,
    /* [out] */ IBitmap** bitmap)
{
    VALIDATE_NOT_NULL(bitmap);

     if (width <= 0 || height <= 0) {
        // throw new IllegalArgumentException("width and height must be > 0");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    AutoPtr<CBitmap> bm;
    FAIL_RETURN(NativeCreate(NULL, 0, width, width, height, config, TRUE, (CBitmap**)&bm));
    if (display != NULL) {
        Int32 densityDpi;
        display->GetDensityDpi(&densityDpi);
        bm->mDensity = densityDpi;
    }
    if (config == BitmapConfig_ARGB_8888 && !hasAlpha) {
        NativeErase(bm->mNativeBitmap, 0xff000000);
        NativeSetHasAlpha(bm->mNativeBitmap, hasAlpha);
    }
    else {
        // No need to initialize it to zeroes; it is backed by a VM byte array
        // which is by definition preinitialized to all zeroes.
        //
        //nativeErase(bm.mNativeBitmap, 0);
    }
    *bitmap = (IBitmap*)bm.Get();
    REFCOUNT_ADD(*bitmap);
    return NOERROR;
}

ECode CBitmap::CreateBitmap(
    /* [in] */ const ArrayOf<Int32>& colors,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 stride,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ BitmapConfig config,
    /* [out] */ IBitmap** bitmap)
{
    VALIDATE_NOT_NULL(bitmap);
    return CreateBitmap(NULL, colors, offset, stride, width, height, config, bitmap);
}

ECode CBitmap::CreateBitmap(
    /* [in] */ IDisplayMetrics* display,
    /* [in] */ const ArrayOf<Int32>& colors,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 stride,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ BitmapConfig config,
    /* [out] */ IBitmap** bitmap)
{
    VALIDATE_NOT_NULL(bitmap);

    FAIL_RETURN(CheckWidthHeight(width, height));
    if (Elastos::Core::Math::Abs(stride) < width) {
        // throw new IllegalArgumentException("abs(stride) must be >= width");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Int32 lastScanline = offset + (height - 1) * stride;
    Int32 length = colors.GetLength();
    if (offset < 0 || (offset + width > length) || lastScanline < 0 ||
            (lastScanline + width > length)) {
        // throw new ArrayIndexOutOfBoundsException();
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    if (width <= 0 || height <= 0) {
        // throw new IllegalArgumentException("width and height must be > 0");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    AutoPtr<CBitmap> bm;
    FAIL_RETURN(NativeCreate(&colors, offset, stride, width, height,
            config, FALSE, (CBitmap**)&bm));
    if (display != NULL) {
        Int32 densityDpi;
        display->GetDensityDpi(&densityDpi);
        bm->mDensity = densityDpi;
    }
    *bitmap = (IBitmap*)bm.Get();
    REFCOUNT_ADD(*bitmap);
    return NOERROR;
}

ECode CBitmap::CreateBitmap(
    /* [in] */ const ArrayOf<Int32>& colors,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ BitmapConfig config,
    /* [out] */ IBitmap** bitmap)
{
    VALIDATE_NOT_NULL(bitmap);
    return CreateBitmap(NULL, colors, 0, width, width, height, config, bitmap);
}

ECode CBitmap::CreateBitmap(
    /* [in] */ IDisplayMetrics* display,
    /* [in] */ const ArrayOf<Int32>& colors,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ BitmapConfig config,
    /* [out] */ IBitmap** bitmap)
{
    VALIDATE_NOT_NULL(bitmap);
    return CreateBitmap(display, colors, 0, width, width, height, config, bitmap);
}

ECode CBitmap::GetNinePatchChunk(
    /* [out, callee] */ ArrayOf<Byte>** data)
{
    VALIDATE_NOT_NULL(data);
    *data = mNinePatchChunk;
    REFCOUNT_ADD(*data);
    return NOERROR;
}

ECode CBitmap::GetLayoutBounds(
    /* [out, callee] */ ArrayOf<Int32>** bounds)
{
    VALIDATE_NOT_NULL(bounds);
    *bounds = mLayoutBounds;
    REFCOUNT_ADD(*bounds);
    return NOERROR;
}

ECode CBitmap::Compress(
    /* [in] */ BitmapCompressFormat format,
    /* [in] */ Int32 quality,
    /* [in] */ IOutputStream* stream,
    /* [out] */ Boolean* isSucceeded)
{
    VALIDATE_NOT_NULL(isSucceeded);

    FAIL_RETURN(CheckRecycled("Can't compress a recycled bitmap"));
    // do explicit check before calling the native method
    if (stream == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }
    if (quality < 0 || quality > 100) {
        Logger::E(TAG, "quality must be 0..100");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    AutoPtr< ArrayOf<Byte> > arr = ArrayOf<Byte>::Alloc(WORKING_COMPRESS_STORAGE);
    *isSucceeded = NativeCompress(
        mNativeBitmap, (Int32)format, quality, stream, arr.Get());
    return NOERROR;
}

ECode CBitmap::IsMutable(
    /* [out] */ Boolean* isMutable)
{
    VALIDATE_NOT_NULL(isMutable);

    *isMutable = mIsMutable;
    return NOERROR;
}

ECode CBitmap::IsPremultiplied(
    /* [out] */ Boolean* isPremultiplied)
{
    VALIDATE_NOT_NULL(isPremultiplied);

    BitmapConfig config;
    GetConfig(&config);
    Boolean hasAlpha;
    HasAlpha(&hasAlpha);
    *isPremultiplied = config != BitmapConfig_RGB_565 && hasAlpha;
    return NOERROR;
}

ECode CBitmap::GetWidth(
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);

    *width = mWidth == -1 ? (mWidth = NativeWidth(mNativeBitmap)) : mWidth;
    return NOERROR;
}

ECode CBitmap::GetHeight(
    /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height);

    *height = mHeight == -1 ? (mHeight = NativeHeight(mNativeBitmap)) : mHeight;
    return NOERROR;
}

ECode CBitmap::GetScaledWidth(
    /* [in] */ ICanvas* canvas,
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);

    Int32 w = 0;
    GetWidth(&w);
    Int32 density;
    canvas->GetDensity(&density);
    *width = ScaleFromDensity(w, mDensity, density);
    return NOERROR;
}

ECode CBitmap::GetScaledHeight(
    /* [in] */ ICanvas* canvas,
    /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height);

    Int32 h = 0;
    GetWidth(&h);
    Int32 density;
    canvas->GetDensity(&density);
    *height = ScaleFromDensity(h, mDensity, density);
    return NOERROR;
}

ECode CBitmap::GetScaledWidth(
    /* [in] */ IDisplayMetrics* metrics,
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);

    Int32 w = 0;
    GetWidth(&w);
    Int32 densityDpi;
    metrics->GetDensityDpi(&densityDpi);
    *width = ScaleFromDensity(w, mDensity, densityDpi);
    return NOERROR;
}

ECode CBitmap::GetScaledHeight(
    /* [in] */ IDisplayMetrics* metrics,
    /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height);

    Int32 h = 0;
    GetWidth(&h);
    Int32 densityDpi;
    metrics->GetDensityDpi(&densityDpi);
    *height = ScaleFromDensity(h, mDensity, densityDpi);
    return NOERROR;
}

ECode CBitmap::GetScaledWidth(
    /* [in] */ Int32 targetDensity,
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);

    Int32 w = 0;
    GetWidth(&w);
    *width = ScaleFromDensity(w, mDensity, targetDensity);
    return NOERROR;
}

ECode CBitmap::GetScaledHeight(
    /* [in] */ Int32 targetDensity,
    /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height);

    Int32 h = 0;
    GetHeight(&h);
    *height = ScaleFromDensity(h, mDensity, targetDensity);
    return NOERROR;
}

Int32 CBitmap::ScaleFromDensity(
    /* [in] */ Int32 size,
    /* [in] */ Int32 sdensity,
    /* [in] */ Int32 tdensity)
{
    if (sdensity == DENSITY_NONE || tdensity == DENSITY_NONE || sdensity == tdensity) {
        return size;
    }

    // Scale by tdensity / sdensity, rounding up.
    return ( (size * tdensity) + (sdensity >> 1) ) / sdensity;
}

ECode CBitmap::GetRowBytes(
    /* [out] */ Int32* bytes)
{
    VALIDATE_NOT_NULL(bytes);

    *bytes = NativeRowBytes(mNativeBitmap);
    return NOERROR;
}

ECode CBitmap::GetByteCount(
    /* [out] */ Int32* bytes)
{
    VALIDATE_NOT_NULL(bytes);

    // int result permits bitmaps up to 46,340 x 46,340
    Int32 rowBytes = 0;
    Int32 height = 0;
    GetRowBytes(&rowBytes);
    GetHeight(&height);
    *bytes =rowBytes * height;
    return NOERROR;
}

ECode CBitmap::GetConfig(
    /* [out] */ BitmapConfig* config)
{
    VALIDATE_NOT_NULL(config);

    *config = NativeToConfig(NativeConfig(mNativeBitmap));
    return NOERROR;
}

ECode CBitmap::HasAlpha(
    /* [out] */ Boolean* isAlpha)
{
    VALIDATE_NOT_NULL(isAlpha);

    *isAlpha = NativeHasAlpha(mNativeBitmap);
    return NOERROR;
}

ECode CBitmap::SetHasAlpha(
    /* [in] */ Boolean hasAlpha)
{
    NativeSetHasAlpha(mNativeBitmap, hasAlpha);
    return NOERROR;
}

ECode CBitmap::HasMipMap(
    /* [out] */ Boolean* hasMipMap)
{
    VALIDATE_NOT_NULL(hasMipMap);

    *hasMipMap = NativeHasMipMap(mNativeBitmap);
    return NOERROR;
}

ECode CBitmap::SetHasMipMap(
    /* [in] */ Boolean hasMipMap)
{
    NativeSetHasMipMap(mNativeBitmap, hasMipMap);
    return NOERROR;
}

ECode CBitmap::EraseColor(
    /* [in] */ Int32 c)
{
    FAIL_RETURN(CheckRecycled("Can't erase a recycled bitmap"));
    Boolean isMutable;
    if (IsMutable(&isMutable), !isMutable) {
        Logger::E(TAG, "cannot erase immutable bitmaps");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    NativeErase(mNativeBitmap, c);
    return NOERROR;
}

ECode CBitmap::GetPixel(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [out] */ Int32* pixel)
{
    VALIDATE_NOT_NULL(pixel);

    FAIL_RETURN(CheckRecycled("Can't call getPixel() on a recycled bitmap"));
    FAIL_RETURN(CheckPixelAccess(x, y));
    *pixel = NativeGetPixel(mNativeBitmap, x, y);
    return NOERROR;
}

ECode CBitmap::GetPixels(
    /* [in] */ const ArrayOf<Int32>& pixels,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 stride,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    FAIL_RETURN(CheckRecycled("Can't call getPixels() on a recycled bitmap"));
    if (width == 0 || height == 0) {
        return NOERROR; // nothing to do
    }
    FAIL_RETURN(CheckPixelsAccess(x, y, width, height, offset, stride, pixels));
    NativeGetPixels(mNativeBitmap, pixels, offset, stride, x, y, width, height);
    return NOERROR;
}

ECode CBitmap::CheckPixelAccess(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    FAIL_RETURN(CheckXYSign(x, y));
    Int32 w = 0, h = 0;
    GetWidth(&w);
    GetHeight(&h);
    if (x >= w) {
        Logger::E(TAG, "x must be < bitmap.width()");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (y >= h) {
        Logger::E(TAG, "y must be < bitmap.height()");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

ECode CBitmap::CheckPixelsAccess(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 stride,
    /* [in] */ const ArrayOf<Int32>& pixels)
{
    FAIL_RETURN(CheckXYSign(x, y));
    if (width < 0) {
        // throw new IllegalArgumentException("width must be >= 0");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (height < 0) {
        // throw new IllegalArgumentException("height must be >= 0");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Int32 w = 0, h = 0;
    GetWidth(&w);
    GetHeight(&h);
    if (x + width > w) {
        Logger::E(TAG, "x + width must be <= bitmap.width()");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (y + height > h) {
        Logger::E(TAG, "y + height must be <= bitmap.height()");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (Elastos::Core::Math::Abs(stride) < width) {
        Logger::E(TAG, "abs(stride) must be >= width");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Int32 lastScanline = offset + (height - 1) * stride;
    Int32 length = pixels.GetLength();
    if (offset < 0 || (offset + width > length)
            || lastScanline < 0
            || (lastScanline + width > length)) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    return NOERROR;
}

ECode CBitmap::SetPixel(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 color)
{
    FAIL_RETURN(CheckRecycled("Can't call setPixel() on a recycled bitmap"));
    Boolean isMutable;
    if (IsMutable(&isMutable), !isMutable) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    FAIL_RETURN(CheckPixelAccess(x, y));
    NativeSetPixel(mNativeBitmap, x, y, color);
    return NOERROR;
}

ECode CBitmap::SetPixels(
    /* [in] */ const ArrayOf<Int32>& pixels,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 stride,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    FAIL_RETURN(CheckRecycled("Can't call setPixels() on a recycled bitmap"));
    Boolean isMutable;
    if (IsMutable(&isMutable), !isMutable) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    if (width == 0 || height == 0) {
        return NOERROR; // nothing to do
    }
    FAIL_RETURN(CheckPixelsAccess(x, y, width, height, offset, stride, pixels));
    NativeSetPixels(mNativeBitmap, pixels, offset, stride, x, y, width, height);
    return NOERROR;
}

ECode CBitmap::GetNativeBitmap(
    /* [out] */ Handle32* bitmap)
{
    VALIDATE_NOT_NULL(bitmap);
    *bitmap = mNativeBitmap;
    return NOERROR;
}

ECode CBitmap::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    return NativeCreateFromParcel(source, this);
}

ECode CBitmap::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    FAIL_RETURN(CheckRecycled("Can't parcel a recycled bitmap"));
    if (!NativeWriteToParcel(mNativeBitmap, mIsMutable, mDensity, dest)) {
        // throw new RuntimeException("native writeToParcel failed");
        return E_RUNTIME_EXCEPTION;
    }
    return NOERROR;
}

ECode CBitmap::ExtractAlpha(
    /* [out] */ IBitmap** bitmap)
{
    VALIDATE_NOT_NULL(bitmap);

    return ExtractAlpha(NULL, NULL, bitmap);
}

ECode CBitmap::ExtractAlpha(
    /* [in] */ IPaint* paint,
    /* [in] */ ArrayOf<Int32>* offsetXY,
    /* [out] */ IBitmap** bitmap)
{
    VALIDATE_NOT_NULL(bitmap);

    FAIL_RETURN(CheckRecycled("Can't extractAlpha on a recycled bitmap"));
    Int32 nativePaint = paint != NULL ? ((Paint*)paint->Probe(EIID_Paint))->mNativePaint : 0;
    AutoPtr<CBitmap> bm;
    ECode ec = NativeExtractAlpha(mNativeBitmap, nativePaint, offsetXY, (CBitmap**)&bm);
    if (FAILED(ec)) {
        Logger::E(TAG, "Failed to extractAlpha on Bitmap");
        return E_RUNTIME_EXCEPTION;
    }
    bm->mDensity = mDensity;
    *bitmap = (IBitmap*)bm.Get();
    REFCOUNT_ADD(*bitmap);
    return NOERROR;
}

ECode CBitmap::SameAs(
    /* [in] */ IBitmap* other,
    /* [out] */ Boolean* isSame)
{
    VALIDATE_NOT_NULL(isSame);

    *isSame = (IBitmap*)this == other || (other != NULL
            && NativeSameAs(mNativeBitmap, ((CBitmap*)other)->mNativeBitmap));
    return NOERROR;
}

ECode CBitmap::PrepareToDraw()
{
    NativePrepareToDraw(mNativeBitmap);
    return NOERROR;
}

///////////////////////////////////////////////////////////////////////////////
// Conversions to/from SkColor, for get/setPixels, and the create method, which
// is basically like setPixels

typedef void (*FromColorProc)(void* dst, const SkColor src[],
        int width, int x, int y);

static void FromColor_D32(void* dst, const SkColor src[],
        int width, int, int)
{
    SkPMColor* d = (SkPMColor*)dst;

    for (int i = 0; i < width; i++) {
        *d++ = SkPreMultiplyColor(*src++);
    }
}

static void FromColor_D565(void* dst, const SkColor src[],
        int width, int x, int y)
{
    uint16_t* d = (uint16_t*)dst;

    DITHER_565_SCAN(y);
    for (int stop = x + width; x < stop; x++) {
        SkColor c = *src++;
        *d++ = SkDitherRGBTo565(SkColorGetR(c), SkColorGetG(c), SkColorGetB(c),
                                DITHER_VALUE(x));
    }
}

static void FromColor_D4444(void* dst, const SkColor src[],
        int width, int x, int y)
{
    SkPMColor16* d = (SkPMColor16*)dst;

    DITHER_4444_SCAN(y);
    for (int stop = x + width; x < stop; x++) {
        SkPMColor c = SkPreMultiplyColor(*src++);
        *d++ = SkDitherARGB32To4444(c, DITHER_VALUE(x));
//        *d++ = SkPixel32ToPixel4444(c);
    }
}

// can return NULL
static FromColorProc ChooseFromColorProc(SkBitmap::Config config)
{
    switch (config) {
        case SkBitmap::kARGB_8888_Config:
            return FromColor_D32;
        case SkBitmap::kARGB_4444_Config:
            return FromColor_D4444;
        case SkBitmap::kRGB_565_Config:
            return FromColor_D565;
        default:
            break;
    }
    return NULL;
}

//////////////////// ToColor procs /////////////////////

typedef void (*ToColorProc)(SkColor dst[], const void* src, int width,
                            SkColorTable*);

static void ToColor_S32_Alpha(SkColor dst[], const void* src, int width,
                              SkColorTable*)
{
    SkASSERT(width > 0);
    const SkPMColor* s = (const SkPMColor*)src;
    do {
        *dst++ = SkUnPreMultiply::PMColorToColor(*s++);
    } while (--width != 0);
}

static void ToColor_S32_Opaque(SkColor dst[], const void* src, int width,
                               SkColorTable*)
{
    SkASSERT(width > 0);
    const SkPMColor* s = (const SkPMColor*)src;
    do {
        SkPMColor c = *s++;
        *dst++ = SkColorSetRGB(SkGetPackedR32(c), SkGetPackedG32(c),
                               SkGetPackedB32(c));
    } while (--width != 0);
}

static void ToColor_S4444_Alpha(SkColor dst[], const void* src, int width,
                                SkColorTable*)
{
    SkASSERT(width > 0);
    const SkPMColor16* s = (const SkPMColor16*)src;
    do {
        *dst++ = SkUnPreMultiply::PMColorToColor(SkPixel4444ToPixel32(*s++));
    } while (--width != 0);
}

static void ToColor_S4444_Opaque(SkColor dst[], const void* src, int width,
                                 SkColorTable*)
{
    SkASSERT(width > 0);
    const SkPMColor* s = (const SkPMColor*)src;
    do {
        SkPMColor c = SkPixel4444ToPixel32(*s++);
        *dst++ = SkColorSetRGB(SkGetPackedR32(c), SkGetPackedG32(c),
                               SkGetPackedB32(c));
    } while (--width != 0);
}

static void ToColor_S565(SkColor dst[], const void* src, int width,
                         SkColorTable*)
{
    SkASSERT(width > 0);
    const uint16_t* s = (const uint16_t*)src;
    do {
        uint16_t c = *s++;
        *dst++ =  SkColorSetRGB(SkPacked16ToR32(c), SkPacked16ToG32(c),
                                SkPacked16ToB32(c));
    } while (--width != 0);
}

static void ToColor_SI8_Alpha(SkColor dst[], const void* src, int width,
                              SkColorTable* ctable)
{
    SkASSERT(width > 0);
    const uint8_t* s = (const uint8_t*)src;
    const SkPMColor* colors = ctable->lockColors();
    do {
        *dst++ = SkUnPreMultiply::PMColorToColor(colors[*s++]);
    } while (--width != 0);
    ctable->unlockColors(FALSE);
}

static void ToColor_SI8_Opaque(SkColor dst[], const void* src, int width,
                               SkColorTable* ctable)
{
    SkASSERT(width > 0);
    const uint8_t* s = (const uint8_t*)src;
    const SkPMColor* colors = ctable->lockColors();
    do {
        SkPMColor c = colors[*s++];
        *dst++ = SkColorSetRGB(SkGetPackedR32(c), SkGetPackedG32(c),
                               SkGetPackedB32(c));
    } while (--width != 0);
    ctable->unlockColors(FALSE);
}

// can return NULL
static ToColorProc ChooseToColorProc(const SkBitmap& src)
{
    switch (src.config()) {
        case SkBitmap::kARGB_8888_Config:
            return src.isOpaque() ? ToColor_S32_Opaque : ToColor_S32_Alpha;
        case SkBitmap::kARGB_4444_Config:
            return src.isOpaque() ? ToColor_S4444_Opaque : ToColor_S4444_Alpha;
        case SkBitmap::kRGB_565_Config:
            return ToColor_S565;
        case SkBitmap::kIndex8_Config:
            if (src.getColorTable() == NULL) {
                return NULL;
            }
            return src.isOpaque() ? ToColor_SI8_Opaque : ToColor_SI8_Alpha;
        default:
            break;
    }
    return NULL;
}

ECode CBitmap::NativeCreate(
    /* [in] */ const ArrayOf<Int32>* colors,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 stride,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 nativeConfig,
    /* [in] */ Boolean isMutable,
    /* [out] */ CBitmap** bitmap)
{
    VALIDATE_NOT_NULL(bitmap);

    if (colors != NULL) {
        Int32 n = colors->GetLength();
        if (n < (Int32)SkAbs32(stride) * height) {
            *bitmap = NULL;
            return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
        }
    }

    SkBitmap nativeBitmap;

    nativeBitmap.setConfig((SkBitmap::Config)nativeConfig, width, height);

    AutoPtr< ArrayOf<Byte> > buff;
    FAIL_RETURN(GraphicsNative::AllocateDroidPixelRef(&nativeBitmap, NULL, (ArrayOf<Byte>**)&buff));
    if (buff == NULL) {
        *bitmap = NULL;
        return NOERROR;
    }

    if (colors != NULL) {
        FAIL_RETURN(GraphicsNative::SetPixels(*colors, offset, stride, 0, 0, width, height, nativeBitmap));
    }

    return GraphicsNative::CreateBitmap(new SkBitmap(nativeBitmap), buff, isMutable, NULL, NULL, -1, bitmap);
}

ECode CBitmap::NativeCopy(
    /* [in] */ Int32 srcBitmap,
    /* [in] */ Int32 nativeConfig,
    /* [in] */ Boolean isMutable,
    /* [out] */ CBitmap** bitmap)
{
    SkBitmap result;
    GraphicsNative::DroidPixelAllocator allocator;

    if (!((SkBitmap*)srcBitmap)->copyTo(&result, (SkBitmap::Config)nativeConfig, &allocator)) {
        *bitmap = NULL;
        return NOERROR;
    }

    return GraphicsNative::CreateBitmap(new SkBitmap(result), allocator.getStorageObj(),
            isMutable, NULL, NULL, -1, bitmap);
}

void CBitmap::NativeDestructor(
    /* [in] */ Int32 nativeBitmap)
{
#ifdef USE_OPENGL_RENDERER
    if (android::uirenderer::Caches::hasInstance()) {
        android::uirenderer::Caches::getInstance().resourceCache.destructor((SkBitmap*)nativeBitmap);
        return;
    }
#endif // USE_OPENGL_RENDERER
    delete (SkBitmap*)nativeBitmap;
}

Boolean CBitmap::NativeRecycle(
    /* [in] */ Int32 nativeBitmap)
{
#ifdef USE_OPENGL_RENDERER
    if (android::uirenderer::Caches::hasInstance()) {
        return android::uirenderer::Caches::getInstance().resourceCache.recycle((SkBitmap*)nativeBitmap);
    }
#endif // USE_OPENGL_RENDERER
    ((SkBitmap*)nativeBitmap)->setPixels(NULL, NULL);
    return TRUE;
}

// These must match the int values in Bitmap.java
enum ElEncodeFormat {
    kJPEG_ElEncodeFormat = 0,
    kPNG_ElEncodeFormat = 1,
    kWEBP_ElEncodeFormat = 2
};

Boolean CBitmap::NativeCompress(
    /* [in] */ Int32 nativeBitmap,
    /* [in] */ Int32 format,
    /* [in] */ Int32 quality,
    /* [in] */ IOutputStream* stream,
    /* [in] */ ArrayOf<Byte>* tempStorage)
{
    SkBitmap* bitmap = (SkBitmap*)nativeBitmap;

    SkImageEncoder::Type fm;

    switch (format) {
    case kJPEG_ElEncodeFormat:
        fm = SkImageEncoder::kJPEG_Type;
        break;
    case kPNG_ElEncodeFormat:
        fm = SkImageEncoder::kPNG_Type;
        break;
    case kWEBP_ElEncodeFormat:
        fm = SkImageEncoder::kWEBP_Type;
        break;
    default:
        return FALSE;
    }

    Boolean success = FALSE;
    if (NULL != bitmap) {
        SkAutoLockPixels alp(*bitmap);

        if (NULL == bitmap->getPixels()) {
            return FALSE;
        }

        SkWStream* strm = CreateOutputStreamAdaptor(stream, tempStorage);
        if (NULL == strm) {
            return FALSE;
        }

        SkImageEncoder* encoder = SkImageEncoder::Create(fm);
        if (NULL != encoder) {
            success = encoder->encodeStream(strm, *bitmap, quality);
            delete encoder;
        }
        delete strm;
    }
    return success;
}

void CBitmap::NativeErase(
    /* [in] */ Int32 nativeBitmap,
    /* [in] */ Int32 color)
{
    ((SkBitmap*)nativeBitmap)->eraseColor(color);
}

Int32 CBitmap::NativeWidth(
    /* [in] */ Int32 nativeBitmap)
{
    return ((SkBitmap*)nativeBitmap)->width();
}

Int32 CBitmap::NativeHeight(
    /* [in] */ Int32 nativeBitmap)
{
    return ((SkBitmap*)nativeBitmap)->height();
}

Int32 CBitmap::NativeRowBytes(
    /* [in] */ Int32 nativeBitmap)
{
    return ((SkBitmap*)nativeBitmap)->rowBytes();
}

Int32 CBitmap::NativeConfig(
    /* [in] */ Int32 nativeBitmap)
{
    return ((SkBitmap*)nativeBitmap)->config();
}

Int32 CBitmap::NativeGetPixel(
    /* [in] */ Int32 nativeBitmap,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    assert(nativeBitmap);
    SkBitmap* bitmap = (SkBitmap*)nativeBitmap;

    SkAutoLockPixels alp(*bitmap);

    ToColorProc proc = ChooseToColorProc(*bitmap);
    if (NULL == proc) {
        return 0;
    }
    const void* src = bitmap->getAddr(x, y);
    if (NULL == src) {
        return 0;
    }

    SkColor dst[1];
    proc(dst, src, 1, bitmap->getColorTable());
    return dst[0];
}

void CBitmap::NativeGetPixels(
    /* [in] */ Int32 nativeBitmap,
    /* [in] */ const ArrayOf<Int32>& pixels,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 stride,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    assert(nativeBitmap);
    SkBitmap* bitmap = (SkBitmap*)nativeBitmap;

    SkAutoLockPixels alp(*bitmap);

    ToColorProc proc = ChooseToColorProc(*bitmap);
    if (NULL == proc) {
        return;
    }
    const void* src = bitmap->getAddr(x, y);
    if (NULL == src) {
        return;
    }

    SkColorTable* ctable = bitmap->getColorTable();
    Int32* dst = pixels.GetPayload();
    SkColor* d = (SkColor*)dst + offset;
    while (--height >= 0) {
        proc(d, src, width, ctable);
        d += stride;
        src = (void*)((const char*)src + bitmap->rowBytes());
    }
}

void CBitmap::NativeSetPixel(
    /* [in] */ Int32 nativeBitmap,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 color)
{
    assert(nativeBitmap);
    SkBitmap* bitmap = (SkBitmap*)nativeBitmap;

    SkAutoLockPixels alp(*bitmap);
    if (NULL == bitmap->getPixels()) {
        return;
    }

    FromColorProc proc = ChooseFromColorProc(bitmap->config());
    if (NULL == proc) {
        return;
    }

    proc(bitmap->getAddr(x, y), (SkColor*)&color, 1, x, y);
    bitmap->notifyPixelsChanged();
}

void CBitmap::NativeSetPixels(
    /* [in] */ Int32 nativeBitmap,
    /* [in] */ const ArrayOf<Int32>& colors,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 stride,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    assert(nativeBitmap);
    SkBitmap* bitmap = (SkBitmap*)nativeBitmap;
    GraphicsNative::SetPixels(colors, offset, stride, x, y, width, height, *bitmap);
}

void CBitmap::NativeCopyPixelsToBuffer(
    /* [in] */ Int32 nativeBitmap,
    /* [in] */ IBuffer* dst)
{
    assert(nativeBitmap);
    SkBitmap* bitmap = (SkBitmap*)nativeBitmap;

    SkAutoLockPixels alp(*bitmap);
    const void* src = bitmap->getPixels();

    if (NULL != src) {
        AutoBufferPointer abp(dst, TRUE);
        // the java side has already checked that buffer is large enough
        memcpy(abp.Pointer(), src, bitmap->getSize());
    }
}

void CBitmap::NativeCopyPixelsFromBuffer(
    /* [in] */ Int32 nativeBitmap,
    /* [in] */ IBuffer* src)
{
    assert(nativeBitmap);
    SkBitmap* bitmap = (SkBitmap*)nativeBitmap;

    SkAutoLockPixels alp(*bitmap);
    void* dst = bitmap->getPixels();

    if (NULL != dst) {
        AutoBufferPointer abp(src, FALSE);
        // the java side has already checked that buffer is large enough
        memcpy(dst, abp.Pointer(), bitmap->getSize());
        bitmap->notifyPixelsChanged();
    }
}

Int32 CBitmap::NativeGenerationId(
    /* [in] */ Int32 nativeBitmap)
{
    return ((SkBitmap*)nativeBitmap)->getGenerationID();
}

ECode CBitmap::NativeCreateFromParcel(
    /* [in] */ IParcel* p,
    /* [in] */ CBitmap* thisObj)
{
    Int32 value = 0;
    p->ReadInt32(&value);
    const Boolean isMutable = value != 0;
    p->ReadInt32(&value);
    const SkBitmap::Config  config = (SkBitmap::Config)value;
    Int32 width = 0;
    p->ReadInt32(&width);
    Int32 height = 0;
    p->ReadInt32(&height);
    Int32 rowBytes = 0;
    p->ReadInt32(&rowBytes);
    Int32 density = 0;
    p->ReadInt32(&density);

    if (SkBitmap::kARGB_8888_Config != config &&
            SkBitmap::kRGB_565_Config != config &&
            SkBitmap::kARGB_4444_Config != config &&
            SkBitmap::kIndex8_Config != config &&
            SkBitmap::kA8_Config != config) {
        Logger::E(TAG, "Bitmap_createFromParcel unknown config: %d\n", config);
        return E_RUNTIME_EXCEPTION;
    }

    SkBitmap* bmp = new SkBitmap;

    bmp->setConfig(config, width, height, rowBytes);

    SkColorTable* ctable = NULL;
    if (config == SkBitmap::kIndex8_Config) {
        Int32 count;
        p->ReadInt32(&count);
        if (count > 0) {
            AutoPtr< ArrayOf<Int32> > array;
            p->ReadArrayOf((Handle32*)&array);
            assert(count == array->GetLength());
            const SkPMColor* src = (const SkPMColor*)array->GetPayload();
            ctable = new SkColorTable(src, count);
        }
    }

    AutoPtr< ArrayOf<Byte> > buffer;
    FAIL_RETURN(GraphicsNative::AllocateDroidPixelRef(bmp, ctable, (ArrayOf<Byte>**)&buffer));
    if (buffer == NULL) {
        SkSafeUnref(ctable);
        delete bmp;
        return E_RUNTIME_EXCEPTION;
    }

    SkSafeUnref(ctable);

    size_t size = bmp->getSize();

    AutoPtr< ArrayOf<Byte> > array;
    p->ReadArrayOf((Handle32*)&array);
    assert(size == (size_t)array->GetLength());

    bmp->lockPixels();
    memcpy(bmp->getPixels(), (void*)array->GetPayload(), size);
    bmp->unlockPixels();

    return thisObj->constructor((Int32)bmp, buffer, isMutable, NULL, NULL, density);
}

Boolean CBitmap::NativeWriteToParcel(
    /* [in] */ Int32 nativeBitmap,
    /* [in] */ Boolean isMutable,
    /* [in] */ Int32 density,
    /* [in] */ IParcel* p)
{
    assert(nativeBitmap);
    SkBitmap* bitmap = (SkBitmap*)nativeBitmap;

    p->WriteInt32(isMutable);
    p->WriteInt32(bitmap->config());
    p->WriteInt32(bitmap->width());
    p->WriteInt32(bitmap->height());
    p->WriteInt32(bitmap->rowBytes());
    p->WriteInt32(density);

    if (bitmap->getConfig() == SkBitmap::kIndex8_Config) {
        SkColorTable* ctable = bitmap->getColorTable();
        if (ctable != NULL) {
            Int32 count = (Int32)ctable->count();
            p->WriteInt32(count);
            ArrayOf<Int32> array((Int32*)ctable->lockColors(), count);
            p->WriteArrayOf((Handle32)&array);
            ctable->unlockColors(FALSE);
        }
        else {
            p->WriteInt32(0); // indicate no ctable
        }
    }

    size_t size = bitmap->getSize();

    bitmap->lockPixels();
    AutoPtr< ArrayOf<Byte> > array = ArrayOf<Byte>::Alloc(size);
    const void* pSrc =  bitmap->getPixels();
    if (pSrc == NULL) {
        memset(array->GetPayload(), 0, size);
    } else {
        memcpy(array->GetPayload(), pSrc, size);
    }
    bitmap->unlockPixels();
    p->WriteArrayOf((Handle32)array.Get());
    return TRUE;
}

ECode CBitmap::NativeExtractAlpha(
    /* [in] */ Int32 nativeBitmap,
    /* [in] */ Int32 nativePaint,
    /* [in] */ ArrayOf<Int32>* offsetXY,
    /* [out] */ CBitmap** bitmap)
{
    assert(nativeBitmap);
    SkBitmap* src = (SkBitmap*)nativeBitmap;

    SkIPoint  offset;
    SkBitmap* dst = new SkBitmap;
    GraphicsNative::DroidPixelAllocator allocator;

    src->extractAlpha(dst, (SkPaint*)nativePaint, &allocator, &offset);
    // If Skia can't allocate pixels for destination bitmap, it resets
    // it, that is set its pixels buffer to NULL, and zero width and height.
    if (dst->getPixels() == NULL && src->getPixels() != NULL) {
        delete dst;
        // doThrowOOME(env, "failed to allocate pixels for alpha");
        *bitmap = NULL;
        return E_OUT_OF_MEMORY_ERROR;
    }
    if (offsetXY != 0 && offsetXY->GetLength()>= 2) {
        Int32* array = offsetXY->GetPayload();
        array[0] = offset.fX;
        array[1] = offset.fY;
    }

    return GraphicsNative::CreateBitmap(dst, allocator.getStorageObj(), TRUE, NULL, NULL, -1, bitmap);
}

void CBitmap::NativePrepareToDraw(
    /* [in] */ Int32 nativeBitmap)
{
    assert(nativeBitmap);
    ((SkBitmap*)nativeBitmap)->lockPixels();
    ((SkBitmap*)nativeBitmap)->unlockPixels();
}

Boolean CBitmap::NativeHasAlpha(
    /* [in] */ Int32 nativeBitmap)
{
    assert(nativeBitmap);
    return (Boolean)!((SkBitmap*)nativeBitmap)->isOpaque();
}

void CBitmap::NativeSetHasAlpha(
    /* [in] */ Int32 nativeBitmap,
    /* [in] */ Boolean hasAlpha)
{
    assert(nativeBitmap);
    ((SkBitmap*)nativeBitmap)->setIsOpaque(!hasAlpha);
}

Boolean CBitmap::NativeHasMipMap(
    /* [in] */ Int32 nativeBitmap)
{
    assert(nativeBitmap);
    return ((SkBitmap*)nativeBitmap)->hasHardwareMipMap();
}

void CBitmap::NativeSetHasMipMap(
        /* [in] */ Int32 nativeBitmap,
        /* [in] */ Boolean hasMipMap)
{
    assert(nativeBitmap);
    ((SkBitmap*)nativeBitmap)->setHasHardwareMipMap(hasMipMap);
}

Boolean CBitmap::NativeSameAs(
    /* [in] */ Int32 nb0,
    /* [in] */ Int32 nb1)
{
    assert(nb0);
    assert(nb1);
    SkBitmap* bm0 = (SkBitmap*)nb0;
    SkBitmap* bm1 = (SkBitmap*)nb1;

    if (bm0->width() != bm1->width() ||
        bm0->height() != bm1->height() ||
        bm0->config() != bm1->config()) {
        return FALSE;
    }

    SkAutoLockPixels alp0(*bm0);
    SkAutoLockPixels alp1(*bm1);

    // if we can't load the pixels, return FALSE
    if (NULL == bm0->getPixels() || NULL == bm1->getPixels()) {
        return FALSE;
    }

    if (bm0->config() == SkBitmap::kIndex8_Config) {
        SkColorTable* ct0 = bm0->getColorTable();
        SkColorTable* ct1 = bm1->getColorTable();
        if (NULL == ct0 || NULL == ct1) {
            return FALSE;
        }
        if (ct0->count() != ct1->count()) {
            return FALSE;
        }

        SkAutoLockColors alc0(ct0);
        SkAutoLockColors alc1(ct1);
        const size_t size = ct0->count() * sizeof(SkPMColor);
        if (memcmp(alc0.colors(), alc1.colors(), size) != 0) {
            return FALSE;
        }
    }

    // now compare each scanline. We can't do the entire buffer at once,
    // since we don't care about the pixel values that might extend beyond
    // the width (since the scanline might be larger than the logical width)
    const Int32 h = bm0->height();
    const size_t size = bm0->width() * bm0->bytesPerPixel();
    for (Int32 y = 0; y < h; y++) {
        if (memcmp(bm0->getAddr(0, y), bm1->getAddr(0, y), size) != 0) {
            return FALSE;
        }
    }
    return TRUE;
}

Int32 CBitmap::Ni()
{
    return mNativeBitmap;
}

AutoPtr<IInterface> CBitmap::CreateBitmap(
    /* [in] */ Int32 nativeBitmap,
    /* [in] */ ArrayOf<Byte>* buffer,
    /* [in] */ Boolean isMutable,
    /* [in] */ ArrayOf<Byte>* ninePatchChunk,
    /* [in] */ Int32 density)
{
    AutoPtr<IBitmap> aBitmap;
    CBitmap::New((Handle32)nativeBitmap, buffer,
             isMutable, ninePatchChunk, density, (IBitmap**)&aBitmap);
    return aBitmap;
}

AutoPtr<IInterface> CBitmap::CreateBitmap(
    /* [in] */ Int32 nativeBitmap,
    /* [in] */ ArrayOf<Byte>* buffer,
    /* [in] */ Boolean isMutable,
    /* [in] */ ArrayOf<Byte>* ninePatchChunk,
    /* [in] */ ArrayOf<Int32>* layoutbounds,
    /* [in] */ Int32 density)
{
    AutoPtr<IBitmap> aBitmap;
    CBitmap::New((Handle32)nativeBitmap, buffer,
             isMutable, ninePatchChunk, layoutbounds, density, (IBitmap**)&aBitmap);
    return aBitmap;
}

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos

