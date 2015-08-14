#ifndef __ELASTOS_DROID_UTILITY_ELAOST_DROID_UTILITY_FLOAT_MATH_H__
#define __ELASTOS_DROID_UTILITY_ELAOST_DROID_UTILITY_FLOAT_MATH_H__

#include <elastos.h>

using namespace Elastos;

namespace Elastos {
namespace Droid {
namespace Utility {

/**
 * Math routines similar to those found in {@link java.lang.Math}. Performs
 * computations on {@code float} values directly without incurring the overhead
 * of conversions to and from {@code double}.
 *
 * <p>On one platform, {@code FloatMath.sqrt(100)} executes in one third of the
 * time required by {@code java.lang.Math.sqrt(100)}.</p>
 */
class FloatMath
{
public:
    /**
     * Returns the float conversion of the most positive (i.e. closest to
     * positive infinity) integer value which is less than the argument.
     *
     * @param value to be converted
     * @return the floor of value
     */
    static Float Floor(
        /* [in] */ Float value);

    /**
     * Returns the float conversion of the most negative (i.e. closest to
     * negative infinity) integer value which is greater than the argument.
     *
     * @param value to be converted
     * @return the ceiling of value
     */
    static Float Ceil(
        /* [in] */ Float value);

    /**
     * Returns the closest float approximation of the sine of the argument.
     *
     * @param angle to compute the cosine of, in radians
     * @return the sine of angle
     */
    static Float Sin(
        /* [in] */ Float angle);

    /**
     * Returns the closest float approximation of the cosine of the argument.
     *
     * @param angle to compute the cosine of, in radians
     * @return the cosine of angle
     */
    static Float Cos(
        /* [in] */ Float angle);

    /**
     * Returns the closest float approximation of the square root of the
     * argument.
     *
     * @param value to compute sqrt of
     * @return the square root of value
     */
    static Float Sqrt(
        /* [in] */ Float value);

    /**
     * Returns the closest float approximation of the raising "e" to the power
     * of the argument.
     *
     * @param value to compute the exponential of
     * @return the exponential of value
     */
    static Float Exp(
        /* [in] */ Float value);

    /**
     * Returns the closest float approximation of the result of raising {@code
     * x} to the power of {@code y}.
     *
     * @param x the base of the operation.
     * @param y the exponent of the operation.
     * @return {@code x} to the power of {@code y}.
     */
    static Float Pow(
        /* [in] */ Float x,
        /* [in] */ Float y);

    /**
     * Returns {@code sqrt(}<i>{@code x}</i><sup>{@code 2}</sup>{@code +} <i>
     * {@code y}</i><sup>{@code 2}</sup>{@code )}.
     *
     * @param x a float number
     * @param y a float number
     * @return the hypotenuse
     */
    static Float Hypot(
        /* [in] */ Float x,
        /* [in] */ Float y);

private:
    FloatMath();
    FloatMath(const FloatMath& other);
};

} // namespace Utility
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_UTILITY_ELAOST_DROID_UTILITY_FLOAT_MATH_H__
