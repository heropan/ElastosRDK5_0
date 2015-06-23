
#include "app/DialogFragment.h"

using Elastos::Droid::View::IWindow;
using Elastos::Droid::View::IViewParent;
using Elastos::Droid::View::IWindowManagerLayoutParams;
using Elastos::Droid::Content::IDialogInterfaceOnCancelListener;
using Elastos::Droid::Content::IDialogInterfaceOnDismissListener;
using Elastos::Droid::Content::EIID_IDialogInterfaceOnCancelListener;
using Elastos::Droid::Content::EIID_IDialogInterfaceOnDismissListener;

namespace Elastos {
namespace Droid {
namespace App {

const char* DialogFragment::SAVED_DIALOG_STATE_TAG = "android:savedDialogState";
const char* DialogFragment::SAVED_STYLE = "android:style";
const char* DialogFragment::SAVED_THEME = "android:theme";
const char* DialogFragment::SAVED_CANCELABLE = "android:cancelable";
const char* DialogFragment::SAVED_SHOWS_DIALOG = "android:showsDialog";
const char* DialogFragment::SAVED_BACK_STACK_ID = "android:backStackId";

PInterface DialogFragment::Probe(
        /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IDialogFragment*)this;
    }
    else if (riid == EIID_IObject) {
        return (IObject*)this;
    }
    else if (riid == EIID_IFragment) {
        return (IFragment*)this;
    }
    else if (riid == EIID_IDialogFragment) {
        return (IDialogFragment*)this;
    }
    else if (riid == EIID_IDialogInterfaceOnCancelListener) {
        return (IDialogInterfaceOnCancelListener*)this;
    }
    else if (riid == EIID_IDialogInterfaceOnDismissListener) {
        return (IDialogInterfaceOnDismissListener*)this;
    }
    return NULL;
}

ECode DialogFragment::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (NULL == pIID) return E_INVALID_ARGUMENT;

    if (pObject == (IInterface *)(IFragment *)this) {
        *pIID = EIID_IFragment;
        return NOERROR;
    }
    else if (pObject == (IInterface *)(IObject *)this) {
        *pIID = EIID_IObject;
        return NOERROR;
    }
    else if (pObject == (IInterface *)(IComponentCallbacks2 *)this) {
        *pIID = EIID_IComponentCallbacks2;
        return NOERROR;
    }
    else if (pObject == (IInterface *)(IViewOnCreateContextMenuListener *)this) {
        *pIID = EIID_IViewOnCreateContextMenuListener;
        return NOERROR;
    }

    return E_INVALID_ARGUMENT;
}

ECode DialogFragment::SetStyle(
    /* [in] */ Int32 style,
    /* [in] */ Int32 theme)
{
    mStyle = style;
    if (mStyle == IDialogFragment::STYLE_NO_FRAME || mStyle == IDialogFragment::STYLE_NO_INPUT) {
        mTheme = 0/*com.android.internal.R.style.Theme_DeviceDefault_Dialog_NoFrame*/;
    }
    if (theme != 0) {
        mTheme = theme;
    }
    return NOERROR;
}

ECode DialogFragment::Show(
    /* [in] */ IFragmentManager* manager,
    /* [in] */ const String& tag)
{
    mDismissed = FALSE;
    mShownByMe = TRUE;
    AutoPtr<IFragmentTransaction> ft;
    manager->BeginTransaction((IFragmentTransaction**)&ft);
    ft->Add((IFragment*)this->Probe(EIID_IFragment), tag);
    Int32 id;
    ft->Commit(&id);
    return NOERROR;
}

ECode DialogFragment::Show(
    /* [in] */ IFragmentTransaction* transaction,
    /* [in] */ const String& tag,
    /* [out] */ Int32* id)
{
    mDismissed = FALSE;
    mShownByMe = TRUE;
    transaction->Add((IFragment*)this->Probe(EIID_IFragment), tag);
    mViewDestroyed = FALSE;
    transaction->Commit(&mBackStackId);
    *id = mBackStackId;
    return NOERROR;
}

