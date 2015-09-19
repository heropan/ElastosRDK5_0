#include "view/menu/CMenuPopupHelper.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Menu {

CMenuPopupHelper::CMenuPopupHelper()
{}

ECode CMenuPopupHelper::constructor(
    /* [in] */IContext* ctx,
    /* [in] */IMenuBuilder* menu)
{
    return MenuPopupHelper::Init(ctx, menu);
}

ECode CMenuPopupHelper::constructor(
    /* [in] */IContext* ctx,
    /* [in] */IMenuBuilder* menu,
    /* [in] */IView* anchorView)
{
    return MenuPopupHelper::Init(ctx, menu, anchorView);
}

ECode CMenuPopupHelper::constructor(
    /* [in] */IContext* ctx,
    /* [in] */IMenuBuilder* menu,
    /* [in] */IView* anchorView,
    /* [in] */Boolean overflowOnly)
{
    return MenuPopupHelper::Init(ctx, menu, anchorView, overflowOnly);
}

PInterface CMenuPopupHelper::Probe(
    /* [in] */ REIID riid)
{
    return _CMenuPopupHelper::Probe(riid);
}

ECode CMenuPopupHelper::SetAnchorView(
        /* [in] */ IView* anchor)
{
    return MenuPopupHelper::SetAnchorView(anchor);
}

ECode CMenuPopupHelper::SetForceShowIcon(
    /* [in] */ Boolean forceShow)
{
    return MenuPopupHelper::SetForceShowIcon(forceShow);
}

ECode CMenuPopupHelper::Show()
{
    return MenuPopupHelper::Show();
}

ECode CMenuPopupHelper::TryShow(
    /* [out] */ Boolean* rst)
{
    *rst = MenuPopupHelper::TryShow();
    return NOERROR;
}

ECode CMenuPopupHelper::Dismiss()
{
    return MenuPopupHelper::Dismiss();
}

ECode CMenuPopupHelper::IsShowing(
    /* [out] */ Boolean* rst)
{
    *rst = MenuPopupHelper::IsShowing();
    return NOERROR;
}

ECode CMenuPopupHelper::OnItemClick(
    /* [in] */ IAdapterView* parent,
    /* [in] */ IView* view,
    /* [in] */ Int32 position,
    /* [in] */ Int64 id)
{
    return MenuPopupHelper::OnItemClick(parent, view, position, id);
}

ECode CMenuPopupHelper::OnKey(
    /* [in] */ IView* v,
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    return MenuPopupHelper::OnKey(v, keyCode, event, result);
}

ECode CMenuPopupHelper::OnGlobalLayout()
{
    return MenuPopupHelper::OnGlobalLayout();
}

ECode CMenuPopupHelper::OnDismiss()
{
    return MenuPopupHelper::OnDismiss();
}

ECode CMenuPopupHelper::OnViewAttachedToWindow(
    /* [in] */ IView* v)
{
    return MenuPopupHelper::OnViewAttachedToWindow(v);
}

ECode CMenuPopupHelper::OnViewDetachedFromWindow(
    /* [in] */ IView* v)
{
    return MenuPopupHelper::OnViewDetachedFromWindow(v);
}

ECode CMenuPopupHelper::InitForMenu(
    /* [in] */ IContext* context,
    /* [in] */ IMenuBuilder* menu)
{
    return MenuPopupHelper::InitForMenu(context, menu);
}

ECode CMenuPopupHelper::GetMenuView(
    /* [in] */ IViewGroup* root,
    /* [out] */ IMenuView** view)
{
    return MenuPopupHelper::GetMenuView(root, view);
}

ECode CMenuPopupHelper::UpdateMenuView(
    /* [in] */ Boolean cleared)
{
    return MenuPopupHelper::UpdateMenuView(cleared);
}

ECode CMenuPopupHelper::SetCallback(
    /* [in] */ IMenuPresenterCallback* cb)
{
    return MenuPopupHelper::SetCallback(cb);
}

ECode CMenuPopupHelper::OnSubMenuSelected(
    /* [in] */ ISubMenuBuilder* subMenu,
    /* [out] */ Boolean* handled)
{
    return MenuPopupHelper::OnSubMenuSelected(subMenu, handled);
}

ECode CMenuPopupHelper::OnCloseMenu(
    /* [in] */ IMenuBuilder* menu,
    /* [in] */ Boolean allMenusAreClosing)
{
    return MenuPopupHelper::OnCloseMenu(menu, allMenusAreClosing);
}

ECode CMenuPopupHelper::FlagActionItems(
    /* [out] */ Boolean* shown)
{
    return MenuPopupHelper::FlagActionItems(shown);
}

ECode CMenuPopupHelper::ExpandItemActionView(
    /* [in] */ IMenuBuilder* menu,
    /* [in] */ IMenuItemImpl* item,
    /* [out] */ Boolean* expanded)
{
    return MenuPopupHelper::ExpandItemActionView(menu, item, expanded);
}

ECode CMenuPopupHelper::CollapseItemActionView(
    /* [in] */ IMenuBuilder* menu,
    /* [in] */ IMenuItemImpl* item,
    /* [out] */ Boolean* collapsed)
{
    return MenuPopupHelper::CollapseItemActionView(menu, item, collapsed);
}

ECode CMenuPopupHelper::GetId(
    /* [out] */ Int32* id)
{
    return MenuPopupHelper::GetId(id);
}

ECode CMenuPopupHelper::OnSaveInstanceState(
    /* [out] */ IParcelable** pa)
{
    return MenuPopupHelper::OnSaveInstanceState(pa);
}

ECode CMenuPopupHelper::OnRestoreInstanceState(
    /* [in] */ IParcelable* state)
{
    return MenuPopupHelper::OnRestoreInstanceState(state);
}

} // namespace Menu
} // namespace View
} // namepsace Droid
} // namespace Elastos
