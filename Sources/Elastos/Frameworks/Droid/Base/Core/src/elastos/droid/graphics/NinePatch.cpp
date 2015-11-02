
#include "elastos/droid/graphics/NinePatch.h"
#include "elastos/droid/graphics/CRectF.h"
#include "elastos/droid/graphics/CRect.h"
#include "elastos/droid/graphics/CBitmap.h"
#include "elastos/droid/graphics/CCanvas.h"
#include "elastos/droid/graphics/CPaint.h"
#include "elastos/droid/graphics/CRegion.h"
#include "elastos/droid/graphics/GraphicsNative.h"
#include "elastos/droid/graphics/NativeCanvas.h"
#include "elastos/droid/graphics/NativePaint.h"
#include <elastos/utility/logging/Logger.h>
#include <androidfw/ResourceTypes.h>
#include <skia/core/SkCanvas.h>

using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Graphics {

extern ECode NinePatch_Draw(SkCanvas* canvas, const SkRect& bounds,
        const SkBitmap& bitmap, const android::Res_png_9patch& chunk,
        const SkPaint* paint, SkRegion** outRegion);

static const char* TAG = "NinePatch";

CAR_INTERFACE_IMPL(NinePatch::InsetStruct, Object, INinePatchInsetStruct);
NinePatch::InsetStruct::InsetStruct(
    /* [in] */ Int32 opticalLeft,
    /* [in] */ Int32 opticalTop,
    /* [in] */ Int32 opticalRight,
    /* [in] */ Int32 opticalBottom,
    /* [in] */ Int32 outlineLeft,
    /* [in] */ Int32 outlineTop,
    /* [in] */ Int32 outlineRight,
    /* [in] */ Int32 outlineBottom,
    /* [in] */ Float outlineRadius,
    /* [in] */ Int32 outlineAlpha,
    /* [in] */ Float decodeScale)
    : mOutlineRadius(outlineRadius * decodeScale)
    , mOutlineAlpha(outlineAlpha / 255.0f)
{
    CRect::New(opticalLeft, opticalTop, opticalRight, opticalBottom, (IRect**)&mOpticalRect);
    CRect::New(outlineLeft, outlineTop, outlineRight, outlineBottom, (IRect**)&mOutlineRect);

    if (decodeScale != 1.0f) {
        // if bitmap was scaled when decoded, scale the insets from the metadata values
        mOpticalRect->Scale(decodeScale);

        // round inward while scaling outline, as the outline should always be conservative
        assert(0 && "TODO");
        // mOutlineRect->ScaleRoundIn(decodeScale);
    }
}

ECode NinePatch::InsetStruct::GetOpticalRect(
    /* [out] */ IRect** rect)
{
    VALIDATE_NOT_NULL(rect);
    *rect = mOpticalRect;
    REFCOUNT_ADD(*rect);
    return NOERROR;
}

ECode NinePatch::InsetStruct::GetOutlineRect(
    /* [out] */ IRect** rect)
{
    VALIDATE_NOT_NULL(rect);
    *rect = mOutlineRect;
    REFCOUNT_ADD(*rect);
    return NOERROR;
}

ECode NinePatch::InsetStruct::GetOutlineRadius(
    /* [out] */ Float* radius)
{
    VALIDATE_NOT_NULL(radius);
    *radius = mOutlineRadius;
    return NOERROR;
}

ECode NinePatch::InsetStruct::GetOutlineAlpha(
    /* [out] */ Float* alpha)
{
    VALIDATE_NOT_NULL(alpha);
    *alpha = mOutlineAlpha;
    return NOERROR;
}

CAR_INTERFACE_IMPL(NinePatch, Object, INinePatch);
NinePatch::~NinePatch()
{
    NativeFinalize(mNativeChunk);
}

ECode NinePatch::constructor(
    /* [in] */ IBitmap* bitmap,
    /* [in] */ ArrayOf<Byte>* chunk)
{
    return constructor(bitmap, chunk, String(NULL));
}

ECode NinePatch::constructor(
    /* [in] */ IBitmap* bitmap,
    /* [in] */ ArrayOf<Byte>* chunk,
    /* [in] */ const String& srcName)
{
    mBitmap = bitmap;
    mSrcName = srcName;
    mNativeChunk = ValidateNinePatchChunk(((CBitmap*)mBitmap.Get())->Ni(), chunk);
    return NOERROR;
}

ECode NinePatch::constructor(
    /* [in] */ INinePatch* _patch)
{
    NinePatch* patch = (NinePatch*)_patch;
    mBitmap = patch->mBitmap;
    mSrcName = patch->mSrcName;
    if (patch->mPaint != NULL) {
        CPaint::New(patch->mPaint, (IPaint**)&mPaint);
    }
    // No need to validate the 9patch chunk again, it was done by
    // the instance we're copying from
    mNativeChunk = patch->mNativeChunk;
    return NOERROR;
}

ECode NinePatch::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = mSrcName;
    return NOERROR;
}

