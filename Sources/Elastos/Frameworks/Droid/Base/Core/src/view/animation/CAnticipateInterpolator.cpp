
#include "view/animation/CAnticipateInterpolator.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Animation {

ECode CAnticipateInterpolator::constructor()
{
    mTension = 2.0f;
    return NOERROR;
}

ECode CAnticipateInterpolator::constructor(
    /* [in] */ Float tension)
{
    mTension = tension;

    return NOERROR;
}

ECode CAnticipateInterpolator::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::AnticipateInterpolator),
            ARRAY_SIZE(R::styleable::AnticipateInterpolator));
    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributesEx2(attrs, attrIds, (ITypedArray**)&a);

    a->GetFloat(R::styleable::AnticipateInterpolator_tension, 2.0f, &mTension);

    a->Recycle();

    return NOERROR;
}

ECode CAnticipateInterpolator::GetInterpolation(
    /* [in] */ Float input,
    /* [out] */ Float* output)
{
    VALIDATE_NOT_NULL(output);

    // a(t) = t * t * ((tension + 1) * t - tension)
    *output = input * input * ((mTension + 1) * input - mTension);

    return NOERROR;
}

}// namespace Animation
}// namespace View
}// namespace Droid
}// namespace Elastos
