
#include "elastos/droid/view/CWindowInsets.h"
#include "elastos/droid/graphics/CRect.h"
#include <elastos/core/StringBuilder.h>

using Elastos::Droid::Graphics::CRect;
using Elastos::Core::StringBuilder;

namespace Elastos {
namespace Droid {
namespace View {

AutoPtr<IWindowInsets> CWindowInsets::CONSUMED = InitStatic();
AutoPtr<IRect> CWindowInsets::EMPTY_RECT;

CAR_OBJECT_IMPL(CWindowInsets);
CAR_INTERFACE_IMPL(CWindowInsets, Object, IWindowInsets);
CWindowInsets::CWindowInsets()
    : mIsRound(FALSE)
    , mSystemWindowInsetsConsumed(FALSE)
    , mWindowDecorInsetsConsumed(FALSE)
    , mStableInsetsConsumed(FALSE)
{}

ECode CWindowInsets::constructor(
    /* [in] */ IRect* systemWindowInsets,
    /* [in] */ IRect* windowDecorInsets,
    /* [in] */ IRect* stableInsets,
    /* [in] */ Boolean isRound)
{
    mSystemWindowInsetsConsumed = systemWindowInsets == NULL;
    mSystemWindowInsets = mSystemWindowInsetsConsumed ? EMPTY_RECT.Get() : systemWindowInsets;

    mWindowDecorInsetsConsumed = windowDecorInsets == NULL;
    mWindowDecorInsets = mWindowDecorInsetsConsumed ? EMPTY_RECT.Get() : windowDecorInsets;

    mStableInsetsConsumed = stableInsets == NULL;
    mStableInsets = mStableInsetsConsumed ? EMPTY_RECT.Get() : stableInsets;

    mIsRound = isRound;
    return NOERROR;
}

ECode CWindowInsets::constructor(
    /* [in] */ IWindowInsets* _src)
{
    CWindowInsets* src = (CWindowInsets*)_src;
    mSystemWindowInsets = src->mSystemWindowInsets;
    mWindowDecorInsets = src->mWindowDecorInsets;
    mStableInsets = src->mStableInsets;
    mSystemWindowInsetsConsumed = src->mSystemWindowInsetsConsumed;
    mWindowDecorInsetsConsumed = src->mWindowDecorInsetsConsumed;
    mStableInsetsConsumed = src->mStableInsetsConsumed;
    mIsRound = src->mIsRound;
    return NOERROR;
}

ECode CWindowInsets::constructor(
    /* [in] */ IRect* systemWindowInsets)
{
    return constructor(systemWindowInsets, NULL, NULL, FALSE);
}

ECode CWindowInsets::GetSystemWindowInsets(
    /* [out] */ IRect** rect)
{
    VALIDATE_NOT_NULL(rect);
    if (mTempRect == NULL) {
        CRect::New((IRect**)&mTempRect);
    }
    if (mSystemWindowInsets != NULL) {
        mTempRect->Set(mSystemWindowInsets);
    } else {
        // If there were no system window insets, this is just empty.
        mTempRect->SetEmpty();
    }
    *rect = mTempRect;
    REFCOUNT_ADD(*rect);
    return NOERROR;
}

ECode CWindowInsets::GetSystemWindowInsetLeft(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    return mSystemWindowInsets->GetLeft(result);
}

ECode CWindowInsets::GetSystemWindowInsetTop(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    return mSystemWindowInsets->GetTop(result);
}

ECode CWindowInsets::GetSystemWindowInsetRight(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    return mSystemWindowInsets->GetRight(result);
}

ECode CWindowInsets::GetSystemWindowInsetBottom(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    return mSystemWindowInsets->GetBottom(result);
}

ECode CWindowInsets::GetWindowDecorInsetLeft(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    return mWindowDecorInsets->GetLeft(result);
}

ECode CWindowInsets::GetWindowDecorInsetTop(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    return mWindowDecorInsets->GetTop(result);
}

ECode CWindowInsets::GetWindowDecorInsetRight(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    return mWindowDecorInsets->GetRight(result);
}

ECode CWindowInsets::GetWindowDecorInsetBottom(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    return mWindowDecorInsets->GetBottom(result);
}

ECode CWindowInsets::HasSystemWindowInsets(
    /* [out] */ Boolean* has)
{
    VALIDATE_NOT_NULL(has);
    Int32 left = 0, top = 0, right = 0, bottom = 0;
    mSystemWindowInsets->Get(&left, &top, &right, &bottom);
    *has = left != 0 || top != 0 ||
            right != 0 || bottom != 0;
    return NOERROR;
}

ECode CWindowInsets::HasWindowDecorInsets(
    /* [out] */ Boolean* has)
{
    VALIDATE_NOT_NULL(has);
    Int32 left = 0, top = 0, right = 0, bottom = 0;
    mWindowDecorInsets->Get(&left, &top, &right, &bottom);
    *has = left != 0 || top != 0 ||
            right != 0 || bottom != 0;
    return NOERROR;
}

ECode CWindowInsets::HasInsets(
    /* [out] */ Boolean* has)
{
    VALIDATE_NOT_NULL(has);
    Boolean value = FALSE;
    *has = (HasSystemWindowInsets(has), *has) || (HasWindowDecorInsets(&value), value);
    return NOERROR;
}

ECode CWindowInsets::IsConsumed(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mSystemWindowInsetsConsumed && mWindowDecorInsetsConsumed && mStableInsetsConsumed;
    return NOERROR;
}

ECode CWindowInsets::IsRound(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mIsRound;
    return NOERROR;
}

ECode CWindowInsets::ConsumeSystemWindowInsets(
    /* [out] */ IWindowInsets** insets)
{
    VALIDATE_NOT_NULL(insets);
    AutoPtr<IWindowInsets> result;
    CWindowInsets::New(this, (IWindowInsets**)&result);
    ((CWindowInsets*)result.Get())->mSystemWindowInsets = EMPTY_RECT;
    ((CWindowInsets*)result.Get())->mSystemWindowInsetsConsumed = TRUE;
    *insets = result;
    REFCOUNT_ADD(*insets);
    return NOERROR;
}

ECode CWindowInsets::ConsumeSystemWindowInsets(
    /* [in] */ Boolean left,
    /* [in] */ Boolean top,
    /* [in] */ Boolean right,
    /* [in] */ Boolean bottom,
    /* [out] */ IWindowInsets** insets)
{
    VALIDATE_NOT_NULL(insets);
    if (left || top || right || bottom) {
        AutoPtr<IWindowInsets> result;
        CWindowInsets::New(this, (IWindowInsets**)&result);
        Int32 l = 0, t = 0, r = 0, b = 0;
        mSystemWindowInsets->Get(&l, &t, &r, &b);
        CRect::New(
            left ? 0 : l,
            top ? 0 : t,
            right ? 0 : r,
            bottom ? 0 : b, (IRect**)&((CWindowInsets*)result.Get())->mSystemWindowInsets);
        *insets = result;
        REFCOUNT_ADD(*insets);
        return NOERROR;
    }

    *insets = this;
    REFCOUNT_ADD(*insets);
    return NOERROR;
}

ECode CWindowInsets::ReplaceSystemWindowInsets(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom,
    /* [out] */ IWindowInsets** insets)
{
    VALIDATE_NOT_NULL(insets);
    AutoPtr<IWindowInsets> result;
    CWindowInsets::New(this, (IWindowInsets**)&result);
    CRect::New(left, top, right, bottom, (IRect**)&((CWindowInsets*)result.Get())->mSystemWindowInsets);
    *insets = result;
    REFCOUNT_ADD(*insets);
    return NOERROR;
}

ECode CWindowInsets::ReplaceSystemWindowInsets(
    /* [in] */ IRect* systemWindowInsets,
    /* [out] */ IWindowInsets** insets)
{
    AutoPtr<IWindowInsets> result;
    CWindowInsets::New(this, (IWindowInsets**)&result);
    CRect::New(systemWindowInsets, (IRect**)&((CWindowInsets*)result.Get())->mSystemWindowInsets);
    *insets = result;
    REFCOUNT_ADD(*insets);
    return NOERROR;
}

ECode CWindowInsets::ConsumeWindowDecorInsets(
    /* [out] */ IWindowInsets** insets)
{
    VALIDATE_NOT_NULL(insets);
    AutoPtr<IWindowInsets> result;
    CWindowInsets::New(this, (IWindowInsets**)&insets);
    ((CWindowInsets*)result.Get())->mWindowDecorInsets->Set(0, 0, 0, 0);
    ((CWindowInsets*)result.Get())->mWindowDecorInsetsConsumed = TRUE;
    *insets = result;
    REFCOUNT_ADD(*insets);
    return NOERROR;
}

ECode CWindowInsets::ConsumeWindowDecorInsets(
    /* [in] */ Boolean left,
    /* [in] */ Boolean top,
    /* [in] */ Boolean right,
    /* [in] */ Boolean bottom,
    /* [out] */ IWindowInsets** insets)
{
    VALIDATE_NOT_NULL(insets);
    if (left || top || right || bottom) {
        AutoPtr<IWindowInsets> result;
        CWindowInsets::New(this, (IWindowInsets**)&result);
        Int32 l = 0, t = 0, r = 0, b = 0;
        mWindowDecorInsets->Get(&l, &t, &r, &b);
        CRect::New(left ? 0 : l,
            top ? 0 : t,
            right ? 0 : r,
            bottom ? 0 : b, (IRect**)&((CWindowInsets*)result.Get())->mWindowDecorInsets);
        *insets = result;
        REFCOUNT_ADD(*insets);
        return NOERROR;
    }
    *insets = this;
    REFCOUNT_ADD(*insets);
    return NOERROR;
}

ECode CWindowInsets::ReplaceWindowDecorInsets(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom,
    /* [out] */ IWindowInsets** insets)
{
    VALIDATE_NOT_NULL(insets);
    AutoPtr<IWindowInsets> result;
    CWindowInsets::New(this, (IWindowInsets**)&result);
    CRect::New(left, top, right, bottom, (IRect**)&((CWindowInsets*)result.Get())->mWindowDecorInsets);
    *insets = result;
    REFCOUNT_ADD(*insets);
    return NOERROR;
}

ECode CWindowInsets::GetStableInsetTop(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    return mStableInsets->GetTop(result);
}

ECode CWindowInsets::GetStableInsetLeft(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    return mStableInsets->GetLeft(result);
}

ECode CWindowInsets::GetStableInsetRight(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    return mStableInsets->GetRight(result);
}

ECode CWindowInsets::GetStableInsetBottom(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    return mStableInsets->GetBottom(result);
}

ECode CWindowInsets::HasStableInsets(
    /* [out] */ Boolean* has)
{
    VALIDATE_NOT_NULL(has);
    Int32 left = 0, top = 0, right = 0, bottom = 0;
    mStableInsets->Get(&left, &top, &right, &bottom);
    *has = top != 0 || left != 0 || right != 0 || bottom != 0;
    return NOERROR;
}

ECode CWindowInsets::ConsumeStableInsets(
    /* [out] */ IWindowInsets** insets)
{
    VALIDATE_NOT_NULL(insets);
    AutoPtr<IWindowInsets> result;
    CWindowInsets::New(this, (IWindowInsets**)&result);
    ((CWindowInsets*)result.Get())->mStableInsets = EMPTY_RECT;
    ((CWindowInsets*)result.Get())->mStableInsetsConsumed = TRUE;
    *insets = result;
    REFCOUNT_ADD(*insets);
    return NOERROR;
}

ECode CWindowInsets::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    StringBuilder sb("WindowInsets{systemWindowInsets=");
    Int32 left = 0, top = 0, right = 0, bottom = 0;
    mSystemWindowInsets->Get(&left, &top, &right, &bottom);
    sb.Append("left:");
    sb.Append(left);
    sb.Append(", top:");
    sb.Append(top);
    sb.Append(", right:");
    sb.Append(right);
    sb.Append(", bottom:");
    sb.Append(bottom);

    mWindowDecorInsets->Get(&left, &top, &right, &bottom);
    sb.Append("; windowDecorInsets=left:");
    sb.Append(left);
    sb.Append(", top:");
    sb.Append(top);
    sb.Append(", right:");
    sb.Append(right);
    sb.Append(", bottom:");
    sb.Append(bottom);

    mStableInsets->Get(&left, &top, &right, &bottom);
    sb.Append("; stableInsets=left:");
    sb.Append(left);
    sb.Append(", top:");
    sb.Append(top);
    sb.Append(", right:");
    sb.Append(right);
    sb.Append(", bottom:");
    sb.Append(bottom);

    Boolean round = FALSE;
    if (round) {
        sb.Append(" round}");
    }
    else {
        sb.Append("}");
    }

    return sb.ToString(str);
}

AutoPtr<IWindowInsets> CWindowInsets::InitStatic()
{
    AutoPtr<CWindowInsets> wi;
    CWindowInsets::NewByFriend(NULL, NULL, NULL, FALSE, (CWindowInsets**)&wi);
    CRect::New(0, 0, 0, 0, (IRect**)&EMPTY_RECT);
    return wi;
}

}// namespace View
}// namespace Droid
}// namespace Elastos
