
#include "elastos/droid/view/animation/CLinearInterpolator.h"
#include "elastos/droid/internal/view/animation/NativeInterpolatorFactoryHelper.h"
#include "elastos/droid/ext/frameworkext.h"

using Elastos::Droid::Animation::EIID_ITimeInterpolator;
using Elastos::Droid::Internal::View::Animation::EIID_INativeInterpolatorFactory;
using Elastos::Droid::Internal::View::Animation::NativeInterpolatorFactoryHelper;

namespace Elastos {
namespace Droid {
namespace View {
namespace Animation {

CAR_OBJECT_IMPL(CLinearInterpolator);

CAR_INTERFACE_IMPL_4(CLinearInterpolator, Object, ILinearInterpolator, INativeInterpolatorFactory, IInterpolator, ITimeInterpolator);

CLinearInterpolator::CLinearInterpolator()
{}

CLinearInterpolator::~CLinearInterpolator()
{}

ECode CLinearInterpolator::constructor()
{
    return NOERROR;
}

ECode CLinearInterpolator::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return NOERROR;
}

ECode CLinearInterpolator::GetInterpolation(
    /* [in] */ Float input,
    /* [out] */ Float* output)
{
    VALIDATE_NOT_NULL(output);
    *output = input;
    return NOERROR;
}

ECode CLinearInterpolator::CreateNativeInterpolator(
    /* [out] */ Int64* interpolator)
{
    VALIDATE_NOT_NULL(interpolator);
    *interpolator = NativeInterpolatorFactoryHelper::CreateLinearInterpolator();
    return NOERROR;
}

}   //namespace Animation
}   //namespace View
}   //namespace Droid
}   //namespace Elastos
