
#ifndef __SELECTPOPUPADAPTER_H__
#define __SELECTPOPUPADAPTER_H__

// import android.content.Context;
// import android.view.View;
// import android.view.ViewGroup;
// import android.widget.ArrayAdapter;
// import android.widget.CheckedTextView;
// import android.widget.TextView;

// import java.util.ArrayList;
// import java.util.List;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {
namespace Input {

/**
 * Select popup item adapter for SelectPopupDialog, used so we can disable
 * OPTION_GROUP items.
 */
class SelectPopupAdapter
    : public Object
    , public IArrayAdapter
{
public:
    /**
     * Creates a new SelectPopupItem adapter for the select popup alert dialog list.
     * @param context        Application context.
     * @param layoutResource Layout resource used for the alert dialog list.
     * @param items          SelectPopupItem array list.
     */
    SelectPopupAdapter(
        /* [in] */ IContext* context,
        /* [in] */ Int32 layoutResource,
        /* [in] */ List<SelectPopupItem> items);

    //@Override
    CARAPI GetView(
        /* [in] */ Int32 position,
        /* [in] */ IView* convertView,
        /* [in] */ IViewGroup* parent,
        /* [out] */ IView** view);

    //@Override
    CARAPI AreAllItemsEnabled(
        /* [out] */ Boolean* result);

    //@Override
    CARAPI IsEnabled(
        /* [in] */ Int32 position,
        /* [out] */ Boolean* result);

private:
    // Holds the items of the select popup alert dialog list.
    List<SelectPopupItem> mItems;

    // True if all items have type PopupItemType.ENABLED.
    Boolean mAreAllItemsEnabled;
};

} // namespace Input
} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__SELECTPOPUPADAPTER_H__
