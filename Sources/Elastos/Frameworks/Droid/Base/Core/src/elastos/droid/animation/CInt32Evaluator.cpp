
#include "elastos/droid/animation/CInt32Evaluator.h"
#include "elastos/droid/ext/frameworkext.h"

using Elastos::Core::IInteger32;
using Elastos::Core::CInteger32;

namespace Elastos {
namespace Droid {
namespace Animation {

CAR_OBJECT_IMPL(CInt32Evaluator);
CAR_INTERFACE_IMPL_2(CInt32Evaluator, Object, IInt32Evaluator, ITypeEvaluator);
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

}   //namespace Animation
}   //namespace Droid
}   //namespace Elastos
