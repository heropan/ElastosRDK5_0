
#include "ext/frameworkext.h"
#include "graphics/CMatrix.h"
#include <elastos/StringBuilder.h>

using Elastos::Core::StringBuilder;

namespace Elastos {
namespace Droid {
namespace Graphics {

CMatrix::IdentityMatrix::IdentityMatrix()
{
    Matrix::Init();
}

CMatrix::IdentityMatrix::IdentityMatrix(
    /* [in] */ IMatrix* src)
{
    Matrix::Init(src);
}

PInterface CMatrix::IdentityMatrix::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)this;
    }
    else if (riid == EIID_IMatrix) {
        return (IMatrix*)this;
    }
    else if (riid == EIID_Matrix) {
        return reinterpret_cast<PInterface>((Matrix*)this);
    }
    return NULL;
}

UInt32 CMatrix::IdentityMatrix::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CMatrix::IdentityMatrix::Release()
{
    return ElRefBase::Release();
}

ECode CMatrix::IdentityMatrix::GetInterfaceID(
    /* [in] */ IInterface* object,
    /* [out] */ InterfaceID* iid)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CMatrix::IdentityMatrix::Oops()
{
    // throw new IllegalStateException("Matrix can not be modified");
    return E_ILLEGAL_STATE_EXCEPTION;
}

ECode CMatrix::IdentityMatrix::IsIdentity(
    /* [out] */ Boolean* isIdentity)
{
    VALIDATE_NOT_NULL(isIdentity);
    return Matrix::IsIdentity(isIdentity);
}

ECode CMatrix::IdentityMatrix::RectStaysRect(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return Matrix::RectStaysRect(result);
}

ECode CMatrix::IdentityMatrix::Set(
    /* [in] */ IMatrix* src)
{
    return Oops();
}

ECode CMatrix::IdentityMatrix::EqualsEx(
    /* [in] */ IMatrix* obj,
    /* [out] */ Boolean* isEqual)
{
    VALIDATE_NOT_NULL(isEqual);
    return Matrix::Equals(obj, isEqual);
}

ECode CMatrix::IdentityMatrix::Equals(
    /* [in] */ IInterface* obj,
    /* [out] */ Boolean* isEqual)
{
    VALIDATE_NOT_NULL(isEqual);
    return Matrix::Equals(IMatrix::Probe(obj), isEqual);
}

ECode CMatrix::IdentityMatrix::GetHashCode(
    /* [out] */ Int32* hash)
{
    VALIDATE_NOT_NULL(hash);
    return Matrix::GetHashCode(hash);
}

ECode CMatrix::IdentityMatrix::Reset()
{
    return Oops();
}

ECode CMatrix::IdentityMatrix::SetTranslate(
    /* [in] */ Float dx,
    /* [in] */ Float dy)
{
    return Oops();
}

ECode CMatrix::IdentityMatrix::SetScale(
    /* [in] */ Float sx,
    /* [in] */ Float sy,
    /* [in] */ Float px,
    /* [in] */ Float py)
{
    return Oops();
}

ECode CMatrix::IdentityMatrix::SetScaleEx(
    /* [in] */ Float sx,
    /* [in] */ Float sy)
{
    return Oops();
}

ECode CMatrix::IdentityMatrix::SetRotate(
    /* [in] */ Float degrees,
    /* [in] */ Float px,
    /* [in] */ Float py)
{
    return Oops();
}

ECode CMatrix::IdentityMatrix::SetRotateEx(
    /* [in] */ Float degrees)
{
    return Oops();
}

ECode CMatrix::IdentityMatrix::SetSinCos(
    /* [in] */ Float sinValue,
    /* [in] */ Float cosValue,
    /* [in] */ Float px,
    /* [in] */ Float py)
{
    return Oops();
}

ECode CMatrix::IdentityMatrix::SetSinCosEx(
    /* [in] */ Float sinValue,
    /* [in] */ Float cosValue)
{
    return Oops();
}

ECode CMatrix::IdentityMatrix::SetSkew(
    /* [in] */ Float kx,
    /* [in] */ Float ky,
    /* [in] */ Float px,
    /* [in] */ Float py)
{
    return Oops();
}

ECode CMatrix::IdentityMatrix::SetSkewEx(
    /* [in] */ Float kx,
    /* [in] */ Float ky)
{
    return Oops();
}

