
#include "CClipRectAnimation.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Animation {

CAR_OBJECT_IMPL(CClipRectAnimation);
CClipRectAnimation::CClipRectAnimation()
{
    CRect::New((IRect**)&mFromRect);
    CRect::New((IRect**)&mToRect);
}

ECode CClipRectAnimation::constructor(
    /* [in] */ IRect* fromClip,
    /* [in] */ IRect* toClip)
{
    if (fromClip == NULL || toClip == NULL) {
        // throw new RuntimeException("Expected non-null animation clip rects");
        return E_RUNTIME_EXCEPTION;
    }
    mFromRect->Set(fromClip);
    return mToRect->Set(toClip);
}

void CClipRectAnimation::ApplyTransformation(
    /* [in] */ Float it,
    /* [in] */ ITransformation* tr)
{
    Int32 v1 = 0, v2 = 0;
    mFromRect->GetLeft(&v1);
    mToRect->GetLeft(&v2);
    Int32 l = v1 + (Int32) ((v2 - v1) * it);

    mFromRect->GetTop(&v1);
    mToRect->GetTop(&v2);
    Int32 t = v1 + (Int32) ((v2 - v1) * it);

    mFromRect->GetRight(&v1);
    mToRect->GetRight(&v2);
    Int32 r = v1 + (Int32) ((v2 - v1) * it);

    mFromRect->GetBottom(&v1);
    mToRect->GetBottom(&v2);
    Int32 b = v1 + (Int32) ((v2 - v1) * it);
    tr->SetClipRect(l, t, r, b);
}

ECode CClipRectAnimation::WillChangeTransformationMatrix(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    return NOERROR;
}

}   //namespace Animation
}   //namespace View
}   //namespace Droid
}   //namespace Elastos
