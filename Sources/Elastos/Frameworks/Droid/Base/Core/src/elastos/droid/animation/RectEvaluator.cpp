
#include "elastos/droid/animation/RectEvaluator.h"
// #include "elastos/droid/graphics/CRect.h"

// using Elastos::Droid::Graphics::CRect;

namespace Elastos {
namespace Droid {
namespace Animation {

CAR_INTERFACE_IMPL(RectEvaluator, Object, ITypeEvaluator);
RectEvaluator::RectEvaluator()
{
}

RectEvaluator::RectEvaluator(
    /* [in] */ IRect* reuseRect)
    : mRect(reuseRect)
{
}

ECode RectEvaluator::Evaluate(
    /* [in] */ Float fraction,
    /* [in] */ IRect* startValue,
    /* [in] */ IRect* endValue,
    /* [out] */ IInterface** rect)
{
    VALIDATE_NOT_NULL(rect);
    Int32 v1 = 0, v2 = 0;
    startValue->GetLeft(&v1);
    endValue->GetLeft(&v2);
    Int32 left = v1 + (Int32) ((v2 - v1) * fraction);

    startValue->GetTop(&v1);
    endValue->GetTop(&v2);
    Int32 top = v1 + (Int32) ((v2 - v1) * fraction);

    startValue->GetRight(&v1);
    endValue->GetRight(&v2);
    Int32 right = v1 + (Int32) ((v2 - v1) * fraction);

    startValue->GetBottom(&v1);
    endValue->GetBottom(&v2);
    Int32 bottom = v1 + (Int32) ((v2 - v1) * fraction);
    if (mRect == NULL) {
        assert(0 && "TODO");
        // return CRect::New(left, top, right, bottom, rect);
        return NOERROR;
    }

    mRect->Set(left, top, right, bottom);
    *rect = mRect;
    REFCOUNT_ADD(*rect);
    return NOERROR;
}

}   //namespace Animation
}   //namespace Droid
}   //namespace Elastos
