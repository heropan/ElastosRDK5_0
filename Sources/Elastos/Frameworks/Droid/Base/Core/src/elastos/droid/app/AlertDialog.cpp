#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/R.h"
#include "elastos/droid/app/AlertDialog.h"
#ifdef DROID_CORE
#include "elastos/droid/app/CAlertController.h"
#endif
#include <elastos/utility/logging/Slogger.h>

using Elastos::Utility::Logging::Slogger;
using Elastos::Droid::R;
using Elastos::Droid::Internal::App::CAlertController;
using Elastos::Droid::Content::IDialogInterface;
using Elastos::Droid::Content::EIID_IDialogInterface;

namespace Elastos {
namespace Droid {
namespace App {


AlertDialog::AlertDialog()
{
    Slogger::V("AlertDialog", " >> create AlertDialog() %p", this);
}

AlertDialog::~AlertDialog()
{
    Slogger::V("AlertDialog", " >> destory ~AlertDialog() %p", this);
}

AlertDialog::AlertDialog(
    /* [in] */ IContext* context)
{
    Init(context);
}

AlertDialog::AlertDialog(
    /* [in] */ IContext* context,
    /* [in] */ Int32 theme)
{
    Init(context, theme);
}

AlertDialog::AlertDialog(
    /* [in] */ IContext* context,
    /* [in] */ Int32 theme,
    /* [in] */ Boolean createThemeContextWrapper)
{
    Init(context, theme, createThemeContextWrapper);
}

AlertDialog::AlertDialog(
    /* [in] */ IContext* context,
    /* [in] */ Boolean cancelable,
    /* [in] */ IDialogInterfaceOnCancelListener* cancelListener)
{
    Init(context, cancelable, cancelListener);
}

ECode AlertDialog::Init(
    /* [in] */ IContext* context)
{
    return Init(context, ResolveDialogTheme(context, 0), TRUE);
}

ECode AlertDialog::Init(
    /* [in] */ IContext* context,
    /* [in] */ Int32 theme)
{
    return Init(context, theme, TRUE);
}

ECode AlertDialog::Init(
    /* [in] */ IContext* context,
    /* [in] */ Int32 theme,
    /* [in] */ Boolean createThemeContextWrapper)
{
    FAIL_RETURN(Dialog::Init(context, ResolveDialogTheme(context, theme), createThemeContextWrapper));

    mWindow->AlwaysReadCloseOnTouchAttr();
    AutoPtr<IContext> ctx = GetContext();
    AutoPtr<IWindow> window = GetWindow();
    return CAlertController::New(ctx, THIS_PROBE(IDialogInterface), window,
            (IAlertController**)&mAlert);
}

ECode AlertDialog::Init(
    /* [in] */ IContext* context,
    /* [in] */ Boolean cancelable,
    /* [in] */ IDialogInterfaceOnCancelListener* cancelListener)
{
    FAIL_RETURN(Dialog::Init(context, ResolveDialogTheme(context, 0)));

    mWindow->AlwaysReadCloseOnTouchAttr();
    SetCancelable(cancelable);
    SetOnCancelListener(cancelListener);
    AutoPtr<IWindow> window = GetWindow();
    return CAlertController::New(context, THIS_PROBE(IDialogInterface), window,
            (IAlertController**)&mAlert);
}

Int32 AlertDialog::ResolveDialogTheme(
    /* [in] */ IContext* context,
    /* [in] */ Int32 resid)
{
    if (resid == IAlertDialog::THEME_TRADITIONAL) {
        return R::style::Theme_Dialog_Alert;
    } else if (resid == IAlertDialog::THEME_HOLO_DARK) {
        return R::style::Theme_Holo_Dialog_Alert;
    } else if (resid == IAlertDialog::THEME_HOLO_LIGHT) {
        return R::style::Theme_Holo_Light_Dialog_Alert;
    } else if (resid == IAlertDialog::THEME_DEVICE_DEFAULT_DARK) {
        return R::style::Theme_DeviceDefault_Dialog_Alert;
    } else if (resid == IAlertDialog::THEME_DEVICE_DEFAULT_LIGHT) {
        return R::style::Theme_DeviceDefault_Light_Dialog_Alert;
    } else if (resid >= 0x01000000) {   // start of real resource IDs.
        return resid;
    } else {
        return GetResourceId(context, R::attr::alertDialogTheme);
    }
}

/**
 * Gets one of the buttons used in the dialog.
 * <p>
 * If a button does not exist in the dialog, NULL will be returned.
 *
 * @param whichButton The identifier of the button that should be returned.
 *            For example, this can be
 *            {@link DialogInterface#BUTTON_POSITIVE}.
 * @return The button from the dialog, or NULL if a button does not exist.
 */
AutoPtr<IButton> AlertDialog::GetButton(
    /* [in] */ Int32 whichButton)
{
    AutoPtr<IButton> button;
    mAlert->GetButton(whichButton, (IButton**)&button);
    return button;
}

/**
 * Gets the list view used in the dialog.
 *
 * @return The {@link ListView} from the dialog.
 */
AutoPtr<IListView> AlertDialog::GetListView()
{
    AutoPtr<IListView> listview;
    mAlert->GetListView((IListView**)&listview);
    return listview;
}

//@Override
ECode AlertDialog::SetTitle(
    /* [in] */ ICharSequence* title)
{
    Dialog::SetTitle(title);
    return mAlert->SetTitle(title);
}

/**
 * @see Builder#setCustomTitle(View)
 */
ECode AlertDialog::SetCustomTitle(
    /* [in] */ IView* customTitleView)
{
    return mAlert->SetCustomTitle(customTitleView);
}

ECode AlertDialog::SetMessage(
    /* [in] */ ICharSequence* message)
{
    return mAlert->SetMessage(message);
}

/**
 * Set the view to display in that dialog.
 */
ECode AlertDialog::SetView(
    /* [in] */ IView* view)
{
    return mAlert->SetView(view);
}

/**
 * Set the view to display in that dialog, specifying the spacing to appear around that
 * view.
 *
 * @param view The view to show in the content area of the dialog
 * @param viewSpacingLeft Extra space to appear to the left of {@code view}
 * @param viewSpacingTop Extra space to appear above {@code view}
 * @param viewSpacingRight Extra space to appear to the right of {@code view}
 * @param viewSpacingBottom Extra space to appear below {@code view}
 */
ECode AlertDialog::SetView(
    /* [in] */ IView* view,
    /* [in] */ Int32 viewSpacingLeft,
    /* [in] */ Int32 viewSpacingTop,
    /* [in] */ Int32 viewSpacingRight,
    /* [in] */ Int32 viewSpacingBottom)
{
    return mAlert->SetView(view,
            viewSpacingLeft, viewSpacingTop,
            viewSpacingRight, viewSpacingBottom);
}

ECode AlertDialog::SetButtonPanelLayoutHint(
    /* [in] */ Int32 layoutHint);
{
    return mAlert->SetButtonPanelLayoutHint(layoutHint);
}

/**
 * Set a message to be sent when a button is pressed.
 *
 * @param whichButton Which button to set the message for, can be one of
 *            {@link DialogInterface#BUTTON_POSITIVE},
 *            {@link DialogInterface#BUTTON_NEGATIVE}, or
 *            {@link DialogInterface#BUTTON_NEUTRAL}
 * @param text The text to display in positive button.
 * @param msg The {@link Message} to be sent when clicked.
 */
ECode AlertDialog::SetButton(
    /* [in] */ Int32 whichButton,
    /* [in] */ ICharSequence* text,
    /* [in] */ IMessage* msg)
{
    return mAlert->SetButton(whichButton, text, NULL, msg);
}

/**
 * Set a listener to be invoked when the positive button of the dialog is pressed.
 *
 * @param whichButton Which button to set the listener on, can be one of
 *            {@link DialogInterface#BUTTON_POSITIVE},
 *            {@link DialogInterface#BUTTON_NEGATIVE}, or
 *            {@link DialogInterface#BUTTON_NEUTRAL}
 * @param text The text to display in positive button.
 * @param listener The {@link DialogInterface.OnClickListener} to use.
 */
ECode AlertDialog::SetButton(
    /* [in] */ Int32 whichButton,
    /* [in] */ ICharSequence* text,
    /* [in] */ IDialogInterfaceOnClickListener* listener)
{
    return mAlert->SetButton(whichButton, text, listener, NULL);
}

/**
 * Set resId to 0 if you don't want an icon.
 * @param resId the resourceId of the drawable to use as the icon or 0
 * if you don't want an icon.
 */
ECode AlertDialog::SetIcon(
    /* [in] */ Int32 resId)
{
    return mAlert->SetIcon(resId);
}

ECode AlertDialog::SetIcon(
    /* [in] */ IDrawable* icon)
{
    return mAlert->SetIcon(icon);
}

ECode AlertDialog::SetIconAttribute(
    /* [in] */ Int32 attrId)
{
    Int32 resourceId = GetResourceId(mContext, attrId);
    return mAlert->SetIcon(resourceId);
}

ECode AlertDialog::SetInverseBackgroundForced(
    /* [in] */ Boolean forceInverseBackground)
{
    return mAlert->SetInverseBackgroundForced(forceInverseBackground);
}

//@Override
void AlertDialog::OnCreate(
    /* [in] */ IBundle* savedInstanceState)
{
    Dialog::OnCreate(savedInstanceState);
    mAlert->InstallContent();
}

//@Override
Boolean AlertDialog::OnKeyDown(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    Boolean result;
    mAlert->OnKeyDown(keyCode, event, &result);
    if (result) {
        return TRUE;
    }

    return Dialog::OnKeyDown(keyCode, event);
}

//@Override
Boolean AlertDialog::OnKeyUp(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    Boolean result;
    mAlert->OnKeyUp(keyCode, event, &result);
    if (result) {
        return TRUE;
    }

    return Dialog::OnKeyUp(keyCode, event);
}

} // namespace App
} // namespace Droid
} // namespace Elastos
