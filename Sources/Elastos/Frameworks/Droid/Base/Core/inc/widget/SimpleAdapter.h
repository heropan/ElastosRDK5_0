
#ifndef __SIMPLEADAPTER_H__
#define __SIMPLEADAPTER_H__

#include "ext/frameworkext.h"
#include "widget/BaseAdapter.h"
#include "widget/Filter.h"
#include <elastos/utility/etl/List.h>

using Elastos::Utility::Etl::List;
using Elastos::Utility::IObjectStringMap;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::View::ILayoutInflater;

namespace Elastos {
namespace Droid {
namespace Widget {


class SimpleAdapter : public BaseAdapter
{
private:
    /**
     * <p>An array filters constrains the content of the array adapter with
     * a prefix. Each item that does not start with the supplied prefix
     * is removed from the list.</p>
     */
    class SimpleFilter
        : public ElRefBase
        , public IFilter
        , public Filter
    {
    public:
        SimpleFilter(
            /* [in] */ SimpleAdapter* host);

        CAR_INTERFACE_DECL();

        CARAPI SetDelayer(
            /* [in] */ IFilterDelayer* delayer);

        CARAPI DoFilter(
            /* [in] */ ICharSequence* constraint);

        CARAPI DoFilter(
            /* [in] */ ICharSequence* constraint,
            /* [in] */ IFilterListener* listener);

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
        SimpleAdapter* mHost;
    };

public:
    SimpleAdapter();

    /**
     * Constructor
     *
     * @param context The context where the View associated with this SimpleAdapter is running
     * @param data A List of Maps. Each entry in the List corresponds to one row in the list. The
     *        Maps contain the data for each row, and should include all the entries specified in
     *        "from"
     * @param resource Resource identifier of a view layout that defines the views for this list
     *        item. The layout file should include at least those named views defined in "to"
     * @param from A list of column names that will be added to the Map associated with each
     *        item.
     * @param to The views that should display column in the "from" parameter. These should all be
     *        TextViews. The first N views in this list are given the values of the first N columns
     *        in the from parameter.
     */
    SimpleAdapter(
        /* [in] */ IContext* context,
        /* [in] */ IObjectContainer* data, //List<? extends Map<String, ?>>
        /* [in] */ Int32 resource,
        /* [in] */ ArrayOf<String>* from,
        /* [in] */ ArrayOf<Int32>* to);

    CARAPI_(Int32) GetCount();

    CARAPI_(AutoPtr<IInterface>) GetItem(
        /* [in] */ Int32 position);

    CARAPI_(Int64) GetItemId(
        /* [in] */ Int32 position);

    virtual CARAPI_(AutoPtr<IView>) GetView(
        /* [in] */ Int32 position,
        /* [in] */ IView* convertView,
        /* [in] */ IViewGroup* parent);

    virtual CARAPI SetDropDownViewResource(
        /* [in] */ Int32 resource);

    CARAPI_(AutoPtr<IView>) GetDropDownView(
        /* [in] */ Int32 position,
        /* [in] */ IView* convertView,
        /* [in] */ IViewGroup* parent);

    virtual CARAPI_(AutoPtr<ISimpleAdapterViewBinder>) GetViewBinder();

    virtual CARAPI SetViewBinder(
        /* [in] */ ISimpleAdapterViewBinder* viewBinder);

    virtual CARAPI SetViewImage(
        /* [in] */ IImageView* v,
        /* [in] */ Int32 value);

    virtual CARAPI SetViewImage(
        /* [in] */ IImageView* v,
        /* [in] */ const String& value);

    virtual CARAPI SetViewText(
        /* [in] */ ITextView* v,
        /* [in] */ const String& text);

    virtual CARAPI_(AutoPtr<IFilter>) GetFilter();

protected:
    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IObjectContainer* data,
        /* [in] */ Int32 resource,
        /* [in] */ ArrayOf<String>* from,
        /* [in] */ ArrayOf<Int32>* to);

private:
    CARAPI_(AutoPtr<IView>) CreateViewFromResource(
        /* [in] */ Int32 position,
        /* [in] */ IView* convertView,
        /* [in] */ IViewGroup* parent,
        /* [in] */ Int32 resource);

    CARAPI BindView(
        /* [in] */ Int32 position,
        /* [in] */ IView* view);

private:
    AutoPtr<ArrayOf<Int32> > mTo;
    AutoPtr<ArrayOf<String> > mFrom;
    AutoPtr<ISimpleAdapterViewBinder> mViewBinder;

    List<AutoPtr<IObjectStringMap> > mData; //List<? extends Map<String, ?>>

    Int32 mResource;
    Int32 mDropDownResource;
    AutoPtr<ILayoutInflater> mInflater;

    AutoPtr<SimpleFilter> mFilter;
    AutoPtr< List< AutoPtr<IObjectStringMap> > > mUnfilteredData;
};


} // namespace Widget
} // namespace Droid
} // namespace Elastos

#endif //__SIMPLEADAPTER_H__
