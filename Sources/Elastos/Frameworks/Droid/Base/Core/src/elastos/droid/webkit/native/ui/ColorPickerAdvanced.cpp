
#include "elastos/droid/webkit/native/ui/ColorPickerAdvanced.h"

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {

//=====================================================================
//                         ColorPickerAdvanced
//=====================================================================
const Int32 ColorPickerAdvanced::HUE_SEEK_BAR_MAX;
const Int32 ColorPickerAdvanced::HUE_COLOR_COUNT;
const Int32 ColorPickerAdvanced::SATURATION_SEEK_BAR_MAX;
const Int32 ColorPickerAdvanced::SATURATION_COLOR_COUNT;
const Int32 ColorPickerAdvanced::VALUE_SEEK_BAR_MAX;
const Int32 ColorPickerAdvanced::VALUE_COLOR_COUNT;

ColorPickerAdvanced::ColorPickerAdvanced(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    // ==================before translated======================
    // super(context, attrs);
    // init();
}

ColorPickerAdvanced::ColorPickerAdvanced(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    // ==================before translated======================
    // super(context, attrs, defStyle);
    // init();
}

ColorPickerAdvanced::ColorPickerAdvanced(
    /* [in] */ IContext* context)
{
    // ==================before translated======================
    // super(context);
    // init();
}

AutoPtr<ColorPickerAdvancedComponent> ColorPickerAdvanced::CreateAndAddNewGradient(
    /* [in] */ Int32 textResourceId,
    /* [in] */ Int32 seekBarMax,
    /* [in] */ ISeekBarOnSeekBarChangeListener* seekBarListener)
{
    // ==================before translated======================
    // LayoutInflater inflater = (LayoutInflater) getContext()
    //         .getSystemService(Context.LAYOUT_INFLATER_SERVICE);
    // View newComponent = inflater.inflate(R.layout.color_picker_advanced_component, null);
    // addView(newComponent);
    //
    // return new ColorPickerAdvancedComponent(newComponent,
    //         textResourceId,
    //         seekBarMax,
    //         seekBarListener);
    assert(0);
    AutoPtr<ColorPickerAdvancedComponent> empty;
    return empty;
}

ECode ColorPickerAdvanced::SetListener(
    /* [in] */ OnColorChangedListener* onColorChangedListener)
{
    VALIDATE_NOT_NULL(onColorChangedListener);
    // ==================before translated======================
    // mOnColorChangedListener = onColorChangedListener;
    assert(0);
    return NOERROR;
}

Int32 ColorPickerAdvanced::GetColor()
{
    // ==================before translated======================
    // return mCurrentColor;
    assert(0);
    return 0;
}

ECode ColorPickerAdvanced::SetColor(
    /* [in] */ Int32 color)
{
    // ==================before translated======================
    // mCurrentColor = color;
    // Color.colorToHSV(mCurrentColor, mCurrentHsvValues);
    // refreshGradientComponents();
    assert(0);
    return NOERROR;
}

ECode ColorPickerAdvanced::OnProgressChanged(
    /* [in] */ ISeekBar* seekBar,
    /* [in] */ Int32 progress,
    /* [in] */ Boolean fromUser)
{
    VALIDATE_NOT_NULL(seekBar);
    // ==================before translated======================
    // if (fromUser) {
    //     mCurrentHsvValues[0] = mHueDetails.getValue();
    //     mCurrentHsvValues[1] = mSaturationDetails.getValue() / 100.0f;
    //     mCurrentHsvValues[2] = mValueDetails.getValue() / 100.0f;
    //
    //     mCurrentColor = Color.HSVToColor(mCurrentHsvValues);
    //
    //     updateHueGradient();
    //     updateSaturationGradient();
    //     updateValueGradient();
    //
    //     notifyColorChanged();
    // }
    assert(0);
    return NOERROR;
}

ECode ColorPickerAdvanced::OnStartTrackingTouch(
    /* [in] */ ISeekBar* seekBar)
{
    VALIDATE_NOT_NULL(seekBar);
    // ==================before translated======================
    // // Do nothing.
    assert(0);
    return NOERROR;
}

ECode ColorPickerAdvanced::OnStopTrackingTouch(
    /* [in] */ ISeekBar* seekBar)
{
    VALIDATE_NOT_NULL(seekBar);
    // ==================before translated======================
    // // Do nothing.
    assert(0);
    return NOERROR;
}

AutoPtr< ArrayOf<Float> > ColorPickerAdvanced::MiddleInitMcurrenthsvvalues()
{
    // ==================before translated======================
    // float[] result = new float[3];
    assert(0);
    AutoPtr< ArrayOf<Float> > empty;
    return empty;
}

ECode ColorPickerAdvanced::Init()
{
    // ==================before translated======================
    // setOrientation(LinearLayout.VERTICAL);
    //
    // mHueDetails = createAndAddNewGradient(R.string.color_picker_hue,
    //         HUE_SEEK_BAR_MAX, this);
    // mSaturationDetails = createAndAddNewGradient(R.string.color_picker_saturation,
    //         SATURATION_SEEK_BAR_MAX, this);
    // mValueDetails = createAndAddNewGradient(R.string.color_picker_value,
    //         VALUE_SEEK_BAR_MAX, this);
    // refreshGradientComponents();
    assert(0);
    return NOERROR;
}

ECode ColorPickerAdvanced::NotifyColorChanged()
{
    // ==================before translated======================
    // if (mOnColorChangedListener != null) {
    //     mOnColorChangedListener.onColorChanged(getColor());
    // }
    assert(0);
    return NOERROR;
}

ECode ColorPickerAdvanced::UpdateHueGradient()
{
    // ==================before translated======================
    // float[] tempHsvValues = new float[3];
    // tempHsvValues[1] = mCurrentHsvValues[1];
    // tempHsvValues[2] = mCurrentHsvValues[2];
    //
    // int[] newColors = new int[HUE_COLOR_COUNT];
    //
    // for (int i = 0; i < HUE_COLOR_COUNT; ++i) {
    //     tempHsvValues[0] = i * 60.0f;
    //     newColors[i] = Color.HSVToColor(tempHsvValues);
    // }
    // mHueDetails.setGradientColors(newColors);
    assert(0);
    return NOERROR;
}

ECode ColorPickerAdvanced::UpdateSaturationGradient()
{
    // ==================before translated======================
    // float[] tempHsvValues = new float[3];
    // tempHsvValues[0] = mCurrentHsvValues[0];
    // tempHsvValues[1] = 0.0f;
    // tempHsvValues[2] = mCurrentHsvValues[2];
    //
    // int[] newColors = new int[SATURATION_COLOR_COUNT];
    //
    // newColors[0] = Color.HSVToColor(tempHsvValues);
    //
    // tempHsvValues[1] = 1.0f;
    // newColors[1] = Color.HSVToColor(tempHsvValues);
    // mSaturationDetails.setGradientColors(newColors);
    assert(0);
    return NOERROR;
}

ECode ColorPickerAdvanced::UpdateValueGradient()
{
    // ==================before translated======================
    // float[] tempHsvValues = new float[3];
    // tempHsvValues[0] = mCurrentHsvValues[0];
    // tempHsvValues[1] = mCurrentHsvValues[1];
    // tempHsvValues[2] = 0.0f;
    //
    // int[] newColors = new int[VALUE_COLOR_COUNT];
    //
    // newColors[0] = Color.HSVToColor(tempHsvValues);
    //
    // tempHsvValues[2] = 1.0f;
    // newColors[1] = Color.HSVToColor(tempHsvValues);
    // mValueDetails.setGradientColors(newColors);
    assert(0);
    return NOERROR;
}

ECode ColorPickerAdvanced::RefreshGradientComponents()
{
    // ==================before translated======================
    // // Round and bound the saturation value.
    // int saturationValue = Math.round(mCurrentHsvValues[1] * 100.0f);
    // saturationValue = Math.min(saturationValue, SATURATION_SEEK_BAR_MAX);
    // saturationValue = Math.max(saturationValue, 0);
    //
    // // Round and bound the Value amount.
    // int valueValue = Math.round(mCurrentHsvValues[2] * 100.0f);
    //
    // valueValue = Math.min(valueValue, VALUE_SEEK_BAR_MAX);
    // valueValue = Math.max(valueValue, 0);
    //
    // // Don't need to round the hue value since its possible values match the seek bar
    // // range directly.
    // mHueDetails.setValue(mCurrentHsvValues[0]);
    // mSaturationDetails.setValue(saturationValue);
    // mValueDetails.setValue(valueValue);
    //
    // updateHueGradient();
    // updateSaturationGradient();
    // updateValueGradient();
    assert(0);
    return NOERROR;
}

} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


