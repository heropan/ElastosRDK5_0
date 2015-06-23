
#ifndef __CDIALOG_H__
#define __CDIALOG_H__

#include "_CDialog.h"
#include "app/Dialog.h"





namespace Elastos {
namespace Droid {
namespace App {

CarClass(CDialog), public Dialog
{
public:
    IDIALOG_METHODS_DECL();
    IWINDOWCALLBACK_METHODS_DECL();
    IKEYEVENTCALLBACK_METHODS_DECL();

    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ Int32 theme);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ Int32 theme,
        /* [in] */ Boolean createContextThemeWrapper);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ Boolean cancelable,
        /* [in] */ IDialogInterfaceOnCancelListener* cancelListener);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI OnCreateContextMenu(
        /* [in] */ IContextMenu* menu,
        /* [in] */ IView* v,
        /* [in] */ IContextMenuInfo* menuInfo);

private:
    // TODO: Add your private member variables here.
};

} // namespace App
} // namespace Droid
} // namespace Elastos

#endif // __CDIALOG_H__
