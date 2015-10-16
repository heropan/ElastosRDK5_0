
#include "elastos/droid/widget/ViewFlipper.h"
#include "elastos/droid/content/CIntentFilter.h"
#include "elastos/droid/R.h"

using Elastos::Core::CStringWrapper;
using Elastos::Droid::Content::IIntentFilter;
using Elastos::Droid::Content::CIntentFilter;
using Elastos::Droid::View::EIID_View;
using Elastos::Droid::View::EIID_ViewGroup;

namespace Elastos {
namespace Droid {
namespace Widget {

String ViewFlipper::TAG("ViewFlipper");
const Boolean ViewFlipper::LOGD = FALSE;
const Int32 ViewFlipper::DEFAULT_INTERVAL = 3000;
const Int32 ViewFlipper::FLIP_MSG = 1;

ECode ViewFlipper::MyHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    Int32 what;
    msg->GetWhat(&what);

    if (what == ViewFlipper::FLIP_MSG) {
        if (mHost->mRunning) {
            mHost->ShowNext();
            Boolean result FALSE;
            SendEmptyMessageDelayed(ViewFlipper::FLIP_MSG, mHost->mFlipInterval, &result);
        }
    }
    return NOERROR;
}

ViewFlipper::FlipperReceiver::FlipperReceiver(
    /* [in] */ ViewFlipper* host)
    : mHost(host)
{}

ECode ViewFlipper::FlipperReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    assert(intent != NULL);
    String action;
    intent->GetAction(&action);
    if (IIntent::ACTION_SCREEN_OFF.Equals(action)) {
        mHost->mUserPresent = FALSE;
        mHost->UpdateRunning();
    }
    else if (IIntent::ACTION_USER_PRESENT.Equals(action)) {
        mHost->mUserPresent = TRUE;
        mHost->UpdateRunning(FALSE);
    }
    return NOERROR;
}

ViewFlipper::ViewFlipper()
    : mFlipInterval(DEFAULT_INTERVAL)
    , mAutoStart(FALSE)
    , mRunning(FALSE)
    , mStarted(FALSE)
    , mVisible(FALSE)
    , mUserPresent(TRUE)
{
    mReceiver = new FlipperReceiver(this);
    mHandler = new MyHandler(this);
}

ECode ViewFlipper::Init(
    /* [in] */ IContext* ctx)
{
    return ViewAnimator::Init(ctx);
}

ECode ViewFlipper::Init(
    /* [in] */ IContext* ctx,
    /* [in] */ IAttributeSet* attrs)
{
    FAIL_RETURN(ViewAnimator::Init(ctx, attrs));

    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::ViewFlipper),
            ARRAY_SIZE(R::styleable::ViewFlipper));
    AutoPtr<ITypedArray> a;
    ctx->ObtainStyledAttributes(attrs, attrIds, (ITypedArray**)&a);
    a->GetInt32(R::styleable::ViewFlipper_flipInterval, DEFAULT_INTERVAL, &mFlipInterval);
    a->GetBoolean(R::styleable::ViewFlipper_autoStart, FALSE, &mAutoStart);
    a->Recycle();
    return NOERROR;
}

ECode ViewFlipper::OnAttachedToWindow()
{
    ViewAnimator::OnAttachedToWindow();

    // Listen for broadcasts related to user-presence
    AutoPtr<IIntentFilter> filter;
    CIntentFilter::New((IIntentFilter**)&filter);
    filter->AddAction(IIntent::ACTION_SCREEN_OFF);
    filter->AddAction(IIntent::ACTION_USER_PRESENT);
    AutoPtr<IIntent> intent;
    GetContext()->RegisterReceiver(mReceiver, filter, (IIntent**)&intent);

    if (mAutoStart) {
        // Automatically start when requested
        StartFlipping();
    }
    return NOERROR;
}

ECode ViewFlipper::OnDetachedFromWindow()
{
    ViewAnimator::OnDetachedFromWindow();
    mVisible = FALSE;

    GetContext()->UnregisterReceiver(mReceiver);
    UpdateRunning();
    return NOERROR;
}

void ViewFlipper::OnWindowVisibilityChanged(
    /* [in] */ Int32 visibility)
{
    ViewAnimator::OnWindowVisibilityChanged(visibility);
    mVisible = visibility == IView::VISIBLE;
    UpdateRunning(FALSE);
}

ECode ViewFlipper::SetFlipInterval(
    /* [in] */ Int32 milliseconds)
{
    mFlipInterval = milliseconds;
    return NOERROR;
}

ECode ViewFlipper::StartFlipping()
{
    mStarted = TRUE;
    UpdateRunning();
    return NOERROR;
}

ECode ViewFlipper::StopFlipping()
{
    mStarted = FALSE;
    UpdateRunning();
    return NOERROR;
}

ECode ViewFlipper::OnInitializeAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    ViewAnimator::OnInitializeAccessibilityEvent(event);
    AutoPtr<ICharSequence> name;
    CStringWrapper::New(String("CViewFlipper"), (ICharSequence**)&name);
    event->SetClassName(name);
    return NOERROR;
}

ECode ViewFlipper::OnInitializeAccessibilityNodeInfo(
    /* [in] */ IAccessibilityNodeInfo* info)
{
    ViewAnimator::OnInitializeAccessibilityNodeInfo(info);
    AutoPtr<ICharSequence> name;
    CStringWrapper::New(String("CViewFlipper"), (ICharSequence**)&name);
    info->SetClassName(name);
    return NOERROR;
}

void ViewFlipper::UpdateRunning()
{
    UpdateRunning(TRUE);
}

void ViewFlipper::UpdateRunning(
    /* [in] */ Boolean flipNow)
{
    Boolean running = mVisible && mStarted && mUserPresent;
    if (running != mRunning) {
        if (running) {
            ShowOnly(mWhichChild, flipNow);
            Boolean result FALSE;
            mHandler->SendEmptyMessageDelayed(FLIP_MSG, mFlipInterval, &result);
        }
        else {
            mHandler->RemoveMessages(FLIP_MSG);
        }
        mRunning = running;
    }
    // if (LOGD) {
    //     Log.d(TAG, "updateRunning() mVisible=" + mVisible + ", mStarted=" + mStarted
    //             + ", mUserPresent=" + mUserPresent + ", mRunning=" + mRunning);
    // }
}

ECode ViewFlipper::IsFlipping(
    /* [out] */ Boolean* flipping)
{
    *flipping = mStarted;
    return NOERROR;
}

ECode ViewFlipper::SetAutoStart(
    /* [in] */ Boolean autoStart)
{
    mAutoStart = autoStart;
    return NOERROR;
}

ECode ViewFlipper::IsAutoStart(
    /* [out] */ Boolean* start)
{
    *start = mAutoStart;
    return NOERROR;
}

} // namespace Widget
} // namespace Droid
} // namespace Elastos
