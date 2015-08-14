
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {

const AutoPtr< ArrayOf<Int32> > ColorPickerSimple::DEFAULT_COLORS = ColorPickerSimple::InitDefaultColors();
const AutoPtr< ArrayOf<Int32> > ColorPickerSimple::DEFAULT_COLOR_LABEL_IDS = ColorPickerSimple::InitDefaultColorLabelIds();

ColorPickerSimple::ColorPickerSimple(
    /* in */ IContext* context)
    : ListView(context)
    , mOnColorChangedListener(NULL)
{
}

ColorPickerSimple::ColorPickerSimple(
    /* in */ IContext* context,
    /* in */ IAttributeSet* attrs)
    : ListView(context, attrs)
    , mOnColorChangedListener(NULL)
{
}

ColorPickerSimple::ColorPickerSimple(
    /* in */ IContext* context,
    /* in */ IAttributeSet* attrs,
    /* in */ Int32 defStyle)
    : ListView(context, attrs, defStyle)
    , mOnColorChangedListener(NULL)
{
}

/**
 * Initializes the listener and sets the adapter for the given list of suggestions. If the
 * suggestions is null a default set of colors will be used.
 *
 * @param suggestions The list of suggestions that should be displayed.
 * @param onColorChangedListener The listener that gets notified when the user touches
 *                               a color.
 */
ECode ColorPickerSimple::Init(
    /* in */ ArrayOf< AutoPtr<ColorSuggestion> >* suggestions,
    /* in */ OnColorChangedListener* onColorChangedListener)
{
    VALIDATE_NOT_NULL(suggestions);
    VALIDATE_NOT_NULL(onColorChangedListener);

    mOnColorChangedListener = onColorChangedListener;
    AutoPtr<IContext> context;
    GetContext(&context);

    if (NULL == suggestions) {
        suggestions = ArrayOf< AutoPtr<ColorSuggestion> >::Alloc(sizeof(DEFAULT_COLORS) / sizeof(DEFAULT_COLORS[0]));
        String idsTemp;
        for (Int32 i = 0; i < suggestions->GetLength(); ++i) {
            context->GetString(DEFAULT_COLOR_LABEL_IDS[i], &idsTemp);
            (*suggestions)[i] = new ColorSuggestion(DEFAULT_COLORS[i], idsTemp);
        }
    }

    AutoPtr<ColorSuggestionListAdapter> adapter = new ColorSuggestionListAdapter(context, suggestions);
    adapter->SetOnColorSuggestionClickListener(this);
    SetAdapter(adapter);
    return NOERROR;
}

//@Override
ECode ColorPickerSimple::OnColorSuggestionClick(
    /* in */ ColorSuggestion* suggestion)
{
    VALIDATE_NOT_NULL(suggestion);
    mOnColorChangedListener->OnColorChanged(suggestion->mColor);
    return NOERROR;
}

AutoPtr< ArrayOf<Int32> > ColorPickerSimple::InitDefaultColors()
{
    Int32 tmps[] = {
        IColor::RED,
        IColor::CYAN,
        IColor::BLUE,
        IColor::GREEN,
        IColor::MAGENTA,
        IColor::YELLOW,
        IColor::BLACK,
        IColor::WHITE
    };

    Int32 count = sizeof(tmps) / sizeof(tmps[0]);
    AutoPtr< ArrayOf<Int32> > result = ArrayOf<Int32>::Alloc(count);
    for (Int32 i=0; i<count; ++i) {
        result->Set(i, tmps[i]);
    }

    return result;
}

AutoPtr< ArrayOf<Int32> > ColorPickerSimple::InitDefaultColorLabelIds()
{
    Int32 tmps[] = {
        R::string::color_picker_button_red,
        R::string::color_picker_button_cyan,
        R::string::color_picker_button_blue,
        R::string::color_picker_button_green,
        R::string::color_picker_button_magenta,
        R::string::color_picker_button_yellow,
        R::string::color_picker_button_black,
        R::string::color_picker_button_white
    };

    Int32 count = sizeof(tmps) / sizeof(tmps[0]);
    AutoPtr< ArrayOf<Int32> > result = ArrayOf<Int32>::Alloc(count);
    for (Int32 i=0; i<count; ++i) {
        result->Set(i, tmps[i]);
    }

    return result;
}

} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

