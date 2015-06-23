
#include "app/CAlertControllerAlertParams.h"
#include <ext/frameworkext.h>

using Elastos::Droid::Content::IDialogInterface;
using Elastos::Droid::View::EIID_ILayoutInflater;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace App {

//==============================================================================
//                  CAlertControllerAlertParams::_RecycleCursorAdapter
//==============================================================================

CAlertControllerAlertParams::_RecycleCursorAdapter::_RecycleCursorAdapter(
    /* [in] */ IContext* context,
    /* [in] */ ICursor* c,
    /* [in] */ Boolean autoRequery,
    /* [in] */ IAlertControllerAlertParams* host,
    /* [in] */ IListView* listView,
    /* [in] */ IAlertController* dialog)
    : CursorAdapter(context, c, autoRequery)
    , mAlertParams(host)
    , mListView(listView)
    , mAlertController(dialog)
{
    InitAdapter();
}

AutoPtr<IView> CAlertControllerAlertParams::_RecycleCursorAdapter::NewView(
    /* [in] */ IContext* context,
    /* [in] */ ICursor* cursor,
    /* [in] */ IViewGroup* parent)
{
    AutoPtr<IView> rst;
    AutoPtr<ILayoutInflater> layoutInfater;
    mAlertParams->GetInflater((ILayoutInflater**)&layoutInfater);
    if (layoutInfater) {
        Int32 layout;
        mAlertController->GetMultiChoiceItemLayout(&layout);
        layoutInfater->Inflate(layout, parent, FALSE, (IView**)&rst);
    }
    return rst;
}

ECode CAlertControllerAlertParams::_RecycleCursorAdapter::BindView(
    /* [in] */ IView* view,
    /* [in] */ IContext* context,
    /* [in] */ ICursor* cursor)
{
    AutoPtr<ICheckedTextView> text;
    view->FindViewById(R::id::text1, (IView**)&text);
    String str;
    AutoPtr<ICharSequence> csq;
    cursor->GetString(mLabelIndex, &str);
    CStringWrapper::New(str, (ICharSequence**)&csq);
    text->SetText(csq);
    Int32 position, checked;
    cursor->GetPosition(&position);
    cursor->GetInt32(mIsCheckedIndex, &checked);
    mListView->SetItemChecked(position, checked == 1);
    return NOERROR;
}

void CAlertControllerAlertParams::_RecycleCursorAdapter::InitAdapter()
{
    String labelColumn, isCheckedColumn;
    mAlertParams->GetLabelColumn(&labelColumn);
    mAlertParams->GetIsCheckedColumn(&isCheckedColumn);

    AutoPtr<ICursor> cursor = GetCursor();
    cursor->GetColumnIndexOrThrow(labelColumn, &mLabelIndex);
    cursor->GetColumnIndexOrThrow(isCheckedColumn, &mIsCheckedIndex);
}

//==============================================================================
//                  CAlertControllerAlertParams::RecycleCursorAdapter
//==============================================================================
PInterface CAlertControllerAlertParams::RecycleCursorAdapter::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(ICursorAdapter*)this;
    }
    else if (riid == EIID_IBaseAdapter) {
        return (ICursorAdapter*)this;
    }
    else if (riid == EIID_IListAdapter) {
        return (ICursorAdapter*)this;
    }
    else if (riid == EIID_IAdapter) {
        return (ICursorAdapter*)this;
    }
    else if (riid == EIID_IFilterable) {
        return (IFilterable*)this;
    }
    else if (riid == EIID_ISpinnerAdapter) {
        return (ISpinnerAdapter*)this;
    }
    else if (riid == EIID_ICursorAdapter) {
        return (ICursorAdapter*)this;
    }

    return NULL;
}
UInt32 CAlertControllerAlertParams::RecycleCursorAdapter::AddRef()
{
    return ElRefBase::AddRef();
}
UInt32 CAlertControllerAlertParams::RecycleCursorAdapter::Release()
{
    return ElRefBase::Release();
}
ECode CAlertControllerAlertParams::RecycleCursorAdapter::GetInterfaceID(
    /* [in] */ IInterface* object,
    /* [out] */ InterfaceID* iid)
{
    VALIDATE_NOT_NULL(iid);
    if (object == (IInterface*)(ICursorAdapter*)this) {
        *iid = EIID_ICursorAdapter ;
    }
    else if(object == (IInterface*)(ISpinnerAdapter*)this) {
        *iid = EIID_ISpinnerAdapter ;
    }
    else if(object == (IInterface*)(IFilterable*)this) {
        *iid = EIID_IFilterable ;
    }
    else {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}
CAlertControllerAlertParams::RecycleCursorAdapter::RecycleCursorAdapter(
    /* [in] */ IContext* context,
    /* [in] */ ICursor* c,
    /* [in] */ Boolean autoRequery,
    /* [in] */ IAlertControllerAlertParams* host,
    /* [in] */ IListView* listView,
    /* [in] */ IAlertController* dialog)
        : _RecycleCursorAdapter(context, c, autoRequery, host, listView, dialog)
{}

ECode CAlertControllerAlertParams::RecycleCursorAdapter::NewView(
    /* [in] */ IContext* context,
    /* [in] */ ICursor* cursor,
    /* [in] */ IViewGroup* parent,
    /* [out] */ IView** view)
{
    AutoPtr<IView> temp = _RecycleCursorAdapter::NewView(context, cursor, parent);
    *view = temp;
    REFCOUNT_ADD(*view);
    return NOERROR;
}

ECode CAlertControllerAlertParams::RecycleCursorAdapter::NewDropDownView(
    /* [in] */ IContext* context,
    /* [in] */ ICursor* cursor,
    /* [in] */ IViewGroup* parent,
    /* [out] */ IView** view)
{
    AutoPtr<IView> temp = _RecycleCursorAdapter::NewDropDownView(context, cursor, parent);
    *view = temp;
    REFCOUNT_ADD(*view);
    return NOERROR;
}

ECode CAlertControllerAlertParams::RecycleCursorAdapter::BindView(
    /* [in] */ IView* view,
    /* [in] */ IContext* context,
    /* [in] */ ICursor* cursor)
{
    return _RecycleCursorAdapter::BindView(view, context, cursor);
}

ECode CAlertControllerAlertParams::RecycleCursorAdapter::GetFilterQueryProvider(
    /* [out] */ IFilterQueryProvider** filterQueryProvider)
{
    VALIDATE_NOT_NULL(*filterQueryProvider);
    AutoPtr<IFilterQueryProvider> temp = _RecycleCursorAdapter::GetFilterQueryProvider();
    *filterQueryProvider = temp;
    REFCOUNT_ADD(*filterQueryProvider);
    return NOERROR;
}

ECode CAlertControllerAlertParams::RecycleCursorAdapter::SetFilterQueryProvider(
    /* [in] */ IFilterQueryProvider* filterQueryProvider)
{
    return _RecycleCursorAdapter::SetFilterQueryProvider(filterQueryProvider);
}

ECode CAlertControllerAlertParams::RecycleCursorAdapter::SwapCursor(
    /* [in] */ ICursor* newCursor,
    /* [out] */ ICursor** cursor)
{
    AutoPtr<ICursor> temp = _RecycleCursorAdapter::SwapCursor(newCursor);
    *cursor = temp;
    REFCOUNT_ADD(*cursor);
    return NOERROR;
}

ECode CAlertControllerAlertParams::RecycleCursorAdapter::NotifyDataSetChanged()
{
    return _RecycleCursorAdapter::NotifyDataSetChanged();
}

ECode CAlertControllerAlertParams::RecycleCursorAdapter::NotifyDataSetInvalidated()
{
    return _RecycleCursorAdapter::NotifyDataSetInvalidated();
}

ECode CAlertControllerAlertParams::RecycleCursorAdapter::AreAllItemsEnabled(
    /* [out] */ Boolean* enabled)
{
    VALIDATE_NOT_NULL(enabled);
    *enabled = _RecycleCursorAdapter::AreAllItemsEnabled();
    return NOERROR;
}

ECode CAlertControllerAlertParams::RecycleCursorAdapter::IsEnabled(
    /* [in] */ Int32 position,
    /* [out] */ Boolean* enabled)
{
    VALIDATE_NOT_NULL(enabled);
    *enabled = _RecycleCursorAdapter::IsEnabled(position);
    return NOERROR;
}

ECode CAlertControllerAlertParams::RecycleCursorAdapter::RegisterDataSetObserver(
    /* [in] */ IDataSetObserver* observer)
{
    return _RecycleCursorAdapter::RegisterDataSetObserver(observer);
}

ECode CAlertControllerAlertParams::RecycleCursorAdapter::UnregisterDataSetObserver(
    /* [in] */ IDataSetObserver* observer)
{
    return _RecycleCursorAdapter::UnregisterDataSetObserver(observer);
}

ECode CAlertControllerAlertParams::RecycleCursorAdapter::GetCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    *count = _RecycleCursorAdapter::GetCount();
    return NOERROR;
}

