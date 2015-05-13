#include "view/menu/CMenuItemImpl.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Menu {

CMenuItemImpl::CMenuItemImpl()
{}

ECode CMenuItemImpl::constructor(
    /* [in] */ IMenuBuilder* menu,
    /* [in] */ Int32 group,
    /* [in] */ Int32 id,
    /* [in] */ Int32 categoryOrder,
    /* [in] */ Int32 ordering,
    /* [in] */ ICharSequence* title,
    /* [in] */ Int32 showAsAction)
{
    return MenuItemImpl::Init(menu, group, id, categoryOrder, ordering, title, showAsAction);
}

PInterface CMenuItemImpl::Probe(
    /* [in] */ REIID riid)
{
    return _CMenuItemImpl::Probe(riid);
}

ECode CMenuItemImpl::Invoke(
    /* [out] */ Boolean* handled)
{
    return MenuItemImpl::Invoke(handled);
}

ECode CMenuItemImpl::GetOrdering(
    /* [out] */ Int32* ordering)
{
    return MenuItemImpl::GetOrdering(ordering);
}

ECode CMenuItemImpl::SetCallback(
    /* [in] */ IRunnable* cb)
{
    return MenuItemImpl::SetCallback(cb);
}

ECode CMenuItemImpl::SetExclusiveCheckable(
    /* [in] */ Boolean exclusive)
{
    return MenuItemImpl::SetExclusiveCheckable(exclusive);
}

ECode CMenuItemImpl::IsExclusiveCheckable(
    /* [out] */ Boolean* checkable)
{
    return MenuItemImpl::IsExclusiveCheckable(checkable);
}

ECode CMenuItemImpl::ShouldShowIcon(
    /* [out] */ Boolean* show)
{
    return MenuItemImpl::ShouldShowIcon(show);
}

ECode CMenuItemImpl::ActionFormatChanged()
{
    return MenuItemImpl::ActionFormatChanged();
}

ECode CMenuItemImpl::IsActionButton(
    /* [out] */ Boolean* action)
{
    return MenuItemImpl::IsActionButton(action);
}

ECode CMenuItemImpl::RequestsActionButton(
    /* [out] */ Boolean* result)
{
    return MenuItemImpl::RequestsActionButton(result);
}

ECode CMenuItemImpl::RequiresActionButton(
    /* [out] */ Boolean* result)
{
    return MenuItemImpl::RequiresActionButton(result);
}

ECode CMenuItemImpl::SetIsActionButton(
    /* [in] */ Boolean isActionButton)
{
    return MenuItemImpl::SetIsActionButton(isActionButton);
}

ECode CMenuItemImpl::ShowsTextAsAction(
    /* [out] */ Boolean* shows)
{
    return MenuItemImpl::ShowsTextAsAction(shows);
}

ECode CMenuItemImpl::HasCollapsibleActionView(
    /* [out] */ Boolean* result)
{
    return MenuItemImpl::HasCollapsibleActionView(result);
}

ECode CMenuItemImpl::SetActionViewExpanded(
    /* [in] */ Boolean isExpanded)
{
    return MenuItemImpl::SetActionViewExpanded(isExpanded);
}

ECode CMenuItemImpl::GetItemId(
    /* [out] */ Int32* id)
{
    return MenuItemImpl::GetItemId(id);
}

ECode CMenuItemImpl::GetGroupId(
    /* [out] */ Int32* id)
{
    return MenuItemImpl::GetGroupId(id);
}

ECode CMenuItemImpl::GetOrder(
    /* [out] */ Int32* order)
{
    return MenuItemImpl::GetOrder(order);
}

ECode CMenuItemImpl::SetTitle(
    /* [in] */ ICharSequence* title)
{
    return MenuItemImpl::SetTitle(title);
}

ECode CMenuItemImpl::SetTitleEx(
    /* [in] */ Int32 title)
{
    return MenuItemImpl::SetTitleEx(title);
}

ECode CMenuItemImpl::GetTitle(
    /* [out] */ ICharSequence** title)
{
    return MenuItemImpl::GetTitle(title);
}

ECode CMenuItemImpl::SetTitleCondensed(
    /* [in] */ ICharSequence* title)
{
    return MenuItemImpl::SetTitleCondensed(title);
}

ECode CMenuItemImpl::GetTitleCondensed(
    /* [out] */ ICharSequence** title)
{
    return MenuItemImpl::GetTitleCondensed(title);
}

ECode CMenuItemImpl::SetIcon(
    /* [in] */ IDrawable* icon)
{
    return MenuItemImpl::SetIcon(icon);
}

ECode CMenuItemImpl::SetIconEx(
    /* [in] */ Int32 iconRes)
{
    return MenuItemImpl::SetIconEx(iconRes);
}

ECode CMenuItemImpl::GetIcon(
    /* [out] */ IDrawable** icon)
{
    return MenuItemImpl::GetIcon(icon);
}

ECode CMenuItemImpl::SetIntent(
    /* [in] */ IIntent* intent)
{
    return MenuItemImpl::SetIntent(intent);
}

ECode CMenuItemImpl::GetIntent(
    /* [out] */ IIntent** intent)
{
    return MenuItemImpl::GetIntent(intent);
}

ECode CMenuItemImpl::SetShortcut(
    /* [in] */ Char32 numericChar,
    /* [in] */ Char32 alphaChar)
{
    return MenuItemImpl::SetShortcut(numericChar, alphaChar);
}

ECode CMenuItemImpl::SetNumericShortcut(
    /* [in] */ Char32 numericChar)
{
    return MenuItemImpl::SetNumericShortcut(numericChar);
}

ECode CMenuItemImpl::GetNumericShortcut(
    /* [out] */ Char32* numericChar)
{
    return MenuItemImpl::GetNumericShortcut(numericChar);
}

ECode CMenuItemImpl::SetAlphabeticShortcut(
    /* [in] */ Char32 alphaChar)
{
    return MenuItemImpl::SetAlphabeticShortcut(alphaChar);
}

ECode CMenuItemImpl::GetAlphabeticShortcut(
    /* [out] */ Char32* alphaChar)
{
    return MenuItemImpl::GetAlphabeticShortcut(alphaChar);
}

ECode CMenuItemImpl::SetCheckable(
    /* [in] */ Boolean checkable)
{
    return MenuItemImpl::SetCheckable(checkable);
}

ECode CMenuItemImpl::IsCheckable(
    /* [out] */ Boolean* checkable)
{
    return MenuItemImpl::IsCheckable(checkable);
}

ECode CMenuItemImpl::SetChecked(
    /* [in] */ Boolean checked)
{
    return MenuItemImpl::SetChecked(checked);
}

ECode CMenuItemImpl::IsChecked(
    /* [out] */ Boolean* checked)
{
    return MenuItemImpl::IsChecked(checked);
}

ECode CMenuItemImpl::SetVisible(
    /* [in] */ Boolean visible)
{
    return MenuItemImpl::SetVisible(visible);
}

ECode CMenuItemImpl::IsVisible(
    /* [out] */ Boolean* visible)
{
    return MenuItemImpl::IsVisible(visible);
}

ECode CMenuItemImpl::SetEnabled(
    /* [in] */ Boolean enabled)
{
    return MenuItemImpl::SetEnabled(enabled);
}

ECode CMenuItemImpl::IsEnabled(
    /* [out] */ Boolean* enabled)
{
    return MenuItemImpl::IsEnabled(enabled);
}

ECode CMenuItemImpl::HasSubMenu(
    /* [out] */ Boolean* hasSubMenu)
{
    return MenuItemImpl::HasSubMenu(hasSubMenu);
}

ECode CMenuItemImpl::GetSubMenu(
    /* [out] */ ISubMenu** subMenu)
{
    return MenuItemImpl::GetSubMenu(subMenu);
}

ECode CMenuItemImpl::SetOnMenuItemClickListener(
    /* [in] */ IOnMenuItemClickListener* menuItemClickListener)
{
    return MenuItemImpl::SetOnMenuItemClickListener(menuItemClickListener);
}

ECode CMenuItemImpl::GetMenuInfo(
    /* [out] */ IContextMenuInfo** menuInfo)
{
    return MenuItemImpl::GetMenuInfo(menuInfo);
}

ECode CMenuItemImpl::SetShowAsAction(
    /* [in] */ Int32 actionEnum)
{
    return MenuItemImpl::SetShowAsAction(actionEnum);
}

ECode CMenuItemImpl::SetShowAsActionFlags(
    /* [in] */ Int32 actionEnum)
{
    return MenuItemImpl::SetShowAsActionFlags(actionEnum);
}

ECode CMenuItemImpl::SetActionView(
    /* [in] */ IView* view)
{
    return MenuItemImpl::SetActionView(view);
}

ECode CMenuItemImpl::SetActionViewEx(
    /* [in] */ Int32 resId)
{
    return MenuItemImpl::SetActionViewEx(resId);
}

ECode CMenuItemImpl::GetActionView(
    /* [out] */ IView** view)
{
    return MenuItemImpl::GetActionView(view);
}

ECode CMenuItemImpl::SetActionProvider(
    /* [in] */ IActionProvider* actionProvider)
{
    return MenuItemImpl::SetActionProvider(actionProvider);
}

ECode CMenuItemImpl::GetActionProvider(
    /* [out] */ IActionProvider** provider)
{
    return MenuItemImpl::GetActionProvider(provider);
}

ECode CMenuItemImpl::ExpandActionView(
    /* [out] */ Boolean* expanded)
{
    return MenuItemImpl::ExpandActionView(expanded);
}

ECode CMenuItemImpl::CollapseActionView(
    /* [out] */ Boolean* collapsed)
{
    return MenuItemImpl::CollapseActionView(collapsed);
}

ECode CMenuItemImpl::IsActionViewExpanded(
    /* [out] */ Boolean* expanded)
{
    return MenuItemImpl::IsActionViewExpanded(expanded);
}

ECode CMenuItemImpl::SetOnActionExpandListener(
    /* [in] */ IOnActionExpandListener* listener)
{
    return MenuItemImpl::SetOnActionExpandListener(listener);
}

ECode CMenuItemImpl::GetShortcut(
    /* [out] */ Char32* c)
{
    return MenuItemImpl::GetShortcut(c);
}

ECode CMenuItemImpl::GetShortcutLabel(
    /* [out] */ String* label)
{
    return MenuItemImpl::GetShortcutLabel(label);
}

ECode CMenuItemImpl::ShouldShowShortcut(
    /* [out] */ Boolean* result)
{
    return MenuItemImpl::ShouldShowShortcut(result);
}

ECode CMenuItemImpl::SetSubMenu(
    /* [in] */ ISubMenuBuilder* subMenu)
{
    return MenuItemImpl::SetSubMenu(subMenu);
}

ECode CMenuItemImpl::GetTitleForItemView(
    /* [in] */ IMenuItemView* itemView,
    /* [out] */ ICharSequence** title)
{
    return MenuItemImpl::GetTitleForItemView(itemView, title);
}

ECode CMenuItemImpl::SetCheckedInt(
    /* [in] */ Boolean checked)
{
    return MenuItemImpl::SetCheckedInt(checked);
}

ECode CMenuItemImpl::SetVisibleInt(
    /* [in] */ Boolean show,
    /* [out] */ Boolean* isVisible)
{
    return MenuItemImpl::SetVisibleInt(show, isVisible);
}

ECode CMenuItemImpl::SetMenuInfo(
    /* [in] */ IContextMenuInfo* menuInfo)
{
    return MenuItemImpl::SetMenuInfo(menuInfo);
}

} // namespace Menu
} // namespace View
} // namepsace Droid
} // namespace Elastos
