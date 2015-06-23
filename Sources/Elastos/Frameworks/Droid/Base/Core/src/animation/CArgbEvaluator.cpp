
#include "animation/CArgbEvaluator.h"
#include "ext/frameworkext.h"

using Elastos::Core::IInteger32;
using Elastos::Core::CInteger32;

namespace Elastos {
namespace Droid {
namespace Animation {

ECode CArgbEvaluator::Evaluate(
    /* [in] */ Float fraction,
    /* [in] */ IInterface* startValue,
    /* [in] */ IInterface* endValue,
    /* [out] */ IInterface** result)
{
    assert(startValue != NULL && IInteger32::Probe(startValue) != NULL);
    assert(endValue != NULL && IInteger32::Probe(endValue) != NULL);

    Int32 startInt = 0;
    IInteger32::Probe(startValue)->GetValue(&startInt);

    Int32 startA = (startInt >> 24) & 0xff;
    Int32 startR = (startInt >> 16) & 0xff;
    Int32 startG = (startInt >> 8) & 0xff;
    Int32 startB = startInt & 0xff;

    Int32 endInt = 0;
    IInteger32::Probe(endValue)->GetValue(&endInt);

    Int32 endA = (endInt >> 24) & 0xff;
    Int32 endR = (endInt >> 16) & 0xff;
    Int32 endG = (endInt >> 8) & 0xff;
    Int32 endB = endInt & 0xff;

    Int32 retValue = (Int32)((startA + (Int32)(fraction * (endA - startA))) << 24) |
            (Int32)((startR + (Int32)(fraction * (endR - startR))) << 16) |
            (Int32)((startG + (Int32)(fraction * (endG - startG))) << 8) |
            (Int32)((startB + (Int32)(fraction * (endB - startB))));

    AutoPtr<IInteger32> r;
    CInteger32::New(retValue, (IInteger32**)&r);
    *result = r;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CArgbEvaluator::Evaluate(
    /* [in] */ Float fraction,
    /* [in] */ Int32 startValue,
    /* [in] */ Int32 endValue,
    /* [out] */ Int32* result)
{
    Int32 startInt = startValue;
    Int32 startA = (startInt >> 24) & 0xff;
    Int32 startR = (startInt >> 16) & 0xff;
    Int32 startG = (startInt >> 8) & 0xff;
    Int32 startB = startInt & 0xff;

    Int32 endInt = endValue;
    Int32 endA = (endInt >> 24) & 0xff;
    Int32 endR = (endInt >> 16) & 0xff;
    Int32 endG = (endInt >> 8) & 0xff;
    Int32 endB = endInt & 0xff;

    *result = (Int32)((startA + (Int32)(fraction * (endA - startA))) << 24) |
            (Int32)((startR + (Int32)(fraction * (endR - startR))) << 16) |
            (Int32)((startG + (Int32)(fraction * (endG - startG))) << 8) |
            (Int32)((startB + (Int32)(fraction * (endB - startB))));
    return NOERROR;
}

}   //namespace Animation
}   //namespace Droid
}   //namespace Elastos
