
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {
namespace Input {

DateTimeSuggestionListAdapter::DateTimeSuggestionListAdapter(
    /* [in] */ IContext* context,
    /* [in] */ List<DateTimeSuggestion> objects)
    : mContext(context)
{
    super(context, R::layout::date_time_suggestion, objects);
}

//@Override
ECode DateTimeSuggestionListAdapter::GetView(
    /* [in] */ Int32 position,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent,
    /* [out] */ IView** outView)
{
    VALIDATE_NOT_NULL(outView);

    View layout = convertView;
    if (convertView == null) {
        LayoutInflater inflater = LayoutInflater.from(mContext);
        layout = inflater.inflate(R.layout.date_time_suggestion, parent, false);
    }
    TextView labelView = (TextView) layout.findViewById(R.id.date_time_suggestion_value);
    TextView sublabelView = (TextView) layout.findViewById(R.id.date_time_suggestion_label);

    if (position == getCount() - 1) {
        labelView.setText(mContext.getText(R.string.date_picker_dialog_other_button_label));
        sublabelView.setText("");
    } else {
        labelView.setText(getItem(position).localizedValue());
        sublabelView.setText(getItem(position).label());
    }

    return layout;
}

//@Override
ECode DateTimeSuggestionListAdapter::GetCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    *count = super.getCount() + 1;
    return NOERROR;
}

} // namespace Input
} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
