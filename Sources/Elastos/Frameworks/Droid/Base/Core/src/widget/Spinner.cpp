
#include "widget/Spinner.h"
#include <elastos/core/Math.h>
#include <elastos/utility/logging/Slogger.h>
#include "view/Gravity.h"
#include "view/CViewGroupLayoutParams.h"
#include "app/CAlertDialogBuilder.h"
#include "widget/TextView.h"

using Elastos::Utility::Logging::Slogger;
using Elastos::Droid::App::CAlertDialogBuilder;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::View::IGravity;
using Elastos::Droid::View::Gravity;
using Elastos::Droid::View::CViewGroupLayoutParams;
using Elastos::Droid::View::IViewTreeObserver;

namespace Elastos {
namespace Droid {
namespace Widget {

//c86bee25-e75e-47b8-8f25-382d31ada6b0
extern "C" const InterfaceID EIID_DropdownPopup =
        { 0xc86bee25, 0xe75e, 0x47b8, { 0x8f, 0x25, 0x38, 0x2d, 0x31, 0xad, 0xa6, 0xb0 } };

//d53105da-4e05-44d1-af84-708d464c18ba
extern "C" const InterfaceID EIID_Spinner =
        { 0xd53105da, 0x4e05, 0x44d1, { 0xaf, 0x84, 0x70, 0x8d, 0x46, 0x4c, 0x18, 0xba } };

const Int32 Spinner::MAX_ITEMS_MEASURED;
const Int32 Spinner::MODE_DIALOG;
const Int32 Spinner::MODE_DROPDOWN;
const Int32 Spinner::MODE_THEME;
const String Spinner::SPINNER_NAME = String("Spinner");

CAR_INTERFACE_IMPL(Spinner::ItemClickListener, IAdapterViewOnItemClickListener)
CAR_INTERFACE_IMPL(Spinner::GlobalLayoutListener, IOnGlobalLayoutListener)
CAR_INTERFACE_IMPL(Spinner::DismissListener, IPopupWindowOnDismissListener)
CAR_INTERFACE_IMPL_2(Spinner::DialogPopup, ISpinnerPopup, IDialogInterfaceOnClickListener)

Spinner::Spinner()
    : mDropDownWidth(0)
    , mGravity(0)
{
    CRect::New((IRect**)&mTempRect);
}

Spinner::Spinner(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle,
    /* [in] */ Int32 mode)
    : AbsSpinner(context, attrs, defStyle)
    , mDropDownWidth(0)
    , mGravity(0)
{
    CRect::New((IRect**)&mTempRect);
    InitImpl(context, attrs, defStyle, mode);
}

Spinner::Spinner(
    /* [in] */ IContext* context,
    /* [in] */ Int32 mode)
    : AbsSpinner(context, NULL, R::attr::spinnerStyle)
    , mDropDownWidth(0)
    , mGravity(0)
{
    CRect::New((IRect**)&mTempRect);
    InitImpl(context, NULL, R::attr::spinnerStyle, mode);
}

ECode Spinner::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle,
    /* [in] */ Int32 mode)
{
    AbsSpinner::Init(context, attrs, defStyle);
    return InitImpl(context, attrs, defStyle, mode);
}

ECode Spinner::InitImpl(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle,
    /* [in] */ Int32 mode)
{
    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::Spinner),
            ARRAY_SIZE(R::styleable::Spinner));
    AutoPtr<ITypedArray> a;
    FAIL_RETURN(context->ObtainStyledAttributes(attrs, attrIds, defStyle, 0, (ITypedArray**)&a));

    if (mode == MODE_THEME) {
        a->GetInt32(R::styleable::Spinner_spinnerMode, MODE_DIALOG, &mode);
    }

    switch (mode) {
        case MODE_DIALOG: {
            mPopup = new DialogPopup(this);
            break;
        }

        case MODE_DROPDOWN: {
            AutoPtr<ISpinnerPopup> popup = new DropdownPopup(context, attrs, defStyle, this);
            a->GetLayoutDimension(R::styleable::Spinner_dropDownWidth, IViewGroupLayoutParams::WRAP_CONTENT, &mDropDownWidth);
            AutoPtr<IDrawable> d;
            a->GetDrawable(R::styleable::Spinner_popupBackground, (IDrawable**)&d);
            popup->SetBackgroundDrawable(d);
            Int32 verticalOffset = 0;
            a->GetDimensionPixelOffset(R::styleable::Spinner_dropDownVerticalOffset, 0, &verticalOffset);
            if (verticalOffset != 0) {
                popup->SetVerticalOffset(verticalOffset);
            }

            Int32 horizontalOffset = 0;
            a->GetDimensionPixelOffset(R::styleable::Spinner_dropDownHorizontalOffset, 0, &horizontalOffset);
            if (horizontalOffset != 0) {
                popup->SetHorizontalOffset(horizontalOffset);
            }
            mPopup = popup;
            break;
        }
    }

    a->GetInt32(R::styleable::Spinner_gravity, IGravity::CENTER, &mGravity);
    String prompt;
    a->GetString(R::styleable::Spinner_prompt, &prompt);
    AutoPtr<ICharSequence> cprompt;
    CStringWrapper::New(prompt, (ICharSequence**)&cprompt);
    mPopup->SetPromptText(cprompt);
    a->GetBoolean(R::styleable::Spinner_disableChildrenWhenDisabled, FALSE, &mDisableChildrenWhenDisabled);

    a->Recycle();

    if (mTempAdapter) {
        mPopup->SetAdapter(mTempAdapter);
        mTempAdapter = NULL;
    }

    return NOERROR;
}

void Spinner::SetPopupBackgroundDrawable(
    /* [in] */ IDrawable* background)
{
    if (!mPopup->Probe(EIID_DropdownPopup)) {
        return;
    }
    AutoPtr<DropdownPopup> pop = reinterpret_cast<DropdownPopup*>(mPopup->Probe(EIID_DropdownPopup));
    pop->SetBackgroundDrawable(background);
}

