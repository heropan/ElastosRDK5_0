
#ifndef __ELASTOS_DROID_WEBKIT_WEBVIEW_CHROMIUM_CONTENT_BROWSER_INPUT_SELECTPOPUPADAPTER_H__
#define __ELASTOS_DROID_WEBKIT_WEBVIEW_CHROMIUM_CONTENT_BROWSER_INPUT_SELECTPOPUPADAPTER_H__

#include "Elastos.Droid.Widget.h"
#include "elastos/droid/ext/frameworkext.h"
#include <elastos/core/Object.h>

using Elastos::Droid::Content::IContext;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::IViewGroup;
using Elastos::Droid::Widget::IArrayAdapter;
using Elastos::Droid::Widget::ICheckedTextView;
using Elastos::Droid::Widget::ITextView;

using Elastos::Utility::IArrayList;
using Elastos::Utility::IList;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Webview {
namespace Chromium {
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
    CAR_INTERFACE_DECL();

    /**
     * Creates a new SelectPopupItem adapter for the select popup alert dialog list.
     * @param context        Application context.
     * @param layoutResource Layout resource used for the alert dialog list.
     * @param items          SelectPopupItem array list.
     */
    SelectPopupAdapter(
        /* [in] */ IContext* context,
        /* [in] */ Int32 layoutResource,
        /* [in] */ IList* items);

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
    AutoPtr<IList> mItems;

    // True if all items have type PopupItemType.ENABLED.
    Boolean mAreAllItemsEnabled;
};

} // namespace Input
} // namespace Browser
} // namespace Content
} // namespace Chromium
} // namespace Webview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__ELASTOS_DROID_WEBKIT_WEBVIEW_CHROMIUM_CONTENT_BROWSER_INPUT_SELECTPOPUPADAPTER_H__