ECode CMatrix::IdentityMatrix::SetConcat(
    /* [in] */ IMatrix* a,
    /* [in] */ IMatrix* b,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    return Oops();
}

ECode CMatrix::IdentityMatrix::PreTranslate(
    /* [in] */ Float dx,
    /* [in] */ Float dy,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    return Oops();
}

ECode CMatrix::IdentityMatrix::PreScale(
    /* [in] */ Float sx,
    /* [in] */ Float sy,
    /* [in] */ Float px,
    /* [in] */ Float py,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    return Oops();
}

ECode CMatrix::IdentityMatrix::PreScaleEx(
    /* [in] */ Float sx,
    /* [in] */ Float sy,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    return Oops();
}

ECode CMatrix::IdentityMatrix::PreRotate(
    /* [in] */ Float degrees,
    /* [in] */ Float px,
    /* [in] */ Float py,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    return Oops();
}

ECode CMatrix::IdentityMatrix::PreRotateEx(
    /* [in] */ Float degrees,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    return Oops();
}

ECode CMatrix::IdentityMatrix::PreSkew(
    /* [in] */ Float kx,
    /* [in] */ Float ky,
    /* [in] */ Float px,
    /* [in] */ Float py,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    return Oops();
}

ECode CMatrix::IdentityMatrix::PreSkewEx(
    /* [in] */ Float kx,
    /* [in] */ Float ky,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    return Oops();
}

ECode CMatrix::IdentityMatrix::PreConcat(
    /* [in] */ IMatrix* other,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    return Oops();
}

ECode CMatrix::IdentityMatrix::PostTranslate(
    /* [in] */ Float dx,
    /* [in] */ Float dy,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    return Oops();
}

ECode CMatrix::IdentityMatrix::PostScale(
    /* [in] */ Float sx,
    /* [in] */ Float sy,
    /* [in] */ Float px,
    /* [in] */ Float py,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    return Oops();
}

ECode CMatrix::IdentityMatrix::PostScaleEx(
    /* [in] */ Float sx,
    /* [in] */ Float sy,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    return Oops();
}

ECode CMatrix::IdentityMatrix::PostRotate(
    /* [in] */ Float degrees,
    /* [in] */ Float px,
    /* [in] */ Float py,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    return Oops();
}

ECode CMatrix::IdentityMatrix::PostRotateEx(
    /* [in] */ Float degrees,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    return Oops();
}

ECode CMatrix::IdentityMatrix::PostSkew(
    /* [in] */ Float kx,
    /* [in] */ Float ky,
    /* [in] */ Float px,
    /* [in] */ Float py,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    return Oops();
}

ECode CMatrix::IdentityMatrix::PostSkewEx(
    /* [in] */ Float kx,
    /* [in] */ Float ky,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    return Oops();
}

ECode CMatrix::IdentityMatrix::PostConcat(
    /* [in] */ IMatrix* other,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    return Oops();
}

ECode CMatrix::IdentityMatrix::SetRectToRect(
    /* [in] */ IRectF* src,
    /* [in] */ IRectF* dst,
    /* [in] */ MatrixScaleToFit stf,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    return Oops();
}

ECode CMatrix::IdentityMatrix::SetPolyToPoly(
    /* [in] */ const ArrayOf<Float>& src,
    /* [in] */ Int32 srcIndex,
    /* [out] */ ArrayOf<Float>* dst,
    /* [in] */ Int32 dstIndex,
    /* [in] */ Int32 pointCount,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    return Oops();
}

ECode CMatrix::IdentityMatrix::Invert(
    /* [in] */ IMatrix* inverse,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return Matrix::Invert(inverse, result);
}

ECode CMatrix::IdentityMatrix::MapPoints(
    /* [out] */ ArrayOf<Float>* dst,
    /* [in] */ Int32 dstIndex,
    /* [in] */ const ArrayOf<Float>& src,
    /* [in] */ Int32 srcIndex,
    /* [in] */ Int32 pointCount)
{
    return Matrix::MapPoints(dst, dstIndex, src, srcIndex, pointCount);
}

ECode CMatrix::IdentityMatrix::MapVectors(
    /* [out] */ ArrayOf<Float>* dst,
    /* [in] */ Int32 dstIndex,
    /* [in] */ const ArrayOf<Float>& src,
    /* [in] */ Int32 srcIndex,
    /* [in] */ Int32 vectorCount)
{
    return Matrix::MapVectors(dst, dstIndex, src, srcIndex, vectorCount);
}

