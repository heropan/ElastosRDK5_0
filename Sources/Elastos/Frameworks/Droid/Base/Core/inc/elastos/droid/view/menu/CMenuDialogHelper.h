#ifndef __ELASTOS_DROID_VIEW_MENU_CMENUDIALOGHELPER_H__
#define __ELASTOS_DROID_VIEW_MENU_CMENUDIALOGHELPER_H__

#include "_Elastos_Droid_View_Menu_CMenuDialogHelper.h"
#include "elastos/droid/view/menu/MenuDialogHelper.h"

using Elastos::Droid::Widget::IAdapterView;
using Elastos::Droid::Os::IBinder;
using Elastos::Droid::Content::IDialogInterface;
using Elastos::Droid::Content::IContext;

namespace Elastos {
namespace Droid {
namespace View {
namespace Menu {

CarClass(CMenuDialogHelper) , public MenuDialogHelper
{
public:
    CMenuDialogHelper();

    CARAPI constructor(
        /* [in] */ IMenuBuilder* menu);

    virtual CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

    CARAPI Show(
        /* [in] */ IBinder* windowToken);

    CARAPI SetPresenterCallback(
        /* [in] */ IMenuPresenterCallback* callback);

    CARAPI Dismiss();

    CARAPI OnKey(
        /* [in] */ IDialogInterface* dialog,
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* consumed);

    CARAPI OnClick(
        /* [in] */ IDialogInterface* dialog,
        /* [in] */ Int32 which);

    CARAPI OnDismiss(
        /* [in] */ IDialogInterface* dialog);

    CARAPI OnCloseMenu(
        /* [in] */ IMenuBuilder* menu,
        /* [in] */ Boolean allMenusAreClosing);

    CARAPI OnOpenSubMenu(
        /* [in] */ IMenuBuilder* subMenu,
        /* [out] */ Boolean* handle);

};
} // namespace Menu
} // namespace View
} // namepsace Droid
} // namespace Elastos

#endif