void Spinner::SetPopupBackgroundResource(
    /* [in] */ Int32 resId)
{
    AutoPtr<IResources> resources;
    GetContext()->GetResources((IResources**)&resources);
    AutoPtr<IDrawable> drawable;
    resources->GetDrawable(resId, (IDrawable**)&drawable);
    SetPopupBackgroundDrawable(drawable);
}

AutoPtr<IDrawable> Spinner::GetPopupBackground()
{
    AutoPtr<IDrawable> d;
    mPopup->GetBackground((IDrawable**)&d);
    return d;
}

void Spinner::SetDropDownVerticalOffset(
    /* [in] */ Int32 pixels)
{
    mPopup->SetVerticalOffset(pixels);
}

Int32 Spinner::GetDropDownVerticalOffset()
{
    Int32 offset = 0;
    mPopup->GetVerticalOffset(&offset);
    return offset;
}

void Spinner::SetDropDownHorizontalOffset(
    /* [in] */ Int32 pixels)
{
    mPopup->SetHorizontalOffset(pixels);
}

Int32 Spinner::GetDropDownHorizontalOffset()
{
    Int32 offset = 0;
    mPopup->GetHorizontalOffset(&offset);
    return offset;
}

void Spinner::SetDropDownWidth(
    /* [in] */ Int32 pixels)
{
    if (!mPopup->Probe(EIID_DropdownPopup)) {
        return;
    }
    mDropDownWidth = pixels;
}

Int32 Spinner::GetDropDownWidth()
{
    return mDropDownWidth;
}

ECode Spinner::SetEnabled(
    /* [in] */ Boolean enabled)
{
    AbsSpinner::SetEnabled(enabled);
    if (mDisableChildrenWhenDisabled) {
        Int32 count = GetChildCount();
        for (Int32 i = 0; i < count; i++) {
            GetChildAt(i)->SetEnabled(enabled);
        }
    }
    return NOERROR;
}

void Spinner::SetGravity(
    /* [in] */ Int32 gravity)
{
    if (mGravity != gravity) {
        if ((gravity & IGravity::HORIZONTAL_GRAVITY_MASK) == 0) {
            gravity |= IGravity::START;
        }
        mGravity = gravity;
        RequestLayout();
    }
}

Int32 Spinner::GetGravity()
{
    return mGravity;
}

ECode Spinner::SetAdapter(
    /* [in] */ IAdapter* adapter)
{
    AutoPtr<ISpinnerAdapter> sa = ISpinnerAdapter::Probe(adapter);
    if (sa == NULL) {
        return NOERROR;
    }

    AbsSpinner::SetAdapter(sa);

    if (mPopup) {
        AutoPtr<IListAdapter> ad = new DropDownAdapter(sa);
        mPopup->SetAdapter(ad);
    }
    else {
        mTempAdapter = new DropDownAdapter(sa);
    }
    return NOERROR;
}

ECode Spinner::GetBaseline(
    /* [out] */ Int32* baseline)
{
    VALIDATE_NOT_NULL(baseline);
    *baseline = -1;

    AutoPtr<IView> child;
    Int32 count = 0;
    mAdapter->GetCount(&count);
    if (GetChildCount() > 0) {
        child = GetChildAt(0);
    }
    else if (mAdapter && count > 0) {
        child = MakeAndAddView(0);
        mRecycler->Put(0, child);
        RemoveAllViewsInLayout();
    }

    if (child) {
        Int32 childBaseline = 0;
        child->GetBaseline(&childBaseline);
        Int32 top = 0;
        child->GetTop(&top);
        if (childBaseline >= 0) {
            *baseline = top + childBaseline;
        }
    }

    return NOERROR;
}

ECode Spinner::SetOnItemClickListener(
    /* [in] */ IAdapterViewOnItemClickListener* l)
{
    //throw new RuntimeException("setOnItemClickListener cannot be used with a spinner.");
    return E_RUNTIME_EXCEPTION;
}

ECode Spinner::SetOnItemClickListenerInt(
    /* [in] */ IAdapterViewOnItemClickListener* l)
{
    return AbsSpinner::SetOnItemClickListener(l);
}

Boolean Spinner::PerformClick()
{
    Boolean handled = AbsSpinner::PerformClick();
    if (!handled) {
        handled = TRUE;
        Boolean show = FALSE;
        mPopup->IsShowing(&show);
        if (!show) {
            mPopup->Show();
        }
    }
    return handled;
}

void Spinner::OnClick(
    /* [in] */ IDialogInterface* dialog,
    /* [in] */ Int32 which)
{
    SetSelection(which);
    dialog->Dismiss();
}

ECode Spinner::OnInitializeAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    AbsSpinner::OnInitializeAccessibilityEvent(event);
    AutoPtr<ICharSequence> seq;
    CStringWrapper::New(String("CSpinner"), (ICharSequence**)&seq);
    return event->SetClassName(seq);
}

ECode Spinner::OnInitializeAccessibilityNodeInfo(
    /* [in] */ IAccessibilityNodeInfo* info)
{
    AbsSpinner::OnInitializeAccessibilityNodeInfo(info);
    AutoPtr<ICharSequence> seq;
    CStringWrapper::New(String("CSpinner"), (ICharSequence**)&seq);
    return info->SetClassName(seq);
}

void Spinner::SetPrompt(
    /* [in] */ ICharSequence* prompt)
{
    mPopup->SetPromptText(prompt);
}

void Spinner::SetPromptId(
    /* [in] */ Int32 promptId)
{
    AutoPtr<ICharSequence> csq;
    GetContext()->GetText(promptId, (ICharSequence**)&csq);
    SetPrompt(csq);
}

