
#include "CPreferenceActivityHeaderAdapter.h"
#include "text/TextUtils.h"
#include "R.h"

using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Text::TextUtils;
using Elastos::Droid::Widget::EIID_IAdapter;

namespace Elastos {
namespace Droid {
namespace Preference {

//====================================================
// CPreferenceActivityHeaderAdapter::HeaderViewHolder
//====================================================

CAR_INTERFACE_IMPL(CPreferenceActivityHeaderAdapter::HeaderViewHolder, IInterface)


//====================================================
// CPreferenceActivityHeaderAdapter
//====================================================

ECode CPreferenceActivityHeaderAdapter::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IObjectContainer* objects)
{
    ArrayAdapter::Init(context, 0, 0, objects);
    context->GetSystemService(IContext::LAYOUT_INFLATER_SERVICE, (IInterface**)&mInflater);
    return NOERROR;
}

ECode CPreferenceActivityHeaderAdapter::RegisterDataSetObserver(
    /* [in] */ IDataSetObserver* observer)
{
    return ArrayAdapter::RegisterDataSetObserver(observer);
}

ECode CPreferenceActivityHeaderAdapter::UnregisterDataSetObserver(
    /* [in] */ IDataSetObserver* observer)
{
    return ArrayAdapter::UnregisterDataSetObserver(observer);
}

ECode CPreferenceActivityHeaderAdapter::GetCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count)
    *count = ArrayAdapter::GetCount();
    return NOERROR;
}

ECode CPreferenceActivityHeaderAdapter::GetItem(
    /* [in] */ Int32 position,
    /* [out] */ IInterface** item)
{
    VALIDATE_NOT_NULL(item)
    *item = ArrayAdapter::GetItem(position);
    INTERFACE_ADDREF(*item)
    return NOERROR;
}

ECode CPreferenceActivityHeaderAdapter::GetItemId(
    /* [in] */ Int32 position,
    /* [out] */ Int64* itemId)
{
    VALIDATE_NOT_NULL(itemId)
    *itemId = ArrayAdapter::GetItemId(position);
    return NOERROR;
}

ECode CPreferenceActivityHeaderAdapter::HasStableIds(
    /* [out] */ Boolean* hasStableIds)
{
    VALIDATE_NOT_NULL(hasStableIds)
    *hasStableIds = ArrayAdapter::HasStableIds();
    return NOERROR;
}

ECode CPreferenceActivityHeaderAdapter::GetView(
    /* [in] */ Int32 position,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent,
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view)
    AutoPtr<HeaderViewHolder> holder;
    AutoPtr<IView> lView;

    if (convertView == NULL) {
        mInflater->InflateEx2(R::layout::preference_header_item, parent, FALSE, (IView**)&lView);
        holder = new HeaderViewHolder();
        AutoPtr<IView> tempView;
        lView->FindViewById(R::id::icon, (IView**)&tempView);
        holder->mIcon = IImageView::Probe(tempView);
        tempView = NULL;
        lView->FindViewById(R::id::title, (IView**)&tempView);
        holder->mTitle = ITextView::Probe(tempView);
        tempView = NULL;
        lView->FindViewById(R::id::summary, (IView**)&tempView);
        holder->mSummary = ITextView::Probe(tempView);
        tempView = NULL;
        lView->SetTag(holder);
    }
    else {
        lView = convertView;
        AutoPtr<IInterface> inface;
        lView->GetTag((IInterface**)&inface);
        holder = (HeaderViewHolder*)inface.Get();
    }

    // All view fields must be updated every time, because the view may be recycled
    AutoPtr<IInterface> obj;
    GetItem(position, (IInterface**)&obj);
    AutoPtr<IPreferenceActivityHeader> header = IPreferenceActivityHeader::Probe(obj);
    Int32 iconRes;
    header->GetIconRes(&iconRes);
    holder->mIcon->SetImageResource(iconRes);
    AutoPtr<IContext> context;
    GetContext((IContext**)&context);
    AutoPtr<IResources> resource;
    context->GetResources((IResources**)&resource);
    AutoPtr<ICharSequence> title;
    header->GetTitleEx(resource, (ICharSequence**)&title);
    holder->mTitle->SetText(title);
    AutoPtr<ICharSequence> summary;
    header->GetSummaryEx(resource, (ICharSequence**)&summary);\
    if (!TextUtils::IsEmpty(summary)) {
        holder->mSummary->SetVisibility(IView::VISIBLE);
        holder->mSummary->SetText(summary);
    }
    else {
        holder->mSummary->SetVisibility(IView::GONE);
    }

    *view = lView;
    INTERFACE_ADDREF(*view)

    return NOERROR;
}

