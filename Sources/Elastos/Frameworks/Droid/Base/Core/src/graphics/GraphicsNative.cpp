
#include "ext/frameworkext.h"
#include "graphics/GraphicsNative.h"
#include "graphics/CRect.h"
#include "graphics/CRectF.h"
#include "graphics/CBitmap.h"
#include "graphics/BitmapRegionDecoder.h"
#include <skia/core/SkDither.h>

namespace Elastos {
namespace Droid {
namespace Graphics {

SkRect* GraphicsNative::IRect2SkRect(
    /* [in] */ IRect* obj,
    /* [in] */ SkRect* sr)
{
    CRect* r = (CRect*)obj;
    sr->set(SkIntToScalar(r->mLeft), SkIntToScalar(r->mTop),
           SkIntToScalar(r->mRight), SkIntToScalar(r->mBottom));
    return sr;
}

SkIRect* GraphicsNative::IRect2SkIRect(
    /* [in] */ IRect* obj,
    /* [in] */ SkIRect* sr)
{
    CRect* r = (CRect*)obj;
    sr->set(r->mLeft, r->mTop, r->mRight, r->mBottom);
    return sr;
}

void GraphicsNative::SkIRect2IRect(
    /* [in] */ const SkIRect& ir,
    /* [in] */ IRect* obj)
{
    CRect* r = (CRect*)obj;
    r->mLeft = ir.fLeft;
    r->mTop = ir.fTop;
    r->mRight = ir.fRight;
    r->mBottom = ir.fBottom;
}

SkRect* GraphicsNative::IRectF2SkRect(
    /* [in] */ IRectF* obj,
    /* [in] */ SkRect* r)
{
    CRectF* rf = (CRectF*)obj;
    r->set(SkFloatToScalar(rf->mLeft), SkFloatToScalar(rf->mTop),
           SkFloatToScalar(rf->mRight), SkFloatToScalar(rf->mBottom));
    return r;
}

void GraphicsNative::SkRect2IRectF(
    /* [in] */ const SkRect& r,
    /* [in] */ IRectF* obj)
{
    CRectF* rf = (CRectF*)obj;
    rf->mLeft = r.fLeft;
    rf->mTop = r.fTop;
    rf->mRight = r.fRight;
    rf->mBottom = r.fBottom;
}

void GraphicsNative::SkIPoint2IPoint(
    /* [in] */ const SkIPoint& point,
    /* [in] */ IPoint* obj)
{
    obj->Set(point.fX, point.fY);
}

SkBitmap* GraphicsNative::GetNativeBitmap(
    /* [in] */ IBitmap* bitmap)
{
    SkBitmap* b = (SkBitmap*)((CBitmap*)bitmap)->mNativeBitmap;
    SkASSERT(b);
    return b;
}

SkBitmap::Config GraphicsNative::GetNativeBitmapConfig(
    /* [in] */ BitmapConfig config)
{
    if (-1 == config) {
        return SkBitmap::kNo_Config;
    }
    Int32 c = config;
    if (c < 0 || c >= SkBitmap::kConfigCount) {
        c = SkBitmap::kNo_Config;
    }
    return static_cast<SkBitmap::Config>(c);
}

ECode GraphicsNative::CreateBitmap(
    /* [in] */ SkBitmap* nativeBitmap,
    /* [in] */ ArrayOf<Byte>* buffer,
    /* [in] */ Boolean isMutable,
    /* [in] */ ArrayOf<Byte>* ninepatch,
    /* [in] */ ArrayOf<Int32>* layoutbounds,
    /* [in] */ Int32 density,
    /* [out] */ CBitmap** bitmap)
{
    VALIDATE_NOT_NULL(bitmap);

    SkASSERT(nativeBitmap != NULL);
    SkASSERT(NULL != nativeBitmap->pixelRef());
    return CBitmap::NewByFriend((Handle32)nativeBitmap, buffer,
            isMutable, ninepatch, layoutbounds, density, bitmap);
}

ECode GraphicsNative::CreateBitmapRegionDecoder(
    /* [in] */ SkBitmapRegionDecoder* bitmap,
    /* [out] */ IBitmapRegionDecoder** decoder)
{
    VALIDATE_NOT_NULL(decoder);

    SkASSERT(bitmap != NULL);

    *decoder = new BitmapRegionDecoder((Handle32)bitmap);
    REFCOUNT_ADD(*decoder);
    return NOERROR;
}

///////////////////////////////////////////////////////////////////////////////

GraphicsNative::DroidPixelRef::DroidPixelRef(void* storage, size_t size, ArrayOf<Byte>* storageObj,
        SkColorTable* ctable) : SkMallocPixelRef(storage, size, ctable)
{
    SkASSERT(storage);

    mStorageObj = storageObj;
    mHasGlobalRef = FALSE;
    mGlobalRefCnt = 0;

    // If storageObj is NULL, the memory was NOT allocated on the Java heap
    mOnDroidHeap = (storageObj != NULL);

}

GraphicsNative::DroidPixelRef::~DroidPixelRef()
{
    if (mOnDroidHeap) {
        if (mStorageObj && mHasGlobalRef) {
            // env->DeleteGlobalRef(fStorageObj);
        }
        mStorageObj = NULL;

        // Set this to NULL to prevent the SkMallocPixelRef destructor
        // from freeing the memory.
        fStorage = NULL;
    }
}

void GraphicsNative::DroidPixelRef::setLocalRef(ArrayOf<Byte>* arr)
{
    if (!mHasGlobalRef) {
        mStorageObj = arr;
    }
}

void GraphicsNative::DroidPixelRef::globalRef(void* localref)
{
    if (mOnDroidHeap && sk_atomic_inc(&mGlobalRefCnt) == 0) {
        // If JNI ref was passed, it is always used
        if (localref) mStorageObj = (ArrayOf<Byte>*)localref;

        if (mStorageObj == NULL) {
            SkDebugf("No valid local ref to create a JNI global ref\n");
            sk_throw();
        }
        if (mHasGlobalRef) {
            // This should never happen
            SkDebugf("Already holding a JNI global ref");
            sk_throw();
        }

        // fStorageObj = (jbyteArray) env->NewGlobalRef(fStorageObj);
        // TODO: Check for failure here
        mHasGlobalRef = TRUE;
    }
    ref();
}

void GraphicsNative::DroidPixelRef::globalUnref()
{
    if (mOnDroidHeap && sk_atomic_dec(&mGlobalRefCnt) == 1) {
        if (!mHasGlobalRef) {
            SkDebugf("We don't have a global ref!");
            sk_throw();
        }
        // env->DeleteGlobalRef(fStorageObj);
        mStorageObj = NULL;
        mHasGlobalRef = FALSE;
    }
    unref();
}

ECode GraphicsNative::AllocateDroidPixelRef(
    /* [in] */ SkBitmap* bitmap,
    /* [in] */ SkColorTable* ctable,
    /* [out] */ ArrayOf<Byte>** pixelRef)
{
    VALIDATE_NOT_NULL(pixelRef);

    Sk64 size64 = bitmap->getSize64();
    if (size64.isNeg() || !size64.is32()) {
        // jniThrowException(env, "java/lang/IllegalArgumentException",
        //                   "bitmap size exceeds 32bits");
        *pixelRef = NULL;
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    size_t size = size64.get32();
    AutoPtr< ArrayOf<Byte> > arrayObj = ArrayOf<Byte>::Alloc(size);
    if (arrayObj) {
        // TODO: make this work without jniGetNonMovableArrayElements
        Byte* addr = arrayObj->GetPayload();
        if (addr) {
            SkPixelRef* pr = new DroidPixelRef((void*)addr, size, arrayObj, ctable);
            bitmap->setPixelRef(pr)->unref();
            // since we're already allocated, we lockPixels right away
            // HeapAllocator behaves this way too
            bitmap->lockPixels();
        }
    }

    *pixelRef = arrayObj;
    REFCOUNT_ADD(*pixelRef);
    return NOERROR;
}

///////////////////////////////////////////////////////////////////////////////

GraphicsNative::DroidPixelAllocator::DroidPixelAllocator()
    : mAllocCount(0)
{}

bool GraphicsNative::DroidPixelAllocator::allocPixelRef(
    /* [in] */ SkBitmap* bitmap,
    /* [in] */ SkColorTable* ctable)
{
    mStorageObj = NULL;
    GraphicsNative::AllocateDroidPixelRef(bitmap, ctable, (ArrayOf<Byte>**)&mStorageObj);
    mAllocCount += 1;
    return mStorageObj != NULL;
}

///////////////////////////////////////////////////////////////////////////////
// Conversions to/from SkColor, for get/setPixels, and the create method, which
// is basically like setPixels

typedef void (*FromColorProc)(void* dst, const SkColor src[], int width,
                              int x, int y);

static void FromColor_D32(void* dst, const SkColor src[], int width,
                          int, int)
{
    SkPMColor* d = (SkPMColor*)dst;

    for (int i = 0; i < width; i++) {
        *d++ = SkPreMultiplyColor(*src++);
    }
}

static void FromColor_D565(void* dst, const SkColor src[], int width,
                           int x, int y)
{
    uint16_t* d = (uint16_t*)dst;

    DITHER_565_SCAN(y);
    for (int stop = x + width; x < stop; x++) {
        SkColor c = *src++;
        *d++ = SkDitherRGBTo565(SkColorGetR(c), SkColorGetG(c), SkColorGetB(c),
                                DITHER_VALUE(x));
    }
}

static void FromColor_D4444(void* dst, const SkColor src[], int width,
                            int x, int y)
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
static FromColorProc ChooseFromColorProc(SkBitmap::Config config) {
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

Boolean GraphicsNative::SetPixels(
    /* [in] */ const ArrayOf<Int32>& srcColors,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 srcStride,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ const SkBitmap& dstBitmap)
{
    SkAutoLockPixels alp(dstBitmap);
    void* dst = dstBitmap.getPixels();
    FromColorProc proc = ChooseFromColorProc(dstBitmap.config());

    if (NULL == dst || NULL == proc) {
        return FALSE;
    }

    const Int32* array = srcColors.GetPayload();
    const SkColor* src = (const SkColor*)array + srcOffset;

    // reset to to actual choice from caller
    dst = dstBitmap.getAddr(x, y);
    // now copy/convert each scanline
    for (Int32 y = 0; y < height; y++) {
        proc(dst, src, width, x, y);
        src += srcStride;
        dst = (char*)dst + dstBitmap.rowBytes();
    }

    dstBitmap.notifyPixelsChanged();

    return TRUE;
}

} // namespace Graphics
} // namespace Droid
} // namespace Elastos
