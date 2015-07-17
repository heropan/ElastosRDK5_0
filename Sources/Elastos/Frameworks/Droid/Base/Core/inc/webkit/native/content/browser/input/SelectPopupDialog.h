
#ifndef __SELECTPOPUPDIALOG_H__
#define __SELECTPOPUPDIALOG_H__

// import android.app.AlertDialog;
// import android.content.Context;
// import android.content.DialogInterface;
// import android.content.res.TypedArray;
// import android.util.SparseBooleanArray;
// import android.view.View;
// import android.widget.AdapterView;
// import android.widget.AdapterView.OnItemClickListener;
// import android.widget.ListView;

// import org.chromium.content.R;
// import org.chromium.content.browser.ContentViewCore;

// import java.util.List;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {
namespace Input {

/**
 * Handles the popup dialog for the <select> HTML tag support.
 */
class SelectPopupDialog
    : public Object,
    , public SelectPopup
{
private:
    class OkDialogInterfaceOnClickListener
        : public Object
        , public IDialogInterfaceOnClickListener
    {
    public:
        OkDialogInterfaceOnClickListener(
            /* [in] */ SelectPopupDialog* owner,
            /* [in] */ IListView* listView);

        CARAPI OnClick(
            /* [in] */ IDialogInterface* dialog,
            /* [in] */ Int32 which);

    private:
        SelectPopupDialog* mOwner;
        IListView* mListView;
    };

    class CancelDialogInterfaceOnClickListener
        : public Object
        , public IDialogInterfaceOnClickListener
    {
    public:
        CancelDialogInterfaceOnClickListener(
            /* [in] */ SelectPopupDialog* owner);

        CARAPI OnClick(
            /* [in] */ IDialogInterface* dialog,
            /* [in] */ Int32 which);

    private:
        SelectPopupDialog* mOwner;
    };

    class InnerAdapterViewOnItemClickListener
        : public Object
        , public IIAdapterViewOnItemClickListener
    {
    public:
        InnerAdapterViewOnItemClickListener(
            /* [in] */ SelectPopupDialog* owner,
            /* [in] */ IListView* listView);

        CARAPI OnItemClick(
            /* [in] */ IAdapterView* parent,
            /* [in] */ IView* v,
            /* [in] */ Int32 position,
            /* [in] */ Int64 id)

    private:
        SelectPopupDialog* mOwner;
        IListView* mListView;
    };

    class InnerDialogInterfaceOnCancelListener
        : public Object
        , public IDialogInterfaceOnCancelListener
    {
    public:
        InnerDialogInterfaceOnCancelListener(
            /* [in] */ SelectPopupDialog* owner);

        CARAPI OnCancel(
            /* [in] */ IDialogInterface* dialog)

    private:
        SelectPopupDialog* mOwner;
    };

public:
    SelectPopupDialog(
        /* [in] */ ContentViewCore* contentViewCore,
        /* [in] */ List<SelectPopupItem> items,
        /* [in] */ Boolean multiple,
        /* [in] */ ArrayOf<Int32>* selected);

    //@Override
    CARAPI_(void) Show();

    //@Override
    CARAPI_(void) Hide();

private:
    CARAPI_(Int32) GetSelectDialogLayout(
        /* [in] */ Boolean isMultiChoice);

    CARAPI_(AutoPtr< ArrayOf<Int32> >) GetSelectedIndices(
        /* [in] */ IListView* listView);

private:
    static const AutoPtr< ArrayOf<Int32> > SELECT_DIALOG_ATTRS;

    // The dialog hosting the popup list view.
    AutoPtr<IAlertDialog> mListBoxPopup;

    const AutoPtr<ContentViewCore> mContentViewCore;
    const AutoPtr<IContext> mContext;
};

} // namespace Input
} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__SELECTPOPUPDIALOG_H__
