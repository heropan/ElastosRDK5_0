
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {
namespace Input {

/**
 * Creates a new SelectPopupItem adapter for the select popup alert dialog list.
 * @param context        Application context.
 * @param layoutResource Layout resource used for the alert dialog list.
 * @param items          SelectPopupItem array list.
 */
SelectPopupAdapter::SelectPopupAdapter(
    /* [in] */ IContext* context,
    /* [in] */ Int32 layoutResource,
    /* [in] */ List<SelectPopupItem> items)
{
    super(context, layoutResource, items);
    mItems = new ArrayList<SelectPopupItem>(items);

    mAreAllItemsEnabled = TRUE;
    for (Int32 i = 0; i < mItems.size(); i++) {
        if (mItems.get(i).getType() != PopupItemType.ENABLED) {
            mAreAllItemsEnabled = false;
            break;
        }
    }
}

//@Override
ECode SelectPopupAdapter::GetView(
    /* [in] */ Int32 position,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent,
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);

    if (position < 0 || position >= getCount()) return null;

    // Always pass in null so that we will get a new CheckedTextView. Otherwise, an item
    // which was previously used as an <optgroup> element (i.e. has no check), could get
    // used as an <option> element, which needs a checkbox/radio, but it would not have
    // one.
    convertView = super.getView(position, null, parent);
    ((TextView) convertView).setText(mItems.get(position).getLabel());

    if (mItems.get(position).getType() != PopupItemType.ENABLED) {
        if (mItems.get(position).getType() == PopupItemType.GROUP) {
            // Currently select_dialog_multichoice uses CheckedTextViews.
            // If that changes, the class cast will no longer be valid.
            // The WebView build cannot rely on this being the case, so
            // we must check.
            if (convertView instanceof CheckedTextView) {
                ((CheckedTextView) convertView).setCheckMarkDrawable(null);
            }
        } else {
            // Draw the disabled element in a disabled state.
            convertView.setEnabled(false);
        }
    }
    return convertView;
}

//@Override
ECode SelectPopupAdapter::AreAllItemsEnabled(
    /* [out] */ Boolean* result)
{
    return mAreAllItemsEnabled;
}

//@Override
ECode SelectPopupAdapter::IsEnabled(
    /* [in] */ Int32 position,
    /* [out] */ Boolean* result)
{
    if (position < 0 || position >= getCount()) return false;
    return mItems.get(position).getType() == PopupItemType.ENABLED;
}

} // namespace Input
} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