AutoPtr<ICharSequence> Spinner::GetPrompt()
{
    AutoPtr<ICharSequence> csq;
    mPopup->GetHintText((ICharSequence**)&csq);
    return csq;
}

Int32 Spinner::MeasureContentWidth(
    /* [in] */ ISpinnerAdapter* adapter,
    /* [in] */ IDrawable* background)
{
    if (!adapter) {
        return 0;
    }
    Int32 width = 0;
    AutoPtr<IView> itemView;
    Int32 itemType = 0;
    Int32 widthMeasureSpec = View::MeasureSpec::MakeMeasureSpec(0, View::MeasureSpec::UNSPECIFIED);
    Int32 heightMeasureSpec = View::MeasureSpec::MakeMeasureSpec(0, View::MeasureSpec::UNSPECIFIED);

    Int32 start = Elastos::Core::Math::Max(0, GetSelectedItemPosition());
    Int32 adc = 0;
    adapter->GetCount(&adc);
    Int32 end = Elastos::Core::Math::Min(adc, start + MAX_ITEMS_MEASURED);
    Int32 count = end - start;
    start = Elastos::Core::Math::Max(0, start - (MAX_ITEMS_MEASURED - count));

    for (Int32 i = start; i < end; i++) {
        Int32 positionType = 0;
        adapter->GetItemViewType(i, &positionType);
        if (positionType != itemType) {
            itemType = positionType;
            itemView = NULL;
        }
        adapter->GetView(i, itemView, (IViewGroup*)this->Probe(EIID_IViewGroup), (IView**)&itemView);

        AutoPtr<IViewGroupLayoutParams> lp;
        itemView->GetLayoutParams((IViewGroupLayoutParams**)&lp);
        if (!lp) {
            AutoPtr<IViewGroupLayoutParams> params;
            CViewGroupLayoutParams::New(IViewGroupLayoutParams::WRAP_CONTENT, IViewGroupLayoutParams::WRAP_CONTENT,
                (IViewGroupLayoutParams**)&params);
            itemView->SetLayoutParams(params);
        }
        itemView->Measure(widthMeasureSpec, heightMeasureSpec);
        Int32  mw = 0;
        itemView->GetMeasuredWidth(&mw);
        width = Elastos::Core::Math::Max(width, mw);
    }

    if (background) {
        Boolean res;
        background->GetPadding(mTempRect, &res);
        AutoPtr<CRect> ct = (CRect*)mTempRect.Get();
        width += ct->mLeft + ct->mRight;
    }
    return width;
}

ECode Spinner::OnDetachedFromWindow()
{
    AbsSpinner::OnDetachedFromWindow();

    Boolean show = FALSE;
    mPopup->IsShowing(&show);
    if (mPopup && show) {
        mPopup->Dismiss();
    }
    return NOERROR;
}

void Spinner::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    AbsSpinner::OnMeasure(widthMeasureSpec, heightMeasureSpec);
    if (mPopup && View::MeasureSpec::GetMode(widthMeasureSpec) == View::MeasureSpec::AT_MOST) {
        Int32 measuredWidth = GetMeasuredWidth();
        AutoPtr<ISpinnerAdapter> adapter = ISpinnerAdapter::Probe(GetAdapter());
        SetMeasuredDimension(Elastos::Core::Math::Min(Elastos::Core::Math::Max(
            measuredWidth, MeasureContentWidth(adapter, GetBackground())),
            MeasureSpec::GetSize(widthMeasureSpec)), GetMeasuredHeight());
    }
}

void Spinner::OnLayout(
    /* [in] */ Boolean changed,
    /* [in] */ Int32 l,
    /* [in] */ Int32 t,
    /* [in] */ Int32 r,
    /* [in] */ Int32 b)
{
    AbsSpinner::OnLayout(changed, l, t, r, b);
    mInLayout = TRUE;
    Layout(0, FALSE);
    mInLayout = FALSE;
}

void Spinner::Layout(
    /* [in] */ Int32 delta,
    /* [in] */ Boolean animate)
{
    Int32 childrenLeft = mSpinnerPadding->mLeft;
    Int32 childrenWidth = mRight - mLeft - mSpinnerPadding->mLeft - mSpinnerPadding->mRight;

    if (mDataChanged) {
        HandleDataChanged();
    }

    if (mItemCount == 0) {
        ResetList();
        return;
    }
     if (mNextSelectedPosition >= 0) {
        SetSelectedPositionInt(mNextSelectedPosition);
    }

    RecycleAllViews();
    RemoveAllViewsInLayout();
    mFirstPosition = mSelectedPosition;
    AutoPtr<IView> sel = MakeAndAddView(mSelectedPosition);
    if (sel != NULL) {
        Int32 width = 0;
        sel->GetMeasuredWidth(&width);
        Int32 selectedOffset = childrenLeft;
        Int32 layoutDirection = GetLayoutDirection();
        Int32 absoluteGravity = Gravity::GetAbsoluteGravity(mGravity, layoutDirection);

        switch (absoluteGravity & IGravity::HORIZONTAL_GRAVITY_MASK) {
            case IGravity::CENTER_HORIZONTAL:
                selectedOffset = childrenLeft + (childrenWidth / 2) - (width / 2);
                break;
            case IGravity::RIGHT:
                selectedOffset = childrenLeft + childrenWidth - width;
                break;
        }

        sel->OffsetLeftAndRight(selectedOffset);
    }

    mRecycler->Clear();
    Invalidate();
    CheckSelectionChanged();
    mDataChanged = FALSE;
    mNeedSync = FALSE;
    SetNextSelectedPositionInt(mSelectedPosition);
}

