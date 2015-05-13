
#ifndef __CINPUTCONTEXTCALLBACK_H__
#define __CINPUTCONTEXTCALLBACK_H__

#include "_CInputContextCallback.h"

using Elastos::Core::ICharSequence;
using Elastos::Droid::View::InputMethod::IExtractedText;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace View {

CarClass(CInputContextCallback)
{
    friend class LocalInputConnectionWrapper;

public:
    /**
     * Returns an InputContextCallback object that is guaranteed not to be in use by
     * any other thread.  The returned object's 'have value' flag is cleared and its expected
     * sequence number is set to a new integer.  We use a sequence number so that replies that
     * occur after a timeout has expired are not interpreted as replies to a later request.
     */
    static CARAPI_(AutoPtr<CInputContextCallback>) GetInstance();

    CInputContextCallback();

    ~CInputContextCallback();

    CARAPI SetTextBeforeCursor(
        /* [in] */ ICharSequence* textBeforeCursor,
        /* [in] */ Int32 seq);

    CARAPI SetTextAfterCursor(
        /* [in] */ ICharSequence* textAfterCursor,
        /* [in] */ Int32 seq);

    CARAPI SetSelectedText(
        /* [in] */ ICharSequence* selectedText,
        /* [in] */ Int32 seq);

    CARAPI SetCursorCapsMode(
        /* [in] */ Int32 capsMode,
        /* [in] */ Int32 seq);

    CARAPI SetExtractedText(
        /* [in] */ IExtractedText* extractedText,
        /* [in] */ Int32 seq);

    CARAPI GetDescription(
        /* [out] */ String* str);

    /**
     * Waits for a result for up to {@link #MAX_WAIT_TIME_MILLIS} milliseconds.
     *
     * <p>The caller must be synchronized on this callback object.
     */
    CARAPI_(void) WaitForResultLocked();

private:
    /**
     * Makes the given InputContextCallback available for use in the future.
     */
    CARAPI_(void) Dispose();

    CARAPI_(void) Wait(
        /* [in] */ Int64 millis);

    CARAPI_(void) NotifyAll();

public:
    Int32 mSeq;
    Boolean mHaveValue;
    AutoPtr<ICharSequence> mTextBeforeCursor;
    AutoPtr<ICharSequence> mTextAfterCursor;
    AutoPtr<ICharSequence> mSelectedText;
    AutoPtr<IExtractedText> mExtractedText;
    Int32 mCursorCapsMode;

private:
    //static CString TAG = "LocalInputConnectionWrapper.ICC";
    // A 'pool' of one InputContextCallback.  Each ICW request will attempt to gain
    // exclusive access to this object.
    static AutoPtr<CInputContextCallback> sInstance;
    static Int32 sSequenceNumber;
    static Mutex sLock;

    pthread_cond_t mWaitCond;
};

} // namespace View
} // namespace Internal
} // namespace Droid
} // namespace Elastos

#endif //__CINPUTCONTEXTCALLBACK_H__