ECode CMatrix::IdentityMatrix::MapPointsEx(
    /* [out] */ ArrayOf<Float>* dst,
    /* [in] */ const ArrayOf<Float>& src)
{
    return Matrix::MapPointsEx(dst, src);
}

ECode CMatrix::IdentityMatrix::MapVectorsEx(
    /* [out] */ ArrayOf<Float>* dst,
    /* [in] */ const ArrayOf<Float>& src)
{
    return Matrix::MapVectorsEx(dst, src);
}

ECode CMatrix::IdentityMatrix::MapPointsEx2(
    /* [in, out] */ ArrayOf<Float>* pts)
{
    return Matrix::MapPointsEx2(pts);
}

ECode CMatrix::IdentityMatrix::MapVectorsEx2(
    /* [in, out] */ ArrayOf<Float>* vecs)
{
    return Matrix::MapVectorsEx2(vecs);
}

ECode CMatrix::IdentityMatrix::MapRect(
    /* [in] */ IRectF* dst,
    /* [in] */ IRectF* src,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return Matrix::MapRect(dst, src, result);
}

ECode CMatrix::IdentityMatrix::MapRectEx(
    /* [in] */ IRectF* rect,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return Matrix::MapRectEx(rect, result);
}

ECode CMatrix::IdentityMatrix::MapRadius(
    /* [in] */ Float radius,
    /* [out] */ Float* result)
{
    VALIDATE_NOT_NULL(result);
    return Matrix::MapRadius(radius, result);
}

ECode CMatrix::IdentityMatrix::GetValues(
    /* [out] */ ArrayOf<Float>* values)
{
    return Matrix::GetValues(values);
}

ECode CMatrix::IdentityMatrix::SetValues(
    /* [in] */ const ArrayOf<Float>& values)
{
    return Oops();
}

ECode CMatrix::IdentityMatrix::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    return Matrix::ToString(str);
}

ECode CMatrix::IdentityMatrix::PrintShortString(
    /* [in] */ IPrintWriter* pw)
{
    return Matrix::PrintShortString(pw);
}

ECode CMatrix::IdentityMatrix::ToShortString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    return Matrix::ToShortString(str);
}

ECode CMatrix::IdentityMatrix::ToShortStringEx(
    /* [in] */ IStringBuilder* sb)
{
    return Matrix::ToShortStringEx(sb);
}


AutoPtr<IMatrix> CMatrix::IDENTITY_MATRIX = new IdentityMatrix();

ECode CMatrix::constructor()
{
    return Matrix::Init();
}

ECode CMatrix::constructor(
    /* [in] */ IMatrix* src)
{
    return Matrix::Init(src);
}

PInterface CMatrix::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_Matrix) {
        return reinterpret_cast<PInterface>((Matrix*)this);
    }
    return _CMatrix::Probe(riid);
}

/**
 * Returns true if the matrix is identity.
 * This maybe faster than testing if (getType() == 0)
 */
ECode CMatrix::IsIdentity(
    /* [out] */ Boolean* isIdentity)
{
    VALIDATE_NOT_NULL(isIdentity);
    return Matrix::IsIdentity(isIdentity);
}

/* Returns true if will map a rectangle to another rectangle. This can be
 * true if the matrix is identity, scale-only, or rotates a multiple of 90
 * degrees.
 */
ECode CMatrix::RectStaysRect(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return Matrix::RectStaysRect(result);
}

/**
 * (deep) copy the src matrix into this matrix. If src is null, reset this
 * matrix to the identity matrix.
 */
ECode CMatrix::Set(
    /* [in] */ IMatrix* src)
{
    return Matrix::Set(src);
}

/** Set the matrix to identity */
ECode CMatrix::Reset()
{
    return Matrix::Reset();
}

/** Set the matrix to translate by (dx, dy). */
ECode CMatrix::SetTranslate(
    /* [in] */ Float dx,
    /* [in] */ Float dy)
{
    return Matrix::SetTranslate(dx, dy);
}

/**
 * Set the matrix to scale by sx and sy, with a pivot point at (px, py).
 * The pivot point is the coordinate that should remain unchanged by the
 * specified transformation.
 */
ECode CMatrix::SetScale(
    /* [in] */ Float sx,
    /* [in] */ Float sy,
    /* [in] */ Float px,
    /* [in] */ Float py)
{
    return Matrix::SetScale(sx,sy,px,py);
}

