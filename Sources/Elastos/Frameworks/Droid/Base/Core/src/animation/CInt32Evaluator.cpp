
#include "animation/CInt32Evaluator.h"
#include "ext/frameworkext.h"

using Elastos::Core::IInteger32;
using Elastos::Core::CInteger32;

namespace Elastos {
namespace Droid {
namespace Animation {

ECode CInt32Evaluator::Evaluate(
    /* [in] */ Float fraction,
    /* [in] */ IInterface* startValue,
    /* [in] */ IInterface* endValue,
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);
    assert(startValue != NULL && IInteger32::Probe(startValue) != NULL);
    assert(endValue != NULL && IInteger32::Probe(endValue) != NULL);

    Int32 startInt = 0, endInt = 0;
    IInteger32::Probe(startValue)->GetValue(&startInt);
    IInteger32::Probe(endValue)->GetValue(&endInt);

    Int32 retValue = (Int32)(startInt + fraction * (endInt - startInt));

    return CInteger32::New(retValue, (IInteger32**)result);
}


ECode CInt32Evaluator::Evaluate(
    /* [in] */ Float fraction,
    /* [in] */ Int32 startValue,
    /* [in] */ Int32 endValue,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = (Int32)(startValue + fraction * (endValue - startValue));
    return NOERROR;
}

}   //namespace Animation
}   //namespace Droid
}   //namespace Elastos
