#ifndef __CANVASMACRO_H__
#define __CANVASMACRO_H__

#define ICANVAS_METHODS_DECL()                                                                        \
    CARAPI IsHardwareAccelerated(                                                                     \
        /* [out] */ Boolean* isAccelerated);                                                          \
                                                                                                      \
    CARAPI SetBitmap(                                                                                 \
        /* [in] */ IBitmap* bitmap);                                                                  \
                                                                                                      \
    CARAPI SetViewport(                                                                               \
        /* [in] */ Int32 width,                                                                       \
        /* [in] */ Int32 height);                                                                     \
                                                                                                      \
    CARAPI IsOpaque(                                                                                  \
        /* [out] */ Boolean* isOpaque);                                                               \
                                                                                                      \
    CARAPI GetWidth(                                                                                  \
        /* [out] */ Int32* width);                                                                    \
                                                                                                      \
    CARAPI GetHeight(                                                                                 \
        /* [out] */ Int32* height);                                                                   \
                                                                                                      \
    CARAPI GetDensity(                                                                                \
        /* [out] */ Int32* density);                                                                  \
                                                                                                      \
    CARAPI SetDensity(                                                                                \
        /* [in] */ Int32 density);                                                                    \
                                                                                                      \
    CARAPI SetScreenDensity(                                                                          \
        /* [in] */ Int32 density);                                                                    \
                                                                                                      \
    CARAPI GetMaximumBitmapWidth(                                                                     \
        /* [out] */ Int32* width);                                                                    \
                                                                                                      \
    CARAPI GetMaximumBitmapHeight(                                                                    \
        /* [out] */ Int32* height);                                                                   \
                                                                                                      \
    CARAPI Save(                                                                                      \
        /* [out] */ Int32* count);                                                                    \
                                                                                                      \
    CARAPI SaveEx(                                                                                    \
        /* [in] */ Int32 saveFlags,                                                                   \
        /* [out] */ Int32* count);                                                                    \
                                                                                                      \
    CARAPI SaveLayer(                                                                                 \
        /* [in] */ IRectF* bounds,                                                                    \
        /* [in] */ IPaint* paint,                                                                     \
        /* [in] */ Int32 saveFlags,                                                                   \
        /* [out] */ Int32* count);                                                                    \
                                                                                                      \
    CARAPI SaveLayerEx(                                                                               \
        /* [in] */ Float left,                                                                        \
        /* [in] */ Float top,                                                                         \
        /* [in] */ Float right,                                                                       \
        /* [in] */ Float bottom,                                                                      \
        /* [in] */ IPaint* paint,                                                                     \
        /* [in] */ Int32 saveFlags,                                                                   \
        /* [out] */ Int32* count);                                                                    \
                                                                                                      \
    CARAPI SaveLayerAlpha(                                                                            \
        /* [in] */ IRectF* bounds,                                                                    \
        /* [in] */ Int32 alpha,                                                                       \
        /* [in] */ Int32 saveFlags,                                                                   \
        /* [out] */ Int32* count);                                                                    \
                                                                                                      \
    CARAPI SaveLayerAlphaEx(                                                                          \
        /* [in] */ Float left,                                                                        \
        /* [in] */ Float top,                                                                         \
        /* [in] */ Float right,                                                                       \
        /* [in] */ Float bottom,                                                                      \
        /* [in] */ Int32 alpha,                                                                       \
        /* [in] */ Int32 saveFlags,                                                                   \
        /* [out] */ Int32* count);                                                                    \
                                                                                                      \
    CARAPI Restore();                                                                                 \
                                                                                                      \
    CARAPI GetSaveCount(                                                                              \
        /* [out] */ Int32* count);                                                                    \
                                                                                                      \
    CARAPI RestoreToCount(                                                                            \
        /* [in] */ Int32 saveCount);                                                                  \
                                                                                                      \
    CARAPI Translate(                                                                                 \
        /* [in] */ Float dx,                                                                          \
        /* [in] */ Float dy);                                                                         \
                                                                                                      \
    CARAPI Scale(                                                                                     \
        /* [in] */ Float sx,                                                                          \
        /* [in] */ Float sy);                                                                         \
                                                                                                      \
    CARAPI ScaleEx(                                                                                   \
        /* [in] */ Float sx,                                                                          \
        /* [in] */ Float sy,                                                                          \
        /* [in] */ Float px,                                                                          \
        /* [in] */ Float py);                                                                         \
                                                                                                      \
    CARAPI Rotate(                                                                                    \
        /* [in] */ Float degrees);                                                                    \
                                                                                                      \
    CARAPI RotateEx(                                                                                  \
        /* [in] */ Float degrees,                                                                     \
        /* [in] */ Float px,                                                                          \
        /* [in] */ Float py);                                                                         \
                                                                                                      \
    CARAPI Skew(                                                                                      \
        /* [in] */ Float sx,                                                                          \
        /* [in] */ Float sy);                                                                         \
                                                                                                      \
    CARAPI Concat(                                                                                    \
        /* [in] */ IMatrix* matrix);                                                                  \
                                                                                                      \
    CARAPI SetMatrix(                                                                                 \
        /* [in] */ IMatrix* matrix);                                                                  \
                                                                                                      \
    CARAPI GetMatrix(                                                                                 \
        /* [in] */ IMatrix* ctm);                                                                     \
                                                                                                      \
    CARAPI GetMatrixEx(                                                                               \
        /* [out] */ IMatrix** metrix);                                                                \
                                                                                                      \
    CARAPI ClipRect(                                                                                  \
        /* [in] */ IRectF* rect,                                                                      \
        /* [in] */ RegionOp op,                                                                       \
        /* [out] */ Boolean* isNotEmpty);                                                             \
                                                                                                      \
    CARAPI ClipRectEx(                                                                                \
        /* [in] */ IRect* rect,                                                                       \
        /* [in] */ RegionOp op,                                                                       \
        /* [out] */ Boolean* isNotEmpty);                                                             \
                                                                                                      \
    CARAPI ClipRectEx2(                                                                               \
        /* [in] */ IRectF* rect,                                                                      \
        /* [out] */ Boolean* isNotEmpty);                                                             \
                                                                                                      \
    CARAPI ClipRectEx3(                                                                               \
        /* [in] */ IRect* rect,                                                                       \
        /* [out] */ Boolean* isNotEmpty);                                                             \
                                                                                                      \
    CARAPI ClipRectEx4(                                                                               \
        /* [in] */ Float left,                                                                        \
        /* [in] */ Float top,                                                                         \
        /* [in] */ Float right,                                                                       \
        /* [in] */ Float bottom,                                                                      \
        /* [in] */ RegionOp op,                                                                       \
        /* [out] */ Boolean* isNotEmpty);                                                             \
                                                                                                      \
    CARAPI ClipRectEx5(                                                                               \
        /* [in] */ Float left,                                                                        \
        /* [in] */ Float top,                                                                         \
        /* [in] */ Float right,                                                                       \
        /* [in] */ Float bottom,                                                                      \
        /* [out] */ Boolean* isNotEmpty);                                                             \
                                                                                                      \
    CARAPI ClipRectEx6(                                                                               \
        /* [in] */ Int32 left,                                                                        \
        /* [in] */ Int32 top,                                                                         \
        /* [in] */ Int32 right,                                                                       \
        /* [in] */ Int32 bottom,                                                                      \
        /* [out] */ Boolean* isNotEmpty);                                                             \
                                                                                                      \
    CARAPI ClipPath(                                                                                  \
        /* [in] */ IPath* path,                                                                       \
        /* [in] */ RegionOp op,                                                                       \
        /* [out] */ Boolean* isNotEmpty);                                                             \
                                                                                                      \
    CARAPI ClipPathEx(                                                                                \
        /* [in] */ IPath* path,                                                                       \
        /* [out] */ Boolean* isNotEmpty);                                                             \
                                                                                                      \
    CARAPI ClipRegion(                                                                                \
        /* [in] */ IRegion* region,                                                                   \
        /* [in] */ RegionOp op,                                                                       \
        /* [out] */ Boolean* isNotEmpty);                                                             \
                                                                                                      \
    CARAPI ClipRegionEx(                                                                              \
        /* [in] */ IRegion* region,                                                                   \
        /* [out] */ Boolean* isNotEmpty);                                                             \
                                                                                                      \
    CARAPI GetDrawFilter(                                                                             \
        /* [out] */ IDrawFilter** filter);                                                            \
                                                                                                      \
    CARAPI SetDrawFilter(                                                                             \
        /* [in] */ IDrawFilter* filter);                                                              \
                                                                                                      \
    CARAPI QuickReject(                                                                               \
        /* [in] */ IRectF* rect,                                                                      \
        /* [in] */ CanvasEdgeType type,                                                               \
        /* [out] */ Boolean* isNotIntersect);                                                         \
                                                                                                      \
    CARAPI QuickRejectEx(                                                                             \
        /* [in] */ IPath* path,                                                                       \
        /* [in] */ CanvasEdgeType type,                                                               \
        /* [out] */ Boolean* isNotIntersect);                                                         \
                                                                                                      \
    CARAPI QuickRejectEx2(                                                                            \
        /* [in] */ Float left,                                                                        \
        /* [in] */ Float top,                                                                         \
        /* [in] */ Float right,                                                                       \
        /* [in] */ Float bottom,                                                                      \
        /* [in] */ CanvasEdgeType type,                                                               \
        /* [out] */ Boolean* isNotIntersect);                                                         \
                                                                                                      \
    CARAPI GetClipBounds(                                                                             \
        /* [in] */ IRect* bounds,                                                                     \
        /* [out] */ Boolean* isNotEmpty);                                                             \
                                                                                                      \
    CARAPI GetClipBoundsEx(                                                                           \
        /* [out] */ IRect** bounds);                                                                  \
                                                                                                      \
    CARAPI DrawRGB(                                                                                   \
        /* [in] */ Int32 r,                                                                           \
        /* [in] */ Int32 g,                                                                           \
        /* [in] */ Int32 b);                                                                          \
                                                                                                      \
    CARAPI DrawARGB(                                                                                  \
        /* [in] */ Int32 a,                                                                           \
        /* [in] */ Int32 r,                                                                           \
        /* [in] */ Int32 g,                                                                           \
        /* [in] */ Int32 b);                                                                          \
                                                                                                      \
    CARAPI DrawColor(                                                                                 \
        /* [in] */ Int32 color);                                                                      \
                                                                                                      \
    CARAPI DrawColorEx(                                                                               \
        /* [in] */ Int32 color,                                                                       \
        /* [in] */ PorterDuffMode mode);                                                              \
                                                                                                      \
    CARAPI DrawPaint(                                                                                 \
        /* [in] */ IPaint* paint);                                                                    \
                                                                                                      \
    CARAPI DrawPoints(                                                                                \
        /* [in] */ const ArrayOf<Float>& pts,                                                         \
        /* [in] */ Int32 offset,                                                                      \
        /* [in] */ Int32 count,                                                                       \
        /* [in] */ IPaint* paint);                                                                    \
                                                                                                      \
    CARAPI DrawPointsEx(                                                                              \
        /* [in] */ const ArrayOf<Float>& pts,                                                         \
        /* [in] */ IPaint* paint);                                                                    \
                                                                                                      \
    CARAPI DrawPoint(                                                                                 \
        /* [in] */ Float x,                                                                           \
        /* [in] */ Float y,                                                                           \
        /* [in] */ IPaint* paint);                                                                    \
                                                                                                      \
    CARAPI DrawLine(                                                                                  \
        /* [in] */ Float startX,                                                                      \
        /* [in] */ Float startY,                                                                      \
        /* [in] */ Float stopX,                                                                       \
        /* [in] */ Float stopY,                                                                       \
        /* [in] */ IPaint* paint);                                                                    \
                                                                                                      \
    CARAPI DrawLines(                                                                                 \
        /* [in] */ const ArrayOf<Float>& pts,                                                         \
        /* [in] */ Int32 offset,                                                                      \
        /* [in] */ Int32 count,                                                                       \
        /* [in] */ IPaint* paint);                                                                    \
                                                                                                      \
    CARAPI DrawLinesEx(                                                                               \
        /* [in] */ const ArrayOf<Float>& pts,                                                         \
        /* [in] */ IPaint* paint);                                                                    \
                                                                                                      \
    CARAPI DrawRect(                                                                                  \
        /* [in] */ IRectF* rect,                                                                      \
        /* [in] */ IPaint* paint);                                                                    \
                                                                                                      \
    CARAPI DrawRectEx(                                                                                \
        /* [in] */ IRect* r,                                                                          \
        /* [in] */ IPaint* paint);                                                                    \
                                                                                                      \
    CARAPI DrawRectEx2(                                                                               \
        /* [in] */ Float left,                                                                        \
        /* [in] */ Float top,                                                                         \
        /* [in] */ Float right,                                                                       \
        /* [in] */ Float bottom,                                                                      \
        /* [in] */ IPaint* paint);                                                                    \
                                                                                                      \
    CARAPI DrawOval(                                                                                  \
        /* [in] */ IRectF* oval,                                                                      \
        /* [in] */ IPaint* paint);                                                                    \
                                                                                                      \
    CARAPI DrawCircle(                                                                                \
        /* [in] */ Float cx,                                                                          \
        /* [in] */ Float cy,                                                                          \
        /* [in] */ Float radius,                                                                      \
        /* [in] */ IPaint* paint);                                                                    \
                                                                                                      \
    CARAPI DrawArc(                                                                                   \
        /* [in] */ IRectF* oval,                                                                      \
        /* [in] */ Float startAngle,                                                                  \
        /* [in] */ Float sweepAngle,                                                                  \
        /* [in] */ Boolean useCenter,                                                                 \
        /* [in] */ IPaint* paint);                                                                    \
                                                                                                      \
    CARAPI DrawRoundRect(                                                                             \
        /* [in] */ IRectF* rect,                                                                      \
        /* [in] */ Float rx,                                                                          \
        /* [in] */ Float ry,                                                                          \
        /* [in] */ IPaint* paint);                                                                    \
                                                                                                      \
    CARAPI DrawPath(                                                                                  \
        /* [in] */ IPath* path,                                                                       \
        /* [in] */ IPaint* paint);                                                                    \
                                                                                                      \
    CARAPI DrawPatch(                                                                                 \
        /* [in] */ IBitmap* bitmap,                                                                   \
        /* [in] */ const ArrayOf<Byte>& chunks,                                                       \
        /* [in] */ IRectF* dst,                                                                       \
        /* [in] */ IPaint* paint);                                                                    \
                                                                                                      \
    CARAPI DrawBitmap(                                                                                \
        /* [in] */ IBitmap* bitmap,                                                                   \
        /* [in] */ Float left,                                                                        \
        /* [in] */ Float top,                                                                         \
        /* [in] */ IPaint* paint);                                                                    \
                                                                                                      \
    CARAPI DrawBitmapEx(                                                                              \
        /* [in] */ IBitmap* bitmap,                                                                   \
        /* [in] */ IRect* src,                                                                        \
        /* [in] */ IRectF* dst,                                                                       \
        /* [in] */ IPaint* paint);                                                                    \
                                                                                                      \
    CARAPI DrawBitmapEx2(                                                                             \
        /* [in] */ IBitmap* bitmap,                                                                   \
        /* [in] */ IRect* src,                                                                        \
        /* [in] */ IRect* dst,                                                                        \
        /* [in] */ IPaint* paint);                                                                    \
                                                                                                      \
    CARAPI DrawBitmapEx3(                                                                             \
        /* [in] */ const ArrayOf<Int32>& colors,                                                      \
        /* [in] */ Int32 offset,                                                                      \
        /* [in] */ Int32 stride,                                                                      \
        /* [in] */ Float x,                                                                           \
        /* [in] */ Float y,                                                                           \
        /* [in] */ Int32 width,                                                                       \
        /* [in] */ Int32 height,                                                                      \
        /* [in] */ Boolean hasAlpha,                                                                  \
        /* [in] */ IPaint* paint);                                                                    \
                                                                                                      \
    CARAPI DrawBitmapEx4(                                                                             \
        /* [in] */ const ArrayOf<Int32>& colors,                                                      \
        /* [in] */ Int32 offset,                                                                      \
        /* [in] */ Int32 stride,                                                                      \
        /* [in] */ Int32 x,                                                                           \
        /* [in] */ Int32 y,                                                                           \
        /* [in] */ Int32 width,                                                                       \
        /* [in] */ Int32 height,                                                                      \
        /* [in] */ Boolean hasAlpha,                                                                  \
        /* [in] */ IPaint* paint);                                                                    \
                                                                                                      \
    CARAPI DrawBitmapEx5(                                                                             \
        /* [in] */ IBitmap* bitmap,                                                                   \
        /* [in] */ IMatrix* matrix,                                                                   \
        /* [in] */ IPaint* paint);                                                                    \
                                                                                                      \
    CARAPI DrawBitmapMesh(                                                                            \
        /* [in] */ IBitmap* bitmap,                                                                   \
        /* [in] */ Int32 meshWidth,                                                                   \
        /* [in] */ Int32 meshHeight,                                                                  \
        /* [in] */ const ArrayOf<Float>& verts,                                                       \
        /* [in] */ Int32 vertOffset,                                                                  \
        /* [in] */ ArrayOf<Int32>* colors,                                                            \
        /* [in] */ Int32 colorOffset,                                                                 \
        /* [in] */ IPaint* paint);                                                                    \
                                                                                                      \
    CARAPI DrawVertices(                                                                              \
        /* [in] */ CanvasVertexMode mode,                                                             \
        /* [in] */ Int32 vertexCount,                                                                 \
        /* [in] */ const ArrayOf<Float>& verts,                                                       \
        /* [in] */ Int32 vertOffset,                                                                  \
        /* [in] */ ArrayOf<Float>* texs,                                                              \
        /* [in] */ Int32 texOffset,                                                                   \
        /* [in] */ ArrayOf<Int32>* colors,                                                            \
        /* [in] */ Int32 colorOffset,                                                                 \
        /* [in] */ ArrayOf<Int16>* indices,                                                           \
        /* [in] */ Int32 indexOffset,                                                                 \
        /* [in] */ Int32 indexCount,                                                                  \
        /* [in] */ IPaint* paint);                                                                    \
                                                                                                      \
    CARAPI DrawText(                                                                                  \
        /* [in] */ const ArrayOf<Char32>& text,                                                       \
        /* [in] */ Int32 index,                                                                       \
        /* [in] */ Int32 count,                                                                       \
        /* [in] */ Float x,                                                                           \
        /* [in] */ Float y,                                                                           \
        /* [in] */ IPaint* paint);                                                                    \
                                                                                                      \
    CARAPI DrawTextEx(                                                                                \
        /* [in] */ const String& text,                                                                \
        /* [in] */ Float x,                                                                           \
        /* [in] */ Float y,                                                                           \
        /* [in] */ IPaint* paint);                                                                    \
                                                                                                      \
    CARAPI DrawTextEx2(                                                                               \
        /* [in] */ const String& text,                                                                \
        /* [in] */ Int32 start,                                                                       \
        /* [in] */ Int32 end,                                                                         \
        /* [in] */ Float x,                                                                           \
        /* [in] */ Float y,                                                                           \
        /* [in] */ IPaint* paint);                                                                    \
                                                                                                      \
    CARAPI DrawTextEx3(                                                                               \
        /* [in] */ ICharSequence* text,                                                               \
        /* [in] */ Int32 start,                                                                       \
        /* [in] */ Int32 end,                                                                         \
        /* [in] */ Float x,                                                                           \
        /* [in] */ Float y,                                                                           \
        /* [in] */ IPaint* paint);                                                                    \
                                                                                                      \
    CARAPI DrawTextRun(                                                                               \
        /* [in] */ const ArrayOf<Char32>& text,                                                       \
        /* [in] */ Int32 index,                                                                       \
        /* [in] */ Int32 count,                                                                       \
        /* [in] */ Int32 contextIndex,                                                                \
        /* [in] */ Int32 contextCount,                                                                \
        /* [in] */ Float x,                                                                           \
        /* [in] */ Float y,                                                                           \
        /* [in] */ Int32 dir,                                                                         \
        /* [in] */ IPaint* paint);                                                                    \
                                                                                                      \
    CARAPI DrawTextRunEx(                                                                             \
        /* [in] */ ICharSequence* text,                                                               \
        /* [in] */ Int32 start,                                                                       \
        /* [in] */ Int32 end,                                                                         \
        /* [in] */ Int32 contextStart,                                                                \
        /* [in] */ Int32 contextEnd,                                                                  \
        /* [in] */ Float x,                                                                           \
        /* [in] */ Float y,                                                                           \
        /* [in] */ Int32 dir,                                                                         \
        /* [in] */ IPaint* paint);                                                                    \
                                                                                                      \
    CARAPI DrawPosText(                                                                               \
        /* [in] */ const ArrayOf<Char32>& text,                                                       \
        /* [in] */ Int32 index,                                                                       \
        /* [in] */ Int32 count,                                                                       \
        /* [in] */ const ArrayOf<Float>& pos,                                                         \
        /* [in] */ IPaint* paint);                                                                    \
                                                                                                      \
    CARAPI DrawPosTextEx(                                                                             \
        /* [in] */ const String& text,                                                                \
        /* [in] */ const ArrayOf<Float>& pos,                                                         \
        /* [in] */ IPaint* paint);                                                                    \
                                                                                                      \
    CARAPI DrawTextOnPath(                                                                            \
        /* [in] */ const ArrayOf<Char32>& text,                                                       \
        /* [in] */ Int32 index,                                                                       \
        /* [in] */ Int32 count,                                                                       \
        /* [in] */ IPath* path,                                                                       \
        /* [in] */ Float hOffset,                                                                     \
        /* [in] */ Float vOffset,                                                                     \
        /* [in] */ IPaint* paint);                                                                    \
                                                                                                      \
    CARAPI DrawTextOnPathEx(                                                                          \
        /* [in] */ const String& text,                                                                \
        /* [in] */ IPath* path,                                                                       \
        /* [in] */ Float hOffset,                                                                     \
        /* [in] */ Float vOffset,                                                                     \
        /* [in] */ IPaint* paint);                                                                    \
                                                                                                      \
    CARAPI DrawPicture(                                                                               \
        /* [in] */ IPicture* picture);                                                                \
                                                                                                      \
    CARAPI DrawPictureEx(                                                                             \
        /* [in] */ IPicture* picture,                                                                 \
        /* [in] */ IRectF* dst);                                                                      \
                                                                                                      \
    CARAPI DrawPictureEx2(                                                                            \
        /* [in] */ IPicture* picture,                                                                 \
        /* [in] */ IRect* dst);                                                                       \
                                                                                                      \
    CARAPI SetSurfaceFormat(                                                                          \
        /* [in] */ Int32 format);                                                                     \
                                                                                                      \
    CARAPI GetNativeCanvas(                                                                           \
        /* [out] */ Handle32* natvieCanvas);

#define ICANVAS_METHODS_IMPL(className, superClass)                                                   \
ECode className::IsHardwareAccelerated(                                                               \
    /* [out] */ Boolean* isAccelerated)                                                               \
{                                                                                                     \
    VALIDATE_NOT_NULL(isAccelerated);                                                                 \
    return superClass::IsHardwareAccelerated(isAccelerated);                                          \
}                                                                                                     \
                                                                                                      \
ECode className::SetBitmap(                                                                           \
    /* [in] */ IBitmap* bitmap)                                                                       \
{                                                                                                     \
    return superClass::SetBitmap(bitmap);                                                             \
}                                                                                                     \
                                                                                                      \
ECode className::SetViewport(                                                                         \
    /* [in] */ Int32 width,                                                                           \
    /* [in] */ Int32 height)                                                                          \
{                                                                                                     \
    return superClass::SetViewport(width, height);                                                    \
}                                                                                                     \
                                                                                                      \
ECode className::IsOpaque(                                                                            \
    /* [out] */ Boolean* isOpaque)                                                                    \
{                                                                                                     \
    VALIDATE_NOT_NULL(isOpaque);                                                                      \
    return superClass::IsOpaque(isOpaque);                                                            \
}                                                                                                     \
                                                                                                      \
ECode className::GetWidth(                                                                            \
    /* [out] */ Int32* width)                                                                         \
{                                                                                                     \
    VALIDATE_NOT_NULL(width);                                                                         \
    return superClass::GetWidth(width);                                                               \
}                                                                                                     \
                                                                                                      \
ECode className::GetHeight(                                                                           \
    /* [out] */ Int32* height)                                                                        \
{                                                                                                     \
    VALIDATE_NOT_NULL(height);                                                                        \
    return superClass::GetHeight(height);                                                             \
}                                                                                                     \
                                                                                                      \
ECode className::GetDensity(                                                                          \
    /* [out] */ Int32* density)                                                                       \
{                                                                                                     \
    VALIDATE_NOT_NULL(density);                                                                       \
    return superClass::GetDensity(density);                                                           \
}                                                                                                     \
                                                                                                      \
ECode className::SetDensity(                                                                          \
    /* [in] */ Int32 density)                                                                         \
{                                                                                                     \
    return superClass::SetDensity(density);                                                           \
}                                                                                                     \
                                                                                                      \
ECode className::SetScreenDensity(                                                                    \
    /* [in] */ Int32 density)                                                                         \
{                                                                                                     \
    return superClass::SetScreenDensity(density);                                                     \
}                                                                                                     \
                                                                                                      \
ECode className::GetMaximumBitmapWidth(                                                               \
    /* [out] */ Int32* width)                                                                         \
{                                                                                                     \
    VALIDATE_NOT_NULL(width);                                                                         \
    return superClass::GetMaximumBitmapWidth(width);                                                  \
}                                                                                                     \
                                                                                                      \
ECode className::GetMaximumBitmapHeight(                                                              \
    /* [out] */ Int32* height)                                                                        \
{                                                                                                     \
    VALIDATE_NOT_NULL(height);                                                                        \
    return superClass::GetMaximumBitmapHeight(height);                                                \
}                                                                                                     \
                                                                                                      \
ECode className::Save(                                                                                \
    /* [out] */ Int32* count)                                                                         \
{                                                                                                     \
    VALIDATE_NOT_NULL(count);                                                                         \
    return superClass::Save(count);                                                                   \
}                                                                                                     \
                                                                                                      \
ECode className::SaveEx(                                                                              \
    /* [in] */ Int32 saveFlags,                                                                       \
    /* [out] */ Int32* count)                                                                         \
{                                                                                                     \
    VALIDATE_NOT_NULL(count);                                                                         \
    return superClass::SaveEx(saveFlags, count);                                                      \
}                                                                                                     \
                                                                                                      \
ECode className::SaveLayer(                                                                           \
    /* [in] */ IRectF* bounds,                                                                        \
    /* [in] */ IPaint* paint,                                                                         \
    /* [in] */ Int32 saveFlags,                                                                       \
    /* [out] */ Int32* count)                                                                         \
{                                                                                                     \
    VALIDATE_NOT_NULL(count);                                                                         \
    return superClass::SaveLayer(bounds, paint, saveFlags, count);                                    \
}                                                                                                     \
                                                                                                      \
ECode className::SaveLayerEx(                                                                         \
    /* [in] */ Float left,                                                                            \
    /* [in] */ Float top,                                                                             \
    /* [in] */ Float right,                                                                           \
    /* [in] */ Float bottom,                                                                          \
    /* [in] */ IPaint* paint,                                                                         \
    /* [in] */ Int32 saveFlags,                                                                       \
    /* [out] */ Int32* count)                                                                         \
{                                                                                                     \
    VALIDATE_NOT_NULL(count);                                                                         \
    return superClass::SaveLayerEx(left, top, right, bottom, paint, saveFlags, count);                \
}                                                                                                     \
                                                                                                      \
ECode className::SaveLayerAlpha(                                                                      \
    /* [in] */ IRectF* bounds,                                                                        \
    /* [in] */ Int32 alpha,                                                                           \
    /* [in] */ Int32 saveFlags,                                                                       \
    /* [out] */ Int32* count)                                                                         \
{                                                                                                     \
    VALIDATE_NOT_NULL(count);                                                                         \
    return superClass::SaveLayerAlpha(bounds, alpha, saveFlags, count);                               \
}                                                                                                     \
                                                                                                      \
ECode className::SaveLayerAlphaEx(                                                                    \
    /* [in] */ Float left,                                                                            \
    /* [in] */ Float top,                                                                             \
    /* [in] */ Float right,                                                                           \
    /* [in] */ Float bottom,                                                                          \
    /* [in] */ Int32 alpha,                                                                           \
    /* [in] */ Int32 saveFlags,                                                                       \
    /* [out] */ Int32* count)                                                                         \
{                                                                                                     \
    VALIDATE_NOT_NULL(count);                                                                         \
    return superClass::SaveLayerAlphaEx(left, top, right, bottom, alpha, saveFlags, count);           \
}                                                                                                     \
                                                                                                      \
ECode className::Restore()                                                                            \
{                                                                                                     \
    return superClass::Restore();                                                                     \
}                                                                                                     \
                                                                                                      \
ECode className::GetSaveCount(                                                                        \
    /* [out] */ Int32* count)                                                                         \
{                                                                                                     \
    VALIDATE_NOT_NULL(count);                                                                         \
    return superClass::GetSaveCount(count);                                                           \
}                                                                                                     \
                                                                                                      \
ECode className::RestoreToCount(                                                                      \
    /* [in] */ Int32 saveCount)                                                                       \
{                                                                                                     \
    return superClass::RestoreToCount(saveCount);                                                     \
}                                                                                                     \
                                                                                                      \
ECode className::Translate(                                                                           \
    /* [in] */ Float dx,                                                                              \
    /* [in] */ Float dy)                                                                              \
{                                                                                                     \
    return superClass::Translate(dx, dy);                                                             \
}                                                                                                     \
                                                                                                      \
ECode className::Scale(                                                                               \
    /* [in] */ Float sx,                                                                              \
    /* [in] */ Float sy)                                                                              \
{                                                                                                     \
    return superClass::Scale(sx, sy);                                                                 \
}                                                                                                     \
                                                                                                      \
ECode className::ScaleEx(                                                                             \
    /* [in] */ Float sx,                                                                              \
    /* [in] */ Float sy,                                                                              \
    /* [in] */ Float px,                                                                              \
    /* [in] */ Float py)                                                                              \
{                                                                                                     \
    return superClass::ScaleEx(sx, sy, px, py);                                                       \
}                                                                                                     \
                                                                                                      \
ECode className::Rotate(                                                                              \
    /* [in] */ Float degrees)                                                                         \
{                                                                                                     \
    return superClass::Rotate(degrees);                                                               \
}                                                                                                     \
                                                                                                      \
ECode className::RotateEx(                                                                            \
    /* [in] */ Float degrees,                                                                         \
    /* [in] */ Float px,                                                                              \
    /* [in] */ Float py)                                                                              \
{                                                                                                     \
    return superClass::RotateEx(degrees, px, py);                                                     \
}                                                                                                     \
                                                                                                      \
ECode className::Skew(                                                                                \
    /* [in] */ Float sx,                                                                              \
    /* [in] */ Float sy)                                                                              \
{                                                                                                     \
    return superClass::Skew(sx, sy);                                                                  \
}                                                                                                     \
                                                                                                      \
ECode className::Concat(                                                                              \
    /* [in] */ IMatrix* matrix)                                                                       \
{                                                                                                     \
    return superClass::Concat(matrix);                                                                \
}                                                                                                     \
                                                                                                      \
ECode className::SetMatrix(                                                                           \
    /* [in] */ IMatrix* matrix)                                                                       \
{                                                                                                     \
    return superClass::SetMatrix(matrix);                                                             \
}                                                                                                     \
                                                                                                      \
ECode className::GetMatrix(                                                                           \
    /* [in] */ IMatrix* ctm)                                                                          \
{                                                                                                     \
    return superClass::GetMatrix(ctm);                                                                \
}                                                                                                     \
                                                                                                      \
ECode className::GetMatrixEx(                                                                         \
    /* [out] */ IMatrix** matrix)                                                                     \
{                                                                                                     \
    VALIDATE_NOT_NULL(matrix);                                                                        \
    return superClass::GetMatrixEx(matrix);                                                           \
}                                                                                                     \
                                                                                                      \
ECode className::ClipRect(                                                                            \
    /* [in] */ IRectF* rect,                                                                          \
    /* [in] */ RegionOp op,                                                                           \
    /* [out] */ Boolean* isNotEmpty)                                                                  \
{                                                                                                     \
    VALIDATE_NOT_NULL(isNotEmpty);                                                                    \
    return superClass::ClipRect(rect, op, isNotEmpty);                                                \
}                                                                                                     \
                                                                                                      \
ECode className::ClipRectEx(                                                                          \
    /* [in] */ IRect* rect,                                                                           \
    /* [in] */ RegionOp op,                                                                           \
    /* [out] */ Boolean* isNotEmpty)                                                                  \
{                                                                                                     \
    VALIDATE_NOT_NULL(isNotEmpty);                                                                    \
    return superClass::ClipRectEx(rect, op, isNotEmpty);                                              \
}                                                                                                     \
                                                                                                      \
ECode className::ClipRectEx2(                                                                         \
    /* [in] */ IRectF* rect,                                                                          \
    /* [out] */ Boolean* isNotEmpty)                                                                  \
{                                                                                                     \
    VALIDATE_NOT_NULL(isNotEmpty);                                                                    \
    return superClass::ClipRectEx2(rect, isNotEmpty);                                                 \
}                                                                                                     \
                                                                                                      \
ECode className::ClipRectEx3(                                                                         \
    /* [in] */ IRect* rect,                                                                           \
    /* [out] */ Boolean* isNotEmpty)                                                                  \
{                                                                                                     \
    VALIDATE_NOT_NULL(isNotEmpty);                                                                    \
    return superClass::ClipRectEx3(rect, isNotEmpty);                                                 \
}                                                                                                     \
                                                                                                      \
ECode className::ClipRectEx4(                                                                         \
    /* [in] */ Float left,                                                                            \
    /* [in] */ Float top,                                                                             \
    /* [in] */ Float right,                                                                           \
    /* [in] */ Float bottom,                                                                          \
    /* [in] */ RegionOp op,                                                                           \
    /* [out] */ Boolean* isNotEmpty)                                                                  \
{                                                                                                     \
    VALIDATE_NOT_NULL(isNotEmpty);                                                                    \
    return superClass::ClipRectEx4(left, top, right, bottom, op, isNotEmpty);                         \
}                                                                                                     \
                                                                                                      \
ECode className::ClipRectEx5(                                                                         \
    /* [in] */ Float left,                                                                            \
    /* [in] */ Float top,                                                                             \
    /* [in] */ Float right,                                                                           \
    /* [in] */ Float bottom,                                                                          \
    /* [out] */ Boolean* isNotEmpty)                                                                  \
{                                                                                                     \
    VALIDATE_NOT_NULL(isNotEmpty);                                                                    \
    return superClass::ClipRectEx5(left, top, right, bottom, isNotEmpty);                             \
}                                                                                                     \
                                                                                                      \
ECode className::ClipRectEx6(                                                                         \
    /* [in] */ Int32 left,                                                                            \
    /* [in] */ Int32 top,                                                                             \
    /* [in] */ Int32 right,                                                                           \
    /* [in] */ Int32 bottom,                                                                          \
    /* [out] */ Boolean* isNotEmpty)                                                                  \
{                                                                                                     \
    VALIDATE_NOT_NULL(isNotEmpty);                                                                    \
    return superClass::ClipRectEx6(left, top, right, bottom, isNotEmpty);                             \
}                                                                                                     \
                                                                                                      \
ECode className::ClipPath(                                                                            \
    /* [in] */ IPath* path,                                                                           \
    /* [in] */ RegionOp op,                                                                           \
    /* [out] */ Boolean* isNotEmpty)                                                                  \
{                                                                                                     \
    VALIDATE_NOT_NULL(isNotEmpty);                                                                    \
    return superClass::ClipPath(path, op, isNotEmpty);                                                \
}                                                                                                     \
                                                                                                      \
ECode className::ClipPathEx(                                                                          \
    /* [in] */ IPath* path,                                                                           \
    /* [out] */ Boolean* isNotEmpty)                                                                  \
{                                                                                                     \
    VALIDATE_NOT_NULL(isNotEmpty);                                                                    \
    return superClass::ClipPathEx(path, isNotEmpty);                                                  \
}                                                                                                     \
                                                                                                      \
ECode className::ClipRegion(                                                                          \
    /* [in] */ IRegion* region,                                                                       \
    /* [in] */ RegionOp op,                                                                           \
    /* [out] */ Boolean* isNotEmpty)                                                                  \
{                                                                                                     \
    VALIDATE_NOT_NULL(isNotEmpty);                                                                    \
    return superClass::ClipRegion(region, op, isNotEmpty);                                            \
}                                                                                                     \
                                                                                                      \
ECode className::ClipRegionEx(                                                                        \
    /* [in] */ IRegion* region,                                                                       \
    /* [out] */ Boolean* isNotEmpty)                                                                  \
{                                                                                                     \
    VALIDATE_NOT_NULL(isNotEmpty);                                                                    \
    return superClass::ClipRegionEx(region, isNotEmpty);                                              \
}                                                                                                     \
                                                                                                      \
ECode className::GetDrawFilter(                                                                       \
    /* [out] */ IDrawFilter** filter)                                                                 \
{                                                                                                     \
    VALIDATE_NOT_NULL(filter);                                                                        \
    return superClass::GetDrawFilter(filter);                                                         \
}                                                                                                     \
                                                                                                      \
ECode className::SetDrawFilter(                                                                       \
    /* [in] */ IDrawFilter* filter)                                                                   \
{                                                                                                     \
    return superClass::SetDrawFilter(filter);                                                         \
}                                                                                                     \
                                                                                                      \
ECode className::QuickReject(                                                                         \
    /* [in] */ IRectF* rect,                                                                          \
    /* [in] */ CanvasEdgeType type,                                                                   \
    /* [out] */ Boolean* isNotIntersect)                                                              \
{                                                                                                     \
    VALIDATE_NOT_NULL(isNotIntersect);                                                                \
    return superClass::QuickReject(rect, type, isNotIntersect);                                       \
}                                                                                                     \
                                                                                                      \
ECode className::QuickRejectEx(                                                                       \
    /* [in] */ IPath* path,                                                                           \
    /* [in] */ CanvasEdgeType type,                                                                   \
    /* [out] */ Boolean* isNotIntersect)                                                              \
{                                                                                                     \
    VALIDATE_NOT_NULL(isNotIntersect);                                                                \
    return superClass::QuickRejectEx(path, type, isNotIntersect);                                     \
}                                                                                                     \
                                                                                                      \
ECode className::QuickRejectEx2(                                                                      \
    /* [in] */ Float left,                                                                            \
    /* [in] */ Float top,                                                                             \
    /* [in] */ Float right,                                                                           \
    /* [in] */ Float bottom,                                                                          \
    /* [in] */ CanvasEdgeType type,                                                                   \
    /* [out] */ Boolean* isNotIntersect)                                                              \
{                                                                                                     \
    VALIDATE_NOT_NULL(isNotIntersect);                                                                \
    return superClass::QuickRejectEx2(left, top, right, bottom, type, isNotIntersect);                \
}                                                                                                     \
                                                                                                      \
ECode className::GetClipBounds(                                                                       \
    /* [in] */ IRect* bounds,                                                                         \
    /* [out] */ Boolean* isNotEmpty)                                                                  \
{                                                                                                     \
    VALIDATE_NOT_NULL(isNotEmpty);                                                                    \
    return superClass::GetClipBounds(bounds, isNotEmpty);                                             \
}                                                                                                     \
                                                                                                      \
ECode className::GetClipBoundsEx(                                                                     \
    /* [out] */ IRect** bounds)                                                                       \
{                                                                                                     \
    VALIDATE_NOT_NULL(bounds);                                                                        \
    return superClass::GetClipBoundsEx(bounds);                                                       \
}                                                                                                     \
                                                                                                      \
ECode className::DrawRGB(                                                                             \
    /* [in] */ Int32 r,                                                                               \
    /* [in] */ Int32 g,                                                                               \
    /* [in] */ Int32 b)                                                                               \
{                                                                                                     \
    return superClass::DrawRGB(r, g, b);                                                              \
}                                                                                                     \
                                                                                                      \
ECode className::DrawARGB(                                                                            \
    /* [in] */ Int32 a,                                                                               \
    /* [in] */ Int32 r,                                                                               \
    /* [in] */ Int32 g,                                                                               \
    /* [in] */ Int32 b)                                                                               \
{                                                                                                     \
    return superClass::DrawARGB(a, r, g, b);                                                          \
}                                                                                                     \
                                                                                                      \
ECode className::DrawColor(                                                                           \
    /* [in] */ Int32 color)                                                                           \
{                                                                                                     \
    return superClass::DrawColor(color);                                                              \
}                                                                                                     \
                                                                                                      \
ECode className::DrawColorEx(                                                                         \
    /* [in] */ Int32 color,                                                                           \
    /* [in] */ PorterDuffMode mode)                                                                   \
{                                                                                                     \
    return superClass::DrawColorEx(color, mode);                                                      \
}                                                                                                     \
                                                                                                      \
ECode className::DrawPaint(                                                                           \
    /* [in] */ IPaint* paint)                                                                         \
{                                                                                                     \
    return superClass::DrawPaint(paint);                                                              \
}                                                                                                     \
                                                                                                      \
ECode className::DrawPoints(                                                                          \
    /* [in] */ const ArrayOf<Float>& pts,                                                             \
    /* [in] */ Int32 offset,                                                                          \
    /* [in] */ Int32 count,                                                                           \
    /* [in] */ IPaint* paint)                                                                         \
{                                                                                                     \
    return superClass::DrawPoints(pts, offset, count, paint);                                         \
}                                                                                                     \
                                                                                                      \
ECode className::DrawPointsEx(                                                                        \
    /* [in] */ const ArrayOf<Float>& pts,                                                             \
    /* [in] */ IPaint* paint)                                                                         \
{                                                                                                     \
    return superClass::DrawPointsEx(pts, paint);                                                      \
}                                                                                                     \
                                                                                                      \
ECode className::DrawPoint(                                                                           \
    /* [in] */ Float x,                                                                               \
    /* [in] */ Float y,                                                                               \
    /* [in] */ IPaint* paint)                                                                         \
{                                                                                                     \
    return superClass::DrawPoint(x, y, paint);                                                        \
}                                                                                                     \
                                                                                                      \
ECode className::DrawLine(                                                                            \
    /* [in] */ Float startX,                                                                          \
    /* [in] */ Float startY,                                                                          \
    /* [in] */ Float stopX,                                                                           \
    /* [in] */ Float stopY,                                                                           \
    /* [in] */ IPaint* paint)                                                                         \
{                                                                                                     \
    return superClass::DrawLine(startX, startY, stopX, stopY, paint);                                 \
}                                                                                                     \
                                                                                                      \
ECode className::DrawLines(                                                                           \
    /* [in] */ const ArrayOf<Float>& pts,                                                             \
    /* [in] */ Int32 offset,                                                                          \
    /* [in] */ Int32 count,                                                                           \
    /* [in] */ IPaint* paint)                                                                         \
{                                                                                                     \
    return superClass::DrawLines(pts, offset, count, paint);                                          \
}                                                                                                     \
                                                                                                      \
ECode className::DrawLinesEx(                                                                         \
    /* [in] */ const ArrayOf<Float>& pts,                                                             \
    /* [in] */ IPaint* paint)                                                                         \
{                                                                                                     \
    return superClass::DrawLinesEx(pts, paint);                                                       \
}                                                                                                     \
                                                                                                      \
ECode className::DrawRect(                                                                            \
    /* [in] */ IRectF* rect,                                                                          \
    /* [in] */ IPaint* paint)                                                                         \
{                                                                                                     \
    return superClass::DrawRect(rect, paint);                                                         \
}                                                                                                     \
                                                                                                      \
ECode className::DrawRectEx(                                                                          \
    /* [in] */ IRect* r,                                                                              \
    /* [in] */ IPaint* paint)                                                                         \
{                                                                                                     \
    return superClass::DrawRectEx(r, paint);                                                          \
}                                                                                                     \
                                                                                                      \
ECode className::DrawRectEx2(                                                                         \
    /* [in] */ Float left,                                                                            \
    /* [in] */ Float top,                                                                             \
    /* [in] */ Float right,                                                                           \
    /* [in] */ Float bottom,                                                                          \
    /* [in] */ IPaint* paint)                                                                         \
{                                                                                                     \
    return superClass::DrawRectEx2(left, top, right, bottom, paint);                                  \
}                                                                                                     \
                                                                                                      \
ECode className::DrawOval(                                                                            \
    /* [in] */ IRectF* oval,                                                                          \
    /* [in] */ IPaint* paint)                                                                         \
{                                                                                                     \
    return superClass::DrawOval(oval, paint);                                                         \
}                                                                                                     \
                                                                                                      \
ECode className::DrawCircle(                                                                          \
    /* [in] */ Float cx,                                                                              \
    /* [in] */ Float cy,                                                                              \
    /* [in] */ Float radius,                                                                          \
    /* [in] */ IPaint* paint)                                                                         \
{                                                                                                     \
    return superClass::DrawCircle(cx, cy, radius, paint);                                             \
}                                                                                                     \
                                                                                                      \
ECode className::DrawArc(                                                                             \
    /* [in] */ IRectF* oval,                                                                          \
    /* [in] */ Float startAngle,                                                                      \
    /* [in] */ Float sweepAngle,                                                                      \
    /* [in] */ Boolean useCenter,                                                                     \
    /* [in] */ IPaint* paint)                                                                         \
{                                                                                                     \
    return superClass::DrawArc(oval, startAngle, sweepAngle, useCenter, paint);                       \
}                                                                                                     \
                                                                                                      \
ECode className::DrawRoundRect(                                                                       \
    /* [in] */ IRectF* rect,                                                                          \
    /* [in] */ Float rx,                                                                              \
    /* [in] */ Float ry,                                                                              \
    /* [in] */ IPaint* paint)                                                                         \
{                                                                                                     \
    return superClass::DrawRoundRect(rect, rx, ry, paint);                                            \
}                                                                                                     \
                                                                                                      \
ECode className::DrawPath(                                                                            \
    /* [in] */ IPath* path,                                                                           \
    /* [in] */ IPaint* paint)                                                                         \
{                                                                                                     \
    return superClass::DrawPath(path, paint);                                                         \
}                                                                                                     \
                                                                                                      \
ECode className::DrawPatch(                                                                           \
    /* [in] */ IBitmap* bitmap,                                                                       \
    /* [in] */ const ArrayOf<Byte>& chunks,                                                           \
    /* [in] */ IRectF* dst,                                                                           \
    /* [in] */ IPaint* paint)                                                                         \
{                                                                                                     \
    return superClass::DrawPatch(bitmap, chunks, dst, paint);                                         \
}                                                                                                     \
                                                                                                      \
ECode className::DrawBitmap(                                                                          \
    /* [in] */ IBitmap* bitmap,                                                                       \
    /* [in] */ Float left,                                                                            \
    /* [in] */ Float top,                                                                             \
    /* [in] */ IPaint* paint)                                                                         \
{                                                                                                     \
    return superClass::DrawBitmap(bitmap, left, top, paint);                                          \
}                                                                                                     \
                                                                                                      \
ECode className::DrawBitmapEx(                                                                        \
    /* [in] */ IBitmap* bitmap,                                                                       \
    /* [in] */ IRect* src,                                                                            \
    /* [in] */ IRectF* dst,                                                                           \
    /* [in] */ IPaint* paint)                                                                         \
{                                                                                                     \
    return superClass::DrawBitmapEx(bitmap, src, dst, paint);                                         \
}                                                                                                     \
                                                                                                      \
ECode className::DrawBitmapEx2(                                                                       \
    /* [in] */ IBitmap* bitmap,                                                                       \
    /* [in] */ IRect* src,                                                                            \
    /* [in] */ IRect* dst,                                                                            \
    /* [in] */ IPaint* paint)                                                                         \
{                                                                                                     \
    return superClass::DrawBitmapEx2(bitmap, src, dst, paint);                                        \
}                                                                                                     \
                                                                                                      \
ECode className::DrawBitmapEx3(                                                                       \
    /* [in] */ const ArrayOf<Int32>& colors,                                                          \
    /* [in] */ Int32 offset,                                                                          \
    /* [in] */ Int32 stride,                                                                          \
    /* [in] */ Float x,                                                                               \
    /* [in] */ Float y,                                                                               \
    /* [in] */ Int32 width,                                                                           \
    /* [in] */ Int32 height,                                                                          \
    /* [in] */ Boolean hasAlpha,                                                                      \
    /* [in] */ IPaint* paint)                                                                         \
{                                                                                                     \
    return superClass::DrawBitmapEx3(colors, offset, stride, x, y, width, height, hasAlpha, paint);   \
}                                                                                                     \
                                                                                                      \
ECode className::DrawBitmapEx4(                                                                       \
    /* [in] */ const ArrayOf<Int32>& colors,                                                          \
    /* [in] */ Int32 offset,                                                                          \
    /* [in] */ Int32 stride,                                                                          \
    /* [in] */ Int32 x,                                                                               \
    /* [in] */ Int32 y,                                                                               \
    /* [in] */ Int32 width,                                                                           \
    /* [in] */ Int32 height,                                                                          \
    /* [in] */ Boolean hasAlpha,                                                                      \
    /* [in] */ IPaint* paint)                                                                         \
{                                                                                                     \
    return superClass::DrawBitmapEx4(colors, offset, stride, x, y, width, height, hasAlpha, paint);   \
}                                                                                                     \
                                                                                                      \
ECode className::DrawBitmapEx5(                                                                       \
    /* [in] */ IBitmap* bitmap,                                                                       \
    /* [in] */ IMatrix* matrix,                                                                       \
    /* [in] */ IPaint* paint)                                                                         \
{                                                                                                     \
    return superClass::DrawBitmapEx5(bitmap, matrix, paint);                                          \
}                                                                                                     \
                                                                                                      \
ECode className::DrawBitmapMesh(                                                                      \
    /* [in] */ IBitmap* bitmap,                                                                       \
    /* [in] */ Int32 meshWidth,                                                                       \
    /* [in] */ Int32 meshHeight,                                                                      \
    /* [in] */ const ArrayOf<Float>& verts,                                                           \
    /* [in] */ Int32 vertOffset,                                                                      \
    /* [in] */ ArrayOf<Int32>* colors,                                                                \
    /* [in] */ Int32 colorOffset,                                                                     \
    /* [in] */ IPaint* paint)                                                                         \
{                                                                                                     \
    return superClass::DrawBitmapMesh(bitmap, meshWidth, meshHeight, verts,                           \
            vertOffset, colors, colorOffset, paint);                                                  \
}                                                                                                     \
                                                                                                      \
ECode className::DrawVertices(                                                                        \
    /* [in] */ CanvasVertexMode mode,                                                                 \
    /* [in] */ Int32 vertexCount,                                                                     \
    /* [in] */ const ArrayOf<Float>& verts,                                                           \
    /* [in] */ Int32 vertOffset,                                                                      \
    /* [in] */ ArrayOf<Float>* texs,                                                                  \
    /* [in] */ Int32 texOffset,                                                                       \
    /* [in] */ ArrayOf<Int32>* colors,                                                                \
    /* [in] */ Int32 colorOffset,                                                                     \
    /* [in] */ ArrayOf<Int16>* indices,                                                               \
    /* [in] */ Int32 indexOffset,                                                                     \
    /* [in] */ Int32 indexCount,                                                                      \
    /* [in] */ IPaint* paint)                                                                         \
{                                                                                                     \
    return superClass::DrawVertices(mode, vertexCount, verts, vertOffset, texs, texOffset,            \
            colors, colorOffset, indices, indexOffset, indexCount, paint);                            \
}                                                                                                     \
                                                                                                      \
ECode className::DrawText(                                                                            \
    /* [in] */ const ArrayOf<Char32>& text,                                                           \
    /* [in] */ Int32 index,                                                                           \
    /* [in] */ Int32 count,                                                                           \
    /* [in] */ Float x,                                                                               \
    /* [in] */ Float y,                                                                               \
    /* [in] */ IPaint* paint)                                                                         \
{                                                                                                     \
    return superClass::DrawText(text, index, count, x, y, paint);                                     \
}                                                                                                     \
                                                                                                      \
ECode className::DrawTextEx(                                                                          \
    /* [in] */ const String& text,                                                                    \
    /* [in] */ Float x,                                                                               \
    /* [in] */ Float y,                                                                               \
    /* [in] */ IPaint* paint)                                                                         \
{                                                                                                     \
    return superClass::DrawTextEx(text, x, y, paint);                                                 \
}                                                                                                     \
                                                                                                      \
ECode className::DrawTextEx2(                                                                         \
    /* [in] */ const String& text,                                                                    \
    /* [in] */ Int32 start,                                                                           \
    /* [in] */ Int32 end,                                                                             \
    /* [in] */ Float x,                                                                               \
    /* [in] */ Float y,                                                                               \
    /* [in] */ IPaint* paint)                                                                         \
{                                                                                                     \
    return superClass::DrawTextEx2(text, start, end, x, y, paint);                                    \
}                                                                                                     \
                                                                                                      \
ECode className::DrawTextEx3(                                                                         \
    /* [in] */ ICharSequence* text,                                                                   \
    /* [in] */ Int32 start,                                                                           \
    /* [in] */ Int32 end,                                                                             \
    /* [in] */ Float x,                                                                               \
    /* [in] */ Float y,                                                                               \
    /* [in] */ IPaint* paint)                                                                         \
{                                                                                                     \
    return superClass::DrawTextEx3(text, start, end, x, y, paint);                                    \
}                                                                                                     \
                                                                                                      \
ECode className::DrawTextRun(                                                                         \
    /* [in] */ const ArrayOf<Char32>& text,                                                           \
    /* [in] */ Int32 index,                                                                           \
    /* [in] */ Int32 count,                                                                           \
    /* [in] */ Int32 contextIndex,                                                                    \
    /* [in] */ Int32 contextCount,                                                                    \
    /* [in] */ Float x,                                                                               \
    /* [in] */ Float y,                                                                               \
    /* [in] */ Int32 dir,                                                                             \
    /* [in] */ IPaint* paint)                                                                         \
{                                                                                                     \
    return superClass::DrawTextRun(text, index, count, contextIndex, contextCount, x, y, dir, paint); \
}                                                                                                     \
                                                                                                      \
ECode className::DrawTextRunEx(                                                                       \
    /* [in] */ ICharSequence* text,                                                                   \
    /* [in] */ Int32 start,                                                                           \
    /* [in] */ Int32 end,                                                                             \
    /* [in] */ Int32 contextStart,                                                                    \
    /* [in] */ Int32 contextEnd,                                                                      \
    /* [in] */ Float x,                                                                               \
    /* [in] */ Float y,                                                                               \
    /* [in] */ Int32 dir,                                                                             \
    /* [in] */ IPaint* paint)                                                                         \
{                                                                                                     \
    return superClass::DrawTextRunEx(text, start, end, contextStart, contextEnd, x, y, dir, paint);   \
}                                                                                                     \
                                                                                                      \
ECode className::DrawPosText(                                                                         \
    /* [in] */ const ArrayOf<Char32>& text,                                                           \
    /* [in] */ Int32 index,                                                                           \
    /* [in] */ Int32 count,                                                                           \
    /* [in] */ const ArrayOf<Float>& pos,                                                             \
    /* [in] */ IPaint* paint)                                                                         \
{                                                                                                     \
    return superClass::DrawPosText(text, index, count, pos, paint);                                   \
}                                                                                                     \
                                                                                                      \
ECode className::DrawPosTextEx(                                                                       \
    /* [in] */ const String& text,                                                                    \
    /* [in] */ const ArrayOf<Float>& pos,                                                             \
    /* [in] */ IPaint* paint)                                                                         \
{                                                                                                     \
    return superClass::DrawPosTextEx(text, pos, paint);                                               \
}                                                                                                     \
                                                                                                      \
ECode className::DrawTextOnPath(                                                                      \
    /* [in] */ const ArrayOf<Char32>& text,                                                           \
    /* [in] */ Int32 index,                                                                           \
    /* [in] */ Int32 count,                                                                           \
    /* [in] */ IPath* path,                                                                           \
    /* [in] */ Float hOffset,                                                                         \
    /* [in] */ Float vOffset,                                                                         \
    /* [in] */ IPaint* paint)                                                                         \
{                                                                                                     \
    return superClass::DrawTextOnPath(text, index, count, path, hOffset, vOffset, paint);             \
}                                                                                                     \
                                                                                                      \
ECode className::DrawTextOnPathEx(                                                                    \
    /* [in] */ const String& text,                                                                    \
    /* [in] */ IPath* path,                                                                           \
    /* [in] */ Float hOffset,                                                                         \
    /* [in] */ Float vOffset,                                                                         \
    /* [in] */ IPaint* paint)                                                                         \
{                                                                                                     \
    return superClass::DrawTextOnPathEx(text, path, hOffset, vOffset, paint);                         \
}                                                                                                     \
                                                                                                      \
ECode className::DrawPicture(                                                                         \
    /* [in] */ IPicture* picture)                                                                     \
{                                                                                                     \
    return superClass::DrawPicture(picture);                                                          \
}                                                                                                     \
                                                                                                      \
ECode className::DrawPictureEx(                                                                       \
    /* [in] */ IPicture* picture,                                                                     \
    /* [in] */ IRectF* dst)                                                                           \
{                                                                                                     \
    return superClass::DrawPictureEx(picture, dst);                                                   \
}                                                                                                     \
                                                                                                      \
ECode className::DrawPictureEx2(                                                                      \
    /* [in] */ IPicture* picture,                                                                     \
    /* [in] */ IRect* dst)                                                                            \
{                                                                                                     \
    return superClass::DrawPictureEx2(picture, dst);                                                  \
}                                                                                                     \
                                                                                                      \
ECode className::SetSurfaceFormat(                                                                    \
    /* [in] */ Int32 format)                                                                          \
{                                                                                                     \
    return superClass::SetSurfaceFormat(format);                                                      \
}                                                                                                     \
                                                                                                      \
ECode className::GetNativeCanvas(                                                                     \
    /* [out] */ Handle32* natvieCanvas)                                                               \
{                                                                                                     \
    return superClass::GetNativeCanvas(natvieCanvas);                                                 \
}

#endif //__CANVASMACRO_H__