ECode DialogFragment::Dismiss()
{
    DismissInternal(FALSE);
    return NOERROR;
}

ECode DialogFragment::DismissAllowingStateLoss()
{
    DismissInternal(TRUE);
    return NOERROR;
}

void DialogFragment::DismissInternal(
    /* [in] */ Boolean allowStateLoss)
{
    if (mDismissed) {
        return;
    }
    mDismissed = TRUE;
    mShownByMe = FALSE;
    if (mDialog != NULL) {
        mDialog->Dismiss();
        mDialog = NULL;
    }
    mViewDestroyed = TRUE;
    if (mBackStackId >= 0) {
        AutoPtr<IFragmentManager> frMgr;
        GetFragmentManager((IFragmentManager**)&frMgr);
        frMgr->PopBackStack(mBackStackId,
                IFragmentManager::POP_BACK_STACK_INCLUSIVE);
        mBackStackId = -1;
    } else {
        AutoPtr<IFragmentTransaction> ft;
        AutoPtr<IFragmentManager> frMgr;
        GetFragmentManager((IFragmentManager**)&frMgr);
        frMgr->BeginTransaction((IFragmentTransaction**)&ft);
        ft->Remove((IFragment*)this->Probe(EIID_IFragment));
        if (allowStateLoss) {
            Int32 result;
            ft->CommitAllowingStateLoss(&result);
        } else {
            Int32 id;
            ft->Commit(&id);
        }
    }
}

ECode DialogFragment::GetDialog(
    /* [out] */ IDialog** dialog)
{
    VALIDATE_NOT_NULL(dialog);
    *dialog = mDialog;
    REFCOUNT_ADD(*dialog);
    return NOERROR;
}

ECode DialogFragment::GetTheme(
    /* [out] */ Int32* theme)
{
    *theme = mTheme;
    return NOERROR;
}

ECode DialogFragment::SetCancelable(
    /* [in] */ Boolean cancelable)
{
    mCancelable = cancelable;
    if (mDialog != NULL) mDialog->SetCancelable(cancelable);
    return NOERROR;
}

ECode DialogFragment::IsCancelable(
    /* [out] */ Boolean* cancelable)
{
    VALIDATE_NOT_NULL(cancelable);
    *cancelable = mCancelable;
    return NOERROR;
}

ECode DialogFragment::SetShowsDialog(
    /* [in] */ Boolean showsDialog)
{
    mShowsDialog = showsDialog;
    return NOERROR;
}

ECode DialogFragment::GetShowsDialog(
    /* [out] */ Boolean* showsDialog)
{
    VALIDATE_NOT_NULL(showsDialog);
    *showsDialog = mShowsDialog;
    return NOERROR;
}

ECode DialogFragment::OnAttach(
    /* [in] */ IActivity* activity)
{
    Fragment::OnAttach(activity);
    if (!mShownByMe) {
        // If not explicitly shown through our API, take this as an
        // indication that the dialog is no longer dismissed.
        mDismissed = FALSE;
    }
    return NOERROR;
}

ECode DialogFragment::OnDetach()
{
    Fragment::OnDetach();
    if (!mShownByMe && !mDismissed) {
        // The fragment was not shown by a direct call here, it is not
        // dismissed, and now it is being detached...  well, okay, thou
        // art now dismissed.  Have fun.
        mDismissed = TRUE;
    }
    return NOERROR;
}

ECode DialogFragment::OnCreate(
    /* [in] */ IBundle* savedInstanceState)
{
    Fragment::OnCreate(savedInstanceState);

    mShowsDialog = mContainerId == 0;

    if (savedInstanceState != NULL) {
        savedInstanceState->GetInt32(String(SAVED_STYLE), IDialogFragment::STYLE_NORMAL, &mStyle);
        savedInstanceState->GetInt32(String(SAVED_THEME), 0, &mTheme);
        savedInstanceState->GetBoolean(String(SAVED_CANCELABLE), TRUE, &mCancelable);
        savedInstanceState->GetBoolean(String(SAVED_SHOWS_DIALOG), mShowsDialog, &mShowsDialog);
        savedInstanceState->GetInt32(String(SAVED_BACK_STACK_ID), -1, &mBackStackId);
    }
    return NOERROR;
}

