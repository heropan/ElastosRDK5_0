
#ifndef __ELASTOS_DROID_ANIMATION_POINTFEVALUATOR_H__
#define __ELASTOS_DROID_ANIMATION_POINTFEVALUATOR_H__

#include "elastos/droid/ext/frameworkext.h"
#include "Elastos.Droid.Animation.h"
#include <elastos/core/Object.h>

using Elastos::Droid::Graphics::IPointF;
using Elastos::Core::Object;

namespace Elastos {
namespace Droid {
namespace Animation {

/**
 * This evaluator can be used to perform type interpolation between <code>PointF</code> values.
 */
class PointFEvaluator
    : public Object
    , public ITypeEvaluator
{
public:
    CAR_INTERFACE_DECL();

    /**
     * Construct a PointFEvaluator that returns a new PointF on every evaluate call.
     * To avoid creating an object for each evaluate call,
     * {@link PointFEvaluator#PointFEvaluator(android.graphics.PointF)} should be used
     * whenever possible.
     */
    PointFEvaluator();

    /**
     * Constructs a PointFEvaluator that modifies and returns <code>reuse</code>
     * in {@link #evaluate(float, android.graphics.PointF, android.graphics.PointF)} calls.
     * The value returned from
     * {@link #evaluate(float, android.graphics.PointF, android.graphics.PointF)} should
     * not be cached because it will change over time as the object is reused on each
     * call.
     *
     * @param reuse A PointF to be modified and returned by evaluate.
     */
    PointFEvaluator(
        /* [in] */ IPointF* reuse);

    /**
     * This function returns the result of linearly interpolating the start and
     * end PointF values, with <code>fraction</code> representing the proportion
     * between the start and end values. The calculation is a simple parametric
     * calculation on each of the separate components in the PointF objects
     * (x, y).
     *
     * <p>If {@link #PointFEvaluator(android.graphics.PointF)} was used to construct
     * this PointFEvaluator, the object returned will be the <code>reuse</code>
     * passed into the constructor.</p>
     *
     * @param fraction   The fraction from the starting to the ending values
     * @param startValue The start PointF
     * @param endValue   The end PointF
     * @return A linear interpolation between the start and end values, given the
     *         <code>fraction</code> parameter.
     */
    // @Override
    virtual CARAPI Evaluate(
        /* [in] */ Float fraction,
        /* [in] */ IPointF* startValue,
        /* [in] */ IPointF* endValue,
        /* [out] */ IInterface** pf);

private:
    /**
     * When null, a new PointF is returned on every evaluate call. When non-null,
     * mPoint will be modified and returned on every evaluate.
     */
    AutoPtr<IPointF> mPoint;
};

}   //namespace Animation
}   //namespace Droid
}   //namespace Elastos

#endif  // __ELASTOS_DROID_ANIMATION_POINTFEVALUATOR_H__
