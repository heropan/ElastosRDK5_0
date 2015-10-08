
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
// #include "graphics/TextLayout.h"
// #include "graphics/TextLayoutCache.h"
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
#include <skia/effects/SkPorterDuff.h>
#include <utils/RefBase.h>

using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Graphics {

// {CC4FB366-48F0-48FF-A6B6-670E64F46A7B}
extern const InterfaceID EIID_Canvas =
    { 0xcc4fb366, 0x48f0, 0x48ff, { 0xa6, 0xb6, 0x67, 0xe, 0x64, 0xf4, 0x6a, 0x7b } };

CAR_INTERFACE_IMPL(Canvas, Object, ICanvas);
Canvas::Canvas()
    : mDensity(IBitmap::DENSITY_NONE)
    , mScreenDensity(IBitmap::DENSITY_NONE)
    , mSurfaceFormat(0)
{}

Canvas::~Canvas()
{
    // If the constructor threw an exception before setting mNativeCanvasWrapper, the native finalizer
    // must not be invoked.
    Dispose();
}

ECode Canvas::constructor()
{
    Boolean is = FALSE;
    if (!(IsHardwareAccelerated(&is), is)) {
        // 0 means no native bitmap
        mNativeCanvasWrapper = InitRaster(0);
    }
    return NOERROR;
}

ECode Canvas::constructor(
    /* [in] */ IBitmap* bitmap)
{
    Boolean isMutable = FALSE;
    if (bitmap->IsMutable(&isMutable), !isMutable) {
        Logger::E("Canvas", "Immutable bitmap passed to Canvas constructor");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    FAIL_RETURN(ThrowIfCannotDraw(bitmap));
    mNativeCanvasWrapper = InitRaster(((CBitmap*)bitmap)->Ni());
    mBitmap = (CBitmap*)bitmap;
    mDensity = ((CBitmap*)bitmap)->mDensity;
    return NOERROR;
}

ECode Canvas::constructor(
    /* [in] */ Int64 nativeCanvas)
{
    if (nativeCanvas == 0) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    mNativeCanvasWrapper = nativeCanvas;
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
//        throw new RuntimeException("Can't set a bitmap device on a HW accelerated canvas");
        return E_RUNTIME_EXCEPTION;
    }


    if (bitmap == NULL) {
        NativeSetBitmap(mNativeCanvasWrapper, 0, FALSE);
        mDensity = IBitmap::DENSITY_NONE;
    } else {
        Boolean isMutable;
        bitmap->IsMutable(&isMutable);
        if (!isMutable) {
            // throw new IllegalStateException();
            return E_ILLEGAL_STATE_EXCEPTION;
        }
        FAIL_RETURN(ThrowIfCannotDraw(bitmap));

        NativeSetBitmap(mNativeCanvasWrapper, ((CBitmap*)bitmap)->Ni(), TRUE);
        mDensity = ((CBitmap*)bitmap)->mDensity;
    }

    mBitmap = (CBitmap*)bitmap;
    return NOERROR;
}

void Canvas::SetNativeBitmap(
    /* [in] */ Int64 bitmapHandle)
{
    NativeSetBitmap(mNativeCanvasWrapper, bitmapHandle, FALSE);
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

ECode Canvas::SetHighContrastText(
    /* [in] */ Boolean highContrastText)
{
    return NOERROR;
}

ECode Canvas::InsertReorderBarrier()
{
    return NOERROR;
}

ECode Canvas::InsertInorderBarrier()
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

    *isOpaque = NativeIsOpaque(mNativeCanvasWrapper);
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

    *width = NativeGetWidth(mNativeCanvasWrapper);
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

    *height = NativeGetHeight(mNativeCanvasWrapper);
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

    *count = NativeSave(mNativeCanvasWrapper, MATRIX_SAVE_FLAG | CLIP_SAVE_FLAG);;
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

    *count = NativeSave(mNativeCanvasWrapper, saveFlags);
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

    assert(0 && "TODO");
    // if (bounds == null) {
    //     bounds = new RectF(getClipBounds());
    // }
    // return SaveLayer(bounds.left, bounds.top, bounds.right, bounds.bottom, paint, saveFlags, count);
    return NOERROR;
}

ECode Canvas::SaveLayer(
    /* [in] */ /*@Nullable*/ IRectF* bounds,
    /* [in] */ /*@Nullable*/ IPaint* paint,
    /* [out] */ Int32* count)
{
    return SaveLayer(bounds, paint, ALL_SAVE_FLAG, count);
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

    assert(0 && "TODO");
    // *count = NativeSaveLayer(mNativeCanvasWrapper, left, top, right, bottom,
    //         paint != NULL ? paint.mNativePaint : 0,
    //         saveFlags);
    return NOERROR;
}

ECode Canvas::SaveLayer(
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [in] */ /*@Nullable*/ IPaint* paint,
    /* [out] */ Int32* count)
{
    return SaveLayer(left, top, right, bottom, paint, ALL_SAVE_FLAG, count);
}

ECode Canvas::SaveLayerAlpha(
    /* [in] */ IRectF* bounds,
    /* [in] */ Int32 alpha,
    /* [in] */ Int32 saveFlags,
    /* [out] */ Int32* count)
{
    assert(count != NULL);

    assert(0 && "TODO");
    // if (bounds == null) {
    //     bounds = new RectF(getClipBounds());
    // }
    // return SaveLayerAlpha(bounds.left, bounds.top, bounds.right, bounds.bottom, alpha, saveFlags, count);
    return NOERROR;
}

ECode Canvas::SaveLayerAlpha(
    /* [in] */ /*@Nullable*/ IRectF* bounds,
    /* [in] */ Int32 alpha,
    /* [out] */ Int32* count)
{
    return SaveLayerAlpha(bounds, alpha, ALL_SAVE_FLAG, count);
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

    assert(0 && "TODO");
    // alpha = Math.min(255, Math.max(0, alpha));
    // *count = NativeSaveLayerAlpha(mNativeCanvasWrapper, left, top, right, bottom,
    //                              alpha, saveFlags);
    return NOERROR;
}

ECode Canvas::SaveLayerAlpha(
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [in] */ Int32 alpha,
    /* [out] */ Int32* count)
{
    return SaveLayerAlpha(left, top, right, bottom, alpha, ALL_SAVE_FLAG, count);
}

ECode Canvas::Restore()
{
    NativeRestore(mNativeCanvasWrapper);
    return NOERROR;
}

ECode Canvas::GetSaveCount(
    /* [out] */ Int32* count)
{
    assert(count != NULL);

    *count = NativeGetSaveCount(mNativeCanvasWrapper);
    return NOERROR;
}

ECode Canvas::RestoreToCount(
    /* [in] */ Int32 saveCount)
{
    NativeRestoreToCount(mNativeCanvasWrapper, saveCount);
    return NOERROR;
}

ECode Canvas::Translate(
    /* [in] */ Float dx,
    /* [in] */ Float dy)
{
    NativeTranslate(mNativeCanvasWrapper, dx, dy);
    return NOERROR;
}

ECode Canvas::Scale(
    /* [in] */ Float sx,
    /* [in] */ Float sy)
{
    NativeScale(mNativeCanvasWrapper, sx, sy);
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
    NativeRotate(mNativeCanvasWrapper, degrees);
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
    NativeSkew(mNativeCanvasWrapper, sx, sy);
    return NOERROR;
}

ECode Canvas::Concat(
    /* [in] */ IMatrix* matrix)
{
    if (matrix != NULL) {
        Int32 nativeMatrix = ((Matrix*)(IMatrix*)matrix->Probe(EIID_Matrix))->mNativeInstance;
        NativeConcat(mNativeCanvasWrapper, nativeMatrix);
    }
    return NOERROR;
}

ECode Canvas::SetMatrix(
    /* [in] */ IMatrix* matrix)
{
    Int32 nativeMatrix = 0;
    if (matrix != NULL) {
        nativeMatrix = ((Matrix*)(IMatrix*)matrix->Probe(EIID_Matrix))->mNativeInstance;
    }
    NativeSetMatrix(mNativeCanvasWrapper, nativeMatrix);
    return NOERROR;
}

ECode Canvas::GetMatrix(
    /* [in, out] */ IMatrix* ctm)
{
    assert(ctm != NULL);

    Int32 nativeMatrix = ((Matrix*)(IMatrix*)ctm->Probe(EIID_Matrix))->mNativeInstance;
    NativeGetCTM(mNativeCanvasWrapper, nativeMatrix);
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
    *isNotEmpty = NativeClipRect(mNativeCanvasWrapper,
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
    *isNotEmpty = NativeClipRect(mNativeCanvasWrapper,
            rect_->mLeft, rect_->mTop, rect_->mRight, rect_->mBottom, op);
    return NOERROR;
}

ECode Canvas::ClipRect(
    /* [in] */ IRectF* rect,
    /* [out] */ Boolean* isNotEmpty)
{
    VALIDATE_NOT_NULL(isNotEmpty);
    assert(rect != NULL);

    CRectF* rect_ = (CRectF*)rect;
    assert(0 && "TODO");
    // *isNotEmpty = NativeClipRect(mNativeCanvasWrapper, rect_->mLeft, rect_->mTop, rect_->mRight, rect_->mBottom,
    //         Region.Op.INTERSECT.nativeInt);
    return NOERROR;
}

ECode Canvas::ClipRect(
    /* [in] */ IRect* rect,
    /* [out] */ Boolean* isNotEmpty)
{
    VALIDATE_NOT_NULL(isNotEmpty);
    assert(rect != NULL);

    CRect* rect_ = (CRect*)rect;
    assert(0 && "TODO");
    // *isNotEmpty = native_clipRect(mNativeCanvasWrapper, rect.left, rect.top, rect.right, rect.bottom,
    //         Region.Op.INTERSECT.nativeInt);
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
    VALIDATE_NOT_NULL(isNotEmpty);

    *isNotEmpty = NativeClipRect(mNativeCanvasWrapper, left, top, right, bottom, op);
    return NOERROR;
}

ECode Canvas::ClipRect(
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [out] */ Boolean* isNotEmpty)
{
    VALIDATE_NOT_NULL(isNotEmpty);

    assert(0 && "TODO");
    // *isNotEmpty = NativeClipRect(mNativeCanvasWrapper, left, top, right, bottom,
    //         Region.Op.INTERSECT.nativeInt);
    return NOERROR;
}

ECode Canvas::ClipRect(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom,
    /* [out] */ Boolean* isNotEmpty)
{
    VALIDATE_NOT_NULL(isNotEmpty);

    assert(0 && "TODO");
    // *isNotEmpty = native_clipRect(mNativeCanvasWrapper, left, top, right, bottom,
    //         Region.Op.INTERSECT.nativeInt);
    return NOERROR;
}

ECode Canvas::ClipPath(
    /* [in] */ IPath* path,
    /* [in] */ RegionOp op,
    /* [out] */ Boolean* isNotEmpty)
{
    VALIDATE_NOT_NULL(isNotEmpty);
    assert(path != NULL);

    Int32 nativePath = ((CPath*)path)->Ni();
    *isNotEmpty = NativeClipPath(mNativeCanvasWrapper, nativePath, op);
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
    *isNotEmpty = NativeClipRegion(mNativeCanvasWrapper, nativeRegion, op);
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
        nativeFilter = ((DrawFilter*)(IDrawFilter*)filter->Probe(EIID_DrawFilter))->mNativeInstance;
    }
    mDrawFilter = filter;
    NativeSetDrawFilter(mNativeCanvasWrapper, nativeFilter);
    return NOERROR;
}

ECode Canvas::QuickReject(
    /* [in] */ IRectF* rect,
    /* [in] */ CanvasEdgeType type,
    /* [out] */ Boolean* isNotIntersect)
{
    VALIDATE_NOT_NULL(isNotIntersect);

    assert(0 && "TODO");
    // *isNotIntersect = native_quickReject(mNativeCanvasWrapper,
    //     rect.left, rect.top, rect.right, rect.bottom);
    return NOERROR;
}

ECode Canvas::QuickReject(
    /* [in] */ IPath* path,
    /* [in] */ CanvasEdgeType type,
    /* [out] */ Boolean* isNotIntersect)
{
    VALIDATE_NOT_NULL(isNotIntersect);
    assert(path != NULL);

    Int32 nativePath = ((CPath*)path)->Ni();
    *isNotIntersect = NativeQuickReject(mNativeCanvasWrapper, nativePath);
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

    *isNotIntersect = NativeQuickReject(mNativeCanvasWrapper, left, top, right, bottom);
    return NOERROR;
}

ECode Canvas::GetClipBounds(
    /* [in] */ IRect* bounds,
    /* [out] */ Boolean* isNotEmpty)
{
    assert(isNotEmpty != NULL);

    *isNotEmpty = NativeGetClipBounds(mNativeCanvasWrapper, bounds);
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
    assert(0 && "TODO");
    // DrawColor(Color.rgb(r, g, b));
    return NOERROR;
}

ECode Canvas::DrawARGB(
    /* [in] */ Int32 a,
    /* [in] */ Int32 r,
    /* [in] */ Int32 g,
    /* [in] */ Int32 b)
{
    assert(0 && "TODO");
    // DrawColor(Color.argb(a, r, g, b));
    return NOERROR;
}

ECode Canvas::DrawColor(
    /* [in] */ Int32 color)
{
    assert(0 && "TODO");
    // native_drawColor(mNativeCanvasWrapper, color, PorterDuff.Mode.SRC_OVER.nativeInt);
    return NOERROR;
}

ECode Canvas::DrawColor(
    /* [in] */ Int32 color,
    /* [in] */ PorterDuffMode mode)
{
    NativeDrawColor(mNativeCanvasWrapper, color, mode);
    return NOERROR;
}

ECode Canvas::DrawPaint(
    /* [in] */ IPaint* paint)
{
    assert(paint != NULL);

    NativeDrawPaint(mNativeCanvasWrapper, ((Paint*)(IPaint*)paint->Probe(EIID_Paint))->mNativePaint);
    return NOERROR;
}

ECode Canvas::DrawPoints(
    /* [in] */ ArrayOf<Float>* pts,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ IPaint* paint)
{
    assert(0 && "TODO");
    // return NativeDrawPoints(mNativeCanvasWrapper, pts, offset, count, paint.mNativePaint);
    return NOERROR;
}

ECode Canvas::DrawPoints(
    /* [in] */ ArrayOf<Float>* pts,
    /* [in] */ IPaint* paint)
{
    return DrawPoints(pts, 0, pts->GetLength(), paint);
}

ECode Canvas::DrawPoint(
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ IPaint* paint)
{
    assert(paint != NULL);
    assert(0 && "TODO");
    // native_drawPoint(mNativeCanvasWrapper, x, y, paint.mNativePaint);
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
    Int32 nativePaint = ((Paint*)(IPaint*)paint->Probe(EIID_Paint))->mNativePaint;
    NativeDrawLine(mNativeCanvasWrapper, startX, startY, stopX, stopY, nativePaint);
    return NOERROR;
}

ECode Canvas::DrawLines(
    /* [in] */ ArrayOf<Float>* pts,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ IPaint* paint)
{
    assert(0 && "TODO");
    // native_drawLines(mNativeCanvasWrapper, pts, offset, count, paint.mNativePaint);
    return NOERROR;
}

ECode Canvas::DrawLines(
    /* [in] */ ArrayOf<Float>* pts,
    /* [in] */ IPaint* paint)
{
    return DrawLines(pts, 0, pts->GetLength(), paint);
}

ECode Canvas::DrawRect(
    /* [in] */ IRectF* rect,
    /* [in] */ IPaint* paint)
{
    assert(paint != NULL);

    assert(0 && "TODO");
    // native_drawRect(mNativeCanvasWrapper,
    //         rect.left, rect.top, rect.right, rect.bottom, paint.mNativePaint);
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

    Int32 nativePaint = ((Paint*)(IPaint*)paint->Probe(EIID_Paint))->mNativePaint;
    NativeDrawRect(mNativeCanvasWrapper, left, top, right, bottom, nativePaint);
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

    assert(0 && "TODO");
    // drawOval(oval.left, oval.top, oval.right, oval.bottom, paint);
    return NOERROR;
}

ECode Canvas::DrawOval(
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [in] */ /*@NonNull*/ IPaint* paint)
{
    assert(0 && "TODO");
    // NativeDrawOval(mNativeCanvasWrapper, left, top, right, bottom, paint.mNativePaint);
    return NOERROR;
}

ECode Canvas::DrawCircle(
    /* [in] */ Float cx,
    /* [in] */ Float cy,
    /* [in] */ Float radius,
    /* [in] */ IPaint* paint)
{
    assert(paint != NULL);

    Int32 nativePaint = ((Paint*)(IPaint*)paint->Probe(EIID_Paint))->mNativePaint;
    NativeDrawCircle(mNativeCanvasWrapper, cx, cy, radius, nativePaint);
    return NOERROR;
}

ECode Canvas::DrawArc(
    /* [in] */ IRectF* oval,
    /* [in] */ Float startAngle,
    /* [in] */ Float sweepAngle,
    /* [in] */ Boolean useCenter,
    /* [in] */ IPaint* paint)
{
    assert(0 && "TODO");
    // drawArc(oval.left, oval.top, oval.right, oval.bottom, startAngle, sweepAngle, useCenter,
    //         paint);
    return NOERROR;
}

ECode Canvas::DrawArc(
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [in] */ Float startAngle,
    /* [in] */ Float sweepAngle,
    /* [in] */ Boolean useCenter,
    /* [in] */ /*@NonNull*/ IPaint* paint)
{
    assert(paint != NULL);
    assert(0 && "TODO");
    // native_drawArc(mNativeCanvasWrapper, left, top, right, bottom, startAngle, sweepAngle,
    //         useCenter, paint.mNativePaint);
    return NOERROR;
}

ECode Canvas::DrawRoundRect(
    /* [in] */ IRectF* rect,
    /* [in] */ Float rx,
    /* [in] */ Float ry,
    /* [in] */ IPaint* paint)
{
    assert(paint != NULL);

    assert(0 && "TODO");
    // drawRoundRect(rect.left, rect.top, rect.right, rect.bottom, rx, ry, paint);
    return NOERROR;
}

ECode Canvas::DrawRoundRect(
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [in] */ Float rx,
    /* [in] */ Float ry,
    /* [in] */ /*@NonNull*/ IPaint* paint)
{
    assert(0 && "TODO");
    // native_drawRoundRect(mNativeCanvasWrapper, left, top, right, bottom, rx, ry, paint.mNativePaint);
    return NOERROR;
}

ECode Canvas::DrawPath(
    /* [in] */ IPath* path,
    /* [in] */ IPaint* paint)
{
    assert(path != NULL);
    assert(paint != NULL);

    Int32 nativePath = ((CPath*)path)->Ni();
    Int32 nativePaint = ((Paint*)(IPaint*)paint->Probe(EIID_Paint))->mNativePaint;
    NativeDrawPath(mNativeCanvasWrapper, nativePath, nativePaint);
    return NOERROR;
}

ECode Canvas::ThrowIfCannotDraw(
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

    assert(0 && "TODO");
    // if (!bitmap.isPremultiplied() && bitmap.getConfig() == Bitmap.Config.ARGB_8888 &&
    //         bitmap.hasAlpha()) {
    //     throw new RuntimeException("Canvas: trying to use a non-premultiplied bitmap "
    //             + bitmap);
    // }

    return NOERROR;
}

ECode Canvas::DrawPatch(
    /* [in] */ /*@NonNull*/ INinePatch* patch,
    /* [in] */ /*@NonNull*/ IRect* dst,
    /* [in] */ /*@Nullable*/ IPaint* paint)
{
    assert(patch != NULL && dst != NULL && paint != NULL);
    assert(0 && "TODO");
    // patch.drawSoftware(this, dst, paint);
    return NOERROR;
}

ECode Canvas::DrawPatch(
    /* [in] */ /*@NonNull*/ INinePatch* patch,
    /* [in] */ IRectF* dst,
    /* [in] */ IPaint* paint)
{
    assert(0 && "TODO");
    // patch.drawSoftware(this, dst, paint);
    return NOERROR;
}

ECode Canvas::DrawBitmap(
    /* [in] */ IBitmap* bitmap,
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ IPaint* paint)
{
    assert(bitmap != NULL);

    FAIL_RETURN(ThrowIfCannotDraw(bitmap));
    Int32 nativeBitmap = ((CBitmap*)bitmap)->Ni();
    Int32 nativePaint = 0;
    if (paint != NULL) {
        nativePaint = ((Paint*)(IPaint*)paint->Probe(EIID_Paint))->mNativePaint;
    }
    NativeDrawBitmap(mNativeCanvasWrapper, nativeBitmap, left, top,
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
    FAIL_RETURN(ThrowIfCannotDraw(bitmap));
    assert(0 && "TODO");
    // final long nativePaint = paint == null ? 0 : paint.mNativePaint;

    // float left, top, right, bottom;
    // if (src == null) {
    //   left = top = 0;
    //   right = bitmap.getWidth();
    //   bottom = bitmap.getHeight();
    // } else {
    //   left = src.left;
    //   right = src.right;
    //   top = src.top;
    //   bottom = src.bottom;
    // }

    // native_drawBitmap(mNativeCanvasWrapper, bitmap.ni(), left, top, right, bottom,
    //       dst.left, dst.top, dst.right, dst.bottom, nativePaint, mScreenDensity,
    //       bitmap.mDensity);
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
    FAIL_RETURN(ThrowIfCannotDraw(bitmap));
    assert(0 && "TODO");
    // final long nativePaint = paint == null ? 0 : paint.mNativePaint;

    // int left, top, right, bottom;
    // if (src == null) {
    //     left = top = 0;
    //     right = bitmap.getWidth();
    //     bottom = bitmap.getHeight();
    // } else {
    //     left = src.left;
    //     right = src.right;
    //     top = src.top;
    //     bottom = src.bottom;
    // }

    // native_drawBitmap(mNativeCanvasWrapper, bitmap.ni(), left, top, right, bottom,
    //     dst.left, dst.top, dst.right, dst.bottom, nativePaint, mScreenDensity,
    //     bitmap.mDensity);
    return NOERROR;
}

ECode Canvas::DrawBitmap(
    /* [in] */ ArrayOf<Int32>* colors,
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
    Int32 length = colors->GetLength();
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
        nativePaint = ((Paint*)(IPaint*)paint->Probe(EIID_Paint))->mNativePaint;
    }
    // punch down to native for the actual draw
    NativeDrawBitmap(mNativeCanvasWrapper, colors, offset, stride, x, y, width,
            height, hasAlpha, nativePaint);
    return NOERROR;
}

ECode Canvas::DrawBitmap(
    /* [in] */ ArrayOf<Int32>* colors,
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
        nativePaint = ((Paint*)(IPaint*)paint->Probe(EIID_Paint))->mNativePaint;
    }
    NativeDrawBitmapMatrix(mNativeCanvasWrapper, nativeBitmap, nativeMatrix, nativePaint);
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
    /* [in] */ ArrayOf<Float>* verts,
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
    FAIL_RETURN(CheckRange(verts->GetLength(), vertOffset, count * 2));
    if (colors != NULL) {
        // no mul by 2, since we need only 1 color per vertex
        FAIL_RETURN(CheckRange(colors->GetLength(), colorOffset, count));
    }
    Int32 nativeBitmap = ((CBitmap*)bitmap)->Ni();
    Int32 nativePaint = 0;
    if (paint != NULL) {
        nativePaint = ((Paint*)(IPaint*)paint->Probe(EIID_Paint))->mNativePaint;
    }
    NativeDrawBitmapMesh(mNativeCanvasWrapper, nativeBitmap, meshWidth, meshHeight,
            verts, vertOffset, colors, colorOffset, nativePaint);
    return NOERROR;
}

