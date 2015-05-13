#include "view/menu/CListMenuPresenter.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Menu {

CListMenuPresenter::CListMenuPresenter()
{}

ECode CListMenuPresenter::constructor(
    /* [in] */ IContext* ctx,
    /* [in] */ Int32 itemLayoutRes)
{
    ListMenuPresenter::Init(ctx, itemLayoutRes);
    return NOERROR;
}

ECode CListMenuPresenter::constructor(
    /* [in] */ Int32 itemLayoutRes,
    /* [in] */ Int32 themeRes)
{
    ListMenuPresenter::Init(itemLayoutRes, themeRes);
    return NOERROR;
}

PInterface CListMenuPresenter::Probe(
    /* [in]  */ REIID riid)
{
    return _CListMenuPresenter::Probe(riid);
}


ECode CListMenuPresenter::OnItemClick(
    /* [in] */ IAdapterView* parent,
    /* [in] */ IView* view,
    /* [in] */ Int32 position,
    /* [in] */ Int64 id)
{
    return ListMenuPresenter::OnItemClick(parent, view, position, id);
}

ECode CListMenuPresenter::GetAdapter(
    /* [out] */ IListAdapter** apt)
{
    AutoPtr<IListAdapter> adapter = ListMenuPresenter::GetAdapter();
    *apt = adapter;
    INTERFACE_ADDREF(*apt);

    return NOERROR;
}

ECode CListMenuPresenter::GetItemIndexOffset(
    /* [out] */ Int32* offset)
{
    *offset = ListMenuPresenter::GetItemIndexOffset();
    return NOERROR;
}

ECode CListMenuPresenter::SetItemIndexOffset(
    /* [in] */ Int32 offset)
{
    return ListMenuPresenter::SetItemIndexOffset(offset);
}

ECode CListMenuPresenter::ExpandItemActionView(
    /* [in] */ IMenuBuilder* menu,
    /* [in] */ IMenuItemImpl* item,
    /* [out] */ Boolean* rst)
{
    return ListMenuPresenter::ExpandItemActionView(menu, item, rst);
}

ECode CListMenuPresenter::CollapseItemActionView(
    /* [in] */ IMenuBuilder* menu,
    /* [in] */ IMenuItemImpl* item,
    /* [out] */ Boolean* rst)
{
    return ListMenuPresenter::CollapseItemActionView(menu, item, rst);
}

ECode CListMenuPresenter::SaveHierarchyState(
    /* [in] */ IBundle* outState)
{
    return ListMenuPresenter::SaveHierarchyState(outState);
}

ECode CListMenuPresenter::RestoreHierarchyState(
    /* [in] */ IBundle* outState)
{
    return ListMenuPresenter::RestoreHierarchyState(outState);
}

ECode CListMenuPresenter::SetId(
    /* [in] */ Int32 id)
{
    return ListMenuPresenter::SetId(id);
}

ECode CListMenuPresenter::InitForMenu(
    /* [in] */ IContext* context,
    /* [in] */ IMenuBuilder* menu)
{
    return ListMenuPresenter::InitForMenu(context, menu);
}

ECode CListMenuPresenter::GetMenuView(
    /* [in] */ IViewGroup* root,
    /* [out] */ IMenuView** view)
{
    return ListMenuPresenter::GetMenuView(root, view);
}

ECode CListMenuPresenter::UpdateMenuView(
    /* [in] */ Boolean cleared)
{
    return ListMenuPresenter::UpdateMenuView(cleared);
}

ECode CListMenuPresenter::SetCallback(
    /* [in] */ IMenuPresenterCallback* cb)
{
    return ListMenuPresenter::SetCallback(cb);
}

ECode CListMenuPresenter::OnSubMenuSelected(
    /* [in] */ ISubMenuBuilder* subMenu,
    /* [out] */ Boolean* handled)
{
    return ListMenuPresenter::OnSubMenuSelected(subMenu, handled);
}

ECode CListMenuPresenter::OnCloseMenu(
    /* [in] */ IMenuBuilder* menu,
    /* [in] */ Boolean allMenusAreClosing)
{
    return ListMenuPresenter::OnCloseMenu(menu, allMenusAreClosing);
}

ECode CListMenuPresenter::FlagActionItems(
    /* [out] */ Boolean* shown)
{
    return ListMenuPresenter::FlagActionItems(shown);
}

ECode CListMenuPresenter::GetId(
    /* [out] */ Int32* id)
{
    return ListMenuPresenter::GetId(id);
}

ECode CListMenuPresenter::OnSaveInstanceState(
    /* [out] */ IParcelable** pa)
{
    return ListMenuPresenter::OnSaveInstanceState(pa);
}

ECode CListMenuPresenter::OnRestoreInstanceState(
    /* [in] */ IParcelable* state)
{
    return ListMenuPresenter::OnRestoreInstanceState(state);
}
} // namespace Menu
} // namespace View
} // namepsace Droid
} // namespace Elastos
