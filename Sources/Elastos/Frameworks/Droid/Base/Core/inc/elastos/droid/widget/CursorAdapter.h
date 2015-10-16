
#ifndef __ELASTOS_DROID_WIDGET_CURSORADAPTER_H__
#define __ELASTOS_DROID_WIDGET_CURSORADAPTER_H__

#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/widget/BaseAdapter.h"
#include "elastos/droid/database/ContentObserver.h"

using Elastos::Core::ICharSequence;
using Elastos::Droid::View::IViewGroup;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Database::ICursor;
using Elastos::Droid::Database::IDataSetObserver;
using Elastos::Droid::Database::EIID_IDataSetObserver;
using Elastos::Droid::Database::ContentObserver;
using Elastos::Droid::Database::IContentObserver;
using Elastos::Droid::Database::EIID_IContentObserver;
using Elastos::Droid::Os::IHandler;

namespace Elastos {
namespace Droid {
namespace Widget {

/**
 * Adapter that exposes data from a {@link android.database.Cursor Cursor} to a
 * {@link android.widget.ListView ListView} widget. The Cursor must include
 * a column named "_id" or this class will not work.
 */
class CursorAdapter : public BaseAdapter
{
public:
    CursorAdapter();

    /**
     * Constructor. The adapter will call requery() on the cursor whenever
     * it changes so that the most recent data is always displayed.
     *
     * @param c The cursor from which to get the data.
     * @param context The context
     */
    CursorAdapter(
        /* [in] */ IContext* context,
        /* [in] */ ICursor* c);

    /**
     * Constructor
     * @param c The cursor from which to get the data.
     * @param context The context
     * @param autoRequery If true the adapter will call requery() on the
     *                    cursor whenever it changes so the most recent
     *                    data is always displayed.
     */
    CursorAdapter(
        /* [in] */ IContext* context,
        /* [in] */ ICursor* c,
        /* [in] */ Boolean autoRequery);

    CursorAdapter(
        /* [in] */ IContext* context,
        /* [in] */ ICursor* c,
        /* [in] */ Int32 flags);

    /**
     * Returns the cursor.
     * @return the cursor.
     */
    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid) = 0;

    virtual CARAPI_(AutoPtr<ICursor>) GetCursor();

    /**
     * @see android.widget.ListAdapter#getCount()
     */
    virtual CARAPI_(Int32) GetCount();

    /**
     * @see android.widget.ListAdapter#getItem(Int32)
     */
    virtual CARAPI_(AutoPtr<IInterface>) GetItem(
        /* [in] */ Int32 position);

    /**
     * @see android.widget.ListAdapter#getItemId(Int32)
     */
    virtual CARAPI_(Int64) GetItemId(
        /* [in] */ Int32 position);

    virtual CARAPI_(Boolean) HasStableIds();

    /**
     * @see android.widget.ListAdapter#getView(Int32, View, ViewGroup)
     */
    virtual CARAPI_(AutoPtr<IView>) GetView(
        /* [in] */ Int32 position,
        /* [in] */ IView* convertView,
        /* [in] */ IViewGroup* parent);

    virtual CARAPI_(AutoPtr<IView>) GetDropDownView(
        /* [in] */ Int32 position,
        /* [in] */ IView* convertView,
        /* [in] */ IViewGroup* parent);

    /**
     * Makes a new view to hold the data pointed to by cursor.
     * @param context Interface to application's global information
     * @param cursor The cursor from which to get the data. The cursor is already
     * moved to the correct position.
     * @param parent The parent to which the new view is attached to
     * @return the newly created view.
     */
    virtual CARAPI_(AutoPtr<IView>) NewView(
        /* [in] */ IContext* context,
        /* [in] */ ICursor* cursor,
        /* [in] */ IViewGroup* parent) = 0;

    /**
     * Makes a new drop down view to hold the data pointed to by cursor.
     * @param context Interface to application's global information
     * @param cursor The cursor from which to get the data. The cursor is already
     * moved to the correct position.
     * @param parent The parent to which the new view is attached to
     * @return the newly created view.
     */
    virtual CARAPI_(AutoPtr<IView>) NewDropDownView(
        /* [in] */ IContext* context,
        /* [in] */ ICursor* cursor,
        /* [in] */ IViewGroup* parent);

    /**
     * Bind an existing view to the data pointed to by cursor
     * @param view Existing view, returned earlier by newView
     * @param context Interface to application's global information
     * @param cursor The cursor from which to get the data. The cursor is already
     * moved to the correct position.
     */
    virtual CARAPI BindView(
        /* [in] */ IView* view,
        /* [in] */ IContext* context,
        /* [in] */ ICursor* cursor) = 0;

    /**
     * Change the underlying cursor to a new cursor. If there is an existing cursor it will be
     * closed.
     *
     * @param cursor the new cursor to be used
     */
    virtual CARAPI ChangeCursor(
        /* [in] */ ICursor* cursor);

    /**
     * <p>Converts the cursor into a CharSequence. Subclasses should override this
     * method to convert their results. The default implementation returns an
     * empty String for NULL values or the default String representation of
     * the value.</p>
     *
     * @param cursor the cursor to convert to a CharSequence
     * @return a CharSequence representing the value
     */
    virtual CARAPI_(AutoPtr<ICharSequence>) ConvertToString(
        /* [in] */ ICursor* cursor);

