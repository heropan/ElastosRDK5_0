
#include "view/menu/BaseMenuPresenter.h"
#include "view/LayoutInflater.h"
#include "view/menu/MenuBuilderBase.h"
#include "view/menu/MenuItemImpl.h"
#include <elastos/utility/etl/List.h>

using Elastos::Utility::Etl::List;
using Elastos::Droid::View::LayoutInflater;

namespace Elastos {
namespace Droid {
namespace View {
namespace Menu {

BaseMenuPresenter::BaseMenuPresenter()
    : mMenuLayoutRes(0)
    , mItemLayoutRes(0)
    , mId(0)
{
}

BaseMenuPresenter::BaseMenuPresenter(
    /* [in] */ IContext* context,
    /* [in] */ Int32 menuLayoutRes,
    /* [in] */ Int32 itemLayoutRes)
    : mSystemContext(context)
    , mMenuLayoutRes(menuLayoutRes)
    , mItemLayoutRes(itemLayoutRes)
    , mId(0)
{
    LayoutInflater::From(context, (ILayoutInflater**)&mSystemInflater);
}

ECode BaseMenuPresenter::InitForMenu(
    /* [in] */ IContext* context,
    /* [in] */ IMenuBuilder* menu)
{
    mContext = context;
    LayoutInflater::From(mContext, (ILayoutInflater**)&mInflater);
    mMenu = menu;
    return NOERROR;
}

ECode BaseMenuPresenter::GetMenuView(
    /* [in] */ IViewGroup* root,
    /* [out] */ IMenuView** view)
{
    assert(view != NULL);
    if (mMenuView == NULL) {
        AutoPtr<IView> vTemp;
        mSystemInflater->Inflate(mMenuLayoutRes, root, FALSE, (IView**)&vTemp);
        mMenuView = IMenuView::Probe(vTemp);
        mMenuView->Initialize(mMenu);
        UpdateMenuView(TRUE);
    }

    *view = mMenuView;
    REFCOUNT_ADD(*view);
    return NOERROR;
}

ECode BaseMenuPresenter::UpdateMenuView(
    /* [in] */ Boolean cleared)
{
    AutoPtr<IViewGroup> parent = IViewGroup::Probe(mMenuView);
    if (parent == NULL) return NOERROR;

    Int32 childIndex = 0;
    if (mMenu != NULL) {
        mMenu->FlagActionItems();

        MenuBuilderBase* base = (MenuBuilderBase*)mMenu->Probe(EIID_MenuBuilderBase);
        assert(base != NULL);

        AutoPtr< List<AutoPtr<IMenuItemImpl> > > visibleItems = base->GetVisibleItems();
        const Int32 itemCount = visibleItems->GetSize();
        for (Int32 i = 0; i < itemCount; i++) {
            AutoPtr<IMenuItemImpl> item = (*visibleItems)[i];
            if (ShouldIncludeItem(childIndex, item)) {
                AutoPtr<IView> convertView;
                parent->GetChildAt(childIndex, (IView**)&convertView);

                AutoPtr<IMenuItemImpl> oldItem;
                if (IMenuItemView::Probe(convertView) != NULL) {
                    IMenuItemView::Probe(convertView)->GetItemData((IMenuItemImpl**)&oldItem);
                }

                AutoPtr<IView> itemView = GetItemView(item, convertView, parent);
                if (item != oldItem) {
                    // Don't let old states linger with new data.
                    itemView->SetPressed(FALSE);
                    itemView->JumpDrawablesToCurrentState();
                }

                if (itemView != convertView) {
                    AddItemView(itemView, childIndex);
                }
                childIndex++;
            }
        }
    }

    // Remove leftover views.
    Int32 childCount = 0;
    while (childIndex < (parent->GetChildCount(&childCount), childCount)) {
        if (!FilterLeftoverView(parent, childIndex)) {
            childIndex++;
        }
    }

    return NOERROR;
}

void BaseMenuPresenter::AddItemView(
    /* [in] */ IView* itemView,
    /* [in] */ Int32 childIndex)
{
    assert(itemView != NULL);
    AutoPtr<IViewParent> parent;
    itemView->GetParent((IViewParent**)&parent);
    AutoPtr<IViewGroup> currentParent = IViewGroup::Probe(parent);
    if (currentParent != NULL) {
        assert(IViewManager::Probe(currentParent) != NULL);
        IViewManager::Probe(currentParent)->RemoveView(itemView);
    }

    assert(IViewGroup::Probe(mMenuView) != NULL);
    IViewGroup::Probe(mMenuView)->AddView(itemView, childIndex);
}

Boolean BaseMenuPresenter::FilterLeftoverView(
    /* [in] */ IViewGroup* parent,
    /* [in] */ Int32 childIndex)
{
    assert(parent != NULL);
    parent->RemoveViewAt(childIndex);
    return TRUE;
}

ECode BaseMenuPresenter::SetCallback(
    /* [in] */ IMenuPresenterCallback* cb)
{
    mCallback = cb;
    return NOERROR;
}

AutoPtr<IMenuItemView> BaseMenuPresenter::CreateItemView(
    /* [in] */ IViewGroup* parent)
{
    AutoPtr<IView> view;
    mSystemInflater->Inflate(mItemLayoutRes, parent, FALSE, (IView**)&view);
    return IMenuItemView::Probe(view);
}

AutoPtr<IView> BaseMenuPresenter::GetItemView(
    /* [in] */ IMenuItemImpl* item,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent)
{
    AutoPtr<IMenuItemView> itemView;
    if (IMenuItemView::Probe(convertView) != NULL) {
        itemView = IMenuItemView::Probe(convertView);
    } else {
        itemView = CreateItemView(parent);
    }

    ((IBaseMenuPresenter*)(this->Probe(EIID_IBaseMenuPresenter)))->BindItemView(item, itemView);
    return IView::Probe(itemView);
}

Boolean BaseMenuPresenter::ShouldIncludeItem(
    /* [in] */ Int32 childIndex,
    /* [in] */ IMenuItemImpl* item)
{
    return TRUE;
}

ECode BaseMenuPresenter::OnCloseMenu(
    /* [in] */ IMenuBuilder* menu,
    /* [in] */ Boolean allMenusAreClosing)
{
    if (mCallback != NULL) {
        return mCallback->OnCloseMenu(menu, allMenusAreClosing);
    }

    return NOERROR;
}

ECode BaseMenuPresenter::OnSubMenuSelected(
    /* [in] */ ISubMenuBuilder* menu,
    /* [out] */ Boolean* result)
{
    assert(result != NULL);
    if (mCallback != NULL) {
        return mCallback->OnOpenSubMenu(menu, result);
    }

    *result = FALSE;
    return NOERROR;
}

ECode BaseMenuPresenter::FlagActionItems(
    /* [out] */ Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode BaseMenuPresenter::ExpandItemActionView(
    /* [in] */ IMenuBuilder* menu,
    /* [in] */ IMenuItemImpl* item,
    /* [out] */ Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode BaseMenuPresenter::CollapseItemActionView(
    /* [in] */ IMenuBuilder* menu,
    /* [in] */ IMenuItemImpl* item,
    /* [out] */ Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode BaseMenuPresenter::GetId(
    /* [out] */ Int32* id)
{
    assert(id != NULL);
    *id = mId;
    return NOERROR;
}

ECode BaseMenuPresenter::SetId(
    /* [in] */ Int32 id)
{
    mId = id;
    return NOERROR;
}

ECode BaseMenuPresenter::Init(
    /* [in] */ IContext* context,
    /* [in] */ Int32 menuLayoutRes,
    /* [in] */ Int32 itemLayoutRes)
{
    mSystemContext = context;
    mMenuLayoutRes = menuLayoutRes;
    mItemLayoutRes = itemLayoutRes;
    return LayoutInflater::From(context, (ILayoutInflater**)&mSystemInflater);
}

} // namespace Menu
} // namespace View
} // namespace Droid
} // namespace Elastos
