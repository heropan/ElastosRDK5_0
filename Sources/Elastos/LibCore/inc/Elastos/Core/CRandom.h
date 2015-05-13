
#ifndef __CRANDOM_H__
#define __CRANDOM_H__

#include "_CRandom.h"

namespace Elastos {
namespace Core {

CarClass(CRandom)
{
public:
    CRandom();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Int64 seed);

    /**
     * Returns a pseudo-random uniformly distributed {@code boolean}.
     */
    CARAPI NextBoolean(
        /* [out] */ Boolean* value);

    /**
     * Fills {@code buf} with random bytes.
     */
    CARAPI NextBytes(
        /* [out] */ ArrayOf<Byte>* buf);

    /**
     * Returns a pseudo-random uniformly distributed {@code double}
     * in the half-open range [0.0, 1.0).
     */
    CARAPI NextDouble(
        /* [out] */ Double* value);

    /**
     * Returns a pseudo-random uniformly distributed {@code float}
     * in the half-open range [0.0, 1.0).
     */
    CARAPI NextFloat(
        /* [out] */ Float* value);

    /**
     * Returns a pseudo-random (approximately) normally distributed
     * {@code double} with mean 0.0 and standard deviation 1.0.
     * This method uses the <i>polar method</i> of G. E. P. Box, M.
     * E. Muller, and G. Marsaglia, as described by Donald E. Knuth in <i>The
     * Art of Computer Programming, Volume 2: Seminumerical Algorithms</i>,
     * section 3.4.1, subsection C, algorithm P.
     */
    CARAPI NextGaussian(
        /* [out] */ Double* value);

    /**
     * Returns a pseudo-random uniformly distributed {@code int}.
     */
    CARAPI NextInt32(
        /* [out] */ Int32* value);

    /**
     * Returns a pseudo-random uniformly distributed {@code int}
     * in the half-open range [0, n).
     */
    CARAPI NextInt32Ex(
        /* [in] */ Int32 n,
        /* [out] */ Int32* value);

    /**
     * Returns a pseudo-random uniformly distributed {@code long}.
     */
    CARAPI NextInt64(
        /* [out] */ Int64* value);

    /**
     * Modifies the seed using a linear congruential formula presented in <i>The
     * Art of Computer Programming, Volume 2</i>, Section 3.2.1.
     */
    CARAPI SetSeed(
        /* [in] */ Int64 seed);

protected:
    /**
     * Returns a pseudo-random uniformly distributed {@code int} value of
     * the number of bits specified by the argument {@code bits} as
     * described by Donald E. Knuth in <i>The Art of Computer Programming,
     * Volume 2: Seminumerical Algorithms</i>, section 3.2.1.
     *
     * <p>Most applications will want to use one of this class' convenience methods instead.
     */
    CARAPI_(Int32) Next(
        /* [in] */ Int32 bits);

private:
    static const Int64 sMultiplier = 0x5deece66dLL;

    /**
     * The boolean value indicating if the second Gaussian number is available.
     *
     * @serial
     */
    Boolean mHaveNextNextGaussian;

    /**
     * @serial It is associated with the internal state of this generator.
     */
    Int64 mSeed;

    /**
     * The second Gaussian generated number.
     *
     * @serial
     */
    Double mNextNextGaussian;
};

} // namespace Elastos
} // namespace Core

#endif //__CRANDOM_H__
