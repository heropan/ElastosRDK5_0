#include "elastos/droid/view/GLES20Canvas.h"
#include "elastos/droid/graphics/CBitmap.h"
#include "elastos/droid/graphics/CPath.h"
#include "elastos/droid/graphics/CPaintFlagsDrawFilter.h"
#include "elastos/droid/graphics/Matrix.h"
#include "elastos/droid/graphics/NinePatch.h"
#include "elastos/droid/graphics/TemporaryBuffer.h"
#include "elastos/droid/graphics/Paint.h"
#include "elastos/droid/text/TextUtils.h"

#include <elastos/core/Math.h>
#include <elastos/utility/logging/Slogger.h>

using Elastos::Droid::Graphics::CBitmap;
using Elastos::Droid::Graphics::CPath;
using Elastos::Droid::Graphics::CPaintFlagsDrawFilter;
using Elastos::Droid::Graphics::IPaintFlagsDrawFilter;
using Elastos::Droid::Graphics::Matrix;
using Elastos::Droid::Graphics::NinePatch;
using Elastos::Droid::Graphics::TemporaryBuffer;
using Elastos::Droid::Graphics::Paint;
using Elastos::Droid::Graphics::PorterDuffMode_SRC_OVER;
using Elastos::Droid::Graphics::RegionOp_INTERSECT;
using Elastos::Droid::Text::TextUtils;
using Elastos::Droid::Text::ISpannedString;
using Elastos::Droid::Text::ISpannableString;
using Elastos::Droid::Text::IGraphicsOperations;

using Elastos::Core::IString;