ECode DialogFragment::GetLayoutInflater(
    /* [in] */ IBundle* savedInstanceState,
    /* [out] */ ILayoutInflater** inflater)
{
    VALIDATE_NOT_NULL(inflater)
    if (!mShowsDialog) {
        return Fragment::GetLayoutInflater(savedInstanceState, inflater);
    }

    mDialog = NULL;
    OnCreateDialog(savedInstanceState, (IDialog**)&mDialog);
    switch (mStyle) {
        case IDialogFragment::STYLE_NO_INPUT:
            {
                AutoPtr<IWindow> window;
                mDialog->GetWindow((IWindow**)&window);
                window->AddFlags(
                        IWindowManagerLayoutParams::FLAG_NOT_FOCUSABLE |
                        IWindowManagerLayoutParams::FLAG_NOT_TOUCHABLE);
                // fall through...
            }
        case IDialogFragment::STYLE_NO_FRAME:
        case IDialogFragment::STYLE_NO_TITLE:
            {
                Boolean support;
                mDialog->RequestWindowFeature(IWindow::FEATURE_NO_TITLE, &support);
            }
    }
    if (mDialog != NULL) {
        AutoPtr<IContext> ctx;
        mDialog->GetContext((IContext**)&ctx);
        ctx->GetSystemService(
                IContext::LAYOUT_INFLATER_SERVICE, (IInterface**)inflater);
        return NOERROR;
    }
    return mActivity->GetSystemService(
            IContext::LAYOUT_INFLATER_SERVICE, (IInterface**)inflater);
}

ECode DialogFragment::OnCreateDialog(
    /* [in] */ IBundle* savedInstanceState,
    /* [out] */ IDialog** dialog)
{
    AutoPtr<IActivity> activity;
    GetActivity((IActivity**)&activity);
    Int32 theme;
    GetTheme(&theme);
//    CDialog::New((IContext*)activity.Get(), theme, dialog);
    return NOERROR;
}

ECode DialogFragment::OnCancel(
    /* [in] */ IDialogInterface* dialog)
{
    return NOERROR;
}

ECode DialogFragment::OnDismiss(
    /* [in] */ IDialogInterface* dialog)
{
    if (!mViewDestroyed) {
        // Note: we need to use allowStateLoss, because the dialog
        // dispatches this asynchronously so we can receive the call
        // after the activity is paused.  Worst case, when the user comes
        // back to the activity they see the dialog again.
        DismissInternal(TRUE);
    }
    return NOERROR;
}

ECode DialogFragment::OnActivityCreated(
    /* [in] */ IBundle* savedInstanceState)
{
    Fragment::OnActivityCreated(savedInstanceState);

    if (!mShowsDialog) {
        return NOERROR;
    }

    AutoPtr<IView> view;
    GetView((IView**)&view);
    if (view != NULL) {
        AutoPtr<IViewParent> parent;
        view->GetParent((IViewParent**)&parent);
        if (parent != NULL) {
//             throw new IllegalStateException("DialogFragment can not be attached to a container view");
            return E_ILLEGAL_STATE_EXCEPTION;
        }
        mDialog->SetContentView(view);
    }
    AutoPtr<IActivity> activity;
    GetActivity((IActivity**)&activity);
    mDialog->SetOwnerActivity(activity);
    mDialog->SetCancelable(mCancelable);
    Boolean result;
    mDialog->TakeCancelAndDismissListeners(String("DialogFragment"),
        (IDialogInterfaceOnCancelListener*)this->Probe(EIID_IDialogInterfaceOnCancelListener),
        (IDialogInterfaceOnDismissListener*)this->Probe(EIID_IDialogInterfaceOnDismissListener),
        &result);
    if (!result) {
//         throw new IllegalStateException(
//                 "You can not set Dialog's OnCancelListener or OnDismissListener");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    if (savedInstanceState != NULL) {
        AutoPtr<IBundle> dialogState;
        savedInstanceState->GetBundle(String(SAVED_DIALOG_STATE_TAG), (IBundle**)&dialogState);
        if (dialogState != NULL) {
            mDialog->OnRestoreInstanceState(dialogState);
        }
    }
    return NOERROR;
}

