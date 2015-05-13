
#include "widget/CursorTreeAdapter.h"
#include "widget/CCursorFilter.h"
#include "os/CHandler.h"

using Elastos::Droid::Os::CHandler;

/**
 * Constructor. The adapter will call {@link Cursor#requery()} on the cursor whenever
 * it changes so that the most recent data is always displayed.
 *
 * @param cursor The cursor from which to get the data for the groups.
 */

namespace Elastos {
namespace Droid {
namespace Widget {

CursorTreeAdapter::CursorTreeAdapter()
    : mAutoRequery(FALSE)
{}

CursorTreeAdapter::CursorTreeAdapter(
    /* [in] */ ICursor* cursor,
    /* [in] */ IContext* context)
{
    Init(cursor, context);
}

/**
 * Constructor.
 *
 * @param cursor The cursor from which to get the data for the groups.
 * @param context The context
 * @param autoRequery If TRUE the adapter will call {@link Cursor#requery()}
 *        on the cursor whenever it changes so the most recent data is
 *        always displayed.
 */
CursorTreeAdapter::CursorTreeAdapter(
    /* [in] */ ICursor* cursor,
    /* [in] */ IContext* context,
    /* [in] */ Boolean autoRequery)
{
    Init(cursor, context, autoRequery);
}

ECode CursorTreeAdapter::Init(
    /* [in] */ ICursor* cursor,
    /* [in] */ IContext* context,
    /* [in] */ Boolean autoRequery)
{
    mContext = context;
    CHandler::New((IHandler**)&mHandler);
    mAutoRequery = autoRequery;

    mGroupCursorHelper = new MyCursorHelper(cursor, this);
    return NOERROR;
}

/**
 * Gets the cursor helper for the children in the given group.
 *
 * @param groupPosition The group whose children will be returned
 * @param requestCursor Whether to request a Cursor via
 *            {@link #getChildrenCursor(Cursor)} (TRUE), or to assume a call
 *            to {@link #setChildrenCursor(Int32, Cursor)} will happen shortly
 *            (FALSE).
 * @return The cursor helper for the children of the given group
 */
AutoPtr<CursorTreeAdapter::MyCursorHelper> CursorTreeAdapter::GetChildrenCursorHelper(
    /* [in] */ Int32 groupPosition,
    /* [in] */ Boolean requestCursor)
{
    Iterator it = mChildrenCursorHelpers.Find(groupPosition);
    AutoPtr<MyCursorHelper> cursorHelper;
    if (it != mChildrenCursorHelpers.End()) {
        cursorHelper = it->mSecond;
    }

    if (cursorHelper == NULL) {
        if (mGroupCursorHelper->MoveTo(groupPosition) == NULL) return NULL;

        AutoPtr<ICursor> cursor = GetChildrenCursor(mGroupCursorHelper->GetCursor());
        cursorHelper = new MyCursorHelper(cursor, this);
        mChildrenCursorHelpers[groupPosition] = cursorHelper;
    }

    return cursorHelper;
}

/**
 * Sets the group Cursor.
 *
 * @param cursor The Cursor to set for the group. If there is an existing cursor
 * it will be closed.
 */
ECode CursorTreeAdapter::SetGroupCursor(
    /* [in] */ ICursor* cursor)
{
    mGroupCursorHelper->ChangeCursor(cursor, FALSE);
    return NOERROR;
}

/**
 * Sets the children Cursor for a particular group. If there is an existing cursor
 * it will be closed.
 * <p>
 * This is useful when asynchronously querying to prevent blocking the UI.
 *
 * @param groupPosition The group whose children are being set via this Cursor.
 * @param childrenCursor The Cursor that contains the children of the group.
 */
ECode CursorTreeAdapter::SetChildrenCursor(
    /* [in] */ Int32 groupPosition,
    /* [in] */ ICursor* childrenCursor)
{
    /*
     * Don't request a cursor from the subclass, instead we will be setting
     * the cursor ourselves.
     */
    AutoPtr<MyCursorHelper> childrenCursorHelper = GetChildrenCursorHelper(groupPosition, FALSE);

    /*
     * Don't release any cursor since we know exactly what data is changing
     * (this cursor, which is still valid).
     */
    childrenCursorHelper->ChangeCursor(childrenCursor, FALSE);

    return NOERROR;
}

AutoPtr<ICursor> CursorTreeAdapter::GetChild(
    /* [in] */ Int32 groupPosition,
    /* [in] */ Int32 childPosition)
{
    // Return this group's children Cursor pointing to the particular child
    return GetChildrenCursorHelper(groupPosition, TRUE)->MoveTo(childPosition);
}

Int64 CursorTreeAdapter::GetChildId(
    /* [in] */ Int32 groupPosition,
    /* [in] */ Int32 childPosition)
{
    return GetChildrenCursorHelper(groupPosition, TRUE)->GetId(childPosition);
}

Int32 CursorTreeAdapter::GetChildrenCount(
    /* [in] */ Int32 groupPosition)
{
    AutoPtr<MyCursorHelper> helper = GetChildrenCursorHelper(groupPosition, TRUE);
    return (mGroupCursorHelper->IsValid() && helper != NULL) ? helper->GetCount() : 0;
}

AutoPtr<ICursor> CursorTreeAdapter::GetGroup(
    /* [in] */ Int32 groupPosition)
{
    // Return the group Cursor pointing to the given group
    return mGroupCursorHelper->MoveTo(groupPosition);
}

Int32 CursorTreeAdapter::GetGroupCount()
{
    return mGroupCursorHelper->GetCount();
}

Int64 CursorTreeAdapter::GetGroupId(
    /* [in] */ Int32 groupPosition)
{
    return mGroupCursorHelper->GetId(groupPosition);
}

AutoPtr<IView> CursorTreeAdapter::GetGroupView(
    /* [in] */ Int32 groupPosition,
    /* [in] */ Boolean isExpanded,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent)
{
    AutoPtr<ICursor> cursor = mGroupCursorHelper->MoveTo(groupPosition);
    assert(cursor != NULL);

    AutoPtr<IView> v;
    if (convertView == NULL) {
        v = NewGroupView(mContext, cursor, isExpanded, parent);
    } else {
        v = convertView;
    }
    BindGroupView(v, mContext, cursor, isExpanded);
    return v;
}

AutoPtr<IView> CursorTreeAdapter::GetChildView(
    /* [in] */ Int32 groupPosition,
    /* [in] */ Int32 childPosition,
    /* [in] */ Boolean isLastChild,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent)
{
    AutoPtr<MyCursorHelper> cursorHelper = GetChildrenCursorHelper(groupPosition, TRUE);

    AutoPtr<ICursor> cursor = cursorHelper->MoveTo(childPosition);
    assert(cursor != NULL);

    AutoPtr<IView> v;
    if (convertView == NULL) {
        v = NewChildView(mContext, cursor, isLastChild, parent);
    } else {
        v = convertView;
    }
    BindChildView(v, mContext, cursor, isLastChild);
    return v;
}

Boolean CursorTreeAdapter::IsChildSelectable(
    /* [in] */ Int32 groupPosition,
    /* [in] */ Int32 childPosition)
{
    return TRUE;
}

Boolean CursorTreeAdapter::HasStableIds()
{
    return TRUE;
}

void CursorTreeAdapter::ReleaseCursorHelpers()
{
    Iterator it = mChildrenCursorHelpers.Begin();
    for (; it != mChildrenCursorHelpers.End(); it++) {
        AutoPtr<MyCursorHelper> helper = it->mSecond;
        helper->Deactivate();
    }

    mChildrenCursorHelpers.Clear();
}

ECode CursorTreeAdapter::NotifyDataSetChanged()
{
    NotifyDataSetChanged(TRUE);

    return NOERROR;
}

/**
 * Notifies a data set change, but with the option of not releasing any
 * cached cursors.
 *
 * @param releaseCursors Whether to release and deactivate any cached
 *            cursors.
 */
ECode CursorTreeAdapter::NotifyDataSetChanged(
    /* [in] */ Boolean releaseCursors)
{
    if (releaseCursors) {
        ReleaseCursorHelpers();
    }

    return BaseExpandableListAdapter::NotifyDataSetChanged();
}

ECode CursorTreeAdapter::NotifyDataSetInvalidated()
{
    ReleaseCursorHelpers();
    return BaseExpandableListAdapter::NotifyDataSetInvalidated();
}

ECode CursorTreeAdapter::OnGroupCollapsed(
    /* [in] */ Int32 groupPosition)
{
    DeactivateChildrenCursorHelper(groupPosition);

    return NOERROR;
}

/**
 * Deactivates the Cursor and removes the helper from cache.
 *
 * @param groupPosition The group whose children Cursor and helper should be
 *            deactivated.
 */
ECode CursorTreeAdapter::DeactivateChildrenCursorHelper(
    /* [in] */ Int32 groupPosition)
{
    AutoPtr<MyCursorHelper> cursorHelper = GetChildrenCursorHelper(groupPosition, TRUE);
    mChildrenCursorHelpers.Erase(groupPosition);
    cursorHelper->Deactivate();

    return NOERROR;
}

/**
 * @see CursorAdapter#convertToString(Cursor)
 */
String CursorTreeAdapter::ConvertToString(
    /* [in] */ ICursor* cursor)
{
    String str;
    return cursor == NULL ? String("") : (/*cursor->ToString(&str),*/ str);
}

/**
 * @see CursorAdapter#runQueryOnBackgroundThread(CharSequence)
 */
AutoPtr<ICursor> CursorTreeAdapter::RunQueryOnBackgroundThread(
    /* [in] */ ICharSequence* constraint)
{
    if (mFilterQueryProvider != NULL) {
        AutoPtr<ICursor> cursor;
        mFilterQueryProvider->RunQuery(constraint, (ICursor**)&cursor);
        return cursor;
    }

    return mGroupCursorHelper->GetCursor();
}

AutoPtr<IFilter> CursorTreeAdapter::GetFilter()
{
    if (mCursorFilter == NULL) {
        CCursorFilter::New(THIS_PROBE(ICursorFilterClient), (ICursorFilter**)&mCursorFilter);
    }
    return mCursorFilter;
}

/**
 * @see CursorAdapter#getFilterQueryProvider()
 */
AutoPtr<IFilterQueryProvider> CursorTreeAdapter::GetFilterQueryProvider()
{
    return mFilterQueryProvider;
}

/**
 * @see CursorAdapter#setFilterQueryProvider(FilterQueryProvider)
 */
ECode CursorTreeAdapter::SetFilterQueryProvider(
    /* [in] */ IFilterQueryProvider* filterQueryProvider)
{
    mFilterQueryProvider = filterQueryProvider;

    return NOERROR;
}

/**
 * @see CursorAdapter#changeCursor(Cursor)
 */
ECode CursorTreeAdapter::ChangeCursor(
    /* [in] */ ICursor* cursor)
{
    mGroupCursorHelper->ChangeCursor(cursor, TRUE);
    return NOERROR;
}

/**
 * @see CursorAdapter#getCursor()
 */
AutoPtr<ICursor> CursorTreeAdapter::GetCursor()
{
    AutoPtr<ICursor> cursor = mGroupCursorHelper->GetCursor();

    return cursor;
}

CursorTreeAdapter::MyCursorHelper::MyCursorHelper(
    /* [in] */ ICursor* cursor,
    /* [in] */ CursorTreeAdapter* owner) : mOwner(owner)
{
    Boolean cursorPresent = cursor != NULL;
    mCursor = cursor;
    mDataValid = cursorPresent;
    mRowIDColumn = -1;
    if(cursorPresent)
        cursor->GetColumnIndex(String("_id"), &mRowIDColumn);

    mContentObserver = new MyContentObserver(this);
    mDataSetObserver = new MyDataSetObserver(this);
    if (cursorPresent) {
        cursor->RegisterContentObserver(mContentObserver);
        cursor->RegisterDataSetObserver(mDataSetObserver);
    }
}

AutoPtr<ICursor> CursorTreeAdapter::MyCursorHelper::GetCursor()
{
    return mCursor;
}

Int32 CursorTreeAdapter::MyCursorHelper::GetCount()
{
    if (mDataValid && mCursor != NULL) {
        Int32 count;
        mCursor->GetCount(&count);
        return count;
    } else {
        return 0;
    }
}

Int64 CursorTreeAdapter::MyCursorHelper::GetId(
    /* [in] */ Int32 position)
{
    if (mDataValid && mCursor != NULL) {
        Boolean res;
        mCursor->MoveToPosition(position, &res);
        if (res) {
            Int64 columnValue;
            mCursor->GetInt64(mRowIDColumn, &columnValue);
            return columnValue;
        } else {
            return 0;
        }
    } else {
        return 0;
    }
}

AutoPtr<ICursor> CursorTreeAdapter::MyCursorHelper::MoveTo(
    /* [in] */ Int32 position)
{
    Boolean res;
    if (mDataValid && (mCursor != NULL) && (mCursor->MoveToPosition(position, &res), res)) {
        return mCursor;
    } else {
        return NULL;
    }
}

void CursorTreeAdapter::MyCursorHelper::ChangeCursor(
    /* [in] */ ICursor* cursor,
    /* [in] */ Boolean releaseCursors)
{
    if (cursor == mCursor) return;

    Deactivate();
    mCursor = cursor;
    if (cursor != NULL) {
        cursor->RegisterContentObserver(mContentObserver);
        cursor->RegisterDataSetObserver(mDataSetObserver);
        cursor->GetColumnIndex(String("_id"), &mRowIDColumn);
        mDataValid = TRUE;
        // notify the observers about the new cursor
        mOwner->NotifyDataSetChanged(releaseCursors);
    } else {
        mRowIDColumn = -1;
        mDataValid = FALSE;
        // notify the observers about the lack of a data set
        mOwner->NotifyDataSetInvalidated();
    }
}

void CursorTreeAdapter::MyCursorHelper::Deactivate()
{
    if (mCursor == NULL) {
        return;
    }

    mCursor->UnregisterContentObserver(mContentObserver);
    mCursor->UnregisterDataSetObserver(mDataSetObserver);
    mCursor->Close();
    mCursor = NULL;
}

Boolean CursorTreeAdapter::MyCursorHelper::IsValid()
{
    return mDataValid && mCursor != NULL;
}

CursorTreeAdapter::MyCursorHelper::MyContentObserver::MyContentObserver(
    /* [in] */ MyCursorHelper* owner)
    : ContentObserver(mHandler)
{
    mOwner = owner;
}

ECode CursorTreeAdapter::MyCursorHelper::MyContentObserver::DeliverSelfNotifications(
    /* [out] */ Boolean* result)
{
    *result = TRUE;
    return NOERROR;
}

ECode CursorTreeAdapter::MyCursorHelper::MyContentObserver::OnChange(
    /* [in] */ Boolean selfChange)
{
    if (mOwner->mOwner->mAutoRequery && mOwner->mCursor != NULL) {
        /*if (Config.LOGV) Log.v("Cursor", "Auto requerying " + mCursor +
                " due to update");*/
        mOwner->mCursor->Requery(&mOwner->mDataValid);
    }

    return NOERROR;
}

CAR_INTERFACE_IMPL(CursorTreeAdapter::MyCursorHelper::MyDataSetObserver, IDataSetObserver)

ECode CursorTreeAdapter::MyCursorHelper::MyDataSetObserver::OnChanged()
{
    mOwner->mDataValid = TRUE;
    return mOwner->mOwner->NotifyDataSetChanged();
}

ECode CursorTreeAdapter::MyCursorHelper::MyDataSetObserver::OnInvalidated()
{
    mOwner->mDataValid = FALSE;
    return mOwner->mOwner->NotifyDataSetInvalidated();
}

} // namespace Widget
} // namespace Droid
} // namespace Elastos