ECode CAlertControllerAlertParams::RecycleCursorAdapter::GetItem(
    /* [in] */ Int32 position,
    /* [out] */ IInterface** item)
{
    VALIDATE_NOT_NULL(item);
    AutoPtr<IInterface> temp = _RecycleCursorAdapter::GetItem(position);
    *item = temp;
    REFCOUNT_ADD(*item);
    return NOERROR;
}

ECode CAlertControllerAlertParams::RecycleCursorAdapter::GetItemId(
    /* [in] */ Int32 position,
    /* [out] */ Int64* itemId)
{
    VALIDATE_NOT_NULL(itemId);
    *itemId = _RecycleCursorAdapter::GetItemId(position);
    return NOERROR;
}

ECode CAlertControllerAlertParams::RecycleCursorAdapter::HasStableIds(
    /* [out] */ Boolean* hasStableIds)
{
    VALIDATE_NOT_NULL(hasStableIds);
    *hasStableIds = _RecycleCursorAdapter::HasStableIds();
    return NOERROR;
}

ECode CAlertControllerAlertParams::RecycleCursorAdapter::GetView(
    /* [in] */ Int32 position,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent,
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);
    AutoPtr<IView> temp = _RecycleCursorAdapter::GetView(position, convertView, parent);
    *view = temp;
    REFCOUNT_ADD(*view);
    return NOERROR;
}

ECode CAlertControllerAlertParams::RecycleCursorAdapter::GetItemViewType(
    /* [in] */ Int32 position,
    /* [out] */ Int32* viewType)
{
    VALIDATE_NOT_NULL(viewType);
    *viewType = _RecycleCursorAdapter::GetItemViewType(position);
    return NOERROR;
}

ECode CAlertControllerAlertParams::RecycleCursorAdapter::GetViewTypeCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    *count = _RecycleCursorAdapter::GetViewTypeCount();
    return NOERROR;
}

ECode CAlertControllerAlertParams::RecycleCursorAdapter::IsEmpty(
     /* [out] */ Boolean* isEmpty)
{
    VALIDATE_NOT_NULL(isEmpty);
    *isEmpty = _RecycleCursorAdapter::IsEmpty();
    return NOERROR;
}

ECode CAlertControllerAlertParams::RecycleCursorAdapter::GetDropDownView(
    /* [in] */ Int32 position,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent,
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);
    AutoPtr<IView> temp = _RecycleCursorAdapter::GetDropDownView(position, convertView, parent);
    *view = temp;
    REFCOUNT_ADD(*view);
    return NOERROR;
}

ECode CAlertControllerAlertParams::RecycleCursorAdapter::GetFilter(
    /* [out] */ IFilter** filter)
{
    VALIDATE_NOT_NULL(filter);
    AutoPtr<IFilter> temp = _RecycleCursorAdapter::GetFilter();
    *filter = temp;
    REFCOUNT_ADD(*filter);
    return NOERROR;
}

//==============================================================================
//                  CAlertControllerAlertParams::_RecycleArrayAdapter
//==============================================================================

CAlertControllerAlertParams::_RecycleArrayAdapter::_RecycleArrayAdapter(
    /* [in] */ IContext* context,
    /* [in] */ Int32 resource,
    /* [in] */ Int32 textViewResourceId,
    /* [in] */ IObjectContainer* objects,
    /* [in] */ IAlertControllerAlertParams* host,
    /* [in] */ IListView* listView)
    : ArrayAdapter(context, resource, textViewResourceId, objects)
    , mAlertParams(host)
    , mListView(listView)
{}