ECode NinePatch::GetPaint(
    /* [out] */ IPaint** paint)
{
    VALIDATE_NOT_NULL(paint);
    *paint = mPaint;
    REFCOUNT_ADD(*paint);
    return NOERROR;
}

ECode NinePatch::SetPaint(
    /* [in] */ IPaint* p)
{
    mPaint = p;
    return NOERROR;
}

ECode NinePatch::GetBitmap(
    /* [out] */ IBitmap** bitmap)
{
    VALIDATE_NOT_NULL(bitmap);
    *bitmap = mBitmap;
    REFCOUNT_ADD(*bitmap);
    return NOERROR;
}

ECode NinePatch::Draw(
    /* [in] */ ICanvas* canvas,
    /* [in] */ IRectF* location)
{
    return canvas->DrawPatch(this, location, mPaint);
}

ECode NinePatch::Draw(
    /* [in] */ ICanvas* canvas,
    /* [in] */ IRect* location)
{
    return canvas->DrawPatch(this, location, mPaint);
}

ECode NinePatch::Draw(
    /* [in] */ ICanvas* canvas,
    /* [in] */ IRect* location,
    /* [in] */ IPaint* paint)
{
    return canvas->DrawPatch(this, location, paint);
}

void NinePatch::DrawSoftware(
    /* [in] */ ICanvas* canvas,
    /* [in] */ IRectF* location,
    /* [in] */ IPaint* paint)
{
    NativeDraw(((CCanvas*)canvas)->GetNativeCanvasWrapper(), location, ((CBitmap*)mBitmap.Get())->Ni(), mNativeChunk,
            paint != NULL ? ((CPaint*)paint)->mNativePaint : 0, ((CCanvas*)canvas)->mDensity, ((CBitmap*)mBitmap.Get())->mDensity);
}

void NinePatch::DrawSoftware(
    /* [in] */ ICanvas* canvas,
    /* [in] */ IRect* location,
    /* [in] */ IPaint* paint)
{
    NativeDraw(((CCanvas*)canvas)->GetNativeCanvasWrapper(), location, ((CBitmap*)mBitmap.Get())->Ni(), mNativeChunk,
            paint != NULL ? ((CPaint*)paint)->mNativePaint  : 0, ((CCanvas*)canvas)->mDensity, ((CBitmap*)mBitmap.Get())->mDensity);
}

ECode NinePatch::GetDensity(
    /* [out] */ Int32* density)
{
    VALIDATE_NOT_NULL(density);
    return mBitmap->GetDensity(density);
}

ECode NinePatch::GetWidth(
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);
    return mBitmap->GetWidth(width);
}

ECode NinePatch::GetHeight(
    /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height);
    return mBitmap->GetHeight(height);
}

ECode NinePatch::HasAlpha(
    /* [out] */ Boolean * has)
{
    VALIDATE_NOT_NULL(has);
    return mBitmap->HasAlpha(has);
}

