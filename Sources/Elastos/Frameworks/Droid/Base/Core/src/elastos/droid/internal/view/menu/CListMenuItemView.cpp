
#include "elastos/droid/internal/view/menu/CListMenuItemView.h"

using Elastos::Droid::View::EIID_ViewGroup;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace View {
namespace Menu {

IVIEW_METHODS_IMPL(CListMenuItemView, ListMenuItemView)
IVIEWGROUP_METHODS_IMPL(CListMenuItemView, ListMenuItemView)
IVIEWPARENT_METHODS_IMPL(CListMenuItemView, ListMenuItemView)
IVIEWMANAGER_METHODS_IMPL(CListMenuItemView, ListMenuItemView)
IDRAWABLECALLBACK_METHODS_IMPL(CListMenuItemView, ListMenuItemView)
IKEYEVENTCALLBACK_METHODS_IMPL(CListMenuItemView, ListMenuItemView)
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CListMenuItemView, ListMenuItemView)

ECode CListMenuItemView::constructor(
    /* [in] */ IContext* ctx,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return ListMenuItemView::Init(ctx, attrs, defStyle);
}

PInterface CListMenuItemView::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    else if (riid == EIID_ViewGroup){
        return reinterpret_cast<PInterface>((ViewGroup*)this);
    }

    return _CListMenuItemView::Probe(riid);
}

ECode CListMenuItemView::Initialize(
    /* [in] */ IMenuItemImpl* itemData,
    /* [in] */ Int32 menuType)
{
    return ListMenuItemView::Initialize(itemData, menuType);
}

ECode CListMenuItemView::SetForceShowIcon(
    /* [in] */ Boolean forceShow)
{
    return ListMenuItemView::SetForceShowIcon(forceShow);
}

ECode CListMenuItemView::GetItemData(
    /* [out] */ IMenuItemImpl** itemData)
{
    VALIDATE_NOT_NULL(itemData);

    AutoPtr<IMenuItemImpl> item = ListMenuItemView::GetItemData();
    *itemData = item.Get();
    REFCOUNT_ADD(*itemData);
    return NOERROR;
}

ECode CListMenuItemView::SetTitle(
    /* [in] */ ICharSequence* title)
{
    return ListMenuItemView::SetTitle(title);
}

ECode CListMenuItemView::SetEnabled(
    /* [in] */ Boolean enabled)
{
    return ListMenuItemView::SetEnabled(enabled);
}

ECode CListMenuItemView::SetCheckable(
    /* [in] */ Boolean checkable)
{
    return ListMenuItemView::SetCheckable(checkable);
}

ECode CListMenuItemView::SetChecked(
    /* [in] */ Boolean checked)
{
    return ListMenuItemView::SetChecked(checked);
}

ECode CListMenuItemView::SetShortcut(
    /* [in] */ Boolean showShortcut,
    /* [in] */ Char32 shortcutKey)
{
    return ListMenuItemView::SetShortcut(showShortcut, shortcutKey);
}

ECode CListMenuItemView::SetIcon(
    /* [in] */ IDrawable* icon)
{
    return ListMenuItemView::SetIcon(icon);
}

ECode CListMenuItemView::PrefersCondensedTitle(
    /* [out] */ Boolean* prefer)
{
    VALIDATE_NOT_NULL(prefer);

    *prefer = ListMenuItemView::PrefersCondensedTitle();
    return NOERROR;
}

ECode CListMenuItemView::ShowsIcon(
    /* [out] */ Boolean* show)
{
    VALIDATE_NOT_NULL(show);

    *show = ListMenuItemView::ShowsIcon();
    return NOERROR;
}

ECode CListMenuItemView::IsBaselineAligned(
    /* [out] */  Boolean* baselineAligned)
{
    VALIDATE_NOT_NULL(baselineAligned);

    *baselineAligned = ListMenuItemView::IsBaselineAligned();
    return NOERROR;
}

