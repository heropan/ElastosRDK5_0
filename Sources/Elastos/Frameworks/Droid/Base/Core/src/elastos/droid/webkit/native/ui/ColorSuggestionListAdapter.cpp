// wuweizuo automatic build .cpp file from .java file.

#include "ColorSuggestionListAdapter.h"

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {

//=====================================================================
//                      ColorSuggestionListAdapter
//=====================================================================
const Int32 ColorSuggestionListAdapter::COLORS_PER_ROW;

ColorSuggestionListAdapter::ColorSuggestionListAdapter(
    /* [in] */ IContext* context,
    /* [in] */ ArrayOf<ColorSuggestion>* suggestions)
{
    // ==================before translated======================
    // mContext = context;
    // mSuggestions = suggestions;
}

ECode ColorSuggestionListAdapter::SetOnColorSuggestionClickListener(
    /* [in] */ OnColorSuggestionClickListener* listener)
{
    VALIDATE_NOT_NULL(listener);
    // ==================before translated======================
    // mListener = listener;
    assert(0);
    return NOERROR;
}

ECode ColorSuggestionListAdapter::OnClick(
    /* [in] */ IView* v)
{
    VALIDATE_NOT_NULL(v);
    // ==================before translated======================
    // if (mListener == null) {
    //     return;
    // }
    // ColorSuggestion suggestion = (ColorSuggestion) v.getTag();
    // if (suggestion == null) {
    //     return;
    // }
    // mListener.onColorSuggestionClick(suggestion);
    assert(0);
    return NOERROR;
}

AutoPtr<IView> ColorSuggestionListAdapter::GetView(
    /* [in] */ Int32 position,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent)
{
    // ==================before translated======================
    // LinearLayout layout;
    // if (convertView != null && convertView instanceof LinearLayout) {
    //     layout = (LinearLayout) convertView;
    // } else {
    //     layout = new LinearLayout(mContext);
    //     layout.setLayoutParams(new AbsListView.LayoutParams(
    //             AbsListView.LayoutParams.MATCH_PARENT,
    //             AbsListView.LayoutParams.WRAP_CONTENT));
    //     layout.setOrientation(LinearLayout.HORIZONTAL);
    //     layout.setBackgroundColor(Color.WHITE);
    //     int buttonHeight =
    //         mContext.getResources().getDimensionPixelOffset(R.dimen.color_button_height);
    //     for (int i = 0; i < COLORS_PER_ROW; ++i) {
    //         View button = new View(mContext);
    //         LinearLayout.LayoutParams layoutParams =
    //                 new LinearLayout.LayoutParams(0, buttonHeight, 1f);
    //         ApiCompatibilityUtils.setMarginStart(layoutParams, -1);
    //         if (i == COLORS_PER_ROW - 1) {
    //             ApiCompatibilityUtils.setMarginEnd(layoutParams, -1);
    //         }
    //         button.setLayoutParams(layoutParams);
    //         button.setBackgroundResource(R.drawable.color_button_background);
    //         layout.addView(button);
    //     }
    // }
    // for (int i = 0; i < COLORS_PER_ROW; ++i) {
    //     setUpColorButton(layout.getChildAt(i), position * COLORS_PER_ROW + i);
    // }
    // return layout;
    assert(0);
    AutoPtr<IView> empty;
    return empty;
}

Int64 ColorSuggestionListAdapter::GetItemId(
    /* [in] */ Int32 position)
{
    // ==================before translated======================
    // return position;
    assert(0);
    return 0;
}

AutoPtr<Object> ColorSuggestionListAdapter::GetItem(
    /* [in] */ Int32 position)
{
    // ==================before translated======================
    // return null;
    assert(0);
    AutoPtr<Object> empty;
    return empty;
}

Int32 ColorSuggestionListAdapter::GetCount()
{
    // ==================before translated======================
    // return (mSuggestions.length + COLORS_PER_ROW - 1) / COLORS_PER_ROW;
    assert(0);
    return 0;
}

ECode ColorSuggestionListAdapter::SetUpColorButton(
    /* [in] */ IView* button,
    /* [in] */ Int32 index)
{
    VALIDATE_NOT_NULL(button);
    // ==================before translated======================
    // if (index >= mSuggestions.length) {
    //     button.setTag(null);
    //     button.setContentDescription(null);
    //     button.setVisibility(View.INVISIBLE);
    //     return;
    // }
    // button.setTag(mSuggestions[index]);
    // button.setVisibility(View.VISIBLE);
    // ColorSuggestion suggestion = mSuggestions[index];
    // LayerDrawable layers = (LayerDrawable) button.getBackground();
    // GradientDrawable swatch =
    //         (GradientDrawable) layers.findDrawableByLayerId(R.id.color_button_swatch);
    // swatch.setColor(suggestion.mColor);
    // String description = suggestion.mLabel;
    // if (TextUtils.isEmpty(description)) {
    //     description = String.format("#%06X", (0xFFFFFF & suggestion.mColor));
    // }
    // button.setContentDescription(description);
    // button.setOnClickListener(this);
    assert(0);
    return NOERROR;
}

} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


