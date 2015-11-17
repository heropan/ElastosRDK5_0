
#ifndef __ELASTOS_DROID_INTERNAL_VIEW_MENU_MENUDIALOGHELPER_H__
#define __ELASTOS_DROID_INTERNAL_VIEW_MENU_MENUDIALOGHELPER_H__

#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/internal/view/menu/ListMenuPresenter.h"

using Elastos::Droid::Content::IDialogInterfaceOnClickListener;
using Elastos::Droid::Content::IDialogInterfaceOnKeyListener;
using Elastos::Droid::Content::IDialogInterfaceOnDismissListener;
using Elastos::Droid::Os::IBinder;
using Elastos::Droid::View::Menu::IMenuPresenterCallback;
using Elastos::Droid::View::Menu::ListMenuPresenter;
using Elastos::Droid::Content::IDialogInterface;
using Elastos::Droid::App::IAlertDialog;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace View {
namespace Menu {

/**
 * Helper for menus that appear as Dialogs (context and submenus).
 *
 * @hide
 */
class MenuDialogHelper
{
public:
    MenuDialogHelper();

    MenuDialogHelper(
        /* [in] */ IMenuBuilder* menu);

    virtual CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid) = 0;
    /**
     * Shows menu as a dialog.
     *
     * @param windowToken Optional token to assign to the window.
     */
    CARAPI Show(
        /* [in] */ IBinder* windowToken);

    CARAPI OnKey(
        /* [in] */ IDialogInterface* dialog,
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* flag);

    CARAPI SetPresenterCallback(
        /* [in] */ IMenuPresenterCallback* cb);

    /**
     * Dismisses the menu's dialog.
     *
     * @see Dialog#dismiss()
     */
    CARAPI Dismiss();

    CARAPI OnDismiss(
        /* [in] */ IDialogInterface* dialog);

    CARAPI OnCloseMenu(
        /* [in] */ IMenuBuilder* menu,
        /* [in] */ Boolean allMenusAreClosing);

    CARAPI OnOpenSubMenu(
        /* [in] */ IMenuBuilder* subMenu,
        /* [out] */ Boolean* result);

    CARAPI OnClick(
        /* [in] */ IDialogInterface* dialog,
        /* [in] */ int which);

protected:
    CARAPI Init(
        /* [in] */ IMenuBuilder* menu);

private:
    AutoPtr<IMenuBuilder> mMenu;
    AutoPtr<IAlertDialog> mDialog;
    AutoPtr<IListMenuPresenter> mPresenter;
    AutoPtr<IMenuPresenterCallback> mPresenterCallback;
};

} // namespace Menu
} // namespace View
} // namespace Internal
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_INTERNAL_VIEW_MENU_MENUDIALOGHELPER_H__
