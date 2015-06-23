
#include "CSkbContainer.h"
#include "view/CMotionEventHelper.h"
#include "os/SystemClock.h"
#include "os/CSystemProperties.h"
#include "widget/CPopupWindow.h"
#include "SoftKeyboard.h"
#include "SkbPool.h"
#include "CPinyinEnvironmentHelper.h"
#include "CBalloonHint.h"
#include "CSoftKeyboardView.h"
#include <elastos/core/Math.h>

using Elastos::Droid::Os::SystemClock;
using Elastos::Droid::Os::ISystemProperties;
using Elastos::Droid::Os::CSystemProperties;
using Elastos::Droid::View::EIID_View;
using Elastos::Droid::View::EIID_IView;
using Elastos::Droid::View::EIID_ViewGroup;
using Elastos::Droid::View::EIID_IViewOnTouchListener;
using Elastos::Droid::View::CMotionEventHelper;
using Elastos::Droid::View::IMotionEventHelper;
using Elastos::Droid::View::IGravity;
using Elastos::Droid::Widget::CPopupWindow;

namespace Elastos {
namespace Droid {
namespace Inputmethods {
namespace PinyinIME {

const Int32 SkbContainer::LongPressTimer::LONG_PRESS_TIMEOUT1 = 500;
const Int32 SkbContainer::LongPressTimer::LONG_PRESS_TIMEOUT2 = 100;
const Int32 SkbContainer::LongPressTimer::LONG_PRESS_TIMEOUT3 = 100;
const Int32 SkbContainer::LongPressTimer::LONG_PRESS_KEYNUM1 = 1;
const Int32 SkbContainer::LongPressTimer::LONG_PRESS_KEYNUM2 = 3;

SkbContainer::LongPressTimer::LongPressTimer(
    /* [in] */ SkbContainer* skbContainer)
    : mSkbContainer(skbContainer)
    , mResponseTimes(0)
{
}

void SkbContainer::LongPressTimer::StartTimer()
{
    Boolean result;
    PostDelayed(this, LONG_PRESS_TIMEOUT1, &result);
    mResponseTimes = 0;
}

Boolean SkbContainer::LongPressTimer::RemoveTimer()
{
    RemoveCallbacks(this);
    return TRUE;
}

ECode SkbContainer::LongPressTimer::Run()
{
    if (mSkbContainer->mWaitForTouchUp) {
        mResponseTimes++;
        if (mSkbContainer->mSoftKeyDown->Repeatable()) {
            Boolean result = FALSE;
            if (mSkbContainer->mSoftKeyDown->IsUserDefKey(&result), result) {
                if (1 == mResponseTimes) {
                    Boolean result = FALSE;
                    if (mSkbContainer->mInputModeSwitcher->TryHandleLongPressSwitch(mSkbContainer->mSoftKeyDown->mKeyCode, &result), result ) {
                        mSkbContainer->mDiscardEvent = TRUE;
                        mSkbContainer->ResetKeyPress(0);
                    }
                }
            }
            else {
                mSkbContainer->ResponseKeyEvent(mSkbContainer->mSoftKeyDown);
                Int64 timeout;
                if (mResponseTimes < LONG_PRESS_KEYNUM1) {
                    timeout = LONG_PRESS_TIMEOUT1;
                }
                else if (mResponseTimes < LONG_PRESS_KEYNUM2) {
                    timeout = LONG_PRESS_TIMEOUT2;
                }
                else {
                    timeout = LONG_PRESS_TIMEOUT3;
                }

                Boolean result;
                PostDelayed(this, timeout, &result);
            }
        }
        else {
            if (1 == mResponseTimes) {
                mSkbContainer->PopupSymbols();
            }
        }
    }

    return NOERROR;
}


//class SkbContainer
const Int32 SkbContainer::Y_BIAS_CORRECTION = -10;
const Int32 SkbContainer::MOVE_TOLERANCE = 6;
Boolean SkbContainer::POPUPWINDOW_FOR_PRESSED_UI = FALSE;

SkbContainer::SkbContainer()
    : mSkbLayout(0)
    , mLastCandidatesShowing(FALSE)
    , mPopupSkbShow(FALSE)
    , mPopupSkbNoResponse(FALSE)
    , mPopupX(0)
    , mPopupY(0)
    , mWaitForTouchUp(FALSE)
    , mDiscardEvent(FALSE)
    , mYBiasCorrection(0)
    , mXLast(0)
    , mYLast(0)
{
    memset(mSkvPosInContainer, 0, sizeof(mSkvPosInContainer));
    memset(mXyPosTmp, 0, sizeof(mXyPosTmp));
}

ECode SkbContainer::SetService(
    /* [in] */ IInputMethodService* service)
{
    mService = service;
    return NOERROR;
}

ECode SkbContainer::SetInputModeSwitcher(
    /* [in] */ IInputModeSwitcher* inputModeSwitcher)
{
    mInputModeSwitcher = inputModeSwitcher;
    return NOERROR;
}

ECode SkbContainer::SetGestureDetector(
    /* [in] */ IGestureDetector* gestureDetector)
{
    mGestureDetector = gestureDetector;
    return NOERROR;
}

ECode SkbContainer::IsCurrentSkbSticky(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    if (NULL == mMajorView) {
        *result = TRUE;
        return NOERROR;
    }

    AutoPtr<ISoftKeyboard> skb;
    mMajorView->GetSoftKeyboard((ISoftKeyboard**)&skb);
    if (NULL != skb) {
        *result = ((SoftKeyboard*)skb.Get())->GetStickyFlag();
    }

    *result = TRUE;
    return NOERROR;
}

ECode SkbContainer::ToggleCandidateMode(
    /* [in] */ Boolean candidatesShowing)
{
    Boolean result = FALSE;
    if (NULL == mMajorView || !(mInputModeSwitcher->IsChineseText(&result), result)
            || mLastCandidatesShowing == candidatesShowing) return NOERROR;
    mLastCandidatesShowing = candidatesShowing;

    AutoPtr<ISoftKeyboard> skb;
    mMajorView->GetSoftKeyboard((ISoftKeyboard**)&skb);
    if (NULL == skb) return NOERROR;

    Int32 state = 0;
    mInputModeSwitcher->GetTooggleStateForCnCand(&state);
    if (!candidatesShowing) {
        ((SoftKeyboard*)skb.Get())->DisableToggleState(state, FALSE);
        AutoPtr<IToggleStates> states;
        mInputModeSwitcher->GetToggleStates((IToggleStates**)&states);
        ((SoftKeyboard*)skb.Get())->EnableToggleStates(states);
    } else {
        ((SoftKeyboard*)skb.Get())->EnableToggleState(state, FALSE);
    }

    return mMajorView->Invalidate();
}

ECode SkbContainer::UpdateInputMode()
{
    Int32 skbLayout = 0;
    mInputModeSwitcher->GetSkbLayout(&skbLayout);
    if (mSkbLayout != skbLayout) {
        mSkbLayout = skbLayout;
        UpdateSkbLayout();
    }

    mLastCandidatesShowing = FALSE;

    if (NULL == mMajorView) return NOERROR;

    AutoPtr<ISoftKeyboard> skb;
    mMajorView->GetSoftKeyboard((ISoftKeyboard**)&skb);
    if (NULL == skb) return NOERROR;
    AutoPtr<IToggleStates> states;
    mInputModeSwitcher->GetToggleStates((IToggleStates**)&states);
    ((SoftKeyboard*)skb.Get())->EnableToggleStates(states);
    return Invalidate();
}

void SkbContainer::UpdateSkbLayout()
{
    Int32 screenWidth = 0;
    mEnvironment->GetScreenWidth(&screenWidth);
    Int32 keyHeight = 0;
    mEnvironment->GetKeyHeight(&keyHeight);
    Int32 skbHeight = 0;
    mEnvironment->GetSkbHeight(&skbHeight);

    AutoPtr<IResources> r;
    mContext->GetResources((IResources**)&r);
    if (NULL == mSkbFlipper) {
        mSkbFlipper = (IViewFlipper*) FindViewById(0x7f0a0008/*R.id.alpha_floatable*/).Get();
    }

    AutoPtr<IView> view;
    mSkbFlipper->GetChildAt(0, (IView**)&view);
    mMajorView = ISoftKeyboardView::Probe(view);

    AutoPtr<SoftKeyboard> majorSkb;
    AutoPtr<SkbPool> skbPool = SkbPool::GetInstance();

    switch (mSkbLayout) {
    case 0x7f040003/*R.xml.skb_qwerty*/:
        majorSkb = skbPool->GetSoftKeyboard(0x7f040003/*R.xml.skb_qwerty*/,
                0x7f040003/*R.xml.skb_qwerty*/, screenWidth, skbHeight, mContext);
        break;

    case 0x7f040005/*R.xml.skb_sym1*/:
        majorSkb = skbPool->GetSoftKeyboard(0x7f040005/*R.xml.skb_sym1*/, 0x7f040005/*R.xml.skb_sym1*/,
                screenWidth, skbHeight, mContext);
        break;

    case 0x7f040006/*R.xml.skb_sym2*/:
        majorSkb = skbPool->GetSoftKeyboard(0x7f040006/*R.xml.skb_sym2*/, 0x7f040006/*R.xml.skb_sym2*/,
                screenWidth, skbHeight, mContext);
        break;

    case 0x7f040004/*R.xml.skb_smiley*/:
        majorSkb = skbPool->GetSoftKeyboard(0x7f040004/*R.xml.skb_smiley*/,
                0x7f040004/*R.xml.skb_smiley*/, screenWidth, skbHeight, mContext);
        break;

    case 0x7f040002/*R.xml.skb_phone*/:
        majorSkb = skbPool->GetSoftKeyboard(0x7f040002/*R.xml.skb_phone*/,
                0x7f040002/*R.xml.skb_phone*/, screenWidth, skbHeight, mContext);
        break;
    // default:
    }

    Boolean result = FALSE;
    if (NULL == majorSkb || !(mMajorView->SetSoftKeyboard(majorSkb, &result), result)) {
        return;
    }
    mMajorView->SetBalloonHint(mBalloonOnKey, mBalloonPopup, FALSE);
    mMajorView->Invalidate();
}

void SkbContainer::ResponseKeyEvent(
    /* [in] */ SoftKey* sKey)
{
    if (NULL == sKey) return;
    (IPinyinIME::Probe(mService))->ResponseSoftKeyEvent(sKey);
}

AutoPtr<ISoftKeyboardView> SkbContainer::InKeyboardView(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ ArrayOf<Int32>* positionInParent)
{
    if (mPopupSkbShow) {
        Int32 value = 0;
        if (mPopupX <= x && mPopupX + (mPopupSkb->GetWidth(&value), value) > x
                && mPopupY <= y && mPopupY + (mPopupSkb->GetHeight(&value), value) > y) {
            assert(positionInParent != NULL);
            (*positionInParent)[0] = mPopupX;
            (*positionInParent)[1] = mPopupY;
            mPopupSkbView->SetOffsetToSkbContainer(positionInParent);
            return mPopupSkbView;
        }
        return NULL;
    }

    return mMajorView;
}

void SkbContainer::PopupSymbols()
{
    Int32 popupResId = mSoftKeyDown->GetPopupResId();
    if (popupResId > 0) {
        Int32 skbContainerWidth = GetWidth();
        Int32 skbContainerHeight = GetHeight();
        // The paddings of the background are not included.
        Int32 miniSkbWidth = (Int32) (skbContainerWidth * 0.8);
        Int32 miniSkbHeight = (Int32) (skbContainerHeight * 0.23);

        AutoPtr<SkbPool> skbPool = SkbPool::GetInstance();
        AutoPtr<SoftKeyboard> skb = skbPool->GetSoftKeyboard(popupResId, popupResId,
                miniSkbWidth, miniSkbHeight, mContext);
        if (NULL == skb) return;

        mPopupX = (skbContainerWidth - skb->GetSkbTotalWidth()) / 2;
        mPopupY = (skbContainerHeight - skb->GetSkbTotalHeight()) / 2;

        if (NULL == mPopupSkbView) {
            CSoftKeyboardView::New(mContext, NULL, (ISoftKeyboardView**)&mPopupSkbView);
            ((CSoftKeyboardView*)mPopupSkbView.Get())->OnMeasure(IViewGroupLayoutParams::WRAP_CONTENT,
                    IViewGroupLayoutParams::WRAP_CONTENT);
        }
        mPopupSkbView->SetOnTouchListener((IViewOnTouchListener*)this->Probe(EIID_IViewOnTouchListener));
        Boolean result = FALSE;
        mPopupSkbView->SetSoftKeyboard(skb, &result);
        mPopupSkbView->SetBalloonHint(mBalloonOnKey, mBalloonPopup, TRUE);

        mPopupSkb->SetContentView(mPopupSkbView);
        Int32 value = 0;
        mPopupSkb->SetWidth(skb->GetSkbCoreWidth()
                + (mPopupSkbView->GetPaddingLeft(&value), value)
                + (mPopupSkbView->GetPaddingRight(&value), value));
        mPopupSkb->SetHeight(skb->GetSkbCoreHeight()
                + (mPopupSkbView->GetPaddingTop(&value), value)
                + (mPopupSkbView->GetPaddingBottom(&value), value));

        Int32 x = 0, y = 0;
        GetLocationInWindow(&x, &y);
        mXyPosTmp[0] = x;
        mXyPosTmp[1] = y;
        mPopupSkb->ShowAtLocation(THIS_PROBE(IView), IGravity::NO_GRAVITY, mPopupX, mPopupY
                + mXyPosTmp[1]);
        mPopupSkbShow = TRUE;
        mPopupSkbNoResponse = TRUE;
        // Invalidate itself to dim the current soft keyboards.
        DimSoftKeyboard(TRUE);
        ResetKeyPress(0);
    }
}

void SkbContainer::DimSoftKeyboard(
    /* [in] */ Boolean dimSkb)
{
    mMajorView->DimSoftKeyboard(dimSkb);
}

void SkbContainer::DismissPopupSkb()
{
    mPopupSkb->Dismiss();
    mPopupSkbShow = FALSE;
    DimSoftKeyboard(FALSE);
    ResetKeyPress(0);
}

void SkbContainer::ResetKeyPress(
    /* [in] */ Int64 delay)
{
    mLongPressTimer->RemoveTimer();

    if (NULL != mSkv) {
        mSkv->ResetKeyPress(delay);
    }
}

ECode SkbContainer::HandleBack(
    /* [in] */ Boolean realAction,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    if (mPopupSkbShow) {
        if (!realAction) {
            *result = TRUE;
            return NOERROR;
        }

        DismissPopupSkb();
        mDiscardEvent = TRUE;
        *result = TRUE;
        return NOERROR;
    }
    *result = FALSE;
    return NOERROR;
}

ECode SkbContainer::DismissPopups()
{
    Boolean result = FALSE;
    HandleBack(TRUE, &result);
    ResetKeyPress(0);
    return NOERROR;
}

void SkbContainer::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    AutoPtr<IPinyinEnvironmentHelper> helper;
    CPinyinEnvironmentHelper::AcquireSingleton((IPinyinEnvironmentHelper**)&helper);
    AutoPtr<IPinyinEnvironment> env;
    helper->GetInstance((IPinyinEnvironment**)&env);
    Int32 measuredWidth = 0, value = 0;
    env->GetScreenWidth(&measuredWidth);
    Int32 measuredHeight = GetPaddingTop();
    measuredHeight += (env->GetSkbHeight(&value), value);
    widthMeasureSpec = MeasureSpec::MakeMeasureSpec(measuredWidth,
            MeasureSpec::EXACTLY);
    heightMeasureSpec = MeasureSpec::MakeMeasureSpec(measuredHeight,
            MeasureSpec::EXACTLY);
    RelativeLayout::OnMeasure(widthMeasureSpec, heightMeasureSpec);
}

Boolean SkbContainer::OnTouchEvent(
    /* [in] */ IMotionEvent* event)
{
    RelativeLayout::OnTouchEvent(event);

    Boolean flipping = FALSE;
    if (mSkbFlipper->IsFlipping(&flipping), flipping) {
        ResetKeyPress(0);
        return TRUE;
    }

    Float value = 0.f;
    Int32 x = (Int32) (event->GetX(&value), value);
    Int32 y = (Int32) (event->GetY(&value), value);
    // Bias correction
    y = y + mYBiasCorrection;

    // Ignore short-distance movement event to get better performance.
    Int32 action = 0;
    if ((event->GetAction(&action), action) == IMotionEvent::ACTION_MOVE) {
        if (Elastos::Core::Math::Abs(x - mXLast) <= MOVE_TOLERANCE
                && Elastos::Core::Math::Abs(y - mYLast) <= MOVE_TOLERANCE) {
            return TRUE;
        }
    }

    mXLast = x;
    mYLast = y;

    if (!mPopupSkbShow) {
        Boolean result = FALSE;
        if (mGestureDetector->OnTouchEvent(event, &result), result) {
            ResetKeyPress(0);
            mDiscardEvent = TRUE;
            return TRUE;
        }
    }

    switch (action) {
        case IMotionEvent::ACTION_DOWN: {
            ResetKeyPress(0);

            mWaitForTouchUp = TRUE;
            mDiscardEvent = FALSE;

            mSkv = NULL;
            mSoftKeyDown = NULL;
            AutoPtr<ArrayOf<Int32> > pos = ArrayOf<Int32>::Alloc(2);
            (*pos)[0] = mSkvPosInContainer[0];
            (*pos)[1] = mSkvPosInContainer[1];
            mSkv = InKeyboardView(x, y, pos);
            if (NULL != mSkv) {
                AutoPtr<ISoftKey> key;
                mSkv->OnKeyPress(x - mSkvPosInContainer[0], y
                        - mSkvPosInContainer[1], mLongPressTimer, FALSE, (ISoftKey**)&key);
                mSoftKeyDown = (SoftKey*)key.Get();
            }
            break;
        }

        case IMotionEvent::ACTION_MOVE: {
            if (x < 0 || x >= GetWidth() || y < 0 || y >= GetHeight()) {
                break;
            }
            if (mDiscardEvent) {
                ResetKeyPress(0);
                break;
            }

            if (mPopupSkbShow && mPopupSkbNoResponse) {
                break;
            }

            AutoPtr<ArrayOf<Int32> > pos = ArrayOf<Int32>::Alloc(2);
            (*pos)[0] = mSkvPosInContainer[0];
            (*pos)[1] = mSkvPosInContainer[1];
            AutoPtr<ISoftKeyboardView> skv = InKeyboardView(x, y, pos);
            if (NULL != skv) {
                if (skv != mSkv) {
                    mSkv = skv;
                    AutoPtr<ISoftKey> key;
                    mSkv->OnKeyPress(x - mSkvPosInContainer[0], y
                            - mSkvPosInContainer[1], mLongPressTimer, TRUE, (ISoftKey**)&key);
                    mSoftKeyDown = (SoftKey*)key.Get();
                } else if (NULL != skv) {
                    if (NULL != mSkv) {
                        AutoPtr<ISoftKey> key;
                        mSkv->OnKeyMove(x - mSkvPosInContainer[0], y - mSkvPosInContainer[1], (ISoftKey**)&key);
                        mSoftKeyDown = (SoftKey*)key.Get();
                        if (NULL == mSoftKeyDown) {
                            mDiscardEvent = TRUE;
                        }
                    }
                }
            }
            break;
        }

        case IMotionEvent::ACTION_UP: {
            if (mDiscardEvent) {
                ResetKeyPress(0);
                break;
            }

            mWaitForTouchUp = FALSE;

            // The view which got the {@link MotionEvent#ACTION_DOWN} event is
            // always used to handle this event.
            if (NULL != mSkv) {
                AutoPtr<ISoftKey> key;
                mSkv->OnKeyRelease(x - mSkvPosInContainer[0], y - mSkvPosInContainer[1], (ISoftKey**)&key);
            }

            if (!mPopupSkbShow || !mPopupSkbNoResponse) {
                ResponseKeyEvent(mSoftKeyDown);
            }

            if (mSkv == mPopupSkbView && !mPopupSkbNoResponse) {
                DismissPopupSkb();
            }
            mPopupSkbNoResponse = FALSE;
            break;
        }

        case IMotionEvent::ACTION_CANCEL:{
            break;
        }
    }

    if (NULL == mSkv) {
        return FALSE;
    }

    return TRUE;
}


//class CSkbContainer
IVIEW_METHODS_IMPL(CSkbContainer, SkbContainer)
IVIEWGROUP_METHODS_IMPL(CSkbContainer, SkbContainer)
IVIEWPARENT_METHODS_IMPL(CSkbContainer, SkbContainer)
IVIEWMANAGER_METHODS_IMPL(CSkbContainer, SkbContainer)
IDRAWABLECALLBACK_METHODS_IMPL(CSkbContainer, SkbContainer)
IKEYEVENTCALLBACK_METHODS_IMPL(CSkbContainer, SkbContainer)
IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CSkbContainer, SkbContainer)

PInterface CSkbContainer::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    else if (riid == EIID_ViewGroup) {
        return reinterpret_cast<PInterface>((ViewGroup*)this);
    }
    return _CSkbContainer::Probe(riid);
}

ECode CSkbContainer::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    SkbContainer::Init(context);

    AutoPtr<IPinyinEnvironmentHelper> helper;
    CPinyinEnvironmentHelper::AcquireSingleton((IPinyinEnvironmentHelper**)&helper);
    helper->GetInstance((IPinyinEnvironment**)&mEnvironment);

    mLongPressTimer = new LongPressTimer(this);

    // If it runs on an emulator, no bias correction
    AutoPtr<ISystemProperties> sp;
    CSystemProperties::AcquireSingleton((ISystemProperties**)&sp);
    String temp;
    sp->Get(String("ro.kernel.qemu"), &temp);

    if (String("1").Equals(temp)) {
        mYBiasCorrection = 0;
    } else {
        mYBiasCorrection = Y_BIAS_CORRECTION;
    }
    CBalloonHint::New(context, this, MeasureSpec::AT_MOST, (IBalloonHint**)&mBalloonPopup);
    if (POPUPWINDOW_FOR_PRESSED_UI) {
        CBalloonHint::New(context, this, MeasureSpec::AT_MOST, (IBalloonHint**)&mBalloonOnKey);
    }

    CPopupWindow::New(mContext, (IPopupWindow**)&mPopupSkb);
    mPopupSkb->SetBackgroundDrawable(NULL);
    return mPopupSkb->SetClippingEnabled(FALSE);
}

