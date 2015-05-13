
#include "view/menu/ListMenuPresenter.h"
#include "view/menu/MenuBuilderBase.h"
#include "view/menu/CMenuDialogHelper.h"
#include <elastos/List.h>
#include "view/LayoutInflater.h"
#include "os/CBundle.h"
#include "view/CContextThemeWrapper.h"
#include "R.h"

using Elastos::Utility::List;
using Elastos::Droid::R;
using Elastos::Droid::Os::CBundle;
using Elastos::Droid::Widget::EIID_IBaseAdapter;
using Elastos::Droid::Widget::IAdapter;
using Elastos::Droid::Widget::EIID_IAdapter;
using Elastos::Droid::Widget::EIID_IAdapterViewOnItemClickListener;
using Elastos::Droid::Widget::EIID_IListAdapter;
using Elastos::Droid::Widget::EIID_ISpinnerAdapter;

namespace Elastos {
namespace Droid {
namespace View {
namespace Menu {

String ListMenuPresenter::TAG = String("ListMenuPresenter");
String ListMenuPresenter::VIEWS_TAG = String("android:menu:list");

ListMenuPresenter::_MenuAdapter::_MenuAdapter(
    /* [in] */ ListMenuPresenter* host)
    : mExpandedIndex(-1)
    , mHost(host)
{

}

AutoPtr<IInterface> ListMenuPresenter::_MenuAdapter::GetItem(
    /* [in] */ Int32 position)
{
    MenuBuilderBase* base = (MenuBuilderBase*)mHost->mMenu->Probe(EIID_MenuBuilderBase);
    assert(base != NULL);
    AutoPtr< List<AutoPtr<IMenuItemImpl> > > items = base->GetNonActionItems();
    position += mHost->mItemIndexOffset;
    if (mExpandedIndex >= 0 && position >= mExpandedIndex) {
        position++;
    }

    AutoPtr<IInterface> item = (*items)[position];
    return item;
}

Int32 ListMenuPresenter::_MenuAdapter::GetCount()
{
    MenuBuilderBase* base = (MenuBuilderBase*)mHost->mMenu->Probe(EIID_MenuBuilderBase);
    assert(base != NULL);
    AutoPtr< List<AutoPtr<IMenuItemImpl> > > items = base->GetNonActionItems();
    Int32 count = items->GetSize() - mHost->mItemIndexOffset;
    if (mExpandedIndex < 0) {
        return count;
    }

    return count - 1;
}

Int64 ListMenuPresenter::_MenuAdapter::GetItemId(
    /* [in] */ Int32 position)
{
    // Since a menu item's ID is optional, we'll use the position as an
    // ID for the item in the AdapterView
    return position;
}

AutoPtr<IView> ListMenuPresenter::_MenuAdapter::GetView(
    /* [in] */ Int32 position,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent)
{
    if (convertView == NULL) {
        mHost->mInflater->InflateEx2(mHost->mItemLayoutRes, parent, FALSE, (IView**)&convertView);
    }

    AutoPtr<IMenuItemView> itemView = IMenuItemView::Probe(convertView);
    AutoPtr<IMenuItemImpl> item = IMenuItemImpl::Probe(GetItem(position));
    itemView->Initialize(item, 0);
    return convertView;
}

void ListMenuPresenter::_MenuAdapter::FindExpandedIndex()
{
    MenuBuilderBase* base = (MenuBuilderBase*)mHost->mMenu->Probe(EIID_MenuBuilderBase);
    assert(base != NULL);
    AutoPtr<IMenuItemImpl> expandedItem = base->GetExpandedItem();
    if (expandedItem != NULL) {
        AutoPtr< List<AutoPtr<IMenuItemImpl> > > items = base->GetNonActionItems();
        const Int32 count = items->GetSize();
        for (Int32 i = 0; i < count; i++) {
            AutoPtr<IMenuItemImpl> item = (*items)[i];
            if (item == expandedItem) {
                mExpandedIndex = i;
                return;
            }
        }
    }

    mExpandedIndex = -1;
}

ECode ListMenuPresenter::_MenuAdapter::NotifyDataSetChanged()
{
    FindExpandedIndex();
    return BaseAdapter::NotifyDataSetChanged();
}

ECode ListMenuPresenter::_MenuAdapter::NotifyDataSetInvalidated()
{
    return BaseAdapter::NotifyDataSetInvalidated();
}

IADAPTER_METHODS_IMPL(ListMenuPresenter::MenuAdapter, ListMenuPresenter::_MenuAdapter)
IBASEADAPTER_METHODS_IMPL(ListMenuPresenter::MenuAdapter, ListMenuPresenter::_MenuAdapter)
ILISTADAPTER_METHODS_IMPL(ListMenuPresenter::MenuAdapter, ListMenuPresenter::_MenuAdapter)
ISPINNERADAPTER_METHODS_IMPL(ListMenuPresenter::MenuAdapter, ListMenuPresenter::_MenuAdapter)

ListMenuPresenter::MenuAdapter::MenuAdapter(
    /* [in] */ ListMenuPresenter* host)
    : _MenuAdapter(host)
{
    FindExpandedIndex();
}

PInterface ListMenuPresenter::MenuAdapter::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_IInterface == riid) {
        return (IBaseAdapter *)this;
    }
    else if (EIID_IBaseAdapter == riid) {
        return (IBaseAdapter *)this;
    }
    else if (EIID_ISpinnerAdapter == riid) {
        return (ISpinnerAdapter *)this;
    }
    else if (EIID_IListAdapter == riid) {
        return (IListAdapter*)(IBaseAdapter *)this;
    }
    else if (EIID_IAdapter == riid) {
        return (IAdapter*)(IBaseAdapter *)this;
    }
    return NULL;
}

UInt32 ListMenuPresenter::MenuAdapter::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ListMenuPresenter::MenuAdapter::Release()
{
    return ElRefBase::Release();
}

ECode ListMenuPresenter::MenuAdapter::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(IBaseAdapter*)this) {
        *pIID = EIID_IBaseAdapter;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

CAR_INTERFACE_IMPL(ListMenuPresenter::OnItemClickListener, IAdapterViewOnItemClickListener)

ECode ListMenuPresenter::OnItemClickListener::OnItemClick(
    /* [in] */ IAdapterView* parent,
    /* [in] */ IView* v,
    /* [in] */ Int32 position,
    /* [in] */ Int64 id)
{
    return mOwner->OnItemClick(parent, v, position, id);
}

ListMenuPresenter::ListMenuPresenter(
    /* [in] */ IContext* context,
    /* [in] */ Int32 itemLayoutRes)
    : mItemIndexOffset(0)
    , mThemeRes(0)
    , mItemLayoutRes(0)
    , mId(0)
{
    Init(itemLayoutRes, 0);
    mContext = context;
    LayoutInflater::From(mContext, (ILayoutInflater**)&mInflater);
}

ListMenuPresenter::ListMenuPresenter()
    : mItemIndexOffset(0)
    , mThemeRes(0)
    , mItemLayoutRes(0)
    , mId(0)
{}

ListMenuPresenter::ListMenuPresenter(
    /* [in] */ Int32 itemLayoutRes,
    /* [in] */ Int32 themeRes)
    : mItemIndexOffset(0)
    , mThemeRes(themeRes)
    , mItemLayoutRes(itemLayoutRes)
    , mId(0)
{}

ECode ListMenuPresenter::InitForMenu(
    /* [in] */ IContext* context,
    /* [in] */ IMenuBuilder* menu)
{
    if (mThemeRes != 0) {
        mContext = NULL;
        mInflater = NULL;
        CContextThemeWrapper::New(context, mThemeRes, (IContextThemeWrapper**)&mContext);
        LayoutInflater::From(mContext, (ILayoutInflater**)&mInflater);
    } else if (mContext != NULL) {
        mContext = context;
        if (mInflater == NULL) {
            LayoutInflater::From(mContext, (ILayoutInflater**)&mInflater);
        }
    }

    mMenu = menu;
    if (mAdapter != NULL) {
        mAdapter->NotifyDataSetChanged();
    }

    return NOERROR;
}

ECode ListMenuPresenter::GetMenuView(
    /* [in] */ IViewGroup* root,
    /* [out] */ IMenuView** view)
{
    assert(view != NULL);
    if (mMenuView == NULL) {
        AutoPtr<IView> tmpView;
        mInflater->InflateEx2(R::layout::expanded_menu_layout, root, FALSE, (IView**)&tmpView);
        mMenuView = IExpandedMenuView::Probe(tmpView);
        if (mAdapter == NULL) {
            mAdapter = new MenuAdapter(this);
        }

        mMenuView->SetAdapter((IAdapter*)mAdapter->Probe(EIID_IAdapter));
        mMenuView->SetOnItemClickListener(new OnItemClickListener(this));
    }

    *view = IMenuView::Probe(mMenuView);
    INTERFACE_ADDREF(*view);

    return NOERROR;
}

AutoPtr<IListAdapter> ListMenuPresenter::GetAdapter()
{
    if (mAdapter == NULL) {
        mAdapter = new MenuAdapter(this);
    }

    return (IListAdapter*)mAdapter->Probe(EIID_IListAdapter);
}

ECode ListMenuPresenter::UpdateMenuView(
    /* [in] */ Boolean cleared)
{
    if (mAdapter != NULL) mAdapter->NotifyDataSetChanged();
    return NOERROR;
}

ECode ListMenuPresenter::SetCallback(
    /* [in] */ IMenuPresenterCallback* cb)
{
    mCallback = cb;
    return NOERROR;
}

ECode ListMenuPresenter::OnSubMenuSelected(
    /* [in] */ ISubMenuBuilder* subMenu,
    /* [out] */ Boolean* selected)
{
    assert(selected != NULL);
    *selected = FALSE;
    Boolean has = FALSE;
    if (!(subMenu->HasVisibleItems(&has), has)) {
        return NOERROR;
    }

    // The window manager will give us a token.
    AutoPtr<IMenuDialogHelper> helper;
    CMenuDialogHelper::New(subMenu, (IMenuDialogHelper**)&helper);
    helper->Show(NULL);
    if (mCallback != NULL) {
        Boolean tmp = FALSE;
        mCallback->OnOpenSubMenu(subMenu, &tmp);
    }

    *selected = TRUE;
    return NOERROR;
}

ECode ListMenuPresenter::OnCloseMenu(
    /* [in] */ IMenuBuilder* menu,
    /* [in] */ Boolean allMenusAreClosing)
{
    if (mCallback != NULL) {
        return mCallback->OnCloseMenu(menu, allMenusAreClosing);
    }

    return NOERROR;
}

Int32 ListMenuPresenter::GetItemIndexOffset()
{
    return mItemIndexOffset;
}

ECode ListMenuPresenter::SetItemIndexOffset(
    /* [in] */ Int32 offset)
{
    mItemIndexOffset = offset;
    if (mMenuView != NULL) {
        UpdateMenuView(FALSE);
    }

    return NOERROR;
}

ECode ListMenuPresenter::OnItemClick(
    /* [in] */ IAdapterView* parent,
    /* [in] */ IView* view,
    /* [in] */ Int32 position,
    /* [in] */ Int64 id)
{
    MenuBuilderBase* base = (MenuBuilderBase*)mMenu->Probe(EIID_MenuBuilderBase);
    assert(base != NULL);

    AutoPtr<IMenuItem> item;
    mAdapter->GetItem(position, (IInterface**)&item);
    base->PerformItemAction(item, 0);
    return NOERROR;
}

ECode ListMenuPresenter::FlagActionItems(
    /* [out] */ Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode ListMenuPresenter::ExpandItemActionView(
    /* [in] */ IMenuBuilder* menu,
    /* [in] */ IMenuItemImpl* item,
    /* [out] */ Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode ListMenuPresenter::CollapseItemActionView(
    /* [in] */ IMenuBuilder* menu,
    /* [in] */ IMenuItemImpl* item,
    /* [out] */ Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode ListMenuPresenter::SaveHierarchyState(
    /* [in] */ IBundle* outState)
{
    assert(0);
    //TODO
    // SparseArray<Parcelable> viewStates = new SparseArray<Parcelable>();
    // if (mMenuView != null) {
    //     ((View) mMenuView).saveHierarchyState(viewStates);
    // }
    // outState.putSparseParcelableArray(VIEWS_TAG, viewStates);
    return NOERROR;
}

ECode ListMenuPresenter::RestoreHierarchyState(
    /* [in] */ IBundle* inState)
{
    assert(0);
    //TODO
    // SparseArray<Parcelable> viewStates = inState.getSparseParcelableArray(VIEWS_TAG);
    // if (viewStates != null) {
    //     ((View) mMenuView).restoreHierarchyState(viewStates);
    // }
    return NOERROR;
}

ECode ListMenuPresenter::SetId(
    /* [in] */ Int32 id)
{
    mId = id;
    return NOERROR;
}

ECode ListMenuPresenter::GetId(
    /* [out] */ Int32* id)
{
    assert(id != NULL);
    *id = mId;
    return NOERROR;
}

ECode ListMenuPresenter::OnSaveInstanceState(
    /* [out] */ IParcelable** parcel)
{
    assert(parcel != NULL);
    *parcel = NULL;
    if (mMenuView == NULL) {
        return NOERROR;
    }

    CBundle::New((IBundle**)&parcel);
    SaveHierarchyState(IBundle::Probe(*parcel));
    return NOERROR;
}

ECode ListMenuPresenter::OnRestoreInstanceState(
    /* [in] */ IParcelable* state)
{
    RestoreHierarchyState(IBundle::Probe(state));
    return NOERROR;
}

ECode ListMenuPresenter::Init(
    /* [in] */ IContext* ctx,
    /* [in] */ Int32 itemLayoutRes)
{
    Init(itemLayoutRes, 0);
    mContext = ctx;
    LayoutInflater::From(mContext, (ILayoutInflater**)&mInflater);
    return NOERROR;
}

ECode ListMenuPresenter::Init(
    /* [in] */ Int32 itemLayoutRes,
    /* [in] */ Int32 themeRes)
{
    mItemLayoutRes = itemLayoutRes;
    mThemeRes = themeRes;
    return NOERROR;
}

} // namespace Menu
} // namespace View
} // namespace Droid
} // namespace Elastos