/** Set the matrix to scale by sx and sy. */
ECode CMatrix::SetScaleEx(
    /* [in] */ Float sx,
    /* [in] */ Float sy)
{
    return Matrix::SetScaleEx(sx,sy);
}

/**
 * Set the matrix to rotate by the specified number of degrees, with a pivot
 * point at (px, py). The pivot point is the coordinate that should remain
 * unchanged by the specified transformation.
 */
ECode CMatrix::SetRotate(
    /* [in] */ Float degrees,
    /* [in] */ Float px,
    /* [in] */ Float py)
{
    return Matrix::SetRotate(degrees,px,py);
}

/**
 * Set the matrix to rotate about (0,0) by the specified number of degrees.
 */
ECode CMatrix::SetRotateEx(
    /* [in] */ Float degrees)
{
    return Matrix::SetRotateEx(degrees);
}

/**
 * Set the matrix to rotate by the specified sine and cosine values, with a
 * pivot point at (px, py). The pivot point is the coordinate that should
 * remain unchanged by the specified transformation.
 */
ECode CMatrix::SetSinCos(
    /* [in] */ Float sinValue,
    /* [in] */ Float cosValue,
    /* [in] */ Float px,
    /* [in] */ Float py)
{
    return Matrix::SetSinCos(sinValue,cosValue,px,py);
}

/** Set the matrix to rotate by the specified sine and cosine values. */
ECode CMatrix::SetSinCosEx(
    /* [in] */ Float sinValue,
    /* [in] */ Float cosValue)
{
    return Matrix::SetSinCosEx(sinValue,cosValue);
}

/**
 * Set the matrix to skew by sx and sy, with a pivot point at (px, py).
 * The pivot point is the coordinate that should remain unchanged by the
 * specified transformation.
 */
ECode CMatrix::SetSkew(
    /* [in] */ Float kx,
    /* [in] */ Float ky,
    /* [in] */ Float px,
    /* [in] */ Float py)
{
    return Matrix::SetSkew(kx,ky,px,py);
}

/** Set the matrix to skew by sx and sy. */
ECode CMatrix::SetSkewEx(
    /* [in] */ Float kx,
    /* [in] */ Float ky)
{
    return Matrix::SetSkewEx(kx,ky);
}

/**
 * Set the matrix to the concatenation of the two specified matrices,
 * returning true if the the result can be represented. Either of the two
 * matrices may also be the target matrix. this = a * b
 */
ECode CMatrix::SetConcat(
    /* [in] */ IMatrix* a,
    /* [in] */ IMatrix* b,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return Matrix::SetConcat(a,b,result);
}

/**
 * Preconcats the matrix with the specified translation.
 * M' = M * T(dx, dy)
 */
ECode CMatrix::PreTranslate(
    /* [in] */ Float dx,
    /* [in] */ Float dy,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return Matrix::PreTranslate(dx,dy,result);
}

/**
 * Preconcats the matrix with the specified scale.
 * M' = M * S(sx, sy, px, py)
 */
ECode CMatrix::PreScale(
    /* [in] */ Float sx,
    /* [in] */ Float sy,
    /* [in] */ Float px,
    /* [in] */ Float py,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return Matrix::PreScale(sx,sy,px,py,result);
}

/**
 * Preconcats the matrix with the specified scale.
 * M' = M * S(sx, sy)
 */
ECode CMatrix::PreScaleEx(
    /* [in] */ Float sx,
    /* [in] */ Float sy,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return Matrix::PreScaleEx(sx,sy,result);
}

/**
 * Preconcats the matrix with the specified rotation.
 * M' = M * R(degrees, px, py)
 */
ECode CMatrix::PreRotate(
    /* [in] */ Float degrees,
    /* [in] */ Float px,
    /* [in] */ Float py,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return Matrix::PreRotate(degrees,px,py,result);
}

/**
 * Preconcats the matrix with the specified rotation.
 * M' = M * R(degrees)
 */
ECode CMatrix::PreRotateEx(
    /* [in] */ Float degrees,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return Matrix::PreRotateEx(degrees,result);
}

/**
 * Preconcats the matrix with the specified skew.
 * M' = M * K(kx, ky, px, py)
 */
ECode CMatrix::PreSkew(
    /* [in] */ Float kx,
    /* [in] */ Float ky,
    /* [in] */ Float px,
    /* [in] */ Float py,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return Matrix::PreSkew(kx,ky,px,py,result);
}

