
#include "ext/frameworkext.h"
#include "graphics/GraphicsNative.h"
#include "graphics/Canvas.h"
#include "graphics/CBitmap.h"
#include "graphics/CRect.h"
#include "graphics/CRectF.h"
#include "graphics/Paint.h"
#include "graphics/CMatrix.h"
#include "graphics/CPath.h"
#include "graphics/CRegion.h"
#include "graphics/CPicture.h"
#include "graphics/DrawFilter.h"
#include "graphics/TextLayout.h"
#include "graphics/TextLayoutCache.h"
#include <elastos/core/Math.h>
#include <elastos/utility/logging/Logger.h>
#include <skia/core/SkCanvas.h>
#include <skia/core/SkBitmap.h>
#include <skia/core/SkPaint.h>
#include <skia/core/SkRect.h>
#include <skia/core/SkDevice.h>
#include <skia/core/SkScalar.h>
#include <skia/core/SkMatrix.h>
#include <skia/core/SkGraphics.h>
#include <skia/core/SkShader.h>
#include <skia/core/SkTemplates.h>
#include <skia/core/SkDrawFilter.h>
#include <skia/images/SkImageRef_GlobalPool.h>
#include <skia/effects/SkPorterDuff.h>
#include <utils/RefBase.h>

using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Graphics {

// {CC4FB366-48F0-48FF-A6B6-670E64F46A7B}
extern const InterfaceID EIID_Canvas =
    { 0xcc4fb366, 0x48f0, 0x48ff, { 0xa6, 0xb6, 0x67, 0xe, 0x64, 0xf4, 0x6a, 0x7b } };

Canvas::Canvas()
    : mDensity(IBitmap::DENSITY_NONE)
    , mScreenDensity(IBitmap::DENSITY_NONE)
    , mSurfaceFormat(0)
{}

Canvas::~Canvas()
{
    // If the constructor threw an exception before setting mNativeCanvas, the native finalizer
    // must not be invoked.
    if (mNativeCanvas != 0) {
        NativeFinalizer(mNativeCanvas);
    }
}

ECode Canvas::Init()
{
    // 0 means no native bitmap
    mNativeCanvas = InitRaster(0);
    return NOERROR;
}

ECode Canvas::Init(
    /* [in] */ IBitmap* bitmap)
{
    Boolean isMutable = FALSE;
    if (bitmap->IsMutable(&isMutable), !isMutable) {
        Logger::E("Canvas", "Immutable bitmap passed to Canvas constructor");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    FAIL_RETURN(ThrowIfRecycled(bitmap));
    mNativeCanvas = InitRaster(((CBitmap*)bitmap)->Ni());
    mBitmap = (CBitmap*)bitmap;
    mDensity = ((CBitmap*)bitmap)->mDensity;
    return NOERROR;
}

ECode Canvas::Init(
    /* [in] */ Int32 nativeCanvas)
{
    if (nativeCanvas == 0) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    mNativeCanvas = nativeCanvas;
    mDensity = CBitmap::GetDefaultDensity();
    return NOERROR;
}

/**
 * Returns null.
 *
 * @deprecated This method is not supported and should not be invoked.
 *
 * @hide
 */
// AutoPtr<IGL> Canvas::GetGL()
// {
//     return NULL;
// }

/**
 * Indicates whether this Canvas uses hardware acceleration.
 *
 * Note that this method does not define what type of hardware acceleration
 * may or may not be used.
 *
 * @return True if drawing operations are hardware accelerated,
 *         false otherwise.
 */
ECode Canvas::IsHardwareAccelerated(
    /* [out] */ Boolean* isAccelerated)
{
    assert(isAccelerated != NULL);

    *isAccelerated = FALSE;
    return NOERROR;
}

/**
 * Specify a bitmap for the canvas to draw into.  As a side-effect, also
 * updates the canvas's target density to match that of the bitmap.
 *
 * @param bitmap Specifies a mutable bitmap for the canvas to draw into.
 *
 * @see #setDensity(int)
 * @see #getDensity()
 */
ECode Canvas::SetBitmap(
    /* [in] */ IBitmap* bitmap)
{
    Boolean result;
    if (IsHardwareAccelerated(&result), result) {
//        throw new RuntimeException("Can't set a bitmap device on a GL canvas");
        return E_RUNTIME_EXCEPTION;
    }

    Int32 pointer = 0;
    if (bitmap != NULL) {
        Boolean isMutable;
        bitmap->IsMutable(&isMutable);
        if (!isMutable) {
            return E_ILLEGAL_STATE_EXCEPTION;
        }
        FAIL_RETURN(ThrowIfRecycled(bitmap));
        mDensity = ((CBitmap*)bitmap)->mDensity;
        pointer =  ((CBitmap*)bitmap)->Ni();
    }

    NativeSetBitmap(mNativeCanvas, pointer);
    mBitmap = (CBitmap*)bitmap;
    return NOERROR;
}

/**
 * Set the viewport dimensions if this canvas is GL based. If it is not,
 * this method is ignored and no exception is thrown.
 *
 *  @param width    The width of the viewport
 *  @param height   The height of the viewport
 */
ECode Canvas::SetViewport(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    return NOERROR;
}

/**
 * Return true if the device that the current layer draws into is opaque
 * (i.e. does not support per-pixel alpha).
 *
 * @return true if the device that the current layer draws into is opaque
 */
ECode Canvas::IsOpaque(
    /* [out] */ Boolean* isOpaque)
{
    assert(isOpaque != NULL);

    // normal technique, rely on the device's bitmap for the answer
    *isOpaque = ((SkCanvas*)mNativeCanvas)->getDevice()->accessBitmap(false).isOpaque();
    return NOERROR;
}

/**
 * Returns the width of the current drawing layer
 *
 * @return the width of the current drawing layer
 */
ECode Canvas::GetWidth(
    /* [out] */ Int32* width)
{
    assert(width != NULL);

    *width = ((SkCanvas*)mNativeCanvas)->getDevice()->accessBitmap(false).width();
    return NOERROR;
}

/**
 * Returns the height of the current drawing layer
 *
 * @return the height of the current drawing layer
 */
ECode Canvas::GetHeight(
    /* [out] */ Int32* height)
{
    assert(height != NULL);

    *height = ((SkCanvas*)mNativeCanvas)->getDevice()->accessBitmap(false).height();
    return NOERROR;
}

/**
 * <p>Returns the target density of the canvas.  The default density is
 * derived from the density of its backing bitmap, or
 * {@link Bitmap#DENSITY_NONE} if there is not one.</p>
 *
 * @return Returns the current target density of the canvas, which is used
 * to determine the scaling factor when drawing a bitmap into it.
 *
 * @see #setDensity(int)
 * @see Bitmap#getDensity()
 */
ECode Canvas::GetDensity(
    /* [out] */ Int32* density)
{
    assert(density != NULL);

    *density = mDensity;
    return NOERROR;
}

/**
 * <p>Specifies the density for this Canvas' backing bitmap.  This modifies
 * the target density of the canvas itself, as well as the density of its
 * backing bitmap via {@link Bitmap#setDensity(int) Bitmap.setDensity(int)}.
 *
 * @param density The new target density of the canvas, which is used
 * to determine the scaling factor when drawing a bitmap into it.  Use
 * {@link Bitmap#DENSITY_NONE} to disable bitmap scaling.
 *
 * @see #getDensity()
 * @see Bitmap#setDensity(int)
 */
ECode Canvas::SetDensity(
    /* [in] */ Int32 density)
{
    if (mBitmap != NULL) {
        mBitmap->SetDensity(density);
    }
    mDensity = density;
    return NOERROR;
}

ECode Canvas::SetScreenDensity(
    /* [in] */ Int32 density)
{
    mScreenDensity = density;
    return NOERROR;
}

/**
 * Returns the maximum allowed width for bitmaps drawn with this canvas.
 * Attempting to draw with a bitmap wider than this value will result
 * in an error.
 *
 * @see #getMaximumBitmapHeight()
 */
ECode Canvas::GetMaximumBitmapWidth(
    /* [out] */ Int32* width)
{
    *width = MAXMIMUM_BITMAP_SIZE;
    return NOERROR;
}

/**
 * Returns the maximum allowed height for bitmaps drawn with this canvas.
 * Attempting to draw with a bitmap taller than this value will result
 * in an error.
 *
 * @see #getMaximumBitmapWidth()
 */
ECode Canvas::GetMaximumBitmapHeight(
    /* [out] */ Int32* height)
{
    *height = MAXMIMUM_BITMAP_SIZE;
    return NOERROR;
}

/**
 * Saves the current matrix and clip onto a private stack. Subsequent
 * calls to translate,scale,rotate,skew,concat or clipRect,clipPath
 * will all operate as usual, but when the balancing call to restore()
 * is made, those calls will be forgotten, and the settings that existed
 * before the save() will be reinstated.
 *
 * @return The value to pass to restoreToCount() to balance this save()
 */
ECode Canvas::Save(
    /* [out] */ Int32* count)
{
    assert(count != NULL);

    *count = ((SkCanvas*)mNativeCanvas)->save();
    return NOERROR;
}

/**
 * Based on saveFlags, can save the current matrix and clip onto a private
 * stack. Subsequent calls to translate,scale,rotate,skew,concat or
 * clipRect,clipPath will all operate as usual, but when the balancing
 * call to restore() is made, those calls will be forgotten, and the
 * settings that existed before the save() will be reinstated.
 *
 * @param saveFlags flag bits that specify which parts of the Canvas state
 *                  to save/restore
 * @return The value to pass to restoreToCount() to balance this save()
 */
ECode Canvas::Save(
    /* [in] */ Int32 saveFlags,
    /* [out] */ Int32* count)
{
    assert(count != NULL);

    *count = ((SkCanvas*)mNativeCanvas)->save((SkCanvas::SaveFlags)saveFlags);
    return NOERROR;
}

/**
 * This behaves the same as save(), but in addition it allocates an
 * offscreen bitmap. All drawing calls are directed there, and only when
 * the balancing call to restore() is made is that offscreen transfered to
 * the canvas (or the previous layer). Subsequent calls to translate,
 * scale, rotate, skew, concat or clipRect, clipPath all operate on this
 * copy. When the balancing call to restore() is made, this copy is
 * deleted and the previous matrix/clip state is restored.
 *
 * @param bounds May be null. The maximum size the offscreen bitmap
 *               needs to be (in local coordinates)
 * @param paint  This is copied, and is applied to the offscreen when
 *               restore() is called.
 * @param saveFlags  see _SAVE_FLAG constants
 * @return       value to pass to restoreToCount() to balance this save()
 */
ECode Canvas::SaveLayer(
    /* [in] */ IRectF* bounds,
    /* [in] */ IPaint* paint,
    /* [in] */ Int32 saveFlags,
    /* [out] */ Int32* count)
{
    assert(count != NULL);

    Int32 nativePaint = 0;
    if (paint != NULL) {
        nativePaint = ((Paint*)paint->Probe(EIID_Paint))->mNativePaint;
    }
    *count = NativeSaveLayer(mNativeCanvas, bounds, nativePaint, saveFlags);
    return NOERROR;
}

/**
 * Helper version of saveLayer() that takes 4 values rather than a RectF.
 */
ECode Canvas::SaveLayer(
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [in] */ IPaint* paint,
    /* [in] */ Int32 saveFlags,
    /* [out] */ Int32* count)
{
    assert(count != NULL);

    Int32 nativePaint = 0;
    if (paint != NULL) {
        nativePaint = ((Paint*)paint->Probe(EIID_Paint))->mNativePaint;
    }
    *count = NativeSaveLayer(mNativeCanvas, left, top, right, bottom,
            nativePaint, saveFlags);
    return NOERROR;
}

ECode Canvas::SaveLayerAlpha(
    /* [in] */ IRectF* bounds,
    /* [in] */ Int32 alpha,
    /* [in] */ Int32 saveFlags,
    /* [out] */ Int32* count)
{
    assert(count != NULL);

    alpha = Elastos::Core::Math::Min(255, Elastos::Core::Math::Max(0, alpha));
    *count = NativeSaveLayerAlpha(mNativeCanvas, bounds, alpha, saveFlags);
    return NOERROR;
}

ECode Canvas::SaveLayerAlpha(
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [in] */ Int32 alpha,
    /* [in] */ Int32 saveFlags,
    /* [out] */ Int32* count)
{
    assert(count != NULL);

    *count = NativeSaveLayerAlpha(mNativeCanvas, left, top, right, bottom,
            alpha, saveFlags);
    return NOERROR;
}

ECode Canvas::Restore()
{
    if (((SkCanvas*)mNativeCanvas)->getSaveCount() <= 1) {  // cannot restore anymore
        Logger::W("Canvas", "Underflow in restore");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    ((SkCanvas*)mNativeCanvas)->restore();
    return NOERROR;
}

ECode Canvas::GetSaveCount(
    /* [out] */ Int32* count)
{
    assert(count != NULL);

    *count = ((SkCanvas*)mNativeCanvas)->getSaveCount();
    return NOERROR;
}

ECode Canvas::RestoreToCount(
    /* [in] */ Int32 saveCount)
{
    if (saveCount < 1) {
        Logger::W("Canvas", "Underflow in restoreToCount");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    ((SkCanvas*)mNativeCanvas)->restoreToCount(saveCount);
    return NOERROR;
}

ECode Canvas::Translate(
    /* [in] */ Float dx,
    /* [in] */ Float dy)
{
    SkScalar dx_ = SkFloatToScalar(dx);
    SkScalar dy_ = SkFloatToScalar(dy);
    ((SkCanvas*)mNativeCanvas)->translate(dx_, dy_);
    return NOERROR;
}

ECode Canvas::Scale(
    /* [in] */ Float sx,
    /* [in] */ Float sy)
{
    SkScalar sx_ = SkFloatToScalar(sx);
    SkScalar sy_ = SkFloatToScalar(sy);
    ((SkCanvas*)mNativeCanvas)->scale(sx_, sy_);
    return NOERROR;
}

ECode Canvas::Scale(
    /* [in] */ Float sx,
    /* [in] */ Float sy,
    /* [in] */ Float px,
    /* [in] */ Float py)
{
    Translate(px, py);
    Scale(sx, sy);
    Translate(-px, -py);
    return NOERROR;
}

ECode Canvas::Rotate(
    /* [in] */ Float degrees)
{
    SkScalar degrees_ = SkFloatToScalar(degrees);
    ((SkCanvas*)mNativeCanvas)->rotate(degrees_);
    return NOERROR;
}

ECode Canvas::Rotate(
    /* [in] */ Float degrees,
    /* [in] */ Float px,
    /* [in] */ Float py)
{
    Translate(px, py);
    Rotate(degrees);
    Translate(-px, -py);
    return NOERROR;
}

ECode Canvas::Skew(
    /* [in] */ Float sx,
    /* [in] */ Float sy)
{
    SkScalar sx_ = SkFloatToScalar(sx);
    SkScalar sy_ = SkFloatToScalar(sy);
    ((SkCanvas*)mNativeCanvas)->skew(sx_, sy_);
    return NOERROR;
}

ECode Canvas::Concat(
    /* [in] */ IMatrix* matrix)
{
    assert(matrix != NULL);

    Int32 nativeMatrix = ((Matrix*)matrix->Probe(EIID_Matrix))->mNativeInstance;
    NativeConcat(mNativeCanvas, nativeMatrix);
    return NOERROR;
}

ECode Canvas::SetMatrix(
    /* [in] */ IMatrix* matrix)
{
    Int32 nativeMatrix = 0;
    if (matrix != NULL) {
        nativeMatrix = ((Matrix*)matrix->Probe(EIID_Matrix))->mNativeInstance;
    }
    NativeSetMatrix(mNativeCanvas, nativeMatrix);
    return NOERROR;
}

ECode Canvas::GetMatrix(
    /* [in, out] */ IMatrix* ctm)
{
    assert(ctm != NULL);

    Int32 nativeMatrix = ((Matrix*)ctm->Probe(EIID_Matrix))->mNativeInstance;
    NativeGetCTM(mNativeCanvas, nativeMatrix);
    return NOERROR;
}

ECode Canvas::GetMatrix(
    /* [out] */ IMatrix** metrix)
{
    assert(metrix != NULL);

    FAIL_RETURN(CMatrix::New(metrix));
    return GetMatrix(*metrix);
}

ECode Canvas::ClipRect(
    /* [in] */ IRectF* rect,
    /* [in] */ RegionOp op,
    /* [out] */ Boolean* isNotEmpty)
{
    assert(isNotEmpty != NULL);
    assert(rect != NULL);

    CRectF* rect_ = (CRectF*)rect;
    *isNotEmpty = NativeClipRect(mNativeCanvas,
            rect_->mLeft, rect_->mTop, rect_->mRight, rect_->mBottom, op);
    return NOERROR;
}

ECode Canvas::ClipRect(
    /* [in] */ IRect* rect,
    /* [in] */ RegionOp op,
    /* [out] */ Boolean* isNotEmpty)
{
    assert(isNotEmpty != NULL);
    assert(rect != NULL);

    CRect* rect_ = (CRect*)rect;
    *isNotEmpty = NativeClipRect(mNativeCanvas,
            rect_->mLeft, rect_->mTop, rect_->mRight, rect_->mBottom, op);
    return NOERROR;
}

ECode Canvas::ClipRect(
    /* [in] */ IRectF* rect,
    /* [out] */ Boolean* isNotEmpty)
{
    assert(isNotEmpty != NULL);
    assert(rect != NULL);

    CRectF* rect_ = (CRectF*)rect;
    SkRect tmp;
    tmp.set(SkFloatToScalar(rect_->mLeft), SkFloatToScalar(rect_->mTop),
            SkFloatToScalar(rect_->mRight), SkFloatToScalar(rect_->mBottom));
    *isNotEmpty = ((SkCanvas*)mNativeCanvas)->clipRect(tmp);
    return NOERROR;
}

ECode Canvas::ClipRect(
    /* [in] */ IRect* rect,
    /* [out] */ Boolean* isNotEmpty)
{
    assert(isNotEmpty != NULL);
    assert(rect != NULL);

    CRect* rect_ = (CRect*)rect;
    SkRect tmp;
    tmp.set(SkIntToScalar(rect_->mLeft), SkIntToScalar(rect_->mTop),
            SkIntToScalar(rect_->mRight), SkIntToScalar(rect_->mBottom));
    *isNotEmpty = ((SkCanvas*)mNativeCanvas)->clipRect(tmp);
    return NOERROR;
}

ECode Canvas::ClipRect(
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [in] */ RegionOp op,
    /* [out] */ Boolean* isNotEmpty)
{
    assert(isNotEmpty != NULL);

    *isNotEmpty = NativeClipRect(mNativeCanvas, left, top, right, bottom, op);
    return NOERROR;
}

ECode Canvas::ClipRect(
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [out] */ Boolean* isNotEmpty)
{
    assert(isNotEmpty != NULL);

    SkRect r;
    r.set(SkFloatToScalar(left), SkFloatToScalar(top),
          SkFloatToScalar(right), SkFloatToScalar(bottom));
    *isNotEmpty = ((SkCanvas*)mNativeCanvas)->clipRect(r);
    return NOERROR;
}

ECode Canvas::ClipRect(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom,
    /* [out] */ Boolean* isNotEmpty)
{
    assert(isNotEmpty != NULL);

    SkRect  r;
    r.set(SkIntToScalar(left), SkIntToScalar(top),
          SkIntToScalar(right), SkIntToScalar(bottom));
    *isNotEmpty = ((SkCanvas*)mNativeCanvas)->clipRect(r);
    return NOERROR;
}

ECode Canvas::ClipPath(
    /* [in] */ IPath* path,
    /* [in] */ RegionOp op,
    /* [out] */ Boolean* isNotEmpty)
{
    assert(isNotEmpty != NULL);
    assert(path != NULL);

    Int32 nativePath = ((CPath*)path)->Ni();
    *isNotEmpty = NativeClipPath(mNativeCanvas, nativePath, op);
    return NOERROR;
}

ECode Canvas::ClipPath(
    /* [in] */ IPath* path,
    /* [out] */ Boolean* isNotEmpty)
{
    return ClipPath(path, RegionOp_INTERSECT, isNotEmpty);
}

ECode Canvas::ClipRegion(
    /* [in] */ IRegion* region,
    /* [in] */ RegionOp op,
    /* [out] */ Boolean* isNotEmpty)
{
    assert(isNotEmpty != NULL);
    assert(region != NULL);

    Int32 nativeRegion = ((CRegion*)region)->Ni();
    *isNotEmpty = NativeClipRegion(mNativeCanvas, nativeRegion, op);
    return NOERROR;
}

ECode Canvas::ClipRegion(
    /* [in] */ IRegion* region,
    /* [out] */ Boolean* isNotEmpty)
{
    return ClipRegion(region, RegionOp_INTERSECT, isNotEmpty);
}

ECode Canvas::GetDrawFilter(
    /* [out] */ IDrawFilter** filter)
{
    assert(filter != NULL);

    *filter = mDrawFilter;
    REFCOUNT_ADD(*filter);
    return NOERROR;
}

ECode Canvas::SetDrawFilter(
    /* [in] */ IDrawFilter* filter)
{
    Int32 nativeFilter = 0;
    if (filter != NULL) {
        nativeFilter = ((DrawFilter*)filter->Probe(EIID_DrawFilter))->mNativeInstance;
    }
    mDrawFilter = filter;
    NativeSetDrawFilter(mNativeCanvas, nativeFilter);
    return NOERROR;
}

ECode Canvas::QuickReject(
    /* [in] */ IRectF* rect,
    /* [in] */ CanvasEdgeType type,
    /* [out] */ Boolean* isNotIntersect)
{
    assert(isNotIntersect != NULL);

    *isNotIntersect = NativeQuickReject(mNativeCanvas, rect, type);
    return NOERROR;
}

ECode Canvas::QuickReject(
    /* [in] */ IPath* path,
    /* [in] */ CanvasEdgeType type,
    /* [out] */ Boolean* isNotIntersect)
{
    assert(isNotIntersect != NULL);
    assert(path != NULL);

    Int32 nativePath = ((CPath*)path)->Ni();
    *isNotIntersect = NativeQuickReject(mNativeCanvas, nativePath, type);
    return NOERROR;
}

ECode Canvas::QuickReject(
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [in] */ CanvasEdgeType type,
    /* [out] */ Boolean* isNotIntersect)
{
    assert(isNotIntersect != NULL);

    *isNotIntersect = NativeQuickReject(mNativeCanvas, left, top, right, bottom, type);
    return NOERROR;
}

ECode Canvas::GetClipBounds(
    /* [in] */ IRect* bounds,
    /* [out] */ Boolean* isNotEmpty)
{
    assert(isNotEmpty != NULL);

    *isNotEmpty = NativeGetClipBounds(mNativeCanvas, bounds);
    return NOERROR;
}

ECode Canvas::GetClipBounds(
    /* [out] */ IRect** bounds)
{
    assert(bounds != NULL);

    FAIL_RETURN(CRect::New(bounds));
    Boolean isNotEmpty;
    return GetClipBounds(*bounds, &isNotEmpty);
}

ECode Canvas::DrawRGB(
    /* [in] */ Int32 r,
    /* [in] */ Int32 g,
    /* [in] */ Int32 b)
{
    NativeDrawRGB(mNativeCanvas, r, g, b);
    return NOERROR;
}

ECode Canvas::DrawARGB(
    /* [in] */ Int32 a,
    /* [in] */ Int32 r,
    /* [in] */ Int32 g,
    /* [in] */ Int32 b)
{
    NativeDrawARGB(mNativeCanvas, a, r, g, b);
    return NOERROR;
}

ECode Canvas::DrawColor(
    /* [in] */ Int32 color)
{
    NativeDrawColor(mNativeCanvas, color);
    return NOERROR;
}

ECode Canvas::DrawColor(
    /* [in] */ Int32 color,
    /* [in] */ PorterDuffMode mode)
{
    NativeDrawColor(mNativeCanvas, color, mode);
    return NOERROR;
}

ECode Canvas::DrawPaint(
    /* [in] */ IPaint* paint)
{
    assert(paint != NULL);

    NativeDrawPaint(mNativeCanvas, ((Paint*)paint->Probe(EIID_Paint))->mNativePaint);
    return NOERROR;
}

static ECode DoPoints(
    /* [in] */ SkCanvas* canvas,
    /* [in] */ const ArrayOf<Float>& _pts,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ IPaint* _paint,
    /* [in] */ SkCanvas::PointMode mode);

ECode Canvas::DrawPoints(
    /* [in] */ const ArrayOf<Float>& pts,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ IPaint* paint)
{
    return DoPoints((SkCanvas*)mNativeCanvas, pts, offset, count, paint,
            SkCanvas::kPoints_PointMode);
}

ECode Canvas::DrawPoints(
    /* [in] */ const ArrayOf<Float>& pts,
    /* [in] */ IPaint* paint)
{
    return DrawPoints(pts, 0, pts.GetLength(), paint);
}

ECode Canvas::DrawPoint(
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ IPaint* paint)
{
    assert(paint != NULL);
    const SkPaint& nativePaint = *(SkPaint*)((Paint*)paint->Probe(EIID_Paint))->mNativePaint;
    ((SkCanvas*)mNativeCanvas)->drawPoint(SkFloatToScalar(x), SkFloatToScalar(y), nativePaint);
    return NOERROR;
}

ECode Canvas::DrawLine(
    /* [in] */ Float startX,
    /* [in] */ Float startY,
    /* [in] */ Float stopX,
    /* [in] */ Float stopY,
    /* [in] */ IPaint* paint)
{
    assert(paint != NULL);
    Int32 nativePaint = ((Paint*)paint->Probe(EIID_Paint))->mNativePaint;
    NativeDrawLine(mNativeCanvas, startX, startY, stopX, stopY, nativePaint);
    return NOERROR;
}

ECode Canvas::DrawLines(
    /* [in] */ const ArrayOf<Float>& pts,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ IPaint* paint)
{
    return DoPoints((SkCanvas*)mNativeCanvas, pts, offset, count, paint,
            SkCanvas::kLines_PointMode);
}

ECode Canvas::DrawLines(
    /* [in] */ const ArrayOf<Float> & pts,
    /* [in] */ IPaint* paint)
{
    return DrawLines(pts, 0, pts.GetLength(), paint);
}

ECode Canvas::DrawRect(
    /* [in] */ IRectF* rect,
    /* [in] */ IPaint* paint)
{
    assert(paint != NULL);

    Int32 nativePaint = ((Paint*)paint->Probe(EIID_Paint))->mNativePaint;
    NativeDrawRect(mNativeCanvas, rect, nativePaint);
    return NOERROR;
}

ECode Canvas::DrawRect(
    /* [in] */ IRect* r,
    /* [in] */ IPaint* paint)
{
    assert(r != NULL);

    CRect* r_ = (CRect*)r;
    return DrawRect(r_->mLeft, r_->mTop, r_->mRight, r_->mBottom, paint);
}

ECode Canvas::DrawRect(
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [in] */ IPaint* paint)
{
    assert(paint != NULL);

    Int32 nativePaint = ((Paint*)paint->Probe(EIID_Paint))->mNativePaint;
    NativeDrawRect(mNativeCanvas, left, top, right, bottom, nativePaint);
    return NOERROR;
}

ECode Canvas::DrawOval(
    /* [in] */ IRectF* oval,
    /* [in] */ IPaint* paint)
{
    assert(paint != NULL);

    if (oval == NULL) {
//        throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }

    Int32 nativePaint = ((Paint*)paint->Probe(EIID_Paint))->mNativePaint;
    NativeDrawOval(mNativeCanvas, oval, nativePaint);
    return NOERROR;
}

ECode Canvas::DrawCircle(
    /* [in] */ Float cx,
    /* [in] */ Float cy,
    /* [in] */ Float radius,
    /* [in] */ IPaint* paint)
{
    assert(paint != NULL);

    Int32 nativePaint = ((Paint*)paint->Probe(EIID_Paint))->mNativePaint;
    NativeDrawCircle(mNativeCanvas, cx, cy, radius, nativePaint);
    return NOERROR;
}

ECode Canvas::DrawArc(
    /* [in] */ IRectF* oval,
    /* [in] */ Float startAngle,
    /* [in] */ Float sweepAngle,
    /* [in] */ Boolean useCenter,
    /* [in] */ IPaint* paint)
{
    assert(paint != NULL);

    if (oval == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }

    Int32 nativePaint = ((Paint*)paint->Probe(EIID_Paint))->mNativePaint;
    NativeDrawArc(mNativeCanvas, oval, startAngle, sweepAngle,
            useCenter, nativePaint);
    return NOERROR;
}

ECode Canvas::DrawRoundRect(
    /* [in] */ IRectF* rect,
    /* [in] */ Float rx,
    /* [in] */ Float ry,
    /* [in] */ IPaint* paint)
{
    assert(paint != NULL);

    if (rect == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }
    Int32 nativePaint = ((Paint*)paint->Probe(EIID_Paint))->mNativePaint;
    NativeDrawRoundRect(mNativeCanvas, rect, rx, ry, nativePaint);
    return NOERROR;
}

ECode Canvas::DrawPath(
    /* [in] */ IPath* path,
    /* [in] */ IPaint* paint)
{
    assert(path != NULL);
    assert(paint != NULL);

    Int32 nativePath = ((CPath*)path)->Ni();
    Int32 nativePaint = ((Paint*)paint->Probe(EIID_Paint))->mNativePaint;
    NativeDrawPath(mNativeCanvas, nativePath, nativePaint);
    return NOERROR;
}

ECode Canvas::ThrowIfRecycled(
    /* [in] */ IBitmap* bitmap)
{
    assert(bitmap != NULL);

    Boolean isRecycled;
    bitmap->IsRecycled(&isRecycled);
    if (isRecycled) {
//        throw new RuntimeException(
//                    "Canvas: trying to use a recycled bitmap " + bitmap);
        return E_RUNTIME_EXCEPTION;
    }
    return NOERROR;
}

ECode Canvas::DrawPatch(
    /* [in] */ IBitmap* bitmap,
    /* [in] */ const ArrayOf<Byte>& chunks,
    /* [in] */ IRectF* dst,
    /* [in] */ IPaint* paint)
{
    return NOERROR;
}

ECode Canvas::DrawBitmap(
    /* [in] */ IBitmap* bitmap,
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ IPaint* paint)
{
    assert(bitmap != NULL);

    FAIL_RETURN(ThrowIfRecycled(bitmap));
    Int32 nativeBitmap = ((CBitmap*)bitmap)->Ni();
    Int32 nativePaint = 0;
    if (paint != NULL) {
        nativePaint = ((Paint*)paint->Probe(EIID_Paint))->mNativePaint;
    }
    NativeDrawBitmap(mNativeCanvas, nativeBitmap, left, top,
            nativePaint, mDensity, mScreenDensity,
            ((CBitmap*)bitmap)->mDensity);
    return NOERROR;
}

ECode Canvas::DrawBitmap(
    /* [in] */ IBitmap* bitmap,
    /* [in] */ IRect* src,
    /* [in] */ IRectF* dst,
    /* [in] */ IPaint* paint)
{
    assert(bitmap != NULL);

    if (dst == NULL) {
//        throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }
    FAIL_RETURN(ThrowIfRecycled(bitmap));
    Int32 nativeBitmap = ((CBitmap*)bitmap)->Ni();
    Int32 nativePaint = 0;
    if (paint != NULL) {
        nativePaint = ((Paint*)paint->Probe(EIID_Paint))->mNativePaint;
    }
    NativeDrawBitmap(mNativeCanvas, nativeBitmap, src, dst,
            nativePaint, mScreenDensity, ((CBitmap*)bitmap)->mDensity);
    return NOERROR;
}

ECode Canvas::DrawBitmap(
    /* [in] */ IBitmap* bitmap,
    /* [in] */ IRect* src,
    /* [in] */ IRect* dst,
    /* [in] */ IPaint* paint)
{
    assert(bitmap != NULL);

    if (dst == NULL) {
//        throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }
    FAIL_RETURN(ThrowIfRecycled(bitmap));
    Int32 nativeBitmap = ((CBitmap*)bitmap)->Ni();
    Int32 nativePaint = 0;
    if (paint != NULL) {
        nativePaint = ((Paint*)paint->Probe(EIID_Paint))->mNativePaint;
    }
    NativeDrawBitmap(mNativeCanvas, nativeBitmap, src, dst,
            nativePaint, mScreenDensity, ((CBitmap*)bitmap)->mDensity);
    return NOERROR;
}

ECode Canvas::DrawBitmap(
    /* [in] */ const ArrayOf<Int32>& colors,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 stride,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Boolean hasAlpha,
    /* [in] */ IPaint* paint)
{
    // check for valid input
    if (width < 0) {
//        throw new IllegalArgumentException("width must be >= 0");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (height < 0) {
//        throw new IllegalArgumentException("height must be >= 0");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (Elastos::Core::Math::Abs(stride) < width) {
//        throw new IllegalArgumentException("abs(stride) must be >= width");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Int32 lastScanline = offset + (height - 1) * stride;
    Int32 length = colors.GetLength();
    if (offset < 0 || (offset + width > length) || lastScanline < 0
            || (lastScanline + width > length)) {
//        throw new ArrayIndexOutOfBoundsException();
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    // quick escape if there's nothing to draw
    if (width == 0 || height == 0) {
        return NOERROR;
    }
    Int32 nativePaint = 0;
    if (paint != NULL) {
        nativePaint = ((Paint*)paint->Probe(EIID_Paint))->mNativePaint;
    }
    // punch down to native for the actual draw
    NativeDrawBitmap(mNativeCanvas, colors, offset, stride, x, y, width,
            height, hasAlpha, nativePaint);
    return NOERROR;
}

ECode Canvas::DrawBitmap(
    /* [in] */ const ArrayOf<Int32> & colors,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 stride,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Boolean hasAlpha,
    /* [in] */ IPaint* paint)
{
    // call through to the common float version
    return DrawBitmap(colors, offset, stride, (Float)x, (Float)y, width, height,
            hasAlpha, paint);
}

ECode Canvas::DrawBitmap(
    /* [in] */ IBitmap* bitmap,
    /* [in] */ IMatrix* matrix,
    /* [in] */ IPaint* paint)
{
    assert(bitmap != NULL);
    assert(matrix != NULL);

    Int32 nativeBitmap = ((CBitmap*)bitmap)->Ni();
    Int32 nativeMatrix = ((CMatrix*)matrix)->Ni();
    Int32 nativePaint = 0;
    if (paint != NULL) {
        nativePaint = ((Paint*)paint->Probe(EIID_Paint))->mNativePaint;
    }
    NativeDrawBitmapMatrix(mNativeCanvas, nativeBitmap, nativeMatrix, nativePaint);
    return NOERROR;
}

ECode Canvas::CheckRange(
    /* [in] */ Int32 length,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count)
{
    if ((offset | count) < 0 || offset + count > length) {
//        throw new ArrayIndexOutOfBoundsException();
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    return NOERROR;
}

ECode Canvas::DrawBitmapMesh(
    /* [in] */ IBitmap* bitmap,
    /* [in] */ Int32 meshWidth,
    /* [in] */ Int32 meshHeight,
    /* [in] */ const ArrayOf<Float>& verts,
    /* [in] */ Int32 vertOffset,
    /* [in] */ ArrayOf<Int32>* colors,
    /* [in] */ Int32 colorOffset,
    /* [in] */ IPaint* paint)
{
    assert(bitmap != NULL);

    if ((meshWidth | meshHeight | vertOffset | colorOffset) < 0) {
//        throw new ArrayIndexOutOfBoundsException();
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    if (meshWidth == 0 || meshHeight == 0) {
        return NOERROR;
    }
    Int32 count = (meshWidth + 1) * (meshHeight + 1);
    // we mul by 2 since we need two floats per vertex
    FAIL_RETURN(CheckRange(verts.GetLength(), vertOffset, count * 2));
    if (colors != NULL) {
        // no mul by 2, since we need only 1 color per vertex
        FAIL_RETURN(CheckRange(colors->GetLength(), colorOffset, count));
    }
    Int32 nativeBitmap = ((CBitmap*)bitmap)->Ni();
    Int32 nativePaint = 0;
    if (paint != NULL) {
        nativePaint = ((Paint*)paint->Probe(EIID_Paint))->mNativePaint;
    }
    NativeDrawBitmapMesh(mNativeCanvas, nativeBitmap, meshWidth, meshHeight,
            verts, vertOffset, colors, colorOffset, nativePaint);
    return NOERROR;
}

ECode Canvas::DrawVertices(
    /* [in] */ CanvasVertexMode mode,
    /* [in] */ Int32 vertexCount,
    /* [in] */ const ArrayOf<Float>& verts,
    /* [in] */ Int32 vertOffset,
    /* [in] */ ArrayOf<Float>* texs,
    /* [in] */ Int32 texOffset,
    /* [in] */ ArrayOf<Int32>* colors,
    /* [in] */ Int32 colorOffset,
    /* [in] */ ArrayOf<Int16>* indices,
    /* [in] */ Int32 indexOffset,
    /* [in] */ Int32 indexCount,
    /* [in] */ IPaint* paint)
{
    assert(paint != NULL);

    FAIL_RETURN(CheckRange(verts.GetLength(), vertOffset, vertexCount));
    if (texs != NULL) {
        FAIL_RETURN(CheckRange(texs->GetLength(), texOffset, vertexCount));
    }
    if (colors != NULL) {
        FAIL_RETURN(CheckRange(colors->GetLength(), colorOffset, vertexCount));
    }
    if (indices != NULL) {
        FAIL_RETURN(CheckRange(indices->GetLength(), indexOffset, indexCount));
    }
    Int32 nativePaint = ((Paint*)paint->Probe(EIID_Paint))->mNativePaint;
    NativeDrawVertices(mNativeCanvas, mode,
            vertexCount, verts, vertOffset, texs, texOffset, colors,
            colorOffset, indices, indexOffset, indexCount, nativePaint);
    return NOERROR;
}

ECode Canvas::DrawText(
    /* [in] */ const ArrayOf<Char32>& text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ IPaint* paint)
{
    assert(paint != NULL);

    if ((index | count | (index + count) |
        (text.GetLength() - index - count)) < 0) {
//        throw new IndexOutOfBoundsException();
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    Int32 nativePaint = ((Paint*)paint->Probe(EIID_Paint))->mNativePaint;
    NativeDrawText(mNativeCanvas, text, index, count, x, y,
            ((Paint*)paint->Probe(EIID_Paint))->mBidiFlags, nativePaint);
    return NOERROR;
}

ECode Canvas::DrawText(
    /* [in] */ const String& text,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ IPaint* paint)
{
    assert(paint != NULL);

    if (text.IsNullOrEmpty()) {
        return NOERROR;
    }

    Paint* p = ((Paint*)paint->Probe(EIID_Paint));
    assert(p != NULL && "Paint cannot be null.");
    NativeDrawText(mNativeCanvas, text, 0, text.GetLength(), x, y,
            p->mBidiFlags, p->mNativePaint);
    return NOERROR;
}

ECode Canvas::DrawText(
    /* [in] */ const String& text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ IPaint* paint)
{
    assert(paint != NULL);

    if (text.IsNullOrEmpty()) {
        return NOERROR;
    }

    if ((start | end | (end - start) | (text.GetLength() - end)) < 0) {
//        throw new IndexOutOfBoundsException();
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    Int32 nativePaint = ((Paint*)paint->Probe(EIID_Paint))->mNativePaint;
    NativeDrawText(mNativeCanvas, text, start, end, x, y,
            ((Paint*)paint->Probe(EIID_Paint))->mBidiFlags, nativePaint);
    return NOERROR;
}

ECode Canvas::DrawText(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ IPaint* paint)
{
//    if (text instanceof String || text instanceof SpannedString ||
//        text instanceof SpannableString) {
//        native_drawText(mNativeCanvas, text.toString(), start, end, x, y,
//                        paint.mNativePaint);
//    }
//    else if (text instanceof GraphicsOperations) {
//        ((GraphicsOperations) text).drawText(this, start, end, x, y,
//                                                 paint);
//    }
//    else {
//        char[] buf = TemporaryBuffer.obtain(end - start);
//        TextUtils.getChars(text, start, end, buf, 0);
//        drawText(buf, 0, end - start, x, y, paint);
//        TemporaryBuffer.recycle(buf);
//    }

    if (text) {
        AutoPtr<ICharSequence> subSeq;
        text->SubSequence(start, end, (ICharSequence**)&subSeq);
        if (subSeq) {
            String str;
            subSeq->ToString(&str);
            return DrawText(str, 0, str.GetLength(), x, y, paint);
        }
    }

    return NOERROR;
}

ECode Canvas::DrawTextRun(
    /* [in] */ const ArrayOf<Char32>& text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [in] */ Int32 contextIndex,
    /* [in] */ Int32 contextCount,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Int32 dir,
    /* [in] */ IPaint* paint)
{
    if (paint == NULL) {
//        throw new NullPointerException("paint is null");
        return E_NULL_POINTER_EXCEPTION;
    }
    if ((index | count | (text.GetLength() - index - count)) < 0) {
//        throw new IndexOutOfBoundsException();
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    if (dir != ICanvas::DIRECTION_LTR && dir != ICanvas::DIRECTION_RTL) {
//        throw new IllegalArgumentException("unknown dir: " + dir);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Int32 nativePaint = ((Paint*)paint->Probe(EIID_Paint))->mNativePaint;
    NativeDrawTextRun(mNativeCanvas, text, index, count,
        contextIndex, contextCount, x, y, dir, nativePaint);
     return NOERROR;
}

ECode Canvas::DrawTextRun(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 contextStart,
    /* [in] */ Int32 contextEnd,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Int32 dir,
    /* [in] */ IPaint* paint)
{
    if (text == NULL) {
//        throw new NullPointerException("text is null");
        return E_NULL_POINTER_EXCEPTION;
    }
    if (paint == NULL) {
//        throw new NullPointerException("paint is null");
        return E_NULL_POINTER_EXCEPTION;
    }
    Int32 length;
    if ((start | end | (end - start) | (text->GetLength(&length), length - end)) < 0) {
//        throw new IndexOutOfBoundsException();
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    Int32 flags = dir == 0 ? 0 : 1;

//    if (text instanceof String || text instanceof SpannedString ||
//            text instanceof SpannableString) {
//        native_drawTextRun(mNativeCanvas, text.toString(), start, end,
//                contextStart, contextEnd, x, y, flags, paint.mNativePaint);
//    } else if (text instanceof GraphicsOperations) {
//        ((GraphicsOperations) text).drawTextRun(this, start, end,
//                contextStart, contextEnd, x, y, flags, paint);
//    } else {
//        Int32 contextLen = contextEnd - contextStart;
//        Int32 len = end - start;
//        char[] buf = TemporaryBuffer.obtain(contextLen);
//        TextUtils.getChars(text, contextStart, contextEnd, buf, 0);
//        native_drawTextRun(mNativeCanvas, buf, start - contextStart, len,
//                0, contextLen, x, y, flags, paint.mNativePaint);
//        TemporaryBuffer.recycle(buf);
//    }
    //TODO:
    //assert(0);
//    PRINT_FILE_LINE_EX("TODO");
    String str;
    text->ToString(&str);
    Int32 nativePaint = ((Paint*)paint->Probe(EIID_Paint))->mNativePaint;
    NativeDrawTextRun(mNativeCanvas, str, start, end,
                contextStart, contextEnd, x, y, flags, nativePaint);
     return NOERROR;
}

ECode Canvas::DrawPosText(
    /* [in] */ const ArrayOf<Char32>& text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [in] */ const ArrayOf<Float>& pos,
    /* [in] */ IPaint* paint)
{
    assert(paint != NULL);

    if (index < 0 || index + count > text.GetLength()
            || count * 2 > pos.GetLength()) {
//        throw new IndexOutOfBoundsException();
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    Int32 nativePaint = ((Paint*)paint->Probe(EIID_Paint))->mNativePaint;
    NativeDrawPosText(mNativeCanvas, text, index, count, pos, nativePaint);
    return NOERROR;
}

ECode Canvas::DrawPosText(
    /* [in] */ const String& text,
    /* [in] */ const ArrayOf<Float>& pos,
    /* [in] */ IPaint* paint)
{
    assert(paint != NULL);

    if ((text.GetLength() * 2) > pos.GetLength()) {
//        throw new ArrayIndexOutOfBoundsException();
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    Int32 nativePaint = ((Paint*)paint->Probe(EIID_Paint))->mNativePaint;
    NativeDrawPosText(mNativeCanvas, text, pos, nativePaint);
    return NOERROR;
}

ECode Canvas::DrawTextOnPath(
    /* [in] */ const ArrayOf<Char32>& text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [in] */ IPath* path,
    /* [in] */ Float hOffset,
    /* [in] */ Float vOffset,
    /* [in] */ IPaint* paint)
{
    assert(path != NULL);
    assert(paint != NULL);

    if (index < 0 || index + count > text.GetLength()) {
//        throw new ArrayIndexOutOfBoundsException();
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    Int32 nativePath = ((CPath*)path)->Ni();
    Int32 nativePaint = ((Paint*)paint->Probe(EIID_Paint))->mNativePaint;
    NativeDrawTextOnPath(mNativeCanvas, text, index, count,
            nativePath, hOffset, vOffset, ((Paint*)paint->Probe(EIID_Paint))->mBidiFlags, nativePaint);
    return NOERROR;
}

ECode Canvas::DrawTextOnPath(
    /* [in] */ const String& text,
    /* [in] */ IPath* path,
    /* [in] */ Float hOffset,
    /* [in] */ Float vOffset,
    /* [in] */ IPaint* paint)
{
    assert(path != NULL);
    assert(paint != NULL);

    if (!text.IsNullOrEmpty()) {
        Int32 nativePath = ((CPath*)path)->Ni();
        Int32 nativePaint = ((Paint*)paint->Probe(EIID_Paint))->mNativePaint;
        NativeDrawTextOnPath(mNativeCanvas, text, nativePath,
                hOffset, vOffset, ((Paint*)paint->Probe(EIID_Paint))->mBidiFlags, nativePaint);
    }
    return NOERROR;
}

ECode Canvas::DrawPicture(
    /* [in] */ IPicture* picture)
{
    assert(picture != NULL);
    picture->EndRecording();
    Int32 nativePicture = ((CPicture*)picture)->Ni();
    NativeDrawPicture(mNativeCanvas, nativePicture);
    return NOERROR;
}

ECode Canvas::DrawPicture(
    /* [in] */ IPicture* picture,
    /* [in] */ IRectF* dst)
{
    Int32 count;
    Save(&count);
    Translate(((CRectF*)dst)->mLeft, ((CRectF*)dst)->mTop);
    Int32 pictureWidth, pictureHeight;
    picture->GetWidth(&pictureWidth);
    picture->GetHeight(&pictureHeight);
    if (pictureWidth > 0 && pictureHeight > 0) {
        Float rectfWidth, rectfHeight;
        dst->GetWidth(&rectfWidth);
        dst->GetHeight(&rectfHeight);
        Scale(rectfWidth / pictureWidth, rectfHeight / pictureHeight);
    }
    DrawPicture(picture);
    Restore();
    return NOERROR;
}

ECode Canvas::DrawPicture(
    /* [in] */ IPicture* picture,
    /* [in] */ IRect* dst)
{
    Int32 count;
    Save(&count);
    Translate(((CRect*)dst)->mLeft, ((CRect*)dst)->mTop);
    Int32 pictureWidth, pictureHeight;
    picture->GetWidth(&pictureWidth);
    picture->GetHeight(&pictureHeight);
    if (pictureWidth > 0 && pictureHeight > 0) {
        Int32 rectWidth, rectHeight;
        dst->GetWidth(&rectWidth);
        dst->GetHeight(&rectHeight);
        Scale((Float)rectWidth / pictureWidth,
                (Float)rectHeight / pictureHeight);
    }
    DrawPicture(picture);
    Restore();
    return NOERROR;
}

void Canvas::FreeCaches()
{
    // these are called in no particular order
    SkImageRef_GlobalPool::SetRAMUsed(0);
    SkGraphics::PurgeFontCache();
}

/**
 * Free up text layout caches
 *
 * @hide
 */
void Canvas::FreeTextLayoutCaches()
{
    android::TextLayoutEngine::getInstance().purgeCaches();
}

Int32 Canvas::InitRaster(
    /* [in] */ Int32 bitmap)
{
    return bitmap ? (Int32)new SkCanvas(*(SkBitmap*)bitmap) : (Int32)new SkCanvas;
}

void Canvas::NativeSetBitmap(
    /* [in] */ Int32 canvas,
    /* [in] */ Int32 bitmap)
{
    if (bitmap) {
        ((SkCanvas*)canvas)->setBitmapDevice(*(SkBitmap*)bitmap);
    }
    else {
        ((SkCanvas*)canvas)->setDevice(NULL);
    }
}

Int32 Canvas::NativeSaveLayer(
    /* [in] */ Int32 canvas,
    /* [in] */ IRectF* bounds,
    /* [in] */ Int32 paint,
    /* [in] */ Int32 flags)
{
    SkRect* bounds_ = NULL;
    SkRect storage;
    if (bounds != NULL) {
        GraphicsNative::IRectF2SkRect(bounds, &storage);
        bounds_ = &storage;
    }
    return ((SkCanvas*)canvas)->saveLayer(bounds_, (SkPaint*)paint, (SkCanvas::SaveFlags)flags);
}

Int32 Canvas::NativeSaveLayer(
    /* [in] */ Int32 canvas,
    /* [in] */ Float l,
    /* [in] */ Float t,
    /* [in] */ Float r,
    /* [in] */ Float b,
    /* [in] */ Int32 paint,
    /* [in] */ Int32 flags)
{
    SkRect bounds;
    bounds.set(SkFloatToScalar(l), SkFloatToScalar(t), SkFloatToScalar(r),
               SkFloatToScalar(b));
    return ((SkCanvas*)canvas)->saveLayer(&bounds, (SkPaint*)paint, (SkCanvas::SaveFlags)flags);
}

Int32 Canvas::NativeSaveLayerAlpha(
    /* [in] */ Int32 canvas,
    /* [in] */ IRectF* bounds,
    /* [in] */ Int32 alpha,
    /* [in] */ Int32 flags)
{
    SkRect* bounds_ = NULL;
    SkRect storage;
    if (bounds != NULL) {
        GraphicsNative::IRectF2SkRect(bounds, &storage);
        bounds_ = &storage;
    }
    return ((SkCanvas*)canvas)->saveLayerAlpha(bounds_, alpha, (SkCanvas::SaveFlags)flags);
}

Int32 Canvas::NativeSaveLayerAlpha(
    /* [in] */Int32 canvas,
    /* [in] */ Float l,
    /* [in] */ Float t,
    /* [in] */ Float r,
    /* [in] */ Float b,
    /* [in] */ Int32 alpha,
    /* [in] */ Int32 flags)
{
    SkRect bounds;
    bounds.set(SkFloatToScalar(l), SkFloatToScalar(t),
               SkFloatToScalar(r), SkFloatToScalar(b));
    return ((SkCanvas*)canvas)->saveLayerAlpha(&bounds, alpha, (SkCanvas::SaveFlags)flags);
}

void Canvas::NativeConcat(
    /* [in] */ Int32 canvas,
    /* [in] */ Int32 matrix)
{
    ((SkCanvas*)canvas)->concat(*(SkMatrix*)matrix);
}

void Canvas::NativeSetMatrix(
    /* [in] */ Int32 canvas,
    /* [in] */ Int32 matrix)
{
    if (0 == matrix) {
        ((SkCanvas*)canvas)->resetMatrix();
    }
    else {
        ((SkCanvas*)canvas)->setMatrix(*(SkMatrix*)matrix);
    }
}

Boolean Canvas::NativeClipRect(
    /* [in] */ Int32 canvas,
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [in] */ RegionOp op)
{
    SkRect rect;
    rect.set(SkFloatToScalar(left), SkFloatToScalar(top),
             SkFloatToScalar(right), SkFloatToScalar(bottom));
    return ((SkCanvas*)canvas)->clipRect(rect, (SkRegion::Op)op);
}

Boolean Canvas::NativeClipPath(
    /* [in] */ Int32 canvas,
    /* [in] */ Int32 path,
    /* [in] */ RegionOp op)
{
    return ((SkCanvas*)canvas)->clipPath(*(SkPath*)path, (SkRegion::Op)op);
}

Boolean Canvas::NativeClipRegion(
    /* [in] */ Int32 canvas,
    /* [in] */ Int32 region,
    /* [in] */ RegionOp op)
{
    return ((SkCanvas*)canvas)->clipRegion(*(SkRegion*)region, (SkRegion::Op)op);
}

void Canvas::NativeSetDrawFilter(
    /* [in] */ Int32 canvas,
    /* [in] */ Int32 nativeFilter)
{
    ((SkCanvas*)canvas)->setDrawFilter((SkDrawFilter*)nativeFilter);
}

Boolean Canvas::NativeGetClipBounds(
    /* [in] */ Int32 canvas,
    /* [in] */ IRect* bounds)
{
    SkRect r;
    SkIRect ir;
    bool result = ((SkCanvas*)canvas)->getClipBounds(&r, SkCanvas::kBW_EdgeType);

    if (!result) {
        r.setEmpty();
    }
    r.round(&ir);
    GraphicsNative::SkIRect2IRect(ir, bounds);
    return (Boolean)result;
}

void Canvas::NativeGetCTM(
    /* [in] */ Int32 canvas,
    /* [in] */ Int32 matrix)
{
    *(SkMatrix*)matrix = ((SkCanvas*)canvas)->getTotalMatrix();
}

Boolean Canvas::NativeQuickReject(
    /* [in] */ Int32 canvas,
    /* [in] */ IRectF* rect,
    /* [in] */ CanvasEdgeType edgeType)
{
    SkRect rect_;
    GraphicsNative::IRectF2SkRect(rect, &rect_);
    return ((SkCanvas*)canvas)->quickReject(rect_, (SkCanvas::EdgeType)edgeType);
}

Boolean Canvas::NativeQuickReject(
    /* [in] */ Int32 canvas,
    /* [in] */ Int32 path,
    /* [in] */ CanvasEdgeType edgeType)
{
    return ((SkCanvas*)canvas)->quickReject(*(SkPath*)path, (SkCanvas::EdgeType)edgeType);
}

Boolean Canvas::NativeQuickReject(
    /* [in] */ Int32 canvas,
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [in] */ CanvasEdgeType edgeType)
{
    SkRect r;
    r.set(SkFloatToScalar(left), SkFloatToScalar(top),
          SkFloatToScalar(right), SkFloatToScalar(bottom));
    return ((SkCanvas*)canvas)->quickReject(r, (SkCanvas::EdgeType)edgeType);
}

void Canvas::NativeDrawRGB(
    /* [in] */ Int32 canvas,
    /* [in] */ Int32 r,
    /* [in] */ Int32 g,
    /* [in] */ Int32 b)
{
    ((SkCanvas*)canvas)->drawARGB(0xFF, r, g, b);
}

void Canvas::NativeDrawARGB(
    /* [in] */ Int32 canvas,
    /* [in] */ Int32 a,
    /* [in] */ Int32 r,
    /* [in] */ Int32 g,
    /* [in] */ Int32 b)
{
    ((SkCanvas*)canvas)->drawARGB(a, r, g, b);
}

void Canvas::NativeDrawColor(
    /* [in] */ Int32 canvas,
    /* [in] */ Int32 color)
{
    ((SkCanvas*)canvas)->drawColor(color);
}

void Canvas::NativeDrawColor(
    /* [in] */ Int32 canvas,
    /* [in] */ Int32 color,
    /* [in] */ PorterDuffMode mode)
{
    ((SkCanvas*)canvas)->drawColor(color, SkPorterDuff::ToXfermodeMode((SkPorterDuff::Mode)mode));
}

void Canvas::NativeDrawPaint(
    /* [in] */ Int32 canvas,
    /* [in] */ Int32 paint)
{
    ((SkCanvas*)canvas)->drawPaint(*(SkPaint*)paint);
}

void Canvas::NativeDrawLine(
    /* [in] */ Int32 canvas,
    /* [in] */ Float startX,
    /* [in] */ Float startY,
    /* [in] */ Float stopX,
    /* [in] */ Float stopY,
    /* [in] */ Int32 paint)
{
    ((SkCanvas*)canvas)->drawLine(SkFloatToScalar(startX), SkFloatToScalar(startY),
                     SkFloatToScalar(stopX), SkFloatToScalar(stopY),
                     *(SkPaint*)paint);
}

void Canvas::NativeDrawRect(
    /* [in] */ Int32 canvas,
    /* [in] */ IRectF* rect,
    /* [in] */ Int32 paint)
{
    SkRect rect_;
    GraphicsNative::IRectF2SkRect(rect, &rect_);
    ((SkCanvas*)canvas)->drawRect(rect_, *(SkPaint*)paint);
}

void Canvas::NativeDrawRect(
    /* [in] */ Int32 canvas,
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [in] */ Int32 paint)
{
    SkScalar left_ = SkFloatToScalar(left);
    SkScalar top_ = SkFloatToScalar(top);
    SkScalar right_ = SkFloatToScalar(right);
    SkScalar bottom_ = SkFloatToScalar(bottom);
    ((SkCanvas*)canvas)->drawRectCoords(left_, top_, right_, bottom_, *(SkPaint*)paint);
}

void Canvas::NativeDrawOval(
    /* [in] */ Int32 canvas,
    /* [in] */ IRectF* oval,
    /* [in] */ Int32 paint)
{
    SkRect oval_;
    GraphicsNative::IRectF2SkRect(oval, &oval_);
    ((SkCanvas*)canvas)->drawOval(oval_, *(SkPaint*)paint);
}

void Canvas::NativeDrawCircle(
    /* [in] */ Int32 canvas,
    /* [in] */ Float cx,
    /* [in] */ Float cy,
    /* [in] */ Float radius,
    /* [in] */ Int32 paint)
{
    ((SkCanvas*)canvas)->drawCircle(SkFloatToScalar(cx), SkFloatToScalar(cy),
                       SkFloatToScalar(radius), *(SkPaint*)paint);
}

void Canvas::NativeDrawArc(
    /* [in] */ Int32 canvas,
    /* [in] */ IRectF* oval,
    /* [in] */ Float startAngle,
    /* [in] */ Float sweepAngle,
    /* [in] */ Boolean useCenter,
    /* [in] */ Int32 paint)
{
    SkRect oval_;
    GraphicsNative::IRectF2SkRect(oval, &oval_);
    ((SkCanvas*)canvas)->drawArc(oval_, SkFloatToScalar(startAngle),
                    SkFloatToScalar(sweepAngle), useCenter, *(SkPaint*)paint);
}

void Canvas::NativeDrawRoundRect(
    /* [in] */ Int32 canvas,
    /* [in] */ IRectF* rect,
    /* [in] */ Float rx,
    /* [in] */ Float ry,
    /* [in] */ Int32 paint)
{
    SkRect rect_;
    GraphicsNative::IRectF2SkRect(rect, &rect_);
    ((SkCanvas*)canvas)->drawRoundRect(rect_, SkFloatToScalar(rx), SkFloatToScalar(ry),
                          *(SkPaint*)paint);
}

void Canvas::NativeDrawPath(
    /* [in] */ Int32 canvas,
    /* [in] */ Int32 path,
    /* [in] */ Int32 paint)
{
    ((SkCanvas*)canvas)->drawPath(*(SkPath*)path, *(SkPaint*)paint);
}

void Canvas::NativeDrawBitmap(
    /* [in] */ Int32 canvas_,
    /* [in] */ Int32 bitmap,
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Int32 paint,
    /* [in] */ Int32 canvasDensity,
    /* [in] */ Int32 screenDensity,
    /* [in] */ Int32 bitmapDensity)
{
    SkCanvas* canvas = (SkCanvas*)canvas_;
    SkScalar left_ = SkFloatToScalar(left);
    SkScalar top_ = SkFloatToScalar(top);

    if (canvasDensity == bitmapDensity || canvasDensity == 0
            || bitmapDensity == 0) {
        if (screenDensity != 0 && screenDensity != bitmapDensity) {
            SkPaint filteredPaint;
            if (paint) {
                filteredPaint = *(SkPaint*)paint;
            }
            filteredPaint.setFilterBitmap(true);
            canvas->drawBitmap(*(SkBitmap*)bitmap, left_, top_, &filteredPaint);
        }
        else {
            canvas->drawBitmap(*(SkBitmap*)bitmap, left_, top_, (SkPaint*)paint);
        }
    }
    else {
        canvas->save();
        SkScalar scale = SkFloatToScalar(canvasDensity / (float)bitmapDensity);
        canvas->translate(left_, top_);
        canvas->scale(scale, scale);

        SkPaint filteredPaint;
        if (paint) {
            filteredPaint = *(SkPaint*)paint;
        }
        filteredPaint.setFilterBitmap(true);

        canvas->drawBitmap(*(SkBitmap*)bitmap, 0, 0, &filteredPaint);

        canvas->restore();
    }
}

static void DoDrawBitmap(
    /* [in] */ SkCanvas* canvas,
    /* [in] */ SkBitmap* bitmap,
    /* [in] */ IRect* srcIRect,
    /* [in] */ const SkRect& dst,
    /* [in] */ SkPaint* paint,
    /* [in] */ Int32 screenDensity,
    /* [in] */ Int32 bitmapDensity);

void Canvas::NativeDrawBitmap(
    /* [in] */ Int32 canvas,
    /* [in] */ Int32 bitmap,
    /* [in] */ IRect* src,
    /* [in] */ IRectF* dst,
    /* [in] */ Int32 paint,
    /* [in] */ Int32 screenDensity,
    /* [in] */ Int32 bitmapDensity)
{
    SkRect dst_;
    GraphicsNative::IRectF2SkRect(dst, &dst_);
    DoDrawBitmap(((SkCanvas*)canvas), (SkBitmap*)bitmap, src, dst_, (SkPaint*)paint,
                 screenDensity, bitmapDensity);
}

void Canvas::NativeDrawBitmap(
    /* [in] */ Int32 canvas,
    /* [in] */ Int32 bitmap,
    /* [in] */ IRect* src,
    /* [in] */ IRect* dst,
    /* [in] */ Int32 paint,
    /* [in] */ Int32 screenDensity,
    /* [in] */ Int32 bitmapDensity)
{
    SkRect dst_;
    GraphicsNative::IRect2SkRect(dst, &dst_);
    DoDrawBitmap(((SkCanvas*)canvas), (SkBitmap*)bitmap, src, dst_, (SkPaint*)paint,
                 screenDensity, bitmapDensity);
}

void Canvas::NativeDrawBitmap(
    /* [in] */ Int32 canvas,
    /* [in] */ const ArrayOf<Int32>& colors,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 stride,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Boolean hasAlpha,
    /* [in] */ Int32 paint)
{
    SkBitmap bitmap;

    bitmap.setConfig(hasAlpha? SkBitmap::kARGB_8888_Config :
                     SkBitmap::kRGB_565_Config, width, height);
    if (!bitmap.allocPixels()) {
        return;
    }

    if (!GraphicsNative::SetPixels(colors, offset, stride,
                             0, 0, width, height, bitmap)) {
        return;
    }

    ((SkCanvas*)canvas)->drawBitmap(bitmap, SkFloatToScalar(x), SkFloatToScalar(y), (SkPaint*)paint);
}

void Canvas::NativeDrawBitmapMatrix(
    /* [in] */ Int32 canvas,
    /* [in] */ Int32 bitmap,
    /* [in] */ Int32 matrix,
    /* [in] */ Int32 paint)
{
    ((SkCanvas*)canvas)->drawBitmapMatrix(*(SkBitmap*)bitmap, *(SkMatrix*)matrix, (SkPaint*)paint);
}

void Canvas::NativeDrawBitmapMesh(
    /* [in] */ Int32 canvas,
    /* [in] */ Int32 bitmap,
    /* [in] */ Int32 meshWidth,
    /* [in] */ Int32 meshHeight,
    /* [in] */ const ArrayOf<Float>& _verts,
    /* [in] */ Int32 vertIndex,
    /* [in] */ ArrayOf<Int32>* colors,
    /* [in] */ Int32 colorIndex,
    /* [in] */ Int32 paint)
{
    const int ptCount = (meshWidth + 1) * (meshHeight + 1);
    const int indexCount = meshWidth * meshHeight * 6;

    if ((_verts.GetLength() < vertIndex + (ptCount << 1)) ||
        (colors != NULL && colors->GetLength() < colorIndex + ptCount)) {
        sk_throw();
    }

    /*  Our temp storage holds 2 or 3 arrays.
        texture points [ptCount * sizeof(SkPoint)]
        optionally vertex points [ptCount * sizeof(SkPoint)] if we need a
            copy to convert from float to fixed
        indices [ptCount * sizeof(uint16_t)]
    */
    ssize_t storageSize = ptCount * sizeof(SkPoint); // texs[]
#ifdef SK_SCALAR_IS_FIXED
    storageSize += ptCount * sizeof(SkPoint);  // storage for verts
#endif
    storageSize += indexCount * sizeof(uint16_t);  // indices[]

    SkAutoMalloc storage(storageSize);
    SkPoint* texs = (SkPoint*)storage.get();
    SkPoint* verts;
    uint16_t* indices;
#ifdef SK_SCALAR_IS_FLOAT
    verts = (SkPoint*)(_verts.GetPayload() + vertIndex);
    indices = (uint16_t*)(texs + ptCount);
#else
    verts = texs + ptCount;
    indices = (uint16_t*)(verts + ptCount);
    // convert floats to fixed
    {
        const float* src = _verts.GetPayload() + vertIndex;
        for (int i = 0; i < ptCount; i++) {
            verts[i].set(SkFloatToFixed(src[0]), SkFloatToFixed(src[1]));
            src += 2;
        }
    }
#endif

    // cons up texture coordinates and indices
    {
        const SkScalar w = SkIntToScalar(((SkBitmap*)bitmap)->width());
        const SkScalar h = SkIntToScalar(((SkBitmap*)bitmap)->height());
        const SkScalar dx = w / meshWidth;
        const SkScalar dy = h / meshHeight;

        SkPoint* texsPtr = texs;
        SkScalar y = 0;
        for (int i = 0; i <= meshHeight; i++) {
            if (i == meshHeight) {
                y = h;  // to ensure numerically we hit h exactly
            }
            SkScalar x = 0;
            for (int j = 0; j < meshWidth; j++) {
                texsPtr->set(x, y);
                texsPtr += 1;
                x += dx;
            }
            texsPtr->set(w, y);
            texsPtr += 1;
            y += dy;
        }
        SkASSERT(texsPtr - texs == ptCount);
    }

    // cons up indices
    {
        uint16_t* indexPtr = indices;
        int index = 0;
        for (int i = 0; i < meshHeight; i++) {
            for (int j = 0; j < meshWidth; j++) {
                // lower-left triangle
                *indexPtr++ = index;
                *indexPtr++ = index + meshWidth + 1;
                *indexPtr++ = index + meshWidth + 2;
                // upper-right triangle
                *indexPtr++ = index;
                *indexPtr++ = index + meshWidth + 2;
                *indexPtr++ = index + 1;
                // bump to the next cell
                index += 1;
            }
            // bump to the next row
            index += 1;
        }
        SkASSERT(indexPtr - indices == indexCount);
        SkASSERT((char*)indexPtr - (char*)storage.get() == storageSize);
    }

    // double-check that we have legal indices
#ifdef SK_DEBUG
    {
        for (int i = 0; i < indexCount; i++) {
            SkASSERT((unsigned)indices[i] < (unsigned)ptCount);
        }
    }
#endif

    // cons-up a shader for the bitmap
    SkPaint tmpPaint;
    if (paint) {
        tmpPaint = *(SkPaint*)paint;
    }
    SkShader* shader = SkShader::CreateBitmapShader(*(SkBitmap*)bitmap,
            SkShader::kClamp_TileMode, SkShader::kClamp_TileMode);
    SkSafeUnref(tmpPaint.setShader(shader));

    ((SkCanvas*)canvas)->drawVertices(SkCanvas::kTriangles_VertexMode, ptCount, verts, texs,
                         (const SkColor*)(colors != NULL? colors->GetPayload() : NULL),
                         NULL, indices, indexCount, tmpPaint);
}

void Canvas::NativeDrawVertices(
    /* [in] */ Int32 canvas,
    /* [in] */ CanvasVertexMode mode,
    /* [in] */ Int32 vertexCount,
    /* [in] */ const ArrayOf<Float>& _verts,
    /* [in] */ Int32 vertIndex,
    /* [in] */ ArrayOf<Float>* _texs,
    /* [in] */ Int32 texIndex,
    /* [in] */ ArrayOf<Int32>* _colors,
    /* [in] */ Int32 colorIndex,
    /* [in] */ ArrayOf<Int16>* _indices,
    /* [in] */ Int32 indexIndex,
    /* [in] */ Int32 indexCount,
    /* [in] */ Int32 paint)
{
    if (_verts.GetLength() < vertIndex + vertexCount){
        sk_throw();
    }
    if (_texs != NULL && _texs->GetLength() < texIndex + vertexCount) {
        sk_throw();
    }
    if (_colors != NULL && _colors->GetLength() < colorIndex + vertexCount) {
        sk_throw();
    }
    if (_indices != NULL && _indices->GetLength() < indexIndex + indexCount) {
        sk_throw();
    }

    const int ptCount = vertexCount >> 1;

    SkPoint* verts;
    SkPoint* texs = NULL;
#ifdef SK_SCALAR_IS_FLOAT
    verts = (SkPoint*)(_verts.GetPayload() + vertIndex);
    if (_texs != NULL) {
        texs = (SkPoint*)(_texs->GetPayload() + texIndex);
    }
#else
    int count = ptCount;    // for verts
    if (_texs != NULL) {
        count += ptCount;   // += for texs
    }
    SkAutoMalloc storage(count * sizeof(SkPoint));
    verts = (SkPoint*)storage.get();
    const float* src = vertA.GetPayload() + vertIndex;
    for (int i = 0; i < ptCount; i++) {
        verts[i].set(SkFloatToFixed(src[0]), SkFloatToFixed(src[1]));
        src += 2;
    }
    if (_texs != NULL) {
        texs = verts + ptCount;
        src = _texs->GetPayload() + texIndex;
        for (int i = 0; i < ptCount; i++) {
            texs[i].set(SkFloatToFixed(src[0]), SkFloatToFixed(src[1]));
            src += 2;
        }
    }
#endif

    const SkColor* colors = NULL;
    const uint16_t* indices = NULL;
    if (_colors != NULL) {
        colors = (const SkColor*)(_colors->GetPayload() + colorIndex);
    }
    if (_indices != NULL) {
        indices = (const uint16_t*)(_indices->GetPayload() + indexIndex);
    }

    ((SkCanvas*)canvas)->drawVertices((SkCanvas::VertexMode)mode, ptCount, verts, texs, colors, NULL,
                         indices, indexCount, *(SkPaint*)paint);
}

static void DoDrawGlyphsPos(
    /* [in] */ SkCanvas* canvas,
    /* [in] */ const Char16* glyphArray,
    /* [in] */ const Float* posArray,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Int32 flags,
    /* [in] */ SkPaint* paint)
{
    SkPoint* posPtr = new SkPoint[count];
    for (Int32 indx = 0; indx < count; indx++) {
        posPtr[indx].fX = SkFloatToScalar(x + posArray[indx * 2]);
        posPtr[indx].fY = SkFloatToScalar(y + posArray[indx * 2 + 1]);
    }
    canvas->drawPosText(glyphArray, count << 1, posPtr, *paint);
    delete [] posPtr;
}

// Same values used by Skia
#define kStdStrikeThru_Offset   (-6.0f / 21.0f)
#define kStdUnderline_Offset    (1.0f / 9.0f)
#define kStdUnderline_Thickness (1.0f / 18.0f)

static void DoDrawTextDecorations(
    /* [in] */ SkCanvas* canvas,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Float length,
    /* [in] */ SkPaint* paint)
{
    uint32_t flags;
    SkDrawFilter* drawFilter = canvas->getDrawFilter();
    if (drawFilter) {
        SkPaint paintCopy(*paint);
        drawFilter->filter(&paintCopy, SkDrawFilter::kText_Type);
        flags = paintCopy.getFlags();
    }
    else {
        flags = paint->getFlags();
    }
    if (flags & (SkPaint::kUnderlineText_Flag | SkPaint::kStrikeThruText_Flag)) {
        SkScalar left = SkFloatToScalar(x);
        SkScalar right = SkFloatToScalar(x + length);
        Float textSize = paint->getTextSize();
        Float strokeWidth = fmax(textSize * kStdUnderline_Thickness, 1.0f);
        if (flags & SkPaint::kUnderlineText_Flag) {
            SkScalar top = SkFloatToScalar(y + textSize * kStdUnderline_Offset
                    - 0.5f * strokeWidth);
            SkScalar bottom = SkFloatToScalar(y + textSize * kStdUnderline_Offset
                    + 0.5f * strokeWidth);
            canvas->drawRectCoords(left, top, right, bottom, *paint);
        }
        if (flags & SkPaint::kStrikeThruText_Flag) {
            SkScalar top = SkFloatToScalar(y + textSize * kStdStrikeThru_Offset
                    - 0.5f * strokeWidth);
            SkScalar bottom = SkFloatToScalar(y + textSize * kStdStrikeThru_Offset
                    + 0.5f * strokeWidth);
            canvas->drawRectCoords(left, top, right, bottom, *paint);
        }
    }
}

static void DrawChar16ArrayWithGlyphs(
    /* [in] */ SkCanvas* canvas,
    /* [in] */ const ArrayOf<Char16>& char16Array,
    /* [in] */ Int32 start,
    /* [in] */ Int32 count,
    /* [in] */ Int32 contextCount,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Int32 flags,
    /* [in] */ SkPaint* paint)
{
    android::sp<android::TextLayoutValue> value = android::TextLayoutEngine::getInstance().getValue(paint,
            char16Array.GetPayload(), start, count, contextCount, flags);
    if (value == NULL) {
        return;
    }
    SkPaint::Align align = paint->getTextAlign();
    if (align == SkPaint::kCenter_Align) {
        x -= 0.5 * value->getTotalAdvance();
    }
    else if (align == SkPaint::kRight_Align) {
        x -= value->getTotalAdvance();
    }
    paint->setTextAlign(SkPaint::kLeft_Align);
    DoDrawGlyphsPos(canvas, value->getGlyphs(), value->getPos(), 0, value->getGlyphsCount(), x, y, flags, paint);
    DoDrawTextDecorations(canvas, x, y, value->getTotalAdvance(), paint);
    paint->setTextAlign(align);
}

static void DrawChar16ArrayWithGlyphs(
    /* [in] */ SkCanvas* canvas,
    /* [in] */ const ArrayOf<Char16>& textArray,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Int32 flags,
    /* [in] */ SkPaint* paint)
{
    Int32 count = end - start;
    DrawChar16ArrayWithGlyphs(canvas, textArray, start, count, count, x, y, flags, paint);
}

static void DrawTextWithGlyphs(
    /* [in] */ SkCanvas* canvas,
    /* [in] */ const ArrayOf<Char32>& textArray,
    /* [in] */ Int32 start,
    /* [in] */ Int32 count,
    /* [in] */ Int32 contextCount,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Int32 flags,
    /* [in] */ SkPaint* paint)
{
    AutoPtr< ArrayOf<Char16> > char16Array = ArrayOf<Char16>::Alloc(textArray.GetLength());
    for (Int32 i = 0; i < textArray.GetLength(); ++i) {
        (*char16Array)[i] = (Char16)textArray[i];
    }
    DrawChar16ArrayWithGlyphs(canvas, *char16Array, start, count, contextCount, x, y, flags, paint);
}

static void DrawTextWithGlyphs(
    /* [in] */ SkCanvas* canvas,
    /* [in] */ const ArrayOf<Char32>& textArray,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Int32 flags,
    /* [in] */ SkPaint* paint)
{
    Int32 count = end - start;
    DrawTextWithGlyphs(canvas, textArray, start, count, count, x, y, flags, paint);
}

void Canvas::NativeDrawText(
    /* [in] */ Int32 canvas,
    /* [in] */ const ArrayOf<Char32>& text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 paint)
{
    DrawTextWithGlyphs(((SkCanvas*)canvas), text, index, count, x, y, flags, (SkPaint*)paint);
}

void Canvas::NativeDrawText(
    /* [in] */ Int32 canvas,
    /* [in] */ const String& text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 paint)
{
    AutoPtr< ArrayOf<Char16> > textArray = text.GetChar16s(start, end);
    if (textArray->GetLength() > 0) {
        DrawChar16ArrayWithGlyphs(((SkCanvas*)canvas), *textArray,
                0, textArray->GetLength(), x, y, flags, (SkPaint*)paint);
    }
}

void Canvas::NativeDrawTextRun(
    /* [in] */ Int32 canvas,
    /* [in] */ const ArrayOf<Char32>& text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [in] */ Int32 contextIndex,
    /* [in] */ Int32 contextCount,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Int32 dirFlags,
    /* [in] */ Int32 paint)
{
    DrawTextWithGlyphs((SkCanvas*)canvas, text, index,
            count, contextCount, x, y, dirFlags, (SkPaint*)paint);
}

void Canvas::NativeDrawTextRun(
    /* [in] */ Int32 canvas,
    /* [in] */ const String& text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 contextStart,
    /* [in] */ Int32 contextEnd,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Int32 dirFlags,
    /* [in] */ Int32 paint)
{
    Int32 count = end - start;
    Int32 contextCount = contextEnd - contextStart;
    AutoPtr< ArrayOf<Char16> > textArray = text.GetChar16s(start, end);
    DrawChar16ArrayWithGlyphs((SkCanvas*)canvas, *textArray, 0,
            textArray->GetLength(), contextCount, x, y, dirFlags, (SkPaint*)paint);
}

void Canvas::NativeDrawPosText(
    /* [in] */ Int32 canvas,
    /* [in] */ const ArrayOf<Char32>& text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [in] */ const ArrayOf<Float>& pos,
    /* [in] */ Int32 nativePaint)
{
    AutoPtr< ArrayOf<Char16> > char16Array = ArrayOf<Char16>::Alloc(text.GetLength());
    for (Int32 i = 0; i < text.GetLength(); ++i) {
        (*char16Array)[i] = (Char16)text[i];
    }
    Char16* textArray = char16Array->GetPayload();
    Float* posArray = pos.GetPayload();
    Int32 posCount = pos.GetLength() >> 1;
    SkPoint* posPtr = posCount > 0 ? new SkPoint[posCount] : NULL;
    for (Int32 i = 0; i < posCount; i++) {
        posPtr[i].fX = SkFloatToScalar(posArray[i << 1]);
        posPtr[i].fY = SkFloatToScalar(posArray[(i << 1) + 1]);
    }

    SkPaint::TextEncoding encoding = ((SkPaint*)nativePaint)->getTextEncoding();
    ((SkPaint*)nativePaint)->setTextEncoding(SkPaint::kUTF16_TextEncoding);
    ((SkCanvas*)canvas)->drawPosText(textArray + index, count << 1, posPtr, *(SkPaint*)nativePaint);
    ((SkPaint*)nativePaint)->setTextEncoding(encoding);

    delete[] posPtr;
}

void Canvas::NativeDrawPosText(
    /* [in] */ Int32 canvas,
    /* [in] */ const String& text,
    /* [in] */ const ArrayOf<Float>& pos,
    /* [in] */ Int32 nativePaint)
{
    AutoPtr< ArrayOf<Char16> > char16Array = text.GetChar16s();
    const void* text_ = char16Array->GetPayload();
    Int32 count = char16Array->GetLength();
    Float* posArray = pos.GetPayload();
    Int32 posCount = pos.GetLength();
    SkPoint* posPtr = posCount > 0 ? new SkPoint[posCount] : NULL;

    for (Int32 i = 0; i < posCount; i++) {
       posPtr[i].fX = SkFloatToScalar(posArray[i << 1]);
       posPtr[i].fY = SkFloatToScalar(posArray[(i << 1) + 1]);
    }

    SkPaint::TextEncoding encoding = ((SkPaint*)nativePaint)->getTextEncoding();
    ((SkPaint*)nativePaint)->setTextEncoding(SkPaint::kUTF16_TextEncoding);
    ((SkCanvas*)canvas)->drawPosText(text_, count << 1, posPtr, *(SkPaint*)nativePaint);
    ((SkPaint*)nativePaint)->setTextEncoding(encoding);

    delete[] posPtr;
}

void Canvas::NativeDrawTextOnPath(
    /* [in] */ Int32 nativeCanvas,
    /* [in] */ const ArrayOf<Char32>& text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [in] */ Int32 nativePath,
    /* [in] */ Float hOffset,
    /* [in] */ Float vOffset,
    /* [in] */ Int32 bidiFlags,
    /* [in] */ Int32 nativePaint)
{
    AutoPtr< ArrayOf<Char16> > char16Array = ArrayOf<Char16>::Alloc(text.GetLength());
    for (Int32 i = 0; i < text.GetLength(); ++i) {
        (*char16Array)[i] = (Char16)text[i];
    }
    Char16* textArray = char16Array->GetPayload();
    android::TextLayout::drawTextOnPath((SkPaint*)nativePaint, textArray + index, count,
            bidiFlags, hOffset, vOffset, (SkPath*)nativePath, (SkCanvas*)nativeCanvas);
}

void Canvas::NativeDrawTextOnPath(
    /* [in] */ Int32 nativeCanvas,
    /* [in] */ const String& text,
    /* [in] */ Int32 nativePath,
    /* [in] */ Float hOffset,
    /* [in] */ Float vOffset,
    /* [in] */ Int32 bidiFlags,
    /* [in] */ Int32 nativePaint)
{
    AutoPtr< ArrayOf<Char16> > char16Array = text.GetChar16s();
    Char16* text_ = char16Array->GetPayload();
    Int32 count = char16Array->GetLength();
    android::TextLayout::drawTextOnPath((SkPaint*)nativePaint, text_, count, bidiFlags,
            hOffset, vOffset, (SkPath*)nativePath, (SkCanvas*)nativeCanvas);
}

void Canvas::NativeDrawPicture(
    /* [in] */ Int32 canvas,
    /* [in] */ Int32 picture)
{
    SkASSERT(canvas);
    SkASSERT(picture);

#ifdef TIME_DRAW
    SkMSec now = get_thread_msec(); //SkTime::GetMSecs();
#endif
    ((SkCanvas*)canvas)->drawPicture(*(SkPicture*)picture);
#ifdef TIME_DRAW
    LOGD("---- picture playback %d ms\n", get_thread_msec() - now);
#endif
}

void Canvas::NativeFinalizer(
    /* [in] */ Int32 canvas)
{
    ((SkCanvas*)canvas)->unref();
}

ECode Canvas::SetSurfaceFormat(
    /* [in] */ Int32 format)
{
    mSurfaceFormat = format;
    return NOERROR;
}

ECode Canvas::GetNativeCanvas(
    /* [out] */ Handle32* nativeCanvas)
{
    VALIDATE_NOT_NULL(nativeCanvas);
    *nativeCanvas = (Handle32)mNativeCanvas;
    return NOERROR;
}

static ECode DoPoints(
    /* [in] */ SkCanvas* canvas,
    /* [in] */ const ArrayOf<Float>& _pts,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ IPaint* _paint,
    /* [in] */ SkCanvas::PointMode mode)
{
    if (canvas == NULL || _paint == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }
    const SkPaint& paint = *(SkPaint*)((Paint*)_paint->Probe(EIID_Paint))->mNativePaint;

    float* floats = _pts.GetPayload();
    const int length = _pts.GetLength();

    if ((offset | count) < 0 || offset + count > length) {
//        doThrowAIOOBE(env);
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    // now convert the floats into SkPoints
    count >>= 1;    // now it is the number of points
    SkAutoSTMalloc<32, SkPoint> storage(count);
    SkPoint* pts = storage.get();
    const float* src = floats + offset;
    for (int i = 0; i < count; i++) {
        pts[i].set(SkFloatToScalar(src[0]), SkFloatToScalar(src[1]));
        src += 2;
    }
    canvas->drawPoints(mode, count, pts, paint);
    return NOERROR;
}

static void DoDrawBitmap(
    /* [in] */ SkCanvas* canvas,
    /* [in] */ SkBitmap* bitmap,
    /* [in] */ IRect* srcIRect,
    /* [in] */ const SkRect& dst,
    /* [in] */ SkPaint* paint,
    /* [in] */ Int32 screenDensity,
    /* [in] */ Int32 bitmapDensity)
{
    SkIRect src, *srcPtr = NULL;

    if (NULL != srcIRect) {
        src.set(((CRect*)srcIRect)->mLeft,
                ((CRect*)srcIRect)->mTop,
                ((CRect*)srcIRect)->mRight,
                ((CRect*)srcIRect)->mBottom);
        srcPtr = &src;
    }

    if (screenDensity != 0 && screenDensity != bitmapDensity) {
        SkPaint filteredPaint;
        if (paint) {
            filteredPaint = *paint;
        }
        filteredPaint.setFilterBitmap(true);
        canvas->drawBitmapRect(*bitmap, srcPtr, dst, &filteredPaint);
    } else {
        canvas->drawBitmapRect(*bitmap, srcPtr, dst, paint);
    }
}

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos

