
#include "elastos/droid/widget/AbsSpinner.h"
#include "elastos/droid/widget/CArrayAdapter.h"
#include "elastos/droid/widget/CAbsSpinnerSavedState.h"
#include "elastos/droid/view/CViewGroupLayoutParams.h"
#include <elastos/core/Math.h>
#include <elastos/core/StringBuilder.h>

using Elastos::Droid::View::Accessibility::IAccessibilityRecord;
using Elastos::Droid::View::CViewGroupLayoutParams;
using Elastos::Droid::View::IViewGroupLayoutParams;
using Elastos::Core::CSystem;
using Elastos::Core::ISystem;
using Elastos::Core::StringBuilder;

namespace Elastos {
namespace Droid {
namespace Widget {

CAR_INTERFACE_IMPL(AbsSpinner::SavedState, BaseSavedState, IAbsSpinnerSavedState);
AbsSpinner::SavedState::SavedState()
    : mSelectedId(0)
    , mPosition(0)
{}

ECode AbsSpinner::SavedState::constructor()
{
    return NOERROR;
}

ECode AbsSpinner::SavedState::constructor(
    /* [in] */ IParcelable* superState)
{
    return BaseSavedState::constructor(superState);
}

ECode AbsSpinner::SavedState::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    BaseSavedState::ReadFromParcel(source);
    source->ReadInt64(&mSelectedId);
    source->ReadInt32(&mPosition);
    return NOERROR;
}

ECode AbsSpinner::SavedState::WriteToParcel(
    /* [in] */ IParcel* out)
{
    BaseSavedState::WriteToParcel(out);
    out->WriteInt64(mSelectedId);
    out->WriteInt32(mPosition);
    return NOERROR;
}

ECode AbsSpinner::SavedState::ToString(
    /* [out] */ String* str)
{
    StringBuilder sb("AbsSpinner.SavedState{");
    AutoPtr<ISystem> system;
    CSystem::AcquireSingleton((ISystem**)&system);
    Int32 hashCode;
    system->IdentityHashCode(THIS_PROBE(IAbsSpinnerSavedState), &hashCode);
    sb.Append(hashCode);
    sb.Append(" selectedId=");
    sb.Append(mSelectedId);
    sb.Append(" position=");
    sb.Append(mPosition);
    sb.Append("}");
    return sb.ToString(str);
}

CAR_INTERFACE_IMPL(AbsSpinner, AdapterView, IAbsSpinner);
AbsSpinner::AbsSpinner()
    : mHeightMeasureSpec(0)
    , mWidthMeasureSpec(0)
    , mSelectionLeftPadding(0)
    , mSelectionTopPadding(0)
    , mSelectionRightPadding(0)
    , mSelectionBottomPadding(0)
{
    CRect::NewByFriend((CRect**)&mSpinnerPadding);
    mRecycler = new RecycleBin(this);
}

ECode AbsSpinner::constructor(
    /* [in] */ IContext* context)
{
    ASSERT_SUCCEEDED(AdapterView::constructor(context));
    InitAbsSpinner();
    return NOERROR;
}

ECode AbsSpinner::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return constructor(context, attrs, 0);
}

ECode AbsSpinner::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyleAttr)
{
    return constructor(context, attrs, defStyleAttr, 0);
}

ECode AbsSpinner::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyleAttr,
    /* [in] */ Int32 defStyleRes)
{
    ASSERT_SUCCEEDED(AdapterView::constructor(context, attrs, defStyleAttr, defStyleRes));
    InitAbsSpinner();
    return InitFromAttributes(context, attrs, defStyleAttr, defStyleRes);
}

AbsSpinner::~AbsSpinner()
{
}

ECode AbsSpinner::InitFromAttributes(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyleAttr,
    /* [in] */ Int32 defStyleRes)
{
    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::AbsSpinner),
            ARRAY_SIZE(R::styleable::AbsSpinner));
    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributes(
            attrs, attrIds, defStyleAttr, defStyleRes, (ITypedArray**)&a);

    AutoPtr<ArrayOf<ICharSequence*> > entries;
    a->GetTextArray(R::styleable::AbsSpinner_entries, (ArrayOf<ICharSequence*>**)&entries);
    if (entries != NULL) {
        AutoPtr<ArrayOf<IInterface*> > e = ArrayOf<IInterface*>::Alloc(entries->GetLength());
        for (Int32 i = 0; i < entries->GetLength(); i++) {
            e->Set(i, (*entries)[i]);
        }
        AutoPtr<IArrayAdapter> adapter;
        CArrayAdapter::New(context, R::layout::simple_spinner_item, e, (IArrayAdapter**)&adapter);
        adapter->SetDropDownViewResource(R::layout::simple_spinner_dropdown_item);
        SetAdapter((IAdapter*)adapter->Probe(EIID_IAdapter));
    }

    a->Recycle();
    return NOERROR;
}

