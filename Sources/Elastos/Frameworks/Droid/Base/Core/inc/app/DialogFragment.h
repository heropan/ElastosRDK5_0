
#ifndef __DIALOGFRAGMENT_H__
#define __DIALOGFRAGMENT_H__

#include "app/Fragment.h"

using Elastos::IO::IPrintWriter;
using Elastos::IO::IFileDescriptor;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::App::IDialog;
using Elastos::Droid::App::IActivity;
using Elastos::Droid::App::IDialogFragment;
using Elastos::Droid::App::IFragmentManager;
using Elastos::Droid::App::IFragmentTransaction;
using Elastos::Droid::View::ILayoutInflater;
using Elastos::Droid::Content::IDialogInterface;
using Elastos::Droid::Content::IDialogInterfaceOnCancelListener;
using Elastos::Droid::Content::IDialogInterfaceOnDismissListener;

namespace Elastos {
namespace Droid {
namespace App {

class DialogFragment
    : public Fragment
    , public IDialogFragment
    , public IDialogInterfaceOnCancelListener
    , public IDialogInterfaceOnDismissListener
{
public:
    DialogFragment()
        : mStyle(IDialogFragment::STYLE_NORMAL)
        , mTheme(0)
        , mCancelable(TRUE)
        , mShowsDialog(TRUE)
        , mBackStackId(-1)
        , mDialog(NULL)
        , mViewDestroyed(FALSE)
        , mDismissed(FALSE)
        , mShownByMe(FALSE)
    {}

    virtual ~DialogFragment()
    {}

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    virtual CARAPI SetStyle(
        /* [in] */ Int32 style,
        /* [in] */ Int32 theme);

    virtual CARAPI Show(
        /* [in] */ IFragmentManager* manager,
        /* [in] */ const String& tag);

    virtual CARAPI Show(
        /* [in] */ IFragmentTransaction* transaction,
        /* [in] */ const String& tag,
        /* [out] */ Int32* id);

    virtual CARAPI Dismiss();

    virtual CARAPI DismissAllowingStateLoss();

    virtual CARAPI_(void) DismissInternal(
        /* [in] */ Boolean allowStateLoss);

    virtual CARAPI GetDialog(
        /* [out] */ IDialog** dialog);

    virtual CARAPI GetTheme(
        /* [out] */ Int32* theme);

    virtual CARAPI SetCancelable(
        /* [in] */ Boolean cancelable);

    virtual CARAPI IsCancelable(
        /* [out] */ Boolean* cancelable);

    virtual CARAPI SetShowsDialog(
        /* [in] */ Boolean showsDialog);

    virtual CARAPI GetShowsDialog(
        /* [out] */ Boolean* showsDialog);

    virtual CARAPI OnAttach(
        /* [in] */ IActivity* activity);

    virtual CARAPI OnDetach();

    virtual CARAPI OnCreate(
        /* [in] */ IBundle* savedInstanceState);

    virtual CARAPI GetLayoutInflater(
        /* [in] */ IBundle* savedInstanceState,
        /* [out] */ ILayoutInflater** inflater);

    virtual CARAPI OnCreateDialog(
        /* [in] */ IBundle* savedInstanceState,
        /* [out] */ IDialog** dialog);

    virtual CARAPI OnCancel(
        /* [in] */ IDialogInterface* dialog);

    virtual CARAPI OnDismiss(
        /* [in] */ IDialogInterface* dialog);

    virtual CARAPI OnActivityCreated(
        /* [in] */ IBundle* savedInstanceState);

    virtual CARAPI OnStart();

    virtual CARAPI OnSaveInstanceState(
        /* [in] */ IBundle* outState);

    virtual CARAPI OnStop();

    virtual CARAPI OnDestroyView();

    virtual CARAPI Dump(
        /* [in] */ const String& prefix,
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ IPrintWriter* writer,
        /* [in] */ ArrayOf<String>* args);

public:
    Int32 mStyle;
    Int32 mTheme;
    Boolean mCancelable;
    Boolean mShowsDialog;
    Int32 mBackStackId;

    AutoPtr<IDialog> mDialog;
    Boolean mViewDestroyed;
    Boolean mDismissed;
    Boolean mShownByMe;

private:
    static const char* SAVED_DIALOG_STATE_TAG;
    static const char* SAVED_STYLE;
    static const char* SAVED_THEME;
    static const char* SAVED_CANCELABLE;
    static const char* SAVED_SHOWS_DIALOG;
    static const char* SAVED_BACK_STACK_ID;

};

} // namespace App
} // namespace Droid
} // namespace Elastos

#endif //__DIALOGFRAGMENT_H__
