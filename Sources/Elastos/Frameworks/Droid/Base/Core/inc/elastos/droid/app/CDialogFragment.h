
#ifndef __ELASTOS_DROID_APP_CDIALOGFRAGMENT_H__
#define __ELASTOS_DROID_APP_CDIALOGFRAGMENT_H__

#include "_Elastos_Droid_App_CDialogFragment.h"
#include "app/DialogFragment.h"


namespace Elastos {
namespace Droid {
namespace App {

class _DialogFragment
    : public DialogFragment
{
public:
    _DialogFragment()
        : DialogFragment()
    {}

    CARAPI OnAttach(
        /* [in] */ IActivity* activity);

    CARAPI OnDetach();

    CARAPI OnCreate(
        /* [in] */ IBundle* savedInstanceState);

    CARAPI GetLayoutInflater(
        /* [in] */ IBundle* savedInstanceState,
        /* [out] */ ILayoutInflater** infalter);

    CARAPI OnActivityCreated(
        /* [in] */ IBundle* savedInstanceState);

    CARAPI OnStart();

    CARAPI OnSaveInstanceState(
        /* [in] */ IBundle* outState);

    CARAPI OnStop();

    CARAPI OnDestroyView();

    CARAPI Dump(
        /* [in] */ const String& prefix,
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ IPrintWriter* writer,
        /* [in] */ ArrayOf<String>* args);
};

CarClass(CDialogFragment) , public _DialogFragment
{
public:
    CDialogFragment()
        : _DialogFragment()
    {}

    IFRAGMENT_METHODS_DECL();

    CARAPI constructor();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI SetStyle(
        /* [in] */ Int32 style,
        /* [in] */ Int32 theme);

    CARAPI Show(
        /* [in] */ IFragmentManager* manager,
        /* [in] */ const String& tag);

    CARAPI Show(
        /* [in] */ IFragmentTransaction* transaction,
        /* [in] */ const String& tag,
        /* [out] */ Int32* id);

    CARAPI Dismiss();

    CARAPI DismissAllowingStateLoss();

    CARAPI GetDialog(
        /* [out] */ IDialog** dialog);

    CARAPI GetTheme(
        /* [out] */ Int32* theme);

    CARAPI SetCancelable(
        /* [in] */ Boolean cancelable);

    CARAPI IsCancelable(
        /* [out] */ Boolean* cancelable);

    CARAPI SetShowsDialog(
        /* [in] */ Boolean showsDialog);

    CARAPI GetShowsDialog(
        /* [out] */ Boolean* showsDialog);

    CARAPI OnCreateDialog(
        /* [in] */ IBundle* savedInstanceState,
        /* [out] */ IDialog** dialog);

    CARAPI OnCancel(
        /* [in] */ IDialogInterface* dialog);

    CARAPI OnDismiss(
        /* [in] */ IDialogInterface* dialog);
};

} // namespace App
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_APP_CDIALOGFRAGMENT_H__