/**
 * Preconcats the matrix with the specified skew.
 * M' = M * K(kx, ky)
 */
ECode CMatrix::PreSkewEx(
    /* [in] */ Float kx,
    /* [in] */ Float ky,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return Matrix::PreSkewEx(kx,ky,result);
}

/**
 * Preconcats the matrix with the specified matrix.
 * M' = M * other
 */
ECode CMatrix::PreConcat(
    /* [in] */ IMatrix* other,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return Matrix::PreConcat(other,result);
}

/**
 * Postconcats the matrix with the specified translation.
 * M' = T(dx, dy) * M
 */
ECode CMatrix::PostTranslate(
    /* [in] */ Float dx,
    /* [in] */ Float dy,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return Matrix::PostTranslate(dx,dy,result);
}

/**
 * Postconcats the matrix with the specified scale.
 * M' = S(sx, sy, px, py) * M
 */
ECode CMatrix::PostScale(
    /* [in] */ Float sx,
    /* [in] */ Float sy,
    /* [in] */ Float px,
    /* [in] */ Float py,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return Matrix::PostScale(sx,sy,px,py,result);
}

/**
 * Postconcats the matrix with the specified scale.
 * M' = S(sx, sy) * M
 */
ECode CMatrix::PostScaleEx(
    /* [in] */ Float sx,
    /* [in] */ Float sy,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return Matrix::PostScaleEx(sx,sy,result);
}

/**
 * Postconcats the matrix with the specified rotation.
 * M' = R(degrees, px, py) * M
 */
ECode CMatrix::PostRotate(
    /* [in] */ Float degrees,
    /* [in] */ Float px,
    /* [in] */ Float py,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return Matrix::PostRotate(degrees,px,py,result);
}

/**
 * Postconcats the matrix with the specified rotation.
 * M' = R(degrees) * M
 */
ECode CMatrix::PostRotateEx(
    /* [in] */ Float degrees,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return Matrix::PostRotateEx(degrees,result);
}

/**
 * Postconcats the matrix with the specified skew.
 * M' = K(kx, ky, px, py) * M
 */
ECode CMatrix::PostSkew(
    /* [in] */ Float kx,
    /* [in] */ Float ky,
    /* [in] */ Float px,
    /* [in] */ Float py,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return Matrix::PostSkew(kx,ky,px,py,result);
}

/**
 * Postconcats the matrix with the specified skew.
 * M' = K(kx, ky) * M
 */
ECode CMatrix::PostSkewEx(
    /* [in] */ Float kx,
    /* [in] */ Float ky,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return Matrix::PostSkewEx(kx,ky,result);
}

/**
 * Postconcats the matrix with the specified matrix.
 * M' = other * M
 */
