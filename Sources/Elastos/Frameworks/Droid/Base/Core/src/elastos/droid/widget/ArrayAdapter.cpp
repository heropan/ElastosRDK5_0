
#include "elastos/droid/widget/ArrayAdapter.h"
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

ECode ArrayAdapter::ArrayFilter::PerformFiltering(
    /* [in] */ ICharSequence* prefix,
    /* [out] */ IFilterResults** filterResults)
{
    VALIDATE_NOT_NULL(filterResults);

    AutoPtr<IFilterResults> results = new FilterResults();

    if (!mHost->mOriginalValues) {
        AutoLock lock(mHost->mLock);
        mHost->mOriginalValues = new List<AutoPtr<IInterface> >(
            mHost->mObjects.Begin(), mHost->mObjects.End());
    }

    Int32 length = -1;
    if (prefix) {
        prefix->GetLength(&length);
    }

    if (prefix == NULL || length == 0) {
        AutoLock lock(mHost->mLock);

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
            AutoLock lock(mHost->mLock);
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

CAR_INTERFACE_IMPL_2(ArrayAdapter, BaseAdapter, IArrayAdapter, IFilterable);
ArrayAdapter::ArrayAdapter()
    : mResource(0)
    , mDropDownResource(0)
    , mFieldId(0)
    , mNotifyOnChange(TRUE)
    , mOriginalValues(NULL)
{}

ECode ArrayAdapter::constructor(
    /* [in] */ IContext* context,
    /* [in] */ Int32 textViewResourceId)
{
    init(context, resource, 0, new ArrayList<T>());
}

ECode ArrayAdapter::constructor(
    /* [in] */ IContext* context,
    /* [in] */ Int32 resource,
    /* [in] */ Int32 textViewResourceId)
{
    init(context, resource, textViewResourceId, new ArrayList<T>());
}

ECode ArrayAdapter::constructor(
    /* [in] */ IContext* context,
    /* [in] */ Int32 textViewResourceId,
    /* [in] */ ArrayOf<IInterface*>* objects)
{
    init(context, resource, 0, Arrays.asList(objects));
}

ECode ArrayAdapter::constructor(
    /* [in] */ IContext* context,
    /* [in] */ Int32 resource,
    /* [in] */ Int32 textViewResourceId,
    /* [in] */ ArrayOf<IInterface*>* objects)
{
    init(context, resource, textViewResourceId, Arrays.asList(objects));
}

ECode ArrayAdapter::constructor(
    /* [in] */ IContext* ctx,
    /* [in] */ Int32 resource,
    /* [in] */ IList* objects)
{
    init(context, resource, 0, objects);
}

ECode ArrayAdapter::constructor(
    /* [in] */ IContext* context,
    /* [in] */ Int32 resource,
    /* [in] */ Int32 textViewResourceId,
    /* [in] */ IList* objects)
{
    init(context, resource, textViewResourceId, objects);
}

ArrayAdapter::~ArrayAdapter()
{
    mOriginalValues = NULL;
}

ECode ArrayAdapter::Add(
    /* [in] */ IInterface* object)
{
    {
        AutoLock lock(mLock);
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
    /* [in] */ ICollection* collection)
{
    assert(collection != NULL);
    {
        AutoLock lock(mLock);
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
        AutoLock lock(mLock);
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

ECode ArrayAdapter::Insert(
    /* [in] */ IInterface* object,
    /* [in] */ Int32 index)
{
    {
        AutoLock lock(mLock);
        if (mOriginalValues != NULL) {
            mOriginalValues->Insert(index, object);
        } else {
            mObjects.Insert(index, object);
        }
    }
    if (mNotifyOnChange) NotifyDataSetChanged();

    return NOERROR;
}

ECode ArrayAdapter::Remove(
    /* [in] */ IInterface* object)
{
    {
        AutoLock lock(mLock);
        if (mOriginalValues != NULL) {
            mOriginalValues->Remove(object);
        } else {
            mObjects.Remove(object);
        }
    }
    if (mNotifyOnChange) NotifyDataSetChanged();

    return NOERROR;
}

ECode ArrayAdapter::Clear()
{
    {
        AutoLock lock(mLock);
        if (mOriginalValues != NULL) {
            mOriginalValues->Clear();
        } else {
            mObjects.Clear();
        }
    }
    if (mNotifyOnChange) NotifyDataSetChanged();

    return NOERROR;
}

ECode ArrayAdapter::Sort(
    /* [in] */ IComparator* comparator)
{
    {
        AutoLock lock(mLock);
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

ECode ArrayAdapter::NotifyDataSetChanged()
{
    BaseAdapter::NotifyDataSetChanged();
    mNotifyOnChange = TRUE;

    return NOERROR;
}

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
    /* [in] */ IList* objects/* = NULL*/)
{
    assert(context);
    // AutoPtr<IWeakReferenceSource> wrs = IWeakReferenceSource::Probe(context);
    // assert(wrs != NULL && "Error: Invalid context, IWeakReferenceSource not implemented!");
    // wrs->GetWeakReference((IWeakReference**)&mWeakContext);
    mContext = context;

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

ECode ArrayAdapter::GetContext(
    /* [out] */ IContext** context)
{
    VALIDATE_NOT_NULL(context);
    // return mWeakContext->Resolve(EIID_IContext, (IInterface**)&context);
    *context = mContext;
    REFCOUNT_ADD(*context);
    return NOERROR;
}

ECode ArrayAdapter::GetCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    *count = mObjects.GetSize();
    return NOERROR;
}

ECode ArrayAdapter::GetItem(
    /* [in] */ Int32 position,
    /* [out] */ IInterface** item)
{
    VALIDATE_NOT_NULL(item);
    *item = mObjects[position];
    REFCOUNT_ADD(*item);
    return NOERROR;
}

ECode ArrayAdapter::GetPosition(
    /* [in] */ IInterface* item,
    /* [out] */ Int32* position)
{
    VALIDATE_NOT_NULL(position);
    List<AutoPtr<IInterface> >::Iterator iter = mObjects.Begin();
    for (Int32 i = 0; iter != mObjects.End(); ++iter, ++i) {
        if ((*iter).Get() == item) {
            *position = i;
            return NOERROR;
        }
    }

    *position = -1;
    return NOERROR;
}

ECode ArrayAdapter::GetItemId(
    /* [in] */ Int32 position,
    /* [out] */ Int64* itemId)
{
    VALIDATE_NOT_NULL(itemId);
    *itemId = position;
    return NOERROR;
}

ECode ArrayAdapter::GetView(
    /* [in] */ Int32 position,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent,
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);
    AutoPtr<IView> v = CreateViewFromResource(position, convertView, parent, mResource);
    *view = v;
    REFCOUNT_ADD(*view);
    return NOERROR;
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

ECode ArrayAdapter::SetDropDownViewResource(
    /* [in] */ Int32 resource)
{
    mDropDownResource = resource;

    return NOERROR;
}

ECode ArrayAdapter::GetDropDownView(
    /* [in] */ Int32 position,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent,
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);
    AutoPtr<IView> v = CreateViewFromResource(position, convertView, parent, mDropDownResource);
    *view = v;
    REFCOUNT_ADD(*view);
    return NOERROR;
}

ECode ArrayAdapter::GetFilter(
    /* [out] */ IFilter** filter)
{
    VALIDATE_NOT_NULL(filter);
    if (mFilter == NULL) {
        mFilter = new ArrayFilter(this);
    }

    *filter = mFilter;
    REFCOUNT_ADD(*filter);
    return NOERROR;
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
