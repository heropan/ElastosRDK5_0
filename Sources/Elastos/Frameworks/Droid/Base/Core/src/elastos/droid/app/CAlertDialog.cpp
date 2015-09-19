
#include "app/CAlertDialog.h"

namespace Elastos {
namespace Droid {
namespace App {

IDIALOG_METHODS_IMPL(CAlertDialog, AlertDialog);
IWINDOWCALLBACK_METHODS_IMPL(CAlertDialog, AlertDialog);
IKEYEVENTCALLBACK_METHODS_IMPL(CAlertDialog, AlertDialog);
IALERTDIALOG_METHODS_IMPL(CAlertDialog, AlertDialog);

ECode CAlertDialog::constructor(
    /* [in] */ IContext* context)
{
    return AlertDialog::Init(context, ResolveDialogTheme(context, 0), TRUE);
}

ECode CAlertDialog::constructor(
    /* [in] */ IContext* context,
    /* [in] */ Int32 theme)
{
    return AlertDialog::Init(context, theme);
}

ECode CAlertDialog::constructor(
    /* [in] */ IContext* context,
    /* [in] */ Int32 theme,
    /* [in] */ Boolean createThemeContextWrapper)
{
    return AlertDialog::Init(context, theme, createThemeContextWrapper);
}

ECode CAlertDialog::constructor(
    /* [in] */ IContext* context,
    /* [in] */ Boolean cancelable,
    /* [in] */ IDialogInterfaceOnCancelListener* cancelListener)
{
    return AlertDialog::Init(context, cancelable, cancelListener);
}

PInterface CAlertDialog::Probe(
    /* [in] */ REIID riid)
{
    return _CAlertDialog::Probe(riid);
}

ECode CAlertDialog::OnCreateContextMenu(
    /* [in] */ IContextMenu* menu,
    /* [in] */ IView* v,
    /* [in] */ IContextMenuInfo* menuInfo)
{
    return AlertDialog::OnCreateContextMenu(menu, v, menuInfo);
}

} // namespace App
} // namespace Droid
} // namespace Elastos