void AbsSpinner::InitAbsSpinner()
{
    SetFocusable(TRUE);
    SetWillNotDraw(FALSE);
}

ECode AbsSpinner::SetAdapter(
    /* [in] */ IAdapter* adapter)
{
    if (adapter != NULL && ISpinnerAdapter::Probe(adapter) == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (mAdapter != NULL) {
        IAdapter::Probe(mAdapter)->UnregisterDataSetObserver(mDataSetObserver);
        ResetList();
    }

    mAdapter = adapter == NULL ? NULL : ISpinnerAdapter::Probe(adapter);

    mOldSelectedPosition = IAdapterView::INVALID_POSITION;
    mOldSelectedRowId = IAdapterView::INVALID_ROW_ID;

    if (mAdapter != NULL) {
        mOldItemCount = mItemCount;
        IAdapter::Probe(mAdapter)->GetCount(&mItemCount);
        CheckFocus();

        mDataSetObserver = new AdapterDataSetObserver(this);
        IAdapter::Probe(mAdapter)->RegisterDataSetObserver(mDataSetObserver);

        Int32 position = mItemCount > 0 ? 0 : IAdapterView::INVALID_POSITION;

        SetSelectedPositionInt(position);
        SetNextSelectedPositionInt(position);

        if (mItemCount == 0) {
            // Nothing selected
            CheckSelectionChanged();
        }
    }
    else {
        CheckFocus();
        ResetList();
        // Nothing selected
        CheckSelectionChanged();
    }

    RequestLayout();

    return NOERROR;
}

void AbsSpinner::ResetList()
{
    mDataChanged = FALSE;
    mNeedSync = FALSE;

    RemoveAllViewsInLayout();
    mOldSelectedPosition = IAdapterView::INVALID_POSITION;
    mOldSelectedRowId = IAdapterView::INVALID_ROW_ID;

    SetSelectedPositionInt(IAdapterView::INVALID_POSITION);
    SetNextSelectedPositionInt(IAdapterView::INVALID_POSITION);
    Invalidate();
}

void AbsSpinner::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    Int32 widthMode = View::MeasureSpec::GetMode(widthMeasureSpec);
    Int32 widthSize;
    Int32 heightSize;

    mSpinnerPadding->mLeft = mPaddingLeft > mSelectionLeftPadding ? mPaddingLeft
            : mSelectionLeftPadding;
    mSpinnerPadding->mTop = mPaddingTop > mSelectionTopPadding ? mPaddingTop
            : mSelectionTopPadding;
    mSpinnerPadding->mRight = mPaddingRight > mSelectionRightPadding ? mPaddingRight
            : mSelectionRightPadding;
    mSpinnerPadding->mBottom = mPaddingBottom > mSelectionBottomPadding ? mPaddingBottom
            : mSelectionBottomPadding;

    if (mDataChanged) {
        HandleDataChanged();
    }

    Int32 preferredHeight = 0;
    Int32 preferredWidth = 0;
    Boolean needsMeasuring = TRUE;

    Int32 selectedPosition = 0;
    GetSelectedItemPosition(&selectedPosition);
    Int32 count = 0;
    if (selectedPosition >= 0 && mAdapter != NULL && selectedPosition < (IAdapter::Probe(mAdapter)->GetCount(&count), count)) {
        // Try looking in the recycler. (Maybe we were measured once already)
        AutoPtr<IView> view = mRecycler->Get(selectedPosition);
        if (view == NULL) {
            // Make a new one
            IAdapter::Probe(mAdapter)->GetView(selectedPosition, NULL, THIS_PROBE(IViewGroup), (IView**)&view);
        }

        if (view != NULL) {
            Int32 access;
            view->GetImportantForAccessibility(&access);
            if (access == IView::IMPORTANT_FOR_ACCESSIBILITY_AUTO) {
                view->SetImportantForAccessibility(IView::IMPORTANT_FOR_ACCESSIBILITY_YES);
            }

            // Put in recycler for re-measuring and/or layout
            mRecycler->Put(selectedPosition, view);

            AutoPtr<IViewGroupLayoutParams> params;
            view->GetLayoutParams((IViewGroupLayoutParams**)&params);
            if (params == NULL) {
                mBlockLayoutRequests = TRUE;
                GenerateDefaultLayoutParams((IViewGroupLayoutParams**)&params);
                view->SetLayoutParams(params);
                mBlockLayoutRequests = FALSE;
            }
            MeasureChild(view, widthMeasureSpec, heightMeasureSpec);

            preferredHeight = GetChildHeight(view) + mSpinnerPadding->mTop + mSpinnerPadding->mBottom;
            preferredWidth = GetChildWidth(view) + mSpinnerPadding->mLeft + mSpinnerPadding->mRight;

            needsMeasuring = FALSE;
        }
    }

    if (needsMeasuring) {
        // No views -- just use padding
        preferredHeight = mSpinnerPadding->mTop + mSpinnerPadding->mBottom;
        if (widthMode == View::MeasureSpec::UNSPECIFIED) {
            preferredWidth = mSpinnerPadding->mLeft + mSpinnerPadding->mRight;
        }
    }

    preferredHeight = Elastos::Core::Math::Max(preferredHeight, GetSuggestedMinimumHeight());
    preferredWidth = Elastos::Core::Math::Max(preferredWidth, GetSuggestedMinimumWidth());

    heightSize = ResolveSizeAndState(preferredHeight, heightMeasureSpec, 0);
    widthSize = ResolveSizeAndState(preferredWidth, widthMeasureSpec, 0);

    SetMeasuredDimension(widthSize, heightSize);
    mHeightMeasureSpec = heightMeasureSpec;
    mWidthMeasureSpec = widthMeasureSpec;
}

