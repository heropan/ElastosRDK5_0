
#include "graphics/NinePatch.h"
#include "graphics/CRectF.h"
#include "graphics/CBitmap.h"
#include "graphics/Canvas.h"
#include "graphics/CPaint.h"
#include "graphics/CRegion.h"
#include "graphics/GraphicsNative.h"
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
    assert(0 && "TODO");
    // mNativeChunk = ValidateNinePatchChunk(mBitmap.ni(), chunk);
    return NOERROR;
}

ECode NinePatch::constructor(
    /* [in] */ INinePatch* patch)
{
    assert(0 && "TODO");
    // mBitmap = patch.mBitmap;
    // mSrcName = patch.mSrcName;
    // if (patch.mPaint != null) {
    //     mPaint = new Paint(patch.mPaint);
    // }
    // // No need to validate the 9patch chunk again, it was done by
    // // the instance we're copying from
    // mNativeChunk = patch.mNativeChunk;
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
    assert(0 && "TODO");
    // canvas.drawPatch(this, location, mPaint);
    return NOERROR;
}

ECode NinePatch::Draw(
    /* [in] */ ICanvas* canvas,
    /* [in] */ IRect* location)
{
    assert(0 && "TODO");
    // return canvas->DrawPatch(this, location, mPaint);
    return NOERROR;
}

ECode NinePatch::Draw(
    /* [in] */ ICanvas* canvas,
    /* [in] */ IRect* location,
    /* [in] */ IPaint* paint)
{
    assert(0 && "TODO");
    // canvas.drawPatch(this, location, paint);
    return NOERROR;
}

void NinePatch::DrawSoftware(
    /* [in] */ ICanvas* canvas,
    /* [in] */ IRectF* location,
    /* [in] */ IPaint* paint)
{
    assert(0 && "TODO");
    // nativeDraw(canvas.getNativeCanvasWrapper(), location, mBitmap.ni(), mNativeChunk,
    //         paint != null ? paint.mNativePaint : 0, canvas.mDensity, mBitmap.mDensity);
}

void NinePatch::DrawSoftware(
    /* [in] */ ICanvas* canvas,
    /* [in] */ IRect* location,
    /* [in] */ IPaint* paint)
{
    assert(0 && "TODO");
    // nativeDraw(canvas.getNativeCanvasWrapper(), location, mBitmap.ni(), mNativeChunk,
    //         paint != null ? paint.mNativePaint : 0, canvas.mDensity, mBitmap.mDensity);
}

ECode NinePatch::GetDensity(
    /* [out] */ Int32* density)
{
    assert(0 && "TODO");
    // return mBitmap.mDensity;
    return NOERROR;
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
    /* [out] */ Boolean * has)
{
    return mBitmap->HasAlpha(has);
}

ECode NinePatch::GetTransparentRegion(
    /* [in] */ IRect* bounds,
    /* [out] */ IRegion** region)
{
    assert(0 && "TODO");
    // Int64 r = nativeGetTransparentRegion(mBitmap.ni(), mNativeChunk, bounds);
    // return r != 0 ? new Region(r) : null;
    return NOERROR;
}

Boolean NinePatch::IsNinePatchChunk(
    /* [in] */ ArrayOf<Byte>* chunk)
{
    assert(0 && "TODO: need jni codes.");
    return FALSE;
}

ECode NinePatch::IsNinePatchChunk(
    /* [in] */ ArrayOf<Byte>* chunk,
    /* [out] */ Boolean* result)
{
    assert(0 && "TODO: need jni codes.");
    return NOERROR;
}

Int64 NinePatch::ValidateNinePatchChunk(
    /* [in] */ Int64 bitmap,
    /* [in] */ ArrayOf<Byte>* chunk)
{
    assert(0 && "TODO: need jni codes.");
    return -1;
}

void NinePatch::NativeFinalize(
    /* [in] */ Int64 chunk)
{
    assert(0 && "TODO: need jni codes.");
}

void NinePatch::NativeDraw(
    /* [in] */ Int64 canvas_instance,
    /* [in] */ IRectF* loc,
    /* [in] */ Int64 bitmap_instance,
    /* [in] */ Int64 c,
    /* [in] */ Int64 paint_instance_or_null,
    /* [in] */ Int32 destDensity,
    /* [in] */ Int32 srcDensity)
{
    assert(0 && "TODO: need jni codes.");
}

void NinePatch::NativeDraw(
    /* [in] */ Int64 canvas_instance,
    /* [in] */ IRect* loc,
    /* [in] */ Int64 bitmap_instance,
    /* [in] */ Int64 c,
    /* [in] */ Int64 paint_instance_or_null,
    /* [in] */ Int32 destDensity,
    /* [in] */ Int32 srcDensity)
{
    assert(0 && "TODO: need jni codes.");
}

Int64 NinePatch::NativeGetTransparentRegion(
    /* [in] */ Int64 bitmap,
    /* [in] */ Int64 chunk,
    /* [in] */ IRect* location)
{
    assert(0 && "TODO: need jni codes.");
    return -1;
}

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos
