#include "ext/frameworkext.h"
#include "widget/ArrayAdapter.h"
#include <elastos/core/StringUtils.h>
#include <elastos/utility/logging/Logger.h>

using Elastos::Core::StringUtils;
using Elastos::Core::CStringWrapper;
using Elastos::Utility::Logging::Logger;
using Elastos::Droid::Content::EIID_IContext;

namespace Elastos{
namespace Droid{
namespace Widget{

ArrayAdapter::ArrayFilter::ArrayFilter(
    /* [in] */ ArrayAdapter* host)
    : mHost(host)
{}

CAR_INTERFACE_IMPL(ArrayAdapter::ArrayFilter, IFilter);

/**
 * Provide an interface that decides how Int64 to delay the message for a given query.  Useful
 * for heuristics such as posting a delay for the delete key to avoid doing any work while the
 * user holds down the delete key.
 *
 * @param delayer The delayer.
 * @hide
 */
ECode ArrayAdapter::ArrayFilter::SetDelayer(
    /* [in] */ IFilterDelayer* delayer)
{
    return Filter::SetDelayer(delayer);
}

/**
 * <p>Starts an asynchronous filtering operation. Calling this method
 * cancels all previous non-executed filtering requests and posts a new
 * filtering request that will be executed later.</p>
 *
 * @param constraint the constraint used to filter the data
 *
 * @see #filter(CharSequence, android.widget.Filter.FilterListener)
 */
ECode ArrayAdapter::ArrayFilter::DoFilter(
    /* [in] */ ICharSequence* constraint)
{
    return Filter::DoFilter(constraint);
}

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
ECode ArrayAdapter::ArrayFilter::DoFilter(
    /* [in] */ ICharSequence* constraint,
    /* [in] */ IFilterListener* listener)
{
    return Filter::DoFilter(constraint, listener);
}

/**
 * <p>Converts a value from the filtered set into a CharSequence. Subclasses
 * should override this method to convert their results. The default
 * implementation returns an empty String for NULL values or the default
 * String representation of the value.</p>
 *
 * @param resultValue the value to convert to a CharSequence
 * @return a CharSequence representing the value
 */
ECode ArrayAdapter::ArrayFilter::ConvertResultToString(
    /* [in] */ IInterface* resultValue,
    /* [out] */ ICharSequence** cs)
{
    VALIDATE_NOT_NULL(cs);
    return Filter::ConvertResultToString(resultValue, cs);
}

//@Override
ECode ArrayAdapter::ArrayFilter::PerformFiltering(
    /* [in] */ ICharSequence* prefix,
    /* [out] */ IFilterResults** filterResults)
{
    VALIDATE_NOT_NULL(filterResults);

    AutoPtr<IFilterResults> results = new FilterResults();

    if (!mHost->mOriginalValues) {
        Mutex::Autolock lock(mHost->mLock);
        mHost->mOriginalValues = new List<AutoPtr<IInterface> >(
            mHost->mObjects.Begin(), mHost->mObjects.End());
    }

    Int32 length = -1;
    if (prefix) {
        prefix->GetLength(&length);
    }

    if (prefix == NULL || length == 0) {
        Mutex::Autolock lock(mHost->mLock);

        AutoPtr<IObjectContainer> container;
        CObjectContainer::New((IObjectContainer**)&container);

        Int32 size = mHost->mOriginalValues->GetSize();
        for(Int32 i = 0; i < size; i++) {
            container->Add((*(mHost->mOriginalValues))[i]);
        }

//        ((FilterResults*)results.Get())->mValues = container;
        results->SetValues(container);
        results->SetCount(size);
    }
    else {
        String prefixString;
        prefix->ToString(&prefixString);

        AutoPtr< List<AutoPtr<IInterface> > > values;
        {
            Mutex::Autolock lock(mHost->mLock);
            values = new List<AutoPtr<IInterface> >(*mHost->mOriginalValues);
        }
        AutoPtr<IObjectContainer> newValues;
        CObjectContainer::New((IObjectContainer**)&newValues);

        List<AutoPtr<IInterface> >::Iterator iter = values->Begin();
        for (; iter != values->End(); ++iter) {
            AutoPtr<IInterface> value = *iter;
            String valueText("");
            if (ICharSequence::Probe(value))
            {
                ICharSequence::Probe(value)->ToString(&valueText);
            }

            // First match against the whole, non-splitted value
            if (valueText.StartWithIgnoreCase(prefixString)) {
                newValues->Add(value);
            }
            else {
                AutoPtr<ArrayOf<String> > words;
                StringUtils::Split(valueText, String(" "), (ArrayOf<String>**)&words);
                if (words != NULL) {
                    // Start at index 0, in case valueText starts with space(s)
                    for (Int32 k = 0; k < words->GetLength(); ++k) {
                        if ((*words)[k].StartWithIgnoreCase(prefixString)) {
                            newValues->Add(value);
                            break;
                        }
                    }
                }
            }
        }

        results->SetValues(newValues);
        Int32 newCount;
        newValues->GetObjectCount(&newCount);
        results->SetCount(newCount);
    }

    *filterResults = results;
    REFCOUNT_ADD(*filterResults);
    return NOERROR;
}

//@Override
ECode ArrayAdapter::ArrayFilter::PublishResults(
    /* [in] */ ICharSequence* constraint,
    /* [in] */ IFilterResults* results)
{
    assert(results);
    FilterResults* filterResults = (FilterResults*)results;
    AutoPtr<IObjectContainer> container;
    AutoPtr<IInterface> temp = filterResults->mValues;
    if(temp->Probe(EIID_IObjectContainer))
    {
        container = (IObjectContainer*)temp->Probe(EIID_IObjectContainer);
    }else {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    //noinspection unchecked
    if(mHost->mObjects.GetSize())
    {
        mHost->mObjects.Clear();
    }
    AutoPtr<IObjectEnumerator> objEnumerator;
    FAIL_RETURN(container->GetObjectEnumerator((IObjectEnumerator**)&objEnumerator));
    Boolean hasNext = FALSE;
    while ((objEnumerator->MoveNext(&hasNext), hasNext)) {
        temp = NULL;
        objEnumerator->Current((IInterface**)&temp);
        mHost->mObjects.PushBack(temp);
    }
    if (filterResults->mCount > 0) {
        mHost->NotifyDataSetChanged();
    }
    else {
        mHost->NotifyDataSetInvalidated();
    }

    return NOERROR;
}

ArrayAdapter::ArrayAdapter()
    : mResource(0)
    , mDropDownResource(0)
    , mFieldId(0)
    , mNotifyOnChange(TRUE)
    , mOriginalValues(NULL)
{}

/**
 * Constructor
 *
 * @param context The current context.
 * @param textViewResourceId The resource ID for a layout file containing a TextView to use when
 *                 instantiating views.
 */
ArrayAdapter::ArrayAdapter(
    /* [in] */ IContext* context,
    /* [in] */ Int32 textViewResourceId)
    : mResource(0)
    , mDropDownResource(0)
    , mFieldId(0)
    , mNotifyOnChange(TRUE)
    , mOriginalValues(NULL)
{
    Init(context, textViewResourceId, 0);
}

/**
 * Constructor
 *
 * @param context The current context.
 * @param resource The resource ID for a layout file containing a layout to use when
 *                 instantiating views.
 * @param textViewResourceId The id of the TextView within the layout resource to be populated
 */
ArrayAdapter::ArrayAdapter(
    /* [in] */ IContext* context,
    /* [in] */ Int32 resource,
    /* [in] */ Int32 textViewResourceId)
    : mResource(0)
    , mDropDownResource(0)
    , mFieldId(0)
    , mNotifyOnChange(TRUE)
    , mOriginalValues(NULL)
{
    Init(context, resource, textViewResourceId);
}

/**
 * Constructor
 *
 * @param context The current context.
 * @param textViewResourceId The resource ID for a layout file containing a TextView to use when
 *                 instantiating views.
 * @param objects The objects to represent in the ListView.
 */
ArrayAdapter::ArrayAdapter(
    /* [in] */ IContext* context,
    /* [in] */ Int32 textViewResourceId,
    /* [in] */ IObjectContainer* objects)
    : mResource(0)
    , mDropDownResource(0)
    , mFieldId(0)
    , mNotifyOnChange(TRUE)
    , mOriginalValues(NULL)
{
    Init(context, textViewResourceId, 0, objects);
}

/**
 * Constructor
 *
 * @param context The current context.
 * @param resource The resource ID for a layout file containing a layout to use when
 *                 instantiating views.
 * @param textViewResourceId The id of the TextView within the layout resource to be populated
 * @param objects The objects to represent in the ListView.
 */
ArrayAdapter::ArrayAdapter(
    /* [in] */ IContext* context,
    /* [in] */ Int32 resource,
    /* [in] */ Int32 textViewResourceId,
    /* [in] */ IObjectContainer* objects)
    : mResource(0)
    , mDropDownResource(0)
    , mFieldId(0)
    , mNotifyOnChange(TRUE)
    , mOriginalValues(NULL)
{
    Init(context, resource, textViewResourceId, objects);
}

ArrayAdapter::~ArrayAdapter()
{
    mOriginalValues = NULL;
}

/**
 * Adds the specified object at the end of the array.
 *
 * @param object The object to add at the end of the array.
 */
ECode ArrayAdapter::Add(
    /* [in] */ IInterface* object)
{
    {
        Mutex::Autolock lock(mLock);
        if (mOriginalValues != NULL) {
            mOriginalValues->PushBack(object);
        } else {
            mObjects.PushBack(object);
        }
    }

    if (mNotifyOnChange) NotifyDataSetChanged();

    return NOERROR;
}

ECode ArrayAdapter::AddAll(
    /* [in] */ IObjectContainer* collection)
{
    assert(collection != NULL);
    {
        Mutex::Autolock lock(mLock);
        AutoPtr<IObjectEnumerator> ator;
        collection->GetObjectEnumerator((IObjectEnumerator**)&ator);
        Boolean hasNext = FALSE;

        if (mOriginalValues != NULL) {
            while (ator->MoveNext(&hasNext), hasNext)
            {
                AutoPtr<IInterface> obj;
                ator->Current((IInterface**)&obj);
                mOriginalValues->PushBack(obj);
            }
        }
        else {
            while (ator->MoveNext(&hasNext), hasNext)
            {
                AutoPtr<IInterface> obj;
                ator->Current((IInterface**)&obj);
                mObjects.PushBack(obj);
            }
        }
    }

    if (mNotifyOnChange) NotifyDataSetChanged();

    return NOERROR;
}

ECode ArrayAdapter::AddAll(
    /* [in] */ ArrayOf<IInterface* >* items)
{
    {
        Mutex::Autolock lock(mLock);
        if (mOriginalValues != NULL) {
            assert(items != NULL);
            if (items->GetLength() > 0) {
                for (Int32 i = 0; i < items->GetLength(); i++) {
                    mOriginalValues->PushBack((*items)[i]);
                }
            }
        }
        else {
            assert(items != NULL);
            if (items->GetLength() > 0) {
                for (Int32 i = 0; i < items->GetLength(); i++) {
                    mObjects.PushBack((*items)[i]);
                }
            }
        }
    }

    if (mNotifyOnChange) NotifyDataSetChanged();

    return NOERROR;
}


/**
 * Inserts the specified object at the specified index in the array.
 *
 * @param object The object to insert into the array.
 * @param index The index at which the object must be inserted.
 */
ECode ArrayAdapter::Insert(
    /* [in] */ IInterface* object,
    /* [in] */ Int32 index)
{
    {
        Mutex::Autolock lock(mLock);
        if (mOriginalValues != NULL) {
            mOriginalValues->Insert(index, object);
        } else {
            mObjects.Insert(index, object);
        }
    }
    if (mNotifyOnChange) NotifyDataSetChanged();

    return NOERROR;
}

/**
 * Removes the specified object from the array.
 *
 * @param object The object to remove.
 */
ECode ArrayAdapter::Remove(
    /* [in] */ IInterface* object)
{
    {
        Mutex::Autolock lock(mLock);
        if (mOriginalValues != NULL) {
            mOriginalValues->Remove(object);
        } else {
            mObjects.Remove(object);
        }
    }
    if (mNotifyOnChange) NotifyDataSetChanged();

    return NOERROR;
}

/**
 * Remove all elements from the list.
 */
ECode ArrayAdapter::Clear()
{
    {
        Mutex::Autolock lock(mLock);
        if (mOriginalValues != NULL) {
            mOriginalValues->Clear();
        } else {
            mObjects.Clear();
        }
    }
    if (mNotifyOnChange) NotifyDataSetChanged();

    return NOERROR;
}

/**
 * Sorts the content of this adapter using the specified comparator.
 *
 * @param comparator The comparator used to sort the objects contained
 *        in this adapter.
 */
ECode ArrayAdapter::Sort(
    /* [in] */ IComparator* comparator)
{
    {
        Mutex::Autolock lock(mLock);
        if (mOriginalValues != NULL) {
            assert(0);
            Sort(mOriginalValues, comparator);
        } else {
           Sort(&mObjects, comparator);
        }
    }

    if (mNotifyOnChange) NotifyDataSetChanged();

    return NOERROR;
}

ECode ArrayAdapter::Sort(
    /* [in] */ List<AutoPtr<IInterface> >* list,
    /* [in] */ IComparator* comparator)
{
    Int32 size = list->GetSize() - 1;
    for(Int32 i = 0; i < size; i++)
    {
        for(Int32 j = 0; j < size -i; j++)
        {
            Int32 compare;
            comparator->Compare((*list)[j], (*list)[j+1], &compare);
            if(compare > 0)
            {
                AutoPtr<IInterface> temp = (*list)[j];
                (*list)[j] = (*list)[j+1];
                (*list)[j+1] = temp;
            }
        }
    }
    return NOERROR;
}
/**
 * {@inheritDoc}
 */
//@Override
ECode ArrayAdapter::NotifyDataSetChanged()
{
    BaseAdapter::NotifyDataSetChanged();
    mNotifyOnChange = TRUE;

    return NOERROR;
}

/**
 * Control whether methods that change the list ({@link #add},
 * {@link #insert}, {@link #remove}, {@link #clear}) automatically call
 * {@link #notifyDataSetChanged}.  If set to false, caller must
 * manually call notifyDataSetChanged() to have the changes
 * reflected in the attached view.
 *
 * The default is TRUE, and calling notifyDataSetChanged()
 * resets the flag to TRUE.
 *
 * @param notifyOnChange if TRUE, modifications to the list will
 *                       automatically call {@link
 *                       #notifyDataSetChanged}
 */
ECode ArrayAdapter::SetNotifyOnChange(
    /* [in] */ Boolean notifyOnChange)
{
    mNotifyOnChange = notifyOnChange;

    return NOERROR;
}

ECode ArrayAdapter::Init(
    /* [in] */ IContext* context,
    /* [in] */ Int32 resource,
    /* [in] */ Int32 textViewResourceId,
    /* [in] */ IObjectContainer* objects/* = NULL*/)
{
    assert(context);
    AutoPtr<IWeakReferenceSource> wrs = IWeakReferenceSource::Probe(context);
    assert(wrs != NULL && "Error: Invalid context, IWeakReferenceSource not implemented!");
    wrs->GetWeakReference((IWeakReference**)&mWeakContext);

    context->GetSystemService(
        IContext::LAYOUT_INFLATER_SERVICE, (IInterface**)&mInflater);
    mResource = mDropDownResource = resource;
    mFieldId = textViewResourceId;

    if(mObjects.GetSize())
    {
        mObjects.Clear();
    }
    if (objects) {
        AutoPtr<IObjectEnumerator> it;
        objects->GetObjectEnumerator((IObjectEnumerator**)&it);
        Boolean hasNext;
        while (it->MoveNext(&hasNext), hasNext) {
            AutoPtr<IInterface> obj;
            it->Current((IInterface**)&obj);
            mObjects.PushBack(obj);
        }
    }
    return NOERROR;
}

/**
 * Returns the context associated with this array adapter. The context is used
 * to create views from the resource passed to the constructor.
 *
 * @return The Context associated with this adapter.
 */
AutoPtr<IContext> ArrayAdapter::GetContext()
{
    AutoPtr<IContext> context;
    mWeakContext->Resolve(EIID_IContext, (IInterface**)&context);
    return context;
}

/**
 * {@inheritDoc}
 */
Int32 ArrayAdapter::GetCount()
{
    return mObjects.GetSize();
}

/**
 * {@inheritDoc}
 */
AutoPtr<IInterface> ArrayAdapter::GetItem(
    /* [in] */ Int32 position)
{
    return mObjects[position];
}

/**
 * Returns the position of the specified item in the array.
 *
 * @param item The item to retrieve the position of.
 *
 * @return The position of the specified item.
 */
Int32 ArrayAdapter::GetPosition(
    /* [in] */ IInterface* item)
{
    List<AutoPtr<IInterface> >::Iterator iter = mObjects.Begin();
    for (Int32 i = 0; iter != mObjects.End(); ++iter, ++i) {
        if ((*iter).Get() == item) {
            return i;
        }
    }

    return -1;
}

/**
 * {@inheritDoc}
 */
Int64 ArrayAdapter::GetItemId(
    /* [in] */ Int32 position)
{
    return position;
}

/**
 * {@inheritDoc}
 */
AutoPtr<IView> ArrayAdapter::GetView(
    /* [in] */ Int32 position,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent)
{
    return CreateViewFromResource(position, convertView, parent, mResource);
}

AutoPtr<IView> ArrayAdapter::CreateViewFromResource(
    /* [in] */ Int32 position,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent,
    /* [in] */ Int32 resource)
{
    AutoPtr<IView> view;
    AutoPtr<ITextView> text;

    if (convertView == NULL) {
        ECode ec = mInflater->Inflate(resource, parent, FALSE, (IView**)&view);
        if (FAILED(ec) || view == NULL) {
            Logger::E("ArrayAdapter", "Error: failed to inflate view with : "
                "position=%d, convertView=%p, parent=%p, resource=%08x, ec=%08x",
                position, convertView, parent, resource, ec);
        }
    }
    else {
        view = convertView;
    }

    //try {
        if (mFieldId == 0) {
            //  If no custom field is assigned, assume the whole resource is a TextView
            text = ITextView::Probe(view);
        }
        else {
            //  Otherwise, find the TextView field within the layout
            AutoPtr<IView> temp;
            view->FindViewById(mFieldId, (IView**)&temp);
            text = ITextView::Probe(temp);
        }

        if (text == NULL) {
            Logger::E("ArrayAdapter", "Failed to create view from resource: position=%d, resource=%08x, fieldId=%08x",
                position, resource, mFieldId);
            return NULL;
        }
    //} catch (ClassCastException e) {
    //    Log.e("ArrayAdapter", "You must supply a resource ID for a TextView");
    //    throw new IllegalStateException(
    //            "ArrayAdapter requires the resource ID to be a TextView", e);
    //}

    AutoPtr<IInterface> item = GetItem(position);
    AutoPtr<ICharSequence> cs = ICharSequence::Probe(item);
    if (cs) {
        text->SetText(cs);
    }
    else {
        IObject* obj = IObject::Probe(item);
        if (obj != NULL) {
            String value;
            obj->ToString(&value);
            CStringWrapper::New(value, (ICharSequence**)&cs);
            text->SetText(cs);
        }
    }

// #if defined(_DEBUG) || defined(_ELASTOS_DEBUG)
//     Int32 viewId = 0;
//     if (view) {
//         view->GetId(&viewId);
//     }
//     String info("");
//     if (cs) cs->ToString(&info);
//     Logger::D("ArrayAdapter", "Created View: %p, id: %08x, resource: %08x, position: %d, text: [%s]",
//         view.Get(), viewId, resource, position, info.string());
// #endif
    assert(view != NULL);
    return view;
}

/**
 * <p>Sets the layout resource to create the drop down views.</p>
 *
 * @param resource the layout resource defining the drop down views
 * @see #getDropDownView(Int32, android.view.View, android.view.ViewGroup)
 */
ECode ArrayAdapter::SetDropDownViewResource(
    /* [in] */ Int32 resource)
{
    mDropDownResource = resource;

    return NOERROR;
}

/**
 * {@inheritDoc}
 */
//@Override
AutoPtr<IView> ArrayAdapter::GetDropDownView(
    /* [in] */ Int32 position,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent)
{
    return CreateViewFromResource(
        position, convertView, parent, mDropDownResource);
}

/**
 * {@inheritDoc}
 */
AutoPtr<IFilter> ArrayAdapter::GetFilter()
{
    if (mFilter == NULL) {
        mFilter = new ArrayFilter(this);
    }

    return mFilter;
}


}// namespace Widget
}// namespace Droid
}// namespace Elastos

