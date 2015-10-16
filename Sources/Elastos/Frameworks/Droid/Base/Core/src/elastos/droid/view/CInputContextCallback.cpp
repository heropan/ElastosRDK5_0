
#include "elastos/droid/view/CInputContextCallback.h"
#include "elastos/droid/view/LocalInputConnectionWrapper.h"
#include "elastos/droid/os/SystemClock.h"

using Elastos::Droid::Os::SystemClock;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace View {

static AutoPtr<CInputContextCallback> Init()
{
    AutoPtr<CInputContextCallback> callback;
    CInputContextCallback::NewByFriend((CInputContextCallback**)&callback);
    return callback;
}

AutoPtr<CInputContextCallback> CInputContextCallback::sInstance = Init();
Int32 CInputContextCallback::sSequenceNumber = 1;
Mutex CInputContextCallback::sLock;

CInputContextCallback::CInputContextCallback()
    : mSeq(0)
    , mHaveValue(FALSE)
    , mCursorCapsMode(0)
{
    pthread_cond_init(&mWaitCond, NULL);
}

CInputContextCallback::~CInputContextCallback()
{
    pthread_cond_destroy(&mWaitCond);
}

AutoPtr<CInputContextCallback> CInputContextCallback::GetInstance()
{
    AutoLock lock(sLock);

    // Return sInstance if it's non-NULL, otherwise construct a new callback
    AutoPtr<CInputContextCallback> callback;
    if (sInstance != NULL) {
        callback = sInstance;
        sInstance = NULL;

        // Reset the callback
        callback->mHaveValue = FALSE;
    }
    else {
        CInputContextCallback::NewByFriend((CInputContextCallback**)&callback);
    }

    // Set the sequence number
    callback->mSeq = sSequenceNumber++;
    return callback;
}

/**
 * Makes the given InputContextCallback available for use in the future.
 */
void CInputContextCallback::Dispose()
{
    AutoLock lock(sLock);

    // If sInstance is non-NULL, just let this object be garbage-collected
    if (sInstance == NULL) {
        // Allow any objects being held to be gc'ed
        mTextAfterCursor = NULL;
        mTextBeforeCursor = NULL;
        mExtractedText = NULL;
        sInstance = this;
    }
}

ECode CInputContextCallback::SetTextBeforeCursor(
    /* [in] */ ICharSequence* textBeforeCursor,
    /* [in] */ Int32 seq)
{
    AutoLock lock(this);

    if (seq == mSeq) {
        mTextBeforeCursor = textBeforeCursor;
        mHaveValue = TRUE;
        NotifyAll();
    }
    else {
        // Log.i(TAG, "Got out-of-sequence callback " + seq + " (expected " + mSeq
        //         + ") in setTextBeforeCursor, ignoring.");
    }

    return NOERROR;
}

ECode CInputContextCallback::SetTextAfterCursor(
    /* [in] */ ICharSequence* textAfterCursor,
    /* [in] */ Int32 seq)
{
    AutoLock lock(this);

    if (seq == mSeq) {
        mTextAfterCursor = textAfterCursor;
        mHaveValue = TRUE;
        NotifyAll();
    }
    else {
        // Log.i(TAG, "Got out-of-sequence callback " + seq + " (expected " + mSeq
        //         + ") in setTextAfterCursor, ignoring.");
    }

    return NOERROR;
}

ECode CInputContextCallback::SetSelectedText(
    /* [in] */ ICharSequence* selectedText,
    /* [in] */ Int32 seq)
{
    AutoLock lock(this);

    if (seq == mSeq) {
        mSelectedText = selectedText;
        mHaveValue = TRUE;
        NotifyAll();
    }
    else {
        // Log.i(TAG, "Got out-of-sequence callback " + seq + " (expected " + mSeq
        //         + ") in setSelectedText, ignoring.");
    }

    return NOERROR;
}

ECode CInputContextCallback::SetCursorCapsMode(
    /* [in] */ Int32 capsMode,
    /* [in] */ Int32 seq)
{
    AutoLock lock(this);

    if (seq == mSeq) {
        mCursorCapsMode = capsMode;
        mHaveValue = TRUE;
        NotifyAll();
    }
    else {
        // Log.i(TAG, "Got out-of-sequence callback " + seq + " (expected " + mSeq
        //         + ") in setCursorCapsMode, ignoring.");
    }

    return NOERROR;
}

ECode CInputContextCallback::SetExtractedText(
    /* [in] */ IExtractedText* extractedText,
    /* [in] */ Int32 seq)
{
    AutoLock lock(this);

    if (seq == mSeq) {
        mExtractedText = extractedText;
        mHaveValue = TRUE;
        NotifyAll();
    }
    else {
        // Log.i(TAG, "Got out-of-sequence callback " + seq + " (expected " + mSeq
        //         + ") in setExtractedText, ignoring.");
    }

    return NOERROR;
}

ECode CInputContextCallback::GetDescription(
    /* [out] */ String* str)
{
    return E_NOT_IMPLEMENTED;
}

void CInputContextCallback::WaitForResultLocked()
{
    Int64 startTime = SystemClock::GetUptimeMillis();
    Int64 endTime = startTime + LocalInputConnectionWrapper::MAX_WAIT_TIME_MILLIS;

    while (!mHaveValue) {
        Int64 remainingTime = endTime - SystemClock::GetUptimeMillis();
        if (remainingTime <= 0) {
            // Log.w(TAG, "Timed out waiting on IInputContextCallback");
            return;
        }
        // try {
        Wait(remainingTime);
        // } catch (InterruptedException e) {
        // }
    }
}

void CInputContextCallback::Wait(
    /* [in] */ Int64 millis)
{
    struct timeval now;
    gettimeofday(&now, NULL);

    struct timespec timeout;
    timeout.tv_sec = now.tv_sec + millis / 1000;
    timeout.tv_nsec = (now.tv_usec + (millis % 1000) * 1000) * 1000;
    pthread_cond_timedwait(&mWaitCond, &_m_syncLock.mMutex, &timeout);
}

void CInputContextCallback::NotifyAll()
{
    pthread_cond_broadcast(&mWaitCond);
}

} // namespace View
} // namespace Internal
} // namespace Droid
} // namespace Elastos
