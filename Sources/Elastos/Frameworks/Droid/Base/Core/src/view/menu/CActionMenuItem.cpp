#include "view/menu/CActionMenuItem.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Menu {

ECode CActionMenuItem::constructor(
    /* [in] */ IContext* context,
    /* [in] */ Int32 group,
    /* [in] */ Int32 id,
    /* [in] */ Int32 categoryOrder,
    /* [in] */ Int32 ordering,
    /* [in] */ ICharSequence* title)
{
    return ActionMenuItem::Init(context, group, id, categoryOrder, ordering, title);
}

PInterface CActionMenuItem::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_IMenuItem == riid) {
        return (IMenuItem*)this;
    } else if(EIID_IActionMenuItem == riid) {
        return (IActionMenuItem*)this;
    } else if(EIID_IInterface == riid) {
        return (IActionMenuItem*)this;
    }

    return NULL;
}

ECode CActionMenuItem::GetAlphabeticShortcut(
    /* [out] */ Char32* shortcut)
{
    VALIDATE_NOT_NULL(shortcut);
    return ActionMenuItem::GetAlphabeticShortcut(shortcut);
}

ECode CActionMenuItem::GetGroupId(
    /* [out] */ Int32* id)
{
    VALIDATE_NOT_NULL(id);
    return ActionMenuItem::GetGroupId(id);
}

ECode CActionMenuItem::GetIcon(
    /* [out] */ IDrawable** icon)
{
    VALIDATE_NOT_NULL(icon);
    return ActionMenuItem::GetIcon(icon);
}

ECode CActionMenuItem::GetIntent(
    /* [out] */ IIntent** intent)
{
    VALIDATE_NOT_NULL(intent);
    return ActionMenuItem::GetIntent(intent);
}

ECode CActionMenuItem::GetItemId(
    /* [out] */ Int32* id)
{
    VALIDATE_NOT_NULL(id);
    return ActionMenuItem::GetItemId(id);
}

ECode CActionMenuItem::GetMenuInfo(
    /* [out] */ IContextMenuInfo** info)
{
    VALIDATE_NOT_NULL(info);
    return ActionMenuItem::GetMenuInfo(info);
}

ECode CActionMenuItem::GetNumericShortcut(
    /* [out] */ Char32* shortcut)
{
    VALIDATE_NOT_NULL(shortcut);
    return ActionMenuItem::GetNumericShortcut(shortcut);
}

ECode CActionMenuItem::GetOrder(
    /* [out] */ Int32* ordering)
{
    VALIDATE_NOT_NULL(ordering);
    return ActionMenuItem::GetOrder(ordering);
}

ECode CActionMenuItem::GetSubMenu(
    /* [out] */ ISubMenu** menu)
{
    VALIDATE_NOT_NULL(menu);
    return ActionMenuItem::GetSubMenu(menu);
}

ECode CActionMenuItem::GetTitle(
    /* [out] */ ICharSequence** title)
{
    VALIDATE_NOT_NULL(title);
    return ActionMenuItem::GetTitle(title);
}

ECode CActionMenuItem::GetTitleCondensed(
    /* [out] */ ICharSequence** title)
{
    VALIDATE_NOT_NULL(title);
    return ActionMenuItem::GetTitleCondensed(title);
}

ECode CActionMenuItem::HasSubMenu(
    /* [out] */ Boolean* has)
{
    VALIDATE_NOT_NULL(has);
    return ActionMenuItem::HasSubMenu(has);
}

ECode CActionMenuItem::IsCheckable(
    /* [out] */ Boolean* checkable)
{
    VALIDATE_NOT_NULL(checkable);
    return ActionMenuItem::IsCheckable(checkable);
}

ECode CActionMenuItem::IsChecked(
    /* [out] */ Boolean* checked)
{
    VALIDATE_NOT_NULL(checked);
    return ActionMenuItem::IsChecked(checked);
}

ECode CActionMenuItem::IsEnabled(
    /* [out] */ Boolean* enabled)
{
    VALIDATE_NOT_NULL(enabled);
    return ActionMenuItem::IsEnabled(enabled);
}

ECode CActionMenuItem::IsVisible(
    /* [out] */ Boolean* visible)
{
    VALIDATE_NOT_NULL(visible);
    return ActionMenuItem::IsVisible(visible);
}

ECode CActionMenuItem::SetAlphabeticShortcut(
    /* [in] */ Char32 alphaChar)
{
    return ActionMenuItem::SetAlphabeticShortcut(alphaChar);
}

