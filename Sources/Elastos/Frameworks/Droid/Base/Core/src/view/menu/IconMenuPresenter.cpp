
#include "view/menu/IconMenuPresenter.h"
#include "view/CContextThemeWrapper.h"
#include "view/menu/MenuBuilderBase.h"
#include "view/menu/MenuItemImpl.h"
#include "view/menu/CIconMenuView.h"
#include "view/menu/CMenuDialogHelper.h"
#include "view/menu/IconMenuItemView.h"
#include "os/CBundle.h"
#include "R.h"
#include <elastos/List.h>

using Elastos::Utility::List;
using Elastos::Droid::R;
using Elastos::Droid::Os::CBundle;
using Elastos::Droid::View::IContextThemeWrapper;

namespace Elastos {
namespace Droid {
namespace View {
namespace Menu {

String IconMenuPresenter::VIEWS_TAG = String("android:menu:icon");
String IconMenuPresenter::OPEN_SUBMENU_KEY = String("android:menu:icon:submenu");


IconMenuPresenter::SubMenuPresenterCallback::SubMenuPresenterCallback(
    /* [in] */ IconMenuPresenter* host)
    : mHost(host)
{
}

PInterface IconMenuPresenter::SubMenuPresenterCallback::Probe(
    /* [in] */ REIID riid)
{
    if (EIID_IMenuPresenterCallback == riid) {
        return (IMenuPresenterCallback *)this;
    }

    return NULL;
}

UInt32 IconMenuPresenter::SubMenuPresenterCallback::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 IconMenuPresenter::SubMenuPresenterCallback::Release()
{
    return ElRefBase::Release();
}

ECode IconMenuPresenter::SubMenuPresenterCallback::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(IMenuPresenterCallback*)this) {
        *pIID = EIID_IMenuPresenterCallback;
    }
    else {
        return E_INVALID_ARGUMENT;
    }

