#include "view/menu/CIconMenuPresenter.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Menu {

CIconMenuPresenter::CIconMenuPresenter()
{}

ECode CIconMenuPresenter::constructor(
    /* [in] */ IContext* context)
{
    IconMenuPresenter::Init(context);
    return NOERROR;
}

PInterface CIconMenuPresenter::Probe(
    /* [in] */ REIID riid)
{
    return _CIconMenuPresenter::Probe(riid);
}


ECode CIconMenuPresenter::InitForMenu(
    /* [in] */ IContext* context,
    /* [in] */ IMenuBuilder* menu)
{
    return IconMenuPresenter::InitForMenu(context, menu);
}

ECode CIconMenuPresenter::GetMenuView(
    /* [in] */ IViewGroup* root,
    /* [out] */ IMenuView** view)
{
    return IconMenuPresenter::GetMenuView(root, view);
}

ECode CIconMenuPresenter::UpdateMenuView(
    /* [in] */ Boolean cleared)
{
    return IconMenuPresenter::UpdateMenuView(cleared);
}

ECode CIconMenuPresenter::SetCallback(
    /* [in] */ IMenuPresenterCallback* cb)
{
    return IconMenuPresenter::SetCallback(cb);
}

ECode CIconMenuPresenter::OnSubMenuSelected(
    /* [in] */ ISubMenuBuilder* subMenu,
    /* [out] */ Boolean* handled)
{
    return IconMenuPresenter::OnSubMenuSelected(subMenu, handled);
}

ECode CIconMenuPresenter::OnCloseMenu(
    /* [in] */ IMenuBuilder* menu,
    /* [in] */ Boolean allMenusAreClosing)
{
    return IconMenuPresenter::OnCloseMenu(menu, allMenusAreClosing);
}

ECode CIconMenuPresenter::FlagActionItems(
    /* [out] */ Boolean* shown)
{
    return IconMenuPresenter::FlagActionItems(shown);
}

ECode CIconMenuPresenter::ExpandItemActionView(
    /* [in] */ IMenuBuilder* menu,
    /* [in] */ IMenuItemImpl* item,
    /* [out] */ Boolean* expanded)
{
    return IconMenuPresenter::ExpandItemActionView(menu, item, expanded);
}

ECode CIconMenuPresenter::CollapseItemActionView(
    /* [in] */ IMenuBuilder* menu,
    /* [in] */ IMenuItemImpl* item,
    /* [out] */ Boolean* collapsed)
{
    return IconMenuPresenter::CollapseItemActionView(menu, item, collapsed);
}

ECode CIconMenuPresenter::GetId(
    /* [out] */ Int32* id)
{
    return IconMenuPresenter::GetId(id);
}

ECode CIconMenuPresenter::OnSaveInstanceState(
    /* [out] */ IParcelable** pa)
{
    return IconMenuPresenter::OnSaveInstanceState(pa);
}

ECode CIconMenuPresenter::OnRestoreInstanceState(
    /* [in] */ IParcelable* state)
{
    return IconMenuPresenter::OnRestoreInstanceState(state);
}

ECode CIconMenuPresenter::CreateItemView(
    /* [in] */ IViewGroup* parent,
    /* [out] */ IMenuItemView** view)
{
    AutoPtr<IMenuItemView> v = IconMenuPresenter::CreateItemView(parent);
    *view = v;
    INTERFACE_ADDREF(*view);
    return NOERROR;
}

ECode CIconMenuPresenter::GetItemView(
    /* [in] */ IMenuItemImpl* item,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent,
    /* [out] */ IView** v)
{
    AutoPtr<IView> vTemp =  IconMenuPresenter::GetItemView(item, convertView, parent);
    *v = vTemp;
    INTERFACE_ADDREF(*v);
    return NOERROR;
}

ECode CIconMenuPresenter::BindItemView(
    /* [in] */ IMenuItemImpl* item,
    /* [in] */ IMenuItemView* itemView)
{
    return IconMenuPresenter::BindItemView(item, itemView);
}

ECode CIconMenuPresenter::ShouldIncludeItem(
    /* [in] */ Int32 childIndex,
    /* [in] */ IMenuItemImpl* item,
    /* [out] */ Boolean* res)
{
    *res = IconMenuPresenter::ShouldIncludeItem(childIndex, item);
    return NOERROR;
}

ECode CIconMenuPresenter::SetId(
    /* [in] */ Int32 id)
{
    return IconMenuPresenter::SetId(id);
}

ECode CIconMenuPresenter::GetNumActualItemsShown(
    /* [out] */ Int32* rst)
{
    *rst = IconMenuPresenter::GetNumActualItemsShown();
    return NOERROR;
}

ECode CIconMenuPresenter::SaveHierarchyState(
    /* [in] */ IBundle* outState)
{
    return IconMenuPresenter::SaveHierarchyState(outState);
}

ECode CIconMenuPresenter::RestoreHierarchyState(
    /* [in] */ IBundle* inState)
{
    return IconMenuPresenter::RestoreHierarchyState(inState);
}

} // namespace Menu
} // namespace View
} // namespace Droid
} // namespace Elastos
