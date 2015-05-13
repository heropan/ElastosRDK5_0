
#ifndef  __LISTACTIVITY_H__
#define  __LISTACTIVITY_H__

#include "app/Activity.h"
#include "os/Runnable.h"

using Elastos::Droid::Os::Runnable;
using Elastos::Droid::Widget::IAdapterViewOnItemClickListener;
using Elastos::Droid::Widget::IListView;
using Elastos::Droid::Widget::IAdapterView;
using Elastos::Droid::Widget::IListAdapter;

namespace Elastos {
namespace Droid {
namespace App {

class ListActivity
    : public IListActivity
    , public Activity
{
private:
    class MyListener
        : public IAdapterViewOnItemClickListener
        , public ElRefBase
    {
    public:
        MyListener(
            /* [in] */ ListActivity* host);

        CAR_INTERFACE_DECL()

        CARAPI OnItemClick(
            /* [in] */ IAdapterView* parent,
            /* [in] */ IView* view,
            /* [in] */ Int32 position,
            /* [in] */ Int64 id);

    private:
        ListActivity* mHost;
    };

    class RequestRunnable : public Runnable
    {
    public:
        RequestRunnable(
            /* [in] */ ListActivity* host);

        CARAPI Run();

    private:
        ListActivity* mHost;
    };

public:
    ListActivity();

    virtual ~ListActivity();

    CAR_INTERFACE_DECL()

    CARAPI GetBaseContext(
        /* [out] */ IContext** ctx);

    CARAPI ApplyOverrideConfiguration(
        /* [in] */ IConfiguration* overrideConfiguration);

    /**
     * Updates the screen state (current list and other views) when the
     * content changes.
     *
     * @see Activity#onContentChanged()
     */
    CARAPI OnContentChanged();

    /**
     * Provide the cursor for the list view.
     */
    virtual CARAPI SetListAdapter(
        /* [in] */ IListAdapter* adapter);

    /**
     * Set the currently selected list item to the specified
     * position with the adapter's data
     *
     * @param position
     */
    virtual CARAPI SetSelection(
        /* [in] */ Int32 position);

    /**
     * Get the position of the currently selected list item.
     */
    virtual CARAPI GetSelectedItemPosition(
        /* [out] */ Int32* pos);

    /**
     * Get the cursor row ID of the currently selected list item.
     */
    virtual CARAPI GetSelectedItemId(
        /* [out] */ Int64* id);

    /**
     * Get the activity's list view widget.
     */
    virtual CARAPI GetListView(
        /* [out] */ IListView** listView);

    /**
     * Get the ListAdapter associated with this activity's ListView.
     */
    virtual CARAPI GetListAdapter(
        /* [out] */ IListAdapter** listAdapter);

protected:
    /**
     * This method will be called when an item in the list is selected.
     * Subclasses should override. Subclasses can call
     * getListView().getItemAtPosition(position) if they need to access the
     * data associated with the selected item.
     *
     * @param l The ListView where the click happened
     * @param v The view that was clicked within the ListView
     * @param position The position of the view in the list
     * @param id The row id of the item that was clicked
     */
    virtual CARAPI OnListItemClick(
        /* [in] */ IListView* l,
        /* [in] */ IView* v,
        /* [in] */ Int32 position,
        /* [in] */ Int64 id);

    /**
     * Ensures the list view has been created before Activity restores all
     * of the view states.
     *
     *@see Activity#onRestoreInstanceState(Bundle)
     */
    virtual CARAPI OnRestoreInstanceState(
        /* [in] */ IBundle* state);

    /**
     * @see Activity#onDestroy()
     */
    virtual CARAPI OnDestroy();

private:
    CARAPI_(void) EnsureList();

protected:
    /**
     * This field should be made private, so it is hidden from the SDK.
     * {@hide}
     */
    AutoPtr<IListAdapter> mAdapter;

    /**
     * This field should be made private, so it is hidden from the SDK.
     * {@hide}
     */
    AutoPtr<IListView> mList;

private:
    AutoPtr<IHandler> mHandler;
    Boolean mFinishedStart;

    AutoPtr<IRunnable> mRequestFocus;

    AutoPtr<IAdapterViewOnItemClickListener> mOnClickListener;
    Mutex mLock;
};

} // namespace App
} // namespace Droid
} // namespace Elastos

#endif  // __LISTACTIVITY_H__
