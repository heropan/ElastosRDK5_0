
#ifndef __CALERTDIALOG_H__
#define __CALERTDIALOG_H__

#include "_CAlertDialog.h"




using Elastos::Droid::Os::IMessage;

namespace Elastos {
namespace Droid {
namespace App {

CarClass(CAlertDialog), public AlertDialog
{
public:
    IALERTDIALOG_METHODS_DECL();
    IDIALOG_METHODS_DECL();
    IWINDOWCALLBACK_METHODS_DECL();
    IKEYEVENTCALLBACK_METHODS_DECL();

    CARAPI constructor(
        /* [in] */ IContext* context);

    /**
     * Construct an AlertDialog that uses an explicit theme.  The actual style
     * that an AlertDialog uses is a private implementation, however you can
     * here supply either the name of an attribute in the theme from which
     * to get the dialog's style (such as {@link android.R.attr#alertDialogTheme}
     * or one of the constants {@link #THEME_TRADITIONAL},
     * {@link #THEME_HOLO_DARK}, or {@link #THEME_HOLO_LIGHT}.
     */
    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ Int32 theme);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ Int32 theme,
        /* [in] */ Boolean createThemeContextWrapper);

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
};

}
}
}

#endif // __CALERTDIALOG_H__