AutoPtr<IView> CAlertControllerAlertParams::_RecycleArrayAdapter::GetView(
    /* [in] */ Int32 position,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent)
{
    AutoPtr<IView> view = ArrayAdapter::GetView(position, convertView, parent);

    AutoPtr<ArrayOf<Boolean> > checkedItems;
    mAlertParams->GetCheckedItems((ArrayOf<Boolean>**)&checkedItems);
    if (checkedItems != NULL) {
        Boolean isItemChecked = (*checkedItems)[position];
        if (isItemChecked) {
            mListView->SetItemChecked(position, TRUE);
        }
    }

    return view;
}

//==============================================================================
//                  CAlertControllerAlertParams::RecycleArrayAdapter
//==============================================================================

PInterface CAlertControllerAlertParams::RecycleArrayAdapter::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IArrayAdapter) {
        return (IArrayAdapter*)this;
    }
    else if (riid == EIID_IInterface) {
        return (ISpinnerAdapter*)this;
    }
    else if (riid == EIID_IBaseAdapter) {
        return (IArrayAdapter*)this;
    }
    else if (riid == EIID_IListAdapter) {
        return (IArrayAdapter*)this;
    }
    else if (riid == EIID_IAdapter) {
        return (IArrayAdapter*)this;
    }
    else if (riid == EIID_IFilterable) {
        return (IFilterable*)this;
    }
    else if (riid == EIID_ISpinnerAdapter) {
        return (ISpinnerAdapter*)this;
    }

    return NULL;
}
UInt32 CAlertControllerAlertParams::RecycleArrayAdapter::AddRef()
{
    return ElRefBase::AddRef();
}
UInt32 CAlertControllerAlertParams::RecycleArrayAdapter::Release()
{
    return ElRefBase::Release();
}
ECode CAlertControllerAlertParams::RecycleArrayAdapter::GetInterfaceID(
    /* [in] */ IInterface* object,
    /* [out] */ InterfaceID* iid)
{
    VALIDATE_NOT_NULL(iid);
    if (object == (IInterface*)(IArrayAdapter*)this) {
        *iid = EIID_IArrayAdapter ;
    }
    else if(object == (IInterface*)(ISpinnerAdapter*)this) {
        *iid = EIID_ISpinnerAdapter ;
    }
    else if(object == (IInterface*)(IFilterable*)this) {
        *iid = EIID_IFilterable ;
    }
    else {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

CAlertControllerAlertParams::RecycleArrayAdapter::RecycleArrayAdapter(
    /* [in] */ IContext* context,
    /* [in] */ Int32 resource,
    /* [in] */ Int32 textViewResourceId,
    /* [in] */ IObjectContainer* objects,
    /* [in] */ IAlertControllerAlertParams* host,
    /* [in] */ IListView* listView)
    : _RecycleArrayAdapter(context, resource, textViewResourceId, objects, host, listView)
{}

ECode CAlertControllerAlertParams::RecycleArrayAdapter::RegisterDataSetObserver(
    /* [in] */ IDataSetObserver* observer)
{
    return ArrayAdapter::RegisterDataSetObserver(observer);
}

ECode CAlertControllerAlertParams::RecycleArrayAdapter::UnregisterDataSetObserver(
    /* [in] */ IDataSetObserver* observer)
{
    return ArrayAdapter::UnregisterDataSetObserver(observer);
}

ECode CAlertControllerAlertParams::RecycleArrayAdapter::GetCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    *count =  ArrayAdapter::GetCount();
    return NOERROR;
}

ECode CAlertControllerAlertParams::RecycleArrayAdapter::GetItem(
    /* [in] */ Int32 position,
    /* [out] */ IInterface** item)
{
    VALIDATE_NOT_NULL(item);
    AutoPtr<IInterface> temp = ArrayAdapter::GetItem(position);
    *item = temp;
    REFCOUNT_ADD(*item);
    return NOERROR;
}

ECode CAlertControllerAlertParams::RecycleArrayAdapter::GetItemId(
    /* [in] */ Int32 position,
    /* [out] */ Int64* itemId)
{
    VALIDATE_NOT_NULL(itemId);
    *itemId = ArrayAdapter::GetItemId(position);
    return NOERROR;
}

ECode CAlertControllerAlertParams::RecycleArrayAdapter::HasStableIds(
    /* [out] */ Boolean* hasStableIds)
{
    VALIDATE_NOT_NULL(hasStableIds);
    *hasStableIds = ArrayAdapter::HasStableIds();
    return NOERROR;
}

ECode CAlertControllerAlertParams::RecycleArrayAdapter::GetView(
    /* [in] */ Int32 position,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent,
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);
    AutoPtr<IView> temp = ArrayAdapter::GetView(position, convertView, parent);
    *view = temp;
    REFCOUNT_ADD(*view);
    return NOERROR;
}

ECode CAlertControllerAlertParams::RecycleArrayAdapter::GetItemViewType(
    /* [in] */ Int32 position,
    /* [out] */ Int32* viewType)
{
    VALIDATE_NOT_NULL(viewType);
    *viewType = ArrayAdapter::GetItemViewType(position);
    return NOERROR;
}


ECode CAlertControllerAlertParams::RecycleArrayAdapter::GetViewTypeCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    *count = ArrayAdapter::GetViewTypeCount();
    return NOERROR;
}

ECode CAlertControllerAlertParams::RecycleArrayAdapter::IsEmpty(
    /* [out] */ Boolean* isEmpty)
{
    VALIDATE_NOT_NULL(isEmpty);
    *isEmpty = ArrayAdapter::IsEmpty();
    return NOERROR;
}

ECode CAlertControllerAlertParams::RecycleArrayAdapter::NotifyDataSetChanged()
{
    return ArrayAdapter::NotifyDataSetChanged();
}

ECode CAlertControllerAlertParams::RecycleArrayAdapter::NotifyDataSetInvalidated()
{
    return ArrayAdapter::NotifyDataSetInvalidated();
}

ECode CAlertControllerAlertParams::RecycleArrayAdapter::Add(
    /* [in] */ IInterface* object)
{
    return ArrayAdapter::Add(object);
}

