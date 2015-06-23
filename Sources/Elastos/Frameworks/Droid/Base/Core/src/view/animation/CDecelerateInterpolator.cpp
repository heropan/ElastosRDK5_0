
#include "view/animation/CDecelerateInterpolator.h"
#include "ext/frameworkext.h"
#include <elastos/core/Math.h>

namespace Elastos {
namespace Droid {
namespace View {
namespace Animation {

ECode CDecelerateInterpolator::constructor()
{
    mFactor = 1.0f;

    return NOERROR;
}

/**
 * Constructor
 *
 * @param factor Degree to which the animation should be eased. Setting factor to 1.0f produces
 *        an upside-down y=x^2 parabola. Increasing factor above 1.0f makes exaggerates the
 *        ease-out effect (i.e., it starts even faster and ends evens slower)
 */
ECode CDecelerateInterpolator::constructor(
    /* [in] */ Float factor)
{
    mFactor = factor;

    return NOERROR;
}

ECode CDecelerateInterpolator::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::DecelerateInterpolator),
            ARRAY_SIZE(R::styleable::DecelerateInterpolator));
    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributes(attrs, attrIds, (ITypedArray**)&a);

    a->GetFloat(R::styleable::DecelerateInterpolator_factor, 1.0f, &mFactor);

    a->Recycle();

    return NOERROR;
}

ECode CDecelerateInterpolator::GetInterpolation(
    /* [in] */ Float input,
    /* [out] */ Float* output)
{
    VALIDATE_NOT_NULL(output);

    if (mFactor == 1.0f) {
        *output = (Float)(1.0f - (1.0f - input) * (1.0f - input));
    }
    else {
        *output = (Float)(1.0f - Elastos::Core::Math::Pow((1.0f - input), 2 * mFactor));
    }

    return NOERROR;
}

}// namespace Animation
}// namespace View
}// namespace Droid
}// namespace Elastos