namespace Elastos {
namespace Droid {
namespace View {

GLES20Canvas::CanvasFinalizer::CanvasFinalizer(
/* [in] */ Int64 renderer)
{}

GLES20Canvas::CanvasFinalizer::~CanvasFinalizer()
{}

Boolean GLES20Canvas::sIsAvailable = GLES20Canvas::nIsAvailable();

ECode GLES20Canvas::GetWidth(
    /* [out] */ Int32* res)
{
    *res = mWidth;
    return NOERROR;
}

ECode GLES20Canvas::GetHeight(
    /* [out] */ Int32* res)
{
    *res = mHeight;
    return NOERROR;
}

ECode GLES20Canvas::GetMaximumBitmapWidth(
    /* [out] */ Int32* res)
{
    *res = nGetMaximumTextureWidth();
    return NOERROR;
}

ECode GLES20Canvas::GetMaximumBitmapHeight(
    /* [out] */ Int32* res)
{
    *res = nGetMaximumTextureHeight();
    return NOERROR;
}

ECode GLES20Canvas::SetViewport(
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    mWidth = width;
    mHeight = height;

    nSetViewport(mRenderer, width, height);
    return NOERROR;
}

ECode GLES20Canvas::SetHighContrastText(
    /* [in] */ Boolean highContrastText)
{
    nSetHighContrastText(mRenderer, highContrastText);
    return NOERROR;
}

ECode GLES20Canvas::InsertReorderBarrier()
{
    nInsertReorderBarrier(mRenderer, TRUE);
    return NOERROR;
}

ECode GLES20Canvas::InsertInorderBarrier()
{
    nInsertReorderBarrier(mRenderer, FALSE);
    return NOERROR;
}

ECode GLES20Canvas::OnPreDraw(
    /* [in] */ IRect* dirty,
    /* [out] */ Int32* res)
{
    if (dirty != NULL) {
        Int32 left ,top ,right ,bottom;
        dirty->GetLeft(&left);
        dirty->GetTop(&top);
        dirty->GetRight(&right);
        dirty->GetBottom(&bottom);
        *res = nPrepareDirty(mRenderer, left, top, right, bottom, mOpaque);
    } else {
        *res = nPrepare(mRenderer, mOpaque);
    }
    return NOERROR;
}

ECode GLES20Canvas::OnPostDraw()
{
    nFinish(mRenderer);
    return NOERROR;
}

ECode GLES20Canvas::CallDrawGLFunction(
    /* [in] */ Int64 drawGLFunction,
    /* [out] */ Int32* res)
{
    *res = nCallDrawGLFunction(mRenderer, drawGLFunction);
    return NOERROR;
}

ECode GLES20Canvas::DrawRenderNode(
    /* [in] */ IRenderNode* renderNode,
    /* [in] */ IRect* dirty,
    /* [in] */ Int32 flags,
    /* [out] */ Int32* res)
{
    Int64 list;
    renderNode->GetNativeDisplayList(&list);
    *res = nDrawRenderNode(mRenderer, list, dirty, flags);
    return NOERROR;
}

ECode GLES20Canvas::DrawHardwareLayer(
    /* [in] */ IHardwareLayer* layer,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ IPaint* paint)
{
    layer->SetLayerPaint(paint);
    Int64 l;
    layer->GetLayer(&l);
    nDrawLayer(mRenderer, l, x, y);
    return NOERROR;
}

ECode GLES20Canvas::ClipPath(
    /* [in] */ IPath* path,
    /* [out] */ Boolean* res)
{
    CPath* p = (CPath*)path;
    *res = nClipPath(mRenderer, p->mNativePath, RegionOp_INTERSECT);

    return NOERROR;
}

ECode GLES20Canvas::ClipPath(
    /* [in] */ IPath* path,
    /* [in] */ RegionOp op,
    /* [out] */ Boolean* res)
{
    CPath* p = (CPath*)path;
    *res = nClipPath(mRenderer, p->mNativePath, op);
    return NOERROR;
}

ECode GLES20Canvas::ClipRect(
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [out] */ Boolean* res)
{
    *res = nClipRect(mRenderer, left, top, right, bottom, RegionOp_INTERSECT);
    return NOERROR;
}

ECode GLES20Canvas::ClipRect(
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [in] */ RegionOp op,
    /* [out] */ Boolean* res)
{
    *res = nClipRect(mRenderer, left, top, right, bottom, op);
    return NOERROR;
}

ECode GLES20Canvas::ClipRect(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom,
    /* [out] */ Boolean* res)
{
    *res = nClipRect(mRenderer, left, top, right, bottom, RegionOp_INTERSECT);
    return NOERROR;
}

ECode GLES20Canvas::ClipRect(
    /* [in] */ IRect* rect)
{
    Int32 left ,top ,right ,bottom;
    rect->GetLeft(&left);
    rect->GetTop(&top);
    rect->GetRight(&right);
    rect->GetBottom(&bottom);
    nClipRect(mRenderer, left, top, right, bottom, RegionOp_INTERSECT);
    return NOERROR;
}

ECode GLES20Canvas::ClipRect(
    /* [in] */ IRect* rect,
    /* [in] */ RegionOp op)
{
    Int32 left ,top ,right ,bottom;
    rect->GetLeft(&left);
    rect->GetTop(&top);
    rect->GetRight(&right);
    rect->GetBottom(&bottom);
    nClipRect(mRenderer, left, top, right, bottom, op);
    return NOERROR;
}

ECode GLES20Canvas::ClipRect(
    /* [in] */ IRectF* rect)
{
    Float left ,top ,right ,bottom;
    rect->GetLeft(&left);
    rect->GetTop(&top);
    rect->GetRight(&right);
    rect->GetBottom(&bottom);
    nClipRect(mRenderer, left, top, right, bottom, RegionOp_INTERSECT);
    return NOERROR;
}

ECode GLES20Canvas::ClipRect(
    /* [in] */ IRectF* rect,
    /* [in] */ RegionOp op)
{
    Float left ,top ,right ,bottom;
    rect->GetLeft(&left);
    rect->GetTop(&top);
    rect->GetRight(&right);
    rect->GetBottom(&bottom);
    nClipRect(mRenderer, left, top, right, bottom, op);
    return NOERROR;
}

ECode GLES20Canvas::ClipRegion(
    /* [in] */ IRegion* region)
{
    Handle64 nRegion;
    region->GetNativeRegion(&nRegion);
    nClipRegion(mRenderer, nRegion, RegionOp_INTERSECT);
    return NOERROR;
}

ECode GLES20Canvas::ClipRegion(
    /* [in] */ IRegion* region,
    /* [in] */ RegionOp op)
{
    Handle64 nRegion;
    region->GetNativeRegion(&nRegion);
    nClipRegion(mRenderer, nRegion, op);
    return NOERROR;
}

ECode GLES20Canvas::GetClipBounds(
    /* [in] */ IRect* bounds,
    /* [out] */ Boolean* res)
{
    *res = nGetClipBounds(mRenderer, bounds);
    return NOERROR;
}

ECode GLES20Canvas::QuickReject(
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [in] */ CanvasEdgeType type,
    /* [out] */ Boolean* res)
{
    *res = nQuickReject(mRenderer, left, top, right, bottom);
    return NOERROR;
}

ECode GLES20Canvas::QuickReject(
    /* [in] */ IPath* path,
    /* [in] */ CanvasEdgeType type,
    /* [out] */ Boolean* res)
{
    AutoPtr<IRectF> pathBounds = GetPathBounds();
    path->ComputeBounds(pathBounds, TRUE);
    Float left, top, right, bottom;
    pathBounds->GetLeft(&left);
    pathBounds->GetLeft(&top);
    pathBounds->GetRight(&right);
    pathBounds->GetBottom(&bottom);
    *res = nQuickReject(mRenderer, left, top, right, bottom);
    return NOERROR;
}

ECode GLES20Canvas::QuickReject(
    /* [in] */ IRectF* rect,
    /* [in] */ CanvasEdgeType type,
    /* [out] */ Boolean* res)
{
    Float left, top, right, bottom;
    rect->GetLeft(&left);
    rect->GetLeft(&top);
    rect->GetRight(&right);
    rect->GetBottom(&bottom);
    nQuickReject(mRenderer, left, top, right, bottom);
    return NOERROR;
}

///////////////////////////////////////////////////////////////////////////
// Transformations
///////////////////////////////////////////////////////////////////////////

ECode GLES20Canvas::Translate(
    /* [in] */ Float dx,
    /* [in] */ Float dy)
{
    if (dx != 0.0f || dy != 0.0f) nTranslate(mRenderer, dx, dy);
    return NOERROR;
}

ECode GLES20Canvas::Skew(
    /* [in] */ Float sx,
    /* [in] */ Float sy)
{
    nSkew(mRenderer, sx, sy);
    return NOERROR;
}


ECode GLES20Canvas::Rotate(
    /* [in] */ Float degrees)
{
    nRotate(mRenderer, degrees);
    return NOERROR;
}

ECode GLES20Canvas::Scale(
    /* [in] */ Float sx,
    /* [in] */ Float sy)
{
    nScale(mRenderer, sx, sy);
    return NOERROR;
}

ECode GLES20Canvas::SetMatrix(
    /* [in] */ IMatrix* matrix)
{
    Matrix* m = (Matrix*)matrix;
    nSetMatrix(mRenderer, m == NULL ? 0 : m->mNativeInstance);
    return NOERROR;
}

ECode GLES20Canvas::GetMatrix(
    /* [in] */ IMatrix* matrix)
{
    Matrix* m = (Matrix*)matrix;
    nGetMatrix(mRenderer, m->mNativeInstance);
    return NOERROR;
}

ECode GLES20Canvas::Concat(
    /* [in] */ IMatrix* matrix)
{
    Matrix* m = (Matrix*)matrix;
    if (matrix != NULL) nConcatMatrix(mRenderer, m->mNativeInstance);
    return NOERROR;
}

///////////////////////////////////////////////////////////////////////////
// State management
///////////////////////////////////////////////////////////////////////////

ECode GLES20Canvas::Save(
    /* [out] */ Int32* res)
{
    *res = nSave(mRenderer, ICanvas::CLIP_SAVE_FLAG | ICanvas::MATRIX_SAVE_FLAG);
    return NOERROR;
}

ECode GLES20Canvas::Save(
    /* [in] */ Int32 saveFlags,
    /* [out] */ Int32* res)
{
    *res = nSave(mRenderer, saveFlags);
    return NOERROR;
}

ECode GLES20Canvas::SaveLayer(
    /* [in] */ IRectF* bounds,
    /* [in] */ IPaint* paint,
    /* [in] */ Int32 saveFlags,
    /* [out] */ Int32* res)
{
    if (bounds != NULL) {
        Float left, top, right, bottom;
        bounds->GetLeft(&left);
        bounds->GetLeft(&top);
        bounds->GetRight(&right);
        bounds->GetBottom(&bottom);
        return SaveLayer(left, top, right, bottom, paint, saveFlags, res);
    }

    Paint* p = (Paint*)paint;
    Int64 nativePaint = p == NULL ? 0 : p->mNativePaint;
    *res = nSaveLayer(mRenderer, nativePaint, saveFlags);
    return NOERROR;
}

ECode GLES20Canvas::SaveLayer(
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [in] */ IPaint* paint,
    /* [in] */ Int32 saveFlags,
    /* [out] */ Int32* res)
{
    if (left < right && top < bottom) {
        Paint* p = (Paint*)paint;
        Int64 nativePaint = p == NULL ? 0 : p->mNativePaint;
        *res = nSaveLayer(mRenderer, left, top, right, bottom, nativePaint, saveFlags);
        return NOERROR;
    }
    return Save(saveFlags, res);
}

ECode GLES20Canvas::SaveLayerAlpha(
    /* [in] */ IRectF* bounds,
    /* [in] */ Int32 alpha,
    /* [in] */ Int32 saveFlags,
    /* [out] */ Int32* res)
{
    if (bounds != NULL) {
        Float left, top, right, bottom;
        bounds->GetLeft(&left);
        bounds->GetLeft(&top);
        bounds->GetRight(&right);
        bounds->GetBottom(&bottom);
        return SaveLayerAlpha(left, top, right, bottom,
                alpha, saveFlags, res);
    }
    *res = nSaveLayerAlpha(mRenderer, alpha, saveFlags);
    return NOERROR;
}

ECode GLES20Canvas::SaveLayerAlpha(
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [in] */ Int32 alpha,
    /* [in] */ Int32 saveFlags,
    /* [out] */ Int32* res)
{
    if (left < right && top < bottom) {
        *res = nSaveLayerAlpha(mRenderer, left, top, right, bottom, alpha, saveFlags);
        return NOERROR;
    }
    return Save(saveFlags, res);
}

ECode GLES20Canvas::Restore()
{
    nRestore(mRenderer);
    return NOERROR;
}

ECode GLES20Canvas::RestoreToCount(
    /* [in] */ Int32 saveCount)
{
    nRestoreToCount(mRenderer, saveCount);
    return NOERROR;
}

ECode GLES20Canvas::GetSaveCount(
    /* [out] */ Int32* res)
{
    *res = nGetSaveCount(mRenderer);
    return NOERROR;
}

///////////////////////////////////////////////////////////////////////////
// Filtering
///////////////////////////////////////////////////////////////////////////

ECode GLES20Canvas::SetDrawFilter(
    /* [in] */ IDrawFilter* filter)
{
    mFilter = filter;
    if (filter == NULL) {
        nResetPaintFilter(mRenderer);
    } else if (IPaintFlagsDrawFilter::Probe(filter) != NULL) {
        IPaintFlagsDrawFilter* flagsFilter = IPaintFlagsDrawFilter::Probe(filter);
        CPaintFlagsDrawFilter* impl = (CPaintFlagsDrawFilter*)flagsFilter;
        nSetupPaintFilter(mRenderer, impl->mClearBits, impl->mSetBits);
    }
    return NOERROR;
}

ECode GLES20Canvas::GetDrawFilter(
    /* [out] */ IDrawFilter** filter)
{
    *filter = mFilter;
    REFCOUNT_ADD(*filter)

    return NOERROR;
}

///////////////////////////////////////////////////////////////////////////
// Drawing
///////////////////////////////////////////////////////////////////////////

ECode GLES20Canvas::DrawArc(
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [in] */ Float startAngle,
    /* [in] */ Float sweepAngle,
    /* [in] */ Boolean useCenter,
    /* [in] */ IPaint* paint)
{
    Paint* p = (Paint*)paint;
    nDrawArc(mRenderer, left, top, right, bottom,
            startAngle, sweepAngle, useCenter, p->mNativePaint);
    return NOERROR;
}

ECode GLES20Canvas::DrawARGB(
    /* [in] */ Int32 a,
    /* [in] */ Int32 r,
    /* [in] */ Int32 g,
    /* [in] */ Int32 b)
{
    return DrawColor((a & 0xFF) << 24 | (r & 0xFF) << 16 | (g & 0xFF) << 8 | (b & 0xFF));
}

ECode GLES20Canvas::DrawPatch(
    /* [in] */ INinePatch* patch,
    /* [in] */ IRect* dst,
    /* [in] */ IPaint* paint)
{
    AutoPtr<IBitmap> bitmap;
    patch->GetBitmap((IBitmap**)&bitmap);
    FAIL_RETURN(ThrowIfCannotDraw(bitmap))
    Paint* p = (Paint*)paint;
    Int64 nativePaint = p == NULL ? 0 : p->mNativePaint;
    CBitmap* nBitmap = (CBitmap*)bitmap.Get();
    NinePatch* nPatch = (NinePatch*)patch;
    Int32 left, top, right, bottom;
    dst->GetLeft(&left);
    dst->GetLeft(&top);
    dst->GetRight(&right);
    dst->GetBottom(&bottom);
    nDrawPatch(mRenderer, nBitmap->mNativeBitmap, nBitmap->mBuffer, nPatch->mNativeChunk,
            left, top, right, bottom, nativePaint);
    return NOERROR;
}

ECode GLES20Canvas::DrawPatch(
    /* [in] */ INinePatch* patch,
    /* [in] */ IRectF* dst,
    /* [in] */ IPaint* paint)
{
    AutoPtr<IBitmap> bitmap;
    patch->GetBitmap((IBitmap**)&bitmap);
    FAIL_RETURN(ThrowIfCannotDraw(bitmap))
    Paint* p = (Paint*)paint;
    Int64 nativePaint = p == NULL ? 0 : p->mNativePaint;
    CBitmap* nBitmap = (CBitmap*)bitmap.Get();
    NinePatch* nPatch = (NinePatch*)patch;
    Float left, top, right, bottom;
    dst->GetLeft(&left);
    dst->GetLeft(&top);
    dst->GetRight(&right);
    dst->GetBottom(&bottom);
    nDrawPatch(mRenderer, nBitmap->mNativeBitmap, nBitmap->mBuffer, nPatch->mNativeChunk,
            left, top, right, bottom, nativePaint);
    return NOERROR;
}

ECode GLES20Canvas::DrawBitmap(
    /* [in] */ IBitmap* bitmap,
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ IPaint* paint)
{
    FAIL_RETURN(ThrowIfCannotDraw(bitmap))
    Paint* p = (Paint*)paint;
    Int64 nativePaint = p == NULL ? 0 : p->mNativePaint;
    CBitmap* nBitmap = (CBitmap*)bitmap;
    nDrawBitmap(mRenderer, nBitmap->mNativeBitmap, nBitmap->mBuffer, left, top, nativePaint);
    return NOERROR;
}

ECode GLES20Canvas::DrawBitmap(
    /* [in] */ IBitmap* bitmap,
    /* [in] */ IMatrix* matrix,
    /* [in] */ IPaint* paint)
{
    FAIL_RETURN(ThrowIfCannotDraw(bitmap))
    Paint* p = (Paint*)paint;
    Int64 nativePaint = p == NULL ? 0 : p->mNativePaint;
    CBitmap* nBitmap = (CBitmap*)bitmap;
    Matrix* m = (Matrix*)matrix;
    nDrawBitmap(mRenderer, nBitmap->mNativeBitmap, nBitmap->mBuffer,
            m->mNativeInstance, nativePaint);
    return NOERROR;
}

ECode GLES20Canvas::DrawBitmap(
    /* [in] */ IBitmap* bitmap,
    /* [in] */ IRect* src,
    /* [in] */ IRect* dst,
    /* [in] */ IPaint* paint)
{
    FAIL_RETURN(ThrowIfCannotDraw(bitmap))
    Paint* p = (Paint*)paint;
    Int64 nativePaint = p == NULL ? 0 : p->mNativePaint;

    Int32 left, top, right, bottom;
    if (src == NULL) {
        left = top = 0;
        bitmap->GetWidth(&right);
        bitmap->GetHeight(&bottom);
    } else {
        src->GetLeft(&left);
        src->GetLeft(&top);
        src->GetRight(&right);
        src->GetBottom(&bottom);
    }

    Int32 dstLeft, dstTop, dstRight, dstBottom;
    dst->GetLeft(&dstLeft);
    dst->GetLeft(&dstTop);
    dst->GetRight(&dstRight);
    dst->GetBottom(&dstBottom);
    CBitmap* nBitmap = (CBitmap*)bitmap;
    nDrawBitmap(mRenderer, nBitmap->mNativeBitmap, nBitmap->mBuffer, left, top, right, bottom,
            dstLeft, dstTop, dstRight, dstBottom, nativePaint);
    return NOERROR;
}

ECode GLES20Canvas::DrawBitmap(
    /* [in] */ IBitmap* bitmap,
    /* [in] */ IRect* src,
    /* [in] */ IRectF* dst,
    /* [in] */ IPaint* paint)
{
    FAIL_RETURN(ThrowIfCannotDraw(bitmap))
    Paint* p = (Paint*)paint;
    Int64 nativePaint = p == NULL ? 0 : p->mNativePaint;

    Int32 left, top, right, bottom;
    if (src == NULL) {
        left = top = 0;
        bitmap->GetWidth(&right);
        bitmap->GetHeight(&bottom);
    } else {
        src->GetLeft(&left);
        src->GetLeft(&top);
        src->GetRight(&right);
        src->GetBottom(&bottom);
    }

    Float dstLeft, dstTop, dstRight, dstBottom;
    dst->GetLeft(&dstLeft);
    dst->GetLeft(&dstTop);
    dst->GetRight(&dstRight);
    dst->GetBottom(&dstBottom);
    CBitmap* nBitmap = (CBitmap*)bitmap;
    nDrawBitmap(mRenderer, nBitmap->mNativeBitmap, nBitmap->mBuffer, left, top, right, bottom,
            dstLeft, dstTop, dstRight, dstBottom, nativePaint);
    return NOERROR;
}

ECode GLES20Canvas::DrawBitmap(
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
    if (width < 0) {
        SLOGGERE("GLES20Canvas", "width must be >= 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (height < 0) {
        SLOGGERE("GLES20Canvas", "height must be >= 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (Elastos::Core::Math::Abs(stride) < width) {
        SLOGGERE("GLES20Canvas", "abs(stride) must be >= width")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Int32 lastScanline = offset + (height - 1) * stride;
    Int32 length = colors->GetLength();

    if (offset < 0 || (offset + width > length) || lastScanline < 0 ||
            (lastScanline + width > length)) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    Paint* p = (Paint*)paint;
    Int64 nativePaint = p == NULL ? 0 : p->mNativePaint;
    nDrawBitmap(mRenderer, colors, offset, stride, x, y,
            width, height, hasAlpha, nativePaint);
    return NOERROR;
}

ECode GLES20Canvas::DrawBitmap(
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
    return DrawBitmap(colors, offset, stride, (Float) x, (Float) y, width, height, hasAlpha, paint);
}

ECode GLES20Canvas::DrawBitmapMesh(
    /* [in] */ IBitmap* bitmap,
    /* [in] */ Int32 meshWidth,
    /* [in] */ Int32 meshHeight,
    /* [in] */ ArrayOf<Float>* verts,
    /* [in] */ Int32 vertOffset,
    /* [in] */ ArrayOf<Int32>* colors,
    /* [in] */ Int32 colorOffset,
    /* [in] */ IPaint* paint)
{
    FAIL_RETURN(ThrowIfCannotDraw(bitmap))
    if (meshWidth < 0 || meshHeight < 0 || vertOffset < 0 || colorOffset < 0) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    if (meshWidth == 0 || meshHeight == 0) {
        return NOERROR;
    }

    Int32 count = (meshWidth + 1) * (meshHeight + 1);
    FAIL_RETURN(CheckRange(verts->GetLength(), vertOffset, count * 2))

    if (colors != NULL) {
        FAIL_RETURN(CheckRange(colors->GetLength(), colorOffset, count))
    }

    Paint* p = (Paint*)paint;
    Int64 nativePaint = p == NULL ? 0 : p->mNativePaint;
    CBitmap* nBitmap = (CBitmap*)bitmap;
    nDrawBitmapMesh(mRenderer, nBitmap->mNativeBitmap, nBitmap->mBuffer, meshWidth, meshHeight,
            verts, vertOffset, colors, colorOffset, nativePaint);
    return NOERROR;
}

ECode GLES20Canvas::DrawCircle(
    /* [in] */ Float cx,
    /* [in] */ Float cy,
    /* [in] */ Float radius,
    /* [in] */ IPaint* paint)
{
    Paint* p = (Paint*)paint;
    nDrawCircle(mRenderer, cx, cy, radius, p->mNativePaint);
    return NOERROR;
}

ECode GLES20Canvas::DrawCircle(
    /* [in] */ ICanvasProperty* cx,
    /* [in] */ ICanvasProperty* cy,
    /* [in] */ ICanvasProperty* radius,
    /* [in] */ ICanvasProperty* paint)
{
    Int64 nCx, nCy, nRadius, nPaint;
    cx->GetNativeContainer(&nCx);
    cy->GetNativeContainer(&nCy);
    radius->GetNativeContainer(&nRadius);
    paint->GetNativeContainer(&nPaint);
    nDrawCircle(mRenderer, nCx, nCy, nRadius, nPaint);

    return NOERROR;
}

ECode GLES20Canvas::DrawRoundRect(
    /* [in] */ ICanvasProperty* left,
    /* [in] */ ICanvasProperty* top,
    /* [in] */ ICanvasProperty* right,
    /* [in] */ ICanvasProperty* bottom,
    /* [in] */ ICanvasProperty* rx,
    /* [in] */ ICanvasProperty* ry,
    /* [in] */ ICanvasProperty* paint)
{
    Int64 nLeft, nTop, nRight, nBottom, nRx, nRy, nPaint;
    left->GetNativeContainer(&nLeft);
    top->GetNativeContainer(&nTop);
    right->GetNativeContainer(&nRight);
    bottom->GetNativeContainer(&nBottom);
    rx->GetNativeContainer(&nRx);
    ry->GetNativeContainer(&nRy);
    paint->GetNativeContainer(&nPaint);
    nDrawRoundRect(mRenderer, nLeft, nTop, nRight, nBottom, nRx, nRy, nPaint);

    return NOERROR;
}

ECode GLES20Canvas::DrawColor(
    /* [in] */ Int32 color)
{
    return DrawColor(color, PorterDuffMode_SRC_OVER);
}

ECode GLES20Canvas::DrawColor(
    /* [in] */ Int32 color,
    /* [in] */ PorterDuffMode mode)
{
    nDrawColor(mRenderer, color, mode);
    return NOERROR;
}

ECode GLES20Canvas::DrawLine(
    /* [in] */ Float startX,
    /* [in] */ Float startY,
    /* [in] */ Float stopX,
    /* [in] */ Float stopY,
    /* [in] */ IPaint* paint)
{
    AutoPtr<ArrayOf<Float> > line = GetLineStorage();
    (*line)[0] = startX;
    (*line)[1] = startY;
    (*line)[2] = stopX;
    (*line)[3] = stopY;
    return DrawLines(line, 0, 4, paint);
}

ECode GLES20Canvas::DrawLines(
    /* [in] */ ArrayOf<Float>* pts,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ IPaint* paint)
{
    if (count < 4) return NOERROR;

    if ((offset | count) < 0 || offset + count > pts->GetLength()) {
        SLOGGERE("GLES20Canvas", "The lines array must contain 4 elements per line.")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Paint* p = (Paint*)paint;
    nDrawLines(mRenderer, pts, offset, count, p->mNativePaint);
    return NOERROR;
}

ECode GLES20Canvas::DrawLines(
    /* [in] */ ArrayOf<Float>* pts,
    /* [in] */ IPaint* paint)
{
    return DrawLines(pts, 0, pts->GetLength(), paint);
}

ECode GLES20Canvas::DrawOval(
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [in] */ IPaint* paint)
{
    Paint* p = (Paint*)paint;
    nDrawOval(mRenderer, left, top, right, bottom, p->mNativePaint);
    return NOERROR;
}

ECode GLES20Canvas::DrawPaint(
    /* [in] */ IPaint* paint)
{
    AutoPtr<IRect> r = GetInternalClipBounds();
    nGetClipBounds(mRenderer, r);
    Int32 left, top, right, bottom;
    r->GetLeft(&left);
    r->GetTop(&top);
    r->GetRight(&right);
    r->GetBottom(&bottom);
    return DrawRect(left, top, right, bottom, paint);
}

ECode GLES20Canvas::DrawPath(
    /* [in] */ IPath* path,
    /* [in] */ IPaint* paint)
{
    CPath* pathImpl = (CPath*)path;
    Paint* p = (Paint*)paint;
    if (pathImpl->mIsSimplePath) {
        if (pathImpl->mRects != NULL) {
            Handle64 nativeRegion;
            pathImpl->mRects->GetNativeRegion(&nativeRegion);
            nDrawRects(mRenderer, nativeRegion, p->mNativePaint);
        }
    } else {
        nDrawPath(mRenderer, pathImpl->mNativePath, p->mNativePaint);
    }
    return NOERROR;
}

ECode GLES20Canvas::DrawPicture(
    /* [in] */ IPicture* picture)
{
    picture->EndRecording();
    return NOERROR;
}

ECode GLES20Canvas::DrawPoint(
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ IPaint* paint)
{
    AutoPtr<ArrayOf<Float> > point = GetPointStorage();
    (*point)[0] = x;
    (*point)[1] = y;
    return DrawPoints(point, 0, 2, paint);
}

ECode GLES20Canvas::DrawPoints(
    /* [in] */ ArrayOf<Float>* pts,
    /* [in] */ IPaint* paint)
{
    return DrawPoints(pts, 0, pts->GetLength(), paint);
}

ECode GLES20Canvas::DrawPoints(
    /* [in] */ ArrayOf<Float>* pts,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ IPaint* paint)
{
    if (count < 2) return NOERROR;

    Paint* p = (Paint*)paint;
    nDrawPoints(mRenderer, pts, offset, count, p->mNativePaint);
    return NOERROR;
}

// Note: drawPosText just uses implementation in Canvas

ECode GLES20Canvas::DrawRect(
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [in] */ IPaint* paint)
{
    if (left == right || top == bottom) return NOERROR;

    Paint* p = (Paint*)paint;
    nDrawRect(mRenderer, left, top, right, bottom, p->mNativePaint);
    return NOERROR;
}

ECode GLES20Canvas::DrawRect(
    /* [in] */ IRect* r,
    /* [in] */ IPaint* paint)
{
    Int32 left, top, right, bottom;
    r->GetLeft(&left);
    r->GetLeft(&top);
    r->GetRight(&right);
    r->GetBottom(&bottom);
    return DrawRect(left, top, right, bottom, paint);
}

ECode GLES20Canvas::DrawRect(
    /* [in] */ IRectF* r,
    /* [in] */ IPaint* paint)
{
    Float left, top, right, bottom;
    r->GetLeft(&left);
    r->GetLeft(&top);
    r->GetRight(&right);
    r->GetBottom(&bottom);
    return DrawRect(left, top, right, bottom, paint);
}

ECode GLES20Canvas::DrawRGB(
    /* [in] */ Int32 r,
    /* [in] */ Int32 g,
    /* [in] */ Int32 b)
{
    return DrawColor(0xFF000000 | (r & 0xFF) << 16 | (g & 0xFF) << 8 | (b & 0xFF));
}

ECode GLES20Canvas::DrawRoundRect(
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [in] */ Float rx,
    /* [in] */ Float ry,
    /* [in] */ IPaint* paint)
{
    Paint* p = (Paint*)paint;
    nDrawRoundRect(mRenderer, left, top, right, bottom, rx, ry, p->mNativePaint);
    return NOERROR;
}

ECode GLES20Canvas::DrawText(
    /* [in] */ ArrayOf<Char32>* text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ IPaint* paint)
{
    if ((index | count | (index + count) | (text->GetLength() - index - count)) < 0) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    Paint* p = (Paint*)paint;
    nDrawText(mRenderer, text, index, count, x, y,
            p->mBidiFlags, p->mNativePaint, p->mNativeTypeface);
    return NOERROR;
}

ECode GLES20Canvas::DrawText(
    /* [in] */ ICharSequence* text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ IPaint* paint)
{
    if (IString::Probe(text) != NULL || ISpannedString::Probe(text) != NULL ||
            ISpannableString::Probe(text) != NULL) {
        String str;
        text->ToString(&str);
        Paint* p = (Paint*)paint;
        nDrawText(mRenderer, str, start, end, x, y, p->mBidiFlags,
                p->mNativePaint, p->mNativeTypeface);
    } else if (IGraphicsOperations::Probe(text) != NULL) {
        AutoPtr<IGraphicsOperations> gop = IGraphicsOperations::Probe(text);
        return gop->DrawText(this, start, end, x, y, paint);
    } else {
        AutoPtr<ArrayOf<Char32> > buf = TemporaryBuffer::Obtain(end - start);
        TextUtils::GetChars(text, start, end, buf, 0);
        Paint* p = (Paint*)paint;
        nDrawText(mRenderer, buf, 0, end - start, x, y,
                p->mBidiFlags, p->mNativePaint, p->mNativeTypeface);
        TemporaryBuffer::Recycle(buf);
    }
    return NOERROR;
}

ECode GLES20Canvas::DrawText(
    /* [in] */ const String& text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ IPaint* paint)
{
    if ((start | end | (end - start) | (text.GetLength() - end)) < 0) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    Paint* p = (Paint*)paint;
    nDrawText(mRenderer, text, start, end, x, y,
            p->mBidiFlags, p->mNativePaint, p->mNativeTypeface);
    return NOERROR;
}

ECode GLES20Canvas::DrawText(
    /* [in] */ const String& text,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ IPaint* paint)
{
    Paint* p = (Paint*)paint;
    nDrawText(mRenderer, text, 0, text.GetLength(), x, y,
        p->mBidiFlags, p->mNativePaint, p->mNativeTypeface);
    return NOERROR;
}

ECode GLES20Canvas::DrawTextOnPath(
    /* [in] */ ArrayOf<Char32>* text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [in] */ IPath* path,
    /* [in] */ Float hOffset,
    /* [in] */ Float vOffset,
    /* [in] */ IPaint* paint)
{
    if (index < 0 || index + count > text->GetLength()) {
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    Paint* p = (Paint*)paint;
    CPath* pathImpl = (CPath*)path;
    nDrawTextOnPath(mRenderer, text, index, count, pathImpl->mNativePath, hOffset, vOffset,
            p->mBidiFlags, p->mNativePaint, p->mNativeTypeface);
    return NOERROR;
}

ECode GLES20Canvas::DrawTextOnPath(
    /* [in] */ const String& text,
    /* [in] */ IPath* path,
    /* [in] */ Float hOffset,
    /* [in] */ Float vOffset,
    /* [in] */ IPaint* paint)
{
    if (text.GetLength() == 0) return NOERROR;

    Paint* p = (Paint*)paint;
    CPath* pathImpl = (CPath*)path;
    nDrawTextOnPath(mRenderer, text, 0, text.GetLength(), pathImpl->mNativePath, hOffset, vOffset,
            p->mBidiFlags, p->mNativePaint, p->mNativeTypeface);
    return NOERROR;
}

ECode GLES20Canvas::DrawTextRun(
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
    if ((index | count | text->GetLength() - index - count) < 0) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    Paint* p = (Paint*)paint;
    nDrawTextRun(mRenderer, text, index, count, contextIndex, contextCount, x, y, isRtl,
            p->mNativePaint, p->mNativeTypeface);
    return NOERROR;
}

ECode GLES20Canvas::DrawTextRun(
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
    Int32 textLen;
    text->GetLength(&textLen);
    if ((start | end | end - start | textLen - end) < 0) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    if (IString::Probe(text) != NULL || ISpannedString::Probe(text) != NULL ||
            ISpannableString::Probe(text) != NULL) {
        String str;
        text->ToString(&str);
        Paint* p = (Paint*)paint;
        nDrawTextRun(mRenderer, str, start, end, contextStart,
                contextEnd, x, y, isRtl, p->mNativePaint, p->mNativeTypeface);
    } else if (IGraphicsOperations::Probe(text) != NULL) {
        AutoPtr<IGraphicsOperations> gop = IGraphicsOperations::Probe(text);
        return gop->DrawTextRun(this, start, end,
                contextStart, contextEnd, x, y, isRtl, paint);
    } else {
        Int32 contextLen = contextEnd - contextStart;
        Int32 len = end - start;
        AutoPtr<ArrayOf<Char32> > buf = TemporaryBuffer::Obtain(contextLen);
        TextUtils::GetChars(text, contextStart, contextEnd, buf, 0);
        Paint* p = (Paint*)paint;
        nDrawTextRun(mRenderer, buf, start - contextStart, len, 0, contextLen,
                x, y, isRtl, p->mNativePaint, p->mNativeTypeface);
        TemporaryBuffer::Recycle(buf);
    }

    return NOERROR;
}

ECode GLES20Canvas::DrawVertices(
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
    // TODO: Implement
    return NOERROR;
}

ECode GLES20Canvas::GetRenderer(
    /* [out] */ Int64* renderer)
{
    *renderer = mRenderer;
    return NOERROR;
}

ECode GLES20Canvas::SetProperty(
    /* [in] */ const String& name,
    /* [in] */ const String& value)
{
    nSetProperty(name, value);
    return NOERROR;
}

GLES20Canvas::GLES20Canvas()
    : mFinalizer(0)
{}

void GLES20Canvas::nSetProperty(
    /* [in] */ const String& name,
    /* [in] */ const String& value)
{}

ECode GLES20Canvas::nFinishRecording(
    /* [in] */ Int64 renderer,
    /* [out] */ Int64* res)
{
    return NOERROR;
}

AutoPtr<IRect> GetInternalClipBounds()
{
    return NULL;
}

AutoPtr<IRectF> GetPathBounds()
{
    return NULL;
}

void SetupFinalizer()
{
}

AutoPtr<ArrayOf<Float> > GetPointStorage()
{
    return NULL;
}

AutoPtr<ArrayOf<Float> > GetLineStorage()
{
    return NULL;
}

Boolean GLES20Canvas::nIsAvailable()
{
    return FALSE;
}

Boolean GLES20Canvas::IsAvailable()
{
    return FALSE;
}

Int64 GLES20Canvas::nCreateDisplayListRenderer()
{
    return 0;
}

void GLES20Canvas::nResetDisplayListRenderer(
    /* [in] */ Int64 renderer)
{}

void GLES20Canvas::nDestroyRenderer(
    /* [in] */ Int64 renderer)
{}

Int32 GLES20Canvas::nGetMaximumTextureWidth()
{
    return 0;
}

Int32 GLES20Canvas::nGetMaximumTextureHeight()
{
    return 0;
}

void GLES20Canvas::nSetViewport(
    /* [in] */ Int64 renderer,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{}

void GLES20Canvas::nSetHighContrastText(
    /* [in] */ Int64 renderer,
    /* [in] */ Boolean highContrastText)
{}

void GLES20Canvas::nInsertReorderBarrier(
    /* [in] */ Int64 renderer,
    /* [in] */ Boolean enableReorder)
{}

Int32 GLES20Canvas::nPrepare(
    /* [in] */ Int64 renderer,
    /* [in] */ Boolean opaque)
{
    return 0;
}

Int32 GLES20Canvas::nPrepareDirty(
    /* [in] */ Int64 renderer,
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom,
    /* [in] */ Boolean opaque)
{
    return 0;
}

void GLES20Canvas::nFinish(
    /* [in] */ Int64 renderer)
{}

Int32 GLES20Canvas::nCallDrawGLFunction(
    /* [in] */ Int64 renderer,
    /* [in] */ Int64 drawGLFunction)
{
    return 0;
}

Int32 GLES20Canvas::nDrawRenderNode(
    /* [in] */ Int64 renderer,
    /* [in] */ Int64 renderNode,
    /* [in] */ IRect* dirty,
    /* [in] */ Int32 flags)
{
    return 0;
}

void GLES20Canvas::nDrawLayer(
    /* [in] */ Int64 renderer,
    /* [in] */ Int64 layer,
    /* [in] */ Float x,
    /* [in] */ Float y)
{}

Boolean GLES20Canvas::nClipPath(
    /* [in] */ Int64 renderer,
    /* [in] */ Int64 path,
    /* [in] */ Int32 op)
{
    return FALSE;
}

Boolean GLES20Canvas::nClipRect(
    /* [in] */ Int64 renderer,
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [in] */ Int32 op)
{
    return FALSE;
}

Boolean GLES20Canvas::nClipRect(
    /* [in] */ Int64 renderer,
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom,
    /* [in] */ Int32 op)
{
    return FALSE;
}

Boolean GLES20Canvas::nClipRegion(
    /* [in] */ Int64 renderer,
    /* [in] */ Int64 region,
    /* [in] */ Int32 op)
{
    return FALSE;
}

Boolean GLES20Canvas::nGetClipBounds(
    /* [in] */ Int64 renderer,
    /* [in] */ IRect* bounds)
{
    return FALSE;
}

Boolean GLES20Canvas::nQuickReject(
    /* [in] */ Int64 renderer,
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom)
{
    return FALSE;
}

void GLES20Canvas::nTranslate(
    /* [in] */ Int64 renderer,
    /* [in] */ Float dx,
    /* [in] */ Float dy)
{}

void GLES20Canvas::nSkew(
    /* [in] */ Int64 renderer,
    /* [in] */ Float sx,
    /* [in] */ Float sy)
{}

void GLES20Canvas::nRotate(
    /* [in] */ Int64 renderer,
    /* [in] */ Float degrees)
{}


void GLES20Canvas::nScale(
    /* [in] */ Int64 renderer,
    /* [in] */ Float sx,
    /* [in] */ Float sy)
{}

void GLES20Canvas::nSetMatrix(
    /* [in] */ Int64 renderer,
    /* [in] */ Int64 matrix)
{}

void GLES20Canvas::nGetMatrix(
    /* [in] */ Int64 renderer,
    /* [in] */ Int64 matrix)
{}

void GLES20Canvas::nConcatMatrix(
    /* [in] */ Int64 renderer,
    /* [in] */ Int64 matrix)
{}

Int32 GLES20Canvas::nSave(
    /* [in] */ Int64 renderer,
    /* [in] */ Int32 flags)
{
    return 0;
}

Int32 GLES20Canvas::nSaveLayer(
    /* [in] */ Int64 renderer,
    /* [in] */ Int64 paint,
    /* [in] */ Int32 saveFlags)
{
    return 0;
}

Int32 GLES20Canvas::nSaveLayer(
    /* [in] */ Int64 renderer,
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [in] */ Int64 paint,
    /* [in] */ Int32 saveFlags)
{
    return 0;
}

Int32 GLES20Canvas::nSaveLayerAlpha(
    /* [in] */ Int64 renderer,
    /* [in] */ Int32 alpha,
    /* [in] */ Int32 saveFlags)
{
    return 0;
}

Int32 GLES20Canvas::nSaveLayerAlpha(
    /* [in] */ Int64 renderer,
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [in] */ Int32 alpha,
    /* [in] */ Int32 saveFlags)
{
    return 0;
}

void GLES20Canvas::nRestore(
    /* [in] */ Int64 renderer)
{}

void GLES20Canvas::nRestoreToCount(
    /* [in] */ Int64 renderer,
    /* [in] */ Int32 saveCount)
{}

Int32 GLES20Canvas::nGetSaveCount(
    /* [in] */ Int64 renderer)
{
    return 0;
}

void GLES20Canvas::nResetPaintFilter(
    /* [in] */ Int64 renderer)
{}

void GLES20Canvas::nSetupPaintFilter(
    /* [in] */ Int64 renderer,
    /* [in] */ Int32 clearBits,
    /* [in] */ Int32 setBits)
{}

void GLES20Canvas::nDrawArc(
    /* [in] */ Int64 renderer,
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [in] */ Float startAngle,
    /* [in] */ Float sweepAngle,
    /* [in] */ Boolean useCenter,
    /* [in] */ Int64 paint)
{}

void GLES20Canvas::nDrawPatch(
    /* [in] */ Int64 renderer,
    /* [in] */ Int64 bitmap,
    /* [in] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int64 chunk,
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [in] */ Int64 paint)
{}

void GLES20Canvas::nDrawBitmap(
    /* [in] */ Int64 renderer,
    /* [in] */ Int64 bitmap,
    /* [in] */ ArrayOf<Byte>* buffer,
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Int64 paint)
{}

void GLES20Canvas::nDrawBitmap(
    /* [in] */ Int64 renderer,
    /* [in] */ Int64 bitmap,
    /* [in] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int64 matrix,
    /* [in] */ Int64 paint)
{}

void GLES20Canvas::nDrawBitmap(
    /* [in] */ Int64 renderer,
    /* [in] */ Int64 bitmap,
    /* [in] */ ArrayOf<Byte>* buffer,
    /* [in] */ Float srcLeft,
    /* [in] */ Float srcTop,
    /* [in] */ Float srcRight,
    /* [in] */ Float srcBottom,
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [in] */ Int64 paint)
{}


void GLES20Canvas::nDrawBitmap(
    /* [in] */ Int64 renderer,
    /* [in] */ ArrayOf<Int32>* colors,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 stride,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Boolean hasAlpha,
    /* [in] */ Int64 nativePaint)
{}

void GLES20Canvas::nDrawBitmapMesh(
    /* [in] */ Int64 renderer,
    /* [in] */ Int64 bitmap,
    /* [in] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 meshWidth,
    /* [in] */ Int32 meshHeight,
    /* [in] */ ArrayOf<Float>* verts,
    /* [in] */ Int32 vertOffset,
    /* [in] */ ArrayOf<Int32>* colors,
    /* [in] */ Int32 colorOffset,
    /* [in] */ Int64 paint)
{}

void GLES20Canvas::nDrawCircle(
    /* [in] */ Int64 renderer,
    /* [in] */ Float cx,
    /* [in] */ Float cy,
    /* [in] */ Float radius,
    /* [in] */ Int64 paint)
{}

void GLES20Canvas::nDrawCircle(
    /* [in] */ Int64 renderer,
    /* [in] */ Int64 propCx,
    /* [in] */ Int64 propCy,
    /* [in] */ Int64 propRadius,
    /* [in] */ Int64 propPaint)
{}

void GLES20Canvas::nDrawRoundRect(
    /* [in] */ Int64 renderer,
    /* [in] */ Int64 propLeft,
    /* [in] */ Int64 propTop,
    /* [in] */ Int64 propRight,
    /* [in] */ Int64 propBottom,
    /* [in] */ Int64 propRx,
    /* [in] */ Int64 propRy,
    /* [in] */ Int64 propPaint)
{}

void GLES20Canvas::nDrawColor(
    /* [in] */ Int64 renderer,
    /* [in] */ Int32 color,
    /* [in] */ Int32 mode)
{}

void GLES20Canvas::nDrawLines(
    /* [in] */ Int64 renderer,
    /* [in] */ ArrayOf<Float>* points,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ Int64 paint)
{}

void GLES20Canvas::nDrawOval(
    /* [in] */ Int64 renderer,
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [in] */ Int64 paint)
{}

void GLES20Canvas::nDrawPath(
    /* [in] */ Int64 renderer,
    /* [in] */ Int64 path,
    /* [in] */ Int64 paint)
{}

void GLES20Canvas::nDrawRects(
    /* [in] */ Int64 renderer,
    /* [in] */ Int64 region,
    /* [in] */ Int64 paint)
{}

void GLES20Canvas::nDrawPoints(
    /* [in] */ Int64 renderer,
    /* [in] */ ArrayOf<Float>* points,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count,
    /* [in] */ Int64 paint)
{}

void GLES20Canvas::nDrawRect(
    /* [in] */ Int64 renderer,
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [in] */ Int64 paint)
{}

void GLES20Canvas::nDrawRoundRect(
    /* [in] */ Int64 renderer,
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [in] */ Float rx,
    /* [in] */ Float y,
    /* [in] */ Int64 paint)
{}

void GLES20Canvas::nDrawText(
    /* [in] */ Int64 renderer,
    /* [in] */ ArrayOf<Char32>* text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Int32 bidiFlags,
    /* [in] */ Int64 paint,
    /* [in] */ Int64 typeface)
{}

void GLES20Canvas::nDrawText(
    /* [in] */ Int64 renderer,
    /* [in] */ const String& text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Int32 bidiFlags,
    /* [in] */ Int64 paint,
    /* [in] */ Int64 typeface)
{}

void GLES20Canvas::nDrawTextOnPath(
    /* [in] */ Int64 renderer,
    /* [in] */ ArrayOf<Char32>* text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [in] */ Int64 path,
    /* [in] */ Float hOffset,
    /* [in] */ Float vOffset,
    /* [in] */ Int32 bidiFlags,
    /* [in] */ Int64 nativePaint,
    /* [in] */ Int64 typeface)
{}

void GLES20Canvas::nDrawTextOnPath(
    /* [in] */ Int64 renderer,
    /* [in] */ const String& text,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ Int64 path,
    /* [in] */ Float hOffset,
    /* [in] */ Float vOffset,
    /* [in] */ Int32 bidiFlags,
    /* [in] */ Int64 nativePaint,
    /* [in] */ Int64 typeface)
{}

void GLES20Canvas::nDrawTextRun(
    /* [in] */ Int64 renderer,
    /* [in] */ ArrayOf<Char32>* text,
    /* [in] */ Int32 index,
    /* [in] */ Int32 count,
    /* [in] */ Int32 contextIndex,
    /* [in] */ Int32 contextCount,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Boolean isRtl,
    /* [in] */ Int64 nativePaint,
    /* [in] */ Int64 nativeTypeface)
{}

void GLES20Canvas::nDrawTextRun(
    /* [in] */ Int64 renderer,
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
{}

} // namespace View
} // namespace Droid
} // namespace Elastos