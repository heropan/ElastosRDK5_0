
#include "view/menu/CActionMenuPresenter.h"
#include "R.h"

using Elastos::Droid::R;

namespace Elastos {
namespace Droid {
namespace View {
namespace Menu {

CActionMenuPresenter::CActionMenuPresenter()
{}

ECode CActionMenuPresenter::constructor(
    /* [in] */ IContext* ctx)
{
    ActionMenuPresenter::Init(ctx, R::layout::action_menu_layout,
            R::layout::action_menu_item_layout);
    return NOERROR;
}

PInterface CActionMenuPresenter::Probe(
        /* [in] */ REIID riid)
{
    return _CActionMenuPresenter::Probe(riid);
}

ECode CActionMenuPresenter::OnSubUiVisibilityChanged(
    /* [in] */ Boolean isVisible)
{
    ActionMenuPresenter::OnSubUiVisibilityChanged(isVisible);
    return NOERROR;
}

ECode CActionMenuPresenter::OnConfigurationChanged(
    /* [in] */ IConfiguration* newConfig)
{
    ActionMenuPresenter::OnConfigurationChanged(newConfig);
    return NOERROR;
}

ECode CActionMenuPresenter::SetWidthLimit(
    /* [in] */ Int32 width,
    /* [in] */ Boolean strict)
{
    ActionMenuPresenter::SetWidthLimit(width, strict);
    return NOERROR;
}

ECode CActionMenuPresenter::SetReserveOverflow(
    /* [in] */ Boolean reserveOverflow)
{
    ActionMenuPresenter::SetReserveOverflow(reserveOverflow);
    return NOERROR;
}

ECode CActionMenuPresenter::SetItemLimit(
    /* [in] */ Int32 itemCount)
{
    ActionMenuPresenter::SetItemLimit(itemCount);
    return NOERROR;
}

ECode CActionMenuPresenter::SetExpandedActionViewsExclusive(
    /* [in] */ Boolean isExclusive)
{
    ActionMenuPresenter::SetExpandedActionViewsExclusive(isExclusive);
    return NOERROR;
}

ECode CActionMenuPresenter::ShowOverflowMenu(
    /* [out] */ Boolean* res)
{
    *res = ActionMenuPresenter::ShowOverflowMenu();
    return NOERROR;
}

ECode CActionMenuPresenter::HideOverflowMenu(
    /* [out] */ Boolean* res)
{
    *res = ActionMenuPresenter::HideOverflowMenu();
    return NOERROR;
}

ECode CActionMenuPresenter::DismissPopupMenus(
    /* [out] */ Boolean* res)
{
    *res = ActionMenuPresenter::DismissPopupMenus();
    return NOERROR;
}

ECode CActionMenuPresenter::HideSubMenus(
    /* [out] */ Boolean* res)
{
    *res =ActionMenuPresenter::HideSubMenus();
    return NOERROR;
}

ECode CActionMenuPresenter::IsOverflowMenuShowing(
    /* [out] */ Boolean* res)
{
    *res =ActionMenuPresenter::IsOverflowMenuShowing();
    return NOERROR;
}

ECode CActionMenuPresenter::IsOverflowReserved(
    /* [out] */ Boolean* res)
{
    *res = ActionMenuPresenter::IsOverflowReserved();
    return NOERROR;
}

ECode CActionMenuPresenter::CreateItemView(
    /* [in] */ IViewGroup* parent,
    /* [out] */ IMenuItemView** view)
{
    AutoPtr<IMenuItemView> temp = ActionMenuPresenter::CreateItemView(parent);
    *view = temp;
    INTERFACE_ADDREF(*view);
    return NOERROR;
}

ECode CActionMenuPresenter::GetItemView(
    /* [in] */ IMenuItemImpl* item,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent,
    /* [out] */ IView** v)
{
    return ActionMenuPresenter::GetItemView(item, convertView, parent, v);
}

ECode CActionMenuPresenter::BindItemView(
    /* [in] */ IMenuItemImpl* item,
    /* [in] */ IMenuItemView* itemView)
{
    ActionMenuPresenter::BindItemView(item, itemView);
    return NOERROR;
}

ECode CActionMenuPresenter::ShouldIncludeItem(
    /* [in] */ Int32 childIndex,
    /* [in] */ IMenuItemImpl* item,
    /* [out] */ Boolean* res)
{
    *res = ActionMenuPresenter::ShouldIncludeItem(childIndex, item);
    return NOERROR;
}

ECode CActionMenuPresenter::SetId(
    /* [in] */ Int32 id)
{
    return ActionMenuPresenter::SetId(id);
}

ECode CActionMenuPresenter::InitForMenu(
    /* [in] */ IContext* context,
    /* [in] */ IMenuBuilder* menu)
{
    return ActionMenuPresenter::InitForMenu(context, menu);
}

ECode CActionMenuPresenter::GetMenuView(
    /* [in] */ IViewGroup* root,
    /* [out] */ IMenuView** view)
{
    return ActionMenuPresenter::GetMenuView(root, view);
}

ECode CActionMenuPresenter::UpdateMenuView(
    /* [in] */ Boolean cleared)
{
    return ActionMenuPresenter::UpdateMenuView(cleared);
}

ECode CActionMenuPresenter::SetCallback(
    /* [in] */ IMenuPresenterCallback* cb)
{
    return ActionMenuPresenter::SetCallback(cb);
}

ECode CActionMenuPresenter::OnSubMenuSelected(
    /* [in] */ ISubMenuBuilder* subMenu,
    /* [out] */ Boolean* handled)
{
    return ActionMenuPresenter::OnSubMenuSelected(subMenu, handled);
}

ECode CActionMenuPresenter::OnCloseMenu(
    /* [in] */ IMenuBuilder* menu,
    /* [in] */ Boolean allMenusAreClosing)
{
    return ActionMenuPresenter::OnCloseMenu(menu, allMenusAreClosing);
}

ECode CActionMenuPresenter::FlagActionItems(
    /* [out] */ Boolean* shown)
{
    return ActionMenuPresenter::FlagActionItems(shown);
}

ECode CActionMenuPresenter::ExpandItemActionView(
    /* [in] */ IMenuBuilder* menu,
    /* [in] */ IMenuItemImpl* item,
    /* [out] */ Boolean* expanded)
{
    return NOERROR;
}

ECode CActionMenuPresenter::CollapseItemActionView(
    /* [in] */ IMenuBuilder* menu,
    /* [in] */ IMenuItemImpl* item,
    /* [out] */ Boolean* collapsed)
{
    return ActionMenuPresenter::CollapseItemActionView(menu, item, collapsed);
}

ECode CActionMenuPresenter::GetId(
    /* [out] */ Int32* id)
{
    return ActionMenuPresenter::GetId(id);
}

ECode CActionMenuPresenter::OnSaveInstanceState(
    /* [out] */ IParcelable** pa)
{
    return ActionMenuPresenter::OnSaveInstanceState(pa);
}

ECode CActionMenuPresenter::OnRestoreInstanceState(
    /* [in] */ IParcelable* state)
{
    return ActionMenuPresenter::OnRestoreInstanceState(state);
}

} // namespace Menu
} // namespace View
} // namespace Droid
} // namespace Elastos
