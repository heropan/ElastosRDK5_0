
#include "view/animation/CAccelerateInterpolator.h"
#include "ext/frameworkext.h"
#include <elastos/Math.h>

namespace Elastos {
namespace Droid {
namespace View {
namespace Animation {

ECode CAccelerateInterpolator::constructor()
{
    mFactor = 1.0f;
    mDoubleFactor = 2.0;

    return NOERROR;
}

ECode CAccelerateInterpolator::constructor(
    /* [in] */ Float factor)
{
    mFactor = factor;
    mDoubleFactor = 2 * mFactor;

    return NOERROR;
}

ECode CAccelerateInterpolator::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::AccelerateInterpolator),
            ARRAY_SIZE(R::styleable::AccelerateInterpolator));
    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributesEx2(attrs, attrIds, (ITypedArray**)&a);

    a->GetFloat(R::styleable::AccelerateInterpolator_factor, 1.0f, &mFactor);
    mDoubleFactor = 2 * mFactor;

    a->Recycle();

    return NOERROR;
}

ECode CAccelerateInterpolator::GetInterpolation(
    /* [in] */ Float input,
    /* [out] */ Float* output)
{
    VALIDATE_NOT_NULL(output);

    if (mFactor == 1.0f) {
        *output = input * input;
    }
    else {
        *output =  (Float)Elastos::Core::Math::Pow(input, mDoubleFactor);
    }

    return NOERROR;
}

}// namespace Animation
}// namespace View
}// namespace Droid
}// namespace Elastos