    /**
     * Runs a query with the specified constraint. This query is requested
     * by the filter attached to this adapter.
     *
     * The query is provided by a
     * {@link android.widget.FilterQueryProvider}.
     * If no provider is specified, the current cursor is not filtered and returned.
     *
     * After this method returns the resulting cursor is passed to {@link #changeCursor(Cursor)}
     * and the previous cursor is closed.
     *
     * This method is always executed on a background thread, not on the
     * application's main thread (or UI thread.)
     *
     * Contract: when constraint is NULL or empty, the original results,
     * prior to any filtering, must be returned.
     *
     * @param constraint the constraint with which the query must be filtered
     *
     * @return a Cursor representing the results of the new query
     *
     * @see #getFilter()
     * @see #getFilterQueryProvider()
     * @see #setFilterQueryProvider(android.widget.FilterQueryProvider)
     */
    virtual CARAPI_(AutoPtr<ICursor>) RunQueryOnBackgroundThread(
        /* [in] */ ICharSequence* constraint);

    virtual CARAPI_(AutoPtr<IFilter>) GetFilter();

    /**
     * Returns the query filter provider used for filtering. When the
     * provider is NULL, no filtering occurs.
     *
     * @return the current filter query provider or NULL if it does not exist
     *
     * @see #setFilterQueryProvider(android.widget.FilterQueryProvider)
     * @see #runQueryOnBackgroundThread(CharSequence)
     */
    virtual CARAPI_(AutoPtr<IFilterQueryProvider>) GetFilterQueryProvider();

    /**
     * Sets the query filter provider used to filter the current Cursor.
     * The provider's
     * {@link android.widget.FilterQueryProvider#runQuery(CharSequence)}
     * method is invoked when filtering is requested by a client of
     * this adapter.
     *
     * @param filterQueryProvider the filter query provider or NULL to remove it
     *
     * @see #getFilterQueryProvider()
     * @see #runQueryOnBackgroundThread(CharSequence)
     */
    virtual CARAPI SetFilterQueryProvider(
        /* [in] */ IFilterQueryProvider* filterQueryProvider);

    /**
     * Swap in a new Cursor, returning the old Cursor.  Unlike
     * {@link #changeCursor(Cursor)}, the returned old Cursor is <em>not</em>
     * closed.
     *
     * @param newCursor The new cursor to be used.
     * @return Returns the previously set Cursor, or null if there wasa not one.
     * If the given new Cursor is the same instance is the previously set
     * Cursor, null is also returned.
     */
    AutoPtr<ICursor> SwapCursor(
        /* [in] */ ICursor* newCursor);

protected:
    virtual CARAPI_(void) Init(
        /* [in] */ IContext* context,
        /* [in] */ ICursor* c,
        /* [in] */ Boolean autoRequery);

    virtual CARAPI_(void) Init(
        /* [in] */ IContext* context,
        /* [in] */ ICursor* c,
        /* [in] */ Int32 flags = ICursorAdapter::FLAG_AUTO_REQUERY);
    /**
     * Called when the {@link ContentObserver} on the cursor receives a change notification.
     * The default implementation provides the auto-requery logic, but may be overridden by
     * sub classes.
     *
     * @see ContentObserver#onChange(Boolean)
     */
    virtual CARAPI_(void) OnContentChanged();

private:
    class ChangeObserver : public ContentObserver
    {
    public:
        ChangeObserver(
            /* [in] */ CursorAdapter* host);

        virtual CARAPI DeliverSelfNotifications(
            /* [out] */ Boolean* result);

        virtual CARAPI OnChange(
            /* [in] */ Boolean selfChange);

    private:

        virtual CARAPI_(AutoPtr<IHandler>) InitSuperHandler();

    protected:
        CursorAdapter* mHost;
    };

    class MyDataSetObserver
        : public IDataSetObserver
        , public ElRefBase
    {
    public:

        MyDataSetObserver(
            /* [in] */ CursorAdapter* host);

        CAR_INTERFACE_DECL()

        virtual CARAPI OnChanged();

        virtual CARAPI OnInvalidated();
    protected:

        CursorAdapter* mHost;

    };

protected:
    /**
     * This field should be made private, so it is hidden from the SDK.
     * {@hide}
     */
    Boolean mDataValid;
    /**
     * This field should be made private, so it is hidden from the SDK.
     * {@hide}
     */
    Boolean mAutoRequery;
    /**
     * This field should be made private, so it is hidden from the SDK.
     * {@hide}
     */
    AutoPtr<ICursor> mCursor;
    /**
     * This field should be made private, so it is hidden from the SDK.
     * {@hide}
     */
    // context usually holds adapter, we use weak-reference here.
    AutoPtr<IWeakReference> mWeakContext;
    // AutoPtr<IContext> mContext;
    /**
     * This field should be made private, so it is hidden from the SDK.
     * {@hide}
     */
    Int32 mRowIDColumn;
    /**
     * This field should be made private, so it is hidden from the SDK.
     * {@hide}
     */
    AutoPtr<ChangeObserver> mChangeObserver;
    /**
     * This field should be made private, so it is hidden from the SDK.
     * {@hide}
     */
    AutoPtr<IDataSetObserver> mDataSetObserver;// = new MyDataSetObserver();
    /**
     * This field should be made private, so it is hidden from the SDK.
     * {@hide}
     */
    AutoPtr<ICursorFilter> mCursorFilter;
    /**
     * This field should be made private, so it is hidden from the SDK.
     * {@hide}
     */
    AutoPtr<IFilterQueryProvider> mFilterQueryProvider;

};

}// namespace Elastos
}// namespace Droid
}// namespace Widget
#endif
