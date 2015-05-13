
#include "widget/AbsoluteLayout.h"
#include <elastos/Math.h>
#include "widget/CAbsoluteLayoutLayoutParams.h"

namespace Elastos{
namespace Droid{
namespace Widget{


AbsoluteLayout::AbsoluteLayout()
{}

AbsoluteLayout::AbsoluteLayout(
    /* [in] */ IContext* context)
    : ViewGroup(context)
{}

AbsoluteLayout::AbsoluteLayout(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
    : ViewGroup(context, attrs)
{}

AbsoluteLayout::AbsoluteLayout(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
    : ViewGroup(context, attrs, defStyle)
{}

void AbsoluteLayout::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    Int32 count = GetChildCount();

    Int32 maxHeight = 0;
    Int32 maxWidth = 0;

    // Find out how big everyone wants to be
    MeasureChildren(widthMeasureSpec, heightMeasureSpec);

    // Find rightmost and bottom-most child
    for (Int32 i = 0; i < count; i++)
    {
        AutoPtr<IView> child = GetChildAt(i);

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

void AbsoluteLayout::OnLayout(
    /* [in] */ Boolean changed,
    /* [in] */ Int32 l,
    /* [in] */ Int32 t,
    /* [in] */ Int32 r,
    /* [in] */ Int32 b)
{
    Int32 count = GetChildCount();

    for (Int32 i = 0; i < count; i++) {
        AutoPtr<IView> child = GetChildAt(i);

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
}

ECode AbsoluteLayout::GenerateLayoutParams(
    /* [in] */ IAttributeSet* attrs,
    /* [out] */ IViewGroupLayoutParams** params)
{
    VALIDATE_NOT_NULL(params);
    AutoPtr<IAbsoluteLayoutLayoutParams> lp;
    FAIL_RETURN(CAbsoluteLayoutLayoutParams::New(GetContext(), attrs, (IAbsoluteLayoutLayoutParams**)&lp));
    *params = IViewGroupLayoutParams::Probe(lp);
    INTERFACE_ADDREF(*params);
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

ECode AbsoluteLayout::Init(
    /* [in] */ IContext* context)
{
    return ViewGroup::Init(context);
}

ECode AbsoluteLayout::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return ViewGroup::Init(context, attrs);
}

ECode AbsoluteLayout::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return ViewGroup::Init(context, attrs, defStyle);
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
