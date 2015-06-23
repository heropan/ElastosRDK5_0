
#include "ext/frameworkext.h"
#include "graphics/CRect.h"
#include <elastos/core/StringBuilder.h>
#include <elastos/core/Math.h>

using Elastos::Core::StringBuilder;

namespace Elastos {
namespace Droid {
namespace Graphics {

ECode CRect::constructor()
{
    mLeft = mRight = mTop = mBottom = 0;

    return NOERROR;
}

ECode CRect::constructor(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    mLeft = left;
    mTop = top;
    mRight = right;
    mBottom = bottom;

    return NOERROR;
}

ECode CRect::constructor(
    /* [in] */ IRect* r)
{
    if (r == NULL) {
        mLeft = 0;
        mTop = 0;
        mRight = 0;
        mBottom = 0;
    }
    else {
        CRect* temp = (CRect*)r;
        mLeft = temp->mLeft;
        mTop = temp->mTop;
        mRight = temp->mRight;
        mBottom = temp->mBottom;
    }
    return NOERROR;
}

ECode CRect::Equals(
    /* [in] */ IRect* r,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;

    if (r != NULL) {
        Int32 l, t, rt, b;
        r->Get(&l, &t, &rt, &b);
        *result = mLeft == l && mTop == t && mRight == rt && mBottom == b;
    }
    return NOERROR;
}

ECode CRect::Equals(
    /* [in] */ IInterface* r,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return Equals(IRect::Probe(r), result);
}

ECode CRect::GetHashCode(
    /* [out] */ Int32* hash)
{
    VALIDATE_NOT_NULL(hash);
    Int32 result = mLeft;
    result = 31 * result + mTop;
    result = 31 * result + mRight;
    result = 31 * result + mBottom;
    *hash = result;
    return NOERROR;
}

ECode CRect::ToString(
    /* [out] */ String* str)
{
    StringBuilder sb(32);
    sb += "Rect(";
    sb += mLeft; sb += ", ";
    sb += mTop; sb += ", ";
    sb += mRight; sb += ", ";
    sb += mBottom; sb += ", ";
    sb.ToString(str);
    return NOERROR;
}

ECode CRect::ToShortString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    StringBuilder sb(32);
    sb.AppendString(String("Rect(")); sb.AppendInt32(mLeft); sb.AppendString(String(", "));
    sb.AppendInt32(mTop); sb.AppendString(String(" - ")); sb.AppendInt32(mRight);
    sb.AppendString(String(", ")); sb.AppendInt32(mBottom); sb.AppendString(String(")"));
    return sb.ToString(str);
}

ECode CRect::FlattenToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    StringBuilder sb(32);
    // WARNING: Do not change the format of this string, it must be
    // preserved because Rects are saved in this flattened format.
    sb += mLeft;
    sb += " ";
    sb += mTop;
    sb += " ";
    sb += mRight;
    sb += " ";
    sb += mBottom;
    return sb.ToString(str);
}

ECode CRect::IsEmpty(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);;

    *result = mLeft >= mRight || mTop >= mBottom;
    return NOERROR;
}

ECode CRect::GetWidth(
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);

    *width = mRight - mLeft;
    return NOERROR;
}

ECode CRect::GetHeight(
    /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height)

    *height = mBottom - mTop;
    return NOERROR;
}

ECode CRect::GetCenterX(
    /* [out] */ Int32* x)
{
    VALIDATE_NOT_NULL(x);

    *x = (mLeft + mRight) >> 1;
    return NOERROR;
}

ECode CRect::GetCenterY(
    /* [out] */ Int32* y)
{
    VALIDATE_NOT_NULL(y);

    *y = (mTop + mBottom) >> 1;
    return NOERROR;
}

ECode CRect::GetExactCenterX(
    /* [out] */ Float* x)
{
    VALIDATE_NOT_NULL(x);

    *x = (mLeft + mRight) * 0.5f;
    return NOERROR;
}

ECode CRect::GetExactCenterY(
    /* [out] */ Float* y)
{
    VALIDATE_NOT_NULL(y);

    *y = (mTop + mBottom) * 0.5f;
    return NOERROR;
}

