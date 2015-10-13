
#include "elastos/droid/webkit/native/components/ColorChooserAndroid.h"

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

    assert(NULL == mOwner);
    assert(NULL == mOwner->mDialog);
    //mOwner->mDialog->Dismiss();
    mOwner->NativeOnColorChosen(mOwner->mNativeColorChooserAndroid, color);
    return NOERROR;
}

//=====================================================================
//                         ColorChooserAndroid
//=====================================================================
ECode ColorChooserAndroid::CloseColorChooser()
{
    // ==================before translated======================
    // mDialog.dismiss();

    //mDialog->Dismiss();
    return NOERROR;
}

AutoPtr<ColorChooserAndroid> ColorChooserAndroid::CreateColorChooserAndroid(
    /* [in] */ Int64 nativeColorChooserAndroid,
    /* [in] */ ContentViewCore* contentViewCore,
    /* [in] */ Int32 initialColor,
    /* [in] */ ArrayOf<ColorSuggestion*>* suggestions)
{
    // ==================before translated======================
    // ColorChooserAndroid chooser = new ColorChooserAndroid(nativeColorChooserAndroid,
    //     contentViewCore.getContext(), initialColor, suggestions);
    // chooser.openColorChooser();
    // return chooser;

    AutoPtr<IContext> context = contentViewCore->GetContext();
    AutoPtr<ColorChooserAndroid> chooser = new ColorChooserAndroid(nativeColorChooserAndroid,
         context, initialColor, suggestions);
    return chooser;
}

ColorChooserAndroid::ColorChooserAndroid(
    /* [in] */ Int64 nativeColorChooserAndroid,
    /* [in] */ IContext* context,
    /* [in] */ Int32 initialColor,
    /* [in] */ ArrayOf<ColorSuggestion*>* suggestions)
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

    AutoPtr<OnColorChangedListener> listener = new InnerOnColorChangedListener(this);
    mNativeColorChooserAndroid = nativeColorChooserAndroid;
    mDialog = new ColorPickerDialog(context, listener, initialColor, suggestions);
}

ECode ColorChooserAndroid::OpenColorChooser()
{
    // ==================before translated======================
    // mDialog.show();

    //mDialog->Show();
    return NOERROR;
}

AutoPtr< ArrayOf<ColorSuggestion*> > ColorChooserAndroid::CreateColorSuggestionArray(
    /* [in] */ Int32 size)
{
    // ==================before translated======================
    // return new ColorSuggestion[size];

    AutoPtr< ArrayOf<ColorSuggestion*> > result = ArrayOf<ColorSuggestion*>::Alloc(size);
    return result;
}

ECode ColorChooserAndroid::AddToColorSuggestionArray(
    /* [in] */ ArrayOf<ColorSuggestion*>* array,
    /* [in] */ Int32 index,
    /* [in] */ Int32 color,
    /* [in] */ const String& label)
{
    VALIDATE_NOT_NULL(array);
    // ==================before translated======================
    // array[index] = new ColorSuggestion(color, label);

    AutoPtr<ColorSuggestion> item = new ColorSuggestion(color, label);
    array->Set(index, item);
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

