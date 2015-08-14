#ifndef __ELASTOS_DROID_APP_CPROGRESSDIALOG_H__
#define __ELASTOS_DROID_APP_CPROGRESSDIALOG_H__

#include "_CProgressDialog.h"




namespace Elastos{
namespace Droid{
namespace App{

CarClass(CProgressDialog), public ProgressDialog
{
public:
    IPROGRESSDIALOG_METHODS_DECL();
    IALERTDIALOG_METHODS_DECL();
    IDIALOG_METHODS_DECL();
    IWINDOWCALLBACK_METHODS_DECL();
    IKEYEVENTCALLBACK_METHODS_DECL();

    CProgressDialog();

    ~CProgressDialog();

    CARAPI constructor(
        /* [in] */ IContext * context);

    CARAPI constructor(
        /* [in] */ IContext * context,
        /* [in] */ Int32 theme);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI OnCreateContextMenu(
        /* [in] */ IContextMenu* menu,
        /* [in] */ IView* v,
        /* [in] */ IContextMenuInfo* menuInfo);
};

} // namespace App
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_APP_CPROGRESSDIALOG_H__
