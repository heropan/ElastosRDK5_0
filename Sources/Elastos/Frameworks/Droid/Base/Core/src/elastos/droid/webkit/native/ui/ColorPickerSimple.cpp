// wuweizuo automatic build .cpp file from .java file.

#include "ColorPickerSimple.h"

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {

//=====================================================================
//                          ColorPickerSimple
//=====================================================================
AutoPtr< ArrayOf<Int32> > ColorPickerSimple::DEFAULT_COLORS = ColorPickerSimple::MiddleInitDefaultColors();
AutoPtr< ArrayOf<Int32> > ColorPickerSimple::DEFAULT_COLOR_LABEL_IDS = ColorPickerSimple::MiddleInitDefaultColorLabelIds();

ColorPickerSimple::ColorPickerSimple(
    /* [in] */ IContext* context)
{
    // ==================before translated======================
    // super(context);
}

ColorPickerSimple::ColorPickerSimple(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    // ==================before translated======================
    // super(context, attrs);
}

ColorPickerSimple::ColorPickerSimple(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    // ==================before translated======================
    // super(context, attrs, defStyle);
}

ECode ColorPickerSimple::Init(
    /* [in] */ ArrayOf<ColorSuggestion>* suggestions,
    /* [in] */ OnColorChangedListener* onColorChangedListener)
{
    VALIDATE_NOT_NULL(suggestions);
    VALIDATE_NOT_NULL(onColorChangedListener);
    // ==================before translated======================
    // mOnColorChangedListener = onColorChangedListener;
    //
    // if (suggestions == null) {
    //     suggestions = new ColorSuggestion[DEFAULT_COLORS.length];
    //     for (int i = 0; i < suggestions.length; ++i) {
    //         suggestions[i] = new ColorSuggestion(DEFAULT_COLORS[i],
    //                 getContext().getString(DEFAULT_COLOR_LABEL_IDS[i]));
    //     }
    // }
    //
    // ColorSuggestionListAdapter adapter = new ColorSuggestionListAdapter(
    //         getContext(), suggestions);
    // adapter.setOnColorSuggestionClickListener(this);
    // setAdapter(adapter);
    assert(0);
    return NOERROR;
}

ECode ColorPickerSimple::OnColorSuggestionClick(
    /* [in] */ ColorSuggestion* suggestion)
{
    VALIDATE_NOT_NULL(suggestion);
    // ==================before translated======================
    // mOnColorChangedListener.onColorChanged(suggestion.mColor);
    assert(0);
    return NOERROR;
}

AutoPtr< ArrayOf<Int32> > ColorPickerSimple::MiddleInitDefaultColors()
{
    // ==================before translated======================
    // ->WWZ_SIGN: ARRAY_INIT_START {
    // Color.RED,
    //          Color.CYAN,
    //          Color.BLUE,
    //          Color.GREEN,
    //          Color.MAGENTA,
    //          Color.YELLOW,
    //          Color.BLACK,
    //          Color.WHITE
    // ->WWZ_SIGN: ARRAY_INIT_END }
    assert(0);
    AutoPtr< ArrayOf<Int32> > empty;
    return empty;
}

AutoPtr< ArrayOf<Int32> > ColorPickerSimple::MiddleInitDefaultColorLabelIds()
{
    // ==================before translated======================
    // ->WWZ_SIGN: ARRAY_INIT_START {
    // R.string.color_picker_button_red,
    //          R.string.color_picker_button_cyan,
    //          R.string.color_picker_button_blue,
    //          R.string.color_picker_button_green,
    //          R.string.color_picker_button_magenta,
    //          R.string.color_picker_button_yellow,
    //          R.string.color_picker_button_black,
    //          R.string.color_picker_button_white
    // ->WWZ_SIGN: ARRAY_INIT_END }
    assert(0);
    AutoPtr< ArrayOf<Int32> > empty;
    return empty;
}

} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