ECode CAlertControllerAlertParams::RecycleArrayAdapter::AddAll(
    /* [in] */ IObjectContainer* collection)
{
    return ArrayAdapter::AddAll(collection);
}

ECode CAlertControllerAlertParams::RecycleArrayAdapter::AddAll(
    /* [in] */ ArrayOf<IInterface* >* items)
{
    return ArrayAdapter::AddAll(items);
}

ECode CAlertControllerAlertParams::RecycleArrayAdapter::Insert(
    /* [in] */ IInterface* object,
    /* [in] */ Int32 index)
{
    return ArrayAdapter::Insert(object, index);
}

ECode CAlertControllerAlertParams::RecycleArrayAdapter::Remove(
    /* [in] */ IInterface* object)
{
    return ArrayAdapter::Remove(object);
}

ECode CAlertControllerAlertParams::RecycleArrayAdapter::Clear()
{
    return ArrayAdapter::Clear();
}

ECode CAlertControllerAlertParams::RecycleArrayAdapter::Sort(
    /* [in] */ IComparator* comparator)
{
    return ArrayAdapter::Sort(comparator);
}

ECode CAlertControllerAlertParams::RecycleArrayAdapter::SetNotifyOnChange(
    /* [in] */ Boolean notifyOnChange)
{
    return ArrayAdapter::SetNotifyOnChange(notifyOnChange);
}

ECode CAlertControllerAlertParams::RecycleArrayAdapter::GetContext(
    /* [out] */ IContext** context)
{
    VALIDATE_NOT_NULL(context);
    AutoPtr<IContext> temp = ArrayAdapter::GetContext();
    *context = temp;
    REFCOUNT_ADD(*context);
    return NOERROR;
}

ECode CAlertControllerAlertParams::RecycleArrayAdapter::GetPosition(
    /* [in] */ IInterface* item,
    /* [out] */ Int32* position)
{
    VALIDATE_NOT_NULL(position);
    *position = ArrayAdapter::GetPosition(item);
    return NOERROR;
}

ECode CAlertControllerAlertParams::RecycleArrayAdapter::SetDropDownViewResource(
    /* [in] */ Int32 resource)
{
    return ArrayAdapter::SetDropDownViewResource(resource);
}

ECode CAlertControllerAlertParams::RecycleArrayAdapter::GetDropDownView(
    /* [in] */ Int32 position,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent,
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);
    AutoPtr<IView> temp = ArrayAdapter::GetDropDownView(position, convertView, parent);
    *view = temp;
    REFCOUNT_ADD(*view);
    return NOERROR;
}

ECode CAlertControllerAlertParams::RecycleArrayAdapter::AreAllItemsEnabled(
    /* [out] */ Boolean* enable)
{
    VALIDATE_NOT_NULL(enable);
    *enable = ArrayAdapter::AreAllItemsEnabled();
    return NOERROR;
}

ECode CAlertControllerAlertParams::RecycleArrayAdapter::IsEnabled(
    /* [in] */ Int32 position,
    /* [in] */ Boolean* enable)
{
    VALIDATE_NOT_NULL(enable);
    *enable = ArrayAdapter::IsEnabled(position);
    return NOERROR;
}

ECode CAlertControllerAlertParams::RecycleArrayAdapter::GetFilter(
    /* [out] */ IFilter** filter)
{
    VALIDATE_NOT_NULL(filter);
    AutoPtr<IFilter> temp;
    temp = ArrayAdapter::GetFilter();
    *filter = temp;
    REFCOUNT_ADD(*filter);
    return NOERROR;
}

//==============================================================================
//                  CAlertControllerAlertParams::AdapterListener
//==============================================================================
CAR_INTERFACE_IMPL(CAlertControllerAlertParams::AdapterListener, IAdapterViewOnItemClickListener)

CAlertControllerAlertParams::AdapterListener::AdapterListener(
    /* [in] */ IAlertControllerAlertParams* params,
    /* [in] */ IAlertController* dialog,
    /* [in] */ IListView* listView,
    /* [in] */ Boolean isClick)
    : mAlertParams(params)
    , mAlertController(dialog)
    , mListView(listView)
    , mIsClick(isClick)
{
}

ECode CAlertControllerAlertParams::AdapterListener::OnItemClick(
    /* [in] */ IAdapterView* parent,
    /* [in] */ IView* view,
    /* [in] */ Int32 position,
    /* [in] */ Int64 id)
{
    if (mIsClick) {
        return DoClick(position);
    }
    else {
        return DoClick(position);
    }
}

ECode CAlertControllerAlertParams::AdapterListener::DoClick(
    /* [in] */ Int32 position)
{
    AutoPtr<IDialogInterface> dialogInterface;
    mAlertController->GetDialogInterface((IDialogInterface**)&dialogInterface);

    AutoPtr<IDialogInterfaceOnClickListener> listener;
    mAlertParams->GetOnClickListener((IDialogInterfaceOnClickListener**)&listener);
    if (listener) {
        listener->OnClick(dialogInterface, position);
    }

    Boolean isSingleChoice;
    mAlertParams->GetIsSingleChoice(&isSingleChoice);
    if (!isSingleChoice) {
        if (dialogInterface) dialogInterface->Dismiss();
    }
    return NOERROR;
}

ECode CAlertControllerAlertParams::AdapterListener::DoClick(
    /* [in] */ Int32 position)
{
    Boolean checked = FALSE;
    mListView->IsItemChecked(position, &checked);

    AutoPtr<ArrayOf<Boolean> > checkedItems;
    mAlertParams->GetCheckedItems((ArrayOf<Boolean>**)&checkedItems);
    if (checkedItems != NULL) {
        (*checkedItems)[position] = checked;
    }

    AutoPtr<IDialogInterfaceOnMultiChoiceClickListener> listener;
    mAlertParams->GetOnCheckboxClickListener((IDialogInterfaceOnMultiChoiceClickListener**)&listener);
    if (listener) {
        AutoPtr<IDialogInterface> dialogInterface;
        mAlertController->GetDialogInterface((IDialogInterface**)&dialogInterface);
        listener->OnClick(dialogInterface, position, checked);
    }
    return NOERROR;
}

//==============================================================================
//                  CAlertControllerAlertParams
//==============================================================================

CAlertControllerAlertParams::CAlertControllerAlertParams()
    : mIconId(0)
    , mIconAttrId(0)
    , mCancelable(FALSE)
    , mViewSpacingLeft(0)
    , mViewSpacingTop(0)
    , mViewSpacingRight(0)
    , mViewSpacingBottom(0)
    , mViewSpacingSpecified(FALSE)
    , mIsMultiChoice(FALSE)
    , mIsSingleChoice(FALSE)
    , mCheckedItem(-1)
    , mForceInverseBackground(FALSE)
    , mRecycleOnMeasure(TRUE)
{}

ECode CAlertControllerAlertParams::SetContext(
    /* [in] */ IContext* cxt)
{
    mContext = cxt;
    return NOERROR;
}

ECode CAlertControllerAlertParams::GetContext(
    /* [out] */ IContext** cxt)
{
    VALIDATE_NOT_NULL(cxt);
    *cxt = mContext;
    REFCOUNT_ADD(*cxt);
    return NOERROR;
}

ECode CAlertControllerAlertParams::SetInflater(
    /* [in] */ ILayoutInflater* inflater)
{
    mInflater = inflater;
    return NOERROR;
}

ECode CAlertControllerAlertParams::GetInflater(
    /* [out] */ ILayoutInflater** inflater)
{
    VALIDATE_NOT_NULL(inflater);
    *inflater = mInflater;
    REFCOUNT_ADD(*inflater);
    return NOERROR;
}

ECode CAlertControllerAlertParams::SetIconId(
    /* [in] */ Int32 iconId)
{
    mIconId = iconId;
    return NOERROR;
}

ECode CAlertControllerAlertParams::GetIconId(
    /* [out] */ Int32* iconId)
{
    VALIDATE_NOT_NULL(iconId);
    *iconId = mIconId;
    return NOERROR;
}

ECode CAlertControllerAlertParams::SetIcon(
    /* [in] */ IDrawable* icon)
{
    mIcon = icon;
    return NOERROR;
}

ECode CAlertControllerAlertParams::GetIcon(
    /* [out] */ IDrawable** icon)
{
    VALIDATE_NOT_NULL(icon);
    *icon = mIcon;
    REFCOUNT_ADD(*icon);
    return NOERROR;
}

ECode CAlertControllerAlertParams::SetIconAttrId(
    /* [in] */ Int32 id)
{
    mIconAttrId = id;
    return NOERROR;
}

ECode CAlertControllerAlertParams::GetIconAttrId(
    /* [out] */ Int32* id)
{
    VALIDATE_NOT_NULL(id);
    *id = mIconAttrId;
    return NOERROR;
}

ECode CAlertControllerAlertParams::SetTitle(
    /* [in] */ ICharSequence* title)
{
    mTitle = title;
    return NOERROR;
}

ECode CAlertControllerAlertParams::GetTitle(
    /* [out] */ ICharSequence** title)
{
    VALIDATE_NOT_NULL(title);
    *title = mTitle;
    REFCOUNT_ADD(*title);
    return NOERROR;
}

ECode CAlertControllerAlertParams::SetCustomTitleView(
    /* [in] */ IView* view)
{
    mCustomTitleView = view;
    return NOERROR;
}

ECode CAlertControllerAlertParams::GetCustomTitleView(
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);
    *view = mCustomTitleView;
    REFCOUNT_ADD(*view);
    return NOERROR;
}

ECode CAlertControllerAlertParams::SetMessage(
    /* [in] */ ICharSequence* message)
{
    mMessage = message;
    return NOERROR;
}

ECode CAlertControllerAlertParams::GetMessage(
    /* [out] */ ICharSequence** message)
{
    VALIDATE_NOT_NULL(message);
    *message = mMessage;
    REFCOUNT_ADD(*message);
    return NOERROR;
}

ECode CAlertControllerAlertParams::SetPositiveButtonText(
    /* [in] */ ICharSequence* text)
{
    mPositiveButtonText = text;
    return NOERROR;
}

ECode CAlertControllerAlertParams::GetPositiveButtonText(
    /* [out] */ ICharSequence** text)
{
    VALIDATE_NOT_NULL(text);
    *text = mPositiveButtonText;
    REFCOUNT_ADD(*text);
    return NOERROR;
}

ECode CAlertControllerAlertParams::SetPositiveButtonListener(
    /* [in] */ IDialogInterfaceOnClickListener* listener)
{
    mPositiveButtonListener = listener;
    return NOERROR;
}

ECode CAlertControllerAlertParams::GetPositiveButtonListener(
    /* [out] */ IDialogInterfaceOnClickListener** listener)
{
    VALIDATE_NOT_NULL(listener);
    *listener = mPositiveButtonListener;
    REFCOUNT_ADD(*listener);
    return NOERROR;
}

ECode CAlertControllerAlertParams::SetNegativeButtonText(
    /* [in] */ ICharSequence* text)
{
    mNegativeButtonText = text;
    return NOERROR;
}

ECode CAlertControllerAlertParams::GetNegativeButtonText(
    /* [out] */ ICharSequence** text)
{
    VALIDATE_NOT_NULL(text);
    *text = mNegativeButtonText;
    REFCOUNT_ADD(*text);
    return NOERROR;
}

ECode CAlertControllerAlertParams::SetNegativeButtonListener(
    /* [in] */ IDialogInterfaceOnClickListener* listener)
{
    mNegativeButtonListener = listener;
    return NOERROR;
}

ECode CAlertControllerAlertParams::GetNegativeButtonListener(
    /* [out] */ IDialogInterfaceOnClickListener** listener)
{
    VALIDATE_NOT_NULL(listener);
    *listener = mNegativeButtonListener;
    REFCOUNT_ADD(*listener);
    return NOERROR;
}

ECode CAlertControllerAlertParams::SetNeutralButtonText(
    /* [in] */ ICharSequence* text)
{
    mNeutralButtonText = text;
    return NOERROR;
}

ECode CAlertControllerAlertParams::GetNeutralButtonText(
    /* [out] */ ICharSequence** text)
{
    VALIDATE_NOT_NULL(text);
    *text = mNeutralButtonText;
    REFCOUNT_ADD(*text);
    return NOERROR;
}

