
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {

/**
 * Initializes the views.
 *
 * @param rootView View that contains all the content, such as the label, gradient view, etc.
 * @param textResourceId The resource ID of the text to show on the label.
 * @param seekBarMax The range of the seek bar.
 * @param seekBarListener The listener for when the seek bar value changes.
 */
ColorPickerAdvancedComponent::ColorPickerAdvancedComponent(
    /* in */ const IView* rootView,
    /* in */ const Int32 textResourceId,
    /* in */ const Int32 seekBarMax,
    /* in */ const IOnSeekBarChangeListener* seekBarListener)
{
    rootView->FindViewById(R::id::gradient, (IView**)&mGradientView);
    rootView->FindViewById(R::id::text, (ITextView**)&mText);
    mText->SetText(textResourceId);
    CGradientDrawable::New(IGradientDrawable::Orientation::LEFT_RIGHT, NULL, (IGradientDrawable**)&mGradientDrawable);
    rootView->FindViewById(R::id::seek_bar, (ISeekBar**)&mSeekBar);
    mSeekBar->SetOnSeekBarChangeListener(seekBarListener);
    mSeekBar->SetMax(seekBarMax);
    // Setting the thumb offset means the seek bar thumb can move all the way to each end
    // of the gradient view.
    AutoPtr<IContext> context;
    rootView->GetContext((IContext**)&context);

    AutoPtr<IResources> resources;
    context->GetResources((IResources**)&resources);

    AutoPtr<IDrawable> drawable;
    resources->GetDrawable(R::drawable::color_picker_advanced_select_handle, (IDrawable**)&drawable);
    Int32 offset = 0;
    drawable->GetIntrinsicWidth(&offset);
    mSeekBar->SetThumbOffset(offset / 2);
}

Float ColorPickerAdvancedComponent::ColorPickerAdvancedComponent::GetValue()
{
	Float progress = 0.0f;
	mSeekBar->GetProgress(&progress);
    return progress;
}

/**
 * Sets the value of the component (by setting the seek bar value).
 *
 * @param newValue The value to give the component.
 */
ECode ColorPickerAdvancedComponent::SetValue(
    /* in */ Float newValue)
{
    mSeekBar->SetProgress((Int32)newValue);
    return NOERROR;
}

/**
 * Sets the colors for the gradient view to interpolate through.
 *
 * @param newColors The set of colors representing the interpolation points for the gradient.
 */
ECode ColorPickerAdvancedComponent::SetGradientColors(
    /* in */ ArrayOf<Int32>* newColors)
{
	VALIDATE_NOT_NULL(newColors);
    mGradientColors = newColors->Clone();
    if (IBuild::VERSION::SDK_INT < IBuild::VERSION_CODES::JELLY_BEAN) {
        AutoPtr<IOrientation> currentOrientation = IOrientation::LEFT_RIGHT;
        CGradientDrawable::New(currentOrientation, mGradientColors, (IGradientDrawable**)&mGradientDrawable);
    }
    else {
        mGradientDrawable->SetColors(mGradientColors);
    }

    ApiCompatibilityUtils::SetBackgroundForView(mGradientView, mGradientDrawable);
    return NOERROR;
}

} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
