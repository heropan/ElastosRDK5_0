
#include "elastos/droid/widget/PopupMenu.h"
#include "elastos/droid/view/menu/CMenuBuilder.h"
#include "elastos/droid/view/CMenuInflater.h"

using Elastos::Droid::View::CMenuInflater;
using Elastos::Droid::View::Menu::CMenuBuilder;
using Elastos::Droid::View::Menu::IMenuPresenter;
using Elastos::Droid::View::Menu::EIID_IMenuBuilderCallback;
using Elastos::Droid::View::Menu::EIID_IMenuPresenterCallback;
using Elastos::Droid::Widget::EIID_IPopupMenu;

namespace Elastos {
namespace Droid {
namespace Widget {

CAR_INTERFACE_IMPL_2(PopupMenu::MyCallback, IMenuBuilderCallback, IMenuPresenterCallback)

ECode PopupMenu::MyCallback::OnMenuItemSelected(
    /* [in] */ IMenuBuilder* menu,
    /* [in] */ IMenuItem* item,
    /* [out] */ Boolean* state)
{
    VALIDATE_NOT_NULL(state);
    *state = mOwner->OnMenuItemSelected(menu, item);
    return NOERROR;
}

ECode PopupMenu::MyCallback::OnMenuModeChange(
    /* [in] */ IMenuBuilder* menu)
{
    return mOwner->OnMenuModeChange(menu);
}

ECode PopupMenu::MyCallback::OnOpenSubMenu(
    /* [in] */ IMenuBuilder* subMenu,
    /* [out] */ Boolean* handle)
{
    VALIDATE_NOT_NULL(handle);
    *handle = mOwner->OnOpenSubMenu(subMenu);
    return NOERROR;
}

ECode PopupMenu::MyCallback::OnCloseMenu(
    /* [in] */ IMenuBuilder* menu,
    /* [in] */ Boolean allMenusAreClosing)
{
    return mOwner->OnCloseMenu(menu, allMenusAreClosing);
}

PopupMenu::PopupMenu()
{}

PopupMenu::PopupMenu(
    /* [in] */ IContext* context,
    /* [in] */ IView* anchor)
{
    Init(context, anchor);
}

ECode PopupMenu::Init(
    /* [in] */ IContext* context,
    /* [in] */ IView* anchor)
{
    mContext = context;
    CMenuBuilder::New(context, (IMenuBuilder**)&mMenu);
    AutoPtr<MyCallback> callback = new MyCallback(this);
    mMenu->SetCallback(callback);
    mAnchor = anchor;
    CMenuPopupHelper::New(context, mMenu, anchor, (IMenuPopupHelper**)&mPopup);
    IMenuPresenter::Probe(mPopup)->SetCallback(callback);
    return NOERROR;
}

AutoPtr<IMenu> PopupMenu::GetMenu()
{
    return mMenu;
}

AutoPtr<IMenuInflater> PopupMenu::GetMenuInflater()
{
    AutoPtr<IMenuInflater> inflate;
    CMenuInflater::New(mContext, (IMenuInflater**)&inflate);
    return inflate;
}

ECode PopupMenu::Inflate(
    /* [in] */ Int32 menuRes)
{
    AutoPtr<IMenu> menu = IMenu::Probe(mMenu);
    GetMenuInflater()->Inflate(menuRes, menu);
    return NOERROR;
}

ECode PopupMenu::Show()
{
    ASSERT_SUCCEEDED(mPopup->Show());
    return NOERROR;
}

ECode PopupMenu::Dismiss()
{
    mPopup->Dismiss();
    return NOERROR;
}

ECode PopupMenu::SetOnMenuItemClickListener(
    /* [in] */ IPopupMenuOnMenuItemClickListener* listener)
{
    mMenuItemClickListener = listener;
    return NOERROR;
}

ECode PopupMenu::SetOnDismissListener(
    /* [in] */ IPopupMenuOnDismissListener* listener)
{
    mDismissListener = listener;
    return NOERROR;
}

Boolean PopupMenu::OnMenuItemSelected(
    /* [in] */ IMenuBuilder* menu,
    /* [in] */ IMenuItem* item)
{
    if (mMenuItemClickListener != NULL) {
        Boolean result;
        mMenuItemClickListener->OnMenuItemClick(item, &result);
        return result;
    }
    return FALSE;
}

ECode PopupMenu::OnCloseMenu(
    /* [in] */ IMenuBuilder* menu,
    /* [in] */ Boolean allMenusAreClosing)
{
    if (mDismissListener != NULL) {
        mDismissListener->OnDismiss((IPopupMenu*)this->Probe(EIID_IPopupMenu));
    }
    return NOERROR;
}

Boolean PopupMenu::OnOpenSubMenu(
    /* [in] */ IMenuBuilder* subMenu)
{
    if (subMenu == NULL) return FALSE;
    Boolean hasVisible = FALSE;
    subMenu->HasVisibleItems(&hasVisible);
    if (!hasVisible) {
        return TRUE;
    }

    AutoPtr<IMenuPopupHelper> helper;
    CMenuPopupHelper::New(mContext, subMenu, mAnchor, (IMenuPopupHelper**)&helper);
    ASSERT_SUCCEEDED(helper->Show());
    return TRUE;
}

ECode PopupMenu::OnCloseSubMenu(
    /* [in] */ ISubMenuBuilder* menu)
{
    return NOERROR;
}

ECode PopupMenu::OnMenuModeChange(
    /* [in] */ IMenuBuilder* menu)
{
    return NOERROR;
}

} // namespace Widget
} // namespace Droid
} // namespace Elastos

