
#include "view/animation/CAccelerateDecelerateInterpolator.h"
#include "ext/frameworkext.h"
#include <elastos/core/Math.h>

namespace Elastos {
namespace Droid {
namespace View {
namespace Animation {

CAR_OBJECT_IMPL(CAccelerateDecelerateInterpolator);
CAR_INTERFACE_IMPL_4(CAccelerateDecelerateInterpolator, Object, IAccelerateDecelerateInterpolator,INativeInterpolatorFactory,IInterpolator,ITimeInterpolator);

ECode CAccelerateDecelerateInterpolator::constructor()
{
    return NOERROR;
}

ECode CAccelerateDecelerateInterpolator::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return NOERROR;
}

ECode CAccelerateDecelerateInterpolator::GetInterpolation(
    /* [in] */ Float input,
    /* [out] */ Float* output)
{
    VALIDATE_NOT_NULL(output);
    *output = (Float)(Elastos::Core::Math::Cos((input + 1) * Elastos::Core::Math::DOUBLE_PI) / 2.0f) + 0.5f;
    return NOERROR;
}

ECode CAccelerateDecelerateInterpolator::CreateNativeInterpolator(
    /* [out] */ Int64* interpolator)
{
    VALIDATE_NOT_NULL(interpolator);
    *interpolator = NativeInterpolatorFactoryHelper::CreateAccelerateDecelerateInterpolator();
    return NOERROR;
}

}// namespace Animation
}// namespace View
}// namespace Droid
}// namespace Elastos