AutoPtr<IView> Spinner::MakeAndAddView(
    /* [in] */ Int32 position)
{
    AutoPtr<IView> child;
    if (!mDataChanged) {
        child = mRecycler->Get(position);
        if (child) {
            SetUpChild(child);
            return child;
        }
    }
    child = NULL;
    mAdapter->GetView(position, NULL, THIS_PROBE(IViewGroup), (IView**)&child);
    if (child == NULL) {
        Slogger::E(SPINNER_NAME, "Failed to make child view!");
    }
    else {
        SetUpChild(child);
    }
    return child;
}

void Spinner::SetUpChild(
    /* [in] */ IView* child)
{
    assert(child != NULL);

    AutoPtr<IViewGroupLayoutParams> lp;
    child->GetLayoutParams((IViewGroupLayoutParams**)&lp);
    if (!lp) {
        GenerateDefaultLayoutParams((IViewGroupLayoutParams**)&lp);
    }

    AddViewInLayout(child, 0, lp);
    child->SetSelected(HasFocus());
    if (mDisableChildrenWhenDisabled) {
        child->SetEnabled(IsEnabled());
    }
    Int32 h;
    lp->GetHeight(&h);
    Int32 childHeightSpec = ViewGroup::GetChildMeasureSpec(mHeightMeasureSpec,
            mSpinnerPadding->mTop + mSpinnerPadding->mBottom, h);
    Int32 w;
    lp->GetWidth(&w);
    Int32 childWidthSpec = ViewGroup::GetChildMeasureSpec(mWidthMeasureSpec,
            mSpinnerPadding->mLeft + mSpinnerPadding->mRight, w);

    child->Measure(childWidthSpec, childHeightSpec);

    Int32 measuredHeight = 0;
    child->GetMeasuredHeight(&measuredHeight);
    Int32 childLeft = 0, childRight = 0;

    Int32 childTop = mSpinnerPadding->mTop + ((GetMeasuredHeight() - mSpinnerPadding->mBottom -
        mSpinnerPadding->mTop - measuredHeight) / 2);
    Int32 childBottom = childTop + measuredHeight;
    Int32 width = 0;
    child->GetMeasuredWidth(&width);
    childLeft = 0;
    childRight = childLeft + width;

    child->Layout(childLeft, childTop, childRight, childBottom);
}

//==========================================================================
//                       Spinner::DropDownAdapter
//==========================================================================

PInterface Spinner::DropDownAdapter::Probe(
    /* [in] */ REIID riid)
{
    if ( riid == EIID_IInterface) {
        return (IInterface*)(IListAdapter *)this;
    }
    else if( riid == EIID_IAdapter) {
        return (IAdapter*)(IListAdapter*)this;
    }
    else if(riid == EIID_IListAdapter) {
        return (IListAdapter*)this;
    }
    else if ( riid == EIID_ISpinnerAdapter) {
        return (ISpinnerAdapter*)this;
    }
    return NULL;
}

UInt32 Spinner::DropDownAdapter::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 Spinner::DropDownAdapter::Release()
{
    return ElRefBase::Release();
}

ECode Spinner::DropDownAdapter::GetInterfaceID(
    /* [in] */ IInterface* object,
    /* [out] */ InterfaceID* iid)
{
    VALIDATE_NOT_NULL(iid);
    if (object == (IInterface*)(IListAdapter *)this) {
        *iid = EIID_IListAdapter;
    }
    if (object == (IInterface*)(ISpinnerAdapter *)this) {
        *iid = EIID_ISpinnerAdapter;
    }
    else {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

Spinner::DropDownAdapter::DropDownAdapter(
    /* [in] */ ISpinnerAdapter* adapter)
{
    mAdapter = adapter;

    AutoPtr<IListAdapter> la = IListAdapter::Probe(adapter);
    if (la) {
        mListAdapter = la;
    }
}

ECode Spinner::DropDownAdapter::GetCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    *count = 0;

    if (mAdapter) {
        mAdapter->GetCount(count);
    }
    return NOERROR;
}

ECode Spinner::DropDownAdapter::GetItem(
    /* [in] */ Int32 position,
    /* [out] */ IInterface** item)
{
    VALIDATE_NOT_NULL(item);
    *item = NULL;

    if (mAdapter) {
        mAdapter->GetItem(position, item);
    }
    return NOERROR;
}

ECode Spinner::DropDownAdapter::GetItemId(
    /* [in] */ Int32 position,
    /* [out] */ Int64* itemId)
{
    VALIDATE_NOT_NULL(itemId);
    *itemId = -1;

    if (mAdapter) {
        mAdapter->GetItemId(position, itemId);
    }
    return NOERROR;
}

ECode Spinner::DropDownAdapter::GetView(
    /* [in] */ Int32 position,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent,
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);
    return GetDropDownView(position, convertView, parent, view);
}

ECode Spinner::DropDownAdapter::GetDropDownView(
    /* [in] */ Int32 position,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent,
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);
    *view = NULL;

    if (mAdapter) {
        mAdapter->GetDropDownView(position, convertView, parent, view);
    }
    return NOERROR;
}

ECode Spinner::DropDownAdapter::HasStableIds(
    /* [out] */ Boolean* hasStableIds)
{
    VALIDATE_NOT_NULL(hasStableIds);
    *hasStableIds = FALSE;

    if (mAdapter) {
        mAdapter->HasStableIds(hasStableIds);
    }

    return NOERROR;
}

ECode Spinner::DropDownAdapter::RegisterDataSetObserver(
    /* [in] */ IDataSetObserver* observer)
{
    if (mAdapter) {
        mAdapter->RegisterDataSetObserver(observer);
    }
    return NOERROR;
}

ECode Spinner::DropDownAdapter::UnregisterDataSetObserver(
    /* [in] */ IDataSetObserver* observer)
{
    if (mAdapter) {
        mAdapter->UnregisterDataSetObserver(observer);
    }
    return NOERROR;
}

