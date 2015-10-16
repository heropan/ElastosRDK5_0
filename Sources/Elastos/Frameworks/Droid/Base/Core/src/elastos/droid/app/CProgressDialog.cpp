
#include "elastos/droid/app/CProgressDialog.h"

namespace Elastos{
namespace Droid{
namespace App{

IPROGRESSDIALOG_METHODS_IMPL(CProgressDialog, ProgressDialog);
IALERTDIALOG_METHODS_IMPL(CProgressDialog, ProgressDialog);
IDIALOG_METHODS_IMPL(CProgressDialog, ProgressDialog);
IWINDOWCALLBACK_METHODS_IMPL(CProgressDialog, ProgressDialog);
IKEYEVENTCALLBACK_METHODS_IMPL(CProgressDialog, ProgressDialog);

CProgressDialog::CProgressDialog()
{}

CProgressDialog::~CProgressDialog()
{}

ECode CProgressDialog::constructor(
    /* [in] */ IContext * context)
{
    return ProgressDialog::Init(context);
}

ECode CProgressDialog::constructor(
    /* [in] */ IContext * context,
    /* [in] */ Int32 theme)
{
    return ProgressDialog::Init(context, theme);
}

PInterface CProgressDialog::Probe(
    /* [in] */ REIID riid)
{
    return _CProgressDialog::Probe(riid);
}

ECode CProgressDialog::OnCreateContextMenu(
    /* [in] */ IContextMenu* menu,
    /* [in] */ IView* v,
    /* [in] */ IContextMenuInfo* menuInfo)
{
    return ProgressDialog::OnCreateContextMenu(menu, v, menuInfo);
}

} // namespace App
} // namespace Droid
} // namespace Elastos
