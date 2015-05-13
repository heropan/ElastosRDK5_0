
#include "widget/CursorAdapter.h"
#include "widget/CCursorFilter.h"
#include "os/CHandler.h"
#include <elastos/Slogger.h>

using Elastos::Utility::Logging::Slogger;
using Elastos::Droid::Os::CHandler;
using Elastos::Droid::Content::EIID_IContext;

namespace Elastos {
namespace Droid {
namespace Widget {

CursorAdapter::CursorAdapter()
    : mDataValid(FALSE)
    , mAutoRequery(FALSE)
    , mRowIDColumn(0)
{
}

CursorAdapter::CursorAdapter(
    /* [in] */ IContext* context,
    /* [in] */ ICursor* c)
    : mDataValid(FALSE)
    , mAutoRequery(FALSE)
    , mRowIDColumn(0)
{
    Init(context, c, ICursorAdapter::FLAG_AUTO_REQUERY);
}

CursorAdapter::CursorAdapter(
    /* [in] */ IContext* context,
    /* [in] */ ICursor* c,
    /* [in] */ Boolean autoRequery)
    : mDataValid(FALSE)
    , mAutoRequery(FALSE)
    , mRowIDColumn(0)
{
    Init(context, c, autoRequery ? ICursorAdapter::FLAG_AUTO_REQUERY : ICursorAdapter::FLAG_REGISTER_CONTENT_OBSERVER);
}

/**
 * Recommended constructor.
 *
 * @param c The cursor from which to get the data.
 * @param context The context
 * @param flags Flags used to determine the behavior of the adapter; may
 * be any combination of {@link #ICursorAdapter::FLAG_AUTO_REQUERY} and
 * {@link #ICursorAdapter::FLAG_REGISTER_CONTENT_OBSERVER}.
 */
CursorAdapter::CursorAdapter(
    /* [in] */ IContext* context,
    /* [in] */ ICursor* c,
    /* [in] */ Int32 flags)
{
    Init(context, c, flags);
}

/**
 * @deprecated Don't use this, use the normal constructor.  This will
 * be removed in the future.
 */
void CursorAdapter::Init(
    /* [in] */ IContext* context,
    /* [in] */ ICursor* c,
    /* [in] */ Boolean autoRequery)
{
    Init(context, c, autoRequery ? ICursorAdapter::FLAG_AUTO_REQUERY : ICursorAdapter::FLAG_REGISTER_CONTENT_OBSERVER);
}

void CursorAdapter::Init(
    /* [in] */ IContext* context,
    /* [in] */ ICursor* c,
    /* [in] */ Int32 flags)
{
    if ((flags & ICursorAdapter::FLAG_AUTO_REQUERY) == ICursorAdapter::FLAG_AUTO_REQUERY) {
        flags |= ICursorAdapter::FLAG_REGISTER_CONTENT_OBSERVER;
        mAutoRequery = TRUE;
    }
    else {
        mAutoRequery = FALSE;
    }
    Boolean cursorPresent = c != NULL;
    mCursor = c;
    mDataValid = cursorPresent;

    assert(context);
    AutoPtr<IWeakReferenceSource> wrs = IWeakReferenceSource::Probe(context);
    assert(wrs != NULL && "Error: Invalid context, IWeakReferenceSource not implemented!");
    wrs->GetWeakReference((IWeakReference**)&mWeakContext);

    Int32 index;
    c->GetColumnIndexOrThrow(String("_id"), &index);
    mRowIDColumn = cursorPresent ? index : -1;
    if ((flags & ICursorAdapter::FLAG_REGISTER_CONTENT_OBSERVER) == ICursorAdapter::FLAG_REGISTER_CONTENT_OBSERVER) {
        mChangeObserver = new ChangeObserver(this);
        mDataSetObserver = new MyDataSetObserver(this);
    }
    else {
        mChangeObserver = NULL;
        mDataSetObserver = NULL;
    }

    if (cursorPresent) {
        if (mChangeObserver != NULL) c->RegisterContentObserver(mChangeObserver);
        if (mDataSetObserver != NULL) c->RegisterDataSetObserver(mDataSetObserver);
    }
}

AutoPtr<ICursor> CursorAdapter::GetCursor()
{
    return mCursor;
}

Int32 CursorAdapter::GetCount()
{
    if (mDataValid && mCursor != NULL) {
        Int32 count;
        mCursor->GetCount(&count);
        return count;
    }
    else {
        return 0;
    }
}

AutoPtr<IInterface> CursorAdapter::GetItem(
    /* [in] */ Int32 position)
{
    if (mDataValid && mCursor != NULL) {
        Boolean res;
        mCursor->MoveToPosition(position, &res);
        return mCursor;
    } else {
        return NULL;
    }
}

Int64 CursorAdapter::GetItemId(
    /* [in] */ Int32 position)
{
    if (mDataValid && mCursor != NULL) {
        Boolean r;
        if (mCursor->MoveToPosition(position, &r)) {
            Int64 res;
            mCursor->GetInt64(mRowIDColumn, &res);
            return res;
        } else {
            return 0;
        }
    } else {
        return 0;
    }
}

Boolean CursorAdapter::HasStableIds()
{
    return TRUE;
}

AutoPtr<IView> CursorAdapter::GetView(
    /* [in] */ Int32 position,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent)
{
    AutoPtr<IView> v;
    if (!mDataValid) {
        Slogger::E("CursorAdapter::GetView", "this should only be called when the cursor is valid");
        return v;
        //throw new IllegalStateException("this should only be called when the cursor is valid");
    }
    Boolean r;
    if (!(mCursor->MoveToPosition(position, &r), r)) {
        Slogger::E("CursorAdapter::GetView", "couldn't move cursor to position  %d", position);
        return v;
        //throw new IllegalStateException("couldn't move cursor to position " + position);
    }

    AutoPtr<IContext> context;
    mWeakContext->Resolve(EIID_IContext, (IInterface**)&context);

    if (context == NULL) {
        Slogger::E("CursorAdapter::GetView", "context has been destoried!");
        return v;
    }

    if (convertView == NULL) {
        v = NewView(context, mCursor, parent);
    }
    else {
        v = convertView;
    }
    BindView(v, context, mCursor);
    return v;
}

AutoPtr<IView> CursorAdapter::GetDropDownView(
    /* [in] */ Int32 position,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent)
{
    AutoPtr<IView> v;
    if (mDataValid) {
        Boolean res;
        mCursor->MoveToPosition(position, &res);

        AutoPtr<IContext> context;
        mWeakContext->Resolve(EIID_IContext, (IInterface**)&context);
        if (context == NULL) {
            Slogger::E("CursorAdapter::GetDropDownView", "context has been destoried!");
            return v;
        }

        if (convertView == NULL) {
            v = NewDropDownView(context, mCursor, parent);
        }
        else {
            v = convertView;
        }
        BindView(v, context, mCursor);
    }

    return v;
}

AutoPtr<IView> CursorAdapter::NewDropDownView(
    /* [in] */ IContext* context,
    /* [in] */ ICursor* cursor,
    /* [in] */ IViewGroup* parent)
{
    return NewView(context, cursor, parent);
}

ECode CursorAdapter::ChangeCursor(
    /* [in] */ ICursor* cursor)
{
    AutoPtr<ICursor> old = SwapCursor(cursor);
    if (old != NULL) {
        old->Close();
    }
    return NOERROR;
}

/**
 * Swap in a new Cursor, returning the old Cursor.  Unlike
 * {@link #changeCursor(Cursor)}, the returned old Cursor is <em>not</em>
 * closed.
 *
 * @param newCursor The new cursor to be used.
 * @return Returns the previously set Cursor, or NULL if there wasa not one.
 * If the given new Cursor is the same instance is the previously set
 * Cursor, NULL is also returned.
 */
AutoPtr<ICursor> CursorAdapter::SwapCursor(
    /* [in] */ ICursor* newCursor)
{
    if (newCursor == mCursor) {
        return NULL;
    }
    AutoPtr<ICursor> oldCursor = mCursor;
    if (oldCursor != NULL) {
        if (mChangeObserver != NULL) oldCursor->UnregisterContentObserver(mChangeObserver);
        if (mDataSetObserver != NULL) oldCursor->UnregisterDataSetObserver(mDataSetObserver);
    }
    mCursor = newCursor;
    if (newCursor != NULL) {
        if (mChangeObserver != NULL) newCursor->RegisterContentObserver(mChangeObserver);
        if (mDataSetObserver != NULL) newCursor->RegisterDataSetObserver(mDataSetObserver);
        newCursor->GetColumnIndexOrThrow(String("_id"), &mRowIDColumn);
        mDataValid = TRUE;
        // notify the observers about the new cursor
        NotifyDataSetChanged();
    } else {
        mRowIDColumn = -1;
        mDataValid = FALSE;
        // notify the observers about the lack of a data set
        NotifyDataSetInvalidated();
    }
    return oldCursor;
}

AutoPtr<ICharSequence> CursorAdapter::ConvertToString(
    /* [in] */ ICursor* cursor)
{
//    return cursor == NULL ? "" : cursor->ToString();
    return NULL;
}

AutoPtr<ICursor> CursorAdapter::RunQueryOnBackgroundThread(
    /* [in] */ ICharSequence* constraint)
{
    if (mFilterQueryProvider != NULL) {
        AutoPtr<ICursor> c;
        mFilterQueryProvider->RunQuery(constraint, (ICursor**)&c);
        return c;
    }

    return mCursor;
}

AutoPtr<IFilter> CursorAdapter::GetFilter()
{
    if(mCursorFilter == NULL) {
        CCursorFilter::New(
                (ICursorFilterClient*)this->Probe(EIID_ICursorFilterClient),
                (ICursorFilter**)&mCursorFilter);
    }
    return mCursorFilter;
}

AutoPtr<IFilterQueryProvider> CursorAdapter::GetFilterQueryProvider()
{
    return mFilterQueryProvider;
}

ECode CursorAdapter::SetFilterQueryProvider(
    /* [in] */ IFilterQueryProvider* filterQueryProvider)
{
    mFilterQueryProvider = filterQueryProvider;

    return NOERROR;
}

void CursorAdapter::OnContentChanged()
{
    Boolean closed;
    if (mAutoRequery && mCursor != NULL && !(mCursor->IsClosed(&closed), closed)) {
        //if (FALSE) Log.v("Cursor", "Auto requerying " + mCursor + " due to update");
        mCursor->Requery(&mDataValid);
    }
}

CursorAdapter::ChangeObserver::ChangeObserver(
    /* [in] */ CursorAdapter* host)
    : ContentObserver(InitSuperHandler())
    , mHost(host)
{}

ECode CursorAdapter::ChangeObserver::DeliverSelfNotifications(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = TRUE;
    return NOERROR;
}

ECode CursorAdapter::ChangeObserver::OnChange(
    /* [in] */ Boolean selfChange)
{
    mHost->OnContentChanged();

    return NOERROR;
}

AutoPtr<IHandler> CursorAdapter::ChangeObserver::InitSuperHandler()
{
    AutoPtr<IHandler> handler;
    CHandler::New((IHandler**)&handler);
    return handler;
}

CAR_INTERFACE_IMPL(CursorAdapter::MyDataSetObserver, IDataSetObserver)

CursorAdapter::MyDataSetObserver::MyDataSetObserver(
    /* [in] */ CursorAdapter* host)
    : mHost(host)
{}

ECode CursorAdapter::MyDataSetObserver::OnChanged()
{
    mHost->mDataValid = TRUE;
    mHost->NotifyDataSetChanged();
    return NOERROR;
}

ECode CursorAdapter::MyDataSetObserver::OnInvalidated()
{
    mHost->mDataValid = FALSE;
    mHost->NotifyDataSetInvalidated();
    return NOERROR;
}

}// namespace Elastos
}// namespace Droid
}// namespace Widget
