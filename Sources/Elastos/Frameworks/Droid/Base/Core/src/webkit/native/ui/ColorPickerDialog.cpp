
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {

//===============================================================
// 		ColorPickerDialog::InnerPositiveButtonClickListener
//===============================================================
ColorPickerDialog::InnerPositiveButtonClickListener::InnerPositiveButtonClickListener(
    /* in */ ColorPickerDialog* dialog)
    : mColorPickerDialog(dialog)
{
}

//@Override
ECode ColorPickerDialog::InnerPositiveButtonClickListener::OnClick(
    /* in */ IDialogInterface* dialogInterface,
    /* in */ Int32 i)
{
	VALIDATE_NOT_NULL(dialogInterface);
	mColorPickerDialog->TryNotifyColorSet(mColorPickerDialog->mCurrentColor);
	return NOERROR;
}

//===============================================================
// 		ColorPickerDialog::InnerNegativeButtonClickListener
//===============================================================
ColorPickerDialog::InnerNegativeButtonClickListener::InnerNegativeButtonClickListener(
    /* in */ ColorPickerDialog* dialog)
    : mColorPickerDialog(dialog)
{
}

//@Override
ECode ColorPickerDialog::InnerNegativeButtonClickListener::OnClick(
    /* in */ IDialogInterface* dialogInterface,
    /* in */ Int32 i)
{
	VALIDATE_NOT_NULL(dialogInterface);
	mColorPickerDialog->TryNotifyColorSet(mColorPickerDialog->mInitialColor);
	return NOERROR;
}

//===============================================================
// 			ColorPickerDialog::InnerOnCancelListener
//===============================================================
ColorPickerDialog::InnerOnCancelListener::InnerOnCancelListener(
    /* in */ ColorPickerDialog* dialog)
    : mColorPickerDialog(dialog)
{
}

//@Override
ECode ColorPickerDialog::InnerOnCancelListener::OnCancel(
    /* in */ IDialogInterface* arg0)
{
	VALIDATE_NOT_NULL(arg0);
	mColorPickerDialog->TryNotifyColorSet(mColorPickerDialog->mInitialColor);
	return NOERROR;
}

//===============================================================
// 			ColorPickerDialog::InnerOnClickListener
//===============================================================
ColorPickerDialog::InnerOnClickListener::InnerOnClickListener(
    /* in */ ColorPickerDialog* dialog)
    : mColorPickerDialog(dialog)
{
}

//@Override
ECode ColorPickerDialog::InnerOnClickListener::OnClick(
    /* in */ IView* v)
{
	VALIDATE_NOT_NULL(v);
	mColorPickerDialog->ShowAdvancedView();
	return NOERROR;
}

//===============================================================
// 						ColorPickerDialog
//===============================================================
/**
 * @param context The context the dialog is to run in.
 * @param listener The object to notify when the color is set.
 * @param color The initial color to set.
 * @param suggestions The list of suggestions.
 */
ColorPickerDialog::ColorPickerDialog(
    /* in */ IContext* context,
    /* in */ OnColorChangedListener* listener,
    /* in */ Int32 color,
    /* in */ ArrayOf< AutoPtr<ColorSuggestion> >* suggestions)
    : AlertDialog(context, 0)
    , mListener(listener)
    , mInitialColor(color)
    , mCurrentColor(mInitialColor)
{
    // Initialize title
    AutoPtr<ILayoutInflater> inflater;
    context->GetSystemService(IContext::LAYOUT_INFLATER_SERVICE, (ILayoutInflater**)&inflater);

    AutoPtr<IView> title;
    inflater->Inflate(R::layout::color_picker_dialog_title, NULL, (IView**)&title);
    SetCustomTitle(title);

    title->FindViewById(R::id::selected_color_view, (IView**)&mCurrentColorView);

    AutoPtr<ITextView> titleText;
    title->FindViewById(R::id::title, (ITextView**)&titleText);
    titleText->SetText(R::string::color_picker_dialog_title);

    // Initialize Set/Cancel buttons
    String positiveButtonText;
    context->GetString(R::string::color_picker_button_set, &positiveButtonText);

    AutoPtr<InnerPositiveButtonClickListener> positiveListener = new InnerPositiveButtonClickListener(this);
    SetButton(BUTTON_POSITIVE, positiveButtonText, positiveListener);

    // Note that with the color picker there's not really any such thing as
    // "cancelled".
    // The color picker flow only finishes when we return a color, so we
    // have to always
    // return something. The concept of "cancelled" in this case just means
    // returning
    // the color that we were initialized with.
    String negativeButtonText;
    context->GetString(R::string::color_picker_button_cancel, &negativeButtonText);
    AutoPtr<InnerNegativeButtonClickListener> negativeListener = new InnerNegativeButtonClickListener(this);
    SetButton(BUTTON_NEGATIVE, negativeButtonText, negativeListener);

	AutoPtr<InnerOnCancelListener> onCancelListener = new InnerOnCancelListener(this);
    SetOnCancelListener(onCancelListener);

    // Initialize main content view
    AutoPtr<IView> content;
    inflater->Inflate(R::layout::color_picker_dialog_content, NULL, (IView**)&content);
    SetView(content);

    // Initialize More button.
    content->FindViewById(R::id::more_colors_button, (IButton**)&mMoreButton);
    AutoPtr<InnerOnClickListener> onClickListener = new InnerOnClickListener(this);
    mMoreButton->SetOnClickListener(onClickListener);

    // Initialize advanced color view (hidden initially).
    content->FindViewById(R::id::color_picker_advanced, &mAdvancedColorPicker);
    mAdvancedColorPicker->SetVisibility(IView::GONE);

    // Initialize simple color view (default view).
    content->FindViewById(R::id::color_picker_simple, &mSimpleColorPicker);
    mSimpleColorPicker->Init(suggestions, this);

    UpdateCurrentColor(mInitialColor);
}

/**
 * Listens to the ColorPicker for when the user has changed the selected color, and
 * updates the current color (the color shown in the title) accordingly.
 *
 * @param color The new color chosen by the user.
 */
//@Override
ECode ColorPickerDialog::OnColorChanged(
    /* in */ Int32 color)
{
    UpdateCurrentColor(color);
    return NOERROR;
}

/**
 * Hides the simple view (the default) and shows the advanced one instead, hiding the
 * "More" button at the same time.
 */
ECode ColorPickerDialog::ShowAdvancedView()
{
    // Only need to hide the borders, not the Views themselves, since the Views are
    // contained within the borders.
    AutoPtr<IView> buttonBorder;
   	FindViewById(R::id::more_colors_button_border, (IView**)&buttonBorder);
    buttonBorder->SetVisibility(IView::GONE);

    AutoPtr<IView> simpleView;
    FindViewById(R::id::color_picker_simple, (IView**)&simpleView);
    simpleView->SetVisibility(IView::GONE);

    mAdvancedColorPicker->SetVisibility(IView::VISIBLE);
    mAdvancedColorPicker->SetListener(this);
    mAdvancedColorPicker->SetColor(mCurrentColor);
    return NOERROR;
}

/**
 * Tries to notify any listeners that the color has been set.
 */
ECode ColorPickerDialog::TryNotifyColorSet(
    /* in */ Int32 color)
{
    if (mListener != NULL)
    	mListener->OnColorChanged(color);
   	return NOERROR;
}

/**
 * Updates the internal cache of the currently selected color, updating the colorful little
 * box in the title at the same time.
 */
ECode ColorPickerDialog::UpdateCurrentColor(
    /* in */ Int32 color)
{
    mCurrentColor = color;
    if (mCurrentColorView != NULL)
    	mCurrentColorView->SetBackgroundColor(color);
   	return NOERROR;
}

} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
