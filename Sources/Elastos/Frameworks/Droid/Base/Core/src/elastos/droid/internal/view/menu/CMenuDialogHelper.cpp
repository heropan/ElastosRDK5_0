#include "elastos/droid/internal/view/menu/CMenuDialogHelper.h"

namespace Elastos {
namespace Droid {
namespace Internal {
namespace View {
namespace Menu {

CMenuDialogHelper::CMenuDialogHelper()
{}

ECode CMenuDialogHelper::constructor(
    /* [in] */ IMenuBuilder* menu)
{
    return MenuDialogHelper::Init(menu);
}

PInterface CMenuDialogHelper::Probe(
        /* [in]  */ REIID riid)
{
    return _CMenuDialogHelper::Probe(riid);
}

ECode CMenuDialogHelper::Show(
        /* [in] */ IBinder* windowToken)
{
    return MenuDialogHelper::Show(windowToken);
}

ECode CMenuDialogHelper::SetPresenterCallback(
        /* [in] */ IMenuPresenterCallback* callback)
{
    return MenuDialogHelper::SetPresenterCallback(callback);
}

ECode CMenuDialogHelper::Dismiss()
{
    return MenuDialogHelper::Dismiss();
}

ECode CMenuDialogHelper::OnKey(
    /* [in] */ IDialogInterface* dialog,
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* consumed)
{
    return MenuDialogHelper::OnKey(dialog, keyCode, event, consumed);
}

ECode CMenuDialogHelper::OnClick(
    /* [in] */ IDialogInterface* dialog,
    /* [in] */ Int32 which)
{
    return MenuDialogHelper::OnClick(dialog, which);
}

ECode CMenuDialogHelper::OnDismiss(
    /* [in] */ IDialogInterface* dialog)
{
    return MenuDialogHelper::OnDismiss(dialog);
}

ECode CMenuDialogHelper::OnCloseMenu(
    /* [in] */ IMenuBuilder* menu,
    /* [in] */ Boolean allMenusAreClosing)
{
    return MenuDialogHelper::OnCloseMenu(menu, allMenusAreClosing);
}

ECode CMenuDialogHelper::OnOpenSubMenu(
    /* [in] */ IMenuBuilder* subMenu,
    /* [out] */ Boolean* handle)
{
    return MenuDialogHelper::OnOpenSubMenu(subMenu, handle);
}

} // namespace Menu
} // namespace View
} // namepsace Droid
} // namespace Elastos