ECode CActionMenuItem::SetCheckable(
    /* [in] */ Boolean checkable)
{
    return ActionMenuItem::SetCheckable(checkable);
}

ECode CActionMenuItem::SetExclusiveCheckable(
    /* [in] */ Boolean exclusive)
{
    return ActionMenuItem::SetExclusiveCheckable(exclusive);
}

ECode CActionMenuItem::SetChecked(
    /* [in] */ Boolean checked)
{
    return ActionMenuItem::SetChecked(checked);
}

ECode CActionMenuItem::SetEnabled(
    /* [in] */ Boolean enabled)
{
    return ActionMenuItem::SetEnabled(enabled);
}

ECode CActionMenuItem::SetIcon(
    /* [in] */ IDrawable* icon)
{
    return ActionMenuItem::SetIcon(icon);
}

ECode CActionMenuItem::SetIconEx(
    /* [in] */ Int32 iconRes)
{
    return ActionMenuItem::SetIconEx(iconRes);
}

ECode CActionMenuItem::SetIntent(
    /* [in] */ IIntent* intent)
{
    return ActionMenuItem::SetIntent(intent);
}

ECode CActionMenuItem::SetNumericShortcut(
    /* [in] */ Char32 numericChar)
{
    return ActionMenuItem::SetNumericShortcut(numericChar);
}

ECode CActionMenuItem::SetOnMenuItemClickListener(
    /* [in] */ IOnMenuItemClickListener* menuItemClickListener)
{
    return ActionMenuItem::SetOnMenuItemClickListener(menuItemClickListener);
}

ECode CActionMenuItem::SetShortcut(
    /* [in] */ Char32 numericChar,
    /* [in] */ Char32 alphaChar)
{
    return ActionMenuItem::SetShortcut(numericChar, alphaChar);
}

ECode CActionMenuItem::SetTitle(
    /* [in] */ ICharSequence* title)
{
    return ActionMenuItem::SetTitle(title);
}

ECode CActionMenuItem::SetTitleEx(
    /* [in] */ Int32 title)
{
    return ActionMenuItem::SetTitleEx(title);
}

ECode CActionMenuItem::SetTitleCondensed(
    /* [in] */ ICharSequence* title)
{
    return ActionMenuItem::SetTitleCondensed(title);
}

ECode CActionMenuItem::SetVisible(
    /* [in] */ Boolean visible)
{
    return ActionMenuItem::SetVisible(visible);
}

ECode CActionMenuItem::Invoke(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    return ActionMenuItem::Invoke(res);
}

ECode CActionMenuItem::SetShowAsAction(
    /* [in] */ Int32 show)
{
    return ActionMenuItem::SetShowAsAction(show);
}

ECode CActionMenuItem::SetActionView(
    /* [in] */ IView* actionView)
{
    return ActionMenuItem::SetActionView(actionView);
}

ECode CActionMenuItem::GetActionView(
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);
    return ActionMenuItem::GetActionView(view);
}

ECode CActionMenuItem::SetActionViewEx(
    /* [in] */ Int32 resId)
{
    return ActionMenuItem::SetActionViewEx(resId);
}

ECode CActionMenuItem::GetActionProvider(
    /* [out] */ IActionProvider** provider)
{
    VALIDATE_NOT_NULL(provider);
    return ActionMenuItem::GetActionProvider(provider);
}

ECode CActionMenuItem::SetActionProvider(
    /* [in] */ IActionProvider* actionProvider)
{
    return ActionMenuItem::SetActionProvider(actionProvider);
}

ECode CActionMenuItem::SetShowAsActionFlags(
    /* [in] */ Int32 actionEnum)
{
    return ActionMenuItem::SetShowAsActionFlags(actionEnum);
}

ECode CActionMenuItem::ExpandActionView(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return ActionMenuItem::ExpandActionView(result);
}

ECode CActionMenuItem::CollapseActionView(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return ActionMenuItem::CollapseActionView(result);
}

ECode CActionMenuItem::IsActionViewExpanded(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return ActionMenuItem::IsActionViewExpanded(result);
}

ECode CActionMenuItem::SetOnActionExpandListener(
    /* [in] */ IOnActionExpandListener* listener)
{
    return ActionMenuItem::SetOnActionExpandListener(listener);
}


} // namespace Menu
} // namespace View
} // namespace Droid
} // namespace Elastos
