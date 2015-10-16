
#include "elastos/droid/app/CAlertDialogBuilder.h"
#include "elastos/droid/app/CAlertDialog.h"
#include "elastos/droid/app/CAlertControllerAlertParams.h"
#include "elastos/droid/view/CContextThemeWrapper.h"

using Elastos::Droid::View::IContextThemeWrapper;
using Elastos::Droid::View::CContextThemeWrapper;
using Elastos::Droid::Internal::App::CAlertControllerAlertParams;

namespace Elastos {
namespace Droid {
namespace App {

CAlertDialogBuilder::CAlertDialogBuilder()
    : mTheme(0)
{
}

/**
 * Constructor using a context for this builder and the {@link AlertDialog} it creates.
 */
ECode CAlertDialogBuilder::constructor(
    /* [in] */ IContext* context)
{
    return constructor(context, AlertDialog::ResolveDialogTheme(context, 0));
}

/**
 * Constructor using a context and theme for this builder and
 * the {@link AlertDialog} it creates.  The actual theme
 * that an AlertDialog uses is a private implementation, however you can
 * here supply either the name of an attribute in the theme from which
 * to get the dialog's style (such as {@link android.R.attr#alertDialogTheme}
 * or one of the constants
 * {@link AlertDialog#THEME_TRADITIONAL AlertDialog.THEME_TRADITIONAL},
 * {@link AlertDialog#THEME_HOLO_DARK AlertDialog.THEME_HOLO_DARK}, or
 * {@link AlertDialog#THEME_HOLO_LIGHT AlertDialog.THEME_HOLO_LIGHT}.
 */
ECode CAlertDialogBuilder::constructor(
    /* [in] */ IContext* context,
    /* [in] */ Int32 theme)
{
    AutoPtr<IContextThemeWrapper> wrapper;
    CContextThemeWrapper::New(context, AlertDialog::ResolveDialogTheme(context, theme),
        (IContextThemeWrapper**)&wrapper);
    CAlertControllerAlertParams::New(IContext::Probe(wrapper), (IAlertControllerAlertParams**)&mP);
    assert(mP != NULL);
    mTheme = theme;

    return NOERROR;
}

/**
 * Returns a {@link Context} with the appropriate theme for dialogs created by this Builder.
 * Applications should use this Context for obtaining LayoutInflaters for inflating views
 * that will be used in the resulting dialogs, as it will cause views to be inflated with
 * the correct theme.
 *
 * @return A Context for built Dialogs.
 */
ECode CAlertDialogBuilder::GetContext(
    /* [out] */ IContext** context)
{
    return mP->GetContext(context);
}

/**
 * Set the title using the given resource id.
 *
 * @return This Builder object to allow for chaining of calls to Set methods
 */
ECode CAlertDialogBuilder::SetTitle(
    /* [in] */ Int32 titleId)
{
    AutoPtr<IContext> context;
    mP->GetContext((IContext**)&context);
    AutoPtr<ICharSequence> text;
    context->GetText(titleId, (ICharSequence**)&text);
    return mP->SetTitle(text);
}

/**
 * Set the title displayed in the {@link Dialog}.
 *
 * @return This Builder object to allow for chaining of calls to Set methods
 */
ECode CAlertDialogBuilder::SetTitle(
    /* [in] */ ICharSequence* title)
{
    return mP->SetTitle(title);
}

/**
 * Set the title using the custom view {@code customTitleView}. The
 * methods {@link #setTitle(Int32)} and {@link #setIcon(Int32)} should be
 * sufficient for most titles, but this is provided if the title needs
 * more customization. Using this will replace the title and icon Set
 * via the other methods.
 *
 * @param customTitleView The custom view to use as the title.
 *
 * @return This Builder object to allow for chaining of calls to Set methods
 */
ECode CAlertDialogBuilder::SetCustomTitle(
    /* [in] */ IView* customTitleView)
{
    return mP->SetCustomTitleView(customTitleView);
}

/**
 * Set the message to display using the given resource id.
 *
 * @return This Builder object to allow for chaining of calls to Set methods
 */
ECode CAlertDialogBuilder::SetMessage(
    /* [in] */ Int32 messageId)
{
    AutoPtr<IContext> context;
    mP->GetContext((IContext**)&context);
    AutoPtr<ICharSequence> message;
    context->GetText(messageId, (ICharSequence**)&message);
    return mP->SetMessage(message);
}

/**
 * Set the message to display.
  *
 * @return This Builder object to allow for chaining of calls to Set methods
 */
ECode CAlertDialogBuilder::SetMessage(
    /* [in] */ ICharSequence* message)
{
    return mP->SetMessage(message);
}

/**
 * Set the resource id of the {@link Drawable} to be used in the title.
 *
 * @return This Builder object to allow for chaining of calls to Set methods
 */
ECode CAlertDialogBuilder::SetIcon(
    /* [in] */ Int32 iconId)
{
    return mP->SetIconId(iconId);
}

/**
 * Set the {@link Drawable} to be used in the title.
  *
 * @return This Builder object to allow for chaining of calls to Set methods
 */
ECode CAlertDialogBuilder::SetIcon(
    /* [in] */ IDrawable* icon)
{
    return mP->SetIcon(icon);
}

/**
 * Set an icon as supplied by a theme attribute. e.g. android.R.attr.alertDialogIcon
 *
 * @param attrId ID of a theme attribute that points to a drawable resource.
 */
ECode CAlertDialogBuilder::SetIconAttribute(
    /* [in] */ Int32 attrId)
{
    AutoPtr<IContext> context;
    mP->GetContext((IContext**)&context);
    Int32 resourceId = Dialog::GetResourceId(context, attrId);
    return mP->SetIconAttrId(resourceId);
}

/**
 * Set a listener to be invoked when the positive button of the dialog is pressed.
 * @param textId The resource id of the text to display in the positive button
 * @param listener The {@link DialogInterface.OnClickListener} to use.
 *
 * @return This Builder object to allow for chaining of calls to Set methods
 */
ECode CAlertDialogBuilder::SetPositiveButton(
    /* [in] */ Int32 textId,
    /* [in] */ IDialogInterfaceOnClickListener* listener)
{
    AutoPtr<IContext> context;
    mP->GetContext((IContext**)&context);
    AutoPtr<ICharSequence> text;
    context->GetText(textId, (ICharSequence**)&text);

    mP->SetPositiveButtonText(text);
    return mP->SetPositiveButtonListener(listener);
}

/**
 * Set a listener to be invoked when the positive button of the dialog is pressed.
 * @param text The text to display in the positive button
 * @param listener The {@link DialogInterface.OnClickListener} to use.
 *
 * @return This Builder object to allow for chaining of calls to Set methods
 */
ECode CAlertDialogBuilder::SetPositiveButton(
    /* [in] */ ICharSequence* text,
    /* [in] */ IDialogInterfaceOnClickListener* listener)
{
    mP->SetPositiveButtonText(text);
    return mP->SetPositiveButtonListener(listener);
}

/**
 * Set a listener to be invoked when the negative button of the dialog is pressed.
 * @param textId The resource id of the text to display in the negative button
 * @param listener The {@link DialogInterface.OnClickListener} to use.
 *
 * @return This Builder object to allow for chaining of calls to Set methods
 */
ECode CAlertDialogBuilder::SetNegativeButton(
    /* [in] */ Int32 textId,
    /* [in] */ IDialogInterfaceOnClickListener* listener)
{
    AutoPtr<IContext> context;
    mP->GetContext((IContext**)&context);
    AutoPtr<ICharSequence> text;
    context->GetText(textId, (ICharSequence**)&text);

    mP->SetNegativeButtonText(text);
    return mP->SetNegativeButtonListener(listener);
}

/**
 * Set a listener to be invoked when the negative button of the dialog is pressed.
 * @param text The text to display in the negative button
 * @param listener The {@link DialogInterface.OnClickListener} to use.
 *
 * @return This Builder object to allow for chaining of calls to Set methods
 */
ECode CAlertDialogBuilder::SetNegativeButton(
    /* [in] */ ICharSequence* text,
    /* [in] */ IDialogInterfaceOnClickListener* listener)
{
    mP->SetNegativeButtonText(text);
    return mP->SetNegativeButtonListener(listener);
}

/**
 * Set a listener to be invoked when the neutral button of the dialog is pressed.
 * @param textId The resource id of the text to display in the neutral button
 * @param listener The {@link DialogInterface.OnClickListener} to use.
 *
 * @return This Builder object to allow for chaining of calls to Set methods
 */
ECode CAlertDialogBuilder::SetNeutralButton(
    /* [in] */ Int32 textId,
    /* [in] */ IDialogInterfaceOnClickListener* listener)
{
    AutoPtr<IContext> context;
    mP->GetContext((IContext**)&context);
    AutoPtr<ICharSequence> text;
    context->GetText(textId, (ICharSequence**)&text);

    mP->SetNeutralButtonText(text);
    return mP->SetNeutralButtonListener(listener);
}

/**
 * Set a listener to be invoked when the neutral button of the dialog is pressed.
 * @param text The text to display in the neutral button
 * @param listener The {@link DialogInterface.OnClickListener} to use.
 *
 * @return This Builder object to allow for chaining of calls to Set methods
 */
ECode CAlertDialogBuilder::SetNeutralButton(
    /* [in] */ ICharSequence* text,
    /* [in] */ IDialogInterfaceOnClickListener* listener)
{
    mP->SetNeutralButtonText(text);
    return mP->SetNeutralButtonListener(listener);
}

/**
 * Sets whether the dialog is cancelable or not.  Default is TRUE.
 *
 * @return This Builder object to allow for chaining of calls to Set methods
 */
ECode CAlertDialogBuilder::SetCancelable(
    /* [in] */ Boolean cancelable)
{
    return mP->SetCancelable(cancelable);
}

/**
 * Sets the callback that will be called if the dialog is canceled.
 *
 * <p>Even in a cancelable dialog, the dialog may be dismissed for reasons other than
 * being canceled or one of the supplied choices being selected.
 * If you are interested in listening for all cases where the dialog is dismissed
 * and not just when it is canceled, see
 * {@link #setOnDismissListener(android.content.DialogInterface.OnDismissListener) setOnDismissListener}.</p>
 * @see #setCancelable(boolean)
 * @see #setOnDismissListener(android.content.DialogInterface.OnDismissListener)
 *
 * @return This Builder object to allow for chaining of calls to set methods
 */
ECode CAlertDialogBuilder::SetOnCancelListener(
    /* [in] */ IDialogInterfaceOnCancelListener* onCancelListener)
{
    return mP->SetOnCancelListener(onCancelListener);
}

/**
 * Sets the callback that will be called when the dialog is dismissed for any reason.
 *
 * @return This Builder object to allow for chaining of calls to set methods
 */
ECode CAlertDialogBuilder::SetOnDismissListener(
    /* [in] */ IDialogInterfaceOnDismissListener* onDismissListener)
{
    return mP->SetOnDismissListener(onDismissListener);
}

/**
 * Sets the callback that will be called if a key is dispatched to the dialog.
 *
 * @return This Builder object to allow for chaining of calls to Set methods
 */
ECode CAlertDialogBuilder::SetOnKeyListener(
    /* [in] */ IDialogInterfaceOnKeyListener* onKeyListener)
{
    return mP->SetOnKeyListener(onKeyListener);
}

/**
 * Set a list of items to be displayed in the dialog as the content, you will be notified of the
 * selected item via the supplied listener. This should be an array type i.e. R.array.foo
 *
 * @return This Builder object to allow for chaining of calls to Set methods
 */
ECode CAlertDialogBuilder::SetItems(
    /* [in] */ Int32 itemsId,
    /* [in] */ IDialogInterfaceOnClickListener* listener)
{
    AutoPtr<IContext> context;
    mP->GetContext((IContext**)&context);
    AutoPtr<IResources> resources;
    context->GetResources((IResources**)&resources);
    AutoPtr<ArrayOf<ICharSequence*> > items;
    resources->GetTextArray(itemsId, (ArrayOf<ICharSequence*>**)&items);
    mP->SetItems(items);
    return mP->SetOnClickListener(listener);
}

/**
 * Set a list of items to be displayed in the dialog as the content, you will be notified of the
 * selected item via the supplied listener.
 *
 * @return This Builder object to allow for chaining of calls to Set methods
 */
ECode CAlertDialogBuilder::SetItems(
    /* [in] */ ArrayOf<ICharSequence*>* _items,
    /* [in] */ IDialogInterfaceOnClickListener* listener)
{
    if (_items) {
        AutoPtr<ArrayOf<ICharSequence*> > items = _items->Clone();
        mP->SetItems(items);
    }
    else {
        mP->SetItems(NULL);
    }
    return mP->SetOnClickListener(listener);
}

/**
 * Set a list of items, which are supplied by the given {@link ListAdapter}, to be
 * displayed in the dialog as the content, you will be notified of the
 * selected item via the supplied listener.
 *
 * @param adapter The {@link ListAdapter} to supply the list of items
 * @param listener The listener that will be called when an item is clicked.
 *
 * @return This Builder object to allow for chaining of calls to Set methods
 */
ECode CAlertDialogBuilder::SetAdapter(
    /* [in] */ IListAdapter* adapter,
    /* [in] */ IDialogInterfaceOnClickListener* listener)
{
    mP->SetAdapter(adapter);
    return mP->SetOnClickListener(listener);
}

/**
 * Set a list of items, which are supplied by the given {@link Cursor}, to be
 * displayed in the dialog as the content, you will be notified of the
 * selected item via the supplied listener.
 *
 * @param cursor The {@link Cursor} to supply the list of items
 * @param listener The listener that will be called when an item is clicked.
 * @param labelColumn The column name on the cursor containing the string to display
 *          in the label.
 *
 * @return This Builder object to allow for chaining of calls to Set methods
 */
ECode CAlertDialogBuilder::SetCursor(
    /* [in] */ ICursor* cursor,
    /* [in] */ IDialogInterfaceOnClickListener* listener,
    /* [in] */ const String& labelColumn)
{
    mP->SetCursor(cursor);
    mP->SetLabelColumn(labelColumn);
    return mP->SetOnClickListener(listener);
}

/**
 * Set a list of items to be displayed in the dialog as the content,
 * you will be notified of the selected item via the supplied listener.
 * This should be an array type, e.g. R.array.foo. The list will have
 * a check mark displayed to the right of the text for each checked
 * item. Clicking on an item in the list will not dismiss the dialog.
 * Clicking on a button will dismiss the dialog.
 *
 * @param itemsId the resource id of an array i.e. R.array.foo
 * @param checkedItems specifies which items are checked. It should be null in which case no
 *        items are checked. If non null it must be exactly the same length as the array of
 *        items.
 * @param listener notified when an item on the list is clicked. The dialog will not be
 *        dismissed when an item is clicked. It will only be dismissed if clicked on a
 *        button, if no buttons are supplied it's up to the user to dismiss the dialog.
 *
 * @return This Builder object to allow for chaining of calls to Set methods
 */
ECode CAlertDialogBuilder::SetMultiChoiceItems(
    /* [in] */ Int32 itemsId,
    /* [in] */ ArrayOf<Boolean>* _checkedItems,
    /* [in] */ IDialogInterfaceOnMultiChoiceClickListener* listener)
{
    AutoPtr<IContext> context;
    mP->GetContext((IContext**)&context);
    AutoPtr<IResources> resources;
    context->GetResources((IResources**)&resources);
    AutoPtr<ArrayOf<ICharSequence*> > items;
    resources->GetTextArray(itemsId, (ArrayOf<ICharSequence*>**)&items);
    mP->SetItems(items);

    mP->SetOnCheckboxClickListener(listener);

    if (_checkedItems) {
        AutoPtr<ArrayOf<Boolean> > checkedItems = _checkedItems->Clone();
        mP->SetCheckedItems(checkedItems);
    }
    else {
        mP->SetCheckedItems(NULL);
    }

    return mP->SetIsMultiChoice(TRUE);
}

/**
 * Set a list of items to be displayed in the dialog as the content,
 * you will be notified of the selected item via the supplied listener.
 * The list will have a check mark displayed to the right of the text
 * for each checked item. Clicking on an item in the list will not
 * dismiss the dialog. Clicking on a button will dismiss the dialog.
 *
 * @param items the text of the items to be displayed in the list.
 * @param checkedItems specifies which items are checked. It should be null in which case no
 *        items are checked. If non null it must be exactly the same length as the array of
 *        items.
 * @param listener notified when an item on the list is clicked. The dialog will not be
 *        dismissed when an item is clicked. It will only be dismissed if clicked on a
 *        button, if no buttons are supplied it's up to the user to dismiss the dialog.
 *
 * @return This Builder object to allow for chaining of calls to Set methods
 */
ECode CAlertDialogBuilder::SetMultiChoiceItems(
    /* [in] */ ArrayOf<ICharSequence*>* _items,
    /* [in] */ ArrayOf<Boolean>* _checkedItems,
    /* [in] */ IDialogInterfaceOnMultiChoiceClickListener* listener)
{
    if (_items) {
        AutoPtr<ArrayOf<ICharSequence*> > items = _items->Clone();
        mP->SetItems(items);
    }
    else {
        mP->SetItems(NULL);
    }

    if (_checkedItems) {
        AutoPtr<ArrayOf<Boolean> > checkedItems = _checkedItems->Clone();
        mP->SetCheckedItems(checkedItems);
    }
    else {
        mP->SetCheckedItems(NULL);
    }

    mP->SetOnCheckboxClickListener(listener);
    return mP->SetIsMultiChoice(TRUE);
}

/**
 * Set a list of items to be displayed in the dialog as the content,
 * you will be notified of the selected item via the supplied listener.
 * The list will have a check mark displayed to the right of the text
 * for each checked item. Clicking on an item in the list will not
 * dismiss the dialog. Clicking on a button will dismiss the dialog.
 *
 * @param cursor the cursor used to provide the items.
 * @param isCheckedColumn specifies the column name on the cursor to use to determine
 *        whether a checkbox is checked or not. It must return an Int32eger value where 1
 *        means checked and 0 means unchecked.
 * @param labelColumn The column name on the cursor containing the string to display in the
 *        label.
 * @param listener notified when an item on the list is clicked. The dialog will not be
 *        dismissed when an item is clicked. It will only be dismissed if clicked on a
 *        button, if no buttons are supplied it's up to the user to dismiss the dialog.
 *
 * @return This Builder object to allow for chaining of calls to Set methods
 */
ECode CAlertDialogBuilder::SetMultiChoiceItems(
    /* [in] */ ICursor* cursor,
    /* [in] */ const String& isCheckedColumn,
    /* [in] */ const String& labelColumn,
    /* [in] */ IDialogInterfaceOnMultiChoiceClickListener* listener)
{
    mP->SetCursor(cursor);
    mP->SetOnCheckboxClickListener(listener);
    mP->SetIsCheckedColumn(isCheckedColumn);
    mP->SetLabelColumn(labelColumn);
    mP->SetIsMultiChoice(TRUE);
    return NOERROR;
}

/**
 * Set a list of items to be displayed in the dialog as the content, you will be notified of
 * the selected item via the supplied listener. This should be an array type i.e.
 * R.array.foo The list will have a check mark displayed to the right of the text for the
 * checked item. Clicking on an item in the list will not dismiss the dialog. Clicking on a
 * button will dismiss the dialog.
 *
 * @param itemsId the resource id of an array i.e. R.array.foo
 * @param checkedItem specifies which item is checked. If -1 no items are checked.
 * @param listener notified when an item on the list is clicked. The dialog will not be
 *        dismissed when an item is clicked. It will only be dismissed if clicked on a
 *        button, if no buttons are supplied it's up to the user to dismiss the dialog.
 *
 * @return This Builder object to allow for chaining of calls to Set methods
 */
ECode CAlertDialogBuilder::SetSingleChoiceItems(
    /* [in] */ Int32 itemsId,
    /* [in] */ Int32 checkedItem,
    /* [in] */ IDialogInterfaceOnClickListener* listener)
{
    AutoPtr<IContext> context;
    mP->GetContext((IContext**)&context);
    AutoPtr<IResources> resources;
    context->GetResources((IResources**)&resources);
    AutoPtr<ArrayOf<ICharSequence*> > items;
    resources->GetTextArray(itemsId, (ArrayOf<ICharSequence*>**)&items);
    mP->SetItems(items);

    mP->SetOnClickListener(listener);
    mP->SetCheckedItem(checkedItem);
    mP->SetIsSingleChoice(TRUE);
    return NOERROR;
}

/**
 * Set a list of items to be displayed in the dialog as the content, you will be notified of
 * the selected item via the supplied listener. The list will have a check mark displayed to
 * the right of the text for the checked item. Clicking on an item in the list will not
 * dismiss the dialog. Clicking on a button will dismiss the dialog.
 *
 * @param cursor the cursor to retrieve the items from.
 * @param checkedItem specifies which item is checked. If -1 no items are checked.
 * @param labelColumn The column name on the cursor containing the string to display in the
 *        label.
 * @param listener notified when an item on the list is clicked. The dialog will not be
 *        dismissed when an item is clicked. It will only be dismissed if clicked on a
 *        button, if no buttons are supplied it's up to the user to dismiss the dialog.
 *
 * @return This Builder object to allow for chaining of calls to Set methods
 */
ECode CAlertDialogBuilder::SetSingleChoiceItems(
    /* [in] */ ICursor* cursor,
    /* [in] */ Int32 checkedItem,
    /* [in] */ const String& labelColumn,
    /* [in] */ IDialogInterfaceOnClickListener* listener)
{
    mP->SetCursor(cursor);
    mP->SetOnClickListener(listener);
    mP->SetCheckedItem(checkedItem);
    mP->SetLabelColumn(labelColumn);
    mP->SetIsSingleChoice(TRUE);
    return NOERROR;
}

/**
 * Set a list of items to be displayed in the dialog as the content, you will be notified of
 * the selected item via the supplied listener. The list will have a check mark displayed to
 * the right of the text for the checked item. Clicking on an item in the list will not
 * dismiss the dialog. Clicking on a button will dismiss the dialog.
 *
 * @param items the items to be displayed.
 * @param checkedItem specifies which item is checked. If -1 no items are checked.
 * @param listener notified when an item on the list is clicked. The dialog will not be
 *        dismissed when an item is clicked. It will only be dismissed if clicked on a
 *        button, if no buttons are supplied it's up to the user to dismiss the dialog.
 *
 * @return This Builder object to allow for chaining of calls to Set methods
 */
ECode CAlertDialogBuilder::SetSingleChoiceItems(
    /* [in] */ ArrayOf<ICharSequence*>* _items,
    /* [in] */ Int32 checkedItem,
    /* [in] */ IDialogInterfaceOnClickListener* listener)
{
    if (_items) {
        AutoPtr<ArrayOf<ICharSequence*> > items = _items->Clone();
        mP->SetItems(items);
    }
    else {
        mP->SetItems(NULL);
    }
    mP->SetOnClickListener(listener);
    mP->SetCheckedItem(checkedItem);
    mP->SetIsSingleChoice(TRUE);
    return NOERROR;
}

/**
 * Set a list of items to be displayed in the dialog as the content, you will be notified of
 * the selected item via the supplied listener. The list will have a check mark displayed to
 * the right of the text for the checked item. Clicking on an item in the list will not
 * dismiss the dialog. Clicking on a button will dismiss the dialog.
 *
 * @param adapter The {@link ListAdapter} to supply the list of items
 * @param checkedItem specifies which item is checked. If -1 no items are checked.
 * @param listener notified when an item on the list is clicked. The dialog will not be
 *        dismissed when an item is clicked. It will only be dismissed if clicked on a
 *        button, if no buttons are supplied it's up to the user to dismiss the dialog.
 *
 * @return This Builder object to allow for chaining of calls to Set methods
 */
ECode CAlertDialogBuilder::SetSingleChoiceItems(
    /* [in] */ IListAdapter* adapter,
    /* [in] */ Int32 checkedItem,
    /* [in] */ IDialogInterfaceOnClickListener* listener)
{
    mP->SetAdapter(adapter);
    mP->SetOnClickListener(listener);
    mP->SetCheckedItem(checkedItem);
    mP->SetIsSingleChoice(TRUE);
    return NOERROR;
}

/**
 * Sets a listener to be invoked when an item in the list is selected.
 *
 * @param listener The listener to be invoked.
 * @see AdapterView#setOnItemSelectedListener(android.widget.AdapterView.OnItemSelectedListener)
 *
 * @return This Builder object to allow for chaining of calls to Set methods
 */
ECode CAlertDialogBuilder::SetOnItemSelectedListener(
    /* [in] */ IAdapterViewOnItemSelectedListener* listener)
{
    mP->SetOnItemSelectedListener(listener);
    return NOERROR;
}

/**
 * Set a custom view to be the contents of the Dialog. If the supplied view is an instance
 * of a {@link ListView} the light background will be used.
 *
 * @param view The view to use as the contents of the Dialog.
 *
 * @return This Builder object to allow for chaining of calls to Set methods
 */
ECode CAlertDialogBuilder::SetView(
    /* [in] */ IView* view)
{
    mP->SetView(view);
    mP->SetViewSpacingSpecified(FALSE);
    return NOERROR;
}

/**
 * Set a custom view to be the contents of the Dialog, specifying the
 * spacing to appear around that view. If the supplied view is an
 * instance of a {@link ListView} the light background will be used.
 *
 * @param view The view to use as the contents of the Dialog.
 * @param viewSpacingLeft Spacing between the left edge of the view and
 *        the dialog frame
 * @param viewSpacingTop Spacing between the top edge of the view and
 *        the dialog frame
 * @param viewSpacingRight Spacing between the right edge of the view
 *        and the dialog frame
 * @param viewSpacingBottom Spacing between the bottom edge of the view
 *        and the dialog frame
 * @return This Builder object to allow for chaining of calls to Set
 *         methods
 *
 *
 * This is currently hidden because it seems like people should just
 * be able to put padding around the view.
 * @hide
 */
ECode CAlertDialogBuilder::SetView(
    /* [in] */ IView* view,
    /* [in] */ Int32 viewSpacingLeft,
    /* [in] */ Int32 viewSpacingTop,
    /* [in] */ Int32 viewSpacingRight,
    /* [in] */ Int32 viewSpacingBottom)
{
    mP->SetView(view);
    mP->SetViewSpacingSpecified(TRUE);
    mP->SetViewSpacingLeft(viewSpacingLeft);
    mP->SetViewSpacingTop(viewSpacingTop);
    mP->SetViewSpacingRight(viewSpacingRight);
    mP->SetViewSpacingBottom(viewSpacingBottom);
    return NOERROR;
}

/**
 * Sets the Dialog to use the inverse background, regardless of what the
 * contents is.
 *
 * @param useInverseBackground Whether to use the inverse background
 *
 * @return This Builder object to allow for chaining of calls to Set methods
 */
ECode CAlertDialogBuilder::SetInverseBackgroundForced(
    /* [in] */ Boolean useInverseBackground)
{
    return mP->SetForceInverseBackground(useInverseBackground);
}

/**
 * @hide
 */
ECode CAlertDialogBuilder::SetRecycleOnMeasureEnabled(
    /* [in] */ Boolean enabled)
{
    return mP->SetRecycleOnMeasure(enabled);
}


/**
 * Creates a {@link AlertDialog} with the arguments supplied to this builder. It does not
 * {@link Dialog#show()} the dialog. This allows the user to do any extra processing
 * before displaying the dialog. Use {@link #show()} if you don't have any other processing
 * to do and want this to be created and displayed.
 */
ECode CAlertDialogBuilder::Create(
    /* [out] */ IAlertDialog** dialog)
{
    VALIDATE_NOT_NULL(dialog);

    AutoPtr<IContext> context;
    mP->GetContext((IContext**)&context);
    AutoPtr<CAlertDialog> cdialog;
    CAlertDialog::NewByFriend(context, mTheme, (CAlertDialog**)&cdialog);
    mP->Apply(cdialog->mAlert);

    Boolean cancelable;
    mP->GetCancelable(&cancelable);
    cdialog->SetCancelable(cancelable);
    if (cancelable) {
        cdialog->SetCanceledOnTouchOutside(TRUE);
    }

    AutoPtr<IDialogInterfaceOnCancelListener> onCancleListener;
    mP->GetOnCancelListener((IDialogInterfaceOnCancelListener**)&onCancleListener);

    AutoPtr<IDialogInterfaceOnDismissListener> onDismissListener;
    mP->GetOnDismissListener((IDialogInterfaceOnDismissListener**)&onDismissListener);

    AutoPtr<IDialogInterfaceOnKeyListener> onKeyListener;
    mP->GetOnKeyListener((IDialogInterfaceOnKeyListener**)&onKeyListener);

    cdialog->SetOnCancelListener(onCancleListener);
    cdialog->SetOnDismissListener(onDismissListener);
    if (onKeyListener != NULL) {
        cdialog->SetOnKeyListener(onKeyListener);
    }

    *dialog = cdialog.Get();
    REFCOUNT_ADD(*dialog);

    return NOERROR;
}

/**
 * Creates a {@link AlertDialog} with the arguments supplied to this builder and
 * {@link Dialog#show()}'s the dialog.
 */
ECode CAlertDialogBuilder::Show(
    /* [out] */ IAlertDialog** dialog)
{
    FAIL_RETURN(Create(dialog));
    (*dialog)->Show();

    return NOERROR;
}

} // namespace App
} // namespace Droid
} // namespace Elastos
