
#include "elastos/droid/widget/ZoomButton.h"
#include "elastos/droid/os/CHandler.h"

using Elastos::Core::CStringWrapper;
using Elastos::Droid::Os::CHandler;
using Elastos::Droid::View::IViewOnLongClickListener;
using Elastos::Droid::View::EIID_IViewOnLongClickListener;

namespace Elastos{
namespace Droid{
namespace Widget{

ZoomButton::ZoomButton(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
    : mZoomSpeed(100)
{
    mRunnable = new ZoomButtonRunnable(this);
    Init(context, attrs, defStyle);
}

ZoomButton::ZoomButton()
    : mZoomSpeed(100)
{
    mRunnable = new ZoomButtonRunnable(this);
}

ECode ZoomButton::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    ImageButton::Init(context, attrs, defStyle);
    CHandler::New((IHandler**)&mHandler);
    SetOnLongClickListener((IViewOnLongClickListener*)this->Probe(EIID_IViewOnLongClickListener));
    return NOERROR;
}

Boolean ZoomButton::OnTouchEvent(
    /* [in] */ IMotionEvent* event)
{
    Int32 action = 0;
    event->GetAction(&action);
    if (action == IMotionEvent::ACTION_CANCEL || action == IMotionEvent::ACTION_UP) {
        mIsInLongpress = FALSE;
    }
    return ImageButton::OnTouchEvent(event);
}

ECode ZoomButton::SetZoomSpeed(
    /* [in] */ Int64 speed)
{
    mZoomSpeed = speed;
    return NOERROR;
}

Boolean ZoomButton::OnLongClick(
    /* [in] */ IView* v)
{
    mIsInLongpress = TRUE;
    Boolean res = FALSE;
    mHandler->Post(mRunnable, &res);
    return TRUE;
}

Boolean ZoomButton::OnKeyUp(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    mIsInLongpress = FALSE;
    return ImageButton::OnKeyUp(keyCode, event);
}

ECode ZoomButton::SetEnabled(
    /* [in] */ Boolean enabled)
{
    if (!enabled) {
        SetPressed(FALSE);
    }
    ImageButton::SetEnabled(enabled);
    return NOERROR;
}

Boolean ZoomButton::DispatchUnhandledMove(
    /* [in] */ IView* focused,
    /* [in] */ Int32 direction)
{
    ClearFocus();
    return ImageButton::DispatchUnhandledMove(focused, direction);
}

ECode ZoomButton::OnInitializeAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    ImageButton::OnInitializeAccessibilityEvent(event);
    AutoPtr<ICharSequence> seq;
    FAIL_RETURN(CStringWrapper::New(String("CZoomButton"), (ICharSequence**)&seq));
    event->SetClassName(seq);
    return NOERROR;
}

ECode ZoomButton::OnInitializeAccessibilityNodeInfo(
    /* [in] */ IAccessibilityNodeInfo* info)
{
    ImageButton::OnInitializeAccessibilityNodeInfo(info);
    AutoPtr<ICharSequence> seq;
    FAIL_RETURN(CStringWrapper::New(String("CZoomButton"), (ICharSequence**)&seq));
    info->SetClassName(seq);
    return NOERROR;
}

//=============================================================================
//                  ZoomButton::ZoomButtonRunnable
//=============================================================================
ZoomButton::ZoomButtonRunnable::ZoomButtonRunnable(
    /* [in] */ ZoomButton* host)
    : mHost(host)
{

}

ECode ZoomButton::ZoomButtonRunnable::Run()
{
    if (mHost->HasOnClickListeners() && mHost->mIsInLongpress && mHost->IsEnabled()) {
        mHost->CallOnClick();
        Boolean res = FALSE;
        mHost->mHandler->PostDelayed(this, mHost->mZoomSpeed, &res);
    }
    return NOERROR;
}

} //namespace Widget
} //namespace Droid
} //namespace Elastos

