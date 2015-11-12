
#ifndef  __ELASTOS_DROID_ANIMATION_INT32ARRAYEVALUATOR_H__
#define  __ELASTOS_DROID_ANIMATION_INT32ARRAYEVALUATOR_H__

#include "elastos/droid/ext/frameworkext.h"
#include <elastos/core/Object.h>


namespace Elastos {
namespace Droid {
namespace Animation {

/**
 * This evaluator can be used to perform type interpolation between <code>int[]</code> values.
 * Each index into the array is treated as a separate value to interpolate. For example,
 * evaluating <code>{100, 200}</code> and <code>{300, 400}</code> will interpolate the value at
 * the first index between 100 and 300 and the value at the second index value between 200 and 400.
 */
class Int32ArrayEvaluator
    : public Object
    , public ITypeEvaluator
{
public:
    CAR_INTERFACE_DECL();

    /**
     * Create an IntArrayEvaluator that does not reuse the animated value. Care must be taken
     * when using this option because on every evaluation a new <code>int[]</code> will be
     * allocated.
     *
     * @see #IntArrayEvaluator(int[])
     */
    Int32ArrayEvaluator();

    /**
     * Create an IntArrayEvaluator that reuses <code>reuseArray</code> for every evaluate() call.
     * Caution must be taken to ensure that the value returned from
     * {@link android.animation.ValueAnimator#getAnimatedValue()} is not cached, modified, or
     * used across threads. The value will be modified on each <code>evaluate()</code> call.
     *
     * @param reuseArray The array to modify and return from <code>evaluate</code>.
     */
    Int32ArrayEvaluator(
        /* [in] */ ArrayOf<Int32>* reuseArray);

    /**
     * Interpolates the value at each index by the fraction. If {@link #IntArrayEvaluator(int[])}
     * was used to construct this object, <code>reuseArray</code> will be returned, otherwise
     * a new <code>int[]</code> will be returned.
     *
     * @param fraction   The fraction from the starting to the ending values
     * @param startValue The start value.
     * @param endValue   The end value.
     * @return An <code>int[]</code> where each element is an interpolation between
     *         the same index in startValue and endValue.
     */
    // @Override
    CARAPI Evaluate(
        /* [in] */ Float fraction,
        /* [in] */ IInterface* startValue,
        /* [in] */ IInterface* endValue,
        /* [out] */ IInterface** result);

private:
    AutoPtr<ArrayOf<Int32> > mArray;
};

}   //namespace Animation
}   //namespace Droid
}   //namespace Elastos

#endif  // __ELASTOS_DROID_ANIMATION_INT32ARRAYEVALUATOR_H__
