
#ifndef __ELASTOS_DROID_GRAPHICS_CMATRIX_H__
#define __ELASTOS_DROID_GRAPHICS_CMATRIX_H__

#include "_Elastos_Droid_Graphics_CMatrix.h"
#include "Matrix.h"

namespace Elastos {
namespace Droid {
namespace Graphics {

CarClass(CMatrix), public Matrix
{
private:
    class IdentityMatrix
        : public ElRefBase
        , public IMatrix
        , public Matrix
    {
    public:
        IdentityMatrix();

        IdentityMatrix(
            /* [in] */ IMatrix* src);

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface* object,
            /* [out] */ InterfaceID* iid);

        CARAPI Oops();

        CARAPI IsIdentity(
            /* [out] */ Boolean* isIdentity);

        CARAPI RectStaysRect(
            /* [out] */ Boolean* result);

        CARAPI Set(
            /* [in] */ IMatrix* src);

        CARAPI Reset();

        CARAPI SetTranslate(
            /* [in] */ Float dx,
            /* [in] */ Float dy);

        CARAPI SetScale(
            /* [in] */ Float sx,
            /* [in] */ Float sy,
            /* [in] */ Float px,
            /* [in] */ Float py);

        CARAPI SetScale(
            /* [in] */ Float sx,
            /* [in] */ Float sy);

        CARAPI SetRotate(
            /* [in] */ Float degrees,
            /* [in] */ Float px,
            /* [in] */ Float py);

        CARAPI SetRotate(
            /* [in] */ Float degrees);

        CARAPI SetSinCos(
            /* [in] */ Float sinValue,
            /* [in] */ Float cosValue,
            /* [in] */ Float px,
            /* [in] */ Float py);

        CARAPI SetSinCos(
            /* [in] */ Float sinValue,
            /* [in] */ Float cosValue);

        CARAPI SetSkew(
            /* [in] */ Float kx,
            /* [in] */ Float ky,
            /* [in] */ Float px,
            /* [in] */ Float py);

        CARAPI SetSkew(
            /* [in] */ Float kx,
            /* [in] */ Float ky);

        CARAPI SetConcat(
            /* [in] */ IMatrix* a,
            /* [in] */ IMatrix* b,
            /* [out] */ Boolean* result);

        CARAPI PreTranslate(
            /* [in] */ Float dx,
            /* [in] */ Float dy,
            /* [out] */ Boolean* result);

        CARAPI PreScale(
            /* [in] */ Float sx,
            /* [in] */ Float sy,
            /* [in] */ Float px,
            /* [in] */ Float py,
            /* [out] */ Boolean* result);

        CARAPI PreScale(
            /* [in] */ Float sx,
            /* [in] */ Float sy,
            /* [out] */ Boolean* result);

        CARAPI PreRotate(
            /* [in] */ Float degrees,
            /* [in] */ Float px,
            /* [in] */ Float py,
            /* [out] */ Boolean* result);

        CARAPI PreRotate(
            /* [in] */ Float degrees,
            /* [out] */ Boolean* result);

        CARAPI PreSkew(
            /* [in] */ Float kx,
            /* [in] */ Float ky,
            /* [in] */ Float px,
            /* [in] */ Float py,
            /* [out] */ Boolean* result);

        CARAPI PreSkew(
            /* [in] */ Float kx,
            /* [in] */ Float ky,
            /* [out] */ Boolean* result);

        CARAPI PreConcat(
            /* [in] */ IMatrix* other,
            /* [out] */ Boolean* result);

        CARAPI PostTranslate(
            /* [in] */ Float dx,
            /* [in] */ Float dy,
            /* [out] */ Boolean* result);

        CARAPI PostScale(
            /* [in] */ Float sx,
            /* [in] */ Float sy,
            /* [in] */ Float px,
            /* [in] */ Float py,
            /* [out] */ Boolean* result);

        CARAPI PostScale(
            /* [in] */ Float sx,
            /* [in] */ Float sy,
            /* [out] */ Boolean* result);

        CARAPI PostRotate(
            /* [in] */ Float degrees,
            /* [in] */ Float px,
            /* [in] */ Float py,
            /* [out] */ Boolean* result);

        CARAPI PostRotate(
            /* [in] */ Float degrees,
            /* [out] */ Boolean* result);

        CARAPI PostSkew(
            /* [in] */ Float kx,
            /* [in] */ Float ky,
            /* [in] */ Float px,
            /* [in] */ Float py,
            /* [out] */ Boolean* result);

        CARAPI PostSkew(
            /* [in] */ Float kx,
            /* [in] */ Float ky,
            /* [out] */ Boolean* result);

        CARAPI PostConcat(
            /* [in] */ IMatrix* other,
            /* [out] */ Boolean* result);

        CARAPI SetRectToRect(
            /* [in] */ IRectF* src,
            /* [in] */ IRectF* dst,
            /* [in] */ MatrixScaleToFit stf,
            /* [out] */ Boolean* result);

        CARAPI SetPolyToPoly(
            /* [in] */ const ArrayOf<Float>& src,
            /* [in] */ Int32 srcIndex,
            /* [out] */ ArrayOf<Float>* dst,
            /* [in] */ Int32 dstIndex,
            /* [in] */ Int32 pointCount,
            /* [out] */ Boolean* result);

        CARAPI Invert(
            /* [in] */ IMatrix* inverse,
            /* [out] */ Boolean* result);

        CARAPI MapPoints(
            /* [out] */ ArrayOf<Float>* dst,
            /* [in] */ Int32 dstIndex,
            /* [in] */ const ArrayOf<Float>& src,
            /* [in] */ Int32 srcIndex,
            /* [in] */ Int32 pointCount);

        CARAPI MapVectors(
            /* [out] */ ArrayOf<Float>* dst,
            /* [in] */ Int32 dstIndex,
            /* [in] */ const ArrayOf<Float>& src,
            /* [in] */ Int32 srcIndex,
            /* [in] */ Int32 vectorCount);

        CARAPI MapPoints(
            /* [out] */ ArrayOf<Float>* dst,
            /* [in] */ const ArrayOf<Float>& src);

        CARAPI MapVectors(
            /* [out] */ ArrayOf<Float>* dst,
            /* [in] */ const ArrayOf<Float>& src);

        CARAPI MapPoints(
            /* [in, out] */ ArrayOf<Float>* pts);

        CARAPI MapVectors(
            /* [in, out] */ ArrayOf<Float>* vecs);

        CARAPI MapRect(
            /* [in] */ IRectF* dst,
            /* [in] */ IRectF* src,
            /* [out] */ Boolean* result);

        CARAPI MapRect(
            /* [in] */ IRectF* rect,
            /* [out] */ Boolean* result);

        CARAPI MapRadius(
            /* [in] */ Float radius,
            /* [out] */ Float* result);

        CARAPI GetValues(
            /* [out] */ ArrayOf<Float>* values);

        CARAPI SetValues(
            /* [in] */ const ArrayOf<Float>& values);

        CARAPI Equals(
            /* [in] */ IMatrix* obj,
            /* [out] */ Boolean* isEqual);

        CARAPI Equals(
            /* [in] */ IInterface* obj,
            /* [out] */ Boolean* isEqual);

        CARAPI GetHashCode(
            /* [out] */ Int32* hash);

        CARAPI ToString(
            /* [out] */ String* str) ;

        CARAPI PrintShortString(
            /* [in] */ IPrintWriter* pw);

        CARAPI ToShortString(
            /* [out] */ String* str);

        CARAPI ToShortString(
            /* [in] */ IStringBuilder* sb);
    };

public:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IMatrix* src);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    /**
     * Returns true if the matrix is identity.
     * This maybe faster than testing if (getType() == 0)
     */
    CARAPI IsIdentity(
        /* [out] */ Boolean* isIdentity);