ECode Canvas::DrawVertices(
    /* [in] */ CanvasVertexMode mode,
    /* [in] */ Int32 vertexCount,
    /* [in] */ ArrayOf<Float>* verts,
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

    FAIL_RETURN(CheckRange(verts->GetLength(), vertOffset, vertexCount));
    if (texs != NULL) {
        FAIL_RETURN(CheckRange(texs->GetLength(), texOffset, vertexCount));
    }
    if (colors != NULL) {
        FAIL_RETURN(CheckRange(colors->GetLength(), colorOffset, vertexCount));
    }
    if (indices != NULL) {
        FAIL_RETURN(CheckRange(indices->GetLength(), indexOffset, indexCount));
    }
    Int32 nativePaint = ((Paint*)(IPaint*)paint->Probe(EIID_Paint))->mNativePaint;
    NativeDrawVertices(mNativeCanvasWrapper, mode,
            vertexCount, verts, vertOffset, texs, texOffset, colors,
            colorOffset, indices, indexOffset, indexCount, nativePaint);
    return NOERROR;
}

ECode Canvas::DrawText(
    /* [in] */ ArrayOf<Char32>* text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ IPaint* paint)
{
    assert(paint != NULL);

    if ((index | count | (index + count) |
        (text->GetLength() - index - count)) < 0) {
//        throw new IndexOutOfBoundsException();
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    Int32 nativePaint = ((Paint*)(IPaint*)paint->Probe(EIID_Paint))->mNativePaint;
    assert(0 && "TODO");
    // NativeDrawText(mNativeCanvasWrapper, text, index, count, x, y,
    //         ((Paint*)(IPaint*)paint->Probe(EIID_Paint))->mBidiFlags, nativePaint, paint.mNativeTypeface);
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

    Paint* p = ((Paint*)(IPaint*)paint->Probe(EIID_Paint));
    assert(p != NULL && "Paint cannot be null.");
    assert(0 && "TODO");
    // NativeDrawText(mNativeCanvasWrapper, text, 0, text.GetLength(), x, y,
    //         p->mBidiFlags, p->mNativePaint, paint.mNativeTypeface);
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

    Int32 nativePaint = ((Paint*)(IPaint*)paint->Probe(EIID_Paint))->mNativePaint;
    assert(0 && "TODO");
    // NativeDrawText(mNativeCanvasWrapper, text, start, end, x, y,
    //         ((Paint*)(IPaint*)paint->Probe(EIID_Paint))->mBidiFlags, nativePaint, paint.mNativeTypeface);
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
    assert(0 && "TODO");
    // if (text instanceof String || text instanceof SpannedString ||
    //     text instanceof SpannableString) {
    //     native_drawText(mNativeCanvasWrapper, text.toString(), start, end, x, y,
    //             paint.mBidiFlags, paint.mNativePaint, paint.mNativeTypeface);
    // } else if (text instanceof GraphicsOperations) {
    //     ((GraphicsOperations) text).drawText(this, start, end, x, y,
    //             paint);
    // } else {
    //     char[] buf = TemporaryBuffer.obtain(end - start);
    //     TextUtils.getChars(text, start, end, buf, 0);
    //     native_drawText(mNativeCanvasWrapper, buf, 0, end - start, x, y,
    //             paint.mBidiFlags, paint.mNativePaint, paint.mNativeTypeface);
    //     TemporaryBuffer.recycle(buf);
    // }

    return NOERROR;
}

ECode Canvas::DrawTextRun(
    /* [in] */ ArrayOf<Char32>* text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [in] */ Int32 contextIndex,
    /* [in] */ Int32 contextCount,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Boolean isRtl,
    /* [in] */ IPaint* paint)
{
    if (paint == NULL) {
//        throw new NullPointerException("paint is null");
        return E_NULL_POINTER_EXCEPTION;
    }
    if ((index | count | (text->GetLength() - index - count)) < 0) {
//        throw new IndexOutOfBoundsException();
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    assert(0 && "TODO");
    // Int32 nativePaint = ((Paint*)(IPaint*)paint->Probe(EIID_Paint))->mNativePaint;
    // NativeDrawTextRun(mNativeCanvasWrapper, text, index, count,
    //     contextIndex, contextCount, x, y, isRtl, nativePaint, paint.mNativeTypeface);
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
    /* [in] */ Boolean isRtl,
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

    // if (text instanceof String || text instanceof SpannedString ||
    //         text instanceof SpannableString) {
    //     native_drawTextRun(mNativeCanvasWrapper, text.toString(), start, end,
    //             contextStart, contextEnd, x, y, isRtl, paint.mNativePaint, paint.mNativeTypeface);
    // } else if (text instanceof GraphicsOperations) {
    //     ((GraphicsOperations) text).drawTextRun(this, start, end,
    //             contextStart, contextEnd, x, y, isRtl, paint);
    // } else {
    //     int contextLen = contextEnd - contextStart;
    //     int len = end - start;
    //     char[] buf = TemporaryBuffer.obtain(contextLen);
    //     TextUtils.getChars(text, contextStart, contextEnd, buf, 0);
    //     native_drawTextRun(mNativeCanvasWrapper, buf, start - contextStart, len,
    //             0, contextLen, x, y, isRtl, paint.mNativePaint, paint.mNativeTypeface);
    //     TemporaryBuffer.recycle(buf);
    // }
    //TODO:
    //assert(0);
//    PRINT_FILE_LINE_EX("TODO");
    assert(0 && "TODO");
    String str;
    text->ToString(&str);
    Int32 nativePaint = ((Paint*)(IPaint*)paint->Probe(EIID_Paint))->mNativePaint;
    // NativeDrawTextRun(mNativeCanvasWrapper, str, start, end,
    //             contextStart, contextEnd, x, y, isRtl, nativePaint, paint.mNativeTypeface);
     return NOERROR;
}

ECode Canvas::DrawPosText(
    /* [in] */ ArrayOf<Char32>* text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [in] */ ArrayOf<Float>* pos,
    /* [in] */ IPaint* paint)
{
    assert(paint != NULL);

    if (index < 0 || index + count > text->GetLength()
            || count * 2 > pos->GetLength()) {
//        throw new IndexOutOfBoundsException();
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    assert(0 && "TODO");
    // for (int i = 0; i < count; i++) {
    //     drawText(text, index + i, 1, pos[i * 2], pos[i * 2 + 1], paint);
    // }
    return NOERROR;
}

ECode Canvas::DrawPosText(
    /* [in] */ const String& text,
    /* [in] */ ArrayOf<Float>* pos,
    /* [in] */ IPaint* paint)
{
    assert(paint != NULL);

    assert(0 && "TODO");
    // drawPosText(text.toCharArray(), 0, text.length(), pos, paint);
    return NOERROR;
}

ECode Canvas::DrawTextOnPath(
    /* [in] */ ArrayOf<Char32>* text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [in] */ IPath* path,
    /* [in] */ Float hOffset,
    /* [in] */ Float vOffset,
    /* [in] */ IPaint* paint)
{
    assert(path != NULL);
    assert(paint != NULL);

    if (index < 0 || index + count > text->GetLength()) {
//        throw new ArrayIndexOutOfBoundsException();
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    Int32 nativePath = ((CPath*)path)->Ni();
    Int32 nativePaint = ((Paint*)(IPaint*)paint->Probe(EIID_Paint))->mNativePaint;
    assert(0 && "TODO");
    // NativeDrawTextOnPath(mNativeCanvasWrapper, text, index, count,
    //         nativePath, hOffset, vOffset, ((Paint*)paint->Probe(EIID_Paint))->mBidiFlags, nativePaint, paint.mNativeTypeface);
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
        Int32 nativePaint = ((Paint*)(IPaint*)paint->Probe(EIID_Paint))->mNativePaint;
        assert(0 && "TODO");
        // NativeDrawTextOnPath(mNativeCanvasWrapper, text, nativePath,
        //         hOffset, vOffset, ((Paint*)(IPaint*)paint->Probe(EIID_Paint))->mBidiFlags, nativePaint, paint.mNativeTypeface);
    }
    return NOERROR;
}

ECode Canvas::DrawPicture(
    /* [in] */ IPicture* picture)
{
    assert(picture != NULL);
    picture->EndRecording();
    assert(0 && "TODO");
    // int restoreCount = save();
    // picture.draw(this);
    // restoreToCount(restoreCount);
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

ECode Canvas::ReleaseResources() {
    return Dispose();
}

ECode Canvas::Dispose()
{
    if (mNativeCanvasWrapper != 0) {
        NativeFinalizer(mNativeCanvasWrapper);
        mNativeCanvasWrapper = 0;
    }
}

void Canvas::FreeCaches()
{
    assert(0 && "TODO: need jni codes.");
}

/**
 * Free up text layout caches
 *
 * @hide
 */
void Canvas::FreeTextLayoutCaches()
{
    assert(0 && "TODO: need jni codes.");
}

Int64 Canvas::GetNativeCanvasWrapper()
{
    return mNativeCanvasWrapper;
}

Boolean Canvas::IsRecordingFor(
    /* [in] */ IInterface* o)
{
    return FALSE;
}

Int64 Canvas::InitRaster(
    /* [in] */ Int64 bitmap)
{
    return bitmap ? (Int64)new SkCanvas(*(SkBitmap*)bitmap) : (Int64)new SkCanvas;
}

void Canvas::NativeSetBitmap(
    /* [in] */ Int64 canvas,
    /* [in] */ Int64 bitmap,
    /* [in] */ Boolean copyState)
{
    assert(0 && "TODO: need jni codes.");
}

Boolean Canvas::NativeIsOpaque(
    /* [in] */ Int64 canvasHandle)
{
    assert(0 && "TODO: need jni codes.");
    return FALSE;
}

Int32 Canvas::NativeGetWidth(
    /* [in] */ Int64 canvasHandle)
{
    assert(0 && "TODO: need jni codes.");
    return -1;
}

Int32 Canvas::NativeGetHeight(
    /* [in] */ Int64 canvasHandle)
{
    assert(0 && "TODO: need jni codes.");
    return -1;
}

Int32 Canvas::NativeSave(
    /* [in] */ Int64 canvasHandle,
    /* [in] */ Int32 saveFlags)
{
    assert(0 && "TODO: need jni codes.");
    return -1;
}

Int32 Canvas::NativeSaveLayer(
    /* [in] */ Int64 canvas,
    /* [in] */ Float l,
    /* [in] */ Float t,
    /* [in] */ Float r,
    /* [in] */ Float b,
    /* [in] */ Int64 paint,
    /* [in] */ Int32 flags)
{
    assert(0 && "TODO: need jni codes.");
    return -1;
}

Int32 Canvas::NativeSaveLayerAlpha(
    /* [in] */Int64 canvas,
    /* [in] */ Float l,
    /* [in] */ Float t,
    /* [in] */ Float r,
    /* [in] */ Float b,
    /* [in] */ Int32 alpha,
    /* [in] */ Int32 flags)
{
    assert(0 && "TODO: need jni codes.");
    return -1;
}

void Canvas::NativeRestore(
    /* [in] */ Int64 canvasHandle)
{
    assert(0 && "TODO: need jni codes.");
}

void Canvas::NativeRestoreToCount(
    /* [in] */ Int64 canvasHandle,
    /* [in] */ Int32 saveCount)
{
    assert(0 && "TODO: need jni codes.");
}

Int32 Canvas::NativeGetSaveCount(
    /* [in] */ Int64 canvasHandle)
{
    assert(0 && "TODO: need jni codes.");
    return -1;
}

void Canvas::NativeTranslate(
    /* [in] */ Int64 canvasHandle,
    /* [in] */ Float dx,
    /* [in] */ Float dy)
{
    assert(0 && "TODO: need jni codes.");
}

void Canvas::NativeScale(
    /* [in] */ Int64 canvasHandle,
    /* [in] */ Float sx,
    /* [in] */ Float sy)
{
    assert(0 && "TODO: need jni codes.");
}

void Canvas::NativeRotate(
    /* [in] */ Int64 canvasHandle,
    /* [in] */ Float degrees)
{
    assert(0 && "TODO: need jni codes.");
}

void Canvas::NativeSkew(
    /* [in] */ Int64 canvasHandle,
    /* [in] */ Float sx,
    /* [in] */ Float sy)
{
    assert(0 && "TODO: need jni codes.");
}

void Canvas::NativeConcat(
    /* [in] */ Int64 canvas,
    /* [in] */ Int64 matrix)
{
    ((SkCanvas*)canvas)->concat(*(SkMatrix*)matrix);
}

void Canvas::NativeSetMatrix(
    /* [in] */ Int64 canvas,
    /* [in] */ Int64 matrix)
{
    assert(0 && "TODO: need jni codes.");
}

Boolean Canvas::NativeClipRect(
    /* [in] */ Int64 canvas,
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [in] */ RegionOp op)
{
    assert(0 && "TODO: need jni codes.");
    return NOERROR;
}

Boolean Canvas::NativeClipPath(
    /* [in] */ Int64 canvas,
    /* [in] */ Int64 path,
    /* [in] */ RegionOp op)
{
    assert(0 && "TODO: need jni codes.");
    return FALSE;
}

Boolean Canvas::NativeClipRegion(
    /* [in] */ Int64 canvas,
    /* [in] */ Int64 region,
    /* [in] */ RegionOp op)
{
    assert(0 && "TODO: need jni codes.");
    return FALSE;
}

void Canvas::NativeSetDrawFilter(
    /* [in] */ Int64 canvas,
    /* [in] */ Int64 nativeFilter)
{
    assert(0 && "TODO: need jni codes.");
}

Boolean Canvas::NativeGetClipBounds(
    /* [in] */ Int64 canvas,
    /* [in] */ IRect* bounds)
{
    assert(0 && "TODO: need jni codes.");
    return FALSE;
}

void Canvas::NativeGetCTM(
    /* [in] */ Int64 canvas,
    /* [in] */ Int64 matrix)
{
    assert(0 && "TODO: need jni codes.");
}

Boolean Canvas::NativeQuickReject(
    /* [in] */ Int64 nativeCanvas,
    /* [in] */ Int64 nativePath)
{
    assert(0 && "TODO: need jni codes.");
    return FALSE;
}

Boolean Canvas::NativeQuickReject(
    /* [in] */ Int64 nativeCanvas,
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom)
{
    assert(0 && "TODO: need jni codes.");
    return FALSE;
}

void Canvas::NativeDrawColor(
    /* [in] */ Int64 canvas,
    /* [in] */ Int32 color,
    /* [in] */ Int32 mode)
{
    assert(0 && "TODO: need jni codes.");
}

void Canvas::NativeDrawPaint(
    /* [in] */ Int64 canvas,
    /* [in] */ Int64 paint)
{
    assert(0 && "TODO: need jni codes.");
}

void Canvas::NativeDrawPoint(
    /* [in] */ Int64 canvasHandle,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Int64 paintHandle)
{
    assert(0 && "TODO: need jni codes.");
}

void Canvas::NativeDrawPoints(
    /* [in] */ Int64 canvasHandle,
    /* [in] */ ArrayOf<Float>* pts,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ Int64 paintHandle)
{
    assert(0 && "TODO: need jni codes.");
}

void Canvas::NativeDrawLine(
    /* [in] */ Int64 canvas,
    /* [in] */ Float startX,
    /* [in] */ Float startY,
    /* [in] */ Float stopX,
    /* [in] */ Float stopY,
    /* [in] */ Int64 paint)
{
    assert(0 && "TODO: need jni codes.");
}

void Canvas::NativeDrawLines(
    /* [in] */ Int64 canvasHandle,
    /* [in] */ ArrayOf<Float>* pts,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ Int64 paintHandle)
{
    assert(0 && "TODO: need jni codes.");
}

void Canvas::NativeDrawRect(
    /* [in] */ Int64 canvas,
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [in] */ Int64 paint)
{
    assert(0 && "TODO: need jni codes.");
}

void Canvas::NativeDrawOval(
    /* [in] */ Int64 nativeCanvas,
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [in] */ Int64 nativePaint)
{
    assert(0 && "TODO: need jni codes.");
}

void Canvas::NativeDrawCircle(
    /* [in] */ Int64 canvas,
    /* [in] */ Float cx,
    /* [in] */ Float cy,
    /* [in] */ Float radius,
    /* [in] */ Int64 paint)
{
    assert(0 && "TODO: need jni codes.");
}

void Canvas::NativeDrawArc(
    /* [in] */ Int64 nativeCanvas,
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [in] */ Float startAngle,
    /* [in] */ Float sweep,
    /* [in] */ Boolean useCenter,
    /* [in] */ Int64 nativePaint)
{
    assert(0 && "TODO: need jni codes.");
}

void Canvas::NativeDrawRoundRect(
    /* [in] */ Int64 nativeCanvas,
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [in] */ Float rx,
    /* [in] */ Float ry,
    /* [in] */ Int64 nativePaint)
{
    assert(0 && "TODO: need jni codes.");
}

void Canvas::NativeDrawPath(
    /* [in] */ Int64 canvas,
    /* [in] */ Int64 path,
    /* [in] */ Int64 paint)
{
    assert(0 && "TODO: need jni codes.");
}

void Canvas::NativeDrawBitmap(
    /* [in] */ Int64 canvas_,
    /* [in] */ Int64 bitmap,
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Int64 paint,
    /* [in] */ Int32 canvasDensity,
    /* [in] */ Int32 screenDensity,
    /* [in] */ Int32 bitmapDensity)
{
    assert(0 && "TODO: need jni codes.");
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
    /* [in] */ Int64 nativeCanvas,
    /* [in] */ Int64 nativeBitmap,
    /* [in] */ Float srcLeft,
    /* [in] */ Float srcTop,
    /* [in] */ Float srcRight,
    /* [in] */ Float srcBottom,
    /* [in] */ Float dstLeft,
    /* [in] */ Float dstTop,
    /* [in] */ Float dstRight,
    /* [in] */ Float dstBottom,
    /* [in] */ Int64 nativePaintOrZero,
    /* [in] */ Int32 screenDensity,
    /* [in] */ Int32 bitmapDensity)
{
    assert(0 && "TODO: need jni codes.");
}

void Canvas::NativeDrawBitmap(
    /* [in] */ Int64 canvas,
    /* [in] */ ArrayOf<Int32>* colors,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 stride,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Boolean hasAlpha,
    /* [in] */ Int64 paint)
{
    assert(0 && "TODO: need jni codes.");
}

void Canvas::NativeDrawBitmapMatrix(
    /* [in] */ Int64 canvas,
    /* [in] */ Int64 bitmap,
    /* [in] */ Int64 matrix,
    /* [in] */ Int64 paint)
{
    assert(0 && "TODO: need jni codes.");
}

void Canvas::NativeDrawBitmapMesh(
    /* [in] */ Int64 canvas,
    /* [in] */ Int64 bitmap,
    /* [in] */ Int32 meshWidth,
    /* [in] */ Int32 meshHeight,
    /* [in] */ ArrayOf<Float>* _verts,
    /* [in] */ Int32 vertIndex,
    /* [in] */ ArrayOf<Int32>* colors,
    /* [in] */ Int32 colorIndex,
    /* [in] */ Int64 paint)
{
    assert(0 && "TODO: need jni codes.");
}

void Canvas::NativeDrawVertices(
    /* [in] */ Int64 canvas,
    /* [in] */ CanvasVertexMode mode,
    /* [in] */ Int32 vertexCount,
    /* [in] */ ArrayOf<Float>* _verts,
    /* [in] */ Int32 vertIndex,
    /* [in] */ ArrayOf<Float>* _texs,
    /* [in] */ Int32 texIndex,
    /* [in] */ ArrayOf<Int32>* _colors,
    /* [in] */ Int32 colorIndex,
    /* [in] */ ArrayOf<Int16>* _indices,
    /* [in] */ Int32 indexIndex,
    /* [in] */ Int32 indexCount,
    /* [in] */ Int64 paint)
{
    assert(0 && "TODO: need jni codes.");
}

// static void DoDrawGlyphsPos(
//     /* [in] */ SkCanvas* canvas,
//     /* [in] */ const Char16* glyphArray,
//     /* [in] */ const Float* posArray,
//     /* [in] */ Int32 index,
//     /* [in] */ Int32 count,
//     /* [in] */ Float x,
//     /* [in] */ Float y,
//     /* [in] */ Int32 flags,
//     /* [in] */ SkPaint* paint)
// {
//     SkPoint* posPtr = new SkPoint[count];
//     for (Int32 indx = 0; indx < count; indx++) {
//         posPtr[indx].fX = SkFloatToScalar(x + posArray[indx * 2]);
//         posPtr[indx].fY = SkFloatToScalar(y + posArray[indx * 2 + 1]);
//     }
//     canvas->drawPosText(glyphArray, count << 1, posPtr, *paint);
//     delete [] posPtr;
// }

// Same values used by Skia
#define kStdStrikeThru_Offset   (-6.0f / 21.0f)
#define kStdUnderline_Offset    (1.0f / 9.0f)
#define kStdUnderline_Thickness (1.0f / 18.0f)

// static void DrawChar16ArrayWithGlyphs(
//     /* [in] */ SkCanvas* canvas,
//     /* [in] */ const ArrayOf<Char16>& char16Array,
//     /* [in] */ Int32 start,
//     /* [in] */ Int32 count,
//     /* [in] */ Int32 contextCount,
//     /* [in] */ Float x,
//     /* [in] */ Float y,
//     /* [in] */ Int32 flags,
//     /* [in] */ SkPaint* paint)
// {
//     android::sp<android::TextLayoutValue> value = android::TextLayoutEngine::getInstance().getValue(paint,
//             char16Array.GetPayload(), start, count, contextCount, flags);
//     if (value == NULL) {
//         return;
//     }
//     SkPaint::Align align = paint->getTextAlign();
//     if (align == SkPaint::kCenter_Align) {
//         x -= 0.5 * value->getTotalAdvance();
//     }
//     else if (align == SkPaint::kRight_Align) {
//         x -= value->getTotalAdvance();
//     }
//     paint->setTextAlign(SkPaint::kLeft_Align);
//     assert(0 && "TODO: need jni codes.");
//     // DoDrawGlyphsPos(canvas, value->getGlyphs(), value->getPos(), 0, value->getGlyphsCount(), x, y, flags, paint);
//     // DoDrawTextDecorations(canvas, x, y, value->getTotalAdvance(), paint);
//     paint->setTextAlign(align);
// }

// static void DrawChar16ArrayWithGlyphs(
//     /* [in] */ SkCanvas* canvas,
//     /* [in] */ const ArrayOf<Char16>& textArray,
//     /* [in] */ Int32 start,
//     /* [in] */ Int32 end,
//     /* [in] */ Float x,
//     /* [in] */ Float y,
//     /* [in] */ Int32 flags,
//     /* [in] */ SkPaint* paint)
// {
//     Int32 count = end - start;
//     DrawChar16ArrayWithGlyphs(canvas, textArray, start, count, count, x, y, flags, paint);
// }

// static void DrawTextWithGlyphs(
//     /* [in] */ SkCanvas* canvas,
//     /* [in] */ const ArrayOf<Char32>& textArray,
//     /* [in] */ Int32 start,
//     /* [in] */ Int32 count,
//     /* [in] */ Int32 contextCount,
//     /* [in] */ Float x,
//     /* [in] */ Float y,
//     /* [in] */ Int32 flags,
//     /* [in] */ SkPaint* paint)
// {
//     AutoPtr< ArrayOf<Char16> > char16Array = ArrayOf<Char16>::Alloc(textArray.GetLength());
//     for (Int32 i = 0; i < textArray.GetLength(); ++i) {
//         (*char16Array)[i] = (Char16)textArray[i];
//     }
//     DrawChar16ArrayWithGlyphs(canvas, *char16Array, start, count, contextCount, x, y, flags, paint);
// }

// static void DrawTextWithGlyphs(
//     /* [in] */ SkCanvas* canvas,
//     /* [in] */ const ArrayOf<Char32>& textArray,
//     /* [in] */ Int32 start,
//     /* [in] */ Int32 end,
//     /* [in] */ Float x,
//     /* [in] */ Float y,
//     /* [in] */ Int32 flags,
//     /* [in] */ SkPaint* paint)
// {
//     Int32 count = end - start;
//     DrawTextWithGlyphs(canvas, textArray, start, count, count, x, y, flags, paint);
// }

void Canvas::NativeDrawText(
    /* [in] */ Int64 canvas,
    /* [in] */ ArrayOf<Char32>* text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Int32 flags,
    /* [in] */ Int64 paint)
{
    assert(0 && "TODO: need jni codes.");
}

void Canvas::NativeDrawText(
    /* [in] */ Int64 canvas,
    /* [in] */ const String& text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Int32 flags,
    /* [in] */ Int64 paint)
{
    assert(0 && "TODO: need jni codes.");
}

void Canvas::NativeDrawTextRun(
    /* [in] */ Int64 nativeCanvas,
    /* [in] */ const String& text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int32 contextStart,
    /* [in] */ Int32 contextEnd,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Boolean isRtl,
    /* [in] */ Int64 nativePaint,
    /* [in] */ Int64 nativeTypeface)
{
    assert(0 && "TODO: need jni codes.");
}

void Canvas::NativeDrawTextRun(
    /* [in] */ Int64 nativeCanvas,
    /* [in] */ ArrayOf<Char32>* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 count,
    /* [in] */ Int32 contextStart,
    /* [in] */ Int32 contextCount,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Boolean isRtl,
    /* [in] */ Int64 nativePaint,
    /* [in] */ Int64 nativeTypeface)
{
    assert(0 && "TODO: need jni codes.");
}

void Canvas::NativeDrawTextOnPath(
    /* [in] */ Int64 nativeCanvas,
    /* [in] */ ArrayOf<Char32>* text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [in] */ Int64 nativePath,
    /* [in] */ Float hOffset,
    /* [in] */ Float vOffset,
    /* [in] */ Int32 bidiFlags,
    /* [in] */ Int64 nativePaint,
    /* [in] */ Int64 nativeTypeface)
{
    assert(0 && "TODO: need jni codes.");
}

void Canvas::NativeDrawTextOnPath(
    /* [in] */ Int64 nativeCanvas,
    /* [in] */ const String& text,
    /* [in] */ Int64 nativePath,
    /* [in] */ Float hOffset,
    /* [in] */ Float vOffset,
    /* [in] */ Int32 bidiFlags,
    /* [in] */ Int64 nativePaint,
    /* [in] */ Int64 nativeTypeface)
{
    assert(0 && "TODO: need jni codes.");
}

void Canvas::NativeFinalizer(
    /* [in] */ Int64 canvas)
{
    assert(0 && "TODO: need jni codes.");
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
    *nativeCanvas = (Handle32)mNativeCanvasWrapper;
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

