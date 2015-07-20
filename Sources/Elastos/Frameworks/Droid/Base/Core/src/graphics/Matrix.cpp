
#include "ext/frameworkext.h"
#include "graphics/Matrix.h"
#include "graphics/GraphicsNative.h"
#include <elastos/core/StringBuilder.h>
#include <skia/core/SkMatrix.h>

using Elastos::Core::StringBuilder;

namespace Elastos {
namespace Droid {
namespace Graphics {

// {01FD9701-2074-4DDE-98A8-81D5FEB5C140}
extern const InterfaceID EIID_Matrix =
    { 0x1fd9701, 0x2074, 0x4dde, { 0x98, 0xa8, 0x81, 0xd5, 0xfe, 0xb5, 0xc1, 0x40 } };

Matrix::~Matrix()
{
    NativeFinalizer(mNativeInstance);
}

ECode Matrix::Init()
{
    mNativeInstance = NativeCreate(0);
    return NOERROR;
}

ECode Matrix::Init(
    /* [in] */ IMatrix* src)
{
    Int32 nSrc = src != NULL? ((Matrix*)src->Probe(EIID_Matrix))->mNativeInstance : NULL;
    mNativeInstance = NativeCreate(nSrc);
    return NOERROR;
}

/**
 * Returns true if the matrix is identity.
 * This maybe faster than testing if (getType() == 0)
 */
ECode Matrix::IsIdentity(
    /* [out] */ Boolean* isIdentity)
{
    *isIdentity = NativeIsIdentity(mNativeInstance);
    return NOERROR;
}

/* Returns true if will map a rectangle to another rectangle. This can be
 * true if the matrix is identity, scale-only, or rotates a multiple of 90
 * degrees.
 */
ECode Matrix::RectStaysRect(
    /* [out] */ Boolean* result)
{
    *result = NativeRectStaysRect(mNativeInstance);
    return NOERROR;
}

/**
 * (deep) copy the src matrix into this matrix. If src is null, reset this
 * matrix to the identity matrix.
 */
ECode Matrix::Set(
    /* [in] */ IMatrix* src)
{
    if (src == NULL) {
        Reset();
    }
    else {
        NativeSet(mNativeInstance, ((Matrix*)src->Probe(EIID_Matrix))->mNativeInstance);
    }
    return NOERROR;
}

/** Returns true iff obj is a Matrix and its values equal our values.
 */
ECode Matrix::Equals(
    /* [in] */ IMatrix* obj,
    /* [out] */ Boolean* isEqual)
{
    *isEqual = obj != NULL &&
               NativeEquals(mNativeInstance, ((Matrix*)obj->Probe(EIID_Matrix))->mNativeInstance);
    return NOERROR;
}

ECode Matrix::GetHashCode(
    /* [out] */ Int32* hash)
{
    VALIDATE_NOT_NULL(hash);
    *hash = (Int32)mNativeInstance;
    return NOERROR;
}

/** Set the matrix to identity */
ECode Matrix::Reset()
{
    NativeReset(mNativeInstance);
    return NOERROR;
}

/** Set the matrix to translate by (dx, dy). */
ECode Matrix::SetTranslate(
    /* [in] */ Float dx,
    /* [in] */ Float dy)
{
    NativeSetTranslate(mNativeInstance, dx, dy);
    return NOERROR;
}

/**
 * Set the matrix to scale by sx and sy, with a pivot point at (px, py).
 * The pivot point is the coordinate that should remain unchanged by the
 * specified transformation.
 */
ECode Matrix::SetScale(
    /* [in] */ Float sx,
    /* [in] */ Float sy,
    /* [in] */ Float px,
    /* [in] */ Float py)
{
    NativeSetScale(mNativeInstance, sx, sy, px, py);
    return NOERROR;
}

/** Set the matrix to scale by sx and sy. */
ECode Matrix::SetScale(
    /* [in] */ Float sx,
    /* [in] */ Float sy)
{
    NativeSetScale(mNativeInstance, sx, sy);
    return NOERROR;
}

/**
 * Set the matrix to rotate by the specified number of degrees, with a pivot
 * point at (px, py). The pivot point is the coordinate that should remain
 * unchanged by the specified transformation.
 */
ECode Matrix::SetRotate(
    /* [in] */ Float degrees,
    /* [in] */ Float px,
    /* [in] */ Float py)
{
    NativeSetRotate(mNativeInstance, degrees, px, py);
    return NOERROR;
}

/**
 * Set the matrix to rotate about (0,0) by the specified number of degrees.
 */
ECode Matrix::SetRotate(
    /* [in] */ Float degrees)
{
    NativeSetRotate(mNativeInstance, degrees);
    return NOERROR;
}

/**
 * Set the matrix to rotate by the specified sine and cosine values, with a
 * pivot point at (px, py). The pivot point is the coordinate that should
 * remain unchanged by the specified transformation.
 */
ECode Matrix::SetSinCos(
    /* [in] */ Float sinValue,
    /* [in] */ Float cosValue,
    /* [in] */ Float px,
    /* [in] */ Float py)
{
    NativeSetSinCos(mNativeInstance, sinValue, cosValue, px, py);
    return NOERROR;
}

/** Set the matrix to rotate by the specified sine and cosine values. */
ECode Matrix::SetSinCos(
    /* [in] */ Float sinValue,
    /* [in] */ Float cosValue)
{
    NativeSetSinCos(mNativeInstance, sinValue, cosValue);
    return NOERROR;
}

/**
 * Set the matrix to skew by sx and sy, with a pivot point at (px, py).
 * The pivot point is the coordinate that should remain unchanged by the
 * specified transformation.
 */
ECode Matrix::SetSkew(
    /* [in] */ Float kx,
    /* [in] */ Float ky,
    /* [in] */ Float px,
    /* [in] */ Float py)
{
    NativeSetSkew(mNativeInstance, kx, ky, px, py);
    return NOERROR;
}

/** Set the matrix to skew by sx and sy. */
ECode Matrix::SetSkew(
    /* [in] */ Float kx,
    /* [in] */ Float ky)
{
    NativeSetSkew(mNativeInstance, kx, ky);
    return NOERROR;
}

/**
 * Set the matrix to the concatenation of the two specified matrices,
 * returning true if the the result can be represented. Either of the two
 * matrices may also be the target matrix. this = a * b
 */
ECode Matrix::SetConcat(
    /* [in] */ IMatrix* a,
    /* [in] */ IMatrix* b,
    /* [out] */ Boolean* result)
{
    *result = NativeSetConcat(mNativeInstance,
            ((Matrix*)a->Probe(EIID_Matrix))->mNativeInstance,
            ((Matrix*)b->Probe(EIID_Matrix))->mNativeInstance);
    return NOERROR;
}

/**
 * Preconcats the matrix with the specified translation.
 * M' = M * T(dx, dy)
 */
ECode Matrix::PreTranslate(
    /* [in] */ Float dx,
    /* [in] */ Float dy,
    /* [out] */ Boolean* result)
{
    *result = NativePreTranslate(mNativeInstance, dx, dy);
    return NOERROR;
}

/**
 * Preconcats the matrix with the specified scale.
 * M' = M * S(sx, sy, px, py)
 */
ECode Matrix::PreScale(
    /* [in] */ Float sx,
    /* [in] */ Float sy,
    /* [in] */ Float px,
    /* [in] */ Float py,
    /* [out] */ Boolean* result)
{
    *result = NativePreScale(mNativeInstance, sx, sy, px, py);
    return NOERROR;
}

/**
 * Preconcats the matrix with the specified scale.
 * M' = M * S(sx, sy)
 */
ECode Matrix::PreScale(
    /* [in] */ Float sx,
    /* [in] */ Float sy,
    /* [out] */ Boolean* result)
{
    *result = NativePreScale(mNativeInstance, sx, sy);
    return NOERROR;
}

/**
 * Preconcats the matrix with the specified rotation.
 * M' = M * R(degrees, px, py)
 */
ECode Matrix::PreRotate(
    /* [in] */ Float degrees,
    /* [in] */ Float px,
    /* [in] */ Float py,
    /* [out] */ Boolean* result)
{
    *result = NativePreRotate(mNativeInstance, degrees, px, py);
    return NOERROR;
}

/**
 * Preconcats the matrix with the specified rotation.
 * M' = M * R(degrees)
 */
ECode Matrix::PreRotate(
    /* [in] */ Float degrees,
    /* [out] */ Boolean* result)
{
    *result = NativePreRotate(mNativeInstance, degrees);
    return NOERROR;
}

/**
 * Preconcats the matrix with the specified skew.
 * M' = M * K(kx, ky, px, py)
 */
ECode Matrix::PreSkew(
    /* [in] */ Float kx,
    /* [in] */ Float ky,
    /* [in] */ Float px,
    /* [in] */ Float py,
    /* [out] */ Boolean* result)
{
    *result = NativePreSkew(mNativeInstance, kx, ky, px, py);
    return NOERROR;
}

/**
 * Preconcats the matrix with the specified skew.
 * M' = M * K(kx, ky)
 */
ECode Matrix::PreSkew(
    /* [in] */ Float kx,
    /* [in] */ Float ky,
    /* [out] */ Boolean* result)
{
    *result = NativePreSkew(mNativeInstance, kx, ky);
    return NOERROR;
}

/**
 * Preconcats the matrix with the specified matrix.
 * M' = M * other
 */
ECode Matrix::PreConcat(
    /* [in] */ IMatrix* other,
    /* [out] */ Boolean* result)
{
    *result = NativePreConcat(mNativeInstance, ((Matrix*)other->Probe(EIID_Matrix))->mNativeInstance);
    return NOERROR;
}

/**
 * Postconcats the matrix with the specified translation.
 * M' = T(dx, dy) * M
 */
ECode Matrix::PostTranslate(
    /* [in] */ Float dx,
    /* [in] */ Float dy,
    /* [out] */ Boolean* result)
{
    *result = NativePostTranslate(mNativeInstance, dx, dy);
    return NOERROR;
}

/**
 * Postconcats the matrix with the specified scale.
 * M' = S(sx, sy, px, py) * M
 */
ECode Matrix::PostScale(
    /* [in] */ Float sx,
    /* [in] */ Float sy,
    /* [in] */ Float px,
    /* [in] */ Float py,
    /* [out] */ Boolean* result)
{
    *result = NativePostScale(mNativeInstance, sx, sy, px, py);
    return NOERROR;
}

/**
 * Postconcats the matrix with the specified scale.
 * M' = S(sx, sy) * M
 */
ECode Matrix::PostScale(
    /* [in] */ Float sx,
    /* [in] */ Float sy,
    /* [out] */ Boolean* result)
{
    *result = NativePostScale(mNativeInstance, sx, sy);
    return NOERROR;
}

/**
 * Postconcats the matrix with the specified rotation.
 * M' = R(degrees, px, py) * M
 */
ECode Matrix::PostRotate(
    /* [in] */ Float degrees,
    /* [in] */ Float px,
    /* [in] */ Float py,
    /* [out] */ Boolean* result)
{
    *result = NativePostRotate(mNativeInstance, degrees, px, py);
    return NOERROR;
}

/**
 * Postconcats the matrix with the specified rotation.
 * M' = R(degrees) * M
 */
ECode Matrix::PostRotate(
    /* [in] */ Float degrees,
    /* [out] */ Boolean* result)
{
    *result = NativePostRotate(mNativeInstance, degrees);
    return NOERROR;
}

/**
 * Postconcats the matrix with the specified skew.
 * M' = K(kx, ky, px, py) * M
 */
ECode Matrix::PostSkew(
    /* [in] */ Float kx,
    /* [in] */ Float ky,
    /* [in] */ Float px,
    /* [in] */ Float py,
    /* [out] */ Boolean* result)
{
    *result = NativePostSkew(mNativeInstance, kx, ky, px, py);
    return NOERROR;
}

/**
 * Postconcats the matrix with the specified skew.
 * M' = K(kx, ky) * M
 */
ECode Matrix::PostSkew(
    /* [in] */ Float kx,
    /* [in] */ Float ky,
    /* [out] */ Boolean* result)
{
    *result = NativePostSkew(mNativeInstance, kx, ky);
    return NOERROR;
}

/**
 * Postconcats the matrix with the specified matrix.
 * M' = other * M
 */
ECode Matrix::PostConcat(
    /* [in] */ IMatrix* other,
    /* [out] */ Boolean* result)
{
    *result = NativePostConcat(mNativeInstance, ((Matrix*)other->Probe(EIID_Matrix))->mNativeInstance);
    return NOERROR;
}

/**
 * Set the matrix to the scale and translate values that map the source
 * rectangle to the destination rectangle, returning true if the the result
 * can be represented.
 *
 * @param src the source rectangle to map from.
 * @param dst the destination rectangle to map to.
 * @param stf the ScaleToFit option
 * @return true if the matrix can be represented by the rectangle mapping.
 */
ECode Matrix::SetRectToRect(
    /* [in] */ IRectF* src,
    /* [in] */ IRectF* dst,
    /* [in] */ MatrixScaleToFit stf,
    /* [out] */ Boolean* result)
{
    if (dst == NULL || src == NULL) {
//        throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }
    *result = NativeSetRectToRect(mNativeInstance, src, dst,
            (SkMatrix::ScaleToFit)stf);
    return NOERROR;
}

// private helper to perform range checks on arrays of "points"
ECode Matrix::CheckPointArrays(
    /* [in] */ const ArrayOf<Float>& src,
    /* [in] */ Int32 srcIndex,
    /* [out] */ ArrayOf<Float>* dst,
    /* [in] */ Int32 dstIndex,
    /* [in] */ Int32 pointCount)
{
    if (dst == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }
    // check for too-small and too-big indices
    Int32 srcStop = srcIndex + (pointCount << 1);
    Int32 dstStop = dstIndex + (pointCount << 1);
    if ((pointCount | srcIndex | dstIndex | srcStop | dstStop) < 0 ||
            srcStop > src.GetLength() || dstStop > dst->GetLength()) {
//        throw new ArrayIndexOutOfBoundsException();
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    return NOERROR;
}

/**
 * Set the matrix such that the specified src points would map to the
 * specified dst points. The "points" are represented as an array of floats,
 * order [x0, y0, x1, y1, ...], where each "point" is 2 float values.
 *
 * @param src   The array of src [x,y] pairs (points)
 * @param srcIndex Index of the first pair of src values
 * @param dst   The array of dst [x,y] pairs (points)
 * @param dstIndex Index of the first pair of dst values
 * @param pointCount The number of pairs/points to be used. Must be [0..4]
 * @return true if the matrix was set to the specified transformation
 */
ECode Matrix::SetPolyToPoly(
    /* [in] */ const ArrayOf<Float>& src,
    /* [in] */ Int32 srcIndex,
    /* [out] */ ArrayOf<Float>* dst,
    /* [in] */ Int32 dstIndex,
    /* [in] */ Int32 pointCount,
    /* [out] */ Boolean* result)
{
    if (pointCount > 4) {
//        throw new IllegalArgumentException();
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    FAIL_RETURN(CheckPointArrays(src, srcIndex, dst, dstIndex, pointCount));
    *result = NativeSetPolyToPoly(mNativeInstance, src, srcIndex,
                                  dst, dstIndex, pointCount);
    return NOERROR;
}

/**
 * If this matrix can be inverted, return true and if inverse is not null,
 * set inverse to be the inverse of this matrix. If this matrix cannot be
 * inverted, ignore inverse and return false.
 */
ECode Matrix::Invert(
    /* [in] */ IMatrix* inverse,
    /* [out] */ Boolean* result)
{
    *result = NativeInvert(mNativeInstance, ((Matrix*)inverse->Probe(EIID_Matrix))->mNativeInstance);
    return NOERROR;
}

/**
* Apply this matrix to the array of 2D points specified by src, and write
 * the transformed points into the array of points specified by dst. The
 * two arrays represent their "points" as pairs of floats [x, y].
 *
 * @param dst   The array of dst points (x,y pairs)
 * @param dstIndex The index of the first [x,y] pair of dst floats
 * @param src   The array of src points (x,y pairs)
 * @param srcIndex The index of the first [x,y] pair of src floats
 * @param pointCount The number of points (x,y pairs) to transform
 */
ECode Matrix::MapPoints(
    /* [out] */ ArrayOf<Float>* dst,
    /* [in] */ Int32 dstIndex,
    /* [in] */ const ArrayOf<Float>& src,
    /* [in] */ Int32 srcIndex,
    /* [in] */ Int32 pointCount)
{
    FAIL_RETURN(CheckPointArrays(src, srcIndex, dst, dstIndex, pointCount));
    NativeMapPoints(mNativeInstance, dst, dstIndex, src, srcIndex,
                    pointCount, TRUE);
    return NOERROR;
}

/**
* Apply this matrix to the array of 2D vectors specified by src, and write
 * the transformed vectors into the array of vectors specified by dst. The
 * two arrays represent their "vectors" as pairs of floats [x, y].
 *
 * Note: this method does not apply the translation associated with the matrix. Use
 * {@link Matrix#mapPoints(float[], int, float[], int, int)} if you want the translation
 * to be applied.
 *
 * @param dst   The array of dst vectors (x,y pairs)
 * @param dstIndex The index of the first [x,y] pair of dst floats
 * @param src   The array of src vectors (x,y pairs)
 * @param srcIndex The index of the first [x,y] pair of src floats
 * @param vectorCount The number of vectors (x,y pairs) to transform
 */
ECode Matrix::MapVectors(
    /* [out] */ ArrayOf<Float>* dst,
    /* [in] */ Int32 dstIndex,
    /* [in] */ const ArrayOf<Float>& src,
    /* [in] */ Int32 srcIndex,
    /* [in] */ Int32 vectorCount)
{
    FAIL_RETURN(CheckPointArrays(src, srcIndex, dst, dstIndex, vectorCount));
    NativeMapPoints(mNativeInstance, dst, dstIndex, src, srcIndex,
                    vectorCount, FALSE);
    return NOERROR;
}

/**
 * Apply this matrix to the array of 2D points specified by src, and write
 * the transformed points into the array of points specified by dst. The
 * two arrays represent their "points" as pairs of floats [x, y].
 *
 * @param dst   The array of dst points (x,y pairs)
 * @param src   The array of src points (x,y pairs)
 */
ECode Matrix::MapPoints(
    /* [out] */ ArrayOf<Float>* dst,
    /* [in] */ const ArrayOf<Float>& src)
{
    if (dst == NULL) return E_NULL_POINTER_EXCEPTION;
    if (dst->GetLength() != src.GetLength()) {
//        throw new ArrayIndexOutOfBoundsException();
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    return MapPoints(dst, 0, src, 0, dst->GetLength() >> 1);
}

/**
 * Apply this matrix to the array of 2D vectors specified by src, and write
 * the transformed vectors into the array of vectors specified by dst. The
 * two arrays represent their "vectors" as pairs of floats [x, y].
 *
 * Note: this method does not apply the translation associated with the matrix. Use
 * {@link Matrix#mapPoints(float[], float[])} if you want the translation to be applied.
 *
 * @param dst   The array of dst vectors (x,y pairs)
 * @param src   The array of src vectors (x,y pairs)
 */
ECode Matrix::MapVectors(
    /* [out] */ ArrayOf<Float>* dst,
    /* [in] */ const ArrayOf<Float>& src)
{
    if (dst == NULL) return E_NULL_POINTER_EXCEPTION;
    if (dst->GetLength() != src.GetLength()) {
//        throw new ArrayIndexOutOfBoundsException();
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    return MapVectors(dst, 0, src, 0, dst->GetLength() >> 1);
}

/**
 * Apply this matrix to the array of 2D points, and write the transformed
 * points back into the array
 *
 * @param pts The array [x0, y0, x1, y1, ...] of points to transform.
 */
ECode Matrix::MapPoints(
    /* [in, out] */ ArrayOf<Float>* pts)
{
    return MapPoints(pts, 0, *pts, 0, pts->GetLength() >> 1);
}

/**
 * Apply this matrix to the array of 2D vectors, and write the transformed
 * vectors back into the array.
 *
 * Note: this method does not apply the translation associated with the matrix. Use
 * {@link Matrix#mapPoints(float[])} if you want the translation to be applied.
 *
 * @param vecs The array [x0, y0, x1, y1, ...] of vectors to transform.
 */
ECode Matrix::MapVectors(
    /* [in, out] */ ArrayOf<Float>* vecs)
{
    return MapVectors(vecs, 0, *vecs, 0, vecs->GetLength() >> 1);
}

/**
 * Apply this matrix to the src rectangle, and write the transformed
 * rectangle into dst. This is accomplished by transforming the 4 corners of
 * src, and then setting dst to the bounds of those points.
 *
 * @param dst Where the transformed rectangle is written.
 * @param src The original rectangle to be transformed.
 * @return the result of calling rectStaysRect()
 */
ECode Matrix::MapRect(
    /* [in] */ IRectF* dst,
    /* [in] */ IRectF* src,
    /* [out] */ Boolean* result)
{
    if (dst == NULL || src == NULL) {
//        throw new NullPointerException();
        return E_NULL_POINTER_EXCEPTION;
    }
    *result = NativeMapRect(mNativeInstance, dst, src);
    return NOERROR;
}

/**
 * Apply this matrix to the rectangle, and write the transformed rectangle
 * back into it. This is accomplished by transforming the 4 corners of rect,
 * and then setting it to the bounds of those points
 *
 * @param rect The rectangle to transform.
 * @return the result of calling rectStaysRect()
 */
ECode Matrix::MapRect(
    /* [in] */ IRectF* rect,
    /* [out] */ Boolean* result)
{
    return MapRect(rect, rect, result);
}

/**
 * Return the mean radius of a circle after it has been mapped by
 * this matrix. NOTE: in perspective this value assumes the circle
 * has its center at the origin.
 */
ECode Matrix::MapRadius(
    /* [in] */ Float radius,
    /* [out] */ Float* result)
{
    *result = NativeMapRadius(mNativeInstance, radius);
    return NOERROR;
}

/** Copy 9 values from the matrix into the array.
*/
ECode Matrix::GetValues(
    /* [out] */ ArrayOf<Float>* values)
{
    if (values == NULL || values->GetLength() < 9) {
//        throw new ArrayIndexOutOfBoundsException();
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    NativeGetValues(mNativeInstance, values);
    return NOERROR;
}

/** Copy 9 values from the array into the matrix.
    Depending on the implementation of Matrix, these may be
    transformed into 16.16 integers in the Matrix, such that
    a subsequent call to getValues() will not yield exactly
    the same values.
*/
ECode Matrix::SetValues(
    /* [in] */ const ArrayOf<Float>& values)
{
    if (values.GetLength() < 9) {
//        throw new ArrayIndexOutOfBoundsException();
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    NativeSetValues(mNativeInstance, values);
    return NOERROR;
}

ECode Matrix::ToString(
    /* [out] */ String* str)
{
    StringBuilder sb(64);
    sb.Append(String("Matrix{"));
    ToShortString((IStringBuilder*)&sb);
    sb.AppendChar('}');
    sb.ToString(str);
    return NOERROR;
}


ECode Matrix::ToShortString(
    /* [out] */ String* str)
{
    StringBuilder sb(64);
    ToShortString((IStringBuilder*)&sb);
    sb.ToString(str);
    return NOERROR;
}

/**
 * @hide
 */
ECode Matrix::ToShortString(
    /* [in] */ IStringBuilder* sb)
{
    ArrayOf_<Float, 9> values;
    FAIL_RETURN(GetValues(&values));
    sb->AppendChar('[');
    sb->AppendFloat(values[0]);
    sb->AppendString(String(", "));
    sb->AppendFloat(values[1]);
    sb->AppendString(String(", "));
    sb->AppendFloat(values[2]);
    sb->AppendString(String("]["));
    sb->AppendFloat(values[3]);
    sb->AppendString(String(", "));
    sb->AppendFloat(values[4]);
    sb->AppendString(String(", "));
    sb->AppendFloat(values[5]);
    sb->AppendString(String("]["));
    sb->AppendFloat(values[6]);
    sb->AppendString(String(", "));
    sb->AppendFloat(values[7]);
    sb->AppendString(String(", "));
    sb->AppendFloat(values[8]);
    sb->AppendChar(']');
    return NOERROR;
}

/**
 * Print short string, to optimize dumping.
 * @hide
 */
ECode Matrix::PrintShortString(
     /* [in] */ IPrintWriter* pw)
{
    ArrayOf_<Float, 9> values;
    FAIL_RETURN(GetValues(&values));
    pw->PrintChar('[');
    pw->PrintFloat(values[0]);
    pw->PrintString(String(", "));
    pw->PrintFloat(values[1]);
    pw->PrintString(String(", "));
    pw->PrintFloat(values[2]);
    pw->PrintString(String("]["));
    pw->PrintFloat(values[3]);
    pw->PrintString(String(", "));
    pw->PrintFloat(values[4]);
    pw->PrintString(String(", "));
    pw->PrintFloat(values[5]);
    pw->PrintString(String("]["));
    pw->PrintFloat(values[6]);
    pw->PrintString(String(", "));
    pw->PrintFloat(values[7]);
    pw->PrintString(String(", "));
    pw->PrintFloat(values[8]);
    pw->PrintChar(']');
    return NOERROR;
}

Int32 Matrix::NativeCreate(
    /* [in] */ Int32 nSrc)
{
    SkMatrix* obj = new SkMatrix();
    if (nSrc) {
        *obj = *(SkMatrix*)nSrc;
    }
    else {
        obj->reset();
    }
    return (Int32)obj;
}

Boolean Matrix::NativeIsIdentity(
    /* [in] */ Int32 nObj)
{
    return ((SkMatrix*)nObj)->isIdentity();
}

Boolean Matrix::NativeRectStaysRect(
    /* [in] */ Int32 nObj)
{
    return ((SkMatrix*)nObj)->rectStaysRect();
}

void Matrix::NativeReset(
    /* [in] */ Int32 nObj)
{
    ((SkMatrix*)nObj)->reset();
}

void Matrix::NativeSet(
    /* [in] */ Int32 nObj,
    /* [in] */ Int32 nOther)
{
    *(SkMatrix*)nObj = *(SkMatrix*)nOther;
}

void Matrix::NativeSetTranslate(
    /* [in] */ Int32 nObj,
    /* [in] */ Float dx,
    /* [in] */ Float dy)
{
    SkScalar dx_ = SkFloatToScalar(dx);
    SkScalar dy_ = SkFloatToScalar(dy);
    ((SkMatrix*)nObj)->setTranslate(dx_, dy_);
}

void Matrix::NativeSetScale(
    /* [in] */ Int32 nObj,
    /* [in] */ Float sx,
    /* [in] */ Float sy,
    /* [in] */ Float px,
    /* [in] */ Float py)
{
    SkScalar sx_ = SkFloatToScalar(sx);
    SkScalar sy_ = SkFloatToScalar(sy);
    SkScalar px_ = SkFloatToScalar(px);
    SkScalar py_ = SkFloatToScalar(py);
    ((SkMatrix*)nObj)->setScale(sx_, sy_, px_, py_);
}

void Matrix::NativeSetScale(
    /* [in] */ Int32 nObj,
    /* [in] */ Float sx,
    /* [in] */ Float sy)
{
    SkScalar sx_ = SkFloatToScalar(sx);
    SkScalar sy_ = SkFloatToScalar(sy);
    ((SkMatrix*)nObj)->setScale(sx_, sy_);
}

void Matrix::NativeSetRotate(
    /* [in] */ Int32 nObj,
    /* [in] */ Float degrees,
    /* [in] */ Float px,
    /* [in] */ Float py)
{
    SkScalar degrees_ = SkFloatToScalar(degrees);
    SkScalar px_ = SkFloatToScalar(px);
    SkScalar py_ = SkFloatToScalar(py);
    ((SkMatrix*)nObj)->setRotate(degrees_, px_, py_);
}

void Matrix::NativeSetRotate(
    /* [in] */ Int32 nObj,
    /* [in] */ Float degrees)
{
    SkScalar degrees_ = SkFloatToScalar(degrees);
    ((SkMatrix*)nObj)->setRotate(degrees_);
}

void Matrix::NativeSetSinCos(
    /* [in] */ Int32 nObj,
    /* [in] */ Float sinValue,
    /* [in] */ Float cosValue,
    /* [in] */ Float px,
    /* [in] */ Float py)
{
    SkScalar sinValue_ = SkFloatToScalar(sinValue);
    SkScalar cosValue_ = SkFloatToScalar(cosValue);
    SkScalar px_ = SkFloatToScalar(px);
    SkScalar py_ = SkFloatToScalar(py);
    ((SkMatrix*)nObj)->setSinCos(sinValue_, cosValue_, px_, py_);
}

void Matrix::NativeSetSinCos(
    /* [in] */ Int32 nObj,
    /* [in] */ Float sinValue,
    /* [in] */ Float cosValue)
{
    SkScalar sinValue_ = SkFloatToScalar(sinValue);
    SkScalar cosValue_ = SkFloatToScalar(cosValue);
    ((SkMatrix*)nObj)->setSinCos(sinValue_, cosValue_);
}

void Matrix::NativeSetSkew(
    /* [in] */ Int32 nObj,
    /* [in] */ Float kx,
    /* [in] */ Float ky,
    /* [in] */ Float px,
    /* [in] */ Float py)
{
    SkScalar kx_ = SkFloatToScalar(kx);
    SkScalar ky_ = SkFloatToScalar(ky);
    SkScalar px_ = SkFloatToScalar(px);
    SkScalar py_ = SkFloatToScalar(py);
    ((SkMatrix*)nObj)->setSkew(kx_, ky_, px_, py_);
}

void Matrix::NativeSetSkew(
    /* [in] */ Int32 nObj,
    /* [in] */ Float kx,
    /* [in] */ Float ky)
{
    SkScalar kx_ = SkFloatToScalar(kx);
    SkScalar ky_ = SkFloatToScalar(ky);
    ((SkMatrix*)nObj)->setSkew(kx_, ky_);
}

Boolean Matrix::NativeSetConcat(
    /* [in] */ Int32 nObj,
    /* [in] */ Int32 nA,
    /* [in] */ Int32 nB)
{
    return ((SkMatrix*)nObj)->setConcat(*(SkMatrix*)nA, *(SkMatrix*)nB);
}

Boolean Matrix::NativePreTranslate(
    /* [in] */ Int32 nObj,
    /* [in] */ Float dx,
    /* [in] */ Float dy)
{
    SkScalar dx_ = SkFloatToScalar(dx);
    SkScalar dy_ = SkFloatToScalar(dy);
    return ((SkMatrix*)nObj)->preTranslate(dx_, dy_);
}

Boolean Matrix::NativePreScale(
    /* [in] */ Int32 nObj,
    /* [in] */ Float sx,
    /* [in] */ Float sy,
    /* [in] */ Float px,
    /* [in] */ Float py)
{
    SkScalar sx_ = SkFloatToScalar(sx);
    SkScalar sy_ = SkFloatToScalar(sy);
    SkScalar px_ = SkFloatToScalar(px);
    SkScalar py_ = SkFloatToScalar(py);
    return ((SkMatrix*)nObj)->preScale(sx_, sy_, px_, py_);
}

Boolean Matrix::NativePreScale(
    /* [in] */ Int32 nObj,
    /* [in] */ Float sx,
    /* [in] */ Float sy)
{
    SkScalar sx_ = SkFloatToScalar(sx);
    SkScalar sy_ = SkFloatToScalar(sy);
    return ((SkMatrix*)nObj)->preScale(sx_, sy_);
}

Boolean Matrix::NativePreRotate(
    /* [in] */ Int32 nObj,
    /* [in] */ Float degrees,
    /* [in] */ Float px,
    /* [in] */ Float py)
{
    SkScalar degrees_ = SkFloatToScalar(degrees);
    SkScalar px_ = SkFloatToScalar(px);
    SkScalar py_ = SkFloatToScalar(py);
    return ((SkMatrix*)nObj)->preRotate(degrees_, px_, py_);
}

Boolean Matrix::NativePreRotate(
    /* [in] */ Int32 nObj,
    /* [in] */ Float degrees)
{
    SkScalar degrees_ = SkFloatToScalar(degrees);
    return ((SkMatrix*)nObj)->preRotate(degrees_);
}

Boolean Matrix::NativePreSkew(
    /* [in] */ Int32 nObj,
    /* [in] */ Float kx,
    /* [in] */ Float ky,
    /* [in] */ Float px,
    /* [in] */ Float py)
{
    SkScalar kx_ = SkFloatToScalar(kx);
    SkScalar ky_ = SkFloatToScalar(ky);
    SkScalar px_ = SkFloatToScalar(px);
    SkScalar py_ = SkFloatToScalar(py);
    return ((SkMatrix*)nObj)->preSkew(kx_, ky_, px_, py_);
}

Boolean Matrix::NativePreSkew(
    /* [in] */ Int32 nObj,
    /* [in] */ Float kx,
    /* [in] */ Float ky)
{
    SkScalar kx_ = SkFloatToScalar(kx);
    SkScalar ky_ = SkFloatToScalar(ky);
    return ((SkMatrix*)nObj)->preSkew(kx_, ky_);
}

Boolean Matrix::NativePreConcat(
    /* [in] */ Int32 nObj,
    /* [in] */ Int32 nOther)
{
    return ((SkMatrix*)nObj)->preConcat(*(SkMatrix*)nOther);
}

Boolean Matrix::NativePostTranslate(
    /* [in] */ Int32 nObj,
    /* [in] */ Float dx,
    /* [in] */ Float dy)
{
    SkScalar dx_ = SkFloatToScalar(dx);
    SkScalar dy_ = SkFloatToScalar(dy);
    return ((SkMatrix*)nObj)->postTranslate(dx_, dy_);
}

Boolean Matrix::NativePostScale(
    /* [in] */ Int32 nObj,
    /* [in] */ Float sx,
    /* [in] */ Float sy,
    /* [in] */ Float px,
    /* [in] */ Float py)
{
    SkScalar sx_ = SkFloatToScalar(sx);
    SkScalar sy_ = SkFloatToScalar(sy);
    SkScalar px_ = SkFloatToScalar(px);
    SkScalar py_ = SkFloatToScalar(py);
    return ((SkMatrix*)nObj)->postScale(sx_, sy_, px_, py_);
}

Boolean Matrix::NativePostScale(
    /* [in] */ Int32 nObj,
    /* [in] */ Float sx,
    /* [in] */ Float sy)
{
    SkScalar sx_ = SkFloatToScalar(sx);
    SkScalar sy_ = SkFloatToScalar(sy);
    return ((SkMatrix*)nObj)->postScale(sx_, sy_);
}

Boolean Matrix::NativePostRotate(
    /* [in] */ Int32 nObj,
    /* [in] */ Float degrees,
    /* [in] */ Float px,
    /* [in] */ Float py)
{
    SkScalar degrees_ = SkFloatToScalar(degrees);
    SkScalar px_ = SkFloatToScalar(px);
    SkScalar py_ = SkFloatToScalar(py);
    return ((SkMatrix*)nObj)->postRotate(degrees_, px_, py_);
}

Boolean Matrix::NativePostRotate(
    /* [in] */ Int32 nObj,
    /* [in] */ Float degrees)
{
    SkScalar degrees_ = SkFloatToScalar(degrees);
    return ((SkMatrix*)nObj)->postRotate(degrees_);
}

Boolean Matrix::NativePostSkew(
    /* [in] */ Int32 nObj,
    /* [in] */ Float kx,
    /* [in] */ Float ky,
    /* [in] */ Float px,
    /* [in] */ Float py)
{
    SkScalar kx_ = SkFloatToScalar(kx);
    SkScalar ky_ = SkFloatToScalar(ky);
    SkScalar px_ = SkFloatToScalar(px);
    SkScalar py_ = SkFloatToScalar(py);
    return ((SkMatrix*)nObj)->postSkew(kx_, ky_, px_, py_);
}

Boolean Matrix::NativePostSkew(
    /* [in] */ Int32 nObj,
    /* [in] */ Float kx,
    /* [in] */ Float ky)
{
    SkScalar kx_ = SkFloatToScalar(kx);
    SkScalar ky_ = SkFloatToScalar(ky);
    return ((SkMatrix*)nObj)->postSkew(kx_, ky_);
}

Boolean Matrix::NativePostConcat(
    /* [in] */ Int32 nObj,
    /* [in] */ Int32 nOther)
{
    return ((SkMatrix*)nObj)->postConcat(*(SkMatrix*)nOther);
}

Boolean Matrix::NativeSetRectToRect(
    /* [in] */ Int32 nObj,
    /* [in] */ IRectF* src,
    /* [in] */ IRectF* dst,
    /* [in] */ Int32 stf)
{
    SkRect src_;
    GraphicsNative::IRectF2SkRect(src, &src_);
    SkRect dst_;
    GraphicsNative::IRectF2SkRect(dst, &dst_);
    return ((SkMatrix*)nObj)->setRectToRect(src_, dst_, (SkMatrix::ScaleToFit)stf);
}

Boolean Matrix::NativeSetPolyToPoly(
    /* [in] */ Int32 nObj,
    /* [in] */ const ArrayOf<Float>& src,
    /* [in] */ Int32 srcIndex,
    /* [out] */ ArrayOf<Float>* dst,
    /* [in] */ Int32 dstIndex,
    /* [in] */ Int32 ptCount)
{
    SkASSERT(srcIndex >= 0);
    SkASSERT(dstIndex >= 0);
    SkASSERT(ptCount <= 4);

    SkASSERT(src.GetLength() >= srcIndex + (ptCount << 1));
    SkASSERT(dst->GetLength() >= dstIndex + (ptCount << 1));

    float* src_ = src.GetPayload() + srcIndex;
    float* dst_ = dst->GetPayload() + dstIndex;

#ifdef SK_SCALAR_IS_FIXED
    SkPoint srcPt[4], dstPt[4];
    for (Int32 i = 0; i < ptCount; i++) {
        Int32 x = i << 1;
        Int32 y = x + 1;
        srcPt[i].set(SkFloatToScalar(src_[x]), SkFloatToScalar(src_[y]));
        dstPt[i].set(SkFloatToScalar(dst_[x]), SkFloatToScalar(dst_[y]));
    }
    return ((SkMatrix*)nObj)->setPolyToPoly(srcPt, dstPt, ptCount);
#else
    return ((SkMatrix*)nObj)->setPolyToPoly((const SkPoint*)src_, (const SkPoint*)dst_,
                               ptCount);
#endif
}

Boolean Matrix::NativeInvert(
    /* [in] */ Int32 nObj,
    /* [in] */ Int32 inverse)
{
    return ((SkMatrix*)nObj)->invert((SkMatrix*)inverse);
}

void Matrix::NativeMapPoints(
    /* [in] */ Int32 nObj,
    /* [out] */ ArrayOf<Float>* dst,
    /* [in] */ Int32 dstIndex,
    /* [in] */ const ArrayOf<Float>& src,
    /* [in] */ Int32 srcIndex,
    /* [in] */ Int32 ptCount,
    /* [in] */ Boolean isPts)
{
    SkASSERT(ptCount >= 0);
    SkASSERT(src.GetLength() >= srcIndex + (ptCount << 1));
    SkASSERT(dst->GetLength() >= dstIndex + (ptCount << 1));
    float* srcArray = src.GetPayload() + srcIndex;
    float* dstArray = dst->GetPayload() + dstIndex;

#ifdef SK_SCALAR_IS_FIXED
    // we allocate twice the count, 1 set for src, 1 for dst
    SkAutoSTMalloc<32, SkPoint> storage(ptCount * 2);
    SkPoint* pts = storage.get();
    SkPoint* srcPt = pts;
    SkPoint* dstPt = pts + ptCount;

    Int32 i;
    for (i = 0; i < ptCount; i++) {
        srcPt[i].set(SkFloatToScalar(srcArray[i << 1]),
                     SkFloatToScalar(srcArray[(i << 1) + 1]));
    }

    if (isPts) {
        ((SkMatrix*)nObj)->mapPoints(dstPt, srcPt, ptCount);
    }
    else {
        ((SkMatrix*)nObj)->mapVectors(dstPt, srcPt, ptCount);
    }

    for (i = 0; i < ptCount; i++) {
        dstArray[i << 1]  = SkScalarToFloat(dstPt[i].fX);
        dstArray[(i << 1) + 1]  = SkScalarToFloat(dstPt[i].fY);
    }
#else
    if (isPts) {
        ((SkMatrix*)nObj)->mapPoints((SkPoint*)dstArray, (const SkPoint*)srcArray,
                        ptCount);
    }
    else {
        ((SkMatrix*)nObj)->mapVectors((SkVector*)dstArray, (const SkVector*)srcArray,
                         ptCount);
    }
#endif
}

Boolean Matrix::NativeMapRect(
    /* [in] */ Int32 nObj,
    /* [in] */ IRectF* dst,
    /* [in] */ IRectF* src)
{
    SkRect dst_, src_;
    GraphicsNative::IRectF2SkRect(src, &src_);
    Boolean rectStaysRect = ((SkMatrix*)nObj)->mapRect(&dst_, src_);
    GraphicsNative::SkRect2IRectF(dst_, dst);
    return rectStaysRect;
}

Float Matrix::NativeMapRadius(
    /* [in] */ Int32 nObj,
    /* [in] */ Float radius)
{
    return SkScalarToFloat(((SkMatrix*)nObj)->mapRadius(SkFloatToScalar(radius)));
}

void Matrix::NativeGetValues(
    /* [in] */ Int32 nObj,
    /* [out] */ ArrayOf<Float>* values)
{
    SkASSERT(values->GetLength() >= 9);
    float* dst = values->GetPayload();

#ifdef SK_SCALAR_IS_FIXED
    for (Int32 i = 0; i < 6; i++) {
        dst[i] = SkFixedToFloat(((SkMatrix*)nObj)->get(i));
    }
    for (Int32 j = 6; j < 9; j++) {
        dst[j] = SkFractToFloat(((SkMatrix*)nObj)->get(j));
    }
#else
    for (Int32 i = 0; i < 9; i++) {
        dst[i] = ((SkMatrix*)nObj)->get(i);
    }
#endif
}

void Matrix::NativeSetValues(
    /* [in] */ Int32 nObj,
    /* [in] */ const ArrayOf<Float>& values)
{
    SkASSERT(values.GetLength() >= 9);
    const float* src = values.GetPayload();

#ifdef SK_SCALAR_IS_FIXED
    for (Int32 i = 0; i < 6; i++) {
        ((SkMatrix*)nObj)->set(i, SkFloatToFixed(src[i]));
    }
    for (Int32 j = 6; j < 9; j++) {
        ((SkMatrix*)nObj)->set(j, SkFloatToFract(src[j]));
    }
#else
    for (Int32 i = 0; i < 9; i++) {
        ((SkMatrix*)nObj)->set(i, src[i]);
    }
#endif
}

Boolean Matrix::NativeEquals(
    /* [in] */ Int32 nA,
    /* [in] */ Int32 nB)
{
    return *(SkMatrix*)nA == *(SkMatrix*)nB;
}

void Matrix::NativeFinalizer(
    /* [in] */ Int32 nObj)
{
    delete (SkMatrix*)nObj;
}

Int32 Matrix::Ni()
{
    return mNativeInstance;
}


} // namespace Graphics
} // namepsace Droid
} // namespace Elastos
