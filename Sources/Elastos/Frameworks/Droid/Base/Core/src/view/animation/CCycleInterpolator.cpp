
#include "view/animation/CCycleInterpolator.h"
#include "ext/frameworkext.h"
#include <elastos/core/Math.h>

namespace Elastos {
namespace Droid {
namespace View {
namespace Animation {

CAR_OBJECT_IMPL(CCycleInterpolator);
CAR_INTERFACE_IMPL_5(CCycleInterpolator, Object, ICycleInterpolator,INativeInterpolatorFactory,IHasNativeInterpolator,IInterpolator,ITimeInterpolator);

ECode CCycleInterpolator::constructor(
    /* [in] */ Float cycles)
{
    mCycles = cycles;

    return NOERROR;
}

ECode CCycleInterpolator::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::CycleInterpolator),
            ARRAY_SIZE(R::styleable::CycleInterpolator));
    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributes(attrs, attrIds, (ITypedArray**)&a);

    a->GetFloat(R::styleable::CycleInterpolator_cycles, 1.0f, &mCycles);

    a->Recycle();

    return NOERROR;
}

ECode CCycleInterpolator::GetInterpolation(
    /* [in] */ Float input,
    /* [out] */ Float* output)
{
    VALIDATE_NOT_NULL(output);
    *output = (Float)(Elastos::Core::Math::Sin(2 * mCycles * Elastos::Core::Math::DOUBLE_PI * input));

    return NOERROR;
}

}// namespace Animation
}// namespace View
}// namespace Droid
}// namespace Elastos