    /* Returns true if will map a rectangle to another rectangle. This can be
     * true if the matrix is identity, scale-only, or rotates a multiple of 90
     * degrees.
     */
    CARAPI RectStaysRect(
        /* [out] */ Boolean* result);

    /**
     * (deep) copy the src matrix into this matrix. If src is null, reset this
     * matrix to the identity matrix.
     */
    CARAPI Set(
        /* [in] */ IMatrix* src);

    /** Set the matrix to identity */
    CARAPI Reset();

    /** Set the matrix to translate by (dx, dy). */
    CARAPI SetTranslate(
        /* [in] */ Float dx,
        /* [in] */ Float dy);

    /**
     * Set the matrix to scale by sx and sy, with a pivot point at (px, py).
     * The pivot point is the coordinate that should remain unchanged by the
     * specified transformation.
     */
    CARAPI SetScale(
        /* [in] */ Float sx,
        /* [in] */ Float sy,
        /* [in] */ Float px,
        /* [in] */ Float py);

    /** Set the matrix to scale by sx and sy. */
    CARAPI SetScale(
        /* [in] */ Float sx,
        /* [in] */ Float sy);

    /**
     * Set the matrix to rotate by the specified number of degrees, with a pivot
     * point at (px, py). The pivot point is the coordinate that should remain
     * unchanged by the specified transformation.
     */
    CARAPI SetRotate(
        /* [in] */ Float degrees,
        /* [in] */ Float px,
        /* [in] */ Float py);

