#include "widget/CPopupMenu.h"

namespace Elastos{
namespace Droid{
namespace Widget{

PInterface CPopupMenu::Probe(
    /* [in] */ REIID riid)
{
    return _CPopupMenu::Probe(riid);
}

ECode CPopupMenu::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IView* anchor)
{
    return PopupMenu::Init(context, anchor);
}

ECode CPopupMenu::GetMenu(
    /* [out] */ IMenu** menu)
{
    VALIDATE_NOT_NULL(menu);
    AutoPtr<IMenu> m = PopupMenu::GetMenu();
    *menu = m;
    REFCOUNT_ADD(*menu);
    return NOERROR;
}

ECode CPopupMenu::GetMenuInflater(
    /* [out] */ IMenuInflater** flater)
{
    VALIDATE_NOT_NULL(flater);
    AutoPtr<IMenuInflater> mf = PopupMenu::GetMenuInflater();
    *flater = mf;
    REFCOUNT_ADD(*flater);
    return NOERROR;
}

ECode CPopupMenu::Inflate(
    /* [in] */ Int32 menuRes)
{
    return PopupMenu::Inflate(menuRes);
}

ECode CPopupMenu::Show()
{
    return PopupMenu::Show();
}

ECode CPopupMenu::Dismiss()
{
    return PopupMenu::Dismiss();
}

ECode CPopupMenu::SetOnMenuItemClickListener(
    /* [in] */ IPopupMenuOnMenuItemClickListener* listener)
{
    return PopupMenu::SetOnMenuItemClickListener(listener);
}

ECode CPopupMenu::SetOnDismissListener(
    /* [in] */ IPopupMenuOnDismissListener* listener)
{
    return PopupMenu::SetOnDismissListener(listener);
}

ECode CPopupMenu::OnMenuItemSelected(
    /* [in] */ IMenuBuilder* menu,
    /* [in] */ IMenuItem* item,
    /* [out] */ Boolean* rst)
{
    *rst = PopupMenu::OnMenuItemSelected(menu, item);
    return NOERROR;
}
ECode CPopupMenu::OnMenuModeChange(
    /* [in] */ IMenuBuilder* menu)
{
    return PopupMenu::OnMenuModeChange(menu);
}

ECode CPopupMenu::OnCloseMenu(
    /* [in] */ IMenuBuilder* menu,
    /* [in] */ Boolean allMenusAreClosing)
{
    return PopupMenu::OnCloseMenu(menu, allMenusAreClosing);
}

ECode CPopupMenu::OnOpenSubMenu(
    /* [in] */ IMenuBuilder* subMenu,
    /* [out] */ Boolean* rst)
{
    *rst = PopupMenu::OnOpenSubMenu(subMenu);
    return NOERROR;
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos