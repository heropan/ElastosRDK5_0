
#include "utility/Spline.h"
#include "utility/FloatMath.h"
#include <elastos/core/Math.h>
#include <elastos/core/StringBuilder.h>

using Elastos::Core::StringBuilder;

namespace Elastos {
namespace Droid {
namespace Utility {


ECode Spline::CreateSpline(
    /* [in] */ ArrayOf<Float>* x,
    /* [in] */ ArrayOf<Float>* y,
    /* [out] */ Spline** spline)
{
    VALIDATE_NOT_NULL(spline)
    *spline = NULL;

    if (!IsStrictlyIncreasing(x)) {
        // throw new IllegalArgumentException("The control points must all have strictly "
        //         + "increasing X values.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (x == NULL || y == NULL || x->GetLength() != y->GetLength() || x->GetLength() < 2) {
        // throw new IllegalArgumentException("There must be at least two control "
        //         + "points and the arrays must be of equal length.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (IsMonotonic(y)) {
        *spline = CreateMonotoneCubicSpline(x, y);
    }
    else {
        *spline = CreateLinearSpline(x, y);
    }

    REFCOUNT_ADD(*spline)
    return NOERROR;
}

AutoPtr<Spline> Spline::CreateMonotoneCubicSpline(
    /* [in] */ ArrayOf<Float>* x,
    /* [in] */ ArrayOf<Float>* y)
{
    if (x == NULL || y == NULL || x->GetLength() != y->GetLength() || x->GetLength() < 2) {
        // throw new IllegalArgumentException("There must be at least two control "
        //         + "points and the arrays must be of equal length.");
        return NULL;
    }

    AutoPtr<Spline> spline = new MonotoneCubicSpline(x, y);
    return spline;
}

AutoPtr<Spline> Spline::CreateLinearSpline(
    /* [in] */ ArrayOf<Float>* x,
    /* [in] */ ArrayOf<Float>* y)
{
    if (x == NULL || y == NULL || x->GetLength() != y->GetLength() || x->GetLength() < 2) {
        // throw new IllegalArgumentException("There must be at least two control "
        //         + "points and the arrays must be of equal length.");
        return NULL;
    }

    AutoPtr<Spline> spline = new LinearSpline(x, y);
    return spline;
}

Boolean Spline::IsStrictlyIncreasing(
    /* [in] */ ArrayOf<Float>* x)
{
    if (x == NULL || x->GetLength() < 2) {
        // throw new IllegalArgumentException("There must be at least two control points.");
        return FALSE;
    }

    Float prev = (*x)[0];
    for (Int32 i = 1; i < x->GetLength(); i++) {
        Float curr = (*x)[i];
        if (curr <= prev) {
            return FALSE;
        }
        prev = curr;
    }
    return TRUE;
}

Boolean Spline::IsMonotonic(
    /* [in] */ ArrayOf<Float>* x)
{
    if (x == NULL || x->GetLength() < 2) {
        //throw new IllegalArgumentException("There must be at least two control points.");
        return FALSE;
    }

    Float prev = (*x)[0];
    for (Int32 i = 1; i < x->GetLength(); i++) {
        Float curr = (*x)[i];
        if (curr < prev) {
            return FALSE;
        }
        prev = curr;
    }
    return TRUE;
}

//================================================================
// MonotoneCubicSpline
//================================================================

MonotoneCubicSpline::MonotoneCubicSpline(
    /* [in] */ ArrayOf<Float>* x,
    /* [in] */ ArrayOf<Float>* y)
{
    Int32 n = x->GetLength();
    AutoPtr<ArrayOf<Float> > d = ArrayOf<Float>::Alloc(n - 1); // could optimize this out
    AutoPtr<ArrayOf<Float> > m = ArrayOf<Float>::Alloc(n);

    // Compute slopes of secant lines between successive points.
    for (Int32 i = 0; i < n - 1; i++) {
        Float h = (*x)[i + 1] - (*x)[i];
        if (h <= 0.0f) {
            // throw new IllegalArgumentException("The control points must all "
            //         + "have strictly increasing X values.");
            assert(0 && "The control points must all have strictly increasing X values.");
        }
        (*d)[i] = ((*y)[i + 1] - (*y)[i]) / h;
    }

    // Initialize the tangents as the average of the secants.
    (*m)[0] = (*d)[0];
    for (Int32 i = 1; i < n - 1; i++) {
        (*m)[i] = ((*d)[i - 1] + (*d)[i]) * 0.5f;
    }
    (*m)[n - 1] = (*d)[n - 2];

    // Update the tangents to preserve monotonicity.
    for (Int32 i = 0; i < n - 1; i++) {
        if ((*d)[i] == 0.0f) { // successive Y values are equal
            (*m)[i] = 0.0f;
            (*m)[i + 1] = 0.0f;
        }
        else {
            Float a = (*m)[i] / (*d)[i];
            Float b = (*m)[i + 1] / (*d)[i];
            if (a < 0.0f || b < 0.0f) {
                // throw new IllegalArgumentException("The control points must have "
                //         + "monotonic Y values.");
                assert(0 && "The control points must have monotonic Y values.");
            }
            Float h = FloatMath::Hypot(a, b);
            if (h > 9.0f) {
                Float t = 3.0f / h;
                (*m)[i] = t * a * (*d)[i];
                (*m)[i + 1] = t * b * (*d)[i];
            }
        }
    }

    mX = x;
    mY = y;
    mM = m;
}

Float MonotoneCubicSpline::Interpolate(
    /* [in] */ Float x)
{
    // Handle the boundary cases.
    Int32 n = mX->GetLength();
    if (Elastos::Core::Math::IsNaN(x)) {
        return x;
    }
    if (x <= (*mX)[0]) {
        return (*mY)[0];
    }
    if (x >= (*mX)[n - 1]) {
        return (*mY)[n - 1];
    }

    // Find the index 'i' of the last point with smaller X.
    // We know this will be within the spline due to the boundary tests.
    Int32 i = 0;
    while (x >= (*mX)[i + 1]) {
        i += 1;
        if (x == (*mX)[i]) {
            return (*mY)[i];
        }
    }

    // Perform cubic Hermite spline interpolation.
    Float h = (*mX)[i + 1] - (*mX)[i];
    Float t = (x - (*mX)[i]) / h;
    return ((*mY)[i] * (1 + 2 * t) + h * (*mM)[i] * t) * (1 - t) * (1 - t)
            + ((*mY)[i + 1] * (3 - 2 * t) + h * (*mM)[i + 1] * (t - 1)) * t * t;
}

ECode MonotoneCubicSpline::ToString(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result)
    StringBuilder str;
    Int32 n = mX->GetLength();
    str.Append("MonotoneCubicSpline{[");
    for (Int32 i = 0; i < n; i++) {
        if (i != 0) {
            str.Append(", ");
        }
        str.Append("(");
        str.Append((*mX)[i]);
        str.Append(", ");
        str.Append((*mY)[i]);
        str.Append(": ");
        str.Append((*mM)[i]);
        str.Append(")");
    }
    str.Append("]}");
    *result = str.ToString();
    return NOERROR;
}


//================================================================
// LinearSpline
//================================================================
LinearSpline::LinearSpline(
    /* [in] */ ArrayOf<Float>* x,
    /* [in] */ ArrayOf<Float>* y)
{
    Int32 N = x->GetLength();
    mM = ArrayOf<Float>::Alloc(N-1);
    for (Int32 i = 0; i < N-1; i++) {
        (*mM)[i] = (y[i+1] - y[i]) / ((*x)[i+1] - (*x)[i]);
    }
    mX = x;
    mY = y;
}

Float LinearSpline::Interpolate(
    /* [in] */ Float x)
{
    // Handle the boundary cases.
    Int32 n = mX->GetLength();
    if (Elastos::Core::Math::IsNaN(x)) {
        return x;
    }
    if (x <= (*mX)[0]) {
        return (*mY)[0];
    }
    if (x >= (*mX)[n - 1]) {
        return (*mY)[n - 1];
    }

    // Find the index 'i' of the last point with smaller X.
    // We know this will be within the spline due to the boundary tests.
    Int32 i = 0;
    while (x >= (*mX)[i + 1]) {
        i += 1;
        if (x == (*mX)[i]) {
            return (*mY)[i];
        }
    }
    return (*mY)[i] + (*mM)[i] * (x - (*mX)[i]);
}

ECode LinearSpline::ToString(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result)
    StringBuilder str;
    Int32 n = mX->GetLength();
    str.Append("LinearSpline{[");
    for (Int32 i = 0; i < n; i++) {
        if (i != 0) {
            str.Append(", ");
        }
        str.Append("(");
        str.Append((*mX)[i]);
        str.Append(", ");
        str.Append((*mY)[i]);
        if (i < n-1) {
            str.Append(": ");
            str.Append((*mM)[i]);
        }
        str.Append(")");
    }
    str.Append("]}");
    *result = str.ToString();
    return NOERROR;
}


} // namespace Utility
} // namespace Droid
} // namespace Elastos
