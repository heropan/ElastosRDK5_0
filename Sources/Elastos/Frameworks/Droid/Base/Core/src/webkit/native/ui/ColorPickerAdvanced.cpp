
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {

const Int32 ColorPickerAdvanced::HUE_SEEK_BAR_MAX;
const Int32 ColorPickerAdvanced::HUE_COLOR_COUNT;
const Int32 ColorPickerAdvanced::SATURATION_SEEK_BAR_MAX;
const Int32 ColorPickerAdvanced::SATURATION_COLOR_COUNT;
const Int32 ColorPickerAdvanced::VALUE_SEEK_BAR_MAX;
const Int32 ColorPickerAdvanced::VALUE_COLOR_COUNT;

ColorPickerAdvanced::ColorPickerAdvanced(
    /* in */ IContext* context,
    /* in */ IAttributeSet* attrs)
    : LinearLayout(context, attrs)
    , mCurrentColor(0)
	, mHueDetails(NULL)
	, mSaturationDetails(NULL)
	, mValueDetails(NULL)
	, mOnColorChangedListener(NULL)
{
	mCurrentHsvValues = ArrayOf<Float>::Alloc(3);
    Init();
}

ColorPickerAdvanced::ColorPickerAdvanced(
    /* in */ IContext* context,
    /* in */ IAttributeSet* attrs,
    /* in */ Int32 defStyle)
    : LinearLayout(context, attrs, defStyle)
    , mCurrentColor(0)
    , mHueDetails(NULL)
	, mSaturationDetails(NULL)
	, mValueDetails(NULL)
	, mOnColorChangedListener(NULL)
{
	mCurrentHsvValues = ArrayOf<Float>::Alloc(3);
    Init();
}

ColorPickerAdvanced::ColorPickerAdvanced(
    /* in */ IContext* context)
    : LinearLayout(context)
    , mCurrentColor(0)
    , mHueDetails(NULL)
	, mSaturationDetails(NULL)
	, mValueDetails(NULL)
	, mOnColorChangedListener(NULL)
{
	mCurrentHsvValues = ArrayOf<Float>::Alloc(3);
    Init();
}

/**
 * Creates a new GradientDetails object from the parameters provided, initializes it,
 * and adds it to this advanced view.
 *
 * @param textResourceId The text to display for the label.
 * @param seekBarMax The maximum value of the seek bar for the gradient.
 * @param seekBarListener Object listening to when the user changes the seek bar.
 *
 * @return A new GradientDetails object initialized with the given parameters.
 */
AutoPtr<ColorPickerAdvancedComponent> ColorPickerAdvanced::CreateAndAddNewGradient(
    /* in */ Int32 textResourceId,
    /* in */ Int32 seekBarMax,
    /* in */ IOnSeekBarChangeListener* seekBarListener)
{
	AutoPtr<IContext> content;
	GetContext((IContext**)&content);
	AutoPtr<ILayoutInflater> inflater;
	content->GetSystemService(IContext::LAYOUT_INFLATER_SERVICE, (ILayoutInflater**)&inflater);
    AutoPtr<IView> newComponent;
   	inflater->Inflate(R::Layout::color_picker_advanced_component, NULL, (IView**)&newComponent);
    AddView(newComponent);

	AutoPtr<ColorPickerAdvancedComponent> ret =
		new ColorPickerAdvancedComponent(newComponent, textResourceId, seekBarMax, seekBarListener);
    return ret;
}

/**
 * Sets the listener for when the user changes the color.
 *
 * @param onColorChangedListener The object listening for the change in color.
 */
ECode ColorPickerAdvanced::SetListener(
    /* in */ OnColorChangedListener* onColorChangedListener)
{
	VALIDATE_NOT_NULL(onColorChangedListener);
    mOnColorChangedListener = onColorChangedListener;
    return NOERROR;
}

/**
 * @return The color the user has currently chosen.
 */
Int32 ColorPickerAdvanced::GetColor()
{
    return mCurrentColor;
}

/**
 * Sets the color that the user has currently chosen.
 *
 * @param color The currently chosen color.
 */
ECode ColorPickerAdvanced::SetColor(
    /* in */ Int32 color)
{
    mCurrentColor = color;
    AutoPtr<IColor> colorHsv;
    CColor::AcquireSingleton((IColor**)&colorHsv);
    colorHsv->ColorToHSV(mCurrentColor, &mCurrentHsvValues);
    RefreshGradientComponents();
    return NOERROR;
}

/**
 * Callback for when a slider is updated on the advanced view.
 *
 * @param seekBar The color slider that was updated.
 * @param progress The new value of the color slider.
 * @param fromUser Whether it was the user the changed the value, or whether
 *            we were setting it up.
 */
//@Override
ECode ColorPickerAdvanced::OnProgressChanged(
    /* in */ ISeekBar* seekBar,
    /* in */ Int32 progress,
    /* in */ Boolean fromUser)
{
	VALIDATE_NOT_NULL(seekBar);

    if (fromUser) {
        (*mCurrentHsvValues)[0] = mHueDetails->GetValue();
        (*mCurrentHsvValues)[1] = mSaturationDetails->GetValue() / 100.0f;
        (*mCurrentHsvValues)[2] = mValueDetails->GetValue() / 100.0f;

		AutoPtr<IColor> color;
    	CColor::AcquireSingleton((IColor**)&color);
        color->HSVToColor(mCurrentHsvValues, &mCurrentColor);

        UpdateHueGradient();
        UpdateSaturationGradient();
        UpdateValueGradient();

        NotifyColorChanged();
    }
    return NOERROR;
}

//@Override
ECode ColorPickerAdvanced::OnStartTrackingTouch(
    /* in */ ISeekBar* seekBar)
{
	VALIDATE_NOT_NULL(seekBar);
    // Do nothing.
    return NOERROR;
}

//@Override
ECode ColorPickerAdvanced::OnStopTrackingTouch(
    /* in */ ISeekBar* seekBar)
{
	VALIDATE_NOT_NULL(seekBar);
    // Do nothing.
    return NOERROR;
}

/**
 * Initializes all the views and variables in the advanced view.
 */
ECode ColorPickerAdvanced::Init()
{
    SetOrientation(ILinearLayout::VERTICAL);

    mHueDetails = CreateAndAddNewGradient(R::string::color_picker_hue,
            HUE_SEEK_BAR_MAX, this);
    mSaturationDetails = CreateAndAddNewGradient(R::string::color_picker_saturation,
            SATURATION_SEEK_BAR_MAX, this);
    mValueDetails = CreateAndAddNewGradient(R::string::color_picker_value,
            VALUE_SEEK_BAR_MAX, this);
    RefreshGradientComponents();
    return NOERROR;
}

/**
 * Notifies the listener, if there is one, of a change in the selected color.
 */
ECode ColorPickerAdvanced::NotifyColorChanged()
{
    if (NULL != mOnColorChangedListener) {
        mOnColorChangedListener->OnColorChanged(GetColor());
    }
    return NOERROR;
}

/**
 * Updates only the hue gradient display with the hue value for the
 * currently selected color.
 */
ECode ColorPickerAdvanced::UpdateHueGradient()
{
    AutoPtr< ArrayOf<Float> > tempHsvValues = ArrayOf<Float>::Alloc(3);
    (*tempHsvValues)[1] = (*mCurrentHsvValues)[1];
    (*tempHsvValues)[2] = (*mCurrentHsvValues)[2];

	AutoPtr< ArrayOf<Int32> > newColors = ArrayOf<Int32>::Alloc(HUE_COLOR_COUNT);
	AutoPtr<IColor> color;
    CColor::AcquireSingleton((IColor**)&color);
    Int32 colorTemp = 0;
    for (Int32 i = 0; i < HUE_COLOR_COUNT; ++i) {
        (*tempHsvValues)[0] = i * 60.0f;
        color->HSVToColor(tempHsvValues, &colorTemp);
        (*newColors)[i] = colorTemp;
    }
    mHueDetails->SetGradientColors(newColors);
    return NOERROR;
}

/**
 * Updates only the saturation gradient display with the saturation value
 * for the currently selected color.
 */
ECode ColorPickerAdvanced::UpdateSaturationGradient()
{
    AutoPtr< ArrayOf<Float> > tempHsvValues = ArrayOf<Float>::Alloc(3);
    (*tempHsvValues)[0] = (*mCurrentHsvValues)[0];
    (*tempHsvValues)[1] = 0.0f;
    (*tempHsvValues)[2] = (*mCurrentHsvValues)[2];

	AutoPtr< ArrayOf<Int32> > newColors = ArrayOf<Int32>::Alloc(SATURATION_COLOR_COUNT);
	Int32 hsvColor = 0;
	AutoPtr<IColor> color;
    CColor::AcquireSingleton((IColor**)&color);
	color->HSVToColor(tempHsvValues, &hsvColor);
    (*newColors)[0] = hsvColor;

    (*tempHsvValues)[1] = 1.0f;
    color->HSVToColor(tempHsvValues, &hsvColor);
    (*newColors)[1] = hsvColor;
    mSaturationDetails->SetGradientColors(newColors);
    return NOERROR;
}

/**
 * Updates only the Value gradient display with the Value amount for
 * the currently selected color.
 */
ECode ColorPickerAdvanced::UpdateValueGradient()
{
    AutoPtr< ArrayOf<Float> > tempHsvValues = ArrayOf<Float>::Alloc(3);
    (*tempHsvValues)[0] = (*mCurrentHsvValues)[0];
    (*tempHsvValues)[1] = (*mCurrentHsvValues)[1];
    (*tempHsvValues)[2] = 0.0f;

	AutoPtr< ArrayOf<Int32> > newColors = ArrayOf<Int32>::Alloc(VALUE_COLOR_COUNT);
	Int32 hsvColor = 0;
	AutoPtr<IColor> color;
    CColor::AcquireSingleton((IColor**)&color);
    color->HSVToColor(tempHsvValues, &hsvColor);
    (*newColors)[0] = hsvColor;

    (*tempHsvValues)[2] = 1.0f;
    color->HSVToColor(tempHsvValues, &hsvColor);
    (*newColors)[1] = hsvColor;
    mValueDetails->SetGradientColors(newColors);
    return NOERROR;
}

/**
 * Updates all the gradient displays to show the currently selected color.
 */
ECode ColorPickerAdvanced::RefreshGradientComponents()
{
    // Round and bound the saturation value.
    Int32 saturationValue = Math::Round((*mCurrentHsvValues)[1] * 100.0f);
    saturationValue = Math::Min(saturationValue, SATURATION_SEEK_BAR_MAX);
    saturationValue = Math::Max(saturationValue, 0);

    // Round and bound the Value amount.
    Int32 valueValue = Math::Round((*mCurrentHsvValues)[2] * 100.0f);
    valueValue = Math::Min(valueValue, VALUE_SEEK_BAR_MAX);
    valueValue = Math::Max(valueValue, 0);

    // Don't need to round the hue value since its possible values match the seek bar
    // range directly.
    mHueDetails->SetValue((*mCurrentHsvValues)[0]);
    mSaturationDetails->SetValue(saturationValue);
    mValueDetails->SetValue(valueValue);

    UpdateHueGradient();
    UpdateSaturationGradient();
    UpdateValueGradient();
    return NOERROR;
}

} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
