
#include "widget/CLinearLayout.h"

using Elastos::Droid::View::EIID_View;
using Elastos::Droid::View::EIID_ViewGroup;

namespace Elastos{
namespace Droid{
namespace Widget{

IVIEW_METHODS_IMPL(CLinearLayout, LinearLayout)
IVIEWGROUP_METHODS_IMPL(CLinearLayout, LinearLayout)
IVIEWPARENT_METHODS_IMPL(CLinearLayout, LinearLayout)
IVIEWMANAGER_METHODS_IMPL(CLinearLayout, LinearLayout)
IDRAWABLECALLBACK_METHODS_IMPL(CLinearLayout, LinearLayout)
IKEYEVENTCALLBACK_METHODS_IMPL(CLinearLayout, LinearLayout)
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CLinearLayout, LinearLayout)


PInterface CLinearLayout::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    else if (riid == EIID_ViewGroup) {
        return reinterpret_cast<PInterface>((ViewGroup*)this);
    }
    return _CLinearLayout::Probe(riid);
}

ECode CLinearLayout::constructor(
    /* [in] */ IContext* context)
{
    return LinearLayout::Init(context);
}

ECode CLinearLayout::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return LinearLayout::Init(context, attrs);
}

ECode CLinearLayout::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return LinearLayout::Init(context, attrs, defStyle);
}

ECode CLinearLayout::SetShowDividers(
    /* [in] */ Int32 showDividers)
{
    return LinearLayout::SetShowDividers(showDividers);
}

ECode CLinearLayout::GetShowDividers(
    /* [out] */ Int32* showDividers)
{
    VALIDATE_NOT_NULL(showDividers);
    *showDividers = LinearLayout::GetShowDividers();
    return NOERROR;
}

ECode CLinearLayout::GetDividerDrawable(
    /* [out] */ IDrawable** divider)
{
    VALIDATE_NOT_NULL(divider);
    AutoPtr<IDrawable> d = LinearLayout::GetDividerDrawable();
    *divider = d;
    REFCOUNT_ADD(*divider);
    return NOERROR;
}

ECode CLinearLayout::SetDividerDrawable(
    /* [in] */ IDrawable* divider)
{
    return LinearLayout::SetDividerDrawable(divider);
}

ECode CLinearLayout::SetDividerPadding(
    /* [in] */ Int32 padding)
{
    return LinearLayout::SetDividerPadding(padding);
}

ECode CLinearLayout::GetDividerPadding(
    /* [out] */ Int32* padding)
{
    VALIDATE_NOT_NULL(padding);
    *padding = LinearLayout::GetDividerPadding();
    return NOERROR;
}

ECode CLinearLayout::GetDividerWidth(
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);
    *width = LinearLayout::GetDividerWidth();
    return NOERROR;
}

ECode CLinearLayout::IsBaselineAligned(
    /* [out] */ Boolean* baselineAligned)
{
    VALIDATE_NOT_NULL(baselineAligned);
    *baselineAligned = LinearLayout::IsBaselineAligned();
    return NOERROR;
}

ECode CLinearLayout::SetBaselineAligned(
    /* [in] */ Boolean baselineAligned)
{
    return LinearLayout::SetBaselineAligned(baselineAligned);
}

ECode CLinearLayout::IsMeasureWithLargestChildEnabled(
    /* [out] */ Boolean* enabled)
{
    VALIDATE_NOT_NULL(enabled);
    *enabled = LinearLayout::IsMeasureWithLargestChildEnabled();
    return NOERROR;
}

ECode CLinearLayout::SetMeasureWithLargestChildEnabled(
    /* [in] */ Boolean enabled)
{
    return LinearLayout::SetMeasureWithLargestChildEnabled(enabled);
}

ECode CLinearLayout::GetBaselineAlignedChildIndex(
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);
    *index = LinearLayout::GetBaselineAlignedChildIndex();
    return NOERROR;
}

ECode CLinearLayout::SetBaselineAlignedChildIndex(
    /* [in] */ Int32 index)
{
    return LinearLayout::SetBaselineAlignedChildIndex(index);
}

ECode CLinearLayout::GetWeightSum(
    /* [out] */ Float* weightSum)
{
    VALIDATE_NOT_NULL(weightSum);
    *weightSum = LinearLayout::GetWeightSum();
    return NOERROR;
}

ECode CLinearLayout::SetWeightSum(
    /* [in] */ Float weightSum)
{
    return LinearLayout::SetWeightSum(weightSum);
}

ECode CLinearLayout::SetOrientation(
    /* [in] */ Int32 orientation)
{
    return LinearLayout::SetOrientation(orientation);
}

ECode CLinearLayout::GetOrientation(
    /* [out] */ Int32* orientation)
{
    VALIDATE_NOT_NULL(orientation);
    *orientation = LinearLayout::GetOrientation();
    return NOERROR;
}

ECode CLinearLayout::SetGravity(
    /* [in] */ Int32 gravity)
{
    return LinearLayout::SetGravity(gravity);
}

ECode CLinearLayout::SetHorizontalGravity(
    /* [in] */ Int32 horizontalGravity)
{
    return LinearLayout::SetHorizontalGravity(horizontalGravity);
}

ECode CLinearLayout::SetVerticalGravity(
    /* [in] */ Int32 verticalGravity)
{
    return LinearLayout::SetVerticalGravity(verticalGravity);
}


}// namespace Widget
}// namespace Droid
}// namespace Elastos
