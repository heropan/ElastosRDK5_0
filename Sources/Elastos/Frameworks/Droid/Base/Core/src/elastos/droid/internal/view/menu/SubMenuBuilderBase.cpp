
#include "elastos/droid/internal/view/menu/SubMenuBuilderBase.h"
#include <elastos/core/StringUtils.h>

using Elastos::Core::StringUtils;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace View {
namespace Menu {

SubMenuBuilderBase::SubMenuBuilderBase(
        /* [in] */ IContext* context,
        /* [in] */ IMenuBuilder* parentMenu,
        /* [in] */ IMenuItemImpl* item)
    : MenuBuilder(context)
    , mParentMenu(parentMenu)
    , mItem(item)
{
}

SubMenuBuilderBase::SubMenuBuilderBase()
{}

ECode SubMenuBuilderBase::Init(
    /* [in] */ IContext* context,
    /* [in] */ IMenuBuilder* parentMenu,
    /* [in] */ IMenuItemImpl* item)
{
    MenuBuilder::Init(context);
    mParentMenu = parentMenu;
    mItem = item;
    return NOERROR;
}

ECode SubMenuBuilderBase::SetQwertyMode(
        /* [in] */ Boolean isQwerty)
{
    return mParentMenu->SetQwertyMode(isQwerty);
}

Boolean SubMenuBuilderBase::IsQwertyMode()
{
    Boolean res = FALSE;
    mParentMenu->IsQwertyMode(&res);
    return res;
}

ECode SubMenuBuilderBase::SetShortcutsVisible(
    /* [in] */ Boolean shortcutsVisible)
{
    return mParentMenu->SetShortcutsVisible(shortcutsVisible);
}

Boolean SubMenuBuilderBase::IsShortcutsVisible()
{
    Boolean isVisible = FALSE;

    mParentMenu->IsShortcutsVisible(&isVisible);
    return isVisible;
}

AutoPtr<IMenu> SubMenuBuilderBase::GetParentMenu()
{
    return IMenu::Probe(mParentMenu);
}

AutoPtr<IMenuItem> SubMenuBuilderBase::GetItem()
{
    return IMenuItem::Probe(mItem);
}

ECode SubMenuBuilderBase::SetCallback(
    /* [in] */ IMenuBuilderCallback* callback)
{
    return mParentMenu->SetCallback(callback);
}

AutoPtr<IMenuBuilder> SubMenuBuilderBase::GetRootMenu()
{
    return mParentMenu;
}

Boolean SubMenuBuilderBase::DispatchMenuItemSelected(
    /* [in] */ IMenuBuilder* menu,
    /* [in] */ IMenuItem* item)
{
    Boolean res = FALSE;
    mParentMenu->DispatchMenuItemSelected(menu, item, &res);
    return MenuBuilder::DispatchMenuItemSelected(menu, item) || res;

}

ECode SubMenuBuilderBase::SetIcon(
    /* [in] */ IDrawable* icon)
{
    return mItem->SetIcon(icon);
}

ECode SubMenuBuilderBase::SetIcon(
    /* [in] */ Int32 iconRes)
{
    return mItem->SetIcon(iconRes);
}

ECode SubMenuBuilderBase::SetHeaderIcon(
    /* [in] */ IDrawable* icon)
{
    MenuBuilder::SetHeaderIconInt(icon);
    return NOERROR;
}

ECode SubMenuBuilderBase::SetHeaderIcon(
    /* [in] */ Int32 iconRes)
{
    MenuBuilder::SetHeaderIconInt(iconRes);
    return NOERROR;
}

ECode SubMenuBuilderBase::SetHeaderTitle(
    /* [in] */ ICharSequence* title)
{
    MenuBuilder::SetHeaderTitleInt(title);
    return NOERROR;
}

ECode SubMenuBuilderBase::SetHeaderTitle(
    /* [in] */ Int32 titleRes)
{
    MenuBuilder::SetHeaderTitleInt(titleRes);
    return NOERROR;
}

ECode SubMenuBuilderBase::SetHeaderView(
    /* [in] */ IView* view)
{
    MenuBuilder::SetHeaderViewInt(view);
    return NOERROR;
}

Boolean SubMenuBuilderBase::ExpandItemActionView(
    /* [in] */ IMenuItemImpl* item)
{
    Boolean res = FALSE;
    mParentMenu->ExpandItemActionView(item, &res);
    return res;
}

Boolean SubMenuBuilderBase::CollapseItemActionView(
    /* [in] */ IMenuItemImpl* item)
{
    Boolean res = FALSE;
    mParentMenu->CollapseItemActionView(item, &res);
    return res;
}

void SubMenuBuilderBase::GetActionViewStatesKey(
    /* [out] */ String* key)
{
    assert(key != NULL);
    Int32 itemId = 0;
    if (mItem != NULL) {
        mItem->GetItemId(&itemId);
    }

    if (itemId == 0) {
        return;
    }

    String value;
    MenuBuilder::GetActionViewStatesKey(&value);
    *key = value + String(":") + StringUtils::Int32ToString(itemId);
}

} // namespace Menu
} // namespace View
} // namespace Internal
} // namespace Droid
} // namespace Elastos