    return NOERROR;
}

ECode IconMenuPresenter::SubMenuPresenterCallback::OnCloseMenu(
    /* [in] */ IMenuBuilder* menu,
    /* [in] */ Boolean allMenusAreClosing)
{
    mHost->mOpenSubMenuId = 0;
    if (mHost->mOpenSubMenu != NULL) {
        mHost->mOpenSubMenu->Dismiss();
        mHost->mOpenSubMenu = NULL;
    }

    return NOERROR;
}

ECode IconMenuPresenter::SubMenuPresenterCallback::OnOpenSubMenu(
    /* [in] */ IMenuBuilder* subMenu,
    /* [out] */ Boolean* result)
{
    assert(result != NULL);
    if (subMenu != NULL) {
        AutoPtr<IMenuItem> item;
        ISubMenu::Probe(subMenu)->GetItemEx((IMenuItem**)&item);
        assert(item != NULL);

        item->GetItemId(&(mHost->mOpenSubMenuId));
    }

    *result = FALSE;
    return NOERROR;
}

IconMenuPresenter::IconMenuPresenter()
    : mOpenSubMenuId(0)
    , mMaxItems(-1)
{
    mSubMenuPresenterCallback = new SubMenuPresenterCallback(this);
}

IconMenuPresenter::IconMenuPresenter(
    /* [in] */ IContext* context)
    : mOpenSubMenuId(0)
    , mMaxItems(-1)
{
    mSubMenuPresenterCallback = new SubMenuPresenterCallback(this);
    AutoPtr<IContext> themeContext;
    CContextThemeWrapper::New(context, R::style::Theme_IconMenu, (IContextThemeWrapper**)&themeContext);
    BaseMenuPresenter::Init(themeContext, R::layout::icon_menu_layout,
            R::layout::icon_menu_item_layout);
}

ECode IconMenuPresenter::InitForMenu(
    /* [in] */ IContext* context,
    /* [in] */ IMenuBuilder* menu)
{
    BaseMenuPresenter::InitForMenu(context, menu);
    mMaxItems = -1;
    return NOERROR;
}

ECode IconMenuPresenter::BindItemView(
    /* [in] */ IMenuItemImpl* item,
    /* [in] */ IMenuItemView* itemView)
{
    AutoPtr<IIconMenuItemView> view = IIconMenuItemView::Probe(itemView);
    view->SetItemData(item);

    AutoPtr<IDrawable> drawable;
    item->GetIcon((IDrawable**)&drawable);
    AutoPtr<ICharSequence> csq;
    item->GetTitleForItemView(IMenuItemView::Probe(itemView), (ICharSequence**)&csq);
    IconMenuItemView* vTemp = reinterpret_cast<IconMenuItemView*>(view->Probe(EIID_IconMenuItemView));
    vTemp->Initialize(csq, drawable);
    Boolean visiable = FALSE;
    item->IsVisible(&visiable);
    view->SetVisibility(visiable ? IView::VISIBLE : IView::GONE);

    Boolean enabled = FALSE;
    view->IsEnabled(&enabled);
    view->SetEnabled(enabled);

    AutoPtr<IViewGroupLayoutParams> params = vTemp->GetTextAppropriateLayoutParams();
    view->SetLayoutParams(params);
    return NOERROR;
}

Boolean IconMenuPresenter::ShouldIncludeItem(
    /* [in] */ Int32 childIndex,
    /* [in] */ IMenuItemImpl* item)
{
    assert(item != NULL);

    MenuBuilderBase* base = (MenuBuilderBase*)mMenu->Probe(EIID_MenuBuilderBase);
    assert(base != NULL);
    AutoPtr<List<AutoPtr<IMenuItemImpl> > > itemsToShow = base->GetNonActionItems();
    Boolean fits = ((Int32)itemsToShow->GetSize() == mMaxItems && childIndex < mMaxItems) ||
            childIndex < mMaxItems - 1;

    Boolean tmp = FALSE;
    return fits && !(item->IsActionButton(&tmp), tmp);
}

ECode IconMenuPresenter::Init(
    /* [in] */ IContext* ctx)
{
    AutoPtr<IContext> themeContext;
    CContextThemeWrapper::New(ctx, R::style::Theme_IconMenu, (IContextThemeWrapper**)&themeContext);
    BaseMenuPresenter::Init(themeContext, R::layout::icon_menu_layout,
            R::layout::icon_menu_item_layout);
    return NOERROR;
}

void IconMenuPresenter::AddItemView(
    /* [in] */ IView* itemView,
    /* [in] */ Int32 childIndex)
{
    AutoPtr<IIconMenuItemView> v = IIconMenuItemView::Probe(itemView);
    AutoPtr<IIconMenuView> parent = IIconMenuView::Probe(mMenuView);

    IconMenuItemView* vTemp = reinterpret_cast<IconMenuItemView*>(v->Probe(EIID_IconMenuItemView));
    vTemp->SetIconMenuView(parent);
    v->SetItemInvoker(IMenuBuilderItemInvoker::Probe(parent));
    v->SetBackgroundDrawable(((IconMenuView*)parent.Get())->GetItemBackgroundDrawable());
    BaseMenuPresenter::AddItemView(itemView, childIndex);
}

ECode IconMenuPresenter::OnSubMenuSelected(
    /* [in] */ ISubMenuBuilder* subMenu,
    /* [out] */ Boolean* result)
{
    assert(result != NULL);

    Boolean tmp = FALSE;
    if (!(subMenu->HasVisibleItems(&tmp), tmp)) {
        *result = FALSE;
        return NOERROR;
    }

    // The window manager will give us a token.
    AutoPtr<IMenuDialogHelper> helper;
    CMenuDialogHelper::New(subMenu, (IMenuDialogHelper**)&helper);
    helper->SetPresenterCallback(mSubMenuPresenterCallback);
    helper->Show(NULL);
    mOpenSubMenu = helper;

    AutoPtr<IMenuItemImpl> item;
    (ISubMenu::Probe(subMenu))->GetItemEx((IMenuItem**)&item);
    assert(item != NULL);

    item->GetItemId(&mOpenSubMenuId);
    BaseMenuPresenter::OnSubMenuSelected(subMenu, &tmp);

    *result = TRUE;
    return NOERROR;
}

ECode IconMenuPresenter::UpdateMenuView(
    /* [in] */ Boolean cleared)
{
    AutoPtr<IIconMenuView> menuView = IIconMenuView::Probe(mMenuView);
    if (mMaxItems < 0) {
        mMaxItems = ((CIconMenuView*)menuView.Get())->GetMaxItems();
    }

    MenuBuilderBase* base = (MenuBuilderBase*)mMenu->Probe(EIID_MenuBuilderBase);
    assert(base != NULL);
    AutoPtr< List<AutoPtr<IMenuItemImpl> > > itemsToShow = base->GetNonActionItems();
    Boolean needsMore = (Int32)itemsToShow->GetSize() > mMaxItems;
    BaseMenuPresenter::UpdateMenuView(cleared);

    AutoPtr<IViewParent> parent;
    if (needsMore && (mMoreView == NULL || (mMoreView->GetParent((IViewParent**)&parent), parent.Get()) != IViewParent::Probe(menuView))) {
        if (mMoreView == NULL) {
            mMoreView = ((CIconMenuView*)menuView.Get())->CreateMoreItemView();

            AutoPtr<IDrawable> drawable = ((CIconMenuView*)menuView.Get())->GetItemBackgroundDrawable();
            mMoreView->SetBackgroundDrawable(drawable);
        }

        menuView->AddView(mMoreView);
    } else if (!needsMore && mMoreView != NULL) {
        assert(IViewParent::Probe(menuView) != NULL);
        IViewManager::Probe(menuView)->RemoveView(mMoreView);
    }

    ((CIconMenuView*)menuView.Get())->SetNumActualItemsShown(needsMore ? mMaxItems - 1 : itemsToShow->GetSize());
    return NOERROR;
}

Boolean IconMenuPresenter::FilterLeftoverView(
    /* [in] */ IViewGroup* parent,
    /* [in] */ Int32 childIndex)
{
    assert(parent != NULL);
    AutoPtr<IView> view;
    parent->GetChildAt(childIndex, (IView**)&view);
    if (view != mMoreView) {
        return BaseMenuPresenter::FilterLeftoverView(parent, childIndex);
    }

    return FALSE;
}

Int32 IconMenuPresenter::GetNumActualItemsShown()
{
    assert(IIconMenuView::Probe(mMenuView) != NULL);
    return ((CIconMenuView*) mMenuView.Get())->GetNumActualItemsShown();
}

ECode IconMenuPresenter::SaveHierarchyState(
    /* [in] */ IBundle* outState)
{
    AutoPtr<IObjectInt32Map> viewStates;
    CObjectInt32Map::New((IObjectInt32Map**)&viewStates);

    if (mMenuView != NULL) {
        IView::Probe(mMenuView)->SaveHierarchyState(viewStates);
    }
    AutoPtr<ArrayOf<Int32> > keys;
    AutoPtr<IObjectContainer> values;
    viewStates->GetAllItems((ArrayOf<Int32>**)&keys, (IObjectContainer**)&values);
    outState->PutInt32Array(VIEWS_TAG += "keys", keys);
    outState->PutParcelableArrayList(VIEWS_TAG += "values", values);
    return NOERROR;
}

ECode IconMenuPresenter::RestoreHierarchyState(
    /* [in] */ IBundle* inState)
{
    AutoPtr<ArrayOf<Int32> > keys;
    AutoPtr<IObjectContainer> values;
    inState->GetInt32Array(VIEWS_TAG += "keys", (ArrayOf<Int32>**)&keys);
    inState->GetParcelableArrayList(VIEWS_TAG += "values", (IObjectContainer**)&values);

    AutoPtr<IObjectInt32Map> viewStates;
    CObjectInt32Map::New((IObjectInt32Map**)&viewStates);
    AutoPtr<IObjectEnumerator> em;
    values->GetObjectEnumerator((IObjectEnumerator**)&em);

    Boolean hasNext;
    Int32 i = 0;
    while((em->MoveNext(&hasNext), hasNext) && i < keys->GetLength())
    {
        AutoPtr<IInterface> c;
        em->Current((IInterface**)&c);
        viewStates->Put((*keys)[i++], c);
    }
    if (mMenuView != NULL) {
        IView::Probe(mMenuView)->RestoreHierarchyState(viewStates);
    }

    Int32 subMenuId;
    inState->GetInt32Ex(OPEN_SUBMENU_KEY, 0, &subMenuId);
    if (subMenuId > 0 && mMenu != NULL) {
        AutoPtr<IMenuItem> item;
        mMenu->FindItem(subMenuId, (IMenuItem**)&item);
        if (item != NULL) {
            AutoPtr<ISubMenu> sm;
            item->GetSubMenu((ISubMenu**)&sm);
            Boolean rst;
            OnSubMenuSelected(ISubMenuBuilder::Probe(sm), &rst);
        }
    }
    return NOERROR;
}

ECode IconMenuPresenter::OnSaveInstanceState(
    /* [out] */ IParcelable** parcel)
{
    assert(parcel != NULL);
    if (mMenuView == NULL) {
        *parcel = NULL;
        return NOERROR;
    }

    AutoPtr<IBundle> state;
    CBundle::New((IBundle**)&state);
    SaveHierarchyState(state);
    if (mOpenSubMenuId > 0) {
        state->PutInt32(String(OPEN_SUBMENU_KEY), mOpenSubMenuId);
    }

    *parcel = IParcelable::Probe(state);
    INTERFACE_ADDREF(*parcel);

    return NOERROR;
}

ECode IconMenuPresenter::OnRestoreInstanceState(
    /* [in] */ IParcelable* state)
{
    return RestoreHierarchyState(IBundle::Probe(state));
}

} // namespace Menu
} // namespace View
} // namespace Droid
} // namespace Elastos
