// wuweizuo automatic build .cpp file from .java file.

#include "ColorPickerDialog.h"

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {

//=====================================================================
//            ColorPickerDialog::InnerDialogOnClickListener
//=====================================================================
ColorPickerDialog::InnerDialogOnClickListener::InnerDialogOnClickListener(
    /* [in] */ ColorPickerDialog* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode ColorPickerDialog::InnerDialogOnClickListener::OnClick(
    /* [in] */ IDialogInterface* dialogInterface,
    /* [in] */ Int32 i)
{
    VALIDATE_NOT_NULL(dialogInterface);
    // ==================before translated======================
    // tryNotifyColorSet(mCurrentColor);
    assert(0);
    return NOERROR;
}

//=====================================================================
//            ColorPickerDialog::InnerDialogOnClickListener
//=====================================================================
ColorPickerDialog::InnerDialogOnClickListener::InnerDialogOnClickListener(
    /* [in] */ ColorPickerDialog* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode ColorPickerDialog::InnerDialogOnClickListener::OnClick(
    /* [in] */ IDialogInterface* dialogInterface,
    /* [in] */ Int32 i)
{
    VALIDATE_NOT_NULL(dialogInterface);
    // ==================before translated======================
    // tryNotifyColorSet(mInitialColor);
    assert(0);
    return NOERROR;
}

//=====================================================================
//       ColorPickerDialog::InnerDialogInterfaceOnCancelListener
//=====================================================================
ColorPickerDialog::InnerDialogInterfaceOnCancelListener::InnerDialogInterfaceOnCancelListener(
    /* [in] */ ColorPickerDialog* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode ColorPickerDialog::InnerDialogInterfaceOnCancelListener::OnCancel(
    /* [in] */ IDialogInterface* arg0)
{
    VALIDATE_NOT_NULL(arg0);
    // ==================before translated======================
    // tryNotifyColorSet(mInitialColor);
    assert(0);
    return NOERROR;
}

//=====================================================================
//            ColorPickerDialog::InnerButtonOnClickListener
//=====================================================================
ColorPickerDialog::InnerButtonOnClickListener::InnerButtonOnClickListener(
    /* [in] */ ColorPickerDialog* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode ColorPickerDialog::InnerButtonOnClickListener::OnClick(
    /* [in] */ IView* v)
{
    VALIDATE_NOT_NULL(v);
    // ==================before translated======================
    // showAdvancedView();
    assert(0);
    return NOERROR;
}

//=====================================================================
//                          ColorPickerDialog
//=====================================================================
ColorPickerDialog::ColorPickerDialog(
    /* [in] */ IContext* context,
    /* [in] */ OnColorChangedListener* listener,
    /* [in] */ Int32 color,
    /* [in] */ ArrayOf<ColorSuggestion>* suggestions)
{
    // ==================before translated======================
    // super(context, 0);
    //
    // mListener = listener;
    // mInitialColor = color;
    // mCurrentColor = mInitialColor;
    //
    // // Initialize title
    // LayoutInflater inflater = (LayoutInflater) context
    //         .getSystemService(Context.LAYOUT_INFLATER_SERVICE);
    // View title = inflater.inflate(R.layout.color_picker_dialog_title, null);
    // setCustomTitle(title);
    //
    // mCurrentColorView = title.findViewById(R.id.selected_color_view);
    //
    // TextView titleText = (TextView) title.findViewById(R.id.title);
    // titleText.setText(R.string.color_picker_dialog_title);
    //
    // // Initialize Set/Cancel buttons
    // String positiveButtonText = context.getString(R.string.color_picker_button_set);
    // setButton(BUTTON_POSITIVE, positiveButtonText,
    //         new Dialog.OnClickListener() {
    //             @Override
    //             public void onClick(DialogInterface dialogInterface, int i) {
    //                 tryNotifyColorSet(mCurrentColor);
    //             }
    //         });
    //
    // // Note that with the color picker there's not really any such thing as
    // // "cancelled".
    // // The color picker flow only finishes when we return a color, so we
    // // have to always
    // // return something. The concept of "cancelled" in this case just means
    // // returning
    // // the color that we were initialized with.
    // String negativeButtonText = context.getString(R.string.color_picker_button_cancel);
    // setButton(BUTTON_NEGATIVE, negativeButtonText,
    //         new Dialog.OnClickListener() {
    //             @Override
    //             public void onClick(DialogInterface dialogInterface, int i) {
    //                 tryNotifyColorSet(mInitialColor);
    //             }
    //         });
    //
    // setOnCancelListener(new DialogInterface.OnCancelListener() {
    //     @Override
    //     public void onCancel(DialogInterface arg0) {
    //         tryNotifyColorSet(mInitialColor);
    //     }
    // });
    //
    // // Initialize main content view
    // View content = inflater.inflate(R.layout.color_picker_dialog_content, null);
    // setView(content);
    //
    // // Initialize More button.
    // mMoreButton = (Button) content.findViewById(R.id.more_colors_button);
    // mMoreButton.setOnClickListener(new Button.OnClickListener() {
    //     @Override
    //     public void onClick(View v) {
    //         showAdvancedView();
    //     }
    // });
    //
    // // Initialize advanced color view (hidden initially).
    // mAdvancedColorPicker =
    //         (ColorPickerAdvanced) content.findViewById(R.id.color_picker_advanced);
    // mAdvancedColorPicker.setVisibility(View.GONE);
    //
    // // Initialize simple color view (default view).
    // mSimpleColorPicker = (ColorPickerSimple) content.findViewById(R.id.color_picker_simple);
    // mSimpleColorPicker.init(suggestions, this);
    //
    // updateCurrentColor(mInitialColor);
}

ECode ColorPickerDialog::OnColorChanged(
    /* [in] */ Int32 color)
{
    // ==================before translated======================
    // updateCurrentColor(color);
    assert(0);
    return NOERROR;
}

ECode ColorPickerDialog::ShowAdvancedView()
{
    // ==================before translated======================
    // // Only need to hide the borders, not the Views themselves, since the Views are
    // // contained within the borders.
    // View buttonBorder = findViewById(R.id.more_colors_button_border);
    // buttonBorder.setVisibility(View.GONE);
    //
    // View simpleView = findViewById(R.id.color_picker_simple);
    // simpleView.setVisibility(View.GONE);
    //
    // mAdvancedColorPicker.setVisibility(View.VISIBLE);
    // mAdvancedColorPicker.setListener(this);
    // mAdvancedColorPicker.setColor(mCurrentColor);
    assert(0);
    return NOERROR;
}

ECode ColorPickerDialog::TryNotifyColorSet(
    /* [in] */ Int32 color)
{
    // ==================before translated======================
    // if (mListener != null) mListener.onColorChanged(color);
    assert(0);
    return NOERROR;
}

ECode ColorPickerDialog::UpdateCurrentColor(
    /* [in] */ Int32 color)
{
    // ==================before translated======================
    // mCurrentColor = color;
    // if (mCurrentColorView != null) mCurrentColorView.setBackgroundColor(color);
    assert(0);
    return NOERROR;
}

} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


