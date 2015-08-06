
#include "animation/CFloatEvaluator.h"
#include "ext/frameworkext.h"

using Elastos::Core::IFloat;
using Elastos::Core::CFloat;

namespace Elastos {
namespace Droid {
namespace Animation {

CAR_OBJECT_IMPL(CFloatEvaluator);
CAR_INTERFACE_IMPL_2(CFloatEvaluator, Object, IFloatEvaluator, ITypeEvaluator);

ECode CFloatEvaluator::Evaluate(
    /* [in] */ Float fraction,
    /* [in] */ IInterface* startValue,
    /* [in] */ IInterface* endValue,
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);
    assert(startValue != NULL && INumber::Probe(startValue) != NULL);
    assert(endValue != NULL && INumber::Probe(endValue) != NULL);

    Float startFloat = 0.0f, endFloat = 0.0f;
    INumber::Probe(startValue)->FloatValue(&startFloat);
    INumber::Probe(endValue)->FloatValue(&endFloat);

    AutoPtr<IFloat> r;
    CFloat::New(startFloat + fraction * (endFloat - startFloat),
            (IFloat**)&r);
    *result = r;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CFloatEvaluator::Evaluate(
    /* [in] */ Float fraction,
    /* [in] */ Float startValue,
    /* [in] */ Float endValue,
    /* [out] */ Float* result)
{
    VALIDATE_NOT_NULL(result);
    *result = startValue + fraction * (endValue - startValue);
    return NOERROR;
}

}   //namespace Animation
}   //namespace Droid
}   //namespace Elastos
