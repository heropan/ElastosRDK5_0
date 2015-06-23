
#include "view/animation/COvershootInterpolator.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Animation {

ECode COvershootInterpolator::constructor()
{
    mTension = 2.0f;

    return NOERROR;
}

ECode COvershootInterpolator::constructor(
    /* [in] */ Float tension)
{
    mTension = tension;

    return NOERROR;
}

ECode COvershootInterpolator::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::OvershootInterpolator),
            ARRAY_SIZE(R::styleable::OvershootInterpolator));
    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributes(attrs, attrIds, (ITypedArray**)&a);

    a->GetFloat(R::styleable::OvershootInterpolator_tension, 2.0f, &mTension);

    a->Recycle();

    return NOERROR;
}

ECode COvershootInterpolator::GetInterpolation(
    /* [in] */ Float input,
    /* [out] */ Float* output)
{
    VALIDATE_NOT_NULL(output);

    // _o(t) = t * t * ((tension + 1) * t + tension)
    // o(t) = _o(t - 1) + 1
    input -= 1.0f;
    *output = input * input * ((mTension + 1) * input + mTension) + 1.0f;

    return NOERROR;
}

}// namespace Animation
}// namespace View
}// namespace Droid
}// namespace Elastos