ECode CAlertControllerAlertParams::SetNeutralButtonListener(
    /* [in] */ IDialogInterfaceOnClickListener* listener)
{
    mNeutralButtonListener = listener;
    return NOERROR;
}

ECode CAlertControllerAlertParams::GetNeutralButtonListener(
    /* [out] */ IDialogInterfaceOnClickListener** listener)
{
    VALIDATE_NOT_NULL(listener);
    *listener = mNeutralButtonListener;
    REFCOUNT_ADD(*listener);
    return NOERROR;
}

ECode CAlertControllerAlertParams::SetCancelable(
    /* [in] */ Boolean cancelable)
{
    mCancelable = cancelable;
    return NOERROR;
}

ECode CAlertControllerAlertParams::GetCancelable(
    /* [out] */ Boolean* cancelable)
{
    VALIDATE_NOT_NULL(cancelable);
    *cancelable = mCancelable;
    return NOERROR;
}

ECode CAlertControllerAlertParams::SetOnCancelListener(
    /* [in] */ IDialogInterfaceOnCancelListener* listener)
{
    mOnCancelListener = listener;
    return NOERROR;
}

ECode CAlertControllerAlertParams::GetOnCancelListener(
    /* [out] */ IDialogInterfaceOnCancelListener** listener)
{
    VALIDATE_NOT_NULL(listener);
    *listener = mOnCancelListener;
    REFCOUNT_ADD(*listener);
    return NOERROR;
}

ECode CAlertControllerAlertParams::SetOnDismissListener(
    /* [in] */ IDialogInterfaceOnDismissListener* listener)
{
    mOnDismissListener = listener;
    return NOERROR;
}

ECode CAlertControllerAlertParams::GetOnDismissListener(
    /* [out] */ IDialogInterfaceOnDismissListener** listener)
{
    VALIDATE_NOT_NULL(listener);
    *listener = mOnDismissListener;
    REFCOUNT_ADD(*listener);
    return NOERROR;
}

ECode CAlertControllerAlertParams::SetOnKeyListener(
    /* [in] */ IDialogInterfaceOnKeyListener* listener)
{
    mOnKeyListener = listener;
    return NOERROR;
}

ECode CAlertControllerAlertParams::GetOnKeyListener(
    /* [out] */ IDialogInterfaceOnKeyListener** listener)
{
    VALIDATE_NOT_NULL(listener);
    *listener = mOnKeyListener;
    REFCOUNT_ADD(*listener);
    return NOERROR;
}

ECode CAlertControllerAlertParams::SetItems(
    /* [in] */ ArrayOf<ICharSequence*>* items)
{
    mItems = items;
    return NOERROR;
}

ECode CAlertControllerAlertParams::GetItems(
    /* [out, callee] */ ArrayOf<ICharSequence*>** items)
{
    VALIDATE_NOT_NULL(items);
    *items = mItems;
    REFCOUNT_ADD(*items);
    return NOERROR;
}

ECode CAlertControllerAlertParams::SetAdapter(
    /* [in] */ IListAdapter* adapter)
{
    mAdapter = adapter;
    return NOERROR;
}

ECode CAlertControllerAlertParams::GetAdapter(
    /* [out] */ IListAdapter** adapter)
{
    VALIDATE_NOT_NULL(adapter);
    *adapter = mAdapter;
    REFCOUNT_ADD(*adapter);
    return NOERROR;
}

ECode CAlertControllerAlertParams::SetOnClickListener(
    /* [in] */ IDialogInterfaceOnClickListener* listener)
{
    mOnClickListener = listener;
    return NOERROR;
}

ECode CAlertControllerAlertParams::GetOnClickListener(
    /* [out] */ IDialogInterfaceOnClickListener** listener)
{
    VALIDATE_NOT_NULL(listener);
    *listener = mOnClickListener;
    REFCOUNT_ADD(*listener);
    return NOERROR;
}

ECode CAlertControllerAlertParams::SetView(
    /* [in] */ IView* view)
{
    mView = view;
    return NOERROR;
}

ECode CAlertControllerAlertParams::GetView(
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);
    *view = mView;
    REFCOUNT_ADD(*view);
    return NOERROR;
}

ECode CAlertControllerAlertParams::SetViewSpacingLeft(
    /* [in] */ Int32 left)
{
    mViewSpacingLeft = left;
    return NOERROR;
}

ECode CAlertControllerAlertParams::GetViewSpacingLeft(
    /* [out] */ Int32* left)
{
    VALIDATE_NOT_NULL(left);
    *left = mViewSpacingLeft;
    return NOERROR;
}

ECode CAlertControllerAlertParams::SetViewSpacingTop(
    /* [in] */ Int32 top)
{
    mViewSpacingTop = top;
    return NOERROR;
}

ECode CAlertControllerAlertParams::GetViewSpacingTop(
    /* [out] */ Int32* top)
{
    VALIDATE_NOT_NULL(top);
    *top = mViewSpacingTop;
    return NOERROR;
}

ECode CAlertControllerAlertParams::SetViewSpacingRight(
    /* [in] */ Int32 right)
{
    mViewSpacingRight = right;
    return NOERROR;
}

ECode CAlertControllerAlertParams::GetViewSpacingRight(
    /* [out] */ Int32* right)
{
    VALIDATE_NOT_NULL(right);
    *right = mViewSpacingRight;
    return NOERROR;
}

ECode CAlertControllerAlertParams::SetViewSpacingBottom(
    /* [in] */ Int32 bottom)
{
    mViewSpacingBottom = bottom;
    return NOERROR;
}

ECode CAlertControllerAlertParams::GetViewSpacingBottom(
    /* [out] */ Int32* bottom)
{
    VALIDATE_NOT_NULL(bottom);
    *bottom = mViewSpacingBottom;
    return NOERROR;
}

ECode CAlertControllerAlertParams::SetViewSpacingSpecified(
    /* [in] */ Boolean value)
{
    mViewSpacingSpecified = value;
    return NOERROR;
}

ECode CAlertControllerAlertParams::GetViewSpacingSpecified(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value);
    *value = mViewSpacingSpecified;
    return NOERROR;
}

ECode CAlertControllerAlertParams::SetCheckedItems(
    /* [in] */ ArrayOf<Boolean>* items)
{
    mCheckedItems = items;
    return NOERROR;
}

ECode CAlertControllerAlertParams::GetCheckedItems(
    /* [out, callee] */ ArrayOf<Boolean>** items)
{
    VALIDATE_NOT_NULL(items);
    *items = mCheckedItems;
    REFCOUNT_ADD(*items);
    return NOERROR;
}

ECode CAlertControllerAlertParams::SetIsMultiChoice(
    /* [in] */ Boolean isMutiChoice)
{
    mIsMultiChoice = isMutiChoice;
    return NOERROR;
}

ECode CAlertControllerAlertParams::GetIsMultiChoice(
    /* [out] */ Boolean* isMutiChoice)
{
    VALIDATE_NOT_NULL(isMutiChoice);
    *isMutiChoice = mIsMultiChoice;
    return NOERROR;
}

ECode CAlertControllerAlertParams::SetIsSingleChoice(
    /* [in] */ Boolean isSingleChoice)
{
    mIsSingleChoice = isSingleChoice;
    return NOERROR;
}

ECode CAlertControllerAlertParams::GetIsSingleChoice(
    /* [out] */ Boolean* isSingleChoice)
{
    VALIDATE_NOT_NULL(isSingleChoice);
    *isSingleChoice = mIsSingleChoice;
    return NOERROR;
}

ECode CAlertControllerAlertParams::SetCheckedItem(
    /* [in] */ Int32 checkedItem)
{
    mCheckedItem = checkedItem;
    return NOERROR;
}

ECode CAlertControllerAlertParams::GetCheckedItem(
    /* [out] */ Int32* checkedItem)
{
    VALIDATE_NOT_NULL(checkedItem);
    *checkedItem = mCheckedItem;
    return NOERROR;
}

ECode CAlertControllerAlertParams::SetOnCheckboxClickListener(
    /* [in] */ IDialogInterfaceOnMultiChoiceClickListener* listener)
{
    mOnCheckboxClickListener = listener;
    return NOERROR;
}

ECode CAlertControllerAlertParams::GetOnCheckboxClickListener(
    /* [out] */ IDialogInterfaceOnMultiChoiceClickListener** listener)
{
    VALIDATE_NOT_NULL(listener);
    *listener = mOnCheckboxClickListener;
    REFCOUNT_ADD(*listener);
    return NOERROR;
}

ECode CAlertControllerAlertParams::SetCursor(
    /* [in] */ ICursor* cursor)
{
    mCursor = cursor;
    return NOERROR;
}

ECode CAlertControllerAlertParams::GetCursor(
    /* [out] */ ICursor** cursor)
{
    VALIDATE_NOT_NULL(cursor);
    *cursor = mCursor;
    REFCOUNT_ADD(*cursor);
    return NOERROR;
}

ECode CAlertControllerAlertParams::SetLabelColumn(
    /* [in] */ const String& labelColumn)
{
    mLabelColumn = labelColumn;
    return NOERROR;
}

ECode CAlertControllerAlertParams::GetLabelColumn(
    /* [out] */ String* labelColumn)
{
    VALIDATE_NOT_NULL(labelColumn);
    *labelColumn = mLabelColumn;
    return NOERROR;
}

ECode CAlertControllerAlertParams::SetIsCheckedColumn(
    /* [in] */ const String& column)
{
    mIsCheckedColumn = column;
    return NOERROR;
}

ECode CAlertControllerAlertParams::GetIsCheckedColumn(
    /* [out] */ String* column)
{
    VALIDATE_NOT_NULL(column);
    *column = mIsCheckedColumn;
    return NOERROR;
}

ECode CAlertControllerAlertParams::SetForceInverseBackground(
    /* [in] */ Boolean value)
{
    mForceInverseBackground = value;
    return NOERROR;
}

ECode CAlertControllerAlertParams::GetForceInverseBackground(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value);
    *value = mForceInverseBackground;
    return NOERROR;
}

ECode CAlertControllerAlertParams::SetOnItemSelectedListener(
    /* [in] */ IAdapterViewOnItemSelectedListener* listener)
{
    mOnItemSelectedListener = listener;
    return NOERROR;
}

ECode CAlertControllerAlertParams::GetOnItemSelectedListener(
    /* [out] */ IAdapterViewOnItemSelectedListener** listener)
{
    VALIDATE_NOT_NULL(listener);
    *listener = mOnItemSelectedListener;
    REFCOUNT_ADD(*listener);
    return NOERROR;
}

ECode CAlertControllerAlertParams::SetOnPrepareListViewListener(
    /* [in] */ IAlertControllerAlertParamsOnPrepareListViewListener* listener)
{
    mOnPrepareListViewListener = listener;
    return NOERROR;
}

ECode CAlertControllerAlertParams::GetOnPrepareListViewListener(
    /* [out] */ IAlertControllerAlertParamsOnPrepareListViewListener** listener)
{
    VALIDATE_NOT_NULL(listener);
    *listener = mOnPrepareListViewListener;
    REFCOUNT_ADD(*listener);
    return NOERROR;
}

ECode CAlertControllerAlertParams::SetRecycleOnMeasure(
    /* [in] */ Boolean recycleOnMeasure)
{
    mRecycleOnMeasure = recycleOnMeasure;
    return NOERROR;
}

ECode CAlertControllerAlertParams::GetRecycleOnMeasure(
    /* [out] */ Boolean* recycleOnMeasure)
{
    VALIDATE_NOT_NULL(recycleOnMeasure);
    *recycleOnMeasure = mRecycleOnMeasure;
    return NOERROR;
}

ECode CAlertControllerAlertParams::constructor(
    /* [in] */ IContext* context)
{
    mContext = context;
    mCancelable = TRUE;
    AutoPtr<IInterface> temp;
    context->GetSystemService(IContext::LAYOUT_INFLATER_SERVICE, (IInterface**)&temp);
    mInflater = ILayoutInflater::Probe(temp.Get());
    return NOERROR;
}

ECode CAlertControllerAlertParams::Apply(
    /* [in] */ IAlertController* dialog)
{
    if (mCustomTitleView != NULL) {
        dialog->SetCustomTitle(mCustomTitleView);
    }
    else {
        if (mTitle != NULL) {
            dialog->SetTitle(mTitle);
        }
        if (mIcon != NULL) {
            dialog->SetIcon(mIcon);
        }
        if (mIconId >= 0) {
            dialog->SetIcon(mIconId);
        }
        if (mIconAttrId > 0) {
        	Int32 resId;
        	dialog->GetIconAttributeResId(mIconAttrId, &resId);
            dialog->SetIcon(resId);
        }
    }
    if (mMessage != NULL) {
        dialog->SetMessage(mMessage);
    }

    if (mPositiveButtonText != NULL) {
        dialog->SetButton(IDialogInterface::BUTTON_POSITIVE, mPositiveButtonText,
                mPositiveButtonListener, NULL);
    }
    if (mNegativeButtonText != NULL) {
        dialog->SetButton(IDialogInterface::BUTTON_NEGATIVE, mNegativeButtonText,
                mNegativeButtonListener, NULL);
    }
    if (mNeutralButtonText != NULL) {
        dialog->SetButton(IDialogInterface::BUTTON_NEUTRAL, mNeutralButtonText,
                mNeutralButtonListener, NULL);
    }
    if (mForceInverseBackground) {
        dialog->SetInverseBackgroundForced(TRUE);
    }

    // For a list, the client can either supply an array of items or an
    // adapter or a cursor
    if ((mItems != NULL) || (mCursor != NULL) || (mAdapter != NULL)) {
        CreateListView(dialog);
    }

    if (mView != NULL) {
        if (mViewSpacingSpecified) {
            dialog->SetView(mView, mViewSpacingLeft, mViewSpacingTop, mViewSpacingRight,
                    mViewSpacingBottom);
        } else {
            dialog->SetView(mView);
        }
    }

    /*
    dialog.setCancelable(mCancelable);
    dialog.setOnCancelListener(mOnCancelListener);
    if (mOnKeyListener != null) {
        dialog.setOnKeyListener(mOnKeyListener);
    }
    */
    return NOERROR;
}

void CAlertControllerAlertParams::CreateListView(
    /* [in] */ IAlertController* dialog)
{
    Int32 listLayout, listItemLayout, multiChoiceItemLayout, singleChoiceItemLayout;
    dialog->GetSingleChoiceItemLayout(&singleChoiceItemLayout);
    dialog->GetMultiChoiceItemLayout(&multiChoiceItemLayout);
    dialog->GetListLayout(&listLayout);
    dialog->GetListItemLayout(&listItemLayout);

    AutoPtr<IView> tempView;
    mInflater->Inflate(listLayout, NULL, (IView**)&tempView);
    AutoPtr<IRecycleListView> listView = IRecycleListView::Probe(tempView.Get());
    assert(listView != NULL);

    AutoPtr<IListAdapter> adapter;
    if (mIsMultiChoice) {
        if (mCursor == NULL) {
            AutoPtr<IObjectContainer> dataList;
            CParcelableObjectContainer::New((IObjectContainer**)&dataList);
            for (Int32 i = 0; i < mItems->GetLength(); ++i) {
                AutoPtr<ICharSequence> seq = (*mItems)[i];
                dataList->Add(seq->Probe(EIID_IInterface));
            }
            adapter = new RecycleArrayAdapter(mContext, multiChoiceItemLayout, R::id::text1, dataList, this, listView);
        }
        else {
            adapter = new RecycleCursorAdapter(mContext, mCursor, FALSE, this, listView, dialog);
        }
    }
    else {
        Int32 layout = mIsSingleChoice ? singleChoiceItemLayout : listItemLayout;
        if (mCursor == NULL) {
            if (mAdapter != NULL) {
                adapter = mAdapter;
            }
            else {
                AutoPtr<IObjectContainer> dataList;
                CParcelableObjectContainer::New((IObjectContainer**)&dataList);
                for (Int32 i = 0; i < mItems->GetLength(); ++i) {
                    AutoPtr<ICharSequence> seq = (*mItems)[i];
                    dataList->Add(seq->Probe(EIID_IInterface));
                }
                CArrayAdapter::New(mContext, layout, R::id::text1, dataList, (IArrayAdapter**)&adapter);
            }

        } else {
            AutoPtr<ArrayOf<String> > sParams = ArrayOf<String>::Alloc(1);
            AutoPtr<ArrayOf<Int32> > iParams = ArrayOf<Int32>::Alloc(1);
            (*sParams)[0] = mLabelColumn;
            (*iParams)[0] = R::id::text1;
            CSimpleCursorAdapter::New(mContext, layout, mCursor, sParams, iParams, (ISimpleCursorAdapter**)&adapter);
        }
    }

    if (mOnPrepareListViewListener != NULL) {
        mOnPrepareListViewListener->OnPrepareListView(listView);
    }

    /* Don't directly set the adapter on the ListView as we might
     * want to add a footer to the ListView later.
     */
    dialog->SetAdapter(adapter);
    dialog->SetCheckedItem(mCheckedItem);

    if (mOnClickListener != NULL) {
        AutoPtr<IAdapterViewOnItemClickListener> l = new AdapterListener(
            (IAlertControllerAlertParams*)this, dialog, listView, TRUE);
        listView->SetOnItemClickListener(l);
    }
    else if (mOnCheckboxClickListener != NULL) {
        AutoPtr<IAdapterViewOnItemClickListener> l = new AdapterListener(
            (IAlertControllerAlertParams*)this, dialog, listView, FALSE);
        listView->SetOnItemClickListener(l);
    }

    // Attach a given OnItemSelectedListener to the ListView
    if (mOnItemSelectedListener != NULL) {
        listView->SetOnItemSelectedListener(mOnItemSelectedListener);
    }

    if (mIsSingleChoice) {
        listView->SetChoiceMode(IListView::CHOICE_MODE_SINGLE);
    }
    else if (mIsMultiChoice) {
        listView->SetChoiceMode(IListView::CHOICE_MODE_MULTIPLE);
    }

    listView->SetRecycleOnMeasure(mRecycleOnMeasure);
    dialog->SetListView(listView);
}


} // namespace App
} // namespace Internal
} // namespace Droid
} // namespace Elastos