ECode CListMenuItemView::SetBaselineAligned(
    /* [in] */ Boolean baselineAligned)
{
    return ListMenuItemView::SetBaselineAligned(baselineAligned);
}

ECode CListMenuItemView::GetBaselineAlignedChildIndex(
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);

    *index = ListMenuItemView::GetBaselineAlignedChildIndex();
    return NOERROR;
}

ECode CListMenuItemView::SetBaselineAlignedChildIndex(
    /* [in] */ Int32 index)
{
    return ListMenuItemView::SetBaselineAlignedChildIndex(index);
}

ECode CListMenuItemView::GetWeightSum(
    /* [out] */ Float* weightSum)
{
    VALIDATE_NOT_NULL(weightSum);

    *weightSum = ListMenuItemView::GetWeightSum();
    return NOERROR;
}

ECode CListMenuItemView::SetWeightSum(
    /* [in] */ Float weightSum)
{
    return ListMenuItemView::SetWeightSum(weightSum);
}

ECode CListMenuItemView::SetOrientation(
    /* [in] */ Int32 orientation)
{
    return ListMenuItemView::SetOrientation(orientation);
}

ECode CListMenuItemView::GetOrientation(
    /* [out] */ Int32* orientation)
{
    VALIDATE_NOT_NULL(orientation);

    *orientation = ListMenuItemView::GetOrientation();
    return NOERROR;
}

ECode CListMenuItemView::SetGravity(
    /* [in] */ Int32 gravity)
{
    return ListMenuItemView::SetGravity(gravity);
}

ECode CListMenuItemView::SetHorizontalGravity(
    /* [in] */ Int32 gravity)
{
    return ListMenuItemView::SetHorizontalGravity(gravity);
}

ECode CListMenuItemView::SetVerticalGravity(
    /* [in] */ Int32 gravity)
{
    return ListMenuItemView::SetVerticalGravity(gravity);
}

ECode CListMenuItemView::SetShowDividers(
    /* [in] */ Int32 showDividers)
{
    return ListMenuItemView::SetShowDividers(showDividers);
}

ECode CListMenuItemView::GetShowDividers(
    /* [out] */ Int32* showDividers)
{
    assert(showDividers != NULL);
    *showDividers = ListMenuItemView::GetShowDividers();
    return NOERROR;
}

ECode CListMenuItemView::GetDividerDrawable(
    /* [out] */ IDrawable** divider)
{
    assert(divider != NULL);
    AutoPtr<IDrawable> temp = ListMenuItemView::GetDividerDrawable();
    *divider = temp;
    REFCOUNT_ADD(*divider);

    return NOERROR;
}

ECode CListMenuItemView::SetDividerDrawable(
    /* [in] */ IDrawable* divider)
{
    return ListMenuItemView::SetDividerDrawable(divider);
}

ECode CListMenuItemView::SetDividerPadding(
    /* [in] */ Int32 padding)
{
    return ListMenuItemView::SetDividerPadding(padding);
}

ECode CListMenuItemView::GetDividerPadding(
    /* [out] */ Int32* padding)
{
    assert(padding != NULL);
    *padding = ListMenuItemView::GetDividerPadding();
    return NOERROR;
}

ECode CListMenuItemView::GetDividerWidth(
    /* [out] */ Int32* width)
{
    assert(width != NULL);
    *width = ListMenuItemView::GetDividerWidth();
    return NOERROR;
}

ECode CListMenuItemView::IsMeasureWithLargestChildEnabled(
    /* [out] */ Boolean* enabled)
{
    assert(enabled != NULL);
    *enabled = ListMenuItemView::IsMeasureWithLargestChildEnabled();
    return NOERROR;
}

ECode CListMenuItemView::SetMeasureWithLargestChildEnabled(
    /* [in] */ Boolean enabled)
{
    return ListMenuItemView::SetMeasureWithLargestChildEnabled(enabled);
}

} // namespace Menu
} // namespace View
} // namespace Internal
} // namespace Droid
} // namespace Elastos
