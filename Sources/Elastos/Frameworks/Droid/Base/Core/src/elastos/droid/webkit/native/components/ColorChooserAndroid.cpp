// wuweizuo automatic build .cpp file from .java file.

#include "ColorChooserAndroid.h"

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Components {

//=====================================================================
//           ColorChooserAndroid::InnerOnColorChangedListener
//=====================================================================
ColorChooserAndroid::InnerOnColorChangedListener::InnerOnColorChangedListener(
    /* [in] */ ColorChooserAndroid* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode ColorChooserAndroid::InnerOnColorChangedListener::OnColorChanged(
    /* [in] */ Int32 color)
{
    // ==================before translated======================
    // mDialog.dismiss();
    // nativeOnColorChosen(mNativeColorChooserAndroid, color);
    assert(0);
    return NOERROR;
}

//=====================================================================
//                         ColorChooserAndroid
//=====================================================================
ECode ColorChooserAndroid::CloseColorChooser()
{
    // ==================before translated======================
    // mDialog.dismiss();
    assert(0);
    return NOERROR;
}

AutoPtr<ColorChooserAndroid> ColorChooserAndroid::CreateColorChooserAndroid(
    /* [in] */ Int64 nativeColorChooserAndroid,
    /* [in] */ ContentViewCore* contentViewCore,
    /* [in] */ Int32 initialColor,
    /* [in] */ ArrayOf<ColorSuggestion>* suggestions)
{
    // ==================before translated======================
    // ColorChooserAndroid chooser = new ColorChooserAndroid(nativeColorChooserAndroid,
    //     contentViewCore.getContext(), initialColor, suggestions);
    // chooser.openColorChooser();
    // return chooser;
    assert(0);
    AutoPtr<ColorChooserAndroid> empty;
    return empty;
}

ColorChooserAndroid::ColorChooserAndroid(
    /* [in] */ Int64 nativeColorChooserAndroid,
    /* [in] */ IContext* context,
    /* [in] */ Int32 initialColor,
    /* [in] */ ArrayOf<ColorSuggestion>* suggestions)
{
    // ==================before translated======================
    // OnColorChangedListener listener = new OnColorChangedListener() {
    //   @Override
    //   public void onColorChanged(int color) {
    //       mDialog.dismiss();
    //       nativeOnColorChosen(mNativeColorChooserAndroid, color);
    //   }
    // };
    //
    // mNativeColorChooserAndroid = nativeColorChooserAndroid;
    // mDialog = new ColorPickerDialog(context, listener, initialColor, suggestions);
}

ECode ColorChooserAndroid::OpenColorChooser()
{
    // ==================before translated======================
    // mDialog.show();
    assert(0);
    return NOERROR;
}

AutoPtr< ArrayOf<ColorSuggestion> > ColorChooserAndroid::CreateColorSuggestionArray(
    /* [in] */ Int32 size)
{
    // ==================before translated======================
    // return new ColorSuggestion[size];
    assert(0);
    AutoPtr< ArrayOf<ColorSuggestion> > empty;
    return empty;
}

ECode ColorChooserAndroid::AddToColorSuggestionArray(
    /* [in] */ ArrayOf<ColorSuggestion>* array,
    /* [in] */ Int32 index,
    /* [in] */ Int32 color,
    /* [in] */ String label)
{
    VALIDATE_NOT_NULL(array);
    // ==================before translated======================
    // array[index] = new ColorSuggestion(color, label);
    assert(0);
    return NOERROR;
}

ECode ColorChooserAndroid::NativeOnColorChosen(
    /* [in] */ Int64 nativeColorChooserAndroid,
    /* [in] */ Int32 color)
{
    assert(0);
    return NOERROR;
}

} // namespace Components
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