ECode NinePatch::GetTransparentRegion(
    /* [in] */ IRect* bounds,
    /* [out] */ IRegion** region)
{
    VALIDATE_NOT_NULL(region);
    Int64 r = NativeGetTransparentRegion(((CBitmap*)mBitmap.Get())->Ni(), mNativeChunk, bounds);
    if (r != 0) {
        return CRegion::New(r, (IRegion**)&region);
    }

    *region = NULL;
    return NOERROR;
}

Boolean NinePatch::IsNinePatchChunk(
    /* [in] */ ArrayOf<Byte>* chunk)
{
    if (NULL == chunk) {
        return FALSE;
    }
    if (chunk->GetLength() < (Int32)sizeof(android::Res_png_9patch)) {
        return FALSE;
    }
    const AutoPtr<ArrayOf<Byte> > array = chunk;
    if (array != NULL) {
        const android::Res_png_9patch* chunk = reinterpret_cast<const android::Res_png_9patch*>(array->GetPayload());
        int8_t wasDeserialized = chunk->wasDeserialized;
        // env->ReleaseByteArrayElements(chunk, const_cast<jbyte*>(array), JNI_ABORT);
        return (wasDeserialized != -1) ? TRUE : FALSE;
    }
    return FALSE;
}

ECode NinePatch::IsNinePatchChunk(
    /* [in] */ ArrayOf<Byte>* chunk,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = IsNinePatchChunk(chunk);
    return NOERROR;
}

Int64 NinePatch::ValidateNinePatchChunk(
    /* [in] */ Int64 bitmap,
    /* [in] */ ArrayOf<Byte>* chunk)
{
    size_t chunkSize = chunk->GetLength();
    if (chunkSize < (int) (sizeof(android::Res_png_9patch))) {
        // jniThrowRuntimeException(env, "Array too small for chunk.");
        return /*NULL*/0;
    }

    int8_t* storage = new int8_t[chunkSize];
    // This call copies the content of the jbyteArray
    // env->GetByteArrayRegion(chunk, 0, chunkSize, reinterpret_cast<jbyte*>(storage));

    // Deserialize in place, return the array we just allocated
    return reinterpret_cast<Int64>(android::Res_png_9patch::deserialize(storage));
}

void NinePatch::NativeFinalize(
    /* [in] */ Int64 patchHandle)
{
    int8_t* patch = reinterpret_cast<int8_t*>(patchHandle);
#ifdef USE_OPENGL_RENDERER
    if (android::uirenderer::Caches::hasInstance()) {
        Res_png_9patch* p = (Res_png_9patch*) patch;
        android::uirenderer::Caches::getInstance().resourceCache.destructor(p);
        return;
    }
#endif // USE_OPENGL_RENDERER
    delete[] patch;
}

static void draw(
    /* [in] */ SkCanvas* canvas,
    /* [in] */ SkRect& bounds,
    /* [in] */ const SkBitmap* bitmap,
    /* [in] */ android::Res_png_9patch* chunk,
    /* [in] */ const SkPaint* paint,
    /* [in] */ Int32 destDensity,
    /* [in] */ Int32 srcDensity)
{
    if (destDensity == srcDensity || destDensity == 0 || srcDensity == 0) {
        Logger::V(TAG, String("Drawing unscaled 9-patch: (%g,%g)-(%g,%g)"),
                SkScalarToFloat(bounds.fLeft), SkScalarToFloat(bounds.fTop),
                SkScalarToFloat(bounds.fRight), SkScalarToFloat(bounds.fBottom));
        NinePatch_Draw(canvas, bounds, *bitmap, *chunk, paint, NULL);
    } else {
        canvas->save();

        SkScalar scale = destDensity / (float)srcDensity;
        canvas->translate(bounds.fLeft, bounds.fTop);
        canvas->scale(scale, scale);

        bounds.fRight = SkScalarDiv(bounds.fRight-bounds.fLeft, scale);
        bounds.fBottom = SkScalarDiv(bounds.fBottom-bounds.fTop, scale);
        bounds.fLeft = bounds.fTop = 0;

        Logger::V(TAG, String("Drawing scaled 9-patch: (%g,%g)-(%g,%g) srcDensity=%d destDensity=%d"),
                SkScalarToFloat(bounds.fLeft), SkScalarToFloat(bounds.fTop),
                SkScalarToFloat(bounds.fRight), SkScalarToFloat(bounds.fBottom),
                srcDensity, destDensity);

        NinePatch_Draw(canvas, bounds, *bitmap, *chunk, paint, NULL);

        canvas->restore();
    }
}

void NinePatch::NativeDraw(
    /* [in] */ Int64 canvasHandle,
    /* [in] */ IRectF* boundsRectF,
    /* [in] */ Int64 bitmapHandle,
    /* [in] */ Int64 chunkHandle,
    /* [in] */ Int64 paintHandle,
    /* [in] */ Int32 destDensity,
    /* [in] */ Int32 srcDensity)
{
    SkCanvas* canvas       = reinterpret_cast<NativeCanvas*>(canvasHandle)->getSkCanvas();
    const SkBitmap* bitmap = reinterpret_cast<SkBitmap*>(bitmapHandle);
    android::Res_png_9patch* chunk  = reinterpret_cast<android::Res_png_9patch*>(chunkHandle);
    const NativePaint* paint     = reinterpret_cast<NativePaint*>(paintHandle);
    SkASSERT(canvas);
    SkASSERT(boundsRectF);
    SkASSERT(bitmap);
    SkASSERT(chunk);
    // paint is optional

    SkRect bounds;
    GraphicsNative::IRectF2SkRect(boundsRectF, &bounds);

    draw(canvas, bounds, bitmap, chunk, paint, destDensity, srcDensity);
}

void NinePatch::NativeDraw(
    /* [in] */ Int64 canvasHandle,
    /* [in] */ IRect* boundsRect,
    /* [in] */ Int64 bitmapHandle,
    /* [in] */ Int64 chunkHandle,
    /* [in] */ Int64 paintHandle,
    /* [in] */ Int32 destDensity,
    /* [in] */ Int32 srcDensity)
{
    SkCanvas* canvas       = reinterpret_cast<NativeCanvas*>(canvasHandle)->getSkCanvas();
    const SkBitmap* bitmap = reinterpret_cast<SkBitmap*>(bitmapHandle);
    android::Res_png_9patch* chunk  = reinterpret_cast<android::Res_png_9patch*>(chunkHandle);
    const NativePaint* paint     = reinterpret_cast<NativePaint*>(paintHandle);
    SkASSERT(canvas);
    SkASSERT(boundsRect);
    SkASSERT(bitmap);
    SkASSERT(chunk);
    // paint is optional

    SkRect bounds;
    GraphicsNative::IRect2SkRect(boundsRect, &bounds);
    draw(canvas, bounds, bitmap, chunk, paint, destDensity, srcDensity);
}

Int64 NinePatch::NativeGetTransparentRegion(
    /* [in] */ Int64 bitmapHandle,
    /* [in] */ Int64 chunkHandle,
    /* [in] */ IRect* boundsRect)
{
    const SkBitmap* bitmap = reinterpret_cast<SkBitmap*>(bitmapHandle);
    android::Res_png_9patch* chunk = reinterpret_cast<android::Res_png_9patch*>(chunkHandle);
    SkASSERT(bitmap);
    SkASSERT(chunk);
    SkASSERT(boundsRect);

    SkRect bounds;
    GraphicsNative::IRect2SkRect(boundsRect, &bounds);

    SkRegion* region = NULL;
    NinePatch_Draw(NULL, bounds, *bitmap, *chunk, NULL, &region);

    return reinterpret_cast<Int64>(region);
}

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos
