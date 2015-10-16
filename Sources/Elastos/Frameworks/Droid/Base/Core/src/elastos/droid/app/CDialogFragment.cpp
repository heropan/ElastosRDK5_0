
#include "elastos/droid/app/CDialogFragment.h"

namespace Elastos {
namespace Droid {
namespace App {

ECode _DialogFragment::OnAttach(
    /* [in] */ IActivity* activity)
{
    return DialogFragment::OnAttach(activity);
}

ECode _DialogFragment::OnDetach()
{
    return DialogFragment::OnDetach();
}

ECode _DialogFragment::OnCreate(
    /* [in] */ IBundle* savedInstanceState)
{
    return DialogFragment::OnCreate(savedInstanceState);
}

ECode _DialogFragment::GetLayoutInflater(
    /* [in] */ IBundle* savedInstanceState,
    /* [out] */ ILayoutInflater** infalter)
{
    return DialogFragment::GetLayoutInflater(savedInstanceState, infalter);
}

ECode _DialogFragment::OnActivityCreated(
    /* [in] */ IBundle* savedInstanceState)
{
    return DialogFragment::OnActivityCreated(savedInstanceState);
}

ECode _DialogFragment::OnStart()
{
    return DialogFragment::OnStart();
}

ECode _DialogFragment::OnSaveInstanceState(
    /* [in] */ IBundle* outState)
{
    return DialogFragment::OnSaveInstanceState(outState);
}

ECode _DialogFragment::OnStop()
{
    return DialogFragment::OnStop();
}

ECode _DialogFragment::OnDestroyView()
{
    return DialogFragment::OnDestroyView();
}

ECode _DialogFragment::Dump(
    /* [in] */ const String& prefix,
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ IPrintWriter* writer,
    /* [in] */ ArrayOf<String>* args)
{
    return DialogFragment::Dump(prefix, fd, writer, args);
}

IFRAGMENT_METHODS_IMPL(CDialogFragment, _DialogFragment);

ECode CDialogFragment::constructor()
{
    return NOERROR;
}

PInterface CDialogFragment::Probe(
    /* [in] */ REIID riid)
{
    return _CDialogFragment::Probe(riid);
}

ECode CDialogFragment::SetStyle(
    /* [in] */ Int32 style,
    /* [in] */ Int32 theme)
{
    return _DialogFragment::SetStyle(style, theme);
}

ECode CDialogFragment::Show(
    /* [in] */ IFragmentManager* manager,
    /* [in] */ const String& tag)
{
    return _DialogFragment::Show(manager, tag);
}

ECode CDialogFragment::Show(
    /* [in] */ IFragmentTransaction* transaction,
    /* [in] */ const String& tag,
    /* [out] */ Int32* id)
{
    return _DialogFragment::Show(transaction, tag, id);
}

ECode CDialogFragment::Dismiss()
{
    return _DialogFragment::Dismiss();
}

ECode CDialogFragment::DismissAllowingStateLoss()
{
    return _DialogFragment::DismissAllowingStateLoss();
}

ECode CDialogFragment::GetDialog(
    /* [out] */ IDialog** dialog)
{
    return _DialogFragment::GetDialog(dialog);
}

ECode CDialogFragment::GetTheme(
    /* [out] */ Int32* theme)
{
    return _DialogFragment::GetTheme(theme);
}

ECode CDialogFragment::SetCancelable(
    /* [in] */ Boolean cancelable)
{
    return _DialogFragment::SetCancelable(cancelable);
}

ECode CDialogFragment::IsCancelable(
    /* [out] */ Boolean* cancelable)
{
    return _DialogFragment::IsCancelable(cancelable);
}

ECode CDialogFragment::SetShowsDialog(
    /* [in] */ Boolean showsDialog)
{
    return _DialogFragment::SetShowsDialog(showsDialog);
}

ECode CDialogFragment::GetShowsDialog(
    /* [out] */ Boolean* showsDialog)
{
    return _DialogFragment::GetShowsDialog(showsDialog);
}

ECode CDialogFragment::OnCreateDialog(
    /* [in] */ IBundle* savedInstanceState,
    /* [out] */ IDialog** dialog)
{
    return _DialogFragment::OnCreateDialog(savedInstanceState, dialog);
}

ECode CDialogFragment::OnCancel(
    /* [in] */ IDialogInterface* dialog)
{
    return _DialogFragment::OnCancel(dialog);
}

ECode CDialogFragment::OnDismiss(
    /* [in] */ IDialogInterface* dialog)
{
    return _DialogFragment::OnDismiss(dialog);
}


} // namespace App
} // namespace Droid
} // namespace Elastos