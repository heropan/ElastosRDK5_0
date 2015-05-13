
#ifndef __ARRAYADAPTER_H__
#define __ARRAYADAPTER_H__

#include <ext/frameworkext.h>
#include "widget/BaseAdapter.h"
#include "widget/Filter.h"
#include <elastos/List.h>
#include <elastos/Mutex.h>

using Elastos::Core::CObjectContainer;
using Elastos::Core::Threading::Mutex;
using Elastos::Utility::List;
using Elastos::Core::IComparator;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::View::ILayoutInflater;
using Elastos::Droid::Widget::BaseAdapter;
using Elastos::Droid::Widget::Filter;
using Elastos::Droid::Widget::IFilterResults;
using Elastos::Droid::Widget::IFilter;

namespace Elastos{
namespace Droid{
namespace Widget{

class ArrayAdapter : public BaseAdapter
{
private:
    /**
     * <p>An array filter constrains the content of the array adapter with
     * a prefix. Each item that does not start with the supplied prefix
     * is removed from the list.</p>
     */
    class ArrayFilter
        : public ElRefBase
        , public IFilter
        , public Filter
    {
    public:
        ArrayFilter(
            /* [in] */ ArrayAdapter* host);

        CAR_INTERFACE_DECL();

        /**
         * Provide an interface that decides how Int64 to delay the message for a given query.  Useful
         * for heuristics such as posting a delay for the delete key to avoid doing any work while the
         * user holds down the delete key.
         *
         * @param delayer The delayer.
         * @hide
         */
        CARAPI SetDelayer(
            /* [in] */ IFilterDelayer* delayer);

        /**
         * <p>Starts an asynchronous filtering operation. Calling this method
         * cancels all previous non-executed filtering requests and posts a new
         * filtering request that will be executed later.</p>
         *
         * @param constraint the constraint used to filter the data
         *
         * @see #filter(CharSequence, android.widget.Filter.FilterListener)
         */
        CARAPI DoFilter(
            /* [in] */ ICharSequence* constraint);

        /**
         * <p>Starts an asynchronous filtering operation. Calling this method
         * cancels all previous non-executed filtering requests and posts a new
         * filtering request that will be executed later.</p>
         *
         * <p>Upon completion, the listener is notified.</p>
         *
         * @param constraint the constraint used to filter the data
         * @param listener a listener notified upon completion of the operation
         *
         * @see #filter(CharSequence)
         * @see #performFiltering(CharSequence)
         * @see #publishResults(CharSequence, android.widget.Filter.FilterResults)
         */
        CARAPI DoFilterEx(
            /* [in] */ ICharSequence* constraint,
            /* [in] */ IFilterListener* listener);

        /**
         * <p>Converts a value from the filtered set into a CharSequence. Subclasses
         * should override this method to convert their results. The default
         * implementation returns an empty String for NULL values or the default
         * String representation of the value.</p>
         *
         * @param resultValue the value to convert to a CharSequence
         * @return a CharSequence representing the value
         */
        CARAPI ConvertResultToString(
            /* [in] */ IInterface* resultValue,
            /* [out] */ ICharSequence** cs);

    protected:
        //@Override
        CARAPI PerformFiltering(
            /* [in] */ ICharSequence* prefix,
            /* [out] */ IFilterResults** filterResults);

        //@Override
        CARAPI PublishResults(
            /* [in] */ ICharSequence* constraint,
            /* [in] */ IFilterResults* results);

    private:
        ArrayAdapter* mHost;
    };

public:
    ArrayAdapter();

    ArrayAdapter(
        /* [in] */ IContext* context,
        /* [in] */ Int32 textViewResourceId);

    ArrayAdapter(
        /* [in] */ IContext* context,
        /* [in] */ Int32 resource,
        /* [in] */ Int32 textViewResourceId);

    ArrayAdapter(
        /* [in] */ IContext* context,
        /* [in] */ Int32 textViewResourceId,
        /* [in] */ IObjectContainer* objects);

    ArrayAdapter(
        /* [in] */ IContext* context,
        /* [in] */ Int32 resource,
        /* [in] */ Int32 textViewResourceId,
        /* [in] */ IObjectContainer* objects);

