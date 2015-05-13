
#include "content/CursorEntityIterator.h"

namespace Elastos {
namespace Droid {
namespace Content {

CursorEntityIterator::CursorEntityIterator(
    /* [in] */ ICursor* cursor)
    : mCursor(cursor)
    , mIsClosed(FALSE)
{
    Boolean succeeded = FALSE;
    mCursor->MoveToFirst(&succeeded);
}

CursorEntityIterator::~CursorEntityIterator()
{}

ECode CursorEntityIterator::HasNext(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)
    if (mIsClosed) {
        //throw new IllegalStateException("calling hasNext() when the iterator is closed");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    Boolean ret = FALSE;
    FAIL_RETURN(mCursor->IsAfterLast(&ret));
    *value = !ret;
    return NOERROR;
}

ECode CursorEntityIterator::Next(
    /* [out] */ IEntity** entity)
{
    VALIDATE_NOT_NULL(entity)
    if (mIsClosed) {
        //throw new IllegalStateException("calling next() when the iterator is closed");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    Boolean has = FALSE;
    if (!(HasNext(&has), has)) {
        //throw new IllegalStateException("you may only call next() if hasNext() is true");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    return GetEntityAndIncrementCursor(mCursor, entity);
}

ECode CursorEntityIterator::Remove()
{
    //throw new UnsupportedOperationException("remove not supported by EntityIterators");
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode CursorEntityIterator::Reset()
{
    if (mIsClosed) {
        //throw new IllegalStateException("calling reset() when the iterator is closed");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    Boolean ret = FALSE;
    return mCursor->MoveToFirst(&ret);
}

ECode CursorEntityIterator::Close()
{
    if (mIsClosed) {
        //throw new IllegalStateException("closing when already closed");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    mIsClosed = TRUE;
    return mCursor->Close();
}

} // namespace Content
} // namespace Droid
} // namespace Elastos