ECode DialogFragment::OnStart()
{
    Fragment::OnStart();
    if (mDialog != NULL) {
        mViewDestroyed = FALSE;
        mDialog->Show();
    }
    return NOERROR;
}

ECode DialogFragment::OnSaveInstanceState(
    /* [in] */ IBundle* outState)
{
    Fragment::OnSaveInstanceState(outState);
    if (mDialog != NULL) {
        AutoPtr<IBundle> dialogState;
        mDialog->OnSaveInstanceState((IBundle**)&dialogState);
        if (dialogState != NULL) {
            outState->PutBundle(String(SAVED_DIALOG_STATE_TAG), dialogState);
        }
    }
    if (mStyle != IDialogFragment::STYLE_NORMAL) {
        outState->PutInt32(String(SAVED_STYLE), mStyle);
    }
    if (mTheme != 0) {
        outState->PutInt32(String(SAVED_THEME), mTheme);
    }
    if (!mCancelable) {
        outState->PutBoolean(String(SAVED_CANCELABLE), mCancelable);
    }
    if (!mShowsDialog) {
        outState->PutBoolean(String(SAVED_SHOWS_DIALOG), mShowsDialog);
    }
    if (mBackStackId != -1) {
        outState->PutInt32(String(SAVED_BACK_STACK_ID), mBackStackId);
    }
    return NOERROR;
}

ECode DialogFragment::OnStop()
{
    Fragment::OnStop();
    if (mDialog != NULL) {
        mDialog->Hide();
    }
    return NOERROR;
}

ECode DialogFragment::OnDestroyView()
{
    Fragment::OnDestroyView();
    if (mDialog != NULL) {
        // Set removed here because this dismissal is just to hide
        // the dialog -- we don't want this to cause the fragment to
        // actually be removed.
        mViewDestroyed = TRUE;
        mDialog->Dismiss();
        mDialog = NULL;
    }
    return NOERROR;
}

ECode DialogFragment::Dump(
    /* [in] */ const String& prefix,
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ IPrintWriter* writer,
    /* [in] */ ArrayOf<String>* args)
{
    Fragment::Dump(prefix, fd, writer, args);
    writer->PrintString(prefix); writer->PrintStringln(String("DialogFragment:"));
    writer->PrintString(prefix); writer->PrintString(String("  mStyle="));
    writer->PrintInt32(mStyle);
    writer->PrintString(String(" mTheme=0x"));
//    writer->PrintStringln(Integer.toHexString(mTheme));
    writer->PrintString(prefix); writer->PrintString(String("  mCancelable="));
    writer->PrintBoolean(mCancelable);
    writer->PrintString(String(" mShowsDialog=")); writer->PrintBoolean(mShowsDialog);
    writer->PrintString(String(" mBackStackId=")); writer->PrintInt32ln(mBackStackId);
    writer->PrintString(prefix); writer->PrintString(String("  mDialog="));
    writer->PrintObjectln((IInterface*)mDialog);
    writer->PrintString(prefix); writer->PrintString(String("  mViewDestroyed="));
    writer->PrintBoolean(mViewDestroyed);
    writer->PrintString(String(" mDismissed=")); writer->PrintBoolean(mDismissed);
    writer->PrintString(String(" mShownByMe=")); writer->PrintBooleanln(mShownByMe);
    return NOERROR;
}

} // namespace App
} // namespace Droid
} // namespace Elastos
