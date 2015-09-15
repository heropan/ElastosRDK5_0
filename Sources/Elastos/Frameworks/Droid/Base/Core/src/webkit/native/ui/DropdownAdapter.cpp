// wuweizuo automatic build .cpp file from .java file.

#include "DropdownAdapter.h"

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {

//=====================================================================
//                           DropdownAdapter
//=====================================================================
DropdownAdapter::DropdownAdapter(
    /* [in] */ IContext* context,
    /* [in] */ IList<DropdownItem*>* items,
    /* [in] */ ISet<Integer*>* separators)
{
    // ==================before translated======================
    // super(context, R.layout.dropdown_item, items);
    // mSeparators = separators;
    // mContext = context;
    // mAreAllItemsEnabled = checkAreAllItemsEnabled();
}

DropdownAdapter::DropdownAdapter(
    /* [in] */ IContext* context,
    /* [in] */ ArrayOf<DropdownItem>* items,
    /* [in] */ ISet<Integer*>* separators)
{
    // ==================before translated======================
    // super(context, R.layout.dropdown_item, items);
    // mSeparators = separators;
    // mContext = context;
    // mAreAllItemsEnabled = checkAreAllItemsEnabled();
}

AutoPtr<IView> DropdownAdapter::GetView(
    /* [in] */ Int32 position,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent)
{
    // ==================before translated======================
    // View layout = convertView;
    // if (convertView == null) {
    //     LayoutInflater inflater =
    //             (LayoutInflater) mContext.getSystemService(Context.LAYOUT_INFLATER_SERVICE);
    //     layout = inflater.inflate(R.layout.dropdown_item, null);
    //     ApiCompatibilityUtils.setBackgroundForView(layout, new DropdownDividerDrawable());
    // }
    //
    // DropdownItem item = getItem(position);
    //
    // TextView labelView = (TextView) layout.findViewById(R.id.dropdown_label);
    // labelView.setText(item.getLabel());
    //
    // labelView.setEnabled(item.isEnabled());
    // if (item.isGroupHeader()) {
    //     labelView.setTypeface(null, Typeface.BOLD);
    // } else {
    //     labelView.setTypeface(null, Typeface.NORMAL);
    // }
    //
    // DropdownDividerDrawable divider = (DropdownDividerDrawable) layout.getBackground();
    // int height = mContext.getResources().getDimensionPixelSize(R.dimen.dropdown_item_height);
    // if (position == 0) {
    //     divider.setColor(Color.TRANSPARENT);
    // } else {
    //     int dividerHeight = mContext.getResources().getDimensionPixelSize(
    //             R.dimen.dropdown_item_divider_height);
    //     height += dividerHeight;
    //     divider.setHeight(dividerHeight);
    //     if (mSeparators != null && mSeparators.contains(position)) {
    //         divider.setColor(mContext.getResources().getColor(
    //                          R.color.dropdown_dark_divider_color));
    //     } else {
    //         divider.setColor(mContext.getResources().getColor(
    //                          R.color.dropdown_divider_color));
    //     }
    // }
    // layout.setLayoutParams(new LayoutParams(LayoutParams.MATCH_PARENT, height));
    //
    // TextView sublabelView = (TextView) layout.findViewById(R.id.dropdown_sublabel);
    // CharSequence sublabel = item.getSublabel();
    // if (TextUtils.isEmpty(sublabel)) {
    //     sublabelView.setVisibility(View.GONE);
    // } else {
    //     sublabelView.setText(sublabel);
    //     sublabelView.setVisibility(View.VISIBLE);
    // }
    //
    // return layout;
    assert(0);
    AutoPtr<IView> empty;
    return empty;
}

Boolean DropdownAdapter::AreAllItemsEnabled()
{
    // ==================before translated======================
    // return mAreAllItemsEnabled;
    assert(0);
    return FALSE;
}

Boolean DropdownAdapter::IsEnabled(
    /* [in] */ Int32 position)
{
    // ==================before translated======================
    // if (position < 0 || position >= getCount()) return false;
    // DropdownItem item = getItem(position);
    // return item.isEnabled() && !item.isGroupHeader();
    assert(0);
    return FALSE;
}

Boolean DropdownAdapter::CheckAreAllItemsEnabled()
{
    // ==================before translated======================
    // for (int i = 0; i < getCount(); i++) {
    //     DropdownItem item = getItem(i);
    //     if (item.isEnabled() && !item.isGroupHeader()) {
    //         return false;
    //     }
    // }
    // return true;
    assert(0);
    return FALSE;
}

} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