ECode Spinner::DropDownAdapter::AreAllItemsEnabled(
    /* [out] */ Boolean* enabled)
{
    VALIDATE_NOT_NULL(enabled);
    *enabled = TRUE;

    if (mListAdapter) {
        return mListAdapter->AreAllItemsEnabled(enabled);
    }
    return NOERROR;
}

ECode Spinner::DropDownAdapter::IsEnabled(
    /* [in] */ Int32 position,
    /* [out] */ Boolean* enabled)
{
    VALIDATE_NOT_NULL(enabled);
    *enabled = TRUE;

    if (mListAdapter) {
        return mListAdapter->IsEnabled(position, enabled);
    }
    return NOERROR;
}

ECode Spinner::DropDownAdapter::GetItemViewType(
    /* [in] */ Int32 position,
    /* [out] */ Int32* viewType)
{
    VALIDATE_NOT_NULL(viewType);
    *viewType = 0;
    return NOERROR;
}

ECode Spinner::DropDownAdapter::GetViewTypeCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    *count = 1;
    return NOERROR;
}

ECode Spinner::DropDownAdapter::IsEmpty(
    /* [out] */ Boolean* isEmpty)
{
    VALIDATE_NOT_NULL(isEmpty);
    Int32 count = 0;
    GetCount(&count);
    *isEmpty = count == 0;
    return NOERROR;
}

//==========================================================================
//                       Spinner::DialogPopup
//==========================================================================

Spinner::DialogPopup::DialogPopup(
    /* [in] */ Spinner* host)
    : mHost(host)
{
}

ECode Spinner::DialogPopup::Dismiss()
{
    mPopup->Dismiss();
    mPopup = NULL;
    return NOERROR;
}

ECode Spinner::DialogPopup::IsShowing(
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = FALSE;

    if (mPopup) {
        return mPopup->IsShowing(res);
    }
    return NOERROR;
}

ECode Spinner::DialogPopup::SetAdapter(
    /* [in] */ IListAdapter* adapter)
{
    mListAdapter = adapter;
    return NOERROR;
}

ECode Spinner::DialogPopup::SetPromptText(
    /* [in] */ ICharSequence* hintText)
{
    mPrompt = hintText;
    return NOERROR;
}

ECode Spinner::DialogPopup::GetHintText(
    /* [out] */ ICharSequence** csq)
{
    VALIDATE_NOT_NULL(csq);
    *csq = mPrompt;
    INTERFACE_ADDREF(*csq);
    return NOERROR;
}

ECode Spinner::DialogPopup::Show()
{
    AutoPtr<IAlertDialogBuilder> builder;
    CAlertDialogBuilder::New(mHost->GetContext(), (IAlertDialogBuilder**)&builder);
    if (mPrompt) {
        builder->SetTitle(mPrompt);
    }
    builder->SetSingleChoiceItems(mListAdapter, mHost->GetSelectedItemPosition(), this);

    mPopup = NULL;
    builder->Show((IAlertDialog**)&mPopup);
    return NOERROR;
}

ECode Spinner::DialogPopup::OnClick(
    /* [in] */ IDialogInterface* dialog,
    /* [in] */ Int32 which)
{
    mHost->SetSelection(which);
    if (mHost->mOnItemClickListener) {
        Int64 id = 0;
        mListAdapter->GetItemId(which, &id);
        mHost->PerformItemClick(NULL, which, id);
    }
    Dismiss();
    return NOERROR;
}

ECode Spinner::DialogPopup::SetBackgroundDrawable(
    /* [in] */ IDrawable* bg)
{
    //Log.e(TAG, "Cannot set popup background for MODE_DIALOG, ignoring");
    return NOERROR;
}

ECode Spinner::DialogPopup::SetVerticalOffset(
    /* [in] */ Int32 px)
{
    //Log.e(TAG, "Cannot set vertical offset for MODE_DIALOG, ignoring");
    return NOERROR;
}

ECode Spinner::DialogPopup::SetHorizontalOffset(
    /* [in] */ Int32 px)
{
    //Log.e(TAG, "Cannot set horizontal offset for MODE_DIALOG, ignoring");
    return NOERROR;
}

ECode Spinner::DialogPopup::GetBackground(
    /* [out] */ IDrawable** d)
{
    VALIDATE_NOT_NULL(d);
    *d = NULL;
    return NOERROR;
}

ECode Spinner::DialogPopup::GetVerticalOffset(
    /* [out] */ Int32* offset)
{
    VALIDATE_NOT_NULL(offset);
    *offset = 0;
    return NOERROR;
}

ECode Spinner::DialogPopup::GetHorizontalOffset(
    /* [out] */ Int32* offset)
{
    VALIDATE_NOT_NULL(offset);
    *offset = 0;
    return NOERROR;
}

//==========================================================================
//                       Spinner::ItemClickListener
//==========================================================================
Spinner::ItemClickListener::ItemClickListener(
    /* [in] */ Spinner* spinnerHost,
    /* [in] */ _DropdownPopup* popupHost)
{
    mSpinnerHost = spinnerHost;
    mPopupHost = popupHost;
}

ECode Spinner::ItemClickListener::OnItemClick(
    /* [in] */ IAdapterView* parent,
    /* [in] */ IView* view,
    /* [in] */ Int32 position,
    /* [in] */ Int64 id)
{
    mSpinnerHost->SetSelection(position);
    if (mSpinnerHost->mOnItemClickListener != NULL) {
        Int64 id;
        mPopupHost->mAdapter->GetItemId(position, &id);
        mSpinnerHost->PerformItemClick(view, position, id);
    }
    mPopupHost->Dismiss();
    return NOERROR;
}

//==========================================================================
//                       Spinner::GlobalLayoutListener
//==========================================================================
Spinner::GlobalLayoutListener::GlobalLayoutListener(
    /* [in] */ Spinner* spinnerHost,
    /* [in] */ _DropdownPopup* popupHost)
{
    mSpinnerHost = spinnerHost;
    mPopupHost = popupHost;
}

ECode Spinner::GlobalLayoutListener::OnGlobalLayout()
{
    if (!mSpinnerHost->IsVisibleToUser()) {
        mPopupHost->Dismiss();
    }
    return NOERROR;
}

//==========================================================================
//                       Spinner::DismissListener
//==========================================================================
Spinner::DismissListener::DismissListener(
    /* [in] */ Spinner* spinnerHost,
    /* [in] */ GlobalLayoutListener* layoutListener)
{
    mSpinnerHost = spinnerHost;
    mLayoutListener = layoutListener;
}

ECode Spinner::DismissListener::OnDismiss()
{
    AutoPtr<IViewTreeObserver> vto = mSpinnerHost->GetViewTreeObserver();
    if (vto) {
        vto->RemoveGlobalOnLayoutListener(mLayoutListener);
    }
    return NOERROR;
}

//==========================================================================
//                       Spinner::_DropdownPopup
//==========================================================================
Spinner::_DropdownPopup::_DropdownPopup(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle,
    /* [in] */ Spinner* host)
    : ListPopupWindow(context, attrs, 0, defStyle)
    , mHost(host)
{
    SetAnchorView((IView*)(mHost->Probe(EIID_IView)));
    SetModal(TRUE);
    SetPromptPosition(IListPopupWindow::POSITION_PROMPT_ABOVE);
    AutoPtr<IAdapterViewOnItemClickListener> listener = new ItemClickListener(mHost, this);
    SetOnItemClickListener(listener);
}

ECode Spinner::_DropdownPopup::SetAdapter(
    /* [in] */ IListAdapter* adapter)
{
    ListPopupWindow::SetAdapter(adapter);
    mAdapter = adapter;
    return NOERROR;
}

AutoPtr<ICharSequence> Spinner::_DropdownPopup::GetHintText()
{
    return mHintText;
}

ECode Spinner::_DropdownPopup::SetPromptText(
    /* [in] */ ICharSequence* hintText)
{
    mHintText = hintText;
    return NOERROR;
}

ECode Spinner::_DropdownPopup::Show()
{
    AutoPtr<IDrawable> background = GetBackground();

    Int32 hOffset = 0;

    if (background) {
        Boolean rst;
        background->GetPadding(mHost->mTempRect, &rst);
        AutoPtr<CRect> cr = (CRect*)mTempRect.Get();
        hOffset = mHost->IsLayoutRtl() ? cr->mRight : -(cr->mLeft);
    }
    else {
        mTempRect->SetLeft(0);
        mTempRect->SetRight(0);
    }

    Int32 spinnerPaddingLeft = mHost->GetPaddingLeft();
    Int32 spinnerPaddingRight = mHost->GetPaddingRight();
    Int32 spinnerWidth = mHost->GetWidth();

    if (mHost->mDropDownWidth == IViewGroupLayoutParams::WRAP_CONTENT) {
        Int32 contentWidth = mHost->MeasureContentWidth(ISpinnerAdapter::Probe(mAdapter), GetBackground());
        AutoPtr<IResources> resources;
        mHost->mContext->GetResources((IResources**)&resources);
        AutoPtr<IDisplayMetrics> metrics;
        resources->GetDisplayMetrics((IDisplayMetrics**)&metrics);
        Int32 pixels = 0;
        metrics->GetWidthPixels(&pixels);
        AutoPtr<CRect> cr = (CRect*)(mTempRect.Get());
        Int32 contentWidthLimit = pixels - cr->mLeft - cr->mRight;

        if (contentWidth > contentWidthLimit) {
            contentWidth = contentWidthLimit;
        }
        SetContentWidth(Elastos::Core::Math::Max(
                       contentWidth, spinnerWidth - spinnerPaddingLeft - spinnerPaddingRight));
    }
    else if (mHost->mDropDownWidth == IViewGroupLayoutParams::MATCH_PARENT) {
        SetContentWidth(spinnerWidth - spinnerPaddingLeft - spinnerPaddingRight);
    }
    else {
        SetContentWidth(mHost->mDropDownWidth);
    }

    if (mHost->IsLayoutRtl()) {
        hOffset += spinnerWidth - spinnerPaddingRight - mHost->GetWidth();
    }
    else {
        hOffset += spinnerPaddingLeft;
    }

    SetHorizontalOffset(hOffset);
    SetInputMethodMode(IListPopupWindow::INPUT_METHOD_NOT_NEEDED);
    ListPopupWindow::Show();
    GetListView()->SetChoiceMode(IListView::CHOICE_MODE_SINGLE);
    mHost->SetSelection(mHost->GetSelectedItemPosition());

    // Make sure we hide if our anchor goes away.
    // TODO: This might be appropriate to push all the way down to PopupWindow,
    // but it may have other side effects to investigate first. (Text editing handles, etc.)
    AutoPtr<IViewTreeObserver> vto = mHost->GetViewTreeObserver();
    if (vto) {
        AutoPtr<GlobalLayoutListener> layoutListener = new GlobalLayoutListener(mHost, this);
        vto->AddOnGlobalLayoutListener(layoutListener);
        AutoPtr<IPopupWindowOnDismissListener> dismissListener = new DismissListener(mHost, layoutListener);
        SetOnDismissListener(dismissListener);
    }
    return NOERROR;
}

//==========================================================================
//                       Spinner::DropdownPopup
//==========================================================================
Spinner::DropdownPopup::DropdownPopup(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle,
    /* [in] */ Spinner* host)
    : _DropdownPopup(context, attrs, defStyle, host)
{}

PInterface Spinner::DropdownPopup::DropdownPopup::Probe(
    /* [in] */ REIID riid)
{
    if ( riid == EIID_IInterface) {
        return (IInterface*)(IListPopupWindow *)this;
    }
    else if ( riid == EIID_IListPopupWindow ) {
        return (IListPopupWindow *)this;
    }
    else if ( riid == EIID_ISpinnerPopup ) {
        return (ISpinnerPopup *)this;
    }
    else if ( riid == EIID_DropdownPopup) {
        return reinterpret_cast<PInterface>(this);
    }
    return NULL;
}

UInt32 Spinner::DropdownPopup::DropdownPopup::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 Spinner::DropdownPopup::DropdownPopup::Release()
{
    return ElRefBase::Release();
}

ECode Spinner::DropdownPopup::DropdownPopup::GetInterfaceID(
    /* [in] */ IInterface* object,
    /* [out] */ InterfaceID* iid)
{
    VALIDATE_NOT_NULL(iid);
    if (object == (IInterface*)(IListPopupWindow *)this) {
        *iid = EIID_IListPopupWindow;
    }
    if (object == (IInterface*)(ISpinnerPopup *)this) {
        *iid = EIID_ISpinnerPopup;
    }
    if (object == reinterpret_cast<PInterface>(this)) {
        *iid = EIID_DropdownPopup;
    }
    else {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

ECode Spinner::DropdownPopup::SetAdapter(
    /* [in] */ IListAdapter* adapter)
{
    return _DropdownPopup::SetAdapter(adapter);
}

ECode Spinner::DropdownPopup::SetPromptPosition(
    /* [in] */ Int32 position)
{
    return _DropdownPopup::SetPromptPosition(position);
}

ECode Spinner::DropdownPopup::GetPromptPosition(
    /* [out] */ Int32* position)
{
    VALIDATE_NOT_NULL(position);
    *position = _DropdownPopup::GetPromptPosition();
    return NOERROR;
}

ECode Spinner::DropdownPopup::SetModal(
    /* [in] */ Boolean modal)
{
    return _DropdownPopup::SetModal(modal);
}

ECode Spinner::DropdownPopup::IsModal(
    /* [out] */ Boolean* modal)
{
    VALIDATE_NOT_NULL(modal);
    *modal = _DropdownPopup::IsModal();
    return NOERROR;
}

ECode Spinner::DropdownPopup::SetForceIgnoreOutsideTouch(
    /* [in] */ Boolean forceIgnoreOutsideTouch)
{
    return _DropdownPopup::SetForceIgnoreOutsideTouch(forceIgnoreOutsideTouch);
}

ECode Spinner::DropdownPopup::SetDropDownAlwaysVisible(
    /* [in] */ Boolean dropDownAlwaysVisible)
{
    return _DropdownPopup::SetDropDownAlwaysVisible(dropDownAlwaysVisible);
}

ECode Spinner::DropdownPopup::IsDropDownAlwaysVisible(
    /* [out] */ Boolean* visible)
{
    VALIDATE_NOT_NULL(visible);
    *visible = _DropdownPopup::IsDropDownAlwaysVisible();
    return NOERROR;
}

ECode Spinner::DropdownPopup::SetSoftInputMode(
    /* [in] */ Int32 mode)
{
    return _DropdownPopup::SetSoftInputMode(mode);
}

ECode Spinner::DropdownPopup::GetSoftInputMode(
    /* [out] */ Int32* mode)
{
    VALIDATE_NOT_NULL(mode);
    *mode = _DropdownPopup::GetSoftInputMode();
    return NOERROR;
}

ECode Spinner::DropdownPopup::SetListSelector(
    /* [in] */ IDrawable* selector)
{
    return _DropdownPopup::SetListSelector(selector);
}

ECode Spinner::DropdownPopup::GetBackground(
    /* [out] */ IDrawable** d)
{
    VALIDATE_NOT_NULL(d);
    AutoPtr<IDrawable> drawable = _DropdownPopup::GetBackground();
    *d = drawable;
    INTERFACE_ADDREF(*d);
    return NOERROR;
}

ECode Spinner::DropdownPopup::SetBackgroundDrawable(
    /* [in] */ IDrawable* d)
{
    return _DropdownPopup::SetBackgroundDrawable(d);
}

ECode Spinner::DropdownPopup::SetAnimationStyle(
    /* [in] */ Int32 animationStyle)
{
    return _DropdownPopup::SetAnimationStyle(animationStyle);
}

ECode Spinner::DropdownPopup::GetAnimationStyle(
    /* [out] */ Int32* style)
{
    VALIDATE_NOT_NULL(style);
    *style = _DropdownPopup::GetAnimationStyle();
    return NOERROR;
}

ECode Spinner::DropdownPopup::GetAnchorView(
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);
    AutoPtr<IView> v = _DropdownPopup::GetAnchorView();
    *view = v;
    INTERFACE_ADDREF(*view);
    return NOERROR;
}

ECode Spinner::DropdownPopup::SetAnchorView(
        /* [in] */ IView* anchor)
{
    return _DropdownPopup::SetAnchorView(anchor);
}

ECode Spinner::DropdownPopup::GetHorizontalOffset(
    /* [out] */ Int32* offset)
{
    VALIDATE_NOT_NULL(offset);
    *offset = _DropdownPopup::GetHorizontalOffset();
    return NOERROR;
}

ECode Spinner::DropdownPopup::SetHorizontalOffset(
    /* [in] */ Int32 offset)
{
    return _DropdownPopup::SetHorizontalOffset(offset);
}

ECode Spinner::DropdownPopup::GetVerticalOffset(
    /* [out] */ Int32* offset)
{
    VALIDATE_NOT_NULL(offset);
    *offset = _DropdownPopup::GetVerticalOffset();
    return NOERROR;
}

ECode Spinner::DropdownPopup::SetVerticalOffset(
        /* [in] */ Int32 offset)
{
    return _DropdownPopup::SetVerticalOffset(offset);
}

ECode Spinner::DropdownPopup::GetWidth(
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);
    *width = _DropdownPopup::GetWidth();
    return NOERROR;
}