    /**
     * Set the matrix to rotate about (0,0) by the specified number of degrees.
     */
    CARAPI SetRotate(
        /* [in] */ Float degrees);

    /**
     * Set the matrix to rotate by the specified sine and cosine values, with a
     * pivot point at (px, py). The pivot point is the coordinate that should
     * remain unchanged by the specified transformation.
     */
    CARAPI SetSinCos(
        /* [in] */ Float sinValue,
        /* [in] */ Float cosValue,
        /* [in] */ Float px,
        /* [in] */ Float py);

    /** Set the matrix to rotate by the specified sine and cosine values. */
    CARAPI SetSinCos(
        /* [in] */ Float sinValue,
        /* [in] */ Float cosValue);

    /**
     * Set the matrix to skew by sx and sy, with a pivot point at (px, py).
     * The pivot point is the coordinate that should remain unchanged by the
     * specified transformation.
     */
    CARAPI SetSkew(
        /* [in] */ Float kx,
        /* [in] */ Float ky,
        /* [in] */ Float px,
        /* [in] */ Float py);

    /** Set the matrix to skew by sx and sy. */
    CARAPI SetSkew(
        /* [in] */ Float kx,
        /* [in] */ Float ky);

    /**
     * Set the matrix to the concatenation of the two specified matrices,
     * returning true if the the result can be represented. Either of the two
     * matrices may also be the target matrix. this = a * b
     */
    CARAPI SetConcat(
        /* [in] */ IMatrix* a,
        /* [in] */ IMatrix* b,
        /* [out] */ Boolean* result);

    /**
     * Preconcats the matrix with the specified translation.
     * M' = M * T(dx, dy)
     */
    CARAPI PreTranslate(
        /* [in] */ Float dx,
        /* [in] */ Float dy,
        /* [out] */ Boolean* result);

    /**
     * Preconcats the matrix with the specified scale.
     * M' = M * S(sx, sy, px, py)
     */
    CARAPI PreScale(
        /* [in] */ Float sx,
        /* [in] */ Float sy,
        /* [in] */ Float px,
        /* [in] */ Float py,
        /* [out] */ Boolean* result);

    /**
     * Preconcats the matrix with the specified scale.
     * M' = M * S(sx, sy)
     */
    CARAPI PreScale(
        /* [in] */ Float sx,
        /* [in] */ Float sy,
        /* [out] */ Boolean* result);

    /**
     * Preconcats the matrix with the specified rotation.
     * M' = M * R(degrees, px, py)
     */
    CARAPI PreRotate(
        /* [in] */ Float degrees,
        /* [in] */ Float px,
        /* [in] */ Float py,
        /* [out] */ Boolean* result);

    /**
     * Preconcats the matrix with the specified rotation.
     * M' = M * R(degrees)
     */
    CARAPI PreRotate(
        /* [in] */ Float degrees,
        /* [out] */ Boolean* result);

    /**
     * Preconcats the matrix with the specified skew.
     * M' = M * K(kx, ky, px, py)
     */
    CARAPI PreSkew(
        /* [in] */ Float kx,
        /* [in] */ Float ky,
        /* [in] */ Float px,
        /* [in] */ Float py,
        /* [out] */ Boolean* result);

    /**
     * Preconcats the matrix with the specified skew.
     * M' = M * K(kx, ky)
     */
    CARAPI PreSkew(
        /* [in] */ Float kx,
        /* [in] */ Float ky,
        /* [out] */ Boolean* result);

