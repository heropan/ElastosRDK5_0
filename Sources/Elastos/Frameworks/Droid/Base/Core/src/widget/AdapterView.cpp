
#include "widget/AdapterView.h"
#include "os/SystemClock.h"
#include "view/accessibility/CAccessibilityEvent.h"
#include "view/accessibility/CAccessibilityManager.h"
#include "view/SoundEffectConstants.h"
#include <elastos/Logger.h>
#include <elastos/Math.h>
#include <R.h>

using Elastos::Droid::View::SoundEffectConstants;

namespace Elastos {
namespace Droid {
namespace Widget {

using Elastos::Core::EIID_IRunnable;
using Elastos::Core::CStringWrapper;
using Elastos::Utility::Logging::Logger;
using Elastos::Droid::Os::SystemClock;
using Elastos::Droid::View::EIID_IView;
using Elastos::Droid::View::Accessibility::CAccessibilityEvent;
using Elastos::Droid::View::Accessibility::IAccessibilityManager;
using Elastos::Droid::View::Accessibility::IAccessibilityEventSource;
using Elastos::Droid::View::Accessibility::EIID_IAccessibilityEventSource;
using Elastos::Droid::View::Accessibility::CAccessibilityManager;

const Int32 AdapterView::SYNC_SELECTED_POSITION;
const Int32 AdapterView::SYNC_FIRST_POSITION;
const Int32 AdapterView::SYNC_MAX_DURATION_MILLIS;

//==============================================================================
//              AdapterView::AdapterContextMenuInfo
//==============================================================================
CAR_INTERFACE_IMPL(AdapterView::AdapterContextMenuInfo, IContextMenuInfo)

AdapterView::AdapterContextMenuInfo::AdapterContextMenuInfo(
    /* [in] */ IView* targetView,
    /* [in] */ Int32 position,
    /* [in] */ Int64 id)
    : mTargetView(targetView)
    , mPosition(position)
    , mId(id)
{
}

ECode AdapterView::AdapterContextMenuInfo::GetTargetView(
    /* [out] */ IView** targetView)
{
    VALIDATE_NOT_NULL(targetView);
    *targetView = mTargetView;
    INTERFACE_ADDREF(*targetView);
    return NOERROR;
}

ECode AdapterView::AdapterContextMenuInfo::SetTargetView(
    /* [in] */ IView* targetView)
{
    mTargetView = targetView;
    return NOERROR;
}

ECode AdapterView::AdapterContextMenuInfo::GetPosition(
    /* [out] */ Int32* position)
{
    VALIDATE_NOT_NULL(position);
    *position = mPosition;
    return NOERROR;
}

ECode AdapterView::AdapterContextMenuInfo::SetPosition(
    /* [in] */ Int32 position)
{
    mPosition = position;
    return NOERROR;
}

ECode AdapterView::AdapterContextMenuInfo::GetId(
    /* [out] */ Int64* id)
{
    VALIDATE_NOT_NULL(id);
    *id = mId;
    return NOERROR;
}

ECode AdapterView::AdapterContextMenuInfo::SetId(
    /* [in] */ Int64 id)
{
    mId = id;
    return NOERROR;
}

//==============================================================================
//              AdapterView::AdapterDataSetObserver
//==============================================================================
CAR_INTERFACE_IMPL(AdapterView::AdapterDataSetObserver, IDataSetObserver);

ECode AdapterView::AdapterDataSetObserver::OnChanged()
{
    mHost->mDataChanged = TRUE;
    mHost->mOldItemCount = mHost->mItemCount;

    AutoPtr<IAdapter> adapter;
    ((IAdapterView*)(mHost->Probe(EIID_IAdapterView)))->GetAdapter((IAdapter**)&adapter);
    assert(adapter.Get());
    adapter->GetCount(&mHost->mItemCount);

    // Detect the case where a cursor that was previously invalidated has
    // been repopulated with new data.
    Boolean hasStableIds;
    adapter->HasStableIds(&hasStableIds);
    if (hasStableIds && mInstanceState != NULL
        && mHost->mOldItemCount == 0 && mHost->mItemCount > 0)
    {
        mHost->OnRestoreInstanceState(mInstanceState);
        mInstanceState = NULL;
    }
    else {
        mHost->RememberSyncState();
    }
    mHost->CheckFocus();
    mHost->RequestLayout();

    return NOERROR;
}

//@Override
ECode AdapterView::AdapterDataSetObserver::OnInvalidated()
{
    mHost->mDataChanged = TRUE;

    AutoPtr<IAdapter> adapter;
    ((IAdapterView*)(mHost->Probe(EIID_IAdapterView)))->GetAdapter((IAdapter**)&adapter);
    assert(adapter.Get());
    Boolean hasStableIds;
    adapter->HasStableIds(&hasStableIds);
    if (hasStableIds) {
        // Remember the current state for the case where our hosting activity is being
        // stopped and later restarted
        mInstanceState = mHost->OnSaveInstanceState();
    }

    // Data is invalid so we should reset our state
    mHost->mOldItemCount = mHost->mItemCount;
    mHost->mItemCount = 0;
    mHost->mSelectedPosition = IAdapterView::INVALID_POSITION;
    mHost->mSelectedRowId = IAdapterView::INVALID_ROW_ID;
    mHost->mNextSelectedPosition = IAdapterView::INVALID_POSITION;
    mHost->mNextSelectedRowId = IAdapterView::INVALID_POSITION;
    mHost->mNeedSync = FALSE;

    mHost->CheckFocus();
    mHost->RequestLayout();

    return NOERROR;
}

ECode AdapterView::AdapterDataSetObserver::ClearSavedState()
{
    mInstanceState = NULL;

    return NOERROR;
}

//==============================================================================
//              AdapterView::SelectionNotifier
//==============================================================================

AdapterView::SelectionNotifier::SelectionNotifier(
    /* [in] */ AdapterView* host)
    : mHost(host)
{
    assert(host);
}

ECode AdapterView::SelectionNotifier::Run()
{
    if (mHost->mDataChanged) {
        // Data has changed between when this SelectionNotifier
        // was posted and now. We need to wait until the AdapterView
        // has been synched to the new data.
        AutoPtr<IAdapter> adapter;
        ((IAdapterView*)(mHost->Probe(EIID_IAdapterView)))->GetAdapter((IAdapter**)&adapter);
        if (adapter != NULL) {
            AutoPtr<IRunnable> r = (IRunnable*)this->Probe(EIID_IRunnable);
            mHost->Post(r);
        }
    }
    else {
        mHost->FireOnSelected();
        mHost->PerformAccessibilityActionsOnSelected();
    }
    return NOERROR;
}

//==============================================================================
//              AdapterView
//==============================================================================
AdapterView::AdapterView()
    : mFirstPosition(0)
    , mSpecificTop(0)
    , mSyncPosition(0)
    , mSyncRowId(IAdapterView::INVALID_ROW_ID)
    , mSyncHeight(0)
    , mNeedSync(FALSE)
    , mSyncMode(0)
    , mInLayout(FALSE)
    , mDataChanged(FALSE)
    , mNextSelectedPosition(IAdapterView::INVALID_POSITION)
    , mNextSelectedRowId(IAdapterView::INVALID_ROW_ID)
    , mSelectedPosition(IAdapterView::INVALID_POSITION)
    , mSelectedRowId(IAdapterView::INVALID_ROW_ID)
    , mItemCount(0)
    , mOldItemCount(0)
    , mOldSelectedPosition(IAdapterView::INVALID_POSITION)
    , mOldSelectedRowId(IAdapterView::INVALID_ROW_ID)
    , mBlockLayoutRequests(FALSE)
    , mLayoutHeight(0)
    , mDesiredFocusableState(FALSE)
    , mDesiredFocusableInTouchModeState(FALSE)
{
}

AdapterView::AdapterView(
    /* [in] */ IContext* context)
    : ViewGroup(context)
    , mFirstPosition(0)
    , mSpecificTop(0)
    , mSyncPosition(0)
    , mSyncRowId(IAdapterView::INVALID_ROW_ID)
    , mSyncHeight(0)
    , mNeedSync(FALSE)
    , mSyncMode(0)
    , mInLayout(FALSE)
    , mDataChanged(FALSE)
    , mNextSelectedPosition(IAdapterView::INVALID_POSITION)
    , mNextSelectedRowId(IAdapterView::INVALID_ROW_ID)
    , mSelectedPosition(IAdapterView::INVALID_POSITION)
    , mSelectedRowId(IAdapterView::INVALID_ROW_ID)
    , mItemCount(0)
    , mOldItemCount(0)
    , mOldSelectedPosition(IAdapterView::INVALID_POSITION)
    , mOldSelectedRowId(IAdapterView::INVALID_ROW_ID)
    , mBlockLayoutRequests(FALSE)
    , mLayoutHeight(0)
    , mDesiredFocusableState(FALSE)
    , mDesiredFocusableInTouchModeState(FALSE)
{
}

AdapterView::AdapterView(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
    : ViewGroup(context, attrs)
    , mFirstPosition(0)
    , mSpecificTop(0)
    , mSyncPosition(0)
    , mSyncRowId(IAdapterView::INVALID_ROW_ID)
    , mSyncHeight(0)
    , mNeedSync(FALSE)
    , mSyncMode(0)
    , mInLayout(FALSE)
    , mDataChanged(FALSE)
    , mNextSelectedPosition(IAdapterView::INVALID_POSITION)
    , mNextSelectedRowId(IAdapterView::INVALID_ROW_ID)
    , mSelectedPosition(IAdapterView::INVALID_POSITION)
    , mSelectedRowId(IAdapterView::INVALID_ROW_ID)
    , mItemCount(0)
    , mOldItemCount(0)
    , mOldSelectedPosition(IAdapterView::INVALID_POSITION)
    , mOldSelectedRowId(IAdapterView::INVALID_ROW_ID)
    , mBlockLayoutRequests(FALSE)
    , mLayoutHeight(0)
    , mDesiredFocusableState(FALSE)
    , mDesiredFocusableInTouchModeState(FALSE)
{
}

AdapterView::AdapterView(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
    : ViewGroup(context, attrs, defStyle)
    , mFirstPosition(0)
    , mSpecificTop(0)
    , mSyncPosition(0)
    , mSyncRowId(IAdapterView::INVALID_ROW_ID)
    , mSyncHeight(0)
    , mNeedSync(FALSE)
    , mSyncMode(0)
    , mInLayout(FALSE)
    , mDataChanged(FALSE)
    , mNextSelectedPosition(IAdapterView::INVALID_POSITION)
    , mNextSelectedRowId(IAdapterView::INVALID_ROW_ID)
    , mSelectedPosition(IAdapterView::INVALID_POSITION)
    , mSelectedRowId(IAdapterView::INVALID_ROW_ID)
    , mItemCount(0)
    , mOldItemCount(0)
    , mOldSelectedPosition(IAdapterView::INVALID_POSITION)
    , mOldSelectedRowId(IAdapterView::INVALID_ROW_ID)
    , mBlockLayoutRequests(FALSE)
    , mLayoutHeight(0)
    , mDesiredFocusableState(FALSE)
    , mDesiredFocusableInTouchModeState(FALSE)
{
    // If not explicitly specified this view is important for accessibility.
    if (GetImportantForAccessibility() == IView::IMPORTANT_FOR_ACCESSIBILITY_AUTO) {
        SetImportantForAccessibility(IView::IMPORTANT_FOR_ACCESSIBILITY_YES);
    }
}

/**
 * Register a callback to be invoked when an item in this AdapterView has
 * been clicked.
 *
 * @param listener The callback that will be invoked.
 */
ECode AdapterView::SetOnItemClickListener(
    /* [in] */ IAdapterViewOnItemClickListener* listener)
{
    mOnItemClickListener = listener;
    return NOERROR;
}

/**
 * @return The callback to be invoked with an item in this AdapterView has
 *         been clicked, or NULL id no callback has been set.
 */
AutoPtr<IAdapterViewOnItemClickListener> AdapterView::GetOnItemClickListener()
{
    return mOnItemClickListener;
}

/**
 * Call the OnItemClickListener, if it is defined.
 *
 * @param view The view within the AdapterView that was clicked.
 * @param position The position of the view in the adapter.
 * @param id The row id of the item that was clicked.
 * @return True if there was an assigned OnItemClickListener that was
 *         called, FALSE otherwise is returned.
 */
Boolean AdapterView::PerformItemClick(
    /* [in] */ IView* view,
    /* [in] */ Int32 position,
    /* [in] */ Int64 id)
{
    if (mOnItemClickListener != NULL) {
        PlaySoundEffect(SoundEffectConstants::CLICK);
        if (view != NULL) {
           AutoPtr<IAccessibilityEventSource> temp = IAccessibilityEventSource::Probe(view);
           temp->SendAccessibilityEvent(IAccessibilityEvent::TYPE_VIEW_CLICKED);
        }
        mOnItemClickListener->OnItemClick(THIS_PROBE(IAdapterView), view, position, id);
        return TRUE;
    }

    return FALSE;
}

/**
 * Register a callback to be invoked when an item in this AdapterView has
 * been clicked and held
 *
 * @param listener The callback that will run
 */
ECode AdapterView::SetOnItemLongClickListener(
    /* [in] */ IAdapterViewOnItemLongClickListener* listener)
{
    if (!IsLongClickable()) {
        SetLongClickable(TRUE);
    }
    mOnItemLongClickListener = listener;

    return NOERROR;
}

/**
 * @return The callback to be invoked with an item in this AdapterView has
 *         been clicked and held, or NULL id no callback as been set.
 */
AutoPtr<IAdapterViewOnItemLongClickListener> AdapterView::GetOnItemLongClickListener()
{
    return mOnItemLongClickListener;
}

/**
 * Register a callback to be invoked when an item in this AdapterView has
 * been selected.
 *
 * @param listener The callback that will run
 */
ECode AdapterView::SetOnItemSelectedListener(
    /* [in] */ IAdapterViewOnItemSelectedListener* listener)
{
    mOnItemSelectedListener = listener;

    return NOERROR;
}

AutoPtr<IAdapterViewOnItemSelectedListener> AdapterView::GetOnItemSelectedListener()
{
    return mOnItemSelectedListener;
}

/**
 * This method is not supported and throws an UnsupportedOperationException when called.
 *
 * @param child Ignored.
 *
 * @throws UnsupportedOperationException Every time this method is invoked.
 */
//@Override
ECode AdapterView::AddView(
    /* [in] */ IView* child)
{
    Logger::E("AdapterView", "addView(View) is not supported in AdapterView");

    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

/**
 * This method is not supported and throws an UnsupportedOperationException when called.
 *
 * @param child Ignored.
 * @param index Ignored.
 *
 * @throws UnsupportedOperationException Every time this method is invoked.
 */
//@Override
ECode AdapterView::AddView(
    /* [in] */ IView* child,
    /* [in] */ Int32 index)
{
    Logger::E("AdapterView",
        "addView(View, Int32) is not supported in AdapterView");

    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode AdapterView::AddView(
    /* [in] */ IView* child,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    Logger::E("AdapterView",
        "addView(View, Int32) is not supported in AdapterView");

    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

/**
 * This method is not supported and throws an UnsupportedOperationException when called.
 *
 * @param child Ignored.
 * @param params Ignored.
 *
 * @throws UnsupportedOperationException Every time this method is invoked.
 */
//@Override
ECode AdapterView::AddView(
    /* [in] */ IView* child,
    /* [in] */ IViewGroupLayoutParams* params)
{
    Logger::E("AdapterView",
        "addView(View, LayoutParams) is not supported in AdapterView");

    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

/**
 * This method is not supported and throws an UnsupportedOperationException when called.
 *
 * @param child Ignored.
 * @param index Ignored.
 * @param params Ignored.
 *
 * @throws UnsupportedOperationException Every time this method is invoked.
 */
//@Override
ECode AdapterView::AddView(
    /* [in] */ IView* child,
    /* [in] */ Int32 index,
    /* [in] */ IViewGroupLayoutParams* params)
{
    Logger::E("AdapterView",
        "addView(View, Int32, LayoutParams) is not supported in AdapterView");

    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

/**
 * This method is not supported and throws an UnsupportedOperationException when called.
 *
 * @param child Ignored.
 *
 * @throws UnsupportedOperationException Every time this method is invoked.
 */
//@Override
ECode AdapterView::RemoveView(
    /* [in] */ IView* child)
{
    Logger::E("AdapterView",
        "removeView(View) is not supported in AdapterView");

    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

/**
 * This method is not supported and throws an UnsupportedOperationException when called.
 *
 * @param index Ignored.
 *
 * @throws UnsupportedOperationException Every time this method is invoked.
 */
//@Override
ECode AdapterView::RemoveViewAt(
    /* [in] */ Int32 index)
{
    Logger::E("AdapterView",
        "removeViewAt(Int32) is not supported in AdapterView");

    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

/**
 * This method is not supported and throws an UnsupportedOperationException when called.
 *
 * @throws UnsupportedOperationException Every time this method is invoked.
 */
//@Override
ECode AdapterView::RemoveAllViews()
{
    Logger::E("AdapterView",
        "removeAllViews() is not supported in AdapterView");

    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

//@Override
void AdapterView::OnLayout(
    /* [in] */ Boolean changed,
    /* [in] */ Int32 l,
    /* [in] */ Int32 t,
    /* [in] */ Int32 r,
    /* [in] */ Int32 b)
{
    mLayoutHeight = GetHeight();
}

/**
 * Return the position of the currently selected item within the adapter's data set
 *
 * @return Int32 Position (starting at 0), or {@link #IAdapterView::INVALID_POSITION} if there is nothing selected.
 */
//@ViewDebug.CapturedViewProperty
Int32 AdapterView::GetSelectedItemPosition()
{
    return mNextSelectedPosition;
}

/**
 * @return The id corresponding to the currently selected item, or {@link #IAdapterView::INVALID_ROW_ID}
 * if nothing is selected.
 */
//@ViewDebug.CapturedViewProperty
Int64 AdapterView::GetSelectedItemId()
{
    return mNextSelectedRowId;
}

/**
 * @return The data corresponding to the currently selected item, or
 * NULL if there is nothing selected.
 */
AutoPtr<IInterface> AdapterView::GetSelectedItem()
{
    AutoPtr<IAdapter> adapter;
    THIS_PROBE(IAdapterView)->GetAdapter((IAdapter**)&adapter);
    Int32 selection = GetSelectedItemPosition();
    if (adapter != NULL) {
        Int32 count;
        adapter->GetCount(&count);
        if (count > 0 && selection >= 0) {
            AutoPtr<IInterface> item;
            adapter->GetItem(selection, (IInterface**)&item);
            return item;
        }
    }

    return AutoPtr<IInterface>(NULL);
}

/**
 * @return The number of items owned by the Adapter associated with this
 *         AdapterView. (This is the number of data items, which may be
 *         larger than the number of visible view.)
 */
//@ViewDebug.CapturedViewProperty
Int32 AdapterView::GetCount()
{
    return mItemCount;
}

/**
 * Get the position within the adapter's data set for the view, where view is a an adapter item
 * or a descendant of an adapter item.
 *
 * @param view an adapter item, or a descendant of an adapter item. This must be visible in this
 *        AdapterView at the time of the call.
 * @return the position within the adapter's data set of the view, or {@link #IAdapterView::INVALID_POSITION}
 *         if the view does not correspond to a list item (or it is not currently visible).
 */
Int32 AdapterView::GetPositionForView(
    /* [in] */ IView* view)
{
    assert(view);
    AutoPtr<IView> listItem = view;
    AutoPtr<IViewParent> vp;
    listItem->GetParent((IViewParent**)&vp);
    AutoPtr<IView> v = IView::Probe(vp);
    while (v.Get() != THIS_PROBE(IView)) {
        if (v == NULL) {
            return IAdapterView::INVALID_POSITION;
        }
        listItem = v;
        vp = NULL;
        listItem->GetParent((IViewParent**)&vp);
        v = IView::Probe(vp);
    }

    // Search the children for the list item
    Int32 childCount = GetChildCount();
    for (Int32 i = 0; i < childCount; i++) {
        AutoPtr<IView> child = GetChildAt(i);
        if (child.Get() == listItem) {
            return mFirstPosition + i;
        }
    }

    // Child not found!
    return IAdapterView::INVALID_POSITION;
}

/**
 * Returns the position within the adapter's data set for the first item
 * displayed on screen.
 *
 * @return The position within the adapter's data set
 */
Int32 AdapterView::GetFirstVisiblePosition()
{
    return mFirstPosition;
}

/**
 * Returns the position within the adapter's data set for the last item
 * displayed on screen.
 *
 * @return The position within the adapter's data set
 */
Int32 AdapterView::GetLastVisiblePosition()
{
    return mFirstPosition + GetChildCount() - 1;
}

/**
 * Sets the view to show if the adapter is empty
 */
ECode AdapterView::SetEmptyView(
    /* [in] */ IView* emptyView)
{
    mEmptyView = emptyView;

    Int32 accessibility;
    emptyView->GetImportantForAccessibility(&accessibility);
    // If not explicitly specified this view is important for accessibility.
    if (emptyView != NULL
        &&  accessibility== IView::IMPORTANT_FOR_ACCESSIBILITY_AUTO) {
            emptyView->SetImportantForAccessibility(IView::IMPORTANT_FOR_ACCESSIBILITY_YES);
    }

    AutoPtr<IAdapter> adapter;
    THIS_PROBE(IAdapterView)->GetAdapter((IAdapter**)&adapter);
    Boolean empty = TRUE;
    if (adapter != NULL) {
        adapter->IsEmpty(&empty);
    }
    UpdateEmptyStatus(empty);

    return NOERROR;
}

/**
 * When the current adapter is empty, the AdapterView can display a special view
 * call the empty view. The empty view is used to provide feedback to the user
 * that no data is available in this AdapterView.
 *
 * @return The view to show if the adapter is empty.
 */
AutoPtr<IView> AdapterView::GetEmptyView()
{
    return mEmptyView;
}

/**
 * Indicates whether this view is in filter mode. Filter mode can for instance
 * be enabled by a user when typing on the keyboard.
 *
 * @return True if the view is in filter mode, FALSE otherwise.
 */
Boolean AdapterView::IsInFilterMode() {
    return FALSE;
}

//@Override
ECode AdapterView::SetFocusable(
    /* [in] */ Boolean focusable)
{
    AutoPtr<IAdapter> adapter;
    THIS_PROBE(IAdapterView)->GetAdapter((IAdapter**)&adapter);
    Boolean empty = TRUE;
    if (adapter != NULL) {
        Int32 count;
        adapter->GetCount(&count);
        empty = (count == 0);
    }

    mDesiredFocusableState = focusable;
    if (!focusable) {
        mDesiredFocusableInTouchModeState = FALSE;
    }

    return ViewGroup::SetFocusable(focusable && (!empty || IsInFilterMode()));
}

//@Override
ECode AdapterView::SetFocusableInTouchMode(
    /* [in] */ Boolean focusable)
{
    AutoPtr<IAdapter> adapter;
    THIS_PROBE(IAdapterView)->GetAdapter((IAdapter**)&adapter);
    Boolean empty = TRUE;
    if (adapter != NULL) {
        Int32 count;
        adapter->GetCount(&count);
        empty = (count == 0);
    }

    mDesiredFocusableInTouchModeState = focusable;
    if (focusable) {
        mDesiredFocusableState = TRUE;
    }

    return ViewGroup::SetFocusableInTouchMode(
        focusable && (!empty || IsInFilterMode()));
}

ECode AdapterView::CheckFocus()
{
    AutoPtr<IAdapter> adapter;
    THIS_PROBE(IAdapterView)->GetAdapter((IAdapter**)&adapter);
    Boolean empty = TRUE;
    if (adapter != NULL) {
        Int32 count;
        adapter->GetCount(&count);
        empty = (count == 0);
    }

    Boolean focusable = !empty || IsInFilterMode();
    // The order in which we set focusable in touch mode/focusable may matter
    // for the client, see View.setFocusableInTouchMode() comments for more
    // details
    ViewGroup::SetFocusableInTouchMode(focusable && mDesiredFocusableInTouchModeState);
    ViewGroup::SetFocusable(focusable && mDesiredFocusableState);
    if (mEmptyView != NULL) {
        Boolean isEmpty = TRUE;
        if (adapter != NULL) {
            adapter->IsEmpty(&isEmpty);
        }
        UpdateEmptyStatus(isEmpty);
    }

    return NOERROR;
}

/**
 * Update the status of the list based on the empty parameter.  If empty is TRUE and
 * we have an empty view, display it.  In all the other cases, make sure that the listview
 * is VISIBLE and that the empty view is GONE (if it's not NULL).
 */
void AdapterView::UpdateEmptyStatus(
    /* [in] */ Boolean empty)
{
    if (IsInFilterMode()) {
        empty = FALSE;
    }

    if (empty) {
        if (mEmptyView != NULL) {
            mEmptyView->SetVisibility(IView::VISIBLE);
            SetVisibility(IView::GONE);
        }
        else {
            // If the caller just removed our empty view, make sure the list view is visible
            SetVisibility(IView::VISIBLE);
        }

        // We are now GONE, so pending layouts will not be dispatched.
        // Force one here to make sure that the state of the list matches
        // the state of the adapter.
        if (mDataChanged) {
            OnLayout(FALSE, mLeft, mTop, mRight, mBottom);
        }
    }
    else {
        if (mEmptyView != NULL) {
            mEmptyView->SetVisibility(IView::GONE);
        }
        SetVisibility(IView::VISIBLE);
    }
}

/**
 * Gets the data associated with the specified position in the list.
 *
 * @param position Which data to get
 * @return The data associated with the specified position in the list
 */
AutoPtr<IInterface> AdapterView::GetItemAtPosition(
    /* [in] */ Int32 position)
{
    AutoPtr<IAdapter> adapter;
    THIS_PROBE(IAdapterView)->GetAdapter((IAdapter**)&adapter);
    if (adapter == NULL || position < 0) {
        return AutoPtr<IInterface>(NULL);
    }
    else {
        AutoPtr<IInterface> item;
        adapter->GetItem(position, (IInterface**)&item);
        return item;
    }
}

Int64 AdapterView::GetItemIdAtPosition(
    /* [in] */ Int32 position)
{
    AutoPtr<IAdapter> adapter;
    THIS_PROBE(IAdapterView)->GetAdapter((IAdapter**)&adapter);
    if (adapter == NULL || position < 0) {
        return IAdapterView::INVALID_ROW_ID;
    }
    else {
        Int64 itemId;
        adapter->GetItemId(position, &itemId);
        return itemId;
    }
}

//@Override
ECode AdapterView::SetOnClickListener(
    /* [in] */ IViewOnClickListener* l)
{
    Logger::E("AdapterView", "Don't call setOnClickListener for an AdapterView. "
            "You probably want setOnItemClickListener instead");

    return E_RUNTIME_EXCEPTION;
}

/**
 * Override to prevent freezing of any views created by the adapter.
 */
//@Override
ECode AdapterView::DispatchSaveInstanceState(
    /* [in] */ IObjectInt32Map* container)
{
    return DispatchFreezeSelfOnly(container);
}

/**
 * Override to prevent thawing of any views created by the adapter.
 */
//@Override
ECode AdapterView::DispatchRestoreInstanceState(
    /* [in] */ IObjectInt32Map* container)
{
    return DispatchThawSelfOnly(container);
}

//@Override
ECode AdapterView::OnDetachedFromWindow()
{
    ViewGroup::OnDetachedFromWindow();
    if (mSelectionNotifier != NULL)
        RemoveCallbacks(mSelectionNotifier);
    return NOERROR;
}

void AdapterView::SelectionChanged()
{
    AutoPtr<IAccessibilityManager> manager;
    Boolean enable = TRUE;
    CAccessibilityManager::GetInstance(mContext, (IAccessibilityManager**)&manager);
    manager->IsEnabled(&enable);
    if (mOnItemSelectedListener != NULL || enable) {
        if (mInLayout || mBlockLayoutRequests) {
            // If we are in a layout traversal, defer notification
            // by posting. This ensures that the view tree is
            // in a consistent state and is able to accomodate
            // new layout or invalidate requests.
            if (mSelectionNotifier == NULL) {
                mSelectionNotifier = new SelectionNotifier(this);
            }
            Post(mSelectionNotifier);
        }
        else {
            FireOnSelected();
            PerformAccessibilityActionsOnSelected();
        }
    }
}

void AdapterView::FireOnSelected()
{
    if (mOnItemSelectedListener == NULL) {
        return;
    }

    Int32 selection = GetSelectedItemPosition();
    if (selection >= 0) {
        AutoPtr<IView> v = GetSelectedView();
        AutoPtr<IAdapter> adapter;
        THIS_PROBE(IAdapterView)->GetAdapter((IAdapter**)&adapter);
        Int64 itemId;
        adapter->GetItemId(selection, &itemId);

        mOnItemSelectedListener->OnItemSelected(
            THIS_PROBE(IAdapterView),
            v, selection, itemId);
    }
    else {
        mOnItemSelectedListener->OnNothingSelected(
            THIS_PROBE(IAdapterView));
    }
}

void AdapterView::PerformAccessibilityActionsOnSelected()
{
    AutoPtr<IAccessibilityManager> manager;
    Boolean enable = TRUE;
    CAccessibilityManager::GetInstance(mContext, (IAccessibilityManager**)&manager);
    manager->IsEnabled(&enable);
    if (!enable) {
        return;
    }
    Int32 position = GetSelectedItemPosition();
    if (position >= 0) {
        // we fire selection events here not in View
        SendAccessibilityEvent(IAccessibilityEvent::TYPE_VIEW_SELECTED);
    }
}

Boolean AdapterView::DispatchPopulateAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    AutoPtr<IView> selectedView = GetSelectedView();
    Int32 visible;
    Boolean hasDispatch;
    selectedView->GetVisibility(&visible);
    selectedView->DispatchPopulateAccessibilityEvent(event, &hasDispatch);
    if (selectedView != NULL && visible == IView::VISIBLE
        && hasDispatch) {
            return TRUE;
    }
    return FALSE;
}

//@Override
Boolean AdapterView::OnRequestSendAccessibilityEvent(
    /* [in] */ IView* child,
    /* [in] */ IAccessibilityEvent* event)
{
    if (ViewGroup::OnRequestSendAccessibilityEvent(child, event)) {
        // Add a record for ourselves as well.

        AutoPtr<IAccessibilityEvent> record;
        CAccessibilityEvent::ObtainEx2((IAccessibilityEvent**)&record);
        OnInitializeAccessibilityEvent(record);
        // Populate with the text of the requesting child.
        Boolean temp;
        child->DispatchPopulateAccessibilityEvent(record, &temp);
        event->AppendRecord(record);
        return TRUE;
    }
    return FALSE;
}

//@Override
ECode AdapterView::OnInitializeAccessibilityNodeInfo(
    /* [in] */ IAccessibilityNodeInfo* info)
{
    View::OnInitializeAccessibilityNodeInfo(info);
    String classNameStr("AdapterView");
    AutoPtr<ICharSequence> className;
    FAIL_RETURN(CStringWrapper::New(classNameStr, (ICharSequence**)&className));
    info->SetClassName(className);
    info->SetScrollable(IsScrollableForAccessibility());
    AutoPtr<IView> selectedView = GetSelectedView();
    if (selectedView != NULL) {
        Boolean enabled;
        selectedView->IsEnabled(&enabled);
        info->SetEnabled(enabled);
    }
    return NOERROR;
}

//@Override
ECode AdapterView::OnInitializeAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    View::OnInitializeAccessibilityEvent(event);
    String classNameStr("AdapterView");
    AutoPtr<ICharSequence> className;
    FAIL_RETURN(CStringWrapper::New(classNameStr, (ICharSequence**)&className));
    event->SetClassName(className);
    event->SetScrollable(IsScrollableForAccessibility());
    AutoPtr<IView> selectedView = GetSelectedView();
    if (selectedView != NULL) {
        Boolean enabled;
        selectedView->IsEnabled(&enabled);
        event->SetEnabled(enabled);
    }
    event->SetCurrentItemIndex(GetSelectedItemPosition());
    event->SetFromIndex(GetFirstVisiblePosition());
    event->SetToIndex(GetLastVisiblePosition());
    event->SetItemCount(GetCount());
    return NOERROR;
}

Boolean AdapterView::IsScrollableForAccessibility()
{
    AutoPtr<IAdapter> adapter;
    THIS_PROBE(IAdapterView)->GetAdapter((IAdapter**)&adapter);
    if (adapter != NULL) {
        Int32 itemCount;
        adapter->GetCount(&itemCount);
        return itemCount > 0
            && (GetFirstVisiblePosition() > 0 || GetLastVisiblePosition() < itemCount - 1);
    }
    return FALSE;
}

Boolean AdapterView::CanAnimate()
{
    return ViewGroup::CanAnimate() && mItemCount > 0;
}

void AdapterView::HandleDataChanged()
{
    Int32 count = mItemCount;
    Boolean found = FALSE;

    if (count > 0) {
        Int32 newPos;

        // Find the row we are supposed to sync to
        if (mNeedSync) {
            // Update this first, since setNextSelectedPositionInt inspects
            // it
            mNeedSync = FALSE;

            // See if we can find a position in the new data with the same
            // id as the old selection
            //
            newPos = FindSyncPosition();
            if (newPos >= 0) {
                // Verify that new selection is selectable
                Int32 selectablePos = LookForSelectablePosition(newPos, TRUE);
                if (selectablePos == newPos) {
                    // Same row id is selected
                    SetNextSelectedPositionInt(newPos);
                    found = TRUE;
                }
            }
        }

        if (!found) {
            // Try to use the same position if we can't find matching data
            newPos = GetSelectedItemPosition();

            // Pin position to the available range
            if (newPos >= count) {
                newPos = count - 1;
            }

            if (newPos < 0) {
                newPos = 0;
            }

            // Make sure we select something selectable -- first look down
            Int32 selectablePos = LookForSelectablePosition(newPos, TRUE);
            if (selectablePos < 0) {
                // Looking down didn't work -- try looking up
                selectablePos = LookForSelectablePosition(newPos, FALSE);
            }

            if (selectablePos >= 0) {
                SetNextSelectedPositionInt(selectablePos);
                CheckSelectionChanged();
                found = TRUE;
            }
        }
    }

    if (!found) {
        // Nothing is selected
        mSelectedPosition = IAdapterView::INVALID_POSITION;
        mSelectedRowId = IAdapterView::INVALID_ROW_ID;
        mNextSelectedPosition = IAdapterView::INVALID_POSITION;
        mNextSelectedRowId = IAdapterView::INVALID_ROW_ID;
        mNeedSync = FALSE;
        CheckSelectionChanged();
    }

    //TODO: Hmm, we do not know the old state so this is sub-optimal
    NotifyAccessibilityStateChanged();

}

void AdapterView::CheckSelectionChanged()
{
    if ((mSelectedPosition != mOldSelectedPosition)
        || (mSelectedRowId != mOldSelectedRowId)) {
        SelectionChanged();
        mOldSelectedPosition = mSelectedPosition;
        mOldSelectedRowId = mSelectedRowId;
    }
}

/**
 * Searches the adapter for a position matching mSyncRowId. The search starts at mSyncPosition
 * and then alternates between moving up and moving down until 1) we find the right position, or
 * 2) we run out of time, or 3) we have looked at every position
 *
 * @return Position of the row that matches mSyncRowId, or {@link #IAdapterView::INVALID_POSITION} if it can't
 *         be found
 */
Int32 AdapterView::FindSyncPosition()
{
    Int32 count = mItemCount;

    if (count == 0) {
        return IAdapterView::INVALID_POSITION;
    }

    Int64 idToMatch = mSyncRowId;
    Int32 seed = mSyncPosition;

    // If there isn't a selection don't hunt for it
    if (idToMatch == IAdapterView::INVALID_ROW_ID) {
        return IAdapterView::INVALID_POSITION;
    }

    // Pin seed to reasonable values
    seed = Elastos::Core::Math::Max(0, seed);
    seed = Elastos::Core::Math::Min(count - 1, seed);

    Int64 endTime = SystemClock::GetUptimeMillis() + SYNC_MAX_DURATION_MILLIS;
    Int64 rowId;

    // first position scanned so far
    Int32 first = seed;

    // last position scanned so far
    Int32 last = seed;

    // True if we should move down on the next iteration
    Boolean next = FALSE;

    // True when we have looked at the first item in the data
    Boolean hitFirst;

    // True when we have looked at the last item in the data
    Boolean hitLast;

    // Get the item ID locally (instead of getItemIdAtPosition), so
    // we need the adapter
    AutoPtr<IAdapter> adapter;
    THIS_PROBE(IAdapterView)->GetAdapter((IAdapter**)&adapter);
    if (adapter == NULL) {
        return IAdapterView::INVALID_POSITION;
    }

    while (SystemClock::GetUptimeMillis() <= endTime) {
        adapter->GetItemId(seed, &rowId);
        if (rowId == idToMatch) {
            // Found it!
            return seed;
        }

        hitLast = last == count - 1;
        hitFirst = first == 0;

        if (hitLast && hitFirst) {
            // Looked at everything
            break;
        }

        if (hitFirst || (next && !hitLast)) {
            // Either we hit the top, or we are trying to move down
            last++;
            seed = last;
            // Try going up next time
            next = FALSE;
        }
        else if (hitLast || (!next && !hitFirst)) {
            // Either we hit the bottom, or we are trying to move up
            first--;
            seed = first;
            // Try going down next time
            next = TRUE;
        }
    }

    return IAdapterView::INVALID_POSITION;
}

/**
 * Find a position that can be selected (i.e., is not a separator).
 *
 * @param position The starting position to look at.
 * @param lookDown Whether to look down for other positions.
 * @return The next selectable position starting at position and then searching either up or
 *         down. Returns {@link #IAdapterView::INVALID_POSITION} if nothing can be found.
 */
Int32 AdapterView::LookForSelectablePosition(
    /* [in] */ Int32 position,
    /* [in] */ Boolean lookDown)
{
    return position;
}

/**
 * Utility to keep mSelectedPosition and mSelectedRowId in sync
 * @param position Our current position
 */
void AdapterView::SetSelectedPositionInt(
    /* [in] */ Int32 position)
{
    mSelectedPosition = position;
    mSelectedRowId = GetItemIdAtPosition(position);
}

/**
 * Utility to keep mNextSelectedPosition and mNextSelectedRowId in sync
 * @param position Intended value for mSelectedPosition the next time we go
 * through layout
 */
void AdapterView::SetNextSelectedPositionInt(
    /* [in] */ Int32 position)
{
    mNextSelectedPosition = position;
    mNextSelectedRowId = GetItemIdAtPosition(position);
    // If we are trying to sync to the selection, update that too
    if (mNeedSync && mSyncMode == SYNC_SELECTED_POSITION && position >= 0) {
        mSyncPosition = position;
        mSyncRowId = mNextSelectedRowId;
    }
}

/**
 * Remember enough information to restore the screen state when the data has
 * changed.
 *
 */
void AdapterView::RememberSyncState()
{
    if (GetChildCount() > 0) {
        mNeedSync = TRUE;
        mSyncHeight = mLayoutHeight;
        if (mSelectedPosition >= 0) {
            // Sync the selection state
            AutoPtr<IView> v = GetChildAt(mSelectedPosition - mFirstPosition);
            mSyncRowId = mNextSelectedRowId;
            mSyncPosition = mNextSelectedPosition;
            if (v != NULL) {
                v->GetTop(&mSpecificTop);
            }
            mSyncMode = SYNC_SELECTED_POSITION;
        }
        else {
            // Sync the based on the offset of the first view
            AutoPtr<IView> v = GetChildAt(0);
            AutoPtr<IAdapter> adapter;
            THIS_PROBE(IAdapterView)->GetAdapter((IAdapter**)&adapter);
            Int32 count;
            adapter->GetCount(&count);

            if (mFirstPosition >= 0 && mFirstPosition < count) {
                adapter->GetItemId(mFirstPosition, &mSyncRowId);
            }
            else {
                mSyncRowId = IView::NO_ID;
            }

            mSyncPosition = mFirstPosition;
            if (v != NULL) {
                v->GetTop(&mSpecificTop);
            }
            mSyncMode = SYNC_FIRST_POSITION;
        }
    }
}

ECode AdapterView::Init(
    /* [in] */ IContext* context)
{
    ViewGroup::Init(context);
    return NOERROR;
}

ECode AdapterView::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    ViewGroup::Init(context, attrs);
    return NOERROR;
}

ECode AdapterView::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    ViewGroup::Init(context, attrs, defStyle);

    if(GetImportantForAccessibility() == IView::IMPORTANT_FOR_ACCESSIBILITY_AUTO)
    {
        SetImportantForAccessibility(IView::IMPORTANT_FOR_ACCESSIBILITY_YES);
    }
    return NOERROR;
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
