// wuweizuo automatic build .cpp file from .java file.

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
    /*
    mOwner = owner;
    */
}

ECode ColorChooserAndroid::InnerOnColorChangedListener::OnColorChanged(
    /* [in] */ Int32 color)
{
    /*
    mDialog.dismiss();
    nativeOnColorChosen(mNativeColorChooserAndroid, color);
    */
    return NOERROR;
}

//=====================================================================
//                         ColorChooserAndroid
//=====================================================================
ECode ColorChooserAndroid::CloseColorChooser()
{
    /*
    mDialog.dismiss();
    */
    return NOERROR;
}

AutoPtr<ColorChooserAndroid> ColorChooserAndroid::CreateColorChooserAndroid(
    /* [in] */ Int64 nativeColorChooserAndroid,
    /* [in] */ ContentViewCore* contentViewCore,
    /* [in] */ Int32 initialColor,
    /* [in] */ ArrayOf<ColorSuggestion>* suggestions)
{
    /*
    ColorChooserAndroid chooser = new ColorChooserAndroid(nativeColorChooserAndroid,
        contentViewCore.getContext(), initialColor, suggestions);
    chooser.openColorChooser();
    return chooser;
    */
    return AutoPtr<ColorChooserAndroid>(this);
}

ColorChooserAndroid::ColorChooserAndroid(
    /* [in] */ Int64 nativeColorChooserAndroid,
    /* [in] */ IContext* context,
    /* [in] */ Int32 initialColor,
    /* [in] */ ArrayOf<ColorSuggestion>* suggestions)
{
    /*
    OnColorChangedListener listener = // [wuweizuo auto add note here: new InnerOnColorChangedListener(this)] new OnColorChangedListener() {
      @Override
      public void onColorChanged(int color) {
          mDialog.dismiss();
          nativeOnColorChosen(mNativeColorChooserAndroid, color);
      }
    };


    mNativeColorChooserAndroid = nativeColorChooserAndroid;
    mDialog = new ColorPickerDialog(context, listener, initialColor, suggestions);
    */
}

ECode ColorChooserAndroid::OpenColorChooser()
{
    /*
    mDialog.show();
    */
    return NOERROR;
}

AutoPtr< ArrayOf<ColorSuggestion> > ColorChooserAndroid::CreateColorSuggestionArray(
    /* [in] */ Int32 size)
{
    /*
    return new ColorSuggestion[size];
    */
    return AutoPtr< ArrayOf<ColorSuggestion> >(this);
}

ECode ColorChooserAndroid::AddToColorSuggestionArray(
    /* [in] */ ArrayOf<ColorSuggestion>* array,
    /* [in] */ Int32 index,
    /* [in] */ Int32 color,
    /* [in] */ String label)
{
    VALIDATE_NOT_NULL(array);
    /*
    array[index] = new ColorSuggestion(color, label);
    */
    return NOERROR;
}

ECode ColorChooserAndroid::NativeOnColorChosen(
    /* [in] */ Int64 nativeColorChooserAndroid,
    /* [in] */ Int32 color)
{
    return NOERROR;
}

} // namespace Components
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


