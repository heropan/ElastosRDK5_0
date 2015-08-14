

#ifndef __ELASTOS_DROID_GRAPHICS_H_H__
#define __ELASTOS_DROID_GRAPHICS_H_H__

#include "graphics/CBitmap.h"

using Elastos::Core::ICharSequence;

namespace Elastos {
namespace Droid {
namespace Graphics {

extern const InterfaceID EIID_Canvas;

class Canvas
{
public:
    Canvas();

    virtual ~Canvas();

public:
    virtual CARAPI IsHardwareAccelerated(
        /* [out] */ Boolean* isAccelerated);

    virtual CARAPI SetBitmap(
        /* [in] */ IBitmap* bitmap);

    virtual CARAPI SetViewport(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    virtual CARAPI IsOpaque(
        /* [out] */ Boolean* isOpaque);

    virtual CARAPI GetWidth(
        /* [out] */ Int32* width);

    virtual CARAPI GetHeight(
        /* [out] */ Int32* height);

    virtual CARAPI GetDensity(
        /* [out] */ Int32* density);

    virtual CARAPI SetDensity(
        /* [in] */ Int32 density);

    virtual CARAPI SetScreenDensity(
        /* [in] */ Int32 density);

    virtual CARAPI GetMaximumBitmapWidth(
        /* [out] */ Int32* width);

    virtual CARAPI GetMaximumBitmapHeight(
        /* [out] */ Int32* height);

    virtual CARAPI Save(
        /* [out] */ Int32* count);

    virtual CARAPI Save(
        /* [in] */ Int32 saveFlags,
        /* [out] */ Int32* count);

    virtual CARAPI SaveLayer(
        /* [in] */ IRectF* bounds,
        /* [in] */ IPaint* paint,
        /* [in] */ Int32 saveFlags,
        /* [out] */ Int32* count);

    virtual CARAPI SaveLayer(
        /* [in] */ Float left,
        /* [in] */ Float top,
        /* [in] */ Float right,
        /* [in] */ Float bottom,
        /* [in] */ IPaint* paint,
        /* [in] */ Int32 saveFlags,
        /* [out] */ Int32* count);

    virtual CARAPI SaveLayerAlpha(
        /* [in] */ IRectF* bounds,
        /* [in] */ Int32 alpha,
        /* [in] */ Int32 saveFlags,
        /* [out] */ Int32* count);

    virtual CARAPI SaveLayerAlpha(
        /* [in] */ Float left,
        /* [in] */ Float top,
        /* [in] */ Float right,
        /* [in] */ Float bottom,
        /* [in] */ Int32 alpha,
        /* [in] */ Int32 saveFlags,
        /* [out] */ Int32* count);

    virtual CARAPI Restore();

    virtual CARAPI GetSaveCount(
        /* [out] */ Int32* count);

    virtual CARAPI RestoreToCount(
        /* [in] */ Int32 saveCount);

    virtual CARAPI Translate(
        /* [in] */ Float dx,
        /* [in] */ Float dy);

    virtual CARAPI Scale(
        /* [in] */ Float sx,
        /* [in] */ Float sy);

    CARAPI Scale(
        /* [in] */ Float sx,
        /* [in] */ Float sy,
        /* [in] */ Float px,
        /* [in] */ Float py);

    virtual CARAPI Rotate(
        /* [in] */ Float degrees);

    CARAPI Rotate(
        /* [in] */ Float degrees,
        /* [in] */ Float px,
        /* [in] */ Float py);

    virtual CARAPI Skew(
        /* [in] */ Float sx,
        /* [in] */ Float sy);

    virtual CARAPI Concat(
        /* [in] */ IMatrix* matrix);

    virtual CARAPI SetMatrix(
        /* [in] */ IMatrix* matrix);

    virtual CARAPI GetMatrix(
        /* [in] */ IMatrix* ctm);

    virtual CARAPI GetMatrix(
        /* [out] */ IMatrix** metrix);

    virtual CARAPI ClipRect(
        /* [in] */ IRectF* rect,
        /* [in] */ RegionOp op,
        /* [out] */ Boolean* isNotEmpty);

    virtual CARAPI ClipRect(
        /* [in] */ IRect* rect,
        /* [in] */ RegionOp op,
        /* [out] */ Boolean* isNotEmpty);

    virtual CARAPI ClipRect(
        /* [in] */ IRectF* rect,
        /* [out] */ Boolean* isNotEmpty);

    virtual CARAPI ClipRect(
        /* [in] */ IRect* rect,
        /* [out] */ Boolean* isNotEmpty);

    virtual CARAPI ClipRect(
        /* [in] */ Float left,
        /* [in] */ Float top,
        /* [in] */ Float right,
        /* [in] */ Float bottom,
        /* [in] */ RegionOp op,
        /* [out] */ Boolean* isNotEmpty);

    virtual CARAPI ClipRect(
        /* [in] */ Float left,
        /* [in] */ Float top,
        /* [in] */ Float right,
        /* [in] */ Float bottom,
        /* [out] */ Boolean* isNotEmpty);

    virtual CARAPI ClipRect(
        /* [in] */ Int32 left,
        /* [in] */ Int32 top,
        /* [in] */ Int32 right,
        /* [in] */ Int32 bottom,
        /* [out] */ Boolean* isNotEmpty);

    virtual CARAPI ClipPath(
        /* [in] */ IPath* path,
        /* [in] */ RegionOp op,
        /* [out] */ Boolean* isNotEmpty);

    virtual CARAPI ClipPath(
        /* [in] */ IPath* path,
        /* [out] */ Boolean* isNotEmpty);

    virtual CARAPI ClipRegion(
        /* [in] */ IRegion* region,
        /* [in] */ RegionOp op,
        /* [out] */ Boolean* isNotEmpty);

    virtual CARAPI ClipRegion(
        /* [in] */ IRegion* region,
        /* [out] */ Boolean* isNotEmpty);

    virtual CARAPI GetDrawFilter(
        /* [out] */ IDrawFilter** filter);

    virtual CARAPI SetDrawFilter(
        /* [in] */ IDrawFilter* filter);

    virtual CARAPI QuickReject(
        /* [in] */ IRectF* rect,
        /* [in] */ CanvasEdgeType type,
        /* [out] */ Boolean* isNotIntersect);

    virtual CARAPI QuickReject(
        /* [in] */ IPath* path,
        /* [in] */ CanvasEdgeType type,
        /* [out] */ Boolean* isNotIntersect);

    virtual CARAPI QuickReject(
        /* [in] */ Float left,
        /* [in] */ Float top,
        /* [in] */ Float right,
        /* [in] */ Float bottom,
        /* [in] */ CanvasEdgeType type,
        /* [out] */ Boolean* isNotIntersect);

    virtual CARAPI GetClipBounds(
        /* [in] */ IRect* bounds,
        /* [out] */ Boolean* isNotEmpty);

    virtual CARAPI GetClipBounds(
        /* [out] */ IRect** bounds);

    virtual CARAPI DrawRGB(
        /* [in] */ Int32 r,
        /* [in] */ Int32 g,
        /* [in] */ Int32 b);

    virtual CARAPI DrawARGB(
        /* [in] */ Int32 a,
        /* [in] */ Int32 r,
        /* [in] */ Int32 g,
        /* [in] */ Int32 b);

    virtual CARAPI DrawColor(
        /* [in] */ Int32 color);

    virtual CARAPI DrawColor(
        /* [in] */ Int32 color,
        /* [in] */ PorterDuffMode mode);

    virtual CARAPI DrawPaint(
        /* [in] */ IPaint* paint);

    virtual CARAPI DrawPoints(
        /* [in] */ const ArrayOf<Float>& pts,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count,
        /* [in] */ IPaint* paint);

    virtual CARAPI DrawPoints(
        /* [in] */ const ArrayOf<Float>& pts,
        /* [in] */ IPaint* paint);

    virtual CARAPI DrawPoint(
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ IPaint* paint);

    virtual CARAPI DrawLine(
        /* [in] */ Float startX,
        /* [in] */ Float startY,
        /* [in] */ Float stopX,
        /* [in] */ Float stopY,
        /* [in] */ IPaint* paint);

    virtual CARAPI DrawLines(
        /* [in] */ const ArrayOf<Float>& pts,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count,
        /* [in] */ IPaint* paint);

    virtual CARAPI DrawLines(
        /* [in] */ const ArrayOf<Float>& pts,
        /* [in] */ IPaint* paint);

    virtual CARAPI DrawRect(
        /* [in] */ IRectF* rect,
        /* [in] */ IPaint* paint);

    virtual CARAPI DrawRect(
        /* [in] */ IRect* r,
        /* [in] */ IPaint* paint);

    virtual CARAPI DrawRect(
        /* [in] */ Float left,
        /* [in] */ Float top,
        /* [in] */ Float right,
        /* [in] */ Float bottom,
        /* [in] */ IPaint* paint);

    virtual CARAPI DrawOval(
        /* [in] */ IRectF* oval,
        /* [in] */ IPaint* paint);

    virtual CARAPI DrawCircle(
        /* [in] */ Float cx,
        /* [in] */ Float cy,
        /* [in] */ Float radius,
        /* [in] */ IPaint* paint);

    virtual CARAPI DrawArc(
        /* [in] */ IRectF* oval,
        /* [in] */ Float startAngle,
        /* [in] */ Float sweepAngle,
        /* [in] */ Boolean useCenter,
        /* [in] */ IPaint* paint);

    virtual CARAPI DrawRoundRect(
        /* [in] */ IRectF* rect,
        /* [in] */ Float rx,
        /* [in] */ Float ry,
        /* [in] */ IPaint* paint);

    virtual CARAPI DrawPath(
        /* [in] */ IPath* path,
        /* [in] */ IPaint* paint);

    virtual CARAPI DrawPatch(
        /* [in] */ IBitmap* bitmap,
        /* [in] */ const ArrayOf<Byte>& chunks,
        /* [in] */ IRectF* dst,
        /* [in] */ IPaint* paint);

    virtual CARAPI DrawBitmap(
        /* [in] */ IBitmap* bitmap,
        /* [in] */ Float left,
        /* [in] */ Float top,
        /* [in] */ IPaint* paint);

    virtual CARAPI DrawBitmap(
        /* [in] */ IBitmap* bitmap,
        /* [in] */ IRect* src,
        /* [in] */ IRectF* dst,
        /* [in] */ IPaint* paint);

    virtual CARAPI DrawBitmap(
        /* [in] */ IBitmap* bitmap,
        /* [in] */ IRect* src,
        /* [in] */ IRect* dst,
        /* [in] */ IPaint* paint);

    virtual CARAPI DrawBitmap(
        /* [in] */ const ArrayOf<Int32>& colors,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 stride,
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Boolean hasAlpha,
        /* [in] */ IPaint* paint);

    virtual CARAPI DrawBitmap(
        /* [in] */ const ArrayOf<Int32>& colors,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 stride,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Boolean hasAlpha,
        /* [in] */ IPaint* paint);

    virtual CARAPI DrawBitmap(
        /* [in] */ IBitmap* bitmap,
        /* [in] */ IMatrix* matrix,
        /* [in] */ IPaint* paint);

    virtual CARAPI DrawBitmapMesh(
        /* [in] */ IBitmap* bitmap,
        /* [in] */ Int32 meshWidth,
        /* [in] */ Int32 meshHeight,
        /* [in] */ const ArrayOf<Float>& verts,
        /* [in] */ Int32 vertOffset,
        /* [in] */ ArrayOf<Int32>* colors,
        /* [in] */ Int32 colorOffset,
        /* [in] */ IPaint* paint);

    virtual CARAPI DrawVertices(
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
        /* [in] */ IPaint* paint);

    virtual CARAPI DrawText(
        /* [in] */ const ArrayOf<Char32>& text,
        /* [in] */ Int32 index,
        /* [in] */ Int32 count,
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ IPaint* paint);

    virtual CARAPI DrawText(
        /* [in] */ const String& text,
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ IPaint* paint);

    virtual CARAPI DrawText(
        /* [in] */ const String& text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ IPaint* paint);

    virtual CARAPI DrawText(
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ IPaint* paint);

    virtual CARAPI DrawTextRun(
        /* [in] */ const ArrayOf<Char32>& text,
        /* [in] */ Int32 index,
        /* [in] */ Int32 count,
        /* [in] */ Int32 contextIndex,
        /* [in] */ Int32 contextCount,
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ Int32 dir,
        /* [in] */ IPaint* paint);

    virtual CARAPI DrawTextRun(
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Int32 contextStart,
        /* [in] */ Int32 contextEnd,
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ Int32 dir,
        /* [in] */ IPaint* paint);

    virtual CARAPI DrawPosText(
        /* [in] */ const ArrayOf<Char32>& text,
        /* [in] */ Int32 index,
        /* [in] */ Int32 count,
        /* [in] */ const ArrayOf<Float>& pos,
        /* [in] */ IPaint* paint);

    virtual CARAPI DrawPosText(
        /* [in] */ const String& text,
        /* [in] */ const ArrayOf<Float>& pos,
        /* [in] */ IPaint* paint);

    virtual CARAPI DrawTextOnPath(
        /* [in] */ const ArrayOf<Char32>& text,
        /* [in] */ Int32 index,
        /* [in] */ Int32 count,
        /* [in] */ IPath* path,
        /* [in] */ Float hOffset,
        /* [in] */ Float vOffset,
        /* [in] */ IPaint* paint);

    virtual CARAPI DrawTextOnPath(
        /* [in] */ const String& text,
        /* [in] */ IPath* path,
        /* [in] */ Float hOffset,
        /* [in] */ Float vOffset,
        /* [in] */ IPaint* paint);

    virtual CARAPI DrawPicture(
        /* [in] */ IPicture* picture);

    virtual CARAPI DrawPicture(
        /* [in] */ IPicture* picture,
        /* [in] */ IRectF* dst);

    virtual CARAPI DrawPicture(
        /* [in] */ IPicture* picture,
        /* [in] */ IRect* dst);

    CARAPI SetSurfaceFormat(
        /* [in] */ Int32 format);

    CARAPI GetNativeCanvas(
        /* [out] */ Handle32* natvieCanvas);

    static CARAPI_(void) FreeCaches();

    static CARAPI_(void) FreeTextLayoutCaches();

protected:
    CARAPI Init();

    CARAPI Init(
        /* [in] */ IBitmap* bitmap);

    CARAPI Init(
        /* [in] */ Int32 nativeCanvas);

    // virtual CARAPI_(AutoPtr<IGL>) GetGL();

    static CARAPI CheckRange(
        /* [in] */ Int32 length,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count);

private:
    static CARAPI ThrowIfRecycled(
        /* [in] */ IBitmap* bitmap);

    CARAPI_(Int32) InitRaster(
        /* [in] */ Int32 bitmap);

    static CARAPI_(void) NativeSetBitmap(
        /* [in] */ Int32 nativeCanvas,
        /* [in] */ Int32 nativeBitmap);

    static CARAPI_(Int32) NativeSaveLayer(
        /* [in] */ Int32 nativeCanvas,
        /* [in] */ IRectF* bounds,
        /* [in] */ Int32 nativePaint,
        /* [in] */ Int32 flags);

    static CARAPI_(Int32) NativeSaveLayer(
        /* [in] */ Int32 nativeCanvas,
        /* [in] */ Float l,
        /* [in] */ Float t,
        /* [in] */ Float r,
        /* [in] */ Float b,
        /* [in] */ Int32 nativePaint,
        /* [in] */ Int32 flags);

    static CARAPI_(Int32) NativeSaveLayerAlpha(
        /* [in] */ Int32 nativeCanvas,
        /* [in] */ IRectF* bounds,
        /* [in] */ Int32 alpha,
        /* [in] */ Int32 flags);

    static CARAPI_(Int32) NativeSaveLayerAlpha(
        /* [in] */ Int32 nativeCanvas,
        /* [in] */ Float l,
        /* [in] */ Float t,
        /* [in] */ Float r,
        /* [in] */ Float b,
        /* [in] */ Int32 alpha,
        /* [in] */ Int32 flags);

    static CARAPI_(void) NativeConcat(
        /* [in] */ Int32 nativeCanvas,
        /* [in] */ Int32 nativeMatrix);

    static CARAPI_(void) NativeSetMatrix(
        /* [in] */ Int32 nativeCanvas,
        /* [in] */ Int32 nativeMatrix);

    static CARAPI_(Boolean) NativeClipRect(
        /* [in] */ Int32 nativeCanvas,
        /* [in] */ Float left,
        /* [in] */ Float top,
        /* [in] */ Float right,
        /* [in] */ Float bottom,
        /* [in] */ RegionOp op);

    static CARAPI_(Boolean) NativeClipPath(
        /* [in] */ Int32 nativeCanvas,
        /* [in] */ Int32 nativePath,
        /* [in] */ RegionOp op);

    static CARAPI_(Boolean) NativeClipRegion(
        /* [in] */ Int32 nativeCanvas,
        /* [in] */ Int32 nativeRegion,
        /* [in] */ RegionOp op);

    static CARAPI_(void) NativeSetDrawFilter(
        /* [in] */ Int32 nativeCanvas,
        /* [in] */ Int32 nativeFilter);

    static CARAPI_(Boolean) NativeGetClipBounds(
        /* [in] */ Int32 nativeCanvas,
        /* [in] */ IRect* bounds);

    static CARAPI_(void) NativeGetCTM(
        /* [in] */ Int32 nativeCanvas,
        /* [in] */ Int32 nativeMatrix);

    static CARAPI_(Boolean) NativeQuickReject(
        /* [in] */ Int32 nativeCanvas,
        /* [in] */ IRectF* rect,
        /* [in] */ CanvasEdgeType edgeType);

    static CARAPI_(Boolean) NativeQuickReject(
        /* [in] */ Int32 nativeCanvas,
        /* [in] */ Int32 nativePath,
        /* [in] */ CanvasEdgeType edgeType);

    static CARAPI_(Boolean) NativeQuickReject(
        /* [in] */ Int32 nativeCanvas,
        /* [in] */ Float left,
        /* [in] */ Float top,
        /* [in] */ Float right,
        /* [in] */ Float bottom,
        /* [in] */ CanvasEdgeType edgeType);

    static CARAPI_(void) NativeDrawRGB(
        /* [in] */ Int32 nativeCanvas,
        /* [in] */ Int32 r,
        /* [in] */ Int32 g,
        /* [in] */ Int32 b);

    static CARAPI_(void) NativeDrawARGB(
        /* [in] */ Int32 nativeCanvas,
        /* [in] */ Int32 a,
        /* [in] */ Int32 r,
        /* [in] */ Int32 g,
        /* [in] */ Int32 b);

    static CARAPI_(void) NativeDrawColor(
        /* [in] */ Int32 nativeCanvas,
        /* [in] */ Int32 color);

    static CARAPI_(void) NativeDrawColor(
        /* [in] */ Int32 nativeCanvas,
        /* [in] */ Int32 color,
        /* [in] */ PorterDuffMode mode);

    static CARAPI_(void) NativeDrawPaint(
        /* [in] */ Int32 nativeCanvas,
        /* [in] */ Int32 nativePaint);

    static CARAPI_(void) NativeDrawLine(
        /* [in] */ Int32 nativeCanvas,
        /* [in] */ Float startX,
        /* [in] */ Float startY,
        /* [in] */ Float stopX,
        /* [in] */ Float stopY,
        /* [in] */ Int32 nativePaint);

    static CARAPI_(void) NativeDrawRect(
        /* [in] */ Int32 nativeCanvas,
        /* [in] */ IRectF* rect,
        /* [in] */ Int32 nativePaint);

    static CARAPI_(void) NativeDrawRect(
        /* [in] */ Int32 nativeCanvas,
        /* [in] */ Float left,
        /* [in] */ Float top,
        /* [in] */ Float right,
        /* [in] */ Float bottom,
        /* [in] */ Int32 nativePaint);

    static CARAPI_(void) NativeDrawOval(
        /* [in] */ Int32 nativeCanvas,
        /* [in] */ IRectF* oval,
        /* [in] */ Int32 nativePaint);

    static CARAPI_(void) NativeDrawCircle(
        /* [in] */ Int32 nativeCanvas,
        /* [in] */ Float cx,
        /* [in] */ Float cy,
        /* [in] */ Float radius,
        /* [in] */ Int32 nativePaint);

    static CARAPI_(void) NativeDrawArc(
        /* [in] */ Int32 nativeCanvas,
        /* [in] */ IRectF* oval,
        /* [in] */ Float startAngle,
        /* [in] */ Float sweepAngle,
        /* [in] */ Boolean useCenter,
        /* [in] */ Int32 nativePaint);

    static CARAPI_(void) NativeDrawRoundRect(
        /* [in] */ Int32 nativeCanvas,
        /* [in] */ IRectF* rect,
        /* [in] */ Float rx,
        /* [in] */ Float ry,
        /* [in] */ Int32 nativePaint);

    static CARAPI_(void) NativeDrawPath(
        /* [in] */ Int32 nativeCanvas,
        /* [in] */ Int32 nativePath,
        /* [in] */ Int32 nativePaint);

    static CARAPI_(void) NativeDrawBitmap(
        /* [in] */ Int32 nativeCanvas,
        /* [in] */ Int32 nativeBitmap,
        /* [in] */ Float left,
        /* [in] */ Float top,
        /* [in] */ Int32 nativePaint,
        /* [in] */ Int32 canvasDensity,
        /* [in] */ Int32 screenDensity,
        /* [in] */ Int32 bitmapDensity);

    static CARAPI_(void) NativeDrawBitmap(
        /* [in] */ Int32 nativeCanvas,
        /* [in] */ Int32 nativeBitmap,
        /* [in] */ IRect* src,
        /* [in] */ IRectF* dst,
        /* [in] */ Int32 nativePaint,
        /* [in] */ Int32 screenDensity,
        /* [in] */ Int32 bitmapDensity);

    static CARAPI_(void) NativeDrawBitmap(
        /* [in] */ Int32 nativeCanvas,
        /* [in] */ Int32 nativeBitmap,
        /* [in] */ IRect* src,
        /* [in] */ IRect* dst,
        /* [in] */ Int32 nativePaint,
        /* [in] */ Int32 screenDensity,
        /* [in] */ Int32 bitmapDensity);

    static CARAPI_(void) NativeDrawBitmap(
        /* [in] */ Int32 nativeCanvas,
        /* [in] */ const ArrayOf<Int32>& colors,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 stride,
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Boolean hasAlpha,
        /* [in] */ Int32 nativePaint);

    static CARAPI_(void) NativeDrawBitmapMatrix(
        /* [in] */ Int32 nativeCanvas,
        /* [in] */ Int32 nativeBitmap,
        /* [in] */ Int32 nativeMatrix,
        /* [in] */ Int32 nativePaint);

    static CARAPI_(void) NativeDrawBitmapMesh(
        /* [in] */ Int32 nativeCanvas,
        /* [in] */ Int32 nativeBitmap,
        /* [in] */ Int32 meshWidth,
        /* [in] */ Int32 meshHeight,
        /* [in] */ const ArrayOf<Float>& verts,
        /* [in] */ Int32 vertIndex,
        /* [in] */ ArrayOf<Int32>* colors,
        /* [in] */ Int32 colorIndex,
        /* [in] */ Int32 nativePaint);

    static CARAPI_(void) NativeDrawVertices(
        /* [in] */ Int32 nativeCanvas,
        /* [in] */ CanvasVertexMode mode,
        /* [in] */ Int32 vertexCount,
        /* [in] */ const ArrayOf<Float>& verts,
        /* [in] */ Int32 vertIndex,
        /* [in] */ ArrayOf<Float>* texs,
        /* [in] */ Int32 texIndex,
        /* [in] */ ArrayOf<Int32>* colors,
        /* [in] */ Int32 colorIndex,
        /* [in] */ ArrayOf<Int16>* indices,
        /* [in] */ Int32 indexIndex,
        /* [in] */ Int32 indexCount,
        /* [in] */ Int32 nativePaint);

    static CARAPI_(void) NativeDrawText(
        /* [in] */ Int32 nativeCanvas,
        /* [in] */ const ArrayOf<Char32>& text,
        /* [in] */ Int32 index,
        /* [in] */ Int32 count,
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 nativePaint);

    static CARAPI_(void) NativeDrawText(
        /* [in] */ Int32 nativeCanvas,
        /* [in] */ const String& text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 nativePaint);

    static CARAPI_(void) NativeDrawPosText(
        /* [in] */ Int32 nativeCanvas,
        /* [in] */ const ArrayOf<Char32>& text,
        /* [in] */ Int32 index,
        /* [in] */ Int32 count,
        /* [in] */ const ArrayOf<Float>& pos,
        /* [in] */ Int32 nativePaint);

    static CARAPI_(void) NativeDrawTextRun(
        /* [in] */ Int32 canvas,
        /* [in] */ const ArrayOf<Char32>& text,
        /* [in] */ Int32 index,
        /* [in] */ Int32 count,
        /* [in] */ Int32 contextIndex,
        /* [in] */ Int32 contextCount,
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ Int32 dirFlags,
        /* [in] */ Int32 paint);

    static CARAPI_(void) NativeDrawTextRun(
        /* [in] */ Int32 canvas,
        /* [in] */ const String& text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Int32 contextStart,
        /* [in] */ Int32 contextEnd,
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ Int32 dirFlags,
        /* [in] */ Int32 paint);

    static CARAPI_(void) NativeDrawPosText(
        /* [in] */ Int32 nativeCanvas,
        /* [in] */ const String& text,
        /* [in] */ const ArrayOf<Float>& pos,
        /* [in] */ Int32 nativePaint);

    static CARAPI_(void) NativeDrawTextOnPath(
        /* [in] */ Int32 nativeCanvas,
        /* [in] */ const ArrayOf<Char32>& text,
        /* [in] */ Int32 index,
        /* [in] */ Int32 count,
        /* [in] */ Int32 nativePath,
        /* [in] */ Float hOffset,
        /* [in] */ Float vOffset,
        /* [in] */ Int32 bidiFlags,
        /* [in] */ Int32 nativePaint);

    static CARAPI_(void) NativeDrawTextOnPath(
        /* [in] */ Int32 nativeCanvas,
        /* [in] */ const String& text,
        /* [in] */ Int32 nativePath,
        /* [in] */ Float hOffset,
        /* [in] */ Float vOffset,
        /* [in] */ Int32 bidiFlags,
        /* [in] */ Int32 nativePaint);

    static CARAPI_(void) NativeDrawPicture(
        /* [in] */ Int32 nativeCanvas,
        /* [in] */ Int32 nativePicture);

    static CARAPI_(void) NativeFinalizer(
        /* [in] */ Int32 nativeCanvas);

public:
    Int32 mNativeCanvas;

    Int32 mDensity;

    Int32 mScreenDensity;

private:
    AutoPtr<CBitmap> mBitmap;

    // optional field set by the caller
    AutoPtr<IDrawFilter> mDrawFilter;

    Int32 mSurfaceFormat;

    static const Int32 MAXMIMUM_BITMAP_SIZE = 32766;
};

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_GRAPHICS_H_H__