    /**
     * Preconcats the matrix with the specified matrix.
     * M' = M * other
     */
    CARAPI PreConcat(
        /* [in] */ IMatrix* other,
        /* [out] */ Boolean* result);

    /**
     * Postconcats the matrix with the specified translation.
     * M' = T(dx, dy) * M
     */
    CARAPI PostTranslate(
        /* [in] */ Float dx,
        /* [in] */ Float dy,
        /* [out] */ Boolean* result);

    /**
     * Postconcats the matrix with the specified scale.
     * M' = S(sx, sy, px, py) * M
     */
    CARAPI PostScale(
        /* [in] */ Float sx,
        /* [in] */ Float sy,
        /* [in] */ Float px,
        /* [in] */ Float py,
        /* [out] */ Boolean* result);

    /**
     * Postconcats the matrix with the specified scale.
     * M' = S(sx, sy) * M
     */
    CARAPI PostScale(
        /* [in] */ Float sx,
        /* [in] */ Float sy,
        /* [out] */ Boolean* result);

    /**
     * Postconcats the matrix with the specified rotation.
     * M' = R(degrees, px, py) * M
     */
    CARAPI PostRotate(
        /* [in] */ Float degrees,
        /* [in] */ Float px,
        /* [in] */ Float py,
        /* [out] */ Boolean* result);

    /**
     * Postconcats the matrix with the specified rotation.
     * M' = R(degrees) * M
     */
    CARAPI PostRotate(
        /* [in] */ Float degrees,
        /* [out] */ Boolean* result);

    /**
     * Postconcats the matrix with the specified skew.
     * M' = K(kx, ky, px, py) * M
     */
    CARAPI PostSkew(
        /* [in] */ Float kx,
        /* [in] */ Float ky,
        /* [in] */ Float px,
        /* [in] */ Float py,
        /* [out] */ Boolean* result);

    /**
     * Postconcats the matrix with the specified skew.
     * M' = K(kx, ky) * M
     */
    CARAPI PostSkew(
        /* [in] */ Float kx,
        /* [in] */ Float ky,
        /* [out] */ Boolean* result);

    /**
     * Postconcats the matrix with the specified matrix.
     * M' = other * M
     */
    CARAPI PostConcat(
        /* [in] */ IMatrix* other,
        /* [out] */ Boolean* result);

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
    CARAPI SetRectToRect(
        /* [in] */ IRectF* src,
        /* [in] */ IRectF* dst,
        /* [in] */ MatrixScaleToFit stf,
        /* [out] */ Boolean* result);

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
    CARAPI SetPolyToPoly(
        /* [in] */ const ArrayOf<Float>& src,
        /* [in] */ Int32 srcIndex,
        /* [out] */ ArrayOf<Float>* dst,
        /* [in] */ Int32 dstIndex,
        /* [in] */ Int32 pointCount,
        /* [out] */ Boolean* result);

    /**
     * If this matrix can be inverted, return true and if inverse is not null,
     * set inverse to be the inverse of this matrix. If this matrix cannot be
     * inverted, ignore inverse and return false.
     */
    CARAPI Invert(
        /* [in] */ IMatrix* inverse,
        /* [out] */ Boolean* result);

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
    CARAPI MapPoints(
        /* [out] */ ArrayOf<Float>* dst,
        /* [in] */ Int32 dstIndex,
        /* [in] */ const ArrayOf<Float>& src,
        /* [in] */ Int32 srcIndex,
        /* [in] */ Int32 pointCount);

    /**
    * Apply this matrix to the array of 2D vectors specified by src, and write
     * the transformed vectors into the array of vectors specified by dst. The
     * two arrays represent their "vectors" as pairs of floats [x, y].
     *
     * @param dst   The array of dst vectors (x,y pairs)
     * @param dstIndex The index of the first [x,y] pair of dst floats
     * @param src   The array of src vectors (x,y pairs)
     * @param srcIndex The index of the first [x,y] pair of src floats
     * @param vectorCount The number of vectors (x,y pairs) to transform
     */
    CARAPI MapVectors(
        /* [out] */ ArrayOf<Float>* dst,
        /* [in] */ Int32 dstIndex,
        /* [in] */ const ArrayOf<Float>& src,
        /* [in] */ Int32 srcIndex,
        /* [in] */ Int32 vectorCount);

    /**
     * Apply this matrix to the array of 2D points specified by src, and write
     * the transformed points into the array of points specified by dst. The
     * two arrays represent their "points" as pairs of floats [x, y].
     *
     * @param dst   The array of dst points (x,y pairs)
     * @param src   The array of src points (x,y pairs)
     */
    CARAPI MapPoints(
        /* [out] */ ArrayOf<Float>* dst,
        /* [in] */ const ArrayOf<Float>& src);

    /**
     * Apply this matrix to the array of 2D vectors specified by src, and write
     * the transformed vectors into the array of vectors specified by dst. The
     * two arrays represent their "vectors" as pairs of floats [x, y].
     *
     * @param dst   The array of dst vectors (x,y pairs)
     * @param src   The array of src vectors (x,y pairs)
     */
    CARAPI MapVectors(
        /* [out] */ ArrayOf<Float>* dst,
        /* [in] */ const ArrayOf<Float>& src);

    /**
     * Apply this matrix to the array of 2D points, and write the transformed
     * points back into the array
     *
     * @param pts The array [x0, y0, x1, y1, ...] of points to transform.
     */
    CARAPI MapPoints(
        /* [in, out] */ ArrayOf<Float>* pts);

    /**
     * Apply this matrix to the array of 2D vectors, and write the transformed
     * vectors back into the array.
     * @param vecs The array [x0, y0, x1, y1, ...] of vectors to transform.
     */
    CARAPI MapVectors(
        /* [in, out] */ ArrayOf<Float>* vecs);

    /**
     * Apply this matrix to the src rectangle, and write the transformed
     * rectangle into dst. This is accomplished by transforming the 4 corners of
     * src, and then setting dst to the bounds of those points.
     *
     * @param dst Where the transformed rectangle is written.
     * @param src The original rectangle to be transformed.
     * @return the result of calling rectStaysRect()
     */
    CARAPI MapRect(
        /* [in] */ IRectF* dst,
        /* [in] */ IRectF* src,
        /* [out] */ Boolean* result);

    /**
     * Apply this matrix to the rectangle, and write the transformed rectangle
     * back into it. This is accomplished by transforming the 4 corners of rect,
     * and then setting it to the bounds of those points
     *
     * @param rect The rectangle to transform.
     * @return the result of calling rectStaysRect()
     */
    CARAPI MapRect(
        /* [in] */ IRectF* rect,
        /* [out] */ Boolean* result);

    /**
     * Return the mean radius of a circle after it has been mapped by
     * this matrix. NOTE: in perspective this value assumes the circle
     * has its center at the origin.
     */
    CARAPI MapRadius(
        /* [in] */ Float radius,
        /* [out] */ Float* result);

    /** Copy 9 values from the matrix into the array.
    */
    CARAPI GetValues(
        /* [in] */ ArrayOf<Float>* values);

    /** Copy 9 values from the array into the matrix.
        Depending on the implementation of Matrix, these may be
        transformed into 16.16 integers in the Matrix, such that
        a subsequent call to getValues() will not yield exactly
        the same values.
    */
    CARAPI SetValues(
        /* [in] */ const ArrayOf<Float>& values);

    /** Returns true iff obj is a Matrix and its values equal our values.
     */
    CARAPI Equals(
        /* [in] */ IMatrix* obj,
        /* [out] */ Boolean* isEqual);

    CARAPI Equals(
        /* [in] */ IInterface* obj,
        /* [out] */ Boolean* isEqual);

    CARAPI GetHashCode(
        /* [out] */ Int32* hash);

    CARAPI ToString(
        /* [out] */ String* str) ;

    /**
     * Print short string, to optimize dumping.
     * @hide
     */
    CARAPI PrintShortString(
        /* [in] */ IPrintWriter* pw);

    CARAPI ToShortString(
        /* [out] */ String* str);

    /**
     * @hide
     */
    CARAPI ToShortString(
        /* [in] */ IStringBuilder* sb);

public:
    static AutoPtr<IMatrix> IDENTITY_MATRIX;
};

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_GRAPHICS_CMATRIX_H__
