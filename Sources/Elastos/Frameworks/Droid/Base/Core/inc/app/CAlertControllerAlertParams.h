
#ifndef __ELASTOS_DROID_INTERNAL_APP_CALERTCONTROLLERALERTPARAMS_H__
#define __ELASTOS_DROID_INTERNAL_APP_CALERTCONTROLLERALERTPARAMS_H__

#include "_Elastos_Droid_Internal_App_CAlertControllerAlertParams.h"
#include "app/CAlertController.h"
#include "widget/ArrayAdapter.h"
#include "widget/CArrayAdapter.h"
#include "widget/CursorAdapter.h"
#include "widget/CRecycleListView.h"
#include "widget/CSimpleCursorAdapter.h"
#include "util/CParcelableObjectContainer.h"

using Elastos::Core::ICharSequence;
using Elastos::Core::CString;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::ILayoutInflater;
using Elastos::Droid::Widget::ArrayAdapter;
using Elastos::Droid::Widget::IArrayAdapter;
using Elastos::Droid::Widget::CArrayAdapter;
using Elastos::Droid::Widget::EIID_IArrayAdapter;
using Elastos::Droid::Widget::CursorAdapter;
using Elastos::Droid::Widget::ICursorAdapter;
using Elastos::Droid::Widget::EIID_ICursorAdapter;
using Elastos::Droid::Widget::IListAdapter;
using Elastos::Droid::Widget::EIID_IListAdapter;
using Elastos::Droid::Widget::ISpinnerAdapter;
using Elastos::Droid::Widget::EIID_ISpinnerAdapter;
using Elastos::Droid::Widget::IBaseAdapter;
using Elastos::Droid::Widget::EIID_IBaseAdapter;
using Elastos::Droid::Widget::IAdapter;
using Elastos::Droid::Widget::EIID_IAdapter;
using Elastos::Droid::Widget::IFilterable;
using Elastos::Droid::Widget::EIID_IFilterable;
using Elastos::Droid::Widget::IAdapterViewOnItemSelectedListener;
using Elastos::Droid::Widget::CRecycleListView;
using Elastos::Droid::Widget::IRecycleListView;
using Elastos::Droid::Widget::IListView;
using Elastos::Droid::Widget::IFilterQueryProvider;
using Elastos::Droid::Widget::ICheckedTextView;
using Elastos::Droid::Widget::IAdapterView;
using Elastos::Droid::Widget::IAdapterViewOnItemClickListener;
using Elastos::Droid::Widget::CSimpleCursorAdapter;
using Elastos::Droid::Widget::ISimpleCursorAdapter;
using Elastos::Droid::Widget::EIID_IAdapterViewOnItemClickListener;
using Elastos::Droid::Utility::CParcelableObjectContainer;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IDialogInterfaceOnKeyListener;
using Elastos::Droid::Content::IDialogInterfaceOnClickListener;
using Elastos::Droid::Content::IDialogInterfaceOnCancelListener;
using Elastos::Droid::Content::IDialogInterfaceOnDismissListener;
using Elastos::Droid::Content::IDialogInterfaceOnMultiChoiceClickListener;
using Elastos::Droid::Database::ICursor;
using Elastos::Droid::Graphics::Drawable::IDrawable;
using Elastos::Droid::Internal::App::IAlertControllerAlertParamsOnPrepareListViewListener;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace App {


CarClass(CAlertControllerAlertParams)
{
private:

    class _RecycleArrayAdapter : public ArrayAdapter
    {
    public:
        _RecycleArrayAdapter(
            /* [in] */ IContext* context,
            /* [in] */ Int32 resource,
            /* [in] */ Int32 textViewResourceId,
            /* [in] */ IObjectContainer* objects,
            /* [in] */ IAlertControllerAlertParams* host,
            /* [in] */ IListView* listView);

        CARAPI_(AutoPtr<IView>) GetView(
            /* [in] */ Int32 position,
            /* [in] */ IView* convertView,
            /* [in] */ IViewGroup* parent);
    protected:
        _RecycleArrayAdapter(){}
    private:
        AutoPtr<IAlertControllerAlertParams> mAlertParams;
        AutoPtr<IListView> mListView;
    };

    class RecycleArrayAdapter
        : public _RecycleArrayAdapter
        , public IArrayAdapter
        , public ISpinnerAdapter
        , public IFilterable
        , public ElRefBase
    {
    public:
        CAR_INTERFACE_DECL()

        RecycleArrayAdapter(
            /* [in] */ IContext* context,
            /* [in] */ Int32 resource,
            /* [in] */ Int32 textViewResourceId,
            /* [in] */ IObjectContainer* objects,
            /* [in] */ IAlertControllerAlertParams* host,
            /* [in] */ IListView* listView);

        CARAPI RegisterDataSetObserver(
            /* [in] */ IDataSetObserver* observer);

        CARAPI UnregisterDataSetObserver(
            /* [in] */ IDataSetObserver* observer);

        CARAPI GetCount(
            /* [out] */ Int32* count);

        CARAPI GetItem(
            /* [in] */ Int32 position,
            /* [out] */ IInterface** item);

        CARAPI GetItemId(
            /* [in] */ Int32 position,
            /* [out] */ Int64* itemId);

        CARAPI HasStableIds(
            /* [out] */ Boolean* hasStableIds);

        CARAPI GetView(
            /* [in] */ Int32 position,
            /* [in] */ IView* convertView,
            /* [in] */ IViewGroup* parent,
            /* [out] */ IView** view);

        CARAPI GetItemViewType(
            /* [in] */ Int32 position,
            /* [out] */ Int32* viewType);

        CARAPI GetViewTypeCount(
            /* [out] */ Int32* count);

        CARAPI IsEmpty(
            /* [out] */ Boolean* isEmpty);

        CARAPI NotifyDataSetChanged();

        CARAPI NotifyDataSetInvalidated();

        CARAPI Add(
            /* [in] */ IInterface* object);

        CARAPI AddAll(
            /* [in] */ IObjectContainer* collection);

        CARAPI AddAll(
            /* [in] */ ArrayOf<IInterface* >* items) ;

        CARAPI Insert(
            /* [in] */ IInterface* object,
            /* [in] */ Int32 index);

        CARAPI Remove(
            /* [in] */ IInterface* object);

        CARAPI Clear();

        CARAPI Sort(
            /* [in] */ IComparator* comparator);

        CARAPI SetNotifyOnChange(
            /* [in] */ Boolean notifyOnChange);

        CARAPI GetContext(
            /* [out] */ IContext** context);

        CARAPI GetPosition(
            /* [in] */ IInterface* item,
            /* [out] */ Int32* position);

        CARAPI SetDropDownViewResource(
            /* [in] */ Int32 resource);

        CARAPI GetDropDownView(
            /* [in] */ Int32 position,
            /* [in] */ IView* convertView,
            /* [in] */ IViewGroup* parent,
            /* [out] */ IView** view);

        CARAPI AreAllItemsEnabled(
            /* [out] */ Boolean* enable);

        CARAPI IsEnabled(
            /* [in] */ Int32 position,
            /* [in] */ Boolean* enable);

        CARAPI GetFilter(
            /* [out] */ IFilter** filter);
    };

    class _RecycleCursorAdapter : public CursorAdapter
    {
    public:
        _RecycleCursorAdapter(
            /* [in] */ IContext* context,
            /* [in] */ ICursor* c,
            /* [in] */ Boolean autoRequery,
            /* [in] */ IAlertControllerAlertParams* host,
            /* [in] */ IListView* listView,
            /* [in] */ IAlertController* dialog);

        virtual CARAPI_(AutoPtr<IView>) NewView(
            /* [in] */ IContext* context,
            /* [in] */ ICursor* cursor,
            /* [in] */ IViewGroup* parent);

        virtual CARAPI BindView(
            /* [in] */ IView* view,
            /* [in] */ IContext* context,
            /* [in] */ ICursor* cursor);
    private:
        CARAPI_(void) InitAdapter();

        Int32 mLabelIndex;
        Int32 mIsCheckedIndex;
        AutoPtr<IAlertControllerAlertParams> mAlertParams;
        AutoPtr<IListView> mListView;
        AutoPtr<IAlertController> mAlertController;
    };

    class RecycleCursorAdapter
        : public _RecycleCursorAdapter
        , public ICursorAdapter
        , public ISpinnerAdapter
        , public IFilterable
        , public ElRefBase
    {
    public:
        CAR_INTERFACE_DECL()

        RecycleCursorAdapter(
            /* [in] */ IContext* context,
            /* [in] */ ICursor* c,
            /* [in] */ Boolean autoRequery,
            /* [in] */ IAlertControllerAlertParams* host,
            /* [in] */ IListView* listView,
            /* [in] */ IAlertController* dialog);

        CARAPI NewView(
            /* [in] */ IContext* context,
            /* [in] */ ICursor* cursor,
            /* [in] */ IViewGroup* parent,
            /* [out] */ IView** view);

        CARAPI NewDropDownView(
            /* [in] */ IContext* context,
            /* [in] */ ICursor* cursor,
            /* [in] */ IViewGroup* parent,
            /* [out] */ IView** view);

        CARAPI BindView(
            /* [in] */ IView* view,
            /* [in] */ IContext* context,
            /* [in] */ ICursor* cursor);

        CARAPI GetFilterQueryProvider(
            /* [out] */ IFilterQueryProvider** filterQueryProvider);

        CARAPI SetFilterQueryProvider(
            /* [in] */ IFilterQueryProvider* filterQueryProvider);

        CARAPI SwapCursor(
            /* [in] */ ICursor* newCursor,
            /* [out] */ ICursor** cursor);

        CARAPI NotifyDataSetChanged();

        CARAPI NotifyDataSetInvalidated();

        CARAPI AreAllItemsEnabled(
            /* [out] */ Boolean* enabled);

        CARAPI IsEnabled(
            /* [in] */ Int32 position,
            /* [out] */ Boolean* enabled);

        CARAPI RegisterDataSetObserver(
            /* [in] */ IDataSetObserver* observer);

        CARAPI UnregisterDataSetObserver(
            /* [in] */ IDataSetObserver* observer);

        CARAPI GetCount(
            /* [out] */ Int32* count);

        CARAPI GetItem(
            /* [in] */ Int32 position,
            /* [out] */ IInterface** item);

        CARAPI GetItemId(
            /* [in] */ Int32 position,
            /* [out] */ Int64* itemId);

        CARAPI HasStableIds(
            /* [out] */ Boolean* hasStableIds);

        CARAPI GetView(
            /* [in] */ Int32 position,
            /* [in] */ IView* convertView,
            /* [in] */ IViewGroup* parent,
            /* [out] */ IView** view);

        CARAPI GetItemViewType(
            /* [in] */ Int32 position,
            /* [out] */ Int32* viewType);

        CARAPI GetViewTypeCount(
            /* [out] */ Int32* count);

        CARAPI IsEmpty(
             /* [out] */ Boolean* isEmpty);

        CARAPI GetDropDownView(
            /* [in] */ Int32 position,
            /* [in] */ IView* convertView,
            /* [in] */ IViewGroup* parent,
            /* [out] */ IView** view);

        CARAPI GetFilter(
            /* [out] */ IFilter** filter);
    };

    class AdapterListener
        : public IAdapterViewOnItemClickListener
        , public ElRefBase
    {
    public:
        CAR_INTERFACE_DECL()

        AdapterListener(
            /* [in] */ IAlertControllerAlertParams* params,
            /* [in] */ IAlertController* dialog,
            /* [in] */ IListView* listView,
            /* [in] */ Boolean isClick);

        CARAPI OnItemClick(
            /* [in] */ IAdapterView* parent,
            /* [in] */ IView* view,
            /* [in] */ Int32 position,
            /* [in] */ Int64 id);

        CARAPI DoClick(
            /* [in] */ Int32 position);

        CARAPI DoClick(
            /* [in] */ Int32 position);
    private:
        AutoPtr<IAlertControllerAlertParams> mAlertParams;
        AutoPtr<IAlertController> mAlertController;
        AutoPtr<IListView> mListView;
        Boolean mIsClick;
    };

public:
    CAlertControllerAlertParams();

    CARAPI SetContext(
        /* [in] */ IContext* cxt);

    CARAPI GetContext(
        /* [out] */ IContext** cxt);

    CARAPI SetInflater(
        /* [in] */ ILayoutInflater* inflater);

    CARAPI GetInflater(
        /* [out] */ ILayoutInflater** inflater);

    CARAPI SetIconId(
        /* [in] */ Int32 iconId);

    CARAPI GetIconId(
        /* [out] */ Int32* iconId);

    CARAPI SetIcon(
        /* [in] */ IDrawable* icon);

    CARAPI GetIcon(
        /* [out] */ IDrawable** icon);

    CARAPI SetIconAttrId(
        /* [in] */ Int32 id);

    CARAPI GetIconAttrId(
        /* [out] */ Int32* id);

    CARAPI SetTitle(
        /* [in] */ ICharSequence* title);

    CARAPI GetTitle(
        /* [out] */ ICharSequence** title);

    CARAPI SetCustomTitleView(
        /* [in] */ IView* view);

    CARAPI GetCustomTitleView(
        /* [out] */ IView** view);

    CARAPI SetMessage(
        /* [in] */ ICharSequence* message);

    CARAPI GetMessage(
        /* [out] */ ICharSequence** message);

    CARAPI SetPositiveButtonText(
        /* [in] */ ICharSequence* text);

    CARAPI GetPositiveButtonText(
        /* [out] */ ICharSequence** text);

    CARAPI SetPositiveButtonListener(
        /* [in] */ IDialogInterfaceOnClickListener* listener);

    CARAPI GetPositiveButtonListener(
        /* [out] */ IDialogInterfaceOnClickListener** listener);

    CARAPI SetNegativeButtonText(
        /* [in] */ ICharSequence* text);

    CARAPI GetNegativeButtonText(
        /* [out] */ ICharSequence** text);

    CARAPI SetNegativeButtonListener(
        /* [in] */ IDialogInterfaceOnClickListener* listener);

    CARAPI GetNegativeButtonListener(
        /* [out] */ IDialogInterfaceOnClickListener** listener);

    CARAPI SetNeutralButtonText(
        /* [in] */ ICharSequence* text);

    CARAPI GetNeutralButtonText(
        /* [out] */ ICharSequence** text);

    CARAPI SetNeutralButtonListener(
        /* [in] */ IDialogInterfaceOnClickListener* listener);

    CARAPI GetNeutralButtonListener(
        /* [out] */ IDialogInterfaceOnClickListener** listener);

    CARAPI SetCancelable(
        /* [in] */ Boolean cancelable);

    CARAPI GetCancelable(
        /* [out] */ Boolean* cancelable);

    CARAPI SetOnCancelListener(
        /* [in] */ IDialogInterfaceOnCancelListener* listener);

    CARAPI GetOnCancelListener(
        /* [out] */ IDialogInterfaceOnCancelListener** listener);

    CARAPI SetOnDismissListener(
        /* [in] */ IDialogInterfaceOnDismissListener* listener);

    CARAPI GetOnDismissListener(
        /* [out] */ IDialogInterfaceOnDismissListener** listener);

    CARAPI SetOnKeyListener(
        /* [in] */ IDialogInterfaceOnKeyListener* listener);

    CARAPI GetOnKeyListener(
        /* [out] */ IDialogInterfaceOnKeyListener** listener);

    CARAPI SetItems(
        /* [in] */ ArrayOf<ICharSequence*>* items);

    CARAPI GetItems(
        /* [out, callee] */ ArrayOf<ICharSequence*>** items);

    CARAPI SetAdapter(
        /* [in] */ IListAdapter* adapter);

    CARAPI GetAdapter(
        /* [out] */ IListAdapter** adapter);

    CARAPI SetOnClickListener(
        /* [in] */ IDialogInterfaceOnClickListener* listener);

    CARAPI GetOnClickListener(
        /* [out] */ IDialogInterfaceOnClickListener** listener);

    CARAPI SetView(
        /* [in] */ IView* view);

    CARAPI GetView(
        /* [out] */ IView** view);

    CARAPI SetViewSpacingLeft(
        /* [in] */ Int32 left);

    CARAPI GetViewSpacingLeft(
        /* [out] */ Int32* left);

    CARAPI SetViewSpacingTop(
        /* [in] */ Int32 top);

    CARAPI GetViewSpacingTop(
        /* [out] */ Int32* top);

    CARAPI SetViewSpacingRight(
        /* [in] */ Int32 right);

    CARAPI GetViewSpacingRight(
        /* [out] */ Int32* right);

    CARAPI SetViewSpacingBottom(
        /* [in] */ Int32 bottom);

    CARAPI GetViewSpacingBottom(
        /* [out] */ Int32* bottom);

    CARAPI SetViewSpacingSpecified(
        /* [in] */ Boolean value);

    CARAPI GetViewSpacingSpecified(
        /* [out] */ Boolean* value);

    CARAPI SetCheckedItems(
        /* [in] */ ArrayOf<Boolean>* items);

    CARAPI GetCheckedItems(
        /* [out, callee] */ ArrayOf<Boolean>** items);

    CARAPI SetIsMultiChoice(
        /* [in] */ Boolean isMutiChoice);

    CARAPI GetIsMultiChoice(
        /* [out] */ Boolean* isMutiChoice);

    CARAPI SetIsSingleChoice(
        /* [in] */ Boolean isSingleChoice);

    CARAPI GetIsSingleChoice(
        /* [out] */ Boolean* isSingleChoice);

    CARAPI SetCheckedItem(
        /* [in] */ Int32 checkedItem);

    CARAPI GetCheckedItem(
        /* [out] */ Int32* checkedItem);

    CARAPI SetOnCheckboxClickListener(
        /* [in] */ IDialogInterfaceOnMultiChoiceClickListener* listener);

    CARAPI GetOnCheckboxClickListener(
        /* [out] */ IDialogInterfaceOnMultiChoiceClickListener** listener);

    CARAPI SetCursor(
        /* [in] */ ICursor* cursor);

    CARAPI GetCursor(
        /* [out] */ ICursor** cursor);

    CARAPI SetLabelColumn(
        /* [in] */ const String& labelColumn);

    CARAPI GetLabelColumn(
        /* [out] */ String* labelColumn);

    CARAPI SetIsCheckedColumn(
        /* [in] */ const String& column);

    CARAPI GetIsCheckedColumn(
        /* [out] */ String* column);

    CARAPI SetForceInverseBackground(
        /* [in] */ Boolean value);

    CARAPI GetForceInverseBackground(
        /* [out] */ Boolean* value);

    CARAPI SetOnItemSelectedListener(
        /* [in] */ IAdapterViewOnItemSelectedListener* listener);

    CARAPI GetOnItemSelectedListener(
        /* [out] */ IAdapterViewOnItemSelectedListener** listener);

    CARAPI SetOnPrepareListViewListener(
        /* [in] */ IAlertControllerAlertParamsOnPrepareListViewListener* listener);

    CARAPI GetOnPrepareListViewListener(
        /* [out] */ IAlertControllerAlertParamsOnPrepareListViewListener** listener);

    CARAPI SetRecycleOnMeasure(
        /* [in] */ Boolean recycleOnMeasure);

    CARAPI GetRecycleOnMeasure(
        /* [out] */ Boolean* recycleOnMeasure);

    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI Apply(
        /* [in] */ IAlertController* dialog);

private:
    CARAPI_(void) CreateListView(
        /* [in] */ IAlertController* dialog);

private:
    AutoPtr<IContext> mContext;
    AutoPtr<ILayoutInflater> mInflater;

    Int32 mIconId;
    AutoPtr<IDrawable> mIcon;
    Int32 mIconAttrId;
    AutoPtr<ICharSequence> mTitle;
    AutoPtr<IView> mCustomTitleView;
    AutoPtr<ICharSequence> mMessage;
    AutoPtr<ICharSequence> mPositiveButtonText;
    AutoPtr<IDialogInterfaceOnClickListener> mPositiveButtonListener;
    AutoPtr<ICharSequence> mNegativeButtonText;
    AutoPtr<IDialogInterfaceOnClickListener> mNegativeButtonListener;
    AutoPtr<ICharSequence> mNeutralButtonText;
    AutoPtr<IDialogInterfaceOnClickListener> mNeutralButtonListener;
    Boolean mCancelable;
    AutoPtr<IDialogInterfaceOnCancelListener> mOnCancelListener;
    AutoPtr<IDialogInterfaceOnDismissListener> mOnDismissListener;
    AutoPtr<IDialogInterfaceOnKeyListener> mOnKeyListener;
    AutoPtr<ArrayOf<ICharSequence*> > mItems;
    AutoPtr<IListAdapter> mAdapter;
    AutoPtr<IDialogInterfaceOnClickListener> mOnClickListener;
    AutoPtr<IView> mView;
    Int32 mViewSpacingLeft;
    Int32 mViewSpacingTop;
    Int32 mViewSpacingRight;
    Int32 mViewSpacingBottom;
    Boolean mViewSpacingSpecified;
    AutoPtr<ArrayOf<Boolean> > mCheckedItems;
    Boolean mIsMultiChoice;
    Boolean mIsSingleChoice;
    Int32 mCheckedItem;
    AutoPtr<IDialogInterfaceOnMultiChoiceClickListener> mOnCheckboxClickListener;
    AutoPtr<ICursor> mCursor;
    String mLabelColumn;
    String mIsCheckedColumn;
    Boolean mForceInverseBackground;
    AutoPtr<IAdapterViewOnItemSelectedListener> mOnItemSelectedListener;
    AutoPtr<IAlertControllerAlertParamsOnPrepareListViewListener> mOnPrepareListViewListener;
    Boolean mRecycleOnMeasure;
};

} // namespace App
} // namespace Internal
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_INTERNAL_APP_CALERTCONTROLLERALERTPARAMS_H__