ECode CSkbContainer::SetIgnoreGravity(
    /* [in] */ Int32 viewId)
{
    return SkbContainer::SetIgnoreGravity(viewId);
}

ECode CSkbContainer::GetGravity(
    /* [out] */ Int32* gravity)
{
    VALIDATE_NOT_NULL(gravity);
    *gravity = SkbContainer::GetGravity();
    return NOERROR;
}

ECode CSkbContainer::SetGravity(
    /* [in] */ Int32 gravity)
{
    return SkbContainer::SetGravity(gravity);
}

ECode CSkbContainer::SetHorizontalGravity(
    /* [in] */ Int32 horizontalGravity)
{
    return SkbContainer::SetHorizontalGravity(horizontalGravity);
}

ECode CSkbContainer::SetVerticalGravity(
    /* [in] */ Int32 verticalGravity)
{
    return SkbContainer::SetVerticalGravity(verticalGravity);
}

ECode CSkbContainer::SetService(
    /* [in] */ IInputMethodService* service)
{
    return SkbContainer::SetService(service);
}

ECode CSkbContainer::SetInputModeSwitcher(
    /* [in] */ IInputModeSwitcher* inputModeSwitcher)
{
    return SkbContainer::SetInputModeSwitcher(inputModeSwitcher);
}

ECode CSkbContainer::SetGestureDetector(
    /* [in] */ IGestureDetector* gestureDetector)
{
    return SkbContainer::SetGestureDetector(gestureDetector);
}

ECode CSkbContainer::IsCurrentSkbSticky(
    /* [out] */ Boolean* result)
{
    return SkbContainer::IsCurrentSkbSticky(result);
}

ECode CSkbContainer::ToggleCandidateMode(
    /* [in] */ Boolean candidatesShowing)
{
    return SkbContainer::ToggleCandidateMode(candidatesShowing);
}

ECode CSkbContainer::UpdateInputMode()
{
    return SkbContainer::UpdateInputMode();
}

ECode CSkbContainer::HandleBack(
    /* [in] */ Boolean realAction,
    /* [out] */ Boolean* result)
{
    return SkbContainer::HandleBack(realAction, result);
}

ECode CSkbContainer::DismissPopups()
{
    return SkbContainer::DismissPopups();
}

ECode CSkbContainer::OnTouch(
    /* [in] */ IView* v,
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    // Translate the event to fit to the container.
    AutoPtr<IMotionEventHelper> helper;
    CMotionEventHelper::AcquireSingleton((IMotionEventHelper**)&helper);
    AutoPtr<IMotionEvent> newEv;
    Int64 tm = 0;
    Int32 value = 0;
    Float fv = 0.f;
    helper->Obtain(
            (event->GetDownTime(&tm), tm),
            (event->GetEventTime(&tm), tm),
            (event->GetAction(&value), value),
            (event->GetX(&fv), fv) + mPopupX,
            (event->GetY(&fv), fv) + mPopupY,
            (event->GetPressure(&fv), fv),
            (event->GetSize(&fv), fv),
            (event->GetMetaState(&value), value),
            (event->GetXPrecision(&fv), fv),
            (event->GetYPrecision(&fv), fv),
            (event->GetDeviceId(&value), value),
            (event->GetEdgeFlags(&value), value),
            (IMotionEvent**)&newEv);

    *result = SkbContainer::OnTouchEvent(newEv);
    return NOERROR;
}

} // namespace PinyinIME
} // namespace Inputmethods
} // namespace Droid
} // namespace Elastos
