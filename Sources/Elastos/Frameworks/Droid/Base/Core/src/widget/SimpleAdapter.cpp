
#include "widget/SimpleAdapter.h"
#include <Elastos.CoreLibrary.h>
#include "os/ElDataInterface.h"
#include "net/Uri.h"
#include <elastos/core/StringUtils.h>
#include <elastos/utility/logging/Slogger.h>

using Elastos::Core::StringUtils;
using Elastos::Core::IBoolean;
using Elastos::Core::IInteger32;
using Elastos::Core::CStringWrapper;
using Elastos::Core::ICharSequence;
using Elastos::Droid::Os::ElDataInterface;
using Elastos::Droid::Net::IUri;
using Elastos::Droid::Net::Uri;
using Elastos::Utility::Logging::Slogger;

namespace Elastos {
namespace Droid {
namespace Widget {

//==============================================================================
//                      SimpleAdapter::SimpleFilter
//==============================================================================
SimpleAdapter::SimpleFilter::SimpleFilter(
    /* [in] */ SimpleAdapter* host)
    : mHost(host)
{}

CAR_INTERFACE_IMPL(SimpleAdapter::SimpleFilter, IFilter);

ECode SimpleAdapter::SimpleFilter::SetDelayer(
    /* [in] */ IFilterDelayer* delayer)
{
    return Filter::SetDelayer(delayer);
}

ECode SimpleAdapter::SimpleFilter::DoFilter(
    /* [in] */ ICharSequence* constraint)
{
    return Filter::DoFilter(constraint);
}

ECode SimpleAdapter::SimpleFilter::DoFilter(
    /* [in] */ ICharSequence* constraint,
    /* [in] */ IFilterListener* listener)
{
    return Filter::DoFilter(constraint, listener);
}

ECode SimpleAdapter::SimpleFilter::ConvertResultToString(
    /* [in] */ IInterface* resultValue,
    /* [out] */ ICharSequence** cs)
{
    VALIDATE_NOT_NULL(cs);
    return Filter::ConvertResultToString(resultValue, cs);
}

//@Override
ECode SimpleAdapter::SimpleFilter::PerformFiltering(
    /* [in] */ ICharSequence* prefix,
    /* [out] */ IFilterResults** filterResults)
{
    AutoPtr<FilterResults> results = new FilterResults();

    if (mHost->mUnfilteredData == NULL) {
        mHost->mUnfilteredData = new List<AutoPtr<IObjectStringMap> >(mHost->mData);
    }

    Int32 length = 0;
    if (prefix) {
        prefix->GetLength(&length);
    }
    if (length == 0) {
        results->mValues = (IInterface*)new ElDataInterface<
            List<AutoPtr<IObjectStringMap> > >(*mHost->mUnfilteredData);
        results->mCount = mHost->mUnfilteredData->GetSize();
    }
    else {
        String prefixString;
        prefix->ToString(&prefixString);

        List<AutoPtr<IObjectStringMap> > newValues;
        List<AutoPtr<IObjectStringMap> >::Iterator iter =
                mHost->mUnfilteredData->Begin();
        for (; iter != mHost->mUnfilteredData->End(); ++iter) {
            IObjectStringMap* h = (*iter).Get();
            if (h != NULL) {
                Int32 len = mHost->mTo->GetLength();
                for (Int32 i = 0; i < len; i++) {
                    //String str =  (String)h.get(mFrom[j]);
                    AutoPtr<ICharSequence> cs;
                    h->Get((*mHost->mFrom)[i], (IInterface**)&cs);
                    String str;
                    cs->ToString(&str);

                    Int32 start = 0;
                    Int32 index = str.IndexOf(' ');
                    String subStr = str;
                    while (index > 0 || subStr.GetLength() > 0) {
                        if (index < 0) {
                            index = subStr.GetLength();
                        }
                        String word = subStr.Substring(start, index - start);
                        subStr = subStr.Substring(index + 1);
                        start = index + 1;
                        if (word.StartWithIgnoreCase(prefixString)) {
                            newValues.PushBack(h);
                            break;
                        }
                        index = subStr.IndexOf(' ');
                    }
                }
            }
        }

        results->mValues =
            new ElDataInterface<List<AutoPtr<IObjectStringMap> > >(newValues);
        results->mCount = newValues.GetSize();
    }

    *filterResults = (IFilterResults*)results.Get();
    REFCOUNT_ADD(*filterResults);

    return NOERROR;
}

//@Override
ECode SimpleAdapter::SimpleFilter::PublishResults(
    /* [in] */ ICharSequence* constraint,
    /* [in] */ IFilterResults* results)
{
    assert(results);
    FilterResults* filterResults = (FilterResults*)results;
    List<AutoPtr<IObjectStringMap> >& values =
        ((ElDataInterface<List<AutoPtr<IObjectStringMap> > >*)filterResults
        ->mValues.Get())->mData;
    //noinspection unchecked
    mHost->mData.Assign(values.Begin(), values.End());
    if (filterResults->mCount > 0) {
        mHost->NotifyDataSetChanged();
    }
    else {
        mHost->NotifyDataSetInvalidated();
    }

    return NOERROR;
}

//==============================================================================
//                      SimpleAdapter::SimpleAdapter
//==============================================================================

SimpleAdapter::SimpleAdapter()
{}

SimpleAdapter::SimpleAdapter(
    /* [in] */ IContext* context,
    /* [in] */ IObjectContainer* data,
    /* [in] */ Int32 resource,
    /* [in] */ ArrayOf<String>* from,
    /* [in] */ ArrayOf<Int32>* to)
{
    Init(context, data, resource, from, to);
}

ECode SimpleAdapter::Init(
    /* [in] */ IContext* context,
    /* [in] */ IObjectContainer* data,
    /* [in] */ Int32 resource,
    /* [in] */ ArrayOf<String>* from,
    /* [in] */ ArrayOf<Int32>* to)
{
    VALIDATE_NOT_NULL(data);
    VALIDATE_NOT_NULL(from);
    VALIDATE_NOT_NULL(to);

    AutoPtr<IObjectEnumerator> it;
    data->GetObjectEnumerator((IObjectEnumerator**)&it);
    Boolean hasNext;
    while (it->MoveNext(&hasNext), hasNext) {
        AutoPtr<IInterface> obj;
        it->Current((IInterface**)&obj);
        AutoPtr<IObjectStringMap> item = IObjectStringMap::Probe(obj.Get());
        mData.PushBack(item);
    }

    mResource = mDropDownResource = resource;
    mFrom = from->Clone();
    mTo = to->Clone();

    context->GetSystemService(
        IContext::LAYOUT_INFLATER_SERVICE, (IInterface**)&mInflater);

    return NOERROR;
}

/**
 * @see android.widget.Adapter#getCount()
 */
Int32 SimpleAdapter::GetCount()
{
    return mData.GetSize();
}

/**
 * @see android.widget.Adapter#getItem(Int32)
 */
AutoPtr<IInterface> SimpleAdapter::GetItem(
    /* [in] */ Int32 position)
{
    return mData[position];
}

/**
 * @see android.widget.Adapter#getItemId(Int32)
 */
Int64 SimpleAdapter::GetItemId(
    /* [in] */ Int32 position)
{
    return position;
}

/**
 * @see android.widget.Adapter#getView(Int32, View, ViewGroup)
 */
AutoPtr<IView> SimpleAdapter::GetView(
    /* [in] */ Int32 position,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent)
{
    return CreateViewFromResource(position, convertView, parent, mResource);
}

AutoPtr<IView> SimpleAdapter::CreateViewFromResource(
    /* [in] */ Int32 position,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent,
    /* [in] */ Int32 resource)
{
    AutoPtr<IView> view;

    if (convertView == NULL) {
        ECode ec = mInflater->Inflate(resource, parent, FALSE, (IView**)&view);
        if (FAILED(ec) || view == NULL) {
            Slogger::E("SimpleAdapter", "Error: failed to inflate view with resource id=%08x, position=%d, ec=%08x",
                resource, position, ec);
            return NULL;
        }
    }
    else {
        view = convertView;
    }

    BindView(position, view);

// #if defined(_DEBUG) || defined(_ELASTOS_DEBUG)
//     Int32 viewId = 0;
//     if (view) {
//         view->GetId(&viewId);
//     }
//     Slogger::D("SimpleAdapter", "Created View: %p, id: %08x, resource: %08x, position: %d",
//         view.Get(), viewId, resource, position);
// #endif
    assert(view != NULL);
    return view;
}

ECode SimpleAdapter::SetDropDownViewResource(
    /* [in] */ Int32 resource)
{
    mDropDownResource = resource;

    return NOERROR;
}

//@Override
AutoPtr<IView> SimpleAdapter::GetDropDownView(
    /* [in] */ Int32 position,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent)
{
    return CreateViewFromResource(
        position, convertView, parent, mDropDownResource);
}

ECode SimpleAdapter::BindView(
    /* [in] */ Int32 position,
    /* [in] */ IView* view)
{
    AutoPtr<IObjectStringMap> dataSet = mData[position];
    if (dataSet == NULL) {
        return NOERROR;
    }

    ISimpleAdapterViewBinder* binder = mViewBinder;
    Int32 count = mTo->GetLength();

    for (Int32 i = 0; i < count; i++) {
        AutoPtr<IView> v;
        view->FindViewById((*mTo)[i], (IView**)&v);
        if (v != NULL) {
            AutoPtr<IInterface> data;
            dataSet->Get((*mFrom)[i], (IInterface**)&data);

            String text;
            if (data != NULL) {
                if (ICharSequence::Probe(data) != NULL) {
                    ICharSequence::Probe(data)->ToString(&text);
                }
                else if (IInteger32::Probe(data) == NULL && IBoolean::Probe(data) == NULL){
                    Slogger::I("SimpleAdapter", "item %d, i: %d, %s, view %08x' data problem may exist.",
                        position, i, (*mFrom)[i].string(), (*mTo)[i]);
                }
            }

            if (text.IsNull()) {
                text = "";
            }

            Boolean bound = FALSE;
            if (binder != NULL) {
                binder->SetViewValue(v, data, text, &bound);
            }

            if (!bound) {
                if (ICheckable::Probe(v)) {
                    if (IBoolean::Probe(data)) {
                        IBoolean* ib = IBoolean::Probe(data);
                        Boolean value;
                        ib->GetValue(&value);
                        ICheckable::Probe(v)->SetChecked((Boolean)value);
                    }
                    else if (ITextView::Probe(v)) {
                        // Note: keep the instanceof TextView check at the bottom of these
                        // ifs since a lot of views are TextViews (e.g. CheckBoxes).
                        SetViewText(ITextView::Probe(v), text);
                    }
                    else {
                        Slogger::E("SimpleAdapter", "item %d, view %08x' data should be bound to a IBoolean",
                            position, (*mTo)[i]);
                        //throw new IllegalStateException(v.getClass().getName() +
                        //        " should be bound to a Boolean, not a " +
                        //        (data == NULL ? "<unknown type>" : data.getClass()));
                        return E_ILLEGAL_STATE_EXCEPTION;
                    }
                }
                else if (ITextView::Probe(v)) {
                    // Note: keep the instanceof TextView check at the bottom of these
                    // ifs since a lot of views are TextViews (e.g. CheckBoxes).
                    SetViewText(ITextView::Probe(v), text);
                }
                else if (IImageView::Probe(v)) {
                    if (IInteger32::Probe(data)) {
                        IInteger32* ii = IInteger32::Probe(data);
                        Int32 value;
                        ii->GetValue(&value);
                        SetViewImage(IImageView::Probe(v), value);
                    }
                    else {
                        SetViewImage(IImageView::Probe(v), text);
                    }
                }
                else {
                    Slogger::E("SimpleAdapter", "item %d, view %08x is not a view that can be bounds by this SimpleAdapter.",
                        position, (*mTo)[i]);
                    return E_ILLEGAL_STATE_EXCEPTION;
                }
            }
        }
    }

    return NOERROR;
}

AutoPtr<ISimpleAdapterViewBinder> SimpleAdapter::GetViewBinder()
{
    return mViewBinder;
}

ECode SimpleAdapter::SetViewBinder(
    /* [in] */ ISimpleAdapterViewBinder* viewBinder)
{
    mViewBinder = viewBinder;

    return NOERROR;
}

ECode SimpleAdapter::SetViewImage(
    /* [in] */ IImageView* v,
    /* [in] */ Int32 value)
{
    return v->SetImageResource(value);
}

ECode SimpleAdapter::SetViewImage(
    /* [in] */ IImageView* v,
    /* [in] */ const String& value)
{
    Int32 intValue = StringUtils::ParseInt32(value);
    if (!value.Equals("0") || intValue != 0) {
        v->SetImageResource(intValue);
    }
    else {
        AutoPtr<IUri> uri;
        Uri::Parse(value, (IUri**)&uri);
        v->SetImageURI(uri);
    }

    return NOERROR;
}

ECode SimpleAdapter::SetViewText(
    /* [in] */ ITextView* v,
    /* [in] */ const String& text)
{
    AutoPtr<ICharSequence> cs;
    CStringWrapper::New(text, (ICharSequence**)&cs);

    return v->SetText(cs);
}

AutoPtr<IFilter> SimpleAdapter::GetFilter()
{
    if (mFilter == NULL) {
        mFilter = new SimpleFilter(this);
    }

    return mFilter;
}


} // namespace Widget
} // namespace Droid
} // namespace Elastos