ECode CRect::SetEmpty()
{
    mLeft = mRight = mTop = mBottom = 0;
    return NOERROR;
}

ECode CRect::Set(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    mLeft = left;
    mTop = top;
    mRight = right;
    mBottom = bottom;
    return NOERROR;
}

ECode CRect::Get(
    /* [out] */ Int32* left,
    /* [out] */ Int32* top,
    /* [out] */ Int32* right,
    /* [out] */ Int32* bottom)
{
    if (left) *left = mLeft;
    if (top) *top = mTop;
    if (right) *right = mRight;
    if (bottom) *bottom = mBottom;
    return NOERROR;
}

ECode CRect::Set(
    /* [in] */ IRect* src)
{
    assert(src != NULL);
    VALIDATE_NOT_NULL(src);

    src->GetLeft(&mLeft);
    src->GetTop(&mTop);
    src->GetRight(&mRight);
    src->GetBottom(&mBottom);

    return NOERROR;
}

ECode CRect::Offset(
    /* [in] */ Int32 dx,
    /* [in] */ Int32 dy)
{
    mLeft += dx;
    mTop += dy;
    mRight += dx;
    mBottom += dy;
    return NOERROR;
}

ECode CRect::OffsetTo(
    /* [in] */ Int32 newLeft,
    /* [in] */ Int32 newTop)
{
    mRight += newLeft - mLeft;
    mBottom += newTop - mTop;
    mLeft = newLeft;
    mTop = newTop;
    return NOERROR;
}

ECode CRect::Inset(
    /* [in] */ Int32 dx,
    /* [in] */ Int32 dy)
{
    mLeft += dx;
    mTop += dy;
    mRight -= dx;
    mBottom -= dy;
    return NOERROR;
}

ECode CRect::Contains(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mLeft < mRight && mTop < mBottom  // check for empty first
           && x >= mLeft && x < mRight && y >= mTop && y < mBottom;
    return NOERROR;
}

ECode CRect::Contains(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    // check for empty first
    *result = mLeft < mRight && mTop < mBottom
           // now check for containment
           && mLeft <= left && mTop <= top
           && mRight >= right && mBottom >= bottom;
    return NOERROR;
}

ECode CRect::Contains(
    /* [in] */ IRect* r,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    CRect* temp = (CRect*)r;
    // check for empty first
    *result = mLeft < mRight && mTop < mBottom
           // now check for containment
           && mLeft <= temp->mLeft && mTop <= temp->mTop
           && mRight >= temp->mRight && mBottom >= temp->mBottom;
    return NOERROR;
}

ECode CRect::Intersect(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    if (mLeft < right && left < mRight
        && mTop < bottom && top < mBottom) {
        if (mLeft < left) {
            mLeft = left;
        }
        if (mTop < top) {
            mTop = top;
        }
        if (mRight > right) {
            mRight = right;
        }
        if (mBottom > bottom) {
            mBottom = bottom;
        }
        *result = TRUE;
    }
    else {
        *result = FALSE;
    }
    return NOERROR;
}

ECode CRect::Intersect(
    /* [in] */ IRect* r,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    CRect* temp = (CRect*)r;
    return Intersect(temp->mLeft, temp->mTop, temp->mRight, temp->mBottom, result);
}

ECode CRect::Intersects(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mLeft < right && left < mRight
        && mTop < bottom && top < mBottom;
    return NOERROR;
}

ECode CRect::Intersects(
    /* [in] */ IRect* a,
    /* [in] */ IRect* b,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    CRect* tempA = (CRect*)a;
    CRect* tempB = (CRect*)b;
    *result = tempA->mLeft < tempB->mRight && tempB->mLeft < tempA->mRight
        && tempA->mTop < tempB->mBottom && tempB->mTop < tempA->mBottom;
    return NOERROR;
}

