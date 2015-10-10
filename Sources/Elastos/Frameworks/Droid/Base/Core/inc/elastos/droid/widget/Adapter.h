
#ifndef __ELASTOS_DROID_WIDGET_ADAPTER_H__
#define __ELASTOS_DROID_WIDGET_ADAPTER_H__

#include "elastos/droid/ext/frameworkext.h"

using Elastos::Droid::Database::IDataSetObserver;
using Elastos::Droid::View::IViewGroup;
using Elastos::Droid::View::IView;

namespace Elastos {
namespace Droid {
namespace Widget {

/**
 * An Adapter object acts as a bridge between an {@link AdapterView} and the
 * underlying data for that view. The Adapter provides access to the data items.
 * The Adapter is also responsible for making a {@link android.view.View} for
 * each item in the data set.
 *
 * @see android.widget.ArrayAdapter
 * @see android.widget.CursorAdapter
 * @see android.widget.SimpleCursorAdapter
 */
class Adapter {
public:
    virtual ~Adapter() {}

    /**
     * Register an observer that is called when changes happen to the data used by this adapter.
     *
     * @param observer the object that gets notified when the data set changes.
     */
    virtual CARAPI RegisterDataSetObserver(
        /* [in] */ IDataSetObserver* observer) = 0;

    /**
     * Unregister an observer that has previously been registered with this
     * adapter via {@link #registerDataSetObserver}.
     *
     * @param observer the object to unregister.
     */
    virtual CARAPI UnregisterDataSetObserver(
        /* [in] */ IDataSetObserver* observer) = 0;

    /**
     * How many items are in the data set represented by this Adapter.
     *
     * @return Count of items.
     */
    virtual CARAPI_(Int32) GetCount() = 0;

    /**
     * Get the data item associated with the specified position in the data set.
     *
     * @param position Position of the item whose data we want within the adapter's
     * data set.
     * @return The data at the specified position.
     */
    virtual CARAPI_(AutoPtr<IInterface>) GetItem(
        /* [in] */ Int32 position) = 0;

    /**
     * Get the row id associated with the specified position in the list.
     *
     * @param position The position of the item within the adapter's data set whose row id we want.
     * @return The id of the item at the specified position.
     */
    virtual CARAPI_(Int64) GetItemId(
        /* [in] */ Int32 position) = 0;

    /**
     * Indicates whether the item ids are stable across changes to the
     * underlying data.
     *
     * @return True if the same id always refers to the same object.
     */
    virtual CARAPI_(Boolean) HasStableIds() = 0;

    /**
     * Get a View that displays the data at the specified position in the data set. You can either
     * create a View manually or inflate it from an XML layout file. When the View is inflated, the
     * parent View (GridView, ListView...) will apply default layout parameters unless you use
     * {@link android.view.LayoutInflater#inflate(int, android.view.ViewGroup, boolean)}
     * to specify a root view and to prevent attachment to the root.
     *
     * @param position The position of the item within the adapter's data set of the item whose view
     *        we want.
     * @param convertView The old view to reuse, if possible. Note: You should check that this view
     *        is non-null and of an appropriate type before using. If it is not possible to convert
     *        this view to display the correct data, this method can create a new view.
     *        Heterogeneous lists can specify their number of view types, so that this View is
     *        always of the right type (see {@link #getViewTypeCount()} and
     *        {@link #getItemViewType(int)}).
     * @param parent The parent that this view will eventually be attached to
     * @return A View corresponding to the data at the specified position.
     */
    virtual CARAPI_(AutoPtr<IView>) GetView(
        /* [in] */ Int32 position,
        /* [in] */ IView* convertView,
        /* [in] */ IViewGroup* parent) = 0;

    /**
     * Get the type of View that will be created by {@link #getView} for the specified item.
     *
     * @param position The position of the item within the adapter's data set whose view type we
     *        want.
     * @return An integer representing the type of View. Two views should share the same type if one
     *         can be converted to the other in {@link #getView}. Note: Integers must be in the
     *         range 0 to {@link #getViewTypeCount} - 1. {@link #IGNORE_ITEM_VIEW_TYPE} can
     *         also be returned.
     * @see #IGNORE_ITEM_VIEW_TYPE
     */
    virtual CARAPI_(Int32) GetItemViewType(
        /* [in] */ Int32 position) = 0;

    /**
     * <p>
     * Returns the number of types of Views that will be created by
     * {@link #getView}. Each type represents a set of views that can be
     * converted in {@link #getView}. If the adapter always returns the same
     * type of View for all items, this method should return 1.
     * </p>
     * <p>
     * This method will only be called when when the adapter is set on the
     * the {@link AdapterView}.
     * </p>
     *
     * @return The number of types of Views that will be created by this adapter
     */
    virtual CARAPI_(Int32) GetViewTypeCount() = 0;

     /**
      * @return true if this adapter doesn't contain any data.  This is used to determine
      * whether the empty view should be displayed.  A typical implementation will return
      * getCount() == 0 but since getCount() includes the headers and footers, specialized
      * adapters might want a different behavior.
      */
    virtual CARAPI_(Boolean) IsEmpty() = 0;
};

/**
 * Extended {@link Adapter} that is the bridge between a {@link ListView}
 * and the data that backs the list. Frequently that data comes from a Cursor,
 * but that is not
 * required. The ListView can display any data provided that it is wrapped in a
 * ListAdapter.
 */
class ListAdapter: public Adapter
{
public:
    /**
     * Indicates whether all the items in this adapter are enabled. If the
     * value returned by this method changes over time, there is no guarantee
     * it will take effect.  If true, it means all items are selectable and
     * clickable (there is no separator.)
     *
     * @return True if all items are enabled, false otherwise.
     *
     * @see #isEnabled(int)
     */
    virtual CARAPI_(Boolean) AreAllItemsEnabled() = 0;

    /**
     * Returns true if the item at the specified position is not a separator.
     * (A separator is a non-selectable, non-clickable item).
     *
     * The result is unspecified if position is invalid. An {@link ArrayIndexOutOfBoundsException}
     * should be thrown in that case for fast failure.
     *
     * @param position Index of the item
     *
     * @return True if the item is not a separator
     *
     * @see #areAllItemsEnabled()
     */
    virtual CARAPI_(Boolean) IsEnabled(
        /* [in] */ Int32 position) = 0;
};


}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif //__APTER_H__
