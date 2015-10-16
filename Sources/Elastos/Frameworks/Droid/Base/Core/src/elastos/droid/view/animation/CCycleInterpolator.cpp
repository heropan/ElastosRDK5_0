
#include "elastos/droid/view/animation/CCycleInterpolator.h"
#include "elastos/droid/ext/frameworkext.h"
#include <elastos/core/Math.h>

namespace Elastos {
namespace Droid {
namespace View {
namespace Animation {

CAR_OBJECT_IMPL(CCycleInterpolator);
CAR_INTERFACE_IMPL_4(CCycleInterpolator, Object, ICycleInterpolator,INativeInterpolatorFactory,IInterpolator,ITimeInterpolator);

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
    AutoPtr<IResources> res;
    context->GetResources((IResources**)&res);
    AutoPtr<IResourcesTheme> theme;
    context->GetTheme((IResourcesTheme**)&theme);
    return constructor(res, theme, attrs);
}

ECode CCycleInterpolator::constructor(
    /* [in] */ IResources* res,
    /* [in] */ IResourcesTheme* theme,
    /* [in] */ IAttributeSet* attrs)
{
    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::CycleInterpolator),
            ARRAY_SIZE(R::styleable::CycleInterpolator));
    AutoPtr<ITypedArray> a;
    if (theme != NULL) {
        theme->btainStyledAttributes(attrs, attrIds, 0, 0, (ITypedArray**)&a);
    } else {
        resources->btainAttributes(attrs, attrIds, (ITypedArray**)&a);
    }

    a->GetFloat(R::styleable::CycleInterpolator_cycles, 1.0f, &mCycles);

    return a->Recycle();
}

ECode CCycleInterpolator::GetInterpolation(
    /* [in] */ Float input,
    /* [out] */ Float* output)
{
    VALIDATE_NOT_NULL(output);
    *output = (Float)(Elastos::Core::Math::Sin(2 * mCycles * Elastos::Core::Math::DOUBLE_PI * input));

    return NOERROR;
}

ECode CCycleInterpolator::CreateNativeInterpolator(
    /* [out] */ Int64* interpolator)
{
    VALIDATE_NOT_NULL(interpolator);
    *interpolator = NativeInterpolatorFactoryHelper::CreateCycleInterpolator(mCycles);
    return NOERROR;
}

}// namespace Animation
}// namespace View
}// namespace Droid
}// namespace Elastos
