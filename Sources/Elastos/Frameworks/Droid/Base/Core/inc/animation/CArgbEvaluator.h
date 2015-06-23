
#ifndef  __CARGBEVALUATOR_H__
#define  __CARGBEVALUATOR_H__

#include "_CArgbEvaluator.h"

namespace Elastos {
namespace Droid {
namespace Animation {

/**
 * This evaluator can be used to perform type interpolation between integer
 * values that represent ARGB colors.
 */
CarClass(CArgbEvaluator)
{
public:
    /**
     * This function returns the calculated in-between value for a color
     * given integers that represent the start and end values in the four
     * bytes of the 32-bit int. Each channel is separately linearly interpolated
     * and the resulting calculated values are recombined into the return value.
     *
     * @param fraction The fraction from the starting to the ending values
     * @param startValue A 32-bit int value representing colors in the
     * separate bytes of the parameter
     * @param endValue A 32-bit int value representing colors in the
     * separate bytes of the parameter
     * @return A value that is calculated to be the linearly interpolated
     * result, derived by separating the start and end values into separate
     * color channels and interpolating each one separately, recombining the
     * resulting values in the same way.
     */
    CARAPI Evaluate(
        /* [in] */ Float fraction,
        /* [in] */ IInterface* startValue,
        /* [in] */ IInterface* endValue,
        /* [out] */ IInterface** result);

    CARAPI Evaluate(
        /* [in] */ Float fraction,
        /* [in] */ Int32 startValue,
        /* [in] */ Int32 endValue,
        /* [out] */ Int32* result);

};


}   //namespace Animation
}   //namespace Droid
}   //namespace Elastos
#endif  //__ARGBEVALUATOR_H__
