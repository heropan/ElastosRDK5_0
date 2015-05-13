
#include "view/CGravity.h"
#include "view/Gravity.h"

namespace Elastos {
namespace Droid {
namespace View {

ECode CGravity::Apply(
    /* [in] */ Int32 gravity,
    /* [in] */ Int32 w,
    /* [in] */ Int32 h,
    /* [in] */ IRect* container,
    /* [in] */ IRect* outRect)
{
    return Gravity::Apply(gravity, w, h, container, outRect);
}

ECode CGravity::ApplyEx(
    /* [in] */ Int32 gravity,
    /* [in] */ Int32 w,
    /* [in] */ Int32 h,
    /* [in] */ IRect* container,
    /* [in] */ IRect* outRect,
    /* [in] */ Int32 layoutDirection)
{
    return Gravity::Apply(gravity, w, h, container, outRect, layoutDirection);
}

ECode CGravity::ApplyEx2(
    /* [in] */ Int32 gravity,
    /* [in] */ Int32 w,
    /* [in] */ Int32 h,
    /* [in] */ IRect* container,
    /* [in] */ Int32 xAdj,
    /* [in] */ Int32 yAdj,
    /* [in] */ IRect* outRect)
{
    return Gravity::Apply(gravity, w, h, container, xAdj, yAdj, outRect);
}

ECode CGravity::ApplyEx3(
    /* [in] */ Int32 gravity,
    /* [in] */ Int32 w,
    /* [in] */ Int32 h,
    /* [in] */ IRect* container,
    /* [in] */ Int32 xAdj,
    /* [in] */ Int32 yAdj,
    /* [in] */ IRect* outRect,
    /* [in] */ Int32 layoutDirection)
{
    return Gravity::Apply(gravity, w, h, container, xAdj, yAdj, outRect, layoutDirection);
}

ECode CGravity::ApplyDisplay(
    /* [in] */ Int32 gravity,
    /* [in] */ IRect* display,
    /* [in] */ IRect* inoutObj)
{
    return Gravity::ApplyDisplay(gravity, display, inoutObj);
}

ECode CGravity::ApplyDisplayEx(
    /* [in] */ Int32 gravity,
    /* [in] */ IRect* display,
    /* [in] */ IRect* inoutObj,
    /* [in] */ Int32 layoutDirection)
{
    return Gravity::ApplyDisplay(gravity, display, inoutObj, layoutDirection);
}

ECode CGravity::IsVertical(
    /* [in] */ Int32 gravity,
    /* [out] */ Boolean* isVertical)
{
    VALIDATE_NOT_NULL(isVertical);
    *isVertical = Gravity::IsVertical(gravity);
    return NOERROR;
}

ECode CGravity::IsHorizontal(
    /* [in] */ Int32 gravity,
    /* [out] */ Boolean* isHorizontal)
{
    VALIDATE_NOT_NULL(isHorizontal);
    *isHorizontal = Gravity::IsHorizontal(gravity);
    return NOERROR;
}

ECode CGravity::GetAbsoluteGravity(
    /* [in] */ Int32 gravity,
    /* [in] */ Int32 layoutDirection,
    /* [out] */ Int32* absoluteGravity)
{
    VALIDATE_NOT_NULL(absoluteGravity);
    *absoluteGravity = Gravity::GetAbsoluteGravity(gravity, layoutDirection);
    return NOERROR;
}

} // namespace View
} // namespace Droid
} // namespace Elastos