ECode Spinner::DropdownPopup::SetWidth(
    /* [in] */ Int32 width)
{
    return _DropdownPopup::SetWidth(width);
}

ECode Spinner::DropdownPopup::SetContentWidth(
    /* [in] */ Int32 width)
{
    return _DropdownPopup::SetContentWidth(width);
}

ECode Spinner::DropdownPopup::GetHeight(
    /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height);
    *height = _DropdownPopup::GetHeight();
    return NOERROR;
}

ECode Spinner::DropdownPopup::SetHeight(
    /* [in] */ Int32 height)
{
    return _DropdownPopup::SetHeight(height);
}

ECode Spinner::DropdownPopup::SetOnItemClickListener(
    /* [in] */ IAdapterViewOnItemClickListener* clickListener)
{
    return _DropdownPopup::SetOnItemClickListener(clickListener);
}

ECode Spinner::DropdownPopup::SetOnItemSelectedListener(
    /* [in] */ IAdapterViewOnItemSelectedListener* selectedListener)
{
    return _DropdownPopup::SetOnItemSelectedListener(selectedListener);
}

ECode Spinner::DropdownPopup::SetPromptView(
    /* [in] */ IView* prompt)
{
    return _DropdownPopup::SetPromptView(prompt);
}

ECode Spinner::DropdownPopup::PostShow()
{
    return _DropdownPopup::PostShow();
}

ECode Spinner::DropdownPopup::Show()
{
    return _DropdownPopup::Show();
}

ECode Spinner::DropdownPopup::Dismiss()
{
    return _DropdownPopup::Dismiss();
}

ECode Spinner::DropdownPopup::SetOnDismissListener(
    /* [in] */ IPopupWindowOnDismissListener* listener)
{
    return _DropdownPopup::SetOnDismissListener(listener);
}

ECode Spinner::DropdownPopup::SetInputMethodMode(
    /* [in] */ Int32 mode)
{
    return _DropdownPopup::SetInputMethodMode(mode);
}

ECode Spinner::DropdownPopup::GetInputMethodMode(
    /* [out] */ Int32* mode)
{
    VALIDATE_NOT_NULL(mode);
    *mode = _DropdownPopup::GetInputMethodMode();
    return NOERROR;
}

ECode Spinner::DropdownPopup::SetSelection(
    /* [in] */ Int32 position)
{
    return _DropdownPopup::SetSelection(position);
}

ECode Spinner::DropdownPopup::ClearListSelection()
{
    return _DropdownPopup::ClearListSelection();
}

ECode Spinner::DropdownPopup::IsShowing(
    /* [out] */ Boolean* showing)
{
    VALIDATE_NOT_NULL(showing);
    *showing = _DropdownPopup::IsShowing();
    return NOERROR;
}

ECode Spinner::DropdownPopup::IsInputMethodNotNeeded(
    /* [out] */ Boolean* needed)
{
    VALIDATE_NOT_NULL(needed);
    *needed = _DropdownPopup::IsInputMethodNotNeeded();
    return NOERROR;
}

ECode Spinner::DropdownPopup::PerformItemClick(
    /* [in] */ Int32 position,
    /* [out] */ Boolean* click)
{
    VALIDATE_NOT_NULL(click);
    *click = _DropdownPopup::PerformItemClick(position);
    return NOERROR;
}

ECode Spinner::DropdownPopup::GetSelectedItem(
    /* [out] */ IInterface** item)
{
    VALIDATE_NOT_NULL(item);
    AutoPtr<IInterface> i = _DropdownPopup::GetSelectedItem();
    *item = i;
    INTERFACE_ADDREF(*item);
    return NOERROR;
}

ECode Spinner::DropdownPopup::GetSelectedItemPosition(
    /* [out] */ Int32* position)
{
    VALIDATE_NOT_NULL(position);
    *position = _DropdownPopup::GetSelectedItemPosition();
    return NOERROR;
}

ECode Spinner::DropdownPopup::GetSelectedItemId(
    /* [out] */ Int64* id)
{
    VALIDATE_NOT_NULL(id);
    *id = _DropdownPopup::GetSelectedItemId();
    return NOERROR;
}

ECode Spinner::DropdownPopup::GetSelectedView(
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);
    AutoPtr<IView> v = _DropdownPopup::GetSelectedView();
    *view = v;
    INTERFACE_ADDREF(*view);
    return NOERROR;
}

ECode Spinner::DropdownPopup::GetListView(
    /* [out] */ IListView** view)
{
    VALIDATE_NOT_NULL(view);
    AutoPtr<IListView> v = _DropdownPopup::GetListView();
    *view = v;
    INTERFACE_ADDREF(*view);
    return NOERROR;
}

ECode Spinner::DropdownPopup::SetListItemExpandMax(
    /* [in] */ Int32 max)
{
    return _DropdownPopup::SetListItemExpandMax(max);
}

ECode Spinner::DropdownPopup::OnKeyDown(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = _DropdownPopup::OnKeyDown(keyCode, event);
    return NOERROR;
}

ECode Spinner::DropdownPopup::OnKeyUp(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = _DropdownPopup::OnKeyUp(keyCode, event);
    return NOERROR;
}

ECode Spinner::DropdownPopup::OnKeyPreIme(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = _DropdownPopup::OnKeyPreIme(keyCode, event);
    return NOERROR;
}

ECode Spinner::DropdownPopup::SetPromptText(
    /* [in] */ ICharSequence* hintText)
{
    return _DropdownPopup::SetPromptText(hintText);
}

ECode Spinner::DropdownPopup::GetHintText(
    /* [out] */ ICharSequence** csq)
{
    AutoPtr<ICharSequence> temp = _DropdownPopup::GetHintText();
    *csq = temp;
    INTERFACE_ADDREF(*csq);
    return NOERROR;
}

} // namespace Widget
} // namespace Droid
} // namespace Elastos
