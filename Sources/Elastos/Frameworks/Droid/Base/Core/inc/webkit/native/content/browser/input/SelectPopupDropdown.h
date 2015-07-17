
#ifndef __SELECTPOPUPDROPDOWN_H__
#define __SELECTPOPUPDROPDOWN_H__

// import android.content.Context;
// import android.graphics.Rect;
// import android.view.View;
// import android.widget.AdapterView;
// import android.widget.PopupWindow;

// import org.chromium.content.browser.ContentViewCore;
// import org.chromium.content.browser.RenderCoordinates;
// import org.chromium.ui.DropdownAdapter;
// import org.chromium.ui.DropdownItem;
// import org.chromium.ui.DropdownPopupWindow;

// import java.util.List;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {
namespace Input {

/**
 * Handles the dropdown popup for the <select> HTML tag support.
 */
class SelectPopupDropdown
    : public Object,
    , public SelectPopup
{
private:
    class InnerAdapterViewOnItemClickListener
        : public Object
        , public IAdapterViewOnItemClickListener
    {
    public:
        InnerAdapterViewOnItemClickListener(
            /* [in] */ SelectPopupDropdown* owner);

        CARAPI OnItemClick(
            /* [in] */ IAdapterView* parent,
            /* [in] */ IView* view,
            /* [in] */ Int32 position,
            /* [in] */ Int64 id);

    private:
        SelectPopupDropdown* mOwner;
    };

    class InnerPopupWindowOnDismissListener
        : public Object
        , public IPopupWindowOnDismissListener
    {
    public:
        InnerPopupWindowOnDismissListener(
            /* [in] */ SelectPopupDropdown* owner);

        CARAPI OnDismiss();

    private:
        SelectPopupDropdown* mOwner;
    };

public:
    SelectPopupDropdown(
        /* [in] */ ContentViewCore* contentViewCore,
        /* [in] */ List<SelectPopupItem> items,
        /* [in] */ IRect* bounds,
        /* [in] */ ArrayOf<Int32>* selected);

    //@Override
    CARAPI_(void) Show();

    //@Override
    CARAPI_(void) Hide();

private:
    const AutoPtr<ContentViewCore> mContentViewCore;
    const AutoPtr<IContext> mContext;

    AutoPtr<DropdownPopupWindow> mDropdownPopupWindow;
    Int32 mInitialSelection;
    Boolean mAlreadySelectedItems;
};

} // namespace Input
} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__SELECTPOPUPDROPDOWN_H__