ECode CPreferenceActivityHeaderAdapter::GetItemViewType(
    /* [in] */ Int32 position,
    /* [out] */ Int32* viewType)
{
    VALIDATE_NOT_NULL(viewType)
    *viewType = ArrayAdapter::GetItemViewType(position);
    return NOERROR;
}

ECode CPreferenceActivityHeaderAdapter::GetViewTypeCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count)
    *count = ArrayAdapter::GetViewTypeCount();
    return NOERROR;
}

ECode CPreferenceActivityHeaderAdapter::IsEmpty(
    /* [out] */ Boolean* isEmpty)
{
    VALIDATE_NOT_NULL(isEmpty)
    *isEmpty = ArrayAdapter::IsEmpty();
    return NOERROR;
}

ECode CPreferenceActivityHeaderAdapter::NotifyDataSetChanged()
{
    return ArrayAdapter::NotifyDataSetChanged();
}

ECode CPreferenceActivityHeaderAdapter::NotifyDataSetInvalidated()
{
    return ArrayAdapter::NotifyDataSetInvalidated();
}

ECode CPreferenceActivityHeaderAdapter::Add(
    /* [in] */ IInterface* object)
{
    return ArrayAdapter::Add(object);
}

ECode CPreferenceActivityHeaderAdapter::AddAll(
    /* [in] */ IObjectContainer* collection)
{
    return ArrayAdapter::AddAll(collection);
}

ECode CPreferenceActivityHeaderAdapter::AddAllEx(
    /* [in] */ ArrayOf<IInterface*>* items)
{
    return ArrayAdapter::AddAllEx(items);
}

ECode CPreferenceActivityHeaderAdapter::Insert(
    /* [in] */ IInterface* object,
    /* [in] */ Int32 index)
{
    return ArrayAdapter::Insert(object, index);
}

ECode CPreferenceActivityHeaderAdapter::Remove(
    /* [in] */ IInterface* object)
{
    return ArrayAdapter::Remove(object);
}

ECode CPreferenceActivityHeaderAdapter::Clear()
{
    return ArrayAdapter::Clear();
}

ECode CPreferenceActivityHeaderAdapter::Sort(
    /* [in] */ IComparator* comparator)
{
    return ArrayAdapter::Sort(comparator);
}

ECode CPreferenceActivityHeaderAdapter::SetNotifyOnChange(
    /* [in] */ Boolean notifyOnChange)
{
    return ArrayAdapter::SetNotifyOnChange(notifyOnChange);
}

ECode CPreferenceActivityHeaderAdapter::GetContext(
    /* [out] */ IContext** context)
{
    VALIDATE_NOT_NULL(context)
    *context = ArrayAdapter::GetContext();
    INTERFACE_ADDREF(*context)
    return NOERROR;
}

ECode CPreferenceActivityHeaderAdapter::GetPosition(
    /* [in] */ IInterface* item,
    /* [out] */ Int32* position)
{
    VALIDATE_NOT_NULL(*position)
    *position = ArrayAdapter::GetPosition(item);
    return NOERROR;
}

ECode CPreferenceActivityHeaderAdapter::SetDropDownViewResource(
    /* [in] */ Int32 resource)
{
    return ArrayAdapter::SetDropDownViewResource(resource);
}

ECode CPreferenceActivityHeaderAdapter::GetDropDownView(
    /* [in] */ Int32 position,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent,
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view)
    *view = ArrayAdapter::GetDropDownView(position, convertView, parent);
    INTERFACE_ADDREF(*view)
    return NOERROR;
}

ECode CPreferenceActivityHeaderAdapter::AreAllItemsEnabled(
    /* [out] */ Boolean* enable)
{
    VALIDATE_NOT_NULL(enable)
    *enable = ArrayAdapter::AreAllItemsEnabled();
    return NOERROR;
}

ECode CPreferenceActivityHeaderAdapter::IsEnabled(
    /* [in] */ Int32 position,
    /* [out] */ Boolean* enable)
{
    VALIDATE_NOT_NULL(enable)
    *enable = ArrayAdapter::IsEnabled(position);
    return NOERROR;
}

ECode CPreferenceActivityHeaderAdapter::GetFilter(
    /* [out] */ IFilter** filter)
{
    VALIDATE_NOT_NULL(filter)
    *filter = ArrayAdapter::GetFilter();
    INTERFACE_ADDREF(*filter)
    return NOERROR;
}

}// namespace Preference
}// namespace Droid
}// namespace Elastos
