
#include "view/menu/MenuPopupHelper.h"
#include "view/menu/MenuBuilderBase.h"
#include "view/menu/CListMenuItemView.h"
#include "view/menu/CMenuPopupHelper.h"
#include "view/LayoutInflater.h"
#include "widget/CFrameLayout.h"
#include "widget/CListPopupWindow.h"
#include <elastos/core/Math.h>
#include <elastos/utility/etl/List.h>
#include "R.h"

using Elastos::Utility::Etl::List;
using Elastos::Droid::R;
using Elastos::Droid::Utility::IDisplayMetrics;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Graphics::Drawable::IDrawable;
using Elastos::Droid::Widget::EIID_IAdapterViewOnItemClickListener;
using Elastos::Droid::Widget::EIID_IPopupWindowOnDismissListener;
using Elastos::Droid::Widget::EIID_IBaseAdapter;
using Elastos::Droid::Widget::EIID_IListAdapter;
using Elastos::Droid::Widget::EIID_IAdapter;
using Elastos::Droid::Widget::IAdapter;
using Elastos::Droid::Widget::IListView;
using Elastos::Droid::Widget::IPopupWindow;
using Elastos::Droid::Widget::CFrameLayout;
using Elastos::Droid::Widget::IFrameLayout;
using Elastos::Droid::Widget::CListPopupWindow;
using Elastos::Droid::Widget::EIID_ISpinnerAdapter;

namespace Elastos {
namespace Droid {
namespace View {
namespace Menu {

String MenuPopupHelper::TAG = String("MenuPopupHelper");
const Int32 MenuPopupHelper::ITEM_LAYOUT = R::layout::popup_menu_item_layout;

MenuPopupHelper::_MenuAdapter::_MenuAdapter(
    /* [in] */ IMenuBuilder* menu,
    /* [in] */ MenuPopupHelper* host)
    : mExpandedIndex(-1)
    , mHost(host)
{
    assert(menu);
    AutoPtr<IWeakReferenceSource> wrs = IWeakReferenceSource::Probe(menu);
    assert(wrs != NULL && "Error: Invalid MenuBuilder, IWeakReferenceSource not implemented!");
    wrs->GetWeakReference((IWeakReference**)&mWeakAdapterMenu);
}

Int32 MenuPopupHelper::_MenuAdapter::GetCount()
{
    AutoPtr<IWeakReferenceSource> wrs;
    mWeakAdapterMenu->Resolve(EIID_IWeakReferenceSource, (IInterface**)&wrs);
    if (wrs == NULL) {
        return 0;
    }

    MenuBuilderBase* base = reinterpret_cast<MenuBuilderBase*>(wrs->Probe(EIID_MenuBuilderBase));
    assert(base != NULL);
    AutoPtr< List<AutoPtr<IMenuItemImpl> > > items = mHost->mOverflowOnly ?
            base->GetNonActionItems() : base->GetVisibleItems();
    if (mExpandedIndex < 0) {
        return items->GetSize();
    }
    return items->GetSize() - 1;
}

AutoPtr<IInterface> MenuPopupHelper::_MenuAdapter::GetItem(
    /* [in] */ Int32 position)
{
    AutoPtr<IWeakReferenceSource> wrs;
    mWeakAdapterMenu->Resolve(EIID_IWeakReferenceSource, (IInterface**)&wrs);
    if (wrs == NULL) {
        return NULL;
    }

    MenuBuilderBase* base = reinterpret_cast<MenuBuilderBase*>(wrs->Probe(EIID_MenuBuilderBase));
    assert(base != NULL);
    AutoPtr< List<AutoPtr<IMenuItemImpl> > > items = mHost->mOverflowOnly ?
            base->GetNonActionItems() : base->GetVisibleItems();
    if (mExpandedIndex >= 0 && position >= mExpandedIndex) {
        position++;
    }
    return (*items)[position];
}

Int64 MenuPopupHelper::_MenuAdapter::GetItemId(
    /* [in] */ Int32 position)
{
    // Since a menu item's ID is optional, we'll use the position as an
    // ID for the item in the AdapterView
    return position;
}

AutoPtr<IView> MenuPopupHelper::_MenuAdapter::GetView(
    /* [in] */ Int32 position,
    /* [in, out?] */ IView* _convertView,
    /* [in] */ IViewGroup* parent)
{
    AutoPtr<IView> convertView = _convertView;
    if (convertView == NULL) {
        mHost->mInflater->Inflate(ITEM_LAYOUT, parent, FALSE, (IView**)&convertView);
    }

    AutoPtr<IMenuItemView> itemView = IMenuItemView::Probe(convertView);
    if (mHost->mForceShowIcon) {
        ((CListMenuItemView*)convertView.Get())->SetForceShowIcon(TRUE);
    }

    itemView->Initialize(IMenuItemImpl::Probe(GetItem(position)), 0);
    return convertView;
}

ECode MenuPopupHelper::_MenuAdapter::NotifyDataSetChanged()
{
    FindExpandedIndex();
    return BaseAdapter::NotifyDataSetChanged();
}

ECode MenuPopupHelper::_MenuAdapter::NotifyDataSetInvalidated()
{
    return BaseAdapter::NotifyDataSetInvalidated();
}

void MenuPopupHelper::_MenuAdapter::FindExpandedIndex()
{
    AutoPtr<IMenuItemImpl> expandedItem;
    mHost->mMenu->GetExpandedItem((IMenuItemImpl**)&expandedItem);
    if (expandedItem != NULL) {
        MenuBuilderBase* base = reinterpret_cast<MenuBuilderBase*>(mHost->mMenu->Probe(EIID_MenuBuilderBase));
        assert(base != NULL);

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


IADAPTER_METHODS_IMPL(MenuPopupHelper::MenuAdapter, _MenuAdapter)
IBASEADAPTER_METHODS_IMPL(MenuPopupHelper::MenuAdapter, _MenuAdapter)
ILISTADAPTER_METHODS_IMPL(MenuPopupHelper::MenuAdapter, _MenuAdapter)
ISPINNERADAPTER_METHODS_IMPL(MenuPopupHelper::MenuAdapter, _MenuAdapter)

MenuPopupHelper::MenuAdapter::MenuAdapter(
    /* [in] */ IMenuBuilder* menu,
    /* [in] */ MenuPopupHelper* host)
    : _MenuAdapter(menu, host)
{
    FindExpandedIndex();
}

PInterface MenuPopupHelper::MenuAdapter::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_IMenuAdapter == riid) {
        return (IMenuAdapter *)this;
    }
    else if (EIID_IBaseAdapter == riid) {
        return (IBaseAdapter *)this;
    }
    else if (EIID_IListAdapter == riid) {
        return (IListAdapter *)this;
    }
    else if (EIID_ISpinnerAdapter == riid) {
        return (ISpinnerAdapter *)this;
    }
    else if (EIID_IAdapter == riid) {
        return (IAdapter *)(IBaseAdapter*)this;
    }
    else if (EIID_IInterface == riid) {
        return (IInterface*)(IBaseAdapter*)this;
    }
    return NULL;
}

UInt32 MenuPopupHelper::MenuAdapter::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 MenuPopupHelper::MenuAdapter::Release()
{
    return ElRefBase::Release();
}

ECode MenuPopupHelper::MenuAdapter::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(IMenuAdapter*)this) {
        *pIID = EIID_IBaseAdapter;
    }
    else if (pObject == (IInterface*)(ISpinnerAdapter*)this) {
        *pIID = EIID_IListAdapter;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

CAR_INTERFACE_IMPL_2(MenuPopupHelper::MyListener, IAdapterViewOnItemClickListener,
    IPopupWindowOnDismissListener)

ECode MenuPopupHelper::MyListener::OnItemClick(
    /* [in] */ IAdapterView* parent,
    /* [in] */ IView* v,
    /* [in] */ Int32 position,
    /* [in] */ Int64 id)
{
    return mOwner->OnItemClick(parent, v, position, id);
}

ECode MenuPopupHelper::MyListener::OnDismiss()
{
    return mOwner->OnDismiss();
}

MenuPopupHelper::MenuPopupHelper()
    : mPopupMaxWidth(0)
    , mOverflowOnly(FALSE)
    , mForceShowIcon(FALSE)
{}

MenuPopupHelper::MenuPopupHelper(
    /* [in] */ IContext* context,
    /* [in] */ IMenuBuilder* menu)
    : mPopupMaxWidth(0)
    , mOverflowOnly(FALSE)
    , mForceShowIcon(FALSE)
{
    Init(context, menu, NULL, FALSE);
}

MenuPopupHelper::MenuPopupHelper(
    /* [in] */ IContext* context,
    /* [in] */ IMenuBuilder* menu,
    /* [in] */ IView* anchorView)
    : mPopupMaxWidth(0)
    , mOverflowOnly(FALSE)
    , mForceShowIcon(FALSE)
{
    Init(context, menu, anchorView, FALSE);
}

MenuPopupHelper::MenuPopupHelper(
    /* [in] */ IContext* context,
    /* [in] */ IMenuBuilder* menu,
    /* [in] */ IView* anchorView,
    /* [in] */ Boolean overflowOnly)
    : mPopupMaxWidth(0)
    , mOverflowOnly(FALSE)
    , mForceShowIcon(FALSE)
{
    Init(context, menu, anchorView, overflowOnly);
}

ECode MenuPopupHelper::SetAnchorView(
    /* [in] */ IView* anchor)
{
    mAnchorView = anchor;
    return NOERROR;
}

ECode MenuPopupHelper::SetForceShowIcon(
    /* [in] */ Boolean forceShow)
{
    mForceShowIcon = forceShow;
    return NOERROR;
}

ECode MenuPopupHelper::Show()
{
    if (!TryShow())
    {
        return E_ILLEGAL_STATE_EXCEPTION;
        //throw new IllegalStateException("MenuPopupHelper cannot be used without an anchor");
    }

    return NOERROR;
}

Boolean MenuPopupHelper::TryShow()
{
    mPopup = NULL;
    CListPopupWindow::New(mContext, NULL, R::attr::popupMenuStyle, (IListPopupWindow**)&mPopup);
    AutoPtr<MyListener> listener = new MyListener(this);
    mPopup->SetOnDismissListener(listener);
    mPopup->SetOnItemClickListener(listener);

    mAdapter = new MenuAdapter(mMenu, this);
    mPopup->SetAdapter((IListAdapter*)mAdapter->Probe(EIID_IListAdapter));
    mPopup->SetModal(TRUE);

    AutoPtr<IView> anchor = mAnchorView;
    if (anchor != NULL) {
        Boolean addGlobalListener = mTreeObserver == NULL;
        mTreeObserver = NULL;
        anchor->GetViewTreeObserver((IViewTreeObserver**)&mTreeObserver); // Refresh to latest
        if (addGlobalListener) mTreeObserver->AddOnGlobalLayoutListener((IOnGlobalLayoutListener*)(this->Probe(EIID_IOnGlobalLayoutListener)));
        anchor->AddOnAttachStateChangeListener((IViewOnAttachStateChangeListener*)(this->Probe(EIID_IViewOnAttachStateChangeListener)));
        mPopup->SetAnchorView(anchor);
    }
    else {
        return FALSE;
    }

    mPopup->SetContentWidth(Elastos::Core::Math::Min(MeasureContentWidth((IListAdapter*)mAdapter->Probe(EIID_IListAdapter)), mPopupMaxWidth));
    mPopup->SetInputMethodMode(IPopupWindow::INPUT_METHOD_NOT_NEEDED);
    mPopup->Show();

    AutoPtr<IListView> listView;
    mPopup->GetListView((IListView**)&listView);
    listView->SetOnKeyListener((IViewOnKeyListener*)(this->Probe(EIID_IViewOnKeyListener)));
    return TRUE;
}

ECode MenuPopupHelper::Dismiss()
{
    if (IsShowing()) {
        // hold the last reference of mPopup until Dismiss() finished
        AutoPtr<IListPopupWindow> popup = mPopup;
        mPopup->Dismiss();
    }

    return NOERROR;
}

ECode MenuPopupHelper::OnDismiss()
{
    mPopup = NULL;
    mMenu->Close();
    if (mTreeObserver != NULL) {
        Boolean tmp = FALSE;
        if (!(mTreeObserver->IsAlive(&tmp), tmp)) {
            mTreeObserver = NULL;
            mAnchorView->GetViewTreeObserver((IViewTreeObserver**)&mTreeObserver);
        }

        mTreeObserver->RemoveGlobalOnLayoutListener((IOnGlobalLayoutListener*)(this->Probe(EIID_IOnGlobalLayoutListener)));
        mTreeObserver = NULL;
    }

    return mAnchorView->RemoveOnAttachStateChangeListener((IViewOnAttachStateChangeListener*)(this->Probe(EIID_IViewOnAttachStateChangeListener)));
}

Boolean MenuPopupHelper::IsShowing()
{
    Boolean tmp = FALSE;
    return mPopup != NULL && (mPopup->IsShowing(&tmp), tmp);
}

ECode MenuPopupHelper::OnItemClick(
    /* [in] */ IAdapterView* parent,
    /* [in] */ IView* view,
    /* [in] */ Int32 position,
    /* [in] */ Int64 id)
{
    AutoPtr<IMenuBuilder> adapterMenu;
    mAdapter->mWeakAdapterMenu->Resolve(EIID_IMenuBuilder, (IInterface**)&adapterMenu);
    if (adapterMenu != NULL) {
        Boolean tmp;
        AutoPtr<IMenuItem> item;
        mAdapter->GetItem(position, (IInterface**)&item);
        return adapterMenu->PerformItemAction(item, 0, &tmp);
    }
    return NOERROR;
}

ECode MenuPopupHelper::OnKey(
    /* [in] */ IView* v,
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    assert(result != NULL);
    Int32 action = 0;
    event->GetAction(&action);
    if (action == IKeyEvent::ACTION_UP && keyCode == IKeyEvent::KEYCODE_MENU) {
        Dismiss();
        *result = TRUE;
        return NOERROR;
    }

    *result = FALSE;
    return NOERROR;
}

Int32 MenuPopupHelper::MeasureContentWidth(
    /* [in] */ IListAdapter* adapter)
{
    // Menus don't tend to be long, so this is more sane than it looks.
    Int32 width = 0;
    AutoPtr<IView> itemView;
    Int32 itemType = 0;
    const Int32 widthMeasureSpec =
        View::MeasureSpec::MakeMeasureSpec(0, View::MeasureSpec::UNSPECIFIED);
    const Int32 heightMeasureSpec =
        View::MeasureSpec::MakeMeasureSpec(0, View::MeasureSpec::UNSPECIFIED);
    Int32 count = 0;
    adapter->GetCount(&count);
    for (Int32 i = 0; i < count; i++) {
        Int32 positionType = 0;
        adapter->GetItemViewType(i, &positionType);
        if (positionType != itemType) {
            itemType = positionType;
            itemView = NULL;
        }

        if (mMeasureParent == NULL) {
            CFrameLayout::New(mContext, (IFrameLayout**)&mMeasureParent);
        }
        AutoPtr<IView> temp;
        adapter->GetView(i, itemView, mMeasureParent, (IView**)&temp);
        itemView = temp;
        VIEW_PROBE(itemView)->Measure(widthMeasureSpec, heightMeasureSpec);
        width = Elastos::Core::Math::Max(width, ((View*)itemView->Probe(EIID_View))->GetMeasuredWidth());
    }

    return width;
}

ECode MenuPopupHelper::OnGlobalLayout()
{
    if (IsShowing()) {
        AutoPtr<IView> anchor = mAnchorView;
        Boolean tmp = FALSE;
        if (anchor == NULL || !(anchor->IsShown(&tmp), tmp)) {
            Dismiss();
        } else if (IsShowing()) {
            // Recompute window size and position
            return mPopup->Show();
        }
    }

    return NOERROR;
}

ECode MenuPopupHelper::OnViewAttachedToWindow(
    /* [in] */ IView* v)
{
    return NOERROR;
}

ECode MenuPopupHelper::OnViewDetachedFromWindow(
    /* [in] */ IView* v)
{
    if (mTreeObserver != NULL) {
        Boolean tmp = FALSE;
        if (!(mTreeObserver->IsAlive(&tmp), tmp)) {
            mTreeObserver = NULL;
            v->GetViewTreeObserver((IViewTreeObserver**)&mTreeObserver);
        }

        mTreeObserver->RemoveGlobalOnLayoutListener((IOnGlobalLayoutListener*)(this->Probe(EIID_IOnGlobalLayoutListener)));
    }

    return v->RemoveOnAttachStateChangeListener((IViewOnAttachStateChangeListener*)(this->Probe(EIID_IViewOnAttachStateChangeListener)));
}

ECode MenuPopupHelper::InitForMenu(
    /* [in] */ IContext* context,
    /* [in] */ IMenuBuilder* menu)
{
    // Don't need to do anything; we added as a presenter in the constructor.
    return NOERROR;
}

ECode MenuPopupHelper::GetMenuView(
    /* [in] */ IViewGroup* root,
    /* [out] */ IMenuView** view)
{
    //throw new UnsupportedOperationException("MenuPopupHelpers manage their own views");
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode MenuPopupHelper::UpdateMenuView(
    /* [in] */ Boolean cleared)
{
    if (mAdapter != NULL) {
        mAdapter->NotifyDataSetChanged();
    }

    return NOERROR;
}

ECode MenuPopupHelper::SetCallback(
    /* [in] */ IMenuPresenterCallback* cb)
{
    mPresenterCallback = cb;
    return NOERROR;
}

ECode MenuPopupHelper::OnSubMenuSelected(
    /* [in] */ ISubMenuBuilder* subMenu,
    /* [out] */ Boolean* selected)
{
    assert(selected != NULL && subMenu != NULL);
    Boolean tmp = FALSE;
    if (subMenu->HasVisibleItems(&tmp), tmp) {
        AutoPtr<IMenuPopupHelper> subPopup;
        CMenuPopupHelper::New(mContext, subMenu, mAnchorView, FALSE, (IMenuPopupHelper**)&subPopup);
        (IMenuPresenter::Probe(subPopup))->SetCallback(mPresenterCallback);

        Boolean preserveIconSpacing = FALSE;
        Int32 count = 0;
        subMenu->GetSize(&count);
        for (Int32 i = 0; i < count; i++) {
            AutoPtr<IMenuItem> childItem;
            subMenu->GetItem(i, (IMenuItem**)&childItem);
            assert(childItem != NULL);

            AutoPtr<IDrawable> icon;
            childItem->GetIcon((IDrawable**)&icon);
            tmp = FALSE;
            if ((childItem->IsVisible(&tmp), tmp) && icon != NULL) {
                preserveIconSpacing = TRUE;
                break;
            }
        }

        subPopup->SetForceShowIcon(preserveIconSpacing);

        Boolean tryShow = FALSE;
        if (subPopup->TryShow(&tryShow), tryShow) {
            if (mPresenterCallback != NULL) {
                mPresenterCallback->OnOpenSubMenu(subMenu, &tmp);
            }
            *selected = TRUE;
            return NOERROR;
        }
    }

    *selected = FALSE;
    return NOERROR;
}

ECode MenuPopupHelper::OnCloseMenu(
    /* [in] */ IMenuBuilder* menu,
    /* [in] */ Boolean allMenusAreClosing)
{
    // Only care about the (sub)menu we're presenting.
    if (menu != mMenu) return NOERROR;

    Dismiss();
    if (mPresenterCallback != NULL) {
        mPresenterCallback->OnCloseMenu(menu, allMenusAreClosing);
    }

    return NOERROR;
}

ECode MenuPopupHelper::FlagActionItems(
    /* [out] */ Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode MenuPopupHelper::ExpandItemActionView(
    /* [in] */ IMenuBuilder* menu,
    /* [in] */ IMenuItemImpl* item,
    /* [out] */ Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode MenuPopupHelper::CollapseItemActionView(
    /* [in] */ IMenuBuilder* menu,
    /* [in] */ IMenuItemImpl* item,
    /* [out] */ Boolean* result)
{
    assert(result != NULL);
    *result = FALSE;
    return NOERROR;
}

ECode MenuPopupHelper::GetId(
    /* [out] */ Int32* id)
{
    assert(id != NULL);
    *id = 0;
    return NOERROR;
}

ECode MenuPopupHelper::OnSaveInstanceState(
    /* [out] */ IParcelable** parcel)
{
    assert(parcel != NULL);
    *parcel = NULL;
    return NOERROR;
}

ECode MenuPopupHelper::OnRestoreInstanceState(
    /* [in] */ IParcelable* state)
{
    return NOERROR;
}

ECode MenuPopupHelper::Init(
    /* [in] */ IContext* context,
    /* [in] */ IMenuBuilder* menu,
    /* [in] */ IView* anchorView,
    /* [in] */ Boolean overflowOnly)
{
    mContext = context;
    LayoutInflater::From(context, (ILayoutInflater**)&mInflater);
    mMenu = menu;
    mOverflowOnly = overflowOnly;

    AutoPtr<IResources> res;
    context->GetResources((IResources**)&res);
    assert(res != NULL);

    AutoPtr<IDisplayMetrics> display;
    res->GetDisplayMetrics((IDisplayMetrics**)&display);
    assert(display != NULL);

    Int32 widthPixels = 0, pixelSize = 0;
    display->GetWidthPixels(&widthPixels);
    res->GetDimensionPixelSize(R::dimen::config_prefDialogWidth, &pixelSize);
    mPopupMaxWidth = Elastos::Core::Math::Max(widthPixels / 2, pixelSize);

    mAnchorView = anchorView;

    menu->AddMenuPresenter((IMenuPresenter*)(this->Probe(EIID_IMenuPresenter)));
    return NOERROR;
}

} // namespace Menu
} // namespace View
} // namespace Droid
} // namespace Elastos