Int32 AbsSpinner::GetChildHeight(
    /* [in] */ IView* child)
{
    Int32 height;
    child->GetMeasuredHeight(&height);

    return height;
}

Int32 AbsSpinner::GetChildWidth(
    /* [in] */ IView* child)
{
    Int32 width;
    child->GetMeasuredWidth(&width);

    return width;
}

ECode AbsSpinner::GenerateDefaultLayoutParams(
    /* [out] */ IViewGroupLayoutParams** params)
{
    VALIDATE_NOT_NULL(params);
    return CViewGroupLayoutParams::New(IViewGroupLayoutParams::MATCH_PARENT,
            IViewGroupLayoutParams::WRAP_CONTENT, params);
}

void AbsSpinner::RecycleAllViews()
{
    Int32 childCount = 0;
    GetChildCount(&childCount);
    Int32 position = mFirstPosition;

    // All views go in recycler
    for (Int32 i = 0; i < childCount; i++) {
        AutoPtr<IView> v;
        GetChildAt(i, (IView**)&v);
        Int32 index = position + i;
        mRecycler->Put(index, v);
    }
}

ECode AbsSpinner::SetSelection(
    /* [in] */ Int32 position,
    /* [in] */ Boolean animate)
{
    // Animate only if requested position is already on screen somewhere
    Int32 count = 0;
    GetChildCount(&count);
    Boolean shouldAnimate = animate && mFirstPosition <= position &&
            position <= mFirstPosition + count - 1;
    SetSelectionInt(position, shouldAnimate);

    return NOERROR;
}

ECode AbsSpinner::SetSelection(
    /* [in] */ Int32 position)
{
    SetNextSelectedPositionInt(position);
    RequestLayout();
    Invalidate();

    return NOERROR;
}


/**
 * Makes the item at the supplied position selected.
 *
 * @param position Position to select
 * @param animate Should the transition be animated
 *
 */
void AbsSpinner::SetSelectionInt(
    /* [in] */ Int32 position,
    /* [in] */ Boolean animate)
{
    if (position != mOldSelectedPosition) {
        mBlockLayoutRequests = TRUE;
        Int32 delta  = position - mSelectedPosition;
        SetNextSelectedPositionInt(position);
        Layout(delta, animate);
        mBlockLayoutRequests = FALSE;
    }
}

ECode AbsSpinner::GetSelectedView(
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);
    if (mItemCount > 0 && mSelectedPosition >= 0) {
        return GetChildAt(mSelectedPosition - mFirstPosition, view);
    }

    *view = NULL;
    return NOERROR;
}

ECode AbsSpinner::RequestLayout()
{
    if (!mBlockLayoutRequests) {
        AdapterView::RequestLayout();
    }

    return NOERROR;
}

ECode AbsSpinner::GetAdapter(
    /* [out] */ IAdapter** adapter)
{
    VALIDATE_NOT_NULL(adapter);
    *adapter = IAdapter::Probe(mAdapter);
    REFCOUNT_ADD(*adapter);
    return NOERROR;
}

ECode AbsSpinner::GetCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    *count = mItemCount;
    return NOERROR;
}

ECode AbsSpinner::PointToPosition(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [out] */ Int32* position)
{
    VALIDATE_NOT_NULL(position);
    AutoPtr<IRect> frame = mTouchFrame;
    if (frame == NULL) {
        mTouchFrame = NULL;
        CRect::New((IRect**)&mTouchFrame);
        frame = mTouchFrame;
    }

    Int32 count = 0;
    GetChildCount(&count);
    for (Int32 i = count - 1; i >= 0; i--) {
        AutoPtr<IView> child;
        GetChildAt(i, (IView**)&child);
        Int32 visible;
        child->GetVisibility(&visible);
        if (visible == IView::VISIBLE) {
            child->GetHitRect(frame);

            Boolean contains;
            frame->Contains(x, y, &contains);
            if (contains) {
                *position = mFirstPosition + i;
                return NOERROR;
            }
        }
    }
    *position = IAdapterView::INVALID_POSITION;
    return NOERROR;
}

AutoPtr<IParcelable> AbsSpinner::OnSaveInstanceState()
{
    AutoPtr<IParcelable> superState = AdapterView::OnSaveInstanceState();
    assert(0 && "TODO");
    // AutoPtr<CAbsSpinnerSavedState> ss;
    // CAbsSpinnerSavedState::NewByFriend(superState, (CAbsSpinnerSavedState**)&ss);
    // ss->mSelectedId = GetSelectedItemId();
    // if (ss->mSelectedId >= 0) {
    //     ss->mPosition = GetSelectedItemPosition();
    // }
    // else {
    //     ss->mPosition = IAdapterView::INVALID_POSITION;
    // }
    // return (IParcelable*)ss->Probe(EIID_IParcelable);
    return NOERROR;
}

void AbsSpinner::OnRestoreInstanceState(
    /* [in] */ IParcelable* state)
{
    assert(0 && "TODO");
    // AutoPtr<CAbsSpinnerSavedState> ss = (CAbsSpinnerSavedState*)IAbsSpinnerSavedState::Probe(state);

    // AutoPtr<IParcelable> p;
    // ss->GetSuperState((IParcelable**)&p);
    // AdapterView::OnRestoreInstanceState(p);

    // if (ss->mSelectedId >= 0) {
    //     mDataChanged = TRUE;
    //     mNeedSync = TRUE;
    //     mSyncRowId = ss->mSelectedId;
    //     mSyncPosition = ss->mPosition;
    //     mSyncMode = AdapterView::SYNC_SELECTED_POSITION;
    //     RequestLayout();
    // }
}

AbsSpinner::RecycleBin::RecycleBin(
    /* [in] */ AbsSpinner* host)
    : mScrapHeap(5)
    , mHost(host)
{
    assert(host);
}

void AbsSpinner::RecycleBin::Put(
    /* [in] */ Int32 position,
    /* [in] */ IView* v)
{
    mScrapHeap[position] = v;
}

AutoPtr<IView> AbsSpinner::RecycleBin::Get(
    /* [in] */ Int32 position)
{
    // System.out.print("Looking for " + position);
    AutoPtr<IView> result;
    HashMap<Int32, AutoPtr<IView> >::Iterator it = mScrapHeap.Find(position);
    if (it != mScrapHeap.End()) result = it->mSecond;
    if (result != NULL) {
        // System.out.println(" HIT");
        mScrapHeap.Erase(it);
    }
    else {
        // System.out.println(" MISS");
    }
    return result;
}

void AbsSpinner::RecycleBin::Clear()
{
    HashMap<Int32, AutoPtr<IView> >::Iterator it;
    for (it = mScrapHeap.Begin(); it != mScrapHeap.End(); ++it) {
        AutoPtr<IView> view = it->mSecond;
        if (view != NULL) {
            mHost->RemoveDetachedView(view, TRUE);
        }
    }
    mScrapHeap.Clear();
}

ECode AbsSpinner::OnInitializeAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    AdapterView::OnInitializeAccessibilityEvent(event);
    String classNameStr("AbsSpinner");
    AutoPtr<ICharSequence> className;
    FAIL_RETURN(CString::New(classNameStr, (ICharSequence**)&className));
    IAccessibilityRecord::Probe(event)->SetClassName(className);
    return NOERROR;
}

ECode AbsSpinner::OnInitializeAccessibilityNodeInfo(
    /* [in] */ IAccessibilityNodeInfo* info)
{
    AdapterView::OnInitializeAccessibilityNodeInfo(info);
    String classNameStr("AbsSpinner");
    AutoPtr<ICharSequence> className;
    FAIL_RETURN(CString::New(classNameStr, (ICharSequence**)&className));
    info->SetClassName(className);
    return NOERROR;
}

}// namespace Widget
}// namespace Droid
}// namespace Widget
