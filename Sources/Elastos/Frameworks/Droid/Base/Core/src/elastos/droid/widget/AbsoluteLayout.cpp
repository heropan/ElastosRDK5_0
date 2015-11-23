
#include "elastos/droid/widget/AbsoluteLayout.h"
#include <elastos/core/Math.h>
#include "elastos/droid/widget/CAbsoluteLayoutLayoutParams.h"

namespace Elastos {
namespace Droid {
namespace Widget {

CAR_INTERFACE_IMPL(AbsoluteLayout, ViewGroup, IAbsoluteLayout)

AbsoluteLayout::AbsoluteLayout()
{}

void AbsoluteLayout::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    Int32 count;
    GetChildCount(&count);

    Int32 maxHeight = 0;
    Int32 maxWidth = 0;

    // Find out how big everyone wants to be
    MeasureChildren(widthMeasureSpec, heightMeasureSpec);

    // Find rightmost and bottom-most child
    for (Int32 i = 0; i < count; i++)
    {
        AutoPtr<IView> child;
        GetChildAt(i, (IView**)&child);

        Int32 visible;
        child->GetVisibility(&visible);
        if (visible != IView::GONE) {
            Int32 childRight;
            Int32 childBottom;

            AutoPtr<IAbsoluteLayoutLayoutParams> lp;
            child->GetLayoutParams((IViewGroupLayoutParams**)&lp);

            Int32 w, h;
            child->GetMeasuredWidth(&w);
            child->GetMeasuredHeight(&h);
            Int32 lx, ly;
            lp->GetX(&lx);
            lp->GetY(&ly);
            childRight = lx + w;
            childBottom = ly + h;

            maxWidth = Elastos::Core::Math::Max(maxWidth, childRight);
            maxHeight = Elastos::Core::Math::Max(maxHeight, childBottom);
        }
    }

    // Account for padding too
    maxWidth += mPaddingLeft + mPaddingRight;
    maxHeight += mPaddingTop + mPaddingBottom;

    // Check against minimum height and width
    maxHeight = Elastos::Core::Math::Max(maxHeight, GetSuggestedMinimumHeight());
    maxWidth = Elastos::Core::Math::Max(maxWidth, GetSuggestedMinimumWidth());

    SetMeasuredDimension(ResolveSizeAndState(maxWidth, widthMeasureSpec, 0),
            ResolveSizeAndState(maxHeight, heightMeasureSpec, 0));
}

/**
 * Returns a set of layout parameters with a width of
 * {@link android.view.ViewGroup.LayoutParams#WRAP_CONTENT},
 * a height of {@link android.view.ViewGroup.LayoutParams#WRAP_CONTENT}
 * and with the coordinates (0, 0).
 */
ECode AbsoluteLayout::GenerateDefaultLayoutParams(
    /* [out] */ IViewGroupLayoutParams** lp)
{
    return CAbsoluteLayoutLayoutParams::New(
            IViewGroupLayoutParams::WRAP_CONTENT,
            IViewGroupLayoutParams::WRAP_CONTENT,
            0, 0, (IAbsoluteLayoutLayoutParams**)lp);
}

//@Override
Boolean AbsoluteLayout::ShouldDelayChildPressedState()
{
    return FALSE;
}

ECode AbsoluteLayout::OnLayout(
    /* [in] */ Boolean changed,
    /* [in] */ Int32 l,
    /* [in] */ Int32 t,
    /* [in] */ Int32 r,
    /* [in] */ Int32 b)
{
    Int32 count;
    GetChildCount(&count);

    for (Int32 i = 0; i < count; i++) {
        AutoPtr<IView> child;
        GetChildAt(i, (IView**)&child);

        Int32 v;
        child->GetVisibility(&v);
        if (v != IView::GONE) {

            AutoPtr<IAbsoluteLayoutLayoutParams> lp;
            child->GetLayoutParams((IViewGroupLayoutParams**)&lp);
            Int32 lx, ly;
            lp->GetX(&lx);
            lp->GetY(&ly);

            Int32 childLeft = mPaddingLeft + lx;
            Int32 childTop = mPaddingTop + ly;

            Int32 w, h;
            child->GetMeasuredWidth(&w);
            child->GetMeasuredHeight(&h);
            child->Layout(childLeft, childTop, childLeft + w, childTop + h);
        }
    }
    return NOERROR;
}

ECode AbsoluteLayout::GenerateLayoutParams(
    /* [in] */ IAttributeSet* attrs,
    /* [out] */ IViewGroupLayoutParams** params)
{
    VALIDATE_NOT_NULL(params);
    AutoPtr<IAbsoluteLayoutLayoutParams> lp;
    AutoPtr<IContext> context;
    GetContext((IContext**)&context);
    FAIL_RETURN(CAbsoluteLayoutLayoutParams::New(context, attrs, (IAbsoluteLayoutLayoutParams**)&lp));
    *params = IViewGroupLayoutParams::Probe(lp);
    REFCOUNT_ADD(*params);
    return NOERROR;
}

// Override to allow type-checking of LayoutParams.
Boolean AbsoluteLayout::CheckLayoutParams(
    /* [in] */ IViewGroupLayoutParams* p)
{
    return IAbsoluteLayoutLayoutParams::Probe(p) != NULL;
}

AutoPtr<IViewGroupLayoutParams> AbsoluteLayout::GenerateLayoutParams(
    /* [in] */ IViewGroupLayoutParams* p)
{
    AutoPtr<IViewGroupLayoutParams> lp;
    CAbsoluteLayoutLayoutParams::New(p, (IAbsoluteLayoutLayoutParams**)&lp);
    return lp;
}

ECode AbsoluteLayout::constructor(
    /* [in] */ IContext* context)
{
    return AbsoluteLayout::constructor(context, NULL);
}

ECode AbsoluteLayout::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return AbsoluteLayout::constructor(context, attrs, 0);
}

ECode AbsoluteLayout::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return ViewGroup::constructor(context, attrs, defStyle);
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