ECode CRect::SetIntersect(
    /* [in] */ IRect* a,
    /* [in] */ IRect* b,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    CRect* tempA = (CRect*)a;
    CRect* tempB = (CRect*)b;
    if (tempA->mLeft < tempB->mRight
            && tempB->mLeft < tempA->mRight
            && tempA->mTop < tempB->mBottom && tempB->mTop < tempA->mBottom) {
        mLeft = Elastos::Core::Math::Max(tempA->mLeft, tempB->mLeft);
        mTop = Elastos::Core::Math::Max(tempA->mTop, tempB->mTop);
        mRight = Elastos::Core::Math::Min(tempA->mRight, tempB->mRight);
        mBottom = Elastos::Core::Math::Min(tempA->mBottom, tempB->mBottom);
        *result = TRUE;
        return NOERROR;
    }
    *result = FALSE;
    return NOERROR;
}

ECode CRect::Union(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    if ((left < right) && (top < bottom)) {
        if ((mLeft < mRight) && (mTop < mBottom)) {
            if (mLeft > left) mLeft = left;
            if (mTop > top) mTop = top;
            if (mRight < right) mRight = right;
            if (mBottom < bottom) mBottom = bottom;
        }
        else {
            mLeft = left;
            mTop = top;
            mRight = right;
            mBottom = bottom;
        }
    }
    return NOERROR;
}

ECode CRect::Union(
    /* [in] */ IRect* r)
{
    CRect* temp = (CRect*)r;
    return Union(temp->mLeft, temp->mTop, temp->mRight, temp->mBottom);
}

ECode CRect::Union(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    if (x < mLeft) {
        mLeft = x;
    }
    else if (x > mRight) {
        mRight = x;
    }
    if (y < mTop) {
        mTop = y;
    }
    else if (y > mBottom) {
        mBottom = y;
    }
    return NOERROR;
}

ECode CRect::Sort()
{
    if (mLeft > mRight) {
        Int32 temp = mLeft;
        mLeft = mRight;
        mRight = temp;
    }
    if (mTop > mBottom) {
        Int32 temp = mTop;
        mTop = mBottom;
        mBottom = temp;
    }
    return NOERROR;
}

ECode CRect::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    dest->WriteInt32(mLeft);
    dest->WriteInt32(mRight);
    dest->WriteInt32(mTop);
    dest->WriteInt32(mBottom);
    return NOERROR;
}

ECode CRect::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    source->ReadInt32(&mLeft);
    source->ReadInt32(&mRight);
    source->ReadInt32(&mTop);
    source->ReadInt32(&mBottom);
    return NOERROR;
}

ECode CRect::Scale(
    /* [in] */ Float scale)
{
    if (scale != 1.0f) {
        mLeft = (Int32)(mLeft * scale + 0.5f);
        mTop = (Int32)(mTop * scale + 0.5f);
        mRight = (Int32)(mRight * scale + 0.5f);
        mBottom = (Int32)(mBottom * scale + 0.5f);
    }
    return NOERROR;
}

ECode CRect::GetTop(
    /* [out] */ Int32* top)
{
    VALIDATE_NOT_NULL(top);
    *top = mTop;
    return NOERROR;
}

ECode CRect::SetTop(
    /* [in] */ Int32 top)
{
    mTop = top;
    return NOERROR;
}

ECode CRect::GetBottom(
    /* [out] */ Int32* bottom)
{
    VALIDATE_NOT_NULL(bottom);
    *bottom = mBottom;
    return NOERROR;
}

ECode CRect::SetBottom(
    /* [in] */ Int32 bottom)
{
    mBottom = bottom;
    return NOERROR;
}

ECode CRect::GetLeft(
    /* [out] */ Int32* left)
{
    VALIDATE_NOT_NULL(left);
    *left = mLeft;
    return NOERROR;
}

ECode CRect::SetLeft(
    /* [in] */ Int32 left)
{
    mLeft = left;
    return NOERROR;
}

ECode CRect::GetRight(
    /* [out] */ Int32* right)
{
    VALIDATE_NOT_NULL(right);
    *right = mRight;
    return NOERROR;
}

ECode CRect::SetRight(
    /* [in] */ Int32 right)
{
    mRight = right;
    return NOERROR;
}

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos
