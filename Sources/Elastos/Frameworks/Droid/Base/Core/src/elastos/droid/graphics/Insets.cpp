
#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/graphics/Insets.h"
#include "elastos/droid/graphics/CRect.h"

namespace Elastos {
namespace Droid {
namespace Graphics {

const AutoPtr<Insets> Insets::NONE = new Insets(0, 0, 0, 0);

CAR_INTERFACE_IMPL(Insets, Object, IInsets);
Insets::Insets(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
    : mLeft(left)
    , mTop(top)
    , mRight(right)
    , mBottom(bottom)
{}

AutoPtr<Insets> Insets::Of(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    if (left == 0 && top == 0 && right == 0 && bottom == 0) {
        return NONE;
    }
    return new Insets(left, top, right, bottom);
}

AutoPtr<Insets> Insets::Of(
    /* [in] */ IRect* r)
{
    if (r == NULL) return NONE;

    CRect* rObj = (CRect*)r;
    return Of(rObj->mLeft, rObj->mTop, rObj->mRight, rObj->mBottom);
}

Boolean Insets::Equals(
    /* [in] */ IInsets* o)
{
    if ((IInsets*)this == o) return TRUE;

    Int32 bottom, left, right, top;
    o->GetBottom(&bottom);
    o->GetLeft(&left);
    o->GetRight(&right);
    o->GetTop(&top);

    if (mBottom != bottom) return FALSE;
    if (mLeft != left) return FALSE;
    if (mRight != right) return FALSE;
    if (mTop != top) return FALSE;

    return TRUE;
}

ECode Insets::GetLeft(
    /* [out] */ Int32* left)
{
    VALIDATE_NOT_NULL(left);
    *left = mLeft;
    return NOERROR;
}

ECode Insets::GetTop(
    /* [out] */ Int32* top)
{
    VALIDATE_NOT_NULL(top);
    *top = mTop;
    return NOERROR;
}

ECode Insets::GetRight(
    /* [out] */ Int32* right)
{
    VALIDATE_NOT_NULL(right);
    *right = mRight;
    return NOERROR;
}

ECode Insets::GetBottom(
    /* [out] */ Int32* bottom)
{
    VALIDATE_NOT_NULL(bottom);
    *bottom = mBottom;
    return NOERROR;
}

} // namespace Graphics
} // namespace Droid
} // namespace Elastos
