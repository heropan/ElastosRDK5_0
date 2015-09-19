// wuweizuo automatic build .cpp file from .java file.

#include "ColorPickerAdvancedComponent.h"

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {

//=====================================================================
//                     ColorPickerAdvancedComponent
//=====================================================================
ColorPickerAdvancedComponent::ColorPickerAdvancedComponent(
    /* [in] */ const IView* rootView,
    /* [in] */ const Int32& textResourceId,
    /* [in] */ const Int32& seekBarMax,
    /* [in] */ const IOnSeekBarChangeListener* seekBarListener)
{
    // ==================before translated======================
    // mGradientView = rootView.findViewById(R.id.gradient);
    // mText = (TextView) rootView.findViewById(R.id.text);
    // mText.setText(textResourceId);
    // mGradientDrawable = new GradientDrawable(GradientDrawable.Orientation.LEFT_RIGHT, null);
    // mSeekBar = (SeekBar) rootView.findViewById(R.id.seek_bar);
    // mSeekBar.setOnSeekBarChangeListener(seekBarListener);
    // mSeekBar.setMax(seekBarMax);
    // // Setting the thumb offset means the seek bar thumb can move all the way to each end
    // // of the gradient view.
    // Context context = rootView.getContext();
    // int offset = context.getResources()
    //                     .getDrawable(R.drawable.color_picker_advanced_select_handle)
    //                     .getIntrinsicWidth();
    // mSeekBar.setThumbOffset(offset / 2);
}

Float ColorPickerAdvancedComponent::GetValue()
{
    // ==================before translated======================
    // return mSeekBar.getProgress();
    assert(0);
    return 0.0f;
}

ECode ColorPickerAdvancedComponent::SetValue(
    /* [in] */ Float newValue)
{
    // ==================before translated======================
    // mSeekBar.setProgress((int) newValue);
    assert(0);
    return NOERROR;
}

ECode ColorPickerAdvancedComponent::SetGradientColors(
    /* [in] */ ArrayOf<Int32>* newColors)
{
    VALIDATE_NOT_NULL(newColors);
    // ==================before translated======================
    // mGradientColors = newColors.clone();
    // if (Build.VERSION.SDK_INT < Build.VERSION_CODES.JELLY_BEAN) {
    //     Orientation currentOrientation = Orientation.LEFT_RIGHT;
    //     mGradientDrawable = new GradientDrawable(currentOrientation, mGradientColors);
    // } else {
    //     mGradientDrawable.setColors(mGradientColors);
    // }
    // ApiCompatibilityUtils.setBackgroundForView(mGradientView, mGradientDrawable);
    assert(0);
    return NOERROR;
}

} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