    virtual ~ArrayAdapter();

    virtual CARAPI Add(
        /* [in] */ IInterface* object);

    virtual CARAPI AddAll(
        /* [in] */ IObjectContainer* collection);

    virtual CARAPI AddAllEx(
        /* [in] */ ArrayOf<IInterface* >* items) ;

    virtual CARAPI Insert(
        /* [in] */ IInterface* object,
        /* [in] */ Int32 index);

    virtual CARAPI Remove(
        /* [in] */ IInterface* object);

    virtual CARAPI Clear();

    virtual CARAPI Sort(
        /* [in] */ IComparator* comparator);

    CARAPI NotifyDataSetChanged();


    virtual CARAPI SetNotifyOnChange(
        /* [in] */ Boolean notifyOnChange);

    virtual CARAPI_(AutoPtr<IContext>) GetContext();

    CARAPI_(Int32) GetCount();

    CARAPI_(AutoPtr<IInterface>) GetItem(
        /* [in] */ Int32 position);

    virtual CARAPI_(Int32) GetPosition(
        /* [in] */ IInterface* item);

    CARAPI_(Int64) GetItemId(
        /* [in] */ Int32 position);

    virtual CARAPI_(AutoPtr<IView>) GetView(
        /* [in] */ Int32 position,
        /* [in] */ IView* convertView,
        /* [in] */ IViewGroup* parent);

    virtual CARAPI SetDropDownViewResource(
        /* [in] */ Int32 resource);

    //@Override
    CARAPI_(AutoPtr<IView>) GetDropDownView(
        /* [in] */ Int32 position,
        /* [in] */ IView* convertView,
        /* [in] */ IViewGroup* parent);

    virtual CARAPI_(AutoPtr<IFilter>) GetFilter();

protected:
    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ Int32 resource,
        /* [in] */ Int32 textViewResourceId,
        /* [in] */ IObjectContainer* objects = NULL);

private:
    CARAPI_(AutoPtr<IView>) CreateViewFromResource(
        /* [in] */ Int32 position,
        /* [in] */ IView* convertView,
        /* [in] */ IViewGroup* parent,
        /* [in] */ Int32 resource);

    CARAPI Sort(
        /* [in] */ List<AutoPtr<IInterface> >* list,
        /* [in] */ IComparator* comparator);

private:
    /**
     * Contains the list of objects that represent the data of this ArrayAdapter.
     * The content of this list is referred to as "the array" in the documentation.
     */
    List<AutoPtr<IInterface> > mObjects;

    /**
     * Lock used to modify the content of {@link #mObjects}. Any write operation
     * performed on the array should be synchronized on this lock. This lock is also
     * used by the filter (see {@link #getFilter()} to make a synchronized copy of
     * the original array of data.
     */
    Mutex mLock;

    /**
     * The resource indicating what views to inflate to display the content of this
     * array adapter.
     */
    Int32 mResource;

    /**
     * The resource indicating what views to inflate to display the content of this
     * array adapter in a drop down widget.
     */
    Int32 mDropDownResource;

    /**
     * If the inflated resource is not a TextView, {@link #mFieldId} is used to find
     * a TextView inside the inflated views hierarchy. This field must contain the
     * identifier that matches the one defined in the resource file.
     */
    Int32 mFieldId;

    /**
     * Indicates whether or not {@link #notifyDataSetChanged()} must be called whenever
     * {@link #mObjects} is modified.
     */
    Boolean mNotifyOnChange;

    // context usually holds adapter, we use weak-reference here.
    AutoPtr<IWeakReference> mWeakContext;
    // AutoPtr<IContext> mContext;

    // A copy of the original mObjects array, initialized from and then used instead as soon as
    // the mFilter ArrayFilter is used. mObjects will then only contain the filtered values.
    AutoPtr< List<AutoPtr<IInterface> > > mOriginalValues;

    AutoPtr<ArrayFilter> mFilter;

    AutoPtr<ILayoutInflater> mInflater;
};

}// namespace Widget
}// namespace Droid
}// namespace Elastos


#endif //__ARRAYADAPTER_H__