ECode CMatrix::PostConcat(
    /* [in] */ IMatrix* other,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return Matrix::PostConcat(other,result);
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
ECode CMatrix::SetRectToRect(
    /* [in] */ IRectF* src,
    /* [in] */ IRectF* dst,
    /* [in] */ MatrixScaleToFit stf,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return Matrix::SetRectToRect(src,dst,stf,result);
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
ECode CMatrix::SetPolyToPoly(
    /* [in] */ const ArrayOf<Float>& src,
    /* [in] */ Int32 srcIndex,
    /* [out] */ ArrayOf<Float>* dst,
    /* [in] */ Int32 dstIndex,
    /* [in] */ Int32 pointCount,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return Matrix::SetPolyToPoly(src,srcIndex,dst,dstIndex,pointCount,result);
}

/**
 * If this matrix can be inverted, return true and if inverse is not null,
 * set inverse to be the inverse of this matrix. If this matrix cannot be
 * inverted, ignore inverse and return false.
 */
ECode CMatrix::Invert(
    /* [in] */ IMatrix* inverse,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return Matrix::Invert(inverse, result);
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
ECode CMatrix::MapPoints(
    /* [out] */ ArrayOf<Float>* dst,
    /* [in] */ Int32 dstIndex,
    /* [in] */ const ArrayOf<Float>& src,
    /* [in] */ Int32 srcIndex,
    /* [in] */ Int32 pointCount)
{
    return Matrix::MapPoints(dst, dstIndex, src, srcIndex, pointCount);
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
ECode CMatrix::MapVectors(
    /* [out] */ ArrayOf<Float>* dst,
    /* [in] */ Int32 dstIndex,
    /* [in] */ const ArrayOf<Float>& src,
    /* [in] */ Int32 srcIndex,
    /* [in] */ Int32 vectorCount)
{
    return Matrix::MapVectors(dst, dstIndex, src, srcIndex, vectorCount);
}

/**
 * Apply this matrix to the array of 2D points specified by src, and write
 * the transformed points into the array of points specified by dst. The
 * two arrays represent their "points" as pairs of floats [x, y].
 *
 * @param dst   The array of dst points (x,y pairs)
 * @param src   The array of src points (x,y pairs)
 */
ECode CMatrix::MapPointsEx(
    /* [out] */ ArrayOf<Float>* dst,
    /* [in] */ const ArrayOf<Float>& src)
{
    return Matrix::MapPointsEx(dst, src);
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
ECode CMatrix::MapVectorsEx(
    /* [out] */ ArrayOf<Float>* dst,
    /* [in] */ const ArrayOf<Float>& src)
{
    return Matrix::MapVectorsEx(dst, src);
}

/**
 * Apply this matrix to the array of 2D points, and write the transformed
 * points back into the array
 *
 * @param pts The array [x0, y0, x1, y1, ...] of points to transform.
 */
ECode CMatrix::MapPointsEx2(
    /* [in, out] */ ArrayOf<Float>* pts)
{
    return Matrix::MapPointsEx2(pts);
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
ECode CMatrix::MapVectorsEx2(
    /* [in, out] */ ArrayOf<Float>* vecs)
{
    return Matrix::MapVectorsEx2(vecs);
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
ECode CMatrix::MapRect(
    /* [in] */ IRectF* dst,
    /* [in] */ IRectF* src,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return Matrix::MapRect(dst, src, result);
}

/**
 * Apply this matrix to the rectangle, and write the transformed rectangle
 * back into it. This is accomplished by transforming the 4 corners of rect,
 * and then setting it to the bounds of those points
 *
 * @param rect The rectangle to transform.
 * @return the result of calling rectStaysRect()
 */
ECode CMatrix::MapRectEx(
    /* [in] */ IRectF* rect,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return Matrix::MapRectEx(rect, result);
}

/**
 * Return the mean radius of a circle after it has been mapped by
 * this matrix. NOTE: in perspective this value assumes the circle
 * has its center at the origin.
 */
ECode CMatrix::MapRadius(
    /* [in] */ Float radius,
    /* [out] */ Float* result)
{
    VALIDATE_NOT_NULL(result);
    return Matrix::MapRadius(radius, result);
}

/** Copy 9 values from the matrix into the array.
*/
ECode CMatrix::GetValues(
    /* [out] */ ArrayOf<Float>* values)
{
    return Matrix::GetValues(values);
}

/** Copy 9 values from the array into the matrix.
    Depending on the implementation of Matrix, these may be
    transformed into 16.16 integers in the Matrix, such that
    a subsequent call to getValues() will not yield exactly
    the same values.
*/
ECode CMatrix::SetValues(
    /* [in] */ const ArrayOf<Float>& values)
{
    return Matrix::SetValues(values);
}

/** Returns true iff obj is a Matrix and its values equal our values.
 */
ECode CMatrix::EqualsEx(
    /* [in] */ IMatrix* obj,
    /* [out] */ Boolean* isEqual)
{
    VALIDATE_NOT_NULL(isEqual);
    return Matrix::Equals(obj, isEqual);
}

ECode CMatrix::Equals(
    /* [in] */ IInterface* obj,
    /* [out] */ Boolean* isEqual)
{
    VALIDATE_NOT_NULL(isEqual);
    return Matrix::Equals(IMatrix::Probe(obj), isEqual);
}

ECode CMatrix::GetHashCode(
    /* [out] */ Int32* hash)
{
    VALIDATE_NOT_NULL(hash);
    return Matrix::GetHashCode(hash);
}

ECode CMatrix::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    return Matrix::ToString(str);
}

ECode CMatrix::ToShortString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    return Matrix::ToShortString(str);
}

/**
 * @hide
 */
ECode CMatrix::ToShortStringEx(
    /* [in] */ IStringBuilder* sb)
{
    return Matrix::ToShortStringEx(sb);
}

/**
 * Print short string, to optimize dumping.
 * @hide
 */
ECode CMatrix::PrintShortString(
     /* [in] */ IPrintWriter* pw)
{
    return Matrix::PrintShortString(pw);
}

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos
