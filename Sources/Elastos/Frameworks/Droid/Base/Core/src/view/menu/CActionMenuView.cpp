
#include "view/menu/CActionMenuView.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Menu {

IVIEW_METHODS_IMPL(CActionMenuView, ActionMenuView);
IVIEWGROUP_METHODS_IMPL(CActionMenuView, ActionMenuView);
IVIEWPARENT_METHODS_IMPL(CActionMenuView, ActionMenuView);
IVIEWMANAGER_METHODS_IMPL(CActionMenuView, ActionMenuView);
IDRAWABLECALLBACK_METHODS_IMPL(CActionMenuView, ActionMenuView);
IKEYEVENTCALLBACK_METHODS_IMPL(CActionMenuView, ActionMenuView);
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CActionMenuView, ActionMenuView);

PInterface CActionMenuView::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    else if (riid == EIID_ViewGroup) {
        return reinterpret_cast<PInterface>((ViewGroup*)this);
    }
    else if (riid == EIID_ActionMenuView) {
        return reinterpret_cast<PInterface>((ActionMenuView*)this);
    }
    return _CActionMenuView::Probe(riid);
}

ECode CActionMenuView::constructor(
    /* [in] */ IContext* context)
{
    return ActionMenuView::Init(context);
}

ECode CActionMenuView::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return ActionMenuView::Init(context, attrs);
}

ECode CActionMenuView::SetPresenter(
        /* [in] */ IActionMenuPresenter* presenter)
{
    return ActionMenuView::SetPresenter(presenter);
}

ECode CActionMenuView::IsExpandedFormat(
        /* [out] */ Boolean* res)
{
    *res = ActionMenuView::IsExpandedFormat();
    return NOERROR;
}

ECode CActionMenuView::SetMaxItemHeight(
        /* [in] */ Int32 maxItemHeight)
{
    return ActionMenuView::SetMaxItemHeight(maxItemHeight);
}

ECode CActionMenuView::IsOverflowReserved(
        /* [out] */ Boolean* res)
{
    *res = ActionMenuView::IsOverflowReserved();
    return NOERROR;
}

ECode CActionMenuView::SetOverflowReserved(
        /* [in] */ Boolean reserveOverflow)
{
    return ActionMenuView::SetOverflowReserved(reserveOverflow);
}

ECode CActionMenuView::GenerateOverflowButtonLayoutParams(
        /* [out] */ IViewGroupLayoutParams** pm)
{
    AutoPtr<IViewGroupLayoutParams> pmTemp = ActionMenuView::GenerateOverflowButtonLayoutParams();
    *pm = pmTemp;
    REFCOUNT_ADD(*pm);
    return NOERROR;
}

ECode CActionMenuView::InvokeItem(
    /* [in] */ IMenuItemImpl* item,
    /* [out] */ Boolean* result)
{
    return ActionMenuView::InvokeItem(item, result);
}

ECode CActionMenuView::Initialize(
    /* [in] */ IMenuBuilder* menu)
{
    return ActionMenuView::Initialize(menu);
}

ECode CActionMenuView::GetWindowAnimations(
    /* [out] */ Int32* animations)
{
    return ActionMenuView::GetWindowAnimations(animations);
}

ECode CActionMenuView::SetShowDividers(
    /* [in] */ Int32 showDividers)
{
    return ActionMenuView::SetShowDividers(showDividers);
}

ECode CActionMenuView::GetShowDividers(
    /* [out] */ Int32* showDividers)
{
    VALIDATE_NOT_NULL(showDividers);
    *showDividers = ActionMenuView::GetShowDividers();
    return NOERROR;
}

ECode CActionMenuView::GetDividerDrawable(
    /* [out] */ IDrawable** divider)
{
    VALIDATE_NOT_NULL(divider);
    AutoPtr<IDrawable> d = ActionMenuView::GetDividerDrawable();
    *divider = d;
    REFCOUNT_ADD(*divider);
    return NOERROR;
}

ECode CActionMenuView::SetDividerDrawable(
    /* [in] */ IDrawable* divider)
{
    return ActionMenuView::SetDividerDrawable(divider);
}

ECode CActionMenuView::SetDividerPadding(
    /* [in] */ Int32 padding)
{
    return ActionMenuView::SetDividerPadding(padding);
}

ECode CActionMenuView::GetDividerPadding(
    /* [out] */ Int32* padding)
{
    VALIDATE_NOT_NULL(padding);
    *padding = ActionMenuView::GetDividerPadding();
    return NOERROR;
}

ECode CActionMenuView::GetDividerWidth(
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);
    *width = ActionMenuView::GetDividerWidth();
    return NOERROR;
}

ECode CActionMenuView::IsBaselineAligned(
    /* [out] */ Boolean* baselineAligned)
{
    VALIDATE_NOT_NULL(baselineAligned);
    *baselineAligned = ActionMenuView::IsBaselineAligned();
    return NOERROR;
}

ECode CActionMenuView::SetBaselineAligned(
    /* [in] */ Boolean baselineAligned)
{
    return ActionMenuView::SetBaselineAligned(baselineAligned);
}

ECode CActionMenuView::IsMeasureWithLargestChildEnabled(
    /* [out] */ Boolean* enabled)
{
    VALIDATE_NOT_NULL(enabled);
    *enabled = ActionMenuView::IsMeasureWithLargestChildEnabled();
    return NOERROR;
}

ECode CActionMenuView::SetMeasureWithLargestChildEnabled(
    /* [in] */ Boolean enabled)
{
    return ActionMenuView::SetMeasureWithLargestChildEnabled(enabled);
}

ECode CActionMenuView::GetBaselineAlignedChildIndex(
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);
    *index = ActionMenuView::GetBaselineAlignedChildIndex();
    return NOERROR;
}

ECode CActionMenuView::SetBaselineAlignedChildIndex(
    /* [in] */ Int32 index)
{
    return ActionMenuView::SetBaselineAlignedChildIndex(index);
}

ECode CActionMenuView::GetWeightSum(
    /* [out] */ Float* weightSum)
{
    VALIDATE_NOT_NULL(weightSum);
    *weightSum = ActionMenuView::GetWeightSum();
    return NOERROR;
}

ECode CActionMenuView::SetWeightSum(
    /* [in] */ Float weightSum)
{
    return ActionMenuView::SetWeightSum(weightSum);
}

ECode CActionMenuView::SetOrientation(
    /* [in] */ Int32 orientation)
{
    return ActionMenuView::SetOrientation(orientation);
}

ECode CActionMenuView::GetOrientation(
    /* [out] */ Int32* orientation)
{
    VALIDATE_NOT_NULL(orientation);
    *orientation = ActionMenuView::GetOrientation();
    return NOERROR;
}

ECode CActionMenuView::SetGravity(
    /* [in] */ Int32 gravity)
{
    return ActionMenuView::SetGravity(gravity);
}

ECode CActionMenuView::SetHorizontalGravity(
    /* [in] */ Int32 horizontalGravity)
{
    return ActionMenuView::SetHorizontalGravity(horizontalGravity);
}

ECode CActionMenuView::SetVerticalGravity(
    /* [in] */ Int32 verticalGravity)
{
    return ActionMenuView::SetVerticalGravity(verticalGravity);
}

} // namespace Menu
} // namespace View
} // namespace Droid
} // namespace Elastos
