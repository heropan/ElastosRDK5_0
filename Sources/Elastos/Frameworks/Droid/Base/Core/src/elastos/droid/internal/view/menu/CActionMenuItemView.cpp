
#include "elastos/droid/internal/view/menu/CActionMenuItemView.h"

using Elastos::Droid::Widget::EIID_TextView;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace View {
namespace Menu {

IVIEW_METHODS_IMPL(CActionMenuItemView, ActionMenuItemView);
IDRAWABLECALLBACK_METHODS_IMPL(CActionMenuItemView, ActionMenuItemView);
IKEYEVENTCALLBACK_METHODS_IMPL(CActionMenuItemView, ActionMenuItemView);
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CActionMenuItemView, ActionMenuItemView);
ITEXTVIEW_METHODS_IMPL(CActionMenuItemView, ActionMenuItemView);

PInterface CActionMenuItemView::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    else if (riid == EIID_TextView) {
        return reinterpret_cast<PInterface>((TextView*)this);
    }
    return _CActionMenuItemView::Probe(riid);
}

ECode CActionMenuItemView::constructor(
    /* [in] */ IContext* contxt)
{
    return ActionMenuItemView::Init(contxt);
}

ECode CActionMenuItemView::constructor(
    /* [in] */ IContext* contxt,
    /* [in] */ IAttributeSet* attrs)
{
    return ActionMenuItemView::Init(contxt, attrs);
}

ECode CActionMenuItemView::constructor(
    /* [in] */ IContext* contxt,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    return ActionMenuItemView::Init(contxt, attrs, defStyle);
}

ECode CActionMenuItemView::OnPreDraw(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = ActionMenuItemView::OnPreDraw();

    return NOERROR;
}

ECode CActionMenuItemView::GetItemData(
    /* [out] */ IMenuItemImpl** itemData)
{
    return ActionMenuItemView::GetItemData(itemData);
}

ECode CActionMenuItemView::Initialize(
    /* [in] */ IMenuItemImpl* itemData,
    /* [in] */ Int32 menuType)
{
    return ActionMenuItemView::Initialize(itemData, menuType);
}

ECode CActionMenuItemView::SetTitle(
    /* [in] */ ICharSequence* title)
{
    return ActionMenuItemView::SetTitle(title);
}

ECode CActionMenuItemView::SetCheckable(
    /* [in] */ Boolean checkable)
{
    return ActionMenuItemView::SetCheckable(checkable);
}

ECode CActionMenuItemView::SetChecked(
    /* [in] */ Boolean checked)
{
    return ActionMenuItemView::SetChecked(checked);
}

ECode CActionMenuItemView::SetShortcut(
    /* [in] */ Boolean showShortcut,
    /* [in] */ Char32 shortcutKey)
{
    return ActionMenuItemView::SetShortcut(showShortcut, shortcutKey);
}

ECode CActionMenuItemView::PrefersCondensedTitle(
    /* [out] */ Boolean* supported)
{
    return ActionMenuItemView::PrefersCondensedTitle(supported);
}

ECode CActionMenuItemView::ShowsIcon(
    /* [out] */ Boolean* shows)
{
    return ActionMenuItemView::ShowsIcon(shows);
}

ECode CActionMenuItemView::SetEnabled(
    /* [in] */ Boolean enabled)
{
    return NOERROR;
}

ECode CActionMenuItemView::NeedsDividerBefore(
    /* [out] */ Boolean* need)
{
    return ActionMenuItemView::NeedsDividerBefore(need);
}

ECode CActionMenuItemView::NeedsDividerAfter(
    /* [out] */ Boolean* need)
{
    return ActionMenuItemView::NeedsDividerAfter(need);
}

ECode CActionMenuItemView::OnLongClick(
    /* [in] */ IView* v,
    /* [out] */ Boolean* result)
{
    return ActionMenuItemView::OnLongClick(v, result);
}

ECode CActionMenuItemView::OnClick(
    /* [in] */ IView* v)
{
    return ActionMenuItemView::OnClick(v);
}

ECode CActionMenuItemView::SetItemInvoker(
    /* [in] */ IMenuBuilderItemInvoker* invoker)
{
    return ActionMenuItemView::SetItemInvoker(invoker);
}

ECode CActionMenuItemView::SetExpandedFormat(
        /* [in] */ Boolean expandedFormat)
{
    return ActionMenuItemView::SetExpandedFormat(expandedFormat);
}

ECode CActionMenuItemView::SetIcon(
        /* [in] */ IDrawable* icon)
{
    return ActionMenuItemView::SetIcon(icon);
}

ECode CActionMenuItemView::HasText(
        /* [out] */ Boolean* rst)
{
    *rst = ActionMenuItemView::HasText();
    return NOERROR;
}

} // namespace Menu
} // namespace View
} // namespace Internal
} // namespace Droid
} // namespace Elastos
