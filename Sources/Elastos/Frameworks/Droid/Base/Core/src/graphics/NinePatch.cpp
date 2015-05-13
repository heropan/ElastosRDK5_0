
#include "graphics/NinePatch.h"
#include "graphics/CRectF.h"
#include "graphics/CBitmap.h"
#include "graphics/Canvas.h"
#include "graphics/CPaint.h"
#include "graphics/CRegion.h"
#include "graphics/GraphicsNative.h"
#include <elastos/Logger.h>
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

NinePatch::NinePatch()
{
    CRectF::New((IRectF**)&mRect);
}

ECode NinePatch::Init(
    /* [in] */ IBitmap* bitmap,
    /* [in] */ ArrayOf<Byte>* chunk,
    /* [in] */ const String& srcName)
{
    mBitmap = bitmap;
    mChunk = chunk;
    mSrcName = srcName;
    return ValidateNinePatchChunk(((CBitmap*)mBitmap.Get())->Ni(), chunk);
}

ECode NinePatch::Init(
    /* [in] */ NinePatch* patch)
{
    mBitmap = patch->mBitmap;
    mChunk = patch->mChunk;
    mSrcName = patch->mSrcName;
    if (patch->mPaint != NULL) {
        FAIL_RETURN(CPaint::New(patch->mPaint.Get(), (IPaint**)&mPaint));
    }
    return ValidateNinePatchChunk(((CBitmap*)mBitmap.Get())->Ni(), mChunk);
}

ECode NinePatch::SetPaint(
    /* [in] */ IPaint* p)
{
    mPaint = p;
    return NOERROR;
}

ECode NinePatch::Draw(
    /* [in] */ ICanvas* canvas,
    /* [in] */ IRectF* location)
{
    Boolean isAccelerated;
    canvas->IsHardwareAccelerated(&isAccelerated);
    if (!isAccelerated) {
        return NativeDraw(((Canvas*)canvas->Probe(EIID_Canvas))->mNativeCanvas,
            location, ((CBitmap*)mBitmap.Get())->Ni(), mChunk.Get(),
            mPaint != NULL ? ((Paint*)mPaint->Probe(EIID_Paint))->mNativePaint : 0,
            ((Canvas*)canvas->Probe(EIID_Canvas))->mDensity,
            ((CBitmap*)mBitmap.Get())->mDensity);
    }
    else {
        return canvas->DrawPatch(mBitmap, *mChunk, location, mPaint);
    }
}

ECode NinePatch::Draw(
    /* [in] */ ICanvas* canvas,
    /* [in] */ IRect* location)
{
    Boolean isAccelerated;
    canvas->IsHardwareAccelerated(&isAccelerated);
    if (!isAccelerated) {
        return NativeDraw(((Canvas*)canvas->Probe(EIID_Canvas))->mNativeCanvas,
            location, ((CBitmap*)mBitmap.Get())->Ni(), mChunk.Get(),
            mPaint != NULL ? ((Paint*)mPaint->Probe(EIID_Paint))->mNativePaint : 0,
            ((Canvas*)canvas->Probe(EIID_Canvas))->mDensity,
            ((CBitmap*)mBitmap.Get())->mDensity);
    }
    else {
        mRect->SetEx2(location);
        return canvas->DrawPatch(mBitmap, *mChunk, mRect, mPaint);
    }
}

ECode NinePatch::Draw(
    /* [in] */ ICanvas* canvas,
    /* [in] */ IRect* location,
    /* [in] */ IPaint* paint)
{
    Boolean isAccelerated;
    canvas->IsHardwareAccelerated(&isAccelerated);
    if (!isAccelerated) {
    return NativeDraw(((Canvas*)canvas->Probe(EIID_Canvas))->mNativeCanvas, location,
            ((CBitmap*)mBitmap.Get())->Ni(), mChunk.Get(),
            paint != NULL ? ((Paint*)paint->Probe(EIID_Paint))->mNativePaint : 0,
            ((Canvas*)canvas->Probe(EIID_Canvas))->mDensity,
            ((CBitmap*)mBitmap.Get())->mDensity);
    }
    else {
        mRect->SetEx2(location);
        return canvas->DrawPatch(mBitmap, *mChunk, mRect, paint);
    }
}

ECode NinePatch::GetDensity(
    /* [out] */ Int32* density)
{
    return mBitmap->GetDensity(density);
}

ECode NinePatch::GetWidth(
    /* [out] */ Int32* width)
{
    return mBitmap->GetWidth(width);
}

ECode NinePatch::GetHeight(
    /* [out] */ Int32* height)
{
    return mBitmap->GetHeight(height);
}

ECode NinePatch::HasAlpha(
    /* [out] */ Boolean* hasAlpha)
{
    return mBitmap->HasAlpha(hasAlpha);
}

ECode NinePatch::GetTransparentRegion(
    /* [in] */ IRect* location,
    /* [out] */ IRegion** region)
{
    Int32 r = NativeGetTransparentRegion(((CBitmap*)mBitmap.Get())->Ni(), mChunk.Get(), location);
    if (r == 0) {
        *region = NULL;
        return NOERROR;
    }
    return CRegion::New(r, region);
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
    Byte* array = chunk->GetPayload();
    if (array != NULL) {
        const android::Res_png_9patch* chunk =
                reinterpret_cast<const android::Res_png_9patch*>(array);
        int8_t wasDeserialized = chunk->wasDeserialized;
        return wasDeserialized != -1;
    }
    return FALSE;
}

ECode NinePatch::ValidateNinePatchChunk(
    /* [in] */ Int32 bitmap,
    /* [in] */ ArrayOf<Byte>* chunk)
{
    if (chunk->GetLength() < (Int32)sizeof(android::Res_png_9patch)) {
        Logger::E(TAG, "Array too small for chunk.");
        return E_RUNTIME_EXCEPTION;
    }
    return NOERROR;
}

static ECode native_draw(
    /* [in] */ SkCanvas* canvas,
    /* [in] */ SkRect& bounds,
    /* [in] */ const SkBitmap* bitmap,
    /* [in] */ ArrayOf<Byte>* chunkObj,
    /* [in] */ const SkPaint* paint,
    /* [in] */ Int32 destDensity,
    /* [in] */ Int32 srcDensity)
{
    ECode ec = NOERROR;
    Int32 chunkSize = chunkObj->GetLength();
    void* storage = chunkObj->GetPayload();
    // need to deserialize the chunk
    android::Res_png_9patch* chunk = static_cast<android::Res_png_9patch*>(storage);
    assert(chunkSize == (Int32)chunk->serializedSize());
    // this relies on deserialization being done in place
    android::Res_png_9patch::deserialize(chunk);

    if (destDensity == srcDensity || destDensity == 0
            || srcDensity == 0) {
        // Logger::V(TAG, "Drawing unscaled 9-patch: (%g,%g)-(%g,%g)",
        //         SkScalarToFloat(bounds.fLeft), SkScalarToFloat(bounds.fTop),
        //         SkScalarToFloat(bounds.fRight), SkScalarToFloat(bounds.fBottom));
        ec = NinePatch_Draw(canvas, bounds, *bitmap, *chunk, paint, NULL);
    }
    else {
        canvas->save();

        SkScalar scale = SkFloatToScalar(destDensity / (float)srcDensity);
        canvas->translate(bounds.fLeft, bounds.fTop);
        canvas->scale(scale, scale);

        bounds.fRight = SkScalarDiv(bounds.fRight-bounds.fLeft, scale);
        bounds.fBottom = SkScalarDiv(bounds.fBottom-bounds.fTop, scale);
        bounds.fLeft = bounds.fTop = 0;

        Logger::V(TAG, "Drawing scaled 9-patch: (%g,%g)-(%g,%g) srcDensity=%d destDensity=%d",
                SkScalarToFloat(bounds.fLeft), SkScalarToFloat(bounds.fTop),
                SkScalarToFloat(bounds.fRight), SkScalarToFloat(bounds.fBottom),
                srcDensity, destDensity);

        ec = NinePatch_Draw(canvas, bounds, *bitmap, *chunk, paint, NULL);

        canvas->restore();
    }

    return ec;
}

ECode NinePatch::NativeDraw(
    /* [in] */ Int32 canvas,
    /* [in] */ IRectF* boundsRect,
    /* [in] */ Int32 bitmap,
    /* [in] */ ArrayOf<Byte>* c,
    /* [in] */ Int32 paint, // may be null
    /* [in] */ Int32 destDensity,
    /* [in] */ Int32 srcDensity)
{
    SkASSERT(canvas);
    SkASSERT(boundsRect);
    SkASSERT(bitmap);
    SkASSERT(c);
    // paint is optional

    SkRect bounds;
    GraphicsNative::IRectF2SkRect(boundsRect, &bounds);

    return native_draw((SkCanvas*)canvas, bounds, (SkBitmap*)bitmap, c,
            (SkPaint*)paint, destDensity, srcDensity);
}

ECode NinePatch::NativeDraw(
    /* [in] */ Int32 canvas,
    /* [in] */ IRect* boundsRect,
    /* [in] */ Int32 bitmap,
    /* [in] */ ArrayOf<Byte>* c,
    /* [in] */ Int32 paint, // may be null
    /* [in] */ Int32 destDensity,
    /* [in] */ Int32 srcDensity)
{
    SkASSERT(canvas);
    SkASSERT(boundsRect);
    SkASSERT(bitmap);
    SkASSERT(c);
    // paint is optional

    SkRect bounds;
    GraphicsNative::IRect2SkRect(boundsRect, &bounds);

    return native_draw((SkCanvas*)canvas, bounds, (SkBitmap*)bitmap, c,
            (SkPaint*)paint, destDensity, srcDensity);
}

Int32 NinePatch::NativeGetTransparentRegion(
    /* [in] */ Int32 bitmap,
    /* [in] */ ArrayOf<Byte>* chunkObj,
    /* [in] */ IRect* boundsRect)
{
    SkASSERT(bitmap);
    SkASSERT(chunkObj);
    SkASSERT(boundsRect);

    SkRect bounds;
    GraphicsNative::IRect2SkRect(boundsRect, &bounds);
    Int32 chunkSize = chunkObj->GetLength();
    void* storage = chunkObj->GetPayload();
    // need to deserialize the chunk
    android::Res_png_9patch* chunk = static_cast<android::Res_png_9patch*>(storage);
    assert(chunkSize == (Int32)chunk->serializedSize());
    // this relies on deserialization being done in place
    android::Res_png_9patch::deserialize(chunk);
    SkRegion* region = NULL;
    NinePatch_Draw(NULL, bounds, *(SkBitmap*)bitmap, *chunk, NULL, &region);
    return (Int32)region;
}

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos
