
#include "ext/frameworkext.h"
#include "graphics/CPath.h"
#include "graphics/CMatrix.h"
#include "graphics/CRectF.h"
#include "graphics/CRegion.h"
#include "graphics/GraphicsNative.h"
#include <skia/core/SkPath.h>
#include <skia/core/SkMatrix.h>

namespace Elastos {
namespace Droid {
namespace Graphics {

// these must be in the same order as their native values
const PathFillType CPath::sFillTypeArray[4] = {
    PathFillType_WINDING,
    PathFillType_EVEN_ODD,
    PathFillType_INVERSE_WINDING,
    PathFillType_INVERSE_EVEN_ODD
};

CAR_OBJECT_IMPL(CPath);
CAR_INTERFACE_IMPL(CPath, Object, IPath);
CPath::CPath()
    : mNativePath(0)
    , mIsSimplePath(FALSE)
    , mLastDirection(-1)
{}

CPath::~CPath()
{
    NativeFinalizer(mNativePath);
}

ECode CPath::constructor()
{
    mNativePath = Init1();
    return NOERROR;
}

ECode CPath::constructor(
    /* [in] */ IPath* src)
{
    Int64 valNative = 0;
    if (src != NULL) {
        valNative = ((CPath*)src)->mNativePath;
        mIsSimplePath = ((CPath*)src)->mIsSimplePath;
        if (((CPath*)src)->mRects != NULL) {
            CRegion::New(((CPath*)src)->mRects, (IRegion**)&mRects);
        }
    }
    mNativePath = Init2(valNative);
    return NOERROR;
}

/**
 * Clear any lines and curves from the path, making it empty.
 * This does NOT change the fill-type setting.
 */
ECode CPath::Reset()
{
    mIsSimplePath = TRUE;
    mLastDirection = -1;
    if (mRects != NULL) mRects->SetEmpty();
    // We promised not to change this, so preserve it around the native
    // call, which does now reset fill type.
    PathFillType fillType;
    GetFillType(&fillType);
    NativeReset(mNativePath);
    return SetFillType(fillType);
}

/**
 * Rewinds the path: clears any lines and curves from the path but
 * keeps the internal data structure for faster reuse.
 */
ECode CPath::Rewind()
{
    mIsSimplePath = TRUE;
    mLastDirection = -1;
    if (mRects != NULL) mRects->SetEmpty();
    NativeRewind(mNativePath);
    return NOERROR;
}

/** Replace the contents of this with the contents of src.
*/
ECode CPath::Set(
    /* [in] */ IPath* src)
{
    CPath* path = (CPath*)src;
    if (this != path) {
        mIsSimplePath = path->mIsSimplePath;
        NativeSet(mNativePath, path->mNativePath);
    }
    return NOERROR;
}

ECode CPath::Op(
    /* [in] */ IPath* path,
    /* [in] */ PathOp op,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);
    assert(0 && "TODO");
    return Op(this, path, op, succeeded);
}

ECode CPath::Op(
    /* [in] */ IPath* path1,
    /* [in] */ IPath* path2,
    /* [in] */ PathOp op,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);
    assert(0 && "TODO");
    // if (NativeOp(path1.mNativePath, path2.mNativePath, op.ordinal(), this.mNativePath)) {
    //     isSimplePath = false;
    //     rects = null;
    //     return true;
    // }
    *succeeded = FALSE;
    return NOERROR;
}

ECode CPath::IsConvex(
    /* [out] */ Boolean* isConvex)
{
    return NativeIsConvex(mNativePath);
}

/**
 * Return the path's fill type. This defines how "inside" is
 * computed. The default value is WINDING.
 *
 * @return the path's fill type
 */
ECode CPath::GetFillType(
    /* [out] */ PathFillType* type)
{
    VALIDATE_NOT_NULL(type);

    *type = sFillTypeArray[NativeGetFillType(mNativePath)];
    return NOERROR;
}

/**
 * Set the path's fill type. This defines how "inside" is computed.
 *
 * @param ft The new fill type for this path
 */
ECode CPath::SetFillType(
    /* [in] */ PathFillType ft)
{
    NativeSetFillType(mNativePath, ft);
    return NOERROR;
}

/**
 * Returns true if the filltype is one of the INVERSE variants
 *
 * @return true if the filltype is one of the INVERSE variants
 */
ECode CPath::IsInverseFillType(
    /* [out] */ Boolean* isInverseFillType)
{
    VALIDATE_NOT_NULL(isInverseFillType);

    Int32 ft = NativeGetFillType(mNativePath);
    *isInverseFillType = (ft & PathFillType_INVERSE_WINDING) != 0;
    return NOERROR;
}

/**
 * Toggles the INVERSE state of the filltype
 */
ECode CPath::ToggleInverseFillType()
{
    Int32 ft = NativeGetFillType(mNativePath);
    ft ^= PathFillType_INVERSE_WINDING;
    NativeSetFillType(mNativePath, ft);
    return NOERROR;
}

/**
 * Returns true if the path is empty (contains no lines or curves)
 *
 * @return true if the path is empty (contains no lines or curves)
 */
ECode CPath::IsEmpty(
    /* [out] */ Boolean* isEmpty)
{
    VALIDATE_NOT_NULL(isEmpty);

    *isEmpty = NativeIsEmpty(mNativePath);
    return NOERROR;
}

/**
 * Returns true if the path specifies a rectangle. If so, and if rect is
 * not null, set rect to the bounds of the path. If the path does not
 * specify a rectangle, return false and ignore rect.
 *
 * @param rect If not null, returns the bounds of the path if it specifies
 *             a rectangle
 * @return     true if the path specifies a rectangle
 */
ECode CPath::IsRect(
    /* [in] */ IRectF* rect,
    /* [out] */ Boolean* isRect)
{
    VALIDATE_NOT_NULL(isRect);

    *isRect = NativeIsRect(mNativePath, rect);
    return NOERROR;
}

/**
 * Compute the bounds of the control points of the path, and write the
 * answer into bounds. If the path contains 0 or 1 points, the bounds is
 * set to (0,0,0,0)
 *
 * @param bounds Returns the computed bounds of the path's control points.
 * @param exact This parameter is no longer used.
 */
ECode CPath::ComputeBounds(
    /* [in] */ IRectF* bounds,
    /* [in] */ Boolean exact)
{
    NativeComputeBounds(mNativePath, bounds);
    return NOERROR;
}

/**
 * Hint to the path to prepare for adding more points. This can allow the
 * path to more efficiently allocate its storage.
 *
 * @param extraPtCount The number of extra points that may be added to this
 *                     path
 */
ECode CPath::IncReserve(
    /* [in] */ Int32 extraPtCount)
{
    NativeIncReserve(mNativePath, extraPtCount);
    return NOERROR;
}

/**
 * Set the beginning of the next contour to the point (x,y).
 *
 * @param x The x-coordinate of the start of a new contour
 * @param y The y-coordinate of the start of a new contour
 */
ECode CPath::MoveTo(
    /* [in] */ Float x,
    /* [in] */ Float y)
{
    NativeMoveTo(mNativePath, x, y);
    return NOERROR;
}

/**
 * Set the beginning of the next contour relative to the last point on the
 * previous contour. If there is no previous contour, this is treated the
 * same as moveTo().
 *
 * @param dx The amount to add to the x-coordinate of the end of the
 *           previous contour, to specify the start of a new contour
 * @param dy The amount to add to the y-coordinate of the end of the
 *           previous contour, to specify the start of a new contour
 */
ECode CPath::RMoveTo(
    /* [in] */ Float dx,
    /* [in] */ Float dy)
{
    NativeRMoveTo(mNativePath, dx, dy);
    return NOERROR;
}

/**
 * Add a line from the last point to the specified point (x,y).
 * If no moveTo() call has been made for this contour, the first point is
 * automatically set to (0,0).
 *
 * @param x The x-coordinate of the end of a line
 * @param y The y-coordinate of the end of a line
 */
ECode CPath::LineTo(
    /* [in] */ Float x,
    /* [in] */ Float y)
{
    mIsSimplePath = FALSE;
    NativeLineTo(mNativePath, x, y);
    return NOERROR;
}

/**
 * Same as lineTo, but the coordinates are considered relative to the last
 * point on this contour. If there is no previous point, then a moveTo(0,0)
 * is inserted automatically.
 *
 * @param dx The amount to add to the x-coordinate of the previous point on
 *           this contour, to specify a line
 * @param dy The amount to add to the y-coordinate of the previous point on
 *           this contour, to specify a line
 */
ECode CPath::RLineTo(
    /* [in] */ Float dx,
    /* [in] */ Float dy)
{
    mIsSimplePath = FALSE;
    NativeRLineTo(mNativePath, dx, dy);
    return NOERROR;
}

/**
 * Add a quadratic bezier from the last point, approaching control point
 * (x1,y1), and ending at (x2,y2). If no moveTo() call has been made for
 * this contour, the first point is automatically set to (0,0).
 *
 * @param x1 The x-coordinate of the control point on a quadratic curve
 * @param y1 The y-coordinate of the control point on a quadratic curve
 * @param x2 The x-coordinate of the end point on a quadratic curve
 * @param y2 The y-coordinate of the end point on a quadratic curve
 */
ECode CPath::QuadTo(
    /* [in] */ Float x1,
    /* [in] */ Float y1,
    /* [in] */ Float x2,
    /* [in] */ Float y2)
{
    mIsSimplePath = FALSE;
    NativeQuadTo(mNativePath, x1, y1, x2, y2);
    return NOERROR;
}

/**
 * Same as quadTo, but the coordinates are considered relative to the last
 * point on this contour. If there is no previous point, then a moveTo(0,0)
 * is inserted automatically.
 *
 * @param dx1 The amount to add to the x-coordinate of the last point on
 *            this contour, for the control point of a quadratic curve
 * @param dy1 The amount to add to the y-coordinate of the last point on
 *            this contour, for the control point of a quadratic curve
 * @param dx2 The amount to add to the x-coordinate of the last point on
 *            this contour, for the end point of a quadratic curve
 * @param dy2 The amount to add to the y-coordinate of the last point on
 *            this contour, for the end point of a quadratic curve
 */
ECode CPath::RQuadTo(
    /* [in] */ Float dx1,
    /* [in] */ Float dy1,
    /* [in] */ Float dx2,
    /* [in] */ Float dy2)
{
    mIsSimplePath = FALSE;
    NativeRQuadTo(mNativePath, dx1, dy1, dx2, dy2);
    return NOERROR;
}

/**
 * Add a cubic bezier from the last point, approaching control points
 * (x1,y1) and (x2,y2), and ending at (x3,y3). If no moveTo() call has been
 * made for this contour, the first point is automatically set to (0,0).
 *
 * @param x1 The x-coordinate of the 1st control point on a cubic curve
 * @param y1 The y-coordinate of the 1st control point on a cubic curve
 * @param x2 The x-coordinate of the 2nd control point on a cubic curve
 * @param y2 The y-coordinate of the 2nd control point on a cubic curve
 * @param x3 The x-coordinate of the end point on a cubic curve
 * @param y3 The y-coordinate of the end point on a cubic curve
 */
ECode CPath::CubicTo(
    /* [in] */ Float x1,
    /* [in] */ Float y1,
    /* [in] */ Float x2,
    /* [in] */ Float y2,
    /* [in] */ Float x3,
    /* [in] */ Float y3)
{
    mIsSimplePath = FALSE;
    NativeCubicTo(mNativePath, x1, y1, x2, y2, x3, y3);
    return NOERROR;
}

/**
 * Same as cubicTo, but the coordinates are considered relative to the
 * current point on this contour. If there is no previous point, then a
 * moveTo(0,0) is inserted automatically.
 */
ECode CPath::RCubicTo(
    /* [in] */ Float x1,
    /* [in] */ Float y1,
    /* [in] */ Float x2,
    /* [in] */ Float y2,
    /* [in] */ Float x3,
    /* [in] */ Float y3)
{
    mIsSimplePath = FALSE;
    NativeRCubicTo(mNativePath, x1, y1, x2, y2, x3, y3);
    return NOERROR;
}

/**
 * Append the specified arc to the path as a new contour. If the start of
 * the path is different from the path's current last point, then an
 * automatic lineTo() is added to connect the current contour to the
 * start of the arc. However, if the path is empty, then we call moveTo()
 * with the first point of the arc. The sweep angle is tread mod 360.
 *
 * @param oval        The bounds of oval defining shape and size of the arc
 * @param startAngle  Starting angle (in degrees) where the arc begins
 * @param sweepAngle  Sweep angle (in degrees) measured clockwise, treated
 *                    mod 360.
 * @param forceMoveTo If true, always begin a new contour with the arc
 */
ECode CPath::ArcTo(
    /* [in] */ IRectF* oval,
    /* [in] */ Float startAngle,
    /* [in] */ Float sweepAngle,
    /* [in] */ Boolean forceMoveTo)
{
    assert(0 && "TODO");
    // arcTo(oval.left, oval.top, oval.right, oval.bottom, startAngle, sweepAngle, forceMoveTo);
    return NOERROR;
}

/**
 * Append the specified arc to the path as a new contour. If the start of
 * the path is different from the path's current last point, then an
 * automatic lineTo() is added to connect the current contour to the
 * start of the arc. However, if the path is empty, then we call moveTo()
 * with the first point of the arc.
 *
 * @param oval        The bounds of oval defining shape and size of the arc
 * @param startAngle  Starting angle (in degrees) where the arc begins
 * @param sweepAngle  Sweep angle (in degrees) measured clockwise
 */
ECode CPath::ArcTo(
    /* [in] */ IRectF* oval,
    /* [in] */ Float startAngle,
    /* [in] */ Float sweepAngle)
{
    assert(0 && "TODO");
    // arcTo(oval.left, oval.top, oval.right, oval.bottom, startAngle, sweepAngle, false);
    return NOERROR;
}

ECode CPath::ArcTo(
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [in] */ Float startAngle,
    /* [in] */ Float sweepAngle,
    /* [in] */ Boolean forceMoveTo)
{
    mIsSimplePath = FALSE;
    assert(0 && "TODO");
    // NativeArcTo(mNativePath, left, top, right, bottom, startAngle, sweepAngle, forceMoveTo);
    return NOERROR;
}

/**
 * Close the current contour. If the current point is not equal to the
 * first point of the contour, a line segment is automatically added.
 */
ECode CPath::Close()
{
    mIsSimplePath = FALSE;
    NativeClose(mNativePath);
    return NOERROR;
}

void CPath::DetectSimplePath(
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [in] */ PathDirection dir)
{
    if (mLastDirection == -1) {
        mLastDirection = dir;
    }
    if (mLastDirection != dir) {
        mIsSimplePath = FALSE;
    }
    else {
        if (mRects == NULL) {
            CRegion::New((IRegion**)&mRects);
        }
        Boolean result;
        mRects->Op((Int32)left, (Int32)top, (Int32)right, (Int32)bottom, RegionOp_UNION, &result);
    }
}

/**
 * Add a closed rectangle contour to the path
 *
 * @param rect The rectangle to add as a closed contour to the path
 * @param dir  The direction to wind the rectangle's contour
 */
ECode CPath::AddRect(
    /* [in] */ IRectF* rect,
    /* [in] */ PathDirection dir)
{
    assert(0 && "TODO");
    // AddRect(rect.left, rect.top, rect.right, rect.bottom, dir);
    return NOERROR;
}

/**
 * Add a closed rectangle contour to the path
 *
 * @param left   The left side of a rectangle to add to the path
 * @param top    The top of a rectangle to add to the path
 * @param right  The right side of a rectangle to add to the path
 * @param bottom The bottom of a rectangle to add to the path
 * @param dir    The direction to wind the rectangle's contour
 */
ECode CPath::AddRect(
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [in] */ PathDirection dir)
{
    DetectSimplePath(left, top, right, bottom, dir);
    NativeAddRect(mNativePath, left, top, right, bottom, dir);
    return NOERROR;
}

ECode CPath::AddOval(
    /* [in] */ IRectF* oval,
    /* [in] */ PathDirection dir)
{
    assert(0 && "TODO");
    // AddOval(oval.left, oval.top, oval.right, oval.bottom, dir);
    return NOERROR;
}

ECode CPath::AddOval(
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [in] */ PathDirection dir)
{
    mIsSimplePath = FALSE;
    assert(0 && "TODO");
    // native_addOval(mNativePath, left, top, right, bottom, dir.nativeInt);
    return NOERROR;
}

/**
 * Add a closed circle contour to the path
 *
 * @param x   The x-coordinate of the center of a circle to add to the path
 * @param y   The y-coordinate of the center of a circle to add to the path
 * @param radius The radius of a circle to add to the path
 * @param dir    The direction to wind the circle's contour
 */
ECode CPath::AddCircle(
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Float radius,
    /* [in] */ PathDirection dir)
{
    mIsSimplePath = FALSE;
    NativeAddCircle(mNativePath, x, y, radius, dir);
    return NOERROR;
}

/**
 * Add the specified arc to the path as a new contour.
 *
 * @param oval The bounds of oval defining the shape and size of the arc
 * @param startAngle Starting angle (in degrees) where the arc begins
 * @param sweepAngle Sweep angle (in degrees) measured clockwise
 */
ECode CPath::AddArc(
    /* [in] */ IRectF* oval,
    /* [in] */ Float startAngle,
    /* [in] */ Float sweepAngle)
{
    assert(0 && "TODO");
    // AddArc(oval.left, oval.top, oval.right, oval.bottom, startAngle, sweepAngle);
    return NOERROR;
}

ECode CPath::AddArc(
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [in] */ Float startAngle,
    /* [in] */ Float sweepAngle)
{
    mIsSimplePath = FALSE;
    assert(0 && "TODO");
    // native_addArc(mNativePath, left, top, right, bottom, startAngle, sweepAngle);
    return NOERROR;
}

/**
    * Add a closed round-rectangle contour to the path
 *
 * @param rect The bounds of a round-rectangle to add to the path
 * @param rx   The x-radius of the rounded corners on the round-rectangle
 * @param ry   The y-radius of the rounded corners on the round-rectangle
 * @param dir  The direction to wind the round-rectangle's contour
 */
ECode CPath::AddRoundRect(
    /* [in] */ IRectF* rect,
    /* [in] */ Float rx,
    /* [in] */ Float ry,
    /* [in] */ PathDirection dir)
{
    assert(0 && "TODO");
    // AddRoundRect(rect.left, rect.top, rect.right, rect.bottom, rx, ry, dir);
    return NOERROR;
}

ECode CPath::AddRoundRect(
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [in] */ Float rx,
    /* [in] */ Float ry,
    /* [in] */ PathDirection dir)
{
    mIsSimplePath = FALSE;
    assert(0 && "TODO");
    // native_addRoundRect(mNativePath, left, top, right, bottom, rx, ry, dir.nativeInt);
    return NOERROR;
}

/**
 * Add a closed round-rectangle contour to the path. Each corner receives
 * two radius values [X, Y]. The corners are ordered top-left, top-right,
 * bottom-right, bottom-left
 *
 * @param rect The bounds of a round-rectangle to add to the path
 * @param radii Array of 8 values, 4 pairs of [X,Y] radii
 * @param dir  The direction to wind the round-rectangle's contour
 */
ECode CPath::AddRoundRect(
    /* [in] */ IRectF* rect,
    /* [in] */ ArrayOf<Float>* radii,
    /* [in] */ PathDirection dir)
{
    if (rect == NULL) {
//        throw new NullPointerException("need rect parameter");
        return E_NULL_POINTER_EXCEPTION;
    }
    assert(0 && "TODO");
    // addRoundRect(rect.left, rect.top, rect.right, rect.bottom, radii, dir);
    return NOERROR;
}

ECode CPath::AddRoundRect(
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [in] */ ArrayOf<Float>* radii,
    /* [in] */ PathDirection dir)
{
    assert(0 && "TODO");
    // if (radii.length < 8) {
    //     throw new ArrayIndexOutOfBoundsException("radii[] needs 8 values");
    // }
    // mIsSimplePath = false;
    // native_addRoundRect(mNativePath, left, top, right, bottom, radii, dir.nativeInt);
    return NOERROR;
}

/**
 * Add a copy of src to the path, offset by (dx,dy)
 *
 * @param src The path to add as a new contour
 * @param dx  The amount to translate the path in X as it is added
 */
ECode CPath::AddPath(
    /* [in] */ IPath* src,
    /* [in] */ Float dx,
    /* [in] */ Float dy)
{
    mIsSimplePath = FALSE;
    NativeAddPath(mNativePath, ((CPath*)src)->mNativePath, dx, dy);
    return NOERROR;
}

/**
 * Add a copy of src to the path
 *
 * @param src The path that is appended to the current path
 */
ECode CPath::AddPath(
    /* [in] */ IPath* src)
{
    mIsSimplePath = FALSE;
    NativeAddPath(mNativePath, ((CPath*)src)->mNativePath);
    return NOERROR;
}

/**
 * Add a copy of src to the path, transformed by matrix
 *
 * @param src The path to add as a new contour
 */
ECode CPath::AddPath(
    /* [in] */ IPath* src,
    /* [in] */ IMatrix* matrix)
{
    if (!((CPath*)src)->mIsSimplePath) mIsSimplePath = FALSE;
    NativeAddPath(mNativePath, ((CPath*)src)->mNativePath,
            ((Matrix*)(IMatrix*)matrix->Probe(EIID_Matrix))->mNativeInstance);
    return NOERROR;
}

/**
 * Offset the path by (dx,dy), returning true on success
 *
 * @param dx  The amount in the X direction to offset the entire path
 * @param dy  The amount in the Y direction to offset the entire path
 * @param dst The translated path is written here. If this is null, then
 *            the original path is modified.
 */
ECode CPath::Offset(
    /* [in] */ Float dx,
    /* [in] */ Float dy,
    /* [in] */ IPath* dst)
{
    Int64 dstNative = 0;
    if (dst != NULL) {
        dstNative = ((CPath*)dst)->mNativePath;
        ((CPath*)dst)->mIsSimplePath = FALSE;
    }
    NativeOffset(mNativePath, dx, dy, dstNative);
    return NOERROR;
}

/**
 * Offset the path by (dx,dy), returning true on success
 *
 * @param dx The amount in the X direction to offset the entire path
 * @param dy The amount in the Y direction to offset the entire path
 */
ECode CPath::Offset(
    /* [in] */ Float dx,
    /* [in] */ Float dy)
{
    mIsSimplePath = FALSE;
    NativeOffset(mNativePath, dx, dy);
    return NOERROR;
}

/**
 * Sets the last point of the path.
 *
 * @param dx The new X coordinate for the last point
 * @param dy The new Y coordinate for the last point
 */
ECode CPath::SetLastPoint(
    /* [in] */ Float dx,
    /* [in] */ Float dy)
{
    mIsSimplePath = FALSE;
    NativeSetLastPoint(mNativePath, dx, dy);
    return NOERROR;
}

/**
 * Transform the points in this path by matrix, and write the answer
 * into dst. If dst is null, then the the original path is modified.
 *
 * @param matrix The matrix to apply to the path
 * @param dst    The transformed path is written here. If dst is null,
 *               then the the original path is modified
 */
ECode CPath::Transform(
    /* [in] */ IMatrix* matrix,
    /* [in] */ IPath* dst)
{
    Int64 dstNative = 0;
    if (dst != NULL) {
        ((CPath*)dst)->mIsSimplePath = FALSE;
        dstNative = ((CPath*)dst)->mNativePath;
    }
    NativeTransform(mNativePath, ((Matrix*)(IMatrix*)matrix->Probe(EIID_Matrix))->mNativeInstance, dstNative);
    return NOERROR;
}

/**
 * Transform the points in this path by matrix.
 *
 * @param matrix The matrix to apply to the path
 */
ECode CPath::Transform(
    /* [in] */ IMatrix* matrix)
{
    mIsSimplePath = FALSE;
    NativeTransform(mNativePath, ((Matrix*)(IMatrix*)matrix->Probe(EIID_Matrix))->mNativeInstance);
    return NOERROR;
}

Int64 CPath::Ni()
{
    return mNativePath;
}

ECode CPath::Approximate(
    /* [in] */ Float acceptableError,
    /* [out] */ ArrayOf<Float>** array)
{
    assert(0 && "TODO");
    // return native_approximate(mNativePath, acceptableError);
    return NOERROR;
}

Int64 CPath::Init1()
{
    return (Int64)new SkPath();
}

Int64 CPath::Init2(
    /* [in] */ Int64 nPath)
{
    return (Int64)new SkPath(*(SkPath*)nPath);
}

void CPath::NativeReset(
    /* [in] */ Int64 nPath)
{
    ((SkPath*)nPath)->reset();
}

void CPath::NativeRewind(
    /* [in] */ Int64 nPath)
{
    ((SkPath*)nPath)->rewind();
}

void CPath::NativeSet(
    /* [in] */ Int64 nDst,
    /* [in] */ Int64 nSrc)
{
    assert(0 && "Need jni codes.");
}

Boolean CPath::NativeIsConvex(
    /* [in] */ Int64 nPath)
{
    assert(0 && "Need jni codes.");
    return FALSE;
}

Int32 CPath::NativeGetFillType(
    /* [in] */ Int64 nPath)
{
    return ((SkPath*)nPath)->getFillType();
}

void CPath::NativeSetFillType(
    /* [in] */ Int64 nPath,
    /* [in] */ PathFillType ft)
{
    ((SkPath*)nPath)->setFillType((SkPath::FillType)ft);
}

Boolean CPath::NativeIsEmpty(
    /* [in] */ Int64 nPath)
{
    return ((SkPath*)nPath)->isEmpty();
}

Boolean CPath::NativeIsRect(
    /* [in] */ Int64 nPath,
    /* [in] */ IRectF* rect)
{
    SkRect rect_;
    Boolean result = ((SkPath*)nPath)->isRect(&rect_);
    GraphicsNative::SkRect2IRectF(rect_, rect);
    return result;
}

void CPath::NativeComputeBounds(
    /* [in] */ Int64 nPath,
    /* [in] */ IRectF* bounds)
{
    const SkRect& bounds_ = ((SkPath*)nPath)->getBounds();
    GraphicsNative::SkRect2IRectF(bounds_, bounds);
}

void CPath::NativeIncReserve(
    /* [in] */ Int64 nPath,
    /* [in] */ Int32 extraPtCount)
{
    ((SkPath*)nPath)->incReserve(extraPtCount);
}

void CPath::NativeMoveTo(
    /* [in] */ Int64 nPath,
    /* [in] */ Float x,
    /* [in] */ Float y)
{
    assert(0 && "TODO: Need jni codes.");
}

void CPath::NativeRMoveTo(
    /* [in] */ Int64 nPath,
    /* [in] */ Float dx,
    /* [in] */ Float dy)
{
    assert(0 && "TODO: Need jni codes.");
}

void CPath::NativeLineTo(
    /* [in] */ Int64 nPath,
    /* [in] */ Float x,
    /* [in] */ Float y)
{
    assert(0 && "TODO: Need jni codes.");
}

void CPath::NativeRLineTo(
    /* [in] */ Int64 nPath,
    /* [in] */ Float dx,
    /* [in] */ Float dy)
{
    assert(0 && "TODO: Need jni codes.");
}

void CPath::NativeQuadTo(
    /* [in] */ Int64 nPath,
    /* [in] */ Float x1,
    /* [in] */ Float y1,
    /* [in] */ Float x2,
    /* [in] */ Float y2)
{
    assert(0 && "TODO: Need jni codes.");
}

void CPath::NativeRQuadTo(
    /* [in] */ Int64 nPath,
    /* [in] */ Float dx1,
    /* [in] */ Float dy1,
    /* [in] */ Float dx2,
    /* [in] */ Float dy2)
{
    assert(0 && "TODO: Need jni codes.");
}

void CPath::NativeCubicTo(
    /* [in] */ Int64 nPath,
    /* [in] */ Float x1,
    /* [in] */ Float y1,
    /* [in] */ Float x2,
    /* [in] */ Float y2,
    /* [in] */ Float x3,
    /* [in] */ Float y3)
{
    assert(0 && "TODO: Need jni codes.");
}

void CPath::NativeRCubicTo(
    /* [in] */ Int64 nPath,
    /* [in] */ Float x1,
    /* [in] */ Float y1,
    /* [in] */ Float x2,
    /* [in] */ Float y2,
    /* [in] */ Float x3,
    /* [in] */ Float y3)
{
    assert(0 && "TODO: Need jni codes.");
}


void CPath::NativeArcTo(
    /* [in] */ Int64 nPath,
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [in] */ Float startAngle,
    /* [in] */ Float sweepAngle,
    /* [in] */ Boolean forceMoveTo)
{
    assert(0 && "TODO: Need jni codes.");
}

void CPath::NativeClose(
    /* [in] */ Int64 nPath)
{
    ((SkPath*)nPath)->close();
}

void CPath::NativeAddRect(
    /* [in] */ Int64 nPath,
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [in] */ PathDirection dir)
{
    assert(0 && "TODO: Need jni codes.");
}

void CPath::NativeAddOval(
    /* [in] */ Int64 nPath,
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [in] */ Int32 dir)
{
    assert(0 && "TODO: Need jni codes.");
}

void CPath::NativeAddCircle(
    /* [in] */ Int64 nPath,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Float radius,
    /* [in] */ PathDirection dir)
{
    assert(0 && "TODO: Need jni codes.");
}

void CPath::NativeAddArc(
    /* [in] */ Int64 nPath,
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [in] */ Float startAngle,
    /* [in] */ Float sweepAngle)
{
    assert(0 && "TODO: Need jni codes.");
}

void CPath::NativeAddRoundRect(
    /* [in] */ Int64 nPath,
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [in] */ Float rx,
    /* [in] */ Float ry,
    /* [in] */ Int32 dir)
{
    assert(0 && "TODO: Need jni codes.");
}

void CPath::NativeAddRoundRect(
    /* [in] */ Int64 nPath,
    /* [in] */ Float left,
    /* [in] */ Float top,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [in] */ ArrayOf<Float>* radii,
    /* [in] */ Int32 dir)
{
    assert(0 && "TODO: Need jni codes.");
}

void CPath::NativeAddPath(
    /* [in] */ Int64 nPath,
    /* [in] */ Int64 src,
    /* [in] */ Float dx,
    /* [in] */ Float dy)
{
    assert(0 && "TODO: Need jni codes.");
}

void CPath::NativeAddPath(
    /* [in] */ Int64 nPath,
    /* [in] */ Int64 src)
{
    ((SkPath*)nPath)->addPath(*(SkPath*)src);
}

void CPath::NativeAddPath(
    /* [in] */ Int64 nPath,
    /* [in] */ Int64 src,
    /* [in] */ Int64 matrix)
{
    ((SkPath*)nPath)->addPath(*(SkPath*)src, *(SkMatrix*)matrix);
}

void CPath::NativeOffset(
    /* [in] */ Int64 nPath,
    /* [in] */ Float dx,
    /* [in] */ Float dy,
    /* [in] */ Int32 dst_path)
{
    assert(0 && "TODO: Need jni codes.");
}

void CPath::NativeOffset(
    /* [in] */ Int64 nPath,
    /* [in] */ Float dx,
    /* [in] */ Float dy)
{
    assert(0 && "TODO: Need jni codes.");
}

void CPath::NativeSetLastPoint(
    /* [in] */ Int64 nPath,
    /* [in] */ Float dx,
    /* [in] */ Float dy)
{
    assert(0 && "TODO: Need jni codes.");
}

void CPath::NativeTransform(
    /* [in] */ Int64 nPath,
    /* [in] */ Int64 matrix,
    /* [in] */ Int64 dst_path)
{
    ((SkPath*)nPath)->transform(*(SkMatrix*)matrix, (SkPath*)dst_path);
}

void CPath::NativeTransform(
    /* [in] */ Int64 nPath,
    /* [in] */ Int64 matrix)
{
    ((SkPath*)nPath)->transform(*(SkMatrix*)matrix);
}

void CPath::NativeFinalizer(
    /* [in] */ Int64 nPath)
{
#ifdef USE_OPENGL_RENDERER
    if (android::uirenderer::Caches::hasInstance()) {
        android::uirenderer::Caches::getInstance().resourceCache.destructor((SkPath*)nPath);
        return;
    }
#endif
    delete (SkPath*)nPath;
}

Boolean CPath::NativeOp(
    /* [in] */ Int64 path1,
    /* [in] */ Int64 path2,
    /* [in] */ Int32 op,
    /* [in] */ Int64 result)
{
    assert(0 && "TODO: Need jni codes.");
    return FALSE;
}

AutoPtr<ArrayOf<Float> > CPath::NativeApproximate(
    /* [in] */ Int64 nPath,
    /* [in] */ Float error)
{
    assert(0 && "TODO: Need jni codes.");
    return NULL;
}

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos
