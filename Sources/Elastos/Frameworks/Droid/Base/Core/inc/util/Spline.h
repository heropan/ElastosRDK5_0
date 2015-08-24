
#ifndef __ELASTOS_DROID_UTILITY_SPLINE_H__
#define __ELASTOS_DROID_UTILITY_SPLINE_H__

#include "ext/frameworkext.h"
#include <elastos/core/Object.h>

namespace Elastos {
namespace Droid {
namespace Utility {

/**
 * Performs spline interpolation given a set of control points.
 * @hide
 */
class Spline :
    public Object
{
public:

    /**
     * Interpolates the value of Y = f(X) for given X.
     * Clamps X to the domain of the spline.
     *
     * @param x The X value.
     * @return The interpolated Y = f(X) value.
     */
    virtual Float Interpolate(float x) = 0;

    /**
     * Creates an appropriate spline based on the properties of the control points.
     *
     * If the control points are monotonic then the resulting spline will preserve that and
     * otherwise optimize for error bounds.
     */
    static CARAPI CreateSpline(
        /* [in] */ ArrayOf<Float>* x,
        /* [in] */ ArrayOf<Float>* y,
        /* [out] */ Spline** spline);

    /**
     * Creates a monotone cubic spline from a given set of control points.
     *
     * The spline is guaranteed to pass through each control point exactly.
     * Moreover, assuming the control points are monotonic (Y is non-decreasing or
     * non-increasing) then the interpolated values will also be monotonic.
     *
     * This function uses the Fritsch-Carlson method for computing the spline parameters.
     * http://en.wikipedia.org/wiki/Monotone_cubic_interpolation
     *
     * @param x The X component of the control points, strictly increasing.
     * @param y The Y component of the control points, monotonic.
     * @return
     *
     * @throws IllegalArgumentException if the X or Y arrays are null, have
     * different lengths or have fewer than 2 values.
     * @throws IllegalArgumentException if the control points are not monotonic.
     */
    static AutoPtr<Spline> CreateMonotoneCubicSpline(
        /* [in] */ ArrayOf<Float>* x,
        /* [in] */ ArrayOf<Float>* y);

    /**
     * Creates a linear spline from a given set of control points.
     *
     * Like a monotone cubic spline, the interpolated curve will be monotonic if the control points
     * are monotonic.
     *
     * @param x The X component of the control points, strictly increasing.
     * @param y The Y component of the control points.
     * @return
     *
     * @throws IllegalArgumentException if the X or Y arrays are null, have
     * different lengths or have fewer than 2 values.
     * @throws IllegalArgumentException if the X components of the control points are not strictly
     * increasing.
     */
    static  AutoPtr<Spline> CreateLinearSpline(
        /* [in] */ ArrayOf<Float>* x,
        /* [in] */ ArrayOf<Float>* y);

private:
     static Boolean IsStrictlyIncreasing(
        /* [in] */ ArrayOf<Float>* x);

    static Boolean IsMonotonic(
        /* [in] */ ArrayOf<Float>* x);
};

class MonotoneCubicSpline : public Spline
{
public:
    AutoPtr<ArrayOf<Float> > mX;
    AutoPtr<ArrayOf<Float> > mY;
    AutoPtr<ArrayOf<Float> > mM;

    MonotoneCubicSpline(
        /* [in] */ ArrayOf<Float>* x,
        /* [in] */ ArrayOf<Float>* y);

    Float Interpolate(
        /* [in] */ Float x);

    // For debugging.
    CARAPI ToString(
        /* [out] */ String* str);
};

class LinearSpline : public Spline
{
public:
    AutoPtr<ArrayOf<Float> > mX;
    AutoPtr<ArrayOf<Float> > mY;
    AutoPtr<ArrayOf<Float> > mM;

    LinearSpline(
        /* [in] */ ArrayOf<Float>* x,
        /* [in] */ ArrayOf<Float>* y);

    Float Interpolate(
        /* [in] */ Float x);

    // For debugging.
    CARAPI ToString(
        /* [out] */ String* str);
};

} // namespace Utility
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_UTILITY_SPLINE_H__
